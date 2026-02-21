/**********************************************************************************************************************
 * \file SecureUdpSender.h
 * \brief Header for the SECURE UDP sender application.
 *********************************************************************************************************************/
#ifndef SECUREUDPSENDER_H_
#define SECUREUDPSENDER_H_

#include "Ifx_Types.h"
#include "IfxPort.h"

#define RECEIVER_IP_BYTE_1          192
#define RECEIVER_IP_BYTE_2          168
#define RECEIVER_IP_BYTE_3          1
#define RECEIVER_IP_BYTE_4          20

#define CAN_UDP_PORT                7

#define EXPECTED_DATA_LOW_WORD      (uint32)0xC0CAC01A
#define EXPECTED_DATA_HIGH_WORD     (uint32)0xBA5EBA11

#define LED_TX                      &MODULE_P13,0

/* --- Security Definitions --- */
/* This secret key MUST be identical on both the sender and receiver boards */
static const uint8 PRE_SHARED_KEY[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
#define DATA_LENGTH                 8 /* 2x uint32 */
#define MAC_LENGTH                  8 /* Length of our authentication tag */

void secureUdpSenderInit(void);
void sendSecureUdpPacket(void);

#endif /* SECUREUDPSENDER_H_ */
