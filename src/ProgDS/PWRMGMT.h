#ifndef _PWRMGMT_H_
#define _PWRMGMT_H_

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#define PWR_TIMEOUT	0xFF
#define DIM_TIMEOUT 0xBB
#define BKL_TIMEOUT 0x77

#define DIM_POWER	0x44

extern unsigned char pwrTimeout;

extern inline void PowerInit(void);

#endif