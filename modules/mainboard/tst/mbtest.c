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

void uart_test(void * ignore)
{
	uart_init();
	while (1) {
		uart_puts("Hello World\n");
		posTaskSleep(MS(5000));
	}
}

void app_main(void *ignore)
{
	posTaskCreate(led_blink_task, NULL, 7);
	posTaskCreate(uart_test, NULL, 6);
}
