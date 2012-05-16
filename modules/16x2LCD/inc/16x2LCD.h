#ifndef _16X2LCD_H
#define _16X2LCD_H

#define LCD_CMD_FUNCTION_SET       _BV(5)
#define LCD_FUNCTION_8BIT_MODE     _BV(4)
#define LCD_FUNCTION_2LINE_MODE    _BV(3)
#define LCD_FUNCTION_8x11_FONT     _BV(2)

#define LCD_CMD_DISPLAY_SET        _BV(3)
#define LCD_DISPLAY_ON             _BV(2)
#define LCD_DISPLAY_CURSOR         _BV(1)
#define LCD_DISPLAY_CURSOR_BLINK   _BV(0)

#define LCD_CMD_CLRSCR             _BV(0)

#define LCD_CMD_ENTRY_MODE_SET     _BV(2)
#define LCD_ENTRY_MODE_INCREMENT   _BV(1)
#define LCD_ENTRY_MODE_SHIFT       _BV(0)

#define REG_SEL _BV(PA2)
#define ENABLE  _BV(PA0)
#define RDnWR   _BV(PA1)

#define LCD_CMD_SET_DDRAM_ADDR     _BV(7)
#ifdef LCD_CONFIG_SINGLELINE
#define LCD_LINE0_START            0x00
#define LCD_LINE0_END              0x4F
/* Make sure the sanity check fails */
#define LCD_LINE1_START            0x01
#define LCD_LINE1_END              0x00
#else /* 2 Line LCD Display */
#define LCD_LINE0_START            0x00
#define LCD_LINE0_END              0x27
#define LCD_LINE1_START            0x40
#define LCD_LINE1_END              0x67
#endif

#ifndef _NOP
#define _NOP() asm("    nop    ")
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	void LCD_init(void);
	void LCD_send_command(int);
	void LCD_putchar(char);
	void LCD_puts(char *str);
	void LCD_clear(void);
	void LCD_deinit(void);
	void LCD_GotoXY(int x, int y);

#ifdef __cplusplus
}
#endif
#endif /* _16X2LCD_H */

