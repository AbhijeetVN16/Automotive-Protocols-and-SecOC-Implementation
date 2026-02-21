/********************************************************************************************************************
 * \file MCMCAN.c
 * \note SENDER VERSION - Corrected for onboard LEDs
 *********************************************************************************************************************/
#include "MCMCAN.h"

#define TX_PIN                      IfxCan_TXD00_P20_8_OUT
#define RX_PIN                      IfxCan_RXD00B_P20_7_IN

McmcanType g_mcmcan;
IfxPort_Pin_Config g_led1;

IFX_INTERRUPT(canIsrTxHandler, 0, ISR_PRIORITY_CAN_TX);

void canIsrTxHandler(void)
{
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canSrcNode.node, IfxCan_Interrupt_transmissionCompleted);
    IfxPort_togglePin(g_led1.port, g_led1.pinIndex); /* Toggle LED1 on successful transmission */
}

void initMcmcan(void)
{
    boolean iState = IfxCpu_disableInterrupts();
    IfxCan_Can_initModuleConfig(&g_mcmcan.canConfig, &MODULE_CAN0);
    IfxCan_Can_initModule(&g_mcmcan.canModule, &g_mcmcan.canConfig);
    IfxCan_Can_initNodeConfig(&g_mcmcan.canNodeConfig, &g_mcmcan.canModule);
    g_mcmcan.canNodeConfig.busLoopbackEnabled = FALSE;
    g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_0;
    g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_transmit;
    IfxCan_Can_Pins pins;
    pins.rxPin = &RX_PIN;
    pins.rxPinMode = IfxPort_InputMode_pullUp;
    pins.txPin = &TX_PIN;
    pins.txPinMode = IfxPort_OutputMode_pushPull;
    pins.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed2;
    g_mcmcan.canNodeConfig.pins = &pins;
    g_mcmcan.canNodeConfig.interruptConfig.transmissionCompletedEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.traco.priority = ISR_PRIORITY_CAN_TX;
    g_mcmcan.canNodeConfig.interruptConfig.traco.interruptLine = IfxCan_InterruptLine_0;
    g_mcmcan.canNodeConfig.interruptConfig.traco.typeOfService = IfxSrc_Tos_cpu0;
    IfxCan_Can_initNode(&g_mcmcan.canSrcNode, &g_mcmcan.canNodeConfig);
    IfxCpu_restoreInterrupts(iState);
}

void transmitCanMessage(void)
{
    IfxCan_Can_initMessage(&g_mcmcan.txMsg);
    g_mcmcan.txData[0] = TX_DATA_LOW_WORD;
    g_mcmcan.txData[1] = TX_DATA_HIGH_WORD;
    g_mcmcan.txMsg.messageId = CAN_MESSAGE_ID;
    while( IfxCan_Status_notSentBusy ==
           IfxCan_Can_sendMessage(&g_mcmcan.canSrcNode, &g_mcmcan.txMsg, &g_mcmcan.txData[0]) )
    {
    }
}

void initLeds(void)
{
    /* *** MODIFICATION ***
     * Configure the correct port and pin for the onboard TX LED (D107).
     * The LEDs on this board are low-active, meaning HIGH = OFF, LOW = ON.
     */
    g_led1.port      = LED_PORT;      /* From MCMCAN.h: &MODULE_P13 */
    g_led1.pinIndex  = LED_TX_PIN;    /* From MCMCAN.h: 0 */
    g_led1.mode      = IfxPort_OutputIdx_general;
    g_led1.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;

    /* Initialize the pin to HIGH to keep the LED turned OFF by default. */
    IfxPort_setPinHigh(g_led1.port, g_led1.pinIndex);
    /* Apply the configuration to the hardware registers. */
    IfxPort_setPinModeOutput(g_led1.port, g_led1.pinIndex, IfxPort_OutputMode_pushPull, g_led1.mode);
    IfxPort_setPinPadDriver(g_led1.port, g_led1.pinIndex, g_led1.padDriver);
}
