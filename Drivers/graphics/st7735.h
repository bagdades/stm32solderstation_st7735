/* vim: set ai et ts=4 sw=4: */
#ifndef __ST7735_H__
#define __ST7735_H__

#include "fonts.h"
#include <stdbool.h>
#include "stm32f1xx_hal.h"
// #include "stm32f1xx_hal_spi.h"

#define LCD_SPI_HAND        SPI2

#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_ML  0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

/*** Redefine if necessary ***/
#define ST7735_SPI_PORT hspi1
extern SPI_HandleTypeDef ST7735_SPI_PORT;

// #define ST7735_RES_Pin       GPIO_PIN_0
// #define ST7735_RES_GPIO_Port GPIOA
// #define ST7735_CS_Pin        GPIO_PIN_2
// #define ST7735_CS_GPIO_Port  GPIOA
// #define ST7735_DC_Pin        GPIO_PIN_1
// #define ST7735_DC_GPIO_Port  GPIOA

// AliExpress/eBay 1.8" display, default orientation
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY)
*/

// AliExpress/eBay 1.8" display, rotate right
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV)
*/

// AliExpress/eBay 1.8" display, rotate left
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV)
*/

// AliExpress/eBay 1.8" display, upside down
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (0)
*/

// WaveShare ST7735S-based 1.8" display, default orientation
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 2
#define ST7735_YSTART 1
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB)
*/

// WaveShare ST7735S-based 1.8" display, rotate right
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#define ST7735_XSTART 1
#define ST7735_YSTART 2
#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB)
*/

// WaveShare ST7735S-based 1.8" display, rotate left
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#define ST7735_XSTART 1
#define ST7735_YSTART 2
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB)
*/

// WaveShare ST7735S-based 1.8" display, upside down
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 2
#define ST7735_YSTART 1
#define ST7735_ROTATION (ST7735_MADCTL_RGB)
*/

// 1.44" display, default orientation
//#define ST7735_IS_128X128 1
//#define ST7735_WIDTH  128
//#define ST7735_HEIGHT 128
//#define ST7735_XSTART 2
//#define ST7735_YSTART 3
//#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR)

// 1.44" display, rotate right
/*
#define ST7735_IS_128X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 128
#define ST7735_XSTART 3
#define ST7735_YSTART 2
#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)
*/

// 1.44" display, rotate left
/*
#define ST7735_IS_128X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 128
#define ST7735_XSTART 1
#define ST7735_YSTART 2
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)
*/

// 1.44" display, upside down
/*
#define ST7735_IS_128X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 128
#define ST7735_XSTART 2
#define ST7735_YSTART 1
#define ST7735_ROTATION (ST7735_MADCTL_BGR)
*/

// mini 160x80 display (it's unlikely you want the default orientation)
/*
#define ST7735_IS_160X80 1
#define ST7735_XSTART 26
#define ST7735_YSTART 1
#define ST7735_WIDTH  80
#define ST7735_HEIGHT 160 
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR)
*/

// mini 160x80, rotate left
#define ST7735_IS_160X80 1
#define ST7735_XSTART 1
#define ST7735_YSTART 26
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 80
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)

// mini 160x80, rotate right 
/*
#define ST7735_IS_160X80 1
#define ST7735_XSTART 1
#define ST7735_YSTART 26
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 80
#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)
*/

/****************************/

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define	C_BLACK   0x0000
#define	C_BLUE    0x001F
#define	C_RED     0xF800
#define	C_GREEN   0x07E0
#define C_CYAN    0x07FF
#define C_MAGENTA 0xF81F
#define C_YELLOW  0xFFE0
#define C_WHITE   0xFFFF
#define C_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

// typedef struct _ucg_t ucg_t;
typedef struct ucg_t
{
    //display width
    uint16_t x_dim;
    //display height
    uint16_t y_dim;
    uint16_t backcolor;
    uint16_t forecolor;
    // FontDef* font;
    const tFont* font;
}ucg_t;

typedef enum{
	error_NMI,
	error_HARDFAULT,
	error_MEMMANAGE,
	error_BUSFAULT,
	error_USAGEFAULT,
	error_RUNAWAY25,
	error_RUNAWAY50,
	error_RUNAWAY75,
	error_RUNAWAY100,
	error_RUNAWAY500,
  error_FLASH,
}FatalErrors;


// call before initializing any SPI devices
void ST7735_Unselect();

void ucg_Init(void);
// void ucg_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ucg_DrawPixel(ucg_t* ucg, uint16_t x, uint16_t y);
// void ucg_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void ucg_WriteString(ucg_t* ucg, uint16_t x, uint16_t y, const char* str);
void ucg_FillRectangle(ucg_t* ucg, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
// void ucg_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ucg_FillScreen(ucg_t* ucg);
void ucg_FillCircle(ucg_t* ucg, int16_t x0, int16_t y0, int16_t r);
void ucg_FillRoundFrame(ucg_t* ucg, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t r);
void ucg_FillFrame(ucg_t* ucg, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
// void ucg_FillScreen(uint16_t color);
void ucg_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
// void ucg_DrawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
uint16_t ucg_GetYDim(ucg_t *ucg);
uint16_t ucg_GetXDim(ucg_t *ucg);
// void ucg_SetColor(ucg_t* ucg, uint8_t idx, uint16_t color);
void ucg_SetBackColor(ucg_t* ucg, uint16_t color);
void ucg_SetForeColor(ucg_t* ucg, uint16_t color);
uint16_t ucg_GetBackColor(ucg_t* ucg);
uint16_t ucg_GetForeColor(ucg_t* ucg);
void ucg_SetFont(ucg_t* ucg, const tFont* font);
const tFont* ucg_GetFont(ucg_t *ucg);
void ucg_DrawLine(ucg_t* ucg, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ucg_DrawArc(ucg_t* ucg, int16_t x0, int16_t y0, int16_t r, uint8_t s);
void ucg_DrawCircle(ucg_t* ucg, int16_t x0, int16_t y0, int16_t r);
void ucg_DrawRBox(ucg_t *ucg, int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);
void ucg_DrawRoundFrame(ucg_t* ucg, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t r);
void ucg_DrawRectangle(ucg_t* ucg, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
uint8_t ucg_GetStrWidth(ucg_t *ucg, const tFont* font, const char *str);
void ucg_InvertColors(bool invert);
void ucg_DrawBmp(uint16_t x, uint16_t y, const tImage* img, uint16_t color, uint16_t bgcolor);
void FatalError(uint8_t type);

#endif // __ST7735_H__
