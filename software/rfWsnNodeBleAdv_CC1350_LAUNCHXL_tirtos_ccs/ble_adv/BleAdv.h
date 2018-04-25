/**************************************************************************************************
  Filename:       BleAdv.h

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

#ifndef BleAdv_H
#define BleAdv_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <stdbool.h>

#include "uble.h"
#include "ugap.h"

#if defined __CC1352R1_LAUNCHXL_BOARD_H__
    #define RF_SWITCH_PIN Board_DIO30_RFSW
#elif defined __CC1350STK_BOARD_H__
    #define RF_SWITCH_PIN IOID_1
#else
    #define RF_SWITCH_PIN Board_DIO1_RFSW
#endif

#if defined __CC1350STK_BOARD_H__
    #define RF_SW_PWR_PIN IOID_30
#elif defined __CC1350_LAUNCHXL_BOARD_H__
    #define RF_SW_PWR_PIN Board_DIO30_SWPWR
#endif

/*********************************************************************
*  EXTERNAL VARIABLES
*/
extern PIN_Handle blePinHandle;

/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * TYPEDEFS
 */

/**
 *  @brief BLE Advertisement Status and error codes
 */
typedef enum
{
    BleAdv_Status_Success         = 0, ///<Success
    BleAdv_Status_Config_Error    = 1, ///<Configuration error
    BleAdv_Status_Param_Error     = 2, ///<Param error
} BleAdv_Status;


/*
 * @brief Type of the callback to update the TLM data
 *
 * @param button Button state to be added to MS beacon Frame
 * @param temp Current temperature
 * @param time100MiliSec time since boot in 100ms units
 */
typedef void (*BleAdv_updateTlmCB_t)(uint16_t *pVbatt, uint16_t *pTemp, uint32_t *pTime100MiliSec);

/**
 * @brief Type of the callback to update the MS button data
 *
 * @param button Button state to be added to MS beacon Frame
 *
 */
typedef void (*BleAdv_updateMsButtonCB_t)(uint8_t *pButton);

/// @brief Structure for the BLE Advertisement Statistics
typedef struct
{
    uint32_t successCnt;            ///successful advertisement packets sent
    uint32_t failCnt;               ///failed advertisement packets sent
    uint32_t rescheduledCnt;           ///rescheduled advertisement packets sent
} BleAdv_Stats;


/**
 * @brief Type of the callback to indicating adv statistics change
 *
 * @param BleAdv_Stats SuccessCnt FailCnt and rescheduleCnt
 *
 */
typedef void (*BleAdv_advStatsCB_t)(BleAdv_Stats stats);

/**
 * @brief BleAdv parameter struct
 *  BleAdv parameters are used with the BleAdv_init() and BleAdv_Params_init() call.
 */
typedef struct {
    uint16_t advertisingInterval;            ///< Advertising interval in units of 625us
    uint8_t advertisingDutyCycleOn;          ///< The amount of time advertising at the advertising
                                             ///  interval during the advertising period in units of 100ms
    uint8_t advertisingDutyCycleOff;         ///< The amount of time advertising is off during the
                                             ///  advertising period in units of 100ms
    uint8_t tlmInterleaving;                 ///< The number of URL or UID advertisements before a TLM packet
                                             ///  is interleaved (0 to disable TLM interleaving)
    ubPostEvtProxyCB_t pfnPostEvtProxyCB;    ///< uBLE event process CB
    BleAdv_updateTlmCB_t pfnUpdateTlmCB;             ///< TLM update CB
    BleAdv_updateMsButtonCB_t pfnUpdateMsButtonCB;   ///< MS update CB
    BleAdv_advStatsCB_t pfnAdvStatsCB;         ///< Beacon statistics change callback
} BleAdv_Params_t;

/// @brief Structure for the BLE Advertisement Packet
typedef struct
{
    uint8_t length;               ///Advertisement packet length
    uint8_t *pAdvData;            ///Advertisement data
} BleAdv_Frame;

typedef enum
{
    BleAdv_AdertiserNone =     0, //None
    BleAdv_AdertiserMs =       1, //Manufacturer Specific advertisement
    BleAdv_AdertiserUrl =      2, //Eddystone interleaved URL and TLM
    BleAdv_AdertiserUid =      3, //Eddystone interleaved UID and TLM
    BleAdv_AdertiserTypeEnd =  4, //End of advertisement type enum's
}BleAdv_AdertiserType;

/*********************************************************************
 * FUNCTIONS
 */
/** @brief  Function to initialize the BleAdv_Params struct to its defaults
 *
 *  @param  params      An pointer to RF_Params structure for
 *                      initialization
 *
 *  Defaults values are:
 *     advertisingInterval    = 100ms
 *     advertisingPeriod      = 2.5s
 *     advertisingDutyCycleOn = 500ms
 *     tlmInterleaving        = 4 packets
 *     pfnPostEvtProxyCB      = {0}
 *     pfnUpdateTlmCB         = {0}
 *     pfnUpdateMsButtonCB    = {0}
 */
extern void BleAdv_Params_init(BleAdv_Params_t *params);

//*****************************************************************************
//
//! \brief Initialize the BleAdv module
//!
//! Initialize the BleAdv module.
//!
//!
//! \return BleAdv_Status
//!
//*****************************************************************************
BleAdv_Status BleAdv_init(BleAdv_Params_t *params);

//*****************************************************************************
//
//! \brief update Eddystone URL Frame
//!
//! This function Updates the URL frame with the url string provided.
//!
//! \param urlOrg URL to be encoded into the UL frame
//!
//! \return BleAdv_Status
//!
//*****************************************************************************
BleAdv_Status BleAdv_updateUrl(const char* newUrl);

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
BleAdv_Status BleAdv_setAdvertiserType(BleAdv_AdertiserType type);

//*****************************************************************************
//
//! \brief Start advertising
//!
//! Closes the BleAdv module.
//!
//!
//! \return BleAdv_Status
//!
//*****************************************************************************
inline BleAdv_Status BleAdv_startAdvertising(void)
{
    ugap_bcastStart(0);
    return BleAdv_Status_Success;
}

//*****************************************************************************
//
//! \brief Stop advertising
//!
//! Closes the BleAdv module.
//!
//!
//! \return BleAdv_Status
//!
//*****************************************************************************
inline BleAdv_Status BleAdv_stopAdvertising(void)
{
    ugap_bcastStop();
    return BleAdv_Status_Success;
}

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* BleAdv_H */
