#include <picoos.h>
#include <xbee/api.h>
#include <xbee/psock.h>

void *h_iface[1];
int nr_iface = 1;

void rx_task (void * ignore)
{
	/* We may not be able to afford this
	 * data structure inside stack! */
	static struct psock ps;

	/* Create a local socket to receive the data from */
	if (psock_local(IFACE0, &ps, 0) == NULL) {
		err("Unable to open local socket.\n");
		posTaskExit();
	}
	while (1) {
		uint8_t *msg = posMessageAlloc();
		if (msg == NULL) {
			err("Running out of memory\n");
			/* Sleep 100ms before we try again */
			posTaskSleep(MS(100));
			continue;
		}
		dl_recv_frame(&ps, msg, POSCFG_MSG_BUFSIZE);
	}
}

void tx_task(void * ignore)
{
	static struct psock ps;
	static uint8_t *data = "Hello World!\n";
	psock_init(&ps, IFACE0, ADDR64_COORDINATOR_HI, ADDR64_COORDINATOR_LO,
			ADDR16_ANY, PS_EP_DATA, PS_EP_DATA, PS_CID_SERIALDATA, PSOCK_PROFILE_DEFAULT, 0);
	PORTB = _BV(PB0);
	DDRB = _BV(DDB0);
	while (1) {
		dl_tx_data(&ps, data, strlen(data));
		posTaskSleep(MS(5000));
		PORTB ^= _BV(PB0);
	}
}

void app_main(void * ignore)
{
	uart_init();
	//posTaskCreate(rx_task, NULL, 4);
	posTaskCreate(tx_task, NULL, 5);
}
