/********************************************************************************************************************
 * \file MCMCAN.h
 * \note Final version for MAC and Freshness Value.
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
#define CAN_MESSAGE_ID              (uint32)0x77           /* Message ID for communication */
#define MAXIMUM_CAN_DATA_PAYLOAD    2                       /* Using 2 words (8 bytes) */

/* --- PAYLOAD STRUCTURE ---
 * Word 0: [ 16-bit DATA | 16-bit FRESHNESS COUNTER ]
 * Word 1: [ 32-bit MAC ]
 */

/* --- Security Key --- */
#define SHARED_SECRET_KEY           (uint32)0x5A5A5A5A      /* Shared secret key for MAC generation */

/* --- Interrupt Priorities --- */
#define ISR_PRIORITY_CAN_TX         2                       /* CAN TX interrupt priority */
#define ISR_PRIORITY_CAN_RX         1                       /* CAN RX interrupt priority */

/* --- LED Definitions --- */
#define LED_TX                      &MODULE_P13,0           /* LED D107 (Port 13, Pin 0) for Transmit */
#define LED_RX_SUCCESS              &MODULE_P13,1           /* LED D108 (Port 13, Pin 1) for Successful Auth */
#define LED_RX_ERROR                &MODULE_P13,2           /* LED D109 (Port 13, Pin 2) for Auth Error */

/*********************************************************************************************************************/
/*--------------------------------------------------Data Structures--------------------------------------------------*/
/*********************************************************************************************************************/
typedef struct
{
    IfxCan_Can_Config   canConfig;
    IfxCan_Can          canModule;
    IfxCan_Can_Node     canNode;        /* Unified node for TX or RX */
    IfxCan_Can_NodeConfig canNodeConfig;
    IfxCan_Filter       canFilter;
    IfxCan_Message      txMsg;
    IfxCan_Message      rxMsg;
    uint32              txData[MAXIMUM_CAN_DATA_PAYLOAD];
    uint32              rxData[MAXIMUM_CAN_DATA_PAYLOAD];
} McmcanType;

/*********************************************************************************************************************/
/*-----------------------------------------------Function Prototypes-------------------------------------------------*/
/*********************************************************************************************************************/
void initMcmcan(void);
void initLeds(void);

#endif /* MCMCAN_H_ */
