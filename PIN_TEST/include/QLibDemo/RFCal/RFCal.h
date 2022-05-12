/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/RFCal.h#20 $
 * $DateTime: 2017/07/10 23:57:19 $
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
#define QLIB_DEMO_EXT

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>   // For _O_BINARY
#include <math.h>   // for log10()
#include <time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <set>

#include <iomanip>
#include <vector>
#include <map>
#include <algorithm> // Find

#include <windows.h>
#include <winnt.h>
#include <intrin.h>
#include <time.h>

#include "RFCal_Enum.h"

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

extern HANDLE deviceMutex;

namespace RFCal
{
   class DeviceDB
   {
   public:
         static DeviceDB& GetInstance()
         {
            DWORD mutexRes = WaitForSingleObject( deviceMutex, WAIT_TIME);
            if ( mutexRes == WAIT_OBJECT_0)
            {            
               static DeviceDB instance;
               ReleaseMutex( deviceMutex );
         return instance;
      }
            else // if ( mutexRes == WAIT_TIMEOUT)
            {
               // Failure, we can stop cal here, 
               static DeviceDB instance;
               ReleaseMutex( deviceMutex );
               return instance;
            }
         }

         virtual unsigned char RegisterDevice (HANDLE hPhone, int equipment_type, int gpib_board, int prim_addr, int sec_addr = 0, bool IsSimulationMode = false );
         virtual unsigned char UnregisterDevice ();
         CMeasurementManager * GetMeasurementMgr() { return pMgr ; };
private:
         CMeasurementManager * pMgr;
         DeviceDB() : pMgr (NULL) {};
         ~DeviceDB();
      // For Singleton 
         DeviceDB(DeviceDB const&) {};
         static const DWORD WAIT_TIME = 10000;
         void operator=(DeviceDB const&);
   };
   // ********************************************************************* //
   // Functions....
   // ********************************************************************* //

   /* XO Cal */
   namespace XOCal
   {
      unsigned char DO_XO_Cal(HANDLE hPhone, int tech, int band, int subband, int channel, 
         ftm_xo_cal_parameter_type & ftm_xo_cal_params,
         unsigned char query_path_from_rfc, RFMPathDB * rfmPathDBPtr = NULL);
      unsigned char AssignParameters(HANDLE hPhone, int tech, int band, int channel, ftm_xo_cal_parameter_type & ftm_xo_cal_params);
      unsigned char UUT_Do_XO_Calibration(HANDLE hPhone, const ftm_xo_cal_command_type xo_cal_command,
      const unsigned char xo_cal_version,
      const ftm_xo_cal_parameter_type& ftm_xo_cal_params,
      FTM_XO_Cal_Response_v2& ftm_xo_cal_response);
      unsigned char LogResults(HANDLE hPhone,int tech, int band, int subband, unsigned long channel,FTM_XO_Cal_Response_v2& ftm_xo_cal_response);
   };


   unsigned char SetPathLoss(HANDLE hPhone, CMeasurementManager & cMgr, const string & calDB_filePath, const string & calConfig);
   unsigned char SetPathLoss(HANDLE hPhone, CMeasurementManager & cMgr, const string & calDB_filePath, const int tech);

   unsigned char Do_FTM_DO_SELF_CAL(HANDLE hPhone);
   unsigned char Get_Therm_Val(HANDLE hPhone, const int tech, const int band, const int subBand,const CRFM_Path & rfm_path, int& therm_val, int& therm_val_degC);
   namespace SetOpCode
   {
      unsigned char InitRecord(HANDLE, cal_sweep_record & swpRecord);
      unsigned char InitSwpRecords(HANDLE hPhone, std::vector < cal_sweep_record > & SwpRecords, unsigned int numRecords);
      
      inline unsigned char GetRadioConfigPathType (HANDLE hPhone, uint8 rfm_path);
      unsigned char NoP ( HANDLE hPhone,  cal_sweep_record & swp, const unsigned short segDuration);
      unsigned char Polling (HANDLE hPhone, cal_sweep_record & swp, 
         const unsigned short segDuration, 
         const unsigned int iTimeOut, 
         const uint64 eventMask, 
         const uint32 seqType, 
         const int handle_it );
      namespace SeqBuild
      {
         unsigned char FillInCommonParams (HANDLE hPhone,  cal_sweep_record & swp,
            const unsigned short segDuration,
            const unsigned short seqType,
            const unsigned long handle_id,
            const unsigned char debugMode = 1,
            const unsigned char version = 0,
            const unsigned char isSupportParallel = 0,
            const unsigned char reserved = 0);

         unsigned char RadioSetup(HANDLE hPhone, 
            cal_sweep_record & swp, 
            unsigned short segmentDuration,
            unsigned long handle_id,
            unsigned char tech, 
            unsigned char isTearDown,  
            const ftm_radio_setup_type & radio_setup_type = RADIO_SETUP_TECH_BAND_PATH_CHAN,
            const ftm_cal_seq_bld_radio_setup_path_params * path_common_param = NULL, 
            const std::vector < CRFPathInformationPacket > * rfmPathDB = NULL );

         unsigned char FBRxGain(HANDLE hPhone, 
            cal_sweep_record & swp, 
            unsigned short segmentDuration,
            unsigned long handle_id,
            const ftm_seq_cal_bld_fbrxgain_parameters_v0 & fbrxBuildParametersV0); 

         unsigned char RxGainV1 (HANDLE hPhone, 
            cal_sweep_record & swp, 
            unsigned short segmentDuration,
            unsigned long handle_id,
            const ftm_cal_seq_bld_multi_rx_cfg_meas_parameter_type & rxParams);

         unsigned char FullBias (HANDLE hPhone, 
            cal_sweep_record & swp, 
            unsigned short segmentDuration,
            unsigned long handle_id,
            unsigned char version,
            ftm_cal_seq_bld_apt_seq_lin_parameter_type * full_bias_param = NULL);

         unsigned char AptLin (HANDLE hPhone, 
            cal_sweep_record & swp, 
            unsigned short segmentDuration,
            unsigned long handle_id,
            unsigned char version,
            ftm_cal_seq_bld_apt_seq_apt_char_data_type * apt_param = NULL);

         unsigned char Xpt (HANDLE hPhone, 
            cal_sweep_record & swp, 
            unsigned short segmentDuration,
            unsigned long handle_id,
            ftm_cal_seq_bld_xpt_params * xptParam = NULL);

      }
      namespace SeqExec
      {
         unsigned char RadioSetup(HANDLE hPhone, cal_sweep_record & SwpRecord, unsigned short segmentDuration, unsigned long handle_id, unsigned char tech);
         unsigned char FBRxGain(HANDLE hPhone, cal_sweep_record & SwpRecord, unsigned short segmentDuration, unsigned long handle_id, unsigned char overrideIndex);
         unsigned char RxGainV1(HANDLE hPhone, cal_sweep_record & SwpRecord, unsigned short segmentDuration, unsigned long handle_id );
         unsigned char FullBias(HANDLE hPhone, cal_sweep_record & SwpRecord, unsigned short segmentDuration, unsigned long handle_id, unsigned char version );
         unsigned char AptLin(HANDLE hPhone, cal_sweep_record & SwpRecord, unsigned short segmentDuration, unsigned long handle_id, unsigned char version );
         unsigned char Xpt(HANDLE hPhone, cal_sweep_record & SwpRecord, unsigned short segmentDuration, unsigned long handle_id );
    
            
         unsigned char FillInCommonParams (HANDLE hPhone,  cal_sweep_record & SwpRecord, 
            const unsigned short segDuration,
            const unsigned short seqType,
            const unsigned long handle_id,
            const unsigned char version = 0,
            const unsigned char debug_mode = 1,
            const unsigned char supportParallel = 0);
      }

      namespace Data
      {
         unsigned char ResultCommitAndNV ( HANDLE hPhone, cal_sweep_record & SwpRecord, unsigned short segmentDuration, int tech, int band, int subband, int result_mask, int nv_mask );
         unsigned char PushFBRxResultType2 ( HANDLE hPhone, cal_sweep_record & SwpRecord, unsigned short segmentDuration, int tech, int band, int subband, cal_sweep_fbrx_gain_paths_results * paths_results);     
      }
   }
   namespace COMMON
   {
      unsigned char SYNC_EFS ( HANDLE hPhone);
   };
   namespace QSC
   {
      /* Main function for QSC Cal */
      unsigned char QSC_CAL(HANDLE hPhone);

      /* User Input ( Similation of QSPR Input on XTT )for QSC */
      namespace UserConfig
      {
         typedef struct QSCUserInput
         {  
            std::vector < CTechBand > inputTechBand;
            void AddTechBand ( int _tech, int _band, int _subBand = 0) { this->inputTechBand.push_back ( CTechBand ( _tech, _band, _subBand ) ); }
         } QSCUserInput; 

         unsigned char FilePaths ( HANDLE hPhone,QSCParams & qscParams );
         unsigned char NotifyFilePaths ( HANDLE hPhone,const QSCParams & qscParams, const QSCUserInput & userInput, bool bXOCal );
         unsigned char DeviceConfig ( HANDLE hPhone, QSCParams & qscParams );
         unsigned char XOCalConfig ( HANDLE hPhone, QSCParams & qscParams );
         unsigned char SelectNodes (  HANDLE hPhone, std::vector < QSCNode > & qsc_nodes );
         unsigned char CheckFilesExists ( HANDLE hPhone, const QSCParams & qscParams );
         unsigned char GenerateQSCNodes( HANDLE hPhone, const QSCParams & qscParams, const QSCUserInput & userInput, std::vector < QSCNode > & qsc_nodes, unsigned int cal_type_mask );
      

      
      }
      /* Helper fucntion For XO Cal */
      unsigned char ExecuteRadioSetup ( HANDLE hPhone, RFMPathDB * rfmPathDBPtr, unsigned short segmentDuration, unsigned long handle_id,unsigned char tech, unsigned short band, unsigned char subband,unsigned long channel, int path_type /* PATH_TYPE_PRX or PATH_TYPE_TX */);
      unsigned char ExecuteTeardown ( HANDLE hPhone, unsigned short segmentDuration, unsigned long handle_id,unsigned char tech);
      
      /* Serial Cal */
      unsigned char ExecuteSerialCal ( HANDLE hPhone, const QSCSwpParams & swpParams, qscRecords & qsc_records ); 
      unsigned char ExecuteSwpRecords_And_FetchResults ( HANDLE hPhone, std::vector < cal_sweep_record > & SwpRecords, unsigned char bCompressedRequest, unsigned char bCompressedResponse,
         unsigned long firstSegNum, unsigned long lastSegNum, bool isLastSweep );

      /* General */
      unsigned char ExecuteQSCSweepRecords ( HANDLE hPhone, std::vector < cal_sweep_record > & SwpRecords, bool isTearDown, bool isDebug, unsigned char isCompressed);
      unsigned char ExecuteGetSweepResult ( HANDLE hPhone, std::vector < cal_sweep_record > & SwpRecords, unsigned char bCompressedResponse);

      unsigned long GetTotalSegmentTime(HANDLE hPhone, const std::vector < cal_sweep_record > & SwpRecords);

      unsigned char GetRFFE_DEVICE_INFO_V2 ( HANDLE hPhone, RFMPathDB * rfmPathDBPtr, const int tech, const int band, const int subband, 
            const CRFPathInformationPacket & txRFM_Path, PA_PARAM_KEY & pa_device_info );
      
      unsigned char GetCalibratedRFMPathInfo(   HANDLE hPhone, 
                                                const QSCSwpParams & swpParams,
                                                const CBandInfo & bandInfo,
                                                std::vector < CRFPathInformationPacket > & aggregatedResult,
                                                const std::vector<ACTION_TYPE> & actionType,
                                                const std::vector < PATH_TYPE > & pathCriteria
                                                );

      unsigned char GetSortedRFMPathVector( HANDLE hPhone, 
                                                const QSCSwpParams & swpParams,
                                                const CBandInfoPtr & perPathbandInfoPtr,
                                                const CRFPathInformationPacket & keyRFMPath, 
                                                std::vector < CRFPathInformationPacket > & rfmPaths,
                                                unsigned char iResultOrderMask = 7 /* Result Order. 7 is default. for example
                                                                                   1 (= 1 << PATH_TYPE_PRX): PRx Only
                                                                                   2 (= 1 << PATH_TYPE_DRX): DRx Only
                                                                                   3 (= 1 << PATH_TYPE_PRX + 1 << PATH_TYPE_DRX): PRx -> DRx order
                                                                                   7 (= 1 << PATH_TYPE_TX +  1 << PATH_TYPE_PRX + 1 << PATH_TYPE_DRX ) 
                                                                                   PRx -> DRx -> Tx order
                                                                                   */);

      unsigned char TE_Fetch_Result (HANDLE hPhone, std::vector < cal_sweep_record> & swpRecords, std::vector <TE_Swp_Params> teRecords); 
      unsigned char Execute_PushFBRxGain_In_ConnectedCal (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams );
      
      namespace Param
      {
          int GetBandwidth_FromBW_Type ( HANDLE hPhone, const vector < int > & bw );
      }
      namespace Log
      {
         enum 
         {
            FULL_BIAS_LINEARLIZER = 0,
            APT_LINEARLIZER,
            RGI_DELTA,
            ALIGNMENT,
         };
         unsigned char CalResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams );
         unsigned char RxGainResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams );
         unsigned char FBRxGainResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams );
         unsigned char TxCalResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams );
         unsigned char FBRxNVResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams );
         unsigned char RxRSBResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams );
         unsigned char IP2Results (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams );
		 unsigned char XOResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams );
         unsigned char OverallResults (HANDLE hPhone, const std::vector < QSCNode > & qscNodes,  QSCParams & qscParams);
         unsigned char GetCalNVID ( HANDLE hPhone, const QSCNode & qscNode, QSCParams & qscParams, seq_cal_res_type type, string & strNVID, int & nvid );
         namespace Overall
         {
            unsigned char OutputNVIDs( HANDLE hPhone, QSCParams & qscParams );
            unsigned char OutputTestTimePerNodes( HANDLE hPhone, const std::vector < QSCNode > & qscNodes, QSCParams & qscParams );

         }
         namespace RxGainCal
         {
            unsigned char StoreResults ( HANDLE hPhone, const QSCSwpParams & swpParams,   
               std::map < QSCNode, std::vector < std::pair < CRFM_Path, seq_cal_rx_cal_path_res_type > > > & rxResults, 
               std::map < QSCNode, int > & rxNVIDResults);
            unsigned char OutputResults  ( HANDLE hPhone, const QSCSwpParams & swpParams,   
               const std::map < QSCNode, std::vector < std::pair < CRFM_Path, seq_cal_rx_cal_path_res_type > > > & rxResults, 
               const std::map < QSCNode, int > & rxNVIDResults);
         };
         namespace RxRSBCal
         {
            unsigned char StoreResults ( HANDLE hPhone, const QSCSwpParams & swpParams,   
               std::map < QSCNode, 
               std::map < CRFM_Path, std::pair < uint64 ,  ftm_rx_rsb_cal_data > > > & rxResults, 
               std::map < QSCNode, int > & rxrsbNVIDResults);
            unsigned char OutputResults  ( HANDLE hPhone, const QSCSwpParams & swpParams,   
               const std::map < QSCNode,  std::map < CRFM_Path, std::pair < uint64,  ftm_rx_rsb_cal_data > > > & rxResults,  
               const std::map < QSCNode, int > & rxrsbNVIDResults);

         }
         namespace IP2Cal
         {
            unsigned char StoreResults ( HANDLE hPhone, const QSCSwpParams & swpParams,   
				std::map < QSCNode,  rfcommon_ip2_cal_band_data_type > & ip2Results,  
				std::map < QSCNode, int > & ip2NVIDResults);
            unsigned char OutputResults  ( HANDLE hPhone, const QSCSwpParams & swpParams,    
				const std::map < QSCNode,  rfcommon_ip2_cal_band_data_type > & ip2Results,  
				const std::map < QSCNode, int > & ip2NVIDResults);

         }

		 namespace XOCal
		 {
			 unsigned char StoreResults (HANDLE hPhone, const QSCSwpParams & swpParams,
				 std::map < QSCNode, map < pair <int, int>, ftm_xo_cal_default_path_data > > & xoResults);
             unsigned char OutputResults  ( HANDLE hPhone, const QSCSwpParams & swpParams,    
				const std::map < QSCNode,  map < pair <int, int>, ftm_xo_cal_default_path_data > > & xoResults);

		 }
         namespace FBRxGainCal
         {
            unsigned char StoreResults ( HANDLE hPhone, const QSCSwpParams & swpParams,   
               std::map < QSCNode, std::map < CRFM_Path, ftm_fbrxgain_path_result > > & fbrxResults,
               std::map < fbrx_gain_results_input_identifiers, CFBRx_MeasParams  > & TxResultMap);
            
            unsigned char OutputResults ( HANDLE hPhone, const QSCSwpParams & swpParams,   
               std::map < QSCNode, std::map < CRFM_Path, ftm_fbrxgain_path_result > > & fbrxResults,
               std::map < fbrx_gain_results_input_identifiers, CFBRx_MeasParams  > & TxResultMap);

         }
         namespace TxCal
         {
            unsigned char StoreResults ( HANDLE hPhone,  const QSCSwpParams & swpParams,  
               map < CRFM_Path, CTX_Results > & txResults,
               int & tx_nv_id );
            unsigned char OutputResults ( HANDLE hPhone, const QSCNode & qscNode,  QSCSwpParams & swpParams,  
               const map < CRFM_Path, CTX_Results > & txResults,
               int & tx_nv_id );

            
            int GetBWFromWvIndex ( HANDLE hPhone, unsigned char wvIndex );
            string GetBWString( HANDLE hPhone, int tech, int bw );
            namespace Output
            {
               unsigned char CreatePA_XPT_ModeMap ( HANDLE hPhone, const QSCNode & qscNode,  QSCSwpParams & swpParams, std::map < int, int > & pa_xpt_mode_map );
               unsigned char Linearlizer (HANDLE hPhone, const QSCNode & qscNode,  QSCSwpParams & swpParams, int result_type, const int bw, int xpt_mode, const PA_DpdType_Waveform_TxResult_Map & tx_results, stringstream & ss);
               unsigned char FreqComp (HANDLE hPhone, const QSCNode & qscNode,  QSCSwpParams & swpParams, 
                  const vector < int > & freqcomp_channels, const PA_fcompOffsets_map & tx_freqcomp_results, const int xpt_type, stringstream & ss);
               unsigned char XptCalStatus (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, const XptMode_results_map & xpt_results, const int xpt_mode, stringstream & ss );
               unsigned char Process_Linearlizer_SetXptWaveform (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, const int result_type, const int bw, int & pa_xpt_mode, int & dpdType, int & waveform );
               string GetCalStatusErrorCode(int cal_status);
               unsigned char XptDebugInfoResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, const xptDebugInfoType & xpt_debug_info, const int xpt_mode_type, const int bw, stringstream & ss);
               namespace XptDebugInfo
               {
                  unsigned char Stg1Pwr (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, const CXPT_Params & xptParams, const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, stringstream & ss);
                  unsigned char Mline_ResultTable (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, const CXPT_Params & xptParams, const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, int version, stringstream & ss);
                  unsigned char Mline_SelectSwp (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, const CXPT_Params & xptParams,  const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, stringstream & ss, const int mode /* 0: calc, 1: meas */);
                  unsigned char Mline_Slope (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, const CXPT_Params & xptParams, const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, stringstream & ss);
                  unsigned char Mline_CompPts (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, const CXPT_Params & xptParams, const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, stringstream & ss);
                  unsigned char Mline_DebugMask (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, const CXPT_Params & xptParams, const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, stringstream & ss);
                  unsigned char Mline_EtDelay (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, const CXPT_Params & xptParams,  const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, stringstream & ss);

                  string GetXptModeName ( const int xpt_mode );
               }
            }
         }
      }
      namespace Read
      {
         unsigned char ParamXML(HANDLE hPhone, QSCParams & params);
         unsigned char PAParamXML(HANDLE hPhone, QSCParams & params);
         unsigned char CharXML(HANDLE hPhone, QSCParams & params);
         unsigned char RxCharXML(HANDLE hPhone, QSCParams & params);

         unsigned char EquivConfigXML ( HANDLE hPhone, QSCParams & params);
         namespace Node
         {
            unsigned char Default( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, QSCSwpParams & swpParams);
            unsigned char CalType( HANDLE hPhone,  const MSXML2::IXMLDOMNodePtr & pNode, CCalType & calType, int sweep_type = 0);

            unsigned char Common( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CCommonParam & commonParams);
            unsigned char Timing_Params( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CCommonParam & commonParams);
            unsigned char Waveform_Params( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CCommonParam & commonParams);

            unsigned char FBRx_Gain( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CFBRx_Gain & fbrxParams);
            unsigned char Rx_Gain( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CRx_Gain & rxParams);
            unsigned char QSC_XO( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CXo & xoParams);
            unsigned char Rx_RSB( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CRx_RSB & rsbParams);

            unsigned char BandInfo( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, QSCSwpParams & swpParams);
            unsigned char Path_Info( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CBandInfo & bandInfo);

            unsigned char Tx_Linearizer( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CTx_Linearizer & txLinParams);
            unsigned char Tx_XPT( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CTx_XPT & tx_xptParams);
            unsigned char XPT_Params( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CXPT_Params & xptParams);
            
			unsigned char IP2_Cal (HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CRx_IP2 &ip2Params); 
            
         }
      }
      namespace CreateSwpRecords
      {
         unsigned char ConnectedCal( HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams);
         unsigned char UnconnectedCal( HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams); // 
		 unsigned char XoCal( HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams); 
         unsigned char FBRx_Rx_Cal( HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams); // later merge to connected cal with fdrsb cal
         unsigned char Align_TE_Reords ( HANDLE hPhone, qscRecords & orgQscRecords);
         unsigned char Tx_Xpt_Cal( HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams); 
         unsigned char IP2_Cal( HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams); 
         unsigned char GetPerPathParam ( HANDLE hPhone, CBandInfo & bandInfo, CBandInfoPtr & bandInfoPtr, const CRFPathInformationPacket & rfmPath);
         unsigned char TransformToSerialRxCal ( HANDLE hPhone, const QSCSwpParams & swpParams, qscRecords & orgQscRecords); 
         unsigned char Adjust_Serial_Rx_TE_RecordList ( HANDLE hPhone, const QSCSwpParams & swpParams, qscRecords & orgQscRecords); 
         
         
         unsigned char DeallocateSweepRecords ( HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams);
         unsigned char Check_RFM_Path_Not_Calibrated ( HANDLE hPhone, const std::vector < CRFPathInformationPacket > & rfm_path_list,  
            const std::set < CRFM_Path > & calibrated_rfm_path_list);
         unsigned char Store_Calibrated_RFM_Path_List ( HANDLE hPhone, const CBandInfoPtr & perPathBandInfo, const std::vector < CRFPathInformationPacket > & rfm_path_list,  
            std::set < CRFM_Path > & calibrated_rfm_path_list);
         unsigned char PushFBRxGain_In_ConnectedCal (HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams );
         
         namespace Add
         {
            unsigned char RadioSetup ( HANDLE hPhone, unsigned long & handle_id, ftm_radio_setup_type radio_setup_type, QSCSwpParams & swpParams,
                  const CBandInfoPtr & perPathBandInfo, const CRFPathInformationPacket & rfmPath, const int channel, const vector < int > & bwList );
          
            unsigned char FBRxGain ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, const CRFPathInformationPacket & rfmPath, const int channel );
            // Only Rx Gain needs bandInfo since it requires multiple rfm path info
            unsigned char RxGain ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo,  const CRFPathInformationPacket & rfmPath, const int channel, const vector < CRFPathInformationPacket > & rfm_path_list );
            unsigned char FillinRxGainSeqBldParams ( HANDLE hPhone, const QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo,
               map < const CRFM_Path,  CRx_Gain > & RxParamsPerPathPerDL,const CRFPathInformationPacket & rfmPath, ftm_cal_seq_bld_multi_rx_cfg_meas_parameter_type & rxParams);
            unsigned char GetRxParamsPerPathPerDL ( HANDLE hPhone, const QSCSwpParams & swpParams,  const CBandInfoPtr & perPathBandInfo,  const vector < CRFPathInformationPacket > & rfm_path_list,
                std::vector < map < const CRFM_Path, CRx_Gain > > & rx_params_per_path_per_DL );
            
            unsigned char GetRxPathParams ( HANDLE hPhone, const CBandInfoPtr & perPathBandInfo,  const vector < CRFPathInformationPacket > & rfm_path_list,  std::map < const CRFM_Path, const CRx_Gain *  > & pRxPerPathParams );
 
            unsigned char FillinFBRxGainSeqBldParams ( HANDLE hPhone,  unsigned char isTxOverrideOnly, const CFBRx_Gain * fbrxGainParams, const int channel, ftm_seq_cal_bld_fbrxgain_parameters_v0 & param);
            unsigned char NewRecord ( HANDLE hPhone, qscRecords & qsc_record, bool usePrevSegment = true);
            unsigned char Teardown ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo);
            unsigned char Data_ResultNV ( HANDLE hPhone, QSCSwpParams & swpParams, const CBandInfoPtr & perPathbandInfoPtr, unsigned char isLastRfmPath = true );
            unsigned char Data_PushFBRx ( HANDLE hPhone, QSCSwpParams & swpParams, const CBandInfo & bandInfo, const std::map < CRFM_Path, ftm_fbrxgain_path_result > & fbrx_rfmPath_ResultMap);
            unsigned char Data_PushRxChar ( HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams);

            unsigned char FillinDataPushFBRx ( HANDLE hPhone, QSCSwpParams & swpParams, const CBandInfo & bandInfo, 
                  const std::map < CRFM_Path, ftm_fbrxgain_path_result > & rfm_result_map, cal_sweep_fbrx_gain_paths_results * paths_results);
            
            unsigned char Tx_Fullbias ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo);            
            unsigned char FillinTx_Fullbias ( HANDLE hPhone, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, ftm_cal_seq_bld_apt_seq_lin_parameter_type & full_bias_param );

            unsigned char Tx_Apt ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, int Txpurpose, const CBandInfoPtr & perPathBandInfo);
            unsigned char FillinTx_AptLin ( HANDLE hPhone, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, int Txpurpose, ftm_cal_seq_bld_apt_seq_apt_char_data_type & aptCharParam );

            unsigned char Tx_Xpt ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, const PA_PARAM_KEY & pa_device_info, const int BW );	   
            unsigned char FillinTx_XptLin ( HANDLE hPhone, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, 
               const int xpt_mode, const int BW, const PA_PARAM_KEY & pa_device_info,  ftm_cal_seq_bld_xpt_params & xptParam );
            unsigned char FdRSB ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, const CRFPathInformationPacket & rfmPath, const int channel );
            unsigned char IP2 ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, const vector<int> & IP2bwList, const int channel );
            unsigned char XO ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, const int channel );
            unsigned short GetTxOverrideRGIMeasLen( HANDLE hPhone, int tech );
         };
         namespace MatrixMode
         {
            unsigned char Pad_Nops ( HANDLE hPhone, const QSCSwpParams & swpParams, qscRecords & orgQscRecords ); 
            unsigned char Find_BaseFrame ( HANDLE hPhone, const QSCSwpParams & swpParams, const qscRecords & orgQscRecords, qscRecords & baseFrame );
            int Get_Index_In_BaseFrame ( HANDLE hPhone, const qscRecords & baseFrame, const cal_sweep_record & swpRecord, const TE_Swp_Params & teRecord ); 
            unsigned char Create_Frame_Records (  HANDLE hPhone, const QSCSwpParams & swpParams, qscRecords & orgQscRecords ); 
         };
      };


   } // End of QSC namespace


   namespace UserConfig
   {
      unsigned char Set_CalDBFilePath(HANDLE hPhone, string & caldb_filepath);
      unsigned char Set_EquipConfigFilePath(HANDLE hPhone, string & equipConfig_filepath);
      unsigned char Set_OutputResultsFilePath(HANDLE hPhone, const string & baseFilePath, const string & headStr, string & QMSLLogFilePath, string & QCNFilePath, string & DLFFilePath );  
   };
}