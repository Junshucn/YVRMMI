/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_GPIB/QMSL_GPIB_MSVC7/QSEQ.h#5 $
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
#ifndef __QSEQ_FTM_SEQUENCER_H__
#define __QSEQ_FTM_SEQUENCER_H__

//--------------------------------------------------------------------------------------------------------------------------------------------
#define QSEQ_SEGMENTS_MAX 2000
//--------------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
   QSEQ_TECH_UNKNOWN,
   QSEQ_TECH_CDMA2000,
   QSEQ_TECH_WCDMA,
   QSEQ_TECH_GSM
}
QSEQ_Technology_Type; 

//--------------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
   QSEQ_ACTION_SUB_CONFIGURE,
   QSEQ_ACTION_SUB_EXECUTE,
   QSEQ_ACTION_SUB_COLLECT,
   QSEQ_ACTION_SUB_PROCESS,
   QSEQ_ACTION_SUB_REPORT
}
QSEQ_SubAction_Type; 

const QSEQ_SubAction_Type QSEQ_Action_Sub[] = {      QSEQ_ACTION_SUB_CONFIGURE,
                                       QSEQ_ACTION_SUB_EXECUTE,
                                       QSEQ_ACTION_SUB_COLLECT,
                                       QSEQ_ACTION_SUB_PROCESS,
                                       QSEQ_ACTION_SUB_REPORT   };

const int QSEQ_Action_Sub_Count = sizeof(QSEQ_Action_Sub)/sizeof(QSEQ_Action_Sub[0]);

//--------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------
//--- Measurement Related Data Structures ---
//-------------------------------------------
typedef struct
{
   bool   Enabled;
}
QSEQ_Measurement_Config_TYPE;

typedef struct
{
   QSEQ_Measurement_Config_TYPE   Config;   // automatically reset at beginning of each Segment
}
QSEQ_Measurement_TYPE;

const QSEQ_Measurement_TYPE QSEQ_Measurement_DEFAULT = {{0}};

//--------------------------------------------------------------------------------------------------------------------------------------------
//--- Sequence Related Data Structures ---
//---------------------------------------
typedef struct
{
   long   Index;
}
QSEQ_Sequence_TYPE;

const QSEQ_Sequence_TYPE QSEQ_Sequence_DEFAULT = {0}; //,0};
//--------------------------------------------------------------------------------------------------------------------------------------------
//--- Segment Related Data Structures ---
//---------------------------------------
typedef struct
{
//   bool   MeasurementSegment;
   long   Duration_ms;
//   long   MeasOffset_ms;
}
QSEQ_Segment_Config_TYPE;

typedef struct
{
   long   Index;
   long   Count;
}
QSEQ_Segment_Data_TYPE;

typedef struct
{
   QSEQ_Segment_Config_TYPE   Config;
   QSEQ_Segment_Data_TYPE      Data;
}
QSEQ_Segment_TYPE;

const QSEQ_Segment_TYPE QSEQ_Segment_DEFAULT = {{0},{0}};

//--------------------------------------------------------------------------------------------------------------------------------------------
//--- ENUM - Process Actions ---
//------------------------------
typedef enum
{
   QSEQ_ACT_SEQ_BEGIN_BEGIN,
   QSEQ_ACT_SEQ_BEGIN_ITEM,
   QSEQ_ACT_SEQ_BEGIN_END,
   
   QSEQ_ACT_SEQ_SEG_BEGIN,
   QSEQ_ACT_SEQ_SEG_ITEM,   // action to manipulate enums : QSEQ_SegmentItem_ENUM
   QSEQ_ACT_SEQ_SEG_END,
   
   QSEQ_ACT_SEQ_END_BEGIN,
   QSEQ_ACT_SEQ_END_ITEM,
   QSEQ_ACT_SEQ_END_END,
}
QSEQ_Action_ENUM;

//--------------------------------------------------------------------------------------------------------------------------------------------
//--- ENUM - direct combined mapping to of all "Devices" Attrributes and Measurements (Segment, Mobile, Equip) Elements ---
//-------------------------------------------------------------------------------------------------------------------------
typedef enum
{
   QSEQ_ATT_NULL,

   QSEQ_ATT_TECHNOLOGY,

   QLIB_ATT_EQ_ACT_DISABLE,   // No Calls to EQ


   //--------------------------------------------
   //--- Segment Attributes - "Device Common" --- (applies to both MS and BS (or "all devices") - is this a good abstraction ???)
   //--------------------------------------------
   QSEQ_ATT_C2KNS_SEG_CNFG_BAND,
   QSEQ_ATT_C2KNS_SEG_CNFG_CHANNEL,
   QSEQ_ATT_SEG_CNFG_DURATION_MS,

   //-----------------------------------
   //--- Segment Attributes - Mobile ---
   //-----------------------------------
   QLIB_ATT_MS_ACT_FTM_BATCH_MODE_ACTIVATE,

   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTRL_TYPE,   
   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTRL_CMD,
   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,
   QSEQ_ATT_C2KNS_MS_CNFG_TX_OPEN_LOOP,
   QSEQ_ATT_C2KNS_MS_CNFG_MOBILE_RX,

   QSEQ_ATT_C2KNS_MS_CNFG_RADIO_CONFIG,
   QSEQ_ATT_C2KNS_MS_CNFG_WALSH,
   QSEQ_ATT_C2KNS_MS_CNFG_FFPC_ENABLE,
   QSEQ_ATT_C2KNS_MS_CNFG_FRAME_OFFSET,
   QSEQ_ATT_C2KNS_MS_CNFG_FCH_RATE,
   QSEQ_ATT_C2KNS_MS_CNFG_RPC_ENABLE,
   QSEQ_ATT_C2KNS_MS_CNFG_NUM_PREAMBLES,
   QSEQ_ATT_C2KNS_MS_CNFG_TX_PATTERN,   

   QSEQ_ATT_C2KNS_MS_CNFG_DURATION_HARDHANDOFF_MS,
   QSEQ_ATT_C2KNS_MS_CNFG_DURATION_FCH_STATUS_GET_MS,
   QSEQ_ATT_C2KNS_MS_CNFG_DURATION_RX_LEVEL_GET_MS,

   QLIB_ATT_C2KNS_MS_ACT_CALL_ESTABLISH,
   QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_RESET,
   QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_GET,

   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,
   QSEQ_ATT_C2KNS_MS_MEAS_SER,
   QSEQ_ATT_C2KNS_MS_MEAS_FER,

   QSEQ_ATT_MS_ACT_CALL_RELEASE,

   //--------------------------------------
   //--- Segment Attributes - Equipment ---
   //--------------------------------------
   QSEQ_ATT_C2KNS_EQ_CNFG_PORT_ANA,
   QSEQ_ATT_C2KNS_EQ_CNFG_PORT_GEN,
   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,
   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,
   QSEQ_ATT_C2KNS_EQ_CNFG_TRIGGER,
   QSEQ_ATT_C2KNS_EQ_CNFG_CABLE_LOSS,
   QSEQ_ATT_C2KNS_EQ_CNFG_WAVEFORM_FILE,

   QLIB_ATT_C2KNS_EQ_ACT_ARB_WAVE_GEN_ON,
   QLIB_ATT_EQ_ACT_RF_GEN_OFF,

   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,
   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,
   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,
   QSEQ_ATT_C2KNS_EQ_MEAS_TIM_ERR,
   QSEQ_ATT_C2KNS_EQ_MEAS_FREQ_ERR,
   QSEQ_ATT_C2KNS_EQ_MEAS_CARR_FT,
   QSEQ_ATT_C2KNS_EQ_MEAS_PH_ERR,
   QSEQ_ATT_C2KNS_EQ_MEAS_MAG_ERR,
   QSEQ_ATT_C2KNS_EQ_MEAS_EVM
}
QSEQ_SegmentItem_ENUM;
 
//--------------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
   QSEQ_Action_ENUM      Action;
   QSEQ_SegmentItem_ENUM   SegmentItem;
   char               *String_Input ;
   char               *String_Output;
}
QSEQ_Sequence_type;

//--------------------------------------------------------------------------------------------------------------------------------------------
//--- Function Prototypes ---
//---------------------------
char* QSEQ_Action_ENUM_to_String      ( const QSEQ_Action_ENUM      Action      );

char* QSEQ_SubAction_ENUM_to_String      ( const QSEQ_SubAction_Type      SubAction   );

char* QSEQ_SegmentItem_ENUM_to_String   ( const QSEQ_SegmentItem_ENUM   SegmentItem );

void QSEQ_Sequencer_Prototype ( int iSequence_ID, HANDLE g_hQMSL, int g_hInst );

#endif
