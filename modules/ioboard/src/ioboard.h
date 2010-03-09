#ifndef _IOBOARD_H
#define _IOBOARD_H

#define LED0 _BV(PC4)
#define LED1 _BV(PC5)
#define LED2 _BV(PC6)
#define LED3 _BV(PC7)
#define LED_ALL (LED0 | LED1 | LED2 | LED3)

#define DEV0 _BV(0)
#define DEV1 _BV(1)
#define DEV2 _BV(2)
#define DEV3 _BV(3)

#define DEV_ALL (DEV0 | DEV1 | DEV2 | DEV3)

/**
 * name : ioboard_out
 * params :
 *         @on  --> Devices to be turned on
 *         @off --> Devices to be turned off
 * Description: Switches on/off the devices mentioned in the parameters
 *            : Toggles the devices set both on and off!
 * NOTE: The function is made as much as re-entrent as possible
 *       This function is callable from an ISR context
 **/
void ioboard_out(unsigned char on, unsigned char off);
unsigned char ioboard_in(void);
void ioboard_init(void);
#endif /* not _IOBOARD_H */
