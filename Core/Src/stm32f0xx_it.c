/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "circular_buffer.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim14;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */
extern cbuf_handle_t cbuf;
extern SPI_HandleTypeDef hspi1;

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM14 global interrupt.
  */
void TIM14_IRQHandler(void)
{
  /* USER CODE BEGIN TIM14_IRQn 0 */
  volatile uint16_t word;
  static bool flip_flop = false;
  volatile uint32_t byte_msb, byte_lsb;
//  static uint16_t word;
  static uint32_t test_data = 0; // FOR TEST ONLY!!! REMOVE IT LATER !!!

  /* USER CODE END TIM14_IRQn 0 */
  HAL_TIM_IRQHandler(&htim14);
  /* USER CODE BEGIN TIM14_IRQn 1 */
  if(true == flip_flop){
    GPIOA->BSRR = CNVST_Pin; // Set High
  	flip_flop = false;
//    GPIOA->BSRR = (uint32_t)CNVST_Pin <<16u; // Set LOW
    // do nothing, this is ADC convert phase
  }else{
  	// SPI read; Ring Buffer write
  	flip_flop = true;
    GPIOA->BSRR = (uint32_t)CNVST_Pin <<16u; // Set LOW

//    GPIOA->BSRR = TEST_PA4_Pin; // Set High
#define SPILOW 1
#ifdef SPILOW
    bool txallowed = true;
    bool rxMsb = true;
    while (true){
      /* Check TXE flag */
      if ( (SPI1->SR & SPI_SR_TXE) && (txallowed == true)){
        SPI1->DR = 0xFF;
        /* Next Data is a reception (Rx). Tx not allowed */
        txallowed = false;
      }
      /* Wait until RXNE flag is reset */
      if( SPI1->SR & SPI_SR_RXNE )
      {
        if(rxMsb == true){
          byte_msb = (*(volatile uint8_t *)&SPI1->DR);

      	  rxMsb = false;
        }else{
          byte_lsb = (*(volatile uint8_t *)&SPI1->DR);
          break;
        }
        /* Next Data is a Transmission (Tx). Tx is allowed */
        txallowed = true;
      }
    }
    word = (byte_lsb & 0xFF) + ((byte_msb & 0xFF)<< 8 );
#else // SPILOW - no
    uint8_t pData[10];
    HAL_SPI_TransmitReceive (&hspi1, pData, pData, 2, HAL_MAX_DELAY);
    word = (pData[0]<<8) + pData[1];
#endif // SPILOW - end
//    GPIOA->BSRR = (uint32_t)TEST_PA4_Pin <<16u; // Set LOW

    /**************************************************/
    /****  // FOR TEST ONLY!!! REMOVE IT LATER !!! ****/
  #ifdef TEST_SPI_DATA
    word = test_data++;
  #endif
    /**************************************************/

    circular_buf_put(cbuf, word); //
//    GPIOA->BSRR = (uint32_t)TEST_PA4_Pin <<16u; // Set LOW
  } // if(true == flip_flop)/else

  /* USER CODE END TIM14_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
