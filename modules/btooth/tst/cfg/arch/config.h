#ifndef _CONFIG_H
#define _CONFIG_H
#include <picoos.h>
#include <avr/io.h>

#define MAX_AT_BUFF_SZ                   72 /* DL Buf size */

#define IFACE0                             0
#define _BIT                               _BV

#define dbg(a...)
#define err(a...)

#include <uart.h>
#define iface_getchar(x)                   uart_getchar()
#define iface_getchar_timeout(x,tout)      uart_getchar_timeout(tout)
#define iface_putchar(x,ch)                uart_putchar(ch)
#define TIMER_MS(x)                        MS(x)

#endif /* !def _CONFIG_H */
