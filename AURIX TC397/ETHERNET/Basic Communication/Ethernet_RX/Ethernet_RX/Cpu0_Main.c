/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \brief Main file for the CAN-over-Ethernet RECEIVER application.
 *********************************************************************************************************************/

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "IfxStm.h"
#include "IfxGeth_Eth.h"
#include "Ifx_Lwip.h"
#include "IfxPort.h"
#include "Configuration.h"
#include "ConfigurationIsr.h"
#include "CanUdpReceiver.h" /* We include our new application header */
#include "lwip/netif.h"      /* Header for network interface functions, including netif_default */

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
IfxCpu_syncEvent g_cpuSyncEvent = 0;

/* The 'extern struct netif g_netif' is REMOVED as it was causing the linker error.
 * We will use the standard 'netif_default' pointer from lwIP instead.
 */

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/

/** \brief Initializes the LED used for indicating received packets. */
void initLed(void)
{
    /* Initialize the LED port pin to push-pull output mode */
    IfxPort_setPinModeOutput(LED_RX, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    /* Turn OFF the LED by default */
    IfxPort_setPinHigh(LED_RX);
}

void core0_main (void)
{
    /* Enable the global interrupts of this CPU */
    IfxCpu_enableInterrupts();

    /* Disable watchdogs */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    /* Wait for all CPUs to be ready */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    /* Initialize the LED */
    initLed();

    /* -- STM and GETH setup is from your reference project -- */
    IfxStm_CompareConfig stmCompareConfig;
    IfxStm_initCompareConfig(&stmCompareConfig);

    stmCompareConfig.triggerPriority     = ISR_PRIORITY_OS_TICK;
    stmCompareConfig.comparatorInterrupt = IfxStm_ComparatorInterrupt_ir0;
    stmCompareConfig.ticks               = IFX_CFG_STM_TICKS_PER_MS * 10;
    stmCompareConfig.typeOfService       = IfxSrc_Tos_cpu0;

    IfxStm_initCompare(&MODULE_STM0, &stmCompareConfig);

    IfxGeth_enableModule(&MODULE_GETH);

    /* Define a unique MAC Address for the RECEIVER board */
    eth_addr_t ethAddr;
    ethAddr.addr[0] = 0xDE;
    ethAddr.addr[1] = 0xAD;
    ethAddr.addr[2] = 0xBE;
    ethAddr.addr[3] = 0xEF;
    ethAddr.addr[4] = 0xFE;
    ethAddr.addr[5] = 0xED; // This is the MAC for the receiver

    /* --- CORRECTED STATIC IP INITIALIZATION --- */
    /* Call the standard init function from your library. This sets up the default network interface. */
    Ifx_Lwip_init(ethAddr);

    /* Now, we explicitly set the static IP address on the default network interface. */
    ip_addr_t ipaddr, netmask, gw;
    IP4_ADDR(&ipaddr,  192, 168, 1, 20); // The receiver's static IP
    IP4_ADDR(&netmask, 255, 255, 255, 0);
    IP4_ADDR(&gw,      192, 168, 1, 1);   // Your network's gateway

    /* Use the standard lwIP global pointer 'netif_default' to set the address */
    if (netif_default)
    {
        netif_set_addr(netif_default, &ipaddr, &netmask, &gw);
        netif_set_up(netif_default); /* Ensure the network interface is marked as up */
    }

    /* Initialize our UDP Receiver application */
    canUdpReceiverInit();

    while (1)
    {
        /* Poll LwIP timers and receive flags continuously */
        Ifx_Lwip_pollTimerFlags();
        Ifx_Lwip_pollReceiveFlags();
    }
}

/* ISR to update LwIP stack (from your reference project, unchanged) */
IFX_INTERRUPT (updateLwIPStackISR, 0, ISR_PRIORITY_OS_TICK);
void updateLwIPStackISR(void)
{
    IfxStm_increaseCompare(&MODULE_STM0, IfxStm_Comparator_0, IFX_CFG_STM_TICKS_PER_MS);
    g_TickCount_1ms++;
    Ifx_Lwip_onTimerTick();
}
