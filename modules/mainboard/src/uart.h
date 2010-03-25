#ifndef _UART_H
#define _UART_H

/*
 * Define termination character for UART gets
 */
#ifndef UART_TCHAR
#define UART_TCHAR '\n'
#endif

#ifndef UART_BUF_SIZE
/* 2^UART_BUF_SIZE will be the size of uart buf */
#define UART_BUF_SIZE  5
#endif

int uart_getchar_timeout(uint16_t tout);
int uart_putchar(uint8_t ch);
int uart_puts(const char *s);
#define uart_getchar() uart_getchar_timeout(INFINITE)
#define UART_RX_TOUT 3
#endif /* !def _UART_H */
