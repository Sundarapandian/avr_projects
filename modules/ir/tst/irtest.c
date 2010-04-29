#include <avr/io.h>
#include <picoos.h>
#include <string.h>

static volatile uint8_t cnt, data[7], ind;

void tmr_tick(void)
{
	static uint8_t bit = 0, lsb, tick, cur;
	PORTA = (PORTA & 0xFE) | bit;
	if (!cur && !(cur = data[ind++])){
		PORTB &= ~_BV(PB3);
		TCCR0 = 0;
		tick = 0;
		return ;
	}
	if (!tick) {
		uint8_t val;
		tick = 32; lsb = cur & 1; cur >>= 1;
		val = TCCR0;
		val &= ~(_BV(COM00) | _BV(COM01));
		TCCR0 = val | (lsb ? _BV(COM00) : _BV(COM01));
		bit = lsb;
	}
	else tick --;
}
PICOOS_SIGNAL(SIG_OUTPUT_COMPARE0, tmr_tick);

uint8_t mirror(uint8_t val)
{
	val = ((val & 0x55) << 1) | ((val & 0xAA) >> 1);
	val = ((val & 0x33) << 2) | ((val & 0xCC) >> 2);
	val = ((val & 0x0F) << 4) | ((val & 0xF0) >> 4);
	return val;
}

void send_rc5(uint8_t addr, uint8_t cmd)
{
	unsigned int val = (mirror(cmd << 2) << 8) | (mirror(addr << 3) << 3) | 0x03;
	uint8_t i, j;

	ind = 0;
	cnt = 99;
	memset(data, 0, sizeof data);
	for (i = 0, j = 0; i < 14; i ++) {
		data[j >> 3] |= ((val & 1) ? 0x02 : 0x01) << (j & 7);
		j += 2;
		val >>= 1;
	}
	data[0] |= 1;
	data[0] &= 0xFD;
	OCR0 = 13;
	TCCR0 = _BV(WGM01) | _BV(CS01);
	//TIMSK |= _BV(OCIE0);
}

void app_main(void * ignore)
{
	PORTB |= _BV(PB3);
	DDRB |= _BV(PB3);
	while (1) {
		send_rc5(0x1c, 0x11);
		posTaskSleep(MS(800));
	}
}
