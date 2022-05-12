/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_GPIB/QMSL_GPIB_MSVC7/QSEQ.cpp#5 $
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

#include "QSEQ.h"
#include "QSEQ_MS.h"
#include "QSEQ_EQ.h"

//--------------------------------------------------------------------------------------------------------------------------
char* QSEQ_SegmentItem_ENUM_to_String ( const QSEQ_SegmentItem_ENUM SegmentItem )
{
   switch ( SegmentItem ) 
   {
      case QSEQ_ATT_NULL:                                 return "";

      case QSEQ_ATT_TECHNOLOGY:                           return "Technology";
      
      case QLIB_ATT_EQ_ACT_DISABLE:                        return "EQ - DISABLED";
      
      case QSEQ_ATT_C2KNS_SEG_CNFG_BAND:                     return "SEG_Config_Band";
      case QSEQ_ATT_C2KNS_SEG_CNFG_CHANNEL:                  return "SEG_Config_Channel";
      case QSEQ_ATT_SEG_CNFG_DURATION_MS:                     return "SEG_Config_Duration_ms";

      case QSEQ_ATT_C2KNS_MS_CNFG_RADIO_CONFIG:               return "MS_RadioConfig";
      case QSEQ_ATT_C2KNS_MS_CNFG_WALSH:                     return "MS_Walsh";
      case QSEQ_ATT_C2KNS_MS_CNFG_FFPC_ENABLE:               return "MS_FFPC_Enable";
      case QSEQ_ATT_C2KNS_MS_CNFG_FRAME_OFFSET:               return "MS_FrameOffset";
      case QSEQ_ATT_C2KNS_MS_CNFG_FCH_RATE:                  return "MS_FCH_Rate";
      case QSEQ_ATT_C2KNS_MS_CNFG_RPC_ENABLE:                  return "MS_Enable_RPC";
      case QSEQ_ATT_C2KNS_MS_CNFG_NUM_PREAMBLES:               return "MS_nPreamlbes";
      case QSEQ_ATT_C2KNS_MS_CNFG_TX_PATTERN:                  return "MS_TxPattern";
      case QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTRL_TYPE:               return "MS_Config_PwrCtrlType";
      case QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTRL_CMD:               return "MS_Config_PwrCtrlCmd";
      case QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR:               return "MS_Config_PwrCtrlTxPwr";
      case QSEQ_ATT_C2KNS_MS_CNFG_TX_OPEN_LOOP:               return "MS_Config_TxOpenLoop";
      case QSEQ_ATT_C2KNS_MS_CNFG_MOBILE_RX:                  return "MS_Config_Mobile_Rx";

      case QSEQ_ATT_C2KNS_MS_CNFG_DURATION_HARDHANDOFF_MS:      return "SEG_HardHandoffDuration_ms";
      case QSEQ_ATT_C2KNS_MS_CNFG_DURATION_FCH_STATUS_GET_MS:      return "SEG_FCH_StatusGet_Duration_ms";
      case QSEQ_ATT_C2KNS_MS_CNFG_DURATION_RX_LEVEL_GET_MS:      return "SEG_RxLevel_Get_ms";

      case QLIB_ATT_MS_ACT_FTM_BATCH_MODE_ACTIVATE:            return "MS_Action_FTM_BatchModeActivate";
      case QLIB_ATT_C2KNS_MS_ACT_CALL_ESTABLISH:               return "MS_Action_NS_Call_Establish";
      case QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_RESET:               return "MS_Action_FCH_Status_Reset";
      case QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_GET:               return "MS_Action_FCH_Status_Get";

      case QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL:                  return "MS_Meas_RxLevel";
      case QSEQ_ATT_C2KNS_MS_MEAS_SER:                     return "MS_Meas_SER";
      case QSEQ_ATT_C2KNS_MS_MEAS_FER:                     return "MS_Meas_FER";
      
      case QSEQ_ATT_MS_ACT_CALL_RELEASE:                     return "MS_Call_Release";
      
      case QSEQ_ATT_C2KNS_EQ_CNFG_PORT_ANA:                  return "EQ_Config_Port_Ana";
      case QSEQ_ATT_C2KNS_EQ_CNFG_PORT_GEN:                  return "EQ_Config_Port_Gen";
      case QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN:                  return "EQ_Config_RfLvl_Gen";
      case QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA:                  return "EQ_Config_RfLvl_Ana";
      
      case QLIB_ATT_C2KNS_EQ_ACT_ARB_WAVE_GEN_ON:               return "EQ_ArbWaveGen_On";
      case QLIB_ATT_EQ_ACT_RF_GEN_OFF:                     return "EQ_RfGen_Off";

      case QSEQ_ATT_C2KNS_EQ_CNFG_TRIGGER:                  return "EQ_Config_Trigger";
      case QSEQ_ATT_C2KNS_EQ_CNFG_CABLE_LOSS:                  return "EQ_CableLoss";
      case QSEQ_ATT_C2KNS_EQ_CNFG_WAVEFORM_FILE:               return "EQ_Waveform_File";

      case QSEQ_ATT_C2KNS_EQ_MEAS_PWR:                     return "EQ_Meas_Pwr";
      case QSEQ_ATT_C2KNS_EQ_MEAS_ACPR:                     return "EQ_Meas_ACPR";
      case QSEQ_ATT_C2KNS_EQ_MEAS_RHO:                     return "EQ_Meas_Rho";
      case QSEQ_ATT_C2KNS_EQ_MEAS_TIM_ERR:                  return "EQ_Meas_TimErr";
      case QSEQ_ATT_C2KNS_EQ_MEAS_FREQ_ERR:                  return "EQ_Meas_FreqErr";
      case QSEQ_ATT_C2KNS_EQ_MEAS_CARR_FT:                  return "EQ_Meas_CarrFT";
      case QSEQ_ATT_C2KNS_EQ_MEAS_PH_ERR:                     return "EQ_Meas_PhaseErr";
      case QSEQ_ATT_C2KNS_EQ_MEAS_MAG_ERR:                  return "EQ_Meas_MagErr";
      case QSEQ_ATT_C2KNS_EQ_MEAS_EVM:                     return "EQ_Meas_EVM";

      default:                                       return "UNKNOWN ENUM - Segment Item";
   }
}

//--------------------------------------------------------------------------------------------------------------------------
char* QSEQ_Action_ENUM_to_String ( const QSEQ_Action_ENUM Action )
{
   switch ( Action )
   {
      case QSEQ_ACT_SEQ_BEGIN_BEGIN:      return "Sequence Begin:Begin";
      case QSEQ_ACT_SEQ_BEGIN_ITEM:      return "Sequence Begin:Item";
      case QSEQ_ACT_SEQ_BEGIN_END:      return "Sequence Begin:End";

      case QSEQ_ACT_SEQ_SEG_BEGIN:      return "Segment Begin";
      case QSEQ_ACT_SEQ_SEG_ITEM:         return "Attribute";
      case QSEQ_ACT_SEQ_SEG_END:         return "Segment End";

      case QSEQ_ACT_SEQ_END_BEGIN:      return "Sequence END:Begin";
      case QSEQ_ACT_SEQ_END_ITEM:         return "Sequence END:Item";
      case QSEQ_ACT_SEQ_END_END:         return "Sequence END:End";

      default:                     return "UNKNOWN ENUM - Action";
   }
}

//--------------------------------------------------------------------------------------------------------------------------
char* QSEQ_SubAction_ENUM_to_String ( const QSEQ_SubAction_Type SubAction )
{
   switch ( SubAction )
   {
      case QSEQ_ACTION_SUB_CONFIGURE:      return "CONFIG";
      case QSEQ_ACTION_SUB_EXECUTE:      return "EXECUTE";
      case QSEQ_ACTION_SUB_COLLECT:      return "COLLECT";
      case QSEQ_ACTION_SUB_PROCESS:      return "PROCESS";
      case QSEQ_ACTION_SUB_REPORT:      return "REPORT";

      default:                     return "UNKNOWN ENUM - SubAction";
   }
}

//--------------------------------------------------------------------------------------------------------------------------
#define NUM_MEAS 50

char SymErrRate [NUM_MEAS][32]  = {};
char Rx_Level   [NUM_MEAS][64]  = {};
char Tx_Power   [NUM_MEAS][32]  = {};
char Rho        [NUM_MEAS][32]  = {};
char TimErr     [NUM_MEAS][32]  = {};
char FreqErr    [NUM_MEAS][32]  = {};
char Carr_FT    [NUM_MEAS][32]  = {};
char Ph_Err     [NUM_MEAS][32]  = {};
char Mag_Err    [NUM_MEAS][32]  = {};
char EVM        [NUM_MEAS][32]  = {}; 
char ACPR       [NUM_MEAS][256] = {};

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
QSEQ_Sequence_type QSEQ_Sequence_CDMA[] =   // Parameters(4):  ACTION,  ACTION_SUB,  String_in,  String_Out
{
   // ******************************
   // ******************************
   { QSEQ_ACT_SEQ_BEGIN_BEGIN   },   // Marks Beginning of SEQUENCE
   { QSEQ_ACT_SEQ_BEGIN_ITEM,   QSEQ_ATT_TECHNOLOGY,                     "CDMA2000" },
//   { QSEQ_ACT_SEQ_BEGIN_ITEM,   QLIB_ATT_EQ_ACT_DISABLE,                  "" },      // ***** EQ DISABLED *****
   { QSEQ_ACT_SEQ_BEGIN_END   },

   // ******************************
   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                           "NS Call Setup/Init: BC0,1013"   },

   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,               "0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_SEG_CNFG_BAND,               "C2K_BandClass_0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_SEG_CNFG_CHANNEL,            "1013" },

   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_WAVEFORM_FILE,         "" },   // default is "d:\\Rohde-Schwarz\\CMW\\data\\waveform\\CDMA2000_ch387_SYNC2000MS_V1.wv"
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_CABLE_LOSS,            "1.6" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,            "-40.0" },   // make all of these tech-independent ???
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,            "30.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_C2KNS_EQ_ACT_ARB_WAVE_GEN_ON,         "" },      // need explicit ON OFF (per Ben)

   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_MS_ACT_FTM_BATCH_MODE_ACTIVATE,      "" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_RADIO_CONFIG,         "1" },   // change to "RC1", "RC2", ???
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_WALSH,               "10" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_FFPC_ENABLE,            "1" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_FRAME_OFFSET,         "0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_FCH_RATE,            "FULL" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_RPC_ENABLE,            "1" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_NUM_PREAMBLES,         "0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_TX_PATTERN,            "0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_C2KNS_MS_ACT_CALL_ESTABLISH,         "" },

   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_DURATION_RX_LEVEL_GET_MS,   "20" },   // not used here, set once for subsequent segments << ok for 1/2 frame too
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_DURATION_FCH_STATUS_GET_MS,   "10" },   // not used here, set once for subsequent segments *** CHECK  1/2 Frame OK ??? ***

   { QSEQ_ACT_SEQ_SEG_END      },
   
   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "FL Power Level Change for EQ TRIGGER (NO Measurement)"    },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_RPC_ENABLE,         "0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,      "24.0" },
   { QSEQ_ACT_SEQ_SEG_END      },
   
   // ******************************
   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement (FIRST): Max Power"    },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "40" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[1] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [1] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [1] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[1] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "FL/RL Power Level Changes" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-75.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,         "10.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,      "0.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: Mod Analysis @ -75 dBm" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "100" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[2] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [2] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [2] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_TIM_ERR,            "",  TimErr  [2] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_FREQ_ERR,         "",  FreqErr [2] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_CARR_FT,            "",  Carr_FT [2] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PH_ERR,            "",  Ph_Err  [2] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_MAG_ERR,            "",  Mag_Err [2] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_EVM,               "",  EVM     [2] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[2] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "FL/RL Power Level Changes" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-80.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,         "-40.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,      "-60.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: Min Power" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "40" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[3] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [3] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [3] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[3] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "RL Power Level Change" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-104.0" },            
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: SER" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "1240" },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[4] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [4] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [4] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[4] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_RESET,      ""   },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_GET,         ""   },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_SER,               "",  SymErrRate[4] },
   { QSEQ_ACT_SEQ_SEG_END      },


   // ******************************
   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Handoff/Re-Tune: BC0,777" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "40" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_SEG_CNFG_BAND,            "C2K_BandClass_0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_SEG_CNFG_CHANNEL,         "777" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,         "30.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-40.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,      "24.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: Max Power" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "40" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[5] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [5] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [5] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[5] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "FL/RL Power Level Changes" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-75.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,         "10.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,      "0.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: Mod Analysis @ -75 dBm" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "100" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[6] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [6] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [6] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_TIM_ERR,            "",  TimErr  [6] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_FREQ_ERR,         "",  FreqErr [6] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_CARR_FT,            "",  Carr_FT [6] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PH_ERR,            "",  Ph_Err  [6] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_MAG_ERR,            "",  Mag_Err [6] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_EVM,               "",  EVM     [6] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[6] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "FL/RL Power Level Changes" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-80.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,         "-40.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,      "-60.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: Min Power" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "40" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[7] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [7] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [7] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[7] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "RL Power Level Change" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-104.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: SER" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "1240" },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[8] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [8] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [8] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[8] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_RESET,      ""   },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_GET,         ""   },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_SER,               "",  SymErrRate[8] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Handoff/Re-Tune: BC1,25" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "40" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_SEG_CNFG_BAND,            "C2K_BandClass_1" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_SEG_CNFG_CHANNEL,         "25" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,         "30.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-40.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,      "24.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: Max Power" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "40" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[9] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [9] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [9] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[9] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "FL/RL Power Level Changes" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-75.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,         "10.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,      "0.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: Mod Analysis @ -75 dBm" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "100" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[10] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [10] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [10] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_TIM_ERR,            "",  TimErr  [10] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_FREQ_ERR,         "",  FreqErr [10] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_CARR_FT,            "",  Carr_FT [10] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PH_ERR,            "",  Ph_Err  [10] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_MAG_ERR,            "",  Mag_Err [10] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_EVM,               "",  EVM     [10] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[10] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "FL/RL Power Level Changes" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-80.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,         "-40.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,      "-60.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: Min Power" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "40" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[11] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [11] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [11] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[11] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "RL Power Level Change" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-104.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: SER" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "1240" },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[12] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [12] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [12] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[12] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_RESET,      ""   },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_GET,         ""   },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_SER,               "",  SymErrRate[12] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Handoff/Re-Tune: BC1,1175" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "40" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_SEG_CNFG_BAND,            "C2K_BandClass_1" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_SEG_CNFG_CHANNEL,         "1175" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,         "30.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-40.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,      "24.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: Max Power" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "40" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[13] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [13] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [13] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[13] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "FL/RL Power Level Changes" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-75.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,         "10.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,      "0.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: Mod Analysis @ -75 dBm" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "100" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power [14] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho      [14] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR     [14] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_TIM_ERR,            "",  TimErr   [14] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_FREQ_ERR,         "",  FreqErr  [14] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_CARR_FT,            "",  Carr_FT  [14] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PH_ERR,            "",  Ph_Err   [14] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_MAG_ERR,            "",  Mag_Err  [14] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_EVM,               "",  EVM      [14] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level [14] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "FL/RL Power Level Changes" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-80.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_ANA,         "-40.0" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_CNFG_PWR_CTL_TX_PWR,      "-60.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: Min Power" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "40" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[15] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [15] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [15] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[15] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "RL Power Level Change" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "20" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_CNFG_RF_LVL_GEN,         "-104.0" },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                        "Measurement: SER" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,            "1240" },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_PWR,               "",  Tx_Power[16] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_RHO,               "",  Rho     [16] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_EQ_MEAS_ACPR,            "1", ACPR    [16] },
//   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_RX_LEVEL,         "",  Rx_Level[16] },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_RESET,      ""   },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_C2KNS_MS_ACT_FCH_STAT_GET,         ""   },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_C2KNS_MS_MEAS_SER,               "",  SymErrRate[16] },
   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   // ******************************
   { QSEQ_ACT_SEQ_END_BEGIN   },
   { QSEQ_ACT_SEQ_END_ITEM,   QSEQ_ATT_MS_ACT_CALL_RELEASE,   ""   },   // *** REQUIRED (until FTM SEQ changed to allow WAIT to be last SEQ Cmd)
   { QSEQ_ACT_SEQ_END_ITEM,   QLIB_ATT_EQ_ACT_RF_GEN_OFF,      ""   },
   { QSEQ_ACT_SEQ_END_END      },
};

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
QSEQ_Sequence_type QSEQ_Sequence_WCDMA[] =   // Parameters(4):  ACTION,  ACTION_SUB,  String_in,  String_Out
{
   // ******************************
   // ******************************
   { QSEQ_ACT_SEQ_BEGIN_BEGIN   },   // Marks Beginning of SEQUENCE
   { QSEQ_ACT_SEQ_BEGIN_ITEM,   QSEQ_ATT_TECHNOLOGY,                     "WCDMA" },
   { QSEQ_ACT_SEQ_BEGIN_ITEM,   QLIB_ATT_EQ_ACT_DISABLE,                  "" },      // ***** EQ DISABLED *****
   { QSEQ_ACT_SEQ_BEGIN_END   },

   // ******************************
   // ******************************
   { QSEQ_ACT_SEQ_SEG_BEGIN,   QSEQ_ATT_NULL,                           "NS Call Setup/Init" },

   { QSEQ_ACT_SEQ_SEG_ITEM,   QSEQ_ATT_SEG_CNFG_DURATION_MS,               "0" },

   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_C2KNS_EQ_ACT_ARB_WAVE_GEN_ON,         "" },

   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_MS_ACT_FTM_BATCH_MODE_ACTIVATE,      "" },
   { QSEQ_ACT_SEQ_SEG_ITEM,   QLIB_ATT_C2KNS_MS_ACT_CALL_ESTABLISH,         "" },

   { QSEQ_ACT_SEQ_SEG_END      },

   // ******************************
   // ******************************
   { QSEQ_ACT_SEQ_END_BEGIN   },
   { QSEQ_ACT_SEQ_END_END      },
};

//--------------------------------------------------------------------------------------------------------------------------
// QSEQ_Sequence_RPT() easily encapsulated in separate file, leave here for now...
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
static QSEQ_Sequence_type *Sequence_RPT;   // check - link error if not unique...???
static int nSeq_RPT = -1;

//--------------------------------------------------------------------------------------------------------------------------------------------
unsigned char QSEQ_Sequence_RPT_Set ( long nSegItem, QSEQ_Sequence_type *SeqItem )
{
   Sequence_RPT = SeqItem;

   nSeq_RPT = nSegItem;

   return 0;
}

unsigned char QSEQ_Sequence_RPT (   QSEQ_SubAction_Type   sub_action )
{
   // Special Function for quick-look Report

   if ( QSEQ_ACTION_SUB_REPORT == sub_action ) 
   {
      unsigned long t_start = GetTickCount();

      for ( int Index_SequenceMaster = 0; Index_SequenceMaster < nSeq_RPT; Index_SequenceMaster++ )
      {
         QSEQ_Sequence_type   *SeqItem = &Sequence_RPT[Index_SequenceMaster];

         char report_header[256]      = "";
         char String_SeqSegItem[256]   = "";
         char String_In[256]         = "";
         char String_Out[256]      = "";
         if ( 0 == Index_SequenceMaster )
         {
            sprintf ( report_header, "\n\n********** REPORT **********\n\n" );
         }

         if ( 0 != &SeqItem->SegmentItem )
         {
            if ( strlen ( QSEQ_SegmentItem_ENUM_to_String ( SeqItem->SegmentItem ) ) )
            {
               sprintf ( String_SeqSegItem, ", %s", QSEQ_SegmentItem_ENUM_to_String ( SeqItem->SegmentItem ));
            }
         }

         if ( 0 != SeqItem->String_Input )
         {
            if ( strlen ( SeqItem->String_Input ) )
            {
               sprintf ( String_In, ", [%s]", SeqItem->String_Input );
            }
         }

         if ( 0 != SeqItem->String_Output )
         {
            if ( strlen ( SeqItem->String_Output ) )
            {
               sprintf ( String_Out, ", [%s]", SeqItem->String_Output );
            }
         }

         printf ( "%s%s%s%s%s\n",
                  report_header,
                  QSEQ_Action_ENUM_to_String ( SeqItem->Action ),
                  String_SeqSegItem,
                  String_In,
                  String_Out );
      }
      unsigned long t_stop  = GetTickCount();

      printf("\n\nREPORT Elapsed Time = %d msec ******************************** Timing ***", t_stop - t_start );
   }
   return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
void QSEQ_Sequencer_Prototype ( int iSequence_ID, HANDLE g_hQMSL, int g_hInst )
{
   unsigned long t_start = GetTickCount();

   switch ( iSequence_ID )
   {
      case 0:
         {
            int nSeq = sizeof(QSEQ_Sequence_CDMA)/sizeof(QSEQ_Sequence_CDMA[0]);

            QSEQ_Sequence_MS_Set  ( nSeq, QSEQ_Sequence_CDMA );
            QSEQ_Sequence_EQ_Set  ( nSeq, QSEQ_Sequence_CDMA );
            QSEQ_Sequence_RPT_Set ( nSeq, QSEQ_Sequence_CDMA );
         }
         break;
      case 1:
         {
            int nSeq = sizeof(QSEQ_Sequence_WCDMA)/sizeof(QSEQ_Sequence_WCDMA[0]);

            QSEQ_Sequence_MS_Set  ( nSeq, QSEQ_Sequence_WCDMA );
            QSEQ_Sequence_EQ_Set  ( nSeq, QSEQ_Sequence_WCDMA );
            QSEQ_Sequence_RPT_Set ( nSeq, QSEQ_Sequence_WCDMA );
         }
         break;
      default:
         printf("\n\n\nUNKNOWN SEQUENCE [%d]\n", iSequence_ID );
         break;
   }

   for ( int i = 0; i < QSEQ_Action_Sub_Count; i++ )
   {
      QSEQ_Sequence_MS  ( g_hQMSL, QSEQ_Action_Sub[i] ); // MUST be FIRST (since NOT running in parallel) *** for EXECUTE Phase
      QSEQ_Sequence_EQ  ( g_hInst, QSEQ_Action_Sub[i] );
      QSEQ_Sequence_RPT (          QSEQ_Action_Sub[i] );   // quick prototype
   }

   unsigned long t_stop  = GetTickCount();

   printf("\n\n\nTotal Elapsed Time = %d msec ******************************** Timing ***\n", t_stop - t_start );
}
