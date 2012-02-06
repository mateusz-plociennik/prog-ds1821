#ifndef _MENU_H_
#define _MENU_H_

#include <stdlib.h>
#include <avr/pgmspace.h>

#include "OWI.h"
#include "DS.h"
#include "HD44780.h"
#include "KEY.h"

#define MENU_UPDATE_NEVER	0x00
#define MENU_UPDATE_ONCE	0x01
#define MENU_UPDATE_ALWAYS	0x02

#define MENU_STATE_INIT		0x00
#define MENU_STATE_READ_T	0x10

#define MENU_STATE_READ_TL	0x20
#define MENU_STATE_SET_TL	0x21
#define MENU_STATE_READ_TH	0x30
#define MENU_STATE_SET_TH	0x31
#define MENU_STATE_READ_POL	0x40
#define MENU_STATE_SET_POL	0x41

#define MENU_STATE_THERMO	0x50

void MENU_ChangeState();
void MENU_Update();

extern unsigned char menuState;

#endif