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

/* TI-RTOS Header files */ 
#include <ti/drivers/PIN.h>

/* Board Header files */
#include "Board.h"

/* Application Header files */ 
#include "SceAdc.h"
#include "NodeTask.h"
#include "NodeRadioTask.h"

#ifdef FEATURE_BLE_ADV
#include "ble_adv/BleAdv.h"
#endif

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aon_batmon.h)
#include DeviceFamily_constructPath(driverlib/aux_adc.h) //for ADC operations

/***** Defines *****/
#define NODE_TASK_STACK_SIZE 1024
#define NODE_TASK_PRIORITY   3

#define NODE_EVENT_ALL                  0xFFFFFFFF
#define NODE_EVENT_NEW_ADC_VALUE    (uint32_t)(1 << 0)
#define NODE_EVENT_UPDATE_LCD       (uint32_t)(1 << 1)

#define NUM_EDDYSTONE_URLS      5

/***** Variable declarations *****/
static Task_Params nodeTaskParams;
Task_Struct nodeTask;    /* Not static so you can see in ROV */
static uint8_t nodeTaskStack[NODE_TASK_STACK_SIZE];
Event_Struct nodeEvent;  /* Not static so you can see in ROV */
static Event_Handle nodeEventHandle;
static uint16_t latestAdcValue;

/* Pin driver handle */
static PIN_Handle buttonPinHandle;
static PIN_Handle ledPinHandle;
static PIN_State buttonPinState;
static PIN_State ledPinState;


#ifdef FEATURE_BLE_ADV
static BleAdv_AdertiserType advertisementType = BleAdv_AdertiserNone;
#endif

/* Enable the 3.3V power domain used by the LCD */
PIN_Config pinTable[] = {
                         CC1350_SWIMTHERMO_DIO5_T_ON1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL, /* T_ON1 should be default high */
                         CC1350_SWIMTHERMO_DIO6_T_ON2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL, /* T_ON2 should be default low */
                         PIN_TERMINATE
                        };

/*
 * Application button pin configuration table:
 *   - Buttons interrupts are configured to trigger on falling edge.
 */
PIN_Config buttonPinTable[] = {
#ifdef FEATURE_BLE_ADV
    CC1350_SWIMTHERMO_DIO8_BUTTON | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
#endif
    PIN_TERMINATE
};

#ifdef FEATURE_BLE_ADV
static BleAdv_Stats bleAdvStats = {0};
#endif

/***** Prototypes *****/
static void nodeTaskFunction(UArg arg0, UArg arg1);
static void adcCallback(uint16_t adcValue);
static void buttonCallback(PIN_Handle handle, PIN_Id pinId);


/***** Function definitions *****/
void NodeTask_init(void)
{

    /* Create event used internally for state changes */
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&nodeEvent, &eventParam);
    nodeEventHandle = Event_handle(&nodeEvent);

    /* Create the node task */
    Task_Params_init(&nodeTaskParams);
    nodeTaskParams.stackSize = NODE_TASK_STACK_SIZE;
    nodeTaskParams.priority = NODE_TASK_PRIORITY;
    nodeTaskParams.stack = &nodeTaskStack;
    Task_construct(&nodeTask, nodeTaskFunction, &nodeTaskParams, NULL);
}

#ifdef FEATURE_BLE_ADV
void NodeTask_advStatsCB(BleAdv_Stats stats)
{
    memcpy(&bleAdvStats, &stats, sizeof(BleAdv_Stats));
}
#endif

static void nodeTaskFunction(UArg arg0, UArg arg1)
{
    /* Open LED pins */
    ledPinHandle = PIN_open(&ledPinState, pinTable);
    if (!ledPinHandle)
    {
        System_abort("Error initializing board 3.3V domain pins\n");
    }

    /* Start the SCE ADC task */
    SceAdc_init();
    SceAdc_registerAdcCallback(adcCallback);
    SceAdc_start();

    buttonPinHandle = PIN_open(&buttonPinState, buttonPinTable);
    if (!buttonPinHandle)
    {
        System_abort("Error initializing button pins\n");
    }

    /* Setup callback for button pins */
    if (PIN_registerIntCb(buttonPinHandle, &buttonCallback) != 0)
    {
        System_abort("Error registering button callback function");
    }

    while (1)
    {
        /* Wait for event */
        uint32_t events = Event_pend(nodeEventHandle, 0, NODE_EVENT_ALL, BIOS_WAIT_FOREVER);

        /* If new ADC value, send this data */
        if (events & NODE_EVENT_NEW_ADC_VALUE) {
            /* Send ADC value to concentrator */
            NodeRadioTask_sendAdcData(latestAdcValue);
        }
    }
}

static void adcCallback(uint16_t adcValue)
{
    /* Calibrate and save latest values */
    uint32_t calADC12_gain = AUXADCGetAdjustmentGain(AUXADC_REF_FIXED);
    int8_t calADC12_offset = AUXADCGetAdjustmentOffset(AUXADC_REF_FIXED);
    latestAdcValue = AUXADCAdjustValueForGainAndOffset(adcValue, calADC12_gain, calADC12_offset);
    /* Post event */
    Event_post(nodeEventHandle, NODE_EVENT_NEW_ADC_VALUE);
}

/*
 *  ======== buttonCallback ========
 *  Pin interrupt Callback function board buttons configured in the pinTable.
 */
static void buttonCallback(PIN_Handle handle, PIN_Id pinId)
{
    /* Debounce logic, only toggle if the button is still pushed (low) */
    CPUdelay(8000*50);


#ifdef FEATURE_BLE_ADV
    if (PIN_getInputValue(CC1350_SWIMTHERMO_DIO8_BUTTON) == 0)
    {
        if (advertisementType == BleAdv_AdertiserUrl)
        {
            advertisementType = BleAdv_AdertiserNone;
        } else {
            advertisementType = BleAdv_AdertiserUrl;
        }

        //Set advertisement type
        BleAdv_setAdvertiserType(advertisementType);

        /* update display */
        Event_post(nodeEventHandle, NODE_EVENT_UPDATE_LCD);
    }
#endif
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
