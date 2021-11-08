/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/**************************************************/
/****  // FOR TEST ONLY!!! REMOVE IT LATER !!! ****/
//#define TEST_SPI_DATA					1
/**************************************************/

#define RING_BUFFER_SIZE			256 // Let TBD again
#define AVERAGE_WINDOW_SIZE			7 // Must be number of averaging minus 1 //was: 8 // Let TBD again
#define SHIFT_AVRG_DATA				3 // was: 3 // This MUST be correlated with AVERAGE_WINDOW_SIZE <= 2^SHIFT_AVRG_DATA
#define COBS_BUFF_SIZE				64

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BAUDRATE 115200
#define TIM14_US 8 // MUST be 8, nor 800 !!!!
#define RS485_DERE__Pin GPIO_PIN_0
#define RS485_DERE__GPIO_Port GPIOA
#define CNVST_Pin GPIO_PIN_3
#define CNVST_GPIO_Port GPIOA
#define TEST_PA4_Pin GPIO_PIN_4
#define TEST_PA4_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
