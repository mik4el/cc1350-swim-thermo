/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/***** Includes *****/
/* XDCtools Header files */ 
#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include "ConcentratorRadioTask.h"

/* BIOS Header files */ 
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Clock.h>

/* Drivers */
#include <ti/drivers/rf/RF.h>
#include <ti/drivers/PIN.h>
#include <stdio.h>

/* Board Header files */
#include "Board.h"

/* EasyLink API Header files */ 
#include "easylink/EasyLink.h"

/* Application Header files */ 
#include "RadioProtocol.h"
#include "ConcentratorTask.h"

#ifdef FEATURE_BLE_ADV
#include "ble_adv/BleAdv.h"
#endif

/***** Defines *****/
#define CONCENTRATORRADIO_TASK_STACK_SIZE 1024
#define CONCENTRATORRADIO_TASK_PRIORITY   3

#define RADIO_EVENT_ALL                     0xFFFFFFFF
#define RADIO_EVENT_VALID_PACKET_RECEIVED   (uint32_t)(1 << 0)
#define RADIO_EVENT_INVALID_PACKET_RECEIVED (uint32_t)(1 << 1)
#ifdef FEATURE_BLE_ADV
#define RADIO_EVENT_UBLE                    (uint32_t)(1 << 2)
#endif

#define CONCENTRATORRADIO_MAX_RETRIES 2
#define NORERADIO_ACK_TIMEOUT_TIME_MS (160)
#define CONCENTRATOR_MAX_NODES 7

#define CONCENTRATOR_ACTIVITY_LED Board_PIN_LED0

/***** Type declarations *****/



/***** Variable declarations *****/
static Task_Params concentratorRadioTaskParams;
Task_Struct concentratorRadioTask; /* not static so you can see in ROV */
static uint8_t concentratorRadioTaskStack[CONCENTRATORRADIO_TASK_STACK_SIZE];
Event_Struct radioOperationEvent;  /* not static so you can see in ROV */
static Event_Handle radioOperationEventHandle;



static ConcentratorRadio_PacketReceivedCallback packetReceivedCallback;
static union ConcentratorPacket latestRxPacket;
static EasyLink_TxPacket txPacket;
static struct AckPacket ackPacket;
static uint8_t concentratorAddress;
static int8_t latestRssi;

/***** Prototypes *****/
static void concentratorRadioTaskFunction(UArg arg0, UArg arg1);
static void rxDoneCallback(EasyLink_RxPacket * rxPacket, EasyLink_Status status);
static void notifyPacketReceived(union ConcentratorPacket* latestRxPacket);
static void sendAck(uint8_t latestSourceAddress);

#ifdef FEATURE_BLE_ADV
static void bleAdv_eventProxyCB(void);
static void bleAdv_updateTlmCB(uint16_t *pVbatt, uint16_t *pTemp, uint32_t *pTime100MiliSec);
#endif

/* Pin driver handle */
static PIN_Handle ledPinHandle;
static PIN_State ledPinState;

/* Configure LED Pin */
PIN_Config ledPinTable[] = {
    CONCENTRATOR_ACTIVITY_LED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

/***** Function definitions *****/
void ConcentratorRadioTask_init(void) {

    /* Open LED pins */
    ledPinHandle = PIN_open(&ledPinState, ledPinTable);
	if (!ledPinHandle)
	{
        System_abort("Error initializing board 3.3V domain pins\n");
    }

    /* Create event used internally for state changes */
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&radioOperationEvent, &eventParam);
    radioOperationEventHandle = Event_handle(&radioOperationEvent);

    /* Create the concentrator radio protocol task */
    Task_Params_init(&concentratorRadioTaskParams);
    concentratorRadioTaskParams.stackSize = CONCENTRATORRADIO_TASK_STACK_SIZE;
    concentratorRadioTaskParams.priority = CONCENTRATORRADIO_TASK_PRIORITY;
    concentratorRadioTaskParams.stack = &concentratorRadioTaskStack;
    Task_construct(&concentratorRadioTask, concentratorRadioTaskFunction, &concentratorRadioTaskParams, NULL);
}

void ConcentratorRadioTask_registerPacketReceivedCallback(ConcentratorRadio_PacketReceivedCallback callback) {
    packetReceivedCallback = callback;
}

static void concentratorRadioTaskFunction(UArg arg0, UArg arg1)
{
#ifdef FEATURE_BLE_ADV
    BleAdv_Params_t bleAdv_Params;
    /* Set mulitclient mode for EasyLink */
    EasyLink_setCtrl(EasyLink_Ctrl_MultiClient_Mode, 1);
#endif

    /* Initialize EasyLink */
	EasyLink_Params easyLink_params;
    EasyLink_Params_init(&easyLink_params);
	
	easyLink_params.ui32ModType = RADIO_EASYLINK_MODULATION;
#ifdef FEATURE_BLE_ADV
    easyLink_params.pClientEventCb = &rfSwitchCallback;
    easyLink_params.nClientEventMask = RF_ClientEventSwitchClientEntered;
#endif

	if(EasyLink_init(&easyLink_params) != EasyLink_Status_Success){ 
		System_abort("EasyLink_init failed");
	}	

    /* If you wich to use a frequency other than the default use
     * the below API
     * EasyLink_setFrequency(868000000);
     */

    /* Set concentrator address */;
    concentratorAddress = RADIO_CONCENTRATOR_ADDRESS;
    EasyLink_enableRxAddrFilter(&concentratorAddress, 1, 1);

    /* Set up Ack packet */
    ackPacket.header.sourceAddress = concentratorAddress;
    ackPacket.header.packetType = RADIO_PACKET_TYPE_ACK_PACKET;

    /* Enter receive */
    if(EasyLink_receiveAsync(rxDoneCallback, 0) != EasyLink_Status_Success) {
        System_abort("EasyLink_receiveAsync failed");
    }

#ifdef FEATURE_BLE_ADV
    /* Initialize the Simple Beacon module wit default params */
    BleAdv_Params_init(&bleAdv_Params);
    bleAdv_Params.pfnPostEvtProxyCB = bleAdv_eventProxyCB;
    bleAdv_Params.pfnUpdateTlmCB = bleAdv_updateTlmCB;
    bleAdv_Params.pfnAdvStatsCB = NodeTask_advStatsCB;
    BleAdv_init(&bleAdv_Params);
    char url_format[] = "https://m4bd.com/s/%x/";
    char url_ready[21];
    sprintf(url_ready, url_format, concentratorAddress);
    BleAdv_updateUrl(url_ready);
    BleAdv_setAdvertiserType(BleAdv_AdertiserNone);
#endif

    while (1) {
        uint32_t events = Event_pend(radioOperationEventHandle, 0, RADIO_EVENT_ALL, BIOS_WAIT_FOREVER);

        /* If valid packet received */
        if(events & RADIO_EVENT_VALID_PACKET_RECEIVED) {

            /* Send ack packet */
            sendAck(latestRxPacket.header.sourceAddress);

            /* Call packet received callback */
            notifyPacketReceived(&latestRxPacket);

            /* Go back to RX */
// THIS BLOCKS BLE SENDING ONLY DO THIS AFTER SOME TIME AND AFTER DISABLING BLE SENDING
//            if(EasyLink_receiveAsync(rxDoneCallback, 0) != EasyLink_Status_Success) {
//                System_abort("EasyLink_receiveAsync failed");
//            }

            /* toggle Activity LED */
            PIN_setOutputValue(ledPinHandle, CONCENTRATOR_ACTIVITY_LED,
                    !PIN_getOutputValue(CONCENTRATOR_ACTIVITY_LED));
        }

        /* If invalid packet received */
        if(events & RADIO_EVENT_INVALID_PACKET_RECEIVED) {
            /* Go back to RX */
            if(EasyLink_receiveAsync(rxDoneCallback, 0) != EasyLink_Status_Success) {
                System_abort("EasyLink_receiveAsync failed");
            }
        }

#ifdef FEATURE_BLE_ADV
        if (events & RADIO_EVENT_UBLE)
        {
            uble_processMsg();
        }
#endif

    }
}

static void sendAck(uint8_t latestSourceAddress) {

    /* Set destinationAdress, but use EasyLink layers destination address capability */
    txPacket.dstAddr[0] = latestSourceAddress;

    /* Copy ACK packet to payload, skipping the destination address byte.
     * Note that the EasyLink API will implicitly both add the length byte and the destination address byte. */
    memcpy(txPacket.payload, &ackPacket.header, sizeof(ackPacket));
    txPacket.len = sizeof(ackPacket);

    /* Send packet  */
    if (EasyLink_transmit(&txPacket) != EasyLink_Status_Success)
    {
        System_abort("EasyLink_transmit failed");
    }
}

static void notifyPacketReceived(union ConcentratorPacket* latestRxPacket)
{
    if (packetReceivedCallback)
    {
        packetReceivedCallback(latestRxPacket, latestRssi);
    }
}

#ifdef FEATURE_BLE_ADV
/*********************************************************************
* @fn      bleAdv_eventProxyCB
*
* @brief   Post an event to the application so that a Micro BLE Stack internal
*          event is processed by Micro BLE Stack later in the application
*          task's context.
*
* @param   None
*
* @return  None
*/
static void bleAdv_eventProxyCB(void)
{
    /* Post event */
    Event_post(radioOperationEventHandle, RADIO_EVENT_UBLE);
}

/*********************************************************************
* @fn      bleAdv_updateTlmCB

* @brief Callback to update the TLM data
*
* @param pvBatt Battery level
* @param pTemp Current temperature
* @param pTime100MiliSec time since boot in 100ms units
*
* @return  None
*/
static void bleAdv_updateTlmCB(uint16_t *pvBatt, uint16_t *pTemp, uint32_t *pTime100MiliSec)
{
    uint16_t vBatt = 1234; // Debug value
    uint16_t temp = FLOAT2FIXED(12.34); // Debug value
    uint32_t time100MiliSec = 4321; // Debug value
    *pvBatt = vBatt; // dmInternalTempSensorPacket.batt;
    *pTemp = temp; // dmInternalTempSensorPacket.temp2;
    *pTime100MiliSec = time100MiliSec; // dmInternalTempSensorPacket.time100MiliSec/10;
}
#endif

static void rxDoneCallback(EasyLink_RxPacket * rxPacket, EasyLink_Status status)
{
    union ConcentratorPacket* tmpRxPacket;

    /* If we received a packet successfully */
    if (status == EasyLink_Status_Success)
    {
        /* Save the latest RSSI, which is later sent to the receive callback */
        latestRssi = (int8_t)rxPacket->rssi;

        /* Check that this is a valid packet */
        tmpRxPacket = (union ConcentratorPacket*)(rxPacket->payload);

        /* If this is a known packet */
        if (tmpRxPacket->header.packetType == RADIO_PACKET_TYPE_DM_SENSOR_PACKET)
        {
            /* Save packet */
            latestRxPacket.header.sourceAddress = rxPacket->payload[0];
            latestRxPacket.header.packetType =  rxPacket->payload[1];
            latestRxPacket.dmSensorPacket.temp2 = (rxPacket->payload[2] << 8) |
                                                  rxPacket->payload[3];
            latestRxPacket.dmSensorPacket.batt =    (rxPacket->payload[4] << 8) |
                                                     rxPacket->payload[5];
            latestRxPacket.dmSensorPacket.temp1 = (rxPacket->payload[6] << 8) |
                                                          rxPacket->payload[7];
            latestRxPacket.dmSensorPacket.time100MiliSec = (rxPacket->payload[8] << 24) |
                                                           (rxPacket->payload[9] << 16) |
                                                           (rxPacket->payload[10] << 8) |
                                                            rxPacket->payload[11];

            /* Signal packet received */
            Event_post(radioOperationEventHandle, RADIO_EVENT_VALID_PACKET_RECEIVED);
        }
        else
        {
            /* Signal invalid packet received */
            Event_post(radioOperationEventHandle, RADIO_EVENT_INVALID_PACKET_RECEIVED);
        }
    }
    else
    {
        /* Signal invalid packet received */
        Event_post(radioOperationEventHandle, RADIO_EVENT_INVALID_PACKET_RECEIVED);
    }
}
