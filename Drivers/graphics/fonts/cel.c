
/*******************************************************************************
* image
* filename: /home/vovan/Документи/cel.xml
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
static const uint16_t image_data_cel[75] = {
    0x0000, 0x0000, 0x0000, 0x0007, 0xf000, 0x0038, 0xe000, 0x00e3, 0x8000, 0x038e, 0x0000, 0x0e38, 0x0000, 0x38e0, 0x0000, 0xe380, 0x0003, 0x8e00, 0x000e, 0x3800, 0x001f, 0xc000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7fc0, 0x0007, 0xffe0, 0x007f, 0xffc0, 0x03ff, 0xff80, 0x1fc0, 0xff00, 0x7e00, 0xfc03, 0xf001, 0xf80f, 0x8003, 0x007e, 0x0000, 0x01f0, 0x0000, 0x07c0, 0x0000, 0x1f00, 0x0000, 0x7c00, 0x0001, 0xf000, 0x0007, 0xc000, 0x001f, 0x0000, 0x007e, 0x0008, 0x00f8, 0x007c, 0x03f0, 0x01f8, 0x07e0, 0x0fc0, 0x1fc0, 0xfe00, 0x3fff, 0xf800, 0x7fff, 0xc000, 0x7ffc, 0x0000, 0x7fc0, 0x0000, 0x0000
};

const tImage cel = { image_data_cel, 30, 40};
