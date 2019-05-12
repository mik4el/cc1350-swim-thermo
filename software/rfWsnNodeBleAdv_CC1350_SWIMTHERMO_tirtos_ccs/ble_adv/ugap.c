/******************************************************************************

 @file  ugap.c

 @brief This file contains the Micro GAP Initialization and Configuration APIs.

 Group: WCS, BTS
 Target Device: CC13xx

 ******************************************************************************
 
 Copyright (c) 2011-2018, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 Release Name: simplelink_cc13x0_sdk_2_10_00_
 Release Date: 2018-04-09 00:04:23
 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Swi.h>

#ifdef USE_ICALL
#include <icall.h>
#endif

#include "bcomdef.h"

#include "uble.h"
#include "ull.h"
#include "ugap.h"

#if defined(FEATURE_BROADCASTER) && !defined(FEATURE_ADVERTISER)
  #error "FEATURE_ADVERTISER should also be defined if FEATURE_BROADCASTER \
is defined."
#endif /* FEATURE_BROADCASTER && !FEATURE_ADVERTISER */
#if defined(FEATURE_OBSERVER) && !defined(FEATURE_SCANNER)
  #error "FEATURE_SCANNER should also be defined if FEATURE_OBSERVER \
is defined."
#endif /* FEATURE_OBSERVER && !FEATURE_SCANNER */
#if !defined(FEATURE_BROADCASTER) && !defined(FEATURE_OBSERVER) && !defined(FEATURE_CM)
  #error "At least one GAP role feature should be defined. Currently only \
FEATURE_BROADCASTER is available."
#endif /* !FEATURE_BROADCASTER && !FEATURE_OBSERVER */

#if defined(FEATURE_MONITOR)
#if defined(FEATURE_OBSERVER) || defined(FEATURE_SCANNER)
  #error "FEATURE_MONITOR should be stand alone. No other uStack features \
should be defined at the same time."
#endif /* FEATURE_OBSERVER || FEATURE_SCANNER */
#endif /* FEATURE_MONITOR */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
// Clock Event
#define UG_CLKEVT_BCAST_DUTY          0x0001
#define UG_CLKEVT_SCAN_TIMER          0x0002

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */
extern ubParams_t ubParams;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
extern bStatus_t uble_buildAndPostEvt(ubEvtDst_t evtDst, ubEvt_t evt,
                                      ubMsg_t *pMsg, uint16 len);

/*********************************************************************
 * LOCAL VARIABLES
 */

#if defined(FEATURE_OBSERVER)
/* Micro GAP Observer Role State */
static ugoScanState_t ugoState = UG_SCAN_STATE_INVALID;
#endif /* FEATURE_OBSERVER */

#if defined(FEATURE_MONITOR)
/* Micro GAP Monitor State */
static ugmMonitorState_t ugmState = UG_MONITOR_STATE_INVALID;
#endif /* FEATURE_MONITOR */

#if defined(FEATURE_BROADCASTER)
/* Micro GAP Broadcaster Role State */
static ugBcastState_t ugbState = UG_BCAST_STATE_INVALID;

/* Clock for Broadcaster Duty Control */
Clock_Struct cBcastDuty;
Clock_Handle hcBcastDuty = NULL;

/* Set of application callbacks to be issued by Broadcaster */
static ugBcastCBs_t ugbAppCBs;

/*********************************************************************
 * Broadcaster Parameters
 */
static uint16 ugbNumAdvEvent;
static uint16 ugbDutyOnTime = 0;
static uint16 ugbDutyOffTime = 0; /* Duty control is off by default */
#endif /* FEATURE_BROADCASTER */

#if defined(FEATURE_OBSERVER)

/* Clock for Scan On/Off time Control */
Clock_Struct cScanTimer;
Clock_Handle hcScanTimer = NULL;

/* Set of application callbacks to be issued by Observer */
static ugoScanCBs_t ugoAppCBs;

/*********************************************************************
 * Observer Parameters
 */
static uint16 ugoScanOnTime = 0;
static uint16 ugoScanOffTime = 0;
static ugoScanState_t ugoStatePrev = UG_SCAN_STATE_INVALID;
static uint8 ugoScanChanMap = 0;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void ugo_scanWindowCompleteCB(bStatus_t status);

#endif /* FEATURE_OBSERVER */

#if defined(FEATURE_MONITOR)

/* Set of application callbacks to be issued by Monitor */
static ugmMonitorCBs_t ugmAppCBs;

/*********************************************************************
 * Monitor Parameters
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

#endif /* FEATURE_OBSERVER */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

#if defined(FEATURE_BROADCASTER)
/*********************************************************************
 * @fn      ugb_changeState
 *
 * @brief   Change Broadcaster State. If successful, post UGB_EVT_STATE_CHANGE
 *          event.
 *
 * @param   state - the new state to switch to
 *
 * @return  SUCCESS, INVALIDPARAMETER, FAILURE
 */
static bStatus_t ugb_changeState(ugBcastState_t state)
{
  volatile uint32 keySwi;
  bStatus_t status = SUCCESS;

  keySwi = Swi_disable();

  if (ugbState == state)
  {
    /* Caller is attempting to switch to the same state. Just return
       without posting UGB_EVT_STATE_CHANGE */
    Swi_restore(keySwi);
    return status;
  }

  switch (state)
  {
  case UG_BCAST_STATE_INITIALIZED:
    ugbState = state;
    break;

  case UG_BCAST_STATE_IDLE:
    if (Clock_isActive(hcBcastDuty))
    {
      Clock_stop(hcBcastDuty);
    }

    if (ugbState == UG_BCAST_STATE_ADVERTISING)
    {
      ull_advStop();
    }

#if defined(FEATURE_OBSERVER)
    ugap_scanResume();
#endif /* FEATURE_OBSERVER */

    ugbState = state;
    break;

  case UG_BCAST_STATE_ADVERTISING:

#if defined(FEATURE_OBSERVER)
    ugap_scanSuspend();
#endif /* FEATURE_OBSERVER */

    status = ull_advStart();
    if (status == SUCCESS)
    {
      /* Set the timeout for duty on time only if duty control is enabled */
      if (ugbDutyOnTime != 0 && ugbDutyOffTime != 0)
      {
        Clock_setTimeout(hcBcastDuty,
                         ugbDutyOnTime * UG_DUTY_TIME_UNIT * MS_TO_SYSTICK);
        Clock_start(hcBcastDuty);
      }

      ugbState = state;
    }
    break;

  case UG_BCAST_STATE_WAITING:
    ugbState = state;
    /* Set the timeout for duty off time */
    Clock_setTimeout(hcBcastDuty,
                     ugbDutyOffTime * UG_DUTY_TIME_UNIT * MS_TO_SYSTICK);
    Clock_start(hcBcastDuty);

    ull_advStop();

#if defined(FEATURE_OBSERVER)
    ugap_scanResume();
#endif /* FEATURE_OBSERVER */

    break;

  case UG_BCAST_STATE_SUSPENDED:
    /* TBD */
    break;

  default:
    status = INVALIDPARAMETER;
    break;
  }

  if (status == SUCCESS)
  {
    ugbMsgStateChange_t msgStateChange;

    msgStateChange.state = ugbState;
    status = uble_buildAndPostEvt(UB_EVTDST_GAP, UGB_EVT_STATE_CHANGE,
                                  (ubMsg_t*) &msgStateChange, sizeof(msgStateChange));
  }

  Swi_restore(keySwi);

  return status;
}
#endif /* FEATURE_BROADCASTER */

#if defined(FEATURE_OBSERVER)
/*********************************************************************
 * @fn      ugo_chanMapToIndex
 *
 * @brief   Conver the channel map the channel index. Mask out the
 *          channel map for the next call.
 *
 * @param   pChanMap - pointer to current channel map
 *
 * @return  Channel index
 */
static uint8_t ugo_chanMapToIndex(uint8_t *pChanMap)
{
  if (*pChanMap == 0)
  {
    /* Reload the scan channel map */
    *pChanMap = ubParams.scanChanMap;
  }
  if ((*pChanMap) & UB_ADV_CHAN_37)
  {
    (*pChanMap) &= ~UB_ADV_CHAN_37;
    return 37;
  }
  if ((*pChanMap) & UB_ADV_CHAN_38)
  {
    (*pChanMap) &= ~UB_ADV_CHAN_38;
    return 38;
  }
  if ((*pChanMap) & UB_ADV_CHAN_39)
  {
    (*pChanMap) &= ~UB_ADV_CHAN_39;
    return 39;
  }

  /* Should not happen */
  return 0;
}

/*********************************************************************
 * @fn      ugo_changeState
 *
 * @brief   Change Observer State. If successful, post UGS_EVT_STATE_CHANGE
 *          event.
 *
 * @param   state - the new state to switch to
 *
 * @return  SUCCESS, INVALIDPARAMETER, FAILURE
 */
static bStatus_t ugo_changeState(ugoScanState_t state)
{
  volatile uint32 keySwi;
  bStatus_t status = SUCCESS;

  keySwi = Swi_disable();

  if (ugoState == state)
  {
    /* Caller is attempting to switch to the same state. Just return
       without posting UGS_EVT_STATE_CHANGE */
    Swi_restore(keySwi);
    return status;
  }

  switch (state)
  {
  case UG_SCAN_STATE_INITIALIZED:
    ugoState = state;
    break;

  case UG_SCAN_STATE_IDLE:
    if (Clock_isActive(hcScanTimer))
    {
      Clock_stop(hcScanTimer);
    }

    if (ugoState == UG_SCAN_STATE_SCANNING)
    {
      ull_scanStop();
    }

    ugoState = state;
    break;

  case UG_SCAN_STATE_SCANNING:
    /* Change the scan timer and start scan on next channel. */
    status = ull_scanStart(ugo_chanMapToIndex(&ugoScanChanMap));
    if (status == SUCCESS)
    {
      ugoState = state;

      /* Set the timeout for scan if scan on time > 0 */
      if (ugoScanOnTime != 0)
      {
        Clock_setTimeout(hcScanTimer, ugoScanOnTime * BLE_TO_SYSTICK);
        Clock_start(hcScanTimer);
      }
    }
    break;

  case UG_SCAN_STATE_WAITING:
    ugoState = state;

    /* Change the scan timer and stop scan. */
    Clock_setTimeout(hcScanTimer, ugoScanOffTime * BLE_TO_SYSTICK);
    Clock_start(hcScanTimer);
    ull_scanStop();
    break;

  case UG_SCAN_STATE_SUSPENDED:
    if (ugoState == UG_SCAN_STATE_SCANNING)
    {
      /* If previous state is SCANNING, stop the ongoing scan timer */
      if (Clock_isActive(hcScanTimer))
      {
        Clock_stop(hcScanTimer);
      }

      /* If previous state is SCANNING, stop the ongoing scan */
      ull_scanStop();
    }

    /* SUSPEND could occur while waiting */
    ugoState = state;
    break;

  default:
    status = INVALIDPARAMETER;
    break;
  }

  if (status == SUCCESS)
  {
    ugoMsgStateChange_t msgStateChange;

    msgStateChange.state = ugoState;
    status = uble_buildAndPostEvt(UB_EVTDST_GAP, UGO_EVT_STATE_CHANGE,
                                  (ubMsg_t*) &msgStateChange, sizeof(msgStateChange));
  }

  Swi_restore(keySwi);

  return status;
}
#endif /* FEATURE_OBSERVER */

#if defined(FEATURE_MONITOR)

/*********************************************************************
 * @fn      ugm_changeState
 *
 * @brief   Change Monitor State. If successful, post UGM_EVT_STATE_CHANGE
 *          event.
 *
 * @param   state - the new state to switch to
 *
 * @return  SUCCESS, INVALIDPARAMETER, FAILURE
 */
static bStatus_t ugm_changeState(ugmMonitorState_t state)
{
  volatile uint32 keySwi;
  bStatus_t status = SUCCESS;

  keySwi = Swi_disable();

  if (ugmState == state)
  {
    /* Caller is attempting to switch to the same state. Just return
       without posting UGM_EVT_STATE_CHANGE */
    Swi_restore(keySwi);
    return status;
  }

  switch (state)
  {
  case UG_MONITOR_STATE_INITIALIZED:
    ugmState = state;
    break;

  case UG_MONITOR_STATE_IDLE:
    if (ugmState == UG_MONITOR_STATE_MONITORING)
    {
      ull_monitorStop();
      ugmState = state;
    }
    else
    {
      status = INVALIDPARAMETER;
    }
    break;

  case UG_MONITOR_STATE_MONITORING:
    /* Change start scan on next channel. */
    status = ull_monitorStart(ubParams.monitorChan);

    if (status == SUCCESS)
    {
      ugmState = state;
    }
    break;

  default:
    status = INVALIDPARAMETER;
    break;
  }

  if (status == SUCCESS)
  {
    ugmMsgStateChange_t msgStateChange;

    msgStateChange.state = ugmState;
    status = uble_buildAndPostEvt(UB_EVTDST_GAP, UGM_EVT_STATE_CHANGE,
                                  (ubMsg_t*) &msgStateChange, sizeof(msgStateChange));
  }

  Swi_restore(keySwi);

  return status;
}
#endif /* FEATURE_MONITOR */

/*********************************************************************
 * CALLBACKS
 */

/*********************************************************************
 * @fn      ugb_clockHandler
 *
 * @brief   Handles a clock event.
 *
 * @param   a0 - event
 *
 * @return  None
 */
void ugap_clockHandler(UArg a0)
{
#if defined(FEATURE_BROADCASTER)
  /* Process Broadcaster Duty Control */
  if (a0 == UG_CLKEVT_BCAST_DUTY)
  {
    if (ugbState == UG_BCAST_STATE_ADVERTISING)
    {
      /* ugbDutyOffTime could have been changed during advertising state,
         so should be checked here. */
      if (ugbDutyOffTime != 0)
      {
        if (ugb_changeState(UG_BCAST_STATE_WAITING) != SUCCESS)
        {
          /* What to do if failed? */
        }
      }
    }
    else /* if (ugbState == UG_BCAST_STATE_WAITING) */
    {
      if (ugb_changeState(UG_BCAST_STATE_ADVERTISING) != SUCCESS)
      {
        /* What to do if failed? */
      }
    }
  }
#endif /* FEATURE_BROADCASTER */

#if defined(FEATURE_OBSERVER)
  /* Process Scan Timer control */
  if (a0 == UG_CLKEVT_SCAN_TIMER)
  {
    if (ugoState == UG_SCAN_STATE_SCANNING)
    {
      /* ugoScanOffTime could have been changed during scanning state,
         so should be checked here. */
      if (ugoScanOffTime != 0)
      {
        if (ugo_changeState(UG_SCAN_STATE_WAITING) != SUCCESS)
        {
          /* What to do if failed? */
        }
      }
    }
    else if (ugoState == UG_SCAN_STATE_WAITING)
    {
      if (ugo_changeState(UG_SCAN_STATE_SCANNING) != SUCCESS)
      {
        /* What to do if failed? */
      }
    }
  }
#endif /* FEATURE_OBSERVER */
}

#if defined(FEATURE_BROADCASTER)
/*********************************************************************
 * @fn      ugb_advPrepareCB
 *
 * @brief   Do pre-processing of advertising event
 *
 * @param   None
 *
 * @return  None
 */
void ugb_advPrepareCB(void)
{
  volatile uint32 keySwi;

  keySwi = Swi_disable();
  if (ubParams.timeToAdv > 0)
  {
    /* Post UGB_EVT_ADV_PREPARE to itself so that it is processed
       in the application task's context */
    uble_buildAndPostEvt(UB_EVTDST_GAP, UGB_EVT_ADV_PREPARE, NULL, 0);
  }
  Swi_restore(keySwi);
}

/*********************************************************************
 * @fn      ugb_advPostprocessCB
 *
 * @brief   Do post-processing of advertising event
 *
 * @param   status - result of the last advertising event
 *
 * @return  none
 */
void ugb_advPostprocessCB(bStatus_t status)
{
  ugbMsgAdvPostprocess_t msg;
  volatile uint32 keySwi;

  keySwi = Swi_disable();
#ifndef RF_MULTI_MODE
  if (status == bleNoResources)
  {
    /* Failed to schedule next advertising event. Switch the state to Idle. */
    ugb_changeState(UG_BCAST_STATE_IDLE);
  }
  else
#endif /* !RF_MULTI_MODE */
  {
    msg.status = status;
    /* Post UGB_EVT_ADV_POSTPROCESS to itself so that it is processed
       in the application task's context */
    uble_buildAndPostEvt(UB_EVTDST_GAP, UGB_EVT_ADV_POSTPROCESS,
                         (ubMsg_t*) &msg, sizeof(ugbMsgAdvPostprocess_t));

    if (ugbNumAdvEvent > 0)
    {
      if (--ugbNumAdvEvent == 0)
      {
        ugb_changeState(UG_BCAST_STATE_IDLE);
      }
    }
  }
  Swi_restore(keySwi);
}
#endif /* FEATURE_BROADCASTER */

#if defined(FEATURE_OBSERVER)
/*********************************************************************
 * @fn      ugo_scanIndicationCB
 *
 * @brief   Do scan indication event
 *
 * @param status status of a scan
 * @param len length of the payload
 * @param pPayload pointer to payload
 *
 * @return  None
 */
void ugo_scanIndicationCB(bStatus_t status, uint8_t len, uint8_t *pPayload)
{
  ugoMsgScanIndication_t msg;
  volatile uint32 keySwi;

  keySwi = Swi_disable();
  msg.status = status;
  msg.len = len;
  msg.pPayload = pPayload;

  /* Post UGO_EVT_SCAN_INDICATION to itself so that it is processed
     in the application task's context */
  uble_buildAndPostEvt(UB_EVTDST_GAP, UGO_EVT_SCAN_INDICATION,
                       (ubMsg_t*) &msg, sizeof(ugoMsgScanIndication_t));
  Swi_restore(keySwi);
}

/*********************************************************************
 * @fn      ugo_scanCompleteCB
 *
 * @brief   Do scan complete event
 *
 * @param   status - result of the scan complete event
 *
 * @return  none
 */
static void ugo_scanWindowCompleteCB(bStatus_t status)
{
  ugoMsgScanWindowComplete_t msg;
  volatile uint32 keySwi;

  keySwi = Swi_disable();
  msg.status = status;
  /* Post UGB_EVT_SCAN_WINDOW_COMPLETE to itself so that it is processed
     in the application task's context */
  uble_buildAndPostEvt(UB_EVTDST_GAP, UGO_EVT_SCAN_WINDOW_COMPLETE,
                       (ubMsg_t*) &msg, sizeof(ugoMsgScanWindowComplete_t));
  Swi_restore(keySwi);
}
#endif /* FEATURE_OBSERVER */

#if defined(FEATURE_MONITOR)
/*********************************************************************
 * @fn      ugm_monitorIndicationCB
 *
 * @brief   Do monitor indication event
 *
 * @param status status of a monitoring scan
 * @param sessionId
 * @param len length of the payload
 * @param pPayload pointer to payload
 *
 * @return  None
 */
static void ugm_monitorIndicationCB(bStatus_t status, uint8_t sessionId, uint8_t len, uint8_t *pPayload)
{
  ugmMsgMonitorIndication_t msg;
  volatile uint32 keySwi;

  keySwi = Swi_disable();
  msg.status = status;
  msg.sessionId = sessionId;
  msg.len = len;
  msg.pPayload = pPayload;

  /* Post UGM_EVT_MONITOR_INDICATION to itself so that it is processed
     in the application task's context */
  uble_buildAndPostEvt(UB_EVTDST_GAP, UGM_EVT_MONITOR_INDICATION,
                       (ubMsg_t*) &msg, sizeof(ugmMsgMonitorIndication_t));
  Swi_restore(keySwi);
}

/*********************************************************************
 * @fn      ugm_monitorCompleteCB
 *
 * @brief   Do monitor complete event
 *
 * @param   status - result of the monitor complete event
 *
 * @return  none
 */
static void ugm_monitorCompleteCB(bStatus_t status, uint8_t sessionId)
{
  ugmMsgMonitorComplete_t msg;
  volatile uint32 keySwi;

  keySwi = Swi_disable();
  msg.status = status;
  msg.sessionId = sessionId;
  /* Post UGM_EVT_MONITOR_COMPLETE to itself so that it is processed
     in the application task's context */
  uble_buildAndPostEvt(UB_EVTDST_GAP, UGM_EVT_MONITOR_COMPLETE,
                       (ubMsg_t*) &msg, sizeof(ugmMsgMonitorComplete_t));
  Swi_restore(keySwi);
}
#endif /* FEATURE_MONITOR */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

#if defined(FEATURE_BROADCASTER)
/*********************************************************************
 * @fn      ugap_bcastInit
 *
 * @brief   Initialize Micro Broadcaster
 *
 * @param   pCBs - a set of application callbacks for Broadcaster
 *
 * @return  SUCCESS, INVALIDPARAMETER, FAILURE
 */
bStatus_t ugap_bcastInit(ugBcastCBs_t* pCBs)
{
  if (ugbState != UG_BCAST_STATE_INVALID &&
      ugbState != UG_BCAST_STATE_INITIALIZED &&
      ugbState != UG_BCAST_STATE_IDLE)
  {
    /* Cannot re-initialize if the state is one of the active ones */
    return FAILURE;
  }

  if (hcBcastDuty == NULL)
  {
    Clock_Params clockParams;

    /* Setup BcastDuty timer as a one-shot timer */
    Clock_Params_init(&clockParams);
    clockParams.arg = UG_CLKEVT_BCAST_DUTY;
    clockParams.period = 0;
    clockParams.startFlag = false;
    Clock_construct(&cBcastDuty, ugap_clockHandler, 0, &clockParams);
    hcBcastDuty = Clock_handle(&cBcastDuty);
  }

  if (pCBs != NULL)
  {
    /* Register application callbacks to be called by Broadcaster */
    memcpy(&ugbAppCBs, pCBs, sizeof(ugBcastCBs_t));
  }

  /* Register advertiser-related uGAP callback in the uLL */
  ull_advRegisterCB(ugb_advPrepareCB, ugb_advPostprocessCB);

  return ugb_changeState(UG_BCAST_STATE_INITIALIZED);
}

/*********************************************************************
 * @fn      ugap_bcastSetDuty
 *
 * @brief   Set Broadcaster Duty On/Off time
 *
 * @param   dutyOnTime  - Time period during which the Broadcaster
 *                        is in Advertising state. 100 ms unit. This cannot be
 *                        0 unless dutyOffTime is also 0.
 *          dutyOffTime - Time period during which the Broadcaster
 *                        is in Waiting state. 100 ms unit. If this is 0,
 *                        Duty Control is disabled regardless of dutyOnTime.
 *
 * @return  SUCCESS or INVALIDPARAMETER
 */
bStatus_t ugap_bcastSetDuty(uint16 dutyOnTime, uint16 dutyOffTime)
{
  if (dutyOnTime == 0 && dutyOffTime != 0)
  {
    return INVALIDPARAMETER;
  }

  ugbDutyOnTime = dutyOnTime;

  if (ugbState == UG_BCAST_STATE_ADVERTISING &&
      ugbDutyOffTime == 0 && dutyOffTime != 0)
  {
    /* If duty control gets enabled while advertising,
       start from WAITING state */
    ugbDutyOffTime = dutyOffTime;
    ugb_changeState(UG_BCAST_STATE_WAITING);
  }
  else
  {
    ugbDutyOffTime = dutyOffTime;
  }

  return SUCCESS;
}

/*********************************************************************
 * @fn      ugap_bcastStart
 *
 * @brief   Start Broadcaster. Proceed the state from either Initialized or IDLE
 *          to Advertising.
 *
 * @param   numAdvEvent - # of Advertising events to be performed before
 *                        the Broadcaster state goes to IDLE. If this is 0,
 *                        the Broadcaster will keep staying in Advertising or
 *                        Advertising and Waiting alternately unless an error
 *                        happens or the application requests to stop.
 *
 * @return  SUCCESS, FAILURE, or INVALIDPARAMETER
 */
bStatus_t ugap_bcastStart(uint16 numAdvEvent)
{
  /* Micro Broadcaster can start only from UG_BCAST_STATE_INITIALIZED state */
  if (ugbState != UG_BCAST_STATE_INITIALIZED && ugbState != UG_BCAST_STATE_IDLE)
  {
    return FAILURE;
  }

  ugbNumAdvEvent = numAdvEvent;

  return ugb_changeState(UG_BCAST_STATE_ADVERTISING);
}

/*********************************************************************
 * @fn      ugap_bcastStop
 *
 * @brief   Stop Broadcaster. Proceed the state from either Advertising or
 *          Waiting to IDLE.
 *
 * @return  SUCCESS, FAILURE, or INVALIDPARAMETER
 */
bStatus_t ugap_bcastStop(void)
{
  /* Micro Broadcaster should have ever been initialized */
  if (ugbState == UG_BCAST_STATE_INVALID ||
      ugbState == UG_BCAST_STATE_INITIALIZED)
  {
    return FAILURE;
  }

  return ugb_changeState(UG_BCAST_STATE_IDLE);
}
#endif /* FEATURE_BROADCASTER */

#if defined(FEATURE_OBSERVER)
/*********************************************************************
 * @fn      ugap_scanInit
 *
 * @brief   Initialize Micro Observer
 *
 * @param   pCBs - a set of application callbacks for Ovserver
 *
 * @return  SUCCESS, INVALIDPARAMETER, FAILURE
 */
bStatus_t ugap_scanInit(ugoScanCBs_t* pCBs)
{
  if (ugoState != UG_SCAN_STATE_INVALID &&
      ugoState != UG_SCAN_STATE_INITIALIZED &&
      ugoState != UG_SCAN_STATE_IDLE)
  {
    /* Cannot re-initialize if the state is one of the active ones */
    return FAILURE;
  }

  if (hcScanTimer == NULL)
  {
    Clock_Params clockParams;

    /* Setup ScanTimer as a one-shot timer */
    Clock_Params_init(&clockParams);
    clockParams.arg = UG_CLKEVT_SCAN_TIMER;
    clockParams.period = 0;
    clockParams.startFlag = false;
    Clock_construct(&cScanTimer, ugap_clockHandler, 0, &clockParams);
    hcScanTimer = Clock_handle(&cScanTimer);
  }

  if (pCBs != NULL)
  {
    /* Register application callbacks to be called by Broadcaster */
    memcpy(&ugoAppCBs, pCBs, sizeof(ugoScanCBs_t));
  }

  /* Register observer-related uGAP callback in the uLL */
  ull_scanRegisterCB(ugo_scanIndicationCB, ugo_scanWindowCompleteCB);

  return ugo_changeState(UG_SCAN_STATE_INITIALIZED);
}

/*********************************************************************
 * @fn      ugap_scanStart
 *
 * @brief   Start Observer scanning. Proceed the state from either Initialized
 *          or IDLE to Scanning.
 *
 * @return  SUCCESS, FAILURE, or INVALIDPARAMETER
 */
bStatus_t ugap_scanStart(void)
{
  /* Micro Ovserver can start only from UG_OBSERVER_STATE_INITIALIZED state */
  if (ugoState != UG_SCAN_STATE_INITIALIZED && ugoState != UG_SCAN_STATE_IDLE)
  {
    return FAILURE;
  }

  return ugo_changeState(UG_SCAN_STATE_SCANNING);
}

/*********************************************************************
 * @fn      ugap_scanStop
 *
 * @brief   Stop Observer scanning. Proceed the state from Scanning to Idle.
 *
 * @return  SUCCESS, FAILURE, or INVALIDPARAMETER
 */
bStatus_t ugap_scanStop(void)
{
  /* Micro Observer should have ever been initialized */
  if (ugoState == UG_SCAN_STATE_INVALID || ugoState == UG_SCAN_STATE_INITIALIZED)
  {
    return FAILURE;
  }

  return ugo_changeState(UG_SCAN_STATE_IDLE);
}

/*********************************************************************
 * @fn      ugap_scanSuspend
 *
 * @brief   Suspend Observer scanning. Proceed the state from either Waiting
 *          or Scanning to Suspended. Suspend the Obesrver to make room
 *          for the Advertiser to transmit.
 *
 * @return  SUCCESS or FAILURE
 */
bStatus_t ugap_scanSuspend(void)
{
  /* Micro Observer should have ever been initialized */
  if (ugoState != UG_SCAN_STATE_WAITING && ugoState != UG_SCAN_STATE_SCANNING)
  {
    return FAILURE;
  }

  ugoStatePrev = ugoState;
  return ugo_changeState(UG_SCAN_STATE_SUSPENDED);
}

/*********************************************************************
 * @fn      ugap_scanResume
 *
 * @brief   Resume a suspended Observer. Proceed the state from Suspended
 *          to Scanning to Waiting.
 *
 * @return  SUCCESS or FAILURE
 */
bStatus_t ugap_scanResume(void)
{
  bStatus_t status = FAILURE;

  /* Micro Observer should have ever been suspended */
  if (ugoState == UG_SCAN_STATE_SUSPENDED)
  {
    if (ugoStatePrev == UG_SCAN_STATE_WAITING)
    {
      status = ugo_changeState(UG_SCAN_STATE_WAITING);
    }
    if (ugoStatePrev == UG_SCAN_STATE_SCANNING)
    {
      status = ugo_changeState(UG_SCAN_STATE_SCANNING);
    }
  }
  return status;
}

/*********************************************************************
 * @fn      ugap_scanRequest
 *
 * @brief   uGAP scan request
 *
 * @param   scanChanMap - Channels to scan advertising PDUs. One of more
 *                        of channels 37, 38 & 39. If 0, ubParam.scanChanMap is used.
 *          scanWindow - Scan window to listen on a primary advertising
 *                       channel index for the duration of the scan window.
 *                       Unit in 0.625 ms; range from 2.5 ms to 40.959375 s.
 *                       If 0, ubParam.scanWindow is used.
 *          scanInterval - Scan interval. This is defined as the start of
 *                         two consecutive scan windows. Unit in 0.625 ms;
 *                         range from 2.5 ms to 40.959375 s. When scanInterval
 *                         is the same as sacnWindow, a continuous scan
 *                         is requested. If 0, ubParam.scanInterval is used.
 *
 * @return  SUCCESS, INVALIDPARAMETER, FAILURE
 */
bStatus_t ugap_scanRequest(uint8_t scanChanMap, uint16_t scanWindow, uint16_t scanInterval)
{
  if (ugoState == UG_SCAN_STATE_SCANNING)
  {
    return FAILURE;
  }

  if (scanWindow > scanInterval)
  {
    return INVALIDPARAMETER;
  }

  if (scanChanMap & UB_ADV_CHAN_ALL)
  {
    /* Override ubParam if there is some channel to scan */
    ubParams.scanChanMap = scanChanMap;
  }

  if (scanWindow > 0)
  {
    if (scanWindow >= UB_MIN_SCAN_WINDOW && scanWindow <= UB_MAX_SCAN_WINDOW)
    {
      /* Override ubParam if scanWindow is in range */
      ubParams.scanWindow = scanWindow;
    }
    else
    {
      return INVALIDPARAMETER;
    }
  }

  if (scanInterval > 0)
  {
    if (scanInterval >= UB_MIN_SCAN_INTERVAL && scanInterval <= UB_MAX_SCAN_INTERVAL)
    {
      /* Override ubParam if scanInterval is in range */
      ubParams.scanInterval = scanInterval;
    }
    else
    {
      return INVALIDPARAMETER;
    }
  }

  ugoScanOnTime = ubParams.scanWindow;
  ugoScanOffTime = ubParams.scanInterval - ubParams.scanWindow;
  ugoScanChanMap = ubParams.scanChanMap;

  /* Start scanning */
  return ugap_scanStart();
}
#endif /* FEATURE_OBSERVER */

#if defined(FEATURE_MONITOR)
/*********************************************************************
 * @fn      ugap_monitorInit
 *
 * @brief   Initialize Micro Monitor
 *
 * @param   pCBs - a set of application callbacks for Monitor
 *
 * @return  SUCCESS, INVALIDPARAMETER, FAILURE
 */
bStatus_t ugap_monitorInit(ugmMonitorCBs_t* pCBs)
{
  if (ugmState != UG_MONITOR_STATE_INVALID &&
      ugmState != UG_MONITOR_STATE_INITIALIZED &&
      ugmState != UG_MONITOR_STATE_IDLE)
  {
    /* Cannot re-initialize if the state is one of the active ones */
    return FAILURE;
  }

  if (pCBs != NULL)
  {
    /* Register application callbacks to be called by Broadcaster */
    memcpy(&ugmAppCBs, pCBs, sizeof(ugmMonitorCBs_t));
  }

  /* Register monitor-related uGAP callback in the uLL */
  ull_monitorRegisterCB(ugm_monitorIndicationCB, ugm_monitorCompleteCB);

  return ugm_changeState(UG_MONITOR_STATE_INITIALIZED);
}

/*********************************************************************
 * @fn      ugap_monitorStart
 *
 * @brief   Start Monitoring scan. Proceed the state from either Initialized
 *          or IDLE to Monitoring.
 *
 * @return  SUCCESS, FAILURE, or INVALIDPARAMETER
 */
bStatus_t ugap_monitorStart(void)
{
  /* Micro Monitor can start only from UG_MONITOR_STATE_INITIALIZED state */
  if (ugmState != UG_MONITOR_STATE_INITIALIZED && ugmState != UG_MONITOR_STATE_IDLE)
  {
    return FAILURE;
  }

  return ugm_changeState(UG_MONITOR_STATE_MONITORING);
}

/*********************************************************************
 * @fn      ugap_monitorStop
 *
 * @brief   Stop Monitoring scan. Proceed the state from Monitoring to Idle.
 *
 * @return  SUCCESS, FAILURE, or INVALIDPARAMETER
 */
bStatus_t ugap_monitorStop(void)
{
  /* Micro Monitor should have not been initialized */
  if (ugmState == UG_MONITOR_STATE_INVALID || ugmState == UG_MONITOR_STATE_INITIALIZED)
  {
    return FAILURE;
  }

  return ugm_changeState(UG_MONITOR_STATE_IDLE);
}

/*********************************************************************
 * @fn      ugap_monitorRequest
 *
 * @brief   uGAP monitor request
 *
 * @param   channel - Channels to monitor.
 *          accessAddr - Access address. 0xFFFFFFFF is used as a
 *                       wildcard access address. An error status will
 *                       be returned if the channel is one of 37, 38,
 *                       or 39 but the accessAddr is not 0x8E89BED6.
 *          startTime - Absolute start time in RAT ticks.
 *          duration - Scan window per scanChanIndexchanIndex. A monitor
 *                     complete time-out statuscallback will be sent to
 *                     the application. A zero indicates a continuous
 *                     scan. Unit in 0.625 ms; range from 2.5 ms to
 *                     40.959375 s.
 *
 * @return  SUCCESS or INVALIDPARAMETER
 */
bStatus_t ugap_monitorRequest(uint8_t  channel,
                              uint32_t accessAddr,
                              uint32_t startTime,
                              uint16_t duration)
{
  if (ugmState == UG_MONITOR_STATE_MONITORING)
  {
    return FAILURE;
  }

  /* An error status will be returned if the channel is one of 37, 38,
   * or 39 but the accessAddr is not 0x8E89BED6.
   */
  if (((channel == 37) || (channel == 38) || (channel == 39)) &&
      (accessAddr != 0x8E89BED6))
  {
    return INVALIDPARAMETER;
  }

  /* Override ubParam */
  ubParams.monitorChan = channel;
  ubParams.startTime = startTime;
  ubParams.accessAddr = accessAddr;

  if (duration <= UB_MAX_MONITOR_DURATION)
  {
    /* Override ubParam if duration is in range */
    ubParams.monitorDuration = duration;
  }
  else
  {
    return INVALIDPARAMETER;
  }

  /* Start monitor scan */
  return ugap_monitorStart();
}
#endif /* FEATURE_MONITOR */

/*********************************************************************
 * @fn      ugap_processGAPMsg
 *
 * @brief   Process event messages sent from Micro BLE Stack to uGAP through
 *          the application's Stack Event Proxy and uble_processMsg().
 *
 * @param   pEvtMsg - Pointer to the Micro BLE Stack event message destined to
 *                    uGAP.
 *
 * @return  none
 */
void uble_processGAPMsg(ubEvtMsg_t *pEvtMsg)
{
  switch (pEvtMsg->hdr.evt)
  {
#if defined(FEATURE_BROADCASTER)
  case UGB_EVT_ADV_PREPARE:
    if (ugbAppCBs.pfnAdvPrepareCB != NULL)
    {
      ugbAppCBs.pfnAdvPrepareCB();
    }
    break;

  case UGB_EVT_ADV_POSTPROCESS:
    if (ugbAppCBs.pfnAdvDoneCB != NULL)
    {
      ugbMsgAdvPostprocess_t *pMsg = (ugbMsgAdvPostprocess_t*) &(pEvtMsg->msg);
      ugbAppCBs.pfnAdvDoneCB(pMsg->status);
    }
    break;

  case UGB_EVT_STATE_CHANGE:
    if (ugbAppCBs.pfnStateChangeCB != NULL)
    {
      ugbMsgStateChange_t *pMsg = (ugbMsgStateChange_t*) &(pEvtMsg->msg);
      ugbAppCBs.pfnStateChangeCB(pMsg->state);
    }
    break;
#endif /* FEATURE_BROADCASTER */

#if defined(FEATURE_OBSERVER)
  case UGO_EVT_SCAN_INDICATION:
    if (ugoAppCBs.pfnScanIndicationCB != NULL)
    {
      ugoMsgScanIndication_t *pMsg = (ugoMsgScanIndication_t*) &(pEvtMsg->msg);
      ugoAppCBs.pfnScanIndicationCB(pMsg->status, pMsg->len, pMsg->pPayload);
    }
    break;

  case UGO_EVT_SCAN_WINDOW_COMPLETE:
    /* The App scan window complete is called at the end of scan window */
    if (ugoAppCBs.pfnScanWindowCompleteCB != NULL)
    {
      ugoMsgScanWindowComplete_t *pMsg = (ugoMsgScanWindowComplete_t*) &(pEvtMsg->msg);
      ugoAppCBs.pfnScanWindowCompleteCB(pMsg->status);
    }
    break;

  case UGO_EVT_STATE_CHANGE:
    if (ugoAppCBs.pfnStateChangeCB != NULL)
    {
      ugoMsgStateChange_t *pMsg = (ugoMsgStateChange_t*) &(pEvtMsg->msg);
      ugoAppCBs.pfnStateChangeCB(pMsg->state);
    }
    break;
#endif /* FEATURE_OVSERVER */

#if defined(FEATURE_MONITOR)
  case UGM_EVT_MONITOR_INDICATION:
    if (ugmAppCBs.pfnMonitorIndicationCB != NULL)
    {
      ugmMsgMonitorIndication_t *pMsg = (ugmMsgMonitorIndication_t*) &(pEvtMsg->msg);
      ugmAppCBs.pfnMonitorIndicationCB(pMsg->status, pMsg->sessionId, pMsg->len, pMsg->pPayload);
    }
    break;

  case UGM_EVT_MONITOR_COMPLETE:
    /* Stop the current monitoring to trigger a state change */
    ugap_monitorStop();

    /* The App monitor complete is called at the end of monitor duration */
    if (ugmAppCBs.pfnMonitorCompleteCB != NULL)
    {
      ugmMsgMonitorComplete_t *pMsg = (ugmMsgMonitorComplete_t*) &(pEvtMsg->msg);
      ugmAppCBs.pfnMonitorCompleteCB(pMsg->status, pMsg->sessionId);
    }
    break;

  case UGM_EVT_STATE_CHANGE:
    if (ugmAppCBs.pfnStateChangeCB != NULL)
    {
      ugmMsgStateChange_t *pMsg = (ugmMsgStateChange_t*) &(pEvtMsg->msg);
      ugmAppCBs.pfnStateChangeCB(pMsg->state);
    }
    break;
#endif /* FEATURE_MONITOR */

  default:
    break;
  }
}


/*********************************************************************
*********************************************************************/
