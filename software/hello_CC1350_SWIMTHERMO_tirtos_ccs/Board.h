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

#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CC1350_SWIMTHERMO.h"

#define Board_initGeneral()      CC1350_SWIMTHERMO_initGeneral()
#define Board_shutDownExtFlash() CC1350_SWIMTHERMO_shutDownExtFlash()
#define Board_wakeUpExtFlash()   CC1350_SWIMTHERMO_wakeUpExtFlash()

/* These #defines allow us to reuse TI-RTOS across other device families */
#define Board_ADC0              CC1350_SWIMTHERMO_ADC0
#define Board_ADC1              CC1350_SWIMTHERMO_ADC1

#define Board_ADCBUF0           CC1350_SWIMTHERMO_ADCBUF0
#define Board_ADCBUF0CHANNEL0   CC1350_SWIMTHERMO_ADCBUF0CHANNEL0
#define Board_ADCBUF0CHANNEL1   CC1350_SWIMTHERMO_ADCBUF0CHANNEL1

#define Board_CRYPTO0           CC1350_SWIMTHERMO_CRYPTO0

#define Board_GPTIMER0A         CC1350_SWIMTHERMO_GPTIMER0A
#define Board_GPTIMER0B         CC1350_SWIMTHERMO_GPTIMER0B
#define Board_GPTIMER1A         CC1350_SWIMTHERMO_GPTIMER1A
#define Board_GPTIMER1B         CC1350_SWIMTHERMO_GPTIMER1B
#define Board_GPTIMER2A         CC1350_SWIMTHERMO_GPTIMER2A
#define Board_GPTIMER2B         CC1350_SWIMTHERMO_GPTIMER2B
#define Board_GPTIMER3A         CC1350_SWIMTHERMO_GPTIMER3A
#define Board_GPTIMER3B         CC1350_SWIMTHERMO_GPTIMER3B

#define Board_PWM0              CC1350_SWIMTHERMO_PWM0
#define Board_PWM1              CC1350_SWIMTHERMO_PWM1
#define Board_PWM2              CC1350_SWIMTHERMO_PWM2
#define Board_PWM3              CC1350_SWIMTHERMO_PWM3
#define Board_PWM4              CC1350_SWIMTHERMO_PWM4
#define Board_PWM5              CC1350_SWIMTHERMO_PWM5
#define Board_PWM6              CC1350_SWIMTHERMO_PWM6
#define Board_PWM7              CC1350_SWIMTHERMO_PWM7

#define Board_UART0             CC1350_SWIMTHERMO_UART0

#define Board_WATCHDOG0         CC1350_SWIMTHERMO_WATCHDOG0

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */
