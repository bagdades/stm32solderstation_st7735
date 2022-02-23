
/*******************************************************************************
* image
* filename: /home/vovan/Документи/far.xml
* name: cel
* encoding: UTF-8
* unicode bom: yes
*
* preset name: monoch1
* data block size: 16 bit(s), uint16_t
* conversion type: Monochrome, Diffuse Dither 128
* bits per pixel: 1
*
* preprocess:
*  rotate: <value not defined>
*  flip horizontal: <value not defined>
*  flip vertical: <value not defined>
*  inverse: yes
*******************************************************************************/

/*
 typedef struct {
     const uint16_t *data;
     uint8_t width;
     uint8_t height;
     } tIcon;
*/
#include <stdint.h>
#include "fonts.h"



static const uint16_t image_data_far[65] = {
    0x0000, 0x0000, 0x0000, 0x07f0, 0x0003, 0x8e00, 0x00e3, 0x8000, 0x38e0, 0x000e, 0x3800, 0x038e, 0x0000, 0xe380, 0x0038, 0xe000, 0x0e38, 0x0001, 0xfc00, 0x0000, 0x0000, 0x0000, 0x0000, 0x7fff, 0xfc1f, 0xffff, 0x07ff, 0xffc1, 0xffff, 0xf07c, 0x0000, 0x1f00, 0x0007, 0xc000, 0x01f0, 0x0000, 0x7c00, 0x001f, 0x0000, 0x07c0, 0x0001, 0xfffc, 0x007f, 0xff00, 0x1fff, 0xc007, 0xfff0, 0x01ff, 0xfc00, 0x7c00, 0x001f, 0x0000, 0x07c0, 0x0001, 0xf000, 0x007c, 0x0000, 0x1f00, 0x0007, 0xc000, 0x01f0, 0x0000, 0x7c00, 0x001f, 0x0000
};
const tImage far = { image_data_far, 26, 40};
