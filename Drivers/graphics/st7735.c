/* vim: set ai et ts=4 sw=4: */
#include "stm32f1xx_hal.h"
#include "st7735.h"
#include "stm32f1xx_hal_spi.h"
#include "main.h"
#include "settings.h"
#include <stdint.h>
#include "iron.h"
#include "gui_strings.h"

#define DELAY 0x80
extern ucg_t ucg;

// based on Adafruit ST7735 library for Arduino
static const uint8_t
  init_cmds1[] = {            // Init for 7735R, part 1 (red or green tab)
    15,                       // 15 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
      150,                    //     150 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      255,                    //     500 ms delay
    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                   //     No inversion
    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                   //     -4.6V
      0x84,                   //     AUTO mode
    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      0x0A,                   //     Opamp current small
      0x00,                   //     Boost frequency
    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A,  
    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
      ST7735_ROTATION,        //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05 },                 //     16-bit color

#if (defined(ST7735_IS_128X128) || defined(ST7735_IS_160X128))
  init_cmds2[] = {            // Init for 7735R, part 2 (1.44" display)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F },           //     XEND = 127
#endif // ST7735_IS_128X128

#ifdef ST7735_IS_160X80
  init_cmds2[] = {            // Init for 7735S, part 2 (160x80 display)
    3,                        //  3 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x4F,             //     XEND = 79
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x9F ,            //     XEND = 159
    ST7735_INVON, 0 },        //  3: Invert colors
#endif

#ifdef ST7735S_IS_160X80
  init_cmds2[] = {            // Init for 7735S, part 2 (160x80 display)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x4F,             //     XEND = 79
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x9F },            //     XEND = 159
#endif

  init_cmds3[] = {            // Init for 7735R, part 3 (red or green tab)
    4,                        //  4 commands in list:
    ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
      100 };                  //     100 ms delay

static void ST7735_SendByte(uint8_t data) {
    while ((LCD_SPI_HAND->SR & SPI_SR_TXE) == RESET);
    LCD_SPI_HAND->DR = data;
    /* HAL_SPI_Transmit(&ST7735_SPI_PORT, &data, sizeof(data), HAL_MAX_DELAY); */
}

static void ST7735_WaitLastData() {
    while((LCD_SPI_HAND->SR & SPI_SR_TXE) == RESET);
    while((LCD_SPI_HAND->SR & SPI_SR_BSY) != RESET);
}

static void ST7735_Select() {
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
}

void ST7735_Unselect() {
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

static void ST7735_Reset() {
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
}

static void ST7735_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
    ST7735_SendByte(cmd);
    ST7735_WaitLastData();
    /* HAL_SPI_Transmit(&ST7735_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY); */
}

static void ST7735_WriteData(uint8_t* buff, size_t buff_size) {
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    /* HAL_SPI_Transmit(&ST7735_SPI_PORT, buff, buff_size, HAL_MAX_DELAY); */
    for (uint32_t i = 0; i < buff_size; i++) {
        ST7735_SendByte(*buff);
        buff++;
    }
    ST7735_WaitLastData();
}

static void ST7735_ExecuteCommandList(const uint8_t *addr) {
    uint8_t numCommands, numArgs;
    uint16_t ms;

    numCommands = *addr++;
    while(numCommands--) {
        uint8_t cmd = *addr++;
        ST7735_WriteCommand(cmd);

        numArgs = *addr++;
        // If high bit set, delay follows args
        ms = numArgs & DELAY;
        numArgs &= ~DELAY;
        if(numArgs) {
            ST7735_WriteData((uint8_t*)addr, numArgs);
            addr += numArgs;
        }

        if(ms) {
            ms = *addr++;
            if(ms == 255) ms = 500;
            HAL_Delay(ms);
        }
    }
}

static void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    // column address set
    ST7735_WriteCommand(ST7735_CASET);
    uint8_t data[] = { 0x00, x0 + ST7735_XSTART, 0x00, x1 + ST7735_XSTART };
    ST7735_WriteData(data, sizeof(data));

    // row address set
    ST7735_WriteCommand(ST7735_RASET);
    data[1] = y0 + ST7735_YSTART;
    data[3] = y1 + ST7735_YSTART;
    ST7735_WriteData(data, sizeof(data));

    // write to RAM
    ST7735_WriteCommand(ST7735_RAMWR);
}

void ucg_Init() {
	LCD_SPI_HAND->CR1 |= SPI_CR1_SPE;
    ST7735_Select();
    ST7735_Reset();
    ST7735_ExecuteCommandList(init_cmds1);
    ST7735_ExecuteCommandList(init_cmds2);
    ST7735_ExecuteCommandList(init_cmds3);
    ST7735_Unselect();
}

void ucg_DrawPixel(ucg_t* ucg, uint16_t x, uint16_t y) {
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
        return;

    ST7735_Select();

    ST7735_SetAddressWindow(x, y, x+1, y+1);
    uint8_t data[] = { ucg->forecolor >> 8, ucg->forecolor & 0xFF };
    ST7735_WriteData(data, sizeof(data));

    ST7735_Unselect();
}
static void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, const tFont* font, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;
    uint8_t chr;
    uint8_t n, m;
    n = m = 0;

    chr = ch - font->chars[0].code;
    /* if(!chr) return; */
    ST7735_SetAddressWindow(x, y, x+font->chars[chr].image->width - 1, y+font->chars[chr].image->height - 1);

    b = font->chars[chr].image->data[m++];
    for(i = 0; i < font->chars[chr].image->height; i++) {
        /* n = 0; */
        for(j = 0; j < font->chars[chr].image->width; j++) {
            if(b & 0x8000)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ST7735_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ST7735_WriteData(data, sizeof(data));
            }
            n++;
            b = b << 1;
            if(n == 16) {
                n = 0;
                b = font->chars[chr].image->data[m++];
            }
        }
    }
}

/* static void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef* font, uint16_t color, uint16_t bgcolor) { */
/*     uint32_t i, b, j; */
/*  */
/*     ST7735_SetAddressWindow(x, y, x+font->width-1, y+font->height-1); */
/*  */
/*     if(font->width > 16) { */
/*         for(i = 0; i < font->height * 2; i++) { */
/*             b = font->data[(ch - 32) * 70 + i]; */
/*             for(j = 0; j < 16; j++) { */
/*                 if((b << j) & 0x8000)  { */
/*                     uint8_t data[] = { color >> 8, color & 0xFF }; */
/*                     ST7735_WriteData(data, sizeof(data)); */
/*                 } else { */
/*                     uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF }; */
/*                     ST7735_WriteData(data, sizeof(data)); */
/*                 } */
/*             } */
/*             i++; */
/*             b = font->data[(ch - 32) * 70 + i]; */
/*             for(j = 0; j < 10; j++) { */
/*                 if((b << j) & 0x8000)  { */
/*                     uint8_t data[] = { color >> 8, color & 0xFF }; */
/*                     ST7735_WriteData(data, sizeof(data)); */
/*                 } else { */
/*                     uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF }; */
/*                     ST7735_WriteData(data, sizeof(data)); */
/*                 } */
/*             } */
/*         } */
/*     } */
/*         else { */
/*             for(i = 0; i < font->height; i++) { */
/*                 b = font->data[(ch - 32) * font->height + i]; */
/*                 for(j = 0; j < font->width; j++) { */
/*                     if((b << j) & 0x8000)  { */
/*                         uint8_t data[] = { color >> 8, color & 0xFF }; */
/*                         ST7735_WriteData(data, sizeof(data)); */
/*                     } else { */
/*                         uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF }; */
/*                         ST7735_WriteData(data, sizeof(data)); */
/*                     } */
/*                 } */
/*             } */
/*         } */
/* } */

/*
Simpler (and probably slower) implementation:

static void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color) {
    uint32_t i, b, j;

    for(i = 0; i < font.height; i++) {
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++) {
            if((b << j) & 0x8000)  {
                ST7735_DrawPixel(x + j, y + i, color);
            } 
        }
    }
}
*/

void ucg_WriteString(ucg_t* ucg, uint16_t x, uint16_t y, const char* str) {
    ST7735_Select();
    uint8_t chr;

    while(*str) {
        if(*str == '\n' || *str == '\r'){
            y = y + ucg->font->chars[0].image->height + 2;
            if((y + ucg->font->chars[0].image->height + 2) >= ST7735_HEIGHT)
                y = 0;
            x = 0;
            str++;
        }
        chr = *str - ucg->font->chars[0].code;
        if(x + ucg->font->chars[chr].image->width >= ST7735_WIDTH) {
            x = 0;
            y += ucg->font->chars[chr].image->height;
            if(y + ucg->font->chars[chr].image->height >= ST7735_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        ST7735_WriteChar(x, y, *str, ucg->font, ucg->forecolor, ucg->backcolor);
        x += ucg->font->chars[chr].image->width;
        str++;
    }

    ST7735_Unselect();
}

/* void ucg_WriteString(ucg_t* ucg, uint16_t x, uint16_t y, const char* str) { */
/*     ST7735_Select(); */
/*  */
/*     while(*str) { */
/*         if(x + ucg->font->width >= ST7735_WIDTH) { */
/*             x = 0; */
/*             y += ucg->font->height; */
/*             if(y + ucg->font->height >= ST7735_HEIGHT) { */
/*                 break; */
/*             } */
/*  */
/*             if(*str == ' ') { */
/*                 // skip spaces in the beginning of the new line */
/*                 str++; */
/*                 continue; */
/*             } */
/*         } */
/*  */
/*         ST7735_WriteChar(x, y, *str, ucg->font, ucg->forecolor, ucg->backcolor); */
/*         x += ucg->font->width; */
/*         str++; */
/*     } */
/*  */
/*     ST7735_Unselect(); */
/* } */

void ucg_FillCircle(ucg_t* ucg, int16_t x0, int16_t y0, int16_t r) {
   int16_t  x,y,xd;

   if ( x0<0 ) return;
   if ( y0<0 ) return;
   if ( r<=0 ) return;

   xd = 3 - (r << 1);
   x = 0;
   y = r;

   while ( x <= y )
   {
     if( y > 0 )
     {
        ucg_DrawLine(ucg, x0 - x, y0 - y,x0 - x, y0 + y);
        ucg_DrawLine(ucg, x0 + x, y0 - y,x0 + x, y0 + y);
     }
     if( x > 0 )
     {
        ucg_DrawLine(ucg, x0 - y, y0 - x,x0 - y, y0 + x);
        ucg_DrawLine(ucg, x0 + y, y0 - x,x0 + y, y0 + x);
     }
     if ( xd < 0 )
     {
        xd += (x << 2) + 6;
     }
     else
     {
        xd += ((x - y) << 2) + 10;
        y--;
     }
     x++;
   }
   ucg_DrawCircle(ucg, x0, y0, r);
}

void ucg_FillFrame(ucg_t* ucg, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
   int16_t n,m;

   if ( x2 < x1 )
   {
      n = x2;
      x2 = x1;
      x1 = n;
   }
   if ( y2 < y1 )
   {
      n = y2;
      y2 = y1;
      y1 = n;
   }

   for( m=y1; m<=y2; m++ )
   {
      for( n=x1; n<=x2; n++ )
      {
         ucg_DrawPixel(ucg, n, m);
      }
   }
}

void ucg_FillRoundFrame(ucg_t* ucg, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t r) {
   int16_t  x,y,xd;

   if ( x2 < x1 )
   {
      x = x2;
      x2 = x1;
      x1 = x;
   }
   if ( y2 < y1 )
   {
      y = y2;
      y2 = y1;
      y1 = y;
   }

   if ( r<=0 ) return;

   xd = 3 - (r << 1);
   x = 0;
   y = r;

   ucg_FillFrame(ucg, x1 + r, y1, x2 - r, y2);

   while ( x <= y )
   {
     if( y > 0 )
     {
        ucg_DrawLine(ucg, x2 + x - r, y1 - y + r, x2+ x - r, y + y2 - r);
        ucg_DrawLine(ucg, x1 - x + r, y1 - y + r, x1- x + r, y + y2 - r);
     }
     if( x > 0 )
     {
        ucg_DrawLine(ucg, x1 - y + r, y1 - x + r, x1 - y + r, x + y2 - r);
        ucg_DrawLine(ucg, x2 + y - r, y1 - x + r, x2 + y - r, x + y2 - r);
     }
     if ( xd < 0 )
     {
        xd += (x << 2) + 6;
     }
     else
     {
        xd += ((x - y) << 2) + 10;
        y--;
     }
     x++;
   }
}

void ucg_FillRectangle(ucg_t* ucg, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    // clipping
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if((x + w - 1) >= ST7735_WIDTH) w = ST7735_WIDTH - x;
    if((y + h - 1) >= ST7735_HEIGHT) h = ST7735_HEIGHT - y;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x+w-1, y+h-1);

    uint8_t data[] = { ucg->forecolor >> 8, ucg->forecolor & 0xFF };
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            /* HAL_SPI_Transmit(&ST7735_SPI_PORT, data, sizeof(data), HAL_MAX_DELAY); */
            ST7735_SendByte(data[0]);
            ST7735_SendByte(data[1]);
        }
    }

    ST7735_Unselect();
}

void ucg_FillScreen(ucg_t* ucg) {
    ucg_FillRectangle(ucg, 0, 0, ST7735_WIDTH, ST7735_HEIGHT);
}

void ucg_DrawRectangle(ucg_t* ucg, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if((x + w - 1) >= ST7735_WIDTH) w = ST7735_WIDTH - x;
    if((y + h - 1) >= ST7735_HEIGHT) h = ST7735_HEIGHT - y;
    for(int i = x; i < (x + w); i++) {
        ucg_DrawPixel(ucg, i, y);
        ucg_DrawPixel(ucg, i, (y + h - 1));
    }

    for (int j = y; j < (y + h); j++) {
      ucg_DrawPixel(ucg, x, j);
      ucg_DrawPixel(ucg, (x + w - 1), j);
    }
}

/* void ucg_SetColor(ucg_t* ucg, uint8_t idx, uint16_t color) { */
/*     ucg->color[idx] = color; */
/* } */

inline void ucg_SetBackColor(ucg_t* ucg, uint16_t color) {
    ucg->backcolor = color;
}

inline void ucg_SetForeColor(ucg_t* ucg, uint16_t color) {
    ucg->forecolor = color;
}

uint16_t ucg_GetBackColor(ucg_t* ucg) {
    uint16_t color;
    color = ucg->backcolor;
    return color;
}

uint16_t ucg_GetForeColor(ucg_t* ucg) {
    uint16_t color;
    color = ucg->forecolor;
    return color;
}

inline void ucg_SetFont(ucg_t* ucg, const tFont* font) {
    ucg->font = font;
}

const tFont* ucg_GetFont(ucg_t *ucg) {
    const tFont *font;
    font = ucg->font;
    return font;
}

void ucg_DrawLine(ucg_t* ucg, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    uint16_t n, dx, dy, sgndx, sgndy, dxabs, dyabs, x, y, drawx, drawy;


    dx = x2 - x1;
    dy = y2 - y1;
    dxabs = (dx>0)?dx:-dx;
    dyabs = (dy>0)?dy:-dy;
    sgndx = (dx>0)?1:-1;
    sgndy = (dy>0)?1:-1;
    x = dyabs >> 1;
    y = dxabs >> 1;
    drawx = x1;
    drawy = y1;

    ucg_DrawPixel(ucg, drawx, drawy);

    if( dxabs >= dyabs )
    {
        for( n=0; n<dxabs; n++ )
        {
            y += dyabs;
            if( y >= dxabs )
            {
                y -= dxabs;
                drawy += sgndy;
            }
            drawx += sgndx;
            ucg_DrawPixel(ucg, drawx, drawy);
        }
    }
    else
    {
        for( n=0; n<dyabs; n++ )
        {
            x += dxabs;
            if( x >= dyabs )
            {
                x -= dyabs;
                drawx += sgndx;
            }
            drawy += sgndy;
            ucg_DrawPixel(ucg, drawx, drawy);
        }
    }  
}

/*    Arc 90 degress */
/*   Paramer s 0x03 0x0C 0x30 0xC0 */
/*   place arc */
void ucg_DrawArc(ucg_t* ucg, int16_t x0, int16_t y0, int16_t r, uint8_t s) {
   int16_t x,y,xd,yd,e;

   if ( x0<0 ) return;
   if ( y0<0 ) return;
   if ( r<=0 ) return;

   xd = 1 - (r << 1);
   yd = 0;
   e = 0;
   x = r;
   y = 0;

   while ( x >= y )
   {
      // Q1
      if ( s & 0x01 ) ucg_DrawPixel(ucg, x0 + x, y0 - y);
      if ( s & 0x02 ) ucg_DrawPixel(ucg, x0 + y, y0 - x);

      // Q2
      if ( s & 0x04 ) ucg_DrawPixel(ucg, x0 - y, y0 - x);
      if ( s & 0x08 ) ucg_DrawPixel(ucg, x0 - x, y0 - y);

      // Q3
      if ( s & 0x10 ) ucg_DrawPixel(ucg, x0 - x, y0 + y);
      if ( s & 0x20 ) ucg_DrawPixel(ucg, x0 - y, y0 + x);

      // Q4
      if ( s & 0x40 ) ucg_DrawPixel(ucg, x0 + y, y0 + x);
      if ( s & 0x80 ) ucg_DrawPixel(ucg, x0 + x, y0 + y);

      y++;
      e += yd;
      yd += 2;
      if ( ((e << 1) + xd) > 0 )
      {
         x--;
         e += xd;
         xd += 2;
      }
   }
}
void ucg_DrawCircle(ucg_t* ucg, int16_t x0, int16_t y0, int16_t r) {
   int16_t x,y,xd,yd,e;

   if ( x0<0 ) return;
   if ( y0<0 ) return;
   if ( r<=0 ) return;

   xd = 1 - (r << 1);
   yd = 0;
   e = 0;
   x = r;
   y = 0;

   while ( x >= y )
   {
      ucg_DrawPixel(ucg, x0 - x, y0 + y);
      ucg_DrawPixel(ucg, x0 - x, y0 - y);
      ucg_DrawPixel(ucg, x0 + x, y0 + y);
      ucg_DrawPixel(ucg, x0 + x, y0 - y);
      ucg_DrawPixel(ucg, x0 - y, y0 + x);
      ucg_DrawPixel(ucg, x0 - y, y0 - x);
      ucg_DrawPixel(ucg, x0 + y, y0 + x);
      ucg_DrawPixel(ucg, x0 + y, y0 - x);

      y++;
      e += yd;
      yd += 2;
      if ( ((e << 1) + xd) > 0 )
      {
         x--;
         e += xd;
         xd += 2;
      }
   }
}

void ucg_DrawRBox(ucg_t *ucg, int16_t x, int16_t y, int16_t w, int16_t h, int16_t r) {
   int16_t x1 = x + w - 1;
   int16_t y1 = y + h - 1;

   if ( r > x1 ) return;
   if ( r > y1 ) return;

   ucg_DrawLine(ucg, x+r, y, x1-r, y);
   ucg_DrawLine(ucg, x+r, y1, x1-r, y1);
   /* ucg_DrawLine(ucg, x, y+r, x, y1-r); */
   /* ucg_DrawLine(ucg, x1, y+r, x1, y1-r); */
   ucg_DrawArc(ucg, x+r, y+r, r, 0x0C);
   ucg_DrawArc(ucg, x1-r, y+r, r, 0x03);
   ucg_DrawArc(ucg, x+r, y1-r, r, 0x30);
   ucg_DrawArc(ucg, x1-r, y1-r, r, 0xC0);
}

void ucg_DrawRoundFrame(ucg_t* ucg, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t r) {
   int16_t n;
   if ( x2 < x1 )
   {
      n = x2;
      x2 = x1;
      x1 = n;
   }
   if ( y2 < y1 )
   {
      n = y2;
      y2 = y1;
      y1 = n;
   }

   if ( r > x2 ) return;
   if ( r > y2 ) return;

   ucg_DrawLine(ucg, x1+r, y1, x2-r, y1);
   ucg_DrawLine(ucg, x1+r, y2, x2-r, y2);
   ucg_DrawLine(ucg, x1, y1+r, x1, y2-r);
   ucg_DrawLine(ucg, x2, y1+r, x2, y2-r);
   ucg_DrawArc(ucg, x1+r, y1+r, r, 0x0C);
   ucg_DrawArc(ucg, x2-r, y1+r, r, 0x03);
   ucg_DrawArc(ucg, x1+r, y2-r, r, 0x30);
   ucg_DrawArc(ucg, x2-r, y2-r, r, 0xC0);
}

void ucg_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data) {
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if((x + w - 1) >= ST7735_WIDTH) return;
    if((y + h - 1) >= ST7735_HEIGHT) return;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x+w-1, y+h-1);
    ST7735_WriteData((uint8_t*)data, sizeof(uint16_t)*w*h);
    ST7735_Unselect();
}

inline uint16_t ucg_GetYDim(ucg_t *ucg) {
    return ucg->y_dim;
}

inline uint16_t ucg_GetXDim(ucg_t *ucg) {
    return ucg->x_dim;
}

//Return width in pixel include distance beetween chars
uint8_t ucg_GetStrWidth(ucg_t *ucg, const tFont* font, const char *str) {
    uint8_t w = 0;
    uint8_t chr;
    if(font->chars[0].code != ' ') {
        chr = *str - '0';
        /* return 0; */
    }
    else chr = *str - font->chars[0].code;
    while (*str) {
        w += font->chars[chr].image->width;
        /* w += 1; */
        str++;
    }
    /* w -= 1; */
    return w;
}

void ucg_InvertColors(bool invert) {
    ST7735_Select();
    ST7735_WriteCommand(invert ? ST7735_INVON : ST7735_INVOFF);
    ST7735_Unselect();
}

void ucg_DrawBmp(uint16_t x, uint16_t y, const tImage* img, uint16_t color, uint16_t bgcolor) {
    uint16_t b;
    uint8_t n;
    uint16_t m;
    n = 0;
    m = 0;
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if((x + img->width - 1) >= ST7735_WIDTH) return;
    if((y + img->height - 1) >= ST7735_HEIGHT) return;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x+img->width - 1, y+img->height - 1);
    b = img->data[m++];
    for (uint8_t i = 0; i < img->height; i++) {
        for (uint8_t j = 0; j < img->width; j++) {
            if(b & 0x8000)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ST7735_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ST7735_WriteData(data, sizeof(data));
            }
            n++;
            b = b << 1;
            if(n == 16) {
                n = 0;
                b = img->data[m++];
            }
        }
    }
    ST7735_Unselect();
}

void Reset_onError(void){
  __disable_irq();
  while(BUTTON_input()){                    // Wait until the button is pressed
    for(uint16_t i=0;i<50000;i++);          // Small delay
    /* HAL_IWDG_Refresh(&hiwdg);               // Clear watchdog */
  }
  while(!BUTTON_input()){                   // Wait until the button is released
    for(uint16_t i=0;i<50000;i++);          // Small delay
    /* HAL_IWDG_Refresh(&hiwdg);               // Clear watchdog */
  }
  NVIC_SystemReset();                       // Reset system
}

void FatalError(uint8_t type){
    /* uint8_t lang =systemSettings.settings.language; */
    /* if(lang>(LANGUAGE_COUNT-1)){ */
    /*     lang=lang_english; */
    /* } */

    setSafeMode(enable);
    /* buzzer_fatal_beep(); */
    Oled_error_init();
    switch(type){
        case error_FLASH:
            ucg_WriteString(&ucg, 1, 1, "FLASH ERROR");
            break;
        case error_NMI:
            ucg_WriteString(&ucg, 1, 1, "NMI HANDLER");
            break;
            break;
        case error_HARDFAULT:
            ucg_WriteString(&ucg, 1, 1, "HARD FAULT");
            break;
            break;
        case error_MEMMANAGE:
            ucg_WriteString(&ucg, 1, 1, "MEM MANAGE");
            break;
            break;
        case error_BUSFAULT:
            ucg_WriteString(&ucg, 1, 1, "BUS FAULT");
            break;
            break;
        case error_USAGEFAULT:
            ucg_WriteString(&ucg, 1, 1, "USAGE FAULT");
            break;
            break;
        case error_RUNAWAY25:
        case error_RUNAWAY50:
        case error_RUNAWAY75:
        case error_RUNAWAY100:
            {
                uint8_t level = 25 * ((type - error_RUNAWAY25)+1);
                char strRunawayLevel[8];
                sprintf(strRunawayLevel,">%u\n",level);
                ucg_WriteString(&ucg, 1, 1, strings[0].ERROR_RUNAWAY);
                ucg_WriteString(&ucg, 1, 15, strRunawayLevel);
                break;
            }
        case error_RUNAWAY500:
            ucg_WriteString(&ucg, 1, 1, strings[0].ERROR_EXCEEDED);
            ucg_WriteString(&ucg, 1, 15, "500 C!");
            break;

        default:
            ucg_WriteString(&ucg, 1, 1, strings[0].ERROR_UNKNOWN);
            break;
    }
    ucg_WriteString(&ucg, 1, 1, strings[0].ERROR_SYSTEM_HALTED);
    ucg_WriteString(&ucg, 1, 1, strings[0].ERROR_BTN_RESET);

    Reset_onError();
}
