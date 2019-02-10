/**************************************************************************************************
  Filename:       BleAdv.c

  Description:    This file contains the Simple Beacon API. This module is a wrapper around the RF
                  driver API's needed to send a non connectable advertisements.


* Copyright (c) 2016-2018, Texas Instruments Incorporated
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
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/System.h>

#include <stdlib.h>

/* Board Header files */
#include "Board.h"

#include "BleAdv.h"
#include "uble.h"
#include "ugap.h"

#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/display/Display.h>
#include <ti/display/DisplayExt.h>

extern BleAdv_Status BleAdv_startAdvertising(void);
extern BleAdv_Status BleAdv_stopAdvertising(void);

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define BLE_PRI_IEEE_ADDR_LOCATION   0x500012E8
#define BLE_SEC_IEEE_ADDR_LOCATION   0x500012D0

#if defined(FEATURE_STATIC_ADDR)
// Pre-generated Random Static Address
#define UEB_PREGEN_RAND_STATIC_ADDR    {0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc}
#endif

#define NODE_BLE_ACTIVITY_LED Board_PIN_LED1

// Advertising interval (units of 625us, 160=100ms)
#define DEFAULT_ADVERTISING_INTERVAL            160
// Advertising duty cycle on (units of 100ms)
#define DEFAULT_ADVERTISING_DUTYCYLE_ON         5
// Advertising duty cycle off (units of 100ms)
#define DEFAULT_ADVERTISING_DUTYCYLE_OFF        20
// How often to interleave a TLM packet during URL and UID advertising
#define DEFAULT_TLM_INTERLEAVE_RATE             4

#define EDDYSTONE_FRAME_TYPE_UID                0x00
#define EDDYSTONE_FRAME_TYPE_URL                0x10
#define EDDYSTONE_FRAME_TYPE_TLM                0x20

#define EDDYSTONE_FRAME_OVERHEAD_LEN            8
#define EDDYSTONE_SVC_DATA_OVERHEAD_LEN         3
#define EDDYSTONE_MAX_URL_LEN                   18
#define EDDYSTONE_UUID_FRAME_LEN                19
#define EDDYSTONE_TLM_FRAME_LEN                 14

// # of URL Scheme Prefix types
#define EDDYSTONE_URL_PREFIX_MAX        4
// # of encodable URL words
#define EDDYSTONE_URL_ENCODING_MAX      14

#define BLEADV_TXPOWER_0M_0DBM   -15

/*********************************************************************
 * TYPEDEFS
 */
// Eddystone UID frame
typedef struct
{
    uint8_t   frameType;      // UID
    int8_t    rangingData;
    uint8_t   namespaceID[10];
    uint8_t   instanceID[6];
    uint8_t   reserved[2];
} eddystoneUID_t;

// Eddystone URL frame
typedef struct
{
    uint8_t   frameType;      // URL | Flags
    int8_t    txPower;
    uint8_t   encodedURL[EDDYSTONE_MAX_URL_LEN];  // the 1st byte is prefix
} eddystoneURL_t;

// Eddystone TLM frame
typedef struct
{
    uint8_t   frameType;      // TLM
    uint8_t   version;        // 0x00 for now
    uint8_t   vBatt[2];       // Battery Voltage, 1mV/bit, Big Endian
    uint8_t   temp[2];        // Temperature. Signed 8.8 fixed point
    uint8_t   advCnt[4];      // Adv count since power-up/reboot
    uint8_t   secCnt[4];      // Time since power-up/reboot
                              // in 0.1 second resolution
} eddystoneTLM_t;

// Eddystone advertisement frame types
typedef union
{
    eddystoneUID_t        uid;
    eddystoneURL_t        url;
    eddystoneTLM_t        tlm;
} eddystoneFrame_t;

typedef struct
{
    uint8_t               length1;        // 2
    uint8_t               dataType1;      // for Flags data type (0x01)
    uint8_t               data1;          // for Flags data (0x04)
    uint8_t               length2;        // 3
    uint8_t               dataType2;      // for 16-bit Svc UUID list data type (0x03)
    uint8_t               data2;          // for Eddystone UUID LSB (0xAA)
    uint8_t               data3;          // for Eddystone UUID MSB (0xFE)
    uint8_t               length;         // Eddystone service data length
    uint8_t               dataType3;      // for Svc Data data type (0x16)
    uint8_t               data4;          // for Eddystone UUID LSB (0xAA)
    uint8_t               data5;          // for Eddystone UUID MSB (0xFE)
    eddystoneFrame_t  frame;
} eddystoneAdvData_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static bool configured = false;

PIN_Config blePinTable[] = {
    NODE_BLE_ACTIVITY_LED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#if defined RF_SWITCH_PIN
    RF_SWITCH_PIN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#endif
#if defined RF_SW_PWR_PIN
    RF_SW_PWR_PIN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#endif
    PIN_TERMINATE
};

PIN_Handle blePinHandle;
static PIN_State blePinState;

// Array of URL Scheme Prefix's
static char* eddystoneURLPrefix[EDDYSTONE_URL_PREFIX_MAX] =
{
    "http://www.",
    "https://www.",
    "http://",
    "https://"
};

// Array of URLs to be encoded
static char* eddystoneURLEncoding[EDDYSTONE_URL_ENCODING_MAX] =
{
    ".com/",
    ".org/",
    ".edu/",
    ".net/",
    ".info/",
    ".biz/",
    ".gov/",
    ".com/",
    ".org/",
    ".edu/",
    ".net/",
    ".info/",
    ".biz/",
    ".gov/"
};

static BleAdv_Stats advStats = {0};

char defaultUrl[] = "http://www.ti.com";

/* The Eddystone UID spec advices to use the first 10 bytes of the sha-1 hash
 * of an owned domain or subdonmian. The subdomain of
 * http://www.ti.com/product/CC1350 = 792f082074ebc132032cad5fdaada66154e14e98 */
static uint8_t uidNameSpace[10] = {0x79, 0x2f, 0x08, 0x20, 0x74, 0xeb, 0xc1,
                                   0x32, 0x03, 0x2c};

/*
 * uid Instance should be unique within the namespae.
 * We set it to 6 LSB's of IEEE addr */
static uint8_t uidInstanceId[6];

/* propreitory advertisement packet */
static uint8_t propAdvertisement[] = {
        0x02, //Length of this section
        0x01, //<<Flags>>
        0x02, //LE General Discoverable Mode
        0x06, //Length of this section
        0xff, //<<Manufacturer Specific Data>>
        0x0d,
        0x00,
        0x03,
        0x00,
        0x00}; //BTN state

BleAdv_Frame propAdvFrame;
BleAdv_Frame urlAdvFrame;
BleAdv_Frame uidAdvFrame;
BleAdv_Frame tlmAdvFrame;

//Initialize UID Frame
eddystoneAdvData_t uidAdvData =
{
    0x02,   // length1 2
    1,      // Flags data type
    6,      // Flags data

    // Complete list of 16-bit Service UUIDs
    0x03,   // length of this data including the data type byte
    3,      // 16-bit Svc UUID list data type (0x03)
    0xaa,   // Eddystone UUID LSB (0xAA)
    0xfe,   // Eddystone UUID MSB (0xFE)

    // Service Data
    0x03,   // Eddystone service data length
    0x16,   // Svc Data data type (0x16)
    0xaa,   // Eddystone UUID LSB (0xAA)
    0xfe    // Eddystone UUID MSB (0xFE)

    //Eddysone Frame to be copied in dynamically
};

//Initialize URL Frame
eddystoneAdvData_t urlAdvData =
{
    0x02,   // length1 2
    1,      // Flags data type
    6,      // Flags data

    // Complete list of 16-bit Service UUIDs
    0x03,   // length of this data including the data type byte
    3,      // 16-bit Svc UUID list data type (0x03)
    0xaa,   // Eddystone UUID LSB (0xAA)
    0xfe,   // Eddystone UUID MSB (0xFE)

    // Service Data
    0x03,   // Eddystone service data length
    0x16,   // Svc Data data type (0x16)
    0xaa,   // Eddystone UUID LSB (0xAA)
    0xfe    // Eddystone UUID MSB (0xFE)

    //Eddysone Frame to be copied in dynamically
};

//Initialize TLM Frame
eddystoneAdvData_t tlmAdvData =
{
    0x02,   // length1 2
    1,      // Flags data type
    6,      // Flags data

    // Complete list of 16-bit Service UUIDs
    0x03,   // length of this data including the data type byte
    3,      // 16-bit Svc UUID list data type (0x03)
    0xaa,   // Eddystone UUID LSB (0xAA)
    0xfe,   // Eddystone UUID MSB (0xFE)

    // Service Data
    0x03,   // Eddystone service data length
    0x16,   // Svc Data data type (0x16)
    0xaa,   // Eddystone UUID LSB (0xAA)
    0xfe    // Eddystone UUID MSB (0xFE)

    //Eddysone Frame to be copied in dynamically
};

static BleAdv_AdertiserType advertiserType = BleAdv_AdertiserNone;

/* Set Default parameters structure */
static const BleAdv_Params_t BleAdv_defaultParams = {
    .advertisingInterval      = DEFAULT_ADVERTISING_INTERVAL,
    .advertisingDutyCycleOn   = DEFAULT_ADVERTISING_DUTYCYLE_ON,
    .advertisingDutyCycleOff  = DEFAULT_ADVERTISING_DUTYCYLE_OFF,
    .tlmInterleaving          = DEFAULT_TLM_INTERLEAVE_RATE,
    .pfnPostEvtProxyCB        = NULL

};

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */
static void updateAdvFrame(BleAdv_Frame *pBeaconFrame);
BleAdv_Status getBleIeeeAddr(uint8_t *ieeeAddr);
static void stateChangeCB(ugBcastState_t newState);
static void advPrepareCB(void);
static void advDoneCB(bStatus_t status);
static void bleAntSettingHandler(void);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

static BleAdv_Params_t bleAdv_params;

void BleAdv_Params_init(BleAdv_Params_t *params)
{
    *params = BleAdv_defaultParams;
}

//*****************************************************************************
//
//! \brief Initialise the BleAdv module
//!
//! Initialise the BleAdv module.
//!
//!
//! \return BleAdv_Status
//!
//*****************************************************************************
BleAdv_Status BleAdv_init(BleAdv_Params_t *params)
{
#if defined(FEATURE_STATIC_ADDR)
    uint8 staticAddr[] = UEB_PREGEN_RAND_STATIC_ADDR;
#endif /* FEATURE_STATIC_ADDR */
    ugBcastCBs_t bcastCBs = {
    stateChangeCB,
    advPrepareCB,
    advDoneCB };

    // Check config parameters
    if ((params == NULL) ||
        (params->pfnPostEvtProxyCB == NULL) ||
        //convert interval and duty cycle to common unit of 625us
        (((uint32)params->advertisingInterval) >
         ((uint32)params->advertisingDutyCycleOn * 160 ) ))
    {
        return BleAdv_Status_Param_Error;
    }
    else
    {
        memcpy(&bleAdv_params, params, sizeof(BleAdv_Params_t));
    }

    /*set ant switch handler for 2.4Ghz before initializing BLE stack */
    uble_registerAntSwitchCB(bleAntSettingHandler);

#if defined(FEATURE_STATIC_ADDR)
    uble_stackInit(UB_ADDRTYPE_STATIC, staticAddr, bleAdv_params.pfnPostEvtProxyCB, RF_TIME_RELAXED );
#else  /* FEATURE_STATIC_ADDR */
    uble_stackInit(UB_ADDRTYPE_PUBLIC, NULL, bleAdv_params.pfnPostEvtProxyCB, RF_TIME_RELAXED);
#endif /* FEATURE_STATIC_ADDR */

    /* Initilaize Micro GAP Broadcaster Role */
    ugap_bcastInit(&bcastCBs);

    /* Open LED pins */
    blePinHandle = PIN_open(&blePinState, blePinTable);
    if (!blePinHandle)
    {
        System_abort("Error initializing board 3.3V domain pins\n");
    }

    /* Power antenna switch */
#if defined RF_SW_PWR_PIN
    PIN_setOutputValue(blePinHandle, RF_SW_PWR_PIN, 1);
#endif

    /* initialize the Manufacturer Specific Beacon frame
     */
    propAdvFrame.length = sizeof(propAdvertisement);
    propAdvFrame.pAdvData = propAdvertisement;

    /* initialize the Eddystone Beacon UID frame
     */
    uidAdvFrame.pAdvData = (uint8_t *) &uidAdvData;
    /* Set frame type */
    uidAdvData.frame.uid.frameType = EDDYSTONE_FRAME_TYPE_UID;
    uidAdvData.frame.uid.rangingData = BLEADV_TXPOWER_0M_0DBM;
    /* Set Eddystone UID namespace and instance:
     * - As per spec the namesapce is 10 bytes of the sha-1 hash
     *   of fully-qualified domain name that we own.
     * - The instance name is assinged the IEEE address of the device
     */
    memcpy(uidAdvData.frame.uid.namespaceID, uidNameSpace, 10);
    getBleIeeeAddr(uidInstanceId);
    memcpy(uidAdvData.frame.uid.instanceID, uidInstanceId, 6);
    /* Set Length fileds */
    uidAdvData.length = EDDYSTONE_SVC_DATA_OVERHEAD_LEN + EDDYSTONE_UUID_FRAME_LEN;
    uidAdvFrame.length = EDDYSTONE_FRAME_OVERHEAD_LEN + uidAdvData.length;

    /* initialize the Eddystone Beacon URL frame
     */
    urlAdvFrame.pAdvData = (uint8_t *) &urlAdvData;
    BleAdv_updateUrl(defaultUrl);

    /* initialize the Eddystone Beacon TLM frame
     */
    tlmAdvData.frame.tlm.frameType = EDDYSTONE_FRAME_TYPE_TLM;
    tlmAdvData.frame.tlm.version = 0;
    //Set length
    tlmAdvData.length = EDDYSTONE_SVC_DATA_OVERHEAD_LEN + EDDYSTONE_TLM_FRAME_LEN;
    tlmAdvFrame.length = EDDYSTONE_FRAME_OVERHEAD_LEN + tlmAdvData.length;
    tlmAdvFrame.pAdvData = (uint8_t *) &tlmAdvData;

    configured = true;

    return BleAdv_Status_Success;
}

//*****************************************************************************
//
//! \brief update Eddystone URL Frame
//!
//! This function Updates the URL frame with the url string provided.
//!
//! \param newUrl URL to be encoded into the UL frame
//!
//! \return BleAdv_Status
//!
//*****************************************************************************
BleAdv_Status BleAdv_updateUrl(const char* newUrl)
{
    uint8_t i, j;
    uint8_t urlLen;
    uint8_t tokenLen;
    uint32_t frameSize;
    uint8_t urlFrameLen;
    eddystoneAdvData_t *pEddystoneAdv = (eddystoneAdvData_t*)(urlAdvFrame.pAdvData);
    BleAdv_Status status = BleAdv_Status_Config_Error;

    if (pEddystoneAdv != NULL)
    {
        //Set length
        urlFrameLen = EDDYSTONE_SVC_DATA_OVERHEAD_LEN;

        // Fill frame with 0s first
        memset((uint8_t*) &pEddystoneAdv->frame.url, 0x00, sizeof(eddystoneURL_t));
        //Set frame type
        pEddystoneAdv->frame.url.frameType = EDDYSTONE_FRAME_TYPE_URL;

        //set Tx Power
        pEddystoneAdv->frame.url.txPower = BLEADV_TXPOWER_0M_0DBM;

        //Copy in the prefix code and url
        urlLen = (uint8_t) strlen(newUrl);


        // search for a matching prefix
        for (i = 0; i < EDDYSTONE_URL_PREFIX_MAX; i++)
        {
            tokenLen = strlen(eddystoneURLPrefix[i]);
            if (strncmp(eddystoneURLPrefix[i], newUrl, tokenLen) == 0)
            {
                break;
            }
        }

        if (i == EDDYSTONE_URL_PREFIX_MAX)
        {
            return BleAdv_Status_Param_Error;       // wrong prefix
        }

        // use the matching prefix number
        pEddystoneAdv->frame.url.encodedURL[0] = i;
        newUrl += tokenLen;
        urlLen -= tokenLen;

        // search for a token to be encoded
        for (i = 0; i < urlLen; i++)
        {
            for (j = 0; j < EDDYSTONE_URL_ENCODING_MAX; j++)
            {
                tokenLen = strlen(eddystoneURLEncoding[j]);
                if (strncmp(eddystoneURLEncoding[j], newUrl + i, tokenLen) == 0)
                {
                    // matching part found
                    break;
                }
            }

            if (j < EDDYSTONE_URL_ENCODING_MAX)
            {
                memcpy(&pEddystoneAdv->frame.url.encodedURL[1], newUrl, i);
                // use the encoded byte
                pEddystoneAdv->frame.url.encodedURL[i + 1] = j;
                break;
            }
        }

        if (i < urlLen)
        {
            memcpy(&pEddystoneAdv->frame.url.encodedURL[i + 2],
                   newUrl + i + tokenLen, urlLen - i - tokenLen);

            frameSize = sizeof(eddystoneURL_t) - EDDYSTONE_MAX_URL_LEN + urlLen - tokenLen + 2;
            urlFrameLen += frameSize;

            //Set length
            pEddystoneAdv->length = urlFrameLen;
            urlAdvFrame.length = EDDYSTONE_FRAME_OVERHEAD_LEN + pEddystoneAdv->length;

            return BleAdv_Status_Success;
        }

        //add 1 for the prefix token
        frameSize = sizeof(eddystoneURL_t) - EDDYSTONE_MAX_URL_LEN + urlLen + 1;
        urlFrameLen += frameSize;

        memcpy(&pEddystoneAdv->frame.url.encodedURL[1], newUrl, urlLen - 1);

        //Set length
        pEddystoneAdv->length = urlFrameLen;
        urlAdvFrame.length = EDDYSTONE_FRAME_OVERHEAD_LEN + pEddystoneAdv->length;

        status = BleAdv_Status_Success;
    }

    return status;
}

//*****************************************************************************
//
//! \brief Changes the advertisement type
//!
//! This transmits a beacon.
//!
//! \param beaconFrame Beacon to be Tx'ed
//!
//! \return BleAdv_Status
//!
//*****************************************************************************
BleAdv_Status BleAdv_setAdvertiserType(BleAdv_AdertiserType type)
{

    /* Start/Stop broadcaster if needed */
    if ((advertiserType == BleAdv_AdertiserNone) && (type != BleAdv_AdertiserNone))
    {
        BleAdv_startAdvertising();
    }
    else if (type == BleAdv_AdertiserNone)
    {
        BleAdv_stopAdvertising();
    }

    advertiserType = type;

    return BleAdv_Status_Success;
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

//*****************************************************************************
//
//! \brief Send a beacon
//!
//! This transmits a beacon.
//!
//! \param beaconFrame Beacon to be Tx'ed
//!
//!
//*****************************************************************************
static void updateAdvFrame(BleAdv_Frame *pBeaconFrame)
{
    if(configured)
    {
        uble_setParameter(UB_PARAM_ADVDATA,
                      pBeaconFrame->length,
                      pBeaconFrame->pAdvData);
    }
}

//*****************************************************************************
//
//! \brief Gets the IEEE address
//!
//! This function gets the IEEE address
//!
//! \param ieeeAddr pointer to a 6 element byte array to write the IEEE
//! address to.
//!
//! \return BleAdv_Status
//!
//*****************************************************************************
BleAdv_Status getBleIeeeAddr(uint8_t *ieeeAddr)
{
    BleAdv_Status status = BleAdv_Status_Param_Error;

    if (ieeeAddr != NULL)
    {
        int i;

        //Reading from primary IEEE location...
        uint8_t *location = (uint8_t *)BLE_PRI_IEEE_ADDR_LOCATION;

        /*
         * ...unless we can find a byte != 0xFF in secondary
         *
         * Intentionally checking all 6 bytes here instead of len, because we
         * are checking validity of the entire IEEE address irrespective of the
         * actual number of bytes the caller wants to copy over.
         */
        for (i = 0; i < 6; i++) {
            if (((uint8_t *)BLE_SEC_IEEE_ADDR_LOCATION)[i] != 0xFF) {
                //A byte in the secondary location is not 0xFF. Use the
                //secondary
                location = (uint8_t *)BLE_SEC_IEEE_ADDR_LOCATION;
                break;
            }
        }

        //inverting byte order
       for (i = 0; i < 6; i++) {
           ieeeAddr[i] = location[6 - 1 - i];
       }


        status = BleAdv_Status_Success;
    }

    return status;
}

/*********************************************************************
* @fn      stateChangeCB
*
* @brief   Callback from Micro Broadcaster indicating a state change.
*
* @param   newState - new state
*
* @return  None.
*/
static void stateChangeCB(ugBcastState_t newState)
{
    switch (newState)
    {
        case UG_BCAST_STATE_INITIALIZED:
        {
            // Parameter containers
            uint8  param8;  /* 8-bit parameter */

            // Setup broadcaster duty cycle
            ugap_bcastSetDuty(bleAdv_params.advertisingDutyCycleOn,
                              bleAdv_params.advertisingDutyCycleOff);

            // Setup adv interval
            uble_setParameter(UB_PARAM_ADVINTERVAL, sizeof(uint16), &bleAdv_params.advertisingInterval);

            // Setup adv channel map
            param8 = UB_ADV_CHAN_37 | UB_ADV_CHAN_38 | UB_ADV_CHAN_39;
            uble_setParameter(UB_PARAM_ADVCHANMAP, sizeof(uint8), &param8);

           param8 = 10; /* setup prepare callback for 10ms before adv */
           uble_setParameter(UB_PARAM_TIMETOADV, sizeof(param8), &param8);
        }
        break;

        case UG_BCAST_STATE_IDLE:
        break;

        case UG_BCAST_STATE_ADVERTISING:
        break;

        case UG_BCAST_STATE_WAITING:
        break;

        default:
        break;
    }
}

/*********************************************************************
* @fn      advPrepareCB
*
* @brief   Callback from Micro Broadcaster notifying that the next
*          advertising event is about to start so it's time to update
*          the adv payload.
*
* @param   None.
*
* @return  None.
*/
static void advPrepareCB(void)
{
    //If advertisement is a URL or UUID then interleave a TLM packet on every fourth beacon
    if ((advertiserType == BleAdv_AdertiserUrl) || (advertiserType == BleAdv_AdertiserUid))
    {
        if ((advStats.successCnt % bleAdv_params.tlmInterleaving) == 0)
        {
            //URL or UUID packet, update and intreleave TLM
            if(bleAdv_params.pfnUpdateTlmCB != NULL)
            {
                uint16_t vBatt;
                uint16_t temp;
                uint32_t time100MiliSec;

                bleAdv_params.pfnUpdateTlmCB(&vBatt, &temp, &time100MiliSec);

                tlmAdvData.frame.tlm.vBatt[0] = (vBatt & 0xFF00) >> 8;
                tlmAdvData.frame.tlm.vBatt[1] = vBatt & 0xFF;
                // Temperature
                tlmAdvData.frame.tlm.temp[0] = (temp & 0xFF00) >> 8;;
                tlmAdvData.frame.tlm.temp[1] = temp & 0xFF;
                // advertise packet cnt;
                tlmAdvData.frame.tlm.advCnt[0] = (advStats.successCnt & 0xFF000000) >> 24;
                tlmAdvData.frame.tlm.advCnt[1] = (advStats.successCnt & 0x00FF0000) >> 16;
                tlmAdvData.frame.tlm.advCnt[2] = (advStats.successCnt & 0x0000FF00) >> 8;
                tlmAdvData.frame.tlm.advCnt[3] = advStats.successCnt++ & 0xFF;
                // running time
                tlmAdvData.frame.tlm.secCnt[0] = (time100MiliSec & 0xFF000000) >> 24;
                tlmAdvData.frame.tlm.secCnt[1] = (time100MiliSec & 0x00FF0000) >> 16;
                tlmAdvData.frame.tlm.secCnt[2] = (time100MiliSec & 0x0000FF00) >> 8;
                tlmAdvData.frame.tlm.secCnt[3] = time100MiliSec & 0xFF;
            }
            updateAdvFrame(&tlmAdvFrame);
        }
        if (((advStats.successCnt - 1) % bleAdv_params.tlmInterleaving) == 0)
        {
            //last packet was TLM set back to URL / UUID
            if(advertiserType == BleAdv_AdertiserUrl)
            {
                //advertise url
                updateAdvFrame(&urlAdvFrame);
            }
            else
            {
                //advertise uid
                updateAdvFrame(&uidAdvFrame);
            }
        }
    }
    else if (advertiserType == BleAdv_AdertiserMs)
    {
        //update and advertise ms
        if(bleAdv_params.pfnUpdateMsButtonCB != NULL)
        {
            bleAdv_params.pfnUpdateMsButtonCB(&propAdvertisement[9]);
        }
        updateAdvFrame(&propAdvFrame);

    }

    PIN_setOutputValue(blePinHandle, NODE_BLE_ACTIVITY_LED,1);
}

/*********************************************************************
* @fn      advDoneCB
*
* @brief   Callback from Micro Broadcaster notifying that an
*          advertising event has been done.
*
* @param   status - How the last event was done. SUCCESS or FAILURE.
*
* @return  None.
*/
static void advDoneCB(bStatus_t status)
{
    //Turn off RF switch
#if defined RF_SW_PWR_PIN
    PIN_setOutputValue(blePinHandle, RF_SW_PWR_PIN, 0);
#endif

    if(status == SUCCESS)
    {
        advStats.successCnt++;
    }
    else if(status == bleNoResources)
    {
        advStats.rescheduledCnt++;
    }
    else
    {
        advStats.failCnt++;
    }

    if(bleAdv_params.pfnAdvStatsCB != NULL)
    {
        bleAdv_params.pfnAdvStatsCB(advStats);
    }

    /* Toggle activity LED */
    PIN_setOutputValue(blePinHandle, NODE_BLE_ACTIVITY_LED,0);

}

/*!
 * @brief       2.4 GHz Antenna switch setting.
 */
static void bleAntSettingHandler(void)
{
    //Turn on RF switch
#if defined RF_SW_PWR_PIN
    PIN_setOutputValue(blePinHandle, RF_SW_PWR_PIN, 1);
#endif
    //Swtich RF switch to 2.4G antenna
#if defined RF_SWITCH_PIN
    PIN_setOutputValue(blePinHandle, RF_SWITCH_PIN, 0);
#endif
}

/*********************************************************************
*********************************************************************/
