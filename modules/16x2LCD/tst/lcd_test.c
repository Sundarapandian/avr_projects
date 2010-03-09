#include <16x2LCD.h>

int app_main(void)
{
	char *p = "Happy New Year!";
	LCD_init();
	while (*p){
		LCD_putchar((unsigned int)*p);
		p++;
	}
	while(1);
	return 0;
}
