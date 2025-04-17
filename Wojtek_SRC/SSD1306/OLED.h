#ifndef OLED_H
#define OLED_H

 #define LCD_I2C_ADDR			 	0x3c		//	0x7A

 #define SSD1306_BUFFER_WIDTH        128
 #define SSD1306_BUFFER_HEIGHT       64
 #define SSD1306_FILL_BORDER		 	(SSD1306_BUFFER_WIDTH * (SSD1306_BUFFER_HEIGHT/8))
 #define SSD1306_BUFFER_SIZE 	 	SSD1306_FILL_BORDER
 #define LCD128x64//LCD128x32	//

 #ifdef STM32F103C8

  #include "stm32f10x_i2c.h"
  #define B16_LSB(x)	((uint8_t)(x & 0xFF))
  #define B16_MSB(x)	((uint8_t)((x>>8) & 0x00FF))
  #define OLED_STM_Version

 #endif



 typedef struct {
	I2C_TypeDef* I2Cx;
	uint8_t I2CAddr;
	uint8_t CurrentX;
	uint8_t CurrentY;
	uint8_t Inverted;
	uint8_t sreg;
	uint8_t FontHeight;
	uint8_t FontWidth;
	uint16_t *FontDataPointer;

 #ifdef OLED_STM_Version

	uint8_t Buffer[SSD1306_FILL_BORDER];
 #else
	uint8_t Buffer[SSD1306_WIDTH/8][SSD1306_HEIGHT];

 #endif

 }OLEDType;


 // SREG bits definition
 #define OLED_TO_BE_INIT 	0x01
 #define OLED_TO_BE_IVERT 	0x02
 #define OLED_REFRESH	 	0x04
 #define OLED_MISSING		0x80


 typedef enum {
	SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	SSD1306_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
 } SSD1306_COLOR_t;

 #define FONT_11X18	//FONT_11X18//FONT_5X8//
 //#define FONT_5X8
 //#define FONT_7X10

 #ifdef FONT_11X18

  #define FONT_WIDTH	11
  #define FONT_HIGH 	18
  #define CHAR_MASK	0x7FF
  extern const uint16_t FONT_11x18_DATA[];			//#include "ssd1306_font_11_18.h"
 #endif





 #ifdef FONT_7X10
 // #include "ssd1306_font_7_10.h"
  extern const uint16_t FONT_7x10_DATA[];
  #define FONT_WIDTH	7
  #define FONT_HIGH 	10
 #endif

 #ifdef STM32F103C8
  #include "stm32f10x_rcc.h"
  #include "stm32f10x_gpio.h"
  #include "stm32f10x_i2c.h"
 #endif


 #ifdef FONT_5X8
  //#include "ssd1306_font_5x8.h"
  extern const unsigned char ssd1306_font_5x8[];
  #define FONT_WIDTH	5
  #define FONT_HIGH 	8
  #define CHAR_MASK	0xFF

 #endif

 #define I2C1_ENABLE


 #ifdef I2C1_ENABLE
	#define LCD_I2C		I2C1
	#define LCD_RCC_I2C_ENABLE		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE)

 #else
	#define LCD_I2C		I2C2
	#define LCD_RCC_I2C_ENABLE		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE)
 #endif




 typedef enum{
		I2C_CHECK_BUSY=0,
		I2C_START,
		I2C_SEND_ADDR,
		I2C_SEND_REG,
		I2C_SEND_START_DATA_READ,
		I2C_SEND_ADDR_TO_READ,
		I2C_READ_DATA,
		RETURN_DATA
 } ReadRegState_t;

 typedef enum {

	I2C_WRITE_CHECK_BUSY=0,
	I2C_WRITE_START,
	I2C_WRITE_SEND_ADDR,
	I2C_WRITE_SEND_REG,
	I2C_WRITE_DATA,
	I2C_WRITE_STOP


 }WriteRegState_t;


 enum {
	I2C_WRITE_OK=0,
	I2C_WRITE_ERROR,
	I2C_WRITE_INWORK


 };

 #define I2C_TIMEOUT_VAL		180

 //SSD COMMANDS
 #define SLAVE_ADDRESS                             0x3C//0x78
 #define SSD1306_WIDTH                             SSD1306_BUFFER_WIDTH
 #define SSD1306_HEIGHT                            SSD1306_BUFFER_HEIGHT
 #define SSD1306_COLOR_BRIGHT                      0xFF
 #define SSD1306_COLOR_DARK                        0x00
 /* SSD1306 commands. */
 #define SSD1306_SETLOWCOLUMN                      0x00
 #define SSD1306_EXTERNALVCC                       0x01
 #define SSD1306_SWITCHCAPVCC                      0x02
 #define SSD1306_SETHIGHCOLUMN                     0x10
 #define SSD1306_MEMORYMODE                        0x20

 #define SSD1306_MEMORYMODE_H                      0x20
 #define SSD1306_MEMORYMODE_V                      0x21
 #define SSD1306_MEMORYMODE_PAGE                   0x22

 #define SSD1306_COLUMNADDR                        0x21
 #define SSD1306_PAGEADDR                          0x22
 #define SSD1306_RIGHT_HORIZONTAL_SCROLL           0x26
 #define SSD1306_LEFT_HORIZONTAL_SCROLL            0x27
 #define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL       0x29
 #define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL       0x2A
 #define SSD1306_DEACTIVATE_SCROLL                 0x2E
 #define SSD1306_ACTIVATE_SCROLL                   0x2F
 #define SSD1306_SETSTARTLINE                      0x40
 #define SSD1306_SETCONTRAST                       0x81
 #define SSD1306_CHARGEPUMP                        0x8D
 #define SSD1306_SEGREMAP                          0xA0
 #define SSD1306_SET_VERTICAL_SCROLL_AREA          0xA3
 #define SSD1306_DISPLAYALLON_RESUME               0xA4
 #define SSD1306_DISPLAYALLON                      0xA5
 #define SSD1306_NORMALDISPLAY                     0xA6
 #define SSD1306_INVERTDISPLAY                     0xA7
 #define SSD1306_SETMULTIPLEX                      0xA8
 #define SSD1306_DISPLAYOFF                        0xAE
 #define SSD1306_DISPLAYON                         0xAF
 #define SSD1306_SETPAGESTARTADDRESS               0xB0
 #define SSD1306_COMSCANINC                        0xC0
 #define SSD1306_COMSCANDEC                        0xC8
 #define SSD1306_SETDISPLAYOFFSET                  0xD3
 #define SSD1306_SETDISPLAYCLOCKDIV                0xD5
 #define SSD1306_SETPRECHARGE                      0xD9
 #define SSD1306_SETCOMPINS                        0xDA
 #define SSD1306_SETVCOMDESELECT                   0xDB


 static const uint8_t Init_Table[]=
 {


 #ifdef OLED_STM_Version
  #ifdef LCD128x32
		  0xAE, //display off
		  0xA8, //--set multiplex ratio(1 to 64)
		  0x1F, //
		  0xD3, //-set display offset
		  0x00, //
		  0x40, //--set start line address
		  0x20, //Set Memory Addressing Mode
		  0x02, //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
		  0xA1, //--set segment re-map 0 to 127
		  0xC8, //Set COM Output Scan Direction
		  0xDA, //--set com pins hardware configuration
		  0x02, //
		  0x81, //--set contrast control register
		  0x1F, //
		  0xA4, //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
		  0xA6, //--set normal display mode
		  0xD5, //--set display clock divide ratio/oscillator frequency
		  0x80, //--set divide ratio
		  0x8D, //--set DC-DC enable
		  0x14, //
		  0x2E, //Disable Scroll
		  0xAF, //--turn on SSD1306 panel

  #endif
  #ifdef LCD128x64
		  SSD1306_DISPLAYOFF, 			//0xAE	wy³¹cz panel OLED
		  SSD1306_SETLOWCOLUMN,			// 0x00,	adres koluny LOW
		  SSD1306_SETHIGHCOLUMN,		//0x10,	//adres koluny HIGH
		  SSD1306_SETSTARTLINE,			// 0x40,	//adres startu linii
	      SSD1306_COLUMNADDR,
	      0,
	      127,
	      SSD1306_PAGEADDR,
	      0,
	      7,
		  SSD1306_MEMORYMODE_H	,		//0x20,	//tryb adresowania strony
		  0x02, 						//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
		  SSD1306_SETCONTRAST,			//0x81,	//ustaw kontrast
		  0xCF,							// contrast value	w 128x32 jest 0x1F tutaj CF...??
		  SSD1306_SEGREMAP ,			//SSD1306_SEGREMAP ustaw remapowanie
		  SSD1306_COMSCANINC, 			//	0xC0,	//kierunek skanowania
		  SSD1306_NORMALDISPLAY, 		//0xA6,	//wyœwietlanie bez inwersji
		  SSD1306_SETMULTIPLEX,			//	ustaw multiplex ratio
		  0x3F,							//	1/64
		  SSD1306_SETDISPLAYOFFSET, 	//0xD3,	//ustaw display offset
		  0x00,							//bez offsetu
		  SSD1306_SETDISPLAYCLOCKDIV, 	//0xD5,	//ustaw divide ratio/czêstotliwoœæ oscylatora
		  0x80,							//100ramek/sec
		  SSD1306_SETPRECHARGE, 		//0xD9,	//ustaw okres pre charge
		  0xF1,							//pre charge 15 cykli, discharge 1 cykl
		  SSD1306_SETCOMPINS, 			//0xDA,	//konfiguracja wyprowadzeñ sterownika
		  0x12,
		  SSD1306_SETVCOMDESELECT, 		//0xDB,							//ustawienie vcomh
		  0x40,
		  SSD1306_CHARGEPUMP, 			//0x8D,	//ustawienie Charge Pump
		  0x14,
		  SSD1306_DISPLAYALLON_RESUME, 	// 0xA4, //”pod³¹czenie” zawartoœci RAM do panelu OLED
		  SSD1306_NORMALDISPLAY, 		//0xA6,	//wy³¹czenie inwersji wyœwietlania
		  SSD1306_DISPLAYON, 			//0xAF	//w³¹cza wyœwietlacz

		  /*  SSD1306_DISPLAYOFF,
		      SSD1306_SETLOWCOLUMN,
		      SSD1306_SETHIGHCOLUMN,
		      SSD1306_SETPAGESTARTADDRESS,
		      SSD1306_SETSTARTLINE,
		      SSD1306_SEGREMAP | 0x01,
		      SSD1306_SETCOMPINS,
		      0x02,   // * Set com pins data.
		      SSD1306_SETDISPLAYOFFSET,
		      0x00,   // * Set display offset data. No offset.
		      SSD1306_COMSCANDEC,
		      SSD1306_NORMALDISPLAY,
		      SSD1306_DISPLAYALLON_RESUME,
		      SSD1306_SETCONTRAST,
		      0x1F,   // * Set contrast data.
		      SSD1306_MEMORYMODE,
		      0x00,   // * Memory addressing mode data. 00 -> Horizontal addressing. 01 -> Vertical addressing.
		      SSD1306_COLUMNADDR,
		      0,
		      127,
		      SSD1306_PAGEADDR,
		      0,
		      7,
		      SSD1306_SETMULTIPLEX,
		      0x1F,   // * Set MUX ratio data. 1/32 duty cycle.
		      SSD1306_SETPRECHARGE,
		      0xF1,   // * Set pre-charge period data.
		      SSD1306_SETVCOMDESELECT,
		      0x40,   //*  Set V com deselect data.
		      SSD1306_CHARGEPUMP,
		      0x14,   // * Charge pump setting data.
		      SSD1306_DISPLAYON,*/
  #endif



 #endif
 };


 OLEDType LCD_OLED;




 #endif





// *****************************************
// ****************** LCD COMMANDS **************
/*
 *    Init LCD
  SSD1306_WRITECOMMAND(0xAE); //display off
  SSD1306_WRITECOMMAND(0xA8); //--set multiplex ratio(1 to 64)
  SSD1306_WRITECOMMAND(0x1F); //
  SSD1306_WRITECOMMAND(0xD3); //-set display offset
  SSD1306_WRITECOMMAND(0x00); //
  SSD1306_WRITECOMMAND(0x40); //--set start line address
  SSD1306_WRITECOMMAND(0x20); //Set Memory Addressing Mode
  SSD1306_WRITECOMMAND(0x02); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  SSD1306_WRITECOMMAND(0xA1); //--set segment re-map 0 to 127
  SSD1306_WRITECOMMAND(0xC8); //Set COM Output Scan Direction
  SSD1306_WRITECOMMAND(0xDA); //--set com pins hardware configuration
  SSD1306_WRITECOMMAND(0x02); //
  SSD1306_WRITECOMMAND(0x81); //--set contrast control register
  SSD1306_WRITECOMMAND(0x1F); //
  SSD1306_WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  SSD1306_WRITECOMMAND(0xA6); //--set normal display mode
  SSD1306_WRITECOMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
  SSD1306_WRITECOMMAND(0x80); //--set divide ratio
  SSD1306_WRITECOMMAND(0x8D); //--set DC-DC enable
  SSD1306_WRITECOMMAND(0x14); //
  SSD1306_WRITECOMMAND(0x2E); //Disable Scroll
  SSD1306_WRITECOMMAND(0xAF); //--turn on SSD1306 panel
  SSD1306_WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
  SSD1306_WRITECOMMAND(0x00); //---set low column address
  SSD1306_WRITECOMMAND(0x10); //---set high column address
  SSD1306_WRITECOMMAND(0xD9); //--set pre-charge period
  SSD1306_WRITECOMMAND(0x22); //
  SSD1306_WRITECOMMAND(0xDB); //--set vcomh
  SSD1306_WRITECOMMAND(0x20); //0x20,0.77xVcc
  SSD1306_WRITECOMMAND(0x2E); //Disable Scroll
  SSD1306_WRITECOMMAND(0x2F); //Disable Scroll
  SSD1306_WRITECOMMAND(0xA5); //--turn on SSD1306 panel
 */
