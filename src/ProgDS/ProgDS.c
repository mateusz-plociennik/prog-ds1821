/*
 * ProgDS.c
 *
 * Created: 2011-12-21 15:58:36
 *  Author: X11
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/atomic.h>

#include "DS.h"
#include "HD44780.h"
#include "KEY.h"
#include "PWRMGMT.h"

#define F_CPU 1000000UL

char key;

int main(void)
{
	//cli();
	//wdt_reset();
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	//cli(); // Disable Interupts
	OWI_Init();
	LCD_Initalize();
	
	
/*
	LCD_Clear();
	LCD_GoTo(0,0);
	LCD_WriteText("ProgDS ");
	LCD_WriteText(__TIME__);
	LCD_GoTo(0,1);
	LCD_WriteText(__DATE__);
	_delay_ms(500);*/
	KEY_Init();
	PowerInit(); // Musi byæ jako ostatnia
	MENU_Update();
	set_sleep_mode(SLEEP_MODE_IDLE);
	sei(); // Enable Interupts
    for (;;)
    {
		//OCR1BL = 0xFF;
		//set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_mode();
	}
}

