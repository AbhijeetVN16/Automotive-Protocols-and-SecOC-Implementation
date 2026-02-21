/**********************************************************************************************************************
 * \file CanUdpSender.c
 * \brief This file implements the UDP sender application.
 *********************************************************************************************************************/
#include "CanUdpSender.h"
#include "lwip/udp.h"
#include "lwip/pbuf.h"

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
/* The UDP protocol control block is defined ONLY here */
struct udp_pcb *g_udp_pcb = NULL;

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
/* The function definition is ONLY here */
void canUdpSenderInit(void)
{
    g_udp_pcb = udp_new();
}

/* The function definition is ONLY here */
void sendUdpPacket(void)
{
    if (g_udp_pcb == NULL) { return; }

    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, 8, PBUF_RAM);
    if (p == NULL) { return; }

    uint32 can_payload[2];
    can_payload[0] = EXPECTED_DATA_LOW_WORD;
    can_payload[1] = EXPECTED_DATA_HIGH_WORD;

    if (pbuf_take(p, can_payload, 8) == ERR_OK)
    {
        ip_addr_t receiver_ip;
        IP4_ADDR(&receiver_ip, RECEIVER_IP_BYTE_1, RECEIVER_IP_BYTE_2, RECEIVER_IP_BYTE_3, RECEIVER_IP_BYTE_4);
        udp_sendto(g_udp_pcb, p, &receiver_ip, CAN_UDP_PORT);
        IfxPort_togglePin(LED_TX);
    }
    pbuf_free(p);
}
