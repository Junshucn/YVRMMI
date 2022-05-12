/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/RFCal_Enum.h#27 $
 * $DateTime: 2017/09/11 22:55:00 $
 *
*
******************************************************************************
*
* Copyright (c) 2016 Qualcomm Technologies, Inc.
* All rights reserved.
* Qualcomm Technologies, Inc. Confidential and Proprietary.
*
 ******************************************************************************
 */

#pragma once
// This file can be included to get data types, structures, and enumeration defintions
// It is not manditory though because all of the libraries interfaces are standard ANSI C
// data types.

//#pragma warning (disable:4503) // To avoid warning for map of map.

#ifndef __QLIB_DEFINES_H
#define __QLIB_DEFINES_H
#include "QLib_Defines.h"
#endif
#include < set >
#include < map >

using namespace std;

namespace RFCal
{
	enum RFCAL_NV_ACTION_TYPE
	{
		RFCAL_NV_ACTION_QUEUE_NV    = 0x01,
		RFCAL_NV_ACTION_SELF_WRITE  = 0x02,
		RFCAL_NV_ACTION_ENABLE_LOG  = 0x04,
		RFCAL_NV_ACTION_MAX,
		RFCAL_NV_ACTION_SIZER       = 0xFFFFFFFF   // force the datatype to 4 bytes
	};
enum GSM_RFC_QUERY_TYPE
{
   GSM_TX_BASED_QUERY = 0,
   GSM_RX_BASED_QUERY = 1
};

enum QSC_CAL_TYPE
{
   CONNECTED_CAL_FBRX_RX   = (1 << 0),
   CONNECTED_CAL_RxRSB     = (1 << 1),
   UNCONNECTED_CAL_TX      = (1 << 2),
};

enum PATH_TYPE :
unsigned char
{
   PATH_TYPE_PRX = 0,
   PATH_TYPE_DRX = 1,
   PATH_TYPE_TX = 2,
   PATH_TYPE_MAX = PATH_TYPE_TX
};
enum RGI_CONST
{
   RGI_CDMA = 417,
   RGI_LTE = 500,
   RGI_TDS = 675,
   RGI_WCDMA = 464
};
enum ACTION_TYPE :
unsigned char
{
   ACTION_TYPE_CHAR = 0, // No Cal
   ACTION_TYPE_CAL = 1,
   ACTION_TYPE_OFFSET_CAL = 2,
   ACTION_TYPE_RSB_CAL = 3,
   ACTION_TYPE_RSB_OFFSET_CAL = 4,
   ACTION_TYPE_IP2_CAL = 5,
   ACTION_TYPE_MAX ,
};
#ifndef __MODULATION_TYPE_ENUM_H
#define __MODULATION_TYPE_ENUM_H
typedef enum
{
   RX_FUNCTION_GET_RSSI,
   RX_FUNCTION_GET_DC_OFFSETS,
   RX_FUNCTION_STOP_CONTINOUS_RX
} enum_gsm_rx_function_type;

typedef enum  MOD_TYPE {
   MOD_TYPE_RFCAL_CW =0,
   MOD_TYPE_RFCAL_DIG_MOD_IS95_FORWARD_LINK = 1,
   //MOD_TYPE_DIG_MOD_IS95_REVERSE_LINK,
   MOD_TYPE_RFCAL_DIG_MOD_WCDMA = 2,
   MOD_TYPE_RFCAL_DIG_GMSK_CONTINUOUS = 3,
   MOD_TYPE_WCDMA_CPICH_ONLY = 4,
   MOD_TYPE_DIG_MOD_TDSCDMA_1P28_QPSK_CONTINUOUS = 5,
   MOD_TYPE_MAX = 6
} MOU_TYPE;
#endif

enum TECHNOLOGY_FAMILY_TYPE
{
   TECHNOLOGY_CDMA = 0,
   TECHNOLOGY_WCDMA = 1,
   TECHNOLOGY_GSM = 2,
   TECHNOLOGY_LTE = 10,
   TECHNOLOGY_TDSCDMA = 11
};
enum 
{
   RF_PORT_MAIN = 1,
   RF_PORT_AUX = 2
};


typedef struct TE_Swp_Params
{
   int tech;
   int band;
   int sub_band;
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
   //! offset to tune (for DL)
   int m_offset_in_kHz;

   int m_dl_modulation_type;// 0 - CW

   TE_Swp_Params() :
      tech(0),
      band(0),
      sub_band(0),
      m_ul_chan(0),
      m_ul_lvl(0.0),
      m_dl_lvl(0.0),
      m_tx_waveform(0),
      m_step_width(0.0),
      m_lte_tx_chan_bw(0),
      m_min_ul_lvl(0.0),
      m_rf_mode(PHONE_MODE_MAX),
      m_lte_rx_chan_bw(0),
      m_teAction(true),
      m_numChans(1),
      m_offset_in_kHz(0)
   {}
} TE_Swp_Params;

typedef struct CRFPathInformationPacket : RFPathInformationPacketVersion1
{
   CRFPathInformationPacket(){};
} CRFPathInformationPacket;

typedef struct CRFM_Path
{
   int rfm_dev;
   int signal_path;
   int antenna_port;
   int reserved;
   CRFM_Path (): reserved(0){};
   CRFM_Path (const CRFPathInformationPacket & rfPathInfoPacket) 
      : reserved (0)
   {
      this->signal_path = static_cast <int> ( rfPathInfoPacket.signalPath );
      this->rfm_dev = static_cast <int> ( rfPathInfoPacket.rfmDevice );
      this->antenna_port = static_cast <int> ( rfPathInfoPacket.antennaPath );
   };
   CRFM_Path (const  rfm_path_type & rfm_path)
   {
      this->signal_path = static_cast <int> ( rfm_path.sigPath );
      this->rfm_dev = static_cast <int> ( rfm_path.rfmDevice );
      this->antenna_port = static_cast <int> ( rfm_path.antennaSwitchPath );
      this->reserved = static_cast <int> ( rfm_path.reserved );
   };

   bool operator < (const CRFM_Path & path) const 
	{
      if (this->signal_path == path.signal_path)
      {
         if (this->rfm_dev == path.rfm_dev)
         {
            return this->antenna_port < path.antenna_port;
         }
         return this->rfm_dev < path.rfm_dev;
      }
      return this->signal_path < path.signal_path; 
   }
   bool operator == (const CRFM_Path & path) const 
	{
      if (  ( this->rfm_dev == path.rfm_dev)
         && ( this->signal_path == path.signal_path)
         && ( this->antenna_port == path.antenna_port)
         )
      {
         return true;
      }
      return false;
   }
} CRFM_Path;


struct CGSM_Cal_Device_SigPath
{
   CRFM_Path tx;
   CRFM_Path prx;
   CRFM_Path drx;
};
typedef struct CGSM_Tx_Sweep_Cfg_Params_TE
{
   double m_ul_max_lvl;
   double m_meas_length_us;//micro
   double m_step_length_us;//micro
   double m_trigger_offset_us;//micro
   int m_num_steps;

   //Override Threshold Setting
   bool m_overRideThreshold;
   double m_threshOverrideDB;

   //Use GPRF List Mode
   bool m_usePowerListMeas;

   //Setup Defaults
   CGSM_Tx_Sweep_Cfg_Params_TE()
   {
      m_ul_max_lvl = -999.99;
      m_meas_length_us = -999.99;//micro
      m_step_length_us = -999.99;//micro
      m_trigger_offset_us = -999.99;//micro
      m_num_steps = 0;

      //Override Threshold Setting
      m_overRideThreshold = false;
      m_threshOverrideDB = -999.99;

      m_usePowerListMeas = false;
   }
} CGSM_Tx_Sweep_Cfg_Params_TE;

typedef struct GSM_DA_CAL_Params
{
   std::vector <unsigned long> channel_list;
   int tcxoPdm;
   double inputLevel;
   int measStartDelay;
   std::vector<unsigned short> paRanges;
   unsigned short trigPA;
   unsigned short segLength;
   unsigned short triggerLevRGI;
   unsigned short stopRGI;
   unsigned short startRGI; 
   unsigned short  stepRGI; 
   unsigned long measLength;
   bool extraRGI0; 
   bool extraRGI0_afterPA;
   double triggerMinLevel;
   double triggerMaxLevel;
   unsigned short numStepsPerPARange;
    double saturationDetectionWindow;
   vector <int> nvMappings;
   unsigned char nv_action;
   CGSM_Tx_Sweep_Cfg_Params_TE tx_swp_cfg;
   GSM_DA_CAL_Params():
   saturationDetectionWindow(0.0)
   {};
} GSM_DA_CAL_Params;

#define MAX_PREDIST_CAPTURE_SIZE 64000
#define MAX_PREDIST_CHANNELS 3
//Huge Array of Phase/Amplitude
typedef struct
{
   //Captured Phase and Amplitude Arrays
   float phaseArr[MAX_PREDIST_CAPTURE_SIZE];
   float ampldArr[MAX_PREDIST_CAPTURE_SIZE];
} PreDist_Capture_Record;
typedef struct 
{
   //Captured data per channel
   PreDist_Capture_Record captureRecords[MAX_PREDIST_CHANNELS];
   //Corresponding Channels
   unsigned long channelNumbers[MAX_PREDIST_CHANNELS];
   //Cal RGI
   unsigned long calRgi;
   //Cal Scale Setting
   unsigned long calScaleSetting;

   //The sampling rate and numbers of samples of the waveform actually capture could vary by box.
   unsigned long samplingRateHz;
   unsigned long numSamples;
} PreDist_Capture_Record_Channel;
typedef struct  
{
   //! Swapped Order
   bool swappedOrderEdgeFirst;

   //! controls NV logging and writing to UUT, bitmask of RFCAL_NV_ACTION_TYPE bits
   unsigned int m_nv_action;
   vector <int> nvMappings;
   std::vector<unsigned long> channel_list;

   //! PA Ranges
   unsigned short paRangeCalibrated;

   //! Phone setup
   long tcxoPdm;
   long preDcDuration;
   double calRGIPower;
   long edgeDuration;
 
   //! Processing Information
   long edgeSkipSymbols;
   double dcSkipPercentage;

   //! TE setup
   double inputLevel; // Expected power
   double measStartDelay;

   //! Override Modem Constants
   bool overRideModemConsts;
   double digGainUnity;
   double rampUnity;
   double extensionFloor;
   double dacScale;

   // overRideSmoothingDefaults
   bool overRideSmoothingDefaults;
   bool enableAmAmSmoothing;
   double amamKnotSize;
   double ampmKnotSize;
   double m_minLimitOnMaxPower;
} EDGE_PreDist_Cal_Data;

typedef struct CEDGE_PreDist_Cal_Data
{
   //! Master Switch
   bool m_performPreDistCal;
   long m_perfPreDistCalLong;

   //! Swapped Order
   bool m_swappedOrderEdgeFirst;
   long m_swappedOrderEdgeFirstLong;

   nv_mode_id_type m_nv_mode;

   //! controls NV logging and writing to UUT, bitmask of RFCAL_NV_ACTION_TYPE bits
   unsigned int m_nv_action;

   vector<int> m_channels;

   //! PA Ranges
   unsigned short m_paRangeCalibrated;

   //! Phone setup
   long m_tcxoPdm;
   long m_preDcDuration;
   double m_calRGIPower;
   long m_edgeDuration;
   vector<long> m_calRgiWithNoDaCal;
   char * m_calRgiWithNoDaCal_str;

   //! Processing Information
   long m_edgeSkipSymbols;
   double m_dcSkipPercentage;

   //! TE setup
   double m_inputLevel;
   double m_measStartDelay;

   //!Sanity Check Limits for trigger
   double m_minLimitOnMaxPower;

   //!Sanity Check Limits for EDGE Waveform
   double m_minLimitOnMinPower;

   //! NV Mapping
   vector<unsigned long> m_nvMappings; //example 1 for F1, 2 for F2 etc.

   //! Logging of IQ Data
   bool m_logCaptureData;
   long m_logCaptureDataLong;

   char * m_nvMappingStr;

   //! Delay Limits
   int m_delayLimit;
   int m_delayLimitMin;
   int m_delayLimitMax;

   //! Override Modem Constants
   bool m_overRideModemConsts;
   long m_overRideModemConstsLong;
   double m_digGainUnity;
   double m_rampUnity;
   double m_extensionFloor;
   double m_dacScale;

   long m_overRideSmoothingDefaultsLong;
   bool m_overRideSmoothingDefaults;
   long m_enableAmAmSmoothingLong;
   bool m_enableAmAmSmoothing;
   double m_amamKnotSize;
   double m_ampmKnotSize;


   CEDGE_PreDist_Cal_Data():
      m_performPreDistCal(false),
      m_paRangeCalibrated(65535),
      m_calRGIPower(-999.9),
      m_logCaptureData(false),
      m_minLimitOnMaxPower(0),
      m_minLimitOnMinPower(0),
      m_delayLimitMin(0),
      m_delayLimitMax(0),
	  m_dacScale(1){};
   
}CEDGE_PreDist_Cal_Data;

typedef struct GSM_Rx_CAL_Params
{
   long rx_chainID_selection;

   long m_rx_antenna_path_selection;

   unsigned short rx_alt_path;

   int            rx_path;
   std::vector<int>    channel_list;
   std::vector<int>    rx_gain_ranges;
   std::vector<double> rx_levels;
   int            burst_avgs;
      
   bool           rx_only_calibrate_drx;
   bool           rx_get_nv_container_from_uut;
   bool           rx_read_nv_data_from_uut;
   bool           rx_use_both_main_and_aux;
   bool           rx_do_hi_lin;
   unsigned int   channel_hi_lin;
   //Extrap Cal
   long        rx_extrapolation_channel;
   std::vector<int> measured_gain_ranges;
   std::vector <int> rx_gain_ranges_hi_lin;

   //Characterized Calibration
   long rx_gain_measurement_channel;

   //Generation of Characterized Calibration
   std::string m_char_data_file_name;

   // PRx path limits
   std::vector<int> rx_gain_offsets_min_lims;
   std::vector<int> rx_gain_offsets_max_lims;
   // DRx path limits
   std::vector<int> rx_gain_offsets_min_lims_drx;
   std::vector<int> rx_gain_offsets_max_lims_drx;

   GSM_Rx_CAL_Params():
      rx_get_nv_container_from_uut(true),
      rx_alt_path(0),
      rx_only_calibrate_drx(false),
      rx_use_both_main_and_aux(false),
      rx_do_hi_lin(false),
      rx_read_nv_data_from_uut(false),
      channel_hi_lin(0)
      {};

} GSM_Rx_CAL_Params;
typedef struct GSM_V4_FBRx_Params
{
   bool                 enableFbrxGainCal;
   bool                 performRxCalwithChar;
   vector < int >       fbrx_gain_state_list;
   vector < int >       pa_state_list;
   vector < int >       rgi_list;
   vector < double >    exp_pwr_list;
   vector<vector<unsigned int>>  m_chanList_rxcal;
   int LSE_MAX;
   int LSE_MIN;
   double fbrxPathGainNominaldB;
   double FBRX_PATH_GAIN_MARGIN;
   GSM_V4_FBRx_Params():
    enableFbrxGainCal(false),
    LSE_MAX(500),
    LSE_MIN(0),
    fbrxPathGainNominaldB(-46.0),
    FBRX_PATH_GAIN_MARGIN(16.0),
    performRxCalwithChar(false)
    {};
} GSM_V4_FBRx_Params;

struct digRecord
{
public:
   digRecord(float d, float p)
   {
      digGain = d;
      pwr = p;
   }
   float digGain;
   float pwr;
};
/*------------------------------- 2D Matrix Operations ---------------------------------*/
// a 2D MATRIX Structure
typedef struct R2MATRIX
{
   int         elemSize;      // data type size: sizeof(float), sizeof(int), etc
   unsigned int numRows;      // number of rows in matrix
   unsigned int numCols;      // number of columns in matrix
   char**       dataPtr;      // 2D pointer for accessing matrix data - can use dataPtr[row][col]
}  R2MATRIX;

// Macro for coying and scaling a matirx
// The backslashes are required.
// Inputs:  (1) a      = pointer to input MATRIX structure.
//          (2) b      = pointer to resultant MATRIX structure.
//          (3) s      = scale factor variable to be multipli ed.
//          (4) rows   = number of rows in matrix b
//          (5) cols   = number of columns in matrix b
//          (6) rowoff = number of rows to offset matrix b
//          (7) coloff = number of columns to offset matrix b
//          (8) typea  = legal C type describing the type of a
//          (9) typeb  = legal C type describing the type of b
#define SCALE_R2MATRIX(a, b, s, rows, cols, rowoff, coloff, typea, typeb)\
{ \
   typea** _AMX = (typea **)a->dataPtr;  \
   typeb** _BMX = (typeb **)b->dataPtr;  \
   typea* _PTA;  \
   typeb* _PTB;  \
   int _IX,_JX;  \
   for(_IX = 0 ; _IX < rows ; _IX++) \
   {  \
      _PTB = _BMX[_IX];  \
      _PTA = _AMX[_IX + rowoff] + coloff;  \
      for(_JX = 0 ; _JX < cols ; _JX++)  \
         *_PTB++ = (typeb)(s * (*_PTA++));  \
   }  \
}
// Macro for multiplying matrixes
// The backslashes are required.
// Inputs:  (1) a     = pointer to first MATRIX structure.
//          (2) b     = pointer to second MATRIX structure.
//          (3) c     = pointer to result MATRIX structure.
//          (4) rowsa = number of rows in matrix a
//          (5) colsa = number of columns in matrix a
//          (6) colsb = number of columns in matrix b
//          (7) typea = legal C type describing the type of a
//          (8) typeb = legal C type describing the type of b
//          (9) typec = legal C type describing the type of c
#define MULT_R2MATRIX(a, b, c, rowsa, colsa, colsb, typea, typeb, typec) \
{  \
   typea** _AMX = (typea **)a->dataPtr;  \
   typeb** _BMX = (typeb **)b->dataPtr;  \
   typec** _CMX = (typec **)c->dataPtr;  \
   typea* _PTA;  \
   typeb* _PTB;  \
   typec* _PTC;  \
   int _IX,_JX,_KX;  \
   for(_IX = 0 ; _IX < rowsa ; _IX++) \
   {  \
      _PTC = _CMX[_IX];  \
      _PTB = _BMX[0];  \
      for(_JX = 0 ; _JX < colsb ; _JX++) \
      {  \
         _PTA = _AMX[_IX];  \
         *_PTC = (*_PTA++) * (*_PTB++);  \
         for(_KX = 1 ; _KX < colsa ; _KX++)  \
            *_PTC += (*_PTA++)* _BMX[_KX][_JX];  \
         _PTC++;  \
      }  \
    }  \
}
struct opPwrRecord
{
public:
   opPwrRecord(float o, float a, float p)
   {
      oppwr = o;
      amam = a;
      ampm = p;
   }
   float oppwr;
   float amam;
   float ampm;
};

typedef struct qscRecords
{
	// Sweep Record
	std::vector < cal_sweep_record > swpRecords;
	// TE Record
   std::vector <TE_Swp_Params> teRecords;
   // TE Record Matrix Mode  (frameIndex to Vector)
   std::map < int, std::vector <TE_Swp_Params> > frame_teRecords;
   
   qscRecords()
   {
      swpRecords.resize(0);
      teRecords.resize(0);
      frame_teRecords.clear();
   };
} qscRecords;

typedef enum
{
	PRX = 0,
	DRX = 1,
	SCELL_PRX = 2,
	SCELL_DRX = 3,
	TX = 4
}rf_path_index;

typedef struct CCal_Status
{
   int nvid;
   int status;
   double cal_time;
   CCal_Status ():
      nvid (0),
      status (0),
      cal_time (0.0) {};
} CCal_Status;

typedef struct PA_PARAM_KEY
{
   int product_rev;
   int product_id;
   int manuf_id;
   PA_PARAM_KEY(){};

   bool operator < (const PA_PARAM_KEY & pa_key) const 
	{
      if (this->product_rev == pa_key.product_rev)
      {
         if (this->product_id == pa_key.product_id)
         {
            return this->manuf_id > pa_key.manuf_id;
         }
         return this->product_id > pa_key.product_id;
      }
      return this->product_rev > pa_key.product_rev;
   }
} PA_PARAM_KEY;
typedef struct CXptInput_V1_Key
{
   int            rf_mode;
   int            instance;
   int            device;
   CXptInput_V1_Key ():
      rf_mode (0),
      instance (0),
      device (0) {};
   bool operator < (const CXptInput_V1_Key & key) const 
	{
      if (this->rf_mode == key.rf_mode)
      {
         if (this->instance == key.instance)
         {
            return this->device > key.device;
         }
         return this->instance > key.instance;
      }
      return this->rf_mode > key.rf_mode;
   }
} CXptInput_V1_Key;
typedef struct CXptInput_V1
{
   int            PAstate;
   int            BW;
	int				IcqEt;
	int				IcqEpt;
	int				IcqRgiDeltaSwp;
	int				IqGainRgiDeltaSwp;
	int				FirmwareDelayIqGain;
	int				AclrDelayCharIqGain;
	int				MLineFitType;
	double			EtVmin;
	double			MLineRegCoef;
   double			CalCompPtEpt;
	double			CalCompPtEt;
	double			MLineMaxPaSupplyEpt;
	double			MLineMaxPaSupplyEt;
   double         qpoetVdMin;
   double         qpoetVdMax;
	vector <int>	Detrough_table;
   CXptInput_V1 ():
      IcqEt (0),
      IcqEpt (0),
      IcqRgiDeltaSwp (0),
      IqGainRgiDeltaSwp (0),
      FirmwareDelayIqGain (0),
      AclrDelayCharIqGain (0),
      MLineFitType (0),
      EtVmin (0.0),
      MLineRegCoef (0.0),
      CalCompPtEpt (0.0),
      CalCompPtEt (0.0),
      MLineMaxPaSupplyEpt (0.0),
      MLineMaxPaSupplyEt (0.0),
      qpoetVdMin (0.0),
	  PAstate(0),
	  BW(10),
      qpoetVdMax (0.0) {};
} CXptInput_V1;

typedef std::vector<CXptInput_V1> CXptInput_V1_BW_V;
typedef struct AptRecord
{
	double	pwr;
	int		vcc;
	int		icq;
   AptRecord (): pwr(0.0), vcc (0), icq (0) {};
   bool operator < (const AptRecord & Apt) const 
   {  
      return this->pwr < Apt.pwr;
   }
} AptRecord;

typedef struct APT_CharV3_Key
{
	int	rf_mode;
	int	channel;
	int	pa_state;
	int	tx_purpose;
	int	device;
   APT_CharV3_Key(int _rf_mode, int _device, int  ichannel, int _pa_state, int _tx_purpose) : rf_mode(_rf_mode), device(_device), channel( ichannel), pa_state(_pa_state), tx_purpose(_tx_purpose) {};
   APT_CharV3_Key():tx_purpose(0),device(0),pa_state(0),channel(0), rf_mode(0){};
	bool operator < (const APT_CharV3_Key & Apt) const 
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
} APT_CharV3_Key;

	//Params needs from QSC Param xml
typedef struct CCalType
{
	unsigned char Enable_FBRx_Gain;
	unsigned char Enable_PRx_Gain;
	unsigned char Enable_DRx_Gain;
	unsigned char Enable_NV_Write;
	unsigned char Enable_Sweep_Mode;
   unsigned char Enable_PRx_RSB;
   unsigned char Enable_DRx_RSB;
   unsigned char Enable_Full_Bias_Linearizers;
   unsigned char Enable_APT_Linearizers;
   unsigned char Enable_APT_FComp;
   unsigned char Enable_XPT_FComp;
   unsigned char Enable_Tx_XPT;
   unsigned char Enable_PRx_IP2Cal;
   unsigned char Enable_DRx_IP2Cal;
   unsigned char Enable_XO_Cal;
   unsigned char Enable_Push_FBRxGain_Data;
   unsigned char Enable_Multiple_Rx_Path_Cal;
   unsigned char Enable_Multiple_FBRx_Path_Cal;
   unsigned char Enable_Multiple_Tx_Path_Cal;
   unsigned char Enable_Offset_Calibration;
   unsigned char Enable_Downlink_Aux_Port;
   unsigned char Enable_TxCal_RGIMeasLen;
   unsigned char Skip_Radio_Setup;
   unsigned char Skip_Radio_TearDown;
   unsigned char Skip_DeInit;
  

   CCalType():
		Enable_FBRx_Gain(false),
		Enable_PRx_Gain(false),
		Enable_DRx_Gain(false),
		Enable_NV_Write(true),
      Enable_Sweep_Mode(true),
      Enable_PRx_RSB(false),
      Enable_DRx_RSB(false),
      Enable_Multiple_Rx_Path_Cal(false),
      Enable_Multiple_FBRx_Path_Cal(false),
      Enable_Multiple_Tx_Path_Cal(false),
      Enable_Offset_Calibration(false),
      Enable_Full_Bias_Linearizers(false),
      Enable_APT_Linearizers(false),
      Enable_APT_FComp(false),
      Enable_XPT_FComp(false),
	  Enable_PRx_IP2Cal(false),
	  Enable_DRx_IP2Cal(false),
      Enable_Push_FBRxGain_Data(false),
      Enable_Tx_XPT(false),
      Enable_TxCal_RGIMeasLen(false),
	  Enable_XO_Cal(false),
      Skip_Radio_Setup(false),
      Skip_Radio_TearDown(false),
      Skip_DeInit(false),
      Enable_Downlink_Aux_Port(false)
      {};
} CCalType;

typedef struct CCommonParam
{
	int Config_Seg_Duration_us;
	int Tuning_Seg_Duration_us;
	int Meas_Seg_Duration_us;
   int Data_Seg_Duration_us;
   int Tx_Override_Seg_Duration_us;
	int Tx_Waveform_Type;
	vector < vector < int > > Tx_Rx_BW_Type;
  	int LTE_Num_RBs;
	int LTE_Start_RB;
   int DL_CW_Offset_KHz;
   int DL_WaveformType;
   int DL_Power_Offset;
	int Ref_Channel; 
   int En_Compress_Esc_Req;
   int En_Compress_Esc_Res;
   int En_Box_Meas; 
   int Poll_Interval; // Polling for Radio Setup
   int Poll_Timeout; // Polling for Radio Setup
   int Expected_Power_Option;
   vector <int> Channel_List;
   CCommonParam ():
      Config_Seg_Duration_us(60000),
         Meas_Seg_Duration_us(4000),
         Tx_Override_Seg_Duration_us(0xFFFF),
         DL_CW_Offset_KHz(500),
         DL_WaveformType(0),
         Data_Seg_Duration_us(10000),
         Tx_Waveform_Type (0),
         //Tx_Rx_BW_Type (0),
         LTE_Num_RBs (0),
         LTE_Start_RB (0),
         DL_Power_Offset (0),
         Ref_Channel (0),
         En_Compress_Esc_Req (0),
         En_Compress_Esc_Res (0),
         En_Box_Meas(0),
         Poll_Interval(1000),
         Poll_Timeout(40000),
         Expected_Power_Option(1)

   {};
} CCommonParam;
typedef struct CRx_RSB
{
   int   LNA_Gain_State;
   int   Accumulation_Time_us;
   int   AGC_Settling_Time_us;
   int   Action_Mask;
   double Pre_Cal_RSB_Max_Lim_dB;
   double Post_Cal_RSB_Max_Lim_dB;
   double DL_Level_dBm;
   
   CRx_RSB ():
      LNA_Gain_State(0),
      Accumulation_Time_us(0),
      AGC_Settling_Time_us(0),
      Action_Mask(0),
      Pre_Cal_RSB_Max_Lim_dB(0.0),
      Post_Cal_RSB_Max_Lim_dB(0.0),
      DL_Level_dBm(0){};

} CRx_RSB;

typedef struct CRx_Gain
{
   int Num_Gain_States_Per_Segment;	
   int Preferred_Device;
   int AGC_Settling_Time;
   int Accumulation_Time;
   int Measurement_Purpose;
   unsigned char Skip_Path;
	std::vector<double> DL_Powers;
	std::vector<int> Gain_States;
	std::vector<int> Power_Mode;
	std::vector<int> Gain_Limit_Max;
	std::vector<int> Gain_Limit_Min;

   CRx_Gain ():
   Num_Gain_States_Per_Segment(3),
   Preferred_Device(0),
   AGC_Settling_Time(0),
   Accumulation_Time(0),
   Measurement_Purpose(0),
   Skip_Path (0) {};
} CRx_Gain;

typedef struct CFBRx_Gain
{
	std::vector<unsigned int> channelList;
	std::vector<int> Gain_State_List;
	std::vector< double > Tx_Power_dBm_List;
	std::vector<int> PA_State_List;
	std::vector<int> RGI_List;
	std::vector<int> Bias_List;
	std::vector<int> Icq_List;
   int Number_Of_Captures;
   int Max_LSE;
   CFBRx_Gain():
     Number_Of_Captures(0),
     Max_LSE(328) /* 1 % of LSE */
     {};
} CFBRx_Gain;


typedef struct CTx_Linearizer
{
   int Upper_Bound_Channel;
   int Seq_Bld_Seg_Dur;
   int Seq_Exec_Seg_Dur;
   vector <int> PA_State_List;
   vector < vector < int > > RGI_List;
   vector <int> Bias_List;
   vector <int> Icq_List;
   vector <double> Exp_Start_Power_List;
   vector <double> Exp_Delta_Power_List;
   vector <double> Min_Delta_Power_List;
   vector <double> Max_Lin_Power_List;
   vector <double> Min_Lin_power_List;
   vector <int> APT_PA_Gain_Delta_List;
   vector <int> PA_Gain_List;

   CTx_Linearizer ():
      Upper_Bound_Channel(0),
      Seq_Bld_Seg_Dur(0),
      Seq_Exec_Seg_Dur(0){};
} CTx_Linearizer;

typedef struct CXPT_Params
{
 //  int xpt_mode; // this parameter may be removed
   int XPT_Seq_Action;
   int Get_Addn_XPT_Dbg_Info;
   int PA_State;
   int Enable_Memory_Kernel;
   vector < int > RgiList;
   vector < int > BiasList;
   vector < double > Exp_Pwrs;
   vector < int > Rgi_Delta_SSS;
   vector < double > Meas_Pwr_List;
   vector < double > Calc_Pwr_List;
   vector < double > Fcomp_meas_pwr_index_list;
   CXPT_Params (): 
      Enable_Memory_Kernel (0),
      XPT_Seq_Action (0),
      Get_Addn_XPT_Dbg_Info (0),
      PA_State (0) {};
} CXPT_Params;

typedef struct CTx_XPT
{
   int Seq_Bld_Seg_Dur;
   int Seq_Exec_Seg_Dur;
   int Poll_Interval;
   int Poll_Timeout;
   vector < int > XPT_Mode_List;
   CXPT_Params EPT_Params;
   CXPT_Params ET_Params;
   CTx_XPT():
      Seq_Bld_Seg_Dur (0),
      Seq_Exec_Seg_Dur (0),
      Poll_Interval (0),
      Poll_Timeout (0) {};
} CTx_XPT;

typedef struct CRx_IP2
{

   int Poll_Interval;
   int Poll_Timeout;
   int oip2_delta_min_limit_db10;
   CRx_IP2():
      Poll_Interval (0),
      Poll_Timeout (0),
	  oip2_delta_min_limit_db10 (0){};
} CRx_IP2;

typedef struct CXo
{
   int XO_Action;   
   int DL_Level_dBm;
   int Temperature_span;
   int Min_Ft_Samples;
   int Max_Ft_Samples;
   int Substage_Time;
   int Timeout_Time;
   int Update_NV;
   int Xtal;
   int max_start_temperature;
   int max_final_temperature;
   int api_state_expected;
   int LNA_Gain_State;
   int Poll_Interval_us;

   CXo ():
   XO_Action(1),
   Temperature_span(0),
   Min_Ft_Samples(5),
   Max_Ft_Samples(100),
   Substage_Time(10),
   LNA_Gain_State (2),
   Timeout_Time(30000),
   Update_NV(3),
   Xtal(4),
   max_start_temperature(45),
   max_final_temperature(60),
   Poll_Interval_us(1000), 
   api_state_expected(1){};

} CXo;

typedef struct CTX_Lin_TxParam_Pwr
{
   int channel;
 //  int pa_state;
   int rgi;
   int bias;
   int icq;
   int iqOffset;
   int dpd_idx;
   int fbrx_gain_state;
   int fbrx_hw_gain;
   double pwr;
   bool operator < (const CTX_Lin_TxParam_Pwr & node) const 
	{
      /* This determines the sort order of tx results */
      if (this->channel == node.channel)
      {
         if (this->rgi == node.rgi)
         {
            return this->bias > node.bias;
         }
         return this->rgi > node.rgi;
      }
      return this->channel > node.channel;
   }
} CTX_Lin_TxParam_Pwr;

typedef struct CXPT_Results
{
   int xpt_mode;
   int pa_state;
   int num_dpd;
   int BW;
   int status;
   CXPT_Results ():
      xpt_mode (0),
      pa_state (0),
      BW (0) {};
   bool operator < (const CXPT_Results & temp) const 
	{
      /* This determines the sort order of xpt results */
      if (this->xpt_mode == temp.xpt_mode)
      {      
         if (this->pa_state == temp.pa_state)
         {
            return this->BW < temp.BW;
         }
         return this->pa_state > temp.pa_state;
      }
      return this->xpt_mode < temp.xpt_mode;
   }
} CXPT_Results;


typedef struct CVec_TxPwr : public std::vector < CTX_Lin_TxParam_Pwr > {} CVec_TxPwr; // To avoid 4503 warning, wrap vector with class
typedef std::map < int, CVec_TxPwr >  Waveform_TxResult_Map;

typedef std::map < int, Waveform_TxResult_Map >  DpdType_Waveform_TxResult_Map;
typedef std::map < int, DpdType_Waveform_TxResult_Map > PA_DpdType_Waveform_TxResult_Map;

typedef std::map < int, vector < seq_cal_tx_freq_comp_tbl_type > > PA_fcompOffsets_map;
typedef std::map < int, vector < CXPT_Results > > XptMode_results_map;

typedef std::map< CXPT_Results, xpt_dbg_type > xptDebugInfoType;

typedef struct CTX_Results
{
   // tx cal success or not 
   int cal_status;
   
   int ref_channel;
   int upper_bound_channel;
   int therm;

   // tx linearlizer results
   PA_DpdType_Waveform_TxResult_Map tx_lin_results;

   vector < int > freqcomp_channels;

   // tx freqcomp results
   PA_fcompOffsets_map tx_freqcomp_results;

   // XPT results
   XptMode_results_map xpt_resuls;
   
   // XPT debugInfo  [1] -- EPT  [2] --- ET
   xptDebugInfoType xpt_debug_info;


} CTX_Results;
typedef struct CLogParams
{
  std::string txtLog;

  union
  {
     struct xptDebugInfo
     {
        xptDebugInfoType & xpt_debug_info;
     };
  };
  CLogParams()
  {
     txtLog ="";
  }
} CLogParams;


typedef struct CFBRx_MeasParams
{
   int pa_state;
   int rgi;
   int bias;
   int icq;
   int expectedPwr;
   double txPower;
   CFBRx_MeasParams ():
      pa_state (0),
      rgi (0),
      bias (0),
      expectedPwr (0),
      txPower (0.0) {};
} CFBRx_MeasParams;

typedef struct QSCNode
{
   int tech;
   int band;
   int subBand;
   int instance;
   int sweep_type;
   
   QSCNode (int _tech,int _sweep_type, int _instance, int _band = -1, int _subband = -1):
      tech(_tech),
      sweep_type(_sweep_type),
      instance(_instance),
      band(_band),
      subBand(_subband) {};
   QSCNode ():instance(0),sweep_type(0),band(0),subBand(0),tech(0) {};
   bool operator < (const QSCNode & node) const 
	{
      if (this->sweep_type == node.sweep_type)
      {
         if (this->tech == node.tech)
         {
            if (this->band == node.band)
            {
               if (this->subBand == node.subBand)
               {
                  return this->instance < node.instance;
               }
               return this->subBand < node.subBand;
            }
            return this->band < node.band;
         }
         return this->tech < node.tech;
      }
      return this->sweep_type < node.sweep_type;
   }
   bool operator == (const QSCNode & node) const 
   {
      if (( this->sweep_type == node.sweep_type )
         && ( this->tech == node.tech )
         && ( this->band == node.band )
         && ( this->subBand == node.subBand )
         && ( this->instance == node.instance )
         )
      {
         return true;
      }
      return false;
   }
} QSCNode;


#define FTM_RF_PATH_INFORMATION_IP2CAL_BWIDTH_BMASK 0x1FFF
#define FTM_RF_PATH_INFORMATION_IP2CAL_ACTION_BSHFT 13
#define FTM_RF_PATH_INFORMATION_IP2CAL_ACTION_BMASK 0x7

/* CPath_Info - store per path parameter */
typedef struct CPath_Info
{
   std::map < CRFM_Path, CCalType > calType;
   std::map < CRFM_Path, CCommonParam > commonParams;
   
   std::map < CRFM_Path, CFBRx_Gain > fbrxGainParams;
   std::map < CRFM_Path, CRx_Gain > rxGainParams;
   std::map < CRFM_Path, CTx_Linearizer > txLinParams;
   std::map < CRFM_Path, CTx_XPT > xptParams;
   std::map < CRFM_Path, CRx_RSB > rsbParams;
   std::map < CRFM_Path, CRx_IP2 > ip2Params;
   std::map < CRFM_Path, CXo > xoParams;
   void clear()
   {
      calType.clear();
      commonParams.clear();
      fbrxGainParams.clear();
      rxGainParams.clear();
      txLinParams.clear();
      xptParams.clear();
      rsbParams.clear();
	  ip2Params.clear();
   };
} CPath_Info;
typedef struct CBandInfoPtr
{
   int band;
   int subBand;
   CCalType * calType;
	CCommonParam * commonParams;
	CRx_Gain * rxGainParams;
	CFBRx_Gain * fbrxGainParams;
   CTx_Linearizer * txLinParams;
   CTx_XPT * xptParams;
   CRx_RSB * rsbParams;
   CRx_IP2 * ip2Params;
   CXo * xoParams;

   PA_PARAM_KEY * pa_param_key;
   CPath_Info * perPathParams; // To find out other path params.
   CBandInfoPtr():
      band(0),
      subBand(0),
      calType(NULL),
      commonParams(NULL),
      rxGainParams(NULL),
      fbrxGainParams(NULL),
      txLinParams(NULL),
      xptParams(NULL),
      rsbParams(NULL),
	  xoParams (NULL),
      ip2Params(NULL){};
} CBandInfoPtr;

typedef struct CBandInfo
{
   int band;
   int subBand;
   CCalType calType;
	CCommonParam commonParams;
	CRx_Gain rxGainParams;
	CFBRx_Gain fbrxGainParams;
   CTx_Linearizer txLinParams;
   CTx_XPT xptParams;
   CRx_IP2 ip2Params;
   CRx_RSB rsbParams;
   CXo xoParams;
   CPath_Info perPathParams;
   PA_PARAM_KEY pa_param_key;
   CBandInfo ():
      band (0),
      subBand (0) {};
} CBandInfo;

typedef struct QSCDefaultParams // This is for connected cal
{
   CCommonParam commonParams;
   CFBRx_Gain fbrxGainParams;
   CRx_Gain rxGainParams;
   CRx_RSB rsbParams;
   CXo xoParams;
} QSCDefaultParams;



   // **************************************** //
   // Rx Char map 
   // **************************************** //

   typedef struct RxCharResultData
   {
      int gain_state;
      int agc_offset;
      RxCharResultData():gain_state(0),agc_offset(0) {};
      bool operator < (const RxCharResultData & data) const 
	   {
         return this->gain_state < data.gain_state;
      }
   } RxCharResultData;
   typedef std::map < int, int > CGainState_AgcOffset;
   typedef std::map < int, CGainState_AgcOffset > CChan_RxResults;
   class CPath_ChanResults // This is workaround to resolve longer map name warning
   {
   public:
       std::map < CRFM_Path, CChan_RxResults > map;
   };
   typedef std::map < QSCNode, CPath_ChanResults > CTechBand_PathChanResults;

   // **************************************** //
   // FBRx  
   // **************************************** //

   typedef struct CFBRx_Path_Result : public ftm_fbrxgain_path_result
   {
      double txMeasPower; // callbox meas power
   } CFBRx_Path_Result;
   typedef struct fbrx_gain_results_input_identifiers
   {
      QSCNode node;
      CRFM_Path rfm_path;
      uint8 bwType;
      uint32 channel;
      //uint8 nvContainerId;
      uint8 fbrxGainState;
     bool operator < (const fbrx_gain_results_input_identifiers & id) const 
     {
        if ( this->node == id.node)
        {
           if ( this->rfm_path == id.rfm_path )
           {
              if ( this->bwType == id.bwType )
              {
                 if ( this->channel == id.channel )
                 {
                     return this->fbrxGainState < id.fbrxGainState;
                 }
                 return this->channel < id.channel;
              }
              return this->bwType < id.bwType;

           }
           return this->rfm_path < id.rfm_path;
        }
        return this->node < id.node;
     }
   } fbrx_gain_results_input_identifiers;
   // ********************************************************************* //
   // Repository
   // ********************************************************************* //
   
   
   
   class RFMPathDB
   {
   protected:
      typedef struct RFPathInformationHead : RFPathInformationRequest{
      bool operator < (const RFPathInformationHead & req) const 
	   {
         if (req.technology == this->technology)
         {
            if (req.band == this->band)
            {
               return req.subBand < this->subBand;
            }
            return req.band < this->band;
         }
         return req.technology < this->technology;
      }
   } RFPathInformationHead;

   public:
      //static RFMPathDB& getInstance()
      //{
      //   static RFMPathDB instance;
      //   return instance;
      //}
      /* Helper function */
      static unsigned char IsRFMPathIncluded(HANDLE hPhone, const CRFPathInformationPacket & keyRFMPath, const vector <CRFPathInformationPacket> & vecRFMPath);
      static unsigned char IsSameRFMPath(HANDLE hPhone, const CRFPathInformationPacket & keyRFMPath, const CRFPathInformationPacket & RFMPathToTest);
      static unsigned char IsAssociatedRFMPath(HANDLE hPhone, const CRFPathInformationPacket & keyRFMPath, const CRFPathInformationPacket & RFMPathToTest);
      static unsigned char ConvertRFPathInfoData(CRFPathInformationPacket & rfmInfopacket, const RFPathInformationPacketVersion0 & oldVersion);

      unsigned char CreateRFMPathDB(HANDLE hPhone, int tech, int band, int subband, unsigned char version = 1); // for older build, use 0
     
      /* Query functions */
      unsigned char QueryRFMPathInfoByPathType(HANDLE hPhone, int tech, int band, int subband, unsigned char pathType, 
         std::vector < CRFPathInformationPacket > & rfmPaths, unsigned char ActionType = 1);
     
      unsigned char QueryRFMPathInfoByPathType(HANDLE hPhone, int tech, int band, int subband, 
         vector<PATH_TYPE>& pathCriteria, vector<ACTION_TYPE>& actionCriteria, std::vector < CRFPathInformationPacket > & results);
      
      unsigned char QueryAssociatedRFMPathInfo(HANDLE hPhone, int tech, int band, int subband, 
         const CRFPathInformationPacket & keyRFMPath, CRFPathInformationPacket & associatedRFMPath);

      int GetPathType( HANDLE hPhone, int tech, int band, int subband, const CRFM_Path & rfm_path );
      int GetPathType( HANDLE hPhone, int tech, int band, int subband, const CRFPathInformationPacket & rfmpkt );


      // Internal data
private:

      std::map < RFPathInformationHead, std::vector < CRFPathInformationPacket > > rfmPathDB;
      unsigned char version;
   };


   namespace QSC 
   {
      class FBRxResultDB
      {
         public:
           unsigned char Init();
           unsigned char SetFBRxResult (HANDLE hPhone, const QSCNode & qscNode, 
              const CRFM_Path & rfmPath, const ftm_fbrxgain_path_result & result);

           unsigned char GetFBRxResult ( HANDLE hPhone, const QSCNode & qscNode, std::map < CRFM_Path, ftm_fbrxgain_path_result > & fbrx_rfmPath_ResultMap );

         private:           
            std::map < QSCNode, std::map < CRFM_Path, ftm_fbrxgain_path_result > > rfmPathDB;
      };
      class Cal_Status_DB
      {
         public:
            enum 
            {
               ALL = 0,
               STATUS = 1,
               NVID = 2,
               TIME = 3,
            } UPDATE;
            enum CAL_STATUS
            {
               FAILURE = 0,
               SUCCESS = 1,
            } ;
            Cal_Status_DB() {};
            unsigned char Init ();
            unsigned char SetData ( HANDLE hPhone, const QSCNode & qsc_node, seq_cal_res_type cal_type,  const CCal_Status & cal_status, int type /* 0: add all, 1: add nv id only, 2: time only */);
            unsigned char GetData ( HANDLE hPhone, const QSCNode & qsc_node, seq_cal_res_type cal_type,  CCal_Status & cal_status );
            unsigned char GetTechAndBand ( HANDLE hPhone, vector < QSCNode > & qsc_nodes );
         private:
            std::map < QSCNode, std::map < seq_cal_res_type , CCal_Status > > cal_status_records;    
      };
      class TestTime
      {
      public:
         unsigned char Start( HANDLE hPhone);
         unsigned char End ( HANDLE hPhone, const QSCNode & qsc_node);
         unsigned char Init();
         unsigned char GetData ( HANDLE hPhone, const QSCNode & qsc_node, unsigned long & ElapsedTime);
         TestTime ():elapsedTime(static_cast<DWORD>(0)){};
      private:
         DWORD elapsedTime;
         std::map < QSCNode, unsigned long > qscnode_elappasedTimeMap;
      };

      class RxCharDB
      {
         public:
            RxCharDB() {};
            ~RxCharDB() {};
            virtual unsigned char RegisterRxCharData ( HANDLE hPhone, CTechBand_PathChanResults & result );
            virtual unsigned char GetRxCharData ( HANDLE hPhone, const QSCNode & techBand, const CRFM_Path & rfmPath, int channel, int gain_state, int & agc_offset ); // return agc_offset
            virtual unsigned char GetRxCharBandData ( HANDLE hPhone, const QSCNode & techBand, CPath_ChanResults & result );
            virtual unsigned char Init();
         private:
            CTechBand_PathChanResults rxCharData;
      };
   }

   class AptCharV3
   {
      public:
         AptCharV3() { this->apt_key_recordset.clear(); };
         unsigned char Init();
         unsigned char SetData ( const APT_CharV3_Key & key, const std::set < AptRecord > & aptRecords ) ;
         unsigned char GetData ( int tech, int band, int subband, int channel, int tx_purpose, std::map < int, std::set < AptRecord > > & pa_aptRecords );
      private:
            std::map < APT_CharV3_Key, std::set < AptRecord > > apt_key_recordset;
   };

   class PA_PARAM_DB
   {
      public:
         PA_PARAM_DB() { this->pa_param_records.clear(); };
         unsigned char Init();
         unsigned char SetData ( const PA_PARAM_KEY & key, const std::map < CXptInput_V1_Key, CXptInput_V1_BW_V > & xptInput_Record_Map) ;
         unsigned char GetData ( const PA_PARAM_KEY & key, const int tech, const int band, const int subband, int bw, CXptInput_V1 & xptInput );
      private:
         unsigned char GetData ( const PA_PARAM_KEY & key, const int tech, const int band, const int subband, CXptInput_V1_BW_V & xptInput );
         std::map < PA_PARAM_KEY, std::map < CXptInput_V1_Key, CXptInput_V1_BW_V > > pa_param_records;
   };
   
   // ***************************************************************//
   //      GSM DA Char V2 Table data structure
   // ***************************************************************//
   struct GSMDACharV2_RGI
   {
	   // map< RGI, dBm*100>
	   map< int, double > rgiVsPwr;
   };
   struct GSMDACharV2_PA
   {
	   // map< PA Range, GSMDAChar_RGI>
	   map< int, GSMDACharV2_RGI > data;
   };
   struct GSMDACharV2_Mod
   {
	   // map< modulation, GSMDAChar_PA >
	   map< int, GSMDACharV2_PA > data;
   };
   struct GSMDACharV2_Chan
   {
	   // map< channel, GSMDAChar_Mod >
	   map< int, GSMDACharV2_Mod > data;
   };
   struct GSMDACharV2_Temp
   {
	   // map< temperature, GSMDAChar_Chan>
	   map< double, GSMDACharV2_Chan > data;
   };
   struct GSMDACharV2_RFMDev
   {
	   // map< chain, GSMDAChar_Chan>
	   map< int, GSMDACharV2_Temp > data;
   };
   struct GSMDACharV2_SigPath
   {
	   // map< rfMode, GSMDAChar_Device>
	   map< int, GSMDACharV2_RFMDev > data;
   };
   struct GSMDACharV2_AntPath
   {
	   // map< unsigned short, GSMDAChar_SigPath>
	   map< unsigned short, GSMDACharV2_SigPath > data;
   };
   struct GSMDACharV2_RfMode
   {
	   map< int, GSMDACharV2_AntPath > data;
   };


   // ***************************************************************//
   //      GSM Rx Char Table data structure
   // ***************************************************************//
   typedef map<int, map<int, double> > GSMRxCharTable; // map< channel, map<LNA state, Gain Offset> >

   // GSM Rx Char Data - A collection of all GSM Rx Char Tables keyed by RF Mode Enum
   typedef map< int, GSMRxCharTable > GSMRxCharData;                 // map< RF_MODE, map< channel, map<LNA state, Gain Offset> > >

   // GSM Rx Char Data Mapped to ChainID - A collection of all GSM Rx Char Tables keyed by RF Mode Enum and ChainID
   typedef map< int, GSMRxCharData >  GSMRxCharData_ChainBand;      // map< chainID, map< RF_MODE, map< channel, map<LNA state, Gain Offset> > > >

   class DACharV2{
   public:
	   DACharV2() {};
	   unsigned char Init();
	   unsigned char SetData ( int rfMode, unsigned short antPath, int sigPath, int rfmDev, int inst, int channel, int mod, int pa, map <int, double> rgiVsPwr);
	   unsigned char GetData ( GSMDACharV2_RfMode & gData );
	   void EnsureSortedEGSMChannels ( QMSL_GSM_DA_Cal_Result& daCalIn );
	   //			   unsigned char GetData ( int tech, int band, int subband, int channel, int tx_purpose, std::map < int, std::set < AptRecord > > & pa_aptRecords );
   private:
	   GSMDACharV2_RfMode daCharV2data;
   };

   class GSMRxChar{
	   public:
		   GSMRxChar(){};
		   unsigned char Init();
		   unsigned char SetData( int rfMode, int chain, int channel, map<int, double> lnaVsGain );
		   unsigned char GetData (GSMRxCharData_ChainBand & g_data);

   private:
	   GSMRxCharData_ChainBand gsmRxChardata;

   };

   class Repository
   {
      private:            
         AptCharV3 aptChar;
         PA_PARAM_DB paParamDb;
		   DACharV2 daCharV2;
		   GSMRxChar gsmRxChar;
         QSC::FBRxResultDB fbrxRes;
         QSC::Cal_Status_DB cal_status;
         QSC::TestTime testTime;
         QSC::RxCharDB rxChar;
      public:
         RFMPathDB rfmPathDB;
         Repository() {};
         
         unsigned char Init ()
         {
            cal_status.Init();
			daCharV2.Init();
			gsmRxChar.Init();
            fbrxRes.Init();
            testTime.Init();
            rxChar.Init();
            return true;
         }

         /* APT char V3 */
         unsigned char SetAptCharData ( const APT_CharV3_Key & key, const std::set < AptRecord > & aptRecords ) 
         {  
            return aptChar.SetData ( key, aptRecords ); 
         };
         unsigned char GetAptCharData ( int tech, int band, int subband, int channel, int tx_purpose, std::map < int, std::set < AptRecord > > & pa_aptRecords )
         {
            return aptChar.GetData ( tech, band, subband, channel, tx_purpose, pa_aptRecords ); 
         };

		 /* GSM DA char V2*/
         unsigned char SetDACharData ( int rfMode, unsigned short antPath, int sigPath, int rfmDev, int temp, int channel, int mod, int pa, map <int, double> rgiVsPwr ) 
         {  
            return daCharV2.SetData ( rfMode, antPath, sigPath, rfmDev, temp, channel, mod, pa, rgiVsPwr ); 
         };
		          
		 unsigned char GetDACharData ( GSMDACharV2_RfMode & g_data ) 
         {  
            return daCharV2.GetData ( g_data ); 
         };

		 unsigned char SetGSMRxCharData( int rfMode, int chain, int channel, map <int, double> lnaVsGain )
		 {
			 return gsmRxChar.SetData ( rfMode, chain, channel, lnaVsGain );
		 }

		 unsigned char GetGSMRxCharData (GSMRxCharData_ChainBand & g_data)
		 {
			 return gsmRxChar.GetData (g_data);
		 }

         /* PA PARAM */
         unsigned char SetPaParamData ( const PA_PARAM_KEY & key, const std::map < CXptInput_V1_Key, CXptInput_V1_BW_V > & xptInput_Record_Map) 
         {
            return paParamDb.SetData ( key, xptInput_Record_Map );
         };
         unsigned char GetPaParamData ( const PA_PARAM_KEY & key, const int tech, const int band, const int subband, int bw, CXptInput_V1 & xptInput )
         {
            return paParamDb.GetData ( key, tech, band, subband, bw, xptInput );
         };


         /* FBRx Result */
         unsigned char SetFBRxResult (HANDLE hPhone, const QSCNode & qscNode, 
              const CRFM_Path & rfmPath, const ftm_fbrxgain_path_result & result)
         { 
            return fbrxRes.SetFBRxResult ( hPhone, qscNode, rfmPath, result ); 
         }

         unsigned char GetFBRxResult ( HANDLE hPhone, const QSCNode & qscNode, std::map < CRFM_Path, ftm_fbrxgain_path_result > & fbrx_rfmPath_ResultMap )
         {
            return fbrxRes.GetFBRxResult ( hPhone, qscNode, fbrx_rfmPath_ResultMap ); 
         }

         /* Cal Status */
         unsigned char SetCalStatus ( HANDLE hPhone, const QSCNode qsc_node, seq_cal_res_type cal_type,  const CCal_Status & cal_status, int type = 0 /* 0: add all, 1: update status only, 2: add nv id only, 3: time only */ )
         {
            return this->cal_status.SetData ( hPhone, qsc_node, cal_type, cal_status, type );
         }
         unsigned char GetCalStatus ( HANDLE hPhone, const QSCNode qsc_node, seq_cal_res_type cal_type,  CCal_Status & cal_status )
         {
            return this->cal_status.GetData ( hPhone, qsc_node, cal_type, cal_status );
         }
         unsigned char GetTechAndBandForCalStatus (HANDLE hPhone, vector < QSCNode > & qsc_nodes )
         {
            return this->cal_status.GetTechAndBand ( hPhone, qsc_nodes );
         }

         /* TestTime */
         unsigned char TestTimeStart( HANDLE hPhone)
         {
            return testTime.Start(hPhone);
         }
         unsigned char TestTimeEnd(HANDLE hPhone, const QSCNode & qsc_node )
         {
            return testTime.End(hPhone, qsc_node);
         }
         unsigned char TestTimeGetData ( HANDLE hPhone, const QSCNode & qsc_node, unsigned long & ElapsedTime)
         {
            return testTime.GetData ( hPhone, qsc_node, ElapsedTime );
         }
 
         ///* EquipmentMgr */
         //unsigned char RegisterDevice ( HANDLE hPhone,  int type, int gpib_board, int prim_addr, int sec_addr)
         //{
         //   return equipMgr.RegisterDevice ( hPhone, static_cast<EQUIPMENT_TYPE> (type), gpib_board,  prim_addr,  sec_addr);
         //};
         //unsigned char UnregisterDevice()
         //{
         //   return equipMgr.UnregisterDevice();
         //};
         //CMeasurementManager * GetMeasurementMgr() { return equipMgr.GetMeasurementMgr() ; };

         /* RxChar DB */
         virtual unsigned char RegisterRxCharData ( HANDLE hPhone, CTechBand_PathChanResults & result )
         {
            return rxChar.RegisterRxCharData ( hPhone, result );
         };
         virtual unsigned char GetRxCharData ( HANDLE hPhone, const QSCNode & techBand, const CRFM_Path & rfmPath, int channel, int gain_state, int & agc_offset )
         {
            return rxChar.GetRxCharData ( hPhone, techBand, rfmPath, channel, gain_state, agc_offset);
         }; // return agc_offset
         virtual unsigned char GetRxCharBandData ( HANDLE hPhone, const QSCNode & techBand, CPath_ChanResults & result )
         {
            return rxChar.GetRxCharBandData ( hPhone, techBand, result);
         }; // return agc_offset

   };
typedef struct QSCSwpParams
{
   int tech;
   int sweep_type;

   CCalType calType;
   
   QSCDefaultParams defaultParams; // Remove later (default parameters have been already copied into band info)
  
   std::vector < CBandInfo > BandInfo; // Store paramters for each of bands ( connected/Unconnected cal )
   std::vector < qscRecords > swps;
   Repository * repoPtr;
   QSCSwpParams ():
     tech (0),
     sweep_type (0),
	 repoPtr(NULL)
   {
      BandInfo.resize(0);
      swps.resize(0);
   }
} QSCSwpParams;
typedef struct CCONF_FILEPATHS
{
  std::string stParamXML;
  std::string stPAParamXML;
  std::string stCharXML;
  std::string stCalibratedXQCN;
  std::string stCalDBXML;
  std::string stEquipConfigXML;
  std::string stQMSLLog;
  std::string stRxCharXML;
  std::string stDLFLog;
  std::string stDMCFilePath;

  CCONF_FILEPATHS()
  {
     stParamXML = "";
     stPAParamXML = "";
     stCharXML = "";
     stCalibratedXQCN = "";
     stCalDBXML = "";
     stEquipConfigXML = "";
     stQMSLLog = "";
     stRxCharXML = "";
  };
} CCONF_FILEPATHS;

typedef struct CDeviceConf
{
   int type;
   int boardId;
   int primaryAddr;
   int secondaryAddr;
   unsigned char similation_mode;
   unsigned char matrix_mode;
   CDeviceConf ():boardId(0), secondaryAddr(0), matrix_mode(0), similation_mode(0) {};
} CDeviceConf;
typedef struct CXOCal_Config
{
   int tech;
   int band;
   int subband;
   int channel;
   ftm_xo_cal_parameter_type ftm_xo_cal_params;
   CXOCal_Config ():
      tech(0),
      band(0),
      subband(0),
      channel(0)
   {
      memset (&ftm_xo_cal_params, 0, sizeof ( ftm_xo_cal_parameter_type ));
   };
} CXOCal_Config;
typedef struct CPhoneInfo
{
   HANDLE hPhone;
   std::string stBuildID;
   CPhoneInfo():
      hPhone(NULL)
   {};
} CPhoneInfo;
typedef struct QSCParams
{
   int equipment_type;
   unsigned char bShow_ConsoleOutput;
   unsigned char bForce_Push_FBRxData_In_ConnectedCal; // Qlib Demo only for now. 
   CCONF_FILEPATHS FilePath;
   CDeviceConf DeviceConfig;
   CXOCal_Config XOCalConfig;
   CPhoneInfo DutInfo;
   std::map < QSCNode, QSCSwpParams > qscParams;
   RFCal::Repository repository;
   QSCParams():equipment_type(0), bShow_ConsoleOutput ( true ){};
} QSCParams;



typedef struct GSM_V4_BandChainChan_Params
{
   // band/channel information
   unsigned int                  m_numBands;    // 4 max
   vector<int>          m_bandList;
   unsigned int                  m_numChan;     // number of channels per band for algorithms in which this is the same for each band
   vector<vector< int>>  m_chanList;    // channel vector for each band index

   vector<int>          m_numChanPerBand;  // the # of channels per band in m_bandList for algorithms in which this varies per band

   // chain information - Tx and Rx are the same as of now
   unsigned int         m_numTxChains;
   unsigned int         m_numRxChains;
   vector<unsigned int> m_txChainList;
   vector<unsigned int> m_rxChainList;

   bool m_get_nv_container_from_uut_rx;     // if yes, the NV container to use is determined by an FTM API
   bool m_get_nv_container_from_uut_tx;     // if yes, the NV container to use is determined by an FTM API
} GSM_V4_BandChainChan_Params;
typedef struct GSM_V4_Meas_Info
{
   unsigned short numExpTxMeas_CB;          // the total number of expected Tx measurements for the entire sweep from the call box perspective
   unsigned short numContiguousDPD_Meas;    // the number of contiguous DPD measurements per channel
   unsigned short numContiguousDA_Meas;     // the number of contiguous DA Cal measurements per channel
   unsigned short numTotalDPD_Meas;         // number of total expected DPD measurements per sweep
   unsigned short numTotalDA_Meas;          // number of total expected DA measurements per sweep
   unsigned short numContiguousDPD_Meas_CB; // the number of contiguous DPD measurements per channel from the call box perspective
} GSM_V4_Meas_Info;
typedef struct GSM_V4_Rx_Params
{
   int performRxCal;                   // indicates whether to perform Rx Cal
   bool write_to_consolidated;        //new param to determine if it is suppose to be written to the consolidated NV, set it in the tree
   bool performDRxCal;                 // inidcates whether ANY band is performing DRx calibration
   bool performHiLinRxCal;             // inidcates whether to perform hi linearity calibration
   bool do_alt_path_cal;               // indicates whether alternate path calibration is done.

   // The call box can also set this to true, post FTM command configuration. It will set it true, if the call box is incapable of transmitting a downlink signal
   // for the first Rx measurement.
   // Set to false by default. Currently, only the call box updates this.
   bool m_skipFirstRxMeas;

   vector<bool> performDRxCalList;     // band-enumerated DRx calibraiton indicator

   unsigned int rx_alt_path;         // the altnernate path to use if do_alt_path_cal is true

   // Rx LNA states
   unsigned int         numLNAState;
   unsigned int         numLnaStateHiLin;
   vector<unsigned int> lnaStateList;  // valid values are 0 to 4
   vector<unsigned int> lnaStateListHiLin; //storage for LNA states at which hi linearity CAL is done.

   unsigned short numExpectedPayloads; // the number of payloads expected for the Rx results from the UUT

   // Rx LNA powers
   unsigned int numLNA_Power;
   unsigned int numLNA_PowerHiLin;
   vector<int> lnaPowerList;
   vector<int> lnaPowerListHiLin;

   // map of LNA state to corresponding cell power. LNA state of the alternative linearity mode is bumpped up by 128.
   //For example, for LNA state 1 in the alternative linearity mode, the state value = 1 + 128 = 129.
   vector<unsigned int> lnaStateListAllMode;
   vector<int>          lnaPowerListAllMode;

   // Gain Offset limits
   vector<vector<int>> gainOffset_MinLim_List;
   vector<vector<int>> gainOffset_MaxLim_List;
   vector<vector<int>> gainOffset_MinLim_List_DRx;
   vector<vector<int>> gainOffset_MaxLim_List_DRx;
   GSM_V4_Rx_Params():
      performRxCal (0)
      {};
} GSM_V4_Rx_Params;
 typedef multimap<unsigned int, unsigned int> LNARSSI_type;
 typedef struct{ LNARSSI_type data; } LNARSSI;
 typedef map<unsigned int, LNARSSI> ChanLNARSSI_type;
 typedef struct { ChanLNARSSI_type data; } ChanLNARSSI;
 typedef map<unsigned int, ChanLNARSSI> BandChanLNARSSI_type;
 typedef struct { BandChanLNARSSI_type data; } BandChanLNARSSI;
 typedef map<unsigned int, BandChanLNARSSI> ChainBandChanLNARSSI_type;
 
 typedef map<int, map<int, double> > GSMRxCharTable; // map< channel, map<LNA state, Gain Offset> >

typedef struct TE_GSMV4_Sweep_Params
{
   vector< vector< int > > chanVec; // list of channels per band
   vector< int > bandVec;         // band list

   vector< int > rxPowerList;              // rx Power List

   // 0 = 8PSK
   // 1 = GMSK
   vector< unsigned short> modList;       // modulation list for DA/PCL cal
   unsigned short dpdMod;                // modulation used for DPD portion of sweep: 0=8PSK, 1=GMSK

   unsigned short numBand;
   // Used for case, where the number of channels per band is the same for each band
   // this parameter is useless for nonChar Rx-only cal.
   unsigned short numChanPerBand;        // # of channels per band

   unsigned short numTotalFrames;        // # of total frames from the CB perspective for the sweep
   unsigned short numDaFramesPerChan;    // # of DA/PCL frames per channel
   unsigned short numDpdFramesPerChan;   // # of DPD frames per channel

   unsigned short intraBandFrameDelay;   // # of frames of delay between band changes in modem software

   unsigned short txSlotShiftAmt;        // # of slots by which Tx slots are shifted relateive to Rx slots in modem software

   unsigned char rxSlotOffset;

   // indicate whether DA/PCL, DPD, or Rx cal are to be performed
   bool performDaCal;
   bool performDpdCal;
   bool performRxCal;
   bool performHiLinRxCal;
   bool performNonCharRxOnlyCal;
   // indicates whether DA/PCL and DPD frames should be segregated per channel
   // Yes --> Perform DA/PCL for all channels for band X followed by performing DPD for all channels for Band X (6 channel changes per band)
   //    -- this allows for any # of DPD frames to occur
   // No  --> Perform DA/PCL follwed by DPD for channel 1 to N for Band X (3 channel changes per band)
   //    -- this allows for only a # of DPD frames evenly divisible by 4 to occur
   bool segregateDaAndDpdPerChan;

   vector<unsigned char> slotConfig;

   // initializer list
   TE_GSMV4_Sweep_Params() :
      dpdMod(0),
      numBand(0),
      numChanPerBand(0),
      numTotalFrames(0),
      numDaFramesPerChan(0),
      numDpdFramesPerChan(0),
      intraBandFrameDelay(0),
      txSlotShiftAmt(0),
      rxSlotOffset(0),
      performDaCal(true),
      performDpdCal(true),
      performRxCal(true),
      performHiLinRxCal(false),
      segregateDaAndDpdPerChan(true)
   {}
} TE_GSMV4_Sweep_Params;


typedef struct GSM_V4_DPD_Params
{
   // indicated whether to perform DPD Cal
   int performDpdCal;

   // if enabled, only the predistortion RGI selection for GSM V4 cal is done.
   // This is part of V3.5 cal.
   int doRGIRunOnly;

   // if enabled, V3 char data is used for predist RGI selection along with V4 char data.
   unsigned char useV3CharData;

   // bools for slope and power adaptation
   unsigned char doSlopeCorrectionAlignLow;
   
   unsigned char m_performDaFrameSweepCal;
   int doSlopeCorrection;
   int doPiecewiseAdapt;
   int doPowerAdaptation;
   
   int doAdaptiveMaxPowEst;

   // predistortion RGI selection method
   int useDaAdaptation;

   // DAC scaling factor for NV processing
   double dacScale;

   // modulation type to use for digGain/RGI sweep
   unsigned int digGainForPredistRGI;

   // PA range at which DPD cal occurs
   unsigned int calPaRange;

   // modulation type for DPD cal
   unsigned int dpdModType;    // 0 = 8PSK, 1 = GMSK

   // power of the RGI calibration
   double calRGI_PowerHighBand;
   double calRGI_PowerLowBand;

   // measurement error checking
   // reflects the minimum power delta between 2 digital gains for a constant RGI
   double minDpdPowDelta;

   // RGI list for doing DPD cal
   vector<vector<unsigned int>> dpd_RgiList;
   unsigned int numDpd_Rgi;

   // Digital Gain list for DPD cal
   vector<vector<unsigned int>> dpd_DigGainList;
   unsigned int numDpd_DigGain;

   // example 1 for F1, 2 for F2 etc.
   vector<unsigned int> nvChanMap;

   // digital gain list to use for slope correction
   vector<vector<unsigned int>> slopeCorrectionDigGainList;


   GSM_V4_DPD_Params():
   useV3CharData(0),
	   useDaAdaptation(0),
   doAdaptiveMaxPowEst(0),
   doPiecewiseAdapt(0),
   doSlopeCorrectionAlignLow(0),
   m_performDaFrameSweepCal(0),
   dpdModType(1){};
} GSM_V4_DPD_Params;

typedef struct GSM_V4_DA_Params
{
 // indicated whether to perform DA Cal
   int performDaCal;

   // Yes -> write the PCL error to the NV
   // No -> adapt all DA cal data based upon PCL error, and rewrite the adapated DA data to the NV
   bool usePclErrorNV;

   // PCL NV target power offset
   double pclNvPowOffset;

   // error checking
   // represents the maximum PCL delta allowed
   // in this case PCL delta = abs(measured PCL power - target PCL power)
   double maxPclPowDelta;

   // PCL lists
   unsigned int numPCLs;
   //vector<unsigned int> numPCLs;             // numPCls per band

   // Note: PCL indexes refer to the PCL power list NVs
   vector< vector<unsigned int> >   pclList;       // list of PCLs per band index
   vector< vector<unsigned short> > pclIdxList;    // list of PCL indexes per band index

   // Maps the PCL value to the PCL index used by Qualcomm (for the PCL NV)
   // map<pcl value, pcl Index>
   map<unsigned short, unsigned short> lowBandPCl_map;
   map<unsigned short, unsigned short> highBandPCl_map;

   // modulations
   unsigned int numMods;               // valid values are 1 or 2
   vector<unsigned int> modList;       // Valid values are 1 to 9
   vector<unsigned short> modIdxList;  // Valid values are 0 to 8
   
   vector<double> m_minGsmDaPwrLimits;
   vector<double> m_maxGsmDaPwrLimits;
   vector<double> m_minEdgeDaPwrLimits;
   vector<double> m_maxEdgeDaPwrLimits;

   GSM_V4_DA_Params():
   performDaCal (0)
   {};
} GSM_V4_DA_Params;
typedef struct GSM_V4_Intermediate_variables
{
  unsigned short m_predistCalRgi; 
  unsigned short m_dutDataRgi;
} GSM_V4_Intermediate_variables;
typedef struct GSMCharDPD_TableKey
{
public:
   // constructor for multi-chain RCCRTable
   GSMCharDPD_TableKey(int rfModeIn, int chanIn, int chainIn, int rgiIn)
   {
      m_rfMode   = rfModeIn;
      m_channel  = chanIn;
      m_chain    = chainIn;
      m_rgi      = rgiIn;
   }
   int m_rfMode;    // rf_mode enum - 18,10,11,12
   int m_channel;
   int m_chain;     // Tx/Rx chain ID (0 or 2)
   int m_rgi;
   bool operator<(const GSMCharDPD_TableKey& rhs) const
   {
      if( m_rfMode == rhs.m_rfMode )
      {
         if( m_channel == rhs.m_channel )
         {
            if( m_chain == rhs.m_chain )
            {
               return m_rgi < rhs.m_rgi;
            }
            else
            {
               return m_chain < rhs.m_chain;
            }
         }
         else
         {
            return m_channel < rhs.m_channel;
         }
      }
      else
      {
         return m_rfMode< rhs.m_rfMode;
      }
   }
} GSMCharDPD_TableKey;
class  OpPwrTableGeneratorV2 
{
public:
   bool isDataPresent(void)         {  return m_getOpPwrDataPresent;  }
   bool isV4DataPresent(void)       {  return m_v4DigGainDataPresent; }
   bool isV3DataPresent(void)       {  return m_v3DigGainDataPresent; }
   double getDigGainDelta_V4_minus_V3( unsigned int rfMode ) { return m_V4_minus_V3_Char_Delta_dB_Map[rfMode]; }
   bool getTables( int rfMode, int channel, int chain, int rgi, vector<struct opPwrRecord>& retOpPow, vector<struct digRecord>& retDigRec );
   bool getTablesV4( int rfMode, int channel, int chain, int rgi, vector<struct opPwrRecord>& retOpPow, vector<struct digRecord>& v3RetDigRec, vector<struct digRecord>& v4RetDigRec );
   void getDigGainRGI_PowerMap( int rfMode, int channel, int chain, map< int, vector<struct digRecord> >& powerMap, bool getV4PowerMap = true );

   map<GSMCharDPD_TableKey, vector<struct opPwrRecord>>::iterator getPwrBegin()  {  return m_pData.begin();   };
   map<GSMCharDPD_TableKey, vector<struct opPwrRecord>>::iterator getPwrEnd()    {  return m_pData.end();   };
   map<GSMCharDPD_TableKey, vector<struct digRecord>>::iterator getDigBegin( bool getV4Data = false ) { return (getV4Data)? m_dData_V4.begin() : m_dData_V3.begin(); };

   map<GSMCharDPD_TableKey, vector<struct digRecord>>::iterator getDigEnd( bool getV4Data = false ){ return (getV4Data)? m_dData_V4.end() : m_dData_V3.end(); };
//         static OpPwrTableGeneratorV2* getObject();

   OpPwrTableGeneratorV2(void)
   {
      m_getOpPwrDataPresent  = false;
      m_v4DigGainDataPresent = false;
      m_v3DigGainDataPresent = false;

      m_V4_minus_V3_Char_Delta_dB_Map[10] = 0.0;
      m_V4_minus_V3_Char_Delta_dB_Map[18] = 0.0;
      m_V4_minus_V3_Char_Delta_dB_Map[11] = 0.0;
      m_V4_minus_V3_Char_Delta_dB_Map[12] = 0.0;
   }
    protected:
     // vector<struct digRecord> createDigTable(MSXML2::IXMLDOMNodePtr digRecs, bool wantV4Tag );
     // virtual bool readSchemaXML(const MSXML2::IXMLDOMDocumentPtr iXMLDoc);

      map< GSMCharDPD_TableKey, vector<struct opPwrRecord> > m_pData;
      map< GSMCharDPD_TableKey, vector<struct digRecord> > m_dData_V3;
      map< GSMCharDPD_TableKey, vector<struct digRecord> > m_dData_V4;

      bool m_getOpPwrDataPresent;
      bool m_v4DigGainDataPresent;
      bool m_v3DigGainDataPresent;

      map<unsigned int, double> m_V4_minus_V3_Char_Delta_dB_Map;
};

typedef struct CGSM_V4_Cal_Params 
{

   GSM_V4_BandChainChan_Params m_bcc;  // the band/chain/channel information

   GSM_V4_DPD_Params m_DPD_Params;
   GSM_V4_DA_Params  m_DA_Params;
   GSM_V4_Rx_Params  m_Rx_Params;
   GSM_V4_FBRx_Params m_FBRx_Params;
   
   GSM_V4_Intermediate_variables m_globals; // only for qlibdemo to store intermediate variables. this can be implemented as member variables in a class

   // call box related parameters
   TE_GSMV4_Sweep_Params swpParams;
   CGSM_Tx_Sweep_Cfg_Params_TE m_sweepParamsTestEquip;
   double                      m_measStartDelay_ms;     // measurement start delay
   unsigned char               m_txShiftAmt;            // Tx shift amount on the phone --> affects the segment setup in the call box
   unsigned char               m_intraBandSegDelay;     // delay between band changes during cal sweep --> this only affects the call box segment setup

   // Slot Functions
   // 0->NA (do nothing)
   // 1->Rx
   // 2->Tx
   // Slot Number: 0 to 7, indicating position of the slot in a TDMA frame
   // slots - the same slot configuration is used for both DA cal and DPD cal
   unsigned int          m_txNumSlots;          // 4 max
   unsigned int          m_rxNumSlots;          // 1 max
   vector<unsigned char> m_TxSlotConfig;        // the slot configuration with respect to the phone sweep configuration
   // The phone puts chains 0 and 2 in the same slot number. They are shifted on the phone.
   vector<unsigned char> m_RxSlotConfig;        // Rx slot configuration. This is the same for the call box and UE
   unsigned char         m_rxSlotOffset;        // the location of the rx slot in m_RxSlotConfig

   vector<unsigned char> m_TxSlotConfig_CB;     // the slot configuration with respect to the call box.

   OpPwrTableGeneratorV2* m_pwrCharTablePtr;
   // functions
   CGSM_V4_Cal_Params(): 
      m_txShiftAmt(GSM_V4_TX_SLOT_SHIFT_ON_PHONE),
	   m_intraBandSegDelay(GSM_V4_INTRA_BAND_SEG_DELAY),
      m_pwrCharTablePtr(NULL)
      {};
   //void                Init();
   //void                Compute_NumFrames();
   //void                Compute_MeasInfo();
   //inline unsigned int Get_NumFrames();
   //inline unsigned int Get_NumFrames_CB();
   //inline unsigned int Get_NumFramesPerChan();
   //inline unsigned int Get_NumDaFramesPerChan();
   //inline unsigned int Get_NumDpdFramesPerChan();
   //inline void         Get_MeasInfo( GSM_V4_Meas_Info& measInfo );

   //// TODO_GSMV4 - combine these 3 functions - tradeoff in performance if combined?
   //void Create_GSM_V4_Cal_Command_FrameDuplication( unsigned char gsmV4CmdBuff[], unsigned short& gsmV4CmdSize,
   //                                                 unsigned short numFrameDup );
   //void Create_GSM_V4_Cal_Command( unsigned char gsmV4CmdBuff[], unsigned short& gsmV4CmdSize,
   //                                map< unsigned int, vector<unsigned short> >& RX_BURST_FRAME_IDENTIFIER );
   //void Create_GSM_V4_Cal_Command_Rx_Only( unsigned char gsmV4CmdBuff[], unsigned short& gsmV4CmdSize,
   //                                        map< unsigned int, vector<unsigned short> >& RX_BURST_FRAME_IDENTIFIER );
   //void Create_MCS_Indexes();
   //void Create_PCL_Indexes();

   //// map of pcl value to pcl index for high bands and low bands
   //void Create_PCL_Map();
   //void Clear_PCL_Maps();

   //// fills the m_dpd_digGain_MapVec and m_dpd_RGI_MapVec containers for DUT command configuration purposes
   //void BuildDpdSweepContainers( bool reverseSweepOrder = false );

   //! controls NV logging and writing to UUT, bitmask of RFCAL_NV_ACTION_TYPE bits
   unsigned int m_nv_action;
   int m_logCalMeasData;

   // the DPD Sweep Configuration - RGI vs Digital Gain -- allow for having an number of DPD sweep slots that is not evenly divisible by m_txNumSlots
   map< unsigned int, vector<unsigned char> > m_dpd_digGain_MapVec;
   map< unsigned int, vector<unsigned char> > m_dpd_RGI_MapVec;

   bool m_computeNV;

   int m_logDebugWaveform;

   unsigned int m_numFrames;              // The # of frames from the UE perspective. Exludes intrabandFrameDelay - has a maximum value of GSM_V4_MAX_NUM_OF_FRAMES
   unsigned int m_numFramesForCB;         // # of frames from the CB perspective. Includes intrabandFrameDelay. - Max values is limited by the call box only
   unsigned int m_numDaFramesPerChan;     // number of DA frames per channel
   unsigned int m_numDpdFramesPerChan;    // the number of DPD sweep (digGain/RGI) frames per channel
   unsigned int m_numFramesPerChan;       // the number of total frames per channel

   // Tx/Rx measurement parameters
   GSM_V4_Meas_Info  m_MeasInfo;

} CGSM_V4_Cal_Params;
typedef struct
{
   map<unsigned char, GSM_V4_FBRx_GAIN_RESULT_TYPE> data;
} CGSM_fbrxGainVsResult_type;

typedef struct 
{
   map<unsigned int, CGSM_fbrxGainVsResult_type> data;
} CGSM_channelVsResult_type;

typedef struct 
{
   map<unsigned int, CGSM_channelVsResult_type> data;
} CGSM_bandVsResult_type;

typedef struct 
{
   map<unsigned int, CGSM_bandVsResult_type> data;
} CGSM_ChainToFBRxResult_type;

typedef struct GSM_CAL_Params
{
   FTM_RF_Mode_Enum rf_mode;
   nv_mode_id_type m_nv_mode;
   int m_txNvContainer;
   int m_txAntPath;

   unsigned short chainID;
   unsigned char channel;
   unsigned char query_path_from_rfc;
   unsigned char performDACal;
   unsigned char performPreDistCal;
   unsigned char performRxCal;
   unsigned char txWriteChainEnumeratedNv;
   unsigned char writeConsolidatedTxNvVersion;
   unsigned char writeConsolidatedRxNvVersion;
   unsigned int setRFMPathVersion;
   unsigned char m_enGsmFtmRfModeExit;

   vector < int > band_list;
   vector < vector < int > > channel_list;
   vector < int > nv_mode_list;
   vector < int > nv_mapping;
   GSM_DA_CAL_Params daCalParams;
   EDGE_PreDist_Cal_Data preDistCalParams;
   CEDGE_PreDist_Cal_Data preDistCalParamsV5;
   GSM_Rx_CAL_Params rxParams;
   CGSM_V4_Cal_Params gsm_v4_Params;
   CGSM_Cal_Device_SigPath sigPaths;
   CDeviceConf DeviceConfig;

   // file paths
   CCONF_FILEPATHS FilePath;
   RFCal::Repository repository;

   GSM_CAL_Params(): 
   query_path_from_rfc(1),
      m_txNvContainer(0),
      performRxCal(0),
      performDACal(0),
      performPreDistCal(0)
      {};
} GSM_CAL_Params;


typedef struct 
{
   //map<rgi, pwr>
   std::map < unsigned short, double > rgiPwr;
} GSM_DA_Cal_Sweep_Record;

typedef struct 
{
   //map<digGainNv, pwr>
   map<unsigned short,double> digGainPwr;
} GSM_DA_Cal_DigGain_Record;

typedef struct GSM_DA_Cal_Results
{
   //map<paRange, map<rgi, pwr>>
   std::map<unsigned short,GSM_DA_Cal_Sweep_Record> edgeDaSweep;
   //map<paRange, map<rgi, pwr>>
   std::map<unsigned short,GSM_DA_Cal_Sweep_Record> gsmDaSweep;
   // map<digGainNv(mode), pwr>
  // map<unsigned short,GSM_DA_Cal_DigGain_Record> digGainSweep;
   std::map<unsigned short,GSM_DA_Cal_Sweep_Record> digGainSweep;

} GSM_DA_Cal_Results;

typedef struct
{
   std::map<unsigned short, GSM_DA_Cal_Results> channelSweepData;
   //map<chan,TriggerLevel>
   std::map<unsigned short, double> chanTriggerLevel;
} GSM_DA_Cal_Results_Channel;

//-------------------------------------------- CGSM_V4_Cal_Alg - Measurement Results --------------------------------------//

//---------- DA Cal PCL error results
// maps PCL index to the difference between expected power and measured power for that PCL index
struct GSM_V4_PCL_Sweep_Record
{
   // map<PCL index, pwr delta>
   map<unsigned short, double> pclPwrDelta; // the PCL power delta in dBm (measured power - expected power)
   map<unsigned short, double> pclMeasPwr;  // measured power at given PCL index in dBm
};

struct GSM_V4_PCL_Results_Channel
{
   // map<channel, map<PCL index, pwr delta>>
   map<unsigned short, GSM_V4_PCL_Sweep_Record> edgePCLSweep;
   // map<channel, map<PCL index, pwr delta>>
   map<unsigned short, GSM_V4_PCL_Sweep_Record> gsmPCLSweep;
};

//map<band, map<channel, map<pcl index, pwr delta>>>
struct GSM_V4_PCL_Results_Band_Chan
{
   // map<band, GSM_V4_PCL_Results_Channel>
   map<unsigned short, GSM_V4_PCL_Results_Channel> bandSweepData;
};

//---------- Characterized Rx Results
struct GSM_V4_PCL_Results_Chain
{
   // map<Tx chain, GSM_V4_PCL_Results_Band_Chan>
   map<unsigned int, GSM_V4_PCL_Results_Band_Chan> chainData;
};

//---------- chain enumerated NV Results
struct GSM_V4_Tx_Cx_NV_Results
{
   // map<Tx chain, map< band, NV results for that band> >
   map< unsigned int, map<unsigned int, QMSL_GSM_Tx_Calibration_NV> > chainNV;

   GSM_V4_Tx_Cx_NV_Results()
   {
      chainNV.clear();
   };
};

//----------- structures for organizing consolidated NV Results
struct GSM_V4_TxConsolidatedNV_RFMDev
{
   //map< RFM Dev, QMSL_GSM_Tx_Calibration_Consolidated_NV >
   map< unsigned short, QMSL_GSM_Tx_Calibration_Consolidated_NV > data;

   GSM_V4_TxConsolidatedNV_RFMDev()
   {
      data.clear();
   }
};

struct GSM_V4_TxConsolidatedNV_SigPath
{
   //map< Signal Path, GSM_V4_TxConsolidatedNV_RFMDev >
   map< unsigned int, GSM_V4_TxConsolidatedNV_RFMDev > data;

   GSM_V4_TxConsolidatedNV_SigPath()
   {
      data.clear();
   }
};

struct GSM_V4_TxConsolidatedNV_AntPath
{
   //map< Antenna Path, GSM_V4_TxConsolidatedNV_SigPath >
   map< unsigned short, GSM_V4_TxConsolidatedNV_SigPath > data;

   GSM_V4_TxConsolidatedNV_AntPath()
   {
      data.clear();
   }
};

struct GSM_V4_Tx_Consolidated_NV_Results
{
   // map<band, GSM_V4_TxConsolidatedNV_AntPath>
   map< unsigned int, GSM_V4_TxConsolidatedNV_AntPath > data;

   GSM_V4_Tx_Consolidated_NV_Results()
   {
      data.clear();
   }
};


typedef struct GSM_RX_MEAS
{
   int m_rssi;
   int m_lna;
} GSM_RX_MEAS_TYPE;


typedef struct GSM_RX_MEAS_Band
{
	map < int, vector <GSM_RX_MEAS_TYPE> > data;
	
}GSM_RX_MEAS_Band;

typedef struct GSM_RX_MEAS_Lin
{
	map <unsigned int, GSM_RX_MEAS_Band> data;
	GSM_RX_MEAS_Lin()
	{
		data.clear();
	}
}GSM_RX_MEAS_Lin;
typedef struct GSM_RX_MEAS_Dev
{
	map < unsigned short, GSM_RX_MEAS_Lin > data;
		GSM_RX_MEAS_Dev()   
	{
		data.clear();
	}
}GSM_RX_MEAS_Dev;

typedef struct GSM_RX_MEAS_ChainID
{
	map < unsigned int, GSM_RX_MEAS_Dev > data;
	GSM_RX_MEAS_ChainID()   
	{
		data.clear();
	}
}GSM_RX_MEAS_ChainID;

typedef struct GSM_RX_MEAS_AntPath
{
	map < unsigned short, GSM_RX_MEAS_ChainID> data;
	GSM_RX_MEAS_AntPath()   
	{
		data.clear();
	}
}GSM_RX_MEAS_AntPath;


//---------- Characterized Rx Results
struct GSM_V4_Rx_Results_Chan
{
   // map< channel, map< LNA state, Average RSSI values > >
   map<unsigned int, map<unsigned int, unsigned int>> chanToLNA_AvgRSSI;
};

struct GSM_V4_Rx_Results_Band_Chan
{
   // map< band, GSM_V4_Rx_Results_Chan>
   map<unsigned int, GSM_V4_Rx_Results_Chan> rssi_Map;
};

struct GSM_V4_Rx_Results_Chain
{
   // map<Tx chain, GSM_V4_Rx_Results_Band_Chan>
   map<unsigned int, GSM_V4_Rx_Results_Band_Chan> chainData;
};

struct GSM_V4_Rx_RSSI_Estimates_Band_Chan
{
   // map< band, map<channel, vector<lna state, estRSSI>
   map<unsigned int, map<int, vector<GSM_RX_MEAS_TYPE>>> estRSSI_Map;
};

struct GSM_V4_RSSI_Est_Chain
{
   // map<Tx chain, GSM_V4_Rx_RSSI_Estimates_Band_Chan>
   map<unsigned int, GSM_V4_Rx_RSSI_Estimates_Band_Chan> chainData;
};
struct GSM_V4_Rx_Cx_NV_Results
{
   // map<Rx chain, map< band, NV results for that band> >
   map< unsigned int, map<unsigned int, QMSL_GSM_Rx_Calibration_NV> > chainNV;

   GSM_V4_Rx_Cx_NV_Results()
   {
      chainNV.clear();
   };
};
struct GSM_V4_Rx_Cx_Cal_Meas
{
   // map<Rx chain, map< band, measurement results for that band > >
   map< unsigned int, map<unsigned int, QMSL_GSM_Rx_Calibration_Params_and_Meas> > chainMeas;

   GSM_V4_Rx_Cx_Cal_Meas()
   {
      chainMeas.clear();
   };
};

//---------- DPD Measurement Results
struct GSM_V4_DPD_Sweep_Record
{
   // map< Digital Gain, map< RGI, power> >
   map< unsigned short, GSM_DA_Cal_DigGain_Record > digGainSweep;
};

struct GSM_V4_DPD_Results_Band_Chan
{
   // map< band, map<channel, GSM_V4_DPD_Sweep_Record> >
   map< unsigned int, map<unsigned int, GSM_V4_DPD_Sweep_Record> > band_Chan_sweepResults;
};

struct GSM_V4_DPD_Results_Chain
{
   // map<Tx chain, GSM_V4_DPD_Results_Band_Chan>
   map<unsigned int, GSM_V4_DPD_Results_Band_Chan> chainData;
};

typedef struct GSM_CAL_Results
{
   PreDist_Capture_Record_Channel * calibratedPreDistData;
   GSM_DA_Cal_Results_Channel da_cal_res;
   map < int, map< int, vector<GSM_RX_MEAS_TYPE> > > rxPath_chan_rssis_list;
   map < int, map< int, vector<GSM_RX_MEAS_TYPE> > > rxPath_chan_rssis_list_hi_lin;
   
   uint16 therm;
   GSM_V4_RSSI_Est_Chain gsmV4_RSSI_Results;
   CGSM_ChainToFBRxResult_type chainToFBRxResult;
   GSM_V4_FTM_CMD_Response_Type gsmV4_measResults;
   vector<double> outputPowVec;
   map<int, ftm_fbrxgain_path_result > fbrx_result; // Band - FBRx result map

   GSM_CAL_Results():
      calibratedPreDistData(NULL),
      therm(0){};
} GSM_CAL_Results;

typedef struct CTechBand
{
   int tech;
   int band;
   int subBand;
   CTechBand(int _tech, int _band, int _subBand):tech(_tech), band(_band), subBand(_subBand) {};
} CTechBand;

};