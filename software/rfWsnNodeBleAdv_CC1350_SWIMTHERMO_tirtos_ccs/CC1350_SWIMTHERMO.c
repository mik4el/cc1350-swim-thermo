/*
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
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
 *  ============================ CC1350_SWIMTHERMO.c ============================
 *  This file is responsible for setting up the board specific items for the
 *  CC1350_SWIMTHERMO board.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "CC1350_SWIMTHERMO.h"


/*
 *  =============================== PIN ===============================
 */
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

const PIN_Config BoardGpioInitTable[] = {

    CC1350_SWIMTHERMO_DIO1_RF_SUB1GHZ | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX, /* RF SW Switch defaults to 2.4 GHz path */
    CC1350_SWIMTHERMO_DIO0_RF_POWER | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX, /* External RF Switch is powered off by default */
    CC1350_SWIMTHERMO_DIO2_PSU_ENABLE | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX, /* LED PSU initially off */
    CC1350_SWIMTHERMO_DIO8_BUTTON | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_BOTHEDGES | PIN_HYSTERESIS, /* Button is active low */
    PIN_TERMINATE
};


const PINCC26XX_HWAttrs PINCC26XX_hwAttrs = {
    .intPriority = ~0,
    .swiPriority = 0
};

/*
 *  =============================== Power ===============================
 */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>

const PowerCC26XX_Config PowerCC26XX_config = {
    .policyInitFxn      = NULL,
    .policyFxn          = &PowerCC26XX_standbyPolicy,
    .calibrateFxn       = &PowerCC26XX_calibrate,
    .enablePolicy       = true,
    .calibrateRCOSC_LF  = true,
    .calibrateRCOSC_HF  = true,
};

/*
 *  =============================== RF Driver ===============================
 */
#include <ti/drivers/rf/RF.h>

/*
 * Board-specific callback function to set the correct antenna path.
 *
 * This function is called by the RF driver on global driver events. It contains
 * a default implementation to set the correct antenna path.
 */
static void CC1350_SWIMTHERMO_rfDriverCallback(RF_Handle client, RF_GlobalEvent events, void *arg);

const RFCC26XX_HWAttrsV2 RFCC26XX_hwAttrs = {
    .hwiPriority        = ~0,                                                       /* Lowest HWI priority */
    .swiPriority        = 0,                                                        /* Lowest SWI priority */
    .xoscHfAlwaysNeeded = true,                                                     /* Keep XOSC dependency while in stanby */
    .globalCallback     = &CC1350_SWIMTHERMO_rfDriverCallback,                        /* Register the board specific callback */
    .globalEventMask    = RF_GlobalEventRadioSetup | RF_GlobalEventRadioPowerDown   /* Subscribe the callback to both events */
};

/*
 *  ======== CC1350_SWIMTHERMO_initGeneral ========
 */
void CC1350_SWIMTHERMO_initGeneral(void)
{
    Power_init();

    if (PIN_init(BoardGpioInitTable) != PIN_SUCCESS) {
        /* Error with PIN_init */
        while (1);
    }
}

/*
 * ======== CC1350_SWIMTHERMO_rfDriverCallback ========
 * This is an implementation for the CC1350 launchpad which uses a
 * single signal for antenna switching.
 */
void CC1350_SWIMTHERMO_rfDriverCallback(RF_Handle client, RF_GlobalEvent events, void *arg)
{
    (void)client;
    RF_RadioSetup* setupCommand = (RF_RadioSetup*)arg;

    if (events & RF_GlobalEventRadioSetup) {
        /* Power up the antenna switch */
        PINCC26XX_setOutputValue(CC1350_SWIMTHERMO_DIO0_RF_POWER, 1);

        if (setupCommand->common.commandNo == CMD_PROP_RADIO_DIV_SETUP) {
            /* Sub-1 GHz, requires antenna switch high */
            PINCC26XX_setOutputValue(CC1350_SWIMTHERMO_DIO1_RF_SUB1GHZ, 1);
        }

    } else if (events & RF_GlobalEventRadioPowerDown) {
        /* Disable antenna switch to save current */
        PINCC26XX_setOutputValue(CC1350_SWIMTHERMO_DIO0_RF_POWER, 0);
        PINCC26XX_setOutputValue(CC1350_SWIMTHERMO_DIO1_RF_SUB1GHZ, 0);
    }
}
