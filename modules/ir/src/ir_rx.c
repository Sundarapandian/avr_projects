#include <ir_rx.h>

extern POSSEMA_t sync;

static volatile int ticks;

int ir_rx()
{
	/* Start Enable Counter */
	OCR0 = irp->width;
	TCCR0 = _BV(WGM01) | _BV(CS01) | _BV(CS00);
	TIMSK |= _BV(OCIE0);
}
