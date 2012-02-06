#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>
//#include <string.h> // Used for memcpy.

#include "DS.h"
#include "OWI.h"

unsigned char DS1821_ReadTemperature(signed char *temperature)
{
	unsigned char status;
	
	if (!OWI_DetectPresence()) { return DS1821_PRESENCE_ERROR; }
	OWI_SendByte(DS1821_READ_STATUS);
	status = OWI_ReceiveByte();
	
	if ((~status) & DS1821_1SHOT)
	{
		if (!OWI_DetectPresence()) { return DS1821_PRESENCE_ERROR; }
		OWI_SendByte(DS1821_WRITE_STATUS);
		OWI_SendByte(status | DS1821_1SHOT);
	
		_delay_ms(50); // Max EEPROM Write Time 50 ms
		/*do { // Wait for NVB
			if (!OWI_DetectPresence()) { return DS1821_PRESENCE_ERROR; }
			OWI_SendByte(DS1821_READ_STATUS);
			status = OWI_ReceiveByte(); 
		} while (status & DS1821_NVB);*/
	}

	if (!OWI_DetectPresence()) { return DS1821_PRESENCE_ERROR; }
	OWI_SendByte(DS1821_START_CONVERT_T);
	
	do { // Wait for DONE
		if (!OWI_DetectPresence()) { return DS1821_PRESENCE_ERROR; }
		OWI_SendByte(DS1821_READ_STATUS);
		status = OWI_ReceiveByte(); 
	} while ((~status) & DS1821_DONE);

	if (!OWI_DetectPresence()) { return DS1821_PRESENCE_ERROR; }
	OWI_SendByte(DS1821_READ_TEMPERATURE);
	(*temperature) = OWI_ReceiveByte();
	
	return 0;
}

void DS1821_ModeToggle()
{
	unsigned char i;
	unsigned char status;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		//OWI_DDR &= ~(1 << DS1821_PIN_DQ); // Set DQ High Imp.
		OWI_PORT &= ~(1 << DS1821_PIN_VDD); // Power down DS
		
		//while (OWI_PIN & (1 << DS1821_PIN_VDD)); // Wait for VDD Low
		
		for (i = 0; i < 32; i++)
		{
			OWI_DDR ^= (1 << DS1821_PIN_DQ); // Toggle DQ
		}
		
		OWI_PORT |= (1 << DS1821_PIN_VDD); // Power up DS
	}
	_delay_us(OWI_DELAY_H_STD_MODE);
}
