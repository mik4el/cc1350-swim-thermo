/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
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
 *  @file       CC1350_LAUNCHXL.h
 *
 *  @brief      CC1350 LaunchPad Board Specific header file.
 *
 *  NB! This is the board file for CC1350 LaunchPad PCB version 1.0
 *
 *  ============================================================================
 */
#ifndef __CC1350_LAUNCHXL_BOARD_H__
#define __CC1350_LAUNCHXL_BOARD_H__

#ifdef __cplusplus
extern "C" {
#endif

/** ============================================================================
 *  Includes
 *  ==========================================================================*/
#include <ti/drivers/PIN.h>
#include <driverlib/ioc.h>

/** ============================================================================
 *  Externs
 *  ==========================================================================*/
extern const PIN_Config BoardGpioInitTable[];

/** ============================================================================
 *  Defines
 *  ==========================================================================*/
#define CC1350_LAUNCHXL

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>        <pin mapping>   <comments>
 */

#define CC1350_SWIMTHERMO_DIO7_ANALOG   IOID_7


/** ============================================================================
 *  Instance identifiers
 *  ==========================================================================*/
/* Generic GPTimer instance identifiers */
#define Board_GPTIMER0A             CC1350_LAUNCHXL_GPTIMER0A
#define Board_GPTIMER0B             CC1350_LAUNCHXL_GPTIMER0B
#define Board_GPTIMER1A             CC1350_LAUNCHXL_GPTIMER1A
#define Board_GPTIMER1B             CC1350_LAUNCHXL_GPTIMER1B
#define Board_GPTIMER2A             CC1350_LAUNCHXL_GPTIMER2A
#define Board_GPTIMER2B             CC1350_LAUNCHXL_GPTIMER2B
#define Board_GPTIMER3A             CC1350_LAUNCHXL_GPTIMER3A
#define Board_GPTIMER3B             CC1350_LAUNCHXL_GPTIMER3B

/** ============================================================================
 *  Number of peripherals and their names
 *  ==========================================================================*/

/*!
 *  @def    CC1350_LAUNCHXL_GPTimerName
 *  @brief  Enum of GPTimer parts
 */
typedef enum CC1350_LAUNCHXL_GPTimerName
{
    CC1350_LAUNCHXL_GPTIMER0A = 0,
    CC1350_LAUNCHXL_GPTIMER0B,
    CC1350_LAUNCHXL_GPTIMER1A,
    CC1350_LAUNCHXL_GPTIMER1B,
    CC1350_LAUNCHXL_GPTIMER2A,
    CC1350_LAUNCHXL_GPTIMER2B,
    CC1350_LAUNCHXL_GPTIMER3A,
    CC1350_LAUNCHXL_GPTIMER3B,
    CC1350_LAUNCHXL_GPTIMERPARTSCOUNT
} CC1350_LAUNCHXL_GPTimerName;

/*!
 *  @def    CC1350_LAUNCHXL_GPTimers
 *  @brief  Enum of GPTimers
 */
typedef enum CC1350_LAUNCHXL_GPTimers
{
    CC1350_LAUNCHXL_GPTIMER0 = 0,
    CC1350_LAUNCHXL_GPTIMER1,
    CC1350_LAUNCHXL_GPTIMER2,
    CC1350_LAUNCHXL_GPTIMER3,
    CC1350_LAUNCHXL_GPTIMERCOUNT
} CC1350_LAUNCHXL_GPTimers;

/*!
 *  @def    CC1350_SWIMTHERMO_ADCBufName
 *  @brief  Enum of ADCBufs
 */
typedef enum CC1350_SWIMTHERMO_ADCBufName {
    CC1350_SWIMTHERMO_ADCBuf0 = 0,
    CC1350_SWIMTHERMO_ADCBufCOUNT
} CC1350_SWIMTHERMO_ADCBufName;


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


#ifdef __cplusplus
}
#endif

#endif /* __CC1350_LAUNCHXL_BOARD_H__ */
