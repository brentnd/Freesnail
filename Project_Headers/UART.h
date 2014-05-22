#ifndef UART_H_
#define UART_H_

#define SDA_SERIAL_BAUD		115200
#define printf io_printf

void InitUARTs();

void UART_Process();

char uart_getchar();

void uart_putchar(char ch);

int uart_getchar_present();

int io_printf(const char *, ...);

int sprintf(char *, const char *, ...);

#endif
