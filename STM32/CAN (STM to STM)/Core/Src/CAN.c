#include"CAN.h"
#include"main.h"

extern CAN_HandleTypeDef hcan;
can_struct_t myCan;

// Starts all the modules, Both Tx and Rx part
void ancit_can_start(void)
{
/* IMPORTANT NOTE
            The following line has to be copied into stm32f1xx_hal_can.c file
            before the comment // Wait initialisation acknowledge
            else, HAL_CAN_Init will hang...

            CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_SLEEP);
    */

    ancit_can_init_rx_filter();
    HAL_CAN_Start(&hcan);

// Enable Rx interrupts.
 if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
  Error_Handler();
  }
}
// Sets up the rx_filter
void ancit_can_init_rx_filter(void)
{
CAN_FilterTypeDef sFilterConfig;

    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

 if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
  {
 /* Filter configuration Error */
   Error_Handler();
  }
 // init Rx flag to 0;
  myCan.rx_message_flag = 0;
}
// Rx Message Interrupt handler
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
 //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
 if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &myCan.RxHeader, myCan.RxData) != HAL_OK)
  {
   Error_Handler();
  }
 else
  {
   myCan.rx_message_flag = 1;
  }
}
void ancit_can_check_for_rx(void)
{
 if (myCan.rx_message_flag == 1)
  {
   myCan.rx_message_flag = 0;
 // message received, take action here...
 if (myCan.RxHeader.StdId == 0x07)
   {
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
      HAL_Delay(100);
   }
  }
}
void ancit_can_tx_message(void)
{
  myCan.TxHeader.IDE = CAN_ID_STD;
  myCan.TxHeader.StdId = 0x08;
  myCan.TxHeader.RTR = CAN_RTR_DATA;
  myCan.TxHeader.DLC = 1;
  myCan.TxData[0]=1;


 // Inserting call functions from DBC generated file.
 if (HAL_CAN_AddTxMessage(&hcan, &myCan.TxHeader, myCan.TxData, &myCan.TxMailbox) !=
HAL_OK)
  {
    //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
   Error_Handler();
  }

}
void ancit_can_wait_for_switch_press(void)
{
uint8_t switch_status = 0;
static uint8_t wait_for_falling_edge = 1;
// switch_status = HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin);
    switch_status = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);
if (switch_status == 0)
{
	// switch debounce delay
	   HAL_Delay(25);
	 //switch_status = HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin);
	   switch_status =  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);
	 if (switch_status == 0)
	   {
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,!switch_status );
	 if (wait_for_falling_edge == 1)
	    {
	     wait_for_falling_edge = 0;
	     ancit_can_tx_message();

	    }
	   }
	  }
	 else
	  {
	   wait_for_falling_edge = 1;
	  }
	}


