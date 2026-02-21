/********************************************************************************************************************
 * \file MCMCAN.h
 *********************************************************************************************************************/
#ifndef MCMCAN_H_
#define MCMCAN_H_ 1

#include <stdio.h>
#include <string.h>
#include "Ifx_Types.h"
#include "IfxCan_Can.h"
#include "IfxCan.h"
#include "IfxCpu_Irq.h"
#include "IfxPort.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define CAN_MESSAGE_ID              (uint32)0x77           /* Message ID that will be used in arbitration phase    */
#define ISR_PRIORITY_CAN_TX         2                       /* Define the CAN TX interrupt priority                 */
#define ISR_PRIORITY_CAN_RX         1                       /* Define the CAN RX interrupt priority                 */
#define TX_DATA_LOW_WORD            (uint32)0xC0CAC01A      /* Define CAN data lower word to be transmitted         */
#define TX_DATA_HIGH_WORD           (uint32)0xBA5EBA11      /* Define CAN data higher word to be transmitted        */
#define MAXIMUM_CAN_DATA_PAYLOAD    2                       /* Define maximum classical CAN payload in 4-byte words */

/* *** MODIFICATION ***
 * Correct LED Port and Pin definitions for the TC3X7 Application Kit.
 * User LEDs D107-D110 are on Port 13. We will use D107 for TX and D108 for RX.
 */
#define LED_TX_PIN                  0                       /* Pin 0 on Port 13 is for LED D107 */
#define LED_RX_PIN                  1                       /* Pin 1 on Port 13 is for LED D108 */
#define LED_PORT                    &MODULE_P13             /* The user LEDs are on Port 13 */

/*********************************************************************************************************************/
/*--------------------------------------------------Data Structures--------------------------------------------------*/
/*********************************************************************************************************************/
typedef struct
{
    IfxCan_Can_Config canConfig;
    IfxCan_Can canModule;
    IfxCan_Can_Node canSrcNode;
    IfxCan_Can_Node canDstNode;
    IfxCan_Can_NodeConfig canNodeConfig;
    IfxCan_Filter canFilter;
    IfxCan_Message txMsg;
    IfxCan_Message rxMsg;
    uint32 txData[MAXIMUM_CAN_DATA_PAYLOAD];
    uint32 rxData[MAXIMUM_CAN_DATA_PAYLOAD];
} McmcanType;

/*********************************************************************************************************************/
/*-----------------------------------------------Function Prototypes-------------------------------------------------*/
/*********************************************************************************************************************/
void initMcmcan(void);
void transmitCanMessage(void);
void initLeds(void);

#endif /* MCMCAN_H_ */
