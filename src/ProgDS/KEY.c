#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "KEY.h"

const char KeyTable[4][3] PROGMEM = 
{
	{'7','8','9'}, 
	{'4','5','6'}, 
	{'1','2','3'}, 
	{'S','0','E'}
};

char key = KEY_NOKEY;

extern inline void KEY_Init()
{
	KEY_DDR = KEY_COLS | LCD_BACKLIGHT; // Set cols pins as outputs // Rows - inputs
	KEY_PORT = KEY_ROWS; // Set cols pins low // Rows - pull-ups
	
	PCMSK = KEY_ROWS;
	GIMSK |= (1 << PCIE);
}

char KEY_Get()
{
	unsigned char row;
	unsigned char col;
	char keyPin;
	
	keyPin = ~(KEY_PIN | KEY_COLS | LCD_BACKLIGHT);
	switch(keyPin)
	{
		case KEY_ROW_789: 
			row = 0;
			break; 
		case KEY_ROW_456: 
			row = 1;
			break;
		case KEY_ROW_123: 
			row = 2;
			break;
		case KEY_ROW_S0E: 
			row = 3;
			break;
		default: 
			return KEY_NOKEY;
	}
	
	KEY_DDR = KEY_ROWS | LCD_BACKLIGHT;
	KEY_PORT = KEY_COLS;
	_delay_us(10);
	
	keyPin = ~(KEY_PIN | KEY_ROWS | LCD_BACKLIGHT);
	switch(keyPin)
	{
		case KEY_COL_S147: 
			col = 0;
			break; 
		case KEY_COL_0258: 
			col = 1;
			break;
		case KEY_COL_E369: 
			col = 2;
			break;
		default: 
			KEY_Init();
			return KEY_NOKEY;		
	}
	
	KEY_Init();
	
	return pgm_read_byte(&(KeyTable[row][col]));
}

ISR(PCINT_B_vect)
{
	key = KEY_Get();
	
	if (key != KEY_NOKEY)
	{	
		if (pwrTimeout == 0x00)
		{
			//cli(); //irq's off
			wdt_enable(WDTO_15MS); //wd on,15ms
			while(1) {} //loop 
		}
		
		TCNT0 = KEY_SPEED;
		pwrTimeout = PWR_TIMEOUT;
		GIMSK &= ~(1 << PCIE);
	
		MENU_Update();
	}
	
	key = KEY_NOKEY;
}