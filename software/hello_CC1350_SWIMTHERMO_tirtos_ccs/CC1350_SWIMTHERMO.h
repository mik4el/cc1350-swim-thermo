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
/** ============================================================================
 *  @file       CC1350_SWIMTHERMO.h
 *
 *  @brief      CC1350 SWIMTHERMO from LaunchPad Board Specific header file.
 *
 *  The CC1350_SWIMTHERMO header file should be included in an application as
 *  follows:
 *  @code
 *  #include "CC1350_SWIMTHERMO.h"
 *  @endcode
 *
 *  ============================================================================
 */
#ifndef __CC1350_SWIMTHERMO_BOARD_H__
#define __CC1350_SWIMTHERMO_BOARD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <ti/drivers/PIN.h>
#include <ti/devices/cc13x0/driverlib/ioc.h>

/* Externs */
extern const PIN_Config BoardGpioInitTable[];

/* Defines */
#define CC1350_SWIMTHERMO

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>        <pin mapping>   <comments>
 */

/* IOs */
#define CC1350_SWIMTHERMO_DIO0_RF_SW_PWR        IOID_0
#define CC1350_SWIMTHERMO_DIO1_RF_SW            IOID_1
#define CC1350_SWIMTHERMO_DIO2                  IOID_2
#define CC1350_SWIMTHERMO_DIO3_TDO              IOID_3
#define CC1350_SWIMTHERMO_DIO4_TDI              IOID_4
#define CC1350_SWIMTHERMO_DIO5                  IOID_5
#define CC1350_SWIMTHERMO_DIO6                  IOID_6
#define CC1350_SWIMTHERMO_DIO7_ANALOG           IOID_7
#define CC1350_SWIMTHERMO_DIO8                  IOID_8
#define CC1350_SWIMTHERMO_DIO9                  IOID_9

/* PWM Outputs */
#define CC1350_SWIMTHERMO_PWMPIN0               PIN_UNASSIGNED
#define CC1350_SWIMTHERMO_PWMPIN1               PIN_UNASSIGNED
#define CC1350_SWIMTHERMO_PWMPIN2               PIN_UNASSIGNED
#define CC1350_SWIMTHERMO_PWMPIN3               PIN_UNASSIGNED
#define CC1350_SWIMTHERMO_PWMPIN4               PIN_UNASSIGNED
#define CC1350_SWIMTHERMO_PWMPIN5               PIN_UNASSIGNED
#define CC1350_SWIMTHERMO_PWMPIN6               PIN_UNASSIGNED
#define CC1350_SWIMTHERMO_PWMPIN7               PIN_UNASSIGNED

/* UART Board */
#define CC1350_SWIMTHERMO_UART_RX               PIN_UNASSIGNED  /* RXD */
#define CC1350_SWIMTHERMO_UART_TX               PIN_UNASSIGNED  /* TXD */
#define CC1350_SWIMTHERMO_UART_CTS              PIN_UNASSIGNED  /* CTS */
#define CC1350_SWIMTHERMO_UART_RTS              PIN_UNASSIGNED  /* RTS */

/*!
 *  @brief  Initialize the general board specific settings
 *
 *  This function initializes the general board specific settings.
 */
void CC1350_SWIMTHERMO_initGeneral(void);

/*!
 *  @def    CC1350_SWIMTHERMO_ADCBufName
 *  @brief  Enum of ADCBufs
 */
typedef enum CC1350_SWIMTHERMO_ADCBufName {
    CC1350_SWIMTHERMO_ADCBUF0 = 0,

    CC1350_SWIMTHERMO_ADCBUFCOUNT
} CC1350_SWIMTHERMO_ADCBufName;

/*!
 *  @def    CC1350_SWIMTHERMO_ADCBuf0SourceName
 *  @brief  Enum of ADCBuf channels
 */
typedef enum CC1350_SWIMTHERMO_ADCBuf0ChannelName {
    CC1350_SWIMTHERMO_ADCBUF0CHANNEL0 = 0,
    CC1350_SWIMTHERMO_ADCBUF0CHANNELVDDS,
    CC1350_SWIMTHERMO_ADCBUF0CHANNELDCOUPL,
    CC1350_SWIMTHERMO_ADCBUF0CHANNELVSS,

    CC1350_SWIMTHERMO_ADCBUF0CHANNELCOUNT
} CC1350_SWIMTHERMO_ADCBuf0ChannelName;

/*!
 *  @def    CC1350_SWIMTHERMO_ADCName
 *  @brief  Enum of ADCs
 */
typedef enum CC1350_SWIMTHERMO_ADCName {
    CC1350_SWIMTHERMO_ADC0 = 0,
    CC1350_SWIMTHERMO_ADCDCOUPL,
    CC1350_SWIMTHERMO_ADCVSS,
    CC1350_SWIMTHERMO_ADCVDDS,

    CC1350_SWIMTHERMO_ADCCOUNT
} CC1350_SWIMTHERMO_ADCName;

/*!
 *  @def    CC1350_SWIMTHERMO_CryptoName
 *  @brief  Enum of Crypto names
 */
typedef enum CC1350_SWIMTHERMO_CryptoName {
    CC1350_SWIMTHERMO_CRYPTO0 = 0,

    CC1350_SWIMTHERMO_CRYPTOCOUNT
} CC1350_SWIMTHERMO_CryptoName;

/*!
 *  @def    CC1350_SWIMTHERMO_GPIOName
 *  @brief  Enum of GPIO names
 */
typedef enum CC1350_SWIMTHERMO_GPIOName {
    CC1350_SWIMTHERMO_GPIO_PSU_ENABLE = 0,
    CC1350_SWIMTHERMO_GPIO_T_ON1,
    CC1350_SWIMTHERMO_GPIO_T_ON2,
    CC1350_SWIMTHERMO_GPIO_BTN,
    CC1350_SWIMTHERMO_GPIO_LED,

    CC1350_SWIMTHERMO_GPIOCOUNT
} CC1350_SWIMTHERMO_GPIOName;

/*!
 *  @def    CC1350_SWIMTHERMO_GPTimerName
 *  @brief  Enum of GPTimer parts
 */
typedef enum CC1350_SWIMTHERMO_GPTimerName {
    CC1350_SWIMTHERMO_GPTIMER0A = 0,
    CC1350_SWIMTHERMO_GPTIMER0B,
    CC1350_SWIMTHERMO_GPTIMER1A,
    CC1350_SWIMTHERMO_GPTIMER1B,
    CC1350_SWIMTHERMO_GPTIMER2A,
    CC1350_SWIMTHERMO_GPTIMER2B,
    CC1350_SWIMTHERMO_GPTIMER3A,
    CC1350_SWIMTHERMO_GPTIMER3B,

    CC1350_SWIMTHERMO_GPTIMERPARTSCOUNT
} CC1350_SWIMTHERMO_GPTimerName;

/*!
 *  @def    CC1350_SWIMTHERMO_GPTimers
 *  @brief  Enum of GPTimers
 */
typedef enum CC1350_SWIMTHERMO_GPTimers {
    CC1350_SWIMTHERMO_GPTIMER0 = 0,
    CC1350_SWIMTHERMO_GPTIMER1,
    CC1350_SWIMTHERMO_GPTIMER2,
    CC1350_SWIMTHERMO_GPTIMER3,

    CC1350_SWIMTHERMO_GPTIMERCOUNT
} CC1350_SWIMTHERMO_GPTimers;

/*!
 *  @def    CC1350_SWIMTHERMO_NVSName
 *  @brief  Enum of NVS names
 */
typedef enum CC1350_SWIMTHERMO_NVSName {
    CC1350_SWIMTHERMO_NVSCC26XX0 = 0,
    CC1350_SWIMTHERMO_NVSSPI25X0,

    CC1350_SWIMTHERMO_NVSCOUNT
} CC1350_SWIMTHERMO_NVSName;

/*!
 *  @def    CC1350_SWIMTHERMO_PWM
 *  @brief  Enum of PWM outputs
 */
typedef enum CC1350_SWIMTHERMO_PWMName {
    CC1350_SWIMTHERMO_PWM0 = 0,
    CC1350_SWIMTHERMO_PWM1,
    CC1350_SWIMTHERMO_PWM2,
    CC1350_SWIMTHERMO_PWM3,
    CC1350_SWIMTHERMO_PWM4,
    CC1350_SWIMTHERMO_PWM5,
    CC1350_SWIMTHERMO_PWM6,
    CC1350_SWIMTHERMO_PWM7,

    CC1350_SWIMTHERMO_PWMCOUNT
} CC1350_SWIMTHERMO_PWMName;

/*!
 *  @def    CC1350_SWIMTHERMO_UARTName
 *  @brief  Enum of UARTs
 */
typedef enum CC1350_SWIMTHERMO_UARTName {
    CC1350_SWIMTHERMO_UART0 = 0,

    CC1350_SWIMTHERMO_UARTCOUNT
} CC1350_SWIMTHERMO_UARTName;

/*!
 *  @def    CC1350_SWIMTHERMO_UDMAName
 *  @brief  Enum of DMA buffers
 */
typedef enum CC1350_SWIMTHERMO_UDMAName {
    CC1350_SWIMTHERMO_UDMA0 = 0,

    CC1350_SWIMTHERMO_UDMACOUNT
} CC1350_SWIMTHERMO_UDMAName;

/*!
 *  @def    CC1350_SWIMTHERMO_WatchdogName
 *  @brief  Enum of Watchdogs
 */
typedef enum CC1350_SWIMTHERMO_WatchdogName {
    CC1350_SWIMTHERMO_WATCHDOG0 = 0,

    CC1350_SWIMTHERMO_WATCHDOGCOUNT
} CC1350_SWIMTHERMO_WatchdogName;

#ifdef __cplusplus
}
#endif

#endif /* __CC1350_SWIMTHERMO_BOARD_H__ */
