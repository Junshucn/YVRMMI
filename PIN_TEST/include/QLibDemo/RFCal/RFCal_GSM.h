/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/RFCal_GSM.h#4 $
 * $DateTime: 2017/09/26 18:18:33 $
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
//#include "..\stdafx.h"
#include "RFCal.h"


namespace RFCal
{
   #define  NV_EDGE_LINEAR_TX_GAIN_PARAM_I                5478
   #define  NV_EDGE_850_LINEAR_TX_GAIN_PARAM_I            6475
   #define  NV_EDGE_1800_LINEAR_TX_GAIN_PARAM_I           6476
   #define  NV_EDGE_1900_LINEAR_TX_GAIN_PARAM_I           6477
   #define	RFNV_GSM_GSM850_TX_CAL_DATA_I				  29230
   #define	RFNV_GSM_GSM900_TX_CAL_DATA_I				  29231
   #define	RFNV_GSM_GSM1800_TX_CAL_DATA_I                29232
   #define  RFNV_GSM_GSM1900_TX_CAL_DATA_I                29233
   #define  RFNV_PCL_NUM_ITEMS          16
   // Consolidated GSM Extended PA switchpoint NVs
   #define  RFNV_GSM_GSM850_EXTENDED_PA_SWPT_I            29150
   #define  RFNV_GSM_GSM900_EXTENDED_PA_SWPT_I            29151
   #define  RFNV_GSM_GSM1800_EXTENDED_PA_SWPT_I           29152
   #define  RFNV_GSM_GSM1900_EXTENDED_PA_SWPT_I           29153
   // Consolidated GSM power control levels (PCL)
   #define RFNV_GSM_GSM850_POWER_LEVELS_I                 29186
   #define RFNV_GSM_GSM900_POWER_LEVELS_I                 29187
   #define RFNV_GSM_GSM1800_POWER_LEVELS_I                29188
   #define RFNV_GSM_GSM1900_POWER_LEVELS_I                29189
   // Consolidated GSM Tx Cal Data NVs
   #define RFNV_GSM_C0_GSM850_TX_CAL_DATA_I				  24972
   #define RFNV_GSM_C0_GSM900_TX_CAL_DATA_I				  24973
   #define RFNV_GSM_C0_GSM1800_TX_CAL_DATA_I			  24974
   #define RFNV_GSM_C0_GSM1900_TX_CAL_DATA_I			  24975
   #define RFNV_GSM_C2_GSM850_TX_CAL_DATA_I				  24976
   #define RFNV_GSM_C2_GSM900_TX_CAL_DATA_I				  24977
   #define RFNV_GSM_C2_GSM1800_TX_CAL_DATA_I			  24978
   #define RFNV_GSM_C2_GSM1900_TX_CAL_DATA_I			  24979
   #define RFNV_GSM_C4_GSM850_TX_CAL_DATA_I				  28581
   #define RFNV_GSM_C4_GSM900_TX_CAL_DATA_I				  28582
   #define RFNV_GSM_C4_GSM1800_TX_CAL_DATA_I			  28583
   #define RFNV_GSM_C4_GSM1900_TX_CAL_DATA_I			  28584
   /* GSM V2 */
   namespace GSM_Cal
   {
     unsigned char DO_GSM_V2_TX_Cal(HANDLE hPhone);
     unsigned char DO_GSM_V2_RX_Cal(HANDLE hPhone);
     unsigned char DO_GSM_FBRX_CAL(HANDLE hPhone);
     unsigned char DO_GSM_V5_CAL (HANDLE hPhone);
      
     unsigned char INIT_TX(HANDLE hPhone, GSM_CAL_Params & params);
     unsigned char INIT_RX(HANDLE hPhone, GSM_CAL_Params & params);
     unsigned char INIT_FBRX(HANDLE hPhone, GSM_CAL_Params & params);
	  unsigned char INIT_V5(HANDLE hPhone, GSM_CAL_Params & params);
      
     unsigned char AssignTxParams(HANDLE hPhone, GSM_CAL_Params & params);
     unsigned char AssignRxParams(HANDLE hPhone, GSM_CAL_Params & params);
     unsigned char AssignFBRxParams(HANDLE hPhone, GSM_CAL_Params & params);

	 unsigned char AssignGSMV5Params(HANDLE hPhone, GSM_CAL_Params & params);
	 unsigned char AssignGSMV5Params_DA(HANDLE hPhone, GSM_CAL_Params & params);
	 unsigned char AssignGSMV5Params_DPD(HANDLE hPhone, GSM_CAL_Params & params);
	 unsigned char AssignGSMV5Params_RX(HANDLE hPhone, GSM_CAL_Params & params);
	 unsigned char AssignGSMV5Params_Predistortion (HANDLE hPhone, GSM_CAL_Params & params);
     unsigned char AssignCommonParams( HANDLE hPhone, GSM_CAL_Params & params);

     unsigned char FTM_SET_MODE_ID_GSM(HANDLE hPhone, unsigned short chainID, FTM_Mode_Id_Enum gsmModeID = FTM_MODE_ID_GSM);
	  unsigned char GSM_Set_RFM_Path(HANDLE hPhone, vector<FTM_RF_Mode_Enum>& rfModeVec, vector<CGSM_Cal_Device_SigPath>& bandRFMPaths);

     unsigned char QueryGSM_Devices_to_Calibrate(HANDLE hPhone, RFMPathDB & rfmPathDB, FTM_RF_Mode_Enum rfMode, 
                  CGSM_Cal_Device_SigPath& bandDevSigPaths, 
                  unsigned short antennaPath = 65535, unsigned short nvContainer = 0, unsigned short rx_chainID_selection = 0,
                  GSM_RFC_QUERY_TYPE rfcQueryType = GSM_TX_BASED_QUERY );
      namespace GSM_V4
      {
         unsigned char Do_GSM_V4_Sweep( HANDLE hPhone, vector < unsigned char > & gsm_V4_cmd, unsigned short gsmV4CmdLen, const vector<unsigned int>& chainID_Vec, bool compressed );
         unsigned char Get_GSM_V4_Sweep_Results( HANDLE hPhone, GSM_V4_FTM_CMD_Response_Type& gsmV4_measResults );
         unsigned char Do_GSM_V4_Cleanup( HANDLE hPhone );
      }



	  namespace GSM_V5
	  {

		  unsigned char Do_V5_Cal( HANDLE hPhone, GSM_CAL_Params & params, GSM_V4_PCL_Results_Chain & gsmV4_PCL_Results, 
			  GSM_V4_DPD_Results_Chain & gsmV4_DPD_Results, GSM_V4_RSSI_Est_Chain & gsmV4_RSSI_Results, GSM_V4_RSSI_Est_Chain & gsmV4_RSSI_Results_HiLin);
		  unsigned char FillCallBoxParams (HANDLE hPhone, GSM_CAL_Params & params);
		  unsigned char Compute_NumFrames( HANDLE hPhone, GSM_CAL_Params & params);
		  unsigned char Create_GSM_V4_Cal_Command( HANDLE hPhone, GSM_CAL_Params & params, unsigned char gsmV4CmdBuff[], unsigned short& gsmV4CmdSize,
			  map< unsigned int, vector<unsigned short> >& RX_BURST_FRAME_IDENTIFIER );
		  unsigned char GSM_Get_NV_Container( HANDLE hPhone, GSM_CAL_Params &params, FTM_RF_Mode_Enum rfMode, unsigned char primPath, unsigned char forTxOrRx,
			  unsigned char & nvContainer );
		  unsigned char Do_GSM_V5_Sweep(HANDLE hPhone, GSM_CAL_Params &params,unsigned char gsm_V4_cmd[], unsigned short gsmV4CmdLen, const vector<unsigned int>& chainID_Vec, bool compressed =false );
		  unsigned char Compute_MeasInfo( HANDLE hPhone, GSM_CAL_Params & params);
		  bool DetermineIfLowBand( unsigned int uiCurBand );
		  

		  namespace Tx
		  {
			  unsigned char Compute_PCL_Deltas( HANDLE hPhone, GSM_CAL_Params & params, unsigned short & m_pclDeltaPowIdx, const vector<double>& inPowVec, const vector<short>& pclVecNV, GSM_V4_PCL_Sweep_Record& io_powDeltaMap,
				  const vector<unsigned short>& pclList, bool isEdge, unsigned char maxEdgePCL); 
			  unsigned char Create_PCL_Map(HANDLE hPhone, GSM_CAL_Params & params);
			  unsigned char Create_PCL_Indexes(HANDLE hPhone, GSM_CAL_Params & params);
			  unsigned char Clear_PCL_Maps( HANDLE hPhone, GSM_CAL_Params & params);
			  unsigned char Create_MCS_Indexes( HANDLE hPhone, GSM_CAL_Params & params);
			  unsigned char BuildDpdSweepContainers( HANDLE hPhone, GSM_CAL_Params & params, bool reverseSweepOrder );

			  unsigned char Get_PCL_FromConsolidatedNV( HANDLE hPhone, int m_curBand, vector<short>& pclValues);
			  unsigned char Get_Predist_Swpt_From_ConsolidatedNV( HANDLE hPhone, int m_curBand, const vector<short>& pclValues );
			  unsigned char Segment_DPD_and_DA_Pwr( HANDLE hPhone, GSM_CAL_Params &params, vector<double>& inputPowVec,
				  map< unsigned int, vector<double> >& dpdPowMap_Chain, map< unsigned int, vector<double> >& daPowMap_Chain );
			  unsigned char Process_GSM_Tx_Cx_NV_From_V4_Cal_PCL( HANDLE hPhone, GSM_CAL_Params &params, GSM_V4_PCL_Results_Chain& gsmV4_PCL_Results, GSM_V4_DPD_Results_Chain& gsmV4_DPD_Results,
				  GSM_V4_Tx_Cx_NV_Results& tx_NV_Results, GSM_V4_Tx_Consolidated_NV_Results& consolidated_Tx_NV_Results );
			  unsigned char Read_GSM_TxCx_NV_Payload( HANDLE hPhone, GSM_CAL_Params &params, QMSL_GSM_Tx_Calibration_NV& readGsmNV, unsigned long& payloadSize, int m_curBand, int m_curChainID );
			  unsigned char Read_GSM_TxConsolidated_NV_Payload_And_Extract_Header( HANDLE hPhone, GSM_CAL_Params &params, int m_curBand, QMSL_GSM_Tx_Calibration_Consolidated_NV& readGsmNV, unsigned long& payloadSize);


			  unsigned char GSM_TX_Cx_NV_Extract_EDGE_DA_Cal_Data( HANDLE hPhone, GSM_CAL_Params &params,QMSL_GSM_Tx_Calibration_NV& calNV, unsigned long& updatedPayloadSize,
				  GSM_DA_Cal_Results_Channel& daCalData, unsigned short& numRGI );
			  unsigned char Apply_PCL_CorrectionTo_EdgeCharDA(  HANDLE hPhone, GSM_CAL_Params &params, int m_curBandIdx, GSM_DA_Cal_Results_Channel& daCalData, GSM_V4_PCL_Results_Band_Chan& gsmV4_PCL_Results, unsigned short numRGI );

			  unsigned char Predistortion_RGI_Master( HANDLE hPhone, GSM_CAL_Params &params,int m_curBandIdx, int m_curChainID,  GSM_DA_Cal_Results_Channel &daCalData, GSM_V4_DPD_Results_Band_Chan& gsmV4_DPD_Results );
			  double Determine_PCL_Pow_Error_For_DaChar( HANDLE hPhone, double curDaPow, unsigned short numPCL, map<unsigned short, double>& pclPowMap,
				  map<unsigned short, double>& pclPowError);
			  unsigned char Pack_GSM_DPD_For_QMSL( HANDLE hPhone, GSM_CAL_Params &params, int m_curBandIdx, GSM_V4_DPD_Results_Band_Chan& gsmV4_DPD_Results, QMSL_GSM_EnhLinTx_Cal_Control& calControl_ToQMSL,
				  QMSL_GSM_EnhLinTx_Cal_Data& calDPD_ToQMSL, unsigned short calRgi, int m_curChainID );
			  unsigned char Determine_PreDist_Cal_RGI( HANDLE hPhone, GSM_CAL_Params &params, GSM_DA_Cal_Results_Channel& daCalData, double preDistCalPower, int m_curBandIdx, int m_curChainID,  unsigned short  & m_predistCalRgi );
			  unsigned char Determine_PreDist_Cal_RGI_from_DPD_AvgPow( HANDLE hPhone, GSM_CAL_Params &params, int m_curBandIdx, int m_curChainID,const vector< int>& chanList, map<unsigned int, GSM_V4_DPD_Sweep_Record>& chanDigGainPwr,
				  const vector<unsigned int>& rgiList, double preDistCalPower );
			  unsigned char Determine_PreDist_Cal_RGI_from_DPD( HANDLE hPhone, GSM_CAL_Params &params, int m_curBandIdx, map<unsigned int, GSM_V4_DPD_Sweep_Record>& chanDigGainPwr, double preDistCalPower,
				  double preDistPowerBuffer );
			  unsigned char Fill_PCL_Results (HANDLE hPhone, GSM_CAL_Params &params, GSM_V4_PCL_Results_Band_Chan& gsmV4_PCL_Results, vector<double>& measPowerVec );
			  unsigned char Fill_DPD_Results( HANDLE hPhone, GSM_CAL_Params &params, GSM_V4_DPD_Results_Band_Chan& gsmV4_DPD_Results, vector<double>& dpdPowerVec );

			  unsigned char DetermineTxNvContainer( HANDLE hPhone, GSM_CAL_Params &params, bool getNvContainerFromUUT );

			  unsigned char Inspect_GSM_Tx_Consolidated_NV_PredistData_Validity( HANDLE hPhone, GSM_CAL_Params &params, FTM_RF_Mode_Enum curBand, unsigned short txAntPath, 
				  unsigned int txSigPath, unsigned short txRfmDev, int isDataValid );
			  unsigned short Select_CharDPD_DataSet_V4( HANDLE hPhone, GSM_CAL_Params &params,unsigned short calRgi, unsigned short band, unsigned short chan, unsigned short chainID, bool useAdaptiveSelection,
				  vector<struct opPwrRecord>& ops, vector<struct digRecord>& daqs,  map<unsigned short, GSM_DA_Cal_DigGain_Record>& dutDigGainSweep );
			  float DetermineDacAtDigGainPwr( const vector<struct opPwrRecord>& ops, float digGainPwrIn, unsigned int numDAC );
			  float EstimatePowerAtMaxDigGain( HANDLE hPhone, float maxCharAMAM, float maxCharPwr, vector<struct digRecord>& charDaqs,
				  map<unsigned short, GSM_DA_Cal_DigGain_Record>& dutDigGainSweep, unsigned short calRgi,
				  float dacAtMaxDigGain, unsigned short edgeDigGain, float dacScale );
			  unsigned char Do_EDGE_PreDist_Cal( HANDLE hPhone, GSM_CAL_Params &params, unsigned long calRgi, unsigned int m_curBandIdx, PreDist_Capture_Record_Channel& calibratedData  );
			  unsigned char Process_EDGE_PreDist_Cal_NV( HANDLE hPhone, GSM_CAL_Params &params, const PreDist_Capture_Record_Channel& calibratedPreDistData, QMSL_GSM_PreDist_Cal_NV_Chan_struct& calNV, int m_curBandIdx );
			  unsigned char Process_EDGE_PreDist_Cal_NV_Intermediate( HANDLE hPhone, GSM_CAL_Params &params, const PreDist_Capture_Record_Channel& calibratedPreDistData,
				  QMSL_GSM_PreDist_Cal_Result& calResult, unsigned char& queue_nv_indicator );
			  short GetEDGELinearTxGain( HANDLE hPhone,GSM_CAL_Params & params, unsigned int nvMode );
			  unsigned char PackPredistCalDataForNVProcessing( HANDLE hPhone, GSM_CAL_Params &params, QMSL_GSM_PreDist_Cal_NV_Chan_struct& predistCalNV, QMSL_GSM_EnhLinTx_Cal_Data& calDPD_ToQMSL,
				  QMSL_GSM_EnhLinTx_Cal_Control& calControl_ToQMSL, GSM_V4_DPD_Results_Band_Chan& gsm_v4_dpdResults , int m_curBandIdx);

			  unsigned char RunPredistSweep_And_Process_Tx_NV( HANDLE hPhone, GSM_CAL_Params &params, GSM_V4_PCL_Results_Chain& gsmV4_PCL_Results, GSM_V4_DPD_Results_Chain& gsmV4_DPD_Results,
				  GSM_V4_Tx_Cx_NV_Results& tx_NV_Results, GSM_V4_Tx_Consolidated_NV_Results& consolidated_Tx_NV_Results, GSM_CAL_Results & results);

		  }
		  namespace Rx
		  {
			  unsigned char Process_Rx_Meas_Results( HANDLE hPhone, GSM_CAL_Params &params, const GSM_V4_FTM_CMD_Response_Type& gsmV4_Rx_MeasResults, GSM_V4_Rx_Results_Chain& rssiMaps,
				  GSM_V4_Rx_Results_Chain& rssiMapsHiLin, map<unsigned int, vector<unsigned short>>& RX_BURST_FRAME_IDENTIFIER,
				  GSM_V4_BandChainChan_Params& bcc, GSM_V4_Rx_Params& rxParams );

			  unsigned char Filter_RSSIs( HANDLE hPhone, GSM_CAL_Params &params,  ChainBandChanLNARSSI_type & chanToLNA_AllRSSI,
				  unsigned int chainID, GSM_V4_Rx_Results_Chain& rssiMaps, bool isHiLinearity, GSM_V4_Rx_Params& rxParams , int m_curBand, int m_curChan );
			  unsigned char Estimate_RSSI_From_CharData( HANDLE hPhone, GSM_CAL_Params &params, GSM_V4_Rx_Results_Band_Chan& rssiMaps, GSM_V4_Rx_RSSI_Estimates_Band_Chan& bandToChan_EstRSSI_Map,
				  unsigned int chainID, bool isHiLinearity, int m_curBandIdx );
			  unsigned char Process_GSM_Rx_V4_Cal_NV_Consolidated ( HANDLE hPhone, GSM_CAL_Params & params, int m_curBand, unsigned char cacheNVItems, GSM_V4_Rx_Params& rxParams,
				  const GSM_RX_MEAS_AntPath& chan_rssis_list_consolidated, //array with Antenna Path, chain ID, RFM Dev, caltype, channel list, RSSI and size of array
				  std::vector<QMSL_GSM_Rx_Calibration_Params_and_Meas_Consolidated>& gsm_rx_chain_data, QMSL_RFNV_RX_CAL_GSM_NV& gsm_rx_cal_nv );
			  unsigned char Process_GSM_Rx_Cx_NV_From_V4_Cal( HANDLE hPhone, GSM_CAL_Params &params, GSM_V4_Rx_Cx_NV_Results& rx_NV_Results, GSM_V4_RSSI_Est_Chain& gsmV4_RSSI_Results,
				  GSM_V4_Rx_Cx_Cal_Meas& rx_Cal_Meas, bool isHiLinearity, GSM_V4_Rx_Params& rxParams, GSM_V4_BandChainChan_Params& bcc,
				  unsigned int nvAction );
			  unsigned char Process_Ind_Rx_Cx_NV( HANDLE hPhone, GSM_CAL_Params &params, int  m_curBand, int m_curChainID, unsigned char cacheNVItems, GSM_V4_Rx_Cx_NV_Results& rx_NV_Results, GSM_V4_RSSI_Est_Chain& gsmV4_RSSI_Results,
				  GSM_V4_Rx_Cx_Cal_Meas& rx_Cal_Meas, unsigned int chainID, bool isHiLinearity, GSM_V4_Rx_Params& rxParams );

		  }

		  namespace MatrixOperation
		  {
			  //typedef struct


			  R2MATRIX *R2MatrixAllocate(HANDLE hPhone, unsigned int numRows, unsigned int numCols, int elementSize);
			  R2MATRIX *R2MatrixTranspose( HANDLE hPhone, R2MATRIX *A);
			  R2MATRIX *R2MatrixMult( HANDLE hPhone, R2MATRIX *A, R2MATRIX *B);
			  void R2MatrixFree(HANDLE hPhone, R2MATRIX *A);
			  R2MATRIX *R2MatrixInvert(HANDLE hPhone, R2MATRIX *A);

		  }
		  namespace InputFile
		  {
			  unsigned char CharXMLPath(HANDLE hPhone, GSM_CAL_Params & params);
			  unsigned char ReadCharXML(HANDLE hPhone, GSM_CAL_Params & params);
			  unsigned char writeDACharToConsolidatedNV (HANDLE hPhone, GSM_CAL_Params & params);
			  unsigned int GetGsmConsolidatedNvID ( int rfMode);

		  }

	  }


      namespace DA_Cal
      {
         unsigned char DO_GSM_DA_Cal(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results);

         unsigned char DO_GSM_Tx_DA_Sweep(HANDLE hPhone, const GSM_CAL_Params & params);
         unsigned char Check_Tx_DA_Sweep_Result(HANDLE hPhone, const GSM_DA_CAL_Params & daCalParams, const vector<double> & daSweepPwrs, 
            unsigned long & gsmSweepIndexOfFirstRGIinFirstPaRange,
            unsigned long & gsmSweepIndexOfLastRGIinFirstPaRange );
         unsigned char Store_Tx_DA_Results(HANDLE hPhone, const unsigned long channel, const GSM_DA_CAL_Params & daCalParams, const vector<double> & daSweepPwrs,  GSM_CAL_Results & results);
      }

      namespace PreDist_Cal
      {
         unsigned char DO_GSM_PreDist_Cal(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results);
         unsigned long Determine_PreDist_Cal_RGI(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results);
         unsigned char DO_EDGE_PreDist_Cal(HANDLE hPhone, const GSM_CAL_Params & params, unsigned long calRgi, PreDist_Capture_Record_Channel * calibratedData);
      }
      namespace FBRx_Cal
      {
         unsigned char Do_FBRx_Cal(HANDLE hPhone, GSM_CAL_Params & params, GSM_CAL_Results & results);
         unsigned char Create_GSM_FBRx_Cal_Command( const GSM_CAL_Params & params, vector < unsigned char > & gsmV4CmdBuff, unsigned short& gsmV4CmdSize,
                                                                map< unsigned int, vector<unsigned short> >& RX_BURST_FRAME_IDENTIFIER );
         unsigned char FillCallBoxParams(HANDLE hPhone, GSM_CAL_Params & params);
         unsigned char Compute_NumFrames(HANDLE hPhone, GSM_CAL_Params & params);
         unsigned char Process_FBRx_Meas_Results(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results);
         unsigned char Store_FBRx_Results(HANDLE hPhone, GSM_CAL_Params & params, GSM_CAL_Results & results);
         unsigned char Do_FBRx_Data_Push(HANDLE hPhone, GSM_CAL_Params & params, GSM_CAL_Results & results);
         unsigned char Log_FBRx_Result(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results);
      }
      namespace Process_TxNV
      {
         unsigned char Consolidated_NV_From_DA_and_IQ_Predistortion_Cal(HANDLE hPhone,  const GSM_CAL_Params & params,
                                                         GSM_CAL_Results & results, QMSL_GSM_Tx_Calibration_Consolidated_NV & calNV);
         unsigned char DA_Cal_NV_Intermediate(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results, 
                                                         QMSL_GSM_DA_Cal_Result * calResultsToQmsl, unsigned char& queue_nv_indicator );
         unsigned char EDGE_PreDist_Cal_NV_Intermediate(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results,   
                                                         QMSL_GSM_PreDist_Cal_Result & calResultPreDistToQMSL, unsigned char queue_nv_indicator_predist);
         unsigned char GetEDGELinearTxGain(HANDLE hPhone, unsigned int nvMode, unsigned char chainID, unsigned char useFTMCmd, short & envGain);

      }
      namespace Process_RxNV
      {
         unsigned char Rx_Cal_NV_Consolidated(HANDLE hPhone,  const GSM_CAL_Params & params, GSM_CAL_Results & results, 
            std::vector<QMSL_GSM_Rx_Calibration_Params_and_Meas_Consolidated> & gsm_rx_chain_data,
            QMSL_RFNV_RX_CAL_GSM_NV & calNV);
      }
      namespace RX_Cal
      {
         unsigned char DO_GSM_Rx_FreqComp( HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results);
         unsigned char DO_GSM_Rx_Gain_Range_Cal( HANDLE hPhone, const vector<int>& range_list, const int& burst_avgs, const bool& toggle_rx, vector<int> * rssis_prx, vector<int> * rssis_drx, bool isHiLinearity = false );
         unsigned char GroupGainRanges( HANDLE hPhone, const GSM_CAL_Params& params, map< double, vector<int> >& batches,  map< double, vector<int> >& batchesHiLin, bool performMeasGainRangeCheck);
      }

      namespace LOG
      {
         unsigned char Rx_FreqComp( HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results);
      }


	  const unsigned short gsmV4_maxNumBands   = 4;
	  // channels
	  const unsigned short gsmV4_minNumChan      = 1;
	  const unsigned short gsmV4_maxNumChan      = 16;
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
	  const double gsmV4_maxPclNvPowOffset       = 2.0;
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
	  const unsigned short gsmV4_minNumDpdGain = 1;
	  const unsigned short gsmV4_reqNumDpdGainRGI_Only = 1;
	  const unsigned short gsmV4_maxNumDpdGain = 20;

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
	  // this is the channel index for which a measured predistortion sweep is performed
	  const unsigned short gsmV4_REF_CHAN_IDX = 0;

	  //Rx
	  const unsigned char gsmV4_maxNumLNA = 4;
	  const unsigned int gsmV4_minLNA     = 0;
	  const unsigned int gsmV4_maxLNA     = 3;
	  const int gsmV4_maxRxPower = -20;  // dBm
	  unsigned short GetGsmNvID( unsigned short rfMode, unsigned short chainID);
	  const int constIntraBandSegDelay = 4 * GSM_V4_MAX_NUM_OF_SLOTS; //4 frames

	  const double minDigGainDeltaPwr_dB = 11.5;
	  const double maxDigGainDeltaPwr_dB = 29.5;
   };
   
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

};