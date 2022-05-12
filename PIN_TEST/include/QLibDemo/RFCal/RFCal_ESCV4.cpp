/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/RFCal_ESCV4.cpp#1 $
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

#include "RFCal_ESCV4.h"

#include <fstream>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <process.h>
#include <iterator>
#include <iomanip>

/* critical section variable */
CRITICAL_SECTION cs;

extern HANDLE g_hResourceContext;
void Test_RFCal_ESC_ET_CAL(HANDLE hPhone, int iOption, unsigned char bSimulationMode, unsigned char bTESimulationMode, int numloop)
{
   vector < QLIB_Demo_RFCAL_ESC_ET_Param > demo_param;
   demo_param.resize(1);

   unsigned char bSupported = true; // whether the option is supported in this demo or not
   unsigned char bAutoScanPort = true;

   //********************************************************//
   //   Assign common parameters for this demo
   //********************************************************//

   // Check the folder (32 bits folder) exist, if not, use 64 bit folder.
   const string stRFCalInput_WTR1625 = "WTR1625\\RFCALInput\\";
   const string stRFCalInput_WTR1605 = "WTR1605\\RFCALInput\\";
   const string stRFCalInput_WTR2955 = "WTR2955\\RFCALInput\\";
   
   const string stRFCalInput_WTR3925 = "WTR3925\\RFCALInput\\";
    const string stRFCalInput_WTR4905 = "WTR4905\\RFCALInput\\";

   string param_folder_root = "C:\\Program Files\\Qualcomm\\QDART\\XTT\\SubSysRFCalibration\\Multimodes\\";
  demo_param[0].stXSLFileName = "C:\\Program Files\\Qualcomm\\QDART\\Databases\\HTML_QDART_Combined.xsl";
    demo_param[0].stCalDBFilepath = "C:\\Program Files\\Qualcomm\\QDART\\Databases\\CalDB_NET.xml";
   if ( access( param_folder_root.c_str(), 0 ) != 0 )
   {
      param_folder_root = "C:\\Program Files (x86)\\Qualcomm\\QDART\\XTT\\SubSysRFCalibration\\Multimodes\\";
        demo_param[0].stCalDBFilepath = "C:\\Program Files (x86)\\Qualcomm\\QDART\\Databases\\CalDB_NET.xml";
      demo_param[0].stXSLFileName = "C:\\Program Files (x86)\\Qualcomm\\QDART\\Databases\\HTML_QDART_Combined.xsl";
   }

   string param_folder = param_folder_root + stRFCalInput_WTR1625;
   demo_param[0].stESC_Params_filename = param_folder + "WTR1625_NAEU_CA_ESC_Params.xml";
   demo_param[0].stESC_Char_filename   = param_folder + "WTR1625_NAEU_CA_ESC_Char.xml";
   demo_param[0].stPA_PARAM_filename   = param_folder + "RFCAL_PA_PARAM.xml";
   demo_param[0].stResultXML_filename  = "c:\\temp\\result.xml";
   demo_param[0].bUseXMLresult = true;
   demo_param[0].stQMSLLogFileName = "C:\\temp\\";
   demo_param[0].stTestName = "RFCal";

   /* <Node_Name name="WCDMA_B1_Dev0_0"> <==  Technology_Band_Dev[DeviceNumber]_[Instance] */
   demo_param[0].BAND = 1;
   demo_param[0].Device = 0;
   demo_param[0].Instance = 0;
   demo_param[0].hPhone = hPhone;

   demo_param[0].bSimulationMode = bSimulationMode;
    demo_param[0].bUseSET_PATH_SEL_OVERRIDE = 0;

    demo_param[0].bEnableQMSLLogging = 1;
   demo_param[0].bEnableDLFLogging = 0;
    demo_param[0].stDMCFilename = "c:\\temp\\FTM_RF.dmc"; // QXDM file mask.
    
    demo_param[0].bCallboxType = 0; // 
    demo_param[0].GPIB_BoardNumber = 0;
    demo_param[0].GPIB_PrimaryAddress = 20;
    demo_param[0].GPIB_SecondaryAddress = 0;
    demo_param[0].bTESimulationMode = bTESimulationMode;
    demo_param[0].bDoNotUseEstimationForTxIntCal = 1;  
   if (bSimulationMode)
   {
      demo_param[0].bEnableDLFLogging = 0;
      demo_param[0].bTESimulationMode = true;
   }
   demo_param[0].bUseTearDownSweep = true;
   //********************************************************//
   //   Assign parameters according to selected Option
   //********************************************************//

   switch(iOption)
   {
      case 0:
         param_folder = param_folder_root + stRFCalInput_WTR3925;
         demo_param[0].stESC_Params_filename = param_folder + "WTR3925_2P0_SSKU_QFES_ET_NF604_ESC_Params.xml";
         demo_param[0].stESC_Char_filename = param_folder  + "WTR3925_2p0_SSKU_QFEs_ET_NF604_ESC_Char.xml";
         demo_param[0].stPA_PARAM_filename = param_folder  + "RFCAL_PA_PARAM.xml";

         demo_param[0].TECH = 3;// LTE
         demo_param[0].BAND = 1;// LTE
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_ET_LTE_B1.txt");
         demo_param[0].stTestName = "ESC LTE B1";
         break;
      case 1: // LTE B1 ET case.
         demo_param[0].TECH = 3;// LTE
         demo_param[0].BAND = 2;//
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_ET_LTE_B2_InterCA.txt");
         demo_param[0].stTestName = "ESC LTE B2";
         break;
      case 2: // LTE B1 ET case.
         demo_param[0].TECH = 3;// LTE
         demo_param[0].BAND = 25;//
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_ET_LTE_B25_IntraCA.txt");
         demo_param[0].stTestName = "ESC LTE B25";
         break;
      case 3: // LTE B1 ET case.
         demo_param[0].TECH = 3;// LTE
         demo_param[0].BAND = 41;//
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_ET_LTE_B41_TDD_APT.txt");
         break;
        case 4: // LTE B41A TDD QFE case
         demo_param[0].TECH = 3;// LTE
         demo_param[0].BAND = 41;//
         param_folder = param_folder_root + stRFCalInput_WTR3925;
         demo_param[0].stESC_Params_filename = param_folder  + "WTR3925_2p0_SSKU_QFEs_ET_ESC_Params.xml";
         demo_param[0].stESC_Char_filename = param_folder  + "WTR3925_2p0_SSKU_QFEs_ET_ESC_Char.xml";
         demo_param[0].stPA_PARAM_filename = param_folder  + "RFCAL_PA_PARAM.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_LTE_B41_A.txt");
         demo_param[0].bUseSET_PATH_SEL_OVERRIDE = 0;
         break;
      case 5: // LTE B1 EPT case
         demo_param[0].TECH = 3;// LTE
         param_folder = param_folder_root + stRFCalInput_WTR1605;
         demo_param[0].stPA_PARAM_filename = param_folder + "RFCAL_PA_PARAM_X1.xml";
         demo_param[0].stESC_Char_filename = param_folder + "WTR1605_CHILE_SxDSDA_V2_ESC_char.xml";
         demo_param[0].stESC_Params_filename = param_folder + "WTR1605_CHILE_SxDSDA_V2_ESC_params_X1.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_LTE_B1_EPT.txt");
         break;
      case 6: // LTE B1 QFE ET-EPT case.
         demo_param[0].TECH = 3;// LTE
         demo_param[0].stESC_Params_filename = param_folder + "WTR1625_CMCC_QFE_ESC_Params.xml";
         demo_param[0].stESC_Char_filename = param_folder + "WTR1625_CMCC_QFE_ESC_Char.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_ET_LTE_B1_QFE_ET_EPT.txt");
         break;
      case 7: // WCDMA B1 ET case.
         demo_param[0].TECH = 1;// WCDMA
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_ET_WCDMA_B1.txt");
         break;
      case 8: // WCDMA B1 EPT case
         demo_param[0].TECH = 1;// LTE
         param_folder = param_folder_root + stRFCalInput_WTR1605;
         demo_param[0].stPA_PARAM_filename = param_folder + "RFCAL_PA_PARAM_X1.xml";
         demo_param[0].stESC_Char_filename = param_folder + "WTR1605_CHILE_SxDSDA_V2_ESC_char.xml";
         demo_param[0].stESC_Params_filename = param_folder + "WTR1605_CHILE_SxDSDA_V2_ESC_params_X1.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_WCDMA_B1_QFE_EPT.txt");
         break;
      case 9: // WCDMA B1 QFE ET-EPT case.
         param_folder = param_folder_root + stRFCalInput_WTR3925;
         demo_param[0].stESC_Params_filename = param_folder + "WTR3925_2P0_SSKU_QFES_ET_ESC_Params.xml";
         demo_param[0].stESC_Char_filename = param_folder  + "WTR3925_2p0_SSKU_QFEs_ET_ESC_Char.xml";
         demo_param[0].stPA_PARAM_filename = param_folder  + "RFCAL_PA_PARAM.xml";

         demo_param[0].TECH = 1;// LTE
         demo_param[0].BAND = 1;// LTE
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_ET_WCDMA_B1.txt");
         demo_param[0].stTestName = "ESC WCDMA B1";
         break;
      case 10: // CDMA BC1 ET case  Use XML
         demo_param[0].TECH = 0;// CDMA
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_ET_CDMA_B1.txt");
         break;
      case 11: // CDMA B1 EPT case
         demo_param[0].TECH = 0;// CDMA
         demo_param[0].stPA_PARAM_filename = param_folder + "RFCAL_PA_PARAM_X1.xml";
         demo_param[0].stESC_Char_filename = param_folder + "WTR1605_CHILE_SxDSDA_V2_ESC_char.xml";
         demo_param[0].stESC_Params_filename = param_folder + "WTR1605_CHILE_SxDSDA_V2_ESC_params_X1.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_CDMA_B1_EPT.txt");
         break;
      case 12: // CDMA B1 QFE ET-EPT case.
         demo_param[0].TECH = 3;// LTE
         demo_param[0].stESC_Params_filename = param_folder + "WTR1625_CMCC_QFE_ESC_Params.xml";
         demo_param[0].stESC_Char_filename = param_folder + "WTR1625_CMCC_QFE_ESC_Char.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_ET_CDMA_B1_QFE_ET_EPT.txt");
         break;
      case 13: // TDSCDMA B40 APT case
         demo_param[0].TECH = 4;// TDSCDMA
         demo_param[0].BAND = 40;//
         demo_param[0].stESC_Params_filename = param_folder + "WTR1625_APAC_CA_ESC_Params.xml";
         demo_param[0].stESC_Char_filename = param_folder + "WTR1625_APAC_CA_ESC_Char.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_TDSCDMA_B40_APT.txt");
         break;
      case 14: // Ref Cal LTE B1 case
         demo_param[0].TECH = 3;//
         demo_param[0].BAND = 1;//
         demo_param[0].stESC_Params_filename = param_folder + "WTR1625_NAEU_CA_REF_CAL_ESC_Params.xml";
         demo_param[0].stRefCalOffset_filename = "c:\\temp\\ref_cal_offset_data.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_REF_CAL_LTE_B1.txt");
         break;
      case 15: // Ref Cal WCDMA B1 case
         demo_param[0].TECH = 1;//
         demo_param[0].BAND = 1;//
         demo_param[0].stESC_Params_filename = param_folder + "WTR1625_NAEU_CA_REF_CAL_ESC_Params.xml";
         demo_param[0].stRefCalOffset_filename = "c:\\temp\\ref_cal_offset_data.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_REF_CAL_WCDMA_B1.txt");
         break;
      case 16: // Int Cal LTE B1 case
         demo_param[0].TECH = 3;//
         demo_param[0].BAND = 1;//
         demo_param[0].stESC_Params_filename = param_folder + "WTR1625_NAEU_CA_INT_CAL_ESC_Params.xml";
         demo_param[0].stRefCalOffset_filename = "c:\\temp\\test_ref_cal_data.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_INT_CAL_LTE_B1.txt");
         break;
      case 17: // Int Cal WCDMA B1 case
         demo_param[0].TECH = 1;//
         demo_param[0].BAND = 1;//
         demo_param[0].stESC_Params_filename = param_folder + "WTR1625_NAEU_CA_INT_CAL_ESC_Params.xml";
         demo_param[0].stRefCalOffset_filename = "c:\\temp\\test_ref_cal_data.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_INT_CAL_WCDMA_B1.txt");
         break;

        case 18: // FBRxCal LTE B1 case (please note we specicy instance 1 for params.xml) WTR3925
         demo_param[0].TECH = 3;//
         demo_param[0].BAND = 1;//
         demo_param[0].Instance = 1;//
         param_folder = param_folder_root + stRFCalInput_WTR3925;
         demo_param[0].stESC_Params_filename = param_folder  + "WTR3925_2p0_SSKU_QFEs_ET_ESC_Params.xml";
         demo_param[0].stESC_Char_filename = param_folder  + "WTR3925_2p0_SSKU_QFEs_ET_ESC_Char.xml";
         demo_param[0].stPA_PARAM_filename = param_folder  + "RFCAL_PA_PARAM.xml";
         demo_param[0].stRefCalOffset_filename = "c:\\temp\\ref_cal_offset_data.xml";
            demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_WTR3925_FBRxCal_LTE_B1.txt");
         demo_param[0].stTestName = "LTE B1 - FBRx Cal";

         break;
      case 19: // FBRx Gain Cal WCDMA B1 case (please note we specicy instance 1 for params.xml) WTR3925
         demo_param[0].TECH = 1;//
         demo_param[0].BAND = 1;//
         demo_param[0].Instance = 1;//
         param_folder = param_folder_root + stRFCalInput_WTR3925;
         demo_param[0].stESC_Params_filename = param_folder  + "WTR3925_2p0_SSKU_QFEs_ET_ESC_Params.xml";
         demo_param[0].stESC_Char_filename = param_folder + "WTR3925_2p0_SSKU_QFEs_ET_ESC_Char.xml";
         demo_param[0].stPA_PARAM_filename = param_folder  + "RFCAL_PA_PARAM.xml";
         demo_param[0].stRefCalOffset_filename = "c:\\temp\\ref_cal_offset_data.xml";
            demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_WTR3925_FBRxCal_WCDMAB1.txt");
         break;
      case 20: // LTE B1 ET_EPT for WTR3925
         demo_param[0].TECH = 3;//
         demo_param[0].BAND = 1;//
         demo_param[0].Instance = 0;//
         param_folder = param_folder_root + stRFCalInput_WTR3925;
         demo_param[0].stESC_Params_filename = param_folder  + "WTR3925_2p0_SSKU_QFEs_ET_ESC_Params.xml";
         demo_param[0].stESC_Char_filename = param_folder + "WTR3925_2p0_SSKU_QFEs_ET_ESC_Char.xml";
         demo_param[0].stPA_PARAM_filename = param_folder  + "RFCAL_PA_PARAM.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_WTR3925_LTEB1.txt");
         break;
      case 21: // WCDMA B1 ET_EPT for WTR3925
         demo_param[0].TECH = 1;//
         demo_param[0].BAND = 1;//
         demo_param[0].Instance = 0;//
         param_folder = param_folder_root + stRFCalInput_WTR3925;
         demo_param[0].stESC_Params_filename = param_folder  + "WTR3925_2p0_SSKU_QFEs_ET_ESC_Params.xml";
         demo_param[0].stESC_Char_filename = param_folder + "WTR3925_2p0_SSKU_QFEs_ET_ESC_Char.xml";
         demo_param[0].stPA_PARAM_filename = param_folder  + "RFCAL_PA_PARAM.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_WTR3925_WCDMAB1.txt");
         break;
      case 22: // TDSCDMA B34 EPT case
         demo_param[0].TECH = 4;// TDSCDMA
         demo_param[0].BAND = 34;// TDSCDMA
         param_folder = param_folder_root + stRFCalInput_WTR1605;
         demo_param[0].stPA_PARAM_filename = param_folder + "RFCAL_PA_PARAM_X1.xml";
         demo_param[0].stESC_Char_filename = param_folder + "WTR1605_CHILE_SxDSDA_V2_ESC_char.xml";
         demo_param[0].stESC_Params_filename = param_folder + "WTR1605_CHILE_SxDSDA_V2_ESC_params_X1.xml";
         demo_param[0].stQMSLLogFileName.append("QLIB_Demo_RFCAL_ESC_V4_TDSCDMA_B34_EPT.txt");
         break;



      case 23: // Multi DUT cal test purpose (just a demo)
         {
            bAutoScanPort = true;
            int numOfDut = 2;

            demo_param.resize(numOfDut);

            demo_param[1] = demo_param[0];
            demo_param[0].TECH = 3; // LTE
            demo_param[1].stQMSLLogFileName =  "C:\\temp\\QLIB_Demo_RFCAL_ESC_ET_LTE_B1-Thread0.txt";

            demo_param[1].TECH = 3; // LTE
            demo_param[1].stQMSLLogFileName =  "C:\\temp\\QLIB_Demo_RFCAL_ESC_ET_LTE_B1-Thread1.txt";

            if (bAutoScanPort)
            {
               QLIB_DisconnectAllServers();
               unsigned short iNumPorts = numOfDut;
               unsigned short PortList[1024] = {0};
               QLIB_GetAvailablePhonesPortList(&iNumPorts,PortList,0, (unsigned short *)NULL);
               if (iNumPorts < numOfDut)
               {
                  cout << "please check available DUTs";
                  return;
               }

               for (int i=0; i < numOfDut; i++)
               {
                  demo_param[i].hPhone = QLIB_ConnectServerWithWait(PortList[i],1000);
               }
            }
            else
            {
               demo_param[0].COM_Port = 47;
               demo_param[1].COM_Port = 55;
               QLIB_DisconnectAllServers();
               for (int i=0; i < numOfDut; i++)
               {
                  demo_param[i].hPhone = QLIB_ConnectServerWithWait(demo_param[i].COM_Port,1000);
               }
            }
         }
         break;
      default:
         bSupported = false;
         break;
   }
   if (bSupported)
   {
      //********************************************************//
      //   Chenk params.XML. PA_PARAM.XML, char.XML exist
      //   Create Log file name
      //********************************************************//
      for (UINT i=0; i< demo_param.size(); i++)
      {
         if ( access( demo_param[i].stPA_PARAM_filename.c_str(), 0 ) != 0 )
         {
            cout << demo_param[i].stPA_PARAM_filename << " not found." << endl << endl ;
            return;
         }
         if ( access( demo_param[i].stESC_Params_filename.c_str(), 0 ) != 0 )
         {
            cout << demo_param[i].stESC_Params_filename << " not found." << endl << endl ;
            return;
         }
         if ( access( demo_param[i].stESC_Char_filename.c_str(), 0 ) != 0 )
         {
            cout << demo_param[i].stESC_Char_filename << " not found." << endl << endl ;
            return;
         }
            string stRFCalConfig[] = {"RFCAL_CDMA","RFCAL_WCDMA","RFCAL_GSM","RFCAL_LTE","RFCAL_TDSCDMA"};
            byte bTempRFCalConfig = (demo_param[i].TECH < 5)?i:1;
            demo_param[i].stCalConfigName = stRFCalConfig[demo_param[i].TECH];

      }
      //**************************************************//
      //   Execute cal thread
      //**************************************************//
      if (numloop == 0)
      {
         string stHTMLFile = demo_param[0].stQMSLLogFileName.substr(0, demo_param[0].stQMSLLogFileName.length()-4);
         demo_param[0].stXMLFileName =  stHTMLFile + ".xml";
         demo_param[0].stHTMLFileName = stHTMLFile + ".html";
         RFCal_Helper_Demo_MultiDUT_Cal(demo_param);
      }
      else
      {
         string orgst = demo_param[0].stQMSLLogFileName.substr(0, demo_param[0].stQMSLLogFileName.length()-4);
         for (int i = 0; i < numloop; i++)
         {
            //time_t curtime;
            //struct tm *loctimep;
            //curtime = time( NULL );
            //loctimep = localtime( &curtime );
            stringstream ss;
            ss << orgst; //<< "-" << (loctimep->tm_year+1900) << "-" << (loctimep->tm_mon+1) << "-" << loctimep->tm_mday << "-"
            // << loctimep->tm_hour << "_"  << loctimep->tm_min << "_"  << loctimep->tm_sec;
            if (numloop > 1)
            {
               ss << "-Loop_" <<i;
            }
//            ss <<".txt";

            demo_param[0].stQMSLLogFileName = ss.str();
                demo_param[0].stDlfFileName = demo_param[0].stQMSLLogFileName + "-DLF.dlf";
            demo_param[0].stXMLFileName =  ss.str() + ".xml";
            if ( access(  demo_param[i].stXMLFileName.c_str(), 0 ) == 0 )
            {
               remove(demo_param[i].stXMLFileName.c_str());
            }
            demo_param[0].stHTMLFileName = ss.str() + ".html";
            demo_param[0].stQMSLLogFileName.append(".txt");

            RFCal_Helper_Demo_MultiDUT_Cal(demo_param);
         }
      }
   }
}

unsigned char Test_RFCal_ESC_ET(QLIB_Demo_RFCAL_ESC_ET_Param & demo_param)
{
   unsigned char bOK = true;
   HANDLE hPhone = demo_param.hPhone;
   dword iStartTime = 0;
   time_t t_StartTime = time( NULL );

   //*********************************************//
   //    Start QMSL Logging
   //*********************************************//

    if (demo_param.bEnableQMSLLogging)
    {
        RFCal::Helper::Log::StartQMSLLogging(hPhone, demo_param.stQMSLLogFileName);
        QLIB_DIAG_SetPhoneLoggingState( hPhone, true ); 
    }

   //*********************************************//
   //   Read QXDM mask (.dmc) file 
   //   Start DLF Logging 
   //   DLF file can be converted into isf (QXDM) file or analyzed with QCAT
   //*********************************************//

    if (demo_param.bEnableDLFLogging)
    {
        RFCal::Helper::DIAG::Set_DMC_File(hPhone,demo_param.stDMCFilename.c_str());
        QLIB_StartDLF_Logging(hPhone,(char *)(demo_param.stDlfFileName.c_str()),true);
    }

   RFCal_Helper_AddMessageToQMSLLog(hPhone,"****************************************************************");
   RFCal_Helper_AddMessageToQMSLLog(hPhone,"Start QLIB Demo ESC ET Cal");
   RFCal_Helper_AddMessageToQMSLLog(hPhone,"****************************************************************");
   
   string stBuildID = "";
   RFCal_helper_Log_Build_ID(hPhone, stBuildID);

   RFCal_Helper_AddMessageToQMSLLog(hPhone,demo_param.stPA_PARAM_filename);
   RFCal_Helper_AddMessageToQMSLLog(hPhone,demo_param.stESC_Params_filename);
   RFCal_Helper_AddMessageToQMSLLog(hPhone,demo_param.stESC_Char_filename);

   //*********************************************//
   //    Assign Parameters
   //*********************************************//
   Cal_Param cal_param;
   /* Load params.xml */
   RFCal_Helper_ESC_ET_Param_Assignment_XML(demo_param, cal_param);
   /* Load char.xml */
   RFCal_Helper_ESC_ET_CharData_Assignment_XML(demo_param, cal_param);
   /* Load PA_PARAMS.xml */
   RFCal_Helper_ESC_ET_PA_PARAM_Assignment_XML(demo_param, cal_param);
   /* Load Ref Cal Offset Data */
//  RFCal_Helper_ESC_ET_RefCal_Tx_Data_Assignment_XML(demo_param, cal_param);


   // ******************************************* //
   // Create XML report
   // ******************************************* //
   if (!RFCal_Helper_Generate_XMLReport(demo_param.stXMLFileName,demo_param.xmlDoc))
   {
      demo_param.xmlDoc = NULL;
   }
   else
   {
      /* Get Build ID */
      string stBuildID = "";
      RFCal_helper_Log_Build_ID(hPhone, stBuildID);

      /* Set UUTID and Build ID into XML */
      RFCal_Helper_SetUUT_BuildID(demo_param.xmlDoc,"QlibDemo",stBuildID);

      /* Add test name */
      RFCal_Helper_AddTestName(demo_param.xmlDoc,demo_param.TestPtr,"ESC",demo_param.stTestName,demo_param.stTestName);
      demo_param.xmlDoc->save((_bstr_t)demo_param.stXMLFileName.c_str());
   }


   int Swp1 = 0;
   int Swp2 = 1;

   //*********************************************//
   //    Initialize sweep record list
   //*********************************************//
   vector < vector < cal_sweep_record > > Sweep;
    cal_param.SwpRecords = &Sweep;
   Sweep.resize(cal_param.Swp.size());

   for (UINT i=0; i < cal_param.Swp.size(); i++)
   {
      RFCal_Helper_ESC_ET_Initialize_Sweep(Sweep[i],cal_param.Swp[i],demo_param.NumOfSegments[i],(int)demo_param.TECH,demo_param.BAND);
        cal_param.Swp[i].bFirstSweep = (i == 0)? true : false; // Qlib Demo original
   }

   //*********************************************//
   //    Check FTM Mode
   //*********************************************//
   if (!demo_param.bSimulationMode)
   {
   //   if(!RFCal_Helper_Change_FTM_Mode(hPhone)) throw RFCal_Helper_Exception("Failed in FTM mode change",  __FILE__, __LINE__ );
   }

    //*********************************************//
    //      Read Path loss from CalDB_Net.xml
    //*********************************************//
    map <double, double> ULLoss;
    map <double, double> DLLoss;
    map <double, double> AUXLoss;

    RFCal::Helper::GetPathLossFromCalDB_XML(hPhone,demo_param.stCalDBFilepath,demo_param.stCalConfigName,CALPATH_DownlinkCP1,-1,-1,DLLoss);
    RFCal::Helper::GetPathLossFromCalDB_XML(hPhone,demo_param.stCalDBFilepath,demo_param.stCalConfigName,CALPATH_UplinkCP1,-1,-1,ULLoss);
    RFCal::Helper::GetPathLossFromCalDB_XML(hPhone,demo_param.stCalDBFilepath,demo_param.stCalConfigName,CALPATH_DownlinkAuxCP1,-1,-1,AUXLoss);
    //*********************************************//
    //      Update Path loss to CalDB_Net.xml
    //*********************************************//
    {
        // Comment out for demo purpose
        // ULLoss[1960] = 8.0; //
        // ULLoss[2100] = 7.5; //
        // string newCalDB_filename = demo_param.stCalDBFilepath;
        // RFCal_Helper_SetPathLossToCalDB_XML(demo_param.stCalDBFilepath,newCalDB_filename,demo_param.stCalConfigName,CALPATH_UplinkCP1,ULLoss);
    }
#ifdef ON_EQUIPMENT
      //*********************************************//
      //    Equipment Initialization and set path loss
      //*********************************************//
#endif

    //*********************************************//
   //    Set cal state and set mode
   //*********************************************//
   if (!demo_param.bSimulationMode)
   {
      short modeID[]= {FTM_MODE_ID_CDMA_1X,FTM_MODE_ID_WCDMA,FTM_MODE_ID_GSM,FTM_MODE_ID_LTE,FTM_MODE_ID_TDSCDMA};
        //bOK &= QLIB_FTM_SET_MODE_ID(hPhone,modeID[demo_param.TECH]);
    //    bOK &= QLIB_FTM_SET_MODE(hPhone,cal_param.RF_Mode);
      if (cal_param.Set_CalState)
      {
      bOK &= QLIB_FTM_SET_CALIBRATION_STATE(hPhone, true);
      }
   if (!bOK) 
   {
   throw RFCal_Helper_Exception("FTM_SET_MODE or SET_CALIBRATION_STATE Failed.", __FILE__, __LINE__); } 
   }

    //*********************************************//
    //      Set Path Override for Split band
    //*********************************************//
    if (demo_param.bUseSET_PATH_SEL_OVERRIDE && demo_param.BAND == 41)
    {
        byte CarrierID = 0, band = 76, iTx = 0, iPRx = 0, iDRx = 0;
        QLIB_FTM_SET_PATH_SEL_OVERRIDE_INDEX(hPhone, CarrierID, band, iTx, iPRx, iDRx);
        CarrierID = 1;
        QLIB_FTM_SET_PATH_SEL_OVERRIDE_INDEX(hPhone, CarrierID, band, iTx, iPRx, iDRx);
    }


   //*********************************************//
   //    Start Calibration
   //*********************************************//
    UINT iNumSweeps = cal_param.Swp.size(); // This may be increased if there is non sweep mode.

   iStartTime = GetTickCount();

    for (UINT iSweepIndex = 0; iSweepIndex < iNumSweeps; iSweepIndex++)
   {
      //*********************************************//
      //   Initialize TE (Test Equipment) records
      //*********************************************//
      vector < TE_Cal_Swp_Params > TE_Records;
        cal_param.TeRecords = &TE_Records;
      TE_Records.clear();
      TE_Records.resize(Sweep[iSweepIndex].size());
      RFCal_Helper_Intialize_TE_Records(TE_Records);

        
      stringstream tmpStr;
      tmpStr << "Start Sweep " << (iSweepIndex+1);
      RFCal_Helper_AddMessageToQMSLLog(hPhone,"****************************************************************");
      RFCal_Helper_AddMessageToQMSLLog(hPhone,tmpStr.str());
      RFCal_Helper_AddMessageToQMSLLog(hPhone,"****************************************************************");

      //*********************************************//
      //    Create Sweep Records
      //*********************************************//
      Sweep_Params sweep_param = cal_param.Swp[iSweepIndex];

        //*********************************************//
        //      If Non Sweep mode, seprate Tx and Rx sweeps
        //      Tx Sweeps ---> Sweeps with each of channel
        //      Rx Sweeps ---> Serial Cal
        //*********************************************//
        if (!sweep_param.Enable_Sweep_Mode)
        {
            // Modify current sweep parameter to each seperated sweeps.
            RFCal_Helper_SeperateTx_RxSweeps(demo_param, cal_param, sweep_param,TE_Records);
        }
      //*********************************************//
      //   Create segments according to sweep_type
      //*********************************************//

        unsigned char bSweepMode = true;

        switch (sweep_param.Sweep_Type)
      {
         case MAX_BIAS_LINEARIZER: // MAX_BIAS_LINEARIZER = 0.
         case XPT_SWP1: // XPT_SWP1 = 2
                bOK &= RFCal_Helper_ESC_ET_createXPT1Swp(demo_param, cal_param, sweep_param, Sweep[iSweepIndex],TE_Records);
            break;

         case XPT_SWP2: // XPT_SWP2 = 6
         case INT_CAL_XPT_SWP2: // INT_CAL_XPT_SWP2 = 107
            bOK &= RFCal_Helper_ESC_ET_createXPT2Swp(demo_param, cal_param, sweep_param, Sweep[Swp1], Sweep[iSweepIndex]);
            break;

         case APT_LINEARIZER: //APT_LINEARIZER = 1.
         case XPT_SWP3: // XPT_SWP3 = 7
                bOK &= RFCal_Helper_ESC_ET_createXPT3Swp(demo_param, cal_param, sweep_param, Sweep[Swp1], Sweep[Swp2], Sweep[iSweepIndex],TE_Records); // In APT_LINEARIZER case, Swp2 will not be used.
            break;

         case MAX_BIAS_FREQCOMP: // MAX_BIAS_FREQCOMP = 3
         case APT_FREQCOMP: // APT_FREQCOMP = 4
         case XPT_FREQCOMP: // XPT_FREQCOMP = 8
         case XPT_FREQCOMP_STAGGER_RXTX: // XPT_FREQCOMP_STAGGER_RXTX = 11
         case XPT_FREQCOMP_SERIAL_TX:
                RFCal_Helper_ESC_ET_createXPTFreqCompSwp(demo_param, cal_param, sweep_param, Sweep, Sweep[iSweepIndex],TE_Records);
                break;
         case XPT_FREQCOMP_SERIAL_RX:
                RFCal_Helper_ESC_ET_createXPTFreqCompSwp(demo_param, cal_param, sweep_param, Sweep, Sweep[iSweepIndex],TE_Records);
                bSweepMode = false;
            break;

         case XPT_DELAY: // XPT_DELAY = 9
            bOK &= RFCal_Helper_ESC_ET_createXPTDelaySwp(demo_param, cal_param, sweep_param, Sweep[Swp2], Sweep[iSweepIndex]);
            break;

         case INTRACA_FCOMP: // INTRACA_FCOMP = 14
            bOK &= RFCal_Helper_ESC_ET_createIntraCAFreqCompSwp(demo_param, cal_param, sweep_param, Sweep[iSweepIndex],TE_Records);
            break;

         case REF_CAL: // Ref Cal 100
            bOK &= RFCal_Helper_ESC_ET_createRefCalSwp(demo_param, cal_param, sweep_param, Sweep[iSweepIndex],TE_Records);
            break;

         case INT_CAL_XPT_SWP1: // INT_CAL_XPT_SWP1 = 106
         case INT_CAL_MAX_BIAS_LINEARIZER:
            bOK &= RFCal_Helper_UpdateRFCalParamsFromCharFile(demo_param, cal_param, sweep_param);
            bOK &= RFCal_Helper_ESC_ET_createINT_CAL_XPTSwp1(demo_param, cal_param, sweep_param, Sweep[iSweepIndex],TE_Records);
            break;

         case INT_CAL_XPT_SWP3: // INT_CAL_XPT_SWP1 = 108
         case INT_CAL_APT_LINEARIZER: // INT_CAL_APT_LINEARIZER = 104
            bOK &= RFCal_Helper_ESC_ET_createINT_CAL_XPTSwp3(demo_param, cal_param, sweep_param, Sweep[Swp1], Sweep[Swp2], Sweep[iSweepIndex],TE_Records);
            break;

         case INT_CAL_XPT_FREQCOMP:
         case INT_CAL_MAX_BIAS_FREQCOMP:
         case INT_CAL_APT_FREQCOMP:
            bOK &= RFCal_Helper_ESC_ET_createINT_CAL_XPTFreqCompSwp(demo_param, cal_param, sweep_param, Sweep, Sweep[iSweepIndex],TE_Records);
            break;
         case RX_RSB_CAL: // RX_RSB_CAL = 112
            bOK &= RFCal_Helper_ESC_ET_createRxRSBCalSwp(demo_param, cal_param, sweep_param, Sweep[iSweepIndex],TE_Records);
            break;
         case XPT_PIN_CAL: // XPT_PIN_CAL = 15
            RFCal_Helper_ESC_ET_createXPTPinCalSwp(demo_param, cal_param, sweep_param, Sweep[Swp2], Sweep[iSweepIndex],TE_Records);
            break;

         default:
            throw RFCal_Helper_Exception("Unsupported sweep type.", __FILE__, __LINE__); // Not supported
            break;
      }
      //*********************************************//
      //        Pre Computation before sweep execution
      //*********************************************//


      //*********************************************//
      //    Adjust TE records
      //*********************************************//
        TE_Records.resize(Sweep[iSweepIndex].size());
        RFCal_Helper_Config_TE_ULChannels(Sweep[iSweepIndex], TE_Records);

      //*********************************************//
      //      Pre Computation before sweep execution
      //*********************************************//
      RFCal_Helper_ESC_ET_PreComputation(demo_param,cal_param,sweep_param,Sweep,TE_Records, iSweepIndex);
#ifdef ON_EQUIPMENT
      //*********************************************//
      //    Config for equipment
      //*********************************************//
#endif

      //*********************************************//
      //    Execute Sweep
      //*********************************************//
        if (bSweepMode)
      {
            if (!demo_param.bSimulationMode)
            {
         bool bIsLastSweep = static_cast<byte>( (iSweepIndex == (cal_param.Swp.size() - 1))? true:false );
            if (demo_param.bUseTearDownSweep)
            {
               bIsLastSweep = false;
            }
            RFCal_Helper_ExecEscSweep(hPhone, 
                Sweep[iSweepIndex] /* Array of Sweep records */,  
                Sweep[iSweepIndex].size() /* Size of Sweep records */, 
                bIsLastSweep /* is Last Sweep. */, 
                cal_param.En_Compress_Esc_Req /* Compress diag request or not */, 
                cal_param.En_Debug_Mode /* Debug mode */);
      }
        }
        else  // Non Sweep Mode
        {
            RFCal_Helper_ESC_Execute_Serial_Rx(demo_param,cal_param,sweep_param,Sweep[iSweepIndex], TE_Records);
        }
#ifdef ON_EQUIPMENT
      //*********************************************//
      //    Fetch results. Start measure and fetch results here
      //*********************************************//
#endif

      //*********************************************//
      //    Get Results
      //*********************************************//
      if (!demo_param.bSimulationMode)
      {
            if (bSweepMode)
            {
            RFCal_Helper_GetEscSweepResults(hPhone,Sweep[iSweepIndex], cal_param.En_Compress_Esc_Res, sweep_param.Enable_Box_Measurements);
      }
            else // Non Sweep Mode
            {

            }
        }
      //*********************************************//
      //    Dummy Result assignment (this is demo purpose)
      //*********************************************//
    //  if (demo_param.bSimulationMode && demo_param.bTESimulationMode) //QlibDemo Executes Simulation demo
        {
            RFCal_Helper_ESC_ET_Dummy_Result_Assignment(demo_param, cal_param, cal_param.Swp[iSweepIndex], Sweep[iSweepIndex]);
        }

      //*********************************************//
      //    Post computation
      //*********************************************//
         bOK &= RFCal_Helper_ESC_ET_PostComputation(demo_param, cal_param, cal_param.Swp[iSweepIndex], Sweep[iSweepIndex], TE_Records);

      //*********************************************//
      //    Logging (Console and QMSL)
      //*********************************************//
      bOK &= RFCal_Helper_ESC_ET_Logging(demo_param,  cal_param,  cal_param.Swp[iSweepIndex], Sweep[iSweepIndex]);

      //*********************************************//
      //    Write a debug CSV file
      //*********************************************//
      stringstream stCSVFilename;
      stCSVFilename << demo_param.stQMSLLogFileName << "-Sweep-" << (iSweepIndex+1) << ".csv";

        ExportSweepRecordToCSV(&Sweep[iSweepIndex][0],&TE_Records[0],stCSVFilename.str().c_str(),Sweep[iSweepIndex].size());
      if (!bOK)
      {
        //  throw RFCal_Helper_Exception("Unexpected error occurred", __FILE__, __LINE__); 
      }

      if (demo_param.xmlDoc)
      {
      demo_param.xmlDoc->save((_bstr_t)demo_param.stXMLFileName.c_str());
      }
 //     RFCal_Helper_ExecuteXSLT(demo_param.stXMLFileName,demo_param.stHTMLFileName,demo_param.stXSLFileName);
   }
   //*********************************************//
   //       Set Path Override for Split band
   //*********************************************//
   if (demo_param.bUseTearDownSweep)
   {
      RFCal_Helper_Execute_TearDown_Sweep(hPhone, demo_param,cal_param);
   }
    //*********************************************//
    //      Set Path Override for Split band
    //*********************************************//
    if (demo_param.bUseSET_PATH_SEL_OVERRIDE && demo_param.BAND == 41)
    {
        byte CarrierID = 0, band = 76, iTx = 255, iPRx = 255, iDRx = 255;
        QLIB_FTM_SET_PATH_SEL_OVERRIDE_INDEX(hPhone, CarrierID, band, iTx, iPRx, iDRx);
        CarrierID = 1;
        QLIB_FTM_SET_PATH_SEL_OVERRIDE_INDEX(hPhone, CarrierID, band, iTx, iPRx, iDRx);
    }

    //*********************************************//
   //    NV Generation (NVs are cached by NV manager)
   //*********************************************//
   bOK &= RFCal_Helper_ESC_ET_NV_Generation(demo_param, cal_param, Sweep);

   //*********************************************//
   //    Write NV to a QCN
   //*********************************************//
   if (bOK)
   {
      string qcn_filename = demo_param.stQMSLLogFileName;
      qcn_filename = qcn_filename.substr(0,qcn_filename.length()-4);// just remove ".txt" part
      bOK &= RFCal_Helper_WriteNVToQCN(hPhone,qcn_filename.append(".qcn"));
   }

   dword iEndTime = GetTickCount();
   time_t t_EndTime = time( NULL );
   RFCal_Helper_SetTime_Duration(demo_param.xmlDoc,(MSXML2::IXMLDOMElementPtr)NULL, (iEndTime - iStartTime),t_StartTime,t_EndTime);
   RFCal_Helper_SetTime_Duration(demo_param.xmlDoc,demo_param.TestPtr, (iEndTime - iStartTime),t_StartTime,t_EndTime);
   RFCal_Helper_SetRunInfo(demo_param.xmlDoc,demo_param.TestPtr, demo_param,(iEndTime - iStartTime),t_StartTime,t_EndTime);

   demo_param.xmlDoc->save((_bstr_t)demo_param.stXMLFileName.c_str());
   RFCal_Helper_ExecuteXSLT(demo_param.stXMLFileName,demo_param.stHTMLFileName,demo_param.stXSLFileName);

   //*********************************************//
   //    Stop QMSL logging
   //*********************************************//
    QLIB_FTM_SET_CALIBRATION_STATE(hPhone, false); 
    if (demo_param.bEnableDLFLogging)
    {
        QLIB_StopDLF_Logging(hPhone);
    }
    if (demo_param.bEnableQMSLLogging)
    {
        QLIB_StopLogging(hPhone);
    }
   return true;
}

void RFCal_Helper_Demo_MultiDUT_Cal(vector < QLIB_Demo_RFCAL_ESC_ET_Param > & demo_param)
{
   int NUM_THREAD = demo_param.size();

   vector <HANDLE> myhandle;
   vector <int>   ThreadID;
   myhandle.resize(NUM_THREAD);
   ThreadID.resize(NUM_THREAD);

   HANDLE iEvent;
   iEvent = CreateEvent(NULL, TRUE, FALSE, (LPCSTR)("ESC ET Cal demo"));
   InitializeCriticalSection(&cs);
   for (int i=0; i<NUM_THREAD; i++)
   {
      myhandle[i] = (HANDLE)_beginthreadex(NULL,0,RFCal_Helper_ESC_ET_threadFunction,(void*)&demo_param[i],0,NULL);
   }
   WaitForMultipleObjects(NUM_THREAD, &myhandle[0], TRUE, INFINITE);

   for (int i=0; i<NUM_THREAD; i++)
   {
      CloseHandle(myhandle[i]);
   }

   CloseHandle(iEvent); // Terminate the event
   DeleteCriticalSection(&cs);
}
unsigned char RFCal_Helper_ForcedCrash(HANDLE hPhone)
{
    unsigned char request[] = {75,37,03,00};
    unsigned char response[4] = {}; 
    short piResponseSize = 4;
    ::QLIB_SendSync(hPhone,4,request,&piResponseSize,response,2000);
    return true;
}
unsigned __stdcall RFCal_Helper_ESC_ET_threadFunction(void* parameters)
{
    QLIB_Demo_RFCAL_ESC_ET_Param * demo_param = (QLIB_Demo_RFCAL_ESC_ET_Param *)parameters;
    HANDLE hPhone = demo_param->hPhone;
    
   try
   {
        Test_RFCal_ESC_ET(*demo_param);
   }
   catch (char * e)
   {
      stringstream ss;
      ss << "Exception caught: " << e;
      RFCal_Helper_AddMessageToQMSLLog(hPhone,ss.str());
      RFCal_Helper_ExecuteXSLT(demo_param->stXMLFileName,demo_param->stHTMLFileName,demo_param->stXSLFileName);
   }
   catch (RFCal_Helper_Exception & e)
   {
      stringstream ss;
      ss << "Exception caught: " << e.what();
      RFCal_Helper_AddMessageToQMSLLog(hPhone,ss.str());
        QLIB_StopDLF_Logging(hPhone);
        QLIB_StopLogging(hPhone);
    //  RFCal_Helper_ForcedCrash(hPhone);
       RFCal_Helper_ExecuteXSLT(demo_param->stXMLFileName,demo_param->stHTMLFileName,demo_param->stXSLFileName);
        exit(0);

   }
   catch (...)
   {
      RFCal_Helper_AddMessageToQMSLLog(hPhone,"Unknown Exception caught." );
   }
   _endthreadex(0);
   return 0;
}
//}

void RFCal_Helper_ESC_ET_Initialize_Sweep(vector < cal_sweep_record >&     Sweep, const Sweep_Params & sweep_param, int NumRecords, int TECH, int BAND)
{
   Sweep.resize(NumRecords); // This is to avoid flagmentation

   for (UINT i = 0;  i < Sweep.size() ;   i++)
   {
      memset(&Sweep[i],0,sizeof(cal_sweep_record));

      Test_RFCal_ESC_InitRecord(&Sweep[i],
                                sweep_param.Sweep_Type, 0 /* iSweepSubType[i] */, 0 /*   iTxPurpose[i]*/,
                                TECH, BAND,
                                sweep_param.Waveform.Waveform_Type,
                                sweep_param.Ref_Channel,
                                sweep_param.Segment_Length[0][0],
                                sweep_param.Waveform.Start_RB,
                                sweep_param.Waveform.Number_Of_RBs,
                                sweep_param.Waveform.Tx_BW,
                                sweep_param.Waveform.Rx_BW
                               );
   }
}
unsigned char RFCal_Helper_ESC_ET_TX_NV_Generation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, vector < vector < cal_sweep_record > > &   Sweep)
{
   unsigned char bOK;
   HANDLE hPhone = demo_param.hPhone;

   //****************************************************************//
    //  Create Tx sweep records from all the sweep records
   //****************************************************************//
    vector <cal_sweep_record > TxRecords;
    UINT i=0;
   vector<Sweep_Params>::const_iterator it;
    unsigned char bMeasTx = false;
   UINT iEstimatedSegNum = 0;
    for ( it = cal_param.Swp.begin(); it != cal_param.Swp.end(); it++, i++)
   {
      if (it->Meas_Tx)
      {
         bMeasTx = true;
      }
      if ((it->Sweep_Type == REF_CAL || it->Sweep_Type == COMBINED_IQ_AGC_REF_CAL)
         && (!it->Do_FBGain_Cal))
      {
         bMeasTx = false;
         break;
      }
      iEstimatedSegNum += Sweep[i].size();
   }
   TxRecords.reserve(iEstimatedSegNum); // Important for performance
   i = 0;
   for ( it = cal_param.Swp.begin(); it != cal_param.Swp.end(); it++, i++)
    {
        if (it->Meas_Tx)
        {
            bMeasTx = true;
        }
        if ((it->Sweep_Type == REF_CAL || it->Sweep_Type == COMBINED_IQ_AGC_REF_CAL)
            && (!it->Do_FBGain_Cal))
        {
            bMeasTx = false;
            break;
        }
        copy(Sweep[i].begin(),Sweep[i].end(),back_inserter(TxRecords));
    }
   //****************************************************************//
    //  Stop Tx NV generation if there is no Tx measurement 
   //****************************************************************//
    if ( !bMeasTx ) 
    { 
        return true;
   }

   //****************************************************************//
    //  Assign Tx NV related parameters
   //****************************************************************//

      QMSL_RFCAL_ESC_HDET_NV_Params HDETNVMeas;
      QMSL_RFCAL_ESC_HDET_NV_Results HDETNVRes;
      QMSL_RFCAL_ESC_Calibration_NV escCalNv;
      QMSL_RFCAL_Tx_LimVsFreq_NV_Results LimFcompNv;

      memset(&escCalNv, 0, sizeof(QMSL_RFCAL_ESC_Calibration_NV));
      memset(&LimFcompNv,  0, sizeof(QMSL_RFCAL_Tx_LimVsFreq_NV_Results));
      memset(&HDETNVRes,   0, sizeof(QMSL_RFCAL_ESC_HDET_NV_Results));
      memset(&HDETNVMeas,  0, sizeof(QMSL_RFCAL_ESC_HDET_NV_Params));

      // Assume HDET measurement is sweep 1.
      for (UINT  i =0; i < cal_param.Swp[0].HDET_Avgs.size(); i++)
      {
         if(cal_param.Swp[0].HDET_Avgs[i] > 0)
         {
            HDETNVMeas.HDETPAState  = cal_param.Swp[0].PA_State_List[i];      // Assign PA gain state which measures HDET.
            HDETNVMeas.HDETEnabled  = true;
         }
      }
      // HDET Max limit
      HDETNVMeas.HDETMaxLim = cal_param.NV_Generation_Params.Max_HDET_Value;//4095;

      // Fill in the HDET Offset and Span Params
      HDETNVMeas.HDETSpanPower_dBm  = cal_param.NV_Generation_Params.Max_HDET_Pwr;
      HDETNVMeas.HDETOffsetPower_dBm   = cal_param.NV_Generation_Params.Min_HDET_Pwr;

      if( HDETNVMeas.HDETLPMEnabled )
      {
         HDETNVMeas.LPMHDETSpanPower_dBm = cal_param.NV_Generation_Params.Max_LPM_HDET_Pwr;
         HDETNVMeas.LPMHDETOffsetPower_dBm = cal_param.NV_Generation_Params.Min_LPM_HDET_Pwr;
         HDETNVMeas.LPMHDETStartPower_dBm = cal_param.NV_Generation_Params.LPM_HDET_Start_Pwr;
         HDETNVMeas.LPMHDETThresholdPower_dBm = 17.5;
      }

      // Figure out what type of HDET NV to use
      if (demo_param.TECH == 0)
      {
         // CDMA
         HDETNVMeas.useHDETV2 = static_cast<unsigned char>(1);
         HDETNVMeas.useExtendedHDETNV = static_cast<unsigned char>(1);
      }
      else if (demo_param.TECH == 1)
      {
         // WCDMA
         HDETNVMeas.useHDETV2 = static_cast<unsigned char>(1);
      }
      else
      {
         // LTE or TDSCDMA
         HDETNVMeas.useHDETV2 = static_cast<unsigned char>(0);
      }

   //****************************************************************//
   //   NV Path
   //****************************************************************//
   unsigned short iNVPath = 0;
   if (!cal_param.Swp[0].Device_Selection.NV_Device_To_Chain_Map.empty() && cal_param.Swp[0].Device_Selection.NV_Device_To_Chain_Map.size() >= 2)
   {
      byte txDeviceID = (cal_param.Swp.begin()->Device_Selection.Tx_Device_To_Calibrate.empty())?0 : cal_param.Swp.begin()->Device_Selection.Tx_Device_To_Calibrate.front();
      map <byte, byte>::const_iterator devToChainIt = cal_param.Swp[0].Device_Selection.NV_Device_To_Chain_Map.find(txDeviceID);
      if (devToChainIt != cal_param.Swp[0].Device_Selection.NV_Device_To_Chain_Map.end())
      {
         iNVPath = static_cast<unsigned short>(devToChainIt->second);
      }
   }


      //****************************************************************//
      // Tx NV Generation
      //****************************************************************//
      bOK=QLIB_RFCAL_Generate_ESC_Tx_NV(  hPhone,
        cal_param.NV_Generation_Params.NV_Mode,
      iNVPath, 
        TxRecords.size(),
        static_cast<word>(cal_param.Swp[0].Ref_Channel),
        static_cast<word>(cal_param.Swp[0].Upper_Bound_Channel),
                                          (void*)&HDETNVMeas,
                                          (void*)&HDETNVRes,
                                          (void*)&LimFcompNv,
        (void*)&TxRecords[0], 
                                          (void*)&escCalNv,
      true /* (unsigned char)cal_param.NV_Generation_Params.NV_Write_Action_Flag */ ); // NV_Write_Action_Flag must be true

    return true;
   }
unsigned char RFCal_Helper_ESC_ET_Process_RX_NV(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, vector < vector < cal_sweep_record > > &  Sweep)
{
    unsigned char bOK;
    HANDLE hPhone = demo_param.hPhone;

   //****************************************************************//
    //  Create Rx sweep records from all the sweep records
   //****************************************************************//

   bool measCA = false;
  // bool writeUnifiedRxNV = false;
   bool intraCAcalibrated = false;
   bool hasMultiplePowerModes = false;
    bool bMultiband = false;

   vector < cal_sweep_record > fcompSwpRecords;
   vector < cal_sweep_record > dualCarrFcompSwpRecords;
   vector < cal_sweep_record > threeCarrFcompSwpRecords;
    UINT i=0;

    unsigned char bIntraCAcalibrated = false;
    vector<Sweep_Params>::const_iterator it;
    vector<Sweep_Params>::const_iterator FreqComp_it;
    UINT freqcompIndex = -1;
    int index = 0;

    //****************************************************************//
    //  Create Rx sweep records (pick up only Rx related Sweep)
    //****************************************************************//

    for ( it = cal_param.Swp.begin(); it != cal_param.Swp.end(); it++, index++)
    {
        //****************************************************************//
        //  Check Rx Sweep Type
        //****************************************************************//

        if ((  it->Sweep_Type  == XPT_FREQCOMP 
            || it->Sweep_Type  == APT_FREQCOMP
            || it->Sweep_Type  == MAX_BIAS_FREQCOMP

            || it->Sweep_Type  == APT_FREQCOMP_STAGGER_RXTX
            || it->Sweep_Type  == XPT_FREQCOMP_STAGGER_RXTX
            || it->Sweep_Type  == REF_CAL

            
            || it->Sweep_Type  == MULTI_BAND_RX_CHARACTERIZED_CAL   
            || it->Sweep_Type  == MULTI_TECH_MULTI_BAND_REF_CAL
            || it->Sweep_Type  == XPT_FREQCOMP_SERIAL_RX
            || it->Sweep_Type  == INTRACA_FCOMP
            )
            && (it->Meas_Rx))

        {
            //If technology is CDMA first check if power mode 1 exists
            //If there is no power mode 1 then populate records for power mode 1 by copying them over from power mode 0.This is done only for CDMA
            if( demo_param.TECH == TECH_CDMA )
            {
               hasMultiplePowerModes = (it->Rx_PM_List.size() == 1)? 0 : 1;
            }
            if (it->Sweep_Type == INTRACA_FCOMP)
            {
                bIntraCAcalibrated = true;
            }
            else
            {
                FreqComp_it = it;
            }
            //****************************************************************//
            //  Check if it is Multiband cal or not
            //****************************************************************//

            if ((it->Sweep_Type == MULTI_TECH_MULTI_BAND_REF_CAL)
              ||(it->Sweep_Type == MULTI_BAND_RX_CHARACTERIZED_CAL)
              ||(it->Sweep_Type == MULTI_BAND_RX_CHARACTERIZED_INTRA_CAL)
              )
            {
                bMultiband = true;
            }
            
            //****************************************************************//
            //  Add Rx Sweep Records
            //****************************************************************//

            vector<cal_sweep_record>::const_iterator seg_itr;

            //Get fcomp measurement records
            unsigned short deviceInd = 0;
            byte deviceMask = 0x1;

            //Fetching the correct device records
            for( deviceInd = 0; deviceInd < ESC_MAX_DEVICES; ++deviceInd )
            {
               if( cal_param.RxDeviceFlag & (deviceMask << deviceInd) )
               {
                  break;
               }
            }

            for(seg_itr = Sweep[index].begin(); seg_itr < Sweep[index].end(); ++seg_itr )
            {
                if(  seg_itr->measSeg && seg_itr->rxParams.rxActions != FTM_TX_RX_FREQ_RX_ACTION_NOTHING )
                {
                    switch (seg_itr->rxMeas.actions[deviceInd].rx_meas_action)
                    {
                    case 2: // DC Cal
                        dualCarrFcompSwpRecords.push_back( *seg_itr );
                        break;
                    case 8: // 3C Cal
                        threeCarrFcompSwpRecords.push_back( *seg_itr );
                        break;
                    default:
                        fcompSwpRecords.push_back( *seg_itr );

                        // Insert PM1 for CDMA in case it is not present
                        if(demo_param.TECH == TECH_CDMA && hasMultiplePowerModes == false)
                        {
                            cal_sweep_record temp_record = *seg_itr;
                            temp_record.rxParams.rxPM = 1;
            for(int deviceInd = 0; deviceInd < ESC_MAX_DEVICES; ++deviceInd)
            {
                                temp_record.rxMeas.actions[deviceInd].payloads[0].power_mode = 1;
   }

                            fcompSwpRecords.push_back( temp_record );
                        }
                        break;
                    } // switch (seg_itr->rxMeas.actions[deviceInd].rx_meas_action)
                } // if(  seg_itr->measSeg && seg_itr->rxParams.rxActions != FTM_TX_RX_FREQ_RX_ACTION_NOTHING )
            } // for(seg_itr = Sweep[index].begin(); seg_itr < Sweep[index].end(); ++seg_itr )
        } // if (bMeasRx)
    } //for ( it = cal_param.Swp.begin(); it != cal_param.Swp.end(); it++, index++)
    //****************************************************************//
    //  Stop Rx NV generation if there is no Rx measurement 
    //****************************************************************//
    if ( fcompSwpRecords.empty() && dualCarrFcompSwpRecords.empty() && threeCarrFcompSwpRecords.empty() )
   {
        return true;
    }


      static QMSL_CDMA_Rx_DVGA_LNA_Cal_NV_Result rxCalNV;
      memset(&rxCalNV,0,sizeof(QMSL_CDMA_Rx_DVGA_LNA_Cal_NV_Result));
      static QMSL_CDMA_NVChannels_struct nvChannels;
      memset(&nvChannels,0,sizeof(QMSL_CDMA_NVChannels_struct));
      static QMSL_LTE_Rx_IntraCA_Cal_NV_Result intraCAnv;
      memset(&intraCAnv, 0, sizeof(QMSL_LTE_Rx_IntraCA_Cal_NV_Result) );

      for( UINT ch_index = 0; ch_index < FreqComp_it->Tx_Channel_List.size()  ; ++ch_index)
      {
         nvChannels.aiNVChannels[ ch_index ] = (unsigned short)RFCal_Helper_ConvertULchtoDLch(demo_param.TECH, demo_param.BAND, FreqComp_it->Tx_Channel_List[ch_index]);
      }

      byte deviceMask = 0x1;
      byte RxDeviceFlag = cal_param.RxDeviceFlag;
    if (FreqComp_it->Measure_CA || bIntraCAcalibrated )
      {
         RxDeviceFlag += 12;
      }

      // Debug Purpose only
      // ExportSweepRecordToCSV(&TotalRecords[0],"c:\\temp\\debug\\All_SweepRecords.csv",TotalRecords.size());


      //****************************************************************//
      // Rx NV Generaion
      //****************************************************************//

      for( unsigned short deviceInd = 0; deviceInd < ESC_MAX_DEVICES; deviceInd++ )
      {
         if( RxDeviceFlag & (deviceMask << deviceInd) )                       // if there are primary and diversity, execute the QLIB function twice
         {
            if (bIntraCAcalibrated)
            {
               bOK=QLIB_RFCAL_Enh_Generate_ESC_Rx_NV(  hPhone,
                    cal_param.NV_Generation_Params.NV_Mode,
                        deviceInd,  // 0 is primary and 1 is diverisity,  2: SCELL Primary Rx = Path2  3: SCELL Diversity Rx = Path 3 
                    fcompSwpRecords.size(),
                                                       (unsigned short)RFCal_Helper_ConvertULchtoDLch(demo_param.TECH, demo_param.BAND, cal_param.Swp[0].Ref_Channel),
                    (void*)(&fcompSwpRecords[0]),
                                                       (void*)&rxCalNV,
                                                       (void*)&intraCAnv,
                                                       (void*)&nvChannels,
                                                       (unsigned char) cal_param.NV_Generation_Params.NV_Write_Action_Flag );
            }
            else
            {
                /***in most cases "NV_Device_To_Chain_Map" is like this: "0,0;1,1", or "2,2;3,3",
                  but in CDMA SV, it may be like: "2,2;3,0". So we need to handle this case here. */
               unsigned short nvChain = deviceInd;

               map<byte, byte>::const_iterator iter = FreqComp_it->Device_Selection.NV_Device_To_Chain_Map.find( static_cast<byte>(deviceInd) );
               if( iter != FreqComp_it->Device_Selection.NV_Device_To_Chain_Map.end() )
               {
                  nvChain = static_cast<unsigned short>( iter->second );
               }
               
               if( nvChain == deviceInd )
               {
               bOK=QLIB_RFCAL_Generate_ESC_Rx_NV(  hPhone,
                                                   cal_param.NV_Generation_Params.NV_Mode,
                                                   deviceInd,                                // 0 is primary and 1 is diverisity
                    fcompSwpRecords.size(),
                                                   (unsigned short)RFCal_Helper_ConvertULchtoDLch(demo_param.TECH, demo_param.BAND, cal_param.Swp[0].Ref_Channel),
                    (void*)(&fcompSwpRecords[0]),
                                                   (void*)&rxCalNV,
                                                   (void*)&nvChannels,
                                                   (unsigned char) cal_param.NV_Generation_Params.NV_Write_Action_Flag );
            }
               else
               {
                  bOK=QLIB_RFCAL_Generate_ESC_Rx_NV_WithChainMap(  hPhone,               
                  cal_param.NV_Generation_Params.NV_Mode,
                  deviceInd,   // 0 is primary and 1 is diverisity
                  nvChain,
                  fcompSwpRecords.size(),
                  (unsigned short)RFCal_Helper_ConvertULchtoDLch(demo_param.TECH, demo_param.BAND, cal_param.Swp[0].Ref_Channel) ,
                  (void*)(&fcompSwpRecords[0]),
                  (void*)&rxCalNV,
                  (void*)&nvChannels,
                  (unsigned char) cal_param.NV_Generation_Params.NV_Write_Action_Flag );
               }
            }
         }
      }

      //****************************************************************//
      // Rx NV Generaion (DC)
      //****************************************************************//

    if(!dualCarrFcompSwpRecords.empty())  // Generate WCDMA DC NVs
    {
        if (dualCarrFcompSwpRecords.size() != 4)
        {
            return false;
        }
            memset(&rxCalNV,0,sizeof(QMSL_CDMA_Rx_DVGA_LNA_Cal_NV_Result));
            memset(&nvChannels,0,sizeof(QMSL_CDMA_NVChannels_struct));
            nvChannels.aiNVChannels[0] = static_cast<word>(cal_param.Swp[0].Ref_Channel);
            deviceMask = 0x1;
            for( unsigned short deviceInd = 0; deviceInd < ESC_MAX_DEVICES; ++deviceInd )
            {
               if(  RxDeviceFlag  & (deviceMask << deviceInd) )
               {

                /***in most cases "NV_Device_To_Chain_Map" is like this: "0,0;1,1", or "2,2;3,3",
                  but in CDMA SV, it may be like: "2,2;3,0". So we need to handle this case here. */
               unsigned short nvChain = deviceInd;

               map<byte, byte>::const_iterator iter = FreqComp_it->Device_Selection.NV_Device_To_Chain_Map.find( static_cast<byte>(deviceInd) );
               if( iter != FreqComp_it->Device_Selection.NV_Device_To_Chain_Map.end() )
               {
                  nvChain = static_cast<unsigned short>( iter->second );
               }
                 
               if( nvChain == deviceInd )
               {
                  QLIB_RFCAL_Generate_ESC_Rx_NV(   hPhone,
                                                   cal_param.NV_Generation_Params.Dual_Carr_NV_Mode,
                                                   deviceInd,
                    dualCarrFcompSwpRecords.size(),  // the number of DC cal records
                                                   (unsigned short)RFCal_Helper_ConvertULchtoDLch(demo_param.TECH, demo_param.BAND, cal_param.Swp[0].Ref_Channel),
                    static_cast<void*>(&dualCarrFcompSwpRecords[0]),
                                                   static_cast<void*>(&rxCalNV),
                                                   static_cast<void*>(&nvChannels),
                                                   (unsigned char) cal_param.NV_Generation_Params.NV_Write_Action_Flag);
               }
               else
               {
               QLIB_RFCAL_Generate_ESC_Rx_NV_WithChainMap(   hPhone,
                  cal_param.NV_Generation_Params.Dual_Carr_NV_Mode,
                  deviceInd,
                  nvChain,
                  dualCarrFcompSwpRecords.size(),  // the number of DC cal records
                  (unsigned short)RFCal_Helper_ConvertULchtoDLch(demo_param.TECH, demo_param.BAND, cal_param.Swp[0].Ref_Channel),
                  static_cast<void*>(&dualCarrFcompSwpRecords[0]),
                  static_cast<void*>(&rxCalNV),
                  static_cast<void*>(&nvChannels),
                     (unsigned char) cal_param.NV_Generation_Params.NV_Write_Action_Flag);
               }
               }
            }
      } // end of if(demo_param.bWCDMA_DC_Enabled)

    return true;
}
unsigned char RFCal_Helper_ESC_ET_Process_Consolidated_RX_NV(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, vector < vector < cal_sweep_record > > & Sweep)
{
    unsigned char bOK = true;
    HANDLE hPhone = demo_param.hPhone;


    //****************************************************************//
    //  Assign all NV Generation related params
    //****************************************************************//

    rxNVgenParam nvParam;
    const unsigned short NVversion = 1;
    nvParam.version = NVversion;
    QMSL_RFNV_RX_CAL_NV rxNV;
    for (size_t rowindex = 0; rowindex < MAX_NUM_DEVICES; ++rowindex)
   {
      for(size_t colindex = 0; colindex < MAX_NUM_PATH; ++colindex)
      {
         nvParam.nvcontainer[rowindex][colindex] = DEFAULT_NV_CONTAINER_VALUE;
      }
   }

    //****************************************************************//
    //  Merge all sweeps records into one large Rx array
    //****************************************************************//
    vector <cal_sweep_record > RxRecords;
    UINT i= 0;
    vector<Sweep_Params>::const_iterator it;
   
   // Reserve memory for vector
   UINT numberSegment = 0;
   for (   it = cal_param.Swp.begin(), i = 0; i < Sweep.size(), it != cal_param.Swp.end(); it++, i++)
   {
      if ((it->Meas_Rx || it->Sweep_Type == RX_RSB_CAL) && it->Device_Selection.DevicePathPair_To_NVcontainer.size() > 0)
      //if (it->Device_Selection.DevicePathPair_To_NVcontainer.size() > 0)
      {
         numberSegment += Sweep[i].size();
      }
   }
   RxRecords.reserve(numberSegment);
   i=0;
    //Accumulate all Sweeps for which Rx measurements have been enabled.
    for (   it = cal_param.Swp.begin(), i = 0; i < Sweep.size(), it != cal_param.Swp.end(); it++, i++)
    {
        if ((it->Meas_Rx || it->Sweep_Type == RX_RSB_CAL) && it->Device_Selection.DevicePathPair_To_NVcontainer.size() > 0)
        //if (it->Device_Selection.DevicePathPair_To_NVcontainer.size() > 0)
        {
            copy(Sweep[i].begin(),Sweep[i].end(),back_inserter(RxRecords));
            //fills the NV container table
            for (UINT i = 0; i < it->Device_Selection.DevicePathPair_To_NVcontainer.size(); i++)
            {
                    
                if (it->Device_Selection.DevicePathPair_To_NVcontainer[i].size() == 3)
                {
                    byte iDevice = it->Device_Selection.DevicePathPair_To_NVcontainer[i][0];
                    byte iPath = it->Device_Selection.DevicePathPair_To_NVcontainer[i][1];
                    byte iNVContainer = it->Device_Selection.DevicePathPair_To_NVcontainer[i][2];

                    nvParam.nvcontainer[iDevice][iPath] = iNVContainer;
                }
                else
                {
                    throw RFCal_Helper_Exception("Invalid DevicePathPair_To_NVcontainer parameters.",  __FILE__, __LINE__ );
                }

            }
        }
    }   
        
   nvParam.readNVdatafromDUT = (cal_param.NV_Generation_Params.Read_Consolidated_Rx_NV_From_DUT)? true : false;
   if (RxRecords.size() == 0) 
   {
      return false;
   }
    //****************************************************************//
    // Generate Consolidated Rx NV
    //****************************************************************//
        bOK = QLIB_RFCAL_Generate_ESC_Consolidated_Rx_NV( hPhone,
            cal_param.NV_Generation_Params.NV_Mode, 
            &(RxRecords[0]), 
            static_cast<unsigned short>(RxRecords.size()), 
            &(rxNV), 
            &(nvParam), 
            true);
   
    return bOK;
}


unsigned char RFCal_Helper_ESC_ET_RX_NV_Generation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, vector < vector < cal_sweep_record > > &   Sweep)
{
    unsigned char bOK = false;
    
    if (cal_param.NV_Generation_Params.Use_Consolidated_Rx_NV)
    {
        //********************************************************************************************************//
        //  Consolidated NV Generation. New Rx NV container for 3DL CA and alternate PATH Rx Calibration
        //********************************************************************************************************//
        bOK = RFCal_Helper_ESC_ET_Process_Consolidated_RX_NV(demo_param,cal_param, Sweep);
    } 
    else 
    {
        bOK = RFCal_Helper_ESC_ET_Process_RX_NV(demo_param,cal_param, Sweep);
    }

    return bOK;
}

unsigned char RFCal_Helper_ESC_ET_NV_Generation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, vector < vector < cal_sweep_record > > &  Sweep)
{
    //****************************************************************//
    //  Tx NV Generation
    //****************************************************************//
    RFCal_Helper_ESC_ET_TX_NV_Generation(demo_param, cal_param, Sweep);

    //****************************************************************//
    //  Rx NV Generation
    //****************************************************************//
    RFCal_Helper_ESC_ET_RX_NV_Generation(demo_param, cal_param, Sweep);

    return true;
}
unsigned char RFCal_Helper_ESC_ET_createXPTPinCalSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, Sweep_Params & sweep_param,
     const vector < cal_sweep_record > & Sweep2, vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
unsigned char   bOK             = true;
    UINT            recordIndex     = 0;
    int txParamMask_PwrOverride  = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN |
    ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT ); // Sweep 
    int             Feedback_Device_ID          =  static_cast<int>(sweep_param.Device_Selection.Feedback_Device_ID);   

   UINT dev = 0;
   if (cal_param.Tx_Rx_Split)
   {
      dev = static_cast<UINT>(sweep_param.Device_Selection.Master_Device_ID);
   }
   else
   {
      dev = static_cast<UINT>(RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag));
   }

    //****************************************************************//
    //  Update the target compression for EPT and ET from PA params
    //****************************************************************//
    sweep_param.XPT_PinCal_Swp_TargetCompList_dB10.resize(sweep_param.XPT_PinCal_Swp_XPT_Action_List.size(), 30);
    for (UINT i=0; i < sweep_param.XPT_PinCal_Swp_XPT_Action_List.size(); i++)
    {
        if (sweep_param.XPT_PinCal_Swp_XPT_Action_List[i] == XPT_TYPE_EPT)
        {
            sweep_param.XPT_PinCal_Swp_TargetCompList_dB10[i] = static_cast<int>(cal_param.PA_Param.CalCompPtEpt*10);
        }
        else if (sweep_param.XPT_PinCal_Swp_XPT_Action_List[i] == XPT_TYPE_ET)
        {
            sweep_param.XPT_PinCal_Swp_TargetCompList_dB10[i] = static_cast<int>(cal_param.PA_Param.CalCompPtEt*10);
        }
    }
    
    //****************************************************************//
    //  Post Computation
    //  Please make sure "get RGI and bias value from Sweep 2"
    //  RFCal_Helper_ESC_ET_PostComputation_Swp2ForPinCal
    //****************************************************************//

    //****************************************************************//
    //Pin Cal involves the following steps:
    //1. Load Unity DPD Tables
    //2. XPT Config - Needed to setup FB path
    //3. Pin IQ Capture - To get AMAM curve needed for compression point calculation and also to compute the Pin back off needed to achieve 3dB compression.
    //4. Pin Proc Segment - To fetch the computed pin back off value from phone to PC
    //****************************************************************//
    
    
    //****************************************************************//
    //  Add Load Unity DPD segment
    //****************************************************************//
    RFCal_Helper_setOpCodeData(Sweep[recordIndex], cal_param, sweep_param, DATA_OPCODE_ACTION_LOAD_DPD /* OpCode Data Option */, demo_param.TECH, demo_param.BAND);
    Sweep[recordIndex].data.nomDPDTbl.xptType       = LOAD_DPD_XPT_TYPE_EPT;
    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
    
    
    //PA State Loop(Pin Cal is performed in EPT mode irrespective of EPT or ET PA State)
    for (UINT paLpVar = 0; paLpVar < sweep_param.XPT_PinCal_Swp_PA_State_List.size(); ++paLpVar)
    {
        // Channel loop
        for (UINT chanIndex  = 0; chanIndex  < sweep_param.XPT_PinCal_Swp_Channel_List.size();  chanIndex ++)
        {
            //****************************************************************//
            //  OpCode 0x100: Radio Setup
            //****************************************************************//        
            int masterDevState = MASTER_RADIO_STATE_RETUNE;
            int slaveDevState = 0;
            if (cal_param.RxDeviceFlag & 2) // if there is diversity
            {
                slaveDevState = (paLpVar == 0 && chanIndex == 0 )?SLAVE_RADIO_STATE_RX_DIV_STATE:SLAVE_RADIO_STATE_RETUNE;  
            }

            
            RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], 
                sweep_param.Device_Selection.Master_Device_ID ,
                sweep_param.Device_Selection.Slave_Device_ID,
                masterDevState, 
                slaveDevState, 
                demo_param.TECH,
                demo_param.BAND, 
                (dword) sweep_param.XPT_PinCal_Swp_Channel_List[chanIndex] /* ulChan */,
                sweep_param.Waveform.Waveform_Type, 
                NULL, 
                sweep_param.Waveform.Number_Of_RBs, 
                sweep_param.Waveform.Start_RB, 
                sweep_param.Waveform.EUL_Waveform_Type,
                sweep_param.Waveform.Rx_BW,
                sweep_param.Waveform.Tx_BW);

            Sweep[recordIndex].radio_cfg.chan.chanInfo[0].ulChan = sweep_param.XPT_PinCal_Swp_Channel_List[chanIndex];
            Sweep[recordIndex].radio_cfg.flag = 0;
        
            //****************************************************************//
            //  Insert NoP (OpCode 0x0)  
            //****************************************************************//
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
            dword config_time = sweep_param.Tuning_Segment_Length;
            RFCal_helper_InsertNop(Sweep,recordIndex, config_time, sweep_param.Segment_Length[0][0]);   

            //*****************************************************************************************/
            //      OpCode 0x2000: XPT2 XPT Config  ( 1 = Cal Config )
            //*****************************************************************************************/
         RFCal_Helper_setOpCode_XPT2(Sweep[recordIndex],  ESC_XPT_ACTION_CAL_CFG /* xptAction */, dev /* cfg_dev */, Feedback_Device_ID, XPT_TYPE_EPT); // Even ET only, use EPT
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

            //*****************************************************************************************/
            //      PIN capture (TxOverride  +  XPT2 IQ capture with 4 = PIN calibration IQ capture )
            //*****************************************************************************************/
            //*****************************************************************************************/
            //      OpCode 0x200: TxOverride 
            //*****************************************************************************************/
            Sweep[recordIndex].subSwpType                                       = ESC_SUB_SWEEP_MAX;
            
            byte rgi = sweep_param.XPT_PinCal_Swp_XPT_RGI_List[paLpVar];
            word bias = sweep_param.XPT_PinCal_Swp_XPT_Bias_List[paLpVar];

            RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],0, cal_param.TxDeviceFlag,
                sweep_param.XPT_PinCal_Swp_XPT_Action_List[paLpVar] /* XPT Mode */,
                ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */, 
                ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,
                txParamMask_PwrOverride /* pwrParamMask */, 
                sweep_param.XPT_PinCal_Swp_PA_State_List[paLpVar], rgi, bias, 
                ESC_TX_OVERRIDE_ACTION_USE_VAL, sweep_param.XPT_PinCal_Swp_IQ_Gain, 
                ESC_TX_OVERRIDE_ACTION_USE_VAL, sweep_param.XPT_PinCal_Swp_Env_Scale,   
                cal_param.PA_Param.IcqEt,ESC_TX_OVERRIDE_ACTION_USE_VAL, 0);

            //*****************************************************************************************/
            //      OpCode 0x2000: XPT2 IQ capture  with 4 = PIN calibration IQ capture )
            //*****************************************************************************************/
            Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_MAX;

         RFCal_Helper_setOpCode_XPT2(Sweep[recordIndex], ESC_XPT_ACTION_IQ_CAP /* xptAction */, dev /* cfg_dev */, Feedback_Device_ID, NULL, 
            ESC_XPT_CAP_PIN /* 4 = PIN calibration IQ capture */,
            sweep_param.XPT_PinCal_Swp_Num_IQ_Samples, 1,1,  ESC_XPT_CAP_PROC_SWP1 );
    
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
        }

        //****************************************************************//
        //  If Cap2ProcWaitTime is not zero, wait for mentioned duration before processing  
        //****************************************************************//
        if (sweep_param.XPT_PinCal_Swp_Cap2Proc_Wait_Time != 0)
        {
            Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;
            RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.XPT_PinCal_Swp_Cap2Proc_Wait_Time);
        //Sweep[recordIndex].opcode.segLen = static_cast<dword>(sweep_param.XPT_PinCal_Swp_Cap2Proc_Wait_Time);
        }
        //RFCal_helper_InsertNop(Sweep,recordIndex, 1); 
        //****************************************************************//
        //  Add Pin Proc Segment 
        //****************************************************************//
        for (UINT chanIndex  = 0; chanIndex  < sweep_param.XPT_PinCal_Swp_Channel_List.size();  chanIndex ++)
        {
            Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_PIN_PROC;
            Sweep[recordIndex].opcode.segLen = static_cast<word>(sweep_param.XPT_PinCal_Swp_Proc_Segment_Length);
            RFCal_Helper_setOpCodeData(Sweep[recordIndex], cal_param, sweep_param, DATA_OPCODE_ACTION_PIN_PROC /* OpCode Data Option */, demo_param.TECH, demo_param.BAND);
            Sweep[recordIndex].data.pinValProc.doPinValProcOp = true;
            Sweep[recordIndex].data.pinValProc.paState = sweep_param.XPT_PinCal_Swp_PA_State_List[paLpVar];
            Sweep[recordIndex].data.pinValProc.currChannel = sweep_param.XPT_PinCal_Swp_Channel_List[chanIndex];
            Sweep[recordIndex].data.pinValProc.targetComp_dB10 = sweep_param.XPT_PinCal_Swp_TargetCompList_dB10[paLpVar];
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
        }
    }

    //*********************************************//
    //      Generate OpCode for all the segment
    //*********************************************//
    Sweep.resize(recordIndex); // Delete unused records. 
    RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());
   return true;
}
unsigned char RFCal_Helper_ESC_ET_createXPTDelaySwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, Sweep_Params & sweep_param,
                                                    const vector < cal_sweep_record > & Sweep2, vector < cal_sweep_record > & Sweep)
{
   unsigned char  bOK            = true;
   UINT        recordIndex    = 0;
   int txParamMask_PwrOverride  = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN |
                                   ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT | ESC_TX_OVERRIDE_DELAY); // Sweep
    int             Feedback_Device_ID          = static_cast<int>(sweep_param.Device_Selection.Feedback_Device_ID);    

   UINT dev = 0;
   if (cal_param.Tx_Rx_Split)
   {
      dev = static_cast<UINT>(sweep_param.Device_Selection.Master_Device_ID);
   }
   else
   {
      dev = static_cast<UINT>(RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag));
   }
   //****************************************************************//
   // Find highest RGI in XPT
   //****************************************************************//
   int HighestRGI = 0;
   for (UINT iSegNum = 0 ; iSegNum < Sweep2.size(); iSegNum++)
   {
      if (Sweep2[iSegNum].data.dpdCalc.do_dpd_calc_op == true && Sweep2[iSegNum].data.dpdCalc.xptMode == 2) // Do Proc result DPD, response is AMAM / AMPM table
      {
         HighestRGI = Sweep2[iSegNum].dataRes.measDpdRes[cal_param.Swp[1].ET_Swp2_Meas_Pwr_List.size()-1].rgi;
      }
   }
   HighestRGI += sweep_param.XPT_Delay_Swp_RGI_Fudge_From_MLine;
   //****************************************************************//
   // Find previous sweep_type and check if CA or DC cal
   //****************************************************************//
   unsigned char bIsMasterDeviceChange = false;
   for (UINT i=0; i < cal_param.Swp.size(); i++)
   {
      if (cal_param.Swp[i].Sweep_Type == sweep_param.Sweep_Type)
      {
         int prev_sweep = i-1;
         if (prev_sweep >= 0
               && (cal_param.Swp[prev_sweep].Measure_CA == (unsigned char)true || cal_param.Swp[prev_sweep].Measure_DC == (unsigned char)true))
         {
            bIsMasterDeviceChange = true;
         }
      }
   }

   for (UINT chanIndex  = 0; chanIndex  < sweep_param.XPT_Delay_Swp_Channel_List.size();  chanIndex ++)
   {
      //****************************************************************//
      // OpCode 0x100: Radio Setup
      //****************************************************************//
      int masterDevState;
      if (chanIndex == 0 && bIsMasterDeviceChange)
      {
         masterDevState = MASTER_RADIO_STATE_RX_TX_STATE;
      }
      else
      {
         masterDevState = MASTER_RADIO_STATE_RETUNE;
      }

      int slaveDevState = (chanIndex == 0)?SLAVE_RADIO_STATE_NO_CHANGE:SLAVE_RADIO_STATE_RETUNE;
      RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex],
                                        sweep_param.Device_Selection.Master_Device_ID ,
                                        sweep_param.Device_Selection.Slave_Device_ID,
                                        masterDevState,
                                        slaveDevState,
                                        demo_param.TECH,
                                        demo_param.BAND,
                                        (dword) sweep_param.XPT_Delay_Swp_Channel_List[chanIndex] /* ulChan */,
                                        sweep_param.Waveform.Waveform_Type,
                                        NULL,
                                        sweep_param.Waveform.Number_Of_RBs,
                                        sweep_param.Waveform.Start_RB,
                                        sweep_param.Waveform.EUL_Waveform_Type,
                                        sweep_param.Waveform.Rx_BW,
                                        sweep_param.Waveform.Tx_BW);

      Sweep[recordIndex].radio_cfg.chan.chanInfo[0].ulChan = sweep_param.XPT_Delay_Swp_Channel_List[chanIndex];
      //Mentioning explicitely in case of a delay sweep. This is being added to avoid RxAgc to reduce the tuning segment duration.
      Sweep[recordIndex].radio_cfg.flag = 0;
      for (UINT i = 0; i < cal_param.Swp.size(); i++)
      {
         if (cal_param.Swp[i].Use_PilotOnlyWaveform)
         {
            Sweep[recordIndex].radio_cfg.flag = 1;
            break;
         }
      }
        if (sweep_param.Sweep_Type == XPT_DELAY)
        {
      Sweep[recordIndex].radio_cfg.flag += 2;
        }
      if (chanIndex == 0)
      {
         RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.Config_Segment_Length);
      }
      else
      {
         //****************************************************************//
         // Insert NoP (OpCode 0x0)
         //****************************************************************//
         RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
         dword config_time = (chanIndex == 0 && bIsMasterDeviceChange)?sweep_param.Config_Segment_Length: sweep_param.Tuning_Segment_Length;
         RFCal_helper_InsertNop(Sweep,recordIndex, config_time, sweep_param.Segment_Length[0][0]);
      }
      //*****************************************************************************************/
      //    OpCode 0x800: Load Unity DPD (only one time)
      //*****************************************************************************************/
      if (chanIndex == 0)
      {
         RFCal_Helper_setOpCodeData(Sweep[recordIndex], cal_param, sweep_param, DATA_OPCODE_ACTION_LOAD_DPD /* OpCode Data Option */, demo_param.TECH, demo_param.BAND);
         Sweep[recordIndex].data.nomDPDTbl.xptType    = LOAD_DPD_XPT_TYPE_ET;
         Sweep[recordIndex].data.refChan = sweep_param.XPT_Delay_Swp_Channel_List[chanIndex];
         RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
      }

      //*****************************************************************************************/
      //    OpCode 0x2000: XPT2 XPT Config
      //*****************************************************************************************/
      RFCal_Helper_setOpCode_XPT2(Sweep[recordIndex],  ESC_XPT_ACTION_CAL_CFG /* xptAction */, dev /* cfg_dev */, Feedback_Device_ID, XPT_TYPE_EPT); // Even ET only, use EPT
      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);

      //*****************************************************************************************/
      //    Delay capture (TxOverride  +  XPT2 IQ capture)
      //*****************************************************************************************/
      //*****************************************************************************************/
      //    OpCode 0x200: TxOverride
      //*****************************************************************************************/
      Sweep[recordIndex].subSwpType                            = ESC_SUB_SWEEP_MAX;

      RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],0, cal_param.TxDeviceFlag, XPT_TYPE_ET, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                        ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,        txParamMask_PwrOverride /* pwrParamMask */,
                                        sweep_param.XPT_Delay_Swp_PA_State, HighestRGI, (dword)sweep_param.XPT_Delay_Swp_Bias,
                                        ESC_TX_OVERRIDE_ACTION_USE_VAL, sweep_param.XPT_Delay_Swp_IQ_Gain,
                                        ESC_TX_OVERRIDE_ACTION_USE_VAL, sweep_param.XPT_Delay_Swp_Env_Scale,
                                        cal_param.PA_Param.IcqEt,ESC_TX_OVERRIDE_ACTION_USE_VAL, 0);

      //*****************************************************************************************/
      //    OpCode 0x2000: XPT2 IQ capture
      //*****************************************************************************************/
      Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_MAX;

      RFCal_Helper_setOpCode_XPT2(Sweep[recordIndex], ESC_XPT_ACTION_IQ_CAP /* xptAction */, dev /* cfg_dev */, Feedback_Device_ID, NULL, 
                                  ESC_XPT_CAP_DELAY,sweep_param.XPT_Delay_Swp_Num_IQ_Samples, 1,1,  ESC_XPT_CAP_PROC_SWP1 );

      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
   }
   //****************************************************************//
   // Insert NoP (OpCode 0x0)
   //****************************************************************//
   RFCal_helper_InsertNop(Sweep,recordIndex, 1);

   //*****************************************************************************************/
   //    OpCode 0x800: Delay Processing
   //*****************************************************************************************/
   for (UINT chanIndex  = 0; chanIndex  < sweep_param.XPT_Delay_Swp_Channel_List.size();  chanIndex ++)
   {
      RFCal_Helper_setOpCodeData(Sweep[recordIndex], cal_param, sweep_param, DATA_OPCODE_ACTION_DELAY_PROC /* OpCode Data Option */, demo_param.TECH, demo_param.BAND);
      Sweep[recordIndex].data.refChan = sweep_param.XPT_Delay_Swp_Channel_List[chanIndex];
      Sweep[recordIndex].data.delayValProc.upperBoundChan = sweep_param.XPT_Delay_Swp_Upper_Bound_Channel_List[chanIndex];
      if (demo_param.TECH == 3)
      {
         Sweep[recordIndex].data.delayValProc.fiveMHzOffset    = sweep_param.XPT_Delay_Swp_5MHz_Offset_List[chanIndex];
         Sweep[recordIndex].data.delayValProc.tenMHzOffset     = sweep_param.XPT_Delay_Swp_10MHz_Offset_List[chanIndex];
         Sweep[recordIndex].data.delayValProc.twentyMHzOffset  = sweep_param.XPT_Delay_Swp_20MHz_Offset_List[chanIndex];
      }
      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
   }
   //*****************************************************************************************/
   //    End of Delay Sweep: Insert a low rgi segment instead of tear down
   //*****************************************************************************************/
   RFCal_Helper_setLowRGISegment(Sweep[recordIndex],cal_param.TxDeviceFlag);
   RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);

   //*********************************************//
   //    Generate OpCode for all the segment
   //*********************************************//
   Sweep.resize(recordIndex); // Delete unused records.
   RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());
   return true;
}
unsigned char RFCal_Helper_ESC_ET_createIntraCAFreqCompSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param,
     vector < cal_sweep_record >&       Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
   UINT        recordIndex  = 0;
   int txParamMask_PwrOverride  = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN |
                                   ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT); // Sweep

   //****************************************************************//
   // Calcululate Rx Device flag
   //****************************************************************//

   byte RxDeviceFlag = 0;
   for (UINT devIndex = 0; devIndex < sweep_param.Device_Selection.Rx_Device_To_Calibrate.size(); devIndex++)
   {
      RxDeviceFlag |= (1 << sweep_param.Device_Selection.Rx_Device_To_Calibrate[devIndex] );
   }

   for (UINT chanIndex = 0 ; chanIndex <  sweep_param.Rx_Channel_List.size(); chanIndex++)
   {
      //****************************************************************//
      // OpCode 0x100: Radio Setup for PCell retune
      //****************************************************************//
      byte masterDeviceId = 0;
      byte slaveDeviceId = 1;
      byte masterDevState = (chanIndex == 0)?MASTER_RADIO_STATE_RETUNE:MASTER_RADIO_STATE_RX_TX_STATE;
      byte slaveDevState = (chanIndex == 0)?SLAVE_RADIO_STATE_RETUNE:SLAVE_RADIO_STATE_RX_DIV_STATE;

      RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], masterDeviceId,slaveDeviceId,
                                        masterDevState /* masterDevState */, slaveDevState /* slaveDevState */, demo_param.TECH, demo_param.BAND, (dword) sweep_param.Rx_Channel_List[chanIndex] /* ulChan */
                                        , sweep_param.Waveform.Waveform_Type, NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);

        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

      //****************************************************************//
      // Insert NoP (OpCode 0x0)
      //****************************************************************//
      RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);

      //****************************************************************//
      // OpCode 0x200: TxOverride  (for trigger)
      //****************************************************************//

      for (UINT paIndex = 0; paIndex < sweep_param.PA_State_List.size(); paIndex++)
      {
         for (UINT rgiIndex = 0; rgiIndex < sweep_param.RGI_List[paIndex].size(); rgiIndex++)
         {
            int txPurpose = -1;
            // Trigger segment
            RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], txPurpose, cal_param.TxDeviceFlag, XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                              ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,        txParamMask_PwrOverride /* pwrParamMask */,
                                              (byte)sweep_param.PA_State_List[paIndex],       (byte)sweep_param.RGI_List[paIndex][rgiIndex],
                                              (dword)sweep_param.Bias_List[paIndex][rgiIndex],   ESC_TX_OVERRIDE_ACTION_SW_DEF,0, ESC_TX_OVERRIDE_ACTION_SW_DEF,0,
                                              sweep_param.PA_Quiescent_Current[paIndex][rgiIndex],ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);
            Sweep[recordIndex].measSeg = false;
            Sweep[recordIndex].rxParams.rxFlag = FTM_TX_RX_FREQ_RX_ACTION_NOTHING;
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

            // Tx off segment
            RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], txPurpose, cal_param.TxDeviceFlag, XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */, 
            ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,         txParamMask_PwrOverride /* pwrParamMask */, 
            (byte)sweep_param.PA_State_List[paIndex],         (byte)0,   
            (dword)sweep_param.Bias_List[paIndex][rgiIndex],   ESC_TX_OVERRIDE_ACTION_SW_DEF,0, ESC_TX_OVERRIDE_ACTION_SW_DEF,0,
            sweep_param.PA_Quiescent_Current[paIndex][rgiIndex],ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);
            Sweep[recordIndex].measSeg = false;
            Sweep[recordIndex].rxParams.rxFlag = FTM_TX_RX_FREQ_RX_ACTION_NOTHING;
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         }
      }

      //****************************************************************//
      // OpCode 0x100: Radio Setup for SCell retune
      //****************************************************************//
      masterDeviceId = 2;
      slaveDeviceId = 3;

      masterDevState = MASTER_RADIO_STATE_RX_STATE;
      slaveDevState = SLAVE_RADIO_STATE_RX_DIV_STATE;
      dword scell_channel = (dword) sweep_param.Rx_Channel_List[chanIndex] + 10; // As of now, 10 is hard coded value, not from parameter XML.

      RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], masterDeviceId, slaveDeviceId,
                                        masterDevState /* masterDevState */, slaveDevState /* slaveDevState */, demo_param.TECH, demo_param.BAND, /* ulChan */ scell_channel
                                        , sweep_param.Waveform.Waveform_Type, NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);

        TE_Records[recordIndex].m_dl_lvl = static_cast<double>(static_cast<short>(sweep_param.DL_Level_List[0])); // To stabilize DL power

      for (UINT iIndex = 0 ; iIndex <4 ; iIndex ++)
      {
         Sweep[recordIndex].rxMeas.actions[iIndex].rx_meas_action = 1;
      }

        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

      //****************************************************************//
      // Insert NoP (OpCode 0x0)
      //****************************************************************//
      RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);

      //****************************************************************//
      //  PCell + SCell Rx measurement
      // OpCode 0x4020: RxConfig + Rx Meas
      //****************************************************************//
      for (UINT RxIndex = 0; RxIndex < sweep_param.Rx_Actions_List.size(); RxIndex++)
      {
         word expRxAGC = (word)CalculateRxAGCValue(demo_param.TECH,sweep_param.DL_Level_List[RxIndex]);
         RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], sweep_param.Rx_Actions_List[RxIndex],
                                                   RxDeviceFlag, 0,expRxAGC, 1);

         Sweep[recordIndex].radio_cfg.device_mapping.pcellmasterDev = sweep_param.Device_Selection.Pcell_Master_Device_ID;
         Sweep[recordIndex].radio_cfg.device_mapping.pcellslaveDev = sweep_param.Device_Selection.Pcell_Slave_Device_ID;
         Sweep[recordIndex].radio_cfg.device_mapping.scellmasterDev = sweep_param.Device_Selection.Scell_Master_Device_ID;
         Sweep[recordIndex].radio_cfg.device_mapping.scellslaveDev = sweep_param.Device_Selection.Scell_Slave_Device_ID;

         TE_Records[recordIndex].m_dl_lvl = static_cast<double>(static_cast<short>(sweep_param.DL_Level_List[RxIndex])); 
         RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
      }
   }

   //*********************************************//
   //    Generate OpCode for all the segment
   //*********************************************//
   Sweep.resize(recordIndex); // Delete unused vector.
   RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());
   return true;
}
unsigned char RFCal_Helper_isTDDBand(byte tech, byte band)
{
   /* LTE */
   if ((tech == 3) & ( band  >= 33 && band <= 41))
   {
      return true;
   }
   /* TDSCDMA */
   if (tech == 4 )
   {
      return true;
   }
   return false;
}
unsigned char RFCal_Helper_ESC_ET_createInterCA_SCellFreqCompSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, Sweep_Params & sweep_param,
    vector < cal_sweep_record >&        Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{

    return true;
}
unsigned char RFCal_Helper_ESC_ET_createXPTFreqCompSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, Sweep_Params & sweep_param,
    const vector < vector < cal_sweep_record > > & AllSweeps/* vector < cal_sweep_record >  APT_Sweep */, vector < cal_sweep_record >&      Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
   unsigned char  bOK            = true;
   UINT        recordIndex    = 0;
   int txParamMask_PwrOverride  = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN |
                                   ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT); // Sweep
   int numberOfSegmentsForOneChannel = 0; // this info is required for inter CA cal and DC cal
   //****************************************************************//
    //  Calcululate Rx Device flag
    //****************************************************************//
    byte RxDeviceFlag = 0;
    for (UINT devIndex = 0; devIndex < sweep_param.Device_Selection.Rx_Device_To_Calibrate.size(); devIndex++)
    {
        RxDeviceFlag |= (1 << sweep_param.Device_Selection.Rx_Device_To_Calibrate[devIndex] );
    }
    
   UINT dev = 0;
   if (cal_param.Tx_Rx_Split)
   {
      dev = static_cast<UINT>(sweep_param.Device_Selection.Master_Device_ID);
   }
   else
   {
      dev = static_cast<UINT>(RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag));
   }
    
    //****************************************************************//
   // PreComputation for FreqComp
   //****************************************************************//

    if ( sweep_param.Pre_Sweep_Computation.size() > 0 && sweep_param.Sweep_Type != XPT_FREQCOMP_SERIAL_RX )
   {
      //****************************************************************//
      // Find APT and Max Bias Sweep Records from Pre_Sweep_Computation parameters
      //****************************************************************//

      map < byte, vector < double > >  paToApt_Power;
      map < byte, vector < int > >     paToApt_RGI,   paToApt_Bias, paToApt_Icq;

      map < byte, vector < double > >  paToMBL_Power;
      map < byte, vector < int > >     paToMBL_RGI,   paToMBL_Bias,  paToMBL_Icq;

      unsigned char bUseMBLforLim = false; // True if pre comp type is 5

      // ********************************************************************* //
      //    Collect measured power rgi list from ATP sweep and MBL sweep (if applicable)
      // ********************************************************************* //

      for (UINT preCompIndex = 0; preCompIndex < sweep_param.Pre_Sweep_Computation.size(); preCompIndex++)
      {
         int pre_comp_type = sweep_param.Pre_Sweep_Computation[preCompIndex];
         int pre_sweep_index = sweep_param.Compute_From_Sweep[preCompIndex];

         if (pre_comp_type == 5)
         {
            bUseMBLforLim = true;
         }

         map <byte, UINT> paToIndex;
         for (UINT i = 0; i < cal_param.Swp[pre_sweep_index].PA_State_List.size(); i++)
         {
            paToIndex.insert(make_pair(cal_param.Swp[pre_sweep_index].PA_State_List[i],i));
         }
         vector < vector < double > > powerList;
         vector < vector < int > > rgiList, biasList, icqList;
         powerList.resize(paToIndex.size());
         rgiList.resize(paToIndex.size());
         biasList.resize(paToIndex.size());
         icqList.resize(paToIndex.size());

         for (UINT i = 0; i < AllSweeps[pre_sweep_index].size(); i++)
         {
            const cal_sweep_record * pTmp = & AllSweeps[pre_sweep_index][i];
            // only max bias segment
            if (pTmp->txParams.txParams[dev].pwrOverrideType == 1 && pTmp->txParams.txParams[dev].rgi > 0)  
            {
               //if (bUseMBLforLim && pTmp->txPurpose != 255)
               //{
               //   continue;
               //}
               byte pa = pTmp->txParams.txParams[dev].paState;

               powerList[paToIndex[pa]].push_back(static_cast<double>(static_cast<short>(pTmp->txRes.txPower))/10.0);
               rgiList[paToIndex[pa]].push_back(static_cast<int>(pTmp->txParams.txParams[dev].rgi)); 
               biasList[paToIndex[pa]].push_back(static_cast<int>(pTmp->txParams.txParams[dev].bias));
               icqList[paToIndex[pa]].push_back(static_cast<int>(pTmp->txParams.txParams[dev].quiescentCurrent)); 
            }
         }

         // ********************************************************************* //
         //    Store the collected into ATP and MBL maps (if applicable)
         // ********************************************************************* //

         if (pre_comp_type == 5)
         {
            for (map <byte, UINT>::iterator it = paToIndex.begin(); it != paToIndex.end(); it ++)
            {
               byte pa = (byte)it->first;
               paToMBL_Power[pa] = (vector < double >)powerList[paToIndex[pa]];
               paToMBL_RGI[pa] = (vector < int >)rgiList[paToIndex[pa]];
               paToMBL_Bias[pa] = (vector < int >)biasList[paToIndex[pa]];
               paToMBL_Icq[pa] = (vector < int >)icqList[paToIndex[pa]];
            }
         }
         else if  (pre_comp_type == 3)
         {
            for (map <byte, UINT>::iterator it = paToIndex.begin(); it != paToIndex.end(); it ++)
            {
               byte pa = (byte)it->first;
               paToApt_Power[pa] = (vector < double >)powerList[paToIndex[pa]];
               paToApt_RGI[pa] = (vector < int >)rgiList[paToIndex[pa]];
               paToApt_Bias[pa] = (vector < int >)biasList[paToIndex[pa]];
               paToApt_Icq[pa] = (vector < int >)icqList[paToIndex[pa]];
            }
         }
        else if  (pre_comp_type == 0)
        {
            // Do nothing
         }
         else
         {
            RFCal_Helper_AddMessageToQMSLLog(demo_param.hPhone," Unknown pre computation type");
         }
      }
      // ********************************************************************* //
      //    Assign closest value to the sweep parameters.
      // ********************************************************************* //

      for (UINT index=0; index < sweep_param.PA_State_List.size(); index++)
      {
         for (UINT rgiIndex = 0; rgiIndex < sweep_param.RGI_List[index].size(); rgiIndex++)
         {
            byte paState      = sweep_param.PA_State_List[index];
            byte txPurpose    = sweep_param.Purpose_List[index];

            double* pwrList;
            int *rgiList, *vccList, *icqList;
            int num;
                if (sweep_param.Use_RGI_Pwr_List[index] == 1)
                {
                    // Use the parameter from XML.
                } 
                else 
                {
            if (bUseMBLforLim && txPurpose == 5)
            {
               pwrList = &paToMBL_Power[paState][0];
               rgiList = &paToMBL_RGI[paState][0];
               vccList = &paToMBL_Bias[paState][0];
               icqList = &paToMBL_Icq[paState][0];
               num = paToMBL_Power[paState].size();
            }
            else
            {
               pwrList = &paToApt_Power[paState][0];
               rgiList = &paToApt_RGI[paState][0];
               vccList = &paToApt_Bias[paState][0];
               icqList = &paToApt_Icq[paState][0];
               num = paToApt_Power[paState].size();
            }
            double targetPwr = sweep_param.FreqComp_Pwr_List[index];
            //power offset for freqcomp use to make sure all freq get enough bias/icq

                /* Find RGI */
            int iRGI = Get_Closet_Input_From_Matrix_WithTargetPwr(pwrList, rgiList, num,  targetPwr);
            targetPwr += sweep_param.APT_FreqComp_Offset_List[index];
                /* Find Bias */
            int iBias = Get_Closet_Input_From_Matrix_WithTargetPwr(pwrList, vccList, num, targetPwr);
                /* Find ICQ */
            int iIcq = Get_Closet_Input_From_Matrix_WithTargetPwr(pwrList, icqList, num, targetPwr);

            sweep_param.RGI_List[index][rgiIndex] = static_cast<byte>(iRGI);
            sweep_param.Bias_List[index][rgiIndex] = static_cast<word>(iBias);
            sweep_param.PA_Quiescent_Current[index][rgiIndex] = static_cast<byte>(iIcq);
         }
      }
        }
   } // End of precomputation

   //****************************************************************//
   // Create XPT FreqComp Segment
   //****************************************************************//

   //****************************************************************//
   // APT freqcomp (FDD case)
   // 5: TxLim          (high PA)   + Rx LNA 0
   // 4: FreqComp          (high PA)   + Rx LNA 1
   // 4: FreqComp          (Mid  PA) + Rx LNA 2
   // 4: FreqComp          (Low  PA) + Rx LNA 3
   // Add PA state 255              + Rx LNA 4 (LTE)
   // Add PA state 255              + Rx LNA 5 (LTE)
   //****************************************************************//
   //****************************************************************//
   // APT freqcomp (TDD case)
   // 5: TxLim          Rx Action 255
   // 4: FreqComp          Rx Action 255
   // 4: FreqComp          Rx Action 255
   // 4: FreqComp          Rx Action 255
   // Add PA state 255     Rx LNA 0
   // Add PA state 255     Rx LNA 1
   // Add PA state 255     Rx LNA 2
   // Add PA state 255     Rx LNA 3
   // Add PA state 255     Rx LNA 4
   // Add PA state 255     Rx LNA 5
   //****************************************************************//

   // Copy sweep parameter since we may need to modify sweep param temporaly
   Sweep_Params freqCompSwpParam = sweep_param;
   // Calculate the number of APT segments in one channel
//  int iNumAptTxSegment = (sweep_param.Meas_Tx & 1)?sweep_param.PA_State_List.size():0; // assume RGI index 1
    int iNumAptTxSegment = sweep_param.PA_State_List.size(); // assume RGI index 1

   int NUM_Rx_PM = (sweep_param.Rx_PM_List.size() > 0)?sweep_param.Rx_PM_List.size(): 1; // in case Rx_PM_List is zero.
    if (NUM_Rx_PM == 2 && demo_param.TECH == TECH_TDSCDMA)
    {
        NUM_Rx_PM = 1;
    }
   int iNumRxSegment = (sweep_param.Meas_Rx & 1)?sweep_param.Rx_Actions_List.size()*NUM_Rx_PM:0;

   int iNumAptSegment;
   if (  sweep_param.Sweep_Type == XPT_FREQCOMP_STAGGER_RXTX
      || sweep_param.Sweep_Type == APT_FREQCOMP_STAGGER_RXTX
      || sweep_param.Sweep_Type == XPT_FREQCOMP_SERIAL_RX
      || demo_param.TECH == TECH_TDSCDMA )
   {
      iNumAptSegment = iNumAptTxSegment;
   }
   else
   {
      if (RFCal_Helper_isTDDBand(demo_param.TECH,demo_param.BAND) && iNumAptTxSegment > 0)
      {
         /* Tx Segment */
            for (UINT i = 0; i < sweep_param.Rx_Actions_List.size()*static_cast<UINT>(NUM_Rx_PM); i ++)
            freqCompSwpParam.PA_State_List.push_back(255);

         /* Rx Segment */
         freqCompSwpParam.Rx_Actions_List.resize(freqCompSwpParam.PA_State_List.size());
         freqCompSwpParam.DL_Level_List.resize(freqCompSwpParam.PA_State_List.size());
         for (UINT i = 0; i < sweep_param.PA_State_List.size(); i ++)
         {
            freqCompSwpParam.Rx_Actions_List[i] = 255;
            freqCompSwpParam.DL_Level_List[i] = 255;
         }
         for (UINT i = 0; i <  sweep_param.Rx_Actions_List.size(); i ++)
         {
            freqCompSwpParam.Rx_Actions_List[i+sweep_param.PA_State_List.size()] = sweep_param.Rx_Actions_List[i];
            freqCompSwpParam.DL_Level_List[i+sweep_param.PA_State_List.size()] = sweep_param.DL_Level_List[i];
         }
         iNumAptSegment =  iNumAptTxSegment+iNumRxSegment;
      }
      else
      {
         int diff = iNumAptTxSegment - iNumRxSegment;
         if (diff > 0)
         {
            /* Tx Segment is larger */
            for (int i = 0 ; i  < diff; i ++)
            {
               freqCompSwpParam.Rx_Actions_List.push_back(255);
            }
         }
         else    /* Rx Segment is larger */
         {
            for (int i = 0 ; i  < (-diff); i ++)
            {
               freqCompSwpParam.PA_State_List.push_back(255);
            }
         }
         iNumAptSegment = (iNumAptTxSegment > iNumRxSegment)? iNumAptTxSegment:iNumRxSegment;
      }
   }

   for(UINT chanIndex = 0; chanIndex < sweep_param.Tx_Channel_List.size(); chanIndex++)
   {
      int iFirstSegment = recordIndex;
      //****************************************************************//
      // OpCode 0x100: Radio Setup
      //****************************************************************//
        int masterDevState = (chanIndex == 0 && sweep_param.bFirstSweep )? MASTER_RADIO_STATE_RX_TX_STATE : MASTER_RADIO_STATE_RETUNE;
      int slaveDevState = (chanIndex == 0)?  SLAVE_RADIO_STATE_RX_DIV_STATE : SLAVE_RADIO_STATE_RETUNE;
        if (RxDeviceFlag == 1) // No diversity
      {
         slaveDevState = SLAVE_RADIO_STATE_NO_CHANGE;
      }
      Sweep[recordIndex].txPurpose = FTM_Tx_Purpose_FreqComp_Lim_vs_Freq;
      RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex],
                                        sweep_param.Device_Selection.Master_Device_ID,
                                        sweep_param.Device_Selection.Slave_Device_ID,
            masterDevState /* masterDevState */, 
                                        slaveDevState /* slaveDevState */,
                                        demo_param.TECH,
                                        demo_param.BAND,
                                        (dword) sweep_param.Tx_Channel_List[chanIndex] /* ulChan */,
                                        sweep_param.Waveform.Waveform_Type,
                                        NULL,
                                        sweep_param.Waveform.Number_Of_RBs,
                                        sweep_param.Waveform.Start_RB);

      Sweep[recordIndex].radio_cfg.flag = sweep_param.Use_PilotOnlyWaveform;

            TE_Records[recordIndex].m_dl_lvl = static_cast<double>(static_cast<short>(freqCompSwpParam.DL_Level_List[0])); // To stabilize DL power
        if (chanIndex == 0)
      {
            dword iConfigDuration = ( sweep_param.bFirstSweep )? sweep_param.Config_Segment_Length : sweep_param.Tuning_Segment_Length;
            RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,iConfigDuration);
      }
      else
      {
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         //****************************************************************//
         // Insert NoP (OpCode 0x0)
         //****************************************************************//
         RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length, sweep_param.Segment_Length[0][0]);
      }

      for(int segmentIndex = 0 ; segmentIndex < iNumAptSegment; segmentIndex++)
      {
         Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;

         //****************************************************************//
         // Tx Measurent Segment
         //****************************************************************//

         if (freqCompSwpParam.PA_State_List[segmentIndex] != 255)
         {
            //****************************************************************//
            // OpCode 0x200: TxOverride
            //****************************************************************//
            RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],
                sweep_param.Purpose_List[segmentIndex], /* Tx Purpose */
                cal_param.TxDeviceFlag, 
                                              XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                              ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,
                                              txParamMask_PwrOverride /* pwrParamMask */,
                sweep_param.PA_State_List[segmentIndex] /* PA State List */,            
                sweep_param.RGI_List[segmentIndex][0] /* RGI List */,   
                (dword)sweep_param.Bias_List[segmentIndex][0] /* Bias List */,  
                ESC_TX_OVERRIDE_ACTION_SW_DEF,0  /* IQ Gain */, 
                ESC_TX_OVERRIDE_ACTION_SW_DEF,0  /* Env Scale */,
                                              sweep_param.PA_Quiescent_Current[segmentIndex][0],
                                              ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);
            Sweep[recordIndex].measSeg = true;

            TE_Records[recordIndex].m_ul_lvl = sweep_param.Max_FreqComp_Pwr[segmentIndex];

            //****************************************************************//
            // OpCode 0x8000: TxMeas            This is for HDET measurement
            //****************************************************************//
            if (sweep_param.HDET_Avgs.size() > 0 && sweep_param.HDET_Avgs[segmentIndex] > 0)
            {
                    RFCal_Helper_setOpCode_TxMeasure(Sweep[recordIndex],
                        Sweep[recordIndex].txParams.txDevice, 
                        1, 
                        sweep_param.HDET_Avgs[segmentIndex]);
            }
         }

            if (sweep_param.Sweep_Type != XPT_FREQCOMP_STAGGER_RXTX 
                && sweep_param.Sweep_Type != APT_FREQCOMP_STAGGER_RXTX
                && sweep_param.Sweep_Type != XPT_FREQCOMP_SERIAL_RX 
                && demo_param.TECH != TECH_TDSCDMA
                )
         {
            //****************************************************************//
            // Rx Measurent Segment
            //****************************************************************//
            if (freqCompSwpParam.Rx_Actions_List[segmentIndex] != 255)
            {
               //****************************************************************//
               // OpCode 0x4020: RxConfig + Rx Meas
               //****************************************************************//
               int rxPM = segmentIndex % NUM_Rx_PM;
               int RxIndex = static_cast<int>(segmentIndex/NUM_Rx_PM);
               word expRxAGC = (word)CalculateRxAGCValue(demo_param.TECH,freqCompSwpParam.DL_Level_List[RxIndex]); // 115 is min RSSI

                    RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], 
                        freqCompSwpParam.Rx_Actions_List[RxIndex] /* Rx Action List = LNA state*/,
                        RxDeviceFlag /* 1: Primary Rx, 2: Diversity Rx. 3: Primary + Diversity Rx */, 
                        rxPM,  /* Power Mode,  CDMA only */
                        expRxAGC, /* Expected Rx AGC.  Use CalculateRxAGCValue function  to convert dBm to AGC */
                        1,
                        sweep_param.Rx_Calibration_Type // Calibration Type
                        );

            TE_Records[recordIndex].m_dl_lvl = static_cast<double>(static_cast<short>(freqCompSwpParam.DL_Level_List[RxIndex]));
                }
         }
         // Channel informaiton for NV Generation
         Sweep[recordIndex].radio_cfg.chan.numChans = 1;
         Sweep[recordIndex].radio_cfg.chan.chanInfo[0].ulChan = (dword)sweep_param.Tx_Channel_List[chanIndex];
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
      }
      if (sweep_param.Sweep_Type == XPT_FREQCOMP || sweep_param.Sweep_Type == XPT_FREQCOMP_STAGGER_RXTX)
      {
         const int EPT_Mode = 0;
         const int ET_Mode = 1;

         if (sweep_param.Meas_Tx) // If Meas_Tx is false, no XPT measure as well as APT Tx measurement
         {
            //****************************************************************//
            // OpCode 0x800: DATA - Load Calc DPD table Action Processing
            //****************************************************************//
            for(int  modeIndex = 0; modeIndex < 2; modeIndex++)  // modeIndex = 0: EPT, 1: ET
            {
               if (modeIndex == EPT_Mode && sweep_param.EPT_Fcomp_Meas_Pwr_List.size() == 0) continue;
               if (modeIndex == ET_Mode  && sweep_param.ET_Fcomp_Meas_Pwr_List.size()  == 0) continue;

               unsigned char bEPT = (modeIndex == 0 )? true:false;
               Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;
               Sweep[recordIndex].resExpected   = true;
               RFCal_Helper_setOpCodeData(Sweep[recordIndex],cal_param,  sweep_param,
                                          DATA_OPCODE_ACTION_LOAD_CALC_DPD /* OpCode Data Option */, demo_param.TECH, demo_param.BAND, bEPT);

               if (modeIndex == 1 && sweep_param.EPT_Fcomp_Meas_Pwr_List.size() > 0)
               {
                  Sweep[recordIndex].data.loadCalcDpd.loadAction = LOAD_CALC_DPD_ACTION_LOAD_SC;
               }
                    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
            }

            //****************************************************************//
            // Meas EPT/ET Segments (EPT first then, ET if EPT is used)
            //****************************************************************//

            for(int modeIndex = 0; modeIndex <2; modeIndex++)
            {
               if (modeIndex == EPT_Mode && sweep_param.EPT_Fcomp_Meas_Pwr_List.size() == 0) continue;
               if (modeIndex == ET_Mode  && sweep_param.ET_Fcomp_Meas_Pwr_List.size()  == 0) continue;

               unsigned char bAddFudgeSegment = true;//(modeIndex == ET_Mode)? true: true;

               int rgiIndexSize = (modeIndex == EPT_Mode)? sweep_param.EPT_Fcomp_Meas_Pwr_List.size() :sweep_param.ET_Fcomp_Meas_Pwr_List.size();

               for(int rgiIndex = 0; rgiIndex <rgiIndexSize; rgiIndex++)
               {
                  int NumLoops = (bAddFudgeSegment)? 2: 1;
                  byte TxPurpose;
                  for (int i = 0; i < NumLoops; i ++) // Fudge Segment loop
                  {
                     /* ESC Parameters */
                     if (bAddFudgeSegment)      /* Fudge Segment */
                     {
                        Sweep[recordIndex].subSwpType = -1;
                        Sweep[recordIndex].measSeg = false;
                        TxPurpose = 0;
                     }
                     else                 /* Regular Segment */
                     {
                        Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_APPLY_DPD;
                        Sweep[recordIndex].measSeg = true;
                        TxPurpose = FTM_Tx_Purpose_FreqComp_Lin_vs_Freq;
                     }

                     //****************************************************************//
                     // OpCode 0x200: TxOverride
                     //****************************************************************//
                            RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], 
                                TxPurpose, 
                                cal_param.TxDeviceFlag, 
                                ((modeIndex == EPT_Mode)?XPT_TYPE_EPT:XPT_TYPE_ET), 
                                ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */, 
                                                       ESC_TX_PWR_OVERRIDE_ABS_PWR /* pwrOverrideType */,       0 /* pwrParamMask */,
                                                       NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,NULL,NULL,
                            (word)((modeIndex == EPT_Mode)?sweep_param.EPT_Fcomp_Meas_Pwr_List[rgiIndex]*100.0:sweep_param.ET_Fcomp_Meas_Pwr_List[rgiIndex]*100.0),
                            sweep_param.XPT_Fcomp_Ref_Channel_List[0]);

            TE_Records[recordIndex].m_ul_lvl =static_cast<double>((modeIndex == EPT_Mode)?sweep_param.EPT_Fcomp_Meas_Pwr_List[rgiIndex]:sweep_param.ET_Fcomp_Meas_Pwr_List[rgiIndex])
                + sweep_param.XPT_Fcomp_Deviation_From_Exp_Power_Max_dBm100/100.0;
                            // APPLY PIN Compensation is required
                     Sweep[recordIndex].txParams.flag = 1; // Apply PIN Compensation.Only FreqComp requires this parameter.
                     // Channel informaiton for NV Generation
                     Sweep[recordIndex].radio_cfg.chan.numChans               = 1;
                     Sweep[recordIndex].radio_cfg.chan.chanInfo[0].ulChan     = (dword) sweep_param.Tx_Channel_List[chanIndex];

                            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
                            bAddFudgeSegment = false;
                        }
                    } // for(int rgiIndex = 0; rgiIndex < sizeof(ET_Fcomp_Meas_Pwr_List)/sizeof(double); rgiIndex++)
                } // for(int modeIndex = 0; modeIndex <2; modeIndex++)
            } // if (sweep_param.Meas_Tx) // If Meas_Tx is false, no XPT measure as well as APT Tx measurement
        } // if (sweep_param.Sweep_Type == XPT_FREQCOMP)

        if (sweep_param.Sweep_Type == XPT_FREQCOMP_STAGGER_RXTX 
            || sweep_param.Sweep_Type == APT_FREQCOMP_STAGGER_RXTX
            || sweep_param.Sweep_Type == XPT_FREQCOMP_SERIAL_RX
            || demo_param.TECH == TECH_TDSCDMA)
        {
            //****************************************************************//
            //  Rx Measurent Segment 
            //****************************************************************//
            for (int segmentIndex = 0; segmentIndex < iNumRxSegment; segmentIndex++)
            {   
                //****************************************************************//
                //  OpCode 0x4020: RxConfig + Rx Meas
                //****************************************************************//
                int rxPM = segmentIndex % NUM_Rx_PM;
                int RxIndex = static_cast<int>(segmentIndex/NUM_Rx_PM);
                if (sweep_param.Rx_Actions_List[RxIndex] == 255) continue; // in case of Rx_Action_List[] = {65535,65535...};

                unsigned char bAlternatePath = true;
                unsigned short pRxPath[2] = {0,0};
                unsigned short arraySize = 2; // 2 : PRx + Div, 1: PRx Only

                if (bAlternatePath)
                {
                    pRxPath[0] = 1; // PRx
                    pRxPath[1] = 1; // DRx
                }

                word expRxAGC = (word)CalculateRxAGCValue(demo_param.TECH,sweep_param.DL_Level_List[RxIndex]); // 115 is min RSSI           
                RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], sweep_param.Rx_Actions_List[RxIndex],
                    RxDeviceFlag, rxPM,expRxAGC, 1,
                    sweep_param.Rx_Calibration_Type,
                    pRxPath,
                    arraySize);

            TE_Records[recordIndex].m_dl_lvl = static_cast<double>(static_cast<short>(freqCompSwpParam.DL_Level_List[RxIndex]));

                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
            }
        }
        int iLastSegment = recordIndex;
        numberOfSegmentsForOneChannel = iLastSegment - iFirstSegment ;
    }

    //*****************************************************************************************/
    //      Meas DC
    //*****************************************************************************************/
    if (sweep_param.Measure_DC)
    {
        //****************************************************************//
        //  OpCode 0x100: Radio Setup
        //****************************************************************//        
        int masterDevState = MASTER_RADIO_STATE_RETUNE;
        int slaveDevState = (RxDeviceFlag == 3)?SLAVE_RADIO_STATE_RETUNE:SLAVE_RADIO_STATE_NO_CHANGE;

        RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], 
            sweep_param.Device_Selection.Master_Device_ID ,  
            sweep_param.Device_Selection.Slave_Device_ID ,
            masterDevState /* Master Device State */, 
            slaveDevState /* Slave Device State */, 
            demo_param.TECH, 
            demo_param.BAND, 
            (dword) sweep_param.Ref_Channel /* ulChan */);
        
        Sweep[recordIndex].radio_cfg.flag = sweep_param.Use_PilotOnlyWaveform;  

        // DC Cal specific configuration
        const dword DualCarrierChannelOffset = 25;  // Hard coding in subsysRFCal. No parameter XML
        Sweep[recordIndex].radio_cfg.chan.numChans = 2;
        Sweep[recordIndex].radio_cfg.chan.chanInfo[1].ulChan = sweep_param.Ref_Channel + DualCarrierChannelOffset;  
        Sweep[recordIndex].radio_cfg.chan.chanInfo[1].txBW  = sweep_param.Waveform.Tx_BW;
        Sweep[recordIndex].radio_cfg.chan.chanInfo[1].rxBW  = sweep_param.Waveform.Rx_BW;
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

         //****************************************************************//
        //  Insert NoP (OpCode 0x0)  
         //****************************************************************//
        RFCal_helper_InsertNop(Sweep,recordIndex, static_cast<int>(ceil(static_cast<double>(sweep_param.Tuning_Segment_Length)/static_cast<double>(sweep_param.Segment_Length[0][0]) - 1.0)));  

            //****************************************************************//
        //  OpCode 0x4020: RxConfig + Rx Meas. DC cal 
            //****************************************************************//
        for (UINT RxIndex = 0;  RxIndex < sweep_param.Rx_Actions_List.size(); RxIndex++)
        {
            word expRxAGC = (word)CalculateRxAGCValue(demo_param.TECH,sweep_param.DL_Level_List[RxIndex]); // 115 is min RSSI
            int RxMeasActionForDCCal = 2;
            RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], sweep_param.Rx_Actions_List[RxIndex],
                RxDeviceFlag, 0,expRxAGC, RxMeasActionForDCCal, RFNV_RX_CAL_DATA_NV_TYPE_WCDMA_DC);

            TE_Records[recordIndex].m_dl_lvl = static_cast<double>(static_cast<short>(freqCompSwpParam.DL_Level_List[RxIndex]));

            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         }
      }
   //*****************************************************************************************/
    //      Meas CA (DRDS. WCDMA )
   //*****************************************************************************************/
   if (sweep_param.Measure_CA)
   {
      //****************************************************************//
      //   Assign RxDeviceFlag for CA
      //****************************************************************//   
      int RxDeviceFlagSCELL = 0;
      // Check Primary Rx is included  
      bool bPRxCal = false;
      bool bDRxCal = false;

      if ( find (sweep_param.Device_Selection.Rx_Device_To_Calibrate.begin(), sweep_param.Device_Selection.Rx_Device_To_Calibrate.end(), sweep_param.Device_Selection.Master_Device_ID) != sweep_param.Device_Selection.Rx_Device_To_Calibrate.end())       
      {
         bPRxCal = true;
         RxDeviceFlagSCELL = (1 << sweep_param.Device_Selection.Scell_Master_Device_ID);
      }      
      // Check Diversity Rx is included 
      if ( find (sweep_param.Device_Selection.Rx_Device_To_Calibrate.begin(), sweep_param.Device_Selection.Rx_Device_To_Calibrate.end(), sweep_param.Device_Selection.Slave_Device_ID) != sweep_param.Device_Selection.Rx_Device_To_Calibrate.end())       
      {
         bDRxCal = true;
         RxDeviceFlagSCELL |= (1 << sweep_param.Device_Selection.Scell_Slave_Device_ID);
      }


      //****************************************************************//
      // OpCode 0x100: Radio Setup to change PCell
      //****************************************************************//
        if (demo_param.TECH == 3) // LTE
        {
      int masterDevState = MASTER_RADIO_STATE_RX_TX_STATE;
         int slaveDevState = (bDRxCal)?SLAVE_RADIO_STATE_RX_DIV_STATE : SLAVE_RADIO_STATE_NO_CHANGE;

      RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID ,sweep_param.Device_Selection.Slave_Device_ID,
                                        masterDevState , slaveDevState ,
                                        demo_param.TECH, sweep_param.CA_PCell_Band, sweep_param.CA_PCell_Channel/* ulChan */,
                                        sweep_param.Waveform.Waveform_Type, NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB,
                                        sweep_param.Waveform.EUL_Waveform_Type,sweep_param.Waveform.Rx_BW,sweep_param.Waveform.Tx_BW,false);
      Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_APT_CA;
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

        }
        else if (demo_param.TECH == 1) // WCDMA
        {
            int masterDevState = MASTER_RADIO_STATE_RETUNE;
         int slaveDevState = (bDRxCal)?SLAVE_RADIO_STATE_RETUNE: SLAVE_RADIO_STATE_NO_CHANGE;

            // Channel secltion, choose next to refchannel
            dword chan = sweep_param.Tx_Channel_List[sweep_param.Tx_Channel_List.size()/2];
            chan = (chan != sweep_param.Ref_Channel) ? chan : sweep_param.Tx_Channel_List[sweep_param.Tx_Channel_List.size()/2+1];

            RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID ,sweep_param.Device_Selection.Slave_Device_ID,
            masterDevState , slaveDevState , 
            demo_param.TECH, 
            demo_param.BAND,
            chan /* ulChan */,
            sweep_param.Waveform.Waveform_Type, NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB,
            sweep_param.Waveform.EUL_Waveform_Type,sweep_param.Waveform.Rx_BW,sweep_param.Waveform.Tx_BW,false);        
            Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_APT_CA;
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
            
        }
      //****************************************************************//
      // Insert NoP (OpCode 0x0)
      //****************************************************************//
      int configNumSeg =  numberOfSegmentsForOneChannel - 1;// ceil(static_cast<double>(sweep_param.Tuning_Segment_Length)/static_cast<double>(sweep_param.Segment_Length[0][0]) + sweep_param.DL_Level_List.size() + sweep_param.ET_Fcomp_Meas_Pwr_List.size()+1.0);
      RFCal_helper_InsertNop(Sweep,recordIndex, configNumSeg);

      for(UINT chanIndex = 0; chanIndex < sweep_param.Tx_Channel_List.size(); chanIndex++)
      {
         int iFirstSegment = recordIndex;
         //****************************************************************//
         // OpCode 0x100: Radio Setup SCell Configuration
         //****************************************************************//
         int masterDevice = static_cast<int>(sweep_param.Device_Selection.Scell_Master_Device_ID);
         int slaveDevice = static_cast<int>(sweep_param.Device_Selection.Scell_Slave_Device_ID );

         int masterDevState = (chanIndex == 0)?MASTER_RADIO_STATE_RX_STATE:MASTER_RADIO_STATE_RETUNE;
         int slaveDevState = (chanIndex == 0 && bDRxCal)?SLAVE_RADIO_STATE_RX_DIV_STATE:SLAVE_RADIO_STATE_NO_CHANGE;

         RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], masterDevice, slaveDevice,
                                           masterDevState /* masterDevState */, slaveDevState /* slaveDevState */,
                                           demo_param.TECH, demo_param.BAND, sweep_param.Tx_Channel_List[chanIndex]/* ulChan */,
                                           sweep_param.Waveform.Waveform_Type, NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB,
                                           sweep_param.Waveform.EUL_Waveform_Type,sweep_param.Waveform.Rx_BW,sweep_param.Waveform.Tx_BW,false);
         Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_APT_CA;
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

         //****************************************************************//
         // Insert Nop
         //****************************************************************//
         RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length, sweep_param.Segment_Length[0][0]);
         for (UINT RxIndex = 0; RxIndex < sweep_param.Rx_Actions_List.size(); RxIndex++)
         {
            //****************************************************************//
            // OpCode 0x4020: RxConfig + Rx Meas
            //****************************************************************//
            Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_APT_CA;
            word expRxAGC = (word)CalculateRxAGCValue(demo_param.TECH,sweep_param.DL_Level_List[RxIndex]); // 115 is min RSSI
            RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], sweep_param.Rx_Actions_List[RxIndex],
                    RxDeviceFlagSCELL, 0 /* pm */,expRxAGC, 1 /* rx meas action */, sweep_param.Rx_Calibration_Type /* Rx Cal Type */);

            TE_Records[recordIndex].m_dl_lvl = static_cast<double>(static_cast<short>(freqCompSwpParam.DL_Level_List[RxIndex]));
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         }
         //****************************************************************//
         // Insert Nop
         //****************************************************************//
         int iLastSegment = recordIndex;
         int numNop = numberOfSegmentsForOneChannel - (iLastSegment - iFirstSegment);

         RFCal_helper_InsertNop(Sweep,recordIndex, numNop);
      }
   }

   //*****************************************************************************************/
    //      High linearity CAL for TDSCDMA
    //*****************************************************************************************/
    if (demo_param.TECH == TECH_TDSCDMA && sweep_param.Rx_PM_List.size() > 1)
    {
        //****************************************************************//
        //  OpCode 0x100: Radio Setup
        //****************************************************************//        
        int masterDevState = MASTER_RADIO_STATE_RETUNE;
        int slaveDevState = (RxDeviceFlag == 3)?SLAVE_RADIO_STATE_RETUNE:SLAVE_RADIO_STATE_NO_CHANGE;

        RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], 
            sweep_param.Device_Selection.Master_Device_ID ,  
            sweep_param.Device_Selection.Slave_Device_ID ,
            masterDevState /* Master Device State */, 
            slaveDevState /* Slave Device State */, 
            demo_param.TECH, 
            demo_param.BAND, 
            (dword) sweep_param.Ref_Channel /* ulChan */);
        
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
        //****************************************************************//
        //  Insert NoP (OpCode 0x0) for Radio Config
        //****************************************************************//
        RFCal_helper_InsertNop(Sweep,recordIndex,sweep_param.Tuning_Segment_Length, sweep_param.Segment_Length[0][0]);  

        //in TDSCDMA case, txrxParamsSize = number of dummy TX + number of LNA states for Callbox DL power sync.
        //****************************************************************//
        //  Insert NoP (OpCode 0x0) for Dummy Tx
        //****************************************************************//
        RFCal_helper_InsertNop(Sweep,recordIndex, iNumAptTxSegment);    

        //****************************************************************//
        //  Rx Measurent Segment 
        //****************************************************************//
        for (int segmentIndex = 0; segmentIndex < iNumRxSegment; segmentIndex++)
        {   
            //****************************************************************//
            //  OpCode 0x4020: RxConfig + Rx Meas
            //****************************************************************//
            int rxPM = 1;
            int RxIndex = static_cast<int>(segmentIndex);
            if (sweep_param.Rx_Actions_List[RxIndex] == 255) continue; // in case of Rx_Action_List[] = {65535,65535...};

            word expRxAGC = (word)CalculateRxAGCValue(demo_param.TECH,sweep_param.DL_Level_List[RxIndex]); // 115 is min RSSI           
            RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], 
                sweep_param.Rx_Actions_List[RxIndex],
                RxDeviceFlag, 
                rxPM,expRxAGC,
                1 /* Rx Meas Action */,
                RFNV_RX_CAL_DATA_NV_TYPE_TDSCDMA_HL);
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
        }
    }

    //*****************************************************************************************/
   //    End of FreqComp: Insert a low rgi segment instead of tear down
   //*****************************************************************************************/
   if (sweep_param.Measure_CA && demo_param.TECH == 3)
   {
   }
   else
   {
      RFCal_Helper_setLowRGISegment(Sweep[recordIndex],cal_param.TxDeviceFlag);
    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
   }

   //*********************************************//
   //    Generate OpCode for all the segment
   //*********************************************//
   Sweep.resize(recordIndex); // Delete unused vector.
 
   //*********************************************//
   //      Disable Tx NV generation if Rx only sweep
   //*********************************************//
   if (sweep_param.Meas_Tx == 0)
   {
      for (vector <cal_sweep_record>::iterator it = Sweep.begin(); it != Sweep.end(); it++)
      {
         it->txPurpose = 0xFFFF;
      }
   }

   RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());
   return true;
}

unsigned char RFCal_Helper_ESC_ET_createINT_CAL_XPTFreqCompSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, Sweep_Params & sweep_param,
    const vector < vector < cal_sweep_record > > & AllSweeps/* vector < cal_sweep_record >  APT_Sweep */, vector < cal_sweep_record >&      Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
   unsigned char  bOK            = true;
   UINT        recordIndex    = 0;
   int txParamMask_PwrOverride  = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN |
                                   ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT); // Sweep
   int numberOfSegmentsForOneChannel = 0; // this info is required for inter CA cal and DC cal
   byte pDev = RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag);

   //****************************************************************//
   // PreComputation for FreqComp
   //****************************************************************//
   const int     fbDev          =   static_cast<int>(sweep_param.Device_Selection.Feedback_Device_ID);   
   const int      fbEnabled      =  1;
   const int      fbDisabled     =  0;
   int            fbGainState    =  0;
   const unsigned char ENABLED   = true;
   const unsigned char DISABLED  = false;
   unsigned char  FBmode = false;

   byte devId = sweep_param.Device_Selection.Master_Device_ID;
    map <RFCal_Helper_Tx_Params, double > APTV6CharToPwr;
    map < pair < dword, byte >, Tx_RefCal_Data_V2_Record > ChanFBGainStateToPwrFBGainParams;

    if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
    {
      RFCal_Helper_ESC_ET_Tx_AptChar_DataV6_Assignment_XML(demo_param,  cal_param, sweep_param, APTV6CharToPwr); // Key: Channel. pa_state, rgi
      RFCal_Helper_ESC_ET_FBGain_Ref_Assignment_XML(demo_param,  cal_param, sweep_param, ChanFBGainStateToPwrFBGainParams);
    }


   if ( sweep_param.Pre_Sweep_Computation.size() > 0 )
   {
      //****************************************************************//
      // Find APT and Max Bias Sweep Records from Pre_Sweep_Computation parameters
      //****************************************************************//

      map < byte, vector < double > >  paToApt_Power;
      map < byte, vector < int > >     paToApt_RGI,   paToApt_Bias, paToApt_Icq;

      map < byte, vector < double > >  paToMBL_Power;
      map < byte, vector < int > >     paToMBL_RGI,   paToMBL_Bias,  paToMBL_Icq;

      unsigned char bUseMBLforLim = false; // True if pre comp type is 5

      // ********************************************************************* //
      //    Collect measured power rgi list from ATP sweep and MBL sweep (if applicable)
      // ********************************************************************* //

      for (UINT preCompIndex = 0; preCompIndex < sweep_param.Pre_Sweep_Computation.size(); preCompIndex++)
      {
         int pre_comp_type = sweep_param.Pre_Sweep_Computation[preCompIndex];
         int pre_sweep_index = sweep_param.Compute_From_Sweep[preCompIndex];

         if (pre_comp_type == 5)
         {
            bUseMBLforLim = true;
         }

         map <byte, UINT> paToIndex;
         for (UINT i = 0; i < cal_param.Swp[pre_sweep_index].PA_State_List.size(); i++)
         {
            paToIndex.insert(make_pair(cal_param.Swp[pre_sweep_index].PA_State_List[i],i));
         }
         vector < vector < double > > powerList;
         vector < vector < int > > rgiList, biasList, icqList;
         powerList.resize(paToIndex.size());
         rgiList.resize(paToIndex.size());
         biasList.resize(paToIndex.size());
         icqList.resize(paToIndex.size());

         for (UINT i = 0; i < AllSweeps[pre_sweep_index].size(); i++)
         {
            const cal_sweep_record * pTmp = & AllSweeps[pre_sweep_index][i];
            // only max bias segment
            if (pTmp->txParams.txParams[devId].pwrOverrideType == 1 && pTmp->txParams.txParams[devId].rgi > 0)  
            {
               if (bUseMBLforLim && pTmp->txPurpose != 255)
               {
                  continue;
               }
               byte pa = pTmp->txParams.txParams[devId].paState;

               powerList[paToIndex[pa]].push_back(static_cast<double>(static_cast<short>(pTmp->txRes.txPower))/10.0);
               rgiList[paToIndex[pa]].push_back(static_cast<int>(pTmp->txParams.txParams[devId].rgi)); 
               biasList[paToIndex[pa]].push_back(static_cast<int>(pTmp->txParams.txParams[devId].bias));
               icqList[paToIndex[pa]].push_back(static_cast<int>(pTmp->txParams.txParams[devId].quiescentCurrent)); 
            }
         }

         // ********************************************************************* //
         //    Store the collected into ATP and MBL maps (if applicable)
         // ********************************************************************* //

         if (pre_comp_type == 5)
         {
            for (map <byte, UINT>::iterator it = paToIndex.begin(); it != paToIndex.end(); it ++)
            {
               byte pa = (byte)it->first;
               paToMBL_Power[pa] = (vector < double >)powerList[paToIndex[pa]];
               paToMBL_RGI[pa] = (vector < int >)rgiList[paToIndex[pa]];
               paToMBL_Bias[pa] = (vector < int >)biasList[paToIndex[pa]];
               paToMBL_Icq[pa] = (vector < int >)icqList[paToIndex[pa]];
            }
         }
         else if  (pre_comp_type == 3)
         {
            for (map <byte, UINT>::iterator it = paToIndex.begin(); it != paToIndex.end(); it ++)
            {
               byte pa = (byte)it->first;
               paToApt_Power[pa] = (vector < double >)powerList[paToIndex[pa]];
               paToApt_RGI[pa] = (vector < int >)rgiList[paToIndex[pa]];
               paToApt_Bias[pa] = (vector < int >)biasList[paToIndex[pa]];
               paToApt_Icq[pa] = (vector < int >)icqList[paToIndex[pa]];
            }
         }
         else
         {
        //      RFCal_Helper_AddMessageToQMSLLog(demo_param.hPhone," Unknown pre computation type");
         }
      }
      // ********************************************************************* //
      //    Assign closest value to the sweep parameters.
      // ********************************************************************* //

      for (UINT index=0; index < sweep_param.PA_State_List.size(); index++)
      {
         for (UINT rgiIndex = 0; rgiIndex < sweep_param.RGI_List[index].size(); rgiIndex++)
         {
            byte paState      = sweep_param.PA_State_List[index];
            byte txPurpose    = sweep_param.Purpose_List[index];

            double* pwrList;
            int *rgiList, *vccList, *icqList;
            int num;
            if (bUseMBLforLim && txPurpose == 5)
            {
               pwrList = &paToMBL_Power[paState][0];
               rgiList = &paToMBL_RGI[paState][0];
               vccList = &paToMBL_Bias[paState][0];
               icqList = &paToMBL_Icq[paState][0];
               num = paToMBL_Power[paState].size();
            }
            else
            {
               pwrList = &paToApt_Power[paState][0];
               rgiList = &paToApt_RGI[paState][0];
               vccList = &paToApt_Bias[paState][0];
               icqList = &paToApt_Icq[paState][0];
               num = paToApt_Power[paState].size();
            }
            double targetPwr = sweep_param.FreqComp_Pwr_List[index];
            //power offset for freqcomp use to make sure all freq get enough bias/icq

            int iRGI = Get_Closet_Input_From_Matrix_WithTargetPwr(pwrList, rgiList, num,  targetPwr);
            targetPwr += sweep_param.APT_FreqComp_Offset_List[index];
            int iBias = Get_Closet_Input_From_Matrix_WithTargetPwr(pwrList, vccList, num, targetPwr);
            int iIcq = Get_Closet_Input_From_Matrix_WithTargetPwr(pwrList, icqList, num, targetPwr);

            sweep_param.RGI_List[index][rgiIndex] = static_cast<byte>(iRGI);
            sweep_param.Bias_List[index][rgiIndex] = static_cast<word>(iBias);
            sweep_param.PA_Quiescent_Current[index][rgiIndex] = static_cast<byte>(iIcq);
         }
      }
   } // End of precomputation

   //****************************************************************//
   // Create Ref RGI map
   //****************************************************************//

   map < pair < byte, byte >,  byte >  PaRgiToRefRGI;
   int pre_comp_type = 0;
   int pre_sweep_index = 0;
   for (UINT preCompIndex = 0; preCompIndex < sweep_param.Pre_Sweep_Computation.size(); preCompIndex++)
   {
      if (pre_comp_type == 3 || pre_comp_type == 5)
      {
         int pre_comp_type = sweep_param.Pre_Sweep_Computation[preCompIndex];
         int pre_sweep_index = sweep_param.Compute_From_Sweep[preCompIndex];
         break;
      }
   }
   for (UINT paIndex = 0 ; paIndex < sweep_param.PA_State_List.size(); paIndex ++ )
   {
      for (UINT rgiIndex = 0; rgiIndex < sweep_param.RGI_List[paIndex].size(); rgiIndex++)
      {
         byte pa = sweep_param.PA_State_List[paIndex];
         byte rgi = sweep_param.RGI_List[paIndex][rgiIndex];

         for (UINT iSegNum = 0; iSegNum < AllSweeps[pre_sweep_index].size(); iSegNum ++)
         {
            if (AllSweeps[pre_sweep_index][iSegNum].txParams.txParams[devId].paState == pa && AllSweeps[pre_sweep_index][iSegNum].txParams.txParams[devId].rgi == rgi)
            {
               PaRgiToRefRGI[make_pair(pa,rgi)] = AllSweeps[pre_sweep_index][iSegNum].txIntCalParams.refRGI;
               break;
            }
         }
      }
   }

   //****************************************************************//
   // Create XPT FreqComp Segment
   //****************************************************************//

   //****************************************************************//
   // APT freqcomp (FDD case)
   // 5: TxLim          (high PA)   + Rx LNA 0
   // 4: FreqComp          (high PA)   + Rx LNA 1
   // 4: FreqComp          (Mid  PA) + Rx LNA 2
   // 4: FreqComp          (Low  PA) + Rx LNA 3
   // Add PA state 255              + Rx LNA 4 (LTE)
   // Add PA state 255              + Rx LNA 5 (LTE)
   //****************************************************************//
   //****************************************************************//
   // APT freqcomp (TDD case)
   // 5: TxLim          Rx Action 255
   // 4: FreqComp          Rx Action 255
   // 4: FreqComp          Rx Action 255
   // 4: FreqComp          Rx Action 255
   // Add PA state 255     Rx LNA 0
   // Add PA state 255     Rx LNA 1
   // Add PA state 255     Rx LNA 2
   // Add PA state 255     Rx LNA 3
   // Add PA state 255     Rx LNA 4
   // Add PA state 255     Rx LNA 5
   //****************************************************************//

   // Copy sweep parameter since we may need to modify sweep param temporaly
   Sweep_Params freqCompSwpParam = sweep_param;
   // Calculate the number of APT segments in one channel
   int iNumAptTxSegment = (sweep_param.Meas_Tx & 1)?sweep_param.PA_State_List.size():0; // assume RGI index 1
   int NUM_Rx_PM = (sweep_param.Rx_PM_List.size() > 0)?sweep_param.Rx_PM_List.size(): 1; // in case Rx_PM_List is zero.
   int iNumRxSegment = (sweep_param.Meas_Rx & 1)?sweep_param.Rx_Actions_List.size()*NUM_Rx_PM:0;

   int iNumAptSegment;

   if (RFCal_Helper_isTDDBand(demo_param.TECH,demo_param.BAND) && iNumAptTxSegment > 0)
   {
      /* Tx Segment */
      for (UINT i = 0; i < sweep_param.Rx_Actions_List.size(); i ++)
         freqCompSwpParam.PA_State_List.push_back(255);

      /* Rx Segment */
      freqCompSwpParam.Rx_Actions_List.resize(freqCompSwpParam.PA_State_List.size());
      freqCompSwpParam.DL_Level_List.resize(freqCompSwpParam.PA_State_List.size());
      for (UINT i = 0; i < sweep_param.PA_State_List.size(); i ++)
      {
         freqCompSwpParam.Rx_Actions_List[i] = 255;
         freqCompSwpParam.DL_Level_List[i] = 255;
      }
      for (UINT i = 0; i <  sweep_param.Rx_Actions_List.size(); i ++)
      {
         freqCompSwpParam.Rx_Actions_List[i+sweep_param.PA_State_List.size()] = sweep_param.Rx_Actions_List[i];
         freqCompSwpParam.DL_Level_List[i+sweep_param.PA_State_List.size()] = sweep_param.DL_Level_List[i];
      }
      iNumAptSegment =  iNumAptTxSegment+iNumRxSegment;
   }
   else
   {
      int diff = iNumAptTxSegment - iNumRxSegment;
      if (diff > 0)
      {
         /* Tx Segment is larger */
         for (int i = 0 ; i  < diff; i ++)
         {
            freqCompSwpParam.Rx_Actions_List.push_back(255);
         }
      }
      else    /* Rx Segment is larger */
      {
         for (int i = 0 ; i  < (-diff); i ++)
         {
            freqCompSwpParam.PA_State_List.push_back(255);
         }
      }
      iNumAptSegment = (iNumAptTxSegment > iNumRxSegment)? iNumAptTxSegment:iNumRxSegment;
   }

   for(UINT chanIndex = 0; chanIndex < sweep_param.Tx_Channel_List.size(); chanIndex++)
   {
      int iFirstSegment = recordIndex;
      //****************************************************************//
      // OpCode 0x100: Radio Setup
      //****************************************************************//
      int slaveDevState = (chanIndex == 0)?  SLAVE_RADIO_STATE_RX_DIV_STATE : SLAVE_RADIO_STATE_RETUNE;
      if (cal_param.RxDeviceFlag == 1) // No diversity
      {
         slaveDevState = 0;
      }
      slaveDevState = 0;
      Sweep[recordIndex].txPurpose = FTM_Tx_Purpose_FreqComp_Lim_vs_Freq;
      RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID ,sweep_param.Device_Selection.Slave_Device_ID,
                                        MASTER_RADIO_STATE_RETUNE /* masterDevState */, slaveDevState /* slaveDevState */, demo_param.TECH, demo_param.BAND, (dword) sweep_param.Tx_Channel_List[chanIndex] /* ulChan */
                                        , sweep_param.Waveform.Waveform_Type, NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);
      Sweep[recordIndex].radio_cfg.flag = sweep_param.Use_PilotOnlyWaveform;
      if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
      {
         Sweep[recordIndex].radio_cfg.flag = 1;
      }
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

      //****************************************************************//
      // Insert NoP (OpCode 0x0)
      //****************************************************************/
      RFCal_helper_InsertNop(Sweep,recordIndex,sweep_param.Tuning_Segment_Length, sweep_param.Segment_Length[0][0]);

        if (!FBmode && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN  )
      {
         //*****************************************************************************************//
         //  OpCode 0x10000: FBSetup   Feedback Action:  4  ( | Bit 2 = Configure RxLM for FB AGC read )
         //*****************************************************************************************//
         FBmode = true;
         RFCal_Helper_ESC_ET_OpCodeConfigureRxLM(demo_param, cal_param, sweep_param, Sweep, recordIndex, FBmode);
      }

      for(int segmentIndex = 0 ; segmentIndex < iNumAptSegment; segmentIndex++)
      {
         Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;

         //****************************************************************//
         // Tx Measurent Segment
         //****************************************************************//

         if (freqCompSwpParam.PA_State_List[segmentIndex] != 255)
            {
            if ( sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN)
         {
            //****************************************************************//
            //    OpCode 0x10000:   Configure WTR
            //****************************************************************//
               RFCal_Helper_ESC_ET_ConfigureFB( Sweep[recordIndex], pDev, true,FBmode, fbDev, fbGainState, sweep_param.PA_State_List[segmentIndex], PaRgiToRefRGI[make_pair(sweep_param.PA_State_List[segmentIndex], sweep_param.RGI_List[segmentIndex][0])]);

            //****************************************************************//
            //    OpCode 0x4020 = (OpCode 0x20 | OpCode 0x4000): Rx config and measurement for Tx
            //****************************************************************//
            RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], 0 /* LNA_state */ ,  2 /* RxFlag */, 0 , 0 /* exRxAGC */ , 1 /* rx_meas_action */);
            }
            else if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
               {
                   //***************************************************************************************//
                   //       OpCode 0x8000: TxMeas. FBGain Measurement 
                   //***************************************************************************************//
               RFCal_Helper_setOpCode_FBMeasurement(sweep_param,  Sweep[recordIndex], sweep_param.PA_State_List[segmentIndex], sweep_param.RGI_List[segmentIndex][0], sweep_param.Ref_RGI_List[segmentIndex][0], NO_OVERLAP,   
               APTV6CharToPwr, ChanFBGainStateToPwrFBGainParams);
               }

            //****************************************************************//
            //    OpCode 0x200: TxOverride
            //****************************************************************//
            RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],
                                              sweep_param.Purpose_List[segmentIndex], cal_param.TxDeviceFlag,
                                              XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                              ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,
                                              txParamMask_PwrOverride /* pwrParamMask */,
                                              sweep_param.PA_State_List[segmentIndex],
                                              sweep_param.RGI_List[segmentIndex][0],
                                              (dword)sweep_param.Bias_List[segmentIndex][0],
                                              ESC_TX_OVERRIDE_ACTION_SW_DEF,0, ESC_TX_OVERRIDE_ACTION_SW_DEF,0,
                                              sweep_param.PA_Quiescent_Current[segmentIndex][0],
                                              ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);
            Sweep[recordIndex].measSeg = true;

//             double MaxExpectedPwr = sweep_param.Max_Expected_Pwr[segmentIndex]- std::floor(segmentIndex*0.6);
               TE_Records[recordIndex].m_ul_lvl = 16;


            // Channel informaiton for NV Generation
            Sweep[recordIndex].radio_cfg.chan.numChans = 1;
            Sweep[recordIndex].radio_cfg.chan.chanInfo[0].ulChan = (dword)sweep_param.Tx_Channel_List[chanIndex];
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

            if (sweep_param.HDET_Avgs.size() > 0 && sweep_param.HDET_Avgs[segmentIndex] > 0)
            {
               //****************************************************************//
               //    OpCode 0x10000:   Configure WTR
               //****************************************************************//
               RFCal_Helper_ESC_ET_ConfigureFB( Sweep[recordIndex], pDev, false, FBmode, fbDev, fbGainState, cal_param.Swp[0].XPT_Swp1_PA_State,cal_param.Swp[0].XPT_Swp1_RGI_Delta_Ref_RGI);

               //****************************************************************//
               //    OpCode 0x8000: TxMeas            This is for HDET measurement
               //****************************************************************//
               RFCal_Helper_setOpCode_TxMeasure(Sweep[recordIndex],Sweep[recordIndex].txParams.txDevice, 1, sweep_param.HDET_Avgs[segmentIndex]);

               // Channel informaiton for NV Generation
               Sweep[recordIndex].radio_cfg.chan.numChans = 1;
               Sweep[recordIndex].radio_cfg.chan.chanInfo[0].ulChan = (dword)sweep_param.Tx_Channel_List[chanIndex];
                    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
            }
         }
      }
      if ( sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP )
      {
         const int EPT_Mode = 0;
         const int ET_Mode = 1;

         if (sweep_param.Meas_Tx) // If Meas_Tx is false, no XPT measure as well as APT Tx measurement
         {
            //****************************************************************//
            // OpCode 0x800: DATA - Load Calc DPD table Action Processing
            //****************************************************************//
            for(int  modeIndex = 0; modeIndex < 2; modeIndex++)  // modeIndex = 0: EPT, 1: ET
            {
               if (modeIndex == EPT_Mode && sweep_param.EPT_Fcomp_Meas_Pwr_List.size() == 0) continue;
               if (modeIndex == ET_Mode  && sweep_param.ET_Fcomp_Meas_Pwr_List.size()  == 0) continue;

               unsigned char bEPT = (modeIndex == 0 )? true:false;
               Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;
               Sweep[recordIndex].resExpected   = true;
               RFCal_Helper_setOpCodeData(Sweep[recordIndex],cal_param,  sweep_param,
                                          DATA_OPCODE_ACTION_LOAD_CALC_DPD /* OpCode Data Option */, demo_param.TECH, demo_param.BAND, bEPT);
               if (modeIndex == 0)
               {
                  Sweep[recordIndex].data.loadCalcDpd.loadAction = LOAD_CALC_DPD_ACTION_LOAD_ISC;
               }
               else if (modeIndex == 1 && sweep_param.EPT_Fcomp_Meas_Pwr_List.size() > 0)
               {
                  Sweep[recordIndex].data.loadCalcDpd.loadAction = LOAD_CALC_DPD_ACTION_LOAD_SC;
               }
                    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
                }
              //****************************************************************//
              //     Find segment in Sweep 2 which obtaines DPD results
              //****************************************************************//
              int SegNum_ET_AMAMPM_Response_Sweep2 = -1;
              int SegNum_EPT_AMAMPM_Response_Sweep2 = -1;
              int SegNum_Detrough_Response_Sweep2 = -1;
            byte iSwp2Index = 1;
              for (UINT i=0; i < AllSweeps[iSwp2Index].size(); i++)
              {
                  if ( AllSweeps[iSwp2Index][i].dataRes.measDpdRes[0].measured &&  AllSweeps[iSwp2Index][i].dataRes.measDpdRes[0].xptMode == 1)// EPT
                      SegNum_EPT_AMAMPM_Response_Sweep2 = i;

                  if ( AllSweeps[iSwp2Index][i].dataRes.measDpdRes[0].measured &&  AllSweeps[iSwp2Index][i].dataRes.measDpdRes[0].xptMode == 2)// ET
                      SegNum_ET_AMAMPM_Response_Sweep2 = i;

                  if ( AllSweeps[iSwp2Index][i].dataRes.measLoadEPDTRes[0].detValid)
                      SegNum_Detrough_Response_Sweep2 = i;
              }
              if ( sweep_param.EPT_Fcomp_Meas_Pwr_List.size() > 0 && SegNum_EPT_AMAMPM_Response_Sweep2 == -1 )  
                  throw RFCal_Helper_Exception("EPT AMAM/AMPM result segment in sweep 2 is not found.",  __FILE__, __LINE__ );

              if ( sweep_param.ET_Fcomp_Meas_Pwr_List.size() > 0 && SegNum_ET_AMAMPM_Response_Sweep2 == -1 ) 
                  throw RFCal_Helper_Exception("ET AMAM/AMPM result segment in sweep 2 is not found.",  __FILE__, __LINE__ );

              if ( sweep_param.ET_Fcomp_Meas_Pwr_List.size() > 0 && SegNum_Detrough_Response_Sweep2 == -1) 
                  throw RFCal_Helper_Exception("Detrough result segment in sweep 2 is not found.",  __FILE__, __LINE__ );
            //****************************************************************//
            // Meas EPT/ET Segments (EPT first then, ET if EPT is used)
            //****************************************************************//

            for(int modeIndex = 0; modeIndex <2; modeIndex++)
            {
               if (modeIndex == EPT_Mode && sweep_param.EPT_Fcomp_Meas_Pwr_List.size() == 0) continue;
               if (modeIndex == ET_Mode  && sweep_param.ET_Fcomp_Meas_Pwr_List.size()  == 0) continue;

               unsigned char bAddFudgeSegment = true;//(modeIndex == ET_Mode)? true: true;

               int rgiIndexSize = (modeIndex == EPT_Mode)? sweep_param.EPT_Fcomp_Meas_Pwr_List.size() :sweep_param.ET_Fcomp_Meas_Pwr_List.size();
               int SegNum_AMAMPM_Response_Sweep2 =  (modeIndex == EPT_Mode)? SegNum_EPT_AMAMPM_Response_Sweep2:SegNum_ET_AMAMPM_Response_Sweep2;
               for(int rgiIndex = 0; rgiIndex <rgiIndexSize; rgiIndex++)
               {
                  int NumLoops = (bAddFudgeSegment)? 2: 1;
                  byte TxPurpose;
                  for (int i = 0; i < NumLoops; i ++) // Fudge Segment loop
                  {
                     /* ESC Parameters */
                     if (bAddFudgeSegment)      /* Fudge Segment */
                     {
                        Sweep[recordIndex].subSwpType = -1;
                        Sweep[recordIndex].measSeg = false;
                        TxPurpose = 0;
                     }
                     else                 /* Regular Segment */
                     {
                        Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_APPLY_DPD;
                        Sweep[recordIndex].measSeg = true;
                        TxPurpose = FTM_Tx_Purpose_FreqComp_Lin_vs_Freq;
                     }

                            if (!bAddFudgeSegment && (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN))
                     {
                        //****************************************************************//
                        //    OpCode 0x10000:   Configure WTR
                        //****************************************************************//
                        RFCal_Helper_ESC_ET_ConfigureFB( Sweep[recordIndex], pDev, true, FBmode, fbDev, fbGainState, 
                                                         cal_param.Swp[0].XPT_Swp1_PA_State, cal_param.Swp[0].XPT_Swp1_RGI_Delta_Ref_RGI);

                        //****************************************************************//
                        //    OpCode 0x4020 = (OpCode 0x20 | OpCode 0x4000): Rx config and measurement for Tx
                        //****************************************************************//
                        RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], 0 /* LNA_state */ ,  2 /* RxFlag */, 0 , 0 /* exRxAGC */ , 1 /* rx_meas_action */);
                     }
                     if (!bAddFudgeSegment && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
                        {
                            //***************************************************************************************//
                            //      OpCode 0x8000: TxMeas. FBGain Measurement 
                            //***************************************************************************************//
                        RFCal_Helper_setOpCode_FBMeasurement(sweep_param,  Sweep[recordIndex], 0, 0, 0, NO_OVERLAP,   
                        APTV6CharToPwr, ChanFBGainStateToPwrFBGainParams);
                        }

                     //****************************************************************//
                     // OpCode 0x200: TxOverride
                     //****************************************************************//
                     RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], TxPurpose, cal_param.TxDeviceFlag, ((modeIndex == EPT_Mode)?XPT_TYPE_EPT:XPT_TYPE_ET), ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                                       ESC_TX_PWR_OVERRIDE_ABS_PWR /* pwrOverrideType */,       0 /* pwrParamMask */,
                                                       NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,NULL,NULL,
                                                       (word)((modeIndex == EPT_Mode)?sweep_param.EPT_Fcomp_Meas_Pwr_List[rgiIndex]*100.0:sweep_param.ET_Fcomp_Meas_Pwr_List[rgiIndex]*100.0),sweep_param.XPT_Fcomp_Ref_Channel_List[0]);

                     TE_Records[recordIndex].m_ul_lvl = 30;

                     // APPLY PIN Compensation is required
                     Sweep[recordIndex].txParams.flag = 1; // Apply PIN Compensation.Only FreqComp requires this parameter.
                     // Channel informaiton for NV Generation
                     Sweep[recordIndex].radio_cfg.chan.numChans               = 1;
                     Sweep[recordIndex].radio_cfg.chan.chanInfo[0].ulChan     = (dword) sweep_param.Tx_Channel_List[chanIndex];

                     //****************************************************************//
                     //  Add DPD info from Sweep2 (this is for NV generation)
                     //****************************************************************//
                     Sweep[recordIndex].txParams.txParams[devId].paState = (modeIndex == EPT_Mode)?cal_param.Swp[iSwp2Index].XPT_Swp2_EPT_PA_State:cal_param.Swp[iSwp2Index].XPT_Swp2_ET_PA_State;   
                     Sweep[recordIndex].txParams.txParams[devId].quiescentCurrent= cal_param.PA_Param.IcqEt; // (modeIndex == EPT_Mode)?cal_param.PA_Param.IcqEpt:cal_param.PA_Param.IcqEt;
    
                  //   if(modeIndex == EPT_Mode)
                     {
                         Sweep[recordIndex].dataRes.measDpdRes[0].dpdValid =  AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].dpdValid;
                         Sweep[recordIndex].dataRes.measDpdRes[0].measured =  AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].measured;
                         Sweep[recordIndex].dataRes.measDpdRes[0].xptMode =  AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].xptMode;

                        Sweep[recordIndex].txParams.txParams[devId].rgi   = (byte) AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].rgi;
                        Sweep[recordIndex].txParams.txParams[devId].bias = AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].smpsBias;
                         for( unsigned short dpdIdx = 0; dpdIdx < QMSL_XPT_CAL_PREDIST_SIZE; dpdIdx++ )
                         {
                             Sweep[recordIndex].dataRes.measDpdRes[0].amamRes[dpdIdx] = AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].amamRes[dpdIdx]; 
                             Sweep[recordIndex].dataRes.measDpdRes[0].ampmRes[dpdIdx] = AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].ampmRes[dpdIdx]; 
                         }
                     } 
                     //else // Calc // As of now, no Calc mode.
                     //{
                        // Sweep[recordIndex].dataRes.calcDpdRes[0].dpdValid =  AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].dpdValid;
                        // Sweep[recordIndex].dataRes.calcDpdRes[0].measured =  AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].measured;
                        // Sweep[recordIndex].dataRes.calcDpdRes[0].xptMode =  AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].xptMode;

                        // Sweep[recordIndex].txParams.txParams[0].rgi  = (byte) AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].rgi;
                        // Sweep[recordIndex].txParams.txParams[0].bias = AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].smpsBias;

                        // for( unsigned short dpdIdx = 0; dpdIdx < QMSL_XPT_CAL_PREDIST_SIZE; dpdIdx++ )
                        // {
                           //  Sweep[recordIndex].dataRes.calcDpdRes[0].amamRes[dpdIdx] =   AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].amamRes[dpdIdx]; 
                           //  Sweep[recordIndex].dataRes.calcDpdRes[0].ampmRes[dpdIdx] =   AllSweeps[iSwp2Index][SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].ampmRes[dpdIdx]; 
                        // }
                     //}
                     if (modeIndex == ET_Mode)  // Detrough
                     {
                         Sweep[recordIndex].dataRes.measLoadEPDTRes[0].detValid = AllSweeps[iSwp2Index][SegNum_Detrough_Response_Sweep2].dataRes.measLoadEPDTRes[rgiIndex].detValid;
                         Sweep[recordIndex].dataRes.measLoadEPDTRes[0].measured = AllSweeps[iSwp2Index][SegNum_Detrough_Response_Sweep2].dataRes.measLoadEPDTRes[rgiIndex].measured;
                         if (Sweep[recordIndex].dataRes.measLoadEPDTRes[0].detValid == 0)
                             throw RFCal_Helper_Exception("In Sweep 2 result. Detrough results are not obtained.",  __FILE__, __LINE__ );

                         if (Sweep[recordIndex].dataRes.measLoadEPDTRes[0].measured)
                         {
                             for( unsigned short detIdx = 0; detIdx < QMSL_XPT_CAL_EXPANDED_DET_SIZE; detIdx++ )
                             {
                                 Sweep[recordIndex].dataRes.measLoadEPDTRes[0].epdtTbl[detIdx] =  AllSweeps[iSwp2Index][SegNum_Detrough_Response_Sweep2].dataRes.measLoadEPDTRes[rgiIndex].epdtTbl[detIdx];
                             }
                         }
                         else
                         {
                             for( unsigned short detIdx = 0; detIdx < QMSL_XPT_CAL_EXPANDED_DET_SIZE; detIdx++ )
                             {
                                 Sweep[recordIndex].dataRes.calcLoadEPDTRes[0].epdtTbl[detIdx] =  AllSweeps[iSwp2Index][SegNum_Detrough_Response_Sweep2].dataRes.measLoadEPDTRes[rgiIndex].epdtTbl[detIdx];
                             }
                         }
                     }

                            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
                     bAddFudgeSegment = false;
                  }
               } // for(int rgiIndex = 0; rgiIndex < sizeof(ET_Fcomp_Meas_Pwr_List)/sizeof(double); rgiIndex++)
            } // for(int modeIndex = 0; modeIndex <2; modeIndex++)
         } // if (sweep_param.Meas_Tx) // If Meas_Tx is false, no XPT measure as well as APT Tx measurement
      } // if (sweep_param.Sweep_Type == XPT_FREQCOMP)
   }

    //*****************************************************************************************//
    //  OpCode 0x10000: FBSetup     Feedback Action:    4   ( | Bit 2 = Configure RxLM for FB AGC read ) 
    //*****************************************************************************************//
   FBmode = false;  
   RFCal_Helper_ESC_ET_OpCodeConfigureRxLM(demo_param, cal_param, sweep_param, Sweep, recordIndex, FBmode);
   RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
   
   //*****************************************************************************************/
   //    End of FreqComp: Insert a low rgi segment instead of tear down
   //*****************************************************************************************/
   RFCal_Helper_setLowRGISegment(Sweep[recordIndex],cal_param.TxDeviceFlag);

    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

   //*********************************************//
   //    Generate OpCode for all the segment
   //*********************************************//
   Sweep.resize(recordIndex); // Delete unused vector.
   RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());
   return true;
}

unsigned char RFCal_Helper_ESC_ET_createINT_CAL_XPTSwp3(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, Sweep_Params & sweep_param,
    const vector < cal_sweep_record > & Sweep1, const vector < cal_sweep_record > & Sweep2, vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
   int   Swp2 = 1;

   unsigned char bUseInterpolatedBiasIcq = false;
   byte pDev = RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag);
   for (UINT i=0; i < sweep_param.Pre_Sweep_Computation.size(); i++)
   {
      if ( sweep_param.Pre_Sweep_Computation[i] == 2)
      {
         bUseInterpolatedBiasIcq = true;
         break;
      }
   }
   //****************************************************************//
   // PreComputation for Sweep 3. Interpolate Bias and ICQ
   //****************************************************************//
   if (bUseInterpolatedBiasIcq)
   {
      // ****************************************************** //
      //    Look up full bias data
      // ****************************************************** //

      map < pair < byte, byte >, double > Pa_Rgi_To_FullBiasPower;

      for (UINT i = 0; i < Sweep1.size(); i++)
      {
         if (Sweep1[i].txPurpose == 3)
         {
            // only when Full Bias Sweep at Sweep 1
            byte devId = sweep_param.Device_Selection.Master_Device_ID;
            byte paState = Sweep1[i].txParams.txParams[devId].paState;
            byte rgi = Sweep1[i].txParams.txParams[devId].rgi;
            double power =  static_cast<double>(static_cast<signed short>(Sweep1[i].txRes.txPower)/10.0);
            Pa_Rgi_To_FullBiasPower[make_pair(paState,rgi)] = power;
         }
      }

      for (UINT i = 0;  i< sweep_param.PA_State_List.size();   i++)
      {
         // ****************************************************** //
         //    Look up APT char data
         // ****************************************************** //
         byte device = sweep_param.Device_Selection.Master_Device_ID;
         byte pa = sweep_param.PA_State_List[i];
         byte tx_purpose = sweep_param.Purpose_List[i];

         APT_CharV3_Table_Key AptCharTablekey(cal_param.RF_Mode,device,(word)sweep_param.Ref_Channel,pa,tx_purpose);

         if (cal_param.APTCharV3[AptCharTablekey].size() == 0)
         {
            throw RFCal_Helper_Exception("APT char does not include all the PA data.",  __FILE__, __LINE__ );
         }

         // ****************************************************** //
         //    Assign char data into map
         // ****************************************************** //

         map <double, unsigned long> charPwrIcq;
         map <double, unsigned long> charPwrVcc;

         for (UINT index = 0; index < cal_param.APTCharV3[AptCharTablekey].size(); index++)
         {
            double power = cal_param.APTCharV3[AptCharTablekey][index].pwr;
            int bias = static_cast<int>( cal_param.APTCharV3[AptCharTablekey][index].vcc);
            int icq = static_cast<int>( cal_param.APTCharV3[AptCharTablekey][index].icq);
            charPwrVcc[power] = bias;
            charPwrIcq[power] = icq;
         }

         // ****************************************************** //
         //    Assign Interpolate Bias and next highest ICQ to sweep parameter
         // ****************************************************** //
         UINT iSizeRGI = sweep_param.RGI_List[i].size();
         sweep_param.Bias_List[i].resize(iSizeRGI);
         sweep_param.PA_Quiescent_Current[i].resize(iSizeRGI);

         for (UINT rgiIndex = 0; rgiIndex < iSizeRGI;  rgiIndex++)
         {
            unsigned long tmp;
            double aPower = Pa_Rgi_To_FullBiasPower[make_pair(sweep_param.PA_State_List[i],sweep_param.RGI_List[i][rgiIndex])];

            Find_Interpolated_Y(charPwrVcc, aPower, tmp);
            sweep_param.Bias_List[i][rgiIndex] = static_cast<word>(tmp);

            Find_Y_of_Next_Highest_X(charPwrIcq, aPower, tmp);
            sweep_param.PA_Quiescent_Current[i][rgiIndex] = static_cast<byte>(tmp);
         }
      }
   }  /* End of precomputation */

   //***********************************************************//
   //    Create Sweep 3 records
   //***********************************************************//

   unsigned char  bOK = true;
   unsigned char  FBmode = false;
   const int      fbDev       =  1;
   const int      fbEnabled      =  1;
   const int      fbDisabled     =  0;
   int            fbGainState    =  0;
   const unsigned char ENABLED      = true;
   const unsigned char DISABLED  = false;

   int            Feedback_Device_ID         = static_cast<int>(sweep_param.Device_Selection.Feedback_Device_ID);   

   UINT recordIndex = 0;
   int txParamMask_PwrOverride  = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN | ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT); // Sweep

    map <RFCal_Helper_Tx_Params, double > APTV6CharToPwr;
    map < pair < dword, byte >, Tx_RefCal_Data_V2_Record > ChanFBGainStateToPwrFBGainParams;

    if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
    {
      RFCal_Helper_ESC_ET_Tx_AptChar_DataV6_Assignment_XML(demo_param,  cal_param, sweep_param, APTV6CharToPwr); // Key: Channel. pa_state, rgi
      RFCal_Helper_ESC_ET_FBGain_Ref_Assignment_XML(demo_param,  cal_param, sweep_param, ChanFBGainStateToPwrFBGainParams);
    }

   //*****************************************************************************************//
   //  OpCode 0x1000: Sync OpCode (LTE Only)
   //*****************************************************************************************//

   if (demo_param.TECH == 3 && sweep_param.Sweep_Type == XPT_SWP3)
   {
      RFCal_Helper_setOpCode_Sync(Sweep[recordIndex]);
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
      Sweep[recordIndex].subSwpType = -1;
      Sweep[recordIndex].isNopSeg = 0;
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
   }

    if (!FBmode || sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
   {
      //*****************************************************************************************//
      //  OpCode 0x10000: FBSetup   Feedback Action:  4  ( | Bit 2 = Configure RxLM for FB AGC read )
      //*****************************************************************************************//
      FBmode = true;
      RFCal_Helper_ESC_ET_OpCodeConfigureRxLM(demo_param, cal_param, sweep_param, Sweep, recordIndex, FBmode);
      RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
      RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length, sweep_param.Segment_Length[0][0] );
   }

   //****************************************************************//
   // APT Linearizer (For PA for APTs)
   //****************************************************************//
   for(UINT paIndex = 0; paIndex < sweep_param.PA_State_List.size() ; paIndex++)
   {
      for (UINT rgiIndex = 0; rgiIndex < sweep_param.RGI_List[paIndex].size(); rgiIndex ++)
      {
            if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN)
            {
         //*********************************************//
         //    OpCode 0x10000:   Configure WTR
         //*********************************************//
            RFCal_Helper_ESC_ET_ConfigureFB(Sweep[recordIndex], pDev, true, FBmode, fbDev, fbGainState, sweep_param.PA_State_List[paIndex],sweep_param.Ref_RGI_List[paIndex][rgiIndex]);

         //*********************************************//
         //    OpCode 0x4020 = (OpCode 0x20 | OpCode 0x4000): Rx config and measurement for Tx
         //*********************************************//
         RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], 0 /* LNA_state */ ,  2 /* RxFlag */, 0 , 0 /* exRxAGC */ , 1 /* rx_meas_action */);
            }


         //***************************************************************************************//
         //    OpCode 0x200: TxOverride
         //***************************************************************************************//
         Sweep[recordIndex].measSeg = true;

         RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],sweep_param.Purpose_List[paIndex],
                                           cal_param.TxDeviceFlag, XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                           ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,        txParamMask_PwrOverride /* pwrParamMask */,
                                           sweep_param.PA_State_List[paIndex], sweep_param.RGI_List[paIndex][rgiIndex],
                                           sweep_param.Bias_List[paIndex][rgiIndex],
                                           ESC_TX_OVERRIDE_ACTION_SW_DEF, 0 /* IQ_Gain */,
                                           ESC_TX_OVERRIDE_ACTION_SW_DEF, 0 /* Env_Scale */,
                                           sweep_param.PA_Quiescent_Current[paIndex][rgiIndex],
                                           ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);

         if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
         {
            //***************************************************************************************//
            //      OpCode 0x8000: TxMeas. FBGain Measurement 
            //***************************************************************************************//
            RFCal_Helper_setOpCode_FBMeasurement(sweep_param,  Sweep[recordIndex], sweep_param.PA_State_List[paIndex], sweep_param.RGI_List[paIndex][rgiIndex], sweep_param.Ref_RGI_List[paIndex][rgiIndex], NO_OVERLAP,   
            APTV6CharToPwr, ChanFBGainStateToPwrFBGainParams);
         }
         double MaxExpectedPwr = sweep_param.Max_Expected_Pwr[paIndex]- floor(rgiIndex*0.6);
         if (APTV6CharToPwr.size() > 0)
         {
            cal_sweep_var_len_tx_params * txRecord = &Sweep[recordIndex].txParams.txParams[sweep_param.Device_Selection.Master_Device_ID];
            RFCal_Helper_Tx_Params txParam;

            txParam.Channel = sweep_param.Ref_Channel;
            txParam.PaState = txRecord->paState;
            txParam.RGI = txRecord->rgi;
            txParam.Bias = txRecord->bias;
            txParam.PaQuiescentCurrent = txRecord->quiescentCurrent;
            txParam.IqGain = txRecord->iqGain;
            txParam.EnvScale = txRecord->envScale;
            txParam.delayVal = txRecord->delay;
            txParam.XPTmode = txRecord->xptMode;

            if (APTV6CharToPwr.find(txParam) != APTV6CharToPwr.end())
            {
               MaxExpectedPwr = APTV6CharToPwr[txParam] + 5.0;
            }
            else // Traditional case
            {
               RFCal_Helper_Tx_Params txParam2;
               txParam2.Channel = sweep_param.Ref_Channel;
               txParam2.RGI = txParam.RGI;
               txParam2.PaState = txParam.PaState;
      
               if (APTV6CharToPwr.find(txParam2) != APTV6CharToPwr.end())
               {
                  MaxExpectedPwr = APTV6CharToPwr[txParam2] + 5.0;
               }
            }     
         }
            TE_Records[recordIndex].m_ul_lvl = MaxExpectedPwr;

         RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);    

         ////***************************************************************************************//
         ////       Overlap
         ////***************************************************************************************//

         //if( sweep_param.Do_FBGain_Cal && sweep_param.FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld_List.front() != 0)
         //{
         //   int tmpfbGainState =  static_cast<int>(Sweep[recordIndex - 1].txMeasParams.txMeas[0].txMeasPayload.fbGainParams.fbGainState);
         //   for(int ovrlp_iter = OVERLAP_MINUS; ovrlp_iter <= OVERLAP_PLUS; ++ovrlp_iter)
         //   {
         //      //***************************************************************************************//
         //      //     OpCode 0x8000: TxMeas. FBGain Measurement 
         //      //***************************************************************************************//
         //      cal_sweep_record tmpSwpRecord;
         //      tmpSwpRecord = Sweep[recordIndex - 1];
         //      RFCal_Helper_setOpCode_FBMeasurement(sweep_param,  tmpSwpRecord, sweep_param.PA_State_List[paIndex], sweep_param.RGI_List[paIndex][rgiIndex], sweep_param.Ref_RGI_List[paIndex][rgiIndex], ovrlp_iter, 
         //      APTV6CharToPwr, ChanFBGainStateToPwrFBGainParams);

         //      if(tmpSwpRecord.txMeasParams.txMeas[0].txMeasPayload.fbGainParams.fbGainState != tmpfbGainState && ovrlp_iter != NO_OVERLAP)
         //      {
         //         Sweep[recordIndex] = tmpSwpRecord;
         //         TE_Records[recordIndex] = TE_Records[recordIndex-1];
         //         RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         //      }
         //   }
         //}

         //***************************************************************************************//
         //    OpCode 0x8000: TxMeasure (HDET Measure) (Seperate HDET and TxOverride)
         //***************************************************************************************//
         if (sweep_param.HDET_Avgs.size() > 0 && sweep_param.HDET_Avgs[paIndex] > 0)
         {
            //*********************************************//
            //    OpCode 0x10000:   Configure WTR
            //*********************************************//
            RFCal_Helper_ESC_ET_ConfigureFB( Sweep[recordIndex],pDev, false, FBmode, fbDev, fbGainState, sweep_param.PA_State_List[paIndex],sweep_param.RGI_List[paIndex][rgiIndex]);

            RFCal_Helper_setOpCode_TxMeasure(Sweep[recordIndex],Sweep[recordIndex].txParams.txDevice, (sweep_param.HDET_Avgs[paIndex]>0)?1:0, (byte)sweep_param.HDET_Avgs[paIndex]);
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         }
      }
      //****************************************************************//
      // Insert NoP (OpCode 0x0) if MaxExpectedPwr is changed.
      //****************************************************************//
      if (paIndex == (sweep_param.PA_State_List.size() - 1) || sweep_param.Max_Expected_Pwr[paIndex] != sweep_param.Max_Expected_Pwr[paIndex+1])
      {
         RFCal_helper_InsertNop(Sweep,recordIndex, 1);
      }
   }
    if (sweep_param.Sweep_Type == INT_CAL_XPT_SWP3)
   {
      const int EPT_Mode = 0;
      const int ET_Mode = 1;

      if (cal_param.En_EPTET_Different_PAstate == 0)
      {
         //****************************************************************//
         // OpCode 0x800: DATA - Load Calc DPD table Action Processing
         //****************************************************************//

         for(int  modeIndex = 0; modeIndex < 2; modeIndex++)  // modeIndex = 0: EPT, 1: ET
         {
            if (modeIndex == EPT_Mode && sweep_param.EPT_Swp3_Meas_Pwr_List.size() == 0) continue;  // if no EPT, skip EPT
            if (modeIndex == ET_Mode  && sweep_param.ET_Swp3_Meas_Pwr_List.size()  == 0) continue;  // if no ET, skip ET

            unsigned char bEPT = ( modeIndex == EPT_Mode );
            Sweep[recordIndex].subSwpType                   = ESC_SUB_SWEEP_DEFAULT;
            Sweep[recordIndex].resExpected                     = true;
            RFCal_Helper_setOpCodeData(Sweep[recordIndex],cal_param,  sweep_param,
                                       DATA_OPCODE_ACTION_LOAD_CALC_DPD /* OpCode Data Option */, demo_param.TECH, demo_param.BAND, bEPT);
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         }
      }
      //****************************************************************//
      //  Find segment in Sweep 2 which obtaines DPD results
      //****************************************************************//
      int SegNum_ET_AMAMPM_Response_Sweep2 = -1;
      int SegNum_EPT_AMAMPM_Response_Sweep2 = -1;
      int SegNum_Detrough_Response_Sweep2 = -1;

      for (UINT i=0; i < Sweep2.size(); i++)
      {
         if (Sweep2[i].dataRes.measDpdRes[0].measured && Sweep2[i].dataRes.measDpdRes[0].xptMode == 1)// EPT
            SegNum_EPT_AMAMPM_Response_Sweep2 = i;

         if (Sweep2[i].dataRes.measDpdRes[0].measured && Sweep2[i].dataRes.measDpdRes[0].xptMode == 2)// ET
            SegNum_ET_AMAMPM_Response_Sweep2 = i;

         if (Sweep2[i].dataRes.measLoadEPDTRes[0].detValid)
            SegNum_Detrough_Response_Sweep2 = i;
      }
      if ( sweep_param.EPT_Swp3_Meas_Pwr_List.size() > 0 && SegNum_EPT_AMAMPM_Response_Sweep2 == -1 )
         throw RFCal_Helper_Exception("EPT AMAM/AMPM result segment in sweep 2 is not found.",  __FILE__, __LINE__ );

      if ( sweep_param.ET_Swp3_Meas_Pwr_List.size() > 0 && SegNum_ET_AMAMPM_Response_Sweep2 == -1 )
         throw RFCal_Helper_Exception("ET AMAM/AMPM result segment in sweep 2 is not found.",  __FILE__, __LINE__ );

      if ( sweep_param.ET_Swp3_Meas_Pwr_List.size() > 0 && SegNum_Detrough_Response_Sweep2 == -1)
         throw RFCal_Helper_Exception("Detrough result segment in sweep 2 is not found.",  __FILE__, __LINE__ );

      //****************************************************************//
      //  Swp3 Apply DPD ET Segments (Override by Power)
      //****************************************************************//
      for(int  modeIndex = 0; modeIndex < 2; modeIndex++)  // modeIndex = 0: EPT, 1: ET
      {
         if (cal_param.En_EPTET_Different_PAstate == 1)
         {
            //****************************************************************//
            // OpCode 0x800: DATA - Load Calc DPD table Action Processing
            //****************************************************************//

            if (modeIndex == EPT_Mode && sweep_param.EPT_Swp3_Meas_Pwr_List.size() == 0) continue;  // if no EPT, skip EPT
            if (modeIndex == ET_Mode  && sweep_param.ET_Swp3_Meas_Pwr_List.size()  == 0) continue;  // if no ET, skip ET

            unsigned char bEPT = ( modeIndex == EPT_Mode );
            Sweep[recordIndex].subSwpType                   = ESC_SUB_SWEEP_DEFAULT;
            Sweep[recordIndex].resExpected                     = true;
            RFCal_Helper_setOpCodeData(Sweep[recordIndex],cal_param,  sweep_param,
                                       DATA_OPCODE_ACTION_LOAD_CALC_DPD /* OpCode Data Option */, demo_param.TECH, demo_param.BAND, bEPT);
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         }

         if (modeIndex == EPT_Mode && sweep_param.EPT_Swp3_Meas_Pwr_List.size() == 0) continue;   // if no EPT, skip EPT
         if (modeIndex == ET_Mode  && sweep_param.ET_Swp3_Meas_Pwr_List.size()  == 0) continue;   // if no ET, skip ET

         for (int calcModeIndex = 0; calcModeIndex < 2; calcModeIndex++)
         {
            const int Meas_Mode = 0;
            const int Calc_Mode = 1;

            if (modeIndex == ET_Mode && calcModeIndex == Calc_Mode) continue;

            vector < double > * pXPT_Pwr_List;
            int SegNum_AMAMPM_Response_Sweep2;
            if (modeIndex == EPT_Mode)
            {
               SegNum_AMAMPM_Response_Sweep2 = SegNum_EPT_AMAMPM_Response_Sweep2;
               if (calcModeIndex == Calc_Mode)
               {
                        if (sweep_param.EPT_Swp3_Calc_Pwr_List.size() == 0) continue;
                  pXPT_Pwr_List = (vector < double > *)& sweep_param.EPT_Swp3_Calc_Pwr_List;
               }
               else
               {
                  pXPT_Pwr_List = (vector < double > *)& sweep_param.EPT_Swp3_Meas_Pwr_List;
               }
            }
            else     // ET mode
            {
               SegNum_AMAMPM_Response_Sweep2 = SegNum_ET_AMAMPM_Response_Sweep2;
               pXPT_Pwr_List = (vector < double > *)& sweep_param.ET_Swp3_Meas_Pwr_List;
            }

            UINT iRgiIndexSize = pXPT_Pwr_List->size();

            //****************************************************************//
            //  Add a fudge segment
            //****************************************************************//
            Sweep[recordIndex].subSwpType = -1;       // Make the previous segment as a fudge segment
            Sweep[recordIndex].measSeg = false;

            //****************************************************************//
            //  OpCode 0x200 (bit 9 enabled): TxOverride
            //****************************************************************//
            RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],sweep_param.Purpose_List[0], cal_param.TxDeviceFlag,
                                              (modeIndex == EPT_Mode)?      XPT_TYPE_EPT:XPT_TYPE_ET, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                              ESC_TX_PWR_OVERRIDE_ABS_PWR /* pwrOverrideType */,       0 /* pwrParamMask */,
                                              NULL, NULL, NULL, NULL, NULL, NULL,NULL,NULL,NULL,NULL,
                                              static_cast<word>(pXPT_Pwr_List->at(0)*100.0), sweep_param.Ref_Channel);
            Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_APPLY_DPD;
            Sweep[recordIndex].txIntCalParams.paState = (modeIndex == EPT_Mode)?cal_param.Swp[0].XPT_Swp1_PA_State_EPT: cal_param.Swp[0].XPT_Swp1_PA_State_ET;
            if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
            {
               //***************************************************************************************//
               //      OpCode 0x8000: TxMeas. FBGain Measurement 
               //***************************************************************************************//
               RFCal_Helper_setOpCode_FBMeasurement(sweep_param,  Sweep[recordIndex], 0, 0, 0, NO_OVERLAP,   
               APTV6CharToPwr, ChanFBGainStateToPwrFBGainParams);
            }


                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
            FBmode = false;

            for(UINT rgiIndex = 0; rgiIndex < iRgiIndexSize ; rgiIndex++)
            {
               Sweep[recordIndex].measSeg = true;
               Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_APPLY_DPD;

               word tmpExpectedPwr =  static_cast<word>(pXPT_Pwr_List->at(rgiIndex)*100.0);

                    if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN)
                    {
               //*********************************************//
               //    OpCode 0x10000:   Configure WTR
               //*********************************************//
                  RFCal_Helper_ESC_ET_ConfigureFB(Sweep[recordIndex],pDev, true, FBmode, fbDev, fbGainState, 0,cal_param.Swp[0].XPT_Swp1_RGI_Delta_Ref_RGI);

               //*********************************************//
               //    OpCode 0x4020 = (OpCode 0x20 | OpCode 0x4000): Rx config and measurement for Tx
               //*********************************************//
               RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], 0 /* LNA_state */ ,  2 /* RxFlag */, 0 , 0 /* exRxAGC */ , 1 /* rx_meas_action */);
                    }
               //****************************************************************//
               //  OpCode 0x200 (bit 9 enabled): TxOverride
               //****************************************************************//
               RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],sweep_param.Purpose_List[0], cal_param.TxDeviceFlag, (modeIndex == EPT_Mode)?XPT_TYPE_EPT:XPT_TYPE_ET, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                                 ESC_TX_PWR_OVERRIDE_ABS_PWR /* pwrOverrideType */,       0 /* pwrParamMask */,
                                                 NULL, NULL, NULL, NULL, NULL, NULL,NULL,NULL,NULL,NULL,
                                                 tmpExpectedPwr, sweep_param.Ref_Channel);
                Sweep[recordIndex].txIntCalParams.paState = (modeIndex == EPT_Mode)?cal_param.Swp[0].XPT_Swp1_PA_State_EPT: cal_param.Swp[0].XPT_Swp1_PA_State_ET;
                  TE_Records[recordIndex].m_ul_lvl = static_cast<double>(tmpExpectedPwr/100.0 + 5.0);
               if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
                    {
                        //***************************************************************************************//
                        //      OpCode 0x8000: TxMeas. FBGain Measurement 
                        //***************************************************************************************//
                  RFCal_Helper_setOpCode_FBMeasurement(sweep_param,  Sweep[recordIndex], 0, 0, 0, NO_OVERLAP,   
                  APTV6CharToPwr, ChanFBGainStateToPwrFBGainParams);
                    }

               //****************************************************************//
               //  Add DPD info from Sweep2 (this is for NV generation)
               //****************************************************************//
               byte devId = sweep_param.Device_Selection.Master_Device_ID;
               Sweep[recordIndex].txParams.txParams[devId].paState = (modeIndex == EPT_Mode)?cal_param.Swp[Swp2].XPT_Swp2_EPT_PA_State:cal_param.Swp[Swp2].XPT_Swp2_ET_PA_State;   
               Sweep[recordIndex].txParams.txParams[devId].quiescentCurrent= cal_param.PA_Param.IcqEt; // (modeIndex == EPT_Mode)?cal_param.PA_Param.IcqEpt:cal_param.PA_Param.IcqEt;

               if(calcModeIndex == Meas_Mode)
               {
                  Sweep[recordIndex].dataRes.measDpdRes[0].dpdValid =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].dpdValid;
                  Sweep[recordIndex].dataRes.measDpdRes[0].measured =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].measured;
                  Sweep[recordIndex].dataRes.measDpdRes[0].xptMode =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].xptMode;

                  Sweep[recordIndex].txParams.txParams[devId].rgi   = (byte) Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].rgi;
                  Sweep[recordIndex].txParams.txParams[devId].bias = Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].smpsBias;
                  for( unsigned short dpdIdx = 0; dpdIdx < QMSL_XPT_CAL_PREDIST_SIZE; dpdIdx++ )
                  {
                     Sweep[recordIndex].dataRes.measDpdRes[0].amamRes[dpdIdx] =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].amamRes[dpdIdx];
                     Sweep[recordIndex].dataRes.measDpdRes[0].ampmRes[dpdIdx] =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].ampmRes[dpdIdx];
                  }
               }
               else // Calc
               {
                  Sweep[recordIndex].dataRes.calcDpdRes[0].dpdValid =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].dpdValid;
                  Sweep[recordIndex].dataRes.calcDpdRes[0].measured =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].measured;
                  Sweep[recordIndex].dataRes.calcDpdRes[0].xptMode =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].xptMode;

                  Sweep[recordIndex].txParams.txParams[devId].rgi   = (byte) Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].rgi;
                  Sweep[recordIndex].txParams.txParams[devId].bias = Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].smpsBias;

                  for( unsigned short dpdIdx = 0; dpdIdx < QMSL_XPT_CAL_PREDIST_SIZE; dpdIdx++ )
                  {
                     Sweep[recordIndex].dataRes.calcDpdRes[0].amamRes[dpdIdx] =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].amamRes[dpdIdx];
                     Sweep[recordIndex].dataRes.calcDpdRes[0].ampmRes[dpdIdx] =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].ampmRes[dpdIdx];
                  }
               }
               if (modeIndex == ET_Mode)  // Detrough
               {
                  Sweep[recordIndex].dataRes.measLoadEPDTRes[0].detValid = Sweep2[SegNum_Detrough_Response_Sweep2].dataRes.measLoadEPDTRes[rgiIndex].detValid;
                  Sweep[recordIndex].dataRes.measLoadEPDTRes[0].measured = Sweep2[SegNum_Detrough_Response_Sweep2].dataRes.measLoadEPDTRes[rgiIndex].measured;
                  if (Sweep[recordIndex].dataRes.measLoadEPDTRes[0].detValid == 0)
                     throw RFCal_Helper_Exception("In Sweep 2 result. Detrough results are not obtained.",  __FILE__, __LINE__ );

                  if (Sweep[recordIndex].dataRes.measLoadEPDTRes[0].measured)
                  {
                     for( unsigned short detIdx = 0; detIdx < QMSL_XPT_CAL_EXPANDED_DET_SIZE; detIdx++ )
                     {
                        Sweep[recordIndex].dataRes.measLoadEPDTRes[0].epdtTbl[detIdx] =  Sweep2[SegNum_Detrough_Response_Sweep2].dataRes.measLoadEPDTRes[rgiIndex].epdtTbl[detIdx];
                     }
                  }
                  else
                  {
                     for( unsigned short detIdx = 0; detIdx < QMSL_XPT_CAL_EXPANDED_DET_SIZE; detIdx++ )
                     {
                        Sweep[recordIndex].dataRes.calcLoadEPDTRes[0].epdtTbl[detIdx] =  Sweep2[SegNum_Detrough_Response_Sweep2].dataRes.measLoadEPDTRes[rgiIndex].epdtTbl[detIdx];
                     }
                  }
               }
                    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
            }  // End of for(rgiIndex = 0; rgiIndex < sizeof(ET_Swp3_Meas_Pwr_List)/sizeof(double); rgiIndex++)
         }
      }
   }
    ////*********************************************//
    ////        OpCode 0x10000: Configure WTR 
    ////*********************************************//
 //  RFCal_Helper_ESC_ET_ConfigureFB(Sweep[recordIndex], false, FBmode, fbDev, fbGainState, 0,cal_param.Swp[0].XPT_Swp1_RGI_Delta_Ref_RGI);

   //****************************************************************//
   //  OpCode 0x8000: TxMeas: THERM READ
   //****************************************************************//
   Sweep[recordIndex].txPurpose  = (word)0;
   Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_THERM_READ;
   RFCal_Helper_setOpCode_TxMeasure(Sweep[recordIndex],cal_param.TxDeviceFlag,ESC_TX_MEAS_ACTION_THERM_READ);
    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

   //*****************************************************************************************/
   //    End of Sweep 3: Insert a low rgi segment instead of tear down
   //*****************************************************************************************/
   RFCal_Helper_setLowRGISegment(Sweep[recordIndex],cal_param.TxDeviceFlag);
    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

   //*********************************************//
   //    Generate OpCode for all the segment
   //*********************************************//
   Sweep.resize(recordIndex); // Delete unused records.
   RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());
   return true;
}

unsigned char RFCal_Helper_ESC_ET_createXPT3Swp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, Sweep_Params & sweep_param,
    const vector < cal_sweep_record > & Sweep1, const vector < cal_sweep_record > & Sweep2, vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
   int   Swp2 = 1;

   unsigned char bUseInterpolatedBiasIcq = false;

   for (UINT i=0; i < sweep_param.Pre_Sweep_Computation.size(); i++)
   {
      if ( sweep_param.Pre_Sweep_Computation[i] == 2)
      {
         bUseInterpolatedBiasIcq = true;
         break;
      }
   }
   UINT dev = 0;
   if (cal_param.Tx_Rx_Split)
   {
      dev = static_cast<UINT>(sweep_param.Device_Selection.Master_Device_ID);
   }
   else
   {
      dev = static_cast<UINT>(RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag));
   }

   //****************************************************************//
   // PreComputation for Sweep 3. Interpolate Bias and ICQ
   //****************************************************************//
   if (bUseInterpolatedBiasIcq)
   {
      // ****************************************************** //
      //    Look up full bias data
      // ****************************************************** //

      map < pair < byte, byte >, double > Pa_Rgi_To_FullBiasPower;

      for (UINT i = 0; i < Sweep1.size(); i++)
      {
         if (Sweep1[i].txPurpose == 3)
         {
            // only when Full Bias Sweep at Sweep 1
            byte paState = Sweep1[i].txParams.txParams[dev].paState;
            byte rgi = Sweep1[i].txParams.txParams[dev].rgi;
            double power =  static_cast<double>(static_cast<signed short>(Sweep1[i].txRes.txPower)/10.0);
            Pa_Rgi_To_FullBiasPower[make_pair(paState,rgi)] = power;
         }
      }

      for (UINT i = 0;  i< sweep_param.PA_State_List.size();   i++)
      {
         // ****************************************************** //
         //    Look up APT char data
         // ****************************************************** //
         byte device = sweep_param.Device_Selection.Master_Device_ID;
         byte pa = sweep_param.PA_State_List[i];
         byte tx_purpose = sweep_param.Purpose_List[i];

         APT_CharV3_Table_Key AptCharTablekey(cal_param.RF_Mode,device,(word)sweep_param.Ref_Channel,pa,tx_purpose);

         if (cal_param.APTCharV3[AptCharTablekey].size() == 0)
         {
            throw RFCal_Helper_Exception("APT char does not include all the PA data.",  __FILE__, __LINE__ );
         }

         // ****************************************************** //
         //    Assign char data into map
         // ****************************************************** //

         map <double, unsigned long> charPwrIcq;
         map <double, unsigned long> charPwrVcc;

         for (UINT index = 0; index < cal_param.APTCharV3[AptCharTablekey].size(); index++)
         {
            double power = cal_param.APTCharV3[AptCharTablekey][index].pwr;
            int bias = static_cast<int>( cal_param.APTCharV3[AptCharTablekey][index].vcc);
            int icq = static_cast<int>( cal_param.APTCharV3[AptCharTablekey][index].icq);
            charPwrVcc[power] = bias;
            charPwrIcq[power] = icq;
         }

         // ****************************************************** //
         //    Assign Interpolate Bias and next highest ICQ to sweep parameter
         // ****************************************************** //
         UINT iSizeRGI = sweep_param.RGI_List[i].size();
         sweep_param.Bias_List[i].resize(iSizeRGI);
         sweep_param.PA_Quiescent_Current[i].resize(iSizeRGI);

         for (UINT rgiIndex = 0; rgiIndex < iSizeRGI;  rgiIndex++)
         {
            unsigned long tmp;
            double aPower = Pa_Rgi_To_FullBiasPower[make_pair(sweep_param.PA_State_List[i],sweep_param.RGI_List[i][rgiIndex])];

            Find_Interpolated_Y(charPwrVcc, aPower, tmp);
            sweep_param.Bias_List[i][rgiIndex] = static_cast<word>(tmp);

            Find_Y_of_Next_Highest_X(charPwrIcq, aPower, tmp);
            sweep_param.PA_Quiescent_Current[i][rgiIndex] = static_cast<byte>(tmp);
         }
      }
   }  /* End of precomputation */

   //***********************************************************//
   //    Create Sweep 3 records
   //***********************************************************//

   unsigned char  bOK = true;
   UINT recordIndex = 0;
   int txParamMask_PwrOverride  = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN | ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT); // Sweep

   //*****************************************************************************************//
   //  OpCode 0x1000: Sync OpCode (LTE Only)
   //*****************************************************************************************//

   if (demo_param.TECH == 3 && sweep_param.Sweep_Type == XPT_SWP3)
   {
      RFCal_Helper_setOpCode_Sync(Sweep[recordIndex]);
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
      Sweep[recordIndex].subSwpType = -1;
      Sweep[recordIndex].isNopSeg = 0;
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
   }

   //****************************************************************//
   // APT Linearizer (For PA for APTs)
   //****************************************************************//
   for(UINT paIndex = 0; paIndex < sweep_param.PA_State_List.size() ; paIndex++)
   {
      for (UINT rgiIndex = 0; rgiIndex < sweep_param.RGI_List[paIndex].size(); rgiIndex ++)
      {
         //***************************************************************************************//
         //    OpCode 0x200: TxOverride
         //***************************************************************************************//
         Sweep[recordIndex].measSeg = true;

            RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],
                sweep_param.Purpose_List[paIndex], /* Tx Purpose */
                cal_param.TxDeviceFlag, /* TxDevice Flag. */
                XPT_TYPE_APT, /* XPT Mode = APT */ 
                ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */, 
                ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,           
                txParamMask_PwrOverride /* pwrParamMask */, 
                sweep_param.PA_State_List[paIndex], /* Pa State */
                sweep_param.RGI_List[paIndex][rgiIndex], /* RGI */
                sweep_param.Bias_List[paIndex][rgiIndex], /* Bias */
                                           ESC_TX_OVERRIDE_ACTION_SW_DEF, 0 /* IQ_Gain */,
                                           ESC_TX_OVERRIDE_ACTION_SW_DEF, 0 /* Env_Scale */,
                sweep_param.PA_Quiescent_Current[paIndex][rgiIndex], /* ICQ */
                                           ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);

            TE_Records[recordIndex].m_ul_lvl = sweep_param.Max_Expected_Pwr[paIndex];
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex); 

         //***************************************************************************************//
         //    OpCode 0x8000: TxMeasure (HDET Measure) (Seperate HDET and TxOverride)
         //***************************************************************************************//
         if (sweep_param.HDET_Avgs.size() > 0 && sweep_param.HDET_Avgs[paIndex] > 0)
         {
            RFCal_Helper_setOpCode_TxMeasure(Sweep[recordIndex],Sweep[recordIndex].txParams.txDevice, (sweep_param.HDET_Avgs[paIndex]>0)?1:0, (byte)sweep_param.HDET_Avgs[paIndex]);
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         }
      }
      //****************************************************************//
      // Insert NoP (OpCode 0x0) if MaxExpectedPwr is changed.
      //****************************************************************//
      if (paIndex == (sweep_param.PA_State_List.size() - 1) || sweep_param.Max_Expected_Pwr[paIndex] != sweep_param.Max_Expected_Pwr[paIndex+1])
      {
         RFCal_helper_InsertNop(Sweep,recordIndex, 1);
      }
   }
   if (sweep_param.Sweep_Type == XPT_SWP3)
   {
      const int EPT_Mode = 0;
      const int ET_Mode = 1;

      if (cal_param.En_EPTET_Different_PAstate == 0)
      {
         //****************************************************************//
         // OpCode 0x800: DATA - Load Calc DPD table Action Processing
         //****************************************************************//

         for(int  modeIndex = 0; modeIndex < 2; modeIndex++)  // modeIndex = 0: EPT, 1: ET
         {
            if (modeIndex == EPT_Mode && sweep_param.EPT_Swp3_Meas_Pwr_List.size() == 0) continue;  // if no EPT, skip EPT
            if (modeIndex == ET_Mode  && sweep_param.ET_Swp3_Meas_Pwr_List.size()  == 0) continue;  // if no ET, skip ET

            unsigned char bEPT = ( modeIndex == EPT_Mode );
            Sweep[recordIndex].subSwpType                   = ESC_SUB_SWEEP_DEFAULT;
            Sweep[recordIndex].resExpected                     = true;
                RFCal_Helper_setOpCodeData(Sweep[recordIndex],
                    cal_param,  
                    sweep_param, 
                    DATA_OPCODE_ACTION_LOAD_CALC_DPD /* OpCode Data Option */, 
                    demo_param.TECH, 
                    demo_param.BAND, 
                    bEPT);
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         }
      }
      //****************************************************************//
      //  Find segment in Sweep 2 which obtaines DPD results
      //****************************************************************//
      int SegNum_ET_AMAMPM_Response_Sweep2 = -1;
      int SegNum_EPT_AMAMPM_Response_Sweep2 = -1;
      int SegNum_Detrough_Response_Sweep2 = -1;

      for (UINT i=0; i < Sweep2.size(); i++)
      {
         if (Sweep2[i].dataRes.measDpdRes[0].measured && Sweep2[i].dataRes.measDpdRes[0].xptMode == 1)// EPT
            SegNum_EPT_AMAMPM_Response_Sweep2 = i;

         if (Sweep2[i].dataRes.measDpdRes[0].measured && Sweep2[i].dataRes.measDpdRes[0].xptMode == 2)// ET
            SegNum_ET_AMAMPM_Response_Sweep2 = i;

         if (Sweep2[i].dataRes.measLoadEPDTRes[0].detValid)
            SegNum_Detrough_Response_Sweep2 = i;
      }
      if ( sweep_param.EPT_Swp3_Meas_Pwr_List.size() > 0 && SegNum_EPT_AMAMPM_Response_Sweep2 == -1 )
         throw RFCal_Helper_Exception("EPT AMAM/AMPM result segment in sweep 2 is not found.",  __FILE__, __LINE__ );

      if ( sweep_param.ET_Swp3_Meas_Pwr_List.size() > 0 && SegNum_ET_AMAMPM_Response_Sweep2 == -1 )
         throw RFCal_Helper_Exception("ET AMAM/AMPM result segment in sweep 2 is not found.",  __FILE__, __LINE__ );

      if ( sweep_param.ET_Swp3_Meas_Pwr_List.size() > 0 && SegNum_Detrough_Response_Sweep2 == -1)
         throw RFCal_Helper_Exception("Detrough result segment in sweep 2 is not found.",  __FILE__, __LINE__ );

      //****************************************************************//
      //  Swp3 Apply DPD ET Segments (Override by Power)
      //****************************************************************//
      for(int  modeIndex = 0; modeIndex < 2; modeIndex++)  // modeIndex = 0: EPT, 1: ET
      {
         if (cal_param.En_EPTET_Different_PAstate == 1)
         {
            //****************************************************************//
            // OpCode 0x800: DATA - Load Calc DPD table Action Processing
            //****************************************************************//

            if (modeIndex == EPT_Mode && sweep_param.EPT_Swp3_Meas_Pwr_List.size() == 0) continue;  // if no EPT, skip EPT
            if (modeIndex == ET_Mode  && sweep_param.ET_Swp3_Meas_Pwr_List.size()  == 0) continue;  // if no ET, skip ET

            unsigned char bEPT = ( modeIndex == EPT_Mode );
            Sweep[recordIndex].subSwpType                   = ESC_SUB_SWEEP_DEFAULT;
            Sweep[recordIndex].resExpected                     = true;
                RFCal_Helper_setOpCodeData(Sweep[recordIndex],cal_param,  
                    sweep_param, 
                    DATA_OPCODE_ACTION_LOAD_CALC_DPD /* OpCode Data Option */, 
                    demo_param.TECH, 
                    demo_param.BAND, 
                    bEPT);
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         }

         if (modeIndex == EPT_Mode && sweep_param.EPT_Swp3_Meas_Pwr_List.size() == 0) continue;   // if no EPT, skip EPT
         if (modeIndex == ET_Mode  && sweep_param.ET_Swp3_Meas_Pwr_List.size()  == 0) continue;   // if no ET, skip ET

         for (int calcModeIndex = 0; calcModeIndex < 2; calcModeIndex++)
         {
            const int Meas_Mode = 0;
            const int Calc_Mode = 1;

            if (modeIndex == ET_Mode && calcModeIndex == Calc_Mode) continue;

            vector < double > * pXPT_Pwr_List;
            int SegNum_AMAMPM_Response_Sweep2;
            if (modeIndex == EPT_Mode)
            {
               SegNum_AMAMPM_Response_Sweep2 = SegNum_EPT_AMAMPM_Response_Sweep2;
               if (calcModeIndex == Calc_Mode)
               {
                  pXPT_Pwr_List = (vector < double > *)& sweep_param.EPT_Swp3_Calc_Pwr_List;
               }
               else
               {
                  pXPT_Pwr_List = (vector < double > *)& sweep_param.EPT_Swp3_Meas_Pwr_List;
               }
            }
            else     // ET mode
            {
               SegNum_AMAMPM_Response_Sweep2 = SegNum_ET_AMAMPM_Response_Sweep2;
               pXPT_Pwr_List = (vector < double > *)& sweep_param.ET_Swp3_Meas_Pwr_List;
            }

            UINT irgiIndexSize = pXPT_Pwr_List->size();

            for(UINT rgiIndex = 0; rgiIndex < irgiIndexSize ; rgiIndex++)
            {
               Sweep[recordIndex].measSeg = true;
               Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_APPLY_DPD;

               word tmpExpectedPwr =  static_cast<word>(pXPT_Pwr_List->at(rgiIndex)*100.0);

               //****************************************************************//
               //  OpCode 0x200 (bit 9 enabled): TxOverride
               //****************************************************************//
                    RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],
                        sweep_param.Purpose_List[0], 
                        cal_param.TxDeviceFlag, 
                        (modeIndex == EPT_Mode)?XPT_TYPE_EPT:XPT_TYPE_ET, 
                        ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */, 
                        ESC_TX_PWR_OVERRIDE_ABS_PWR /* pwrOverrideType */,          
                        0 /* pwrParamMask */, 
                                                 NULL, NULL, NULL, NULL, NULL, NULL,NULL,NULL,NULL,NULL,
                        tmpExpectedPwr, 
                        sweep_param.Ref_Channel);

            TE_Records[recordIndex].m_ul_lvl = sweep_param.XPT_Swp3_Max_Expected_Power;
                    Sweep[recordIndex].txParams.flag = sweep_param.XPT_Swp3_Apply_Pin;


               //****************************************************************//
               //  Add a Fudge segment
               //****************************************************************//
               if ((rgiIndex == 0 && cal_param.En_EPTET_Different_PAstate == 1)
                     ||(rgiIndex == 0 && cal_param.En_EPTET_Different_PAstate == 0 && calcModeIndex == Meas_Mode ))
               {
                        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
                  Sweep[recordIndex] = Sweep[recordIndex - 1];
                  Sweep[recordIndex - 1].subSwpType = -1;         // Make the previous segment as a fudge segment
                  Sweep[recordIndex - 1].measSeg   = false;
               }

               //****************************************************************//
               //  Add DPD info from Sweep2 (this is for NV generation)
               //****************************************************************//
               Sweep[recordIndex].txParams.txParams[dev].paState = (modeIndex == EPT_Mode)?cal_param.Swp[Swp2].XPT_Swp2_EPT_PA_State:cal_param.Swp[Swp2].XPT_Swp2_ET_PA_State;   
               Sweep[recordIndex].txParams.txParams[dev].quiescentCurrent= cal_param.PA_Param.IcqEt; // (modeIndex == EPT_Mode)?cal_param.PA_Param.IcqEpt:cal_param.PA_Param.IcqEt;

               if(calcModeIndex == Meas_Mode)
               {
                  Sweep[recordIndex].dataRes.measDpdRes[0].dpdValid =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].dpdValid;
                  Sweep[recordIndex].dataRes.measDpdRes[0].measured =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].measured;
                  Sweep[recordIndex].dataRes.measDpdRes[0].xptMode =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].xptMode;

                  Sweep[recordIndex].txParams.txParams[dev].rgi   = (byte) Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].rgi;
                  Sweep[recordIndex].txParams.txParams[dev].bias = Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].smpsBias;
                  for( unsigned short dpdIdx = 0; dpdIdx < QMSL_XPT_CAL_PREDIST_SIZE; dpdIdx++ )
                  {
                     Sweep[recordIndex].dataRes.measDpdRes[0].amamRes[dpdIdx] =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].amamRes[dpdIdx];
                     Sweep[recordIndex].dataRes.measDpdRes[0].ampmRes[dpdIdx] =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.measDpdRes[rgiIndex].ampmRes[dpdIdx];
                  }
               }
               else // Calc
               {
                  Sweep[recordIndex].dataRes.calcDpdRes[0].dpdValid =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].dpdValid;
                  Sweep[recordIndex].dataRes.calcDpdRes[0].measured =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].measured;
                  Sweep[recordIndex].dataRes.calcDpdRes[0].xptMode =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].xptMode;

                  Sweep[recordIndex].txParams.txParams[dev].rgi   = (byte) Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].rgi;
                  Sweep[recordIndex].txParams.txParams[dev].bias = Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].smpsBias;

                  for( unsigned short dpdIdx = 0; dpdIdx < QMSL_XPT_CAL_PREDIST_SIZE; dpdIdx++ )
                  {
                     Sweep[recordIndex].dataRes.calcDpdRes[0].amamRes[dpdIdx] =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].amamRes[dpdIdx];
                     Sweep[recordIndex].dataRes.calcDpdRes[0].ampmRes[dpdIdx] =  Sweep2[SegNum_AMAMPM_Response_Sweep2].dataRes.calcDpdRes[rgiIndex].ampmRes[dpdIdx];
                  }
               }
               if (modeIndex == ET_Mode)  // Detrough
               {
                  Sweep[recordIndex].dataRes.measLoadEPDTRes[0].detValid = Sweep2[SegNum_Detrough_Response_Sweep2].dataRes.measLoadEPDTRes[rgiIndex].detValid;
                  Sweep[recordIndex].dataRes.measLoadEPDTRes[0].measured = Sweep2[SegNum_Detrough_Response_Sweep2].dataRes.measLoadEPDTRes[rgiIndex].measured;
                  if (Sweep[recordIndex].dataRes.measLoadEPDTRes[0].detValid == 0)
                     throw RFCal_Helper_Exception("In Sweep 2 result. Detrough results are not obtained.",  __FILE__, __LINE__ );

                  if (Sweep[recordIndex].dataRes.measLoadEPDTRes[0].measured)
                  {
                     for( unsigned short detIdx = 0; detIdx < QMSL_XPT_CAL_EXPANDED_DET_SIZE; detIdx++ )
                     {
                        Sweep[recordIndex].dataRes.measLoadEPDTRes[0].epdtTbl[detIdx] =  Sweep2[SegNum_Detrough_Response_Sweep2].dataRes.measLoadEPDTRes[rgiIndex].epdtTbl[detIdx];
                     }
                  }
                  else
                  {
                     for( unsigned short detIdx = 0; detIdx < QMSL_XPT_CAL_EXPANDED_DET_SIZE; detIdx++ )
                     {
                        Sweep[recordIndex].dataRes.calcLoadEPDTRes[0].epdtTbl[detIdx] =  Sweep2[SegNum_Detrough_Response_Sweep2].dataRes.measLoadEPDTRes[rgiIndex].epdtTbl[detIdx];
                     }
                  }
               }
                    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
            }  // End of for(rgiIndex = 0; rgiIndex < sizeof(ET_Swp3_Meas_Pwr_List)/sizeof(double); rgiIndex++)
         }
      }
   }

   //****************************************************************//
    //   OpCode 0x8000: TxMeas: THERM READ (XPT_SWP3 only. Other case, at sweep 1. This is just same as SubsysRFCal. not calibraiton design.
   //****************************************************************//
    if (sweep_param.Sweep_Type == XPT_SWP3)
    {
   Sweep[recordIndex].txPurpose  = (word)0;
   Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_THERM_READ;
    
    RFCal_Helper_setOpCode_TxMeasure(Sweep[recordIndex],
        cal_param.TxDeviceFlag,
        ESC_TX_MEAS_ACTION_THERM_READ);
    
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
    }
   //*****************************************************************************************/
   //    End of Sweep 3: Insert a low rgi segment instead of tear down
   //*****************************************************************************************/
   RFCal_Helper_setLowRGISegment(Sweep[recordIndex],cal_param.TxDeviceFlag);
    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

   //*********************************************//
   //    Generate OpCode for all the segment
   //*********************************************//
   Sweep.resize(recordIndex); // Delete unused records.
   RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());
   return true;
}

unsigned char RFCal_Helper_ESC_ET_createXPT2Swp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param,
                                                const vector < cal_sweep_record >   & Sweep1, vector < cal_sweep_record > & Sweep)
{
   //***********************************************************//
   //    Create Sweep 2 records
   //***********************************************************//

   unsigned char  bOK            = true;
   UINT        recordIndex    = 0;
   int txParamMask_PwrOverride  = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN |
                                   ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT | ESC_TX_OVERRIDE_DELAY); // Sweep
    int             Feedback_Device_ID          = static_cast<int>(sweep_param.Device_Selection.Feedback_Device_ID);    

   const int      Swp1 = 0;
   int MasterDevState = 4;
   int SlaveDevState = 0;

   UINT dev = 0;
   if (cal_param.Tx_Rx_Split)
   {
      dev = static_cast<UINT>(sweep_param.Device_Selection.Master_Device_ID);
   }
   else
   {
      dev = static_cast<UINT>(RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag));
   }
   //*****************************************************************************************/
   //    TDSCDMA only. Set DPD waveform
   //*****************************************************************************************/
   if (demo_param.TECH == 4)
   {
      //*****************************************************************************************//
      // OpCode 0x100: Radio Setup
      //*****************************************************************************************//
      byte txWfm = 5;
      MasterDevState = 0;
      RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex],
                                        sweep_param.Device_Selection.Master_Device_ID,
                                        sweep_param.Device_Selection.Slave_Device_ID,
                                        MasterDevState,
                                        SlaveDevState,
                                        demo_param.TECH,
                                        demo_param.BAND,
                                        sweep_param.Ref_Channel /* ulChan */,
                                        txWfm);
      Sweep[recordIndex].radio_cfg.flag = 0;
      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);

      //*****************************************************************************************//
      // Insert NoP (OpCode 0x0)
      //*****************************************************************************************//
      RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length, sweep_param.Segment_Length[0][0]);
   }

   //*****************************************************************************************//
   //  OpCode 0x1000: Sync OpCode (LTE Only)
   //*****************************************************************************************//

   if (demo_param.TECH == 3 )
   {  
      RFCal_Helper_setOpCode_Sync(Sweep[recordIndex]);
      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
      RFCal_helper_InsertNop(Sweep,recordIndex,1);
   }

   //***********************************************************//
   //    Insert XPT CFG
   //    OpCode 0x2000: XPT2
   //***********************************************************//

   Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;
   Sweep[recordIndex].txPurpose  = static_cast<word>(0);
    RFCal_Helper_setOpCode_XPT2(Sweep[recordIndex],  
        ESC_XPT_ACTION_CAL_CFG /* xptAction */, 
      dev /* cfg_dev */, 
        Feedback_Device_ID, 
        XPT_TYPE_EPT);
    
   RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);

   // Bit 0 - Data Push RGIvsPwr requires the following fields.
   // Pre Computation 1. for Sweep 2

   //for (UINT iPreComputeLoop = 0; iPreComputeLoop < sweep_param.Pre_Sweep_Computation.size(); iPreComputeLoop++)
   //{
   // if (sweep_param.Pre_Sweep_Computation[iPreComputeLoop] == 1)
   // {
   if (cal_param.En_EPTET_Different_PAstate == 0)
   {
      //***********************************************************//
      //    OpCode 0x800: Push RGI Delta data
      //***********************************************************//

      Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;
      Sweep[recordIndex].resExpected = true;
      Sweep[recordIndex].txPurpose = static_cast<word>(0);

      unsigned char bEPT = (sweep_param.EPT_Swp2_Meas_Pwr_List.size()> 0);

        RFCal_Helper_setOpCodeData(Sweep[recordIndex], 
            cal_param, 
            sweep_param, 
            DATA_OPCODE_ACTION_RGI_PWR_TBL /* OpCode Data Option */, 
            demo_param.TECH, 
            demo_param.BAND, 
            bEPT);
      Sweep[recordIndex].data.rgiVsPwr.doRgiVsPwrOp      = true;  // This field is required.

      byte rgi;
      word pwr;
      byte numRgiDelta = 0;
      for (UINT i = 0; i < Sweep1.size(); i++)
      {
         if (Sweep1[i].subSwpType == ESC_SUB_SWEEP_RGI_DELTA)
         {
            rgi = Sweep1[i].txParams.txParams[dev].rgi; 
                pwr = static_cast<word>(Sweep1[i].txRes.txPower);
            Sweep[recordIndex].data.rgiVsPwr.rgi[numRgiDelta]     = rgi;
            Sweep[recordIndex].data.rgiVsPwr.pwr[numRgiDelta]     = pwr;
            numRgiDelta++;
         }
      }
      Sweep[recordIndex].data.rgiVsPwr.numRGIDeltas = numRgiDelta;
      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
   }
   else      // Assume that this is ET and EPT
   {
      for (UINT EptEtIndex = 0; EptEtIndex < 2; EptEtIndex++) // EptEtIndex 0 : EPT, 1 : ET
      {
         //***********************************************************//
         //    OpCode 0x800: DataPush RGIvsPwrvsPAstate
         //***********************************************************//
         Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;
         Sweep[recordIndex].resExpected = true;
         Sweep[recordIndex].txPurpose = static_cast<word>(0);

         unsigned char bEPT = (EptEtIndex == 0)? true: false;

         RFCal_Helper_setOpCodeData(Sweep[recordIndex], cal_param, sweep_param, DATA_OPCODE_ACTION_RGI_PWR_TBL_COM /* OpCode Data Option */, demo_param.TECH, demo_param.BAND, bEPT);
         Sweep[recordIndex].data.rgiVsPwrVsState.doRgiVsPwrVsStateOp    = true;  // This field is required.'

         vector <byte> XPT_Swp1_RGI_Delta_RGI_List = (EptEtIndex == 0)?cal_param.Swp[Swp1].XPT_Swp1_RGI_Delta_RGI_List_EPT:cal_param.Swp[Swp1].XPT_Swp1_RGI_Delta_RGI_List_ET;;


         Sweep[recordIndex].data.rgiVsPwrVsState.paState = (EptEtIndex == 0)?cal_param.Swp[Swp1].XPT_Swp1_PA_State_EPT:cal_param.Swp[Swp1].XPT_Swp1_PA_State_ET;

         Sweep[recordIndex].data.rgiVsPwrVsState.xptMode = 0;

         byte numRgiDelta = 0;
            byte firstPwr = true;
            word prePwr = 0;

         for (UINT i = 0; i < Sweep1.size(); i++)
         {
            word pwr;
            byte pa = Sweep1[i].txParams.txParams[dev].paState;
            if (Sweep1[i].subSwpType == ESC_SUB_SWEEP_RGI_DELTA && pa == Sweep[recordIndex].data.rgiVsPwrVsState.paState)
            {
               byte rgi = Sweep1[i].txParams.txParams[dev].rgi; 
                    pwr = static_cast<word>(Sweep1[i].txRes.txPower);
                    short pwrDelta = static_cast<short>(pwr) - static_cast<short>(prePwr);
                    if ((pwrDelta/10.0  > (sweep_param.XPT_Swp1_Minimum_RGI_Delta_Power_dBm100/100.0)) || firstPwr)
                    {
               Sweep[recordIndex].data.rgiVsPwrVsState.rgi[numRgiDelta]    = rgi;
               Sweep[recordIndex].data.rgiVsPwrVsState.pwr[numRgiDelta]    = pwr;
               numRgiDelta++;
                        firstPwr = false;
                        prePwr = pwr;
                    }
                }
            }
            Sweep[recordIndex].data.rgiVsPwrVsState.numRGIDeltas = numRgiDelta;
         RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
      }
   }

   // Precomputation
   if (cal_param.En_EPTET_Different_PAstate == 0)
   {
      //***********************************************************//
      //    OpCode 0x800: M-Line Calc processing
      //***********************************************************//

      /* ESC Parameters */
      Sweep[recordIndex].subSwpType                   = ESC_SUB_SWEEP_DEFAULT;
      Sweep[recordIndex].resExpected                     = true;
      Sweep[recordIndex].txPurpose                    = (word)0;

      if (sweep_param.XPT_Swp2_MLine_Calc_Segment_Length != 0)
      {
         Sweep[recordIndex].opcode.segLen                = sweep_param.XPT_Swp2_MLine_Calc_Segment_Length;
      }

        RFCal_Helper_setOpCodeData(Sweep[recordIndex], 
            cal_param, 
            sweep_param,
            DATA_OPCODE_ACTION_MLINE_CALC /* OpCode Data Option */, 
            demo_param.TECH, 
            demo_param.BAND);

      Sweep[recordIndex].data.mptCalc.doMPtCalcOp = true; // This field is required

      // Enable Bit 2 - M-Line Calculation

      // Bit 2 - M-Line Calculation requires the following fields. i.e., (Action & 0x4) = 0x4
      // Percentage of the curve to be assumed as linear to find the 2/3dB compression points. (In *100)
      Sweep[recordIndex].data.mptCalc.linRegPct = static_cast<word>(cal_param.PA_Param.MLineRegCoef*100.0);

      // Compression point for M-Line in EPT
      if (cal_param.PA_Param.CalCompPtEpt != 0xFFFF)
      {
         Sweep[recordIndex].data.mptCalc.compDBEpt = static_cast<word>(cal_param.PA_Param.CalCompPtEpt*100.0);//2dBm
      }
      else
      {
         Sweep[recordIndex].data.mptCalc.compDBEpt = -1;
      }

      // Compression point for M-Line in ET
      if (cal_param.PA_Param.CalCompPtEt != 0xFFFF)
      {
         Sweep[recordIndex].data.mptCalc.compDBEt = static_cast<word>(cal_param.PA_Param.CalCompPtEt*100.0);//3dBm
      }
      else
      {
         Sweep[recordIndex].data.mptCalc.compDBEt = -1;//3dBm
      }

      Sweep[recordIndex].data.mptCalc.paEpt = sweep_param.XPT_Swp2_EPT_PA_State; // PA for EPT
      Sweep[recordIndex].data.mptCalc.paEt = sweep_param.XPT_Swp2_ET_PA_State; // PA for ET

      Sweep[recordIndex].data.mptCalc.mlineBiasOption = sweep_param.XPT_Swp2_MLine_Bias_Option;

      // M-Line Curve Fit Type. 0 - Linear Fit, 1 - Quadratic Fit
      Sweep[recordIndex].data.mptCalc.curveFitType = cal_param.PA_Param.MLineFitType;  // Linear fit.

      Sweep[recordIndex].data.mptCalc.vdMaxEpt = static_cast<dword>(cal_param.PA_Param.MLineMaxPaSupplyEpt *1000.0);  // VdMax for EPT to convert voltage
      Sweep[recordIndex].data.mptCalc.vdMaxEt = static_cast<dword>(cal_param.PA_Param.MLineMaxPaSupplyEt *1000.0);  // VdMax for ET

      // Number of Power Values measured in Alignment for which IQ capture was performed. Used for calculation.
      Sweep[recordIndex].data.mptCalc.numSwp1MeasPwrs = static_cast<byte>(cal_param.Swp[Swp1].XPT_Swp1_RGI_List.size());   // This value is 4

      // Number of Power Values (measured) for DPD Construction Sweep to find the RGI and Bias from M-Line
      Sweep[recordIndex].data.mptCalc.numSwp2EptMeasPwrs = static_cast<byte>(sweep_param.EPT_Swp2_Meas_Pwr_List.size());

      // Number of Power Values (calculated) for DPD Construction Sweep to find the RGI and Bias from M-Line
      Sweep[recordIndex].data.mptCalc.numSwp2EptCalcPwrs = static_cast<byte>(sweep_param.EPT_Swp2_Calc_Pwr_List.size());

      // Number of Power Values (measured) for DPD Construction Sweep to find the RGI and Bias from M-Line
      Sweep[recordIndex].data.mptCalc.numSwp2EtMeasPwrs = static_cast<byte>(sweep_param.ET_Swp2_Meas_Pwr_List.size());  // This value is 4.

      // Number of Power Values (calculated) for DPD Construction Sweep to find the RGI and Bias from M-Line
      Sweep[recordIndex].data.mptCalc.numSwp2EtCalcPwrs = static_cast<byte>(0);

      // PreSweepCompute is 4 (XPT_SWP2_FROM_SWP1_DATA_COMPUTE_TYPE)
      for (UINT iPreComputeLoop = 0; iPreComputeLoop < sweep_param.Pre_Sweep_Computation.size(); iPreComputeLoop++)
      {
         if (sweep_param.Pre_Sweep_Computation[iPreComputeLoop] == 4)
         {
            for(UINT index = 0; index <   Sweep[recordIndex].data.mptCalc.numSwp1MeasPwrs ; index++)
            {
               UINT iAlignmentSegment = 0;
               //iRecordIndexXPT
               for (UINT i = 0; i < Sweep1.size(); i++)
               {
                  if ((Sweep1[i].subSwpType == ESC_SUB_SWEEP_XPT_ALIGNMENT) && (Sweep1[i].txParams.txParams[dev].bias == cal_param.Swp[Swp1].XPT_Swp1_Bias_List[index]))
                  {
                     iAlignmentSegment = i;
                     break;
                  }
               }
               // if not found, something wrong. please exit.
               if (iAlignmentSegment == 0)
                  throw RFCal_Helper_Exception("Alignment Segment not found.",  __FILE__, __LINE__ );

               const cal_sweep_record * tmp = &(Sweep1[iAlignmentSegment]);

               // PA State in Alignment Sweep
               Sweep[recordIndex].data.mptCalc.swp1pa[index] = tmp->txParams.txParams[dev].paState;// XPT_Swp1_PA_State;//higher power range
               // RGI used in Alignment Sweep
               Sweep[recordIndex].data.mptCalc.swp1rgi[index] = tmp->txParams.txParams[dev].rgi;//XPT_Swp1_RGI_List[index];

               // Bias used in Alignment Sweep
               Sweep[recordIndex].data.mptCalc.swp1bias[index] = tmp->txParams.txParams[dev].bias; //XPT_Swp1_Bias_List[index];

               // Power Measured in Alignment Sweep
                    Sweep[recordIndex].data.mptCalc.measPwrSwp1DB10[index] = static_cast<word>(tmp->txRes.txPower);//POWER_ALIGNMENT_LIST_From_1st_Sweep[index]*10.0;
            }
         }
      }
      // End of PreSweepCompute 4

      for(int index = 0; index < Sweep[recordIndex].data.mptCalc.numSwp2EptMeasPwrs ; index++)
      {
         // Requested (measured) power for Swp 2 DPD Construction
         Sweep[recordIndex].data.mptCalc.eptMeasPwrSwp2DB100[index] = static_cast<word>(sweep_param.EPT_Swp2_Meas_Pwr_List[index]*100.0);
      }

      for(int index = 0; index < Sweep[recordIndex].data.mptCalc.numSwp2EptCalcPwrs ; index++)
      {
         // Requested (calculated) power for Swp 2 DPD Construction
         Sweep[recordIndex].data.mptCalc.eptCalcPwrSwp2DB100[index] = static_cast<word>(sweep_param.EPT_Swp2_Calc_Pwr_List[index]*100.0);
      }
      for(int index = 0; index < Sweep[recordIndex].data.mptCalc.numSwp2EtMeasPwrs ; index++)
      {
         // Requested (measured) power for Swp 2 DPD Construction
         Sweep[recordIndex].data.mptCalc.etMeasPwrSwp2DB100[index] = static_cast<word>(sweep_param.ET_Swp2_Meas_Pwr_List[index]*100.0);
      }

      for(int index = 0; index < Sweep[recordIndex].data.mptCalc.numSwp2EtCalcPwrs ; index++)
      {
         // Requested (calculated) power for Swp 2 DPD Construction
         Sweep[recordIndex].data.mptCalc.etCalcPwrSwp2DB100[index] = 0;
      }
      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
   }
   else
   {
      for (UINT EptEtIndex = 0; EptEtIndex < 2; EptEtIndex++) // EptEtIndex 0 : EPT, 1 : ET
      {
         //***********************************************************//
         //    OpCode 0x800: M-Line Calc processing
         //***********************************************************//

         /* ESC Parameters */
         Sweep[recordIndex].subSwpType                   = ESC_SUB_SWEEP_DEFAULT;
         Sweep[recordIndex].resExpected                     = true;
         Sweep[recordIndex].txPurpose                    = (word)0;

         Sweep[recordIndex].opcode.segLen                = sweep_param.XPT_Swp2_MLine_Calc_Segment_Length;

         RFCal_Helper_setOpCodeData(Sweep[recordIndex], cal_param, sweep_param, DATA_OPCODE_ACTION_MLINE_CALC /* OpCode Data Option */, demo_param.TECH, demo_param.BAND);

         Sweep[recordIndex].data.mptCalc.doMPtCalcOp = true; // This field is required

         // Enable Bit 2 - M-Line Calculation

         // Bit 2 - M-Line Calculation requires the following fields. i.e., (Action & 0x4) = 0x4
         // Percentage of the curve to be assumed as linear to find the 2/3dB compression points. (In *100)
         Sweep[recordIndex].data.mptCalc.linRegPct = static_cast<word>(cal_param.PA_Param.MLineRegCoef*100.0);

         // Compression point for M-Line in EPT
         if (cal_param.PA_Param.CalCompPtEpt != 0xFFFF)
         {
            Sweep[recordIndex].data.mptCalc.compDBEpt = (EptEtIndex == 0)?static_cast<word>(cal_param.PA_Param.CalCompPtEpt*100.0):-1;//2dBm
         }
         else
         {
            Sweep[recordIndex].data.mptCalc.compDBEpt = -1;
         }

         // Compression point for M-Line in ET
         if (cal_param.PA_Param.CalCompPtEt != 0xFFFF)
         {
            Sweep[recordIndex].data.mptCalc.compDBEt = (EptEtIndex == 0)?-1:static_cast<word>(cal_param.PA_Param.CalCompPtEt*100.0);//3dBm
         }
         else
         {
            Sweep[recordIndex].data.mptCalc.compDBEt = -1;//3dBm
         }

         Sweep[recordIndex].data.mptCalc.paEpt = sweep_param.XPT_Swp2_EPT_PA_State; // PA for EPT
         Sweep[recordIndex].data.mptCalc.paEt = sweep_param.XPT_Swp2_ET_PA_State; // PA for ET

         Sweep[recordIndex].data.mptCalc.mlineBiasOption = sweep_param.XPT_Swp2_MLine_Bias_Option;

         // M-Line Curve Fit Type. 0 - Linear Fit, 1 - Quadratic Fit
         Sweep[recordIndex].data.mptCalc.curveFitType = cal_param.PA_Param.MLineFitType;  // Linear fit.

         Sweep[recordIndex].data.mptCalc.vdMaxEpt = static_cast<dword>(cal_param.PA_Param.MLineMaxPaSupplyEpt *1000.0);  // VdMax for EPT to convert voltage
         Sweep[recordIndex].data.mptCalc.vdMaxEt = static_cast<dword>(cal_param.PA_Param.MLineMaxPaSupplyEt *1000.0);  // VdMax for ET

         vector <byte> XPT_Swp1_RGI_List =  (EptEtIndex == 0)?cal_param.Swp[Swp1].XPT_Swp1_RGI_List_EPT:cal_param.Swp[Swp1].XPT_Swp1_RGI_List_ET;
         // Number of Power Values measured in Alignment for which IQ capture was performed. Used for calculation.
         Sweep[recordIndex].data.mptCalc.numSwp1MeasPwrs = static_cast<byte>(XPT_Swp1_RGI_List.size());  // This value is 4

         // Number of Power Values (measured) for DPD Construction Sweep to find the RGI and Bias from M-Line
         Sweep[recordIndex].data.mptCalc.numSwp2EptMeasPwrs = static_cast<byte>(sweep_param.EPT_Swp2_Meas_Pwr_List.size());

         // Number of Power Values (calculated) for DPD Construction Sweep to find the RGI and Bias from M-Line
         Sweep[recordIndex].data.mptCalc.numSwp2EptCalcPwrs = static_cast<byte>(sweep_param.EPT_Swp2_Calc_Pwr_List.size());

         // Number of Power Values (measured) for DPD Construction Sweep to find the RGI and Bias from M-Line
         Sweep[recordIndex].data.mptCalc.numSwp2EtMeasPwrs = (EptEtIndex == 0)?0:static_cast<byte>(sweep_param.ET_Swp2_Meas_Pwr_List.size());   // This value is 4.

         // Number of Power Values (calculated) for DPD Construction Sweep to find the RGI and Bias from M-Line
         Sweep[recordIndex].data.mptCalc.numSwp2EtCalcPwrs = static_cast<byte>(0);

         // PreSweepCompute is 4 (XPT_SWP2_FROM_SWP1_DATA_COMPUTE_TYPE)
         for (UINT iPreComputeLoop = 0; iPreComputeLoop < sweep_param.Pre_Sweep_Computation.size(); iPreComputeLoop++)
         {
            if (sweep_param.Pre_Sweep_Computation[iPreComputeLoop] == 4)
            {
               vector <word> XPT_Swp1_Bias_List =  (EptEtIndex == 0)?cal_param.Swp[Swp1].XPT_Swp1_Bias_List_EPT:cal_param.Swp[Swp1].XPT_Swp1_Bias_List_ET;
               byte pa = (EptEtIndex == 0)? sweep_param.XPT_Swp2_EPT_PA_State:  sweep_param.XPT_Swp2_ET_PA_State;
               for(UINT index = 0; index <   Sweep[recordIndex].data.mptCalc.numSwp1MeasPwrs ; index++)
               {
                  UINT iAlignmentSegment = 0;
                  //iRecordIndexXPT
                  for (UINT i = 0; i < Sweep1.size(); i++)
                  {
                     if ( Sweep1[i].subSwpType == ESC_SUB_SWEEP_XPT_ALIGNMENT
                        && Sweep1[i].txParams.txParams[dev].bias == XPT_Swp1_Bias_List[index]
                           && Sweep1[i].txParams.txParams[dev].paState == pa)
                     {
                        iAlignmentSegment = i;
                        break;
                     }
                  }
                  // if not found, something wrong. please exit.
                  if (iAlignmentSegment == 0)
                     throw RFCal_Helper_Exception("Alignment Segment not found.",  __FILE__, __LINE__ );

                  const cal_sweep_record * tmp = &(Sweep1[iAlignmentSegment]);

                  // PA State in Alignment Sweep
                  Sweep[recordIndex].data.mptCalc.swp1pa[index] = tmp->txParams.txParams[dev].paState;// XPT_Swp1_PA_State;//higher power range
                  // RGI used in Alignment Sweep
                  Sweep[recordIndex].data.mptCalc.swp1rgi[index] = tmp->txParams.txParams[dev].rgi;//XPT_Swp1_RGI_List[index];

                  // Bias used in Alignment Sweep
                  Sweep[recordIndex].data.mptCalc.swp1bias[index] = tmp->txParams.txParams[dev].bias; //XPT_Swp1_Bias_List[index];

                  // Power Measured in Alignment Sweep
                        Sweep[recordIndex].data.mptCalc.measPwrSwp1DB10[index] = static_cast<word>(tmp->txRes.txPower);//POWER_ALIGNMENT_LIST_From_1st_Sweep[index]*10.0;
               }
            }
         }
         // End of PreSweepCompute 4

         for(int index = 0; index < Sweep[recordIndex].data.mptCalc.numSwp2EptMeasPwrs ; index++)
         {
            // Requested (measured) power for Swp 2 DPD Construction
            Sweep[recordIndex].data.mptCalc.eptMeasPwrSwp2DB100[index] = static_cast<word>(sweep_param.EPT_Swp2_Meas_Pwr_List[index]*100.0);
         }

         for(int index = 0; index < Sweep[recordIndex].data.mptCalc.numSwp2EptCalcPwrs ; index++)
         {
            // Requested (calculated) power for Swp 2 DPD Construction
            Sweep[recordIndex].data.mptCalc.eptCalcPwrSwp2DB100[index] = static_cast<word>(sweep_param.EPT_Swp2_Calc_Pwr_List[index]*100.0);
         }
         for(int index = 0; index < Sweep[recordIndex].data.mptCalc.numSwp2EtMeasPwrs ; index++)
         {
            // Requested (measured) power for Swp 2 DPD Construction
            Sweep[recordIndex].data.mptCalc.etMeasPwrSwp2DB100[index] = static_cast<word>(sweep_param.ET_Swp2_Meas_Pwr_List[index]*100.0);
         }

         for(int index = 0; index < Sweep[recordIndex].data.mptCalc.numSwp2EtCalcPwrs ; index++)
         {
            // Requested (calculated) power for Swp 2 DPD Construction
            Sweep[recordIndex].data.mptCalc.etCalcPwrSwp2DB100[index] = 0;
         }
         RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
      }
   }
   //****************************************************************//
   // OpCode 0x800: DATA - Get MLine results segment
   //  Since an older chipset may not handle this, we skip.cal_param.En_EPTET_Different_PAstate == 0 in this demo
   //****************************************************************//
   if (sweep_param.XPT_Swp2_Enable_Mline_Debug_Info == (unsigned char)true)
   {
      if (cal_param.En_EPTET_Different_PAstate == 1)
      {
         /* ESC Parameters */
         Sweep[recordIndex].subSwpType                   = ESC_SUB_SWEEP_DEFAULT;
         Sweep[recordIndex].resExpected                     = true;
         Sweep[recordIndex].txPurpose                    = (word)0;
         Sweep[recordIndex].opcode.segLen                = sweep_param.XPT_Swp2_MLine_Calc_Segment_Length;

         RFCal_Helper_setOpCodeData(Sweep[recordIndex], cal_param, sweep_param, DATA_OPCODE_ACTION_GET_MLINE_RES /* OpCode Data Option */, demo_param.TECH, demo_param.BAND);

         Sweep[recordIndex].data.getMlineRes.do_get_mline_res_op = true; // This field is required

            Sweep[recordIndex].data.getMlineRes.eptMlineResAction = (RESULT_MLINE_TABLE | RESULT_COMPRESSION_POINTS | RESULT_MLINE_SLOPE_POINTS);
         Sweep[recordIndex].data.getMlineRes.etMlineResAction = (RESULT_MLINE_TABLE |  RESULT_SELECTION_SWEEP | RESULT_COMPRESSION_POINTS | RESULT_MLINE_SLOPE_POINTS);

         if (Sweep[recordIndex].data.getMlineRes.eptMlineResAction && RESULT_SELECTION_SWEEP)
         {
            Sweep[recordIndex].data.getMlineRes.numEptSelSwpCalcPwrs = sweep_param.EPT_Swp2_Calc_Pwr_List.size();
            Sweep[recordIndex].data.getMlineRes.numEptSelSwpMeasPwrs = sweep_param.EPT_Swp2_Meas_Pwr_List.size();
            for (UINT idx=0; idx < sweep_param.EPT_Swp2_Calc_Pwr_List.size(); idx++)
            {
               Sweep[recordIndex].data.getMlineRes.eptSelSwpCalcPwrsdB100[idx]= static_cast<word>(sweep_param.EPT_Swp2_Calc_Pwr_List[idx]*100);
            }
            for (UINT idx=0; idx < sweep_param.EPT_Swp2_Meas_Pwr_List.size(); idx++)
            {
               Sweep[recordIndex].data.getMlineRes.eptSelSwpMeasPwrsdB100[idx]= static_cast<word>(sweep_param.EPT_Swp2_Meas_Pwr_List[idx]*100);
            }
         }
         if (Sweep[recordIndex].data.getMlineRes.etMlineResAction && RESULT_SELECTION_SWEEP)
         {
            Sweep[recordIndex].data.getMlineRes.numEtSelSwpMeasPwrs = sweep_param.ET_Swp2_Meas_Pwr_List.size();
            for (UINT idx=0; idx < sweep_param.ET_Swp2_Meas_Pwr_List.size(); idx++)
            {
               Sweep[recordIndex].data.getMlineRes.etSelSwpMeasPwrsdB100[idx]= static_cast<word>(sweep_param.ET_Swp2_Meas_Pwr_List[idx]*100);
            }
         }

         if (Sweep[recordIndex].data.getMlineRes.eptMlineResAction && RESULT_COMPRESSION_POINTS)
         {
            Sweep[recordIndex].data.getMlineRes.numEptCompPts = sweep_param.EPT_Swp2_Meas_Pwr_List.size();
            for (UINT idx=0; idx < Sweep[recordIndex].data.getMlineRes.numEptCompPts ; idx++)
            {
               if (cal_param.Swp[Swp1].XPT_Swp1_RGI_List_EPT.size() > idx)
               {
               Sweep[recordIndex].data.getMlineRes.eptCompPtRgi[idx]= static_cast<byte>(cal_param.Swp[Swp1].XPT_Swp1_RGI_List_EPT[idx]);
               Sweep[recordIndex].data.getMlineRes.eptCompPtBias[idx]= static_cast<dword>(cal_param.Swp[Swp1].XPT_Swp1_Bias_List_EPT[idx]);
            }
               else if (cal_param.Swp[Swp1].XPT_Swp1_RGI_List.size() > idx)
               {
                       Sweep[recordIndex].data.getMlineRes.eptCompPtRgi[idx]= static_cast<byte>(cal_param.Swp[Swp1].XPT_Swp1_RGI_List[idx]);
                  Sweep[recordIndex].data.getMlineRes.eptCompPtBias[idx]= static_cast<dword>(cal_param.Swp[Swp1].XPT_Swp1_Bias_List[idx]);
               }
                }
         }

         if (Sweep[recordIndex].data.getMlineRes.etMlineResAction && RESULT_COMPRESSION_POINTS)
         {
            Sweep[recordIndex].data.getMlineRes.numEtCompPts = sweep_param.ET_Swp2_Meas_Pwr_List.size();
            for (UINT idx=0; idx < Sweep[recordIndex].data.getMlineRes.numEtCompPts ; idx++)
            {
               if (cal_param.Swp[Swp1].XPT_Swp1_RGI_List_ET.size() > idx)
               {
                  Sweep[recordIndex].data.getMlineRes.etCompPtRgi[idx]= static_cast<byte>(cal_param.Swp[Swp1].XPT_Swp1_RGI_List_ET[idx]);
                  Sweep[recordIndex].data.getMlineRes.etCompPtBias[idx]= static_cast<dword>(cal_param.Swp[Swp1].XPT_Swp1_Bias_List_ET[idx]);
               }
               else if (cal_param.Swp[Swp1].XPT_Swp1_RGI_List.size() > idx)
               {
                  Sweep[recordIndex].data.getMlineRes.etCompPtRgi[idx]= static_cast<byte>(cal_param.Swp[Swp1].XPT_Swp1_RGI_List[idx]);
                  Sweep[recordIndex].data.getMlineRes.etCompPtBias[idx]= static_cast<dword>(cal_param.Swp[Swp1].XPT_Swp1_Bias_List[idx]);
               }
            }
         }
         RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
      }
   }

   //****************************************************************//
   // OpCode 0x800: DATA - Load Calc DPD table Action Processing
   //****************************************************************//
   for( int modeIndex = 0; modeIndex < 2; modeIndex++)  // modeIndex = 0: EPT, 1: ET
   {
      if (modeIndex == 0 && sweep_param.EPT_Swp2_Meas_Pwr_List.size() == 0) continue;
      if (modeIndex == 1 && sweep_param.ET_Swp2_Meas_Pwr_List.size() == 0) continue;

      Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;
      Sweep[recordIndex].resExpected = true;
        RFCal_Helper_setOpCodeData(Sweep[recordIndex],cal_param,  
            sweep_param, 
            DATA_OPCODE_ACTION_LOAD_CALC_DPD /* OpCode Data Option */, 
            demo_param.TECH, 
            demo_param.BAND, (modeIndex == 0));
      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
   }

   //****************************************************************//
   //  XPT IQ Capture
   //****************************************************************//
   const int EPT_MODE = 0;
   const int ET_MODE = 1;

   for( int modeIndex = 0; modeIndex < 2; modeIndex++)  // modeIndex = 0: EPT, 1: ET
   {
      if (modeIndex == EPT_MODE && sweep_param.EPT_Swp2_Meas_Pwr_List.size() == 0) continue;
      if (modeIndex == ET_MODE && sweep_param.ET_Swp2_Meas_Pwr_List.size() == 0) continue;

      int iRgiIndexSize = (modeIndex == EPT_MODE)? sweep_param.EPT_Swp2_Meas_Pwr_List.size():sweep_param.ET_Swp2_Meas_Pwr_List.size();

      for(int rgiIndex = 0; rgiIndex < iRgiIndexSize; rgiIndex++)
      {
         //****************************************************************//
         // OpCode 0x200:  TxOverride
         //****************************************************************//
            RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],
                0, 
                cal_param.TxDeviceFlag, 
                ((modeIndex == EPT_MODE)?XPT_TYPE_EPT:XPT_TYPE_ET), 
                ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */, 
                ESC_TX_PWR_OVERRIDE_ABS_PWR /* pwrOverrideType */,          
                0 /* pwrParamMask */, 
                                           NULL, NULL, NULL,NULL,NULL, NULL, NULL,   NULL, 0, 0,
            (word)( (modeIndex == EPT_MODE)?(sweep_param.EPT_Swp2_Meas_Pwr_List[rgiIndex]*100.0):(sweep_param.ET_Swp2_Meas_Pwr_List[rgiIndex]*100.0)),
            sweep_param.XPT_Swp2_Channel_List[0]);

         Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DPD_CONSTRUCT;
         Sweep[recordIndex].measSeg = true;

         //    if (modeIndex == ET_MODE || cal_param.En_EPTET_Different_PAstate == 1)  // Divide IQ capture and TxOverride for ET or EPT_ET different is true
         RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
         if (rgiIndex == 0)
         {
            //****************************************************************//
            // Add a fudge segment
            //****************************************************************//
            Sweep[recordIndex] = Sweep[recordIndex-1];
            Sweep[recordIndex-1].subSwpType = ESC_SUB_SWEEP_MAX;        // FudgeSegment is  -1
            Sweep[recordIndex-1].measSeg = false;
            RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
         }

         //****************************************************************//
         // OpCode 0x2000: XPT2 - IQ Capture
         //****************************************************************//
         Sweep[recordIndex].subSwpType =  ESC_SUB_SWEEP_MAX;
         int firstTrigger = 0, lastTrigger = 0;
         // First Trigger. // For 1st IQ Cap segment, first trigger needs to be 1. Else 0
         // (1) EPT only / EPT + ET case: EPT is first. rgiIndex == 0 && the current mode is EPT
         // (2) ET only case: rgiIndex == 0 && the current mode is ET and there is no EPT (i.e., Sweep_param.EPT_Swp2_Meas_Pwr_List.size () == 0)
         if ((rgiIndex == 0 && modeIndex == EPT_MODE) || (rgiIndex == 0 && modeIndex == ET_MODE && sweep_param.EPT_Swp2_Meas_Pwr_List.size () == 0 ))
            firstTrigger = 1;

         // Last Trigger. // For last IQ Cap segment, last trigger needs to be 1. Else 0
         // (1) EPT + ET / ET only case: last rgiIndex && the current mode is ET
         // (2) EPT only: last rgiIndex && the current mode is EPT && there is no ET segment
         if ((rgiIndex == (iRgiIndexSize-1) && modeIndex == ET_MODE) || (rgiIndex == (iRgiIndexSize-1) && modeIndex == EPT_MODE && sweep_param.ET_Swp2_Meas_Pwr_List.size () == 0 ))
            lastTrigger = 1;

         RFCal_Helper_setOpCode_XPT2(Sweep[recordIndex], ESC_XPT_ACTION_IQ_CAP /* xptAction */, dev /* cfg_dev */, Feedback_Device_ID, NULL, 
                                     (modeIndex == EPT_MODE)?ESC_XPT_CAP_EPT:ESC_XPT_CAP_ET,  sweep_param.XPT_Swp2_Num_IQ_Samples,
                                     firstTrigger /* first trigger */,
                                     lastTrigger,
                                     ESC_XPT_CAP_PROC_SWP2 );

         RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
      }
   }

   //****************************************************************//
   // Data processing - Insert NoP (OpCode 0x0)
   //****************************************************************//
   RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.XPT_Swp2_IQ_Processing_Segment_Length, Sweep[recordIndex].opcode.segLen);

   //****************************************************************//
   // OpCode: 0x800 Data - DPD Result Processing
   //****************************************************************//
   //EPT_MODE
   for(int modeIndex = 0; modeIndex < 2; modeIndex++)  // modeIndex = 0: EPT, 1: ET
   {
      if (modeIndex == EPT_MODE && sweep_param.EPT_Swp2_Meas_Pwr_List.size() == 0) continue;
      if (modeIndex == ET_MODE && sweep_param.ET_Swp2_Meas_Pwr_List.size() == 0) continue;

      Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;
      Sweep[recordIndex].resExpected = true;
      Sweep[recordIndex].txPurpose = static_cast<word>(0);

        RFCal_Helper_setOpCodeData(Sweep[recordIndex], 
            cal_param, 
            sweep_param, 
            DATA_OPCODE_ACTION_DPD_PROC /* OpCode Data Option */, 
            demo_param.TECH, 
            demo_param.BAND);

      Sweep[recordIndex].data.dpdCalc.do_dpd_calc_op  = true;

      // Bit 4 - DPD Result Processing requires the following fields

      // XPT Mode for DPD Result Processing Operation
      Sweep[recordIndex].data.dpdCalc.xptMode = (modeIndex == EPT_MODE)?XPT_TYPE_EPT:XPT_TYPE_ET;

      // Number of Power Values (measured) for DPD Construction Sweep to find the RGI and Bias from M-Line
      Sweep[recordIndex].data.dpdCalc.numSwp2MeasPwrs = (modeIndex == EPT_MODE)? sweep_param.EPT_Swp2_Meas_Pwr_List.size() : sweep_param.ET_Swp2_Meas_Pwr_List.size();

      // Number of Power Values (calculated) for DPD Construction Sweep to find the RGI and Bias from M-Line
      Sweep[recordIndex].data.dpdCalc.numSwp2CalcPwrs = (modeIndex == EPT_MODE)? sweep_param.EPT_Swp2_Calc_Pwr_List.size() : 0;

      for(int index = 0; index < Sweep[recordIndex].data.dpdCalc.numSwp2MeasPwrs; index++)
      {
         // Requested (measured) power for Swp 2 DPD Construction
         Sweep[recordIndex].data.dpdCalc.measPwrSwp2DB100[index] = (word)((modeIndex == EPT_MODE)?sweep_param.EPT_Swp2_Meas_Pwr_List[index]*100.0 : sweep_param.ET_Swp2_Meas_Pwr_List[index]*100.0);
      }

      for(int index = 0; index < Sweep[recordIndex].data.dpdCalc.numSwp2CalcPwrs; index++)
      {
         // Requested (calculated) power for Swp 2 DPD Construction
         Sweep[recordIndex].data.dpdCalc.calcPwrSwp2DB100[index] = (word)((modeIndex == EPT_MODE)?sweep_param.EPT_Swp2_Calc_Pwr_List[index]*100.0 : 0);
      }
      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
   }
   //*****************************************************************************************/
   //    TDSCDMA only. Revert to default waveform
   //*****************************************************************************************/
   if (demo_param.TECH == 4)
   {
      //*****************************************************************************************//
      // OpCode 0x100: Radio Setup
      //*****************************************************************************************//
      MasterDevState = 0;
      RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex],
                                        sweep_param.Device_Selection.Master_Device_ID,
                                        sweep_param.Device_Selection.Slave_Device_ID,
                                        MasterDevState,
                                        SlaveDevState,
                                        demo_param.TECH,
                                        demo_param.BAND,
                                        sweep_param.Ref_Channel /* ulChan */,
                                        sweep_param.Waveform.Waveform_Type );
      Sweep[recordIndex].radio_cfg.flag = 0;
      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);

      //*****************************************************************************************//
      // Insert NoP (OpCode 0x0)
      //*****************************************************************************************//
      RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length, sweep_param.Segment_Length[0][0]);
   }

   //*****************************************************************************************/
   //    End of Sweep 2: Insert a low rgi segment instead of tear down
   //*****************************************************************************************/
   RFCal_Helper_setLowRGISegment(Sweep[recordIndex],cal_param.TxDeviceFlag);
   RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);

   //*********************************************//
   //    Generate OpCode for all the segment
   //*********************************************//
   Sweep.resize(recordIndex); // Delete unused vector.
   RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());

   return true;
}

unsigned char RFCal_Helper_ESC_ET_Logging_XPT_Delay(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep)
{
   HANDLE hPhone = demo_param.hPhone;

   //***********************************************************//
   //    Delay Sweep
   //***********************************************************//
   RFCal_Helper_AddMessageToQMSLLog(hPhone, "Delay Sweep");
   RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tBW\tdelayVal");

   for (UINT i = 0; i < Sweep.size(); i ++)
   {
      if (Sweep[i].subSwpType == ESC_SUB_SWEEP_DELAY_PROC)
      {
         int delayBw = Sweep[i].dataRes.delayRes.delayBw;
         int delayChan = Sweep[i].dataRes.delayRes.delayChan;
            int delayVal = static_cast<int>(Sweep[i].dataRes.delayRes.delayVal);
         stringstream tmpStr;
         tmpStr << delayChan << "\t" << delayBw << "\t" << delayVal;
         RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());
      }
   }
   return true;
}

unsigned char RFCal_Helper_ESC_ET_Logging_XPT_FreqComp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep)
{
   const int EPT_Mode = 0;
   const int ET_Mode = 1;
   HANDLE hPhone = demo_param.hPhone;
   MSXML2::IXMLDOMDocument2Ptr xmlDoc = demo_param.xmlDoc;
   MSXML2::IXMLDOMElementPtr DataSetCollectionElementPtr = NULL;

   map < RFCal_Helper_Rx_Params, int > RxParamsToLnaOffset;
   map < RFCal_Helper_Tx_Params, int > TxParamsToAptPwrSegNum;
   map < RFCal_Helper_Tx_Params, word > TxParamsToLimHdetSegNum;
   map < RFCal_Helper_Tx_Params, int > TxParamsToXptPwrSegNum;

   map < word, word > ExpRxAGCToDLLevel;
   UINT dev = 0;
   if (cal_param.Tx_Rx_Split)
   {
      dev = static_cast<UINT>(sweep_param.Device_Selection.Master_Device_ID);  
   } 
   else
   {
      dev = static_cast<UINT>(RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag));
   }

   //***********************************************************//
   //    Create Maps to store Tx and Rx data
   //***********************************************************//

   for (UINT RxIndex = 0; RxIndex < sweep_param.DL_Level_List.size(); RxIndex++)
   {
      word expRxAGC = (word)CalculateRxAGCValue(demo_param.TECH,sweep_param.DL_Level_List[RxIndex]);
      ExpRxAGCToDLLevel.insert(make_pair(expRxAGC,sweep_param.DL_Level_List[RxIndex]));
   }

   for (UINT iSegNum = 0; iSegNum < Sweep.size() - 1; iSegNum++)
   {
      if (Sweep[iSegNum].txParams.txParams[dev].pwrOverrideType == 1 && Sweep[iSegNum].txPurpose != 0 && Sweep[iSegNum].measSeg )
      {
         RFCal_Helper_Tx_Params txParam;
         txParam.PaState = Sweep[iSegNum].txParams.txParams[dev].paState;
         txParam.Channel = static_cast<word>(Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan);
         txParam.Purpose = static_cast<byte>(Sweep[iSegNum].txPurpose);
         TxParamsToAptPwrSegNum[txParam] = iSegNum;
      }
      if (Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.numHdetAvg > 0)
      {
         RFCal_Helper_Tx_Params txParam;
         txParam.PaState = Sweep[iSegNum].txParams.txParams[dev].paState;
         txParam.Channel = Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan;
         TxParamsToLimHdetSegNum[txParam] = iSegNum;
      }

      if (Sweep[iSegNum].txParams.txParams[dev].pwrOverrideType == 2 
            && Sweep[iSegNum].txPurpose == FTM_Tx_Purpose_FreqComp_Lin_vs_Freq)
      {
         RFCal_Helper_Tx_Params txParam;
         txParam.XPTmode = Sweep[iSegNum].txParams.txParams[dev].xptMode;
         txParam.Channel = static_cast<word>(Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan);
         txParam.ExpPwr =  Sweep[iSegNum].txParams.txParams[dev].expPwr;
         TxParamsToXptPwrSegNum[txParam] = iSegNum;
      }

      for (int devIndex = 0; devIndex < 8; devIndex ++)
      {
         int rx_meas_action = Sweep[iSegNum].rxMeas.actions[devIndex].rx_meas_action;
         if ( rx_meas_action == 1 || rx_meas_action== 2)
         {
            RFCal_Helper_Rx_Params rxParam;
            rxParam.Channel = static_cast<word>(Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan);
            rxParam.LNARange  =  Sweep[iSegNum].rxMeas.actions[devIndex].payloads[0].lna_state;
            rxParam.RxPM = Sweep[iSegNum].rxMeas.actions[devIndex].payloads[0].power_mode;
            rxParam.RxPath = devIndex;
            rxParam.RxMeasAction = rx_meas_action;
            RxParamsToLnaOffset[rxParam] = Sweep[iSegNum].rxRes.lnaOffset[devIndex][0]
                                           + Sweep[iSegNum].rxRes.lnaOffset[devIndex][1];
         }
      }
   } // for (UINT iSegNum = 0; iSegNum < Sweep.size(); iSegNum++)

   if (sweep_param.Sweep_Type == XPT_FREQCOMP
         || sweep_param.Sweep_Type == APT_FREQCOMP
         || sweep_param.Sweep_Type == XPT_FREQCOMP_STAGGER_RXTX
         || sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
         || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP
         || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP
      )
   {
      //***********************************************************//
      //    APT FreqComp power (Tx Linearizer Primary Freq Comp)
      //***********************************************************//
      RFCal_Helper_AddMessageToQMSLLog(hPhone, "Tx Linearizer Primary Freq Comp");

      DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
      RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: Tx Linearizer Primary Freq Comp");

      if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == 0)
      {
          RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tFBAGC\tPWR\tRefPwr\tHDET");
      }
      else if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode ==2)
      {
          RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tFBState\tRxScale\tuBias\tFBGain\tPWR\tRefPwr\tDiff");
      }
      else
      {
         RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tPWR");
      }
      for (UINT paIndex = 0; paIndex< sweep_param.PA_State_List.size(); paIndex++)
      {
         if (sweep_param.Purpose_List[paIndex] != FTM_Tx_Purpose_FreqComp_Lin_vs_Freq) continue;
         for (UINT i=0; i < sweep_param.Tx_Channel_List.size(); i++)
         {
            RFCal_Helper_Tx_Params txParam;
            txParam.PaState = sweep_param.PA_State_List[paIndex];
            txParam.Channel = (dword)sweep_param.Tx_Channel_List[i];
            txParam.Purpose = FTM_Tx_Purpose_FreqComp_Lin_vs_Freq;
            int iSegNum = TxParamsToAptPwrSegNum[txParam];
            int rgi = Sweep[iSegNum].txParams.txParams[dev].rgi;
            int bias = Sweep[iSegNum].txParams.txParams[dev].bias;
            int icq = Sweep[iSegNum].txParams.txParams[dev].quiescentCurrent;
            int txpwr = static_cast<int>(static_cast<short>(Sweep[iSegNum].txRes.txPower));

               MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;

            if (Sweep[iSegNum].txPurpose == FTM_Tx_Purpose_FreqComp_Lin_vs_Freq)
            {
               stringstream tmpStr;
               tmpStr << (int)txParam.Channel << "\t" << (int)txParam.PaState << "\t" << rgi << "\t" << bias << "\t" << icq << "\t";
                  DataSetElementPtr = 
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel", txParam.Channel );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"PA State", txParam.PaState);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"TxAGC", rgi);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Vcc", bias);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"ICQ", icq);

               if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN )
               {
                  double refPwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txIntCalRes.refTxPower)) / 10.0;
                  signed short fbagc = static_cast<signed short>(Sweep[iSegNum].rxRes.lnaOffset[1][0]);
                        tmpStr << fbagc << "\t" << txpwr<< "\t" << refPwr;
               }
               if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN )
               {
                      double refPwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txIntCalRes.refTxPower)) / 10.0;
                  tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState) << "\t";
                  tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale) << "\t";
                  tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias) << "\t";
                      tmpStr << static_cast<int>(Sweep[iSegNum].txRes.fbGainMeas.fbGainRes) << "\t";
                      double diff = (txpwr/10.0) - refPwr;
                
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FB State", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState );
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Rx Scale", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale );
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"uBias", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias );
                  
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"FBGain", Sweep[iSegNum].txRes.fbGainMeas.fbGainRes);
         
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"TxLinFCompPower (dBm)", txpwr/10.0);
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Reference Power", refPwr);
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Lin FComp Error", diff);
                  word MaxMagLSE = RFCal_Helper_Calculate_FB_LSError_Result(Sweep[iSegNum].txRes.fbGainMeas.fbLSError);
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Max Mag LSE", MaxMagLSE);
    
                      tmpStr << (txpwr/10.0) << "\t" << refPwr<< "\t" << diff<< "\t" ;
               }
               else
               {
                        tmpStr << (txpwr/10.0);
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"TxLinFCompPower (dBm)", txpwr/10.0);
               }
               RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());
            }
         }
      }

      if (TxParamsToLimHdetSegNum.size() > 0)
      {
         RFCal_Helper_AddMessageToQMSLLog(hPhone, "Tx Limiting Freq Comp");
         DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
         RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: Tx Limiting Primary Freq Comp");
         
         if (  sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
               || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP
               || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP)
         {
            RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tFBAGC\tPWR\tHDET");
         }
         else
         {
            RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tPWR\tHDET");
         }
         for (UINT i=0; i < sweep_param.Tx_Channel_List.size(); i++)
         {
               MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;

            RFCal_Helper_Tx_Params txParam;
            txParam.PaState = sweep_param.PA_State_List[0];
            txParam.Channel = sweep_param.Tx_Channel_List[i];
            int iSegNum = TxParamsToLimHdetSegNum[txParam];
            int rgi = Sweep[iSegNum].txParams.txParams[dev].rgi;
            int bias = Sweep[iSegNum].txParams.txParams[dev].bias;
            int icq = Sweep[iSegNum].txParams.txParams[dev].quiescentCurrent;
            int txpwr = static_cast<int>(Sweep[iSegNum].txRes.txPower);
            int hdet =  Sweep[iSegNum].txRes.hdet[0][0];

            DataSetElementPtr = 
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel", txParam.Channel );
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"PA State", txParam.PaState);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"TxAGC", rgi);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Vcc", bias);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"ICQ", icq);

            stringstream tmpStr;
            if ( sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
                  || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP
                  || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP)
            {
               signed short fbagc = static_cast<signed short>(Sweep[iSegNum].rxRes.lnaOffset[1][0]);
               tmpStr   << (int)txParam.Channel << "\t" << (int)txParam.PaState << "\t" << rgi << "\t" << bias << "\t" << icq
                                << "\t" << fbagc << "\t" << (txpwr/10.0) << "\t" << hdet;
            }
            else
            {
               tmpStr   << (int)txParam.Channel << "\t" << (int)txParam.PaState << "\t" << (int)rgi << "\t"
                        << bias << "\t" << icq << "\t" << txpwr << "\t" << hdet;
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"TxLimFCompPower (dBm)", txpwr/10.0);

            }
            RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());
         }
      }
      //***********************************************************//
      //    XPT FreqComp power (exclude fudge segment)
      //***********************************************************//
        if (  sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
            || sweep_param.Sweep_Type == XPT_FREQCOMP )
        {

      RFCal_Helper_AddMessageToQMSLLog(hPhone, "Tx XPT - FreqComp");
         DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
         RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: Tx XPT - FreqComp");

           MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;

         if ( RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN )
      {
            RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tXPTmode\tExpPwr\tFBAGC\tPWR\tRefPwr");
      }
         else if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN )
      {
            RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tXPTmode\tExpPwr\tFBState\tRxScale\tuBias\tFBGain\tPWR\tRefPwr\tDiff");
      }
      else
      {
         RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tXPTmode\tExpPwr\tPWR");
      }
      for (int modeIndex = 0; modeIndex < 2; modeIndex ++)  // modeIndex 0 is EPT and modeIndex 1 is ET.
      {
         int paIndexSize = (modeIndex == EPT_Mode)? sweep_param.EPT_Fcomp_Meas_Pwr_List.size() : sweep_param.ET_Fcomp_Meas_Pwr_List.size();
         for (int paIndex = 0; paIndex < paIndexSize; paIndex++)
         {
            for (UINT i=0; i < sweep_param.Tx_Channel_List.size(); i++)
            {
               RFCal_Helper_Tx_Params txParam;
               txParam.XPTmode = (modeIndex == EPT_Mode)? 1 : 2;
               txParam.Channel = sweep_param.Tx_Channel_List[i];
               txParam.ExpPwr =  static_cast<int>((modeIndex == EPT_Mode)?sweep_param.EPT_Fcomp_Meas_Pwr_List[paIndex]*100.0 : sweep_param.ET_Fcomp_Meas_Pwr_List[paIndex]*100.0);
               int iSegNum = TxParamsToXptPwrSegNum[txParam];
               int txpwr = static_cast<int>(Sweep[iSegNum].txRes.txPower);
               stringstream tmpStr;

                  MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
                  DataSetElementPtr = 
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel", txParam.Channel );
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"XPT Mode", txParam.XPTmode);
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Meas Exp Power", txParam.ExpPwr);
  
                  if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN )
               {
                  double refPwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txIntCalRes.refTxPower)) / 10.0;
                  signed short fbagc = static_cast<signed short>(Sweep[iSegNum].rxRes.lnaOffset[1][0]);
                  tmpStr   << (int)txParam.Channel  << "\t" << (int)txParam.XPTmode << "\t" << (int)txParam.ExpPwr
                                    << "\t" << fbagc << "\t" << txpwr << "\t" <<refPwr;

               }
                   else if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN )
               {
                  tmpStr << (int)txParam.Channel  << "\t" << (int)txParam.XPTmode << "\t" << (int)txParam.ExpPwr << "\t";
                  double refPwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txIntCalRes.refTxPower)) / 10.0;
                     tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState) << "\t";
                     tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale) << "\t";
                     tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias) << "\t";
                      tmpStr << static_cast<int>(Sweep[iSegNum].txRes.fbGainMeas.fbGainRes);
                      double diff = txpwr/10.0 - refPwr;
                
                      tmpStr << "\t" << (txpwr/10.0) << "\t" << refPwr<< "\t" << diff;
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FB State", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState );
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Rx Scale", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale );
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"uBias", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias );
                  
                  
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"FBGain", Sweep[iSegNum].txRes.fbGainMeas.fbGainRes);
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"XPT FComp Power", (txpwr/10.0) );
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Reference Power", (refPwr) );
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"XPT FComp Error", (diff) );
               
               }
               else
               {
                  tmpStr << (int)txParam.Channel  << "\t" << (int)txParam.XPTmode << "\t" << (int)txParam.ExpPwr << "\t" << txpwr;
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"XPT FComp Power", (txpwr/10.0) );

               }
               RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());
                   }
            }
         }
      }
   }
   if (sweep_param.Sweep_Type == XPT_FREQCOMP
         || sweep_param.Sweep_Type == APT_FREQCOMP
         || sweep_param.Sweep_Type == XPT_FREQCOMP_STAGGER_RXTX
         || sweep_param.Sweep_Type == INTRACA_FCOMP)
   {
      if (sweep_param.Meas_Rx)
      {
         int RxDevice = sweep_param.Device_Selection.Rx_Device_To_Calibrate.size();
         if ((sweep_param.Measure_CA & 1) == true )
         {
            RxDevice = 4;
         }
         RxDevice = 8;
         UINT iNumLoop = 1;
         short channel_offset = 0;
         int rx_meas_action = 1;
         string stHeader;
         string LNAOffsetHeader;
         if (sweep_param.Measure_DC)
         {
            iNumLoop = 2;
         }

         for (UINT iLoop = 0; iLoop < iNumLoop; iLoop++)
         {
            if (sweep_param.Sweep_Type == INTRACA_FCOMP)
            {
                    stHeader = "MEASUREMENTS: IntraCA LNA Offset Freq Comp Device: ";
               LNAOffsetHeader = "IntraFCompLNAOffset";

               channel_offset = 10;
            }
            else if (iLoop == 1)
            {
                    stHeader = "MEASUREMENTS: LNA DC Offset Freq Comp Device:  ";
               LNAOffsetHeader = "DCRxFCompLNAOffset";

               rx_meas_action = 2;
            }
            else
            {
                    stHeader = "MEASUREMENTS: LNA Offset Freq Comp Device: ";
               LNAOffsetHeader = "RxFCompLNAOffset";

            }

            for (int devIndex = 0; devIndex < RxDevice; devIndex ++)
            {
               if (cal_param.RxDeviceFlag & (1 << devIndex))
               {
               stringstream tmpStr;
               tmpStr << stHeader << devIndex;
               RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());
               RFCal_Helper_AddMessageToQMSLLog(hPhone, "RxPath\tChannel\tRxLevel\tLNA\tRxPM\tLNA Offset");

               DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
               RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name",tmpStr.str());

               for (UINT chanIndex = 0 ;  chanIndex < sweep_param.Tx_Channel_List.size(); chanIndex ++)
               {
                  for (UINT RxLevelIndex = 0 ;  RxLevelIndex < sweep_param.Rx_Actions_List.size(); RxLevelIndex ++)
                  {
                     for (UINT RxPMlIndex = 0 ;  RxPMlIndex < sweep_param.Rx_PM_List.size(); RxPMlIndex ++)
                     {
                        RFCal_Helper_Rx_Params rxParam;
                        rxParam.Channel = static_cast<word>(sweep_param.Tx_Channel_List[chanIndex])+channel_offset;
                        if (rx_meas_action == 2 ) // DC cal
                        {
                           if (rxParam.Channel != sweep_param.Ref_Channel) continue;
                        }
                        rxParam.LNARange  =  sweep_param.Rx_Actions_List[RxLevelIndex];
                                double dl_level = static_cast<double>(static_cast<short>(sweep_param.DL_Level_List[rxParam.LNARange]));
                        rxParam.RxPM = sweep_param.Rx_PM_List[RxPMlIndex];
                        rxParam.RxPath = devIndex;
                        rxParam.RxMeasAction = rx_meas_action;
                        word lnaoffset = RxParamsToLnaOffset[rxParam];
                        stringstream tmpStr;
                        tmpStr   << devIndex << "\t" << (int)rxParam.Channel << "\t" << (short)dl_level << "\t"
                                            << (int)rxParam.LNARange << "\t" << (int)rxParam.RxPM << "\t" << static_cast<short>(lnaoffset);
                        RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());

                        MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
                        DataSetElementPtr = 
                        RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"RxPath", rxParam.RxPath );
                        RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Band",  demo_param.BAND);
                        RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel", rxParam.Channel );
                        RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"RxLevel(dBm)", dl_level );
                        RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"LNARange", rxParam.LNARange );
                        RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"RxPM", rxParam.RxPM );
                        RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,LNAOffsetHeader, static_cast<short>(lnaoffset) );

                     
                     }
                  }
               }
            }
         }
      }
   }
   }
   return true;
}

unsigned char RFCal_Helper_ESC_ET_Logging_XPT_Swp3(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep)
{
   HANDLE hPhone = demo_param.hPhone;

   MSXML2::IXMLDOMDocument2Ptr xmlDoc = demo_param.xmlDoc;
   MSXML2::IXMLDOMElementPtr DataSetCollectionElementPtr = NULL;

   //***********************************************************//
   //    APT Linearlizer power
   //***********************************************************//
   RFCal_Helper_AddMessageToQMSLLog(hPhone, "Swp3. APT Linearlizer");

   UINT dev = 0;
   if (cal_param.Tx_Rx_Split)
   {
      dev = static_cast<UINT>(sweep_param.Device_Selection.Master_Device_ID);  
   } 
   else
   {
      dev = static_cast<UINT>(RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag));
   }

   DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
   RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: APT Tx Linearizer Sweep");

   if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN )
    {
        RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tFBAGC\tPWR\tRefPwr\tHDET");
    }
   else if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN )
    {
        RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tFBState\tRxScale\tuBias\tFBGain\tPWR\tRefPwr\tDiff");
    }
    else
    {
   RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tPWR\tHDET");
    }
   int prevPAState = -1;
   for (UINT iSegNum = 0; iSegNum < Sweep.size() - 1; iSegNum++)
   {
      if (Sweep[iSegNum].measSeg && Sweep[iSegNum].txParams.txParams[dev].pwrOverrideType == 1 && Sweep[iSegNum].txParams.txParams[dev].rgi > 0)  // Tx Pwr
      {
         int pa = Sweep[iSegNum].txParams.txParams[dev].paState;
         int rgi = Sweep[iSegNum].txParams.txParams[dev].rgi;
         int bias =  Sweep[iSegNum].txParams.txParams[dev].bias;
         int icq =  Sweep[iSegNum].txParams.txParams[dev].quiescentCurrent;
         double pwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.txPower)) / 10.0;
         int   hdet = Sweep[iSegNum].txRes.hdet[dev][0];
         stringstream tmpStr;
            tmpStr << sweep_param.Ref_Channel << "\t" << pa << "\t" << rgi << "\t" << bias << "\t" << icq<< "\t" ;

            MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
            DataSetElementPtr = 
         RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel", sweep_param.Ref_Channel);
         RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"PA State", pa);
         RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"TxAGC", rgi);
         RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Vcc", bias);
         RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"ICQ", icq);

         if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type )  && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
         {
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"FBGain", Sweep[iSegNum].txRes.fbGainMeas.fbGainRes);
         }
         RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"AptPower", pwr);
         if (hdet > 0)
         {
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"AptHDET", hdet);
         }
         else
         {
            string stTmp = "-";
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"AptHDET", stTmp);
         }

            if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == 0)
            {
                double refPwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txIntCalRes.refTxPower)) / 10.0;
                tmpStr << pwr << "\tr" << refPwr;
            }
            else if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
            {
                double refPwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txIntCalRes.refTxPower)) / 10.0;
            tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState) << "\t";
            tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale) << "\t";
            tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias) << "\t";
                tmpStr << static_cast<int>(Sweep[iSegNum].txRes.fbGainMeas.fbGainRes) << "\t";
            double diff = pwr - refPwr;
                
                tmpStr << (pwr) << "\t" << refPwr<< "\t" << diff<< "\t" ;

            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FB State", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState );
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Rx Scale", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale );
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"uBias", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias );
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Reference Power", refPwr);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"APT Lin Swp Error", diff);
            word MaxMagLSE = RFCal_Helper_Calculate_FB_LSError_Result(Sweep[iSegNum].txRes.fbGainMeas.fbLSError);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Max Mag LSE", MaxMagLSE);

            double refPwrDiff = 0.0;
            double PwrDiff = 0.0;
            if (prevPAState == pa)
            {
               for (vector<cal_sweep_record>::const_iterator it = Sweep.begin(); it != Sweep.end(); it++) // Find the previous RGI
               {
                  if (  it->measSeg
                     && it->txParams.txParams[dev].paState == pa 
                     && it->txParams.txParams[dev].rgi == (rgi+1) )
                  {
                     PwrDiff = static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.txPower - it->txRes.txPower)) / 10.0;
                        refPwrDiff = static_cast<double>(static_cast<short>(Sweep[iSegNum].txIntCalRes.refTxPower - it->txIntCalRes.refTxPower)) / 10.0;
                      break;
                  }
               }    
            }
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Tx Int Cal Calculated Power Delta", PwrDiff);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Box Measured Power Delta", refPwrDiff);
           if (Sweep[iSegNum].txRes.fbPower == 0)
            {
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Tx Int Cal result Power",0);//static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.fbPower))/10.0);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Diff TxInt Cal",0);//static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.fbPower))/10.0 - refPwr);
            }
            else
            {
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Tx Int Cal result Power", static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.fbPower))/10.0);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Diff TxInt Cal",static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.fbPower))/10.0 - refPwr);
             }

            }
            else
            {
                tmpStr << "\t" << pwr << "\t" << hdet;
            }
        
         RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());
         prevPAState = pa;
        }
    }

  if ( sweep_param.Sweep_Type == INT_CAL_XPT_SWP3 && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
   {
      DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
      RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: APT Tx Linearizer Overlap Sweep");
       for (vector < cal_sweep_record >::const_iterator it = Sweep.begin(); it != (Sweep.end() -1); it++)
      {
         const cal_sweep_var_len_tx_params * curTxParam = &(it->txParams.txParams[dev]);
         const cal_sweep_var_len_tx_params * nextTxParam = &((it+1)->txParams.txParams[dev]);

         if (  (it->subSwpType == 0)
            && (curTxParam->paState == nextTxParam->paState)
            && (curTxParam->rgi == nextTxParam->rgi)
            && (it->txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState != (it+1)->txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState)
            && (curTxParam->paState != 255))
         {
            for (vector < cal_sweep_record >::const_iterator sit = it; sit !=it+2; sit++)
            {
                  MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;

                  DataSetElementPtr = 
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel", sweep_param.Ref_Channel);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"PA State", sit->txParams.txParams[dev].paState);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"TxAGC", sit->txParams.txParams[dev].rgi);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Vcc", sit->txParams.txParams[dev].bias);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"ICQ", sit->txParams.txParams[dev].quiescentCurrent);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FB State", sit->txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Rx Scale", sit->txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"uBias", sit->txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"FBGain", sit->txRes.fbGainMeas.fbGainRes);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Power", (static_cast<double>(static_cast<short>(sit->txRes.txPower))/10.0));
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Reference Power", (static_cast<double>(static_cast<short>(sit->txIntCalRes.refTxPower))/10.0));
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Diff Power", (static_cast<double>(static_cast<short>(sit->txRes.txPower - sit->txIntCalRes.refTxPower))/10.0));

               word MaxMagLSE = RFCal_Helper_Calculate_FB_LSError_Result(sit->txRes.fbGainMeas.fbLSError);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Max Mag LSE", MaxMagLSE);


            }
         }
      }

   }


   if (sweep_param.Sweep_Type == XPT_SWP3
        || sweep_param.Sweep_Type == INT_CAL_XPT_SWP3
      )
   {
      //***********************************************************//
      //    Tx Xpt Swp 3 - Linearlizer
      //***********************************************************//
        RFCal_Helper_AddMessageToQMSLLog(hPhone, "MEASUREMENTS: Tx XPT Swp3 - Linearizer");
      DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
      RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: Tx XPT Swp3 - Linearizer");

      if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN )
      {
            RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tXPT\tPA\tRGI\tBIAS\tExpPwr\tFBAGC\tPWR\tRefPwr");
      }
      else if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN )
       {
        RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tXPT\tPA\tRGI\tBIAS\tExpPwr\tFBState\tRxScale\tuBias\tFBGain\tPWR\tRefPwr\tDiff");
      }
      else
      {
         RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tXPT\tPA\tRGI\tBIAS\tExpPwr\tPWR");
      }
      for (UINT iSegNum = 0; iSegNum < Sweep.size(); iSegNum++)
      {
            if (Sweep[iSegNum].measSeg && Sweep[iSegNum].subSwpType == ESC_SUB_SWEEP_APPLY_DPD)  // Tx Pwr
         {
            int xpt = Sweep[iSegNum].txParams.txParams[dev].xptMode;
            int pa = Sweep[iSegNum].txParams.txParams[dev].paState;
            int rgi = Sweep[iSegNum].txParams.txParams[dev].rgi;
            int bias =  Sweep[iSegNum].txParams.txParams[dev].bias;
            word expPwr =  Sweep[iSegNum].txParams.txParams[dev].expPwr;
            double pwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.txPower)) / 10.0;


               MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
               DataSetElementPtr = 

            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel", sweep_param.Ref_Channel);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"XPT Mode", xpt);

            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"PA State", pa);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"TxAGC", rgi);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Vcc", bias);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Exp Power", expPwr);

            if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type )  && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
            {
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"FBGain", Sweep[iSegNum].txRes.fbGainMeas.fbGainRes);
            }
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"XPT Swp3 Power", pwr);

            stringstream tmpStr;
            tmpStr << sweep_param.Ref_Channel << "\t" << xpt << "\t" <<  pa << "\t" << rgi << "\t" << bias << "\t" << expPwr << "\t" ;
            if (sweep_param.Sweep_Type == INT_CAL_XPT_SWP3 && sweep_param.Fb_Rx_Gain_Cal_Mode == 0)
            {
               signed short fbagc = static_cast<signed short>(Sweep[iSegNum].rxRes.lnaOffset[1][0]);
                    tmpStr << fbagc << "\t" << pwr;
            }
            else if (sweep_param.Sweep_Type == INT_CAL_XPT_SWP3 && sweep_param.Fb_Rx_Gain_Cal_Mode == 2)
            {
               double refPwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txIntCalRes.refTxPower)) / 10.0;
               tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState) << "\t";
               tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale) << "\t";
               tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias) << "\t";
                   tmpStr << static_cast<int>(Sweep[iSegNum].txRes.fbGainMeas.fbGainRes);
                      double diff = pwr - refPwr;
                
                      tmpStr << "\t" << pwr << "\t" << refPwr<< "\t" << diff;
                 
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FB State", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Rx Scale", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"uBias", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Reference Power", refPwr);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Tx XPT Sweep 3 Lin Error", diff);
               word MaxMagLSE = RFCal_Helper_Calculate_FB_LSError_Result(Sweep[iSegNum].txRes.fbGainMeas.fbLSError);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Max Mag LSE", MaxMagLSE);
            }
            else
            {
                    tmpStr << pwr;
            }
            RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());
         }
      }
   }

   //***********************************************************//
   //    Tx Xpt Swp 3 - Thermo ADC measurement
   //***********************************************************//
   if (sweep_param.Sweep_Type == XPT_SWP3
        || sweep_param.Sweep_Type == INT_CAL_XPT_SWP3)
    {
   RFCal_Helper_AddMessageToQMSLLog(hPhone, "Tx Xpt Swp 3 - Thermo ADC measurement");
   RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tTherm Read");

      DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
      RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: Tx XPT Swp3 - Therm ADC Meas");

   int etIndex = 0;
   for (UINT iSegNum = 0; iSegNum < Sweep.size(); iSegNum++)
   {
      if (Sweep[iSegNum].subSwpType == ESC_SUB_SWEEP_THERM_READ)  // Tx Pwr
      {
            int adc =   Sweep[iSegNum].txRes.hdet[dev][2];
         stringstream tmpStr;
         tmpStr << sweep_param.Ref_Channel << "\t" << adc;
         RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());

           MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
               DataSetElementPtr = 
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel", sweep_param.Ref_Channel);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Therm Read", adc);
         }
       }
   }
   return true;
}

unsigned char RFCal_Helper_ESC_ET_Logging_XPT_Swp2(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep)
{
   HANDLE hPhone = demo_param.hPhone;

   RFCal_Helper_AddMessageToQMSLLog(hPhone, "Swp2. Detrough");
   stringstream ss;
   for (UINT i = 0; i < Sweep.size(); i++)
   {
      if (Sweep[i].dataRes.measDpdRes[0].dpdValid == 1)
      {
         byte numAMAMres = Sweep[i].data.dpdCalc.numSwp2MeasPwrs;
         ss << "\nSegment [ " << i << " ]\t AMAM AMPM Info" << endl << endl;
         ss << "DPDValid";
         for (int j = 0; j < numAMAMres; j++)
         {
            ss << "\t" << (long)Sweep[i].dataRes.measDpdRes[j].dpdValid ;
         }
         ss << endl;
         ss << "XPT Mode";
         for (int j = 0; j < numAMAMres; j++)
         {
            ss << "\t" << (long)Sweep[i].dataRes.measDpdRes[j].xptMode ;
         }
         ss << endl;
         ss << "RGI\t";
         for (int j = 0; j < numAMAMres; j++)
         {
            ss << "\t" << (long)Sweep[i].dataRes.measDpdRes[j].rgi ;
         }
         ss << endl;
         ss << "SMPS Bias";
         for (int j = 0; j < numAMAMres; j++)
         {
            ss << "\t" << (long)Sweep[i].dataRes.measDpdRes[j].smpsBias ;
         }
         ss << endl;
         ss << "PA state";
         for (int j = 0; j < numAMAMres; j++)
         {
            ss << "\t" << (long)Sweep[i].dataRes.measDpdRes[j].paState ;
         }
         ss << endl;
         ss << "Measured";
         for (int j = 0; j < numAMAMres; j++)
         {
            ss << "\t" << (long)Sweep[i].dataRes.measDpdRes[j].measured ;
         }
         ss << endl;
         ss << "Power\t";
         for (int j = 0; j < numAMAMres; j++)
         {
            ss << "\t" << (long)Sweep[i].dataRes.measDpdRes[j].power ;
         }
         ss << endl << endl;

         for (int k = 0; k < 16; k++)
         {
            ss << "AMAM [ " << k <<  " ]\t";
            for (int j = 0; j < numAMAMres; j++)
            {
               ss << (long)Sweep[i].dataRes.measDpdRes[j].amamRes[k]  << "\t";
            }
            ss << endl;
         }
         ss << endl;
         for (int k = 0; k < 16; k++)
         {
            ss << "AMPM [ " << k << " ]\t";
            for (int j = 0; j < numAMAMres; j++)
            {
               ss << (short)Sweep[i].dataRes.measDpdRes[j].ampmRes[k]  << "\t";
            }
            ss << endl;
         }
         ss << endl;
      }
      if (Sweep[i].dataRes.measLoadEPDTRes[0].detValid == 1)
      {
         ss << "\nSegment [ " << i << " ]\t Detrouph Info" << endl<< endl;
         for (int k = 0; k < 128; k++)
         {
            ss << "Detrough[ " << k << " ]\t";
            for (int j = 0; j < 4; j++)
            {
               ss << (long)Sweep[i].dataRes.measLoadEPDTRes[j].epdtTbl[k]  << "\t";
            }
            ss << endl;
         }
         ss << endl << endl;
      }
   }
   RFCal_Helper_AddMessageToQMSLLog(hPhone, ss.str());
   for (UINT i = 0; i < Sweep.size(); i ++)
   {
      if (Sweep[i].data.dataAction == 16 && Sweep[i].data.dpdCalc.xptMode == 2)   // ET
      {
         for (UINT j=0; j < sweep_param.ET_Swp2_Meas_Pwr_List.size(); j ++)
         {
            if (Sweep[i].dataRes.measDpdRes[j].dpdValid == 0)
               throw RFCal_Helper_Exception("DPD valid for ET is zero. Check Mline Result.",  __FILE__, __LINE__ );
         }
      }
      else if (Sweep[i].data.dataAction == 16 && Sweep[i].data.dpdCalc.xptMode == 1)     // EPT
      {
         for (UINT j=0; j < sweep_param.EPT_Swp2_Meas_Pwr_List.size(); j ++)
         {
            if (Sweep[i].dataRes.measDpdRes[j].dpdValid == 0)
               throw RFCal_Helper_Exception("DPD valid for EPT is zero. Check Mline Result.",  __FILE__, __LINE__ );
         }
      }
   }
   return true;
}

unsigned char RFCal_Helper_ESC_ET_Logging_XPT_Swp1(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep)
{
   HANDLE hPhone = demo_param.hPhone;

   MSXML2::IXMLDOMDocument2Ptr xmlDoc = demo_param.xmlDoc;
   MSXML2::IXMLDOMElementPtr DataSetCollectionElementPtr = NULL;

   UINT dev = 0;
   if (cal_param.Tx_Rx_Split)
   {
      dev = static_cast<UINT>(sweep_param.Device_Selection.Master_Device_ID);  
   } 
   else
   {
      dev = static_cast<UINT>(RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag));
   }

   
   //***********************************************************//
   //    MAX Bias Linearlizer power
   //***********************************************************//
   RFCal_Helper_AddMessageToQMSLLog( hPhone, "Swp1. Max Bias Linearlizer");
   DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
   RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: Tx Linearizer Sweep");

    if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN)
    {
        RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tFBAGC\tPWR\tRefPwr\tHDET");
    }
    else if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode ==FBRX_UKERNEL_GAIN)
    {
        RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tFBState\tRxScale\tuBias\tFBGain\tPWR\tRefPwr\tDiff");
   }
   else
   {
      RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tPWR\tHDET");
   }
   int prevPAState = -1;
   for (UINT iSegNum = 0; iSegNum < Sweep.size() - 1; iSegNum++)
   {
        if (Sweep[iSegNum].measSeg && (Sweep[iSegNum].txPurpose == 3 || Sweep[iSegNum].txPurpose == 255) ) // Tx Pwr
      {
         int pa = Sweep[iSegNum].txParams.txParams[dev].paState;
         int rgi = Sweep[iSegNum].txParams.txParams[dev].rgi;
         int bias =  Sweep[iSegNum].txParams.txParams[dev].bias;
         int icq =  Sweep[iSegNum].txParams.txParams[dev].quiescentCurrent;
         double pwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.txPower)) / 10.0;
            double refPwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txIntCalRes.refTxPower)) / 10.0;
         word hdet = Sweep[iSegNum].txRes.hdet[dev][0];
         signed short fbagc = static_cast<signed short>(Sweep[iSegNum].rxRes.lnaOffset[1][0]);
         stringstream tmpStr;
         tmpStr << sweep_param.Ref_Channel << "\t" << pa << "\t" << rgi << "\t" << bias << "\t" << icq << "\t";

            MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
            DataSetElementPtr = 
         RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel", sweep_param.Ref_Channel);
         RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"PA State", pa);
         RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"TxAGC", rgi);
         RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Vcc", bias);
         RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"ICQ", icq);

         if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type )  && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
         {
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"FBGain", Sweep[iSegNum].txRes.fbGainMeas.fbGainRes);
         }
         RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Power", pwr);
         if (hdet > 0)
         {
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"HDET", hdet);
         }
         else
         {
            string stTmp = "-";
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"HDET", stTmp);
         }
            if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type )  && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN)
         {
                tmpStr << fbagc << "\t" << hdet;
            } else if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type )  && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
            {
            tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState) << "\t";
            tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale) << "\t";
            tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias) << "\t";
                tmpStr << static_cast<int>(Sweep[iSegNum].txRes.fbGainMeas.fbGainRes) << "\t";
             double diff = pwr - refPwr;
                
                 tmpStr  << (pwr) << "\t" << refPwr<< "\t" << diff << "\t";
            
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FB State", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState );
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Rx Scale", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale );
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"uBias", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias );
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Reference Power", refPwr);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Max Bias Lin Swp Error", diff);
            word MaxMagLSE = RFCal_Helper_Calculate_FB_LSError_Result(Sweep[iSegNum].txRes.fbGainMeas.fbLSError);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Max Mag LSE", MaxMagLSE);

            double refPwrDiff = 0.0;
            double PwrDiff = 0.0;
            if (prevPAState == pa)
            {
               for (vector<cal_sweep_record>::const_iterator it = Sweep.begin(); it != Sweep.end(); it++) // Find the previous RGI
               {
                  if (  it->measSeg
                     && it->txParams.txParams[dev].paState == pa 
                     && it->txParams.txParams[dev].rgi == (rgi+1) )
                  {
                     PwrDiff = static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.txPower - it->txRes.txPower)) / 10.0;
                        refPwrDiff = static_cast<double>(static_cast<short>(Sweep[iSegNum].txIntCalRes.refTxPower - it->txIntCalRes.refTxPower)) / 10.0;
                      break;
                  }
               }
            }
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Tx Int Cal Calculated Power Delta", PwrDiff);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Box Measured Power Delta", refPwrDiff);
            if (Sweep[iSegNum].txRes.fbPower == 0)
            {
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Tx Int Cal result Power",0);//static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.fbPower))/10.0);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Diff TxInt Cal",0);//static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.fbPower))/10.0 - refPwr);
            }
            else
            {
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Tx Int Cal result Power",static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.fbPower))/10.0);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Diff TxInt Cal",static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.fbPower))/10.0 - refPwr);
            }

         } 
         else 
         {
                tmpStr << pwr << "\t" << hdet;
         }
         RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());
         prevPAState = pa;
        
        }
    }
    if ( sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
   {
      DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
      RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: Tx Linearizer Overlap Sweep");
       for (vector < cal_sweep_record >::const_iterator it = Sweep.begin(); it != (Sweep.end() -1); it++)
      {
         const cal_sweep_var_len_tx_params * curTxParam = &(it->txParams.txParams[dev]);
         const cal_sweep_var_len_tx_params * nextTxParam = &((it+1)->txParams.txParams[dev]);

         if (  
               (it->subSwpType == 0)
            && (curTxParam->paState == nextTxParam->paState)
            && (curTxParam->rgi == nextTxParam->rgi)
            && (it->txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState != (it+1)->txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState))
         {
            for (vector < cal_sweep_record >::const_iterator sit = it; sit !=it+2; sit++)
            {
                  MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;

                  DataSetElementPtr = 
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel", sweep_param.Ref_Channel);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"PA State", sit->txParams.txParams[dev].paState);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"TxAGC", sit->txParams.txParams[dev].rgi);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Vcc", sit->txParams.txParams[dev].bias);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"ICQ", sit->txParams.txParams[dev].quiescentCurrent);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FB State", sit->txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Rx Scale", sit->txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"uBias", sit->txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"FBGain", sit->txRes.fbGainMeas.fbGainRes);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Power", (static_cast<double>(static_cast<short>(sit->txRes.txPower))/10.0));
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Reference Power", (static_cast<double>(static_cast<short>(sit->txIntCalRes.refTxPower))/10.0));
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Diff Power", (static_cast<double>(static_cast<short>(sit->txRes.txPower - sit->txIntCalRes.refTxPower))/10.0));

               word MaxMagLSE = RFCal_Helper_Calculate_FB_LSError_Result(sit->txRes.fbGainMeas.fbLSError);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Max Mag LSE", MaxMagLSE);
              
            }
         }
      }

   }

   if (sweep_param.Sweep_Type == XPT_SWP1
         || sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 )
   {
      //***********************************************************//
      //     Tx XPT Swp1 - Alignment Sweep
      //***********************************************************//
      RFCal_Helper_AddMessageToQMSLLog(hPhone, "Tx XPT Swp1 - Alignment Sweep");
   DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
   RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: Tx XPT Swp1 - Alignment Sweep");

  if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN)
    {
        RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tFBAGC\tPWR\tRefPwr\tHDET");
    }
    else if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode ==FBRX_UKERNEL_GAIN)
    {
        RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tFBState\tRxScale\tuBias\tFBGain\tPWR\tRefPwr\tHDET");
      }
      else
      {
        RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tPWR\tHDET");
      }
      for (UINT iSegNum = 0; iSegNum < Sweep.size(); iSegNum++)
      {
         if (Sweep[iSegNum].subSwpType == ESC_SUB_SWEEP_XPT_ALIGNMENT) // Tx Pwr
         {
            int pa = Sweep[iSegNum].txParams.txParams[dev].paState;
            int rgi = Sweep[iSegNum].txParams.txParams[dev].rgi;
            int bias =  Sweep[iSegNum].txParams.txParams[dev].bias;
            int icq =  Sweep[iSegNum].txParams.txParams[dev].quiescentCurrent;
            double pwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.txPower)) / 10.0;
            double refPwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txIntCalRes.refTxPower)) / 10.0;
            signed short fbagc = static_cast<signed short>(Sweep[iSegNum].rxRes.lnaOffset[1][0]);

            MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
            DataSetElementPtr = 
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel", sweep_param.Ref_Channel);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"PA State", pa);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"TxAGC", rgi);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Vcc", bias);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"ICQ", icq);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Swp1IQGain", Sweep[iSegNum].txParams.txParams[dev].iqGain);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Swp1EnvScale", Sweep[iSegNum].txParams.txParams[dev].envScale);
            if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type )  && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
            {
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"FBGain", Sweep[iSegNum].txRes.fbGainMeas.fbGainRes);
            }
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"XPT Swp1 Power", pwr);

            stringstream tmpStr;
            if (sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN)
            {
                    tmpStr << sweep_param.Ref_Channel << "\t" << pa << "\t" << rgi << "\t" << bias << "\t" << icq << "\t" << fbagc << "\t" << pwr << "\t" << refPwr;
            } 
            else if (sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
            {
                tmpStr << sweep_param.Ref_Channel << "\t" << pa << "\t" << rgi << "\t" << bias << "\t" << icq << "\t" ;
               tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState) << "\t";
               tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale) << "\t";
               tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias) << "\t";
                tmpStr << static_cast<int>(Sweep[iSegNum].txRes.fbGainMeas.fbGainRes) << "\t";
            double diff;
            diff = pwr - refPwr;
                tmpStr << pwr << "\t" << refPwr << "\t" << diff<< "\t";
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FB State", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Rx Scale", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"uBias", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Reference Power", refPwr);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Allignment Sweep Error", diff);
               word MaxMagLSE = RFCal_Helper_Calculate_FB_LSError_Result(Sweep[iSegNum].txRes.fbGainMeas.fbLSError);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Max Mag LSE", MaxMagLSE);

            }
            else
            {
               tmpStr << sweep_param.Ref_Channel << "\t" << pa << "\t" << rgi << "\t" << bias << "\t" << icq << "\t" << pwr;
            }
            RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());
         }
      }
   }

   if (sweep_param.Sweep_Type == XPT_SWP1
         || sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 )
   {
      //***********************************************************//
      //     Tx XPT Swp1 - RGI Delta Sweep
      //***********************************************************//
   DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
   RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: Tx XPT Swp1 - RGI Delta Sweep");

    if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == 0)
    {
        RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tFBAGC\tPWR\tRefPwr\tHDET");
    }
   else if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
    {
        RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tFBState\tRxScale\tuBias\tFBGain\tPWR\tRefPwr\tDiff");
      }
      else
      {
        RFCal_Helper_AddMessageToQMSLLog( hPhone, "Channel\tPA\tRGI\tBIAS\tICQ\tPWR\tHDET");
      }
      for (UINT iSegNum = 0; iSegNum < Sweep.size(); iSegNum++)
      {
         if (Sweep[iSegNum].subSwpType == ESC_SUB_SWEEP_RGI_DELTA) // Tx Pwr
         {
            int pa = Sweep[iSegNum].txParams.txParams[dev].paState;
            int rgi = Sweep[iSegNum].txParams.txParams[dev].rgi;
            int bias =  Sweep[iSegNum].txParams.txParams[dev].bias;
            int icq =  Sweep[iSegNum].txParams.txParams[dev].quiescentCurrent;
            double pwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.txPower)) / 10.0;
            signed short fbagc = static_cast<signed short>(Sweep[iSegNum].rxRes.lnaOffset[1][0]);
                double refPwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txIntCalRes.refTxPower)) / 10.0;
            stringstream tmpStr;

            MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
            DataSetElementPtr = 
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel", sweep_param.Ref_Channel);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"PA State", pa);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"TxAGC", rgi);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Vcc", bias);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"ICQ", icq);

            if (RFCal_Helper_Is_IntCal_SweepType(sweep_param.Sweep_Type )  && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
            {
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"FBGain", Sweep[iSegNum].txRes.fbGainMeas.fbGainRes);
            }
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"XPT Swp1 RGI Delta Swp Power", pwr);

            if (sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 && sweep_param.Fb_Rx_Gain_Cal_Mode == 0)
            {
                    tmpStr << sweep_param.Ref_Channel << "\t" << pa << "\t" << rgi << "\t" << bias << "\t" << icq << "\t" << fbagc << "\t" << pwr << "\t" << refPwr;
            } 
            else if (sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
            {
                tmpStr << sweep_param.Ref_Channel << "\t" << pa << "\t" << rgi << "\t" << bias << "\t" << icq << "\t" ;
               tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState) << "\t";
               tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale) << "\t";
               tmpStr << static_cast<int>(Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias) << "\t";
                tmpStr << static_cast<int>(Sweep[iSegNum].txRes.fbGainMeas.fbGainRes) << "\t";
            double diff = pwr -refPwr;
                tmpStr << pwr << "\t" << refPwr<< "\t" << diff;
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FB State", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Rx Scale", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"uBias", Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias );
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Reference Power", refPwr);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"RGI Delta Sweep Error", diff);
               word MaxMagLSE = RFCal_Helper_Calculate_FB_LSError_Result(Sweep[iSegNum].txRes.fbGainMeas.fbLSError);
               RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Max Mag LSE", MaxMagLSE);

            }
            else
            {
               tmpStr << sweep_param.Ref_Channel << "\t" << pa << "\t" << rgi << "\t" << bias << "\t" << icq << "\t" << pwr;
            }
            RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());
         }
      }
   }

    return true;
}
unsigned char RFCal_Helper_ESC_ET_Logging_RsbCal(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep)
{
    HANDLE hPhone = demo_param.hPhone;
   MSXML2::IXMLDOMDocument2Ptr xmlDoc = demo_param.xmlDoc;
   MSXML2::IXMLDOMElementPtr DataSetCollectionElementPtr = NULL;

    //***********************************************************//
    //      FBRx RSB /Rx RSB Cal Logging
    //***********************************************************//

   DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,demo_param.TestPtr,"DataSetCollection","");
   RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: Rx RSB Cal");


    for (UINT iSegNum = 0; iSegNum < Sweep.size() - 1; iSegNum++)
    {
        if ((Sweep[iSegNum].EnhIntDevCal.int_cal_type_mask == ENH_INT_DEV_CAL_RSB_GET_RESULT) && (Sweep[iSegNum].rx_fbrxRsbRes.rsb_cal_flag!=3))
        {
            byte res_dev_mask = Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.device_mask;
            
            //***********************************************************//
            //       FBRx RSB Logging
            //***********************************************************//
            
            if (Sweep[iSegNum].EnhIntDevCal.rsb_instr.rsb_type == FTM_ENH_INTERNAL_DEVICE_CAL_FB_RSB)
            {
                RFCal_Helper_AddMessageToQMSLLog(hPhone, "MEASUREMENTS: FBRx RSB Cal" );
                stringstream ss;
                for (byte i = 0; i < FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_MAX; i++)
                {
                    if( res_dev_mask & (1<<i) )
                    {
                        byte tech = Sweep[iSegNum].EnhIntDevCal.tech;
                        byte band = static_cast<byte>(Sweep[iSegNum].EnhIntDevCal.band);
                        dword ref_chan = Sweep[iSegNum].EnhIntDevCal.ref_chan;
                        ss << "Technology: " << static_cast<UINT>(tech) << " Band: " <<  static_cast<UINT>(band) << " RefChan: " <<  static_cast<UINT>(ref_chan);
                        RFCal_Helper_AddMessageToQMSLLog(hPhone,  (ss.str()) ); ss.str("");
                        for ( short j = 0; j < Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.num_gain_states; j++)
                        {

                            byte gain_state =  Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.gain_states[j];

                            word rsb_before_cal = static_cast<unsigned short>( Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].rsb_before_cal );
                            word rsb_after_cal = static_cast<unsigned short> (Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].rsb_after_cal);

                            ss  <<  "Rsb Cal Path: " << static_cast<UINT>(i) << " / " 
                                        << "Gain States: "<< static_cast<int>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.gain_states[j]) <<  " / "
                                        << "Nv_active: " << static_cast<int>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].nv_active) <<  " / "
                                        << "Sin_theta_before_cal: " << static_cast<short>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].nv_active) <<  " / "
                                        << "Cos_theta_before_cal: " << static_cast<short>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].sin_theta_final) <<  " / "
                                        << "Gain_inv_before_cal: " << static_cast<short>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].cos_theta_final) <<  " / "
                                        << "Rsb_before_cal: " << static_cast<short>(rsb_before_cal) <<  " / ";
                            RFCal_Helper_AddMessageToQMSLLog(hPhone,  (ss.str()) ); ss.str("");
                            if ((rsb_before_cal <50) && ( rsb_after_cal < 99) && ( sweep_param.FBRxRsb_DbgLevel != 0 ))
                            {
                            ss          << "Sin_theta_after_cal: " << static_cast<short>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].sin_theta_after_cal) <<  " / "
                                        << "Cos_theta_after_cal: "<< static_cast<short>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].cos_theta_after_cal) <<  " / "
                                        << "gain_inv_after_cal: " << static_cast<short>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].gain_inv_after_cal) <<  " / "
                                        << "FBRx_Rsb_after_cal: " << static_cast<short>(rsb_after_cal) <<  " / ";
                            RFCal_Helper_AddMessageToQMSLLog(hPhone,  (ss.str()) ); ss.str("");
                            }
                            
                            if (sweep_param.Do_FBRxRsb_Limit_Check)
                            {
                                if (sweep_param.FBRxRsb_DbgLevel == 0)
                                {
                                    if (!RFCal_Helper_EvaluateResult(rsb_after_cal,static_cast<word>(999),sweep_param.FBRxRsb_PreCal_Rsb_Limit))
                                    {
                                        ss << "Pre_cal out of limit";
                                        RFCal_Helper_AddMessageToQMSLLog(hPhone,  (ss.str()) ); ss.str("");
                                    }
                                } else {
                                    if (!RFCal_Helper_EvaluateResult(rsb_after_cal,static_cast<word>(999),sweep_param.FBRxRsb_PostCal_Rsb_Limit))
                                    {
                                        ss << "Post_cal out of limit";
                                        RFCal_Helper_AddMessageToQMSLLog(hPhone,  (ss.str()) ); ss.str("");
                                    }
                                }
                                
                            } // if (sweep_param.Do_FBRxRsb_Limit_Check)
                        }
                    }// ( res_dev_mask & (1<<i) )
                } // for (byte i = 0; i < FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_MAX; i++)
                
            } //  FTM_ENH_INTERNAL_DEVICE_CAL_FB_RSB)

            //***********************************************************//
            //       Rx RSB Logging
            //***********************************************************//
            
            if (Sweep[iSegNum].EnhIntDevCal.rsb_instr.rsb_type == FTM_ENH_INTERNAL_DEVICE_CAL_RX_RSB)
            {
                RFCal_Helper_AddMessageToQMSLLog(hPhone, "MEASUREMENTS: RxRSB Cal" );
                stringstream ss;
                for (byte i = 0; i < FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_MAX; i++)
                {
                    if( res_dev_mask & (1<<i) )
                    {
                        byte tech = Sweep[iSegNum].EnhIntDevCal.tech;
                        byte band = static_cast<byte>(Sweep[iSegNum].EnhIntDevCal.band);
                        dword ref_chan = Sweep[iSegNum].EnhIntDevCal.ref_chan;
                        ss << "Technology: " << static_cast<UINT>(tech) << " Band: " <<  static_cast<UINT>(band) << " RefChan: " <<  static_cast<UINT>(ref_chan);
                        RFCal_Helper_AddMessageToQMSLLog(hPhone,  (ss.str()) ); ss.str("");
                        for ( short j = 0; j < Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.num_gain_states; j++)
                        {

                            byte gain_state =  Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.gain_states[j];

                            word rsb_before_cal = static_cast<unsigned short>( Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].rsb_before_cal );
                            word rsb_after_cal = static_cast<unsigned short> (Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].rsb_after_cal);

                            ss          << "Rsb Cal Path: " << static_cast<UINT>(i) <<  " / "
                                        << "Gain States: "<< static_cast<int>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.gain_states[j]) <<  " / "
                                        << "Nv_active: " << static_cast<int>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].nv_active) <<  " / "
                                      << "Sin_theta_before_cal: " << static_cast<short>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].sin_theta_final) <<  " / "
                                      << "Cos_theta_before_cal: " << static_cast<short>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].cos_theta_final) <<  " / "
                                      << "Gain_inv_before_cal: " << static_cast<short>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].gain_inv_final) <<  " / "
                                        << "Rsb_before_cal: " << static_cast<short>(rsb_before_cal) <<  " / ";
                            RFCal_Helper_AddMessageToQMSLLog(hPhone,  (ss.str()) ); ss.str("");

                            if ((rsb_before_cal <50) && ( rsb_after_cal < 99) && ( sweep_param.RxRsb_DbgLevel != 0 ))
                            {
                            ss          << "Sin_theta_after_cal: " << static_cast<short>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].sin_theta_after_cal) <<  " / "
                                        << "Cos_theta_after_cal: "<< static_cast<short>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].cos_theta_after_cal) << " / "
                                        << "gain_inv_after_cal: " << static_cast<short>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].gain_inv_after_cal) <<  " / "
                                        << "FBRx_Rsb_after_cal: " << static_cast<short>(rsb_after_cal) <<  " / ";
                            RFCal_Helper_AddMessageToQMSLLog(hPhone,  (ss.str()) ); ss.str("");

                            }
                            if (sweep_param.Do_RxRsb_Limit_Check)
                            {
                                if (sweep_param.RxRsb_DbgLevel == 0)
                                {
                                    if (!RFCal_Helper_EvaluateResult(rsb_after_cal,static_cast<word>(999),sweep_param.RxRsb_PreCal_Rsb_Limit))
                                    {
                                        ss << "Pre_cal out of limit";
                                        RFCal_Helper_AddMessageToQMSLLog(hPhone,  (ss.str()) ); ss.str("");
                                    }
                                } else {
                                    if (!RFCal_Helper_EvaluateResult(rsb_after_cal,static_cast<word>(999),sweep_param.RxRsb_PostCal_Rsb_Limit))
                                    {
                                        ss << "Post_cal out of limit";
                                        RFCal_Helper_AddMessageToQMSLLog(hPhone,  (ss.str()) ); ss.str("");
                                    }
                                }
                                
                            } // if (sweep_param.Do_RxRsb_Limit_Check)
                         MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;

                         DataSetElementPtr = 
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Technology",demo_param.TECH);
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Band",demo_param.BAND);
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel",ref_chan);
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Bandwidth",Sweep[iSegNum].EnhIntDevCal.bw[0]);
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Waveform Offset_in_kHz",Sweep[iSegNum].EnhIntDevCal.rsb_instr.offset_khz);
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Device_Id",i);
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Gain_State",Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.gain_states[j]);
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"sin_theta_before_cal",Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].sin_theta_final);
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"cos_theta_before_cal",Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].cos_theta_final);
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"gain_inv_before_cal",Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].gain_inv_after_cal);
                     RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Rsb_before_cal",static_cast<int>(Sweep[iSegNum].rx_fbrxRsbRes.rsb_resp.rsb_res[i][j].rsb_before_cal));


                        }
                    }// ( res_dev_mask & (1<<i) )
                } // for (byte i = 0; i < FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_MAX; i++)
            } //  FTM_ENH_INTERNAL_DEVICE_CAL_RX_RSB
        }   
    } // end of For Loop

   return true;
}
unsigned char RFCal_Helper_ESC_ET_Logging_RefCal(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep)
{
   HANDLE hPhone = demo_param.hPhone;

   MSXML2::IXMLDOMDocument2Ptr xmlDoc = demo_param.xmlDoc;
   MSXML2::IXMLDOMElementPtr DataSetCollectionElementPtr = NULL;
 
    if (sweep_param.Do_Enh_Int_Cal)
    {
        if (sweep_param.Enh_Int_Cal_Type_Mask & ENH_INT_DEV_CAL_FBDC)
        {
            //***********************************************************//
            //      FBRx DC Cal Logging
            //***********************************************************//
            DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,demo_param.TestPtr,"DataSetCollection","");
        RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: FBRx DC Cal");


            for (UINT iSegNum = 0; iSegNum < Sweep.size() - 1; iSegNum++)
            {
                if (Sweep[iSegNum].EnhIntDevCal.int_cal_type_mask == ENH_INT_DEV_CAL_FBDC)
                {
                    RFCal_Helper_AddMessageToQMSLLog(hPhone, "MEASUREMENTS: FBRx DC Cal" );
                    stringstream ss;
                    for (byte i = 0; i < sweep_param.FBRx_DC_Gain_State_List.size(); i++)
                    {
                        ss  <<  "FBRx NV Mode: "<<Sweep[iSegNum].fbresDc.dc_resp.fbrx_mode_nv
                            << "/ FBRx Gain State: "<<static_cast<int>(Sweep[iSegNum].fbresDc.dc_resp.fbrx_gain_state[i])
                            << "/  DC_I = "<<Sweep[iSegNum].fbresDc.dc_resp.dcval[i][0]
                            << "/  DC_Q = "<<Sweep[iSegNum].fbresDc.dc_resp.dcval[i][1];
                        RFCal_Helper_AddMessageToQMSLLog(hPhone, ss.str()); ss.str("");
                      MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
                      DataSetElementPtr = RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FBRx_NV_Mode",Sweep[iSegNum].fbresDc.dc_resp.fbrx_mode_nv);
                      RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FBRx_Gain_State",static_cast<int>(Sweep[iSegNum].fbresDc.dc_resp.fbrx_gain_state[i]));
                      RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"DC_I",static_cast<int>(Sweep[iSegNum].fbresDc.dc_resp.dcval[i][0]));
                      RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"DC_Q",static_cast<int>(Sweep[iSegNum].fbresDc.dc_resp.dcval[i][1]));

                  stringstream stType;
                        
                  
                  
                  if (!RFCal_Helper_EvaluateResult(Sweep[iSegNum].fbresDc.dc_resp.dcval[i][0],sweep_param.FBRx_DC_I_Max_Limit[i],sweep_param.FBRx_DC_I_Min_Limit[i]))
                        {
                            ss << "FBRx Gain State: "<<static_cast<int>(Sweep[iSegNum].fbresDc.dc_resp.fbrx_gain_state[i])
                            << "/  DC_I = "<<Sweep[iSegNum].fbresDc.dc_resp.dcval[i][0] << " is out of range. ";
                            RFCal_Helper_AddMessageToQMSLLog(hPhone, ss.str()); ss.str("");
                        }
                        if (!RFCal_Helper_EvaluateResult(Sweep[iSegNum].fbresDc.dc_resp.dcval[i][1],sweep_param.FBRx_DC_Q_Max_Limit[i],sweep_param.FBRx_DC_Q_Min_Limit[i]))
                        {
                            ss << "FBRx Gain State: "<<static_cast<int>(Sweep[iSegNum].fbresDc.dc_resp.fbrx_gain_state[i])
                            << "/  DC_I = "<<Sweep[iSegNum].fbresDc.dc_resp.dcval[i][1] << " is out of range. ";
                            RFCal_Helper_AddMessageToQMSLLog(hPhone, ss.str()); ss.str("");
                        }
                    }               
                }
            
            } // end of For Loop
        } // End of FBRx DC cal Logging
        if (sweep_param.Enh_Int_Cal_Type_Mask & ENH_INT_DEV_CAL_RSB)
        {
            //***********************************************************//
            //      FBRx RSB Cal Logging
            //***********************************************************//
            RFCal_Helper_ESC_ET_Logging_RsbCal(demo_param,cal_param,sweep_param,Sweep);

        } // End of FBRx RSB Cal Logging

   if (!sweep_param.Do_FBGain_Cal && sweep_param.FBRx_Cal_Mode == DO_REF_CAL)
   {
      //***********************************************************//
      //    Ref Cal Tx power
      //***********************************************************//
      RFCal_Helper_AddMessageToQMSLLog(hPhone, "Ref Cal - Tx");
      RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tPA\tRGI\tBAND\tPWR\tFB AGC\tFB Gain");

      for (UINT iSegNum = 0; iSegNum < Sweep.size() - 1; iSegNum++)
      {
         if (Sweep[iSegNum].txPurpose == 3 || Sweep[iSegNum].txPurpose == 255 ) // Tx Pwr
         {
            int channel =   Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan;
            UINT dev = static_cast<UINT>(Sweep[iSegNum].radio_cfg.device.masterDev);
            int pa = Sweep[iSegNum].txParams.txParams[dev].paState;
            int rgi = Sweep[iSegNum].txParams.txParams[dev].rgi;
            int band = demo_param.BAND;
            short shPwr = static_cast<short>( (Sweep[iSegNum].txRes.txPower >= 0)? Sweep[iSegNum].txRes.txPower/10.0+0.5: Sweep[iSegNum].txRes.txPower/10.0-0.5);
            double pwr = static_cast<double>(shPwr*10)/10.0;
            int fbagc = (short)Sweep[iSegNum].rxRes.lnaOffset[1][0];

            int fbgain;
            if (demo_param.TECH == 3)
            {
               fbgain = fbagc +  (short)Sweep[iSegNum].txRes.txPower;
            }
            else
            {
               fbgain = -fbagc +  (short)Sweep[iSegNum].txRes.txPower;
            }
            stringstream tmpStr;
                tmpStr << channel << "\t" << pa << "\t" << rgi << "\t" << band << "\t"  << pwr << "\t" << fbagc << "\t" << fbgain;
            RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());

                MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
                DataSetElementPtr = 
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel",channel);
                RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"PA State",pa);
                RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"TxAGC",rgi);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FBRx Gain State",Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FBRx Mode",Sweep[iSegNum].radio_cfg.chan.chanInfo[0].txBW);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"UBias",Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Rx Scale",Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale);

            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Power",pwr);

         }
      }
   }
   else if (sweep_param.Do_FBGain_Cal)
   {
      //***********************************************************//
        //      FBRx Gain Cal Logging
      //***********************************************************//
      DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,demo_param.TestPtr,"DataSetCollection","");
      RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: FBRx Gain Cal");

      RFCal_Helper_AddMessageToQMSLLog(hPhone, "Ref Cal - Tx");
      RFCal_Helper_AddMessageToQMSLLog(hPhone, "Channel\tPA\tRGI\tBAND\tPWR\tFB Gain\tuBias\tRxScale\tXCorrPeakVal\tXCorrPeakIdx\tLS Error\tMax Mag LSE");

      for (UINT iSegNum = 0; iSegNum < Sweep.size() - 1; iSegNum++)
      {
            if (Sweep[iSegNum].EnhIntDevCal.int_cal_type_mask != ENH_INT_DEV_CAL_FBDC && (Sweep[iSegNum].txPurpose == 3 || Sweep[iSegNum].txPurpose == 255 )) // Tx Pwr
         {
            int channel =   Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan;
            UINT dev = static_cast<UINT>(Sweep[iSegNum].radio_cfg.device.masterDev);
            int pa = Sweep[iSegNum].txParams.txParams[dev].paState;
            int rgi = Sweep[iSegNum].txParams.txParams[dev].rgi;
            int band = demo_param.BAND;
            double orgPwr = Sweep[iSegNum].txRes.txPower;
            short shPwr = static_cast<short>( (orgPwr >= 0)? orgPwr+0.5: orgPwr-0.5);
            double pwr = static_cast<double>(shPwr)/10.0;
         
         //   double pwr = static_cast<double>(static_cast<short>(Sweep[iSegNum].txRes.txPower)) / 10.0;
            int fbgain = (short)Sweep[iSegNum].txRes.fbGainMeas.fbGainRes;
            int uBias = (short)Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias;
            int RxScale = (short)Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale;

            UINT maxValIdx = 0;
            // Just find the index for max value.
            for( UINT logIdx = 0; logIdx < QMSL_FB_GAIN_MEAS_XCORR_LOG_SIZE - 1; logIdx++ )
            {
               if( Sweep[iSegNum].txRes.fbGainMeas.fbXCorrLog[logIdx + 1]  >  Sweep[iSegNum].txRes.fbGainMeas.fbXCorrLog[maxValIdx] )
                  maxValIdx = logIdx + 1;
            }
            int XCorrPeakVal = static_cast<signed int>(Sweep[iSegNum].txRes.fbGainMeas.fbXCorrLog[maxValIdx]);

            // The 32-bit LSE represents a complex number, the real part of which is the 16 MSB and the imaginary part is the 16 LSB.  Both in 2's complement.
            unsigned short reLSE = static_cast<unsigned short>((Sweep[iSegNum].txRes.fbGainMeas.fbLSError >> 16) & 0xFFFF);
            unsigned short imLSE = static_cast<unsigned short>((Sweep[iSegNum].txRes.fbGainMeas.fbLSError) & 0xFFFF);

            signed short reLSEVal = (reLSE > 32767)?(-(65535-reLSE+1)):reLSE;
            signed short imLSEVal = (imLSE > 32767)?(-(65535-imLSE+1)):imLSE;
            unsigned short maxMagLSE = static_cast<unsigned short>(max( abs(reLSEVal), abs(imLSEVal) ));

            stringstream tmpStr;
            tmpStr   << sweep_param.Ref_Channel << "\t" << pa << "\t" << rgi << "\t" << band << "\t"  << pwr  << "\t" << fbgain << "\t"
                     << uBias<< "\t" << RxScale << "\t" << XCorrPeakVal << "\t" << maxValIdx << "\t" << maxMagLSE;
            RFCal_Helper_AddMessageToQMSLLog(hPhone, tmpStr.str());

                MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
                DataSetElementPtr = 
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Channel",channel);
                RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"PA State",pa);
                RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"TxAGC",rgi);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FBRx Gain State",Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"FBRx Mode",Sweep[iSegNum].radio_cfg.chan.chanInfo[0].txBW);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"UBias",Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Rx Scale",Sweep[iSegNum].txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale);

            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Power",pwr);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"FBGain",fbgain);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"XCorr Log Peak Val",XCorrPeakVal);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"XCorr Log Peak Index",maxValIdx);
            RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"Max Mag LSE",maxMagLSE);
        
         
         }
        }
      //***********************************************************//
      //      FBRx Gain Cal Logging - Therm
      //***********************************************************//
      if (sweep_param.FBGain_Cal_Do_Therm_Read)
      {
         DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,demo_param.TestPtr,"DataSetCollection","");
         RFCal_Helper_AddNewTextElement(demo_param.xmlDoc,DataSetCollectionElementPtr,"Name","MEASUREMENTS: FBRx Gain Cal Therm Read");
         for (vector<cal_sweep_record>::const_iterator it = Sweep.begin(); it != Sweep.end(); it++)
         {
            UINT dev = static_cast<UINT>(it->radio_cfg.device.masterDev);
            if (dev > 8) { continue;}

            if ( it->txMeasParams.txMeas[dev].txMeasAction & ESC_TX_MEAS_ACTION_THERM_READ)
            {
               UINT therm = static_cast<UINT>(it->txRes.hdet[dev][2]);
                  MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
                  DataSetElementPtr = 
                  RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_MEAS,"FBRx Gain Cal Therm Read",therm);
                  break;

            }
         }
      }
    }

    }
   return true;
}
unsigned char RFCal_Helper_ESC_ET_Logging(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep)
{
   unsigned char bStatus = true;
   switch (sweep_param.Sweep_Type )
   {
      case XPT_SWP1:
      case MAX_BIAS_LINEARIZER:
      case INT_CAL_XPT_SWP1:
      case INT_CAL_MAX_BIAS_LINEARIZER:
         bStatus = RFCal_Helper_ESC_ET_Logging_XPT_Swp1(demo_param,cal_param,sweep_param,Sweep);
         break;

      case XPT_SWP2:
      case INT_CAL_XPT_SWP2:
         bStatus = RFCal_Helper_ESC_ET_Logging_XPT_Swp2(demo_param,cal_param,sweep_param,Sweep);
         break;

      case XPT_SWP3:
      case APT_LINEARIZER:
      case INT_CAL_XPT_SWP3:
      case INT_CAL_APT_LINEARIZER:
         bStatus = RFCal_Helper_ESC_ET_Logging_XPT_Swp3(demo_param,cal_param,sweep_param,Sweep);
         break;

      case XPT_FREQCOMP:
      case APT_FREQCOMP:
      case XPT_FREQCOMP_STAGGER_RXTX:
      case INTRACA_FCOMP:
      case INT_CAL_XPT_FREQCOMP:
      case INT_CAL_MAX_BIAS_FREQCOMP:
      case INT_CAL_APT_FREQCOMP:
         bStatus = RFCal_Helper_ESC_ET_Logging_XPT_FreqComp(demo_param,cal_param,sweep_param,Sweep);
         break;

      case XPT_DELAY:
         bStatus = RFCal_Helper_ESC_ET_Logging_XPT_Delay(demo_param,cal_param,sweep_param,Sweep);
         break;

      case REF_CAL:
         bStatus = RFCal_Helper_ESC_ET_Logging_RefCal(demo_param,cal_param,sweep_param,Sweep);
         break;

        case RX_RSB_CAL:
            bStatus = RFCal_Helper_ESC_ET_Logging_RsbCal(demo_param,cal_param,sweep_param,Sweep);
            break;
      default:
         break;
   }
   return bStatus;
}
unsigned char RFCal_Helper_RecordIndex_Increment(vector < cal_sweep_record > & Sweep, UINT & recordIndex)
{
   vector < TE_Cal_Swp_Params > TE_Records;
   TE_Records.resize(0);
   return RFCal_Helper_RecordIndex_Increment(Sweep,TE_Records,recordIndex);

}
unsigned char RFCal_Helper_RecordIndex_Increment(vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records, UINT & recordIndex)
{
   const int MAX_RECORD_SIZE = 3000;
    const int fudge = 6;
   UINT currentSize = Sweep.size();
   //***********************************************************//
   //    Check the sweep record vector does not overflow.
   //      If approaching limit increase the size
   //***********************************************************//
   if (recordIndex > currentSize - fudge)
   {
      //***********************************************************//
      //    Increase the size and initialize new segments
      //***********************************************************//
      if (recordIndex > MAX_RECORD_SIZE - fudge)
         throw RFCal_Helper_Exception("The number of segments reached the max value. Please check parameters are correct.",  __FILE__, __LINE__ );

      Sweep.resize(currentSize*2);
      bool bTERecords = (TE_Records.size() > 0)? true:false; 
      if (bTERecords)
      {
         TE_Records.resize(currentSize*2);
      }
      for (UINT i = recordIndex + 2; i < currentSize*2 - 1; i ++)
      {
         if (bTERecords)
         {
            TE_Records[i] = TE_Records[i-1];
         }
         Sweep[i] = Sweep[i-1];
      }
   }
   //***********************************************************//
   //    Increament record Index
   //***********************************************************//
   recordIndex++;

   // We can check record size here for safety purpose.
   return true;
}
unsigned char RFCal_Helper_ESC_ET_createXPT1Swp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
   //***********************************************************//
   //    Create Sweep 1 records
   //***********************************************************//

   unsigned char  bOK            = true;
   UINT        recordIndex    = 0;
   int txParamMask_PwrOverride  = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN |
                                   ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT); // Sweep
   if (sweep_param.Sweep_Type == XPT_SWP1)
   {
      txParamMask_PwrOverride |= ESC_TX_OVERRIDE_DELAY;
   }

   int            Feedback_Device_ID         = 1;  // hard coding

   int            MasterDevState = 3;
   int            SlaveDevState = 0;

   //*****************************************************************************************//
   // OpCode 0x100: Radio Setup
   //*****************************************************************************************//
   RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex],
                                     sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID,
                                     MasterDevState /* Master Device State */,
                                     SlaveDevState  /* Slave Device State */,
                                     demo_param.TECH,
                                     demo_param.BAND,
                                     sweep_param.Ref_Channel /* ulChan */,
                                     sweep_param.Waveform.Waveform_Type,
                                     NULL,
                                     sweep_param.Waveform.Number_Of_RBs,
                                     sweep_param.Waveform.Start_RB);
   Sweep[recordIndex].radio_cfg.flag = sweep_param.Use_PilotOnlyWaveform;

   RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.Config_Segment_Length); // New change from QDART 4821

   //*****************************************************************************************//
    //  OpCode 0x1000: Sync OpCode (LTE Only and ET/EPT only) 
   //*****************************************************************************************//
   if (demo_param.TECH == 3 && sweep_param.Sweep_Type == XPT_SWP1)
   {
      RFCal_Helper_setOpCode_Sync(Sweep[recordIndex]);
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
      Sweep[recordIndex].isNopSeg = true;
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
   }

   //****************************************************************//
   // Max Bias Linearizer (For PA for APTs)
   // Set Max Bias (e.g., 3700) to the bias configuration, and then measure the power.
   //****************************************************************//

   for(UINT paIndex = 0; paIndex < sweep_param.PA_State_List.size() ; paIndex++)
   {
      for (UINT rgiIndex = 0; rgiIndex < sweep_param.RGI_List[paIndex].size(); rgiIndex ++)
      {
         //***************************************************************************************//
         //    OpCode 0x200: TxOverride
         //***************************************************************************************//
         Sweep[recordIndex].measSeg = true;
         RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], sweep_param.Purpose_List[paIndex],
                                           cal_param.TxDeviceFlag, XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                           ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,
                                           txParamMask_PwrOverride /* pwrParamMask */,
                                           sweep_param.PA_State_List[paIndex], sweep_param.RGI_List[paIndex][rgiIndex],
                                           sweep_param.Bias_List[paIndex][0],ESC_TX_OVERRIDE_ACTION_SW_DEF, 0 /* IQ_Gain */,
                                           ESC_TX_OVERRIDE_ACTION_SW_DEF, 0 /* Env_Scale */, sweep_param.PA_Quiescent_Current[paIndex][0],ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);

            TE_Records[recordIndex].m_ul_lvl = sweep_param.Max_Expected_Pwr[paIndex];

            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex); 
            

         //***************************************************************************************//
         //    OpCode 0x8000: TxMeasure (HDET Measure) (Seperate HDET and TxOverride)
         //***************************************************************************************//
         if (sweep_param.HDET_Avgs.size() > 0 && sweep_param.HDET_Avgs[paIndex] > 0)
         {
            Sweep[recordIndex].txParams.txDevice = Sweep[recordIndex-1].txParams.txDevice;
            RFCal_Helper_setOpCode_TxMeasure(Sweep[recordIndex],Sweep[recordIndex].txParams.txDevice, (sweep_param.HDET_Avgs[paIndex]>0)?1:0, (byte)sweep_param.HDET_Avgs[paIndex]);
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         }
      }
      //*****************************************************************************************//
      //    Insert NoP (OpCode 0x0) if MaxExpected power (i.e., DL level will be changed) or pa state change
      //*****************************************************************************************//
        if (paIndex == (sweep_param.PA_State_List.size() - 1) 
            || sweep_param.Max_Expected_Pwr[paIndex] != sweep_param.Max_Expected_Pwr[paIndex+1] 
            || sweep_param.PA_State_List[paIndex] != sweep_param.PA_State_List[paIndex+1])
         RFCal_helper_InsertNop(Sweep,recordIndex, 1);
   }

   if (sweep_param.Sweep_Type == XPT_SWP1)
   {
        RFCal_helper_InsertNop(Sweep,recordIndex, 1);

      //*****************************************************************************************/
      //    XPT Config
      //    OpCode 0x2000: XPT2
      //*****************************************************************************************/
      byte cfg_dev_id = sweep_param.Device_Selection.Master_Device_ID; 
      RFCal_Helper_setOpCode_XPT2(Sweep[recordIndex],  ESC_XPT_ACTION_CAL_CFG /* xptAction */, cfg_dev_id /* cfg_dev */, Feedback_Device_ID, XPT_TYPE_EPT); // Even ET only, use EPT
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

      //*****************************************************************************************/
      //    Load Unity DPD
      //    OpCode 0x800: Data
      //*****************************************************************************************/
      RFCal_Helper_setOpCodeData(Sweep[recordIndex], cal_param, sweep_param, DATA_OPCODE_ACTION_LOAD_DPD /* OpCode Data Option */, demo_param.TECH, demo_param.BAND);
      Sweep[recordIndex].data.nomDPDTbl.xptType    = LOAD_DPD_XPT_TYPE_EPT;
      Sweep[recordIndex].data.nomDPDTbl.numDET     = 0; // because EPT does not use DET
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

      RFCal_Helper_setOpCodeData(Sweep[recordIndex], cal_param, sweep_param, DATA_OPCODE_ACTION_LOAD_DPD /* OpCode Data Option */, demo_param.TECH, demo_param.BAND);
      Sweep[recordIndex].data.nomDPDTbl.xptType    = LOAD_DPD_XPT_TYPE_ET;
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

        //RFCal_Helper_setOpCodeData(Sweep[recordIndex], cal_param, sweep_param, DATA_OPCODE_ACTION_LOAD_DPD /* OpCode Data Option */, demo_param.TECH, demo_param.BAND);
        //Sweep[recordIndex].data.nomDPDTbl.xptType     = LOAD_DPD_XPT_TYPE_EPT;
        //Sweep[recordIndex].data.nomDPDTbl.numDET      = 0; // because EPT does not use DET
        //RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

      //*****************************************************************************************/
      //    Alignment Sweep
      //    IQ Capture for 4 RGI  (Tx Override --> IQ capture)
      //*****************************************************************************************/

      unsigned char bFudgeSegment = true;
      UINT iETindex = (cal_param.En_EPTET_Different_PAstate == 1)? 2 : 1;
      for (UINT epEptLoopIndex = 0; epEptLoopIndex < iETindex; epEptLoopIndex ++)
      {
         byte XPT_Swp1_PA_State;
         vector <byte> XPT_Swp1_RGI_List;
         vector <word> XPT_Swp1_Bias_List;

         if (cal_param.En_EPTET_Different_PAstate == 1)
         {
            if (epEptLoopIndex == 0)
            {
               // EPT case
               XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State_EPT;
               XPT_Swp1_RGI_List = sweep_param.XPT_Swp1_RGI_List_EPT;
               XPT_Swp1_Bias_List = sweep_param.XPT_Swp1_Bias_List_EPT;
            }
            else     // ET case
            {
               XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State_ET;
               XPT_Swp1_RGI_List = sweep_param.XPT_Swp1_RGI_List_ET;
               XPT_Swp1_Bias_List = sweep_param.XPT_Swp1_Bias_List_ET;
            }
         }
         else
         {
            XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State;
            XPT_Swp1_RGI_List = sweep_param.XPT_Swp1_RGI_List;
            XPT_Swp1_Bias_List = sweep_param.XPT_Swp1_Bias_List;
         }

         for(UINT rgiIndex = 0; rgiIndex  < XPT_Swp1_RGI_List.size() ; rgiIndex++)
         {
            //*****************************************************************************************/
            //    Add fudgeSegment Before mode change
            //*****************************************************************************************/
            UINT iNumOfLoops = (bFudgeSegment)?2 :1 ;

            //*****************************************************************************************/
            //    OpCode 0x200: TxOverride
            //*****************************************************************************************/
            for (UINT i = 0; i < iNumOfLoops; i ++)
            {
               Sweep[recordIndex].subSwpType = (bFudgeSegment)? ESC_SUB_SWEEP_MAX:ESC_SUB_SWEEP_XPT_ALIGNMENT;
               byte RGI = (bFudgeSegment)? 0 : XPT_Swp1_RGI_List[rgiIndex]; // RGI = 0 if it is fudge segment

               RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], 0, cal_param.TxDeviceFlag, XPT_TYPE_EPT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                                 ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,        txParamMask_PwrOverride /* pwrParamMask */,
                                                 XPT_Swp1_PA_State, RGI, (dword)XPT_Swp1_Bias_List[rgiIndex],
                                                 ESC_TX_OVERRIDE_ACTION_USE_VAL, sweep_param.XPT_Swp1_IQ_Gain,
                                                 ESC_TX_OVERRIDE_ACTION_USE_VAL, sweep_param.XPT_Swp1_Env_Scale,
                                                 cal_param.PA_Param.IcqEt,ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);
               Sweep[recordIndex].measSeg = (bFudgeSegment)?false:true;

            TE_Records[recordIndex].m_ul_lvl =  (bFudgeSegment)? -20 :sweep_param.XPT_Swp1_Max_Expected_Power;

                    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

               //*****************************************************************************************/
               //    TDSCDMA only. Waveform setting for Alignment Sweep
               //*****************************************************************************************/
               if (bFudgeSegment && demo_param.TECH == 4)
               {
                  //*****************************************************************************************//
                  // OpCode 0x100: Radio Setup
                  //*****************************************************************************************//
                  byte txWfm = 5;
                  MasterDevState = 0;
                  RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, MasterDevState, SlaveDevState, demo_param.TECH, demo_param.BAND,
                                                    sweep_param.Ref_Channel /* ulChan */, txWfm , NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);
                  Sweep[recordIndex].radio_cfg.flag = 0;
                        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

                  //*****************************************************************************************//
                  // Insert NoP (OpCode 0x0)
                  //*****************************************************************************************//
                        RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);
               }
               bFudgeSegment = false;
            }

            //*****************************************************************************************/
            //    OpCode 0x2000: XPT2 IQ capture
            //*****************************************************************************************/

            Sweep[recordIndex].txPurpose = (word)0;
            Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_MAX;
            byte FirstTrig = 0;
            byte LastTrig = 0;
            if ((cal_param.En_EPTET_Different_PAstate == 0 && rgiIndex==0 )
                  || (cal_param.En_EPTET_Different_PAstate == 1 && epEptLoopIndex == 0  && rgiIndex==0 ))
            {
               FirstTrig = 1;
            }
            if ((cal_param.En_EPTET_Different_PAstate == 0 && rgiIndex== (XPT_Swp1_RGI_List.size()-1) )
                  || (cal_param.En_EPTET_Different_PAstate == 1 && epEptLoopIndex == 1  && rgiIndex==  (XPT_Swp1_RGI_List.size()-1) ))
            {
               LastTrig = 1;
            }

            RFCal_Helper_setOpCode_XPT2(Sweep[recordIndex], ESC_XPT_ACTION_IQ_CAP /* xptAction */, cfg_dev_id /* cfg_dev */, Feedback_Device_ID, NULL, 
                                        ESC_XPT_CAP_EPT,sweep_param.XPT_Swp1_Num_IQ_Samples, FirstTrig, LastTrig,  ESC_XPT_CAP_PROC_SWP1 );

            TE_Records[recordIndex].m_ul_lvl =  sweep_param.XPT_Swp1_Max_Expected_Power;

                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

            //*****************************************************************************************//
            // Insert NoP (OpCode 0x0)
            //*****************************************************************************************//
            // Find the number of NOPs required to wait for the IQ Capture
            RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.XPT_Swp1_Cap_Segment_Length, sweep_param.Segment_Length[0][0]);
         }
      }
      //*****************************************************************************************/
      //    RGI Delta Sweep
      //*****************************************************************************************/
      bFudgeSegment = true;
      iETindex = (cal_param.En_EPTET_Different_PAstate == 1)? 2 : 1;
      for (UINT epEptLoopIndex = 0; epEptLoopIndex < iETindex; epEptLoopIndex ++)
      {
         byte XPT_Swp1_PA_State;
         vector <byte> XPT_Swp1_RGI_Delta_RGI_List;
         vector <word> XPT_Swp1_Bias_List;
         if (cal_param.En_EPTET_Different_PAstate == 1)
         {
            if (epEptLoopIndex == 0)
            {
               // EPT case
               XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State_EPT;
               XPT_Swp1_RGI_Delta_RGI_List = sweep_param.XPT_Swp1_RGI_Delta_RGI_List_EPT;
            }
            else     // ET case
            {
               XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State_ET;
               XPT_Swp1_RGI_Delta_RGI_List = sweep_param.XPT_Swp1_RGI_Delta_RGI_List_ET;
            }
         }
         else
         {
            XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State;
            XPT_Swp1_RGI_Delta_RGI_List = sweep_param.XPT_Swp1_RGI_Delta_RGI_List;
         }

         for(UINT rgiIndex = 0; rgiIndex  < XPT_Swp1_RGI_Delta_RGI_List.size()   ;  rgiIndex++)
         {
            //*****************************************************************************************/
            //    Add fudgeSegment Before mode change
            //*****************************************************************************************/
            UINT iNumOfLoops = (bFudgeSegment)?2 :1 ;

            for (UINT i = 0; i < iNumOfLoops; i ++)
            {
               Sweep[recordIndex].subSwpType = (bFudgeSegment)?ESC_SUB_SWEEP_MAX:ESC_SUB_SWEEP_RGI_DELTA;
               Sweep[recordIndex].opcode.segLen = static_cast<word>(sweep_param.XPT_Swp1_RGI_Delta_Segment_Length);

               //*****************************************************************************************/
               //    OpCode 0x200: TxOverride
               //*****************************************************************************************/
               RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],0, cal_param.TxDeviceFlag, XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                                 ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,        txParamMask_PwrOverride /* pwrParamMask */,
                                                 XPT_Swp1_PA_State, (byte)XPT_Swp1_RGI_Delta_RGI_List[rgiIndex],      sweep_param.XPT_Swp1_RGI_Delta_Bias, ESC_TX_OVERRIDE_ACTION_USE_VAL, cal_param.PA_Param.IqGainRgiDeltaSwp,
                                                 ESC_TX_OVERRIDE_ACTION_SW_DEF, sweep_param.XPT_Swp1_Env_Scale, cal_param.PA_Param.IcqRgiDeltaSwp,  ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);
               Sweep[recordIndex].measSeg = (bFudgeSegment)?false:true;

            if (XPT_Swp1_PA_State == sweep_param.XPT_Swp1_PA_State_EPT)
            {
                TE_Records[recordIndex].m_ul_lvl = sweep_param.XPT_Swp1_Max_Expected_Power-4;
            }
            else
            {
                TE_Records[recordIndex].m_ul_lvl = sweep_param.XPT_Swp1_Max_Expected_Power;

            }
                    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
               //*****************************************************************************************/
               //    TDSCDMA only.  Revert to default waveform before RGI Delta Sweep
               //*****************************************************************************************/
               if (bFudgeSegment && demo_param.TECH == 4)
               {
                  //*****************************************************************************************//
                  // OpCode 0x100: Radio Setup
                  //*****************************************************************************************//
                  MasterDevState = 0;
                  RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, MasterDevState, SlaveDevState, demo_param.TECH, demo_param.BAND,
                                                    sweep_param.Ref_Channel /* ulChan */, sweep_param.Waveform.Waveform_Type , NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);
                  Sweep[recordIndex].radio_cfg.flag = 0;
                  //*****************************************************************************************//
                  // Insert NoP (OpCode 0x0)
                  //*****************************************************************************************//
                  //RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);
                        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
               }

               bFudgeSegment = false;
            }
         }
      }
   }

    //****************************************************************//
    //   OpCode 0x8000: TxMeas: THERM READ (Except XPT_SWP1)
    //****************************************************************//
    if (sweep_param.Sweep_Type != XPT_SWP1)
    {
        Sweep[recordIndex].txPurpose    = (word)0;
        Sweep[recordIndex].subSwpType   = ESC_SUB_SWEEP_THERM_READ;
    
        RFCal_Helper_setOpCode_TxMeasure(Sweep[recordIndex],
            cal_param.TxDeviceFlag,
            ESC_TX_MEAS_ACTION_THERM_READ);
    
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
    }

   //*****************************************************************************************/
   //    End of Sweep 1: Insert a low rgi segment instead of tear down
   //*****************************************************************************************/
   RFCal_Helper_setLowRGISegment(Sweep[recordIndex],cal_param.TxDeviceFlag);
    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

   //*********************************************//
   //    Generate OpCode for all the segment
   //*********************************************//
   Sweep.resize(recordIndex); // Delete unused records
   RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());

   return true;
}


unsigned char RFCal_Helper_Calculate_FB_LSError_Result( dword fbLSError, FB_LS_Error_Result & _fbLSErrorStruct)
{
   _fbLSErrorStruct.reLSE = static_cast<unsigned short>((fbLSError >> 16) & 0xFFFF);
   _fbLSErrorStruct.imLSE = static_cast<unsigned short>(fbLSError & 0xFFFF);
   _fbLSErrorStruct.reLSEVal = (_fbLSErrorStruct.reLSE > 32767)?(-(65535-_fbLSErrorStruct.reLSE+1)):_fbLSErrorStruct.reLSE;
   _fbLSErrorStruct.imLSEVal = (_fbLSErrorStruct.imLSE > 32767)?(-(65535-_fbLSErrorStruct.imLSE+1)):_fbLSErrorStruct.imLSE;
   _fbLSErrorStruct.maxMagLSE = static_cast<unsigned short>(max( abs(_fbLSErrorStruct.reLSEVal), abs(_fbLSErrorStruct.imLSEVal) ));
   return true;
}

unsigned short RFCal_Helper_Calculate_FB_LSError_Result( dword fbLSError)
{
   FB_LS_Error_Result _fbLSErrorStruct;
   _fbLSErrorStruct.reLSE = static_cast<unsigned short>((fbLSError >> 16) & 0xFFFF);
   _fbLSErrorStruct.imLSE = static_cast<unsigned short>(fbLSError & 0xFFFF);
   _fbLSErrorStruct.reLSEVal = (_fbLSErrorStruct.reLSE > 32767)?(-(65535-_fbLSErrorStruct.reLSE+1)):_fbLSErrorStruct.reLSE;
   _fbLSErrorStruct.imLSEVal = (_fbLSErrorStruct.imLSE > 32767)?(-(65535-_fbLSErrorStruct.imLSE+1)):_fbLSErrorStruct.imLSE;
   _fbLSErrorStruct.maxMagLSE = static_cast<unsigned short>(max( abs(_fbLSErrorStruct.reLSEVal), abs(_fbLSErrorStruct.imLSEVal) ));
   return _fbLSErrorStruct.maxMagLSE;
}


unsigned char RFCal_Helper_PopulateXptModefBGainStateChanvsGainDeltaforIntCalFcompSweep(vector < cal_sweep_record > & intCalFreqCompSweep, 
   byte TECH, word band, int refChannel,  
 //  chanFbGainState_pwrVsFbGain_type refCalchanFbGainState_pwrVsFbGain,
      map < pair < dword, byte >, Tx_RefCal_Data_V2_Record > ChanFBGainStateToPwrFBGainParams,
   map<int, map<unsigned char, map<int, double > > >& xptModeFBGainStateChanvsGainDelta )
{
   vector<cal_sweep_record>::  iterator meas_iter;

   map < pair < dword, byte >, Tx_RefCal_Data_V2_Record > :: iterator refCalchanFbGainState_pwrvsFbGainIter;
   map<int, map<unsigned char, double > > xptModeRefChan_FBGainStatevsGainDelta;

   map<int, map<double,double>> fbGainStateUlFreqvsRefCalPower;
   map<int, map<double,double>> fbGainStateUlFreqvsRefCalFBGain;
   double ulFreq;

   for(refCalchanFbGainState_pwrvsFbGainIter = ChanFBGainStateToPwrFBGainParams.begin(); refCalchanFbGainState_pwrvsFbGainIter != ChanFBGainStateToPwrFBGainParams.end(); 
      ++refCalchanFbGainState_pwrvsFbGainIter)
   {
      double dlFreq = 0.0;
      RFCal_Helper_CalculateFrequencyFromULChan(static_cast<int>(TECH),static_cast<int>(band),refChannel,ulFreq,dlFreq);
      fbGainStateUlFreqvsRefCalFBGain[refCalchanFbGainState_pwrvsFbGainIter->first.second][ulFreq] = 20*log10(static_cast<double>(refCalchanFbGainState_pwrvsFbGainIter->second.FBGain));
      fbGainStateUlFreqvsRefCalPower[refCalchanFbGainState_pwrvsFbGainIter->first.second][ulFreq] = refCalchanFbGainState_pwrvsFbGainIter->second.pwr;
   }

   for( meas_iter = intCalFreqCompSweep.begin();meas_iter != intCalFreqCompSweep.end(); ++ meas_iter)
   {
      if( meas_iter->opcode.varLenOpcode.opCodeGrp.opCode == 0x8200)
      {
         int xptMode = (meas_iter->subSwpType == ESC_SUB_SWEEP_APPLY_DPD)?1:0;
         unsigned char fbGainState = static_cast<unsigned char>(meas_iter->txRes.fbGainMeas.fbGainState);
         int currChan = static_cast<int> (meas_iter->radio_cfg.chan.chanInfo[0].ulChan);
         double refCalTxPower = 0.0, refCalFBGain = 0.0;
         if(ChanFBGainStateToPwrFBGainParams.find(make_pair(currChan,fbGainState))!=ChanFBGainStateToPwrFBGainParams.end())
         {
            refCalTxPower = ChanFBGainStateToPwrFBGainParams[make_pair(currChan,fbGainState)].pwr;
            refCalFBGain = 20*log10(static_cast<double>(ChanFBGainStateToPwrFBGainParams[make_pair(currChan,fbGainState)].FBGain));
         }
         else
         {
            double ULFrequency = 0.0, DLFrequency = 0.0;
            RFCal_Helper_CalculateFrequencyFromULChan(TECH,band,currChan,ULFrequency,DLFrequency);
            Find_Interpolated_Y(fbGainStateUlFreqvsRefCalPower[static_cast<int>(fbGainState)],ULFrequency,refCalTxPower);
            Find_Interpolated_Y(fbGainStateUlFreqvsRefCalFBGain[static_cast<int>(fbGainState)],ULFrequency,refCalFBGain);
         }

         if( xptModeFBGainStateChanvsGainDelta[xptMode][fbGainState].find(currChan) == xptModeFBGainStateChanvsGainDelta[xptMode][fbGainState].end() )
         {
            xptModeFBGainStateChanvsGainDelta[xptMode][fbGainState][currChan] = 20*log10(static_cast<double>(meas_iter->txRes.fbGainMeas.fbGainRes))
               + refCalTxPower - refCalFBGain;
            if(currChan == refChannel)
            {
               xptModeRefChan_FBGainStatevsGainDelta[xptMode][fbGainState] = 20*log10(static_cast<double>(meas_iter->txRes.fbGainMeas.fbGainRes)) - refCalFBGain + refCalTxPower ;
            }
         }
      }
   }

   map<int, map<unsigned char, map<int, double > > >  ::iterator xptModeFBGainStateChanvsGainDeltaItr;

   for( xptModeFBGainStateChanvsGainDeltaItr = xptModeFBGainStateChanvsGainDelta.begin();
      xptModeFBGainStateChanvsGainDeltaItr != xptModeFBGainStateChanvsGainDelta.end(); ++ xptModeFBGainStateChanvsGainDeltaItr)
   {
      map<unsigned char, map<int,double> > fBGainStateChanvsGainDelta;
      map<unsigned char, map<int,double> > :: iterator fBGainStateChanvsGainDeltaItr;

      fBGainStateChanvsGainDelta = xptModeFBGainStateChanvsGainDeltaItr->second;

      for( fBGainStateChanvsGainDeltaItr = fBGainStateChanvsGainDelta.begin();
         fBGainStateChanvsGainDeltaItr != fBGainStateChanvsGainDelta.end(); ++fBGainStateChanvsGainDeltaItr)
      {
         map<int,double> chanvsGainDelta;
         map<int,double> ::iterator chanvsGainDeltaItr;

         chanvsGainDelta = fBGainStateChanvsGainDeltaItr->second;

         for (chanvsGainDeltaItr = chanvsGainDelta.begin(); chanvsGainDeltaItr!= chanvsGainDelta.end();++chanvsGainDeltaItr)
         {
            xptModeFBGainStateChanvsGainDelta[xptModeFBGainStateChanvsGainDeltaItr->first]
            [fBGainStateChanvsGainDeltaItr->first][chanvsGainDeltaItr->first] -=
               xptModeRefChan_FBGainStatevsGainDelta[xptModeFBGainStateChanvsGainDeltaItr->first][fBGainStateChanvsGainDeltaItr->first];
         }
      }
   }
   return true;
}
unsigned char RFCal_Helper_FindCompSwpIdxforFBGainUKernelPowerCompute(map<unsigned short, unsigned short> &swpTypeSwpIdxMap, unsigned short currSwpType, bool is_XPT_Measurement,
   unsigned short &compSwpIdx, unsigned char &useCharRGIvsPwrXML)
{
   if( currSwpType == INT_CAL_MAX_BIAS_LINEARIZER || currSwpType == INT_CAL_XPT_SWP1)
   {
      useCharRGIvsPwrXML = 1;
   }

   else
   {
      useCharRGIvsPwrXML =0;

      switch (currSwpType)
      {
      case INT_CAL_XPT_SWP3:
         if( is_XPT_Measurement)
         {
            compSwpIdx = 1;//swpTypeSwpIdxMap[INT_CAL_XPT_SWP2];
         }
         else
         {
            compSwpIdx = 0;//swpTypeSwpIdxMap[INT_CAL_XPT_SWP1];
         }
         break;

      case INT_CAL_XPT_FREQCOMP:
         if( is_XPT_Measurement)
         {
            compSwpIdx = 1; //swpTypeSwpIdxMap[INT_CAL_XPT_SWP2];
         }
         else
         {
            compSwpIdx = 2; //swpTypeSwpIdxMap[INT_CAL_XPT_SWP3];
         }
         break;

      case INT_CAL_APT_LINEARIZER:
         compSwpIdx = swpTypeSwpIdxMap[INT_CAL_MAX_BIAS_LINEARIZER];
         break;

      case INT_CAL_APT_FREQCOMP:
         compSwpIdx = swpTypeSwpIdxMap[INT_CAL_APT_LINEARIZER];
         break;

      case INT_CAL_MAX_BIAS_FREQCOMP:
         compSwpIdx = swpTypeSwpIdxMap[INT_CAL_MAX_BIAS_LINEARIZER];
         break;

      default:
         useCharRGIvsPwrXML =0;
         break;
      }
   }
   return true;
}

unsigned char RFCal_Helper_ESC_ET_PostComputation_FBGainPowerComputation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, 
   Cal_Param & cal_param, const Sweep_Params & sweep_param,vector < cal_sweep_record >   &   Sweep)
{
   byte rf_mode = cal_param.RF_Mode;

   byte device = sweep_param.Device_Selection.Master_Device_ID;

   byte instance = 0;

   double refPower = 0.0;
   
   double gainDelta;
   signed short fbGain;
   int currUbias,currRxScale,fbgainstate,currRGI;
   dword iChannel;
   int prevRGI = 255;
   int prevfbgainstate = sweep_param.FBGain_Cal_Gain_State_List.front();
   byte devId = sweep_param.Device_Selection.Master_Device_ID;
   map<int,double> fbgainstate_fudgingfactor_map;

   for( unsigned char fbgsitr=0; fbgsitr < sweep_param.FBGain_Cal_Gain_State_List.size(); fbgsitr++)
   {
      fbgainstate_fudgingfactor_map[sweep_param.FBGain_Cal_Gain_State_List[fbgsitr]] = 0.0;
   }
   signed short prevfbGain = 68;
   int selectedfbGainState = sweep_param.FBGain_Cal_Gain_State_List.front();
   int prevselectedfbGainState = sweep_param.FBGain_Cal_Gain_State_List.front();
   byte pa;
   byte prevPa = 255;
   bool lower_power_rgi_threshold_found = false;
   double lower_power_rgi_threshold_offset =0.0;

   double currIntCalPowerEstimate = 0.0;
   FB_LS_Error_Result currIntCalLSE;
   FB_LS_Error_Result prevIntCalLSE = {0,0,0,0,0};

   map<int, map<int,double>> chanPA_rgiPwr;
   map<unsigned short, unsigned short> swpType_swpIdx_map;
   map<int,double>::const_iterator itCharRGIPwr;

   map <RFCal_Helper_Tx_Params, double > APTV6CharToPwr;
   map < pair < dword, byte >, Tx_RefCal_Data_V2_Record > ChanFBGainStateToPwrFBGainParams;
   map< chanPAState_type, rgipwr_type> refcalchanPa_rgi_pwr;
   const int INVALID_FBGAIN_STATE = 255;
   if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
   {
      RFCal_Helper_ESC_ET_Tx_AptChar_DataV6_Assignment_XML(demo_param,  cal_param, sweep_param, APTV6CharToPwr); // Key: Channel. pa_state, rgi
      RFCal_Helper_ESC_ET_FBGain_Ref_Assignment_XML(demo_param,  cal_param, sweep_param, ChanFBGainStateToPwrFBGainParams);
   }

   map < double, double > charRGItoPower;
   //RFCal_Helper_GetEstimatedY_LeastSquaresMethod_quadratic

   map<int, map<unsigned char, map<int, double > > > xptModeFBGainStateChanvsGainDelta;

   if (RFCal_Helper_Is_FreqComp_SweepType(sweep_param.Sweep_Type))
   {
   RFCal_Helper_PopulateXptModefBGainStateChanvsGainDeltaforIntCalFcompSweep(Sweep,demo_param.TECH,demo_param.BAND,
      static_cast<int>(sweep_param.Ref_Channel),ChanFBGainStateToPwrFBGainParams,xptModeFBGainStateChanvsGainDelta);
   }
   int iLowest_RGI_FBRxMeasured = -1;
   const double LowerPowerThreshold = -400.0; // -45.0 dbm
   double PrevPower = 0.0;
   
   if (RFCal_Helper_Is_FreqComp_SweepType(sweep_param.Sweep_Type))
   {
      for (vector <byte>::const_iterator git = sweep_param.FBGain_Cal_Gain_State_List.begin(); 
         git !=  sweep_param.FBGain_Cal_Gain_State_List.end(); git ++)
      {
         map < double, double > FreqToPower;
         map < double, double > FreqToFBGain;
         byte uBias = 0;
         word RxScale = 0;
         for (map < pair < dword, byte >, Tx_RefCal_Data_V2_Record >::iterator mit = ChanFBGainStateToPwrFBGainParams.begin();
            mit != ChanFBGainStateToPwrFBGainParams.end(); mit++)
         {
            byte FBGainState = mit->first.second;
            if ( FBGainState == *git)
            {
               dword channel = mit->first.first;
               Tx_RefCal_Data_V2_Record txRefCal = mit->second;
               double ULFrequency = 0.0, DLFrequency = 0.0;
               RFCal_Helper_CalculateFrequencyFromULChan(demo_param.TECH, demo_param.BAND,channel,ULFrequency,DLFrequency);
               FreqToPower[ULFrequency] = txRefCal.pwr;
               FreqToFBGain[ULFrequency] = 20*log10(static_cast<double>(txRefCal.FBGain));
               
               uBias = txRefCal.uBias;
               RxScale = txRefCal.rxScale;

            }
         }

         /* This interpolation is required if Ref Cal channels and Tx Internal Cal Freq Comp Channels are mismatched */
         /* E.g., Ref Cal. Ch18050, 18300, 18500, Tx Int Cal FreqComp Ch 18100, 18200, 18300.18400... */
         for (vector <dword>::const_iterator it = sweep_param.Tx_Channel_List.begin(); it != sweep_param.Tx_Channel_List.end(); it ++)
         {
            dword channel = *it;
            if ( ChanFBGainStateToPwrFBGainParams.find(make_pair(channel, *git)) == ChanFBGainStateToPwrFBGainParams.end())
            {
               Tx_RefCal_Data_V2_Record txRefCal;
               txRefCal.rxScale = RxScale;
               txRefCal.uBias = uBias;
               double ULFrequency = 0.0, DLFrequency = 0.0;
               RFCal_Helper_CalculateFrequencyFromULChan(demo_param.TECH, demo_param.BAND,channel,ULFrequency,DLFrequency);

               double interpolatedPwr = 0.0;
               double interpolatedFBGain = 0.0;
               Find_Interpolated_Y(FreqToPower,ULFrequency,interpolatedPwr);
               Find_Interpolated_Y(FreqToFBGain,ULFrequency,interpolatedFBGain);
               txRefCal.pwr = interpolatedPwr;
               txRefCal.FBGain = static_cast<word>(pow(10, interpolatedFBGain/20));
               ChanFBGainStateToPwrFBGainParams[make_pair(channel, *git)] = txRefCal;
            }

         }
      }
   } // ENd of Is FreqCOmp Sweep
   int segNum = 0;
   for( vector < cal_sweep_record >::iterator it = Sweep.begin(); it != Sweep.end(); it++)
   {
      segNum++;
      if(!(it->measSeg) 
         || !(it->opcode.varLenOpcode.opCodeGrp.opCode & 0x200)
         || !(it->opcode.varLenOpcode.opCodeGrp.opCode & 0x8000))
      {
         continue;
      }
      
         bool status = false;
         unsigned char useCharRGIvsPwrXML = 0;
         unsigned short compFromSweep = 0;
         bool is_XPT_Measurement= (it->subSwpType == ESC_SUB_SWEEP_APPLY_DPD)?true:false;
         pa = it->txIntCalParams.paState;
         fbgainstate = static_cast<int>(it->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.fbGainState);
      if (pa != prevPa) {  prevselectedfbGainState = INVALID_FBGAIN_STATE; }
         currRGI = it->txParams.txParams[devId].rgi;
         
         iChannel = it->radio_cfg.chan.chanInfo[0].ulChan;
         
         fbGain = static_cast<signed short>(it->txRes.fbGainMeas.fbGainRes);

         currUbias = static_cast<int>(it->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.uBias);

         currRxScale = static_cast<int>(it->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.rxScale);
         
      bool IsXptMode = (it->subSwpType == ESC_SUB_SWEEP_APPLY_DPD || it->txParams.txParams[devId].xptMode)? true:false;

         // **************************************************************** //
         // FreqComp
         // **************************************************************** //
         if (RFCal_Helper_Is_FreqComp_SweepType(sweep_param.Sweep_Type))
         {
            Tx_RefCal_Data_V2_Record curFBGainParam;
            curFBGainParam.FBGain = it->txRes.fbGainMeas.fbGainRes;
            curFBGainParam.rxScale = it->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.rxScale;
            curFBGainParam.uBias = it->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.uBias;
               
            Tx_RefCal_Data_V2_Record preFBGainParam = ChanFBGainStateToPwrFBGainParams[make_pair(iChannel,fbgainstate)];
              
            double FBGainRatio = static_cast<double>(curFBGainParam.FBGain)/static_cast<double>(preFBGainParam.FBGain);
            double RxScaleRatio = static_cast<double>(preFBGainParam.rxScale)/static_cast<double>(curFBGainParam.rxScale);
            double powerOfUBias = pow(2.0,static_cast<double>(preFBGainParam.uBias - curFBGainParam.uBias));
            double aPower = (20 * log10(static_cast<double>(FBGainRatio*RxScaleRatio*powerOfUBias))+preFBGainParam.pwr);
            it->txRes.txPower = aPower*10.0 ;
         }
         else 
         {
            // **************************************************************** //
            //  Non freqComp case
            // **************************************************************** //
         
            Tx_RefCal_Data_V2_Record refFBGainData = ChanFBGainStateToPwrFBGainParams[make_pair(iChannel,fbgainstate)];
            refPower = refFBGainData.pwr;
          
            gainDelta = static_cast<double>((pow(2.0,(refFBGainData.uBias-currUbias))*refFBGainData.rxScale*(static_cast<double>(fbGain)))/(currRxScale*refFBGainData.FBGain));

              
                  it->txRes.txPower = static_cast<signed short>((20*log10(gainDelta) + refPower)*10);
      }
      if (IsXptMode) // if XptMode, there is no overlap since XPT mode always uses highest fbgain state.
      {
         continue;
      }
      
         /* pointer for segment X+1 */
         vector < cal_sweep_record >::iterator first_it = (it != Sweep.end())? (it+1) : Sweep.end();
         int firstSegGainState = (first_it != Sweep.end())? first_it->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.fbGainState : INVALID_FBGAIN_STATE;
         vector < cal_sweep_record >::iterator second_it =(first_it != Sweep.end())? (first_it+1) : Sweep.end();
         int secondSegGainState = (second_it != Sweep.end())? second_it->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.fbGainState : INVALID_FBGAIN_STATE;
            
         if (first_it != Sweep.end() && (!first_it->measSeg))    first_it =  Sweep.end();
         if (second_it != Sweep.end() && (!second_it->measSeg))   second_it =  Sweep.end();

         // PA and RGI check for next next segment
         if ( second_it != Sweep.end())
         {
            if (  pa != second_it->txIntCalParams.paState
               || currRGI != static_cast<int>(second_it->txParams.txParams[devId].rgi)
               || second_it->subSwpType == ESC_SUB_SWEEP_APPLY_DPD)
            {
               second_it = Sweep.end();
            }
            // Same PA and RGI --> Overlap case, check FBGain State
            if ( second_it != Sweep.end() && prevselectedfbGainState < secondSegGainState)
            {
               second_it->measSeg = false;
               second_it->txPurpose = 0xFFFF;
               second_it = Sweep.end();
            }
         }

         // PA and RGI check for next segment
         if ( first_it != Sweep.end())
         {
            if (  pa != first_it->txIntCalParams.paState
               || currRGI != static_cast<int>(first_it->txParams.txParams[devId].rgi)
               || first_it->subSwpType == ESC_SUB_SWEEP_APPLY_DPD)
            {
               first_it = Sweep.end();
            }
            // Same PA and RGI --> Overlap case, check FBGain State
            if (first_it != Sweep.end() && prevselectedfbGainState < firstSegGainState)
            {
               first_it->measSeg = false;
               first_it->txPurpose = 0xFFFF;
               first_it = Sweep.end();
            }
         }

         if ( first_it == Sweep.end() && second_it != Sweep.end()) // Probably no case
         {
            first_it = second_it;
            second_it = Sweep.end();
         }
         if ( first_it != Sweep.end())
                  {
                     RFCal_Helper_Calculate_FB_LSError_Result(it->txRes.fbGainMeas.fbLSError , currIntCalLSE);

            // Case: Three segments are overlapped (Same PA and Same RGI)
            if (first_it != Sweep.end() && second_it != Sweep.end())
            {
               FB_LS_Error_Result firstIntCalLSE;
               FB_LS_Error_Result secondIntCalLSE;
              
               RFCal_Helper_Calculate_FB_LSError_Result(first_it->txRes.fbGainMeas.fbLSError , firstIntCalLSE);
               RFCal_Helper_Calculate_FB_LSError_Result(second_it->txRes.fbGainMeas.fbLSError , secondIntCalLSE);
               
                     it->txRes.hdet[0][0] =  static_cast<int>(it->txRes.txPower);// for debug
               first_it->txRes.hdet[0][0] = static_cast<int>(first_it->txRes.txPower); // for debug
               second_it->txRes.hdet[0][0] = static_cast<int>(second_it->txRes.txPower); // for debug

               // Check maxMagLSE between first and second
               if (firstIntCalLSE.maxMagLSE == secondIntCalLSE.maxMagLSE)
               {
                  if (firstSegGainState < secondSegGainState)
                  { 
                     first_it->measSeg = false;
                     first_it->txPurpose = 0xFFFF;
                     first_it = second_it;    // Assign second segment to first_it pointer
                     second_it = Sweep.end();
                     firstSegGainState = secondSegGainState;
                  }
                  else
                  {
                     second_it->measSeg = false;
                     second_it->txPurpose = 0xFFFF;
                     second_it = Sweep.end();
                  }
               }
               else if (firstIntCalLSE.maxMagLSE < secondIntCalLSE.maxMagLSE)
               {
                     second_it->measSeg = false;
                     second_it->txPurpose = 0xFFFF;
                     second_it = Sweep.end();
               }
               else //  (firstIntCalLSE.maxMagLSE > secondIntCalLSE.maxMagLSE)
               {
                     first_it->measSeg = false;
                     first_it->txPurpose = 0xFFFF;
                     first_it = second_it;    // Assign second segment to first_it pointer
                     second_it = Sweep.end();
                     firstSegGainState = secondSegGainState;
               }
            } // End of three segments overlapped 
            
            if ( first_it != Sweep.end() )
            {
               FB_LS_Error_Result firstIntCalLSE;
              
               RFCal_Helper_Calculate_FB_LSError_Result(first_it->txRes.fbGainMeas.fbLSError , firstIntCalLSE);

               if (firstIntCalLSE.maxMagLSE == currIntCalLSE.maxMagLSE)
               {
                  if (  fbgainstate < firstSegGainState)
                  {
                     it->measSeg = false;
                     it->txPurpose = 0xFFFF;
                     prevselectedfbGainState = firstSegGainState;
                  }
                  else
                  {
                     first_it->measSeg = false;
                     first_it->txPurpose = 0xFFFF;

                     prevselectedfbGainState = fbgainstate;
                  }
               }
               else if (firstIntCalLSE.maxMagLSE < currIntCalLSE.maxMagLSE)
               {
                     it->measSeg = false;
                     it->txPurpose = 0xFFFF;
                     prevselectedfbGainState = firstSegGainState;
               }
               else
               {
                     first_it->measSeg = false;
                     first_it->txPurpose = 0xFFFF;
                     prevselectedfbGainState = fbgainstate;
               }
            }  
            else // No over lap case
                  {
                     if ((it+1)->txParams.txParams[devId].rgi != currRGI)
                     {
                        prevselectedfbGainState = fbgainstate;
                     }
                  }
               
        
   
            if (currRGI > prevRGI) // paState Changeed
            {
               iLowest_RGI_FBRxMeasured = -1;
            }
            PrevPower = it->txRes.txPower;
            prevPa = pa;
            prevRGI = currRGI;
            prevfbgainstate = fbgainstate;
            prevfbGain = fbGain;
   }

   }


   return true;
}
unsigned char RFCal_Helper_ESC_ET_PostComputation_Swp2ForPinCal(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, const Sweep_Params & sweep_param,vector < cal_sweep_record >    &   Sweep, byte ComputeForSweep)
{
    //****************************************************************/
    // Post Computation case 5: // XPT_SWP2_SAVE_BIAS_RGI_FOR_PIN_CAL_SWP_TYPE
    //****************************************************************/
    typedef pair<byte, word> rgiBias;
    vector <rgiBias> etRgiBias;
    vector <rgiBias> eptRgiBias;

    //****************************************************************/
    // Collect RGI and Bias data from dpd result 
    //****************************************************************/

    for (UINT i=0; i < Sweep.size(); i++)
    {
        cal_sweep_record *ps = &Sweep[i];
        if (ps->data.dpdCalc.do_dpd_calc_op)
        {
            for (byte measIdx = 0; measIdx < ps->data.dpdCalc.numSwp2MeasPwrs; measIdx++)
            {
                rgiBias tmpRgiBias;
                // RGI result
                tmpRgiBias.first = static_cast<byte>(ps->dataRes.measDpdRes[measIdx].rgi);
                // Bias result
                tmpRgiBias.second = ps->dataRes.measDpdRes[measIdx].smpsBias;
                if (ps->dataRes.measDpdRes[measIdx].xptMode == 1 /*EPT*/)
                {
                    eptRgiBias.push_back(tmpRgiBias);
                }
                else if (ps->dataRes.measDpdRes[measIdx].xptMode == 2 /*ET*/)
                {
                    etRgiBias.push_back(tmpRgiBias);
                }

            }
        }
    }
    //****************************************************************/
    // Update PIN Cal parameters
    //****************************************************************/
    cal_param.Swp[ComputeForSweep].XPT_PinCal_Swp_XPT_RGI_List.resize(0);
    cal_param.Swp[ComputeForSweep].XPT_PinCal_Swp_XPT_Bias_List.resize(0);
    for (UINT i=0; i < cal_param.Swp[ComputeForSweep].XPT_PinCal_Swp_XPT_Action_List.size(); i++)
    {
        byte rgi = 0;
        word bias = 0;
        if (cal_param.Swp[ComputeForSweep].XPT_PinCal_Swp_XPT_Action_List[i] == XPT_TYPE_ET && etRgiBias.size() > 2)
        { 
            rgi = etRgiBias[(etRgiBias.size() - 2)].first;
            bias = etRgiBias[(etRgiBias.size() - 2)].second;
        }

        if (cal_param.Swp[ComputeForSweep].XPT_PinCal_Swp_XPT_Action_List[i] == XPT_TYPE_EPT && eptRgiBias.size() > 2)
        {
            rgi = eptRgiBias[(eptRgiBias.size() - 2)].first;
            bias = eptRgiBias[(eptRgiBias.size() - 2)].second;
        }
        cal_param.Swp[ComputeForSweep].XPT_PinCal_Swp_XPT_RGI_List.push_back(rgi);
        cal_param.Swp[ComputeForSweep].XPT_PinCal_Swp_XPT_Bias_List.push_back(bias);
    }
    return true;
}
unsigned char RFCal_Helper_ESC_ET_PostComputation_AddCharacterizedRxData(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, const Sweep_Params & sweep_param,vector < cal_sweep_record >   &   Sweep)
{
   //****************************************************************//
   //   Assign parameters for NV generation for each of bands  
   //****************************************************************//
    map <RFCal_Helper_Rx_Params, short>  RxParamsToOffset;

   RFCal_Helper_ESC_ET_Rx_Char_Data_Assignment_XML(demo_param,  cal_param,  sweep_param,  RxParamsToOffset);

   UINT iNumOrgSegments = Sweep.size();


   for (UINT i=0; i < iNumOrgSegments; i++)
   {
      if (!(Sweep[i].opcode.varLenOpcode.opCodeGrp.opCodeGrp0 & 0x4020)) { continue; }

      cal_sweep_record tmp = Sweep[i];
      word band = tmp.radio_cfg.band;
      vector <word>::const_iterator it;
      it = find(sweep_param.Band_List.begin(),sweep_param.Band_List.end(),band);
      if ((it = find(sweep_param.Band_List.begin(),sweep_param.Band_List.end(),band)) != sweep_param.Band_List.end())
      {
         size_t iBandIndex = std::distance(sweep_param.Band_List.begin(),it);
         
         dword refChannel = tmp.radio_cfg.chan.chanInfo[0].ulChan;
         byte rxFlag = tmp.rxParams.rxFlag;
         byte rxPM = tmp.rxParams.rxPM;
         unsigned short rxCalType = tmp.rxParams.rxCalibrationType;

         byte rxMeasAction = 0;
         if (rxCalType == RFNV_RX_CAL_DATA_NV_TYPE_WCDMA_DC) { rxMeasAction = 1;}
         if (rxCalType == RFNV_RX_CAL_DATA_NV_TYPE_WCDMA_3C) { rxMeasAction = 2;}

         int ChannelOffset = static_cast<int>(sweep_param.RefChannel_List[iBandIndex]) -  static_cast<int>(sweep_param.Ref_Channel);
         for (UINT iChanIndex =0 ; iChanIndex <sweep_param.Rx_Channel_List.size(); iChanIndex++)
         {
            dword iChannel = sweep_param.Rx_Channel_List[iChanIndex];
            if (iChannel == sweep_param.Ref_Channel) { continue; }

            RFCal_Helper_Rx_Params RxParam;

            for (byte devIndex = 0; devIndex < 6; devIndex++)
            {
               if (rxFlag & (1 << devIndex))
               {
                  byte lnaState = tmp.rxMeas.actions[devIndex].payloads[rxMeasAction].lna_state;

                  short MeasuredOffset = static_cast<short>(tmp.rxRes.lnaOffset[devIndex][rxMeasAction]);
                  
                  
                  RxParam.rf_mode = sweep_param.RFMode_List[iBandIndex];
                  RxParam.instance = 0;
                  RxParam.rx_cal_type = rxMeasAction; // Assign RxMeasAction intentionall for now.
                  RxParam.Channel = refChannel;
                  RxParam.RxPM = rxPM;
                  RxParam.RxPath = devIndex; // = Chain
                  RxParam.LNARange = lnaState;

                  short RefChannelOffsetInChar = RxParamsToOffset[RxParam];

                  short offset = RefChannelOffsetInChar - MeasuredOffset;
                              
                  RxParam.Channel = iChannel;
                  short  CharLnaOffset = RxParamsToOffset[RxParam];
                  short updatedOffset = CharLnaOffset + offset;
                  tmp.rxRes.lnaOffset[devIndex][rxMeasAction] = updatedOffset;
                  tmp.radio_cfg.chan.chanInfo[0].ulChan = iChannel;
               }

            }
            Sweep.push_back(tmp);
         } // for (byte devIndex = 0; devIndex < 6; devIndex++)
         
      }
      
      //if(
      
      //RFCal_Helper_Rx_Params rxParam;
      //rx
   }

   return true;
}
unsigned char RFCal_Helper_Fb_Gain_Param_Compute(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, Sweep_Params & sweep_param,vector < cal_sweep_record >   &   Sweep, UINT compSwpIdx)
{
   unsigned short eptPAState;
   unsigned short etPAState;

   int refRGI;
   int currRGI;
   byte pa;
   int chan;

   // initializing to 0 since these are just passed to a function to calculate there values
   int uBias = 0;
   int rxScale = 0;

   eptPAState = cal_param.Swp[compSwpIdx].XPT_Swp2_EPT_PA_State;
   etPAState = cal_param.Swp[compSwpIdx].XPT_Swp2_ET_PA_State;

   byte rf_mode = cal_param.RF_Mode;

   byte device = cal_param.TxDeviceFlag;

   byte instance = 0;

   vector<cal_sweep_record>::iterator it;

   map<int, map<int,double>> chanPA_rgiPwr;
   map <RFCal_Helper_Tx_Params, double > APTV6CharToPwr;
   map < pair < dword, byte >, Tx_RefCal_Data_V2_Record > ChanFBGainStateToPwrFBGainParams;
   map< chanPAState_type, rgipwr_type> refcalchanPa_rgi_pwr;

   if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
   {
      RFCal_Helper_ESC_ET_Tx_AptChar_DataV6_Assignment_XML(demo_param,  cal_param, sweep_param, APTV6CharToPwr, refcalchanPa_rgi_pwr); // Key: Channel. pa_state, rgi
      RFCal_Helper_ESC_ET_FBGain_Ref_Assignment_XML(demo_param,  cal_param, sweep_param, ChanFBGainStateToPwrFBGainParams);
   }

  // chanFbGainState_pwrVsFbGain_type chanPa_pwr_fbgainresult;
   RFCal_Helper_ESC_ET_Tx_AptChar_DataV6_Assignment_XML(demo_param,  cal_param, sweep_param, APTV6CharToPwr, refcalchanPa_rgi_pwr); // Key: Channel. pa_state, rgi
   RFCal_Helper_ESC_ET_FBGain_Ref_Assignment_XML(demo_param,  cal_param, sweep_param, ChanFBGainStateToPwrFBGainParams);


 //  map<int,double>::const_iterator itCharRGIPwr;

   chan = static_cast<int>( sweep_param.Ref_Channel);

   // set up the ref rgi point
   for( it = Sweep.begin(); it != Sweep.end(); it++)
   {
      if( it->measSeg == true
         && (it->subSwpType == ESC_SUB_SWEEP_APPLY_DPD
         || it->swpType == INT_CAL_XPT_FREQCOMP) )
      {
         pa = it->txParams.txParams[0].paState;
         currRGI = it->txParams.txParams[0].rgi;
         refRGI = it->txIntCalParams.refRGI;
         
         if( it->swpType == INT_CAL_XPT_SWP1
            || it->swpType == INT_CAL_XPT_SWP3
            || (it->swpType == INT_CAL_XPT_FREQCOMP
            && it->subSwpType == ESC_SUB_SWEEP_APPLY_DPD) )
         {
            pa = static_cast<byte>(eptPAState);
            refRGI = cal_param.Swp[compSwpIdx].XPT_Swp1_RGI_Delta_Ref_RGI;
            it->txIntCalParams.refRGI = static_cast<byte>(refRGI);
         }

      }
   }
   return true;
}

unsigned char RFCal_Helper_CalculateFBStatebasedonExpectedPwr(const Sweep_Params & sweep_param, double expPower, unsigned short &fbGainStateIndex, int &fbGainState, int overlapMode)
{
   for(unsigned short fbGainStateSwptListitr=0; fbGainStateSwptListitr <sweep_param.FBGain_Cal_Gain_State_List.size(); ++fbGainStateSwptListitr)
   {
      short FBGain_Cal_Swep_Ovrlp_Threshold = (sweep_param.FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld_List.size() > 0)?sweep_param.FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld_List[fbGainStateSwptListitr]:0;
      if( expPower < sweep_param.FBGain_Cal_Gain_State_Swpt_List[fbGainStateSwptListitr] + FBGain_Cal_Swep_Ovrlp_Threshold*overlapMode)
      {
         fbGainStateIndex = fbGainStateSwptListitr;
         fbGainState = sweep_param.FBGain_Cal_Gain_State_List[fbGainStateSwptListitr];
      }
   }
   return true;
}
unsigned char RFCal_Helper_LoadMeasTxPwrFromSpecifiedSweepForXPTFreqComp( vector < cal_sweep_record > & currSweep, byte pa, int rgi, byte XPTMode, bool is_XPT_Measurement, double& charTxPwr, bool& status, Tx_RefCal_Data_V2_Record & preFBGainParam, byte FBGainState)
{
   vector<cal_sweep_record>::  iterator meas_iter;

   for( meas_iter = currSweep.begin();
      meas_iter < currSweep.end();
      ++meas_iter)
   {
     
      if ( 
         /* ( pa == static_cast<byte>(meas_iter->txParams.txParams[0].paState))
         && ( rgi == static_cast<int>(meas_iter->txParams.txParams[0].rgi)) 
         &&*/ ( XPTMode == static_cast<byte>(meas_iter->txParams.txParams[0].xptMode))
         && ( preFBGainParam.FBGainState == meas_iter->txMeasParams.txMeas[0].txMeasPayload.fbGainParams.fbGainState)
         && ( preFBGainParam.uBias == meas_iter->txMeasParams.txMeas[0].txMeasPayload.fbGainParams.uBias)
         && ( preFBGainParam.rxScale == meas_iter->txMeasParams.txMeas[0].txMeasPayload.fbGainParams.rxScale))
      {
         preFBGainParam.FBGain = meas_iter->txRes.fbGainMeas.fbGainRes;
         preFBGainParam.pwr = meas_iter->txRes.txPower/10.0;
         status = true;
         break;
      }
      
   }
  
   return true;
}
unsigned char RFCal_Helper_LoadMeasTxPwrFromSpecifiedSweep( vector < cal_sweep_record > & currSweep, byte devId, byte pa, int rgi, byte XPTMode, bool is_XPT_Measurement, double& charTxPwr, bool& status, Tx_RefCal_Data_V2_Record & preFBGainParam, byte FBGainState)
{
   vector<cal_sweep_record>::  iterator meas_iter;

   if( is_XPT_Measurement)
   {
      for( meas_iter = currSweep.begin();
         meas_iter < currSweep.end();
         ++meas_iter)
      {
         if(((static_cast<unsigned int>(meas_iter->dataRes.EptMlineAction) & RESULT_MLINE_TABLE) == RESULT_MLINE_TABLE) && XPTMode == XPT_TYPE_EPT)
         {
            unsigned int num_rows = static_cast<unsigned short>(meas_iter->dataRes.EptMlineRes.mline_table_res.num_mline_table_rows);
            for( unsigned int mline_table_loop_counter = 0; mline_table_loop_counter < num_rows; mline_table_loop_counter++)
            {
               if( rgi == static_cast<unsigned short>(meas_iter->dataRes.EptMlineRes.mline_table_res.mline_results[mline_table_loop_counter].rgi))
               {
                  charTxPwr = (static_cast<double>(static_cast<signed short>(meas_iter->dataRes.EptMlineRes.mline_table_res.mline_results[mline_table_loop_counter].pow_out_db100)))/100.0;
                  preFBGainParam.FBGain = meas_iter->txRes.fbGainMeas.fbGainRes;
                  preFBGainParam.rxScale = meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.rxScale;
                  preFBGainParam.uBias = meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.uBias;
                  preFBGainParam.FBGainState = meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.fbGainState; //just in case
                  
                  preFBGainParam.pwr = meas_iter->txRes.txPower/10.0; // Just in case

                  status = true;
                  break;
               }
            }
         }
         else if(((static_cast<unsigned int>(meas_iter->dataRes.EtMlineAction) & RESULT_MLINE_TABLE) == RESULT_MLINE_TABLE) && XPTMode == XPT_TYPE_ET)
         {
            unsigned int num_rows = static_cast<unsigned short>(meas_iter->dataRes.EtMlineRes.mline_table_res.num_mline_table_rows);
            for( unsigned int mline_table_loop_counter = 0; mline_table_loop_counter < num_rows; mline_table_loop_counter++)
            {
               if( rgi == static_cast<unsigned short>(meas_iter->dataRes.EtMlineRes.mline_table_res.mline_results[mline_table_loop_counter].rgi))
               {
                  charTxPwr = (static_cast<double>(static_cast<signed short>(meas_iter->dataRes.EtMlineRes.mline_table_res.mline_results[mline_table_loop_counter].pow_out_db100)))/100.0;
                  preFBGainParam.FBGain = meas_iter->txRes.fbGainMeas.fbGainRes;
                  preFBGainParam.rxScale = meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.rxScale;
                  preFBGainParam.uBias = meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.uBias;
                  preFBGainParam.FBGainState = meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.fbGainState; //just in case
                  
                  preFBGainParam.pwr = meas_iter->txRes.txPower/10.0; // Just in case

                  status =true;
                  break;
               }
            }
         }
      }
   }

   else
   {
      for( meas_iter = currSweep.begin();
         meas_iter < currSweep.end();
         ++meas_iter)
      {
         if((((FBGainState!= 255) && meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.fbGainState == FBGainState) 
          || (FBGainState == 255)) 
          && meas_iter->txParams.txParams[devId].paState ==pa && meas_iter->txParams.txParams[devId].rgi ==rgi /* && meas_iter->measSeg */ )
         {
            charTxPwr = (static_cast<double>(static_cast<signed short>(meas_iter->txRes.txPower)))/10.0;
            preFBGainParam.FBGain = meas_iter->txRes.fbGainMeas.fbGainRes;
            preFBGainParam.rxScale = meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.rxScale;
            preFBGainParam.uBias = meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.uBias;
            preFBGainParam.FBGainState = meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.fbGainState; //just in case
                  
            preFBGainParam.pwr = meas_iter->txRes.txPower/10.0; // Just in case

            status = true;
            break;
         }
      }
   }
   return true;
}
unsigned char RFCal_Helper_Fb_Gain_UBias_RxScale_Compute_from_Specified_Sweep(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, 
   Cal_Param & cal_param, 
   Sweep_Params & sweep_param,
   vector < cal_sweep_record >  & Sweep, 
   vector < cal_sweep_record >  & CompFromSweep, 
   vector < TE_Cal_Swp_Params > & TE_Records, 
   UINT iCompFromSwpIndex,  
   bool is_Param_Compute_for_XPT )
{
   int chan, fbgainstate, fbGSOvrlp;
   unsigned short fbgainstateindex, currPA, currRGI;
   bool status = false, perform_fb_gain_param_compute_for_curr_seg = false;
   byte currXPTMode;

   map<int, map<int,double>> chanPA_rgiPwr;
   
   fbgainstate = sweep_param.FBGain_Cal_Gain_State_List.front();
   fbGSOvrlp = sweep_param.FBGain_Cal_Gain_State_List.front();

   map <RFCal_Helper_Tx_Params, double > APTV6CharToPwr;
   map < pair < dword, byte >, Tx_RefCal_Data_V2_Record > ChanFBGainStateToPwrFBGainParams;
   map< chanPAState_type, rgipwr_type> refcalchanPa_rgi_pwr;

   if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
   {
      RFCal_Helper_ESC_ET_Tx_AptChar_DataV6_Assignment_XML(demo_param,  cal_param, sweep_param, APTV6CharToPwr, refcalchanPa_rgi_pwr); // Key: Channel. pa_state, rgi
      RFCal_Helper_ESC_ET_FBGain_Ref_Assignment_XML(demo_param,  cal_param, sweep_param, ChanFBGainStateToPwrFBGainParams);
   }

  
   vector<cal_sweep_record> intCalSwpRecordList;
   intCalSwpRecordList.clear();
   vector<cal_sweep_record>::iterator meas_iter;

   vector<TE_Cal_Swp_Params> intCalTERecordList;
   intCalTERecordList.clear();
   vector<TE_Cal_Swp_Params>::iterator te_iter;

  
   chan = static_cast<int>( sweep_param.Ref_Channel );
   byte devId = sweep_param.Device_Selection.Master_Device_ID;
   if(!is_Param_Compute_for_XPT)
   {
      for( meas_iter = Sweep.begin(), te_iter = TE_Records.begin();
      meas_iter != Sweep.end(), te_iter != TE_Records.end();
      ++meas_iter, ++te_iter)
      {
         if(!meas_iter->measSeg 
            || (meas_iter->measSeg && meas_iter->subSwpType == ESC_SUB_SWEEP_APPLY_DPD)
            || (meas_iter->measSeg && meas_iter->subSwpType == ESC_SUB_SWEEP_THERM_READ))
         {
            intCalSwpRecordList.push_back(*meas_iter);
            intCalTERecordList.push_back(*te_iter);
         }
         else
         {
            currPA = meas_iter->txParams.txParams[devId].paState;
            currRGI = meas_iter->txParams.txParams[devId].rgi;
            if (::RFCal_Helper_Is_FreqComp_SweepType(sweep_param.Sweep_Type))
            {
           //    currRGI +=1;
            }
            currXPTMode = meas_iter->txParams.txParams[devId].xptMode;
            double inputCharPwr;
            dword ichannel = meas_iter->radio_cfg.chan.chanInfo[0].ulChan;
           Tx_RefCal_Data_V2_Record preFBGainParam;
        
            RFCal_Helper_LoadMeasTxPwrFromSpecifiedSweep(CompFromSweep, devId, static_cast<byte>(currPA), currRGI, currXPTMode, is_Param_Compute_for_XPT, inputCharPwr, status, preFBGainParam);

            if(status)
            {
               RFCal_Helper_CalculateFBStatebasedonExpectedPwr(sweep_param, inputCharPwr, fbgainstateindex, fbgainstate, NO_OVERLAP);
               Tx_RefCal_Data_V2_Record refCalPwrFBGainResult = ChanFBGainStateToPwrFBGainParams[make_pair(static_cast<dword>(chan),static_cast<byte>(fbgainstate))];
       

                word outputRxScale = 0;
               byte outputuBias = 0;
               RFCal_Helper_CalculateUKernelParameters(inputCharPwr,refCalPwrFBGainResult.pwr,refCalPwrFBGainResult.rxScale,refCalPwrFBGainResult.uBias,outputRxScale,outputuBias,refCalPwrFBGainResult.FBGain);


               meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.fbGainState = static_cast<byte>(fbgainstate);
               meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.uBias = static_cast<byte>(outputuBias);
               meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.rxScale = static_cast<word>(outputRxScale);

               intCalSwpRecordList.push_back(*meas_iter);
               intCalTERecordList.push_back(*te_iter);

               if( sweep_param.FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld_List.size() > 0 && meas_iter->subSwpType != ESC_SUB_SWEEP_APPLY_DPD)               {
                  for(int overlp_iter = OVERLAP_MINUS; overlp_iter <= OVERLAP_PLUS; ++overlp_iter)
                  {
                     RFCal_Helper_CalculateFBStatebasedonExpectedPwr(sweep_param, inputCharPwr, fbgainstateindex, fbGSOvrlp, overlp_iter);

                     if(fbGSOvrlp != fbgainstate  && overlp_iter != NO_OVERLAP)
                     {
                        Tx_RefCal_Data_V2_Record refCalPwrFBGainResult = ChanFBGainStateToPwrFBGainParams[make_pair(static_cast<dword>(chan),static_cast<byte>(fbGSOvrlp))];
     
                         word outputRxScale = 0;
                        byte outputuBias = 0;
                        RFCal_Helper_CalculateUKernelParameters(inputCharPwr,refCalPwrFBGainResult.pwr,refCalPwrFBGainResult.rxScale,refCalPwrFBGainResult.uBias,outputRxScale,outputuBias,refCalPwrFBGainResult.FBGain);


                        meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.fbGainState = static_cast<byte>(fbGSOvrlp);
                        meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.uBias = static_cast<byte>(outputuBias);
                        meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.rxScale = static_cast<word>(outputRxScale);

                        intCalSwpRecordList.push_back(*meas_iter);
                        intCalTERecordList.push_back(*te_iter);
                     }
                  }
               }
            }
         }
      }

      Sweep.clear();
      TE_Records.clear();

      Sweep = intCalSwpRecordList;
      TE_Records = intCalTERecordList;

   /*   sweep_param.tx.addSwpParams[swpIdx].txIntCalParams.m_EstSwpTime = sweepParams.escSweep[swpIdx].swpRecords.size() * sweepParams.escSweep[swpIdx].swpRecords.back().opcode.segLen 
         + sweepParams.escSweep[swpIdx].swpRecords.front().opcode.segLen;*/
   }

   else
   {
      for( meas_iter = Sweep.begin();
      meas_iter != Sweep.end();
      ++meas_iter)
      {
         perform_fb_gain_param_compute_for_curr_seg = (meas_iter->measSeg == true) && (meas_iter->subSwpType == ESC_SUB_SWEEP_APPLY_DPD);
      
         if( perform_fb_gain_param_compute_for_curr_seg)
         {
            currPA = meas_iter->txParams.txParams[devId].paState;
            currRGI = meas_iter->txParams.txParams[devId].rgi;
            currXPTMode = meas_iter->txParams.txParams[devId].xptMode;

            double inputCharPwr;
             Tx_RefCal_Data_V2_Record preFBGainParam;
            RFCal_Helper_LoadMeasTxPwrFromSpecifiedSweep(CompFromSweep, devId, static_cast<byte>(currPA), currRGI, currXPTMode, is_Param_Compute_for_XPT, inputCharPwr, status, preFBGainParam);

  
           
 
            if(status)
            {            
                RFCal_Helper_CalculateFBStatebasedonExpectedPwr(sweep_param, inputCharPwr, fbgainstateindex, fbgainstate, NO_OVERLAP);
               Tx_RefCal_Data_V2_Record refCalPwrFBGainResult = ChanFBGainStateToPwrFBGainParams[make_pair(static_cast<dword>(chan),static_cast<byte>(fbgainstate))];
       

                word outputRxScale = 0;
               byte outputuBias = 0;
               RFCal_Helper_CalculateUKernelParameters(inputCharPwr,refCalPwrFBGainResult.pwr,refCalPwrFBGainResult.rxScale,refCalPwrFBGainResult.uBias,outputRxScale,outputuBias,refCalPwrFBGainResult.FBGain);


               meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.fbGainState = static_cast<byte>(fbgainstate);
               meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.uBias = static_cast<byte>(outputuBias);
               meas_iter->txMeasParams.txMeas[devId].txMeasPayload.fbGainParams.rxScale = static_cast<word>(outputRxScale);
            }
         }
      }
   }
   return true;

}
unsigned char RFCal_Helper_ESC_ET_PreComputation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, Sweep_Params & sweep_param,vector < vector < cal_sweep_record > > &   Sweep, vector < TE_Cal_Swp_Params > & TE_Records, UINT iSwpIndex)
{
   if (sweep_param.Pre_Sweep_Computation.size() != sweep_param.Compute_From_Sweep.size())
   {
      ::RFCal_Helper_Exception("Precomputation. The number of <Pre_Sweep_Compution> and <Compute_For_Sweep> is mismatch");
   }
   vector <byte>::iterator PreSwpIt;
   vector <byte>::iterator CompForSwpIt;
   for (PreSwpIt = sweep_param.Pre_Sweep_Computation.begin(), CompForSwpIt = sweep_param.Compute_From_Sweep.begin();
      PreSwpIt != sweep_param.Pre_Sweep_Computation.end(); PreSwpIt++,CompForSwpIt++)
   {
      switch(*PreSwpIt)
      {
      case FB_GAIN_PARAM_COMPUTE_TYPE: // 6
            RFCal_Helper_Fb_Gain_Param_Compute( demo_param,  cal_param,  sweep_param,   Sweep[iSwpIndex],  *CompForSwpIt );
            break;
      case FB_GAIN_UBIAS_RX_SCALE_COMPUTE_FOR_APT: //9
            RFCal_Helper_Fb_Gain_UBias_RxScale_Compute_from_Specified_Sweep( demo_param,  cal_param,  sweep_param,   Sweep[iSwpIndex], Sweep[*CompForSwpIt], TE_Records, *CompForSwpIt,  false );
            break;
      case FB_GAIN_UBIAS_RX_SCALE_COMPUTE_FOR_XPT: // 10
            RFCal_Helper_Fb_Gain_UBias_RxScale_Compute_from_Specified_Sweep( demo_param,  cal_param,  sweep_param,   Sweep[iSwpIndex], Sweep[*CompForSwpIt], TE_Records, *CompForSwpIt,  true );
            break;
      default:
         break;
      }
   }


   return true;
}
unsigned char RFCal_Helper_ESC_ET_PostComputation(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, const Sweep_Params & sweep_param,vector < cal_sweep_record >  &   Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
    //****************************************************************/
    // Post Computation
    //****************************************************************/
    for (UINT i=0; i < sweep_param.Post_Sweep_Computation.size(); i++)
    {
        switch (sweep_param.Post_Sweep_Computation[i])
        {
            case 5: // XPT_SWP2_SAVE_BIAS_RGI_FOR_PIN_CAL_SWP_TYPE
                RFCal_Helper_ESC_ET_PostComputation_Swp2ForPinCal(demo_param,cal_param,sweep_param,Sweep,sweep_param.Compute_For_Sweep[i]);
                break;
         case 7: // FB_GAIN_CAL_POWER_COMPUTE_TYPE
            RFCal_Helper_ESC_ET_PostComputation_FBGainPowerComputation(demo_param,cal_param,sweep_param,Sweep);
            break;
            default:
                break;
        }
    }
   if (  sweep_param.Sweep_Type == XPT_SWP1
         || sweep_param.Sweep_Type == MAX_BIAS_LINEARIZER
         || sweep_param.Sweep_Type == APT_LINEARIZER
         || sweep_param.Sweep_Type == XPT_SWP3
         || sweep_param.Sweep_Type == INT_CAL_XPT_SWP1
         || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_LINEARIZER
         || sweep_param.Sweep_Type == INT_CAL_XPT_SWP3
         || sweep_param.Sweep_Type == INT_CAL_APT_LINEARIZER
      )
   {
      //*********************************************************//
      //    Find if this sweep has HDET measurement
      //*********************************************************//
      unsigned char bHdetFound = false;
      for (UINT i = 0;  i < sweep_param.HDET_Avgs.size(); i++)
      {
         if (sweep_param.HDET_Avgs[i] > 0)
         {
            bHdetFound = true;
            break;
         }
      }
      for (UINT i = 0;  i < sweep_param.LPM_HDET_Avgs.size(); i++)
      {
         if (sweep_param.LPM_HDET_Avgs[i] > 0)
         {
            bHdetFound = true;
            break;
         }
      }

      //*********************************************************//
      //    Merge HDET and TxPwr measurement segment
      //*********************************************************//
      if (bHdetFound)
      {
         int device = Sweep[0].radio_cfg.device.masterDev; // device is 0
         for (UINT iSegNum = 1; iSegNum < Sweep.size(); iSegNum++)
         {
            if( Sweep[iSegNum].txMeasParams.txMeas[device].txMeasAction & ESC_TX_MEAS_ACTION_HDET_MEAS
                  || Sweep[iSegNum].txMeasParams.txMeas[device].txMeasAction & ESC_TX_MEAS_ACTION_LPM_HDET_MEAS)
            {
               if (Sweep[iSegNum].opcode.varLenOpcode.opCodeGrp.opCodeGrp0 != 0x8200)
               {
                  Sweep[iSegNum-1].txRes.hdet[device][0] = Sweep[iSegNum].txRes.hdet[device][0];
                  Sweep[iSegNum-1].txRes.hdet[device][1] = Sweep[iSegNum].txRes.hdet[device][1];

                  Sweep[iSegNum-1].txMeasParams.txMeas[device].txMeasAction =  Sweep[iSegNum].txMeasParams.txMeas[device].txMeasAction; // New additon Required after QMSL 6.0.56

                  Sweep[iSegNum-1].txMeasParams.txMeas[device].txMeasPayload.numHdetAvg = Sweep[iSegNum].txMeasParams.txMeas[device].txMeasPayload.numHdetAvg;
                  Sweep[iSegNum-1].txMeasParams.txMeas[device].txMeasPayload.numLpmHdetAvg = Sweep[iSegNum].txMeasParams.txMeas[device].txMeasPayload.numLpmHdetAvg;
                  Sweep[iSegNum].txMeasParams.txMeas[device].txMeasPayload.numHdetAvg = 0;
                  Sweep[iSegNum].txMeasParams.txMeas[device].txMeasPayload.numLpmHdetAvg = 0;

                  Sweep[iSegNum].opcode.varLenOpcode.opCodeGrp.opCodeGrp0        = 0; // to make NOP
                  Sweep[iSegNum].measSeg =  0;
                  Sweep[iSegNum].resExpected = 0;
                  Sweep[iSegNum-1].measSeg =  true;
               }
            }
         }
      }
   }
   if (sweep_param.Sweep_Type == REF_CAL && demo_param.stRefCalOffset_filename.size() > 0)
   {
      if (sweep_param.Do_FBGain_Cal)
      {
         map <RFCal_Helper_Tx_Params, double> mapTxParmasToPwr;
         RFCal_Helper_ESC_ET_RefCal_FBGainDataAssignment(demo_param,cal_param,sweep_param,Sweep,mapTxParmasToPwr);
         RFCal_Helper_WriteRefCalTxData_V2_ToXML(demo_param.stRefCalOffset_filename,mapTxParmasToPwr);
      }
      else
   {
      Tx_RefCal_Data data;
      RFCal_Helper_ESC_ET_RefCal_CalculateOffset(demo_param,cal_param,sweep_param,Sweep,data);
      RFCal_Helper_WriteRefCalTxDataToXML(demo_param.stRefCalOffset_filename,data);
   }
   }
   if ((  sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 
         || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_LINEARIZER)
      && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN)
   {
      for (UINT iSegNum = 1; iSegNum < Sweep.size(); iSegNum++)
      {
         if (Sweep[iSegNum].opcode.varLenOpcode.opCodeGrp.opCodeGrp0 & 0x4220 && Sweep[iSegNum].subSwpType != -1)
         {
            RFCal_Helper_Tx_Params Tx_Params;
            Tx_Params.Tech = demo_param.TECH;
            Tx_Params.Band = demo_param.BAND;
            Tx_Params.PaState = Sweep[iSegNum].txParams.txParams[0].paState;
            Tx_Params.Channel = Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan;
            Tx_Params.RGI = Sweep[iSegNum].txIntCalParams.refRGI;

            if (sweep_param.Low_Power_Threshold_PA == Tx_Params.PaState
                  &&  sweep_param.Low_Power_Threshold_RGI > Tx_Params.RGI)
            {
               double power = cal_param.TxParamsToPower[Tx_Params];
               Sweep[iSegNum].txRes.txPower = static_cast<word>(power*10.0);
            }
            else
            {
               int fbagc = (short)Sweep[iSegNum].rxRes.lnaOffset[1][0];
               int offset = cal_param.TxParamsToOffset[Tx_Params];

               if (demo_param.TECH == 3)
               {
                  Sweep[iSegNum].txRes.txPower = offset - fbagc;
               }
               else
               {
                  Sweep[iSegNum].txRes.txPower = offset + fbagc;
               }
            }
         }
      }
   }
    if ((   sweep_param.Sweep_Type == INT_CAL_XPT_SWP3
         || sweep_param.Sweep_Type == INT_CAL_APT_LINEARIZER
         || sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
         || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP
         || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP
        ) && sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN)
   {
      //! Reference Cal offsets
      map< pair<int,int>, map<double, signed short> > paRefRGI_freq_offset;

      if (sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
            || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP
            || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP)
      {
         map <RFCal_Helper_Tx_Params, int>::iterator TxParamsToOffset_Iterator;
         for (TxParamsToOffset_Iterator = cal_param.TxParamsToOffset.begin();
               TxParamsToOffset_Iterator != cal_param.TxParamsToOffset.end(); ++TxParamsToOffset_Iterator)
         {
            int pa  = static_cast<int>(TxParamsToOffset_Iterator->first.PaState);
            int refRGI = static_cast<int>(TxParamsToOffset_Iterator->first.RGI);
            int iChan = static_cast<int>(TxParamsToOffset_Iterator->first.Channel);
            int offset = static_cast<int>(TxParamsToOffset_Iterator->second);
            double dlfreq = 0.0, ulFreq = 0.0;
            RFCal_Helper_CalculateFrequencyFromULChan(demo_param.TECH,demo_param.BAND,iChan,ulFreq,dlfreq);
            paRefRGI_freq_offset[make_pair(pa,refRGI)].insert(make_pair(ulFreq,offset));
         }
      }

      byte device = sweep_param.Device_Selection.Master_Device_ID;
      byte fbDevice = sweep_param.Device_Selection.Slave_Device_ID;
      signed short LowPowerThresholdOffset = 0;
      byte refRGI;
      for (UINT iSegNum = 1; iSegNum < Sweep.size(); iSegNum++)
      {
         cout << iSegNum << endl;
         if (Sweep[iSegNum].measSeg == 1)
         {
            dword chan = Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan;
            dword refChan = sweep_param.Ref_Channel;
            byte pa = Sweep[iSegNum].txParams.txParams[device].paState;
            byte rgi = Sweep[iSegNum].txParams.txParams[device].rgi;
            signed short fbAGC, offset;
            double refPower = 0.0;
            if( sweep_param.FBRx_Cal_Mode != 2)
            {
               fbAGC = Sweep[iSegNum].rxRes.lnaOffset[fbDevice][0];
            }
            else
            {
               fbAGC = Sweep[iSegNum].txRes.fbGainMeas.fbGainRes;
            }

            Sweep[iSegNum].txIntCalRes.fbAGC = fbAGC;

            if( Sweep[iSegNum].subSwpType == ESC_SUB_SWEEP_APPLY_DPD )
            {
               pa = cal_param.Swp[0].XPT_Swp1_PA_State;
            }

            if( pa != 0xFF )
            {
               double dlFreq = 0.0, ulFreq = 0.0;
               RFCal_Helper_CalculateFrequencyFromULChan(demo_param.TECH,demo_param.BAND,chan,ulFreq,dlFreq);

               // Find ref RGIs and offsets
               if(   sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
                     || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP
                     || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP
                 )
               {
                  if( Sweep[iSegNum].subSwpType == ESC_SUB_SWEEP_APPLY_DPD )
                  {
                     refRGI = cal_param.Swp[0].XPT_Swp1_RGI_Delta_Ref_RGI;
                  }
                  else
                  {
                     refRGI =  Sweep[iSegNum].txIntCalParams.refRGI;
                  }
                  if ( !paRefRGI_freq_offset.empty () )
                  {
                     Find_Interpolated_Y(paRefRGI_freq_offset[make_pair(pa,refRGI)], ulFreq, offset);
                  }
                }
               else
               {
                  RFCal_Helper_Tx_Params Tx_Params;
                  Tx_Params.Tech = demo_param.TECH;
                  Tx_Params.Band = demo_param.BAND;
                  Tx_Params.PaState = Sweep[iSegNum].txParams.txParams[0].paState;
                  Tx_Params.Channel = Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan;
                  Tx_Params.RGI = Sweep[iSegNum].txIntCalParams.refRGI;

                  offset = static_cast<short>(cal_param.TxParamsToOffset[Tx_Params]);
                  refPower = cal_param.TxParamsToPower[Tx_Params];
                  Sweep[iSegNum].txIntCalRes.offset = offset;
               }

               // Apply offsets
               if( demo_param.TECH != 3     // Not LTE, TDSCDMA
                     && demo_param.TECH != 4 )
               {
                  Sweep[iSegNum].txRes.txPower = static_cast<word>(offset + fbAGC);
               }
               else
               {
                  Sweep[iSegNum].txRes.txPower = static_cast<word>(offset - fbAGC);
               }

               // Find and apply the low power threshold offset
               if( pa == sweep_param.Low_Power_Threshold_PA )
               {
                  if ( rgi == sweep_param.Low_Power_Threshold_RGI )
                  {
                     signed short iPower = static_cast<signed short>(Sweep[iSegNum].txRes.txPower);
                     LowPowerThresholdOffset =
                        static_cast<int>( (refPower * 10)
                                          - iPower );
                  }
                  else if( rgi < sweep_param.Low_Power_Threshold_RGI )
                  {
                     Sweep[iSegNum].txRes.txPower = static_cast<signed short>( (refPower  * 10)
                                                                               - LowPowerThresholdOffset );
                  }
               }
            }
         }
      }
   }

   return true;
}
void  RFCal_Helper_ESC_ET_Dummy_Result_Assignment(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, vector < cal_sweep_record >   &  Sweep)
{
   //***********************************************************//
   //    Dummy Result parameters
   //***********************************************************//

   /* Max Bias linearlizer */
   map < RFCal_Helper_Tx_Params, double > MaxBiasTxParamToPower;
   map < RFCal_Helper_Tx_Params, int >    MaxBiasTxParamToHdet;
   map < RFCal_Helper_Tx_Params, int >    MaxBiasTxParamToLpmHdet;
   map < RFCal_Helper_Tx_Params, int >    MaxBiasTxParamToFbAgc;

   map < RFCal_Helper_Tx_Params, double > Swp1AlignmentTxParamToPower;
   map < RFCal_Helper_Tx_Params, int >     Swp1AlignmentTxParamToFbAgc;

   map < RFCal_Helper_Tx_Params, double > Swp1RgiDeltaTxParamToPower;
   map < RFCal_Helper_Tx_Params, int >     Swp1RgiDeltaTxParamToFbAgc;

   /* APT linearlizer */
   map < RFCal_Helper_Tx_Params, double > AptTxParamToPower;
   map < RFCal_Helper_Tx_Params, int >    AptTxParamToHdet;
   map < RFCal_Helper_Tx_Params, int >    AptTxParamToLpmHdet;
   map < RFCal_Helper_Tx_Params, double > Xpt3TxParamToPower;
   map < RFCal_Helper_Tx_Params, int >     AptTxParamToFbAgc;
   map < RFCal_Helper_Tx_Params, int >    Xpt3TxParamToFbAgc;

   /* Freq Comp */
   map < RFCal_Helper_Tx_Params, double > XptFreqCompTxParamToPower;
   map < RFCal_Helper_Tx_Params, double > TxLimFreqCompTxParamToPower;
   map < RFCal_Helper_Tx_Params, int >    TxLimFreqCompTxParamToHdet;
   map < RFCal_Helper_Tx_Params, double > AptFreqCompTxParamToPower;
   map < RFCal_Helper_Rx_Params, int >    FreqCompRxParamToLNAoffset;
   map < RFCal_Helper_Rx_Params, int >    IntraCARxParamToLNAoffset;

   map < RFCal_Helper_Tx_Params, int >    TxLimFreqCompTxParamToFbAgc;
   map < RFCal_Helper_Tx_Params, int >    AptFreqCompTxParamToFbAgc;
   map < RFCal_Helper_Tx_Params, int >    XptFreqCompTxParamToFbAgc;

   unsigned char bUseXMLresult = demo_param.bUseXMLresult;
   if (demo_param.bUseXMLresult)
   {
      //**************************************************//
      //       parameters
      //**************************************************//
      MSXML2::IXMLDOMDocumentPtr spXMLDoc;
      MSXML2::IXMLDOMNodePtr pRoot_Node;

      string stTECH[5]= {"CDMA","WCDMA","GSM","LTE","TD-SCDMA"};

      //**************************************************//
      //       Load XML and get a node with the rf_mode
      //**************************************************//
      CoInitialize(NULL); // Initialize COM
      spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
      spXMLDoc->load(demo_param.stResultXML_filename.c_str());

      //**************************************************//
      //       Get the root nodes giving tech and band
      //**************************************************//
      stringstream stXPATH;
      stXPATH << "/source/TestCollection/Test[DataSetCollection/DataSet/Inputs/DI/V = '" << stTECH[demo_param.TECH] << "'][DataSetCollection/DataSet/Inputs/DI[N = 'RF Mode'][V = '"
              << cal_param.RF_Mode << "']]";

      pRoot_Node = spXMLDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());
      if (pRoot_Node == NULL)
      {
         stringstream ss;
         ss << "Could not open the result XML: " << demo_param.stResultXML_filename << " or opened, but the XML is not valid."
            << endl << "Use dummy result values" << endl;
         bUseXMLresult = false;
      }
      else
      {
         //**************************************************//
         //       Sweep 1
         //**************************************************//
         if (sweep_param.Sweep_Type == XPT_SWP1 || sweep_param.Sweep_Type == MAX_BIAS_LINEARIZER
               || sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_LINEARIZER)
         {
            MSXML2::IXMLDOMNodePtr pSweepResultNode = pRoot_Node->selectSingleNode((_bstr_t)"DataSetCollection[Name = 'MEASUREMENTS: Tx Linearizer Sweep']");
            if (pSweepResultNode != NULL)
            {
               MSXML2::IXMLDOMNodeListPtr pDataSetList = pSweepResultNode->selectNodes((_bstr_t)"DataSet");
               for (int i =0; i < pDataSetList->length; i++)
               {
                  MSXML2::IXMLDOMNodePtr pResultNode = pDataSetList->nextNode();
                  RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
                  double power;
                  word Hdet, LpmHdet;
                  int FBAGC;
                  RFCal_Helper_Read_Result(pResultNode,0,"Channel",txParams.Channel);
                  RFCal_Helper_Read_Result(pResultNode,0,"PA State",txParams.PaState);
                  RFCal_Helper_Read_Result(pResultNode,0,"TxAGC",txParams.RGI);
                  RFCal_Helper_Read_Result(pResultNode,0,"Vcc",txParams.Bias);
                  RFCal_Helper_Read_Result(pResultNode,0,"Icq",txParams.PaQuiescentCurrent);
                  if (sweep_param.Sweep_Type == XPT_SWP1 || sweep_param.Sweep_Type == MAX_BIAS_LINEARIZER)
                  {
                     if (!RFCal_Helper_Read_Result(pResultNode,1,"Power",power))
                     {
                        RFCal_Helper_Read_Result(pResultNode,1,"AptPower",power);
                     }
                     MaxBiasTxParamToPower.insert(make_pair(txParams,power));
                  }
                  else
                  {
                     RFCal_Helper_Read_Result(pResultNode,0,"FB AGC",FBAGC);
                     MaxBiasTxParamToFbAgc.insert(make_pair(txParams,FBAGC));
                  }
                  RFCal_Helper_Read_Result(pResultNode,1,"HDET",Hdet);
                  RFCal_Helper_Read_Result(pResultNode,1,"LPM HDET",LpmHdet);

                  MaxBiasTxParamToHdet.insert(make_pair(txParams,Hdet));
                  MaxBiasTxParamToLpmHdet.insert(make_pair(txParams,LpmHdet));
                  pResultNode.Release();
               }
               pSweepResultNode.Release();
            }
         }
         if (sweep_param.Sweep_Type == REF_CAL)
         {
            MSXML2::IXMLDOMNodePtr pSweepResultNode = pRoot_Node->selectSingleNode((_bstr_t)"DataSetCollection[Name = 'MEASUREMENTS: Max Bias Reference Cal']");
            if (pSweepResultNode != NULL)
            {
               MSXML2::IXMLDOMNodeListPtr pDataSetList = pSweepResultNode->selectNodes((_bstr_t)"DataSet");
               for (int i =0; i < pDataSetList->length; i++)
               {
                  MSXML2::IXMLDOMNodePtr pResultNode = pDataSetList->nextNode();
                  RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
                  double power;
                  RFCal_Helper_Read_Result(pResultNode,0,"Channel",txParams.Channel);
                  RFCal_Helper_Read_Result(pResultNode,0,"PA State",txParams.PaState);
                  RFCal_Helper_Read_Result(pResultNode,0,"TxAGC",txParams.RGI);
                  RFCal_Helper_Read_Result(pResultNode,0,"Band",txParams.Band);
                  RFCal_Helper_Read_Result(pResultNode,1,"Power",power);
                  MaxBiasTxParamToPower.insert(make_pair(txParams,power));

                  int FBAGC = 0;
                  RFCal_Helper_Read_Result(pResultNode,1,"FB AGC",FBAGC);
                  MaxBiasTxParamToFbAgc.insert(make_pair(txParams,FBAGC));
                  pResultNode.Release();
               }
               pSweepResultNode.Release();
            }
         }

         if (sweep_param.Sweep_Type == XPT_SWP1 || sweep_param.Sweep_Type == INT_CAL_XPT_SWP1)
         {
            MSXML2::IXMLDOMNodePtr pSweepResultNode = pRoot_Node->selectSingleNode((_bstr_t)"DataSetCollection[Name = 'MEASUREMENTS: Tx XPT Swp1 - Alignment Sweep']");
            if (pSweepResultNode != NULL)
            {
               MSXML2::IXMLDOMNodeListPtr pDataSetList = pSweepResultNode->selectNodes((_bstr_t)"DataSet");
               for (int i = 0; i < pDataSetList->length; i++)
               {
                  MSXML2::IXMLDOMNodePtr pResultNode = pDataSetList->nextNode();
                  RFCal_Helper_Tx_Params txParams(demo_param.TECH,demo_param.BAND);
                  double power;

                  RFCal_Helper_Read_Result(pResultNode,0,"Channel",txParams.Channel);
                  RFCal_Helper_Read_Result(pResultNode,0,"PA State",txParams.PaState);
                  RFCal_Helper_Read_Result(pResultNode,0,"TxAGC",txParams.RGI);
                  RFCal_Helper_Read_Result(pResultNode,0,"Vcc",txParams.Bias);

                  if (sweep_param.Sweep_Type == XPT_SWP1 )
                  {
                     RFCal_Helper_Read_Result(pResultNode,1,"XPT Swp1 Power",power);
                     Swp1AlignmentTxParamToPower.insert(make_pair(txParams,power));
                  }
                  else
                  {
                     int FBAGC = 0;
                     RFCal_Helper_Read_Result(pResultNode,1,"FB AGC",FBAGC);
                     Swp1AlignmentTxParamToFbAgc.insert(make_pair(txParams,FBAGC));
                  }
                  pResultNode.Release();
               }
               pDataSetList.Release();
               pSweepResultNode.Release();
            }
         }
         if (sweep_param.Sweep_Type == XPT_SWP1 || sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 )
         {
            MSXML2::IXMLDOMNodePtr pSweepResultNode = pRoot_Node->selectSingleNode((_bstr_t)"DataSetCollection[Name = 'MEASUREMENTS: Tx XPT Swp1 - RGI Delta Sweep']");
            if (pSweepResultNode != NULL)
            {
               MSXML2::IXMLDOMNodeListPtr pDataSetList = pSweepResultNode->selectNodes((_bstr_t)"DataSet");
               for (int i = 0; i < pDataSetList->length; i++)
               {
                  MSXML2::IXMLDOMNodePtr pResultNode = pDataSetList->nextNode();
                  RFCal_Helper_Tx_Params txParams(demo_param.TECH,demo_param.BAND);
                  double power;
                  RFCal_Helper_Read_Result(pResultNode,0,"Channel",txParams.Channel);
                  RFCal_Helper_Read_Result(pResultNode,0,"PA State",txParams.PaState);
                  RFCal_Helper_Read_Result(pResultNode,0,"TxAGC",txParams.RGI);
                  RFCal_Helper_Read_Result(pResultNode,0,"Vcc",txParams.Bias);

                  if (sweep_param.Sweep_Type == XPT_SWP1 )
                  {
                     RFCal_Helper_Read_Result(pResultNode,1,"XPT Swp1 RGI Delta Swp Power",power);
                     Swp1RgiDeltaTxParamToPower.insert(make_pair(txParams,power));
                  }
                  else
                  {
                     int FBAGC = 0;
                     RFCal_Helper_Read_Result(pResultNode,1,"FB AGC",FBAGC);
                     Swp1RgiDeltaTxParamToFbAgc.insert(make_pair(txParams,FBAGC));
                  }

                  pResultNode.Release();
               }
               pDataSetList.Release();
               pSweepResultNode.Release();
            }
         }

         //**************************************************//
         //       Sweep 3
         //**************************************************//
         if (  sweep_param.Sweep_Type == XPT_SWP3
               || sweep_param.Sweep_Type == APT_LINEARIZER
               || sweep_param.Sweep_Type == INT_CAL_XPT_SWP3
               || sweep_param.Sweep_Type == INT_CAL_APT_LINEARIZER )
         {
            MSXML2::IXMLDOMNodePtr pSweepResultNode = pRoot_Node->selectSingleNode((_bstr_t)"DataSetCollection[Name = 'MEASUREMENTS: APT Tx Linearizer Sweep']");
            if (pSweepResultNode != NULL)
            {
               MSXML2::IXMLDOMNodeListPtr pDataSetList = pSweepResultNode->selectNodes((_bstr_t)"DataSet");
               for (int i =0; i < pDataSetList->length; i++)
               {
                  MSXML2::IXMLDOMNodePtr pResultNode = pDataSetList->nextNode();
                  RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
                  double power;
                  int FbAgc;
                  word Hdet, LpmHdet;
                  RFCal_Helper_Read_Result(pResultNode,0,"Channel",txParams.Channel);
                  RFCal_Helper_Read_Result(pResultNode,0,"PA State",txParams.PaState);
                  RFCal_Helper_Read_Result(pResultNode,0,"TxAGC",txParams.RGI);
                  RFCal_Helper_Read_Result(pResultNode,0,"FB AGC",FbAgc);

                  RFCal_Helper_Read_Result(pResultNode,1,"AptPower",power);
                  RFCal_Helper_Read_Result(pResultNode,1,"HDET",Hdet);
                  RFCal_Helper_Read_Result(pResultNode,1,"LPM HDET",LpmHdet);

                  AptTxParamToPower.insert(make_pair(txParams,power));
                  AptTxParamToHdet.insert(make_pair(txParams,Hdet));
                  AptTxParamToLpmHdet.insert(make_pair(txParams,LpmHdet));
                  AptTxParamToFbAgc.insert(make_pair(txParams,FbAgc));

                  pResultNode.Release();
               }
               pDataSetList.Release();
               pSweepResultNode.Release();
            }
         }
         if (sweep_param.Sweep_Type == XPT_SWP3 || sweep_param.Sweep_Type == INT_CAL_XPT_SWP3 )
         {
            MSXML2::IXMLDOMNodePtr pSweepResultNode = pRoot_Node->selectSingleNode((_bstr_t)"DataSetCollection[Name = 'MEASUREMENTS: Tx XPT Swp3 - Linearizer']");
            if (pSweepResultNode != NULL)
            {
               MSXML2::IXMLDOMNodeListPtr pDataSetList = pSweepResultNode->selectNodes((_bstr_t)"DataSet");
               for (int i =0; i < pDataSetList->length; i++)
               {
                  MSXML2::IXMLDOMNodePtr pResultNode = pDataSetList->nextNode();
                  RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
                  double power;
                  RFCal_Helper_Read_Result(pResultNode,0,"Channel",txParams.Channel);
                  RFCal_Helper_Read_Result(pResultNode,0,"XPT Mode",txParams.XPTmode);
                  RFCal_Helper_Read_Result(pResultNode,0,"PA State",txParams.PaState);
                  RFCal_Helper_Read_Result(pResultNode,0,"Exp Power",txParams.ExpPwr);
                  int FbAgc;
                  if (sweep_param.Sweep_Type == XPT_SWP3 )
                  {
                     RFCal_Helper_Read_Result(pResultNode,1,"XPT Swp3 Power",power);
                     Xpt3TxParamToPower.insert(make_pair(txParams,power));
                  }
                  else
                  {
                     RFCal_Helper_Read_Result(pResultNode,1,"FB AGC",FbAgc);
                     Xpt3TxParamToFbAgc.insert(make_pair(txParams,FbAgc));
                  }
                  pResultNode.Release();
               }
               pDataSetList.Release();
               pSweepResultNode.Release();
            }
         }

         //**************************************************//
         //       FreqComp
         //**************************************************//
         if (sweep_param.Sweep_Type == XPT_FREQCOMP
               || sweep_param.Sweep_Type == XPT_FREQCOMP_STAGGER_RXTX
               || sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP )
         {
            MSXML2::IXMLDOMNodePtr pSweepResultNode = pRoot_Node->selectSingleNode((_bstr_t)"DataSetCollection[Name = 'MEASUREMENTS: Tx XPT - FreqComp']");
            if (pSweepResultNode != NULL)
            {
               MSXML2::IXMLDOMNodeListPtr pDataSetList = pSweepResultNode->selectNodes((_bstr_t)"DataSet");
               for (int i =0; i < pDataSetList->length; i++)
               {
                  MSXML2::IXMLDOMNodePtr pResultNode = pDataSetList->nextNode();
                  RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
                  double power;
                  RFCal_Helper_Read_Result(pResultNode,0,"Channel",txParams.Channel);
                  RFCal_Helper_Read_Result(pResultNode,0,"XPT Mode",txParams.XPTmode);
                  RFCal_Helper_Read_Result(pResultNode,0,"Meas Exp Power",txParams.ExpPwr);
                  txParams.ExpPwr /= 100.0;
                  if (sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP )
                  {
                     int FBAGC = 0;
                     RFCal_Helper_Read_Result(pResultNode,1,"FB AGC",FBAGC);
                     XptFreqCompTxParamToFbAgc.insert(make_pair(txParams,FBAGC));
                  }
                  else
                  {
                     RFCal_Helper_Read_Result(pResultNode,1,"XPT FComp Power",power);

                     XptFreqCompTxParamToPower.insert(make_pair(txParams,power));
                  }
                  pResultNode.Release();
               }
               pDataSetList.Release();
               pSweepResultNode.Release();
            }
         }
         if (sweep_param.Sweep_Type == XPT_FREQCOMP
               || sweep_param.Sweep_Type == APT_FREQCOMP
               || sweep_param.Sweep_Type == MAX_BIAS_FREQCOMP
               || sweep_param.Sweep_Type == XPT_FREQCOMP_STAGGER_RXTX
               || sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
               || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP
               || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP
            )
         {
            MSXML2::IXMLDOMNodePtr pSweepResultNode = pRoot_Node->selectSingleNode((_bstr_t)"DataSetCollection[Name = 'MEASUREMENTS: Tx Limiting Freq Comp']");
            if (pSweepResultNode != NULL)
            {
               MSXML2::IXMLDOMNodeListPtr pDataSetList = pSweepResultNode->selectNodes((_bstr_t)"DataSet");
               for (int i =0; i < pDataSetList->length; i++)
               {
                  MSXML2::IXMLDOMNodePtr pResultNode = pDataSetList->nextNode();
                  RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
                  double power;
                  word Hdet;
                  RFCal_Helper_Read_Result(pResultNode,0,"Channel",txParams.Channel);
                  RFCal_Helper_Read_Result(pResultNode,0,"PA State",txParams.PaState);
                  RFCal_Helper_Read_Result(pResultNode,0,"TxAGC",txParams.RGI);
                  RFCal_Helper_Read_Result(pResultNode,1,"TxLimFCompPowerNvIn",power);
                  RFCal_Helper_Read_Result(pResultNode,1,"TxLimFCompHDETNvIn",Hdet);
                  TxLimFreqCompTxParamToHdet.insert(make_pair(txParams,Hdet));

                  if (  sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
                        || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP
                        || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP )
                  {
                     int FBAGC = 0;
                     RFCal_Helper_Read_Result(pResultNode,1,"FB AGC",FBAGC);
                     TxLimFreqCompTxParamToFbAgc.insert(make_pair(txParams,FBAGC));
                  }
                  else
                  {
                     TxLimFreqCompTxParamToPower.insert(make_pair(txParams,power));
                  }
                  pResultNode.Release();
               }
               pDataSetList.Release();
               pSweepResultNode.Release();
            }
         }

         if (sweep_param.Sweep_Type == XPT_FREQCOMP
               || sweep_param.Sweep_Type == APT_FREQCOMP
               || sweep_param.Sweep_Type == MAX_BIAS_FREQCOMP
               || sweep_param.Sweep_Type == XPT_FREQCOMP_STAGGER_RXTX
               || sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
               || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP
               || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP
            )
         {
            MSXML2::IXMLDOMNodePtr pSweepResultNode = pRoot_Node->selectSingleNode((_bstr_t)"DataSetCollection[Name = 'MEASUREMENTS: Tx Linearizer Primary Freq Comp']");
            if (pSweepResultNode != NULL)
            {
               MSXML2::IXMLDOMNodeListPtr pDataSetList = pSweepResultNode->selectNodes((_bstr_t)"DataSet");

               for (int i =0; i < pDataSetList->length; i++)
               {
                  MSXML2::IXMLDOMNodePtr pResultNode = pDataSetList->nextNode();
                  RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
                  double power;
                  RFCal_Helper_Read_Result(pResultNode,0,"Channel",txParams.Channel);
                  RFCal_Helper_Read_Result(pResultNode,0,"PA State",txParams.PaState);
                  RFCal_Helper_Read_Result(pResultNode,0,"TxAGC",txParams.RGI);
                  if (  sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
                        || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP
                        || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP )
                  {
                     int FBAGC = 0;
                     RFCal_Helper_Read_Result(pResultNode,1,"FB AGC",FBAGC);
                     AptFreqCompTxParamToFbAgc.insert(make_pair(txParams,FBAGC));
                  }
                  else
                  {
                     RFCal_Helper_Read_Result(pResultNode,1,"TxLinFCompPower",power);
                     AptFreqCompTxParamToPower.insert(make_pair(txParams,power));
                  }
                  pResultNode.Release();
               }
               pDataSetList.Release();
               pSweepResultNode.Release();
            }
         }
         if (sweep_param.Sweep_Type == XPT_FREQCOMP
               || sweep_param.Sweep_Type == APT_FREQCOMP
               || sweep_param.Sweep_Type == MAX_BIAS_FREQCOMP
               || sweep_param.Sweep_Type == XPT_FREQCOMP_STAGGER_RXTX

            )
         {
            for (UINT devIndex = 0; devIndex < 4; devIndex++)
            {
               stringstream ss;
               ss << "DataSetCollection[Name = 'MEASUREMENTS: LNA Offset Freq Comp Device: " << devIndex << "']";
               MSXML2::IXMLDOMNodePtr pSweepResultNode = pRoot_Node->selectSingleNode((_bstr_t)ss.str().c_str());
               if (pSweepResultNode != NULL)
               {
                  MSXML2::IXMLDOMNodeListPtr pDataSetList = pSweepResultNode->selectNodes((_bstr_t)"DataSet");
                  for (int i =0; i < pDataSetList->length; i++)
                  {
                     MSXML2::IXMLDOMNodePtr pResultNode = pDataSetList->nextNode();
                     RFCal_Helper_Rx_Params rxParams;
                     int LNAoffset;
                     RFCal_Helper_Read_Result(pResultNode,0,"Channel",rxParams.Channel);
                     RFCal_Helper_Read_Result(pResultNode,0,"RxPath",rxParams.RxPath);
                     //RFCal_Helper_Read_Result(pResultNode,0,"RxLevel",rxParams.RxLevel);
                     RFCal_Helper_Read_Result(pResultNode,0,"LNARange",rxParams.LNARange);
                     RFCal_Helper_Read_Result(pResultNode,0,"RxPM",rxParams.RxPM);

                     RFCal_Helper_Read_Result(pResultNode,1,"RxFCompLNAOffset",LNAoffset);

                     FreqCompRxParamToLNAoffset.insert(make_pair(rxParams,LNAoffset));
                     pResultNode.Release();
                  }
                  pDataSetList.Release();
                  pSweepResultNode.Release();
               }
            }
         }
         //**************************************************//
         //       Intra CA
         //**************************************************//
         if (sweep_param.Sweep_Type == INTRACA_FCOMP )
         {
            for (UINT devIndex = 0; devIndex < 4; devIndex++)
            {
               stringstream ss;
               ss << "DataSetCollection[Name = 'MEASUREMENTS: Intraband-CA LNA Offset Freq Comp Device: " << devIndex << "']";
               MSXML2::IXMLDOMNodePtr pSweepResultNode = pRoot_Node->selectSingleNode((_bstr_t)ss.str().c_str());
               if (pSweepResultNode != NULL)
               {
                  MSXML2::IXMLDOMNodeListPtr pDataSetList = pSweepResultNode->selectNodes((_bstr_t)"DataSet");
                  for (int i =0; i < pDataSetList->length; i++)
                  {
                     MSXML2::IXMLDOMNodePtr pResultNode = pDataSetList->nextNode();
                     RFCal_Helper_Rx_Params rxParams;
                     int LNAoffset;
                     RFCal_Helper_Read_Result(pResultNode,0,"Channel",rxParams.Channel);
                     RFCal_Helper_Read_Result(pResultNode,0,"RxPath",rxParams.RxPath);
                     //RFCal_Helper_Read_Result(pResultNode,0,"RxLevel",rxParams.RxLevel);
                     RFCal_Helper_Read_Result(pResultNode,0,"LNARange",rxParams.LNARange);
                     RFCal_Helper_Read_Result(pResultNode,0,"RxPM",rxParams.RxPM);

                     RFCal_Helper_Read_Result(pResultNode,1,"IntraCA_RxFCompLNAOffset",LNAoffset);

                     IntraCARxParamToLNAoffset.insert(make_pair(rxParams,LNAoffset));
                     pResultNode.Release();
                  }
                  pDataSetList.Release();
                  pSweepResultNode.Release();
               }
            }
         }
         pRoot_Node.Release();
      }
      spXMLDoc.Release();
      CoUninitialize();
   } // end of if (demo_param.bUseXMLresult)

   //***********************************************************//
   //    Sweep 1 Dummy Result Assignment
   //***********************************************************//

   if (sweep_param.Sweep_Type == XPT_SWP1 || sweep_param.Sweep_Type == MAX_BIAS_LINEARIZER
         || sweep_param.Sweep_Type == REF_CAL  || sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_LINEARIZER)
   {
      //***********************************************************//
      //    Assign Power and HDET results.
      //      If you execute QLIB_FTM_GET_ESC_SWEEP,
      //    the function automatically fills in the HDET data from a DUT into the following sweep record
      //      You need to assign the power data from a callbox
      //***********************************************************//
      map <int, int > paToPaindex;
      for (UINT paIndex = 0; paIndex < sweep_param.PA_State_List.size(); paIndex++)
      {
         paToPaindex.insert(make_pair(sweep_param.PA_State_List[paIndex],paIndex));  // e.g., LTE 2,1,0,0 is mapped to  PA2 --> Index 0, PA1 --> Index 1, PA0 --> Index 0
      }
      double dummyPwr_MBL_PA[][120] =
      {
         {27.9, 27.7, 27.6, 27.4, 27.1, 27, 26.5, 25.8, 25.1, 24, 22.8, 21.8, 20.4, 19.6, 18.5, 17.7, 16.2, 15.1, 13.9, 12.7, 11.5, 11, 9.9, 8.9, 7.3, 6.6, 5.6, 5, 4.1, 3.3, 2.5, 0.9, 0.1, -0.4, -1.5, -2.5, -2.9, -4.2, -5.2, -6.2, -7, -8.2, -9.6, -10.2, -11.4, -12.7, -13.8, -14.2, -14.9, -16.2, -17.2, -17.9, -18.6, -19.8, -20.5, -20.9, -22.1, -22.8, -23.6, -23.7, -25.5, -26.2, -26.8, -27.5, -28.1, -28.5, -29.4, -30.5, -30.9, -31.7, -32.2, -32.7, -33.3, -33.9, -34.2, -34.7, -35.1, -35.2, -35.4, -35.6, -36.0, -36.4, -37.0, -37.4},
         {26.5, 25.7, 25, 23.9, 22.7, 21.7, 20.4, 19.5, 18.5, 17.6, 16.1, 15, 13.8, 12.6, 11.4, 11, 9.8, 8.8, 7.2, 6.5, 5.6, 4.9, 4, 3.2, 2.4, 0.9, 0, -0.5, -1.5, -2.6, -3, -4.3, -5.3, -6.3, -7, -8.3, -9.7, -10.2, -11.5, -12.8, -13.9, -14.3, -15, -16.3, -17.4, -18, -18.7, -19.9, -20.6, -21.1, -22.2, -23, -23.9, -23.9, -25.8, -26.6, -27.3, -28, -28.7, -29.2, -30.2, -31.5, -31.9, -33.1, -33.8, -34.5, -35.5, -36.4, -36.9, -38.2, -39, -39.2, -39.5, -40.3, -40.7, -40.9, -41.2, -41.5, -41.5, -41.8, -42.2, -42.6, -43.1, -43.7, -44.2, -44.6, -45.1},
         {20, 19, 17.7, 16.8, 15.7, 14.5, 13.2, 12, 10.7, 9.5, 8.3, 7.8, 6.7, 5.6, 4.7, 4, 3.1, 2.4, 1.5, 0.7, 0, -1.6, -2.4, -3, -4.1, -5.1, -5.5, -6.8, -7.7, -8.8, -9.5, -10.7, -12.1, -12.7, -13.9, -15.2, -16.4, -16.7, -17.5, -18.8, -19.8, -20.5, -21.2, -22.4, -23.1, -23.6, -24.7, -25.5, -26.4, -26.4, -28.3, -29.1, -29.8, -30.6, -31.3, -31.8, -32.9, -34.2, -34.8, -36, -36.9, -37.7, -38.9, -40, -40.9, -42.6, -43.9, -44.3, -45, -46.8, -47.8, -48.5, -49.8, -50.7, -51.3, -52.4, -53.3, -54.3, -55.3, -56, -56.2, -56.8, -57.5, -58.1, -58.9, -59.4},
         {20, 19, 17.7, 16.8, 15.7, 14.5, 13.2, 12, 10.7, 9.5, 8.3, 7.8, 6.7, 5.6, 4.7, 4, 3.1, 2.4, 1.5, 0.7, 0, -1.6, -2.4, -3, -4.1, -5.1, -5.5, -6.8, -7.7, -8.8, -9.5, -10.7, -12.1, -12.7, -13.9, -15.2, -16.4, -16.7, -17.5, -18.8, -19.8, -20.5, -21.2, -22.4, -23.1, -23.6, -24.7, -25.5, -26.4, -26.4, -28.3, -29.1, -29.8, -30.6, -31.3, -31.8, -32.9, -34.2, -34.8, -36, -36.9, -37.7, -38.9, -40, -40.9, -42.6, -43.9, -44.3, -45, -46.8, -47.8, -48.5, -49.8, -50.7, -51.3, -52.4, -53.3, -54.3, -55.3, -56, -56.2, -56.8, -57.5, -58.1, -58.9, -59.4}
      };

      word dummyHDET_MBL[] = {3274, 3139, 3068, 2954, 2825, 2745, 2509, 2191, 1944, 1612, 1342, 1158, 977, 887, 796, 736, 658, 615, 581, 553, 532, 525, 512, 501, 491, 487, 481, 479, 477, 476, 474, 473, 470, 468, 469, 469, 470, 468, 469, 467, 468, 467, 467, 467, 465, 466, 468, 468, 466, 466, 467, 467, 467, 467, 466, 467, 467, 467, 467, 467, 467, 466, 466, 466, 465, 465, 465, 464, 465, 467, 467, 466, 466, 467, 467, 467, 465, 467, 467, 466};

      //***********************************************************//
      //    Alignment and RGIDeltaPower.
      //      If sweep 2 fails, then, please modify these pwr values.
      //    or use XML.
      //***********************************************************//

      word dummyPwr_AlighmentSwp[] = {185, 218, 241, 259, 265, 280, 291, 300};

      word dummyPwr_RGI_Delta[] = {38, 49, 59, 68, 75, 87, 96, 108, 116, 128, 140, 148, 161, 172, 189, 199, 210, 219, 230, 236, 248, 251,255,259,263,267,271,275,279,280};

      map <int, int> paIndexToHighestRGI;
      for (UINT i=0; i < sweep_param.RGI_List.size(); i++)
      {
         paIndexToHighestRGI.insert(make_pair(i,sweep_param.RGI_List[i][0]));
      }
      for (UINT iSegNum = 0; iSegNum<Sweep.size(); iSegNum++)
      {
         //***********************************************************//
         //    MAX Bias Linearlizer power
         //***********************************************************//
         if (Sweep[iSegNum].txPurpose == 3 || Sweep[iSegNum].txPurpose == 255 ) // Tx Pwr
         {
            RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
            txParams.Channel = static_cast<word>(Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan);
            txParams.PaState = Sweep[iSegNum].txParams.txParams[0].paState;
            txParams.RGI = Sweep[iSegNum].txParams.txParams[0].rgi;

            if (sweep_param.Sweep_Type == REF_CAL)
            {
               txParams.Band = (byte)Sweep[iSegNum].radio_cfg.band;
            }
            else
            {
               txParams.Bias = (dword)Sweep[iSegNum].txParams.txParams[0].bias;
               txParams.PaQuiescentCurrent = (byte)Sweep[iSegNum].txParams.txParams[0].quiescentCurrent;
            }

            if (demo_param.bSimulationMode && bUseXMLresult && MaxBiasTxParamToFbAgc.size() > 0)
            {
               int fbAGC = MaxBiasTxParamToFbAgc[txParams];
               Sweep[iSegNum].rxRes.lnaOffset[1][0] = fbAGC;
            }

            if (  sweep_param.Sweep_Type == INT_CAL_XPT_SWP1
                  || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_LINEARIZER)
            {
               continue;
            }

            if (bUseXMLresult && MaxBiasTxParamToPower.size() > 0)
            {
               word power = static_cast<word>(MaxBiasTxParamToPower[txParams]*10.0);
               Sweep[iSegNum].txRes.txPower = power;
            }
            else
            {
               int paIndex = paToPaindex[txParams.PaState];
               int rgiIndex = (paIndexToHighestRGI[paIndex] - txParams.RGI );

               Sweep[iSegNum].txRes.txPower = static_cast<word>(dummyPwr_MBL_PA[paIndex][ rgiIndex]*10.0);
            }
         }
         //***********************************************************//
         //    Alignement sweep power
         //***********************************************************//
         if (Sweep[iSegNum].subSwpType == ESC_SUB_SWEEP_XPT_ALIGNMENT)
         {
            byte rgi = Sweep[iSegNum].txParams.txParams[0].rgi;
            byte pa  = Sweep[iSegNum].txParams.txParams[0].paState;
            dword bias = Sweep[iSegNum].txParams.txParams[0].bias;

            vector <byte> XPT_Swp1_RGI_List;
            vector <word> XPT_Swp1_Bias_List;

            if (cal_param.En_EPTET_Different_PAstate == 1 && pa == sweep_param.XPT_Swp1_PA_State_EPT)
            {
               XPT_Swp1_RGI_List = sweep_param.XPT_Swp1_RGI_List_EPT;
               XPT_Swp1_Bias_List = sweep_param.XPT_Swp1_Bias_List_EPT;
            }
            else if (cal_param.En_EPTET_Different_PAstate == 1 && pa == sweep_param.XPT_Swp1_PA_State_ET)
            {
               XPT_Swp1_RGI_List = sweep_param.XPT_Swp1_RGI_List_ET;
               XPT_Swp1_Bias_List = sweep_param.XPT_Swp1_Bias_List_ET;
            }
            else
            {
               XPT_Swp1_RGI_List = sweep_param.XPT_Swp1_RGI_List;
               XPT_Swp1_Bias_List = sweep_param.XPT_Swp1_Bias_List;
            }

            for (UINT i=0 ; i < XPT_Swp1_RGI_List.size(); i ++)
            {
               if (rgi == XPT_Swp1_RGI_List[i] && bias == XPT_Swp1_Bias_List[i])
               {
                  RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
                  txParams.PaState = pa;
                  txParams.RGI = rgi;
                  txParams.Bias = bias;
                  txParams.Channel = static_cast<word>(Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan);
                  // txParams.PaQuiescentCurrent = Sweep[iSegNum].txParams.txParams[0].quiescentCurrent;

                  if (sweep_param.Sweep_Type == INT_CAL_XPT_SWP1)
                  {
                     if (demo_param.bSimulationMode && bUseXMLresult && Swp1AlignmentTxParamToFbAgc.size() > 0)
                     {
                        int fbAGC = Swp1AlignmentTxParamToFbAgc[txParams];
                        Sweep[iSegNum].rxRes.lnaOffset[1][0] = static_cast<word>(fbAGC);
                     }
                  }
                  else
                  {
                     double pwr = Swp1AlignmentTxParamToPower[txParams];
                     if (bUseXMLresult)
                     {
                        Sweep[iSegNum].txRes.txPower = static_cast<word>(pwr*10.0);
                     }
                     else
                     {
                        Sweep[iSegNum].txRes.txPower = dummyPwr_AlighmentSwp[i];
                     }
                     break;
                  }
               }
            }
         }
         //***********************************************************//
         //    RGI Delta sweep power
         //***********************************************************//
         if (Sweep[iSegNum].subSwpType == ESC_SUB_SWEEP_RGI_DELTA)
         {
            byte rgi = Sweep[iSegNum].txParams.txParams[0].rgi;
            byte pa  = Sweep[iSegNum].txParams.txParams[0].paState;
            vector <byte> XPT_Swp1_RGI_Delta_RGI_List;
            if (cal_param.En_EPTET_Different_PAstate == 1 && pa == sweep_param.XPT_Swp1_PA_State_EPT)
            {
               XPT_Swp1_RGI_Delta_RGI_List = sweep_param.XPT_Swp1_RGI_Delta_RGI_List_EPT;
            }
            else if (cal_param.En_EPTET_Different_PAstate == 1 && pa == sweep_param.XPT_Swp1_PA_State_ET)
            {
               XPT_Swp1_RGI_Delta_RGI_List = sweep_param.XPT_Swp1_RGI_Delta_RGI_List_ET;
            }
            else
            {
               XPT_Swp1_RGI_Delta_RGI_List = sweep_param.XPT_Swp1_RGI_Delta_RGI_List;
            }

            for (UINT i =0 ; i < XPT_Swp1_RGI_Delta_RGI_List.size(); i ++)
            {
               if (rgi == XPT_Swp1_RGI_Delta_RGI_List[i])
               {
                  RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
                  txParams.PaState = Sweep[iSegNum].txParams.txParams[0].paState;
                  txParams.RGI = rgi;
                  txParams.Bias = Sweep[iSegNum].txParams.txParams[0].bias;
                  txParams.Channel = static_cast<word>(Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan);
                  // txParams.PaQuiescentCurrent = Sweep[iSegNum].txParams.txParams[0].quiescentCurrent;

                  if (sweep_param.Sweep_Type == INT_CAL_XPT_SWP1)
                  {
                     if (demo_param.bSimulationMode && bUseXMLresult && Swp1RgiDeltaTxParamToFbAgc.size() > 0)
                     {
                        int fbAGC = Swp1RgiDeltaTxParamToFbAgc[txParams];
                        Sweep[iSegNum].rxRes.lnaOffset[1][0] = static_cast<word>(fbAGC);
                     }
                  }
                  else
                  {
                     if (bUseXMLresult)
                     {
                        word power = static_cast<word>(Swp1RgiDeltaTxParamToPower[txParams]*10.0);
                        Sweep[iSegNum].txRes.txPower = power;
                     }
                     else
                     {
                        Sweep[iSegNum].txRes.txPower = dummyPwr_RGI_Delta[i];
                     }
                     break;
                  }
               }
            }
         }
         if(demo_param.bSimulationMode)
         {
            //***********************************************************//
            //    HDET
            //***********************************************************//
            if (Sweep[iSegNum].opcode.varLenOpcode.opCodeGrp.opCodeGrp0 & 0x8000) // HDET
            {
               int rgi = Sweep[iSegNum-1].txParams.txParams[0].rgi; // seperate HDET and TxPwr measurement

               if (bUseXMLresult)
               {
                  RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
                  txParams.PaState = Sweep[iSegNum-1].txParams.txParams[0].paState;
                  txParams.RGI = Sweep[iSegNum-1].txParams.txParams[0].rgi;
                  txParams.Bias = Sweep[iSegNum-1].txParams.txParams[0].bias;
                  txParams.Channel = static_cast<word>(Sweep[iSegNum-1].radio_cfg.chan.chanInfo[0].ulChan);
                  txParams.PaQuiescentCurrent = Sweep[iSegNum-1].txParams.txParams[0].quiescentCurrent;
                  Sweep[iSegNum].txRes.hdet[0][0] = MaxBiasTxParamToHdet[txParams];
                  Sweep[iSegNum].txRes.hdet[0][1] = MaxBiasTxParamToLpmHdet[txParams];
               }
               else
               {
                  int rgiIndex = paIndexToHighestRGI[0] - rgi;
                  Sweep[iSegNum].txRes.hdet[0][0]  = dummyHDET_MBL[rgiIndex];
                  // ToDo: LpmHdet
               }
            }
         }
      }
   }

   //***********************************************************//
   //    Sweep 2 Dummy Result Assignment
   //***********************************************************//

   if (sweep_param.Sweep_Type == XPT_SWP2 || sweep_param.Sweep_Type == INT_CAL_XPT_SWP2) // Sweep 2
   {
      if(demo_param.bSimulationMode)
      {
         //***********************************************************//
         //    Assign dummy DPD table.
         //      If you execute QLIB_FTM_GET_ESC_SWEEP,
         //    the function automatically fills in the data from a DUT into the following sweep record
         //***********************************************************//

         word Detrough[][128] =
         {
            { 4098, 4104, 4113, 4125, 4141, 4160, 4182, 4206, 4233, 4263, 4295, 4330, 4366, 4404, 4445, 4487, 4532, 4577, 4625, 4674, 4724, 4776, 4829, 4883, 4939, 4996, 5053, 5112, 5172, 5233, 5294, 5357, 5420, 5484, 5549, 5614, 5681, 5747, 5815, 5883, 5951, 6020, 6089, 6159, 6230, 6300, 6372, 6443, 6515, 6587, 6660, 6733, 6806, 6880, 6953, 7027, 7102, 7176, 7251, 7326, 7401, 7476, 7552, 7627, 7703, 7779, 7855, 7932, 8008, 8085, 8162, 8238, 8315, 8392, 8470, 8547, 8624, 8701, 8779, 8856, 8934, 9012, 9090, 9167, 9245, 9323, 9401, 9479, 9557, 9635, 9714, 9792, 9870, 9949, 10027, 10105, 10184, 10262, 10341, 10420, 10498, 10576, 10655, 10734, 10812, 10891, 10970, 11048, 11127, 11206, 11285, 11364, 11443, 11521, 11600, 11679, 11758, 11837, 11916, 11994, 12073, 12152, 12231, 12310, 12389, 12468, 12547, 12626},
            { 4099, 4108, 4122, 4143, 4167, 4197, 4232, 4271, 4314, 4361, 4412, 4466, 4524, 4585, 4649, 4716, 4786, 4859, 4934, 5011, 5091, 5173, 5258, 5344, 5432, 5522, 5613, 5706, 5801, 5897, 5995, 6094, 6194, 6296, 6398, 6502, 6607, 6712, 6819, 6927, 7035, 7145, 7255, 7365, 7477, 7589, 7702, 7815, 7929, 8044, 8159, 8274, 8391, 8507, 8624, 8741, 8859, 8977, 9096, 9214, 9333, 9453, 9572, 9692, 9812, 9933, 10053, 10175, 10295, 10417, 10538, 10660, 10782, 10904, 11026, 11149, 11271, 11394, 11516, 11639, 11762, 11886, 12009, 12132, 12256, 12379, 12503, 12627, 12750, 12874, 12998, 13122, 13246, 13370, 13494, 13619, 13743, 13867, 13992, 14116, 14241, 14365, 14490, 14614, 14739, 14864, 14988, 15113, 15238, 15363, 15488, 15613, 15738, 15862, 15987, 16112, 16237, 16362, 16487, 16612, 16737, 16862, 16987, 17112, 17237, 17362, 17488, 17613},
            { 4100, 4111, 4130, 4155, 4187, 4225, 4269, 4318, 4373, 4433, 4497, 4566, 4640, 4717, 4799, 4884, 4973, 5066, 5161, 5260, 5361, 5466, 5573, 5682, 5794, 5908, 6025, 6143, 6264, 6386, 6510, 6636, 6763, 6892, 7023, 7155, 7288, 7422, 7558, 7695, 7833, 7972, 8111, 8252, 8394, 8536, 8680, 8824, 8969, 9114, 9261, 9407, 9555, 9703, 9852, 10001, 10150, 10300, 10451, 10602, 10753, 10906, 11058, 11210, 11363, 11516, 11669, 11823, 11977, 12131, 12286, 12440, 12595, 12750, 12906, 13061, 13217, 13373, 13529, 13685, 13842, 13998, 14155, 14312, 14469, 14625, 14783, 14940, 15097, 15255, 15413, 15570, 15728, 15886, 16043, 16201, 16360, 16517, 16676, 16834, 16992, 17150, 17309, 17467, 17625, 17784, 17943, 18101, 18260, 18418, 18577, 18736, 18895, 19053, 19212, 19371, 19530, 19689, 19848, 20007, 20165, 20324, 20484, 20643, 20802, 20960, 21120, 21279},
            { 4101, 4117, 4141, 4176, 4218, 4270, 4329, 4396, 4470, 4550, 4637, 4731, 4830, 4935, 5045, 5160, 5280, 5405, 5534, 5667, 5804, 5945, 6089, 6237, 6388, 6542, 6699, 6859, 7022, 7187, 7355, 7525, 7697, 7871, 8046, 8225, 8404, 8586, 8769, 8954, 9140, 9327, 9516, 9706, 9897, 10090, 10284, 10478, 10674, 10870, 11067, 11265, 11465, 11665, 11866, 12067, 12268, 12471, 12675, 12878, 13082, 13288, 13493, 13698, 13905, 14112, 14318, 14526, 14734, 14942, 15151, 15359, 15568, 15778, 15988, 16197, 16408, 16619, 16829, 17040, 17251, 17462, 17674, 17886, 18097, 18309, 18521, 18734, 18946, 19158, 19371, 19584, 19797, 20010, 20223, 20436, 20650, 20863, 21076, 21290, 21503, 21717, 21931, 22145, 22358, 22573, 22786, 23000, 23215, 23428, 23643, 23857, 24072, 24286, 24500, 24715, 24929, 25144, 25358, 25572, 25787, 26001, 26217, 26431, 26645, 26860, 27075, 27290}
         };
         word AMAM[][16] =
         {
            { 3937, 7874, 11767, 15434, 18923, 22320, 25752, 29236, 32957, 36864, 41116, 45496, 50095, 54942, 59997, 65535},
            { 4244, 8488, 12619, 16396, 19964, 23557, 27168, 30900, 34816, 38835, 42955, 47167, 51514, 55909, 60531, 65535},
            { 4511, 9005, 13196, 16972, 20608, 24275, 27975, 31839, 35760, 39794, 43877, 48057, 52300, 56591, 61010, 65535},
            { 4607, 9209, 13534, 17445, 21142, 24930, 28798, 32731, 36695, 40715, 44759, 48834, 52782, 56902, 61145, 65535}
         };
         word AMPM[][16] =
         {
            { 913, 908, 776, 498, 292, 144, 44, 38, 29, 31, 19, 30, 10, -18, -68, -177},
            { 949, 948, 764, 497, 312, 199, 137, 115, 112, 106, 78, 39, -15, -80, -121, -149},
            { 1045, 1041, 912, 581, 352, 240, 212, 203, 189, 151, 119, 51, -39, -104, -194, -215},
            { 1092, 1090, 911, 617, 452, 364, 328, 288, 253, 187, 86, 19, -70, -131, -226, -308}
         };
         byte resRGI[] = {81, 82, 83, 84, 85, 86, 87 ,88};
         word smpsBias[] = {1833, 2320, 2678, 3265, 3300, 3400, 3500, 3600};

         for (UINT iSegNum = 0; iSegNum<Sweep.size(); iSegNum++)
         {
            if (Sweep[iSegNum].data.loadCalcDpd.do_load_calc_dpd_op == true && Sweep[iSegNum].data.loadCalcDpd.xptMode == 2) // Load cal, response is detrough table
            {
               for (int i = 0; i < 4; i++)
               {
                  Sweep[iSegNum].dataRes.measLoadEPDTRes[i].detValid = true;
                  Sweep[iSegNum].dataRes.measLoadEPDTRes[i].measured = true;
                  memcpy(&Sweep[iSegNum].dataRes.measLoadEPDTRes[i].epdtTbl[0],&Detrough[i][0],sizeof(word)*128);
               }
            }
            // ET Case
            if (Sweep[iSegNum].data.dpdCalc.do_dpd_calc_op == true && Sweep[iSegNum].data.dpdCalc.xptMode == 2) // Do Proc result DPD, response is AMAM / AMPM table
            {
               for (UINT i = 0; i < sweep_param.ET_Swp2_Meas_Pwr_List.size(); i++)
               {
                  Sweep[iSegNum].dataRes.measDpdRes[i].dpdValid = true;
                  Sweep[iSegNum].dataRes.measDpdRes[i].xptMode = 2;
                  Sweep[iSegNum].dataRes.measDpdRes[i].rgi = resRGI[i];
                  Sweep[iSegNum].dataRes.measDpdRes[i].smpsBias = smpsBias[i];
                  Sweep[iSegNum].dataRes.measDpdRes[i].paState= sweep_param.XPT_Swp2_ET_PA_State;
                  Sweep[iSegNum].dataRes.measDpdRes[i].measured = true;
                  Sweep[iSegNum].dataRes.measDpdRes[i].power = static_cast<word>(sweep_param.ET_Swp2_Meas_Pwr_List[i]*100.0);
                  memcpy(&Sweep[iSegNum].dataRes.measDpdRes[i].amamRes[0],&AMAM[0],sizeof(word)*16);
                  memcpy(&Sweep[iSegNum].dataRes.measDpdRes[i].ampmRes[0],&AMPM[0],sizeof(word)*16);
               }
            }
            // EPT case
            if (Sweep[iSegNum].data.dpdCalc.do_dpd_calc_op == true && Sweep[iSegNum].data.dpdCalc.xptMode == 1) // Do Proc result DPD, response is AMAM / AMPM table
            {
               for (UINT i = 0; i < sweep_param.EPT_Swp2_Meas_Pwr_List.size(); i++)
               {
                  Sweep[iSegNum].dataRes.measDpdRes[i].dpdValid = true;
                  Sweep[iSegNum].dataRes.measDpdRes[i].xptMode = 1;
                  Sweep[iSegNum].dataRes.measDpdRes[i].rgi = resRGI[i];
                  Sweep[iSegNum].dataRes.measDpdRes[i].smpsBias = smpsBias[i];
                  Sweep[iSegNum].dataRes.measDpdRes[i].paState= sweep_param.XPT_Swp2_EPT_PA_State;
                  Sweep[iSegNum].dataRes.measDpdRes[i].measured = true;
                  Sweep[iSegNum].dataRes.measDpdRes[i].power = static_cast<word>(sweep_param.EPT_Swp2_Meas_Pwr_List[i]*100.0);
                  memcpy(&Sweep[iSegNum].dataRes.measDpdRes[i].amamRes[0],&AMAM[0],sizeof(word)*16);
                  memcpy(&Sweep[iSegNum].dataRes.measDpdRes[i].ampmRes[0],&AMPM[0],sizeof(word)*16);
               }
               for (UINT i = 0; i < sweep_param.EPT_Swp2_Calc_Pwr_List.size(); i++)
               {
                  Sweep[iSegNum].dataRes.calcDpdRes[i].dpdValid = true;
                  Sweep[iSegNum].dataRes.calcDpdRes[i].xptMode = 1;
                  Sweep[iSegNum].dataRes.calcDpdRes[i].rgi = resRGI[i];
                  Sweep[iSegNum].dataRes.calcDpdRes[i].smpsBias = smpsBias[i];
                  Sweep[iSegNum].dataRes.calcDpdRes[i].paState= sweep_param.XPT_Swp2_EPT_PA_State;
                  Sweep[iSegNum].dataRes.calcDpdRes[i].measured = false;
                  Sweep[iSegNum].dataRes.calcDpdRes[i].power = static_cast<word>(sweep_param.EPT_Swp2_Calc_Pwr_List[i]*100.0);
                  memcpy(&Sweep[iSegNum].dataRes.calcDpdRes[i].amamRes[0],&AMAM[0],sizeof(word)*16);
                  memcpy(&Sweep[iSegNum].dataRes.calcDpdRes[i].ampmRes[0],&AMPM[0],sizeof(word)*16);
               }
            }
         }
      }
   }

   //***********************************************************//
   //    Sweep 3 Dummy Result Assignment
   //***********************************************************//

   if (  sweep_param.Sweep_Type == XPT_SWP3
         || sweep_param.Sweep_Type == APT_LINEARIZER
         || sweep_param.Sweep_Type == INT_CAL_XPT_SWP3
         || sweep_param.Sweep_Type == INT_CAL_APT_LINEARIZER
      ) // Sweep 3 APT linearlizer
   {
      map <int, int > paToPaindex;
      for (UINT paIndex = 0; paIndex < sweep_param.PA_State_List.size(); paIndex++)
      {
         paToPaindex.insert(make_pair(sweep_param.PA_State_List[paIndex],paIndex));  // e.g., LTE 2,1,0,0 is mapped to  PA2 --> Index 0, PA1 --> Index 1, PA0 --> Index 0
      }
      map <int, int> paIndexToHighestRGI;
      for (UINT i=0; i < sweep_param.RGI_List.size(); i++)
      {
         paIndexToHighestRGI.insert(make_pair(i,sweep_param.RGI_List[i][0]));
      }

      short dummyAPT_Pwer[][128] =
      {
         {270, 267, 266, 263, 260, 258, 252, 243, 232, 217, 202, 188, 170, 156, 140, 126, 104, 89, 73, 58, 43, 37, 24, 12, -5, -13, -23, -31, -41, -51, -60, -79, -88, -95, -107, -121, -125, -141, -153, -165, -172, -185, -201, -207, -218, -233, -244, -247, -254, -266, -275, -281, -287, -297, -301, -306, -314, -318, -324, -324, -335, -337, -341, -345, -345, -348, -351, -353, -355, -358, -357, -359, -359, -357, -361, -360, -362, -361, -362, -361, -364, -366, -366, -370},
         {210, 193, 185, 176, 165, 156, 143, 132, 116, 103, 83, 69, 54, 40, 23, 17, 2, -9, -26, -35, -46, -55, -66, -76, -83, -104, -114, -123, -137, -156, -160, -173, -183, -193, -200, -213, -226, -232, -244, -257, -268, -272, -278, -291, -301, -308, -314, -325, -332, -336, -345, -352, -359, -359, -373, -379, -383, -387, -391, -394, -399, -406, -406, -410, -412, -412, -417, -417, -419, -421, -421, -421, -423, -424, -424, -424, -425, -425, -425, -425, -426, -428, -428, -430},
         { 91, 85, 68, 59, 46, 33, 18, 4, -12, -27, -41, -48, -59, -74, -84, -92, -104, -112, -122, -132, -140, -162, -172, -180, -195, -207, -212, -225, -234, -245, -252, -265, -278, -284, -296, -309, -321, -324, -332, -345, -355, -362, -369, -381, -388, -393, -405, -412, -421, -422, -440, -448, -455, -463, -470, -475, -486, -498, -505, -516, -525, -533, -544, -554, -564, -579, -591, -593, -599, -613, -623, -627, -636, -641, -643, -652, -655, -660, -665, -670, -675, -680},
         { 91, 85, 68, 59, 46, 33, 18, 4, -12, -27, -41, -48, -59, -74, -84, -92, -104, -112, -122, -132, -140, -162, -172, -180, -195, -207, -212, -225, -234, -245, -252, -265, -278, -284, -296, -309, -321, -324, -332, -345, -355, -362, -369, -381, -388, -393, -405, -412, -421, -422, -440, -448, -455, -463, -470, -475, -486, -498, -505, -516, -525, -533, -544, -554, -564, -579, -591, -593, -599, -613, -623, -627, -636, -641, -643, -652, -655, -660, -665, -670, -675, -680}
      };
      short dummyXPT_Pwr[] = {208, 231, 247, 263, 267, 275 , 280, 285};
      short dummyTherm = 1152;
      for (UINT iSegNum = 0; iSegNum<Sweep.size(); iSegNum++)
      {
         //***********************************************************//
         //    APT Linearlizer power
         //***********************************************************//
         if (Sweep[iSegNum].txParams.txParams[0].pwrOverrideType == 1 && Sweep[iSegNum].txParams.txParams[0].rgi > 0) // Tx Pwr
         {
            RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
            txParams.PaState = Sweep[iSegNum].txParams.txParams[0].paState;
            txParams.RGI = Sweep[iSegNum].txParams.txParams[0].rgi;
            txParams.Channel = static_cast<word>(Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan);
            if ( sweep_param.Sweep_Type == INT_CAL_XPT_SWP3 || sweep_param.Sweep_Type == INT_CAL_APT_LINEARIZER)
            {
               if (demo_param.bSimulationMode && bUseXMLresult && AptTxParamToFbAgc.size() > 0)
               {
                  int fbAGC = AptTxParamToFbAgc[txParams];
                  Sweep[iSegNum].rxRes.lnaOffset[1][0] = static_cast<word>(fbAGC);
               }
            }
            else
            {
               if (bUseXMLresult)
               {
                  word power = static_cast<word>(AptTxParamToPower[txParams]*10.0);
                  Sweep[iSegNum].txRes.txPower = power;
               }
               else
               {
                  int paIndex = paToPaindex[txParams.PaState];
                  int rgiIndex =   (paIndexToHighestRGI[paIndex] - txParams.RGI );
                  Sweep[iSegNum].txRes.txPower = dummyAPT_Pwer[paIndex][ rgiIndex];
               }
            }
         }
         //***********************************************************//
         //    XPT Linearlizer power
         //***********************************************************//
         if (Sweep[iSegNum].txParams.txParams[0].pwrOverrideType == 2)
         {
            for (UINT i =0; i < sweep_param.ET_Swp3_Meas_Pwr_List.size(); i++)
            {
               if (Sweep[iSegNum].txParams.txParams[0].expPwr == static_cast<word>(sweep_param.ET_Swp3_Meas_Pwr_List[i]*100.0))
               {
                  Sweep[iSegNum].txRes.txPower = dummyXPT_Pwr[i];
                  break;
               }
            }
            for (UINT i =0; i < sweep_param.EPT_Swp3_Meas_Pwr_List.size(); i++)
            {
               if (Sweep[iSegNum].txParams.txParams[0].expPwr == static_cast<word>(sweep_param.EPT_Swp3_Meas_Pwr_List[i]*100.0))
               {
                  Sweep[iSegNum].txRes.txPower = dummyXPT_Pwr[i];
                  break;
               }
            }
            for (UINT i =0; i < sweep_param.EPT_Swp3_Calc_Pwr_List.size(); i++)
            {
               if (Sweep[iSegNum].txParams.txParams[0].expPwr == static_cast<word>(sweep_param.EPT_Swp3_Calc_Pwr_List[i]*100.0))
               {
                  Sweep[iSegNum].txRes.txPower = dummyXPT_Pwr[i];
                  break;
               }
            }
            if (bUseXMLresult)
            {
               RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
               txParams.Channel = static_cast<word>(Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan);
               txParams.XPTmode = Sweep[iSegNum].txParams.txParams[0].xptMode;
               txParams.PaState = Sweep[iSegNum].txParams.txParams[0].paState;
               txParams.ExpPwr = static_cast<double>(Sweep[iSegNum].txParams.txParams[0].expPwr)/100.0;

               if ( sweep_param.Sweep_Type == INT_CAL_XPT_SWP3 )
               {
                  if (demo_param.bSimulationMode && bUseXMLresult && Xpt3TxParamToFbAgc.size() > 0)
                  {
                     int fbAGC = Xpt3TxParamToFbAgc[txParams];
                     Sweep[iSegNum].rxRes.lnaOffset[1][0] = static_cast<word>(fbAGC);
                  }
               }
               else
               {
                  word power = static_cast<word>(Xpt3TxParamToPower[txParams]*10.0);
                  Sweep[iSegNum].txRes.txPower = power;
               }
            }
         }
         if(demo_param.bSimulationMode)
         {
            //***********************************************************//
            //    Therm Read (this value is from a DUT)
            //***********************************************************//
            if (Sweep[iSegNum].txMeasParams.txMeas[0].txMeasAction == 4)
            {
               Sweep[iSegNum].txRes.hdet[0][2] = dummyTherm;
            }
         }
      }
   }

   //***********************************************************//
   //    Sweep 4 & 6 Dummy Result Assignment
   //***********************************************************//

   if (sweep_param.Sweep_Type == XPT_FREQCOMP
         || sweep_param.Sweep_Type == APT_FREQCOMP
         || sweep_param.Sweep_Type == XPT_FREQCOMP_STAGGER_RXTX
         || sweep_param.Sweep_Type == INTRACA_FCOMP
         || sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
         || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP
         || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP
      )
   {
      int dummyAPT_Power_Sweep4[][6] =
      {
         { 226, 167, 121, 35, 35, 35},
         { 224, 163, 116, 29, 29, 29},
         { 221, 160, 114, 27, 27, 27},
         { 219, 160, 114, 28, 27, 27},
         { 219, 159, 114, 28, 28, 28},
         { 216, 157, 112, 27, 27, 27},
         { 214, 155, 111, 25, 25, 25},
         { 212, 154, 110, 25, 25, 25},
         { 211, 154, 110, 26, 26, 26},
         { 211, 154, 111, 28, 28, 28},
         { 212, 156, 113, 29, 29, 29},
         { 213, 156, 113, 30, 30, 30},
         { 212, 155, 112, 29, 29, 29},
         { 209, 152, 108, 25, 25, 25},
         { 206, 149, 105, 21, 21, 21},
         { 202, 145, 103, 19, 19, 19},
      };
      int dummyET_Power_Sweep4[][4] =
      {
         { 171, 207, 231, 255},
         { 174, 210, 235, 258},
         { 179, 215, 240, 264},
         { 183, 219, 243, 266},
         { 183, 219, 243, 266},
         { 183, 219, 242, 266},
         { 181, 217, 242, 266},
         { 181, 217, 241, 265},
         { 181, 216, 240, 264},
         { 180, 215, 238, 262},
         { 180, 215, 239, 262},
         { 180, 214, 238, 262},
         { 178, 213, 237, 261},
         { 177, 213, 237, 261},
         { 179, 215, 238, 263},
         { 181, 216, 240, 264}
      };

      int dummyHDET_Sweep4[] = {1344, 1354, 1356, 1365, 1370, 1362, 1340, 1355, 1348, 1351, 1341, 1322, 1301, 1276, 1275, 1255};

      int dummyRxRes_LNAOffset[][16][6] =
      {
         {
            {65534, 65533, 65533, 14, 14, 14},
            {65535, 65534, 65534, 15, 15, 14},
            {0, 65535, 65535, 15, 15, 14},
            {65535, 65534, 65534, 14, 14, 14},
            {65535, 65535, 65535, 14, 15, 14},
            {2, 1, 1, 17, 17, 16},
            {4, 3, 3, 18, 18, 18},
            {3, 2, 3, 17, 17, 17},
            {2, 0, 1, 15, 16, 16},
            {0, 65535, 0, 15, 15, 15},
            {1, 0, 1, 16, 16, 16},
            {4, 3, 3, 18, 18, 18},
            {6, 5, 6, 20, 20, 19},
            {5, 4, 5, 19, 19, 19},
            {5, 4, 4, 20, 20, 20},
            {8, 7, 8, 25, 23, 22}
         },
         {
            {65534, 65533, 65533, 14, 14, 14},
            {65535, 65534, 65534, 15, 15, 14},
            {0, 65535, 65535, 15, 15, 14},
            {65535, 65534, 65534, 14, 14, 14},
            {65535, 65535, 65535, 14, 15, 14},
            {2, 1, 1, 17, 17, 16},
            {4, 3, 3, 18, 18, 18},
            {3, 2, 3, 17, 17, 17},
            {2, 0, 1, 15, 16, 16},
            {0, 65535, 0, 15, 15, 15},
            {1, 0, 1, 16, 16, 16},
            {4, 3, 3, 18, 18, 18},
            {6, 5, 6, 20, 20, 19},
            {5, 4, 5, 19, 19, 19},
            {5, 4, 4, 20, 20, 20},
            {8, 7, 8, 25, 23, 22}
         } ,
         {
            {65534, 65533, 65533, 14, 14, 14},
            {65535, 65534, 65534, 15, 15, 14},
            {0, 65535, 65535, 15, 15, 14},
            {65535, 65534, 65534, 14, 14, 14},
            {65535, 65535, 65535, 14, 15, 14},
            {2, 1, 1, 17, 17, 16},
            {4, 3, 3, 18, 18, 18},
            {3, 2, 3, 17, 17, 17},
            {2, 0, 1, 15, 16, 16},
            {0, 65535, 0, 15, 15, 15},
            {1, 0, 1, 16, 16, 16},
            {4, 3, 3, 18, 18, 18},
            {6, 5, 6, 20, 20, 19},
            {5, 4, 5, 19, 19, 19},
            {5, 4, 4, 20, 20, 20},
            {8, 7, 8, 25, 23, 22}
         },
         {
            {65534, 65533, 65533, 14, 14, 14},
            {65535, 65534, 65534, 15, 15, 14},
            {0, 65535, 65535, 15, 15, 14},
            {65535, 65534, 65534, 14, 14, 14},
            {65535, 65535, 65535, 14, 15, 14},
            {2, 1, 1, 17, 17, 16},
            {4, 3, 3, 18, 18, 18},
            {3, 2, 3, 17, 17, 17},
            {2, 0, 1, 15, 16, 16},
            {0, 65535, 0, 15, 15, 15},
            {1, 0, 1, 16, 16, 16},
            {4, 3, 3, 18, 18, 18},
            {6, 5, 6, 20, 20, 19},
            {5, 4, 5, 19, 19, 19},
            {5, 4, 4, 20, 20, 20},
            {8, 7, 8, 25, 23, 22}
         }
      };
      // APT Power assignment
      int EtPwrIdx = 0;
      int EptCalcPwrIdx = 0;
      int EptMeasPwrIdx = 0;

      int AptPwrIdx = 0;
      int iSizeAptPA = sweep_param.PA_State_List.size();
      int iSizeETPwr = sweep_param.ET_Fcomp_Meas_Pwr_List.size();
      int iSizeEPTCalc = sweep_param.EPT_Fcomp_Calc_Pwr_List.size();
      int iSizeEPTMeas = sweep_param.EPT_Fcomp_Meas_Pwr_List.size();

      int hdetIdx = 0;
      int RxIdx[4] = {0};
      int RxDCIdx[4] = {0};
      int iSizeRxAction  = sweep_param.Rx_Actions_List.size();

      for (UINT iSegNum = 0; iSegNum < Sweep.size(); iSegNum++)
      {
         //***********************************************************//
         //    APT FreqComp power
         //***********************************************************//
         if (Sweep[iSegNum].txParams.txParams[0].pwrOverrideType == 1 && Sweep[iSegNum].txPurpose != 0)
         {
            if (iSizeAptPA > 0)
               Sweep[iSegNum].txRes.txPower = (word)dummyAPT_Power_Sweep4[AptPwrIdx/iSizeAptPA][AptPwrIdx%iSizeAptPA];
            AptPwrIdx++;
            if (bUseXMLresult && Sweep[iSegNum].txPurpose != 5)
            {
               RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
               txParams.Channel = static_cast<word>(Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan);
               txParams.PaState = Sweep[iSegNum].txParams.txParams[0].paState;
               txParams.RGI = Sweep[iSegNum].txParams.txParams[0].rgi;

               if (  sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
                     || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP
                     || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP )
               {
                  if (demo_param.bSimulationMode && AptFreqCompTxParamToFbAgc.size() > 0)
                  {
                     int fbAGC = AptFreqCompTxParamToFbAgc[txParams];
                     Sweep[iSegNum].rxRes.lnaOffset[1][0] = static_cast<word>(fbAGC);
                  }
               }
               else
               {
                  word power = static_cast<word>(AptFreqCompTxParamToPower[txParams]*10.0);
                  Sweep[iSegNum].txRes.txPower = power;
               }
            }
         }
         //***********************************************************//
         //    HDET
         //***********************************************************//
         if(demo_param.bSimulationMode)
         {
            if (Sweep[iSegNum].txMeasParams.txMeas[0].txMeasPayload.numHdetAvg > 0)
            {
               Sweep[iSegNum].txRes.hdet[0][0] = static_cast<word>(dummyHDET_Sweep4[hdetIdx]);
               hdetIdx++;
            }
         }

         //***********************************************************//
         //    APT FreqComp power - Tx Lim  - XML only
         //***********************************************************//
         if (Sweep[iSegNum].txParams.txParams[0].pwrOverrideType == 1 && Sweep[iSegNum].txPurpose != 0)
         {
            if (bUseXMLresult && Sweep[iSegNum].txPurpose == 5)
            {
               RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
               txParams.Channel = static_cast<word>(Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan);
               txParams.PaState = Sweep[iSegNum].txParams.txParams[0].paState;
               txParams.RGI = Sweep[iSegNum].txParams.txParams[0].rgi;
               word power = static_cast<word>(TxLimFreqCompTxParamToPower[txParams]*10.0);

               if (  sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
                     || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP
                     || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP )
               {
                  if (demo_param.bSimulationMode && TxLimFreqCompTxParamToFbAgc.size() > 0)
                  {
                     int fbAGC = TxLimFreqCompTxParamToFbAgc[txParams];
                     Sweep[iSegNum].rxRes.lnaOffset[1][0] = static_cast<word>(fbAGC);
                  }
               }
               else
               {
                  Sweep[iSegNum].txRes.txPower = power;
               }
               if(demo_param.bSimulationMode)
               {
                  Sweep[iSegNum].txRes.hdet[0][0] = static_cast<word>(TxLimFreqCompTxParamToHdet[txParams]);
               }
            }
         }
         //***********************************************************//
         //    XPT FreqComp power (exclude fudge segment)
         //***********************************************************//
         if (Sweep[iSegNum].txParams.txParams[0].pwrOverrideType == 2 && Sweep[iSegNum].txPurpose == 4)
         {
            if (bUseXMLresult)
            {
               RFCal_Helper_Tx_Params txParams (demo_param.TECH,demo_param.BAND);
               txParams.Channel = static_cast<word>(Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan);
               txParams.XPTmode = Sweep[iSegNum].txParams.txParams[0].xptMode;
               txParams.ExpPwr = static_cast<double>(Sweep[iSegNum].txParams.txParams[0].expPwr)/100.0;

               if (  sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP  )
               {
                  if (demo_param.bSimulationMode && XptFreqCompTxParamToFbAgc.size() > 0)
                  {
                     int fbAGC = XptFreqCompTxParamToFbAgc[txParams];
                     Sweep[iSegNum].rxRes.lnaOffset[1][0] = static_cast<word>(fbAGC);
                  }
               }
               else
               {
                  word power = static_cast<word>(XptFreqCompTxParamToPower[txParams]*10.0);
                  Sweep[iSegNum].txRes.txPower = power;
               }
            }
            else
            {
               if (iSizeETPwr > 0)
               {
                  Sweep[iSegNum].txRes.txPower = (word)dummyET_Power_Sweep4[EtPwrIdx/iSizeETPwr][EtPwrIdx%iSizeETPwr];
                  EtPwrIdx++;
               }

               if (iSizeEPTCalc > 0)
               {
                  Sweep[iSegNum].txRes.txPower = (word)dummyET_Power_Sweep4[EptCalcPwrIdx/iSizeEPTCalc][EptCalcPwrIdx%iSizeEPTCalc];
                  EptCalcPwrIdx++;
               }

               if (iSizeEPTMeas > 0)
               {
                  Sweep[iSegNum].txRes.txPower = (word)dummyET_Power_Sweep4[EptMeasPwrIdx/iSizeEPTMeas][EptMeasPwrIdx%iSizeEPTMeas];
                  EptMeasPwrIdx++;
               }
            }
         }

         if (  sweep_param.Sweep_Type == INT_CAL_XPT_FREQCOMP
               || sweep_param.Sweep_Type == INT_CAL_APT_FREQCOMP
               || sweep_param.Sweep_Type == INT_CAL_MAX_BIAS_FREQCOMP )
         {
            continue;
         }

         //***********************************************************//
         //    Rx
         //***********************************************************//
         for (int iRxPath=0; iRxPath<4; iRxPath++)
         {
            if (Sweep[iSegNum].rxMeas.actions[iRxPath].rx_meas_action == 1) // Regular Rx cal
            {
               Sweep[iSegNum].rxRes.lnaOffset[iRxPath][0] = dummyRxRes_LNAOffset[iRxPath][RxIdx[iRxPath]/iSizeRxAction][RxIdx[iRxPath]%iSizeRxAction];
               RxIdx[iRxPath]++;
               if (bUseXMLresult)
               {
                  RFCal_Helper_Rx_Params rxParams;
                  int LNAoffset;
                  rxParams.Channel = Sweep[iSegNum].radio_cfg.chan.chanInfo[0].ulChan;
                  rxParams.RxPath = iRxPath;
                  rxParams.LNARange = Sweep[iSegNum].rxMeas.actions[iRxPath].payloads[0].lna_state;
                  rxParams.RxPM = Sweep[iSegNum].rxMeas.actions[iRxPath].payloads[0].power_mode;

                  if (sweep_param.Sweep_Type == INTRACA_FCOMP)
                  {
                     LNAoffset = IntraCARxParamToLNAoffset[rxParams];
                  }
                  else
                  {
                     LNAoffset = FreqCompRxParamToLNAoffset[rxParams];
                  }
                  Sweep[iSegNum].rxRes.lnaOffset[iRxPath][0] = static_cast<word>(LNAoffset);
               }
            }
            if (Sweep[iSegNum].rxMeas.actions[iRxPath].rx_meas_action == 2) // DC cal use dummy result
            {
               Sweep[iSegNum].rxRes.lnaOffset[iRxPath][1] = dummyRxRes_LNAOffset[iRxPath][RxDCIdx[iRxPath]/iSizeRxAction][RxDCIdx[iRxPath]%iSizeRxAction];
               RxDCIdx[iRxPath]++;
            }
         }
      }
   }

   //***********************************************************//
   //    Sweep 5 Dummy Result Assignment // use dummy result
   //***********************************************************//

   if ( sweep_param.Sweep_Type == XPT_DELAY)
   {
      if(demo_param.bSimulationMode)
      {
         const int DelayRes[] = {1233, 1218, 1217, 1223, 1217, 1208, 1195, 1201, 1220, 1241, 1246, 1231, 1205, 1186, 1190, 1185};
         int UBChanIdx = 0;
         for (UINT i = 0; i < Sweep.size(); i ++)
         {
            if (Sweep[i].subSwpType == ESC_SUB_SWEEP_DELAY_PROC)
            {
               Sweep[i].dataRes.delayRes.delayBw = (demo_param.TECH == 3)?3:255;
               Sweep[i].dataRes.delayRes.delayChan = sweep_param.XPT_Delay_Swp_Upper_Bound_Channel_List[UBChanIdx];
               Sweep[i].dataRes.delayRes.delayVal  = (demo_param.TECH == 3)?DelayRes[UBChanIdx]:DelayRes[UBChanIdx]/5;
               UBChanIdx++;
            }
         }
      }
   }

    //***********************************************************//
    //      Sweep PIN Cal
    //***********************************************************//

    if (sweep_param.Sweep_Type == XPT_PIN_CAL)
    {
        if(demo_param.bSimulationMode)
        {
            const short PinCompensation[] = {-48, -43, -46, -41, -45, -42, -43, -44, -41, -45, -42, -48, -44, -41, -45, -40, -42, -43, -45, -41, -45, -44, -43, -45, -41, -47, -42, -43, -44, -41, -45, -40};
            
            const byte PeakCompensation[] = {42, 43, 44, 41, 45, 42, 43, 44, 41, 45, 42, 43, 44, 41, 45, 40, 42, 43, 44, 41, 45, 44, 43, 45, 41, 47, 42, 43, 44, 41, 45, 40};
            UINT iChanIdx = 0;
            for (UINT i = 0; i < Sweep.size(); i ++)
            {
                if (Sweep[i].swpType == XPT_PIN_CAL && Sweep[i].subSwpType == ESC_SUB_SWEEP_PIN_PROC)
                {
                    Sweep[i].dataRes.pinCalRes.isPinCalSuccessful = 1;
                    Sweep[i].dataRes.pinCalRes.pinChan = Sweep[i].data.pinValProc.currChannel;
                    Sweep[i].dataRes.pinCalRes.pinPeakCompression = PeakCompensation[(iChanIdx%32)];
                    Sweep[i].dataRes.pinCalRes.pinCompensation = PinCompensation[(iChanIdx%32)];
                    iChanIdx++;
                }
            }
        }
    }

}

unsigned char RFCal_Helper_Change_FTM_Mode(HANDLE hPhone, int count, int sleepTime)
{
   unsigned char isFTMmode = false;
   QLIB_IsFTM_Mode(hPhone,&isFTMmode);
   if (!isFTMmode)
   {
      QLIB_DIAG_CONTROL_F(hPhone, MODE_FTM_F);
      do
      {
         Sleep(sleepTime);
         QLIB_IsFTM_Mode(hPhone,&isFTMmode);
         count --;
      }
      while (!isFTMmode || count < 0);
   }
   return isFTMmode;
}

//used for disable all opcode and initial

void RFCal_Helper_AddMessageToQMSLLog(HANDLE hPhone, const string & Log, unsigned char bShowConsole)
{
   // Log.append("\n");   // if Execute this if you are using QMSL 6.0.36 or older version
   QLIB_AddUserMessageToTextLog(hPhone,LOG_FN,(char *)Log.c_str(), bShowConsole);
}

void RFCal_Helper_AddSectionTitleToQMSLLog(HANDLE hPhone, const string & Log, unsigned char bShowConsole)
{
   RFCal_Helper_AddMessageToQMSLLog(hPhone,"=========================================================================");
   QLIB_AddUserMessageToTextLog(hPhone,LOG_FN,(char *)Log.c_str(), bShowConsole);
   RFCal_Helper_AddMessageToQMSLLog(hPhone,"=========================================================================");
}

unsigned char RFCal_Helper_WriteNVToQCN(HANDLE hPhone, const string & filepath, unsigned char bWriteToDUT)
{
   //*********************************************//
   //    Write NV to a Phone
   //*********************************************//
   if (bWriteToDUT)
   {
      QMSL_RFCal_WriteToPhone_Status _status;
      unsigned char status_efs;
      string sFilePath = "/";
      QLIB_RFCAL_NV_Manager_WriteToPhone( hPhone,&_status);
      QLIB_EFS2_SyncWithWait(hPhone,(unsigned char*)(sFilePath.c_str()),1000,&status_efs);
   }

   //*********************************************//
   //    Write NV to a QCN
   //*********************************************//
   unsigned short nv_items_list_size;
   unsigned char  nv_write_to_QCN_status;

   ostringstream qcn_file_path;
   qcn_file_path << filepath;

    nv_items_list_size = 0;
    //nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_Copy_RFNV_Items_From_NV_Tool(hPhone, &numItemsCopied);

   unsigned short* backupNVStatus   = (unsigned short*) malloc(nv_items_list_size*sizeof(unsigned short));
   unsigned short* backupNVItemIDs  = (unsigned short*) malloc(nv_items_list_size*sizeof(unsigned short));

   if (backupNVStatus != NULL && backupNVItemIDs != NULL)
   {
      // Copy requested AUTO CAL NV Items to RFCAL NV Manger
    //  nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_BackupNVItems(hPhone, backupNVItemIDs, backupNVStatus, nv_items_list_size);
   }
   free(backupNVStatus);
   free(backupNVItemIDs);

   // Dump to QCN
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_Create_QCN( hPhone, (char*)qcn_file_path.str().c_str() );

   return true;
}

unsigned char RFCal_Helper_setOpCode_RxConfigParameters(cal_sweep_record& tmp, int LNA_state,byte rxFlag, byte rxPM,
    word expRxAGC, byte rx_meas_action, unsigned short rxCalType, unsigned short* pRxPath, unsigned short arraySize)
{
   tmp.measSeg = true;
   tmp.resExpected   = true;
   //*********************************************//
   //    OpCode 0x20: RxConfig
   //*********************************************//
   // Select the Rx action to be performed
   tmp.rxParams.rxActions                                = 0x1 << LNA_state;//Rx_Action;

   // Select device to use based on the device mask
   tmp.rxParams.rxFlag                                   = rxFlag;//3; //RxDeviceFlag;

   // 0 - Power Mode 0. 1 - Power Mode 1
   tmp.rxParams.rxPM                                  = rxPM;//0;//  = Rx_PM_List[RxPowerModeIndex];

   // RxAGC corresponding to the Rx Power Level
   tmp.rxParams.expRxAGC                                 = (word)expRxAGC; // 115 is min RSSI

    // Set RxPath
    for(size_t index = 0; index < arraySize; ++index)
   {
        tmp.rxParams.rxPath[index] = static_cast<byte>(pRxPath[index]);
   }

   //Set the rx Calibration Type. Used for Rx Consolidated NV generation
   tmp.rxParams.rxCalibrationType = rxCalType;

   //*********************************************//
   //    OpCode 0x4000: RxMeas
   //*********************************************//
   // RxMeas : Device Mask
   tmp.rxMeas.dev                                     = tmp.rxParams.rxFlag; //PRx + DRx
   {
      for (int i = 0, devMask = 1; i<8; i++, devMask <<= 1)
      {
         if (devMask & tmp.rxMeas.dev)
         {
            // Rx Meas Action : // Bit 0 - LNA Offset Measurement
            tmp.rxMeas.actions[i].rx_meas_action            =  rx_meas_action;//fixed right now.

            // The following payloads are required since Rx Meas Action bit 0 is enabled
            // LNA state for which the Rx Meas is to be performed
            tmp.rxMeas.actions[i].payloads[0].lna_state        =  LNA_state;

            // Analog transceiver power mode for which the Rx measurement is to be performed.
            tmp.rxMeas.actions[i].payloads[0].power_mode    =  tmp.rxParams.rxPM;

            // LNA Offset for computed by taking the difference between expected RxAGC and actual measured RxAGC the given LNA state and Power Mode
            tmp.rxMeas.actions[i].payloads[0].exp_rxagc        = tmp.rxParams.expRxAGC;
         }
      } // end of for (i = 0, devMask = 1; i<(
   }
   return true;
}
#define ROUNDL( d ) ((long)((d) + ((d) > 0 ? 0.5 : -0.5)))
unsigned char RFCal_Helper_CalculateUKernelParameters(double inputCharPwr,  double inputRefCalPwr, word inputRxScale, byte inputuBias, word & outputRxSCale, byte & outputuBias, double RefFBGain)
{
   const short minUbias = 0;

   const short maxUbias = 5;

   double deltaP = (inputRefCalPwr - inputCharPwr);

   double powerScaled =ROUNDL( deltaP/6.0 );
   RefFBGain = 0;
   if (RefFBGain != 0)
   {
      double targetFBGain = 6000;
      double newRxScale = 1/(pow(10.0,(-1.0*deltaP/20.0)))*targetFBGain/RefFBGain*(inputRxScale*pow(2.0,inputuBias));
      bool bFound = true;
      if (newRxScale < 8192)
      {
         outputRxSCale = static_cast<word>(newRxScale);
         outputuBias = 0;
      } 
      else if (newRxScale < 8192*2)
      {
         outputRxSCale = static_cast<word>(newRxScale/2);
         outputuBias = 1;
      } 
      else if (newRxScale < 8192*4)
      {
         outputRxSCale = static_cast<word>(newRxScale/4);
         outputuBias = 2;
      } 
      else if (newRxScale < 8192*8)
      {
         outputRxSCale = static_cast<word>(newRxScale/8);
         outputuBias = 3;
      } 
      else if (newRxScale < 8192*16)
      {
         outputRxSCale = static_cast<word>(newRxScale/16);
         outputuBias = 4;
      } 
      else if (newRxScale < 8192*32)
      {
         outputRxSCale = static_cast<word>(newRxScale/32);
         outputuBias = 5;
      } 
      else
      {
         bFound = false;
      }
      if (bFound)
      {
         return true;
      }
   }
   char tmpOutputuBias = static_cast<char>(inputuBias + static_cast<short>(powerScaled));

   if(tmpOutputuBias < 0)
   {
      outputRxSCale = static_cast<int>(inputRxScale * pow(10.0,(deltaP - 6*(minUbias - inputuBias))/20) );
      outputuBias = 0;
   }
   else if(tmpOutputuBias >= 0 && tmpOutputuBias <= 5)
   {
      outputRxSCale = static_cast<int>( inputRxScale * pow(10.0,(deltaP - 6*powerScaled)/20) );
      outputuBias = static_cast<byte>(tmpOutputuBias);
   }
   else if(tmpOutputuBias > 5)
   {
      outputRxSCale = static_cast<int>( inputRxScale * pow(10.0,(deltaP - 6*(maxUbias - inputuBias))/20) );
      outputuBias = 5;
   }
  
   if(tmpOutputuBias >= 5 && outputRxSCale >= 32767)
   {
     outputRxSCale = 32767;
      outputuBias = 5;
   }

   return true;
}
unsigned char RFCal_Helper_setOpCode_FBMeasurement(const Sweep_Params & sweep_param, cal_sweep_record & record, byte pa_state, byte rgi, byte refRGI, int overlapMode,   
   map < RFCal_Helper_Tx_Params, double > & APTV6CharToPwr,
   map < pair < dword, byte >, Tx_RefCal_Data_V2_Record > & ChanFBGainStateToPwrFBGainParams, double dEstimatedPwr)
{
   if (sweep_param.Fb_Rx_Gain_Cal_Mode != FBRX_UKERNEL_GAIN) { return true; }

   unsigned short fbGainStateIndex = 0;
    int fbGainState = sweep_param.FBGain_Cal_Gain_State_List.front() ; //setting initial FBGain State to 3
    
   double inputCharPwr = 0.0;
         
     if( rgi!=0)
   {
      if (dEstimatedPwr > -115.0) // If Estimated Power is given (known),
      {
         inputCharPwr = dEstimatedPwr;
      }
      else
      {
      // Try extended XPT char first.
      RFCal_Helper_Tx_Params txParam;
      txParam.Channel = sweep_param.Ref_Channel;
      byte devId = sweep_param.Device_Selection.Master_Device_ID;
      txParam.RGI = rgi;
      txParam.PaState = pa_state;
      txParam.Bias = record.txParams.txParams[devId].bias;
      txParam.PaQuiescentCurrent = record.txParams.txParams[devId].quiescentCurrent;
      if (record.txParams.txParams[devId].iqGainAction == 1)
      {
         txParam.IqGain = record.txParams.txParams[devId].iqGain;
      }
      if (record.txParams.txParams[devId].envScaleAction == 1)
      {
          txParam.EnvScale = record.txParams.txParams[devId].envScale;
      }
      txParam.delayVal = record.txParams.txParams[devId].delay;
      txParam.XPTmode = record.txParams.txParams[devId].xptMode;

      if (APTV6CharToPwr.find(txParam) != APTV6CharToPwr.end())
      {
         inputCharPwr = APTV6CharToPwr[txParam];
      }
      else // Traditional case
      {
         RFCal_Helper_Tx_Params txParam2;
         txParam2.Channel = sweep_param.Ref_Channel;
         txParam2.RGI = rgi;
         txParam2.PaState = pa_state;
      
         if (APTV6CharToPwr.find(txParam2) != APTV6CharToPwr.end())
         {
            inputCharPwr = APTV6CharToPwr[txParam2];
         }
      }

      }

      for(unsigned short fbGainStateSwptListitr=0; fbGainStateSwptListitr <sweep_param.FBGain_Cal_Gain_State_Swpt_List.size(); ++fbGainStateSwptListitr)
      {
         double FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld = 0.0;
        if (sweep_param.FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld_List.size() != 0)
        {
           FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld = sweep_param.FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld_List[fbGainStateSwptListitr]*overlapMode;
        }
        if( inputCharPwr < sweep_param.FBGain_Cal_Gain_State_Swpt_List[fbGainStateSwptListitr] + FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld)
        {
          fbGainStateIndex = fbGainStateSwptListitr;
          fbGainState = sweep_param.FBGain_Cal_Gain_State_List[fbGainStateSwptListitr];
        }
      }
   }
   
   Tx_RefCal_Data_V2_Record FBGainRefCalResult;

   if (ChanFBGainStateToPwrFBGainParams.find(make_pair(sweep_param.Ref_Channel,static_cast<byte>(fbGainState))) != ChanFBGainStateToPwrFBGainParams.end())
   {
      FBGainRefCalResult = ChanFBGainStateToPwrFBGainParams[make_pair(sweep_param.Ref_Channel,static_cast<byte>(fbGainState))];
   }
   word outputRxScale = 0;
   byte outputuBias = 0;
   RFCal_Helper_CalculateUKernelParameters(inputCharPwr,FBGainRefCalResult.pwr,FBGainRefCalResult.rxScale,FBGainRefCalResult.uBias,outputRxScale,outputuBias,FBGainRefCalResult.FBGain);

   //*********************************************//
   //      OpCode 0x8000:   FBGain 
   //*********************************************//
   byte debugMode = sweep_param.Enable_FBGain_Cal_Debug_Mode;
   byte txDevFlag = (1 << sweep_param.Device_Selection.Master_Device_ID);
   RFCal_Helper_setOpCode_txMeas_fbGainParams( record, txDevFlag, sweep_param.FBGain_Cal_XPTMode_List[fbGainStateIndex],
      sweep_param.FBGain_Cal_GainComp_Val, fbGainState, outputuBias, outputRxScale,debugMode);

   //*********************************************//
   //      OpCode 0x10000:   Add Ref RGI
   //*********************************************//
   unsigned char FBMode = 0;
   byte pDev = sweep_param.Device_Selection.Master_Device_ID;
   RFCal_Helper_ESC_ET_ConfigureFB(record, pDev, false,FBMode, sweep_param.Device_Selection.Feedback_Device_ID, fbGainState, pa_state,refRGI);

    //FillTxIntCalRecord( swpRecord, ESC_FEEDBACK_SETUP_ACTION_SKIP,
    //devParams.m_FeedbackDevice, devParams.m_FeedbackDevice,
    //refRGI, paState, 0, false );

   return true;
}
unsigned char RFCal_Helper_setOpCode_txMeas_fbGainParams( cal_sweep_record& tmp,    byte txDevice,    byte xptMode,    word gaincomp,    byte fbGainState,    byte uBias,    int rxScale,
   byte debugModeEn, byte bMeasFBGainTherm)
{
   tmp.txMeasParams.txMeasFlag = static_cast<byte>(txDevice);
   tmp.resExpected = true;

   tmp.txIntCalParams.fbRxCalMode = FBRX_UKERNEL_GAIN;

   int dev = 0;

   for( byte dev_mask = 0x1; dev < MAX_NUM_DEVICES; ++dev, dev_mask <<= 0x1 )
   {
      if( txDevice & dev_mask )
      {
         tmp.txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.xptMode = static_cast<byte>(xptMode);
         tmp.txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.gainComp = static_cast<byte>(gaincomp);
         tmp.txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState = static_cast<byte>(fbGainState);
         tmp.txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias = static_cast<byte>(uBias);
         tmp.txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale = static_cast<word>(rxScale);
         tmp.txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.debugModeEn = static_cast<byte>(debugModeEn);

         tmp.txMeasParams.txMeas[dev].txMeasAction |= ESC_TX_MEAS_ACTION_FB_GAIN_MEAS;

         if ( bMeasFBGainTherm )
         {
            tmp.txMeasParams.txMeas[dev].txMeasAction |= ESC_TX_MEAS_ACTION_THERM_READ;
         }
      }
   }
   return true;
}

unsigned char RFCal_Helper_setOpCode_FB_Setup(cal_sweep_record& tmp, byte PrimaryDev, byte fbDev, word fbAction, byte fbEnable, byte fbGainState, byte paState, byte refRGI) 
{
   //*********************************************//
   //    OpCode 0x10000: FBSetup
   //*********************************************//
   tmp.txIntCalParams.debug_mode = true;
   tmp.txIntCalParams.pdev = PrimaryDev;
   tmp.txIntCalParams.fbDev = fbDev;
   tmp.txIntCalParams.fbAction = fbAction;
   if (fbAction & 0x1)
   {
      tmp.txIntCalParams.fbEnable   = fbEnable;
      tmp.txIntCalParams.fbGainState = fbGainState;
   }
   if (fbAction & 0x2)
   {
      tmp.txIntCalParams.rxlmXptEnable = fbEnable;
   }
   if (fbAction & 0x4)
   {
      tmp.txIntCalParams.rxlmFbAgcEnable = fbEnable;
   }

   if (paState != 255)
   {
      tmp.txIntCalParams.fbGainState = fbGainState;
      tmp.txIntCalParams.paState = paState;
      tmp.txIntCalParams.refRGI = refRGI;    // to calculate offset
   }

   return true;
}
unsigned char RFCal_Helper_setOpCode_TxOverride(cal_sweep_record& tmp, byte TxPurpose, byte TxDeviceFlag, byte xptMode, dword txOverrideMask, byte pwrOverrideType, dword pwrParamMask,
                                                byte PA_State, byte RGI, dword BIAS, byte IQ_Gain_Action, word IQ_Gain, byte Env_Scale_Action,word Env_Scale, word PA_Quiescent_Current, byte delayAction, byte delayValue,
                                                word PwrdBm, dword refChan, word dpdIndex)
{
   tmp.txPurpose = TxPurpose;
   //*********************************************//
   //    OpCode 0x200: TxOverride
   //*********************************************//

   // Tx Override: Device Mask
   tmp.txParams.txDevice = TxDeviceFlag;

   // Tx configuration for RFM_DEVICE_0 requires the following fields.
   // - txParams[0] indicates configuration for RFM_DEVICE_0.

      int dev = 0;   

      // Get Dev ID from Dev bit mask
      dev = static_cast<int>(RFCal_Helper_GetIndexFromBitMask(TxDeviceFlag));

      // XPT mode for RFM_DEVICE_[dev]
   tmp.txParams.txParams[dev].xptMode = xptMode;

   // Tx Override type mask
   tmp.txParams.txParams[dev].txOverrideMask = txOverrideMask;// ESC_TX_OVERRIDE_MASK_POWER;    // other option is DPD

   if (tmp.txParams.txParams[dev].txOverrideMask & ESC_TX_OVERRIDE_MASK_POWER)
   {
         // Tx Override Type Mask for RFM_DEVICE_[dev]
      // Power override Type (byte)
      tmp.txParams.txParams[dev].pwrOverrideType = pwrOverrideType; // ESC_TX_PWR_OVERRIDE_PARAMS  ESC_TX_PWR_OVERRIDE_ABS_PWR

      if (tmp.txParams.txParams[dev].pwrOverrideType == ESC_TX_PWR_OVERRIDE_PARAMS)
      {
         // Power parameter Mask  TypeAdd Delay
         tmp.txParams.txParams[dev].pwrParamMask = pwrParamMask; // Delaty is for Sweep 1 only
         // Bit 0 Payload (PA state)
         tmp.txParams.txParams[dev].paState = (byte)PA_State;
         // Bit 1 Payload (RGI)
         tmp.txParams.txParams[dev].rgi = (byte)RGI;//RGI_START_List[paIndex]+rgiIndex*RGI_STEP_LIST[paIndex];
         // Bit 2 Payload (Bias)
         tmp.txParams.txParams[dev].bias = (dword)BIAS; //full bias default value
         // Bit 3 Payload (IQ Gain)
         tmp.txParams.txParams[dev].iqGainAction = IQ_Gain_Action;
         tmp.txParams.txParams[dev].iqGain = (word)IQ_Gain;   // From param XML
         // Bit 4 Payload (Envelope Scale)
         tmp.txParams.txParams[dev].envScaleAction = Env_Scale_Action;
         tmp.txParams.txParams[dev].envScale = (word)Env_Scale;  // From param XML
         // Bit 5 Payload (Q Current)
         tmp.txParams.txParams[dev].quiescentCurrent = (word)PA_Quiescent_Current;   // From param XML
         // Bit 6 Payload (Delay)
                tmp.txParams.txParams[dev].delayAction  = delayAction;
         tmp.txParams.txParams[dev].delay =  delayValue;  // delayAction = ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV.
      } // end of if (tmp.txParams.txParams[dev].pwrOverrideType == 1)
      else if (tmp.txParams.txParams[dev].pwrOverrideType == ESC_TX_PWR_OVERRIDE_ABS_PWR )
      {
         tmp.txParams.txParams[dev].expPwr = PwrdBm;
         tmp.txParams.txParams[dev].refChan = refChan;
      }
   } // end of if (tmp.txParams.txParams[dev].txOverrideMask & ESC_TX_OVERRIDE_MASK_POWER)

   if (tmp.txParams.txParams[dev].txOverrideMask & ESC_TX_OVERRIDE_MASK_DPD )
   {
      tmp.txParams.txParams[dev].dpdIndex = dpdIndex;
   }

   return true;
}

unsigned char RFCal_Helper_setOpCode_RadioSetup(cal_sweep_record& tmp, byte masterDev, byte slaveDev, byte masterDevState, byte slaveDevState, byte TECH, word BAND, dword ulChan,  byte txWaveform, dword offset, byte numRBs, byte startRBs, word eulConfig, dword RxBandwidth, dword TxBandwidth, unsigned char flag)
{
   //*********************************************//
   //    OpCode 0x100: RadioSetup
   //*********************************************//

   tmp.isCfgSeg = true;       // Radio Setup
   tmp.radio_cfg.device.masterDev = masterDev;
   tmp.radio_cfg.device.slaveDev = slaveDev;
   tmp.radio_cfg.device.masterDevState = masterDevState;          // Enable tx,rx
   tmp.radio_cfg.device.slaveDevState = slaveDevState;            // NO_CHANGE

   tmp.radio_cfg.tech = TECH;
   tmp.radio_cfg.band = BAND;

   tmp.radio_cfg.flag = flag;       // NBEE on and Int cal

   tmp.radio_cfg.chan.numChans   = 1;

   tmp.radio_cfg.chan.chanInfo[0].ulChan = ulChan;
   tmp.radio_cfg.chan.chanInfo[0].rxBW = RxBandwidth;
   tmp.radio_cfg.chan.chanInfo[0].txBW = TxBandwidth;

   tmp.radio_cfg.wfm.txWaveform = txWaveform;

   if (tmp.radio_cfg.wfm.txWaveform == 2)
   {
      tmp.radio_cfg.wfm.cwOffset = offset;
   }
   else if (tmp.radio_cfg.wfm.txWaveform    == 3)
   {
      tmp.radio_cfg.wfm.numRBs = numRBs;
      tmp.radio_cfg.wfm.startRB = startRBs;
   }
   else if (tmp.radio_cfg.wfm.txWaveform    == 4)
   {
      tmp.radio_cfg.wfm.eulConfig   = eulConfig;
   }

   return true;
}

unsigned char RFCal_Helper_setOpCode_TxMeasure(cal_sweep_record& tmp, byte txDevice, word txMeasAction, byte numHdetAvg, byte numLpmHdetAvg, byte xptMode, 
    word gainComp, byte fbGainState, byte uBias, word rxScale, byte debugModeEn, byte  numSamples)
{
   //*********************************************//
   //    OpCode 0x8000: TxMeasure
   //*********************************************//
   tmp.measSeg = true;
   tmp.resExpected   = true;     // Set resExpected = true when HDET measurement is performed.
   // TxMeas. Device Mask
   tmp.txMeasParams.txMeasFlag   = txDevice;

      tmp.txParams.txDevice = txDevice;
   //Tx Meas Action. The field below is present since [Device Mask] & 0x1 = 0x1
        byte dev = 0;
        for( byte dev_mask = 0x1; dev < MAX_NUM_DEVICES; ++dev, dev_mask <<= 0x1 )
        {
          if( txDevice & dev_mask )
          {
            tmp.txMeasParams.txMeas[dev].txMeasAction   = txMeasAction;

   // 1 indicates HDET Measurement
        
            if (tmp.txMeasParams.txMeas[dev].txMeasAction & ESC_TX_MEAS_ACTION_HDET_MEAS)
            {
                tmp.txMeasParams.txMeas[dev].txMeasPayload.numHdetAvg = numHdetAvg ;
            }
            if (tmp.txMeasParams.txMeas[dev].txMeasAction & ESC_TX_MEAS_ACTION_LPM_HDET_MEAS)
            {
                tmp.txMeasParams.txMeas[dev].txMeasPayload.numLpmHdetAvg = numLpmHdetAvg ;
            }
            if (tmp.txMeasParams.txMeas[dev].txMeasAction & ESC_TX_MEAS_ACTION_FB_GAIN_MEAS)
            {
                tmp.txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.xptMode = xptMode;

                tmp.txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.gainComp = gainComp;

                tmp.txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.fbGainState = fbGainState;

                tmp.txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.uBias = uBias;

                tmp.txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.rxScale = rxScale;

                tmp.txMeasParams.txMeas[dev].txMeasPayload.fbGainParams.debugModeEn = debugModeEn;
            }
            // 16 = FB Power Measurement 
            if (tmp.txMeasParams.txMeas[dev].txMeasAction & ESC_TX_MEAS_ACTION_FB_POWER_MEAS )
            {
                tmp.txMeasParams.txMeas[dev].txMeasPayload.fbPowerMeasParams.fbGainState = fbGainState;

                tmp.txMeasParams.txMeas[dev].txMeasPayload.fbPowerMeasParams.numSamples = numSamples;
            }
          }
        } // for( byte dev_mask = 0x1; dev < MAX_NUM_DEVICES; ++dev, dev_mask <<= 0x1 )

   return true;
}

unsigned char RFCal_Helper_setOpCode_XPT2(cal_sweep_record& tmp, dword xptAction, byte cfg_dev, byte cfg_fbDev, byte cfg_xptMode,
                                          byte capType, word numSamples, byte firstTrig, byte lastTrig, byte procFlag)
{
   //*********************************************//
   //    OpCode 0x2000: XPT2
   //*********************************************//
   tmp.resExpected = true;
   tmp.txPurpose = (word)0;
   tmp.xpt.xptAction = xptAction;

   if (  tmp.xpt.xptAction & ESC_XPT_ACTION_CAL_CFG)  // 1 (i.e., Bit 0 CAL_CONFIG to init XPT action)
   {
      /* XPT Action bit 0 enabled (CAL_CONFIG) requires the following fields */
      /* Primary Device */
      tmp.xpt.xpt_cfg.dev = cfg_dev;
      /* Feedback Device */
      tmp.xpt.xpt_cfg.fbDev = cfg_fbDev;// feedback chain
      /* XPT Mode 0 - APT | 1 - EPT | 2 - ET */
      tmp.xpt.xpt_cfg.xptMode = cfg_xptMode; //for 1st sweep always use EPT, it's fixed
   }
   if (  tmp.xpt.xptAction & ESC_XPT_ACTION_IQ_CAP)   // 2 (i.e., Bit 1 enabled)
   {
      // XPT Action bit 1 enabled (IQ_CAPTURE) requires the following fields

      // Primary device
      tmp.xpt.xptCaptureIQ.dev = cfg_dev;
      // Feedback device
      tmp.xpt.xptCaptureIQ.fbDev = cfg_fbDev;
      // Capture Type
      tmp.xpt.xptCaptureIQ.capType = capType; // 0 - EPT IQ Capture
      // Num of Samples
      tmp.xpt.xptCaptureIQ.numSamples = numSamples;
      // Flag indicating the frst IQ Capture
      tmp.xpt.xptCaptureIQ.firstTrig = firstTrig;
      // Flag indicating the last IQ Capture
      tmp.xpt.xptCaptureIQ.lastTrig = lastTrig;
      // Flag indicating the type of processing to be done
      tmp.xpt.xptCaptureIQ.procFlag = procFlag;
   }
   return true;
}

unsigned char RFCal_Helper_setOpCode_Sync(cal_sweep_record& tmp, byte txDeviceId)
{
   //*********************************************//
   //    OpCode 0x1000: Sync
   //*********************************************//

   tmp.subSwpType = ESC_SUB_SWEEP_MAX;
   tmp.sync.syncEvent = ESC_SYNC_EVENT_SYNC_WITH_SYS_TX_BDRY;
   tmp.sync.device   = txDeviceId;
   return true;
}
unsigned char RFCal_Helper_setLowRGISegment(cal_sweep_record& tmp, byte TxDeviceFlag)
{
   //*********************************************//
   //    Set Low RGI instead of tear down
   //*********************************************//
   tmp.txPurpose  = (word)0;
   tmp.subSwpType = ESC_SUB_SWEEP_DEFAULT;
   //*********************************************//
   //    OpCode 0x200 (bit 9 enabled): TxOverride
   //*********************************************//
   int LowRgitxParamMask = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_PA_CURRENT);
   RFCal_Helper_setOpCode_TxOverride(tmp, -1,TxDeviceFlag, XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,     ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,        LowRgitxParamMask  /* pwrParamMask */,
                                     0 /* PA state */, 0 /* RGI */,   0,0,0,   0, 0, 0, 0, 0);
   tmp.measSeg = false;
   return true;
}
unsigned char RFCal_Helper_setOpcode(HANDLE hPhone, vector< cal_sweep_record > & Sweep, int recordIndex)
{
   //*********************************************//
   //    Generate OpCode for all the segment
   //*********************************************//
   unsigned char _bOk = true;
   for (int iLoop = 0; iLoop <   recordIndex; iLoop++)
   {
      QLIB_FTM_GENERATE_ESC_OPCODE(NULL, (void *)&Sweep[iLoop]);  // NULL handler makes a little faster.
      if(!Sweep[iLoop].isCfgSeg && iLoop > 0)
      {
         Sweep[iLoop].radio_cfg = Sweep[iLoop-1].radio_cfg;
      }
   }
   return _bOk;
}
unsigned char RFCal_Helper_GetEscSweepResults(HANDLE hPhone, vector< cal_sweep_record > & Sweep, unsigned char bCompressedResponse, unsigned char bCallBoxMeasurement)
{
   //*********************************************//
   //    QLIB_FTM_GET_ESC_SWEEP_RESULTS
   //*********************************************//
   unsigned __int64  err            = 0;  // For MSVC6. VS2010. Unsigned long long is available
   unsigned short err_segment       = 0;
   unsigned char bOK = false;
    
   if (!bCallBoxMeasurement)
   {
   dword iWaitTime = 0;
   for (UINT i=0; i < Sweep.size() ; i++)
   {
      iWaitTime += static_cast<dword>(Sweep[i].opcode.segLen/1000) ;
   }
      Sleep(iWaitTime + 200); // Wait for the completion of measurements
    }
   unsigned char bFound = false;
   for (UINT i=0; i < Sweep.size() ; i++)
   {
      if (Sweep[i].resExpected == true)
      {
         bFound = true;
         break;
      }
   }
   if (!bFound)
   {
      return true;  // E.g., APT only case, Sweep 2 has no response, then we should not execute get sweep result.
   }
   if (bCompressedResponse)
   {
        bOK = QLIB_FTM_GET_ESC_SWEEP_RESULTS_COMPRESSED(hPhone, (void*)(&Sweep[0]),(unsigned long)Sweep.size(),&err,&err_segment);
   }
   else
   {
       bOK = QLIB_FTM_GET_ESC_SWEEP_RESULTS(hPhone, (void*)(&Sweep[0]),(unsigned long)Sweep.size(),&err,&err_segment);
   }
   return bOK;
}
unsigned char RFCal_Helper_ExecEscSweep(HANDLE hPhone, const vector< cal_sweep_record > & Sweep, int recordIndex, bool bIsLastSweep, unsigned char bCompressed, unsigned char  bDebug)
{
   //*********************************************//
   //    Execute Sweep  (Sending OpCode to DUT)
   //*********************************************//
   unsigned char _bOk;
   bool _bDebug = (bDebug == 1)?true:false;
   if (bCompressed)
    
   {
      _bOk = QLIB_FTM_CFG_AND_EXEC_ESC_SWEEP_COMPRESSED(hPhone, (void*)(&Sweep[0]),recordIndex, bIsLastSweep , _bDebug);
   }
   else
   {
      _bOk = QLIB_FTM_CFG_AND_EXEC_ESC_SWEEP(hPhone, (void*)(&Sweep[0]),recordIndex, bIsLastSweep, _bDebug);
   }
   // if (!_bOk) return false;
   // In the case of failure, you can do measurement again. Please change Retune for 0x100 RadioSetup

   return static_cast<byte>(_bOk);
}

unsigned char RFCal_helper_setOpCodeData_LoadCalcDPD(cal_sweep_record& tmp,const Cal_Param & cal_param,const Sweep_Params & sweep_param, int TECH, int BAND, unsigned char bEPT)
{
   //*********************************************//
   //    OpCode 0x800: DATA LoadCalcDPD
   //*********************************************//
   int Swp2 = -1;
   for (UINT index = 0; index < cal_param.Swp.size(); index++)
   {
      if (cal_param.Swp[index].Sweep_Type == XPT_SWP2 || cal_param.Swp[index].Sweep_Type == INT_CAL_XPT_SWP2)
      {
         Swp2 = index;
         break;
      }
   }
   if (Swp2 == -1)
      throw RFCal_Helper_Exception("XPT_SWP2 index not found.",  __FILE__, __LINE__ );

   //const  = 1;
   const double QPOET_VMIN = 0.6; // hard coding in subsysrfcal (As of now, XML does not define this parameter)
   const double QPOET_VMAX = 3.8;   // hard coding in subsysrfcal (As of now, XML does not define this parameter)

   // Enable Bit 5 - Load Calc DPD Tables
   tmp.data.loadCalcDpd.do_load_calc_dpd_op = true;// This field is required.

   // Enable Bit 5 - Load Calc DPD Tables requires the following fields
   // Load Action for Constructed DPDs. 0 - Load Unity DPD | 1 - Load Meas DPD
   if (sweep_param.Sweep_Type == XPT_SWP2)
   {
      tmp.data.loadCalcDpd.loadAction     = LOAD_CALC_DPD_ACTION_LOAD_UNITY;  //  0 - Load Unity DPD
   }
   else if (sweep_param.Sweep_Type == XPT_SWP3 ||sweep_param.Sweep_Type == XPT_FREQCOMP || sweep_param.Sweep_Type == XPT_FREQCOMP_STAGGER_RXTX )   // Sweep XPT 3 or XPT_FreqComp
   {
      tmp.data.loadCalcDpd.loadAction     = LOAD_CALC_DPD_ACTION_LOAD_ISC; //  0 - Load Unity DPD
   }
   // XPT Mode for Load Calc DPD Operation
   tmp.data.loadCalcDpd.xptMode        = (bEPT)?XPT_TYPE_EPT:XPT_TYPE_ET;

   // QPOET Min Volt. Min QPOET Voltage for Scaling Detrough
   tmp.data.loadCalcDpd.qpoetMinV      = static_cast<word>(QPOET_VMIN*100.0);

   // QPOET Max Volt. Max QPOET Voltage for Scaling Detrough
   tmp.data.loadCalcDpd.qpoetMaxV      = static_cast<word>(QPOET_VMAX*100.0);

   // Min ET Voltage for Scaling Detrough
   tmp.data.loadCalcDpd.etMinV         = static_cast<word>(cal_param.PA_Param.EtVmin *100.0) ;

   // Number of Power Values (calculated) for DPD Construction Sweep to find the RGI and Bias from M-Line
   tmp.data.loadCalcDpd.numSwp2CalcPwrs = (byte)((bEPT)?cal_param.Swp[Swp2].EPT_Swp2_Calc_Pwr_List.size() : 0);
   if (sweep_param.Sweep_Type == XPT_FREQCOMP || sweep_param.Sweep_Type == XPT_FREQCOMP_STAGGER_RXTX)
   {
      tmp.data.loadCalcDpd.numSwp2CalcPwrs = 0;
   }
   // Number of Power Values (measured) for DPD Construction Sweep to find the RGI and Bias from M-Line
   tmp.data.loadCalcDpd.numSwp2MeasPwrs = (byte)((bEPT)?cal_param.Swp[Swp2].EPT_Swp2_Meas_Pwr_List.size()  : cal_param.Swp[Swp2].ET_Swp2_Meas_Pwr_List.size() );
   for(int index = 0; index < tmp.data.loadCalcDpd.numSwp2CalcPwrs ; index++)
   {
      // Requested (calculated) power for Swp 2 DPD Construction
      tmp.data.loadCalcDpd.calcPwrSwp2DB100[index] =  (bEPT)? static_cast<word>(cal_param.Swp[Swp2].EPT_Swp2_Calc_Pwr_List[index]*100.0) : 0;
   }

   for(int index = 0; index < tmp.data.loadCalcDpd.numSwp2MeasPwrs ; index++)
   {
      // Requested (measured) power for Swp 2 DPD Construction
      tmp.data.loadCalcDpd.measPwrSwp2DB100[index] = (bEPT)? static_cast<word>(cal_param.Swp[Swp2].EPT_Swp2_Meas_Pwr_List[index]*100.0) : static_cast<word>(cal_param.Swp[Swp2].ET_Swp2_Meas_Pwr_List[index]*100.0);
   }

   return true;
}
unsigned char RFCal_helper_setOpCodeData_UnityDPD(cal_sweep_record& tmp,const Cal_Param & cal_param, const Sweep_Params & sweep_param, int TECH, int BAND, unsigned char bEPT)
{
   //*********************************************//
   //    OpCode 0x800: DATA UnityDPD
   //*********************************************//

   // RFCal_Helper_ESC_ET_PA_PARAM_Assignment_XML function assings qpoetVdMin and qpoetVdMax by executing QLIB_FTM_GET_RFFE_DEVICE_INFO. 
   const double QPOET_VMIN = cal_param.PA_Param.qpoetVdMin; 
   const double QPOET_VMAX = cal_param.PA_Param.qpoetVdMax;   

   // DETROUGH TABLE   read from RFCAL_PA_PARAM.xml

   tmp.data.nomDPDTbl.doDPDLoadOp   = true;  // Enables Bit 1 - Load Nominal DPD Table on dataAction
   // This assignement is for QLIB_FTM_CFG_AND_EXEC_ESC_SWEEP
   tmp.data.dataAction              = DATA_OPCODE_ACTION_LOAD_DPD;

   /* Bit 1 - Load Nominal DPD Table requires the following fields */
   // XPT Mode
    tmp.data.nomDPDTbl.xptType      = LOAD_DPD_XPT_TYPE_ET; // 0=EPT | 1=ET. This is ET use

   // Number of AMAM, AMPM and Detrough
   int numAMAM = 64;
   int numAMPM = 64;

   tmp.data.nomDPDTbl.numAMAM    = numAMAM ;
   tmp.data.nomDPDTbl.numAMPM    = numAMPM ;

   // Nominal DPD file will be read from xml later
   for(int index = 0; index < tmp.data.nomDPDTbl.numAMAM;   index++)
   {
      unsigned int amamVal = ( index << 10 ) + 0x400;
      if( amamVal > 0xFFFF )
         amamVal = 0xFFFF;
      tmp.data.nomDPDTbl.AMAM[index] = amamVal; /// 1024, 2048, 3072, 4096 .....
      tmp.data.nomDPDTbl.AMPM[index] = 0;
   }

   tmp.data.nomDPDTbl.numDET           = cal_param.PA_Param.Detrough_table.size();

    if (sweep_param.Sweep_Type == XPT_SWP1 
      || sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 
      || sweep_param.Sweep_Type == XPT_PIN_CAL )
   {
      for(int i = 0; i < tmp.data.nomDPDTbl.numDET; i++)
      {
         tmp.data.nomDPDTbl.DET[i]     = cal_param.PA_Param.Detrough_table[i];
      }
   }
   else if (sweep_param.Sweep_Type == XPT_DELAY)
   {
      const double POW_2_15 = pow( 2.0, 15 ); // 32768
      const double Vdmax_mline   = sweep_param.XPT_Delay_Swp_Bias/1000.0;
      const double Vdmin_et      = cal_param.PA_Param.EtVmin;

      // Scale the detrough before loading
      double dFloor = (Vdmin_et-QPOET_VMIN)/(QPOET_VMAX-QPOET_VMIN)*POW_2_15+0.5;
      for(int index = 0; index < tmp.data.nomDPDTbl.numDET; index++)
      {
         // Calculate detrough entry to achieve Vd_mline range
         tmp.data.nomDPDTbl.DET[index] =  (unsigned short) floor(
                                             ((Vdmax_mline-Vdmin_et)/(QPOET_VMAX-QPOET_VMIN)*((word)cal_param.PA_Param.Detrough_table[index])
                                              +  dFloor));
      }
   }

   return true;
}
unsigned char RFCal_Helper_setOpCodeData(cal_sweep_record& tmp, const Cal_Param & cal_param, const Sweep_Params & sweep_param, int iDataOption,int TECH, int BAND, unsigned char bEPT)
{
   //*********************************************//
   //    OpCode 0x800: DATA
   //*********************************************//

   tmp.resExpected               = true;
   tmp.txPurpose              = (word)0;

   tmp.data.tech              = TECH;
   tmp.data.refChan           = sweep_param.Ref_Channel;

   tmp.data.dev               = sweep_param.Device_Selection.Tx_Device_To_Calibrate[0];

   tmp.data.doDataOp          = true;

   switch(iDataOption)
   {
      case DATA_OPCODE_ACTION_LOAD_DPD:
         RFCal_helper_setOpCodeData_UnityDPD(tmp, cal_param, sweep_param, TECH, BAND);
         break;
      case DATA_OPCODE_ACTION_LOAD_CALC_DPD:
         RFCal_helper_setOpCodeData_LoadCalcDPD(tmp, cal_param, sweep_param, TECH, BAND, bEPT);
         break;
      case DATA_OPCODE_ACTION_DELAY_PROC:
         tmp.subSwpType             = ESC_SUB_SWEEP_DELAY_PROC;
         tmp.data.delayValProc.doDelayValProcOp = true; // this assignment is not required.
         break;
      case DATA_OPCODE_ACTION_GET_MLINE_RES:
      default:
         break;
   }
   return true;
}
unsigned char RFCal_helper_Log_Build_ID(HANDLE hPhone)
{
   string stTmp;
   return RFCal_helper_Log_Build_ID(hPhone,stTmp);
}

unsigned char RFCal_helper_Log_Build_ID(HANDLE hPhone, string & stBuildID)
{
   unsigned long iMSM_HW_Version= 0;
   unsigned long iMobModel = 0;
   char sMobSwRev[128] = {0};
   char sModelStr[128] = {0};
   stBuildID = "";
   unsigned char bOK = QLIB_DIAG_EXT_BUILD_ID_F ( hPhone, &iMSM_HW_Version, &iMobModel, sMobSwRev, sModelStr );
   if (bOK)
   {
      stBuildID.assign(sMobSwRev);
   }
   return bOK;
}

template<typename T> unsigned char RFCal_Helper_Convert_Str_To_Value(const string & stValue, T & value)
{
   if (stValue.compare("TRUE") == 0  || stValue.compare("true") == 0 )
   {
      value = true;
      return true;
   }
   if (stValue.compare("FALSE") == 0 || stValue.compare("false") == 0 )
   {
      value = false;
      return true;
   }

   if ( typeid(T) == typeid(double) || typeid(T) == typeid(float) )
   {
      istringstream (stValue) >> value;
   }
   else
   {
      long lTmp = 0;
      istringstream (stValue) >> lTmp;
      value = static_cast<T>(lTmp);
   }
   return true;
}

unsigned char RFCal_Helper_Convert_Str_To_Value(const string & stValue, bool & value)
{
   if (stValue.compare("TRUE") == 0  || stValue.compare("true") == 0 )
   {
      value = true;
      return true;
   }
   if (stValue.compare("FALSE") == 0 || stValue.compare("false") == 0 )
   {
      value = false;
      return true;
   }

   long lTmp;
   istringstream (stValue) >> lTmp;
   value = (lTmp != 0);

   return true;
}
string RFCal_Helper_Convert_BSTR_to_String(BSTR & bst)
{
   wstring ws = (wstring)bst;
   string tmpSt(ws.begin(),ws.end());
   return tmpSt;
}
template<typename T> unsigned char RFCal_Helper_Read_Param(MSXML2::IXMLDOMNodePtr parentNode,const string & XPath, vector < vector <T> > & parameter)
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
   BSTR text;
   pParentElement->get_text(&text);

   //**************************************************//
   //       Split the text a vector < string >
   //       Delimiter is ';'. If not, try ','
   //**************************************************//
   wstring ws = (wstring)text;
   string tmpSt(ws.begin(),ws.end());
   stringstream  ss(tmpSt);
   string item;
   vector <string> elems;
   while (std::getline(ss, item, ';'))
   {
      elems.push_back(item);
   }
   if (elems.size() == 1)
   {
      elems.clear();
      string tmpSt(ws.begin(),ws.end());
      stringstream  ss(tmpSt);
      string item;

      while (std::getline(ss, item, ','))
      {
         elems.push_back(item);
      }
   }
   parameter.clear();
   parameter.resize(elems.size());

   //**************************************************//
   //       Convert each of string to T type
   //**************************************************//
   for (UINT i=0; i < elems.size(); i++)
   {
      if ((elems[i].length() > 7) && (elems[i].substr(0,3).compare("SSS") == 0))
      {
         stringstream value (elems[i].substr(4,elems[i].length()));
         string strStartRGI;
         string strEndRGI;
         string strStepRGI;
         std::getline(value,strStartRGI,',');
         std::getline(value,strEndRGI,',');
         std::getline(value,strStepRGI,')');
         int tmpVal[3];
         RFCal_Helper_Convert_Str_To_Value(strStartRGI, tmpVal[0]);
         RFCal_Helper_Convert_Str_To_Value(strEndRGI, tmpVal[1]);
         RFCal_Helper_Convert_Str_To_Value(strStepRGI, tmpVal[2]);
         for (int index = tmpVal[0]; index >= tmpVal[1]; index += tmpVal[2])
         {
            parameter[i].push_back(static_cast<T>(index));
         }
      }
      else if ((elems[i].length() > 6) && (elems[i].substr(0,2).compare("MC") == 0))
      {
         stringstream value (elems[i].substr(3,elems[i].length()));
         string strValue;
         string strNumRepeats;
         std::getline(value,strValue,',');
         std::getline(value,strNumRepeats,')');
         int tmpVal[2];
         RFCal_Helper_Convert_Str_To_Value(strValue, tmpVal[0]);
         RFCal_Helper_Convert_Str_To_Value(strNumRepeats, tmpVal[1]);

         for (int index = 0; index < tmpVal[1]; index ++)
         {
            parameter[i].push_back(static_cast<T>( tmpVal[0] ));
         }
      }
      else
      {
            vector <string> sub_elems;
            stringstream  sub_ss(elems[i]);
            string sub_item;
        
            while (std::getline(sub_ss, sub_item, ','))
            {
                sub_elems.push_back(sub_item);
            }
            for (UINT j=0; j < sub_elems.size(); j++)
            {
         T tmpVal;
            //  RFCal_Helper_Convert_Str_To_Value(elems[i], tmpVal);
                RFCal_Helper_Convert_Str_To_Value(sub_elems[j], tmpVal);
                
         parameter[i].push_back(static_cast<T>( tmpVal ));
      }
   }
    }
   pXPathNode.Release();
   return true;
}
template<typename T> unsigned char RFCal_Helper_Read_Param(MSXML2::IXMLDOMNodePtr parentNode, const string & XPath, vector <T> & parameter)
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

   BSTR text;
   pParentElement->get_text(&text);

   //**************************************************//
   //       Split the text into a vector <string>
   //**************************************************//
   wstring ws = (wstring)text;
   string tmpSt(ws.begin(),ws.end());
   stringstream  ss(tmpSt);
   string item;
   vector <string> elems;
   while (std::getline(ss, item, ','))
   {
      elems.push_back(item);
   }
   parameter.clear();

   //**************************************************//
   //       Convert each of string to T type
   //**************************************************//
   for (UINT i=0; i < elems.size(); i++)
   {
      T tmp;
      RFCal_Helper_Convert_Str_To_Value(elems[i] , tmp);
      parameter.push_back(tmp);
   }
   pXPathNode.Release();
   return true;
}
template<typename T> unsigned char RFCal_Helper_Read_Param(MSXML2::IXMLDOMNodePtr parentNode, const string & XPath, T& parameter, T defaultValue)
{
   if(parentNode == NULL)
   {
      parameter =  defaultValue;
      return false;
   }
   //**************************************************//
   //       Get a text from given XPath node
   //**************************************************//
   MSXML2::IXMLDOMNodePtr pXPathNode = parentNode->selectSingleNode((_bstr_t)(XPath.c_str()));
   if (pXPathNode == NULL)
   {
      parameter = defaultValue;
      return false;
   }
   MSXML2::IXMLDOMElementPtr pParentElement(pXPathNode);
   BSTR text;
   pParentElement->get_text(&text);
   wstring ws = (wstring)text;
   string tmpSt(ws.begin(),ws.end());
   //**************************************************//
   //       Convert the text to T type
   //**************************************************//
   T tmp;
   RFCal_Helper_Convert_Str_To_Value(tmpSt, tmp);
   parameter = tmp;
   pXPathNode.Release();
   return true;
}
template <typename T> unsigned char RFCal_Helper_Read_Result(MSXML2::IXMLDOMNodePtr parentNode,const int type, const string & stAttribute, T & parameter)
{
   string stTmp;
   if (type == 0)
   {
      stTmp = "Inputs/DI/V[../N[. = '" + stAttribute + "']]";
   }
   else if (type == 1)
   {
      stTmp = "Outputs/Result/DI/V[../N[. = '" + stAttribute + "']]";
   }
   unsigned char ret = RFCal_Helper_Read_Param(parentNode,stTmp, parameter);
   if (typeid(T) == typeid(double))
   {
      if ((double)parameter < - 100000)
      {
         parameter = 0;
      }
   }
   return ret;
}
unsigned char RFCal_Helper_ESC_ET_PA_PARAM_Assignment_XML(QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param& param)
{
   // The following is dummy assignment for demo purpose, they will be overwritten by XML.
   int product_rev = 0;
   int product_id = 1;
   int manuf_id = 263;

   // ***************************************************************//
    //      Get GPOET info using QLIB_FTM_GET_RFFE_DEVICE_INFO
    // ***************************************************************//

   double qpoetVdMin = 0.6;
   double qpoetVdMax = 3.8;
   byte devId = 0;
    if (!demo_param.bSimulationMode)
    {
        FTM_GET_RFFE_DEVICE_INFO_Request device_req;
        FTM_GET_RFFE_DEVICE_INFO_Response device_res;
        device_req.rf_mode = param.RF_Mode;
      devId = RFCal_Helper_GetIndexFromBitMask(param.TxDeviceFlag);
      device_req.rfm_device = devId;
        device_req.rffe_device_type = RFFE_DEVICE_PAPM;
        memset(&device_res,0,sizeof(FTM_GET_RFFE_DEVICE_INFO_Response));
      if(QLIB_FTM_GET_RFFE_DEVICE_INFO(demo_param.hPhone,&device_req,&device_res))
      {
            // Comment out for demo purpose
            //throw RFCal_Helper_Exception("Failed: QLIB_FTM_GET_RFFE_DEVICE_INFO.",  __FILE__, __LINE__ );
            product_rev = static_cast<int>(device_res.rev_id[0]);
            product_id = static_cast<int>(device_res.prd_id[0]);
            manuf_id = static_cast<int>(device_res.mfg_id[0]);
           if( manuf_id == 535 && product_id == 48 && product_rev == 5 )
          {
              qpoetVdMin = 0.6;
              qpoetVdMax = 3.8;
          }
          //QFE3100
          else if( manuf_id == 535 && product_id == 50 && product_rev == 4 )
          {
              qpoetVdMin = 1.0;
              qpoetVdMax = 4.2;
          }
          //Others(if SW doesn't support PAPM device info, then it returns all 0s. Default to QFE1100 in that case)
          else
          {
              qpoetVdMin = 0.6;
              qpoetVdMax = 3.8;
          }
      }
    }
   param.PA_Param.qpoetVdMin = qpoetVdMin;
   param.PA_Param.qpoetVdMax = qpoetVdMax;

    // ***************************************************************//
   //    Get device info using QLIB_FTM_GET_RFFE_DEVICE_INFO
   // ***************************************************************//
   if (!demo_param.bSimulationMode)
   {
      FTM_GET_RFFE_DEVICE_INFO_Request device_req;
      FTM_GET_RFFE_DEVICE_INFO_Response device_res;
      device_req.rf_mode = param.RF_Mode;
      device_req.rfm_device = devId;
        device_req.rffe_device_type = RFFE_DEVICE_PA;
      memset(&device_res,0,sizeof(FTM_GET_RFFE_DEVICE_INFO_Response));
      if(!QLIB_FTM_GET_RFFE_DEVICE_INFO(demo_param.hPhone,&device_req,&device_res))
      {
            // Comment out for demo purpose
            //throw RFCal_Helper_Exception("Failed: QLIB_FTM_GET_RFFE_DEVICE_INFO.",  __FILE__, __LINE__ );
            product_rev = static_cast<int>(device_res.rev_id[0]);
            product_id = static_cast<int>(device_res.prd_id[0]);
            manuf_id = static_cast<int>(device_res.mfg_id[0]);
      }
      else
      {
         product_rev = static_cast<int>(device_res.rev_id[0]);
         product_id = static_cast<int>(device_res.prd_id[0]);
         manuf_id = static_cast<int>(device_res.mfg_id[0]);
      }
   }

   //**************************************************//
   //       Get the root nodes giving tech and band
   //**************************************************//
   CoInitialize(NULL); // Initialize COM
   MSXML2::IXMLDOMDocumentPtr spXMLDoc;
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
   spXMLDoc->load(demo_param.stPA_PARAM_filename.c_str());
   if (!spXMLDoc)
   {
      CoUninitialize();
      throw RFCal_Helper_Exception("The char XML not loaded.",  __FILE__, __LINE__ );
   }

   // ***************************************************************//
   //    Assign PA PARAM parameters
   // ***************************************************************//

   stringstream stPA_Param_XPATH;
   stPA_Param_XPATH << "/RFCal_Data/PA[@product_rev='" << product_rev;
   stPA_Param_XPATH << "' and @product_id='" << product_id << "' and @manuf_id='" << manuf_id << "']";
   stPA_Param_XPATH << "/XptInput_V1 [@rf_mode='" << param.RF_Mode << "' and @device='0']";

   MSXML2::IXMLDOMNodePtr pNode;
   pNode = spXMLDoc->selectSingleNode((_bstr_t)stPA_Param_XPATH.str().c_str());

   if (pNode != NULL) // APT only RF Mode may not have this information.
   {
      RFCal_Helper_Read_Attribute(pNode,"IcqEt_table", "IcqEt", param.PA_Param.IcqEt);
      RFCal_Helper_Read_Attribute(pNode,"IcqEpt_table", "IcqEpt", param.PA_Param.IcqEpt);
      RFCal_Helper_Read_Attribute(pNode,"IcqRgiDeltaSwp_table", "IcqRgiDeltaSwp", param.PA_Param.IcqRgiDeltaSwp);
      RFCal_Helper_Read_Attribute(pNode,"MLineMaxPaSupplyEt_table", "MLineMaxPaSupplyEt", param.PA_Param.MLineMaxPaSupplyEt);
      RFCal_Helper_Read_Attribute(pNode,"MLineMaxPaSupplyEpt_table", "MLineMaxPaSupplyEpt", param.PA_Param.MLineMaxPaSupplyEpt);
      RFCal_Helper_Read_Attribute(pNode,"IqGainRgiDeltaSwp_table", "IqGainRgiDeltaSwp", param.PA_Param.IqGainRgiDeltaSwp);
      RFCal_Helper_Read_Attribute(pNode,"EtVmin_table", "EtVmin", param.PA_Param.EtVmin);
      RFCal_Helper_Read_Attribute(pNode,"FirmwareDelayIqGain_table", "FirmwareDelayIqGain", param.PA_Param.FirmwareDelayIqGain);
      RFCal_Helper_Read_Attribute(pNode,"AclrDelayCharIqGain_table", "AclrDelayCharIqGain", param.PA_Param.AclrDelayCharIqGain);

      RFCal_Helper_Read_Attribute(pNode,"MLineFitType_table", "MLineFitType", param.PA_Param.MLineFitType);
      RFCal_Helper_Read_Attribute(pNode,"MLineRegCoef_table", "MLineRegCoef", param.PA_Param.MLineRegCoef);
      RFCal_Helper_Read_Attribute(pNode,"CalCompPtEpt_table", "CalCompPtEpt", param.PA_Param.CalCompPtEpt);
      RFCal_Helper_Read_Attribute(pNode,"CalCompPtEt_table", "CalCompPtEt", param.PA_Param.CalCompPtEt);
      pNode.Release();

      for (UINT i = 0; i < param.Swp.size() ; i++)
      {
         if (param.Swp[i].Sweep_Type == XPT_DELAY)
         {
            if (demo_param.TECH == 0)
            {
               // CDMA
               param.Swp[i].XPT_Delay_Swp_IQ_Gain = param.PA_Param.AclrDelayCharIqGain;
            }
            else
            {
               param.Swp[i].XPT_Delay_Swp_IQ_Gain = param.PA_Param.FirmwareDelayIqGain;
            }
         }
      }

      //**************************************************//
      //       Get Detrough_table
      //**************************************************//
      stPA_Param_XPATH << "/Detrough_table/XptEpdtRec";
      MSXML2::IXMLDOMNodeListPtr pNodeList;
      pNodeList = spXMLDoc->selectNodes((_bstr_t)stPA_Param_XPATH.str().c_str());
      if (pNodeList != NULL) // APT only char table may not have detrough data.
      {
         param.PA_Param.Detrough_table.resize(128);
         for (int i= 0; i < pNodeList->length  ; i++)
         {
            pNode = pNodeList->nextNode();
            if (i >= static_cast<int>(param.PA_Param.Detrough_table.size()))
            {
               pNode.Release();
               break;
            }
            RFCal_Helper_Read_Attribute(pNode, "epdt", param.PA_Param.Detrough_table[i]);
            pNode.Release();
         }
         pNodeList.Release();
      }
   }
   else
   {
      // This is only demo purpose
      param.PA_Param.IcqEt = 156;
      param.PA_Param.IcqRgiDeltaSwp  = 255;
      param.PA_Param.IqGainRgiDeltaSwp  = 400;
      param.PA_Param.EtVmin = 1.0;
      param.PA_Param.FirmwareDelayIqGain = 622;
      param.PA_Param.MLineRegCoef = 0.5;
      param.PA_Param.CalCompPtEpt = 2;
      param.PA_Param.CalCompPtEt = 3;
      param.PA_Param.MLineFitType = 0;
      param.PA_Param.MLineMaxPaSupplyEpt = 3.3;
      param.PA_Param.MLineMaxPaSupplyEt = 3.8;
   }
   spXMLDoc.Release();
   CoUninitialize();
   return true;
}
unsigned char RFCal_Helper_ESC_ET_CharData_Assignment_XML(QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param& param)
{
   //**************************************************//
   //       Load XML and get a node with the rf_mode
   //**************************************************//
   CoInitialize(NULL); // Initialize COM
   MSXML2::IXMLDOMDocumentPtr spXMLDoc;
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
   spXMLDoc->load(demo_param.stESC_Char_filename.c_str());
   if (!spXMLDoc)
   {
      CoUninitialize();
      throw RFCal_Helper_Exception("The char XML not loaded.",  __FILE__, __LINE__ );
      return true;
   }
   // ***************************************************************//
   //    APT Char XML
   //    Get the root nodes giving tech and band
   //************************************++**************************//

   stringstream stChar_Param_XPATH;
   int device = static_cast<int>(param.Swp[0].Device_Selection.Master_Device_ID);
   stChar_Param_XPATH << "/RFCal_Data/APTCharV3[@rf_mode='" <<  param.RF_Mode << "' and @device='" << device << "']/APTTable";
   MSXML2::IXMLDOMNodeListPtr pAPTtableList = spXMLDoc->selectNodes((_bstr_t)stChar_Param_XPATH.str().c_str());

   if (!pAPTtableList)
   {
      spXMLDoc.Release();
      CoUninitialize();
      throw RFCal_Helper_Exception("APT char table not found.",  __FILE__, __LINE__ );
   }

   // ***************************************************************//
   //    Read each of nodes and assign APT table map
   //************************************++**************************//

   for (int i= 0; i < pAPTtableList->length; i++)
   {
      MSXML2::IXMLDOMNodePtr pNode;

      vector <APTRec> vecAPTRec;
      pNode = pAPTtableList->nextNode();
      dword channel;
      byte pa_state;
      byte tx_purpose;
      RFCal_Helper_Read_Attribute(pNode, "channel", channel);
      RFCal_Helper_Read_Attribute(pNode, "pa_state", pa_state);
      RFCal_Helper_Read_Attribute(pNode, "tx_purpose", tx_purpose);
      MSXML2::IXMLDOMNodeListPtr pAPTRecList = pNode->selectNodes((_bstr_t)"APTRec");
      for (int AptRecIndex = 0; AptRecIndex < pAPTRecList->length; AptRecIndex++)
      {
         APTRec tmpRecord;
         MSXML2::IXMLDOMNodePtr pRecNode = pAPTRecList->nextNode();
         RFCal_Helper_Read_Attribute(pRecNode, "pwr", tmpRecord.pwr);
         RFCal_Helper_Read_Attribute(pRecNode, "vcc", tmpRecord.vcc);
         RFCal_Helper_Read_Attribute(pRecNode, "icq", tmpRecord.icq);
         vecAPTRec.push_back(tmpRecord);
         pRecNode.Release();
      }
      pAPTRecList.Release();
      pNode.Release();
      APT_CharV3_Table_Key AptCharTablekey(param.RF_Mode,(byte)device,channel,pa_state,tx_purpose);
      param.APTCharV3.insert( make_pair(AptCharTablekey,vecAPTRec));
   }
   pAPTtableList.Release();
   spXMLDoc.Release();
   CoUninitialize();
   return true;
}
unsigned char RFCal_Helper_ESC_ET_Param_Assignment_XML(QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param& param)
{
   //**************************************************//
   //       This function reads
   //       params.xml, char.xml and PA_PARAMS.xml
   //       Also, configure sweep segement
   //
   //**************************************************//

   //**************************************************//
   //       parameters
   //**************************************************//

   MSXML2::IXMLDOMDocumentPtr spXMLDoc;
   MSXML2::IXMLDOMNodePtr pRoot_Node;
   string stTECH[5]= {"CDMA","WCDMA","GSM","LTE","TDSCDMA"};
   stringstream stXPATH;

   //**************************************************//
   //       Load XML and get a node with the rf_mode
   //**************************************************//
   CoInitialize(NULL); // Initialize COM
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
   spXMLDoc->load(demo_param.stESC_Params_filename.c_str());
   if (!spXMLDoc)
   {
      CoUninitialize();
      stringstream ss;
      ss << "The param XML not loaded. " << demo_param.stESC_Params_filename.c_str();
      throw RFCal_Helper_Exception(ss.str(),  __FILE__, __LINE__ );
   }
   //**************************************************//
   //       Get the root nodes giving tech and band
   //**************************************************//
   stXPATH << "/CalV4/Node_Name[@name='" << stTECH[demo_param.TECH] << "_B" << (int)demo_param.BAND
           << "_Dev" << (int)demo_param.Device << "_" << (int)demo_param.Instance << "']";
   pRoot_Node = spXMLDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());

   if (pRoot_Node == NULL)
   {
      spXMLDoc.Release();
      CoUninitialize();
      throw RFCal_Helper_Exception("The param data not found.",  __FILE__, __LINE__ );
   }

   const byte INVALIDbye = 0xFF;
   //**************************************************//
   //       Assign parameters
   //**************************************************//
   RFCal_Helper_Read_Param(pRoot_Node,"RF_Mode",param.RF_Mode);
   RFCal_Helper_Read_Param(pRoot_Node,"En_Debug_Mode",param.En_Debug_Mode);
   RFCal_Helper_Read_Param(pRoot_Node,"En_Compress_Esc_Req",param.En_Compress_Esc_Req);
   RFCal_Helper_Read_Param(pRoot_Node,"En_Compress_Esc_Res",param.En_Compress_Esc_Res);
   RFCal_Helper_Read_Param(pRoot_Node,"Tx_Rx_Split",param.Tx_Rx_Split);
   RFCal_Helper_Read_Param(pRoot_Node,"Set_CalState",param.Set_CalState);

   
   RFCal_Helper_Read_Param(pRoot_Node,"En_EPTET_Different_PAstate",param.En_EPTET_Different_PAstate);
   RFCal_Helper_Read_Param(pRoot_Node,"En_EPTET_Different_Icq",param.En_EPTET_Different_Icq);

   RFCal_Helper_Read_Param(pRoot_Node,"Bias_RGI_Zero_Seg",param.Bias_RGI_Zero_Seg);

   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/NV_Mode",param.NV_Generation_Params.NV_Mode);
   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/Use_Consolidated_Rx_NV",param.NV_Generation_Params.Use_Consolidated_Rx_NV);
   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/Read_Consolidated_Rx_NV_From_DUT",param.NV_Generation_Params.Read_Consolidated_Rx_NV_From_DUT);
   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/Max_HDET_Value",param.NV_Generation_Params.Max_HDET_Value,static_cast<word>(4095));
   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/Max_HDET_Pwr",param.NV_Generation_Params.Max_HDET_Pwr);
   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/Min_HDET_Pwr",param.NV_Generation_Params.Min_HDET_Pwr);
   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/Min_HDET_Delta",param.NV_Generation_Params.Min_HDET_Delta);
   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/Dual_Carr_NV_Mode",param.NV_Generation_Params.Dual_Carr_NV_Mode);
   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/NV_Write_Action_Flag",param.NV_Generation_Params.NV_Write_Action_Flag);
   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/NV_Logging_En_Flag",param.NV_Generation_Params.NV_Logging_En_Flag);
   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/Max_LPM_HDET_Pwr",param.NV_Generation_Params.Max_LPM_HDET_Pwr);
   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/Min_LPM_HDET_Pwr",param.NV_Generation_Params.Min_LPM_HDET_Pwr);
   RFCal_Helper_Read_Param(pRoot_Node,"NV_Generation_Params/LPM_HDET_Start_Pwr",param.NV_Generation_Params.LPM_HDET_Start_Pwr);

   //**************************************************//
   //       Get sweep_param nodes
   //**************************************************//
   MSXML2::IXMLDOMNodeListPtr pSweep_Param_Node_List;
   pSweep_Param_Node_List = pRoot_Node->selectNodes("Sweep_Params");
   if (pSweep_Param_Node_List == NULL)
   {
      pRoot_Node.Release();
      spXMLDoc.Release();
      CoUninitialize();
      throw RFCal_Helper_Exception("The sweep param data not found in the XML.",  __FILE__, __LINE__ );
   }

   unsigned int iSweepSize = static_cast<unsigned int>(pSweep_Param_Node_List->length);
   param.Swp.resize(iSweepSize);

   //**************************************************//
   //       Assign parameters for each of sweep_param node
   //**************************************************//

   for (unsigned int iSwpIndex = 0; iSwpIndex < iSweepSize; iSwpIndex++)
   {
      MSXML2::IXMLDOMNodePtr pNode = pSweep_Param_Node_List->nextNode();

      Sweep_Params & sweep_param = param.Swp[iSwpIndex];
      RFCal_Helper_Read_Param(pNode,"Sweep_Type",sweep_param.Sweep_Type);
      RFCal_Helper_Read_Param(pNode,"Pre_Sweep_Computation",sweep_param.Pre_Sweep_Computation);
      RFCal_Helper_Read_Param(pNode,"Compute_From_Sweep",sweep_param.Compute_From_Sweep);

      //**************************************************//
      //    Device Selection
      //**************************************************//
      MSXML2::IXMLDOMNodePtr pDevNode = pNode->selectSingleNode("Device_Selection");
      RFCal_Helper_Read_Param(pDevNode,"Master_Device_ID",sweep_param.Device_Selection.Master_Device_ID);
      RFCal_Helper_Read_Param(pDevNode,"Slave_Device_ID",sweep_param.Device_Selection.Slave_Device_ID);
      RFCal_Helper_Read_Param(pDevNode,"Feedback_Device_ID",sweep_param.Device_Selection.Feedback_Device_ID,static_cast<byte>(1));
      RFCal_Helper_Read_Param(pDevNode,"Tx_NV_Container",sweep_param.Device_Selection.Tx_NV_Container,static_cast<byte>(0xFF));
      RFCal_Helper_Read_Param(pDevNode,"Pcell_Master_Device_ID",sweep_param.Device_Selection.Scell_Master_Device_ID,sweep_param.Device_Selection.Master_Device_ID);
      RFCal_Helper_Read_Param(pDevNode,"Pcell_Slave_Device_ID",sweep_param.Device_Selection.Scell_Slave_Device_ID,sweep_param.Device_Selection.Slave_Device_ID);

      RFCal_Helper_Read_Param(pDevNode,"Scell_Master_Device_ID",sweep_param.Device_Selection.Scell_Master_Device_ID,static_cast<byte>(2));
      RFCal_Helper_Read_Param(pDevNode,"Scell_Slave_Device_ID",sweep_param.Device_Selection.Scell_Slave_Device_ID,static_cast<byte>(3));
      
      // If Tx_Rx_Split is true AND there is some value for Tx_NV_Container, then, ignore NV_Device_To_Chain_Map
      if (param.Tx_Rx_Split && sweep_param.Device_Selection.Tx_NV_Container != 0xFF)
      {
         byte dev = sweep_param.Device_Selection.Master_Device_ID;
         byte chain = sweep_param.Device_Selection.Tx_NV_Container;
         sweep_param.Device_Selection.NV_Device_To_Chain_Map[dev] = chain;
      }
      else
      {
         vector < vector  < byte > > tmpNV_Device_To_Chain;
         RFCal_Helper_Read_Param(pDevNode,"NV_Device_To_Chain_Map", tmpNV_Device_To_Chain);
         for (vector < vector  < byte > >::iterator it = tmpNV_Device_To_Chain.begin(); it != tmpNV_Device_To_Chain.end(); it ++)
         {
            if ((*it).size() >= 2)
            {
               byte dev = (*it)[0];
               byte chain = (*it)[1];
               sweep_param.Device_Selection.NV_Device_To_Chain_Map[dev] = chain;
            }
         }
      }
      
      RFCal_Helper_Read_Param(pDevNode,"DevicePathPair_To_NVcontainer",sweep_param.Device_Selection.DevicePathPair_To_NVcontainer);

      RFCal_Helper_Read_Param(pDevNode,"Master_Device_ID_List",sweep_param.Device_Selection.Master_Device_ID_List);
      RFCal_Helper_Read_Param(pDevNode,"Slave_Device_ID_List",sweep_param.Device_Selection.Slave_Device_ID_List);

      RFCal_Helper_Read_Param(pDevNode,"Tx_Device_To_Calibrate",sweep_param.Device_Selection.Tx_Device_To_Calibrate);
      RFCal_Helper_Read_Param(pDevNode,"Rx_Device_To_Calibrate",sweep_param.Device_Selection.Rx_Device_To_Calibrate);
      pDevNode.Release();

      //**************************************************//
      //    Waveform
      //**************************************************//
      MSXML2::IXMLDOMNodePtr pWavNode = pNode->selectSingleNode("Waveform");
      RFCal_Helper_Read_Param(pWavNode,"Waveform_Type",sweep_param.Waveform.Waveform_Type);
      RFCal_Helper_Read_Param(pWavNode,"Start_RB",sweep_param.Waveform.Start_RB);
      RFCal_Helper_Read_Param(pWavNode,"Number_Of_RBs",sweep_param.Waveform.Number_Of_RBs);
      RFCal_Helper_Read_Param(pWavNode,"Tx_BW",sweep_param.Waveform.Tx_BW);
      RFCal_Helper_Read_Param(pWavNode,"Rx_BW",sweep_param.Waveform.Rx_BW);
      RFCal_Helper_Read_Param(pWavNode,"EUL_Waveform_Type",sweep_param.Waveform.EUL_Waveform_Type);
      pWavNode.Release();

      //**************************************************//
      //    APT and general parameters
      //**************************************************//

      RFCal_Helper_Read_Param(pNode,"Ref_Channel",sweep_param.Ref_Channel);
      RFCal_Helper_Read_Param(pNode,"Upper_Bound_Channel",sweep_param.Upper_Bound_Channel);
      RFCal_Helper_Read_Param(pNode,"PA_State_List",sweep_param.PA_State_List);
      RFCal_Helper_Read_Param(pNode,"RGI_List",sweep_param.RGI_List);
      RFCal_Helper_Read_Param(pNode,"Bias_List",sweep_param.Bias_List);
      RFCal_Helper_Read_Param(pNode,"PA_Quiescent_Current",sweep_param.PA_Quiescent_Current);
      RFCal_Helper_Read_Param(pNode,"IQ_Gain",sweep_param.IQ_Gain);
      RFCal_Helper_Read_Param(pNode,"Env_Scale",sweep_param.Env_Scale);
      RFCal_Helper_Read_Param(pNode,"HDET_Avgs",sweep_param.HDET_Avgs);
      RFCal_Helper_Read_Param(pNode,"LPM_HDET_Avgs",sweep_param.LPM_HDET_Avgs);
      RFCal_Helper_Read_Param(pNode,"Segment_Length",sweep_param.Segment_Length);
      RFCal_Helper_Read_Param(pNode,"Config_Segment_Length",sweep_param.Config_Segment_Length);
      RFCal_Helper_Read_Param(pNode,"Tuning_Segment_Length",sweep_param.Tuning_Segment_Length);

      RFCal_Helper_Read_Param(pNode,"Min_Start_Pwr",sweep_param.Min_Start_Pwr);
      RFCal_Helper_Read_Param(pNode,"Max_Expected_Pwr",sweep_param.Max_Expected_Pwr);
      RFCal_Helper_Read_Param(pNode,"Lowest_End_Pwr",sweep_param.Lowest_End_Pwr);
      RFCal_Helper_Read_Param(pNode,"Purpose_List",sweep_param.Purpose_List);
      if (sweep_param.Purpose_List.size() != sweep_param.PA_State_List.size())
      {
         for (vector < byte >::iterator it = sweep_param.PA_State_List.begin(); it != sweep_param.PA_State_List.end(); it++)
            sweep_param.Purpose_List.push_back(255);
      }
      RFCal_Helper_Read_Param(pNode,"Post_Sweep_Computation",sweep_param.Post_Sweep_Computation);
      RFCal_Helper_Read_Param(pNode,"Compute_For_Sweep",sweep_param.Compute_For_Sweep);
      RFCal_Helper_Read_Param(pNode,"Enable_Sweep_Mode",sweep_param.Enable_Sweep_Mode,static_cast<byte>(1));
      RFCal_Helper_Read_Param(pNode,"Enable_Downlink_Aux_Port",sweep_param.Enable_Downlink_Aux_Port);

      //**************************************************//
      //    Tx Internal Cal / Ref Cal parameters
      //**************************************************//
      RFCal_Helper_Read_Param(pNode,"Low_Power_Threshold_RGI",sweep_param.Low_Power_Threshold_RGI);
      RFCal_Helper_Read_Param(pNode,"Low_Power_Threshold_PA",sweep_param.Low_Power_Threshold_PA);
      RFCal_Helper_Read_Param(pNode,"Ref_RGI_List",sweep_param.Ref_RGI_List);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_RGI_Delta_Ref_RGI",sweep_param.XPT_Swp1_RGI_Delta_Ref_RGI);
      // Fb mode for tx internal cal
      RFCal_Helper_Read_Param(pNode,"Fb_Rx_Gain_Cal_Mode",sweep_param.Fb_Rx_Gain_Cal_Mode); 
      RFCal_Helper_Read_Param(pNode,"FBGain_Cal_Gain_State_Swpt_List",sweep_param.FBGain_Cal_Gain_State_Swpt_List); 
      RFCal_Helper_Read_Param(pNode,"FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld_List",sweep_param.FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld_List); 
      RFCal_Helper_Read_Param(pNode,"Enable_Tx_Int_Cal_Power_Calc_Fudge",sweep_param.Enable_Tx_Int_Cal_Power_Calc_Fudge,static_cast<byte>(0)); 
      RFCal_Helper_Read_Param(pNode,"Enable_Box_Measurements",sweep_param.Enable_Box_Measurements,static_cast<byte>(true));


      //**************************************************//
      //    Feedback Rx Gain Cal parameters
      //**************************************************//
      RFCal_Helper_Read_Param(pNode,"Do_FBGain_Cal",sweep_param.Do_FBGain_Cal);
      RFCal_Helper_Read_Param(pNode,"FBGain_Cal_XPTMode_List",sweep_param.FBGain_Cal_XPTMode_List);
      RFCal_Helper_Read_Param(pNode,"FBGain_Cal_Gain_State_List",sweep_param.FBGain_Cal_Gain_State_List);
      RFCal_Helper_Read_Param(pNode,"FBGain_Cal_UBias_List",sweep_param.FBGain_Cal_UBias_List);
      RFCal_Helper_Read_Param(pNode,"FBGain_Cal_RxScale_List",sweep_param.FBGain_Cal_RxScale_List);
      RFCal_Helper_Read_Param(pNode,"FBGain_Cal_GainComp_Val",sweep_param.FBGain_Cal_GainComp_Val);
      RFCal_Helper_Read_Param(pNode,"Enable_FBGain_Cal_Debug_Mode",sweep_param.Enable_FBGain_Cal_Debug_Mode);
      RFCal_Helper_Read_Param(pNode,"FBGain_Cal_Do_Therm_Read",sweep_param.FBGain_Cal_Do_Therm_Read);
      RFCal_Helper_Read_Param(pNode,"FBGain_Cal_BW_List",sweep_param.FBGain_Cal_BW_List);
      RFCal_Helper_Read_Param(pNode,"FBGain_Cal_StartRB_List",sweep_param.FBGain_Cal_StartRB_List);
      RFCal_Helper_Read_Param(pNode,"FBGain_Cal_NumRB_List",sweep_param.FBGain_Cal_NumRB_List);
      RFCal_Helper_Read_Param(pNode,"FBRx_Cal_Mode",sweep_param.FBRx_Cal_Mode);
      RFCal_Helper_Read_Param(pNode,"Fb_Rx_Gain_Cal_Mode",sweep_param.Fb_Rx_Gain_Cal_Mode);

      //**************************************************//
      //    Enh Internal Device Cal parameters
      //**************************************************//
      RFCal_Helper_Read_Param(pNode,"Do_Enh_Int_Cal",sweep_param.Do_Enh_Int_Cal);
      RFCal_Helper_Read_Param(pNode,"Enh_Int_Cal_Tx_Bandwidth_List",sweep_param.Enh_Int_Cal_Tx_Bandwidth_List);
      RFCal_Helper_Read_Param(pNode,"Enh_Int_Cal_Type_Mask",sweep_param.Enh_Int_Cal_Type_Mask);
      RFCal_Helper_Read_Param(pNode,"PA_State",sweep_param.PA_State);
      RFCal_Helper_Read_Param(pNode,"RGI",sweep_param.RGI);
      RFCal_Helper_Read_Param(pNode,"Bias",sweep_param.Bias);
      RFCal_Helper_Read_Param(pNode,"Icq",sweep_param.Icq);
      RFCal_Helper_Read_Param(pNode,"Enh_Int_Cal_Flag",sweep_param.Enh_Int_Cal_Flag,static_cast<word>(1));
      RFCal_Helper_Read_Param(pNode,"TechBW_List",sweep_param.TechBW_List);
      RFCal_Helper_Read_Param(pNode,"Enh_Int_Cal_Segment_Length",sweep_param.Enh_Int_Cal_Segment_Length,static_cast<dword>(20000));

      //**************************************************//
      //      Enh Internal Device Cal - FBRxDC Cal - parameters
      //**************************************************//
      RFCal_Helper_Read_Param(pNode,"Enh_Int_Cal_Debug_Mode_Enable",sweep_param.Enh_Int_Cal_Debug_Mode_Enable);
      RFCal_Helper_Read_Param(pNode,"Device_Mask",sweep_param.Device_Mask);
      RFCal_Helper_Read_Param(pNode,"FBRx_DC_Gain_State_List",sweep_param.FBRx_DC_Gain_State_List);
      RFCal_Helper_Read_Param(pNode,"FBRx_DC_Num_Capture_Samples",sweep_param.FBRx_DC_Num_Capture_Samples);
      RFCal_Helper_Read_Param(pNode,"FBRx_DC_I_Min_Limit",sweep_param.FBRx_DC_I_Min_Limit);
      RFCal_Helper_Read_Param(pNode,"FBRx_DC_I_Max_Limit",sweep_param.FBRx_DC_I_Max_Limit);
      RFCal_Helper_Read_Param(pNode,"FBRx_DC_Q_Min_Limit",sweep_param.FBRx_DC_Q_Min_Limit);
      RFCal_Helper_Read_Param(pNode,"FBRx_DC_Q_Max_Limit",sweep_param.FBRx_DC_Q_Max_Limit);

      //**************************************************//
      //      Enh Internal Device Cal - FBRxRSB Cal - parameters
      //**************************************************//
      if (sweep_param.Sweep_Type == REF_CAL)
      {
        RFCal_Helper_Read_Param(pNode,"Do_Enh_Int_FBRxRsb_Cal",sweep_param.Do_Enh_Int_FBRxRsb_Cal);
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_Segment_Length",sweep_param.FBRxRsb_Segment_Length);
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_Device_Mask",sweep_param.FBRxRsb_Device_Mask,static_cast<byte>(1));
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_Bandwidth",sweep_param.FBRxRsb_Bandwidth,static_cast<byte>(13));
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_DbgLevel",sweep_param.FBRxRsb_DbgLevel);
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_Type",sweep_param.FBRxRsb_Type,static_cast<byte>(1));
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_Algorithm",sweep_param.FBRxRsb_Algorithm,static_cast<byte>(1));
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_Waveform_Offset_in_kHz",sweep_param.FBRxRsb_Waveform_Offset_in_kHz,static_cast<dword>(1000));
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_Gain_State_List",sweep_param.FBRxRsb_Gain_State_List);
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_Power_List",sweep_param.FBRxRsb_Power_List);
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_CarrierID",sweep_param.FBRxRsb_CarrierID);
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_IQ_Capture_Type",sweep_param.FBRxRsb_IQ_Capture_Type,static_cast<byte>(1));
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_IQ_Capture_Size",sweep_param.FBRxRsb_IQ_Capture_Size,static_cast<word>(4096));
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_PCell_Band_For_SCell",sweep_param.FBRxRsb_PCell_Band_For_SCell);
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_PCell_RefChan_For_SCell",sweep_param.FBRxRsb_PCell_RefChan_For_SCell);
        RFCal_Helper_Read_Param(pNode,"Do_FBRxRsb_Limit_Check",sweep_param.Do_FBRxRsb_Limit_Check);
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_PreCal_Rsb_Limit",sweep_param.FBRxRsb_PreCal_Rsb_Limit);
        RFCal_Helper_Read_Param(pNode,"FBRxRsb_PostCal_Rsb_Limit",sweep_param.FBRxRsb_PostCal_Rsb_Limit);
      }
      //**************************************************//
      //      Enh Internal Device Cal - RxRSB Cal - parameters
      //**************************************************//
      if (sweep_param.Sweep_Type == RX_RSB_CAL)
      {
        RFCal_Helper_Read_Param(pNode,"Do_Enh_Int_Rx_Rsb_Cal",sweep_param.Do_Enh_Int_Rx_Rsb_Cal);
        RFCal_Helper_Read_Param(pNode,"RxRsb_Segment_Length",sweep_param.RxRsb_Segment_Length);
        RFCal_Helper_Read_Param(pNode,"RxRsb_Device_Mask",sweep_param.RxRsb_Device_Mask);
        RFCal_Helper_Read_Param(pNode,"RxRsb_DL_Bandwidth",sweep_param.RxRsb_DL_Bandwidth,static_cast<byte>(3));
        RFCal_Helper_Read_Param(pNode,"RxRsb_DbgLevel",sweep_param.RxRsb_DbgLevel);
        RFCal_Helper_Read_Param(pNode,"RxRsb_Type",sweep_param.RxRsb_Type);
        RFCal_Helper_Read_Param(pNode,"RxRsb_Algorithm",sweep_param.RxRsb_Algorithm);
        RFCal_Helper_Read_Param(pNode,"RxRsb_Waveform_Offset_in_kHz",sweep_param.RxRsb_Waveform_Offset_in_kHz);
        RFCal_Helper_Read_Param(pNode,"RxRsb_Gain_State_List",sweep_param.RxRsb_Gain_State_List);
        RFCal_Helper_Read_Param(pNode,"RxRsb_DL_Power_List",sweep_param.RxRsb_DL_Power_List);
        RFCal_Helper_Read_Param(pNode,"RxRsb_CarrierID",sweep_param.RxRsb_CarrierID);
        RFCal_Helper_Read_Param(pNode,"RxRsb_IQ_Capture_Type",sweep_param.RxRsb_IQ_Capture_Type);
        RFCal_Helper_Read_Param(pNode,"RxRsb_IQ_Capture_Size",sweep_param.RxRsb_IQ_Capture_Size);
        RFCal_Helper_Read_Param(pNode,"RxRsb_PCell_RFMode_For_SCell",sweep_param.RxRsb_PCell_RFMode_For_SCell);
        RFCal_Helper_Read_Param(pNode,"RxRsb_PCell_Band_For_SCell",sweep_param.RxRsb_PCell_Band_For_SCell);
        RFCal_Helper_Read_Param(pNode,"RxRsb_PCell_RefChan_For_SCell",sweep_param.RxRsb_PCell_RefChan_For_SCell);
        RFCal_Helper_Read_Param(pNode,"Do_RxRsb_Limit_Check",sweep_param.Do_RxRsb_Limit_Check);
        RFCal_Helper_Read_Param(pNode,"RxRsb_PreCal_Rsb_Limit",sweep_param.RxRsb_PreCal_Rsb_Limit);
        RFCal_Helper_Read_Param(pNode,"RxRsb_PostCal_Rsb_Limit",sweep_param.RxRsb_PostCal_Rsb_Limit);
      }
      //**************************************************//
      //    XPT1 parameters
      //**************************************************//
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_Channel_List",sweep_param.XPT_Swp1_Channel_List);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_PA_State",sweep_param.XPT_Swp1_PA_State, INVALIDbye);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_PA_State_ET",sweep_param.XPT_Swp1_PA_State_ET, INVALIDbye);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_PA_State_EPT",sweep_param.XPT_Swp1_PA_State_EPT, INVALIDbye);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_RGI_List",sweep_param.XPT_Swp1_RGI_List);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_RGI_List_ET",sweep_param.XPT_Swp1_RGI_List_ET);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_RGI_List_EPT",sweep_param.XPT_Swp1_RGI_List_EPT);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_Bias_List",sweep_param.XPT_Swp1_Bias_List);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_Bias_List_ET",sweep_param.XPT_Swp1_Bias_List_ET);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_Bias_List_EPT",sweep_param.XPT_Swp1_Bias_List_EPT);

      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_RGI_List",sweep_param.XPT_Swp1_RGI_List);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_IQ_Gain",sweep_param.XPT_Swp1_IQ_Gain);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_Env_Scale",sweep_param.XPT_Swp1_Env_Scale);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_Num_IQ_Samples",sweep_param.XPT_Swp1_Num_IQ_Samples);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_Cap_Segment_Length",sweep_param.XPT_Swp1_Cap_Segment_Length);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_RGI_Delta_RGI_List",sweep_param.XPT_Swp1_RGI_Delta_RGI_List);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_RGI_Delta_RGI_List_ET",sweep_param.XPT_Swp1_RGI_Delta_RGI_List_ET);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_RGI_Delta_RGI_List_EPT",sweep_param.XPT_Swp1_RGI_Delta_RGI_List_EPT);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_Max_Expected_Power",sweep_param.XPT_Swp1_Max_Expected_Power,static_cast<double>(22.0));

      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_RGI_Delta_Bias",sweep_param.XPT_Swp1_RGI_Delta_Bias);
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_RGI_Delta_Segment_Length",sweep_param.XPT_Swp1_RGI_Delta_Segment_Length);
      if ( /* sweep_param.XPT_Swp1_RGI_Delta_Segment_Length == 0 && */ sweep_param.Segment_Length.size() > 0 && sweep_param.Segment_Length[0].size() > 0)
      {
        sweep_param.XPT_Swp1_RGI_Delta_Segment_Length = sweep_param.Segment_Length[0][0];
      }
      RFCal_Helper_Read_Param(pNode,"XPT_Swp1_Minimum_RGI_Delta_Power_dBm100",sweep_param.XPT_Swp1_Minimum_RGI_Delta_Power_dBm100,static_cast<word>(0));

      /* Add Lowest RGI */
        //if (sweep_param.Low_Power_Threshold_RGI > 0 )
        //{
        //  double min_Max_Expected_Pwr = *min_element(sweep_param.Max_Expected_Pwr.begin(),sweep_param.Max_Expected_Pwr.end());
        //  for (UINT i = 0; i < sweep_param.PA_State_List.size(); i++)
        //  {
        //      if (sweep_param.PA_State_List[i] == sweep_param.Low_Power_Threshold_PA && min_Max_Expected_Pwr == sweep_param.Max_Expected_Pwr[i])
        //      {
        //          vector <byte>::iterator it = sweep_param.RGI_List[i].begin();
        //          sweep_param.RGI_List[i].insert(it,sweep_param.Low_Power_Threshold_RGI);
        //          
        //          if (sweep_param.Sweep_Type != REF_CAL)
        //          {
        //              it = sweep_param.Ref_RGI_List[i].begin();
        //              sweep_param.Ref_RGI_List[i].insert(it,sweep_param.Low_Power_Threshold_RGI);
        //          }
        //      }
        //  }
        //}
      if (sweep_param.RGI_List.size() >  sweep_param.Ref_RGI_List.size())
      {
        sweep_param.Ref_RGI_List.resize(sweep_param.RGI_List.size());
      }
      for (UINT iLoop = 0 ; iLoop < sweep_param.RGI_List.size(); iLoop++)
      {
        if (sweep_param.RGI_List[iLoop].size() > sweep_param.Ref_RGI_List[iLoop].size())
        {
          sweep_param.Ref_RGI_List[iLoop].resize(sweep_param.RGI_List[iLoop].size());
        }
      }
        
      if (sweep_param.Sweep_Type == REF_CAL && sweep_param.XPT_Swp1_RGI_Delta_Ref_RGI > 0 )
      {
         sweep_param.PA_State_List.push_back(sweep_param.XPT_Swp1_PA_State);
         sweep_param.RGI_List.resize(sweep_param.PA_State_List.size());
         sweep_param.Bias_List.resize(sweep_param.PA_State_List.size());
         sweep_param.PA_Quiescent_Current.resize(sweep_param.PA_State_List.size());
         sweep_param.Env_Scale.resize(sweep_param.PA_State_List.size());
         sweep_param.IQ_Gain.resize(sweep_param.PA_State_List.size());
         sweep_param.Purpose_List.push_back(sweep_param.Purpose_List.back());

         sweep_param.Segment_Length.resize(sweep_param.PA_State_List.size());
         sweep_param.RGI_List.back().push_back(sweep_param.XPT_Swp1_RGI_Delta_Ref_RGI);
         sweep_param.Bias_List.back().push_back(sweep_param.XPT_Swp1_RGI_Delta_Bias);
         sweep_param.IQ_Gain.back().push_back(sweep_param.XPT_Swp1_IQ_Gain);

         sweep_param.PA_Quiescent_Current.back().push_back(0);
         sweep_param.Env_Scale.back().push_back(sweep_param.XPT_Swp1_Env_Scale);
         sweep_param.Segment_Length.back().push_back(sweep_param.Segment_Length[0][0]);
      }

      //**************************************************//
      //    XPT2 parameters
      //**************************************************//
      if ((sweep_param.Sweep_Type == XPT_SWP2)
        || (sweep_param.Sweep_Type == INT_CAL_XPT_SWP2))
      {
        RFCal_Helper_Read_Param(pNode,"XPT_Swp2_Channel_List",sweep_param.XPT_Swp2_Channel_List);
        RFCal_Helper_Read_Param(pNode,"XPT_Swp2_EPT_PA_State",sweep_param.XPT_Swp2_EPT_PA_State);
        RFCal_Helper_Read_Param(pNode,"XPT_Swp2_ET_PA_State",sweep_param.XPT_Swp2_ET_PA_State);
        RFCal_Helper_Read_Param(pNode,"ET_Swp2_Meas_Pwr_List",sweep_param.ET_Swp2_Meas_Pwr_List);
        RFCal_Helper_Read_Param(pNode,"EPT_Swp2_Meas_Pwr_List",sweep_param.EPT_Swp2_Meas_Pwr_List);
        RFCal_Helper_Read_Param(pNode,"EPT_Swp2_Calc_Pwr_List",sweep_param.EPT_Swp2_Calc_Pwr_List);

        RFCal_Helper_Read_Param(pNode,"XPT_Swp2_Num_IQ_Samples",sweep_param.XPT_Swp2_Num_IQ_Samples);
        RFCal_Helper_Read_Param(pNode,"XPT_Swp2_Cap_Segment_Length",sweep_param.XPT_Swp2_Cap_Segment_Length);
        RFCal_Helper_Read_Param(pNode,"XPT_Swp2_MLine_Calc_Segment_Length",sweep_param.XPT_Swp2_MLine_Calc_Segment_Length);
        RFCal_Helper_Read_Param(pNode,"XPT_Swp2_IQ_Processing_Segment_Length",sweep_param.XPT_Swp2_IQ_Processing_Segment_Length,static_cast<dword>(220000));
        RFCal_Helper_Read_Param(pNode,"XPT_Swp2_MLine_Bias_Option",sweep_param.XPT_Swp2_MLine_Bias_Option);
        RFCal_Helper_Read_Param(pNode,"XPT_Swp2_Enable_Mline_Debug_Info",sweep_param.XPT_Swp2_Enable_Mline_Debug_Info);

        if (sweep_param.Segment_Length.size() == 0)
        {
          sweep_param.Segment_Length.resize(1);
          sweep_param.Segment_Length[0].push_back(sweep_param.XPT_Swp2_Cap_Segment_Length);
        }
      }
      //**************************************************//
      //    XPT3 parameters
      //**************************************************//
      if ((sweep_param.Sweep_Type == XPT_SWP3)
            || (sweep_param.Sweep_Type == INT_CAL_XPT_SWP3))
      {
        RFCal_Helper_Read_Param(pNode,"XPT_Swp3_Channel_List",sweep_param.XPT_Swp3_Channel_List);
        RFCal_Helper_Read_Param(pNode,"ET_Swp3_Meas_Pwr_List",sweep_param.ET_Swp3_Meas_Pwr_List);
        RFCal_Helper_Read_Param(pNode,"EPT_Swp3_Meas_Pwr_List",sweep_param.EPT_Swp3_Meas_Pwr_List);
        RFCal_Helper_Read_Param(pNode,"EPT_Swp3_Calc_Pwr_List",sweep_param.EPT_Swp3_Calc_Pwr_List);
        RFCal_Helper_Read_Param(pNode,"XPT_Swp3_Segment_Length",sweep_param.XPT_Swp3_Segment_Length);
        RFCal_Helper_Read_Param(pNode,"XPT_Swp3_Deviation_From_Exp_Power_Min_dBm100",sweep_param.XPT_Swp3_Deviation_From_Exp_Power_Min_dBm100);
        RFCal_Helper_Read_Param(pNode,"XPT_Swp3_Deviation_From_Exp_Power_Max_dBm100",sweep_param.XPT_Swp3_Deviation_From_Exp_Power_Max_dBm100);
        RFCal_Helper_Read_Param(pNode,"XPT_Swp3_Linearizer_Delta_Meas_Power_Min_dBm100",sweep_param.XPT_Swp3_Linearizer_Delta_Meas_Power_Min_dBm100);
        RFCal_Helper_Read_Param(pNode,"XPT_Swp3_Apply_Pin",sweep_param.XPT_Swp3_Apply_Pin);
        RFCal_Helper_Read_Param(pNode,"XPT_Swp3_Max_Expected_Power",sweep_param.XPT_Swp3_Max_Expected_Power,static_cast<double>(28.0));
      }
      //**************************************************//
      //    FreqComp parameters
      //**************************************************//

      RFCal_Helper_Read_Param(pNode,"Meas_Tx",sweep_param.Meas_Tx);
      RFCal_Helper_Read_Param(pNode,"Meas_Rx",sweep_param.Meas_Rx);
      RFCal_Helper_Read_Param(pNode,"Use_PilotOnlyWaveform",sweep_param.Use_PilotOnlyWaveform,static_cast<unsigned char>(0));

      RFCal_Helper_Read_Param(pNode,"Tx_Channel_List",sweep_param.Tx_Channel_List);
      RFCal_Helper_Read_Param(pNode,"RGI_Threshold_List",sweep_param.RGI_Threshold_List);
      RFCal_Helper_Read_Param(pNode,"Use_RGI_Pwr_List",sweep_param.Use_RGI_Pwr_List);
      RFCal_Helper_Read_Param(pNode,"FreqComp_Pwr_List",sweep_param.FreqComp_Pwr_List);
      RFCal_Helper_Read_Param(pNode,"APT_FreqComp_Offset_List",sweep_param.APT_FreqComp_Offset_List);
      RFCal_Helper_Read_Param(pNode,"XPT_Fcomp_Ref_Channel_List",sweep_param.XPT_Fcomp_Ref_Channel_List);
      RFCal_Helper_Read_Param(pNode,"XPT_Upper_Bound_Channel_List",sweep_param.XPT_Upper_Bound_Channel_List);
      RFCal_Helper_Read_Param(pNode,"ET_Fcomp_Meas_Pwr_List",sweep_param.ET_Fcomp_Meas_Pwr_List);
      RFCal_Helper_Read_Param(pNode,"EPT_Fcomp_Meas_Pwr_List",sweep_param.EPT_Fcomp_Meas_Pwr_List);
      RFCal_Helper_Read_Param(pNode,"EPT_Fcomp_Calc_Pwr_List",sweep_param.EPT_Fcomp_Calc_Pwr_List);
      RFCal_Helper_Read_Param(pNode,"Max_FreqComp_Pwr",sweep_param.Max_FreqComp_Pwr);
      RFCal_Helper_Read_Param(pNode,"Min_FreqComp_Pwr",sweep_param.Min_FreqComp_Pwr);

      if (sweep_param.Max_FreqComp_Pwr.size() != sweep_param.PA_State_List.size())
      {
        sweep_param.Max_FreqComp_Pwr.resize(sweep_param.PA_State_List.size());
        for (UINT i=0; i < sweep_param.PA_State_List.size();i++)
        {
          sweep_param.Max_FreqComp_Pwr[i] = 28;
        }
      }


      RFCal_Helper_Read_Param(pNode,"XPT_Fcomp_Deviation_From_Exp_Power_Min_dBm100",sweep_param.XPT_Fcomp_Deviation_From_Exp_Power_Min_dBm100,static_cast<int>(300));
      RFCal_Helper_Read_Param(pNode,"XPT_Fcomp_Deviation_From_Exp_Power_Max_dBm100",sweep_param.XPT_Fcomp_Deviation_From_Exp_Power_Max_dBm100,static_cast<int>(300));
        
      RFCal_Helper_Read_Param(pNode,"Rx_Channel_List",sweep_param.Rx_Channel_List);
      RFCal_Helper_Read_Param(pNode,"Rx_Actions_List",sweep_param.Rx_Actions_List);
      RFCal_Helper_Read_Param(pNode,"Rx_PM_List",sweep_param.Rx_PM_List);

      if (sweep_param.Rx_PM_List.size() == 0)
      {
         sweep_param.Rx_PM_List.push_back(0);
      }

        RFCal_Helper_Read_Param(pNode,"Split_Rx",sweep_param.Split_Rx);
        RFCal_Helper_Read_Param(pNode,"Enable_SCell_Only_Cal",sweep_param.Enable_SCell_Only_Cal);
        RFCal_Helper_Read_Param(pNode,"Rx_Calibration_Type",sweep_param.Rx_Calibration_Type,static_cast<unsigned short>(RFNV_RX_CAL_DATA_NV_TYPE_TDSCDMA_MAX));
        
        if (sweep_param.Rx_Calibration_Type == RFNV_RX_CAL_DATA_NV_TYPE_TDSCDMA_MAX)
        {
            //Set some default values to Calibration Type for each tech
            switch (demo_param.TECH)
            {
            case TECH_CDMA :
                sweep_param.Rx_Calibration_Type = RFNV_RX_CAL_NV_TYPE_CDMA_PM0;
                break;
            case TECH_TDSCDMA :
                sweep_param.Rx_Calibration_Type = RFNV_RX_CAL_DATA_NV_TYPE_TDSCDMA_LL;
                break;
            case TECH_WCDMA :
                sweep_param.Rx_Calibration_Type = RFNV_RX_CAL_DATA_NV_TYPE_WCDMA_SC;
                break;
            case TECH_LTE :
                sweep_param.Rx_Calibration_Type = RFNV_RX_CAL_DATA_NV_TYPE_LTE_BW_10p0;
                break;
            default :
                break;
            }
        }

      RFCal_Helper_Read_Param(pNode,"DL_Level_List",sweep_param.DL_Level_List);
      RFCal_Helper_Read_Param(pNode,"Rx_C0_Max_Limits",sweep_param.Rx_C0_Max_Limits);
      RFCal_Helper_Read_Param(pNode,"Rx_C0_Min_Limits",sweep_param.Rx_C0_Min_Limits);
      RFCal_Helper_Read_Param(pNode,"Rx_C1_Max_Limits",sweep_param.Rx_C1_Max_Limits);
      RFCal_Helper_Read_Param(pNode,"Rx_C1_Min_Limits",sweep_param.Rx_C1_Min_Limits);
      RFCal_Helper_Read_Param(pNode,"Rx_C2_Max_Limits",sweep_param.Rx_C2_Max_Limits);
      RFCal_Helper_Read_Param(pNode,"Rx_C2_Min_Limits",sweep_param.Rx_C2_Min_Limits);
      RFCal_Helper_Read_Param(pNode,"Rx_C3_Max_Limits",sweep_param.Rx_C3_Max_Limits);
      RFCal_Helper_Read_Param(pNode,"Rx_C3_Min_Limits",sweep_param.Rx_C3_Min_Limits);

      RFCal_Helper_Read_Param(pNode,"Meas_DC",sweep_param.Measure_DC);
      if (!sweep_param.Measure_DC)
      {
        RFCal_Helper_Read_Param(pNode,"Measure_DC",sweep_param.Measure_DC);
      }
      RFCal_Helper_Read_Param(pNode,"Measure_3C",sweep_param.Measure_3C);

      RFCal_Helper_Read_Param(pNode,"Meas_CA",sweep_param.Measure_CA);
      if (!sweep_param.Measure_CA)
      {
        RFCal_Helper_Read_Param(pNode,"Measure_CA",sweep_param.Measure_CA);
      }
      RFCal_Helper_Read_Param(pNode,"CA_PCell_RF_Mode",sweep_param.CA_PCell_RF_Mode);
      RFCal_Helper_Read_Param(pNode,"CA_PCell_Band",sweep_param.CA_PCell_Band);
      RFCal_Helper_Read_Param(pNode,"CA_PCell_Channel",sweep_param.CA_PCell_Channel);

      //**************************************************//
      //    Delay parameters
      //**************************************************//
      if (sweep_param.Sweep_Type == XPT_DELAY)
      {
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_Channel_List",sweep_param.XPT_Delay_Swp_Channel_List);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_Upper_Bound_Channel_List",sweep_param.XPT_Delay_Swp_Upper_Bound_Channel_List);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_Num_RB_List",sweep_param.XPT_Delay_Swp_Num_RB_List);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_Start_RB_List",sweep_param.XPT_Delay_Swp_Start_RB_List);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_BW_List",sweep_param.XPT_Delay_Swp_BW_List);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_5MHz_Offset_List",sweep_param.XPT_Delay_Swp_5MHz_Offset_List);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_10MHz_Offset_List",sweep_param.XPT_Delay_Swp_10MHz_Offset_List);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_20MHz_Offset_List",sweep_param.XPT_Delay_Swp_20MHz_Offset_List);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_PA_State",sweep_param.XPT_Delay_Swp_PA_State);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_RGI_Fudge_From_MLine",sweep_param.XPT_Delay_Swp_RGI_Fudge_From_MLine);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_Bias",sweep_param.XPT_Delay_Swp_Bias);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_IQ_Gain",sweep_param.XPT_Delay_Swp_IQ_Gain);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_Env_Scale",sweep_param.XPT_Delay_Swp_Env_Scale);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_PA_Quiescent_Current",sweep_param.XPT_Delay_Swp_PA_Quiescent_Current);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_Num_IQ_Samples",sweep_param.XPT_Delay_Swp_Num_IQ_Samples);
        RFCal_Helper_Read_Param(pNode,"XPT_Delay_Swp_Cap_Segment_Length",sweep_param.XPT_Delay_Swp_Cap_Segment_Length);

        for (UINT i = 0 ; i < sweep_param.Segment_Length.size(); i++)
        {
          for (UINT j = 0 ; j < sweep_param.Segment_Length[i].size(); j++)
          {
            sweep_param.Segment_Length[i][j] = sweep_param.XPT_Delay_Swp_Cap_Segment_Length;
          }
        }
       }

        //**************************************************//
        //      PINCAL parameters
        //**************************************************//
        if (sweep_param.Sweep_Type == XPT_PIN_CAL)
        {
            RFCal_Helper_Read_Param(pNode,"XPT_PinCal_Swp_XPT_Action_List",sweep_param.XPT_PinCal_Swp_XPT_Action_List);
            RFCal_Helper_Read_Param(pNode,"XPT_PinCal_Swp_Channel_List",sweep_param.XPT_PinCal_Swp_Channel_List);
            RFCal_Helper_Read_Param(pNode,"XPT_PinCal_Swp_Ref_Channel",sweep_param.XPT_PinCal_Swp_Ref_Channel);
            RFCal_Helper_Read_Param(pNode,"XPT_PinCal_Swp_PA_State_List",sweep_param.XPT_PinCal_Swp_PA_State_List);
            RFCal_Helper_Read_Param(pNode,"XPT_PinCal_Swp_IQ_Gain",sweep_param.XPT_PinCal_Swp_IQ_Gain);
            RFCal_Helper_Read_Param(pNode,"XPT_PinCal_Swp_Env_Scale",sweep_param.XPT_PinCal_Swp_Env_Scale);
            RFCal_Helper_Read_Param(pNode,"XPT_PinCal_Swp_Num_IQ_Samples",sweep_param.XPT_PinCal_Swp_Num_IQ_Samples);
            RFCal_Helper_Read_Param(pNode,"XPT_PinCal_Swp_Cap_Segment_Length",sweep_param.XPT_PinCal_Swp_Cap_Segment_Length);
            RFCal_Helper_Read_Param(pNode,"XPT_PinCal_Swp_Proc_Segment_Length",sweep_param.XPT_PinCal_Swp_Proc_Segment_Length);
            RFCal_Helper_Read_Param(pNode,"XPT_PinCal_Swp_Cap2Proc_Wait_Time",sweep_param.XPT_PinCal_Swp_Cap2Proc_Wait_Time);
            sweep_param.XPT_PinCal_Swp_XPT_RGI_List.resize(sweep_param.XPT_PinCal_Swp_XPT_Action_List.size(),0);
            sweep_param.XPT_PinCal_Swp_XPT_Bias_List.resize(sweep_param.XPT_PinCal_Swp_XPT_Action_List.size(),0);
            RFCal_Helper_Read_Param(pNode,"XPT_PinCal_Swp_RGI_Fudge_Factor",sweep_param.XPT_PinCal_Swp_RGI_Fudge_Factor,static_cast<byte>(3));
         if (sweep_param.Segment_Length.empty())
         {
            sweep_param.Segment_Length.resize(1);
            sweep_param.Segment_Length[0].push_back( sweep_param.XPT_PinCal_Swp_Cap_Segment_Length);

         } else if (sweep_param.Segment_Length[0].empty())
         {
            sweep_param.Segment_Length[0].push_back( sweep_param.XPT_PinCal_Swp_Cap_Segment_Length);
         } 
         else if (sweep_param.Segment_Length[0][0] == 0)
            {
                sweep_param.Segment_Length[0][0] = sweep_param.XPT_PinCal_Swp_Cap_Segment_Length;
            }
    //      vector <word>      XPT_PinCal_Swp_Peak_Compression_dB10_Max_Lim_List; // 
    //      vector <word>      XPT_PinCal_Swp_Pin_Variation_dB10_Max_Lim_List; // 
        }
        //**************************************************//
        //      Multi Band Rx Cal/Char
        //**************************************************//
        if (sweep_param.Sweep_Type == MULTI_BAND_RX_CHARACTERIZED_CAL)
        {
            RFCal_Helper_Read_Param(pNode,"Band_List",sweep_param.Band_List);
            RFCal_Helper_Read_Param(pNode,"RFMode_List",sweep_param.RFMode_List);
            RFCal_Helper_Read_Param(pNode,"NVMode_List",sweep_param.NVMode_List);
            RFCal_Helper_Read_Param(pNode,"NV_DC_Mode_List",sweep_param.NV_DC_Mode_List);
            RFCal_Helper_Read_Param(pNode,"RefChannel_List",sweep_param.RefChannel_List);
            RFCal_Helper_Read_Param(pNode,"CA_PCell_RF_Mode_List",sweep_param.CA_PCell_RF_Mode_List);
            RFCal_Helper_Read_Param(pNode,"CA_PCell_Band_List",sweep_param.CA_PCell_Band_List);
            RFCal_Helper_Read_Param(pNode,"CA_PCell_Channel_List",sweep_param.CA_PCell_Channel_List);
        }

        if (sweep_param.Segment_Length[0][0] == 0)
        {
            sweep_param.Segment_Length[0][0] = static_cast<dword>(10000);
        }
      pNode.Release();
   }
   pSweep_Param_Node_List.Release();
   pRoot_Node.Release();
   spXMLDoc.Release();
   CoUninitialize();

   // ***************************************************************//
   //    Common Parameters
   // ***************************************************************//
   int Swp1 = 0;
   param.TxDeviceFlag = 0;
   if (param.Tx_Rx_Split) // //If tx/rx split, ignore Tx_Device_to_Calibrate param provided in RFCal param, and set it equal to Master_Device_ID.
   {
      param.TxDeviceFlag = 0x01 << param.Swp[Swp1].Device_Selection.Master_Device_ID;
   }
   else
   {
   for( UINT i = 0; i< param.Swp[Swp1].Device_Selection.Tx_Device_To_Calibrate.size() ; i++ )
      param.TxDeviceFlag|= (0x01 << param.Swp[Swp1].Device_Selection.Tx_Device_To_Calibrate[i]);
   }
   param.RxDeviceFlag   = 0;
   for( UINT i=0; i< param.Swp[Swp1].Device_Selection.Rx_Device_To_Calibrate.size(); i++ )
      param.RxDeviceFlag |= (0x01 << param.Swp[Swp1].Device_Selection.Rx_Device_To_Calibrate[i]);  // 1 is primary only and 3 is Primary + Diversity

   for (UINT i= 0; i < iSweepSize; i++)
   {
      param.Swp[i].Ref_Channel = param.Swp[Swp1].Ref_Channel;  // This is QLIB demo only
   }

   // ***************************************************************//
   //    This is to handle new format of bias/iQ_gain/PA_Quiescent_Current
   //    Check the size of Bias/IQ_Gain/PA_Quiescent_Current/Env_Scale
   //    if the style <Bias_List>3700,3700,3700,3700</Bias_List>
   //    Fill out the same value to make the same size of RGI list
   // ***************************************************************//

   for (UINT i = 0; i < static_cast<UINT>(iSweepSize); i++)
   {
      // Recent param XML may not include IQ_Gain and Env_Scale if they are APT sweep.
      if (param.Swp[i].IQ_Gain.empty())
         param.Swp[i].IQ_Gain.resize(param.Swp[i].PA_State_List.size());

      if (param.Swp[i].Env_Scale.empty())
         param.Swp[i].Env_Scale.resize(param.Swp[i].PA_State_List.size());

      for (UINT paIndex = 0; paIndex < param.Swp[i].RGI_List.size(); paIndex++)
      {
         UINT iRGISize = param.Swp[i].RGI_List[paIndex].size();

         word lastBias = param.Swp[i].Bias_List[paIndex].back();
         UINT size = param.Swp[i].Bias_List[paIndex].size();
         for (UINT j = 0; j < (iRGISize - size); j++)
            param.Swp[i].Bias_List[paIndex].push_back(lastBias);

         byte lastIcq = param.Swp[i].PA_Quiescent_Current[paIndex].back();
         size = param.Swp[i].PA_Quiescent_Current[paIndex].size();
         for (UINT j = 0; j < (iRGISize - size); j++)
            param.Swp[i].PA_Quiescent_Current[paIndex].push_back(lastIcq);


         if (param.Swp[i].IQ_Gain[paIndex].empty())
            param.Swp[i].IQ_Gain[paIndex].push_back(static_cast<word>(0));
         if (param.Swp[i].Env_Scale[paIndex].empty())
            param.Swp[i].Env_Scale[paIndex].push_back(static_cast<word>(0));

         word lastIqGain = param.Swp[i].IQ_Gain[paIndex].back();
         size = param.Swp[i].IQ_Gain[paIndex].size();
         for (UINT j = 0; j < (iRGISize - size); j++)
            param.Swp[i].IQ_Gain[paIndex].push_back(lastIqGain);

         dword lastSegmentLength = param.Swp[i].Segment_Length[paIndex].back();
         size = param.Swp[i].Segment_Length[paIndex].size();
         for (UINT j = 0; j < (iRGISize - size); j++)
            param.Swp[i].Segment_Length[paIndex].push_back(lastSegmentLength);

         word lastEnv_Scale = param.Swp[i].Env_Scale[paIndex].back();
         size = param.Swp[i].Env_Scale[paIndex].size();
         for (UINT j = 0; j < (iRGISize - size); j++)
            param.Swp[i].Env_Scale[paIndex].push_back(lastEnv_Scale);
      }
   }

   //****************************************************************//
   //       This demo only, enable DC if demo_param.bDC_Enabled
   //       since DC cal parameters are not included default XML.
   //****************************************************************//
   if (demo_param.bDC_Enabled && param.NV_Generation_Params.Dual_Carr_NV_Mode == 0 && demo_param.TECH == 1)
   {
      for (unsigned int i = 0; i < iSweepSize; i++)
      {
         param.Swp[i].Measure_DC = true;
      }

      switch (demo_param.BAND)
      {
         case  1:
            param.NV_Generation_Params.Dual_Carr_NV_Mode  =  NV_MODE_WCDMA_IMT_DC; // 43;
            break;
         case  2:
            param.NV_Generation_Params.Dual_Carr_NV_Mode  =  NV_MODE_WCDMA_1900B_DC; // 78
            break;
         case  3:
            param.NV_Generation_Params.Dual_Carr_NV_Mode  =  NV_MODE_WCDMA_1800_DC; // 161
            break;
         case  4:
            param.NV_Generation_Params.Dual_Carr_NV_Mode  =  NV_MODE_WCDMA_B4_DC; // 162
            break;
         case  5:
            param.NV_Generation_Params.Dual_Carr_NV_Mode  =  NV_MODE_WCDMA_DC1; // 41
            break;
         case  8:
            param.NV_Generation_Params.Dual_Carr_NV_Mode  =  NV_MODE_WCDMA_900_DC; // 79
            break;
         case  11:
            param.NV_Generation_Params.Dual_Carr_NV_Mode  =  NV_MODE_WCDMA_1500_DC; // 160
            break;
         default:
            // Not supported band information
            break;
      }
   }

   //**************************************************//
   //       Calculate ROUGH number of segments
   //       This is qlib demo only. just to save memory/avoid fragmentation due to vector
   //**************************************************//
    demo_param.NumOfSegments.resize(iSweepSize);
   for (unsigned int iSwpIndex = 0; iSwpIndex < iSweepSize; iSwpIndex++)
   {
      int num = 0;
      Sweep_Params sweep_param = param.Swp[iSwpIndex];
      if (sweep_param.Config_Segment_Length > 100000)
      {
         sweep_param.Config_Segment_Length = 100000;
      }
      int configseg = sweep_param.Config_Segment_Length/sweep_param.Segment_Length[0][0];
      int tuningseg = sweep_param.Tuning_Segment_Length/sweep_param.Segment_Length[0][0];

      int AptSegNum = 0;
      for (UINT paIndex = 0; paIndex < sweep_param.PA_State_List.size(); paIndex++)
      {
         AptSegNum += sweep_param.RGI_List[paIndex].size();
      }
      AptSegNum = AptSegNum * 3;  //  Enough to cover ET segments

      switch (param.Swp[iSwpIndex].Sweep_Type)
      {
         case XPT_SWP2: // XPT_SWP2 = 6
            num +=   (sweep_param.XPT_Swp2_IQ_Processing_Segment_Length/sweep_param.Segment_Length[0][0]) *2+100;
            break;

         case APT_FREQCOMP: // APT_FREQCOMP = 4.
         case XPT_FREQCOMP: // XPT_FREQCOMP = 8
         case XPT_FREQCOMP_STAGGER_RXTX:
            num +=    ((tuningseg + sweep_param.ET_Fcomp_Meas_Pwr_List.size() + sweep_param.EPT_Fcomp_Meas_Pwr_List.size()+ sweep_param.Rx_Actions_List.size() * (sweep_param.Rx_PM_List.size() +1 ))*sweep_param.Tx_Channel_List.size())*3;
            break;

         case XPT_DELAY: // XPT_DELAY = 9
            num +=  tuningseg*sweep_param.XPT_Delay_Swp_Channel_List.size()*6;
            break;

         case MAX_BIAS_LINEARIZER: // MAX_BIAS_LINEARIZER = 0.
         case XPT_SWP1: // XPT_SWP1 = 2
         case XPT_SWP3: // XPT_SWP3 = 7
         case APT_LINEARIZER: //APT_LINEARIZER = 1.
            num = configseg + AptSegNum;
            break;
         default:
                break;
        }
        if (num == 0)
        {
            num = 500;
      }
      demo_param.NumOfSegments[iSwpIndex] = num;
   }
   return true;
}

void RFCal_helper_InsertNop( vector < cal_sweep_record > & Sweep, UINT & recordIndex, int NumNop, int div)
{
   if (div <= 0)
   {
      throw RFCal_Helper_Exception("divided by zero or negative",__FILE__,__LINE__);
   }
   if (div != 1)
   {
      NumNop = static_cast<int>(ceil(static_cast<double>(NumNop)/static_cast<double>(div)-1.0)); // Note  minus 1
   } // if div == 1, then, use the NumNop as is.
   if(NumNop==1 && (((Sweep[recordIndex].opcode.segLen)/1000) % 2==1) )
   {
      NumNop = 1;  // Use 2 for certain callbox to fix wcdma cal issue during tx freq comp. 
   }
   for (int i=0; i < NumNop; i++)
   {
      Sweep[recordIndex].isNopSeg = true;
      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
   }
}
unsigned char RFCal_Helper_setConfigTime_InsertNop( vector < cal_sweep_record > & Sweep, UINT & recordIndex, dword config_duration, word maxConfigSegmentLength)
{
   dword SegmentLengthBeforeUpdate = static_cast<dword>(Sweep[recordIndex].opcode.segLen);
   if (config_duration <= maxConfigSegmentLength)
   {
      Sweep[recordIndex].opcode.segLen = static_cast<word>(config_duration);
      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
   }
   else
   {
      Sweep[recordIndex].opcode.segLen = static_cast<word>(maxConfigSegmentLength);


      config_duration -= maxConfigSegmentLength;
//      config_duration += static_cast<dword>(Sweep[recordIndex].opcode.segLen); // because RFCal_helper_InsertNop decrease 1. 
//      RFCal_helper_InsertNop(Sweep,recordIndex,config_duration,Sweep[recordIndex].opcode.segLen);


        if (config_duration < SegmentLengthBeforeUpdate)
      {
        // To make sure that all the segment duration is more than <segment_length>
            config_duration = SegmentLengthBeforeUpdate;
      }
        RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
        Sweep[recordIndex].isNopSeg = true;
        // Recursively call this function.
//      RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,config_duration,maxConfigSegmentLength);
//      RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,config_duration,2000);
        RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,config_duration, static_cast<word>(SegmentLengthBeforeUpdate));
        
   }
   return true;
}
long RFCal_Helper_ConvertULchtoDLch(int tech, int band, long ulChannel)
{
   int offset = 0;
   switch (tech)
   {
      case 1:
      {
         if (band == 1)
         {
            offset = 950;
         }
         else if ((band == 2) || (band == 19))
         {
            offset = 400;
         }
         else if ((band >=3) && (band <=10))
         {
            offset = 225;
         }
         else if ((band == 11) )
         {
            offset = 225;
         }
      }
      break;
      case 3:
      {
         if ((band >=1) && (band <=28))
         {
            offset = -18000;
         }
         else if ((band >=30) && (band <=31))
         {
            offset = -17890;
         }
         else
         {
            offset = 0;  // B29 is DL only/
         }
      }
      break;
      case 0: // CDMA
      case 2: // GSM
      case 4: // TDSCDMA
      default :
         // UL = DL
         break;
   }
   return ulChannel+offset;
}

long CalculateRxAGCValue(byte Tech, double RxPower, double DynamicRange)
{
   double RxMin = 0.0; // LTE
   double offset = 512;
   switch (Tech)
   {
      case 0:  // CDMA
         RxMin = -115.0;
         break;
      case 3:  // LTE
         RxMin = 0.0;
         offset = 0.0;
         break;
      case 1:  // WCDMA
      default:
         RxMin = -106.0;
         break;
   }
   return static_cast<long>( (1023/DynamicRange)*(RxPower - RxMin) - offset );
}

void Test_RFCal_ESC_InitRecord(cal_sweep_record* tmpSweep,  int swpType,   int subSwpType,   int txPurpose, byte tech, word band, int txWaveform, int channel,int txSweepTime, int startRB=22, int numRBs=6, int Tx_BW=3, int Rx_BW=3)
{
   /* ESC Parameters */
   tmpSweep->swpType = swpType;
   tmpSweep->subSwpType = subSwpType;
   tmpSweep->opcode.segLen = txSweepTime;

   tmpSweep->opcode.varLenOpcode.opCodeGrp.opCode = 0;
   tmpSweep->opcode.varLenOpcode.opCodeLen = 0;

   // tmpSweep->txPurpose = txPurpose;
   tmpSweep->txPurpose = -1;

   tmpSweep->version = 0;
   tmpSweep->isCfgSeg = false;
   tmpSweep->isNopSeg = false;
   tmpSweep->measSeg = false;
   tmpSweep->resExpected = false;

   /* Radio Setup */
   tmpSweep->radio_cfg.version   = 0;
   tmpSweep->radio_cfg.debugMode = 0;
   tmpSweep->radio_cfg.flag = 0;
   tmpSweep->radio_cfg.device.masterDev = 0;
   tmpSweep->radio_cfg.device.slaveDev = 1;
   tmpSweep->radio_cfg.device.masterDevState = 0;           // only need for radio_setup and retune segment
   tmpSweep->radio_cfg.device.slaveDevState = 0;
   tmpSweep->radio_cfg.tech = (byte)tech; // CDMA:0 WCDMA:1 LTE:3
   tmpSweep->radio_cfg.band = (word)band;

   tmpSweep->radio_cfg.chan.numChans = 1;
   tmpSweep->radio_cfg.chan.chanInfo[0].ulChan = channel;
   tmpSweep->radio_cfg.chan.chanInfo[0].rxBW = Rx_BW;
   tmpSweep->radio_cfg.chan.chanInfo[0].txBW = Tx_BW;

   tmpSweep->radio_cfg.wfm.txWaveform = (byte)txWaveform;
   tmpSweep->radio_cfg.wfm.numRBs = (byte)numRBs;
   tmpSweep->radio_cfg.wfm.startRB  = (byte)startRB;
   tmpSweep->radio_cfg.wfm.eulConfig = (word)0;

   // Tx Variable Length
   tmpSweep->txParams.txDevice   = 1;

   for( int dev = 0; dev < MAX_NUM_DEVICES; dev++ )
   {
      tmpSweep->txParams.txParams[dev].paState = 0xFF;
   }

   // rxParams
   tmpSweep->rxParams.rxActions = (word) 0xFFFF;
   // Data Set Data Operations to false this is for ET/EPT
   tmpSweep->data.doDataOp = false;
   tmpSweep->data.nomDPDTbl.doDPDLoadOp = false;
   tmpSweep->data.rgiVsPwr.doRgiVsPwrOp = false;
   tmpSweep->data.mptCalc.doMPtCalcOp = false;
   tmpSweep->data.delayValProc.doDelayValProcOp = false;

   //rxMeas Rx Variable Length
   for( UINT dev = 0; dev < MAX_NUM_DEVICES; dev++ )
   {
      tmpSweep->rxMeas.actions[dev].rx_meas_action = 0xFF;
   }

   // TxMeas. Device Mask
   tmpSweep->txMeasParams.txMeasFlag = 1;
   tmpSweep->sync.syncEvent = 0xFFFF;

    // Sequence Build (new options)
   tmpSweep->seqBuild.seqType = SEQ_TYPE_UNDEFINED;
    tmpSweep->seqExec.seqType = SEQ_TYPE_UNDEFINED;

}

void Find_Y_of_Next_Highest_X( const map< double, unsigned long >& xy_map,
                               const double& x, unsigned long& y)
{
   if ( ! xy_map.empty() )
   {
      if(x <= xy_map.begin()->first)
         y = xy_map.begin()->second;
      else if(x >= xy_map.rbegin()->first)
         y = xy_map.rbegin()->second;
      else
      {
         map<double,unsigned long>::const_iterator xy_iter = xy_map.begin();

         for(; xy_iter!=xy_map.end(); ++xy_iter)
         {
            if(xy_iter->first >= x)
            {
               y = xy_iter->second;
               break;
            }
         }
      }
   }
}
template<typename Tx, typename Ty> void RFCal_Helper_GetEstimatedY_LeastSquaresMethod( const map< Tx, Ty >& xy_map,
   const Tx& x, Ty& y)
{
   double A = 0.0, B = 0.0;
   RFCal_Helper_GetEstimatedY_LeastSquaresMethod(xy_map,x,y,A,B);
}
template<typename Tx, typename Ty> void RFCal_Helper_GetEstimatedY_LeastSquaresMethod( const map< Tx, Ty >& xy_map,
   const Tx& x, Ty& y, double & a, double & b)
{
   Ty A_Top,A_Bottom;
   Ty B_Top,B_Bottom;
   A_Top = static_cast<Ty>(0);
   A_Bottom = static_cast<Ty>(0);
   B_Top = static_cast<Ty>(0);
   B_Bottom = static_cast<Ty>(0);

   Ty size = static_cast<Ty>(xy_map.size());

   Ty SumX = static_cast<Ty>(0);
   Ty SumY = static_cast<Ty>(0);
   Ty SumX_Power_Two =  static_cast<Ty>(0);
   Ty SumXY = static_cast<Ty>(0);
   
   for (map<Tx, Ty>::const_iterator it = xy_map.begin();it != xy_map.end();it++)
   {
     Ty X = static_cast<Ty>(it->first);
     Ty Y = static_cast<Ty>(it->second);

     SumXY += ( X * Y);
     SumX += X;
     SumY += Y;
     SumX_Power_Two += ( X * X);
   }
   A_Top = size*SumXY - SumX*SumY;
   A_Bottom = size* SumX_Power_Two-SumX*SumX;

   B_Top = SumX_Power_Two*SumY - SumXY*SumX;
 
   B_Bottom = A_Bottom; 

   a = (A_Top/A_Bottom);

   b = (B_Top/B_Bottom);

   y = a*x + b;

}
template<typename Tx, typename Ty> void RFCal_Helper_GetEstimatedY_LeastSquaresMethod_quadratic( const map< Tx, Ty >& xy_map,
   const Tx& x, Ty& y, double & a, double & b, double & c)
{
   Ty A_Top,A_Bottom;
   Ty B_Top,B_Bottom;
   Ty C_Top,C_Bottom;
 
   A_Top = static_cast<Ty>(0);
   A_Bottom = static_cast<Ty>(0);
   B_Top = static_cast<Ty>(0);
   B_Bottom = static_cast<Ty>(0);
   C_Top = static_cast<Ty>(0);
   C_Bottom = static_cast<Ty>(0);

   Ty size = static_cast<Ty>(xy_map.size());

   Ty SumX = static_cast<Ty>(0);
   Ty SumY = static_cast<Ty>(0);
   Ty SumX2 =  static_cast<Ty>(0);
   Ty SumX3 =  static_cast<Ty>(0);
   Ty SumX4 =  static_cast<Ty>(0);
   
   Ty SumXY = static_cast<Ty>(0);
   Ty SumX2_Y = static_cast<Ty>(0);
   

   for (map<Tx, Ty>::const_iterator it = xy_map.begin();it != xy_map.end();it++)
   {
     Ty X = static_cast<Ty>(it->first);
     Ty Y = static_cast<Ty>(it->second);

     SumXY += ( X * Y);
     SumX += X;
     SumY += Y;
     SumX2 += ( X * X);
     SumX3 += ( X * X * X);
     SumX4 += ( X * X * X * X);
     SumX2_Y += ( X *  X * Y);
   }

   A_Top = size * SumX2* SumX2_Y - SumX * SumX * SumX2_Y + SumX * SumX2 * SumXY - size * SumX3 * SumXY + SumX * SumX3 * SumY - SumX2 * SumX2 * SumY;
   A_Bottom = 2.0 * SumX * SumX2 * SumX3 + size * SumX2 * SumX4 - SumX * SumX * SumX4 - size * SumX3 * SumX3 - SumX2* SumX2 * SumX2;

   B_Top = SumX * SumX2 * SumX2_Y - size * SumX3 * SumX2_Y + size * SumX4 * SumXY - SumX2 * SumX2 * SumXY + SumX2 * SumX3 * SumY - SumX * SumX4 * SumY;
   B_Bottom = A_Bottom;

   C_Top = -1.0 * SumX2 * SumX2 * SumX2_Y + SumX * SumX3 * SumX2_Y - SumX * SumX4 * SumXY + SumX2 * SumX3 * SumXY - SumX3 * SumX3 * SumY + SumX2 * SumX4 * SumY;
   C_Bottom = A_Bottom;

   a = (A_Top/A_Bottom);

   b = (B_Top/B_Bottom);

   c = (C_Top/C_Bottom);

   y = a*(x*x)+b*x + c;



}
template<typename Tx, typename Ty> void Find_Interpolated_Y( const map< Tx, Ty >& xy_map,
                                                             const Tx& x, Ty& y)
{
   if(x <= xy_map.begin()->first)
      y = xy_map.begin()->second;
   else if(x >= xy_map.rbegin()->first)
      y = xy_map.rbegin()->second;
   else
   {
      map<Tx,Ty>::const_iterator xy_iter = xy_map.begin();

      Tx x1 = xy_map.begin()->first;
      Ty y1= xy_map.begin()->second;
      ++xy_iter;

      for(; xy_iter!=xy_map.end(); ++xy_iter)
      {
         if(xy_iter->first > x) break;
         x1 = xy_iter->first;
         y1 = xy_iter->second;
      }

      Tx x2 = xy_iter->first;
      Ty y2 = xy_iter->second;

      y = (Ty)(y1+(x-x1)*(y2-y1)/(x2-x1));
   }
}

int Get_Closet_Input_From_Matrix_WithTargetPwr(double *pwrList, int *inputList, int num, double targetPwr)
{
   double pwr_delta_min = 999.0;
   int targetIndex = 0;
   for (int index = 0; index < num; index++)
   {
      double pwr =  *(pwrList + index) ;
      if (pwr_delta_min > fabs( pwr - targetPwr))
      {
         pwr_delta_min = fabs( pwr - targetPwr);
         targetIndex = index;
      }
   }
   return *(inputList + targetIndex);
}
unsigned short getNVMode(byte tech, word band)
{
   unsigned short NVMode = 0;
   if (tech == 3)
   {
      // LTE
      switch (band)
      {
         case 1:
            NVMode = NV_MODE_LTE_B1;
            break;
         default:
            break;
      }
   }
   else if (tech == 1)
   {
      // WCDMA
      switch (band)
      {
         case 1:
            NVMode = NV_MODE_WCDMA_IMT;
            break;
         default:
            break;
      }
   }
   else if (tech == 0)
   {
      // CDMA
      switch (band)
      {
         case 0:
            NVMode = NV_MODE_BC0;
            break;
         case 1:
            NVMode = NV_MODE_BC1;
            break;
         default:
            break;
      }
   }
   return NVMode;
}

unsigned short getDC_NVmode(unsigned short BAND)
{
   unsigned short DC_NVmode = 255;

   switch (BAND)
   {
      case  1:
         DC_NVmode =  NV_MODE_WCDMA_IMT_DC; // 43;
         break;
      case  2:
         DC_NVmode =  NV_MODE_WCDMA_1900B_DC; // 78
         break;
      case  3:
         DC_NVmode =  NV_MODE_WCDMA_1800_DC; // 161
         break;
      case  4:
         DC_NVmode =  NV_MODE_WCDMA_B4_DC; // 162
         break;
      case  5:
         DC_NVmode =  NV_MODE_WCDMA_DC1; // 41
         break;
      case  8:
         DC_NVmode =  NV_MODE_WCDMA_900_DC; // 79
         break;
      case  11:
         DC_NVmode =  NV_MODE_WCDMA_1500_DC; // 160
         break;
      default:
         // Not supported band information
         break;
   }
   return DC_NVmode;
}
template<typename Tx> unsigned char RFCal_Helper_EvaluateResult(Tx iResult, Tx iMaxLimit, Tx iMinLimit, unsigned long iOption)
{
    unsigned char bStatus = true;
    if ((iResult < iMaxLimit) &&  (iResult > iMinLimit))
    {
        bStatus = true;
    }
    return bStatus;
}
unsigned char RFCal_Helper_WriteRefCalTxData_V2_ToXML(const string & filename, const map <RFCal_Helper_Tx_Params, double> & mapTxParmasToPwr)
{
    HRESULT hr = S_OK; 
    //**************************************************//
    //          Parameters 
    //**************************************************//
    MSXML2::IXMLDOMDocument2Ptr xmlDoc; 

    MSXML2::IXMLDOMNodePtr pNode;
    MSXML2::IXMLDOMNodePtr pDataNode;
    
    MSXML2::IXMLDOMElementPtr element;
    MSXML2::IXMLDOMElementPtr Parentelement;
    _variant_t varValue;
    stringstream stXPATH;

    set < pair < byte, byte > > rfMode_Dev;

    for (map <RFCal_Helper_Tx_Params, double>::const_iterator it = mapTxParmasToPwr.begin(); it != mapTxParmasToPwr.end(); it++)
    {
        rfMode_Dev.insert( make_pair(it->first.RFMode,it->first.device));
    }
    
    //**************************************************//
    //          Create Processing Intruction 
    //**************************************************//

    CoInitialize(NULL); // Initialize COM
    
    xmlDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
    
    if ( xmlDoc->load(filename.c_str()) != VARIANT_TRUE)
    {
        MSXML2::IXMLDOMProcessingInstructionPtr pPI = NULL;
        xmlDoc.CreateInstance("Msxml2.DOMDocument.4.0");
        pPI = xmlDoc->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
        varValue.vt = VT_NULL;
        xmlDoc->insertBefore(pPI, varValue);

        //**************************************************//
        //          Create  Ref Cal Root node
        //**************************************************//

        xmlDoc->raw_createElement((_bstr_t)("RefCal_Data_V2"),&element);
        xmlDoc->raw_appendChild(element,NULL);
    }
    pNode = xmlDoc->selectSingleNode((_bstr_t)"/RefCal_Data_V2"); 

    if (pNode == NULL)
    {
        xmlDoc.Release();
        CoUninitialize();
        throw RFCal_Helper_Exception("RefCal_Data node not found.", __FILE__, __LINE__);
    }
    
    for ( set < pair < byte, byte > >::iterator itrfmode = rfMode_Dev.begin(); itrfmode != rfMode_Dev.end(); itrfmode++)
    {
        byte rfMode = itrfmode->first;
        byte device = itrfmode->second;

        stXPATH << "/RefCal_Data_V2/Tx_RefCal_Data_V2[@rf_mode='" << static_cast<int>(rfMode) << "' and @device='" << static_cast<int>(device) << "']";

        pDataNode = RFCal_Helper_GetSingleXMLNode(xmlDoc,stXPATH.str()); 
    
        if (!pDataNode)
        {
            //**************************************************//
         //         Add Tx_RefCal_Data_V2 for the rf_mode
            //**************************************************//

            pNode = xmlDoc->selectSingleNode((_bstr_t)"RefCal_Data_V2"); 
            
            if (!pNode)
            {
                xmlDoc.Release();
                CoUninitialize();
                throw RFCal_Helper_Exception("RefCal_Data_V2 node not found.", __FILE__, __LINE__);
            }

            Parentelement = pNode;
            element = xmlDoc->createElement((_bstr_t)"Tx_RefCal_Data_V2");

            element->setAttribute((_bstr_t)"rf_mode",static_cast<variant_t>(rfMode));
            element->setAttribute((_bstr_t)"device",static_cast<variant_t>(device));

            Parentelement->appendChild(element);
        
            pDataNode = xmlDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str()); 
        }
      
      int bw = 0;
      MSXML2::IXMLDOMNodePtr pTmpDataNode = pDataNode;
      for (byte bwIdx = 0; bwIdx < 8; bwIdx++)
      {
      
         pDataNode = pTmpDataNode;
         MSXML2::IXMLDOMNodePtr pBWNode = NULL;
        for (map <RFCal_Helper_Tx_Params, double>::const_iterator it = mapTxParmasToPwr.begin(); it != mapTxParmasToPwr.end(); it++)
        {
            if (it->first.RFMode != rfMode || it->first.device != device || it->first.bw != bwIdx) { continue; }

            if (pBWNode == NULL)
            {
               stXPATH.str("");
               stXPATH.clear();
               stXPATH << "Tx_RefCal_Data_Bandwidth[@bw_mode='" << static_cast<int>(bwIdx) << "']";
               pBWNode = pDataNode->selectSingleNode((_bstr_t)stXPATH.str().c_str()); 
               if (!pBWNode)
               {
                  //**************************************************//
                  //         Add Tx_RefCal_Data_Bandwidth for the bw
                  //**************************************************//

                  Parentelement = pDataNode;
                  element = xmlDoc->createElement((_bstr_t)"Tx_RefCal_Data_Bandwidth");
                  element->setAttribute((_bstr_t)"bw_mode",static_cast<variant_t>(bwIdx));
                  Parentelement->appendChild(element);
      
                  pBWNode = pDataNode->selectSingleNode((_bstr_t)stXPATH.str().c_str()); 
               }
            }
            stXPATH.str("");
            stXPATH.clear();
            stXPATH << "Tx_RefCal_Data_Table[@channel='" << static_cast<int>(it->first.Channel) 
                << "' and @fbgainstate='" << static_cast<int>(it->first.FBGainState) << "']";
        
            MSXML2::IXMLDOMNodePtr pTableNode = NULL;
            pTableNode = pBWNode->selectSingleNode((_bstr_t)stXPATH.str().c_str()); 
            if (pTableNode == NULL)
            {       
               Parentelement = pBWNode;
                element = xmlDoc->createElement((_bstr_t)"Tx_RefCal_Data_Table");
                element->setAttribute((_bstr_t)"fbgainstate",static_cast<variant_t>(it->first.FBGainState));
                element->setAttribute((_bstr_t)"channel",static_cast<variant_t>(it->first.Channel));

                Parentelement->appendChild(element);
               pTableNode = pBWNode->selectSingleNode((_bstr_t)stXPATH.str().c_str()); 
            }

            
            MSXML2::IXMLDOMNodePtr pRecordNode = NULL;
            stringstream ssDataRecord;
            ssDataRecord << "Tx_RefCal_Data_Record[@uBias='" << (int)it->first.uBias << "' and @rxScale='" << static_cast<int>(it->first.RxScale) << "']";
            pRecordNode = ::RFCal_Helper_GetSingleXMLNode(pTableNode,ssDataRecord.str());
            
            if (pRecordNode)
            {
                pTableNode->removeChild(pRecordNode);
            }
    
            Parentelement = pTableNode;
            
            element = xmlDoc->createElement((_bstr_t)"Tx_RefCal_Data_Record");
            element->setAttribute((_bstr_t)"pwr",static_cast<variant_t>(it->second));
            element->setAttribute((_bstr_t)"gain",static_cast<variant_t>(it->first.FBgain));
            element->setAttribute((_bstr_t)"uBias",static_cast<variant_t>(it->first.uBias));

            element->setAttribute((_bstr_t)"rxScale",static_cast<variant_t>(it->first.RxScale));

            Parentelement->appendChild(element);

          if (pTableNode != NULL)
          {
             pTableNode.Release();
          }
         }
         if (pBWNode != NULL)
         {
            pBWNode.Release();
         }
         pBWNode = NULL;
       }
   }
   if (pDataNode != NULL)
   {
      pDataNode.Release();
   }
   if (filename.length() > 0)
   {
        hr = xmlDoc->save((_bstr_t)filename.c_str());
   }
   else
   {
        hr = xmlDoc->save((_bstr_t)"c:\\temp\\ref_cal_data.xml");
   }
    xmlDoc.Release();
    CoUninitialize();
    return true;
    return true;
}
unsigned char RFCal_Helper_WriteRefCalTxDataToXML(const string & filename, const Tx_RefCal_Data & data)
{
   HRESULT hr = S_OK;
   //**************************************************//
   //       Parameters
   //**************************************************//
   MSXML2::IXMLDOMNodePtr pNode;
   MSXML2::IXMLDOMNodePtr pDataNode;

   MSXML2::IXMLDOMElementPtr element;
   MSXML2::IXMLDOMElementPtr Parentelement;
   _variant_t varValue;
   stringstream stXPATH;

   //**************************************************//
   //       Create Processing Intruction
   //**************************************************//

   CoInitialize(NULL); // Initialize COM

   MSXML2::IXMLDOMDocument2Ptr xmlDoc;
   xmlDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));

   if ( xmlDoc->load(filename.c_str()) != VARIANT_TRUE)
   {
      MSXML2::IXMLDOMProcessingInstructionPtr pPI = NULL;
      xmlDoc.CreateInstance("Msxml2.DOMDocument.4.0");
      pPI = xmlDoc->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
      varValue.vt = VT_NULL;
      xmlDoc->insertBefore(pPI, varValue);

      //**************************************************//
      //       Create  Ref Cal Root node
      //**************************************************//

      xmlDoc->raw_createElement((_bstr_t)("RefCal_Data"),&element);
      xmlDoc->raw_appendChild(element,NULL);
   }
   pNode = xmlDoc->selectSingleNode((_bstr_t)"RefCal_Data");

   if (pNode == NULL)
   {
      xmlDoc.Release();
      CoUninitialize();
      throw RFCal_Helper_Exception("RefCal_Data node not found.", __FILE__, __LINE__);
   }

   stXPATH << "/RefCal_Data/Tx_RefCal_Data_V1[@rf_mode='" << static_cast<int>(data.rf_mode) << "' and @device='" << static_cast<int>(data.device) << "']";

   pDataNode = xmlDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());

   if (pDataNode == NULL)
   {
      //**************************************************//
      //       Add Tx_RefCal_Data_V1 for the rf_mode
      //**************************************************//

      pNode = xmlDoc->selectSingleNode((_bstr_t)"RefCal_Data");

      Parentelement = pNode;
      element = xmlDoc->createElement((_bstr_t)"Tx_RefCal_Data_V1");

      element->setAttribute((_bstr_t)"rf_mode",static_cast<variant_t>(data.rf_mode));
      element->setAttribute((_bstr_t)"device",static_cast<variant_t>(data.device));

      Parentelement->appendChild(element);

      pDataNode = xmlDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());
   }

   for (UINT iTableIdx=0; iTableIdx < data.table.size(); iTableIdx++)
   {
      stXPATH.str("");
      stXPATH.clear();
      stXPATH << "Tx_RefCal_Data_Table[@pa='" << static_cast<int>(data.table[iTableIdx].pa)
              << "' and @channel='" << static_cast<int>(data.table[iTableIdx].channel) << "']";

      MSXML2::IXMLDOMNodePtr pTableNode;
      pTableNode = pDataNode->selectSingleNode((_bstr_t)stXPATH.str().c_str());
      if (pTableNode == NULL)
      {
         Parentelement = pDataNode;
         element = xmlDoc->createElement((_bstr_t)"Tx_RefCal_Data_Table");
         element->setAttribute((_bstr_t)"channel",static_cast<variant_t>(data.table[iTableIdx].channel));
         element->setAttribute((_bstr_t)"pa",static_cast<variant_t>(data.table[iTableIdx].pa));

         Parentelement->appendChild(element);
         pTableNode = pDataNode->selectSingleNode((_bstr_t)stXPATH.str().c_str());
      }

      for (unsigned int iRecordIdx=0; iRecordIdx < data.table[iTableIdx].record.size(); iRecordIdx++)
      {
         MSXML2::IXMLDOMNodeListPtr pRecordNodes;
         stringstream ss;
         ss << "Tx_RefCal_Data_Record[@rgi='" << (int)data.table[iTableIdx].record[iRecordIdx].rgi << "']";
         pRecordNodes = pTableNode->selectNodes((_bstr_t)ss.str().c_str());
         for (long i=0; i < pRecordNodes->length; i++)
         {
            MSXML2::IXMLDOMNodePtr pRecordNode = pRecordNodes->nextNode();
            MSXML2::IXMLDOMNodePtr pParent = pRecordNode->parentNode;
            pParent->removeChild(pRecordNode);
         }
         //pRecordNode->

         Parentelement = pTableNode;

         element = xmlDoc->createElement((_bstr_t)"Tx_RefCal_Data_Record");

         element->setAttribute((_bstr_t)"rgi",static_cast<variant_t>(data.table[iTableIdx].record[iRecordIdx].rgi));
         element->setAttribute((_bstr_t)"pwr",static_cast<variant_t>(data.table[iTableIdx].record[iRecordIdx].power));
         element->setAttribute((_bstr_t)"offset",static_cast<variant_t>(data.table[iTableIdx].record[iRecordIdx].offset));

         Parentelement->appendChild(element);
      }
      if (pTableNode != NULL)
      {
         pTableNode.Release();
      }
   }
   if (pDataNode != NULL)
   {
      pDataNode.Release();
   }
   if (filename.length() > 0)
   {
   hr = xmlDoc->save((_bstr_t)filename.c_str());
   }
   else
   {
        hr = xmlDoc->save((_bstr_t)"c:\\temp\\ref_cal_data.xml");
   }
   xmlDoc.Release();
   CoUninitialize();
   return true;
}
unsigned char RFCal_Helper_ESC_ET_RefCal_Tx_Data_Assignment_XML(QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param)
{
   //**************************************************//
   //       Check this is Tx Internal Cal sweep
   //**************************************************//
   if (cal_param.Swp[0].Sweep_Type != INT_CAL_XPT_SWP1
         && cal_param.Swp[0].Sweep_Type != INT_CAL_MAX_BIAS_LINEARIZER)
   {
      return true;
   }
    if (cal_param.Swp[0].Fb_Rx_Gain_Cal_Mode != 0) { return true; }
   //**************************************************//
   //       Parameters
   //**************************************************//
   MSXML2::IXMLDOMDocumentPtr spXMLDoc;
   MSXML2::IXMLDOMNodePtr pNode;
   MSXML2::IXMLDOMElementPtr element;
   _variant_t varValue;

   stringstream stXPATH;
   MSXML2::IXMLDOMNodeListPtr TableList;
   MSXML2::IXMLDOMNodeListPtr RecordList;

   //**************************************************//
   //       Load XML and get a node with the rf_mode
   //**************************************************//
   CoInitialize(NULL); // Initialize COM
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
   spXMLDoc->load(demo_param.stRefCalOffset_filename.c_str());
   if (spXMLDoc == NULL)
   {
      CoUninitialize();
      return false;
      //throw RFCal_Helper_Exception("XML file not found.",__FILE__,__LINE__);
   }
   int rf_mode = cal_param.RF_Mode;
   int device  = 0;
   stXPATH << "/RefCal_Data/Tx_RefCal_Data_V1[@rf_mode='" << rf_mode << "' and @device='" << device << "']";
   pNode = spXMLDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());
   if (pNode == NULL)
   {
      CoUninitialize();
      stXPATH << " Not Found.";
      return false;
      //throw RFCal_Helper_Exception(stXPATH.str(),__FILE__,__LINE__);
   }

   int pa = 0, channel = 0, offset = 0,pwr = 0, rgi = 0;
   long tableCount, recordCount;

   RFCal_Helper_Tx_Params Tx_Params;
   Tx_Params.Tech = demo_param.TECH;
   Tx_Params.Band = demo_param.BAND;

   //**************************************************//
   //       Read each of Tx_RefCal_Data_Table
   //**************************************************//
   TableList = pNode->GetchildNodes();
   HRESULT hr = S_OK;
   hr = TableList->get_length(&tableCount);

   for(long i=0; i<tableCount; i++)
   {
      hr= TableList->get_item(i,&pNode);

      element = pNode;
      varValue = element->getAttribute(L"pa");
      Tx_Params.PaState = static_cast<byte>((int)varValue);
      varValue = element->getAttribute(L"channel");
      Tx_Params.Channel = static_cast<word>((int)varValue);

      //**************************************************//
      //       Read each of Tx_RefCal_Data_Record
      //**************************************************//

      RecordList = pNode->GetchildNodes();
      hr = RecordList->get_length(&recordCount);

      //datatable.record;
      int offset;
      double power;

      for (long j=0; j<recordCount; j++)
      {
         hr= RecordList->get_item(j,&pNode);
         element = pNode;
         varValue = element->getAttribute(L"offset");
         {
            offset = (int)varValue;
         }
         varValue = element->getAttribute(L"pwr");
         {
            power = static_cast<double>(varValue);
         }
         varValue = element->getAttribute(L"rgi");
         {
            Tx_Params.RGI = static_cast<byte>((int)varValue);
         }

         cal_param.TxParamsToOffset[Tx_Params] = offset;
         cal_param.TxParamsToPower[Tx_Params] = power;
      }
   }
   pNode.Release();
   spXMLDoc.Release();

   CoUninitialize();
   return true;
}
unsigned char RFCal_Helper_ESC_ET_OpCodeConfigureRxLM(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, vector < cal_sweep_record > & Sweep, UINT & recordIndex, unsigned char fbEnabled)
{
   int   fbDev               =   static_cast<int>(sweep_param.Device_Selection.Feedback_Device_ID);
   const byte     TECH_CDMA   =  0;
   //*****************************************************************************************//
   //  OpCode 0x10000: FBSetup   Feedback Action:  4  ( | Bit 2 = Configure RxLM for FB AGC read )
   //*****************************************************************************************//
   if (demo_param.TECH != TECH_CDMA 
      || (demo_param.TECH == TECH_CDMA 
      && (sweep_param.Fb_Rx_Gain_Cal_Mode != FBRX_AGC_GAIN
      && sweep_param.Fb_Rx_Gain_Cal_Mode != FBRX_IQ_GAIN)))
   {
      byte pDev = RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag);
      RFCal_Helper_setOpCode_FB_Setup(Sweep[recordIndex],   pDev, fbDev,   ESC_FEEDBACK_SETUP_ACTION_CONFIG_RXLM_FOR_FBAGC /* fbAction */,   fbEnabled ,0,0xFF);
   }
   else     // CDMA
   {
      RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex],
                                        sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID,
            MASTER_RADIO_STATE_NO_CHANGE, MASTER_RADIO_STATE_NO_CHANGE, 
                                        demo_param.TECH, demo_param.BAND, sweep_param.Ref_Channel /* ulChan */,
                                        sweep_param.Waveform.Waveform_Type, NULL, sweep_param.Waveform.Number_Of_RBs,
                                        sweep_param.Waveform.Start_RB);
      Sweep[recordIndex].resExpected = false;
      Sweep[recordIndex].measSeg = false;
      RFCal_Helper_RecordIndex_Increment(Sweep, recordIndex);
      RFCal_helper_InsertNop(Sweep,recordIndex,5);
   }
    //RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
   //*****************************************************************************************//
   // Insert NoP (OpCode 0x0)
   //*****************************************************************************************//
    //RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length, sweep_param.Segment_Length[0][0] );
   return true;
}

unsigned char RFCal_Helper_ESC_ET_ConfigureFB( cal_sweep_record & record, unsigned char pDev, unsigned char bEnableFBmode, unsigned char & FBmode, int fbDev, int fbGainState, byte pa_state, byte rgi)
{
   //*********************************************//
   //    OpCode 0x10000:   Configure WTR
   //*********************************************//
   if (bEnableFBmode)
   {
      if (!FBmode)
      {
         FBmode = true;

         RFCal_Helper_setOpCode_FB_Setup(record,   pDev, fbDev,   ESC_FEEDBACK_SETUP_ACTION_CONFIG_WTR /* fbAction */, FBmode, fbGainState, pa_state,rgi);
      }
      else
      {
         RFCal_Helper_setOpCode_FB_Setup(record,   pDev, fbDev,   ESC_FEEDBACK_SETUP_ACTION_SKIP /* fbAction */,   FBmode, fbGainState, pa_state,rgi);
      }
   }
   else
   {
      if (FBmode)
      {
         FBmode = false;
         RFCal_Helper_setOpCode_FB_Setup(record,   pDev, fbDev,   ESC_FEEDBACK_SETUP_ACTION_CONFIG_WTR /* fbAction */, FBmode, fbGainState, pa_state,rgi);
      }
      else
      {
         RFCal_Helper_setOpCode_FB_Setup(record,   pDev, fbDev,   ESC_FEEDBACK_SETUP_ACTION_SKIP /* fbAction */,   FBmode, fbGainState, pa_state,rgi);
      }
   }
   return true;
}
unsigned char RFCal_Helper_setOpCode_EnhIntDevCal(cal_sweep_record& tmp, const Cal_Param & cal_param,const Sweep_Params & sweep_param, int TECH, int BAND)
{
   //*********************************************//
    //      OpCode 0x20000: Enh Int Dev Cal for FBRx DC cal
   //*********************************************//
   memset(&tmp.EnhIntDevCal,0,sizeof(cal_sweep_enh_int_device_cal)); // Initialize

   tmp.resExpected = true;
   tmp.EnhIntDevCal.do_Enhanced_Internal_Device_cal = 1;
   tmp.EnhIntDevCal.res_rev = static_cast<unsigned char>(0);
   tmp.EnhIntDevCal.dbg_mode_en = static_cast<unsigned char>(sweep_param.Enh_Int_Cal_Debug_Mode_Enable);
   tmp.EnhIntDevCal.flag = static_cast<unsigned short>(sweep_param.Enh_Int_Cal_Flag);

   tmp.EnhIntDevCal.tech = static_cast<unsigned char>(TECH);
   tmp.EnhIntDevCal.band = static_cast<unsigned short>(BAND);
   tmp.EnhIntDevCal.ref_chan = static_cast<unsigned long>(sweep_param.Ref_Channel);
   tmp.EnhIntDevCal.int_cal_type_mask = static_cast<unsigned long>(sweep_param.Enh_Int_Cal_Type_Mask);
   tmp.EnhIntDevCal.num_sig_bandwidth = static_cast<unsigned char>(sweep_param.TechBW_List.size());

   for(UINT bwiter=0; bwiter < sweep_param.TechBW_List.size() ; ++bwiter)
   {
      tmp.EnhIntDevCal.bw[bwiter] = static_cast<unsigned char>(sweep_param.TechBW_List[bwiter]);
   }

   tmp.EnhIntDevCal.dev_mask = static_cast<unsigned short>(sweep_param.Device_Mask);

   if( tmp.EnhIntDevCal.int_cal_type_mask & ENH_INT_DEV_CAL_FBDC )
   {
      tmp.EnhIntDevCal.int_cal_type_mask = static_cast<unsigned long>(ENH_INT_DEV_CAL_FBDC); //seperate rsb cal from dc cal
      tmp.EnhIntDevCal.dc_instr.num_Gain_States = static_cast<unsigned char>(sweep_param.FBRx_DC_Gain_State_List.size());

      for(UINT fbrx_gain_state_iter = 0; fbrx_gain_state_iter < sweep_param.FBRx_DC_Gain_State_List.size() ; ++fbrx_gain_state_iter)
      {
         tmp.EnhIntDevCal.dc_instr.fbrx_gain_state[fbrx_gain_state_iter] = static_cast<unsigned char>( sweep_param.FBRx_DC_Gain_State_List[fbrx_gain_state_iter]);
      }

      tmp.EnhIntDevCal.dc_instr.num_iq_samples = static_cast<unsigned short>(sweep_param.FBRx_DC_Num_Capture_Samples);
   }

   return true;
}
unsigned char RFCal_Helper_setOpCode_EnhIntDevRSBCal(cal_sweep_record& tmp, const Cal_Param & cal_param,const Sweep_Params & sweep_param, int TECH, int BAND, byte bFBRxRSB,  unsigned long int_cal_type_mask, byte gainStateListIndex, byte bSCell )
{
    //*********************************************//
    //      OpCode 0x20000: Enh Int Dev Cal for FBRx/RxRSB Cal
    //*********************************************//
   memset(&tmp.EnhIntDevCal,0,sizeof(cal_sweep_enh_int_device_cal)); // Initialize


    //*********************************************//
    //      Common Parmaeters
    //*********************************************//

   tmp.resExpected = true;
   tmp.EnhIntDevCal.do_Enhanced_Internal_Device_cal = 1;
   tmp.EnhIntDevCal.res_rev = static_cast<unsigned char>(0);
   tmp.EnhIntDevCal.flag = static_cast<unsigned short>(sweep_param.Enh_Int_Cal_Flag);

   tmp.EnhIntDevCal.tech = static_cast<unsigned char>(TECH);
   tmp.EnhIntDevCal.band = static_cast<unsigned short>(BAND);
   tmp.EnhIntDevCal.ref_chan = static_cast<unsigned long>(sweep_param.Ref_Channel);
   tmp.EnhIntDevCal.int_cal_type_mask = static_cast<unsigned long>(int_cal_type_mask);



   tmp.EnhIntDevCal.num_sig_bandwidth = static_cast<unsigned char>(1); // we only support 1 sig_bw
   tmp.EnhIntDevCal.dc_instr.num_Gain_States = static_cast<unsigned char>(1);

   
    byte RxDeviceFlag = 0;
   // If bScell == 0, then PCell
   if ( sweep_param.Device_Selection.Master_Device_ID_List.size() >= static_cast<size_t>(bSCell+1) 
      && sweep_param.Device_Selection.Master_Device_ID_List[bSCell].size() >= static_cast<size_t>(bSCell+1))
   {
      RxDeviceFlag = 1 << sweep_param.Device_Selection.Master_Device_ID_List[bSCell][bSCell];
      if ( sweep_param.Device_Selection.Slave_Device_ID_List.size() >= static_cast<size_t>(bSCell) 
         && sweep_param.Device_Selection.Slave_Device_ID_List[bSCell].size() >= static_cast<size_t>(bSCell+1))
      {
         RxDeviceFlag |= 1 << sweep_param.Device_Selection.Slave_Device_ID_List[bSCell][bSCell];
      }
   }
   else
   {
    for (UINT devIndex = 0; devIndex < sweep_param.Device_Selection.Rx_Device_To_Calibrate.size(); devIndex++)
    {
        RxDeviceFlag |= (1 << sweep_param.Device_Selection.Rx_Device_To_Calibrate[devIndex] );
    }
   }

   if( bFBRxRSB)
   {
        //*********************************************//
        //      FBRxRSB parameter assignment
        //*********************************************//
        byte devMask = (bSCell)? 0xc: 0x3;
        tmp.EnhIntDevCal.dev_mask = static_cast<unsigned short>(sweep_param.FBRxRsb_Device_Mask & devMask);
        tmp.EnhIntDevCal.dbg_mode_en = static_cast<unsigned char>(sweep_param.FBRxRsb_DbgLevel);
        tmp.EnhIntDevCal.rsb_instr.rsb_type = sweep_param.FBRxRsb_Type;
        tmp.EnhIntDevCal.rsb_instr.rsb_cal_alg = sweep_param.FBRxRsb_Algorithm;
        
        tmp.EnhIntDevCal.rsb_instr.offset_khz = sweep_param.FBRxRsb_Waveform_Offset_in_kHz;
        tmp.EnhIntDevCal.rsb_instr.iq_capture_type = sweep_param.FBRxRsb_IQ_Capture_Type;
        tmp.EnhIntDevCal.rsb_instr.iq_sample_size = sweep_param.FBRxRsb_IQ_Capture_Size;
        if (bSCell == 1)
        {
            tmp.EnhIntDevCal.rsb_instr.carrier_id = 1;
            tmp.EnhIntDevCal.rsb_instr.pcell_band_for_scell = sweep_param.FBRxRsb_PCell_Band_For_SCell;
            tmp.EnhIntDevCal.rsb_instr.pcell_ref_chan_for_scell = sweep_param.FBRxRsb_PCell_RefChan_For_SCell;
        } else {
            tmp.EnhIntDevCal.rsb_instr.carrier_id = 0;
            tmp.EnhIntDevCal.rsb_instr.pcell_band_for_scell = 0;
            tmp.EnhIntDevCal.rsb_instr.pcell_ref_chan_for_scell = 0;
        }
         for(unsigned char gain_state_iter = 0; gain_state_iter < sweep_param.FBRxRsb_Gain_State_List.size() ; ++ gain_state_iter)
         {
            tmp.EnhIntDevCal.rsb_instr.gain_states[gain_state_iter] = static_cast<unsigned char>( sweep_param.FBRxRsb_Gain_State_List[gain_state_iter]);
         }
         tmp.EnhIntDevCal.rsb_instr.num_gain_states = sweep_param.FBRxRsb_Gain_State_List.size();

         for(UINT bwiter=0; bwiter < sweep_param.TechBW_List.size() ; ++bwiter)
         {
          tmp.EnhIntDevCal.bw[bwiter] = static_cast<unsigned char>(sweep_param.TechBW_List[bwiter]);
         }
         tmp.EnhIntDevCal.num_sig_bandwidth = static_cast<unsigned char>(sweep_param.TechBW_List.size());



   }  else {
        //*********************************************//
        //      RxRSB parameter assignment
        //*********************************************//
        byte devMask = (bSCell)? 0xc: 0x3;
 //     devMask = devMask & RxDeviceFlag;
    //  tmp.EnhIntDevCal.numReqRsbCalElement = 4;
        if (bSCell && sweep_param.RxRsb_Device_Mask <= 3)
        {
            tmp.EnhIntDevCal.dev_mask = static_cast<unsigned short>((sweep_param.RxRsb_Device_Mask << 2) & devMask);
        }
        else
        {
        tmp.EnhIntDevCal.dev_mask = static_cast<unsigned short>(sweep_param.RxRsb_Device_Mask & devMask);
        }
        tmp.EnhIntDevCal.dbg_mode_en = static_cast<unsigned char>(sweep_param.RxRsb_DbgLevel);
        tmp.EnhIntDevCal.rsb_instr.rsb_type = sweep_param.RxRsb_Type;
        tmp.EnhIntDevCal.rsb_instr.rsb_cal_alg = sweep_param.RxRsb_Algorithm;

        tmp.EnhIntDevCal.rsb_instr.offset_khz = sweep_param.RxRsb_Waveform_Offset_in_kHz;
        tmp.EnhIntDevCal.rsb_instr.iq_capture_type = sweep_param.RxRsb_IQ_Capture_Type;
        tmp.EnhIntDevCal.rsb_instr.iq_sample_size = sweep_param.RxRsb_IQ_Capture_Size;
        if (bSCell == 1)
        {
            tmp.EnhIntDevCal.rsb_instr.carrier_id = 1;
            tmp.EnhIntDevCal.rsb_instr.pcell_band_for_scell = sweep_param.RxRsb_PCell_Band_For_SCell;
            tmp.EnhIntDevCal.rsb_instr.pcell_ref_chan_for_scell = sweep_param.RxRsb_PCell_RefChan_For_SCell;
        } else {
            tmp.EnhIntDevCal.rsb_instr.carrier_id = 0;
            tmp.EnhIntDevCal.rsb_instr.pcell_band_for_scell = 0;
            tmp.EnhIntDevCal.rsb_instr.pcell_ref_chan_for_scell = 0;
        }
        tmp.EnhIntDevCal.bw[0] = static_cast<unsigned char>(sweep_param.RxRsb_DL_Bandwidth); // over-write bw
        tmp.EnhIntDevCal.num_sig_bandwidth = static_cast<unsigned char>(1);

        if (sweep_param.RxRsb_Gain_State_List.size() > gainStateListIndex) 
        {
            tmp.EnhIntDevCal.rsb_instr.gain_states[0] = static_cast<unsigned char>(sweep_param.RxRsb_Gain_State_List[gainStateListIndex]); //rx rsb can't do multiple gain_state as need sync with dl power
            tmp.EnhIntDevCal.rsb_instr.num_gain_states = 1;
        } else {
        return false;
        }   
   }

   return true;
}

unsigned char RFCal_Helper_ESC_ET_createRxRSBCalSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
    //*********************************************//
    //      Parameters.
    //*********************************************//
    const byte  IQGainAction = ESC_TX_OVERRIDE_ACTION_SW_DEF,   EnvScaleAction = ESC_TX_OVERRIDE_ACTION_SW_DEF;

    int txParamMask_PwrOverride  = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN |
    ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT); // Sweep 

    UINT recordIndex = 0;

    //*********************************************//
    //      Enh Internal Device Cal - RxRSB Cal
    //*********************************************//
    if (sweep_param.Do_Enh_Int_Cal != 1) { return false;}
    if (!(sweep_param.Enh_Int_Cal_Type_Mask & ENH_INT_DEV_CAL_RSB)){ return false;}
    if (sweep_param.Do_Enh_Int_Rx_Rsb_Cal != 1) { return false;}

   byte   masterDevState            = MASTER_RADIO_STATE_RETUNE;
   byte   slaveDevState            = (sweep_param.RxRsb_Device_Mask & 2)?SLAVE_RADIO_STATE_RX_DIV_STATE:SLAVE_RADIO_STATE_NO_CHANGE;

    //*********************************************//
    //      OpCode 0x100: Radio Setup 
    //*********************************************//
    RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], 
        sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, 
        masterDevState /* Master Device State */, 
        slaveDevState /* Slave Device State */, 
        demo_param.TECH /* Tech */, 
        demo_param.BAND /* Band */, 
        sweep_param.Ref_Channel /* ulChan */, 
        sweep_param.Waveform.Waveform_Type, NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB,NULL,sweep_param.Waveform.Rx_BW,sweep_param.Waveform.Tx_BW);
            
    Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;
    Sweep[recordIndex].radio_cfg.flag = 0;
    if (demo_param.TECH == 4)
    {
        RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.Tuning_Segment_Length); 
    }
    else
    {
    RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.Config_Segment_Length); 
    }
    //*****************************************************************************************//
    //       RxRSB Cal segments (Note that this not FBRxRSB Cal). Require Callbox DL 
    //*****************************************************************************************//
        
    //*********************************************//
    //      OpCode 0x200: Tx Override - Trigger
    //*********************************************//
    RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], 
                0xFF /* TxPurpose Invalid */,
                cal_param.TxDeviceFlag, XPT_TYPE_APT, 
                ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */, 
                ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,           
                txParamMask_PwrOverride /* pwrParamMask */, 
                sweep_param.PA_State /* PA State */, 
                sweep_param.RGI /* RGI */,   
                sweep_param.Bias /* Bias */,
                IQGainAction, 0 /* IQGain Action & IqGain */,   
                EnvScaleAction, 0 /* EnvScale Action & EnvScale */, 
                sweep_param.Icq, 
                0 /* delay action */, 0 /* delay value */);
    //  Sweep[recordIndex].measSeg = true;

    TE_Records[recordIndex].m_ul_lvl = (sweep_param.Max_Expected_Pwr.size() > 0)?sweep_param.Max_Expected_Pwr[0]:24.0;  

   RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);      // Increament of Segment // 
    
    //*********************************************//
    //      OpCode 0x200: Tx Override - set low RGI
    //*********************************************//
    RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], 
                0xFF /* TxPurpose Invalid */,
                cal_param.TxDeviceFlag, XPT_TYPE_APT, 
                ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */, 
                ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,           
                txParamMask_PwrOverride /* pwrParamMask */, 
                sweep_param.PA_State /* PA State */, 
                5   /* RGI */,   
                sweep_param.Bias /* Bias */,
                IQGainAction, 0 /* IQGain Action & IqGain */,   
                EnvScaleAction, 0 /* EnvScale Action & EnvScale */, 
                sweep_param.Icq, 
                0 /* delay action */, 0 /* delay value */);
    //  Sweep[recordIndex].measSeg = true;
#ifdef ON_EQUIPMENT
               TE_Records[recordIndex].m_ul_lvl = 10; // To declare this segment a non-trigger segment
#endif
    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);         // Increament of Segment // 
    
    //****************************************************************//
    //  Calcululate Rx Device flag
    //****************************************************************//

    byte RxDeviceFlag = 0;
    for (UINT devIndex = 0; devIndex < sweep_param.Device_Selection.Rx_Device_To_Calibrate.size(); devIndex++)
    {
        RxDeviceFlag |= (1 << sweep_param.Device_Selection.Rx_Device_To_Calibrate[devIndex] );
    }
    
    const byte RX_RSBCAL = 0;
    byte bSCEll = 0; // 0 is for PCELL, 1 is for SCELL.
    for (byte RsbGainStateIndex = 0; RsbGainStateIndex < sweep_param.RxRsb_Gain_State_List.size(); RsbGainStateIndex++)
    {
        //*****************************************************************************************//
        //      OpCode 0x20000: RxRSB Cal 
        //*****************************************************************************************//
        RFCal_Helper_setOpCode_EnhIntDevRSBCal(Sweep[recordIndex],cal_param, sweep_param, demo_param.TECH, demo_param.BAND, RX_RSBCAL, ENH_INT_DEV_CAL_RSB, RsbGainStateIndex, bSCEll );
        Sweep[recordIndex].opcode.segLen = static_cast<word>(sweep_param.Segment_Length[0][0]);
                
        //*****************************************************************************************//
        //      OpCode 0x20: RxConfig
        //*****************************************************************************************//
      byte devMask = 0;
      if ( !sweep_param.Device_Selection.Master_Device_ID_List.empty() && !sweep_param.Device_Selection.Master_Device_ID_List[0].empty())
      {
         devMask = 1 << sweep_param.Device_Selection.Master_Device_ID_List[0][0];
         if ( !sweep_param.Device_Selection.Slave_Device_ID_List.empty() && !sweep_param.Device_Selection.Slave_Device_ID_List[0].empty())
         {
             devMask |= 1 << sweep_param.Device_Selection.Slave_Device_ID_List[0][0];
         }
      }
      else
      {
         devMask = (sweep_param.RxRsb_Device_Mask & 0x3) & RxDeviceFlag ;
      }
      
      if ( sweep_param.Device_Selection.DevicePathPair_To_NVcontainer.size() > 0)
      {
         unsigned short RxPath[4];
         memset(RxPath, 0, sizeof(unsigned short)*4);

         for (vector <vector <byte> >::const_iterator it = sweep_param.Device_Selection.DevicePathPair_To_NVcontainer.begin();
             it != sweep_param.Device_Selection.DevicePathPair_To_NVcontainer.end();it++)
         {
                if (it->size() == 3)
                {
                    unsigned short iDevice = (*it)[0];
                    unsigned short iPath = (*it)[1];
                    RxPath[iDevice] = iPath;
                }      
         }
        RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], sweep_param.RxRsb_Gain_State_List[RsbGainStateIndex] /* Gain State */,
         devMask /* Device Flag PCell PRx and DRx only */, 
         0 /* Power Mode */,
         static_cast<word>(CalculateRxAGCValue(demo_param.TECH,    sweep_param.RxRsb_DL_Power_List[RsbGainStateIndex])) /* Expected AGC */, 
         0 /* Rx Meas Action*/, 4, RxPath, 4);

      }
      else
      {
      RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], sweep_param.RxRsb_Gain_State_List[RsbGainStateIndex] /* Gain State */,
            devMask /* Device Flag PCell PRx and DRx only */, 
            0 /* Power Mode */,
            static_cast<word>(CalculateRxAGCValue(demo_param.TECH,  sweep_param.RxRsb_DL_Power_List[RsbGainStateIndex])) /* Expected AGC */, 
            0 /* Rx Meas Action*/);
      }
        TE_Records[recordIndex].m_dl_lvl = sweep_param.RxRsb_DL_Power_List[RsbGainStateIndex]; // To declare this segment a non-trigger segment
        TE_Records[recordIndex].m_offset_in_kHz = 500;

        //*****************************************************************************************//
        //       Add OpCode 0x0:NoP segmnets for sweep_param.RxRsb_Segment_Length
        //*****************************************************************************************//
        RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.RxRsb_Segment_Length, Sweep[recordIndex].opcode.segLen); 

        //*****************************************************************************************//
        //       OpCode 0x20000: RxRSB_GET_RESULT 
        //*****************************************************************************************//
        RFCal_Helper_setOpCode_EnhIntDevRSBCal(Sweep[recordIndex],cal_param, sweep_param, demo_param.TECH, demo_param.BAND, RX_RSBCAL, ENH_INT_DEV_CAL_RSB_GET_RESULT, RsbGainStateIndex, bSCEll );
        Sweep[recordIndex].opcode.segLen = static_cast<word>(sweep_param.Segment_Length[0][0]);
    
      Sweep[recordIndex].EnhIntDevCal.use_txrx_split = false;
      if ( !sweep_param.Device_Selection.Master_Device_ID_List.empty() && !sweep_param.Device_Selection.Master_Device_ID_List[0].empty())
      {
           Sweep[recordIndex].EnhIntDevCal.use_txrx_split = true;
         Sweep[recordIndex].EnhIntDevCal.unique_dev_logical_dev_map[0] = static_cast<unsigned char> ( sweep_param.Device_Selection.Master_Device_ID_List[0][0]);
         if ( !sweep_param.Device_Selection.Slave_Device_ID_List.empty() && !sweep_param.Device_Selection.Slave_Device_ID_List[0].empty())
         {
            Sweep[recordIndex].EnhIntDevCal.unique_dev_logical_dev_map[1] = static_cast<unsigned char> ( sweep_param.Device_Selection.Slave_Device_ID_List[0][0]);
         }
      }
          // Set Path using DevicePathPair_To_NVcontainer
        if ( sweep_param.Device_Selection.DevicePathPair_To_NVcontainer.size() > 0)
         {
         
            for (vector <vector <byte> >::const_iterator it = sweep_param.Device_Selection.DevicePathPair_To_NVcontainer.begin();
                it != sweep_param.Device_Selection.DevicePathPair_To_NVcontainer.end();it++)
            {
               if (it->size() == 3)
               {
                  unsigned short iDevice = (*it)[0];
                  unsigned short iPath = (*it)[1];
                  Sweep[recordIndex].rxParams.rxPath[iDevice] = static_cast<byte>(iPath);
               }      
            }
         }

        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);     // Increament of Segment
    } // End of Gain State Loop 
        
    //*****************************************************************************************//
    //       RxRSB Cal segments for SCELL (LTE only 
    //*****************************************************************************************//
    if (demo_param.TECH == 3 && sweep_param.RxRsb_PCell_Band_For_SCell > 0) 
    {
        //*****************************************************************************************//
        //      OpCode 0x100: Radio Setup for PCELL change
        //*****************************************************************************************//
        byte iMasterDeviceID = 0, iSlaveDeviceID = 1;
        bSCEll = 1; // this is for RxRSB

        RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], 
            iMasterDeviceID, iSlaveDeviceID, 
            masterDevState /* Master Device State */, 
            slaveDevState /* Slave Device State */, 
            demo_param.TECH /* Tech */, 
            sweep_param.RxRsb_PCell_Band_For_SCell /* PCell Band */, 
            sweep_param.RxRsb_PCell_RefChan_For_SCell /* PCell ulChan */, 
            sweep_param.Waveform.Waveform_Type, NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB,NULL,sweep_param.Waveform.Rx_BW,sweep_param.Waveform.Tx_BW);
        Sweep[recordIndex].radio_cfg.flag = 0;  
        Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);     // Increament of Segment  Note; Please add Nop if necessary for callbox.

        //*****************************************************************************************//
        //      Add NoP
        //*****************************************************************************************//
        RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.Config_Segment_Length,static_cast<word>(sweep_param.Segment_Length[0][0])); 

        //*****************************************************************************************//
        //      OpCode 0x100: Radio Setup for SCELL change
        //*****************************************************************************************//
            iMasterDeviceID = 2;  iSlaveDeviceID = 3;
            masterDevState = MASTER_RADIO_STATE_RX_STATE;
        RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], 
            iMasterDeviceID, iSlaveDeviceID, 
            masterDevState /* Master Device State */, 
            slaveDevState /* Slave Device State */, 
            demo_param.TECH /* Tech */, 
            demo_param.BAND /* Band */, 
            sweep_param.Ref_Channel /* ulChan */, 
            sweep_param.Waveform.Waveform_Type, NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB,NULL,sweep_param.Waveform.Rx_BW,sweep_param.Waveform.Tx_BW);
            
        Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_APT_CA; // This is not necessary. Just same as subsysRFcal. No affect for NV or measurement.
        Sweep[recordIndex].radio_cfg.flag = 0;
        //*****************************************************************************************//
        //      Add NoP
        //*****************************************************************************************//
        RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.Config_Segment_Length,static_cast<word>(sweep_param.Segment_Length[0][0])); 

        for (byte RsbGainStateIndex = 0; RsbGainStateIndex < sweep_param.RxRsb_Gain_State_List.size(); RsbGainStateIndex++)
        {
            //*****************************************************************************************//
            //      OpCode 0x20000: RxRSB Cal 
            //*****************************************************************************************//
            RFCal_Helper_setOpCode_EnhIntDevRSBCal(Sweep[recordIndex],cal_param, sweep_param, demo_param.TECH, demo_param.BAND, RX_RSBCAL, ENH_INT_DEV_CAL_RSB, RsbGainStateIndex , bSCEll);
            Sweep[recordIndex].opcode.segLen = static_cast<word>(sweep_param.Segment_Length[0][0]);
            Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_APT_CA;   
            if (demo_param.TECH == 4)
            {
                Sweep[recordIndex].EnhIntDevCal.flag = 1;
            }
            //*****************************************************************************************//
            //      OpCode 0x20: RxConfig
            //*****************************************************************************************//
         byte devMask = 0;
         if ( sweep_param.Device_Selection.Master_Device_ID_List.size() >= 2 && sweep_param.Device_Selection.Master_Device_ID_List[1].size() >= 2)
         {
            devMask = 1 << sweep_param.Device_Selection.Master_Device_ID_List[1][1];
            if ( sweep_param.Device_Selection.Slave_Device_ID_List.size() >= 2 && sweep_param.Device_Selection.Slave_Device_ID_List[1].size() >= 2)
            {
                devMask |= 1 << sweep_param.Device_Selection.Slave_Device_ID_List[1][1];
            }
         }
         else
         {
            devMask = ((sweep_param.RxRsb_Device_Mask << 2) & 0xC ) & RxDeviceFlag ;
         }
         
        
        RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], sweep_param.RxRsb_Gain_State_List[RsbGainStateIndex] /* Gain State */,
            devMask /* Device Flag PCell PRx and DRx only */, 
            0 /* Power Mode */,
            static_cast<word>(CalculateRxAGCValue(demo_param.TECH,  sweep_param.RxRsb_DL_Power_List[RsbGainStateIndex])) /* Expected AGC */, 
            0 /* Rx Meas Action*/);

            //*****************************************************************************************//
            //       Add OpCode 0x0:NoP segmnets for sweep_param.RxRsb_Segment_Length
            //*****************************************************************************************//
            RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.RxRsb_Segment_Length, Sweep[recordIndex].opcode.segLen); 

            //*****************************************************************************************//
            //       OpCode 0x20000: RxRSB_GET_RESULT 
            //*****************************************************************************************//
            RFCal_Helper_setOpCode_EnhIntDevRSBCal(Sweep[recordIndex],cal_param, sweep_param, demo_param.TECH, demo_param.BAND, RX_RSBCAL, ENH_INT_DEV_CAL_RSB_GET_RESULT, RsbGainStateIndex, bSCEll );
            Sweep[recordIndex].opcode.segLen = static_cast<word>(sweep_param.Segment_Length[0][0]);
            Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_APT_CA;
         Sweep[recordIndex].EnhIntDevCal.use_txrx_split = false;
         
         if ( sweep_param.Device_Selection.Master_Device_ID_List.size() >= 2 && sweep_param.Device_Selection.Master_Device_ID_List[1].size() >= 2)
         {
              Sweep[recordIndex].EnhIntDevCal.use_txrx_split = true;
            Sweep[recordIndex].EnhIntDevCal.unique_dev_logical_dev_map[2] = static_cast<unsigned char> ( sweep_param.Device_Selection.Master_Device_ID_List[1][1]);
            if ( sweep_param.Device_Selection.Slave_Device_ID_List.size() >= 2 && sweep_param.Device_Selection.Slave_Device_ID_List[1].size() >= 2)
            {
               Sweep[recordIndex].EnhIntDevCal.unique_dev_logical_dev_map[3] = static_cast<unsigned char> ( sweep_param.Device_Selection.Slave_Device_ID_List[1][1]);
            }
         }
       
      // Set Path using DevicePathPair_To_NVcontainer
        if ( sweep_param.Device_Selection.DevicePathPair_To_NVcontainer.size() > 0)
         {
         
            for (vector <vector <byte> >::const_iterator it = sweep_param.Device_Selection.DevicePathPair_To_NVcontainer.begin();
                it != sweep_param.Device_Selection.DevicePathPair_To_NVcontainer.end();it++)
            {
               if (it->size() == 3)
               {
                  unsigned short iDevice = (*it)[0];
                  unsigned short iPath = (*it)[1];
                  Sweep[recordIndex].rxParams.rxPath[iDevice] =  static_cast<byte>(iPath);
               }      
            }
         }
            if (demo_param.TECH == 4)
            {
                Sweep[recordIndex].EnhIntDevCal.flag = 1;
            }
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);     // Increament of Segment
        } // End of Gain State Loop
    } // End of RxRSC Cal for SCell
        
    //*********************************************//
    //      Generate OpCode for all the segment
    //*********************************************//
    Sweep.resize(recordIndex); // Delete unused records
    RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());
    
    return true;
}


unsigned char RFCal_Helper_ESC_ET_createRefCalSwp(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
   //*********************************************//
   //    Sweep 1 Parameters.
   //*********************************************//
    byte    IQGainAction = ESC_TX_OVERRIDE_ACTION_SW_DEF,   EnvScaleAction = ESC_TX_OVERRIDE_ACTION_SW_DEF;
   word  IQGain,        EnvScale = 0;
   word  Icq;

   int            fbDev            =   static_cast<int>(sweep_param.Device_Selection.Feedback_Device_ID);
   const int      fbEnabled      =  1;
   const int      fbDisabled     =  0;
   int            fbGainState    =  0;

   unsigned char  FBmode         = false;

   int txParamMask_PwrOverride  = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN |
                                   ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT); // Sweep
   byte pDev = RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag);
   vector <dword> Tx_Channel_List;
   vector <byte> BW_List;
   vector <byte> StartRB_List;
   vector <byte> NumRB_List;

   if (sweep_param.FBRx_Cal_Mode == DO_REF_CAL &&  sweep_param.Do_FBGain_Cal) // FBRxGain Cal
   {
      UINT FB_BW_ListSize = sweep_param.FBGain_Cal_BW_List.size();
      for (UINT i=0; i < FB_BW_ListSize; i++)
      {
         for (UINT chan = 0; chan < sweep_param.Tx_Channel_List.size(); chan++)
         {
            Tx_Channel_List.push_back(sweep_param.Tx_Channel_List[chan]);

            BW_List.push_back(sweep_param.FBGain_Cal_BW_List[i]);
            StartRB_List.push_back(sweep_param.FBGain_Cal_StartRB_List[i]);
            NumRB_List.push_back(sweep_param.FBGain_Cal_NumRB_List[i]);
         }
      }
   }
   else     // Regular Ref Cal (For WTR1625)
   {
      for (UINT i=0; i < sweep_param.Tx_Channel_List.size(); i++)
      {
         Tx_Channel_List.push_back(sweep_param.Tx_Channel_List[i]);
         BW_List.push_back(sweep_param.Waveform.Tx_BW);
         StartRB_List.push_back(sweep_param.Waveform.Start_RB);
         NumRB_List.push_back(sweep_param.Waveform.Number_Of_RBs);
      }
   }

   //******************************************************************************//
   //       Sweep 1
   //******************************************************************************//
   UINT recordIndex = 0;

   //*********************************************//
   //    Enh Internal Device Cal
   //*********************************************//

   if (sweep_param.Do_Enh_Int_Cal == 1)
   {
        if (sweep_param.Enh_Int_Cal_Type_Mask & ENH_INT_DEV_CAL_FBDC)
      {
         byte  masterDevState          = MASTER_RADIO_STATE_RX_TX_STATE;
         byte   slaveDevState            = (cal_param.RxDeviceFlag == 1)?SLAVE_RADIO_STATE_NO_CHANGE :SLAVE_RADIO_STATE_NO_CHANGE;

            if (demo_param.TECH == 4)
            {


         //*********************************************//
         //    OpCode 0x100: Radio Setup
         //*********************************************//

         RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex],
                                           sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID,
                                           masterDevState /* Master Device State */,
                                           slaveDevState /* Slave Device State */,
                                           demo_param.TECH /* Tech */,
                                           demo_param.BAND /* Band */,
                                           sweep_param.Ref_Channel /* ulChan */,
                    5, NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB,NULL,sweep_param.Waveform.Rx_BW,sweep_param.Waveform.Tx_BW);

              Sweep[recordIndex].radio_cfg.flag = 1; // Tx Internal / Ref Cal

         Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;

         RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.Config_Segment_Length);
                masterDevState              = 0;
                slaveDevState               = 0;
            }

            //*********************************************//
            //      OpCode 0x100: Radio Setup 
            //*********************************************//
            int waveform = (demo_param.TECH == 4)? 5 : sweep_param.Waveform.Waveform_Type;
            RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], 
                sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, 
                masterDevState /* Master Device State */, 
                slaveDevState /* Slave Device State */, 
                demo_param.TECH /* Tech */, 
                demo_param.BAND /* Band */, 
                sweep_param.Ref_Channel /* ulChan */, 
                waveform, NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB,NULL,sweep_param.Waveform.Rx_BW,sweep_param.Waveform.Tx_BW);
            
            Sweep[recordIndex].radio_cfg.flag = (demo_param.TECH == 4)?0:1; // Tx Internal / Ref Cal
            Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_DEFAULT;
            if (demo_param.TECH == 4)
            {
//              RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.Enh_Int_Cal_Segment_Length);  
                RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.Tuning_Segment_Length);  

            }
            else
            {
                RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.Config_Segment_Length); 
            }


          //*****************************************************************************************//
          //  OpCode 0x1000: Sync OpCode (LTE Only and ET/EPT only) 
          //*****************************************************************************************//
         if (demo_param.TECH == 3 && sweep_param.Sweep_Type == XPT_SWP1 )
         {
                  RFCal_Helper_setOpCode_Sync(Sweep[recordIndex]);
                  RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
                  Sweep[recordIndex].isNopSeg = true;
                  RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
          }
    
         //*********************************************//
         //    OpCode 0x200: Tx Override
         //*********************************************//
         RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], 0xFF /* TxPurpose Invalid */,
                                           cal_param.TxDeviceFlag, XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                           ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,        txParamMask_PwrOverride /* pwrParamMask */,
                                           sweep_param.PA_State /* PA State */,
                                           sweep_param.RGI   /* RGI */,
                                           sweep_param.Bias /* Bias */,
                                           IQGainAction, 0 /* IQGain Action & IqGain */,
                                           EnvScaleAction, 0 /* EnvScale Action & EnvScale */,
                                           sweep_param.Icq,
                                           3 /* delay action */, 0 /* delay value */);
         Sweep[recordIndex].measSeg = true;


            TE_Records[recordIndex].m_teAction = false; // To declare this segment a non-trigger segment
         //*********************************************//
         //    OpCode 0x20000: Enh Int Dev Cal
         //*********************************************//
         RFCal_Helper_setOpCode_EnhIntDevCal(Sweep[recordIndex],cal_param, sweep_param, demo_param.TECH, demo_param.BAND);
            Sweep[recordIndex].opcode.segLen = static_cast<word>(sweep_param.Enh_Int_Cal_Segment_Length);

            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);     // Increament of Segment
      }
        //*****************************************************************************************//
        //       Add FBRxRSB Cal (Note that this not RxRSB Cal)
        //*****************************************************************************************//
        if (sweep_param.Do_Enh_Int_FBRxRsb_Cal == 1)
        {

            const byte FBRX_RSBCAL = 1;
            byte FBRXGainState = 2;
            //if (sweep_param.FBRxRsb_Gain_State_List.size() > 0 ) { FBRXGainState = sweep_param.FBRxRsb_Gain_State_List[0]; }
            FBRXGainState = 2;
            //*****************************************************************************************//
            //      OpCode 0x20000:  FBRxRSB Cal (Note that this not RxRSB Cal)
            //*****************************************************************************************//
            RFCal_Helper_setOpCode_EnhIntDevRSBCal(Sweep[recordIndex],cal_param, sweep_param, demo_param.TECH, demo_param.BAND, FBRX_RSBCAL, ENH_INT_DEV_CAL_RSB, FBRXGainState );
            Sweep[recordIndex].opcode.segLen = static_cast<word>(sweep_param.Segment_Length[0][0]);

            //*****************************************************************************************//
            //       Add NoP for sweep_param.FBRxRsb_Segment_Length
            //*****************************************************************************************//
///         RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.FBRxRsb_Segment_Length, Sweep[recordIndex].opcode.segLen); 
                RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,30000, Sweep[recordIndex].opcode.segLen); 

            //*****************************************************************************************//
            //      OpCode 0x20000:  FBRxRSB_GET_RESULT 
            //*****************************************************************************************//
            RFCal_Helper_setOpCode_EnhIntDevRSBCal(Sweep[recordIndex],cal_param, sweep_param, demo_param.TECH, demo_param.BAND, FBRX_RSBCAL, ENH_INT_DEV_CAL_RSB_GET_RESULT, FBRXGainState );
            Sweep[recordIndex].opcode.segLen = static_cast<word>(sweep_param.Segment_Length[0][0]);
    
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);     // Increament of Segment

        }

      //*****************************************************************************************//
      //  OpCode 0x10000: FBSetup   Feedback Action:  4  ( | Bit 2 = Configure RxLM for FB AGC read )
      //  Enable Diveristy chain/configure the correct RxLM to get FB AGC
      //*****************************************************************************************//
      FBmode = true;
      
      RFCal_Helper_setOpCode_FB_Setup(Sweep[recordIndex],   pDev, fbDev,   ESC_FEEDBACK_SETUP_ACTION_CONFIG_RXLM_FOR_FBAGC /* fbAction */,   fbEnabled ,0,0xFF);
      Sweep[recordIndex].measSeg = false;
        Sweep[recordIndex].opcode.segLen = static_cast<word>(sweep_param.Enh_Int_Cal_Segment_Length);
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);     // Increament of Segment

   }

   //*********************************************//
   //    FBRx Gain Cal or Ref Cal
   //*********************************************//

   if (sweep_param.FBRx_Cal_Mode == DO_REF_CAL)
   {
      for (UINT iLoopChannel = 0; iLoopChannel < Tx_Channel_List.size(); iLoopChannel++)
      {
         //*****************************************************************************************//
         // OpCode 0x100: Radio Setup
         //*****************************************************************************************//
         byte  masterDevState          = (iLoopChannel == 0 && sweep_param.Do_Enh_Int_Cal == 0)?3:4;
            byte    slaveDevState               = (iLoopChannel == 0 && sweep_param.Do_Enh_Int_Cal == 0)?0:0;

            if (iLoopChannel > 0 && (BW_List[iLoopChannel] != BW_List[iLoopChannel-1])) // if BW is changed
         {
            masterDevState = 3;
                slaveDevState = 0; 
         }
            if (demo_param.TECH == 4)
            {
                slaveDevState = 0; 
            }
            slaveDevState = (cal_param.RxDeviceFlag == 1)?SLAVE_RADIO_STATE_NO_CHANGE : slaveDevState;

         RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, masterDevState, slaveDevState, demo_param.TECH, demo_param.BAND,
                                           Tx_Channel_List[iLoopChannel] /* ulChan */, sweep_param.Waveform.Waveform_Type, NULL, NumRB_List[iLoopChannel], StartRB_List[iLoopChannel],NULL
                                           ,BW_List[iLoopChannel],BW_List[iLoopChannel]);
         Sweep[recordIndex].radio_cfg.flag = (demo_param.TECH == 4)?1:1; // Tx Internal / Ref Cal
            TE_Records[recordIndex].m_ul_lvl = sweep_param.Max_Expected_Pwr[0]; 
            
         if (demo_param.TECH == 4)
         {
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
                RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Enh_Int_Cal_Segment_Length,sweep_param.Segment_Length[0][0]);
         }

         //*****************************************************************************************//
         // Insert NoP (OpCode 0x0)
         //*****************************************************************************************//
            if (iLoopChannel == 0)  // First channel requires config duration
            {
                if (sweep_param.Do_Enh_Int_Cal == 1)
                {
                    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
               RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);
         }
         else
         {
                    RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.Tuning_Segment_Length); // New change from QDART 4821
                }
            }
            else  // Second channel: Tuning duration.
            {
            TE_Records[recordIndex].m_ul_lvl = sweep_param.Max_Expected_Pwr[0];
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
            RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);
         }

            if (!FBmode && !sweep_param.Do_FBGain_Cal && sweep_param.Fb_Rx_Gain_Cal_Mode != FBRX_WTR_PWR) /* Ref Cal for Tx Internal Cal of WTR 1625 */
         {
            //*****************************************************************************************//
            //  OpCode 0x10000: FBSetup   Feedback Action:  4  ( | Bit 2 = Configure RxLM for FB AGC read )
            //  Enable Diveristy chain/configure the correct RxLM to get FB AGC
            //*****************************************************************************************//
            FBmode = true;
            RFCal_Helper_ESC_ET_OpCodeConfigureRxLM(demo_param, cal_param, sweep_param, Sweep, recordIndex, FBmode);
                FBmode = false;
        
         } // if (!FBmode)

         unsigned short fbGainMeasIndex = 0;

         //*********************************************//
         //    PA & RGI Loop
         //*********************************************//
         for (UINT iPAIndex = 0; iPAIndex < sweep_param.PA_State_List.size(); iPAIndex ++)
         {
            for (UINT iRGIIndex = 0; iRGIIndex < sweep_param.RGI_List[iPAIndex].size(); iRGIIndex ++)
            {
               if (sweep_param.Do_FBGain_Cal)
               {
                  //*********************************************//
                  //    OpCode 0x8000: FBGain
                  //*********************************************//
                  byte debugMode = sweep_param.Enable_FBGain_Cal_Debug_Mode && (sweep_param.Ref_Channel == Tx_Channel_List[iLoopChannel]);
                  
                  byte bThermRead = 0;
                  if ( Tx_Channel_List[ iLoopChannel ] == sweep_param.Ref_Channel && iPAIndex == 0 && iRGIIndex == 0 
                     && sweep_param.FBGain_Cal_Do_Therm_Read )
                  {
                     if (demo_param.TECH != 3)
                     {
                        bThermRead = 1;
                     } else if ( demo_param.TECH == 3 && BW_List[ iLoopChannel ] == 3)
                     {
                        // in case of LTE, only 10 MHz required.
                         bThermRead = 1;
                     }
                  }
                  RFCal_Helper_setOpCode_txMeas_fbGainParams( Sweep[recordIndex], cal_param.TxDeviceFlag,sweep_param.FBGain_Cal_XPTMode_List[iPAIndex],
                                                              sweep_param.FBGain_Cal_GainComp_Val, sweep_param.FBGain_Cal_Gain_State_List[iPAIndex], sweep_param.FBGain_Cal_UBias_List[iPAIndex],
                     sweep_param.FBGain_Cal_RxScale_List[iPAIndex],debugMode, bThermRead);
                  fbGainMeasIndex++;
               }
                    else if ( sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_WTR_PWR)
                    {
                        //*********************************************//
                        //      OpCode 0x8000:  FB Power Measurement for WTR4905 Ref Cal
                        //*********************************************//
                        byte FBGainState = 0;
                        byte NumSample = 1;
                        RFCal_Helper_setOpCode_TxMeasure(Sweep[recordIndex], cal_param.TxDeviceFlag, ESC_TX_MEAS_ACTION_FB_POWER_MEAS, 0, 0, 0, 0, FBGainState, 0, 0, 0, NumSample);
                    }
               else
               {
                  //*********************************************//
                  //    OpCode 0x10000:   Configure WTR
                  //      Ref Cal for Tx Internal Cal of WTR 1625
                  //*********************************************//
                  RFCal_Helper_ESC_ET_ConfigureFB( Sweep[recordIndex],pDev, true, FBmode, fbDev, fbGainState, sweep_param.PA_State_List[iPAIndex],sweep_param.RGI_List[iPAIndex][iRGIIndex]);

                  //*********************************************//
                  //    OpCode 0x4020 = (OpCode 0x20 | OpCode 0x4000): Rx config and measurement for Tx
                  //*********************************************//
                  RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], 0 /* LNA_state */ ,  2 /* RxFlag */, sweep_param.Rx_PM_List[0] , 0 /* exRxAGC */ , 1 /* rx_meas_action */);
               }
               //*********************************************//
               //    0x200: Tx Override
               //*********************************************//
               if (sweep_param.PA_State_List[iPAIndex] == sweep_param.XPT_Swp1_PA_State)
               {
                  IQGainAction = 1;
                  IQGain = static_cast<word>(cal_param.PA_Param.IqGainRgiDeltaSwp);
                  Icq = cal_param.PA_Param.IcqRgiDeltaSwp;
               }
               else
               {
                  IQGainAction = 2;
                  IQGain = 0;
                  Icq = sweep_param.PA_Quiescent_Current[iPAIndex][iRGIIndex];
               }

               RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], sweep_param.Purpose_List[iPAIndex],
                                                 cal_param.TxDeviceFlag, XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                                 ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,        txParamMask_PwrOverride /* pwrParamMask */,
                                                 sweep_param.PA_State_List[iPAIndex], sweep_param.RGI_List[iPAIndex][iRGIIndex],   sweep_param.Bias_List[iPAIndex][iRGIIndex],
                                                 IQGainAction, IQGain,   EnvScaleAction, EnvScale, Icq, 0 /* delay action */, 0 /* delay value */);
               Sweep[recordIndex].measSeg = true;

                    TE_Records[recordIndex].m_ul_lvl = sweep_param.Max_Expected_Pwr[iPAIndex]; // To declare this segment a non-trigger segment
                    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
            } // end of for (int iRGIIndex = 0; iRGIIndex < sweep_param.RGI_List[iPAIndex].size(); iRGIIndex++)
         } // end of for (int iPAIndex = 0;  iPAIndex < sweep_param.PA_State_List.size(); iPAIndex ++)
      }
   } // end of if (sweep_param.FBRx_Cal_Mode == DO_REF_CAL)
   //*****************************************************************************************/
   //    End of Sweep 1: Insert a low rgi segment instead of tear down
   //*****************************************************************************************/
   RFCal_Helper_setLowRGISegment(Sweep[recordIndex],cal_param.TxDeviceFlag);
    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

   //*********************************************//
   //    Generate OpCode for all the segment
   //*********************************************//
   Sweep.resize(recordIndex); // Delete unused records
   RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());

   return true;
}
unsigned char RFCal_Helper_ESC_ET_RefCal_FBGainDataAssignment(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep, 
    map <RFCal_Helper_Tx_Params, double> & mapTxParmasToPwr)
{


   int seg = 0;
   for (vector <cal_sweep_record>::const_iterator it = Sweep.begin(); it != Sweep.end(); it ++)
   {
      seg++;
      if( ( (it->opcode.varLenOpcode.opCodeGrp.opCodeGrp0 & static_cast<qword>(0x8000)) 
         && (it->opcode.varLenOpcode.opCodeGrp.opCodeGrp0 & static_cast<qword>(0x200)))
         && it->measSeg) 
      {
    for (byte devIndex = 0; devIndex < 8; devIndex++)
    {
        if ((1 << devIndex) & cal_param.TxDeviceFlag)
        {
                    RFCal_Helper_Tx_Params TxParam;
                    TxParam.RFMode = static_cast<byte>(cal_param.RF_Mode);
                    TxParam.device = devIndex;
               TxParam.bw = static_cast<byte>(it->radio_cfg.chan.chanInfo[0].txBW);
                    TxParam.Channel = it->radio_cfg.chan.chanInfo[0].ulChan;
               TxParam.FBGainState = it->txMeasParams.txMeas[devIndex].txMeasPayload.fbGainParams.fbGainState;
               TxParam.uBias = it->txMeasParams.txMeas[devIndex].txMeasPayload.fbGainParams.uBias;
               TxParam.RxScale = it->txMeasParams.txMeas[devIndex].txMeasPayload.fbGainParams.rxScale;
                    TxParam.FBgain = it->txRes.fbGainMeas.fbGainRes;
                    double power = (it->txRes.txPower)/10.0;

                    mapTxParmasToPwr[TxParam] = power;
                }
         }
      }
   }

    return true;
}
unsigned char RFCal_Helper_ESC_ET_RefCal_CalculateOffset(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, const vector < cal_sweep_record > & Sweep,
                                                         Tx_RefCal_Data & data)
{
   //*********************************************//
   //    Calculate offset (FBand store data on memory
   //*********************************************//

   data.rf_mode = cal_param.RF_Mode;
   data.device = sweep_param.Device_Selection.Tx_Device_To_Calibrate[0];

   for (UINT i=0; i < Sweep.size(); i++)
   {
      if (Sweep[i].txPurpose == 3)
      {
         int TableIndex = -1;
         unsigned char bRecordFound = false;
         Tx_RefCal_Data_Table * tablePtr = NULL;
         Tx_RefCal_Data_Record * recordPtr = NULL;

         short offset;
         byte pa = Sweep[i].txParams.txParams[0].paState;
         byte rgi = Sweep[i].txParams.txParams[0].rgi;
         word channel = static_cast<word>(Sweep[i].radio_cfg.chan.chanInfo[0].ulChan);
         short power = static_cast<short>(Sweep[i].txRes.txPower);
         short FBAGC = static_cast<short>(Sweep[i].rxRes.lnaOffset[1][0]);

         offset = (demo_param.TECH == 3)? power + FBAGC :  power - FBAGC;

         for (UINT j=0; j < data.table.size(); j ++)
         {
            if (data.table[j].channel == channel && data.table[j].pa == pa)
            {
               tablePtr = &data.table[j];
               break;
            }
         }

         if (tablePtr == NULL)
         {
            Tx_RefCal_Data_Table table;
            table.channel = channel;
            table.pa = pa;
            data.table.push_back(table);
            tablePtr = &data.table.back();
         }
         for (UINT j=0; j < tablePtr->record.size(); j ++)
         {
            if (tablePtr->record[j].rgi == rgi)
            {
               tablePtr->record[j].offset = static_cast<int>(offset);
               tablePtr->record[j].power = static_cast<double>(power)/10.0;
               bRecordFound = true;
               break;
            }
         }
         if (!bRecordFound)
         {
            Tx_RefCal_Data_Record record;
            record.offset = offset;
            record.power =  static_cast<double>(power)/10.0;
            record.rgi = rgi;
            tablePtr->record.push_back(record);
         }
      }
   }
   return true;
}

unsigned char RFCal_Helper_ESC_ET_createINT_CAL_XPTSwp1(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, vector < cal_sweep_record > & Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
   //***********************************************************//
   //    Create Sweep 1 records
   //***********************************************************//

   unsigned char  bOK            = true;
   UINT           recordIndex    = 0;
   int txParamMask_PwrOverride  = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN |
   ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT | ESC_TX_OVERRIDE_DELAY); // Sweep
   byte pDev = RFCal_Helper_GetIndexFromBitMask(cal_param.TxDeviceFlag);
   byte devID = sweep_param.Device_Selection.Master_Device_ID;

   map <RFCal_Helper_Tx_Params, double > APTV6CharToPwr;
   map < pair < dword, byte >, Tx_RefCal_Data_V2_Record > ChanFBGainStateToPwrFBGainParams;

   if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
   {
      RFCal_Helper_ESC_ET_Tx_AptChar_DataV6_Assignment_XML(demo_param,  cal_param, sweep_param, APTV6CharToPwr); // Key: Channel. pa_state, rgi
      RFCal_Helper_ESC_ET_FBGain_Ref_Assignment_XML(demo_param,  cal_param, sweep_param, ChanFBGainStateToPwrFBGainParams);
   }

   if (sweep_param.Sweep_Type == XPT_SWP1)
   {
      txParamMask_PwrOverride |= ESC_TX_OVERRIDE_DELAY;
   }
   int            fbDev            =   static_cast<int>(sweep_param.Device_Selection.Feedback_Device_ID);;
   const int      fbEnabled      =  1;
   const int      fbDisabled     =  0;
   int            fbGainState    =  0;
   const unsigned char ENABLED   = true;
   const unsigned char DISABLED  = false;

//   int            Feedback_Device_ID         = static_cast<int>(sweep_param.Device_Selection.Feedback_Device_ID);   

   int            MasterDevState = 3;
   int            SlaveDevState = 0;

   unsigned char FBmode = false;

   //*****************************************************************************************//
   // OpCode 0x100: Radio Setup
   //*****************************************************************************************//
   RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, MasterDevState, SlaveDevState, demo_param.TECH, demo_param.BAND,
                                     sweep_param.Ref_Channel /* ulChan */, sweep_param.Waveform.Waveform_Type, NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);
   Sweep[recordIndex].radio_cfg.flag = sweep_param.Use_PilotOnlyWaveform;
   //if ( sweep_param.Fb_Rx_Gain_Cal_Mode != FBRX_UKERNEL_GAIN)
    {
      Sweep[recordIndex].radio_cfg.flag = 1;
   }
    RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,sweep_param.Config_Segment_Length); 


   //*****************************************************************************************//
   //  OpCode 0x1000: Sync OpCode (LTE Only)
   //*****************************************************************************************//
   if (demo_param.TECH == 3 )
   {
      RFCal_Helper_setOpCode_Sync(Sweep[recordIndex]);
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
      RFCal_helper_InsertNop(Sweep,recordIndex,1);
   }

    /* Enable Diveristy chain/configure the correct RxLM to get FB AGC */
   if ((  sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN
      || sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_IQ_GAIN )       
      )
   {
      //*****************************************************************************************//
      //  OpCode 0x10000: FBSetup   Feedback Action:  4  ( | Bit 2 = Configure RxLM for FB AGC read )
      //*****************************************************************************************//
      FBmode = true;
      RFCal_Helper_ESC_ET_OpCodeConfigureRxLM(demo_param, cal_param, sweep_param, Sweep, recordIndex, FBmode);
   }
   //****************************************************************//
   // Max Bias Linearizer (For PA for APTs)
   // Set Max Bias (e.g., 3700) to the bias configuration, and then measure the power.
   //****************************************************************//

   for(UINT paIndex = 0; paIndex < sweep_param.PA_State_List.size() ; paIndex++)
   {
      for (UINT rgiIndex = 0; rgiIndex < sweep_param.RGI_List[paIndex].size(); rgiIndex ++)
      {
         if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN)
         {
         //*********************************************//
         //    OpCode 0x10000:   Configure WTR
         //*********************************************//
            RFCal_Helper_ESC_ET_ConfigureFB(Sweep[recordIndex], pDev, true, FBmode, fbDev, fbGainState, sweep_param.PA_State_List[paIndex],sweep_param.Ref_RGI_List[paIndex][rgiIndex]);

         //*********************************************//
         //    OpCode 0x4020 = (OpCode 0x20 | OpCode 0x4000): Rx config and measurement for Tx
         //*********************************************//
         RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], 0 /* LNA_state */ ,  2 /* RxFlag */, 0 , 0 /* exRxAGC */ , 1 /* rx_meas_action */);
         }
         //***************************************************************************************//
         //    OpCode 0x200: TxOverride
         //***************************************************************************************//
         Sweep[recordIndex].measSeg = true;
         RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], sweep_param.Purpose_List[paIndex],
                                           cal_param.TxDeviceFlag, XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                           ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,
                                           txParamMask_PwrOverride /* pwrParamMask */,
                                           sweep_param.PA_State_List[paIndex], sweep_param.RGI_List[paIndex][rgiIndex],
                                           sweep_param.Bias_List[paIndex][0],ESC_TX_OVERRIDE_ACTION_SW_DEF, 0 /* IQ_Gain */,
                                           ESC_TX_OVERRIDE_ACTION_SW_DEF, 0 /* Env_Scale */, sweep_param.PA_Quiescent_Current[paIndex][0],ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);

         if (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)
         {
            //***************************************************************************************//
            //      OpCode 0x8000: TxMeas. FBGain Measurement 
            //***************************************************************************************//
            RFCal_Helper_setOpCode_FBMeasurement(sweep_param,  Sweep[recordIndex], sweep_param.PA_State_List[paIndex], sweep_param.RGI_List[paIndex][rgiIndex], sweep_param.Ref_RGI_List[paIndex][rgiIndex], NO_OVERLAP,   
            APTV6CharToPwr, ChanFBGainStateToPwrFBGainParams);
         }


#ifdef ON_EQUIPMENT
         double MaxExpectedPwr = sweep_param.Max_Expected_Pwr[paIndex] - floor(rgiIndex*0.7);

         if (APTV6CharToPwr.size() > 0)
         {
            RFCal_Helper_Tx_Params txParam;
            txParam.Channel = sweep_param.Ref_Channel;
            txParam.PaState = Sweep[recordIndex].txParams.txParams[0].paState;
            txParam.RGI = Sweep[recordIndex].txParams.txParams[0].rgi;
            txParam.Bias = Sweep[recordIndex].txParams.txParams[0].bias;
            txParam.PaQuiescentCurrent = Sweep[recordIndex].txParams.txParams[0].quiescentCurrent;
            txParam.IqGain = Sweep[recordIndex].txParams.txParams[0].iqGain;
            txParam.EnvScale = Sweep[recordIndex].txParams.txParams[0].envScale;
            txParam.delayVal = Sweep[recordIndex].txParams.txParams[0].delay;
            txParam.XPTmode = Sweep[recordIndex].txParams.txParams[0].xptMode;
            if (APTV6CharToPwr.find(txParam) != APTV6CharToPwr.end())
            {
               MaxExpectedPwr = APTV6CharToPwr[txParam] + 5.0;
            }
            else // Traditional case
            {
               RFCal_Helper_Tx_Params txParam2;
               txParam2.Channel = sweep_param.Ref_Channel;
               txParam2.RGI = txParam.RGI;
               txParam2.PaState = txParam.PaState;
      
               if (APTV6CharToPwr.find(txParam2) != APTV6CharToPwr.end())
               {
                  MaxExpectedPwr = APTV6CharToPwr[txParam2] + 5.0;
               }
            }     
         }
            TE_Records[recordIndex].m_ul_lvl = MaxExpectedPwr;
#endif
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex); 

         //***************************************************************************************//
         //    HDET Measurement
         //***************************************************************************************//
         if (sweep_param.HDET_Avgs.size() > 0 && sweep_param.HDET_Avgs[paIndex] > 0)
         {
            //*********************************************//
            //    OpCode 0x10000:   Configure WTR
            //*********************************************//
            RFCal_Helper_ESC_ET_ConfigureFB( Sweep[recordIndex],pDev,false, FBmode, fbDev, fbGainState, sweep_param.PA_State_List[paIndex],sweep_param.RGI_List[paIndex][rgiIndex]);

            //***************************************************************************************//
            //    OpCode 0x8000: TxMeasure (HDET Measure) (Seperate HDET and TxOverride)
            //***************************************************************************************//
            RFCal_Helper_setOpCode_TxMeasure(Sweep[recordIndex],Sweep[recordIndex].txParams.txDevice, (sweep_param.HDET_Avgs[paIndex]>0)?1:0, (byte)sweep_param.HDET_Avgs[paIndex]);
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
         }
         //***************************************************************************************//
         //      Overlap
         //***************************************************************************************//

         if( sweep_param.Do_FBGain_Cal && sweep_param.FBGain_Cal_Gain_State_Swpt_Ovrlp_Thrshld_List.front() != 0)
         {
            int tmpfbGainState =  static_cast<int>(Sweep[recordIndex - 1].txMeasParams.txMeas[devID].txMeasPayload.fbGainParams.fbGainState);
            for(int ovrlp_iter = OVERLAP_MINUS; ovrlp_iter <= OVERLAP_PLUS; ++ovrlp_iter)
            {
               //***************************************************************************************//
               //      OpCode 0x8000: TxMeas. FBGain Measurement 
               //***************************************************************************************//
               cal_sweep_record tmpSwpRecord;
               tmpSwpRecord = Sweep[recordIndex - 1];
               RFCal_Helper_setOpCode_FBMeasurement(sweep_param,  tmpSwpRecord, sweep_param.PA_State_List[paIndex], sweep_param.RGI_List[paIndex][rgiIndex], sweep_param.Ref_RGI_List[paIndex][rgiIndex], ovrlp_iter,   
               APTV6CharToPwr, ChanFBGainStateToPwrFBGainParams);

               if(tmpSwpRecord.txMeasParams.txMeas[devID].txMeasPayload.fbGainParams.fbGainState != tmpfbGainState && ovrlp_iter != NO_OVERLAP)
               {
                  Sweep[recordIndex] = tmpSwpRecord;
                  TE_Records[recordIndex] = TE_Records[recordIndex-1];
                  RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
               }
            }
         }
      }

      //*****************************************************************************************//
      // Insert NoP (OpCode 0x0) if MaxExpected power (i.e., DL level will be changed)
      //*****************************************************************************************//
      if (paIndex == (sweep_param.PA_State_List.size() - 1) || sweep_param.Max_Expected_Pwr[paIndex] != sweep_param.Max_Expected_Pwr[paIndex+1])
         RFCal_helper_InsertNop(Sweep,recordIndex, 1);
   }

   if (sweep_param.Sweep_Type == INT_CAL_XPT_SWP1)
   {
         //*****************************************************************************************//
         //  OpCode 0x10000: FBSetup   Feedback Action:  4  ( | Bit 2 = Configure RxLM for FB AGC read )
         //*****************************************************************************************//
         FBmode = false;
         RFCal_Helper_ESC_ET_OpCodeConfigureRxLM(demo_param, cal_param, sweep_param, Sweep, recordIndex, FBmode);
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

      //*****************************************************************************************/
      //    XPT Config
      //    OpCode 0x2000: XPT2
      //*****************************************************************************************/
      byte cfg_dev_id = sweep_param.Device_Selection.Master_Device_ID; 
      RFCal_Helper_setOpCode_XPT2(Sweep[recordIndex],  ESC_XPT_ACTION_CAL_CFG /* xptAction */, cfg_dev_id /* cfg_dev */, fbDev, XPT_TYPE_EPT); // Even ET only, use EPT
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

      //*****************************************************************************************/
      //    Load Unity DPD
      //    OpCode 0x800: Data
      //*****************************************************************************************/
      RFCal_Helper_setOpCodeData(Sweep[recordIndex], cal_param, sweep_param, DATA_OPCODE_ACTION_LOAD_DPD /* OpCode Data Option */, demo_param.TECH, demo_param.BAND);
      Sweep[recordIndex].data.nomDPDTbl.xptType    = LOAD_DPD_XPT_TYPE_EPT;
      Sweep[recordIndex].data.nomDPDTbl.numDET     = 0; // because EPT does not use DET
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

      RFCal_Helper_setOpCodeData(Sweep[recordIndex], cal_param, sweep_param, DATA_OPCODE_ACTION_LOAD_DPD /* OpCode Data Option */, demo_param.TECH, demo_param.BAND);
      Sweep[recordIndex].data.nomDPDTbl.xptType    = LOAD_DPD_XPT_TYPE_ET;
        RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

      //*****************************************************************************************/
      //    Alignment Sweep
      //    IQ Capture for 4 RGI  (Tx Override --> IQ capture)
      //*****************************************************************************************/

      unsigned char bFudgeSegment = true;
      UINT iETindex = (cal_param.En_EPTET_Different_PAstate == 1)? 2 : 1;
      for (UINT epEptLoopIndex = 0; epEptLoopIndex < iETindex; epEptLoopIndex ++)
      {
         byte XPT_Swp1_PA_State;
         vector <byte> XPT_Swp1_RGI_List;
         vector <word> XPT_Swp1_Bias_List;   
   
         if (cal_param.En_EPTET_Different_PAstate == 1)
         {
            if (epEptLoopIndex == 0) 
            {
               // EPT case
               XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State_EPT;
               XPT_Swp1_RGI_List = sweep_param.XPT_Swp1_RGI_List_EPT;   
               XPT_Swp1_Bias_List = sweep_param.XPT_Swp1_Bias_List_EPT; 
            }
            else     // ET case
            {
               XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State_ET;
               XPT_Swp1_RGI_List = sweep_param.XPT_Swp1_RGI_List_ET;   
               XPT_Swp1_Bias_List = sweep_param.XPT_Swp1_Bias_List_ET; 
             }
         }
         else
         {
            XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State;
               XPT_Swp1_RGI_List = sweep_param.XPT_Swp1_RGI_List;   
               XPT_Swp1_Bias_List = sweep_param.XPT_Swp1_Bias_List;    
         }
          for(UINT rgiIndex = 0; rgiIndex   < XPT_Swp1_RGI_List.size() ; rgiIndex++)
      {
         //*****************************************************************************************/
         //    Add fudgeSegment Before mode change
         //*****************************************************************************************/
         UINT iNumOfLoops = (bFudgeSegment)?2 :1 ;

         //*****************************************************************************************/
         //    OpCode 0x200: TxOverride
         //*****************************************************************************************/
         for (UINT i = 0; i < iNumOfLoops; i ++)
         {
               Sweep[recordIndex].subSwpType = (bFudgeSegment)? ESC_SUB_SWEEP_MAX:ESC_SUB_SWEEP_MAX;
               byte RGI = (bFudgeSegment)? XPT_Swp1_RGI_List[rgiIndex]: XPT_Swp1_RGI_List[rgiIndex]; // RGI = 0 if it is fudge segment

            RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], 0, cal_param.TxDeviceFlag, XPT_TYPE_EPT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                              ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,        txParamMask_PwrOverride /* pwrParamMask */,
                  XPT_Swp1_PA_State, RGI,   (dword)XPT_Swp1_Bias_List[rgiIndex], 
                                              ESC_TX_OVERRIDE_ACTION_USE_VAL, sweep_param.XPT_Swp1_IQ_Gain,
                                              ESC_TX_OVERRIDE_ACTION_USE_VAL, sweep_param.XPT_Swp1_Env_Scale,
                                              cal_param.PA_Param.IcqEt,ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);
               Sweep[recordIndex].measSeg = (bFudgeSegment)?false:false;

#ifdef ON_EQUIPMENT
         TE_Records[recordIndex].m_ul_lvl =  (bFudgeSegment)? sweep_param.XPT_Swp1_Max_Expected_Power :sweep_param.XPT_Swp1_Max_Expected_Power;
#endif

               RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

            //*****************************************************************************************/
            //    TDSCDMA only. Waveform setting for Alignment Sweep
            //*****************************************************************************************/
               if (bFudgeSegment && demo_param.TECH == 4)
            {
               //*****************************************************************************************//
               // OpCode 0x100: Radio Setup
               //*****************************************************************************************//
               byte txWfm = 5;
                  MasterDevState = 0;
               RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, MasterDevState, SlaveDevState, demo_param.TECH, demo_param.BAND,
                                                 sweep_param.Ref_Channel /* ulChan */, txWfm , NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);
                  Sweep[recordIndex].radio_cfg.flag = 0;
                  RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

               //*****************************************************************************************//
               // Insert NoP (OpCode 0x0)
               //*****************************************************************************************//
               RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);
                  RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
            }
            bFudgeSegment = false;
         }

         //*****************************************************************************************/
         //    OpCode 0x2000: XPT2 IQ capture
         //*****************************************************************************************/
   
         Sweep[recordIndex].txPurpose = (word)0;
         Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_MAX;
            byte FirstTrig = 0;
            byte LastTrig = 0;
            if ((cal_param.En_EPTET_Different_PAstate == 0 && rgiIndex==0 ) 
               || (cal_param.En_EPTET_Different_PAstate == 1 && epEptLoopIndex == 0  && rgiIndex==0 ))
            {
               FirstTrig = 1;
            }
            if ((cal_param.En_EPTET_Different_PAstate == 0 && rgiIndex== (XPT_Swp1_RGI_List.size()-1) ) 
               || (cal_param.En_EPTET_Different_PAstate == 1 && epEptLoopIndex == 1  && rgiIndex==  (XPT_Swp1_RGI_List.size()-1) ))
            {
               LastTrig = 1;
            }

            RFCal_Helper_setOpCode_XPT2(Sweep[recordIndex], ESC_XPT_ACTION_IQ_CAP /* xptAction */, cfg_dev_id /* cfg_dev */, fbDev, NULL, 
            ESC_XPT_CAP_EPT,sweep_param.XPT_Swp1_Num_IQ_Samples, FirstTrig, LastTrig,  ESC_XPT_CAP_PROC_SWP1 );
#ifdef ON_EQUIPMENT
         TE_Records[recordIndex].m_ul_lvl =  sweep_param.XPT_Swp1_Max_Expected_Power;
#endif

            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

            //*****************************************************************************************//
            //   Insert NoP (OpCode 0x0)
            //*****************************************************************************************//
            // Find the number of NOPs required to wait for the IQ Capture            
            RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.XPT_Swp1_Cap_Segment_Length, sweep_param.Segment_Length[0][0]);
         }
      }
         //*****************************************************************************************//
         //  OpCode 0x10000: FBSetup   Feedback Action:  4  ( | Bit 2 = Configure RxLM for FB AGC read )
         //*****************************************************************************************//
         FBmode = true;
         RFCal_Helper_ESC_ET_OpCodeConfigureRxLM(demo_param, cal_param, sweep_param, Sweep, recordIndex, FBmode);
      RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
      RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);
      }

   if (sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 && (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_AGC_GAIN)) 
   {
      unsigned char bFudgeSegment = true;
      for(UINT rgiIndex = 0; rgiIndex  < sweep_param.XPT_Swp1_RGI_List.size() ; rgiIndex++)
      {
         //*********************************************//
         //    OpCode 0x10000:   Configure WTR
         //*********************************************//
         RFCal_Helper_ESC_ET_ConfigureFB( Sweep[recordIndex], pDev, true,FBmode, fbDev, fbGainState, sweep_param.XPT_Swp1_PA_State, sweep_param.XPT_Swp1_RGI_Delta_Ref_RGI);

         //*********************************************//
         //    OpCode 0x4020 = (OpCode 0x20 | OpCode 0x4000): Rx config and measurement for Tx
         //*********************************************//
         RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], 0 /* LNA_state */ ,  2 /* RxFlag */, 0 , 0 /* exRxAGC */ , 1 /* rx_meas_action */);

         //*****************************************************************************************/
         //    OpCode 0x200: TxOverride
         //*****************************************************************************************/
         Sweep[recordIndex].subSwpType = ESC_SUB_SWEEP_XPT_ALIGNMENT;
         Sweep[recordIndex].opcode.segLen = static_cast<word>(sweep_param.XPT_Swp1_Cap_Segment_Length);

         RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], 0, cal_param.TxDeviceFlag, XPT_TYPE_EPT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                           ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,        txParamMask_PwrOverride /* pwrParamMask */,
                                           sweep_param.XPT_Swp1_PA_State,  sweep_param.XPT_Swp1_RGI_List[rgiIndex],   (dword)sweep_param.XPT_Swp1_Bias_List[rgiIndex],
                                           ESC_TX_OVERRIDE_ACTION_USE_VAL, sweep_param.XPT_Swp1_IQ_Gain,
                                           ESC_TX_OVERRIDE_ACTION_USE_VAL, sweep_param.XPT_Swp1_Env_Scale,
                                           cal_param.PA_Param.IcqEt,ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);
         Sweep[recordIndex].measSeg = (bFudgeSegment)?false:true;
#ifdef ON_EQUIPMENT
         TE_Records[recordIndex].m_ul_lvl =  sweep_param.XPT_Swp1_Max_Expected_Power;
#endif   
            RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

         //*****************************************************************************************/
         //    TDSCDMA only. Waveform setting for Alignment Sweep
         //*****************************************************************************************/
         if (!bFudgeSegment && demo_param.TECH == 4)
         {
            //*****************************************************************************************//
            // OpCode 0x100: Radio Setup
            //*****************************************************************************************//
            byte txWfm = 5;
            RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, MasterDevState, SlaveDevState, demo_param.TECH, demo_param.BAND,
                                              sweep_param.Ref_Channel /* ulChan */, txWfm , NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

            //*****************************************************************************************//
            // Insert NoP (OpCode 0x0)
            //*****************************************************************************************//
            RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);
         }
      }

      //*****************************************************************************************/
      //    RGI Delta Sweep
      //*****************************************************************************************/
      bFudgeSegment = true;
      for(UINT rgiIndex = 0; rgiIndex  < sweep_param.XPT_Swp1_RGI_Delta_RGI_List.size()   ;  rgiIndex++)
      {
         //*****************************************************************************************/
         //    Add fudgeSegment Before mode change
         //*****************************************************************************************/
         UINT iNumOfLoops = (bFudgeSegment)?2 :1 ;

         for (UINT i = 0; i < iNumOfLoops; i ++)
         {
            Sweep[recordIndex].subSwpType = (bFudgeSegment)?ESC_SUB_SWEEP_MAX:ESC_SUB_SWEEP_RGI_DELTA;
            Sweep[recordIndex].opcode.segLen = static_cast<word>(sweep_param.XPT_Swp1_RGI_Delta_Segment_Length);

            //*********************************************//
            //    OpCode 0x10000:   Configure WTR
            //*********************************************//
            RFCal_Helper_ESC_ET_ConfigureFB(Sweep[recordIndex], pDev, true,  FBmode, fbDev, fbGainState, sweep_param.XPT_Swp1_PA_State, sweep_param.XPT_Swp1_RGI_Delta_Ref_RGI);

            //*********************************************//
            //    OpCode 0x4020 = (OpCode 0x20 | OpCode 0x4000): Rx config and measurement for Tx
            //*********************************************//
            RFCal_Helper_setOpCode_RxConfigParameters(Sweep[recordIndex], 0 /* LNA_state */ ,  2 /* RxFlag */, 0 , 0 /* exRxAGC */ , 1 /* rx_meas_action */);

            //*****************************************************************************************/
            //    OpCode 0x200: TxOverride
            //*****************************************************************************************/
            RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],0, cal_param.TxDeviceFlag, XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */,
                                              ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,        txParamMask_PwrOverride /* pwrParamMask */,
                                              sweep_param.XPT_Swp1_PA_State, (byte)sweep_param.XPT_Swp1_RGI_Delta_RGI_List[rgiIndex],      sweep_param.XPT_Swp1_RGI_Delta_Bias, ESC_TX_OVERRIDE_ACTION_USE_VAL, cal_param.PA_Param.IqGainRgiDeltaSwp,
                                              ESC_TX_OVERRIDE_ACTION_SW_DEF, sweep_param.XPT_Swp1_Env_Scale, cal_param.PA_Param.IcqRgiDeltaSwp,  ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);
            Sweep[recordIndex].measSeg = (bFudgeSegment)?false:true;

            //*****************************************************************************************/
            //    TDSCDMA only.  Revert to default waveform before RGI Delta Sweep
            //*****************************************************************************************/
            if (!bFudgeSegment && demo_param.TECH == 4)
            {
               //*****************************************************************************************//
               // OpCode 0x100: Radio Setup
               //*****************************************************************************************//
               RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, MasterDevState, SlaveDevState, demo_param.TECH, demo_param.BAND,
                                                 sweep_param.Ref_Channel /* ulChan */, sweep_param.Waveform.Waveform_Type , NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);
                    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

               //*****************************************************************************************//
               // Insert NoP (OpCode 0x0)
               //*****************************************************************************************//
               RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);
            }
                RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
            bFudgeSegment = false;
         }
      }
   }
   if (sweep_param.Sweep_Type == INT_CAL_XPT_SWP1 && (sweep_param.Fb_Rx_Gain_Cal_Mode == FBRX_UKERNEL_GAIN)) 
   {
   //*****************************************************************************************/
      //      Waveform setting for Alignment Sweep
      //*****************************************************************************************/
      if ( demo_param.TECH == 1)
      {
         //*****************************************************************************************//
         //   OpCode 0x100: Radio Setup
         //*****************************************************************************************//
         byte txWfm = 4; //4 = Enhanced UL Waveform 
         RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, 0 /* MasterDevState */, 0 /* SlaveDevState */, demo_param.TECH, demo_param.BAND, 
            sweep_param.Ref_Channel /* ulChan */, txWfm , NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB, 7 /* Eul Config */);
         RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
      }
      else if ( demo_param.TECH == 4)
      {
         //*****************************************************************************************//
         //   OpCode 0x100: Radio Setup
         //*****************************************************************************************//
         byte txWfm = 0;
         RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, MasterDevState, SlaveDevState, demo_param.TECH, demo_param.BAND, 
            sweep_param.Ref_Channel /* ulChan */, txWfm , NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);
         RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

         //*****************************************************************************************//
         //   Insert NoP (OpCode 0x0)
         //*****************************************************************************************//
         RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);
      }





      //*****************************************************************************************/
      //      Alignment Sweep
      //      For Power Measurement
      //*****************************************************************************************/
      
      unsigned char bFudgeSegment = false; // For now.
      UINT iETindex = (cal_param.En_EPTET_Different_PAstate == 1)? 2 : 1;
      for (UINT epEptLoopIndex = 0; epEptLoopIndex < iETindex; epEptLoopIndex ++)
      {
         byte XPT_Swp1_PA_State;
         vector <byte> XPT_Swp1_RGI_List;
         vector <word> XPT_Swp1_Bias_List;   
         vector <double> XPT_Swp1_Power_List;   
   
         if (cal_param.En_EPTET_Different_PAstate == 1)
         {
            if (epEptLoopIndex == 0) 
            {
               // EPT case
               XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State_EPT;
               XPT_Swp1_RGI_List = sweep_param.XPT_Swp1_RGI_List_EPT;   
               XPT_Swp1_Bias_List = sweep_param.XPT_Swp1_Bias_List_EPT;
               XPT_Swp1_Power_List = sweep_param.XPT_Swp1_Power_List_EPT;
            }
            else     // ET case
            {
               XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State_ET;
               XPT_Swp1_RGI_List = sweep_param.XPT_Swp1_RGI_List_ET;   
               XPT_Swp1_Bias_List = sweep_param.XPT_Swp1_Bias_List_ET; 
               XPT_Swp1_Power_List = sweep_param.XPT_Swp1_Power_List_ET;
         }
         }
         else
         {
               XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State;
               XPT_Swp1_RGI_List = sweep_param.XPT_Swp1_RGI_List;   
               XPT_Swp1_Bias_List = sweep_param.XPT_Swp1_Bias_List;    
               XPT_Swp1_Power_List = sweep_param.XPT_Swp1_Power_List;
         }
         if (XPT_Swp1_Power_List.size() < XPT_Swp1_RGI_List.size())
         {
            for (vector<byte>::iterator it = XPT_Swp1_RGI_List.begin(); it != XPT_Swp1_RGI_List.end(); it++)
               XPT_Swp1_Power_List.push_back(sweep_param.XPT_Swp1_Max_Expected_Power);
         }

         for(UINT rgiIndex = 0; rgiIndex   < XPT_Swp1_RGI_List.size() ; rgiIndex++)
         {
            //*****************************************************************************************/
            //      Add fudgeSegment Before mode change
            //*****************************************************************************************/
            UINT iNumOfLoops = (bFudgeSegment)?2 :1 ;

            for (UINT i = 0; i < iNumOfLoops; i ++) 
            { 
               Sweep[recordIndex].subSwpType = (bFudgeSegment)? ESC_SUB_SWEEP_MAX:ESC_SUB_SWEEP_XPT_ALIGNMENT;
               byte RGI = (bFudgeSegment)? 0 : XPT_Swp1_RGI_List[rgiIndex]; // RGI = 0 if it is fudge segment


               //*****************************************************************************************/
               //      OpCode 0x200: TxOverride 
               //*****************************************************************************************/      

               RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex], 0, cal_param.TxDeviceFlag, XPT_TYPE_EPT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */, 
                  ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,         txParamMask_PwrOverride /* pwrParamMask */, 
                  XPT_Swp1_PA_State, RGI,   (dword)XPT_Swp1_Bias_List[rgiIndex], 
                  ESC_TX_OVERRIDE_ACTION_USE_VAL, sweep_param.XPT_Swp1_IQ_Gain, 
                  ESC_TX_OVERRIDE_ACTION_USE_VAL, sweep_param.XPT_Swp1_Env_Scale,   
                  cal_param.PA_Param.IcqEt,ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);
               Sweep[recordIndex].measSeg = (bFudgeSegment)?false:true;

               //***************************************************************************************//
               //      OpCode 0x8000: TxMeas. FBGain Measurement 
               //***************************************************************************************//
               if (!bFudgeSegment)
               {
                  RFCal_Helper_setOpCode_FBMeasurement(sweep_param,  Sweep[recordIndex], XPT_Swp1_PA_State, RGI, RGI, NO_OVERLAP,   
                  APTV6CharToPwr, ChanFBGainStateToPwrFBGainParams,XPT_Swp1_Power_List[rgiIndex]);
               }

               TE_Records[recordIndex].m_ul_lvl =  XPT_Swp1_Power_List[rgiIndex]+5.0;

               RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
               
               //*****************************************************************************************/
               //      TDSCDMA only. Waveform setting for Alignment Sweep
               //*****************************************************************************************/
               if (bFudgeSegment && demo_param.TECH == 4)
               {
                  //*****************************************************************************************//
                  //   OpCode 0x100: Radio Setup
                  //*****************************************************************************************//
                  byte txWfm = 5;
                  MasterDevState = 0;
                  RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, MasterDevState, SlaveDevState, demo_param.TECH, demo_param.BAND, 
                     sweep_param.Ref_Channel /* ulChan */, txWfm , NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);
                  Sweep[recordIndex].radio_cfg.flag = 0;
                  RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

                  //*****************************************************************************************//
                  //   Insert NoP (OpCode 0x0)
                  //*****************************************************************************************//
                  RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);
               }
               bFudgeSegment = false;
            }

         }
      }
      //*****************************************************************************************/
      //      Waveform setting for RGI delta sweep
      //*****************************************************************************************/
      if ( demo_param.TECH == 1 || demo_param.TECH == 4) // WCDMA or TDSCDMA
      {
         //*****************************************************************************************//
         //   OpCode 0x100: Radio Setup
         //*****************************************************************************************//
         byte txWfm = 0; //4 = Enhanced UL Waveform 
         RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, 0 /* MasterDevState */, 0 /* SlaveDevState */, demo_param.TECH, demo_param.BAND, 
            sweep_param.Ref_Channel /* ulChan */, txWfm , NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);
         RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
      }

      //*****************************************************************************************/
      //      RGI Delta Sweep
      //*****************************************************************************************/
      bFudgeSegment = true;
      iETindex = (cal_param.En_EPTET_Different_PAstate == 1)? 2 : 1;
      for (UINT epEptLoopIndex = 0; epEptLoopIndex < iETindex; epEptLoopIndex ++)
      {
         byte XPT_Swp1_PA_State;
         vector <byte> XPT_Swp1_RGI_Delta_RGI_List;
         vector <word> XPT_Swp1_Bias_List;
         if (cal_param.En_EPTET_Different_PAstate == 1)
         {
            if (epEptLoopIndex == 0) 
            {
               // EPT case
               XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State_EPT;
               XPT_Swp1_RGI_Delta_RGI_List = sweep_param.XPT_Swp1_RGI_Delta_RGI_List_EPT;
            }
            else     // ET case
            {
               XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State_ET;
               XPT_Swp1_RGI_Delta_RGI_List = sweep_param.XPT_Swp1_RGI_Delta_RGI_List_ET;
         }
         }
         else
         {
            XPT_Swp1_PA_State = sweep_param.XPT_Swp1_PA_State;
               XPT_Swp1_RGI_Delta_RGI_List = sweep_param.XPT_Swp1_RGI_Delta_RGI_List;
         }

         for(UINT rgiIndex = 0; rgiIndex   < XPT_Swp1_RGI_Delta_RGI_List.size()   ;   rgiIndex++)
         {
            //*****************************************************************************************/
            //      Add fudgeSegment Before mode change
            //*****************************************************************************************/
            UINT iNumOfLoops = (bFudgeSegment)?2 :1 ;

            for (UINT i = 0; i < iNumOfLoops; i ++) 
            {        
               Sweep[recordIndex].subSwpType   = (bFudgeSegment)?ESC_SUB_SWEEP_MAX:ESC_SUB_SWEEP_RGI_DELTA;   
               Sweep[recordIndex].opcode.segLen   = static_cast<word>(sweep_param.XPT_Swp1_RGI_Delta_Segment_Length);
   
               //*****************************************************************************************/
               //      OpCode 0x200: TxOverride 
               //*****************************************************************************************/
               RFCal_Helper_setOpCode_TxOverride(Sweep[recordIndex],0, cal_param.TxDeviceFlag, XPT_TYPE_APT, ESC_TX_OVERRIDE_MASK_POWER /* txOverrideMask */, 
                  ESC_TX_PWR_OVERRIDE_PARAMS /* pwrOverrideType */,         txParamMask_PwrOverride /* pwrParamMask */, 
                  XPT_Swp1_PA_State, (byte)XPT_Swp1_RGI_Delta_RGI_List[rgiIndex],      sweep_param.XPT_Swp1_RGI_Delta_Bias, ESC_TX_OVERRIDE_ACTION_USE_VAL, cal_param.PA_Param.IqGainRgiDeltaSwp, 
                  ESC_TX_OVERRIDE_ACTION_SW_DEF, sweep_param.XPT_Swp1_Env_Scale, cal_param.PA_Param.IcqRgiDeltaSwp,   ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV, 0);

               //***************************************************************************************//
               //      OpCode 0x8000: TxMeas. FBGain Measurement 
               //***************************************************************************************//
               if (!bFudgeSegment)
               {
                  RFCal_Helper_setOpCode_FBMeasurement(sweep_param,  Sweep[recordIndex], XPT_Swp1_PA_State, (byte)XPT_Swp1_RGI_Delta_RGI_List[rgiIndex], (byte)XPT_Swp1_RGI_Delta_RGI_List[rgiIndex], NO_OVERLAP,   
                  APTV6CharToPwr, ChanFBGainStateToPwrFBGainParams);
               }

               Sweep[recordIndex].measSeg = (bFudgeSegment)?false:true;
               if (XPT_Swp1_PA_State == sweep_param.XPT_Swp1_PA_State_EPT)
               {
                  TE_Records[recordIndex].m_ul_lvl = sweep_param.XPT_Swp1_Max_Expected_Power;
               }
               else
               {
                  TE_Records[recordIndex].m_ul_lvl = sweep_param.XPT_Swp1_Max_Expected_Power;

               }

               RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
               //*****************************************************************************************/
               //      TDSCDMA only.  Revert to default waveform before RGI Delta Sweep
               //*****************************************************************************************/
               if (bFudgeSegment && demo_param.TECH == 4)
               {
                  //*****************************************************************************************//
                  //   OpCode 0x100: Radio Setup
                  //*****************************************************************************************//
                  MasterDevState = 0;
                  RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], sweep_param.Device_Selection.Master_Device_ID, sweep_param.Device_Selection.Slave_Device_ID, MasterDevState, SlaveDevState, demo_param.TECH, demo_param.BAND, 
                     sweep_param.Ref_Channel /* ulChan */, sweep_param.Waveform.Waveform_Type , NULL, sweep_param.Waveform.Number_Of_RBs, sweep_param.Waveform.Start_RB);
                  Sweep[recordIndex].radio_cfg.flag = 0;
                  //*****************************************************************************************//
                  //   Insert NoP (OpCode 0x0)
                  //*****************************************************************************************//
                  //RFCal_helper_InsertNop(Sweep,recordIndex, sweep_param.Tuning_Segment_Length,sweep_param.Segment_Length[0][0]);
                  RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
               }
               
               bFudgeSegment = false;
            }
         }
      }
   }
   if (sweep_param.Fb_Rx_Gain_Cal_Mode == 2)
   {
      RFCal_Helper_ESC_ET_OpCodeConfigureRxLM(demo_param, cal_param, sweep_param, Sweep, recordIndex, false);
      RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);
   }
   //*****************************************************************************************/
   //    End of Sweep 1: Insert a low rgi segment instead of tear down
   //*****************************************************************************************/
   RFCal_Helper_setLowRGISegment(Sweep[recordIndex],cal_param.TxDeviceFlag);
   TE_Records[recordIndex].m_ul_lvl = 28 ;
    RFCal_Helper_RecordIndex_Increment(Sweep, TE_Records, recordIndex);

   //*********************************************//
   //    Generate OpCode for all the segment
   //*********************************************//
   Sweep.resize(recordIndex); // Delete unused records
   RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());

   return true;
}
unsigned short RFCal_Helper_GetRxCaltype( const cal_sweep_record * pSweepRecord, unsigned int rxChain )
{
    unsigned short rxCalType = RFCal_Helper_GetRxCaltype( pSweepRecord->swpType,
        pSweepRecord->rxMeas.actions[rxChain].rx_meas_action,
        (pSweepRecord->subSwpType == ESC_SUB_SWEEP_APT_CA) );
    return rxCalType;
}
unsigned short RFCal_Helper_GetRxCaltype( unsigned short swpType, unsigned int rxMeasAction, bool measCA )
{
    unsigned short rxCalType = 0xFF;

    if( swpType == INTRACA_FCOMP )
    {
        rxCalType = ESC_RX_CAL_TYPE_INTRA_CA;
    }
    else if( measCA )
    {
        rxCalType = ESC_RX_CAL_TYPE_INTER_CA;
    }
    else
    {
        if ( rxMeasAction == 0x1 )
            rxCalType = ESC_RX_CAL_TYPE_SC;
        else if ( rxMeasAction == 0x2 )
            rxCalType = ESC_RX_CAL_TYPE_DC;
        else if ( rxMeasAction == 0x8 )
            rxCalType = ESC_RX_CAL_TYPE_3C;
    }

    return rxCalType;
}
unsigned char RFCal_Helper_SeperateTx_RxSweeps(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, Sweep_Params & sweep_param, vector < TE_Cal_Swp_Params > & TE_Records)
{
    Sweep_Params curSweep_Param = sweep_param;
    if (sweep_param.Meas_Tx)
    {
        // ToDo:
    }
    if (sweep_param.Meas_Rx)
    {
        sweep_param.Sweep_Type = XPT_FREQCOMP_SERIAL_RX;
    }

    return true;
}
unsigned char RFCal_Helper_Seperate_CA_Sweeps(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, vector < vector < cal_sweep_record > > & Sweep, UINT iSweepIndex)
{
    unsigned char bOK = false;
    if (demo_param.TECH == 1) // WCDMA
    {
//      vector <Sweep_Params>::iterator it = cal_param.Swp.begin();
        Sweep_Params tmpSwp = cal_param.Swp[iSweepIndex];
        tmpSwp.Sweep_Type = INTERCA_SCELL_FCOMP;
        tmpSwp.Measure_CA = false;
        vector < cal_sweep_record > tmpSweeps;

        RFCal_Helper_ESC_ET_Initialize_Sweep(tmpSweeps,tmpSwp, 500,(int)demo_param.TECH,demo_param.BAND);

        if (cal_param.Swp.size() == (iSweepIndex+1))
        {
            cal_param.Swp.push_back(tmpSwp);
            Sweep.push_back(tmpSweeps);
        }
        else
        {
            cal_param.Swp.insert((cal_param.Swp.begin()+iSweepIndex+1),tmpSwp);
            Sweep.insert((Sweep.begin() + iSweepIndex+1), tmpSweeps);
        }
        bOK = true;
    }
    return bOK;
}
unsigned char RFCal_Helper_ESC_Execute_Serial_Rx_Measurement(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, 
    Sweep_Params & sweep_param, dword currChan, byte masterDev, byte CalType, map< rxOffsetRec, int> & rxParam_To_LnaOffset)
{
    HANDLE hPhone = demo_param.hPhone;
    byte TECH = demo_param.TECH;

    //Configure the frequency offset
    double dlOffsetMHz = 0.0;//default for cdma/wcdma.

    if( (demo_param.TECH == 3) || (demo_param.TECH == 4) )
    {
        dlOffsetMHz = 0.5;
    }

    // Set master device before tuning (CDMA will not work without this)        
    //RFCal_Helper_FTM_SET_SECONDARY_CHAIN(hPhone, rfMode, TECH, 0); // Comment out for now
        
    // Set Channel

    if (CalType == ESC_RX_CAL_TYPE_DC ) // WCDMA DC Dual Carrier case
    {
        const unsigned short  MULTICARRIER_SPACING = 25;
        RFCal_Helper_FTM_SET_MODE_ID(hPhone,1 /* WCDMA */, 0);
        QLIB_FTM_SET_MULTI_CHAN(hPhone,  static_cast<unsigned short>(currChan),  static_cast<unsigned short>(currChan + MULTICARRIER_SPACING));
    }
    else if (CalType == ESC_RX_CAL_TYPE_3C ) // WCDMA 3C Case 
    {
        vector <long> channels;
        const unsigned short  MULTICARRIER_SPACING = 25;
        channels.push_back( static_cast <long> (currChan) );
        channels.push_back( static_cast <long> (currChan + MULTICARRIER_SPACING) );
        channels.push_back( static_cast <long> (currChan + 2*MULTICARRIER_SPACING) );
        RFCal_Helper_FTM_SET_MODE_ID(hPhone,1 /* WCDMA */, 0);
        QLIB_FTM_SET_MULTI_CHAN_ENH( hPhone, (long*) channels[0], channels.size() );
    }
    else // Everything else case
    {
        QLIB_FTM_SET_CHAN(hPhone, static_cast<unsigned short>(currChan));
    }
        
    Sleep (sweep_param.Tuning_Segment_Length / 1000);

    for( unsigned int dlLvlLpVar = 0; dlLvlLpVar < sweep_param.DL_Level_List.size(); ++dlLvlLpVar )
    {

        word expRxAGC = (word)CalculateRxAGCValue(demo_param.TECH,sweep_param.DL_Level_List[dlLvlLpVar]); // 115 is min RSSI
        double currDlLevel = sweep_param.DL_Level_List[ dlLvlLpVar ];

        // Set DL Level for call box 
        // Add code
        vector<int> lna_offsets;
        lna_offsets.clear();
        unsigned char lnaState = 0;
        for (unsigned int rxDevIdx = 0; rxDevIdx < sweep_param.Device_Selection.Rx_Device_To_Calibrate.size(); ++rxDevIdx )
        {
            unsigned char RxPath = sweep_param.Device_Selection.Rx_Device_To_Calibrate[rxDevIdx];
            if (sweep_param.Device_Selection.Rx_Device_To_Calibrate[rxDevIdx] == masterDev)
            {
                // PRX :: Don't execute FTM_SET_SECONDARY_CHAIN
            }
            else
            {
                // DRX
                RFCal_Helper_FTM_SET_SECONDARY_CHAIN(hPhone, TECH, 0,1); 
            }
            if (TECH == 4) // TDSCDMA case
            {
                // For TDSCDMA, the channel should be re-tuned after the secondary chain state changes
                // This will inform FW to config rx per the prim/div settings
                ::QLIB_FTM_SET_CHAN(hPhone, static_cast<unsigned short>(currChan));

            }
            // DVGA Calibration (CDMA)
            if ( sweep_param.Rx_Actions_List[dlLvlLpVar] == 0 && TECH == 0 /* CDMA */ )
            {
                unsigned char count = 20;
                vector <unsigned short> array;
                array.resize(count);
                unsigned char status = QLIB_FTM_CDMA_API2_CALIBRATE_DVGA (hPhone, static_cast<unsigned char>(RxPath), expRxAGC, &count, &array[0]);
                    
                for(UINT i=0; i<count; ++i)
                {
                    lna_offsets.push_back( (signed short)array[i] );
                }

                if (status == 0)
                {
                //  throw RFCal_Helper_Exception("LIB_FTM_CDMA_API2_CALIBRATE_DVGA API failed.",  __FILE__, __LINE__ ); 
                }
            }
            // DVGA Calibration (WCDMA)
            else if ( sweep_param.Rx_Actions_List[dlLvlLpVar] == 0 && TECH == 1 /* WCDMA */ )
            {
                    short dvga;
                    unsigned char status = QLIB_FTM_GET_DVGA_OFFSET( hPhone, expRxAGC, &dvga);

                    lna_offsets.push_back( dvga );
                if (status == 0)
                {
                //  throw RFCal_Helper_Exception("QLIB_FTM_GET_DVGA_OFFSET API failed.",  __FILE__, __LINE__ ); 
                }
            }
            // LNA Calibration Other Case
            else
            {
                bool isIntelliceiver = (TECH == 0 /* CDMA */ );
                    

                if(TECH == 0 /* CDMA */ )
                    lnaState = sweep_param.Rx_Actions_List[dlLvlLpVar]-1; //For CDMA: DVGA, LNA0, LNA1, LNA2, ....
                else
                    lnaState = sweep_param.Rx_Actions_List[dlLvlLpVar];//For Techs other than CDMA: DVGA(or LNA0), LNA1, LNA2, LNA3, ....

                if ( isIntelliceiver )
                {
                    unsigned char count = 20;
                    vector <unsigned short> array;
                    array.resize(count);
                    unsigned char status = QLIB_FTM_CDMA_API2_CALIBRATE_LNA (hPhone, RxPath, expRxAGC, lnaState, &count, &array[0] );
                    lna_offsets.clear();
                    for(UINT i=0; i<count; ++i)
                    {
                        lna_offsets.push_back( (signed short)array[i] );
                    }

                    if(status == 0)
                    {
                //      throw RFCal_Helper_Exception("QLIB_FTM_CDMA_API2_CALIBRATE_LNA API failed.",  __FILE__, __LINE__ ); 
                    }
                }
                else 
                {
                    short lna_offset = 0;
                    unsigned char status = 0;

                    if (TECH == 3 /* LTE */ && RxPath == 0)
                    {
                        status = QLIB_FTM_SET_LNA_RANGE( hPhone, lnaState );
                        if(status == 0)
                        {
                    //      throw RFCal_Helper_Exception("QLIB_FTM_SET_LNA_RANGE API failed.",  __FILE__, __LINE__ ); 
                        }
                    }
                    status = QLIB_FTM_GET_LNA_OFFSET( hPhone,lnaState, expRxAGC, &lna_offset);

                    if (TECH == 3 /* LTE */ && RxPath == 0)
                    {
                        if ( lnaState == 0 && ((lna_offset>100)||(lna_offset<-100)))
                        {
                            status = QLIB_FTM_SET_LNA_RANGE( hPhone, 1 );
                            status = QLIB_FTM_GET_LNA_OFFSET( hPhone, 1, expRxAGC, &lna_offset);

                            status = QLIB_FTM_SET_LNA_RANGE( hPhone, lnaState );
                            status = QLIB_FTM_GET_LNA_OFFSET( hPhone, lnaState, expRxAGC, &lna_offset);
                        }

                    }
                    lna_offsets.clear();
                    lna_offsets.push_back( lna_offset );
                } // else 
            } // else// LNA Calibration Other Case
            // Save the results
            for(unsigned int rxPmLpVar = 0; rxPmLpVar < sweep_param.Rx_PM_List.size(); ++rxPmLpVar)
            {

                rxOffsetRec rxOffset(static_cast<word>(currChan),RxPath,lnaState,sweep_param.Rx_PM_List[rxPmLpVar], CalType);

                if (demo_param.bSimulationMode)
                {
                    // Assign dummay LNA offset data just for demonstartion on Simulation mode
                    lna_offsets[rxPmLpVar] = -50+10*lnaState+rxPmLpVar*2+dlLvlLpVar+currChan - sweep_param.Ref_Channel;
                }
                rxParam_To_LnaOffset[rxOffset] = lna_offsets[rxPmLpVar];
            }

        } // Rx Device Loop for (unsigned int rxDevIdx = 0; rxDevIdx < sweep_param.Device_Selection.Rx_Device_To_Calibrate.size(); ++rxDevIdx )
    } // Rx DL Loop for( unsigned int dlLvlLpVar = 0; dlLvlLpVar < sweep_param.DL_Level_List.size(); ++dlLvlLpVar )

        return true;
}

unsigned char RFCal_Helper_ESC_Execute_Serial_Rx(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, Sweep_Params & sweep_param,
    vector < cal_sweep_record >&        Sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
    //This function is intended only to perform non-sweep mode Rx calibration(both TE and UUT side).
    if (sweep_param.Sweep_Type != XPT_FREQCOMP_SERIAL_RX)
    {
        return false;
    }

    if (sweep_param.Device_Selection.Rx_Device_To_Calibrate.size() == 0)
    {
        return false;
    }

    static FTM_RF_Mode_Enum prev_rfMode = PHONE_MODE_MAX;
    static byte prev_RxDeviceFlag = 0xFF;
    HANDLE hPhone = demo_param.hPhone;
    byte TECH = demo_param.TECH;
    map< rxOffsetRec, int> rxParam_To_LnaOffset;;

    // ************************************************************ //
    // Configure RF Mode on DUT
    // ************************************************************ //
    byte masterDev = sweep_param.Device_Selection.Master_Device_ID;


    //In CDMA, Master Device has 2 power modes and slave device has 1 power mode, so create new power mode list per rx device
    //vector< vector<int> > rxPwrModesPerDevice;
    //rxPwrModesPerDevice.resize( rxDevices.size() );
    //vector<int> numPwrModesPerDevice;
    //for(unsigned int rxDevLpVar = 0; rxDevLpVar < rxDevices.size();  ++rxDevLpVar)
    //{
    //  if( rxDevices[rxDevLpVar] == masterDev)
    //  {
    //      rxPwrModesPerDevice[rxDevLpVar] = rxPwrModes;
    //  }
    //  else//Slave device always has one power mode
    //  {
    //      rxPwrModesPerDevice[rxDevLpVar].push_back(0);
    //  }

    //  numPwrModesPerDevice.push_back( rxPwrModesPerDevice[rxDevLpVar].size() );
    //}
    FTM_RF_Mode_Enum rfMode = static_cast<FTM_RF_Mode_Enum>(cal_param.RF_Mode);
//  if(prev_rfMode != rfMode || prev_RxDeviceFlag != addSwpParams->devParams.m_RxDeviceFlag )
    {
        RFCal_Helper_FTM_SET_MODE(hPhone, rfMode, TECH, masterDev);
        Sleep( sweep_param.Config_Segment_Length/1000 );
        prev_rfMode = rfMode;
//      prev_RxDeviceFlag = addSwpParams->devParams.m_RxDeviceFlag;
    }
    
    if (demo_param.TECH == 3)// LTE
    {
        RFCal_Helper_FTM_SET_MODE_ID(hPhone, 3, masterDev);
        QLIB_FTM_LTE_SET_TX_BANDWIDTH(hPhone, 3 /* 10 MHz */);
        QLIB_FTM_LTE_SET_RX_BANDWIDTH(hPhone, 3 /* 10 MHz */);
    }

    for (UINT chLpVar = 0; chLpVar < sweep_param.Tx_Channel_List.size(); ++chLpVar)
    {
        dword currChan = static_cast<dword>(sweep_param.Tx_Channel_List[chLpVar]);
        RFCal_Helper_ESC_Execute_Serial_Rx_Measurement( demo_param, cal_param, sweep_param, currChan,  masterDev, ESC_RX_CAL_TYPE_SC /* SC */, rxParam_To_LnaOffset);
    } 
    if (sweep_param.Measure_DC ) 
    {
        dword currChan = sweep_param.Ref_Channel;
        RFCal_Helper_ESC_Execute_Serial_Rx_Measurement( demo_param, cal_param, sweep_param, currChan,  masterDev, ESC_RX_CAL_TYPE_DC /* DC */, rxParam_To_LnaOffset);
    }
    if (sweep_param.Measure_3C ) 
    {
        dword currChan = sweep_param.Ref_Channel;
        RFCal_Helper_ESC_Execute_Serial_Rx_Measurement( demo_param, cal_param, sweep_param, currChan,  masterDev, ESC_RX_CAL_TYPE_3C /* 3C */, rxParam_To_LnaOffset);
    }

    // **************************************************************************** //
    // Populate Rx Result in the sweep
    // **************************************************************************** //


    for (UINT i=0; i < Sweep.size(); i ++)
    {
        cal_sweep_record *pSeg = &Sweep[i];

        if (pSeg->resExpected)
        {
            unsigned int dev = 0;
            for( byte devMask = 0x1; dev < MAX_NUM_DEVICES; dev++, devMask <<= 1 )
            {
                if( devMask & pSeg->rxMeas.dev )
                {
                    unsigned short meas = 0;
                    for( byte measMask = 0x1; meas < MAX_NUM_MEAS_ACTIONS; meas++, measMask <<= 1 )
                    {
                        if(measMask & pSeg->rxMeas.actions[dev].rx_meas_action)
                        {
                            //ConvertBitMaskToBitPosition(segItr->rxParams.rxActions, rxAction);
                            unsigned int rxCalType = RFCal_Helper_GetRxCaltype( pSeg, dev );
                            word ulChan = static_cast<word>(pSeg->radio_cfg.chan.chanInfo[0].ulChan);
                            
                            byte lnaState = pSeg->rxMeas.actions[dev].payloads[0].lna_state;
                            byte pwrMode = pSeg->rxMeas.actions[dev].payloads[0].power_mode;
                            rxOffsetRec rxOffset(ulChan,dev,lnaState,pwrMode, rxCalType);
                            pSeg->rxRes.lnaOffset[dev][meas] = rxParam_To_LnaOffset[rxOffset];
                        }
                    }

                }
            }
        }


    }

    
    return true;
}
unsigned char RFCal_Helper_FTM_SET_SECONDARY_CHAIN(HANDLE hPhone, byte Tech, byte isMasterDev, byte RxPath)
{
    if (isMasterDev == 1)
    {
        return true;
    }
    unsigned char status = 1;

    const long rxPath2 = 2; //path number 2, used in dual transceiver devices
   
    RFCal_Helper_FTM_SET_MODE_ID(hPhone,Tech,RxPath);

    if( RxPath >= rxPath2 && Tech == 3 ) //for LTE, do not set the secondary chain if rx path is 2 or higher
    {
        status = 1;
    }
    else
    {
        status = QLIB_FTM_SET_SECONDARY_CHAIN( hPhone, 1 );
    }
    return true;
}
unsigned char RFCal_Helper_FTM_SET_MODE(HANDLE hPhone, FTM_RF_Mode_Enum mode, byte Tech, byte RxPath)
{
    RFCal_Helper_FTM_SET_MODE_ID (hPhone, Tech, RxPath);
    unsigned char status = 0;

    status = QLIB_FTM_SET_MODE(hPhone, mode);

    return status;
}

unsigned char RFCal_Helper_FTM_SET_MODE_ID(HANDLE hPhone,byte Tech, byte RxPath)
{
    FTM_Mode_Id_Enum mode_id = FTM_MODE_ID_CDMA_1X;
    
    switch (Tech)
    {
        case 0:// CDMA
            switch (RxPath)
            {
              case 0:
                 mode_id = FTM_MODE_ID_CDMA_1X;
                 break;
              case 1:
                 mode_id = FTM_MODE_ID_CDMA_1X_RX1;
                 break;
              case 2:
                 mode_id = FTM_MODE_ID_CDMA_1X_C2;
                 break;
              case 3:
                 mode_id = FTM_MODE_ID_1X_C3_C;
                 break;
              case 4:
                 mode_id = FTM_MODE_ID_1X_C4_C;
                 break;
              default:
                 throw RFCal_Helper_Exception("Rx Path is not supported",  __FILE__, __LINE__ );
                 break;
            }
            break;
        case 1: // WCDMA
            switch( RxPath )
            {
                case 0:
                case 2:
                    mode_id = FTM_MODE_ID_WCDMA;
                    break;
                case 1:
                    mode_id = FTM_MODE_WCDMA_RX1;
                    break;
                default:
                    throw RFCal_Helper_Exception("Rx Path is not supported",  __FILE__, __LINE__ );
                    break;
            }
             break;
        case 2: // GSM
            switch( RxPath )
            {    
                case 0:
                    mode_id = FTM_MODE_ID_GSM;
                    break;
                case 1:
                case 2:
                    mode_id = FTM_MODE_ID_GSM_C2;
                    break;
                default:
                    throw RFCal_Helper_Exception("Rx Path is not supported",  __FILE__, __LINE__ );
                    break;
            }
            break;
        case 3: // LTE
          switch( RxPath )
          {
              case 0:
                 mode_id = FTM_MODE_ID_LTE;
                 break;
              case 1:
                 mode_id = FTM_MODE_LTE_RX1;
                 break;
              case 2:
                 mode_id = FTM_MODE_ID_LTE_C3;
                 break;
              case 3:
                 mode_id = FTM_MODE_ID_LTE_C4;
                 break;
              default:
                 throw RFCal_Helper_Exception("Rx Path is not supported",  __FILE__, __LINE__ );
                 break;
          }
            break;
        case 4:
          switch( RxPath )
          {
          case 0:
             mode_id = FTM_MODE_ID_TDSCDMA;
             break;
          case 1:
             mode_id = FTM_MODE_ID_TDSCDMA_RX1;
             break;
          default:
             throw RFCal_Helper_Exception("Rx Path is not supported",  __FILE__, __LINE__ );
        }
         break;
        default:
            throw RFCal_Helper_Exception("Technology is not supported",  __FILE__, __LINE__ );
            break;
    }
    QLIB_FTM_SET_MODE_ID(hPhone, mode_id);
    return true;
}


int getCommaCount(string tmp)
{
   int count =0;
   UINT position = 0;
   while(((position = tmp.find(",",position))!=std::string::npos) && (position <= tmp.length()))
   {
      count++;
      position++;
   }

   return count;
}

unsigned char addHeader(string* stHeader1stLine, string* stHeader2ndLine,  string stHeader1,string stHeader2)
{
   (*stHeader1stLine).append(stHeader1);
   (*stHeader2ndLine).append(stHeader2);
   int count = getCommaCount(stHeader2);
   for (int i = 0; i< count; i++)
   {
      (*stHeader1stLine).append(",");
   }

   return true;
}
unsigned char addArrayHeader(string* stHeader1stLine, string* stHeader2ndLine,   string stArrayName,int count)
{
   for (int i = 0; i< count; i++)
   {
      (*stHeader1stLine).append(",");
      stringstream ss;
      ss << stArrayName << "[" << i << "],";
      (*stHeader2ndLine).append(ss.str());
   }
   return true;
}
unsigned char ExportSweepRecordToCSV(cal_sweep_record* tmp, string filename, int size)
{
    return ExportSweepRecordToCSV(tmp, NULL,filename, size); 
}
unsigned char ExportSweepRecordToCSV(cal_sweep_record* tmp, TE_Cal_Swp_Params * teSweep,string filename, int size) 
{
   ofstream Logfile;
   Logfile.open(filename.c_str());
   string stHeader1stLine = "";
   string stHeader2ndLine = "";
   int m,k;
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "SegNum",          "Seg Number,");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "ESC Parameters",  "swpType,subSwpType,txPurpose,isCfgSeg,isNopSeg,measSeg,resExpected,");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "OpCode",          "OpCode,SegmentLength,");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "RadioSetup (0x100)", "RadioVer,RadioDebug,RadioFlag,MasterDev,SlaveDev,MasterDevState,SlaveDevState,Band,Tech,NumChan,ChannelINfoUL,txBW,rxBW,txWaveform,numRB,StartRB,eulConfig,");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "TxOverride (0x200)", "txParamTxDevice,xptMode,txOverrideMask,pwrOverrideType,pwrParamMask,paState,RGI,BIAS,iqGainAction,iqGain,envScaleAction,envScale,quiescentCurrent,delayAction,delay,expPwr,refChan,dpdIndex,");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "TxMeasure (0x8000)", "TxMeasureDevice,TxMeasAction,numHdetAvg,numLpmHdetAvg,");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "TxRes (",               "TxResPower,HDET[0][0],HDET[0][1],THERM[0][2],");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "RxParams (0x20)",          "RxParamsExpRxAGC,rxActions,rxFlag,rxPM,");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "RxRes",              "RxResLnaOffset[0][0],lnaOffset[0][1],lnaOffset[1][0],lnaOffset[1][1],LnaOffset[2][0],lnaOffset[2][1],lnaOffset[3][0],lnaOffset[3][1],");

   addHeader(&stHeader1stLine ,&stHeader2ndLine , "Data (0x800)",          "Data.tech,data.refChan,data.dev,data.doDataOp,dataAction,");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "Data.rgiVsPwr",            "Data.rgiVsPwr.doRgiVsPwrOp,numRGIDeltas,rgi[0],rgi[1],pwr[0],pwr[1],");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "Data.nomDPDTbl",           "Data.nomDPDTbl.doDPDLoadOp,xptType,numAMAM,AMAM[0],AMAM[1],AMAM[2],numAMPM,AMPM[0],AMPM[1],AMPM[2],numDET,DET[0],DET[1],DET[2],");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "Data.mptCalc",          "Data.mptCalc.doMPtCalcOp,linRegPct,compDBEpt,compDBEt,paEpt,paEt,curveFitType,vdMaxEpt,vdMaxEt,mlineBiasOption,numSwp1MeasPwrs,numSwp2EptMeasPwrs,numSwp2EptCalcPwrs,numSwp2EtMeasPwrs,numSwp2EtCalcPwrs,");
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "swp1pa",4);
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "swp1rgi",4);
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "swp1bias",4);
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "measPwrSwp1DB10",4);

   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "eptMeasPwrSwp2DB100",4);
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "eptCalcPwrSwp2DB100",4);
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "etMeasPwrSwp2DB100",4);
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "etCalcPwrSwp2DB100",4);

   addHeader(&stHeader1stLine ,&stHeader2ndLine , "Data.delayValProc",           "Data.delayValProc.doDelayValProcOp, upperChannel,");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "DataRes.delay",               "DelayRes.delayChan,delayBw,delayVal,");

   addHeader(&stHeader1stLine ,&stHeader2ndLine , "Data.DpdCalc",          "Data.dpdCalc.do_dpd_calc_op, xptMode, numSwp2MeasPwrs, numSwp2CalcPwrs,");
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "DpdCalc.measPwrSwp2DB100",4);
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "DpdCalc.calcPwrSwp2DB100",4);

   addHeader(&stHeader1stLine ,&stHeader2ndLine , "Data.LoadCalcDpd",            "Data.loadCalcDpd.do_load_calc_dpd_op, loadAction, xptMode, qpoetMinV, qpoetMaxV, etMinV, numSwp2MeasPwrs, numSwp2CalcPwrs,");
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "loadCalcDpd.measPwrSwp2DB100",4);
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "loadCalcDpd.calcPwrSwp2DB100",4);

   addHeader(&stHeader1stLine ,&stHeader2ndLine , "DataRes.measDpdRes",          "dataRes.measDpdRes[0].dpdValid,xptMode,rgi,smpsBias,paState,measured,power,");
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "amamRes",2);
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "ampmRes",2);

   addHeader(&stHeader1stLine ,&stHeader2ndLine , "DataRes.calcDpdRes",          "dataRes.calcDpdRes[0].dpdValid,xptMode,rgi,smpsBias,paState,measured,power,");
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "amamRes",2);
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "ampmRes",2);

   addHeader(&stHeader1stLine ,&stHeader2ndLine , "DataRes.measLoadEPDTRes",           "dataRes.measLoadEPDTRes[0].detValid, measured,");
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "epdtTbl",1);

   addHeader(&stHeader1stLine ,&stHeader2ndLine , "DataRes.calcLoadEPDTRes",           "dataRes.calcLoadEPDTRes[0].detValid, measured,");
   addArrayHeader(&stHeader1stLine ,&stHeader2ndLine, "epdtTbl",1);

   addHeader(&stHeader1stLine ,&stHeader2ndLine , "XPT (0x2000)",          "xptAction, xpt_cfg.dev, xpt_cfg.fbDev, xpt_cfg.xptMode,");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "XPT.IQCapture",            "xptCaptureIQ.dev, fbDev, capType, numSamples, firstTrig, lastTrig, procFlag,");
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "RxMeasure (0x4000)",
             "rxMeas.dev,actions[0].rx_meas_action,payloads[0].lna_state,power_mode,exp_rxagc,payloads[1].lna_state,power_mode,exp_rxagc,"
             "actions[1].rx_meas_action,payloads[0].lna_state,power_mode,exp_rxagc,payloads[1].lna_state,power_mode,exp_rxagc,"
             "rxMeas.dev,actions[2].rx_meas_action,payloads[0].lna_state,power_mode,exp_rxagc,payloads[1].lna_state,power_mode,exp_rxagc,"
             "actions[3].rx_meas_action,payloads[0].lna_state,power_mode,exp_rxagc,payloads[1].lna_state,power_mode,exp_rxagc,"

            );
   addHeader(&stHeader1stLine ,&stHeader2ndLine , "Sync (0x1000)",            "SyncEvent, device,");

   const int MAX = 2;
   const int MAX4 = 4;

   Logfile << stHeader1stLine << "\n " ;
   Logfile << stHeader2ndLine << "\n " ;

   for (int i = 0; i < size; i++)
   {
      Logfile << i
              << ", " << (long) tmp->swpType
              << ", " << (long) tmp->subSwpType
              << ", " << (long) tmp->txPurpose

              << ", " << (long) tmp->isCfgSeg
              << ", " << (long) tmp->isNopSeg
              << ", " << (long) tmp->measSeg
              << ", " << (long) tmp->resExpected

              << ", " << std::hex << (long) tmp->opcode.varLenOpcode.opCodeGrp.opCodeGrp0 << std::dec  /* To show OpCode 0x10000 or larger opcode */
              << ", " << (long) tmp->opcode.segLen

              // Radio Setup
              << ", " << (long) tmp->radio_cfg.version
              << ", " << (long) tmp->radio_cfg.debugMode
              << ", " << (long) tmp->radio_cfg.flag

              << ", " << (long) tmp->radio_cfg.device.masterDev
              << ", " << (long) tmp->radio_cfg.device.slaveDev
              << ", " << (long) tmp->radio_cfg.device.masterDevState
              << ", " << (long) tmp->radio_cfg.device.slaveDevState
              << ", " << (long) tmp->radio_cfg.band
              << ", " << (long) tmp->radio_cfg.tech

              << ", " << (long) tmp->radio_cfg.chan.numChans
              << ", " << (long) tmp->radio_cfg.chan.chanInfo[0].ulChan
              << ", " << (long) tmp->radio_cfg.chan.chanInfo[0].txBW
              << ", " << (long) tmp->radio_cfg.chan.chanInfo[0].rxBW

              << ", " << (long) tmp->radio_cfg.wfm.txWaveform
              << ", " << (long) tmp->radio_cfg.wfm.numRBs
              << ", " << (long) tmp->radio_cfg.wfm.startRB
              << ", " << (long) tmp->radio_cfg.wfm.eulConfig

              << ", " << (long) tmp->txParams.txDevice

              << ", " << (long) tmp->txParams.txParams[0].xptMode
              << ", " << (long) tmp->txParams.txParams[0].txOverrideMask
              << ", " << (long) tmp->txParams.txParams[0].pwrOverrideType
              << ", " << (long) tmp->txParams.txParams[0].pwrParamMask
              << ", " << (long) tmp->txParams.txParams[0].paState
              << ", " << (long) tmp->txParams.txParams[0].rgi
              << ", " << (long) tmp->txParams.txParams[0].bias
              << ", " << (long) tmp->txParams.txParams[0].iqGainAction
              << ", " << (long) tmp->txParams.txParams[0].iqGain
              << ", " << (long) tmp->txParams.txParams[0].envScaleAction
              << ", " << (long) tmp->txParams.txParams[0].envScale
              << ", " << (long) tmp->txParams.txParams[0].quiescentCurrent
              << ", " << (long) tmp->txParams.txParams[0].delayAction
              << ", " << (long) tmp->txParams.txParams[0].delay
              << ", " << (long) tmp->txParams.txParams[0].expPwr
              << ", " << (long) tmp->txParams.txParams[0].refChan
              << ", " << (long) tmp->txParams.txParams[0].dpdIndex

              << ", " << (long) tmp->txMeasParams.txMeasFlag
              << ", " << (long) tmp->txMeasParams.txMeas[0].txMeasAction
              << ", " << (long) tmp->txMeasParams.txMeas[0].txMeasPayload.numHdetAvg
              << ", " << (long) tmp->txMeasParams.txMeas[0].txMeasPayload.numLpmHdetAvg

              << ", " << (word) tmp->txRes.txPower
              << ", " << (word) tmp->txRes.hdet[0][0]
              << ", " << (word) tmp->txRes.hdet[0][1]
              << ", " << (word) tmp->txRes.hdet[0][2]

              << ", " << (long) tmp->rxParams.expRxAGC
              << ", " << (long) tmp->rxParams.rxActions
              << ", " << (long) tmp->rxParams.rxFlag
              << ", " << (long) tmp->rxParams.rxPM

              << ", " << (long) tmp->rxRes.lnaOffset[0][0]
              << ", " << (long) tmp->rxRes.lnaOffset[0][1]
              << ", " << (long) tmp->rxRes.lnaOffset[1][0]
              << ", " << (long) tmp->rxRes.lnaOffset[1][1]
              << ", " << (long) tmp->rxRes.lnaOffset[2][0]
              << ", " << (long) tmp->rxRes.lnaOffset[2][1]
              << ", " << (long) tmp->rxRes.lnaOffset[3][0]
              << ", " << (long) tmp->rxRes.lnaOffset[3][1]

              << ", " << (long) tmp->data.tech
              << ", " << (long) tmp->data.refChan
              << ", " << (long) tmp->data.dev
              << ", " << (long) tmp->data.doDataOp

              << ", " << (long) tmp->data.dataAction

              << ", " << (long) tmp->data.rgiVsPwr.doRgiVsPwrOp
              << ", " << (long) tmp->data.rgiVsPwr.numRGIDeltas
              << ", " << (long) tmp->data.rgiVsPwr.rgi[0]
              << ", " << (long) tmp->data.rgiVsPwr.rgi[1]
              << ", " << (long) tmp->data.rgiVsPwr.pwr[0]
              << ", " << (long) tmp->data.rgiVsPwr.pwr[1]

              << ", " << (long) tmp->data.nomDPDTbl.doDPDLoadOp
              << ", " << (long) tmp->data.nomDPDTbl.xptType
              << ", " << (long) tmp->data.nomDPDTbl.numAMAM
              << ", " << (long) tmp->data.nomDPDTbl.AMAM[0]
              << ", " << (long) tmp->data.nomDPDTbl.AMAM[1]
              << ", " << (long) tmp->data.nomDPDTbl.AMAM[2]

              << ", " << (long) tmp->data.nomDPDTbl.numAMPM
              << ", " << (long) tmp->data.nomDPDTbl.AMPM[0]
              << ", " << (long) tmp->data.nomDPDTbl.AMPM[1]
              << ", " << (long) tmp->data.nomDPDTbl.AMPM[2]

              << ", " << (long) tmp->data.nomDPDTbl.numDET
              << ", " << (long) tmp->data.nomDPDTbl.DET[0]
              << ", " << (long) tmp->data.nomDPDTbl.DET[1]
              << ", " << (long) tmp->data.nomDPDTbl.DET[2]

              << ", " << (long) tmp->data.mptCalc.doMPtCalcOp
              << ", " << (long) tmp->data.mptCalc.linRegPct
              << ", " << (long) tmp->data.mptCalc.compDBEpt
              << ", " << (long) tmp->data.mptCalc.compDBEt
              << ", " << (long) tmp->data.mptCalc.paEpt
              << ", " << (long) tmp->data.mptCalc.paEt
              << ", " << (long) tmp->data.mptCalc.curveFitType
              << ", " << (long) tmp->data.mptCalc.vdMaxEpt
              << ", " << (long) tmp->data.mptCalc.vdMaxEt
              << ", " << (long) tmp->data.mptCalc.mlineBiasOption
              << ", " << (long) tmp->data.mptCalc.numSwp1MeasPwrs
              << ", " << (long) tmp->data.mptCalc.numSwp2EptMeasPwrs
              << ", " << (long) tmp->data.mptCalc.numSwp2EptCalcPwrs
              << ", " << (long) tmp->data.mptCalc.numSwp2EtMeasPwrs
              << ", " << (long) tmp->data.mptCalc.numSwp2EtCalcPwrs;

      for (m = 0; m<MAX4; m++)
      {
         Logfile << ", " << (long) tmp->data.mptCalc.swp1pa[m];
      }
      for (m = 0; m<MAX4; m++)
      {
         Logfile << ", " << (long) tmp->data.mptCalc.swp1rgi[m];
      }
      for (m = 0; m<MAX4; m++)
      {
         Logfile << ", " << (long) tmp->data.mptCalc.swp1bias[m];
      }
      for (m = 0; m<MAX4; m++)
      {
         Logfile << ", " << (long) tmp->data.mptCalc.measPwrSwp1DB10[m];
      }
      for (m = 0; m<MAX4; m++)
      {
         Logfile << ", " << (long) tmp->data.mptCalc.eptMeasPwrSwp2DB100[m];
      }
      for (m = 0; m<MAX4; m++)
      {
         Logfile << ", " << (long) tmp->data.mptCalc.eptCalcPwrSwp2DB100[m];
      }
      for (m = 0; m<MAX4; m++)
      {
         Logfile << ", " << (long) tmp->data.mptCalc.etMeasPwrSwp2DB100[m];
      }
      for (m = 0; m<MAX4; m++)
      {
         Logfile << ", " << (long) tmp->data.mptCalc.etCalcPwrSwp2DB100[m];
      }

      Logfile
            << ", " << (long) tmp->data.delayValProc.doDelayValProcOp
            << ", " << (long) tmp->data.delayValProc.upperBoundChan

            << ", " << (long) tmp->dataRes.delayRes.delayChan
            << ", " << (long) tmp->dataRes.delayRes.delayBw
            << ", " << (long) tmp->dataRes.delayRes.delayVal

            << ", " << (long) tmp->data.dpdCalc.do_dpd_calc_op
            << ", " << (long) tmp->data.dpdCalc.xptMode
            << ", " << (long) tmp->data.dpdCalc.numSwp2MeasPwrs
            << ", " << (long) tmp->data.dpdCalc.numSwp2CalcPwrs;
      for (m = 0; m<MAX4; m++)
      {
         Logfile  << ", " << (long) tmp->data.dpdCalc.measPwrSwp2DB100[m];
      }
      for (m = 0; m<MAX4; m++)
      {
         Logfile  << ", " << (long) tmp->data.dpdCalc.calcPwrSwp2DB100[m];
      }
      Logfile
            << ", " << (long) tmp->data.loadCalcDpd.do_load_calc_dpd_op
            << ", " << (long) tmp->data.loadCalcDpd.loadAction
            << ", " << (long) tmp->data.loadCalcDpd.xptMode
            << ", " << (long) tmp->data.loadCalcDpd.qpoetMinV
            << ", " << (long) tmp->data.loadCalcDpd.qpoetMaxV
            << ", " << (long) tmp->data.loadCalcDpd.etMinV
            << ", " << (long) tmp->data.loadCalcDpd.numSwp2MeasPwrs
            << ", " << (long) tmp->data.loadCalcDpd.numSwp2CalcPwrs;

      for (m = 0; m<MAX4; m++)
      {
         Logfile  << ", " << (long) tmp->data.loadCalcDpd.measPwrSwp2DB100[m];
      }
      for (m = 0; m<MAX4; m++)
      {
         Logfile  << ", " << (long) tmp->data.loadCalcDpd.calcPwrSwp2DB100[m];
      }

      for (k = 0; k<1; k++)
      {
         Logfile
               << ", " << (long) tmp->dataRes.measDpdRes[k].dpdValid
               << ", " << (long) tmp->dataRes.measDpdRes[k].xptMode
               << ", " << (long) tmp->dataRes.measDpdRes[k].rgi
               << ", " << (long) tmp->dataRes.measDpdRes[k].smpsBias
               << ", " << (long) tmp->dataRes.measDpdRes[k].paState
               << ", " << (long) tmp->dataRes.measDpdRes[k].measured
               << ", " << (long) tmp->dataRes.measDpdRes[k].power;
         for (m = 0; m<MAX; m++)
         {
            Logfile  << ", " << (long) tmp->dataRes.measDpdRes[k].amamRes[m];
         }
         for ( m=0; m<MAX; m++)
         {
            Logfile  << ", " << (long) tmp->dataRes.measDpdRes[k].ampmRes[m];
         }
      }
      for ( k=0; k<1; k++)
      {
         Logfile
               << ", " << (long) tmp->dataRes.calcDpdRes[k].dpdValid
               << ", " << (long) tmp->dataRes.calcDpdRes[k].xptMode
               << ", " << (long) tmp->dataRes.calcDpdRes[k].rgi
               << ", " << (long) tmp->dataRes.calcDpdRes[k].smpsBias
               << ", " << (long) tmp->dataRes.calcDpdRes[k].paState
               << ", " << (long) tmp->dataRes.calcDpdRes[k].measured
               << ", " << (long) tmp->dataRes.calcDpdRes[k].power;
         for ( m=0; m<MAX; m++)
         {
            Logfile  << ", " << (long) tmp->dataRes.calcDpdRes[k].amamRes[m];
         }
         for ( m=0; m<MAX; m++)
         {
            Logfile  << ", " << (long) tmp->dataRes.calcDpdRes[k].ampmRes[m];
         }
      }

      for ( k=0; k<1; k++)
      {
         Logfile
               << ", " << (long) tmp->dataRes.measLoadEPDTRes[k].detValid
               << ", " << (long) tmp->dataRes.measLoadEPDTRes[k].measured;
         for (m = 0; m<1; m++)
         {
            Logfile
                  << ", " << (long) tmp->dataRes.measLoadEPDTRes[k].epdtTbl[m];
         }
      }

      for (k=0; k<1; k++)
      {
         Logfile
               << ", " << (long) tmp->dataRes.calcLoadEPDTRes[k].detValid
               << ", " << (long) tmp->dataRes.calcLoadEPDTRes[k].measured;
         for (m = 0; m<1; m++)
         {
            Logfile
                  << ", " << (long) tmp->dataRes.calcLoadEPDTRes[k].epdtTbl[m];
         }
      }

      Logfile
            << ", " << (long) tmp->xpt.xptAction
            << ", " << (long) tmp->xpt.xpt_cfg.dev
            << ", " << (long) tmp->xpt.xpt_cfg.fbDev
            << ", " << (long) tmp->xpt.xpt_cfg.xptMode
            << ", " << (long) tmp->xpt.xptCaptureIQ.dev
            << ", " << (long) tmp->xpt.xptCaptureIQ.fbDev
            << ", " << (long) tmp->xpt.xptCaptureIQ.capType
            << ", " << (long) tmp->xpt.xptCaptureIQ.numSamples
            << ", " << (long) tmp->xpt.xptCaptureIQ.firstTrig
            << ", " << (long) tmp->xpt.xptCaptureIQ.lastTrig
            << ", " << (long) tmp->xpt.xptCaptureIQ.procFlag

            << ", " << (long) tmp->rxMeas.dev;
      for (k=0; k<4; k++)
      {
         Logfile
               << ", " << (long) tmp->rxMeas.actions[k].rx_meas_action
               << ", " << (long) tmp->rxMeas.actions[k].payloads[0].lna_state
               << ", " << (long) tmp->rxMeas.actions[k].payloads[0].power_mode
               << ", " << (long) tmp->rxMeas.actions[k].payloads[0].exp_rxagc
               << ", " << (long) tmp->rxMeas.actions[k].payloads[1].lna_state
               << ", " << (long) tmp->rxMeas.actions[k].payloads[1].power_mode
               << ", " << (long) tmp->rxMeas.actions[k].payloads[1].exp_rxagc;
      }
      Logfile
            << ", " << (long) tmp->sync.syncEvent
            << ", " << (long) tmp->sync.device;

      Logfile << "\n";
      tmp++;
   }

   Logfile.close();
   return true;
}

void Cfg_ESC_Tx_Mask_ForFirstDeivce(cal_sweep_record &tmp,ESC_TX_MASK_TYPE type)
{
   int txOverrideMask   = ESC_TX_OVERRIDE_MASK_POWER;
   int pwrOverrideType  = ESC_TX_PWR_OVERRIDE_PARAMS;
   int txParamMask      = 0;

   if(TX_NORMAL_MASK == type)
   {
      txParamMask = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_IQ_GAIN |
                     ESC_TX_OVERRIDE_ENV_SCALE | ESC_TX_OVERRIDE_PA_CURRENT );
   }

   else if(TX_DPD_MASK == type)
   {
      //txParamMask     = 0;
      pwrOverrideType = ESC_TX_PWR_OVERRIDE_ABS_PWR;
   }
   else if(TX_LOW_RGI == type)
   {
      txParamMask = (ESC_TX_OVERRIDE_PA_STATE | ESC_TX_OVERRIDE_RGI | ESC_TX_OVERRIDE_BIAS | ESC_TX_OVERRIDE_PA_CURRENT);
   }
   tmp.txParams.txParams[0].txOverrideMask   = txOverrideMask;
   tmp.txParams.txParams[0].pwrOverrideType  = pwrOverrideType;
   tmp.txParams.txParams[0].pwrParamMask     = txParamMask;
}

// ******************************************************************//
//    End of RF ESC ET cal related demo code
// ******************************************************************//

unsigned char Test_XPT_Capture_Sweep()
{
   ostringstream stOutput;
   printf("\nStarting XPT Capture QLIB Demo");

   vector < cal_sweep_record > records;
   records.resize ( 1000 );

   for (int j = 0; j < 1000; j++)
   {
      Test_RFCal_ESC_InitRecord( &records[j], 0, 0, 0, 1, 1, 0, 9750, 20000, 19, 12, 3, 3 );
   }

   HANDLE phoneHandle = g_hResourceContext;
   unsigned short recordIndex = 0;
   for( unsigned int no_of_loops =0; no_of_loops<15; no_of_loops++)
   {
      // Form Sweep

      // Segment 0      :  Radio Setup
      // Segments 1-6   :  No OpCode
      if(no_of_loops ==0)
      {
         for( unsigned short segIdx = 0; segIdx < 7; segIdx++)
         {
            if(segIdx == 0)
            {
               records[segIdx].isCfgSeg = true;       // Radio Setup for Segment 0
            }
            else
            {
               records[segIdx].isNopSeg = true;       // NoOp. For segment 1 - 6
            }

            records[segIdx].subSwpType = -1;

            //radio setup : MasterDevState and slaveDevState
            records[segIdx].radio_cfg.device.masterDevState = 3;           // Enable tx,rx
            records[segIdx].radio_cfg.device.slaveDevState = 0;            // NO_CHANGE

#ifndef __NO_RESOURCE_CONTEXT
            QLIB_FTM_GENERATE_ESC_OPCODE(g_hResourceContext, (void *)&records[segIdx]);
#endif
            recordIndex++;
         }
      }

      // Segment 7  :  XPT CFG
      {
         records[recordIndex].resExpected = true;
         records[recordIndex].txPurpose = (word)0;

         /* XPT Action indicates the type of XPT operation to be done.
         Bit 0 = CAL_CONFIG | Bit 1 = XPT IQ_CAPTURE  */
         records[recordIndex].xpt.xptAction = ESC_XPT_ACTION_CAL_CFG;   // Bit 0 CAL_CONFIG to init XPT action
         /* XPT Action bit 0 enabled (CAL_CONFIG) requires the following fields */
         /* Primary Device */
         records[recordIndex].xpt.xpt_cfg.dev = 0;
         /* Feedback Device */
         records[recordIndex].xpt.xpt_cfg.fbDev = 1;
         /* XPT Mode 0 - APT | 1 - EPT | 2 - ET */
         records[recordIndex].xpt.xpt_cfg.xptMode = XPT_TYPE_EPT;

#ifndef __NO_RESOURCE_CONTEXT
         QLIB_FTM_GENERATE_ESC_OPCODE(g_hResourceContext, (void *)&records[recordIndex]);
#endif
         recordIndex++;
      }

      //   // Segment 8  :  DATA - LOAD UNITY DPD
      //   {
      //      records[recordIndex].resExpected = true;
      //      records[recordIndex].txPurpose = (word)0;
      //      records[recordIndex].data.tech = 1;
      //      records[recordIndex].data.refChan = 9750;
      //      records[recordIndex].data.dev = 0;
      //      records[recordIndex].data.doDataOp = true; //This assignement is required for QLIB_FTM_GENERATE_ESC_OPCODE.
      //      records[recordIndex].data.nomDPDTbl.doDPDLoadOp  = true;  // This assignement is for QLIB_FTM_CFG_AND_EXEC_ESC_SWEEP.
      //      records[recordIndex].data.dataAction = DATA_OPCODE_ACTION_LOAD_DPD; // Enables Bit 1 - Load Nominal DPD Table
      //
      //      // Bit 1 - Load Nominal DPD Table requires the following fields
      //      // XPT Type
      //      records[recordIndex].data.nomDPDTbl.xptType = LOAD_DPD_XPT_TYPE_ET;   // 0 EPT | 1 ET. This is ET use
      //
      //      // Number of AMAM, AMPM and Detrough
      //      int numAMAM = 64;
      //      int numAMPM = 64;
      //
      //      records[recordIndex].data.nomDPDTbl.numAMAM = numAMAM ;
      //      records[recordIndex].data.nomDPDTbl.numAMPM = numAMPM ;
      //
      //      // DPD Table
      //      for(int index = 0; index <  records[recordIndex].data.nomDPDTbl.numAMAM; index++)
      //      {
      //         unsigned int amamVal = ( index << 10 ) + 0x400;
      //         if( amamVal > 0xFFFF )
      //            amamVal = 0xFFFF;
      //         records[recordIndex].data.nomDPDTbl.AMAM[index] = amamVal;   /// 1024, 2048, 3072, 4096 .....
      //         records[recordIndex].data.nomDPDTbl.AMPM[index] = 0;
      //      }
      //      unsigned short numDET = 128;
      //      // DETROUGH TABLE
      //      const unsigned short AVAGO_A4A_EPDT_TABLE[] =
      //      {7,29,64,113,173,246,329,424,528,642,765,897,1037,1185,1341,1503,1673,1849,2031,2219,
      //      2413,2612,2816,3025,3238,3456,3678,3904,4134,4367,4604,4844,5087,5333,5581,5833,6087,6343,
      //      6602,6863,7126,7391,7657,7926,8196,8468,8742,9016,9293,9570,9849,10129,10411,10693,10977,
      //      11261,11546,11832,12120,12407,12696,12986,13276,13566,13858,14150,14442,14736,15029,15323,
      //      15618,15913,16208,16504,16801,17097,17394,17692,17989,18287,18585,18884,19183,19482,19781,
      //      20080,20380,20680,20980,21280,21581,21882,22182,22483,22784,23085,23387,23688,23990,24292,
      //      24593,24895,25197,25499,25801,26104,26406,26708,27011,27313,27616,27919,28222,28524,28827,
      //      29130,29433,29736,30039,30342,30645,30948,31252,31555,31858,32161,32465,32767};
      //
      //      records[recordIndex].data.nomDPDTbl.numDET = (byte)numDET;
      //      for(unsigned short idx = 0; idx < numDET; idx++)
      //      {
      //         records[recordIndex].data.nomDPDTbl.DET[idx]  = (word)0;
      //      }
      //
      //      for(unsigned short detidx = 0; detidx < numDET; detidx++)
      //      {
      //         records[recordIndex].data.nomDPDTbl.DET[detidx]  = (word)AVAGO_A4A_EPDT_TABLE[detidx];
      //      }
      //
      //      //for(unsigned short detIndex = 0; detIndex < numDET; detIndex++)
      //      //{
      //      //   unsigned int detVal = (unsigned int)(((double)(detIndex+1)/128.0)*pow(2.0,15.0));
      //      //   if( detVal > 0xFFFF )
      //      //      detVal = 0xFFFF;
      //      //   records[recordIndex].data.nomDPDTbl.DET[detIndex] = detVal;
      //      //}
      //
      //#ifndef __NO_RESOURCE_CONTEXT
      //      QLIB_FTM_GENERATE_ESC_OPCODE(g_hResourceContext, (void *)&records[recordIndex]);
      //#endif
      //      recordIndex++;
      //   }

      // Segment 9  :  DATA - LOAD UNITY DPD
      {
         records[recordIndex].resExpected = true;
         records[recordIndex].txPurpose = (word)0;
         records[recordIndex].data.tech = 1;
         records[recordIndex].data.refChan = 9750;
         records[recordIndex].data.dev = 0;
         records[recordIndex].data.doDataOp = true;   //This assignement is required for QLIB_FTM_GENERATE_ESC_OPCODE.
         records[recordIndex].data.nomDPDTbl.doDPDLoadOp = true;  // This assignement is for QLIB_FTM_CFG_AND_EXEC_ESC_SWEEP.
         records[recordIndex].data.dataAction = DATA_OPCODE_ACTION_LOAD_DPD; // Enables Bit 1 - Load Nominal DPD Table

         // Bit 1 - Load Nominal DPD Table requires the following fields
         // XPT Type
         records[recordIndex].data.nomDPDTbl.xptType = LOAD_DPD_XPT_TYPE_EPT; // 0 EPT | 1 ET. This is ET use

         // Number of AMAM, AMPM and Detrough
         int numAMAM = 64;
         int numAMPM = 64;

         records[recordIndex].data.nomDPDTbl.numAMAM = numAMAM ;
         records[recordIndex].data.nomDPDTbl.numAMPM = numAMPM ;
         // DPD Table
         for(int index = 0; index < records[recordIndex].data.nomDPDTbl.numAMAM; index++)
         {
            unsigned int amamVal = ( index << 10 ) + 0x400;
            if( amamVal > 0xFFFF )
               amamVal = 0xFFFF;
            records[recordIndex].data.nomDPDTbl.AMAM[index] = amamVal;  /// 1024, 2048, 3072, 4096 .....
            records[recordIndex].data.nomDPDTbl.AMPM[index] = 0;
         }

         records[recordIndex].data.nomDPDTbl.numDET = 0;

#ifndef __NO_RESOURCE_CONTEXT
         QLIB_FTM_GENERATE_ESC_OPCODE(g_hResourceContext, (void *)&records[recordIndex]);
#endif
         recordIndex++;
      }

      // Segment 10-29  :  10 * TX OVERRIDE + IQ CAPTURES
      {
         unsigned short XPT_Swp1_RGI_List[15] = {70,69,68,67,66,65,64,63,70,69,68,67,66,65,64};
         unsigned short XPT_Swp1_Bias_List[15] = {1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200};

         for(int capIndex = 0; capIndex < 15; capIndex++)
         {
            /* ESC Parameters */
            records[recordIndex].txPurpose = (word)0;
            records[recordIndex].subSwpType =   ESC_SUB_SWEEP_XPT_ALIGNMENT;
            records[recordIndex].measSeg = true;

            // OpCode 0x200 (bit 9 enabled): TxOverride
            // Tx Device
            records[recordIndex].txParams.txDevice = 1;
            // XPT mode for RFM_DEVICE_0
            records[recordIndex].txParams.txParams[0].xptMode = (byte)XPT_TYPE_EPT;
            // Tx Override Type Mask Setting - Bit 0 - Output Power
            Cfg_ESC_Tx_Mask_ForFirstDeivce(records[recordIndex],TX_OVERRIDE_NORMAL_MASK);
            records[recordIndex].txParams.txParams[0].pwrParamMask |= ESC_TX_OVERRIDE_DELAY;
            // Tx Override Type Mask Bit 0 enabled (Output Power) requires the following fields
            // Bit 0 Payload (PA state)
            records[recordIndex].txParams.txParams[0].paState = 3;
            // Bit 1 Payload (RGI)
            records[recordIndex].txParams.txParams[0].rgi = (byte)XPT_Swp1_RGI_List[capIndex];
            // Bit 2 Payload (Bias)
            records[recordIndex].txParams.txParams[0].bias = (dword)XPT_Swp1_Bias_List[capIndex];
            // Bit 3 Payload (IQ Gain)
            records[recordIndex].txParams.txParams[0].iqGain = (word)622;
            records[recordIndex].txParams.txParams[0].iqGainAction = (byte)1;
            // Bit 4 Payload (Envelope Scale)
            records[recordIndex].txParams.txParams[0].envScale = (word)1350;
            records[recordIndex].txParams.txParams[0].envScaleAction = (byte)1;
            // Bit 5 Payload (Q Current)
            records[recordIndex].txParams.txParams[0].quiescentCurrent = (word)127;
            // Bit 6 Payload (Delay)
            records[recordIndex].txParams.txParams[0].delayAction = ESC_TX_OVERRIDE_ACTION_FROM_STATIC_NV;
            records[recordIndex].txParams.txParams[0].delay = 0;

#ifndef __NO_RESOURCE_CONTEXT
            QLIB_FTM_GENERATE_ESC_OPCODE(g_hResourceContext, (void *)&records[recordIndex]);
#endif
            recordIndex++;

            // OpCode: 0x2000 XPT2
            records[recordIndex].txPurpose = (word)0;
            records[recordIndex].subSwpType = -1;
            records[recordIndex].resExpected = true;

            /* XPT Action indicates the type of XPT operation to be done.
            Bit 0 = CAL_CONFIG | Bit 1 = XPT IQ_CAPTURE  */
            records[recordIndex].xpt.xptAction = ESC_XPT_ACTION_IQ_CAP; // 2 (i.e., Bit 1 enabled)

            // XPT Action bit 1 enabled (IQ_CAPTURE) requires the following fields
            // Primary device
            records[recordIndex].xpt.xptCaptureIQ.dev = 0;
            // Feedback device
            records[recordIndex].xpt.xptCaptureIQ.fbDev = 1;
            // Capture Type
            records[recordIndex].xpt.xptCaptureIQ.capType = ESC_XPT_CAP_EPT; // 0 - EPT IQ Capture
            // Num of Samples
            records[recordIndex].xpt.xptCaptureIQ.numSamples = (word)9216;
            // Flag indicating the first IQ Capture
            records[recordIndex].xpt.xptCaptureIQ.firstTrig = (capIndex==0)?1:0;
            // Flag indicating the last IQ Capture
            records[recordIndex].xpt.xptCaptureIQ.lastTrig = (capIndex==14)?1:0;
            // Flag indicating the type of processing to be done
            records[recordIndex].xpt.xptCaptureIQ.procFlag = ESC_XPT_CAP_PROC_SWP2;

            records[recordIndex].data.tech = (byte)1;
            records[recordIndex].data.refChan = (dword)9750;

#ifndef __NO_RESOURCE_CONTEXT
            QLIB_FTM_GENERATE_ESC_OPCODE(g_hResourceContext, (void *)&records[recordIndex]);
#endif
            recordIndex++;
         }
      }
      // Segment 30  :  DATA REQUEST XPT CAPTURE RESULTS
      {
         records[recordIndex].resExpected = true;
         records[recordIndex].txPurpose = (word)0;
         records[recordIndex].data.tech = 1;
         records[recordIndex].data.refChan = 9750;
         records[recordIndex].data.dev = 0;
         records[recordIndex].data.doDataOp = true;   //This assignement is required for QLIB_FTM_GENERATE_ESC_OPCODE.
         records[recordIndex].data.xptCapReq.do_xpt_cap_res_op = true;  // This assignement is for QLIB_FTM_CFG_AND_EXEC_ESC_SWEEP.
         records[recordIndex].data.dataAction = DATA_OPCODE_ACTION_XPT_CAP_RES;
         // XPT Capture Results Payload
         records[recordIndex].data.xptCapReq.xptResMask = (dword)(XPT_CAP_RES_MASK_COMP | XPT_CAP_RES_MASK_DPD);
         records[recordIndex].data.xptCapReq.xptCapNumRes = 15;

#ifndef __NO_RESOURCE_CONTEXT
         QLIB_FTM_GENERATE_ESC_OPCODE(g_hResourceContext, (void *)&records[recordIndex]);
#endif
         recordIndex++;
      }

   }
   recordIndex--;

   //#ifndef __NO_RESOURCE_CONTEXT
   //#ifdef ON_TARGET
   // Do the sweep now
   g_hResourceContext = phoneHandle;
   unsigned char _bOk = TRUE;

   _bOk = QLIB_FTM_CFG_AND_EXEC_ESC_SWEEP_COMPRESSED(g_hResourceContext, (void*)(&records[0]),records.size(),false ,true);

   if (!_bOk) return false;

   //::Sleep(3000);
   Sleep(30000);

   unsigned __int64 err = 0;
   unsigned short err_segment = 0;
   _bOk = QLIB_FTM_GET_ESC_SWEEP_RESULTS(g_hResourceContext, (void*)(&records[0]),records.size(),&err,&err_segment);

   if (!_bOk) return false;

   printf("\nXPT Capture Results: ");
   printf("\n");
   printf("Number of Captures = %d\n", records[recordIndex].dataRes.xptCapRes.xptCapNumRes);
   printf("Capture Mask = %d\n", records[recordIndex].dataRes.xptCapRes.xptCapResMask);
   printf("DPD Flag\tXPT Mode\tRGI\tBias\tPA\tPAIcq\tIQ Gain\tEnv Sc\n");
   unsigned int idx = 0;
   for( idx = 0; idx < records[recordIndex].dataRes.xptCapRes.xptCapNumRes; idx++ )
   {
      printf("   %1d    \t   %1d    \t %2d\t%4d\t %1d\t %3d \t  %3d  \t %4d \n",
             records[recordIndex].dataRes.xptCapRes.xptRes[idx].xptCapCommonRes.dpdValid,
             records[recordIndex].dataRes.xptCapRes.xptRes[idx].xptCapCommonRes.xptMode,
             records[recordIndex].dataRes.xptCapRes.xptRes[idx].xptCapCommonRes.rgi,
             records[recordIndex].dataRes.xptCapRes.xptRes[idx].xptCapCommonRes.smpsBias,
             records[recordIndex].dataRes.xptCapRes.xptRes[idx].xptCapCommonRes.paState,
             records[recordIndex].dataRes.xptCapRes.xptRes[idx].xptCapCommonRes.icq,
             records[recordIndex].dataRes.xptCapRes.xptRes[idx].xptCapCommonRes.iqGain,
             records[recordIndex].dataRes.xptCapRes.xptRes[idx].xptCapCommonRes.envSc);
   }
   printf("\n");
   for( idx = 0; idx < records[recordIndex].dataRes.xptCapRes.xptCapNumRes; idx++ )
   {
      printf("Compression[%d] = %d\n", idx, records[recordIndex].dataRes.xptCapRes.xptRes[idx].xptCapCompRes.compression);
   }
   printf("\n");
   printf("DPD AMAM[0] = %d\n", (int)records[recordIndex].dataRes.xptCapRes.xptRes[0].xptCalDpdRes.amamRes[0]);
   printf("DPD AMAM[15] = %d\n", (int)records[recordIndex].dataRes.xptCapRes.xptRes[0].xptCalDpdRes.amamRes[15]);
   printf("DPD AMPM[0] = %d\n", (int)((signed short)records[recordIndex].dataRes.xptCapRes.xptRes[0].xptCalDpdRes.ampmRes[0]));
   printf("DPD AMPM[15] = %d\n", (int)((signed short)records[recordIndex].dataRes.xptCapRes.xptRes[0].xptCalDpdRes.ampmRes[15]));
   //if (err != 0)
   //{
   // cout << "Error at segment " << (int)err_segment << endl;
   // return -1;
   //}
   return true;

   //#endif
   //#endif
}


unsigned char RFCal_Helper_ESC_ET_FBGain_Ref_Assignment_XML(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, map < pair < dword, byte >, Tx_RefCal_Data_V2_Record > & ChanFBGainStateToPwrFBGainParams)
{
   //**************************************************//
   //         Parameters 
   //**************************************************//
   MSXML2::IXMLDOMDocumentPtr spXMLDoc = NULL;
   
   //**************************************************//
   //         Load XML and get a node with the rf_mode
   //**************************************************//
   CoInitialize(NULL); // Initialize COM
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
   spXMLDoc->load(demo_param.stRefCalOffset_filename.c_str());
   if (spXMLDoc == NULL)
   {
      CoUninitialize();
      return false;
   }

   int RF_Mode = static_cast<int>(cal_param.RF_Mode);
   int devID = static_cast<int>(sweep_param.Device_Selection.Master_Device_ID);
   int bw_mode = static_cast<int>(sweep_param.Waveform.Tx_BW);
   
   MSXML2::IXMLDOMNodeListPtr TxRefCalDataTableLists = NULL;
   stringstream ssXPATH;
   ssXPATH  << "/RefCal_Data_V2/Tx_RefCal_Data_V2[@rf_mode='" << RF_Mode << "' and @device='" << devID << "']"
            << "/Tx_RefCal_Data_Bandwidth[@bw_mode='" << bw_mode  << "']"
            << "/Tx_RefCal_Data_Table";
   TxRefCalDataTableLists = RFCal_Helper_GetXMLNodes(spXMLDoc,ssXPATH.str());
   if (!TxRefCalDataTableLists) { return false; }
   long iNumTable = 0;
   TxRefCalDataTableLists->get_length(&iNumTable);
   for (long iTable = 0; iTable <iNumTable; iTable++)
   {
      MSXML2::IXMLDOMNodePtr TxTablePtr = TxRefCalDataTableLists->nextNode();

      if (!TxTablePtr) { continue; }      
      dword iChannel = 0;
      byte FBGainState = 0;
      RFCal_Helper_Read_Attribute(TxTablePtr,"channel",iChannel);
      RFCal_Helper_Read_Attribute(TxTablePtr,"fbgainstate",FBGainState);
            
      MSXML2::IXMLDOMNodePtr TxRecordPtr = TxTablePtr->firstChild;
      if (TxRecordPtr)
      {      
         Tx_RefCal_Data_V2_Record TxParam;
         RFCal_Helper_Read_Attribute(TxRecordPtr,"rxScale",TxParam.rxScale);
         RFCal_Helper_Read_Attribute(TxRecordPtr,"uBias",TxParam.uBias);
         RFCal_Helper_Read_Attribute(TxRecordPtr,"gain",TxParam.FBGain);
         RFCal_Helper_Read_Attribute(TxRecordPtr,"pwr",TxParam.pwr);

         ChanFBGainStateToPwrFBGainParams[ make_pair(iChannel,FBGainState)] = TxParam;
      }
   }

   spXMLDoc.Release();
   
   CoUninitialize();
   return true;
}

unsigned char RFCal_Helper_ESC_ET_Tx_AptChar_DataV6_Assignment_XML(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, map <RFCal_Helper_Tx_Params, double> & TxParamsToPwr)
{
   map< chanPAState_type, rgipwr_type> refcalchanPa_rgi_pwr;
   RFCal_Helper_ESC_ET_Tx_AptChar_DataV6_Assignment_XML(demo_param,  cal_param, sweep_param, TxParamsToPwr, refcalchanPa_rgi_pwr);
   return true;
}
unsigned char RFCal_Helper_UpdateRFCalParamsFromCharFile(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, Sweep_Params & sweep_param)
{
   //**************************************************//
   //         Parameters 
   //**************************************************//
   MSXML2::IXMLDOMDocumentPtr spXMLDoc = NULL;
   
   //**************************************************//
   //         Load XML and get a node with the rf_mode
   //**************************************************//
   CoInitialize(NULL); // Initialize COM
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
   spXMLDoc->load(demo_param.stRFCalCharData_filename.c_str());
   if (spXMLDoc == NULL)
   {
      CoUninitialize();
      return false;
      //throw RFCal_Helper_Exception("XML file not found.",__FILE__,__LINE__);
   }

   byte RF_Mode = cal_param.RF_Mode;
   dword iChannel = sweep_param.Ref_Channel;
   MSXML2::IXMLDOMNodeListPtr RxAPTChanTableLists = NULL;
   stringstream ssXPATH;
   ssXPATH << "/RFCal_Char_Data/RgiBiasChar[@rf_mode='" << static_cast<UINT>(RF_Mode) << "' and @device='0']/RgiBiasCharRec[@channel='"<< iChannel <<"']";
   RxAPTChanTableLists = RFCal_Helper_GetXMLNodes(spXMLDoc,ssXPATH.str());
   if (!RxAPTChanTableLists) { return false; }
   long iNumTable = 0;
   RxAPTChanTableLists->get_length(&iNumTable);
   for (long iTable = 0; iTable <iNumTable; iTable++)
   {
      MSXML2::IXMLDOMNodePtr RxTablePtr = RxAPTChanTableLists->nextNode();

      if (!RxTablePtr) { continue; }
      byte PA_State = 0;
      RFCal_Helper_Read_Attribute(RxTablePtr,"pa_state",PA_State);
      word IQGain;
      word EnvScale = 0;
      vector <word> Bias_List;
      vector <byte> RGI_List;
      vector <byte> RGI_Delta_RGI_List;
      vector <double> PowerList;
      RFCal_Helper_Read_Param(RxTablePtr,"XPT_Swp1_IQ_Gain",IQGain);
      RFCal_Helper_Read_Param(RxTablePtr,"XPT_Swp1_Env_Scale",EnvScale);
      RFCal_Helper_Read_Param(RxTablePtr,"XPT_Swp1_Bias_List",Bias_List);
      RFCal_Helper_Read_Param(RxTablePtr,"XPT_Swp1_RGI_List",RGI_List);
      RFCal_Helper_Read_Param(RxTablePtr,"XPT_Swp1_RGI_Delta_RGI_List",RGI_Delta_RGI_List);
      RFCal_Helper_Read_Param(RxTablePtr,"XPT_Swp1_Power_List",PowerList);

      if (cal_param.En_EPTET_Different_PAstate)
      {
         if (PA_State == sweep_param.XPT_Swp1_PA_State_EPT)
         {
            sweep_param.XPT_Swp1_IQ_Gain = IQGain;
            sweep_param.XPT_Swp1_Env_Scale = EnvScale;
            sweep_param.XPT_Swp1_Bias_List_EPT = Bias_List;
            sweep_param.XPT_Swp1_RGI_List_EPT = RGI_List;
            sweep_param.XPT_Swp1_RGI_Delta_RGI_List_EPT = RGI_Delta_RGI_List;
            sweep_param.XPT_Swp1_Power_List_EPT = PowerList;
         }
         else
         {
            sweep_param.XPT_Swp1_IQ_Gain = IQGain;
            sweep_param.XPT_Swp1_Env_Scale = EnvScale;
            sweep_param.XPT_Swp1_Bias_List_ET = Bias_List;
            sweep_param.XPT_Swp1_RGI_List_ET = RGI_List;
            sweep_param.XPT_Swp1_RGI_Delta_RGI_List_ET = RGI_Delta_RGI_List;
            sweep_param.XPT_Swp1_Power_List_ET = PowerList;
         }
      }
      else
      {
            sweep_param.XPT_Swp1_IQ_Gain = IQGain;
            sweep_param.XPT_Swp1_Env_Scale = EnvScale;
            sweep_param.XPT_Swp1_Bias_List = Bias_List;
            sweep_param.XPT_Swp1_RGI_List = RGI_List;
            sweep_param.XPT_Swp1_RGI_Delta_RGI_List = RGI_Delta_RGI_List;
            sweep_param.XPT_Swp1_Power_List = PowerList;
      }

   }

   spXMLDoc.Release();
   
   CoUninitialize();
   return true;

   return true;
}
unsigned char RFCal_Helper_ESC_ET_Tx_AptChar_DataV6_Assignment_XML(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param, const Sweep_Params & sweep_param, map <RFCal_Helper_Tx_Params, double> & TxParamsToPwr, map< chanPAState_type, rgipwr_type> & refcalchanPa_rgi_pwr)
{
    //**************************************************//
    //          Parameters 
    //**************************************************//
    MSXML2::IXMLDOMDocumentPtr spXMLDoc = NULL;
    
    //**************************************************//
    //          Load XML and get a node with the rf_mode
    //**************************************************//
    CoInitialize(NULL); // Initialize COM
    spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
    spXMLDoc->load(demo_param.stRFCalCharData_filename.c_str());
    if (spXMLDoc == NULL)
    {
        CoUninitialize();
        return false;
        //throw RFCal_Helper_Exception("XML file not found.",__FILE__,__LINE__);
    }

    byte RF_Mode = cal_param.RF_Mode;
    dword iChannel = sweep_param.Ref_Channel;
   int devId = static_cast<int>(sweep_param.Device_Selection.Master_Device_ID);
    MSXML2::IXMLDOMNodeListPtr RxAPTChanTableLists = NULL;
    stringstream ssXPATH;
   ssXPATH << "/RFCal_Data/APTCharV6/APTRfmodeTable[@rf_mode='" << static_cast<UINT>(RF_Mode) << "' and @device='" << devId << "']/APTTempTable/APTChanTable[@channel='"<< iChannel <<"']";
    RxAPTChanTableLists = RFCal_Helper_GetXMLNodes(spXMLDoc,ssXPATH.str());
    if (!RxAPTChanTableLists) { return false; }
    long iNumTable = 0;
    RxAPTChanTableLists->get_length(&iNumTable);
    for (long iTable = 0; iTable <iNumTable; iTable++)
    {
        MSXML2::IXMLDOMNodePtr RxTablePtr = RxAPTChanTableLists->nextNode();

        if (!RxTablePtr) { continue; }
      // Check this APTChanTable includes xptMode attribute
      unsigned char isExtendedAptCharTable = 0;
      byte xptMode = 0;
      word iqGain = 0;
      word envScale = 0;
      byte delayVal = 0;
      if (RFCal_Helper_Read_Attribute(RxTablePtr,"xptMode",xptMode)) // if true, the attribute exists
      {
         isExtendedAptCharTable = 1;
         RFCal_Helper_Read_Attribute(RxTablePtr,"iqGain",iqGain);
         RFCal_Helper_Read_Attribute(RxTablePtr,"envScale",envScale);
         RFCal_Helper_Read_Attribute(RxTablePtr,"delayVal",delayVal);
      }

      //MSXML2::IXMLDOMNode xptMode

        byte PA_State = 0;
        RFCal_Helper_Read_Attribute(RxTablePtr,"pa_state",PA_State);
                
        MSXML2::IXMLDOMNodeListPtr APTRecListPtr = NULL;
    
        APTRecListPtr = RFCal_Helper_GetXMLNodes(RxTablePtr,"APTRec");
        if (!APTRecListPtr) { continue; }
                
        long iNumChar= 0;
        APTRecListPtr->get_length(&iNumChar);
        for (long iChar = 0; iChar <iNumChar; iChar++)
        {
            MSXML2::IXMLDOMNodePtr APTRetPtr = APTRecListPtr->nextNode();
            if (!APTRetPtr) { continue; }
            RFCal_Helper_Tx_Params txParam;
        
            txParam.Channel = iChannel;
            txParam.PaState = PA_State;
         txParam.XPTmode = xptMode;
         txParam.IqGain = iqGain;
         txParam.EnvScale = envScale;
         txParam.delayVal = delayVal;
         dword vcc = 0;
         dword icq = 0;
         if (isExtendedAptCharTable)
         {
            RFCal_Helper_Read_Attribute(APTRetPtr,"vcc",vcc);
            RFCal_Helper_Read_Attribute(APTRetPtr,"icq",icq);
         }
         txParam.Bias = vcc;
         txParam.PaQuiescentCurrent = icq;

            double pwr = 0.0;
            RFCal_Helper_Read_Attribute(APTRetPtr,"rgi",txParam.RGI);
            RFCal_Helper_Read_Attribute(APTRetPtr,"pwr",pwr);
            TxParamsToPwr[txParam] = pwr;
         //rgipwr_type rgiToPwr;
         //rgiToPwr.insert(make_pair(txParam.RGI,pwr));
         //refcalchanPa_rgi_pwr[make_pair (iChannel,PA_State)] = rgiToPwr;

        }           
    }

    spXMLDoc.Release();
    
    CoUninitialize();
    return true;

}
unsigned char RFCal_Helper_ESC_ET_Rx_Char_Data_Assignment_XML(const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, Cal_Param & cal_param, const Sweep_Params & sweep_param, map <RFCal_Helper_Rx_Params, short> & RxParamsToOffset)
{
    //**************************************************//
    //          Parameters 
    //**************************************************//
    MSXML2::IXMLDOMDocumentPtr spXMLDoc = NULL;
    
    //**************************************************//
    //          Load XML and get a node with the rf_mode
    //**************************************************//
    CoInitialize(NULL); // Initialize COM
    spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
    spXMLDoc->load(demo_param.stRxCharDataXML_filename.c_str());
    if (spXMLDoc == NULL)
    {
        CoUninitialize();
        return false;
        //throw RFCal_Helper_Exception("XML file not found.",__FILE__,__LINE__);
    }

    for (vector<byte>::const_iterator it = sweep_param.RFMode_List.begin(); it != sweep_param.RFMode_List.end(); it++)
    {
        byte RF_Mode = (*it);
        MSXML2::IXMLDOMNodeListPtr RxOffsetCharTableLists = NULL;
        stringstream ssXPATH;
        ssXPATH << "/RFCal_Data/RxOffsetCharTable[@rf_mode='" << static_cast<UINT>(RF_Mode) << "']";
        RxOffsetCharTableLists = RFCal_Helper_GetXMLNodes(spXMLDoc,ssXPATH.str());
        if (!RxOffsetCharTableLists) { continue; }
        long iNumTable = 0;
        RxOffsetCharTableLists->get_length(&iNumTable);
        for (long iTable = 0; iTable <iNumTable; iTable++)
        {
            RFCal_Helper_Rx_Params RxParam;
            RxParam.rf_mode = RF_Mode;
            MSXML2::IXMLDOMNodePtr RxTablePtr = RxOffsetCharTableLists->nextNode();

            if (!RxTablePtr) { continue; }

            RFCal_Helper_Read_Attribute(RxTablePtr,"chain",RxParam.RxPath);
            RFCal_Helper_Read_Attribute(RxTablePtr,"instance",RxParam.instance);
                
            MSXML2::IXMLDOMNodeListPtr RxOffsetCharV1 = NULL;
    
            RxOffsetCharV1 = RFCal_Helper_GetXMLNodes(RxTablePtr,"RxOffsetCharV1");
            if (!RxOffsetCharV1) { continue; }
                
            long iNumChar= 0;
            RxOffsetCharV1->get_length(&iNumChar);
            for (long iChar = 0; iChar <iNumChar; iChar++)
            {
                MSXML2::IXMLDOMNodePtr RxCharPtr = RxOffsetCharV1->nextNode();
                if (!RxCharPtr) { continue; }
                RFCal_Helper_Read_Attribute(RxCharPtr,"channel",RxParam.Channel);
                RFCal_Helper_Read_Attribute(RxCharPtr,"rx_cal_type",RxParam.rx_cal_type);

                MSXML2::IXMLDOMNodeListPtr RxOffsetListPtr = NULL;
                RxOffsetListPtr = RFCal_Helper_GetXMLNodes(RxCharPtr,"RxOffset");
                if (!RxOffsetListPtr) { continue; }

                long iNumOffsets= 0;
                RxOffsetListPtr->get_length(&iNumOffsets);
                for (long iChar = 0; iChar <iNumChar; iChar++)
                {
                    MSXML2::IXMLDOMNodePtr RxOffsetPtr = RxOffsetListPtr->nextNode();
                    if (!RxOffsetPtr) { continue; }
                                
                    short offset = 0;
                    RFCal_Helper_Read_Attribute(RxOffsetPtr,"pm",RxParam.RxPM);
                    RFCal_Helper_Read_Attribute(RxOffsetPtr,"lna",RxParam.LNARange);
                    RFCal_Helper_Read_Attribute(RxOffsetPtr,"offset",offset);
                            
                    RxParamsToOffset[RxParam] = offset;
                                
                }
            }           
        }
    }

    spXMLDoc.Release();
    
    CoUninitialize();
    return true;
}

unsigned char ValidateGetPathLossParameters(MSXML2::IXMLDOMDocumentPtr & spXMLDoc, const string & calDBXML, const string & calConfigName, word calPathNumber, double & FreqBegin ,double & FreqEnd)
{
    if ( _access( calDBXML.c_str(), 0 ) != 0 )
    {
        cout << "the file does not exist. " << endl;
        return false;
    }
    
    //**************************************************//
    //          Load XML and get a node with the rf_mode
    //**************************************************//
    CoInitialize(NULL); // Initialize COM
    spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
    VARIANT_BOOL bOK = spXMLDoc->load(calDBXML.c_str());
    if (!spXMLDoc)
    {
        CoUninitialize();
        return false;
    }
    const double MAX_FREQ = 100000;
    if (FreqBegin < 0 ) { FreqBegin = 0; }
    if (FreqEnd < 0 ) { FreqEnd = MAX_FREQ; }

    if (FreqBegin > FreqEnd )
    {
        double temp = FreqEnd;
        FreqEnd = FreqBegin;
        FreqBegin = temp;

    }

    return true;
}

MSXML2::IXMLDOMNodeListPtr RFCal_Helper_GetXMLNodes(MSXML2::IXMLDOMNodePtr  pNode, const string & stXPATH)
{
    try
    {
        MSXML2::IXMLDOMNodeListPtr pTmp = pNode->selectNodes((_bstr_t)stXPATH.c_str()); 
        return pTmp; 
    }
    catch(...)
    {
        return NULL;
    }
}
MSXML2::IXMLDOMNodePtr RFCal_Helper_GetSingleXMLNode(MSXML2::IXMLDOMNodePtr  pNode, const string & stXPATH)
{
    try
    {
        MSXML2::IXMLDOMNodePtr pTmp = pNode->selectSingleNode((_bstr_t)stXPATH.c_str()); 
        return pTmp; 
    }
    catch(...)
    {
        return NULL;
    }

}
unsigned char RFCal_Helper_SetPathLossToCalDB_XML(const string & calDBXML, const string & NewCalDBXML, const string & calConfigName, word calPathNumber,map<double,double> & freqToPathLoss)
{
    MSXML2::IXMLDOMDocumentPtr spXMLDoc = NULL;
    double FreqBegin = 0.0; // Dummy value 
    double FreqEnd = 1000000.0; // Dummy value;

    // 1. Check parameter
    if (! ValidateGetPathLossParameters(spXMLDoc, calDBXML, calConfigName, calPathNumber, FreqBegin,FreqEnd)) { return false;}
    
    string tmpCalConfigName = calConfigName;
    word tmpCalPathNumber = calPathNumber;
    //**************************************************//
    //          Process CalPathLin
    //**************************************************//
    MSXML2::IXMLDOMNodePtr pCalPathLink = NULL;
    
    stringstream stXPATH_CalPathLink;
    stXPATH_CalPathLink << "/CalibrationData/CalConfig[@name='" << calConfigName << "']/CalPathLink[@srcPathNumber='" << calPathNumber << "']";
    pCalPathLink = RFCal_Helper_GetSingleXMLNode((MSXML2::IXMLDOMNodePtr)spXMLDoc,stXPATH_CalPathLink.str());
    if (pCalPathLink)
    {
        
        MSXML2::IXMLDOMNodePtr pSingleCalPathNode;
    
        MSXML2::IXMLDOMElementPtr element = pCalPathLink;
        _variant_t  varValueSrcCalConfig = element->getAttribute(L"srcCalConfig");
        _variant_t  varValuedestPathNumber = element->getAttribute(L"destPathNumber");

        tmpCalConfigName = RFCal_Helper_Convert_BSTR_to_String(varValueSrcCalConfig.bstrVal);;
        tmpCalPathNumber = static_cast<word>(varValuedestPathNumber);
    }
    //**************************************************//
    //          Get the root nodes giving tech and band
    //**************************************************//

    MSXML2::IXMLDOMNodePtr pRoot_Node;

    stringstream stXPATH;
    stXPATH << "/CalibrationData/CalConfig[@name='" << tmpCalConfigName << "']/CalPath[@number='" << tmpCalPathNumber << "']";

    pRoot_Node = RFCal_Helper_GetSingleXMLNode((MSXML2::IXMLDOMNodePtr)spXMLDoc,stXPATH.str());

    if (pRoot_Node == NULL) 
    {
        spXMLDoc.Release();
        CoUninitialize();
        cout << "Parameter not found";
        return false;
    }

    MSXML2::IXMLDOMNodeListPtr pCalPathgNodes = NULL;
    MSXML2::IXMLDOMNodeListPtr pCalPointerNodes = NULL;


    // To sort frequency, first read all the freq - pathloss data.
    HRESULT hr = S_OK; 
    map <double, double> AllFreqPassLoss;
    map <double, double> FreqPassLossToBeAdded;
    
    map <double, double>::iterator it;
    // Loop the updated map
    for (it = freqToPathLoss.begin(); it != freqToPathLoss.end(); it ++)
    {
        MSXML2::IXMLDOMNodePtr pCalPointPtr = NULL;
        double  keyFreq = static_cast<double>(it->first);
        double  Loss =  static_cast<double>(it->second);
        UINT iPrecision = 0;
        for (iPrecision = 0; iPrecision < 8; iPrecision ++ )
        {
            stringstream stXPATHForCalPoint;
            stXPATHForCalPoint << "CalPoint[@freq='" << std::fixed << std::setprecision(iPrecision) << keyFreq<< "']";
            string tmpDebugSt = stXPATHForCalPoint.str(); // this is for debug purpose.
            pCalPointerNodes = RFCal_Helper_GetXMLNodes((MSXML2::IXMLDOMNodePtr)pRoot_Node,stXPATHForCalPoint.str());
            if (pCalPointerNodes) 
            { 
                break;
            }
        }
        if (pCalPointerNodes)  // Find the same frequency in the CalDB_NET.xml, then update
        
        {
            long iNumCalConfigNodes;
            HRESULT hr = S_OK; 
            hr = pCalPointerNodes->get_length(&iNumCalConfigNodes);
            
            for (long iLoop = 0; iLoop < iNumCalConfigNodes; iLoop++)
            {
                MSXML2::IXMLDOMElementPtr element = pCalPointerNodes->nextNode();
                stringstream stLoss;
                stLoss << std::fixed << std::setprecision(1) << Loss;
                hr = element->setAttribute(L"loss",static_cast<variant_t>(stLoss.str().c_str()));
            }
        
        }
        else  // Did not find the same frequency in the CalDB_NET.xml, then add a new node
        {
            MSXML2::IXMLDOMElementPtr NewElement;
            MSXML2::IXMLDOMElementPtr Parentelement = pRoot_Node;
        
            stringstream stFreq;
            stFreq << keyFreq;
            stringstream stLoss;
            stLoss << std::fixed << std::setprecision(1) << Loss;

             
            NewElement = spXMLDoc->createElement((_bstr_t)"CalPoint");

            hr = NewElement->setAttribute((_bstr_t)"freq",static_cast<variant_t>(stFreq.str().c_str()));
            hr = NewElement->setAttribute((_bstr_t)"loss",static_cast<variant_t>(stLoss.str().c_str()));
            
            hr = Parentelement->appendChild(NewElement);

        }
    }

    if (calDBXML.compare(NewCalDBXML) == 0) // if two filename is the same
    {
        string backupFileName = calDBXML;
        backupFileName += ".bak";
        ::CopyFileA((_bstr_t)calDBXML.c_str(),(_bstr_t)backupFileName.c_str(),false);
    }

    spXMLDoc->save(NewCalDBXML.c_str());
    if (pRoot_Node) { pRoot_Node.Release(); pRoot_Node = NULL; }
    if (spXMLDoc) { spXMLDoc.Release(); spXMLDoc = NULL; }

    CoUninitialize();
    return true;
}


unsigned char RFCal_Helper_Intialize_TE_Records(vector < TE_Cal_Swp_Params > & TE_Records)
{
    vector < TE_Cal_Swp_Params >::iterator it;

    for (it = TE_Records.begin(); it != TE_Records.end(); it++)
    {
    //  TE_Cal_Swp_Params * p = &(*it);
        memset(&(*it),0,sizeof(TE_Cal_Swp_Params));
        it->m_teAction = true;
        it->m_dl_lvl = -115.0;
    }
    return true;
}
unsigned char RFCal_Helper_Config_TE_ULChannels(const vector < cal_sweep_record > & sweep, vector < TE_Cal_Swp_Params > & TE_Records)
{
    
    for (UINT iSeg = 0; iSeg < sweep.size(); iSeg++)
    {
        int ulchan = static_cast<int>(sweep[iSeg].radio_cfg.chan.chanInfo[0].ulChan);
        //double ULFreq = 0;
        //double DLFreq = 0;
        //::RFCal_Helper_CalculateFrequencyFromULChan(static_cast<int>(sweep[iSeg].radio_cfg.tech),static_cast<int>(sweep[iSeg].radio_cfg.band),ulchan,ULFreq,DLFreq);
        TE_Records[iSeg].m_ul_chan =  static_cast<UINT>(ulchan);
    
    }
    return true;
}
unsigned char RFCal_Helper_Is_FreqComp_SweepType( word sweep_type)
{
   if ((sweep_type== MAX_BIAS_FREQCOMP)
      || (sweep_type== APT_FREQCOMP)
      || (sweep_type== XPT_FREQCOMP)
      || (sweep_type== APT_FREQCOMP_STAGGER_RXTX)
      || (sweep_type== XPT_FREQCOMP_STAGGER_RXTX)
      || (sweep_type== INT_CAL_XPT_FREQCOMP)
      || (sweep_type== INT_CAL_MAX_BIAS_FREQCOMP)
      || (sweep_type== INT_CAL_APT_FREQCOMP)
      || (sweep_type== XPT_FREQCOMP_SERIAL_TX)
      || (sweep_type== XPT_FREQCOMP_SERIAL_RX))

   {
      return true;
   }
   return false;
}

unsigned char RFCal_Helper_Is_IntCal_SweepType( word sweep_type)
{
    if ((sweep_type== INT_CAL_XPT_SWP1)
        || (sweep_type== INT_CAL_XPT_SWP2)
        || (sweep_type== INT_CAL_XPT_SWP3)
        || (sweep_type== INT_CAL_XPT_FREQCOMP)
        || (sweep_type== INT_CAL_MAX_BIAS_LINEARIZER)
        || (sweep_type== INT_CAL_APT_LINEARIZER)
        || (sweep_type== INT_CAL_MAX_BIAS_FREQCOMP)
        || (sweep_type== INT_CAL_APT_FREQCOMP))
    {
        return true;
    }
    return false;
}

unsigned char RFCal_Helper_Execute_TearDown_Sweep(HANDLE hPhone, const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, const Cal_Param & cal_param)
{
   vector <cal_sweep_record> Sweep;
    RFCal_Helper_ESC_ET_Initialize_Sweep(Sweep,cal_param.Swp[0],100,(int)demo_param.TECH,demo_param.BAND);


   const Sweep_Params * pSweep_param = &(cal_param.Swp[0]);
   UINT recordIndex = 0;
    //*****************************************************************************************//
    //  OpCode 0x100: Radio Setup
    //*****************************************************************************************//
    RFCal_Helper_setOpCode_RadioSetup(Sweep[recordIndex], 
        pSweep_param->Device_Selection.Master_Device_ID, pSweep_param->Device_Selection.Slave_Device_ID, 
        0   /* Master Device State */, 
        0   /* Slave Device State */, 
        demo_param.TECH, 
        demo_param.BAND, 
        pSweep_param->Ref_Channel /* ulChan */, 
        pSweep_param->Waveform.Waveform_Type, 
        NULL, 
        pSweep_param->Waveform.Number_Of_RBs, 
        pSweep_param->Waveform.Start_RB);

    RFCal_Helper_setConfigTime_InsertNop(Sweep,recordIndex,pSweep_param->Config_Segment_Length); // New change from QDART 4821

   //*****************************************************************************************/
    //      End of Sweep : Insert a low rgi segment instead of tear down
    //*****************************************************************************************/
    RFCal_Helper_setLowRGISegment(Sweep[recordIndex],cal_param.TxDeviceFlag);
    RFCal_Helper_RecordIndex_Increment(Sweep,  recordIndex);

    //*********************************************//
    //      Generate OpCode for all the segment
    //*********************************************//
    Sweep.resize(recordIndex); // Delete unused records
    RFCal_Helper_setOpcode(demo_param.hPhone, Sweep,  Sweep.size());
 
   //*********************************************//
   //       Execute Sweep
   //*********************************************//
   ::RFCal_Helper_AddSectionTitleToQMSLLog(hPhone,"Tead down Sweep");
        RFCal_Helper_ExecEscSweep(hPhone, 
                    Sweep /* Array of Sweep records */,  
                    Sweep.size() /* Size of Sweep records */, 
                    true /* is Last Sweep. */, 
                    cal_param.En_Compress_Esc_Req /* Compress diag request or not */, 
                    cal_param.En_Debug_Mode /* Debug mode */);
   return true;
}
unsigned char RFCal_Helper_DisableAllExtededMsgLogEvent(HANDLE hPhone)
{
   QLIB_DIAG_SetAllMessageFilter(hPhone,0);
   QLIB_DIAG_SetEntireEventMaskState(hPhone, false);
   for (unsigned char iEquipmentID = 0; iEquipmentID < 15; iEquipmentID++)
   {
      QLIB_DIAG_ClearAllLogCodes(hPhone,iEquipmentID);
   }
   return true;
}

MSXML2::IXMLDOMElementPtr  RFCal_Helper_AddNewTextElement( MSXML2::IXMLDOMDocument2Ptr xmlDoc, MSXML2::IXMLDOMElementPtr baseElement, string newElementName, string Text)
{
    if ( xmlDoc == NULL ) return NULL;
    if (baseElement == NULL) return NULL;

    MSXML2::IXMLDOMElementPtr newElement;

    newElement = xmlDoc->createElement((_bstr_t)((newElementName.c_str())));
    if (Text.length() > 0 )
    {
        newElement->text = (_bstr_t )(Text.c_str());
    }
    baseElement->appendChild(newElement);

    return newElement;
}

MSXML2::IXMLDOMElementPtr RFCal_Helper_AddTest( MSXML2::IXMLDOMDocument2Ptr xmlDoc, MSXML2::IXMLDOMElementPtr baseElement, string newTestName, string newTestExtendedName, string newTestNodeName)
{
    if ( xmlDoc == NULL ) return NULL;
    if (baseElement == NULL) return NULL;

    MSXML2::IXMLDOMElementPtr newTestElement;

    newTestElement = xmlDoc->createElement((_bstr_t)"Test");
    if (newTestElement == NULL)
    {
        return NULL;
//      newElement->text = (_bstr_t )(Text.c_str());
    }
    RFCal_Helper_AddNewTextElement(xmlDoc,newTestElement,"Name",newTestName);
    RFCal_Helper_AddNewTextElement(xmlDoc,newTestElement,"ExtendedName",newTestExtendedName);
    RFCal_Helper_AddNewTextElement(xmlDoc,newTestElement,"NodeName",newTestNodeName);


    baseElement->appendChild(newTestElement);

//  newElement.Release();
    return newTestElement;
}

MSXML2::IXMLDOMElementPtr RFCal_Helper_DataSet( MSXML2::IXMLDOMDocument2Ptr xmlDoc, MSXML2::IXMLDOMElementPtr DataCollectionElement, MSXML2::IXMLDOMElementPtr DataSetElementPtr, unsigned char bInput, string ValueN, string ValueV, unsigned char bStatus)
{
    if ( xmlDoc == NULL ) return NULL;
    if (DataCollectionElement == NULL) return NULL;

    MSXML2::IXMLDOMNodePtr DataSetNodePtr;

    stringstream stXPATH;

    if (DataSetElementPtr == NULL)
    {
        DataSetElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,DataCollectionElement,"DataSet","");
        DataSetNodePtr = DataSetElementPtr;
    }
    else
    {
        DataSetNodePtr = DataSetElementPtr;
    }
    stXPATH.str("");
    MSXML2::IXMLDOMNodePtr InputOutputNodePtr;
    MSXML2::IXMLDOMElementPtr InputOutputElementPtr;
    string stInputOutput = "";

   if (bInput == RESULT_MEAS)
   {
      stInputOutput = "Outputs";
   }
   else if (bInput == RESULT_CONF)
    {
        stInputOutput = "Inputs";
    }
    else if (bInput == RESULT_DSREF)
    {
        stInputOutput = "DSRef";
    }
    else if (bInput == RESULT_DESC)
    {
        stInputOutput = "Description";
    }
    else if (bInput == RESULT_MIN)
    {
        stInputOutput = "Outputs";
    }
   else if (bInput == RESULT_MAX)
    {
        stInputOutput = "Outputs";
   }
        stXPATH << "./" << stInputOutput;

    InputOutputNodePtr = DataSetNodePtr->selectSingleNode((_bstr_t)(stInputOutput.c_str()));
    if (InputOutputNodePtr == NULL)
    {
        InputOutputElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,DataSetElementPtr,stInputOutput,"");
    }
    else
    {
        InputOutputElementPtr = InputOutputNodePtr;
    }

    if (InputOutputElementPtr == NULL) 
    {
        return NULL;
    }

    // Add Result Node only for Outputs
    if (bInput == RESULT_MEAS)
    {
        InputOutputElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,InputOutputElementPtr,"Result","");
    }

    if (bInput == RESULT_DSREF)
    {
        InputOutputElementPtr = InputOutputElementPtr->setAttribute((_bstr_t)"id",static_cast<variant_t>(ValueN.c_str()));
        return InputOutputElementPtr;
    }


    MSXML2::IXMLDOMElementPtr DIElementPtr;
    
    DIElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,InputOutputElementPtr,"DI","");
    
    if (DIElementPtr != NULL)
    {
        RFCal_Helper_AddNewTextElement(xmlDoc,DIElementPtr,"N",ValueN);
        RFCal_Helper_AddNewTextElement(xmlDoc,DIElementPtr,"V",ValueV);
    }

    if (!bStatus)
    {
        MSXML2::IXMLDOMElementPtr LimitElementPtr;
    
        LimitElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,InputOutputElementPtr,"Limits","");
    
        MSXML2::IXMLDOMElementPtr PassFaillementPtr;

        PassFaillementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,LimitElementPtr,"PassFail","");

        MSXML2::IXMLDOMElementPtr DI_PassFail_ElementPtr;
    
        DI_PassFail_ElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,PassFaillementPtr,"DI","");

        if (DI_PassFail_ElementPtr != NULL)
        {
            RFCal_Helper_AddNewTextElement(xmlDoc,DI_PassFail_ElementPtr,"N","Status");
            RFCal_Helper_AddNewTextElement(xmlDoc,DI_PassFail_ElementPtr,"V","FAIL");
        }

    }

    return DataSetElementPtr;
}

template<class T> MSXML2::IXMLDOMElementPtr RFCal_Helper_DataSet( MSXML2::IXMLDOMDocument2Ptr xmlDoc, MSXML2::IXMLDOMElementPtr DataCollectionElement, MSXML2::IXMLDOMElementPtr DataSetElementPtr, unsigned char bInput, string ValueN, T ValueV, unsigned char bStatus)
{
    stringstream ss;
    if (typeid(T) == typeid(byte))
    {
        ss << static_cast<UINT>(ValueV);
    }
    else if (typeid(T) == typeid(char))
    {
        ss << static_cast<short>(static_cast<char>(ValueV));
    }
    else if (typeid(T) == typeid(word))
    {
        ss << static_cast<word>(ValueV);
    }
    else if (typeid(T) == typeid(short))
    {
        ss << static_cast<short>(ValueV);
    }
    else if (typeid(T) == typeid(int))
    {
        ss << static_cast<int>(ValueV);
    }
    else if (typeid(T) == typeid(long))
    {
        ss << static_cast<int>(ValueV);
    }
   else if (typeid(T) == typeid(double))
    {
        ss << static_cast<double>(ValueV);
    }
    else if (typeid(T) == typeid(UINT))
    {
        ss << static_cast<UINT>(ValueV);
    }
    else if (typeid(T) == typeid(dword))
    {
        ss << static_cast<dword>(ValueV);
    }
    else if (typeid(T) == typeid(signed long long))
    {
        ss << static_cast<signed long long>(ValueV);
    }
    else if (typeid(T) == typeid(unsigned long long))
    {
        ss << static_cast<unsigned long long>(ValueV);
    }
    else if (typeid(T) == typeid(string))
    {
        ss << ValueV;
    }

    return RFCal_Helper_DataSet( xmlDoc, DataCollectionElement, DataSetElementPtr, bInput, ValueN,(ss.str()));
}

unsigned char RFCal_Helper_Generate_XMLReport(string stXMLName, MSXML2::IXMLDOMDocument2Ptr & xmlDoc)
{
   HRESULT hr = S_OK;
   //**************************************************//
   //           Parameters
   //**************************************************//
   MSXML2::IXMLDOMNodePtr pTestCollection;
   MSXML2::IXMLDOMNodePtr pDataNode;

   MSXML2::IXMLDOMElementPtr element;
   MSXML2::IXMLDOMElementPtr Parentelement;
    
   _variant_t varValue;
   stringstream stXPATH;

    CoInitialize(NULL); // Initialize COM
  

   //**************************************************//
   //           Create Processing Intruction
   //**************************************************//
   try
   {
      xmlDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
    
      if ( xmlDoc->load((_bstr_t) stXMLName.c_str()) != VARIANT_TRUE)
      {
         MSXML2::IXMLDOMProcessingInstructionPtr pPI = NULL;
         xmlDoc.CreateInstance("Msxml2.DOMDocument.6.0");
         pPI = xmlDoc->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
         varValue.vt = VT_NULL;
         xmlDoc->insertBefore(pPI, varValue);

           pPI = xmlDoc->createProcessingInstruction("xml-stylesheet", "type='text/xsl' href='C:\\Program Files (x86)\\Qualcomm\\QDART\\Databases\\HTML_QDART_Combined.xsl'");

         xmlDoc->insertBefore(pPI, varValue);

           //**************************************************//
         //         Create  Source Root node
         //**************************************************//

         xmlDoc->raw_createElement((_bstr_t)("source"),&element);
         xmlDoc->raw_appendChild(element,NULL);
    
      }
      pTestCollection = xmlDoc->selectSingleNode((_bstr_t)"source");

      if (pTestCollection == NULL)
      {
         xmlDoc.Release();
         CoUninitialize();
         return false;
      }

      stXPATH << "/source/TestCollection" ;

      pDataNode = xmlDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());

      if (pDataNode == NULL)
      {
         //**************************************************//
         //         Add TestCollection 
         //**************************************************//

         pTestCollection = xmlDoc->selectSingleNode((_bstr_t)"source");

         Parentelement = pTestCollection;
         element = xmlDoc->createElement((_bstr_t)"TestCollection");
           Parentelement->appendChild(element);

           RFCal_Helper_AddNewTextElement(xmlDoc,element, "Name","QDART Test Station");
                   // Add Tester Node
           RFCal_Helper_AddNewTextElement(xmlDoc,element, "Tester","");

          MSXML2::IXMLDOMElementPtr childElement = RFCal_Helper_AddNewTextElement(xmlDoc,element, "Date","");

         time_t curtime;
            struct tm *loctimep;
            curtime = time( NULL );
            loctimep = localtime( &curtime );

         stringstream ss;
 
         ss << (loctimep->tm_mon+1) ;
         RFCal_Helper_AddNewTextElement(xmlDoc,childElement,"MM",ss.str());
         ss.str("");

         ss << ( loctimep->tm_mday) ;
         RFCal_Helper_AddNewTextElement(xmlDoc,childElement,"DD",ss.str());
         ss.str("");
 
         ss << loctimep->tm_year+1900;
         RFCal_Helper_AddNewTextElement(xmlDoc,childElement,"YYYY",ss.str());
         ss.str("");

           stXPATH.str("");
           stXPATH << "/source/TestCollection/Tester";
           pTestCollection = xmlDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());
           Parentelement = pTestCollection;

       element = xmlDoc->createElement((_bstr_t)"Transformed");
           Parentelement->appendChild(element);

           ::RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "Name","QLIB Demo");
           ::RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "Type","QLIB Demo Test Station");
 

      }
       stXPATH.str("");
       stXPATH << "/source/TestCollection";
     // pTestCollection = xmlDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());
   } catch (...)
   {

      return false;
   }
   return true;
}
unsigned char RFCal_Helper_ExecuteXSLT(const string & XMLfilename,const string & htmlname,const  string & xslfilename)
{
    
    if ( _access( xslfilename.c_str(), 0 ) != 0 )
    {
        return false;
    }

    stringstream ssArgument;
    ssArgument << "msxsl " << "\"" << XMLfilename << "\" \"" << xslfilename << "\" -o " << "\"" <<  htmlname << "\"";

    string stCommand = ssArgument.str();
    wstring wstCommand(stCommand.begin(), stCommand.end());
    LPWSTR lpwstrCommand = &wstCommand[0];
    

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    GetStartupInfoW(&si);

    CreateProcessW(NULL,lpwstrCommand,NULL, NULL,false,NULL,NULL,NULL, &si, &pi);
    CloseHandle(pi.hThread);
    WaitForSingleObject(pi.hProcess,INFINITE);
    CloseHandle(pi.hProcess);

    return 0;
}
unsigned char RFCal_Helper_AddTestName(MSXML2::IXMLDOMDocument2Ptr & xmlDoc, MSXML2::IXMLDOMElementPtr & TestPtr, const string & stTestType, const string & stTestName, const string & stNodeName)
{
   stringstream stXPATH;
   MSXML2::IXMLDOMNodePtr pNode;
   MSXML2::IXMLDOMElementPtr Parentelement;

    stXPATH << "/source/TestCollection";
    pNode = xmlDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());
    Parentelement = pNode;

    TestPtr = RFCal_Helper_AddTest(xmlDoc,Parentelement,stTestType,stTestName,stNodeName);

   return true;
}
unsigned char RFCal_Helper_SetUUT_BuildID(MSXML2::IXMLDOMDocument2Ptr & xmlDoc, const string & stUUTID, const string & stBuildID)
{
   stringstream stXPATH;
   MSXML2::IXMLDOMNodePtr pNode;
   MSXML2::IXMLDOMElementPtr Parentelement;
   MSXML2::IXMLDOMElementPtr element;

    stXPATH << "/source/TestCollection";
    pNode = xmlDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());
    Parentelement = pNode;

   element = xmlDoc->createElement((_bstr_t)"UUT");
   Parentelement->appendChild(element);
   Parentelement = element;

   RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "ID",stUUTID);
   RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "SWBuildID",stBuildID);
   RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "Type","GENERIC");
 

   return true;
}
unsigned char RFCal_Helper_SetRunInfo(MSXML2::IXMLDOMDocument2Ptr & xmlDoc, MSXML2::IXMLDOMElementPtr & TestPtr,
   const QLIB_Demo_RFCAL_ESC_ET_Param & demo_param, dword iDuration,const time_t & starTime_t,const time_t & stopTime_t)
{
   MSXML2::IXMLDOMNodePtr pNode;
   MSXML2::IXMLDOMElementPtr Parentelement;
   MSXML2::IXMLDOMElementPtr TestPtrElement;
 
   MSXML2::IXMLDOMElementPtr element;

   if (TestPtr == NULL)
   {
      stringstream stXPATH;
      stXPATH << "/source/TestCollection";

      pNode = xmlDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());
       TestPtrElement = pNode;
   }
   else
   {
      TestPtrElement = TestPtr;
   }
   MSXML2::IXMLDOMElementPtr DataSetCollectionElementPtr = NULL;
   DataSetCollectionElementPtr = RFCal_Helper_AddNewTextElement(xmlDoc,TestPtr,"DataSetCollection","");
   RFCal_Helper_AddNewTextElement(xmlDoc,DataSetCollectionElementPtr,"Name","Run Info");

   MSXML2::IXMLDOMElementPtr  DataSetElementPtr = NULL;
   DataSetElementPtr = 
   RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Switchpoint Data", static_cast<string>("Stream Not supported") );
   RFCal_Helper_DataSet(xmlDoc,DataSetCollectionElementPtr,DataSetElementPtr, RESULT_CONF,"Temp", 22);
   DataSetElementPtr->setAttribute((_bstr_t)"id",static_cast<variant_t>("15"));
   DataSetElementPtr->setAttribute((_bstr_t)"ts",static_cast<variant_t>("200"));
   DataSetElementPtr->setAttribute((_bstr_t)"dur",static_cast<variant_t>("5715"));
   string stTech[] = {"CDMA","WCDMA","GSM","LTE","TDSCDMA"};

   return true;
}
unsigned char RFCal_Helper_SetTime_Duration(MSXML2::IXMLDOMDocument2Ptr & xmlDoc, MSXML2::IXMLDOMElementPtr & TestPtr, dword iDuration,const time_t & starTime_t,const time_t & stopTime_t)
{
   MSXML2::IXMLDOMNodePtr pNode;
   MSXML2::IXMLDOMElementPtr Parentelement;
   MSXML2::IXMLDOMElementPtr TestPtrElement;
 
   MSXML2::IXMLDOMElementPtr element;

   if (TestPtr == NULL)
   {
      stringstream stXPATH;
      stXPATH << "/source/TestCollection";

      pNode = xmlDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());
       TestPtrElement = pNode;
   }
   else
   {
      TestPtrElement = TestPtr;
   }
   element = xmlDoc->createElement((_bstr_t)"Time");
   TestPtrElement->appendChild(element);
   TestPtrElement = element;

   {
      element = xmlDoc->createElement((_bstr_t)"Start");
      TestPtrElement->appendChild(element);
      Parentelement = element;
        struct tm *loctimep;
        loctimep = localtime( &starTime_t );
    
      stringstream ss;
      ss << std::setfill('0') << std::setw(2) << loctimep->tm_sec;
      RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "SS",ss.str());
      ss.str(""); 
      ss << std::setfill('0') << std::setw(2) << loctimep->tm_min;
      RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "MM",ss.str());
      ss.str(""); 
      ss << std::setfill('0') << std::setw(2) << loctimep->tm_hour;
      RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "HH",ss.str());
   }
   {
      element = xmlDoc->createElement((_bstr_t)"Stop");
      TestPtrElement->appendChild(element);
      Parentelement = element;
        struct tm *loctimep;
        loctimep = localtime( &stopTime_t );
    
      stringstream ss;
      ss << std::setfill('0') << std::setw(2) << loctimep->tm_sec;
      RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "SS",ss.str());
      ss.str(""); 
      ss << std::setfill('0') << std::setw(2) << loctimep->tm_min;
      RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "MM",ss.str());
      ss.str(""); 
      ss << std::setfill('0') << std::setw(2) << loctimep->tm_hour;
      RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "HH",ss.str());
   }
   // Duration
   {
   element = xmlDoc->createElement((_bstr_t)"Duration");
   TestPtrElement->appendChild(element);
   Parentelement = element;
  

   stringstream ss;
   int iSec = iDuration / 1000;
   int iMin = iSec / 60;
   int iHour = iMin / 60;
   iMin = iMin - 60*iHour;
   iSec = iSec - (3600*iHour + 60*iMin);
   iDuration = iDuration - (1000*iSec);
   ss << iDuration;
   RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "MS",ss.str());
   
   ss.str(""); 
   ss << std::setfill('0') << std::setw(2) << iSec; 
   RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "SS",ss.str());
   ss.str(""); 
   ss << std::setfill('0') << std::setw(2) << iMin; 

   RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "MM",ss.str());
   ss.str(""); 
   ss << std::setfill('0') << std::setw(2) << iHour; 

   RFCal_Helper_AddNewTextElement(xmlDoc,Parentelement, "HH",ss.str());

  }
   return true;
}


template<typename T> T RFCal_Helper_GetIndexFromBitMask(T bitMask, T MaxIndex)
{
   T index = 0;
   
   // If MaxIndex is not specified, then set MaxIndex from the size of T
   if ( MaxIndex == 0)
   {
      MaxIndex = sizeof(T)*8;
   }

   for (T id = 0; id < MaxIndex; id++)
   {
      if (bitMask & ( 1 << id ))
      {
         index = id;
         break;
      }
   }
   return index;
}


void RFCal_Helper_Enh_Internal_Device_Param_Assigment_From_XTT(HANDLE hPhone,  int TECH, int BAND,  unsigned char isSCell, int int_cal_type_mask, const string & XTTFilename, XTT_Enh_Internal_Dev_Cal_Param & params)
{
   //**************************************************//
   //       parameters
   //**************************************************//
   MSXML2::IXMLDOMDocumentPtr spXMLDoc;
   MSXML2::IXMLDOMNodePtr pRoot_Node;
   stringstream stXPATH;

   //**************************************************//
   //       Load XML and get a node with the rf_mode
   //**************************************************//
   CoInitialize(NULL); // Initialize COM
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
   spXMLDoc->load(XTTFilename.c_str());
   if (!spXMLDoc)
   {
      CoUninitialize();
      throw RFCal_Helper_Exception("The XTT not loaded.",  __FILE__, __LINE__ );
   }
   //**************************************************//
   //       Get the root nodes giving tech and band
   //**************************************************//
   if (isSCell == false )
   {
      stXPATH << "/SerializeComponents/.//HiveProperties[RealName='RFCal_Enh_Internal_Device_Cal']/Parameters[ParameterCls[Name='Technology' and Value='" << TECH
              << "'] and ParameterCls[Name='Band' and Value='" << BAND
              << "'] and ParameterCls[Name='Internal_Cal_Type_Mask' and Value='" << int_cal_type_mask << "'] ]" ;
   }
   else if (isSCell == (unsigned char)true && int_cal_type_mask == 4 )
   {
      stXPATH << "/SerializeComponents/.//HiveProperties[RealName='RFCal_Enh_Internal_Device_Cal' and TestName='LTE_B" <<(int)BAND<<"_SCell_Rx_Rsb_Cal']/Parameters[ParameterCls[Name='Technology' and Value='" << TECH
              << "'] and ParameterCls[Name='Band' and Value='" << BAND
              << "'] and ParameterCls[Name='Internal_Cal_Type_Mask' and Value='" << int_cal_type_mask << "'] ]" ;
   }

   pRoot_Node = spXMLDoc->selectSingleNode((_bstr_t)stXPATH.str().c_str());

   if (!pRoot_Node)
   {
      spXMLDoc.Release();
      CoUninitialize();
      throw RFCal_Helper_Exception("The tech and band are not found in the XTT.",  __FILE__, __LINE__ );
   }
   // Default Values used if the XTT does not define the values

   // Root node

   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Mode']/Value",params.RF_Mode);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='NV Mode']/Value",params.NV_Mode);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Technology']/Value",params.Technology);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Band']/Value",params.Band);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='iChannel']/Value",params.iChannel);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Tx Bandwidth']/Value",params.TxBandwidth);

   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='iPAstate']/Value",params.iPAstate);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Tx Gain Index']/Value",params.TxGainIndex);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='iBiasValue']/Value",params.iBiasValue);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Is_Debug_Mode_Enabled']/Value",params.is_Debug_Mode_Enabled);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Internal_Cal_Type_Mask']/Value",params.Internal_Cal_Type_Mask);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FBRx_Tech_Bandwidth_Config']/Value",params.FBRx_Tech_Bandwidth_Config);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Device_Mask']/Value",params.Device_Mask);

    // FB Droop
    if (params.Internal_Cal_Type_Mask == 0)
    {
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Fb_Droop_Method']/Value",params.FbRx_Droop_Cal.FB_Droop_Method);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Tx_Device']/Value",params.FbRx_Droop_Cal.Tx_Device);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Fb_Rx_Gain_State']/Value",params.FbRx_Droop_Cal.Fb_Rx_Gain_State);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:lte_fbrxtech_bw_list']/Value",params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:chan_lte_10M']/Value",params.FbRx_Droop_Cal.Chan_Lte_10M);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:chan_lte_15M']/Value",params.FbRx_Droop_Cal.Chan_Lte_15M);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:stg_freq_offset_khz_list']/Value",params.FbRx_Droop_Cal.stg_freq_offset_kHz_list);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:stg_freq_offset_khz_list_lte10M']/Value",params.FbRx_Droop_Cal.stg_freq_offset_kHz_list_Lte10M);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:stg_freq_offset_khz_list_lte15M']/Value",params.FbRx_Droop_Cal.stg_freq_offset_kHz_list_Lte15M);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Num_IQ_Samples']/Value",params.FbRx_Droop_Cal.Num_IQ_Samples);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Enable_Processing']/Value",params.FbRx_Droop_Cal.Enable_Processing);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Coeff_Min_Lim']/Value",params.FbRx_Droop_Cal.Coeff_Min_Lim);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Coeff_Max_Lim']/Value",params.FbRx_Droop_Cal.Coeff_Max_Lim);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Tone_Pwr_Min_Lim_dB']/Value",params.FbRx_Droop_Cal.Tone_Pwr_Min_Lim_dB);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Tone_Pwr_Max_Lim_dB']/Value",params.FbRx_Droop_Cal.Tone_Pwr_Max_Lim_dB);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:NV_is_band_specific']/Value",params.FbRx_Droop_Cal.NV_is_band_specific);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Verify_Cal']/Value",params.FbRx_Droop_Cal.Verify_Cal);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Verification_Method' and ParentName='FbRX:Droop Cal:Verify_Cal']/Value",params.FbRx_Droop_Cal.Verification_Method);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:LTE5_Droop_PP_Limit_dB100' and ParentName='FbRX:Droop Cal:Verify_Cal']/Value",params.FbRx_Droop_Cal.LTE5_Droop_PP_Limit_dB100);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:LTE10_Droop_PP_Limit_dB100' and ParentName='FbRX:Droop Cal:Verify_Cal']/Value",params.FbRx_Droop_Cal.LTE10_Droop_PP_Limit_dB100);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:LTE15_Droop_PP_Limit_dB100' and ParentName='FbRX:Droop Cal:Verify_Cal']/Value",params.FbRx_Droop_Cal.LTE15_Droop_PP_Limit_dB100);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:LTE20_Droop_PP_Limit_dB100' and ParentName='FbRX:Droop Cal:Verify_Cal']/Value",params.FbRx_Droop_Cal.LTE20_Droop_PP_Limit_dB100);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Samp_Freq_List_Per_BW_Mode_MHz' and ParentName='FbRX:Droop Cal:Verify_Cal']/Value",params.FbRx_Droop_Cal.Sampl_Freq_List_Per_BW_Mode_MHz);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Band_Edge_List_Per_BW_Mode_MHz' and ParentName='FbRX:Droop Cal:Verify_Cal']/Value",params.FbRx_Droop_Cal.Band_Edge_List_Per_BW_Mode_MHz);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Residual_Droop_Min_Lim_dB' and ParentName='FbRX:Droop Cal:Verify_Cal']/Value",params.FbRx_Droop_Cal.Residual_Droop_Min_Lim_dB);
        RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRX:Droop Cal:Residual_Droop_Max_Lim_dB' and ParentName='FbRX:Droop Cal:Verify_Cal']/Value",params.FbRx_Droop_Cal.Residual_Droop_Max_Lim_dB);
    }
   // DC Cal

   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRx:DC Cal:Fb_Rx_Gain_State_List' and ParentName='FbRX:DC Cal']/Value",params.FbRx_DC_Cal.Fb_Rx_Gain_State_List);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRx:DC Cal:Num_IQ_Samples' and ParentName='FbRX:DC Cal']/Value",params.FbRx_DC_Cal.Num_IQ_Samples);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRx:DC Cal:Min DC_I Limit)' and ParentName='FbRX:DC Cal']/Value",params.FbRx_DC_Cal.Min_DC_I_Limit);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRx:DC Cal:Max DC_I Limit)' and ParentName='FbRX:DC Cal']/Value",params.FbRx_DC_Cal.Max_DC_I_Limit);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRx:DC Cal:Min DC_Q Limit)' and ParentName='FbRX:DC Cal']/Value",params.FbRx_DC_Cal.Min_DC_Q_Limit);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='FbRx:DC Cal:Max DC_Q Limit)' and ParentName='FbRX:DC Cal']/Value",params.FbRx_DC_Cal.Max_DC_Q_Limit);

   // RX_FbRx:RSB Cal

   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal:Rsb_Type' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.Rsb_Type);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal:Signal Source' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.Signal_Source);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal:waveform offset (in kHz)' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.Waveform_Offset_in_kHz);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal:Number of Gain States' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.Number_Of_Gain_States);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal:Gain_State_List' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.Rx_Gain_State_List);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal:Power_List' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.Power_List);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal:Number of Gain States' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.Number_Of_Gain_States);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal: IQ Capture Type' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.IQ_Capture_Type);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal:Num_IQ_Samples' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.Num_IQ_Samples);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal: Carrier to be calibrated' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.Carrier_To_Be_Calibrated);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal: Scell Mode' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.SCellMode);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal: Pcell Mode For SCell (LTE Only)' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.PCellModeForSCell);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal: Pcell band' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.PCellBand);
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal: Pcell Ref Channel' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.PCellRefChannel);
    RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal: Write to rsb_efs file' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.WriteToRsb_efsFile,static_cast<word>(1));
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal: post-cal rsb limit' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.Post_Cal_Rsb_Limit,static_cast<word>(40));
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal: pre-cal rsb limit' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.Pre_Cal_Rsb_Limit,static_cast<word>(26));
   RFCal_Helper_Read_Param(pRoot_Node,"ParameterCls[Name='Rsb Cal: do rsb limit check?' and ParentName='RX_FbRx:RSB Cal']/Value",params.Rx_FbRx_RSB_Cal.Do_Rsb_Limit_Check,static_cast<byte>(0));


}

void RFCal_Helpder_Enh_Internal_Device_Request_Param_assignment(const XTT_Enh_Internal_Dev_Cal_Param & params, common_enh_internal_device_cal & enh_internal_device_cal)
{
   // common head init
   memset(&enh_internal_device_cal,0, sizeof(enh_internal_device_cal));

   enh_internal_device_cal.common_header.req_len=sizeof(enh_internal_device_cal.common_header);
   enh_internal_device_cal.common_header.resp_len=0;
   enh_internal_device_cal.common_header.res_rev=3;
   enh_internal_device_cal.common_header.dbg_mode_en= params.is_Debug_Mode_Enabled;
   enh_internal_device_cal.common_header.flag=params.Rx_FbRx_RSB_Cal.WriteToRsb_efsFile;   // New Change Default value is 1 if not defined.
   enh_internal_device_cal.common_header.int_cal_type_mask = params.Internal_Cal_Type_Mask;
   enh_internal_device_cal.common_header.tech= params.Technology;
   enh_internal_device_cal.common_header.bw= params.FBRx_Tech_Bandwidth_Config;
   enh_internal_device_cal.common_header.band =params.Band;
   enh_internal_device_cal.common_header.ref_chan= params.iChannel;
   enh_internal_device_cal.common_header.dev_mask= params.Device_Mask;

   if (params.Internal_Cal_Type_Mask == ENH_INTERNAL_DEVICE_CAL_FBDROOP) // Droop cal
   {
       // only do more captures for older verification method, otherwise no additional captures using FIR freqz method
       if (params.FbRx_Droop_Cal.Verify_Cal == 1 && params.FbRx_Droop_Cal.Verification_Method == 0 ) 
       {
           if (params.FbRx_Droop_Cal.FB_Droop_Method == 0)
           {
               enh_internal_device_cal.common_header.dbg_mode_en = 1;
           }
           else if (params.FbRx_Droop_Cal.FB_Droop_Method == 1)
           {
               enh_internal_device_cal.common_header.dbg_mode_en = 2;
           }
       }

       enh_internal_device_cal.droop_instr.fb_droop_method = params.FbRx_Droop_Cal.FB_Droop_Method;
       enh_internal_device_cal.droop_instr.tx_device = params.FbRx_Droop_Cal.Tx_Device;
       enh_internal_device_cal.droop_instr.fbrx_gain_state = params.FbRx_Droop_Cal.Fb_Rx_Gain_State;
       enh_internal_device_cal.droop_instr.num_Captures = static_cast<byte>(params.FbRx_Droop_Cal.stg_freq_offset_kHz_list.size());
       enh_internal_device_cal.droop_instr.num_iq_samples = params.FbRx_Droop_Cal.Num_IQ_Samples;
       enh_internal_device_cal.droop_instr.do_processing = params.FbRx_Droop_Cal.Enable_Processing;
       memset(& enh_internal_device_cal.droop_instr.stg_freq_offset_list_khz[0], 0, sizeof( long ) * NUM_MAX_STG_FREQ_OFFS_FBRX_DROOP);
       for (byte i = 0; i < params.FbRx_Droop_Cal.stg_freq_offset_kHz_list.size(); i++)
       {
           enh_internal_device_cal.droop_instr.stg_freq_offset_list_khz[i] = static_cast<unsigned int>(params.FbRx_Droop_Cal.stg_freq_offset_kHz_list[i]);
       }

        //**************************************************//
        //  Override droop parameter depending on bandwidth
        //**************************************************//

        // Check LTE BW10 or 20 ro 15MHz
        byte bLTE_BW10MHz = false;
        byte bLTE_BW20MHz = false;
        byte bLTE_BW15MHz = false;

        const unsigned char LTE_BW10MHZ = 13;
        const unsigned char LTE_BW15MHZ = 3;
        const unsigned char LTE_BW20MHZ = 2;

        bLTE_BW10MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW10MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(); 
        bLTE_BW20MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW20MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end();
        if (!bLTE_BW20MHz)
        {
            bLTE_BW15MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW15MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end();
        }
        if (bLTE_BW10MHz)
        {
            enh_internal_device_cal.common_header.bw = LTE_BW10MHZ;
            enh_internal_device_cal.common_header.ref_chan = params.FbRx_Droop_Cal.Chan_Lte_10M;
            enh_internal_device_cal.droop_instr.num_Captures = static_cast<unsigned char>(params.FbRx_Droop_Cal.stg_freq_offset_kHz_list_Lte10M.size());
            memset(& enh_internal_device_cal.droop_instr.stg_freq_offset_list_khz[0], 0, sizeof( long ) * NUM_MAX_STG_FREQ_OFFS_FBRX_DROOP);
            for (byte i = 0; i < params.FbRx_Droop_Cal.stg_freq_offset_kHz_list.size(); i++)
            {
               enh_internal_device_cal.droop_instr.stg_freq_offset_list_khz[i] = static_cast<unsigned int>(params.FbRx_Droop_Cal.stg_freq_offset_kHz_list_Lte10M[i]);
            }
        }
        else if (bLTE_BW20MHz || bLTE_BW15MHz)
        {
            enh_internal_device_cal.common_header.bw = (bLTE_BW20MHz)?LTE_BW20MHZ : LTE_BW15MHZ;
            enh_internal_device_cal.common_header.ref_chan = params.FbRx_Droop_Cal.Chan_Lte_15M;
            enh_internal_device_cal.droop_instr.num_Captures = static_cast<unsigned char>(params.FbRx_Droop_Cal.stg_freq_offset_kHz_list_Lte15M.size());
            memset(& enh_internal_device_cal.droop_instr.stg_freq_offset_list_khz[0], 0, sizeof( long ) * NUM_MAX_STG_FREQ_OFFS_FBRX_DROOP);
            for (byte i = 0; i < params.FbRx_Droop_Cal.stg_freq_offset_kHz_list.size(); i++)
            {
               enh_internal_device_cal.droop_instr.stg_freq_offset_list_khz[i] = static_cast<unsigned int>(params.FbRx_Droop_Cal.stg_freq_offset_kHz_list_Lte15M[i]);
            }
        }
   }
   // dc instr init
   if (params.Internal_Cal_Type_Mask == ENH_INTERNAL_DEVICE_CAL_DC)
   {
   enh_internal_device_cal.dc_instr.num_Gain_States=static_cast<unsigned char>(params.FbRx_DC_Cal.Fb_Rx_Gain_State_List.size());
   enh_internal_device_cal.dc_instr.num_iq_samples=params.FbRx_DC_Cal.Num_IQ_Samples;

   memset(enh_internal_device_cal.dc_instr.fbrx_gain_state,0,sizeof(enh_internal_device_cal.dc_instr.fbrx_gain_state)/sizeof(unsigned char));

   //Get the FBRx Gain states
   for(unsigned short i=0; i < params.FbRx_DC_Cal.Fb_Rx_Gain_State_List.size(); ++i)
   {
      enh_internal_device_cal.dc_instr.fbrx_gain_state[i]=static_cast<unsigned char>(params.FbRx_DC_Cal.Fb_Rx_Gain_State_List[i]);
   }
   }
   // rsb instr init
   enh_internal_device_cal.rsb_instr.rsb_type = params.Rx_FbRx_RSB_Cal.Rsb_Type;
   enh_internal_device_cal.rsb_instr.rsb_cal_alg = params.Rx_FbRx_RSB_Cal.Signal_Source; ;
   enh_internal_device_cal.rsb_instr.offset_khz = params.Rx_FbRx_RSB_Cal.Waveform_Offset_in_kHz;
   enh_internal_device_cal.rsb_instr.num_gain_states = params.Rx_FbRx_RSB_Cal.Number_Of_Gain_States;
   enh_internal_device_cal.rsb_instr.carrier_id = params.Rx_FbRx_RSB_Cal.Carrier_To_Be_Calibrated;
   enh_internal_device_cal.rsb_instr.pcell_band_for_scell = params.Rx_FbRx_RSB_Cal.PCellBand;
   enh_internal_device_cal.rsb_instr.pcell_ref_chan_for_scell = params.Rx_FbRx_RSB_Cal.PCellRefChannel;

   enh_internal_device_cal.rsb_instr.iq_capture_type = params.Rx_FbRx_RSB_Cal.IQ_Capture_Type;
   enh_internal_device_cal.rsb_instr.iq_sample_size = params.Rx_FbRx_RSB_Cal.Num_IQ_Samples;

   //Check if rsb gain state list is empty
   if(enh_internal_device_cal.common_header.int_cal_type_mask==ENH_INTERNAL_DEVICE_CAL_RSB_BIT)
   {
      memset(enh_internal_device_cal.rsb_instr.gain_states,0,sizeof(enh_internal_device_cal.rsb_instr.gain_states)/sizeof(unsigned char));
      //Get the rsb Gain states
      for(unsigned short i=0; i< params.Rx_FbRx_RSB_Cal.Rx_Gain_State_List.size(); ++i)
      {
         enh_internal_device_cal.rsb_instr.gain_states[i]=static_cast<unsigned char>( params.Rx_FbRx_RSB_Cal.Rx_Gain_State_List[i]);
      }
   }
   //Find the instruction length based on type of FBRx cal
   switch(enh_internal_device_cal.common_header.int_cal_type_mask)
   {
   case ENH_INTERNAL_DEVICE_CAL_FBDROOP:
      enh_internal_device_cal.common_header.tot_instr_len = sizeof(fb_droop_instruction);
      break;
   case ENH_INTERNAL_DEVICE_CAL_DC: 
         //enh_internal_device_cal.common_header.tot_instr_len=7;
         enh_internal_device_cal.common_header.tot_instr_len = sizeof(fb_dc_instruction);
         break;
   case ENH_INTERNAL_DEVICE_CAL_RSB:
         enh_internal_device_cal.common_header.tot_instr_len= sizeof(rx_fbrx_rsb_instruction); //12;
         if(enh_internal_device_cal.rsb_instr.rsb_type == FTM_ENH_INTERNAL_DEVICE_CAL_RX_RSB /* FTM_ENH_INTERNAL_DEVICE_CAL_RX_RSB */)
         {
            enh_internal_device_cal.common_header.bw = (unsigned char) params.TxBandwidth;
         }
         break;
      default:
         throw RFCal_Helper_Exception("Invalid FBRx internal cal type mask.", __FILE__, __LINE__); // Not supported
   }

   //Total request length
   enh_internal_device_cal.common_header.req_len=enh_internal_device_cal.common_header.req_len+static_cast<unsigned short>(enh_internal_device_cal.common_header.tot_instr_len);
}

void RFCal_Helper_Enh_Internal_Device_Cal_Verify_rsb_cal(unsigned short dev_mask, const common_enh_internal_device_resp & enh_internal_device_resp, const XTT_Enh_Internal_Dev_Cal_Param params)
{
   int rsb_before_cal[FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_MAX];
   int rsb_after_cal[FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_MAX];

   unsigned char res_dev_mask = enh_internal_device_resp.rsp_individual.rsb_rsp_payload.device_mask;

   for (int i=0; i<FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_MAX; i++)
   {
      rsb_before_cal[i] = enh_internal_device_resp.rsp_individual.rsb_rsp_payload.rsb_res[i][0].rsb_before_cal;
      rsb_after_cal[i] = enh_internal_device_resp.rsp_individual.rsb_rsp_payload.rsb_res[i][0].rsb_after_cal;
   }
   // Rsb cal limit check.
   for (int i=0; i< FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_MAX; i++)
   {
      if( (res_dev_mask & (1<<i) ) && ( rsb_after_cal[i]< params.Rx_FbRx_RSB_Cal.Post_Cal_Rsb_Limit) )
      {
         cout << "QLIB_FTM_ENH_INTERNAL_DEVICE_CAL: rsb cal out of limit" <<  __FILE__ << ", "  << __LINE__ << endl;
      }
      if( (res_dev_mask & (1<<i) ) && (( rsb_after_cal[i]==99) && ( rsb_before_cal[i]<params.Rx_FbRx_RSB_Cal.Pre_Cal_Rsb_Limit)))
      {
         cout << "QLIB_FTM_ENH_INTERNAL_DEVICE_CAL: pre-rsb cal out of limit" <<  __FILE__ << ", "  << __LINE__ << endl;
      }
   }
}

unsigned char RFCal_Helper_Enh_Internal_Device_TxOverride(HANDLE hPhone, const XTT_Enh_Internal_Dev_Cal_Param & params, const common_enh_internal_device_cal & enh_internal_device_cal, unsigned char bSetMode )
{
   FTM_Mode_Id_Enum modeID, drxModeID;
   unsigned char status;

   switch(enh_internal_device_cal.common_header.tech)
   {
      case 0:
         modeID = FTM_MODE_ID_CDMA_1X;
      drxModeID = FTM_MODE_ID_CDMA_1X_RX1;
         break;
      case 1:
         modeID = FTM_MODE_ID_WCDMA;
      drxModeID = FTM_MODE_WCDMA_RX1;
         break;
      case 3:
         modeID = FTM_MODE_ID_LTE;
      drxModeID = FTM_MODE_LTE_RX1;
         break;
      case 4:
         modeID = FTM_MODE_ID_TDSCDMA;
      drxModeID = FTM_MODE_ID_TDSCDMA_RX1;
         break;
      default:
         throw RFCal_Helper_Exception( "Invalid tech.", __FILE__, __LINE__);
   }

   if(!RFCal_Helper_Change_FTM_Mode(hPhone)) throw RFCal_Helper_Exception("Failed in FTM mode change",  __FILE__, __LINE__ );
   if (bSetMode)  // We can skip when the 2nd Droop cal
   {
   status = QLIB_FTM_SET_CALIBRATION_STATE( hPhone , true );
   }
   //Set Mode ID
   status = QLIB_FTM_SET_MODE_ID( hPhone, static_cast<short>( modeID ));

   //Set RF Mode  // We can skip when the 2nd Droop cal
   if (bSetMode)
   {
   if (enh_internal_device_cal.rsb_instr.carrier_id != 0)
   {
      status=QLIB_FTM_SET_MODE( hPhone,(short) params.Rx_FbRx_RSB_Cal.PCellModeForSCell);
   }
   else
   {
      status=QLIB_FTM_SET_MODE( hPhone,(short) params.RF_Mode);
   }
   }
   //If technology is LTE
   if(enh_internal_device_cal.common_header.tech==3)
   {
      unsigned char BW = params.TxBandwidth;
      
      if (enh_internal_device_cal.common_header.int_cal_type_mask == ENH_INTERNAL_DEVICE_CAL_FBDROOP)
      {
          BW = 3;
        
      }
      //Set TX Bandwidth
      status=QLIB_FTM_LTE_SET_TX_BANDWIDTH(hPhone,static_cast<unsigned char>(params.TxBandwidth));

      //Set RX Bandwidth
      status=QLIB_FTM_LTE_SET_RX_BANDWIDTH(hPhone,static_cast<unsigned char>(params.TxBandwidth));
   }

   // TDS secondary chain set before channel tune
   if((enh_internal_device_cal.common_header.dev_mask & (1 << FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_DRX1 )) && (enh_internal_device_cal.common_header.tech==4) )
   {
      status = QLIB_FTM_SET_SECONDARY_CHAIN(hPhone, 1);//drx
   }

   //Set UL channel
   if (enh_internal_device_cal.rsb_instr.carrier_id != 0)
   {
      status=QLIB_FTM_SET_CHAN( hPhone, static_cast<unsigned short>(params.Rx_FbRx_RSB_Cal.PCellRefChannel));
   }
   else
   {
      status=QLIB_FTM_SET_CHAN( hPhone, params.iChannel );
   }
   // secondary chain set Except TDS
   if((enh_internal_device_cal.common_header.dev_mask & (1 << FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_DRX1 )) && (enh_internal_device_cal.common_header.tech !=4) ) 
   {
      status = QLIB_FTM_SET_SECONDARY_CHAIN(hPhone, 1);//drx
   }

   //Set TX waveform
   if(enh_internal_device_cal.common_header.tech==3)//If tech is LTE
   {
      unsigned char numRBsPUSCH = 50;
      unsigned char startRB = 0;

      if (enh_internal_device_cal.common_header.int_cal_type_mask == ENH_INTERNAL_DEVICE_CAL_FBDROOP)
      {
         numRBsPUSCH = 1;
         switch (params.TxBandwidth)
         {
         case 3:
             startRB = 24;
             break;
         case 4:
             startRB = 37;
             break;
         case 5:
             startRB = 49;
             break;
         default:
             break;
         }
      }
      status = (QLIB_FTM_LTE_SET_TX_WAVEFORM(hPhone,1,numRBsPUSCH,0,startRB) != 0);//Set LTE PUSCH waveform
   }

   if(enh_internal_device_cal.common_header.tech==4)//If tech is TDSCDMA
   {
      QLIB_FTM_CDMA_CW_WAVEFORM(hPhone,0);//Set TDSCDMA waveform
   }

   //Set Tx on

   if ( (enh_internal_device_cal.common_header.int_cal_type_mask != ENH_INTERNAL_DEVICE_CAL_RSB_BIT)  ||
         ( (enh_internal_device_cal.common_header.int_cal_type_mask == ENH_INTERNAL_DEVICE_CAL_RSB_BIT)
           && (enh_internal_device_cal.rsb_instr.rsb_type == FTM_ENH_INTERNAL_DEVICE_CAL_FB_RSB)) )
   {
      status= QLIB_FTM_SET_TX_ON( hPhone);

      if(enh_internal_device_cal.common_header.tech==4)//If tech is TDSCDMA
      {
         QLIB_FTM_TDSCDMA_SWITCH_TX_WAVEFORM(hPhone,1);// 1 for DPD Waveform
      }

      //Set PA Bias
      status=  QLIB_FTM_SET_SMPS_PA_BIAS_VAL(hPhone,static_cast<unsigned short>(params.iBiasValue));

      //Set PA state
      unsigned short error_code;
      status = QLIB_FTM_SET_PA_STATE( hPhone, static_cast<unsigned char>(params.iPAstate),  &error_code);

      //Set RGI
      if(enh_internal_device_cal.common_header.tech==3)
      {
         status=QLIB_FTM_SET_TX_GAIN_INDEX(hPhone,static_cast<unsigned short>(params.TxGainIndex));
      }
   }
   if (enh_internal_device_cal.common_header.int_cal_type_mask == ENH_INTERNAL_DEVICE_CAL_RSB_BIT)
   {
      QLIB_FTM_SET_LNA_RANGE(hPhone, params.Rx_FbRx_RSB_Cal.Rx_Gain_State_List[0]);

      if(enh_internal_device_cal.rsb_instr.carrier_id == 1) // Scell 1
      {
        short scell_mode_id = FTM_MODE_ID_LTE_C3; //hard-code for now

        //Set RF Mode
         status = QLIB_FTM_SET_MODE_ID( hPhone, scell_mode_id );
         status=QLIB_FTM_SET_MODE( hPhone,(short) params.Rx_FbRx_RSB_Cal.SCellMode );

         //Set RX Bandwidth
        status = QLIB_FTM_SET_MODE_ID( hPhone, static_cast<short>( scell_mode_id));
         status=QLIB_FTM_LTE_SET_RX_BANDWIDTH(hPhone,static_cast<unsigned char>(params.TxBandwidth));

         //Set UL channel
        status = QLIB_FTM_SET_MODE_ID( hPhone, static_cast<short>( scell_mode_id));
         status=QLIB_FTM_SET_CHAN( hPhone, params.iChannel );

        if(enh_internal_device_cal.common_header.dev_mask & (1<<FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_DRX2)) // New change we need to set secondary chain for SCEL DRx              
         {
            status = QLIB_FTM_SET_MODE_ID( hPhone, static_cast<short>( scell_mode_id ));
            status = QLIB_FTM_SET_SECONDARY_CHAIN(hPhone, 1);//drx
         }
        //Set LNA Range
         status = QLIB_FTM_SET_MODE_ID( hPhone, static_cast<short>( scell_mode_id ));
        status = QLIB_FTM_SET_LNA_RANGE(hPhone, params.Rx_FbRx_RSB_Cal.Rx_Gain_State_List[0]);
      }

      //  enh_int_device_cal->common_header.dev_mask = 3;
      if (enh_internal_device_cal.rsb_instr.rsb_cal_alg == FTM_ENH_INTERNAL_DEVICE_CAL_RSB_ALG_CALLBOX)
      {
         // double dl_offset = ((double) enh_int_device_cal->rsb_instr.offset_khz)/1000.0;
         // Set Band
         // Set Channel
         // Set Modulation CW
         // Set DL Level (-50.0);
         // Set DL state(true);
      }
   }

   return true;
}
unsigned char RFCal_Helper_Enh_Internal_Device_Cal_Verification(HANDLE hPhone,const XTT_Enh_Internal_Dev_Cal_Param & params, const common_enh_internal_device_cal & enh_internal_device_cal,
                                                                const common_enh_internal_device_resp & enh_internal_device_res)
{
   bool status = true;

   if(enh_internal_device_cal.common_header.int_cal_type_mask == ENH_INTERNAL_DEVICE_CAL_RSB_BIT)
   {
      RFCal_Helper_Enh_Internal_Device_Cal_Verify_rsb_cal(enh_internal_device_cal.common_header.dev_mask,  enh_internal_device_res, params);
   }
   return status;
}
unsigned char RFCal_Helper_Enh_Internal_Device_Cal_NVGeneration(HANDLE hPhone,const XTT_Enh_Internal_Dev_Cal_Param & params, const common_enh_internal_device_cal & enh_internal_device_cal,
                                                                common_enh_internal_device_resp & enh_internal_device_res)
{
   short iNVPath=0;
   stringstream ostr;
   switch(enh_internal_device_cal.common_header.int_cal_type_mask)
   {
    case ENH_INTERNAL_DEVICE_CAL_FBDROOP_BIT:
        break;
      case ENH_INTERNAL_DEVICE_CAL_DC_BIT:
      {
         ostringstream ostr;
         for(unsigned short i=0; i< params.FbRx_DC_Cal.Fb_Rx_Gain_State_List.size(); ++i)
         {
            ostr <<"FBRx NV Mode "<<enh_internal_device_res.rsp_individual.dc_resp.fbrx_mode_nv<<",  FBRx Gain State "<<params.FbRx_DC_Cal.Fb_Rx_Gain_State_List[i]<<",  DC_I = "<<enh_internal_device_res.rsp_individual.dc_resp.dcval[i][0]<<",  DC_Q = "<<enh_internal_device_res.rsp_individual.dc_resp.dcval[i][1]<<endl;
         }

         QMSL_DC_CAL_NV_RESULT DC_NV;
       memset((void *)&DC_NV,0,sizeof(DC_NV));
       DC_NV.DC_CAL_STRUCT.reserved = 1;
         QLIB_RFCAL_Generate_FBRX_DC_NV(  hPhone,
                                          params.NV_Mode,
                                          iNVPath,
                                          enh_internal_device_cal.dc_instr.num_Gain_States,
                                          enh_internal_device_res.rsp_common_header.tech,
                                          true,
                                          static_cast<void*>(&enh_internal_device_res.rsp_individual.dc_resp),
                                          static_cast<void*>(&DC_NV));
      }
      break;

      case ENH_INTERNAL_DEVICE_CAL_RSB_BIT:
         break;

      default:
         throw RFCal_Helper_Exception(  "Invalid FBRx internal cal type mask");
   }
   RFCal_Helper_AddMessageToQMSLLog(hPhone, ostr.str());

   return true;
}

void Test_RFCal_Enh_Internal_Device_Cal(HANDLE hPhone, int TECH, int BAND, unsigned char isSCell, int int_cal_type_mask, const string & XTTFilename, unsigned char bSimulationMode)
{
   stringstream ssTitle;
   ssTitle << "Test_RFCal_Enh_Internal_Device_Cal.  Tech: " << TECH << " Band: " << BAND << " IntCalTypeMask: " << int_cal_type_mask;
   RFCal_Helper_AddSectionTitleToQMSLLog(hPhone,ssTitle.str());

   int iWaitTimeForEnhIntDeviceCal = 200;

   // *******************************************************************************************
   // Read XTT Parameters
   // *******************************************************************************************
   XTT_Enh_Internal_Dev_Cal_Param params;
   RFCal_Helper_Enh_Internal_Device_Param_Assigment_From_XTT(hPhone,TECH, BAND, isSCell, int_cal_type_mask, XTTFilename, params);

   // *******************************************************************************************
   // Create enh internal device cal parameters from XTT parameters
   // *******************************************************************************************
   common_enh_internal_device_cal enh_internal_device_cal;
   RFCal_Helpder_Enh_Internal_Device_Request_Param_assignment(params,enh_internal_device_cal);

    // ************************************ //
    // Droop Cal 
    // ************************************ //
    common_enh_internal_device_cal enh_internal_device_cal_for_droop_LTE_20Mhz; // Request For 2nd Droop Cal
    common_enh_internal_device_resp enh_internal_device_res_for_droop_LTE_20Mhz; // Response For 2nd Droop Cal
    memset (&enh_internal_device_cal_for_droop_LTE_20Mhz, 0, sizeof (common_enh_internal_device_cal)); // Initialize 
    memset (&enh_internal_device_res_for_droop_LTE_20Mhz, 0, sizeof (common_enh_internal_device_resp)); // Initialize 
    QMSL_FBRX_DROOP_CAL_NV_RESULT FBRX_DROOP_NV;
    QMSL_RFNV_FBRX_DROOP_PER_BAND_NV_RESULT FBRX_DROOP_NV_V2;

    byte bLTE_BW10MHz = false;
    byte bLTE_BW20MHz = false;
    byte bLTE_BW15MHz = false;

    if (params.Internal_Cal_Type_Mask == ENH_INTERNAL_DEVICE_CAL_FBDROOP) // Droop cal
    {
        const unsigned char LTE_BW10MHZ = 13;
        const unsigned char LTE_BW15MHZ = 3;
        const unsigned char LTE_BW20MHZ = 2;

        bLTE_BW10MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW10MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(); 
        bLTE_BW20MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW20MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end();
        if (!bLTE_BW20MHz)
        {
            bLTE_BW15MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW15MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end();
        }
        if (bLTE_BW10MHz)
        {
            params.TxBandwidth = 3; // 10 MHz
        }
        else if (LTE_BW20MHZ)
        {
            params.TxBandwidth = 5; // 20 MHz
        }
        else if (bLTE_BW15MHz)
        {
            params.TxBandwidth = 4; // 15 MHz
        }

    }
   // *******************************************************************************************
   // Tx Output (Override)
   // *******************************************************************************************
   if (!bSimulationMode)
      RFCal_Helper_Enh_Internal_Device_TxOverride(hPhone,params,enh_internal_device_cal);

   Sleep(iWaitTimeForEnhIntDeviceCal);

   // *******************************************************************************************
   // Execute Enh Internal Device Cal
   // *******************************************************************************************
   unsigned char status;
   uint8 loop_gain = 1;
   unsigned char gain_states[NUM_RSB_GAIN_STATES];
   common_enh_internal_device_resp enh_internal_device_res;
    word tmpOriginalFlagValue = enh_internal_device_cal.common_header.flag;

    FTM_Mode_Id_Enum modeID, drxModeID;

    switch(enh_internal_device_cal.common_header.tech)
    {
    case 0:
        modeID = FTM_MODE_ID_CDMA_1X;
        drxModeID = FTM_MODE_ID_CDMA_1X_RX1;
        break;
    case 1:
        modeID = FTM_MODE_ID_WCDMA;
        drxModeID = FTM_MODE_WCDMA_RX1;
        break;
    case 3:
        modeID = FTM_MODE_ID_LTE;
        drxModeID = FTM_MODE_LTE_RX1;
        break;
    case 4:
        modeID = FTM_MODE_ID_TDSCDMA;
        drxModeID = FTM_MODE_ID_TDSCDMA_RX1;
        break;
    default:
        break;
    }

   // For RxRSB Cal, QLIB_FTM_ENH_INTERNAL_DEVICE_CAL is called for each of gain_state
   // For Other cal including FBRx_RSBCal, QLIB_FTM_ENH_INTERNAL_DEVICE_CAL is called only once.

   if( (enh_internal_device_cal.common_header.int_cal_type_mask == ENH_INTERNAL_DEVICE_CAL_RSB) &&
         (enh_internal_device_cal.rsb_instr.rsb_type == FTM_ENH_INTERNAL_DEVICE_CAL_RX_RSB))
   {
      loop_gain = enh_internal_device_cal.rsb_instr.num_gain_states;
      memcpy(gain_states,enh_internal_device_cal.rsb_instr.gain_states,sizeof(gain_states));
      enh_internal_device_cal.rsb_instr.num_gain_states =1; //if rx rsb cal, reset num_gain_states=1 as we need sync to box for each gain_state timeline
   }

   if (!bSimulationMode)
   {
      for (uint8 gain_index =0; gain_index <loop_gain; gain_index++)
      {
         if( (enh_internal_device_cal.rsb_instr.rsb_type == FTM_ENH_INTERNAL_DEVICE_CAL_RX_RSB)
               && (enh_internal_device_cal.common_header.int_cal_type_mask == ENH_INTERNAL_DEVICE_CAL_RSB))
         {
             if(enh_internal_device_cal.rsb_instr.carrier_id != 0) 
             {
                  status = QLIB_FTM_SET_MODE_ID( hPhone, static_cast<short>( FTM_MODE_ID_LTE_C3 ));
             } 
             else 
             {
                 status = QLIB_FTM_SET_MODE_ID( hPhone, static_cast<short>( modeID ) );
             }
             QLIB_FTM_SET_LNA_RANGE(hPhone,enh_internal_device_cal.rsb_instr.gain_states[gain_index]);
             
             if((enh_internal_device_cal.common_header.dev_mask & (1<<FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_DRX2)) // DRX       
             || (enh_internal_device_cal.common_header.dev_mask & (1<<FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_DRX1)))
            {
                // setup DRX LNA state
                 if(enh_internal_device_cal.rsb_instr.carrier_id != 0)  {
                    status = QLIB_FTM_SET_MODE_ID( hPhone, static_cast<short>( FTM_MODE_ID_LTE_C4 ));
                 } else {
                    status = QLIB_FTM_SET_MODE_ID( hPhone, static_cast<short>( drxModeID ) );
            }
            QLIB_FTM_SET_LNA_RANGE(hPhone,enh_internal_device_cal.rsb_instr.gain_states[gain_index]);
             }

            //m_pLM->TE_Set_DL_Level(static_cast<double>( params.Rx_FbRx_RSB_Cal.Power_List[gain_index])); // Callbox DL level setting.

            enh_internal_device_cal.rsb_instr.gain_states[0] = gain_states[gain_index]; // over-write gain_states for rx rsb cal
          
             // New Change. For RxRSB cal case, if the flag = 2, only assign flag =2 for last gain. otherwise set = 0
             if (tmpOriginalFlagValue == 2 && gain_index != (loop_gain - 1))
             {
                 enh_internal_device_cal.common_header.flag = 0;
             } else if (tmpOriginalFlagValue == 2 && gain_index == (loop_gain - 1))
             {
                 enh_internal_device_cal.common_header.flag = 2;
             }
          
         }

         status= QLIB_FTM_ENH_INTERNAL_DEVICE_CAL( hPhone, (void *)&enh_internal_device_cal, (void *)&enh_internal_device_res );


         // *******************************************************************************************
            // Droop cal only: 2nd Droop cal (execute only when the prior droop cal is 10 MHz and there is 15 or 20 Mhz)
            // *******************************************************************************************
            if (bLTE_BW10MHz && (bLTE_BW20MHz|| bLTE_BW15MHz) && params.Internal_Cal_Type_Mask == ENH_INTERNAL_DEVICE_CAL_FBDROOP) // Droop cal only
            {
                // *******************************************************************************************
                //  Tx Output (Override). Change Tx Bandwidth depending Bandwidth
                // *******************************************************************************************
                params.TxBandwidth = (bLTE_BW20MHz)? 5 : 4;

                RFCal_Helper_Enh_Internal_Device_TxOverride(hPhone,params,enh_internal_device_cal, false);
                Sleep(200);

                // *******************************************************************************************
                //  Parameter change for the 2nd droop cal
                // *******************************************************************************************
                memcpy (& enh_internal_device_cal_for_droop_LTE_20Mhz, &enh_internal_device_cal, sizeof( common_enh_internal_device_cal ));

                enh_internal_device_cal_for_droop_LTE_20Mhz.common_header.bw = (bLTE_BW20MHz)? 3 : 2 ; // LTE_BW20MHZ : LTE_BW15MHZ;
                enh_internal_device_cal_for_droop_LTE_20Mhz.common_header.ref_chan = params.FbRx_Droop_Cal.Chan_Lte_15M;
                enh_internal_device_cal_for_droop_LTE_20Mhz.droop_instr.num_Captures = static_cast<unsigned char>(params.FbRx_Droop_Cal.stg_freq_offset_kHz_list_Lte15M.size());
                memset(& enh_internal_device_cal_for_droop_LTE_20Mhz.droop_instr.stg_freq_offset_list_khz[0], 0, sizeof( long ) * NUM_MAX_STG_FREQ_OFFS_FBRX_DROOP);
        
                for (byte i = 0; i < params.FbRx_Droop_Cal.stg_freq_offset_kHz_list_Lte15M.size(); i++)
                {
                    enh_internal_device_cal_for_droop_LTE_20Mhz.droop_instr.stg_freq_offset_list_khz[i] = static_cast<unsigned int>(params.FbRx_Droop_Cal.stg_freq_offset_kHz_list_Lte15M[i]);
                }
                // *******************************************************************************************
                //  Execute Enh Internal Device Cal for 2nd Droop Cal
                // *******************************************************************************************

                status = QLIB_FTM_ENH_INTERNAL_DEVICE_CAL( hPhone, (void *)&enh_internal_device_cal_for_droop_LTE_20Mhz, (void *)&enh_internal_device_res_for_droop_LTE_20Mhz );

            }

            // *******************************************************************************************
         // Verify results
         // *******************************************************************************************
            if ( params.Internal_Cal_Type_Mask == ENH_INTERNAL_DEVICE_CAL_FBDROOP )
            {
                RFCal_Helper_Enh_Internal_Device_Cal_Verify_FBRx_Droop_Filter(hPhone,params,
                    enh_internal_device_cal, 
                    enh_internal_device_cal_for_droop_LTE_20Mhz, 
                    enh_internal_device_res, 
                    enh_internal_device_res_for_droop_LTE_20Mhz
                );
            }
            else
            {
         RFCal_Helper_Enh_Internal_Device_Cal_Verification(hPhone,params,enh_internal_device_cal, enh_internal_device_res);
      }
   }
    }

   //If tech is TDSCDMA,currently need to switch back the waveform to 0  and turn Tx off after running FTM_ENH_INTERNAL_DEVICE_CAL
   if (!bSimulationMode)
   {
      if (enh_internal_device_cal.common_header.int_cal_type_mask !=ENH_INTERNAL_DEVICE_CAL_RSB_BIT
            || enh_internal_device_cal.rsb_instr.rsb_type != FTM_ENH_INTERNAL_DEVICE_CAL_RX_RSB    )
      {
         if(enh_internal_device_cal.common_header.tech==4)
         {
            QLIB_FTM_TDSCDMA_SWITCH_TX_WAVEFORM(hPhone,0);
         }
         QLIB_FTM_SET_TX_OFF( hPhone);
      }
   }
   // *******************************************************************************************
   // NV Generation
   // *******************************************************************************************
   if (!bSimulationMode)
    {   
        if ( params.Internal_Cal_Type_Mask == ENH_INTERNAL_DEVICE_CAL_FBDROOP )
        {
            if (params.FbRx_Droop_Cal.NV_is_band_specific)
            {
                status = RFCal_Helper_Enh_Internal_Device_Cal_FBRx_Droop_PerBand_NVGeneration(hPhone,params,
                        enh_internal_device_cal, 
                        enh_internal_device_cal_for_droop_LTE_20Mhz, 
                        enh_internal_device_res, 
                        enh_internal_device_res_for_droop_LTE_20Mhz,
                        FBRX_DROOP_NV_V2
                );
            }
            else
            {
                status = RFCal_Helper_Enh_Internal_Device_Cal_FBRx_Droop_NVGeneration(hPhone,params,
                        enh_internal_device_cal, 
                        enh_internal_device_cal_for_droop_LTE_20Mhz, 
                        enh_internal_device_res, 
                        enh_internal_device_res_for_droop_LTE_20Mhz,
                        FBRX_DROOP_NV
                );
            }
        }
        else
        {
            status = RFCal_Helper_Enh_Internal_Device_Cal_NVGeneration(hPhone,params,enh_internal_device_cal, enh_internal_device_res);
        }
    }
   // *******************************************************************************************
    //  Create QCN (For now, FBRx DC Cal or DroopCal only)
   // *******************************************************************************************
    if ((enh_internal_device_cal.common_header.int_cal_type_mask == ENH_INTERNAL_DEVICE_CAL_DC_BIT)
      || (enh_internal_device_cal.common_header.int_cal_type_mask == ENH_INTERNAL_DEVICE_CAL_FBDROOP_BIT))
    {
        RFCal_Helper_WriteNVToQCN(hPhone,"c:\\temp\\FBRxNV.qcn");
    }
}
unsigned char RFCal_Helper_Enh_Internal_Device_Cal_FBRx_Droop_NVGeneration(HANDLE hPhone, const XTT_Enh_Internal_Dev_Cal_Param & params, const common_enh_internal_device_cal & req_10MHz, const common_enh_internal_device_cal & req_20MHz, 
     common_enh_internal_device_resp & resp_10MHz,   common_enh_internal_device_resp & resp_20Mhz,  QMSL_FBRX_DROOP_CAL_NV_RESULT & FBRX_DROOP_NV)
{

    byte bLTE_BW10MHz = false;
    byte bLTE_BW20MHz = false;
    byte bLTE_BW15MHz = false;
    short iNVPath = 0;
    const unsigned char LTE_BW10MHZ = 13;
    const unsigned char LTE_BW15MHZ = 3;
    const unsigned char LTE_BW20MHZ = 2;

    bLTE_BW10MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW10MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(); 
    bLTE_BW20MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW20MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end();
    if (!bLTE_BW20MHz)
    {
        bLTE_BW15MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW15MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end();
    }

    for (unsigned short i = 0; i < resp_10MHz.rsp_individual.droop_rsp_payload.num_filters ; ++i)
    {
        QLIB_RFCAL_Generate_FBRX_DROOP_NV(  hPhone,
            NV_MODE_MAX,
            iNVPath,
            true,
            static_cast<void*>(&resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i]),
            static_cast<void*>(&FBRX_DROOP_NV),
            static_cast<int>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].fbrx_tech_bw_type)
            );
    }
    if ((bLTE_BW20MHz || bLTE_BW15MHz)  && bLTE_BW10MHz)
    {
        for (unsigned short i = 0; i < resp_20Mhz.rsp_individual.droop_rsp_payload.num_filters ; ++i)
        {
            QLIB_RFCAL_Generate_FBRX_DROOP_NV(  hPhone,
            NV_MODE_MAX,
            iNVPath,
            true,
            static_cast<void*>(&(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i])),
            static_cast<void*>(&FBRX_DROOP_NV),
            static_cast<int>(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].fbrx_tech_bw_type)
            );
        }
    }
    
 
    return true;
}
unsigned char RFCal_Helper_Enh_Internal_Device_Cal_FBRx_Droop_PerBand_NVGeneration(HANDLE hPhone, const XTT_Enh_Internal_Dev_Cal_Param & params, const common_enh_internal_device_cal & req_10MHz, const common_enh_internal_device_cal & req_20MHz, 
     common_enh_internal_device_resp & resp_10MHz,   common_enh_internal_device_resp & resp_20Mhz, QMSL_RFNV_FBRX_DROOP_PER_BAND_NV_RESULT & FBRX_DROOP_NV_V2)
{

    byte bLTE_BW10MHz = false;
    byte bLTE_BW20MHz = false;
    byte bLTE_BW15MHz = false;
    short iNVPath = 0;
    const unsigned char LTE_BW10MHZ = 13;
    const unsigned char LTE_BW15MHZ = 3;
    const unsigned char LTE_BW20MHZ = 2;

    bLTE_BW10MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW10MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(); 
    bLTE_BW20MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW20MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end();
    if (!bLTE_BW20MHz)
    {
        bLTE_BW15MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW15MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end();
    }

    unsigned short elem_ctr = 0;
    QMSL_RFCAL_FBRX_Droop_data_type fbrx_droop_data_accum;
    for (unsigned short i = 0; i < params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.size(); ++i)
    {
        for (unsigned short j = 0; j < resp_10MHz.rsp_individual.droop_rsp_payload.num_filters; ++j)
        {
            if (static_cast<int>(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List[i]) == static_cast<int>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[j].fbrx_tech_bw_type))
            {
            memcpy((void*)&(fbrx_droop_data_accum.fbrx_droop_data_for_nv[elem_ctr]), 
                (void*)&(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[j]), 
                sizeof(ftm_fbrx_droop_result_data_type));
            elem_ctr++;
            }
        }
        for (unsigned short j = 0; j < resp_20Mhz.rsp_individual.droop_rsp_payload.num_filters; ++j)
        {
            if (static_cast<int>(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List[i]) == static_cast<int>(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[j].fbrx_tech_bw_type))
            {
            memcpy((void*)&(fbrx_droop_data_accum.fbrx_droop_data_for_nv[elem_ctr]), 
                (void*)&(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[j]), 
                sizeof(ftm_fbrx_droop_result_data_type));
            elem_ctr++;
            }
        }
    }

    fbrx_droop_data_accum.num_modes = static_cast<uint8>(elem_ctr);

    // New band-specific NVs
    QLIB_RFCAL_Generate_FBRX_DROOP_NV_V2(  hPhone,
            params.NV_Mode,
            iNVPath,
            true,
            static_cast<void*>(&fbrx_droop_data_accum),
            static_cast<void*>(&FBRX_DROOP_NV_V2)
            );
    
    return true;
}
unsigned char RFCal_Helper_Enh_Internal_Device_Cal_Verify_FBRx_Droop_Filter(HANDLE hPhone, const XTT_Enh_Internal_Dev_Cal_Param & params, const common_enh_internal_device_cal & req_10MHz, const common_enh_internal_device_cal & req_20MHz, 
    const common_enh_internal_device_resp & resp_10MHz,  const common_enh_internal_device_resp & resp_20Mhz)
{
    // *******************************************************************************************
    //  Droop cal verification
    // *******************************************************************************************
    RFCal_Helper_AddSectionTitleToQMSLLog(hPhone,"\tFBRx Droop Cal Results");
    const double PI = acos(-1.0);

    stringstream ostr;
    ostr.str("");
    int minPosFreq = INT_MAX;
    int maxNegFreq = INT_MIN;
    int minPosFreqInd = 0;
    int refRawTonePwr = 1;
    int maxNegFreqTonePwr= 1;
    double avgFreqOffsetPt = 0.0;
    double relativeTonePwrsDb[NUM_MAX_STG_FREQ_OFFS_FBRX_DROOP];
    double residual_droop_10M[NUM_MAX_STG_FREQ_OFFS_FBRX_DROOP];
    int minPosFreq_20M = INT_MAX;
    int maxNegFreq_20M = INT_MIN;
    int minPosFreqInd_20M = 0;
    int refRawTonePwr_20M = 1;
    int maxNegFreqTonePwr_20M= 1;
    double avgDcPwr = 0;
    double avgDcPwr_20M = 0;
    double avgFreqOffsetPt_20M = 0.0;
    double relativeTonePwrsDb_20M[NUM_MAX_STG_FREQ_OFFS_FBRX_DROOP];
    unsigned char coeffsSanityCheck = true;
    byte bLTE_BW10MHz = false;
    byte bLTE_BW20MHz = false;
    byte bLTE_BW15MHz = false;

    const unsigned char LTE_BW10MHZ = 13;
    const unsigned char LTE_BW15MHZ = 3;
    const unsigned char LTE_BW20MHZ = 2;

    bLTE_BW10MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW10MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(); 
    bLTE_BW20MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW20MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end();
    if (!bLTE_BW20MHz)
    {
        bLTE_BW15MHz = find(params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.begin(), params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end(), LTE_BW15MHZ) != params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.end();
    }

    for (unsigned short i = 0; i < static_cast<int>(req_10MHz.droop_instr.num_Captures); ++i)
    {
        if ( (static_cast<int>( req_10MHz.droop_instr.stg_freq_offset_list_khz[i])  > 0)  
            &&  (static_cast<int>( req_10MHz.droop_instr.stg_freq_offset_list_khz[i])  < minPosFreq) )
        {
            minPosFreq = static_cast<int>( req_10MHz.droop_instr.stg_freq_offset_list_khz[i]);
            minPosFreqInd = i;
            refRawTonePwr = static_cast<int> (resp_10MHz.rsp_individual.droop_rsp_payload.stg_raw_tone_pwrs[i]) ;
        }
        if ( (static_cast<int>( req_10MHz.droop_instr.stg_freq_offset_list_khz[i])  < 0)  
            &&  (static_cast<int>( req_10MHz.droop_instr.stg_freq_offset_list_khz[i])  > maxNegFreq) )
        {
            maxNegFreq = static_cast<int>( req_10MHz.droop_instr.stg_freq_offset_list_khz[i]);
            maxNegFreqTonePwr = static_cast<int> (resp_10MHz.rsp_individual.droop_rsp_payload.stg_raw_tone_pwrs[i]) ;
        }
    }

    if ((bLTE_BW20MHz || bLTE_BW15MHz)  && bLTE_BW10MHz)
    {
        for (unsigned short i = 0; i < static_cast<int>(req_10MHz.droop_instr.num_Captures); ++i)
        {
            if ( (static_cast<int>( req_20MHz.droop_instr.stg_freq_offset_list_khz[i])  > 0)  
            &&  (static_cast<int>( req_20MHz.droop_instr.stg_freq_offset_list_khz[i])  < minPosFreq_20M) )
            {
            minPosFreq_20M = static_cast<int>( req_20MHz.droop_instr.stg_freq_offset_list_khz[i]);
            minPosFreqInd_20M = i;
            refRawTonePwr_20M = static_cast<int> (resp_20Mhz.rsp_individual.droop_rsp_payload.stg_raw_tone_pwrs[i]) ;
            }
            if ( (static_cast<int>( req_20MHz.droop_instr.stg_freq_offset_list_khz[i])  < 0)  
            &&  (static_cast<int>( req_20MHz.droop_instr.stg_freq_offset_list_khz[i])  > maxNegFreq_20M) )
            {
            maxNegFreq_20M = static_cast<int>( req_20MHz.droop_instr.stg_freq_offset_list_khz[i]);
            maxNegFreqTonePwr_20M = static_cast<int> (resp_20Mhz.rsp_individual.droop_rsp_payload.stg_raw_tone_pwrs[i]) ;
            }
        }
    }
    avgDcPwr = static_cast<double>(maxNegFreqTonePwr+refRawTonePwr)/2;
    avgDcPwr_20M = static_cast<double>(maxNegFreqTonePwr_20M+refRawTonePwr_20M)/2;
    avgFreqOffsetPt = (maxNegFreq + minPosFreq)/2;
    avgFreqOffsetPt_20M = (maxNegFreq_20M + minPosFreq_20M)/2;

    for (unsigned short i = 0; i < static_cast<int>(req_10MHz.droop_instr.num_Captures); ++i)
    {
        relativeTonePwrsDb[i] =  10*log10( ( static_cast<double> (resp_10MHz.rsp_individual.droop_rsp_payload.stg_raw_tone_pwrs[i]) ) ) 
                                - 10*log10(  avgDcPwr  )  ;
    }
    for (unsigned short i = 0; i < static_cast<int>(req_20MHz.droop_instr.num_Captures); ++i)
    {
        relativeTonePwrsDb_20M[i] = 10*log10( ( static_cast<double> (resp_20Mhz.rsp_individual.droop_rsp_payload.stg_raw_tone_pwrs[i]) ) )
                                    -10*log10( avgDcPwr_20M)    ;
    }
    // *******************************************************************************************
    //  Droop cal verification : Relative Tone Check 
    // *******************************************************************************************

    for (unsigned short i = 0; i < static_cast<int>(req_10MHz.droop_instr.num_Captures); ++i)
    {
        ostr << left << setw(40) << setfill(' ') 
            << "Freq[" << static_cast<int>(i) << "]: " << static_cast<int>( req_10MHz.droop_instr.stg_freq_offset_list_khz[i] ) 
            << " kHz, Tone Power: " << static_cast<int> (resp_10MHz.rsp_individual.droop_rsp_payload.stg_raw_tone_pwrs[i]) 
            << " , Relative dB: " << static_cast<double> (relativeTonePwrsDb[i])
            << endl;

        if ( ( relativeTonePwrsDb[i] > params.FbRx_Droop_Cal.Tone_Pwr_Max_Lim_dB) ||
                ( relativeTonePwrsDb[i] < params.FbRx_Droop_Cal.Tone_Pwr_Min_Lim_dB) )
            {
            ostr << "QLIB_FTM_ENH_INTERNAL_DEVICE_CAL: Relative Tone Power for Freq "
                    << static_cast<int>( req_10MHz.droop_instr.stg_freq_offset_list_khz[i] )
                    << " = "
                    << static_cast<double>(relativeTonePwrsDb[i])
                    << " exceeds limit: ("
                    << params.FbRx_Droop_Cal.Tone_Pwr_Min_Lim_dB<< ", "
                    << params.FbRx_Droop_Cal.Tone_Pwr_Max_Lim_dB
                    << ")" << endl;
            coeffsSanityCheck = false;
            }

    }
    // *******************************************************************************************
    //  Droop cal verification : Relative Tone Check for 2nd Droop cal
    // *******************************************************************************************

    if ( (bLTE_BW20MHz || bLTE_BW15MHz)  && bLTE_BW10MHz)
    {
        for (unsigned short i = 0; i < static_cast<int>(req_20MHz.droop_instr.num_Captures); ++i)
        {
            ostr << left << setw(40) << setfill(' ') 
            << "Freq[" << static_cast<int>(i) << "]: " << static_cast<int>( req_20MHz.droop_instr.stg_freq_offset_list_khz[i] ) 
            << " kHz, Tone Power: " << static_cast<int> (resp_20Mhz.rsp_individual.droop_rsp_payload.stg_raw_tone_pwrs[i]) 
            << " , Relative dB: " << static_cast<double> (relativeTonePwrsDb_20M[i])
            << endl;

            if ( ( relativeTonePwrsDb_20M[i] > params.FbRx_Droop_Cal.Tone_Pwr_Max_Lim_dB) ||
            ( relativeTonePwrsDb_20M[i] < params.FbRx_Droop_Cal.Tone_Pwr_Min_Lim_dB) )
            {
          
            ostr << "QLIB_FTM_ENH_INTERNAL_DEVICE_CAL: Relative Tone Power for Freq "
                << static_cast<int>( req_20MHz.droop_instr.stg_freq_offset_list_khz[i] )
                << " = "
                << static_cast<double>(relativeTonePwrsDb_20M[i])
                << " exceeds limit: ("
                << params.FbRx_Droop_Cal.Tone_Pwr_Min_Lim_dB << ", "
                << params.FbRx_Droop_Cal.Tone_Pwr_Max_Lim_dB
                << ")" << endl;
          
            coeffsSanityCheck = false;
            }
        }
    }
  
    // *******************************************************************************************
    //  Droop cal verification : Coefficient Check 
    // *******************************************************************************************

    for(unsigned short i=0; i<resp_10MHz.rsp_individual.droop_rsp_payload.num_filters; ++i)
    {
        ostr << endl;
        ostr << left << setw(40) << setfill(' ')<< "FBRx Filter Index" << right << setw(20) << setfill(' ')<< static_cast<int>(i)  << endl;
        ostr << left << setw(40) << setfill(' ')<< "FBRx Gain State"  << right << setw(20) << setfill(' ')<< static_cast<int>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_gain_state) << endl;
        ostr << left << setw(40) << setfill(' ')<< "FBRx TechBW Type"  << right << setw(20) << setfill(' ') << static_cast<int>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].fbrx_tech_bw_type)  << endl;
        ostr << left << setw(40) << setfill(' ')<< "FBRx Filter Group Delay" << right << setw(20) << setfill(' ') << static_cast<int>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_group_delay)   <<endl;
        ostr << left << setw(40) << setfill(' ')<< "FBRx Filter Number of Taps" << right << setw(20) << setfill(' ') << static_cast<int>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_taps_num)  << endl;
        ostr << endl;
        ostr << "======================" << endl;
        ostr << endl;

        for (unsigned short jj=0; jj< resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_taps_num; ++jj)
        {
            short filter_coeff_i = static_cast<short>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj] & 0xFFFF);
            short filter_coeff_q = static_cast<short>((resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj] >> 16) & 0xFFFF);
            double filter_coeff_abs = sqrt(pow(static_cast<double>(filter_coeff_i),2.0)+pow(static_cast<double>(filter_coeff_q),2.0));
   
            if ( (static_cast<int>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj]) > params.FbRx_Droop_Cal.Coeff_Max_Lim) ||
                (static_cast<int>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj]) < params.FbRx_Droop_Cal.Coeff_Min_Lim) )
            {
           
            ostr << "QLIB_FTM_ENH_INTERNAL_DEVICE_CAL: Droop Coeff #"
                    << jj
                    << " = "
                    << static_cast<int>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj])
                    << " for tech/bw "
                    << static_cast<int>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].fbrx_tech_bw_type)
                    << " exceeds limit: ("
                    << params.FbRx_Droop_Cal.Coeff_Min_Lim << ", "
                    << params.FbRx_Droop_Cal.Coeff_Max_Lim
                    << ")" << endl;
            coeffsSanityCheck = false;
            }
        }
        ostr << endl;
        ostr << "======================" << endl;
    }

    // *******************************************************************************************
    //  Droop cal verification : Coefficient Check for 2nd Droop cal
    // *******************************************************************************************

    if ((bLTE_BW20MHz || bLTE_BW15MHz)  && bLTE_BW10MHz)
    {
        for(unsigned short i=0; i<resp_20Mhz.rsp_individual.droop_rsp_payload.num_filters; ++i)
        {
            ostr << endl;
            ostr << left << setw(40) << setfill(' ')<< "FBRx Filter Index" << right << setw(20) << setfill(' ')<< static_cast<int>(i)  << endl;
            ostr << left << setw(40) << setfill(' ')<< "FBRx Gain State"  << right << setw(20) << setfill(' ')<< static_cast<int>(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_gain_state) << endl;
            ostr << left << setw(40) << setfill(' ')<< "FBRx TechBW Type"  << right << setw(20) << setfill(' ') << static_cast<int>(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].fbrx_tech_bw_type)  << endl;
            ostr << left << setw(40) << setfill(' ')<< "FBRx Filter Group Delay" << right << setw(20) << setfill(' ') << static_cast<int>(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_group_delay)   <<endl;
            ostr << left << setw(40) << setfill(' ')<< "FBRx Filter Number of Taps" << right << setw(20) << setfill(' ') << static_cast<int>(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_taps_num)  << endl;
            ostr << endl;
            ostr << "======================" << endl;
            ostr << endl;

            for (unsigned short jj=0; jj<resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_taps_num; ++jj)
            {
                short filter_coeff_i = static_cast<short>(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj] & 0xFFFF);
                short filter_coeff_q = static_cast<short>((resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj] >> 16) & 0xFFFF);
                double filter_coeff_abs = sqrt(pow(static_cast<double>(filter_coeff_i),2.0)+pow(static_cast<double>(filter_coeff_q),2.0));

                if ( (static_cast<int>(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj]) > params.FbRx_Droop_Cal.Coeff_Max_Lim) ||
                    (static_cast<int>(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj]) < params.FbRx_Droop_Cal.Coeff_Min_Lim) )
                {
                           
                    ostr << "QLIB_FTM_ENH_INTERNAL_DEVICE_CAL: Droop Coeff #"
                        << jj
                        << " = "
                        << static_cast<int>(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj])
                        << " for tech/bw "
                        << static_cast<int>(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].fbrx_tech_bw_type)
                        << " exceeds limit: ("
                        << params.FbRx_Droop_Cal.Coeff_Max_Lim << ", "
                        << params.FbRx_Droop_Cal.Coeff_Min_Lim
                        << ")" << endl;
                    coeffsSanityCheck = false;
                }
            }
            ostr << endl;
            ostr << "======================" << endl;
        }
    }

    if ((params.FbRx_Droop_Cal.Verify_Cal == 1) && (params.FbRx_Droop_Cal.Verification_Method == 1) )
    {
        double eqTonePower[NUM_MAX_STG_FREQ_OFFS_FBRX_DROOP] = {0.0};
        double eqTonePower_dcSub[NUM_MAX_STG_FREQ_OFFS_FBRX_DROOP] = {0.0};
        int maxNegOffsTone = INT_MIN;
        int minPosOffsTone = INT_MAX;
        double maxNegOffsEqTonePwr  = 0.0;
        double minPosOffsEqTonePwr = 0.0;
        // find freq resp of 10M filter and log and limit check
        for(unsigned short i=0; i<resp_10MHz.rsp_individual.droop_rsp_payload.num_filters; ++i)
        {
            double samp_freq_mhz = 0.0;
            double band_edge_mhz = 0.0;
            bool valid_for_verif = false;
            for (unsigned short mm = 0; mm < params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.size(); ++mm)
            {
                if (params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List[mm] == resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].fbrx_tech_bw_type)
                {
                    samp_freq_mhz = params.FbRx_Droop_Cal.Sampl_Freq_List_Per_BW_Mode_MHz[mm];
                    band_edge_mhz = params.FbRx_Droop_Cal.Band_Edge_List_Per_BW_Mode_MHz[mm];
                    valid_for_verif = true;
                    break;
                }
            }

            // *******************************************************************************************
            //  Droop cal verification :  Residual Droop  
            // *******************************************************************************************

            if (valid_for_verif)
            {
                for (unsigned short kk = 0; kk < req_10MHz.droop_instr.num_Captures; ++kk)
                {
                    double sum_real = 0.0;
                    double sum_imag = 0.0;
                    for (unsigned short jj=0; jj<resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_taps_num; ++jj)
                    {
                        short filter_coeff_i = static_cast<short>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj] & 0xFFFF);
                        short filter_coeff_q = static_cast<short>((resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj] >> 16) & 0xFFFF);

                        double cos_val = cos(static_cast<double>(jj)*2*PI*static_cast<double>(req_10MHz.droop_instr.stg_freq_offset_list_khz[kk])*1e3/(samp_freq_mhz*1e6));
                        double sin_val =-sin(static_cast<double>(jj)*2*PI*static_cast<double>(req_10MHz.droop_instr.stg_freq_offset_list_khz[kk])*1e3/(samp_freq_mhz*1e6));

                        sum_real +=  ( static_cast<double>(filter_coeff_i) * cos_val ) - (static_cast<double>(filter_coeff_q) * sin_val);
                        sum_imag +=  ( static_cast<double>(filter_coeff_i) * sin_val ) + (static_cast<double>(filter_coeff_q) * cos_val);

                    }

                    eqTonePower[kk] = 10*log10(pow(sum_real,2.0) + pow(sum_imag,2.0));
                    if ( (req_10MHz.droop_instr.stg_freq_offset_list_khz[kk] < 0) &&
                        (req_10MHz.droop_instr.stg_freq_offset_list_khz[kk] > maxNegOffsTone) )
                    {
                        maxNegOffsTone = req_10MHz.droop_instr.stg_freq_offset_list_khz[kk];
                        maxNegOffsEqTonePwr = eqTonePower[kk];
                    }
                    if ( (req_10MHz.droop_instr.stg_freq_offset_list_khz[kk] > 0) &&
                        (req_10MHz.droop_instr.stg_freq_offset_list_khz[kk] < minPosOffsTone) )
                    {
                        minPosOffsTone = req_10MHz.droop_instr.stg_freq_offset_list_khz[kk];
                        minPosOffsEqTonePwr = eqTonePower[kk];
                    }
                }

                double avgDcEqToneOffset = static_cast<double>(maxNegOffsTone + minPosOffsTone)/2;
                double avgDcEqTonePwr    = (maxNegOffsEqTonePwr + minPosOffsEqTonePwr)/2;


                for (unsigned short kk = 0; kk < req_10MHz.droop_instr.num_Captures; ++kk)
                {
                    eqTonePower_dcSub[kk] = eqTonePower[kk] - avgDcEqTonePwr;
                    residual_droop_10M[kk] = eqTonePower_dcSub[kk] + relativeTonePwrsDb[kk];

                    if ( static_cast<double>(abs(req_10MHz.droop_instr.stg_freq_offset_list_khz[kk])) <= (band_edge_mhz*1000) )
                    {
                        //pDs_verifv2->addResultLimits( CDataID("Residual Droop dB"), residual_droop_10M[kk], 
                        //   m_enh_int_device_cal.Droop_Instr.residual_droop_min_lim_db,
                        //   m_enh_int_device_cal.Droop_Instr.residual_droop_max_lim_db);
                    }
                    else
                    {
                        //pDs_verifv2->addResultLimits( CDataID("Residual Droop dB"), residual_droop_10M[kk], 
                        //   m_enh_int_device_cal.Droop_Instr.residual_droop_oob_min_lim_db,
                        //   m_enh_int_device_cal.Droop_Instr.residual_droop_oob_max_lim_db);
                    }


                    if ( ( ( residual_droop_10M[kk] > params.FbRx_Droop_Cal.Residual_Droop_Max_Lim_dB) ||
                        ( residual_droop_10M[kk] < params.FbRx_Droop_Cal.Residual_Droop_Min_Lim_dB) ) &&
                        (static_cast<double>(abs(req_10MHz.droop_instr.stg_freq_offset_list_khz[kk])) <= (band_edge_mhz*1000)) )
                    {
                              
                        ostr << "QLIB_FTM_ENH_INTERNAL_DEVICE_CAL: Residual Droop for FBRx_TechBW_Type "
                            << static_cast<int>(resp_10MHz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].fbrx_tech_bw_type)
                            << " & Freq "
                            << static_cast<int>( req_10MHz.droop_instr.stg_freq_offset_list_khz[i] )
                            << " = "
                            << static_cast<double>(residual_droop_10M[kk])
                            << " exceeds limit: ("
                            << params.FbRx_Droop_Cal.Residual_Droop_Min_Lim_dB << ", "
                            << params.FbRx_Droop_Cal.Residual_Droop_Max_Lim_dB
                            << ")" << endl;

                        coeffsSanityCheck = false;
                    }
                }
            } // if (valid_for_verif)
        }
        // *******************************************************************************************
        //  Droop cal verification :  Residual Droop for 2nd Droop cal
        // *******************************************************************************************

        // find difference from STG for 10M and log and limit check
        if ((bLTE_BW20MHz || bLTE_BW15MHz)  && bLTE_BW10MHz)
        {
            double eqTonePower_20M[NUM_MAX_STG_FREQ_OFFS_FBRX_DROOP] = {0.0};
            double eqTonePower_dcSub_20M[NUM_MAX_STG_FREQ_OFFS_FBRX_DROOP] = {0.0};
            int maxNegOffsTone_20M = INT_MIN;
            int minPosOffsTone_20M = INT_MAX;
            double maxNegOffsEqTonePwr_20M  = 0.0;
            double minPosOffsEqTonePwr_20M = 0.0;
            // find freq resp of 10M filter and log and limit check
            for(unsigned short i=0; i<resp_20Mhz.rsp_individual.droop_rsp_payload.num_filters; ++i)
            {
                double samp_freq_mhz = 0.0;
                double band_edge_mhz = 0.0;
                bool valid_for_verif = false;
                for (unsigned short mm = 0; mm < params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List.size(); ++mm)
                {
                    if (params.FbRx_Droop_Cal.Lte_FBRxTech_Bw_List[mm] == resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].fbrx_tech_bw_type)
                    {
                        samp_freq_mhz = params.FbRx_Droop_Cal.Sampl_Freq_List_Per_BW_Mode_MHz[mm];
                        band_edge_mhz = params.FbRx_Droop_Cal.Band_Edge_List_Per_BW_Mode_MHz[mm];
                        valid_for_verif = true;
                        break;
                    }
                }
                if (valid_for_verif)
                {
                    for (unsigned short kk = 0; kk < req_20MHz.droop_instr.num_Captures; ++kk)
                    {
                        double sum_real = 0.0;
                        double sum_imag = 0.0;
                        for (unsigned short jj=0; jj<resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_taps_num; ++jj)
                        {
                            short filter_coeff_i = static_cast<short>(resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj] & 0xFFFF);
                            short filter_coeff_q = static_cast<short>((resp_20Mhz.rsp_individual.droop_rsp_payload.fbrx_droop_res[i].filter_coeffs[jj] >> 16) & 0xFFFF);

                            double cos_val = cos(static_cast<double>(jj)*2*PI*static_cast<double>(req_20MHz.droop_instr.stg_freq_offset_list_khz[kk])*1e3/(samp_freq_mhz*1e6));
                            double sin_val =-sin(static_cast<double>(jj)*2*PI*static_cast<double>(req_20MHz.droop_instr.stg_freq_offset_list_khz[kk])*1e3/(samp_freq_mhz*1e6));

                            sum_real +=  ( static_cast<double>( filter_coeff_i ) * cos_val ) - (static_cast<double>(filter_coeff_q) * sin_val);
                            sum_imag +=  ( static_cast<double>( filter_coeff_i ) * sin_val ) + (static_cast<double>(filter_coeff_q) * cos_val);

                        }

                        eqTonePower_20M[kk] = 10*log10(pow(sum_real,2.0) + pow(sum_imag,2.0));
                        if ( (req_20MHz.droop_instr.stg_freq_offset_list_khz[kk] < 0) &&
                            (req_20MHz.droop_instr.stg_freq_offset_list_khz[kk] > maxNegOffsTone_20M) )
                        {
                            maxNegOffsTone_20M = req_20MHz.droop_instr.stg_freq_offset_list_khz[kk];
                            maxNegOffsEqTonePwr_20M = eqTonePower_20M[kk];
                        }
                        if ( (req_20MHz.droop_instr.stg_freq_offset_list_khz[kk] > 0) &&
                            (req_20MHz.droop_instr.stg_freq_offset_list_khz[kk] < minPosOffsTone_20M) )
                        {
                            minPosOffsTone_20M = req_20MHz.droop_instr.stg_freq_offset_list_khz[kk];
                            minPosOffsEqTonePwr_20M = eqTonePower_20M[kk];
                        }
                    }

                    double avgDcEqToneOffset = static_cast<double>(maxNegOffsTone_20M + minPosOffsTone_20M)/2;
                    double avgDcEqTonePwr    = (maxNegOffsEqTonePwr_20M + minPosOffsEqTonePwr_20M)/2;

                    for (unsigned short kk = 0; kk < req_20MHz.droop_instr.num_Captures; ++kk)
                    {
                        eqTonePower_dcSub_20M[kk] = eqTonePower_20M[kk] - avgDcEqTonePwr;
                        residual_droop_10M[kk] = eqTonePower_dcSub_20M[kk] + relativeTonePwrsDb_20M[kk];

                        if ( static_cast<double>(abs(req_20MHz.droop_instr.stg_freq_offset_list_khz[kk])) <= (band_edge_mhz*1000) )
                        {
                            //pDs_verifv2->addResultLimits( CDataID("Residual Droop dB"), residual_droop_10M[kk], 
                            //m_enh_int_device_cal.Droop_Instr.residual_droop_min_lim_db,
                            //m_enh_int_device_cal.Droop_Instr.residual_droop_max_lim_db);
                        }
                        else
                        {
                            //pDs_verifv2->addResultLimits( CDataID("Residual Droop dB"), residual_droop_10M[kk], 
                            //m_enh_int_device_cal.Droop_Instr.residual_droop_oob_min_lim_db,
                            //m_enh_int_device_cal.Droop_Instr.residual_droop_oob_max_lim_db);
                        }

                        if ( ( ( residual_droop_10M[kk] > params.FbRx_Droop_Cal.Residual_Droop_Max_Lim_dB) ||
                            ( residual_droop_10M[kk] < params.FbRx_Droop_Cal.Residual_Droop_Min_Lim_dB) ) &&
                            (static_cast<double>(abs(req_20MHz.droop_instr.stg_freq_offset_list_khz[kk])) <= (band_edge_mhz*1000)) )
                        {
                       
                            ostr << "QLIB_FTM_ENH_INTERNAL_DEVICE_CAL: Residual Droop for Freq "
                            << static_cast<int>( req_20MHz.droop_instr.stg_freq_offset_list_khz[i] )
                            << " = "
                            << static_cast<double>(residual_droop_10M[kk])
                            << " exceeds limit: ("
                            <<  params.FbRx_Droop_Cal.Residual_Droop_Min_Lim_dB << ", "
                            << params.FbRx_Droop_Cal.Residual_Droop_Max_Lim_dB
                            << ")" << endl;
                            coeffsSanityCheck = false;
                        }

                    }
                } //  (valid_for_verif)
            }
        }
    }
    // Output the error message to QMSL log
    if (!coeffsSanityCheck)
    {
        RFCal_Helper_AddMessageToQMSLLog(hPhone,ostr.str());
    }
    return coeffsSanityCheck;
}
unsigned char Test_FB_Droop_Cal_Enh_Internal_Device_Cal()
{
   HANDLE hPhone = g_hResourceContext;
   string XTTFilename = "C:\\Program Files (x86)\\Qualcomm\\QDART\\XTT\\SubSysRFCalibration\\Multimodes\\WTR3925\\MDM9x35_WTR3925_2p0_SSKU_QFEs_ET__MTP__C_G_W_L__Xvc__RFCal1.0.xtt";
   unsigned char bSimulrationMode = false;
    string stQMSLLog = "c:\\temp\\QLIBDemo_Enh_Internal_Device_FB_Droop_Cal_Demo.txt";
    RFCal::Helper::Log::StartQMSLLogging(hPhone,stQMSLLog);
    RFCal_Helper_AddSectionTitleToQMSLLog(hPhone,"\tStart Enh Internal Device Cal Demo");
    
   try
   {
      int int_cal_type_mask;
      int tech, band = 1;
      unsigned char isSCell = false;

      // **************************************** //
      //        FBRx_Droop Cal Cal Demo
      // **************************************** //
      int_cal_type_mask  = 0; // FBRx_DCCal
      tech = 3;
      band = 1; // WCDMA
      Test_RFCal_Enh_Internal_Device_Cal(hPhone,tech,band, isSCell, int_cal_type_mask, XTTFilename, bSimulrationMode);
   }
   catch (RFCal_Helper_Exception & e)
   {
      stringstream ss;
      ss << "Exception caught: " << e.what();
      RFCal_Helper_AddMessageToQMSLLog(hPhone,ss.str());
   }
   catch (...)
   {
      RFCal_Helper_AddMessageToQMSLLog(hPhone,"Unknown Exception caught." );
   }
   QLIB_StopLogging(hPhone);
   return true;
}
unsigned char Test_FBDC_Cal_Enh_Internal_Device_Cal()
{
   HANDLE hPhone = g_hResourceContext;
   string XTTFilename = "C:\\Program Files (x86)\\Qualcomm\\QDART\\XTT\\SubSysRFCalibration\\Multimodes\\WTR3925\\MDM9x35_WTR3925_2p0_SSKU_QFEs_ET__MTP__C_G_W_L__Xvc__RFCal1.0.xtt";
   unsigned char bSimulrationMode = false;
    string stQMSLLog = "c:\\temp\\QLIBDemo_Enh_Internal_Device_Cal_Demo.txt";
    RFCal::Helper::Log::StartQMSLLogging(hPhone,stQMSLLog);
    RFCal_Helper_AddSectionTitleToQMSLLog(hPhone,"\tStart Enh Internal Device Cal Demo");
    
   try
   {
      int int_cal_type_mask;
      int tech, band = 1;
      unsigned char isSCell = false;

      // **************************************** //
      //        FBRx_DC Cal Cal Demo
      // **************************************** //
      int_cal_type_mask  = 3; // FBRx_DCCal
      tech = 1;
      band = 1; // WCDMA
      Test_RFCal_Enh_Internal_Device_Cal(hPhone,tech,band, isSCell, int_cal_type_mask, XTTFilename, bSimulrationMode);

      tech = 3;
      band = 1; // LTE
      int_cal_type_mask  = 3;
      Test_RFCal_Enh_Internal_Device_Cal(hPhone,tech,band, isSCell, int_cal_type_mask, XTTFilename, bSimulrationMode);
   }
   catch (RFCal_Helper_Exception & e)
   {
      stringstream ss;
      ss << "Exception caught: " << e.what();
      RFCal_Helper_AddMessageToQMSLLog(hPhone,ss.str());
   }
   catch (...)
   {
      RFCal_Helper_AddMessageToQMSLLog(hPhone,"Unknown Exception caught." );
   }
   QLIB_StopLogging(hPhone);
   return true;
}

unsigned char Test_RxRSB_Cal_Enh_Internal_Device_Cal()
{
   HANDLE hPhone = g_hResourceContext;
   string XTTFilename = "C:\\Program Files (x86)\\Qualcomm\\QDART\\XTT\\SubSysRFCalibration\\Multimodes\\WTR3925\\MDM9x35_WTR3925_2p0_SSKU_QFEs_ET__MTP__C_G_W_L__Xvc__RFCal1.0.xtt";

   unsigned char bSimulrationMode = false;
   RFCal::Helper::Log::StartQMSLLogging(hPhone,"c:\\temp\\QLIBDemo_Enh_Internal_Device_Cal_RxRSB_Cal_Demo.txt");
   RFCal_Helper_AddSectionTitleToQMSLLog(hPhone,"\tStart Rx RSB Cal - Enh Internal Device Cal Demo");
   try
   {
      int int_cal_type_mask;
      int tech, band = 1;
      unsigned char isSCell = false;

      // **************************************** //
      //        Rx RSB Cal Demo
      // **************************************** //
      int_cal_type_mask = 4; // Rx_Rsb_Cal
      tech = 1;
      band = 1; // WCDMA
      Test_RFCal_Enh_Internal_Device_Cal(hPhone,tech,band, isSCell,  int_cal_type_mask, XTTFilename, bSimulrationMode);

      tech = 3;
      band = 1; // LTE
      int_cal_type_mask = 4; // Rx_Rsb_Cal
      Test_RFCal_Enh_Internal_Device_Cal(hPhone,tech,band, isSCell, int_cal_type_mask, XTTFilename, bSimulrationMode);

      tech = 3;
      band = 1; // LTE
      int_cal_type_mask = 4; // Rx_Rsb_Cal SCELL
      isSCell = true;
      Test_RFCal_Enh_Internal_Device_Cal(hPhone,tech,band, isSCell, int_cal_type_mask, XTTFilename, bSimulrationMode);
   }
   catch (RFCal_Helper_Exception & e)
   {
      stringstream ss;
      ss << "Exception caught: " << e.what();
      RFCal_Helper_AddMessageToQMSLLog(hPhone,ss.str());
   }
   catch (...)
   {
      RFCal_Helper_AddMessageToQMSLLog(hPhone,"Unknown Exception caught." );
   }
   QLIB_StopLogging(hPhone);
   return true;
}

unsigned char RFCal_Helper_CalculateFrequencyFromULChan(int TECH, int band, int ULChannel, double & ULFrequency, double & DLFrequency)
{
   word BandClass;
   switch (TECH)
   {
      case 0:
         BandClass = HDR_BAND_ENUM_START + band;
         break;
      case 1:
         BandClass = WCDMA_BAND_ENUM_START + band - 1;
         break;
      case 2:
         switch (band)
         {
         case 8: //850
            BandClass = BH_GSM_BAND_850;
         break;
         case 9: //PGSM
            BandClass = BH_GSM_BAND_P_GSM;
            break;
         case 10: //EGSM
            BandClass = BH_GSM_BAND_E_GSM;
            break;
         case 13: //1800 DCS
            BandClass = BH_GSM_BAND_DCS;
            break;
         case 14: //1900 PCS
            BandClass = BH_GSM_BAND_PCS;
            break;
         default:
            BandClass = GSM_BAND_ENUM_START + band; // Unsupported
            break;
         }
      case 3:
         BandClass = LTE_BAND_ENUM_START + band - 1;
         break;
      case 4:
         BandClass = TDSCDMA_BAND_ENUM_START + band - 33;
         break;
      default:
         return false;
         break;
   }
   ULFrequency = QLIB_Band_Helper_CalculateULFrequency(BandClass,ULChannel);
   int DLChannel = RFCal_Helper_ConvertULchtoDLch(TECH,band,ULChannel);
   DLFrequency = QLIB_Band_Helper_CalculateDLFrequency(BandClass,DLChannel);

   return true;
}
unsigned char Test_RFCal_ESC_ET()
{
   unsigned char bMainLoop = true;
   unsigned char bContinue = true;     // Variable for menu
   unsigned char bSubMenuContinue = true;    // Variable for menu

   int command = 0;
   unsigned char bSimulationMode = true;
   do // bMainLoop
   {
      do // bContinue
      {
         // Print options
         printf("\n\n\nESC ET cal Menu ======================================\n\n");
         printf("------------------------------------------------------------\n\n");
         printf("[Input] Requires the following XMLs\n");
         printf("\t(1) Params XML.\t\t(e.g., WTR1625_NAEU_CA_ESC_Params.xml)\n");
         printf("\t(2) Char XML.\t\t(e.g., WTR1625_NAEU_CA_ESC_Char.xml)\n");
         printf("\t(3) PA_PARAM XML.\t(e.g., RFCAL_PA_PARAM.xml:)\n");
         printf("\t(4) Result XML.\t\t(C:\\temp\\result.xml) (Optional)\n");
         printf("\t** Rename the QSPR generated XML to C:\\temp\\result.xml. \n");
         printf("\t** With the result XML, this demo code can generate the same output.\n\n");
         printf("\t(5) Ref Cal data XML.\t(C:\\temp\\test_ref_cal_data.xml)\n");
         printf("\t** Ref Cal data XML is required for Tx Internal Cal.\n\n");
         printf("(1)-(3) XMLs in \n<QDART>\\XTT\\SubSysRFCalibration\\Multimodes\\WTR1625\\RFCALInput\\ \n");
         printf("OR please copy XML from AMSS folder\n\n");

         printf("------------------------------------------------------------\n");

         printf("[Output]\n");
         printf("\t(1) QMSL Log.\n");
         printf("\t(2) QCN file.\n");
         printf("\t(3) CSV files.\n");
         printf("** All the outputs is saved at c:\\temp\\ \n");
         printf("------------------------------------------------------------\n\n");

         printf("[Menu]\n");
         printf("\n 0 = LTE B1 ET-EPT\t\t\t(Use WTR3925_2P0_SSKU_QFES_ET_NF604_ESC_Params.xml)");
         printf("\n 1 = LTE B2 ET Inter CA\t\t(Use WTR1625_NAEU_CA_ESC_Params.xml)");
         printf("\n 2 = LTE B25 ET Intra CA\t(Use WTR1625_NAEU_CA_ESC_Params.xml)");
         printf("\n 3 = LTE B38 (TDD) APT\t\t(Use WTR1625_NAEU_CA_ESC_Params.xml)");
         printf("\n 4 = LTE B41A (TDD) EPT \t(Use WTR3925_2p0_SSKU_QFEs_ET_ESC_Params.xml) ");
         printf("\n 5 = LTE BC1 EPT\t\t(Use WTR1605_CHILE_SxDSDA_V2_ESC_params_X1.xml)");
         printf("\n 6 = LTE BC1 QFE ET-EPT\t\t(Use WTR1625_CMCC_QFE_ESC_Params.xml)");
         printf("\n 7 = WCDMA B1 ET\t\t(Use WTR1625_NAEU_CA_ESC_Params.xml)");
         printf("\n 8 = WCDMA B1 QFE EPT\t\t(Use WTR1605_CHILE_SxDSDA_V2_ESC_params_X1.xml)");
         printf("\n 9 = WCDMA B1 ET-EPT\t(Use WTR3925_2p0_SSKU_QFEs_ET_ESC_Params.xml)");
         printf("\n A = CDMA BC1 ET\t\t(Use WTR1625_NAEU_CA_ESC_Params.xml) ");
         printf("\n B = CDMA BC1 EPT\t\t(Use WTR1605_CHILE_SxDSDA_V2_ESC_params_X1.xml)");
         printf("\n C = CDMA BC1 EPT-ET\t\t(Use WTR1625_CMCC_QFE_ESC_Params.xml) ");
         printf("\n D = TDSCDMA B40 APT\t\t(Use WTR1625_APAC_CA_ESC_Params.xml)");
         printf("\n E = LTE B1 Ref Cal\t\t(Use WTR1625_NAEU_CA_REF_CAL_ESC_Params.xml)");
         printf("\n F = WCDMA B1 Ref Cal\t\t(Use WTR1625_NAEU_CA_REF_CAL_ESC_Params.xml)");
         printf("\n G = LTE B1 Tx Internal Cal\t(Use WTR1625_NAEU_CA_INT_CAL_ESC_Params.xml)");
         printf("\n H = WCDMA B1 Tx Internal Cal\t(Use WTR1625_NAEU_CA_INT_CAL_ESC_Params.xml)");
         printf("\n I = LTE B1 FBRx Cal\t(Use WTR3925_2p0_SSKU_QFEs_ET_ESC_Params.xml)");
         printf("\n J = WCDMA B1 FBRx Cal\t(Use WTR3925_2p0_SSKU_QFEs_ET_ESC_Params.xml)");
         printf("\n K = LTE B1 ET_EPT Cal\t\t(Use WTR3925_2p0_SSKU_QFEs_ET_ESC_Params.xml)");
         printf("\n L = WCDMA B1 ET_EPT Cal\t(Use WTR3925_2p0_SSKU_QFEs_ET_ESC_Params.xml)");
         printf("\n M = TDSCDMA B34 EPT Cal\t(Use WTR1605_CHILE_SxDSDA_V2_ESC_params_X1.xml)");
         printf("\n N = Multi thread DUT cal (Demo. LTE B1)\n\t\t\t\t(Use WTR1625_NAEU_CA_ESC_Params.xml)");
         printf("\n This version also supports PIN Cal / Serial DRx cal / TDS HiLin Cal ");
         printf("\n Q = QUIT. Back to the main menu");

         printf("\n\n Please enter the key (0 - 9 / A - E  or Q)\n");

         // Get a key
         char cKey = toupper( _getch() );

         printf("\n");
         if (cKey >= '0' && cKey <='9')
         {
            command = static_cast <int>(cKey - '0');
            bContinue = false;
         }
         else if (cKey >= 'a' && cKey <='n')
         {
            command = static_cast <int>(cKey - 'a') + 10;
            bContinue = false;
         }
         else if (cKey >= 'A' && cKey <='N')
         {
            command = static_cast <int>(cKey - 'A') + 10;
            bContinue = false;
         }
         else if (cKey == 'q' || cKey == 'Q')
         {
            printf("\n Go back to main menu");
            return true;
         }
         do
         {
            if(!bContinue)
            {
               printf("%c\n",cKey);
               printf("\n======================================\n\n");
               printf("Choose (1) Simulation mode or (2) real DUT mode?\n\n");
               printf("\n 1 Simulation Mode. \t (No RFCal FTM command sent to a DUT. No need DUT.)");
               printf("\n 2 Real mode. \t\t (RFCal FTM command sent to a DUT. Need a DUT) ");
               printf("\n Q Back to the previous menu");

               printf("\n\n Please enter the key (1 - 2 or Q)\n");
               char cKey = toupper( _getch() );

               printf("\n");
               switch (cKey)
               {
                  case '1':
                     bSimulationMode = true;
                     bSubMenuContinue = false;
                     break;
                  case '2':
                     bSimulationMode = false;
                     bSubMenuContinue = false;
                     break;
                  case 'Q':
                  case 'q':
                     bSubMenuContinue = false;
                     bContinue = true;
                     break;
                  default:
                     break;
               }
            }
         }
         while (bSubMenuContinue);
      }
      while (bContinue);

      HANDLE hPhone ;
      if (bSimulationMode)
      {
         hPhone = QLIB_ConnectServer(1);
      }
      else
      {
         hPhone = g_hResourceContext;
      }
      Test_RFCal_ESC_ET_CAL(hPhone,command,bSimulationMode, true /* TE Simulation Mode */);

      printf("\nRFCal Demo finished. The output is saved at C:\\temp\\.\nPlease press any key to back to ESC ET menu");
      char cKey = toupper( _getch() );
   }
   while (bMainLoop);
   return true;
}
