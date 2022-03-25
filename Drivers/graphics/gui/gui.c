/*
 * gui.c
 *
 *  Created on: Jan 12, 2021
 *      Author: David    Original work by Jose Barros (PTDreamer), 2017
 */

#include "screen.h"
#include "screens.h"
#include "oled.h"
#include "gui.h"
#include "splash_screen.h"


void guiInit(void) {

  oled_addScreen(&Screen_boot, screen_boot);
  boot_screen_setup(&Screen_boot);

  oled_addScreen(&Screen_main,screen_main);
  main_screen_setup(&Screen_main);

  oled_addScreen(&Screen_settings,screen_settings);
  settings_screen_setup(&Screen_settings);

  oled_addScreen(&Screen_iron,screen_iron);
  iron_screen_setup(&Screen_iron);

  oled_addScreen(&Screen_system,screen_system);
  system_screen_setup(&Screen_system);

  oled_addScreen(&Screen_reset,screen_reset);
  reset_screen_setup(&Screen_reset);

  oled_addScreen(&Screen_tip_list,screen_tip_list);
  tip_list_screen_setup(&Screen_tip_list);

  oled_addScreen(&Screen_tip_settings, screen_tip_settings);
  tip_settings_screen_setup(&Screen_tip_settings);

  oled_addScreen(&Screen_calibration,screen_calibration);
  calibration_screen_setup(&Screen_calibration);
  
  oled_addScreen(&Screen_splash, screen_splash);
  splash_screen_setup(&Screen_splash);
#ifdef ENABLE_DEBUG_SCREEN

  oled_addScreen(&Screen_debug,screen_debug);
  debug_screen_setup(&Screen_debug);

#endif
}
