/*
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
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
 *  ======== spimaster.c ========
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* POSIX Header files */
#include <pthread.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/SPI.h>

/* Example/Board Header files */
#include "Board.h"

#define THREADSTACKSIZE (1024)

#define SPI_MSG_LENGTH  (72)

#define MAX_LOOP        (10)

unsigned char masterRxBuffer[SPI_MSG_LENGTH];
//unsigned char masterTxBuffer[] = { 0b11001100, 0b11001100, 0b11001100, 0b11001100, 0b11001100, 0b11001100 };
//unsigned char masterTxBuffer[] = { 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000 };
//unsigned char masterTxBuffer[] = { 0b00111111, 0b00111111, 0b00111111, 0b00111111, 0b00111111, 0b00111111 };
//unsigned char masterTxBuffer[] = { 0b11000000, 0b11001100, 0b11000000, 0b11001100, 0b11000000, 0b11001100 };
//unsigned char masterTxBuffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

#define ONE     0b00110011
#define ZERO    0b00111111
#define NONE    0b11111111
//unsigned char masterTxBuffer[] = { 0b00111111, 0b00110011, 0b00111111, 0b00110011, 0b00111111, 0b00110011 };
//unsigned char masterTxBuffer[] = { ZERO, ONE, ZERO, ONE, ZERO, ONE };

// TLC5973 message: GSLAT, 0x3AA, 0xFFF, 0xFFF, 0xFFF, GSLAT (white)
unsigned char masterTxBuffer[] = { NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, // GSLAT
                                   ZERO, ZERO, ONE, ONE, ONE, ZERO, ONE, ZERO, ONE, ZERO, ONE, ZERO, //0x3AA  0b001110101010
                                   ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, //0xFFF OUT0
                                   ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, //0xFFF OUT1
                                   ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, //0xFFF OUT2
                                   NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, // GSLAT
                                 };

/*
// TLC5973 message: low, 0x3AA, 0xFFF, 0x000, 0x000, low (blue)
unsigned char masterTxBuffer[] = { NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, // GSLAT
                                   ZERO, ZERO, ONE, ONE, ONE, ZERO, ONE, ZERO, ONE, ZERO, ONE, ZERO, //0x3AA  0b001110101010
                                   ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, //0xFFF OUT0
                                   ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, //0x000 OUT1
                                   ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, //0x000 OUT2
                                   NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, // GSLAT
                                 };
*/

/*
// TLC5973 message: low, 0x3AA, 0x000, 0xFFF, 0x000, low (green)
unsigned char masterTxBuffer[] = { NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, // GSLAT
                                   ZERO, ZERO, ONE, ONE, ONE, ZERO, ONE, ZERO, ONE, ZERO, ONE, ZERO, //0x3AA  0b001110101010
                                   ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, //0x000 OUT0
                                   ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, //0xFFF OUT1
                                   ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, //0x000 OUT1
                                   NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, // GSLAT
                                 };
*/

/*
// TLC5973 message: low, 0x3AA, 0x000, 0x000, 0xFFF, low (red)
unsigned char masterTxBuffer[] = { NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, // GSLAT
                                   ZERO, ZERO, ONE, ONE, ONE, ZERO, ONE, ZERO, ONE, ZERO, ONE, ZERO, //0x3AA  0b001110101010
                                   ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, //0x000 OUT0
                                   ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, //0x000 OUT1
                                   ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, ONE, //0xFFF OUT2
                                   NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, // GSLAT
                                 };
*/

/*
 *  ======== masterThread ========
 *  Master SPI sends a message to slave while simultaneously receiving a
 *  message from the slave.
 */
void *masterThread(void *arg0)
{
    SPI_Handle      masterSpi;
    SPI_Params      spiParams;
    SPI_Transaction transaction;
    uint32_t        i;
    bool            transferOK;

    /* Open SPI as master (default) */
    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_TI;
    spiParams.bitRate = 1000000;
    masterSpi = SPI_open(Board_SPI_MASTER, &spiParams);
    if (masterSpi == NULL) {
        printf("Error initializing master SPI\n");
        while (1);
    }
    else {
        printf("Master SPI initialized\n");
    }

    for (i = 0; i < MAX_LOOP; i++) {
        /*
         * Wait until slave is ready for transfer; slave will pull
         * Board_SPI_SLAVE_READY low.
         */

        /* Initialize master SPI transaction structure */
        memset((void *) masterRxBuffer, 0, SPI_MSG_LENGTH);
        transaction.count = SPI_MSG_LENGTH;
        transaction.txBuf = (void *) masterTxBuffer;
        transaction.rxBuf = (void *) masterRxBuffer;

        /* Perform SPI transfer */
        transferOK = SPI_transfer(masterSpi, &transaction);
        if (!transferOK) {
           printf("Unsuccessful master SPI transfer");
        }

        /* Sleep for a bit before starting the next SPI transfer  */
        sleep(3);
    }

    SPI_close(masterSpi);

    printf("\nDone");

    return (NULL);
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    pthread_t           thread0;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    /* Call driver init functions. */
    SPI_init();

    printf("Starting the SPI master example");
    printf("This example requires external wires to be "
        "connected to the header pins. Please see the Board.html for details.\n");

    /* Create application threads */
    pthread_attr_init(&attrs);

    detachState = PTHREAD_CREATE_DETACHED;
    /* Set priority and stack size attributes */
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    /* Create master thread */
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc = pthread_create(&thread0, &attrs, masterThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    return (NULL);
}
