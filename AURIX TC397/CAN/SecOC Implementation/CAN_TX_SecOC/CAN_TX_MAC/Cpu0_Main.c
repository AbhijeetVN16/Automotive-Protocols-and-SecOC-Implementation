#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "MCMCAN.h"
#include "Bsp.h"

IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent = 0;
extern McmcanType g_mcmcan;

// NEW: A counter to ensure freshness of each message
static uint16 g_freshness_counter = 0;

static uint32 calculateMac(uint32 data, uint32 key)
{
    return data ^ key;
}

// MODIFIED: Now sends 16-bit data with a freshness value
void transmitAuthenticatedMessage(uint16 dataPayload)
{
    // 1. Increment the freshness counter for this new message
    g_freshness_counter++;

    // 2. Pack the 16-bit data and 16-bit freshness value into one 32-bit word
    uint32 packedData = ((uint32)dataPayload << 16) | g_freshness_counter;

    // 3. Calculate the MAC over the entire packed data word
    uint32 mac = calculateMac(packedData, SHARED_SECRET_KEY);

    // 4. Prepare the CAN message payload
    g_mcmcan.txData[0] = packedData; // Word 0: Data + Freshness
    g_mcmcan.txData[1] = mac;       // Word 1: MAC

    // 5. Initialize and send the message
    IfxCan_Can_initMessage(&g_mcmcan.txMsg);
    g_mcmcan.txMsg.messageId = CAN_MESSAGE_ID;

    while(IfxCan_Status_notSentBusy == IfxCan_Can_sendMessage(&g_mcmcan.canNode, &g_mcmcan.txMsg, &g_mcmcan.txData[0]))
    {
    }

    IfxPort_togglePin(LED_TX);
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

    uint16 legitimateData = 0xC0CA; // Example 16-bit data

    while(1)
    {
        transmitAuthenticatedMessage(legitimateData);
        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 1000));
    }
}
