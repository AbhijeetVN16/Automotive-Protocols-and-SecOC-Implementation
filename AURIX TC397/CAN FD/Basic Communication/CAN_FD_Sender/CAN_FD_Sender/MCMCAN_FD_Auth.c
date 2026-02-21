/**********************************************************************************************************************
 * \file MCMCAN_FD_Auth.c
 * \brief CAN-FD Secure Sender – Implementation
 *********************************************************************************************************************/
#include "MCMCAN_FD_Auth.h"
#include <string.h>

/* Define CAN pins (adjust to your board) */
#define TX_PIN  IfxCan_TXD00_P20_8_OUT
#define RX_PIN  IfxCan_RXD00B_P20_7_IN   /* Still required for ACKs */

McmcanType g_mcmcan;
static uint16 g_freshness_counter = 0;

/*---------------------------------- Simple XOR MAC ----------------------------------*/
static uint32 calculateMac(uint32 data, uint32 key)
{
    return data ^ key;
}

/*---------------------------------- LED Init ----------------------------------*/
void initLeds(void)
{
    IfxPort_setPinModeOutput(LED_TX, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinLow(LED_TX);
}

/*---------------------------------- CAN-FD Init ----------------------------------*/
void initMcmcan_FD(void)
{
    /* === Module Config === */
    IfxCan_Can_initModuleConfig(&g_mcmcan.canConfig, &MODULE_CAN0);
    IfxCan_Can_initModule(&g_mcmcan.canModule, &g_mcmcan.canConfig);

    /* === Node Config === */
    IfxCan_Can_initNodeConfig(&g_mcmcan.canNodeConfig, &g_mcmcan.canModule);

    g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_0;
    g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_transmit;
    g_mcmcan.canNodeConfig.frame.mode = IfxCan_FrameMode_fdLongAndFast;   /* CAN-FD with BRS */
    g_mcmcan.canNodeConfig.txConfig.txMode = IfxCan_TxMode_dedicatedBuffers;
    g_mcmcan.canNodeConfig.txConfig.txBufferDataFieldSize = IfxCan_DataFieldSize_64;

    /* === Pins === */
    static IfxCan_Can_Pins pins;
    pins.rxPin = &RX_PIN;
    pins.rxPinMode = IfxPort_InputMode_pullUp;
    pins.txPin = &TX_PIN;
    pins.txPinMode = IfxPort_OutputMode_pushPull;
    pins.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed2;
    g_mcmcan.canNodeConfig.pins = &pins;

    /* === Init Node === */
    IfxCan_Can_initNode(&g_mcmcan.canNode, &g_mcmcan.canNodeConfig);

    /* === Prepare Tx Message === */
    IfxCan_Can_initMessage(&g_mcmcan.txMsg);
    g_mcmcan.txMsg.messageId = CAN_MESSAGE_ID;
    g_mcmcan.txMsg.frameMode = IfxCan_FrameMode_fdLongAndFast;
    g_mcmcan.txMsg.messageIdLength = IfxCan_MessageIdLength_standard;
    g_mcmcan.txMsg.dataLengthCode = IfxCan_DataLengthCode_8; /* Using 8 bytes payload */
}

/*---------------------------------- Authenticated Transmit ----------------------------------*/
void transmitAuthenticatedMessage_FD(uint16 dataPayload)
{
    g_freshness_counter++;

    /* Compose 32-bit packet: [DATA(16) | FRESHNESS(16)] */
    uint32 packedData = ((uint32)dataPayload << 16) | g_freshness_counter;
    uint32 mac = calculateMac(packedData, SHARED_SECRET_KEY);

    /* Fill CAN-FD payload (8 bytes total here, but could use up to 64) */
    memset(g_mcmcan.txData, 0, MAX_CAN_FD_DATA_BYTES);
    memcpy(&g_mcmcan.txData[0], &packedData, sizeof(packedData));
    memcpy(&g_mcmcan.txData[4], &mac, sizeof(mac));

    /* Send message */
    while(IfxCan_Status_notSentBusy ==
          IfxCan_Can_sendMessage(&g_mcmcan.canNode, &g_mcmcan.txMsg, (uint32*)g_mcmcan.txData))
    {
    }

    IfxPort_togglePin(LED_TX);
}
