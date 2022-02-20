/*
 * boot_screen.c
 *
 *  Created on: Jan 12, 2021
 *      Author: David    Original work by Jose Barros (PTDreamer), 2017
 */

#include "boot_screen.h"
#include "screen_common.h"
#include "st7735.h"
#include "fonts.h"
#define SPLASH_TIMEOUT 100


//-------------------------------------------------------------------------------------------------------------------------------
// Boot Screen variables
//-------------------------------------------------------------------------------------------------------------------------------
extern ucg_t ucg;
screen_t Screen_boot;
static widget_t *Widget_profile;
static widget_t *Widget_lang;
static widget_t *Widget_ok;
static uint8_t boot_step=0;


void boot_screen_create(screen_t *scr);

//=========================================================
static void * getProfile() {
	temp = profile;
	return &temp;
}
static void setProfile(uint32_t *val) {
	profile=*val;
}
//=========================================================
static void * getLanguage() {
	temp = systemSettings.settings.language;
	return &temp;
}
static void setLanguage(uint32_t *val) {
	lang = *val;
	systemSettings.settings.language=*val;
}
//=========================================================

static int SaveSetup(widget_t* w) {
	loadProfile(profile);
	saveSettingsFromMenu(save_Settings);
	boot_step++;
	return -1;
}
//=========================================================
void draw_boot_strings(void){
	uint8_t length;
	uint8_t posx;
	length = ucg_GetStrWidth(&ucg, ucg_GetFont(&ucg), strings[lang].boot_firstBoot);
	posx = (ucg_GetXDim(&ucg) - length) / 2;
	ucg_SetFont(&ucg, default_font);
	ucg_SetForeColor(&ucg, C_CYAN);
	ucg_WriteString(&ucg, posx, 0, strings[lang].boot_firstBoot);
	/* ucg_DrawLine(&ucg, 0, 13, ucg_GetXDim(&ucg), 13); */
	ucg_WriteString(&ucg, 0, 20, strings[lang].boot_Profile);
	ucg_WriteString(&ucg, 0, 40, strings[lang]._Language);
}

static uint8_t boot_screen_draw(screen_t *scr){
	uint8_t refresh = scr->refresh;                 // Save refresh state. If screen set to be erased, default_screenDraw will erase it.
	uint8_t ret = default_screenDraw(scr);          // So we need to run screenDraw first and save the return value

	if(refresh!=screen_Idle && boot_step==1){       // If screen not idle (erased) and boot_step=1 (Setup screen)
		draw_boot_strings();                          // Redraw strings
	}

	return (ret);                                   // return the ret value from screenDraw
}


int boot_screen_processInput(screen_t * scr, RE_Rotation_t input, RE_State_t *state) {
	if(current_lang!=lang){                                                       // If language changed
		current_lang=lang;
		oled_destroy_screen(scr);                                                   // Destroy and create the screen
		boot_screen_create(scr);
		scr->current_widget = Widget_lang;
		scr->refresh = screen_Erase;
		((editable_widget_t*)Widget_lang->content)->selectable.state=widget_edit;
		widgetEnable(Widget_lang);
		widgetEnable(Widget_profile);
		widgetEnable(Widget_ok);
	}

	/* if(input!=Rotate_Nothing){ */
	/* 	wakeOledDim(); */
	/* } */
	/* handleOledDim(); */

	switch(boot_step){

		case 2:
			systemSettings.setupMode=disable;                                                               // Save button clicked, disable setup mode
			setSafeMode(disable);                                                                           // Disable safe mode and exit

		case 0:
			if(checkScreenTimer(SPLASH_TIMEOUT)){                                                           // After splash timeout
				if(!systemSettings.setupMode){                                                                // If not in setup mode
					ADC_Reset_measures();                                                                       // Reset the averages, show current values to avoid filtering delay at startup
					resetIronError();                                                                           // Force timeout of any error (This won't clear errors if still detected)
					return screen_main;                                                                         // Go to main screen
				}
				widgetEnable(Widget_lang);                                                                    // In setup mode, enable widgets
				widgetEnable(Widget_profile);
				widgetEnable(Widget_ok);
				scr->refresh = screen_Erase;                                                                  // Force screen wipe to clear the boot logo
				boot_step++;                                                                                  // Increase boot step
			}
			else{
				return -1;                                                                                    // Boot timeout not expired, do nothing
			}

		default:
			break;
	}

	return (default_screenProcessInput(scr, input, state));
}


void boot_screen_init(screen_t * scr){
	default_init(scr);
	profile=systemSettings.settings.currentProfile;
	if( (systemSettings.settings.state!=initialized) || (profile>profile_C210) ){
		profile=profile_T12;
		setSafeMode(enable);
		systemSettings.setupMode=enable;
	}
	/* ucg_SetForeColor(&ucg, C_CYAN); */
	/* ucg_DrawBmp(0, 0, &logo, C_WHITE, C_BLACK); */
	scr->refresh = screen_Erased;
	wakeOledDim();
}


void boot_screen_create(screen_t *scr){
	widget_t *w;
	displayOnly_widget_t *dis;
	editable_widget_t *edit;
	lang = systemSettings.settings.language;
	if(lang>=LANGUAGE_COUNT){
		lang=lang_english;
	}
	current_lang = lang;

	//  [ Profile Select Widget ]
	//
	newWidget(&w, widget_multi_option, scr);
	Widget_profile = w;
	dis=extractDisplayPartFromWidget(w);
	edit=extractEditablePartFromWidget(w);
	dis->font = default_font;
	dis->reservedChars=4;
	dis->getData = &getProfile;
	dis->textAlign = align_right;
	edit->big_step = 1;
	edit->step = 1;
	edit->selectable.tab = 0;
	edit->setData = (void (*)(void *))&setProfile;
	edit->options = profileStr;
	edit->numberOfOptions = ProfileSize;
	w->posY = 20;
	w->width = 48;
	w->posX = ucg_GetXDim(&ucg) - w->width - 1;
	w->enabled=0;

	//  [ Language Select Widget ]
	//
	newWidget(&w, widget_multi_option, scr);
	Widget_lang = w;
	dis=extractDisplayPartFromWidget(w);
	edit=extractEditablePartFromWidget(w);
	dis->font = default_font;
	dis->reservedChars=2;
	dis->getData = &getLanguage;
	dis->textAlign = align_right;
	edit->big_step = 1;
	edit->step = 1;
	edit->selectable.tab = 1;
	edit->setData = (void (*)(void *))&setLanguage;
	edit->options = Langs;
	edit->numberOfOptions = LANGUAGE_COUNT;
	w->posY = 40;
	w->width = 48;
	w->posX = ucg_GetXDim(&ucg) - w->width - 1;
	w->enabled=0;

	//  [ OK button Widget ]
	//
	newWidget(&w, widget_button, scr);
	Widget_ok = w;
	button_widget_t* button=Widget_ok->content;
	button->displayString = strings[lang]._SAVE;
	button->font = default_font;
	button->selectable.tab = 2;
	button->action = &SaveSetup;
	w->posY = 60;
	w->width = 50;
	w->posX = (ucg_GetXDim(&ucg)-1) - w->width;
	w->enabled=0;
}


void boot_screen_setup(screen_t *scr) {
	scr->draw = &boot_screen_draw;
	scr->processInput = &boot_screen_processInput;
	scr->init = &boot_screen_init;
	scr->create = &boot_screen_create;
}
