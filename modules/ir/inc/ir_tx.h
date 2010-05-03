#ifndef _IR_TX_H
#define _IR_TX_H

#include <avr/io.h>

#define RC5         0
#define SIRC        1
#define SAMSUNG     2

#if !defined(IR_ENPORT) && !defined(IR_ENPIN)
#define IR_ENPORT   PORTA
#define IR_ENPIN    PA5
#endif

#ifdef __cplusplus
extern "C"
{
#endif

void ir_tx_init(void);
void ir_tx(int protocol, uint8_t addr, uint8_t cmd);

#ifdef __cplusplus
}
#endif
#endif /* !def _IR_TX_H */
