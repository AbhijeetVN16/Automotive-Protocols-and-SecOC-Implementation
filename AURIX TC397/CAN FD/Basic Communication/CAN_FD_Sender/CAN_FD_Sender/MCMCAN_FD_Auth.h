/**********************************************************************************************************************
 * \file MCMCAN_FD_Auth.h
 * \brief CAN-FD Secure Sender – Transmit authenticated SECOC-style messages
 *********************************************************************************************************************/
#ifndef MCMCAN_FD_AUTH_H_
#define MCMCAN_FD_AUTH_H_ 1

#include "Ifx_Types.h"
#include "IfxCan_Can.h"
#include "IfxCan.h"
#include "IfxCpu_Irq.h"
#include "IfxPort.h"

/*---------------------------------- Config Macros ----------------------------------*/
#define CAN_MESSAGE_ID              ((uint32)0x77)
#define MAX_CAN_FD_DATA_BYTES       64

#define SHARED_SECRET_KEY           ((uint32)0x5A5A5A5A)
#define LED_TX                      &MODULE_P13,0

/*---------------------------------- Data Structure ---------------------------------*/
typedef struct
{
    IfxCan_Can_Config      canConfig;
    IfxCan_Can             canModule;
    IfxCan_Can_Node        canNode;
    IfxCan_Can_NodeConfig  canNodeConfig;
    IfxCan_Filter          canFilter;
    IfxCan_Message         txMsg;
    uint8                  txData[MAX_CAN_FD_DATA_BYTES];
} McmcanType;

/*---------------------------------- API Functions ----------------------------------*/
void initMcmcan_FD(void);
void initLeds(void);
void transmitAuthenticatedMessage_FD(uint16 dataPayload);

#endif /* MCMCAN_FD_AUTH_H_ */
