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

/*
 *  ======== pinInterrupt.c ========
 */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

/* Example/Board Header files */
#include "Board.h"

/* Pin driver handles */
static PIN_Handle ledPinHandle;

/* Global memory storage for a PIN_Config table */
static PIN_State ledPinState;

/*
 * Initial LED pin configuration table
 *   - LEDs Board_LED0 is on.
 *   - LEDs Board_LED1 is off.
 */
PIN_Config ledPinTable[] = {
    PIN_LED_EASYSET | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL,
    PIN_TERMINATE
};

void led_easyset_high() {
    PIN_setOutputValue(ledPinHandle, PIN_LED_EASYSET, 0);
}

void led_easyset_low() {
    PIN_setOutputValue(ledPinHandle, PIN_LED_EASYSET, 1);
}


void output_high() {
    led_easyset_high();
}

void output_low() {
    led_easyset_low();
}

void delay() {
    asm(" NOP");
}

uint16_t *pixels;        // Holds LED color values (3 or 4 bytes each)

void pulse() {
    led_easyset_high();
    led_easyset_low();
}

void writeZero() {
    pulse();
    led_easyset_low();
    led_easyset_low();
}

void writeNone() {
    delay();
    delay();
}

void writeOne() {
    pulse();
    pulse();
}

void waitGSLAT(uint8_t num) {
    uint8_t i;
    for (i = 0; i < num; i++) {
        writeNone();
    }
}

void writeWord(uint16_t word) {
    uint8_t i;
    for (i = 0; i < 12; i++){
        if (word & 0x800) {
            writeOne();
        } else {
            writeZero();
        }
        word <<= 1;
    }
}

void setPixelColor(uint16_t r, uint16_t g, uint16_t b) {
    //numWords = numLEDs * 3;
    if((pixels = (uint16_t *)malloc(3))) {
        memset(pixels, 0, 3);
    }
    uint16_t *p;
    p = &pixels[1 * 3];    // 3 bytes per pixel
    p[2] = r;          // R,G,B always stored
    p[1] = g;
    p[0] = b;
    writeWord( 0x03AA );
    writeWord( pixels[0] );
    writeWord( pixels[1] );
    writeWord( pixels[2] );
    waitGSLAT(4);
}

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initGeneral();

    /* Setup */
    /* Open LED pins */
    ledPinHandle = PIN_open(&ledPinState, ledPinTable);
    if(!ledPinHandle) {
        System_abort("Error initializing board LED pin\n");
    }

    System_printf("TLC5973 test!\n");

    setPixelColor(1234, 1234, 1234); // timings are off, this will not work t_cycle for 1 is 2.8uS*2 and t_cycle for 0 is 6.2uS

    /* Start kernel. */
    BIOS_start();

    return (0);
}
