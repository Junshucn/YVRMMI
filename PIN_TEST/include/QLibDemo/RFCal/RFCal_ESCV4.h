#pragma once
/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/RFCal_ESCV4.h#1 $
 * $DateTime: 2017/05/25 12:46:42 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2017 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#pragma once
#include "RFCal.h"

// callbox implementation
#include "CMeasurementManager_QSCPI.h"

#ifndef QLIB_DEMO_EXT
#include "CMeasurementManager_Misc.h"
#endif

// Disable compiler waring 'getch' was declared deprecated, for MSVC8
#pragma warning (disable : 4996)

// This file is manditory
#ifndef __QLIB_H
#define __QLIB_H
#include "QLib.h"
#endif

// This file can be included to get data types, structures, and enumeration defintions
// It is not manditory though because all of the libraries interfaces are standard ANSI C
// data types.
#ifndef __QLIB_DEFINES_H
#define __QLIB_DEFINES_H
#include "QLib_Defines.h"
#endif

#include "RFCal_Helper.h"
#include <vector>
using namespace std;
// For RF FTM execution.
//#define ON_TARGET


unsigned char Test_RFCal_ESC_ET();


enum ESC_TX_MASK_TYPE
{
   TX_NORMAL_MASK = 0,
   TX_DPD_MASK = 1,
   TX_LOW_RGI = 2,
   TX_OVERRIDE_NORMAL_MASK = 0,
   TX_OVERRIDE_DPD_MASK = 1,
   TX_OVERRIDE_LOW_RGI_MASK = 2,
};

typedef enum
{
   DATA_OPCODE_ACTION_RGI_PWR_TBL = 0x1,
   DATA_OPCODE_ACTION_LOAD_DPD = 0x2,
   DATA_OPCODE_ACTION_MLINE_CALC = 0x4,
   DATA_OPCODE_ACTION_DELAY_PROC = 0x8,
   DATA_OPCODE_ACTION_DPD_PROC = 0x10,
   DATA_OPCODE_ACTION_LOAD_CALC_DPD = 0x20,
   DATA_OPCODE_ACTION_GET_MLINE_RES = 0X40,
   DATA_OPCODE_ACTION_XPT_CAP_RES = 0x80,
   DATA_OPCODE_ACTION_PIN_PROC = 0x100,
   DATA_OPCODE_ACTION_RGI_PWR_TBL_COM = 0x200,
   DATA_OPCODE_ACTION_MAX = 0xFFFFFFFF
} DATA_OPCODE_ACTION_TYPE;
typedef enum
{
   NO_ACTION_PRE_COMPUTE_TYPE = 0,
   RGI_DELTAS_PRE_COMPUTE_TYPE = 1,
   APT_BIAS_PRE_COMPUTE_TYPE = 2,
   FREQCOMP_RGI_PRE_COMPUTE_TYPE = 3,
   XPT_SWP2_FROM_SWP1_DATA_COMPUTE_TYPE = 4,
   LIM_FREQCOMP_RGI_PRE_COMPUTE_TYPE = 5,
   FB_GAIN_PARAM_COMPUTE_TYPE = 6,
   TXRSB_PA_SETTING_COMPUTE_TYPE = 7,
   MEASUREMENT_REDUCTION_PRE_COMPUTE_TYPE = 8,
   FB_GAIN_UBIAS_RX_SCALE_COMPUTE_FOR_APT = 9,
   FB_GAIN_UBIAS_RX_SCALE_COMPUTE_FOR_XPT = 10,
   RXRSB_TRIG_SETTING_COMPUTE_TYPE = 11,
   MAX_PRE_COMPUTE_TYPE = 0xFFFF
} PRE_COMPUTE_TYPE;
typedef enum 
{
   RESULT_CONF = 1,

   RESULT_MEAS = 0,
   
   RESULT_DSREF = 2,
   RESULT_DESC = 3,
   RESULT_MIN = 4,
   RESULT_MAX = 5
} RESULT_XML;
enum CALPATH
{
    CALPATH_SG1 = 1,
    CALPATH_SG2 = 2,
    CALPATH_SG3 = 3,
    CALPATH_DownlinkCP1 = 4,
    CALPATH_UplinkCP1 = 5,
    CALPATH_GPS = 6,
    CALPATH_DownlinkDivCP2 = 7,
    CALPATH_UplinkCP2 = 8,
    CALPATH_DownlinkAuxCP1 = 14,
    CALPATH_DownlinkAuxCP2 = 214
};
/************************************

Helper functions

*************************************/

// CAL V3
typedef struct TE_Cal_Swp_Params
{
public:
   //! UL Channel for the segment
   UINT m_ul_chan;
   //! Input level for the segment
   double m_ul_lvl;
   //! DL Level for the segment
   double m_dl_lvl;
   //! Tx waveform to be set for the segment
   int m_tx_waveform;
   //! Step width for the segment in ms
   double m_step_width;
   //! LTE Rx BW for the segment
   int m_lte_tx_chan_bw;
   //! Total number of steps in the sweep. Needed in Linearizer sweep. Does it need to be part of this struct. Maybe not!!!! CHECK
   //int m_total_steps;
   //! UL DB per step. May not be required!!!! CHECK
   double m_ul_db_change_per_step;
   //! Min UL Level
   double m_min_ul_lvl;
   //! RF Mode
   FTM_RF_Mode_Enum m_rf_mode;
   //! LTE Rx BW for the segment
   int m_lte_rx_chan_bw;
   //! indicate wheater to expect a te result or not (used only in ESC)
   bool m_teAction;
   //! num of Rx channels to calibrate
   int m_numChans;
   //! offset to tune
   int m_offset_in_kHz;


   TE_Cal_Swp_Params() :
      m_ul_chan(0),
      m_ul_lvl(0.0),
      m_dl_lvl(0.0),
      m_tx_waveform(0),
      m_step_width(0.0),
      m_lte_tx_chan_bw(0),
      m_ul_db_change_per_step(0.0),
      m_min_ul_lvl(0.0),
      m_rf_mode(PHONE_MODE_MAX),
      m_lte_rx_chan_bw(0),
      m_teAction(true),
      m_numChans(1),
      m_offset_in_kHz(0)
   {}
} TE_Cal_Swp_Params;
unsigned char ExportSweepRecordToCSV(cal_sweep_record* tmp, string filename, int size);
unsigned char ExportSweepRecordToCSV(cal_sweep_record* tmp, TE_Cal_Swp_Params * teSweep,string filename, int size);
unsigned char addHeader(string* stHeader1stLine, string* stHeader2ndLine,  string stHeader1,string stHeader2);
int getCommaCount(string tmp);
unsigned char addArrayHeader(string* stHeader1stLine, string* stHeader2ndLine,   string stArrayName,int count);

// End of helper function for ESC ET (V4) Cal.

// ******************************************************************//
//    Begging of RFCal ESC ET related demo code
//    This RFCal ESC ET requires XML support
// ******************************************************************//

typedef enum
{
   MASTER_RADIO_STATE_NO_CHANGE = 0,
   MASTER_RADIO_STATE_DISABLED = 1,
   MASTER_RADIO_STATE_RX_STATE = 2,
   MASTER_RADIO_STATE_RX_TX_STATE = 3,
   MASTER_RADIO_STATE_RETUNE = 4,
   MASTER_RADIO_STATE_MAX = 0xFF
} MASTER_RADIO_STATE;

typedef enum
{
   SLAVE_RADIO_STATE_NO_CHANGE = 0,
   SLAVE_RADIO_STATE_DISABLED = 1,
   SLAVE_RADIO_STATE_RX_DIV_STATE = 2,
   SLAVE_RADIO_STATE_RETUNE = 4,
   SLAVE_RADIO_STATE_MAX = 0xFF
} SLAVE_RADIO_STATE;

enum
{
   TECH_CDMA            = 0,
   TECH_WCDMA           = 1,
   TECH_LTE             = 3,
   TECH_TDSCDMA     = 4
};

typedef struct
{
   byte        NV_Mode;
   byte        NV_Write_Action_Flag;
   byte        NV_Logging_En_Flag;
   byte        Dual_Carr_NV_Mode;
   byte         Use_Consolidated_Rx_NV;
   byte        Read_Consolidated_Rx_NV_From_DUT;
   word         Max_HDET_Value;
   double         Max_HDET_Pwr;
   double         Min_HDET_Pwr;
   double         Max_LPM_HDET_Pwr;
   double         Min_LPM_HDET_Pwr;
   double         LPM_HDET_Start_Pwr;
   double         Min_HDET_Delta;
} _NV_Generation_Params;

typedef struct
{
   int            Master_Device_ID;
   int            Slave_Device_ID;
} _Device_Selection;

typedef struct
{
   int            Waveform_Type;
   int            Start_RB;
   int            Number_Of_RBs;
   int            Tx_BW;
   int            Rx_BW;
   int            EUL_Waveform_Type;
} _Waveform;

typedef struct
{
   word        IcqEt;
   word        IcqEpt;
   word        IcqRgiDeltaSwp;
   word        IqGainRgiDeltaSwp;
   word        FirmwareDelayIqGain;
   word        AclrDelayCharIqGain;
   double         EtVmin;
   double         MLineRegCoef;
   byte        MLineFitType;
   double         CalCompPtEpt;
   double         CalCompPtEt;
   double         MLineMaxPaSupplyEpt;
   double         MLineMaxPaSupplyEt;
   double         qpoetVdMin;
   double         qpoetVdMax;
   vector <word>  Detrough_table;
} XptInput_V1;
typedef struct
{
   byte           Master_Device_ID;
   byte           Slave_Device_ID;
   byte           Feedback_Device_ID;
   byte           Scell_Master_Device_ID;
   byte           Scell_Slave_Device_ID;
   byte           Tx_NV_Container;
   byte           Pcell_Master_Device_ID;
   byte           Pcell_Slave_Device_ID;

   map <byte, byte >   NV_Device_To_Chain_Map;
   vector <vector <byte> > DevicePathPair_To_NVcontainer;
   vector <vector <byte> > Master_Device_ID_List;
   vector <vector <byte> > Slave_Device_ID_List;

   vector <byte>     Tx_Device_To_Calibrate;
   vector <byte>     Rx_Device_To_Calibrate;
} _device_selection;

typedef struct Sweep_Params
{
   unsigned char        bFirstSweep;  // QlibDemo Original
   /* Common parameters for all sweep type */
   word           Sweep_Type;
   vector <byte>     Pre_Sweep_Computation;
   vector <byte>     Compute_From_Sweep;
   vector <byte>     Post_Sweep_Computation;
   vector <byte>     Compute_For_Sweep;
   _Waveform         Waveform;
   _device_selection Device_Selection;

   byte           Enable_Sweep_Mode;
   byte           Meas_Tx;
   byte           Meas_Rx;
   dword          Ref_Channel;
   dword          Upper_Bound_Channel;

   vector <dword>    Tx_Channel_List;
   vector <dword>    Rx_Channel_List;
   vector <byte>     HDET_Avgs;
   vector <byte>     LPM_HDET_Avgs;
   vector <byte>     Purpose_List;

   vector <byte>     PA_State_List;
   vector <vector <byte> >    RGI_List;
   vector <vector <word> >    Bias_List;
   vector <vector <byte> >    PA_Quiescent_Current;
   vector <vector <word> >    IQ_Gain;
   vector <vector <word> >    Env_Scale;

   vector <vector <dword > >     Segment_Length;
   dword          Config_Segment_Length;
   dword          Tuning_Segment_Length;
   vector <dword>       Rx_Segment_Length;
   vector <dword>       Tx_Segment_Length;

   /* Parameters for callbox */
   vector <double>   Min_Start_Pwr;
   vector <double>   Max_Expected_Pwr;
   vector <double>   Lowest_End_Pwr;

        /* Parameters for FBRxGain Cal */
   byte           Do_FBGain_Cal;
   byte           FBRx_Cal_Mode;
   vector <byte>     FBGain_Cal_XPTMode_List;
   vector <byte>     FBGain_Cal_Gain_State_List;
   vector <byte>     FBGain_Cal_UBias_List;
   vector <word>     FBGain_Cal_RxScale_List;
   word           FBGain_Cal_GainComp_Val;
   byte           Enable_FBGain_Cal_Debug_Mode;
      byte              FBGain_Cal_Do_Therm_Read;
   vector <byte>     FBGain_Cal_BW_List;
   vector <byte>     FBGain_Cal_StartRB_List;
   vector <byte>     FBGain_Cal_NumRB_List;
   word              FBGain_Cal_Min_Limit;
   word              FBGain_Cal_Max_Limit;
        vector < short >        FBGain_Cal_Gain_State_Swpt_List;
        vector < short >        FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld_List;

        /* Common Parameters for Enh Internal Dev FBRx Cal */
   byte              Do_Enh_Int_Cal;
        word                    Enh_Int_Cal_Flag;
   vector <byte>        Enh_Int_Cal_Tx_Bandwidth_List;

   byte              Enh_Int_Cal_Type_Mask;
   byte              PA_State;
   byte              RGI;
   word              Bias;
        word                    Icq;
   vector<byte>         TechBW_List;
        dword                   Enh_Int_Cal_Segment_Length;

        /* Parameters for FBRxDC cal */
        byte                    Enh_Int_Cal_Debug_Mode_Enable;
   byte              Device_Mask;
   vector<byte>         FBRx_DC_Gain_State_List;
   word              FBRx_DC_Num_Capture_Samples;
        vector<long>            FBRx_DC_I_Min_Limit;
        vector<long>            FBRx_DC_I_Max_Limit;
        vector<long>            FBRx_DC_Q_Min_Limit;
        vector<long>            FBRx_DC_Q_Max_Limit;
        
        /* Parameters for FBRxRSB */

        byte                    Do_Enh_Int_FBRxRsb_Cal;
        dword                   FBRxRsb_Segment_Length;
        byte                    FBRxRsb_Device_Mask;
        byte                    FBRxRsb_Bandwidth;
        byte                    FBRxRsb_DbgLevel;
        byte                    FBRxRsb_Type;
        byte                    FBRxRsb_Algorithm;
        dword                   FBRxRsb_Waveform_Offset_in_kHz;
        vector < byte >         FBRxRsb_Gain_State_List;
        vector < short >        FBRxRsb_Power_List;
        byte                    FBRxRsb_CarrierID;
        byte                    FBRxRsb_IQ_Capture_Type;
        word                    FBRxRsb_IQ_Capture_Size;
        byte                    FBRxRsb_PCell_Band_For_SCell;
        dword                   FBRxRsb_PCell_RefChan_For_SCell;
        byte                    Do_FBRxRsb_Limit_Check;
        word                    FBRxRsb_PreCal_Rsb_Limit;
        word                    FBRxRsb_PostCal_Rsb_Limit;

        /* Parameters for RxRSB Cal */
        byte                    Do_Enh_Int_Rx_Rsb_Cal;
        dword                   RxRsb_Segment_Length;
        byte                    RxRsb_Device_Mask;
        byte                    RxRsb_DL_Bandwidth;
        byte                    RxRsb_DbgLevel;
        byte                    RxRsb_Type;
        byte                    RxRsb_Algorithm;
        dword                   RxRsb_Waveform_Offset_in_kHz;
        vector < byte >         RxRsb_Gain_State_List;
        vector < short >        RxRsb_DL_Power_List;
        byte                    RxRsb_CarrierID;
        byte                    RxRsb_IQ_Capture_Type;
        word                    RxRsb_IQ_Capture_Size;
        byte                    RxRsb_PCell_RFMode_For_SCell;
        byte                    RxRsb_PCell_Band_For_SCell;
        dword                   RxRsb_PCell_RefChan_For_SCell;
        byte                    Do_RxRsb_Limit_Check;
        word                    RxRsb_PreCal_Rsb_Limit;
        word                    RxRsb_PostCal_Rsb_Limit;

   /* Parameters for XPT_SWP1 */
   vector <dword>    XPT_Swp1_Channel_List;
   byte           XPT_Swp1_PA_State;
   byte           XPT_Swp1_PA_State_ET;
   byte           XPT_Swp1_PA_State_EPT;

   vector <byte>     XPT_Swp1_RGI_List;
   vector <byte>     XPT_Swp1_RGI_List_ET;
   vector <byte>     XPT_Swp1_RGI_List_EPT;

   vector <word>     XPT_Swp1_Bias_List;
   vector <word>     XPT_Swp1_Bias_List_ET;
   vector <word>     XPT_Swp1_Bias_List_EPT;

   word           XPT_Swp1_IQ_Gain;
   word           XPT_Swp1_Env_Scale;
   word           XPT_Swp1_Num_IQ_Samples;
   dword          XPT_Swp1_Cap_Segment_Length;
      unsigned char       Enable_Tx_Lin_Limits; // Enable/Disable Tx limits to check for min and max power per PA state

   /* Parameters for RGI Delta Sweep / XPT_SWP1 */
   vector <byte>     XPT_Swp1_RGI_Delta_RGI_List;
   vector <byte>     XPT_Swp1_RGI_Delta_RGI_List_ET;
   vector <byte>     XPT_Swp1_RGI_Delta_RGI_List_EPT;
   byte           XPT_Swp1_RGI_Delta_Ref_RGI;
   word           XPT_Swp1_RGI_Delta_Bias;
   dword          XPT_Swp1_RGI_Delta_Segment_Length;
   word           XPT_Swp1_Minimum_RGI_Delta_Power_dBm100;
   double                   XPT_Swp1_Max_Expected_Power;
   byte                 Enable_Tx_Int_Cal_Power_Calc_Fudge;

   /* Parameters for Ref Cal/Tx Internal Cal */
   byte           Low_Power_Threshold_RGI;
   byte           Low_Power_Threshold_PA;
   byte           Enable_Box_Measurements;
   vector <vector <byte > > Ref_RGI_List;
   word           Fb_Rx_Gain_Cal_Mode;
      vector <double>   XPT_Swp1_Power_List_EPT;
      vector <double>   XPT_Swp1_Power_List_ET;
      vector <double>   XPT_Swp1_Power_List;

   /* Parameters for XPT_SWP2 */
   vector <dword>    XPT_Swp2_Channel_List;
   byte           XPT_Swp2_EPT_PA_State;
   byte           XPT_Swp2_ET_PA_State;
   vector <double>      ET_Swp2_Meas_Pwr_List;
   vector <double>      EPT_Swp2_Meas_Pwr_List;
   vector <double>      EPT_Swp2_Calc_Pwr_List;
   word           XPT_Swp2_Num_IQ_Samples;
   word           XPT_Swp2_Cap_Segment_Length;
   word           XPT_Swp2_MLine_Calc_Segment_Length;
   dword             XPT_Swp2_IQ_Processing_Segment_Length;
   byte           XPT_Swp2_MLine_Bias_Option;
   byte           XPT_Swp2_Enable_Mline_Debug_Info;

   /* Parameter for XPT_SWP3 */
   vector <dword>    XPT_Swp3_Channel_List;
   vector <double>      ET_Swp3_Meas_Pwr_List;
   vector <double>      EPT_Swp3_Meas_Pwr_List;
   vector <double>      EPT_Swp3_Calc_Pwr_List;
   dword          XPT_Swp3_Segment_Length;
   int               XPT_Swp3_Linearizer_Delta_Meas_Power_Min_dBm100;
   int               XPT_Swp3_Deviation_From_Exp_Power_Max_dBm100;
   int               XPT_Swp3_Deviation_From_Exp_Power_Min_dBm100;
   byte             XPT_Swp3_Apply_Pin;
   double               XPT_Swp3_Max_Expected_Power;

   /* Parameter for XPT_FreqComp */
   vector <dword>    XPT_Fcomp_Ref_Channel_List;
   vector <dword>    XPT_Upper_Bound_Channel_List;
   vector <double>      ET_Fcomp_Meas_Pwr_List;
   vector <double>      EPT_Fcomp_Meas_Pwr_List;
   vector <double>      EPT_Fcomp_Calc_Pwr_List;
   int               XPT_Fcomp_Deviation_From_Exp_Power_Min_dBm100;
   int               XPT_Fcomp_Deviation_From_Exp_Power_Max_dBm100;

   /* Parameter for FreqComp */
   vector <byte>     RGI_Threshold_List;
   vector <byte>     Use_RGI_Pwr_List;
   vector <double>      FreqComp_Pwr_List;
   vector <double>      APT_FreqComp_Offset_List;
   vector <double>      Max_FreqComp_Pwr;
   vector <double>      Min_FreqComp_Pwr;
   vector <double>  Max_FreqComp_Pwr_dB;
   vector <double>  Min_FreqComp_Pwr_dB;
   word              Min_FreqComp_HDET;
   word              Max_FreqComp_HDET;
   unsigned char     Use_PilotOnlyWaveform;
   byte           Measure_DC;
   byte           Measure_CA;
   byte           Measure_3DLCA;
   byte           Measure_3C;
   byte           CA_PCell_RF_Mode;
   byte           CA_PCell_Band;
   dword          CA_PCell_Channel;
   byte                 Split_Rx;
   byte              Enable_Downlink_Aux_Port;
   byte                 Enable_SCell_Only_Cal;
   unsigned short       Rx_Calibration_Type;
   vector <byte>     Rx_Actions_List;
   vector <byte>     Rx_PM_List;
   vector <word>     DL_Level_List;
   vector <word>     Rx_C0_Max_Limits;
   vector <word>     Rx_C0_Min_Limits;
   vector <word>     Rx_C1_Max_Limits;
   vector <word>     Rx_C1_Min_Limits;
   vector <word>     Rx_C2_Max_Limits;
   vector <word>     Rx_C2_Min_Limits;
   vector <word>     Rx_C3_Max_Limits;
   vector <word>     Rx_C3_Min_Limits;

   // Delay sweep parameters
   vector <dword>    XPT_Delay_Swp_Channel_List;
   vector <dword>    XPT_Delay_Swp_Upper_Bound_Channel_List;
   vector <byte>     XPT_Delay_Swp_Num_RB_List;
   vector <byte>     XPT_Delay_Swp_Start_RB_List;
   vector <byte>     XPT_Delay_Swp_BW_List;
   vector <byte>     XPT_Delay_Swp_5MHz_Offset_List;
   vector <byte>     XPT_Delay_Swp_10MHz_Offset_List;
   vector <byte>     XPT_Delay_Swp_20MHz_Offset_List;
   byte           XPT_Delay_Swp_PA_State;
   byte           XPT_Delay_Swp_RGI_Fudge_From_MLine;
   word           XPT_Delay_Swp_Bias;
   word           XPT_Delay_Swp_IQ_Gain;
   word           XPT_Delay_Swp_Env_Scale;
   byte           XPT_Delay_Swp_PA_Quiescent_Current;
   word           XPT_Delay_Swp_Num_IQ_Samples;
   dword          XPT_Delay_Swp_Cap_Segment_Length;

   /* Parameters for PIN CAL */
   vector <dword>      XPT_PinCal_Swp_Channel_List;
   dword              XPT_PinCal_Swp_Ref_Channel;
   vector <byte>      XPT_PinCal_Swp_PA_State_List;
   vector <byte>      XPT_PinCal_Swp_XPT_Action_List; // XPT Mode
   word               XPT_PinCal_Swp_IQ_Gain;
   word               XPT_PinCal_Swp_Env_Scale;
   word               XPT_PinCal_Swp_Num_IQ_Samples;
   dword              XPT_PinCal_Swp_Cap_Segment_Length;
   dword              XPT_PinCal_Swp_Proc_Segment_Length;
   dword              XPT_PinCal_Swp_Cap2Proc_Wait_Time;
   vector <int>    XPT_PinCal_Swp_TargetCompList_dB10; // Data are assigned in createXPTPinCalSweep.
   vector <byte>       XPT_PinCal_Swp_XPT_RGI_List; // Assigned by post computation
   vector <word>       XPT_PinCal_Swp_XPT_Bias_List; // Assigned by post computation
   vector <word>       XPT_PinCal_Swp_Peak_Compression_dB10_Max_Lim_List; // 
   vector <word>       XPT_PinCal_Swp_Pin_Variation_dB10_Max_Lim_List; // 
   byte            XPT_PinCal_Swp_RGI_Fudge_Factor; // Default 3

        // Multi Band Rx Cal/Char
        vector <word>       Band_List;
        vector <byte>       RFMode_List;
        vector <byte>       NVMode_List;
        vector <dword>      RefChannel_List;
        vector <byte>       CA_PCell_RF_Mode_List;
        vector <byte>       CA_PCell_Band_List;
        vector <dword>      CA_PCell_Channel_List;
        vector <byte>       NV_DC_Mode_List;
//      vector <byte>       NV_3C_Mode_List;

} Sweep_Params;

typedef struct
{
   double   pwr;
   word  vcc;
   byte  icq;
} APTRec;

typedef struct APT_CharV3_Table_Key
{
   byte  rf_mode;
   dword channel;
   byte  pa_state;
   byte  tx_purpose;
   byte  device;
   APT_CharV3_Table_Key(byte _rf_mode, byte _device, dword  ichannel, byte _pa_state, byte _tx_purpose) : rf_mode(_rf_mode), device(_device), channel( ichannel), pa_state(_pa_state), tx_purpose(_tx_purpose) {};

   bool operator < (const APT_CharV3_Table_Key & Apt) const
   {
      if (this->rf_mode == Apt.rf_mode)
      {
         if (this->channel == Apt.channel)
         {
            if (this->pa_state == Apt.pa_state)
            {
               if (this->tx_purpose == Apt.tx_purpose)
               {
                  return this->device > Apt.device;
               }
                  return this->tx_purpose > Apt.tx_purpose;
               }
               return this->pa_state > Apt.pa_state;
            }
            return this->channel > Apt.channel;
         }
         return this->rf_mode > Apt.rf_mode;
      }
} APT_CharV3_Table_Key;

typedef struct RFCal_Helper_Rx_Params
{
    byte    rf_mode;
    byte    instance;
   dword Channel;
   byte  LNARange;
    byte    RxPath;// Chain
   double   RxLevel;
   byte  RxPM;
   byte  RxMeasAction;
    word    rx_cal_type;
    RFCal_Helper_Rx_Params():rf_mode(0), instance(0), Channel(0), LNARange(0),RxPath(0), RxLevel(0),RxPM(0), RxMeasAction(0), rx_cal_type(0) {} ;

   bool operator < (const RFCal_Helper_Rx_Params & Rx) const
   {
      if (this->rf_mode == Rx.rf_mode)
      {
        if (this->instance == Rx.instance)
        {
      if (this->Channel == Rx.Channel)
      {
         if (this->LNARange == Rx.LNARange)
         {
            if (this->RxPath == Rx.RxPath)
            {
               if (this->RxLevel == Rx.RxLevel)
               {
                  if (this->RxMeasAction == Rx.RxMeasAction)
                  {
                                if (this->RxPM == Rx.RxPM)
                                {
                                    return this->rx_cal_type > Rx.rx_cal_type;
                                }
                                else { return this->RxPM > Rx.RxPM;}
                            } else { return this->RxMeasAction > Rx.RxMeasAction; }
                        } else { return this->RxLevel > Rx.RxLevel; }
                    } else { return this->RxPath > Rx.RxPath;}
                } else { return this->LNARange > Rx.LNARange; }
            } else { return this->Channel > Rx.Channel; }
        } else {return this->instance > Rx.instance; }
      } else { return this->rf_mode > Rx.rf_mode; }
   }
} RFCal_Helper_Rx_Params;
typedef struct RFCal_Helper_Tx_Params
{
    double  ExpPwr;
    dword   Channel;
    dword   Bias;
    dword   PaQuiescentCurrent;

    word    Band;
   dword   IqGain;
   dword   EnvScale;
    word    FBgain;
    word    RxScale;

   byte  Tech;
   byte  PaState;
   byte  XPTmode;
   byte  RGI;
   byte  Purpose;
    byte    FBGainState;
    byte    uBias;
    byte    RFMode;
    byte    device;
   dword  delayVal;
   byte bw;

   RFCal_Helper_Tx_Params ():
      Tech(0), Band(0), Channel(0), PaState(0),XPTmode(0),RGI(0),Bias(0),IqGain(0),Purpose(0)
      ,EnvScale(0), PaQuiescentCurrent(0), ExpPwr(0), FBGainState(0), FBgain(0), uBias(0),RxScale(0), RFMode(0), device(0), delayVal(0), bw(0) {};
   RFCal_Helper_Tx_Params (byte _Tech, word _Band) :
      Tech(_Tech), Band(_Band), Channel(0), PaState(0),XPTmode(0),RGI(0),Bias(0),IqGain(0),Purpose(0)
      ,EnvScale(0), PaQuiescentCurrent(0), ExpPwr(0), FBGainState(0), FBgain(0), uBias(0),RxScale(0), RFMode(0), device(0), delayVal(0), bw(0) {};

   //RFCal_Helper_Tx_Result (byte _Tech, byte _Band, work _Channel, byte _paState, byte XPTMode, byte RGI, word Bias, word)
   // Make sortable for map
   bool operator < (const RFCal_Helper_Tx_Params & Tx) const
   {
      if (this->Tech == Tx.Tech)
      {
         if (this->Band == Tx.Band)
         {
            if (this->Bias == Tx.Bias)
            {
               if (this->bw == Tx.bw)
               {
               if (this->Channel == Tx.Channel)
               {
                  if (this->PaState == Tx.PaState)
                  {
                     if (this->XPTmode == Tx.XPTmode)
                     {
                        if (this->RGI == Tx.RGI)
                        {
                           if (this->IqGain == Tx.IqGain)
                           {
                              if (this->EnvScale == Tx.EnvScale)
                              {
                                 if (this->PaQuiescentCurrent == Tx.PaQuiescentCurrent)
                                 {
                                    if (this->ExpPwr == Tx.ExpPwr)
                                    {
                                        if (this->Purpose == Tx.Purpose)
                                        {
                                            if (this->FBgain == Tx.FBgain)
                                            {
                                                if (this->uBias == Tx.uBias)
                                                {
                                                    if (this->RxScale == Tx.RxScale)
                                                    {
                                                        if (this->RFMode == Tx.RFMode)
                                                        {
                                                            if (this->device == Tx.device)
                                                            {
                                                         if (this->FBGainState == Tx.FBGainState )
                                                         {
                                                            return this->delayVal > Tx.delayVal;
                                                         }
                                                                return this->FBGainState > Tx.FBGainState;
                                                            }
                                                                return this->device > Tx.device;
                                                            }
                                                            return this->RFMode > Tx.RFMode;
                                                        }
                                                    return this->RxScale > Tx.RxScale;
                                                    }
                                                    return this->uBias > Tx.uBias;
                                                }
                                                return this->FBgain > Tx.FBgain;
                                            }
                                       return this->Purpose > Tx.Purpose;
                                    }
                                       return this->ExpPwr > Tx.ExpPwr;
                                    }
                                    return this->PaQuiescentCurrent > Tx.PaQuiescentCurrent;
                                 }
                                 return this->EnvScale > Tx.EnvScale;
                              }
                              return this->IqGain > Tx.IqGain;
                           }
                           return this->RGI > Tx.RGI;
                        }
                        return this->XPTmode > Tx.XPTmode;
                     }
                     return this->PaState > Tx.PaState;
                  }
                  return this->Channel > Tx.Channel;
               }
               return this->bw > Tx.bw;
            }
               return this->Bias > Tx.Bias;
            }
            return this->Band > Tx.Band;
         }
         return this->Tech > Tx.Tech;
      }
} RFCal_Helper_Tx_Params;

typedef struct
{
   int               RF_Mode; // RF Mode represents Technology and Band. Some tech and band, RF mode is different from NV mode.
   int               Bias_RGI_Zero_Seg; // Bias value when RGI is set to zero. This value may be used at the end of a sweep.
      
        unsigned char       En_Debug_Mode;
   unsigned char     En_Compress_Esc_Req;
      unsigned char     En_Compress_Esc_Res; // Enable Results Compression Flag
   unsigned char     En_EPTET_Different_PAstate;
      unsigned char     En_EPTET_Different_Icq;
      unsigned char     Tx_Rx_Split;
      unsigned char     Set_CalState;
   XptInput_V1       PA_Param;   // Parameters in PA_PARAM.xml

   _NV_Generation_Params   NV_Generation_Params;
   byte           TxDeviceFlag; // This element is QLIB Demo only. This is not defined in a parameter XML.
   byte           RxDeviceFlag; // This element is QLIB Demo only. This is not defined in a parameter XML.
   typedef  vector<Sweep_Params> Sweep_Params_list;
   Sweep_Params_list Swp;
      vector < vector < cal_sweep_record > > * SwpRecords;
      vector < TE_Cal_Swp_Params > * TeRecords;

   map <APT_CharV3_Table_Key, vector < APTRec > > APTCharV3; // APT char table
   map <RFCal_Helper_Tx_Params, int> TxParamsToOffset; // Ref Cal data
   map <RFCal_Helper_Tx_Params, double> TxParamsToPower; // Ref Cal data
} Cal_Param;

 typedef struct Tx_RefCal_Data_V2_Record
{
    double pwr;
    word rxScale;
    word    FBGain;
    byte    uBias;
   byte FBGainState;
    Tx_RefCal_Data_V2_Record ():pwr(0),rxScale(0),FBGain(0),uBias(0),FBGainState(0){}
    bool operator < (const Tx_RefCal_Data_V2_Record & Tx) const 
    {
      if (this->rxScale == Tx.rxScale)
      {
          if (this->FBGain == Tx.FBGain)
          {
           if (this->uBias == Tx.uBias)
           {
              return this->FBGainState > Tx.FBGainState;
           }
              return this->uBias > Tx.uBias;
           }
              return this->FBGain >  Tx.FBGain;
          }
          return this->rxScale >  Tx.rxScale;
      }
} Tx_RefCal_Data_V2_Record;
typedef struct
{
   byte        TECH;
   byte        BAND;
   byte        Device;
   byte        Instance;
   byte        NumOfSweeps;
    byte                bCallboxType;
    byte                GPIB_BoardNumber;
    byte                GPIB_PrimaryAddress;
    byte                GPIB_SecondaryAddress;
   byte           bUseTearDownSweep; // Qlib_demo original. 
   byte           bDoNotUseEstimationForTxIntCal; //Qlib_demo original.
   vector <int>     NumOfSegments;
   string         stQMSLLogFileName;
   string           stXMLFileName;
   string         stHTMLFileName;
   string         stXSLFileName;
   string         stESC_Params_filename;
   string         stESC_Char_filename;
   string         stPA_PARAM_filename;
   string         stResultXML_filename;
    string          stRxCharDataXML_filename;
   string         stRefCalOffset_filename;
    string          stRFCalCharData_filename;
   string         stTestName;
    string          stDlfFileName; // .dmc file can be converted into .isf (QXDM file) using DLF converter on QXDM
    string          stDMCFilename; // QXDM Mask (.dmc file)
    string          stCalDBFilepath;
    string          stCalConfigName;
   unsigned char  bSimulationMode; // Simulation mode does not use an actual phone.
    unsigned char   bTESimulationMode; // Simulation mode for equipment
   unsigned char  bUseXMLresult;
   unsigned char  bCompressed;
   unsigned char  bDebugMsg;
   unsigned char  bDC_Enabled; // this is for demo purpose since default tree does not include.
    unsigned char   bUseSET_PATH_SEL_OVERRIDE;
    unsigned char   bEnableQMSLLogging;
    unsigned char   bEnableDLFLogging;
   MSXML2::IXMLDOMDocument2Ptr xmlDoc;
   MSXML2::IXMLDOMElementPtr TestPtr;
   HANDLE         hPhone;
   int            COM_Port;
   int            ThreadID;
} QLIB_Demo_RFCAL_ESC_ET_Param;

typedef struct
{
   int      offset;
   double   power;
   byte  rgi;
} Tx_RefCal_Data_Record;
typedef struct
{
   byte  pa;
   word  channel;
   vector <Tx_RefCal_Data_Record> record;
} Tx_RefCal_Data_Table;
typedef struct
{
   word  rf_mode;
   byte  device;
   vector <Tx_RefCal_Data_Table> table;
} Tx_RefCal_Data;

class rxOffsetRec
{
public:

   word channel;
   byte chain;
   byte lna;
   byte power_mode;
    byte RxCalType;

    rxOffsetRec(word _channel,byte _chain, byte _lna, byte _pm, byte _CalType = 0)
   {
      channel        = _channel;
      chain          = _chain;
      lna            = _lna;
      power_mode     = _pm;
    RxCalType       = _CalType;
   }
   // Since rxOffsetRec is used f or map, rxOffsetRec must be sorted. Need to implement operator < to enable sort.
   bool operator<(const rxOffsetRec& tmpRex) const
   {
      if (channel == tmpRex.channel)
      {
         if (chain == tmpRex.chain)
         {
            if (lna == tmpRex.lna)
            {
                if ( power_mode == tmpRex.power_mode)
                {
                    return RxCalType > tmpRex.RxCalType; 
                }
               return power_mode > tmpRex.power_mode;
            }
               return lna > tmpRex.lna;
            }
            return chain > tmpRex.chain;
         }
         return channel > tmpRex.channel;
      }
};
typedef pair<dword,byte> chanPAState_type; 
typedef map<byte,double> rgipwr_type;  
typedef struct RFCal_Helper_Exception : public std::exception
{
   string msg;
   RFCal_Helper_Exception(const string & _msg) : msg(_msg) {};
   RFCal_Helper_Exception(const string & _msg, const string & filename, int numline)
   {
      stringstream ss;
      ss << _msg << " [ Line " << numline << " in " << filename << " ]" << endl;
      msg = ss.str();
   };

   ~RFCal_Helper_Exception() throw () {} ;
   const char* what() const throw()
   {
      return msg.c_str();
   }
} RFCal_Helper_Exception;
typedef struct FB_LS_Error_Result
{
   signed short reLSEVal;
   signed short imLSEVal;
   unsigned short reLSE;
   unsigned short imLSE;
   unsigned short maxMagLSE;

} FB_LS_Error_Result;
               
typedef enum
{
   OVERLAP_MINUS = -1,
   NO_OVERLAP,
   OVERLAP_PLUS
} OVERLAP_MODE;


/* End: Helper function for ESC ET */

// ******************************************* //
// RFCal Helper function
// ******************************************* //

/* Create Initial segments */
void RFCal_Helper_ESC_ET_Initialize_Sweep(vector < cal_sweep_record > & Sweep, const Sweep_Params & sweep_param, int NumRecords, int TECH, int BAND);
/* Create Swp1 segments */
unsigned char RFCal_Helper_ESC_ET_createXPT1Swp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records);

/* Create Swp2 segments */
unsigned char RFCal_Helper_ESC_ET_createXPT2Swp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep1, vector < cal_sweep_record > & Sweep2);
/* Create Swp3 segments */
unsigned char RFCal_Helper_ESC_ET_createXPT3Swp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, Sweep_Params & sweep_param,
    const vector < cal_sweep_record > & Sweep1, const vector < cal_sweep_record > & Sweep2, vector < cal_sweep_record > & Sweep3, vector < TE_Cal_Swp_Params > & TE_Records);
/* Create FreqComp segments */
unsigned char RFCal_Helper_ESC_ET_createXPTFreqCompSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, Sweep_Params & sweep_param,
    const vector < vector < cal_sweep_record > > & Sweep, vector < cal_sweep_record >&      FreqComp_Sweep, vector < TE_Cal_Swp_Params > & TE_Records);
/* Create Delay segments */
unsigned char RFCal_Helper_ESC_ET_createXPTDelaySwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, Sweep_Params & sweep_param,
    const vector < cal_sweep_record > & Sweep2, vector < cal_sweep_record > & Sweep5);
/* Create IntraCA segments */
unsigned char RFCal_Helper_ESC_ET_createIntraCAFreqCompSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param,
     vector < cal_sweep_record > &      Sweep, vector < TE_Cal_Swp_Params > & TE_Records);
/* Create RxRSB Cal segments */
unsigned char RFCal_Helper_ESC_ET_createRxRSBCalSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records);
/* Create Ref Cal segments */
unsigned char RFCal_Helper_ESC_ET_createRefCalSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records);
/* Create INT Cal Swp1 segments */
unsigned char RFCal_Helper_ESC_ET_createINT_CAL_XPTSwp1(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records);
/* Create INT Cal Swp3 segments */
unsigned char RFCal_Helper_ESC_ET_createINT_CAL_XPTSwp3(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, Sweep_Params & sweep_param,
    const vector < cal_sweep_record > & Sweep1, const vector < cal_sweep_record > & Sweep2, vector < cal_sweep_record > & Sweep3, vector < TE_Cal_Swp_Params > & TE_Records);

/* Create INT Cal Freq Comp segments */
unsigned char RFCal_Helper_ESC_ET_createINT_CAL_XPTFreqCompSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, Sweep_Params & sweep_param,    const vector < vector < cal_sweep_record > > & AllSweeps/* vector < cal_sweep_record >  APT_Sweep */, vector < cal_sweep_record >&      Sweep, vector < TE_Cal_Swp_Params > & TE_Records);
/* Create XPT Pin Cal segments */
unsigned char RFCal_Helper_ESC_ET_createXPTPinCalSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep2, vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records);
/* Seperate Tx and Rx sweeps (for Rx non sweep mode) */
unsigned char RFCal_Helper_SeperateTx_RxSweeps(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, Sweep_Params & sweep_param, vector < TE_Cal_Swp_Params > & TE_Records);
/* Seperate CA sweeps (for CA and DRDSDS) */
unsigned char RFCal_Helper_Seperate_CA_Sweeps(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, vector < vector < cal_sweep_record > > & Sweep, UINT iSweepIndex, vector < TE_Cal_Swp_Params > & TE_Records);


/* Execute Serial Rx Measurement */
unsigned char RFCal_Helper_ESC_Execute_Serial_Rx(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, Sweep_Params & sweep_param,
    vector < cal_sweep_record >&        Sweep, vector < TE_Cal_Swp_Params > & TE_Records);

/* Post computation */
unsigned char RFCal_Helper_ESC_ET_PostComputation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, const Sweep_Params & sweep_param,vector < cal_sweep_record >  &   Sweep, vector < TE_Cal_Swp_Params > & TE_Records);

/* NV Generation */
unsigned char RFCal_Helper_ESC_ET_NV_Generation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, vector < vector < cal_sweep_record > > & Sweep);

/* Tx NV Generation */
unsigned char RFCal_Helper_ESC_ET_TX_NV_Generation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, vector < vector < cal_sweep_record > > &   Sweep);
/* Rx NV Generation */
unsigned char RFCal_Helper_ESC_ET_RX_NV_Generation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, vector < vector < cal_sweep_record > > &   Sweep);

/* thread execution function */
unsigned __stdcall RFCal_Helper_ESC_ET_threadFunction(void * parameters);
/* Multi DUT cal demo. */
void RFCal_Helper_Demo_MultiDUT_Cal(vector < QLIB_Demo_RFCAL_ESC_ET_Param > & demo_param);
/* Parameter assignment. obsolete function */
unsigned char RFCal_Helper_ESC_ET_Param_Assignment(QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & param);// no longer used. just for demo.
/* Parameter assignment function for Param.xml */
unsigned char RFCal_Helper_ESC_ET_Param_Assignment_XML(QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & param);
/* Parameter assignment function for char data xml */
unsigned char RFCal_Helper_ESC_ET_CharData_Assignment_XML(QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param& param);
/* Parameter assignment function for PA_PARAM.xml */
unsigned char RFCal_Helper_ESC_ET_PA_PARAM_Assignment_XML(QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param& param);
/* Parameter assignment function for Ref Cal/Tx Internal Cal */
unsigned char RFCal_Helper_ESC_ET_RefCal_Tx_Data_Assignment_XML(QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & para);
/* Parameter assignment function for Multiband Rx Cal */ 
unsigned char RFCal_Helper_ESC_ET_Rx_Char_Data_Assignment_XML(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, const Sweep_Params & sweep_param, map <RFCal_Helper_Rx_Params, short> & RxParamsToOffset);
/* Parameter assignment function for Multiband Rx Cal */ 
unsigned char RFCal_Helper_ESC_ET_Tx_AptChar_DataV6_Assignment_XML(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, map <RFCal_Helper_Tx_Params, double> & TxParamsToPwr);
unsigned char RFCal_Helper_ESC_ET_Tx_AptChar_DataV6_Assignment_XML(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, map <RFCal_Helper_Tx_Params, double> & TxParamsToPwr, map< chanPAState_type, rgipwr_type> & refcalchanPa_rgi_pwr);
;
unsigned char RFCal_Helper_UpdateRFCalParamsFromCharFile(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, Sweep_Params & sweep_param);

/* RFCal Logging main function */
unsigned char RFCal_Helper_ESC_ET_Logging(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep);
/* RFCal Logging function for Ref Cal */
unsigned char RFCal_Helper_ESC_ET_Logging_RefCal(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep);
/* RFCal Logging function for RSB Cal */
unsigned char RFCal_Helper_ESC_ET_Logging_RsbCal(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep);
/* RFCal Logging function for Sweep 1 */
unsigned char RFCal_Helper_ESC_ET_Logging_XPT_Swp1(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep);
/* RFCal Logging function for Sweep 2 */
unsigned char RFCal_Helper_ESC_ET_Logging_XPT_Swp2(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep);
/* RFCal Logging function for Sweep 3 */
unsigned char RFCal_Helper_ESC_ET_Logging_XPT_Swp3(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep);
/* RFCal Logging function for FreqComp */
unsigned char RFCal_Helper_ESC_ET_Logging_XPT_FreqComp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep);
/* RFCal Logging function for Delay Sweep */
unsigned char RFCal_Helper_ESC_ET_Logging_XPT_Delay(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep);

/* Dummy result assignment function */
void RFCal_Helper_ESC_ET_Dummy_Result_Assignment(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, vector < cal_sweep_record > & Sweep);
/* Index Increment function */
unsigned char RFCal_Helper_RecordIndex_Increment(vector < cal_sweep_record > & Sweep, UINT & recordIndex);
/* Index Increment function */
unsigned char RFCal_Helper_RecordIndex_Increment(vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records, UINT & recordIndex);
/* FBGain calculation function for Ref cal */
unsigned char RFCal_Helper_ESC_ET_RefCal_CalculateOffset(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep,
                                                         Tx_RefCal_Data & data);
/* Assign FBGain Cal result to a map of TxParamToPwr */
unsigned char RFCal_Helper_ESC_ET_RefCal_FBGainDataAssignment(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep, 
   map <RFCal_Helper_Tx_Params, double> & mapTxParmasToPwr);

/* Pre Computation */
unsigned char RFCal_Helper_ESC_ET_PreComputation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, Sweep_Params & sweep_param,vector < cal_sweep_record >   &   Sweep, UINT iSwpIndex);
unsigned char RFCal_Helper_ESC_ET_FBGain_Ref_Assignment_XML(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, map < pair < dword, byte >, Tx_RefCal_Data_V2_Record > & ChanFBGainStateToPwrFBGainParams);
unsigned char RFCal_Helper_ESC_ET_PostComputation_FBGainPowerComputation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, const Sweep_Params & sweep_param,vector < cal_sweep_record >   &   Sweep);
unsigned char RFCal_Helper_setOpCode_FBMeasurement(const Sweep_Params & sweep_param, cal_sweep_record & record, byte pa_state, byte rgi, byte refRGI, int overlapMode,   
   map < RFCal_Helper_Tx_Params, double > & APTV6CharToPwr,
   map < pair < dword, byte >, Tx_RefCal_Data_V2_Record > & ChanFBGainStateToPwrFBGainParams, double dEstimatedPwr = -115.0);
unsigned char RFCal_Helper_CalculateUKernelParameters(double inputCharPwr,  double inputRefCalPwr, word inputRxScale, byte inputuBias, word & outputRxSCale, byte & outputuBias, double RefFBGain = 0);
unsigned char RFCal_Helper_Calculate_FB_LSError_Result( dword fbLSError, FB_LS_Error_Result & _fbLSErrorStruct);
unsigned short RFCal_Helper_Calculate_FB_LSError_Result( dword fbLSError);

/* Ref cal data writing function */
unsigned char RFCal_Helper_WriteRefCalTxDataToXML(const string & filename, const Tx_RefCal_Data & data);
/* Ref cal data writing V2 function */
unsigned char RFCal_Helper_WriteRefCalTxData_V2_ToXML(const string & filename, const map <RFCal_Helper_Tx_Params, double> & mapTxParmasToPwr);
unsigned char RFCal_Helper_ESC_ET_PreComputation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, Sweep_Params & sweep_param,vector < vector < cal_sweep_record > > &    Sweep, vector < TE_Cal_Swp_Params > & TE_Records, UINT iSwpIndex);
unsigned char RFCal_Helper_LoadMeasTxPwrFromSpecifiedSweep( vector < cal_sweep_record > & currSweep, byte devId, byte pa, int rgi, byte XPTMode, bool is_XPT_Measurement, double& charTxPwr, bool& status, Tx_RefCal_Data_V2_Record & preFBGainParam, byte FBGainState=255);
unsigned char RFCal_Helper_Execute_TearDown_Sweep(HANDLE hPhone, const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param);
unsigned char RFCal_Helper_LoadMeasTxPwrFromSpecifiedSweepForXPTFreqComp( vector < cal_sweep_record > & currSweep, byte pa, int rgi, byte XPTMode, bool is_XPT_Measurement, double& charTxPwr, bool& status, Tx_RefCal_Data_V2_Record & preFBGainParam, byte FBGainState);

/* TE related helper function for ESC Cal V4 */
unsigned char RFCal_Helper_Intialize_TE_Records(vector < TE_Cal_Swp_Params > & TE_Records);
/* TE related helper function for ESC Cal V4 - Set UL channel for all the segments*/
unsigned char RFCal_Helper_Config_TE_ULChannels(const vector < cal_sweep_record > & sweep, vector < TE_Cal_Swp_Params > & TE_Records);

// ******************************************* //
// RFCal XML related helper
// ******************************************* //

template <typename T> unsigned char RFCal_Helper_Convert_Str_To_Value(const string & stValue, T & value);
/* Read attributes on the node */
template <typename T> unsigned char RFCal_Helper_Read_Attribute(MSXML2::IXMLDOMNodePtr parentNode,const string & stAttribute, T & parameter);
/* Read attributes on XPATH from the node*/
template <typename T> unsigned char RFCal_Helper_Read_Attribute(MSXML2::IXMLDOMNodePtr parentNode,const string & XPath, const string & stAttribute, T & parameter);
/* Read 2-D vector type parameters on XPATH from the node*/
template <typename T> unsigned char RFCal_Helper_Read_Param(MSXML2::IXMLDOMNodePtr parentNode,const string & XPath, vector < vector <T> > & parameter);
/* Read 1-D vector type parameters on XPATH */
template <typename T> unsigned char RFCal_Helper_Read_Param(MSXML2::IXMLDOMNodePtr parentNode,const string & XPath, vector <T> & parameter);
/* Read parameter on XPATH */
template <typename T> unsigned char RFCal_Helper_Read_Param(MSXML2::IXMLDOMNodePtr parentNode,const string & XPath, T & parameter, T defaultValue = 0);
/* Read result data type = 0 is input node, type = 1 is output node */
template <typename T> unsigned char RFCal_Helper_Read_Result(MSXML2::IXMLDOMNodePtr parentNode,const int type, const string & stAttribute, T & parameter);
unsigned char RFCal_Helper_AddTestName(MSXML2::IXMLDOMDocument2Ptr & xmlDoc, MSXML2::IXMLDOMElementPtr & TestPtr, const string & stTestType, const string & stTestName, const string & stNodeName);
unsigned char RFCal_Helper_SetUUT_BuildID(MSXML2::IXMLDOMDocument2Ptr & xmlDoc, const string & stUUTID, const string & stBuildID);
unsigned char RFCal_Helper_SetTime_Duration(MSXML2::IXMLDOMDocument2Ptr & xmlDoc, MSXML2::IXMLDOMElementPtr & TestPtr, dword iDuration,const time_t & starTime_t,const time_t & endTime_t);
MSXML2::IXMLDOMElementPtr RFCal_Helper_DataSet( MSXML2::IXMLDOMDocument2Ptr xmlDoc, MSXML2::IXMLDOMElementPtr DataCollectionElement, MSXML2::IXMLDOMElementPtr DataSetElementPtr, unsigned char bInput, string ValueN, string ValueV, unsigned char bStatus = true);

template<class T> MSXML2::IXMLDOMElementPtr RFCal_Helper_DataSet( MSXML2::IXMLDOMDocument2Ptr xmlDoc, MSXML2::IXMLDOMElementPtr DataCollectionElement, MSXML2::IXMLDOMElementPtr DataSetElementPtr, unsigned char bInput, string ValueN, T ValueV, unsigned char bStatus = true);


MSXML2::IXMLDOMElementPtr  RFCal_Helper_AddNewTextElement( MSXML2::IXMLDOMDocument2Ptr xmlDoc, MSXML2::IXMLDOMElementPtr baseElement, string newElementName, string Text);
MSXML2::IXMLDOMElementPtr RFCal_Helper_AddTest( MSXML2::IXMLDOMDocument2Ptr xmlDoc, MSXML2::IXMLDOMElementPtr baseElement, string newTestName, string newTestExtendedName, string newTestNodeName);
unsigned char RFCal_Helper_SetRunInfo(MSXML2::IXMLDOMDocument2Ptr & xmlDoc, MSXML2::IXMLDOMElementPtr & TestPtr,
   const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, dword iDuration,const time_t & starTime_t,const time_t & stopTime_t);

//unsigned char ExecuteXSLT(const string & XMLfilename,const string & htmlname,const  string & xslfilename);
unsigned char RFCal_Helper_ExecuteXSLT(const string & XMLfilename,const string & htmlname,const  string & xslfilename);



unsigned char RFCal_Helper_Generate_XMLReport(string stXMLName, MSXML2::IXMLDOMDocument2Ptr & xmlDoc);


MSXML2::IXMLDOMNodeListPtr RFCal_Helper_GetXMLNodes(MSXML2::IXMLDOMNodePtr  pNode, const string & stXPATH);
MSXML2::IXMLDOMNodePtr RFCal_Helper_GetSingleXMLNode(MSXML2::IXMLDOMNodePtr  pNode, const string & stXPATH);

string RFCal_Helper_Convert_BSTR_to_String(BSTR & bst);

// ******************************************* //
// RFCal OpCode helper
// ******************************************* //
/* 0x0 Nop */
void RFCal_helper_InsertNop( vector < cal_sweep_record > & Sweep, UINT & recordIndex, int NumNop, int div = 1);
/* 0x100: Radio Setup */
unsigned char RFCal_Helper_setOpCode_RadioSetup(cal_sweep_record& tmp, byte masterDev, byte slaveDev, byte masterDevState, byte slaveDevState, byte TECH, word BAND, dword ulChan,  byte txWaveform = 0,
                                                dword offset = NULL, byte numRBs = NULL, byte startRBs = NULL, word eulConfig = NULL, dword RxBandwidth = 3, dword TxBandwidth = 3, unsigned char flag = true);
/* Config_time and add Nop Segment */
unsigned char RFCal_Helper_setConfigTime_InsertNop( vector < cal_sweep_record > & Sweep, UINT & recordIndex, dword config_time, word maxConfigSegmentLength = 60000);

/* 0x200: TxOverride */
unsigned char RFCal_Helper_setOpCode_TxOverride(cal_sweep_record& tmp, byte TxPurpose, byte TxDeviceFlag, byte xptMode, dword txOverrideMask, byte pwrOverrideType, dword pwrParamMask,
                                                byte PA_State, byte RGI, dword BIAS,  byte IQ_Gain_Action, word IQ_Gain,  byte Env_Scale_Action,word Env_Scale, word PA_Quiescent_Current, byte delayAction, byte delayValue,
                                                word PwrdBm = NULL, dword refChan = NULL, word dpdIndex = NULL);
/* 0x200: TxOverride. Set low RGI segment at the end of sweep*/
unsigned char RFCal_Helper_setLowRGISegment(cal_sweep_record& tmp, byte TxDeviceFlag);
/* 0x800: Data */
unsigned char RFCal_Helper_setOpCodeData(cal_sweep_record& tmp, const Cal_Param & cal_param, const Sweep_Params & sweep_param,  int iDataOption,int TECH, int BAND, unsigned char bEPT = false);
/* 0x800: Data - Unity DPD */
unsigned char RFCal_helper_setOpCodeData_UnityDPD( cal_sweep_record& tmp, const Cal_Param & cal_param,const Sweep_Params & sweep_param,int TECH, int BAND, unsigned char bEPT = false);
/* 0x800: Data - Load Calc DPD */
unsigned char RFCal_helper_setOpCodeData_LoadCalcDPD( cal_sweep_record& tmp, const Cal_Param & cal_param,const Sweep_Params & sweep_param, int TECH, int BAND, unsigned char bEPT = false);
/* 0x1000: Sync */
unsigned char RFCal_Helper_setOpCode_Sync(cal_sweep_record& tmp, byte txDeviceId = 0);
/* 0x2000: XPT2 */
unsigned char RFCal_Helper_setOpCode_XPT2(cal_sweep_record& tmp, dword xptAction, byte cfg_dev, byte cfg_fbDev, byte cfg_xptMode,
                                          byte capType = NULL, word numSamples = NULL, byte firstTrig = NULL, byte lastTrig = NULL, byte procFlag = NULL);
/* 0x4020: RxConfig (= 0x20 RxConfig + 0x4000 RxMeas) */
unsigned char RFCal_Helper_setOpCode_RxConfigParameters(cal_sweep_record& tmp, int Rx_Action,byte rxFlag, byte rxPM,
    word expRxAGC, byte rx_meas_action, unsigned short rxCalType = 4, unsigned short* pRxPath = 0, unsigned short arraySize = 0);
/* 0x8000: Tx Measure */
unsigned char RFCal_Helper_setOpCode_TxMeasure(cal_sweep_record& tmp, byte txDevice, word txMeasAction, byte numHdetAvg = NULL, byte numLpmHdetAvg = NULL,
    byte XPTMode = NULL, word GainComp = NULL, byte fbGainState = NULL, byte UBias = NULL, word Rx_Scale = NULL, byte DebugModeEnable = NULL, byte  numSamples = 1);
/* 0x8000: Tx Measure fbGain Params for WTR 3925 */
unsigned char RFCal_Helper_setOpCode_txMeas_fbGainParams( cal_sweep_record& tmp,    byte txDevice,    byte xptMode,    word gaincomp,    byte fbGainState,    byte uBias,    int rxScale,
   byte debugModeEn, byte bMeasFBGainTherm = 0);
/* 0x10000: Feedback Setup */
unsigned char RFCal_Helper_setOpCode_FB_Setup(cal_sweep_record& tmp, byte PrimaryDev, byte fbDev, word fbAction, byte fbEnable = false, byte fbGainState = NULL,  byte paState = NULL, byte refRGI = NULL);
/* 0x10000: Feedback Setup : Configure RxLM */
unsigned char RFCal_Helper_ESC_ET_OpCodeConfigureRxLM(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, vector < cal_sweep_record > & Sweep, UINT & recordIndex, unsigned char fbEnabled);
/* 0x10000: Feedback Setup : Configure FB path */
unsigned char RFCal_Helper_ESC_ET_ConfigureFB( cal_sweep_record & record, unsigned char pDev, unsigned char bEnableFBmode, unsigned char & FBmode, int fbDev, int fbGainState, byte pa_state, byte rgi);
/* 0x20000: Enh Int Device Cal for FBRxDC Cal*/
unsigned char RFCal_Helper_setOpCode_EnhIntDevCal(cal_sweep_record& tmp, const Cal_Param & cal_param,const Sweep_Params & sweep_param, int TECH, int BAND);
/* 0x20000: Enh Int Device Cal for FBRx/RxRSB Cal*/
unsigned char RFCal_Helper_setOpCode_EnhIntDevRSBCal(cal_sweep_record& tmp, const Cal_Param & cal_param,const Sweep_Params & sweep_param, int TECH, int BAND, byte bFBRxRSB, unsigned long int_cal_type_mask,  byte gainStateListIndex, byte bSCell = 0);
unsigned char RFCal_Helper_Is_FreqComp_SweepType( word sweep_type);
unsigned char RFCal_Helper_Is_IntCal_SweepType( word sweep_type);
// ******************************************* //
// QLib helper functions
// ******************************************* //
/* QLIB_FTM_GENERATE_ESC_OPCODE */
unsigned char RFCal_Helper_setOpcode(HANDLE hPhone, vector< cal_sweep_record > & Sweep, int recordIndex);
/* QLIB_FTM_CFG_AND_EXEC_ESC_SWEEP */
unsigned char RFCal_Helper_ExecEscSweep(HANDLE hPhone, const vector< cal_sweep_record > & Sweep, int recordIndex, bool bIsLastSweep, unsigned char bCompressed, unsigned char bDebug);
/* QLIB_FTM_GET_ESC_SWEEP_RESULTS */
unsigned char RFCal_Helper_GetEscSweepResults(HANDLE hPhone, vector < cal_sweep_record > & Sweep, unsigned char bCompressedResponse, unsigned char bCallBoxMeasurement);
/* QLIB_RFCAL_NV_Manager_BackupNVItems */
unsigned char RFCal_Helper_WriteNVToQCN(HANDLE hPhone, const string & filepath, unsigned char bWriteToDUT = false);
/* Change FTM mode */
unsigned char RFCal_Helper_Change_FTM_Mode(HANDLE hPhone, int count = 10, int sleepTime = 500);
/* Convert UL to DL channel */
long RFCal_Helper_ConvertULchtoDLch(int tech, int band, long ulChannel);
/* Check a given band is TDD band or not */
unsigned char RFCal_Helper_isTDDBand(byte tech, byte band);
/* Assign UL and DL frequency given UL (DL channel) */
unsigned char RFCal_Helper_CalculateFrequencyFromULChan(int TECH, int band, int ULChannel, double & ULFrequency, double & DLFrequency);
/* Set a QLib System mode given tech and RxPath */
unsigned char RFCal_Helper_FTM_SET_MODE_ID(HANDLE hPhone, byte Tech, byte RxPath);
/* Execute FTM_SET_MODE after setting FTM_SET_MODE_ID */
unsigned char RFCal_Helper_FTM_SET_MODE(HANDLE hPhone, FTM_RF_Mode_Enum mode, byte Tech, byte RxPath);
/* Execute FTM_SET_SECONDARY_CHAIN after setting FTM_SET_MODE_ID */
unsigned char RFCal_Helper_FTM_SET_SECONDARY_CHAIN(HANDLE hPhone, byte Tech, byte isMasterDev, byte RxPath);
/* Get Rx Cal Type */
unsigned short RFCal_Helper_GetRxCaltype( const cal_sweep_record * pSweepRecord, unsigned int rxChain );
unsigned short RFCal_Helper_GetRxCaltype( unsigned short swpType, unsigned int rxMeasAction, bool measCA );
template<typename T> T RFCal_Helper_GetIndexFromBitMask(T bitMask, T MaxIndex = 0);

// ******************************************* //
// QMSL Log helper functions
// ******************************************* //
/* Add messages on QMSL log */
void RFCal_Helper_AddMessageToQMSLLog(HANDLE hPhone, const string & Log, unsigned char bShowConsole = true);
/* Add messages on QMSL log */
void RFCal_Helper_AddSectionTitleToQMSLLog(HANDLE hPhone, const string & Log, unsigned char bShowConsole = true);
/* Show build ID on QMSL log */
unsigned char RFCal_helper_Log_Build_ID(HANDLE hPhone);
/* Show build ID on QMSL log and get build ID*/
unsigned char RFCal_helper_Log_Build_ID(HANDLE hPhone, string & stBuildID);

long CalculateRxAGCValue(byte Tech, double RxPower, double DynamicRange = 102.3);

template<typename Tx> unsigned char RFCal_Helper_EvaluateResult(Tx iResult, Tx iMaxLimit, Tx iMinLimit = static_cast<Tx>(0), unsigned long iOption = 0);

/* Helper Function */
void Test_RFCal_ESC_InitRecord(cal_sweep_record* tmpSweep,int swpType,int subSwpType,int txPurpose,   byte tech, word band, int txWaveform, int channel,int txSweepTime, int startRB, int numRBs, int Tx_BW, int Rx_BW);

int Get_Closet_Input_From_Matrix_WithTargetPwr(double *pwrList, int *inputList, int num, double targetPwr);

void Find_Y_of_Next_Highest_X( const map< double, unsigned long >& xy_map,
                               const double& x, unsigned long& y);
template<typename Tx, typename Ty> void Find_Interpolated_Y( const map< Tx, Ty >& xy_map,
                                                             const Tx& x, Ty& y);
template<typename Tx, typename Ty> void RFCal_Helper_GetEstimatedY_LeastSquaresMethod( const map< Tx, Ty >& xy_map,
   const Tx& x, Ty& y);
template<typename Tx, typename Ty> void RFCal_Helper_GetEstimatedY_LeastSquaresMethod( const map< Tx, Ty >& xy_map,
   const Tx& x, Ty& y, double & a, double & b);
template<typename Tx, typename Ty> void RFCal_Helper_GetEstimatedY_LeastSquaresMethod_quadratic( const map< Tx, Ty >& xy_map,
   const Tx& x, Ty& y, double & a, double & b, double & c);

void Cfg_ESC_Tx_Mask_ForFirstDeivce(cal_sweep_record &tmp,ESC_TX_MASK_TYPE type);
void Test_RFCal_ESC_InitRecord(cal_sweep_record* tmpSweep,int swpType,int subSwpType,int txPurpose,   byte tech, word band, int txWaveform, int channel,int txSweepTime, int startRB, int numRBs, int Tx_BW, int Rx_BW);
void Test_RFCal_ESC_ET_CAL(HANDLE hPhone, int iOption, unsigned char bSimulationMode, unsigned char bTESimulationMode, int numLoop = 1);



typedef struct 
{
    byte    FB_Droop_Method;
    byte    Tx_Device;
    byte    Fb_Rx_Gain_State;
    vector <byte> Lte_FBRxTech_Bw_List;
    dword   Chan_Lte_10M;
    dword   Chan_Lte_15M;
    vector <double> stg_freq_offset_kHz_list;
    vector <double> stg_freq_offset_kHz_list_Lte10M;
    vector <double> stg_freq_offset_kHz_list_Lte15M;
    int     Coeff_Min_Lim;
    int     Coeff_Max_Lim;
    double  Tone_Pwr_Min_Lim_dB;
    double  Tone_Pwr_Max_Lim_dB;
    byte    NV_is_band_specific;

//  byte    Num_Captures;
    word    Num_IQ_Samples;
    byte    Enable_Processing;
    byte    Verify_Cal;
    byte    Verification_Method;
    vector <word>   Verification_Freq_Offsets;
    vector <word>   LTE5_Droop_PP_Limit_dB100;
    vector <word>   LTE10_Droop_PP_Limit_dB100;
    vector <word>   LTE15_Droop_PP_Limit_dB100;
    vector <word>   LTE20_Droop_PP_Limit_dB100;
    vector <double> Sampl_Freq_List_Per_BW_Mode_MHz;
    vector <double> Band_Edge_List_Per_BW_Mode_MHz;
    double Residual_Droop_Max_Lim_dB;
    double Residual_Droop_Min_Lim_dB;
} _FbRx_Droop_Cal;
typedef struct
{
   vector <byte> Fb_Rx_Gain_State_List;
   word  Num_IQ_Samples;
   vector <int>   Min_DC_I_Limit;
   vector <int>   Max_DC_I_Limit;
   vector <int>   Min_DC_Q_Limit;
   vector <int>   Max_DC_Q_Limit;
} _FbRx_DC_Cal;

typedef struct
{
   byte  Rsb_Type;
   byte  Signal_Source;
   word  Waveform_Offset_in_kHz;
   byte  Number_Of_Gain_States;
   vector <byte> Rx_Gain_State_List;
   vector <int> Power_List;
   byte  IQ_Capture_Type;
   word  Num_IQ_Samples;
   byte  Carrier_To_Be_Calibrated;
   byte  SCellMode;
   byte  PCellModeForSCell;
   byte  PCellBand;
   dword PCellRefChannel;
   word  Post_Cal_Rsb_Limit;
   word  Pre_Cal_Rsb_Limit;
   byte  Do_Rsb_Limit_Check;
   word  WriteToRsb_efsFile;
} _Rx_FbRx_RSB_Cal;

/* Base XTT class */

typedef struct
{
   byte  RF_Mode;
   byte  NV_Mode;
   byte  NV_Action;
   byte  Device_Mask;
   vector<word> ChannelList;
   unsigned char bSetCalibrationState;
   byte  CalibrationMode;  // default: composite
   //_DeviceSelection DeviceSelection;
   //_LTE      LTE;
   //_RxTx     RxTx;
   //_Tx       Tx;
   //_Rx       Rx;
   //_NVGeneration NVGeneration;
} XTT_Param;

/* Extended XTT class */

typedef struct XTT_Enh_Internal_Dev_Cal_Param : XTT_Param
{
   word     iChannel;
   byte     TxBandwidth;
   byte     iPAstate;
   byte     TxGainIndex;
   word     iBiasValue;
   byte     Internal_Cal_Type_Mask;
   byte  Technology;
   byte  Band;
   byte     is_Debug_Mode_Enabled;
   int         FBRx_Tech_Bandwidth_Config;
    _FbRx_Droop_Cal FbRx_Droop_Cal;
   _FbRx_DC_Cal FbRx_DC_Cal;
   _Rx_FbRx_RSB_Cal Rx_FbRx_RSB_Cal;
} XTT_Enh_Internal_Dev_Cal_Param;

// XTT_Enh_Internal_Dev_Cal_Param
typedef enum
{
   ENH_INTERNAL_DEVICE_CAL_FBDROOP_BIT = 0,
   ENH_INTERNAL_DEVICE_CAL_GROUP_DELAY_BIT = 1,
   ENH_INTERNAL_DEVICE_CAL_IP2_BIT = 2,
   ENH_INTERNAL_DEVICE_CAL_DC_BIT = 3,
   ENH_INTERNAL_DEVICE_CAL_RSB_BIT=4,
   ENH_INTERNAL_DEVICE_CAL_MAX = 5
} ftm_enh_internal_device_cal_bit_type;

// ******************************************* //
// Enh Internal devicel cal
// ******************************************* //
/* Main function of Enh Internal devicel cal */
void Test_RFCal_Enh_Internal_Device_Cal(HANDLE hPhone, int TECH, int BAND, unsigned char isSCell, int int_cal_type_mask, const string & XTTFilename, unsigned char bSimulationMode);

/* Read XTT parameters given tech/band/int_cal_type_mask\ */
void RFCal_Helper_Enh_Internal_Device_Param_Assigment_From_XTT(HANDLE hPhone,  int TECH, int BAND, unsigned char isSCell, int int_cal_type_mask,  const string & XTTFilename, XTT_Enh_Internal_Dev_Cal_Param & params);
/* Assign parameters into Enh Internal device cal request parameters */
void RFCal_Helpder_Enh_Internal_Device_Request_Param_assignment(const XTT_Enh_Internal_Dev_Cal_Param & params, common_enh_internal_device_cal & enh_internal_device_cal);
/* Output Tx power for Enh Int cal */
unsigned char RFCal_Helper_Enh_Internal_Device_TxOverride(HANDLE hPhone, const XTT_Enh_Internal_Dev_Cal_Param & params, const common_enh_internal_device_cal & enh_internal_device_cal, unsigned char bSetMode = true);
/* NV Generation for Enh Int cal */
unsigned char RFCal_Helper_Enh_Internal_Device_Cal_NVGeneration(HANDLE hPhone,const XTT_Enh_Internal_Dev_Cal_Param & params, const common_enh_internal_device_cal & enh_internal_device_cal,
                                                                common_enh_internal_device_resp & enh_internal_device_res);
/* NV Generation for Enh Int cal FBRx Droop cal */
unsigned char RFCal_Helper_Enh_Internal_Device_Cal_FBRx_Droop_NVGeneration(HANDLE hPhone, const XTT_Enh_Internal_Dev_Cal_Param & params, const common_enh_internal_device_cal & req_10MHz, const common_enh_internal_device_cal & req_20MHz, 
     common_enh_internal_device_resp & resp_10MHz,   common_enh_internal_device_resp & resp_20Mhz, QMSL_FBRX_DROOP_CAL_NV_RESULT & FBRX_DROOP_NV);
/* NV Generation (Per Band) for Enh Int cal FBRx Droop cal*/
unsigned char RFCal_Helper_Enh_Internal_Device_Cal_FBRx_Droop_PerBand_NVGeneration(HANDLE hPhone, const XTT_Enh_Internal_Dev_Cal_Param & params, const common_enh_internal_device_cal & req_10MHz, const common_enh_internal_device_cal & req_20MHz, 
     common_enh_internal_device_resp & resp_10MHz,   common_enh_internal_device_resp & resp_20Mhz, QMSL_RFNV_FBRX_DROOP_PER_BAND_NV_RESULT & FBRX_DROOP_NV_V2);
/* Enh Internal device Cal Verificaiton */
unsigned char RFCal_Helper_Enh_Internal_Device_Cal_Verification(HANDLE hPhone,const XTT_Enh_Internal_Dev_Cal_Param & params, const common_enh_internal_device_cal & enh_internal_device_cal,
                                                                const common_enh_internal_device_resp & enh_internal_device_res);
/* Enh Internal device Cal Verificaiton for FBRx_Droop_Filter */
unsigned char RFCal_Helper_Enh_Internal_Device_Cal_Verify_FBRx_Droop_Filter(HANDLE hPhone, const XTT_Enh_Internal_Dev_Cal_Param & params, const common_enh_internal_device_cal & req_10MHz, const common_enh_internal_device_cal & req_20MHz, 
    const common_enh_internal_device_resp & resp_10MHz,  const common_enh_internal_device_resp & resp_20Mhz);
/* Enh Internal device Cal Verificaiton for FBRx_Rsb_Cal */
void RFCal_Helper_Enh_Internal_Device_Cal_Verify_rsb_cal(unsigned short dev_mask, const common_enh_internal_device_resp & enh_internal_device_resp, const XTT_Enh_Internal_Dev_Cal_Param params);

template<typename T> unsigned char RFCal_Helper_Read_Attribute(MSXML2::IXMLDOMNodePtr parentNode,const string & stAttribute, T & parameter)
{
   if (parentNode == NULL)
   {
      parameter = static_cast<T>(0);
      return false;
   }
    //MSXML2::IXMLDOMElementPtr pParentElement(parentNode);
    //_variant_t varValue = pParentElement->getAttribute((_bstr_t)(stAttribute.c_str()));
    //parameter = static_cast<T>(varValue);
    //return true;
    MSXML2::IXMLDOMNamedNodeMap* pParentAttributes;
    parentNode->get_attributes(&pParentAttributes);
    MSXML2::IXMLDOMNodePtr pCategoryNameNode = pParentAttributes->getNamedItem((_bstr_t)(stAttribute.c_str()));   
    if (pCategoryNameNode!=NULL)
    {
        _variant_t varValue;
        pCategoryNameNode->get_nodeValue(&varValue);
   parameter = static_cast<T>(varValue);
   return true;
    }
    //varValue = pParentElement->getAttribute((_bstr_t)(stAttribute.c_str()));
    else
    {
        parameter = static_cast<T>(0); 
        return false;
    }

}
template<typename T> unsigned char RFCal_Helper_Read_Attribute(MSXML2::IXMLDOMNodePtr parentNode,const string & XPath, const string & stAttribute, T & parameter)
{
   if(parentNode == NULL)
   {
      return false;
   }
   //**************************************************//
   //       Get a text from given XPath node
   //**************************************************//
   MSXML2::IXMLDOMNodePtr pXPathNode = parentNode->selectSingleNode((_bstr_t)(XPath.c_str()));
   if (pXPathNode == NULL) return false;
   MSXML2::IXMLDOMElementPtr pParentElement(pXPathNode);
   _variant_t varValue = pParentElement->getAttribute((_bstr_t)(stAttribute.c_str()));
   parameter = static_cast<T>(varValue);
   pXPathNode.Release();
   return true;
}

unsigned char Test_RFCal_TxRsb_TxLo_Internal_Cal();
unsigned char Test_RxRSB_Cal_Enh_Internal_Device_Cal();
unsigned char Test_FBDC_Cal_Enh_Internal_Device_Cal();
unsigned char Test_FB_Droop_Cal_Enh_Internal_Device_Cal();

unsigned char Test_XPT_Capture_Sweep();