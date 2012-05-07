#include <string.h>
#include <avr/io.h>
#include <picoos.h>
#include <16x2LCD.h>
#define _BIT(x) (1 << (x))

#ifdef SIMULATION
#define TIME_DELAY			20
#else
#define TIME_DELAY			1000
#endif

char *num2str(char *str, int num, int base)
{
   int i, count = 0;
	if (!num) str[count ++] = '0';
	while (num) {
		str[count ++] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[num % base];
		num /= base;
	}
	for (i = 0; i < (count >> 1); i++){
		char t = str[i];
		str[i] = str[count - i - 1];
		str[count - i - 1] = t;
	}
	str[count] = 0;
	return str;
}

void lcd_write(void *ignore)
{
	char p[20] = "TIME:";
	int tim = 0, len = strlen(p);
	LCD_init();
	LCD_puts(p);
	while (1) {
		LCD_GotoXY(0, len + 1);
		LCD_puts(num2str(p,tim,10));
		tim ++;
		posTaskSleep(MS(TIME_DELAY));
	}
}

void app_main(void *ignore)
{
	posTaskCreate(lcd_write, NULL, 7);
}

