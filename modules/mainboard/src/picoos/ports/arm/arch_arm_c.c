/*
 *  Copyright (c) 2004, Jun Li, lj_sourceforge@users.sourceforge.net.
 *  Copyright (c) 2004, Dennis Kuschel.
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


/*
 * This file is originally from the pico]OS realtime operating system
 * (http://picoos.sourceforge.net).
 *
 * CVS-ID $Id: arch_arm_c.c,v 1.1 2004/06/07 18:47:22 dkuschel Exp $
 */

#include "../../inc/picoos.h"

#define SUP_MODE  0x13       // Supervisor Mode (SVC)


#if POSCFG_TASKSTACKTYPE != 1
#error  Only  POSCFG_TASKSTACKTYPE = 1  supported !
#endif


extern void osHeapFree(void *ptr);
extern void *osHeapAlloc(unsigned int size);
extern void osTimerInterruptInitialize(void);

/*---------------------------------------------------------------------------
 *  INITIALIZE THIS PORT
 *-------------------------------------------------------------------------*/
void p_pos_initArch(void)
{
      osTimerInterruptInitialize();
}


/*  INIT TASK CONTEXT  */ 
VAR_t p_pos_initTask(POSTASK_t task, UINT_t stacksize,
                     POSTASKFUNC_t funcptr, void *funcarg)
{
    unsigned int    *stk, z;

    task->stackstart = osHeapAlloc(stacksize);
    if (task->stackstart == NULL)
      return -1;

    /* get aligned stack pointer, reserve 32 bytes */
    z = (unsigned int)task->stackstart + stacksize - 4;
    z = (z - POSCFG_ALIGNMENT) & ~(POSCFG_ALIGNMENT - 1);
    stk = (unsigned int*) z;

    /* setup initial stack frame */
    *(stk)   = (unsigned int)0x00000000;  /* reserve space        */
    *(--stk) = (unsigned int)0x00000000;  /* free stack           */
    *(--stk) = (unsigned int)funcptr;     /* Entry Point          */
    *(--stk) = (unsigned int)0xcccccccc;  /* r12                  */
    *(--stk) = (unsigned int)0xbbbbbbbb;  /* r11                  */
    *(--stk) = (unsigned int)0xaaaaaaaa;  /* r10                  */
    *(--stk) = (unsigned int)0x99999999;  /* r9                   */
    *(--stk) = (unsigned int)0x88888888;  /* r8                   */
    *(--stk) = (unsigned int)0x77777777;  /* r7                   */
    *(--stk) = (unsigned int)0x66666666;  /* r6                   */
    *(--stk) = (unsigned int)0x55555555;  /* r5                   */
    *(--stk) = (unsigned int)0x44444444;  /* r4                   */
    *(--stk) = (unsigned int)0x33333333;  /* r3                   */
    *(--stk) = (unsigned int)0x22222222;  /* r2                   */
    *(--stk) = (unsigned int)0x11111111;  /* r1                   */
    *(--stk) = (unsigned int)funcarg;     /* r0 : argument        */
    *(--stk) = (unsigned int)SUP_MODE;    /* CPSR                 */
    *(--stk) = (unsigned int)posTaskExit; /* lr                   */
    *(--stk) = (unsigned int)0xFFFFFFFF;  /* free stack           */

    task->stackptr = (void*)stk;

    return 0;
}


void p_pos_freeStack(POSTASK_t task)
{
    osHeapFree(task->stackstart);
}
