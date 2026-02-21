/********************************************************************************************************************
 * \file MCMCAN.c
 * \note Final Receiver Version - Verifies MAC and Freshness Value.
 *********************************************************************************************************************/
#include "MCMCAN.h"

#define TX_PIN                      IfxCan_TXD00_P20_8_OUT
#define RX_PIN                      IfxCan_RXD00B_P20_7_IN

/* Global CAN object */
McmcanType g_mcmcan;

/* Stores the counter of the last successfully authenticated message */
static uint16 g_last_valid_freshness = 0;

/* Interrupt Service Routine (ISR) for receiving CAN messages */
IFX_INTERRUPT(canIsrRxHandler, 0, ISR_PRIORITY_CAN_RX);

void canIsrRxHandler(void)
{
    /* Clear the interrupt flag for the specific node */
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canNode.node, IfxCan_Interrupt_messageStoredToDedicatedRxBuffer);

    /* Read the CAN message from the hardware buffer into our struct */
    IfxCan_Can_readMessage(&g_mcmcan.canNode, &g_mcmcan.rxMsg, g_mcmcan.rxData);

    /* 1. Extract payload from the received message */
    uint32 receivedPackedData = g_mcmcan.rxData[0];
    uint32 receivedMac  = g_mcmcan.rxData[1];

    /* 2. FIRST: Verify the integrity by recalculating the MAC */
    uint32 calculatedMac = receivedPackedData ^ SHARED_SECRET_KEY;

    if (calculatedMac != receivedMac)
    {
        /* MAC check FAILED. The message is corrupt or not authentic. */
        IfxPort_togglePin(LED_RX_ERROR);
        return; /* Stop processing immediately */
    }

    /* --- MAC check PASSED. Now check for freshness to prevent replay attacks. --- */

    /* 3. Unpack the freshness value from the (now trusted) payload */
    uint16 receivedFreshness = (uint16)(receivedPackedData & 0xFFFF);

    /* 4. SECOND: Verify the message is new by checking the counter */
    if (receivedFreshness > g_last_valid_freshness)
    {
        /* SUCCESS! The message is authentic AND new. */
        g_last_valid_freshness = receivedFreshness; /* Update to the new highest counter */
        IfxPort_togglePin(LED_RX_SUCCESS);

        /* You can now safely unpack and use the actual data if needed */
        /* uint16 actual_data = (uint16)(receivedPackedData >> 16); */
    }
    else
    {
        /* FRESHNESS check FAILED. This is a stale message or a replay attack. */
        IfxPort_togglePin(LED_RX_ERROR);
    }
}

/**
 * @brief Initializes the RX Success and Error LEDs.
 */
void initLeds(void)
{
    /* Initialize the SUCCESS LED pin */
    IfxPort_setPinModeOutput(LED_RX_SUCCESS, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinLow(LED_RX_SUCCESS); /* Start with LED OFF */

    /* Initialize the ERROR LED pin */
    IfxPort_setPinModeOutput(LED_RX_ERROR, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinLow(LED_RX_ERROR); /* Start with LED OFF */
}

/**
 * @brief Initializes the MCMCAN module and node for RECEIVING.
 */
void initMcmcan(void)
{
    /* Disable interrupts during initialization */
    boolean iState = IfxCpu_disableInterrupts();

    /* Initialize the CAN module */
    IfxCan_Can_initModuleConfig(&g_mcmcan.canConfig, &MODULE_CAN0);
    IfxCan_Can_initModule(&g_mcmcan.canModule, &g_mcmcan.canConfig);

    /* Initialize the CAN node */
    IfxCan_Can_initNodeConfig(&g_mcmcan.canNodeConfig, &g_mcmcan.canModule);
    g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_0;
    g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_receive; /* Set frame type to receive */

    /* Configure physical pins */
    IfxCan_Can_Pins pins;
    pins.rxPin = &RX_PIN;
    pins.rxPinMode = IfxPort_InputMode_pullUp;
    pins.txPin = &TX_PIN;
    pins.txPinMode = IfxPort_OutputMode_pushPull;
    pins.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed2;
    g_mcmcan.canNodeConfig.pins = &pins;

    /* Configure interrupt for message reception */
    g_mcmcan.canNodeConfig.interruptConfig.messageStoredToDedicatedRxBufferEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.reint.priority = ISR_PRIORITY_CAN_RX;
    g_mcmcan.canNodeConfig.interruptConfig.reint.interruptLine = IfxCan_InterruptLine_1;
    g_mcmcan.canNodeConfig.interruptConfig.reint.typeOfService = IfxSrc_Tos_cpu0;

    IfxCan_Can_initNode(&g_mcmcan.canNode, &g_mcmcan.canNodeConfig);

    /* Configure a filter to only accept messages with our specific ID */
    g_mcmcan.canFilter.number = 0;
    g_mcmcan.canFilter.elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxBuffer;
    g_mcmcan.canFilter.id1 = CAN_MESSAGE_ID;
    g_mcmcan.canFilter.rxBufferOffset = IfxCan_RxBufferId_0;
    IfxCan_Can_setStandardFilter(&g_mcmcan.canNode, &g_mcmcan.canFilter);

    /* Restore interrupts */
    IfxCpu_restoreInterrupts(iState);
}
