#ifndef _DS_H_
#define _DS_H_

#include <avr/io.h>

#include "OWI.h"

// Defines used only in code example.
#define DS1820_FAMILY_ID                0x10 
#define DS1820_START_CONVERSION         0x44
#define DS1820_READ_SCRATCHPAD          0xbe
#define DS1820_ERROR                    -1000   // Return code. Outside temperature range.

#define DS1821_PRESENCE_ERROR			0x01
#define DS1821_WRITE_ERROR				0x02
#define DS1821_BUS_ERROR				0x03

#define DS1821_READ_TEMPERATURE			0xAA
#define DS1821_START_CONVERT_T			0xEE
#define DS1821_STOP_CONVERT_T			0x22
#define DS1821_WRITE_TH					0x01
#define DS1821_WRITE_TL					0x02
#define DS1821_READ_TH					0xA1
#define DS1821_READ_TL					0xA2
#define DS1821_WRITE_STATUS				0x0C
#define DS1821_READ_STATUS				0xAC
#define DS1821_READ_COUNTER				0xA0
#define DS1821_LOAD_COUNTER				0x41

#define DS1821_DONE						0x80
//#define DS1821_1						0x40
#define DS1821_NVB						0x20
#define DS1821_THF						0x10
#define DS1821_TLF						0x08
#define DS1821_T_R						0x04
#define DS1821_POL						0x02
#define DS1821_1SHOT					0x01

#define DS1821_READ						0x00
#define DS1821_WRITE					0x01


#define SEARCH_SUCCESSFUL               0x00
#define SEARCH_CRC_ERROR                0x01

#define FALSE							0
#define TRUE							1

#define MAX_DEVICES						1       //!< Max number of devices to search for.

#define DS1821_PIN_DQ					PORTA0
#define DS1821_PIN_VDD					PORTA1

void DS1821_ModeToggle();

#endif