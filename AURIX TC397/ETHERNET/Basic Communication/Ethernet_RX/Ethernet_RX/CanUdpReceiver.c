/**********************************************************************************************************************
 * \file CanUdpReceiver.c
 * \brief This file implements the UDP receiver application.
 *********************************************************************************************************************/

#include "CanUdpReceiver.h"
#include "lwip/udp.h"
#include "lwip/pbuf.h"

/*********************************************************************************************************************/
/*---------------------------------------------Function Prototypes---------------------------------------------------*/
/*********************************************************************************************************************/
void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/

/**
 * \brief This is the callback function that lwIP calls when a UDP packet is received.
 */
void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    /* If a packet has been received */
    if (p != NULL)
    {
        /* Check if the received packet has the expected length (8 bytes) */
        if (p->len == 8 && p->tot_len == 8)
        {
            uint32 received_data[2];
            /* Copy the payload from the packet buffer into our data array */
            pbuf_copy_partial(p, received_data, p->tot_len, 0);

            /* Check if the received data matches the expected CAN message payload */
            if (received_data[0] == EXPECTED_DATA_LOW_WORD && received_data[1] == EXPECTED_DATA_HIGH_WORD)
            {
                /* If data matches, toggle the LED */
                IfxPort_togglePin(LED_RX);
            }
        }
        /* Free the packet buffer to release memory */
        pbuf_free(p);
    }
}

/**
 * \brief Initializes the UDP receiver.
 */
void canUdpReceiverInit(void)
{
    /* Create a new UDP protocol control block */
    struct udp_pcb *udp_pcb = udp_new();

    if (udp_pcb != NULL)
    {
        /* Bind the UDP PCB to any IP address and the defined port */
        err_t err = udp_bind(udp_pcb, IP_ADDR_ANY, CAN_UDP_PORT);

        if (err == ERR_OK)
        {
            /* Set the callback function for receiving data */
            udp_recv(udp_pcb, udp_receive_callback, NULL);
        }
    }
}
