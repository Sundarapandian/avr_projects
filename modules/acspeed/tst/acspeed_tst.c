#include <string.h>
#include <avr/io.h>
#include <picoos.h>
#include <acspeed.h>

#ifdef SIMULATION
#define TIME_DELAY			20
#else
#define TIME_DELAY			1000
#endif

const int speed = 1;

static void toggle_port(int on)
{
	if (on)
		PORTB |= _BV(PB0);
	else
		PORTB &= ~_BV(PB0);
}

void app_main(void *ignore)
{
	PORTB &= ~_BV(PB0);
	DDRB |= _BV(DDB0);
	acspeed_init(toggle_port);
	acspeed_set(ACSPEED_MAX_RANGE - speed);
}


