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
 *  @brief      CC1350 Swimthermo Board Specific header file.
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
 *      <board signal alias>                    <pin mapping>   <comments>
 */
#define CC1350_SWIMTHERMO_DIO0_RF_POWER         IOID_0
#define CC1350_SWIMTHERMO_DIO1_RF_SUB1GHZ       IOID_1
#define CC1350_SWIMTHERMO_DIO2_PSU_ENABLE       IOID_2
#define CC1350_SWIMTHERMO_DIO5_T_ON1            IOID_5  //T_ON1 If set high, enables LMT70 on p1
#define CC1350_SWIMTHERMO_DIO6_T_ON2            IOID_6  //T_ON2 If set high, enables LMT70 on p2
#define CC1350_SWIMTHERMO_DIO7_ANALOG           IOID_7  //TAO on active LMT70
#define CC1350_SWIMTHERMO_DIO8_BUTTON           IOID_8

/*!
 *  @brief  Initialize the general board specific settings
 *
 *  This function initializes the general board specific settings.
 */
void CC1350_SWIMTHERMO_initGeneral(void);

/*!
 *  @def    CC1350_SWIMTHERMO_CryptoName
 *  @brief  Enum of Crypto names
 */
typedef enum CC1350_SWIMTHERMO_CryptoName {
    CC1350_SWIMTHERMO_CRYPTO0 = 0,

    CC1350_SWIMTHERMO_CRYPTOCOUNT
} CC1350_SWIMTHERMO_CryptoName;

#ifdef __cplusplus
}
#endif

#endif /* __CC1350_SWIMTHERMO_BOARD_H__ */
