
/*******************************************************************************
* image
* filename: /home/vovan/Документи/logo.xml
* name: logo
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



static const uint16_t image_data_logo[800] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x002a, 0xaaa0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x007f, 0xfff0, 0x0000, 0x0000, 0x0000, 0x4fc0, 0x0000, 0x0000, 0x0000, 0x0000, 0x07ff, 0xffff, 0x0000, 0x0000, 0x0003, 0xcffc, 0x0000, 0x0000, 0x0000, 0x0000, 0x007f, 0xfff0, 0x0000, 0x0000, 0x000f, 0xcfff, 0x0000, 0x0000, 0x0000, 0x0000, 0x03c1, 0x049c, 0x0000, 0x0000, 0x003f, 0xcfff, 0x8000, 0x0000, 0x0000, 0x0000, 0x005f, 0xdc90, 0x0000, 0x0000, 0x0073, 0xcfff, 0xe000, 0x0000, 0x0000, 0x0000, 0x1fc1, 0xdd5f, 0x0000, 0x0000, 0x00e1, 0xcfff, 0xf000, 0x0000, 0x0000, 0x0000, 0x007d, 0xddd0, 0x0000, 0x0000, 0x01c0, 0x001f, 0xf800, 0x0000, 0x0000, 0x0000, 0x01c1, 0xddd8, 0x0000, 0x0000, 0x03e0, 0x000f, 0xf800, 0x0000, 0x0000, 0x0000, 0x007f, 0xfff0, 0x0000, 0x0000, 0x03f3, 0xffc7, 0xfc0f, 0xcccf, 0xc000, 0x0000, 0x7fff, 0xfffc, 0x0000, 0x0000, 0x07ff, 0xffe0, 0x000f, 0xcccf, 0xc000, 0x0000, 0x007e, 0x33f0, 0x0000, 0x0000, 0x0fff, 0xc030, 0x0003, 0x0ccc, 0x0000, 0x0000, 0x07ff, 0xddff, 0x0000, 0x0000, 0x0fff, 0x000f, 0xff03, 0x0fcf, 0x8000, 0x0000, 0x007f, 0x3bf0, 0x0000, 0x0000, 0x0ffe, 0x0007, 0x9f83, 0x0fcf, 0x8000, 0x0000, 0x01ff, 0xd7ff, 0x0000, 0x0000, 0x1ffc, 0x0003, 0x0f83, 0x0ccc, 0x0000, 0x0000, 0x027e, 0x31f0, 0x0000, 0x0000, 0x1ff8, 0x0002, 0x0783, 0x0ccf, 0xc000, 0x0000, 0x0fff, 0xfffe, 0x0000, 0x0000, 0x0078, 0x0601, 0x0f83, 0x0ccf, 0xc000, 0x0000, 0x007f, 0xfff0, 0x0000, 0x0000, 0x0070, 0x1f81, 0x9fc0, 0x0000, 0x0000, 0x0000, 0x002a, 0xaaa0, 0x0000, 0x0000, 0x3e70, 0x1f80, 0x9fc0, 0x0000, 0x0000, 0x0000, 0x0022, 0x2a80, 0x0000, 0x0000, 0x3e70, 0x3fc0, 0x9fc0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0a80, 0x0000, 0x0000, 0x3e70, 0x3fc0, 0x9fcf, 0xcfcc, 0x0fcf, 0xcfcf, 0xdccf, 0xc200, 0x0000, 0x0000, 0x3e70, 0x1f81, 0x9fcf, 0xcfcc, 0x0e4f, 0xcfcf, 0xcecf, 0xc200, 0x0000, 0x0000, 0x3e70, 0x1f80, 0x98cc, 0x0ccc, 0x0e4c, 0x0c43, 0x0fcc, 0x0000, 0x0000, 0x0000, 0x3e78, 0x0601, 0x900f, 0xcccc, 0x0e4f, 0x8fc3, 0x0fcd, 0xc000, 0x0000, 0x0000, 0x3e78, 0x0001, 0x900f, 0xcccc, 0x0e4f, 0x8d03, 0x0dcd, 0xc000, 0x0000, 0x0000, 0x3e7c, 0x0603, 0x98c0, 0xcccc, 0x0e4c, 0x0d83, 0x0ccc, 0xc000, 0x0000, 0x0000, 0x3c3c, 0x0603, 0x9fcf, 0xcfcf, 0xce4f, 0xcccf, 0xcccf, 0xc000, 0x0000, 0x0000, 0x181e, 0x0600, 0x1f8f, 0xcfcf, 0xcfcf, 0xcccf, 0xcccf, 0xc000, 0x0000, 0x0000, 0x181f, 0x0600, 0x1f80, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0c3f, 0x061f, 0xff00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0e7f, 0x061f, 0xff00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07fe, 0x060f, 0xfe0f, 0xcfcf, 0xcfcf, 0xcfcc, 0xc000, 0x0000, 0x0000, 0x0000, 0x03fc, 0x0607, 0xfc0f, 0xcfcf, 0xcfcf, 0xcfce, 0xc000, 0x0000, 0x0000, 0x0000, 0x01fc, 0x0603, 0xf80c, 0x030c, 0xc303, 0x0ccf, 0xc000, 0x0000, 0x0000, 0x0000, 0x00f8, 0x0603, 0xf00f, 0xc30f, 0xc303, 0x0ccd, 0xc000, 0x0000, 0x0000, 0x0000, 0x0078, 0x0601, 0xe00f, 0xc30f, 0xc303, 0x0ccc, 0xc000, 0x0000, 0x0000, 0x0000, 0x0030, 0x0600, 0xc000, 0xc30c, 0xc303, 0x0ccc, 0xc000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x000f, 0xc30c, 0xc30f, 0xcfcc, 0xc000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x000f, 0xc30c, 0xc30f, 0xcfcc, 0xc000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x0000, 0x0000, 0x0000, 0x0010, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x0000, 0x0000, 0x0000, 0x0010, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x0000, 0x0000, 0x0000, 0x002c, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x0000, 0x0000, 0x0000, 0x0028, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x0000, 0x0000, 0x0000, 0x0014, 0x8000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x0000, 0x0000, 0x0000, 0x0028, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x0000, 0x0000, 0x0000, 0x0014, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x0000, 0x0000, 0x0000, 0x0008, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x0000, 0x0000, 0x0000, 0x0010, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0600, 0x0000, 0x0000, 0x0000, 0x0008, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0700, 0x0000, 0x0000, 0x0008, 0x0008, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0380, 0x03ff, 0xffff, 0xffb8, 0x0010, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01c0, 0xf7ff, 0xffff, 0xffb8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00fe, 0xf7ff, 0xffff, 0xffbb, 0xfff0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x007e, 0xf7ff, 0xffff, 0xffbb, 0xfff0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xf7ff, 0xffff, 0xffb8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x03ff, 0xffff, 0xffb8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0008, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
const tImage logo = { image_data_logo, 160, 80};