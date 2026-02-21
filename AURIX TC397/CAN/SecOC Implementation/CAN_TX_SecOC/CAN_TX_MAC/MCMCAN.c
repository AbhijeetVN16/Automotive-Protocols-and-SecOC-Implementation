/********************************************************************************************************************
 * \file MCMCAN.c
 * \note SENDER VERSION - Configured for transmitting.
 *********************************************************************************************************************/
#include "MCMCAN.h"

/* Define CAN node pins */
#define TX_PIN                      IfxCan_TXD00_P20_8_OUT
#define RX_PIN                      IfxCan_RXD00B_P20_7_IN /* RX pin is still needed for CAN ACK */

McmcanType g_mcmcan; /* Global CAN object */

/**
 * @brief Initializes the TX LED.
 */
void initLeds(void)
{
    IfxPort_setPinModeOutput(LED_TX, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    /* Start with LED OFF (assuming high-level active for simplicity, adjust if needed) */
    IfxPort_setPinLow(LED_TX);
}

/**
 * @brief Initializes the MCMCAN module and node for TRANSMITTING.
 */
void initMcmcan(void)
{
    IfxCan_Can_initModuleConfig(&g_mcmcan.canConfig, &MODULE_CAN0);
    IfxCan_Can_initModule(&g_mcmcan.canModule, &g_mcmcan.canConfig);

    IfxCan_Can_initNodeConfig(&g_mcmcan.canNodeConfig, &g_mcmcan.canModule);
    g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_0; /* Using Node 0 */
    g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_transmit; /* MODIFIED: Set frame type to transmit */

    /* Pin configuration */
    IfxCan_Can_Pins pins;
    pins.rxPin = &RX_PIN;
    pins.rxPinMode = IfxPort_InputMode_pullUp;
    pins.txPin = &TX_PIN;
    pins.txPinMode = IfxPort_OutputMode_pushPull;
    pins.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed2;
    g_mcmcan.canNodeConfig.pins = &pins;

    /* No receive interrupts are needed for the sender */
    g_mcmcan.canNodeConfig.interruptConfig.messageStoredToDedicatedRxBufferEnabled = FALSE;

    IfxCan_Can_initNode(&g_mcmcan.canNode, &g_mcmcan.canNodeConfig);
}
