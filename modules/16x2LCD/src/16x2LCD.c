#include <avr/io.h>
#include <picoos.h>
#include "16x2LCD.h"

#define DELAY(val) do{volatile int x = (val); while(x--) _NOP();} while(0)

static void LCD_send_data(unsigned char ch, char cmd)
{
	PORTA = cmd ? (PORTA & ~REG_SEL) : (PORTA | REG_SEL); /* First select the Instruction register */
	PORTA &= ~RDnWR;
	PORTA |= ENABLE;
	PORTC = (unsigned char) (ch & 0xFF);
	DELAY(10);
	PORTA &= ~ENABLE;
}

void LCD_send_command(int cmd)
{
	LCD_send_data((unsigned char) (cmd & 0xFF), 1);
}

void LCD_putchar(char ch)
{
	LCD_send_data(ch, 0);
}

void LCD_clear(void)
{
	LCD_send_command(LCD_CMD_CLRSCR);
}

void LCD_puts(char *str)
{
	while (str && *str){
		LCD_putchar(*str++);
	}
}

void LCD_GotoXY(int x, int y)
{
	int start, end;
#ifndef LCD_CONFIG_SINGLELINE
	if (x > 1 || x < 0) return;
#else
	if (x) return ;
#endif
	start = x ? LCD_LINE1_START : LCD_LINE0_START;
	end = x ? LCD_LINE1_END : LCD_LINE0_END;
	if (start + y <= end)
		LCD_send_command(LCD_CMD_SET_DDRAM_ADDR | (start + y));
}

void LCD_init(void)
{
	PORTA = 0;
	PORTC = 0;
	DDRA |= _BV(DDA0) | _BV(DDA1) | _BV(DDA2);
	DDRC = 0xFF;
	MCUCSR |= _BV(JTD);
	/* After RESET LCD controller will take 10ms to finish initialization */
	posTaskSleep(MS(20));

	LCD_send_command(LCD_CMD_DISPLAY_SET); /* Turn OFF LCD first */
	posTaskSleep(MS(2));

	LCD_send_command(LCD_CMD_FUNCTION_SET | LCD_FUNCTION_8BIT_MODE | LCD_FUNCTION_2LINE_MODE);
	posTaskSleep(MS(2));

	LCD_send_command(LCD_CMD_ENTRY_MODE_SET | LCD_ENTRY_MODE_INCREMENT);
	posTaskSleep(MS(2));

	LCD_send_command(LCD_CMD_DISPLAY_SET | LCD_DISPLAY_ON |
		LCD_DISPLAY_CURSOR | LCD_DISPLAY_CURSOR_BLINK);
	posTaskSleep(MS(2));

	LCD_send_command(LCD_CMD_CLRSCR);
	posTaskSleep(MS(2));
}


