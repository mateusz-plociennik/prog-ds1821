#ifndef _KEY_H_
#define _KEY_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "HD44780.h"
#include "MENU.h"
#include "PWRMGMT.h"

#define KEY_DDR			DDRB
#define KEY_PORT		PORTB
#define KEY_PIN			PINB

#define KEY_ROW_789		(1 << PORTB7)
#define KEY_ROW_456		(1 << PORTB6)
#define KEY_ROW_123		(1 << PORTB5)
#define KEY_ROW_S0E		(1 << PORTB3)
#define KEY_COL_S147	(1 << PORTB2)
#define KEY_COL_0258	(1 << PORTB1)
#define KEY_COL_E369	(1 << PORTB0)

#define KEY_ROWS		0xE8
#define KEY_COLS		0x07

#define KEY_NOKEY	'X'
#define KEY_DOWN	'2'
#define KEY_LEFT	'4'
#define KEY_RIGHT	'6'
#define KEY_UP		'8'
#define KEY_ENTER	'E'

#define KEY_SPEED	0x11

extern char key;

extern inline void KEY_Init();
char KEY_Get();

#endif