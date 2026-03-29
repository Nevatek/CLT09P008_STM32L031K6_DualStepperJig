/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
TIM_HandleTypeDef* GetInstance_Timer2(void);
TIM_HandleTypeDef* GetInstance_Timer21(void);
UART_HandleTypeDef* GetInstance_UART2(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HOME_SENS_OUT_1_Pin GPIO_PIN_1
#define HOME_SENS_OUT_1_GPIO_Port GPIOA
#define HOME_SENS_OUT_2_Pin GPIO_PIN_3
#define HOME_SENS_OUT_2_GPIO_Port GPIOA
#define SW1_INP_Pin GPIO_PIN_4
#define SW1_INP_GPIO_Port GPIOA
#define SW2_INP_Pin GPIO_PIN_5
#define SW2_INP_GPIO_Port GPIOA
#define SW3_INP_Pin GPIO_PIN_6
#define SW3_INP_GPIO_Port GPIOA
#define SW4_INP_Pin GPIO_PIN_7
#define SW4_INP_GPIO_Port GPIOA
#define SW5_INP_Pin GPIO_PIN_0
#define SW5_INP_GPIO_Port GPIOB
#define SW6_INP_Pin GPIO_PIN_1
#define SW6_INP_GPIO_Port GPIOB
#define MOT1_PUL__Pin GPIO_PIN_8
#define MOT1_PUL__GPIO_Port GPIOA
#define MOT1_DISABLE__Pin GPIO_PIN_9
#define MOT1_DISABLE__GPIO_Port GPIOA
#define MOT2_PUL__Pin GPIO_PIN_10
#define MOT2_PUL__GPIO_Port GPIOA
#define MOT2_DISABLE__Pin GPIO_PIN_11
#define MOT2_DISABLE__GPIO_Port GPIOA
#define MOT2_DIR__Pin GPIO_PIN_6
#define MOT2_DIR__GPIO_Port GPIOB
#define MOT1_DIR__Pin GPIO_PIN_7
#define MOT1_DIR__GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
