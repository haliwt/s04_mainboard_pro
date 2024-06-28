/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

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
#define ADC_FAN_Pin GPIO_PIN_0
#define ADC_FAN_GPIO_Port GPIOA
#define ADC_NTC_Pin GPIO_PIN_1
#define ADC_NTC_GPIO_Port GPIOA



#define FAN_CMD_2_Pin GPIO_PIN_6
#define FAN_CMD_2_GPIO_Port GPIOA
#define FAN_CMD_1_Pin GPIO_PIN_7
#define FAN_CMD_1_GPIO_Port GPIOA
#define BEEP_PWM_Pin GPIO_PIN_1
#define BEEP_PWM_GPIO_Port GPIOB
#define TEMP_SENSOR_Pin GPIO_PIN_13
#define TEMP_SENSOR_GPIO_Port GPIOB
#define ULTR_PWM_Pin GPIO_PIN_8
#define ULTR_PWM_GPIO_Port GPIOA


#define PTC_PIN     GPIO_PIN_0
#define PTC_GPIO    GPIOB



#define PLASMA_Pin              GPIO_PIN_5
#define PLASMA_GPIO_Port        GPIOA




/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
