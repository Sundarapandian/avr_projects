#ifndef _INTERRUPT_H
#define  _INTERRUPT_H

#include "types.h"

void osTimerInterruptInitialize(void);

void osInterruptInitialize(void);

void osInterruptConnect(int vector, int (*isr_handler)(void *), void (*os_handle)(void *), void *arg);

#endif

