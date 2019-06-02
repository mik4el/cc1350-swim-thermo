/******************************************************************************

 @file  ull.c

 @brief This file contains the Micro Link Layer (uLL) API for the Micro
        BLE Stack.

        This API is mostly based on the Bluetooth Core Specification,
        V4.2, Vol. 6.

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
#include <stdlib.h>

#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Swi.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/ioc.h)

#ifdef USE_ICALL
#include <icall.h>
#endif

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_ble_mailbox.h)

#ifdef USE_USER_CONFIG
#include "mac_user_config.h"
#else
#include <ti/drivers/rf/RF.h>
#endif

#include <bcomdef.h>

#include <uble.h>
#include <urfi.h>
#include <ull.h>

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/* uLL State */
#define UL_STATE_STANDBY               0
#define UL_STATE_ADVERTISING           1
#define UL_STATE_SCANNING              2
#define UL_STATE_MONITORING            3

/* Clock Event for about to advertising */
#define UL_CLKEVT_ADV_ABOUT_TO         1
/* Clock event for advertisement interval */
#define UL_CLKEVT_ADV_INT_EXPIRED      2

/* Time gap in RAT ticks between adv commands in the same command chain */
#define UL_ADV_GAP_IN_CHAIN  ((uint32) (374.5 * US_TO_RAT))

/* uLL adv tx status */
#define UL_ADV_TX_DONE                 1
#define UL_ADV_TX_SCHEDULED            2
#define UL_ADV_TX_NO_RF_RESOURCE       3
#define UL_ADV_TX_FAILED               4

/* uLL advertisement modes */
#define UL_ADV_MODE_START              1
#define UL_ADV_MODE_IMMEDIATE          2
#define UL_ADV_MODE_PERIODIC           3
#define UL_ADV_MODE_PERIODIC_RELAXED   4
#define UL_ADV_MODE_RESCHEDULE         5

/* uLL scan rx status */
#define UL_SCAN_RX_SCHEDULED           1
#define UL_SCAN_RX_NO_RF_RESOURCE      2
#define UL_SCAN_RX_FAILED              3

/* uLL scanning modes */
#define UL_SCAN_MODE_START             1
#define UL_SCAN_MODE_RESCHEDULE        2

/* uLL monitor rx status */
#define UL_MONITOR_RX_SCHEDULED        1
#define UL_MONITOR_RX_NO_RF_RESOURCE   2
#define UL_MONITOR_RX_FAILED           3

/* uLL moniotr modes */
#define UL_MONITOR_MODE_START          1
#define UL_MONITOR_MODE_RESCHEDULE     2

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
/* Micro Link Layer State */
uint8 ulState = UL_STATE_STANDBY;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
extern bStatus_t uble_buildAndPostEvt(ubEvtDst_t evtDst, ubEvt_t evt,
                                      ubMsg_t *pMsg, uint16 len);

/*********************************************************************
 * EXTERNAL VARIABLES
 */

extern ubParams_t ubParams;
extern rfc_CMD_BLE_ADV_COMMON_t urAdvCmd[];
extern uint8 rfTimeCrit;
extern rfc_CMD_BLE_SCANNER_t urScanCmd;
extern rfc_CMD_BLE_GENERIC_RX_t urGenericRxCmd;

/*********************************************************************
 * LOCAL VARIABLES
 */

#if defined(FEATURE_ADVERTISER)
/* Advertiser callbacks */
static pfnAdvAboutToCB_t ull_notifyAdvAboutTo;
static pfnAdvDoneCB_t    ull_notifyAdvDone;
/* Clock object used for "about to advertise" notification */
static Clock_Struct cAdvAboutTo;
static Clock_Handle hcAdvAboutTo = NULL;

/* Advertisement status */
//static uint8 advTxStatus = UL_ADV_TX_DONE; //scw temp move to compile wo FEATURE_ADVERTISER

/* Clock object used to track the advertisement interval */
static Clock_Struct cAdvInt;

#if defined(RF_MULTI_MODE)
static Clock_Handle hcAdvInt = NULL;
#endif /* RF_MULTI_MODE */

#endif /* FEATURE_ADVERTISER */
static uint8 advTxStatus = UL_ADV_TX_DONE;

#if defined(FEATURE_SCANNER)
/* Scanner callbacks */
static pfnScanIndCB_t ull_notifyScanIndication;
static pfnScanWindowCompCB_t ull_notifyScanWindowComplete;

/* Scan status */
static uint8 scanRxStatus = UL_SCAN_RX_SCHEDULED;

/* Static Data Entries */
struct
{
  dataEntry_t entry;
  uint8       data[ ULL_ADV_ALIGNED_BUFFER_SIZE ];
} ull_scanDataEntry[ ULL_NUM_RX_SCAN_ENTRIES ];

/* Scan Data Queue */
dataQ_t       ull_scanDataQueue;

uint8 advPkt[ ULL_PKT_HDR_LEN + ULL_MAX_BLE_ADV_PKT_SIZE + ULL_SUFFIX_MAX_SIZE - ULL_SUFFIX_CRC_SIZE];

#endif /* FEATURE_SCANNER */

#if defined(FEATURE_MONITOR)
/* Monitor callbacks */
static pfnMonitorIndCB_t ull_notifyMonitorIndication;
static pfnMonitorCompCB_t ull_notifyMonitorComplete;

/* Monitor status */
static uint8 monitorRxStatus = UL_MONITOR_RX_SCHEDULED;

/* Monitor Session ID */
static uint8_t ull_sessionId;

/* Static Data Entries */
struct
{
  dataEntry_t entry;
  uint8       data[ ULL_BLE_ALIGNED_BUFFER_SIZE ];
} ull_monitorDataEntry[ ULL_NUM_RX_MONITOR_ENTRIES ];

/* Scan Data Queue */
dataQ_t       ull_monitorDataQueue;

uint8 monitorPkt[ ULL_PKT_HDR_LEN + ULL_MAX_BLE_PKT_SIZE + ULL_SUFFIX_MAX_SIZE - ULL_SUFFIX_CRC_SIZE ];

#endif /* FEATURE_MONITOR */

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */
bStatus_t ull_advSchedule(uint8 mode);

#if defined(FEATURE_SCANNER)
/*******************************************************************************
 * @fn          ull_setupScanDataEntryQueue
 *
 * @brief       This routine is used to setup a static ring buffer.
 *
 * input parameters
 *
 * @param
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Pointer to the Rx Data Entry Queue.
 */
dataEntryQ_t *ull_setupScanDataEntryQueue( void )
{
  /* init data entry */
  for ( uint8 i=0; i<ULL_NUM_RX_SCAN_ENTRIES; i++ )
  {
    ull_scanDataEntry[i].entry.status     = 0;
    ull_scanDataEntry[i].entry.config     = 0;
    ull_scanDataEntry[i].entry.length     = ULL_ADV_ALIGNED_BUFFER_SIZE;
    ull_scanDataEntry[i].entry.pNextEntry = &ull_scanDataEntry[i+1].entry;
  }

  /* adjust last entry to wrap */
  ull_scanDataEntry[ULL_NUM_RX_SCAN_ENTRIES-1].entry.pNextEntry = &ull_scanDataEntry[0].entry;

  /* init data queue */
  ull_scanDataQueue.dataEntryQ.pCurEntry  = &ull_scanDataEntry[0].entry;
  ull_scanDataQueue.dataEntryQ.pLastEntry = NULL;
  ull_scanDataQueue.pNextDataEntry        = &ull_scanDataEntry[0].entry;
  ull_scanDataQueue.pTempDataEntry        = NULL;

  return( &ull_scanDataQueue.dataEntryQ );
}
#endif /* FEATURE_SCANNER */

#if defined(FEATURE_MONITOR)
/*******************************************************************************
 * @fn          ull_setupMonitorDataEntryQueue
 *
 * @brief       This routine is used to setup a static ring buffer.
 *
 * input parameters
 *
 * @param
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Pointer to the Rx Data Entry Queue.
 */
dataEntryQ_t *ull_setupMonitorDataEntryQueue( void )
{
  /* init data entry */
  for ( uint8 i=0; i<ULL_NUM_RX_MONITOR_ENTRIES; i++ )
  {
    ull_monitorDataEntry[i].entry.status     = 0;
    ull_monitorDataEntry[i].entry.config     = 0;
    ull_monitorDataEntry[i].entry.length     = ULL_BLE_ALIGNED_BUFFER_SIZE;
    ull_monitorDataEntry[i].entry.pNextEntry = &ull_monitorDataEntry[i+1].entry;
  }

  /* adjust last entry to wrap */
  ull_monitorDataEntry[ULL_NUM_RX_MONITOR_ENTRIES-1].entry.pNextEntry = &ull_monitorDataEntry[0].entry;

  /* init data queue */
  ull_monitorDataQueue.dataEntryQ.pCurEntry  = &ull_monitorDataEntry[0].entry;
  ull_monitorDataQueue.dataEntryQ.pLastEntry = NULL;
  ull_monitorDataQueue.pNextDataEntry        = &ull_monitorDataEntry[0].entry;
  ull_monitorDataQueue.pTempDataEntry        = NULL;

  return( &ull_monitorDataQueue.dataEntryQ );
}
#endif /* FEATUR_MONITOR */

#if defined(FEATURE_SCANNER) || defined(FEATURE_MONITOR)
/*******************************************************************************
 * @fn          ull_getNextDataEntry
 *
 * @brief       This function is used to return a pointer to the next data
 *              entry in the data entry queue that is available for System
 *              processing. Note that this does not necessarily mean the data
 *              entry has be Finished by the radio - to determine this, the
 *              data entry status would have to be first checked. This is only
 *              the data entry to would be processed next by System software.
 *
 * input parameters
 *
 * @param       dataEntryQueue_t - Pointer to data entry queue.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Pointer to next data entry of a data queue to be processed.
 */
dataEntry_t *ull_getNextDataEntry( dataEntryQ_t *pDataEntryQ )
{
  /* return next data entry to may be processed by System software */
  return( (dataEntry_t *)((dataQ_t *)pDataEntryQ)->pNextDataEntry );
}

/*******************************************************************************
 * @fn          ull_nextDataEntryDone
 *
 * @brief       This function is used to mark the next System data entry on a
 *              data entry queue as Pending so that the radio can once again
 *              use it. It should be called after the user has processed the
 *              data entry.
 *
 * input parameters
 *
 * @param       dataEntryQueue_t - Pointer to data entry queue.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void ull_nextDataEntryDone( dataEntryQ_t *pDataEntryQ )
{
  dataQ_t         *pDataQueue;
  volatile uint32 keyHwi;

  keyHwi = Hwi_disable();

  /* point to data queue */
  pDataQueue = (dataQ_t *)pDataEntryQ;

  if ( pDataQueue->pNextDataEntry != NULL )
  {
    /* mark the next System data entry as Pending */
    pDataQueue->pNextDataEntry->status = DATA_ENTRY_PENDING;

    /* advance to the next data entry in the data entry queue */
    pDataQueue->pNextDataEntry = pDataQueue->pNextDataEntry->pNextEntry;

    Hwi_restore(keyHwi);

    /* return pointer to next entry, or NULL if there isn't one
     * Note: For a ring buffer, there is always another.
     */
    return;
  }

  Hwi_restore(keyHwi);

  /* return next data entry to may be processed by System software */
  return;
}

/*******************************************************************************
 * @fn          ull_flushAllDataEntry
 *
 * @brief       This function is used to mark the all System data entry on a
 *              data entry queue as Pending so that the radio can once again
 *              use all available data entry queue. It should be called after
 *              the user has processed after the Rx buffer full is reported.
 *              NOTE: this assumes a ring buffer is used.
 *
 * input parameters
 *
 * @param       dataEntryQueue_t - Pointer to data entry queue.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void ull_flushAllDataEntry( dataEntryQ_t *pDataEntryQ )
{
  dataQ_t         *pDataQueue;
  volatile uint32 keyHwi;

  keyHwi = Hwi_disable();

  /* point to data queue */
  pDataQueue = (dataQ_t *)pDataEntryQ;

  while ( pDataQueue->pNextDataEntry != NULL &&
          pDataQueue->pNextDataEntry->status != DATA_ENTRY_PENDING)
  {
    /* mark the next System data entry as Pending */
    pDataQueue->pNextDataEntry->status = DATA_ENTRY_PENDING;

    /* advance to the next data entry in the data entry queue */
    pDataQueue->pNextDataEntry = pDataQueue->pNextDataEntry->pNextEntry;
  }

  Hwi_restore(keyHwi);
}

#endif /* FEATURE_SCANNER || FEATUR_MONITOR */

/*********************************************************************
 * CALLBACKS
 */

/*********************************************************************
 * @fn      ull_clockHandler
 *
 * @brief   Clock handler function
 *
 * @param   a0 - event
 *
 * @return  none
 */
void ull_clockHandler(UArg a0)
{
#if defined(FEATURE_ADVERTISER)
  /* Process AboutToAdv */
  if (a0 == UL_CLKEVT_ADV_ABOUT_TO)
  {
    ull_notifyAdvAboutTo();
  }
#ifdef RF_MULTI_MODE
  /* Process advertise interval expiry */
  if (a0 == UL_CLKEVT_ADV_INT_EXPIRED)
  {
    volatile uint32 keySwi;

    keySwi = Swi_disable();
    uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_ADV_TX_TIMER_EXPIRED, NULL, 0);
    Swi_restore(keySwi);
  }
#endif /* RF_MULTI_MODE */
#endif  /* FEATURE_ADVERTISER */
}

/*********************************************************************
 * @fn      ull_advDoneCb
 *
 * @brief   Callback function to be invoked by RF driver
 *
 * @param   rfHandle - RF client handle
 *
 * @param   cmdHandle - RF command handle
 *
 * @param   events - RF events
 *
 * @return  none
 */
#if defined(RF_MULTI_MODE)
/*
#if defined __TI_COMPILER_VERSION || defined __TI_COMPILER_VERSION__
#pragma FUNCTION_OPTIONS(ull_advDoneCb, "--opt_level=0")
#endif*/
void ull_advDoneCb(RF_Handle rfHandle, RF_CmdHandle cmdHandle,
                   RF_EventMask events)
{
  volatile uint32 keySwi;
  uint16_t status;

  keySwi = Swi_disable();

  status = urAdvCmd[0].status | urAdvCmd[1].status | urAdvCmd[2].status;
  if ((events & RF_EventLastCmdDone) && (status == BLE_DONE_OK))
  {
    uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_ADV_TX_SUCCESS, NULL, 0);
  }
  else if (events &
           (RF_EventCmdAborted | RF_EventCmdStopped |
            RF_EventCmdPreempted | RF_EventCmdCancelled))
  {
    if (advTxStatus == UL_ADV_TX_SCHEDULED)
    {
      uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_ADV_TX_FAILED, NULL, 0);
    }
  }

  Swi_restore(keySwi);
}
#else
void ull_advDoneCb(RF_Handle rfHandle, RF_CmdHandle cmdHandle,
                   RF_EventMask events)
{
  volatile uint32 keySwi;

  keySwi = Swi_disable();

  if (events & RF_EventLastCmdDone)
  {
    /* Notify uGAP that the last ADV was done successfully. */
    ull_notifyAdvDone(SUCCESS);

    if (ulState == UL_STATE_ADVERTISING)
    {
      if (SUCCESS != ull_advSchedule(UL_ADV_MODE_PERIODIC))
      {
        /* Switch to StadnBy state */
        ulState = UL_STATE_STANDBY;

        Swi_restore(keySwi);

        /* No more scheduling is available */
        ull_notifyAdvDone(bleNoResources);
      }
      else
      {
        Swi_restore(keySwi);
      }
    }
    else
    {
      Swi_restore(keySwi);
    }
  }
  else if (events &
           (RF_EventCmdAborted | RF_EventCmdStopped | RF_EventCmdCancelled))
  {
    /* Switch to StadnBy state */
    ulState = UL_STATE_STANDBY;

    Swi_restore(keySwi);
  }
  else
  {
    Swi_restore(keySwi);
  }
}
#endif /* RF_MULTI_MODE */

#if defined(FEATURE_SCANNER)
/*********************************************************************
 * @fn      ull_scanDoneCb
 *
 * @brief   Callback function to be invoked by RF driver
 *
 * @param   rfHandle - RF client handle
 *
 * @param   cmdHandle - RF command handle
 *
 * @param   events - RF events
 *
 * @return  none
 */
#if defined __TI_COMPILER_VERSION || defined __TI_COMPILER_VERSION__
#pragma FUNCTION_OPTIONS(ull_scanDoneCb, "--opt_level=0")
#endif
void ull_scanDoneCb(RF_Handle rfHandle, RF_CmdHandle cmdHandle,
                    RF_EventMask events)
{
  volatile uint32 keySwi;

  keySwi = Swi_disable();

  if (events & RF_EventRxEntryDone)
  {
    if (scanRxStatus == UL_SCAN_RX_SCHEDULED)
    {
      /* Radio has received a packet */
      uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_SCAN_RX_SUCCESS, NULL, 0);
    }
  }

  /* RF_EventRxEntryDone and RF_EventLastCmdDone can be set at the same time */
  if (events & RF_EventLastCmdDone)
  {
    if (scanRxStatus == UL_SCAN_RX_SCHEDULED)
    {
      switch(urScanCmd.status)
      {
        case BLE_DONE_OK:
        case BLE_DONE_RXTIMEOUT:
        case BLE_DONE_NOSYNC:
        case BLE_DONE_RXERR:
        case BLE_DONE_ENDED:
        case BLE_DONE_ABORT:
        case BLE_DONE_STOPPED:
          /* Operation finished normally */
          break;

        case BLE_ERROR_RXBUF:
          /* No available Rx buffer or received overflowed */
          uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_SCAN_RX_BUF_FULL, NULL, 0);
          break;

        default:
          /* Do not allow further scans */
          ulState = UL_STATE_STANDBY;

          /* Synth error or other fatal errors */
          uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_SCAN_RX_FAILED, NULL, 0);
          break;
      }
    }
  }
  else if (events & RF_EventInternalError)
  {
    /* Do not allow further scans */
    ulState = UL_STATE_STANDBY;

    /* Internal fatal errors */
    uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_SCAN_RX_FAILED, NULL, 0);
  }

  Swi_restore(keySwi);
}
#endif /* FEATURE_SCANNER */

#if defined(FEATURE_MONITOR)
/*********************************************************************
 * @fn      ull_monitorDoneCb
 *
 * @brief   Callback function to be invoked by RF driver
 *
 * @param   rfHandle - RF client handle
 *
 * @param   cmdHandle - RF command handle
 *
 * @param   events - RF events
 *
 * @return  none
 */
#if defined __TI_COMPILER_VERSION || defined __TI_COMPILER_VERSION__
#pragma FUNCTION_OPTIONS(ull_monitorDoneCb, "--opt_level=0")
#endif
void ull_monitorDoneCb(RF_Handle rfHandle, RF_CmdHandle cmdHandle,
                       RF_EventMask events)
{
  volatile uint32 keySwi;

  keySwi = Swi_disable();

  if (events & RF_EventRxEntryDone)
  {
    if (monitorRxStatus == UL_MONITOR_RX_SCHEDULED)
    {
      /* Radio has received a packet */
      uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_MONITOR_RX_SUCCESS, NULL, 0);
    }
  }

  /* RF_EventRxEntryDone and RF_EventLastCmdDone can be set at the same time */
  if (events & RF_EventLastCmdDone)
  {
    if (monitorRxStatus == UL_MONITOR_RX_SCHEDULED)
    {
      switch(urGenericRxCmd.status)
      {
        case BLE_DONE_OK:
        case BLE_DONE_RXTIMEOUT:
        case BLE_DONE_NOSYNC:
        case BLE_DONE_RXERR:
        case BLE_DONE_ENDED:
        case BLE_DONE_ABORT:
        case BLE_DONE_STOPPED:
        case ERROR_PAST_START:
          /* Operation finished normally or past start trigger */
          uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_MONITOR_RX_WINDOW_COMPLETE, NULL, 0);
          break;

        case BLE_ERROR_RXBUF:
          /* No available Rx buffer */
          uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_MONITOR_RX_BUF_FULL, NULL, 0);
          break;

        default:
          /* Do not allow further monitoring scans */
          ulState = UL_STATE_STANDBY;

          /* Synth error or other fatal errors */
          uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_MONITOR_RX_FAILED, NULL, 0);
          break;
      }
    }
  }
  else if (events & RF_EventInternalError)
  {
    /* Do not allow further monitoring scans */
    ulState = UL_STATE_STANDBY;

    /* Internal fatal errors */
    uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_MONITOR_RX_FAILED, NULL, 0);
  }

  Swi_restore(keySwi);
}
#endif /* FEATURE_MONITOR */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn     ull_init
 *
 * @brief  Initialization function for the Micro Link Layer.
 *
 * @param  none
 *
 * @return SUCCESS or FAILURE
 */
bStatus_t ull_init(void)
{
  if (ulState != UL_STATE_STANDBY)
  {
    /* Cannot re-initialize if running */
    return FAILURE;
  }

  /* Initialize RF Interface */
  if (urfi_init() != SUCCESS)
  {
    return FAILURE;
  }

#if defined(FEATURE_ADVERTISER)
  if (hcAdvAboutTo == NULL)
  {
    Clock_Params clockParams;

    /* Setup AdvAboutTo timer as a one-shot timer */
    Clock_Params_init(&clockParams);
    clockParams.arg = UL_CLKEVT_ADV_ABOUT_TO;
    clockParams.period = 0;
    clockParams.startFlag = false;
    Clock_construct(&cAdvAboutTo, ull_clockHandler, 0, &clockParams);

    hcAdvAboutTo = Clock_handle(&cAdvAboutTo);
  }

#if defined(RF_MULTI_MODE)
  if (hcAdvInt == NULL)
  {
    Clock_Params clockParams;

    /* Setup hcAdvInt timer as a one-shot timer */
    Clock_Params_init(&clockParams);
    clockParams.arg = UL_CLKEVT_ADV_INT_EXPIRED;
    clockParams.period = 0;
    clockParams.startFlag = false;
    Clock_construct(&cAdvInt, ull_clockHandler, 0, &clockParams);

    hcAdvInt = Clock_handle(&cAdvInt);
  }
#endif /* RF_MULTI_MODE */
#endif /* FEATURE_ADVERTISER */

  return SUCCESS;
}

#if defined(FEATURE_ADVERTISER)
/*********************************************************************
 * @fn     ull_advRegisterCB
 *
 * @brief  Register callbacks supposed to be called by Advertiser.
 *
 * @param  pfnAdvAboutToCB - callback to nofity the application of time to
 *                           update the advertising packet payload.
 * @param  pfnAdvDoneCB    - callback to post-process Advertising Event
 *
 * @return  none
 */
void ull_advRegisterCB(pfnAdvAboutToCB_t pfnAdvAboutToCB,
                       pfnAdvDoneCB_t pfnAdvDoneCB)
{
  ull_notifyAdvAboutTo = pfnAdvAboutToCB;
  ull_notifyAdvDone = pfnAdvDoneCB;
}
#endif /* FEATURE_ADVERTISER */

#if defined(FEATURE_SCANNER)
/*********************************************************************
 * @fn     ull_scanRegisterCB
 *
 * @brief  Register callbacks supposed to be called by Observer.
 *
 * @param  pfnScanIndicationCB - callback to nofity the application that a
 *                               packet has been received.
 * @param  pfnScanWindowCompleteCB - callback to post-process scan interval complete
 *
 * @return  none
 */
void ull_scanRegisterCB(pfnScanIndCB_t pfnScanIndicationCB,
                        pfnScanWindowCompCB_t pfnScanWindowCompleteCB)
{
  ull_notifyScanIndication = pfnScanIndicationCB;
  ull_notifyScanWindowComplete = pfnScanWindowCompleteCB;
}
#endif /* FEATURE_SCANNER */

#if defined(FEATURE_MONITOR)
/*********************************************************************
 * @fn     ull_monitorRegisterCB
 *
 * @brief  Register callbacks supposed to be called by Monitor.
 *
 * @param  pfnMonitorIndicationCB - callback to nofity the application that a
 *                                  packet has been received.
 * @param  pfnMonitorCompleteCB - callback to post-process monitor duration complete
 *
 * @return  none
 */
void ull_monitorRegisterCB(pfnMonitorIndCB_t pfnMonitorIndicationCB,
                           pfnMonitorCompCB_t pfnMonitorCompleteCB)
{
  ull_notifyMonitorIndication = pfnMonitorIndicationCB;
  ull_notifyMonitorComplete = pfnMonitorCompleteCB;
}
#endif /* FEATURE_MONITOR */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

#if defined(FEATURE_ADVERTISER)
/*********************************************************************
 * @fn      ull_advSchedule
 *
 * @brief   Update ADV command with latest parameters and schedule
 *          the radio operation
 *
 * @param   mode   -  if UL_ADV_MODE_START, start the advertisement and schedule
 *                    the operation for 1 ms later. If set to
 *                    UL_ADV_MODE_IMMEDIATE schedules the operation 1ms later.
 *                    UL_ADV_MODE_PERIODIC and UL_ADV_MODE_PERIODIC_RELAXED,
 *                    sechedule the operation, according to the advInterval and
 *                    the random delay for time critical and time relaxed
 *                    operation respectively. UL_ADV_MODE_RESCHEDULE,
 *                    reschedules the operation at the same previous schedule.
 *
 * @return  SUCCESS - the radio operation has been successfully scheduled
 *          FAILURE - failed to schedule the radio operation due to RF driver
 *                    error
 */
bStatus_t ull_advSchedule(uint8 mode)
{
  uint16 commandNo;
  uint8 i;
#if defined(RF_MULTI_MODE)
  RF_ScheduleCmdParams cmdParams;
  uint8  numChan; /* # of adv channels */
  volatile uint32 keyHwi;
#endif /* RF_MULTI_MODE */

#if defined(FEATURE_SCAN_RESPONSE)
  if (ubParams.advType == UB_ADVTYPE_ADV_SCAN)
  {
    commandNo = CMD_BLE_ADV_SCAN;
  }
  else
#endif /* FEATURE_SCAN_RESPONSE */
  /* UB_ADVTYPE_ADV_NC */
  {
    commandNo = CMD_BLE_ADV_NC;
  }

  for (i = 0; i < 3; i++)
  {
    urAdvCmd[i].commandNo = (ubParams.advChanMap & (1 << i)) ?
                            commandNo : CMD_NOP;
  }

  if ((UL_ADV_MODE_START == mode) || (UL_ADV_MODE_IMMEDIATE == mode))
  {
    /* Schedule the first adv event for 1 ms from now */
    urAdvCmd[0].startTime = RF_getCurrentTime() + 1 * MS_TO_RAT;
  }
  else if (UL_ADV_MODE_PERIODIC == mode)
  {
    /* Add a random delay up to 10ms */
    urAdvCmd[0].startTime += ubParams.advInterval * BLE_TO_RAT +
                             (uint32) rand() % (MS_TO_RAT * 10);
  }
  else if (UL_ADV_MODE_PERIODIC_RELAXED == mode)
  {
    /* Anchor to the current time*/
    urAdvCmd[0].startTime = RF_getCurrentTime();
    /* Add a random delay up to 10ms to the interval */
    urAdvCmd[0].startTime += ubParams.advInterval * BLE_TO_RAT +
                             (uint32) rand() % (MS_TO_RAT * 10);

  }
  // clear command status values
  urAdvCmd[0].status = IDLE;
  urAdvCmd[1].status = IDLE;
  urAdvCmd[2].status = IDLE;

#if defined(RF_MULTI_MODE)
    /* Calculate the end time */
    numChan = ubParams.advChanMap & 0x01 + ((ubParams.advChanMap & 0x02) >> 1) +
              ((ubParams.advChanMap & 0x04) >> 2);
    cmdParams.endTime =
      urAdvCmd[0].startTime
      + numChan * (((urAdvParams.advLen + 16) * BYTE_TO_RAT) + UL_ADV_GAP_IN_CHAIN)
      - UL_ADV_GAP_IN_CHAIN;
    cmdParams.priority = (RF_Priority) ubParams.rfPriority;

  if ((mode != UL_ADV_MODE_RESCHEDULE) && (mode != UL_ADV_MODE_IMMEDIATE))
  {
    uint32 timerTicks;

    if (RF_TIME_CRITICAL == rfTimeCrit)
    {
      if (UL_ADV_MODE_START == mode)
      {
        timerTicks = ubParams.advInterval * BLE_TO_RAT;
      }
      else
      {
        timerTicks = cmdParams.endTime - RF_getCurrentTime();
      }
      /* Set timeout equal to advertisement interval plus 1 ms */
      timerTicks = (timerTicks)/SYSTICK_TO_RAT;
      timerTicks += MS_TO_SYSTICK;

    }
    else //relaxed operation.
    {
      if (UL_ADV_MODE_START == mode)
      {
        timerTicks = ubParams.advInterval * BLE_TO_RAT;
      }
      else
      {
        timerTicks = cmdParams.endTime - RF_getCurrentTime();
      }

      /* Set timeout equal to two times the advertisement interval minus 1 ms */
      timerTicks = (timerTicks * 2)/SYSTICK_TO_RAT;
      timerTicks -= MS_TO_SYSTICK;
    }

    if (Clock_isActive(hcAdvInt))
    {
      /* stop the clock first */
      Clock_stop(hcAdvInt);
    }

    Clock_setTimeout(hcAdvInt, timerTicks);
    Clock_start(hcAdvInt);
  }

  keyHwi = Hwi_disable();
  urAdvHandle = RF_scheduleCmd(urHandle, (RF_Op*) &urAdvCmd[0],
                               &cmdParams, ull_advDoneCb,
                               RF_EventInternalError);
  Hwi_restore(keyHwi);

  if (urAdvHandle >= 0)
  {
    advTxStatus = UL_ADV_TX_SCHEDULED;
  }
  else
  {
    //ToDo: Once RF driver add the error status, use that instead
    //and one for failure case.
    advTxStatus = UL_ADV_TX_NO_RF_RESOURCE;
  }


#else /* RF Single Mode */
  urAdvHandle = RF_postCmd(urHandle, (RF_Op*) &urAdvCmd[0],
                           (RF_Priority) ubParams.rfPriority, ull_advDoneCb,
                           RF_EventInternalError);
#endif

  if (urAdvHandle >= 0)
  {
    if ((mode != UL_ADV_MODE_START) && ubParams.timeToAdv)
    {
      /* # of RAT ticks to the next Adv */
      uint32 ticksToAdv = urAdvCmd[0].startTime - RF_getCurrentTime();

      /* Is there enough time to issue "about to advertise" notification
       * before the next ADV happens?
       * Count 1 ms additionally to the required time for safety.
       */
      if (ticksToAdv >= ((uint32) ubParams.timeToAdv + 1) * MS_TO_RAT)
      {
        if (Clock_isActive(hcAdvAboutTo))
        {
          /* Stop clock first */
          Clock_stop(hcAdvAboutTo);
        }

        /* Convert RAT ticks to system ticks. */
        ticksToAdv /= SYSTICK_TO_RAT;

        /* Set the timeout */
        Clock_setTimeout(hcAdvAboutTo,
                         ticksToAdv - ubParams.timeToAdv * MS_TO_SYSTICK);

        /* Start clock instance */
        Clock_start(hcAdvAboutTo);
      }
    }

    return SUCCESS;
  }

#if defined(RF_MULTI_MODE)
  return SUCCESS;
#else
  return FAILURE;
#endif
}

/*********************************************************************
 * @fn      ull_advStart
 *
 * @brief   Enter UL_STATE_ADVERTISING
 *
 * @param   none
 *
 * @return  SUCCESS - Successfully entered UL_STATE_ADVERTISING
 *          FAILURE - Failed to enter UL_STATE_ADVERSISING
 */
bStatus_t ull_advStart(void)
{
  volatile uint32 keySwi;
  bStatus_t status = FAILURE;

  keySwi = Swi_disable();
  /* Possible to enter UL_STATE_ADVERTISING from only UL_STATE_STANDBY */
  if (ulState != UL_STATE_STANDBY)
  {
    Swi_restore(keySwi);
    return status;
  }
  status = uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_ADV_TX_STARTED, NULL, 0);

  if (status == SUCCESS)
  {
    ulState = UL_STATE_ADVERTISING;
  }
  Swi_restore(keySwi);
  return status;
}

/*********************************************************************
 * @fn      ull_advStop
 *
 * @brief   Exit UL_STATE_ADVERTISING
 *
 * @param   none
 *
 * @return  none
 */
void ull_advStop(void)
{
  volatile uint32 keySwi;

  keySwi = Swi_disable();

  if (ulState == UL_STATE_ADVERTISING)
  {
    /* Cancel or stop ADV command */
    if (urAdvHandle > 0)
    {
      RF_cancelCmd(urHandle, urAdvHandle, 0);
      urAdvHandle = UR_CMD_HANDLE_INVALID;
    }

    /* Cancel AdvAboutTo timer */
    if (Clock_isActive(hcAdvAboutTo))
    {
      /* Stop clock first */
      Clock_stop(hcAdvAboutTo);
    }

#if defined(RF_MULTI_MODE)
    /* cancel the interval timer */
    if (Clock_isActive(hcAdvInt))
    {
      /* stop the clock first */
      Clock_stop(hcAdvInt);
    }
#endif /* RF_MULTI_MODE */

    ulState = UL_STATE_STANDBY;
  }

  Swi_restore(keySwi);
}
#endif /* FEATURE_ADVERTISER */

#if defined(FEATURE_SCANNER)
/*********************************************************************
 * @fn      ull_scanSchedule
 *
 * @brief   Update SCAN command with latest parameters and schedule
 *          the radio operation
 *
 * @param   mode   -  if UL_SCAN_MODE_START, start the scanning and schedule
 *                    the operation immediately. UL_SCAN_MODE_RESCHEDULE,
 *                    reschedules the operation at the same previous schedule.
 *
 * @return  SUCCESS - the radio operation has been successfully scheduled
 *          FAILURE - failed to schedule the radio operation due to RF driver
 *                    error
 */
bStatus_t ull_scanSchedule(uint8 mode)
{
#if defined(RF_MULTI_MODE)
  RF_ScheduleCmdParams cmdParams;
  RF_EventMask bmEvent;
  volatile uint32 keyHwi;
#endif /* RF_MULTI_MODE */

  /* The UL_SCAN_MODE_START and UL_SCAN_MODE_RESCHEDULE
   * are treated the same.
   */
  (void) mode;

  /* clear command status values */
  urScanCmd.status = IDLE;

#if defined(RF_MULTI_MODE)
  bmEvent = (RF_EventInternalError | RF_EventLastCmdDone | RF_EventRxEntryDone);

  /* uGAP controls the start of scan timing. The end time is unknown.
   * ADV always has equal or higher priority.
   * The end time feature is not required.
   */
  cmdParams.endTime = 0;
  cmdParams.priority =
    (RF_Priority) ((ubParams.rfPriority > RF_PRI_NORMAL) ?
                   (ubParams.rfPriority - 1) : RF_PRI_NORMAL);

  keyHwi = Hwi_disable();
  urScanHandle = RF_scheduleCmd(urHandle, (RF_Op*) &urScanCmd,
                                &cmdParams, ull_scanDoneCb, bmEvent);
  Hwi_restore(keyHwi);

  if (urScanHandle >= 0)
  {
    scanRxStatus = UL_SCAN_RX_SCHEDULED;
  }
  else
  {
    //ToDo: Once RF driver add the error status, use that instead
    //and one for failure case.
    scanRxStatus = UL_SCAN_RX_NO_RF_RESOURCE;
  }

  return SUCCESS;
#else /* RF Single Mode */
  urScanHandle = RF_postCmd(urHandle, (RF_Op*) &urScanCmd,
                            (RF_Priority) ubParams.rfPriority, ull_scanDoneCb,
                            RF_EventInternalError);

  if (urScanHandle >= 0)
  {
    return SUCCESS;
  }

  return FAILURE;
#endif /* RF_MULTI_MODE */
}

/*********************************************************************
 * @fn      ull_scanStart
 *
 * @brief   Enter UL_STATE_SCANNING
 *
 * @param   scanChanIndex - scan channel index
 *
 * @return  SUCCESS - Successfully entered UL_STATE_SCANNING
 *          FAILURE - Failed to enter UL_STATE_SCANNING
 */
bStatus_t ull_scanStart(uint8_t scanChanIndex)
{
  volatile uint32 keySwi;
  bStatus_t status = FAILURE;

  keySwi = Swi_disable();
  /* Possible to enter UL_STATE_SCANNING from only UL_STATE_STANDBY */
  if (ulState != UL_STATE_STANDBY)
  {
    Swi_restore(keySwi);
    return status;
  }

  /* Update scan parameters */
  urScanCmd.channel = scanChanIndex;

  /* TBD: Update other scan parameters */

  /* Post an ul event */
  status = uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_SCAN_RX_STARTED, NULL, 0);

  if (status == SUCCESS)
  {
    ulState = UL_STATE_SCANNING;
  }

  Swi_restore(keySwi);
  return(status);
}

/*********************************************************************
 * @fn      ull_scanStop
 *
 * @brief   Exit UL_STATE_SCANNING
 *
 * @param   none
 *
 * @return  none
 */
void ull_scanStop(void)
{
  volatile uint32 keySwi;

  keySwi = Swi_disable();

  if (ulState == UL_STATE_SCANNING)
  {
    ulState = UL_STATE_STANDBY;

    /* Cancel or stop Scan command */
    if (urScanHandle > 0)
    {
      RF_cancelCmd(urHandle, urScanHandle, 0);
      urScanHandle = UR_CMD_HANDLE_INVALID;
    }

    ull_notifyScanWindowComplete(SUCCESS);
  }

  Swi_restore(keySwi);
}

/*******************************************************************************
 * @fn          ull_getAdvChanPDU
 *
 * @brief       This function is used to retrieve the Advertising Channel PDU,
 *              and parse its data for either a Advertising Packet Report or a
 *              Scan Packet Report.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       len      - Pointer to payload length (sans the Adv's address).
 * @param       payload  - Pointer to payload (sans the Adv's address).
 *
 * @return      None.
 */
void ull_getAdvChanPDU( uint8 *len, uint8 *payload )
{
  uint8       *pPdu;
  dataEntry_t *pDataEntry;
  uint8        i;

  /* get pointer to packet */
  pDataEntry = ull_getNextDataEntry( (dataEntryQ_t *)urScanCmd.pParams->pRxQ );

  if (pDataEntry != NULL)
  {
    /* get pointer to BLE PDU packet */
    pPdu = (uint8 *)(pDataEntry + 1);

    /* read length, add postfix length, exclude CRC length  */
    *len = pPdu[1] + ULL_PKT_HDR_LEN + ULL_SUFFIX_MAX_SIZE - ULL_SUFFIX_CRC_SIZE;

    if (*len < ULL_PKT_HDR_LEN + ULL_MAX_BLE_ADV_PKT_SIZE + ULL_SUFFIX_MAX_SIZE - ULL_SUFFIX_CRC_SIZE)
    {
      /* copy PDU to payload */
      for (i=0; i<*len; i++)
      {
        *payload++ = *pPdu++;
      }
    }
    else
    {
      /* packet length error */
      *len = 0;
    }
  }
  else
  {
    /* data entry error */
    *len = 0;
  }
}

/*********************************************************************
 * @fn      ull_rxEntryDoneCback
 *
 * @brief   Callback to send the received packet up.
 *
 * @param   none
 *
 * @return  none
 */
void ull_rxEntryDoneCback(void)
{
  uint8 dataLen;
  volatile uint32 keyHwi;

  {
    dataEntry_t *pDataEntry = ull_getNextDataEntry( (dataEntryQ_t *)urScanCmd.pParams->pRxQ );

    /* get pointer to packet */
    if ( (pDataEntry == NULL) || (pDataEntry->status != DATA_ENTRY_FINISHED) )
    {
      /* nothing to do here */
      return;
    }
  }

  keyHwi = Hwi_disable();

  /* process RX FIFO data */
  ull_getAdvChanPDU( &dataLen, advPkt );

  /* in all cases, mark the RX queue data entry as free
   * Note: Even if there isn't any heap to copy to, this packet is considered
   *       lost, and the queue entry is marked free for radio use.
   */
  ull_nextDataEntryDone( (dataEntryQ_t *)urScanCmd.pParams->pRxQ );

  Hwi_restore(keyHwi);

  /* TBD: handle filtering and white list */
  /* TBD: advPkt needs to be dynamically allocated and application freed */

  /* We have an advertisment packet:
   *
   * | Preamble  | Access Addr | PDU         | CRC     |
   * | 1-2 bytes | 4 bytes     | 2-257 bytes | 3 bytes |
   *
   * The PDU is expended to:
   * | Header  | Payload     |
   * | 2 bytes | 1-255 bytes |
   *
   * The Header is expended to:
   * | PDU Type...RxAdd | Length |
   * | 1 byte           | 1 byte |
   *
   * The Payload is expended to:
   * | AdvA    | AdvData    |
   * | 6 bytes | 0-31 bytes |
   *
   * The radio stripps the CRC and replaces it with the postfix.
   *
   * The Postfix is expended to:
   * | RSSI   | Status | TimeStamp |
   * | 1 byte | 1 byte | 4 bytes   |
   *
   * The Status is expended to:
   * | bCrcErr | bIgnore | channel  |
   * | bit 7   | bit 6   | bit 5..0 |
   *
   * Note the advPkt is the beginning of PDU; the dataLen includes
   * the postfix length.
   *
   */
  if (dataLen != 0)
  {
    ull_notifyScanIndication( SUCCESS, dataLen, advPkt );
  }
  return;
}
#endif /* FEATURE_SCANNER */

#if defined(FEATURE_MONITOR)
/*********************************************************************
 * @fn      ull_monitorSchedule
 *
 * @brief   Update Generic RX command with latest parameters and schedule
 *          the radio operation
 *
 * @param   mode   -  if UL_MONITOR_MODE_START, start the scanning and schedule
 *                    the operation immediately. UL_MONITOR_MODE_RESCHEDULE,
 *                    reschedules the operation at the same previous schedule.
 *
 * @return  SUCCESS - the radio operation has been successfully scheduled
 *          FAILURE - failed to schedule the radio operation due to RF driver
 *                    error
 */
bStatus_t ull_monitorSchedule(uint8 mode)
{
#if defined(RF_MULTI_MODE)
  RF_ScheduleCmdParams cmdParams;
  RF_EventMask bmEvent;
  volatile uint32 keyHwi;
#endif /* RF_MULTI_MODE */

  /* clear command status values */
  urGenericRxCmd.status = IDLE;

  /* Note: cannot do wildcard access address */
  urGenericRxParams.accessAddress = ubParams.accessAddr;

#if defined(RF_MULTI_MODE)
  bmEvent = (RF_EventInternalError | RF_EventLastCmdDone | RF_EventRxEntryDone);

  if (mode == UL_MONITOR_MODE_START)
  {
    urGenericRxCmd.startTrigger.triggerType = TRIG_ABSTIME;
    urGenericRxCmd.startTime = ubParams.startTime;

    if (ubParams.monitorDuration > 0)
    {
      urGenericRxParams.endTrigger.triggerType = TRIG_REL_START;
      urGenericRxParams.endTime = ubParams.monitorDuration * BLE_TO_RAT;
      cmdParams.endTime = urGenericRxParams.endTime;
    }
    else
    {
      /* Continuous monitoring */
      urGenericRxParams.endTrigger.triggerType = TRIG_NEVER;
      urGenericRxParams.endTime = 0;
      cmdParams.endTime = 0;
    }
  }
  else
  {
    /* UL_MONITOR_MODE_RESCHEDULE */
    urGenericRxCmd.startTrigger.triggerType = TRIG_NOW;
    urGenericRxCmd.startTime = 0;

    if (ubParams.monitorDuration > 0)
    {
      if (urGenericRxParams.endTime > ubParams.monitorDuration * BLE_TO_RAT)
      {
        /* shorten the end time */
        urGenericRxParams.endTime -= ubParams.monitorDuration * BLE_TO_RAT;
      }
      else
      {
        /* past the end time */
        urGenericRxParams.endTrigger.triggerType = TRIG_REL_START;
        urGenericRxParams.endTime = 100; /* End ASAP */
      }
      cmdParams.endTime = urGenericRxParams.endTime;
    }
    else
    {
      /* Continuous monitoring */
      urGenericRxParams.endTrigger.triggerType = TRIG_NEVER;
      urGenericRxParams.endTime = 0;
      cmdParams.endTime = 0;
    }
  }

  cmdParams.priority = (RF_Priority)ubParams.rfPriority;

  keyHwi = Hwi_disable();

  /* Save local session Id */
  ull_sessionId = ubParams.monitorHandle;
  urGenericRxHandle = RF_scheduleCmd(urHandle, (RF_Op*) &urGenericRxCmd,
                                     &cmdParams, ull_monitorDoneCb, bmEvent);
  Hwi_restore(keyHwi);

  if (urGenericRxHandle >= 0)
  {
    monitorRxStatus = UL_MONITOR_RX_SCHEDULED;
  }
  else
  {
    //ToDo: Once RF driver add the error status, use that instead
    //and one for failure case.
    monitorRxStatus = UL_MONITOR_RX_NO_RF_RESOURCE;
  }

  return SUCCESS;
#else /* RF Single Mode */
  urGenericRxHandle = RF_postCmd(urHandle, (RF_Op*) &urGenericRxParams,
                                 (RF_Priority) ubParams.rfPriority, ull_monitorDoneCb,
                                 RF_EventInternalError);

  if (urGenericRxHandle >= 0)
  {
    return SUCCESS;
  }

  return FAILURE;
#endif /* RF_MULTI_MODE */
}

/*********************************************************************
 * @fn      ull_monitorStart
 *
 * @brief   Enter UL_STATE_MONITORING
 *
 * @param   channel - monitoring scan channel
 *
 * @return  SUCCESS - Successfully entered UL_STATE_MONITORING
 *          FAILURE - Failed to enter UL_STATE_MONITORING
 */
bStatus_t ull_monitorStart(uint8_t channel)
{
  volatile uint32 keySwi;
  bStatus_t status = FAILURE;

  keySwi = Swi_disable();
  /* Possible to enter UL_STATE_MONITORING from only UL_STATE_STANDBY */
  if (ulState != UL_STATE_STANDBY)
  {
    Swi_restore(keySwi);
    return status;
  }

  /* Update scan parameters */
  urGenericRxCmd.channel = ubParams.monitorChan = channel;

  /* TBD: Update other generic Rx parameters */

  /* Post an ul event */
  status = uble_buildAndPostEvt(UB_EVTDST_LL, ULL_EVT_MONITOR_RX_STARTED, NULL, 0);

  if (status == SUCCESS)
  {
    ulState = UL_STATE_MONITORING;
  }

  Swi_restore(keySwi);
  return (status);
}

/*********************************************************************
 * @fn      ull_monitorStop
 *
 * @brief   Exit UL_STATE_MONITORING
 *
 * @param   none
 *
 * @return  none
 */
void ull_monitorStop(void)
{
  volatile uint32 keySwi;

  keySwi = Swi_disable();

  if (ulState == UL_STATE_MONITORING)
  {
    ulState = UL_STATE_STANDBY;

    /* Cancel or stop generic Rx command */
    if (urGenericRxHandle > 0)
    {
      RF_cancelCmd(urHandle, urGenericRxHandle, 0);
      urGenericRxHandle = UR_CMD_HANDLE_INVALID;
    }
  }

  Swi_restore(keySwi);
}

/*******************************************************************************
 * @fn          ull_getPDU
 *
 * @brief       This function is used to retrieve the Rx PDU.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       len      - Pointer to payload length.
 * @param       payload  - Pointer to payload.
 *
 * @return      None.
 */
void ull_getPDU( uint8 *len, uint8 *payload )
{
  uint8       *pPdu;
  dataEntry_t *pDataEntry;

  /* get pointer to packet */
  pDataEntry = ull_getNextDataEntry( (dataEntryQ_t *)urGenericRxCmd.pParams->pRxQ );

  /* get pointer to BLE PDU packet */
  pPdu = (uint8 *)(pDataEntry + 1);

  /* read length, add postfix length, exclude CRC length  */
  *len = pPdu[1] + ULL_PKT_HDR_LEN + ULL_SUFFIX_MAX_SIZE - ULL_SUFFIX_CRC_SIZE;

  /* The maximum length is ULL_PKT_HDR_LEN + ULL_MAX_BLE_PKT_SIZE + 
   * ULL_SUFFIX_MAX_SIZE - ULL_SUFFIX_CRC_SIZE. This is greater than 255.
   * There is no need to do a length check here since it will always pass.
   */

  /* copy PDU to payload */
  memcpy(payload, pPdu, *len);
}

/*********************************************************************
 * @fn      ull_rxEntryDoneCback
 *
 * @brief   Callback to send the received packet up.
 *
 * @param   none
 *
 * @return  none
 */
void ull_rxEntryDoneCback(void)
{
  uint8 dataLen;
  volatile uint32 keyHwi;

  {
    dataEntry_t *pDataEntry = ull_getNextDataEntry( (dataEntryQ_t *)urGenericRxCmd.pParams->pRxQ );

    /* get pointer to packet */
    if ( (pDataEntry == NULL) || (pDataEntry->status != DATA_ENTRY_FINISHED) )
    {
      /* nothing to do here */
      return;
    }
  }

  keyHwi = Hwi_disable();

  /* process RX FIFO data */
  ull_getPDU( &dataLen, monitorPkt );

  /* in all cases, mark the RX queue data entry as free
   * Note: Even if there isn't any heap to copy to, this packet is considered
   *       lost, and the queue entry is marked free for radio use.
   */
  ull_nextDataEntryDone( (dataEntryQ_t *)urGenericRxCmd.pParams->pRxQ );

  Hwi_restore(keyHwi);

  /* TBD: monitorPkt can be dynamically allocated and application freed */

  /* We have a packet:
   *
   * | Preamble  | Access Addr | PDU         | CRC     |
   * | 1-2 bytes | 4 bytes     | 2-257 bytes | 3 bytes |
   *
   * The PDU is expended to:
   * | Header  | Payload     |
   * | 2 bytes | 1-255 bytes |
   *
   * The Header is expended to:
   * | PDU Type...RxAdd | Length |
   * | 1 byte           | 1 byte |
   *
   * The radio stripps the CRC and replaces it with the postfix.
   *
   * The Postfix is expended to:
   * | RSSI   | Status | TimeStamp |
   * | 1 byte | 1 byte | 4 bytes   |
   *
   * The Status is expended to:
   * | bCrcErr | bIgnore | channel  |
   * | bit 7   | bit 6   | bit 5..0 |
   *
   * Note the monitorPkt is the beginning of PDU; the dataLen includes
   * the postfix length.
   *
   */
  if (dataLen != 0)
  {
    ull_notifyMonitorIndication( SUCCESS, ull_sessionId, dataLen, monitorPkt );
  }
  return;
}
#endif /* FEATURE_MONITOR */

/*********************************************************************
 * @fn      uble_processLLMsg
 *
 * @brief   Process event messages sent from Micro BLE Stack to uLL through
 *          the application's Stack Event Proxy and uble_processMsg().
 *
 * @param   pEvtMsg - Pointer to the Micro BLE Stack event message destined to
 *                    uLL.
 *
 * @return  none
 */
void uble_processLLMsg(ubEvtMsg_t *pEvtMsg)
{
  switch (pEvtMsg->hdr.evt)
  {
#if defined(FEATURE_ADVERTISER)
  case ULL_EVT_ADV_TX_SUCCESS:
    /* Adv tx was successful */
    advTxStatus = UL_ADV_TX_DONE;

#if defined(RF_MULTI_MODE)
    if (Clock_isActive(hcAdvInt))
    {
      /* stop the clock first */
      Clock_stop(hcAdvInt);
    }
#endif /* RF_MULTI_MODE */

    ull_notifyAdvDone(SUCCESS);
    if (ulState == UL_STATE_ADVERTISING)
    {
      /* set the next advertisement */
      if (RF_TIME_CRITICAL == rfTimeCrit)
      {
        ull_advSchedule(UL_ADV_MODE_PERIODIC);
      }
      else
      {
        ull_advSchedule(UL_ADV_MODE_PERIODIC_RELAXED);
      }
    }
    break;

  case ULL_EVT_ADV_TX_FAILED:
    /* adv rf tx failure */
    advTxStatus = UL_ADV_TX_NO_RF_RESOURCE;
    ull_notifyAdvDone(bleNoResources);
    break;

  case ULL_EVT_ADV_TX_TIMER_EXPIRED:
    /* adv interval timer expired */
    advTxStatus = UL_ADV_TX_FAILED;
    ull_notifyAdvDone(FAILURE);
    if (ulState == UL_STATE_ADVERTISING)
    {
      if (RF_TIME_CRITICAL == rfTimeCrit)
      {
        ull_advSchedule(UL_ADV_MODE_PERIODIC);
      }
      else
      {
        /* Cancel or stop ADV command */
        if (urAdvHandle > 0)
        {
          RF_cancelCmd(urHandle, urAdvHandle, 0);
          urAdvHandle = UR_CMD_HANDLE_INVALID;
        }

        ull_advSchedule(UL_ADV_MODE_PERIODIC_RELAXED);
      }
    }
    break;

  case ULL_EVT_ADV_TX_RADIO_AVAILABLE:
    /* rf radio resource available */
    if (UL_ADV_TX_NO_RF_RESOURCE == advTxStatus)
    {
      /* check if there is time to reschedule the advertisement. */
      uint32_t currentTime = RF_getCurrentTime();
      if ((currentTime + 1 * MS_TO_RAT) < urAdvCmd[0].startTime)
      {
        if (ulState == UL_STATE_ADVERTISING)
        {
          /* reschedule the advertisement.*/
          ull_advSchedule(UL_ADV_MODE_RESCHEDULE);
        }
      }
      else
      {
        if (RF_TIME_CRITICAL == rfTimeCrit)
        {
          advTxStatus = UL_ADV_TX_FAILED;
          ull_notifyAdvDone(FAILURE);
#if defined(RF_MULTI_MODE)
          if (Clock_isActive(hcAdvInt))
          {
            /* stop the clock first */
            Clock_stop(hcAdvInt);
          }
#endif /* RF_MULTI_MODE */
          if (ulState == UL_STATE_ADVERTISING)
          {
            ull_advSchedule(UL_ADV_MODE_PERIODIC);
          }
        }
        else /* relaxed operation */
        {
          /* past due, send it immediate */
          if (ulState == UL_STATE_ADVERTISING)
          {
            ull_advSchedule(UL_ADV_MODE_IMMEDIATE);
          }
        }
      }
    }
    break;

  case ULL_EVT_ADV_TX_STARTED:
    if (ulState == UL_STATE_ADVERTISING)
    {
      ull_advSchedule(UL_ADV_MODE_START);
    }
    break;
#endif /* FEATURE_ADVERTISER */

#if defined(FEATURE_SCANNER)
  case ULL_EVT_SCAN_RX_SUCCESS:
    /* Scan Rx has received a packet */
    ull_rxEntryDoneCback();
    break;

  case ULL_EVT_SCAN_RX_FAILED:
    /* Scan rx failure. This is caused by radio failures,
     * not preemption eventhough the scanRxStatus is the same.
     */
    scanRxStatus = UL_SCAN_RX_NO_RF_RESOURCE;
    ull_notifyScanWindowComplete(bleNoResources);
    break;

  case ULL_EVT_SCAN_RX_BUF_FULL:
    /* Scan Rx buffer full */
    scanRxStatus = UL_SCAN_RX_NO_RF_RESOURCE;
    ull_notifyScanIndication( MSG_BUFFER_NOT_AVAIL, 0, NULL );

    /* Flush RX queue data entries */
    ull_flushAllDataEntry( (dataEntryQ_t *)urScanCmd.pParams->pRxQ );

    if (ulState == UL_STATE_SCANNING)
    {
      /* reschedule the scanning */
      ull_scanSchedule(UL_SCAN_MODE_RESCHEDULE);
    }
    break;

  case ULL_EVT_SCAN_RX_RADIO_AVAILABLE:
    /* Rf radio resource available. This is caused by PHY preemption.
     */
    if (scanRxStatus == UL_SCAN_RX_NO_RF_RESOURCE && ulState == UL_STATE_SCANNING)
    {
      /* reschedule the scanning */
      ull_scanSchedule(UL_SCAN_MODE_RESCHEDULE);
    }
    break;

  case ULL_EVT_SCAN_RX_STARTED:
    if (ulState == UL_STATE_SCANNING)
    {
      ull_scanSchedule(UL_SCAN_MODE_START);
    }
    break;
#endif /* FEATURE_SCANNER */

#if defined(FEATURE_MONITOR)
  case ULL_EVT_MONITOR_RX_SUCCESS:
    /* Monitoring scan Rx has received a packet */
    ull_rxEntryDoneCback();
    break;

  case ULL_EVT_MONITOR_RX_FAILED:
    /* Monitoring scan rx failure. This is caused by radio failures,
     * not preemption eventhough the monitorRxStatus is the same.
     */
    monitorRxStatus = UL_MONITOR_RX_NO_RF_RESOURCE;
    ull_notifyMonitorComplete(bleNoResources, ull_sessionId);
    break;

  case ULL_EVT_MONITOR_RX_BUF_FULL:
    /* Monitoring scan Rx buffer full */
    monitorRxStatus = UL_MONITOR_RX_NO_RF_RESOURCE;
    ull_notifyMonitorIndication( MSG_BUFFER_NOT_AVAIL, ull_sessionId, 0, NULL );

    /* flush RX queue data entries */
    ull_flushAllDataEntry( (dataEntryQ_t *)urGenericRxCmd.pParams->pRxQ );

    if (ulState == UL_STATE_MONITORING)
    {
      /* reschedule the monitoring scan */
      ull_monitorSchedule(UL_MONITOR_MODE_RESCHEDULE);
    }
    break;

  case ULL_EVT_MONITOR_RX_WINDOW_COMPLETE:
    if (ulState == UL_STATE_MONITORING)
    {
      /* Monitoring scan rx window complete. */
      ull_notifyMonitorComplete(SUCCESS, ull_sessionId);
    }
    break;

  case ULL_EVT_MONITOR_RX_RADIO_AVAILABLE:
    /* Rf radio resource available. This is caused by PHY preemption.
     */
    if (monitorRxStatus == UL_MONITOR_RX_NO_RF_RESOURCE && ulState == UL_STATE_MONITORING)
    {
      /* reschedule the monitoring scan */
      ull_monitorSchedule(UL_MONITOR_MODE_RESCHEDULE);
    }
    break;

  case ULL_EVT_MONITOR_RX_STARTED:
    if (ulState == UL_STATE_MONITORING)
    {
      ull_monitorSchedule(UL_MONITOR_MODE_START);
    }
    break;
#endif /* FEATURE_MONITOR */

  default:
    break;
  }
}


/*********************************************************************
*********************************************************************/
