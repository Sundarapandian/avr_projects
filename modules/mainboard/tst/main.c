#include <picoos.h>
#include <string.h>
#include <avr/io.h>

void led_blink_task(void *ignore)
{
	/* For RED LED */
	PORTB = _BV(PB0);
	DDRB = _BV(DDB0);

	/* Blink it every 2 seconds */
	while (1){
		PORTB ^= _BV(PB0);
		posTaskSleep(MS(2000));
	}
}
void app_main(void *ignore)
{
	posTaskCreate(led_blink_task, NULL, 7);
}
