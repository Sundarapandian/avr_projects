#include <picoos.h>
#include <string.h>
#include <avr/io.h>

void led_blink_task(void *ignore)
{
	/* For RED LED */
	PORTB = _BV(PB0);
	DDRB = _BV(DDB0);

	/* Blink it every 2 seconds */
	while (1) {
		PORTB ^= _BV(PB0);
		posTaskSleep(MS(2000));
	}
}

char uart_buf[40]="Hello World\n";
char uart_rx_buf[40];
void uart_tx_test(void * ignore)
{
	while (1) {
		uart_puts(uart_buf);
		posTaskSleep(MS(5000));
	}
}

void uart_rx_test(void * ignore)
{
	while (1) {
		uart_gets(uart_rx_buf);
		strcpy(uart_buf, uart_rx_buf);
	}
}
void app_main(void *ignore)
{
	uart_init();
	posTaskCreate(led_blink_task, NULL, 7);
	posTaskCreate(uart_tx_test, NULL, 6);
	posTaskCreate(uart_rx_test, NULL, 5);
}
