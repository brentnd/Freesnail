#include "Config.h"
#include "buffer.h"

// Circular buffers for transmit and receive
#define BUFLEN 1024

static uint8_t _tx_buffer[sizeof(RingBuffer) + BUFLEN] __attribute__ ((aligned(4)));
static uint8_t _rx_buffer[sizeof(RingBuffer) + BUFLEN] __attribute__ ((aligned(4)));

static RingBuffer *const tx_buffer = (RingBuffer *) &_tx_buffer;
static RingBuffer *const rx_buffer = (RingBuffer *) &_rx_buffer;

void UART0_IRQHandler()
{
    int status;
    
    status = UART0_S1;
    
    // If transmit data register empty, and data in the transmit buffer,
    // send it.  If it leaves the buffer empty, disable the transmit interrupt.
    if ((status & UART_S1_TDRE_MASK) && !buf_isempty(tx_buffer)) {
        UART0_D = buf_get_byte(tx_buffer);
        if(buf_isempty(tx_buffer))
            UART0_C2 &= ~UART_C2_TIE_MASK;
    }
    
    // If there is received data, read it into the receive buffer.  If the
    // buffer is full, disable the receive interrupt.
    if ((status & UART_S1_RDRF_MASK)) { // && !buf_isfull(rx_buffer)) {
        buf_put_byte(rx_buffer, UART0_D);
        if(buf_isfull(rx_buffer))
            UART0_C2 &= ~UART_C2_RIE_MASK;
    }
}


void uart_putchar(char x)
{
  uart_write(&x,1);
}

int uart_write(char *p, int len)
{
    int i;
    
    for(i=0; i<len; i++) {
        while(buf_isfull(tx_buffer))        // Spin wait while full
            ;
        buf_put_byte(tx_buffer, *p++);
        UART0_C2 |= UART_C2_TIE_MASK;           // Turn on Tx interrupts
    }
    return len;
}

// A blocking write, useful for error/crash/debug reporting
int uart_write_err(char *p, int len)
{
    int i;
    
    __disable_irq();
    for(i=0; i<len; i++) {
        while((UART0_S1 & UART_S1_TDRE_MASK) == 0)  // Wait until transmit buffer empty
            ;
        
        UART0_D = *p++;                     // Send char
    }
    __enable_irq();
    return len;
}

int uart_rx(int num)
{
  return buf_len(rx_buffer) >= num;
}

int uart_read(char *p, int len)
{
    int i = len;

    while(i > 0) {
        while(buf_isempty(rx_buffer))           // Spin wait
            ;

        *p++ = buf_get_byte(rx_buffer);
        UART0_C2 |= UART_C2_RIE_MASK;           // Turn on Rx interrupt
        i--;
    }
    return len - i;
}

void InitUARTs() {
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;

  PORTA_PCR1 = PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
  PORTA_PCR2 = PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
  
  SIM_SOPT2 &= ~(3 << 26);
  SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);
  SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

  //We have to feed this function the clock in KHz!
  uart_init(CORE_CLOCK / 2 / 1000, SERIAL_BAUD);
}

//
// uart_init() -- Initialize debug / OpenSDA UART0
//
//      The OpenSDA UART RX/TX is connected to pins 27/28, PTA1/PTA2 (ALT2)
//
void uart_init(int sysclk, int baud)
{
  uint8 i;
  uint32 calculated_baud = 0;
  uint32 baud_diff = 0;
  uint32 osr_val = 0;
  uint32 sbr_val, uart0clk;
  uint32 reg_temp = 0;
  uint32 baud_rate;
  uint32 temp = 0;
  
  SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;

  UART0_C2 &= ~(UART0_C2_RE_MASK | UART0_C2_TE_MASK | UART0_C2_RIE_MASK);  

    // Verify that a valid clock value has been passed to the function 
    if ((sysclk > 50000) || (sysclk < 32)) {
      sysclk = 0;
      reg_temp = SIM_SOPT2;
      reg_temp &= ~SIM_SOPT2_UART0SRC_MASK;
      reg_temp |= SIM_SOPT2_UART0SRC(0);
      SIM_SOPT2 = reg_temp;

      // Enter inifinite loop because the 
      // the desired system clock value is 
      // invalid!!
      while (1) {
      }
    }

    // Initialize baud rate
    baud_rate = baud;
    
    // Change units to Hz
    uart0clk = sysclk * 1000;
    // Calculate the first baud rate using the lowest OSR value possible.  
    i = 4;
    sbr_val = (uint32) (uart0clk / (baud_rate * i));
    calculated_baud = (uart0clk / (i * sbr_val));

    if (calculated_baud > baud_rate)
      baud_diff = calculated_baud - baud_rate;
    else
      baud_diff = baud_rate - calculated_baud;

    osr_val = i;

    // Select the best OSR value
    for (i = 5; i <= 32; i++) {
      sbr_val = (uint32) (uart0clk / (baud_rate * i));
      calculated_baud = (uart0clk / (i * sbr_val));

      if (calculated_baud > baud_rate)
        temp = calculated_baud - baud_rate;
      else
        temp = baud_rate - calculated_baud;

      if (temp <= baud_diff) {
        baud_diff = temp;
        osr_val = i;
      }
    }

    if (baud_diff < ((baud_rate / 100) * 3)) {
      // If the OSR is between 4x and 8x then both
      // edge sampling MUST be turned on.  
      if ((osr_val > 3) && (osr_val < 9))
        UART0_C5 |= UART0_C5_BOTHEDGE_MASK;

      // Setup OSR value 
      reg_temp = UART0_C4;
      reg_temp &= ~UART0_C4_OSR_MASK;
      reg_temp |= UART0_C4_OSR(osr_val-1);

      // Write reg_temp to C4 register
      UART0_C4 = reg_temp;

      reg_temp = (reg_temp & UART0_C4_OSR_MASK) + 1;
      sbr_val = (uint32) ((uart0clk) / (baud_rate * (reg_temp)));

      /* Save off the current value of the uartx_BDH except for the SBR field */
      reg_temp = UART0_BDH & ~(UART0_BDH_SBR(0x1F));

      UART0_BDH = reg_temp | UART0_BDH_SBR(((sbr_val & 0x1F00) >> 8));
      UART0_BDL = (uint8) (sbr_val & UART0_BDL_SBR_MASK);

    } else {
      // Unacceptable baud rate difference
      // More than 3% difference!!
      // Enter infinite loop!
      //while(1)
      //  {}
    }

    // Initialize transmit and receive circular buffers
    buf_reset(tx_buffer, BUFLEN);
    buf_reset(rx_buffer, BUFLEN);

    // Enable the transmitter, receiver, and receive interrupts
    UART0_C2 |= (UART0_C2_RE_MASK | UART0_C2_TE_MASK | UART0_C2_RIE_MASK);
    enable_irq(INT_UART0 - 16);
}
