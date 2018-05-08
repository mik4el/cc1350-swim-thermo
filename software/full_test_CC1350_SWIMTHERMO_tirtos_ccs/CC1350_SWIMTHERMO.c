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
 *  ============================ CC1350_LAUNCHXL.c ============================
 *  This file is responsible for setting up the board specific items for the
 *  CC1350_LAUNCHXL board.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ti/devices/cc13x0/driverlib/ioc.h>
#include <ti/devices/cc13x0/driverlib/udma.h>
#include <ti/devices/cc13x0/inc/hw_ints.h>
#include <ti/devices/cc13x0/inc/hw_memmap.h>

#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

#include "CC1350_SWIMTHERMO.h"

/*
 *  =============================== GPIO ===============================
 */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

/*
 * Array of Pin configurations
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in CC1350_LAUNCHXL.h
 * NOTE: Pins not used for interrupts should be placed at the end of the
 *       array.  Callback entries can be omitted from callbacks array to
 *       reduce memory usage.
 */

GPIO_PinConfig gpioPinConfigs[] = {
    /* Output pins */
    GPIOCC26XX_DIO_02 | GPIO_DO_NOT_CONFIG,  /* PIN_PSU_ENABLE */
};

/*
 * Array of callback function pointers
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in CC1350_LAUNCHXL.h
 * NOTE: Pins not used for interrupts can be omitted from callbacks array to
 *       reduce memory usage (if placed at end of gpioPinConfigs array).
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {};

const GPIOCC26XX_Config GPIOCC26XX_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = sizeof(gpioPinConfigs)/sizeof(GPIO_PinConfig),
    .numberOfCallbacks = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn),
    .intPriority = (~0)
};

/*
 *  =============================== PIN ===============================
 */
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

const PIN_Config BoardGpioInitTable[] = {

    CC1350_SWIMTHERMO_DIO1_RF_SUB1GHZ | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX, /* RF SW Switch defaults to 2.4 GHz path */
    CC1350_SWIMTHERMO_DIO0_RF_POWER | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX, /* External RF Switch is powered off by default */
    CC1350_SWIMTHERMO_DIO2_PSU_ENABLE | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX, /* LED PSU initially off */
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
 *  =============================== UDMA ===============================
 */
#include <ti/drivers/dma/UDMACC26XX.h>

UDMACC26XX_Object udmaObjects[CC1350_SWIMTHERMO_UDMACOUNT];

const UDMACC26XX_HWAttrs udmaHWAttrs[CC1350_SWIMTHERMO_UDMACOUNT] = {
    {
        .baseAddr    = UDMA0_BASE,
        .powerMngrId = PowerCC26XX_PERIPH_UDMA,
        .intNum      = INT_DMA_ERR,
        .intPriority = ~0
    }
};

const UDMACC26XX_Config UDMACC26XX_config[CC1350_SWIMTHERMO_UDMACOUNT] = {
    {
         .object  = &udmaObjects[CC1350_SWIMTHERMO_UDMA0],
         .hwAttrs = &udmaHWAttrs[CC1350_SWIMTHERMO_UDMA0]
    },
};

/*
 *  =============================== SPI DMA ===============================
 */
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26XXDMA.h>

SPICC26XXDMA_Object spiCC26XXDMAObjects[CC1350_SWIMTHERMO_SPICOUNT];

/*
 * NOTE: The SPI instances below can be used by the SD driver to communicate
 * with a SD card via SPI.  The 'defaultTxBufValue' fields below are set to 0xFF
 * to satisfy the SDSPI driver requirement.
 */
const SPICC26XXDMA_HWAttrsV1 spiCC26XXDMAHWAttrs[CC1350_SWIMTHERMO_SPICOUNT] = {
    {
        .baseAddr           = SSI0_BASE,
        .intNum             = INT_SSI0_COMB,
        .intPriority        = ~0,
        .swiPriority        = 0,
        .powerMngrId        = PowerCC26XX_PERIPH_SSI0,
        .defaultTxBufValue  = 0xFF,
        .rxChannelBitMask   = 1<<UDMA_CHAN_SSI0_RX,
        .txChannelBitMask   = 1<<UDMA_CHAN_SSI0_TX,
        .mosiPin            = CC1350_SWIMTHERMO_SPI0_MOSI,
        .misoPin            = CC1350_SWIMTHERMO_SPI0_MISO,
        .clkPin             = CC1350_SWIMTHERMO_SPI0_CLK,
        .csnPin             = CC1350_SWIMTHERMO_SPI0_CSN,
        .minDmaTransferSize = 10
    },
    {
        .baseAddr           = SSI1_BASE,
        .intNum             = INT_SSI1_COMB,
        .intPriority        = ~0,
        .swiPriority        = 0,
        .powerMngrId        = PowerCC26XX_PERIPH_SSI1,
        .defaultTxBufValue  = 0xFF,
        .rxChannelBitMask   = 1<<UDMA_CHAN_SSI1_RX,
        .txChannelBitMask   = 1<<UDMA_CHAN_SSI1_TX,
        .mosiPin            = CC1350_SWIMTHERMO_SPI1_MOSI,
        .misoPin            = CC1350_SWIMTHERMO_SPI1_MISO,
        .clkPin             = CC1350_SWIMTHERMO_SPI1_CLK,
        .csnPin             = CC1350_SWIMTHERMO_SPI1_CSN,
        .minDmaTransferSize = 10
    }
};

const SPI_Config SPI_config[CC1350_SWIMTHERMO_SPICOUNT] = {
    {
         .fxnTablePtr = &SPICC26XXDMA_fxnTable,
         .object      = &spiCC26XXDMAObjects[CC1350_SWIMTHERMO_SPI0],
         .hwAttrs     = &spiCC26XXDMAHWAttrs[CC1350_SWIMTHERMO_SPI0]
    },
    {
         .fxnTablePtr = &SPICC26XXDMA_fxnTable,
         .object      = &spiCC26XXDMAObjects[CC1350_SWIMTHERMO_SPI1],
         .hwAttrs     = &spiCC26XXDMAHWAttrs[CC1350_SWIMTHERMO_SPI1]
    },
};

const uint_least8_t SPI_count = CC1350_SWIMTHERMO_SPICOUNT;

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
 *  ======== CC1350_SWIMTHERMO_SPI0_MOSI_initGeneral ========
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
 * ======== CC1350_SWIMTHERMO_SPI0_MOSI_rfDriverCallback ========
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
