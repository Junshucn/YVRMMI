/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/RFCal.cpp#41 $
 * $DateTime: 2017/09/15 02:29:33 $
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

#include <ctime>

#include "RFCal.h"
#include "RFCal_Helper.h"
#include <iterator>

using namespace std;

unsigned char RFCal::QSC::QSC_CAL(HANDLE hPhone)
{ 
   RFCal::QSC::UserConfig::QSCUserInput userInput;
   vector < QSCNode > qsc_nodes;   // Specify "Tech, CalType and Instance (and band/subband)" to calibrate
   QSCParams qscParams;          // Store all the parameters (of all tech/bands) and sweep/TE records
   qscParams.DutInfo.hPhone = hPhone;
   ::QLIB_DIAG_ClearExtendedLogCode(hPhone, 1, 0xd9);
   ::QLIB_DIAG_ClearAllLogCodes(hPhone, 1);

   bool bReboot = false; 
   bool bXOCal_FTM = false;
   // **************************************************************** //
   // Force Push FBRx Data In Connected cal (QlibDemo only for now
   // True : Push FBRx Data in connected, and disable in Unconnected cal
   // **************************************************************** //   
   qscParams.bForce_Push_FBRxData_In_ConnectedCal = false;

   // **************************************************************** //
   // User Configuartion: Read param XMLs 
   // **************************************************************** //

   CoInitialize(NULL); // Initialize COM

   qscParams.repository.Init(); //Initialize Repository

   RFCal::QSC::UserConfig::FilePaths ( hPhone, qscParams );

   RFCal::QSC::UserConfig::DeviceConfig ( hPhone, qscParams );

   RFCal::QSC::UserConfig::XOCalConfig ( hPhone, qscParams );

   deviceMutex = CreateMutex( NULL, FALSE, NULL); // Create mutex. This must be created outside this funciton. For demo purpose, created here.
   unsigned int cal_type_mask = CONNECTED_CAL_FBRX_RX | CONNECTED_CAL_RxRSB | UNCONNECTED_CAL_TX;
#if 1
   // **************************************************************** //
   // User Input: Specify tech and band (and subband) to calibrate
   // **************************************************************** //

   // **************************************************************** //
   // Method 1. (QlibDemo only) Specify tech and band
   // This user input will be converted to QSCNodes
   // Cal Order: All the connected cals --> All the unconnected cals
   // **************************************************************** //
   // userInput.AddTechBand ( TECHNOLOGY_CDMA, 0 /* Band */ ) ; // BC0
   // userInput.AddTechBand ( TECHNOLOGY_WCDMA, 1 /* Band */ ) ;
   //   userInput.AddTechBand ( TECHNOLOGY_TDSCDMA, 34 /* Band */ ) ;
   userInput.AddTechBand ( TECHNOLOGY_LTE, 1 /* Band */ ) ;
//   userInput.AddTechBand ( TECHNOLOGY_LTE, 2 /* Band */ ) ;
//   userInput.AddTechBand ( TECHNOLOGY_LTE, 3 /* Band */ ) ;
//   userInput.AddTechBand ( TECHNOLOGY_LTE, 8 /* Band */ ) ;
//   userInput.AddTechBand ( TECHNOLOGY_LTE, 28 /* Band */, 0 /* Subband */ ) ; // LTE Band 28A
//   userInput.AddTechBand ( TECHNOLOGY_LTE, 28 /* Band */, 1 /* Subband */ ) ; // LTE Band 28B
#endif

#if 0
   // **************************************************************** //
   // Method 2. User Input: Specify tech, sweep type and instance
   // This is the same as XTT node
   // **************************************************************** //
  // qsc_nodes.push_back( QSCNode ( TECHNOLOGY_LTE, SEQ_CAL_XO_CAL, 0 /* Instance */, 1/* Band */, 0 /* Subband */));

   qsc_nodes.push_back( QSCNode ( TECHNOLOGY_LTE, SEQ_CAL_CONNECTED, 0 /* Instance */));
   qsc_nodes.push_back( QSCNode ( TECHNOLOGY_LTE, SEQ_CAL_UNCONNECTED, 0 /* Instance */, 1/* Band */, 0 /* Subband */));
  // qsc_nodes.push_back( QSCNode ( TECHNOLOGY_LTE, SEQ_CAL_UNCONNECTED, 200 /* Instance */, 1/* Band */, 0 /* Subband */));// IP2 Cal

#endif
#if 0
   // **************************************************************** //
   // Method 3. User Input: Specify tech, sweep type and instance
   // This is the same as XTT node (and method 2)
   // **************************************************************** //
   RFCal::QSC::UserConfig::SelectNodes ( qsc_nodes );
#endif
#ifdef QLIB_DEMO_EXT
   if ( ! RFCal::QSC::UserConfig::NotifyFilePaths ( hPhone, qscParams, userInput, bXOCal_FTM ) )
   {
      return false;
   }
#endif
   // **************************************************************** //
   // QMSL Start logging
   // **************************************************************** //
   RFCal::Helper::Log::StartQMSLLogging ( hPhone, qscParams.FilePath.stQMSLLog, true);
   RFCal::Helper::DIAG::GetBuildID ( hPhone, qscParams.DutInfo.stBuildID );
   unsigned char bDLFLogging = false;
   
   if ( bDLFLogging ) 
   {
      bDLFLogging = RFCal::Helper::DIAG::Set_DMC_File( hPhone, "C:\\Temp\\qxdm.dmc" );
      
      QLIB_StartDLF_Logging(hPhone, (char *)qscParams.FilePath.stDLFLog.c_str(), true);
      
   }
  
   // **************************************************************** //
   // Parameter File Check
   // **************************************************************** //
   if ( ! RFCal::QSC::UserConfig::CheckFilesExists ( hPhone, qscParams ) )
   {
      return false;
   }

   // **************************************************************** //
   // Parameter XML reading
   // **************************************************************** //

   if ( Read::ParamXML(hPhone, qscParams) )
   {
      Read::CharXML(hPhone, qscParams);
      Read::PAParamXML(hPhone, qscParams);
      Read::RxCharXML(hPhone, qscParams);
   }
   else
   {
      return false;
   }
   CoUninitialize();

   // **************************************************************** //
   // Helper: Generate QSC Nodes for readability (Method 1 only)
   // **************************************************************** //
   RFCal::QSC::UserConfig::GenerateQSCNodes( hPhone, qscParams, userInput, qsc_nodes, cal_type_mask );

   // **************************************************************** //
   // Reboot DUT 
   // **************************************************************** //
   if ( bReboot )
   {
      RFCal::Helper::DIAG::RebootPhone ( hPhone );
   }
   // **************************************************************** //
   // Check FTM mode
   // **************************************************************** //   
   RFCal::Helper::DIAG::Change_FTM_Mode(hPhone, 3, 200);

   // ************************************************ //
   // Equipment Configuration
   // ************************************************ //
   qscParams.DeviceConfig.similation_mode = false; // Set true if you don't have a callbox connected
   RFCal::DeviceDB::GetInstance().RegisterDevice (hPhone, qscParams.DeviceConfig.type, qscParams.DeviceConfig.boardId, qscParams.DeviceConfig.primaryAddr, qscParams.DeviceConfig.secondaryAddr, ( qscParams.DeviceConfig.similation_mode)? true : false  );
   RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();
   
   if ( NULL == pMgr )
   {
#ifdef USE_GPIB_32_DLL
      RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "Device manager failed loading. Check GPIB-32.dll exists or equivconfig.xml");
#else
      RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "Device manager failed loading. Check equivconfig.xml");
#endif
      return false;
   }
   pMgr->Enable_ConsoleOutput ( false );
   pMgr->Get_DeviceName(); // Execute IDN
   pMgr->Init(); // Initialization for callbox

   // ************************************************ //
   // Set Calibration state on
   // ************************************************ //
   QLIB_FTM_SET_CALIBRATION_STATE(hPhone, true);

   // ************************************************ //
   // Internal Device Cal and XO Cal
   // ************************************************ //
   if ( bXOCal_FTM )
   {
      /* Internal Device Cal */
      RFCal::Do_FTM_DO_SELF_CAL (hPhone);

      /* XO Cal */
      CXOCal_Config & xo_param = qscParams.XOCalConfig;
      RFCal::SetPathLoss(hPhone, (*pMgr), qscParams.FilePath.stCalDBXML, xo_param.tech );

      RFCal::XOCal::DO_XO_Cal (hPhone, xo_param.tech, xo_param.band , xo_param.subband, xo_param.channel,
         xo_param.ftm_xo_cal_params /* XO cal param. Must be assigned before */,
         true /* true for RFM Path info Query, false for traditional */, &qscParams.repository.rfmPathDB );
   }

   int prevTech = -1;

   // ************************************************ //
   // QSC Cal - Node Loop
   // ************************************************ //


   for (vector <QSCNode>::iterator it = qsc_nodes.begin(); it != qsc_nodes.end(); it++)
   {
      // ************************************************ //
      // Check Phone is active for each of cal ( in case of crash)
      // ************************************************ //
      RFCal::Helper::DIAG::IsPhoneConneted(hPhone, 100, 200);

      // ************************************************ //
      // Get RFM Path. ( Execute per tech )
      // Set path loss for callbox ( Execute per tech )
      // ************************************************ //

      if ( prevTech != it->tech) 
      {

		 if ( SEQ_CAL_CONNECTED == it->sweep_type || SEQ_CAL_XO_CAL == it->sweep_type)
         {

            pMgr->Set_Tech(static_cast<int>(it->tech));

            RFCal::SetPathLoss(hPhone, (*pMgr), qscParams.FilePath.stCalDBXML, it->tech);
         }
         // ************************************************ //
         // Get RFM Path info and save it to the data base
         // ************************************************ //
         if (!(qscParams.repository.rfmPathDB.CreateRFMPathDB(hPhone, static_cast<byte>(it->tech), -1 /* all band */, 0, 1)))
         {
            RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, "Failed in getting RFM Path info ");
            continue;
         }
         prevTech = it->tech;
         ::Sleep (10); // wait for callbox in case
      }

      // ************************************************ //
      // Test Time Starts
      // ************************************************ //
      qscParams.repository.TestTimeStart ( hPhone);

      // Check Parameters for this tech/band/subband exists or not
      std::map < QSCNode, QSCSwpParams >::iterator swpParamIt = qscParams.qscParams.find(*it);
      if (swpParamIt == qscParams.qscParams.end()) // Parameters for this tech/band/subband not found
      {
         continue;
      }

      // ************************************************ //
      // Create Sweep Records
      // ************************************************ //
      const QSCNode & qscNode = swpParamIt->first;
      QSCSwpParams & swpParams = swpParamIt->second;
      swpParams.repoPtr = &qscParams.repository;
     
      switch (it->sweep_type)
      {
         case SEQ_CAL_CONNECTED:
            // **************************************************************** //
            // Create Connnected Cal sequence
            // **************************************************************** //
            if ( !RFCal::QSC::CreateSwpRecords::ConnectedCal( hPhone, qscNode, swpParams ))
            {
               RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "Sweep Records creation failed");
               return false;
            }
            break;

         case SEQ_CAL_UNCONNECTED:
            // **************************************************************** //
            // Create Unconnnected Cal sequence
            // **************************************************************** //
            if ( !RFCal::QSC::CreateSwpRecords::UnconnectedCal( hPhone, qscNode, swpParams ) )
            {
               RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "Sweep Records creation failed");
               return false;
            }
            break;

		 case SEQ_CAL_XO_CAL:
			// **************************************************************** //
            // Create QSC XO Cal sequence
            // **************************************************************** //

			if ( !RFCal::QSC::CreateSwpRecords::XoCal( hPhone, qscNode, swpParams ) )
            {
               RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "Sweep Records creation failed");
               return false;
            }
            break;

         default:
            // Error
            break;
      }

      // *************************************************************** //
      // Execute Sweep Records
      // Multiple sweep loop
      // Note: Current QSC has single sweep only for each of instances
      // *************************************************************** //
      for ( vector < qscRecords >::iterator sweep_it = swpParams.swps.begin(); sweep_it != swpParams.swps.end(); sweep_it ++)
      {
         if (swpParams.calType.Enable_Sweep_Mode)
         {
            // **************************************************************** //
            // Config equipment sequence (Connected cal only)
            // **************************************************************** //
            if ( SEQ_CAL_CONNECTED == swpParams.sweep_type )
            {
               // **************************************************************** //
               // If Matrix mode required, then transform swp record into matrix style
               // **************************************************************** //
               if ( qscParams.DeviceConfig.matrix_mode )
               {
                  RFCal::QSC::CreateSwpRecords::MatrixMode::Pad_Nops ( hPhone, swpParams, (* sweep_it));
                  RFCal::QSC::CreateSwpRecords::Align_TE_Reords ( hPhone, (* sweep_it ));
                  RFCal::QSC::CreateSwpRecords::MatrixMode::Create_Frame_Records ( hPhone, swpParams, (* sweep_it));
                  IMeasurementMatrixMode * pMatrixMgr = dynamic_cast < IMeasurementMatrixMode * > ( pMgr );
                  if ( pMatrixMgr )
                  {
                     pMatrixMgr->Config_Cal_Sweep ( sweep_it->frame_teRecords );
                  }
               }
               else
               {  
                  RFCal::QSC::CreateSwpRecords::Align_TE_Reords ( hPhone, (* sweep_it ));
                  pMgr->Config_Cal_Sweep ( sweep_it->teRecords );   
               }                

               pMgr->Set_OutputPort ( swpParams.calType.Enable_Downlink_Aux_Port );

               pMgr->Start_Cal_Sweep ();
            }

            // **************************************************************** //
            // Execute DUT sweep record
            // **************************************************************** //
            RFCal::QSC::ExecuteQSCSweepRecords ( hPhone, sweep_it->swpRecords, 
                  ( ! swpParams.calType.Skip_Radio_TearDown ), 
                  true /* debug mode */,
                  ( swpParams.defaultParams.commonParams.En_Compress_Esc_Req ) );

            if ( SEQ_CAL_CONNECTED == swpParams.sweep_type )
            {
               // **************************************************************** //
               // Get equipment Tx meas results ( Connected cal only )
               // **************************************************************** //
               RFCal::QSC::TE_Fetch_Result ( hPhone, sweep_it->swpRecords, sweep_it->teRecords);
            }
            else
            {
               // **************************************************************** //
               // Calculate Wait time
               // **************************************************************** //
               uint32 iTotalSegDuration =  ( RFCal::QSC::GetTotalSegmentTime (hPhone, sweep_it->swpRecords) ) / 1000 + 20;
               if ( swpParams.calType.Enable_Tx_XPT )
                  iTotalSegDuration = static_cast < uint32 > ( iTotalSegDuration * 1.0 );
               if ( swpParams.BandInfo.back().commonParams.En_Compress_Esc_Req )
                  iTotalSegDuration += 20;
               if ( swpParams.BandInfo.back().commonParams.En_Compress_Esc_Res )
                  iTotalSegDuration += 20;
               
               Sleep(iTotalSegDuration);
            }
            // **************************************************************** //
            // Get DUT sweep results
            // **************************************************************** //
            RFCal::QSC::ExecuteGetSweepResult ( hPhone, sweep_it->swpRecords, swpParams.defaultParams.commonParams.En_Compress_Esc_Res );          
         }
         else
         {
            // **************************************************************** //
            // Serial Rx Cal 
            // **************************************************************** //
            // Do not Add align TE records for serial cal
            //RFCal::QSC::CreateSwpRecords::Align_TE_Reords ( hPhone, (* sweep_it ));
           
            // Transform cal records into serial cal style (removing FBRx/Nop and modify Rx Cal opcode order)
            RFCal::QSC::CreateSwpRecords::TransformToSerialRxCal ( hPhone, swpParams, (* sweep_it) );
            
            // Adjust te records to reduce cal time
            RFCal::QSC::CreateSwpRecords::Adjust_Serial_Rx_TE_RecordList ( hPhone, swpParams, (* sweep_it) );

            // Execute Serial Cal
            RFCal::QSC::ExecuteSerialCal ( hPhone, swpParams, (* sweep_it) );
         }
      }

      // ************************************************ //
      // Logging
      // ************************************************ //
      RFCal::QSC::Log::CalResults ( hPhone, qscNode, swpParams );

      // ************************************************ //
      // Deallocation of memory
      // ************************************************ //
      RFCal::QSC::CreateSwpRecords::DeallocateSweepRecords( hPhone, qscNode, swpParams );

      // ************************************************ //
      // Push FBRx Gain Data (if parameter enabled) // QLibDemo Only for now
      // ************************************************ //
      RFCal::QSC::Execute_PushFBRxGain_In_ConnectedCal ( hPhone, qscNode, swpParams );

      // ************************************************ //
      // Send Sync to make sure generate NVs are synched
      // ************************************************ //
      if ( (it+1) == qsc_nodes.end()
         || prevTech != (it+1)->tech)
      {
         RFCal::COMMON::SYNC_EFS( hPhone );

      }

      // ************************************************ //
      // Test Time end
      // ************************************************ //
      qscParams.repository.TestTimeEnd ( hPhone, (* it) );
   }
   // ************************************************ //
   // Logging overall results
   // ************************************************ //
   RFCal::QSC::Log::OverallResults ( hPhone, qsc_nodes, qscParams );

   // ************************************************ //
   // Stop QMSL Logging
   // ************************************************ //
   if ( bDLFLogging )
   {
      ::QLIB_StopDLF_Logging ( hPhone );
   }
   RFCal::Helper::Log::StopQMSLLogging(hPhone);
   std::cout << "QMSL Log: " << qscParams.FilePath.stQMSLLog << std::endl;
   
   RFCal::DeviceDB::GetInstance().UnregisterDevice();

   CloseHandle( deviceMutex ); // CloseHendle for the deviceMutex must be called outside this function. For demo purpose, called here.
   // hPhone handle must be terminated 
   return true;
}
unsigned char RFCal::UserConfig::Set_CalDBFilePath(HANDLE hPhone, string & caldb_filepath)
{
   // ****************************** //
   // Set CalDB XML
   // ****************************** //
   string calDbXML = "CalDB_NET.xml";
   string QDART_CalDB_XML = "C:\\HTE\\Tools\\SUITE-3G-EV\\Databases\\" + calDbXML;
   if ( access ( QDART_CalDB_XML.c_str() , 0 ) != 0 )
   {
      QDART_CalDB_XML = "C:\\Program Files (x86)\\Qualcomm\\QDART\\Databases\\" + calDbXML;
      if ( access ( QDART_CalDB_XML.c_str(), 0)  != 0 )
      {
         QDART_CalDB_XML = "C:\\Program Files\\Qualcomm\\QDART\\Databases\\" + calDbXML;
         if ( access ( QDART_CalDB_XML.c_str(), 0)  != 0 )
         {
            RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "CalDB_NET.xml is not found" );
            return false;
         }
      }
   }
   caldb_filepath = QDART_CalDB_XML;
   return true;
}
unsigned char RFCal::UserConfig::Set_EquipConfigFilePath(HANDLE hPhone, string & equipConfig_filepath)
{
   // ****************************** //
   // Set equipconfig Paths
   // ****************************** //
   string equipconfigXML = "equipconfig.xml";
   string QDART_equipconfig_XML = "C:\\HTE\\Tools\\SUITE-3G-EV\\Databases\\" + equipconfigXML;
   if ( access ( QDART_equipconfig_XML.c_str() , 0 ) != 0 )
   {
      QDART_equipconfig_XML = "C:\\Program Files (x86)\\Qualcomm\\QDART\\Databases\\" + equipconfigXML;
      if ( access ( QDART_equipconfig_XML.c_str(), 0)  != 0 )
      {
         QDART_equipconfig_XML = "C:\\Program Files\\Qualcomm\\QDART\\Databases\\" + equipconfigXML;
         if ( access ( QDART_equipconfig_XML.c_str(), 0)  != 0 )
         {
            RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "equipconfig.xml is not found" );
            return false;
         }
      }
   }
   equipConfig_filepath = QDART_equipconfig_XML;
   return true;
}
unsigned char RFCal::UserConfig::Set_OutputResultsFilePath(HANDLE hPhone, const string & baseFilePath, const string & headStr, string & QMSLLogFilePath, string & QCNFilePath, string & DLFFilePath  )
{
   string QDART_Temp_Output;
   if ( baseFilePath.empty() || baseFilePath.length() == 0)
   {
      QDART_Temp_Output = "C:\\Qualcomm\\QDART\\Temp";
   }
   else
   {
      QDART_Temp_Output = baseFilePath;
   }

   if ( access ( QDART_Temp_Output.c_str(), 0 ) != 0 )
   {
      QDART_Temp_Output = "c:\\temp";
   }

   string filePathHeader;
   filePathHeader = headStr;
   filePathHeader += "_20XX_MMDD_HHMM ";
   string filepathForTime;
   filepathForTime = headStr;
   filepathForTime += "_%Y_%m%d_%H%M";
   time_t curtime;
   struct tm *loctimep;
   curtime = time( NULL );
   loctimep = localtime( &curtime );
   if ( loctimep )
   {
      strftime( &filePathHeader[0], filePathHeader.length(), filepathForTime.c_str(), loctimep);
   }
   filePathHeader[0] = 'Q';

   QDART_Temp_Output += "\\";
   QDART_Temp_Output += filePathHeader.substr ( 0, filePathHeader.length() - 1);
   
   QCNFilePath = QDART_Temp_Output + "_calNVs.xqcn";
   QMSLLogFilePath = QDART_Temp_Output + "_QMSL_Log.txt";
   DLFFilePath = QDART_Temp_Output + ".dlf"; 
   return true;
}
unsigned char RFCal::QSC::UserConfig::FilePaths (  HANDLE hPhone, QSCParams & qscParams )
{
   // ****************************** //
   // Set Parameter XML Paths
   // ****************************** //

   string param_folder = "c:\\temp\\"; // Set working folder correctly, otherwise relative does not work.

   qscParams.FilePath.stParamXML    = param_folder + "SDR845_MIMO_NA_LAA_V2_QSC_Params.xml";
   qscParams.FilePath.stCharXML     = param_folder + "SDR845_MIMO_NA_LAA_V2_ESC_Char.xml";
   qscParams.FilePath.stPAParamXML  = param_folder + "RFCAL_PA_PARAM_V2.xml";

//   qscParams.FilePath.stRxCharXML   = "C:\\Qualcomm\\QDART\\Temp\\OutputFiles\\RxChar\\RxCharMeas\\EmptyUutID.xml";
   
   // ****************************** //
   // Set CalDB XML
   // ****************************** //  
   RFCal::UserConfig::Set_CalDBFilePath ( hPhone, qscParams.FilePath.stCalDBXML );

   // ****************************** //
   // Set equipconfig Paths
   // ****************************** //
   RFCal::UserConfig::Set_EquipConfigFilePath ( hPhone, qscParams.FilePath.stEquipConfigXML);

   // ****************************** //
   // Set Output Filepaths (XQCN and QMSL log)
   // ****************************** //
   RFCal::UserConfig::Set_OutputResultsFilePath ( hPhone, "C:\\Qualcomm\\QDART\\Temp","QSC_Cal",  qscParams.FilePath.stQMSLLog, qscParams.FilePath.stCalibratedXQCN, qscParams.FilePath.stDLFLog);

   return true;
}
unsigned char RFCal::QSC::UserConfig::SelectNodes ( HANDLE hPhone, vector < QSCNode > & qsc_nodes )
{
   /* CDMA BC 0 */
   qsc_nodes.push_back( QSCNode( TECHNOLOGY_CDMA, SEQ_CAL_CONNECTED, 0 /* Instance */));
   qsc_nodes.push_back( QSCNode( TECHNOLOGY_CDMA, SEQ_CAL_CONNECTED, 1 /* Instance */));

   qsc_nodes.push_back( QSCNode( TECHNOLOGY_CDMA, SEQ_CAL_UNCONNECTED, 0 /* Instance */, 0 /* Band */, 0 /* Subband */));
   qsc_nodes.push_back( QSCNode( TECHNOLOGY_CDMA, SEQ_CAL_UNCONNECTED, 0 /* Instance */, 1 /* Band */, 0 /* Subband */));

   /* WCDMA B1 */
   qsc_nodes.push_back( QSCNode( TECHNOLOGY_WCDMA, SEQ_CAL_CONNECTED, 0 /* Instance */));
   qsc_nodes.push_back( QSCNode( TECHNOLOGY_WCDMA, SEQ_CAL_CONNECTED, 1 /* Instance */));

   qsc_nodes.push_back( QSCNode( TECHNOLOGY_WCDMA, SEQ_CAL_UNCONNECTED, 0 /* Instance */, 1 /* Band */, 0 /* Subband */));
   qsc_nodes.push_back( QSCNode( TECHNOLOGY_WCDMA, SEQ_CAL_UNCONNECTED, 0 /* Instance */, 2 /* Band */, 0 /* Subband */));

   /* TDSCDMA B34 */
   qsc_nodes.push_back( QSCNode( TECHNOLOGY_TDSCDMA, SEQ_CAL_CONNECTED, 0 /* Instance */));
   qsc_nodes.push_back( QSCNode( TECHNOLOGY_TDSCDMA, SEQ_CAL_UNCONNECTED, 0 /* Instance */, 34 /* Band */, 0 /* Subband */));

   /* LTE B1, B12+B29, LTEB66 */

   qsc_nodes.push_back( QSCNode( TECHNOLOGY_LTE, SEQ_CAL_CONNECTED, 0 /* Instance */));
   qsc_nodes.push_back( QSCNode( TECHNOLOGY_LTE, SEQ_CAL_CONNECTED, 1 /* Instance */));

   qsc_nodes.push_back( QSCNode( TECHNOLOGY_LTE, SEQ_CAL_CONNECTED, 8 /* Instance */)); // GSKU, Multiband B12 + B29 case
   qsc_nodes.push_back( QSCNode ( TECHNOLOGY_LTE, SEQ_CAL_CONNECTED, 28 /* Instance */));
  
   qsc_nodes.push_back( QSCNode( TECHNOLOGY_LTE, SEQ_CAL_UNCONNECTED, 0 /* Instance */, 1 /* Band */, 0 /* Subband */));
   qsc_nodes.push_back( QSCNode( TECHNOLOGY_LTE, SEQ_CAL_UNCONNECTED, 0 /* Instance */, 2 /* Band */, 0 /* Subband */));

   qsc_nodes.push_back( QSCNode( TECHNOLOGY_LTE, SEQ_CAL_UNCONNECTED, 0 /* Instance */, 12 /* Band */, 0 /* Subband */));
   qsc_nodes.push_back( QSCNode ( TECHNOLOGY_LTE, SEQ_CAL_UNCONNECTED, 0 /* Instance */, 66 /* Band */, 0 /* Subband */));

   return true;
}
unsigned char RFCal::QSC::UserConfig::DeviceConfig (  HANDLE hPhone, QSCParams & qscParams )
{
   // Read equipconfig.xml, if failed, manually assigned
   if ( ! RFCal::QSC::Read::EquivConfigXML ( hPhone, qscParams ) )
   {
 #if 0
      qscParams.DeviceConfig.type = CMW; // QSCPI;
 #else
      qscParams.DeviceConfig.type = QSCPI; // QSCPI;
 #endif
      qscParams.DeviceConfig.boardId = 0;
      qscParams.DeviceConfig.primaryAddr = 20;   
   }
#ifdef QLIB_DEMO_EXT
   qscParams.DeviceConfig.type = QSCPI; // QSCPI; // Force QSCPI
#endif
   qscParams.DeviceConfig.similation_mode = false;

   if ( qscParams.DeviceConfig.type != QSCPI &&
       qscParams.DeviceConfig.type != CMW )
   {
      qscParams.DeviceConfig.matrix_mode = true;
   }
   else 
   {
      qscParams.DeviceConfig.matrix_mode = false;
   }
   return true;
}
unsigned char RFCal::QSC::UserConfig::XOCalConfig (  HANDLE hPhone, QSCParams & qscParams )
{
   CXOCal_Config & param = qscParams.XOCalConfig;

   param.tech = TECHNOLOGY_CDMA;
   param.band = 0; // BC0
   param.subband = 0;
   param.channel = 358;

   RFCal::XOCal::AssignParameters ( NULL, param.tech, param.band, param.channel, param.ftm_xo_cal_params ); 
   return true;
}
unsigned char RFCal::QSC::UserConfig::GenerateQSCNodes( HANDLE hPhone, const QSCParams & qscParams, const QSCUserInput & userInput, std::vector < QSCNode > & qsc_nodes, unsigned int cal_type_mask )
{
   vector < QSCNode > connetedCal;
   vector < QSCNode > unconnetedCal;
   vector < QSCNode > qsc_find_itr;
   const vector < CTechBand > & techBands = userInput.inputTechBand;
   for ( std::vector < CTechBand >::const_iterator user_itr = techBands.begin(); user_itr != techBands.end(); user_itr++ )
   {
      for ( std::map < QSCNode, QSCSwpParams >::const_iterator qsc_node_itr = qscParams.qscParams.begin(); 
         qsc_node_itr !=  qscParams.qscParams.end(); qsc_node_itr++)
      {
         bool bFoundConnectedQSCNode   = ( cal_type_mask & CONNECTED_CAL_FBRX_RX)? false : true;
         bool bFoundConnectedFDRSBNode = ( cal_type_mask & CONNECTED_CAL_RxRSB)? false : true;
         bool bFoundUnConnectedQSCNode = ( cal_type_mask & UNCONNECTED_CAL_TX)? false : true;

         // ************************************* //
         // Find Connected Cal node 
         // ************************************* //
         if ( ( ! bFoundConnectedQSCNode ) 
            && qsc_node_itr->first.sweep_type == SEQ_CAL_CONNECTED
            && qsc_node_itr->first.tech == user_itr->tech )
         {
            for ( vector < CBandInfo >::const_iterator bandItr = qsc_node_itr->second.BandInfo.begin();
               bandItr != qsc_node_itr->second.BandInfo.end(); bandItr++)
            {
               if ( bandItr->band == user_itr->band && bandItr->subBand == user_itr->subBand )
               {
                
                  // ************************************* //
                  // Find Connected Cal node (FBRx | Rx )
                  // ************************************* //
                  // There is some case, same band, but different instance (e.g., LTE B2 PRx and LTE B2 DRx.  LTE B2, LTE2-252-255...
                  if ( (cal_type_mask & CONNECTED_CAL_FBRX_RX)
                     && ( bandItr->calType.Enable_FBRx_Gain || bandItr->calType.Enable_DRx_Gain || bandItr->calType.Enable_PRx_Gain ))
                  {
                  bFoundConnectedQSCNode = true;
                     // to avoid duplicate instance, check it 
                     if ( std::find( connetedCal.begin(), connetedCal.end(),  qsc_node_itr->first ) == connetedCal.end() )
                     {
                  connetedCal.push_back ( qsc_node_itr->first );
            }
         }

                  // ************************************* //
                  // Find Connected Cal node (FDRSB )
                  // ************************************* //
                  if ( ( cal_type_mask & CONNECTED_CAL_RxRSB)
                     && ( bandItr->calType.Enable_PRx_RSB|| bandItr->calType.Enable_DRx_RSB )
                     )
                  {
                     // to avoid duplicate instance, check it 
                     if ( std::find( connetedCal.begin(), connetedCal.end(),  qsc_node_itr->first ) == connetedCal.end() )
                     {
                        connetedCal.push_back ( qsc_node_itr->first ); 
                     }
                  }
               }
            }
         }
         // ************************************* //
         // Find Unconnected Cal - Tx
         // ************************************* //
         if ( ( cal_type_mask & UNCONNECTED_CAL_TX ) 
            && qsc_node_itr->first.sweep_type == SEQ_CAL_UNCONNECTED
            && qsc_node_itr->first.tech == user_itr->tech
            && qsc_node_itr->first.band == user_itr->band
            && qsc_node_itr->first.subBand == user_itr->subBand )
         {
            unconnetedCal.push_back ( qsc_node_itr->first );
         }

      }
   }
   qsc_nodes.reserve ( qsc_nodes.size() + connetedCal.size() + unconnetedCal.size () );
   std::copy ( connetedCal.begin(), connetedCal.end(), back_inserter ( qsc_nodes ));
   std::copy ( unconnetedCal.begin(), unconnetedCal.end(), back_inserter ( qsc_nodes ));

   return true;
}
unsigned char  RFCal::QSC::UserConfig::NotifyFilePaths ( HANDLE hPhone, const QSCParams & qscParams , const QSCUserInput & userInput, bool bXOCal_FTM )
{
   stringstream ss;
  ss << endl << "==============================================================================" << endl;

   ss << "QSC RF Calibration (ESC V5) Demo" << endl;
  ss << "==============================================================================" << endl << endl;
 
   ss << "[Current configuration for XMLs]" << endl;
   ss << "------------------------------------------------------------------------------" << endl;
   ss << "[Param XML   ] " << qscParams.FilePath.stParamXML << endl;
   ss << "[Char XML    ] " << qscParams.FilePath.stCharXML << endl;
   ss <<  "[PA Param XML] " << qscParams.FilePath.stPAParamXML << endl;
   ss <<  "------------------------------------------------------------------------------" << endl;

   ss << endl;
   ss << "[Tech and bands to calibrate]" << endl;
   ss << "------------------------------------------------------------------------------" << endl;
   ss << "XO Cal: ";
   if ( bXOCal_FTM )
      ss << "Enabled" << endl;
   else
      ss << "Disabled" << endl;

   for ( std::vector < CTechBand >::const_iterator itr =  userInput.inputTechBand.begin(); itr != userInput.inputTechBand.end(); itr++ )
   {
      ss << RFCal::Helper::Convert::TechID_To_Str(itr->tech);
      ss << " Band: " <<  itr->band;
      if ( 10 == itr->tech )
      {
         if ( 28 == itr->band || 41 == itr->band )
         {
            char subband = 'A';
            subband += static_cast < char > ( itr->subBand );
            ss << subband;
         }
      }
      ss << endl;
   }
   ss << endl << "Number of bands to calibrate: " << userInput.inputTechBand.size() << endl;
   ss << "------------------------------------------------------------------------------" << endl;

   // Get a key
   ss << endl;
   ss << "Please modify RFCal.cpp, and build the qlibdemo if you would like to change the cal configration above." << endl << endl;
   std::cout << ss.str();
   ss.str("");
   unsigned char bProceed = 0xFF;
   while ( 0xFF == bProceed)
   {
      std::cout << "Do you continue QSC RF calibration ? (y: Continue, n: Quit)" << endl;   
      char cKey = toupper( getch() );
      switch (cKey)
      {
      case 'Y':
      case 'y':
         bProceed = 1;
         break;
      case 'N':
      case 'n':
      case 'Q':
      case 'q':
         bProceed = 0;
         break;   
      }
   }
   return bProceed;
}
unsigned char RFCal::QSC::UserConfig::CheckFilesExists ( HANDLE hPhone, const QSCParams & qscParams )
 {
    stringstream ss;
    unsigned char status = true;
    if ( access ( qscParams.FilePath.stCalDBXML.c_str(), 0 ) != 0 )
    {
       ss << "CalDB file: " << qscParams.FilePath.stCalDBXML << " is missing";
       RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str());
       status = false;
    }

    if ( access ( qscParams.FilePath.stParamXML.c_str(), 0 ) != 0 )
    {
       ss << "Param XML file: " << qscParams.FilePath.stParamXML << " is missing";
       RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str());
       status = false;
    }

    if ( access ( qscParams.FilePath.stPAParamXML.c_str(), 0 ) != 0 )
    {
       ss << "PA Param XML file: " << qscParams.FilePath.stPAParamXML << " is missing";
       RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str());
       status = false;
    }

    if ( access ( qscParams.FilePath.stCharXML.c_str(), 0 ) != 0 )
    {
       ss << "Char XML file: " << qscParams.FilePath.stCharXML << " is missing";
       RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str());
       status = false;
    }

    RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "==============================================================================" );
    RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "[Param XML   ]" + qscParams.FilePath.stParamXML);
    RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "[Char XML    ]" + qscParams.FilePath.stCharXML);
    RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "[PA Param XML]" + qscParams.FilePath.stPAParamXML);
    RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "[Equipment   ]" + qscParams.FilePath.stEquipConfigXML);
    RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "[Path loss   ]" + qscParams.FilePath.stCalibratedXQCN);
    RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, "==============================================================================" );
    
    return status;
 }
unsigned char RFCal::QSC::Log::RxGainCal::OutputResults  ( HANDLE hPhone, const QSCSwpParams & swpParams,   
               const std::map < QSCNode, std::vector < std::pair < CRFM_Path, seq_cal_rx_cal_path_res_type > > > & rxResults, 
               const std::map < QSCNode, int > & rxNVIDResults)
{
   stringstream ss;
   const int WIDTH = 15;
   ss << endl;
   ss << "===================================================" << endl;
   ss << "Rx Gain Cal Result " << endl;
   ss << "===================================================" << endl << endl;

   for ( map < QSCNode, std::vector< std::pair < CRFM_Path, seq_cal_rx_cal_path_res_type > > >::const_iterator band_it =  rxResults.begin(); band_it != rxResults.end(); band_it++)
   {
      // ************************************//
      // Rx NVID
      // ************************************//

      std::map < QSCNode, int >::const_iterator nvid_itr = rxNVIDResults.find (  band_it->first );
      int nvid = 0;
      if ( rxNVIDResults.end() != nvid_itr )
      {
         nvid = ( nvid_itr->second );
      }
      CCal_Status cal_status;
      if (  0 != nvid  )
         cal_status.status = RFCal::QSC::Cal_Status_DB::SUCCESS;
      else
         cal_status.status = RFCal::QSC::Cal_Status_DB::FAILURE;

      cal_status.nvid = nvid;
      cal_status.cal_time = 0;// Cal time, not nv writing time.

      // Register NV ID result
      swpParams.repoPtr->SetCalStatus ( hPhone, band_it->first , SEQ_CAL_RESULT_TYPE_RXGAIN, cal_status, RFCal::QSC::Cal_Status_DB::NVID );

      ss << "---------------------------------------------------" << endl;
         ss << "Tech: " << band_it->first.tech; 
         ss << ", Band: " << band_it->first.band; 
         ss << ", Subband: " << band_it->first.subBand << endl;
         ss << "NVID: " << nvid << endl;

      ss << "---------------------------------------------------" << endl;

      ss << endl << endl;

      vector < CBandInfo >::const_iterator bandParamIt  = swpParams.BandInfo.begin();
      for ( ; bandParamIt != swpParams.BandInfo.end(); bandParamIt++ )
      {
         if ( ( bandParamIt->band == band_it->first.band)
            && ( bandParamIt->subBand == band_it->first.subBand) )
         {
            break;
         }
      }
      if ( bandParamIt == swpParams.BandInfo.end() )
      {
         continue; // We should not come here
      }

      for (  std::vector< std::pair < CRFM_Path, seq_cal_rx_cal_path_res_type > >::const_iterator it = band_it->second.begin(); it != band_it->second.end(); it++)
      {
         string stPathType = "";
         if ( PATH_TYPE_PRX == swpParams.repoPtr->rfmPathDB.GetPathType( hPhone, 
            band_it->first.tech, band_it->first.band, band_it->first.subBand, it->first ))
            stPathType = "PRX";
         else
            stPathType = "DRX";

         ss << "---------------------------------------------------" << endl;

         ss << "RFM Path ( signal path: " << it->first.signal_path;
         ss << ", rfm_dev: " << it->first.rfm_dev;
         ss << ", antenna port: " << it->first.antenna_port << ")" << endl;
         ss << "PathType: " << stPathType << endl;
         string stCalSuccess = "";
         if ( 0 == it->second.error_debug_mask )
            stCalSuccess = " Success";
         else
            stCalSuccess = " Failure";
        
         ss << "Cal Result: " << stCalSuccess << endl;
         ss << "---------------------------------------------------" << endl << endl;


         const seq_cal_rx_cal_path_res_type & path = it->second;
         ss << std::setw(WIDTH) << "Channel" ;//<<

         ss << std::setw(WIDTH) << "LNA" ;//<<
         ss << std::setw(WIDTH) << "RxAgc Offset" ;//<<
         ss << std::setw(WIDTH) << "RxAgc Min" ;//<<
         ss << std::setw(WIDTH) << "RxAgc Max" ;//<<

         ss << endl;
         for ( uint8 chIdx = 0; chIdx < path.standalone_lna_result.number_of_valid_channels; chIdx++)
         {
            for ( uint16 idx = 0 ; idx < 8; idx++)
            {
               if ((1 << idx) & path.lna_state_mask)
               {
                  ss << std::setw(WIDTH) << static_cast <int> (path.standalone_lna_result.channel_list[chIdx]);

                  ss << std::setw(WIDTH) << static_cast <int> (idx);
                  ss << std::setw(WIDTH) << static_cast <int> (path.standalone_lna_result.freq_comp_offset[idx][chIdx]);
                  ss << std::setw(WIDTH) << static_cast <int> (bandParamIt->rxGainParams.Gain_Limit_Min[idx]);
                  ss << std::setw(WIDTH) << static_cast <int> (bandParamIt->rxGainParams.Gain_Limit_Max[idx]);
                  ss << endl;
               }
            }
         }
         ss << endl;
      }
   }
   RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str() );
   return true;
}
unsigned char RFCal::QSC::Log::RxRSBCal::OutputResults  ( HANDLE hPhone, const QSCSwpParams & swpParams,   
               const std::map < QSCNode,  std::map < CRFM_Path, std::pair < uint64,  ftm_rx_rsb_cal_data > > > & rxrsbResults,  
               const std::map < QSCNode, int > & rxrsbNVIDResults)
{
   stringstream ss;
   const int WIDTH = 8;
   ss << endl;
   ss << "===================================================" << endl;
   ss << "RxRSB Cal Result " << endl;
   ss << "===================================================" << endl << endl;
   unsigned char bOK = true;
   for ( map < QSCNode, std::map < CRFM_Path, std::pair < uint64, ftm_rx_rsb_cal_data > > >::const_iterator band_it =  rxrsbResults.begin(); band_it != rxrsbResults.end(); band_it++)
   {

      ss << "---------------------------------------------------" << endl;
         ss << "Tech: " << band_it->first.tech; 
         ss << ", Band: " << band_it->first.band; 
         ss << ", Subband: " << band_it->first.subBand << endl;
      //   ss << "NVID: " << nvid << endl;

      ss << "---------------------------------------------------" << endl;

      ss << endl << endl;

      vector < CBandInfo >::const_iterator bandParamIt  = swpParams.BandInfo.begin();
      for ( ; bandParamIt != swpParams.BandInfo.end(); bandParamIt++ )
      {
         if ( ( bandParamIt->band == band_it->first.band)
            && ( bandParamIt->subBand == band_it->first.subBand) )
         {
            break;
         }
      }
      if ( bandParamIt == swpParams.BandInfo.end() )
      {
         continue; // We should not come here
      }

      for ( std::map < CRFM_Path, std::pair < uint64, ftm_rx_rsb_cal_data > >::const_iterator it = band_it->second.begin(); it != band_it->second.end(); it++)
      {
         string stPathType = "";
         if ( PATH_TYPE_PRX == swpParams.repoPtr->rfmPathDB.GetPathType( hPhone, 
            band_it->first.tech, band_it->first.band, band_it->first.subBand, it->first ))
            stPathType = "PRX";
         else
            stPathType = "DRX";

         ss << "---------------------------------------------------" << endl;

         ss << "RFM Path ( signal path: " << it->first.signal_path;
         ss << ", rfm_dev: " << it->first.rfm_dev;
         ss << ", antenna port: " << it->first.antenna_port << ")" << endl;
         ss << "PathType: " << stPathType << endl;
         string stCalSuccess = "";
         if ( 0 == it->second.first )
            stCalSuccess = " Success";
         else
         {
            bOK &= false;
            stCalSuccess = " Failure";
         }
         ss << "Cal Result: " << stCalSuccess << endl;
         ss << "---------------------------------------------------" << endl << endl;


         const ftm_rx_rsb_cal_data & path = it->second.second;
         if ( path.numBWs > 0 )
         {
            ss << std::setw(WIDTH) << "Chan" ;//<<

            ss << std::setw(WIDTH) << "BWType" ;//<<
            ss << std::setw(WIDTH) << "Offset" ;//<<
         ss << std::setw(WIDTH) << "BBR" ;//<<
         ss << std::setw(WIDTH) << "BBI" ;//<<
            if ( path.bwData[0].debugDataMask & ( 1 << 0 ))
            {
               ss << std::setw(WIDTH) << "preCal" ;//<<
               ss << std::setw(WIDTH) << "preMAX" ;//<<      
            }
            if ( path.bwData[0].debugDataMask & ( 1 << 1 ))
            {
               ss << std::setw(WIDTH) << "postCal" ;//<<
               ss << std::setw(WIDTH) << "postMax" ;//<<      

            }

         }
         ss << endl;
         for ( uint8 bwIdx = 0; bwIdx < path.numBWs; bwIdx++)
         {
           
            const ftm_fd_rx_rsb_bw_data & bw_data =  path.bwData[bwIdx];
            for ( uint8 j = 0; j < bw_data.numLOFreqOffsets; j++)
            {
               ss << std::setw(WIDTH) << static_cast <int> (path.channel);
               
               ss << std::setw(WIDTH) << static_cast <int> (bw_data.bwType);
               ss << std::setw(WIDTH) << static_cast <double> (bw_data.loFreqOffset[j]/1000000);
               ss << std::setw(WIDTH) << static_cast <int> (bw_data.realCoeff[j]);
               ss << std::setw(WIDTH) << static_cast <int> (bw_data.imagCoeff[j]);
               if ( path.bwData[0].debugDataMask & ( 1 << 0 ))
               {
               ss << std::setw(WIDTH) << static_cast <double> (bw_data.preCalRSB_dB10[j]/10.0);
                  ss << std::setw(WIDTH) << static_cast <double> (bandParamIt->rsbParams.Pre_Cal_RSB_Max_Lim_dB/10.0);
                  if(bw_data.preCalRSB_dB10[j]/10.0 > bandParamIt->rsbParams.Pre_Cal_RSB_Max_Lim_dB/10.0)
                  {
                     ss << " PreRSB Limit failure";
                  }
               }
               if ( path.bwData[0].debugDataMask & ( 1 << 1 ))
               {
                   ss << std::setw(WIDTH) << static_cast <double> (bw_data.postCalRSB_dB10[j]/10.0);
                   ss << std::setw(WIDTH) << static_cast <double> (bandParamIt->rsbParams.Post_Cal_RSB_Max_Lim_dB/10.0);
                  if(bw_data.postCalRSB_dB10[j]/10.0 > bandParamIt->rsbParams.Post_Cal_RSB_Max_Lim_dB/10.0)
                  {
                     ss << " PostRSB Limit failure";
                  }

               }

               ss << endl;
            }
         }
         ss << endl;
      }
   }
   RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str() );

   return bOK;
}

unsigned char RFCal::QSC::Log::XOCal::OutputResults  ( HANDLE hPhone, const QSCSwpParams & swpParams,    
				const std::map < QSCNode,  map < pair <int, int>, ftm_xo_cal_default_path_data > > & xoResults)
{
	stringstream ss;
	const int WIDTH = 8;
	ss << endl;
	ss << "===================================================" << endl;
	ss << "XO Cal Result " << endl;
	ss << "===================================================" << endl << endl;
	unsigned char bOK = true;


	
	for ( std::map < QSCNode,  map < pair <int, int>, ftm_xo_cal_default_path_data > >:: const_iterator band_it = xoResults.begin(); band_it != xoResults.end(); band_it ++ )
	{
		ss << "--------- Parameters: XO Calibration ---------" << endl << endl;
		ss << "Tech: " << band_it ->first.tech;
		ss << ", Band: " << band_it->first.band ; 
		ss << ", Subband: " << static_cast<uint16> (band_it->first.subBand);
		ss << ", Signal_Path: " << band_it->second.begin()->second.rfmPath.sigPath;
		ss << ", RFM Deivce: " << band_it->second.begin()->second.rfmPath.rfmDevice;
		ss << ", Antenna Switch Path: " << band_it->second.begin()->second.rfmPath.antennaSwitchPath << endl;

		ss << endl << "--------- Measurements: XO Calibration ---------" << endl << endl;
		ss << "DL Level (dBm): " << band_it->second.begin()->first.first; 
		ss << ", LNA Gain State: " << band_it->second.begin()->first.second << endl;
		ss << "CalCoeffC0: " << band_it->second.begin()->second.xoCalData.cal_coef.c0;
		ss << ", CalCoeffC1: " << band_it->second.begin()->second.xoCalData.cal_coef.c1;
		ss << ", CalCoeffC2: " << band_it->second.begin()->second.xoCalData.cal_coef.c2;
		ss << ", CalCoeffC3: " << band_it->second.begin()->second.xoCalData.cal_coef.c3;
		ss << ", FTQualInd: " << static_cast <int> (band_it->second.begin()->second.xoCalData.cal_coef.ft_qual_ind);
		ss << ", PostDCXOTrim: " << static_cast <int> (band_it->second.begin()->second.xoCalData.xo_cal_info_pkt.dc_xo_trim) << endl;
		ss << "XOCal_Operating_Therm: " << band_it->second.begin()->second.xoCalData.cal_coef.t0p;
		ss << ", XOTempInit: " << band_it->second.begin()->second.xoCalData.xo_cal_info_pkt.init_temp.xo;
      if ( ! swpParams.BandInfo.empty () )
      {
		   ss << ", XOTempInit Max: " << swpParams.BandInfo.begin()->xoParams.max_start_temperature * 1024;
		   ss << ", XOTempFinal Max: " << swpParams.BandInfo.begin()->xoParams.max_final_temperature * 1024;
      }
      ss << endl << endl;
		

		RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str() );

	}
	return bOK;

}
unsigned char RFCal::QSC::Log::IP2Cal::OutputResults ( HANDLE hPhone, const QSCSwpParams & swpParams, 
			const std::map < QSCNode,  rfcommon_ip2_cal_band_data_type > & ip2Results, 			
			const std::map < QSCNode, int > & ip2NVIDResults)
{

	stringstream ss;
	const int WIDTH = 8;
	ss << endl;
	ss << "===================================================" << endl;
	ss << "IP2 Cal Result " << endl;
	ss << "===================================================" << endl << endl;
	unsigned char bOK = true;
	
	for ( map < QSCNode,  rfcommon_ip2_cal_band_data_type >::const_iterator band_it = ip2Results.begin(); band_it != ip2Results.end(); band_it ++ )
	{
		int channel = 0;
		for ( vector<CBandInfo>::const_iterator itr = swpParams.BandInfo.begin(); itr != swpParams.BandInfo.end(); itr++ )
		{
			if ( itr->band == band_it->first.band  && itr->subBand == band_it->first.subBand )
			{
				if (! itr->commonParams.Channel_List.empty() )
				{
					channel = *itr->commonParams.Channel_List.begin();
				}
			}
		}
		

		CCal_Status cal_status;
		if (  0 != band_it->second.nv_id  )
			cal_status.status = RFCal::QSC::Cal_Status_DB::SUCCESS;
		else
			cal_status.status = RFCal::QSC::Cal_Status_DB::FAILURE;

		cal_status.nvid = band_it->second.nv_id;
		cal_status.cal_time = 0;// Cal time, not nv writing time.

		// Register NV ID result
		swpParams.repoPtr->SetCalStatus ( hPhone, band_it->first , SEQ_CAL_RESULT_TYPE_IP2, cal_status, RFCal::QSC::Cal_Status_DB::NVID );



		ss << "--------- Band Info ---------" << endl;
		ss << "Tech: LTE";
		ss << ", Band: " << band_it->first.band ; 
		ss << ", Subband: " << static_cast<uint16> (band_it->first.subBand);
		ss << ", channel: " << channel;
		ss << ", Num Paths: " << static_cast<uint16> (band_it->second.num_valid_ip2_paths);
		ss << " Band Cal Status: " << static_cast<uint16>  (band_it->second.cal_status);
		ss << ", NV ID: " << band_it->second.nv_id << endl;


		for ( int pathIdx = 0; pathIdx < band_it->second.num_valid_ip2_paths ; pathIdx ++)
		{
			const seq_cal_ip2_cal_path_result * path = & ( band_it->second.ip2_data_arr[ pathIdx ]);
			ss << endl << "--------- Path Info ---------" << endl;
			ss << "RFMDev: " << path->pathType.rfmDevice;
			ss << ", SigPath: " << path->pathType.sigPath;
			ss << ", AntSwitchPath: " << path->pathType.antennaSwitchPath;
			ss << ", NumBWs: " << static_cast<uint16> (path->numBWTypes) << endl << endl;

			for (int bwIdx = 0; bwIdx < band_it->second.ip2_data_arr->numBWTypes; bwIdx ++)
			{
				ss << "Bandwidth: " << static_cast<uint16> (path->bwResults[bwIdx].bandwidth) 
					<< ", CalCode_I: " << path->bwResults[bwIdx].calibratedIcode
					<< ", CalCode_Q: " << path->bwResults[bwIdx].calibratedQcode
					<< ", ErrorCode: " << path->bwResults[bwIdx].error_code
					<< ", OIP2 Delta dB10: " << path->bwResults[bwIdx].oip2_delta_db10 << endl;
			}
		}
		
	}

   RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str() );
   
   return bOK;

}
 unsigned char RFCal::QSC::Log::RxRSBCal::StoreResults ( HANDLE hPhone, const QSCSwpParams & swpParams,   
               std::map < QSCNode, std::map < CRFM_Path, std::pair < uint64 ,  ftm_rx_rsb_cal_data > > > & rxrsbResults,  
               std::map < QSCNode, int > & rxrsbNVIDResults)
 {
   for ( vector < qscRecords >::const_iterator qsc_it = swpParams.swps.begin(); qsc_it != swpParams.swps.end(); qsc_it++)
   {
      for ( vector < cal_sweep_record >::const_iterator it = qsc_it->swpRecords.begin(); it != qsc_it->swpRecords.end(); it++)
      {
         QSCNode tmpQscNode(0,0,0);
         std::map < CRFM_Path, pair<uint64, ftm_rx_rsb_cal_data > > tmpPathResult;

         bool bFound = false;
         if ( it->dataRes.seqCalRes && it->dataRes.seqCalRes->result_mask & SEQ_CAL_RESULT_TYPE_FD_RXRSB)
         {
            seq_cal_ftm_fd_rx_rsb_cal_data & result = it->dataRes.seqCalRes->conn_res_data.fdRxRSBdata;
            bFound = true;

            for (uint8 i=0; i < result.numPaths; i++ )
            {
               ftm_fd_rx_rsb_path_data & path_result = result.pathData[i];
               if ( i == 0) 
               {
                  tmpQscNode.tech = static_cast <int> (path_result.tech);
                  tmpQscNode.band = static_cast <int> (path_result.band);
                  tmpQscNode.subBand = static_cast <int> (path_result.subband);
               }

               CRFM_Path rfm_path ( path_result.rfmPath );

               tmpPathResult [ rfm_path ] = make_pair ( path_result.error_debug_mask, path_result.rsbCalData );
            }
         }
         if ( bFound )
         {
            rxrsbResults[ tmpQscNode ] = tmpPathResult;
         }
      }
   }
    return true;
 }

 unsigned char RFCal::QSC::Log::XOCal::StoreResults (HANDLE hPhone, const QSCSwpParams & swpParams,
	 std::map < QSCNode, map < pair <int, int>, ftm_xo_cal_default_path_data > > & xoResults)

 {

	 for ( vector <qscRecords >::const_iterator qsc_it = swpParams.swps.begin(); qsc_it != swpParams.swps.end(); qsc_it ++ )
	 {
		 for ( vector <cal_sweep_record >::const_iterator it = qsc_it->swpRecords.begin(); it != qsc_it->swpRecords.end(); it ++ )
		 {
			 if ( it->dataRes.seqCalRes && it->dataRes.seqCalRes->result_mask & SEQ_CAL_RESULT_TYPE_XO_CAL)
			 {
				 ftm_xo_cal_default_path_data tmpXoResults = it ->dataRes.seqCalRes->conn_res_data.xoCaldata.path_data;
				 QSCNode qscNode;
				 qscNode.tech = tmpXoResults.tech;
				 qscNode.band = tmpXoResults.band;
				 qscNode.subBand = tmpXoResults.subband;

				 std::pair <int,int> rxSetting;
				 if ( !swpParams.BandInfo.empty())
				 {
					 rxSetting = std::make_pair ( swpParams.BandInfo.begin()->xoParams.DL_Level_dBm, swpParams.BandInfo.begin()->xoParams.LNA_Gain_State);
				 }
				 
				 std::map < pair < int, int >, ftm_xo_cal_default_path_data > rxSetting_xoRes;
				 rxSetting_xoRes[rxSetting] = tmpXoResults;

				 xoResults[qscNode] = rxSetting_xoRes;
			 }
		 }
	 }
	 return true;
 }

 unsigned char RFCal::QSC::Log::IP2Cal::StoreResults ( HANDLE hPhone, const QSCSwpParams & swpParams,     
   	    std::map < QSCNode,  rfcommon_ip2_cal_band_data_type > & ip2Results,
	    std::map < QSCNode, int > & ip2NVIDResults)             
 {
	 
   for ( vector < qscRecords >::const_iterator qsc_it = swpParams.swps.begin(); qsc_it != swpParams.swps.end(); qsc_it++)
   {
      for ( vector < cal_sweep_record >::const_iterator it = qsc_it->swpRecords.begin(); it != qsc_it->swpRecords.end(); it++)
      {

		  if ( it->dataRes.seqCalRes && it->dataRes.seqCalRes->result_mask & SEQ_CAL_RESULT_TYPE_IP2)
          {
			 		 
			  rfcommon_ip2_cal_band_data_type *tmpIp2Results = it->dataRes.seqCalRes->unconn_res_data.ip2_cal_band_data;

			  if (tmpIp2Results)
			  {
				  int nvid = static_cast<int>(tmpIp2Results->nv_id);			 
				  QSCNode qscNode;
				  qscNode.tech = static_cast <int>( TECHNOLOGY_LTE);
				  qscNode.band = tmpIp2Results->band;
				  qscNode.subBand = tmpIp2Results->subBand;
				  ip2NVIDResults [ qscNode ] = nvid;
				  ip2Results[ qscNode] = *tmpIp2Results;
			  }
            
         }

      }
   }
    return true;
 }

unsigned char RFCal::QSC::Log::RxGainCal::StoreResults ( HANDLE hPhone, const QSCSwpParams & swpParams,   
            std::map < QSCNode, std::vector < std::pair < CRFM_Path, seq_cal_rx_cal_path_res_type > > > & rxResults, 
            std::map < QSCNode, int > & rxNVIDResults)
{
   for ( vector < qscRecords >::const_iterator qsc_it = swpParams.swps.begin(); qsc_it != swpParams.swps.end(); qsc_it++)
   {
      for ( vector < cal_sweep_record >::const_iterator it = qsc_it->swpRecords.begin(); it != qsc_it->swpRecords.end(); it++)
      {
         QSCNode tmpQscNode(0,0,0);
         bool bFound = false;
         if ( it->dataRes.seqCalRes && it->dataRes.seqCalRes->result_mask & SEQ_CAL_RESULT_TYPE_RXGAIN)
         {
            const seq_cal_rx_cal_res_type & result = (it->dataRes.seqCalRes->conn_res_data.rx_cal_data);
            if ( result.num_valid_rx_paths > 0 )
            {
               std::vector < pair < CRFM_Path, seq_cal_rx_cal_path_res_type > > rfm_path_vector;
               rfm_path_vector.reserve ( result.num_valid_rx_paths );

               for ( uint8 idx = 0; idx < result.num_valid_rx_paths; idx++ )
               {
                  const seq_cal_rx_cal_path_res_type & path_res = result.rx_data_arr [ idx ];
                  
                  if ( idx == 0 )
                  {
                     tmpQscNode.tech = static_cast < int > ( path_res.tech );
                     tmpQscNode.band = static_cast < int > ( path_res.band );
                     tmpQscNode.subBand = static_cast < int > ( path_res.subband );
                  }

                  CRFM_Path rfmPath;
                  rfmPath.rfm_dev = static_cast < int > ( path_res.rfm_device_id );
                  rfmPath.signal_path = static_cast < int > ( path_res.signal_path );
                  rfmPath.antenna_port = static_cast < int > ( path_res.antenna_port );
                  
                  rfm_path_vector.push_back ( make_pair ( rfmPath, path_res ));
               }

               rxResults [ tmpQscNode ] = rfm_path_vector;
               bFound = true;

            }
         }
         if ( it->dataRes.seqCalRes && it->dataRes.seqCalRes->nv_write_mask & SEQ_CAL_RESULT_TYPE_RXGAIN)
         {
            if ( bFound )
            {
               int nvid = static_cast < int > (it->dataRes.seqCalRes->conn_res_data.rx_cal_data.nv_id);
               if (nvid > 0 )
               {
                  rxNVIDResults [ tmpQscNode ] = nvid;
               }
            }
         }
      }
   }

   return true;
}
unsigned char RFCal::QSC::Log::TxCal::Output::XptDebugInfo::Mline_Slope ( HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, 
   const CXPT_Params & xptParams, const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, stringstream & ss)
{
   const int WIDTH = 12;
   ss << endl;
   ss << endl << "-----------------------------------------";
   ss << endl << "Xpt debug info - MLine Slope- " << GetXptModeName ( xpt_results.xpt_mode );
   ss << endl << "-----------------------------------------" << endl << endl;

   const ftm_calibration_mline_result_slope_type & slopeRes = xpt_debug_info.mline_res.mline_slope_result;

   ss << setw (WIDTH) << "Vout_Vin_a:" << setw (WIDTH) << slopeRes.vout_vin_a << endl;
   ss << setw (WIDTH) << "Vout_Vin_b:" << setw (WIDTH) << slopeRes.vout_vin_b << endl;

   ss << setw (WIDTH) << "Vcc_vin_d:" << setw (WIDTH) << slopeRes.vcc_vin_d << endl;
   ss << setw (WIDTH) << "Vcc_vin_e:" << setw (WIDTH) << slopeRes.vcc_vin_e << endl;

   ss << endl;
   return true;
}
unsigned char RFCal::QSC::Log::TxCal::Output::XptDebugInfo::Mline_SelectSwp (HANDLE hPhone, 
   const QSCNode & qscNode, QSCSwpParams & swpParams, const CXPT_Params & xptParams, const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, stringstream & ss, const int mode /* 0: calc, 1: meas */)
{
   const int WIDTH = 12;
   ss << endl;
   ss << endl << "-----------------------------------------";
   if ( 0 == mode )
      ss << endl << "Xpt debug info - Mline_SelectSwp Calc - " << GetXptModeName ( xpt_results.xpt_mode );
   else
      ss << endl << "Xpt debug info - Mline_SelectSwp Meas- " << GetXptModeName ( xpt_results.xpt_mode );

   ss << endl << "-----------------------------------------" << endl << endl;

   const ftm_calibration_mline_result_selec_sweep_type * result = ( 0 == mode )?
         & xpt_debug_info.mline_res.selec_swp_calc_result : & xpt_debug_info.mline_res.selec_swp_meas_result;

   ss << endl <<"Num of Rows:" << static_cast<int>(result->num_selec_swp_pwrs) << endl;

   ss<<"-------------------------------------------------"<<endl;
   ss<<left<<setw(5)<<"Idx"<<setw(7)<<"Valid"<<setw(5)<<"RGI"
      <<setw(6)<<"Bias"<<setw(10)<<"IqOffset"<<setw(7)<<"Power"<<endl;
   ss<<"-------------------------------------------------"<<endl;
   
   const CBandInfo & bandInfo = swpParams.BandInfo.back();

   CXptInput_V1 xpt_pa_param;
   if ( ! swpParams.repoPtr->GetPaParamData (bandInfo.pa_param_key , swpParams.tech, bandInfo.band, bandInfo.subBand, xpt_results.BW, xpt_pa_param ) )
   {
      return false;
   }

   uint32 vdMax = static_cast<uint32>( ( XPT_TYPE_EPT == xpt_results.xpt_mode )? 
      xpt_pa_param.MLineMaxPaSupplyEpt*1000.0 : xpt_pa_param.MLineMaxPaSupplyEt*1000.0 );  

   for (uint8 i=0; i < result->num_selec_swp_pwrs; i++)
   {
      if (result->mline_selec_swp[i].selec_swp_bias > vdMax)
      {
         ss << "XPT Mode: " << GetXptModeName( xpt_results.xpt_mode ) << " For Target Power:"<<static_cast<short>( result->mline_selec_swp[i].selec_swp_pwr)/100.0 <<"dBm,"
            "The bias picked from MLINE table exceeds Maximum PA Bias of "<< (double(vdMax)/1000.0) <<"V" <<std::endl;
      } 

      ss<<left
         <<setw(5)<<static_cast<int>(i)
         <<setw(7)<<static_cast<int>(result->mline_selec_swp[i].selec_swp_valid)
         <<setw(5)<<static_cast<int>(result->mline_selec_swp[i].selec_swp_rgi)
         <<setw(6)<<static_cast<int>(result->mline_selec_swp[i].selec_swp_bias)
         <<setw(10)<<static_cast<int>(result->mline_selec_swp[i].selec_swp_iq_offset)
         <<setw(7)<<static_cast<int>(result->mline_selec_swp[i].selec_swp_pwr)/100.0
         <<endl;

      if (result->mline_selec_swp[i].selec_swp_valid != 1)
      {
         ss    << "Xpt Cal Failure: Selection Swp Valid Flag is not 1. " 
            << "Row: "   << static_cast<int>(i)
            << ", RGI: " << static_cast<int>(result->mline_selec_swp[i].selec_swp_rgi) 
            << ", Bias: " << static_cast<int>(result->mline_selec_swp[i].selec_swp_bias) 
            << ", IQ Offset: " << static_cast<int>(result->mline_selec_swp[i].selec_swp_iq_offset) 
            << ", Pwr: " << static_cast<int>(result->mline_selec_swp[i].selec_swp_pwr)/100.0 << endl;
      }
      if (result->mline_selec_swp[i].selec_swp_bias > vdMax)
      {
         ss    << "Xpt Cal Failure: Selection Swp. Bias is over than VdMax. " 
            << "Row: "   << static_cast<int>(i)
            << ", VdMax: "   << vdMax
            << ", RGI: " << static_cast<int>(result->mline_selec_swp[i].selec_swp_rgi) 
            << ", Bias: " << static_cast<int>(result->mline_selec_swp[i].selec_swp_bias) 
            << ", IQ Offset: " << static_cast<int>(result->mline_selec_swp[i].selec_swp_iq_offset) 
            << ", Pwr: " << static_cast<int>(result->mline_selec_swp[i].selec_swp_pwr)/100.0 << endl;
      }
   }
   return true;
}
unsigned char RFCal::QSC::Log::TxCal::Output::XptDebugInfo::Mline_ResultTable ( HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, 
   const CXPT_Params & xptParams, const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, int version,  stringstream & ss)
{
   const int WIDTH = 12;
   ss << endl;
   ss << endl << "-----------------------------------------";
   if ( 2 == version )
      ss << endl << "Xpt debug info - MLine Result Table V2 - " << GetXptModeName ( xpt_results.xpt_mode );
   else
      ss << endl << "Xpt debug info - MLine Result Table - " << GetXptModeName ( xpt_results.xpt_mode );
   
   ss << endl << "-----------------------------------------" << endl << endl;
   
   const CBandInfo & bandInfo = swpParams.BandInfo.back();
   CXptInput_V1 xpt_pa_param;

   if ( ! swpParams.repoPtr->GetPaParamData (bandInfo.pa_param_key , swpParams.tech, bandInfo.band, bandInfo.subBand, xpt_results.BW, xpt_pa_param ) )
   {
      return false;
   }
   uint32 vdMax = static_cast<uint32>( ( XPT_TYPE_EPT == xpt_results.xpt_mode )? 
      xpt_pa_param.MLineMaxPaSupplyEpt*1000.0 : xpt_pa_param.MLineMaxPaSupplyEt*1000.0 );  

   const ftm_calibration_mline_result_table_type & result = xpt_debug_info.mline_res.mline_table_result;
   ss<<"Num of Rows:"<<static_cast<int>(result.num_mline_table_rows)<<endl;

   ss<<"-------------------------------------------------"<<endl;
   ss<<"MLineResult - Result Table : "<<endl;
   ss<<left<<setw(5)<<"Idx"<<setw(5)<<"RGI"<<setw(7)<<"Bias"
      <<setw(10)<<"IqOffset"<<setw(7)<<"Power";
   if ( 2 == version )
   {
      ss <<setw(7)<<"Pin";
   }
   ss <<setw(11)<<"DebugMask"<<endl;
   ss<<"-------------------------------------------------"<<endl;
   bool xptMaxFound = false;
   unsigned short biasIndex = 0;
   double maxXptPwr =  -999;
   for (uint8 i=0; i < result.num_mline_table_rows ; i++)
   {
      // find the threshold where M-line bias is greater than the given xPT max bias
      if( result.mline_results[i].bias_out > vdMax )
      {
         if( xptMaxFound != true && i > 0 )
         {
            biasIndex = i;
            xptMaxFound = true;
         }
      }

      ss<<left
         <<setw(5)<<static_cast<int>(i)
         <<setw(5)<<static_cast<int>(result.mline_results[i].rgi)
         <<setw(7)<<static_cast<int>(result.mline_results[i].bias_out)
         <<setw(10)<<static_cast<int>(result.mline_results[i].iq_offset_db10)
         <<setw(7)<<static_cast<double>(static_cast<int>(result.mline_results[i].pow_out_db10))/100.0;
      if ( 2 == version )
      {
         ss <<setw(7)<<static_cast<double>(static_cast<int>(result.mline_results[i].pin_peak_db10))/10.0;
      }
      ss <<setw(11)<<static_cast<int>(result.mline_results[i].dbg_mask)	
         <<endl;
   }

   ss << endl 
      << "-------------------------------------------------"<<endl;
   ss << "Max M Line Power for " << GetXptModeName ( xpt_results.xpt_mode ) << endl;
   ss<< "-------------------------------------------------"<<endl;
 
   // find the maximum possible M-Line Power for the given max bias. using linear interpolation
   int16 targetPwr = -999;
   if( xptMaxFound == false )
   {
    
      ss<<endl<<"Could not determine the maximum M-line power for vd_max = " << vdMax;
   }
   else
   {
      ss<<endl<<"Calculating the maximum target power based on the M-line table and vd_max = " << vdMax << " ...";
      double pwr0 = result.mline_results[biasIndex-1].pow_out_db10 / 100.0;
      double pwr1 = result.mline_results[biasIndex].pow_out_db10 / 100.0;
      long bias0 = result.mline_results[biasIndex-1].bias_out;
      long bias1 = result.mline_results[biasIndex].bias_out;
      // long biasLimit = addSwpParams.xptSwp2Params.m_swp2MLineEPTVdMax;
      maxXptPwr = ( pwr0 + ((( pwr1 - pwr0 ) * ( vdMax - bias0 )) / ( bias1 - bias0 )));

      /* Find Target Power */
      const vector <double> * PwrList;
      if (XPT_TYPE_EPT == xpt_results.xpt_mode )
         PwrList = &(swpParams.BandInfo.back().xptParams.EPT_Params.Meas_Pwr_List );
      else
         PwrList = &(swpParams.BandInfo.back().xptParams.ET_Params.Meas_Pwr_List );

      for (uint32 i = 0; i < PwrList->size(); i++)
      {
         int16 pwr = static_cast<int16>(PwrList->at(i) * 100.0);
         if (pwr > targetPwr)
            targetPwr = pwr;
      }

      ss <<endl<<"Maximum possible " << GetXptModeName ( xpt_results.xpt_mode )  << " target power = " << maxXptPwr;
      ss <<endl<<"Target Power (from Selection Swp Meas Result) = " <<  static_cast<double>(targetPwr)/100.0;
      double PwrMargin = maxXptPwr - static_cast<double>(targetPwr)/100.0;
      ss <<endl<<"XPTPowerMargin (dB) = " << PwrMargin;
   }


   // Log EPT Max M Line power
   //if( xptMaxFound == true )
   {
      ss << endl;
      ss << "MEASUREMENTS: Tx XPT Swp2 - Max M Line Power for " + GetXptModeName ( xpt_results.xpt_mode ) ;

      ss << "Max Vcc (mV):" << vdMax << endl;
      if( xptMaxFound == true )
      {
         ss << " Swp2 Max MLine Power (dBm):" <<  maxXptPwr << endl;

         ss << "Max TargetPwr (dBm):" << static_cast<double>(targetPwr)/100.0 << endl;
         double PwrMargin = maxXptPwr - static_cast<double>(targetPwr)/100.0;
         ss << "XPTPowerMargin (dB):" << PwrMargin << endl;
      }
      else
      {
         ss << "Could not determine the maximum M-line power for vd_max = " << vdMax << endl;     
      }
   }

   ss << endl;

   return true;
}

unsigned char RFCal::QSC::Log::TxCal::Output::XptDebugInfo::Mline_EtDelay ( HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, 
   const CXPT_Params & xptParams, const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, stringstream & ss)
{
   const int WIDTH = 12;
   ss << endl;
   ss << endl << "-----------------------------------------";
   ss << endl << "Xpt debug info - MLine Delay- " << GetXptModeName ( xpt_results.xpt_mode );
   ss << endl << "-----------------------------------------" << endl << endl;

   ss << setw (WIDTH) << "Delay:";
   ss << setw (WIDTH) << xpt_debug_info.et_delay;

   ss << endl;

   return true;
}
unsigned char RFCal::QSC::Log::TxCal::Output::XptDebugInfo::Mline_DebugMask ( HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, 
   const CXPT_Params & xptParams, const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, stringstream & ss)
{
   const int WIDTH = 12;
   ss << endl;
   ss << endl << "-----------------------------------------";
   ss << endl << "Xpt debug info - Debug mask - " << GetXptModeName ( xpt_results.xpt_mode );
   ss << endl << "-----------------------------------------" << endl << endl;

   uint32 debug_mask = xpt_debug_info.mline_res.mline_debug_mask;
   string debug_str = "";
   if (debug_mask == 0)
   {
      debug_str = "Success";
   }
   else
   {
      map <uint32, string> mapDebugMaskStr;

      mapDebugMaskStr[0] = "Did not achieve expected compression for M Line. ";
      mapDebugMaskStr[1] = "Did not find a power in the M Line table greater than requested power for measured powers. ";
      mapDebugMaskStr[2] = "Did not find a power in the M Line table less than requested power  for measured powers. ";
      mapDebugMaskStr[3] = "Selected Bias > Vdmax, but interpolated Bias < Vmax for measured powers. ";
      mapDebugMaskStr[4] = "Selected Bias > Vdmax, and interpolated Bias > Vmax for measured powers. ";
      mapDebugMaskStr[5] = "Did not find a power in the M Line table greater than requested power for calculated powers. ";
      mapDebugMaskStr[6] = "Did not find a power in the M Line table less than requested power for calculated powers. ";
      mapDebugMaskStr[7] = "Selected Bias > Vdmax, but interpolated Bias < Vmax for calculated powers. ";
      mapDebugMaskStr[8] = "Selected Bias > Vdmax, and interpolated Bias > Vmax for calculated powers. ";
      mapDebugMaskStr[9] = "Progressively decreasing Bias condition in the M Line table. ";

   
      for (uint32 i = 0 ; i < mapDebugMaskStr.size(); i++)
      {
         if ( (1 << i) & debug_mask)
         {
            debug_str += mapDebugMaskStr[i];
         }
      }
   }
   ss << setw (WIDTH) << "DebugMask :" << setw (WIDTH) << debug_mask << endl;
   ss << debug_str ;

   ss << endl;

   return true;
}

unsigned char RFCal::QSC::Log::TxCal::Output::XptDebugInfo::Mline_CompPts ( HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, 
   const CXPT_Params & xptParams, const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, stringstream & ss)
{
   const int WIDTH = 12;
   ss << endl;
   ss << endl << "-----------------------------------------";
   ss << endl << "Xpt debug info - Mline CompPts - " << GetXptModeName ( xpt_results.xpt_mode );
   ss << endl << "-----------------------------------------" << endl << endl;

   const ftm_calibration_mline_result_compression_point_type & compRes = xpt_debug_info.mline_res.compression_point_result;

   if (  compRes.num_compression_points > 0 
      && compRes.num_compression_points < sizeof ( compRes.compression_point_list )/sizeof( uint8 ) )
   {

      for (uint8 i=0; i < xpt_debug_info.stg1_powers.num_meas; i++)
      {
         ss << setw ( WIDTH ) << static_cast < double> ( 
               static_cast < char > ( compRes.compression_point_list [i] ) 
               ) / 10.0 << endl;
      }
   }
   ss << endl;
   return true;
}

unsigned char RFCal::QSC::Log::TxCal::Output::XptDebugInfo::Stg1Pwr ( HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, 
   const CXPT_Params & xptParams,const CXPT_Results & xpt_results, const xpt_dbg_type & xpt_debug_info, stringstream & ss)
{
   const int WIDTH = 12;
   ss << endl;
   ss << endl << "-----------------------------------------";
   ss << endl << "Xpt debug info - Stg1Pwr - " << GetXptModeName ( xpt_results.xpt_mode );
   ss << endl << "-----------------------------------------" << endl << endl;

   if (  xpt_debug_info.stg1_powers.num_meas > 0 
      && xpt_debug_info.stg1_powers.num_meas < sizeof (xpt_debug_info.stg1_powers.pwr)/sizeof(int) )
   {
      ss << setw ( WIDTH ) << "RGI";
      ss << setw ( WIDTH ) << "Bias";
      ss << setw ( WIDTH ) << "ExpPw";
      ss << setw ( WIDTH ) << "Pwr" << endl;

      for (uint8 i=0; i < xpt_debug_info.stg1_powers.num_meas; i++)
      {
         ss << setw ( WIDTH ) << xptParams.RgiList [i];
         ss << setw ( WIDTH ) << xptParams.BiasList [i];
         ss << setw ( WIDTH ) << xptParams.Exp_Pwrs [i];
         ss << setw ( WIDTH ) << static_cast < double> ( xpt_debug_info.stg1_powers.pwr [i] ) / 10.0 << endl;
      }
   }
   ss << endl;
   return true;
}
string RFCal::QSC::Log::TxCal::Output::XptDebugInfo::GetXptModeName ( const int xpt_mode )
{
   switch ( xpt_mode)
   {
   case XPT_TYPE_EPT:
      return "EPT";
      break;
   case XPT_TYPE_ET:
      return "ET";
      break;
   default:
      break;
   }
   return "";
}

unsigned char RFCal::QSC::Log::TxCal::Output::XptDebugInfoResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, const xptDebugInfoType & xpt_debug_info, const int xpt_mode_type, const int bw, stringstream & ss)
{
   for ( xptDebugInfoType::const_iterator itr = xpt_debug_info.begin(); itr != xpt_debug_info.end(); itr++)
   {
      if ( itr->first.xpt_mode != xpt_mode_type || itr->first.BW != bw ) continue;

      CXPT_Params * xptParams = ( XPT_TYPE_EPT == xpt_mode_type)? & ( swpParams.BandInfo.back().xptParams.EPT_Params ) : & ( swpParams.BandInfo.back().xptParams.ET_Params );

   const int WIDTH = 12;
   ss << endl;
   ss << endl << "-----------------------------------------";
      ss << endl << "Xpt debug info ";
      if ( TECHNOLOGY_LTE ==qscNode.tech )
      {
         if ( 10 == bw)
		      ss << std::setw ( WIDTH ) << "10 Mhz";
         else if (40 == bw)
		      ss << std::setw ( WIDTH ) << "40 Mhz";
      }
   ss << endl << "-----------------------------------------" << endl << endl;


      if ( itr->second.debug_return_mask & ( 1 << XPT_DEBUG_INFO_STG1_PWR ) )
   {
         XptDebugInfo::Stg1Pwr ( hPhone, qscNode, swpParams, ( *xptParams ), itr->first, itr->second, ss );
   }

   // if version 2 is avaialble, then ignore version 1 since it is redundant info.
      if (  itr->second.debug_return_mask & ( 1 << XPT_DEBUG_INFO_MLINE_RESULT_TABLE_V2 ) )
   {
         XptDebugInfo::Mline_ResultTable ( hPhone, qscNode, swpParams, ( *xptParams ), itr->first, itr->second, 2 /* version */, ss );
   } 
      else if ( itr->second.debug_return_mask & ( 1 << XPT_DEBUG_INFO_MLINE_RESULT_TABLE ) )
   {
         XptDebugInfo::Mline_ResultTable ( hPhone, qscNode, swpParams, ( *xptParams ), itr->first, itr->second, 1 /* version */, ss );
   }
   
      if ( itr->second.debug_return_mask & ( 1 << XPT_DEBUG_INFO_MLINE_RESULT_SELECT_SWP_CALC ) )
      {
         XptDebugInfo::Mline_SelectSwp ( hPhone, qscNode, swpParams, ( *xptParams ), itr->first, itr->second, ss, 0 /* Calc */ );
      }
      if ( itr->second.debug_return_mask & ( 1 << XPT_DEBUG_INFO_MLINE_RESULT_SELECT_SWP_MEAS ) )
      {
         XptDebugInfo::Mline_SelectSwp ( hPhone, qscNode, swpParams, ( *xptParams ), itr->first, itr->second, ss, 1 /* Meas */ );
      }
      if ( itr->second.debug_return_mask & ( 1 << XPT_DEBUG_INFO_MLINE_RESULT_SLOPE ) )
      {
         XptDebugInfo::Mline_Slope ( hPhone, qscNode, swpParams, ( *xptParams ),itr->first, itr->second, ss );
      }
      if ( itr->second.debug_return_mask & ( 1 << XPT_DEBUG_INFO_MLINE_RESULT_DEBUG_MASK ) )
      {
         XptDebugInfo::Mline_DebugMask ( hPhone, qscNode, swpParams, ( *xptParams ), itr->first, itr->second, ss );
      }
      if ( itr->second.debug_return_mask & ( 1 << XPT_DEBUG_INFO_MLINE_RESULT_COMP_PTS ) )
      {
         XptDebugInfo::Mline_CompPts ( hPhone, qscNode, swpParams, ( *xptParams ), itr->first, itr->second, ss );
      }
      if ( itr->second.debug_return_mask & ( 1 << XPT_DEBUG_INFO_ET_DELAY ) )
      {
         XptDebugInfo::Mline_EtDelay ( hPhone, qscNode, swpParams, ( *xptParams ), itr->first, itr->second, ss );
   }
   }
   return true;
}
unsigned char RFCal::QSC::Log::TxCalResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams )
{
   int tx_nv_id = 0;
   map < CRFM_Path, CTX_Results > Tx_Results_map;

   TxCal::StoreResults ( hPhone, swpParams, Tx_Results_map, tx_nv_id );

   TxCal::OutputResults ( hPhone, qscNode, swpParams, Tx_Results_map, tx_nv_id );

   return false;
}
unsigned char RFCal::QSC::Log::TxCal::Output::CreatePA_XPT_ModeMap (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, std::map < int, int > & pa_xpt_mode_map )
{
   const CBandInfo & bandInfo = swpParams.BandInfo.back();
  
   for ( vector < int >::const_iterator it = bandInfo.txLinParams.PA_State_List.begin();  it!= bandInfo.txLinParams.PA_State_List.end(); it ++ )
   {
      pa_xpt_mode_map [ (*it) ] = XPT_TYPE_APT;
   }

   if ( bandInfo.calType.Enable_Tx_XPT )
   {
      const vector <int> & xpt_list = bandInfo.xptParams.XPT_Mode_List;
      if ( std::find ( xpt_list.begin(), xpt_list.end(),  XPT_TYPE_EPT ) != xpt_list.end ())
      {
         pa_xpt_mode_map [ bandInfo.xptParams.EPT_Params.PA_State ] = XPT_TYPE_EPT;
      }
      if ( std::find ( xpt_list.begin(), xpt_list.end(),  XPT_TYPE_ET ) != xpt_list.end ())
      {
         pa_xpt_mode_map [ bandInfo.xptParams.ET_Params.PA_State ] = XPT_TYPE_ET;
      }

   }

  return  !(pa_xpt_mode_map.empty());
}
string RFCal::QSC::Log::TxCal::Output::GetCalStatusErrorCode(int cal_status)
{
   map <int, string> errcode_map;
   errcode_map[0] = "Pass";
   errcode_map[1] = "XPT PARAMS INVALID";
   errcode_map[2] = "State machine failure. Wrong state or Wrong event received in a state.";
   errcode_map[3] = "Memory allocation failure ";
   errcode_map[4] = "Status of internal API returned false";
   errcode_map[5] = "RGI Delta not available";
   errcode_map[6] = "STG1 Powers measured failing limit";
   errcode_map[7] = "STG1 Captures failed. No results available";
   errcode_map[8] = "STG1 Processing failed. No results available";
   errcode_map[9] = "STG1 Compression not achieved";
   errcode_map[32] = "Mline failure. Bias picked lower than ET Vdmin";
   errcode_map[33] = "Mline failure. For detail look in mline result packet debug mask.";
   errcode_map[34] = "STG2 Captures failed. No results available";
   errcode_map[35] = "STG2 Processing failed. No results available";
   errcode_map[36] = "STG0 Powers measured failing limit";
   errcode_map[37] = "STG0 Captures failed";
   errcode_map[38] = "STG0 Processing failed";
   errcode_map[39] = "Mline Failure. Negative slope VCC";
   errcode_map[40] = "Mline Failure. Negative slope VIN";
   errcode_map[41] = "STG0 SNR failing min limit";
   errcode_map[42] = "STG1 SNR failing min limit";
   errcode_map[43] = "STG2 SNR failing min limit";
   return errcode_map[cal_status];
}
unsigned char RFCal::QSC::Log::TxCal::Output::Process_Linearlizer_SetXptWaveform (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, const int result_type, const int bw, int & xpt_mode, int & dpdType, int & waveform )
{
   waveform = 3;
   dpdType = TX_LINEARIZER_NO_AUX_DATA; // APT

   switch ( result_type )
   {
      case RFCal::QSC::Log::APT_LINEARLIZER:
         {
            if ( TECHNOLOGY_LTE == qscNode.tech)
               waveform = ( bw == 40 )? 5 : 1;
            else
               waveform = 0;
         }
         break;
      case RFCal::QSC::Log::RGI_DELTA:
          waveform = ( bw == 40 )? 4 : 3;
         break;
      case RFCal::QSC::Log::ALIGNMENT:
           dpdType = (XPT_TYPE_EPT == xpt_mode)? TX_LINEARIZER_EPT_VOLTERRA_DATA : TX_LINEARIZER_ET_VOLTERRA_DATA; 
            if ( TECHNOLOGY_LTE == qscNode.tech )
                waveform = ( bw == 40 )? 5: 1;
            else
               waveform = 0;
          break;
  
      case RFCal::QSC::Log::FULL_BIAS_LINEARLIZER:
           if ( TECHNOLOGY_LTE ==  qscNode.tech )
               waveform = ( bw == 40 )? 4: 3;
      default:
         break;
   }
   return true;
}
unsigned char RFCal::QSC::Log::TxCal::Output::XptCalStatus (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, 
   const XptMode_results_map & xpt_results, const int xpt_mode, stringstream & ss )
{
   const int WIDTH = 20;
   /* ======================================= */
   // Create pa-xpt-mode map for convenience
   /* ======================================= */
  
   map < int, int > pa_xpt_mode_map;
   CreatePA_XPT_ModeMap ( hPhone, qscNode, swpParams, pa_xpt_mode_map);

   XptMode_results_map::const_iterator itr = xpt_results.find ( xpt_mode );
   if ( itr == xpt_results.end() )
      return false;

   for(size_t index=0; index<(itr->second.size()); index++)
   {

   /* ======================================= */
   // XPT Cal Status
   /* ======================================= */
    ss << std::setw ( WIDTH ) << "PA State:" << std::setw ( WIDTH ) << std::setw ( WIDTH ) << static_cast < int > ( itr->second[index].pa_state ) << std::endl;
    ss << std::setw ( WIDTH ) << "XPT Mode:" << std::setw ( WIDTH ) << xpt_mode << std::endl;
    ss << std::setw ( WIDTH ) << "Num DPD:"  << std::setw ( WIDTH ) << static_cast < int > ( itr->second[index].num_dpd ) << std::endl;
	ss <<  std::setw ( WIDTH ) << "Xpt Cal BW:";
	if ( 1 ==  itr->second[index].BW )
		ss << std::setw ( WIDTH ) << "10 Mhz" << endl;
	if ( 5 ==  itr->second[index].BW )
		ss << std::setw ( WIDTH ) << "40 Mhz" << endl;

   ss <<  std::setw ( WIDTH ) << "Xpt Cal Status:";
   if ( 0 ==  itr->second[index].status )
         ss << std::setw ( WIDTH ) << "Success" << endl;
      else
      ss << std::setw ( WIDTH ) << "Failed. Error Code: " << itr->second[index].status << " ( = " << GetCalStatusErrorCode(itr->second[0].status) << " )" << endl;
    ss << std::endl;
   }
   return true;
}

unsigned char RFCal::QSC::Log::TxCal::Output::FreqComp (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, 
            const vector < int > & freqcomp_channels, const PA_fcompOffsets_map & tx_freqcomp_results, const int xpt_type, stringstream & ss)
{
   const int WIDTH = 10;
   /* ======================================= */
   // Create pa-xpt-mode map for convenience
   /* ======================================= */
  
   map < int, int > pa_xpt_mode_map;
   CreatePA_XPT_ModeMap ( hPhone, qscNode, swpParams, pa_xpt_mode_map);

   /* ======================================= */
   // FreqComp Channel
   /* ======================================= */
   bool bFirstTime = true;
   for ( PA_fcompOffsets_map::const_iterator pa_itr = tx_freqcomp_results.begin(); pa_itr != tx_freqcomp_results.end(); pa_itr ++ )
   {
      if ( ( pa_xpt_mode_map.find ( pa_itr->first ) == pa_xpt_mode_map.end() )
         || ( pa_xpt_mode_map [  pa_itr->first ] != xpt_type ) ) // 
      {
         continue;
      }
      
      if ( bFirstTime )
      {
         ss << std::setw ( WIDTH ) << "PA State ";
         ss << std::setw ( WIDTH ) << "FComp Index ";
         ss << std::setw ( WIDTH ) << "Channel ";
         ss << std::setw ( WIDTH ) << "FComp Offset";
         ss << std::endl;

         bFirstTime = false;
      }
      for ( vector < seq_cal_tx_freq_comp_tbl_type >::const_iterator itr = pa_itr->second.begin(); itr != pa_itr->second.end() ;  itr++ )
      {

         for ( vector < int >::const_iterator ch_itr = freqcomp_channels.begin(); ch_itr != freqcomp_channels.end(); ch_itr++ )
         {
            ss << std::setw ( WIDTH ) << static_cast < int > (pa_itr->first );
            ss << std::setw ( WIDTH ) << static_cast < int > (itr->fcomp_idx);
            ss << std::setw ( WIDTH ) << ( * ch_itr );
            ss << std::setw ( WIDTH ) << static_cast < int > ( itr->fcomp[ std::distance ( freqcomp_channels.begin(), ch_itr) ]);
            ss << std::endl;
         }
         ss << endl;
      }
   }

   return true;
}

unsigned char RFCal::QSC::Log::TxCal::Output::Linearlizer (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, int result_type, const int bw, int xpt_mode, const PA_DpdType_Waveform_TxResult_Map & pa_dpd_wavform_tx_results, stringstream & ss)
{
   const int WIDTH = 10;
  
   /* ======================================= */
   // Create pa-xpt-mode map for convenience
   /* ======================================= */
  
   map < int, int > pa_xpt_mode_map;
   CreatePA_XPT_ModeMap ( hPhone, qscNode, swpParams, pa_xpt_mode_map);

   /* ======================================= */
   // Find necessary waveform type and xpt mode for the requested result type
   /* ======================================= */
   int waveform, dpdType; 
   Process_Linearlizer_SetXptWaveform (  hPhone, qscNode, swpParams, result_type, bw, xpt_mode, dpdType, waveform );

   bool bFirstTime = true;
   
   for (  PA_DpdType_Waveform_TxResult_Map::const_reverse_iterator pa_itr = pa_dpd_wavform_tx_results.rbegin(); pa_itr != pa_dpd_wavform_tx_results.rend(); pa_itr++)
   {
      int pa_state = pa_itr->first;

      if (  pa_xpt_mode_map.find ( pa_state ) == pa_xpt_mode_map.end()
         || pa_xpt_mode_map[ pa_state ] != xpt_mode )
      {
         continue;
      }
      
      for ( DpdType_Waveform_TxResult_Map::const_iterator dpd_itr = pa_itr->second.begin(); dpd_itr != pa_itr->second.end(); dpd_itr ++ )
      {
         if ( dpdType != dpd_itr->first )
         {
            continue;
         }

         for ( Waveform_TxResult_Map::const_iterator wv_itr = dpd_itr->second.begin(); wv_itr != dpd_itr->second.end(); wv_itr ++ )
         {
            if ( waveform != wv_itr->first )
               continue;
            
            // Check FBRx Debug Info if available.
            bool isFBRxDebugInfo = false;
            for ( std::vector < CTX_Lin_TxParam_Pwr >::const_iterator it = wv_itr->second.begin(); it != wv_itr->second.end(); it ++ )
            {
               if (it->fbrx_gain_state != 0 || it->fbrx_hw_gain != 0 )
               {
                  isFBRxDebugInfo = true;
                  break;
               }
            }
            
            for ( std::vector < CTX_Lin_TxParam_Pwr >::const_iterator it = wv_itr->second.begin(); it != wv_itr->second.end(); it ++ )
            {
               if (bFirstTime )
               {
                  ss << std::setw ( WIDTH ) <<  "Channel";
				  if ( TECHNOLOGY_LTE == qscNode.tech )
                  {
				    ss << std::setw ( WIDTH ) <<  "BW";
                  }
                  ss << std::setw ( WIDTH ) <<  "PA";
                  ss << std::setw ( WIDTH ) <<  "RGI";
                  ss << std::setw ( WIDTH ) <<  "BIAS";
                  ss << std::setw ( WIDTH ) <<  "ICQ";
                  ss << std::setw ( WIDTH ) <<  "Power"; 
                  if ( isFBRxDebugInfo )
                  {
                     ss << std::setw ( WIDTH ) <<  "FBGS";
                     ss << std::setw ( WIDTH ) <<  "FBHWG";
                  }
                  ss << endl;
                  bFirstTime = false;
               }
               ss << std::setw ( WIDTH ) << it->channel;
			   if ( TECHNOLOGY_LTE == qscNode.tech )
               {
                  ss << std::setw ( WIDTH ) <<  RFCal::QSC::Log::TxCal::GetBWFromWvIndex ( hPhone, static_cast < unsigned char > ( waveform ) );
               }
               ss << std::setw ( WIDTH ) << pa_state;
               ss << std::setw ( WIDTH ) << it->rgi;
               ss << std::setw ( WIDTH ) << it->bias;
               ss << std::setw ( WIDTH ) << it->icq;
               ss << std::setw ( WIDTH ) << it->pwr;
               
               if ( isFBRxDebugInfo )
               {
                  ss << std::setw ( WIDTH ) << it->fbrx_gain_state;
                  ss << std::setw ( WIDTH ) << it->fbrx_hw_gain;
               }
               ss << endl;
            }
            ss << endl;
          }
      }
   }
   return true;
}

unsigned char RFCal::QSC::Log::FBRxGainResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams )
{
   std::map < QSCNode, map < CRFM_Path, ftm_fbrxgain_path_result > > fbrxResults;
   std::map < fbrx_gain_results_input_identifiers, CFBRx_MeasParams  >  TxResultMap;

   FBRxGainCal::StoreResults ( hPhone, swpParams, fbrxResults, TxResultMap);

   FBRxGainCal::OutputResults ( hPhone, swpParams, fbrxResults, TxResultMap);
   return true;
}
unsigned char RFCal::QSC::Log::Overall::OutputNVIDs ( HANDLE hPhone,  QSCParams & qscParams )
{
   stringstream ss;
  
   vector < QSCNode > qsc_nodes;
   map < int, string > stTechMap;
   stTechMap[0] = "CDMA";
   stTechMap[1] = "WCDMA";
   stTechMap[10] = "LTE";
   stTechMap[11] = "TDSCDMA";

   qscParams.repository.GetTechAndBandForCalStatus ( hPhone, qsc_nodes );
   ss << endl;
   ss << "======================================" << endl;
   ss << "Cal Results" << endl;
   ss << "======================================" << endl;
   int index = 0;
   int prevTech = -1;
   const int WIDTH = 10;
   ss << endl;
   ss << setw ( WIDTH ) << "Idx";
   ss << setw ( WIDTH ) << "Tech";
   ss << setw ( WIDTH ) << "Band";
   ss << setw ( WIDTH ) << "Subband";
   ss << setw ( WIDTH ) << "TxNVID";
   ss << setw ( WIDTH ) << "RxNVID";
   ss << setw ( WIDTH ) << "FBRxNVID";
   ss << setw ( WIDTH ) << "RxRSBNVID";
   ss << setw ( WIDTH ) << "IP2NVID";
   ss << endl;
   int result = 0;
   std::set < int > nvids;
   for ( std::vector < QSCNode >::const_iterator itr = qsc_nodes.begin(); itr != qsc_nodes.end(); itr++, index++)
   {
      if ( prevTech != itr->tech )
      {
         ss << endl;
      }
      ss << setw ( WIDTH ) << index;
      ss << setw ( WIDTH ) << stTechMap[itr->tech];
      ss << setw ( WIDTH ) << itr->band;
      ss << setw ( WIDTH ) << itr->subBand;

      string strNVID = "";
      int nvid = -1;
      
      if ( GetCalNVID (hPhone, (*itr), qscParams, SEQ_CAL_RESULT_TYPE_TX, strNVID, nvid ))
      {
         ss << setw ( WIDTH ) << strNVID;
         if ( -1 != nvid )
            nvids.insert ( nvid );
      }
      if ( GetCalNVID (hPhone, (*itr), qscParams, SEQ_CAL_RESULT_TYPE_RXGAIN, strNVID, nvid ))
      {
         ss << setw ( WIDTH ) << strNVID;
         if ( -1 != nvid )
            nvids.insert ( nvid );
      }
      if ( GetCalNVID (hPhone, (*itr), qscParams, SEQ_CAL_RESULT_TYPE_FBRXGAIN, strNVID, nvid ))
      {
         ss << setw ( WIDTH ) << strNVID;
         if ( -1 != nvid )
            nvids.insert ( nvid );
      }
	  if ( GetCalNVID (hPhone, (*itr), qscParams, SEQ_CAL_RESULT_TYPE_FD_RXRSB, strNVID, nvid ))
      {
         ss << setw ( WIDTH ) << strNVID;
         if ( -1 != nvid )
            nvids.insert ( nvid );
      }
	  if ( GetCalNVID (hPhone, (*itr), qscParams, SEQ_CAL_RESULT_TYPE_IP2, strNVID, nvid ))
      {
         ss << setw ( WIDTH ) << strNVID;
         if ( -1 != nvid )
            nvids.insert ( nvid );
      }
      ss << endl;

   }

   // *******************************//
   // Add XO NV in XQCN if necessary 
   // *******************************//
   if ( 1 )
   {
      nvids.insert ( 4954 );
   }

   vector < unsigned short > err_list;
   err_list.resize ( nvids.size() );

   if ( !nvids.empty () )
   {
      // *******************************//
      // Get NV data from mobile
      // *******************************//
      vector <unsigned short > tempNVID;
      tempNVID.reserve ( nvids.size() );
      // std::set removes duplicate NVIDs
      for ( std::set < int >::const_iterator itr = nvids.begin(); itr !=  nvids.end(); itr++)
      {
         tempNVID.push_back ( static_cast < unsigned short > ( *itr ) );
      }
      unsigned short numIds = static_cast < unsigned short >  (tempNVID.size());

      unsigned char status = QLIB_RFCAL_NV_Manager_BackupNVItems (hPhone, &tempNVID[0], &err_list[0], numIds );

      // *******************************//
      // Create XQCN
      // *******************************//
      if ( numIds > 0  && status)
      {
         QLIB_RFCAL_NV_Manager_Create_QCN ( hPhone, (char *)qscParams.FilePath.stCalibratedXQCN.c_str() );

         ss << endl;
         ss << "[XQCN Generation from mobile]" << endl;
         ss << "XQCN Filepath: " << qscParams.FilePath.stCalibratedXQCN << endl;
         ss << "The number of NVs:" << numIds << endl;
      }
   }   
   RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ( ss.str() ) );

   return true;
}
unsigned char RFCal::QSC::Log::Overall::OutputTestTimePerNodes ( HANDLE hPhone, const std::vector < QSCNode > & qscNodes, QSCParams & qscParams )
{
   stringstream ss;
  
   // Test Time
   const int WIDTH = 10;
   int index = 0;
   ss << "======================================" << endl;
   ss << "Test Time per QSC Nodes" << endl;
   ss << "======================================" << endl;
   ss << endl;
   ss << setw ( WIDTH ) << "Idx";
   ss << setw ( WIDTH ) << "Tech";
   ss << setw ( WIDTH ) << "Band";
   ss << setw ( WIDTH ) << "Subband";
   ss << setw ( WIDTH ) << "SwpType";
   ss << setw ( WIDTH ) << "Instance";
   ss << setw ( WIDTH ) << "CalTime";
   ss << endl;
   for ( vector <QSCNode>::const_iterator itr = qscNodes.begin(); itr != qscNodes.end(); itr++ )
   {
      /* Index */
      ss << setw ( WIDTH ) << index;
      
      /* Tech */
      ss << setw ( WIDTH ) << RFCal::Helper::Convert::TechID_To_Str (itr->tech );
      
      /* Band and Subband */
      std::map < QSCNode, QSCSwpParams >::const_iterator swpParamItr = qscParams.qscParams.find ( *itr );
      if ( swpParamItr == qscParams.qscParams.end() )
      {
         continue;
      }

      stringstream tmpBandSS, tmpSubbandSS;
      for ( vector < CBandInfo >::const_iterator bandInfoItr = swpParamItr->second.BandInfo.begin();
         bandInfoItr !=  swpParamItr->second.BandInfo.end(); bandInfoItr++)
      {
         if ( swpParamItr->second.BandInfo.begin() == bandInfoItr)
         {
            tmpBandSS << bandInfoItr->band;
            tmpSubbandSS << bandInfoItr->subBand; 
         }
         else
         {
            tmpBandSS << "/" << bandInfoItr->band;
            tmpSubbandSS << "/" << bandInfoItr->subBand; 
         }
      }
      ss << setw ( WIDTH ) << tmpBandSS.str();
      ss << setw ( WIDTH ) << tmpSubbandSS.str();

      /* Sweep Type */
      if ( itr->sweep_type == SEQ_CAL_CONNECTED )
         ss << setw ( WIDTH ) << "Connect";
      else if ( itr->sweep_type == SEQ_CAL_UNCONNECTED )
         ss << setw ( WIDTH ) << "Unconnect";

      /* Instance */
      ss << setw ( WIDTH ) << itr->instance;
      unsigned long calTime = 0;
      qscParams.repository.TestTimeGetData ( hPhone, ( *itr), calTime);

      /* Cal Time */
      ss << setw ( WIDTH ) << static_cast < double > ( calTime / 1000.0);
      ss << endl;
      index++;
   }
   RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ( ss.str() ) );
   return true;
}
unsigned char RFCal::QSC::Log::OverallResults (HANDLE hPhone, const std::vector < QSCNode > & qscNodes,  QSCParams & qscParams)
{
   if ( qscNodes.empty () )
   {
      return false;
   }
   Overall::OutputNVIDs ( hPhone, qscParams );

   Overall::OutputTestTimePerNodes ( hPhone, qscNodes, qscParams );

   return true;
}
unsigned char  RFCal::QSC::Log::GetCalNVID ( HANDLE hPhone, const QSCNode & qscNode, QSCParams & qscParams, seq_cal_res_type type, string & strNVID, int & nvid)
{
   // if no cal, return -1,
   stringstream ss;
   CCal_Status cal_status;
   nvid = -1;
   if ( qscParams.repository.GetCalStatus ( hPhone, qscNode, type, cal_status ) )
   {
      if ( RFCal::QSC::Cal_Status_DB::SUCCESS == cal_status.status )
      {
         ss << cal_status.nvid;    
         nvid = cal_status.nvid;
      }
      else
      {
         ss << "Failed";
      }
   }
   else
   {
      ss << "-";
   }
   strNVID = (ss.str());
   return true;
}
unsigned char RFCal::QSC::Log::FBRxNVResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams )
{
   stringstream ss;
   const int WIDTH = 10;
   ss << endl;
   ss << "===================================================" << endl;
   ss << "Push FBRx NV Data Result " << endl;
   ss << "===================================================" << endl << endl;
   
   for ( vector < qscRecords >::const_iterator qsc_it = swpParams.swps.begin(); qsc_it != swpParams.swps.end(); qsc_it++)
   {
      for ( vector < cal_sweep_record >::const_iterator it = qsc_it->swpRecords.begin(); it != qsc_it->swpRecords.end(); it++)
      {
         
         bool bFound = false;
         if ( it->data.fbrxAgcTbl.ptr_fbrx_cal_data )
         {
            int nvid = static_cast <int> ( it->dataRes.pushfbrxRes.nv_id );
            int nv_writing_time = static_cast <int> ( it->dataRes.pushfbrxRes.nv_write_time );
            QSCNode qsc_node_band;
            qsc_node_band.tech = static_cast <int> (it->data.tech);
            qsc_node_band.band = static_cast <int> (it->data.fbrxAgcTbl.ptr_fbrx_cal_data->band );
            qsc_node_band.subBand = static_cast <int> (it->data.fbrxAgcTbl.ptr_fbrx_cal_data->subband );

            CCal_Status cal_status;
            if ( ( 0 != nvid ) && ( 0 != nv_writing_time ) )
               cal_status.status = RFCal::QSC::Cal_Status_DB::SUCCESS;
            else
               cal_status.status = RFCal::QSC::Cal_Status_DB::FAILURE;

            cal_status.nvid = nvid;
            cal_status.cal_time = 0;// Cal time, not nv writing time.


            ss << "---------------------------------------------------" << endl;
               ss << "Tech: " << qsc_node_band.tech; 
               ss << ", Band: " << qsc_node_band.band; 
               ss << ", Subband: " << qsc_node_band.subBand; 
               ss << endl;
               ss << "NV Writing Status: ";
               if ( RFCal::QSC::Cal_Status_DB::SUCCESS == cal_status.status )
               {
                  ss << "Success.  NVID = " << cal_status.nvid;
               }
               else
               {
                  ss << "Failure.";
               }
               ss << endl;
               ss << "---------------------------------------------------" << endl << endl;

             // Register NV ID result
            swpParams.repoPtr->SetCalStatus ( hPhone, qsc_node_band, SEQ_CAL_RESULT_TYPE_FBRXGAIN, cal_status, RFCal::QSC::Cal_Status_DB::NVID );

         }
      }
   }
   RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str() );
   return true;
}
int RFCal::QSC::Param::GetBandwidth_FromBW_Type( HANDLE hPhone, const vector < int > & bw_list )
{
   int bw = 0;
   for ( vector < int >::const_iterator itr = bw_list.begin(); itr != bw_list.end(); itr++ )
   {
      switch ( *itr )
      {
         case 3:
            bw += 10;
            break;
         case 5:
            bw += 20;
            break;
      }
   }
   return bw;
}

unsigned char RFCal::QSC::Log::TxCal::OutputResults ( HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams, 
   const std::map < CRFM_Path, CTX_Results > & txResults,
   int & tx_nv_id )
{
   stringstream ss;
   const int WIDTH = 10;
   ss << endl;
   ss << "===================================================" << endl;
   ss << "Tx Cal Result " << endl;
   ss << "===================================================" << endl << endl;

   ss << "---------------------------------------------------" << endl;
      ss << "Tech: " << qscNode.tech; 
      ss << ", Band: " << qscNode.band; 
      ss << ", Subband: " << qscNode.subBand; 
      ss << endl;
    ss << "---------------------------------------------------" << endl << endl;
 
    ss << "---------------------------------------------------" << endl;
 
    ss << "Tx NV ID: " << tx_nv_id << endl;
    ss << "---------------------------------------------------" << endl << endl;

   CCal_Status cal_status;
   if ( 0 != tx_nv_id )
      cal_status.status = RFCal::QSC::Cal_Status_DB::SUCCESS;
   else
      cal_status.status = RFCal::QSC::Cal_Status_DB::FAILURE;

   cal_status.nvid = tx_nv_id;

   // Register NV ID result
   swpParams.repoPtr->SetCalStatus ( hPhone, qscNode, SEQ_CAL_RESULT_TYPE_TX, cal_status, RFCal::QSC::Cal_Status_DB::NVID );

   const CBandInfo & bandInfo = swpParams.BandInfo.back();
   for ( map < CRFM_Path, CTX_Results >::const_iterator rfm_it = txResults.begin();
      rfm_it != txResults.end(); rfm_it ++ )
   {
      ss << "===================================================" << endl;
      ss << "Signal Path: " << rfm_it->first.signal_path;
      ss << ", Rfm Device: " << rfm_it->first.rfm_dev;
      ss << ", Antenna Port: " << rfm_it->first.antenna_port << endl ;
      if ( 0 == rfm_it->second.cal_status )
         ss << "Cal Status: Success" << endl;
      else
         ss << "Cal Status: Failed" << endl;

      ss << "===================================================" << endl;

    // Check offset cal, offset cal has no result
    if (  rfm_it->second.tx_freqcomp_results.empty()
       && rfm_it->second.tx_lin_results.empty()
       && rfm_it->second.xpt_debug_info.empty()
       && rfm_it->second.freqcomp_channels.empty()
       && rfm_it->second.xpt_resuls.empty() )
    {
       continue;
    }

    ss << "---------------------------------------------------" << endl ;
    ss << "Therm Adc: " << rfm_it->second.therm << endl;
    ss << "---------------------------------------------------" << endl << endl;

    for ( vector < vector < int > >::const_iterator itr = bandInfo.commonParams.Tx_Rx_BW_Type.begin();
         itr != bandInfo.commonParams.Tx_Rx_BW_Type.end(); itr++ )
    {
      int bw = RFCal::QSC::Param::GetBandwidth_FromBW_Type ( hPhone, *itr );
      int xpt_mode = XPT_TYPE_APT;
      /* Full bias */
      if ( bandInfo.calType.Enable_Full_Bias_Linearizers )
      {
         ss << "===================================================" << endl;
         ss << "Full Bias Linearlizer " << GetBWString(hPhone, qscNode.tech, bw) << endl;
         ss << "===================================================" << endl << endl;  
         TxCal::Output::Linearlizer ( hPhone, qscNode, swpParams, RFCal::QSC::Log::FULL_BIAS_LINEARLIZER, bw, xpt_mode, rfm_it->second.tx_lin_results, ss );      
      }

      /* Apt Cal */
      if ( bandInfo.calType.Enable_APT_Linearizers )
      {
         ss << "===================================================" << endl;
         ss << "APT Linearlizer " << GetBWString(hPhone, qscNode.tech, bw) << endl;
         ss << "===================================================" << endl << endl;
         TxCal::Output::Linearlizer ( hPhone, qscNode, swpParams, RFCal::QSC::Log::APT_LINEARLIZER, bw, xpt_mode, rfm_it->second.tx_lin_results, ss );
      }

      if ( bandInfo.calType.Enable_APT_FComp )
      {
         ss << "===================================================" << endl;
         ss << "APT FreqComp " << GetBWString(hPhone, qscNode.tech, bw) << endl;
         ss << "===================================================" << endl << endl;
         TxCal::Output::FreqComp ( hPhone, qscNode, swpParams, rfm_it->second.freqcomp_channels, rfm_it->second.tx_freqcomp_results, XPT_TYPE_APT, ss );
      }

      RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str() );
      ss.str("");
      ss << endl;

      /* Ept/Et Cal */
      if (! bandInfo.calType.Enable_Tx_XPT ) continue;
  
      for ( vector < int >::const_iterator xpt_itr = bandInfo.xptParams.XPT_Mode_List.begin(); xpt_itr != bandInfo.xptParams.XPT_Mode_List.end(); xpt_itr++ )
      {
         string stXPT = (*xpt_itr == XPT_TYPE_ET )? "ET" : "EPT";
         ss << "===================================================" << endl;
         ss << stXPT << " Cal Result" << GetBWString(hPhone, qscNode.tech, bw)<< endl;
         ss << "===================================================" << endl << endl;
         TxCal::Output::XptCalStatus ( hPhone, qscNode, swpParams, rfm_it->second.xpt_resuls, (*xpt_itr), ss );

         ss << "===================================================" << endl;
         ss << "RGI Delta sweep for " << stXPT << GetBWString(hPhone, qscNode.tech, bw) << endl;
         ss << "===================================================" << endl << endl;
         TxCal::Output::Linearlizer ( hPhone, qscNode, swpParams, RFCal::QSC::Log::RGI_DELTA, bw, *xpt_itr,rfm_it->second.tx_lin_results, ss );
		   
         ss << "===================================================" << endl;
         ss << "Alighnment sweep for" << stXPT << GetBWString(hPhone, qscNode.tech, bw)<< endl;
         ss << "===================================================" << endl << endl;
          
         TxCal::Output::Linearlizer ( hPhone, qscNode, swpParams, RFCal::QSC::Log::ALIGNMENT, bw, *xpt_itr, rfm_it->second.tx_lin_results, ss );


         if ( bandInfo.calType.Enable_XPT_FComp )
         {
            ss << "===================================================" << endl;
            ss << stXPT << " FreqComp " << GetBWString(hPhone, qscNode.tech, bw) << endl;
            ss << "===================================================" << endl << endl;
            TxCal::Output::FreqComp ( hPhone, qscNode, swpParams, rfm_it->second.freqcomp_channels, rfm_it->second.tx_freqcomp_results, (*xpt_itr), ss );
         }

         TxCal::Output::XptDebugInfoResults ( hPhone, qscNode, swpParams, rfm_it->second.xpt_debug_info, (*xpt_itr), bw, ss );
      
      RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str() );
      ss.str("");
      ss << endl;
         }
      }
   }
   RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str() );

   return true;
}
string RFCal::QSC::Log::TxCal::GetBWString( HANDLE hPhone, int tech, int bw )
{
   stringstream ss;
   if ( TECHNOLOGY_LTE == tech )
   {
      switch ( bw )
      {
      case 10:
           ss << " ( 10 MHz )";
           break;
      case 40:
           ss << " ( 40 MHz )";
           break;
      }
   }
   return ss.str();
}
int RFCal::QSC::Log::TxCal::GetBWFromWvIndex ( HANDLE hPhone, unsigned char wvIndex )
{
   int bw = -1;
   switch ( wvIndex )
   {
   case 1:   // APT
      bw = 10; 
      break;
   case 3:  // // Fullbias
      bw = 10; 
      break;
   case 4: // Fullbias 40 Mhz
      bw = 40;
      break;
   case 5: // Apt/Xpt 40 Mhz
      bw = 40;
      break;
   }
   return bw;
}
unsigned char RFCal::QSC::Log::TxCal::StoreResults ( HANDLE hPhone, const QSCSwpParams & swpParams, 
   std::map < CRFM_Path, CTX_Results > & txResults,
   int & tx_nv_id )
{
   const CBandInfo & bandInfo = swpParams.BandInfo.front();
 
   for ( vector < qscRecords >::const_iterator qsc_it = swpParams.swps.begin(); qsc_it != swpParams.swps.end(); qsc_it++)
   {
      for ( vector < cal_sweep_record >::const_iterator it = qsc_it->swpRecords.begin(); it != qsc_it->swpRecords.end(); it++)
      {
         QSCNode tmpQscNode(0,0,0);
         bool bFound = false;
         if (!( it->dataRes.seqCalRes && it->dataRes.seqCalRes->result_mask & SEQ_CAL_RESULT_TYPE_TX )) continue;
         uint8 numValidResult = it->dataRes.seqCalRes->unconn_res_data.tx_cal_data.num_valid_results;
         // *************************************************** //
         // NV ID
         // *************************************************** //
         tx_nv_id = static_cast <int> ( it->dataRes.seqCalRes->unconn_res_data.tx_cal_data.nv_id );

         // *************************************************** //
         // RFM path loop
         // *************************************************** //
         for ( uint8 idx = 0; idx < numValidResult; idx++)
         {
            seq_cal_tx_res_arr_type & tx_res = it->dataRes.seqCalRes->unconn_res_data.tx_cal_data.tx_res_arr [ idx ];
               
            // *************************************************** //
            // RFM path
            // *************************************************** //
            CRFM_Path rfm_path ;
            rfm_path.rfm_dev = static_cast <int> ( tx_res.rfm_dev );
            rfm_path.antenna_port = static_cast <int> ( tx_res.antenna_path );
            rfm_path.signal_path = static_cast <int> ( tx_res.signal_path ); // Nv container is obsolete

            // *************************************************** //
            // Cal Status
            // *************************************************** //
            txResults [ rfm_path ].cal_status = static_cast <int> ( tx_res.cal_status );
            seq_cal_tx_lin_fcomp_res_type & apt = tx_res.tx_fcomp_lin_res_data;
               
            // *************************************************** //
            // Ref Channel/Therm/Upperbound channel
            // *************************************************** //

            txResults [ rfm_path ].ref_channel = static_cast < int> ( apt.ref_channel );
            txResults [ rfm_path ].therm =static_cast < int> ( apt.ref_lin_temp_adc );
            txResults [ rfm_path ].upper_bound_channel = static_cast < int> ( apt.upper_bound_channel );
               
            // *************************************************** //
            // Fcomp channels
            // *************************************************** //

            txResults [ rfm_path ].freqcomp_channels.clear();
            txResults [ rfm_path ].freqcomp_channels.reserve ( apt.tx_cal_chan_size );

            for ( uint8 chIdx = 0; chIdx < apt.tx_cal_chan_size; chIdx++)
            {
               txResults [ rfm_path ].freqcomp_channels.push_back ( static_cast < int > ( apt.tx_cal_chan [ chIdx ] ));
            }
               
            // *************************************************** //
            // PA State loop
            // *************************************************** //

            for ( uint8 pa_state_idx = 0; pa_state_idx < 8; pa_state_idx++)
            {
               if (! ( (1 << pa_state_idx ) & apt.pa_state_mask ) ) continue;

               seq_cal_tx_cal_res_per_pa_data_type & result_per_pa = apt.tx_pa_state_cal_data[pa_state_idx];
               int pa_state = result_per_pa.pa_state;    
                   
               // *************************************************** //
               // FreqComp Offset
               // *************************************************** //
                     
               vector < seq_cal_tx_freq_comp_tbl_type > tmpFreqCompPerPA;
               tmpFreqCompPerPA.reserve ( result_per_pa.num_fcomp );
               for ( uint8 idx = 0; idx < result_per_pa.num_fcomp; idx++)
               {
                  tmpFreqCompPerPA.push_back (  result_per_pa.freq_comp_data[ idx ] );
               }
               txResults [ rfm_path ].tx_freqcomp_results [ pa_state ] = tmpFreqCompPerPA;
                     
               const int MAX_WAVEFORM = 6;
                    
               // *************************************************** //
               // Waveform loop
               // *************************************************** //
                     
               for ( int wvIdx = 0; wvIdx < MAX_WAVEFORM; wvIdx++ ) 
               {
						 
                  if (! ( ( 1 << wvIdx ) & result_per_pa.linearizer_table_mask )) continue;

                  seq_cal_tx_cal_res_linearizer_tbl_type & tbl_per_wv = result_per_pa.linearizer_tbl_data[wvIdx].tx_linearizer_table;;
                 
                  // *************************************************** //
                  // Dpd Type: ET/EPT Results
                  // *************************************************** //
                          
                  int dpd_type = static_cast < int > ( tbl_per_wv.dpd_type );

                  //std::vector < CTX_Lin_TxParam_Pwr > txResultSet; 
                  CVec_TxPwr txResultSet;

                  if (  ( tbl_per_wv.dpd_type == TX_LINEARIZER_EPT_VOLTERRA_DATA )
                     || ( tbl_per_wv.dpd_type == TX_LINEARIZER_ET_VOLTERRA_DATA )
                     )
                  {
                     FTM_xptType_Enum xpt_mode = ( tbl_per_wv.dpd_type == TX_LINEARIZER_EPT_VOLTERRA_DATA )? XPT_TYPE_EPT : XPT_TYPE_ET;
                     int bw  = GetBWFromWvIndex ( hPhone, wvIdx );
                     CXPT_Results tempXptResult;
                     tempXptResult.xpt_mode = static_cast < int > ( xpt_mode );
                     tempXptResult.num_dpd = static_cast <int> ( tbl_per_wv.num_dpd );
                     tempXptResult.pa_state = static_cast <int> ( pa_state);
                     tempXptResult.BW = bw;
                     tempXptResult.status = static_cast <int> ( tx_res.xpt_cal_status[ xpt_mode ] ); 
						   
                     txResults [ rfm_path ].xpt_resuls [ xpt_mode ].push_back( tempXptResult );
                     int bwIdx = -1;
                     for ( vector < vector < int > >::const_iterator itr = bandInfo.commonParams.Tx_Rx_BW_Type.begin();
                                 itr != bandInfo.commonParams.Tx_Rx_BW_Type.end(); itr++ )
                     {
                        int ParamBW = RFCal::QSC::Param::GetBandwidth_FromBW_Type ( hPhone, *itr );
                        if ( bw == ParamBW)
                        {
                           bwIdx = std::distance < vector < vector < int > >::const_iterator >( bandInfo.commonParams.Tx_Rx_BW_Type.begin(), itr );
                        }
                     }
                     if ( bwIdx >= 0 && bwIdx < MAX_NUM_BW_CAL_SINGLE_SWEEP_QSC )
                     {
                        txResults [ rfm_path ].xpt_debug_info [ tempXptResult ] = it->dataRes.seqCalRes->unconn_res_data.xpt_debug_data[ bwIdx ] [ xpt_mode ];
                     }
                  }
                           
                  // *************************************************** //
                  // RGI Loop
                  // *************************************************** //
                  txResultSet.reserve ( tbl_per_wv.num_valid_rows );
                  for ( uint8 tblIdx = 0 ; tblIdx < tbl_per_wv.num_valid_rows; tblIdx++ )
                  {
                     CTX_Lin_TxParam_Pwr result;
                     seq_cal_tx_lin_row_type & row = tbl_per_wv.table[ tblIdx ];
                     seq_cal_fbrx_debug_info_type & fbrx_debug = tbl_per_wv.fbrx_debug_info_table [ tblIdx ];

                     result.rgi = static_cast < int > ( row.rgi );
                     result.channel = static_cast < int > (  apt.ref_channel );
                     result.icq = static_cast < int > ( row.pa_q_current );
                     result.bias = static_cast < int > ( row.pa_bias_voltage );
                     result.dpd_idx = static_cast < int > ( row.dpd_index );
                     result.iqOffset = static_cast < int > ( row.iq_offset );
                     result.pwr = static_cast < double > ( row.power / 10.0 );

                     // FBRx Debug Info
                     result.fbrx_gain_state = static_cast < int > ( fbrx_debug.fbrx_gain_state );
                     result.fbrx_hw_gain = static_cast < int > ( fbrx_debug.fbrx_hw_gain );

                     txResultSet.push_back ( result );                           
                  }

                  // *************************************************** //
                  // Store tx linearizer results for this rfm path
                  // *************************************************** //

                  txResults [ rfm_path ].tx_lin_results[ pa_state ][ dpd_type ][ wvIdx ]= txResultSet ;
                  
                  }
               }
            }
      }
   }
   
   return true;
}
unsigned char RFCal::QSC::Log::FBRxGainCal::OutputResults ( HANDLE hPhone, const QSCSwpParams & swpParams,   
            std::map < QSCNode, std::map < CRFM_Path, ftm_fbrxgain_path_result > > & fbrxResults, 
            std::map < fbrx_gain_results_input_identifiers, CFBRx_MeasParams  > & TxResultMap )
{
   stringstream ss;
   const int WIDTH = 11;
   ss << endl << "===================================================" << endl;
   ss << "FBRx Gain Cal Result " << endl;
   ss << "===================================================" << endl << endl;

   for ( map < QSCNode, map < CRFM_Path, ftm_fbrxgain_path_result > >::iterator band_it =  fbrxResults.begin(); band_it != fbrxResults.end(); band_it++)
   {
   ss << "---------------------------------------------------" << endl;
      ss << "Tech: " << band_it->first.tech; 
      ss << ", Band: " << band_it->first.band; 
      ss << ", Subband: " << band_it->first.subBand; 
      ss << endl;
    ss << "---------------------------------------------------" << endl << endl;

      vector < CBandInfo >::const_iterator bandParamIt  = swpParams.BandInfo.begin();
      for ( ; bandParamIt != swpParams.BandInfo.end(); bandParamIt++ )
      {
         if ( ( bandParamIt->band == band_it->first.band)
            && ( bandParamIt->subBand == band_it->first.subBand) )
         {
            break;
         }
      }
      if ( bandParamIt == swpParams.BandInfo.end() )
      {
         continue; // We should not come here
      }
      const double FBRX_PATH_GAIN_MARGIN = 16.0;
      for (  map < CRFM_Path, ftm_fbrxgain_path_result >::iterator it = band_it->second.begin(); it != band_it->second.end(); it++)
      {
         string stCalSuccess = "";
         if ( 0 == it->second.calSuccessful)
            stCalSuccess = " Success";
         else
            stCalSuccess = " Failure";
         ss << "---------------------------------------------------" << endl;
         ss << "RFM Path ( signal path: " << it->first.signal_path;
         ss << ", rfm_dev: " << it->first.rfm_dev;
         ss << ", antenna port: " << it->first.antenna_port;
         ss << ") " << endl;
         ss << "Cal Result: " << stCalSuccess << endl;
         ss << "---------------------------------------------------" << endl;

         ftm_fbrxgain_path_result & path = it->second;
         
         ss << std::setw(WIDTH) << "Channel" ;//<<

         ss << std::setw(WIDTH) << "FBRxMode" ;//<<
         ss << std::setw(WIDTH) << "FBGainState" ;//<<
         ss << std::setw(WIDTH) << "PA" ;//<<
         ss << std::setw(WIDTH) << "RGI" ;//<<
         ss << std::setw(WIDTH) << "Bias" ;//<<
         ss << std::setw(WIDTH) << "Icq" ;//<<

         ss << std::setw(WIDTH) << "ExpFBPGan" ;//<<
         ss << std::setw(WIDTH) << "ExpTxPwr" ;//<<
         ss << std::setw(WIDTH) << "DigScale" ;//<<
        // ss << std::setw(WIDTH) << "HWGDelta" ;//<<

         ss << std::setw(WIDTH) << "FBHwGain" ;//<<
         ss << std::setw(WIDTH) << "FBSwGain" ;//<<
         ss << std::setw(WIDTH) << "CorrIdx" ;//<<
         ss << std::setw(WIDTH) << "LSE" ;//<<
         ss << std::setw(WIDTH) << "FBPathG" ;//<<
         ss << std::setw(WIDTH) << "FBPathGMax" ;//<<
         ss << std::setw(WIDTH) << "FBPathGMin" ;//<<
         
         ss << std::setw(WIDTH) << "TxPower" ;//<<
         if ( path.fbrxDCCaldataavailable )
         {
            ss << std::setw(WIDTH) << "FBRxDC_I" ;//<<
            ss << std::setw(WIDTH) << "FBRxDC_Q" ;//<<
         }

         ss << endl;
         for (uint8 mode_idx = 0; mode_idx < path.numberOfFbrxModes; mode_idx++)
         {
            ftm_fbrxgain_mode_result * mode = & ( path.modeResults [mode_idx] );
            int bw = static_cast < int > ( mode->fbrxMode );
            for (uint8 ch_idx = 0; ch_idx < mode->numberofChannels; ch_idx++)
            {
               ftm_fbrxgain_channel_result * chan_result = & ( mode->channelResults[ ch_idx ] );
               uint32 channel = chan_result->channel;
               for (uint8 gain_idx = 0; gain_idx < chan_result->numberOfGainStates; gain_idx++ )
               {
                  rfcommon_core_fbrx_result_type * gain_result = & ( chan_result->fbrxResult [ gain_idx ] );
                  int fbrx_gain_state = gain_result->fbrx_gain_state;

                  fbrx_gain_results_input_identifiers fbrx_param;
                  fbrx_param.node = (band_it->first);
                  fbrx_param.rfm_path = it->first;

                  fbrx_param.bwType =  bw;
                  fbrx_param.channel =  channel;

                  fbrx_param.fbrxGainState = fbrx_gain_state;
                  
                  const std::map < fbrx_gain_results_input_identifiers, CFBRx_MeasParams  >::const_iterator txResultItr = TxResultMap.find ( fbrx_param );
                  if ( txResultItr == TxResultMap.end() ) { continue; }
                  const double txPower = txResultItr->second.txPower;

                   ss << std::setw(WIDTH) << static_cast <int> (channel);
                   ss << std::setw(WIDTH) << static_cast <int> (bw); // FBRx Mode
                   ss << std::setw(WIDTH) << static_cast <int> (fbrx_gain_state);

                   ss << std::setw(WIDTH) << static_cast <int> (txResultItr->second.pa_state);
                   ss << std::setw(WIDTH) << static_cast <int> (txResultItr->second.rgi);

                   ss << std::setw(WIDTH) << static_cast <int> (txResultItr->second.bias);
                   ss << std::setw(WIDTH) << static_cast <int> (txResultItr->second.icq);
                   
                   /* Nominal Gain of the FBRx Path stored in the phone */
                   double fbrxPathGainNominaldB = gain_result->gain_fbrx_nom/10.0;
                   ss << std::setw(WIDTH) << static_cast <int> (fbrxPathGainNominaldB); // ExpectedFBRxPathGain 
                   ss << std::setw(WIDTH) << static_cast <int> (txResultItr->second.expectedPwr);
                   ss << std::setw(WIDTH) << static_cast <int> (gain_result->trunc_bits); // /* Digital Scaling */
                  // ss << std::setw(WIDTH) << static_cast <int> (gain_result->hw_gain_delta); // 
                   ss << std::setw(WIDTH) << static_cast <int> (gain_result->fbrx_hw_gain); // 
                   ss << std::setw(WIDTH) << static_cast <int> (gain_result->fbrx_sw_gain); // Digital Scale
                   ss << std::setw(WIDTH) << static_cast <int> (gain_result->predet_idx); // 
                   ss << std::setw(WIDTH) << static_cast <int> (gain_result->ls_err); // 
                   if (  static_cast <int> ( gain_result->ls_err) > bandParamIt->fbrxGainParams.Max_LSE )
                   {
                      ss << " MaxLSE is over than limit";
                   }

                   /* Tx power of Reference Signal to ILPC HW */
                   double pRefPowerdBm= gain_result->txfe_power / 10.0;
                   
                                    
                   double swGaindB = 20* log10 ((double)gain_result->fbrx_sw_gain / (double)(32768));
                   /* FBRx Path Analog Gain. */
                   double fbrxPathAnalogGaindB = /* Overall Gain */ swGaindB
                                                                  - /* Tx Path Gain */(txResultItr->second.txPower - pRefPowerdBm) - /* FBRx Path Digital Gain */(6.02 * (10 - gain_result->trunc_bits));
        
                   
                   double fbrx_path_gain = RFCal::Helper::Convert::ROUND(fbrxPathAnalogGaindB, 2);
                   ss << std::setw(WIDTH) <<  fbrx_path_gain;
                   ss << std::setw(WIDTH) <<  (fbrxPathGainNominaldB + FBRX_PATH_GAIN_MARGIN );
                   ss << std::setw(WIDTH) <<  (fbrxPathGainNominaldB - FBRX_PATH_GAIN_MARGIN );


                //   ss << std::setw(WIDTH) << static_cast <int> (gain_result->fbrx_hw_gain); // 
                   
                  
                   ss << std::setw(WIDTH) << txResultItr->second.txPower;
                   if ( path.fbrxDCCaldataavailable )
                   {
                     ss << std::setw(WIDTH) << gain_result->fbrx_dc_i ;//<<
                     ss << std::setw(WIDTH) << gain_result->fbrx_dc_q ;//<<
                   }
 
                   ss << endl;


                  //******************************************************//
                  // Assign tx meas value into txfe_power for FBRx Result DB.
                  //******************************************************//

                  if (txPower >= 0)
                  {
                     gain_result->txfe_power = static_cast < int > ( txPower * 10.0 + 0.5);
                  }
                  else
                  {
                     gain_result->txfe_power = static_cast < int > ( txPower * 10.0 - 0.5);
                  }
                  // for debug purpose
                  gain_result->fbrx_mode = bw;
               }
            }
         }
         //******************************************************//
         // Register FBRx path data into FBRx Result DB (for unconnected cal)
         // Make sure gain_result->txfe_power is measuremed Tx data now
         //******************************************************//

         swpParams.repoPtr->SetFBRxResult (hPhone, (band_it->first), it->first, it->second);
      }
   }
    RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str() );

   return true;
}
unsigned char RFCal::QSC::Log::FBRxGainCal::StoreResults ( HANDLE hPhone, const QSCSwpParams & swpParams,   
            std::map < QSCNode, std::map < CRFM_Path, ftm_fbrxgain_path_result > > & fbrxResults, 
            std::map < fbrx_gain_results_input_identifiers, CFBRx_MeasParams  > & TxResultMap )
{
   
   for ( vector < qscRecords >::const_iterator qsc_it = swpParams.swps.begin(); qsc_it != swpParams.swps.end(); qsc_it++)
   {

      int tech = -1;
      const ftm_seq_cal_bld_fbrxgain_parameters_v0 * p_fbrx_param = NULL;
      const ftm_cal_seq_bld_radio_setup_path_params * p_radio_path = NULL; 
      for ( vector < cal_sweep_record >::const_iterator it = qsc_it->swpRecords.begin(); it != qsc_it->swpRecords.end(); it++)
      {
        
         bool bFound = false;
         if ( SEQ_RADIO_SETUP == it->seqBuild.seqType)
         {
            tech = static_cast < int > (it->seqBuild.radioSetupParams.radioSetupV0.tech );
            for ( int i = 0; i <  it->seqBuild.radioSetupParams.radioSetupV0.numRFMPaths; i++)
            {

               if ( RFM_PATH_TYPE_TX == it->seqBuild.radioSetupParams.radioSetupV0.radioSetupPathParams[i].rfmPathType)
               {
                   p_radio_path = & ( it->seqBuild.radioSetupParams.radioSetupV0.radioSetupPathParams[i]);
                   break;
               }
            }
         }
         if ( SEQ_FBRXGAIN_CAL == it->seqBuild.seqType )
         {
            p_fbrx_param = & (it->seqBuild.fbrxGainParameters.fbrxGainbuildParameters.version0);
         }
         if ( SEQ_FBRXGAIN_CAL == it->seqExec.seqType )
         {
            fbrx_gain_results_input_identifiers fbrx_param;
            fbrx_param.node.tech = tech;
            if ( p_radio_path )
            {
               fbrx_param.node.band = static_cast < int > ( p_radio_path->band);
               fbrx_param.node.subBand = static_cast < int > ( p_radio_path->subBand);
 
               fbrx_param.rfm_path.rfm_dev = static_cast < int > ( p_radio_path->rfmPath.rfmDevice);
               fbrx_param.rfm_path.signal_path = static_cast < int > ( p_radio_path->rfmPath.sigPath);
               fbrx_param.rfm_path.antenna_port = static_cast < int > ( p_radio_path->rfmPath.antennaSwitchPath);

               fbrx_param.bwType =  static_cast < uint8 > ( p_radio_path->bwType);
               fbrx_param.channel =  static_cast < uint32 > ( p_radio_path->channelList[0]);
            }
            int overrideIndex = it->seqExec.fbrxGainParameters.fbrxGainexecParameters.version0.overrideIndex;
            if ( NULL != p_fbrx_param )
            {
               fbrx_param.fbrxGainState = p_fbrx_param->fbrxParameters[ overrideIndex ].fbrxGainState;
               CFBRx_MeasParams measParam;
               measParam.txPower = it->txRes.txPower;
               measParam.pa_state = static_cast < int > ( p_fbrx_param->txOverrideParameters[ overrideIndex].paState );
               measParam.rgi = static_cast < int > ( p_fbrx_param->txOverrideParameters[ overrideIndex].rgi );
               measParam.bias = static_cast < int > ( p_fbrx_param->txOverrideParameters[ overrideIndex].biasinmilliVolts );
               measParam.icq = static_cast < int > ( p_fbrx_param->txOverrideParameters[ overrideIndex].quiescentCurrent );
               measParam.expectedPwr = static_cast < int > ( p_fbrx_param->fbrxParameters[ overrideIndex].expectedPowerdBm10 );

               TxResultMap [ fbrx_param ] = measParam;
            }
         }
      
      }
   }


   for ( vector < qscRecords >::const_iterator qsc_it = swpParams.swps.begin(); qsc_it != swpParams.swps.end(); qsc_it++)
   {
      for ( vector < cal_sweep_record >::const_iterator it = qsc_it->swpRecords.begin(); it != qsc_it->swpRecords.end(); it++)
      {
         QSCNode tmpQscNode(0,0,0);
         bool bFound = false;
         if ( it->dataRes.seqCalRes && it->dataRes.seqCalRes->result_mask & SEQ_CAL_RESULT_TYPE_FBRXGAIN)
         {
          
            seq_cal_fbrxgain_cal_res_type & result = (it->dataRes.seqCalRes->conn_res_data.fbrx_cal_data);

            if ( result.numberofRFMPaths > 0)
            {
               map < CRFM_Path, ftm_fbrxgain_path_result > rfm_path_map;
               for ( uint8 idx = 0; idx < result.numberofRFMPaths ; idx++)
               {
                  ftm_fbrxgain_path_result & path_res = result.pathResults [ idx ];
                  tmpQscNode.tech = static_cast < int > ( path_res.tech );
                  tmpQscNode.band = static_cast < int > ( path_res.band );
                  tmpQscNode.subBand = static_cast < int > ( path_res.subband );
                  
                  CRFM_Path rfmPath;
                  rfmPath.rfm_dev = static_cast < int > ( path_res.rfmDevice );
                  rfmPath.signal_path = static_cast < int > ( path_res.sig_path );
                  rfmPath.antenna_port = static_cast < int > ( path_res.antennaPath );

                  rfm_path_map.insert( make_pair( rfmPath, path_res ));
                  
                  bFound = true;
               }
               fbrxResults [ tmpQscNode ] = rfm_path_map;
            }
         }

      }

   }
  
   return true;
}
unsigned char RFCal::QSC::Log::RxRSBResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams )
{
   std::map < QSCNode, std::map < CRFM_Path, std::pair < uint64, ftm_rx_rsb_cal_data > > > rsbResults;
   std::map < QSCNode, int > rxrsbNVIDResults;
   unsigned char bOK = true;
   //****************************//
   // Store results
   //****************************//
   RxRSBCal::StoreResults ( hPhone, swpParams, rsbResults, rxrsbNVIDResults );
  
   //****************************//
   // Output results
   //****************************//
   bOK = RxRSBCal::OutputResults ( hPhone, swpParams, rsbResults, rxrsbNVIDResults );

   return bOK;
}

unsigned char RFCal::QSC::Log::XOResults (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams )
{
	std::map < QSCNode,  map < pair <int, int>, ftm_xo_cal_default_path_data > > xoResults;
	unsigned char bOK = true;

	//****************************//
    // Store results
    //****************************//
	XOCal::StoreResults ( hPhone, swpParams, xoResults );
      
	//****************************//
	// Output results
	//****************************//
	bOK = XOCal::OutputResults ( hPhone, swpParams, xoResults );

	return bOK;
}

unsigned char RFCal::QSC::Log::IP2Results (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams )
{
	std::map < QSCNode,  rfcommon_ip2_cal_band_data_type > ip2Results;
	std::map < QSCNode, int > ip2NVIDResults;
	unsigned char bOK = true;

	//****************************//
    // Store results
    //****************************//
	IP2Cal::StoreResults ( hPhone, swpParams, ip2Results, ip2NVIDResults );
      
	//****************************//
	// Output results
	//****************************//
	bOK = IP2Cal::OutputResults ( hPhone, swpParams, ip2Results, ip2NVIDResults );

	return bOK;
}
unsigned char RFCal::QSC::Log::RxGainResults ( HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams )
{
   std::map < QSCNode, std::vector< std::pair < CRFM_Path, seq_cal_rx_cal_path_res_type > > > rxResults;
  
   std::map < QSCNode, int > rxNVIDResults;

   //****************************//
   // Store results
   //****************************//
   RxGainCal::StoreResults( hPhone, swpParams, rxResults, rxNVIDResults );

   //****************************//
   // Output results
   //****************************//
   RxGainCal::OutputResults ( hPhone, swpParams, rxResults, rxNVIDResults );
  

   return true;
}
unsigned char RFCal::QSC::Log::CalResults ( HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams )
{
   unsigned char bOK = true;
   if ( swpParams.calType.Enable_PRx_Gain || swpParams.calType.Enable_DRx_Gain )
   {
      bOK &= RxGainResults ( hPhone, qscNode, swpParams) ;// LogRxResult
   }
   if ( swpParams.calType.Enable_FBRx_Gain )
   {
      bOK &= FBRxGainResults ( hPhone, qscNode, swpParams) ;// Log FBRx and DC Result
   }
   if ( swpParams.calType.Enable_PRx_RSB
      || swpParams.calType.Enable_DRx_RSB )
   {
      bOK &= RxRSBResults( hPhone, qscNode, swpParams); // Log RxRSB Result
   }
   if ( swpParams.calType.Enable_Push_FBRxGain_Data )
   {
      bOK &= FBRxNVResults ( hPhone, qscNode, swpParams) ;// Log FBRx and FBRxDC NVID
   }
   if ( swpParams.calType.Enable_Full_Bias_Linearizers  // Tx/Xpt Result and Xpt debug info
      || swpParams.calType.Enable_APT_Linearizers)
   {
      bOK &= TxCalResults ( hPhone, qscNode, swpParams);
   }
   if (swpParams.calType.Enable_PRx_IP2Cal || swpParams.calType.Enable_DRx_IP2Cal)
   {
	   bOK &= IP2Results (hPhone, qscNode, swpParams); // Log IP2
   }
   if ( swpParams.calType.Enable_XO_Cal)
   {
	   bOK &= XOResults (hPhone, qscNode, swpParams); // Log XO
   }

   return bOK;
}
unsigned char RFCal::QSC::CreateSwpRecords::Adjust_Serial_Rx_TE_RecordList ( HANDLE hPhone, const QSCSwpParams & swpParams, qscRecords & orgQscRecords)
{
   if ( orgQscRecords.teRecords.empty() )
   {
      return false;
   }
   vector < TE_Swp_Params >::iterator curTeItr = orgQscRecords.teRecords.begin();
   /*
   Reduce cal time.
   Change the DL level from -115 to other DL level so that we can avoid unnecessary DL change
   e.g., -115, -115, -60, -50, -40, -115, -115, -60 ---> -60, -60, -60, -50, -40, -40, -40, -60
   */
   curTeItr =  orgQscRecords.teRecords.begin();

   const double LOWEST_DL = -115.0;
   double prevDL = orgQscRecords.teRecords.rbegin()->m_dl_lvl;//LOWEST_DL;
   int prevChan = orgQscRecords.teRecords.rbegin()->m_ul_chan;
   int prevOffset = orgQscRecords.teRecords.rbegin()->m_offset_in_kHz;

   // (1) Change DL from Channel segment to the first Rx meas Segment by checking reverse order
   //  e.g., -115 (Channel X), -115, -115,-60, -50, -40, -115, -115, -115 (Channel Y)
   //  ---> -115 (Channel X), -60, -60, -60, -50, -40, -115, -115, -60 (Channel Y)

   for (vector<TE_Swp_Params>::reverse_iterator prevItr = orgQscRecords.teRecords.rbegin();
         prevItr != orgQscRecords.teRecords.rend(); prevItr++)
   {
      if (prevChan != prevItr->m_ul_chan)
      {
         prevDL = LOWEST_DL;
         prevChan = prevItr->m_ul_chan;
         prevOffset = prevItr->m_offset_in_kHz;
         continue;
      }
      if ( prevDL != prevItr->m_dl_lvl)
      {
         if (prevItr->m_dl_lvl == LOWEST_DL)
         {
            prevItr->m_dl_lvl = prevDL;
            prevItr->m_offset_in_kHz = prevOffset;
         }
         else
         {
            prevDL = prevItr->m_dl_lvl;
            prevOffset = prevItr->m_offset_in_kHz;
         }
      }
   }
   prevDL = LOWEST_DL;
   prevChan = 0;
   prevOffset = 0;

   // (2) Change DL using previous segment if the DL level is -115 by checking forward order
   //  e.g., -115 (Channel X), -115, -115,-60, -50, -40, -115, -115, -115 (Channel Y)
   //  ---> -115 (Channel X), -60, -60, -60, -50, -40, -40, -40, -60 (Channel Y)
   for (vector<TE_Swp_Params>::iterator prevItr = orgQscRecords.teRecords.begin();
         prevItr != orgQscRecords.teRecords.end(); prevItr++)
   {
      if (prevChan != prevItr->m_ul_chan)
      {
         prevDL = LOWEST_DL;
         prevChan = prevItr->m_ul_chan;
         prevOffset = prevItr->m_offset_in_kHz;
         continue;
      }
      if ( prevDL != prevItr->m_dl_lvl)
      {
         if (prevItr->m_dl_lvl == LOWEST_DL)
         {
            prevItr->m_dl_lvl = prevDL;
            prevItr->m_offset_in_kHz = prevOffset;
         }
         else
         {
            prevDL = prevItr->m_dl_lvl;
            prevOffset = prevItr->m_offset_in_kHz;
         }
      }
   }

   return true;
}
int RFCal::QSC::CreateSwpRecords::MatrixMode::Get_Index_In_BaseFrame ( HANDLE hPhone, const qscRecords & baseFrame, const cal_sweep_record & swpRecord, const TE_Swp_Params & teRecord )
{
   int index = 0;
   unsigned long FrameDuration = RFCal::QSC::GetTotalSegmentTime ( hPhone, baseFrame.swpRecords );
   vector < cal_sweep_record >::const_iterator swp_itr = baseFrame.swpRecords.begin();
   vector < TE_Swp_Params >::const_iterator te_itr = baseFrame.teRecords.begin();
   for ( ; te_itr != baseFrame.teRecords.end() && swp_itr != baseFrame.swpRecords.end(); te_itr++, swp_itr++ )
   {
      if ( ( swpRecord.seqExec.seqType == SEQ_FBRXGAIN_CAL && te_itr->m_ul_lvl == teRecord.m_ul_lvl )
         || ( swpRecord.seqExec.seqType == SEQ_MULTI_RX_CFG_MEAS && te_itr->m_dl_lvl == teRecord.m_dl_lvl )
         || ( swpRecord.seqBuild.seqType == SEQ_FBRXGAIN_CAL && swp_itr->seqBuild.seqType == SEQ_FBRXGAIN_CAL)
         || ( swpRecord.seqBuild.seqType == SEQ_MULTI_RX_CFG_MEAS && swpRecord.seqExec.seqType != SEQ_MULTI_RX_CFG_MEAS
             && swp_itr->seqBuild.seqType == SEQ_MULTI_RX_CFG_MEAS))
      {
         break;
      }
      index++;
   }
   return index;
}
unsigned char RFCal::QSC::CreateSwpRecords::MatrixMode::Find_BaseFrame ( HANDLE hPhone, const QSCSwpParams & swpParams, const qscRecords & orgQscRecords, qscRecords & baseFrame )
{
   baseFrame.swpRecords.clear();
   baseFrame.teRecords.clear();
   baseFrame.swpRecords.reserve (20);
   baseFrame.teRecords.reserve (20);

   vector < TE_Swp_Params >::const_iterator te_itr = orgQscRecords.teRecords.begin();
   vector < cal_sweep_record >::const_iterator swp_itr = orgQscRecords.swpRecords.begin();
   bool bFoundFBRxBuild = false;
   for ( ; ( te_itr !=  orgQscRecords.teRecords.end()) && ( swp_itr != orgQscRecords.swpRecords.end()); te_itr++, swp_itr++)
   {
      if ( (!bFoundFBRxBuild) && SEQ_FBRXGAIN_CAL == swp_itr->seqBuild.seqType )
      {
         bFoundFBRxBuild = true;
      }
      if ( ! bFoundFBRxBuild )
      {
         continue;
      }
      if ( bFoundFBRxBuild && SEQ_RADIO_SETUP == swp_itr->seqBuild.seqType )
      {
         baseFrame.swpRecords.push_back ( * swp_itr );
         baseFrame.teRecords.push_back (  baseFrame.teRecords.back() );
         baseFrame.teRecords.back().m_teAction = false;
         continue;
      }
      if ( bFoundFBRxBuild && SEQ_RADIO_SETUP == swp_itr->seqExec.seqType )
      {
         baseFrame.swpRecords.push_back ( * swp_itr );
         baseFrame.teRecords.push_back (  baseFrame.teRecords.back() );

         return true;
      }
      if ( bFoundFBRxBuild )
      {
         baseFrame.swpRecords.push_back ( * swp_itr );
         baseFrame.teRecords.push_back ( * te_itr );

      }
   }
   return false;
}
unsigned char RFCal::QSC::CreateSwpRecords::MatrixMode::Create_Frame_Records (  HANDLE hPhone, const QSCSwpParams & swpParams, qscRecords & orgQscRecords )
{
   qscRecords baseFrame;
   orgQscRecords.frame_teRecords.clear(); 
   // **************************************** //
   // 1. Find base frame (first channel cal)
   // **************************************** //
   if (! RFCal::QSC::CreateSwpRecords::MatrixMode::Find_BaseFrame ( hPhone, swpParams, orgQscRecords, baseFrame ))
   {
      // Failed finding baseframe;
      return false;
   }
   unsigned long FrameDuration = RFCal::QSC::GetTotalSegmentTime ( hPhone, baseFrame.swpRecords );
   unsigned long numSegmentsInFrame = baseFrame.teRecords.size();
   unsigned long segDuration = static_cast < unsigned long > ( baseFrame.teRecords.front().m_step_width );
   unsigned long numPadding = (FrameDuration - numSegmentsInFrame*segDuration)/segDuration;
   for ( unsigned long i = 0; i < numPadding; i++ )
   {
      baseFrame.teRecords.push_back ( baseFrame.teRecords.back() );
   }
   numSegmentsInFrame = baseFrame.teRecords.size();

   vector < TE_Swp_Params > tempTeRecords;
   tempTeRecords.reserve ( 60 );

   vector < TE_Swp_Params >::iterator te_itr = orgQscRecords.teRecords.begin();
  
   // **************************************** //
   // 2. Find the first TE Action
   // **************************************** //
   int segNumFirstFBRx = 0;
   for ( ;  te_itr !=  orgQscRecords.teRecords.end() ; te_itr++)
   {
      if ( te_itr->m_teAction )
      {
         break;
      }
      // **************************************** //
      // Construct records for the first fram
      // **************************************** //
      tempTeRecords.push_back ( * te_itr );
   }
   // **************************************** //
   // 2. Register First frame
   // **************************************** //
   orgQscRecords.frame_teRecords[0] =  tempTeRecords;
   

   // **************************************** //
   // 3. Create frames per baseFrame size
   // **************************************** //
   unsigned long count = 0;
   for ( ;  te_itr !=  orgQscRecords.teRecords.end() ; te_itr++)
   {
      if ( count % numSegmentsInFrame == 0)
      {
         if ( count != 0 )
         {
            // Register
            orgQscRecords.frame_teRecords [ count / numSegmentsInFrame ] = tempTeRecords;
         }
         tempTeRecords.clear();
         tempTeRecords.reserve ( numSegmentsInFrame );
      }
      tempTeRecords.push_back ( *te_itr);
      count++;
   }
   if ( count % numSegmentsInFrame != 0)
   {
      // Register
      orgQscRecords.frame_teRecords [ count / numSegmentsInFrame + 1] = tempTeRecords;
   }

   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::MatrixMode::Pad_Nops ( HANDLE hPhone, const QSCSwpParams & swpParams, qscRecords & orgQscRecords )
{
   qscRecords baseFrame;

   // **************************************** //
   // 1. Find base frame (first channel cal)
   // **************************************** //
   if (! RFCal::QSC::CreateSwpRecords::MatrixMode::Find_BaseFrame ( hPhone, swpParams, orgQscRecords, baseFrame ))
   {
      // Failed finding baseframe;
      return false;
   }
   unsigned long FrameDuration = RFCal::QSC::GetTotalSegmentTime ( hPhone, baseFrame.swpRecords );
   unsigned long segDuration = static_cast < unsigned long > ( baseFrame.teRecords.front().m_step_width );
   unsigned long numFrames = FrameDuration/segDuration;//baseFrame.teRecords.size();
  
   qscRecords newRecords;
   newRecords.swpRecords.reserve ( orgQscRecords.swpRecords.size() * 2 );
   newRecords.teRecords.reserve ( orgQscRecords.teRecords.size() * 2 );

   vector < TE_Swp_Params >::iterator te_itr = orgQscRecords.teRecords.begin();
   vector < cal_sweep_record >::const_iterator swp_itr = orgQscRecords.swpRecords.begin();
  
   // **************************************** //
   // 2. Create new records that forms matrix by padding with Nop
   // **************************************** //
   // Find the first FBRx Build segment
   int segNumFirstFBRx = 0;
   for ( ; ( te_itr !=  orgQscRecords.teRecords.end()) && ( swp_itr != orgQscRecords.swpRecords.end()); te_itr++, swp_itr++)
   {
      if ( SEQ_FBRXGAIN_CAL == swp_itr->seqBuild.seqType )
      {
         segNumFirstFBRx = static_cast <int> ( std::distance ( orgQscRecords.teRecords.begin(), te_itr) ); 
         // Push back this FBRx Build segment
         //newRecords.swpRecords.push_back ( * swp_itr );
         //newRecords.teRecords.push_back ( * te_itr );
         break;
      }
      // **************************************** //
      // Construct new records
      // **************************************** //
      newRecords.swpRecords.push_back ( * swp_itr );
      newRecords.teRecords.push_back ( * te_itr );
   }

   // Check each of segment and location (index) and add Nop if necessary
   unsigned long currentSegDuration = 0;
   for ( ; ( te_itr !=  orgQscRecords.teRecords.end()) && ( swp_itr != orgQscRecords.swpRecords.end()); te_itr++, swp_itr++)
   {
      if (  te_itr->m_teAction 
         || SEQ_FBRXGAIN_CAL == swp_itr->seqBuild.seqType   
         || ( SEQ_MULTI_RX_CFG_MEAS == swp_itr->seqBuild.seqType && SEQ_MULTI_RX_CFG_MEAS != swp_itr->seqExec.seqType ))
      {
         // ********************************** //
         // Calculate num of NoPs for padding
         // ********************************** //
         unsigned long index = static_cast < unsigned long > ( RFCal::QSC::CreateSwpRecords::MatrixMode::Get_Index_In_BaseFrame ( hPhone, baseFrame, *swp_itr, *te_itr ) );
         unsigned long current_index = ( currentSegDuration % FrameDuration ) / segDuration;

         unsigned long  numNopCode = 0;
         if ( current_index > index )
         {
            numNopCode = numFrames - current_index + index;
         }
         else if ( current_index <= index )
         {
            numNopCode = index - current_index;
         }
         // ********************************** //
         // Add Nop Code for padding
         // ********************************** //
         for ( unsigned long i=0; i < numNopCode; i++)
         {
            newRecords.swpRecords.push_back ( * swp_itr );
            newRecords.teRecords.push_back ( * te_itr );
            RFCal::SetOpCode::NoP ( hPhone, newRecords.swpRecords.back(), static_cast < unsigned short > ( segDuration ) );
            newRecords.teRecords.back().m_teAction = false;
            currentSegDuration += segDuration;
         }
      }
      currentSegDuration += static_cast < unsigned long> (swp_itr->opcode.segLen );
      newRecords.swpRecords.push_back ( * swp_itr );
      newRecords.teRecords.push_back ( * te_itr );
   }
   // ********************************** //
   // allocated memory for seqCalRes is copied to newRecords.
   // Set NULL in case of free.
   // ********************************** //
   
   for ( vector < cal_sweep_record >::iterator swp_itr = orgQscRecords.swpRecords.begin(); swp_itr != orgQscRecords.swpRecords.end() ; swp_itr ++ )
   {
      // allocated memory for seqCalRes is copied to newRecords.
      // Set NULL in case of free.
      if ( swp_itr->dataRes.seqCalRes )
      {
         swp_itr->dataRes.seqCalRes = NULL;
      }
   }
   // ********************************** //
   // Replace the qsc record with the newly created matrix qsc records.
   // ********************************** //

   orgQscRecords = newRecords;
   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::TransformToSerialRxCal ( HANDLE hPhone, const QSCSwpParams & swpParams, qscRecords & orgQscRecords )
{
   // ********************************** //
   // Transform to Serial Cal sequence 
   // 1. Remove FBRx
   // 2. Make two segments: one is Build and the other one is Exec
   //    No parallel. (i.e., one segment must not have both Build and Exec segment
   // ********************************** //

   vector < cal_sweep_record >::iterator it = orgQscRecords.swpRecords.begin();
   vector < TE_Swp_Params >::iterator curTeItr = orgQscRecords.teRecords.begin();

   if(it == orgQscRecords.swpRecords.end() || curTeItr == orgQscRecords.teRecords.end())
   {
      return false;
   }
   vector < cal_sweep_record > tmpSwp;
   vector < TE_Swp_Params > tmpTE;

   /* reserve memory for performance */
   tmpSwp.reserve(orgQscRecords.swpRecords.size());
   tmpTE.reserve(orgQscRecords.teRecords.size());

   /*
   Modify sweep record for serial Rx cal.
   1. Remove or disable FBRx segments
   2. Separate RxBuild and Rx Execute segment (since they are different DL level)
   3. Set same DL level for Rx Build and Rx Execute segment since they need to be exeucted in the same sweep.
   */
   word teStepDuration = static_cast<word>(orgQscRecords.teRecords.front().m_step_width);
   for (; ( it !=  orgQscRecords.swpRecords.end() && curTeItr != orgQscRecords.teRecords.end()) ; it++, curTeItr++)
   {

      // Increment CurTeItr
      // If end of pointer after increament, then, use the last value
      if( curTeItr == orgQscRecords.teRecords.end())
         curTeItr = orgQscRecords.teRecords.end() - 1;

      /* Case: FBRx Bld or Execute only --> Remove */
      if (  (it->seqBuild.seqType == SEQ_FBRXGAIN_CAL && it->seqExec.seqType == SEQ_TYPE_UNDEFINED)
            || (it->seqBuild.seqType == SEQ_TYPE_UNDEFINED && it->seqExec.seqType == SEQ_FBRXGAIN_CAL))
      {

         continue; // remove
      }
      /* Case: FBRx Build and Other Exec --> Remove FBRx */
      if (it->seqBuild.seqType == SEQ_FBRXGAIN_CAL)
      {
         it->seqBuild.seqType = SEQ_TYPE_UNDEFINED;
         it->seqBuild.handle = 0;
         it->opcode.varLenOpcode.opCodeGrp.opCodeGrp0 &= ~(static_cast<qword>(0x40000));
      }
      /* Case: FBRx Exec and Other Build --> Remove FBRx */
      if (it->seqExec.seqType == SEQ_FBRXGAIN_CAL)
      {
         it->seqExec.seqType = SEQ_TYPE_UNDEFINED;
         it->measSeg = false;
         it->seqExec.handle = 0;
         it->opcode.varLenOpcode.opCodeGrp.opCodeGrp0 &= ~(static_cast<qword>(0x80000));
      }

      /* Case: Remove TX device from Radio Setup */
      if (it->seqBuild.radioSetupParams.radioSetupV0.numRFMPaths > 0)
      {
         for (UINT i=0; i < 16; i++)
         {
            if (it->seqBuild.radioSetupParams.radioSetupV0.radioSetupPathParams[i].rfmPathType == RFM_PATH_TYPE_TX
                  && it->seqBuild.radioSetupParams.radioSetupV0.radioSetupPathParams[i].numChannels > 0)
            {
               it->seqBuild.radioSetupParams.radioSetupV0.numRFMPaths--;
               it->seqBuild.radioSetupParams.radioSetupV0.radioSetupPathParams[i].rfmPathType = RFM_PATH_TYPE_INVALID;
            }
         }
      }

      /* Case: Rx Build and Rx Exec case */
      /* Separate into two segments.
      1. Rx Exec. Use the current TE segment
      2. Rx Build. Use the next TE segment */
      if (it->seqBuild.seqType == SEQ_MULTI_RX_CFG_MEAS && it->seqExec.seqType == SEQ_MULTI_RX_CFG_MEAS) // if build Rx, then use the next segment
      {
         // Add two segments (1. SeqExec only, 2nd: SebBuild Only (TE segment is from the next segmnet)
         // 1st Segment
         tmpSwp.push_back(*it);
         if ( curTeItr != orgQscRecords.teRecords.end())
         {
            tmpTE.push_back(*curTeItr);
         }
         // Modify, Remove SeqBuild
         tmpSwp.back().opcode.varLenOpcode.opCodeGrp.opCodeGrp0 &= ~(static_cast<qword>(0x40000));  // Remove Build
         tmpSwp.back().seqBuild.seqType = 0;

         // 2nd Segment
         tmpSwp.push_back(*it);
         tmpSwp.back().opcode.varLenOpcode.opCodeGrp.opCodeGrp0 &= ~(static_cast<qword>(0x80000)); // Remove Exec
         tmpSwp.back().seqExec.seqType = SEQ_TYPE_UNDEFINED;
         if ((curTeItr+1) != orgQscRecords.teRecords.end())
            tmpTE.push_back(*(curTeItr+1));
         else
         {
            if ( curTeItr != orgQscRecords.teRecords.end())
            {
               tmpTE.push_back(*curTeItr);
            }
         }
      }
      /* Case: RxBuild in this segment and RxExec in the next segment
      Use the next TE segment to execute RxBuild and RxExec in the same sweep.
      */
      else if (it->seqBuild.seqType == SEQ_MULTI_RX_CFG_MEAS)
      {
         tmpSwp.push_back(*it);
         if ((curTeItr+1) != orgQscRecords.teRecords.end())
            tmpTE.push_back(*(curTeItr+1));
         else
         {
            if ( curTeItr != orgQscRecords.teRecords.end())
            {
               tmpTE.push_back(*curTeItr);
            }
         }
      }
      /* Other case: Store the segment */
      else
      {
         tmpSwp.push_back(*it);
         tmpTE.push_back(*curTeItr);
      }

      // Case: DUT and Callbox has different segment duration:
      // E.g., Radio Setup 20000 us.  Callbox has 4000 us.
      // Increase Callbox Segment by (20000 - 4000)
      //int size = std::distance ( orgQscRecords.teRecords.begin(), curTeItr);
      ////while (stepSize > 0 && curTeItr != orgQscRecords.teRecords.end())
      ////{
      ////   curTeItr++;
      ////   stepSize--;
      ////}
      //if ( curTeItr == orgQscRecords.teRecords.end())
      //{
      //   curTeItr--;
      //}
   }

   // if all the DUT sweep records are not stored, then, store all.
   if (it != orgQscRecords.swpRecords.end())
   {
      for (; it != orgQscRecords.swpRecords.end(); it++)
         tmpSwp.push_back(*it);
   }
   // Replace the original sweep record.
   orgQscRecords.swpRecords = tmpSwp;
   orgQscRecords.teRecords = tmpTE;

   for ( vector < cal_sweep_record >::iterator itr = orgQscRecords.swpRecords.begin(); itr != orgQscRecords.swpRecords.end(); itr++)
   {
      QLIB_FTM_GENERATE_ESC_OPCODE ( NULL, &( * itr ) );
   }

   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::ConnectedCal(HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams)
{
   swpParams.swps.clear();
   unsigned char status = true;

   // *************************************** //
   // Initialize segments
   // *************************************** //

   const unsigned int NUM_SEGMENTS = 1000;
   swpParams.swps.clear();
   swpParams.swps.resize(1);
   qscRecords & qsc_record = swpParams.swps.back();
   
   qsc_record.swpRecords.reserve( NUM_SEGMENTS );
   qsc_record.teRecords.reserve( NUM_SEGMENTS );

   //if (  swpParams.calType.Enable_FBRx_Gain 
   //   || swpParams.calType.Enable_DRx_Gain 
   //   || swpParams.calType.Enable_PRx_Gain 
   //   )
   {
      // FBRx + RX or FDRSBCal
      status = RFCal::QSC::CreateSwpRecords::FBRx_Rx_Cal(hPhone, qsc_node, swpParams);
   }

   return status;
}
unsigned char RFCal::QSC::CreateSwpRecords::UnconnectedCal(HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams)
{
   swpParams.swps.clear();
   unsigned char status = true;

   // *************************************** //
   // Initialize segments
   // *************************************** //

   const unsigned int NUM_SEGMENTS = 1000;
   swpParams.swps.clear();
   swpParams.swps.resize(1);
   qscRecords & qsc_record = swpParams.swps.back();
   
   //RFCal::QSC::SetOpCode::InitSwpRecords(hPhone, qsc_record.swpRecords, NUM_SEGMENTS);
   qsc_record.swpRecords.reserve( NUM_SEGMENTS );
   qsc_record.teRecords.reserve( NUM_SEGMENTS );

   if (  swpParams.calType.Enable_APT_Linearizers 
      || swpParams.calType.Enable_Full_Bias_Linearizers 
      || swpParams.calType.Enable_APT_FComp 
      || swpParams.calType.Enable_Tx_XPT 
      || swpParams.calType.Enable_XPT_FComp 
      )
   {
      // Tx Cal
      status = RFCal::QSC::CreateSwpRecords::Tx_Xpt_Cal(hPhone, qsc_node, swpParams);
   }

   // IP2 Cal
   else if ( swpParams.calType.Enable_PRx_IP2Cal || swpParams.calType.Enable_DRx_IP2Cal)
   {
	   status = RFCal::QSC::CreateSwpRecords::IP2_Cal(hPhone, qsc_node, swpParams);
   }

   return status;
}
unsigned char RFCal::QSC::CreateSwpRecords::XoCal(HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams)
{
	swpParams.swps.clear();
	unsigned char status = true;

	// *************************************** //
	// Initialize segments
	// *************************************** //

	const unsigned int NUM_SEGMENTS = 1000;
	swpParams.swps.clear();
	swpParams.swps.resize(1);
	qscRecords & qsc_record = swpParams.swps.back();
	CBandInfoPtr perPathBandInfo;
   
	qsc_record.swpRecords.reserve( NUM_SEGMENTS );
	qsc_record.teRecords.reserve( NUM_SEGMENTS );

	qscRecords & qsc_records = swpParams.swps[0];
	std::vector < cal_sweep_record >::iterator swpIt = qsc_records.swpRecords.begin();
	std::vector <TE_Swp_Params>::iterator teIt = qsc_records.teRecords.begin();
	unsigned long handle_id = 0;
	      
	// ************************************** //
    // Get RFM path for this band
    // ************************************** //
	
	std::vector <ACTION_TYPE> actionType;
	actionType.push_back (ACTION_TYPE_CAL);

	std::vector <PATH_TYPE> pathType;
	pathType.push_back (PATH_TYPE_PRX);
      
	std::vector < CRFPathInformationPacket > aggregatedResult;

	if ( !RFCal::QSC::GetCalibratedRFMPathInfo( hPhone, swpParams, *swpParams.BandInfo.begin(), aggregatedResult, actionType, pathType))
    {
		// Failed
		return false;

    }
	
	// check if aggregatedResult[0] is NULL?
	GetPerPathParam( hPhone, *swpParams.BandInfo.begin(), perPathBandInfo, *aggregatedResult.begin());
	
	ftm_radio_setup_type setup_type = RADIO_SETUP_TECH_BAND_PATH_CHAN;
	
	const int chan = swpParams.BandInfo[0].commonParams.Channel_List[0];
	
	vector <int> bwList;
	bwList = *(perPathBandInfo.commonParams->Tx_Rx_BW_Type.begin());

	RFCal::QSC::CreateSwpRecords::Add::RadioSetup (hPhone, handle_id, setup_type, swpParams, perPathBandInfo, aggregatedResult[0], chan, bwList); 

	RFCal::QSC::CreateSwpRecords::Add::XO ( hPhone, handle_id, swpParams, perPathBandInfo, chan );

	RFCal::QSC::CreateSwpRecords::Add::Teardown ( hPhone, handle_id, swpParams, perPathBandInfo);

	RFCal::QSC::CreateSwpRecords::Add::Data_ResultNV (hPhone, swpParams, perPathBandInfo, static_cast <unsigned char> (TRUE));

   return status;
}
unsigned char RFCal::QSC::Execute_PushFBRxGain_In_ConnectedCal (HANDLE hPhone, const QSCNode & qscNode, QSCSwpParams & swpParams )
{
   if ( (SEQ_CAL_CONNECTED != swpParams.sweep_type) || !swpParams.calType.Enable_Push_FBRxGain_Data  )
   {
      return true;
   }
   unsigned char status = true;
   unsigned char bFound = false;
   // **************************************************************** //
   // Check Enable_Push_FBRxGain_Data is true or not
   // **************************************************************** //   
   for ( vector < CBandInfo >::const_iterator band_itr = swpParams.BandInfo.begin();
      band_itr != swpParams.BandInfo.end(); band_itr++ )
   {
      if ( band_itr->calType.Enable_NV_Write 
         && band_itr->calType.Enable_FBRx_Gain )
      {
         bFound = true;
         break;
      }
   }
   if ( !bFound ) 
   {
      return true;
   }

   // **************************************************************** //
   // Create sweep records to push FBRx data
   // **************************************************************** //   

   if ( RFCal::QSC::CreateSwpRecords::PushFBRxGain_In_ConnectedCal ( hPhone, qscNode, swpParams ) )
   {
      // **************************************************************** //
      // Execute DUT sweep records
      // **************************************************************** //
      RFCal::QSC::ExecuteQSCSweepRecords ( hPhone, swpParams.swps.back().swpRecords, 
            ( ! swpParams.calType.Skip_Radio_TearDown ), 
            true /* debug mode */,
            ( swpParams.defaultParams.commonParams.En_Compress_Esc_Req ) );

      // **************************************************************** //
      // Calculate Wait time
      // **************************************************************** //
      uint32 iTotalSegDuration =  ( RFCal::QSC::GetTotalSegmentTime (hPhone, swpParams.swps.back().swpRecords) ) / 1000 + 20;

      if ( swpParams.BandInfo.back().commonParams.En_Compress_Esc_Req )
         iTotalSegDuration += 20;
      if ( swpParams.BandInfo.back().commonParams.En_Compress_Esc_Res )
         iTotalSegDuration += 20;
               
      Sleep(iTotalSegDuration);

      // **************************************************************** //
      // Get DUT sweep results
      // **************************************************************** //
      RFCal::QSC::ExecuteGetSweepResult ( hPhone, swpParams.swps.back().swpRecords, swpParams.defaultParams.commonParams.En_Compress_Esc_Res );          
 
      // **************************************************************** //
      // Logging FBRx NVIDs
      // **************************************************************** //
      RFCal::QSC::Log::FBRxNVResults ( hPhone, qscNode, swpParams );

      // ************************************************ //
      // Deallocation of memory
      // ************************************************ //
      RFCal::QSC::CreateSwpRecords::DeallocateSweepRecords( hPhone, qscNode, swpParams );
   }

   return status;
}
      
unsigned char RFCal::QSC::TE_Fetch_Result (HANDLE hPhone, vector < cal_sweep_record> & swpRecords, std::vector <TE_Swp_Params> teRecords)
{
   RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();
   if ( NULL == pMgr )
   {
      return false;
   }

   unsigned char bStatus = 0;
   // Get Callbox Result
   int measTime = RFCal::QSC::GetTotalSegmentTime (hPhone, swpRecords)/1000;
   if (pMgr->Wait_For_Tx_Sweep_To_Complete( measTime ))
   {
      vector < double > data;
      pMgr->Fecth_Sweep_Result ( data );
      if (data.empty())
         return false;

      // first find out Tx trigger
      if ( teRecords.empty () )
      {
         return false;
      }
      vector <  TE_Swp_Params >::iterator te_it = teRecords.begin();
      unsigned long te_config_duration = 0;
      unsigned long te_meas_duration = static_cast < unsigned long > ( te_it->m_step_width );
      for (; te_it != teRecords.end(); te_it++)
      {
         if ( te_it->m_teAction )
            break;
         te_config_duration += static_cast < unsigned long > ( te_it->m_step_width );
      }

      if ( te_it == teRecords.end() ) //if not found
      {
         return false;
      }
      // Find 
      vector < cal_sweep_record>::iterator swp_it = swpRecords.begin();
      unsigned long dut_config_duraiton = 0;
      for (; swp_it != swpRecords.end(); swp_it++ )
      {
         if ( dut_config_duraiton == te_config_duration)
            break;
         dut_config_duraiton += static_cast <unsigned long> (swp_it->opcode.segLen);
      }

      if ( swp_it == swpRecords.begin() )
      {
         return false;
      }

      vector < double >::iterator data_it =  data.begin();
      
      for (; (swp_it != swpRecords.end()) && (data_it != data.end()) ; swp_it++, data_it++ )
      {
         swp_it->txRes.txPower = ( * data_it );
         if ( swp_it->opcode.segLen == te_meas_duration)
            continue;

         if ( swp_it->opcode.segLen < te_meas_duration )
         {
            return false;
         }

         uint32 numLoops = static_cast <uint32> ( swp_it->opcode.segLen / te_meas_duration - 1);
         
         for (uint32 i = 0; i < numLoops; i++)
         {
            if ( data_it == data.end())
               continue;
            data_it++;
            if ( data_it == data.end())
            {
               return bStatus;
            }
         }
      }
      //
   }

   return bStatus;
}
unsigned char RFCal::QSC::CreateSwpRecords::Store_Calibrated_RFM_Path_List ( HANDLE hPhone, const CBandInfoPtr & perPathBandInfo, const std::vector < CRFPathInformationPacket > & rfm_path_list,  
            std::set < CRFM_Path > & calibrated_rfm_path_list)
{
   if ( NULL ==  perPathBandInfo.calType)
   {
      return false;
   }
   const CCalType & calType =  *(perPathBandInfo.calType);
   for (  vector < CRFPathInformationPacket >::const_iterator itr = rfm_path_list.begin();
      itr != rfm_path_list.end(); itr++)
   {
      if ( PATH_TYPE_TX == itr->rfmPathType )
      {
         continue;
      }
      CRFM_Path rfm_path;
      rfm_path.signal_path = static_cast < int > ( itr->signalPath );
      rfm_path.rfm_dev = static_cast < int > ( itr->rfmDevice );
      rfm_path.antenna_port = static_cast < int > ( itr->antennaPath );

      if ( PATH_TYPE_PRX == itr->rfmPathType && calType.Enable_PRx_Gain )
      {
         calibrated_rfm_path_list.insert ( rfm_path );
      }
      if ( PATH_TYPE_DRX == itr->rfmPathType && calType.Enable_DRx_Gain )
      {
         calibrated_rfm_path_list.insert ( rfm_path );
      }
   }
   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::Check_RFM_Path_Not_Calibrated ( HANDLE hPhone, 
      const std::vector < CRFPathInformationPacket > & rfm_path_list,  
      const std::set < CRFM_Path > & calibrated_rfm_path_list)
{
   unsigned char calibrated = true;
   for (  vector < CRFPathInformationPacket >::const_iterator itr = rfm_path_list.begin();
        itr != rfm_path_list.end(); itr++)
   {
      if ( itr->rfmPathType == 2 ) 
      {
         continue;
      }
      CRFM_Path rfm_path;
      rfm_path.signal_path = static_cast < int > ( itr->signalPath );
      rfm_path.rfm_dev = static_cast < int > ( itr->rfmDevice );
      rfm_path.antenna_port = static_cast < int > ( itr->antennaPath );

     if (  calibrated_rfm_path_list.end() == calibrated_rfm_path_list.find ( rfm_path ) )
     {
        calibrated = false;
        break;
     }
   }

   return !calibrated;
}
unsigned char RFCal::QSC::CreateSwpRecords::DeallocateSweepRecords ( HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams)
{
   for ( vector < qscRecords >::iterator qsc_record_it = swpParams.swps.begin(); qsc_record_it != swpParams.swps.end(); qsc_record_it++)
   {
      for ( vector < cal_sweep_record >::iterator it = qsc_record_it->swpRecords.begin(); it != qsc_record_it->swpRecords.end(); it++)
      {
         if (NULL != it->dataRes.seqCalRes)
         {
    		if ( NULL != it->dataRes.seqCalRes->unconn_res_data.ip2_cal_band_data  )
			{
				delete it->dataRes.seqCalRes->unconn_res_data.ip2_cal_band_data;
				it->dataRes.seqCalRes->unconn_res_data.ip2_cal_band_data = NULL;
			}
    
            delete it->dataRes.seqCalRes;
            it->dataRes.seqCalRes = NULL;
         }
         if (NULL != it->data.fbrxAgcTbl.ptr_fbrx_cal_data)
         {
            delete it->data.fbrxAgcTbl.ptr_fbrx_cal_data;
            it->data.fbrxAgcTbl.ptr_fbrx_cal_data = NULL;
         }
      }
      qsc_record_it->swpRecords.clear();
      qsc_record_it->teRecords.clear();
   }
   swpParams.swps.clear();

   return true;
}

unsigned char RFCal::QSC::CreateSwpRecords::FBRx_Rx_Cal(HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams)
{
   unsigned char status = true;
   
   qscRecords & qsc_records = swpParams.swps[0];
   std::vector < cal_sweep_record >::iterator swpIt = qsc_records.swpRecords.begin();
   std::vector <TE_Swp_Params>::iterator teIt = qsc_records.teRecords.begin();
   unsigned long handle_id = 0;

   // ************************************** //
   // Data Rx Char if data available
   // ************************************** //
   unsigned char bRxChar = false;
   bRxChar = RFCal::QSC::CreateSwpRecords::Add::Data_PushRxChar( hPhone, qsc_node, swpParams );

   // ************************************** //
   // Band Loop
   // ************************************** //
   for ( std::vector < CBandInfo >::iterator bandIt = swpParams.BandInfo.begin(); bandIt != swpParams.BandInfo.end(); bandIt++)
   {
      // ************************************** //
      // Get RFM path for this band
      // ************************************** //
      
      std::vector < ACTION_TYPE > actionType;
      if ( bandIt->calType.Enable_PRx_RSB || bandIt->calType.Enable_DRx_RSB )
      {
         actionType.push_back( ACTION_TYPE_RSB_CAL );
      }
      else
      {
      actionType.push_back( ACTION_TYPE_CAL );
         if ( bandIt->calType.Enable_Offset_Calibration )
            actionType.push_back( ACTION_TYPE_OFFSET_CAL );
      }
      std::vector < CRFPathInformationPacket > aggregatedResult;
      std::vector < PATH_TYPE > pathType;

      if (! ( bandIt->calType.Enable_PRx_RSB || bandIt->calType.Enable_DRx_RSB ))
      {
      pathType.push_back ( PATH_TYPE_TX );
      }
      if ( bandIt->calType.Enable_PRx_Gain || bandIt->calType.Enable_PRx_RSB)
         pathType.push_back ( PATH_TYPE_PRX );
      if ( bandIt->calType.Enable_DRx_Gain || bandIt->calType.Enable_DRx_RSB)
         pathType.push_back ( PATH_TYPE_DRX );

      if (! RFCal::QSC::GetCalibratedRFMPathInfo( hPhone, swpParams, * bandIt, aggregatedResult, actionType, pathType) )
      {
         // Failed...
         continue;
      }
       
      
      std::set < CRFM_Path > calibrated_rfm_path_list;

      // ************************************** //
      // RFM path loop - Aggregated RFM paths
      // ************************************** //
      std::vector < CRFPathInformationPacket >::iterator aggr_it = aggregatedResult.begin();
      CBandInfoPtr perPathBandInfo;
      for( ; aggr_it != aggregatedResult.end(); aggr_it++)
      {
        // if ( aggr_it != aggregatedResult.begin()) continue;
         // ************************************** //
         // Get per Path parameter
         // ************************************** //
         GetPerPathParam( hPhone, (*bandIt), perPathBandInfo, (*aggr_it));

         // ************************************** //
         // RFM path list to calibrate
         // ************************************** //
         vector < CRFPathInformationPacket > rfm_path_list;
         RFCal::QSC::GetSortedRFMPathVector( hPhone, swpParams, perPathBandInfo, *aggr_it, rfm_path_list);

         const CCalType & calType = ( perPathBandInfo.calType )? *(perPathBandInfo.calType) : (*bandIt).calType;
         const CCommonParam & commonParams = ( perPathBandInfo.commonParams )? *(perPathBandInfo.commonParams) : (*bandIt).commonParams;
         
         const std::vector < int > & channels =  commonParams.Channel_List;

         // ************************************** //
         // Channel Loops
         // ************************************** //
         for ( std::vector < int >::const_iterator chan_it = channels.begin();  chan_it != channels.end(); chan_it++)
         {
            // ************************************** //
            // Radio Setup Type
            // - RADIO_SETUP_TECH_BAND_PATH_CHAN- Complete setup (for 1 st channel )
            // - RADIO_SETUP_PATH_CHAN - RFM Path change
            // - RADIO_SETUP_CHAN - Retune (Channel change )
            // ************************************** //
            for ( std::vector < vector < int > >::const_iterator bwList_it = commonParams.Tx_Rx_BW_Type.begin();
               bwList_it !=  commonParams.Tx_Rx_BW_Type.end(); bwList_it++ )
            {
            ftm_radio_setup_type setup_type = RADIO_SETUP_CHAN;
               if (  bwList_it == commonParams.Tx_Rx_BW_Type.begin())
               {
            if (channels.begin() == chan_it )
            {
               if (aggregatedResult.begin() == aggr_it)
               {
                  if ( swpParams.BandInfo.begin() == bandIt) 
                     setup_type = RADIO_SETUP_TECH_BAND_PATH_CHAN;
                  else
                     setup_type = RADIO_SETUP_BAND_PATH_CHAN;
               }
               else
               {
                 setup_type = RADIO_SETUP_PATH_CHAN;             
               }
            } 
                  else
                  {
                      setup_type = RADIO_SETUP_CHAN;
                  }
               }
               else
               {
                  // ********************************************************* //
                  // Radio Setup - Tear down for each of BW path change
                  // ********************************************************* //
                  RFCal::QSC::CreateSwpRecords::Add::Teardown ( hPhone, handle_id, swpParams, perPathBandInfo);
                  handle_id++;
                  setup_type = RADIO_SETUP_BANDWIDTH;
               }
               RFCal::QSC::CreateSwpRecords::Add::RadioSetup (hPhone, handle_id, setup_type, swpParams, perPathBandInfo, (*aggr_it), ( *chan_it ), ( *bwList_it ) ); 
            
            // ************************************** //
            // FBRx Segments  (this may be used as trigger even if there is FBRx measurement)
            // ************************************** //
            if ( PATH_TYPE_TX == aggr_it->rfmPathType )
            {
               //
                  if ( calType.Enable_FBRx_Gain /* (1) FBRx Gain case */
                     || ( ( channels.begin() == chan_it ) && ( aggregatedResult.begin() == aggr_it) && ( swpParams.BandInfo.begin() == bandIt ))
                     /* (2) No FBRx Gal cal. but FBRx is used as trigger. Then, first channel at the first RFM path in the first band only */ )
               {
                  RFCal::QSC::CreateSwpRecords::Add::FBRxGain (hPhone, handle_id, swpParams, perPathBandInfo, (*aggr_it), ( *chan_it )); 
               }
            }
            // ************************************** //
            // Rx Segments 
            // ************************************** //
            if ( calType.Enable_PRx_Gain || calType.Enable_DRx_Gain )
            {
               // Check the rfm path list to see the path has already calibrated
               if ( RFCal::QSC::CreateSwpRecords::Check_RFM_Path_Not_Calibrated ( hPhone, 
                     rfm_path_list, calibrated_rfm_path_list) )
               {
                  RFCal::QSC::CreateSwpRecords::Add::RxGain ( hPhone, handle_id, swpParams, perPathBandInfo, (*aggr_it), ( *chan_it ), rfm_path_list); 
               }
            }

               // ************************************** //
               // FDRSB Segments 
               // ************************************** //
               if ( calType.Enable_PRx_RSB || calType.Enable_DRx_RSB )
               {
                  RFCal::QSC::CreateSwpRecords::Add::FdRSB ( hPhone, handle_id, swpParams, perPathBandInfo, (*aggr_it), ( *chan_it ) ); 
               }
            }
        } 

         // ********************************************************* //
         // Radio Setup - Tear down for each of RFM path change
         // ********************************************************* //
         RFCal::QSC::CreateSwpRecords::Add::Teardown ( hPhone, handle_id, swpParams, perPathBandInfo);
         handle_id++;

         // ********************************************************* //
         // Store calibrated RFM path info to avoid duplicate calibration (Multi Tx path case )
         // ********************************************************* //
         RFCal::QSC::CreateSwpRecords::Store_Calibrated_RFM_Path_List ( hPhone, perPathBandInfo, rfm_path_list, calibrated_rfm_path_list);
      }
      // ********************************************************* //
      // Data Opcode segments - For each of bands
      // ********************************************************* //
      RFCal::QSC::CreateSwpRecords::Add::Data_ResultNV ( hPhone, swpParams, perPathBandInfo);
   }

   if ( bRxChar && qsc_records.teRecords.size() >= 2 )
   {
      // If rx char segment is inserted, then, copy equipment data from Radio Setup (segment 1) to Rx Char ( segment 0 )
      qsc_records.teRecords[0] = qsc_records.teRecords[1];
   }
   return status;
}
unsigned char RFCal::QSC::CreateSwpRecords::PushFBRxGain_In_ConnectedCal (HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams )
{
   unsigned char status = false;


   // *************************************** //
   // Initialize segments
   // *************************************** //

   const unsigned int NUM_SEGMENTS = 10;
   swpParams.swps.clear();
   swpParams.swps.resize(1);
   qscRecords & qsc_records = swpParams.swps.back();
   qsc_records.swpRecords.reserve( NUM_SEGMENTS );
   std::vector < cal_sweep_record >::iterator swpIt = qsc_records.swpRecords.begin();
 
   // ************************************** //
   // Get RFM path for this band
   // ************************************** //      
   std::vector < ACTION_TYPE > actionType;
   actionType.push_back( ACTION_TYPE_CAL );

   std::vector < CRFPathInformationPacket > aggregatedResult;
   aggregatedResult.clear();

   if ( swpParams.BandInfo.empty () )
   {
      return status; // Critial parameter reading error
   }

   for ( vector < CBandInfo >::const_iterator band_itr = swpParams.BandInfo.begin();
      band_itr != swpParams.BandInfo.end(); band_itr++ )
   {
      if ( band_itr->calType.Enable_NV_Write 
         && band_itr->calType.Enable_FBRx_Gain )
      {
         std::vector < PATH_TYPE > pathType;
         pathType.push_back ( PATH_TYPE_TX );

         if (! RFCal::QSC::GetCalibratedRFMPathInfo( hPhone, swpParams, (*band_itr), aggregatedResult, actionType, pathType) )
         {
            // Failed...
            return false;
         }


         if ( qsc_node.tech != TECHNOLOGY_GSM && aggregatedResult.empty() )
         {
            continue; // if there is no Tx path such as LTE B29. 
         }
         // ************************************** //
         // Data - PushFBRxGain Result
         // ************************************** //
         QSCNode qsc_node_band;
         qsc_node_band.tech = qsc_node.tech;
         qsc_node_band.band = band_itr->band;
         qsc_node_band.subBand = band_itr->subBand;

         std::map < CRFM_Path, ftm_fbrxgain_path_result > fbrx_rfmPath_ResultMap;
         swpParams.repoPtr->GetFBRxResult ( hPhone, qsc_node_band, fbrx_rfmPath_ResultMap );
         if ( ! RFCal::QSC::CreateSwpRecords::Add::Data_PushFBRx ( hPhone, swpParams, (*band_itr), fbrx_rfmPath_ResultMap ) )
         {
            return false;
         }
         status = true;
      }
   }
   return status;
}
unsigned char  RFCal::QSC::CreateSwpRecords::GetPerPathParam ( HANDLE hPhone, CBandInfo & bandInfo, CBandInfoPtr & perPathPtr, const CRFPathInformationPacket & rfPathInfo)
{
   CRFM_Path rfm_path ( rfPathInfo );
   CPath_Info & pathParam = bandInfo.perPathParams;

   perPathPtr.band = bandInfo.band;
   perPathPtr.subBand = bandInfo.subBand;
   perPathPtr.pa_param_key = &(bandInfo.pa_param_key);
   perPathPtr.perPathParams = &(bandInfo.perPathParams);

   /* if <Path_Info> data available, use the Path_Info data. */ 
   perPathPtr.calType = (pathParam.calType.find(rfm_path) == pathParam.calType.end())? & (bandInfo.calType) : & (pathParam.calType[rfm_path]);
   perPathPtr.commonParams = (pathParam.commonParams.find(rfm_path) == pathParam.commonParams.end())? &(bandInfo.commonParams) : & (pathParam.commonParams[rfm_path]);
   perPathPtr.rxGainParams = (pathParam.rxGainParams.find(rfm_path) == pathParam.rxGainParams.end())? & (bandInfo.rxGainParams) : & (pathParam.rxGainParams[rfm_path]);
   perPathPtr.fbrxGainParams = (pathParam.fbrxGainParams.find(rfm_path) == pathParam.fbrxGainParams.end())? & (bandInfo.fbrxGainParams) : & (pathParam.fbrxGainParams[rfm_path]);
   perPathPtr.txLinParams = (pathParam.txLinParams.find(rfm_path) == pathParam.txLinParams.end())? & (bandInfo.txLinParams) : & (pathParam.txLinParams[rfm_path]);

   perPathPtr.xptParams = (pathParam.xptParams.find(rfm_path) == pathParam.xptParams.end())? & (bandInfo.xptParams) : & (pathParam.xptParams[rfm_path]);
   perPathPtr.rsbParams = (pathParam.rsbParams.find(rfm_path) == pathParam.rsbParams.end())? & (bandInfo.rsbParams) : & (pathParam.rsbParams[rfm_path]);
   perPathPtr.ip2Params = (pathParam.ip2Params.find(rfm_path) == pathParam.ip2Params.end())? & (bandInfo.ip2Params) : & (pathParam.ip2Params[rfm_path]);
   perPathPtr.xoParams  = (pathParam.xoParams.find(rfm_path)  == pathParam.xoParams.end())? & (bandInfo.xoParams) : & (pathParam.xoParams[rfm_path]);
   return true;
}

unsigned char RFCal::QSC::CreateSwpRecords::Tx_Xpt_Cal(HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams)
{

   unsigned char status = true;

   qscRecords & qsc_records = swpParams.swps.back();
   std::vector < cal_sweep_record >::iterator swpIt = qsc_records.swpRecords.begin();
   std::vector <TE_Swp_Params>::iterator teIt = qsc_records.teRecords.begin();
   unsigned long handle_id = 0;

   // ************************************** //
   // Get RFM path for this band
   // ************************************** //
      
   std::vector < ACTION_TYPE > actionType;
   actionType.push_back( ACTION_TYPE_CAL );

   std::vector < CRFPathInformationPacket > aggregatedResult;
   aggregatedResult.clear();

   if ( swpParams.BandInfo.empty () )
   {
      return false; // Critial parameter reading error
   }
   CBandInfo & bandInfo = swpParams.BandInfo.back();
   if (  bandInfo.calType.Enable_Offset_Calibration )
       actionType.push_back( ACTION_TYPE_OFFSET_CAL );

   std::vector < PATH_TYPE > pathType;
   pathType.push_back ( PATH_TYPE_TX );

   if (! RFCal::QSC::GetCalibratedRFMPathInfo( hPhone, swpParams, bandInfo, aggregatedResult, actionType, pathType) )
   {
      // Failed...
      return false;
   }


   // ************************************** //
   // Data - PushFBRxGain Result
   // If connected cal push fbrx gain data, then, no need to send.
   // ************************************** //
   if ( bandInfo.calType.Enable_Push_FBRxGain_Data )
   {
      std::map < CRFM_Path, ftm_fbrxgain_path_result > fbrx_rfmPath_ResultMap;
      swpParams.repoPtr->GetFBRxResult ( hPhone, qsc_node, fbrx_rfmPath_ResultMap );
      RFCal::QSC::CreateSwpRecords::Add::Data_PushFBRx ( hPhone, swpParams, bandInfo, fbrx_rfmPath_ResultMap );
   }

   // ************************************** //
   // Remove offset cal. execute only full path
   // ************************************** //
   if ( bandInfo.calType.Enable_Offset_Calibration )
   {
       aggregatedResult.clear();
       actionType.clear();
       actionType.push_back( ACTION_TYPE_CAL );
       if ( !RFCal::QSC::GetCalibratedRFMPathInfo( hPhone, swpParams, bandInfo, aggregatedResult, actionType, pathType))
       {
          return false;
       }
   }

   // ************************************** //
   // RFM path loop - Aggregated RFM paths
   // ************************************** //
   std::vector < CRFPathInformationPacket >::iterator aggr_it = aggregatedResult.begin();

   for( ; aggr_it != aggregatedResult.end(); aggr_it++)
   {
      // ************************************** //
      // Check path type is Tx or not
      // ************************************** //
      if ( swpParams.repoPtr->rfmPathDB.GetPathType ( hPhone, swpParams.tech, bandInfo.band, bandInfo.subBand, ( *aggr_it )) != PATH_TYPE_TX)
      {
         continue;
      }
      // ************************************** //
      // Per path parameter
      // ************************************** //
      CBandInfoPtr perPathBandInfo;
      GetPerPathParam( hPhone, bandInfo, perPathBandInfo, (*aggr_it));

      const CCalType & calType = ( perPathBandInfo.calType )? *(perPathBandInfo.calType) : bandInfo.calType;
      const CCommonParam & commonParams = ( perPathBandInfo.commonParams )? *(perPathBandInfo.commonParams) : bandInfo.commonParams;

      // ************************************** //
      // FTM_GET_RFFE_DEVICE_INFO_V2 for XPT cal
      // ************************************** //
      if ( calType.Enable_Tx_XPT)
      {
         RFCal::QSC::GetRFFE_DEVICE_INFO_V2 ( hPhone, &swpParams.repoPtr->rfmPathDB, swpParams.tech, perPathBandInfo.band, perPathBandInfo.subBand, ( * aggr_it ), bandInfo.pa_param_key );
         perPathBandInfo.pa_param_key = &( bandInfo.pa_param_key );
      }

      const std::vector < int > & channels =  commonParams.Channel_List;

      // ************************************** //
      // Channel Loops
      // ************************************** //
      for ( std::vector < int >::const_iterator chan_it = channels.begin();  chan_it != channels.end(); chan_it++)
      {
         // ************************************** //
         // Radio Setup Type
         // - RADIO_SETUP_TECH_BAND_PATH_CHAN- Complete setup (for 1 st channel )
         // - RADIO_SETUP_PATH_CHAN - RFM Path change
         // - RADIO_SETUP_CHAN - Retune (Channel change )
         // ************************************** //
         ftm_radio_setup_type setup_type = RADIO_SETUP_CHAN;
         
         for ( std::vector < vector < int > >::const_iterator bwList_it = commonParams.Tx_Rx_BW_Type.begin();
               bwList_it !=  commonParams.Tx_Rx_BW_Type.end(); bwList_it++ )
         {
            ftm_radio_setup_type setup_type = RADIO_SETUP_CHAN;
            int BW = RFCal::QSC::Param::GetBandwidth_FromBW_Type ( hPhone, *bwList_it );
            if (  bwList_it == commonParams.Tx_Rx_BW_Type.begin())
            {
               if (channels.begin() == chan_it )
               {
                  if (aggregatedResult.begin() == aggr_it)
                  {
                     setup_type = RADIO_SETUP_TECH_BAND_PATH_CHAN;
                  }
                  else
                  {
                     setup_type = RADIO_SETUP_PATH_CHAN;             
                  }
               } 
               else
               {
                     setup_type = RADIO_SETUP_CHAN;
               }
            }
            else
            {
               // ********************************************************* //
               // Radio Setup - Tear down for each of BW path change
               // ********************************************************* //
               RFCal::QSC::CreateSwpRecords::Add::Teardown ( hPhone, handle_id, swpParams, perPathBandInfo);
               handle_id++;
               setup_type = RADIO_SETUP_BANDWIDTH;
            }
            RFCal::QSC::CreateSwpRecords::Add::RadioSetup (hPhone, handle_id, setup_type, swpParams, perPathBandInfo, (*aggr_it), ( *chan_it ), ( *bwList_it )); 
            
            // ************************************** //
            // Tx Segments - Fullbias
            // ************************************** //
            if ( calType.Enable_Full_Bias_Linearizers )
            {
               RFCal::QSC::CreateSwpRecords::Add::Tx_Fullbias (hPhone, handle_id, swpParams, perPathBandInfo ); 
            }

            // ************************************** //
            // Tx Segments - Apt
            // ************************************** //
            if ( calType.Enable_APT_Linearizers)
            {
              // Note: use the same handle_id as fullbias cal
			  // for APT sweep, only has 10 and 40mhz with BW 3 or 5,5
			  int txPurpose = (BW == 40)? 5 : 1 ;
              RFCal::QSC::CreateSwpRecords::Add::Tx_Apt (hPhone, handle_id, swpParams, txPurpose, perPathBandInfo ); 
			   }

            // ************************************** //
            // Tx Segments - Xpt Cal
            // ************************************** //
            if ( calType.Enable_Tx_XPT)
            {
               RFCal::QSC::CreateSwpRecords::Add::Tx_Xpt (hPhone, handle_id, swpParams, perPathBandInfo, (*perPathBandInfo.pa_param_key), BW ); 
            }
          }
      }
      // ********************************************************* //
      // Data Opcode segments - For each of rfmpath (in the case of unconnected cal)
      // NV write should be done at the last rfm path
      // ********************************************************* //
      
      unsigned char isLastRFMPath = (aggregatedResult.end() == ( aggr_it + 1) )? true : false;
      RFCal::QSC::CreateSwpRecords::Add::Data_ResultNV ( hPhone, swpParams, perPathBandInfo, isLastRFMPath );

      // ********************************************************* //
      // Radio Setup - Tear down for each of RFM path change
      // ********************************************************* //
      RFCal::QSC::CreateSwpRecords::Add::Teardown ( hPhone, handle_id, swpParams, perPathBandInfo);
      handle_id++;
   }

   return status;
}

unsigned char RFCal::QSC::CreateSwpRecords::IP2_Cal(HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams)
{
   unsigned char status = true;

   qscRecords & qsc_records = swpParams.swps.back();
   std::vector < cal_sweep_record >::iterator swpIt = qsc_records.swpRecords.begin();
   std::vector <TE_Swp_Params>::iterator teIt = qsc_records.teRecords.begin();
   unsigned long handle_id = 0;
   CBandInfoPtr perPathBandInfo;

   // ************************************** //
   // Get RFM path for this band
   // ************************************** //
      
   std::vector < ACTION_TYPE > actionType;
   std::vector < CRFPathInformationPacket > aggregatedResult;
   aggregatedResult.clear();

   if ( swpParams.BandInfo.empty () )
   {
      return false; // Critial parameter reading error
   }
   CBandInfo & bandInfo = swpParams.BandInfo.back();

   std::vector < PATH_TYPE > pathType;

   
   if (bandInfo.calType.Enable_PRx_IP2Cal || bandInfo.calType.Enable_DRx_IP2Cal)
	   actionType.push_back( ACTION_TYPE_IP2_CAL );
	
   if (bandInfo.calType.Enable_PRx_IP2Cal)
	   pathType.push_back ( PATH_TYPE_PRX );

   if (bandInfo.calType.Enable_DRx_IP2Cal)
	   pathType.push_back ( PATH_TYPE_DRX );

   if (! RFCal::QSC::GetCalibratedRFMPathInfo( hPhone, swpParams, bandInfo, aggregatedResult, actionType, pathType) )
   {
      // Failed...
      return false;
   }

   // ************************************** //
   // RFM path loop - Aggregated RFM paths
   // ************************************** //
   std::vector < CRFPathInformationPacket >::iterator aggr_it = aggregatedResult.begin();

   for( ; aggr_it != aggregatedResult.end(); aggr_it++)
   {
      // ************************************** //
      // Check IP2 Cal bitmask for each path
	  // If bitmask is 0, IP2 Cal is not needed for the path
	  // If bitmask is not 0, check bandwith info from bitmask
      // ************************************** //

	   uint16 ip2cal_cfg_bitmask = aggr_it -> ip2cal_cfg_bitmask;

	   vector <vector <int>> bwList;
	   vector<int> IP2bwList;
	   bwList.clear();
	   IP2bwList.clear();
	         
	   // ************************************** //
      // Check IP2 Cal bandwidth bitmask for each path
      // ************************************** //

	   for (int bShiftIdx = 0; bShiftIdx < FTM_RF_PATH_INFORMATION_IP2CAL_ACTION_BSHFT; bShiftIdx ++)
	   {
		   if (((ip2cal_cfg_bitmask & FTM_RF_PATH_INFORMATION_IP2CAL_BWIDTH_BMASK) >> bShiftIdx ) & 0x1)
		   {
			   IP2bwList.push_back(bShiftIdx);
		   }	
	   }
	   

      // ************************************** //
      // Per path parameter
      // ************************************** //
      GetPerPathParam( hPhone, bandInfo, perPathBandInfo, (*aggr_it));

      const CCalType & calType = ( perPathBandInfo.calType )? *(perPathBandInfo.calType) : bandInfo.calType;
      const CCommonParam & commonParams = ( perPathBandInfo.commonParams )? *(perPathBandInfo.commonParams) : bandInfo.commonParams;

	  const std::vector < int > & channels =  commonParams.Channel_List;

      // ************************************** //
      // Channel Loops
      // ************************************** //
      for ( std::vector < int >::const_iterator chan_it = channels.begin();  chan_it != channels.end(); chan_it++)
      {
         // ************************************** //
         // Radio Setup Type
         // - RADIO_SETUP_TECH_BAND_PATH_CHAN- Complete setup (for 1 st channel )
         // - RADIO_SETUP_PATH_CHAN - RFM Path change
         // - RADIO_SETUP_CHAN - Retune (Channel change )
         // ************************************** //
         ftm_radio_setup_type setup_type = RADIO_SETUP_CHAN;
         
		 std::vector < vector < int > >::const_iterator bwList_it = bwList.begin();
         
		 for ( std::vector < vector < int > >::const_iterator bwList_it = commonParams.Tx_Rx_BW_Type.begin();
              bwList_it !=  commonParams.Tx_Rx_BW_Type.end(); bwList_it++ )
		 {
            ftm_radio_setup_type setup_type = RADIO_SETUP_CHAN;
            if (  bwList_it == commonParams.Tx_Rx_BW_Type.begin())
            {
				if (channels.begin() == chan_it )
				{
					if (aggregatedResult.begin() == aggr_it)
					{
						setup_type = RADIO_SETUP_TECH_BAND_PATH_CHAN;
					}
					else
					{
						setup_type = RADIO_SETUP_PATH_CHAN;             
					}
				} 
                else
                {
                    setup_type = RADIO_SETUP_CHAN;
                }
            }
            else
			{
				// ********************************************************* //
				// Radio Setup - Tear down for each of BW path change
				// ********************************************************* //
				RFCal::QSC::CreateSwpRecords::Add::Teardown ( hPhone, handle_id, swpParams, perPathBandInfo);
				handle_id++;
				setup_type = RADIO_SETUP_BANDWIDTH;
			}
			RFCal::QSC::CreateSwpRecords::Add::RadioSetup (hPhone, handle_id, setup_type, swpParams, perPathBandInfo, (*aggr_it), ( *chan_it ), ( *bwList_it )); 

			// ************************************** //
			// IP2 Segments
			// ************************************** //
			if ( calType.Enable_PRx_IP2Cal || calType.Enable_DRx_IP2Cal )
			{
				RFCal::QSC::CreateSwpRecords::Add::IP2(hPhone, handle_id, swpParams, perPathBandInfo ,IP2bwList, ( *chan_it )); 
			}			
		 }
	  }

	  // ********************************************************* //
	  // Radio Setup - Tear down for each of RFM path change
	  // ********************************************************* //
	  RFCal::QSC::CreateSwpRecords::Add::Teardown ( hPhone, handle_id, swpParams, perPathBandInfo);


	  
	  handle_id++;
   }

   // ********************************************************* //
   // Data Opcode segments - For each of rfmpath (in the case of unconnected cal)
   // NV write should be done at the last rfm path
   // ********************************************************* //

   RFCal::QSC::CreateSwpRecords::Add::Data_ResultNV ( hPhone, swpParams, perPathBandInfo, static_cast <unsigned char> (TRUE) );
   return status;
}

unsigned char RFCal::QSC::CreateSwpRecords::Align_TE_Reords ( HANDLE hPhone, qscRecords & orgQscRecords)
{
   // Check teRecords empty, if empty, exit
   if (orgQscRecords.teRecords.empty())
   {
      return false;
   }

   vector < TE_Swp_Params > newTeRecords;
   newTeRecords.clear();

   unsigned long total_seg_duration = RFCal::QSC::GetTotalSegmentTime( hPhone, orgQscRecords.swpRecords);

   const int measTime = static_cast <int> ( orgQscRecords.teRecords.front().m_step_width );
   if ( 0 == measTime)
   {
      // Stop here. let crash for debugging
   }
   newTeRecords.reserve ( static_cast < unsigned int > ( total_seg_duration / measTime ) );

   vector < TE_Swp_Params >::const_iterator te_itr = orgQscRecords.teRecords.begin();
   vector < cal_sweep_record >::const_iterator swp_itr = orgQscRecords.swpRecords.begin();
   for ( ; ( te_itr !=  orgQscRecords.teRecords.end()) && ( swp_itr != orgQscRecords.swpRecords.end()); te_itr++, swp_itr++)
   {
      int segDuration = static_cast < int > ( swp_itr->opcode.segLen );
      const int numLoop = segDuration / measTime;
      
      for ( int i = 0; i < ( numLoop - 1); i++)
      {
         if ( te_itr ==  orgQscRecords.teRecords.begin() )
            newTeRecords.push_back ( * (te_itr) );
         else
            newTeRecords.push_back ( * (te_itr-1) );
      }
      newTeRecords.push_back ( * (te_itr) );
   }
   orgQscRecords.teRecords = newTeRecords;
   return true;
}
         
unsigned char RFCal::QSC::Read::PAParamXML(HANDLE hPhone, QSCParams & params)
{
   //**************************************************//
   //         Load XML 
   //**************************************************//
   MSXML2::IXMLDOMDocumentPtr spXMLDoc;   
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
   spXMLDoc->load(params.FilePath.stPAParamXML.c_str());
   if (!spXMLDoc)
   {
      stringstream ss;
      ss << "Failed to load " << params.FilePath.stCharXML.c_str() << endl;
      return false;
   }
   // ***************************************************************//
   //      Get PA node list
   //************************************++**************************//
   
   MSXML2::IXMLDOMNodeListPtr pPAList = spXMLDoc->selectNodes((_bstr_t)"/RFCal_Data/PA");    
   if (!pPAList)
   {
      spXMLDoc.Release();
      return false;
   }
   // ***************************************************************//
   //      Read each of nodes and assign table map
   // ***************************************************************//
   for (int i= 0; i < pPAList->length; i++)
   {
      MSXML2::IXMLDOMNodePtr pPANode = pPAList->nextNode();
      if ( NULL == pPANode)
         continue;

      PA_PARAM_KEY key;
      RFCal::Helper::XML::Read_Attribute(pPANode, "product_rev", key.product_rev);
        RFCal::Helper::XML::Read_Attribute(pPANode, "product_id", key.product_id);
        RFCal::Helper::XML::Read_Attribute(pPANode, "manuf_id", key.manuf_id);

      
      MSXML2::IXMLDOMNodeListPtr pXptInput_V1List = pPANode->selectNodes((_bstr_t)"XptInput_V1"); 
      
    std::map < CXptInput_V1_Key, CXptInput_V1_BW_V> xptInputMap;
      for (int table_idx = 0 ; table_idx < pXptInput_V1List->length; table_idx++)
      {
         MSXML2::IXMLDOMNodePtr pNode = pXptInput_V1List->nextNode();
         if ( NULL == pNode)
            continue;
         

         CXptInput_V1_Key xptInputKey;
         CXptInput_V1 xptInputParam;
		 CXptInput_V1_BW_V xptInput_V1_BW_V;
         RFCal::Helper::XML::Read_Attribute(pNode, "rf_mode", xptInputKey.rf_mode);
           RFCal::Helper::XML::Read_Attribute(pNode, "device", xptInputKey.device);
           RFCal::Helper::XML::Read_Attribute(pNode, "instance", xptInputKey.instance);
        
	    MSXML2::IXMLDOMNodeListPtr pBWinputList = pNode->selectNodes((_bstr_t)"BwInput"); 

		if ( 0 !=pBWinputList->length)// if there is BWINput tag
		{
		  for (int table_idx1 = 0 ; table_idx1 < pBWinputList->length; table_idx1++)
		  {
			 MSXML2::IXMLDOMNodePtr pBWNode = pBWinputList->nextNode();
             if ( NULL == pBWNode)
              continue;
			 RFCal::Helper::XML::Read_Attribute(pBWNode, "bw", xptInputParam.BW);
			 MSXML2::IXMLDOMNodeListPtr pBWPAinputList = pBWNode->selectNodes((_bstr_t)"PaInput");
			 for (int table_idx2 = 0 ; table_idx2 < pBWPAinputList->length; table_idx2++)
			 {
				  MSXML2::IXMLDOMNodePtr pBWPANode = pBWPAinputList->nextNode();
                  if ( NULL == pBWPANode)
                  continue;
				  pNode=pBWPANode;
				  RFCal::Helper::XML::Read_Attribute(pNode, "pa_state", xptInputParam.PAstate);
		          RFCal::Helper::XML::Read_Attribute(pNode, "IcqEt_table", "IcqEt", xptInputParam.IcqEt);
				  RFCal::Helper::XML::Read_Attribute(pNode, "IcqEpt_table", "IcqEpt", xptInputParam.IcqEpt);
				  RFCal::Helper::XML::Read_Attribute(pNode, "IcqRgiDeltaSwp_table", "IcqRgiDeltaSwp", xptInputParam.IcqRgiDeltaSwp);
                  RFCal::Helper::XML::Read_Attribute(pNode, "MLineMaxPaSupplyEt_table", "MLineMaxPaSupplyEt", xptInputParam.MLineMaxPaSupplyEt);
                  RFCal::Helper::XML::Read_Attribute(pNode, "MLineMaxPaSupplyEpt_table", "MLineMaxPaSupplyEpt", xptInputParam.MLineMaxPaSupplyEpt);
                  RFCal::Helper::XML::Read_Attribute(pNode, "IqGainRgiDeltaSwp_table", "IqGainRgiDeltaSwp", xptInputParam.IqGainRgiDeltaSwp);
                  RFCal::Helper::XML::Read_Attribute(pNode, "EtVmin_table", "EtVmin", xptInputParam.EtVmin);
                  RFCal::Helper::XML::Read_Attribute(pNode, "FirmwareDelayIqGain_table", "FirmwareDelayIqGain", xptInputParam.FirmwareDelayIqGain);
                  RFCal::Helper::XML::Read_Attribute(pNode, "AclrDelayCharIqGain_table", "AclrDelayCharIqGain", xptInputParam.AclrDelayCharIqGain);

                  RFCal::Helper::XML::Read_Attribute(pNode, "MLineFitType_table", "MLineFitType", xptInputParam.MLineFitType);
                  RFCal::Helper::XML::Read_Attribute(pNode, "MLineRegCoef_table", "MLineRegCoef", xptInputParam.MLineRegCoef);
                  RFCal::Helper::XML::Read_Attribute(pNode, "CalCompPtEpt_table", "CalCompPtEpt", xptInputParam.CalCompPtEpt);
                  RFCal::Helper::XML::Read_Attribute(pNode, "CalCompPtEt_table", "CalCompPtEt", xptInputParam.CalCompPtEt);
     
                   MSXML2::IXMLDOMNodeListPtr pEpdtList = pNode->selectNodes((_bstr_t)"Detrough_table/XptEpdtRec");
                   if (pEpdtList != NULL) // APT only char table may not have detrough data.
                   {
                      xptInputParam.Detrough_table.clear();
                      xptInputParam.Detrough_table.reserve (128);

                      for (int i= 0; i < pEpdtList->length  ; i++)
                     {
                         MSXML2::IXMLDOMNodePtr pEpdtNode = pEpdtList->nextNode();
                       if ( NULL != pEpdtNode )
                         {
                           int epdt = 0;
                          RFCal::Helper::XML::Read_Attribute(pEpdtNode, "epdt", epdt);
                          xptInputParam.Detrough_table.push_back (epdt);   
                          }
                       pEpdtNode.Release();
                      }
                    pEpdtList.Release();
                   }

			 }
		 xptInput_V1_BW_V.push_back(xptInputParam);
		  }
		 xptInputMap [ xptInputKey ] = xptInput_V1_BW_V;
		 }


		else 
	{    xptInputParam.BW=10; // NO BW info, default to 10
	 	 xptInputParam.PAstate=99;// no PA state, set to 99 as invalid
         RFCal::Helper::XML::Read_Attribute(pNode, "IcqEt_table", "IcqEt", xptInputParam.IcqEt);
         RFCal::Helper::XML::Read_Attribute(pNode, "IcqEpt_table", "IcqEpt", xptInputParam.IcqEpt);
         RFCal::Helper::XML::Read_Attribute(pNode, "IcqRgiDeltaSwp_table", "IcqRgiDeltaSwp", xptInputParam.IcqRgiDeltaSwp);
         RFCal::Helper::XML::Read_Attribute(pNode, "MLineMaxPaSupplyEt_table", "MLineMaxPaSupplyEt", xptInputParam.MLineMaxPaSupplyEt);
         RFCal::Helper::XML::Read_Attribute(pNode, "MLineMaxPaSupplyEpt_table", "MLineMaxPaSupplyEpt", xptInputParam.MLineMaxPaSupplyEpt);
         RFCal::Helper::XML::Read_Attribute(pNode, "IqGainRgiDeltaSwp_table", "IqGainRgiDeltaSwp", xptInputParam.IqGainRgiDeltaSwp);
         RFCal::Helper::XML::Read_Attribute(pNode, "EtVmin_table", "EtVmin", xptInputParam.EtVmin);
         RFCal::Helper::XML::Read_Attribute(pNode, "FirmwareDelayIqGain_table", "FirmwareDelayIqGain", xptInputParam.FirmwareDelayIqGain);
         RFCal::Helper::XML::Read_Attribute(pNode, "AclrDelayCharIqGain_table", "AclrDelayCharIqGain", xptInputParam.AclrDelayCharIqGain);

         RFCal::Helper::XML::Read_Attribute(pNode, "MLineFitType_table", "MLineFitType", xptInputParam.MLineFitType);
         RFCal::Helper::XML::Read_Attribute(pNode, "MLineRegCoef_table", "MLineRegCoef", xptInputParam.MLineRegCoef);
         RFCal::Helper::XML::Read_Attribute(pNode, "CalCompPtEpt_table", "CalCompPtEpt", xptInputParam.CalCompPtEpt);
         RFCal::Helper::XML::Read_Attribute(pNode, "CalCompPtEt_table", "CalCompPtEt", xptInputParam.CalCompPtEt);
      
         //**************************************************//
         //         Get Detrough_table
         //**************************************************//
         MSXML2::IXMLDOMNodeListPtr pEpdtList = pNode->selectNodes((_bstr_t)"Detrough_table/XptEpdtRec");
         if (pEpdtList != NULL) // APT only char table may not have detrough data.
         {
            xptInputParam.Detrough_table.clear();
            xptInputParam.Detrough_table.reserve (128);

            for (int i= 0; i < pEpdtList->length  ; i++)
            {
               MSXML2::IXMLDOMNodePtr pEpdtNode = pEpdtList->nextNode();
               if ( NULL != pEpdtNode )
               {
                  int epdt = 0;
                  RFCal::Helper::XML::Read_Attribute(pEpdtNode, "epdt", epdt);
                  xptInputParam.Detrough_table.push_back (epdt);   
               }
               pEpdtNode.Release();
            }
            pEpdtList.Release();
         }
		 xptInput_V1_BW_V.push_back(xptInputParam);
        // xptInputMap [ xptInputKey ] = xptInputParam;
		 xptInputMap [ xptInputKey ] = xptInput_V1_BW_V;
      }
      }
      params.repository.SetPaParamData ( key, xptInputMap );
   }

   return true;
}
unsigned char RFCal::QSC::Read::EquivConfigXML ( HANDLE hPhone, QSCParams & params)
{
   //**************************************************//
   //         Load XML 
   //**************************************************//
   MSXML2::IXMLDOMDocumentPtr spXMLDoc;   
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
   if (!spXMLDoc)
   {
      stringstream ss;
      ss << "xml Doc CreateInstance failed. ";
      RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str());
      return false;
   }
   spXMLDoc->load(params.FilePath.stEquipConfigXML.c_str());
   if (!spXMLDoc)
   {
      stringstream ss;
      ss << "The param XML not loaded. " << params.FilePath.stEquipConfigXML.c_str();
      RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str());
      return false;
   }

   // ***************************************************************//
   //      EquipmentSet XML
   //************************************++**************************//
   
   MSXML2::IXMLDOMNodeListPtr pEquipmentList = spXMLDoc->selectNodes((_bstr_t)"/EquipmentSet/Equipment"); 
   
   if (!pEquipmentList)
   {
      spXMLDoc.Release();
      return false;
   }
   params.DeviceConfig.type = -1;
   params.DeviceConfig.boardId = 0;
   params.DeviceConfig.secondaryAddr = 0;
   // ***************************************************************//
   //      Read each of nodes and assign APT table map
   // ***************************************************************//
  for (int i= 0; i < pEquipmentList->length; i++)
  {
      MSXML2::IXMLDOMNodePtr pEquipmentNode = pEquipmentList->nextNode();
      if ( NULL == pEquipmentNode)
         continue;

      string stidentifier;
      if ( RFCal::Helper::XML::Read_Str_Attribute ( pEquipmentNode, "identifier", stidentifier))
      {
         if ( stidentifier != "Call_Processor1")
         {
            continue;
         }
         int pad;
         if ( RFCal::Helper::XML::Read_Attribute ( pEquipmentNode, "pad", pad))
         {
            params.DeviceConfig.primaryAddr = pad;
         }

         int sad;
         if ( RFCal::Helper::XML::Read_Attribute ( pEquipmentNode, "sad", sad))
         {
            params.DeviceConfig.secondaryAddr = sad;
         }

         int board_id;
         if ( RFCal::Helper::XML::Read_Attribute ( pEquipmentNode, "board_id", board_id))
         {
            params.DeviceConfig.boardId = board_id;
         }      

         string stlanguage;
         if ( RFCal::Helper::XML::Read_Str_Attribute ( pEquipmentNode, "language", stlanguage))
         {
            if ( stlanguage == "wan_rfcal_qcom_scpi")
            {
               params.DeviceConfig.type = QSCPI;
               break;
            }
         }
         string name;
         if ( RFCal::Helper::XML::Read_Str_Attribute ( pEquipmentNode, "name", name))
         {
            if ( name == "CMW500" )
            {
               params.DeviceConfig.type = CMW;
               break;
            }
            else if ( name == "Anritsu MT8870a" )
            {
               params.DeviceConfig.type = ANR8870A;
               break;
            }
            else if ( name == "Anritsu MT8820" )
            {
               params.DeviceConfig.type = ANR8820C;
               break;
            }
            // No support
         }
      }
  }

  pEquipmentList.Release();
  spXMLDoc.Release();
  return ( params.DeviceConfig.type != -1 )? true : false;
}
unsigned char RFCal::QSC::Read::RxCharXML(HANDLE hPhone, QSCParams & params)
{
   //**************************************************//
   //         Load XML 
   //**************************************************//
   MSXML2::IXMLDOMDocumentPtr spXMLDoc;   
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
   spXMLDoc->load(params.FilePath.stRxCharXML.c_str());
   if (!spXMLDoc)
   {
      stringstream ss;
      ss << "The param XML not loaded. " << params.FilePath.stRxCharXML.c_str();
      return false;
   }

   // ***************************************************************//
   //      Rx Char XML
   //************************************++**************************//
   
   MSXML2::IXMLDOMNodeListPtr pRxCharList = spXMLDoc->selectNodes((_bstr_t)"/RFCal_Data/RxCharBandData"); 
   
   if (!pRxCharList)
   {
      spXMLDoc.Release();
      return false;
   }

   // ***************************************************************//
   //      Read each of nodes and assign APT table map
   // ***************************************************************//
   CTechBand_PathChanResults techBand_Results;

   for (int i= 0; i < pRxCharList->length; i++)
   {
      MSXML2::IXMLDOMNodePtr pBandNode = pRxCharList->nextNode();
      if ( NULL == pBandNode)
         continue;

      QSCNode qscNode;
      RFCal::Helper::XML::Read_Attribute(pBandNode, "tech", qscNode.tech);
        RFCal::Helper::XML::Read_Attribute(pBandNode, "band", qscNode.band);
        RFCal::Helper::XML::Read_Attribute(pBandNode, "subband", qscNode.subBand);

      MSXML2::IXMLDOMNodeListPtr pPathList = pBandNode->selectNodes((_bstr_t)"RxCharPathData");

     // std::set < CRFM_Path > apt_record_set;

      if ( NULL == pPathList )
         continue;

      CPath_ChanResults path_results;
      
      for (int pathIdx = 0; pathIdx < pPathList->length; pathIdx++)
      {
         MSXML2::IXMLDOMNodePtr pPathNode = pPathList->nextNode();
         if ( NULL == pPathNode)
            continue;
         CRFM_Path rfmPath;
         RFCal::Helper::XML::Read_Attribute(pPathNode, "signal_path", rfmPath.signal_path);
           RFCal::Helper::XML::Read_Attribute(pPathNode, "rfm_device", rfmPath.rfm_dev);
           RFCal::Helper::XML::Read_Attribute(pPathNode, "antenna_port", rfmPath.antenna_port);

         MSXML2::IXMLDOMNodeListPtr pChanList = pPathNode->selectNodes((_bstr_t)"RxCharChannelData");
         if ( NULL == pChanList)
            continue;

         CChan_RxResults chan_results;
         for (int chIdx = 0; chIdx < pChanList->length; chIdx++)
         {
            MSXML2::IXMLDOMNodePtr pChanNode = pChanList->nextNode();
            if ( NULL == pChanNode)
               continue;
            int channel = 0;
            RFCal::Helper::XML::Read_Attribute(pChanNode, "channel", channel);

            MSXML2::IXMLDOMNodeListPtr pResultList = pChanNode->selectNodes((_bstr_t)"RxCharResultData");
            if ( NULL == pResultList)
               continue;
            
            CGainState_AgcOffset rxCharResultData;
            
            for (int resIdx = 0; resIdx < pResultList->length; resIdx++)
            {
               MSXML2::IXMLDOMNodePtr pResNode = pResultList->nextNode();
               if ( NULL == pResNode)
                  continue;
               int gain_state;
               int agc_offset;
               RFCal::Helper::XML::Read_Attribute(pResNode, "gain_state", gain_state);
               RFCal::Helper::XML::Read_Attribute(pResNode, "agc_offset", agc_offset);
               rxCharResultData[ gain_state ] = agc_offset;
            }
            chan_results [ channel ] = rxCharResultData;
         }
         path_results.map [ rfmPath ] = chan_results;
      }
      techBand_Results [ qscNode ] = path_results;
   }
   //**************************************************//
   //         Register Rx char data into global DB 
   //**************************************************//
  
   if ( ! techBand_Results.empty() )
   {
      params.repository.RegisterRxCharData ( hPhone, techBand_Results );
   }

   return true;
}
unsigned char RFCal::QSC::Read::CharXML(HANDLE hPhone, QSCParams & params)
{
   //**************************************************//
   //         Load XML 
   //**************************************************//
   MSXML2::IXMLDOMDocumentPtr spXMLDoc;   
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
   spXMLDoc->load(params.FilePath.stCharXML.c_str());
   if (!spXMLDoc)
   {
      stringstream ss;
      ss << "The param XML not loaded. " << params.FilePath.stCharXML.c_str();
      return false;
   }

   // ***************************************************************//
   //      APT Char XML
   //************************************++**************************//
   
   MSXML2::IXMLDOMNodeListPtr pAPTCharV3List = spXMLDoc->selectNodes((_bstr_t)"/RFCal_Data/APTCharV3"); 
   
   if (!pAPTCharV3List)
   {
      spXMLDoc.Release();
      return false;
   }

   // ***************************************************************//
   //      Read each of nodes and assign APT table map
   // ***************************************************************//
  for (int i= 0; i < pAPTCharV3List->length; i++)
   {
      MSXML2::IXMLDOMNodePtr pAptCharV3Node = pAPTCharV3List->nextNode();
      if ( NULL == pAptCharV3Node)
         continue;

      APT_CharV3_Key key;

      RFCal::Helper::XML::Read_Attribute(pAptCharV3Node, "rf_mode", key.rf_mode);
        RFCal::Helper::XML::Read_Attribute(pAptCharV3Node, "device", key.device);

      MSXML2::IXMLDOMNodeListPtr pAPTtableList = pAptCharV3Node->selectNodes((_bstr_t)"APTTable"); 
      
      for (int table_idx = 0 ; table_idx < pAPTtableList->length; table_idx++)
      {
         MSXML2::IXMLDOMNodePtr pTableNode = pAPTtableList->nextNode();
         if ( NULL == pTableNode)
            continue;

         RFCal::Helper::XML::Read_Attribute(pTableNode, "channel", key.channel);
           RFCal::Helper::XML::Read_Attribute(pTableNode, "pa_state", key.pa_state);
           RFCal::Helper::XML::Read_Attribute(pTableNode, "tx_purpose", key.tx_purpose);

         MSXML2::IXMLDOMNodeListPtr pAPTRecList = pTableNode->selectNodes((_bstr_t)"APTRec");

         std::set < AptRecord > apt_record_set;

         if ( NULL == pAPTRecList )
            continue;
         for (int AptRecIndex = 0; AptRecIndex < pAPTRecList->length; AptRecIndex++)
         {
            MSXML2::IXMLDOMNodePtr pAptRecordNode = pAPTRecList->nextNode();
            if ( NULL == pAptRecordNode)
               continue;

            AptRecord apt_record;
            RFCal::Helper::XML::Read_Attribute(pAptRecordNode, "pwr", apt_record.pwr);
            RFCal::Helper::XML::Read_Attribute(pAptRecordNode, "vcc", apt_record.vcc);
            RFCal::Helper::XML::Read_Attribute(pAptRecordNode, "icq", apt_record.icq);
            
            apt_record_set.insert(apt_record);
            pAptRecordNode.Release();
         }   
         if ( ! apt_record_set.empty() )
         {
            // ***************************************************************//
            //      Store the set of char data into the repository
            // ***************************************************************//
            params.repository.SetAptCharData( key, apt_record_set);
         }
      }
      pAPTtableList.Release();
   }
   pAPTCharV3List.Release();
   spXMLDoc.Release();

   return true;
}
unsigned char RFCal::QSC::Read::ParamXML(HANDLE hPhone, QSCParams & params)
{
   //**************************************************//
   //         Load XML 
   //**************************************************//
   MSXML2::IXMLDOMDocumentPtr spXMLDoc;   
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
   spXMLDoc->load(params.FilePath.stParamXML.c_str());
   if (!spXMLDoc)
   {
      stringstream ss;
      ss << "The param XML not loaded. " << params.FilePath.stParamXML.c_str();
      return false;
   }
   MSXML2::IXMLDOMNodeListPtr pNodeList = spXMLDoc->selectNodes((_bstr_t)"/QSC_V1/Node");
    if (NULL == pNodeList)
   {
      pNodeList.Release();
      spXMLDoc.Release();
      return false;
   }
   unsigned int iNodeSize = static_cast<unsigned int>(pNodeList->length);
   params.qscParams.clear();

   //**************************************************//
   //   Node Loop to read all the parameters in the XML 
   //**************************************************//
   for (unsigned int i = 0 ; i < iNodeSize; i++)
   {
      MSXML2::IXMLDOMNodePtr pNode = pNodeList->nextNode();
      if (pNode)
      {
         /* Node process */
         QSCNode qNode;
         QSCSwpParams swpParams;
         string stTech;
         RFCal::Helper::XML::Read_Str_Attribute(pNode,"tech",stTech);
         if (stTech.compare("CDMA") == 0)
         {
            qNode.tech = 0;
         } 
         else if (stTech.compare("WCDMA") == 0)
         {
            qNode.tech = 1;
         } 
         else if (stTech.compare("LTE") == 0)
         {
            qNode.tech = 10;
         } 
         else if (stTech.compare("TDSCDMA") == 0)
         {
            qNode.tech = 11;
         } 
         swpParams.tech = qNode.tech; // For convenience purpose

         RFCal::Helper::XML::Read_Attribute(pNode,"instance",qNode.instance);
         RFCal::Helper::XML::Read_Attribute(pNode,"sweep_type",qNode.sweep_type);
         swpParams.sweep_type = qNode.sweep_type;

         if (SEQ_CAL_UNCONNECTED == qNode.sweep_type)
         {
            RFCal::Helper::XML::Read_Attribute(pNode,"band",qNode.band);      
            RFCal::Helper::XML::Read_Attribute(pNode,"sub_band",qNode.subBand);      
         }
         else // Connected cal, possible multiband case, paramXML does not have attributes 
         {
            qNode.band = -1;
            qNode.subBand = -1;
         }
         
         // Read CalType Node
         Node::CalType( hPhone, pNode, swpParams.calType, qNode.sweep_type);

         // Force Push FBRx Data in Connected cal (QlibDemo only)
         if ( params.bForce_Push_FBRxData_In_ConnectedCal )
         {
            if ( SEQ_CAL_UNCONNECTED == qNode.sweep_type )
               swpParams.calType.Enable_Push_FBRxGain_Data = false;
            else
               swpParams.calType.Enable_Push_FBRxGain_Data = true;
         }

         // Read Default Node
         if (SEQ_CAL_CONNECTED == qNode.sweep_type || SEQ_CAL_XO_CAL == qNode.sweep_type)
         {
            Node::Default( hPhone, pNode, swpParams );

            Node::BandInfo( hPhone, pNode, swpParams );

			if (SEQ_CAL_XO_CAL == qNode.sweep_type )
			{
				qNode.band = swpParams.BandInfo[0].band;
				qNode.subBand = swpParams.BandInfo[0].subBand;
				
				// Transform to serial cal manner
				swpParams.calType.Enable_Sweep_Mode = false;
			}
         }
         else if (SEQ_CAL_UNCONNECTED == qNode.sweep_type)
         {
            CBandInfo bandinfo;
            bandinfo.band = qNode.band;
            bandinfo.subBand = qNode.subBand;
            bandinfo.calType = swpParams.calType;

            Node::Common( hPhone, pNode, bandinfo.commonParams );

            Node::Tx_Linearizer( hPhone, pNode, bandinfo.txLinParams );

            Node::Tx_XPT( hPhone, pNode, bandinfo.xptParams );  

			Node::IP2_Cal( hPhone, pNode, bandinfo.ip2Params); 

            Node::Path_Info( hPhone, pNode, bandinfo);

            swpParams.BandInfo.push_back ( bandinfo );
         }
         // Register parameters
         params.qscParams[qNode] = swpParams;
      }
   }
   return true;
}
unsigned char RFCal::QSC::Read::Node::Path_Info( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CBandInfo & bandInfo)
{
   MSXML2::IXMLDOMNodeListPtr pPathInfoList = pNode->selectNodes((_bstr_t)"Path_Info/Path");
   if (NULL == pPathInfoList)  return false;

   unsigned int iPathInfoSize = static_cast<unsigned int>(pPathInfoList->length);
   bandInfo.perPathParams.clear();

   for (unsigned int i = 0 ; i < iPathInfoSize; i++)
   {
      MSXML2::IXMLDOMNodePtr pPath_Info = pPathInfoList->nextNode();
      if (pPath_Info)
      {
         CRFM_Path rfmPath;
         RFCal::Helper::XML::Read_Attribute(pPath_Info,"sig_path",      rfmPath.signal_path);      
         RFCal::Helper::XML::Read_Attribute(pPath_Info,"device",        rfmPath.rfm_dev);      
         RFCal::Helper::XML::Read_Attribute(pPath_Info,"ant_path",      rfmPath.antenna_port);      
         
         CCommonParam commonParams = bandInfo.commonParams;
         CCalType calType = bandInfo.calType;
         CFBRx_Gain fbrxGainParams = bandInfo.fbrxGainParams; // Deep copy, Do not do shallow copy
         CRx_Gain rxGainParams = bandInfo.rxGainParams; // Deep copy, Do not do shallow copy
         CTx_Linearizer txLinParams = bandInfo.txLinParams;
         CTx_XPT xptParams = bandInfo.xptParams;
         CRx_RSB rsbParams = bandInfo.rsbParams;
         
         // Read from XML to override
         if (  RFCal::QSC::Read::Node::CalType(hPhone, pPath_Info, calType) )
         {
            bandInfo.perPathParams.calType [ rfmPath ] = calType;
         }
         if (  RFCal::QSC::Read::Node::Common(hPhone, pPath_Info, commonParams) )
         {
            bandInfo.perPathParams.commonParams [ rfmPath ] = commonParams;
         }
         if (  RFCal::QSC::Read::Node::FBRx_Gain(hPhone, pPath_Info, fbrxGainParams) )
         {
            bandInfo.perPathParams.fbrxGainParams [ rfmPath ] = fbrxGainParams;
         }
         if (  RFCal::QSC::Read::Node::Rx_Gain(hPhone, pPath_Info, rxGainParams) )
         {
            bandInfo.perPathParams.rxGainParams [ rfmPath ] = rxGainParams;
         }
         if (  RFCal::QSC::Read::Node::Rx_RSB(hPhone, pPath_Info, rsbParams) )
         {
            bandInfo.perPathParams.rsbParams [ rfmPath ] = rsbParams;
         }
         if (  RFCal::QSC::Read::Node::Tx_Linearizer(hPhone, pPath_Info, txLinParams) )
         {
            bandInfo.perPathParams.txLinParams [ rfmPath ] = txLinParams;
         }
         if (  RFCal::QSC::Read::Node::Tx_XPT(hPhone, pPath_Info, xptParams) )
         {
            bandInfo.perPathParams.xptParams [ rfmPath ] = xptParams;
         }
      }
   }
   return true;
}
unsigned char RFCal::QSC::Read::Node::BandInfo(HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, QSCSwpParams & swpParams)
{
   MSXML2::IXMLDOMNodeListPtr pBandList = pNode->selectNodes((_bstr_t)"BandInfo/Band");
   if (NULL == pBandList)  return false;

   unsigned int iBandListSize = static_cast<unsigned int>(pBandList->length);
   swpParams.BandInfo.clear();
   swpParams.BandInfo.reserve(iBandListSize);

   for (unsigned int i = 0 ; i < iBandListSize; i++)
   {
      MSXML2::IXMLDOMNodePtr pBand = pBandList->nextNode();
      if (pBand)
      {
         CBandInfo bandinfo;

         // Copy default values
         bandinfo.commonParams = swpParams.defaultParams.commonParams;
         bandinfo.rxGainParams = swpParams.defaultParams.rxGainParams;
         bandinfo.fbrxGainParams = swpParams.defaultParams.fbrxGainParams;
         bandinfo.rsbParams =  swpParams.defaultParams.rsbParams;
		 bandinfo.xoParams = swpParams.defaultParams.xoParams;
         bandinfo.calType = swpParams.calType;

         RFCal::Helper::XML::Read_Attribute(pBand,"band",      bandinfo.band);      
         RFCal::Helper::XML::Read_Attribute(pBand,"sub_band",  bandinfo.subBand);         

         // Cal Type Node
         RFCal::QSC::Read::Node::CalType(hPhone, pBand, bandinfo.calType);

         // Common Node
         RFCal::QSC::Read::Node::Common(hPhone, pBand, bandinfo.commonParams);

         // RxGain Params Node
         RFCal::QSC::Read::Node::Rx_Gain(hPhone, pBand, bandinfo.rxGainParams);

         // FBRx Params Node
         RFCal::QSC::Read::Node::FBRx_Gain(hPhone, pBand, bandinfo.fbrxGainParams);

         // RxRSB Params Node
         RFCal::QSC::Read::Node::Rx_RSB(hPhone, pBand, bandinfo.rsbParams);

		 // XO Params Node
		 RFCal::QSC::Read::Node::QSC_XO(hPhone, pBand, bandinfo.xoParams);

         // Path_Info params
         RFCal::QSC::Read::Node::Path_Info(hPhone, pBand, bandinfo);

         // Register bandinfo
         swpParams.BandInfo.push_back(bandinfo);
      }
   }
   return true;
}

 unsigned char RFCal::QSC::Read::Node::Default(HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, QSCSwpParams & swpParams)
 {
   MSXML2::IXMLDOMNodePtr pDefaultNode = pNode->selectSingleNode((_bstr_t)"Default");
   if (NULL == pDefaultNode)  return false;

   // Common Node
   RFCal::QSC::Read::Node::Common(hPhone, pDefaultNode, swpParams.defaultParams.commonParams);

   // RxGain Params Node
   RFCal::QSC::Read::Node::Rx_Gain(hPhone, pDefaultNode, swpParams.defaultParams.rxGainParams);

   // FBRx Params Node
   RFCal::QSC::Read::Node::FBRx_Gain(hPhone, pDefaultNode, swpParams.defaultParams.fbrxGainParams);

   // RxRSB Params Node
   RFCal::QSC::Read::Node::Rx_RSB(hPhone, pDefaultNode, swpParams.defaultParams.rsbParams);
   
   // XO Params Node
   RFCal::QSC::Read::Node::QSC_XO(hPhone, pDefaultNode, swpParams.defaultParams.xoParams);
   return true;
 }
 unsigned char RFCal::QSC::Read::Node::CalType(HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pRootNode, CCalType & calType, int sweep_type)
 {
   MSXML2::IXMLDOMNodePtr pNode = pRootNode->selectSingleNode((_bstr_t)"CalType");
   if (NULL == pNode)  return false;

   RFCal::Helper::XML::Read_Param(pNode, "Enable_NV_Write",       calType.Enable_NV_Write);
   RFCal::Helper::XML::Read_Param(pNode, "Skip_DeInit",                 calType.Skip_DeInit);
   RFCal::Helper::XML::Read_Param(pNode, "Skip_Radio_Setup",            calType.Skip_Radio_Setup);
   RFCal::Helper::XML::Read_Param(pNode, "Skip_Radio_TearDown",         calType.Skip_Radio_TearDown);

   if (SEQ_CAL_CONNECTED == sweep_type || 0 == sweep_type)
   {
      RFCal::Helper::XML::Read_Param(pNode, "Enable_FBRx_Gain",               calType.Enable_FBRx_Gain);
      RFCal::Helper::XML::Read_Param(pNode, "Enable_PRx_Gain",                calType.Enable_PRx_Gain);
      RFCal::Helper::XML::Read_Param(pNode, "Enable_DRx_Gain",                calType.Enable_DRx_Gain);
      RFCal::Helper::XML::Read_Param(pNode, "Enable_PRx_RSB",                 calType.Enable_PRx_RSB);
      RFCal::Helper::XML::Read_Param(pNode, "Enable_DRx_RSB",                 calType.Enable_DRx_RSB);
      RFCal::Helper::XML::Read_Param(pNode, "Enable_Sweep_Mode",              calType.Enable_Sweep_Mode );
      RFCal::Helper::XML::Read_Param(pNode, "Enable_Downlink_Aux_Port",       calType.Enable_Downlink_Aux_Port );
      RFCal::Helper::XML::Read_Param(pNode, "Enable_Multiple_Rx_Path_Cal",    calType.Enable_Multiple_Rx_Path_Cal );
      RFCal::Helper::XML::Read_Param(pNode, "Enable_Multiple_FBRx_Path_Cal",  calType.Enable_Multiple_FBRx_Path_Cal );
      RFCal::Helper::XML::Read_Param(pNode, "Enable_Offset_Calibration",      calType.Enable_Offset_Calibration );
      RFCal::Helper::XML::Read_Param(pNode, "Enable_Push_FBRxGain_Data",      calType.Enable_Push_FBRxGain_Data); // QlibDemo only

      if ( calType.Enable_PRx_RSB || calType.Enable_DRx_RSB )
      {
         calType.Enable_Sweep_Mode = false;
      }
   }
   else if (SEQ_CAL_UNCONNECTED == sweep_type || 0 == sweep_type)
   {
      RFCal::Helper::XML::Read_Param(pNode, "Enable_Tx_XPT",                     calType.Enable_Tx_XPT);
      RFCal::Helper::XML::Read_Param(pNode, "Enable_Full_Bias_Linearizers",      calType.Enable_Full_Bias_Linearizers);
      RFCal::Helper::XML::Read_Param(pNode, "Enable_APT_Linearizers",            calType.Enable_APT_Linearizers);
      RFCal::Helper::XML::Read_Param(pNode, "Enable_APT_FComp",                  calType.Enable_APT_FComp);
      RFCal::Helper::XML::Read_Param(pNode, "Enable_XPT_FComp",                  calType.Enable_XPT_FComp);
      RFCal::Helper::XML::Read_Param(pNode, "Enable_Push_FBRxGain_Data",         calType.Enable_Push_FBRxGain_Data);
      RFCal::Helper::XML::Read_Param(pNode, "Enable_Offset_Calibration",         calType.Enable_Offset_Calibration );
      RFCal::Helper::XML::Read_Param(pNode, "Enable_Multiple_Tx_Path_Cal",       calType.Enable_Multiple_Tx_Path_Cal );   
      RFCal::Helper::XML::Read_Param(pNode, "Enable_TxCal_RGIMeasLen",           calType.Enable_TxCal_RGIMeasLen );    
	  RFCal::Helper::XML::Read_Param(pNode, "Enable_PRx_IP2Cal",                 calType.Enable_PRx_IP2Cal);			
	  RFCal::Helper::XML::Read_Param(pNode, "Enable_DRx_IP2Cal",                 calType.Enable_DRx_IP2Cal);
   }
   else if (SEQ_CAL_XO_CAL == sweep_type || 0 == sweep_type)
   {
	   RFCal::Helper::XML::Read_Param(pNode, "Enable_XO_Cal",				  calType.Enable_XO_Cal);
   }
   return true;
 }
 unsigned char RFCal::QSC::Read::Node::Common(HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CCommonParam & commonParams)
 {
   MSXML2::IXMLDOMNodePtr pTargetNode = pNode->selectSingleNode((_bstr_t)"Common");
   if (NULL == pTargetNode)  return false;

   // Timing_Params Node
   RFCal::QSC::Read::Node::Timing_Params(hPhone, pTargetNode, commonParams);

   // Waveform Params Node
   RFCal::QSC::Read::Node::Waveform_Params(hPhone, pTargetNode, commonParams);
    
   RFCal::Helper::XML::Read_Param(pTargetNode, "Channel_List",             commonParams.Channel_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Ref_Channel",              commonParams.Ref_Channel);
   RFCal::Helper::XML::Read_Param(pTargetNode, "En_Compress_Esc_Req",      commonParams.En_Compress_Esc_Req);
   RFCal::Helper::XML::Read_Param(pTargetNode, "En_Compress_Esc_Res",      commonParams.En_Compress_Esc_Res);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Expected_Power_Option",    commonParams.Expected_Power_Option);
   
   return true;
 } 
 unsigned char RFCal::QSC::Read::Node::Timing_Params(HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CCommonParam & commonParams)
 {
   MSXML2::IXMLDOMNodePtr pTargetNode = pNode->selectSingleNode((_bstr_t)"Timing_Params");
   if (NULL == pTargetNode)  return false;

   RFCal::Helper::XML::Read_Param(pTargetNode, "Config_Seg_Duration_us",            commonParams.Config_Seg_Duration_us);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Tuning_Seg_Duration_us",            commonParams.Tuning_Seg_Duration_us);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Meas_Seg_Duration_us",              commonParams.Meas_Seg_Duration_us);    
   RFCal::Helper::XML::Read_Param(pTargetNode, "Data_Seg_Duration_us",              commonParams.Data_Seg_Duration_us);  
   RFCal::Helper::XML::Read_Param(pTargetNode, "Tx_Override_Seg_Duration_us",       commonParams.Tx_Override_Seg_Duration_us); 
   RFCal::Helper::XML::Read_Param(pTargetNode, "Poll_Interval",                     commonParams.Poll_Interval);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Poll_Timeout",                      commonParams.Poll_Timeout);
   return true;
 } 
 unsigned char RFCal::QSC::Read::Node::Waveform_Params(HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CCommonParam & commonParams)
 {
   MSXML2::IXMLDOMNodePtr pTargetNode = pNode->selectSingleNode((_bstr_t)"Waveform_Params");
   if (NULL == pTargetNode)  return false;

   RFCal::Helper::XML::Read_Param(pTargetNode, "Tx_Waveform_Type",            commonParams.Tx_Waveform_Type);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Tx_Rx_BW_Type",               commonParams.Tx_Rx_BW_Type);
   RFCal::Helper::XML::Read_Param(pTargetNode, "LTE_Num_RBs",                 commonParams.LTE_Num_RBs);
   RFCal::Helper::XML::Read_Param(pTargetNode, "LTE_Start_RB",                commonParams.LTE_Start_RB);
   RFCal::Helper::XML::Read_Param(pTargetNode, "DL_CW_Offset_KHz",            commonParams.DL_CW_Offset_KHz);
   RFCal::Helper::XML::Read_Param(pTargetNode, "DL_WaveformType",             commonParams.DL_WaveformType);
   RFCal::Helper::XML::Read_Param(pTargetNode, "DL_Power_Offset",             commonParams.DL_Power_Offset);
   return true;
 } 

unsigned char RFCal::QSC::Read::Node::QSC_XO( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CXo & xoParams)
{
   MSXML2::IXMLDOMNodePtr pTargetNode = pNode->selectSingleNode((_bstr_t)"XO_Cal");
   if (NULL == pTargetNode)  return false;

   RFCal::Helper::XML::Read_Param(pTargetNode, "XO_Action",                   xoParams.XO_Action);
   RFCal::Helper::XML::Read_Param(pTargetNode, "DL_Level_dBm",                xoParams.DL_Level_dBm);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Temperature_span",            xoParams.Temperature_span);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Min_Ft_Samples",              xoParams.Min_Ft_Samples);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Max_Ft_Samples",              xoParams.Max_Ft_Samples);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Substage_Time",               xoParams.Substage_Time);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Timeout_Time",                xoParams.Timeout_Time);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Update_NV",					  xoParams.Update_NV);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Xtal",						  xoParams.Xtal);
   RFCal::Helper::XML::Read_Param(pTargetNode, "LNA_Gain_State",			  xoParams.LNA_Gain_State);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Poll_Interval_us",			  xoParams.Poll_Interval_us);
   return true;
}
unsigned char RFCal::QSC::Read::Node::Rx_RSB( HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CRx_RSB & rsbParams)
{
   MSXML2::IXMLDOMNodePtr pTargetNode = pNode->selectSingleNode((_bstr_t)"Rx_RSB");
   if (NULL == pTargetNode)  return false;

   RFCal::Helper::XML::Read_Param(pTargetNode, "LNA_Gain_State",                   rsbParams.LNA_Gain_State);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Accumulation_Time_us",                   rsbParams.Accumulation_Time_us);
   RFCal::Helper::XML::Read_Param(pTargetNode, "AGC_Settling_Time_us",                   rsbParams.AGC_Settling_Time_us);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Action_Mask",                   rsbParams.Action_Mask);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Pre_Cal_RSB_Max_Lim_dB",                   rsbParams.Pre_Cal_RSB_Max_Lim_dB);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Post_Cal_RSB_Max_Lim_dB",                   rsbParams.Post_Cal_RSB_Max_Lim_dB);
   RFCal::Helper::XML::Read_Param(pTargetNode, "DL_Level_dBm",                   rsbParams.DL_Level_dBm);
   rsbParams.Pre_Cal_RSB_Max_Lim_dB *= 10.0;
   rsbParams.Post_Cal_RSB_Max_Lim_dB *= 10.0;
   return true;
}

unsigned char RFCal::QSC::Read::Node::Rx_Gain(HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CRx_Gain & rxGainParams)
 {
   MSXML2::IXMLDOMNodePtr pTargetNode = pNode->selectSingleNode((_bstr_t)"Rx_Gain");
   if (NULL == pTargetNode)  return false;

   RFCal::Helper::XML::Read_Param(pTargetNode, "DL_Powers",                   rxGainParams.DL_Powers);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Gain_States",                 rxGainParams.Gain_States);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Power_Mode",                  rxGainParams.Power_Mode);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Gain_Limit_Max",              rxGainParams.Gain_Limit_Max);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Gain_Limit_Min",              rxGainParams.Gain_Limit_Min);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Num_Gain_States_Per_Segment", rxGainParams.Num_Gain_States_Per_Segment);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Preferred_Device",            rxGainParams.Preferred_Device);
   RFCal::Helper::XML::Read_Param(pTargetNode, "AGC_Settling_Time",           rxGainParams.AGC_Settling_Time);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Accumulation_Time",           rxGainParams.Accumulation_Time);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Measurement_Purpose",         rxGainParams.Measurement_Purpose);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Skip_Path",                   rxGainParams.Skip_Path); // This parameter should appear only within Path_Info
  
   return true;
}
unsigned char RFCal::QSC::Read::Node::FBRx_Gain(HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CFBRx_Gain & fbrxParams)
{
   MSXML2::IXMLDOMNodePtr pTargetNode = pNode->selectSingleNode((_bstr_t)"FBRx_Gain");
   if (NULL == pTargetNode)  return false;

   RFCal::Helper::XML::Read_Param(pTargetNode, "Gain_State_List",       fbrxParams.Gain_State_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Tx_Power_dBm_List",     fbrxParams.Tx_Power_dBm_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "PA_State_List",         fbrxParams.PA_State_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "RGI_List",              fbrxParams.RGI_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Bias_List",             fbrxParams.Bias_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Icq_List",              fbrxParams.Icq_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Number_Of_Captures",    fbrxParams.Number_Of_Captures);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Max_LSE",               fbrxParams.Max_LSE);

   return true;
}
unsigned char RFCal::QSC::Read::Node::Tx_Linearizer(HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CTx_Linearizer & txLinParams)
{
   MSXML2::IXMLDOMNodePtr pTargetNode = pNode->selectSingleNode((_bstr_t)"Tx_Linearizer");
   if (NULL == pTargetNode)  return false;

   RFCal::Helper::XML::Read_Param(pTargetNode, "Upper_Bound_Channel",   txLinParams.Upper_Bound_Channel);
   RFCal::Helper::XML::Read_Param(pTargetNode, "PA_State_List",         txLinParams.PA_State_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "RGI_List",              txLinParams.RGI_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Bias_List",             txLinParams.Bias_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Icq_List",              txLinParams.Icq_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Exp_Start_Power_List",  txLinParams.Exp_Start_Power_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Exp_Delta_Power_List",  txLinParams.Exp_Delta_Power_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Min_Delta_Power_List",  txLinParams.Min_Delta_Power_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Max_Lin_Power_List",    txLinParams.Max_Lin_Power_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Min_Lin_power_List",    txLinParams.Min_Lin_power_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "APT_PA_Gain_Delta_List",  txLinParams.APT_PA_Gain_Delta_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "PA_Gain_List",          txLinParams.PA_Gain_List);

   return true;
}
unsigned char RFCal::QSC::Read::Node::Tx_XPT(HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CTx_XPT & tx_xptParams)
{
   MSXML2::IXMLDOMNodePtr pTargetNode = pNode->selectSingleNode((_bstr_t)"Tx_XPT");
   if (NULL == pTargetNode)  return false;

   RFCal::Helper::XML::Read_Param(pTargetNode, "Seq_Bld_Seg_Dur",    tx_xptParams.Seq_Bld_Seg_Dur);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Seq_Exec_Seg_Dur",   tx_xptParams.Seq_Exec_Seg_Dur);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Poll_Interval",      tx_xptParams.Poll_Interval);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Poll_Timeout",       tx_xptParams.Poll_Timeout);
   RFCal::Helper::XML::Read_Param(pTargetNode, "XPT_Mode_List",      tx_xptParams.XPT_Mode_List);

   /* EPT_Params */
   pTargetNode = pNode->selectSingleNode((_bstr_t)"Tx_XPT/EPT_Params");
   XPT_Params ( hPhone, pTargetNode, tx_xptParams.EPT_Params );

   /* ET_Params */
   pTargetNode = pNode->selectSingleNode((_bstr_t)"Tx_XPT/ET_Params");
   XPT_Params ( hPhone, pTargetNode, tx_xptParams.ET_Params );

   return true;
}

 unsigned char RFCal::QSC::Read::Node::IP2_Cal(HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pNode, CRx_IP2 & ip2Params)
 {
   MSXML2::IXMLDOMNodePtr pTargetNode = pNode->selectSingleNode((_bstr_t)"IP2_Cal");
   if (NULL == pTargetNode)  return false;

   RFCal::Helper::XML::Read_Param(pTargetNode, "Poll_Interval",      ip2Params.Poll_Interval);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Poll_Timeout",       ip2Params.Poll_Timeout);

   return true;
 } 
 
unsigned char RFCal::QSC::Read::Node::XPT_Params(HANDLE hPhone, const MSXML2::IXMLDOMNodePtr & pTargetNode, CXPT_Params & xptParam)
{
   if (NULL == pTargetNode)  return false;

   RFCal::Helper::XML::Read_Param(pTargetNode, "XPT_Seq_Action",              xptParam.XPT_Seq_Action);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Get_Addn_XPT_Dbg_Info",       xptParam.Get_Addn_XPT_Dbg_Info);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Enable_Memory_Kernel",        xptParam.Enable_Memory_Kernel);
   RFCal::Helper::XML::Read_Param(pTargetNode, "PA_State",                    xptParam.PA_State);
   RFCal::Helper::XML::Read_Param(pTargetNode, "RgiList",                     xptParam.RgiList);
   RFCal::Helper::XML::Read_Param(pTargetNode, "BiasList",                    xptParam.BiasList);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Exp_Pwrs",                    xptParam.Exp_Pwrs);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Rgi_Delta_SSS",               xptParam.Rgi_Delta_SSS);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Meas_Pwr_List",               xptParam.Meas_Pwr_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Calc_Pwr_List",               xptParam.Calc_Pwr_List);
   RFCal::Helper::XML::Read_Param(pTargetNode, "Fcomp_meas_pwr_index_list",   xptParam.Fcomp_meas_pwr_index_list);

   return true;
}

unsigned char RFCal::QSC::GetCalibratedRFMPathInfo(HANDLE hPhone, 
                                                   const QSCSwpParams & swpParams, 
                                                   const CBandInfo & bandInfo,
                                                   std::vector < CRFPathInformationPacket > & aggregatedResult,
                                                   const std::vector < ACTION_TYPE > & actionType,
                                                   const std::vector < PATH_TYPE > & pathType )
{
   std::vector < CRFPathInformationPacket > txResults; // keep for now
   std::vector < CRFPathInformationPacket > rxResults; // keep for now

   const int tech = swpParams.tech;
   //Get TX and RX paths to be calibrated from RFC data
   vector<ACTION_TYPE> actionCriteria = actionType;
   
   bool bTxPath = ( std::find ( pathType.begin(), pathType.end(), PATH_TYPE_TX) !=  pathType.end() );
   bool bPRxPath = ( std::find ( pathType.begin(), pathType.end(), PATH_TYPE_PRX) !=  pathType.end() );
   bool bDRxPath = ( std::find ( pathType.begin(), pathType.end(), PATH_TYPE_DRX) !=  pathType.end() );

   //TX paths
   if ( bTxPath ) // If Tx is found
   {
      vector<PATH_TYPE> pathCriteria;
      pathCriteria.push_back(PATH_TYPE_TX);
      swpParams.repoPtr->rfmPathDB.QueryRFMPathInfoByPathType(hPhone, tech, bandInfo.band, bandInfo.subBand, pathCriteria, actionCriteria, txResults);
      swpParams.repoPtr->rfmPathDB.QueryRFMPathInfoByPathType(hPhone, tech, bandInfo.band, bandInfo.subBand, pathCriteria, actionCriteria, aggregatedResult);
      if ( aggregatedResult.empty())
      {
       //  return false; // Not found for Tx device
      } 
      else if ( ( SEQ_CAL_CONNECTED == swpParams.sweep_type && 0 == swpParams.calType.Enable_Multiple_FBRx_Path_Cal )
         || ( SEQ_CAL_UNCONNECTED == swpParams.sweep_type && 0 == swpParams.calType.Enable_Multiple_Tx_Path_Cal ) )
      {
         aggregatedResult.resize(1);
      }
      
      rxResults.clear();

      /* For each Tx Path, push paired PRx Path. It is necessary to calibrate Tx with the correct associated PRx Path returned from RFC*/
      for(std::vector < CRFPathInformationPacket >::iterator it = txResults.begin(); it != txResults.end(); it++)
      {
         CRFPathInformationPacket associatedPath;
         if (swpParams.repoPtr->rfmPathDB.QueryAssociatedRFMPathInfo(hPhone, tech,  bandInfo.band, bandInfo.subBand, (*it), associatedPath))
         {
            rxResults.push_back(associatedPath);
         }
      }
   }

   //RX paths
   /*
   One PRX path may be referred by 0 or multiple DRX paths as "associated path".
   If it is not referred by any DRX, it is a stand-alone PRX.
   */
   
   bool combinedPRxDRx = bPRxPath && bDRxPath;

   if( bPRxPath || bDRxPath)
   {
      vector<PATH_TYPE> pathCriteria;
      if(combinedPRxDRx)
      {
         // Query PRX indices into result vector
         vector < CRFPathInformationPacket > prxIndices;

         pathCriteria.clear();
         pathCriteria.push_back(PATH_TYPE_PRX);
         
         swpParams.repoPtr->rfmPathDB.QueryRFMPathInfoByPathType(hPhone, tech, bandInfo.band, bandInfo.subBand, pathCriteria, actionCriteria, prxIndices);
     
         // Store PRx device which is NOT associated Tx or all PRx device (if txEnabled == false)
         for(std::vector < CRFPathInformationPacket >::iterator it = prxIndices.begin(); it != prxIndices.end(); it++)
         {
            // Check rxResult includes the RFM path (*it), if not, insert the RFM path
            if ( ! RFCal::RFMPathDB::IsRFMPathIncluded(hPhone, (*it), rxResults ))
            {
               rxResults.push_back(*it);
               aggregatedResult.push_back(*it);
            }
         }

         //query DRX indices, search for stand-alone DRX, then add them into result vector.
         vector < CRFPathInformationPacket > drxIndices;
         pathCriteria.clear();
         pathCriteria.push_back(PATH_TYPE_DRX);

         swpParams.repoPtr->rfmPathDB.QueryRFMPathInfoByPathType(hPhone, tech, bandInfo.band, bandInfo.subBand, pathCriteria, actionCriteria, drxIndices);
     
         /* Handling case where number of DRx Paths is greater than number of PRx paths */
         /* Find out DRx path which is not associated with PRx */
         for(std::vector < CRFPathInformationPacket >::iterator it = drxIndices.begin(); it != drxIndices.end(); it++)
         {
            bool pushDRxPathforAggregation = true;
            for(std::vector < CRFPathInformationPacket >::iterator rx_result_it = rxResults.begin(); rx_result_it != rxResults.end(); rx_result_it++)
            {
               if (  (rx_result_it->associatedRxSignalPath  == it->signalPath )
                  && (rx_result_it->associatedRxRFMDevice   == it->rfmDevice )
                  && (rx_result_it->associatedRxAntennaPath == it->antennaPath ))
               {
                  pushDRxPathforAggregation = false;
                  break;
               }
            }
            if( true == pushDRxPathforAggregation )
            {
               rxResults.push_back(*it);
               aggregatedResult.push_back(*it);
            }
         }
      }
      else
      {
         pathCriteria.clear();
         if( bPRxPath )
         {
            //query PRX indices into result vector
            vector < CRFPathInformationPacket > prxIndices;
            pathCriteria.clear();
            pathCriteria.push_back(PATH_TYPE_PRX);
      
            swpParams.repoPtr->rfmPathDB.QueryRFMPathInfoByPathType(hPhone, tech, bandInfo.band, bandInfo.subBand, pathCriteria, actionCriteria, prxIndices);
     
            // Store PRx device which is NOT associated Tx or all PRx device (if txEnabled == false)
            for(std::vector < CRFPathInformationPacket >::iterator it = prxIndices.begin(); it != prxIndices.end(); it++)
            {
               // Check rxResult includes the RFM path (*it), if not, insert the RFM path
               if ( ! RFCal::RFMPathDB::IsRFMPathIncluded(hPhone, (*it), rxResults ))
               {
                  rxResults.push_back(*it);
                  aggregatedResult.push_back(*it);
               }
            }
         }
         else
         {
            //query DRX indices, search for stand-alone DRX, then add them into result vector.
            vector < CRFPathInformationPacket > drxIndices;

            pathCriteria.clear();
            pathCriteria.push_back(PATH_TYPE_DRX);      
            swpParams.repoPtr->rfmPathDB.QueryRFMPathInfoByPathType(hPhone, tech, bandInfo.band, bandInfo.subBand, pathCriteria, actionCriteria, drxIndices);

            /* Handling case where number of DRx Paths is greater than number of PRx paths */
            /* Find out DRx path which is not associated with PRx */
            for(std::vector < CRFPathInformationPacket >::iterator it = drxIndices.begin(); it != drxIndices.end(); it++)
            {
               bool pushDRxPathforAggregation = true;
               for(std::vector < CRFPathInformationPacket >::iterator rx_result_it = rxResults.begin(); rx_result_it != rxResults.end(); rx_result_it++)
               {
                  if (  (rx_result_it->associatedRxSignalPath  == it->signalPath )
                     && (rx_result_it->associatedRxRFMDevice   == it->rfmDevice )
                     && (rx_result_it->associatedRxAntennaPath == it->antennaPath ))
                  {
                     pushDRxPathforAggregation = false;
                     break;
                  }
               }
               if( true == pushDRxPathforAggregation )
               {
                  rxResults.push_back(*it);
                  aggregatedResult.push_back(*it);
               }
            }
         }
      }
   }

   return true;
}
unsigned char RFCal::QSC::FBRxResultDB::Init()
{
   this->rfmPathDB.clear();
   return true;
}
unsigned char RFCal::QSC::FBRxResultDB::SetFBRxResult (HANDLE hPhone, const QSCNode & qscNode, 
              const CRFM_Path & rfmPath, const ftm_fbrxgain_path_result & result)
{
   QSCNode tmpQsc = qscNode;
   tmpQsc.sweep_type = 0;
   tmpQsc.instance = 0;
   this->rfmPathDB[tmpQsc][rfmPath] = result;
   return true;
}

 unsigned char RFCal::QSC::FBRxResultDB::GetFBRxResult ( HANDLE hPhone, const QSCNode & qscNode, std::map < CRFM_Path, ftm_fbrxgain_path_result > & fbrx_rfmPath_ResultMap )
 {
   fbrx_rfmPath_ResultMap.clear();
   QSCNode tmpQsc = qscNode;
   tmpQsc.sweep_type = 0;
   tmpQsc.instance = 0;
   std::map < QSCNode, std::map < CRFM_Path, ftm_fbrxgain_path_result > >::iterator it;
   it = this->rfmPathDB.find(tmpQsc);
   if ( it != this->rfmPathDB.end() )
   {
      fbrx_rfmPath_ResultMap = it->second;
      return true;
   }
   return false;
}
 unsigned char RFCal::QSC::TestTime::Init()
 {
    this->qscnode_elappasedTimeMap.clear();
    return true;
 }
 unsigned char RFCal::QSC::TestTime::Start ( HANDLE hPhone )
 {
    this->elapsedTime = GetTickCount();
    return true;
 }
 unsigned char RFCal::QSC::TestTime::End ( HANDLE hPhone, const QSCNode & qsc_node )
 {
    DWORD curTime = GetTickCount();
    unsigned long diff = static_cast <unsigned long> ( ( curTime > this->elapsedTime )? (curTime - this->elapsedTime): (this->elapsedTime - curTime ));
    this->qscnode_elappasedTimeMap [ qsc_node ] = diff;
    return true;
 }
 unsigned char RFCal::QSC::TestTime::GetData ( HANDLE hPhone, const QSCNode & qsc_node, unsigned long  & elapsedTime)
 {
    elapsedTime = 0;
    std::map < QSCNode, unsigned long >::const_iterator itr =  this->qscnode_elappasedTimeMap.find ( qsc_node );
    if (this->qscnode_elappasedTimeMap.end() != itr )
    {
       elapsedTime = itr->second;
       return true;
    }
    else
    {
      return false;
    }
 }
 unsigned char RFCal::QSC::RxCharDB::Init()
 {
    this->rxCharData.clear();
    return true;
 }
 unsigned char RFCal::QSC::RxCharDB::RegisterRxCharData (  HANDLE hPhone, CTechBand_PathChanResults & result )
 {
    this->rxCharData = result;
    return true;
 }
 unsigned char RFCal::QSC::RxCharDB::GetRxCharBandData ( HANDLE hPhone, const QSCNode & techBand,CPath_ChanResults & result )
 {
    CTechBand_PathChanResults::iterator band_itr =  this->rxCharData.find ( techBand ) ;
    if ( this->rxCharData.end () == band_itr )
    {
       return false;
    }
    result = band_itr->second;
    return true;
 }
 unsigned char RFCal::QSC::RxCharDB::GetRxCharData ( HANDLE hPhone, const QSCNode & techBand, const CRFM_Path & rfmPath, int channel, int gain_state, int & agc_offset )
 {
    CTechBand_PathChanResults::iterator band_itr =  this->rxCharData.find ( techBand ) ;
    if ( this->rxCharData.end () == band_itr )
    {
       return false;
    }
    std::map < CRFM_Path, CChan_RxResults >::iterator path_itr = band_itr->second.map.find ( rfmPath );
    if ( band_itr->second.map.end() == path_itr )
    {
       return false;
    }
    CChan_RxResults::iterator chan_itr = path_itr->second.find ( channel );
    if (  path_itr->second.end() == chan_itr )
    {
       return false;
    }
    CGainState_AgcOffset::iterator gain_itr = chan_itr->second.find ( gain_state );
    if ( chan_itr->second.end() == gain_itr )
    {
       return false;
    }
    agc_offset = gain_itr->second;
    return true;
 }
unsigned char RFCal::QSC::Cal_Status_DB::Init()
{
   this->cal_status_records.clear();
   return true;
}

unsigned char RFCal::QSC::Cal_Status_DB::SetData ( HANDLE hPhone, const QSCNode & qsc_node, seq_cal_res_type cal_type,  const CCal_Status & cal_status, int type )
{
   QSCNode tmpQSCNode;
   tmpQSCNode.tech = qsc_node.tech;
   tmpQSCNode.band = qsc_node.band;
   tmpQSCNode.subBand = qsc_node.subBand;

   std::map < QSCNode, std::map < seq_cal_res_type , CCal_Status > >::iterator itr = this->cal_status_records.find ( tmpQSCNode );

   if ( this->cal_status_records.end() != itr )
   {
      if ( RFCal::QSC::Cal_Status_DB::ALL ==  type )
      {
         std::map < seq_cal_res_type , CCal_Status >::iterator tmpItr = itr->second.find ( cal_type );
         itr->second [ cal_type ] = cal_status;
      }
      else
      {
         std::map < seq_cal_res_type , CCal_Status >::iterator tmpItr = itr->second.find ( cal_type );
         if ( itr->second.end () != tmpItr )
         {
            switch ( type )
            {
               case RFCal::QSC::Cal_Status_DB::STATUS:
                  tmpItr->second.nvid = cal_status.nvid;
                  break;
               case RFCal::QSC::Cal_Status_DB::NVID:
                  tmpItr->second.status = cal_status.status;
                  break;
               case RFCal::QSC::Cal_Status_DB::TIME:
                  tmpItr->second.cal_time = cal_status.cal_time;
                  break;
            }
         }
         else
         {
//            std::map < seq_cal_res_type , CCal_Status > & tmpMap = 
               itr->second.insert ( make_pair ( cal_type, cal_status ) );
  //          tmpMap [ cal_type ] = cal_status;
    //        this->cal_status_records [ tmpQSCNode ] = tmpMap;
         }
      }
   }
   else
   { 
      std::map < seq_cal_res_type , CCal_Status > tmpMap;
      tmpMap [ cal_type ] = cal_status;
      this->cal_status_records [ tmpQSCNode ] = tmpMap;
   }
   return true;
}
unsigned char  RFCal::QSC::Cal_Status_DB::GetTechAndBand ( HANDLE hPhone, vector < QSCNode > & qsc_nodes )
{
   qsc_nodes.clear();
   qsc_nodes.reserve ( this->cal_status_records.size() );

   for (  std::map < QSCNode, std::map < seq_cal_res_type , CCal_Status > >::const_iterator itr = this->cal_status_records.begin();
      itr != this->cal_status_records.end(); itr ++ )
   {
      qsc_nodes.push_back ( itr->first );
   }
   
   return true;
}
unsigned char  RFCal::QSC::Cal_Status_DB::GetData ( HANDLE hPhone, const QSCNode & qsc_node, seq_cal_res_type cal_type,  CCal_Status & cal_status )
{
   QSCNode tmpQSCNode;
   tmpQSCNode.tech = qsc_node.tech;
   tmpQSCNode.band = qsc_node.band;
   tmpQSCNode.subBand = qsc_node.subBand;

   std::map < QSCNode, std::map < seq_cal_res_type , CCal_Status > >::const_iterator itr = this->cal_status_records.find ( tmpQSCNode );

   memset ( & cal_status, 0, sizeof ( CCal_Status ) );

   if ( this->cal_status_records.end() == itr )
   {
      return false;     
   }
   std::map < seq_cal_res_type , CCal_Status >::const_iterator tmpItr = itr->second.find ( cal_type );

   if ( itr->second.end() == tmpItr )
   {
      return false;
   }

   // Copy the result
   cal_status = tmpItr->second;

   return true;
}
unsigned char RFCal::PA_PARAM_DB::Init()
{
   this->pa_param_records.clear();
   return true;
}
unsigned char RFCal::PA_PARAM_DB::SetData ( const PA_PARAM_KEY & key, const std::map < CXptInput_V1_Key, CXptInput_V1_BW_V > & xptInput_Record_Map)
{
    this->pa_param_records [ key ] = xptInput_Record_Map;
    return true;
}
unsigned char RFCal::PA_PARAM_DB::GetData ( const PA_PARAM_KEY & key, const int tech, const int band, const int subband, CXptInput_V1_BW_V & xptInput )
{
   unsigned char bFound = false;
   int rfmode = RFCal::Helper::Convert::GetRFMode ( tech, band, subband );
  
   std::map < PA_PARAM_KEY, std::map < CXptInput_V1_Key, CXptInput_V1_BW_V > >::iterator it = this->pa_param_records.find ( key );
   if ( it != this->pa_param_records.end() )
   {
      CXptInput_V1_Key key;
      key.rf_mode = rfmode;
      key.instance = 0;
      key.device = 0;
      std::map < CXptInput_V1_Key, CXptInput_V1_BW_V >::iterator xptIt = it->second.find ( key );
      if ( xptIt != it->second.end() )
      {
         xptInput = xptIt->second;
         bFound = true;
      }
   }
   return bFound;
}
unsigned char RFCal::PA_PARAM_DB::GetData ( const PA_PARAM_KEY & key, const int tech, const int band, const int subband, int bw, CXptInput_V1 & xptInput )
{
   unsigned char bFound = false;
   CXptInput_V1_BW_V xptInput_v;

   if ( this->GetData ( key, tech, band, subband, xptInput_v ) )
   {
      for ( CXptInput_V1_BW_V::iterator it=xptInput_v.begin(); it!=xptInput_v.end(); ++it )
	   {
         // Find pa_param for each of bandwidth
		   if (  (( 3 == bw || 10 == bw ) && ( 10 == it->BW ))
            || (( 5 == bw || 40 == bw  ) && ( 40 == it->BW ))  )
		   {
			   xptInput=*it;
            bFound = true;
			   break;
		   }
	   }
       /// workaround in the case PA_PARAM does not have 40 MHZ parameter, use 10 Mhzh
      if ( !bFound )
      {
         for ( CXptInput_V1_BW_V::iterator it=xptInput_v.begin(); it!=xptInput_v.end(); ++it )
	      {
            // Find pa_param for each of bandwidth
		      if ( 10 == it->BW )
		      {
			      xptInput=*it;
               bFound = true;
			      break;
		      }
	      }
      }
   }
  


   return bFound;
}

unsigned char RFCal::AptCharV3::Init () 
{
   this->apt_key_recordset.clear(); 
   return true;
}
unsigned char RFCal::AptCharV3::SetData ( const APT_CharV3_Key & key, const std::set < AptRecord > & aptRecords ) 
{
   this->apt_key_recordset[key] = aptRecords; 
   return true;
}

unsigned char RFCal::AptCharV3::GetData ( int tech, int band, int subband, int channel, int tx_purpose, std::map < int, std::set < AptRecord > > & pa_aptRecords )
{
   std::map < APT_CharV3_Key, std::set < AptRecord > >::iterator it = this->apt_key_recordset.begin();
   int rfmode = RFCal::Helper::Convert::GetRFMode ( tech, band, subband );

   pa_aptRecords.clear();
   int txPurpose = tx_purpose;
 /*  if ( 3 == tech || 10 == tech ) 
   { 
      txPurpose = 1; 
   }
   */
   for ( ; it != this->apt_key_recordset.end(); it++ )
   {
      if ( ( it->first.rf_mode == rfmode )
         && ( it->first.channel == channel )
         && ( it->first.device == 0 )
         && ( it->first.tx_purpose == txPurpose )
         )
      {
         pa_aptRecords [ it->first.pa_state ] = it->second;
      }
   }

   if ( pa_aptRecords.empty() )
   {
      it = this->apt_key_recordset.begin();
      for ( ; it != this->apt_key_recordset.end(); it++ )
      {
         if (  it->first.rf_mode == rfmode )
         {
            pa_aptRecords [ it->first.pa_state ] = it->second;
         }
      }
   }

   return !pa_aptRecords.empty();
}

unsigned char RFCal::XOCal::DO_XO_Cal(HANDLE hPhone, int tech, int band, int subband, 
   int channel, ftm_xo_cal_parameter_type & ftm_xo_cal_params, unsigned char query_path_from_rfc, RFMPathDB * rfmPathDBPtr)
{
   // ************************************************ //
   // Equipment Configuration (Must be setup in advance)
   // ************************************************ //
   RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();
   if ( NULL == pMgr )
   {
      return false;
   }
   pMgr->Set_Tech(static_cast<int>(tech));
   pMgr->Set_Band(static_cast<int>(band));

   unsigned short segDuration = 60000;
   unsigned long handle_id = 0;

   // Assign Parameters;
   ftm_xo_cal_command_type xo_cal_command = XO_DC_COARSE_CAL;
   unsigned char xo_cal_version = 3;

   if (1 == query_path_from_rfc && rfmPathDBPtr) // Get RFM Path info using RFC
   {
      // ************************************************ //
      // Get RFM Path info and save it to the data base
      // ************************************************ //
      if (!(rfmPathDBPtr->CreateRFMPathDB( hPhone, tech, -1 /* all band */, subband, 1 /* Calibrated Path only */ )))
      {
         cout << "Failed in getting RFM Path info " << endl;
         return false;
      }

      // Set Calibration state ON
      QLIB_FTM_SET_CALIBRATION_STATE(hPhone, true);
     
      // ************************************************ //
      // Radio Setup using SeqBuild 0x40000
      // ************************************************ //
      if (!RFCal::QSC::ExecuteRadioSetup(hPhone, rfmPathDBPtr, segDuration, handle_id, tech, band, subband, channel, 0 /* Rx only */))
      {
         cout << "Failed";
         return false;
      }
   }
   else
   {
      // traditional FTM SET MODE
   }
   // ************************************************ //
   // Equipment - Generate DL 
   // ************************************************ //

   pMgr->Set_DL_Mod_Type(MOD_TYPE_RFCAL_CW);
   pMgr->Set_DL_Channel(channel, (ftm_xo_cal_params.offset/1000.0)); // 200 kHz offset
   pMgr->Set_DL_Level(-60.0);
   pMgr->Set_DL_State( true );
   
   // ************************************************ //
   // Execute FTM_XO_CALIBRATION
   // ************************************************ //
   FTM_XO_Cal_Response_v2 ftm_xo_cal_response;
   RFCal::XOCal::UUT_Do_XO_Calibration(hPhone, xo_cal_command, xo_cal_version, ftm_xo_cal_params,ftm_xo_cal_response);

   // ************************************************ //
   // Add XO NV to NV Manager and generate XQCN (optional)
   // ************************************************ //
   
   // Create XO NV data by converting ftm_xo_cal_response to XO_CAL_NV data structure
   QMSL_RFCAL_NV_Item xo_nv;
   memset (&xo_nv.aNVItemData[0],0,128);
   memcpy_s(&xo_nv.aNVItemData[0], sizeof(ftm_tcxomgr_cal_fac_data_type), (void *)&(ftm_xo_cal_response.cal_coef), sizeof(ftm_tcxomgr_cal_fac_data_type));
   
   nv_xo_cal_fac_data_type * xo_nv_ptr = (nv_xo_cal_fac_data_type *)&xo_nv.aNVItemData[0];
   xo_nv_ptr->ft_qual_ind = static_cast < word > ( RFCal::Helper::Convert::XO_CAL_FT_QUAL_TO_NV ( ftm_xo_cal_response.cal_coef.ft_qual_ind ));
   xo_nv_ptr->xo_trim = ftm_xo_cal_response.cal_coef.xo_trim;
   
   xo_nv.iNumOfBytes = 128; // Legacy NV size
   xo_nv.iNVenum = 4954; // XO NV ID
   xo_nv.iNumOfValidBytesInRFItemData = 0;
   
   // Add the XO NV to NV manager
   QLIB_RFCAL_NV_Manager_AddNVItem(hPhone, (void *)&xo_nv);
  
   // Generate XQCN
   QLIB_RFCAL_NV_Manager_Create_QCN( hPhone, "c:\\temp\\Xo.xqcn"  );
   
   // Clear data
   QLIB_RFCAL_NV_Manager_Clear_Data( hPhone);


   if (1 == query_path_from_rfc)
   {
      // ************************************************ //
      // Tear Down using SeqBuild 0x40000
      // ************************************************ //
      //handle_id ++;
      RFCal::QSC::ExecuteTeardown(hPhone, segDuration, handle_id, tech);
   }

   pMgr->Set_DL_State( false );

   // ************************************************ //
   // Logging Result
   // ************************************************ //
   LogResults(hPhone, tech, band, subband, channel,ftm_xo_cal_response);
   RFCal::COMMON::SYNC_EFS( hPhone );

   ::Sleep (200);
   RFCal::Helper::DIAG::Change_FTM_Mode(hPhone, 3, 200);
   QLIB_IsPhoneConnected (hPhone);
   return true;
}
unsigned char RFCal::XOCal::AssignParameters(HANDLE hPhone,int tech, int band, int channel, 
   ftm_xo_cal_parameter_type& ftm_xo_cal_params)
{
   ftm_xo_cal_params.max_ft_samples = 100;
   ftm_xo_cal_params.min_ft_samples = 5;
   ftm_xo_cal_params.offset = 200*1000; /* Unit Hz */
   double dlfrequency, ulfrequency;
   RFCal::Helper::Convert::ULchannel_To_Frequency (tech, band, channel, ulfrequency, dlfrequency);
   ftm_xo_cal_params.rx_freq_khz = static_cast<dword>(dlfrequency*1000);
   ftm_xo_cal_params.substage_time = 10;
   ftm_xo_cal_params.technology = tech;
   ftm_xo_cal_params.temp_grad_timeout = 10;
   ftm_xo_cal_params.temp_span = 0;
   ftm_xo_cal_params.timeout_time = 30000;
   ftm_xo_cal_params.update_nv = 3; /* 3: RAM and NV */
   ftm_xo_cal_params.wb_id = 0;
   ftm_xo_cal_params.xtal = 1;
   return true;
}
unsigned char RFCal::XOCal::UUT_Do_XO_Calibration(HANDLE hPhone, const ftm_xo_cal_command_type xo_cal_command,
      const unsigned char xo_cal_version,
      const ftm_xo_cal_parameter_type& ftm_xo_cal_params,
      FTM_XO_Cal_Response_v2& ftm_xo_cal_response)
{
   unsigned char status = QLIB_FTM_XO_CALIBRATION( hPhone,
                                                   (word)xo_cal_command,
                                                   (byte)xo_cal_version,
                                                   (void*)&ftm_xo_cal_params,
                                                   (void*)&ftm_xo_cal_response );
   
   return status;
}
unsigned char RFCal::XOCal::LogResults(HANDLE hPhone,int tech, int band, int subband, unsigned long channel,FTM_XO_Cal_Response_v2& ftm_xo_cal_response)
{

   stringstream ss;
   ss << "XO Result: ";
   if (1 == ftm_xo_cal_response.state)
      ss << "Success";
   else
      ss << "Failed.";
   RFCal::Helper::Log::AddMessageToQMSLLog(hPhone,"****************************************************************");
   RFCal::Helper::Log::AddMessageToQMSLLog(hPhone,(ss.str()));
   RFCal::Helper::Log::AddMessageToQMSLLog(hPhone,"****************************************************************");
   ss.str("");
   ss << endl << "Status: " << static_cast<int>(ftm_xo_cal_response.state) << endl;
   
   ss << "CalCoeffC0: " << static_cast<int>(ftm_xo_cal_response.cal_coef.c0) << endl;
   ss << "CalCoeffC1: " << static_cast<int>(ftm_xo_cal_response.cal_coef.c1) << endl;
   ss << "CalCoeffC2: " << static_cast<int>(ftm_xo_cal_response.cal_coef.c2) << endl;
   ss << "CalCoeffC3: " << static_cast<int>(ftm_xo_cal_response.cal_coef.c3) << endl;
   ss << "FTQualInd: " << static_cast<int>(ftm_xo_cal_response.cal_coef.ft_qual_ind) << endl;
   ss << "InitXOTrim: " << static_cast<int>(ftm_xo_cal_response.tcxomgr_cal_info_pkt.init_xo_trim) << endl;
   ss << "PostDCXOTrim: " << static_cast<int>(ftm_xo_cal_response.tcxomgr_cal_info_pkt.dc_xo_trim) << endl;
   ss << "PostFinXOTrim: " << static_cast<int>(ftm_xo_cal_response.tcxomgr_cal_info_pkt.fin_xo_trim) << endl;
   ss << "PostFinalCalXOTrim: " << static_cast<int>(ftm_xo_cal_response.tcxomgr_cal_info_pkt.final_xo_trim) << endl;
   ss << "PATempInit: " << static_cast<int>(ftm_xo_cal_response.tcxomgr_cal_info_pkt.init_temp.pa) << endl;
   ss << "PATempFinal: " << static_cast<int>(ftm_xo_cal_response.tcxomgr_cal_info_pkt.final_temp.pa) << endl;
   ss << "XOTempInit: " << static_cast<int>(ftm_xo_cal_response.tcxomgr_cal_info_pkt.init_temp.xo) << endl;
   ss << "XOTempFinal: " << static_cast<int>(ftm_xo_cal_response.tcxomgr_cal_info_pkt.final_temp.xo) << endl;
  
   RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone,(ss.str()) );
   return true;
}
unsigned char RFCal::RFMPathDB::ConvertRFPathInfoData(CRFPathInformationPacket & newVersion, const RFPathInformationPacketVersion0 & oldVer)
{
   memset(&newVersion, 0, sizeof(CRFPathInformationPacket));
   newVersion.band = oldVer.band;
   newVersion.subBand = oldVer.subBand;
   newVersion.signalPath = oldVer.signalPath;
   newVersion.rfmDevice = oldVer.rfmDevice;
   newVersion.antennaPath = oldVer.antennaPath;
   newVersion.ip2cal_cfg_bitmask = 0;//oldVer.reserved;

   // Associated Rx path
   newVersion.associatedRxSignalPath = oldVer.associatedSignalPath;
   newVersion.associatedRxRFMDevice = oldVer.associatedRFMDevice;
   newVersion.associatedRxAntennaPath = oldVer.associatedantennaPath;
   //newVersion.sigpathcapability = 0;// already zero by memset

   // Associated Tx path
   newVersion.associatedTxSignalPath = oldVer.associatedSignalPath;
   newVersion.associatedTxRFMDevice = oldVer.associatedRFMDevice;
   newVersion.associatedTxAntennaPath = oldVer.associatedantennaPath;
   newVersion.uniquePathNumber = oldVer.associatedreserved;

   // Associated Tx path
   newVersion.associatedTxSignalPath = oldVer.associatedSignalPath;
   newVersion.associatedTxRFMDevice = oldVer.associatedRFMDevice;
   newVersion.associatedTxAntennaPath = oldVer.associatedantennaPath;
   newVersion.uniquePathNumber = oldVer.associatedreserved;

   newVersion.nvcontainer = oldVer.nvcontainer;
   newVersion.rfmPathType = oldVer.rfmPathType; /*!< Defines Type of RFM Path; 0 = PRx; 1 = DRx 2 = Tx */
   newVersion.txrxcalaction = oldVer.actionType; /*!< 0 - Characterization 1 - Calibration 2 - Offset Calibration  */

   newVersion.calibrationreferenceSignalPath = 0;
   newVersion.rxrsbcalaction = oldVer.actionType; /*!< 1 = Calibration 0 = Characterization 2 = offset calibration */

   newVersion.antennaNumber = 0;
   newVersion.antennaConfigState = 0;
   return true;
}
unsigned char RFCal::RFMPathDB::CreateRFMPathDB(HANDLE hPhone, int tech, int band, int subband, unsigned char version)
{ 
   RFPathInformationHead req;
   req.technology = tech;
   req.band = band;
   req.subBand = subband;
   req.version = version;
   this->version = version;
   RFPathInformationResponse response;
   unsigned char status = QLIB_FTM_GET_RF_PATH_INFORMATION(hPhone, (RFPathInformationRequest *)& req, &response);

   // Remove path data for the requested band
   for ( map < RFPathInformationHead, vector < CRFPathInformationPacket > >::iterator it = rfmPathDB.begin();
      it != rfmPathDB.end(); it++)
   {
      if (req.technology == it->first.technology && (-1 == req.band || req.band == it->first.band))
      {
         it->second.resize(0);
      }
   }
   if ( status )
   {
      // Verion = 0
      if ( response.responsePayload.numberofPackets > 0 && 0 == response.responsePayload.packetVersion)
      {
         RFPathInformationHead temp = req;

         for (uint16 idx = 0; idx < response.responsePayload.numberofPackets; idx++)
         {
            RFPathInformationPacketVersion0 * data = &(response.responsePayload.packet[idx].version0);
            temp.band = data->band;
            temp.subBand = data->subBand;
 
            vector < CRFPathInformationPacket > * vec = &(rfmPathDB[temp]);
            CRFPathInformationPacket rfmInfo;
            RFCal::RFMPathDB::ConvertRFPathInfoData(rfmInfo, *data);
            vec->push_back(rfmInfo);
         }
      }
      else if ( response.responsePayload.numberofPackets > 0 && 1 == response.responsePayload.packetVersion)
      {
         RFPathInformationHead temp = req;

         for (uint16 idx = 0; idx < response.responsePayload.numberofPackets; idx++)
         {
            RFPathInformationPacketVersion1 * data = &(response.responsePayload.packet[idx].version1);
            temp.band = data->band;
            temp.subBand = data->subBand;
 
            vector < CRFPathInformationPacket > * vec = &(rfmPathDB[temp]);

            vec->push_back(*(CRFPathInformationPacket *)(data));
         }
      }
   }
   return status;
}

unsigned char RFCal::RFMPathDB::QueryRFMPathInfoByPathType(HANDLE hPhone, int tech, int band, int subband, unsigned char pathType, 
      std::vector < CRFPathInformationPacket > & rfmPaths, unsigned char ActionType)
{
   RFPathInformationHead req;
   req.technology = tech;
   req.band = band;
   req.subBand = subband;
   req.version = ActionType;
   std::vector < CRFPathInformationPacket > & tmpRes = (this->rfmPathDB[req]);

   rfmPaths.clear();
   for (std::vector < CRFPathInformationPacket >::iterator it = tmpRes.begin(); it != tmpRes.end(); it++)
   {
      if (it->rfmPathType == pathType && it->txrxcalaction == ActionType)
      {
         rfmPaths.push_back(*it);
      }
   }

   return !(rfmPaths.empty());
}
/*
Records returned ---
Case 1: both pathCriteria and actionCriteria are not empty - meeting any criteria in pathCriteria, and at the same time, meeting any criteria in actionCriteria.
Case 2: pathCriteria is empty - meeting any criteria in actionCriteria; actionCriteria is empty - meeting any criteria in pathCriteria.
Case 3: both pathCriteria and actionCriteria are empty - all available records.
Please note that resultIndices is not cleared before populating the indices. If the resultIndices vector size is non-zero, this function will just append the path indexes
and not overwrite them.
*/
unsigned char RFCal::RFMPathDB::QueryRFMPathInfoByPathType(HANDLE hPhone, int tech, int band, int subband, vector<PATH_TYPE>& pathCriteria, vector<ACTION_TYPE>& actionCriteria, std::vector < CRFPathInformationPacket > & results)
{
   RFPathInformationHead req;
   req.technology = static_cast < uint8 > ( tech );
   req.band = static_cast < uint16 > ( band );
   req.subBand = static_cast < uint8 > ( subband );
   req.version = 0;//Ignored
   std::vector < CRFPathInformationPacket > & tmpRes = (this->rfmPathDB[req]);

   std::vector < CRFPathInformationPacket > tmpResults;
   results.clear();
   tmpResults.clear();
   tmpResults.reserve(tmpRes.size());

   bool isPathCriteria = !(pathCriteria.empty());
   bool isActionCriteria = !(actionCriteria.empty());

   for (std::vector < CRFPathInformationPacket >::iterator it = tmpRes.begin(); it != tmpRes.end(); it++)
   {
      // Path Criteria Check 
      if (isPathCriteria)
      {
         bool bFound = false;
         for (vector<PATH_TYPE>::iterator path_it = pathCriteria.begin(); path_it != pathCriteria.end(); path_it++)
         {
            if ( TECHNOLOGY_GSM == tech ) // Need to check
            {
               if ((PATH_TYPE_TX == *path_it && 2 == it->rfmPathType )
                  || (PATH_TYPE_PRX == *path_it && ( 1 == it->rfmPathType  ||  0 == it->rfmPathType )))
               {
                  bFound = true;
                  break;
               }
            }
            else if ( *path_it == it->rfmPathType )
            {
               bFound = true;
               break;
            }
          
         }
         if (!bFound)
         {
            continue;
         }
      }

      if (isActionCriteria)
      { 
         bool bFound = false;
         /*!< 1 = Calibration, 0 = Characterization, 2 = offset calibration, 3 = RSB Cal, 4 = RSB offset Cal, 5 = IP2 Cal*/


         for (vector<ACTION_TYPE>::iterator action_it = actionCriteria.begin(); action_it != actionCriteria.end(); action_it++)
         {
            switch (*action_it)
            {
               /* Regular Cal case */
               case ACTION_TYPE_CHAR:
               case ACTION_TYPE_CAL:
               case ACTION_TYPE_OFFSET_CAL:
                  if ((*action_it) == it->txrxcalaction )
            {
               bFound = true;
                  }
                  break;
               /* RSB Cal */
               case ACTION_TYPE_RSB_CAL:
                  if (1 == it->rxrsbcalaction )
                  {
                     bFound = true;
                  }
                  break;

               case ACTION_TYPE_RSB_OFFSET_CAL:
                  if (2 == it->rxrsbcalaction )
            {
               bFound = true;
                  }
                  break;

			   case ACTION_TYPE_IP2_CAL:
				   if ((((it -> ip2cal_cfg_bitmask) >> FTM_RF_PATH_INFORMATION_IP2CAL_ACTION_BSHFT ) & FTM_RF_PATH_INFORMATION_IP2CAL_ACTION_BMASK) != 0)
				   {
					   bFound = true;
				   }
				   break;
               default:
                  break;
            }
            if ( bFound )
            {
               break;
            }
         }
         if (!bFound)
         {
            continue;
         }
      }

      tmpResults.push_back(*it);
   }

   // Remove redundant RFM paths
   results.reserve(tmpResults.size());
   for ( std::vector < CRFPathInformationPacket >::const_iterator itr = tmpResults.begin(); itr != tmpResults.end(); itr++ )
   {
      if ( ! this->IsRFMPathIncluded(hPhone, (*itr), results ))
      {
         results.push_back ( * itr );
      }
   }

   return true;
}
unsigned char RFCal::RFMPathDB::QueryAssociatedRFMPathInfo(HANDLE hPhone, int tech, int band, int subband, 
         const CRFPathInformationPacket & keyRFMPath, CRFPathInformationPacket & associatedRFMPath)
{
   RFPathInformationHead req;
   req.technology = tech;
   req.band = band;
   req.subBand = subband;
   req.version = 0;//Ignored
   std::vector < CRFPathInformationPacket> & tmpRes = (this->rfmPathDB[req]);
   unsigned char bFound = false;
   for (std::vector < CRFPathInformationPacket >::iterator it = tmpRes.begin(); it != tmpRes.end(); it++)
   {
      if (  (it->signalPath == keyRFMPath.associatedRxSignalPath)
         && (it->rfmDevice == keyRFMPath.associatedRxRFMDevice)
         && (it->antennaPath == keyRFMPath.associatedRxAntennaPath))
      {
         // Found and copy
         bFound = true;
         associatedRFMPath = (*it);
         break;
      }
   }
   return bFound;
}
int RFCal::RFMPathDB::GetPathType( HANDLE hPhone, int tech, int band, int subband, const CRFPathInformationPacket & rfmpkt )
{
   RFPathInformationHead req;
   req.technology = tech;
   req.band = band;
   req.subBand = subband;
   req.version = 0;//Ignored
   std::vector < CRFPathInformationPacket > & tmpRes = (this->rfmPathDB[req]);
   int path_type = -1;
   for (std::vector < CRFPathInformationPacket >::iterator it = tmpRes.begin(); it != tmpRes.end(); it++)
   {
      if (  ( it->signalPath == rfmpkt.signalPath)
         && ( it->rfmDevice == rfmpkt.rfmDevice)
         && ( it->antennaPath == rfmpkt.antennaPath))
      {
         // Found and copy
         path_type = static_cast < int > ( it->rfmPathType );
         break;
      }
   }
 
  return path_type;
}

int RFCal::RFMPathDB::GetPathType( HANDLE hPhone, int tech, int band, int subband, const CRFM_Path & rfm_path )
{
   CRFPathInformationPacket rfmpkt;
   rfmpkt.signalPath = static_cast <uint32>(rfm_path.signal_path);
   rfmpkt.rfmDevice = static_cast <uint16>(rfm_path.rfm_dev);
   rfmpkt.antennaPath = static_cast <uint16>(rfm_path.antenna_port);
  
  return GetPathType ( hPhone, tech, band, subband, rfmpkt);
}

unsigned char RFCal::RFMPathDB::IsRFMPathIncluded(HANDLE hPhone, const CRFPathInformationPacket & keyRFMPath, const vector <CRFPathInformationPacket> & vecRFMPath)
{
   unsigned char bFound = false;

   for ( vector < CRFPathInformationPacket >::const_iterator it = vecRFMPath.begin(); it != vecRFMPath.end(); it++)
   {
      if (  (it->signalPath == keyRFMPath.signalPath)
         && (it->rfmDevice == keyRFMPath.rfmDevice)
         && (it->antennaPath == keyRFMPath.antennaPath))
      {
         bFound = true;
         break;
      }
   }
   return bFound;
}
unsigned char RFCal::RFMPathDB::IsSameRFMPath(HANDLE hPhone, const CRFPathInformationPacket & keyRFMPath, const CRFPathInformationPacket & RFMPathToTest)
{
   if (  ( keyRFMPath.signalPath == RFMPathToTest.signalPath)
      && ( keyRFMPath.rfmDevice == RFMPathToTest.rfmDevice)
      && ( keyRFMPath.antennaPath == RFMPathToTest.antennaPath))
   {
      return true;
   }
   return false;
}
unsigned char RFCal::RFMPathDB::IsAssociatedRFMPath(HANDLE hPhone, const CRFPathInformationPacket & keyRFMPath, const CRFPathInformationPacket & RFMPathToTest)
{
   unsigned char bFound = false;
   if (  ( keyRFMPath.associatedRxSignalPath == RFMPathToTest.signalPath)
      && ( keyRFMPath.associatedRxRFMDevice == RFMPathToTest.rfmDevice)
      && ( keyRFMPath.associatedRxAntennaPath == RFMPathToTest.antennaPath) )
   {
      bFound = true;
   }
   else if (( keyRFMPath.associatedTxSignalPath == RFMPathToTest.signalPath)
      && ( keyRFMPath.associatedTxRFMDevice == RFMPathToTest.rfmDevice)
      && ( keyRFMPath.associatedTxAntennaPath == RFMPathToTest.antennaPath) )
   {
      bFound = true;
   }
   return bFound;
}


unsigned char RFCal::SetPathLoss(HANDLE hPhone, CMeasurementManager & cMgr, const string & calDB_filePath, const int tech)
{
   string calConfig = "";

   switch (tech)
   {
      case 1:
      case 3:
      case 4:
      case 10:
      case 11:
         calConfig = "RFCAL_WCDMA"; // "RFCAL_WCDMA"; because AUX 
         break;
      case 2:
         calConfig = "RFCAL_GSM";
         break;
      case 0:
      default:
         calConfig = "RFCAL_CDMA";
         break;
   }
   return SetPathLoss ( hPhone, cMgr, calDB_filePath, calConfig);
}

unsigned char RFCal::SetPathLoss(HANDLE hPhone, CMeasurementManager & cMgr, const string & calDB_filePath, const string & calConfig)
{
   //*********************************************//
   //      Read Path loss from CalDB_Net.xml
   //*********************************************//
   map <double, double> ULLoss;
   map <double, double> DLLoss;
   map <double, double> AUXLoss;

   if (! RFCal::Helper::GetPathLossFromCalDB_XML(hPhone, calDB_filePath,calConfig, 4 /* = CALPATH_DownlinkCP1 */,-1,-1,DLLoss) )
   {
      RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, "Failed in loading main port DL path loss data.");
      return false;
   }
   if (! RFCal::Helper::GetPathLossFromCalDB_XML(hPhone, calDB_filePath,calConfig, 5 /* = CALPATH_UplinkCP1 */,-1,-1,ULLoss) )
   {
      RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, "Failed in loading main port UL path loss data.");
      return false;
   }
   if ( ! RFCal::Helper::GetPathLossFromCalDB_XML(hPhone, calDB_filePath,calConfig, 14 /* = CALPATH_DownlinkAuxCP1 */,-1,-1,AUXLoss) )
   {
      RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, "Failed in loading aux port path loss data.");
      RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, "Use main port downlink path loss as aux port path loss.");

      RFCal::Helper::GetPathLossFromCalDB_XML(hPhone, calDB_filePath,calConfig, 4 /* = CALPATH_DownlinkAuxCP1 */,-1,-1,AUXLoss);
   };
   cMgr.Register_PathLoss(ULLoss,DLLoss,AUXLoss);
   cMgr.Set_PathLoss(ULLoss,DLLoss,AUXLoss);
   return true;
}
 inline unsigned char RFCal::SetOpCode::GetRadioConfigPathType (HANDLE hPhone, uint8 rfm_path)
 {
   uint8 radio_config_type = RFM_PATH_TYPE_INVALID;

   switch ( rfm_path )
   {
      case PATH_TYPE_TX:
         radio_config_type = RFM_PATH_TYPE_TX;
         break;
      case PATH_TYPE_PRX:
         radio_config_type = RFM_PATH_TYPE_PRX;
         break;
      case PATH_TYPE_DRX:
         radio_config_type = RFM_PATH_TYPE_DRX;
         break;
      default:
         break;
   }
   return radio_config_type;
 }
unsigned char RFCal::SetOpCode::NoP ( HANDLE hPhone, cal_sweep_record & swp, const unsigned short segDuration)
{
   swp.isNopSeg = true;
   swp.resExpected = false;

   swp.rxParams.rxActions = (word) 0xFFFF;
   // TxMeas. Device Mask
   swp.txMeasParams.txMeasFlag = 1;
   swp.sync.syncEvent = 0xFFFF; 

   // Sequence Build (new options)
   swp.seqBuild.seqType = SEQ_TYPE_UNDEFINED;
   swp.seqExec.seqType = SEQ_TYPE_UNDEFINED;
   return true;
}
unsigned char RFCal::SetOpCode::Polling (HANDLE hPhone, cal_sweep_record & swp, const unsigned short segDuration, 
   const unsigned int iTimeOut, const uint64 eventMask, const uint32 seqType, const int handle_it )
{
   swp.opcode.segLen = segDuration;
   swp.polling.event_mask = eventMask;
   swp.polling.version = 0;
   swp.polling.timeout_us = iTimeOut;
   swp.polling.flag = 0;
   swp.polling.debugMode = 1;

   if ( swp.polling.event_mask & POLL_EVENT_MASK_SEQ_BLD_DONE )
   {
      swp.polling.seq_bld_parm.handle_id =  handle_it;
      swp.polling.seq_bld_parm.seq_type =  seqType;
   }
   if ( swp.polling.event_mask & POLL_EVENT_MASK_SEQ_EXEC_DONE )
   {
      swp.polling.seq_exec_parm.handle_id =  handle_it;
      swp.polling.seq_exec_parm.seq_type =  seqType;
   }

   return true;
}

unsigned char RFCal::SetOpCode::SeqBuild::RadioSetup(HANDLE hPhone, cal_sweep_record & swp, 
   unsigned short segmentDuration,
   unsigned long handle_id, 
   unsigned char tech, 
   unsigned char isTearDown, 
   const ftm_radio_setup_type & radio_setup_type,
   const ftm_cal_seq_bld_radio_setup_path_params * path_common_param, 
   const std::vector < CRFPathInformationPacket > * rfmPathDB )
{
   /* Set Seq Build Common Parameter */
   FillInCommonParams (hPhone, swp, segmentDuration, SEQ_RADIO_SETUP, handle_id);

   ftm_cal_seq_bld_radio_setup_params * radio_setup = &(swp.seqBuild.radioSetupParams);
   radio_setup->version = 0;
  
   ftm_cal_seq_bld_radio_setup_path_params_v0 * param  = &(radio_setup->radioSetupV0);
   param->tech = static_cast<word>(tech);
   param->isTearDown = isTearDown;
  
   if (0 == isTearDown && NULL != rfmPathDB)
   {
      param->radioSetupType = radio_setup_type; //RADIO_SETUP_TECH_BAND_PATH_CHAN
      
      if (param->numRFMPaths < MAX_NUM_RFM_PATHS)
      {
         param->numRFMPaths = static_cast<unsigned char>(rfmPathDB->size());
      
         for (uint8 idx = 0; idx < param->numRFMPaths ; idx++)
         {
            ftm_cal_seq_bld_radio_setup_path_params * path = &(param->radioSetupPathParams[idx]);
            const CRFPathInformationPacket * pathDB = &(rfmPathDB->at(idx));
            (*path) = (*path_common_param); // Copy path parameter
            path->band = path_common_param->band;
            path->subBand = path_common_param->subBand;

            if ( PATH_TYPE_TX != pathDB->rfmPathType ) // If rfm_path is Rx, then, change channel from UL channel to DL channel
            {
               path->channelList[0] = RFCal::Helper::Convert::ULch_To_DLch ( static_cast < int > (param->tech), static_cast < int > ( path->band ),
                  static_cast < int > ( pathDB->subBand ), static_cast < long > ( path->channelList[0] )); 
            }
            path->rfmPathType = GetRadioConfigPathType (hPhone, pathDB->rfmPathType); // Radio Config type, not rfm path type used in GetRFMPathInfo
            path->rfmPath.rfmDevice = pathDB->rfmDevice;
            path->rfmPath.sigPath = pathDB->signalPath;
            path->rfmPath.antennaSwitchPath = pathDB->antennaPath;
            path->rfmPath.reserved = 0; 
            path->nvContainerId = pathDB->nvcontainer;
         }
      }
   }
   return true;
}
unsigned char RFCal::SetOpCode::Data::PushFBRxResultType2 ( HANDLE hPhone, cal_sweep_record & SwpRecord, unsigned short segmentDuration, int tech, int band, int subband, cal_sweep_fbrx_gain_paths_results * paths_results )
{
   if ( NULL == paths_results )
   {
      return false;
   }

   SwpRecord.opcode.segLen = segmentDuration; //us

   SwpRecord.data.doDataOp = true;
   SwpRecord.data.fbrxAgcTbl.doFbGainAgcOp = true;
   SwpRecord.txPurpose = 255;
   
   SwpRecord.data.tech = static_cast < unsigned char >( tech );
   SwpRecord.data.dev = 0; // Any value is OK
   SwpRecord.data.refChan = -1; // Any value is OK

   SwpRecord.data.debugMode = true;
   SwpRecord.resExpected = true;
   
   // Use dataType = 3 if DC Cal result is available.
   if ( paths_results->numberofRFMPaths > 0 && paths_results->pathResults[0].fbrxDCCaldataavailable )
      SwpRecord.data.fbrxAgcTbl.dataType = PUSH_FBRX_GAIN_DC_CAL; // 3
   else
      SwpRecord.data.fbrxAgcTbl.dataType = PUSH_FBRX_GAIN_CAL; // 2

   if ( NULL != SwpRecord.data.fbrxAgcTbl.ptr_fbrx_cal_data )
   {
      delete SwpRecord.data.fbrxAgcTbl.ptr_fbrx_cal_data;
      SwpRecord.data.fbrxAgcTbl.ptr_fbrx_cal_data = NULL;
   }
   SwpRecord.data.fbrxAgcTbl.ptr_fbrx_cal_data = paths_results;

   SwpRecord.data.fbrxAgcTbl.ptr_fbrx_cal_data->band = static_cast < uint16 > ( band );
   SwpRecord.data.fbrxAgcTbl.ptr_fbrx_cal_data->subband = static_cast < uint8 > ( subband );
   SwpRecord.data.fbrxAgcTbl.ptr_fbrx_cal_data->reserved = 0;

  
   return true;
}
unsigned char RFCal::SetOpCode::Data::ResultCommitAndNV ( HANDLE hPhone, cal_sweep_record & SwpRecord, unsigned short segmentDuration, int tech, int band, int sub_band, int result_mask, int nv_write_mask )
{
   SwpRecord.opcode.segLen = segmentDuration; //us

   SwpRecord.data.doDataOp = true;
   SwpRecord.data.getRfCalResCommitNvReq.do_get_rf_cal_res_commit_nv_op = true;
   SwpRecord.data.dataAction = 0x1000;//DATA_OPCODE_ACTION_RETURN_RF_CAL_RESULTS_NV_COMMIT;
   SwpRecord.txPurpose = 255;
   SwpRecord.data.debugMode = true;
   SwpRecord.data.version = 0;

   SwpRecord.data.tech = static_cast < unsigned char >( tech );
   SwpRecord.data.dev = 0; // Any value is OK
   SwpRecord.data.refChan = 0; // Any value is OK

   SwpRecord.data.getRfCalResCommitNvReq.nv_write_mask = static_cast < dword > ( nv_write_mask );;
   SwpRecord.data.getRfCalResCommitNvReq.result_mask = static_cast < dword > ( result_mask );

   SwpRecord.data.getRfCalResCommitNvReq.band = static_cast < unsigned short >( band );
   SwpRecord.data.getRfCalResCommitNvReq.subband = static_cast < unsigned char >( sub_band );
   SwpRecord.resExpected = true;

   return true;
}
unsigned char RFCal::SetOpCode::SeqBuild::FillInCommonParams (HANDLE hPhone,  cal_sweep_record & swp, 
            const unsigned short segmentDuration,
            const unsigned short seqType,
            const unsigned long handle_id,
            const unsigned char debugMode,
            const unsigned char version,
            const unsigned char supportParallel,
            const unsigned char reserved)
{
   swp.opcode.segLen = segmentDuration;
   swp.resExpected = true;
   swp.seqBuild.handle = handle_id;
   swp.seqBuild.seqType = seqType;
   swp.seqBuild.debugMode = debugMode;
   swp.seqBuild.version = version,
   swp.seqBuild.reserved = reserved;
   swp.seqBuild.supportParallel = supportParallel;
   return true;
}
unsigned char RFCal::SetOpCode::SeqBuild::FullBias (HANDLE hPhone, 
            cal_sweep_record & swp, 
            unsigned short segmentDuration,
            unsigned long handle_id,
            unsigned char version,
            ftm_cal_seq_bld_apt_seq_lin_parameter_type * full_bias_param)
{
   /* Set Seq Build Common Parameter */
   FillInCommonParams (hPhone, swp, segmentDuration, SEQ_APT_CAL, handle_id);

   swp.seqBuild.aptSeq.aptSeqType = APT_SEQ_FULL_BIAS_LIN;
   swp.seqBuild.aptSeq.version = version;

   // Tx data might be big. So directly assign
   if ( NULL != full_bias_param)
   {
      // Copy
      swp.seqBuild.aptSeq.linParams = (* full_bias_param );
   }
   return true;
} 
unsigned char RFCal::SetOpCode::SeqBuild::Xpt (HANDLE hPhone, 
            cal_sweep_record & swp, 
            unsigned short segmentDuration,
            unsigned long handle_id,
            ftm_cal_seq_bld_xpt_params * xptParam)
{
   /* Set Seq Build Common Parameter */
   FillInCommonParams (hPhone, swp, segmentDuration, SEQ_XPT_CAL, handle_id);

   // Tx data might be big. So directly assign
   if ( NULL != xptParam )
   {
      // Copy
      swp.seqBuild.xptSeqParams = ( * xptParam );
   }
   return true;
}

unsigned char RFCal::SetOpCode::SeqBuild::AptLin (HANDLE hPhone, 
            cal_sweep_record & swp, 
            unsigned short segmentDuration,
            unsigned long handle_id,
            unsigned char version,
            ftm_cal_seq_bld_apt_seq_apt_char_data_type * apt_param)
{
   /* Set Seq Build Common Parameter */
   FillInCommonParams (hPhone, swp, segmentDuration, SEQ_APT_CAL, handle_id);

   swp.seqBuild.aptSeq.aptSeqType = APT_SEQ_APT_LIN;
   swp.seqBuild.aptSeq.version = version;
   // Tx data might be big. So directly assign
   if ( NULL != apt_param )
   {
      // Copy
      swp.seqBuild.aptSeq.aptCharData = ( * apt_param );
   }
   return true;
}
unsigned char RFCal::SetOpCode::SeqBuild::FBRxGain(HANDLE hPhone, 
            cal_sweep_record & swp, 
            unsigned short segmentDuration,
            unsigned long handle_id,
            const ftm_seq_cal_bld_fbrxgain_parameters_v0 & fbrxBuildParametersV0 )
{
   /* Set Seq Build Common Parameter */
   FillInCommonParams (hPhone, swp, segmentDuration, SEQ_FBRXGAIN_CAL, handle_id);

   /* Copy */
   swp.seqBuild.fbrxGainParameters.version = 0;
   swp.seqBuild.fbrxGainParameters.fbrxGainbuildParameters.version0 = fbrxBuildParametersV0;
   return true;
}
 unsigned char RFCal::SetOpCode::SeqBuild::RxGainV1 (HANDLE hPhone, 
            cal_sweep_record & swp, 
            unsigned short segmentDuration,
            unsigned long handle_id,
            const ftm_cal_seq_bld_multi_rx_cfg_meas_parameter_type & rxParams)
 {
   /* Set Seq Build Common Parameter */
   FillInCommonParams (hPhone, swp, segmentDuration, SEQ_MULTI_RX_CFG_MEAS, handle_id);

   /* Copy */
   swp.seqBuild.multiRxCfgMeas = rxParams;
   
   swp.seqBuild.multiRxCfgMeas.version = 1;
   // Payload size is filled by QMSL.
   return true;
 }


unsigned char RFCal::SetOpCode::SeqExec::RadioSetup(HANDLE hPhone, cal_sweep_record & swp,  
   unsigned short segmentDuration, 
   unsigned long handle_id, 
   unsigned char tech)
{
   FillInCommonParams (hPhone, swp, segmentDuration, SEQ_RADIO_SETUP, handle_id );
   return true;
}

unsigned char RFCal::SetOpCode::SeqExec::FBRxGain(HANDLE hPhone, cal_sweep_record & swp,  
   unsigned short segmentDuration, 
   unsigned long handle_id, 
   unsigned char overrideIndex)
{
   FillInCommonParams (hPhone, swp, segmentDuration, SEQ_FBRXGAIN_CAL, handle_id );
   swp.seqExec.fbrxGainParameters.version = 0;
   swp.seqExec.fbrxGainParameters.fbrxGainexecParameters.version0.overrideIndex = overrideIndex;

   return true;
}
unsigned char RFCal::SetOpCode::SeqExec::RxGainV1(HANDLE hPhone, cal_sweep_record & swp,  
   unsigned short segmentDuration, 
   unsigned long handle_id)
{
   FillInCommonParams (hPhone, swp, segmentDuration, SEQ_MULTI_RX_CFG_MEAS, handle_id );
   return true;
}
unsigned char RFCal::SetOpCode::SeqExec::FullBias(HANDLE hPhone, cal_sweep_record & swp,  
   unsigned short segmentDuration, 
   unsigned long handle_id,
   unsigned char version)
{
   FillInCommonParams (hPhone, swp, segmentDuration, SEQ_APT_CAL, handle_id );
   swp.seqExec.aptSeqType = APT_SEQ_FULL_BIAS_LIN;
   swp.seqExec.aptSeqVersion = version;
   return true;
}

unsigned char RFCal::SetOpCode::SeqExec::AptLin(HANDLE hPhone, cal_sweep_record & swp,  
   unsigned short segmentDuration, 
   unsigned long handle_id,
   unsigned char version)
{
   FillInCommonParams (hPhone, swp, segmentDuration, SEQ_APT_CAL, handle_id );
   swp.seqExec.aptSeqType = APT_SEQ_APT_LIN;
   swp.seqExec.aptSeqVersion = version;
   return true;
}
     
unsigned char RFCal::SetOpCode::SeqExec::Xpt(HANDLE hPhone, cal_sweep_record & swp,  
   unsigned short segmentDuration, 
   unsigned long handle_id)
{
   FillInCommonParams (hPhone, swp, segmentDuration, SEQ_XPT_CAL, handle_id );
   return true;
}
     

unsigned char RFCal::SetOpCode::SeqExec::FillInCommonParams (HANDLE hPhone,  cal_sweep_record & swp, 
const unsigned short segDuration,
const unsigned short seqType,
const unsigned long handle_id,
const unsigned char version,
const unsigned char debug_mode,
const unsigned char supportParallel)
{
   swp.resExpected = true;
   swp.opcode.segLen = segDuration;

   swp.seqExec.handle = handle_id;
   swp.seqExec.seqType = seqType;

   swp.seqExec.debugMode = debug_mode;
   swp.seqExec.version = version;
   swp.seqExec.supportParallel = supportParallel;
   return true;
}


unsigned char RFCal::SetOpCode::InitRecord(HANDLE, cal_sweep_record & swpRecord)
{
   memset( &swpRecord, 0, sizeof(cal_sweep_record) );

   /* ESC Parameters */
   swpRecord.opcode.segLen = 60000;
   swpRecord.txPurpose = -1;
   swpRecord.isNopSeg = false;
   swpRecord.resExpected = false;

   // Tx Variable Length
   swpRecord.txParams.txDevice   = 1;
   for( int dev = 0; dev < MAX_NUM_DEVICES; dev++ )
   {
      swpRecord.txParams.txParams[dev].paState = 0xFF;   
   }

   // rxParams
   swpRecord.rxParams.rxActions = (word) 0xFFFF;
   // Data Set Data Operations to false this is for ET/EPT

   //rxMeas Rx Variable Length
   for( UINT dev = 0; dev < MAX_NUM_DEVICES; dev++ )
   {
      swpRecord.rxMeas.actions[dev].rx_meas_action = 0xFF;
   }

   // TxMeas. Device Mask
   swpRecord.txMeasParams.txMeasFlag = 1;
   swpRecord.sync.syncEvent = 0xFFFF; 

   // Sequence Build (new options)
   swpRecord.seqBuild.seqType = SEQ_TYPE_UNDEFINED;
   swpRecord.seqExec.seqType = SEQ_TYPE_UNDEFINED;

   return true;
};

unsigned char RFCal::SetOpCode::InitSwpRecords(HANDLE hPhone, vector < cal_sweep_record > & Sweep, unsigned int numRecords)
{
   Sweep.resize(numRecords);
   for ( vector < cal_sweep_record >::iterator it = Sweep.begin(); it != Sweep.end(); it++ )
   {
      InitRecord ( hPhone, *it );
   }
   return true;
} 
unsigned char RFCal::QSC::GetSortedRFMPathVector(HANDLE hPhone, 
                                                   const QSCSwpParams & swpParams,
                                                   const CBandInfoPtr & perPathbandInfoPtr,
                                                   const CRFPathInformationPacket & rfmPath, 
                                                   std::vector < CRFPathInformationPacket > & finalRfmPaths,
                                                   unsigned char iResultOrderMask)
{
   std::vector < CRFPathInformationPacket > rfmPaths;
   const int tech = swpParams.tech;
   CRFPathInformationPacket associated_rfmPath1;
   CRFPathInformationPacket associated_rfmPath2;
   bool bFound_assoc_path1 = false;
   bool bFound_assoc_path2 = false;
   if ( swpParams.repoPtr->rfmPathDB.QueryAssociatedRFMPathInfo ( hPhone, tech, perPathbandInfoPtr.band, perPathbandInfoPtr.subBand, 
      rfmPath, associated_rfmPath1 ))
   {
      bFound_assoc_path1 = true;
   }
   if ( bFound_assoc_path1 && swpParams.repoPtr->rfmPathDB.QueryAssociatedRFMPathInfo ( hPhone, tech, perPathbandInfoPtr.band, perPathbandInfoPtr.subBand, 
      associated_rfmPath1, associated_rfmPath2 ))
   {
      bFound_assoc_path2 = true;
   }

   if ( PATH_TYPE_TX == rfmPath.rfmPathType )
   {
      // Tx is last
      if ( bFound_assoc_path2 )
      {
         if ( PATH_TYPE_PRX == associated_rfmPath2.rfmPathType )
         {
            rfmPaths.push_back ( associated_rfmPath2 );
            rfmPaths.push_back ( associated_rfmPath1 );
         }
         else
         {
            rfmPaths.push_back ( associated_rfmPath1 );
            rfmPaths.push_back ( associated_rfmPath2 );
         }

      } 
      else if ( bFound_assoc_path1 )
      {
        rfmPaths.push_back ( associated_rfmPath1 );
      }
      if ( (1 << PATH_TYPE_TX) && iResultOrderMask )
      {
         rfmPaths.push_back ( rfmPath );
      }
   }
   else if ( PATH_TYPE_PRX == rfmPath.rfmPathType) 
   {
      rfmPaths.push_back ( rfmPath );
      if ( bFound_assoc_path1 )
      {
         rfmPaths.push_back ( associated_rfmPath1 );
      }
   }
   else if ( PATH_TYPE_DRX == rfmPath.rfmPathType )
   {
      if ( bFound_assoc_path1 )
      {
         if ( PATH_TYPE_PRX == associated_rfmPath1.rfmPathType )
         {
            rfmPaths.push_back ( associated_rfmPath1 );
            rfmPaths.push_back ( rfmPath );
         }
         else

         {
            associated_rfmPath1.rfmPathType = PATH_TYPE_PRX;
            rfmPaths.push_back ( associated_rfmPath1 );
            rfmPaths.push_back ( rfmPath );
           
         }
      }
      else
      {
         rfmPaths.push_back ( rfmPath );
      }
   }

   finalRfmPaths.clear();
   finalRfmPaths.reserve ( rfmPaths.size () );

   for (  std::vector < CRFPathInformationPacket >::const_iterator itr = rfmPaths.begin();
      itr != rfmPaths.end(); itr++)
   {
      if ( ( 1 << itr->rfmPathType ) & iResultOrderMask)
      {
         finalRfmPaths.push_back ( *itr );
      }
   }


   return !(finalRfmPaths.empty());
}

unsigned char RFCal::QSC::CreateSwpRecords::Add::GetRxParamsPerPathPerDL ( HANDLE hPhone, const QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo,  const vector < CRFPathInformationPacket > & rfm_path_list,
   std::vector < map < const CRFM_Path, CRx_Gain > > & rx_params_per_path_per_DL )
{
   map < const CRFM_Path, const CRx_Gain *  > RxParamsPerPath;
   if ( ! RFCal::QSC::CreateSwpRecords::Add::GetRxPathParams ( hPhone, perPathBandInfo, rfm_path_list, RxParamsPerPath ))
   {
      return false;
   }

   if ( RxParamsPerPath.empty() 
      || NULL ==  RxParamsPerPath.begin()->second 
      || RxParamsPerPath.begin()->second->DL_Powers.empty()
      )
   {
      return false;
   }
  
   rx_params_per_path_per_DL.clear ();
   uint32 numDL_Powers = RxParamsPerPath.begin()->second->DL_Powers.size();
   rx_params_per_path_per_DL.reserve ( numDL_Powers );



   map < const CRFM_Path, CRx_Gain > tmp_param_map;
   
   for ( uint32 idx = 0;  idx < numDL_Powers; idx++)
   {
      const CRx_Gain & org =  * ( RxParamsPerPath.begin()->second );
      for (map < const CRFM_Path, const CRx_Gain *  >::iterator itr = RxParamsPerPath.begin(); itr != RxParamsPerPath.end(); itr++)
      {
        
         CRx_Gain & tmp_param = tmp_param_map [ itr->first ];
         const CRx_Gain & org = *itr->second;
         
         tmp_param.Accumulation_Time = org.Accumulation_Time;
         tmp_param.AGC_Settling_Time = org.AGC_Settling_Time;
         tmp_param.Measurement_Purpose = org.Measurement_Purpose;
         tmp_param.Num_Gain_States_Per_Segment = org.Num_Gain_States_Per_Segment;
         tmp_param.Preferred_Device = org.Preferred_Device;
         tmp_param.Skip_Path = org.Skip_Path;
         
      tmp_param.DL_Powers.push_back ( org.DL_Powers [ idx ] );
      tmp_param.Gain_Limit_Max.push_back ( org.Gain_Limit_Max [ idx ] );
      tmp_param.Gain_Limit_Min.push_back ( org.Gain_Limit_Min [ idx ] );
      tmp_param.Gain_States.push_back ( org.Gain_States [ idx ] );
      tmp_param.Power_Mode.push_back ( org.Power_Mode [ idx ] );
   }
      if ( idx == (numDL_Powers - 1) ||  (org.DL_Powers[ idx + 1 ] != org.DL_Powers[ idx ]))
      {
         rx_params_per_path_per_DL.push_back ( tmp_param_map );
         tmp_param_map.clear();
      }
   }
   return true;
}
            
unsigned char RFCal::QSC::CreateSwpRecords::Add::GetRxPathParams ( HANDLE hPhone, const CBandInfoPtr & perPathBandInfo,  const vector < CRFPathInformationPacket > & rfm_path_list, std::map < const CRFM_Path, const CRx_Gain *  > & pRxPerPathParams )
{
   pRxPerPathParams.clear();
   for ( vector < CRFPathInformationPacket >::const_iterator itr = rfm_path_list.begin(); itr != rfm_path_list.end(); itr++)
   {
      if ( PATH_TYPE_TX == itr->rfmPathType )
      {
         continue;
      }
      // Rx only
      //bandInfo.

      // ************************************************ //
      // Get PerPath parameter (if any )
      // ************************************************ //
      const CRx_Gain * rxGainParamsPtr = NULL;
      CRFM_Path rfmPath(*itr);
      if ( perPathBandInfo.perPathParams->rxGainParams.find ( rfmPath ) == perPathBandInfo.perPathParams->rxGainParams.end())
      {
         // Regular parameter
         rxGainParamsPtr = perPathBandInfo.rxGainParams;
      }
      else
      {
         // <Path_Info><Path> parameter
         rxGainParamsPtr = &perPathBandInfo.perPathParams->rxGainParams.find ( rfmPath )->second;
      }
      if ( NULL == rxGainParamsPtr)
      {
         return false;
      }
      pRxPerPathParams[rfmPath] = rxGainParamsPtr;
   }
   return (!pRxPerPathParams.empty());
}
 
            
unsigned char RFCal::QSC::CreateSwpRecords::Add::RxGain ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, 
   const CBandInfoPtr & perPathBandInfo, const CRFPathInformationPacket & key_rfmPath, const int channel, const vector < CRFPathInformationPacket > & rfm_path_list )
{
   // ************************************************ //
   // RxGain: may not start with a new segment
   // Execute multiple LNA gain states if DL level is the same
   // ************************************************ //
   if (NULL == perPathBandInfo.commonParams)
   {
      return false;
   }
   const CCommonParam & commonParams = *perPathBandInfo.commonParams;
   

   std::vector < map < const CRFM_Path, CRx_Gain > > rx_params_list_per_path_per_DL;
   if ( ! GetRxParamsPerPathPerDL ( hPhone, swpParams, perPathBandInfo, rfm_path_list , rx_params_list_per_path_per_DL ) )
   {
      return false;
   }

   cal_sweep_record * p_swpRecord = NULL;
   TE_Swp_Params * p_teRecord = NULL;
   unsigned short segDuration = static_cast < unsigned short > ( commonParams.Meas_Seg_Duration_us );

   for (  std::vector < map < const CRFM_Path, CRx_Gain > >::iterator it = rx_params_list_per_path_per_DL.begin(); it != rx_params_list_per_path_per_DL.end(); it++)

   {
      // ************************************************ //
      // SeqBld: Rx Gain Cal
      // ************************************************ //

      ftm_cal_seq_bld_multi_rx_cfg_meas_parameter_type param;
      FillinRxGainSeqBldParams ( hPhone, swpParams, perPathBandInfo, ( *it ), key_rfmPath, param );

      if ( 0 == param.target_path_list.number_of_paths )
      {
         // if number_of_paths = 0, then, no need to calibrate, return.
         return true;
      }
      if ( it == rx_params_list_per_path_per_DL.begin())
      {
         NewRecord ( hPhone, swpParams.swps.back(), true );
      }
      p_swpRecord = &(swpParams.swps.back().swpRecords.back());
      p_teRecord = &(swpParams.swps.back().teRecords.back());

      RFCal::SetOpCode::SeqBuild::RxGainV1 ( hPhone, ( *p_swpRecord ), segDuration, handle_id, param );

      // ************************************************ //
      // SeqExec: Rx Gain Cal
      // ************************************************ //

      NewRecord ( hPhone, swpParams.swps.back(), false );
      if ( ! swpParams.swps.back().swpRecords.empty () )
      {
         p_swpRecord = &(swpParams.swps.back().swpRecords.back());
         p_teRecord = &(swpParams.swps.back().teRecords.back());
         RFCal::SetOpCode::SeqExec::RxGainV1 ( hPhone, ( *p_swpRecord ), segDuration, handle_id );
      }
      p_teRecord->m_teAction = true;
      if ( ! it->empty() )
      {
         p_teRecord->m_dl_lvl = it->begin()->second.DL_Powers[0];
      }

      handle_id++;

   }
   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::Add::FillinRxGainSeqBldParams ( 
   HANDLE hPhone, 
   const QSCSwpParams & swpParams, 
   const CBandInfoPtr & perPathBandInfo, 
   map < const CRFM_Path,  CRx_Gain > & RxParamsPerPathPerDL ,
   const CRFPathInformationPacket & keyrfmPath,
   ftm_cal_seq_bld_multi_rx_cfg_meas_parameter_type & params )
{
   const CRx_Gain & rx_per_path_params = *perPathBandInfo.rxGainParams;
   const CCalType & calType = *perPathBandInfo.calType;

   memset ( & params, 0, sizeof ( ftm_cal_seq_bld_multi_rx_cfg_meas_parameter_type ));

   params.accumulation_time = static_cast < uint16 > ( rx_per_path_params.Accumulation_Time );
   params.agc_settling_time = static_cast < uint16 > ( rx_per_path_params.AGC_Settling_Time );
   params.band = static_cast < uint16 > ( perPathBandInfo.band );
   params.subBand = static_cast < uint8 > ( perPathBandInfo.subBand );

   if ( RxParamsPerPathPerDL.empty () )
   {
      return false;
   }
   // Get Primary Rx path's parameter
   CRFM_Path primary_rx_rfm_path = RxParamsPerPathPerDL.begin()->first;
   const CRx_Gain & rx_per_DL_params = RxParamsPerPathPerDL[primary_rx_rfm_path];
   
   if ( rx_per_DL_params.DL_Powers.empty() )
   {
      return false;
   }
   params.dl_power = static_cast < int > ( rx_per_DL_params.DL_Powers[0] );
   params.meas_purpose = static_cast < uint8 > ( rx_per_path_params.Measurement_Purpose );
   params.nv_action_flag = 1;
   params.limit_option = 1;

   uint8 measurementType = ( static_cast < uint8 > ( perPathBandInfo.commonParams->DL_WaveformType )) << 7; // 0: CW,  128: Modulated signal
   params.meas_type = measurementType;
   
   params.gain_state_bit_mask = 0;
   for ( uint32 idx = 0; idx < rx_per_DL_params.DL_Powers.size(); idx++)
   {
      params.gain_state_bit_mask += ( 1 << rx_per_DL_params.Gain_States [ idx ] );
      
      params.exp_rxagc [ idx ] = RFCal::Helper::Convert::DLPower_To_ExpectedAgc ( swpParams.tech,  rx_per_DL_params.DL_Powers[0]);
      params.pwr_mode [ idx ] = rx_per_DL_params.Power_Mode [ idx ];

      // This is for version 0 payload. Version 1 packet is no need to fill in, but no harm.
      params.rx_limit[ idx ].rxmax = rx_per_DL_params.Gain_Limit_Max [ idx ];
      params.rx_limit[ idx ].rxmin = rx_per_DL_params.Gain_Limit_Min [ idx ];
      
   }
   vector < CRFPathInformationPacket > rfmPaths;
   rfmPaths.clear();
   unsigned char iResultOrderMask = 0;
   iResultOrderMask += ( ( calType.Enable_PRx_Gain )?  1 << PATH_TYPE_PRX : 0 );
   iResultOrderMask += ( ( calType.Enable_DRx_Gain )?  1 << PATH_TYPE_DRX : 0 );

   RFCal::QSC::GetSortedRFMPathVector ( hPhone, swpParams, perPathBandInfo, keyrfmPath, rfmPaths, iResultOrderMask );
   uint16 numRxPath = 0;
   for ( uint32 rfmIdx = 0; rfmIdx < rfmPaths.size(); rfmIdx++)
   {
      CRFM_Path rfmPath( rfmPaths[ rfmIdx ]);
      const CRx_Gain * perPathPerDLparam = &RxParamsPerPathPerDL[rfmPath];
      if ( perPathPerDLparam->Skip_Path )
      {
         continue;
      }
      numRxPath++;

      params.target_path_list.target_paths[ rfmIdx ].rfm_device = rfmPaths[ rfmIdx ].rfmDevice;
      params.target_path_list.target_paths[ rfmIdx ].signal_path = rfmPaths[ rfmIdx ].signalPath;
      params.target_path_list.target_paths[ rfmIdx ].switch_port = rfmPaths[ rfmIdx ].antennaPath;
      params.target_path_list.target_paths[ rfmIdx ].reserved = 0;

      for ( uint32 idx = 0; idx < rx_per_DL_params.DL_Powers.size(); idx++)
      {
         params.target_path_list.target_paths[ rfmIdx ].rx_limit [ idx ].rxmax = perPathPerDLparam->Gain_Limit_Max [ idx ];
         params.target_path_list.target_paths[ rfmIdx ].rx_limit [ idx ].rxmin = perPathPerDLparam->Gain_Limit_Min [ idx ];
      }
   }
   params.target_path_list.number_of_paths = numRxPath;
   
   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::Add::FdRSB ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, const CRFPathInformationPacket & rfmPath, const int channel )
{
   if ( NULL == perPathBandInfo.rsbParams
      || NULL == perPathBandInfo.commonParams
      || NULL == perPathBandInfo.calType )
   {
      return false;
   }
   const CCalType & calType = *( perPathBandInfo.calType);
   const CCommonParam & commonParams = *( perPathBandInfo.commonParams );
   const CRx_RSB & rsbParams = *( perPathBandInfo.rsbParams );

   // ************************************************ //
   // FDRSB: always start with a new segment
   // ************************************************ //
   NewRecord ( hPhone, swpParams.swps.back(), false );
   cal_sweep_record * p_swpRecord = &(swpParams.swps.back().swpRecords.back());
   TE_Swp_Params * p_teRecord = &(swpParams.swps.back().teRecords.back());
   p_swpRecord->opcode.segLen = static_cast < word > ( commonParams.Meas_Seg_Duration_us);
   p_swpRecord->resExpected = true;
   p_swpRecord->seqBuild.debugMode = 1;
   p_swpRecord->seqBuild.handle = handle_id;
   p_swpRecord->seqBuild.fdRxRSBParams.version =0;
   p_swpRecord->seqBuild.seqType = SEQ_FD_RX_RSB;
   ftm_cal_seq_bld_fd_rx_rsb_params_v0 & param = p_swpRecord->seqBuild.fdRxRSBParams.fdRxRSB_V0;

   param.accumulationTime_us = static_cast <uint16> ( rsbParams.Accumulation_Time_us );
   param.actionMask = static_cast <uint16> ( rsbParams.Action_Mask );
   param.agcSettlingTime_us = static_cast <uint16> ( rsbParams.AGC_Settling_Time_us );
   param.lnaGainState = static_cast <uint8> ( rsbParams.LNA_Gain_State );
   param.postCalRSBMaxLim_dB10 = static_cast <int16> ( rsbParams.Post_Cal_RSB_Max_Lim_dB );
   param.preCalRSBMaxLim_dB10 = static_cast <int16> ( rsbParams.Pre_Cal_RSB_Max_Lim_dB );

   if ( calType.Enable_PRx_RSB && calType.Enable_DRx_RSB )
   {
      param.measType = 0; // PRx + DRx
   }
   else if ( calType.Enable_PRx_RSB )
   {
      param.measType = 1; // PRx
   }
   else if ( calType.Enable_DRx_RSB )
   {
      param.measType = 2; // DRx
   }

   p_teRecord->m_dl_lvl =  rsbParams.DL_Level_dBm;
   handle_id++;
   return true;
}
            
unsigned char RFCal::QSC::CreateSwpRecords::Add::XO ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, const int channel )
{
   if ( NULL == perPathBandInfo.commonParams
      || NULL == perPathBandInfo.calType 
	  || NULL == perPathBandInfo.xoParams)
   {
      return false;
   }
   const CCalType & calType = *( perPathBandInfo.calType);
   const CCommonParam & commonParams = *( perPathBandInfo.commonParams );
   const CXo &xoParams = * (perPathBandInfo.xoParams);

   // ************************************************ //
   // XO: always start with a new segment
   // ************************************************ //
   NewRecord ( hPhone, swpParams.swps.back(), false );
   cal_sweep_record * p_swpRecord = &(swpParams.swps.back().swpRecords.back());
   TE_Swp_Params * p_teRecord = &(swpParams.swps.back().teRecords.back());
   
   p_swpRecord->opcode.segLen = static_cast < word > ( commonParams.Meas_Seg_Duration_us);
   p_swpRecord->resExpected = true;
   p_swpRecord->seqBuild.debugMode = 1;
   p_swpRecord->seqBuild.handle = handle_id;

   p_swpRecord->seqBuild.seqType = SEQ_XO_CAL;
   
   ftm_seq_cal_bld_xo_cal_default_params & param = p_swpRecord->seqBuild.xoCalParams.default_params;
   
   param.lna_state = static_cast <byte> (xoParams.LNA_Gain_State);
   param.max_ft_samples = static_cast <word> (xoParams.Max_Ft_Samples);
   param.min_ft_samples = static_cast <word> (xoParams.Min_Ft_Samples);
   param.offset = static_cast <int32> (commonParams.DL_CW_Offset_KHz * 1000);
   param.substage_time = static_cast <word> (xoParams.Substage_Time);
   param.temp_span = static_cast <dword> (xoParams.Temperature_span);
   param.timeout_time = static_cast <word> (xoParams.Timeout_Time);
   param.update_nv = static_cast <byte> (xoParams.Update_NV);
   param.xo_action = static_cast <byte> (xoParams.XO_Action);
   param.xtal = static_cast <byte> (xoParams.Xtal);

   // ************************************************ //
   // Add polling to wait for XO Cal to complete
   // ************************************************ //

   NewRecord (hPhone, swpParams.swps.back(), false);
   p_swpRecord = &(swpParams.swps.back().swpRecords.back());

   RFCal::SetOpCode::Polling ( hPhone, ( * p_swpRecord ),  static_cast < uint16 > ( xoParams.Poll_Interval_us ), 
		static_cast < uint32 > ( 1000000 ), 
		POLL_EVENT_MASK_SEQ_BLD_DONE, SEQ_XO_CAL, handle_id );

   handle_id++;
   p_teRecord->m_teAction = TRUE;
   p_teRecord->m_dl_lvl =  static_cast <double> (xoParams.DL_Level_dBm);
   
   handle_id++;
   return true;
}

unsigned char RFCal::QSC::CreateSwpRecords::Add::IP2 ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, const vector<int> & IP2bwList, const int channel )
{
   if ( NULL == perPathBandInfo.ip2Params
      || NULL == perPathBandInfo.commonParams
      || NULL == perPathBandInfo.calType )
   {
      return false;
   }
   const CCalType & calType = *( perPathBandInfo.calType);
   const CCommonParam & commonParams = *( perPathBandInfo.commonParams );
   const CRx_IP2 & ip2Params = *( perPathBandInfo.ip2Params );

   // ************************************************ //
   // IP2: always start with a new segment
   // ************************************************ //
   NewRecord ( hPhone, swpParams.swps.back(), false );
   cal_sweep_record * p_swpRecord = &(swpParams.swps.back().swpRecords.back());
   p_swpRecord->opcode.segLen = static_cast < word > ( commonParams.Meas_Seg_Duration_us );
   p_swpRecord->resExpected = true;
   p_swpRecord->seqBuild.debugMode = 1;
   p_swpRecord->seqBuild.handle = handle_id;
   p_swpRecord->seqBuild.seqType = SEQ_IP2_CAL;

   ip2_cal_default_params & param = p_swpRecord->seqBuild.ip2Params.default_params;
  
   param.numBWs = static_cast<unsigned char>(IP2bwList.size());
   param.oip2_delta_min_limit_db10 = static_cast<int16>(ip2Params.oip2_delta_min_limit_db10);

   for ( size_t idx = 0; idx < IP2bwList.size(); idx++)
   {
	   param.bwList[idx] = static_cast<unsigned char>(IP2bwList[idx]);
   }
   
   if ( calType.Enable_PRx_IP2Cal && calType.Enable_DRx_IP2Cal )
   {
      param.measMask = 3; // PRx + DRx
   }
   else if ( calType.Enable_PRx_IP2Cal )
   {
      param.measMask = 1; // PRx
   }
   else if ( calType.Enable_DRx_IP2Cal )
   {
      param.measMask = 2; // DRx
   }

      
   NewRecord ( hPhone, swpParams.swps.back(), false );
   p_swpRecord = &(swpParams.swps.back().swpRecords.back());

   // ************************************************ //
   // Add polling to wait for IP2 Cal to complete
   // ************************************************ //

   RFCal::SetOpCode::Polling ( hPhone, ( * p_swpRecord ),  static_cast < uint16 > ( ip2Params.Poll_Interval ), 
		static_cast < uint32 > ( ip2Params.Poll_Timeout ), 
		POLL_EVENT_MASK_SEQ_BLD_DONE, SEQ_IP2_CAL, handle_id );

   handle_id++;
   return true;
}

unsigned char RFCal::QSC::CreateSwpRecords::Add::FBRxGain (HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, const CRFPathInformationPacket & rfmPathInfo, const int channel )
{
   if ( PATH_TYPE_TX != rfmPathInfo.rfmPathType )
   {
      return true;
   }
   if ( NULL == perPathBandInfo.fbrxGainParams
      || NULL == perPathBandInfo.commonParams
      || NULL == perPathBandInfo.calType
      || perPathBandInfo.fbrxGainParams->Gain_State_List.empty() )
   {
      return false;
   }
   const CCalType & calType = *( perPathBandInfo.calType );
   const CCommonParam & commonParams = *( perPathBandInfo.commonParams );
   const CFBRx_Gain & fbrxGainParams = *( perPathBandInfo.fbrxGainParams );


   // ************************************************ //
   // FBRxGain: always start with a new segment
   // ************************************************ //
   NewRecord ( hPhone, swpParams.swps.back(), false );
   cal_sweep_record * p_swpRecord = &(swpParams.swps.back().swpRecords.back());
   TE_Swp_Params * p_teRecord = &(swpParams.swps.back().teRecords.back());
   
   // ************************************************ //
   // SeqBld: FBRx Gain Cal
   // ************************************************ //
   
   unsigned short segDuration = static_cast < unsigned short > ( commonParams.Meas_Seg_Duration_us );
   p_teRecord->m_step_width = static_cast < double > ( segDuration );
   ftm_seq_cal_bld_fbrxgain_parameters_v0 fbrx_seqbld_param;
   unsigned char isTxOverrideOnly = ( ! calType.Enable_FBRx_Gain )? 1 : 0;
   FillinFBRxGainSeqBldParams (hPhone, isTxOverrideOnly, &fbrxGainParams, channel, fbrx_seqbld_param);  
   RFCal::SetOpCode::SeqBuild::FBRxGain (hPhone, (* p_swpRecord ), segDuration, handle_id, fbrx_seqbld_param);

   // ************************************************ //
   // SeqExec: FBRx Gain Cal
   // Execute seqExec for each of gain state
   // ************************************************ //
   
   for (uint8 iOverrideIdx = 0 ; iOverrideIdx < static_cast < uint8 > ( fbrx_seqbld_param.numberofOverrides); iOverrideIdx++)
   {
      NewRecord ( hPhone, swpParams.swps.back(), false );
      p_swpRecord = &(swpParams.swps.back().swpRecords.back());
      p_teRecord = &(swpParams.swps.back().teRecords.back());

      RFCal::SetOpCode::SeqExec::FBRxGain ( hPhone,  (* p_swpRecord ), segDuration, handle_id, iOverrideIdx);
      p_teRecord->m_teAction = true;
      p_teRecord->m_ul_lvl = static_cast < double > ( fbrxGainParams.Tx_Power_dBm_List [iOverrideIdx]);
      p_teRecord->m_lte_rx_chan_bw = commonParams.Tx_Rx_BW_Type[0][0];
      p_teRecord->m_lte_tx_chan_bw = commonParams.Tx_Rx_BW_Type[0][0];

   }
   handle_id++;
   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::Add::FillinFBRxGainSeqBldParams ( HANDLE hPhone,  unsigned char isTxOverrideOnly, const CFBRx_Gain * fbrxGainParams, 
   const int channel, ftm_seq_cal_bld_fbrxgain_parameters_v0 & param)
{
   if ( NULL == fbrxGainParams || fbrxGainParams->Gain_State_List.size() > MAX_NUM_OVERRIDES)
   {
      return false;
   }
   memset ( & param, 0 , sizeof ( ftm_seq_cal_bld_fbrxgain_parameters_v0 ));
   param.purposeType = static_cast < uint8 > ( fbrxGainParams->Number_Of_Captures);
   if ( 1 == isTxOverrideOnly )
     param.purposeType = TX_OVERRIDE_ONLY;

   /* If FBRx Opcode is for trigger only, then, numberofOverrides = 1 is enough */
   param.numberofOverrides =  static_cast < uint16 > ( ( isTxOverrideOnly )? 1 : fbrxGainParams->Gain_State_List.size() );


   for ( uint16 i = 0; i < param.numberofOverrides; i++)
   {
      ftm_cal_seq_bld_fbrxgain_fbrxParameters * fbrx = & param.fbrxParameters [ i ];
      ftm_seq_bld_fbrxgain_txoverrideParameters * tx = & param.txOverrideParameters [ i ];
      
      /* Set FBRx Gain param */
      fbrx->fbrxGainState = fbrxGainParams->Gain_State_List [ i ];
      fbrx->expectedPowerdBm10 = static_cast < int16 > ( fbrxGainParams->Tx_Power_dBm_List [ i ] * 10.0 );

      /* Tx Override param */
      tx->txPowerOverridebitMask = 0;
      tx->xptMode = 0;

      if ( fbrxGainParams->PA_State_List.size () > i )
      {
         tx->txPowerOverridebitMask += ESC_TX_OVERRIDE_PA_STATE;
         tx->paState = fbrxGainParams->PA_State_List [ i ];
      }

      if ( fbrxGainParams->RGI_List.size () > i )
      {
         tx->txPowerOverridebitMask += ESC_TX_OVERRIDE_RGI;
         tx->rgi = fbrxGainParams->RGI_List [ i ];
      }

      if ( fbrxGainParams->Bias_List.size () > i )
      {
         tx->txPowerOverridebitMask += ESC_TX_OVERRIDE_BIAS;
         tx->biasinmilliVolts = fbrxGainParams->Bias_List [ i ];
      }
      if ( fbrxGainParams->Icq_List.size () > i )
      {
         tx->txPowerOverridebitMask += ESC_TX_OVERRIDE_PA_CURRENT;
         tx->quiescentCurrent = fbrxGainParams->Icq_List [ i ];
      }
   }
   return true;
}
            

unsigned char RFCal::QSC::CreateSwpRecords::Add::RadioSetup(HANDLE hPhone, unsigned long & handle_id, ftm_radio_setup_type radio_setup_type, 
   QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, const CRFPathInformationPacket & rfmPath, const int channel, const vector < int > & bwList )
{
   if ( NULL == perPathBandInfo.commonParams || bwList.empty())
   {
      return false;
   }
   const CCommonParam & commonParams = *( perPathBandInfo.commonParams );
 

   const int tech = swpParams.tech;

   // ************************************************ //
   // Radio setup: always start with a new segment
   // ************************************************ //
   NewRecord ( hPhone, swpParams.swps.back(), false );
   cal_sweep_record * p_swpRecord = &(swpParams.swps.back().swpRecords.back());
   TE_Swp_Params * p_teRecord = &(swpParams.swps.back().teRecords.back());
   
   // ************************************************ //
   // SeqBld: Radio Setup
   // ************************************************ //
   
   unsigned short segDuration = ( RADIO_SETUP_TECH_BAND_PATH_CHAN == radio_setup_type
      || RADIO_SETUP_BAND_PATH_CHAN == radio_setup_type
      || RADIO_SETUP_BANDWIDTH == radio_setup_type
      || RADIO_SETUP_PATH_CHAN == radio_setup_type) ?
      commonParams.Config_Seg_Duration_us : commonParams.Tuning_Seg_Duration_us;

   ftm_cal_seq_bld_radio_setup_path_params common_param;
   memset(&common_param,0, sizeof(ftm_cal_seq_bld_radio_setup_path_params));
   common_param.bwType = static_cast < uint8 > ( bwList.front() );
   common_param.band = static_cast < uint16 > ( perPathBandInfo.band );
   common_param.subBand = static_cast < uint8 > ( perPathBandInfo.subBand );
   
   common_param.numChannels = 1;
   common_param.channelList[0] = channel; // Convert DL Channel in RFCal::SetOpCode::SeqBuild::RadioSetup for Rx Path
   common_param.cwOffset = 0;
   common_param.numRBs = static_cast<uint8>(commonParams.LTE_Num_RBs);
   common_param.startRBIdx = static_cast<uint8>(commonParams.LTE_Start_RB);
   common_param.txWfmType = static_cast<uint8>(commonParams.Tx_Waveform_Type);

   p_teRecord->tech = tech;
   p_teRecord->band = perPathBandInfo.band;
   p_teRecord->sub_band = perPathBandInfo.subBand;
   p_teRecord->m_numChans = 1;
   p_teRecord->m_ul_chan = channel;
   p_teRecord->m_offset_in_kHz = commonParams.DL_CW_Offset_KHz;
   p_teRecord->m_dl_lvl = -115.0;
   p_teRecord->m_ul_lvl = -115.0;
   p_teRecord->m_step_width = commonParams.Meas_Seg_Duration_us;
   p_teRecord->m_lte_tx_chan_bw = bwList.front() ;
   p_teRecord->m_lte_tx_chan_bw = bwList.front() ;

   p_teRecord->m_teAction = false;
   p_teRecord->m_rf_mode = static_cast < FTM_RF_Mode_Enum > ( RFCal::Helper::Convert::GetRFMode ( tech, perPathBandInfo.band, perPathBandInfo.subBand ) );
   // ************************************************ //
   // Get ordered RFM Path vector
   // Always, Tx is last, PRx --> DRX --> Tx order
   // ************************************************ //
   std::vector < CRFPathInformationPacket >  rfmPaths;
   if (! RFCal::QSC::GetSortedRFMPathVector(hPhone, swpParams, perPathBandInfo, rfmPath, rfmPaths ) )
   {
      // Failure...
      return false;
   }

   if ( bwList.size() > 1 ) // For 40 Mhz FDRSB
   {
      uint32 size = rfmPaths.size();
      for ( uint32 i=0; i < size; i++ )
      {
         rfmPaths.push_back (rfmPaths[i] );
      }
   }
   RFCal::SetOpCode::SeqBuild::RadioSetup (hPhone, ( *p_swpRecord ), segDuration, handle_id, tech,  false, 
      radio_setup_type, &common_param, &rfmPaths);
  
   // ************************************************ //
   // OpCode 0x400 Polling for Radio Setup
   // Connected Cal. Add only the first radio setup (RADIO_SETUP_TECH_BAND_PATH_CHAN = 0case )
   // Unconneced Cal. Add every radio setup ( RADIO_SETUP_TECH_BAND_PATH_CHAN = 0 and RADIO_SETUP_PATH_CHAN = 2 case)
   // ************************************************ //
   if( RADIO_SETUP_TECH_BAND_PATH_CHAN == radio_setup_type  
      || ( RADIO_SETUP_PATH_CHAN == radio_setup_type  && 
         swpParams.sweep_type ==  SEQ_CAL_UNCONNECTED ))
   {
      NewRecord ( hPhone, swpParams.swps.back(), false );
      p_swpRecord = &(swpParams.swps.back().swpRecords.back());
      p_teRecord = &(swpParams.swps.back().teRecords.back());
      p_teRecord->m_step_width = commonParams.Poll_Interval ;
      RFCal::SetOpCode::Polling ( hPhone, ( * p_swpRecord ),  static_cast < uint16 > ( commonParams.Poll_Interval ), 
         static_cast < uint32 > ( commonParams.Poll_Timeout ), 
         POLL_EVENT_MASK_SEQ_BLD_DONE, SEQ_RADIO_SETUP, handle_id );
   }

   // ************************************************ //
   // Skip Radio Setup Exec if it is complete radio setup
   // ************************************************ //
   if (( RADIO_SETUP_TECH_BAND_PATH_CHAN == radio_setup_type)
      || ( RADIO_SETUP_BAND_PATH_CHAN == radio_setup_type)
      || ( RADIO_SETUP_PATH_CHAN == radio_setup_type)
      || ( RADIO_SETUP_BANDWIDTH == radio_setup_type))
   {
      // Optimization. Skip SeqExec
      handle_id++;
      return true;
   }


   // ************************************************ //
   // SeqExec: Radio Setup
   // ************************************************ //
   
   NewRecord ( hPhone, swpParams.swps.back(), false );
   p_swpRecord = &(swpParams.swps.back().swpRecords.back());
   p_teRecord = &(swpParams.swps.back().teRecords.back());
   p_teRecord->m_ul_chan = channel; // This is for equipment. Set the channel in Radio Setup Exec

   segDuration = commonParams.Meas_Seg_Duration_us;
   RFCal::SetOpCode::SeqExec::RadioSetup (hPhone, ( *p_swpRecord), segDuration, handle_id, tech );
   
   handle_id++;
   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::Add::Teardown ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo)
 {
   if ( NULL == perPathBandInfo.commonParams)
   {
      return false;
   }
   const CCommonParam & commonParams = *( perPathBandInfo.commonParams );
   // ************************************************ //
   // SeqBld: Radio Setup - Tear down
   // ************************************************ //

   NewRecord ( hPhone, swpParams.swps.back(), false );
   cal_sweep_record * p_swpRecord = &(swpParams.swps.back().swpRecords.back());
   TE_Swp_Params * p_teRecord = &(swpParams.swps.back().teRecords.back());
   p_teRecord->tech = swpParams.tech;
   p_teRecord->band = perPathBandInfo.band;
   p_teRecord->sub_band = perPathBandInfo.subBand;
   p_teRecord->m_numChans = 1;
   p_teRecord->m_offset_in_kHz = commonParams.DL_CW_Offset_KHz;
   p_teRecord->m_dl_lvl = -115.0;
   p_teRecord->m_ul_lvl = -115.0;
   p_teRecord->m_min_ul_lvl = -115.0;
   p_teRecord->m_step_width = commonParams.Meas_Seg_Duration_us;
   p_teRecord->m_teAction = false;


   unsigned short segDuration = commonParams.Config_Seg_Duration_us;

   RFCal::SetOpCode::SeqBuild::RadioSetup(hPhone, (* p_swpRecord), segDuration, handle_id, swpParams.tech, true /* isTearDown */);
   return true;
 }
 
unsigned char RFCal::QSC::CreateSwpRecords::Add::Data_PushRxChar ( HANDLE hPhone, const QSCNode & qsc_node, QSCSwpParams & swpParams)
{

   byte numBand = 1;
   
   for ( std::vector < CBandInfo >::iterator bandIt = swpParams.BandInfo.begin(); bandIt != swpParams.BandInfo.end(); bandIt++)
   {
     if (bandIt->calType.Enable_PRx_RSB || bandIt->calType.Enable_DRx_RSB )
     {
        return false;
     }
   }
   CPath_ChanResults path_chan_results ;
   QSCNode qscNode;
   qscNode.tech = swpParams.tech;
   qscNode.band = swpParams.BandInfo.front().band;
   qscNode.subBand  =  swpParams.BandInfo.front().subBand;
   if ( !swpParams.repoPtr->GetRxCharBandData ( hPhone, qscNode, path_chan_results ))
   {
      return false;
   }
   ftm_rx_char_data_per_band_type * rx_char = new ftm_rx_char_data_per_band_type[numBand];
   if ( NULL == rx_char )
   {
      return false;
   }
   memset ( rx_char, 0, sizeof ( ftm_rx_char_data_per_band_type )*numBand );

    // ************************************************ //
   // Radio setup: always start with a new segment
   // ************************************************ //
   NewRecord ( hPhone, swpParams.swps.back(), false );
   cal_sweep_record * p_swpRecord = &(swpParams.swps.back().swpRecords.back());

   if ( NULL != p_swpRecord->data.pushRxChar.band_data)
   {
      delete p_swpRecord->data.pushRxChar.band_data;
   }
   p_swpRecord->data.pushRxChar.band_data = rx_char;

   rx_char->tech = static_cast <uint8> (qscNode.tech );
   rx_char->band = static_cast <uint16> (qscNode.band );
   rx_char->subBand = static_cast <uint8> (qscNode.subBand );

   rx_char->algo_type = QSC_PUSH_RX_CHAR_ALGORITHM_OPTION_LINEAR_INTERPOLATION;
   rx_char->debug_info = 0;//1;//QSC_PUSH_RX_CHAR_DEBUG_INFO_GET_STATUS;
   rx_char->channelOption = QSC_PUSH_RX_CHAR_CHANNEL_OPTION_USE_PER_BAND;
   rx_char->limitOption = QSC_PUSH_RX_CHAR_LIMIT_OPTION_USE_PER_BAND;
   //rx_char->common_limit;
   for ( uint32 i = 0; i < swpParams.BandInfo.front().rxGainParams.Gain_Limit_Max.size(); i++)
   {
      rx_char->common_limit[i].max = static_cast <int16> ( swpParams.BandInfo.front().rxGainParams.Gain_Limit_Max[i]);
      rx_char->common_limit[i].min = static_cast <int16> ( swpParams.BandInfo.front().rxGainParams.Gain_Limit_Min[i]);
   }

   if ( ! path_chan_results.map.empty() )
   {
      rx_char->numChan = static_cast < uint8 > ( path_chan_results.map.begin()->second.size() );
  
      for ( CChan_RxResults::iterator chan_itr =  path_chan_results.map.begin()->second.begin();  chan_itr != path_chan_results.map.begin()->second.end(); chan_itr++ )
      {
         int chIdx = std::distance ( path_chan_results.map.begin()->second.begin(), chan_itr );
         if ( chIdx < MAX_NUM_FCOMP_CHANS_QSC )
         {
            rx_char->DLChan [ chIdx ] = chan_itr->first;
         }
      }
   }
   rx_char->numRfmPaths = static_cast < uint8 > ( path_chan_results.map.size() );

   memset ( rx_char->rfmPaths, 0, sizeof ( ftm_rx_char_data_per_rfmPath_type ) * rx_char->numRfmPaths );

   for ( std::map < CRFM_Path, CChan_RxResults >::iterator path_itr = path_chan_results.map.begin(); path_itr != path_chan_results.map.end(); path_itr++ )
   {
      int path_idx = std::distance ( path_chan_results.map.begin(), path_itr );
      ftm_rx_char_data_per_rfmPath_type * path = & ( rx_char->rfmPaths [ path_idx ] );
      path->rfm_path.sigPath = path_itr->first.signal_path;
      path->rfm_path.rfmDevice = path_itr->first.rfm_dev;
      path->rfm_path.antennaSwitchPath = path_itr->first.antenna_port;
      path->rfm_path.reserved = path_itr->first.reserved;
      path->reserved = 0; // Hard coded
      
      // Limit per path here 

      //

      for ( CChan_RxResults::iterator chan_itr =  path_chan_results.map.begin()->second.begin();  chan_itr != path_chan_results.map.begin()->second.end(); chan_itr++ )
      {
         int chIdx = std::distance ( path_chan_results.map.begin()->second.begin(), chan_itr );
         if ( chIdx < 16 )
         {
            for ( CGainState_AgcOffset::iterator gain_itr = chan_itr->second.begin(); gain_itr != chan_itr->second.end(); gain_itr++ )
            {
               path->lnaOffset [ gain_itr->first ] [ chIdx ] = gain_itr->second;
               if ( 0 == chIdx )
               {
                  rx_char->LNAStateMask |= ( static_cast < uint16 > (1 << (gain_itr->first) ) );
               }
            }
         }
      }
   }
   p_swpRecord->data.doDataOp = true;
   p_swpRecord->data.pushRxChar.doPushRxCharOp = true;
   p_swpRecord->resExpected = true;
   p_swpRecord->data.pushRxChar.num_bands = numBand;
   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::Add::Data_PushFBRx ( HANDLE hPhone, QSCSwpParams & swpParams, const CBandInfo & bandInfo, const std::map < CRFM_Path, ftm_fbrxgain_path_result > & fbrx_rfmPath_ResultMap)
{
   // ******************************************** //
   // Get Sweep Record pointer
   // ******************************************** //
   NewRecord ( hPhone, swpParams.swps.back(), false );
   cal_sweep_record * p_swpRecord = &(swpParams.swps.back().swpRecords.back());
   TE_Swp_Params * p_teRecord = &(swpParams.swps.back().teRecords.back());

   // ******************************************** //
   // Segment Duration Calc
   // ******************************************** //
   unsigned short dataExchangeSegmentDuration = ( bandInfo.commonParams.Data_Seg_Duration_us > 60000)? 60000: static_cast<unsigned short>( bandInfo.commonParams.Data_Seg_Duration_us );
   /* Change the data opcode segment to be a multiple of measurement segment duration */

   dataExchangeSegmentDuration = bandInfo.commonParams.Meas_Seg_Duration_us * ( (dataExchangeSegmentDuration / bandInfo.commonParams.Meas_Seg_Duration_us ) + ((dataExchangeSegmentDuration % bandInfo.commonParams.Meas_Seg_Duration_us > 0)? 1 : 0));
   uint32 num_teseg = ( dataExchangeSegmentDuration ) / bandInfo.commonParams.Meas_Seg_Duration_us;
   for ( uint32 i = 1; i < num_teseg; i++)
   {
      swpParams.swps.back().teRecords.push_back ( *p_teRecord );
   }

   cal_sweep_fbrx_gain_paths_results * ptr_fbrx_cal_data = new cal_sweep_fbrx_gain_paths_results;
   
   if ( NULL == ptr_fbrx_cal_data )
   {
      return false; // Critial error
   }
   RFCal::QSC::CreateSwpRecords::Add::FillinDataPushFBRx ( hPhone, swpParams, bandInfo, fbrx_rfmPath_ResultMap, ptr_fbrx_cal_data );
   
   if ( fbrx_rfmPath_ResultMap.empty () )
   {
      delete ptr_fbrx_cal_data;
      ptr_fbrx_cal_data = NULL;
      return false;
   }
   RFCal::SetOpCode::Data::PushFBRxResultType2 ( hPhone, ( * p_swpRecord ), dataExchangeSegmentDuration, swpParams.tech, bandInfo.band, bandInfo.subBand, ptr_fbrx_cal_data );

   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::Add::FillinTx_XptLin ( HANDLE hPhone, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, 
   const int xpt_mode, const int BW, const PA_PARAM_KEY & pa_device_info, ftm_cal_seq_bld_xpt_params & xptSeqParams )
{
   if ( NULL == perPathBandInfo.xptParams 
      || NULL == perPathBandInfo.commonParams 
      || NULL == perPathBandInfo.txLinParams )
   {
      return false;
   }
   const CXPT_Params & xptParam = (xpt_mode == XPT_TYPE_EPT)? perPathBandInfo.xptParams->EPT_Params : perPathBandInfo.xptParams->ET_Params;
   const CTx_Linearizer & txLinParams = *perPathBandInfo.txLinParams;
   const CCommonParam & commonParam = *perPathBandInfo.commonParams;
   
   // **********************************************//
   // Get PA_PARAM parameters
   // For 40 MHz, if not found, use 10 MHz instead as workaround
   // **********************************************//   
   CXptInput_V1 xpt_pa_param;
   if ( ! swpParams.repoPtr->GetPaParamData ( pa_device_info, swpParams.tech, perPathBandInfo.band, perPathBandInfo.subBand, BW, xpt_pa_param ) )
   {
      return false;
   }

   xptSeqParams.tx_override_seg_len = static_cast<uint16>(perPathBandInfo.commonParams->Tx_Override_Seg_Duration_us);
   xptSeqParams.enableMemoryKernelCal = static_cast<byte>(xptParam.Enable_Memory_Kernel);

   xptSeqParams.xpt_mode = static_cast < uint8 > ( xpt_mode );
   xptSeqParams.xpt_seq_actn = static_cast < uint8 > ( xptParam.XPT_Seq_Action );
   xptSeqParams.get_xpt_dbg_info = static_cast < uint8 > ( xptParam.Get_Addn_XPT_Dbg_Info );

   xptSeqParams.paState = static_cast<byte>( xptParam.PA_State );

   xptSeqParams.icq = static_cast< uint16 >( (xpt_mode == XPT_TYPE_EPT)? xpt_pa_param.IcqEpt : xpt_pa_param.IcqEt );
   xptSeqParams.rgi_delta_src_from_parms = (xptParam.Rgi_Delta_SSS.size() == 0)? false:true; // 0 - RGI_DELTA_SRC_WTR, 1 = from list.
   if (xptSeqParams.rgi_delta_src_from_parms)
   {
      xptSeqParams.rgi_delta_start = static_cast<byte> (xptParam.Rgi_Delta_SSS[0]);
      xptSeqParams.rgi_delta_stop = static_cast<byte> (xptParam.Rgi_Delta_SSS[1]);
      xptSeqParams.rgi_delta_step = static_cast<byte> (xptParam.Rgi_Delta_SSS[2]);
   }
   /* IF ET mode load unity Detrough */
   if (xptSeqParams.xpt_mode == XPT_TYPE_ET )
   {
      if ( xpt_pa_param.Detrough_table.size() <= QMSL_XPT_CAL_EXPANDED_DET_SIZE )
      {
         int i = 0;
         for ( vector <int>::iterator it =  xpt_pa_param.Detrough_table.begin();
               it !=  xpt_pa_param.Detrough_table.end(); it++, i++)
         {
             xptSeqParams.epdt[i] = static_cast < unsigned short > (*it);
         }
      }
   }
   xptSeqParams.num_captures = static_cast < uint32 > (xptParam.RgiList.size() );
   if ( 3 == commonParam.Expected_Power_Option )
   {
      xptSeqParams.exp_pwr_option = 3;
   }

   xptSeqParams.exp_pwrs_in_params =  (xptParam.Exp_Pwrs.size() != 0)? 1:0;
   xptSeqParams.limit_option = 0;//No limits.//TODO:Remove hardcoding.
   for (unsigned int i = 0 ; i < xptSeqParams.num_captures; i++ )
   {
      xptSeqParams.rb[i].bias = static_cast < uint16 > ( xptParam.BiasList[i] ) ;
      xptSeqParams.rb[i].rgi = static_cast < uint16 > ( xptParam.RgiList[i] );
      if (xptSeqParams.exp_pwrs_in_params)
      {
         xptSeqParams.exp_pwr_db10[i] =  static_cast<short>(xptParam.Exp_Pwrs[i]*10);
      }
   }

   /* Mline Params */
   xptSeqParams.comp_dB = static_cast< uint16 >( (xpt_mode == XPT_TYPE_EPT)? xpt_pa_param.CalCompPtEpt *100 : xpt_pa_param.CalCompPtEt *100 );;
   xptSeqParams.curve_fit_type=  static_cast<byte> ( xpt_pa_param.MLineFitType );
   xptSeqParams.mline_rsvd_flag = 0;
   xptSeqParams.vd_max = static_cast< uint16 >( (xpt_mode == XPT_TYPE_EPT)? xpt_pa_param.MLineMaxPaSupplyEpt * 1000.0 : xpt_pa_param.MLineMaxPaSupplyEt * 1000.0);
   xptSeqParams.kernel_mask = 0xFFFFFFFF;
   xptSeqParams.num_target_calc_pwrs = static_cast<byte> (xptParam.Calc_Pwr_List.size());
   xptSeqParams.num_target_meas_pwrs = static_cast<byte> (xptParam.Meas_Pwr_List.size());

   for (int i = 0 ; i < xptSeqParams.num_target_calc_pwrs; i++)
   {
      xptSeqParams.target_calc_pwrs[i] = static_cast<short> (xptParam.Calc_Pwr_List[i]*100 );
   }
   for (int i = 0 ; i < xptSeqParams.num_target_meas_pwrs; i++)
   {
      xptSeqParams.target_meas_pwrs[i] =  static_cast<short> (xptParam.Meas_Pwr_List[i]*100 );
      if (xptSeqParams.xpt_mode == XPT_TYPE_ET)
      {
            xptSeqParams.vd_min[i] = static_cast< uint16 >( xpt_pa_param.EtVmin*1000.0 );
      } // if ET
   }

   /* Fcomp params*/
   if ( perPathBandInfo.calType->Enable_XPT_FComp )
   {
      xptSeqParams.num_fcomps = static_cast<uint8> ( xptParam.Fcomp_meas_pwr_index_list.size() );
      if (xptSeqParams.num_fcomps)
      {
         xptSeqParams.num_fcomp_chans  = static_cast<uint8>( perPathBandInfo.commonParams->Channel_List.size());
         for (int i = 0 ; i < xptSeqParams.num_fcomps; i++)
            xptSeqParams.fcomp_meas_pwr_idx[i] = static_cast<int16>(xptParam.Fcomp_meas_pwr_index_list[i]);
      }
   }
   else
   {
      xptSeqParams.num_fcomps = 0;
   }
   xptSeqParams.upperBoundChannel = static_cast <uint32> ( txLinParams.Upper_Bound_Channel);
   return true;
}
            
unsigned char RFCal::QSC::CreateSwpRecords::Add::FillinTx_AptLin ( HANDLE hPhone, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, int Txpurpose, ftm_cal_seq_bld_apt_seq_apt_char_data_type & aptCharParam )
{
   if ( NULL == perPathBandInfo.txLinParams
      || NULL == perPathBandInfo.commonParams)
   {
      return false;
   }
   const CCommonParam & commonParams = *perPathBandInfo.commonParams;
   const CTx_Linearizer & txLinParams = *perPathBandInfo.txLinParams;

   std::map < int, std::set < AptRecord > > pa_aptRecords;
   if ( ! swpParams.repoPtr->GetAptCharData ( swpParams.tech, perPathBandInfo.band, perPathBandInfo.subBand, commonParams.Ref_Channel,Txpurpose, pa_aptRecords ) )
   {
      // Workaround, if TxPurpose = 5 is not included, use TxPurpose = 1
      if ( ! swpParams.repoPtr->GetAptCharData ( swpParams.tech, perPathBandInfo.band, perPathBandInfo.subBand, commonParams.Ref_Channel,1, pa_aptRecords ) )
      {
         return false;
      }
   }

   aptCharParam.numPaStates = pa_aptRecords.size();
   
   std::map < int, std::set < AptRecord > >::iterator it = pa_aptRecords.begin();
   int char_idx = 0;
   for ( int pa_idx = 0 ; it != pa_aptRecords.end(); it++, pa_idx++ )
   {
      aptCharParam.paStates [ pa_idx ] = static_cast < uint8 > ( it->first );

      aptCharParam.numEntriesPerPa [ pa_idx ] = static_cast < uint8 > ( it->second.size() );
      
      std::set < AptRecord >::iterator recIt = it->second.begin();
      for ( ; recIt != it->second.end(); recIt++ )
      {
         if ( char_idx > 255 )
         {
            break;
         }
         int pwr = static_cast < int16 > ( ( recIt->pwr > 0)? recIt->pwr*10.0 + 0.5 : recIt->pwr*10.0 - 0.5 );

         aptCharParam.aptPowers [ char_idx ] = pwr;
         aptCharParam.aptBias [ char_idx ] = static_cast < uint16 > ( recIt->vcc );
         aptCharParam.aptIcq [ char_idx ] = static_cast < uint16 > ( recIt->icq );

         if ( ( !txLinParams.APT_PA_Gain_Delta_List.empty() )
            && ( txLinParams.APT_PA_Gain_Delta_List.size() > static_cast < uint32 > ( pa_idx ) ))
         {
            aptCharParam.gainDeltas[ char_idx ] = static_cast <int16> ( txLinParams.APT_PA_Gain_Delta_List[ pa_idx ] * 10 );
         }
         else
         {
            aptCharParam.gainDeltas[ char_idx ] = 0;
         }
         char_idx++;
      }
   }
   aptCharParam.gainDeltaOption = ( txLinParams.APT_PA_Gain_Delta_List.empty() )? 0 : 1;
      
   return true;
}

unsigned char RFCal::QSC::CreateSwpRecords::Add::Tx_Xpt ( HANDLE hPhone, unsigned long & handle_id, 
   QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo,  const PA_PARAM_KEY & pa_device_info, const int BW )
{
   // ********************************************//
   // OpCode Order
   // 1. SeqBld -> 2. Polling -> 3. SeqExec -> 4. Polling
   // ********************************************//
   if ( NULL == perPathBandInfo.xptParams 
      || NULL == perPathBandInfo.commonParams)
   {
      return false;
   }
   const CTx_XPT & xptParams = *(perPathBandInfo.xptParams);
   const CCommonParam & commonParams = *(perPathBandInfo.commonParams   );
 
   // XPT Mode loop
   for ( vector <int>::const_iterator xpt_mode_it = xptParams.XPT_Mode_List.begin(); 
      xpt_mode_it != xptParams.XPT_Mode_List.end(); xpt_mode_it++ )
   {
      // ******************************************** //
      // 1. Seq Bld
      // Get Sweep Record pointer
      // ******************************************** //
      NewRecord ( hPhone, swpParams.swps.back(), false );
      cal_sweep_record * p_swpRecord = &(swpParams.swps.back().swpRecords.back());

      unsigned short segDuration = static_cast < unsigned short > (  ( xptParams.Seq_Bld_Seg_Dur == 0 )? 
            1000 : xptParams.Seq_Bld_Seg_Dur );

      RFCal::QSC::CreateSwpRecords::Add::FillinTx_XptLin ( hPhone, swpParams, perPathBandInfo, ( * xpt_mode_it ), BW, pa_device_info, p_swpRecord->seqBuild.xptSeqParams);
      RFCal::SetOpCode::SeqBuild::Xpt ( hPhone, ( * p_swpRecord ), segDuration, handle_id );

      // ******************************************** //
      // 2. OpCode 0x400 Poliing for SeqBld
      // ******************************************** //
      NewRecord ( hPhone, swpParams.swps.back(), false );
      p_swpRecord = &(swpParams.swps.back().swpRecords.back());

      unsigned short PollingSegDuration = static_cast < unsigned short > (  ( xptParams.Poll_Interval == 0 )? 
            2000 : xptParams.Poll_Interval );

      RFCal::SetOpCode::Polling ( hPhone, ( * p_swpRecord ), PollingSegDuration, 
         static_cast < uint32 > ( xptParams.Poll_Timeout), 
         POLL_EVENT_MASK_SEQ_BLD_DONE, SEQ_XPT_CAL, handle_id );

      // ******************************************** //
      // 3. SeqExec: Parameter Assignment
      // ******************************************** //
      NewRecord ( hPhone, swpParams.swps.back(), false );
      p_swpRecord = &(swpParams.swps.back().swpRecords.back());

      segDuration = static_cast < unsigned short > ( ( xptParams.Seq_Exec_Seg_Dur == 0)?
            commonParams.Meas_Seg_Duration_us :  xptParams.Seq_Exec_Seg_Dur );

      RFCal::SetOpCode::SeqExec::Xpt ( hPhone, ( * p_swpRecord ), segDuration, handle_id );

      // ******************************************** //
      // 4. OpCode 0x400 Poliing for SeqBld
      // ******************************************** //
      NewRecord ( hPhone, swpParams.swps.back(), false );
      p_swpRecord = &(swpParams.swps.back().swpRecords.back());

      RFCal::SetOpCode::Polling ( hPhone, ( * p_swpRecord ), PollingSegDuration, 
         static_cast < uint32 > ( xptParams.Poll_Timeout), 
         POLL_EVENT_MASK_SEQ_EXEC_DONE, SEQ_XPT_CAL, handle_id );
      
      // Update handle id
      handle_id++;
   }
   return true;
}

unsigned char RFCal::QSC::CreateSwpRecords::Add::Tx_Apt ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, int Txpurpose,const CBandInfoPtr & perPathBandInfo)
{
   if ( NULL == perPathBandInfo.txLinParams
      || NULL == perPathBandInfo.commonParams
      || NULL == perPathBandInfo.calType )
   {
      return false;
   }
   const CCalType & calType =  *( perPathBandInfo.calType );
   const CCommonParam & commonParams = *( perPathBandInfo.commonParams );
   const CTx_Linearizer & txLinParams = *( perPathBandInfo.txLinParams );

   // ******************************************** //
   // Get Sweep Record pointer
   // ******************************************** //
   NewRecord ( hPhone, swpParams.swps.back(), false );
   cal_sweep_record * p_swpRecord = &(swpParams.swps.back().swpRecords.back());

   // ******************************************** //
   // Segment Duration
   // ******************************************** //
   unsigned short segDuration = static_cast < unsigned short > (  ( txLinParams.Seq_Bld_Seg_Dur == 0 )? 
         4000 : txLinParams.Seq_Bld_Seg_Dur );

   // ******************************************** //
   // SeqBld: Parameter Assignment
   // ******************************************** //
   unsigned char AptLinOpCodeVersion = ( commonParams.En_Box_Meas || calType.Enable_TxCal_RGIMeasLen )? 1 : 0;
   if ( 1 == AptLinOpCodeVersion )
   {
      p_swpRecord->seqBuild.aptSeq.aptCharData.txOverrideSegLen = GetTxOverrideRGIMeasLen ( hPhone, swpParams.tech );
   }
   RFCal::QSC::CreateSwpRecords::Add::FillinTx_AptLin ( hPhone, swpParams, perPathBandInfo, Txpurpose, p_swpRecord->seqBuild.aptSeq.aptCharData);
   RFCal::SetOpCode::SeqBuild::AptLin ( hPhone, ( * p_swpRecord ), segDuration, handle_id, AptLinOpCodeVersion );

   // ******************************************** //
   // SeqExec: Parameter Assignment
   // New Segment
   // ******************************************** //
   NewRecord ( hPhone, swpParams.swps.back(), false );
   p_swpRecord = &(swpParams.swps.back().swpRecords.back());

   // ******************************************** //
   // Segment Duration
   // ******************************************** //
   segDuration = static_cast < unsigned short > ( ( txLinParams.Seq_Exec_Seg_Dur == 0)?
         commonParams.Meas_Seg_Duration_us :  txLinParams.Seq_Exec_Seg_Dur );
   if ( 1 == AptLinOpCodeVersion )
   {
      segDuration = 60000; //We can calculate from the number of RGI and per RGI meas length
   }
   RFCal::SetOpCode::SeqExec::AptLin ( hPhone, ( * p_swpRecord ), segDuration, handle_id, AptLinOpCodeVersion );
  
   handle_id++;
   return true;
}
unsigned short RFCal::QSC::CreateSwpRecords::Add::GetTxOverrideRGIMeasLen( HANDLE hPhone, int tech )
{
   unsigned short RGIMeasLen = 0;
   switch ( tech )
   {
      case TECHNOLOGY_CDMA:
         RGIMeasLen = RGI_CDMA;
         break;
      case TECHNOLOGY_WCDMA:
         RGIMeasLen = RGI_WCDMA;
         break;
      case TECHNOLOGY_LTE:
         RGIMeasLen = RGI_LTE;
         break;      
      case TECHNOLOGY_TDSCDMA:
         RGIMeasLen = RGI_TDS;
         break;
      default:
         break;
   }
   return RGIMeasLen;
}

unsigned char RFCal::QSC::CreateSwpRecords::Add::FillinTx_Fullbias ( HANDLE hPhone, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, ftm_cal_seq_bld_apt_seq_lin_parameter_type & full_bias_param )
{
   const CTx_Linearizer & txLin = *perPathBandInfo.txLinParams;
   const CCommonParam & commonParams = *perPathBandInfo.commonParams;

   full_bias_param.numPAStates = static_cast < uint8 > ( txLin.PA_State_List.size() );

   full_bias_param.upperBoundChannel = static_cast < uint32 > ( txLin.Upper_Bound_Channel );
   
   for ( uint8 idx = 0; idx < full_bias_param.numPAStates; idx++ )
   {
      ftm_cal_seq_bld_lin_per_pa_param_type & param = full_bias_param.aptLin[idx];

      if ( txLin.PA_State_List.size () > idx )
         param.paState = static_cast < uint8 > (txLin.PA_State_List [idx] );

      if ( txLin.Bias_List.size () > idx )
         param.maxBias = static_cast < uint16 > (txLin.Bias_List [idx] );

      if ( txLin.Icq_List.size () > idx )
         param.maxIcq  = static_cast < uint16 > (txLin.Icq_List [idx] );

      if ( txLin.RGI_List.size () > idx )
      {
         param.numRGIs = txLin.RGI_List[idx].size();
      
         for ( uint32 rgiIdx = 0; rgiIdx <  param.numRGIs; rgiIdx++)
         {
            param.rgi[ rgiIdx ] = static_cast < uint16 > ( txLin.RGI_List[idx][rgiIdx] );
         }

         param.expPwrOption = static_cast<byte>(commonParams.Expected_Power_Option);
        
         if ( ( txLin.Exp_Start_Power_List.size () > idx ) &&
            ( txLin.Exp_Delta_Power_List.size () > idx ))
         {
            for ( uint32 rgiIdx = 0; rgiIdx <  param.numRGIs; rgiIdx++)
            {
               param.expPwr[ rgiIdx ] = static_cast < int16 > ( txLin.Exp_Start_Power_List[idx] - txLin.Exp_Delta_Power_List[idx]*rgiIdx )*10;
            }
         }
      }

      param.limitOption = 1;

      if ( txLin.Max_Lin_Power_List.size () > idx )
         param.maxPwr  = static_cast < int16 > ( ( txLin.Max_Lin_Power_List [idx] > 0 )? 
            txLin.Max_Lin_Power_List [idx]*10.0 + 0.5 : txLin.Max_Lin_Power_List [idx] * 10.0 - 0.5);

      if ( txLin.Min_Lin_power_List.size () > idx )
         param.minPwr  = static_cast < int16 > ( ( txLin.Min_Lin_power_List [idx] > 0 )?
            txLin.Min_Lin_power_List [idx]*10.0 + 0.5 : txLin.Min_Lin_power_List [idx] * 10.0 - 0.5);

      if ( txLin.Min_Delta_Power_List.size () > idx )
         param.minDeltaPwr  = static_cast < int16 > ( (txLin.Min_Delta_Power_List [idx] > 0 )?
            txLin.Min_Delta_Power_List [idx]*10.0 + 0.5 : txLin.Min_Delta_Power_List [idx] * 10.0 - 0.5);

   }
   
   // full_bias_param.
   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::Add::Tx_Fullbias ( HANDLE hPhone, unsigned long & handle_id, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo)
{
   if ( NULL == perPathBandInfo.txLinParams
      || NULL == perPathBandInfo.commonParams
      || NULL == perPathBandInfo.calType )
   {
      return false;
   }
   const CCalType & calType = * ( perPathBandInfo.calType );
   const CCommonParam & commonParams = *( perPathBandInfo.commonParams );
   const CTx_Linearizer & txLinParams = *( perPathBandInfo.txLinParams );

   // ******************************************** //
   // Get Sweep Record pointer
   // ******************************************** //
   NewRecord ( hPhone, swpParams.swps.back(), false );
   cal_sweep_record * p_swpRecord = &(swpParams.swps.back().swpRecords.back());
   TE_Swp_Params * p_teRecord = &(swpParams.swps.back().teRecords.back());

   // ******************************************** //
   // Segment Duration
   // ******************************************** //
   unsigned short segDuration = static_cast < unsigned short > (  ( txLinParams.Seq_Bld_Seg_Dur == 0 )? 
         4000 : txLinParams.Seq_Bld_Seg_Dur );

   // ******************************************** //
   // SeqBld: Parameter Assignment
   // ******************************************** //
   unsigned char FullBiasOpCodeVersion = ( commonParams.En_Box_Meas || calType.Enable_TxCal_RGIMeasLen )? 1 : 0;
   if ( 1== FullBiasOpCodeVersion )
   {
      p_swpRecord->seqBuild.aptSeq.linParams.txOverrideSegLen = GetTxOverrideRGIMeasLen ( hPhone, swpParams.tech );
   }
   RFCal::QSC::CreateSwpRecords::Add::FillinTx_Fullbias ( hPhone, swpParams, perPathBandInfo, p_swpRecord->seqBuild.aptSeq.linParams);
   RFCal::SetOpCode::SeqBuild::FullBias ( hPhone, ( * p_swpRecord ), segDuration, handle_id, FullBiasOpCodeVersion );

   // ******************************************** //
   // SeqExec: Parameter Assignment
   // New Segment
   // ******************************************** //
   NewRecord ( hPhone, swpParams.swps.back(), false );
   p_swpRecord = &(swpParams.swps.back().swpRecords.back());
 
   // ******************************************** //
   // Segment Duration
   // ******************************************** //
   segDuration = static_cast < unsigned short > ( ( txLinParams.Seq_Exec_Seg_Dur == 0)?
   commonParams.Meas_Seg_Duration_us :  txLinParams.Seq_Exec_Seg_Dur );
   if ( 1 == FullBiasOpCodeVersion )
   {
      segDuration = 60000; //We can calculate from the number of RGI and per RGI meas length
   }
   RFCal::SetOpCode::SeqExec::FullBias ( hPhone, ( * p_swpRecord ), segDuration, handle_id, FullBiasOpCodeVersion );
  
   // Don't update handle_id;

   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::Add::FillinDataPushFBRx ( HANDLE hPhone, QSCSwpParams & swpParams, const CBandInfo & bandInfo, 
   const std::map < CRFM_Path, ftm_fbrxgain_path_result > & rfm_result_map, cal_sweep_fbrx_gain_paths_results * fbrx_cal_data)
{
   if ( NULL == fbrx_cal_data ) return false;

   fbrx_cal_data->numberofRFMPaths = static_cast<uint8>(rfm_result_map.size());

   // Copy fbrx path result for each of RFM path
   UINT path_idx = 0;
   for (map < CRFM_Path, ftm_fbrxgain_path_result >::const_iterator path_it = rfm_result_map.begin();
         path_it != rfm_result_map.end(); path_it++, path_idx++)
   {
      ftm_fbrxgain_path_result * path_result = &fbrx_cal_data->pathResults[path_idx];
      ( * path_result ) = path_it->second;
   }

   return true;
}
unsigned char RFCal::QSC::CreateSwpRecords::Add::Data_ResultNV ( HANDLE hPhone, QSCSwpParams & swpParams, const CBandInfoPtr & perPathBandInfo, unsigned char isLastRfmPath)
{
    if ( NULL == perPathBandInfo.calType
      || NULL == perPathBandInfo.commonParams)
   {
      return false;
   }
   const CCommonParam & commonParams = *perPathBandInfo.commonParams;
   const CCalType & calType = *perPathBandInfo.calType;

   // ******************************************** //
   // Get Sweep Record pointer
   // ******************************************** //
   NewRecord ( hPhone, swpParams.swps.back(), false );
   cal_sweep_record * p_swpRecord = &(swpParams.swps.back().swpRecords.back());
   TE_Swp_Params * p_teRecord = &(swpParams.swps.back().teRecords.back());

   // ******************************************** //
   // Segment Duration Calc
   // ******************************************** //
   unsigned short dataExchangeSegmentDuration = ( commonParams.Data_Seg_Duration_us > 60000)? 60000: static_cast<unsigned short>( commonParams.Data_Seg_Duration_us );
   /* Change the data opcode segment to be a multiple of measurement segment duration */

   dataExchangeSegmentDuration = commonParams.Meas_Seg_Duration_us * ((dataExchangeSegmentDuration / commonParams.Meas_Seg_Duration_us ) + ((dataExchangeSegmentDuration % commonParams.Meas_Seg_Duration_us > 0)? 1 : 0));
   // ******************************************** //
   // Result and NV mask calc
   // ******************************************** //

   int nv_write_mask = SEQ_CAL_RESULT_TYPE_NONE;
   int result_mask = SEQ_CAL_RESULT_TYPE_NONE;

   if ( SEQ_CAL_CONNECTED == swpParams.sweep_type )
   {
      /* Rx Result and NV */
      if ( calType.Enable_PRx_Gain || calType.Enable_DRx_Gain )
      {
         result_mask |= SEQ_CAL_RESULT_TYPE_RXGAIN;
         if (  calType.Enable_NV_Write )
            nv_write_mask |= SEQ_CAL_RESULT_TYPE_RXGAIN;
      }
   
      /* FBRx Result and NV */
      if ( calType.Enable_FBRx_Gain )
      {
         result_mask |= SEQ_CAL_RESULT_TYPE_FBRXGAIN;
         nv_write_mask |= SEQ_CAL_RESULT_TYPE_FBRXGAIN;  // FBRx Cal NV generation will be done by push fbrx reuslt opcode, not this data opcode
      }
 
      /* FDRSB Result and NV */
      if ( calType.Enable_PRx_RSB || calType.Enable_DRx_RSB )
      {
         result_mask |= SEQ_CAL_RESULT_TYPE_FD_RXRSB;
         if (  swpParams.calType.Enable_NV_Write )
            nv_write_mask |= SEQ_CAL_RESULT_TYPE_FD_RXRSB;
      }
   } 
   else if ( SEQ_CAL_UNCONNECTED == swpParams.sweep_type )
   {
      /* Tx Result and NV */
      if (  calType.Enable_Full_Bias_Linearizers
         || calType.Enable_APT_Linearizers
         || calType.Enable_APT_FComp)
      {
         result_mask |= SEQ_CAL_RESULT_TYPE_TX;
         if (  calType.Enable_NV_Write && isLastRfmPath )
             nv_write_mask |= SEQ_CAL_RESULT_TYPE_TX;
      }
      
      /* XPT debug info */
      /* XPT NV is a part of Tx, there is no NV mask for XPT. */
      if ( calType.Enable_Tx_XPT )
      {
         result_mask |= SEQ_CAL_RESULT_TYPE_XPT_DBG_INFO;
      }

	  if ( (calType.Enable_PRx_IP2Cal || calType.Enable_DRx_IP2Cal) && isLastRfmPath )
	  {
		  result_mask |= SEQ_CAL_RESULT_TYPE_IP2;
		  nv_write_mask |= SEQ_CAL_RESULT_TYPE_IP2;
	  }
   }
   else if ( SEQ_CAL_XO_CAL == swpParams.sweep_type)
   {
	  /* XO Result and NV */
      if ( calType.Enable_XO_Cal )
      {
         result_mask |= SEQ_CAL_RESULT_TYPE_XO_CAL;
		 nv_write_mask |= SEQ_CAL_RESULT_TYPE_NONE;
	  }
   }

   /* =========================================== */
   // ALLOCATE Pointer for Result
   // Make sure deallocation
   /* =========================================== */
   
   if ( result_mask != SEQ_CAL_RESULT_TYPE_NONE )
   {
      if ( ! p_swpRecord->dataRes.seqCalRes )
      {
         p_swpRecord->dataRes.seqCalRes = new cal_sweep_data_seq_cal_res_type;
         if ( NULL == p_swpRecord->dataRes.seqCalRes)
         {
            // Allocation failed. This is critical.
            return false;
         }
         memset(p_swpRecord->dataRes.seqCalRes, 0, sizeof(cal_sweep_data_seq_cal_res_type));

		 if ( result_mask & SEQ_CAL_RESULT_TYPE_IP2 )
		 {
			 p_swpRecord->dataRes.seqCalRes->unconn_res_data.ip2_cal_band_data = new rfcommon_ip2_cal_band_data_type;
			 if ( NULL == p_swpRecord->dataRes.seqCalRes->unconn_res_data.ip2_cal_band_data  )
			 {
				 delete p_swpRecord->dataRes.seqCalRes;
				 p_swpRecord->dataRes.seqCalRes = NULL;
				// Allocation failed. This is critical.
				return false;
			 }
		 }
      }
   }

   // ******************************************** //
   // Set OpCode
   // ******************************************** //
   return RFCal::SetOpCode::Data::ResultCommitAndNV ( hPhone, 
      ( *p_swpRecord ), dataExchangeSegmentDuration, swpParams.tech, perPathBandInfo.band, perPathBandInfo.subBand,
      result_mask, nv_write_mask );
}

unsigned char RFCal::QSC::CreateSwpRecords::Add::NewRecord ( HANDLE hPhone, qscRecords & qsc_record, bool usePrevSegment)
 {
    // ************************************ //
    // Add new record
    // if the following condition creates a new sweep/TE record 
    // (1) No record in the sweep record
    // (2) Parameter usePrevSegment = false
    // (3) Previous segment is Radio Setup Bld/Exec
    // or not, if radio setup, don't use previous segment, create a new segment
    // othewise do nothing, 
    // ************************************ //

    if (  qsc_record.swpRecords.empty() 
       || ( !usePrevSegment )
       || ( qsc_record.swpRecords.back().seqBuild.seqType == SEQ_RADIO_SETUP
          || qsc_record.swpRecords.back().seqExec.seqType == SEQ_RADIO_SETUP)
       )
    {
       cal_sweep_record tmp;
       RFCal::SetOpCode::InitRecord (hPhone, tmp);
       qsc_record.swpRecords.push_back (tmp);

       if ( qsc_record.teRecords.empty())
       {
          TE_Swp_Params TEtmp;
          qsc_record.teRecords.push_back (TEtmp);
       }
       else
       {
          qsc_record.teRecords.push_back (qsc_record.teRecords.back());
       }
    }
    else // ( usePrevSegment == true case )
    {
       // Do nothing
    }
    return true;
 }
 
unsigned char RFCal::QSC::ExecuteGetSweepResult(HANDLE hPhone, std::vector < cal_sweep_record > & SwpRecords, unsigned char bCompressedResponse)
{
   //*********************************************//
   //      Check resExpected is true
   //*********************************************//
   unsigned char bFound = false;
   for ( vector < cal_sweep_record >::iterator it = SwpRecords.begin(); it != SwpRecords.end(); it++)
   {
      if ( it->resExpected == true)
      {
         bFound = true;
         break;
      }
   }
   if ( !bFound )
   {
      // if no response expected, then, we should not execute this FTM command.
      return false;
   }
   //*********************************************//
   //      QLIB_FTM_GET_ESC_SWEEP_RESULTS
   //*********************************************//
   unsigned __int64  err            = 0;   // For MSVC6. VS2010. Unsigned long long is available
   unsigned short err_segment         = 0;
   unsigned char bOK = false;
   
   if ( bCompressedResponse )
   {
      ::Sleep ( 20 );
        bOK = QLIB_FTM_GET_ESC_SWEEP_RESULTS_COMPRESSED(hPhone, (void*)(&SwpRecords[0]),(unsigned long)SwpRecords.size(),&err,&err_segment);
   }
   else
   {
        bOK = QLIB_FTM_GET_ESC_SWEEP_RESULTS(hPhone, (void*)(&SwpRecords[0]),(unsigned long)SwpRecords.size(),&err,&err_segment);
   }
   //*********************************************//
   // Debug feature -- force crash dump
   //*********************************************//
#if 0
   if ( err & (static_cast<__int64>(1) << FTM_CAL_SEG_ERR_SEG_DURATION_OVERRUN ) )
   {
      // If ISR overrun occurs, force crash, for debug purpose
      RFCal::Helper::DIAG::ForcedCrash ( hPhone ); 
   }
#endif
   return bOK;
}
unsigned char RFCal::QSC::ExecuteQSCSweepRecords(HANDLE hPhone, std::vector < cal_sweep_record > & SwpRecords, bool isTearDown, bool isDebug, unsigned char isCompressed)
{
   if ( SwpRecords.empty() )
   {
      return false;
   }
   for (vector < cal_sweep_record >::iterator it = SwpRecords.begin(); it != SwpRecords.end(); it++)
   {
      QLIB_FTM_GENERATE_ESC_OPCODE( NULL /* faster */, (void *)&(*it));
   }
   bool bCompress = ( isCompressed == 0)? false : true;
   unsigned char status = QLIB_FTM_CFG_AND_EXEC_QSC_SWEEP(hPhone, &SwpRecords[0], SwpRecords.size(), isTearDown, isDebug, bCompress);

   return status;
}
unsigned char RFCal::QSC::GetRFFE_DEVICE_INFO_V2 ( HANDLE hPhone, RFMPathDB * rfmPathDBPtr, const int tech, const int band, const int subband, 
            const CRFPathInformationPacket & txRFM_Path, PA_PARAM_KEY & pa_device_info )
{
   unsigned char status = 0;
   FTM_GET_RFFE_DEVICE_INFO_Request_V2 rffeDeviceInfoRequest_V2;
   rffeDeviceInfoRequest_V2.rf_mode = static_cast < byte > ( RFCal::Helper::Convert::GetRFMode ( tech, band, subband ) );
   rffeDeviceInfoRequest_V2.tx_signal_path = static_cast < byte > ( txRFM_Path.signalPath );
   rffeDeviceInfoRequest_V2.tx_rfm_device = static_cast < byte > ( txRFM_Path.rfmDevice );
  
   CRFPathInformationPacket associatedRFMPath;
   
   if ( ! rfmPathDBPtr->QueryAssociatedRFMPathInfo ( hPhone, tech, band, subband, txRFM_Path, associatedRFMPath ) )
   {
      return false;
   }

   rffeDeviceInfoRequest_V2.rx_signal_path = static_cast < byte > ( associatedRFMPath.signalPath );
   rffeDeviceInfoRequest_V2.rx_rfm_device = static_cast < byte > ( associatedRFMPath.rfmDevice );
   rffeDeviceInfoRequest_V2.rffe_device_type = static_cast<byte>(1);

   FTM_GET_RFFE_DEVICE_INFO_Response rffeDeviceInfo;

   status = QLIB_FTM_GET_RFFE_DEVICE_INFO_V2( hPhone,
                                              static_cast<void*>(&rffeDeviceInfoRequest_V2),
                                              static_cast<void*>(&rffeDeviceInfo) );
   if ( status == 0 || rffeDeviceInfo.status == 1 )
   {
      // Failed
      return false;
   }
    
   pa_device_info.manuf_id = static_cast< int >(rffeDeviceInfo.mfg_id[0]);
   pa_device_info.product_id = static_cast< int >(rffeDeviceInfo.prd_id[0]);
   pa_device_info.product_rev = static_cast< int >(rffeDeviceInfo.rev_id[0]);
   return true;
}
unsigned long RFCal::QSC::GetTotalSegmentTime(HANDLE hPhone, const std::vector < cal_sweep_record > & SwpRecords)
{
   unsigned long tmp = 0;

   for (vector < cal_sweep_record >::const_iterator it = SwpRecords.begin(); it != SwpRecords.end(); it++)
   {
      if ( ( it->polling.event_mask & POLL_EVENT_MASK_SEQ_BLD_DONE )
         || ( it->polling.event_mask & POLL_EVENT_MASK_SEQ_EXEC_DONE ))
      {
         tmp += static_cast<unsigned long>(it->polling.timeout_us);
      }
      else
      {
      tmp += static_cast<unsigned long>(it->opcode.segLen);
   }
   }

   return tmp;
}
unsigned char RFCal::QSC::ExecuteSerialCal ( HANDLE hPhone, const QSCSwpParams & swpParams, qscRecords & qsc_records )
{
   const int CallBoxWaitingTime = 20; // unit ms. Required to wait after changing DL or CH for callbox

   RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();
   if ( swpParams.BandInfo.empty() )
   {
      return false;
   }
   const CBandInfo & bandInfo = swpParams.BandInfo.front();
   
   pMgr->Set_Tech ( swpParams.tech );

   //TE Configuration - Set Band
#if 0
   if( pConfig->itemExists( "SkipCPInit" ))
   {
      if(pConfig->getBool( "SkipCPInit" ) == true)
      {
         m_pLM->TE_Set_Band(CBandHelper::BandNumberToBandType( seqCalConnectedSwpParams->bandParams[0].band, technology ), RFCAL_ALG_TYPE_COMPOSITE_CAL, true);
         m_pMM->TE_Set_Band(CBandHelper::BandNumberToBandType( seqCalConnectedSwpParams->bandParams[0].band, technology ), RFCAL_ALG_TYPE_COMPOSITE_CAL, true);
      }
      else
      {
         m_pLM->TE_Set_Band(CBandHelper::BandNumberToBandType( seqCalConnectedSwpParams->bandParams[0].band, technology ), RFCAL_ALG_TYPE_SERIAL_CAL);
         m_pMM->TE_Set_Band(CBandHelper::BandNumberToBandType( seqCalConnectedSwpParams->bandParams[0].band, technology ), RFCAL_ALG_TYPE_SERIAL_CAL);
      }
   }
   else
   {
      m_pLM->TE_Set_Band(CBandHelper::BandNumberToBandType( seqCalConnectedSwpParams->bandParams[0].band, technology ), RFCAL_ALG_TYPE_SERIAL_CAL);
      m_pMM->TE_Set_Band(CBandHelper::BandNumberToBandType( seqCalConnectedSwpParams->bandParams[0].band, technology ), RFCAL_ALG_TYPE_SERIAL_CAL);
   }
#endif
   // Select Main/Aux Port
   pMgr->Set_OutputPort ( swpParams.calType.Enable_Downlink_Aux_Port );

   //TE Configuration - Set up the DL modulation waveform
   bool forceCW = ( bandInfo.commonParams.DL_WaveformType == 0)? true : false;
   if ( true == forceCW) // for now, support CW only
   {
      pMgr->Set_DL_Mod_Type ( MOD_TYPE_RFCAL_CW );
   }

   //TE Configuration -Turn ON the DL state
   pMgr->Set_DL_State ( true );

   //Iterator for TE
   vector<TE_Swp_Params>::iterator teItr = qsc_records.teRecords.begin();
   vector<TE_Swp_Params>::iterator PrevTeItr = teItr;

   teItr = qsc_records.teRecords.begin();
   if(teItr == qsc_records.teRecords.end())
   {
      return false;
   }
   int prevChan = teItr->m_ul_chan; // Channel at Segment 0
   double prevDLPower = teItr->m_dl_lvl; // DL Power at Segment 0

   pMgr->Set_Band ( teItr->band );
   //Set channel on TE
   pMgr->Set_DL_Channel( prevChan, (teItr->m_offset_in_kHz), 0.0 );

   // TE Set Level
   pMgr->Set_DL_Level( prevDLPower);
   
   Sleep(CallBoxWaitingTime);
   
   // Get phone handle
   
   unsigned char status = false;
   UINT prevSegNum = 0;
   UINT curSegNum = 0;
   bool isLastSweep = false;
   /*
   Iterator all TE records
   Execute TE command every ch or DL changes.
   */
   for (teItr = qsc_records.teRecords.begin(); teItr != qsc_records.teRecords.end(); teItr++, curSegNum++)
   {
      // Check DL Level or Channel changes in the next segment
      if (  PrevTeItr->m_ul_chan != teItr->m_ul_chan
            || PrevTeItr->m_dl_lvl != teItr->m_dl_lvl )
      {
        // m_pLM->TE_Set_Band(CBandHelper::RFModeToBandType(PrevTeItr->m_rf_mode),RFCAL_ALG_TYPE_SERIAL_CAL);
        // m_pMM->TE_Set_Band(CBandHelper::RFModeToBandType(PrevTeItr->m_rf_mode),RFCAL_ALG_TYPE_SERIAL_CAL);

         // If the next segment changes Channel, then set channel
         if (prevChan != PrevTeItr->m_ul_chan)
         {
            pMgr->Set_Band ( teItr->band );

            prevChan = PrevTeItr->m_ul_chan;
            pMgr->Set_DL_Channel ( prevChan, ( PrevTeItr->m_offset_in_kHz), 0.0 );
         }

         // If the next segment changes DL power, then set DL power
         if (prevDLPower != PrevTeItr->m_dl_lvl)
         {
            prevDLPower = PrevTeItr->m_dl_lvl;
            pMgr->Set_DL_Level ( prevDLPower );

         }
         ::Sleep(CallBoxWaitingTime);
         isLastSweep = (curSegNum >= ( qsc_records.swpRecords.size() - 1))? true : false; // Set true if it is the last sweep
         UINT lastSegment = (isLastSweep)?curSegNum: curSegNum-1;
         
         // Execute QLIB_FTM_CFG_AND_EXEC_ESC_SWEEP and QLIB_FTM_GET_ESC_SWEEP_RESULTS for this block of segments
         status = RFCal::QSC::ExecuteSwpRecords_And_FetchResults ( hPhone, 
            qsc_records.swpRecords, 
            bandInfo.commonParams.En_Compress_Esc_Req,
            bandInfo.commonParams.En_Compress_Esc_Res,
            prevSegNum, lastSegment, isLastSweep);

         prevSegNum = curSegNum;
         PrevTeItr = teItr;
      }
   }
   if (!isLastSweep)
   {
      curSegNum --;
      // Execute last sweep
      if ( /* status && */PrevTeItr != qsc_records.teRecords.end())
      {
         // If the next segment changes Channel, then set new channel
         if ( teItr ==  qsc_records.teRecords.end())
         {
            teItr--;
         }
         if (prevChan != PrevTeItr->m_ul_chan)
         {
            pMgr->Set_Band ( teItr->band );

            pMgr->Set_DL_Channel ( PrevTeItr->m_ul_chan, (PrevTeItr->m_offset_in_kHz), 0.0 );
         }

         // If the next segment changes DL power, then set new DL power
         if (prevDLPower != PrevTeItr->m_dl_lvl)
         {
            pMgr->Set_DL_Level ( PrevTeItr->m_dl_lvl );
         }
         Sleep(CallBoxWaitingTime);
      }
      if (curSegNum <= ( qsc_records.swpRecords.size() - 1))
      {
         isLastSweep = true;
         // Execute QLIB_FTM_CFG_AND_EXEC_ESC_SWEEP and QLIB_FTM_GET_ESC_SWEEP_RESULTS for this block of segments
         // curSegNum is the last segment.
         curSegNum = qsc_records.swpRecords.size() - 1;
         
         status = RFCal::QSC::ExecuteSwpRecords_And_FetchResults ( hPhone, 
            qsc_records.swpRecords, 
            bandInfo.commonParams.En_Compress_Esc_Req,
            bandInfo.commonParams.En_Compress_Esc_Res,
            prevSegNum, curSegNum, isLastSweep);

      }
   }
   // TE Configuration - Turn Off DL
   pMgr->Set_DL_State( false );

   return true;
}
      
unsigned char RFCal::QSC::ExecuteSwpRecords_And_FetchResults ( HANDLE hPhone, std::vector < cal_sweep_record > & SwpRecords, unsigned char bCompressedRequest, unsigned char bCompressedResponse,
   unsigned long firstSegNum, unsigned long lastSegNum, bool isLastSweep )
{
   // The number of segments to be executed.
   unsigned long segSize = lastSegNum - firstSegNum + 1;

   unsigned char status = QLIB_FTM_CFG_AND_EXEC_QSC_SWEEP( hPhone,
                                             static_cast<void*>(&(SwpRecords[firstSegNum])),        // Array of sweep records
                                             segSize,                // number of segments
                                             isLastSweep,                   // Is Last pSweep?
                                             true,// Enable Debug Mode?,
                                             bCompressedRequest && segSize > 4);

   if(status == 0)
   {
      RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, "QLIB_FTM_CFG_AND_EXEC_QSC_SWEEP API failed."); 
      return false;
   }


   // Wait for opcode execution and check whether Get Sweep result is needed or not]
   bool isResultExpected = false;
   if (status)
   {
      unsigned long iWaitTime = 0;
      for (UINT idx = 0; idx < segSize; idx++)
      {
         iWaitTime += static_cast<UINT>(SwpRecords[firstSegNum+idx].opcode.segLen);
         if ( (!isResultExpected) && SwpRecords[firstSegNum+idx].resExpected)
         {
            isResultExpected = true;
         }
      }
      Sleep(iWaitTime/1000);
   }
   // FTM_GET_ESC_SWEEP_RESULTS is executed only when FTM_CFG_AND_EXEC_ESC_SWEEP returns true AND Result is expected
   if (status && isResultExpected)
   {
      // Get OpCode Result
      unsigned long long errCode = 0;
      unsigned short errSegNum = 0;

      // Check Compressed FTM_GET_ESC_SWEEP_RESULTS or not
      // Result will be stored at segment [firstSegNum], not segment zero.
      if( bCompressedResponse && segSize > 4 )
      {
         status = QLIB_FTM_GET_ESC_SWEEP_RESULTS_COMPRESSED( hPhone,
                                                             static_cast<void*>(&(SwpRecords[firstSegNum])),
                                                             segSize,
                                                             &errCode,
                                                             &errSegNum );
      }
      else
      {
         status = QLIB_FTM_GET_ESC_SWEEP_RESULTS( hPhone,
                                                  static_cast<void*>(&(SwpRecords[firstSegNum])),
                                                  segSize,
                                                  &errCode,
                                                  &errSegNum );
      }
      if(status == 0)
      {
         return false;
      }
   }

   return true;
}

unsigned char RFCal::QSC::ExecuteRadioSetup(HANDLE hPhone, RFMPathDB * rfmPathDBPtr, unsigned short segmentDuration, unsigned long handle_id,unsigned char tech, unsigned short band, unsigned char subband,unsigned long channel, int path_type )
{
   // Get RFM Path for PRx
   vector < CRFPathInformationPacket > rfmPaths;
   if (!(rfmPathDBPtr->QueryRFMPathInfoByPathType(hPhone, tech, band, subband, path_type /* Path Type */,rfmPaths, 1 /* Calibration Action Type */)))
   {
      cout << "Failed in getting RFM Path info " << endl;
      return false;
   }
   
   if (PATH_TYPE_PRX == path_type)
   {
      // Restrict only one path
      rfmPaths.resize(1);
   }
   ftm_cal_seq_bld_radio_setup_path_params common_param;
   memset(&common_param,0, sizeof(ftm_cal_seq_bld_radio_setup_path_params));
   common_param.bwType = 3;
   common_param.band = band;
   common_param.subBand = subband;
   common_param.numChannels = 1;
   common_param.channelList[0] = channel;

   vector <cal_sweep_record> swp_records;
   RFCal::SetOpCode::InitSwpRecords(hPhone, swp_records, 2);
   if ( swp_records.empty() )
   {
      return false;
   }
   vector <cal_sweep_record>::iterator swpIt = swp_records.begin();

   // Segment 0
   RFCal::SetOpCode::SeqBuild::RadioSetup(hPhone, (*swpIt),segmentDuration, handle_id,tech, false /* isTearDown */,
      RADIO_SETUP_TECH_BAND_PATH_CHAN, &common_param, &rfmPaths );
   swpIt++;

   if ( swpIt == swp_records.end() )
   {
      return false;
   }
   // Segment 1
   RFCal::SetOpCode::SeqExec::RadioSetup(hPhone, (*swpIt), segmentDuration, handle_id, tech);
   swpIt++;
   
   swp_records.resize(std::distance(swp_records.begin(), swpIt));

   unsigned char status = RFCal::QSC::ExecuteQSCSweepRecords(hPhone, swp_records, false /* isTearDown */, true /* isDebug */, false /* isCompressed */);

   unsigned long sleepTime = RFCal::QSC::GetTotalSegmentTime(hPhone, swp_records);

   ::Sleep(sleepTime/1000 + 10);
   return status;
}
unsigned char RFCal::QSC::ExecuteTeardown(HANDLE hPhone, unsigned short segmentDuration, unsigned long handle_id,unsigned char tech)
{
   vector <cal_sweep_record> swp_records;
   RFCal::SetOpCode::InitSwpRecords(hPhone, swp_records, 2);
   vector <cal_sweep_record>::iterator swpIt = swp_records.begin();
   if ( swp_records.empty() )
   {
      return false;
   }
   /* Tear down*/
   swpIt = swp_records.begin();

   // Segment 0
   RFCal::SetOpCode::SeqBuild::RadioSetup(hPhone, (*swpIt),segmentDuration, handle_id,tech, true /* isTearDown */);
   swpIt++;
   if ( swpIt == swp_records.end() )
   {
      return false;
   }

   // Segment 1
   RFCal::SetOpCode::SeqExec::RadioSetup(hPhone, (*swpIt), segmentDuration, handle_id, tech);
   swpIt++;

   swp_records.resize(std::distance(swp_records.begin(), swpIt));

   RFCal::QSC::ExecuteQSCSweepRecords(hPhone, swp_records, true /* isTearDown */, true /* isDebug */, false /* isCompressed */);

   unsigned long sleepTime = RFCal::QSC::GetTotalSegmentTime(hPhone, swp_records);

   ::Sleep(sleepTime/1000 + 10);
   return true;
}

unsigned char RFCal::Get_Therm_Val(HANDLE hPhone, const int tech, const int band, const int subBand,const CRFM_Path & rfm_path, int& therm_val, int& therm_val_degC)
{
   unsigned char     status            = true;
   unsigned short    thermVal          = 0;
   short             thermValDegC      = 0;

   status = QLIB_FTM_GET_UNIFIED_ENH_THERM_V2( hPhone, static_cast<byte>(tech), static_cast<byte>(band),
      static_cast<byte>(subBand), static_cast<byte>(rfm_path.signal_path), static_cast<byte>(rfm_path.rfm_dev), &thermVal, &thermValDegC );

   therm_val = static_cast <int>( thermVal );
   therm_val_degC = static_cast <int>( thermValDegC );

   return status;
}

unsigned char RFCal::Do_FTM_DO_SELF_CAL(HANDLE hPhone)
{
   unsigned char _bOK      = false;
   stringstream ss;
   ss << endl;
   ss << "===========================================" << endl;
   ss << " Internal Device Cal" << endl;
   ss << "===========================================" << endl << endl;

   RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ss.str());

   //********************************************************//
   //             Set Calibration state ON
   //********************************************************//
   _bOK = ::QLIB_FTM_SET_CALIBRATION_STATE( hPhone,true);

   //********************************************************//
   //            FTM DO SELF_CAL
   //********************************************************//

   _bOK = ::QLIB_FTM_DO_SELF_CAL(hPhone,0);
   if ( _bOK )
      RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, "Internal Device Cal : Success");
   else
      RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, "Internal Device Cal : Failed");

   if ( _bOK )
   {
      //********************************************************//
      //            QLIB_EFS2_SyncWithWait
      //********************************************************//     
      _bOK = RFCal::COMMON::SYNC_EFS( hPhone );
   }
 
   return _bOK;
}
unsigned char RFCal::COMMON::SYNC_EFS(HANDLE hPhone)
{
   unsigned char _bOK      = false;
   //********************************************************//
   //            QLIB_EFS2_SyncWithWait
   //********************************************************//
   unsigned char piStatus = 0;
   _bOK = ::QLIB_EFS2_SyncWithWait(hPhone,(unsigned char *)"/",10000,&piStatus);
   if ( _bOK )
      RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, "EFS2_SyncWithWait : Success");
   else
      RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, "EFS2_SyncWithWait : Failed");
   return _bOK;
}
RFCal::DeviceDB::~DeviceDB()
{
   if ( NULL != pMgr )
   {
      delete pMgr;
      pMgr = NULL;
   }
}
unsigned char RFCal::DeviceDB::UnregisterDevice ()
{
   DWORD mutexRes = WaitForSingleObject( deviceMutex, WAIT_TIME);
   if ( mutexRes == WAIT_OBJECT_0)
   {
   if (NULL != pMgr)
   {
      delete pMgr;
      pMgr = NULL;
   }
      ReleaseMutex( deviceMutex );
   return true;
}
   else
   {
      // Failure....
      return false;
   }
}
unsigned char RFCal::DeviceDB::RegisterDevice (HANDLE hPhone, int equipment_type, int gpib_board, int prim_addr, int sec_addr, bool IsSimulationMode )
{
   RFCal::EQUIPMENT_TYPE type = static_cast < RFCal::EQUIPMENT_TYPE >(equipment_type);
   DWORD mutexRes = WaitForSingleObject( deviceMutex, WAIT_TIME);
   if ( mutexRes == WAIT_OBJECT_0)
   {
      if (NULL != pMgr)
      {
         delete pMgr;
         pMgr = NULL;
      }
      switch (type)
      {
         // QDART QLIBDEMO support only QSCPI
         case QSCPI:
            pMgr = new CMeasurementManager_QSCPI();
            break;
          
         default:
   #ifndef QLIB_DEMO_EXT
            pMgr = RFCal::CMeasurementManager_Misc::GetMeasurementManager(type);
   #endif

            break;
      }
      if ( IsSimulationMode )
      {
         if ( NULL != pMgr)
         {
            pMgr->Set_SimulationMode ( TRUE );
            pMgr->Set_DUTHandle( hPhone );
         }
      }
      else
      {
         if (NULL != pMgr)
         {
            HANDLE hInst = pMgr->Connect ( hPhone, gpib_board, prim_addr, sec_addr);
            if ( NULL == hInst)
            {
               delete pMgr;
               pMgr = NULL;
            }
         }
      }
      ReleaseMutex( deviceMutex );
   }
   else
   {
      // Failure...
   }

   return (NULL != pMgr)? true : false;
};
