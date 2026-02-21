/**********************************************************************************************************************
 * \file SecureUdpSender.c
 * \brief Implements the SECURE UDP sender with a Message Authentication Code (MAC).
 *********************************************************************************************************************/
#include "SecureUdpSender.h"
#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include <string.h> /* For memcpy */

struct udp_pcb *g_udp_pcb = NULL;

/* This is a simple XOR-based "HMAC" for demonstration. A real system would use SHA-256. */
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

void secureUdpSenderInit(void)
{
    g_udp_pcb = udp_new();
}

void sendSecureUdpPacket(void)
{
    if (g_udp_pcb == NULL) { return; }

    /* The total payload is now the original data + the MAC tag */
    const int payload_size = DATA_LENGTH + MAC_LENGTH;
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, payload_size, PBUF_RAM);
    if (p == NULL) { return; }

    /* 1. Prepare data and MAC */
    uint8 secure_payload[payload_size];
    uint32* data_part = (uint32*)secure_payload;
    uint8* mac_part = secure_payload + DATA_LENGTH;

    data_part[0] = EXPECTED_DATA_LOW_WORD;
    data_part[1] = EXPECTED_DATA_HIGH_WORD;

    /* Generate the MAC/signature using the data and the secret key */
    generate_mac(secure_payload, DATA_LENGTH, PRE_SHARED_KEY, mac_part);

    /* 2. Copy the full payload (data + MAC) into the packet buffer */
    if (pbuf_take(p, secure_payload, payload_size) == ERR_OK)
    {
        ip_addr_t receiver_ip;
        IP4_ADDR(&receiver_ip, RECEIVER_IP_BYTE_1, RECEIVER_IP_BYTE_2, RECEIVER_IP_BYTE_3, RECEIVER_IP_BYTE_4);
        udp_sendto(g_udp_pcb, p, &receiver_ip, CAN_UDP_PORT);
        IfxPort_togglePin(LED_TX);
    }
    pbuf_free(p);
}
