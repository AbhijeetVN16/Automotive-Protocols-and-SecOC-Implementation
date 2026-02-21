/********************************************************************************************************************
 * \file MCMCAN.h
 * \note Receiver side for authenticated CAN-FD (MAC + Freshness)
 *********************************************************************************************************************/

#ifndef MCMCAN_H_
#define MCMCAN_H_ 1

#include "Ifx_Types.h"
#include "IfxCan_Can.h"
#include "IfxCan.h"
#include "IfxCpu_Irq.h"
#include "IfxPort.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define CAN_MESSAGE_ID              (uint32)0x77
#define MAXIMUM_CAN_DATA_PAYLOAD    2        /* 8 bytes total */

/* Payload:
 * Word0 = [16-bit DATA | 16-bit FRESHNESS]
 * Word1 = [32-bit MAC]
 */

/* Shared key for XOR-based MAC */
#define SHARED_SECRET_KEY           (uint32)0x5A5A5A5A

/* Interrupt priorities */
#define ISR_PRIORITY_CAN_TX         2
#define ISR_PRIORITY_CAN_RX         1

/* LED mapping */
#define LED_TX                      &MODULE_P13,0
#define LED_RX_SUCCESS              &MODULE_P13,1
#define LED_RX_ERROR                &MODULE_P13,2

/* Fallback for older iLLD missing this define */
#ifndef IfxCan_DataLengthCode_8
#define IfxCan_DataLengthCode_8 8
#endif

/*********************************************************************************************************************/
/*--------------------------------------------------Data Structures--------------------------------------------------*/
/*********************************************************************************************************************/
typedef struct
{
    IfxCan_Can_Config     canConfig;
    IfxCan_Can            canModule;
    IfxCan_Can_Node       canNode;
    IfxCan_Can_NodeConfig canNodeConfig;
    IfxCan_Filter         canFilter;
    IfxCan_Message        txMsg;
    IfxCan_Message        rxMsg;
    uint32                txData[MAXIMUM_CAN_DATA_PAYLOAD];
    uint32                rxData[MAXIMUM_CAN_DATA_PAYLOAD];
} McmcanType;

/*********************************************************************************************************************/
/*-----------------------------------------------Function Prototypes-------------------------------------------------*/
/*********************************************************************************************************************/
void initMcmcan(void);
void initLeds(void);

#endif /* MCMCAN_H_ */
