#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "MCMCAN.h"
#include "Bsp.h"

IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent = 0;
extern McmcanType g_mcmcan;

void transmitCustomMessage(uint32 data0, uint32 data1)
{
    g_mcmcan.txData[0] = data0;
    g_mcmcan.txData[1] = data1;
    IfxCan_Can_initMessage(&g_mcmcan.txMsg);
    g_mcmcan.txMsg.messageId = CAN_MESSAGE_ID;
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

    /* --- ATTACK ---
     * Modified "Fuzzing" Attack: Send an incrementing value instead of random.
     * This is deterministic and easy to follow in Busmaster.
     */
    uint32 counter = 0;
    while(1)
    {
        /* Send the counter value as the data payload */
        transmitCustomMessage(counter, ~counter); // Send the counter and its bitwise NOT
        counter++; // Increment for the next message

        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 500));
    }
}
