/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/CMeasurementManager.h#9 $
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
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <sstream>
#include <vector>
#include <math.h>

#ifndef __QLIB_DEFINES_H
#define __QLIB_DEFINES_H
#include "QLib_Defines.h"
#endif

#ifndef __QLIB_H
#define __QLIB_H
#include "QLib.h"
#endif

// RFCal QlibDemo uses gpib-32.dll by default. Undefine if you use static library. 
#define USE_GPIB_32_DLL

#ifdef USE_GPIB_32_DLL
      static int (__stdcall *ibdev_dll)(int boardID, int pad, int sad, int tmo, int eot, int esp);
      static int (__stdcall *ibwrt_dll)(int ud, PVOID buf, long cnt);
      static int (__stdcall *ibrd_dll)(int ud, PVOID buf, long cnt);
#endif

#ifndef USE_GPIB_32_DLL
#ifndef __DECL_32_H
#define __DECL_32_H

#include "decl-32.h"
#endif 
#endif

#include "RFCal_Enum.h"
#include "RFCal_Helper.h"


using namespace std;

namespace RFCal
{
   // ********************************* //
   // Measurement Manager classes
   // CDeviceManager ---- CMeasurementManager -- CMeasurmentManager_QSCPI/../
   // ********************************* //


   enum EQUIPMENT_TYPE
   {
      QSCPI = 0,
      CMW = 1,
      ANR8870A = 2, 
      ANR8820C = 3  
   } ;

   // This struct is used for matrix mode callbox
   typedef struct 
   {
      double ul_loss_db;
      double dl_loss_main_db;
      double dl_loss_aux_db;
   } CPathloss_per_frequency;
   typedef struct
   {
      int channel;
      double ul_freq;
      double dl_freq;
   } CChanFreq;

   // ********************************* //
   // Interfaces for RFCal Measurment
   // ********************************* //

   class IMeasurementRFCalMgr
   {
   public:
      virtual void Set_DL_Level(double power) = 0;
      virtual void Set_DL_Mod_Type(MOD_TYPE mod) = 0;
      virtual void Set_DL_Channel(unsigned long channel,  double dloffset, double uloffset = 0) = 0;
      virtual void Set_DL_State(unsigned char on_off) = 0;
      virtual void Fecth_Sweep_Result ( std::vector < double > & result ) = 0; 
      virtual void Config_Cal_Sweep ( const std::vector <TE_Swp_Params> & teRecords ) = 0;
  
      virtual unsigned char Wait_For_Tx_Sweep_To_Complete( int meastime = 2000) = 0;
      virtual void Set_UL_Expected_Level(double power) = 0;

      virtual void Set_Segment_Meas_Duration(double segmentDuration, double measOffset, double measDuration) = 0;
      virtual void Set_Trigger(double relative_pwr) = 0;
      virtual void Start_Cal_Sweep() = 0;
   protected:
      virtual unsigned char Wait_For_Meas_To_Complete( int toMS, const string& measID ) = 0;
   };
   class IMeasurementGSMRFCal
   {
   public:
      virtual void Config_GSM_Tx_Sweep( const CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams ) = 0;
      virtual void Start_GSM_Tx_Sweep() = 0;
      virtual void Get_GSM_Tx_Sweep_Results( const int& numSteps, vector< double >& sweepData, bool doBinaryTransfer = false ) = 0;
      virtual void Config_PreDist_Capture(unsigned long waveformDurationQS, unsigned long & numSamples, unsigned long & samplingRate ) = 0;
      virtual void Start_PreDist_Capture( void ) = 0;
      virtual void Get_PreDist_Capture_Data(unsigned long & numSamples, float* phaseArr, float* amplArr ) = 0;
      virtual void Config_GSM_V4_FBRx_Sweep(  CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, TE_GSMV4_Sweep_Params& swpParams,
         bool &skipFirstRxMeas ) {};
	  virtual void Config_GSM_V4_Tx_Sweep( const CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, map< int, map< int, double>>& freqMap_UL_Hz,
		  const TE_GSMV4_Sweep_Params& swpParams ){};
	  virtual void Start_GSM_V4_FBRx_Sweep( const TE_GSMV4_Sweep_Params& swpParams, bool enable_rx ) {};
	  virtual void Get_GSM_V4_FBRx_Sweep_Results( const int& numSteps, unsigned int numChanperBand, unsigned int numFramesperChan, vector<double>& outputPowVec,
		  unsigned short numCongituousTxMeasPerBand, unsigned short intraBandSegDelay ) {};
	  virtual void End_GSM_V4_Sweep( const TE_GSMV4_Sweep_Params& swpParams ){};

	  virtual void Config_GSM_V4_TxRx_Sweep( const CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, const TE_GSMV4_Sweep_Params& swpParams,
		  bool &skipFirstRxMeas ) {};
	  virtual void Config_GSM_V4_Rx_Sweep( const CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, map< int, map< int, double>>& freqMap_DL_Hz,
		  const TE_GSMV4_Sweep_Params& swpParams ){};
	  virtual void Config_GSM_V4_Rx_Sweep_Ext( const CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, map< int, map< int, double>>& freqMap_DL_Hz,
		  const TE_GSMV4_Sweep_Params& swpParams ){};
	  virtual void Start_GSM_V4_Sweep( const TE_GSMV4_Sweep_Params& swpParams ) {};
	  virtual void Get_GSM_V4_Tx_Sweep_Results( const int& numSteps, vector<unsigned char>& slotConfig, vector<double>& outputPowVec,
		  unsigned short numCongituousTxMeasPerBand, unsigned short intraBandSegDelay ) {};

   };
   class IMeasurementMatrixMode
   {
   public: 
      virtual void Config_Cal_Sweep ( const std::map < int,  std::vector <TE_Swp_Params> > & frame_teRecords ) = 0;

   protected:
      virtual unsigned char Set_MatrixMode_Config ( const std::map < int, std::vector <TE_Swp_Params> > & frame_teRecords) = 0;
   };

   // ********************************* //
   // Link State (common status)
   // ********************************* //
   class CLinkState
   {
   public:
       int tech;
       int band;
       int subband;
       int ULChannel;
       int DLPort;
       unsigned char bDL_State;
       bool m_is_basic_RF;
       bool m_list_mode;
       int modType;
       double dlPower;

       CLinkState():
         tech(0),
         band(0),
         subband(0),
         ULChannel(0),
         DLPort(0), 
         modType (-1),
         dlPower(0.0),
         m_is_basic_RF ( true ),
         m_list_mode ( false ),
         bDL_State ( false )
         {}
   };
   typedef struct CDeviceState
   {
      int Gpib_board_id;
      int PrimaryAddress;
      int SecondaryAddress;
      bool bSimulationMode;
      HANDLE hInst;
      CDeviceState():
         Gpib_board_id (0),
         PrimaryAddress (0),
         SecondaryAddress (0),
         bSimulationMode (false),
         hInst (NULL) {};
   } CDeviceState;

   
   // ********************************* //
   // Base Class of Device Manager
   // Device Type: (1) Callbox (2) Powersupply ....
   // ********************************* //
   class CDeviceManager
   {
   public:
      /* Basic GPIB read and write wrapper function */
       unsigned char Write(const std::string & cmd, bool enableQuery = true);
      unsigned char Read(const std::string & cmd, std::string & response, bool enableQueryProcess = true);
      unsigned char Read(const std::string & cmd);     
      unsigned char RepeatUntilResponse(const string & stQuery, const string & expectedResponse, unsigned long iTimeoutCount = 10, unsigned long iSleepTime = 20);      
      void Get_Response(std::string & response);

      /* Basic GPIB command */
      std::string Get_DeviceName( unsigned char forceExecuteIDN = true ); // Execute IDN

      /* Basic configuration */
      HANDLE Connect(HANDLE hPhone, int gpib_board_id, int primaryAddress, int secondaryAddress);
      void Set_SimulationMode(bool enable) {this->DeviceConfig.bSimulationMode = enable;}
      bool IsSimulationMode() { return this->DeviceConfig.bSimulationMode;}
      
      /* QMSL Logging */
      void Set_DUTHandle(HANDLE tmpHand);
      void Enable_ConsoleOutput ( unsigned char bEnableLog) { this->bShowLogConsole = bEnableLog; };
      
      /* Helper function */
      unsigned char ParseResponse(const string & response, vector <double> & data);
      unsigned char ParseResponse(const string & response, vector <string> & data);
      double ConvertStrToDouble(const string & orgSt);
#ifdef USE_GPIB_32_DLL
      HMODULE hGPIB_32_DLL;
      typedef HANDLE (*_ibdev)(int boardID, int pad, int sad, int tmo, int eot, int eos);
      typedef int (*_ibwrt)(int ud, PVOID buf, long cnt);
      typedef int (*_ibrd)(int ud, PVOID buf, long cnt);
#endif
   
      CDeviceManager():
            hDUT(NULL),
            bShowLogConsole(true),
#ifdef USE_GPIB_32_DLL
            hGPIB_32_DLL(NULL),
#endif
            deviceName("")
            {
            };
      virtual ~CDeviceManager();
   private:
      HANDLE hDUT;
      HANDLE Get_DUTHandle();

      static const long BUFFSIZE = 16000;
      static const unsigned int QMSL_LOG_LEVEL = 0x10000;
     
      char command [ BUFFSIZE ];
      char buffer [ BUFFSIZE ];
      std::string bufResponse;
      std::string deviceName;
      unsigned char bShowLogConsole;

   protected:
      CDeviceState DeviceConfig;

      /* Basic read/write function for GPIB. */
      void initialize();
      unsigned char WriteToEquip(char *sStr);
      unsigned char ReadFromEquipment(string & response);
      unsigned char ReadFromEquipment(char * sStr, long iSizeOfStr);
      void Set_Response(string tmp);

      /* QMSL Log support for logging purpose */
      void Add_QMSLLog(const std::string & Log);
      void Add_QMSLLogSection(const std::string & Log);
   };

   // ********************************* //
   // Base Class of Measurement Manager
   // Device: callbox
   // ********************************* //

   class CMeasurementManager : public RFCal::CDeviceManager, 
      public IMeasurementRFCalMgr /* Interface */,
      public IMeasurementGSMRFCal /* Interface */
   {
 
   protected:
      CLinkState LinkState;

      void Set_DLPort(int _dlPort) { this->LinkState.DLPort =  _dlPort; }
      int Get_DLPort() { return this->LinkState.DLPort; }

   public:
      CMeasurementManager(int gpib_board_id, int primaryAddress, int secondaryAddress, bool simulationMode = false) ;
      CMeasurementManager() {};

      /* Set Path Loss Must implement */
      virtual unsigned char Set_PathLoss(map <double, double> mapUPLoss,map <double, double> mapDLLoss,map <double, double> mapAUXLoss) = 0;
      virtual unsigned char Delete_PathLossTable() {return true;};
      
      // Temp
      virtual unsigned char Register_PathLoss(map <double, double> mapUPLoss,map <double, double> mapDLLoss,map <double, double> mapAUXLoss)
      {
         this->mapAuxLoss = mapAuxLoss;
         this->mapDLLoss = mapDLLoss;
         this->mapUPLoss = mapUPLoss;
         return true;
      };
      
      /* Set Rx Output Port Must implement */
      virtual void Set_OutputPort (unsigned char isAuxport) = 0;
      
      void Set_Tech(int tech) {
         this->LinkState.tech = tech;
      };
      void Set_Band(int band) {this->LinkState.band = band;};
      
      unsigned char Set_RFMode(FTM_RF_Mode_Enum rfMode);
      
      void Set_ULChannel(int _ULChannel, int band=-999) {this->LinkState.ULChannel = _ULChannel;};
      

      int Get_Tech() {return this->LinkState.tech; };
      int Get_Band() { return this->LinkState.band; };
      int Get_ULChannel(){ return this->LinkState.ULChannel; };
     
      virtual void Init() {};
      virtual double Get_Version() { return 0.0;} 
      
      virtual double GetCalPointLoss (const std::map < double, double > & pathloss_map, const double frequency);
      virtual void ComputeFreqFromARFCN( int tech, map<int, map<int, double>>& freqMap_UL_MHz,
                                                              map< int, map< int, double>>& freqMap_DL_MHz, const vector<vector<int > > & chanList, const vector<int>& bandList,
                                                              bool addFreqOffset, int outputFreqUnit /* 0- Hz, 1: KHz, 2:MHz */  );
   protected:
      map <double, double> mapUPLoss; // ToDo: move to caldb object
      map <double, double> mapDLLoss; // ToDo: move to caldb object
      map <double, double> mapAuxLoss; // ToDo: move to caldb object

      template <typename T> string Get_ListModeConfigString (const string stHeader, std::vector < T > data )
      {
         std::stringstream ss;
         ss << stHeader << " ";

         for ( std::vector < T >::iterator it = data.begin(); it != data.end(); it++)
         {
            if (it != data.begin())
            {
               ss << ",";
            }
            ss << (*it);
         }
         return ss.str();
      };
      template <typename T> string Get_ConfigString (const string stHeader,  T data )
      {
         std::stringstream ss;
         ss << stHeader << " " << data;
         return ss.str();
      }
   };

   class CMeasurementMatrixModeManager : public CMeasurementManager, public IMeasurementMatrixMode
   {
      protected: 
         std::vector < TE_Swp_Params >  m_baseFrame;
         std::vector < CChanFreq > m_ChanFreqList;
      public:
         virtual unsigned char Set_MatrixMode_Config ( const std::map < int, std::vector <TE_Swp_Params> > & frame_teRecords);
   };
   class CMeasurementListModeManager : public CMeasurementManager
   {
   };
};
