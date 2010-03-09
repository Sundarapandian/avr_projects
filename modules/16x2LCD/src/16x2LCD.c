#include <avr/io.h>
#include <picoos.h>
#include <16x2LCD.h>

#define DELAY(val) do{int x = (val); while(x--) _NOP();} while(0)

static void LCD_send_data(char ch, char cmd)
{
	PORTA = cmd ? (PORTA & ~REG_SEL) : (PORTA | REG_SEL); /* First select the Instruction register */
	PORTC = (unsigned char) (ch & 0xFF);
	PORTA &= ~RDnWR;
	PORTA |= ENABLE;
	posTaskSleep(MS(10));
	PORTA &= ~ENABLE;
	posTaskSleep(MS(10));
}

void LCD_send_command(int cmd)
{
	LCD_send_data((char) (cmd & 0xFF), 1);
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

void LCD_init(void)
{
	PORTA = 0;
	PORTC = 0;
	DDRA |= _BV(DDA0) | _BV(DDA1) | _BV(DDA2);
	DDRC = 0xFF;
	MCUCSR |= _BV(JTD);
	LCD_send_command(LCD_CMD_FUNCTION_SET | LCD_FUNCTION_8BIT_MODE | LCD_FUNCTION_2LINE_MODE);
	posTaskSleep(MS(10));
	LCD_send_command(LCD_CMD_DISPLAY_SET | LCD_DISPLAY_ON | LCD_DISPLAY_CURSOR | LCD_DISPLAY_CURSOR_BLINK);
	posTaskSleep(MS(10));
	LCD_send_command(LCD_CMD_CLRSCR);
	posTaskSleep(MS(10));
	LCD_send_command(LCD_CMD_ENTRY_MODE_SET | LCD_ENTRY_MODE_INCREMENT);
	posTaskSleep(MS(10));
}

