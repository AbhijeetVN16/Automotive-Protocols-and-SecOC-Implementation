/*
 * CAN.h
 *
 *  Created on: May 8, 2025
 *      Author: abhij
 */

#ifndef SRC_CAN_H_
#define SRC_CAN_H_


#include"main.h"
typedef struct
{
 CAN_TxHeaderTypeDef TxHeader;
 uint8_t TxData[8];
 uint32_t TxMailbox;

 CAN_RxHeaderTypeDef RxHeader;
 uint8_t RxData[8];

 uint8_t rx_message_flag;
}can_struct_t;

extern can_struct_t myCan;

void ancit_can_start(void);
void ancit_can_init_rx_filter(void);
void ancit_can_check_for_rx(void);
void ancit_can_tx_message(void);
void ancit_can_wait_for_switch_press(void);

#endif /* SRC_CAN_H_ */

