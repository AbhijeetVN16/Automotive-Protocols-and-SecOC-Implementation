/********************************************************************************************************************
 * \file MCMCAN.c
 * \note Receiver side for authenticated CAN-FD (MAC + Freshness Counter)
 *********************************************************************************************************************/

#include "MCMCAN.h"
#include <string.h>     /* for memset / memcpy */

#define TX_PIN  IfxCan_TXD00_P20_8_OUT
#define RX_PIN  IfxCan_RXD00B_P20_7_IN

McmcanType g_mcmcan;

/* Track last accepted freshness counter */
static uint16 g_last_valid_freshness = 0;

/* ------------------------- ISR ------------------------- */
IFX_INTERRUPT(canIsrRxHandler, 0, ISR_PRIORITY_CAN_RX);

void canIsrRxHandler(void)
{
    /* Clear RX interrupt flag */
    IfxCan_Node_clearInterruptFlag(
        g_mcmcan.canNode.node,
        IfxCan_Interrupt_messageStoredToDedicatedRxBuffer);

    /* Read message from RX buffer */
    IfxCan_Can_readMessage(&g_mcmcan.canNode, &g_mcmcan.rxMsg, g_mcmcan.rxData);

    uint32 receivedPackedData = g_mcmcan.rxData[0];
    uint32 receivedMac        = g_mcmcan.rxData[1];
    uint32 calculatedMac      = receivedPackedData ^ SHARED_SECRET_KEY;

    /* Step 1: verify MAC */
    if (calculatedMac != receivedMac)
    {
        IfxPort_togglePin(LED_RX_ERROR);
        return;
    }

    /* Step 2: check freshness */
    uint16 receivedFreshness = (uint16)(receivedPackedData & 0xFFFF);

    if (receivedFreshness > g_last_valid_freshness)
    {
        g_last_valid_freshness = receivedFreshness;
        IfxPort_togglePin(LED_RX_SUCCESS);

        /* Optional: extract 16-bit data payload */
        /* uint16 dataPayload = (uint16)(receivedPackedData >> 16); */
    }
    else
    {
        /* Replay or stale frame */
        IfxPort_togglePin(LED_RX_ERROR);
    }
}

/* ----------------------- LED Init ---------------------- */
void initLeds(void)
{
    IfxPort_setPinModeOutput(LED_RX_SUCCESS, IfxPort_OutputMode_pushPull,
                              IfxPort_OutputIdx_general);
    IfxPort_setPinLow(LED_RX_SUCCESS);

    IfxPort_setPinModeOutput(LED_RX_ERROR, IfxPort_OutputMode_pushPull,
                              IfxPort_OutputIdx_general);
    IfxPort_setPinLow(LED_RX_ERROR);
}

/* ----------------------- CAN Init ---------------------- */
void initMcmcan(void)
{
    boolean iState = IfxCpu_disableInterrupts();

    /* Module init */
    IfxCan_Can_initModuleConfig(&g_mcmcan.canConfig, &MODULE_CAN0);
    IfxCan_Can_initModule(&g_mcmcan.canModule, &g_mcmcan.canConfig);

    /* Node config */
    IfxCan_Can_initNodeConfig(&g_mcmcan.canNodeConfig, &g_mcmcan.canModule);
    g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_0;
    g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_receive;
    g_mcmcan.canNodeConfig.frame.mode = IfxCan_FrameMode_fdLong; /* Enable CAN-FD */

    /* Pin config */
    IfxCan_Can_Pins pins;
    pins.rxPin      = &RX_PIN;
    pins.rxPinMode  = IfxPort_InputMode_pullUp;
    pins.txPin      = &TX_PIN;
    pins.txPinMode  = IfxPort_OutputMode_pushPull;
    pins.padDriver  = IfxPort_PadDriver_cmosAutomotiveSpeed2;
    g_mcmcan.canNodeConfig.pins = &pins;

    /* Interrupt config */
    g_mcmcan.canNodeConfig.interruptConfig.messageStoredToDedicatedRxBufferEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.reint.priority       = ISR_PRIORITY_CAN_RX;
    g_mcmcan.canNodeConfig.interruptConfig.reint.interruptLine  = IfxCan_InterruptLine_1;
    g_mcmcan.canNodeConfig.interruptConfig.reint.typeOfService  = IfxSrc_Tos_cpu0;

    IfxCan_Can_initNode(&g_mcmcan.canNode, &g_mcmcan.canNodeConfig);

    /* Acceptance filter */
    g_mcmcan.canFilter.number                = 0;
    g_mcmcan.canFilter.elementConfiguration  = IfxCan_FilterElementConfiguration_storeInRxBuffer;
    g_mcmcan.canFilter.id1                   = CAN_MESSAGE_ID;
    g_mcmcan.canFilter.rxBufferOffset        = IfxCan_RxBufferId_0;
    IfxCan_Can_setStandardFilter(&g_mcmcan.canNode, &g_mcmcan.canFilter);

    /* Message configuration (for CAN-FD) */
    IfxCan_Can_initMessage(&g_mcmcan.rxMsg);
    g_mcmcan.rxMsg.messageId       = CAN_MESSAGE_ID;
    g_mcmcan.rxMsg.frameMode       = IfxCan_FrameMode_fdLong;
    g_mcmcan.rxMsg.dataLengthCode  = IfxCan_DataLengthCode_8; /* 8-byte payload */

    IfxCpu_restoreInterrupts(iState);
}
