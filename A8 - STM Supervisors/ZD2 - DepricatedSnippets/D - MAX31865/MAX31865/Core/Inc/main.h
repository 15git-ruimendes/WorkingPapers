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
#include "stm32f4xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CLK_Pin GPIO_PIN_4
#define CLK_GPIO_Port GPIOA
#define MOSI_Pin GPIO_PIN_5
#define MOSI_GPIO_Port GPIOA
#define MISO_Pin GPIO_PIN_6
#define MISO_GPIO_Port GPIOA
#define CS5_Pin GPIO_PIN_7
#define CS5_GPIO_Port GPIOA
#define CS1_Pin GPIO_PIN_4
#define CS1_GPIO_Port GPIOC
#define CS2_Pin GPIO_PIN_5
#define CS2_GPIO_Port GPIOC
#define CS3_Pin GPIO_PIN_0
#define CS3_GPIO_Port GPIOB
#define CS4_Pin GPIO_PIN_1
#define CS4_GPIO_Port GPIOB
#define CS6_Pin GPIO_PIN_10
#define CS6_GPIO_Port GPIOB
#define CS7_Pin GPIO_PIN_12
#define CS7_GPIO_Port GPIOB
#define CS8_Pin GPIO_PIN_13
#define CS8_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */