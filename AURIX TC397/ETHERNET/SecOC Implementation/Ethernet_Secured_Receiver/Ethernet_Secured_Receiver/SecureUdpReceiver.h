/**********************************************************************************************************************
 * \file SecureUdpReceiver.h
 * \brief Header for the SECURE UDP receiver application.
 *********************************************************************************************************************/
#ifndef SECUREUDPRECEIVER_H_
#define SECUREUDPRECEIVER_H_

#include "Ifx_Types.h"
#include "IfxPort.h"

#define CAN_UDP_PORT                7
#define EXPECTED_DATA_LOW_WORD      (uint32)0xC0CAC01A
#define EXPECTED_DATA_HIGH_WORD     (uint32)0xBA5EBA11
#define LED_RX                      &MODULE_P13,1

/* --- Security Definitions --- */
/* This secret key MUST be identical to the one on the sender board */
static const uint8 PRE_SHARED_KEY[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
#define DATA_LENGTH                 8 /* 2x uint32 */
#define MAC_LENGTH                  8 /* Length of our authentication tag */

void secureUdpReceiverInit(void);

#endif /* SECUREUDPRECEIVER_H_ */
