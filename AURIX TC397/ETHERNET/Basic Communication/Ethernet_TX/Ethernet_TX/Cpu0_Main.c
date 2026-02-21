/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \brief Main file for the CAN-over-Ethernet SENDER application.
 *********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "IfxStm.h"
#include "Ifx_Lwip.h"
#include "IfxPort.h"
#include "Configuration.h"
#include "ConfigurationIsr.h"
#include "CanUdpSender.h" /* Header for our sender application */
#include "lwip/netif.h"   /* Header for lwIP network interface functions */

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
/* This variable MUST be defined here for all cores to sync */
IfxCpu_syncEvent g_cpuSyncEvent = 0;

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
void initLed(void)
{
    IfxPort_setPinModeOutput(LED_TX, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinHigh(LED_TX);
}

/* This is the main entry point for the application, which the linker was looking for */
void core0_main (void)
{
    IfxCpu_enableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    initLed();

    IfxStm_CompareConfig stmCompareConfig;
    IfxStm_initCompareConfig(&stmCompareConfig);
    stmCompareConfig.triggerPriority     = ISR_PRIORITY_OS_TICK;
    stmCompareConfig.comparatorInterrupt = IfxStm_ComparatorInterrupt_ir0;
    stmCompareConfig.ticks               = IFX_CFG_STM_TICKS_PER_MS * 10;
    stmCompareConfig.typeOfService       = IfxSrc_Tos_cpu0;
    IfxStm_initCompare(&MODULE_STM0, &stmCompareConfig);

    IfxGeth_enableModule(&MODULE_GETH);

    eth_addr_t ethAddr;
    ethAddr.addr[0] = 0xDE;
    ethAddr.addr[1] = 0xAD;
    ethAddr.addr[2] = 0xBE;
    ethAddr.addr[3] = 0xEF;
    ethAddr.addr[4] = 0xFE;
    ethAddr.addr[5] = 0xEC; // Sender's unique MAC

    /* Initialize LwIP stack */
    Ifx_Lwip_init(ethAddr);

    /* Set a static IP for the sender */
    ip_addr_t ipaddr, netmask, gw;
    IP4_ADDR(&ipaddr,  192, 168, 1, 10);
    IP4_ADDR(&netmask, 255, 255, 255, 0);
    IP4_ADDR(&gw,      192, 168, 1, 1);

    if (netif_default)
    {
        netif_set_addr(netif_default, &ipaddr, &netmask, &gw);
        netif_set_up(netif_default);
    }

    /* Initialize our UDP application */
    canUdpSenderInit();

    /* Wait for 3 seconds to ensure the network link is fully established */
    for (volatile Ifx_TickTime i = 0; i < 300000000; ++i) { __nop(); }

    while (1)
    {
        Ifx_Lwip_pollTimerFlags();
        Ifx_Lwip_pollReceiveFlags();

        if (netif_is_link_up(netif_default))
        {
            sendUdpPacket();
        }

        /* Wait for 1 second */
        for (volatile Ifx_TickTime i = 0; i < 100000000; ++i) { __nop(); }
    }
}

IFX_INTERRUPT (updateLwIPStackISR, 0, ISR_PRIORITY_OS_TICK);
void updateLwIPStackISR(void)
{
    IfxStm_increaseCompare(&MODULE_STM0, IfxStm_Comparator_0, IFX_CFG_STM_TICKS_PER_MS);
    g_TickCount_1ms++;
    Ifx_Lwip_onTimerTick();
}
