/**********************************************************************************************************************
 * \file CanUdpReceiver.h
 * \brief Header file for the CAN UDP receiver application.
 *********************************************************************************************************************/

#ifndef CANUDPRECEIVER_H_
#define CANUDPRECEIVER_H_

#include "Ifx_Types.h"
#include "IfxPort.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

/* This is the UDP port the receiver will listen on */
#define CAN_UDP_PORT                7

/* These are the data values the receiver expects to see */
#define EXPECTED_DATA_LOW_WORD      (uint32)0xC0CAC01A
#define EXPECTED_DATA_HIGH_WORD     (uint32)0xBA5EBA11

/* Define the LED pin for indicating a received message */
#define LED_RX                      &MODULE_P13,1   /* LED D108 (Port 13, Pin 1) for Receive */

/*********************************************************************************************************************/
/*-----------------------------------------------Function Prototypes-------------------------------------------------*/
/*********************************************************************************************************************/

/** \brief Initializes the UDP listener. */
void canUdpReceiverInit(void);

#endif /* CANUDPRECEIVER_H_ */
