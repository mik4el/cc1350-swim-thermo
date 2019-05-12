/******************************************************************************

 @file  urfi.c

 @brief This file contains the RF driver interfacing API for the Micro
        BLE Stack.

 Group: WCS, BTS
 Target Device: CC13xx

 ******************************************************************************
 
 Copyright (c) 2009-2018, Texas Instruments Incorporated
 All rights reserved.

 IMPORTANT: Your use of this Software is limited to those specific rights
 granted under the terms of a software license agreement between the user
 who downloaded the software, his/her employer (which must be your employer)
 and Texas Instruments Incorporated (the "License"). You may not use this
 Software unless you agree to abide by the terms of the License. The License
 limits your use, and you acknowledge, that the Software may not be modified,
 copied or distributed unless embedded on a Texas Instruments microcontroller
 or used solely and exclusively in conjunction with a Texas Instruments radio
 frequency transceiver, which is integrated into your product. Other than for
 the foregoing purpose, you may not use, reproduce, copy, prepare derivative
 works of, modify, distribute, perform, display or sell this Software and/or
 its documentation for any purpose.

 YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
 PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
 NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
 TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
 NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
 LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
 INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
 OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
 OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
 (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

 Should you have any questions regarding your right to use this Software,
 contact Texas Instruments Incorporated at www.TI.com.

 ******************************************************************************
 Release Name: simplelink_cc13x0_sdk_2_10_00_
 Release Date: 2018-04-09 00:04:23
 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <ti/sysbios/knl/Swi.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_ble_cmd.h)
#include DeviceFamily_constructPath(driverlib/rf_ble_mailbox.h)

#ifdef USE_USER_CONFIG
#include "mac_user_config.h"
#else
#include <ti/drivers/rf/RF.h>
#endif

#include <bcomdef.h>

#include <ll_common.h>
#include <urfi.h>
#include <uble.h>
#include <ull.h>

/*********************************************************************
 * CONSTANTS
 */

#if (!defined(RF_SINGLE_MODE) && !defined(RF_MULTI_MODE)) ||                   \
    (defined(RF_SINGLE_MODE) && defined(RF_MULTI_MODE))
  #error "Either RF_SINGLE_MODE or RF_MULTI_MODE should be defined."
#endif /* RF_SINGLE_MODE, RF_MULTI_MODE */
      
#if (defined(FEATURE_SCANNER) && defined(FEATURE_MONITOR))
  #error "FEATURE_SCANNER and FEATURE_MONITOR cannot be defined at the same time."
#endif /* FEATURE_SCANNER, FEATURE_MONITOR */      

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

extern ubParams_t ubParams;
extern uint8      ubBDAddr[];
extern uint8      rfTimeCrit;
extern pfnAntSwitchCB_t ubleAntSwitchSel;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
extern bStatus_t uble_buildAndPostEvt(ubEvtDst_t evtDst, ubEvt_t evt,
                                      ubMsg_t *pMsg, uint16 len);
extern dataEntryQ_t *ull_setupScanDataEntryQueue( void );
extern dataEntryQ_t *ull_setupMonitorDataEntryQueue( void );

/*********************************************************************
 * GLOBAL VARIABLES
 */

RF_Handle    urHandle = NULL;

/* Radio Setup Parameters.
 * config, txPower, and pRegOverride will be initialized at runtime.
 */
RF_RadioSetup urSetup =
{
  .common.commandNo                = CMD_RADIO_SETUP,
  .common.status                   = IDLE,
  .common.pNextOp                  = NULL,
  .common.startTime                = 0,
  .common.startTrigger.triggerType = TRIG_NOW,
  .common.startTrigger.bEnaCmd     = 0,
  .common.startTrigger.triggerNo   = 0,
  .common.startTrigger.pastTrig    = 0,
  .common.condition.rule           = COND_NEVER,
  .common.condition.nSkip          = 0,
  .common.mode                     = 0,
};

#if defined(FEATURE_ADVERTISER)
RF_CmdHandle urAdvHandle = UR_CMD_HANDLE_INVALID;

/* CMD_BLE_ADV_XX Params */
rfc_bleAdvPar_t urAdvParams =
{
  .pRxQ = 0,
  .rxConfig.bAutoFlushIgnored = 0,
  .rxConfig.bAutoFlushCrcErr = 0,
  .rxConfig.bAutoFlushEmpty = 0,
  .rxConfig.bIncludeLenByte = 0,
  .rxConfig.bIncludeCrc = 0,
  .rxConfig.bAppendRssi = 0,
  .rxConfig.bAppendStatus = 0,
  .rxConfig.bAppendTimestamp = 0,
  .advConfig.advFilterPolicy = 0,
  .advConfig.deviceAddrType = 0,
  .advConfig.peerAddrType = 0,
  .advConfig.bStrictLenFilter = 0,
  .advLen = 0,
  .scanRspLen = 0,
  .pAdvData = ubParams.advData,
#if defined(FEATURE_SCAN_RESPONSE)
  .pScanRspData = ubParams.scanRspData,
#else   /* FEATURE_SCAN_RESPONSE */
  .pScanRspData = 0,
#endif  /* FEATURE_SCAN_RESPONSE */
  .pDeviceAddress = (uint16*) ubBDAddr,
  .pWhiteList = 0,
  .__dummy0 = 0,
  .__dummy1 = 0,
  .endTrigger.triggerType = TRIG_NEVER,
  .endTrigger.bEnaCmd = 0,
  .endTrigger.triggerNo = 0,
  .endTrigger.pastTrig = 0,
  .endTime = 0,
};

#if defined(FEATURE_SCAN_RESPONSE)
/* CMD_BLE_ADV_XX Output */
rfc_bleAdvOutput_t urAdvOutput;
#endif  /* FEATURE_SCAN_RESPONSE */

/* CMD_BLE_ADV_XX Command */
rfc_CMD_BLE_ADV_COMMON_t urAdvCmd[3];

#endif  /* FEATURE_ADVERTISER */

#if defined(FEATURE_SCANNER)
RF_CmdHandle urScanHandle = UR_CMD_HANDLE_INVALID;

/* CMD_BLE_SCANNER Params */
rfc_bleScannerPar_t urScanParams =
{
  .pRxQ = 0,
  .rxConfig.bAutoFlushIgnored = 1,
  .rxConfig.bAutoFlushCrcErr = 1,
  .rxConfig.bAutoFlushEmpty = 1,
  .rxConfig.bIncludeLenByte = 1,
  .rxConfig.bIncludeCrc = 0,
  .rxConfig.bAppendRssi = 1,
  .rxConfig.bAppendStatus = 1,
  .rxConfig.bAppendTimestamp = 1,
  .scanConfig.scanFilterPolicy = ULL_SCAN_WL_POLICY_ANY_ADV_PKTS,
  .scanConfig.bActiveScan = 0,  /* Passive scan only */
  .scanConfig.deviceAddrType = 0,
  .scanConfig.rpaFilterPolicy = 0,
  .scanConfig.bStrictLenFilter = 1,
  .scanConfig.bAutoWlIgnore = 0,
  .scanConfig.bEndOnRpt = 0, /* Must continue to scan */
  .scanConfig.rpaMode = 0,
  .randomState = 0,
  .backoffCount = 1,
  .backoffPar = 0,
  .scanReqLen = 0,
  .pScanReqData = 0,
  .pDeviceAddress = 0,
  .pWhiteList = 0,
  .__dummy0 = 0,
  .timeoutTrigger.triggerType = TRIG_NEVER,
  .timeoutTrigger.bEnaCmd = 0,
  .timeoutTrigger.triggerNo = 0,
  .timeoutTrigger.pastTrig = 1,
  .endTrigger.triggerType = TRIG_NEVER,
  .endTrigger.bEnaCmd = 0,
  .endTrigger.triggerNo = 0,
  .endTrigger.pastTrig = 0,
  .timeoutTime = 0,
  .endTime = 0,
};

/* CMD_BLE_SCANNER outputs */
rfc_bleScannerOutput_t pScanOutput;

/* CMD_BLE_SCANNER Command */
rfc_CMD_BLE_SCANNER_t urScanCmd;

#endif  /* FEATURE_SCANNER */

#if defined(FEATURE_MONITOR)
RF_CmdHandle urGenericRxHandle = UR_CMD_HANDLE_INVALID;

/* CMD_BLE_GENERIC_RX Params */
rfc_bleGenericRxPar_t urGenericRxParams =
{
  .pRxQ = 0,
  .rxConfig.bAutoFlushIgnored = 0, // Should never turn on for generic Rx cmd
  .rxConfig.bAutoFlushCrcErr = 0, // TBD: receiving with CRC error for now
  .rxConfig.bAutoFlushEmpty = 1,
  .rxConfig.bIncludeLenByte = 1,
  .rxConfig.bIncludeCrc = 0,
  .rxConfig.bAppendRssi = 1,
  .rxConfig.bAppendStatus = 1,
  .rxConfig.bAppendTimestamp = 1,
  .bRepeat = 1,
  .__dummy0 = 0,
  .accessAddress = 0,
  .crcInit0 = 0x55, // TBD: Adv packets for now
  .crcInit1 = 0x55,
  .crcInit2 = 0x55,
  .endTrigger.triggerType = TRIG_NEVER,
  .endTrigger.bEnaCmd = 0,
  .endTrigger.triggerNo = 0,
  .endTrigger.pastTrig = 1,
  .endTime = 0,
};

/* CMD_BLE_GENERIC_RX outputs */
rfc_bleGenericRxOutput_t pGenericRxOutput;

/* CMD_BLE_GENERIC_RX Command */
rfc_CMD_BLE_GENERIC_RX_t urGenericRxCmd;

#endif  /* FEATURE_MONITOR */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static RF_Object urObject;
static RF_Params urParams;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      urfi_initAdvCmd
 *
 * @brief   Initialize Adv RF command
 *
 * @param   None
 *
 * @return  None
 */
#if defined(FEATURE_ADVERTISER)
void urfi_initAdvCmd(void)
{
  uint8 i;

  for (i = 0; i < 3; i++)
  {
    /* Advertising channel */
    urAdvCmd[i].channel                = 37 + i;

    urAdvCmd[i].whitening.init         = 0; /* No whitening */
    urAdvCmd[i].pParams                = &urAdvParams;

    urAdvCmd[i].startTrigger.bEnaCmd   = 0;

  #if defined(FEATURE_SCAN_RESPONSE)
    urAdvCmd[i].pOutput                = &urAdvOutput;
  #else  /* FEATURE_SCAN_RESPONSE */
    urAdvCmd[i].pOutput                = NULL;
  #endif /* FEATURE_SCAN_RESPONSE */
  }

  /* 1st channel adv is supposed to start at a certain time */
  urAdvCmd[0].startTrigger.triggerType = TRIG_ABSTIME;
  /* 2nd and 3rd channel adv's are supposed to start as soon as
     the previous channel operation ends */
  urAdvCmd[1].startTrigger.triggerType =
  urAdvCmd[2].startTrigger.triggerType = TRIG_NOW;

  urAdvCmd[1].startTime                =
  urAdvCmd[2].startTime                = 0;

  urAdvCmd[0].condition.rule           =
  urAdvCmd[1].condition.rule           = COND_STOP_ON_FALSE;
  urAdvCmd[2].condition.rule           = COND_NEVER;

#ifdef RF_MULTI_MODE
  if (RF_TIME_RELAXED == rfTimeCrit)
  {
    urAdvCmd[0].startTrigger.pastTrig    =
    urAdvCmd[1].startTrigger.pastTrig    =
    urAdvCmd[2].startTrigger.pastTrig    = 1;
  }
  else
  {
    urAdvCmd[0].startTrigger.pastTrig    =
    urAdvCmd[1].startTrigger.pastTrig    =
    urAdvCmd[2].startTrigger.pastTrig    = 0;
  }
#endif /* RF_MULTI_MODE */

  urAdvCmd[0].pNextOp                  = (rfc_radioOp_t*) &urAdvCmd[1];
  urAdvCmd[1].pNextOp                  = (rfc_radioOp_t*) &urAdvCmd[2];
  urAdvCmd[2].pNextOp                  = NULL;
}
#endif /* FEATURE_ADVERTISER */

/*********************************************************************
 * @fn      urfi_initScanCmd
 *
 * @brief   Initialize Scan RF command
 *
 * @param   None
 *
 * @return  None
 */
#if defined(FEATURE_SCANNER)
void urfi_initScanCmd(void)
{
  urScanCmd.commandNo = CMD_BLE_SCANNER;
  urScanCmd.status = IDLE;
  urScanCmd.pNextOp = NULL;
  urScanCmd.startTime = 0;
  urScanCmd.startTrigger.triggerType = TRIG_NOW;
  urScanCmd.startTrigger.bEnaCmd = 0;
  urScanCmd.startTrigger.triggerNo = 0;
  
  /* uGAP controls the scan timing. Scanning late should be allowed. */
  urScanCmd.startTrigger.pastTrig = 1;

  urScanCmd.condition.rule = COND_NEVER;
  urScanCmd.condition.nSkip = 0;
  
  urScanCmd.channel = 37;
  urScanCmd.whitening.init = 0;
  urScanCmd.whitening.bOverride = 0;
  urScanCmd.pParams = &urScanParams;
  urScanCmd.pOutput = &pScanOutput;
  
  urScanParams.pRxQ = (dataQueue_t *)ull_setupScanDataEntryQueue();
}
#endif /* FEATURE_SCANNER */

#if defined(FEATURE_MONITOR)
/*********************************************************************
 * @fn      urfi_initGenericRxCmd
 *
 * @brief   Initialize Scan RF command
 *
 * @param   None
 *
 * @return  None
 */
void urfi_initGenericRxCmd(void)
{
  urGenericRxCmd.commandNo = CMD_BLE_GENERIC_RX;
  urGenericRxCmd.status = IDLE;
  urGenericRxCmd.pNextOp = NULL;
  urGenericRxCmd.startTime = 0;
  urGenericRxCmd.startTrigger.triggerType = TRIG_ABSTIME;
  urGenericRxCmd.startTrigger.bEnaCmd = 0;
  urGenericRxCmd.startTrigger.triggerNo = 0;
  
#ifdef RF_MULTI_MODE
  urGenericRxCmd.startTrigger.pastTrig = 
    (RF_TIME_RELAXED == rfTimeCrit) ? 1 : 0;
#endif /* RF_MULTI_MODE */

  urGenericRxCmd.condition.rule = COND_NEVER;
  urGenericRxCmd.condition.nSkip = 0;
  
  urGenericRxCmd.channel = 37;
  urGenericRxCmd.whitening.init = 0;
  urGenericRxCmd.whitening.bOverride = 0;

  urGenericRxCmd.pParams = &urGenericRxParams;
  urGenericRxCmd.pOutput = &pGenericRxOutput;
  
  urGenericRxParams.pRxQ = (dataQueue_t *)ull_setupMonitorDataEntryQueue();
}
#endif /* FEATURE_MONITOR */

/*********************************************************************
 * @fn      urfi_clientEventCb
 *
 * @brief   Callback function to be invoked by RF driver
 *
 * @param   rfHandle - RF client handle
 *
 * @param   cmdHandle - RF command handle
 * @param   events - RF client events
 * @param   arg - reserved for future use
 *
 * @return  none
 */
void urfi_clientEventCb(RF_Handle h, RF_ClientEvent events, void* arg)
{
  volatile uint32 keySwi;

  keySwi = Swi_disable();

  if (events & RF_ClientEventRadioFree)
  {
#if defined(FEATURE_ADVERTISER)
    uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_ADV_TX_RADIO_AVAILABLE, NULL, 0);
#endif /* FEATURE_ADVERTISER */
#if defined(FEATURE_SCANNER)
    uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_SCAN_RX_RADIO_AVAILABLE, NULL, 0);   
#endif /* FEATURE_SCANNER */
#if defined(FEATURE_MONITOR)
    uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_MONITOR_RX_RADIO_AVAILABLE, NULL, 0);
#endif /* FEATURE_MONITOR */
  }

  if (events & RF_ClientEventSwitchClientEntered)
  {
    if (ubleAntSwitchSel != NULL)
    {
      ubleAntSwitchSel();
    }
  }

  Swi_restore(keySwi);
}

/*********************************************************************
 * @fn      urfi_init
 *
 * @brief   Initialize radio interface and radio commands
 *
 * @param   None
 *
 * @return  SUCCESS - RF driver has been successfully opened
 *          FAILURE - Failed to open RF driver
 */
bStatus_t urfi_init(void)
{
  if (urHandle == NULL)
  {
    RF_Params_init(&urParams); /* Get default values from RF driver */
    urParams.nInactivityTimeout = 0; /* Do not use the default value for this */
    urParams.pClientEventCb = urfi_clientEventCb;
    urParams.nClientEventMask =  RF_ClientEventRadioFree | RF_ClientEventSwitchClientEntered;

    /* Setup radio setup command.
     * TODO: Make more variables user-configurable
     */
    /* Differential mode */
    urSetup.common.config.frontEndMode  = ubFeModeBias & 0x07;
    /* Internal bias */
    urSetup.common.config.biasMode      = (ubFeModeBias & 0x08) >> 3;
    /* Keep analog configuration */
    urSetup.common.config.analogCfgMode = 0x2D;
    /* Power up frequency synth */
    urSetup.common.config.bNoFsPowerUp  =    0;
    /* 0 dBm */
    urSetup.common.txPower            = urfi_getTxPowerVal(UB_PARAM_DFLT_TXPOWER);
    /* Register Overrides */
    urSetup.common.pRegOverride       = (uint32_t*) ubRfRegOverride;

    /* Request access to the radio */
    urHandle = RF_open(&urObject, (RF_Mode*) &ubRfMode, &urSetup, &urParams);

    if (urHandle == NULL)
    {
      return FAILURE;
    }
  }

#if defined(FEATURE_ADVERTISER)
  urfi_initAdvCmd();
#endif  /* FEATURE_ADVERTISER */

#if defined(FEATURE_SCANNER)
  urfi_initScanCmd();
#endif  /* FEATURE_SCANNER */
  
#if defined(FEATURE_MONITOR)
  urfi_initGenericRxCmd();
#endif  /* FEATURE_SCANNER */
  
  return SUCCESS;
}

/*********************************************************************
 * CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      urfi_getTxPowerVal
 *
 * @brief   Get the value, corresponding with the given TX Power,
 *          to be used to setup the radio accordingly.
 *
 * @param   txPower - TX Power in dBm.
 *
 * @return  The register value correspondign with txPower, if found.
 *          UB_TX_POWER_INVALID otherwise.
 */
uint16 urfi_getTxPowerVal(int8 txPower)
{
  uint8 i;

  for (i = 0; i < ubTxPowerTable.numTxPowerVal; i++)
  {
    if (ubTxPowerTable.pTxPowerVals[i].dBm == txPower)
    {
      return ubTxPowerTable.pTxPowerVals[i].txPowerVal;
    }
  }

  return UB_TX_POWER_INVALID;
}


/*********************************************************************
*********************************************************************/
