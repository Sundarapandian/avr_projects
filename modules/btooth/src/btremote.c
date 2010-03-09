#include <string.h>
#include <picoos.h>
#include <16x2LCD.h>

#include "remote.h"
#include "uart.h"

/* External task modules */
extern void timer_task(void *);
static void bt_task(void *);

/**
 * Initial task to create all tasks
 **/
void app_main(void *arg)
{
	UNUSED_PARAM(arg);

	ioboard_init();

	/* For RED LED */
	PORTB = _BV(PB0);
	DDRB = _BV(DDB0);

	/* Start all the tasks */
	posTaskCreate(bt_task, NULL, 7);

	/** 
	 *  All initializations are done
	 *  and we go online from here 
	 **/
	posTaskExit();
}
POSTASK_t bt_task_id;

static void bt_task(void *arg)
{
	/* Find out who am I */
	bt_task_id = posTaskGetCurrent();

	uart_init(bt_task_id);
	/* We are ready to dispatch any BT messages */
	while(1){
		char *msg = posMessageGet();
		if (strstr(msg, "!CONNECT") != NULL){
			posTaskSleep(MS(2000));
			uart_tx("AT+CMER=3,2\r\n", 0);
		}

		//LCD_puts(msg);
		LCD_clear();
		posTaskSleep(MS(20));
		LCD_puts(msg);
		PORTB ^= _BV(PB0);
		posMessageFree(msg);
	}
}

