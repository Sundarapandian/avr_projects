#include <avr/io.h>
#include <picoos.h>
#include <string.h>

struct tx_data {
	uint8_t data[7];
	uint8_t bit;
	uint8_t cnt;
	POSSEMA_t lock;
}tx_data, *txd=&tx_data;

static POSSEMA_t sync;
#if !defined(IR_ENPORT) && !defined(IR_ENPIN)
#define IR_ENPORT   PORTA
#define IR_ENPIN    PA5
#endif

void tmr_tick(void)
{
	static uint8_t i;
	uint8_t val;
	i++;
	if (i == txd->cnt) {
		IR_ENPORT &= ~_BV(IR_ENPIN);
		TCCR0 = 0;
		TIMSK &= ~_BV(OCIE0);
		posSemaSignal(txd->lock);
		i = 0;
	}
	val = IR_ENPORT;
	val &= ~(_BV(IR_ENPIN));
	IR_ENPORT = val | txd->bit;
	txd->bit = (txd->data[(i+1)>>3] & _BV((i+1)&7)) ? _BV(IR_ENPIN) : 0;
}
PICOOS_SIGNAL(SIG_OUTPUT_COMPARE0, tmr_tick);

static uint8_t mirror(uint8_t val)
{
	val = ((val & 0x55) << 1) | ((val & 0xAA) >> 1);
	val = ((val & 0x33) << 2) | ((val & 0xCC) >> 2);
	val = ((val & 0x0F) << 4) | ((val & 0xF0) >> 4);
	return val;
}

#define start_carrier(cnt) do { \
	OCR2 = ((cnt) >> 1) - 1; \
	TCCR2 = _BV(WGM21) | _BV(CS21) | _BV(COM20); \
}while (0)

#define stop_carrier() if(1){TCCR2 = 0;}
/**
 * Function that transmits given 5-bit address
 * @addr and 6-bit command @cmd to the IR LEDs
 * using Sony SIRC protocl using a carrier
 * frequency of 36KHz.
 **/
void ir_send_rc5(uint8_t addr, uint8_t cmd)
{
	unsigned int val = (mirror(cmd << 2) << 8) | (mirror(addr << 3) << 3) | 0x03;
	uint8_t i, j;

	/* Start preparing Tx Data */
	posSemaGet(sync);
	memset(txd->data, 0, sizeof txd->data);
	for (i = 0, j = 0; i < 14; i ++) {
		txd->data[j >> 3] |= ((val & 1) ? 0x02 : 0x01) << (j & 7);
		j += 2;
		val >>= 1;
	}
	IR_ENPORT &= ~_BV(IR_ENPIN);
	txd->bit = _BV(IR_ENPIN);
	txd->cnt = 28;

	/* Start Enable Counter */
	OCR0 = 111; /* 111 x 8 = 888 uSec */
	TCCR0 = _BV(WGM01) | _BV(CS01) | _BV(CS00);
	TIMSK |= _BV(OCIE0);

	/* Start the carrier */
	start_carrier(28); /* 28 uSec Cycle = 36 KHz */
	posSemaGet(txd->lock);
	stop_carrier();
	posSemaSignal(sync);
}

/**
 * Function that transmits given 5-bit address
 * @addr and 7-bit command @cmd to the IR LEDs
 * using Sony SIRC protocl using a carrier
 * frequency of 40KHz.
 **/
void ir_send_sirc(uint8_t addr, uint8_t cmd)
{
	uint8_t i, j;
	unsigned int val = (addr << 7) | cmd;

	/* Start preparing Tx Data */
	posSemaGet(sync);
	memset(txd->data, 0, sizeof txd->data);
	txd->data[0] = 0x0F; /* 5-bit start seq */
	for (i = 0, j = 5; i < 12; i++, val >>= 1, j++) {
		txd->data[j>>3] |= _BV(j&7);
		j++;

		if (val&1) {
			txd->data[j>>3] |= _BV(j&7);
			j++;
		}
	}

	IR_ENPORT |= _BV(IR_ENPIN);
	txd->bit = _BV(IR_ENPIN);
	txd->cnt = j;

	/* Start Enable Counter */
	OCR0 = 75; /* 75 x 8 = 600 uSec */
	TCCR0 = _BV(WGM01) | _BV(CS01) | _BV(CS00);
	TIMSK |= _BV(OCIE0);

	/* Start the carrier */
	start_carrier(25); /* 25 uSec Cycle = 40 KHz */
	posSemaGet(txd->lock);
	stop_carrier();
	posSemaSignal(sync);
}

/**
 * Initialize IR transmitter
 **/
void ir_tx_init(void)
{
	/* Tx Initialization */
	DDRD |= _BV(PD7);
	DDRA |= _BV(IR_ENPIN);
	PORTD &= ~_BV(PD7);
	PORTA &= ~_BV(IR_ENPIN);
	txd->lock = posSemaCreate(0);
	sync = posSemaCreate(1);
}
