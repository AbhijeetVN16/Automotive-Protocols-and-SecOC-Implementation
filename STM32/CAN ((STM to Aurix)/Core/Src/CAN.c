/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : can.c
  * @brief          : CAN communication functions
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "CAN.h"

/* Private variables ---------------------------------------------------------*/
extern CAN_HandleTypeDef hcan;

/* Private function prototypes -----------------------------------------------*/
static void can_set_data(uint8_t *TxData);

/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Sends a CAN message with predefined data and ID.
  * @retval None
  */
void can_send_message(void)
{
    uint8_t TxData[8];
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;

    // Set up the data payload
    can_set_data(TxData);

    // Configure the CAN message header
    TxHeader.StdId = CAN_MESSAGE_ID_TX; // Matches Aurix receiver
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = 8;

    // Transmit the CAN message
    if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
    {
        Error_Handler();
    }
}

/* Private user code ---------------------------------------------------------*/
static void can_set_data(uint8_t *TxData)
{
    // The CAN message data to send, matching the Aurix receiver's expectation
    // Corresponds to 0xBA5EBA11 and 0xC0CAC01A in the Aurix code.
    TxData[0] = 0xBA;
    TxData[1] = 0x5E;
    TxData[2] = 0xBA;
    TxData[3] = 0x11;
    TxData[4] = 0xC0;
    TxData[5] = 0xCA;
    TxData[6] = 0xC0;
    TxData[7] = 0x1A;
}
