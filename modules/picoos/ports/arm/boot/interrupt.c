/*
 *  Copyright (c) 2004, Jun Li, lj_sourceforge@users.sourceforge.net.
 *  All rights reserved. 
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. The name of the author may not be used to endorse or promote
 *      products derived from this software without specific prior written
 *      permission. 
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 *  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 *  INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <string.h>
#include "types.h"
#include "../../../inc/picoos.h"

#define     MAXHNDLRS       40

#define     VPint                   *(volatile unsigned int *)

/* ************ BEGIN TARGET SPECIFIC SECTION **************/

#define Base_Addr       0xf0000000

#define TMOD            (Base_Addr+0x40000)               // Timer mode register    
#define TDATA(channel)  (Base_Addr+0x40010 + channel*0x8) // Timer  data register
#define INTMASK         (Base_Addr+0x140008)            //  Internal Interrupt mask register       
#define EXTMASK         (Base_Addr+0x14000C)            //  External Interrupt mask register       

#define     gBUSCLK         133000000

#define     TM_RUN(device)              (0x1<<(device*4))
#define     TM_TOGGLE(device)           (0x2<<(device*4))
#define     TimerStart(device)          ((VPint(TMOD)) |= TM_RUN(device))
#define     TimerInterval(device)       ((VPint(TMOD)) &= ~TM_TOGGLE(device))

#define     Enable_Int(n)       (VPint(INTMASK))    &= (~((unsigned int)1<<(n)))

#define     GlobalEn_Int()      (VPint(EXTMASK))    &= (~((unsigned int)1<<31))

#define     TIMER0_INT          23

/* ************ END TARGET SPECIFIC SECTION **************/

typedef struct
{
    int     (*isr_hw_service)(void *arg);
    void        (*isr_os_service)(void *arg);
    void        *arg;
} InterruptHandlers_t;

InterruptHandlers_t InterruptHandlers[MAXHNDLRS];

void osTimerInterruptInitialize(void)
{
/* ************ BEGIN TARGET SPECIFIC SECTION **************/
    TimerInterval(0);
    (VPint(TDATA(0))) = gBUSCLK/100;
    TimerStart(0);

    Enable_Int(TIMER0_INT);
/* ************ END TARGET SPECIFIC SECTION **************/
}

void osInterruptInitialize(void)
{
    memset(InterruptHandlers, 0, sizeof(InterruptHandlers));

/* ************ BEGIN TARGET SPECIFIC SECTION ************/
    GlobalEn_Int();
/* ************ END TARGET SPECIFIC SECTION **************/
}

void osInterruptConnect(int vector, int (*isr_hw_service)(void *), void (*isr_os_service)(void *), void *arg)
{
    InterruptHandlers_t *intr;
    if ((vector>=0) && (vector < MAXHNDLRS))
    {
        intr = &InterruptHandlers[vector];
        intr->isr_hw_service = isr_hw_service;
        intr->isr_os_service = isr_os_service;
        intr->arg = arg;
        if (isr_os_service)
            posSoftIntSetHandler(vector, (void (*)(UVAR_t))isr_os_service);
    }
}


void ISRIrqHandler(int vector)
{
    InterruptHandlers_t *intr;
    if ((vector>=0) && (vector < MAXHNDLRS))
    {
        intr = &InterruptHandlers[vector];
        if ((intr->isr_hw_service) && ((*intr->isr_hw_service)(intr->arg))&& (intr->isr_os_service))
            posSoftInt(vector, (u_int)intr->arg);
    }   
    else
    {
        /* uninstall vector or invalid vector*/
    }
}

void ISRUndefHandler(volatile unsigned long *address)
{
    //error_printf("\n\n * Exception -> Undefined Abort : [0x%08x] : 0x%08x  !!!", (u_int)address, (u_int)*address);
    while(1);
}

void ISRPrefetchHandler(volatile unsigned long *address)
{
    //error_printf("\n\n * Exception -> Prefetch  Abort : [0x%08x] : 0x%08x  !!!", (u_int)address, (u_int)*address);
    while(1);       
}

void ISRAbortHandler(volatile unsigned long *address)
{
    // error_printf("\n\n * Exception -> Data Abort : [0x%08x] : 0x%08x  !!!", (u_int)address, (u_int)*address);
    while(1);   
}

void ISRSwiHandler(volatile unsigned long number)
{           
    //error_printf("\n\n * Exception -> Software Interrupt Number %d!!! ",number);
}

void ISRFiqHandler(void)
{
}


