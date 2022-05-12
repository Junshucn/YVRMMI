/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/CMeasurementManager_QSCPI.h#7 $
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

#include "CMeasurementManager.h"

#ifndef CMEASUREMENT_MANAGER_QSCPI
#define CMEASUREMENT_MANAGER_QSCPI

namespace RFCal
{
   class CMeasurementManager_QSCPI: public CMeasurementListModeManager
   {
      protected:
         double qspi_version;
		 double m_timeout_ms;
      public:
         CMeasurementManager_QSCPI()  ;
         ~CMeasurementManager_QSCPI() {};


         virtual unsigned char Set_PathLoss(map <double, double> mapUPLoss,map <double, double> mapDLLoss,map <double, double> mapAUXLoss);
         virtual double Get_Version();
         virtual void Set_DL_Mod_Type(MOD_TYPE mod);     
         virtual void Set_DL_Level(double power);
         virtual void Set_Trigger (double relative_pwr);
         virtual void Set_OutputPort (unsigned char isAuxport);
               
         virtual void Set_DL_Channel(unsigned long channel,  double dloffset, double uloffset);
         virtual void Set_DL_State(unsigned char on_off);
         virtual void Set_UL_Expected_Level (double power);
         /* RFCal */
         virtual unsigned char  Wait_For_Meas_To_Complete ( int toMS, const string& measID );
         virtual void Config_Cal_Sweep ( const std::vector <TE_Swp_Params> & teRecords );
         virtual unsigned char Wait_For_Tx_Sweep_To_Complete (int meastime );
         virtual void Fecth_Sweep_Result ( std::vector < double > & result ); 
         virtual void Start_Cal_Sweep();

         /* GSM */
         virtual void Config_GSM_Tx_Sweep( const CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams );
         virtual void Config_PreDist_Capture(unsigned long waveformDurationQS, unsigned long & numSamples, unsigned long & samplingRate );
         
         virtual void Start_PreDist_Capture( void );
         virtual void Get_PreDist_Capture_Data(unsigned long & numSamples, float* phaseArr, float* amplArr );
         virtual void Set_Segment_Meas_Duration(double segmentDuration, double measOffset, double measDuration);
         virtual void Start_GSM_Tx_Sweep();
         virtual void Get_GSM_Tx_Sweep_Results( const int& numSteps, vector< double >& sweepData, bool doBinaryTransfer );  
         virtual void Config_GSM_V4_FBRx_Sweep(  CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, TE_GSMV4_Sweep_Params& swpParams,
            bool &skipFirstRxMeas );
         virtual void Config_GSM_V4_Tx_Sweep( const CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, map< int, map< int, double>>& freqMap_UL_Hz,
            const TE_GSMV4_Sweep_Params& swpParams );
		 virtual void Start_GSM_V4_FBRx_Sweep( const TE_GSMV4_Sweep_Params& swpParams, bool enable_rx );
		 virtual void Get_GSM_V4_FBRx_Sweep_Results( const int& numSteps, unsigned int numChanperBand, unsigned int numFramesperChan, vector<double>& outputPowVec,
			 unsigned short numCongituousTxMeasPerBand, unsigned short intraBandSegDelay );
		 virtual void Config_GSM_V4_TxRx_Sweep( const CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, const TE_GSMV4_Sweep_Params& swpParams,
			 bool &skipFirstRxMeas );
		 virtual void Config_GSM_V4_Rx_Sweep( const CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, map< int, map< int, double>>& freqMap_DL_Hz,
			 const TE_GSMV4_Sweep_Params& swpParams );
		virtual void Start_GSM_V4_Sweep( const TE_GSMV4_Sweep_Params& swpParams );
		void Start_List_Mode_Sweep (void);
		virtual void Get_GSM_V4_Tx_Sweep_Results( const int& numSteps, vector<unsigned char>& slotConfig, vector<double>& outputPowVec,
			unsigned short numCongituousTxMeasPerBand, unsigned short intraBandSegDelay );
      private:
         virtual void Set_Filter_Type(int technology, int bw);
         virtual void Config_DL_Waveform ( int technology, bool pilotOnly, bool mod_onoff, bool isForceCW );
   };
}
#endif
