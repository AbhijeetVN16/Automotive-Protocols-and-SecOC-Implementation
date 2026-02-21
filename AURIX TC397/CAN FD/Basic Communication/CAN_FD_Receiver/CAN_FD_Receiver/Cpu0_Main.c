/********************************************************************************************************************
 * \file core0_main.c
 * \brief Receiver core for authenticated CAN-FD communication (MAC + Freshness)
 *********************************************************************************************************************/

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "MCMCAN.h"

IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent = 0;

void core0_main(void)
{
    IfxCpu_enableInterrupts();

    /* Disable watchdogs (CPU + Safety) */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    /* CPU synchronisation */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    /* Init CAN + LEDs */
    initMcmcan();
    initLeds();

    while (1)
    {
        /* Idle loop; all processing done in ISR */
    }
}
