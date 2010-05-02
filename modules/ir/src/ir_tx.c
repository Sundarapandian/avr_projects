#include <avr/io.h>
#include <avr/pgmspace.h>
#include <picoos.h>
#include <string.h>
#include <ir_tx.h>

/**
 * Macros that will start/stop
 * the carrier signal for modulation
 **/
#define start_carrier(cnt) do { \
	OCR2 = ((cnt) >> 1) - 1; \
	TCCR2 = _BV(WGM21) | _BV(CS21) | _BV(COM20); \
}while (0)

#define stop_carrier() if(1){TCCR2 = 0;}

/**
 * Data used by timer interrupt handler
 **/
struct tx_data {
	uint8_t data[16];
	uint8_t bit;
	uint8_t cnt;
	POSSEMA_t lock;
}tx_data, *txd=&tx_data;

static POSSEMA_t sync;
/**
 * Data handlers for various protocols
 **/
static uint8_t prepare_rc5(uint8_t addr, uint8_t cmd);
static uint8_t prepare_sirc(uint8_t addr, uint8_t cmd);
static uint8_t prepare_samsung(uint8_t addr, uint8_t cmd);

/**
 * Various IR protocol entries
 **/
static struct prot{
	uint8_t freq;
	uint8_t width;
	uint8_t (*fill_data)(uint8_t addr, uint8_t cmd);
}irproto[] PROGMEM = {
	[RC5] = {    /* Philips RC5 Protocol */
		28,         /* 1/28uSec = 36 KHz */
		111,        /* 111 x 8 = 888 uSec */
		prepare_rc5,
	},
	[SIRC] = {   /* Sony IR protocol */
		25,         /* 1/25uSec = 40 KHz */
		75,         /* 75 x 8 = uSec */
		prepare_sirc,
	},
	[SAMSUNG] = {
		26,         /* 1/26uSec = 38 KHz */
		70,         /* 70 x 8 = 540 uSec */
		prepare_samsung,
	},
};

/**
 * Timer tick interrupt handler
 * It will transmit the enable signal via ENPIN
 * of ENPORT. Must be connected to NAND to get
 * the modulated output.
 **/
static void tmr_tick(void)
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

/**
 * Mirrors the bits in a given byte
 **/
static uint8_t mirror(uint8_t val)
{
	val = ((val & 0x55) << 1) | ((val & 0xAA) >> 1);
	val = ((val & 0x33) << 2) | ((val & 0xCC) >> 2);
	val = ((val & 0x0F) << 4) | ((val & 0xF0) >> 4);
	return val;
}

/* Prepares data for RC5 Protocol */
static uint8_t prepare_rc5(uint8_t addr, uint8_t cmd)
{
	uint16_t val = (mirror(cmd << 2) << 8) | (mirror(addr << 3) << 3) | 0x03;
	uint8_t i, j;

	for (i = 0, j = 0; i < 14; i ++) {
		txd->data[j >> 3] |= ((val & 1) ? 0x02 : 0x01) << (j & 7);
		j += 2;
		val >>= 1;
	}
	return 28;
}

/* Prepare data for Sony SIRC protocol */
static uint8_t prepare_sirc(uint8_t addr, uint8_t cmd)
{
	uint8_t i, j;
	uint16_t val = (addr << 7) | cmd;

	txd->data[0] = 0x0F; /* 5-bit start seq */
	for (i = 0, j = 5; i < 12; i++, val >>= 1, j++) {
		txd->data[j>>3] |= _BV(j&7);
		j++;

		if (val&1) {
			txd->data[j>>3] |= _BV(j&7);
			j++;
		}
	}
	return j;
}

/* Prepare data for Samsung IR protocol */
static uint8_t prepare_samsung(uint8_t addr, uint8_t cmd)
{
	uint16_t val1, val2, val;
	int i, j;
	val1 = addr | (addr << 8);
	val2 = (~cmd << 8) | cmd;
	txd->data[0] = 0xFF;
	for (i = 0, j = 16, val = val1; i < 33; i ++, val >>= 1) {
		if (i == 16) val = val2;
		txd->data[j>>3] |= _BV(j&7);
		j += _BV(1+(val&1));
	}
	return j;
}

/**
 * Function that transmits given 5-bit address
 * @addr and 6-bit command @cmd to the IR LEDs
 * using Sony SIRC protocl using a carrier
 * frequency of 36KHz.
 **/
void ir_tx(int protocol, uint8_t addr, uint8_t cmd)
{
	struct prot tmp, *irp = &tmp;

	memcpy_P(irp, &irproto[protocol], sizeof(struct prot));
	/* Start preparing Tx Data */
	posSemaGet(sync);
	memset(txd->data, 0, sizeof txd->data);
	txd->cnt = irp->fill_data(addr, cmd);
	if (txd->data[0] & 1) {
		IR_ENPORT |= _BV(IR_ENPIN);
	} else {
		IR_ENPORT &= ~_BV(IR_ENPIN);
	}
	txd->bit = (txd->data[0] & 2)?_BV(IR_ENPIN):0;

	/* Start Enable Counter */
	OCR0 = irp->width;
	TCCR0 = _BV(WGM01) | _BV(CS01) | _BV(CS00);
	TIMSK |= _BV(OCIE0);

	/* Start the carrier */
	start_carrier(irp->freq);
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
