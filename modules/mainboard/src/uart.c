#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <picoos.h>

#include "uart.h"
#include <util/setbaud.h>

#define DISABLE_TXE_INTERRUPT UCSRB &= ~(1 << UDRIE)
#define ENABLE_TXE_INTERRUPT  UCSRB |= 1 << UDRIE

static struct {
	uint8_t buf[UART_BUF_SIZE];
	volatile uint8_t head;
	volatile uint8_t tail;
	volatile uint8_t full;
	POSSEMA_t lock;
}ttx,*tx=&ttx,trx,*rx=&trx;

int uart_puts(const char *s) {
	int cnt = 0;
	while (*s && !uart_putchar(*s++)) cnt++;
	return cnt;
}

int uart_putchar(uint8_t ch)
{
	posSemaGet(tx->lock);
	tx->buf[tx->head] = ch;
	tx->head = (tx->head + 1) & (UART_BUF_SIZE - 1);
	ENABLE_TXE_INTERRUPT;
	return 0;
}
/**
 * UART transmit interrupt service routine
 **/
static void usart_txrdy(void)
{
	UDR = tx->buf[tx->tail];
	tx->tail = (tx->tail + 1) & (UART_BUF_SIZE - 1);
	if (tx->tail == tx->head) DISABLE_TXE_INTERRUPT;
	posSemaSignal(tx->lock);
}
PICOOS_SIGNAL(SIG_USART_DATA, usart_txrdy)

int uart_gets(char *s)
{
	int cnt = 0;
	while((*s++ = uart_getchar()) != UART_TCHAR) cnt++;
	*s = '\0';
	return cnt;
}

int uart_getchar_timeout(uint16_t timeout)
{
	int ch;
	if (posSemaWait(rx->lock, timeout) != E_OK) return -1;
	ch = (int) rx->buf[rx->tail];
	rx->tail = (rx->tail + 1) & (UART_BUF_SIZE - 1);
	rx->full = 0;
	return ch;
}

static void uart_rx(void)
{
	uint8_t ch;
	ch = UDR;
	if (rx->full) {
		/* TODO: Handle Serious buffer
		 * Overflow problem. Currently
		 * discarding rx data */
		return ;
	}
	rx->buf[rx->head] = ch;
	rx->head = (rx->head + 1) & (UART_BUF_SIZE - 1);
	posSemaSignal(rx->lock);
	if (rx->head == rx->tail) rx->full = 1;
}
PICOOS_SIGNAL(SIG_USART_RECV, uart_rx)

/**
 * Initialize the UART and create the necessary sync.
 **/
void uart_init(void)
{
	/* Initialize the UART Registers */
	UBRRL = UBRRL_VALUE;
	UBRRH = UBRRH_VALUE;

	/* Enable the UARTs now */
	UCSRB |= _BV(RXCIE) | _BV(RXEN) | _BV(TXEN);
	UCSRA |= (USE_2X << U2X);
	tx->lock = posSemaCreate(UART_BUF_SIZE);
	rx->lock = posSemaCreate(0);
}

