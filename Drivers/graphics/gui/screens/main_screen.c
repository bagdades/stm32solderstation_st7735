/*
 * main_screen.c
 *
 *  Created on: Jan 12, 2021
 *      Author: David    Original work by Jose Barros (PTDreamer), 2017
 */

#include "main_screen.h"
#include "screen_common.h"
#include "st7735.h"
#include "widgets.h"

#define SCREENSAVER
#define PWR_BAR_WIDTH   160
#define SCALE_FACTOR    (int)((65536*PWR_BAR_WIDTH*1.005)/100)

//-------------------------------------------------------------------------------------------------------------------------------
// Main screen variables
//-------------------------------------------------------------------------------------------------------------------------------

extern ucg_t ucg;

static uint32_t barTime;
slide_t screenSaver = {
	.x = 34,
	.y = 0,
	.xAdd = 1,
	.yAdd = 1,
};

static char *tipNames[TipSize];
enum mode{  main_none=0, main_irontemp, main_error, main_setpoint, main_tipselect };
enum{ status_ok=0x20, status_error };
enum { temp_numeric, temp_graph };
//-------------------------------------------------------------------------------------------------------------------------------
// Main screen widgets
//-------------------------------------------------------------------------------------------------------------------------------
screen_t Screen_main;

#ifdef USE_NTC
static widget_t *Widget_AmbTemp;
#endif

#ifdef USE_VIN
static widget_t *Widget_Voltage;
#endif

static widget_t *Widget_IronTemp;
static widget_t *Widget_SetPoint;
static widget_t *Widget_SetTip;
static widget_t *Widget_CLabel;

static struct{
	uint8_t lastPwr;                        // Last stored power for widget
	uint8_t shakeActive;                    // Shake icon status: 0=disabled, 1=needs drawing, 2=drawign done, 3=needs clearing
	uint8_t ironStatus;                     // iron status: status_ok, status_error
	uint8_t lastError;                      // Last error, stored to detect error changes
	uint8_t setMode;                        // Main screen variable, set to switch between modes (iron_temp, setpoint, tip_select)
	uint8_t currentMode;                    // Current screen mode (iron_temp, setpoint, tip_select)
	uint8_t displayMode;                    // Iron temp display mode (temp_numeric, temp_graph)
	uint8_t updateReadings;                 // Flag to update power, tip, ambient, voltage widgets
	uint8_t boost_enable;                   // Flag used only for boost mode while in plot graph display
	int16_t lastTip;                        // Last stored tip temperature for widget
#ifdef USE_NTC
	int16_t lastAmb;                        // Last stored ambient temperature for widget
#endif
#ifdef USE_VIN
	uint16_t lastVin;                       // Last stored voltage for widget
#endif
	uint32_t modeTimer;                     // Timer to track current screen mode time
	uint32_t inputBlockTimer;               // Timer to block user input Load current time+blocking time in ms
}mainScr;

uint16_t m_tip = 0;
//-------------------------------------------------------------------------------------------------------------------------------
// Main screen widgets functions
//-------------------------------------------------------------------------------------------------------------------------------

static void *getTip() {
	m_tip = systemSettings.Profile.currentTip;
	return &m_tip;
}

void resetModeTimer(void){
	mainScr.modeTimer=current_time;
}
uint8_t checkModeTimer(uint32_t time){
	if((current_time-mainScr.modeTimer)>time){
		return 1;
	}
	return 0;
}

static void setTemp(uint16_t *val) {
	setUserTemperature(*val);
}

static void * getTemp() {
	temp = getUserTemperature();
	return &temp;
}

static void * main_screen_getIronTemp() {
	if(mainScr.updateReadings){
		mainScr.lastTip=readTipTemperatureCompensated(old_reading, read_average, systemSettings.settings.tempUnit);
		if(getCurrentMode()>mode_sleep){
			if(Iron.temperatureReached && abs(mainScr.lastTip-Iron.CurrentSetTemperature)<systemSettings.settings.guiTempDenoise){                       // Lock numeric display if within limits
				mainScr.lastTip = Iron.CurrentSetTemperature;
			}
		}
	}
	temp=mainScr.lastTip;
	return &temp;
}

#ifdef USE_VIN
static void * main_screen_getVin() {
	if(mainScr.updateReadings){
		mainScr.lastVin = getSupplyVoltage_v_x10();
	}
	temp=mainScr.lastVin;
	return &temp;
}
#endif

#ifdef USE_NTC
static void * main_screen_getAmbTemp() {
	if(mainScr.updateReadings){
		if(systemSettings.settings.tempUnit==mode_Celsius){
			mainScr.lastAmb = last_NTC_C;
		}
		else{
			mainScr.lastAmb = last_NTC_F;
		}
	}
	temp=mainScr.lastAmb;
	return &temp;
}
#endif

static void updateIronPower() {

	static uint32_t stored=0;
	static uint32_t updateTim;
	if((current_time-updateTim)>19){
		updateTim = current_time;
		int32_t tmpPwr = getCurrentPower();
		if(tmpPwr < 0){
			tmpPwr = 0 ;
		}
		tmpPwr = tmpPwr<<12;
		stored = ( ((stored<<3)-stored)+tmpPwr+(1<<11))>>3 ;
		tmpPwr = stored>>12;
		tmpPwr = (tmpPwr*SCALE_FACTOR)>>16;
		mainScr.lastPwr=tmpPwr;
	}
}

static void setMainWidget(widget_t* w){
	Screen_main.refresh=screen_Erase;
	Screen_main.current_widget=w;
	widgetEnable(w);
}

//-------------------------------------------------------------------------------------------------------------------------------
// Main screen functions
//-------------------------------------------------------------------------------------------------------------------------------
static void setMainScrTempUnit(void) {
	if(systemSettings.settings.tempUnit==mode_Farenheit){
		/* ((displayOnly_widget_t*)Widget_IronTemp->content)->endString="F";      // \260 = ASCII dec. 176(°) in octal representation */
#ifdef USE_NTC
		((displayOnly_widget_t*)Widget_AmbTemp->content)->endString="F";
#endif
		/* ((editable_widget_t*)Widget_SetPoint->content)->inputData.endString="F"; */
	}
	else{
		/* ((displayOnly_widget_t*)Widget_IronTemp->content)->endString="C"; */
#ifdef USE_NTC
		((displayOnly_widget_t*)Widget_AmbTemp->content)->endString="C";
#endif
		/* ((editable_widget_t*)Widget_SetPoint->content)->inputData.endString="C"; */
	}
}

// Ignore future input for specified amount of time
void blockInput(uint16_t time){
	mainScr.inputBlockTimer = current_time+time;
}

/* void updateScreenSaver(void){ */
/* #ifdef SCREENSAVER */
/* 	if(!screenSaver.enabled || Iron.CurrentMode!=mode_sleep || (Iron.Error.Flags & FLAG_ACTIVE)){ */
/* 		return; */
/* 	} */
/* 	if(current_time-screenSaver.timer>50){ */
/* 		screenSaver.timer=current_time; */
/* 		screenSaver.x+=screenSaver.xAdd; */
/* 		screenSaver.y+=screenSaver.yAdd; */
/* 		if(screenSaver.x<-(ScrSaverXBM[0]+10) || screenSaver.x>(ucg_GetXDim(&ucg)+10)){ */
/* 			screenSaver.xAdd = -screenSaver.xAdd; */
/* 		} */
/* 		if(screenSaver.y<-(ScrSaverXBM[1]+10) || screenSaver.y>(ucg_GetXDim(&ucg)+10)){ */
/* 			screenSaver.yAdd = -screenSaver.yAdd; */
/* 		} */
/* 		screenSaver.update=1; */
/* 	} */
/* #endif */
/* } */

// Switch main screen modes
int8_t switchScreenMode(void){
	if(mainScr.setMode!=main_none){
		resetScreenTimer();
		resetModeTimer();
		plot.enabled = (mainScr.displayMode==temp_graph);
		Screen_main.refresh=screen_Erase;
		mainScr.updateReadings=1;
		switch(mainScr.setMode){

			case main_irontemp:
				widgetDisable(Widget_SetPoint);
				mainScr.boost_enable=0;
				if(mainScr.ironStatus!=status_error){
					if(!plot.enabled){
						setMainWidget(Widget_IronTemp);
					}
					break;
				}
				mainScr.setMode=main_error;
				// No break intentionally
			case main_error:
				plot.enabled = 0;
				widgetDisable(Widget_IronTemp);
				break;

			case main_setpoint:
				plot.enabled = 0;
				setMainWidget(Widget_SetPoint);
				break;

			case main_tipselect:
				break;

			default:
				break;
		}
		mainScr.currentMode=mainScr.setMode;
		mainScr.setMode=main_none;
		return 1;                                 // Changed mode
	}
	return 0;                                   // No changes
}

int main_screenProcessInput(screen_t * scr, RE_Rotation_t input, RE_State_t *state) {
	uint8_t current_mode = getCurrentMode();
	mainScr.updateReadings=update_GUI_Timer();
	updateIronPower();
	/* updatePlot(); */
	/* updateScreenSaver(); */


	if(Iron.Error.Flags & FLAG_ACTIVE){
		if(mainScr.shakeActive){
			mainScr.shakeActive=3;
		}
		if(mainScr.ironStatus!=status_error || mainScr.lastError!=Iron.Error.Flags){  // If error appeared or changed
			/* wakeOledDim();                                                          		// Wake up screen */
			mainScr.ironStatus = status_error;
			mainScr.lastError=Iron.Error.Flags;
		}
	}
	else if(mainScr.ironStatus != status_ok){                                   // If error is gone
		mainScr.ironStatus = status_ok;
		/* wakeOledDim();                                                            // Wake up screen */
	}


	// Timer for ignoring user input
	if(current_time < mainScr.inputBlockTimer){
		input=Rotate_Nothing;
	}

	if(input!=Rotate_Nothing){
		resetScreenTimer();                                                     // Reset screen idle timer
		/* if(getOledPower()==disable){                                            // If oled off, block user action */
		/*   input=Rotate_Nothing; */
		/* } */
		/* wakeOledDim();                                                          // But  wake up screen */
	}

	/* if(systemSettings.settings.dim_mode!=dim_always && current_mode>mode_standby){  // If dim not enabled in all modes */
	/* 	wakeOledDim();																															// Refresh timeout if running */
	/* } */
    /*  */
	/* handleOledDim(); */

	// Handle shake wake icon drawing and timeout
	if( !mainScr.shakeActive && Iron.shakeActive){
		Iron.shakeActive=0;
		mainScr.shakeActive=1;
	}
	else if(mainScr.shakeActive==2 && (current_time-Iron.lastShakeTime)>150){
		mainScr.shakeActive=3; // Clear
	}


	// Handle main screen
	switch(mainScr.currentMode){


		case main_irontemp:

			if(mainScr.ironStatus!=status_ok){                // When the screen goes to error state
				mainScr.setMode=main_error;                     // Set error screen
				break;
			}
			switch((uint8_t)input){

				case LongClick:
					return screen_settings;

				case Rotate_Increment_while_click:
					mainScr.setMode=main_tipselect;
					break;

				case Rotate_Decrement_while_click:
					if(Iron.CurrentMode>mode_standby){
						setCurrentMode(mode_standby);
					}
					else{
						setCurrentMode(mode_sleep);
					}
					break;

				case Rotate_Increment:
				case Rotate_Decrement:
					if(Iron.CurrentMode==mode_boost){
						setCurrentMode(mode_run);
						break;
					}
					else if(current_mode!=mode_run){
						IronWake(wakeButton);
						if(getCurrentMode()==mode_run){                                 // If mode changed, don't process the click
							break;
						}
					}
					if(mainScr.displayMode==temp_graph){
						if(!checkModeTimer(1000)){                                      // If last step happened less than 1 second ago, disable boost flag and modify the setpoint.
							mainScr.boost_enable=0;                                       // Disable boost flag
							widgetEnable(Widget_SetPoint);                                // Enable the setpoint widget, but don't set it as current widget (Dirty hack)
							default_widgetProcessInput(Widget_SetPoint, input, state);    // Just to be able to process the input. If the widget is disabled, the widget process will skip it. It will be disabled before drawing in drawMisc function
						}
						else{                                                           // If last step was more than 1 second ago, enable boost flag
							mainScr.boost_enable=1;                                       // Set boost flag. Click within 1 second to enable boost mode
						}
						resetModeTimer();                                               // Reset mode timer
					}
					else{
						mainScr.setMode=main_setpoint;
						widgetDisable(Widget_IronTemp);
					}
					return -1;
					break;

				case Click:
					if(Iron.CurrentMode==mode_boost){
						setCurrentMode(mode_run);
						break;
					}
					if(Iron.CurrentMode!=mode_run){
						IronWake(wakeButton);
						if(getCurrentMode()==mode_run){                                 // If mode changed, don't process the click
							break;
						}
					}
					/* scr->refresh=screen_Erase; */
					/* if(mainScr.displayMode==temp_numeric){ */
					/* 	mainScr.updateReadings=1; */
					/* 	mainScr.displayMode=temp_graph; */
					/* 	widgetDisable(Widget_IronTemp); */
					/* 	plot.enabled=1; */
					/* 	plot.update=1; */
					/* } */
					/* else if(mainScr.displayMode==temp_graph){ */
					/* 	if(checkModeTimer(1000)){                                       // If more than 1 second since last rotation, disable boost flag */
					/* 		mainScr.boost_enable=0; */
					/* 	} */
					/* 	if(mainScr.boost_enable && current_mode==mode_run){             // If boost flag enabled and iron running */
					/* 		mainScr.boost_enable=0;                                       // Clear flag */
					/* 		setCurrentMode(mode_boost);                                   // Set boost mode */
					/* 	} */
					/* 	else{ */
					/* 		mainScr.updateReadings=1; */
					/* 		mainScr.displayMode=temp_numeric;                             // Else, switch to numeric display mode */
					/* 		widgetEnable(Widget_IronTemp); */
					/* 		plot.enabled=0; */
					/* 	} */
					/* } */

				default:
					break;
			}
			break;


		case main_error:

			switch((uint8_t)input){
				case LongClick:
					return screen_settings;

				case Rotate_Increment_while_click:
					mainScr.setMode=main_tipselect;
					break;

				case Rotate_Increment:
				case Rotate_Decrement:
					mainScr.setMode=main_setpoint;

				default:
					break;
			}
			if(mainScr.ironStatus==status_ok){
				mainScr.setMode=main_irontemp;
			}
			break;


		case main_tipselect:
			widgetDisable(Widget_Voltage);
			widgetEnable(Widget_SetTip);
			if(mainScr.ironStatus==status_error){  // If error appears while adjusting tip select, it needs to update now to avoid overlapping problems
				plot.enabled = 0;
				widgetDisable(Widget_IronTemp);
			}
			else{
				if(mainScr.displayMode==temp_numeric){
					widgetEnable(Widget_IronTemp);
				}
				else{
					plot.enabled=1;
				}
			}
			switch((uint8_t)input){
				case LongClick:
					Selected_Tip = systemSettings.Profile.currentTip;
					return screen_tip_settings;

				case Click:
					mainScr.setMode=main_irontemp;
					widgetEnable(Widget_Voltage);
					widgetDisable(Widget_SetTip);
					break;

				case Rotate_Nothing:
					if(checkScreenTimer(2000)){
						widgetEnable(Widget_Voltage);
						widgetDisable(Widget_SetTip);
						mainScr.setMode=main_irontemp;
					}
					break;

				default:
					{
						uint8_t tip = systemSettings.Profile.currentTip;
						if(input==Rotate_Increment_while_click || input==Rotate_Increment){
							if(++tip >= systemSettings.Profile.currentNumberOfTips){
								tip=0;
							}
						}
						else if(input==Rotate_Decrement_while_click || input==Rotate_Decrement){
							if(--tip>=systemSettings.Profile.currentNumberOfTips){    // If underflowed
								tip = systemSettings.Profile.currentNumberOfTips-1;
							}
						}
						if(tip!=systemSettings.Profile.currentTip){
							__disable_irq();
							setCurrentTip(tip);
							__enable_irq();
							/* Screen_main.refresh=screen_Erase; */
						}
						break;
					}
			}
			break;

		case main_setpoint:

			switch((uint8_t)input){
				case LongClick:
				case Click:
					if(mainScr.ironStatus != status_error && current_mode==mode_run && !checkModeTimer(1000)){
						setCurrentMode(mode_boost);
					}
					mainScr.setMode=main_irontemp;
					break;

				case Rotate_Nothing:
					if(checkScreenTimer(1000)){
						mainScr.setMode=main_irontemp;
					}
					break;

				case Rotate_Increment_while_click:
				case Rotate_Decrement_while_click:
					mainScr.setMode=main_irontemp;
					break;

				default:
					break;
			}
			if(input!=Rotate_Nothing){
				IronWake(wakeButton);
			}
		default:
			break;
	}

	if(switchScreenMode()){
		return -1;
	}

	return default_screenProcessInput(scr, input, state);
}

/* static uint8_t  drawIcons(uint8_t refresh){ */
/* 	if(refresh){ */
/* #ifdef USE_NTC */
/* 		#<{(| u8g2_DrawXBMP(&u8g2, Widget_AmbTemp->posX-tempXBM[0]-2, 0, tempXBM[0], tempXBM[1], &tempXBM[2]); |)}># */
/* #endif */
/*  */
/* #ifdef USE_VIN */
/* 		#<{(| u8g2_DrawXBMP(&u8g2, 0, 0, voltXBM[0], voltXBM[1], &voltXBM[2]); |)}># */
/* #endif */
/* 	} */
/*  */
/* 	if(mainScr.shakeActive==1 || (mainScr.shakeActive==2 && refresh) ){ //1 = needs drawing, 2 = already drawn */
/* 		mainScr.shakeActive=2; */
/* 		#<{(| u8g2_DrawXBMP(&u8g2, 49, OledHeight-shakeXBM[1], shakeXBM[0], shakeXBM[1], &shakeXBM[2]); |)}># */
/* 		return 1; */
/* 	} */
/* 	else if(mainScr.shakeActive==3){                                    // 3 = needs clearing */
/* 		mainScr.shakeActive=0; */
/* 		ucg_SetForeColor(&ucg, C_BLACK); */
/* 		ucg_FillRectangle(&ucg, 49, ucg_GetYDim(&ucg)-shakeXBM[1], shakeXBM[0], shakeXBM[1]); */
/* 		ucg_SetForeColor(&ucg, C_CYAN); */
/* 		return 1; */
/* 	} */
/* 	return 0; */
/* } */


/* static uint8_t  drawScreenSaver(uint8_t refresh){ */
/* #ifdef SCREENSAVER */
/* 	if(!refresh || !screenSaver.enabled || getCurrentMode()!=mode_sleep || mainScr.currentMode!=main_irontemp){ */
/* 		return 0; */
/* 	} */
/* 	screenSaver.update=0; */
/* 	if(screenSaver.x>(-ScrSaverXBM[0]) ||screenSaver.x<ucg_GetXDim(&ucg) || screenSaver.y>(-ScrSaverXBM[1]) || screenSaver.y<ucg_GetYDim(&ucg) ){ */
/* 		ucg_SetForeColor(&ucg, C_CYAN); */
/* 		#<{(| u8g2_DrawXBMP(&u8g2, screenSaver.x, screenSaver.y, ScrSaverXBM[0], ScrSaverXBM[1], &ScrSaverXBM[2]); |)}># */
/* 		return 1; */
/* 	} */
/* #endif */
/* 	return 0; */
/* } */

static void  drawMode(uint8_t refresh){
	if(!refresh) return;

	ucg_SetFont(&ucg, &font_18m);
	ucg_SetForeColor(&ucg, C_YELLOW);
	ucg_SetBackColor(&ucg, C_BLACK);

	switch(getCurrentMode()){

		case mode_run:
			{
				char SetTemp[6];
				char c;
				if(systemSettings.settings.tempUnit==mode_Celsius){
					c='C';
				}
				else{
					c='F';
				}
				sprintf(SetTemp,"%u^%c", Iron.CurrentSetTemperature,c);
				ucg_WriteString(&ucg, 99, 0, SetTemp);
				break;
			}

		case mode_sleep:
			ucg_WriteString(&ucg,  strings[lang].main_mode_Sleep_xpos, 0, strings[lang].main_mode_Sleep);
			break;

		case mode_standby:
			ucg_WriteString(&ucg, strings[lang].main_mode_Standby_xpos, 0, strings[lang].main_mode_Standby);
			break;

		case mode_boost:
			ucg_WriteString(&ucg, strings[lang].main_mode_Boost_xpos, 0, strings[lang].main_mode_Boost);

		default:
			break;
	}
	ucg_SetForeColor(&ucg, default_color);
}

static uint8_t  drawPowerBar(uint8_t refresh){
	static uint8_t previousPower=0;
	uint8_t update=refresh;
	uint8_t lastPwr;
	if((current_time-barTime)>9){
		barTime = current_time;
		if(previousPower!=mainScr.lastPwr){
			previousPower = mainScr.lastPwr;
			update=1;
		}
	}
	ucg_SetForeColor(&ucg, C_RED);
	if(update){                          // Update every 10mS or if screen was erased
		/* if(!refresh){                           // If screen not erased */
		/* 	ucg_SetForeColor(&ucg,C_WHITE);                               // Draw a black square to wipe old widget data */
		/* 	#<{(| ucg_FillRectangle(&ucg, ucg_GetXDim(&ucg)-PWR_BAR_WIDTH-2 , ucg_GetYDim(&ucg)-7, PWR_BAR_WIDTH, 5); |)}># */
		/* 	ucg_FillRectangle(&ucg, 0, 74, ucg_GetXDim(&ucg), 5); */
		/* 	ucg_SetForeColor(&ucg,C_RED); */
		/* 	ucg_DrawRectangle(&ucg, 0, 74, ucg_GetXDim(&ucg), 5); */
		/* } */
		/* else{ */
		/* 	#<{(| ucg_DrawRectangle(&ucg, ucg_GetXDim(&ucg)-PWR_BAR_WIDTH-4, ucg_GetYDim(&ucg)-9, PWR_BAR_WIDTH+4, 9); |)}># */
		/* 	ucg_DrawRectangle(&ucg, 0, 74, ucg_GetXDim(&ucg), 5); */
		/* } */
		/* ucg_FillRectangle(&ucg, ucg_GetXDim(&ucg)-PWR_BAR_WIDTH-2, ucg_GetYDim(&ucg)-7, mainScr.lastPwr, 5); */
		lastPwr = mainScr.lastPwr;
		ucg_SetForeColor(&ucg, C_RED);
		ucg_FillRectangle(&ucg, 0, 74, lastPwr, 5);
		ucg_SetForeColor(&ucg, C_WHITE);
		ucg_FillRectangle(&ucg, lastPwr, 74, ucg_GetXDim(&ucg) - lastPwr, 5);
		ucg_SetForeColor(&ucg, default_color);
		return 1;
	}
	ucg_SetForeColor(&ucg, default_color);
	return 0;
}

/* static uint8_t  drawPlot(uint8_t refresh){ */
/* #define PLOT_X  0 */
/* #define PLOT_Y  25 */
/*  */
/* 	ucg_SetForeColor(&ucg, C_YELLOW); */
/* 	plot.enabled &= !(Iron.Error.Flags & FLAG_ACTIVE); */
/*  */
/* 	if(!plot.enabled){ return 0; } */
/* 	if(refresh || plot.update){ */
/* 		int16_t ref; */
/* 		if(Iron.CurrentMode!=mode_sleep){ */
/* 			ref=Iron.CurrentSetTemperature; */
/* 			if(systemSettings.settings.tempUnit==mode_Farenheit){ */
/* 				ref = TempConversion(ref, mode_Celsius, 0); */
/* 			} */
/* 		} */
/* 		else{ */
/* 			ref = (last_NTC_C+5)/10;                                          // Use ambient temperature as reference when sleeping */
/* 		} */
/*  */
/* 		plot.update=0; */
/* 		// plot is 16-56 V, 14-113 H ? */
/* 		#<{(| u8g2_DrawVLine(&u8g2, PLOT_X+3, PLOT_Y, 41);                                 |)}># */
/* 		ucg_DrawLine(&ucg, PLOT_X + 3, PLOT_Y, PLOT_X + 3, PLOT_Y + 41);// left scale */
/* 		for(uint8_t t=0;t<5;t++){ */
/* 			ucg_DrawLine(&ucg, PLOT_X, PLOT_Y + (10 * t), PLOT_X + 3, PLOT_Y + (10 * t));// left ticks */
/* 			#<{(| u8g2_DrawHLine(&u8g2, PLOT_X, PLOT_Y+(10*t), 3);                                      |)}># */
/* 		} */
/* 		#<{(| */
/* 		   12-13-14-15-16-17-18-19-20-21 */
/* 		   22- */
/* 		   32------ */
/* 		   42- */
/* 		   52- */
/* 		   |)}># */
/* 		for(uint8_t x=0; x<100; x++){ */
/* 			uint8_t pos=plot.index+x; */
/* 			if(pos>99){ pos-=100; }                                             // Reset index if > 99 */
/*  */
/* 			int16_t plotV = (plot.d[pos]-ref)+20;                               // relative to t, +-20C */
/*  */
/* 			if (plotV < 1) plotV = 0; */
/* 			else if (plotV > 40) plotV = 40; */
/*  */
/* 			#<{(| u8g2_DrawVLine(&u8g2, x+PLOT_X+7, (PLOT_Y+40)-plotV, plotV+1);               |)}># */
/* 			ucg_DrawLine(&ucg, x + PLOT_X + 7, (PLOT_Y + 40) - plotV, x + PLOT_X + 7, (PLOT_Y + 40) + 1);// data points */
/* 		} */
/* #define set (PLOT_Y+20) */
/* 		#<{(| u8g2_DrawTriangle(&u8g2, PLOT_X+116, set-3, PLOT_X+116, set+3, PLOT_X+110, set);           // Setpoint marker |)}># */
/* 		ucg_SetForeColor(&ucg, C_CYAN); */
/* 		return 1; */
/* 	} */
/* 	ucg_SetForeColor(&ucg, C_CYAN); */
/* 	return 0; */
/* } */

static uint8_t  drawError(uint8_t refresh){
	static uint32_t last_time;
	static uint8_t x_mark_state;
	if(mainScr.ironStatus!=status_error || mainScr.currentMode==main_setpoint ){
		x_mark_state=0;
		last_time = current_time;
		return 0;
	}


	if(Iron.Error.Flags==(FLAG_ACTIVE | FLAG_NO_IRON)){                               // Only "No iron detected". Don't show error screen just for it

		/* uint8_t xp = (ucg_GetXDim(&ucg)-iron[0]-x_mark[0]-5)/2; */
		uint8_t update = 0;

		if(refresh){
			/* u8g2_DrawXBM(&u8g2, xp, (OledHeight-iron[1])/2, iron[0], iron[1], &iron[2]); */
			update = 1;
		}

		if(current_time-last_time>500){
			last_time=current_time;
			x_mark_state ^=1;
			update=1;
		}

		if(update){
			if(x_mark_state){
				ucg_SetForeColor(&ucg, C_BLACK);
				/* ucg_FillRectangle(&ucg, xp+iron[0]+5, (ucg_GetYDim(&ucg)-x_mark[1])/2, x_mark[0], x_mark[1]); */
				ucg_FillRectangle(&ucg, 0, 19, 160, 53);
				ucg_SetForeColor(&ucg, default_color);
			}
			else{
				ucg_SetForeColor(&ucg, C_RED);
				ucg_WriteString(&ucg, 0, 25, strings[lang].main_error_noIron_Detected);
				/* u8g2_DrawXBM(&u8g2, xp+iron[0]+5, (OledHeight-x_mark[1])/2, x_mark[0], x_mark[1], &x_mark[2]); */
			}
		}
		return update;
	}
	else if(refresh){
		uint8_t Err_ypos;

		uint8_t err = (uint8_t)Iron.Error.V_low+Iron.Error.safeMode+(Iron.Error.NTC_low|Iron.Error.NTC_high)+Iron.Error.noIron;
		if(err<4){
			Err_ypos= 12+ ((40-(err*12))/2);
		}
		else{
			Err_ypos=12;
		}
		ucg_SetFont(&ucg, &font_18m);
		ucg_SetForeColor(&ucg, C_RED);
		/* widgetDisable(Widget_IronTemp); */
		/* widgetDisable(Widget_SetPoint); */
		if(Iron.Error.V_low){
			ucg_WriteString(&ucg, 0, Err_ypos, strings[lang].main_error_VoltageLow);
			Err_ypos+=25;
		}
		if(Iron.Error.safeMode){
			ucg_WriteString(&ucg, 0, Err_ypos, strings[lang].main_error_failsafe);
			Err_ypos+=25;
		}
		if(Iron.Error.NTC_high){
			ucg_WriteString(&ucg, 0, Err_ypos, strings[lang].main_error_NTC_high);
			Err_ypos+=25;
		}
		else if(Iron.Error.NTC_low){
			ucg_WriteString(&ucg, 0, Err_ypos, strings[lang].main_error_NTC_low);
			Err_ypos+=25;
		}
		if(Iron.Error.noIron){
			ucg_WriteString(&ucg, 0, Err_ypos, strings[lang].main_error_noIron_Detected);
			Err_ypos+=25;
		}
		return 1;
	}
	/* widgetEnable(Widget_IronTemp); */
	/* widgetEnable(Widget_SetPoint); */
	return 0;
}

/* static void  drawMisc(uint8_t refresh){ */
/* 	if(!refresh) return; */
/*  */
/* 	Widget_SetPoint->enabled &= (mainScr.currentMode==main_setpoint);                            // Disable setpoint widget if not in setpoint screen */
/*  */
/* 	ucg_SetFont(&ucg, &font_18m); */
/* 	if(mainScr.currentMode==main_tipselect){ */
/* 		uint8_t len = ucg_GetStrWidth(&ucg, ucg_GetFont(&ucg), tipNames[systemSettings.Profile.currentTip])+4;   // Draw edit frame */
/* 		ucg_DrawRectangle(&ucg, 118, 53, len, 21); */
/* 		ucg_SetForeColor(&ucg, C_CYAN); */
/* 	} */
/* 	ucg_WriteString(&ucg, 119, 54, tipNames[systemSettings.Profile.currentTip]);                  // Draw tip name */
/* 		ucg_SetForeColor(&ucg, C_CYAN); */
/* 	return; */
/* } */

static uint8_t main_screen_draw(screen_t *scr){
	uint8_t refresh=0,ret=0;
	static uint32_t lastState = 0;
	uint32_t currentState = (uint32_t)Iron.Error.Flags<<24 | (uint32_t)Iron.CurrentMode<<16 | mainScr.currentMode;    // Simple method to detect changes

	if( lastState!=currentState || Widget_SetPoint->refresh || Widget_IronTemp->refresh || plot.update || screenSaver.update || scr->refresh==screen_Erase
#ifdef USE_NTC
			|| Widget_AmbTemp->refresh
#endif
#ifdef USE_VIN
			|| Widget_Voltage->refresh
#endif
	  ){

		lastState=currentState;
		refresh=1;
	}
if(systemSettings.settings.tempUnit == mode_Celsius)
		((bmp_widget_t*)Widget_CLabel->content)->xbm =  &cel;
	else ((bmp_widget_t*)Widget_CLabel->content)->xbm =  &far;

	if(refresh){
		scr->refresh=screen_Erased;
		//TODO
		/* ucg_SetForeColor(&ucg, C_BLACK); */
		/* ucg_FillScreen(&ucg); */
		/* ucg_SetForeColor(&ucg, C_CYAN); */
	}


	if(mainScr.ironStatus != status_error){
		/* ret |= drawScreenSaver(refresh); */
	}
	ret |= drawPowerBar(refresh);
	/* ret |= drawIcons(refresh); */
	drawMode(refresh);
	/* drawMisc(refresh); */
	/* ret |= drawPlot(refresh); */
	ret |= drawError(refresh);
	ret |= default_screenDraw(scr);
	if((Iron.Error.Flags&FLAG_ACTIVE) && mainScr.ironStatus!=status_error){
		return 0;                                                                                // If a new error appeared during the screen draw, skip oled update to avoid random artifacts
	}
	return (ret);
}

static void main_screen_init(screen_t *scr) {
	editable_widget_t *edit;
	default_init(scr);
	Iron.shakeActive = 0;
	mainScr.shakeActive = 0;
	plot.timeStep = (systemSettings.Profile.readPeriod+1)/200;                                                         // Update at the same rate as the system pwm

	mainScr.setMode = main_irontemp;
	switchScreenMode();

	edit = extractEditablePartFromWidget(Widget_SetPoint);
	edit->step = systemSettings.settings.tempStep;
	edit->big_step = systemSettings.settings.tempBigStep;
	edit->max_value = systemSettings.Profile.MaxSetTemperature;
	edit->min_value = systemSettings.Profile.MinSetTemperature;
	setMainScrTempUnit();
}

static void main_screen_create(screen_t *scr){
	widget_t *w;
	displayOnly_widget_t* dis;
	editable_widget_t* edit;
	selectable_widget_t* sel;

	//  [ Iron Temp Widget ]
	//
	newWidget(&w,widget_display,scr);
	Widget_IronTemp = w;
	dis=extractDisplayPartFromWidget(w);
	edit=extractEditablePartFromWidget(w);
	dis->reservedChars=5;
	dis->dispAlign=align_disabled;
	dis->textAlign=align_right;
	dis->font=&font_53;
	dis->type = field_temp;
	/* dis->type = field_uint16; */
	w->fcolor = C_GREEN;
	w->posY = 19;
	dis->getData = &main_screen_getIronTemp;
	w->width = 120;
	w->enabled=0;

	//  [ Iron Setpoint Widget ]
	//
	newWidget(&w,widget_editable,scr);
	Widget_SetPoint=w;
	dis=extractDisplayPartFromWidget(w);
	edit=extractEditablePartFromWidget(w);
	dis->reservedChars=5;
	w->posY = 19;
	dis->getData = &getTemp;
	dis->dispAlign=align_disabled;
	dis->textAlign=align_right;
	dis->font=((displayOnly_widget_t*)Widget_IronTemp->content)->font;
	edit->selectable.tab = 1;
	edit->setData = (void (*)(void *))&setTemp;
	w->frameType=frame_solid;
	edit->selectable.state=widget_edit;
	w->radius=8;
	w->fcolor = C_COLOR565(0xFF, 0xAA, 0x00);
	w->enabled=0;
	w->width=120;

	// [C label ]
	newWidget(&w, widget_bmp, scr);
	((bmp_widget_t*)w->content)->xbm =  &cel;
	w->posX = ucg_GetXDim(&ucg) - ((bmp_widget_t*)w->content)->xbm->width;
	w->posY = ucg_GetYDim(&ucg) - ((bmp_widget_t*)w->content)->xbm->height - 8;
	w->draw = &default_widgetDraw;
	w->fcolor = C_GREEN;
	w->bcolor = C_BLACK;
	Widget_CLabel = w;

	//[ SelectTip Widget]
	newWidget(&w, widget_multi_option, scr);
	Widget_SetTip = w;
	dis = extractDisplayPartFromWidget(w);
	edit = extractEditablePartFromWidget(w);
	sel = extractSelectablePartFromWidget(w);
	dis->reservedChars = 5;
	dis->font = default_font;
	dis->textAlign = align_left;
	w->fcolor = C_BLUE;
	edit->inputData.getData = &getTip;
	edit->numberOfOptions = systemSettings.Profile.currentNumberOfTips;
	edit->options = tipNames;
	edit->step = 1;
	edit->big_step = 1;
	sel->state = widget_edit;
	sel->tab = 0;
	w->posY = 0;
	w->enabled = 0;
	w->width = 60;


#ifdef USE_VIN
	//  [ V. Supply Widget ]
	//
	newWidget(&w,widget_display,scr);
	Widget_Voltage=w;
	dis=extractDisplayPartFromWidget(w);
	dis->getData = &main_screen_getVin;
	dis->endString="V";
	dis->reservedChars=5;
	dis->textAlign=align_center;
	dis->number_of_dec=1;
	dis->font=default_font;
	w->fcolor = C_MAGENTA;
	w->posY= 0;
	w->posX = 0;
	edit=extractEditablePartFromWidget(w);
	w->width = 60;
#endif

#ifdef USE_NTC
	//  [ Ambient Temp Widget ]
	//
	newWidget(&w,widget_display,scr);
	Widget_AmbTemp=w;
	dis=extractDisplayPartFromWidget(w);
	dis->reservedChars=7;
	dis->dispAlign=align_right;
	dis->textAlign=align_right;
	dis->number_of_dec=1;
	dis->font=default_font;
	dis->getData = &main_screen_getAmbTemp;
	w->posY = 0;
	w->posX = 90;
	w->width = 38;
#endif
}


void main_screen_setup(screen_t *scr) {
	scr->draw = &main_screen_draw;
	scr->init = &main_screen_init;
	scr->processInput = &main_screenProcessInput;
	scr->create = &main_screen_create;

	for(int x = 0; x < TipSize; x++) {
		tipNames[x] = systemSettings.Profile.tip[x].name;
	}
}
