/********************************************************************************************************************
 * \file MCMCAN.c
 * \note RECEIVER VERSION - Corrected for onboard LEDs
 *********************************************************************************************************************/
#include "MCMCAN.h"

#define TX_PIN                      IfxCan_TXD00_P20_8_OUT
#define RX_PIN                      IfxCan_RXD00B_P20_7_IN

McmcanType g_mcmcan;
IfxPort_Pin_Config g_led2;

IFX_INTERRUPT(canIsrRxHandler, 0, ISR_PRIORITY_CAN_RX);

void canIsrRxHandler(void)
{
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canDstNode.node, IfxCan_Interrupt_messageStoredToDedicatedRxBuffer);
    IfxCan_Can_readMessage(&g_mcmcan.canDstNode, &g_mcmcan.rxMsg, g_mcmcan.rxData);

    const uint32 expectedDataLow = TX_DATA_LOW_WORD;
    const uint32 expectedDataHigh = TX_DATA_HIGH_WORD;

    if( (g_mcmcan.rxData[0] == expectedDataLow) && (g_mcmcan.rxData[1] == expectedDataHigh) )
    {
        IfxPort_togglePin(g_led2.port, g_led2.pinIndex);
    }
}

void initMcmcan(void)
{
    boolean iState = IfxCpu_disableInterrupts();
    IfxCan_Can_initModuleConfig(&g_mcmcan.canConfig, &MODULE_CAN0);
    IfxCan_Can_initModule(&g_mcmcan.canModule, &g_mcmcan.canConfig);
    IfxCan_Can_initNodeConfig(&g_mcmcan.canNodeConfig, &g_mcmcan.canModule);
    g_mcmcan.canNodeConfig.busLoopbackEnabled = FALSE;
    g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_0;
    g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_receive;
    IfxCan_Can_Pins pins;
    pins.rxPin = &RX_PIN;
    pins.rxPinMode = IfxPort_InputMode_pullUp;
    pins.txPin = &TX_PIN;
    pins.txPinMode = IfxPort_OutputMode_pushPull;
    pins.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed2;
    g_mcmcan.canNodeConfig.pins = &pins;
    g_mcmcan.canNodeConfig.interruptConfig.messageStoredToDedicatedRxBufferEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.reint.priority = ISR_PRIORITY_CAN_RX;
    g_mcmcan.canNodeConfig.interruptConfig.reint.interruptLine = IfxCan_InterruptLine_1;
    g_mcmcan.canNodeConfig.interruptConfig.reint.typeOfService = IfxSrc_Tos_cpu0;
    IfxCan_Can_initNode(&g_mcmcan.canDstNode, &g_mcmcan.canNodeConfig);
    g_mcmcan.canFilter.number = 0;
    g_mcmcan.canFilter.elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxBuffer;
    g_mcmcan.canFilter.id1 = CAN_MESSAGE_ID;
    g_mcmcan.canFilter.rxBufferOffset = IfxCan_RxBufferId_0;
    IfxCan_Can_setStandardFilter(&g_mcmcan.canDstNode, &g_mcmcan.canFilter);
    IfxCpu_restoreInterrupts(iState);
}

void initLeds(void)
{
    /* *** MODIFICATION ***
     * Configure the correct port and pin for the onboard RX LED (D108).
     * The LEDs on this board are low-active, meaning HIGH = OFF, LOW = ON.
     */
    g_led2.port      = LED_PORT;      /* From MCMCAN.h: &MODULE_P13 */
    g_led2.pinIndex  = LED_RX_PIN;    /* From MCMCAN.h: 1 */
    g_led2.mode      = IfxPort_OutputIdx_general;
    g_led2.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;

    /* Initialize the pin to HIGH to keep the LED turned OFF by default. */
    IfxPort_setPinHigh(g_led2.port, g_led2.pinIndex);
    /* Apply the configuration to the hardware registers. */
    IfxPort_setPinModeOutput(g_led2.port, g_led2.pinIndex, IfxPort_OutputMode_pushPull, g_led2.mode);
    IfxPort_setPinPadDriver(g_led2.port, g_led2.pinIndex, g_led2.padDriver);
}
