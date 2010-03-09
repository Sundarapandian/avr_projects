#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <util/setbaud.h>
#include <picoos.h>

#include "uart.h"

#define DISABLE_TXE_INTERRUPT UCSRB &= ~(1 << UDRIE)
#define ENABLE_TXE_INTERRUPT  UCSRB |= 1 << UDRIE
static int ubuf_len;
static POSSEMA_t uart_tx_sem, sem_res, tmr_rx;
static unsigned const char *ubuf;
static POSTASK_t rx_task_id; /* Rx Task */
static POSTIMER_t htmr_rx; /* Timer Handler */
static char *index;

/**
 * UART transmit interrupt service routine
 **/
static void usart_txrdy(void)
{
	if (ubuf_len--){
		UDR = *ubuf++;
	}else{
		DISABLE_TXE_INTERRUPT;
		posSemaSignal(uart_tx_sem);
	}
}
PICOOS_SIGNAL(SIG_USART_DATA, usart_txrdy)

int uart_rx(char * buf, int size)
{
	int cnt = 0;
	whlie (1) {
		if (isempty) {
			posSemaGet(tmr_rx);
		}
		buf[cnt++] = uart_buf[tail];
		tail = (tail + 1) & (UART_RX_BUF_SIZE - 1);
		if (tail == head) 
			isempty  = 1;
		if (!buf[cnt - 1] || cnt == size)
	}
}
/**
 * Bluetooth interrupt service routine
 **/
static void uart_rx(void)
{
	char data;
	data = UDR;

	if (isfull) {
		/* Handle buffer overflow situation */
	}
	if (data == '\r' || data == '\n'){
		if (isempty || !uart_buf[head ? head - 1 : UART_RX_BUF_SIZE - 1])
			return;
		data = 0;
	}
	uart_buf[head] = data;
	head = (head + 1) & (UART_RX_BUF_SIZE - 1);
	isempty = 0;
	if (head ==  tail) 
		isfull = 1;
	posSemaSignal(tmr_rx);
	
#if 0
	if (index != NULL){
		*index++ = data;
		*index = 0;

		/* Wait  Seconds for the boot-up */
		posTimerSet(htmr_rx, tmr_rx, MS(50), 0);
		posTimerStart(htmr_rx);
	}
#endif
}
PICOOS_SIGNAL(SIG_USART_RECV, uart_rx)

#define UART_RX_BUF_SIZE  32
static char * uart_buf[UART_RX_BUF_SIZE];
static unsigned char head, tail, isempty, isfull;
#if 0
/**
 * Task that posts UART messages to receiving task
 **/
static void rx_task(void *ignore)
{
	char *uart_msg;

	/* Create the timer */
	htmr_rx = posTimerCreate();


	while(1){
		uart_msg = index = posMessageAlloc();
		if (index == NULL){
			/* TODO: Handle out of memory here */
			break;
		}
		posSemaGet(tmr_rx);
		posMessageSend(uart_msg, rx_task_id);
		uart_msg = NULL;
	}
}
#endif

/**
 * Initialize the UART and create the necessary sync.
 **/
void uart_init(POSTASK_t rx_tid)
{
	/* Initialize the UART Registers */
	UBRRL = UBRRL_VALUE;
	UBRRH = UBRRH_VALUE;

	/* Enable the UARTs now */
	UCSRB |= _BV(RXCIE) | _BV(RXEN) | _BV(TXEN);
	UCSRA |= (USE_2X << U2X);
	uart_tx_sem = posSemaCreate(0);
	sem_res = posSemaCreate(1);
	tmr_rx = posSemaCreate(0);
	rx_task_id = rx_tid;
	posTaskCreate(rx_task, NULL, 4);

	/* Initialize the Rx queue */
	isfull = 0;
	isempty = 1;
	head = tail = 1;
}

/**
 * Trasmits a string (s) of length (len) to Bluetooth module
 **/
void uart_tx(const char *s, unsigned int len)
{
	posSemaGet(sem_res);
	ubuf = (unsigned const char *)s;
	ubuf_len = len ? len : strlen(s);
	ENABLE_TXE_INTERRUPT;
	posSemaGet(uart_tx_sem);
	posSemaSignal(sem_res);
}

