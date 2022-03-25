/*
 * =====================================================================================
 *
 *       Filename:  splash_screen.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  24.03.22 22:23:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "splash_screen.h"
#include "screen.h"
#include "st7735.h"

screen_t Screen_splash;
extern  ucg_t ucg;

static void splash_screen_init(screen_t *scr) {
	default_init(scr);
}

static void splash_screen_create(screen_t *scr) {
	/* widget_t *w; */
	/* displayOnly_widget_t *dis; */
	/* newWidget(&w, widget_display, scr); */
	/* dis = extractDisplayPartFromWidget(w); */
	/* dis->reservedChars = 9; */
	/* dis->displayString = showTime; */
	/* #<{(| dis->dispAlign = align_center; |)}># */
	/* dis->font = &font_32; */
	/* dis->type = field_splash; */
	/* w->fcolor = C_COLOR565(0x3E, 0xB2, 0xFF); */
	/* w->posX = 9; */
	/* w->posY = 0; */
	/* w->width = 159; */
	/* w->enabled = 1; */
    /*  */
	/* newWidget(&w, widget_display, scr); */
	/* dis = extractDisplayPartFromWidget(w); */
	/* dis->reservedChars = 11; */
	/* dis->displayString = showDate; */
	/* #<{(| dis->dispAlign = align_center; |)}># */
	/* dis->font = &font_32; */
	/* dis->type = field_splash; */
	/* w->fcolor = C_YELLOW; */
	/* w->posX = 9; */
	/* w->posY = 45; */
	/* w->width = 159; */
	/* w->enabled = 1; */
}

uint8_t splash_screen_draw(screen_t *scr) {
	ucg_SetFont(&ucg, &font_32);
	ucg_SetBackColor(&ucg, C_BLACK);
	ucg_SetForeColor(&ucg, C_COLOR565(0x3E, 0xB2, 0xFF));
	ucg_WriteString(&ucg, 9, 0, showTime);
	ucg_SetForeColor(&ucg, C_YELLOW);
	ucg_WriteString(&ucg, 9, 45, showDate);
	return 0;
}

int splash_screenProcessInput(screen_t * scr, RE_Rotation_t input, RE_State_t *state) {
	if(input != Rotate_Nothing || Iron.CurrentMode != mode_sleep)
		return screen_main;
	return default_screenProcessInput(scr, input, state);
}

void splash_screen_setup(screen_t *scr) {
	scr->create = &splash_screen_create;
	scr->init = &splash_screen_init;
	scr->draw = &splash_screen_draw;
	scr->processInput = &splash_screenProcessInput;
}
