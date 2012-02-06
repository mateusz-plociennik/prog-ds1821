#ifndef _OWI_H_
#define _OWI_H_

#include <avr/io.h>
#include <avr/delay.h>
#include <util/atomic.h>

#include "DS.h"

#define     OWI_SOFTWARE_DRIVER    

#define     OWI_PORT        PORTA   //!< 1-Wire PORT Data register.
#define     OWI_PIN         PINA    //!< 1-Wire Input pin register.
#define     OWI_DDR         DDRA    //!< 1-Wire Data direction register.

//#define     OWI_DELAY_OFFSET_CYCLES		13   //!< Timing delay when pulling bus low and releasing bus.

// Bit timing delays in clock cycles (= us*clock freq in MHz).
#define     OWI_DELAY_A_STD_MODE		6
#define     OWI_DELAY_B_STD_MODE		64
#define     OWI_DELAY_C_STD_MODE		60
#define     OWI_DELAY_D_STD_MODE		10
#define     OWI_DELAY_E_STD_MODE		9
#define     OWI_DELAY_F_STD_MODE		55
//#define     OWI_DELAY_G_STD_MODE		((0 * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_H_STD_MODE		480
#define     OWI_DELAY_I_STD_MODE		70
#define     OWI_DELAY_J_STD_MODE		410   



extern inline void OWI_Init();
void OWI_WriteBit1();
void OWI_WriteBit0();
unsigned char OWI_ReadBit();
unsigned char OWI_DetectPresence();

void OWI_SendByte(unsigned char data);
unsigned char OWI_ReceiveByte();

#endif
