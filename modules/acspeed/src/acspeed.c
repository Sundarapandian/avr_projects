#include <avr/io.h>
#include <picoos.h>
#include "acspeed.h"

#define error_display(x)     while(1)
static void (*zd_callback)(int);
static POSTIMER_t zd_tmr;
static POSSEMA_t zd_tmr_sem;

static void zero_detect(void)
{
	ACSR |= _BV(ACI);
	zd_callback(0);
	posTimerStart(zd_tmr);
}
PICOOS_SIGNAL(ANA_COMP_vect, zero_detect)

void acspeed_set(unsigned char speed)
{
	/* Turn off device if speed is Zero */
	if (speed == 0) {
		acspeed_stop();
		zd_callback(0);
		return ;
	}

	/* Permanantly on */
	if (speed >= ACSPEED_MAX_RANGE) {
		zd_callback(1);
		return ;
	}

	if (posTimerSet(zd_tmr, zd_tmr_sem, MS(speed), 0))
	{
		error_display(ERROR_BASE + 1);
	}
	acspeed_start();
}

static void zd_task(void *cb)
{
	zd_callback = (void (*) (int))cb;
	zd_tmr = posTimerCreate();

	if (zd_tmr == NULL) {
		error_display(ERROR_BASE + 0);
	}

	zd_tmr_sem = posSemaCreate(0);
	if (zd_tmr_sem == NULL) {
		error_display(ERROR_BASE + 2);
	}

	while (1) {
		posSemaGet(zd_tmr_sem);
		zd_callback(1);
	}
}

void acspeed_init(void (*cb)(int))
{
	SFIOR &= ~_BV(ACME); /* Enable AN1 */
	ACSR &= ~(_BV(ACBG) | _BV(ACIS0));  /* Enable AN0 */
	ACSR |= _BV(ACIE) | _BV(ACIS1);
	posTaskCreate(zd_task, cb, POSCFG_MAX_PRIO_LEVEL - 1);
}

void acspeed_start(void)
{
	/* Start Zero detection */
	ACSR &= ~_BV(ACD);
}

void acspeed_stop(void)
{
	/* Stop Zero detection */
	ACSR = _BV(ACD);
	posTimerStop(zd_tmr);
}
