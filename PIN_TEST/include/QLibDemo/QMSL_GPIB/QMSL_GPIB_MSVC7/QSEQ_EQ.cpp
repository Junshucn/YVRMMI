/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_GPIB/QMSL_GPIB_MSVC7/QSEQ_EQ.cpp#5 $
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

#include "QSEQ_EQ.h"

//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
#define CONST_64    64
#define CONST_128   128
#define CONST_256   256
#define CONST_512   512
#define CONST_1024 1024
#define CONST_2048 2048

#define CONVERSION_MSEC_PER_CDMA_FRAME 1.25

//--------------------------------------------------------------------------------------------------------------------------------------------
static bool First_Run   = true;
static bool EQ_Disabled = false;

#define MACRO_IF_EQ_DISABLED_RETURN if ( EQ_Disabled ) return;

#define MACRO_IF_FIRST_RUN if ( First_Run )

//--------------------------------------------------------------------------------------------------------------------------------------------
//--- High-Level Data Structures ---
//----------------------------------
typedef enum
{
   QSEQ_EQ_VENDOR_UNKNOWN,
   QSEQ_EQ_VENDOR_R_S_CMW,      // R&S CMW-500
   QSEQ_EQ_VENDOR_AGI_EXT      // Agilent EXT
}
QSEQ_EquipmentVendor_TYPE;

static QSEQ_EquipmentVendor_TYPE QSEQ_EquipmentVendor = QSEQ_EQ_VENDOR_R_S_CMW;

typedef enum
{
   QSEQ_TECH_BAND_UNKNOWN,
   QSEQ_TECH_BAND_C2K_BC0,
   QSEQ_TECH_BAND_C2K_BC1
}
QSEQ_Technology_Band_TYPE;

//--------------------------------------------------------------------------------------------------------------------------------------------
void QSEQ_UTIL_BandChannel_to_Freq( const QSEQ_Technology_Band_TYPE Band, const unsigned int iChannel, double *dl_MHz, double *ul_MHz );

//--------------------------------------------------------------------------------------------------------------------------------------------
//--- Equipment Related Data Structures ---
//-----------------------------------------
typedef struct
{
   QSEQ_Technology_Band_TYPE   Band;
   long   Channel;
   int      Port_RF_Analyer;
   int      Port_RF_Generator;
   double   RF_Level_Generator;
   double   RF_Level_Analyzer;
   bool   Trigger;
   double   Cable_Loss;
   char   Waveform_File[1024];
}
QSEQ_Equip_Config_C2K_TYPE;

typedef struct
{
   long   ArbGen_On;
   long   RfGen_Off;   // these should be BOOLEAN
}
QSEQ_Equip_Action_C2K_TYPE;


typedef struct
{
   QSEQ_Measurement_TYPE   TxPower;
   QSEQ_Measurement_TYPE   ACPR;
   QSEQ_Measurement_TYPE   Rho;
   QSEQ_Measurement_TYPE   TimingError;
   QSEQ_Measurement_TYPE   FreqError;
   QSEQ_Measurement_TYPE   CarrierFeedthrough;
   QSEQ_Measurement_TYPE   PhaseError;
   QSEQ_Measurement_TYPE   MagnitudeError;
   QSEQ_Measurement_TYPE   EVM;
}
QSEQ_Equip_Meas_C2K_TYPE;

typedef struct
{
   QSEQ_Equip_Config_C2K_TYPE   Config;
   QSEQ_Equip_Action_C2K_TYPE  Action;
   QSEQ_Equip_Meas_C2K_TYPE   Meas;
}
QSEQ_Equip_C2KNS_TYPE;

typedef struct
{
   QSEQ_Technology_Type   Technology;
   QSEQ_Sequence_TYPE      Sequence;
   QSEQ_Segment_TYPE      Segment;
   QSEQ_Equip_C2KNS_TYPE   C2KNS;
}
QSEQ_Equip_State_TYPE;

static const QSEQ_Equip_State_TYPE QSEQ_Equip_State_DEFAULT =
{
   QSEQ_TECH_UNKNOWN,
   QSEQ_Sequence_DEFAULT,
   QSEQ_Segment_DEFAULT,
   { // C2KNS
      {
         QSEQ_TECH_BAND_C2K_BC0,
         387,
         0,      // Port_RF_Analyer;         QSEQ_ATT_C2KNS_EQ_CNFG_PORT_ANA
         0,      // Port_RF_Generator;      QSEQ_ATT_C2KNS_EQ_CNFG_PORT_GEN
         -104.0,   // RF_Level_Generator;      QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN
         30.0,   // RF_Level_Analyzer;      QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA
         1,      // Trigger;               QSEQ_ATT_C2KNS_EQ_CNFG_TRIGGER
         1.0,      // Cable_Loss
      },
      {
         0,
      },
      { //QSEQ_SeqState_EquipMeas_TYPE
         QSEQ_Measurement_DEFAULT,   // Power;               QSEQ_ATT_C2KNS_EQ_MEAS_PWR
         QSEQ_Measurement_DEFAULT,   // ACPR;               QSEQ_ATT_C2KNS_EQ_MEAS_ACPR      // FIX  ACP offset Frequencies ****
         QSEQ_Measurement_DEFAULT,   // Rho;                  QSEQ_ATT_C2KNS_EQ_MEAS_RHO
         QSEQ_Measurement_DEFAULT,   // TimingError;            QSEQ_ATT_C2KNS_EQ_MEAS_TIM_ERR
         QSEQ_Measurement_DEFAULT,   // FreqError;            QSEQ_ATT_C2KNS_EQ_MEAS_FREQ_ERR
         QSEQ_Measurement_DEFAULT,   // CarrierFeedthrough;      QSEQ_ATT_C2KNS_EQ_MEAS_CARR_FT
         QSEQ_Measurement_DEFAULT,   // PhaseError;            QSEQ_ATT_C2KNS_EQ_MEAS_PH_ERR
         QSEQ_Measurement_DEFAULT,   // MagnitudeError;         QSEQ_ATT_C2KNS_EQ_MEAS_MAG_ERR
         QSEQ_Measurement_DEFAULT   // EVM;                  QSEQ_ATT_C2KNS_EQ_MEAS_EVM
      }
   }
};

//-------------------------------------------------------------------------------------------------------------
static const long seg_bias = -1;

static QSEQ_Equip_State_TYPE   QSEQ_Equip_State_Current  = QSEQ_Equip_State_DEFAULT;

static QSEQ_Sequence_type      *Sequence_LocalPointer;
static int                  nSeq = -1;

static QSEQ_Equip_State_TYPE   QSEQ_Equip_SegData      [QSEQ_SEGMENTS_MAX] = {};

static double               GPRF_LIST_DownlinkFreq  [QSEQ_SEGMENTS_MAX] = {};   // FIX OPTIMIZE *** REMOVE ??? ***  Leave in for now - handly for diagnostics
static double               GPRF_LIST_DownlinkPower [QSEQ_SEGMENTS_MAX] = {};   // FIX OPTIMIZE *** REMOVE ??? ***  Leave in for now - handly for diagnostics

//-------------------------------------------------------------------------------------------------------------

typedef struct
{
   double Integrity;
   double TxPower;
   double Rho;
   double TimingError;
   double FreqError;
   double CarrierFeedthrough;
   double PhaseError;
   double MagnitudeError;
   double EVM;
}
QSEQ_SEG_C2KNS_MeasCache_Mod_TYPE;

typedef struct
{
   double Integ;
   double m10;
   double m09;
   double m08;
   double m07;
   double m06;
   double m05;
   double m04;
   double m03;
   double m02;
   double m01;

   double p01;
   double p02;
   double p03;
   double p04;
   double p05;
   double p06;
   double p07;
   double p08;
   double p09;
   double p10;
}
QSEQ_SEG_C2KNS_MeasCache_ACP_TYPE;

typedef struct
{
   QSEQ_SEG_C2KNS_MeasCache_Mod_TYPE   Mod;
   QSEQ_SEG_C2KNS_MeasCache_ACP_TYPE   ACP;
}
QSEQ_SEG_C2KNS_MeasCache_TYPE;

static QSEQ_SEG_C2KNS_MeasCache_TYPE QSEQ_SEG_C2KNS_MeasCache         = {};
static QSEQ_SEG_C2KNS_MeasCache_TYPE QSEQ_SEG_C2KNS_MeasCache_DEFAULT   = {};

//-------------------------------------------------------------------------------------------------------------
//***** Special Logic *****
//-------------------------
// QSEQ_EQ_C2KNS_ACP_Enabled_TYPE is used to determine which ACPR measurement offsets are enabled for 
// ALL SEGMENTS of a sequence.  The CMW500 is not capable specifying ACPR offsets per SEGMENT. At each 
// SEGMENT offsets are enabled but never disabled - to collect all of the offset required by the sequence.
// This parameter (QSEQ_EQ_C2KNS_ACPR_All_Segments) is reset to all FALSE upon SEQUENCE BEGIN and the used to
// config the CMW at SEQUENCE END.
typedef struct
{
   bool   Offset_01;
   bool   Offset_02;
   bool   Offset_03;
   bool   Offset_04;
   bool   Offset_05;
   bool   Offset_06;
   bool   Offset_07;
   bool   Offset_08;
   bool   Offset_09;
   bool   Offset_10;
}
QSEQ_EQ_C2KNS_ACP_Enabled_TYPE;

static const QSEQ_EQ_C2KNS_ACP_Enabled_TYPE QSEQ_EQ_C2KNS_ACPR_All_Segments_DEFAULT =
{
   false,
   false,
   false,
   false,
   false,
   false,
   false,
   false,
   false,
   false
};

static QSEQ_EQ_C2KNS_ACP_Enabled_TYPE QSEQ_EQ_C2KNS_ACPR_All_Segments = QSEQ_EQ_C2KNS_ACPR_All_Segments_DEFAULT;

//-------------------------------------------------------------------------------------------------------------
void QSEQ_WriteToEquip      ( int g_hInst, char *sStr);
void QSEQ_ReadFromEquipment (int g_hInst, char *sStr, long iSizeOfStr);

void QSEQ_ParseStringToDouble   (char* string, double *doubleArray, int& sizeOfArray);
void QSEQ_UTIL_ParseStringToLong(char* string,   long *longArray,   int& sizeOfArray);

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_Begin_Begin ( int g_hInst, const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Equip_State_TYPE *Data )
{
   Data->Segment.Data.Index = 0;
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
void QSEQ_EQ_Sequence_Begin_Item_Mgmt (   int                  index_master,
                              QSEQ_SubAction_Type      sub_action,
                              QSEQ_Equip_State_TYPE   *Data )
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
                        Data->Technology = QSEQ_TECH_UNKNOWN;   // FIX - ERROR MSG - ADD
                        printf("\n\nERROR: UNKNOWN QSEQ_ATT_TECHNOLOGY [%s]", SeqItem->String_Input );
                     }
                  }
                  break;
               case QLIB_ATT_EQ_ACT_DISABLE:
                  {
                     EQ_Disabled = true;
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
                     
//-------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_Begin_End ( int g_hInst, const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Equip_State_TYPE *Data )
{
   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     QSEQ_EQ_C2KNS_ACPR_All_Segments = QSEQ_EQ_C2KNS_ACPR_All_Segments_DEFAULT;   // SPECIAL BOOKKEEPING Parameter - keep ***HERE*** QSEQ_EQ_Sequence_Begin
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
void QSEQ_EQ_Sequence_Segment_Begin_CONFIG_C2K  ( int g_hInst, const int index_master, const int segment_id_SEGM, QSEQ_Equip_State_TYPE *Data );
void QSEQ_EQ_Sequence_Segment_Begin_COLLECT_C2K ( int g_hInst, const int index_master, const int segment_id_SEGM, QSEQ_Equip_State_TYPE *Data );

//-------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_Segment_Begin ( int g_hInst, const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Equip_State_TYPE *Data )
{
   MACRO_IF_EQ_DISABLED_RETURN

   const int segment_id = ++Data->Segment.Data.Index; // 1-based
                       ++Data->Segment.Data.Count;

   const long segment_id_SEGM = segment_id + seg_bias;      // *** FIX *** REVISIT *** application of "bias" ok for all scenarios???

   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {   
            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     QSEQ_EQ_Sequence_Segment_Begin_CONFIG_C2K ( g_hInst, index_master, segment_id_SEGM, Data );
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
                     QSEQ_EQ_Sequence_Segment_Begin_COLLECT_C2K ( g_hInst, index_master, segment_id_SEGM, Data );
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
         break;
   }
}

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_Segment_Item_Mgmt_CONFIG_C2K  ( const int index_master, QSEQ_Sequence_type *SeqItem, QSEQ_Equip_State_TYPE *Data );
void QSEQ_EQ_Sequence_Segment_Item_Mgmt_COLLECT_C2K ( const int index_master, QSEQ_Sequence_type *SeqItem, QSEQ_Equip_State_TYPE *Data );

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_Segment_Item_Mgmt ( const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Equip_State_TYPE *Data )
{
   MACRO_IF_EQ_DISABLED_RETURN

   QSEQ_Sequence_type    *SeqItem = &Sequence_LocalPointer[index_master];

   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            switch ( SeqItem->SegmentItem )
            {
               case QSEQ_ATT_SEG_CNFG_DURATION_MS:
                  Data->Segment.Config.Duration_ms = atol (SeqItem->String_Input);
                  break;
               default:
                  break;
            }

            if ( First_Run )
            {
               switch ( Data->Technology ) 
               {
                  case QSEQ_TECH_CDMA2000:
                     {
                        QSEQ_EQ_Sequence_Segment_Item_Mgmt_CONFIG_C2K ( index_master, SeqItem, Data );
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
                     QSEQ_EQ_Sequence_Segment_Item_Mgmt_COLLECT_C2K ( index_master, SeqItem, Data );
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
            break;
         }
      case QSEQ_ACTION_SUB_PROCESS:
         break;
      case QSEQ_ACTION_SUB_REPORT:
         break;
      default:
         return;
   }
}

//-------------------------------------------------------------------------------------------------------------
void EQ_ArbGen_Activate_C2K                ( int g_hInst, const QSEQ_Technology_Band_TYPE Band, const long Channel, const double RF_Level_Generator, const double Cable_Loss, char *Waveform_File );

void QSEQ_EQ_Sequence_Segment_End_CONFIG_C2K ( int g_hInst, const int index_master, const long segment_id_SEGM, QSEQ_Equip_State_TYPE *Data );

//-------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_Segment_End           ( int g_hInst, const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Equip_State_TYPE *Data )
{
   MACRO_IF_EQ_DISABLED_RETURN

   const int segment_id = Data->Segment.Data.Index;

   const long segment_id_SEGM = segment_id + seg_bias;

   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {      
            if ( First_Run )
            {
               QSEQ_Equip_SegData[segment_id] = *Data;
            }

            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     QSEQ_EQ_Sequence_Segment_End_CONFIG_C2K ( g_hInst, index_master, segment_id_SEGM, Data );
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
void QSEQ_EQ_Sequence_End_Begin ( int g_hInst, const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Equip_State_TYPE *Data )
{
   MACRO_IF_EQ_DISABLED_RETURN

   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     Data->C2KNS.Action.RfGen_Off = 0;
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
void QSEQ_EQ_Sequence_End_Item_Mgmt (   int                  index_master,
                              QSEQ_SubAction_Type      sub_action,
                              QSEQ_Equip_State_TYPE   *Data )
{
   MACRO_IF_EQ_DISABLED_RETURN

   QSEQ_Sequence_type   *SeqItem = &Sequence_LocalPointer[index_master];

   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     switch ( SeqItem->SegmentItem )
                     {
                        case QLIB_ATT_EQ_ACT_RF_GEN_OFF:
                           Data->C2KNS.Action.RfGen_Off = 1;
                           break;
                        default:
                           break;
                     }
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
void QSEQ_EQ_Sequence_End_End_CONFIG_C2K  ( int g_hInst, const int index_master, QSEQ_Equip_State_TYPE *Data );
void QSEQ_EQ_Sequence_End_End_EXECUTE_C2K ( int g_hInst, const int index_master, QSEQ_Equip_State_TYPE *Data );
void QSEQ_EQ_Sequence_End_End_COLLECT_C2K ( int g_hInst, const int index_master, QSEQ_Equip_State_TYPE *Data );

//-------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_End_End           ( int g_hInst, const int index_master, const QSEQ_SubAction_Type sub_action, QSEQ_Equip_State_TYPE *Data )
{
   MACRO_IF_EQ_DISABLED_RETURN

   switch (sub_action)
   {
      case QSEQ_ACTION_SUB_CONFIGURE:
         {
            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     QSEQ_EQ_Sequence_End_End_CONFIG_C2K ( g_hInst, index_master, Data );
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
         {
            MACRO_IF_FIRST_RUN printf("\n\nQSEQ_EQ_Sequence_End_End() [START]: EXECUTE"); 

            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     QSEQ_EQ_Sequence_End_End_EXECUTE_C2K ( g_hInst, index_master, Data );
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

//            First_Run = false;

            MACRO_IF_FIRST_RUN printf("\nQSEQ_EQ_Sequence_End_End() [STOP]: EXECUTE"); 
         }
         break;   
      case QSEQ_ACTION_SUB_COLLECT:
         {   
            switch ( Data->Technology ) 
            {
               case QSEQ_TECH_CDMA2000:
                  {
                     QSEQ_EQ_Sequence_End_End_COLLECT_C2K ( g_hInst, index_master, Data );
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
         break;
   }
}

//--------------------------------------------------------------------------------------------------------------------------
unsigned char QSEQ_Sequence_EQ_Set ( long nSegItem, QSEQ_Sequence_type *Sequence )
{
   Sequence_LocalPointer = Sequence;

   nSeq = nSegItem;
   
   return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
unsigned char QSEQ_Sequence_EQ (   const int               g_hInst, 
                           const QSEQ_SubAction_Type   sub_action )
{
   unsigned long t_start = GetTickCount();

   for ( int Index_SequenceMaster = 0; Index_SequenceMaster < nSeq; Index_SequenceMaster++ )
   {
      switch ( Sequence_LocalPointer[Index_SequenceMaster].Action)
      {
         case QSEQ_ACT_SEQ_BEGIN_BEGIN:   QSEQ_EQ_Sequence_Begin_Begin   ( g_hInst, Index_SequenceMaster, sub_action, &QSEQ_Equip_State_Current );   break;   //   CONFIG
         case QSEQ_ACT_SEQ_BEGIN_ITEM:   QSEQ_EQ_Sequence_Begin_Item_Mgmt (         Index_SequenceMaster, sub_action, &QSEQ_Equip_State_Current );   break;   //   CONFIG
         case QSEQ_ACT_SEQ_BEGIN_END:   QSEQ_EQ_Sequence_Begin_End      ( g_hInst, Index_SequenceMaster, sub_action, &QSEQ_Equip_State_Current );   break;   //   CONFIG

         case QSEQ_ACT_SEQ_SEG_BEGIN:   QSEQ_EQ_Sequence_Segment_Begin   ( g_hInst, Index_SequenceMaster, sub_action, &QSEQ_Equip_State_Current );   break;   //   CONFIG   COLLECT
         case QSEQ_ACT_SEQ_SEG_ITEM:      QSEQ_EQ_Sequence_Segment_Item_Mgmt (       Index_SequenceMaster, sub_action, &QSEQ_Equip_State_Current );   break;   //   CONFIG   COLLECT
         case QSEQ_ACT_SEQ_SEG_END:      QSEQ_EQ_Sequence_Segment_End   ( g_hInst, Index_SequenceMaster, sub_action, &QSEQ_Equip_State_Current );   break;   //   CONFIG

         case QSEQ_ACT_SEQ_END_BEGIN:   QSEQ_EQ_Sequence_End_Begin      ( g_hInst, Index_SequenceMaster, sub_action, &QSEQ_Equip_State_Current );   break;
         case QSEQ_ACT_SEQ_END_ITEM:      QSEQ_EQ_Sequence_End_Item_Mgmt  (         Index_SequenceMaster, sub_action, &QSEQ_Equip_State_Current );   break;   //   CONFIG
         case QSEQ_ACT_SEQ_END_END:      QSEQ_EQ_Sequence_End_End      ( g_hInst, Index_SequenceMaster, sub_action, &QSEQ_Equip_State_Current );   break;   //   CONFIG         EXECUTE
         default:                                                                                                   break;
      }
   }

   unsigned long t_stop  = GetTickCount();

   printf("\n\nEQ [%s] Elapsed Time = %d msec ******************************** Timing ***\n", QSEQ_SubAction_ENUM_to_String(sub_action), t_stop - t_start );

   return 0;
}




















































//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_Segment_Begin_CONFIG_C2K  ( int g_hInst, const int index_master, const int segment_id_SEGM, QSEQ_Equip_State_TYPE *Data )
{
   //-------------------------------------------------
   //-   reset "action" and "measurment" trigger flags -
   //-------------------------------------------------
   Data->C2KNS.Action.ArbGen_On = 0;

   Data->C2KNS.Meas.TxPower.Config.Enabled            = 0;   // group better - one #define for DEFAULTing to all "off" ?
   Data->C2KNS.Meas.ACPR.Config.Enabled            = 0;   // *** OPTION *** reset/init once-globally, then individually upon use ?
   Data->C2KNS.Meas.Rho.Config.Enabled               = 0;
   Data->C2KNS.Meas.TimingError.Config.Enabled         = 0;
   Data->C2KNS.Meas.FreqError.Config.Enabled         = 0;
   Data->C2KNS.Meas.CarrierFeedthrough.Config.Enabled   = 0;
   Data->C2KNS.Meas.PhaseError.Config.Enabled         = 0;
   Data->C2KNS.Meas.MagnitudeError.Config.Enabled      = 0;
   Data->C2KNS.Meas.EVM.Config.Enabled               = 0;
}

//-------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_Segment_Begin_COLLECT_C2K ( int g_hInst, const int index_master, const int segment_id_SEGM, QSEQ_Equip_State_TYPE *Data )
{
   QSEQ_SEG_C2KNS_MeasCache = QSEQ_SEG_C2KNS_MeasCache_DEFAULT;   // FIX DEFAULT should be "NaN" vs just "0.0"

   QSEQ_Equip_State_TYPE Data_Recorded = QSEQ_Equip_SegData [Data->Segment.Data.Index];

   char cmd_64   [CONST_64]   = "";
   char buf_1024 [CONST_1024] = "";

   if (   Data_Recorded.C2KNS.Meas.TxPower.Config.Enabled            ||
         Data_Recorded.C2KNS.Meas.Rho.Config.Enabled               ||
         Data_Recorded.C2KNS.Meas.TimingError.Config.Enabled         ||
         Data_Recorded.C2KNS.Meas.FreqError.Config.Enabled         ||
         Data_Recorded.C2KNS.Meas.CarrierFeedthrough.Config.Enabled   ||
         Data_Recorded.C2KNS.Meas.PhaseError.Config.Enabled         ||
         Data_Recorded.C2KNS.Meas.MagnitudeError.Config.Enabled      ||
         Data_Recorded.C2KNS.Meas.EVM.Config.Enabled                  )
   {
      double modResult_18 [18] = {};
      int modResult_size = 18;

      sprintf( cmd_64, ":FETC:CDMA:MEAS:MEV:LIST:SEGM%d:MOD:AVER?;", segment_id_SEGM );
      QSEQ_WriteToEquip     ( g_hInst, cmd_64 );
      QSEQ_ReadFromEquipment( g_hInst, buf_1024, CONST_1024 );
      QSEQ_ParseStringToDouble( buf_1024, modResult_18, modResult_size);

      QSEQ_SEG_C2KNS_MeasCache.Mod.Integrity      = modResult_18[0];      // need Sequence_index - for multi-sequence scenario (?)
      QSEQ_SEG_C2KNS_MeasCache.Mod.EVM         = modResult_18[3]; // RMS
      QSEQ_SEG_C2KNS_MeasCache.Mod.MagnitudeError   = modResult_18[4];
      QSEQ_SEG_C2KNS_MeasCache.Mod.PhaseError      = modResult_18[6];
      QSEQ_SEG_C2KNS_MeasCache.Mod.FreqError      = modResult_18[10];
      QSEQ_SEG_C2KNS_MeasCache.Mod.TxPower      = modResult_18[12];
      QSEQ_SEG_C2KNS_MeasCache.Mod.Rho         = modResult_18[14];   // rho

      QSEQ_SEG_C2KNS_MeasCache.Mod.TimingError      = -999; // modResult_18[];
      QSEQ_SEG_C2KNS_MeasCache.Mod.CarrierFeedthrough   = -999; //modResult_18[];
   }

   if ( Data_Recorded.C2KNS.Meas.ACPR.Config.Enabled ) // ok to use - Segment level...
   {
      double acprResult [30] = {};
      int acprResultSz = 30;
      
      sprintf( cmd_64, ":FETC:CDMA:MEAS:MEV:LIST:SEGM%d:SPEC:AVER?;", segment_id_SEGM );
      QSEQ_WriteToEquip     ( g_hInst, cmd_64 );
      QSEQ_ReadFromEquipment( g_hInst, buf_1024, CONST_1024 );
      QSEQ_ParseStringToDouble( buf_1024, acprResult, acprResultSz);

      QSEQ_SEG_C2KNS_MeasCache.ACP.Integ   = acprResult[0];
      
      QSEQ_SEG_C2KNS_MeasCache.ACP.m10   = acprResult[2];
      QSEQ_SEG_C2KNS_MeasCache.ACP.m09   = acprResult[3];
      QSEQ_SEG_C2KNS_MeasCache.ACP.m08   = acprResult[4];
      QSEQ_SEG_C2KNS_MeasCache.ACP.m07   = acprResult[5];
      QSEQ_SEG_C2KNS_MeasCache.ACP.m06   = acprResult[6];
      QSEQ_SEG_C2KNS_MeasCache.ACP.m05   = acprResult[7];
      QSEQ_SEG_C2KNS_MeasCache.ACP.m04   = acprResult[8];
      QSEQ_SEG_C2KNS_MeasCache.ACP.m03   = acprResult[9];
      QSEQ_SEG_C2KNS_MeasCache.ACP.m02   = acprResult[10];
      QSEQ_SEG_C2KNS_MeasCache.ACP.m01   = acprResult[11];

      QSEQ_SEG_C2KNS_MeasCache.ACP.p01   = acprResult[13];
      QSEQ_SEG_C2KNS_MeasCache.ACP.p02   = acprResult[14];
      QSEQ_SEG_C2KNS_MeasCache.ACP.p03   = acprResult[15];
      QSEQ_SEG_C2KNS_MeasCache.ACP.p04   = acprResult[16];
      QSEQ_SEG_C2KNS_MeasCache.ACP.p05   = acprResult[17];
      QSEQ_SEG_C2KNS_MeasCache.ACP.p06   = acprResult[18];
      QSEQ_SEG_C2KNS_MeasCache.ACP.p07   = acprResult[19];
      QSEQ_SEG_C2KNS_MeasCache.ACP.p08   = acprResult[20];
      QSEQ_SEG_C2KNS_MeasCache.ACP.p09   = acprResult[21];
      QSEQ_SEG_C2KNS_MeasCache.ACP.p10   = acprResult[22];
   }
}

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_Segment_Item_Mgmt_CONFIG_C2K  ( const int index_master, QSEQ_Sequence_type *SeqItem, QSEQ_Equip_State_TYPE *Data )
{
   switch ( SeqItem->SegmentItem )
   {
      case QSEQ_ATT_C2KNS_SEG_CNFG_BAND:                  
         {
            if      ( 0 == strcmp ( SeqItem->String_Input, "C2K_BandClass_0" ) )   {   Data->C2KNS.Config.Band = QSEQ_TECH_BAND_C2K_BC0;   }
            else if ( 0 == strcmp ( SeqItem->String_Input, "C2K_BandClass_1" ) )   {   Data->C2KNS.Config.Band = QSEQ_TECH_BAND_C2K_BC1;   }
            else
            {
               Data->C2KNS.Config.Band = QSEQ_TECH_BAND_UNKNOWN;
               printf("\n\nERROR: UNKNOWN QSEQ_ATT_C2KNS_SEG_CNFG_BAND [%s]", SeqItem->String_Input );
            }
         }
         break;
      case QSEQ_ATT_C2KNS_SEG_CNFG_CHANNEL:
         Data->C2KNS.Config.Channel = atol (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_EQ_CNFG_PORT_ANA:
         Data->C2KNS.Config.Port_RF_Analyer = atol (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_EQ_CNFG_PORT_GEN:
         Data->C2KNS.Config.Port_RF_Generator = atol (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN:
         Data->C2KNS.Config.RF_Level_Generator = atof (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA:
         Data->C2KNS.Config.RF_Level_Analyzer = atof (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_EQ_CNFG_TRIGGER:
         Data->C2KNS.Config.Trigger = ( atol (SeqItem->String_Input) != 0 );
         break;
      case QSEQ_ATT_C2KNS_EQ_CNFG_CABLE_LOSS:
         Data->C2KNS.Config.Cable_Loss = atof (SeqItem->String_Input);
         break;
      case QSEQ_ATT_C2KNS_EQ_CNFG_WAVEFORM_FILE:
         strcpy ( Data->C2KNS.Config.Waveform_File, SeqItem->String_Input );
         break;
      
      case QLIB_ATT_C2KNS_EQ_ACT_ARB_WAVE_GEN_ON:
         Data->C2KNS.Action.ArbGen_On = 1;
         break;

      case QSEQ_ATT_C2KNS_EQ_MEAS_PWR:
         Data->C2KNS.Meas.TxPower.Config.Enabled = 1;
         break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_ACPR:      // expecting string: "" to "1,2,3,4,5,6,7,8,9,10"  (note: "" same as "1")
         {
            long longArray[10]      = {};
            int    nPts            = 10;
            char buf_64[CONST_64]   =  "";
            
            strcpy ( buf_64, SeqItem->String_Input );

            QSEQ_UTIL_ParseStringToLong( buf_64, longArray, nPts );

            Data->C2KNS.Meas.ACPR.Config.Enabled = 1;

            if ( nPts > 0 )
            {
               bool err = false;

               for ( int i=0; i<nPts; i++ )
               {
                  switch ( longArray[i] ) 
                  {
                     case 1: QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_01 = true;   break;
                     case 2:   QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_02 = true;   break;
                     case 3:   QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_03 = true;   break;
                     case 4: QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_04 = true;   break;
                     case 5: QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_05 = true;   break;
                     case 6: QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_06 = true;   break;
                     case 7: QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_07 = true;   break;
                     case 8: QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_08 = true;   break;
                     case 9: QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_09 = true;   break;
                     case 10:QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_10 = true;   break;
                     default:            err = true;                     break;
                  }
               }

               if ( err ) printf("\n\nERROR: UNKNOWN QSEQ_ATT_C2KNS_EQ_MEAS_ACPR [%s]", SeqItem->String_Input );
            }
            else
            {
               QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_01 = true;
            }
         }
         break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_RHO:
         Data->C2KNS.Meas.Rho.Config.Enabled = 1;
         break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_TIM_ERR:
         Data->C2KNS.Meas.TimingError.Config.Enabled = 1;
         break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_FREQ_ERR:
         Data->C2KNS.Meas.FreqError.Config.Enabled = 1;
         break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_CARR_FT:
         Data->C2KNS.Meas.CarrierFeedthrough.Config.Enabled = 1;
         break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_PH_ERR:
         Data->C2KNS.Meas.PhaseError.Config.Enabled = 1;
         break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_MAG_ERR:
         Data->C2KNS.Meas.MagnitudeError.Config.Enabled = 1;
         break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_EVM:
         Data->C2KNS.Meas.EVM.Config.Enabled = 1;
         break;
      default:
         break;
   }
}

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_Segment_Item_Mgmt_COLLECT_C2K ( const int index_master, QSEQ_Sequence_type *SeqItem, QSEQ_Equip_State_TYPE *Data )
{
   switch ( SeqItem->SegmentItem )
   {
      case QSEQ_ATT_C2KNS_EQ_MEAS_PWR:
            sprintf ( SeqItem->String_Output, "%d,%0.2f", (long)QSEQ_SEG_C2KNS_MeasCache.Mod.Integrity, QSEQ_SEG_C2KNS_MeasCache.Mod.TxPower );
            break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_ACPR:
         {
            long longArray[10] = {};
            int     nPts    = 10;
            char buffer_256[CONST_256]  = "";

            strcpy ( buffer_256, SeqItem->String_Input );
            
            QSEQ_UTIL_ParseStringToLong( buffer_256, longArray, nPts );

            char buf_256[CONST_256] = "";
            char rpt_256[CONST_256] = "";

            if ( strlen ( buffer_256) > 0 )
            {
               for ( int i=0; i<10; i++ )
               {
                  switch ( longArray[i] ) // FIX - presume/REQUIRE(?) order (ascenting??)  - revisit !!!!!!!!!
                  {
                     case  1: sprintf ( rpt_256, ",%0.2f%s,%0.2f", QSEQ_SEG_C2KNS_MeasCache.ACP.m01, buf_256, QSEQ_SEG_C2KNS_MeasCache.ACP.p01 );   strcpy ( buf_256, rpt_256 ); break;
                     case  2: sprintf ( rpt_256, ",%0.2f%s,%0.2f", QSEQ_SEG_C2KNS_MeasCache.ACP.m02, buf_256, QSEQ_SEG_C2KNS_MeasCache.ACP.p02 );   strcpy ( buf_256, rpt_256 ); break;
                     case  3: sprintf ( rpt_256, ",%0.2f%s,%0.2f", QSEQ_SEG_C2KNS_MeasCache.ACP.m03, buf_256, QSEQ_SEG_C2KNS_MeasCache.ACP.p03 );   strcpy ( buf_256, rpt_256 ); break;
                     case  4: sprintf ( rpt_256, ",%0.2f%s,%0.2f", QSEQ_SEG_C2KNS_MeasCache.ACP.m04, buf_256, QSEQ_SEG_C2KNS_MeasCache.ACP.p04 );   strcpy ( buf_256, rpt_256 ); break;
                     case  5: sprintf ( rpt_256, ",%0.2f%s,%0.2f", QSEQ_SEG_C2KNS_MeasCache.ACP.m05, buf_256, QSEQ_SEG_C2KNS_MeasCache.ACP.p05 );   strcpy ( buf_256, rpt_256 ); break;
                     case  6: sprintf ( rpt_256, ",%0.2f%s,%0.2f", QSEQ_SEG_C2KNS_MeasCache.ACP.m06, buf_256, QSEQ_SEG_C2KNS_MeasCache.ACP.p06 );   strcpy ( buf_256, rpt_256 ); break;
                     case  7: sprintf ( rpt_256, ",%0.2f%s,%0.2f", QSEQ_SEG_C2KNS_MeasCache.ACP.m07, buf_256, QSEQ_SEG_C2KNS_MeasCache.ACP.p07 );   strcpy ( buf_256, rpt_256 ); break;
                     case  8: sprintf ( rpt_256, ",%0.2f%s,%0.2f", QSEQ_SEG_C2KNS_MeasCache.ACP.m08, buf_256, QSEQ_SEG_C2KNS_MeasCache.ACP.p08 );   strcpy ( buf_256, rpt_256 ); break;
                     case  9: sprintf ( rpt_256, ",%0.2f%s,%0.2f", QSEQ_SEG_C2KNS_MeasCache.ACP.m09, buf_256, QSEQ_SEG_C2KNS_MeasCache.ACP.p09 );   strcpy ( buf_256, rpt_256 ); break;
                     case 10: sprintf ( rpt_256, ",%0.2f%s,%0.2f", QSEQ_SEG_C2KNS_MeasCache.ACP.m10, buf_256, QSEQ_SEG_C2KNS_MeasCache.ACP.p10 );   strcpy ( buf_256, rpt_256 ); break;
                     default:
                        // FIX add error msg
                        break;
                  }
               }
            }
            else
            {
               sprintf ( rpt_256, ",%0.2f,%0.2f", QSEQ_SEG_C2KNS_MeasCache.ACP.m01, QSEQ_SEG_C2KNS_MeasCache.ACP.p01 );
            }
            sprintf ( SeqItem->String_Output, "%d%s", (long)QSEQ_SEG_C2KNS_MeasCache.ACP.Integ, rpt_256 );
         }
         break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_RHO:
            sprintf ( SeqItem->String_Output, "%d,%0.4f", (long)QSEQ_SEG_C2KNS_MeasCache.Mod.Integrity, QSEQ_SEG_C2KNS_MeasCache.Mod.Rho );
            break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_TIM_ERR:
            sprintf ( SeqItem->String_Output, "%d,%0.4f", (long)QSEQ_SEG_C2KNS_MeasCache.Mod.Integrity, QSEQ_SEG_C2KNS_MeasCache.Mod.TimingError );
            break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_FREQ_ERR:
            sprintf ( SeqItem->String_Output, "%d,%0.4f", (long)QSEQ_SEG_C2KNS_MeasCache.Mod.Integrity, QSEQ_SEG_C2KNS_MeasCache.Mod.FreqError );
            break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_CARR_FT:
            sprintf ( SeqItem->String_Output, "%d,%0.4f", (long)QSEQ_SEG_C2KNS_MeasCache.Mod.Integrity, QSEQ_SEG_C2KNS_MeasCache.Mod.CarrierFeedthrough );
            break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_PH_ERR:
            sprintf ( SeqItem->String_Output, "%d,%0.4f", (long)QSEQ_SEG_C2KNS_MeasCache.Mod.Integrity, QSEQ_SEG_C2KNS_MeasCache.Mod.PhaseError );
            break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_MAG_ERR:
            sprintf ( SeqItem->String_Output, "%d,%0.4f", (long)QSEQ_SEG_C2KNS_MeasCache.Mod.Integrity, QSEQ_SEG_C2KNS_MeasCache.Mod.MagnitudeError );
            break;
      case QSEQ_ATT_C2KNS_EQ_MEAS_EVM:
            sprintf ( SeqItem->String_Output, "%d,%0.4f", (long)QSEQ_SEG_C2KNS_MeasCache.Mod.Integrity, QSEQ_SEG_C2KNS_MeasCache.Mod.EVM );
            break;
      default:
         break;
   }
}

//-------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_Segment_End_CONFIG_C2K ( int g_hInst, const int index_master, const long segment_id_SEGM, QSEQ_Equip_State_TYPE *Data )
{
   //---------------------------------------------------
   //-- Configure of Modulation & Spectrum Parameters --
   //---------------------------------------------------

   // Required Measurements: Power, Rho, TimingError, FrequencyError, CarrierFeedthrough, PhaseError, MagnitudeError, EVM

   if ( Data->C2KNS.Action.ArbGen_On )
   {
      EQ_ArbGen_Activate_C2K ( g_hInst, Data->C2KNS.Config.Band, Data->C2KNS.Config.Channel, Data->C2KNS.Config.RF_Level_Generator, Data->C2KNS.Config.Cable_Loss, Data->C2KNS.Config.Waveform_File );
   }

   if ( segment_id_SEGM > 0 && First_Run )   // *** "SEGM0" is illegal ***
   {            
      double  Freq_Dnlink_MHz   = 0.0;
      double   Freq_Uplink_MHz   = 0.0;

      QSEQ_UTIL_BandChannel_to_Freq( Data->C2KNS.Config.Band, Data->C2KNS.Config.Channel, &Freq_Dnlink_MHz, &Freq_Uplink_MHz );

      // GPRF LIST Data Capture
      GPRF_LIST_DownlinkFreq  [ segment_id_SEGM ] = Freq_Dnlink_MHz;
      GPRF_LIST_DownlinkPower [ segment_id_SEGM ] = Data->C2KNS.Config.RF_Level_Generator;

      char cmd_512[CONST_512]  = "";
      char buf_512[CONST_512]  = "";

      char cmd_MevMod_128 [CONST_128] = "";
      char cmd_MevSpec_64 [CONST_64]  = "";

      double   ExpectedPower_dBm   = Data->C2KNS.Config.RF_Level_Analyzer; 
      int      StatisticLength      = 1;   // *** REVISIT ? *** (all user to change?)

      bool   obw_enabled         = false; // ************************* FIX **************************

// CONF:CDMA:MEAS<i>:MEV:LIST:SEGMent<no>:MOD StatisticLength,
//  EnableEVM, EnableMagError, EnablePhaseErr, EnableWaveQual, EnableIQError, EnableChPow, EnableChTimOff, EnableChPhseOff, EnableWBNBPow, EnableFreqErr

      sprintf( cmd_MevMod_128, ":CONF:CDMA:MEAS:MEV:LIST:SEGM%d:MOD %d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s;",
                           segment_id_SEGM , StatisticLength,
                           ( Data->C2KNS.Meas.EVM.Config.Enabled                     ? "ON" : "OFF" ),
                           ( Data->C2KNS.Meas.MagnitudeError.Config.Enabled            ? "ON" : "OFF" ),
                           ( Data->C2KNS.Meas.PhaseError.Config.Enabled               ? "ON" : "OFF" ),
                           ( Data->C2KNS.Meas.Rho.Config.Enabled                     ? "ON" : "OFF" ),
                           ( /*Data->C2KNS.Meas.CarrierFeedthrough.Config.Enabled*/   0   ? "ON" : "OFF" ),
                           (    /*MevMod.ChPow.Enabled*/                        0   ? "ON" : "OFF" ),
                           ( /*Data->C2KNS.Meas.TimingError.Config.Enabled*/         0   ? "ON" : "OFF" ),
                           (    /*MevMod.ChPhseOff.Enabled*/                     0   ? "ON" : "OFF" ),
                           ( Data->C2KNS.Meas.TxPower.Config.Enabled                  ? "ON" : "OFF" ),
                           ( Data->C2KNS.Meas.FreqError.Config.Enabled                  ? "ON" : "OFF" ) );

      sprintf( cmd_MevSpec_64, ":CONF:CDMA:MEAS:MEV:LIST:SEGM%d:SPEC %d,%s,%s;",
                           segment_id_SEGM,
                           StatisticLength,
                           ( Data->C2KNS.Meas.ACPR.Config.Enabled   ? "ON" : "OFF" ),
                           ( obw_enabled                     ? "ON" : "OFF" ) );

      long frames = (long)(((int)Data->Segment.Config.Duration_ms) / CONVERSION_MSEC_PER_CDMA_FRAME ); // compiler issue if NOT broken out in separate param?

      sprintf( cmd_512,   ":CONF:CDMA:MEAS:MEV:LIST:SEGM%d:SET %d,%6.2f,%6.2fe+6,OFF,R12Q;"   //2010-11-30 @ RIM per R&S (newer firmware on RIM CMW)
                     "%s"
                     "%s"
                     ":SOUR:GPRF:GEN:LIST:RFL %d,%0.2f;"
                     ":SOUR:GPRF:GEN:LIST:FREQ %d,%0.2fe6;;"
                     "*OPC?;:SYST:ERR?;"
                        , segment_id_SEGM 
                        , frames
                        , ExpectedPower_dBm
                        , Freq_Uplink_MHz
                        , cmd_MevMod_128
                        , cmd_MevSpec_64
                        , segment_id_SEGM, Data->C2KNS.Config.RF_Level_Generator
                        , segment_id_SEGM, Freq_Dnlink_MHz );

      QSEQ_WriteToEquip     ( g_hInst, cmd_512 );
      QSEQ_ReadFromEquipment( g_hInst, buf_512, CONST_512 );
      if ( buf_512[0] != '1' || buf_512[2] != '0' )
      {
         printf("\nError %s %d\n", __FUNCTION__, __LINE__);
      }
   }
}

//-------------------------------------------------------------------------------------------------------------
void EQ_ArbGen_Activate_C2K ( int g_hInst, const QSEQ_Technology_Band_TYPE Band, const long Channel, const double RF_Level_Generator, const double Cable_Loss, char* Waveform_File )
{
   static bool one_time   = true;

   char buffer_64 [CONST_64]   = "";

   if ( one_time )
   {
      QSEQ_WriteToEquip ( g_hInst, "*RST;*CLS;*OPC?" );
   }
   else
   {
      QSEQ_WriteToEquip ( g_hInst, "*CLS;*OPC?" );
   }
//   one_time = false;

   QSEQ_ReadFromEquipment( g_hInst, buffer_64, CONST_64 );
   if ( buffer_64[0] != '1' )
   {
      printf("\nError %s,%d: ClearStatusError", __FUNCTION__, __LINE__);
   }

   if ( First_Run )
   {
      char cmd_2048      [CONST_2048] = "";
      char cmd_arb_1024  [CONST_1024] = "";
      char cmd_ana_512   [CONST_512]  = "";

      double dl_Freq_MHz = -1;
      double ul_Freq_MHz = -1;
      
      QSEQ_UTIL_BandChannel_to_Freq( Band, Channel, &dl_Freq_MHz, &ul_Freq_MHz );

      GPRF_LIST_DownlinkFreq  [0] = dl_Freq_MHz;
      GPRF_LIST_DownlinkPower [0] = RF_Level_Generator;

      //-----------------------------------------------------------------------
      //   Configure the CMW500 arbitrary signal generator to play the waveform
      //-----------------------------------------------------------------------
      {
         char Waveform_File_Local[1024] = "";

         if ( 0 == strcmp ( "", Waveform_File ) )
         {
            strcpy ( Waveform_File_Local, "d:\\Rohde-Schwarz\\CMW\\data\\waveform\\CDMA2000_ch387_SYNC2000MS_V1.wv" );   // Default *** ok?
         }
         else
         {
            strcpy ( Waveform_File_Local, Waveform_File );
         }

         sprintf (cmd_arb_1024,   ":ROUT:GPRF:GEN:RFS:CONN RF1C;"
                           ":SOUR:GPRF:GEN:RFS:EATT %0.2f;"      // FIX ** NEED table lookup - auto - based on Connector(?)/Band/Chan-Freq/etc
                           ":SOUR:GPRF:GEN:BBM ARB;"
                           ":SOUR:GPRF:GEN:ARB:FILE '%s';"
                           ":SOUR:GPRF:GEN:ARB:REP CONT;"
                           ":SOUR:GPRF:GEN:ARB:CYCL 1;"         //remove?
                           ":TRIG:GPRF:GEN:ARB:RETR ON;"         //remove?    // optional (?) enables Waveform trigger pts...
                           ":TRIG:GPRF:GEN:ARB:AUT ON;"         // auto start (when Gen turned ON auto loads wv file...)
                           ":TRIG:GPRF:GEN:ARB:SEGM:MODE CSE;"      // use only in LIST mode... (SEamless) not using "yet"...
                           ":SOUR:GPRF:GEN:LIST OFF;"
                           ":SOUR:GPRF:GEN:STAT OFF;"
                           ":SOUR:GPRF:GEN:LIST:RFL 0,%0.2f;"
                           ":SOUR:GPRF:GEN:LIST:FREQ 0,%0.2fe6;"
                              , Cable_Loss
                              , Waveform_File_Local
                              , RF_Level_Generator
                              , dl_Freq_MHz );
      }

      //---------------------------------------------------------
      //   Configure the CMW RF analyzer for a CDMA MELM capture
      //---------------------------------------------------------
      {
         sprintf( cmd_ana_512,   ":ROUT:CDMA:MEAS:SCEN:SAL RF1C,RX1;"      // RF1C is connector ID, RX1 is RF sub-system-instr... not doing anything - leave in MK
                           ":CONF:CDMA:MEAS:RFS:EATT %0.2f;"         // ********FIX************   need freq based    // INPUT  need abstraction***
                           ":CONF:CDMA:MEAS:RCON R12Q;"            // INPUT  need abstraction***
                           ":CONF:CDMA:MEAS:MEV:MOEX ON;"            // measure on exception (regardless of lvl, etc... suggest OFF for a ProductionMode)
                           ":CONF:CDMA:MEAS:MEV:LIST ON;"
                           ":TRIG:CDMA:MEAS:SOUR 'IF Auto Sync';"
                           ":TRIG:CDMA:MEAS:TOUT 30000;"
                           ":TRIG:CDMA:MEAS:THR -30.0;"
                           ":TRIG:CDMA:MEAS:EOFF 8;"
                              , Cable_Loss );            // INPUT?  (ask MarkK?)
      }

      {
         sprintf( cmd_2048,   "%s"
                        "%s"
                        "*OPC?;:SYST:ERR?;"
                           , cmd_arb_1024
                           , cmd_ana_512 );

         QSEQ_WriteToEquip      ( g_hInst, cmd_2048  );
         QSEQ_ReadFromEquipment ( g_hInst, buffer_64, CONST_64 );
         if ( buffer_64[0] != '1' || buffer_64[2] != '0' )
         {
            printf("\nError %s,%d\n", __FUNCTION__, __LINE__);      // *** FIX *** why no error HERE if waveform file does not exist ???
         }
      }
   }
}

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_End_End_CONFIG_C2K ( int g_hInst, const int index_master, QSEQ_Equip_State_TYPE *Data )
{
   char cmd_1024    [CONST_1024] = "";      // *********** Check MAX size **********
   char buffer_64   [CONST_64  ] = "";

   //**********************************************
   //***** GPRF LIST Configuration/Activation *****
   //**********************************************
   if ( First_Run )
   {
      char cmd_acp_128 [CONST_128 ] = "";

      // Configure ACP Frequency Offsets --  only called ONCE per Sequence - common across all segments
      sprintf ( cmd_acp_128, ":CONF:CDMA:MEAS:MEV:ACP:FOFF %s,%s,%s,%s,%s,%s,%s,%s,%s,%s;",
                           ( QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_01   ? "0.87"  : "OFF" ),
                           ( QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_02   ? "0.885" : "OFF" ),
                           ( QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_03   ? "1.0"   : "OFF" ),
                           ( QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_04   ? "1.25"  : "OFF" ),
                           ( QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_05   ? "1.5"   : "OFF" ),
                           ( QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_06   ? "1.98"  : "OFF" ),
                           ( QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_07   ? "2.05"  : "OFF" ),
                           ( QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_08   ? "3.0"   : "OFF" ),
                           ( QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_09   ? "3.5"   : "OFF" ),
                           ( QSEQ_EQ_C2KNS_ACPR_All_Segments.Offset_10   ? "4.0"   : "OFF" ) );

      sprintf ( cmd_1024,   "%s"
                     ":CONF:CDMA:MEAS:MEV:LIST:COUN %d;"         // "1" based AND first is ignored (???) need to clarify all of this in comments!
                     ":SOUR:GPRF:GEN:LIST:REP CONT;"
                     ":SOUR:GPRF:GEN:LIST:INCR 'CDMA2000 Meas1: Multi Evaluation';"
                     ":SOUR:GPRF:GEN:LIST:INCR:ENAB 'Immediate';"
                     ":SOUR:GPRF:GEN:LIST:STAR 0;"    // "0" based
                     ":SOUR:GPRF:GEN:LIST:STOP %d;"
                     ":SOUR:GPRF:GEN:LIST ON;"
                     ":SOUR:GPRF:GEN:STAT ON;"
                     ":INIT:CDMA:MEAS:MEV;"                      //********** ARM MEV Trigger *****
                     "*OPC?;:SYST:ERR?;"
                        , cmd_acp_128
                        , Data->Segment.Data.Count + seg_bias
                        , Data->Segment.Data.Count - 1 );
   }
   else
   {
      sprintf ( cmd_1024,   ":SOUR:GPRF:GEN:STAT ON;"
                     ":INIT:CDMA:MEAS:MEV;"                      //********** ARM MEV Trigger *****
                     "*OPC?;:SYST:ERR?;"  );
   }

   QSEQ_WriteToEquip     ( g_hInst, cmd_1024 );
   QSEQ_ReadFromEquipment( g_hInst, buffer_64, CONST_64 ); 
   if ( '1' != buffer_64[0] || '0' !=  buffer_64[2] )
   {
      printf("\nError %s %d %s\n", __FUNCTION__, __LINE__, ( First_Run ? "First": "" ) );
   }
}

//-------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_End_End_EXECUTE_C2K ( int g_hInst, const int index_master, QSEQ_Equip_State_TYPE *Data )
{
   char buffer_64 [CONST_64] = "";

   unsigned long t_start = GetTickCount();

   for ( int i=1; i<1000; i++ )      // *** FIX *** NOT OPTIMIZED *** hard 10-second timeout  (CMW "READ" timeout logic is essentially the "same"... avoid polling...")
   {
      QSEQ_WriteToEquip     ( g_hInst,":FETC:CDMA:MEAS:MEV:STAT:ALL?;");
      QSEQ_ReadFromEquipment( g_hInst, buffer_64, CONST_64 );
      if ( buffer_64[2] == 'Y' ) break;   // looking for "RDY", revise not robust
      Sleep(10);      // *** FIX *** ??? *** change to 100 ms (suggestion - Mark K)
   }

   unsigned long t_stop = GetTickCount();

   if ( 'Y' == buffer_64[2] )
   {
      MACRO_IF_FIRST_RUN printf ( "\nFETCH:STATUS queries=%d [%d msec]", i, t_stop - t_start );
   }
   else
   {
      printf ( "\nFETCH:STATUS ***DID NOT COMPLETE***" );
   }

   char cmd_RfGenOff[32] = "";

   if ( Data->C2KNS.Action.RfGen_Off ) 
   {
      strcpy ( cmd_RfGenOff, ":SOUR:GPRF:GEN:STAT OFF;" );
   }
   else
   {
      printf("\nGPRF:GEN *** NOT TURNED OFF ***"); 
   }
   
   {
      char cmd[128] = "";

      sprintf ( cmd,   //":STOP:CDMA:MEAS:MEV;"      // ABORT required to avoid Analyzer-EATT Issues (which *RST solves)
                  "%s"
                  "*OPC?;:SYST:ERR?;"
                           , cmd_RfGenOff );

      QSEQ_WriteToEquip( g_hInst, cmd );
      QSEQ_ReadFromEquipment( g_hInst, buffer_64, CONST_64 );
      if ( buffer_64[0] != '1' || buffer_64[2] != '0' )
      {
         printf("\nError %s %d", __FUNCTION__, __LINE__);
      }
   }
}

//-------------------------------------------------------------------------------------------------------------
void QSEQ_EQ_Sequence_End_End_COLLECT_C2K ( int g_hInst, const int index_master, QSEQ_Equip_State_TYPE *Data )
{
   char buffer_64 [CONST_64] = "";

   QSEQ_WriteToEquip( g_hInst, ":ABORT:CDMA:MEAS:MEV;"      // ABORT required to avoid Analyzer-EATT Issues (which *RST solves)  "ABORT" MUST be after all FETCH-es (per R&S)
                        "*OPC?;:SYST:ERR?;" );

   QSEQ_ReadFromEquipment( g_hInst, buffer_64, CONST_64 );
   if ( buffer_64[0] != '1' || buffer_64[2] != '0' )
   {
      printf("\nError %s %d", __FUNCTION__, __LINE__);
   }
}











//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
void QSEQ_ParseStringToDouble(char* string, double *doubleArray, int& sizeOfArray)
{
   char* pch;
   int index = 0;
   pch = strtok(string, ",");

   while((pch!= NULL) && (index < sizeOfArray))
   {
      doubleArray[index++] = atof(pch);
      pch = strtok(NULL, ",");
   }
   sizeOfArray = index;
}

void QSEQ_UTIL_ParseStringToLong(char* string, long *longArray, int& sizeOfArray)
{
   char* pch;
   int index = 0;
   pch = strtok(string, ",");

   while((pch!= NULL) && (index < sizeOfArray))
   {
      longArray[index++] = atol(pch);
      pch = strtok(NULL, ",");
   }
   sizeOfArray = index;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
// ***** should NOT be including Mobile related header files ******
#include "qlib.h" 
#include "qlib_defines.h"

void QSEQ_UTIL_BandChannel_to_Freq( const QSEQ_Technology_Band_TYPE Band, const unsigned int iChannel, double *dl_MHz, double *ul_MHz )
{
   unsigned short iBand = END_OF_BH_BAND_CLASS_ENUM; // need undefined/unknown
   
   switch (Band )
   {
      case QSEQ_TECH_BAND_C2K_BC0:   iBand = BH_CDMA_BAND_CLASS_0;   break;
      case QSEQ_TECH_BAND_C2K_BC1:   iBand = BH_CDMA_BAND_CLASS_1;   break;
      default:
         {
            printf("\n\n********** Error: Unknown Band: Function,Line: %s,%d\n", __FUNCTION__, __LINE__);
         }
      break;
   }

   *dl_MHz = QLIB_Band_Helper_CalculateDLFrequency( iBand, iChannel );
   *ul_MHz = QLIB_Band_Helper_CalculateULFrequency( iBand, iChannel );
}       

//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
// Include NI GPIB header. You must setup the MSVC include and library paths to the NI files.
// usually under C:\Program Files\National Instruments\Shared\ExternalCompilerSupport\C
#include "decl-32.h"

void QSEQ_WriteToEquip( int g_hInst, char *sStr)
{
   ASSERT(g_hInst);
   ibwrt(g_hInst, sStr, (long)strlen(sStr));
}

void QSEQ_ReadFromEquipment(int g_hInst, char *sStr, long iSizeOfStr)
{
   ASSERT(g_hInst);
   char *pBuffer = new char[iSizeOfStr];
   memset(pBuffer, 0, iSizeOfStr);
   ibrd(g_hInst, pBuffer, iSizeOfStr);
   memcpy(sStr, pBuffer, iSizeOfStr);
}
