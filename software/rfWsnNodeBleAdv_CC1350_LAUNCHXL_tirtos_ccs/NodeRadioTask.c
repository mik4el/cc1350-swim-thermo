/*
 * Copyright (c) 2016-2018, Texas Instruments Incorporated
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

/* BIOS Header files */ 
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Clock.h>

/* TI-RTOS Header files */ 
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <ti/drivers/rf/RF.h>
#include <ti/drivers/PIN.h>

/* Board Header files */
#include "Board.h"

/* Standard C Libraries */
#include <stdlib.h>
#include <stdio.h>

/* EasyLink API Header files */ 
#include "easylink/EasyLink.h"

/* Application Header files */ 
#include "RadioProtocol.h"
#include "NodeRadioTask.h"
#include "NodeTask.h"

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aon_batmon.h)
#include DeviceFamily_constructPath(driverlib/trng.h)
#include DeviceFamily_constructPath(driverlib/aux_adc.h) //for ADC operations

#ifdef FEATURE_BLE_ADV
#include "ble_adv/BleAdv.h"
#endif

/***** Defines *****/
#define NODERADIO_TASK_STACK_SIZE 1024
#define NODERADIO_TASK_PRIORITY   3

#define RADIO_EVENT_ALL                 0xFFFFFFFF
#define RADIO_EVENT_SEND_ADC_DATA       (uint32_t)(1 << 0)
#define RADIO_EVENT_DATA_ACK_RECEIVED   (uint32_t)(1 << 1)
#define RADIO_EVENT_ACK_TIMEOUT         (uint32_t)(1 << 2)
#define RADIO_EVENT_SEND_FAIL           (uint32_t)(1 << 3)
#ifdef FEATURE_BLE_ADV
#define NODE_EVENT_UBLE                 (uint32_t)(1 << 4)
#endif

#define NODERADIO_MAX_RETRIES 2
#define NORERADIO_ACK_TIMEOUT_TIME_MS (160)


/***** Type declarations *****/
struct RadioOperation {
    EasyLink_TxPacket easyLinkTxPacket;
    uint8_t retriesDone;
    uint8_t maxNumberOfRetries;
    uint32_t ackTimeoutMs;
    enum NodeRadioOperationStatus result;
};


/***** Variable declarations *****/
static Task_Params nodeRadioTaskParams;
Task_Struct nodeRadioTask;        /* not static so you can see in ROV */
static uint8_t nodeRadioTaskStack[NODERADIO_TASK_STACK_SIZE];
Semaphore_Struct radioAccessSem;  /* not static so you can see in ROV */
static Semaphore_Handle radioAccessSemHandle;
Event_Struct radioOperationEvent; /* not static so you can see in ROV */
static Event_Handle radioOperationEventHandle;
Semaphore_Struct radioResultSem;  /* not static so you can see in ROV */
static Semaphore_Handle radioResultSemHandle;
static struct RadioOperation currentRadioOperation;
static uint16_t adcData;
static uint8_t nodeAddress = 0;
static struct DualModeInternalTempSensorPacket dmInternalTempSensorPacket;


/* previous Tick count used to calculate uptime */
static uint32_t prevTicks;

/* Pin driver handle */
extern PIN_Handle ledPinHandle;

/***** Prototypes *****/
static void nodeRadioTaskFunction(UArg arg0, UArg arg1);
static void returnRadioOperationStatus(enum NodeRadioOperationStatus status);
static void sendDmPacket(struct DualModeInternalTempSensorPacket sensorPacket, uint8_t maxNumberOfRetries, uint32_t ackTimeoutMs);
static void resendPacket(void);
static void rxDoneCallback(EasyLink_RxPacket * rxPacket, EasyLink_Status status);

#ifdef FEATURE_BLE_ADV
static void bleAdv_eventProxyCB(void);
static void bleAdv_updateTlmCB(uint16_t *pVbatt, uint16_t *pTemp, uint32_t *pTime100MiliSec);
#endif

/***** Function definitions *****/
double convertADCToTempDouble(uint16_t adcValue) {
    return -0.193 * adcValue * (AUXADC_FIXED_REF_VOLTAGE_UNSCALED / 1000)  / 4095 + 212.009; // constants from LMT70 datasheet
}

void NodeRadioTask_init(void) {

    /* Create semaphore used for exclusive radio access */
    Semaphore_Params semParam;
    Semaphore_Params_init(&semParam);
    Semaphore_construct(&radioAccessSem, 1, &semParam);
    radioAccessSemHandle = Semaphore_handle(&radioAccessSem);

    /* Create semaphore used for callers to wait for result */
    Semaphore_construct(&radioResultSem, 0, &semParam);
    radioResultSemHandle = Semaphore_handle(&radioResultSem);

    /* Create event used internally for state changes */
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&radioOperationEvent, &eventParam);
    radioOperationEventHandle = Event_handle(&radioOperationEvent);

    /* Create the radio protocol task */
    Task_Params_init(&nodeRadioTaskParams);
    nodeRadioTaskParams.stackSize = NODERADIO_TASK_STACK_SIZE;
    nodeRadioTaskParams.priority = NODERADIO_TASK_PRIORITY;
    nodeRadioTaskParams.stack = &nodeRadioTaskStack;
    Task_construct(&nodeRadioTask, nodeRadioTaskFunction, &nodeRadioTaskParams, NULL);
}

uint8_t nodeRadioTask_getNodeAddr(void)
{
    return nodeAddress;
}

static void nodeRadioTaskFunction(UArg arg0, UArg arg1)
{
#ifdef FEATURE_BLE_ADV
    BleAdv_Params_t bleAdv_Params;
    /* Set mulitclient mode for EasyLink */
    EasyLink_setCtrl(EasyLink_Ctrl_MultiClient_Mode, 1);

#endif

    EasyLink_Params easyLink_params;
    EasyLink_Params_init(&easyLink_params);

    easyLink_params.ui32ModType = RADIO_EASYLINK_MODULATION;
#ifdef FEATURE_BLE_ADV
    easyLink_params.pClientEventCb = &rfSwitchCallback;
    easyLink_params.nClientEventMask = RF_ClientEventSwitchClientEntered;
#endif

    /* Initialize EasyLink */
    if(EasyLink_init(&easyLink_params) != EasyLink_Status_Success){
        System_abort("EasyLink_init failed");
    }

    /* If you wish to use a frequency other than the default use
     * the below API
     * EasyLink_setFrequency(868000000);
     */

    /* Use the True Random Number Generator to generate sensor node address randomly */;
    Power_setDependency(PowerCC26XX_PERIPH_TRNG);
    TRNGEnable();
    /* Do not accept the same address as the concentrator, in that case get a new random value */
    do
    {
        while (!(TRNGStatusGet() & TRNG_NUMBER_READY))
        {
            //wait for random number generator
        }
        nodeAddress = (uint8_t)TRNGNumberGet(TRNG_LOW_WORD);
    } while (nodeAddress == RADIO_CONCENTRATOR_ADDRESS);
    TRNGDisable();
    Power_releaseDependency(PowerCC26XX_PERIPH_TRNG);

    /* Set the filter to the generated random address */
    if (EasyLink_enableRxAddrFilter(&nodeAddress, 1, 1) != EasyLink_Status_Success)
    {
        System_abort("EasyLink_enableRxAddrFilter failed");
    }

    /* Setup ADC sensor packet */
    dmInternalTempSensorPacket.header.sourceAddress = nodeAddress;
    dmInternalTempSensorPacket.header.packetType = RADIO_PACKET_TYPE_DM_SENSOR_PACKET;

    /* Initialise previous Tick count used to calculate uptime for the TLM beacon */
    prevTicks = Clock_getTicks();

#ifdef FEATURE_BLE_ADV
    /* Initialize the Simple Beacon module wit default params */
    BleAdv_Params_init(&bleAdv_Params);
    bleAdv_Params.pfnPostEvtProxyCB = bleAdv_eventProxyCB;
    bleAdv_Params.pfnUpdateTlmCB = bleAdv_updateTlmCB;
    bleAdv_Params.pfnAdvStatsCB = NodeTask_advStatsCB;
    BleAdv_init(&bleAdv_Params);
    char url_format[] = "https://m4bd.com/s/%x/";
    char url_ready[21];
    sprintf(url_ready, url_format, nodeAddress);
    BleAdv_updateUrl(url_ready);
    BleAdv_setAdvertiserType(BleAdv_AdertiserNone);
#endif

    /* Enter main task loop */
    while (1)
    {
        /* Wait for an event */
        uint32_t events = Event_pend(radioOperationEventHandle, 0, RADIO_EVENT_ALL, BIOS_WAIT_FOREVER);

        /* If we should send ADC data */
        if (events & RADIO_EVENT_SEND_ADC_DATA)
        {
            uint32_t currentTicks;

            currentTicks = Clock_getTicks();
            //check for wrap around
            if (currentTicks > prevTicks)
            {
                //calculate time since last reading in 0.1s units
                dmInternalTempSensorPacket.time100MiliSec = ((currentTicks - prevTicks) * Clock_tickPeriod) / 100000;
            }
            else
            {
                //calculate time since last reading in 0.1s units
                dmInternalTempSensorPacket.time100MiliSec = ((prevTicks - currentTicks) * Clock_tickPeriod) / 100000;
            }
            prevTicks = currentTicks;

            dmInternalTempSensorPacket.batt = (AONBatMonBatteryVoltageGet() * 125) >> 5;
            dmInternalTempSensorPacket.temp1 = INT2FIXED((int16_t)AONBatMonTemperatureGetDegC());
            dmInternalTempSensorPacket.temp2 = FLOAT2FIXED(convertADCToTempDouble(adcData));

            sendDmPacket(dmInternalTempSensorPacket, NODERADIO_MAX_RETRIES, NORERADIO_ACK_TIMEOUT_TIME_MS);
        }

        /* If we get an ACK from the concentrator */
        if (events & RADIO_EVENT_DATA_ACK_RECEIVED)
        {
            returnRadioOperationStatus(NodeRadioStatus_Success);
        }

        /* If we get an ACK timeout */
        if (events & RADIO_EVENT_ACK_TIMEOUT)
        {

            /* If we haven't resent it the maximum number of times yet, then resend packet */
            if (currentRadioOperation.retriesDone < currentRadioOperation.maxNumberOfRetries)
            {
                resendPacket();
            }
            else
            {
                /* Else return send fail */
                Event_post(radioOperationEventHandle, RADIO_EVENT_SEND_FAIL);
            }
        }

        /* If send fail */
        if (events & RADIO_EVENT_SEND_FAIL)
        {
            returnRadioOperationStatus(NodeRadioStatus_Failed);
        }

#ifdef FEATURE_BLE_ADV
        if (events & NODE_EVENT_UBLE)
        {
            uble_processMsg();
        }
#endif
    }
}

enum NodeRadioOperationStatus NodeRadioTask_sendAdcData(uint16_t data)
{
    enum NodeRadioOperationStatus status;

    /* Get radio access semaphore */
    Semaphore_pend(radioAccessSemHandle, BIOS_WAIT_FOREVER);

    /* Save data to send */
    adcData = data;

    /* Raise RADIO_EVENT_SEND_ADC_DATA event */
    Event_post(radioOperationEventHandle, RADIO_EVENT_SEND_ADC_DATA);

    /* Wait for result */
    Semaphore_pend(radioResultSemHandle, BIOS_WAIT_FOREVER);

    /* Get result */
    status = currentRadioOperation.result;

    /* Return radio access semaphore */
    Semaphore_post(radioAccessSemHandle);

    return status;
}

static void returnRadioOperationStatus(enum NodeRadioOperationStatus result)
{
    /* Save result */
    currentRadioOperation.result = result;

    /* Post result semaphore */
    Semaphore_post(radioResultSemHandle);
}

static void sendDmPacket(struct DualModeInternalTempSensorPacket sensorPacket, uint8_t maxNumberOfRetries, uint32_t ackTimeoutMs)
{
    /* Set destination address in EasyLink API */
    currentRadioOperation.easyLinkTxPacket.dstAddr[0] = RADIO_CONCENTRATOR_ADDRESS;

    /* Copy ADC packet to payload
     * Note that the EasyLink API will implcitily both add the length byte and the destination address byte. */
    currentRadioOperation.easyLinkTxPacket.payload[0] = dmInternalTempSensorPacket.header.sourceAddress;
    currentRadioOperation.easyLinkTxPacket.payload[1] = dmInternalTempSensorPacket.header.packetType;
    currentRadioOperation.easyLinkTxPacket.payload[2] = (dmInternalTempSensorPacket.temp2 & 0xFF00) >> 8;
    currentRadioOperation.easyLinkTxPacket.payload[3] = (dmInternalTempSensorPacket.temp2 & 0xFF);
    currentRadioOperation.easyLinkTxPacket.payload[4] = (dmInternalTempSensorPacket.batt & 0xFF00) >> 8;
    currentRadioOperation.easyLinkTxPacket.payload[5] = (dmInternalTempSensorPacket.batt & 0xFF);
    currentRadioOperation.easyLinkTxPacket.payload[6] = (dmInternalTempSensorPacket.temp1 & 0xFF00) >> 8;
    currentRadioOperation.easyLinkTxPacket.payload[7] = (dmInternalTempSensorPacket.temp1 & 0xFF);
    currentRadioOperation.easyLinkTxPacket.payload[8] = (dmInternalTempSensorPacket.time100MiliSec & 0xFF000000) >> 24;
    currentRadioOperation.easyLinkTxPacket.payload[9] = (dmInternalTempSensorPacket.time100MiliSec & 0x00FF0000) >> 16;
    currentRadioOperation.easyLinkTxPacket.payload[10] = (dmInternalTempSensorPacket.time100MiliSec & 0xFF00) >> 8;
    currentRadioOperation.easyLinkTxPacket.payload[11] = (dmInternalTempSensorPacket.time100MiliSec & 0xFF);

    currentRadioOperation.easyLinkTxPacket.len = sizeof(struct DualModeInternalTempSensorPacket);

    /* Setup retries */
    currentRadioOperation.maxNumberOfRetries = maxNumberOfRetries;
    currentRadioOperation.ackTimeoutMs = ackTimeoutMs;
    currentRadioOperation.retriesDone = 0;
    EasyLink_setCtrl(EasyLink_Ctrl_AsyncRx_TimeOut, EasyLink_ms_To_RadioTime(ackTimeoutMs));

    /* Send packet  */
    if (EasyLink_transmit(&currentRadioOperation.easyLinkTxPacket) != EasyLink_Status_Success)
    {
        System_abort("EasyLink_transmit failed");
    }
#if defined(Board_DIO30_SWPWR)
    /* this was a blocking call, so Tx is now complete. Turn off the RF switch power */
    PIN_setOutputValue(blePinHandle, Board_DIO30_SWPWR, 0);
#endif

    /* Enter RX */
    if (EasyLink_receiveAsync(rxDoneCallback, 0) != EasyLink_Status_Success)
    {
        System_abort("EasyLink_receiveAsync failed");
    }
}

static void resendPacket(void)
{
    /* Send packet  */
    if (EasyLink_transmit(&currentRadioOperation.easyLinkTxPacket) != EasyLink_Status_Success)
    {
        System_abort("EasyLink_transmit failed");
    }
#if defined(Board_DIO30_SWPWR)
    /* this was a blocking call, so Tx is now complete. Turn off the RF switch power */
    PIN_setOutputValue(blePinHandle, Board_DIO30_SWPWR, 0);
#endif

    /* Enter RX and wait for ACK with timeout */
    if (EasyLink_receiveAsync(rxDoneCallback, 0) != EasyLink_Status_Success)
    {
        System_abort("EasyLink_receiveAsync failed");
    }

    /* Increase retries by one */
    currentRadioOperation.retriesDone++;
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
    Event_post(radioOperationEventHandle, NODE_EVENT_UBLE);
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
    *pvBatt = dmInternalTempSensorPacket.batt;
    *pTemp = dmInternalTempSensorPacket.temp2;
    *pTime100MiliSec = dmInternalTempSensorPacket.time100MiliSec/10;
}
#endif

static void rxDoneCallback(EasyLink_RxPacket * rxPacket, EasyLink_Status status)
{
    struct PacketHeader* packetHeader;

#if defined(Board_DIO30_SWPWR)
    /* Rx is now complete. Turn off the RF switch power */
    PIN_setOutputValue(blePinHandle, Board_DIO30_SWPWR, 0);
#endif

    /* If this callback is called because of a packet received */
    if (status == EasyLink_Status_Success)
    {
        /* Check the payload header */
        packetHeader = (struct PacketHeader*)rxPacket->payload;

        /* Check if this is an ACK packet */
        if (packetHeader->packetType == RADIO_PACKET_TYPE_ACK_PACKET)
        {
            /* Signal ACK packet received */
            Event_post(radioOperationEventHandle, RADIO_EVENT_DATA_ACK_RECEIVED);
        }
        else
        {
            /* Packet Error, treat as a Timeout and Post a RADIO_EVENT_ACK_TIMEOUT
               event */
            Event_post(radioOperationEventHandle, RADIO_EVENT_ACK_TIMEOUT);
        }
    }
    /* did the Rx timeout */
    else if(status == EasyLink_Status_Rx_Timeout)
    {
        /* Post a RADIO_EVENT_ACK_TIMEOUT event */
        Event_post(radioOperationEventHandle, RADIO_EVENT_ACK_TIMEOUT);
    }
    else
    {
        /* The Ack receiption may have been corrupted causing an error.
         * Treat this as a timeout
         */
        Event_post(radioOperationEventHandle, RADIO_EVENT_ACK_TIMEOUT);
    }
}
