/**********************************************************************************************************************
 * \file SecureUdpReceiver.c
 * \brief Implements the SECURE UDP receiver with MAC verification.
 *********************************************************************************************************************/
#include "SecureUdpReceiver.h"
#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include <string.h> /* For memcpy and memcmp */

/* This function MUST be identical to the one on the sender */
void generate_mac(const uint8* data, uint32 data_len, const uint8* key, uint8* mac_out)
{
    for (int i = 0; i < MAC_LENGTH; ++i)
    {
        mac_out[i] = key[i];
        for (int j = 0; j < data_len; ++j)
        {
            mac_out[i] ^= data[j];
        }
    }
}

void secure_udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    const int expected_len = DATA_LENGTH + MAC_LENGTH;

    if (p != NULL)
    {
        /* 1. Check if the packet has the correct total length */
        if (p->len == expected_len && p->tot_len == expected_len)
        {
            uint8 received_payload[expected_len];
            pbuf_copy_partial(p, received_payload, p->tot_len, 0);

            uint32* data_part = (uint32*)received_payload;
            uint8* received_mac = received_payload + DATA_LENGTH;

            /* 2. Check if the data part matches what we expect */
            if (data_part[0] == EXPECTED_DATA_LOW_WORD && data_part[1] == EXPECTED_DATA_HIGH_WORD)
            {
                /* 3. Verify the MAC/signature */
                uint8 calculated_mac[MAC_LENGTH];
                generate_mac(received_payload, DATA_LENGTH, PRE_SHARED_KEY, calculated_mac);

                /* Compare the MAC from the packet with the one we just calculated */
                if (memcmp(received_mac, calculated_mac, MAC_LENGTH) == 0)
                {
                    /* SUCCESS: Data is authentic and has integrity. Toggle the LED. */
                    IfxPort_togglePin(LED_RX);
                }
            }
        }
        pbuf_free(p);
    }
}

void secureUdpReceiverInit(void)
{
    struct udp_pcb *udp_pcb = udp_new();
    if (udp_pcb != NULL)
    {
        if (udp_bind(udp_pcb, IP_ADDR_ANY, CAN_UDP_PORT) == ERR_OK)
        {
            udp_recv(udp_pcb, secure_udp_receive_callback, NULL);
        }
    }
}
