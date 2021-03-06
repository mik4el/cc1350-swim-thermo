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

/* BIOS Header files */ 
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Clock.h>

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>
#include <ti/display/Display.h>
#include <ti/display/DisplayExt.h>

/* Board Header files */
#include "Board.h"

/* Application Header files */ 
#include "ConcentratorRadioTask.h"
#include "ConcentratorTask.h"
#include "RadioProtocol.h"

#ifdef FEATURE_BLE_ADV
#include "ble_adv/BleAdv.h"
#endif

/***** Defines *****/
#define CONCENTRATOR_TASK_STACK_SIZE 1024
#define CONCENTRATOR_TASK_PRIORITY   3

#define CONCENTRATOR_EVENT_ALL                     0xFFFFFFFF
#define CONCENTRATOR_EVENT_NEW_ADC_SENSOR_VALUE    (uint32_t)(1 << 0)
#define CONCENTRATOR_EVENT_UPDATE_DISPLAY          (uint32_t)(1 << 1)

#define CONCENTRATOR_MAX_NODES 7

#define CONCENTRATOR_DISPLAY_LINES 10
#define CONCENTRATOR_DISPLAY_UPDATE_MS 1000

#define CONCENTRATOR_LED_BLINK_ON_DURATION_MS       100
#define CONCENTRATOR_LED_BLINK_OFF_DURATION_MS      400
#define CONCENTRATOR_LED_BLINK_TIMES                5

#define CONCENTRATOR_IDENTIFY_LED CC1350_LAUNCHXL_PIN_RLED

/***** Type declarations *****/
struct AdcSensorNode {
    uint8_t address;
    uint16_t latestTemp2Value; //fixed 8.8 notation
    uint16_t latestBatt;
    uint16_t latestTemp1Value; //Fixed 8.8 notation
    int8_t latestRssi;
    uint32_t timeForLastRX;
    uint32_t receivedPackets;
};

/***** Variable declarations *****/
static Task_Params concentratorTaskParams;
Task_Struct concentratorTask;    /* not static so you can see in ROV */
static uint8_t concentratorTaskStack[CONCENTRATOR_TASK_STACK_SIZE];
Event_Struct concentratorEvent;  /* not static so you can see in ROV */
static Event_Handle concentratorEventHandle;
static struct AdcSensorNode latestActiveAdcSensorNode;
struct AdcSensorNode knownSensorNodes[CONCENTRATOR_MAX_NODES];
static struct AdcSensorNode* lastAddedSensorNode = knownSensorNodes;
static Display_Handle hDisplayLcd;
uint32_t prevTicks = 0;
uint32_t upTime = 0;

Clock_Struct displayClock;     /* Not static so you can see in ROV */
static Clock_Handle displayClockHandle;

/***** Prototypes *****/
static void concentratorTaskFunction(UArg arg0, UArg arg1);
static void packetReceivedCallback(union ConcentratorPacket* packet, int8_t rssi);
static void updateLcd(void);
static void addNewNode(struct AdcSensorNode* node);
static void updateNode(struct AdcSensorNode* node);
static uint8_t isKnownNodeAddress(uint8_t address);
static void displayClockCb(UArg arg0);

/***** Function definitions *****/
void ConcentratorTask_init(void) {
    /* Create event used internally for state changes */
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&concentratorEvent, &eventParam);
    concentratorEventHandle = Event_handle(&concentratorEvent);

    /* Create the concentrator radio protocol task */
    Task_Params_init(&concentratorTaskParams);
    concentratorTaskParams.stackSize = CONCENTRATOR_TASK_STACK_SIZE;
    concentratorTaskParams.priority = CONCENTRATOR_TASK_PRIORITY;
    concentratorTaskParams.stack = &concentratorTaskStack;
    Task_construct(&concentratorTask, concentratorTaskFunction, &concentratorTaskParams, NULL);

    /* Create Clock to update LCD */
    Clock_Params displayClkParams;
    Clock_Params_init(&displayClkParams);
    displayClkParams.startFlag = FALSE;
    Clock_construct(&displayClock, displayClockCb, 1, &displayClkParams);
    displayClockHandle = Clock_handle(&displayClock);

    prevTicks = Clock_getTicks();
}

static void concentratorTaskFunction(UArg arg0, UArg arg1)
{
    /* Initialize display */
    Display_Params params;
    Display_Params_init(&params);
    params.lineClearMode = DISPLAY_CLEAR_BOTH;

    /* Open both an available LCD display */
    hDisplayLcd = Display_open(Display_Type_LCD, &params);

    /* Check if the selected Display type was found and successfully opened */
    if (hDisplayLcd)
    {
        Display_printf(hDisplayLcd, 0, 0, "Starting...");
    }

    /* Register a packet received callback with the radio task */
    ConcentratorRadioTask_registerPacketReceivedCallback(packetReceivedCallback);

    Clock_setTimeout(displayClockHandle,
                    CONCENTRATOR_DISPLAY_UPDATE_MS * 1000 / Clock_tickPeriod);
    Clock_start(displayClockHandle);

    /* Enter main task loop */
    while(1) {
        /* Wait for event */
        uint32_t events = Event_pend(concentratorEventHandle, 0, CONCENTRATOR_EVENT_ALL, BIOS_WAIT_FOREVER);

        /* If we got a new ADC sensor value */
        if(events & CONCENTRATOR_EVENT_NEW_ADC_SENSOR_VALUE) {
            /* If we knew this node from before, update the value */
            if(isKnownNodeAddress(latestActiveAdcSensorNode.address)) {
                updateNode(&latestActiveAdcSensorNode);
            }
            else {
                /* Else add it */
                latestActiveAdcSensorNode.receivedPackets = 1;
                addNewNode(&latestActiveAdcSensorNode);
            }
        }
        if(events & CONCENTRATOR_EVENT_UPDATE_DISPLAY) {
            updateLcd();
        }
    }
}

static void packetReceivedCallback(union ConcentratorPacket* packet, int8_t rssi)
{
    /* If we received an DualMode ADC sensor packet*/
    if(packet->header.packetType == RADIO_PACKET_TYPE_DM_SENSOR_PACKET)
    {
        /* Save the values */
        latestActiveAdcSensorNode.address = packet->header.sourceAddress;
        latestActiveAdcSensorNode.latestTemp2Value = packet->dmSensorPacket.temp2;
        latestActiveAdcSensorNode.latestTemp1Value = packet->dmSensorPacket.temp1;
        latestActiveAdcSensorNode.latestBatt = packet->dmSensorPacket.batt;
        latestActiveAdcSensorNode.latestRssi = rssi;

        Event_post(concentratorEventHandle, CONCENTRATOR_EVENT_NEW_ADC_SENSOR_VALUE);
    }
}

static uint8_t isKnownNodeAddress(uint8_t address) {
    uint8_t found = 0;
    uint8_t i;
    for (i = 0; i < CONCENTRATOR_MAX_NODES; i++)
    {
        if (knownSensorNodes[i].address == address)
        {
            found = 1;
            break;
        }
    }
    return found;
}

static void updateNode(struct AdcSensorNode* node) {
    uint8_t i;
    for (i = 0; i < CONCENTRATOR_MAX_NODES; i++) {
        if (knownSensorNodes[i].address == node->address)
        {
            knownSensorNodes[i].latestTemp2Value = node->latestTemp2Value;
            knownSensorNodes[i].latestTemp1Value = node->latestTemp1Value;
            knownSensorNodes[i].latestBatt = node->latestBatt;
            knownSensorNodes[i].latestRssi = node->latestRssi;
            knownSensorNodes[i].timeForLastRX = 0;
            knownSensorNodes[i].receivedPackets += 1;
            break;
        }
    }
}

static void addNewNode(struct AdcSensorNode* node) {
    *lastAddedSensorNode = *node;
    /* Increment and wrap */
    lastAddedSensorNode++;
    if (lastAddedSensorNode > &knownSensorNodes[CONCENTRATOR_MAX_NODES-1])
    {
        lastAddedSensorNode = knownSensorNodes;
    }
}

static void updateLcd(void) {
    struct AdcSensorNode* nodePointer = knownSensorNodes;
    uint8_t currentLcdLine;

    Display_clear(hDisplayLcd);
    Display_printf(hDisplayLcd, 0, 0, "Uptime: %is", upTime);

    /* Start on the third line */
    currentLcdLine = 2;

    uint32_t currentTicks = Clock_getTicks();

    while ((nodePointer < &knownSensorNodes[CONCENTRATOR_MAX_NODES]) &&
          (nodePointer->address != 0) )
    {
        double temp2Formatted = FIXED2DOUBLE(nodePointer->latestTemp2Value);
        if (temp2Formatted > 128.0) {
            temp2Formatted = temp2Formatted - 256.0; //display negative temperature correct
        }
        double temp1Formatted = FIXED2DOUBLE(nodePointer->latestTemp1Value);
        if (temp1Formatted > 128.0) {
            temp1Formatted = temp1Formatted - 256.0; //display negative temperature correct
        }

        if (currentTicks > prevTicks) {
            nodePointer->timeForLastRX += ((currentTicks - prevTicks) * Clock_tickPeriod) / 1000000;
        } else {
            nodePointer->timeForLastRX += ((prevTicks - currentTicks) * Clock_tickPeriod) / 1000000;
        }

        /* print to LCD */
        Display_printf(hDisplayLcd, currentLcdLine, 0, "NodeID: 0x%02x", nodePointer->address);
        currentLcdLine++;
        Display_printf(hDisplayLcd, currentLcdLine, 0, "Temp1: %3.3f", temp1Formatted);
        currentLcdLine++;
        Display_printf(hDisplayLcd, currentLcdLine, 0, "Temp2: %3.3f", temp2Formatted);
        currentLcdLine++;
        Display_printf(hDisplayLcd, currentLcdLine, 0, "Batt: %i", nodePointer->latestBatt);
        currentLcdLine++;
        Display_printf(hDisplayLcd, currentLcdLine, 0, "RSSI: %04d", nodePointer->latestRssi);
        currentLcdLine++;
        Display_printf(hDisplayLcd, currentLcdLine, 0, "Time: %is", nodePointer->timeForLastRX);
        currentLcdLine++;
        Display_printf(hDisplayLcd, currentLcdLine, 0, "Packets: %i", nodePointer->receivedPackets);
        currentLcdLine++;
        currentLcdLine++;

        nodePointer++;
    }

    if (currentTicks > prevTicks) {
        upTime += ((currentTicks - prevTicks) * Clock_tickPeriod) / 1000000;
    } else {
        upTime += ((prevTicks - currentTicks) * Clock_tickPeriod) / 1000000;
    }

    prevTicks = currentTicks;

}

static void displayClockCb(UArg arg0) {
    Event_post(concentratorEventHandle, CONCENTRATOR_EVENT_UPDATE_DISPLAY);

    Clock_setTimeout(displayClockHandle,
                    CONCENTRATOR_DISPLAY_UPDATE_MS * 1000 / Clock_tickPeriod);
    Clock_start(displayClockHandle);
}

#ifdef FEATURE_BLE_ADV
void rfSwitchCallback(RF_Handle h, RF_ClientEvent event, void* arg){
#if defined(RF_SW_PWR_PIN)
    //Turn on switch
    PIN_setOutputValue(blePinHandle, RF_SW_PWR_PIN, 1);
#endif
    PIN_setOutputValue(blePinHandle, RF_SWITCH_PIN, 1);
}
#endif
