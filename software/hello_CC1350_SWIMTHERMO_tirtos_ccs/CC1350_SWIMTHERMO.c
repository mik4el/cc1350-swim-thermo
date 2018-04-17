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

#include <CC1350_SWIMTHERMO.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ti/devices/cc13x0/driverlib/ioc.h>
#include <ti/devices/cc13x0/driverlib/udma.h>
#include <ti/devices/cc13x0/inc/hw_ints.h>
#include <ti/devices/cc13x0/inc/hw_memmap.h>

#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/adcbuf/ADCBufCC26XX.h>

ADCBufCC26XX_Object adcBufCC26XXobjects[CC1350_SWIMTHERMO_ADCBUFCOUNT];

/*
 *  This table converts a virtual adc channel into a dio and internal analogue
 *  input signal. This table is necessary for the functioning of the adcBuf
 *  driver. Comment out unused entries to save flash. Dio and internal signal
 *  pairs are hardwired. Do not remap them in the table. You may reorder entire
 *  entries. The mapping of dio and internal signals is package dependent.
 */
const ADCBufCC26XX_AdcChannelLutEntry ADCBufCC26XX_adcChannelLut[CC1350_SWIMTHERMO_ADCBUF0CHANNELCOUNT] = {
    {CC1350_SWIMTHERMO_DIO7_ANALOG, ADC_COMPB_IN_AUXIO5},
    {PIN_UNASSIGNED, ADC_COMPB_IN_VDDS},
    {PIN_UNASSIGNED, ADC_COMPB_IN_DCOUPL},
    {PIN_UNASSIGNED, ADC_COMPB_IN_VSS},
};

const ADCBufCC26XX_HWAttrs adcBufCC26XXHWAttrs[CC1350_SWIMTHERMO_ADCBUFCOUNT] = {
    {
        .intPriority       = ~0,
        .swiPriority       = 0,
        .adcChannelLut     = ADCBufCC26XX_adcChannelLut,
        .gpTimerUnit       = CC1350_SWIMTHERMO_GPTIMER0A,
        .gptDMAChannelMask = 1 << UDMA_CHAN_TIMER0_A,
    }
};

const ADCBuf_Config ADCBuf_config[CC1350_SWIMTHERMO_ADCBUFCOUNT] = {
    {
        &ADCBufCC26XX_fxnTable,
        &adcBufCC26XXobjects[CC1350_SWIMTHERMO_ADCBUF0],
        &adcBufCC26XXHWAttrs[CC1350_SWIMTHERMO_ADCBUF0]
    },
};

const uint_least8_t ADCBuf_count = CC1350_SWIMTHERMO_ADCBUFCOUNT;

/*
 *  =============================== ADC ===============================
 */
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCCC26XX.h>

ADCCC26XX_Object adcCC26xxObjects[CC1350_SWIMTHERMO_ADCCOUNT];

const ADCCC26XX_HWAttrs adcCC26xxHWAttrs[CC1350_SWIMTHERMO_ADCCOUNT] = {
    {
        .adcDIO              = CC1350_SWIMTHERMO_DIO7_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO5,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_DCOUPL,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_VSS,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_VDDS,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    }
};

const ADC_Config ADC_config[CC1350_SWIMTHERMO_ADCCOUNT] = {
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC1350_SWIMTHERMO_ADC0], &adcCC26xxHWAttrs[CC1350_SWIMTHERMO_ADC0]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC1350_SWIMTHERMO_ADCDCOUPL], &adcCC26xxHWAttrs[CC1350_SWIMTHERMO_ADCDCOUPL]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC1350_SWIMTHERMO_ADCVSS], &adcCC26xxHWAttrs[CC1350_SWIMTHERMO_ADCVSS]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC1350_SWIMTHERMO_ADCVDDS], &adcCC26xxHWAttrs[CC1350_SWIMTHERMO_ADCVDDS]},
};

const uint_least8_t ADC_count = CC1350_SWIMTHERMO_ADCCOUNT;

/*
 *  =============================== Crypto ===============================
 */
#include <ti/drivers/crypto/CryptoCC26XX.h>

CryptoCC26XX_Object cryptoCC26XXObjects[CC1350_SWIMTHERMO_CRYPTOCOUNT];

const CryptoCC26XX_HWAttrs cryptoCC26XXHWAttrs[CC1350_SWIMTHERMO_CRYPTOCOUNT] = {
    {
        .baseAddr       = CRYPTO_BASE,
        .powerMngrId    = PowerCC26XX_PERIPH_CRYPTO,
        .intNum         = INT_CRYPTO_RESULT_AVAIL_IRQ,
        .intPriority    = ~0,
    }
};

const CryptoCC26XX_Config CryptoCC26XX_config[CC1350_SWIMTHERMO_CRYPTOCOUNT] = {
    {
         .object  = &cryptoCC26XXObjects[CC1350_SWIMTHERMO_CRYPTO0],
         .hwAttrs = &cryptoCC26XXHWAttrs[CC1350_SWIMTHERMO_CRYPTO0]
    },
};

/*
 *  =============================== GPIO ===============================
 */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

/*
 * Array of Pin configurations
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in CC1350_SWIMTHERMO.h
 * NOTE: Pins not used for interrupts should be placed at the end of the
 *       array.  Callback entries can be omitted from callbacks array to
 *       reduce memory usage.
 */
GPIO_PinConfig gpioPinConfigs[] = {
    GPIOCC26XX_DIO_02  | GPIO_DO_NOT_CONFIG,  /* PSU_ENABLE */
    GPIOCC26XX_DIO_05  | GPIO_DO_NOT_CONFIG,  /* T_ON1 */
    GPIOCC26XX_DIO_06  | GPIO_DO_NOT_CONFIG,  /* T_ON2 */
    GPIOCC26XX_DIO_08  | GPIO_DO_NOT_CONFIG,  /* BTN */
    GPIOCC26XX_DIO_09  | GPIO_DO_NOT_CONFIG,  /* LED */
};

/*
 * Array of callback function pointers
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in CC1350_SWIMTHERMO.h
 * NOTE: Pins not used for interrupts can be omitted from callbacks array to
 *       reduce memory usage (if placed at end of gpioPinConfigs array).
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL, /* BTN */
};

const GPIOCC26XX_Config GPIOCC26XX_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = sizeof(gpioPinConfigs)/sizeof(GPIO_PinConfig),
    .numberOfCallbacks = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn),
    .intPriority = (~0)
};

/*
 *  =============================== GPTimer ===============================
 *  Remove unused entries to reduce flash usage both in Board.c and Board.h
 */
#include <ti/drivers/timer/GPTimerCC26XX.h>

GPTimerCC26XX_Object gptimerCC26XXObjects[CC1350_SWIMTHERMO_GPTIMERCOUNT];

const GPTimerCC26XX_HWAttrs gptimerCC26xxHWAttrs[CC1350_SWIMTHERMO_GPTIMERPARTSCOUNT] = {
    { .baseAddr = GPT0_BASE, .intNum = INT_GPT0A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT0, .pinMux = GPT_PIN_0A, },
    { .baseAddr = GPT0_BASE, .intNum = INT_GPT0B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT0, .pinMux = GPT_PIN_0B, },
    { .baseAddr = GPT1_BASE, .intNum = INT_GPT1A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT1, .pinMux = GPT_PIN_1A, },
    { .baseAddr = GPT1_BASE, .intNum = INT_GPT1B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT1, .pinMux = GPT_PIN_1B, },
    { .baseAddr = GPT2_BASE, .intNum = INT_GPT2A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT2, .pinMux = GPT_PIN_2A, },
    { .baseAddr = GPT2_BASE, .intNum = INT_GPT2B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT2, .pinMux = GPT_PIN_2B, },
    { .baseAddr = GPT3_BASE, .intNum = INT_GPT3A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT3, .pinMux = GPT_PIN_3A, },
    { .baseAddr = GPT3_BASE, .intNum = INT_GPT3B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT3, .pinMux = GPT_PIN_3B, },
};

const GPTimerCC26XX_Config GPTimerCC26XX_config[CC1350_SWIMTHERMO_GPTIMERPARTSCOUNT] = {
    { &gptimerCC26XXObjects[CC1350_SWIMTHERMO_GPTIMER0], &gptimerCC26xxHWAttrs[CC1350_SWIMTHERMO_GPTIMER0A], GPT_A },
    { &gptimerCC26XXObjects[CC1350_SWIMTHERMO_GPTIMER0], &gptimerCC26xxHWAttrs[CC1350_SWIMTHERMO_GPTIMER0B], GPT_B },
    { &gptimerCC26XXObjects[CC1350_SWIMTHERMO_GPTIMER1], &gptimerCC26xxHWAttrs[CC1350_SWIMTHERMO_GPTIMER1A], GPT_A },
    { &gptimerCC26XXObjects[CC1350_SWIMTHERMO_GPTIMER1], &gptimerCC26xxHWAttrs[CC1350_SWIMTHERMO_GPTIMER1B], GPT_B },
    { &gptimerCC26XXObjects[CC1350_SWIMTHERMO_GPTIMER2], &gptimerCC26xxHWAttrs[CC1350_SWIMTHERMO_GPTIMER2A], GPT_A },
    { &gptimerCC26XXObjects[CC1350_SWIMTHERMO_GPTIMER2], &gptimerCC26xxHWAttrs[CC1350_SWIMTHERMO_GPTIMER2B], GPT_B },
    { &gptimerCC26XXObjects[CC1350_SWIMTHERMO_GPTIMER3], &gptimerCC26xxHWAttrs[CC1350_SWIMTHERMO_GPTIMER3A], GPT_A },
    { &gptimerCC26XXObjects[CC1350_SWIMTHERMO_GPTIMER3], &gptimerCC26xxHWAttrs[CC1350_SWIMTHERMO_GPTIMER3B], GPT_B },
};

/*
 *  =============================== PIN ===============================
 */
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

const PIN_Config BoardGpioInitTable[] = {
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
 *  =============================== PWM ===============================
 *  Remove unused entries to reduce flash usage both in Board.c and Board.h
 */
#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTimerCC26XX.h>

PWMTimerCC26XX_Object pwmtimerCC26xxObjects[CC1350_SWIMTHERMO_PWMCOUNT];

const PWMTimerCC26XX_HwAttrs pwmtimerCC26xxHWAttrs[CC1350_SWIMTHERMO_PWMCOUNT] = {
    { .pwmPin = CC1350_SWIMTHERMO_PWMPIN0, .gpTimerUnit = CC1350_SWIMTHERMO_GPTIMER0A },
    { .pwmPin = CC1350_SWIMTHERMO_PWMPIN1, .gpTimerUnit = CC1350_SWIMTHERMO_GPTIMER0B },
    { .pwmPin = CC1350_SWIMTHERMO_PWMPIN2, .gpTimerUnit = CC1350_SWIMTHERMO_GPTIMER1A },
    { .pwmPin = CC1350_SWIMTHERMO_PWMPIN3, .gpTimerUnit = CC1350_SWIMTHERMO_GPTIMER1B },
    { .pwmPin = CC1350_SWIMTHERMO_PWMPIN4, .gpTimerUnit = CC1350_SWIMTHERMO_GPTIMER2A },
    { .pwmPin = CC1350_SWIMTHERMO_PWMPIN5, .gpTimerUnit = CC1350_SWIMTHERMO_GPTIMER2B },
    { .pwmPin = CC1350_SWIMTHERMO_PWMPIN6, .gpTimerUnit = CC1350_SWIMTHERMO_GPTIMER3A },
    { .pwmPin = CC1350_SWIMTHERMO_PWMPIN7, .gpTimerUnit = CC1350_SWIMTHERMO_GPTIMER3B },
};

const PWM_Config PWM_config[CC1350_SWIMTHERMO_PWMCOUNT] = {
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[CC1350_SWIMTHERMO_PWM0], &pwmtimerCC26xxHWAttrs[CC1350_SWIMTHERMO_PWM0] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[CC1350_SWIMTHERMO_PWM1], &pwmtimerCC26xxHWAttrs[CC1350_SWIMTHERMO_PWM1] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[CC1350_SWIMTHERMO_PWM2], &pwmtimerCC26xxHWAttrs[CC1350_SWIMTHERMO_PWM2] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[CC1350_SWIMTHERMO_PWM3], &pwmtimerCC26xxHWAttrs[CC1350_SWIMTHERMO_PWM3] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[CC1350_SWIMTHERMO_PWM4], &pwmtimerCC26xxHWAttrs[CC1350_SWIMTHERMO_PWM4] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[CC1350_SWIMTHERMO_PWM5], &pwmtimerCC26xxHWAttrs[CC1350_SWIMTHERMO_PWM5] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[CC1350_SWIMTHERMO_PWM6], &pwmtimerCC26xxHWAttrs[CC1350_SWIMTHERMO_PWM6] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[CC1350_SWIMTHERMO_PWM7], &pwmtimerCC26xxHWAttrs[CC1350_SWIMTHERMO_PWM7] },
};

const uint_least8_t PWM_count = CC1350_SWIMTHERMO_PWMCOUNT;

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
 *  =============================== UART ===============================
 */
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>

UARTCC26XX_Object uartCC26XXObjects[CC1350_SWIMTHERMO_UARTCOUNT];

uint8_t uartCC26XXRingBuffer[CC1350_SWIMTHERMO_UARTCOUNT][32];

const UARTCC26XX_HWAttrsV2 uartCC26XXHWAttrs[CC1350_SWIMTHERMO_UARTCOUNT] = {
    {
        .baseAddr       = UART0_BASE,
        .powerMngrId    = PowerCC26XX_PERIPH_UART0,
        .intNum         = INT_UART0_COMB,
        .intPriority    = ~0,
        .swiPriority    = 0,
        .txPin          = CC1350_SWIMTHERMO_UART_TX,
        .rxPin          = CC1350_SWIMTHERMO_UART_RX,
        .ctsPin         = PIN_UNASSIGNED,
        .rtsPin         = PIN_UNASSIGNED,
        .ringBufPtr     = uartCC26XXRingBuffer[CC1350_SWIMTHERMO_UART0],
        .ringBufSize    = sizeof(uartCC26XXRingBuffer[CC1350_SWIMTHERMO_UART0]),
        .txIntFifoThr   = UARTCC26XX_FIFO_THRESHOLD_1_8,
        .rxIntFifoThr   = UARTCC26XX_FIFO_THRESHOLD_4_8,
        .errorFxn       = NULL
    }
};

const UART_Config UART_config[CC1350_SWIMTHERMO_UARTCOUNT] = {
    {
        .fxnTablePtr = &UARTCC26XX_fxnTable,
        .object      = &uartCC26XXObjects[CC1350_SWIMTHERMO_UART0],
        .hwAttrs     = &uartCC26XXHWAttrs[CC1350_SWIMTHERMO_UART0]
    },
};

const uint_least8_t UART_count = CC1350_SWIMTHERMO_UARTCOUNT;

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
 *  =============================== Watchdog ===============================
 */
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogCC26XX.h>

WatchdogCC26XX_Object watchdogCC26XXObjects[CC1350_SWIMTHERMO_WATCHDOGCOUNT];

const WatchdogCC26XX_HWAttrs watchdogCC26XXHWAttrs[CC1350_SWIMTHERMO_WATCHDOGCOUNT] = {
    {
        .baseAddr    = WDT_BASE,
        .reloadValue = 1000 /* Reload value in milliseconds */
    },
};

const Watchdog_Config Watchdog_config[CC1350_SWIMTHERMO_WATCHDOGCOUNT] = {
    {
        .fxnTablePtr = &WatchdogCC26XX_fxnTable,
        .object      = &watchdogCC26XXObjects[CC1350_SWIMTHERMO_WATCHDOG0],
        .hwAttrs     = &watchdogCC26XXHWAttrs[CC1350_SWIMTHERMO_WATCHDOG0]
    },
};

const uint_least8_t Watchdog_count = CC1350_SWIMTHERMO_WATCHDOGCOUNT;

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
        PINCC26XX_setOutputValue(CC1350_SWIMTHERMO_DIO0_RF_SW_PWR, 1);

        if (setupCommand->common.commandNo == CMD_PROP_RADIO_DIV_SETUP) {
            /* Sub-1 GHz, requires antenna switch high */
            PINCC26XX_setOutputValue(CC1350_SWIMTHERMO_DIO1_RF_SW, 1);
        }

    } else if (events & RF_GlobalEventRadioPowerDown) {
        /* Disable antenna switch to save current */
        PINCC26XX_setOutputValue(CC1350_SWIMTHERMO_DIO0_RF_SW_PWR, 0);
        PINCC26XX_setOutputValue(CC1350_SWIMTHERMO_DIO1_RF_SW, 0);
    }
}
