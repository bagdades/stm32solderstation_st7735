/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "board.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
// #include <math.h>
#include <malloc.h>
// #include "stm32f1xx_hal_iwdg.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
// extern IWDG_HandleTypeDef hiwdg;
extern CRC_HandleTypeDef hcrc;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

// this is left here just to have it handy for copying when debugging a specific function
// don't uncomment!!
//               __attribute__((optimize("o0")))

#define debug_error

#ifdef debug
extern struct mallinfo mi;
#define debug_alloc                   // enable alloc debugging in debug builds
#endif
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

// stm32cube ide removed line printing in error handler. this macro restores it.
// Credits: https://community.st.com/s/question/0D50X00009XkffVSAR/stm32cubemx-v421-errorhandler-definition-issues-in-mainh
#ifdef DEBUG_ERROR
	#define GET_MACRO( _0, _1, NAME, ... ) NAME
	#define Error_Handler(...) GET_MACRO( _0, ##__VA_ARGS__, Error_Handler1, Error_Handler0 )()
	#define Error_Handler0() _Error_Handler(__BASE_FILE__, __LINE__ )
	#define Error_Handler1(unused) _Error_Handler( char * file, int line )
	void _Error_Handler(char *, int);
#endif
/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

void Program_Handler(void);



/*
 * Macro to enable debugging of the allocated memory
 * max_allocated will hold the max used memory at any time
 *
 */
#ifdef DEBUG_ALLOC
extern uint32_t max_allocated;
#define dbg_mem() mi=mallinfo();                  \
                  if(mi.uordblks>max_allocated){  \
                    max_allocated=mi.uordblks;     \
                  }                               \

#define _malloc(x)    malloc(x); dbg_mem()
#define _calloc(x,y)  calloc(x,y); dbg_mem()
#define _free(x)      free(x); dbg_mem()

#else
#define _malloc     malloc
#define _calloc     calloc
#define _free       free
#endif
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define WAKE_Pin GPIO_PIN_0
#define WAKE_GPIO_Port GPIOA
#define BUZ0_Pin GPIO_PIN_1
#define BUZ0_GPIO_Port GPIOA
#define BUZ1_Pin GPIO_PIN_2
#define BUZ1_GPIO_Port GPIOA
#define NTC_Pin GPIO_PIN_5
#define NTC_GPIO_Port GPIOA
#define PWM_Pin GPIO_PIN_6
#define PWM_GPIO_Port GPIOA
#define VIN_Pin GPIO_PIN_0
#define VIN_GPIO_Port GPIOB
#define TIP_Pin GPIO_PIN_1
#define TIP_GPIO_Port GPIOB
#define LCD_SCL_Pin GPIO_PIN_13
#define LCD_SCL_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_14
#define LCD_CS_GPIO_Port GPIOB
#define LCD_SDA_Pin GPIO_PIN_15
#define LCD_SDA_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_8
#define LCD_RST_GPIO_Port GPIOA
#define LCD_DC_Pin GPIO_PIN_9
#define LCD_DC_GPIO_Port GPIOA
#define ENC_SW_Pin GPIO_PIN_15
#define ENC_SW_GPIO_Port GPIOA
#define ENC_L_Pin GPIO_PIN_3
#define ENC_L_GPIO_Port GPIOB
#define ENC_R_Pin GPIO_PIN_4
#define ENC_R_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define WAKE_input()		HAL_GPIO_ReadPin(WAKE_GPIO_Port, WAKE_Pin)
#define BUTTON_input()		HAL_GPIO_ReadPin(ENC_SW_GPIO_Port, ENC_SW_Pin)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
