#include <string.h>
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
		struct rx_pkt * rx;
		uint8_t *msg = posMessageAlloc();
		if (msg == NULL) {
			err("Running out of memory\n");
			/* Sleep 100ms before we try again */
			posTaskSleep(MS(100));
			continue;
		}
		rx = (struct rx_pkt *) msg;
		if (dl_recv_frame(&ps, rx) > 0) {
			PORTB ^= _BV(PB0);
		}
		posMessageFree(msg);
	}
}

static void wait_a_minute (int val)
{
	int i;
	for (i = 0; i < 12 * val; i++)
		posTaskSleep(MS(5000));
}

static uint32_t units;
static void send_data(const struct psock * ps)
{
	static struct rx_frame data = {.start = '<',};
	data.value[0] = (units >> 24);
	data.value[1] = (units >> 16) & 0xFF;
	data.value[2] = (units >> 8) & 0xFF;
	data.value[3] = units & 0xFF;
	dl_tx_data(ps, (uint8_t *)&data, sizeof(data));
	PORTB ^= _BV(PB0);
}

void tx_task(void * socket)
{
	const struct psock *ps = (const struct psock *) socket;
	while (1) {
		wait_a_minute(2);
		units += 10;
		send_data(ps);
	}
}

void keypad_task(void * socket)
{
	uint8_t pindwn = 0;
	const struct psock *ps = (const struct psock *) socket;
	while(1) {
		if (PINB & _BV(PB5)) {
			pindwn |= _BV(PB5);
			posTaskSleep(MS(10));
		} else if (pindwn & _BV(PB5)) {
			/* Button up */
			pindwn &= ~_BV(PB5);
			units += 10;
			posTaskSleep(MS(10));
		}
		if (PINB & _BV(PB6)) {
			pindwn |= _BV(PB6);
			posTaskSleep(MS(10));
		} else if (pindwn & _BV(PB6)) {
			/* Button up */
			pindwn &= ~_BV(PB6);
			send_data(ps);
			posTaskSleep(MS(10));
		}
		posTaskSleep(MS(25));
	}
}

void app_main(void * ignore)
{
	static struct psock ps;
	PORTB |= _BV(PB0);
	DDRB |= _BV(DDB0);

	SFIOR &= ~_BV(PUD);
	PORTB |= _BV(PB5) | _BV(PB6);
	DDRB &= ~(_BV(PB5) | _BV(PB6));
	psock_init(&ps, IFACE0, ADDR64_COORDINATOR_HI, ADDR64_COORDINATOR_LO,
			ADDR16_ANY, PS_EP_DATA, PS_EP_DATA, PS_CID_SERIALDATA, PSOCK_PROFILE_DEFAULT, 0);
	uart_init();
//	posTaskCreate(rx_task, NULL, 4);
	posTaskCreate(tx_task, &ps, 5);
	posTaskCreate(keypad_task, &ps, 3);
}
