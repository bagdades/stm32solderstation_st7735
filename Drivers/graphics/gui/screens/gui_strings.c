/*
 * gui_strings.c
 *
 *  Created on: 27 ene. 2021
 *      Author: David
 */
#include "gui_strings.h"

// language indexes and LANGUAGE_COUNT defined in settings.h

const strings_t strings[LANGUAGE_COUNT] = {

    [lang_english] = {
      .boot_firstBoot = "First Boot!",
      .boot_Profile = "Profile",

      .main_error_noIron_Detected = "NO IRON DETECTED",
      .main_error_failsafe = "FAILSAFE MODE",
      .main_error_NTC_high = "NTC READ HIGH",
      .main_error_NTC_low = "NTC READ LOW",
      .main_error_VoltageLow = "VOLTAGE LOW",
      .main_mode_Sleep = "SLEEP",
      .main_mode_Sleep_xpos = 99,
      .main_mode_Standby = " STBY",
      .main_mode_Standby_xpos = 99,
      .main_mode_Boost = "BOOST",
      .main_mode_Boost_xpos = 99,

      .settings_IRON = "IRON",
      .settings_SYSTEM = "SYSTEM",
      .settings_DEBUG = "DEBUG",
      .settings_EDIT_TIPS = "EDIT TIPS",
      .settings_CALIBRATION = "CALIBRATION",
      .settings_EXIT = "EXIT",

      .IRON_Max_Temp = "Max temp",
      .IRON_Min_Temp = "Min temp",
      .IRON_User_Temp = "User temp",
      .IRON_Standby = "Standby",
      .IRON_Sleep = "Sleep",
      .IRON_Boost = "Boost",
      .IRON_Boost_Add = "Increase",
      .IRON_Power = "Power",
      .IRON_Heater = "Heater",
      .IRON_ADC_Time = "ADC Time",
      .IRON_PWM_mul = "PWM mul.",
      .IRON_No_Iron = "No iron",
      .IRON_Error_Timeout = "Err time",
      .IRON_Error_Resume_Mode = "Resume",
      .IRON_FILTER_MENU = "FILTER MENU",
      .IRON_NTC_MENU = "NTC MENU",

      .FILTER_Filter = "Filter",
      .FILTER__Threshold = "Threshold",
      .FILTER__Count_limit = "Count limit",
      .FILTER__Step_down = "Step down",
      .FILTER__Min = "Min",
      .FILTER_Reset_limit = "Reset lmt",

      .SYSTEM_Profile = "Profile",
      .SYSTEM_Oled_Contrast = "Contrast",
      .SYSTEM_Oled_Dim = "Dimmer",
      .SYSTEM_Oled_Dim_inSleep = "In sleep",
      .SYSTEM_Oled_Offset = "Offset",
      .SYSTEM_Wake_Mode = "Wake md.",
      .SYSTEM_Shake_Filtering = "Filter",
      .SYSTEM_Stand_Mode = "In stand",
      .SYSTEM_Boot = "Boot",
      .SYSTEM_Button_Wake = "Btn wake",
      .SYSTEM_Shake_Wake = "Shake wk",
      .SYSTEM_Encoder = "Encoder",
      .SYSTEM_Buzzer = "Buzzer",
      .SYSTEM_Temperature = "Temp.",
      .SYSTEM__Step = "Step",
      .SYSTEM__Big_Step = "Big step",
      .SYSTEM_Active_Detection = "Active det.",
      .SYSTEM_LVP = "LVP",
      .SYSTEM_Gui_Time = "Gui time",
      .SYSTEM_DEBUG = "DEBUG",
      .SYSTEM_RESET_MENU = "RESET MENU",

      .NTC_Enable_NTC = "Enable NTC",
      .NTC_Pull = "Pull",
      .NTC__Res = "Res",
      .NTC__Beta = "Beta",
      .NTC_NTC_Detect = "NTC Detect",
      .NTC__High = "High",
      .NTC__Low = "Low",

      .RESET_Reset_Settings = "Reset Settings",
      .RESET_Reset_Profile = "Reset Profile",
      .RESET_Reset_Profiles = "Reset Profiles",
      .RESET_Reset_All = "Reset All",
      .RESET_Reset_msg_settings_1 = "RESET SYSTEM",
      .RESET_Reset_msg_settings_2 = "SETTINGS?",
      .RESET_Reset_msg_profile_1 = "RESET CURRENT",
      .RESET_Reset_msg_profile_2 = "PROFILE?",
      .RESET_Reset_msg_profiles_1 = "RESET ALL",
      .RESET_Reset_msg_profiles_2 = "PROFILES?",
      .RESET_Reset_msg_all_1 = "PERFORM FULL",
      .RESET_Reset_msg_all_2 = "SYSTEM RESET?",

      .TIP_SETTINGS_Name = "Name",
      .TIP_SETTINGS_PID_kp = "PID Kp",
      .TIP_SETTINGS_PID_ki = "PID Ki",
      .TIP_SETTINGS_PID_kd = "PID Kd",
      .TIP_SETTINGS_PID_Imax = "PID Imax",
      .TIP_SETTINGS_PID_Imin = "PID Imin",
      .TIP_SETTINGS_COPY = "COPY",
      .TIP_SETTINGS_DELETE = "DELETE",

      .CAL_ZeroSet = "Zero set  ",       // Must be 11 chars long
      .CAL_Sampling = "Sampling  ",      // Must be 11 chars long
      .CAL_Captured = "Captured  ",      // Must be 11 chars long
      .CAL_Step = "CAL STEP:",
      .CAL_Wait = "WAIT...",
      .CAL_Measured = "MEASURED:",
      .CAL_Success = "SUCCESS!",
      .CAL_Failed = "FAILED!",
      .CAL_DELTA_HIGH_1 = "DELTA TOO HIGH!",
      .CAL_DELTA_HIGH_2 = "Adjust manually",
      .CAL_DELTA_HIGH_3 = "and try again",
      .CAL_Error = "ERROR DETECTED!",
      .CAL_Aborting = "Aborting...",

      ._Language = "Language",
      .__Temp = "Temp",
      .__Delay = "Delay",
      ._Cal_250 = "Cal 250 C",
      ._Cal_400 = "Cal 400 C",
      ._BACK = "BACK",
      ._SAVE = "SAVE",
      ._CANCEL = "CANCEL",
      ._STOP = "STOP",
      ._RESET = "RESET",
      ._START = "START",
      ._SETTINGS = "SETTINGS",
      ._ADD_NEW = "ADD NEW",

      .ERROR_RUNAWAY = "TEMP RUNAWAY",
      .ERROR_EXCEEDED = "EXCEEDED",
      .ERROR_UNKNOWN = "UNKNOWN ERROR",
      .ERROR_SYSTEM_HALTED = "SYSTEM HALTED",
      .ERROR_BTN_RESET = "Use btn to reset",

      .OffOn =       { "OFF", "ON" },
      .DownUp =      { "DOWN", "UP" },
      .WakeModes =   { "OFF", "STBY", "SLP", "ALL" },
      .wakeMode =    { "SHAKE", "STAND" },
      .encMode =     { "REV", "FWD" },
      .InitMode =    { "SLP", "STBY", "RUN" },
      .dimMode =     { "OFF", "SLP", "ALL" },
      .errMode =     { "SLP", "RUN", "LAST" },
    },
};


char * tempUnit[2] =    { "C", "F" };
char * profileStr[ProfileSize] = { "T12", "C245", "C210" };
char * Langs[LANGUAGE_COUNT] = {
                                   [lang_english] = "EN",
                               };
