#include <avr/io.h>
#include <picoos.h>
#include "uart.h"

static char buf[80];
static char devaddr[80];
static char ptr[10][7];
void rx_task(void * ignore)
{
	int cnt = 0;
	while (1) {
		if (uart_gets(buf) <= 0)
			continue;
			PORTB ^= _BV(PB0);
		if (!strncmp(buf, "+BTINQ: ", 8)) {
			char * x = strchr(&buf[8], ',');
			if (x == NULL) continue;
			*(x+1) = 0;
			//strcpy(ptr[cnt], &buf[8]);
			uart_puts("AT+BTSDP=");
			uart_puts(&buf[8]);
			uart_puts("2\r");
			posTaskSleep(MS(500));
			PORTB ^= _BV(PB0);
			posTaskSleep(MS(500));
			PORTB ^= _BV(PB0);
			posTaskSleep(MS(500));
			PORTB ^= _BV(PB0);
		}
	}
}

void tx_task(void * ignore)
{
	posTaskSleep(MS(200));
	uart_puts("+++\r\n");
	posTaskSleep(MS(1000));
	while (1) {
		uart_puts("AT+BTINQ=5\r");
		posTaskSleep(MS(2000));
		//uart_gets(buf);
		//if (buf[2] == 'O' && buf[3] == 'K')
			PORTB ^= _BV(PB0);
		while(1)
		posTaskSleep(MS(3000));
	}
}

void app_main(void * ignore)
{
	PORTB = _BV(PB0);
	DDRB = _BV(DDB0);
	uart_init();
	posTaskCreate(tx_task, NULL, 7);
	posTaskCreate(rx_task, NULL, 6);
}
