/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : CAN.h
  * @brief          : Header file for CAN.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __CAN_H
#define __CAN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported functions prototypes ---------------------------------------------*/
void can_filter_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
