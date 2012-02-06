#include "HD44780.h"
#include "DS.h"
#include "PWRMGMT.h"
#include "KEY.h"
#include "MENU.h"

unsigned char pwrTimeout;
unsigned char menuUpdateOnTimer;

extern inline void PowerInit(void)
{
	pwrTimeout = PWR_TIMEOUT;
	//TCCR0A |= (0 << WGM02) | (0 << WGM01) | (0 << WGM00); // Normal mode
	TCCR0B = (1 << CS02) | (0 << CS01) | (1 << CS00); // CLK / 1024
	TIMSK |= (1 << TOIE0);
}

ISR(TIMER0_OVF_vect)
{
	if (pwrTimeout > 0)
	{
		pwrTimeout--;
	}
	
	
	if ((pwrTimeout % 3) == 0)
	{
		MENU_Update();
	}
	
	switch (pwrTimeout)
	{
		case (PWR_TIMEOUT - 1):
		{
			OCR1BL = 0xFF; // Backlight ON
			GIMSK |= (1 << PCIE);
			break;
		}
		case (DIM_TIMEOUT - 1):
		{
			OCR1BL = DIM_POWER; // Backlight DIM/OFF
			break;
		}
		case (BKL_TIMEOUT - 1):
		{
			OCR1BL = 0x00; // Backlight OFF
			break;
		}			
		case 0x00:
		{
			DDRD = LCD_POWER;
			PORTD = 0x00; // Display OFF, other I/O Hi-Z
			//DDRB = LCD_BACKLIGHT;
			//PORTB = 0x00;
			DDRA = 0x00;
			PORTA = 0x00;
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			//sleep_mode();
			break;
		}			
	}
}