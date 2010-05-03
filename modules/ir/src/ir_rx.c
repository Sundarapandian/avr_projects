#include <ir_rx.h>

extern POSSEMA_t sync;

static volatile int ticks;

static void tmr_tick(void)
{
	ticks ++;
}
PICOOS_SIGNAL(SIG_OUTPUT_COMPARE0, tmr_tick);

int ir_rx()
{

	/* Start Ticks Timer */
	OCR0 = 0xFF;
	TCCR0 = _BV(WGM01) | _BV(CS01);
	TIMSK |= _BV(OCIE0);
}
