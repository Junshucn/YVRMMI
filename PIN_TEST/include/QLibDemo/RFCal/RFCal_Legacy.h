/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/RFCal_Legacy.h#2 $
 * $DateTime: 2017/09/11 22:55:00 $
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
#include <typeinfo>
#import <msxml6.dll> 
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <list>
#include <stdlib.h>
#include <io.h>   // For access().
#ifndef __QLIB_H
#define __QLIB_H
#include "QLib.h"
#endif

#ifndef __QLIB_DEFINES_H
#define __QLIB_DEFINES_H
#include "QLib_Defines.h"
#endif

using namespace std;

// Self cal, FTM_RFNV_RELOAD
unsigned char Test_FTM_Common( );

// Self cal
unsigned char Test_FTM_DO_SELF_CAL();

// Serial cal
unsigned char Test_RFCal_V2();

// SCell Rx only serial cal
unsigned char testLTECA_B29SCELLCal();

// TxRsb_TxLo cal
unsigned char Test_RFCal_TxRsb_TxLo_Internal_Cal();

unsigned char Test_RFCal_V3();
unsigned char Test_RFCal_V3_LTE();
unsigned char Test_RFCal_V3_TDSCDMA();
unsigned char Test_RFCal_V3_CDMA();

unsigned char Test_FTM_WCDMA_ICI_CAL( void );
unsigned char Test_FTM_WCDMA_ICI_IQ_CAL(void);
unsigned char Test_FTM_WCDMA_IQ_MISMATCH_CAL( void );
unsigned char Test_FTM_DIVERSITY_CAL( void );

//Dual carrier
unsigned char Test_Dual_Carrier();

// Dual carrier + ICI
unsigned char WCDMA_Daul_Carrier_and_ICI_Cal( );

unsigned char Test_FTM_CAL_API_V2 (void );

// ESC Parser for ESC V3
unsigned char ESC_Parser(bool parseRes);
void Test_ESC_Parser();

// Helper
unsigned long ConvertUint16HextoBinary( word hexVal );
unsigned long ConvertUint8HextoBinary( byte hexVal );
unsigned long ConvertUint32HextoBinary( dword hexVal );

// Offset for new RFNV items
const unsigned int newRfNvItemOffset = 20000;
const unsigned int maxRfNvItemId = 65535;


unsigned char Test_GSM_IP2_Cal();

/*****************************GSM IP2 cal starts************************************/

typedef struct
{
    unsigned char use_default_params;
    unsigned char band;
    short startICode;
    short startQCode;
    unsigned char unitStepSize;
    unsigned char width;
    unsigned short ARFCN;
    unsigned char txRgi;
    unsigned char rxLNARange;
    unsigned char numberOfIteration;
    unsigned short stepDuration;
    unsigned short dcSettlingTime;
}GSM_IP2_Params;


typedef struct
{
   int charChannel;
   int charRGI;
   double charAMAM[128];
   double charPwr[128];
   double charAMPM[128];
   int charDigGain[4];
   double charDigPwr[4];
} char_DPD_Tbl;

typedef struct
{
   int charChannel;
   int charRGI;
   double charAMAM[128];
   double charPwr[128];
   double charAMPM[128];
   int charDigGain_V3[4];
   double charDigPwr_V3[4];
   int charDigGain_V4[5];
   double charDigPwr_V4[5];
} char_DPD_Tbl_V3_V4;




typedef struct
{
   int iChanNum;
   int iLNAState[4];
   double fGain[4];
} char_Rx_Tbl;

struct GSM_V4_Rx_Results_Chan
{
   // map< channel, map< LNA state, Average RSSI values > >
   map<unsigned int, map<unsigned int, unsigned int > > chanToLNA_AvgRSSI;
};

// structures for organizing the parsed XML data
struct GSMDAChar_RGI
{
   // map< RGI, dBm*100>
   map< int, double > rgiVsPwr;
};
struct GSMDAChar_PA
{
   // map< PA Range, GSMDAChar_RGI>
   map< int, GSMDAChar_RGI > data;
};
struct GSMDAChar_Mod
{
   // map< modulation, GSMDAChar_PA >
   map< int, GSMDAChar_PA > data;
};
struct GSMDAChar_Chan
{
   // map< channel, GSMDAChar_Mod >
   map< int, GSMDAChar_Mod > data;
};
struct GSMDAChar_Chain
{
   // map< chain, GSMDAChar_Chan>
   map< int, GSMDAChar_Chan > data;
};
struct GSMDAChar_RfMode
{
   // map< rfMode, GSMDAChar_Chain>
   map< int,GSMDAChar_Chain > data;
};
struct GSM_V4_Rx_Results_Band_Chan
{
   // map< band, GSM_V4_Rx_Results_Chan>
   map<unsigned int, GSM_V4_Rx_Results_Chan> rssi_Map;
};

typedef struct
{
   bool boolPerformDA;
   bool boolUsePCLErrorNV;
   unsigned int uiNumMods;               // valid values are 1 or 2
   vector<unsigned int> uiModList;       // Valid values are 1 to 9
   vector<unsigned short> usModIdxList;       // Valid values are 0 to 8
   // PCL NV target power offset
   double dPclNvPowOffset;

   // error checking
   // represents the maximum PCL delta allowed
   // in this case PCL delta = abs(measured PCL power - target PCL power)
   double dMaxPclPowDelta;
   unsigned int uiNumPCLs;
   vector< vector<unsigned int> >   uiPclList;       // list of PCLs per band index
   vector< vector<unsigned short> > usPclIdxList;    // list of PCL indexes per band index

   // Maps the PCL value to the PCL index used by Qualcomm (for the PCL NV)
   // map<pcl value, pcl Index>
   map<unsigned short, unsigned short> usBandPCl_map;
} GSM_V4_DA_Params;

typedef struct
{
   bool boolPerformDPD;
   bool boolSlopeAlignLow;
   bool boolDoSlopeCorrection;
   bool boolDoPieceWiseLinear;
   bool boolDoPowerAdaptation;
   bool boolDoAdaptiveMaxPwrEst;
   unsigned short usDigGainForPredistRGI;

   vector<vector<unsigned int> > uiDpdRgiList;
   unsigned int uiNumDpdRgi;

   vector<vector<unsigned int> > uiDpdDigGainList;
   unsigned int uiNumDpdDigGain;

   double dCalRGIPowerHighBand;
   double dCalRGIPowerLowBand;
   unsigned int uiCalPaRange;
   double dDacScale;

   vector<unsigned int> uiNvChanMap;
   vector<vector<unsigned int> > uiSlopeCorrectionDigGainList;
   double dMinDpdPowDelta;
   map< unsigned int, vector<unsigned char> > dpdDigGainMapVec;
   map< unsigned int, vector<unsigned char> > dpdRGIMapVec;

    bool boolUseV3CharData;
    bool boolDoRGIRunOnly;
    bool boolDAAdaptation;
    bool boolUseAdjacentRGI;
} GSM_V4_DPD_Params;

typedef struct
{
   bool boolPerformRxCal;
   unsigned int uiNumRxPower;
   vector<int> iRxPowerList;
   unsigned int uiNumLNAState;
   vector<unsigned int> uiLNAStateList;  // valid values are 0 to 4
   vector<vector<int> > iGainOffsetMinLimList;
   vector<vector<int> > iGainOffsetMaxLimList;

   bool boolPerformHiLinRxCal;
   unsigned int uiNumLNAStateHiLin;
   vector<unsigned int> uiLNAStateListHiLin;
   unsigned int uiNumLNAPowerHiLin;
   vector<int> iLNAPowerListHiLin;

   // map of LNA state to corresponding cell power. LNA state of the alternative linearity mode is bumpped up by 128.
   //For example, for LNA state 1 in the alternative linearity mode, the state value = 1 + 128 = 129.
   vector<unsigned int> lnaStateListAllMode;
   vector<int> lnaPowerListAllMode;

   bool boolPerformDRxCal; 
} GSM_V4_Rx_Params;

typedef struct
{
   unsigned int uiNumBands;               // 4 max
   vector<unsigned int> uiBandList;
   unsigned int uiNumChan;                // number of channels per band
   vector<vector<unsigned int> > uiChanList;

   unsigned int uiNumTxChains;
   unsigned int uiNumRxChains;
   vector<unsigned int> uiTxChainList;
   vector<unsigned int> uiRxChainList;

   GSM_V4_DA_Params gsmV4DAParams;
   GSM_V4_DPD_Params gsmV4DPDParams;
   GSM_V4_Rx_Params gsmV4RxParams;

   unsigned int uiNumDAFrames;
   unsigned int uiNumDPDFrames;
   unsigned int uiNumFrames;
   unsigned int uiNumFrames_CB;
   unsigned int uiNumDAFramesPerChannel;
   unsigned int uiNumDPDFramesPerChannel;
   unsigned int uiNumFramesPerChannel;
   unsigned int uiNumTxSlots;
   unsigned int uiNumRxSlots;
   vector<unsigned char> ucTxSlotConfig;
   vector<unsigned char> ucRxSlotConfig;
   unsigned char ucRxSlotOffset;

   bool boolSkipFirstRxMeas;
   vector<unsigned char> ucTxSlotConfig_CB;
} GSM_V4_Cal_Params;

struct GSM_DA_Cal_Sweep_Record
{
   //map<rgi, pwr>
   map<unsigned short,double> rgiPwr;
};

struct GSM_DA_Cal_DigGain_Record
{
   //map<digGainNv, pwr>
   map<unsigned short,double> digGainPwr;
};

struct GSM_DA_Cal_Results
{
   //map<paRange, map<rgi, pwr>>
   map<unsigned short,GSM_DA_Cal_Sweep_Record> edgeDaSweep;
   //map<paRange, map<rgi, pwr>>
   map<unsigned short,GSM_DA_Cal_Sweep_Record> gsmDaSweep;
   // map<digGainNv(mode), pwr>
   map<unsigned short,GSM_DA_Cal_DigGain_Record> digGainSweep;
};

//map<chan,map<paRange, map<rgi,Pwr>>>
struct GSM_DA_Cal_Results_Channel
{
   //map<chan,GSM_DA_Cal_Results>
   map<unsigned short, GSM_DA_Cal_Results> channelSweepData;
   //map<chan,TriggerLevel>
   map<unsigned short, double> chanTriggerLevel;
};

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

struct GSM_V4_PCL_Results_Chain
{
   // map<Tx chain, GSM_V4_PCL_Results_Band_Chan>
   map<unsigned int, GSM_V4_PCL_Results_Band_Chan> chainData;
};

//---------- Characterized Rx Results
typedef struct GSM_RX_MEAS
{
   int m_rssi;
   int m_lna;
} GSM_RX_MEAS_TYPE;

struct GSM_V4_Rx_RSSI_Estimates_Band_Chan
{
   // map< band, map<channel, vector<lna state, estRSSI>
   map<unsigned int, map<int, vector<GSM_RX_MEAS_TYPE> > > estRSSI_Map;
};

struct GSM_V4_RSSI_Est_Chain
{
   // map<Tx chain, GSM_V4_Rx_RSSI_Estimates_Band_Chan>
   map<unsigned int, GSM_V4_Rx_RSSI_Estimates_Band_Chan> chainData;
};

struct GSM_V4_Rx_Results_Chain
{
   // map<Tx chain, GSM_V4_Rx_Results_Band_Chan>
   map<unsigned int, GSM_V4_Rx_Results_Band_Chan> chainData;
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

//---------- NV Results
struct GSM_V4_Tx_Cx_NV_Results
{
   // map<Tx chain, map< band, NV results for that band> >
   map< unsigned int, map<unsigned int, QMSL_GSM_Tx_Calibration_NV> > chainNV;
};

struct GSM_V4_Rx_Cx_NV_Results
{
   // map<Rx chain, map< band, NV results for that band> >
   map< unsigned int, map<unsigned int, QMSL_GSM_Rx_Calibration_NV> > chainNV;
};

struct GSM_V4_Rx_Cx_Cal_Meas
{
   // map<Rx chain, map< band, measurement results for that band > >
   map< unsigned int, map<unsigned int, QMSL_GSM_Rx_Calibration_Params_and_Meas> > chainMeas;
};
struct GSM_V4_Meas_Info
{
   unsigned short numExpTxMeas;             // the total number of expected Tx measurements for the entire sweep
   unsigned short numContiguousDPD_Meas;    // the number of contiguous DPD measurements per channel
   unsigned short numContiguousDA_Meas;     // the number of contiguous DA Cal measurements per channel
   unsigned short numTotalDPD_Meas;         // number of total expected DPD measurements per sweep
   unsigned short numTotalDA_Meas;          // number of total expected DA measurements per sweep
   unsigned short numExpRxMeas;             // number of total expected Rx measurements per sweep

   unsigned short numContiguousDPD_Meas_CB;
   unsigned short numExpTxMeas_CB;
};

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

// a 2D MATRIX Structure
typedef struct
{
   int         elemSize;      // data type size: sizeof(float), sizeof(int), etc
   unsigned int numRows;      // number of rows in matrix
   unsigned int numCols;      // number of columns in matrix
   char**       dataPtr;      // 2D pointer for accessing matrix data - can use dataPtr[row][col]
}  R2MATRIX;

//call box params
struct GSMV4_SweepConfig
{
   unsigned short numSweeps;           // the # of required sweeps
   unsigned short numBandsPerSweep;    // the # of bands per sweep
   unsigned short numChainsPerSweep;   // the # of chains per sweep

   // the original number of bands and channels
   unsigned short orgNumBand;
   unsigned short orgNumChan;

   // original number of Tx/Rx chains
   unsigned short orgNumTxChain;
   unsigned short orgNumRxChain;

   // 1) Sweep can be broken up by Band
   // NOTE: all band-enumerated lists must be stored here
   // original band-enumerated param lists
   vector<unsigned int>             orgBandVec;
   vector<vector<unsigned int> >     orgChanVec;
   vector<vector<int> >              orgGainOffset_MinLim_List;
   vector<vector<int> >              orgGainOffset_MaxLim_List;
   vector< vector<unsigned short> > orgPclIdxList;
   vector<vector<unsigned int> >     orgDpd_RgiList;
   vector<vector<unsigned int> >     orgDpd_DigGainList;
   vector<vector<unsigned int> >     orgSlopeCorrectionDigGainList;

   // 2) Sweep can be broken up by Chain ID
   // store chain IDs as well
   vector<unsigned int> orgTxChainIDs;
   vector<unsigned int> orgRxChainIDs;

   bool addHoppingFrame;         // duplicated each frame for the UE. Some call boxes only make measurements once every 2 frames

   bool segmentMultiChain;       // inidcates whether sweep segmentation by chainID is required

   int iNumFramesPerStep_CB;
};

struct TECapabilities
{
   bool boolSupportsFrameSweep;
   unsigned int iNumFramesPerStep;
   bool boolSupportSimultaneousTxRx;
   unsigned int iMaxFramesPerSweep;
   unsigned int iMaxSlotsPerFrame;
};

#define NUM_CHAR_CHAN 3
#define NUM_RX_CHAR_CHAN 8
// GSM power control levels (PCL)
#define  RFNV_GSM_C0_GSM850_POWER_LEVELS_I         (25076)
#define  RFNV_GSM_C0_GSM900_POWER_LEVELS_I         (25077)
#define  RFNV_GSM_C0_GSM1800_POWER_LEVELS_I        (25078)
#define  RFNV_GSM_C0_GSM1900_POWER_LEVELS_I        (25079)
#define  RFNV_GSM_C2_GSM850_POWER_LEVELS_I         (25080)
#define  RFNV_GSM_C2_GSM900_POWER_LEVELS_I         (25081)
#define  RFNV_GSM_C2_GSM1800_POWER_LEVELS_I        (25082)
#define  RFNV_GSM_C2_GSM1900_POWER_LEVELS_I        (25083)


#define RFNV_PCL_NUM_ITEMS          16

#define GSMV4_INTRA_BAND_FRAME_DELAY   4

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


typedef map<int, map<int, double> > GSMRxCharTable; // map< channel, map<LNA state, Gain Offset> >

typedef enum
{
   RX_FUNCTION_GET_RSSI,
   RX_FUNCTION_GET_DC_OFFSETS,
   RX_FUNCTION_STOP_CONTINOUS_RX
} enum_rx_function_type;


 typedef multimap<unsigned int, unsigned int> LNARSSI_type;
 typedef struct{ LNARSSI_type data; } LNARSSI;
 typedef map<unsigned int, LNARSSI> ChanLNARSSI_type;
 typedef struct { ChanLNARSSI_type data; } ChanLNARSSI;
 typedef map<unsigned int, ChanLNARSSI> BandChanLNARSSI_type;
 typedef struct { BandChanLNARSSI_type data; } BandChanLNARSSI;
 typedef map<unsigned int, BandChanLNARSSI> ChainBandChanLNARSSI_type;

const unsigned short gsmV4_maxNumBands   = 4;
// channels
const unsigned short gsmV4_minNumChan      = 1;
const unsigned short gsmV4_maxNumChan      = 3;
const unsigned short gsmV4_maxNumTotalChan = gsmV4_maxNumBands*gsmV4_maxNumChan;
// RGIs
const unsigned char gsmV4_maxNumRGI     = 32;
// PCLs
const unsigned short gsmV4_maxNumPCL     = 16;
// max number of Rx and Tx slots
const unsigned char gsmV4_maxNumTxSlots = 4;
const unsigned char gsmV4_maxNumRxSlots = 1;
const unsigned char gsmV4_maxNumMods    = 2;
const unsigned short gsmV4_max8PSK_pclIdx_LB   = 9;   // predist level is 22.5 dBm (LB), which is pcl index 9
const unsigned short gsmV4_max8PSK_pclIdx_HB   = 11;  // predist level is 21.5 dBm (HB), which is pcl index 11
// PCL value limits
const unsigned short gsmV4_minLowBand_PCL  = 4;
const unsigned short gsmV4_maxLowBand_PCL  = 19;
const unsigned short gsmV4_minHighBand_PCL = 0;
const unsigned short gsmV4_maxHighBand_PCL = 15;
const double gsmV4_maxPclNvPowOffset       = 0.5;
const unsigned int gsmV4_minMCS = 1;
const unsigned int gsmV4_maxMCS = 9;
const unsigned char  gsmV4_GMSK_MCS_Idx  = 3;
const unsigned char gsmV4_maxGMSK_mcsIdx = 3;   // the maximum MCS index for GMSK
const unsigned short gsmV4_minNumDpdRgi  = 3;
const unsigned short gsmV4_maxNumDpdRgi  = 32;

//RGI
const unsigned int gsmV4_minRGI = 0;
const unsigned int gsmV4_maxRGI = 31;

// DPD params
const unsigned short gsmV4_maxDpdGain    = 93;
const unsigned short gsmV4_minDpdGain    = 1;
const unsigned short gsmV4_minNumDpdGain = 4;
const unsigned short gsmV4_reqNumDpdGainRGI_Only = 1;
const unsigned short gsmV4_maxNumDpdGain = 6;

const unsigned int gsmV4_g850_minChan  = 128;
const unsigned int gsmV4_g850_maxChan  = 251;
const unsigned int gsmV4_e900_minChan  = 0;
const unsigned int gsmV4_e900_maxChan  = 1023;
const unsigned int gsmV4_g1800_minChan = 512;
const unsigned int gsmV4_g1800_maxChan = 885;
const unsigned int gsmV4_g1900_minChan = 512;
const unsigned int gsmV4_g1900_maxChan = 810;

const unsigned char gsmV4_TriggerRGI     = gsmV4_maxRGI;
const unsigned char gsmV4_TriggerPA      = 0;
const unsigned char gsmV4_TriggerDigGain = 47;
const unsigned char gsmV4_TriggerSlotIdx = 0;
const unsigned short gsmV4_Desired_DPD_Mod_Type = 1;

//Rx
const unsigned char gsmV4_maxNumLNA = 4;
const unsigned int gsmV4_minLNA     = 0;
const unsigned int gsmV4_maxLNA     = 3;
const int gsmV4_maxRxPower = -20;  // dBm
unsigned short GetGsmNvID( unsigned short rfMode, unsigned short chainID);
const int constIntraBandSegDelay = 4 * GSM_V4_MAX_NUM_OF_SLOTS; //4 frames

const double minDigGainDeltaPwr_dB = 11.5;
const double maxDigGainDeltaPwr_dB = 29.5;

unsigned char Test_GSMV4_WriteCharNV();
/*Char DA power from XML.
e.g
<!-- GSM TX DA Char: GSM 900 -->
<GSMDaCharTable rf_mode="10" chain="0" instance="0">
<GSMChanModPa channel="37" mod="0" pa="0">
<GSMRgiPwr rgi="0" pwr="6.9"/>
<GSMRgiPwr rgi="1" pwr="7.36667"/>
<GSMRgiPwr rgi="2" pwr="8.53333"/>
<GSMRgiPwr rgi="3" pwr="9.2"/>
<GSMRgiPwr rgi="4" pwr="10.4"/>
<GSMRgiPwr rgi="5" pwr="11.4"/>
<GSMRgiPwr rgi="6" pwr="12.4"/>
*/
//Char DA - 3ch, 2mod, 2PA, 32 pwr values(for 32 RGI) for APAC-CA


/*Char Rx data based on XML*/
/*
<RxGainTable rf_mode="10" chain="0" instance="0">
<RxGainChar channel="1">
<RxGain lna="0" gain="142.666875" />
<RxGain lna="1" gain="130.729375" />
<RxGain lna="2" gain="130.666875" />
<RxGain lna="3" gain="106.270625" />
</RxGainChar>
...
*/

unsigned char Test_GSM_V4_Cal();
unsigned char Test_GSM_V3_Cal();
unsigned char Test_GSM_V3_Rx_Cal();
unsigned char Test_GSM_Rx_Cal();
