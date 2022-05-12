/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_GPIB/QMSL_GPIB_MSVC7/QSEQ_MS.cpp#5 $
 * $DateTime: 2016/04/04 17:51:51 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2014-2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#include "stdafx.h"

#include "QSEQ_MS.h"

// Include QMSL headers

#include "qlib.h" 
#include "qlib_defines.h"

//--------------------------------------------------------------------------------------------------------------------------------------------
static bool First_Run = true;

#define MACRO_IF_FIRST_RUN_THEN if ( First_Run )

//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------
//--- CDMA -----------------------------
//--------------------------------------
typedef struct
{
   long   Band;
   long   Channel;
   int      PwrCtrlType;      // 0=internal,1=external
   long   PwrCtrlCmd;         // enum 0=AGC,1=Up,2=Down
   bool   RPC_Enabled;      // ReversePower Control Enabled (0=ExplicitSetPoint (then TxPower_SetPoint is used) or 1=ReversePowerControl) 
   double   TxPower_SetPoint;   // Mobile Tx Power Level, IF RPC_Enabled= 0
   bool   TxOpenLoop;         // 0=disabled, 1=enabled
   int      Receiver;         // enum 0=PRx, 1=DRx (ENUM?)
   int      Rx_Action;         // ENUM... "None","SER"  ? one of multiple MS measurements?

   long   RadioConfig;
   long   Walsh;
   unsigned char   FFPC_Enable;
   long   Frame_Offset;
   long   FCH_Rate;
   long   Preambles_NumberOf;
   long   Tx_Pattern;

   long   HardHandoff_Duration_ms;
   long   FCH_StatusGet_Duration_ms;
   long   RxLevelGet_Duration_ms;
}
QSEQ_Mobile_Config_C2K_TYPE;

typedef struct
{
   long   NS_Call_Establish;   // all should be BOOLEAN....
   long   FCH_Status_Reset;
   long   FCH_Status_Get;
   long   Call_Release;            
}
QSEQ_Mobile_Action_C2K_TYPE;

typedef struct
{
   QSEQ_Measurement_TYPE   RxLevel;
   QSEQ_Measurement_TYPE   Ser;
   QSEQ_Measurement_TYPE   Fer;
}
QSEQ_Mobile_Meas_C2K_TYPE;

//-------------------------

typedef struct
{
   QSEQ_Mobile_Config_C2K_TYPE   Config;
   QSEQ_Mobile_Action_C2K_TYPE Action;
   QSEQ_Mobile_Meas_C2K_TYPE   Meas;
}
QSEQ_Mobile_C2KNS_TYPE;


//--------------------------------------
//--- WCDMA ----------------------------
//--------------------------------------
typedef struct
{
   long   Band;
}
QSEQ_Mobile_Config_WCDMA_TYPE;

typedef struct
{
   long   NS_Call_Establish;
   long   Call_Release;            // all should be BOOLEAN....
}
QSEQ_Mobile_Action_WCDMA_TYPE;

typedef struct
{
   QSEQ_Measurement_TYPE   dummy;
}
QSEQ_Mobile_Meas_WCDMA_TYPE;

typedef struct
{
   QSEQ_Mobile_Config_WCDMA_TYPE   Config;
   QSEQ_Mobile_Action_WCDMA_TYPE   Action;
   QSEQ_Mobile_Meas_WCDMA_TYPE      Meas;
}
QSEQ_Mobile_WCDMA_TYPE;

//--------------------------------------
//--- Generic --------------------------
//--------------------------------------
typedef struct
{
   long   FTM_BatchMode_Activate;
}
QSEQ_Mobile_Generic_TYPE;

//--------------------------------------
//--- Mobile Data ----------------------
//--------------------------------------
typedef struct
{
   QSEQ_Technology_Type      Technology;
   QSEQ_Sequence_TYPE         Sequence;
   QSEQ_Segment_TYPE         Segment;
   QSEQ_Mobile_Generic_TYPE   Generic;
   QSEQ_Mobile_C2KNS_TYPE      C2KNS;
   QSEQ_Mobile_WCDMA_TYPE      WCDMA;
}
QSEQ_Mobile_TYPE;

//--------------------------------------------------------------------------------------------------------------------------
static QSEQ_Mobile_TYPE   QSEQ_Mobile_State_Current = {};

static QSEQ_Sequence_type   *Sequence_LocalPointer;
static int               nSeq = -1;

static QSEQ_Mobile_TYPE   QSEQ_Mobile_SegData[QSEQ_SEGMENTS_MAX] = {};

//--------------------------------------------------------------------------------------------------------------------------
typedef struct
{
   int      Band_Class_PREVIOUS;
   int      Channel_PREVIOUS;
   int      TxPower_Type_PREVIOUS;
   int      Rev_Pwr_PREVIOUS;
}
QSEQ_LocalParameters_C2K_Type;

typedef struct
{
   long                     TotalSequenceTime_ms;
   QSEQ_LocalParameters_C2K_Type   C2KNS;
}
QSEQ_LocalParameters_Type;

QSEQ_LocalParameters_Type LOCAL_Param =
{
   0,   // TotalSequenceTime_ms
   -1,   // Band_Class_PREVIOUS
   -1,   // CDMA_Channel_PREVIOUS
   -1,   // CDMA_TxPower_Type_PREVIOUS
   -1   // CDMA_Rev_Pwr_PREVIOUS
};

typedef struct
{
   short         RxLevelx10;
   unsigned long   iNumFrame;
   unsigned long   iFrameError;
   unsigned long   iNumSymbol;
   unsigned long   iSymbolError;
}
QSEQ_Mobile_MeasDataCached_C2K_TYPE;

typedef struct
{
   QSEQ_Mobile_MeasDataCached_C2K_TYPE   C2K;
}
QSEQ_Mobile_MeasDataCached_TYPE;

static QSEQ_Mobile_MeasDataCached_TYPE QSEQ_Mobile_MeasDataCached_DEFAULT = {};

static QSEQ_Mobile_MeasDataCached_TYPE QSEQ_Mobile_MeasDataCached[QSEQ_SEGMENTS_MAX] = {};

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Sequence_Begin_Begin ( HANDLE g_hQMSL, const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Mobile_TYPE *Data )
{
   Data->Segment.Data.Index = 0;   // FIX leave here or move to "End" ?
   Data->Segment.Data.Count = 0;

   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         break;
      case QSEQ_ACTION_SUB_EXECUTE:
         break;   
      case QSEQ_ACTION_SUB_COLLECT:
         break;
      case QSEQ_ACTION_SUB_PROCESS:
         break;
      case QSEQ_ACTION_SUB_REPORT:
         break;
      default:
         break;
   }
}

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Sequence_Begin_Item_Mgmt (   int               index_master,
                              QSEQ_SubAction_Type   sub_action,
                              QSEQ_Mobile_TYPE   *Data )
{
   QSEQ_Sequence_type   *SeqItem = &Sequence_LocalPointer[index_master];

   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            switch ( SeqItem->SegmentItem )
            {
               case QSEQ_ATT_TECHNOLOGY:
                  {
                     if      ( 0 == strcmp ( SeqItem->String_Input, "CDMA2000" ) ) { Data->Technology = QSEQ_TECH_CDMA2000; }
                     else if ( 0 == strcmp ( SeqItem->String_Input, "WCDMA"    ) ) { Data->Technology = QSEQ_TECH_WCDMA;    }
                     else if ( 0 == strcmp ( SeqItem->String_Input, "GSM"      ) ) { Data->Technology = QSEQ_TECH_GSM;      }
                     else
                     {
                        Data->Technology = QSEQ_TECH_UNKNOWN;
                        printf("\n\nERROR: UNKNOWN QSEQ_ATT_TECHNOLOGY [%s]", SeqItem->String_Input );
                     }
                  }
                  break;
               default:
                  break;
            }
         }
         break;
      case QSEQ_ACTION_SUB_EXECUTE:
         break;   
      case QSEQ_ACTION_SUB_COLLECT:
         break;
      case QSEQ_ACTION_SUB_PROCESS:
         break;
      case QSEQ_ACTION_SUB_REPORT:
         break;
      default:
         break;
   }
}

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Sequence_Begin_End ( HANDLE g_hQMSL, const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Mobile_TYPE *Data )
{
   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            MACRO_IF_FIRST_RUN_THEN printf("\n\nQSEQ_MS_Sequence_Begin_End() [START]: CONFIG"); 

            LOCAL_Param.TotalSequenceTime_ms =  0;
            
            for ( int i=0; i<QSEQ_SEGMENTS_MAX; i++ )
            {
               QSEQ_Mobile_MeasDataCached[i] = QSEQ_Mobile_MeasDataCached_DEFAULT;      // fix - technology specific - split up (proib not worth it, trivial one-time? )
            }

            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     LOCAL_Param.C2KNS.Band_Class_PREVIOUS      = -1;
                     LOCAL_Param.C2KNS.Channel_PREVIOUS         = -1;
                     LOCAL_Param.C2KNS.TxPower_Type_PREVIOUS      = -1;
                     LOCAL_Param.C2KNS.Rev_Pwr_PREVIOUS         = -9999;
                  }      
                  break;
               case QSEQ_TECH_WCDMA:
                  break;
               case QSEQ_TECH_GSM:
                  break;
               default:
                  printf("\nERROR: Unknown Technology [%s,&d]", __FUNCTION__, __LINE__); 
                  break;
            }

            MACRO_IF_FIRST_RUN_THEN printf("\nQSEQ_MS_Sequence_Begin_End() [STOP]: CONFIG"); 
         }   
         break;
      case QSEQ_ACTION_SUB_EXECUTE:
         break;   
      case QSEQ_ACTION_SUB_COLLECT:
         break;
      case QSEQ_ACTION_SUB_PROCESS:
         break;
      case QSEQ_ACTION_SUB_REPORT:
         break;
      default:
         break;
   }
}

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Sequence_Segment_Begin ( HANDLE g_hQMSL, const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Mobile_TYPE *Data )
{
   int id = ++Data->Segment.Data.Index; // one-based (INCREMENT here)
            ++Data->Segment.Data.Count;

   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            Data->Generic.FTM_BatchMode_Activate      = 0;

            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     Data->C2KNS.Action.NS_Call_Establish      = 0;
                     Data->C2KNS.Action.FCH_Status_Reset         = 0;
                     Data->C2KNS.Action.FCH_Status_Get         = 0;

                     Data->C2KNS.Meas.RxLevel.Config.Enabled      = 0;
                     Data->C2KNS.Meas.Ser.Config.Enabled         = 0;
                     Data->C2KNS.Meas.Fer.Config.Enabled         = 0;
                  }
                  break;
               case QSEQ_TECH_WCDMA:
                  break;
               case QSEQ_TECH_GSM:
                  break;
               default:
                  printf("\nERROR: Unknown Technology [%s,&d]", __FUNCTION__, __LINE__); 
                  break;
            }
         }
         break;
      case QSEQ_ACTION_SUB_EXECUTE:
         break;   
      case QSEQ_ACTION_SUB_COLLECT:
         {
            QSEQ_Mobile_TYPE Data_Recorded = QSEQ_Mobile_SegData [Data->Segment.Data.Index];
         }
         break;
      case QSEQ_ACTION_SUB_PROCESS:
         break;
      case QSEQ_ACTION_SUB_REPORT:
         break;
      default:
         break;
   }
}

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Segment_Item_Mgmt_CONFIG_C2K  ( const int index_master, QSEQ_Mobile_TYPE *Data );
void QSEQ_MS_Segment_Item_Mgmt_COLLECT_C2K ( const int index_master, QSEQ_Mobile_TYPE *Data );
//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Segment_Item_Mgmt ( int index_master, QSEQ_SubAction_Type sub_action, QSEQ_Mobile_TYPE   *Data )
{
   QSEQ_Sequence_type   *SeqItem = &Sequence_LocalPointer[index_master];

   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            switch ( SeqItem->SegmentItem )
            {
               case QSEQ_ATT_SEG_CNFG_DURATION_MS:
                  Data->Segment.Config.Duration_ms = atol (SeqItem->String_Input);
                  break;

               case QLIB_ATT_MS_ACT_FTM_BATCH_MODE_ACTIVATE:
                  Data->Generic.FTM_BatchMode_Activate = 1;
                  break;

               default:
                  break;
            }


            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     QSEQ_MS_Segment_Item_Mgmt_CONFIG_C2K ( index_master, Data );
                  }
                  break;
               case QSEQ_TECH_WCDMA:
                  break;
               case QSEQ_TECH_GSM:
                  break;
               default:
                  printf("\nERROR: Unknown Technology [%s,&d]", __FUNCTION__, __LINE__); 
                  break;
            }
         }
         break;
      case QSEQ_ACTION_SUB_EXECUTE:
         break;
      case QSEQ_ACTION_SUB_COLLECT:
         {   
            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     QSEQ_MS_Segment_Item_Mgmt_COLLECT_C2K ( index_master, Data );
                  }
                  break;
               case QSEQ_TECH_WCDMA:
                  break;
               case QSEQ_TECH_GSM:
                  break;
               default:
                  printf("\nERROR: Unknown Technology [%s,&d]", __FUNCTION__, __LINE__); 
                  break;
            }
         }
         break;
      case QSEQ_ACTION_SUB_PROCESS:
         break;
      case QSEQ_ACTION_SUB_REPORT:
         break;
      default:
         return;
   }
}

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Sequence_Segment_End_CONFIG_C2K   ( HANDLE g_hQMSL, QSEQ_Mobile_TYPE *Data );
void QSEQ_MS_Sequence_Segment_End_CONFIG_WCDMA ( HANDLE g_hQMSL, QSEQ_Mobile_TYPE *Data );

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Sequence_Segment_End ( HANDLE g_hQMSL, const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Mobile_TYPE *Data )
{
   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            int id               = Data->Segment.Data.Index;
            QSEQ_Mobile_SegData[id] = *Data;

            int   iStatus                  = 0;
            unsigned short iFTM_Error_Code   = -1;

            //-------------------------------------------------------------   
            LOCAL_Param.TotalSequenceTime_ms += Data->Segment.Config.Duration_ms;

            MACRO_IF_FIRST_RUN_THEN printf("\n\nQSEQ_MS_Sequence_Segment_End(SEG %d) [START]: CONFIG", id); 

            //-------------------------------------------------------------------------------------------
            //--- FTM Batch Mode - Activate --- DEVICE STATE CHANGE
            //---------------------------------
            if ( Data->Generic.FTM_BatchMode_Activate ) 
            {
               bool Messages_F3_Enabled = true;   // *** FIX ***

               if ( Messages_F3_Enabled ) 
               {
                  iStatus = QLIB_DIAG_SetPhoneLoggingState( g_hQMSL, 1 );
                  MACRO_IF_FIRST_RUN_THEN printf( "\nQLIB_DIAG_SetPhoneLoggingState(), Status=%d", iStatus);
                  // Factory Test Mode Msg
   //               iStatus = QLIB_DIAG_SetMessageFilter( g_hQMSL, 23, 0xFFFFFFFF );
   //               MACRO_IF_FIRST_RUN_THEN printf( "\nQLIB_DIAG_SetMessageFilter(), Status=%d", iStatus);
                  // Searcher L1 Msg
   //               iStatus = QLIB_DIAG_SetMessageFilter( g_hQMSL, 1007, 0xFFFFFFFF );
   //               MACRO_IF_FIRST_RUN_THEN printf( "\nQLIB_DIAG_SetMessageFilter(), Status=%d", iStatus);
               }

               unsigned char bSequencerRunningOnPC = 0; //sequencer engine on embedded side
               iStatus = QLIB_FTM_SEQ_SetSequencerMode( g_hQMSL, bSequencerRunningOnPC );
               MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_SetSequencerMode ( %d ): Status: %d", bSequencerRunningOnPC, iStatus);         

               //*******************************
               //***** Activate Batch Mode *****
               //*******************************
               iStatus = QLIB_FTM_SEQ_ActivateBatchMode( g_hQMSL );
               MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_ActivateBatchMode (): Status: %d", iStatus); 

               iStatus = QLIB_FTM_SEQ_CLEAR_QUEUE( g_hQMSL, &iFTM_Error_Code );
               MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_CLEAR_QUEUE (): Status: %d,%d", iStatus,iFTM_Error_Code);
            }

            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     QSEQ_MS_Sequence_Segment_End_CONFIG_C2K ( g_hQMSL, Data );
                  }
                  break;
               case QSEQ_TECH_WCDMA:
                  {
                     QSEQ_MS_Sequence_Segment_End_CONFIG_WCDMA ( g_hQMSL, Data );                  
                  }
                  break;
               case QSEQ_TECH_GSM:
                  break;
               default:
                  printf("\nERROR: Unknown Technology [%s,&d]", __FUNCTION__, __LINE__); 
                  break;
            }

            MACRO_IF_FIRST_RUN_THEN printf("\nQSEQ_MS_Sequence_Segment_End(SEG %d) [STOP]: CONFIG", id); 
         }
         break;
      case QSEQ_ACTION_SUB_EXECUTE:
         break;   
      case QSEQ_ACTION_SUB_COLLECT:
         break;
      case QSEQ_ACTION_SUB_PROCESS:
         break;
      case QSEQ_ACTION_SUB_REPORT:
         break;
      default:
         break;
   }
}

//-------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Sequence_End_Begin ( HANDLE g_hQMSL, const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Mobile_TYPE *Data )
{
   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            Data->C2KNS.Action.Call_Release = 0;
         }
         break;
      case QSEQ_ACTION_SUB_EXECUTE:
         break;   
      case QSEQ_ACTION_SUB_COLLECT:
         break;
      case QSEQ_ACTION_SUB_PROCESS:
         break;
      case QSEQ_ACTION_SUB_REPORT:
         break;
      default:
         break;
   }
}

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Sequence_End_Item_Mgmt (   int                  index_master,
                              QSEQ_SubAction_Type      sub_action,
                              QSEQ_Mobile_TYPE      *Data )
{
   QSEQ_Sequence_type   *SeqItem = &Sequence_LocalPointer[index_master];

   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            switch ( SeqItem->SegmentItem )
            {
               case QSEQ_ATT_MS_ACT_CALL_RELEASE:
                  Data->C2KNS.Action.Call_Release = 1;
                  break;
               default:
                  break;
            }
         }
         break;
      case QSEQ_ACTION_SUB_EXECUTE:
         break;   
      case QSEQ_ACTION_SUB_COLLECT:
         break;
      case QSEQ_ACTION_SUB_PROCESS:
         break;
      case QSEQ_ACTION_SUB_REPORT:
         break;
      default:
         break;
   }
}
                     
//-------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Sequence_End_End ( HANDLE g_hQMSL, const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Mobile_TYPE *Data )
{
   unsigned char iSequenceCompleted = false;
   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            int _iStatus = -1;

            MACRO_IF_FIRST_RUN_THEN printf("\n\nQSEQ_MS_Sequence_End_End() [START]: CONFIG"); 

            if ( Data->C2KNS.Action.Call_Release )      // ******************* FIX **** action generic or Tech-Specific ??????
            {
               switch ( Data->Technology ) 
               {
                  case QSEQ_TECH_CDMA2000:
                     {
                        _iStatus = QLIB_FTM_CDMA2000_CMD_RELEASE( g_hQMSL );                           // move to tech-specific function
                        MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_CDMA2000_CMD_RELEASE(): Status = %d", _iStatus);
                     }
                     break;
                  case QSEQ_TECH_WCDMA:
                     {
                        unsigned char   iStatus_QLIbFunctCall   = 0;
                        unsigned char   iStop_Mode_Status      = 0;
                        
                        iStatus_QLIbFunctCall =    QLIB_FTM_WCDMA_STOP_MODE_REQ( g_hQMSL, &iStop_Mode_Status);
                        MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_WCDMA_STOP_MODE_REQ(), Status=%d,%d", iStatus_QLIbFunctCall, iStop_Mode_Status );
                     }
                     break;
                  case QSEQ_TECH_GSM:
                     break;
                  default:
                     printf("\nERROR: Unknown Technology [%s,&d]", __FUNCTION__, __LINE__); 
                     break;
               }
            }

            MACRO_IF_FIRST_RUN_THEN printf("\nQSEQ_MS_Sequence_End_End() [STOP]: CONFIG");       
         }
         break;
      case QSEQ_ACTION_SUB_EXECUTE:
         {
            unsigned short iFTM_Error_Code;
            
            int _iStatus = -1;

            MACRO_IF_FIRST_RUN_THEN printf("\n\nQSEQ_MS_Sequence_End_End() [START]: EXECUTE"); 

            unsigned long t_start = GetTickCount();
            _iStatus = QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hQMSL, &iFTM_Error_Code, &iSequenceCompleted);      // Generic (fully?) ok here? - (Blocking) returns AFTER all BatchMode Commands have been executed
            unsigned long t_stop  = GetTickCount();

            MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_EXECUTE_QUEUE(): Status = %d [SegTotalTime= %d msec, ElapsedTime=%d]", _iStatus, LOCAL_Param.TotalSequenceTime_ms, t_stop- t_start );

            MACRO_IF_FIRST_RUN_THEN printf("\nQSEQ_MS_Sequence_End_End() [STOP]: EXECUTE");       

//            First_Run = false;

         }
         break;   
      case QSEQ_ACTION_SUB_COLLECT:
         break;
      case QSEQ_ACTION_SUB_PROCESS:
         break;
      case QSEQ_ACTION_SUB_REPORT:
         break;
      default:
         break;
   }
}

//--------------------------------------------------------------------------------------------------------------------------------------------
unsigned char QSEQ_Sequence_MS_Set ( long nSegItem, QSEQ_Sequence_type *Sequence )
{
   Sequence_LocalPointer = Sequence;

   nSeq = nSegItem;

   return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
unsigned char QSEQ_Sequence_MS (   HANDLE            g_hQMSL, 
                           QSEQ_SubAction_Type   sub_action )
{
   unsigned long t_start = GetTickCount();

   for ( int Index_SequenceMaster = 0; Index_SequenceMaster < nSeq; Index_SequenceMaster++ )
   {
      switch ( Sequence_LocalPointer[Index_SequenceMaster].Action )
      {
         case QSEQ_ACT_SEQ_BEGIN_BEGIN:   QSEQ_MS_Sequence_Begin_Begin   ( g_hQMSL, Index_SequenceMaster, sub_action, &QSEQ_Mobile_State_Current );   break;   //   CONFIG 
         case QSEQ_ACT_SEQ_BEGIN_ITEM:   QSEQ_MS_Sequence_Begin_Item_Mgmt (         Index_SequenceMaster, sub_action, &QSEQ_Mobile_State_Current );   break;   //   CONFIG
         case QSEQ_ACT_SEQ_BEGIN_END:   QSEQ_MS_Sequence_Begin_End      ( g_hQMSL, Index_SequenceMaster, sub_action, &QSEQ_Mobile_State_Current );   break;   //   CONFIG 

         case QSEQ_ACT_SEQ_SEG_BEGIN:   QSEQ_MS_Sequence_Segment_Begin   ( g_hQMSL, Index_SequenceMaster, sub_action, &QSEQ_Mobile_State_Current );   break;   //   CONFIG   COLLECT   
         case QSEQ_ACT_SEQ_SEG_ITEM:      QSEQ_MS_Segment_Item_Mgmt      (          Index_SequenceMaster, sub_action, &QSEQ_Mobile_State_Current );   break;   //   CONFIG   COLLECT
         case QSEQ_ACT_SEQ_SEG_END:      QSEQ_MS_Sequence_Segment_End   ( g_hQMSL, Index_SequenceMaster, sub_action, &QSEQ_Mobile_State_Current );   break;   //   CONFIG

         case QSEQ_ACT_SEQ_END_BEGIN:   QSEQ_MS_Sequence_End_Begin      ( g_hQMSL, Index_SequenceMaster, sub_action, &QSEQ_Mobile_State_Current );   break;
         case QSEQ_ACT_SEQ_END_ITEM:      QSEQ_MS_Sequence_End_Item_Mgmt  (          Index_SequenceMaster, sub_action, &QSEQ_Mobile_State_Current );   break;   //   CONFIG
         case QSEQ_ACT_SEQ_END_END:      QSEQ_MS_Sequence_End_End      ( g_hQMSL, Index_SequenceMaster, sub_action, &QSEQ_Mobile_State_Current );   break;   //               EXECUTE
         default:                                                                                                   break;
      }
   }

   unsigned long t_stop  = GetTickCount();

   printf("\n\nMS [%s] Elapsed Time = %d msec ******************************** Timing ***\n", QSEQ_SubAction_ENUM_to_String(sub_action), t_stop - t_start );
   
   return 0;
}








































//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Segment_Item_Mgmt_CONFIG_C2K (   int               index_master,
                                 QSEQ_Mobile_TYPE   *Data )
{
   QSEQ_Sequence_type   *SeqItem = &Sequence_LocalPointer[index_master];

   switch ( SeqItem->SegmentItem )
   {
      case QSEQ_ATT_C2KNS_SEG_CNFG_BAND:                  
         if      ( 0 == strcmp ( SeqItem->String_Input, "C2K_BandClass_0" ) ) { Data->C2KNS.Config.Band = FTM_CDMA2000_BAND_BC0;   }
         else if ( 0 == strcmp ( SeqItem->String_Input, "C2K_BandClass_1" ) ) { Data->C2KNS.Config.Band = FTM_CDMA2000_BAND_BC1; }
         else
         {
            Data->C2KNS.Config.Band = -1;   // FIX - ERROR MSG - ADD
            printf("\n\nERROR: UNKNOWN QSEQ_ATT_C2KNS_SEG_CNFG_BAND [%s]", SeqItem->String_Input );
         }
         break;
      case QSEQ_ATT_C2KNS_SEG_CNFG_CHANNEL:
         Data->C2KNS.Config.Channel = atol (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_DURATION_HARDHANDOFF_MS:
         Data->C2KNS.Config.HardHandoff_Duration_ms = atol (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_DURATION_FCH_STATUS_GET_MS:
         Data->C2KNS.Config.FCH_StatusGet_Duration_ms = atol (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_DURATION_RX_LEVEL_GET_MS:
         Data->C2KNS.Config.RxLevelGet_Duration_ms = atol (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTRL_TYPE:
         Data->C2KNS.Config.PwrCtrlType = atol (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTRL_CMD:
         Data->C2KNS.Config.PwrCtrlCmd = atol (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR:
         Data->C2KNS.Config.TxPower_SetPoint = atof (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_TX_OPEN_LOOP:
         Data->C2KNS.Config.TxOpenLoop = ( atol (SeqItem->String_Input) != 0 );
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_MOBILE_RX:
         Data->C2KNS.Config.Receiver = atol (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_RPC_ENABLE:
         Data->C2KNS.Config.RPC_Enabled = ( 0 != atol (SeqItem->String_Input) );
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_RADIO_CONFIG:
         Data->C2KNS.Config.RadioConfig = ( 0 != atol (SeqItem->String_Input) );
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_WALSH:
         Data->C2KNS.Config.Walsh = atol (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_FFPC_ENABLE:
         Data->C2KNS.Config.FFPC_Enable = ( 0 != atol (SeqItem->String_Input) );
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_FRAME_OFFSET:
         Data->C2KNS.Config.Frame_Offset = ( 0 != atol (SeqItem->String_Input) );
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_FCH_RATE:
         if ( strcmp ( "FULL", SeqItem->String_Input) )
         {
            // FIX ****************************************************
         }
         Data->C2KNS.Config.FCH_Rate = FTM_CDMA2000_NS_REV_FCH_FULL_RATE;
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_NUM_PREAMBLES:
         Data->C2KNS.Config.Preambles_NumberOf = ( 0 != atol (SeqItem->String_Input) );
         break;
      case QSEQ_ATT_C2KNS_MS_CNFG_TX_PATTERN:
         Data->C2KNS.Config.Tx_Pattern = ( 0 != atol (SeqItem->String_Input) );
         break;

      case QLIB_ATT_C2KNS_MS_ACT_CALL_ESTABLISH:
         Data->C2KNS.Action.NS_Call_Establish = 1;
         break;
      case QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_RESET:
         Data->C2KNS.Action.FCH_Status_Reset = 1;
         break;
      case QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_GET:
         Data->C2KNS.Action.FCH_Status_Get = 1;
         break;

      case QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL:
         Data->C2KNS.Meas.RxLevel.Config.Enabled = 1;
         break;
      case QSEQ_ATT_C2KNS_MS_MEAS_SER:
         Data->C2KNS.Meas.Ser.Config.Enabled = 1;
         break;
      case QSEQ_ATT_C2KNS_MS_MEAS_FER:
         Data->C2KNS.Meas.Fer.Config.Enabled = 1;
         break;

      default:
         break;
   }
}

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Segment_Item_Mgmt_COLLECT_C2K (   const int         index_master,
                                    QSEQ_Mobile_TYPE   *Data )
{
   QSEQ_Sequence_type   *SeqItem = &Sequence_LocalPointer[index_master];

   int id = Data->Segment.Data.Index;

   switch ( SeqItem->SegmentItem )
   {                                       // ? ok ignore - all other cases ? (yes)
      case QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL:
         sprintf ( SeqItem->String_Output, "%0.1f", (((double) QSEQ_Mobile_MeasDataCached[id].C2K.RxLevelx10) / 10.0 ) );
         break;
      case QSEQ_ATT_C2KNS_MS_MEAS_SER:
         {
            double ser_running = (((double)QSEQ_Mobile_MeasDataCached[id].C2K.iSymbolError) /  ((double)QSEQ_Mobile_MeasDataCached[id].C2K.iNumSymbol) ) * 100;

            double ser_inst = -1;

            if ( id > 0 )
            {
               int iSym_m1 = QSEQ_Mobile_MeasDataCached[id-1].C2K.iSymbolError;   // ***FIX*** logic * if two back-to-back (SEG-IDs) SERs at DIFF states - ERROR!!
               int nSym_m1 = QSEQ_Mobile_MeasDataCached[id-1].C2K.iNumSymbol;
               int iSym    = QSEQ_Mobile_MeasDataCached[id].C2K.iSymbolError;
               int nSym    = QSEQ_Mobile_MeasDataCached[id].C2K.iNumSymbol;

               if ( 0 != ( nSym - nSym_m1 ) )
               {
                  ser_inst = (((double)(iSym - iSym_m1 )) / ((double)(nSym - nSym_m1 )) ) * 100;
               }
            }

            sprintf ( SeqItem->String_Output, "%d, %d, %0.2f, %0.2f", 
                                       QSEQ_Mobile_MeasDataCached[id].C2K.iSymbolError,
                                       QSEQ_Mobile_MeasDataCached[id].C2K.iNumSymbol,
                                       ser_inst,
                                       ser_running );
         }
         break;
      case QSEQ_ATT_C2KNS_MS_MEAS_FER:
         {
            double fer_running = ( ((double)QSEQ_Mobile_MeasDataCached[id].C2K.iFrameError )  / ((double)QSEQ_Mobile_MeasDataCached[id].C2K.iNumFrame ) ) * 100;

            double fer_inst = -1;

            if ( id > 0 )
            {
               int iFrm_m1 = QSEQ_Mobile_MeasDataCached[id-1].C2K.iFrameError;
               int nFrm_m1 = QSEQ_Mobile_MeasDataCached[id-1].C2K.iNumFrame;
               int iFrm    = QSEQ_Mobile_MeasDataCached[id].C2K.iFrameError;
               int nFrm    = QSEQ_Mobile_MeasDataCached[id].C2K.iNumFrame;

               if ( 0 != ( nFrm - nFrm_m1 ) )
               {
                  fer_inst = (((double)(iFrm - iFrm_m1 )) / ((double)(nFrm - nFrm_m1 )) ) * 100;
               }
            }

            sprintf ( SeqItem->String_Output, "%d, %d, %0.2f, %0.2f",
                                       QSEQ_Mobile_MeasDataCached[id].C2K.iFrameError,
                                       QSEQ_Mobile_MeasDataCached[id].C2K.iNumFrame,
                                       fer_inst,
                                       fer_running );
         }
         break;
      default:
         break;
   }
}

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Sequence_Segment_End_CONFIG_C2K ( HANDLE g_hQMSL, QSEQ_Mobile_TYPE *Data )
{
   int   iStatus                  = 0;
   unsigned short iFTM_Error_Code   = -1;

   bool QLIB_FTM_CDMA2000_SET_REV_PWR_enabled = true;

   int id = Data->Segment.Data.Index;

   long SegmentLength_us = ((long) Data->Segment.Config.Duration_ms ) * 1000; // convert from ms to us

   long iRev_Pwr_indBm10 = (long)(Data->C2KNS.Config.TxPower_SetPoint * 10.0);

   unsigned char TxPower_Type    = -1;

   if ( Data->C2KNS.Config.RPC_Enabled )   { TxPower_Type = 0; }
   else                           { TxPower_Type = 1;   }

   //-------------------------------------------------------------------------------------------
   //--- Establish NS Call --- DEVICE STATE CHANGE
   //-------------------------
   long hard_wait = 1000 * 1000;

   if ( Data->C2KNS.Action.NS_Call_Establish )
   {         
      iStatus = QLIB_FTM_CDMA2000_PILOT_ACQ(   g_hQMSL,
                                    (unsigned long) Data->C2KNS.Config.Band, 
                                    (unsigned short) Data->C2KNS.Config.Channel,
                                    0 ); //unsigned long iAcq_Mode);  FTM_CDMA2000_NS_FTM_ACQ_MODE_FULL
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_CDMA2000_PILOT_ACQ(), Status=%d", iStatus);

      iStatus = QLIB_FTM_SEQ_ADD_WAIT(   g_hQMSL,      // ********************** REQUIRED *******************************
                                 hard_wait,
                                 &iFTM_Error_Code );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_ADD_WAIT(%d micro seconds): Status: %d",  hard_wait, iStatus);

      iStatus = QLIB_FTM_CDMA2000_DEMOD_SYNC(   g_hQMSL);
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_CDMA2000_DEMOD_SYNC(), Status=%d", iStatus);
      
      iStatus = QLIB_FTM_SEQ_ADD_WAIT(   g_hQMSL, // ********************** REQUIRED *******************************
                                 hard_wait,
                                 &iFTM_Error_Code );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_ADD_WAIT(%d micro seconds): Status: %d",  hard_wait, iStatus);

      iStatus = QLIB_FTM_CDMA2000_DEMOD_FCH(   g_hQMSL,      //RC_1,10,0,False,0,0,False,0,0,0,0,0)
                                    (unsigned long) Data->C2KNS.Config.RadioConfig,
                                    (unsigned char) Data->C2KNS.Config.Walsh,
                                    0,            // unsigned char iQOF_Mask_ID,
                                    false,         // unsigned char iBypass_LC,
                                    (unsigned char) Data->C2KNS.Config.Frame_Offset,
                                    0,            // unsigned char iSubChan_Gain,
                                    false,         //unsigned char iFFPC_Enable,
                                    0,            // unsigned long iFPC_Mode,
                                    0,            // unsigned char iInit_Setpt,
                                    0,            // unsigned char iOLC_Params_Target_FER,
                                    0,            // unsigned char iOLC_Params_Min_Setpt,
                                    0  );         // unsigned char iOLC_Params_Max_Setpt);
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_CDMA2000_DEMOD_FCH(), Status=%d", iStatus);

      iStatus = QLIB_FTM_SEQ_ADD_WAIT(   g_hQMSL,         // ********************** REQUIRED *******************************
                                 hard_wait,
                                 &iFTM_Error_Code );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_ADD_WAIT(%d micro seconds): Status: %d",  hard_wait, iStatus);

      LOCAL_Param.C2KNS.Band_Class_PREVIOUS   = Data->C2KNS.Config.Band;
      LOCAL_Param.C2KNS.Channel_PREVIOUS      = Data->C2KNS.Config.Channel;

      iStatus = QLIB_FTM_CDMA2000_MOD_FCH(   g_hQMSL, 
                                    (unsigned long) Data->C2KNS.Config.RadioConfig, 
                                    (unsigned char) Data->C2KNS.Config.Frame_Offset, 
                                    (unsigned long) Data->C2KNS.Config.FCH_Rate, 
                                    (! (unsigned char) Data->C2KNS.Config.RPC_Enabled), // ***INVERTED*** (boolean)  ? if RPC is NOT enabled then shouln't TxPwr be explicitly set (here?)?
                                    (unsigned short)Data->C2KNS.Config.Preambles_NumberOf,
                                    (unsigned long) Data->C2KNS.Config.Tx_Pattern );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_CDMA2000_MOD_FCH(), Status=%d", iStatus);

      iStatus = QLIB_FTM_SEQ_ADD_WAIT(   g_hQMSL, 
                                 hard_wait,// ********************** REQUIRED *******************************
                                 &iFTM_Error_Code );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_ADD_WAIT(%d micro seconds): Status: %d",  hard_wait, iStatus);

      if  ( ! Data->C2KNS.Config.RPC_Enabled ) 
      {
         iStatus = QLIB_FTM_CDMA2000_SET_REV_PWR(   g_hQMSL,
                                          1, // see comments   TxPower_Type, 
                                          iRev_Pwr_indBm10 );
         MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_CDMA2000_SET_REV_PWR(%0.2f dBm): Status: %d", Data->C2KNS.Config.TxPower_SetPoint, iStatus);
         LOCAL_Param.C2KNS.TxPower_Type_PREVIOUS   = 1;
         LOCAL_Param.C2KNS.Rev_Pwr_PREVIOUS      = iRev_Pwr_indBm10;
      }

      QLIB_FTM_CDMA2000_SET_REV_PWR_enabled = false;   // quick test REIMPLEMENT **********************************************************
   }
   
   //-------------------------------------------------------------------------------------------
   //--- Hard-Handoff --- DEVICE STATE CHANGE
   //--------------------
   if (   LOCAL_Param.C2KNS.Band_Class_PREVIOUS   !=   Data->C2KNS.Config.Band      ||      // pseudo state caching - revise (FIX)
         LOCAL_Param.C2KNS.Channel_PREVIOUS      !=   Data->C2KNS.Config.Channel      )
   {
      iStatus = QLIB_FTM_CDMA2000_FTM_FWD_HHO_SC( g_hQMSL, 
                                       (unsigned long)  Data->C2KNS.Config.Band, 
                                       (unsigned short) Data->C2KNS.Config.Channel,   // header file calls this "freq" when it is really a "channel" QLIB_FIX ?
                                       (!(unsigned char)  Data->C2KNS.Config.RPC_Enabled) );  //****INVERTED****//(boolean)
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_CDMA2000_FTM_FWD_HHO_SC(band=%d,chan=%d): Status: %d", Data->C2KNS.Config.Band, (unsigned short) Data->C2KNS.Config.Channel, iStatus);
      
      if ( Data->C2KNS.Config.HardHandoff_Duration_ms > 0 )
      {
         long wait = Data->C2KNS.Config.HardHandoff_Duration_ms * 1000;
      
         iStatus = QLIB_FTM_SEQ_ADD_WAIT(   g_hQMSL, 
                                    wait,
                                    &iFTM_Error_Code );
         MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_ADD_WAIT(%d micro seconds): Status: %d [HardHandoff]",  wait, iStatus);

         SegmentLength_us -= wait;
      } 
      LOCAL_Param.C2KNS.Band_Class_PREVIOUS   = Data->C2KNS.Config.Band;
      LOCAL_Param.C2KNS.Channel_PREVIOUS      = Data->C2KNS.Config.Channel;
   }

   //-------------------------------------------------------------------------------------------
   //--- Set Reverse Power --- DEVICE STATE CHANGE
   //-------------------------
   if ( QLIB_FTM_CDMA2000_SET_REV_PWR_enabled )
   {
      if (   LOCAL_Param.C2KNS.TxPower_Type_PREVIOUS   != TxPower_Type || 
            LOCAL_Param.C2KNS.Rev_Pwr_PREVIOUS      != iRev_Pwr_indBm10 )
      {
         // is there a scenario where this shound not be called (yes..)
         iStatus = QLIB_FTM_CDMA2000_SET_REV_PWR(   g_hQMSL,         
                                          TxPower_Type,   // Ignore Qlib.h comment for this param- error in implementation
                                          iRev_Pwr_indBm10 );
         MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_CDMA2000_SET_REV_PWR(%0.2f dBm): Status: %d", Data->C2KNS.Config.TxPower_SetPoint, iStatus);
         LOCAL_Param.C2KNS.TxPower_Type_PREVIOUS   = TxPower_Type;
         LOCAL_Param.C2KNS.Rev_Pwr_PREVIOUS      = iRev_Pwr_indBm10;
      }
   }

   //----------------------------------------------------------------------------------------------------------
   //--- FCH Status Reset --- MEASUREMENT RELATED *** By Definition, the FIRST Measurement Related function ***
   //----------------------------------------------------------------------------------------------------------
   if ( Data->C2KNS.Action.FCH_Status_Reset )
   {
      iStatus = QLIB_FTM_CDMA2000_RESET_FCH_STAT ( g_hQMSL );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_CDMA2000_RESET_FCH_STAT(): Status: %d",  iStatus);
   }

   //-------------------------------------------------------------------------------------------
   //-------------------------------------------------------------------------------------------
   //-------------------------------------------------------------------------------------------
   //--- Segment Critical Timing ---
   //-------------------------------
   long wait_Rx_Get  = 0;
   long wait_Fch_Get = 0;

   if ( Data->C2KNS.Meas.RxLevel.Config.Enabled )
   {
      wait_Rx_Get  = Data->C2KNS.Config.RxLevelGet_Duration_ms    * 1000;

      if ( wait_Rx_Get > 0 )
      {
         SegmentLength_us -= wait_Rx_Get;   // Account for call to QLIB_FTM_CDMA2000_RX_LEVEL_REQ() 
      }
   }
   if ( Data->C2KNS.Action.FCH_Status_Get   ||
      Data->C2KNS.Meas.Ser.Config.Enabled ||
      Data->C2KNS.Meas.Fer.Config.Enabled   )
   {
      wait_Fch_Get = Data->C2KNS.Config.FCH_StatusGet_Duration_ms * 1000;

      if ( wait_Fch_Get > 0 )
      {
         SegmentLength_us -= wait_Fch_Get;   // Account for call to QLIB_FTM_CDMA2000_GET_FCH_STAT() 
      }
   }

   if ( SegmentLength_us > 0 )
   {
      iStatus = QLIB_FTM_SEQ_ADD_WAIT(   g_hQMSL, 
                                 SegmentLength_us,
                                 &iFTM_Error_Code );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_ADD_WAIT(%d micro seconds): Status: %d",  SegmentLength_us, iStatus);
   }
   else if ( SegmentLength_us == 0 )
   {
      // ok - do nothing  *** CHECK ****
   }
   else
   {
      bool error =  true; 
   }

   //-------------------------------------------------------------------------------------------
   //--- Rx_Level --- MEASUREMENT RELATED ---
   //----------------------------------------
   if ( Data->C2KNS.Meas.RxLevel.Config.Enabled )
   {
      unsigned char   iRxChain = 0;   // 0 - Primary Rx, 1 - Secondary Rx
      unsigned char   iFTM_Status;
      short         iRxAGC;

      iStatus = QLIB_FTM_CDMA2000_RX_LEVEL_REQ(   g_hQMSL,
                                       iRxChain,
                                       &iFTM_Status,                        // ??? report this too ???
                                       &iRxAGC,                           // ??? report this too ???
                                       &QSEQ_Mobile_MeasDataCached[id].C2K.RxLevelx10 );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_CDMA2000_RX_LEVEL_REQ(): Status: %d",  iStatus);

      if ( wait_Rx_Get > 0 )
      {
         iStatus = QLIB_FTM_SEQ_ADD_WAIT(   g_hQMSL, 
                                    wait_Rx_Get,      // *** Presume 1-frame to execute instruction *** FIX ???
                                    &iFTM_Error_Code );
         MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_ADD_WAIT(%d micro seconds): Status: %d [Rx_Level_Get_Post]",  wait_Rx_Get, iStatus);
      }            
   }

   //-----------------------------------------------------------------------------------
   //--- FCH Status Get --- MEASUREMENT RELATED *** By Definition, the LAST function ***
   //-----------------------------------------------------------------------------------
   if ( Data->C2KNS.Action.FCH_Status_Get   ||      // revisit? is Data->C2KNS.Action.FCH_Status_Get redundant, feeling is - keep it explicit... (?)
      Data->C2KNS.Meas.Ser.Config.Enabled ||      // ? remove
      Data->C2KNS.Meas.Fer.Config.Enabled   )   // ? remove
   {   
      double dFER_not_computed;
      double dSER_not_computed;

      iStatus = QLIB_FTM_CDMA2000_GET_FCH_STAT(   g_hQMSL,
                                       &QSEQ_Mobile_MeasDataCached[id].C2K.iNumFrame,   // data is cached for collect/report in another function
                                       &QSEQ_Mobile_MeasDataCached[id].C2K.iFrameError,
                                       &dFER_not_computed,                     // not computed in FTM_BatchMode
                                       &QSEQ_Mobile_MeasDataCached[id].C2K.iNumSymbol, 
                                       &QSEQ_Mobile_MeasDataCached[id].C2K.iSymbolError,
                                       &dSER_not_computed);                  // not computed in FTM_BatchMode
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_CDMA2000_GET_FCH_STAT(): Status: %d", iStatus);

      if ( wait_Fch_Get > 0 )
      {
         iStatus = QLIB_FTM_SEQ_ADD_WAIT(   g_hQMSL, 
                                    wait_Fch_Get,      // *** Presume 1-frame to execute instruction *** FIX ???
                                    &iFTM_Error_Code );
         MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_ADD_WAIT(%d micro seconds): Status: %d [FCH_Status_Get_Post]",  wait_Fch_Get, iStatus);
      }
   }
}





//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_MS_Sequence_Segment_End_CONFIG_WCDMA ( HANDLE g_hQMSL, QSEQ_Mobile_TYPE *Data )
{
   int id = Data->Segment.Data.Index;

   long SegmentLength_us = ((long) Data->Segment.Config.Duration_ms ) * 1000; // convert from ms to us

   //-------------------------------------------------------------------------------------------
   //--- Establish NS Call --- DEVICE STATE CHANGE
   //-------------------------
   if ( 1 ) /// Data->WCDMA.Action.NS_Call_Establish )
   {         
      unsigned char   iStatus_QLIbFunctCall   = 0;
      unsigned char   iStatus_UC            = 0;
      unsigned short   iStatus_US            = 0;
      
      unsigned char   iHandOver_Status      = 0;
      unsigned char   iAcquire_Status         = 0;
      unsigned short   iStart_Mode_Done_Status   = 0;
      unsigned short   iAcquire_Done_Status   = 0;
      unsigned short   iHandover_Done_Status   = 0;
      unsigned char   iStart_Mode_Status      = 0;
      unsigned short   iClear_Register_Status   = 0;
      unsigned long   iTimeOut_MS            = 500; // milliseconds

      unsigned char   iAcqType   = FTM_WCDMA_BER_AcqTypeFreqOnly; 
      unsigned char   iAcqMode   = FTM_WCDMA_BER_AcqModeFull; 
      unsigned short   iFreq      = 10562; //
      unsigned short   iScrCode   = 0; //
      unsigned long   iPN_Pos      = 0;//

      unsigned char   iRMC_Type            = FTM_WCDMA_BER_RMC_Type12_2kpbs_Symmetric; //
      unsigned char   iUL_MaxTxPwr         = 24; //
      unsigned long   iUL_DPCH_ScrCode      = 0; //
      unsigned char   iUL_PCA               = FTM_WCDMA_BER_PCA_Type_Alg1;//
      unsigned char   iUL_TPC_StepSize      = FTM_WCDMA_BER_PCA_Size_1dB; //
      unsigned short   iDL_PriCPICH_Scr_Code   = 0; //
      unsigned short   iDL_ChanCode         = 9;//
      unsigned char   iBetaC               = 8; //
      unsigned char   iBetaD               = 15;//
      short         iDPCCH_PowerOffset      = -60;
      unsigned char   n_312               = FTM_WCDMA_BER_N312_s1;
      unsigned char   n_313               = FTM_WCDMA_BER_N313_s20;
      unsigned char   n_315               = FTM_WCDMA_BER_N315_s1;
      unsigned char   t_312               = 1;
      unsigned char   t_313               = 3;
//      unsigned short   iChannel            = 10562;//

//      int iStatus = QLIB_FTM_SEQ_DeactivateBatchMode( g_hQMSL );
//      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_SEQ_ActivateBatchMode (): Status: %d", iStatus); 

      iStatus_QLIbFunctCall =   QLIB_FTM_WCDMA_BER_EVENT_STATUS_CLEAR_REQ( g_hQMSL, &iClear_Register_Status);
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_WCDMA_BER_EVENT_STATUS_CLEAR_REQ(), Status=%d,%d", iStatus_QLIbFunctCall, iClear_Register_Status );

      iStatus_QLIbFunctCall =   QLIB_FTM_WCDMA_START_MODE_REQ ( g_hQMSL, &iStart_Mode_Status );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_WCDMA_START_MODE_REQ(), Status=%d,%d", iStatus_QLIbFunctCall, iStart_Mode_Status );

      iStatus_QLIbFunctCall =   QLIB_FTM_WCDMA_BER_EVENT_STATUS_REQ( g_hQMSL, FTM_WCDMA_BER_START_WCDMA_MODE_CNF, iTimeOut_MS, &iStart_Mode_Done_Status);
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_WCDMA_BER_EVENT_STATUS_REQ(), Status=%d,%d", iStatus_QLIbFunctCall, iStart_Mode_Done_Status );

      iStatus_QLIbFunctCall =   QLIB_FTM_WCDMA_ACQUIRE_REQ ( g_hQMSL, iAcqType, iAcqMode, iFreq, iScrCode, iPN_Pos, &iAcquire_Status );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_WCDMA_ACQUIRE_REQ(), Status=%d,%d", iStatus_QLIbFunctCall, iAcquire_Status );

      iStatus_QLIbFunctCall =   QLIB_FTM_WCDMA_BER_EVENT_STATUS_REQ ( g_hQMSL, FTM_WCDMA_BER_ACQ_CNF, iTimeOut_MS, &iAcquire_Done_Status );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_WCDMA_BER_EVENT_STATUS_REQ(), Status=%d,%d", iStatus_QLIbFunctCall, iAcquire_Done_Status );

      iStatus_QLIbFunctCall = QLIB_FTM_WCDMA_RMC_DCH_SETUP_REQ_V2B(   g_hQMSL,
                                                      iRMC_Type, 
                                                      iUL_MaxTxPwr, 
                                                      iUL_DPCH_ScrCode, 
                                                      iUL_PCA,
                                                      iUL_TPC_StepSize, 
                                                      iDL_PriCPICH_Scr_Code, 
                                                      iDL_ChanCode,
                                                      iBetaC, 
                                                      iBetaD,
                                                      iDPCCH_PowerOffset,
                                                      n_312,
                                                      n_313,
                                                      n_315,
                                                      t_312,
                                                      t_313,
                                                      iFreq, // iChannel,
                                                      &iHandOver_Status );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_WCDMA_RMC_DCH_SETUP_REQ_V2B(), Status=%d, %d", iStatus_QLIbFunctCall, iHandOver_Status );

      iStatus_QLIbFunctCall =     QLIB_FTM_WCDMA_BER_EVENT_STATUS_REQ ( g_hQMSL, FTM_WCDMA_PHYCHAN_ESTABLISHED_IND, iTimeOut_MS, &iHandover_Done_Status );
      MACRO_IF_FIRST_RUN_THEN printf("\nQLIB_FTM_WCDMA_BER_EVENT_STATUS_REQ(), Status=%d,%d", iStatus_QLIbFunctCall, iHandover_Done_Status );

      if ( 0 ) //   misc WCDMA FTM prototypes ...
      {
         unsigned char bOverride         = 0;
         unsigned short iUL_Pwr_dBm10   = 0;
         QLIB_FTM_WCDMA_BER_SET_UL_POWER( g_hQMSL, bOverride, iUL_Pwr_dBm10, &iStatus_US );

         unsigned char iRxChain   = 0;
         short   iRxAGC         = 0;
         short   iRxPowerdBm      = 0;
         QLIB_FTM_WCDMA_BER_RX_LEVEL_REQ( g_hQMSL, iRxChain, &iStatus_US, &iRxAGC, &iRxPowerdBm );
      }
   }
}
