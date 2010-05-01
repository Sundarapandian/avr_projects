#ifndef _IR_TX_H
#define _IR_TX_H

#include <avr/io.h>

#ifdef __cplusplus
extern "C"
{
#endif

void ir_tx_init(void);
void ir_send_sirc(uint8_t addr, uint8_t cmd);
void ir_send_rc5(uint8_t addr, uint8_t cmd);

#ifdef __cplusplus
}
#endif
#endif /* !def _IR_TX_H */
