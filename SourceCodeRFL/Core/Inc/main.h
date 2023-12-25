/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DA_595_Pin GPIO_PIN_0
#define DA_595_GPIO_Port GPIOA
#define SA_4051_Pin GPIO_PIN_1
#define SA_4051_GPIO_Port GPIOA
#define SB_4051_Pin GPIO_PIN_2
#define SB_4051_GPIO_Port GPIOA
#define SH_595_Pin GPIO_PIN_3
#define SH_595_GPIO_Port GPIOA
#define ST_595_Pin GPIO_PIN_5
#define ST_595_GPIO_Port GPIOA
#define SC_4051_Pin GPIO_PIN_6
#define SC_4051_GPIO_Port GPIOA
#define DIR_LEFT_Pin GPIO_PIN_15
#define DIR_LEFT_GPIO_Port GPIOB
#define TIM1_CH1_HC_SR04_ECHO_Pin GPIO_PIN_8
#define TIM1_CH1_HC_SR04_ECHO_GPIO_Port GPIOA
#define DIR_RIGHT_Pin GPIO_PIN_10
#define DIR_RIGHT_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
