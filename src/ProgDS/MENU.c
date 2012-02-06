#include "MENU.h"
/*                          "1234567890123456"*/
const char MN00[] PROGMEM = "0. Wyszuk.DS1821";
const char MN01[] PROGMEM = "Znaleziono";
const char MN10[] PROGMEM = "1. Odczyt temp.";
const char MN20[] PROGMEM = "2. Ustaw temp.TL";
const char MN30[] PROGMEM = "3. Ustaw temp.TH";
const char MN40[] PROGMEM = "4. Ustaw polar.";
const char MN50[] PROGMEM = "5. Tryb termos.";
const char MN51[] PROGMEM = ", DQ=";

unsigned char menuState = MENU_STATE_INIT;
char key;

signed char temperature, tempL, tempH;
unsigned char status, polar;

void MENU_Update() //zmiana stanu menu po naciœniêciu przycisku
{
	if (!OWI_DetectPresence() && menuState != MENU_STATE_THERMO) // Jeœli podczas programowania zostanie od³¹czony DS, wróæ do inicjalizacji 1-Wire
	{
		menuState = MENU_STATE_INIT;
	}
	
	LCD_Clear();
	switch(menuState)
	{
		case MENU_STATE_INIT:
		{
			LCD_WriteTextFromPGM(MN00);
			DS1821_ModeToggle();
			if (OWI_DetectPresence())
			{
				LCD_GoTo(3,1);
				LCD_WriteTextFromPGM(MN01);
				menuState = MENU_STATE_READ_T;
				pwrTimeout = PWR_TIMEOUT;
			}
			break;
		}			
		case MENU_STATE_READ_T:
		{
			LCD_WriteTextFromPGM(MN10);
			LCD_GoTo(3,1);
			LCD_ShowNumber(temperature);
			switch (key)
			{
				case KEY_UP:
				{
					menuState = MENU_STATE_THERMO;
					break;
				}					
				case KEY_DOWN:
				{
					menuState = MENU_STATE_READ_TL;
					break;
				}
				default:
				{
					DS1821_ReadTemperature(&temperature);
				}					
			}
			break;
		}			
		case MENU_STATE_READ_TL:
		{
			LCD_WriteTextFromPGM(MN20);
			switch (key)
			{
				case KEY_UP:
				{
					menuState = MENU_STATE_READ_T;
					break;
				}					
				case KEY_DOWN:
				{
					menuState = MENU_STATE_READ_TH;
					break;
				}
				case KEY_LEFT:
				{
					OWI_SendByte(DS1821_WRITE_TL);
					OWI_SendByte(--tempL);
					break;
				}
				case KEY_RIGHT:
				{
					OWI_SendByte(DS1821_WRITE_TL);
					OWI_SendByte(++tempL);
					break;
				}
				default:
				{
					OWI_SendByte(DS1821_READ_TL);
					tempL = OWI_ReceiveByte();
				}
			}
			LCD_GoTo(3,1);
			LCD_ShowNumber(tempL);
			break;
		}
		case MENU_STATE_READ_TH:
		{
			LCD_WriteTextFromPGM(MN30);
			switch (key)
			{
				case KEY_UP:
				{
					menuState = MENU_STATE_READ_TL;
					break;
				}					
				case KEY_DOWN:
				{
					menuState = MENU_STATE_READ_POL;
					break;
				}
				case KEY_LEFT:
				{
					OWI_SendByte(DS1821_WRITE_TH);
					OWI_SendByte(--tempH);
					break;
				}
				case KEY_RIGHT:
				{
					OWI_SendByte(DS1821_WRITE_TH);
					OWI_SendByte(++tempH);
					break;
				}
				default:
				{
					OWI_SendByte(DS1821_READ_TH);
					tempH = OWI_ReceiveByte();
				}
			}
			LCD_GoTo(3,1);
			LCD_ShowNumber(tempH);
			break;
		}
		case MENU_STATE_READ_POL:
		{
			LCD_WriteTextFromPGM(MN40);
			switch (key)
			{
				case KEY_UP:
				{
					menuState = MENU_STATE_READ_TH;
					break;
				}					
				case KEY_DOWN:
				{
					menuState = MENU_STATE_THERMO;
					break;
				}
				case KEY_LEFT:
				case KEY_RIGHT:
				{
					status ^= DS1821_POL;
					OWI_SendByte(DS1821_WRITE_STATUS);
					OWI_SendByte(status);
					break;
				}
				default:
				{
					OWI_SendByte(DS1821_READ_STATUS);
					status = OWI_ReceiveByte();
				}
			}
			LCD_GoTo(3,1);
			LCD_WriteData(((status & DS1821_POL) >> 1) + '0');
			break;
		}
		case MENU_STATE_THERMO:
		{
			LCD_WriteTextFromPGM(MN50);
			switch (key)
			{
				case KEY_UP:
				{
					menuState = MENU_STATE_READ_POL;
					break;
				}					
				case KEY_DOWN:
				{
					menuState = MENU_STATE_READ_T;
					break;
				}
				case KEY_LEFT:
				case KEY_RIGHT:
				{
					status |= DS1821_T_R;
					OWI_SendByte(DS1821_WRITE_STATUS);
					OWI_SendByte(status);
					DS1821_ModeToggle();
					break;
				}
			}
			LCD_GoTo(3,1);
			LCD_WriteData('1' - OWI_DetectPresence());
			LCD_WriteTextFromPGM(MN51);
			LCD_WriteData('0' + (OWI_PIN & (1 >> DS1821_PIN_DQ)));
			break;
		}								
	}
	
	return;
}
