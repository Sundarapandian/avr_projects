#include <picoos.h>

#define LED_ITERATION 3
#define DEV_ITERATION 1
#define LED_DELAY MS(500)
#define DEV_DELAY MS(1000)
void ioboard_test(void * ignore)
{
	int i;
	/**
	 * LED TEST: Checking LED by blinking LED in a
	 * row at the duration of 1 seconds
	 **/
	for(i = 0; i < LED_ITERATION * 4; i++){
		ioboard_out(1 << (4 + (i & 3)), 0);
		posTaskSleep(LED_DELAY);
		ioboard_out(0, 1 << (4 + (i & 3)));
	}

	/**
	 * DEVICE TEST: Checking devices,
	 * by switching them on and off one after another
	 * at a duration of 5 seconds
	 **/
	for(i = 0; i < DEV_ITERATION * 4; i++){
		ioboard_out(1 << (i & 3), 0);
		posTaskSleep(DEV_DELAY);
		ioboard_out(0, 1 << (i & 3));
	}

	/**
	 * MAX POWER TEST: Checking the max power withstanding
	 * capabilities of ioboard by enabling all the devices
	 * for 10 seconds
	 **/
	ioboard_out(0xFF, 0);
	posTaskSleep(MS(10000));
	ioboard_out(0, 0xFF);

	/**
	 * INPUT TEST: Initialize the input interrupt
	 * rest will be taken care at ISR
	 **/
//	MCUCR |= _BV(ISC01) | _BV(ISC00);
//	GICR |= _BV(INT0);
//	DDRD &= ~_BV(PD2);
//	PORTD |= _BV(PD2);
	while (1){
		posTaskSleep(MS(1500));
		ioboard_out(1 << 4, 1 << 4);
	}
}

/**
 * INPUT TEST: Wait for interrupt and switch on corresponding
 * relay and LED!
 * TODO: For now input tests are failing... the observation
 * is that the interrupt line is always kept low!!
 **/
void button_pressed(void)
{
	unsigned char val;
	val = ioboard_in();
	val = ~val << 4;
	ioboard_out(val & 0xF0, val & 0xF0);
}
//PICOOS_SIGNAL(SIG_INTERRUPT0, button_pressed);

void app_main(void *ignore)
{
	ioboard_init();
	posTaskCreate(ioboard_test, NULL, 7);
}
