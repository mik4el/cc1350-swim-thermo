/*
 * Copyright (c) 2017, Texas Instruments Incorporated
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

/***** Includes *****/
/* Standard C Libraries */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* TI Drivers */
#include <ti/drivers/rf/RF.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/ADC.h>

/* POSIX Header files */
#include <pthread.h>
#include <unistd.h>

/* Driverlib Header files */
#include DeviceFamily_constructPath(driverlib/rf_prop_mailbox.h)

/* Board Header files */
#include "Board.h"

/* Application Header files */ 
#include "RFQueue.h"
#include "smartrf_settings/smartrf_settings.h"



/***** Prototypes *****/
static void echoCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e);



/***** Variables and defines *****/

/* RF section */
#define THREADSTACKSIZE (1024)

// Packet RX/TX Configuration
#define PAYLOAD_LENGTH         30 /* Max length byte the radio will accept */
#define TX_DELAY             (uint32_t)(4000000*0.1f) /* Set Transmit (echo) delay to 100ms */
#define NUM_DATA_ENTRIES       2 /* NOTE: Only two data entries supported at the moment */

/* The Data Entries data field will contain:
 * 1 Header byte (RF_cmdPropRx.rxConf.bIncludeHdr = 0x1)
 * Max 30 payload bytes
 * 1 status byte (RF_cmdPropRx.rxConf.bAppendStatus = 0x1) */
#define NUM_APPENDED_BYTES     2

static RF_Object rfObject;
static RF_Handle rfHandle;

/* Buffer which contains all Data Entries for receiving data.
 * Pragmas are needed to make sure this buffer is aligned to a 4 byte boundary
 * (requirement from the RF core)
 */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(rxDataEntryBuffer, 4)
static uint8_t
rxDataEntryBuffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(NUM_DATA_ENTRIES,
                                                  PAYLOAD_LENGTH,
                                                  NUM_APPENDED_BYTES)];
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment = 4
static uint8_t
rxDataEntryBuffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(NUM_DATA_ENTRIES,
                                                  PAYLOAD_LENGTH,
                                                  NUM_APPENDED_BYTES)];
#elif defined(__GNUC__)
static uint8_t
rxDataEntryBuffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(NUM_DATA_ENTRIES,
                                                  PAYLOAD_LENGTH,
                                                  NUM_APPENDED_BYTES)]
                                                  __attribute__((aligned(4)));
#else
#error This compiler is not supported
#endif //defined(__TI_COMPILER_VERSION__)

static rfc_propRxOutput_t rxStatistics; /* Receive Statistics */

/* Receive dataQueue for RF Core to fill in data */
static dataQueue_t dataQueue;
static rfc_dataEntryGeneral_t* currentDataEntry;
static uint8_t packetLength;
static uint8_t* packetDataPointer;
static uint8_t txPacket[PAYLOAD_LENGTH];

/* End of RF section */

/* SPI section */
#define SPI_MSG_LENGTH  (64)
#define MAX_LOOP        (10)

// Bits for SPI to send to, NB: TLC5973 SDI pin is connected with a mosfet to SPI port, high on SPI port is low on TLC5973
#define ONE     0b0101
#define ZERO    0b0111
#define NONE    0b1111

unsigned char masterTxBuffer[] = { NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, // GSLAT
                                   ZERO, ZERO, ONE, ONE, ONE, ZERO, ONE, ZERO, ONE, ZERO, ONE, ZERO, //0x3AA  0b001110101010
                                   ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, //0x000 OUT0
                                   ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, //0x000 OUT1
                                   ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, //0x000 OUT2
                                   NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, // GSLAT
                                 };

unsigned char masterRxBuffer[SPI_MSG_LENGTH];

// Controlling variables to animate through colors for LED using the TLC5973.
unsigned short grayscale = 0;
int operand = 1;
int active_rgb_channel = 0;
int prev_rgb_channel = 0;

/* End of SPI section */



/* PIN section */

//Pin driver handles
static PIN_Handle pinHandle;

// Global memory storage for a PIN_Config table
static PIN_State pinState;

/*
 * Application button pin configuration table:
 *   - Buttons interrupts are configured to trigger on falling edge.
 */
PIN_Config pinTable[] = {
                         CC1350_SWIMTHERMO_DIO5_T_ON1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL, /* T_ON1 should be default high */
                         CC1350_SWIMTHERMO_DIO6_T_ON2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL, /* T_ON2 should be default low */
                         CC1350_SWIMTHERMO_DIO8_BUTTON  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
                         PIN_TERMINATE
                        };

/* End of PIN section */



/* Other section */
int rxCount = 0;
uint16_t adcValue0;

/* End of Other section */



/***** Function definitions *****/

// Convert uV reading from LMT70 to degrees celcius
double convertLMT70uVToDegC(uint32_t adcValue) {
    return (-0.193 * (adcValue / 1000)) + 212.009; // constants from LMT70 datasheet p13 http://www.ti.com/lit/ds/symlink/lmt70.pdf
}

// Reads LMT70 on ADC and prints temperature
void readLMT70()
{
    ADC_Handle   adc;
    ADC_Params   params;
    int_fast16_t res;
    uint32_t     high_res;
    CPUdelay(1000); // Let reading from LMT70 stabilize

    ADC_Params_init(&params);
    adc = ADC_open(CC1350_SWIMTHERMO_ADC0, &params);

    if (adc == NULL) {
        printf("Error initializing ADC channel 0\n");
    }
    else {
        printf("ADC channel 0 initialized\n");
    }

    /* Blocking mode conversion */
    res = ADC_convert(adc, &adcValue0);
    high_res = ADC_convertRawToMicroVolts(adc, adcValue0); //microvolts, not millivolts

    if (res == ADC_STATUS_SUCCESS) {
        printf("LMT70 reading: %i uV %f deg C\n", high_res, convertLMT70uVToDegC(high_res));
    }
    else {
        printf("ADC channel 0 convert failed\n");
    }

    ADC_close(adc);
}

void toggleLMT70() {
    uint32_t currVal = 0;
    currVal = PIN_getOutputValue(CC1350_SWIMTHERMO_DIO5_T_ON1);
    PIN_setOutputValue(pinHandle, CC1350_SWIMTHERMO_DIO5_T_ON1, !currVal);
    PIN_setOutputValue(pinHandle, CC1350_SWIMTHERMO_DIO6_T_ON2, currVal);
}

/*
 *  ======== buttonCallbackFxn ========
 *  Pin interrupt Callback function board buttons configured in the pinTable.
 *  If Board_LED3 and Board_LED4 are defined, then we'll add them to the PIN
 *  callback function.
 */
void buttonCallbackFxn(PIN_Handle handle, PIN_Id pinId) {
    if (!PIN_getInputValue(pinId)) {
        switch (pinId) {
            case CC1350_SWIMTHERMO_DIO8_BUTTON:
                if (active_rgb_channel == -1) {
                    active_rgb_channel = prev_rgb_channel; // Make led animation previous led channel
                } else {
                    prev_rgb_channel = active_rgb_channel;
                    active_rgb_channel = -1; // Make led animation white
                    toggleLMT70();
                    readLMT70();
                }
                printf("Button press\n");
                break;

            default:
                /* Do nothing */
                break;
        }
    }
}

/*
 *  ======== LEDThread ========
 *  Animates LED
 */
void updateLEDTxBufferCycleColor() {
    grayscale = grayscale + operand;
    if (grayscale >= 4096) {
        grayscale = 4096;
        operand = -1;
    }
    if (grayscale <= 0) {
        grayscale = 0;
        operand = 1;
    }
    if (active_rgb_channel > 2) {
        active_rgb_channel = 0;
    }
    int i;
    unsigned short word = grayscale;
    for(i = 0; i < 12; i++) {
        if (word & 0x0800) {
            if (active_rgb_channel == 0 || active_rgb_channel < 0 ) {
                masterTxBuffer[20+i] = ONE; // Blue
            }
            if (active_rgb_channel == 1 || active_rgb_channel < 0) {
                masterTxBuffer[32+i] = ONE; // Green
            }
            if (active_rgb_channel == 2 || active_rgb_channel < 0) {
                masterTxBuffer[44+i] = ONE; // Red
            }
        } else {
            if (active_rgb_channel == 0 || active_rgb_channel < 0 ) {
                masterTxBuffer[20+i] = ZERO; // Blue
            }
            if (active_rgb_channel == 1 || active_rgb_channel < 0 ) {
                masterTxBuffer[32+i] = ZERO; // Green
            }
            if (active_rgb_channel == 2 || active_rgb_channel < 0 ) {
                masterTxBuffer[44+i] = ZERO; // Red
            }
        }
        // Zero out non-active channels
        if (active_rgb_channel == 0) {
            masterTxBuffer[44+i] = ZERO; // Red
            masterTxBuffer[32+i] = ZERO; // Green
        }
        if (active_rgb_channel == 1) {
            masterTxBuffer[20+i] = ZERO; // Blue
            masterTxBuffer[44+i] = ZERO; // Red
        }
        if (active_rgb_channel == 2) {
            masterTxBuffer[32+i] = ZERO; // Green
            masterTxBuffer[20+i] = ZERO; // Blue
        }
        word <<= 1;
    }
    return;
}

void led_breathe() {
    SPI_Handle      masterSpi;
    SPI_Params      spiParams;
    SPI_Transaction transaction;
    bool            transferOK;

    /* Open SPI as master (default) */
    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_TI;
    spiParams.bitRate = 1000000; // fails around 3 Mbps
    spiParams.dataSize = 4;
    masterSpi = SPI_open(CC1350_SWIMTHERMO_SPI0, &spiParams);
    if (masterSpi == NULL) {
        printf("Error initializing master SPI\n");
        while (1);
    }
    else {
        printf("Master SPI initialized\n");
    }

    while (1) {
        updateLEDTxBufferCycleColor();

        /* Initialize master SPI transaction structure */
        transaction.count = SPI_MSG_LENGTH;
        transaction.txBuf = (void *) masterTxBuffer;
        transaction.rxBuf = (void *) masterRxBuffer;

        /* Perform SPI transfer */
        transferOK = SPI_transfer(masterSpi, &transaction);
        if (!transferOK) {
           printf("Unsuccessful master SPI transfer");
        }
        usleep(50);
    }

    SPI_close(masterSpi);

    return;
}

void *LEDThread(void *arg0)
{
    led_breathe();

    printf("Done\n");

    return (NULL);
}

void *mainThread(void *arg0)
{
    pthread_t           thread0;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    /* Call driver init functions. */
    SPI_init();
    ADC_init();

    pinHandle = PIN_open(&pinState, pinTable);
    if(!pinHandle) {
        printf("Error initializing pins\n");
    }

    //Setup callback for button pins
    if (PIN_registerIntCb(pinHandle, &buttonCallbackFxn) != 0) {
        printf("Error registering button callback function");
    }

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

    retc = pthread_create(&thread0, &attrs, LEDThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    RF_Params rfParams;
    RF_Params_init(&rfParams);

    if( RFQueue_defineQueue(&dataQueue,
                            rxDataEntryBuffer,
                            sizeof(rxDataEntryBuffer),
                            NUM_DATA_ENTRIES,
                            PAYLOAD_LENGTH + NUM_APPENDED_BYTES))
    {
        while(1);
    }

    /* Modify CMD_PROP_TX and CMD_PROP_RX commands for application needs */
    /* Set the Data Entity queue for received data */
    RF_cmdPropRx.pQueue = &dataQueue;
    /* Discard ignored packets from Rx queue */
    RF_cmdPropRx.rxConf.bAutoFlushIgnored = 1;
    /* Discard packets with CRC error from Rx queue */
    RF_cmdPropRx.rxConf.bAutoFlushCrcErr = 1;
    /* Implement packet length filtering to avoid PROP_ERROR_RXBUF */
    RF_cmdPropRx.maxPktLen = PAYLOAD_LENGTH;
    /* End RX operation when a packet is received correctly and move on to the
     * next command in the chain */
    RF_cmdPropRx.pktConf.bRepeatOk = 0;
    RF_cmdPropRx.pktConf.bRepeatNok = 1;
    RF_cmdPropRx.startTrigger.triggerType = TRIG_NOW;
    RF_cmdPropRx.pNextOp = (rfc_radioOp_t *)&RF_cmdPropTx;
    /* Only run the TX command if RX is successful */
    RF_cmdPropRx.condition.rule = COND_STOP_ON_FALSE;
    RF_cmdPropRx.pOutput = (uint8_t *)&rxStatistics;

    RF_cmdPropTx.pktLen = PAYLOAD_LENGTH;
    RF_cmdPropTx.pPkt = txPacket;
    RF_cmdPropTx.startTrigger.triggerType = TRIG_REL_PREVEND;
    RF_cmdPropTx.startTime = TX_DELAY;


    /* Request access to the radio */
    rfHandle = RF_open(&rfObject, &RF_prop,
                       (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rfParams);

    /* Set the frequency */
    RF_postCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

    while(1)
    {
        /* Wait for a packet
         * - When the first of the two chained commands (RX) completes, the
         * RF_EventCmdDone and RF_EventRxEntryDone events are raised on a
         * successful packet reception, and then the next command in the chain
         * (TX) is run
         * - If the RF core runs into an issue after receiving the packet
         * incorrectly onlt the RF_EventCmdDone event is raised; this is an
         * error condition
         * - If the RF core successfully echos the received packet the RF core
         * should raise the RF_EventLastCmdDone event
         */
        RF_EventMask terminationReason =
                RF_runCmd(rfHandle, (RF_Op*)&RF_cmdPropRx, RF_PriorityNormal,
                          echoCallback, (RF_EventRxEntryDone |
                          RF_EventLastCmdDone));

        switch(terminationReason)
        {
        case RF_EventCmdDone:
            // A radio operation command in a chain finished
            break;
        case RF_EventLastCmdDone:
            // A stand-alone radio operation command or the last radio
            // operation command in a chain finished.
            break;
        case RF_EventCmdCancelled:
            // Command cancelled before it was started; it can be caused
            // by RF_cancelCmd() or RF_flushCmd().
            break;
        case RF_EventCmdAborted:
            // Abrupt command termination caused by RF_cancelCmd() or
            // RF_flushCmd().
            break;
        case RF_EventCmdStopped:
            // Graceful command termination caused by RF_cancelCmd() or
            // RF_flushCmd().
            break;
        default:
            // Uncaught error event
            while(1);
        }

        uint32_t cmdStatus = ((volatile RF_Op*)&RF_cmdPropRx)->status;
        switch(cmdStatus)
        {
        case PROP_DONE_OK:
            // Packet received with CRC OK
            break;
        case PROP_DONE_RXERR:
            // Packet received with CRC error
            break;
        case PROP_DONE_RXTIMEOUT:
            // Observed end trigger while in sync search
            break;
        case PROP_DONE_BREAK:
            // Observed end trigger while receiving packet when the command is
            // configured with endType set to 1
            break;
        case PROP_DONE_ENDED:
            // Received packet after having observed the end trigger; if the
            // command is configured with endType set to 0, the end trigger
            // will not terminate an ongoing reception
            break;
        case PROP_DONE_STOPPED:
            // received CMD_STOP after command started and, if sync found,
            // packet is received
            break;
        case PROP_DONE_ABORT:
            // Received CMD_ABORT after command started
            break;
        case PROP_ERROR_RXBUF:
            // No RX buffer large enough for the received data available at
            // the start of a packet
            break;
        case PROP_ERROR_RXFULL:
            // Out of RX buffer space during reception in a partial read
            break;
        case PROP_ERROR_PAR:
            // Observed illegal parameter
            break;
        case PROP_ERROR_NO_SETUP:
            // Command sent without setting up the radio in a supported
            // mode using CMD_PROP_RADIO_SETUP or CMD_RADIO_SETUP
            break;
        case PROP_ERROR_NO_FS:
            // Command sent without the synthesizer being programmed
            break;
        case PROP_ERROR_RXOVF:
            // RX overflow observed during operation
            break;
        default:
            // Uncaught error event - these could come from the
            // pool of states defined in rf_mailbox.h
            while(1);
        }
    }
}

static void echoCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    if (e & RF_EventRxEntryDone)
    {
        /* Successful RX */
        rxCount++;

        printf("Successful RX %i\n", rxCount);

        /* Get current unhandled data entry */
        currentDataEntry = RFQueue_getDataEntry();

        /* Handle the packet data, located at &currentDataEntry->data:
         * - Length is the first byte with the current configuration
         * - Data starts from the second byte */
        packetLength      = *(uint8_t *)(&(currentDataEntry->data));
        packetDataPointer = (uint8_t *)(&(currentDataEntry->data) + 1);

        /* Copy the payload + status byte to the rxPacket variable, and then
         * over to the txPacket
         */
        memcpy(txPacket, packetDataPointer, packetLength);

        RFQueue_nextEntry();
    }
    else if (e & RF_EventLastCmdDone)
    {
        /* Successful Echo (TX)*/
        printf("Successful Echo\n");

        /* Change LED color to indicate successful Echo */
        active_rgb_channel++;
    }
    else // any uncaught event
    {
        /* Error Condition: set LED1, clear LED2 */
        printf("Error\n");
    }
}
