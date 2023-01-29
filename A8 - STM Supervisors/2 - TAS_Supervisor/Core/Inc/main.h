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
#define EN_7_Pin GPIO_PIN_13
#define EN_7_GPIO_Port GPIOC
#define TS_0_Pin GPIO_PIN_0
#define TS_0_GPIO_Port GPIOC
#define TS_1_Pin GPIO_PIN_1
#define TS_1_GPIO_Port GPIOC
#define EN_0_Pin GPIO_PIN_2
#define EN_0_GPIO_Port GPIOC
#define EN_1_Pin GPIO_PIN_3
#define EN_1_GPIO_Port GPIOC
#define CLK_Pin GPIO_PIN_0
#define CLK_GPIO_Port GPIOA
#define MOSI_Pin GPIO_PIN_1
#define MOSI_GPIO_Port GPIOA
#define MISO_Pin GPIO_PIN_2
#define MISO_GPIO_Port GPIOA
#define EN_2_Pin GPIO_PIN_3
#define EN_2_GPIO_Port GPIOA
#define uSD_CS_Pin GPIO_PIN_4
#define uSD_CS_GPIO_Port GPIOA
#define SFLH_In_Pin GPIO_PIN_4
#define SFLH_In_GPIO_Port GPIOC
#define SFLH_Out_Pin GPIO_PIN_5
#define SFLH_Out_GPIO_Port GPIOC
#define SFLL_In_Pin GPIO_PIN_0
#define SFLL_In_GPIO_Port GPIOB
#define SFLL_Out_Pin GPIO_PIN_1
#define SFLL_Out_GPIO_Port GPIOB
#define EN_3_Pin GPIO_PIN_10
#define EN_3_GPIO_Port GPIOB
#define CS_7_Pin GPIO_PIN_12
#define CS_7_GPIO_Port GPIOB
#define CS_6_Pin GPIO_PIN_13
#define CS_6_GPIO_Port GPIOB
#define CS_5_Pin GPIO_PIN_14
#define CS_5_GPIO_Port GPIOB
#define CS_4_Pin GPIO_PIN_15
#define CS_4_GPIO_Port GPIOB
#define CS_3_Pin GPIO_PIN_6
#define CS_3_GPIO_Port GPIOC
#define CS_2_Pin GPIO_PIN_7
#define CS_2_GPIO_Port GPIOC
#define CS_1_Pin GPIO_PIN_8
#define CS_1_GPIO_Port GPIOC
#define CS_0_Pin GPIO_PIN_9
#define CS_0_GPIO_Port GPIOC
#define EN_6_Pin GPIO_PIN_8
#define EN_6_GPIO_Port GPIOA
#define EN_5_Pin GPIO_PIN_11
#define EN_5_GPIO_Port GPIOA
#define EN_4_Pin GPIO_PIN_12
#define EN_4_GPIO_Port GPIOA
#define TS_7_Pin GPIO_PIN_10
#define TS_7_GPIO_Port GPIOC
#define TS_6_Pin GPIO_PIN_11
#define TS_6_GPIO_Port GPIOC
#define TS_5_Pin GPIO_PIN_12
#define TS_5_GPIO_Port GPIOC
#define TS_4_Pin GPIO_PIN_2
#define TS_4_GPIO_Port GPIOD
#define TS_3_Pin GPIO_PIN_4
#define TS_3_GPIO_Port GPIOB
#define TS_2_Pin GPIO_PIN_5
#define TS_2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
