/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : can.h
  * @brief          : Header file for CAN.c
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
void can_send_message(void);

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H */
