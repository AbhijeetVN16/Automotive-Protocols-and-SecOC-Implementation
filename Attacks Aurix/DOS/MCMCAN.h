#ifndef MCMCAN_H_
#define MCMCAN_H_ 1

#include <stdio.h>
#include <string.h>
#include "Ifx_Types.h"
#include "IfxCan_Can.h"
#include "IfxCan.h"
#include "IfxCpu_Irq.h"
#include "IfxPort.h"

#define ISR_PRIORITY_CAN_TX         2
#define ISR_PRIORITY_CAN_RX         1
#define TX_DATA_LOW_WORD            (uint32)0xC0CAC01A
#define TX_DATA_HIGH_WORD           (uint32)0xBA5EBA11
#define MAXIMUM_CAN_DATA_PAYLOAD    2

#define LED_TX_PIN                  0
#define LED_RX_PIN                  1
#define LED_PORT                    &MODULE_P13

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
    uint32 currentMessageId;  // <--- Added to control message ID dynamically
} McmcanType;

extern McmcanType g_mcmcan;

void initMcmcan(void);
void transmitCanMessage(void);
void initLeds(void);

#endif /* MCMCAN_H_ */
