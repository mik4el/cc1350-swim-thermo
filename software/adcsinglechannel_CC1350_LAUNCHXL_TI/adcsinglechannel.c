/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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

/*
 *  ======== adcsinglechannel.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* Driver Header files */
#include <ti/drivers/ADC.h>
#if defined(CC2650DK_7ID) || defined(CC1310DK_7XD)
#include <ti/drivers/PIN.h>
#endif

/* Example/Board Header files */
#include "Board.h"

/* ADC sample count */
#define ADC_SAMPLE_COUNT  (10)

/*Task Specific defines */
#define TASKSTACKSIZE     (768)

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

Task_Struct task1Struct;
Char task1Stack[TASKSTACKSIZE];

/* ADC conversion result variables */
uint16_t adcValue0;
uint16_t adcValue1[ADC_SAMPLE_COUNT];

/*
 *  ======== taskFxn0 ========
 *  Open an ADC instance and get a sampling result from a one-shot conversion.
 */
Void taskFxn0(UArg arg0, UArg arg1)
{
    ADC_Handle   adc;
    ADC_Params   params;
    int_fast16_t res;

    ADC_Params_init(&params);
    adc = ADC_open(Board_ADC0, &params);

    if (adc == NULL) {
        System_abort("Error initializing ADC channel 0\n");
    }
    else {
        System_printf("ADC channel 0 initialized\n");
    }

    /* Blocking mode conversion */
    res = ADC_convert(adc, &adcValue0);

    if (res == ADC_STATUS_SUCCESS) {
        System_printf("ADC channel 0 convert result: 0x%x\n", adcValue0);
    }
    else {
        System_printf("ADC channel 0 convert failed\n");
    }

    ADC_close(adc);

    System_flush();
}

/*
 *  ======== taskFxn1 ========
 *  Open a ADC handle and get a array of sampling results after
 *  calling several conversions.
 */
Void taskFxn1(UArg arg0, UArg arg1)
{
    uint16_t     i;
    ADC_Handle   adc;
    ADC_Params   params;
    int_fast16_t res;

    ADC_Params_init(&params);
    adc = ADC_open(Board_ADC1, &params);

    if (adc == NULL) {
        System_abort("Error initializing ADC channel 1\n");
    }
    else {
        System_printf("ADC channel 1 initialized\n");
    }

    for (i = 0; i < ADC_SAMPLE_COUNT; i++) {
        res = ADC_convert(adc, &adcValue1[i]);

        if (res == ADC_STATUS_SUCCESS) {
            System_printf("ADC channel 1 convert result (%d): 0x%x\n", i,
                adcValue1[i]);
        }
        else {
            System_printf("ADC channel 1 convert failed (%d)\n", i);
        }

        System_flush();
    }

    ADC_close(adc);
}

/*
 *  ======== main ========
 */
int main(void)
{
    Task_Params taskParams;

    /* Call board init functions */
    Board_initGeneral();
    Board_initADC();

    /* Create tasks */
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)taskFxn0, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task1Stack;
    Task_construct(&task1Struct, (Task_FuncPtr)taskFxn1, &taskParams, NULL);

/*
 * The CC2650DK_7ID and CC1310DK_7XD measure an ambient light sensor in this example.
 * It is not powered by default to avoid high current consumption in other examples.
 * The code below turns on the power to the sensor.
 */
#if defined(CC2650DK_7ID) || defined(CC1310DK_7XD)
    PIN_State pinState;

    PIN_Config AlsPinTable[] =
    {
        Board_ALS_PWR    | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH    | PIN_PUSHPULL, /* Turn on ALS power */
        PIN_TERMINATE                                                            /* Terminate list */
    };

    /* Turn on the power to the ambient light sensor */
    PIN_open(&pinState, AlsPinTable);
#endif

    System_printf("Starting the ADC Single Channel example\nSystem provider is "
        "set to SysMin.  Halt the target to view any SysMin contents in ROV.\n");

    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    BIOS_start();

    return (0);
}
