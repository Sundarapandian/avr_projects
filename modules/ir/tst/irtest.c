#include <ir_tx.h>
#include <picoos.h>

void app_main(void * ignore)
{
	ir_tx_init();
	while (1) {
		ir_send_rc5(0x1c, 0x11);
		posTaskSleep(MS(2000));
		ir_send_sirc(0x1, 0x13);
		posTaskSleep(MS(8000));
	}
}
