#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>
//#include <string.h> // Used for memcpy.

#include "DS.h"
#include "OWI.h"

void DS1821_ModeToggle()
{
	unsigned char i;
	
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		OWI_PORT = (0 << DS1821_PIN_VDD) | (0 << DS1821_PIN_DQ); // Power down DS // DQ High
		//OWI_DDR = (1 << DS1821_PIN_VDD) | (0 << DS1821_PIN_DQ); // Set DQ High Imp.
		
		_delay_ms(128);
		
		for (i = 0; i < 32; i++)
		{
			OWI_DDR ^= (1 << DS1821_PIN_DQ); // Toggle DQ
		}
		
		//OWI_DDR = (1 << DS1821_PIN_VDD) | (0 << DS1821_PIN_DQ);
		OWI_PORT = (1 << DS1821_PIN_VDD) | (0 << DS1821_PIN_DQ); // Power up DS
		
		_delay_us(OWI_DELAY_H_STD_MODE);
	}
/*
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		OWI_PORT = (0 << DS1821_PIN_VDD) | (1 << DS1821_PIN_DQ); // Power down DS // DQ High
		OWI_DDR = (1 << DS1821_PIN_VDD) | (1 << DS1821_PIN_DQ); // Set DQ High Imp.
		
		_delay_ms(128);
		
		for (i = 0; i < 32; i++)
		{
			OWI_PORT ^= (1 << DS1821_PIN_DQ); // Toggle DQ
		}
		
		OWI_DDR = (1 << DS1821_PIN_VDD) | (0 << DS1821_PIN_DQ);
		OWI_PORT = (1 << DS1821_PIN_VDD) | (0 << DS1821_PIN_DQ); // Power up DS
		
		_delay_us(OWI_DELAY_H_STD_MODE);
	}*/
}
