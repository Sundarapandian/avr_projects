#ifndef _UART_H
#define _UART_H

#ifndef BAUD
#define BAUD 9600UL
#endif

/*
 * Define termination character for UART gets
 */
#ifndef UART_TCHAR
#define UART_TCHAR '\n'
#endif

#ifndef UART_BUF_SIZE
/* UART BUF SIZE has to be a power of 2 */
#define UART_BUF_SIZE  32
#endif

#define uart_getchar() uart_getchar_timeout(INFINITE)
#ifdef __cplusplus
extern "C" {
#endif
	int uart_getchar_timeout(uint16_t tout);
	int uart_putchar(uint8_t ch);
	int uart_puts(const char *s);
	void uart_init(void);

#ifdef __cplusplus
}
#endif
#endif /* !def _UART_H */
