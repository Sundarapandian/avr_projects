#include <avr/io.h>
#include "ioboard.h"

#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)

/**
 * name : ioboard_out
 * params :
 *         @on  --> Devices to be turned on
 *         @off --> Devices to be turned off
 * Description: Switches on/off the devices mentioned in the parameters
 *            : Toggles the devices set both on and off!
 * NOTE: The function is made as much as re-entrent as possible
 *       This function is callable from an ISR context
 **/
void ioboard_out(unsigned char on, unsigned char off)
{
	static unsigned char status;
	unsigned char tmp;
	tmp = on & off;
	status ^= tmp;
	off ^= tmp;
	on  ^= tmp;
	status |= on;
	status &= ~off;
	tmp = DDRC;
	DDRC = 0xFF;
	PORTC = status;
	PORTA |= _BV(PA3);
	_NOP();
	_NOP();
	_NOP();
	_NOP();
	PORTA &= ~_BV(PA3);
	DDRC = tmp;
}

unsigned char ioboard_in(void)
{
	unsigned char old_dir, val;

	old_dir = DDRC;
	DDRC &= 0xF0;

	/* Enable input */
	PORTA &= ~_BV(PA4);
	_NOP();
	_NOP();
	val = PINC;

	/* Disable input */
	PORTA |= ~_BV(PA4);
	DDRC = old_dir;
	return val & 0x0F;
}

void ioboard_init(void)
{
	DDRA |= _BV(DDA3); /* Set LA (connected to PA3) as output */
	DDRA |= _BV(DDA4); /* Set nEN (Connected to PA4) as output */
	PORTA |= _BV(PA4); /* Disable Input */
	PORTA &= ~_BV(PA3); /* Disable Output */
	DDRC = 0xFF;
	PORTC = 0x00;

	/* set the output ports all 0 */
	ioboard_out(0, 0xFF);
}

