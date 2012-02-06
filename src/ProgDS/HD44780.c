//-------------------------------------------------------------------------------------------------
// Wyœwietlacz alfanumeryczny ze sterownikiem HD44780
// Sterowanie w trybie 4-bitowym bez odczytu flagi zajêtoœci
// z dowolnym przypisaniem sygna³ów steruj¹cych
// Plik : HD44780.c	
// Mikrokontroler : Atmel AVR
// Kompilator : avr-gcc
// Autor : Rados³aw Kwiecieñ
// ród³o : http://radzio.dxp.pl/hd44780/
// Data : 24.03.2007
//-------------------------------------------------------------------------------------------------

#include "HD44780.h"
//-------------------------------------------------------------------------------------------------
//
// Funkcja wystawiaj¹ca pó³bajt na magistralê danych
//
//-------------------------------------------------------------------------------------------------
void _LCD_OutNibble(unsigned char nibbleToWrite) //informacja w 0xF0
{
	LCD_PORT &= 0b10000111;
	LCD_PORT |= ((nibbleToWrite >> 1) & 0b01111000);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu bajtu do wyœwietacza (bez rozró¿nienia instrukcja/dane).
//
//-------------------------------------------------------------------------------------------------
void _LCD_Write(unsigned char dataToWrite)
{
	LCD_PORT |= LCD_E;
	_LCD_OutNibble(dataToWrite);
	LCD_PORT &= ~LCD_E;
	LCD_PORT |= LCD_E;
	_LCD_OutNibble(dataToWrite << 4);
	LCD_PORT &= ~LCD_E;
	_delay_us(50);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu rozkazu do wyœwietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteCommand(unsigned char commandToWrite)
{
	LCD_PORT &= ~LCD_RS;
	_LCD_Write(commandToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu danych do pamiêci wyœwietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteData(unsigned char dataToWrite)
{
	LCD_PORT |= LCD_RS;
	_LCD_Write(dataToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja wyœwietlenia napisu na wyswietlaczu.
//
//-------------------------------------------------------------------------------------------------

/*
void LCD_WriteText(char *text)
{
	while(*text)
	{
		LCD_WriteData(*(text++));
	}
}*/

void LCD_WriteTextFromPGM(PGM_P pgm_text)
{
	char buffer;
	while (buffer = pgm_read_byte(pgm_text++))
	{
		LCD_WriteData(buffer);
	}
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja ustawienia wspó³rzêdnych ekranowych
//
//-------------------------------------------------------------------------------------------------
void LCD_GoTo(unsigned char x, unsigned char y)
{
	LCD_WriteCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja czyszczenia ekranu wyœwietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Clear(void)
{
	LCD_WriteCommand(HD44780_CLEAR);
	_delay_ms(1.5);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja przywrócenia pocz¹tkowych wspó³rzêdnych wyœwietlacza.
//
//-------------------------------------------------------------------------------------------------
/*
inline void LCD_Home(void)
{
	LCD_WriteCommand(HD44780_HOME);
	_delay_ms(1.5);
}*/
//-------------------------------------------------------------------------------------------------
//
// Procedura inicjalizacji kontrolera HD44780.
//
//-------------------------------------------------------------------------------------------------
inline void LCD_Initalize(void)
{
	unsigned char i;
	DDRB |= LCD_BACKLIGHT;
	//PORTB &= ~LCD_BACKLIGHT; // Wy³¹czenie podœwietlenia

	
	LCD_DIR = 0xFF; // Konfiguracja kierunku pracy wyprowadzeñ
	LCD_PORT = ~LCD_POWER; // Wy³¹czenie wyœwietlacza, reszta wyprowadzeñ HIGH
	_delay_ms(250);
	//LCD_PORT &= ~(LCD_RS | LCD_E); // wyzerowanie linii RS i E
	LCD_PORT = LCD_POWER; // W³¹czenie wyœwietlacza
	 
	
	_delay_ms(50); // w/g dokumentacji
	//_delay_ms(100); // oczekiwanie na ustalibizowanie siê napiecia zasilajacego

/*
	for (i = 0; i < 3; i++)
	{
		LCD_PORT |= LCD_E; //  E = 1
		_LCD_OutNibble(0x30); // 
		LCD_PORT &= ~LCD_E; // E = 0
		_delay_ms(5);
	}*/
	
	LCD_PORT |= LCD_E; //  E = 1
	_LCD_OutNibble(HD44780_FUNCTION_SET); // 
	LCD_PORT &= ~LCD_E; // E = 0
	_delay_ms(1);
	
	LCD_WriteCommand(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT); // interfejs 4-bity, 2-linie, znak 5x7
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK); // wy³¹czenie wyswietlacza
	LCD_Clear(); // czyszczenie zawartosæi pamieci DDRAM
	LCD_WriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);// inkrementaja adresu i przesuwanie kursora
	
	// Za³¹czenia Timera1 jako PWM steruj¹cego podœwietleniem wyœwietlacza.
	TCCR1A = (1 << COM1B1) | (0 << COM1B0) | (0 << WGM11) | (1 << WGM10);
	TCCR1B = (0 << WGM13) | (0 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);
	//OCR1BL = 0x88; // Jasnoœæ wyœwietlacza
}

//-------------------------------------------------------------------------------------------------
//
// Koniec pliku HD44780.c
//
//-------------------------------------------------------------------------------------------------

void LCD_ShowNumber(signed char number)
{
	char i;
	char buffer[5];
	
	if (number > 0)
	{
		buffer[0] = '+';
		number *= -1;
	}
	else
	{
		buffer[0] = '-';
	}
	
	for (i = 3; i != 0; i--)
	{
		buffer[i] = -(number % -10) + '0';
		number /= 10;
	}
	buffer[4] = '\0';
	
	while (buffer[i])
	{
		LCD_WriteData(buffer[i++]);
	}
}
