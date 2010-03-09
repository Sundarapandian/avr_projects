#ifndef _UART_H
#define _UART_H
void uart_tx(const char *s, unsigned int len);
void uart_init(POSTASK_t rx_tid);
#define uart_puts(s) uart_tx((s), 0)
#define UART_RX_TOUT 3
#endif /* !def _UART_H */
