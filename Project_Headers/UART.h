#ifndef UART_H_
#define UART_H_

#define SERIAL_BAUD   115200

#define printf io_printf

void UART0_IRQHandler() __attribute__((interrupt("IRQ")));
void uart_putchar(char x);
int uart_write(char *p, int len);
int uart_write_err(char *p, int len);
int uart_read(char *p, int len);
int uart_rx(int num);
void uart_init(int sysclk, int baud_rate);
void InitUARTs();

static inline void __enable_irq(void) { asm volatile ("cpsie i"); }
static inline void __disable_irq(void)  { asm volatile ("cpsid i"); }

int io_printf(const char *, ...);

int sprintf(char *, const char *, ...);

#endif
