
#include <avr/io.h>
#include <util/atomic.h>

#include "OWI.h"
//#include "OWIHighLevelFunctions.h"
//#include "OWIBitFunctions.h"
//#include "OWIPolled.h"

//#include "OWIPolled.h"

//#include <avr/io.h>
//#include <inavr.h>


//#include "OWIBitFunctions.h"

/*! \brief Initialization of the one wire bus(es). (Software only driver)
 *  
 *  This function initializes the 1-Wire bus(es) by releasing it and
 *  waiting until any presence sinals are finished.
 *
 *  \param  pins    A bitmask of the buses to initialize.
 */

/*
#define OWI_PULL_BUS_LOW(bitMask) \
            OWI_DDR |= (1 << DS1821_PIN_DQ); \
            OWI_PORT &= ~bitMask;
            
#define OWI_RELEASE_BUS(bitMask) \
            OWI_DDR &= ~(1 << DS1821_PIN_DQ); \
            OWI_PORT &= ~(1 << DS1821_PIN_DQ);
*/

inline void OWI_Init()
{
	// Power on DS1821 (VDD High) // DQ pull-up off
	OWI_PORT = (1 << DS1821_PIN_VDD) | (0 << DS1821_PIN_DQ); 
	// Set VDD as output // DQ as input
	OWI_DDR = (1 << DS1821_PIN_VDD) | (0 << DS1821_PIN_DQ);
	
	
	//OWI_PORT &= ~(1 << DS1821_PIN_DQ);
    //OWI_DDR &= ~(1 << DS1821_PIN_DQ);
    // The first rising edge can be interpreted by a slave as the end of a
    // Reset pulse. Delay for the required reset recovery time (H) to be 
    // sure that the real reset is interpreted correctly.
	_delay_us(OWI_DELAY_H_STD_MODE);
    //__builtin_avr_delay_cycles(OWI_DELAY_H_STD_MODE);
}


/*! \brief  Write a '1' bit to the bus(es). (Software only driver)
 *
 *  Generates the waveform for transmission of a '1' bit on the 1-Wire
 *  bus.
 *
 *  \param  pins    A bitmask of the buses to write to.
 */
void OWI_WriteBit1()
{
    //unsigned char intState;
    
    // Disable interrupts.
    //intState = __save_interrupt();
    //__disable_interrupt();
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		// Drive bus low and delay.
		OWI_DDR |= (1 << DS1821_PIN_DQ);
		_delay_us(OWI_DELAY_A_STD_MODE);
		//__builtin_avr_delay_cycles(OWI_DELAY_A_STD_MODE);
    
		// Release bus and delay.
		OWI_DDR &= ~(1 << DS1821_PIN_DQ);
		_delay_us(OWI_DELAY_B_STD_MODE);
		//__builtin_avr_delay_cycles(OWI_DELAY_B_STD_MODE);
	}    
    // Restore interrupts.
    //__restore_interrupt(intState);
}


/*! \brief  Write a '0' to the bus(es). (Software only driver)
 *
 *  Generates the waveform for transmission of a '0' bit on the 1-Wire(R)
 *  bus.
 *
 *  \param  pins    A bitmask of the buses to write to.
 */
void OWI_WriteBit0()
{
    //unsigned char intState;
    
    // Disable interrupts.
    //intState = __save_interrupt();
    //__disable_interrupt();
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		// Drive bus low and delay.
		OWI_DDR |= (1 << DS1821_PIN_DQ);
		_delay_us(OWI_DELAY_C_STD_MODE);
		//__builtin_avr_delay_cycles(OWI_DELAY_C_STD_MODE);
    
		// Release bus and delay.
		OWI_DDR &= ~(1 << DS1821_PIN_DQ);
		_delay_us(OWI_DELAY_D_STD_MODE);
		//__builtin_avr_delay_cycles(OWI_DELAY_D_STD_MODE);
	}    
    // Restore interrupts.
    //__restore_interrupt(intState);
}


/*! \brief  Read a bit from the bus(es). (Software only driver)
 *
 *  Generates the waveform for reception of a bit on the 1-Wire(R) bus(es).
 *
 *  \param  pins    A bitmask of the bus(es) to read from.
 *
 *  \return A bitmask of the buses where a '1' was read.
 */
unsigned char OWI_ReadBit()
{
    //unsigned char intState;
    unsigned char bitsRead;
    
    // Disable interrupts.
    //intState = __save_interrupt();
    //__disable_interrupt();
	
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		// Drive bus low and delay.
		OWI_DDR |= (1 << DS1821_PIN_DQ);
		_delay_us(OWI_DELAY_A_STD_MODE);
		//__builtin_avr_delay_cycles(OWI_DELAY_A_STD_MODE);
    
		// Release bus and delay.
		OWI_DDR &= ~(1 << DS1821_PIN_DQ);
		_delay_us(OWI_DELAY_E_STD_MODE);
		//__builtin_avr_delay_cycles(OWI_DELAY_E_STD_MODE);
    
		// Sample bus and delay.
		bitsRead = OWI_PIN & (1 << DS1821_PIN_DQ);
		_delay_us(OWI_DELAY_F_STD_MODE);
		//__builtin_avr_delay_cycles(OWI_DELAY_F_STD_MODE);
	}    
    // Restore interrupts.
    //__restore_interrupt(intState);
    
    return bitsRead;
}


/*! \brief  Send a Reset signal and listen for Presence signal. (software
 *  only driver)
 *
 *  Generates the waveform for transmission of a Reset pulse on the 
 *  1-Wire(R) bus and listens for presence signals.
 *
 *  \param  pins    A bitmask of the buses to send the Reset signal on.
 *
 *  \return A bitmask of the buses where a presence signal was detected.
 */
unsigned char OWI_DetectPresence()
{
    //unsigned char intState;
    unsigned char presenceDetected;
    
    // Disable interrupts.
    //intState = __save_interrupt();
    //__disable_interrupt();
	
    if ((~OWI_PIN) & (1 << DS1821_PIN_DQ)) //Check if bus is locked to low by DS1821
    {
		return 0x00;
    }
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		// Drive bus low and delay.
		OWI_DDR |= (1 << DS1821_PIN_DQ);
		_delay_us(OWI_DELAY_H_STD_MODE);
		//__builtin_avr_delay_cycles(OWI_DELAY_H_STD_MODE);
    
		// Release bus and delay.
		OWI_DDR &= ~(1 << DS1821_PIN_DQ);
		_delay_us(OWI_DELAY_I_STD_MODE);
		//__builtin_avr_delay_cycles(OWI_DELAY_I_STD_MODE);
    
		// Sample bus to detect presence signal and delay.
		presenceDetected = ((~OWI_PIN) & (1 << DS1821_PIN_DQ));
		_delay_us(OWI_DELAY_J_STD_MODE);
		//__builtin_avr_delay_cycles(OWI_DELAY_J_STD_MODE);
	}    
    // Restore interrupts.
    //__restore_interrupt(intState);
	
    return presenceDetected;
}

/*! \brief  Sends one byte of data on the 1-Wire(R) bus(es).
 *  
 *  This function automates the task of sending a complete byte
 *  of data on the 1-Wire bus(es).
 *
 *  \param  data    The data to send on the bus(es).
 *  
 *  \param  pins    A bitmask of the buses to send the data to.
 */
void OWI_SendByte(unsigned char data)
{
    unsigned char temp;
    unsigned char i;
    
    // Do once for each bit
    for (i = 0; i < 8; i++)
    {
        // Determine if lsb is '0' or '1' and transmit corresponding
        // waveform on the bus.
        temp = data & 0x01;
        if (temp)
        {
            OWI_WriteBit1();
        }
        else
        {
            OWI_WriteBit0();
        }
        // Right shift the data to get next bit.
        data >>= 1;
    }
}

/*! \brief  Receives one byte of data from the 1-Wire(R) bus.
 *
 *  This function automates the task of receiving a complete byte 
 *  of data from the 1-Wire bus.
 *
 *  \param  pin     A bitmask of the bus to read from.
 *  
 *  \return     The byte read from the bus.
 */
unsigned char OWI_ReceiveByte()
{
    unsigned char data;
    unsigned char i;

    // Clear the temporary input variable.
    data = 0x00;
    
    // Do once for each bit
    for (i = 0; i < 8; i++)
    {
        // Shift temporary input variable right.
        data >>= 1;
        // Set the msb if a '1' value is read from the bus.
        // Leave as it is ('0') else.
        if (OWI_ReadBit())
        {
            // Set msb
            data |= 0x80;
        }
    }
    return data;
}


