/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFVerify/RFVerify_Legacy.cpp#1 $
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

#include "RFVerify_Legacy.h"
#include <sstream>
extern HANDLE g_hResourceContext;


unsigned char Test_LTE_FTM_RF_Tx()
{
   unsigned char bOk=true;
   short iMode=PHONE_MODE_LTE_B1;
   unsigned char iBandWidth=3;//0 - RFCOM_BW_LTE_1P4MHz 1 - RFCOM_BW_LTE_3MHz 2 - RFCOM_BW_LTE_5MHz 3 - RFCOM_BW_LTE_10MHz 4 - RFCOM_BW_LTE_15MHz 5 - RFCOM_BW_LTE_20MHz
   unsigned short iChannel=18050;
   short eModeID=FTM_MODE_ID_LTE;

   unsigned char  iTxWaveform=1;//0-CW, 1- PUSCH, 2- PUCCH, 3 - PRACH, 4 - SRS, 5 - UpPTS
   unsigned char  inumRBsPUSCH=50;
   unsigned char  inumRBsPUCCH=0;
   unsigned char  iPUSCHStartRBIndex=0;

   unsigned long  iModulationType=0;//0 - QPSK, 1 - 16 QAM, 2 - 64 QAM

   short iPowerLevel=300;//The unit is dBm*10

   //Enable QMSL logging
   printf("\nEnable QMSL logging...\n");
   QLIB_StopLogging(g_hResourceContext);
   ostringstream QMSLLogFilename;

   QMSLLogFilename << "C:\\Qualcomm\\QDART\\temp\\QLIBDemo-QMSL-log-LTE_FTM_RF_TX.txt";
   unsigned int _iLogFlags = LOG_FN | LOG_RET | LOG_IO | LOG_ERR | LOG_C_HIGH_LEVEL_START | LOG_C_HIGH_LEVEL_STOP;
   QLIB_SetLogFlags( g_hResourceContext, _iLogFlags );
   QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
   QLIB_StartLogging(g_hResourceContext,(char *)QMSLLogFilename.str().c_str());



   //Enter into FTM Mode
   printf("\nEnter into FTM mode...\n");
   int eMode=MODE_FTM_F;
   bOk&=QLIB_DIAG_CONTROL_F(g_hResourceContext, eMode);

   //Make sure calibration state off before doing FTM RF Testing
   printf("\nTurn off calibration state...\n");
   bOk&=QLIB_FTM_SET_CALIBRATION_STATE(g_hResourceContext, 0);




   /*------------------Set LTE Band Mode And Channel------------------*/
   // For example,
   // LTE Band 1
   // UL Channel 18050
   /*-------------------------------------------------------------------*/

   //Set Mode
   printf("\nSet LTE Band mode...\n");
   bOk&=QLIB_FTM_SET_MODE(g_hResourceContext, iMode);


   //Set BandWidth
   printf("\nSet LTE BandWidth...\n");
   bOk&=QLIB_FTM_LTE_SET_TX_BANDWIDTH(g_hResourceContext, iBandWidth);
   bOk&=QLIB_FTM_LTE_SET_RX_BANDWIDTH(g_hResourceContext, iBandWidth);

   //Set Channel
   printf("\nSet the UL channel...\n");
   bOk&=QLIB_FTM_SET_CHAN(g_hResourceContext, iChannel);


   /*------------Set LTE waveform type and set Tx On------------*/

   printf("\nSet Tx Modulation Type...\n");

   bOk&=QLIB_FTM_LTE_SET_TX_MODULATION_TYPE(g_hResourceContext, iModulationType);

   printf("\nSet Tx Waveform Type...\n");
   bOk&=QLIB_FTM_LTE_SET_TX_WAVEFORM(g_hResourceContext, iTxWaveform, inumRBsPUSCH, inumRBsPUCCH, iPUSCHStartRBIndex);

   printf("\nSet Tx on...\n");
   bOk&=QLIB_FTM_SET_TX_ON(g_hResourceContext);


   //Set Tx power level
   printf("\nSet Tx power level...\n");
   unsigned char iEnable=true;
   unsigned short iFTM_Error_Code=0;
   bOk&=QLIB_FTM_SET_TX_POWER_DBM(g_hResourceContext, iEnable, iPowerLevel, &iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);

   //Set NS value
   printf("\nSet Network signal value...\n");
   unsigned char iNS=1;
   bOk&=QLIB_FTM_LTE_SET_NET_SIG_VALUE(g_hResourceContext, iNS, &iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);

   /*--------Frequency Adjust--------*/
   printf("\nFrequency Adjust...\n");
   long userFreqAdjust=0;
   unsigned long centreFreq=static_cast<unsigned long>(QLIB_Band_Helper_CalculateULFrequency(iMode,iChannel)); //Channel 18050, use helper function
   char enableXOFreqAdjust=true;
   long totalFreqAdjust=0;

   //To establish the frequency adjust after applying the correction based on the frequency error predicted by XO manager.
   bOk&=QLIB_FTM_SET_FREQ_ADJUST(g_hResourceContext, userFreqAdjust, centreFreq, enableXOFreqAdjust, &totalFreqAdjust, &iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);
   //Measure the Frequency error on the test equipment. The measured frequency error will be used in the user_freq_adjust_ppb field in subsequent calls to FTM_SET_FREQ_ADJUST
   //For example, get the vlaue of Freq Error, -1.067519E+002, from test equipment
   double freqError=-106.7519;

   userFreqAdjust=static_cast<long>(((freqError*1000)/centreFreq));//Channel 18050, Freq:1925MHz
   bOk&=QLIB_FTM_SET_FREQ_ADJUST(g_hResourceContext, userFreqAdjust, centreFreq, enableXOFreqAdjust, &totalFreqAdjust, &iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);

   /*--------Start Tx Testing--------*/
   //
   // GPIB commands for Tx measurement,
   // For example, like Max power, Min power, ACLR, OBW, SEM,etc..
   //
   /*--------------------------------*/

   //FTM RF Tx test finished. Disable QMSL logging
   printf("\nStop QMSL logging...\n");
   QLIB_StopLogging(g_hResourceContext);

   return bOk;
}

unsigned char Test_LTE_FTM_RF_Rx()
{
   unsigned char bOk=true;
   short iMode=PHONE_MODE_LTE_B1;
   unsigned char iBandWidth=3;//0 - RFCOM_BW_LTE_1P4MHz 1 - RFCOM_BW_LTE_3MHz 2 - RFCOM_BW_LTE_5MHz 3 - RFCOM_BW_LTE_10MHz 4 - RFCOM_BW_LTE_15MHz 5 - RFCOM_BW_LTE_20MHz
   unsigned short iChannel=18050;
   unsigned short iSecondaryChainMode=0;
   short iRx_AGC=0;
   short iRx_Power_dBm=0;
   unsigned short iFTM_Error_Code=0;
   short eModeID=FTM_MODE_ID_LTE;


   //Enable QMSL logging
   printf("\nEnable QMSL logging...\n");
   QLIB_StopLogging(g_hResourceContext);
   ostringstream QMSLLogFilename;
   QMSLLogFilename << "C:\\Qualcomm\\QDART\\temp\\QLIBDemo-QMSL-log-LTE_FTM_RF_RX.txt";
   unsigned int _iLogFlags = LOG_FN | LOG_RET | LOG_IO | LOG_ERR | LOG_C_HIGH_LEVEL_START | LOG_C_HIGH_LEVEL_STOP | LOG_ASYNC;
   QLIB_SetLogFlags( g_hResourceContext, _iLogFlags );
   QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
   QLIB_StartLogging(g_hResourceContext,(char *)QMSLLogFilename.str().c_str());


   //Enter into FTM Mode
   printf("\nEnter into FTM mode...\n");
   int eMode=MODE_FTM_F;
   bOk&=QLIB_DIAG_CONTROL_F(g_hResourceContext, eMode);

   //Make sure calibration state off before doing FTM RF Testing
   printf("\nTurn off calibration state...\n");
   bOk&=QLIB_FTM_SET_CALIBRATION_STATE(g_hResourceContext, 0);

   /*------------------Set LTE Band Mode And Channel------------------*/
   // For example,
   // LTE Band 1
   // UL Channel 18050
   /*-------------------------------------------------------------------*/

   //Set Mode
   printf("\nSet LTE Band mode...\n");
   bOk&=QLIB_FTM_SET_MODE(g_hResourceContext, iMode);


   //Set BandWidth
   printf("\nSet LTE BandWidth...\n");
   bOk&=QLIB_FTM_LTE_SET_TX_BANDWIDTH(g_hResourceContext, iBandWidth);
   bOk&=QLIB_FTM_LTE_SET_RX_BANDWIDTH(g_hResourceContext, iBandWidth);

   //Set Channel
   printf("\nSet the UL channel...\n");
   bOk&=QLIB_FTM_SET_CHAN(g_hResourceContext, iChannel);

   //Set the secondary chain
   eModeID=FTM_MODE_LTE_RX1;
   bOk&=QLIB_FTM_SET_MODE_ID(g_hResourceContext, eModeID);
   printf("\nSet secondary chain...\n");
   bOk&=QLIB_FTM_SET_SECONDARY_CHAIN(g_hResourceContext, iSecondaryChainMode);


   //Set the LNA state
   eModeID=FTM_MODE_ID_LTE;
   bOk&=QLIB_FTM_SET_MODE_ID(g_hResourceContext, eModeID);

   short  iExpected_RX_Power_dBm=-950;
   unsigned char iSelected_LNA_Range=0;//This will be a return value from modem software side

   printf("\nSet the LNA state...\n");
   bOk&=QLIB_FTM_LTE_SET_LNA_STATE_DBM(g_hResourceContext,iExpected_RX_Power_dBm,&iSelected_LNA_Range,&iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);

   //Get the Rx Level
   bOk&=QLIB_FTM_GET_RX_LEVEL_DBM(g_hResourceContext, &iRx_AGC, &iRx_Power_dBm, &iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);
   printf("\nGet Rx Level, Rx AGC:%d, Rx dBm:%d\n",(short)iRx_AGC,(short)iRx_Power_dBm);


   //FTM RF Rx test finished. Disable QMSL logging
   printf("\nStop QMSL logging...\n");
   QLIB_StopLogging(g_hResourceContext);

   return bOk;
}

unsigned char Test_GSM_FTM_RF_Tx()
{
   unsigned char bOk=true;
   short eModeID=FTM_MODE_ID_GSM;
   short iMode=PHONE_MODE_GSM_850;
   unsigned short iChannel=150;
   unsigned short iFTM_Error_Code=0;


   //Enable QMSL logging
   printf("\nEnable QMSL logging...\n");
   QLIB_StopLogging(g_hResourceContext);
   ostringstream QMSLLogFilename;
   QMSLLogFilename << "C:\\Qualcomm\\QDART\\temp\\QLIBDemo-QMSL-log-GSM_FTM_RF_TX.txt";
   unsigned int _iLogFlags = LOG_FN | LOG_RET | LOG_IO | LOG_ERR | LOG_C_HIGH_LEVEL_START | LOG_C_HIGH_LEVEL_STOP;
   QLIB_SetLogFlags( g_hResourceContext, _iLogFlags );
   QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
   QLIB_StartLogging(g_hResourceContext,(char *)QMSLLogFilename.str().c_str());



   //Enter into FTM Mode
   printf("\nEnter into FTM mode...\n");
   int eMode=MODE_FTM_F;
   bOk&=QLIB_DIAG_CONTROL_F(g_hResourceContext, eMode);

   //Make sure calibration state off before doing FTM RF Testing
   printf("\nTurn off calibration state...\n");
   bOk&=QLIB_FTM_SET_CALIBRATION_STATE(g_hResourceContext, 0);




   /*------------------Set GSM Band Mode And Channel------------------*/
   // For example,
   // GSM Band 850
   // Channel 150
   /*-------------------------------------------------------------------*/

   //Set Mode
   printf("\nSet GSM Band mode...\n");
   bOk&=QLIB_FTM_SET_MODE(g_hResourceContext, iMode);


   //Set Channel
   printf("\nSet the UL channel...\n");
   bOk&=QLIB_FTM_SET_CHAN(g_hResourceContext, iChannel);



   /*------------Set Tx Frame Matrix------------*/


   for(int slotNum=0; slotNum<8; slotNum++)
   {
      bOk&=QLIB_FTM_SET_TX_FRAME_MATRIX(g_hResourceContext,slotNum,0,0,0);
   }
   printf("\nSet Slot 3 Tx on...\n");
   unsigned char  iSlotNum=3;
   unsigned char  iTurnSlotOn=1;
   unsigned short  iPwrLevel=3000;
   unsigned char  iModScheme=0;
   bOk&=QLIB_FTM_SET_TX_FRAME_MATRIX(g_hResourceContext,iSlotNum,iTurnSlotOn,iPwrLevel,iModScheme);

   printf("\nSet Tx on...\n");
   bOk&=QLIB_FTM_SET_TX_ON(g_hResourceContext);




   /*--------Frequency Adjust--------*/
   printf("\nFrequency Adjust...\n");
   long userFreqAdjust=0;
   unsigned long centreFreq=static_cast<unsigned long>(QLIB_Band_Helper_CalculateULFrequency(iMode,iChannel)); //Channel 9750, use helper function
   char enableXOFreqAdjust=true;
   long totalFreqAdjust=0;

   //To establish the frequency adjust after applying the correction based on the frequency error predicted by XO manager.
   bOk&=QLIB_FTM_SET_FREQ_ADJUST(g_hResourceContext, userFreqAdjust, centreFreq, enableXOFreqAdjust, &totalFreqAdjust, &iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);
   //Measure the Frequency error on the test equipment. The measured frequency error will be used in the user_freq_adjust_ppb field in subsequent calls to FTM_SET_FREQ_ADJUST
   //For example, get the vlaue of Freq Error, -1.067519E+002, from test equipment
   double freqError=-106.7519;

   userFreqAdjust=static_cast<long>((freqError*1000)/centreFreq);
   bOk&=QLIB_FTM_SET_FREQ_ADJUST(g_hResourceContext, userFreqAdjust, centreFreq, enableXOFreqAdjust, &totalFreqAdjust, &iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);

   /*--------Start Tx Testing--------*/
   //
   // GPIB commands for Tx measurement,
   //For example, Modulation analysis, ORFS, etc.
   //
   /*--------------------------------*/


   //FTM RF Tx test finished. Disable QMSL logging
   printf("\nStop QMSL logging...\n");
   QLIB_StopLogging(g_hResourceContext);

   return bOk;
}

unsigned char Test_GSM_FTM_RF_Rx()
{
   unsigned char bOk=true;
   short eModeID=FTM_MODE_ID_GSM;
   short iMode=PHONE_MODE_GSM_850;
   unsigned short iChannel=150;
   unsigned short iFTM_Error_Code=0;


   //Enable QMSL logging
   printf("\nEnable QMSL logging...\n");
   QLIB_StopLogging(g_hResourceContext);
   ostringstream QMSLLogFilename;
   QMSLLogFilename << "C:\\Qualcomm\\QDART\\temp\\QLIBDemo-QMSL-log-GSM_FTM_RF_RX.txt";
   unsigned int _iLogFlags = LOG_FN | LOG_RET | LOG_IO | LOG_ERR | LOG_C_HIGH_LEVEL_START | LOG_C_HIGH_LEVEL_STOP | LOG_ASYNC;
   QLIB_SetLogFlags( g_hResourceContext, _iLogFlags );
   QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
   QLIB_StartLogging(g_hResourceContext,(char *)QMSLLogFilename.str().c_str());


   //Enter into FTM Mode
   printf("\nEnter into FTM mode...\n");
   int eMode=MODE_FTM_F;
   bOk&=QLIB_DIAG_CONTROL_F(g_hResourceContext, eMode);

   //Make sure calibration state off before doing FTM RF Testing
   printf("\nTurn off calibration state...\n");
   bOk&=QLIB_FTM_SET_CALIBRATION_STATE(g_hResourceContext, 0);

   /*------------------Set GSM Band Mode And Channel------------------*/
   // For example,
   // GSM Band 850
   // Channel 150
   /*-------------------------------------------------------------------*/

   //Set Mode
   printf("\nSet GSM Band mode...\n");
   bOk&=QLIB_FTM_SET_MODE(g_hResourceContext, iMode);


   //Set Channel
   printf("\nSet the UL channel...\n");
   bOk&=QLIB_FTM_SET_CHAN(g_hResourceContext, iChannel);


   //Configure LNA state
   printf("\nConfigure LNA state based on expected DL pwr level...\n");
   int  iExpected_RX_Power_dbm16=-1440;//-90dBm*16
   unsigned short  iNumber_of_Averages=3;

   bOk&=QLIB_FTM_GSM_SETUP_RX_BURST_FOR_EXPECTED_PWR(g_hResourceContext,iExpected_RX_Power_dbm16,iNumber_of_Averages,&iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);

   //Get the Rx Level
   unsigned short  iMeasured_Dbm16_Value=0;
   unsigned long  iRaw_RSSI=0;
   bOk&=QLIB_FTM_GSM_GET_MEAS_PWR(g_hResourceContext,&iMeasured_Dbm16_Value,&iRaw_RSSI,&iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);
   printf("\nGet Rx Level, Rx AGC:%d, Rx dBm:%d\n",iRaw_RSSI,(short)iMeasured_Dbm16_Value/16);


   //FTM RF Rx test finished. Disable QMSL logging
   printf("\nStop QMSL logging...\n");
   QLIB_StopLogging(g_hResourceContext);

   return bOk;
}





unsigned char Test_WCDMA_FTM_RF_Tx()
{
   unsigned char bOk=true;
   short iMode=PHONE_MODE_WCDMA_IMT;
   unsigned short iChannel=9750;
   unsigned short iSecondaryChainMode=2;
   short iPowerLevel=300;

   //Enable QMSL logging
   printf("\nEnable QMSL logging...\n");
   QLIB_StopLogging(g_hResourceContext);
   ostringstream QMSLLogFilename;
   QMSLLogFilename << "C:\\temp\\QLIBDemo-QMSL-log-WCDMA_FTM_RF_TX.txt";
   unsigned int _iLogFlags = LOG_FN | LOG_RET | LOG_IO | LOG_ERR | LOG_C_HIGH_LEVEL_START | LOG_C_HIGH_LEVEL_STOP;
   QLIB_SetLogFlags( g_hResourceContext, _iLogFlags );
   QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
   QLIB_StartLogging(g_hResourceContext,(char *)QMSLLogFilename.str().c_str());

   //Enter into FTM Mode
   printf("\nEnter into FTM mode...\n");
   int eMode=MODE_FTM_F;
   bOk&=QLIB_DIAG_CONTROL_F(g_hResourceContext, eMode);

   //Make sure calibration state off before doing FTM RF Testing
   printf("\nTurn off calibration state...\n");
   bOk&=QLIB_FTM_SET_CALIBRATION_STATE(g_hResourceContext, 0);

   /*------------------Set WCDMA Band Mode And Channel------------------*/
   // For example,
   // WCDMA Band 1
   // Channel 9750
   /*-------------------------------------------------------------------*/

   //Set Mode
   printf("\nSet WCDMA Band mode...\n");
   bOk&=QLIB_FTM_SET_MODE(g_hResourceContext, iMode);

   //Turn off the phone's transmit chain
   printf("\nTurn off Tx chain...\n");
   bOk&=QLIB_FTM_SET_TX_OFF(g_hResourceContext);

   //Set Channel
   printf("\nSet the UL channel...\n");
   bOk&=QLIB_FTM_SET_CHAN(g_hResourceContext, iChannel);

   //Set Mode ID
   short eModeID=FTM_MODE_WCDMA_RX1;
   bOk&=QLIB_FTM_SET_MODE_ID(g_hResourceContext, eModeID);

   //Set the secondary chain
   printf("\nTurn off secondary chain...\n");
   bOk&=QLIB_FTM_SET_SECONDARY_CHAIN(g_hResourceContext, iSecondaryChainMode);

   /*------------Set Tx On------------*/

   printf("\nSet Tx on...\n");
   eModeID=FTM_MODE_ID_WCDMA;
   bOk&=QLIB_FTM_SET_MODE_ID(g_hResourceContext, eModeID);
   bOk&=QLIB_FTM_SET_TX_ON(g_hResourceContext);

   //Set uplink waveform
   printf("\nTurn off the CW waveform...\n");
   bOk&=QLIB_FTM_CDMA_CW_WAVEFORM(g_hResourceContext, 0);

   //Set Tx power level
   printf("\nSet Tx power level...\n");
   unsigned char iEnable=true;
   unsigned short iFTM_Error_Code=0;
   bOk&=QLIB_FTM_SET_TX_POWER_DBM(g_hResourceContext, iEnable, iPowerLevel, &iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);

   /*--------Frequency Adjust--------*/
   printf("\nFrequency Adjust...\n");
   long userFreqAdjust=0;
   unsigned long centreFreq = (unsigned long)QLIB_Band_Helper_CalculateULFrequency(BH_WCDMA_BAND_I,iChannel); //Channel 9750, use helper function
   char enableXOFreqAdjust=true;
   long totalFreqAdjust=0;

   //To establish the frequency adjust after applying the correction based on the frequency error predicted by XO manager.
   bOk&=QLIB_FTM_SET_FREQ_ADJUST(g_hResourceContext, userFreqAdjust, centreFreq, enableXOFreqAdjust, &totalFreqAdjust, &iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);
   //Measure the Frequency error on the test equipment. The measured frequency error will be used in the user_freq_adjust_ppb field in subsequent calls to FTM_SET_FREQ_ADJUST
   //For example, get the vlaue of Freq Error, -1.067519E+002, from test equipment
   double freqError=-106.7519;

   userFreqAdjust=(long)((freqError*1000)/1950);
   bOk&=QLIB_FTM_SET_FREQ_ADJUST(g_hResourceContext, userFreqAdjust, centreFreq, enableXOFreqAdjust, &totalFreqAdjust, &iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);

   //FTM RF Tx test finished. Disable QMSL logging
   printf("\nStop QMSL logging...\n");
   QLIB_StopLogging(g_hResourceContext);

   return bOk;
}

unsigned char Test_WCDMA_FTM_RF_Rx()
{
   unsigned char bOk=true;
   short iMode=PHONE_MODE_WCDMA_IMT;
   unsigned short iChannel=9750;
   unsigned short iSecondaryChainMode=0;
   short iRx_AGC=0;
   short iRx_Power_dBm=0;
   unsigned short iFTM_Error_Code=0;

   //Enable QMSL logging
   printf("\nEnable QMSL logging...\n");
   QLIB_StopLogging(g_hResourceContext);
   ostringstream QMSLLogFilename;
   QMSLLogFilename << "C:\\temp\\QLIBDemo-QMSL-log-WCDMA_FTM_RF_RX.txt";
   unsigned int _iLogFlags = LOG_FN | LOG_RET | LOG_IO | LOG_ERR | LOG_C_HIGH_LEVEL_START | LOG_C_HIGH_LEVEL_STOP | LOG_ASYNC;
   QLIB_SetLogFlags( g_hResourceContext, _iLogFlags );
   QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
   QLIB_StartLogging(g_hResourceContext,(char *)QMSLLogFilename.str().c_str());

   //Enter into FTM Mode
   printf("\nEnter into FTM mode...\n");
   int eMode=MODE_FTM_F;
   bOk&=QLIB_DIAG_CONTROL_F(g_hResourceContext, eMode);

   //Make sure calibration state off before doing FTM RF Testing
   printf("\nTurn off calibration state...\n");
   bOk&=QLIB_FTM_SET_CALIBRATION_STATE(g_hResourceContext, 0);

   /*------------------Set WCDMA Band Mode And Channel------------------*/
   // For example,
   // WCDMA Band 1
   // Channel 9750
   /*-------------------------------------------------------------------*/

   //Set Mode
   printf("\nSet WCDMA Band mode...\n");
   bOk&=QLIB_FTM_SET_MODE(g_hResourceContext, iMode);

   //Set Channel
   printf("\nSet the UL channel...\n");
   bOk&=QLIB_FTM_SET_CHAN(g_hResourceContext, iChannel);

   //Set Mode ID
   short eModeID=FTM_MODE_WCDMA_RX1;
   bOk&=QLIB_FTM_SET_MODE_ID(g_hResourceContext, eModeID);

   //Set the secondary chain
   printf("\nTurn off secondary chain...\n");
   bOk&=QLIB_FTM_SET_SECONDARY_CHAIN(g_hResourceContext, iSecondaryChainMode);

   //Get the Rx Level
   eModeID=FTM_MODE_ID_WCDMA;
   bOk&=QLIB_FTM_SET_MODE_ID(g_hResourceContext, eModeID);
   bOk&=QLIB_FTM_GET_RX_LEVEL_DBM(g_hResourceContext, &iRx_AGC, &iRx_Power_dBm, &iFTM_Error_Code);
   bOk&=(!iFTM_Error_Code);
   printf("\nGet Rx Level, Rx AGC:%d, Rx dBm:%d\n",(short)iRx_AGC,(short)iRx_Power_dBm);

   //FTM RF Rx test finished. Disable QMSL logging
   printf("\nStop QMSL logging...\n");
   QLIB_StopLogging(g_hResourceContext);

   return bOk;
}






unsigned char Test_WCDMA_BER( void )
{
   unsigned char bOk = true;
   char cKey = ' ';
   unsigned short _iChannel = 10562;    // 10562 for IMT, 9800 for WCDMA1900B
   unsigned short _iChannel2 = 10562;
   unsigned long c_iDefaultTimeout = FTM_NONSIG_DEFAULT_TIMEOUT;

   // For this parameter, Agilent default is 9, R&S default is 2
   unsigned short iDL_ChanCode = 9;

   // Other RMC parameters
   unsigned char iUL_MaxTxPwr = 24;
   unsigned long iUL_DPCH_ScrCode =0;

   unsigned char iUL_PCA = FTM_WCDMA_BER_PCA_Type_Alg1;

   unsigned char iUL_TPC_StepSize = FTM_WCDMA_BER_PCA_Size_1dB;

   unsigned short iDL_PriCPICH_Scr_Code = 0;

   unsigned char iBetaC = 8;
   unsigned char iBetaD = 15;

   printf("\n----------------\nWCDMA FTM BER Test\n----------------\n");

   printf("\nSetup WCDMA non-signaling:");
   printf("\n     -> Generator settings->primary scrambling = 0");
   printf("\n     -> Generator settings->RF Channel = %d", _iChannel );
   printf("\n     -> Channel Settings ");
   printf("\n          -> Dedicated & Broadcast channels");
   printf("\n                 -> DPCH Channel Code = %d ", iDL_ChanCode );
   printf("\n                 -> Secondary Scrambling Code = 0");
   printf("\n          -> 3GPP DL Reference Channel");
   printf("\n                 -> Reference channel type = 12.2kbps ");
   printf("\n      UE Code (Uplink channel) = 0");
   printf("\n     -> Connect -> Page 2 -> Trigger -> Source = Internal (for timing measurements)");
   printf("\n     -> Measure -> Max Power -> Power Configuration ->Maximum Power -> TPC = AUTO");
   printf("\n     -> Measure -> Max Power -> Power Configuration ->Minimum Power -> TPC = AUTO");
   printf("\n");
   printf("\n1 = Run a normal acquisition and RMC setup");
   printf("\n2 = Run a loop of 1000 acquisitions");
   printf("\n3 = Run a normal acquisition and RMC setup with support TFCI configuration for Agilent 8960");
   char _cKeyPress = toupper( getch() );

   QLIB_AddUserMessageToTextLog(g_hResourceContext, LOG_FN|LOG_IO, "<<<WCDMA BER -- Start>>>>", false );

   if ( ( _cKeyPress == '1' )
         ||    ( _cKeyPress == '3' )
      )
   {
      // Specify number of buffers per FTM log message
      QLIB_FTM_LOG_SetFTM_LogBufferCount( g_hResourceContext, 10 );

      // Start
      bOk = QLIB_FTM_WCDMA_BER_StartSession( g_hResourceContext, c_iDefaultTimeout );
      printf("\n QLIB_FTM_WCDMA_BER_StartSession: %s", bOk ? "Pass" : "FAIL" );

      if ( bOk )
      {
         // Acq pilot
         bOk = QLIB_FTM_WCDMA_BER_AcquirePilot(     g_hResourceContext, _iChannel );
         printf("\n QLIB_FTM_WCDMA_BER_AcquirePilot: %s", bOk ? "Pass" : "FAIL" );
      }

      if (  ( bOk)
            && ( _cKeyPress == '3' )
         )
      {
         unsigned char _aTFCI_Config[4] = { 0,2,1, 3};
         // Configure UL TFCI
         bOk = QLIB_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ(  g_hResourceContext, FTM_WCDMA_BER_UL_TFCS_CONFIG, _aTFCI_Config );
         printf("\n QLIB_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ(), UL: %s", bOk ? "Pass" : "FAIL" );

         // Configure UL TFCI
         bOk &= QLIB_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ(     g_hResourceContext, FTM_WCDMA_BER_DL_TFCS_CONFIG, _aTFCI_Config );
         printf("\n QLIB_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ(), DL: %s", bOk ? "Pass" : "FAIL" );
      } // ( _cKeyPress == '3' )

      if ( bOk )
      {
         // This is the simplest setup for DPCH, but cannot be used for 8960
         // bOk = QLIB_FTM_WCDMA_BER_SetupDefaultRMC_DCH( g_hResourceContext );
         // printf("\n FTM_WCDMA_BER_SetupDefaultRMC_DCH: %s", bOk ? "Pass" : "FAIL" );

         // Setup parameters
         unsigned char iRMC_Type = FTM_WCDMA_BER_RMC_Type12_2kpbs;
         // Enable the following line for the 8960
         // iRMC_Type = FTM_WCDMA_BER_RMC_Type12_2kpbs_Symmetric;

         unsigned char iUL_MaxTxPwr = 24;
         unsigned long iUL_DPCH_ScrCode =0;

         unsigned char iUL_PCA = FTM_WCDMA_BER_PCA_Type_Alg1;

         unsigned char iUL_TPC_StepSize = FTM_WCDMA_BER_PCA_Size_1dB;

         unsigned short iDL_PriCPICH_Scr_Code = 0;

         bOk = QLIB_FTM_WCDMA_BER_SetupRMC_DCH(
                  g_hResourceContext,
                  iRMC_Type,
                  iUL_MaxTxPwr,
                  iUL_DPCH_ScrCode,
                  iUL_PCA,
                  iUL_TPC_StepSize,
                  iDL_PriCPICH_Scr_Code,
                  iDL_ChanCode );

         printf("\n QLIB_FTM_WCDMA_BER_SetupRMC_DCH: %s", bOk ? "Pass" : "FAIL" );;
      }

      if ( bOk )
      {
         unsigned char _bExit = true;

         QLIB_AddUserMessageToTextLog(g_hResourceContext, LOG_FN|LOG_IO, "<<<WCDMA BER -- Setup complete>>>>", false );

         do
         {
            _bExit = true;

            printf("\n\nPerform testing on physical layer then <<press any key to stop>>");
            printf("\n  'R' for WCDMA Report");
            printf("\n  'H' to handover to %d V2", _iChannel2);
            printf("\n  '2' to handover using method V2B");
            printf("\n  'S' to initiate RMC Symetrical (with CRC) 12.2kbps loopback, on the 1st demo channel(%d)", _iChannel);

            cKey = toupper( getch() );

            if ( cKey == 'R' )
            {
               PrintWCDMA_AGC();
               _bExit = false;
            }   // if ( cKey == 'R' )

            if ( cKey == 'H' )
            {
               // Setup parameters
               unsigned char iRMC_Type = FTM_WCDMA_BER_RMC_Type12_2kpbs;

               bOk = QLIB_FTM_WCDMA_BER_Handover(
                        g_hResourceContext,
                        iRMC_Type,
                        iUL_MaxTxPwr,
                        iUL_DPCH_ScrCode,
                        iUL_PCA,
                        iUL_TPC_StepSize,
                        iDL_PriCPICH_Scr_Code,
                        iDL_ChanCode,
                        iBetaC,
                        iBetaD,
                        _iChannel2);
               printf("\n QLIB_FTM_WCDMA_BER_Handover: %s", bOk ? "Pass" : "FAIL" );

               _bExit = false;
            }   // if ( cKey == 'H' )
            else if ( cKey == 'S' )
            {
               // Setup parameters
               unsigned char iRMC_Type = FTM_WCDMA_BER_RMC_Type12_2kpbs_Symmetric;

               bOk = QLIB_FTM_WCDMA_BER_Handover(
                        g_hResourceContext,
                        iRMC_Type,
                        iUL_MaxTxPwr,
                        iUL_DPCH_ScrCode,
                        iUL_PCA,
                        iUL_TPC_StepSize,
                        iDL_PriCPICH_Scr_Code,
                        iDL_ChanCode,
                        iBetaC,
                        iBetaD,
                        _iChannel);
               printf("\n QLIB_FTM_WCDMA_BER_Handover: %s", bOk ? "Pass" : "FAIL" );

               _bExit = false;
            }   // if ( cKey == 'H' )
            if ( cKey == '2' )
            {
               unsigned char iRMC_Type = FTM_WCDMA_BER_RMC_Type12_2kpbs;

               short iDPCCH_PowerOffset = -60;
               unsigned char n_312 = 0;
               unsigned char n_313 = 4;
               unsigned char n_315 = 0;
               unsigned char t_312 = 1;
               unsigned char t_313 = 3;

               // This the new channel number
               unsigned short iChannel = 10562;

               bOk = QLIB_FTM_WCDMA_BER_Handover_V2B(
                        g_hResourceContext,
                        iRMC_Type,
                        iUL_MaxTxPwr,
                        iUL_DPCH_ScrCode,
                        iUL_PCA,
                        iUL_TPC_StepSize,
                        iDL_PriCPICH_Scr_Code,
                        iDL_ChanCode,
                        iBetaC,
                        iBetaD,
                        iDPCCH_PowerOffset,
                        n_312,
                        n_313,
                        n_315,
                        t_312,
                        t_313,
                        iChannel );

               printf("\n QLIB_FTM_WCDMA_BER_Handover_V2B: %s", bOk ? "Pass" : "FAIL" );

               _bExit = false;
            }   // if ( cKey == 'H' )
         }
         while (!_bExit);   // do while (!bExit)

         bOk = QLIB_FTM_WCDMA_BER_StopSession( g_hResourceContext );
         printf("\n QLIB_FTM_WCDMA_BER_StopSession: %s", bOk ? "Pass" : "FAIL" );
      } //if ( bOk )
   }    // if ( _cKeyPress == '1' )
   else if ( _cKeyPress == '2' )
   {
      QLIB_DIAG_SetMessageFilter( g_hResourceContext,  23, 0xff );  // FTM
      QLIB_DIAG_SetMessageFilter( g_hResourceContext,  3000, 0xff );  // WCDMA
      QLIB_DIAG_SetMessageFilter( g_hResourceContext,  3001, 0xff );  // WCDMA L1
      QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );

      // This forces the mobile to be in a STOPPED state
      bOk = QLIB_FTM_UMTS_BER_ForceStopped( g_hResourceContext );

      // Set text file logging
      QLIB_SetLogFlags( g_hResourceContext, LOG_C_HIGH_LEVEL_START | LOG_RET | LOG_FN | LOG_ASYNC );
      QLIB_StartLogging( g_hResourceContext, "c:\\factory_log.txt");

      // Error count
      int _iErrorCount = 0;
      int c_iMaxLoops = 1000;

      for ( int _iLoop = 0; _iLoop < c_iMaxLoops; _iLoop++ )
      {
         printf("\n\nLoop# %d/%d, Errors found: %d\n", _iLoop, c_iMaxLoops, _iErrorCount );

         // Start
         bOk = QLIB_FTM_WCDMA_BER_StartSession( g_hResourceContext, c_iDefaultTimeout );
         printf("\n QLIB_FTM_WCDMA_BER_StartSession: %s", bOk ? "Pass" : "FAIL" );

         if ( bOk )
         {
            // Going to IDLE is not required because this is the state
            // after the session is started
            bOk &= QLIB_FTM_WCDMA_BER_Idle(g_hResourceContext);
            printf("\n QLIB_FTM_WCDMA_BER_Idle: %s", bOk ? "Pass" : "FAIL" );

            // Acq pilot
            bOk &= QLIB_FTM_WCDMA_BER_AcquirePilot(     g_hResourceContext, _iChannel );
            printf("\n QLIB_FTM_WCDMA_BER_AcquirePilot: %s", bOk ? "Pass" : "FAIL" );

            if ( bOk && false )
            {
               // Setup parameters
               unsigned char iRMC_Type = FTM_WCDMA_BER_RMC_Type12_2kpbs;
               // Enable the following line for the 8960
               // iRMC_Type = FTM_WCDMA_BER_RMC_Type12_2kpbs_Symmetric;

               unsigned char iUL_MaxTxPwr = 24;
               unsigned long iUL_DPCH_ScrCode =0;

               unsigned char iUL_PCA = FTM_WCDMA_BER_PCA_Type_Alg1;

               unsigned char iUL_TPC_StepSize = FTM_WCDMA_BER_PCA_Size_1dB;

               unsigned short iDL_PriCPICH_Scr_Code = 0;

               bOk = QLIB_FTM_WCDMA_BER_SetupRMC_DCH(
                        g_hResourceContext,
                        iRMC_Type,
                        iUL_MaxTxPwr,
                        iUL_DPCH_ScrCode,
                        iUL_PCA,
                        iUL_TPC_StepSize,
                        iDL_PriCPICH_Scr_Code,
                        iDL_ChanCode );

               printf("\n QLIB_FTM_WCDMA_BER_SetupRMC_DCH: %s", bOk ? "Pass" : "FAIL" );;
            }
         }

         // Stop session
         bOk = QLIB_FTM_WCDMA_BER_StopSession( g_hResourceContext );
         printf("\n QLIB_FTM_WCDMA_BER_StopSession: %s", bOk ? "Pass" : "FAIL" );

         // Keep count of errors
         if ( !bOk )
         {
            _iErrorCount++;
         }

         if ( kbhit() )
         {
            getch();
            break;
         }  // if ( kbhit() )
      }

      printf("\n\nLoop completed, %d errors found out of %d.  Press any key to continue", _iErrorCount, c_iMaxLoops);
      getch();
   }    // if ( _cKeyPress == '2' )

   return bOk;
}

unsigned char Test_HSDPA_BER( void )
{
   unsigned char bOk = true;
   char cKey = ' ';
   unsigned short _iChannel = 10562;    // 10562 for IMT, 9800 for PCS
   unsigned long c_iDefaultTimeout = FTM_NONSIG_DEFAULT_TIMEOUT;

   printf("\n----------------\nHSDPA FTM BER Test (!!! Still under development!!!!)\n----------------\n");

   printf("\nSetup WCDMA non-signaling:");
   printf("\n     -> Generator settings->primary scrambling = 0");
   printf("\n     -> Generator settings->RF Channel = %d", _iChannel );
   printf("\n     -> Channel Settings ");
   printf("\n          -> Dedicated & Broadcast channels");
   printf("\n                 -> DPCH Channel Code = 3 ");
   printf("\n                 -> Secondary Scrambling Code = 0");
   printf("\n          -> 3GPP DL Reference Channel");
   printf("\n                 -> Reference channel type = 12.2kbps ");
   printf("\n      UE Code (Uplink channel) = 0");
   printf("\n     -> Connect -> Page 2 -> Trigger -> Source = Internal (for timing measurements)");
   printf("\n     -> Measure -> Max Power -> Power Configuration ->Maximum Power -> TPC = AUTO");
   printf("\n     -> Measure -> Max Power -> Power Configuration ->Minimum Power -> TPC = AUTO");
   printf("\n     -> Other HSDPA setups, according to 80-VA360-1 ");
   printf("\n");
   printf("\n1=HSET5");
   printf("\n2=1.8Mbps");
   printf("\n<<press a key to continue>>", _iChannel);
   cKey = toupper(getch());

   if ( cKey=='Q')
      return false;

   QLIB_AddUserMessageToTextLog(g_hResourceContext, LOG_FN|LOG_IO, "<<<WCDMA BER -- Start>>>>", false );

   // Start
   bOk = QLIB_FTM_WCDMA_BER_StartSession( g_hResourceContext, c_iDefaultTimeout );
   printf("\n FTM_WCDMA_BER_StartSession: %s", bOk ? "Pass" : "FAIL" );

   if ( bOk )
   {
      // Acq pilot
      bOk = QLIB_FTM_WCDMA_BER_AcquirePilot(    g_hResourceContext, _iChannel );
      printf("\n FTM_WCDMA_BER_AcquirePilot: %s", bOk ? "Pass" : "FAIL" );
   }

   if ( bOk )
   {
      // DPCH -- using defaults, the difference from the procedure is the DPCH code is 3 here instead of 2 in the document
      bOk = QLIB_FTM_WCDMA_BER_SetupDefaultRMC_DCH( g_hResourceContext );
      printf("\n FTM_WCDMA_BER_SetupDefaultRMC_DCH: %s", bOk ? "Pass" : "FAIL" );
   }

   if ( bOk )
   {
      unsigned char iHS_PDSCH_RadioLink_PCS = 0;

      bOk = QLIB_FTM_HSDPA_BLER_Configure_PDSCH( g_hResourceContext , iHS_PDSCH_RadioLink_PCS );

      printf("\n QLIB_FTM_HSDPA_BLER_Configure_PDSCH: %s", bOk ? "Pass" : "FAIL" );
   }

   if ( bOk )
   {
      unsigned char iNumCodes =  1;
      unsigned char aiCodes[4] = {12, 0, 0, 0};

      bOk = QLIB_FTM_HSDPA_BLER_Configure_HS_SCCH( g_hResourceContext,  iNumCodes, aiCodes );
      printf("\n QLIB_FTM_HSDPA_BLER_Configure_HS_SCCH: %s", bOk ? "Pass" : "FAIL" );
   }

   // This setup is for HSET 5
   if ( ( cKey=='1' )   &&  ( bOk ) )
   {
      unsigned short iH_RNTI = 43690;
      unsigned char iNum_HARQ_Processes = 3;
      unsigned char bMemPartitionValid = TRUE;
      unsigned char aiMemPartitionInfo[8] =  {11, 11, 11, 0, 0 , 0, 0, 0};

      bOk = QLIB_FTM_HSDPA_BLER_Configure_HS_DSCH(
               g_hResourceContext,
               iH_RNTI,
               iNum_HARQ_Processes,
               bMemPartitionValid,
               aiMemPartitionInfo
            );

      printf("\n QLIB_FTM_HSDPA_BLER_Configure_HS_DSCH: %s", bOk ? "Pass" : "FAIL" );
   }

   // This setup is for 1.8Mbps, a custom configuration
   if ( ( cKey=='2' ) &&    ( bOk ) )
   {
      unsigned short iH_RNTI = 43690;
      unsigned char iNum_HARQ_Processes = 6;
      unsigned char bMemPartitionValid = TRUE;
      unsigned char aiMemPartitionInfo[8] =  {5, 5, 5, 5, 5 , 5, 0, 0}; // 5 = 4800

      bOk = QLIB_FTM_HSDPA_BLER_Configure_HS_DSCH(
               g_hResourceContext,
               iH_RNTI,
               iNum_HARQ_Processes,
               bMemPartitionValid,
               aiMemPartitionInfo
            );

      printf("\n QLIB_FTM_HSDPA_BLER_Configure_HS_DSCH: %s", bOk ? "Pass" : "FAIL" );
   }

   if ( bOk )
   {
      unsigned char iCPICH_PowerOffset = 3;
      unsigned char iCQI_FeedbackCycle = 4;
      unsigned char iCQI_RepetitionFactor = 1;
      unsigned char iDeltaCQI = 0;
      unsigned char iDeltaACK = 0;
      unsigned char iDeltaNACK = 0;
      unsigned char iACK_NACK_RepetitionFactor = 1;

      bOk = QLIB_FTM_HSDPA_BLER_Configure_HS_DPCCH(
               g_hResourceContext,
               iCPICH_PowerOffset,
               iCQI_FeedbackCycle,
               iCQI_RepetitionFactor,
               iDeltaCQI,
               iDeltaACK,
               iDeltaNACK,
               iACK_NACK_RepetitionFactor
            );

      printf("\n QLIB_FTM_HSDPA_BLER_Configure_HS_DPCCH: %s", bOk ? "Pass" : "FAIL" );
   }

   // Now start HSDPA
   if ( bOk )
   {
      bOk = QLIB_FTM_HSDPA_BER_StartSession( g_hResourceContext );

      printf("\n QLIB_FTM_HSDPA_BER_StartSession: %s", bOk ? "Pass" : "FAIL" );
   }

   QLIB_AddUserMessageToTextLog(g_hResourceContext, LOG_FN|LOG_IO, "<<<WCDMA/HSDPA BER -- Setup complete>>>>", false );

   if ( bOk )
   {
      printf("\n\nPerform testing on physical layer then <<press any key to stop>> or 'A' for WCDMA Report");
      cKey = toupper( getch() );

      if ( cKey == 'A' )
      {
         PrintWCDMA_AGC();
      }
   }

   bOk = QLIB_FTM_HSDPA_BER_StopSession( g_hResourceContext );
   printf("\n QLIB_FTM_HSDPA_BER_StopSession: %s", bOk ? "Pass" : "FAIL" );

   bOk = QLIB_FTM_WCDMA_BER_StopSession( g_hResourceContext );
   printf("\n QLIB_FTM_WCDMA_BER_StopSession: %s", bOk ? "Pass" : "FAIL" );

   return bOk;
}
unsigned char Test_FTM_cdma2k_NON_SIGNALING( void )
{
   unsigned char bOk=true;
   unsigned char cKey;
   unsigned short _iChannel_BC0 = 387;
   unsigned char iWalsh = 10;
   double _dTurnAroundConstant = -73.0;
   unsigned long _iRadioConfig = FTM_CDMA2000_NS_RC_1;
   unsigned char iFFPC_Enable = 1;
   unsigned char bUseFastFunctionCall = false;

   // Perform Acquire will indicate if the pilot/sync/traffic channels should be assigned and deassigned
   unsigned char bPerformAcquire = false;

   // Most locals variables defined in this function that are parameters for QLIB functions do not use "_"
   // so that the function headers can be copied and uses almost as-is
   unsigned long iBand_Class;
   unsigned short iCDMA_Freq;

   printf("\n----------------\nTest_FTM_cdma2k_NON_SIGNALING\n----------------\n");

   if ( bOk )
   {
      printf("\n\n1=BC0, CH %d CDMA 800, using full function calls", _iChannel_BC0);
      printf("\n\n2=BC0, CH %d CDMA 800, using fast function call", _iChannel_BC0);
      printf("\n\n3=Start capturing Logs without acquisistion" );
      printf("\n\n4=BC0, CH %d CDMA 800, using fast function call with RDA log only", _iChannel_BC0);
      printf("\n");
      cKey = getch();

      printf("\nSetup:");
      printf("\n FCH Walsh = %d", iWalsh);
      printf("\n RC = %d", _iRadioConfig);
      printf("\n FCH Traffic = full rate");
      printf("\n SCH Walsh = 3");
      printf("\n SCH Coding = convolutional");
      printf("\n SCH Rate = 78.6kbps");
      printf("\n");
      printf("\n <<Press any key to continue>>");
      printf("\n");

      if (  ( cKey == '1')
            || ( cKey == '2')
            || ( cKey == '4')
         )
      {
         // Send band & channel parameters
         iBand_Class = FTM_EVDO_BAND_CLASS_CELLULAR;
         iCDMA_Freq = _iChannel_BC0;

         _dTurnAroundConstant = -73.0;

         bPerformAcquire = true;
      } // if ( cKey == '1')

      if ( cKey == '2' )
         bUseFastFunctionCall = true;

      // Timer around the connection functions
      dword _hTimer1 = QLIB_StartTimer(g_hResourceContext );

      //
      // Next go to online, then back to FTM, to avoid a searcher bug that will crash the phone
      //
      printf("\nShould the 75 FTM command mode used with run-time mode changing? Y/N)");
      char cKeyRuntimeSwitch = toupper(getch());

      if ( cKeyRuntimeSwitch == 'Y' )
      {
         //
         // This path uses command mode 75 and run-time mode changes
         //

         bOk = QLIB_FTM_SET_COMMAND_CODE( g_hResourceContext, FTM_COMMAND_75);

         //
         // This first part is going into ONLINE, then back to FTM.  This was done
         // to get around problems on the 6500, which could occur when the phone
         // powered up in FTM mode.  Of course the real solution is to fix the embedded
         // software, and this can probably be removed from newer code, but is necessary
         // here for better backwards compatability
         //
         bool _bGoIntoOnlineFirst = false;

         if ( _bGoIntoOnlineFirst)
         {
            if ( bOk )
            {
               // Go into ONLINE mode
               bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_ONLINE_F );
               printf("\nQLIB_DIAG_CONTROL_F(  MODE_ONLINE_F ): %s", bOk ? "PASS" : "FAIL\n" );
            }

            if ( bOk )
            {
               // Go back into
               bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_FTM_F );
               printf("\nQLIB_DIAG_CONTROL_F(  MODE_FTM_F ): %s", bOk ? "PASS" : "FAIL\n" );
            }
         }  // _bGoIntoOnlineFirst
      }
      else
      {
         //
         // This path uses command mode 59, and assumes phone is already in FTM mode.
         //
         bOk = QLIB_FTM_SET_COMMAND_CODE( g_hResourceContext, FTM_COMMAND_59);
      } // if ( cKeyRuntimeSwitch == 'Y' )

      //Track the NS execution time
      SYSTEMTIME start_systime, stop_systime;
      GetSystemTime(&start_systime);

      if ( cKey == '4' ) //4=BC0, CH %d CDMA 800, using fast function call with minimum logs
      {
         // Prospone the log display till very end for NS speed improvement
         // Quick command to get into traffic
         bOk = QLIB_FTM_CDMA2000_GOTO_FWD_TRAFFIC_STATE(
                  g_hResourceContext,
                  iBand_Class,
                  iCDMA_Freq,
                  _iRadioConfig,
                  iWalsh,
                  iFFPC_Enable );
         printf("\nQLIB_FTM_CDMA2000_GOTO_FWD_TRAFFIC_STATE: %s", bOk ? "PASS" : "FAIL\n" );

         //Prepare to monitor and parse the incoming SER logs
         bOk &= QLIB_FTM_CDMA2000_NS_SetSER_RCandRS(
                   g_hResourceContext,
                   (unsigned char) _iRadioConfig,
                   FTM_CDMA2000_NS_REV_FCH_FULL_RATE);

         printf("\nQLIB_FTM_CDMA2000_NS_SetSER_RCandRS: %s", bOk ? "PASS" : "FAIL\n" );

         // This will enable verbose messages to be generated for RDA parsing. The log type will be LOG_ASYC
         QLIB_ExtendedTextMessage_SetCategoryEnable(
            g_hResourceContext,
            QMSL_ExtTextMsgCat_cdma2000_RDA_Parse,
            TRUE );

         // Setup the RDA log code list for SER measurement
         const unsigned short c_iLogCodeListSize = 2;
         unsigned short _aiLogCodeList[ c_iLogCodeListSize ] =  { FTM_CDMA2000_RDA_FRAME_INFO, FTM_CDMA2000_RDA2_FRAME_INFO };
         bOk &= QLIB_DIAG_AddExtendedLogCodes( g_hResourceContext, LOG_EQUIP_ID_1X, _aiLogCodeList, c_iLogCodeListSize );
         printf("\n\nDIAG_AddExtendedLogCodes: %s \n  FTM_CDMA2000_RDA_FRAME_INFO\n  FTM_CDMA2000_RDA2_FRAME_INFO\n", bOk ? "PASS" : "FAIL\n" );

         // Setup the statistics showing for SER log only since SER is the only log that is turned on
         bOk &= QLIB_FTM_CDMA2000_NS_SetStatisticsState( g_hResourceContext, FTM_CDMA2000_NS_Log_RDA2, true );
         bOk &= QLIB_FTM_CDMA2000_NS_SetStatisticsState( g_hResourceContext, FTM_CDMA2000_NS_Log_RDA, true );

         printf("\nEnable the SER statistic reading: %s", bOk ? "PASS" : "FAIL\n" );
      }
      else
      {
         //
         // Start the CDMA2000 NS subystem, this will activate log codes that are needed for
         // sychronization, status, and statistics. If this is not done, the CDMA2000 NS commands
         // will operate in a request/response mode only, and status & statistics will not be updated.
         //

         // Start, with dynamic range of 102.4dB
         bOk = QLIB_FTM_CDMA2000_NS_Start(g_hResourceContext, FTM_CDMA2000_NS_DR_102_4, _dTurnAroundConstant, 0 );
         printf("\nQLIB_FTM_CDMA2000_NS_Start: %s", bOk ? "PASS" : "FAIL\n" );

         // This will enable verbose messages to be generated for RDA parsing. The log type will be LOG_ASYC
         QLIB_ExtendedTextMessage_SetCategoryEnable(
            g_hResourceContext,
            QMSL_ExtTextMsgCat_cdma2000_RDA_Parse,
            TRUE );

         if ( bPerformAcquire )
         {
            /*
            The fast funciton call is more simple to use because most parameters are set to values that will
            work for almost every case.  This single function calls the Pilot, Sync and Traffic acquisition
            functions.
            */
            if ( bUseFastFunctionCall )
            {
               // Quick command to get into traffic
               bOk = QLIB_FTM_CDMA2000_GOTO_FWD_TRAFFIC_STATE(
                        g_hResourceContext,
                        iBand_Class,
                        iCDMA_Freq,
                        _iRadioConfig,
                        iWalsh,
                        iFFPC_Enable );
               printf("\nQLIB_FTM_CDMA2000_GOTO_FWD_TRAFFIC_STATE: %s", bOk ? "PASS" : "FAIL\n" );
            }
            else    // bUseFastFunctionCall
            {
               //
               // Acquire pilot
               //
               if (bOk)
               {
                  unsigned long iACQ_Mode = FTM_CDMA2000_NS_FTM_ACQ_MODE_FULL;
                  unsigned char iFTM_Error_Code = 99;

                  bOk = QLIB_FTM_CDMA2000_PILOT_ACQ_V2(
                           g_hResourceContext,
                           iBand_Class,
                           iCDMA_Freq,
                           iACQ_Mode,
                           &iFTM_Error_Code);

                  printf("\nQLIB_FTM_CDMA2000_PILOT_ACQ_V2: %s, FTM_ERROR: %d", bOk ? "PASS" : "FAIL\n" , iFTM_Error_Code );
               }    // if (bOk)

               //
               // Acquire sync
               //
               if (bOk)
               {
                  unsigned char iFTM_Error_Code = 0;
                  bOk = QLIB_FTM_CDMA2000_DEMOD_SYNC_V2(g_hResourceContext, &iFTM_Error_Code);

                  printf("\nQLIB_FTM_CDMA2000_DEMOD_SYNC: %s, FTM_ERROR: %d", bOk ? "PASS" : "FAIL\n", iFTM_Error_Code );
               }    // if (bOk)

               //
               // Assign forward link FCH
               //
               if (bOk)
               {
                  unsigned long iRadio_Cfg = _iRadioConfig;
                  unsigned char iQOF_Mask_ID = 0;
                  unsigned char iBypass_LC = 0;
                  unsigned char iFrame_Offset = 0;
                  unsigned char iSubChan_Gain = 0;
                  unsigned long iFPC_Mode = 0;
                  unsigned char iInit_Setpt = 0;
                  unsigned char iOLC_Params_Target_FER = 0;
                  unsigned char iOLC_Params_Min_Setpt = 0;
                  unsigned char iOLC_Params_Max_Setpt = 0;
                  unsigned char iFTM_Error_Code = 0;

                  bOk = QLIB_FTM_CDMA2000_DEMOD_FCH_V2(
                           g_hResourceContext,
                           iRadio_Cfg,
                           iWalsh,
                           iQOF_Mask_ID,
                           iBypass_LC,
                           iFrame_Offset,
                           iSubChan_Gain,
                           iFFPC_Enable,
                           iFPC_Mode,
                           iInit_Setpt,
                           iOLC_Params_Target_FER,
                           iOLC_Params_Min_Setpt,
                           iOLC_Params_Max_Setpt,
                           &iFTM_Error_Code);

                  printf("\nQLIB_FTM_CDMA2000_DEMOD_FCH: %s FTM_ERROR: %d", bOk ? "PASS" : "FAIL\n", iFTM_Error_Code );
               }    // if ( bOk )
            }       // bUseFastFunctionCall
            //
            // Assign reverse fundamental
            //
            if (bOk)
            {
               unsigned long iRadio_Cfg = _iRadioConfig;
               unsigned char iFrame_Offset = 0;
               unsigned long iFCH_Rate = FTM_CDMA2000_NS_REV_FCH_FULL_RATE;
               unsigned char iEnable_RPC = 1;       // Enable power control? Y/N
               unsigned short iNum_Preambles = 0;
               unsigned long iTX_Pattern = 0;
               unsigned char iFTM_Error_Code = 0;

               bOk = QLIB_FTM_CDMA2000_MOD_FCH_V2(
                        g_hResourceContext,
                        iRadio_Cfg,
                        iFrame_Offset,
                        iFCH_Rate,
                        iEnable_RPC,
                        iNum_Preambles,
                        iTX_Pattern,
                        &iFTM_Error_Code);

               printf("\nQLIB_FTM_CDMA2000_MOD_FCH: %s FTM_ERROR: %d", bOk ? "PASS" : "FAIL\n", iFTM_Error_Code );
            }   // if (bOk)
         }  // if ( bPerformAcquire )

         /**
         The library must be informed of the RC and Data rate so that it can
         properly calculate SER.  Both items must be specified because this
         function is also used for signaling mode, in which RC is not sent
         via the library function QLIB_FTM_CDMA2000_DEMOD_FCH()
         */
         if ( bOk )
         {
            bOk = QLIB_FTM_CDMA2000_NS_SetSER_RCandRS(
                     g_hResourceContext,
                     (unsigned char) _iRadioConfig,
                     FTM_CDMA2000_NS_REV_FCH_FULL_RATE);

            printf("\nQLIB_FTM_CDMA2000_NS_SetSER_RCandRS: %s", bOk ? "PASS" : "FAIL\n" );
         } // if ( bOk )
      } // if ( cKey == 4 ) BC0, CH %d CDMA 800, using fast function call with minimum logs

      //Calculate the total NS execution time
      GetSystemTime(&stop_systime);
      int ms = ( stop_systime.wHour - start_systime.wHour ) * 60*60*1000 +
               ( stop_systime.wMinute - start_systime.wMinute ) * 60*1000 +
               ( stop_systime.wSecond - start_systime.wSecond ) * 1000 +
               ( stop_systime.wMilliseconds - start_systime.wMilliseconds );

      printf( "\nTotal Execution Time = %d ms ", ms );
      printf( "\n=================================\n" );

      // Print the timer
      QLIB_PrintTimer( g_hResourceContext, _hTimer1, "cdma20000 Non-signaling channel setup time:");

      // Parameters for function that waits for a certain # of logs to be returned
      unsigned long _iAGC_Count = 100;      // iMetric1MinimumCount,
      unsigned long _iInterLogTimeOut_ms = 1000;
      unsigned long _iFrameCount = 140;

      bool _bLoop = true;
      do
      {
         printf("\n------------------------------" );
         printf("\n\n  'R' for a status report,");
         printf("\n  'C' to clear all status");
         printf("\n  'L' to clear SER status only ");
         printf("\n  'S' to start supplemental channel");
         printf("\n  'H' for handover");
         printf("\n  'A' to collect %d AGC packets", _iAGC_Count );
         printf("\n  'F' to collect minimum %d frames using QLIB_FTM_CDMA2000_NS_Get_FCH_SER_FER()", _iFrameCount );
         printf("\n\n  Any other key to release the connection...");
         cKey = toupper( getch() );

         printf("\n------------------------------" );
         if ( cKey == 'C' )
         {
            bOk = QLIB_FTM_CDMA2000_NS_ClearStatus( g_hResourceContext, false );
            printf("\nQLIB_FTM_CDMA2000_NS_ClearStatus: %s", bOk ? "PASS" : "FAIL\n" );
         } //if ( cKey == 'C' )
         else if ( cKey == 'L' )
         {
            bOk = QLIB_FTM_CDMA2000_NS_ClearFER_Status( g_hResourceContext );
            printf("\nQLIB_FTM_CDMA2000_NS_ClearFER_Status: %s", bOk ? "PASS" : "FAIL\n" );
         } //if ( cKey == 'C' )
         else if ( cKey == 'R' )
         {
            // Visual separator

            FTM_CDMA2000_NS_Status_Struct _oStatus;

            bOk = QLIB_FTM_CDMA2000_NS_GetStatus( g_hResourceContext, &_oStatus );
            printf("\nQLIB_FTM_CDMA2000_NS_GetStatus: %s", bOk ? "PASS" : "FAIL\n" );

            printf("\n Searcher State:  %d", _oStatus.eSearcherState );
            printf("\n");
            printf("\n FCH.TotalFrames:  %d", _oStatus.oCDMA2000_FCH_SER_Stats.iSumFrames );
            printf("\n FCH.TotalSymbols:  %d", _oStatus.oCDMA2000_FCH_SER_Stats.iSumSymbols );
            printf("\n FCH.FER:  %6.2f", _oStatus.oCDMA2000_FCH_SER_Stats.dFER );
            printf("\n FCH.SER:  %6.2f", _oStatus.oCDMA2000_FCH_SER_Stats.dSER );
            printf("\n");
            printf("\n SCH.TotalFrames:  %d", _oStatus.oCDMA2000_SCH_SER_Stats.iSumFrames );
            printf("\n SCH.TotalSymbols:  %d", _oStatus.oCDMA2000_SCH_SER_Stats.iSumSymbols );
            printf("\n SCH.FER:  %6.2f", _oStatus.oCDMA2000_SCH_SER_Stats.dFER );
            printf("\n SCH.SER:  %6.2f", _oStatus.oCDMA2000_SCH_SER_Stats.dSER );
            printf("\n");
            printf("\n <<Primary AGC>>");
            printf("\n Record Count: %d", _oStatus.oCDMA2000_AGC_C0_LatestStats.iTotalRecords);
            printf("\n TX AGC CTL:  %6.0fdBm", _oStatus.oCDMA2000_AGC_C0_LatestStats.dTxGainCtl_dBm );
            printf("\n RX AGC:  %6.2fdBm", _oStatus.oCDMA2000_AGC_C0_LatestStats.dRxAGC_dBm );
            printf("\n TX OPEN LOOP:  %6.2fdB", _oStatus.oCDMA2000_AGC_C0_LatestStats.dTxOpenLoop_dBm );

            // these but integer values stored in floating point location, use "%.0f" to print without decimal place,
            printf("\n LNA State:  %.0f", _oStatus.oCDMA2000_AGC_C0_LatestStats.dLNA_State );
            printf("\n PA State:  %.0f", _oStatus.oCDMA2000_AGC_C0_LatestStats.dPA_State );
            printf("\n HDET:  %.0f", _oStatus.oCDMA2000_AGC_C0_LatestStats.dHDET_Raw );

            unsigned char _iBandClass =  0;
            unsigned short _iCurrChan =  0;
            unsigned short _iCDMA_RxState =  0;

            if ( QLIB_DIAG_IS2000_STATUS_F(
                     g_hResourceContext,
                     &_iBandClass,
                     &_iCurrChan,
                     &_iCDMA_RxState ) )
            {
               printf("\n BAND_CLASS:  %d", _iBandClass );
               printf("\n  CURR_CHAN:  %d", _iCurrChan );
            }   // f ( QLIB_DIAG_IS2000_STATUS_F(
         } // cKey == 'R'
         else if ( cKey == 'S' )
         {
            // Visual separator

            unsigned long iRadio_Cfg = FTM_CDMA2000_NS_RC_3;
            unsigned char iWalsh = 3;
            unsigned char iQPF_Mask_ID  = 0;
            unsigned long iSch_Rate = FTM_RATE_8X;
            unsigned long iCoding_Type = FTM_SCH_CODING_CONVOLUTIONAL;
            unsigned char iFFPC_Enable = 1;
            unsigned long iFPC_Mode = 0;
            unsigned char iInit_Setpt = 0;
            unsigned char iOLC_Params_Target_FER = 0;
            unsigned char iOLC_Params_Min_Setpt = 0;
            unsigned char iOLC_Params_Max_Setpt = 0;

            bOk = QLIB_FTM_CDMA2000_DEMOD_FSCH(
                     g_hResourceContext,
                     iRadio_Cfg,
                     iWalsh,
                     iQPF_Mask_ID,
                     iSch_Rate,
                     iCoding_Type,
                     iFFPC_Enable,
                     iFPC_Mode,
                     iInit_Setpt,
                     iOLC_Params_Target_FER,
                     iOLC_Params_Min_Setpt,
                     iOLC_Params_Max_Setpt);

            printf("\nQLIB_FTM_CDMA2000_DEMOD_FSCH: %s", bOk ? "PASS" : "FAIL\n" );
         } // cKey == 'S'
         else if ( cKey == 'H' )
         {
            unsigned long _iBand_Class =  0;
            unsigned short _iCDMA_Freq =  25;
            unsigned char _iEnable_RPC = 1;

            // Get the user input for band class and channel
            printf("\n Band Class Number: ");
            scanf("%d", &_iBand_Class );
            printf("\n Channel Number: ");
            scanf("%d", &_iCDMA_Freq );

            bOk = QLIB_FTM_CDMA2000_FTM_FWD_HHO_SC( g_hResourceContext, _iBand_Class, _iCDMA_Freq, _iEnable_RPC );
            printf("\nQLIB_FTM_CDMA2000_FTM_FWD_HHO_SC( _iBand_Class = %d, _iCDMA_Freq = %d, _iEnable_RPC = %d): %s",
                   _iBand_Class,
                   _iCDMA_Freq,
                   _iEnable_RPC,
                   bOk ? "PASS" : "FAIL\n");
         } //if ( cKey == 'C' )
         else if ( cKey == 'A' )
         {
            // printf("\n  'A' to collect %d AGC packets");

            // Clear the status so that counting starts from zero
            bOk = QLIB_FTM_CDMA2000_NS_ClearStatus( g_hResourceContext,FALSE );
            printf("\nQLIB_FTM_CDMA2000_NS_ClearStatus(): %s", bOk ? "PASS" : "FAIL\n");

            printf("\nWaiting for %d records...", _iAGC_Count );

            bOk = QLIB_FTM_WaitForMetric(
                     g_hResourceContext,
                     QMSL_WaitForMetric_cdma2000_AGC_C0,      // eMetric1Type,
                     QMSL_WaitForMetric_Max,                  // eMetric2Type,
                     _iAGC_Count,                         // iMetric1MinimumCount,
                     0,                                       // iMetric2MinimumCount,
                     _iInterLogTimeOut_ms
                  );
            printf("\nQLIB_FTM_WaitForMetric(): %s", bOk ? "PASS" : "FAIL\n");

            FTM_CDMA2000_NS_Status_Struct _oStatus;

            bOk = QLIB_FTM_CDMA2000_NS_GetStatus( g_hResourceContext, &_oStatus );
            printf("\nQLIB_FTM_CDMA2000_NS_GetStatus: %s", bOk ? "PASS" : "FAIL\n" );

            printf("\n\n\n <<Primary AGC>>");
            printf("\n Record Count: %d", _oStatus.oCDMA2000_AGC_C0_AvgStats.iTotalRecords );
            printf("\n TX AGC CTL:  %6.0fdBm", _oStatus.oCDMA2000_AGC_C0_AvgStats.dTxGainCtl_dBm );
            printf("\n RX AGC:  %6.2fdBm", _oStatus.oCDMA2000_AGC_C0_AvgStats.dRxAGC_dBm );
            printf("\n TX OPEN LOOP:  %6.2fdB", _oStatus.oCDMA2000_AGC_C0_AvgStats.dTxOpenLoop_dBm );
         }  // else if ( cKey == 'A' )
         else if( cKey == 'F' )
         {
            unsigned long iMinimumNumSymbols = 0;
            unsigned long iMinimumNumFrames = _iFrameCount;
            unsigned long piTotalSymbols;
            unsigned long piTotalErrorSymbols;
            unsigned long piTotalFrames;
            unsigned long piTotalFrameErrors;
            double pdSER_Calculated;
            double pdFER_Calculated;
            unsigned long iTimeOut_ms = 5000;

            QLIB_FTM_CDMA2000_NS_Get_FCH_SER_FER( g_hResourceContext,
                                                  iMinimumNumSymbols,
                                                  iMinimumNumFrames,
                                                  &piTotalSymbols,
                                                  &piTotalErrorSymbols,
                                                  &piTotalFrames,
                                                  &piTotalFrameErrors,
                                                  &pdSER_Calculated,
                                                  &pdFER_Calculated,
                                                  iTimeOut_ms);

            printf("\n piTotalSymbols:  %d", piTotalSymbols );
            printf("\n piTotalErrorSymbols:  %d", piTotalErrorSymbols );
            printf("\n piTotalFrames:  %d", piTotalFrames );
            printf("\n piTotalFrameErrors:  %6.2f", piTotalErrorSymbols );
            printf("\n pdSER_Calculated:  %6.2f", pdSER_Calculated );
            printf("\n pdFER_Calculated:  %6.2f", pdFER_Calculated );
            printf("\n===\n");
         }   // else if ( cKey == 'F' )
         else
         {
            _bLoop = false;
         }
      }
      while (_bLoop);

      //
      // Release the channel
      //
      if ( bPerformAcquire )
      {
         bOk = QLIB_FTM_CDMA2000_CMD_RELEASE( g_hResourceContext );
         printf("\nQLIB_FTM_CDMA2000_CMD_RELEASE: %s", bOk ? "PASS" : "FAIL\n" );
      } // if ( bPerformAcquire )

      //
      // Stop the NS subystem, this will deactivate log codes that are used for this mode.
      // If this is not done, they log codes will continue to be generated by the mobile and
      // statistics will continue to be updated.
      //
      bOk = QLIB_FTM_CDMA2000_NS_Stop( g_hResourceContext );
      printf("\nQLIB_FTM_CDMA2000_NS_Stop: %s", bOk ? "PASS" : "FAIL\n" );
   } // if (bOk)

   return bOk ;
}

unsigned char Test_FTM_1XEVDO_NON_SIGNALING( void )
{
   unsigned char bOk=true;
   unsigned char cKey;
   unsigned short _iChannel_BC0 = 387;
   double _dTurnAroundConstant = -73.0;

   // Most locals variables defined in this function that are parameters for QLIB functions do not use "_"
   // so that the function headers can be copied and uses almost as-is
   unsigned long iBand_Class;
   unsigned short iCDMA_Freq;

   printf("\n----------------\nTest_FTM_1xEV-DO_NON_SIGNALING\n----------------\n");

   if ( bOk )
   {
      printf("\n\n1=BC0, CH %d CDMA 800, MAC index 63, rate 4 (307.2kbps)", _iChannel_BC0);
      cKey = getch();

      if ( cKey == '1')
      {
         // Send band & channel parameters
         iBand_Class = FTM_EVDO_BAND_CLASS_CELLULAR;
         iCDMA_Freq = _iChannel_BC0;

         _dTurnAroundConstant = -73.0;
      } // if ( cKey == '1')

      // Timer around the connection functions
      dword _hTimer1 = QLIB_StartTimer(g_hResourceContext );

      //
      // This first part is going into ONLINE, then back to FTM.  This was done
      // to get around problems on the 6500, which could occur when the phone
      // powered up in FTM mode.  Of course the real solution is to fix the embedded
      // software, and this can probably be removed from newer code, but is necessary
      // here for better backwards compatability
      //
      unsigned char _bGoIntoOnlineModeFirst = false;
      if ( _bGoIntoOnlineModeFirst )
      {
         if ( bOk )
         {
            // Go into ONLINE mode
            bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_ONLINE_F );
            printf("\nQLIB_DIAG_CONTROL_F(  MODE_ONLINE_F ): %s", bOk ? "PASS" : "FAIL\n" );
         }

         if ( bOk )
         {
            // Go back into
            bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_FTM_F );
            printf("\nQLIB_DIAG_CONTROL_F(  MODE_FTM_F ): %s", bOk ? "PASS" : "FAIL\n" );
         }

         // Found on the FFA6550, with version 6.3.03, that there must be some time to allow the
         // mobile to fully enter theFTM mode, before sending the logging configuration in the ..._Start()
         // function
         Sleep(250);
      } // _bGoIntoOnlineModeFirst

      //
      // Start the EVDO NS subystem, this will activate log codes that are needed for
      // sychronization, status, and statistics. If this is not done, the EVDO NS commands
      // will operate in a request/response mode only, and status & statistics will not be updated.
      //

      // Start, with dynamic range of 102.4dB
      bOk = QLIB_FTM_EVDO_NS_Start(g_hResourceContext, FTM_EVDO_NS_DR_102_4, _dTurnAroundConstant );
      printf("\nQLIB_FTM_EVDO_NS_Start: %s", bOk ? "PASS" : "FAIL\n" );

      //
      // Acquire pilot
      //
      if (bOk)
      {
         unsigned long iACQ_Mode = 0;
         unsigned long iDiversity_Mode = 0;

         bOk = QLIB_FTM_EVDO_PILOT_ACQ(
                  g_hResourceContext,
                  iBand_Class,
                  iCDMA_Freq,
                  iACQ_Mode,
                  iDiversity_Mode
               );
         printf("\nQLIB_FTM_EVDO_PILOT_ACQ: %s", bOk ? "PASS" : "FAIL\n" );
      } // if (bOk)

      //
      // Acquire system time
      //
      if (bOk)
      {
         QLIB_FTM_EVDO_SYS_TIME_ACQ( g_hResourceContext);

         printf("\nQLIB_FTM_EVDO_SYS_TIME_ACQ: %s", bOk ? "PASS" : "FAIL\n" );
      } // if (bOk)

      //
      // Modulate reverse link access channel
      //
      if (bOk)
      {
         unsigned long iACLC_Mask_I_Hi = 0;
         unsigned long iACLC_Mask_I_Lo = 0;
         unsigned long iACLC_Mask_Q_Hi = 0;
         unsigned long iACLC_Mask_Q_Lo = 0;
         unsigned long iAC_Txpattern = 0xAAAAAAAA;
         unsigned char iAccess_Cycle_Duration = 64;
         unsigned char iOpen_Loop_Adjust = 86;
         unsigned char iProbe_Initial_Adjust = 0;
         unsigned char iProbe_Num_Step = 6;
         unsigned char iPower_Step = 6;
         unsigned char iPreamble_Length = 2;

         bOk = QLIB_FTM_EVDO_MOD_ACC(
                  g_hResourceContext,
                  iACLC_Mask_I_Hi,
                  iACLC_Mask_I_Lo,
                  iACLC_Mask_Q_Hi,
                  iACLC_Mask_Q_Lo,
                  iAC_Txpattern,
                  iAccess_Cycle_Duration,
                  iOpen_Loop_Adjust,
                  iProbe_Initial_Adjust,
                  iProbe_Num_Step,
                  iPower_Step,
                  iPreamble_Length);

         printf("\nQLIB_FTM_EVDO_MOD_ACC: %s", bOk ? "PASS" : "FAIL\n" );
      } // if (bOk)

      //
      // Modulate reverse traffic channel
      //
      if (bOk)
      {
         unsigned char iACK_Gain = 6;
         unsigned char iDataoffset_Nom = 0;
         unsigned char iDataoffset_9K6 = 0;
         unsigned char iDataoffset_19K2 = 0;
         unsigned char iDataoffset_38K4 = 0;
         unsigned char iDataoffset_76K8 = 0;
         unsigned char iDataoffset_153K6 = 0;
         unsigned char iDRC_Value = 4;
         unsigned char iDRC_Cover = 1;
         unsigned char iDRC_Gain = 0;
         unsigned char iDRC_Length = 0;
         unsigned char iDRC_Gating = 0;
         unsigned char iFrame_Offset = 0;
         unsigned char iRPC_Enable = 1;
         unsigned char iRPC_Step = 1;
         unsigned char iRRI = 1;
         unsigned long iRTCLC_Mask_I_Hi = 0;
         unsigned long iRTCLC_Mask_I_Lo = 0;
         unsigned long iRTCLC_Mask_Q_Hi = 0;
         unsigned long iRTCLC_Mask_Q_Lo = 0;
         unsigned long iRTC_Txpattern = 0xAAAAAAAA;
         unsigned char iRTC_Txduty_Cycle = 0xFF;
         unsigned char iHDR_ACK_Mode = FTM_EVDO_IS890_MODE_0;

         bOk = QLIB_FTM_EVDO_MOD_REVERSE_TRA(
                  g_hResourceContext,
                  iACK_Gain,
                  iDataoffset_Nom,
                  iDataoffset_9K6,
                  iDataoffset_19K2,
                  iDataoffset_38K4,
                  iDataoffset_76K8,
                  iDataoffset_153K6,
                  iDRC_Value,
                  iDRC_Cover,
                  iDRC_Gain,
                  iDRC_Length,
                  iDRC_Gating,
                  iFrame_Offset,
                  iRPC_Enable,
                  iRPC_Step,
                  iRRI,
                  iRTCLC_Mask_I_Hi,
                  iRTCLC_Mask_I_Lo,
                  iRTCLC_Mask_Q_Hi,
                  iRTCLC_Mask_Q_Lo,
                  iRTC_Txpattern,
                  iRTC_Txduty_Cycle,
                  iHDR_ACK_Mode );

         printf("\nQLIB_FTM_EVDO_MOD_REVERSE_TRA: %s", bOk ? "PASS" : "FAIL\n" );
      } // if (bOk)

      //
      // Demodulate forward link
      //
      if (bOk)
      {
         unsigned char iARQ_Enable      = 1;
         unsigned char iDRC_Lockperiod  = 1;
         unsigned char iDRC_Locklength  = 1;
         unsigned char iMAC_Index       = 63;
         unsigned char iRAB_Index       = 0;
         unsigned char iRAB_Offset      = 0;

         bOk = QLIB_FTM_EVDO_DEMOD_CC_MAC_FTC(
                  g_hResourceContext,
                  &iARQ_Enable,
                  &iDRC_Lockperiod,
                  &iDRC_Locklength,
                  &iMAC_Index,
                  &iRAB_Index,
                  &iRAB_Offset);

         printf("\nQLIB_FTM_EVDO_DEMOD_FORWARD_CC_MAC_FTC: %s", bOk ? "PASS" : "FAIL\n" );
      } // if (bOk)

      //
      // Activate measurement logs
      //
      if (bOk)
      {
         bOk = QLIB_FTM_EVDO_NS_EnableMeasurementLogs(
                  g_hResourceContext,
                  FTM_EVDO_NS_LOG_Default // Default setting includes all logs
               );

         printf("\nQLIB_FTM_EVDO_NS_EnableMeasurementLogs: %s", bOk ? "PASS" : "FAIL\n" );
      } // if (bOk)

      // Print the timer
      QLIB_PrintTimer( g_hResourceContext, _hTimer1, "1xEV0-DO Non-signaling channel setup time:");

      do
      {
         printf("\n\nPerform testing then press 'R' for a status report\n 'C' to clear all status \n'L' to clear PER status only \n or any key to release the connection...");
         cKey = toupper( getch() );
         if ( cKey == 'C' )
         {
            bOk = QLIB_FTM_EVDO_NS_ClearStatus( g_hResourceContext );
            printf("\nQLIB_FTM_EVDO_NS_ClearStatus: %s", bOk ? "PASS" : "FAIL\n" );
         } //if ( cKey == 'C' )

         else if ( cKey == 'L' )
         {
            bOk = QLIB_FTM_EVDO_NS_ClearPER_Status( g_hResourceContext );
            printf("\nQLIB_FTM_EVDO_NS_ClearPER_Status: %s", bOk ? "PASS" : "FAIL\n" );
         } //if ( cKey == 'L' )

         else if ( cKey == 'R' )
         {
            // Visual separator
            printf("\n------------------------------" );

            // Get PER, this is part of the larger status structure, but can be
            // retrived with a special function
            unsigned long _iMinimumNumPackets = 0;  // Set to 0 to return instantaneously
            unsigned long _iNumPackets;
            double _dPER;
            unsigned long _iTimeOut_ms = 0;         // Not used if iMinimumNumPackets==0

            bOk = QLIB_FTM_EVDO_NS_GetPER( g_hResourceContext, _iMinimumNumPackets, &_iNumPackets, &_dPER, _iTimeOut_ms );
            printf("\nQLIB_FTM_EVDO_NS_GetPER: %s", bOk ? "PASS" : "FAIL\n" );
            if (bOk)
               printf("\n      iNumPackets = %d, dPER = %6.2f", _iNumPackets, _dPER);

            //
            // Example of getting the entire status structure
            //

            // Declare a status structure
            FTM_EVDO_NS_Status_Struct _oStatus;

            bOk = QLIB_FTM_EVDO_NS_GetStatus( g_hResourceContext, (void*)&_oStatus );
            printf("\nQLIB_FTM_EVDO_NS_GetStatus: %s", bOk ? "PASS" : "FAIL\n" );

            if (bOk )
            {
               printf("\n  Searcher State: %d", _oStatus.eSearcherState );

               // Print the average C2I
               printf("\n\n  C2I Count: %d", _oStatus.oEVDO_LatestCtoI_Stats.iTotalReports );
               printf("\n  CurrentC2I: %6.2fdB", _oStatus.oEVDO_LatestCtoI_Stats.dC2I_dB);
               printf("\n  Current C0 C2I: %6.2fdB", _oStatus.oEVDO_LatestCtoI_Stats.dC2I_C0_dB);
               printf("\n  Current C1 C2I: %6.2fdB", _oStatus.oEVDO_LatestCtoI_Stats.dC2I_C1_dB);

               // Print the latest AGC statistics
               printf("\n\n  AGC C0 Count: %d", _oStatus.oEVDO_AGC_C0_LatestStats.iTotalRecords );
               printf("\n\n  AGC C1 Count: %d", _oStatus.oEVDO_AGC_C1_LatestStats.iTotalRecords );
               printf("\n  C0 RX AGC: %6.1fdBm", _oStatus.oEVDO_AGC_C0_LatestStats.dRxAGC_dBm  );
               printf("\n  C1 RX AGC: %6.1fdBm", _oStatus.oEVDO_AGC_C1_LatestStats.dRxAGC_dBm  );
               printf("\n  TX AGC: %6.2fdBm", _oStatus.oEVDO_AGC_C0_LatestStats.dTxGainCtl_dBm );
               printf("\n  C0 LNA State: %.0f", _oStatus.oEVDO_AGC_C0_LatestStats.dLNA_State );
               printf("\n  C1 LNA State: %.0f", _oStatus.oEVDO_AGC_C1_LatestStats.dLNA_State );
               printf("\n  PA State: %.0f", _oStatus.oEVDO_AGC_C0_LatestStats.dPA_State );
               printf("\n  HDET State: %.0f", _oStatus.oEVDO_AGC_C0_LatestStats.dHDET_Raw );
            } // if (bOk )
         } // cKey == 'R'
      }
      while ((cKey == 'R') ||(cKey == 'C')||(cKey == 'L'));

      //
      // Release the channel
      //
      bOk = QLIB_FTM_EVDO_CMD_RELEASE( g_hResourceContext );
      printf("\nQLIB_FTM_EVDO_CMD_RELEASE: %s", bOk ? "PASS" : "FAIL\n" );

      //
      // Stop the EVDO NS subystem, this will deactivate log codes that are used for this mode.
      // If this is not done, they log codes will continue to be generated by the mobile and
      // statistics will continue to be updated.
      //
      bOk = QLIB_FTM_EVDO_NS_Stop( g_hResourceContext );
      printf("\nQLIB_FTM_EVDO_NS_Stop: %s", bOk ? "PASS" : "FAIL\n" );
   } // if (bOk)

   return bOk ;
}

void PrintWCDMA_AGC( void )
{
   char cKey = '\0';

   printf("\nCapturing...update every 500ms press 'Q' to quit\n");

   // Activate AGC logging
   unsigned char _bOk = QLIB_FTM_WCDMA_AGC_Start_Log( g_hResourceContext );
   printf("\n QLIB_FTM_WCDMA_AGC_Start_Log = %s", _bOk ? "PASS" : "FAIL" );

   printf("\nRX_AGC\tTX_AGC\tTX_ADJ\tLNA\tPA\tHDET\tTadc\tTstate\tTindex\tTremainder");
   printf("\n------\t------\t------\t---\t--\t----\t-----\t------\t-----\t----------");
   // Wait for keypress on PC
   while ( true )
   {
      // Storage for WCDMA AGC structure
      FTM2LogMessage_WCDMA_AGC _oWCDMA_AGC;

      // Default timeout
      const dword c_iDefaultWait_ms = 500;

      // Look for WCDMA AGC logs
      if ( QLIB_FTM_WCDMA_AGC_WaitForNextLog( g_hResourceContext, &_oWCDMA_AGC, c_iDefaultWait_ms ) )
      {
         // Print results
         /*
         typedef struct
         {
         // Portion unique to FTM logs
         unsigned short iFTM_LogID; //!<' Log ID
         unsigned short iRX_AGC;
         unsigned short iTX_AGC;
         unsigned short iTx_AGC_ADJ;
         unsigned short iLNA_STATE;
         unsigned short iPA_STATE;
         unsigned short iHDET;
         unsigned short iThemistor;
         unsigned short iScaledThermistor;
         unsigned short iTempCompIndex;
         unsigned short iTempCompRemainder;
         } FTM2LogMessage_WCDMA_AGC;

         */
         printf("\n%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d --  Press Q to exit",
                _oWCDMA_AGC.iRX_AGC,
                _oWCDMA_AGC.iTX_AGC,
                _oWCDMA_AGC.iTx_AGC_ADJ,
                _oWCDMA_AGC.iLNA_STATE,
                _oWCDMA_AGC.iPA_STATE,
                _oWCDMA_AGC.iHDET,
                _oWCDMA_AGC.iThemistor,
                _oWCDMA_AGC.iScaledThermistor,
                _oWCDMA_AGC.iTempCompIndex,
                _oWCDMA_AGC.iTempCompRemainder );
      } // if ( _pFTM2_Message->iFTM_LogID == LOG_FTM2_LOG_WCDMA_AGC )

      if ( kbhit() )
      {
         cKey = toupper(getch());

         if ( cKey == 'Q')
            break;
      }

      // Pause 400ms for another measurement
      Sleep( 500);
   }    // while ( !kbhit() )

   if ( kbhit() )
   {
      // Flush the PC's keypress buffer
      getch();
      printf("\n\n");
   }

   // Stop AGC logging
   _bOk = QLIB_FTM_WCDMA_AGC_Stop_Log( g_hResourceContext );
   printf("\n QLIB_FTM_WCDMA_AGC_Stop_Log = %s", _bOk ? "PASS" : "FAIL" );
}

unsigned char Test_FTM_Sequencer( void )
{
   unsigned short ms = 1000; // 1000 us = 1ms;
   char key = '0';
   char* sLogFile = "C:\\ftm_sequencer_unit_test.txt";

   while(key != 'Q')
   {
      printf("\n Test_FTM_Sequencer\n");
      printf("=====================\n");
      printf(" 1 - FTM Sequencer Unit Test: Tx Sweep PDM 80, step 2, # steps = 10 (AMSS Mode)\n");
      printf(" 2 - FTM Sequencer Unit Test CDMA2000 non-signaling \n");
      printf(" 3 - FTM Sequencer Unit Test WCDMA non-signaling \n");
      printf(" 4 - FTM Sequencer Unit Test EDGE non-signaling \n");
      printf(" 5 - FTM Sequencer Unit Test EVDO (Rev A)  non-signaling \n");
      printf(" 6 - FTM Sequencer Unit Test LTE  non-signaling \n");
      printf(" 8 - 1k support for EFS NV RF items \n");
      printf(" 9 - GSM Rx RSSI Calibration with GSM Tx Trigger\n");
      printf(" A - Perform only status + sequence execution\n");
      printf(" B - FTM Sequencer Stress Test using Set PDM Command\n");
      printf(" C - FTM Sequencer for GSM DA Cal\n");

      printf(" Q - Quit\n");
      printf(" -->");

      key = toupper(getch());

      unsigned short iFTM_Error_Code = 0;
      unsigned long iQueue_Item_Count = 0;
      unsigned char  bIsExecuting = 0;
      unsigned long iNext_ID_Number = 0;
      unsigned long iTimeRef = 0;
      unsigned char iStatus = 0;
      unsigned char iTimeOutOcurred = 0;

      if(key == 'Q')
      {
         break;
      }
      else if (key == '1')
      {
         int _steps = 10;
         int k;
         unsigned short *_HDET = new unsigned short[_steps]; // HDET array
         unsigned short *_PDM  = new unsigned short[_steps]; // PDM array
         unsigned short _HDET_ADC = 25;
         unsigned char bIsExecuting;
         unsigned short _iUserVersion = 1;

         QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\nLogging to file %s\n\n", sLogFile );

         QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_CDMA_800 );
         QLIB_FTM_SET_CHAN( g_hResourceContext, 387 );
         QLIB_FTM_SET_TX_ON( g_hResourceContext );
         QLIB_FTM_SET_PA_RANGE( g_hResourceContext, 0 );
         QLIB_FTM_SET_PDM(g_hResourceContext, 2, 0 );

         QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );
         QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );

         //QLIB_FTM_SEQ_STATUS( g_hResourceContext, & iFTM_Error_Code, & iQueue_Item_Count, &bIsExecuting, &iNext_ID_Number, & iTimeRef);

         //printf("Pre-execution Status 0");
         //printf("iQueue_Item_Count = %d, bIsExecuting = %d\n", iQueue_Item_Count, bIsExecuting);

         for(k = 0; k < _steps; k++)
         {
            _PDM[k] = 80 - (k*2);
            QLIB_FTM_SET_PDM(g_hResourceContext, 2, _PDM[k]);
            printf("QLIB_FTM_SET_PDM( iPDMvalue = %d )\n", _PDM[k]);
            QLIB_FTM_SEQ_ADD_WAIT(g_hResourceContext, 10*ms, &iFTM_Error_Code);
            QLIB_FTM_GET_ADC_VAL(g_hResourceContext, _HDET_ADC, &_HDET[k]);
         }

         QLIB_FTM_SET_TX_OFF( g_hResourceContext);

         QLIB_FTM_SEQ_Flush_Command_Buffer( g_hResourceContext );

         iStatus = QLIB_FTM_SEQ_STATUS( g_hResourceContext, & iFTM_Error_Code, & iQueue_Item_Count, &bIsExecuting, &iNext_ID_Number, & iTimeRef);
         printf("\nPre-execution Status\n");
         printf("QLIB_FTM_SEQ_STATUS: Status= %d, iQueue_Item_Count = %d, bIsExecuting = %d\n\n", iStatus, iQueue_Item_Count, bIsExecuting);

         QLIB_FTM_SEQ_CreateSequenceFile(g_hResourceContext, "c:\\FTM_TX_SWEEP.bin", _iUserVersion );

         QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred);

         iStatus = QLIB_FTM_SEQ_STATUS( g_hResourceContext, & iFTM_Error_Code, & iQueue_Item_Count, &bIsExecuting, &iNext_ID_Number, & iTimeRef);
         printf("Post-execution Status\n");
         printf("QLIB_FTM_SEQ_STATUS: Status= %d, iQueue_Item_Count = %d, bIsExcuting = %d\n\n", iStatus, iQueue_Item_Count, bIsExecuting);

         QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iFTM_Error_Code);

         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         QLIB_StopLogging( g_hResourceContext );

         // print HDET value after the commands on the queue have been executed
         for(k = 0; k < _steps; k++)
            printf("PDM = %d, HDET value = %d\n", _PDM[k], _HDET[k]);

         delete [] _HDET;
         delete [] _PDM;
      }
      else if (key == '2')
      {
         unsigned char bOk;

         unsigned char _iRxLevelStatus = 0;
         short _iRxAGC = 0;
         short _iRxPwrdBm10 = 0;

         unsigned long _iNumFrame =0;
         unsigned long _iFrameError = 0;
         double _dFER = 0;
         unsigned long _iNumSymbol = 0;
         unsigned long _iSymbolError = 0;
         unsigned char _iStatus = 0;
         double _dSER = 0;

         unsigned short iADD_WAIT_Status = 0;

         unsigned char _bEnableFER_SER = 1;     // read FER/SER
         unsigned short _iChannel = 387;
         unsigned char _iBC = 0;
         unsigned char _iRC = 1;
         unsigned char _iWalsh = 10;
         unsigned char _EnableRPC = 1;

         // Setup for using "small waits", in which the PC side transforms a single ADD_WAIT into multiple
         // embedded ADD_WAITS. This deals with a situation where the embedded rex_sleep() takes more time
         // than it should, so segments are delayed. This is a problem in Jan/Feb 2011 that is fixed in Mar 2011
         unsigned char c_bUseSmallWaits = false;

         const int c_iLoopMax = 10; // Loop iterations for alternating mid-high power
         const unsigned long c_iSegmentLength = 20; // ms

         QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("Logging to file %s", sLogFile );

         QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );
         QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );

         QLIB_FTM_CDMA2000_GOTO_FWD_TRAFFIC_STATE(g_hResourceContext, _iBC, _iChannel, _iRC, _iWalsh, 0);

         QLIB_FTM_CDMA2000_MOD_FCH(g_hResourceContext, _iRC, 0, 0, 1 ,0 ,0);

         QLIB_FTM_CDMA2000_SET_REV_PWR(g_hResourceContext, _EnableRPC, 240);

         QLIB_FTM_CDMA2000_RX_LEVEL_REQ(g_hResourceContext, 0, &_iRxLevelStatus, &_iRxAGC, &_iRxPwrdBm10);

         if ( _bEnableFER_SER )
         {
            QLIB_FTM_CDMA2000_RESET_FCH_STAT(g_hResourceContext);
         }
         ////////
         // Loop through segments
         ///////
         printf("\n\nSetting up through %d sets of alternating High/Low segments, of %dms in duration each",
                c_iLoopMax,
                static_cast<int>(c_iSegmentLength)
               );

         for (int iLoop =0; iLoop < c_iLoopMax; iLoop ++ )
         {
            bOk &= Test_FTM_Sequencer_AddWait(
                      c_iSegmentLength, // unsigned long iWaitPeriod,
                      c_bUseSmallWaits,
                      &iADD_WAIT_Status);

            bOk &= QLIB_FTM_CDMA2000_SET_REV_PWR(g_hResourceContext, 1, -200);

            bOk &= Test_FTM_Sequencer_AddWait(
                      c_iSegmentLength, // unsigned long iWaitPeriod,
                      c_bUseSmallWaits,
                      &iADD_WAIT_Status);

            bOk &= QLIB_FTM_CDMA2000_SET_REV_PWR(g_hResourceContext, 1, 200);
         }

         ///////
         // Read FER/SER Status
         ////////
         if ( _bEnableFER_SER )
         {
            QLIB_FTM_CDMA2000_GET_FCH_STAT(g_hResourceContext, &_iNumFrame, &_iFrameError, &_dFER, &_iNumSymbol, &_iSymbolError, &_dSER);
         }

         QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         _dFER = (double) _iFrameError/_iNumFrame * 100;
         _dSER = (double) _iSymbolError/_iNumSymbol * 100;

         // print resuslt from SER/FER status, Rx Level
         printf("RxLevl (dBm*10) = %d\n", _iRxPwrdBm10);
         printf("iNumFrame=%d, iFrameError=%d, dFER=%6.2f, iNumSymbol=%d, iSymbolError=%d, dSER=%6.2f\n", _iNumFrame, _iFrameError, _dFER, _iNumSymbol, _iSymbolError, _dSER);

         QLIB_FTM_SEQ_CLEAR_QUEUE(g_hResourceContext, & iFTM_Error_Code);

         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         QLIB_StopLogging( g_hResourceContext );
      }
      else if (key == '3')  // WCDMA
      {
         /*
         Equipment Setup
         WCDMA
         UL = 9612
         DL = 10562
         CPICH Primary scrambling code = 0
         DPCH = 9
         DPCH scrambling code = 0
         DPCH Data = PN9
         */
         unsigned short ms = 1000;

         // Pilot acquire parameter
         unsigned char iAcqType= FTM_WCDMA_BER_AcqTypeFreqOnly;
         unsigned char iAcqMode= FTM_WCDMA_BER_AcqModeFull;
         unsigned short iFreq= 10562;
         unsigned short iScrCode= 0;
         unsigned long iPN_Pos= 0;

         //
         // RMC DMC channel setup (Handover) paramters
         //

         // alt settings: { FTM_WCDMA_BER_RMC_Type64kbps, FTM_WCDMA_BER_RMC_Type12_2kpbs }
         unsigned char iRMC_Type= FTM_WCDMA_BER_RMC_Type12_2kpbs;

         unsigned char iUL_MaxTxPwr= 24;
         unsigned long iUL_DPCH_ScrCode= 0;
         unsigned char iUL_PCA= FTM_WCDMA_BER_PCA_Type_Alg1;
         unsigned char iUL_TPC_StepSize= FTM_WCDMA_BER_PCA_Size_1dB;
         unsigned short iDL_PriCPICH_Scr_Code= 0;
         unsigned short iDL_ChanCode= 9;
         unsigned char iBetaC= 8;
         unsigned char iBetaD= 15;
         short iDPCCH_PowerOffset= -60;
         unsigned char n_312= FTM_WCDMA_BER_N312_s1;
         unsigned char n_313= FTM_WCDMA_BER_N313_s20;
         unsigned char n_315= FTM_WCDMA_BER_N315_s1;
         unsigned char t_312= 1;
         unsigned char t_313= 3;
         unsigned short iChannel= 10562;

         // Function return values
         unsigned char bOk = TRUE;

         // Command status
         unsigned short iCLEAR_QUEUE_Status = DIAG_FTM_STATUS_FAIL;
         unsigned char iAcquire_Status = DIAG_FTM_STATUS_FAIL;
         unsigned char iHandOver_Status = DIAG_FTM_STATUS_FAIL;
         unsigned char iStop_Mode_Status = DIAG_FTM_STATUS_FAIL;
         unsigned char iStart_Mode_Status = DIAG_FTM_STATUS_FAIL;
         unsigned short iClear_WCDMA_Events_Status = DIAG_FTM_STATUS_FAIL;
         unsigned short iSET_UL_POWER_Status = DIAG_FTM_STATUS_FAIL;
         unsigned short iADD_TIMING_MARKER_Status = DIAG_FTM_STATUS_FAIL;
         unsigned short iADD_WAIT_Status = DIAG_FTM_STATUS_FAIL;
         unsigned short iSE_BER_CLEAR_Status = DIAG_FTM_STATUS_FAIL;
         unsigned short iSE_BER_GET_Status = DIAG_FTM_STATUS_FAIL;

         // Synchronous event status
         unsigned short iStart_Mode_Done_Status = FTM_WCDMA_BER_SYNC_STATUS_GEN_FAILURE;
         unsigned short iAcquire_Done_Status = FTM_WCDMA_BER_SYNC_STATUS_GEN_FAILURE;
         unsigned short iPHYCHAN_ESTABLISHED_Status = FTM_WCDMA_BER_SYNC_STATUS_GEN_FAILURE;
         unsigned short iCPHY_SETUP_Status = FTM_WCDMA_BER_SYNC_STATUS_GEN_FAILURE;
         unsigned short iWCDMA_BER_STOP_Status = FTM_WCDMA_BER_SYNC_STATUS_GEN_FAILURE;

         // Other return variables
         unsigned long iTotal_Bits = 0;
         unsigned long iTotal_Bit_Errors = 0;
         double dBER = 0.0;

         // Flag to use ADD_TIMING_MARKER, which is not available on targets before March 2011
         // Timing markers allow the DUT to adjust the sequence timeline, to support a variable-length
         // acquisition time.  Without the this feature a fixed size delay must be used to account for the
         // worst case embedded acquisition time.
         unsigned char _bUseTimingMarker = false;

         // Flag to use SE BER
         unsigned char _bEnableSE_BER = true;

         // Flag to enable F3 messages
         unsigned char _bEnableF3Messages= false;

         // Setup for using "small waits", in which the PC side transforms a single ADD_WAIT into multiple
         // embedded ADD_WAITS. This deals with a situation where the embedded rex_sleep() takes more time
         // than it should, so segments are delayed. This is a problem in Jan/Feb 2011 that is fixed in Mar 2011
         unsigned char c_bUseSmallWaits = false;

         const int c_iLoopMax = 10; // Loop iterations for alternating mid-high power
         const unsigned long c_iSegmentLength = 20; // ms
         const short c_iPowerHigh = 240;     // dBm * 10
         const short c_iPowerMid = 100;  // dBm * 10
         const short c_iPowerMin = -500;     // dBm * 10

         printf("\nExecuting WCDMA NS Sequence on DL channel %d", iFreq );
         printf("\n  Variables:");
         printf("\n             Loop count: %d", c_iLoopMax );
         printf("\n         Segment length: %dms", c_iSegmentLength );
         printf("\n             High power: %d dBm*10", c_iPowerHigh );
         printf("\n              Mid power: %d dBm*10", c_iPowerMid );
         printf("\n              Low power: %d dBm*10", c_iPowerMin );
         printf("\n      Use TIMING_MARKER: %s", _bUseTimingMarker ? "YES" : "NO" );
         printf("\n          Enable SE BER: %s", _bEnableSE_BER ? "YES" : "NO" );
         printf("\n    Use small ADD_WAITs: %s", c_bUseSmallWaits ? "YES" : "NO" );

         // Start text log
         bOk &= QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\n\nLogging to file %s", sLogFile );

         if ( _bEnableF3Messages )
         {
            // Activate FTM F3 Messages
            bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, 23, 0xFF );
         }

         // Select embedded sequence mode
         bOk &= QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );

         // Enter the batch mode, to queue up subsequent FTM commands
         bOk &= QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );

         // Clear the embedded queue
         bOk &= QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iCLEAR_QUEUE_Status );

         // Clear WCDMA events
         bOk &= QLIB_FTM_WCDMA_BER_EVENT_STATUS_CLEAR_REQ(g_hResourceContext, &iClear_WCDMA_Events_Status);

         // Start WCDMA mode
         bOk &= QLIB_FTM_WCDMA_START_MODE_REQ(g_hResourceContext, &iStart_Mode_Status);

         // Wait for start event
         bOk &= QLIB_FTM_WCDMA_BER_EVENT_STATUS_REQ(g_hResourceContext, FTM_WCDMA_BER_START_WCDMA_MODE_CNF, 500*ms, &iStart_Mode_Done_Status);

         // Acquire Pilot
         bOk &= QLIB_FTM_WCDMA_ACQUIRE_REQ( g_hResourceContext,iAcqType, iAcqMode, iFreq, iScrCode, iPN_Pos, &iAcquire_Status );

         // Wait for Pilot acquire
         bOk &= QLIB_FTM_WCDMA_BER_EVENT_STATUS_REQ(g_hResourceContext, FTM_WCDMA_BER_ACQ_CNF, 500*ms, &iAcquire_Done_Status);

         // Setup DCH
         bOk &= QLIB_FTM_WCDMA_RMC_DCH_SETUP_REQ_V2B(    g_hResourceContext,
                                                         iRMC_Type,
                                                         iUL_MaxTxPwr,
                                                         iUL_DPCH_ScrCode,
                                                         iUL_PCA,
                                                         iUL_TPC_StepSize,
                                                         iDL_PriCPICH_Scr_Code,
                                                         iDL_ChanCode,
                                                         iBetaC,
                                                         iBetaD,
                                                         iDPCCH_PowerOffset,
                                                         n_312,
                                                         n_313,
                                                         n_315,
                                                         t_312,
                                                         t_313,
                                                         iChannel,
                                                         &iHandOver_Status);

         // Wait for DCH events
         bOk &= QLIB_FTM_WCDMA_BER_EVENT_STATUS_REQ(g_hResourceContext, FTM_WCDMA_PHYCHAN_ESTABLISHED_IND, 500*ms, &iPHYCHAN_ESTABLISHED_Status);
         bOk &= QLIB_FTM_WCDMA_BER_EVENT_STATUS_REQ(g_hResourceContext, FTM_WCDMA_BER_CPHY_SETUP_CNF, 500*ms, &iCPHY_SETUP_Status);

         if ( _bUseTimingMarker )
         {
            bOk &= QLIB_FTM_SEQ_ADD_TIMING_MARKER( g_hResourceContext, &iADD_TIMING_MARKER_Status );

            // Issue: max power cannot be set immediately after DCH setup. These commands force a 20ms wait
            bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, 20*ms, &iADD_WAIT_Status );

            // Set WCDMA UL power to min
            bOk &= QLIB_FTM_WCDMA_BER_SET_UL_POWER(
                      g_hResourceContext,
                      TRUE,        // bOverride,
                      c_iPowerMin,     // iUL_Pwr_dBm10,
                      &iSET_UL_POWER_Status );
         }
         else
         {
            // Set WCDMA UL power to min
            bOk &= QLIB_FTM_WCDMA_BER_SET_UL_POWER(
                      g_hResourceContext,
                      TRUE,        // bOverride,
                      c_iPowerMin,     // iUL_Pwr_dBm10,
                      &iSET_UL_POWER_Status );

            Test_FTM_Sequencer_AddWait(
               400, // unsigned long iWaitPeriod,
               c_bUseSmallWaits,
               &iADD_WAIT_Status);
         }

         // Set WCDMA UL power to min
         bOk &= QLIB_FTM_WCDMA_BER_SET_UL_POWER(
                   g_hResourceContext,
                   TRUE,       // bOverride,
                   240,        // iUL_Pwr_dBm10,
                   &iSET_UL_POWER_Status );

         if ( _bEnableSE_BER )
         {
            // Activate SE BER state machine
            bOk &=  QLIB_FTM_SE_BER_CONFIGURE_REQUEST( g_hResourceContext,
                                                       1,    // PN9 unsigned short iPnSeqType, // *FIX* user input?
                                                       0,    // unsigned char bPnInverted,
                                                       20,   // unsigned short iAcqThreshold,
                                                       30 ); // unsigned short iAcqLostWm );

            // Clear the SE BER Accumulators
            bOk &=  QLIB_FTM_SE_BER_CLEAR_SYNC_STATUS( g_hResourceContext, &iSE_BER_CLEAR_Status );
         }

         ////////
         // Loop through segments
         ///////
         printf("\n\nSetting up through %d sets of alternating High/Low segments, of %dms in duration each",
                c_iLoopMax,
                c_iSegmentLength
               );

         for (int iLoop =0; iLoop < c_iLoopMax; iLoop ++ )
         {
            bOk &= Test_FTM_Sequencer_AddWait(
                      c_iSegmentLength, // unsigned long iWaitPeriod,
                      c_bUseSmallWaits,
                      &iADD_WAIT_Status);

            bOk &= QLIB_FTM_WCDMA_BER_SET_UL_POWER(
                      g_hResourceContext,
                      TRUE,        // bOverride,
                      -200,        // iUL_Pwr_dBm10,
                      &iSET_UL_POWER_Status );

            bOk &= Test_FTM_Sequencer_AddWait(
                      c_iSegmentLength, // unsigned long iWaitPeriod,
                      c_bUseSmallWaits,
                      &iADD_WAIT_Status);

            bOk &= QLIB_FTM_WCDMA_BER_SET_UL_POWER(
                      g_hResourceContext,
                      TRUE,        // bOverride,
                      100,     // iUL_Pwr_dBm10,
                      &iSET_UL_POWER_Status );
         }

         ///////
         // Read SE BER Status
         ////////
         if ( _bEnableSE_BER )
         {
            // Clear the SE BER results
            bOk &=  QLIB_FTM_SE_BER_GET_SYNC_STATUS (
                       g_hResourceContext,
                       &iTotal_Bits,
                       &iTotal_Bit_Errors,
                       &dBER,
                       &iSE_BER_GET_Status);
         }

         ///////
         // Execute sequence
         ////////
         bOk &= QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         // Turn off sequencer
         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         ///////
         // Print results
         ////////

         // Function return values
         printf("\n    Function return status: %s", bOk  ? "PASS" : "FAIL" );
         // Response status
         printf("\n       iCLEAR_QUEUE_Status: %s", iCLEAR_QUEUE_Status        == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
         printf("\n        iStart_Mode_Status: %s", iStart_Mode_Status         == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
         printf("\niClear_WCDMA_Events_Status: %s", iClear_WCDMA_Events_Status == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
         printf("\n           iAcquire_Status: %s", iAcquire_Status            == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
         printf("\n          iHandOver_Status: %s", iHandOver_Status           == DIAG_FTM_STATUS_SUCCESS ? "PASS" : "FAIL" );
         if ( _bUseTimingMarker ) printf("\n iADD_TIMING_MARKER_Status: %s", iADD_TIMING_MARKER_Status == DIAG_FTM_STATUS_SUCCESS ? "PASS" : "FAIL" );
         printf("\n          iADD_WAIT_Status: %s", iADD_WAIT_Status           == DIAG_FTM_STATUS_SUCCESS ? "PASS" : "FAIL" );
         printf("\n      iSET_UL_POWER_Status: %s", iSET_UL_POWER_Status       == DIAG_FTM_STATUS_SUCCESS ? "PASS" : "FAIL" );
         if (_bEnableSE_BER)
         {
            printf("\n      iSE_BER_CLEAR_Status: %s", iSE_BER_CLEAR_Status == DIAG_FTM_STATUS_SUCCESS ? "PASS" : "FAIL" );
            printf("\n        iSE_BER_GET_Status: %s", iSE_BER_GET_Status == DIAG_FTM_STATUS_SUCCESS ? "PASS" : "FAIL" );
         } // if (_bEnableSE_BER)

         // Synchronous Status
         printf("\n      iAcquire_Done_Status: %s (%d)", iAcquire_Done_Status == FTM_WCDMA_BER_SYNC_STATUS_SUCCESS ? "PASS" : "FAIL",iAcquire_Done_Status );
         printf("\n      iPHYCHAN_ESTABLISHED_Status: %s (%d)", iPHYCHAN_ESTABLISHED_Status == FTM_WCDMA_BER_SYNC_STATUS_SUCCESS ? "PASS" : "FAIL",iPHYCHAN_ESTABLISHED_Status );
         printf("\n        iCPHY_SETUP_Status: %s (%d)", iCPHY_SETUP_Status == FTM_WCDMA_BER_SYNC_STATUS_SUCCESS ? "PASS" : "FAIL", iCPHY_SETUP_Status );

         if (_bEnableSE_BER)
         {
            printf("\n SE BER results");
            printf("\n                Total Bits: %d", iTotal_Bits );
            printf("\n          Total Bit Errors: %d", iTotal_Bit_Errors );
            printf("\n                       BER: %d", dBER );
         }  // if (_bEnableSE_BER)

         ///////
         // Stop WCDMA
         ////////
         printf("\n\n>>Please check the WCDMA Tx power and press any key to stop the WCDMA NS mode<<");
         getch();

         bOk &= QLIB_FTM_WCDMA_STOP_MODE_REQ(g_hResourceContext, &iStop_Mode_Status);
         bOk &= QLIB_FTM_WCDMA_BER_EVENT_STATUS_REQ(g_hResourceContext, FTM_WCDMA_BER_STOP_WCDMA_MODE_CNF, 500*ms, &iWCDMA_BER_STOP_Status);
         printf("\n    iWCDMA_BER_STOP_Status: %s (%d)", iWCDMA_BER_STOP_Status == FTM_WCDMA_BER_SYNC_STATUS_SUCCESS ? "PASS" : "FAIL", iWCDMA_BER_STOP_Status );
         printf("\n");

         QLIB_StopLogging( g_hResourceContext );
      } // end of WCDMA
      else if (key == '4')      // EDGE VFS
      {
         /*
         Equipment setup
         GSM900, Channel 128, Mobile Rx = 941.4MHz, Mobile Tx = 896.4MHz
         DL slots = 4, 5
         UL slots = 4, 5
         DL MCS = MCS9
         DL Data = PN9

         */
         // Millisecond conversion
         unsigned long ms = 1000;

         // Function return values
         unsigned char bOk = TRUE;

         // DL config
         unsigned char iDL_TSC = 0;
         unsigned char iDL_TFI = 0;

         // UL config
         unsigned char iUL_TSC = 0;
         unsigned char iUSF = 0;
         unsigned short iMCS = 9;

         // Command response packet Status
         unsigned short iCLEAR_QUEUE_Status       = DIAG_FTM_STATUS_FAIL;
         unsigned short iStart_Mode_Status        = DIAG_FTM_STATUS_FAIL;
         unsigned short iBCCH_Acquire_Status      = DIAG_FTM_STATUS_FAIL;
         unsigned short iADD_WAIT_Status          = DIAG_FTM_STATUS_FAIL;
         unsigned short iClear_GSM_Events_Status  = DIAG_FTM_STATUS_FAIL;
         unsigned short iClear_EDGE_Events_Status = DIAG_FTM_STATUS_FAIL;
         unsigned short iADD_TIMING_MARKER_Status = DIAG_FTM_STATUS_FAIL;
         unsigned short iSE_BER_CLEAR_Status      = DIAG_FTM_STATUS_FAIL;
         unsigned short iSE_BER_GET_Status        = DIAG_FTM_STATUS_FAIL;
         unsigned char iRel_Rsp_Status            = DIAG_FTM_STATUS_FAIL;   // Release SR B response packet status
         unsigned short iStop_Mode_Status         = DIAG_FTM_STATUS_FAIL;

         // Synchronous event status
         unsigned short iGSM_START_Status          = FTM_GSM_BER_SYNC_STATUS_GEN_FAILURE;
         unsigned short iGSM_BCCH_CNF_Status       = FTM_GSM_BER_SYNC_STATUS_GEN_FAILURE;
         unsigned char  iEDGE_IDLE_Status          = FTM_GSM_BER_SYNC_STATUS_GEN_FAILURE;
         unsigned short iUL_Rel_Status             = FTM_GSM_BER_SYNC_STATUS_GEN_FAILURE;   // UL released status
         unsigned short iDL_Rel_Status             = FTM_GSM_BER_SYNC_STATUS_GEN_FAILURE;   // DL released status
         unsigned short iGSM_BER_STOP_Status       = FTM_GSM_BER_SYNC_STATUS_GEN_FAILURE;

         // Other return variables
         unsigned long iTotal_Bits = 0;
         unsigned long iTotal_Bit_Errors = 0;
         double dBER = 0.0;

         // Flag to use ADD_TIMING_MARKER, which is not available on targets before March 2011
         // Timing markers allow the DUT to adjust the sequence timeline, to support a variable-length
         // acquisition time.  Without the this feature a fixed size delay must be used to account for the
         // worst case embedded acquisition time.
         unsigned char _bUseTimingMarker = false;

         // Flag to use SE BER
         unsigned char _bEnableSE_BER = true;

         // Flag to enable F3 messages
         unsigned char _bEnableF3Messages= false;

         // Setup for using "small waits", in which the PC side transforms a single ADD_WAIT into multiple
         // embedded ADD_WAITS. This deals with a situation where the embedded rex_sleep() takes more time
         // than it should, so segments are delayed. This is a problem in Jan/Feb 2011 that is fixed in Mar 2011
         unsigned char c_bUseSmallWaits = false;

         // Structure to define each EDGE segment
         typedef struct
         {
            unsigned short iChannel;
            unsigned char iBand;
            unsigned char bFirstSlotActive;
            unsigned char iFirstSlotNum;
            unsigned char bSecondSlotActive;
            unsigned char iSecondSlotNum;
            unsigned char iGamma;
            unsigned long iDuration_ms;
            unsigned char  iConf_UL_Rsp_Status;   // Configure UL response packet status
            unsigned char  iConf_DL_Rsp_Status;   // Configure DL response packet status
            unsigned short iADD_WAIT_Rsp_Status;    // ADD_WAIT status
            unsigned char  iEst_Rsp_Status; // Estabalish SRB response packet status
            unsigned short iUL_Est_Status;  // UL established status
            unsigned short iDL_Est_Status;  // DL established status
         } EDGE_VFS_Setup_Item;

         // Setup segment data
         const unsigned char c_iDefaultSyncStatus = FTM_GSM_BER_SYNC_STATUS_GEN_FAILURE;
         const unsigned char c_iDefaultCmdStatus = DIAG_FTM_STATUS_FAIL;
         const unsigned long c_iSegmentCount = 16;  // Loop iterations for alternating mid-high power
         // Duration for each power. Use long duration (1000) with non-sequence capable equipment
         // 26 frames = 119.9ms -> 120ms
         const unsigned long c_iSegmentDuration_ms = 120;
         EDGE_VFS_Setup_Item aEDGE_Segments[c_iSegmentCount] =
         {
            // iChannel, iBand,          1-Actv, 1-#, 2-Actv, 2-#, Gamma, Dur(ms),               ULStatus,         DLStatus
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   17,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   3,   c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   17,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   13,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   17,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   13,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   17,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   13,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   17,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   13,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   17,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   13,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   17,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   13,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   17,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
            {   32, FTM_GSM_BER_EGSM_900,  TRUE,   3,   FALSE,   3,   13,  c_iSegmentDuration_ms, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultCmdStatus, c_iDefaultSyncStatus, c_iDefaultSyncStatus, },
         };
         /*
         Gamma reference { 3 = high, 13 = mid, 20 = min }

         Choosing this order for the beginning:
         1) low power to get the first TBF estabalished
         2) high power, to trigger equipment
         3) after that alternating mid/low power for measurement
         */

         // GSM_BER Sync Status timeout (ms)
         unsigned long _iSyncStatusTimeout = 500;

         // Index in the loop
         int iLoopIndex = 0;

         printf("\nExecuting EDGE NS Sequence " );
         printf("\n  Variables:");
         printf("\n          Segment count: %d", c_iSegmentCount );
         printf("\n      Use TIMING_MARKER: %s", _bUseTimingMarker ? "YES" : "NO" );
         printf("\n          Enable SE BER: %s", _bEnableSE_BER    ? "YES" : "NO" );
         printf("\n     Enable F3 Messages: %s", _bEnableF3Messages? "YES" : "NO" );
         printf("\n    Use small ADD_WAITs: %s", c_bUseSmallWaits  ? "YES" : "NO" );

         // Start text log
         bOk &= QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\n\nLogging to file %s", sLogFile );

         if ( _bEnableF3Messages )
         {
            // Activate FTM F3 Messages
            bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, 23, 0xFF );
         }

         // Select embedded sequence mode
         bOk &= QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );

         // Enter the batch mode, to queue up subsequent FTM commands
         bOk &= QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );

         // Clear the embedded queue
         bOk &= QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iCLEAR_QUEUE_Status );

         // Clear GSM synchronous events
         bOk &= QLIB_FTM_GSM_BER_EVENT_STATUS_CLEAR_REQ(g_hResourceContext, &iClear_GSM_Events_Status);

         // Clear EDGE synchronous events
         bOk &= QLIB_FTM_EGPRS_BER_EVENT_STATUS_CLEAR_REQ(g_hResourceContext, &iClear_EDGE_Events_Status);

         //
         // Start
         //
         bOk &= QLIB_FTM_GSM_START_MODE_REQ ( g_hResourceContext, &iStart_Mode_Status);

         // Wait for start event
         bOk &= QLIB_FTM_GSM_BER_EVENT_STATUS_REQ(
                   g_hResourceContext,
                   FTM_GSM_BER_START_GSM_MODE_SYNC_STATUS,
                   _iSyncStatusTimeout*ms,
                   &iGSM_START_Status);

         //
         // GSM BCCH
         //
         printf("\nSetting up Acquire on Band=%d, Channel=%d",
                aEDGE_Segments[0].iBand,
                aEDGE_Segments[0].iChannel
               );
         bOk &= QLIB_FTM_GSM_SELECT_SPECIFIC_BCCH_REQ(
                   g_hResourceContext,
                   aEDGE_Segments[0].iChannel,
                   aEDGE_Segments[0].iBand,
                   &iBCCH_Acquire_Status);
         // Wait for BCCH event
         bOk &= QLIB_FTM_GSM_BER_EVENT_STATUS_REQ(
                   g_hResourceContext,
                   FTM_GSM_BER_SELECT_SPECIFIC_BCCH_CNF_SYNC_STATUS,
                   _iSyncStatusTimeout*ms,
                   &iGSM_BCCH_CNF_Status );

         //
         // GSM IDLE
         //
         bOk &= QLIB_FTM_EGPRS_BER_START_GPRS_IDLE_MODE_REQ( g_hResourceContext, &iEDGE_IDLE_Status );
         // Wait for IDLE event
         bOk &= QLIB_FTM_GSM_BER_EVENT_STATUS_REQ(
                   g_hResourceContext,
                   FTM_GSM_BER_START_IDLE_MODE_SYNC_STATUS,
                   _iSyncStatusTimeout*ms,
                   &iGSM_BCCH_CNF_Status );

         if ( _bEnableSE_BER )
         {
            // Activate SE BER state machine
            bOk &=  QLIB_FTM_SE_BER_CONFIGURE_REQUEST( g_hResourceContext,
                                                       1,    // PN9 unsigned short iPnSeqType,
                                                       0,    // unsigned char bPnInverted,
                                                       20,   // unsigned short iAcqThreshold,
                                                       30 ); // unsigned short iAcqLostWm );
            // Configure MCS
            bOk &= QLIB_FTM_SE_BER_Configure_EDGE_Expected_MCS(
                      g_hResourceContext,
                      iMCS);

            // Clear the SE BER Accumulators
            bOk &=  QLIB_FTM_SE_BER_CLEAR_SYNC_STATUS( g_hResourceContext, &iSE_BER_CLEAR_Status );
         }

         if ( _bUseTimingMarker )
         {
            bOk &= QLIB_FTM_SEQ_ADD_TIMING_MARKER( g_hResourceContext, &iADD_TIMING_MARKER_Status );
         }
         else
         {
            // Add a wait to account for all of the acquisition time
            Test_FTM_Sequencer_AddWait(
               500, // unsigned long iWaitPeriod,
               c_bUseSmallWaits,
               &iADD_WAIT_Status);
         }

         ////////
         // Loop through segments
         ///////
         printf("\n\nSetting up through %d sets of alternating High/Low segments",  c_iSegmentCount );

         for (iLoopIndex =0; iLoopIndex < c_iSegmentCount; iLoopIndex++ )
         {
            // Clear EDGE synchronous events
            bOk &= QLIB_FTM_EGPRS_BER_EVENT_STATUS_CLEAR_REQ(g_hResourceContext, &iClear_EDGE_Events_Status);

            // DL config
            bOk = QLIB_FTM_EGPRS_BER_CONFIGURE_DL_TBF(
                     g_hResourceContext,
                     iDL_TSC,
                     iDL_TFI,
                     aEDGE_Segments[iLoopIndex].iChannel,
                     aEDGE_Segments[iLoopIndex].iBand,
                     aEDGE_Segments[iLoopIndex].bFirstSlotActive,
                     aEDGE_Segments[iLoopIndex].iFirstSlotNum,
                     aEDGE_Segments[iLoopIndex].bSecondSlotActive,
                     aEDGE_Segments[iLoopIndex].iSecondSlotNum,
                     &aEDGE_Segments[iLoopIndex].iConf_DL_Rsp_Status);

            // UL config
            QLIB_FTM_EGPRS_BER_CONFIGURE_UL_TBF(
               g_hResourceContext,
               iUL_TSC,
               iUSF,
               aEDGE_Segments[iLoopIndex].iGamma,
               aEDGE_Segments[iLoopIndex].bFirstSlotActive,
               aEDGE_Segments[iLoopIndex].iFirstSlotNum,
               aEDGE_Segments[iLoopIndex].bSecondSlotActive,
               aEDGE_Segments[iLoopIndex].iSecondSlotNum,
               &aEDGE_Segments[iLoopIndex].iConf_UL_Rsp_Status );

            ////////////
            // Establish EDGE SRB loopback
            ////////////
            bOk &= QLIB_FTM_EGPRS_BER_ESTABLISH_SRB_LOOPBACK (
                      g_hResourceContext,
                      &aEDGE_Segments[iLoopIndex].iEst_Rsp_Status );

            // Wait for UL Established event
            bOk &= QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(
                      g_hResourceContext,
                      FTM_EGPRS_BER_UL_TBF_ESTABLISHED_SYNC_STATUS,
                      _iSyncStatusTimeout*ms,
                      &aEDGE_Segments[iLoopIndex].iUL_Est_Status );

            // Wait for DL Established event
            bOk &= QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(
                      g_hResourceContext,
                      FTM_EGPRS_BER_DL_TBF_ESTABLISHED_SYNC_STATUS,
                      _iSyncStatusTimeout*ms,
                      &aEDGE_Segments[iLoopIndex].iDL_Est_Status );

            // ADD_WAIT
            bOk &= Test_FTM_Sequencer_AddWait(
                      aEDGE_Segments[iLoopIndex].iDuration_ms, // unsigned long iWaitPeriod,
                      c_bUseSmallWaits,
                      &aEDGE_Segments[iLoopIndex].iADD_WAIT_Rsp_Status );
         }  // for (int iLoopIndex =0; iLoopIndex < c_iSegmentCount; iLoopIndex++ )

         ///////
         // Read SE BER Status
         ////////
         if (
            ( _bEnableSE_BER )
            ||  (TRUE ) ///!!! including this so that the last commmand is not an ADD_WAIT, for which there is not a response
            ///!!! message created by FTM Sequencer (in which case the desktop will not know the sequence is
            ///!!! completed.
         )
         {
            // Clear the SE BER results
            bOk &=  QLIB_FTM_SE_BER_GET_SYNC_STATUS (
                       g_hResourceContext,
                       &iTotal_Bits,
                       &iTotal_Bit_Errors,
                       &dBER,
                       &iSE_BER_GET_Status);
         }

         ///////
         // Execute sequence
         ////////
         bOk &= QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         // Turn off sequencer
         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         ///////
         // Print results
         ////////

         // Function return values
         printf("\n    Function return status: %s", bOk  ? "PASS" : "FAIL" );
         // Response status
         printf("\n       iCLEAR_QUEUE_Status: %s", iCLEAR_QUEUE_Status       == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
         printf("\n        iStart_Mode_Status: %s", iStart_Mode_Status        == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
         printf("\n  iClear_GSM_Events_Status: %s", iClear_GSM_Events_Status  == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
         printf("\n iClear_EDGE_Events_Status: %s", iClear_EDGE_Events_Status == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
         printf("\n           iAcquire_Status: %s", iBCCH_Acquire_Status      == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
         if ( _bUseTimingMarker ) printf("\n iADD_TIMING_MARKER_Status: %s", iADD_TIMING_MARKER_Status == DIAG_FTM_STATUS_SUCCESS ? "PASS" : "FAIL" );
         printf("\n          iADD_WAIT_Status: %s", iADD_WAIT_Status          == DIAG_FTM_STATUS_SUCCESS ? "PASS" : "FAIL" );
         if ( _bEnableSE_BER )
         {
            printf("\n      iSE_BER_CLEAR_Status: %s", iSE_BER_CLEAR_Status      == DIAG_FTM_STATUS_SUCCESS ? "PASS" : "FAIL" );
            printf("\n        iSE_BER_GET_Status: %s", iSE_BER_GET_Status        == DIAG_FTM_STATUS_SUCCESS ? "PASS" : "FAIL" );
         } // if (_bEnableSE_BER)

         // Synchronous Status
         printf("\n      iGSM_START_Status: %s (%d)", iGSM_START_Status     == FTM_GSM_BER_SYNC_STATUS_SUCCESS ? "PASS" : "FAIL",iGSM_START_Status );
         printf("\n   iBCCH_Acquire_Status: %s (%d)", iGSM_BCCH_CNF_Status  == FTM_GSM_BER_SYNC_STATUS_SUCCESS ? "PASS" : "FAIL",iGSM_BCCH_CNF_Status );
         printf("\n      iEDGE_IDLE_Status: %s (%d)", iEDGE_IDLE_Status     == FTM_GSM_BER_SYNC_STATUS_SUCCESS ? "PASS" : "FAIL",iEDGE_IDLE_Status );

         printf("\nStatus  EST   UL    DL    WAIT");
         printf("\n        ----  ----  ----  ----");

         for (iLoopIndex =0; iLoopIndex < c_iSegmentCount; iLoopIndex++ )
         {
            printf("\n        %s %s %s %s",
                   aEDGE_Segments[iLoopIndex].iEst_Rsp_Status == DIAG_FTM_STATUS_SUCCESS ? "PASS" : "FAIL",
                   aEDGE_Segments[iLoopIndex].iUL_Est_Status == FTM_GSM_BER_SYNC_STATUS_SUCCESS ? "PASS" : "FAIL",
                   aEDGE_Segments[iLoopIndex].iDL_Est_Status == FTM_GSM_BER_SYNC_STATUS_SUCCESS ? "PASS" : "FAIL",
                   aEDGE_Segments[iLoopIndex].iADD_WAIT_Rsp_Status == DIAG_FTM_STATUS_SUCCESS ? "PASS" : "FAIL"
                  );
         }  // for (int iLoopIndex =0; iLoopIndex < c_iSegmentCount; iLoopIndex++ )

         if (_bEnableSE_BER)
         {
            printf("\n SE BER results");
            printf("\n                Total Bits: %d", iTotal_Bits );
            printf("\n          Total Bit Errors: %d", iTotal_Bit_Errors );
            printf("\n                       BER: %d", dBER );
         }  // if (_bEnableSE_BER)

         ///////
         // Stop GSM
         ////////
         printf("\n\n>>Please check the GSM Tx power and press any key to stop the GSM NS mode<<");
         getch();

         ///////
         // Release EDGE SRB loopback
         ////////
         bOk &= QLIB_FTM_EGPRS_BER_RELEASE_ALL_TBF(
                   g_hResourceContext,
                   &iRel_Rsp_Status);
         // Wait for UL Released event
         bOk &= QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(
                   g_hResourceContext,
                   FTM_EGPRS_BER_UL_TBF_RELEASED_SYNC_STATUS,
                   _iSyncStatusTimeout*ms,
                   &iUL_Rel_Status );

         // Wait for DL Released event--Having a problem here
         bOk &= QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(
                   g_hResourceContext,
                   FTM_EGPRS_BER_DL_TBF_RELEASED_SYNC_STATUS,
                   _iSyncStatusTimeout*ms,
                   &iDL_Rel_Status );

         bOk &= QLIB_FTM_GSM_STOP_GSM_MODE_REQ(g_hResourceContext, &iStop_Mode_Status);

         bOk &= QLIB_FTM_GSM_BER_EVENT_STATUS_REQ(
                   g_hResourceContext,
                   FTM_GSM_BER_STOP_GSM_MODE_SYNC_STATUS,
                   _iSyncStatusTimeout*ms,
                   &iGSM_BER_STOP_Status);

         printf("\n          iDL_Rel_Status: %s (%d)", iDL_Rel_Status       == FTM_GSM_BER_SYNC_STATUS_SUCCESS ? "PASS" : "FAIL", iGSM_BER_STOP_Status );
         printf("\n          iUL_Rel_Status: %s (%d)", iUL_Rel_Status       == FTM_GSM_BER_SYNC_STATUS_SUCCESS ? "PASS" : "FAIL", iGSM_BER_STOP_Status );
         printf("\n    iGSM_BER_STOP_Status: %s (%d)", iGSM_BER_STOP_Status == FTM_GSM_BER_SYNC_STATUS_SUCCESS ? "PASS" : "FAIL", iGSM_BER_STOP_Status );
         printf("\n");

         QLIB_StopLogging( g_hResourceContext );
      } // end of EDGE

      else if (key == '5')//EVDO
      {
         unsigned char bOk;

         unsigned char _iRxLevelStatus = 0;
         short _iRxAGC = 0;
         short _iRxPwrdBm10 = 0;

         unsigned long _iNumPkt =0;
         unsigned long _iPktError = 0;
         double _dPER = 0;
         unsigned char _iStatus = 0;

         unsigned short iADD_WAIT_Status = 0;
         unsigned char iSet_Pwr_Status = 0;
         unsigned char iPilot_Status = 0;
         unsigned char iSync_Status = 0;
         unsigned char iAccess_Status = 0; // access channel command status
         unsigned char iTraffic_Status = 0; // traffic channel command status

         unsigned char _bEnableRPC = 1;

         unsigned char _bEnablePER = 1;     // read PER
         unsigned short _iChannel = 387;
         unsigned long _iBC = FTM_EVDO_BAND_BC0;

         // Rev A: MOD ACC  taken from Diag_FTM::FTM_EVDO_REV_A_START_CALL
         unsigned long iACLC_MASK_I_HI= 0x3ff;
         unsigned long iACLC_MASK_I_LO= 0;
         unsigned long iACLC_MASK_Q_HI= 0x3fe;
         unsigned long iACLC_MASK_Q_LO= 0x1;
         unsigned long iAC_Txpattern= 0x84B3E374; //iAC_Txpattern = Data pattern to transmit; a PRBS 5 sequence, much better than all 5s or As
         unsigned char iACCESS_CYCLE_DURATION= 64;
         unsigned char iOpen_Loop_Adjust= 81; // 81 for BC0, use 84 for high band
         unsigned char iProbe_Initial_Adjust= 15;
         unsigned char iProbe_Num_Step= 15;
         unsigned char iPower_Step= 15;
         unsigned char iPreamble_Length= 2;
         unsigned char iEnhanced_Access_Enable= 1;
         unsigned char iPreamble_Length_Slots= 1;
         unsigned char iAccess_Offset= 0;
         unsigned char iSector_Access_Rate_Max= 0;
         unsigned char iTerminal_Access_Rate_Max = 0;

         // Rev A: MOD TRA
         unsigned char iDRC_Value = 12;
         unsigned char iRRI_Payload = 10;

         // Setup for using "small waits", in which the PC side transforms a single ADD_WAIT into multiple
         // embedded ADD_WAITS. This deals with a situation where the embedded rex_sleep() takes more time
         // than it should, so segments are delayed. This is a problem in Jan/Feb 2011 that is fixed in Mar 2011
         unsigned char c_bUseSmallWaits = false;

         const int c_iLoopMax = 10; // Loop iterations for alternating mid-high power
         const unsigned long c_iSegmentLength = 20; // ms

         QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("Logging to file %s", sLogFile );

         QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, 0);
         QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );

         QLIB_FTM_EVDO_PILOT_ACQ_V2(g_hResourceContext, _iBC, _iChannel, 0, 0, &iPilot_Status);

         QLIB_FTM_EVDO_SYS_TIME_ACQ_V2(g_hResourceContext, &iSync_Status);

         QLIB_FTM_EVDO_REV_A_MOD_ACC_V2(    g_hResourceContext,
                                            iACLC_MASK_I_HI,
                                            iACLC_MASK_I_LO,
                                            iACLC_MASK_Q_HI,
                                            iACLC_MASK_Q_LO,
                                            iAC_Txpattern,
                                            iACCESS_CYCLE_DURATION,
                                            iOpen_Loop_Adjust,
                                            iProbe_Initial_Adjust,
                                            iProbe_Num_Step,
                                            iPower_Step,
                                            iPreamble_Length,
                                            iEnhanced_Access_Enable,
                                            iPreamble_Length_Slots,
                                            iAccess_Offset,
                                            iSector_Access_Rate_Max,
                                            iTerminal_Access_Rate_Max,
                                            &iAccess_Status);

         static const byte t2p[13]= {0, 3, 15, 28, 35, 40, 46, 52, 57, 62, 68, 74, 85};
         //rri channel gain
         static const byte rri_gain = -6;

         QLIB_FTM_EVDO_REV_A_MOD_TRA(
            g_hResourceContext,
            iDRC_Value,//iDRC_Value = DRC value, forward traffic channel data rate, valid values are from 1 to 14.
            1,//iDRC_COVER_VALUE = DRC cover value associated with the sector from which the forward traffic data at particular DRC value is requested; valid values are from 1 to 7. default value is 1
            0, //drc length. 0: 1slot, 1: 2 slots, 2: 4 slots, 3: 8 slots //cmu nonsig B89 option requires at least 4 slots. 8960 fixed to 8 slots. However, IS866A requires 0 (1slot)
            0, //drc gating, default disable.
            6, //iDRC_CH_GAIN = Power of DRC channel relative to pilot channel, unit 0.5dB
            6, //iACK_CH_GAIN = Power of the ACK channel relative to the pilot channel, unit 0.5dB
            1, //iDSC_VALUE = Cell cover associated with the forward link data source
            8, //iDSC_LENGTH = Length of single DSC value transmission in units of 8 slots;default is 64 slots
            18,//iDSC_GAIN = Power of the DSC channel relative to the pilot channel in units of -0.5 dB;
            1,//rpc step, 0->0.5dB; 1->1dB
            0,//frame offset
            _bEnableRPC,//rpc enable
            0x84B3E374,//iRTC_TXPATTERN = Data pattern to transmit. a PRBS 5 sequence, much better than all 5s or As
            iACLC_MASK_I_HI,//iLCM_I_CHAN_HI = Long code mask for I channel high 32 bits, default value is 0x3FF
            iACLC_MASK_I_LO,//iLCM_I_CHAN_LOW = Long code mask for I channel low 32 bits, default value is 0x0
            iACLC_MASK_Q_HI,//iLCM_Q_CHAN_HI = Long code mask for q channel high 32 bits, default value is 0x3FE
            iACLC_MASK_Q_LO,//iLCM_Q_CHAN_LOW = Long code mask for q channel low 32 bits, default value is 0x1
            iRRI_Payload,//iRRI_PAYLOAD = Physical layer packet size index; valid values are from 1 to 12
            0,//iHI_CAP_LO_LATENCY = The transmit mode of operation; valid values are  0= High capacity; 1 = Low latency. hi-cap, default per IS-866-A
            3,//iGAIN_TRANSITION_POINT = gain trans point, default 3 per IS-866-A
            t2p[iRRI_Payload], // T2P pre-trans gain, unit 0.25dB
            t2p[iRRI_Payload], //t2p post-trans gain, unit 0.25dB
            0,//arq mode,default is 0: BPSK
            7,//aux min payload, default is 7: 3072 bit, note, this payload starts from 0 to 11 (not 1 - 12)
            12,//aux pilot chan gain, unit -1dB
            rri_gain,//rri chan pre-trans gain
            rri_gain,//rri chan post trans gain.
            FTM_EVDO_IS890_MODE_1);//Ack mode, default 3: full packet decode and nack in each slot. (Must be FTM_EVDO_IS890_MODE_1 to comply with 3GPP2 Spec IS-866-A)

         if ( _bEnablePER )
         {
            QLIB_FTM_EVDO_RESET_FWD_STAT(g_hResourceContext);
         }
         ////////
         // Loop through segments
         ///////
         printf("\n\nSetting up through %d sets of alternating High/Low segments, of %dms in duration each",
                c_iLoopMax,
                c_iSegmentLength
               );

         for (int iLoop =0; iLoop < c_iLoopMax; iLoop ++ )
         {
            bOk &= Test_FTM_Sequencer_AddWait(
                      c_iSegmentLength, // unsigned long iWaitPeriod,
                      c_bUseSmallWaits,
                      &iADD_WAIT_Status);

            bOk &= QLIB_FTM_EVDO_SET_REV_PWR(g_hResourceContext, 1, -200, &iSet_Pwr_Status);

            bOk &= Test_FTM_Sequencer_AddWait(
                      c_iSegmentLength, // unsigned long iWaitPeriod,
                      c_bUseSmallWaits,
                      &iADD_WAIT_Status);

            bOk &= QLIB_FTM_EVDO_SET_REV_PWR(g_hResourceContext, 1, 200, &iSet_Pwr_Status);
         }

         ///////
         // Read FER/SER Status
         ////////
         if ( _bEnablePER )
         {
            QLIB_FTM_EVDO_GET_FWD_STAT(g_hResourceContext, &_iNumPkt, &_iPktError, &_dPER);
         }

         QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         // Update PER
         _dPER = (double) _iPktError/_iNumPkt * 100;

         printf("EVDO Pilot Status = %d\n", iPilot_Status);
         printf("EVDO Sync Status = %d\n", iSync_Status);
         printf("EVDO Access Channel Command Status = %d\n", iAccess_Status);
         printf("EVDO Traffic Channel Command Status = %d\n", iTraffic_Status);

         // print resuslt from SER/FER status, Rx Level
         printf("RxLevl (dBm*10) = %d\n", _iRxPwrdBm10);
         printf("iNumPkt=%d, iPktError=%d, dPER=%6.2f\n", _iNumPkt, _iPktError, _dPER);

         QLIB_FTM_SEQ_CLEAR_QUEUE(g_hResourceContext, & iFTM_Error_Code);

         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         QLIB_StopLogging( g_hResourceContext );
      }
      else if (key == '6')  // LTE
      {
         /*
         Equipment Setup
         LTE
         For Band: BC4
         UL Freq. = 1731 MHz
         DL Freq. = 2131 MHz
         Select the appropriate LTE ARB file
         */
         unsigned short ms = 1000;

         // LTE acquire parameter
         unsigned char iBand= 4; /* For LTE Band 4*/
         unsigned short iDLChannel = 2160;

         // LTE Start NS    Data path Parameters
         /* LTE NS Data Path Parameters provided here are compatible with CMW500. These values are hardcoded in the ARB Waveforms. */
         unsigned short iCRNTI = 14;
         unsigned char iNetworkSigValue = 1;   /*Base station signaled value. In the case of NS we force the mobile to a value of choice. */
         char iBSMaxPowerLimit = 30;
         unsigned char iDLLCID = 1;
         unsigned char iULLCID = 2;

         // Parameters to configure upling waveform

         unsigned char iConfigOverride = 1;
         unsigned char iTxChain = 0;
         unsigned char iStartRB = 0;
         const int _RbLoopCount = 3;
         unsigned char iNumRB[_RbLoopCount] = {6,12,50};
         unsigned char iMCS = 0;

         // Parameters to Configure UL Power
         unsigned char iTxPwrCtrlMode = 1;
         const int _TxLoopCount = 5;
         short iTxLevel[ _TxLoopCount] = {230, 100, 0, -100, 230};

         // Loop Variables
         const int _RxLoopCount = 3;

         // _AddWait: Adds add wait calls in FTM_Sequencer when set to 1
         int _AddWait = 0;

         // Parameters to Get DL Power Levels

         unsigned char iRxChain = 0;
         short iRxAGC[_RbLoopCount*_RxLoopCount] = {0};
         short iRxLeveldBm10[_RbLoopCount*_RxLoopCount] = {0};

         // Parameters to measure BLER
         /* To be added */

         //Delay Profiling between each command
         int Delay_After_Start = 200;
         int Delay_After_Acquire = 200;
         int Delay_After_Set_Data_Path = 200;
         int Dealy_After_Call_Status = 200;
         int Delay_After_Conf_UL = 200;
         int Delay_After_Set_UL_Power = 200;
         int Delay_After_Stop = 50;

         // Function return values
         unsigned char bOk = TRUE;

         // LTE Command status
         unsigned short iCLEAR_QUEUE_Status                                     =   DIAG_FTM_STATUS_FAIL;
         unsigned short iADD_WAIT_Status                                            =   DIAG_FTM_STATUS_FAIL;
         unsigned long  iLTE_Start_Status                                       =   DIAG_FTM_STATUS_FAIL;
         unsigned long  iLTE_Acquire_Status                                     =   DIAG_FTM_STATUS_FAIL;
         unsigned long  iLTE_DataPath_Status                                        =   DIAG_FTM_STATUS_FAIL;
         unsigned long  iLTE_CallConnected_Status                               =   DIAG_FTM_STATUS_FAIL;
         unsigned long  iLTE_Conf_UL_Waveform_Status[_RbLoopCount]              =   {DIAG_FTM_STATUS_FAIL};
         unsigned long  iLTE_Conf_UL_Power_Status[_RbLoopCount*_TxLoopCount]        =   {DIAG_FTM_STATUS_FAIL};
         unsigned long  iLTE_DL_Power_Status[_RbLoopCount*_RxLoopCount]         =   {DIAG_FTM_STATUS_FAIL};
         unsigned long  iLTE_Get_DL_Level_Status                                    =   DIAG_FTM_STATUS_FAIL;
         unsigned long  iLTE_Reset_DL_BLER_Status                               =   DIAG_FTM_STATUS_FAIL;
         unsigned long  iLTE_Get_DL_BLER_Status                                 =   DIAG_FTM_STATUS_FAIL;
         unsigned long  iLTE_Stop_Status                                            =   DIAG_FTM_STATUS_FAIL;

         int i = 0;
         for(i=0; i<_RbLoopCount; i++)
         {
            iLTE_Conf_UL_Waveform_Status[i] = DIAG_FTM_STATUS_FAIL;
         }
         for(i=0; i<(_RbLoopCount*_TxLoopCount); i++)
         {
            iLTE_Conf_UL_Power_Status[i] = DIAG_FTM_STATUS_FAIL;
         }
         for(i=0; i<_RbLoopCount*_RxLoopCount; i++)
         {
            iLTE_DL_Power_Status[i] = DIAG_FTM_STATUS_FAIL;
         }

         // Flag to use ADD_TIMING_MARKER, which is not available on targets before March 2011
         // Timing markers allow the DUT to adjust the sequence timeline, to support a variable-length
         // acquisition time.  Without the this feature a fixed size delay must be used to account for the
         // worst case embedded acquisition time.
         unsigned char _bUseTimingMarker = false;

         // Flag to enable F3 messages
         unsigned char _bEnableF3Messages= false;

         printf("\nExecuting LTE NS Sequence on DL channel %d", iDLChannel );

         // Start text log
         sLogFile = "C:\\ftm_sequencer_LTE_NS_test.txt";
         bOk &= QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\n\nLogging to file %s", sLogFile );

         if ( _bEnableF3Messages )
         {
            // Activate FTM F3 Messages
            bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, 23, 0xFF );
         }

         // Select embedded sequence mode
         bOk &= QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );

         // Enter the batch mode, to queue up subsequent FTM commands
         bOk &= QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );

         // Start LTE NS Mode
         bOk &= QLIB_FTM_LTE_NS_START( g_hResourceContext, &iLTE_Start_Status );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Start*ms, &iADD_WAIT_Status );

         // Acquire LTE NS Downling Signal
         bOk &= QLIB_FTM_LTE_NS_ACQ( g_hResourceContext, iBand, iDLChannel, &iLTE_Acquire_Status );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Acquire*ms, &iADD_WAIT_Status );

         // Start LTE NS Data Path
         bOk &= QLIB_FTM_LTE_NS_START_DP( g_hResourceContext, iCRNTI, iNetworkSigValue, iBSMaxPowerLimit, iDLLCID, iULLCID, &iLTE_DataPath_Status );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Data_Path*ms, &iADD_WAIT_Status );

         //Check to see if LTE NS Call is UP
         bOk &= QLIB_FTM_LTE_NS_IS_CONNECTED( g_hResourceContext, &iLTE_CallConnected_Status);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Dealy_After_Call_Status*ms, &iADD_WAIT_Status );

         int LoopRb = 0;
         //Loop to test for different RB combination
         for(LoopRb = 0; LoopRb < _RbLoopCount ; LoopRb++)
         {
            // Configure UL Waveform
            bOk &= QLIB_FTM_LTE_NS_CONFIG_UL_WAVEFORM( g_hResourceContext, iConfigOverride, iTxChain, iStartRB, iNumRB[LoopRb], iMCS, &iLTE_Conf_UL_Waveform_Status[LoopRb] );
            if(_AddWait)    bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Conf_UL*ms, &iADD_WAIT_Status );

            //Loop to Change Tx Power level _TxLoopCount times
            for(int LoopULPwr = 0; LoopULPwr < _TxLoopCount; LoopULPwr++)
            {
               // Set UL Power
               bOk &= QLIB_FTM_LTE_NS_CONFIG_UL_POWER( g_hResourceContext, iTxChain, iTxPwrCtrlMode, iTxLevel[LoopULPwr], &iLTE_Conf_UL_Power_Status[(_TxLoopCount*LoopRb) + LoopULPwr] );
               if(_AddWait) bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_UL_Power*ms, &iADD_WAIT_Status );
            }

            //Loop to Measure RX Power levels _RxLoopCount times
            for(int LoopRxLvl = 0; LoopRxLvl < _RxLoopCount; LoopRxLvl++)
            {
               // Measure DL Power
               QLIB_FTM_LTE_NS_GET_DL_LEVEL( g_hResourceContext, iRxChain, &iRxAGC[(_RxLoopCount*LoopRb) + LoopRxLvl], &iRxLeveldBm10[(_RxLoopCount*LoopRb) + LoopRxLvl], &iLTE_DL_Power_Status[(_RxLoopCount*LoopRb) + LoopRxLvl] );
               if(_AddWait) bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_UL_Power*ms, &iADD_WAIT_Status );
            }
         }

         ///////
         // Execute sequence
         ////////
         bOk &= QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         // Turn off sequencer
         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         ///////
         // Print results
         ////////

         // Function return values
         printf("\n\nFunction return status: %s\n", bOk  ? "PASS" : "FAIL" );

         // Response status For LTE Start
         if(iLTE_Start_Status == 0
               || iLTE_Start_Status == 13)
         {
            iLTE_Start_Status = DIAG_FTM_STATUS_SUCCESS;
         }
         printf("\n\tiLTE_Start_Status:           \t%s", iLTE_Start_Status              == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );

         // Response status For LTE Acquire
         if(iLTE_Acquire_Status == 0
               || iLTE_Acquire_Status == 13)
         {
            iLTE_Acquire_Status = DIAG_FTM_STATUS_SUCCESS;
         }
         printf("\n\tiLTE_Acquire_Status:         \t%s", iLTE_Acquire_Status            == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );

         // Response status For LTE Set DataPath
         printf("\n\tiLTE_DataPath_Status:        \t%s", iLTE_DataPath_Status           == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );

         // Response status For LTE Connected
         printf("\n\tiLTE_CallConnected_Status:   \t%s", iLTE_CallConnected_Status      == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );

         printf("\n\n\t**********************************");
         for(LoopRb = 0; LoopRb < _RbLoopCount ; LoopRb++)
         {
            // Response status For Conf UL Waveform
            printf("\n\t********** Num RBs = %d **********",iNumRB[LoopRb]);
            printf("\n\t**********************************\n");
            printf("\n\tiLTE_Conf_UL_Waveform_Status[%d]:\t%s\n", LoopRb, iLTE_Conf_UL_Waveform_Status[LoopRb]  == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );

            //Loop to Change Tx Power level _TxLoopCount times
            for(int LoopULPwr = 0; LoopULPwr < _TxLoopCount; LoopULPwr++)
            {
               // Response status For LTE Set Conf UL  Power
               printf("\n\t\t[%d] --> TX Power = %d\tStatus: %s", LoopULPwr+1, iTxLevel[LoopULPwr], iLTE_Conf_UL_Power_Status[(_TxLoopCount*LoopRb) + LoopULPwr] == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
            }
            printf("\n");

            //Loop to Measure RX Power levels _RxLoopCount times
            for(int LoopRxLvl = 0; LoopRxLvl < _RxLoopCount; LoopRxLvl++)
            {
               // Response status For LTE Set Conf UL  Power
               printf("\n\t\t[%d] --> RxAGC = %d\tRx Level(dBm10): %d \t Status: %s",
                      LoopRxLvl+1, iRxAGC[(_RxLoopCount*LoopRb) + LoopRxLvl], iRxLeveldBm10[(_RxLoopCount*LoopRb) + LoopRxLvl],
                      iLTE_DL_Power_Status[(_RxLoopCount*LoopRb) + LoopRxLvl] == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
            }
            printf("\n");
            printf("\n\t**********************************");
         }
         ///////
         // Stop LTE
         ////////
         printf("\n\n>>Press any key to stop the LTE NS Call<<");
         getch();

         //End LTE NS Call
         bOk &= QLIB_FTM_LTE_NS_STOP(g_hResourceContext, &iLTE_Stop_Status );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Stop*ms, &iADD_WAIT_Status );

         printf("\n\tiLTE_Stop_Status:            \t%s", iLTE_Stop_Status != 0 ? "PASS" : "FAIL");
         printf("\n");

         QLIB_StopLogging( g_hResourceContext );
      } // end of LTE  NS
      else if (key == '8')
      {
         /* Notes
         FTM Sequencer Unit TEST: 1k support for EFS NV RF items
         _userInput= '0' ==> Non-sequencer mode
         _userInput= '1' ==> Test using sequencer mode
         */

         bool _bUseSequencer = false;

         unsigned char _userInput;

         unsigned char _iStatus = 1;

         unsigned short iFTM_Error_Code = 0;

         unsigned char iSequenceCompleted = 0;

         unsigned int k=0;

         int j = 0;

         const int MAX_NV_ITEM_SIZE = 1024;
         const int MAX_NV_ITEM = 1000;
         unsigned char bOk = '\0';
         unsigned RFNV_ID = 20000;
         unsigned NV_Count = 100;
         unsigned int NV_Item_Size = MAX_NV_ITEM_SIZE;
         unsigned char NV_Item[MAX_NV_ITEM_SIZE];
         unsigned long _hTimer = 0;
         unsigned long _iElapsedTime = 0;
         unsigned long _iStartTime = 0;
         unsigned long _iFinishTime = 0;
         unsigned long _iCurrentTime = 0;
         unsigned short Check = 0;

         QLIB_StartLogging( g_hResourceContext,  "C:\\EFS_NV_RF_MAX_SIZE.txt");
         QLIB_DIAG_SetMessageFilter( g_hResourceContext, 23, 0xFF );

         _iStatus = QLIB_DIAG_CONTROL_F ( g_hResourceContext, 3 );
         if(_iStatus)
         {
            printf("Set FTM Mode SUCEESS!\n");
         }
         printf("Start Adress for EFS NV Write (Should be above 20000) : ");
         scanf("%d", &RFNV_ID);
         printf("\n");

         Check = 0;
         while(!Check)
         {
            printf("Number of EFS NV_Items to Write (Max 1000) : ");
            scanf("%d", &NV_Count);
            printf("\n");
            if(NV_Count <= MAX_NV_ITEM)
            {
               Check = 1;
            }
            else
            {
               NV_Count = 0;
            }
         }

         Check = 0;
         while(!Check)
         {
            printf("Size of Each NV item (1K Max): ");
            scanf("%d", &NV_Item_Size);
            printf("\n");
            if(NV_Item_Size <= MAX_NV_ITEM_SIZE)
            {
               Check = 1;
            }
            else
            {
               NV_Item_Size = 0;
            }
         }

         printf("Use Sequencer or not (1=YES, 0=NO)-->");
         _userInput = getch(); //'0';

         if(_userInput == '1')
         {
            _bUseSequencer = true;
            printf("Use Sequencer\n");
         }
         else if(_userInput == '0')
         {
            _bUseSequencer = false;
            printf("Sequencer Mode - OFF\n");
         }

         for(j = 0; j < 1; j++)
         {
            if(_bUseSequencer)
            {
               QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );
               printf("QLIB_FTM_SEQ_SetSequencerMode()\n");
               QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );
               printf("QLIB_FTM_SEQ_ActivateBatchMode()\n");
            }

            /*
            All Bytes of NV_Item are initialized to 0xFF and first two bytes are set to 0x0F and 0xF0 respectively
            to check if RFNV_WRITE is successfull
            */
            memset(NV_Item, 255, NV_Item_Size);
            NV_Item[0] = 0x0F;
            NV_Item[1] = 0xF0;

            // Start a timer
            _hTimer = QLIB_StartTimer( g_hResourceContext );
            printf("Timer Id = %ld \n", _hTimer);

            // Get Start time
            _iStartTime = QLIB_GetTimerElapsed( g_hResourceContext, _hTimer );
            printf("Start Time = %ld \n", _iStartTime);
            for(k = 0; k < NV_Count ; k++)
            {
               bOk = QLIB_FTM_RFNV_WRITE(g_hResourceContext, (RFNV_ID+k), NV_Item, NV_Item_Size, &iFTM_Error_Code);
               printf("\n%d) --> QLIB_FTM_RFNV_WRITE() = %s\tWrite Address = %d\tFTM_Error_Code =%d \n", k+1 ,bOk ? "PASS" : "FAIL", RFNV_ID+k, iFTM_Error_Code );

               if(_bUseSequencer)
               {
                  QLIB_FTM_SEQ_ADD_WAIT(g_hResourceContext, 1, &iFTM_Error_Code);
                  //printf("QLIB_FTM_SEQ_ADD_WAIT(%d)\n", iFTM_Error_Code);
               }
            }

            bOk = QLIB_FTM_RFNV_WRITE(g_hResourceContext, (RFNV_ID+k), NV_Item, NV_Item_Size, &iFTM_Error_Code);
            printf("\n%d) --> QLIB_FTM_RFNV_WRITE() = %s\tWrite Address = %d\tFTM_Error_Code =%d \n", k+1 ,bOk ? "PASS" : "FAIL", RFNV_ID+k, iFTM_Error_Code );
            if(_bUseSequencer)
            {
               QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iSequenceCompleted );
               printf("QLIB_FTM_SEQ_EXECUTE_QUEUE\n");
            }

            // Get Finish time
            _iFinishTime = QLIB_GetTimerElapsed( g_hResourceContext, _hTimer );
            printf("Finish Time = %ld \n", _iFinishTime);

            if(_bUseSequencer)
            {
               QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iFTM_Error_Code );
               printf("QLIB_FTM_SEQ_CLEAR_QUEUE\n");
               QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );
               printf("QLIB_FTM_SEQ_DeactivateBatchMode\n");
            }

            // Total elapsed time  = Finish Time - Start Time
            _iElapsedTime = _iFinishTime - _iStartTime;
            printf("Elapsed Time = %ld \n", _iElapsedTime);

            // Stop timer
            QLIB_StopTimer( g_hResourceContext, _hTimer );
            printf("Stop Timer: Timer Id = %ld \n", _hTimer);
         }
         printf("Press a key to continue\n");
         while(!kbhit());
      }
      else if(key == '9')
      {
         QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\nLogging to file %s", sLogFile );

         unsigned short iChannel = 32;
         unsigned char bOk = false;
         unsigned long c_iGSM_Frame_us = (unsigned long)(4.61538*1000);  //

         // Allocating 18 frames (83.07684ms) for each measurement, based on ~75ms required for 5 averages on 2 gain states
         unsigned long c_iGSM_RX_LevelDuration = 18*c_iGSM_Frame_us;

         unsigned short _iGSM_RX_LevelADD_Wait_Status = 0;
         unsigned long c_iGSM_TX_SetupDuration = 10*1000;   // 10ms for Mode, Channel & Tx setup
         unsigned short _iGSM_TX_SetupADD_Wait_Status = 0;

         // Activate sequence mode for embedded mode
         QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );
         printf("QLIB_FTM_SEQ_SetSequencerMode()\n");
         QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );
         printf("QLIB_FTM_SEQ_ActivateBatchMode()\n");

         QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iFTM_Error_Code );
         printf("QLIB_FTM_SEQ_CLEAR_QUEUE\n");

         bOk = true;

         // RF Mode, GSM900
         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_GSM_900 );
         printf("\nRF FTM Mode = GSM 900, QLIB_FTM_SET_MODE(): %s", bOk ? "PASS" : "FAIL" );

         // Set the frequency channel
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel);
         printf("\nQLIB_FTM_SET_CHAN(): %s", bOk ? "PASS" : "FAIL" );
         printf("\nARFCN = %d", iChannel);
         printf("\nNote:  for ARFCN(32), DL = 941.4MHz, UL = 896.4MHz");

         // Set Tx On
         bOk &= QLIB_FTM_SET_TX_ON( g_hResourceContext );
         printf("\nQLIB_FTM_SET_TX_ON(): %s", bOk ? "PASS" : "FAIL" );

         // Set Rx Off
         bOk &= QLIB_FTM_SET_RX_BURST( g_hResourceContext, 0, 2 );
         printf("\nQLIB_FTM_SET_RX_BURST(): %s", bOk ? "PASS" : "FAIL" );

         unsigned char _iSlotNum = 0;
         unsigned char _iTurnSlotOn = 1;
         unsigned short _iPwrLevel_High = 3000; // dBm/100
         unsigned short _iPwrLevel_Low = 0; // dBm/100
         unsigned char _iModScheme = 0;
         unsigned short _iADD_Wait_Status = 0;
         unsigned char _bTxHighLoToggleState = 1;

         // High power
         bOk = QLIB_FTM_SET_TX_FRAME_MATRIX (
                  g_hResourceContext,
                  _iSlotNum,
                  _iTurnSlotOn,
                  _iPwrLevel_High,
                  _iModScheme);
         printf("\nQLIB_FTM_SET_TX_FRAME_MATRIX(): %s", bOk ? "PASS" : "FAIL" );

         // ADD_WAIT
         bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_TX_SetupDuration, &_iGSM_TX_SetupADD_Wait_Status );

         // Rx Range bit mask
         unsigned char _iRxRangeBitMask =
            (
               FTM_GSM_RX_GAIN_RANGE_0
               |   FTM_GSM_RX_GAIN_RANGE_1
            );

         // Number of bursts to average over
         unsigned char _iNumAverages = 5;

         // Toggle flag
         unsigned char _bToggleRx = true;

         bOk = true;

         unsigned long iLoopCount = 0;

         /// Number of cases
         const unsigned long c_iNumCases = 10;

         // Array of channels
         unsigned short c_iChannelList[c_iNumCases]  = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };
         int _iRSSI_Index = 0;

         // RSSI results
         unsigned long _aiRSSI_Results[c_iNumCases][ FTM_GSM_RX_GAIN_RANGE_CAL_NUM_RETURN_VALUES ];

         // Loop through all cases
         for ( iLoopCount = 0; iLoopCount < c_iNumCases; iLoopCount++)
         {
            // Set the frequency channel
            bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, c_iChannelList[iLoopCount] );
            printf("\nQLIB_FTM_SET_CHAN(): %s", bOk ? "PASS" : "FAIL" );

            // High power
            bOk = QLIB_FTM_SET_TX_FRAME_MATRIX (
                     g_hResourceContext,
                     _iSlotNum,
                     _iTurnSlotOn,
                     _bTxHighLoToggleState ? _iPwrLevel_High : _iPwrLevel_Low,
                     _iModScheme);
            printf("\nQLIB_FTM_SET_TX_FRAME_MATRIX(): %s", bOk ? "PASS" : "FAIL" );

            // Toggle the Tx state
            _bTxHighLoToggleState = !_bTxHighLoToggleState;

            // The first time the function is called, then toggle must be TRUE to turn on the Rx
            // The last time the function is called, the toggle should be TRUE again, to turn RX off
            _bToggleRx = TRUE;

            // Clear RSSI array
            for ( _iRSSI_Index = 0; _iRSSI_Index < FTM_GSM_RX_GAIN_RANGE_CAL_NUM_RETURN_VALUES; _iRSSI_Index++ )
               _aiRSSI_Results[iLoopCount][ _iRSSI_Index ] = 0;

            // Call the calibration function
            bOk = QLIB_FTM_RX_GAIN_RANGE_CAL(
                     g_hResourceContext,
                     _iRxRangeBitMask,
                     _iNumAverages,
                     _bToggleRx,
                     &_aiRSSI_Results[iLoopCount][0]
                  );

            // Turn off Rx toggle
            _bToggleRx = false;

            // ADD_WAIT
            bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_RX_LevelDuration, &_iGSM_RX_LevelADD_Wait_Status );
            // bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, _iPowerStrobeDuration_us, &_iGSM_RX_LevelADD_Wait_Status );
         } // for ( iLoopCount = 0; iLoopCount < c_iNumCases; iLoopCount++)

         // Set Tx OFF
         bOk &= QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         printf("\nQLIB_FTM_SET_TX_OFF(): %s", bOk ? "PASS" : "FAIL" );

         QLIB_FTM_SEQ_STATUS( g_hResourceContext, & iFTM_Error_Code, & iQueue_Item_Count, &bIsExecuting, &iNext_ID_Number, & iTimeRef);

         printf("Pre-execution Status");
         printf("iQueue_Item_Count = %d, bIsExecuting = %d\n", iQueue_Item_Count, bIsExecuting);

         QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         QLIB_FTM_SEQ_STATUS( g_hResourceContext, & iFTM_Error_Code, & iQueue_Item_Count, &bIsExecuting, &iNext_ID_Number, & iTimeRef);
         printf("Post-execution Status");
         printf("iQueue_Item_Count = %d, bIsExecuting = %d\n", iQueue_Item_Count, bIsExecuting);

         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         // Loop through all cases
         for ( iLoopCount = 0; iLoopCount < c_iNumCases; iLoopCount++)
         {
            // Loop through all RSSI values

            for ( _iRSSI_Index = 0; _iRSSI_Index < FTM_GSM_RX_GAIN_RANGE_CAL_NUM_RETURN_VALUES; _iRSSI_Index++ )
            {
               printf( "  \nRSSI[%d][%d] = %d",
                       iLoopCount,
                       _iRSSI_Index,
                       _aiRSSI_Results[iLoopCount][ _iRSSI_Index ]
                     );
            }   // for ( _iRSSI_Index = 0; _iRSSI_Index < FTM_GSM_RX_GAIN_RANGE_CAL_NUM_RETURN_VALUES; _iRSSI_Index++ )
         }  // for ( iLoopCount = 0; iLoopCount < c_iNumCases; iLoopCount++)
         QLIB_StopLogging( g_hResourceContext );
      } // '9'
      else if(key == 'A')
      {
         QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\nLogging to file %s", sLogFile );

         // Activate sequence mode for embedded mode
         QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );
         printf("QLIB_FTM_SEQ_SetSequencerMode()\n");
         QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );
         printf("QLIB_FTM_SEQ_ActivateBatchMode()\n");

         unsigned char bOk = true;

         QLIB_FTM_SEQ_STATUS( g_hResourceContext, & iFTM_Error_Code, & iQueue_Item_Count, &bIsExecuting, &iNext_ID_Number, & iTimeRef);
         printf("Pre-execution Status");
         printf("iQueue_Item_Count = %d, bIsExecuting = %d\n", iQueue_Item_Count, bIsExecuting);

         QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         QLIB_FTM_SEQ_STATUS( g_hResourceContext, & iFTM_Error_Code, & iQueue_Item_Count, &bIsExecuting, &iNext_ID_Number, & iTimeRef);
         printf("Post-execution Status");
         printf("iQueue_Item_Count = %d, bIsExecuting = %d\n", iQueue_Item_Count, bIsExecuting);

         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         QLIB_StopLogging( g_hResourceContext );
      } // 'A'

      else if (key == 'B')
      {
         /* Stress Test for FTM Sequencer using QLIB_FTM_SET_PDM command */

         bool _bUseSequencer = false;
         unsigned short iPDM = 100;
         unsigned char _iStatus = 1;
         unsigned char _userInput;
         unsigned short _iInitialPDM = 100;
         unsigned short iFTM_Error_Code = 0;
         unsigned char iSequenceCompleted = 0;
         unsigned long Loop_Count = 0;
         bool Valid_Count = true;
         unsigned int stress_test_index = 0;

         QLIB_StartLogging( g_hResourceContext,  "C:\\FTM_Sequencer_PDM_Stress_Test.txt");

         QLIB_DIAG_SetMessageFilter( g_hResourceContext, 23, 0xFF );

         _iStatus = QLIB_DIAG_CONTROL_F ( g_hResourceContext, 3 );
         if(_iStatus)
         {
            printf("Set FTM Mode SUCEESS!\n");
         }

         printf("Use Sequencer or not (1=YES, 0=NO)-->");
         _userInput = getch();
         if(_userInput == '1')
         {
            _bUseSequencer = true;
            printf("Use Sequencer\n");
         }
         printf("Enter Loop Count for stress Test: ");
         scanf("%d", &Loop_Count);
         printf("\n");

         if(Loop_Count <= 100000)
         {
            printf("Loop Count = %d\n", Loop_Count);
         }
         else
         {
            printf("Enter Loop Count value less than 100,000 \n\n");
            Valid_Count = false;
         }

         if(Valid_Count ==  true)
         {
            QLIB_FTM_SET_MODE_ID(g_hResourceContext, 0);

            if(_bUseSequencer)
            {
               QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, 0);
               printf("QLIB_FTM_SEQ_SetSequencerMode()\n");

               QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );
               printf("QLIB_FTM_SEQ_ActivateBatchMode()\n");
            }

            iPDM = _iInitialPDM;

            // Loop to perform Stress Test to check Sequencer functionality and Heap Size
            for(stress_test_index = 0; stress_test_index < Loop_Count ; stress_test_index++)
            {
               printf("Loop Iteration: %d\n", stress_test_index+1);

               QLIB_FTM_SET_PDM(g_hResourceContext, 2, iPDM  );
               printf("QLIB_FTM_SET_PDM(%d)\n", (2*stress_test_index+1));

               QLIB_FTM_SEQ_ADD_WAIT(g_hResourceContext, 5*ms, &iFTM_Error_Code);
               printf("QLIB_FTM_SEQ_ADD_WAIT(%d)\n", (2*stress_test_index+2));
            }

            QLIB_FTM_SET_PDM(g_hResourceContext, 2, iPDM  );
            printf("QLIB_FTM_SET_PDM(%d)\n", (2*stress_test_index+1));

            QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iSequenceCompleted );
            printf("QLIB_FTM_SEQ_EXECUTE_QUEUE : iSequenceCompleted = %d\n", iSequenceCompleted );

            if(_bUseSequencer)
            {
               QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iFTM_Error_Code );
               printf("QLIB_FTM_SEQ_CLEAR_QUEUE\n");
               QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );
               printf("QLIB_FTM_SEQ_DeactivateBatchMode\n");
            }
         }
      } // End of 'B'
      else if (key == 'C')
      {
         QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\nLogging to file %s", sLogFile );

         printf("\n\n----------- Setting Up GSM DA FTM Sequencer ----------\n\n");

         //--------------- initial settings ----------------//
         unsigned char bOk = false;
         QLIB_FTM_SET_COMMAND_CODE( g_hResourceContext, 75 );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );

         unsigned short i = 0; // index
         unsigned short j = 0;
         unsigned int segLength = 50;
         // --------------------------- create arrays ----------------------------//
         // These arrays are for QLIB_FTM_DA_CAL()
         // they include segmentLength, RGIs, PAs, and GSM_MODE
         unsigned int numSegmentsLowBand  = 193;
         unsigned int numSegmentsHighBand = 129;
         // ---- segment length lists
         unsigned short segLengthListLowBand[193];
         unsigned short segLengthListHighBand[129];
         for (i=0; i<numSegmentsLowBand; ++i)
         {
            segLengthListLowBand[i] = segLength;
         }
         for (i=0; i<numSegmentsHighBand; ++i)
         {
            segLengthListHighBand[i] = segLength;
         }
         // ------- RGI Lists
         unsigned char rgiListLowBand[193];
         unsigned char rgiListHighBand[129];
         rgiListLowBand[0]  = 31;
         rgiListHighBand[0] = 31;
         for (i=1; i<numSegmentsLowBand; i+=2)
         {
            rgiListLowBand[i]   = (unsigned char)j;
            if (i == numSegmentsLowBand-1)
               break;
            rgiListLowBand[i+1] = (unsigned char)j;
            j++;
            if (j == 32)
               j = 0;
         }
         j = 0;
         for (i=1; i<numSegmentsHighBand; i+=2)
         {
            rgiListHighBand[i] = (unsigned char)j;
            if (i == numSegmentsHighBand-1)
               break;
            rgiListHighBand[i+1] = (unsigned char)j;
            j++;
            if (j == 32)
               j = 0;
         }
         // ------- GSM Mode Lists
         unsigned char gsmModeLowBand[193];
         unsigned char gsmModeHighBand[129];
         for (i=0; i<numSegmentsLowBand; i+=2)
         {
            gsmModeLowBand[i]   = (unsigned char)1;
            if (i == numSegmentsLowBand-1)
               break;
            gsmModeLowBand[i+1] = (unsigned char)0;
         }
         for (i=0; i<numSegmentsHighBand; i+=2)
         {
            gsmModeHighBand[i]   = (unsigned char)1;
            if (i == numSegmentsHighBand-1)
               break;
            gsmModeHighBand[i+1] = (unsigned char)0;
         }
         // ------- PA Range Lists
         unsigned char paRangeLowBand[193];
         unsigned char paRangeHighBand[129];
         for (i=0; i<numSegmentsLowBand; ++i)
         {
            if (i<65)
               paRangeLowBand[i] = (unsigned char)0;
            else if (i<129)
               paRangeLowBand[i] = (unsigned char)1;
            else
               paRangeLowBand[i] = (unsigned char)2;
         }
         for (i=0; i<numSegmentsHighBand; ++i)
         {
            if (i<65)
               paRangeHighBand[i] = (unsigned char)0;
            else
               paRangeHighBand[i] = (unsigned char)2;
         }
         // ------ Channel arrays
         // the default channel lists are for easy viewing of the DA Cal output on a spec analyzer
         unsigned short gsm900ChanList[3]  = {37, 39, 41};
         unsigned short gsm850ChanList[3]  = {128, 130, 132};
         unsigned short gsm1800ChanList[3] = {512, 514, 516};
         unsigned short gsm1900ChanList[3] = {512, 514, 516};
         bool useRFCalChanList = false;
         if (useRFCalChanList)                         // use the channel lists actually used in RF Cal
         {
            gsm900ChanList[0]  = 975;
            gsm900ChanList[1]  = 37;
            gsm900ChanList[2]   = 124;
            gsm850ChanList[0]  = 128;
            gsm850ChanList[1]  = 190;
            gsm850ChanList[2]   = 251;
            gsm1800ChanList[0] = 512;
            gsm1800ChanList[1] = 700;
            gsm1800ChanList[2] = 885;
            gsm1900ChanList[0] = 512;
            gsm1900ChanList[1] = 661;
            gsm1900ChanList[2] = 810;
         }

         // --------------- Set Up the FTM Sequencer----------------------//

         // Activate sequence mode for embedded mode
         QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );
         printf("QLIB_FTM_SEQ_SetSequencerMode()\n");
         QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );
         printf("QLIB_FTM_SEQ_ActivateBatchMode()\n");
         QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iFTM_Error_Code );
         printf("QLIB_FTM_SEQ_CLEAR_QUEUE\n");

         // Creating a wait duration for QLIB_FTM_SEQ_ADD_WAIT
         unsigned long c_iGSM_Frame_us = (unsigned long)(4.61538*1000);
         unsigned long c_iGSM_RX_LevelDuration = 18*c_iGSM_Frame_us;
         unsigned short waitStatus = 0;
         unsigned int chanIdx = 0;

         // --------------- Start setting commands to be sequenced----------------------//
         // ------- GSM 900 ------ //
         // RF Mode, GSM900
         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_GSM_900 );
         bOk &= QLIB_FTM_SET_TRANSMIT_BURST( g_hResourceContext,0,0,5,0,1);
         // Channel 1
         chanIdx = 0;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, gsm900ChanList[chanIdx]);
         // other settings
         QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0 );
         QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
         QLIB_FTM_DA_CAL( g_hResourceContext, numSegmentsLowBand,
                          segLengthListLowBand, rgiListLowBand, gsmModeLowBand, paRangeLowBand );
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_RX_LevelDuration, &waitStatus );
         // Channel 2
         chanIdx = 1;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, gsm900ChanList[chanIdx]);
         // other settings
         QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0 );
         QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
         QLIB_FTM_DA_CAL( g_hResourceContext, numSegmentsLowBand,
                          segLengthListLowBand, rgiListLowBand, gsmModeLowBand, paRangeLowBand );
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_RX_LevelDuration, &waitStatus );
         // Channel 3
         chanIdx = 2;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, gsm900ChanList[chanIdx]);
         // other settings
         QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0 );
         QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
         QLIB_FTM_DA_CAL( g_hResourceContext, numSegmentsLowBand,
                          segLengthListLowBand, rgiListLowBand, gsmModeLowBand, paRangeLowBand );
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_RX_LevelDuration, &waitStatus );

         // ------- GSM 850 ------ //
         // RF Mode, GSM850
         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_GSM_850 );
         bOk &= QLIB_FTM_SET_TRANSMIT_BURST( g_hResourceContext,0,0,0,0,1);
         // Channel 1
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );
         chanIdx = 0;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, gsm850ChanList[chanIdx]);
         // other settings
         QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0 );
         QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
         QLIB_FTM_DA_CAL( g_hResourceContext, numSegmentsLowBand,
                          segLengthListLowBand, rgiListLowBand, gsmModeLowBand, paRangeLowBand );
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_RX_LevelDuration, &waitStatus );
         // Channel 2
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );
         chanIdx = 1;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, gsm850ChanList[chanIdx]);
         // other settings
         QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0 );
         QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
         QLIB_FTM_DA_CAL( g_hResourceContext, numSegmentsLowBand,
                          segLengthListLowBand, rgiListLowBand, gsmModeLowBand, paRangeLowBand );
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_RX_LevelDuration, &waitStatus );
         // Channel 3
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );
         chanIdx = 2;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, gsm850ChanList[chanIdx]);
         // other settings
         QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0 );
         QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
         QLIB_FTM_DA_CAL( g_hResourceContext, numSegmentsLowBand,
                          segLengthListLowBand, rgiListLowBand, gsmModeLowBand, paRangeLowBand );
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_RX_LevelDuration, &waitStatus );

         // ------- GSM 1800 ------ //
         // RF Mode, GSM1800
         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_GSM_1800 );
         bOk &= QLIB_FTM_SET_TRANSMIT_BURST( g_hResourceContext,0,0,0,0,1);
         // Channel 1
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );
         chanIdx = 0;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, gsm1800ChanList[chanIdx]);
         // other settings
         QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0 );
         QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
         QLIB_FTM_DA_CAL( g_hResourceContext, numSegmentsHighBand,
                          segLengthListHighBand, rgiListHighBand, gsmModeHighBand, paRangeHighBand );
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_RX_LevelDuration, &waitStatus );
         // Channel 2
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );
         chanIdx = 1;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, gsm1800ChanList[chanIdx]);
         // other settings
         QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0 );
         QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
         QLIB_FTM_DA_CAL( g_hResourceContext, numSegmentsHighBand,
                          segLengthListHighBand, rgiListHighBand, gsmModeHighBand, paRangeHighBand );
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_RX_LevelDuration, &waitStatus );
         // Channel 3
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );
         chanIdx = 2;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, gsm1800ChanList[chanIdx]);
         // other settings
         QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0 );
         QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
         QLIB_FTM_DA_CAL( g_hResourceContext, numSegmentsHighBand,
                          segLengthListHighBand, rgiListHighBand, gsmModeHighBand, paRangeHighBand );
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_RX_LevelDuration, &waitStatus );

         // ------- GSM 1900 ------ //
         // RF Mode, GSM1900
         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_GSM_1900 );
         bOk &= QLIB_FTM_SET_TRANSMIT_BURST( g_hResourceContext,0,0,0,0,1);
         // Channel 1
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );
         chanIdx = 0;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, gsm1900ChanList[chanIdx]);
         // other settings
         QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0 );
         QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
         QLIB_FTM_DA_CAL( g_hResourceContext, numSegmentsHighBand,
                          segLengthListHighBand, rgiListHighBand, gsmModeHighBand, paRangeHighBand );
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_RX_LevelDuration, &waitStatus );
         // Channel 2
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );
         chanIdx = 1;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, gsm1900ChanList[chanIdx]);
         // other settings
         QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0 );
         QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
         QLIB_FTM_DA_CAL( g_hResourceContext, numSegmentsHighBand,
                          segLengthListHighBand, rgiListHighBand, gsmModeHighBand, paRangeHighBand );
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, c_iGSM_RX_LevelDuration, &waitStatus );
         // Channel 3
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );
         chanIdx = 2;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, gsm1900ChanList[chanIdx]);
         // other settings
         QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0 );
         QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
         QLIB_FTM_DA_CAL( g_hResourceContext, numSegmentsHighBand,
                          segLengthListHighBand, rgiListHighBand, gsmModeHighBand, paRangeHighBand );

         //---------------- Execute the FTM Sequencer ---------------//
         unsigned short _iUserVersion = 1;
         QLIB_FTM_SEQ_Flush_Command_Buffer( g_hResourceContext );
         iStatus = QLIB_FTM_SEQ_STATUS( g_hResourceContext, & iFTM_Error_Code, & iQueue_Item_Count, &bIsExecuting, &iNext_ID_Number, & iTimeRef);
         printf("\nPre-execution Status\n");
         printf("QLIB_FTM_SEQ_STATUS: Status= %d, iQueue_Item_Count = %d, bIsExecuting = %d\n\n", iStatus, iQueue_Item_Count, bIsExecuting);

         //QLIB_FTM_SEQ_CreateSequenceFile(g_hResourceContext, "c:\\FTM_TX_SWEEP.bin", _iUserVersion );
         QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred);
         printf("\nFTM Error Code: %d\n", iFTM_Error_Code);
         iStatus = QLIB_FTM_SEQ_STATUS( g_hResourceContext, & iFTM_Error_Code, & iQueue_Item_Count, &bIsExecuting, &iNext_ID_Number, & iTimeRef);

         printf("Post-execution Status\n");
         printf("QLIB_FTM_SEQ_STATUS: Status= %d, iQueue_Item_Count = %d, bIsExcuting = %d\n\n", iStatus, iQueue_Item_Count, bIsExecuting);

         QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iFTM_Error_Code);
         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );
         QLIB_StopLogging( g_hResourceContext );
      } // END of 'C'

      printf("Press a key to continue\n");
      while(!kbhit());
   }
   return true;
}
/*
\param iWaitPeriod = total wait period in milliseconds
\param c_bUseSmallWaits = TRUE to use small waits, false to use a single large wait
\param c_iSmallWaitPeriod = duration of the small wait period (if c_bUseSmallWaits == TRUE)
\param pStatus = pointer to the result status data
*/
unsigned char Test_FTM_Sequencer_AddWait(
   unsigned long iWaitPeriod,
   unsigned char bUseSmallWaits,
   unsigned short* pStatus)
{
   unsigned long iWaitRemaining = 0;
   unsigned long iWaited = 0;
   unsigned long ms = 1000;
   unsigned char bOk = FALSE;
   unsigned long _iSmallWaitPeriod = 0;

   if ( bUseSmallWaits )
   {
      // 80%
      _iSmallWaitPeriod = ( iWaitPeriod * 80);
      _iSmallWaitPeriod = _iSmallWaitPeriod / 100;

      // Only one ADD_WAIT
      bOk = QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, _iSmallWaitPeriod*ms, pStatus );

      // Remainder
      _iSmallWaitPeriod = ( iWaitPeriod - _iSmallWaitPeriod);

      if ( _iSmallWaitPeriod > 0 )
      {
         // Only one ADD_WAIT
         bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, _iSmallWaitPeriod*ms, pStatus );
      }
   }
   else
   {
      // Only one ADD_WAIT
      bOk = QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, iWaitPeriod*ms, pStatus );
   }
   return bOk;
}

unsigned char Test_RF_Verification( void )
{
   unsigned short ms = 1000; // 1000 us = 1ms;
   char key = '0';
   char* sLogFile = "C:\\ftm_rf_verification_test.txt";

   while(key != 'Q')
   {
      printf("\nTest_RF_Verification\n");
      printf("=====================\n");
      printf(" 1 - FTM Sequencer - CDMA RF Verification Test\n");
      printf(" 2 - FTM Sequencer - WCDMA RF Verification Test\n");
      printf(" 3 - FTM Sequencer - LTE RF Verification Test - TX\n");
      printf(" 4 - FTM Sequencer - GSM RF Verification Test\n");
      printf(" 5 - FTM Sequencer - TDS-CDMA RF Verification Test\n"); /* Work in porgress - will be updated over next few weeks */
      printf(" 6 - FTM Sequencer - TDD-LTE RF Verification Test\n");  /* Work in porgress - will be updated over next few weeks */
      printf(" 7 - FTM RFFE Get Device Setting API\n");
      printf(" Q - Quit\n");
      printf(" -->");

      key = toupper(getch());

      unsigned short iFTM_Error_Code = 0;
      unsigned long iQueue_Item_Count = 0;
      unsigned char  bIsExecuting = 0;
      unsigned long iNext_ID_Number = 0;
      unsigned long iTimeRef = 0;
      unsigned char iStatus = 0;
      unsigned char iTimeOutOcurred = 0;

      if(key == 'Q')
      {
         break;
      }
      else if (key == '1')  // CDMA RF Verification
      {
         /*
         Equipment Setup
         CDMA
         For Band: BC0
         Channel: 50
         UL Freq. = 826.5 MHz
         DL Freq. = 871.5 MHz
         Select the appropriate CDMA ARB file
         */
         unsigned short ms = 1000;

         // CDMA Band and Channel information
         short iModeID = FTM_MODE_ID_CDMA_1X; /* CDMA = 0 */
         short iBand = PHONE_MODE_CDMA_800; /* For CDMA Band 0*/
         unsigned short iChannel = 50;

         //
         const int TxPowerCount = 3;
         const int RxMeasCount = 10;

         //Parameters for Set Tx Power DBM
         unsigned char iEnable = TRUE;
         short iPowerLevel[5] = {230, 100, 0, -200, -400};

         //Parameters for Get Rx Power DBM
         short iRX_AGC[TxPowerCount*RxMeasCount] = {0};
         short iRX_Power_dBm[TxPowerCount*RxMeasCount] = {0};

         // _AddWait: Adds add wait calls in FTM_Sequencer when set to 1
         int _AddWait = 0;

         //Delay Profiling between each command
         //int Delay_After_Start = 200;
         int Delay_After_Set_Mode_ID   = 50;
         int Delay_After_Set_Mode      = 50;
         int Delay_After_Set_Channel   = 50;
         int Delay_After_Tx_ON         = 50;
         int Delay_After_Set_Tx_Power  = 500;
         int Delay_After_Get_Rx_Level  = 100;
         int Delay_After_Tx_OFF        = 50;

         // Function return values
         unsigned char bOk = TRUE;

         // LTE Command status
         unsigned short iCLEAR_QUEUE_Status                            =    DIAG_FTM_STATUS_FAIL;
         unsigned short iADD_WAIT_Status                               =    DIAG_FTM_STATUS_FAIL;
         unsigned short iSet_TX_Power_Status[TxPowerCount]             =    {DIAG_FTM_STATUS_FAIL};
         unsigned short iGet_RX_Level_Status[TxPowerCount*RxMeasCount] =    {DIAG_FTM_STATUS_FAIL};

         int i = 0;
         for(i=0; i<TxPowerCount; i++)
         {
            iSet_TX_Power_Status[i] = DIAG_FTM_STATUS_FAIL;
         }
         for(i=0; i<(TxPowerCount*RxMeasCount); i++)
         {
            iGet_RX_Level_Status[i] = DIAG_FTM_STATUS_FAIL;
         }

         // Flag to enable F3 messages
         unsigned char _bEnableF3Messages= false;

         //Parameters to save Embedded sequencer logs in EFS
         unsigned char filename[40] = "RF_VER_CDMA_";
         int Temp = 1;

         int TxLoop = 0;
         int RxLoop = 0;

         printf("\nExecuting CDMA FTM RF Verification Sequence on Band = %d : channel = %d", iBand, iChannel );

         // Start text log
         sLogFile = "C:\\ftm_rf_verification_test_CDMA.txt";
         bOk &= QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\n\nLogging to file %s", sLogFile );

         if ( _bEnableF3Messages )
         {
            // Activate FTM F3 Messages
            bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, 23, 0xFF );
         }

         // Select embedded sequence mode
         bOk &= QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );

         // Enter the batch mode, to queue up subsequent FTM commands
         bOk &= QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );

         bOk &= QLIB_FTM_SET_MODE_ID( g_hResourceContext, iModeID );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Mode_ID*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, iBand);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Mode*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_CHAN(g_hResourceContext, iChannel);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Channel*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_TX_ON( g_hResourceContext );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_ON*ms, &iADD_WAIT_Status );

         for(TxLoop = 0; TxLoop < TxPowerCount; TxLoop ++)
         {
            bOk &= QLIB_FTM_SET_TX_POWER_DBM(g_hResourceContext , iEnable,iPowerLevel[0], &iSet_TX_Power_Status[TxLoop]);
            bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Tx_Power*ms, &iADD_WAIT_Status );

            for(RxLoop = 0; RxLoop < RxMeasCount; RxLoop++)
            {
               bOk &= QLIB_FTM_GET_RX_LEVEL_DBM(g_hResourceContext , &iRX_AGC[TxLoop*RxMeasCount + RxLoop], &iRX_Power_dBm[TxLoop*RxMeasCount + RxLoop], &iGet_RX_Level_Status[TxLoop*RxMeasCount + RxLoop]);
               bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Get_Rx_Level*ms, &iADD_WAIT_Status );
            }
         }

         bOk &= QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_OFF*ms, &iADD_WAIT_Status );

         ///////
         // Execute sequence
         ////////
         bOk &= QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         /*
         Note:- The code controlled by #if condition below is not a part of regular test scenario.
         This piece of code is only enabled for debug purpose i.e. to store the sequence data on UE's EFS memory
         */
#if 0
         ///////////////////////////////////////////////////////////
         //Preparing Test case for QLIB_FTM_STORE_DEBUG_INFO_TO_FILE
         ///////////////////////////////////////////////////////////

         itoa(Temp, Count, 10);
         strcat((char *)filename, Count);
         printf("\nFilename = %s\n", filename);
         Temp++;

         QLIB_FTM_SEQ_STORE_DEBUG_INFO_TO_FILE( g_hResourceContext, filename, &iFTM_Error_Code);
         printf("QLIB_FTM_SEQ_STORE_DEBUG_INFO_TO_FILE --> Filename: %s : FTM_ERROR_CODE = %d\n", filename , iFTM_Error_Code);

#endif

         QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iFTM_Error_Code );
         printf("QLIB_FTM_SEQ_CLEAR_QUEUE\n");

         // Turn off sequencer
         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         ///////
         // Print results
         ////////

         // Function return values
         printf("\n\nFunction return status: %s\n", bOk  ? "PASS" : "FAIL" );
         // Response status For Conf UL Waveform
         printf("\n\t**********************************\n");
         printf("\t***** Results for CDMA RF Verification *****");
         printf("\n\t*****      Band = %d : Channel = %d     *****", iBand , iChannel);

         for(TxLoop = 0; TxLoop < TxPowerCount; TxLoop ++)
         {
            printf("\n\t**********************************\n\n");
            // Response status For  Set Tx Power
            printf("\n\t\t[%d] --> TX Power = %d\tStatus: %s", TxLoop+1
                   , iPowerLevel[TxLoop]
                   , iSet_TX_Power_Status[TxLoop] == DIAG_FTM_STATUS_SUCCESS? "FAIL" : "PASS" );

            for(RxLoop = 0; RxLoop < RxMeasCount; RxLoop++)
            {
               // Response status For Get Rx Level
               printf("\n\t\t\t[%d] --> RxAGC = %d\tRx_Power_dBm = %d\tStatus: %s", RxLoop+1
                      , (iRX_AGC[TxLoop*RxMeasCount + RxLoop]/64)
                      , (iRX_Power_dBm[TxLoop*RxMeasCount + RxLoop] - 65535)
                      , iGet_RX_Level_Status[TxLoop*RxMeasCount + RxLoop] == DIAG_FTM_STATUS_SUCCESS? "FAIL" : "PASS" );
            }
         }

         printf("\n");
         printf("\n\t**********************************");

         QLIB_StopLogging( g_hResourceContext );
      } // end of CDMA RF Verification

      else if (key == '2')  // WCDMA RF Verification
      {
         /*
         Equipment Setup
         WCDMA
         For Band: Babd 1
         Channel: 9612
         UL Freq. = 1922.4 MHz
         DL Freq. = 2112.4 MHz
         Select the appropriate WCDMA ARB file
         */
         unsigned short ms = 1000;

         // WCDMA Band and Channel information
         short iModeID = FTM_MODE_ID_WCDMA; /* WCDMA = 1 */
         short iBand = PHONE_MODE_WCDMA_IMT; /* For WCDMA Band 1*/
         unsigned short iChannel = 9612;

         //
         const int RxMeasCount = 10;

         //Parameters for Set Tx Power DBM
         unsigned char iEnable = TRUE;
         short iPowerLevel[5] = {230, 100, 0, -200, -400};

         //Parameters for Get Rx Power DBM
         short iRX_AGC[RxMeasCount] = {0};
         short iRX_Power_dBm[RxMeasCount] = {0};

         // _AddWait: Adds add wait calls in FTM_Sequencer when set to 1
         int _AddWait = 0;

         //Delay Profiling between each command
         //int Delay_After_Start = 200;
         int Delay_After_Set_Mode_ID   = 50;
         int Delay_After_Set_Mode      = 50;
         int Delay_After_Set_Channel   = 50;
         int Delay_After_Tx_ON         = 50;
         int Delay_After_Set_Tx_Power  = 1000;
         int Delay_After_Get_Rx_Level  = 100;
         int Delay_After_Tx_OFF        = 50;

         // Function return values
         unsigned char bOk = TRUE;

         // LTE Command status
         unsigned short iCLEAR_QUEUE_Status               = DIAG_FTM_STATUS_FAIL;
         unsigned short iADD_WAIT_Status                  = DIAG_FTM_STATUS_FAIL;
         unsigned short iSet_TX_Power_Status              = DIAG_FTM_STATUS_FAIL;
         unsigned short iGet_RX_Level_Status[RxMeasCount] = {DIAG_FTM_STATUS_FAIL};

         int i = 0;
         for(i=0; i<RxMeasCount; i++)
         {
            iGet_RX_Level_Status[i] = DIAG_FTM_STATUS_FAIL;
         }

         // Flag to enable F3 messages
         unsigned char _bEnableF3Messages= false;

         //Parameters to save Embedded sequencer logs in EFS
         unsigned char filename[40] = "RF_VER_WCDMA_";
         int Temp = 1;
         int RxLoop = 0;

         printf("\nExecuting WCDMA FTM RF Verification Sequence on Band = %d : channel = %d", iBand, iChannel );

         // Start text log
         sLogFile = "C:\\ftm_rf_verification_test_WCDMA.txt";
         bOk &= QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\n\nLogging to file %s", sLogFile );

         if ( _bEnableF3Messages )
         {
            // Activate FTM F3 Messages
            bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, 23, 0xFF );
         }

         // Select embedded sequence mode
         bOk &= QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );

         // Enter the batch mode, to queue up subsequent FTM commands
         bOk &= QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );

         bOk &= QLIB_FTM_SET_MODE_ID( g_hResourceContext, iModeID );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Mode_ID*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, iBand);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Mode*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_CHAN(g_hResourceContext, iChannel);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Channel*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_TX_ON( g_hResourceContext );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_ON*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_TX_POWER_DBM(g_hResourceContext , iEnable,iPowerLevel[0], &iSet_TX_Power_Status);
         if(_AddWait)  bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Tx_Power*ms, &iADD_WAIT_Status );

         for(RxLoop = 0; RxLoop < RxMeasCount; RxLoop++)
         {
            bOk &= QLIB_FTM_GET_RX_LEVEL_DBM(g_hResourceContext , &iRX_AGC[RxLoop], &iRX_Power_dBm[RxLoop], &iGet_RX_Level_Status[RxLoop]);
            if(_AddWait)  bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Get_Rx_Level*ms, &iADD_WAIT_Status );
         }

         bOk &= QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_OFF*ms, &iADD_WAIT_Status );

         ///////
         // Execute sequence
         ////////

         bOk &= QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         /*
         Note:- The code controlled by #if condition below is not a part of regular test scenario.
         This piece of code is only enabled for debug purpose i.e. to store the sequence data on UE's EFS memory
         */
#if 0
         ///////////////////////////////////////////////////////////
         //Preparing Test case for QLIB_FTM_STORE_DEBUG_INFO_TO_FILE
         ///////////////////////////////////////////////////////////

         itoa(Temp, Count, 10);
         strcat((char *)filename, Count);
         printf("\nFilename = %s\n", filename);
         Temp++;

         QLIB_FTM_SEQ_STORE_DEBUG_INFO_TO_FILE( g_hResourceContext, filename, &iFTM_Error_Code);
         printf("QLIB_FTM_SEQ_STORE_DEBUG_INFO_TO_FILE --> Filename: %s : FTM_ERROR_CODE = %d\n", filename , iFTM_Error_Code);
#endif
         QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iFTM_Error_Code );
         printf("QLIB_FTM_SEQ_CLEAR_QUEUE\n");

         // Turn off sequencer
         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         ///////
         // Print results
         ////////

         // Function return values
         printf("\n\nFunction return status: %s\n", bOk  ? "PASS" : "FAIL" );
         // Response status For Conf UL Waveform
         printf("\n\t**********************************\n");
         printf("\t***** Results for WCDMA RF Verification *****");
         printf("\n\t*****      Band = %d : Channel = %d     *****", iBand , iChannel);

         printf("\n\t**********************************\n\n");
         // Response status For  Set Tx Power
         printf("\n\t\t[%d] --> TX Power = %d\tStatus: %s", 1
                , iPowerLevel[0]
                , iSet_TX_Power_Status == DIAG_FTM_STATUS_SUCCESS? "FAIL" : "PASS" );

         for(RxLoop = 0; RxLoop < RxMeasCount; RxLoop++)
         {
            // Response status For Get Rx Level
            printf("\n\t\t\t[%d] --> RxAGC = %d\tRx_Power_dBm = %d\tStatus: %s", RxLoop+1
                   , (iRX_AGC[RxLoop]- 65535)
                   , (iRX_Power_dBm[RxLoop] - 65535)
                   , iGet_RX_Level_Status[RxLoop] == DIAG_FTM_STATUS_SUCCESS? "FAIL" : "PASS" );
         }

         printf("\n");
         printf("\n\t**********************************");

         QLIB_StopLogging( g_hResourceContext );
      } // end of CDMA RF Verification

      else if (key == '3')  // LTE RF Verification
      {
         /*
         Equipment Setup
         LTE
         For Band: BC4
         Channel: 20160
         UL Freq. = 1731 MHz
         DL Freq. = 2131 MHz
         Select the appropriate LTE ARB file
         */
         unsigned short ms = 1000;

         // LTE Band and Channel information
         short iModeID = FTM_MODE_ID_LTE; /* LTE = 29 */
         short iBand = PHONE_MODE_LTE_B4; /* For LTE BC 4 */
         unsigned short iChannel = 20160;

         // Parameters for LTE UL and DL BandWidth settings
         //      0 - RFCOM_BW_LTE_1P4MHz
         //         1 - RFCOM_BW_LTE_3MHz
         //         2 - RFCOM_BW_LTE_5MHz
         //         3 - RFCOM_BW_LTE_10MHz
         //         4 - RFCOM_BW_LTE_15MHz
         //         5 - RFCOM_BW_LTE_20MHz
         unsigned char itxChanBW[6] = {0,1,2,3,4,5};
         unsigned char irxChanBW[6] = {0,1,2,3,4,5};

         //Parameters for set Modulation Type
         //0 - QPSK, 1 - 16 QAM, 2 - 64 QAM
         unsigned long iModulationType  = 0;

         //Parameters to set Tx Waveform
         //0-CW, 1- PUSCH, 2- PUCCH, 3 - PRACH, 4 - SRS, 5 - UpPTS
         unsigned char iTxWaveform         = 1;
         unsigned char inumRBsPUSCH        = 50;
         unsigned char inumRBsPUCCH        = 0;
         unsigned char iPUSCHStartRBIndex  = 0;

         //
         const int TxPowerCount = 5;
         const int RxMeasCount  = 10;

         // Parameters for Set NS command
         unsigned char iNS = 1;

         //Parameters for Set Tx Power DBM
         unsigned char iEnable = TRUE;
         short iPowerLevel[5] = {230, 100, 0, -200, -400};

         //Parameters for Set LNA State
         short iExpected_RX_Power_dBm[5] = {-800, -600, -500, -400, -300};
         unsigned char iSelected_LNA_Range[RxMeasCount] = {9};

         //Parameters for Get Rx Power DBM
         short iRX_AGC[RxMeasCount] = {0};
         short iRX_Power_dBm[RxMeasCount] = {0};

         // _AddWait: Adds add wait calls in FTM_Sequencer when set to 1
         int _AddWait = 0;

         //Delay Profiling between each command
         //int Delay_After_Start = 200;
         int Delay_After_Set_Mode_ID         = 50;
         int Delay_After_Set_Mode            = 50;
         int Delay_After_Tx_BW               = 50;
         int Delay_After_Rx_BW               = 50;
         int Delay_After_Set_Channel         = 50;
         int Delay_After_Set_Modulation_Type = 50;
         int Delay_After_Tx_ON               = 50;
         int Delay_After_Set_Tx_Waveform     = 50;
         int Delay_After_Set_NS              = 50;
         int Delay_After_Set_Tx_Power        = 500;
         int Delay_After_Set_LNA_State       = 50;
         int Delay_After_Get_Rx_Level        = 100;
         int Delay_After_Tx_OFF              = 50;

         // Function return values
         unsigned char bOk = TRUE;

         // LTE Command status
         unsigned short iCLEAR_QUEUE_Status                    = DIAG_FTM_STATUS_FAIL;
         unsigned short iADD_WAIT_Status                       = DIAG_FTM_STATUS_FAIL;
         unsigned short iSet_Net_Sig_Status                    = DIAG_FTM_STATUS_FAIL;
         unsigned short iSet_TX_Power_Status                   = DIAG_FTM_STATUS_FAIL;
         unsigned short iSet_LNA_State_DBM_Status[RxMeasCount] = {DIAG_FTM_STATUS_FAIL};
         unsigned short iGet_RX_Level_Status[RxMeasCount]      = {DIAG_FTM_STATUS_FAIL};

         int i = 0;
         for(i=0; i<RxMeasCount; i++)
         {
            iGet_RX_Level_Status[i] = DIAG_FTM_STATUS_FAIL;
         }

         // Flag to enable F3 messages
         unsigned char _bEnableF3Messages= false;

         //Parameters to save Embedded sequencer logs in EFS
         unsigned char filename[40] = "RF_VER_LTE_";
         int Temp = 1;

         int TxLoop = 0;
         int RxLoop = 0;

         printf("\nExecuting LTE FTM RF Verification Sequence on Band = %d : channel = %d", iBand, iChannel );

         // Start text log
         sLogFile = "C:\\ftm_rf_verification_test_LTE.txt";
         bOk &= QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\n\nLogging to file %s", sLogFile );

         if ( _bEnableF3Messages )
         {
            // Activate FTM F3 Messages
            bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, 23, 0xFF );
         }

         // Select embedded sequence mode
         bOk &= QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );

         // Enter the batch mode, to queue up subsequent FTM commands
         bOk &= QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );

         bOk &= QLIB_FTM_SET_MODE_ID( g_hResourceContext, iModeID );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Mode_ID*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, iBand);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Mode*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_LTE_SET_TX_BANDWIDTH( g_hResourceContext, itxChanBW[3]);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_BW*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_LTE_SET_RX_BANDWIDTH( g_hResourceContext, irxChanBW[3]);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Rx_BW*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_CHAN(g_hResourceContext, iChannel);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Channel*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_LTE_SET_TX_MODULATION_TYPE(g_hResourceContext, iModulationType);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Modulation_Type*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_LTE_SET_TX_WAVEFORM(g_hResourceContext, iTxWaveform, inumRBsPUSCH, inumRBsPUCCH, iPUSCHStartRBIndex );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Tx_Waveform*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_TX_ON( g_hResourceContext );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_ON*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_LTE_SET_NET_SIG_VALUE(g_hResourceContext, iNS, &iSet_Net_Sig_Status);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_NS*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_TX_POWER_DBM(g_hResourceContext , iEnable, iPowerLevel[0], &iSet_TX_Power_Status);
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Tx_Power*ms, &iADD_WAIT_Status );
         //if(_AddWait) bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Tx_Power*ms, &iADD_WAIT_Status );

         for(RxLoop = 0; RxLoop < RxMeasCount; RxLoop++)
         {
            bOk &= QLIB_FTM_LTE_SET_LNA_STATE_DBM (g_hResourceContext , iExpected_RX_Power_dBm[1], &iSelected_LNA_Range[RxLoop], &iSet_LNA_State_DBM_Status[RxLoop]);
            if(_AddWait)    bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_LNA_State*ms, &iADD_WAIT_Status );

            bOk &= QLIB_FTM_GET_RX_LEVEL_DBM(g_hResourceContext , &iRX_AGC[RxLoop], &iRX_Power_dBm[RxLoop], &iGet_RX_Level_Status[RxLoop]);
            if(_AddWait)    bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Get_Rx_Level*ms, &iADD_WAIT_Status );
         }

         bOk &= QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_OFF*ms, &iADD_WAIT_Status );

         ///////
         // Execute sequence
         ////////
         bOk &= QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         /*
         Note:- The code controlled by #if condition below is not a part of regular test scenario.
         This piece of code is only enabled for debug purpose i.e. to store the sequence data on UE's EFS memory
         */
#if 0

         ///////////////////////////////////////////////////////////
         //Preparing Test case for QLIB_FTM_STORE_DEBUG_INFO_TO_FILE
         ///////////////////////////////////////////////////////////

         itoa(Temp, Count, 10);
         strcat((char *)filename, Count);
         printf("\nFilename = %s\n", filename);
         Temp++;

         QLIB_FTM_SEQ_STORE_DEBUG_INFO_TO_FILE( g_hResourceContext, filename, &iFTM_Error_Code);
         printf("QLIB_FTM_SEQ_STORE_DEBUG_INFO_TO_FILE --> Filename: %s : FTM_ERROR_CODE = %d\n", filename , iFTM_Error_Code);

#endif

         QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iFTM_Error_Code );
         printf("QLIB_FTM_SEQ_CLEAR_QUEUE\n");

         // Turn off sequencer
         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         ///////
         // Print results
         ////////

         // Function return values
         printf("\n\nFunction return status: %s\n", bOk  ? "PASS" : "FAIL" );
         // Response status For Conf UL Waveform
         printf("\n\t**********************************\n");
         printf("\t***** Results for LTE RF Verification *****");
         printf("\n\t*****      Band = %d : Channel = %d : Modulation Type = %d    *****", iBand , iChannel, iModulationType);

         printf("\n\t**********************************\n\n");
         // Response status For  Set Tx Power
         printf("\n\t[%d] --> TX Power = %d\tStatus: %s\n", 1
                , iPowerLevel[0]
                , iSet_TX_Power_Status == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );

         for(RxLoop = 0; RxLoop < RxMeasCount; RxLoop++)
         {
            // Response status For Get Rx Level
            printf("\n\t\t[%d] --> LNA State = %d\tRxAGC = %d\tRx_Power_dBm = %d\tStatus: %s", RxLoop+1,
                   iSelected_LNA_Range[RxLoop],
                   (iRX_AGC[RxLoop]/64),
                   (iRX_Power_dBm[RxLoop] - 65535),
                   ((iGet_RX_Level_Status[RxLoop] || iSet_LNA_State_DBM_Status[RxLoop]) == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL") );
         }

         printf("\n");
         printf("\n\t**********************************");

         QLIB_StopLogging( g_hResourceContext );
      } // end of LTE RF Verification
      else if (key == '4')  // GSM - RF Verification
      {
         /*
         Equipment Setup
         GSM
         For Band: GSM1800
         Channel:  550
         UL Freq. = 1717.8 MHz
         DL Freq. = 1812.8 MHz
         Select the appropriate ARB file
         */
         unsigned short ms = 1000;

         // GSM Band and Channel information
         short iModeID = FTM_MODE_ID_GSM_EXTENDED_C; /* GSM = 17 */
         short iBand = PHONE_MODE_GSM_1800;         /* For Band = GSM1800*/
         unsigned short iChannel = 550;

         const int RxMeasCount = 5;

         // Parameter for Set_Tx_Frame_Matrix
         unsigned char iSlotNum[8]   = {0,1,2,3,4,5,6,7};
         unsigned char iTurnSlotOn[8] = {1,0,0,0,0,0,0,0};
         unsigned short iPwrLevel[8]  = {3000,0,0,0,0,0,0,0};
         unsigned char iModScheme[8]  = {1,0,0,0,0,0,0,0};
         int No_of_Slots = 8;

         // Parameters for SET_TRANSMIT_BURST
         unsigned char iSlotNumber          = 0;
         short iDataSource                  = FTM_GSM_TX_DATA_SOURCE_PSDRND;
         unsigned char iTSCindex                = 0;
         unsigned long iNumBursts           = 4;
         unsigned char bIsInfiniteDuration  = false;

         // Parameters for Set Rx LNA Range -- FTM_GSM_SETUP_RX_BURST_FOR_EXPECTED_PWR API
         /*-100dBm,-90dBm,-80dBm,-70dBm,-60dBm,-50dBm */
         int iExpected_RX_Power_dbm16[6] = {-1600, -1440, -1280, -1120, -960, -800};
         unsigned short iNumber_of_Averages = 2;

         //Parameters for Get Rx Power -- FTM_GSM_GET_MEAS_PWR  API
         unsigned long iRX_Raw_RSSI[5*RxMeasCount] = {0};
         unsigned short iRX_Power_dBm16[5*RxMeasCount] = {0};

         // _AddWait: Adds add wait calls in FTM_Sequencer when set to 1
         int _AddWait = 0;

         //Delay Profiling between each command
         int Delay_After_Set_Mode_ID         = 50;
         int Delay_After_Set_Mode            = 50;
         int Delay_After_Set_Channel         = 50;
         int Delay_After_Tx_ON               = 50;
         int Delay_After_Set_Frame_Matrix    = 100;
         int Delay_After_Set_Transmit_Burst  = 100;
         int Delay_After_Set_Rx_Burst        = 30;
         int Delay_After_Get_Rx_Meas         = 100;
         int Delay_After_Tx_OFF              = 50;

         // Function return values
         unsigned char bOk = TRUE;

         // GSM Command status
         unsigned short iCLEAR_QUEUE_Status                  = DIAG_FTM_STATUS_FAIL;
         unsigned short iADD_WAIT_Status                     = DIAG_FTM_STATUS_FAIL;
         unsigned short iSet_LNA_State_Status[5]             = {DIAG_FTM_STATUS_FAIL};
         unsigned short iGet_RX_Level_Status[5*RxMeasCount]  = {DIAG_FTM_STATUS_FAIL};

         int i = 0;
         for(i=0; i<(RxMeasCount); i++)
         {
            iGet_RX_Level_Status[i] = DIAG_FTM_STATUS_FAIL;
         }
         for(i=0; i<5; i++)
         {
            iSet_LNA_State_Status[i] = DIAG_FTM_STATUS_FAIL;
         }

         // Flag to enable F3 messages
         unsigned char _bEnableF3Messages= false;

         //Parameters to save Embedded sequencer logs in EFS
         unsigned char filename[40] = "RF_VER_GSM_";
         int Temp = 1;

         int TxLoop = 0;
         int SlotLoop = 0;
         int RxLoop = 0;
         int Power_Level_Loop=0;

         printf("\nExecuting GSM FTM RF Verification Sequence on Band = %d : channel = %d", iBand, iChannel );

         // Start text log
         sLogFile = "C:\\ftm_rf_verification_test_GSM_Rx.txt";
         bOk &= QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\n\nLogging to file %s", sLogFile );

         if ( _bEnableF3Messages )
         {
            // Activate FTM F3 Messages
            bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, 23, 0xFF );
         }

         // Select embedded sequence mode
         bOk &= QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );

         // Enter the batch mode, to queue up subsequent FTM commands
         bOk &= QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );

         bOk &= QLIB_FTM_SET_MODE_ID( g_hResourceContext, iModeID );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Mode_ID*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, iBand);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Mode*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_CHAN(g_hResourceContext, iChannel);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Channel*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_TX_ON(g_hResourceContext);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_ON*ms, &iADD_WAIT_Status );

         //Tx Tests
         for(SlotLoop=0; SlotLoop<No_of_Slots; SlotLoop++)
         {
            /* Sets individual Slots */
            bOk &= QLIB_FTM_SET_TX_FRAME_MATRIX(g_hResourceContext, iSlotNum[SlotLoop], iTurnSlotOn[SlotLoop], iPwrLevel[SlotLoop], iModScheme[SlotLoop]);
            if(_AddWait)    bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Frame_Matrix*ms, &iADD_WAIT_Status );
         }

         bOk &= QLIB_FTM_SET_TRANSMIT_BURST(g_hResourceContext, iSlotNumber, iDataSource, iTSCindex,iNumBursts, bIsInfiniteDuration );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Transmit_Burst*ms, &iADD_WAIT_Status );

         // Rx Tests
         for (Power_Level_Loop=0; Power_Level_Loop<3; Power_Level_Loop++)
         {
            bOk &= QLIB_FTM_GSM_SETUP_RX_BURST_FOR_EXPECTED_PWR(g_hResourceContext , iExpected_RX_Power_dbm16[1], iNumber_of_Averages, &iSet_LNA_State_Status[Power_Level_Loop]);
            bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Rx_Burst*ms, &iADD_WAIT_Status );
            //Sleep(15);  /* Minimum wait time between these two command */

            for(RxLoop = 0; RxLoop < RxMeasCount; RxLoop++)
            {
               bOk &= QLIB_FTM_GSM_GET_MEAS_PWR (g_hResourceContext , &iRX_Power_dBm16[Power_Level_Loop*RxMeasCount + RxLoop], &iRX_Raw_RSSI[Power_Level_Loop*RxMeasCount + RxLoop], &iGet_RX_Level_Status[Power_Level_Loop*RxMeasCount + RxLoop]);
               if(_AddWait) bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Get_Rx_Meas*ms, &iADD_WAIT_Status );
            }
         }

         bOk &= QLIB_FTM_SET_TX_OFF(g_hResourceContext);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_OFF*ms, &iADD_WAIT_Status );
         ///////
         // Execute sequence
         ////////
         bOk &= QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         /*
         Note:- The code controlled by #if condition below is not a part of regular test scenario.
         This piece of code is only enabled for debug purpose i.e. to store the sequence data on UE's EFS memory
         */
#if 0
         ///////////////////////////////////////////////////////////
         //Preparing Test case for QLIB_FTM_STORE_DEBUG_INFO_TO_FILE
         ///////////////////////////////////////////////////////////

         itoa(Temp, Count, 10);
         strcat((char *)filename, Count);
         printf("\nFilename = %s\n", filename);
         Temp++;

         QLIB_FTM_SEQ_STORE_DEBUG_INFO_TO_FILE( g_hResourceContext, filename, &iFTM_Error_Code);
         printf("QLIB_FTM_SEQ_STORE_DEBUG_INFO_TO_FILE --> Filename: %s : FTM_ERROR_CODE = %d\n", filename , iFTM_Error_Code);
#endif

         QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iFTM_Error_Code );
         //printf("QLIB_FTM_SEQ_CLEAR_QUEUE\n");

         // Turn off sequencer
         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         ///////
         // Print results
         ////////

         // Function return values
         printf("\n\nFunction return status: %s\n", bOk  ? "PASS" : "FAIL" );
         // Response status For Conf UL Waveform
         printf("\n\t**********************************\n");
         printf("\t***** Results for GSM RF Verification - Rx *****");
         printf("\n\t*****      Band = %d : Channel = %d     *****", iBand , iChannel);

         printf("\n\t**********************************\n\n");
         printf("\n\tSet Tx ON - Done");
         printf("\n\tSet Tx Frame Matrix - Done");
         for(SlotLoop=0; SlotLoop<No_of_Slots; SlotLoop++)
         {
            printf("\n\t\t Slot - %d\t %s \t %d \t MCS - %d", SlotLoop+1, iTurnSlotOn[SlotLoop]?"ON":"OFF", iPwrLevel[SlotLoop], iModScheme[SlotLoop]);
         }
         printf("\n\tSet Tx Burst - Done");
         for(Power_Level_Loop = 0; Power_Level_Loop < 3; Power_Level_Loop++)
         {
            printf("\n\t**********************************\n\n");
            // Response status For  Set LNA State Power
            printf("\n\t[%d] --> Expected Rx Power = %d dBm\t%d dBm16\tStatus: %s", Power_Level_Loop+1,
                   iExpected_RX_Power_dbm16[1]/16,
                   iExpected_RX_Power_dbm16[1],
                   iSet_LNA_State_Status[Power_Level_Loop] == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
            for(RxLoop = 0; RxLoop < RxMeasCount; RxLoop++)
            {
               // Response status For Get Rx Level
               printf("\n\t\t[%d] --> Rx_Power_dBm16 = %d\tRxAGC = %ld\tStatus: %s", Power_Level_Loop*RxMeasCount+RxLoop+1,
                      (short)iRX_Power_dBm16[Power_Level_Loop*RxMeasCount + RxLoop]/16 ,
                      (long)iRX_Raw_RSSI[Power_Level_Loop*RxMeasCount + RxLoop],
                      iGet_RX_Level_Status[Power_Level_Loop*RxMeasCount + RxLoop] == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );
            }
         }

         printf("\n");
         printf("\n\t**********************************");

         QLIB_StopLogging( g_hResourceContext );
      } // end of CDMA RF Verification

      else if (key == '5')  // TD-SCDMA - RF Verification
      {
         /*
         Equipment Setup
         TDS
         For Band: Band34
         Channel:  10080
         UL Freq. = 2016 MHz
         DL Freq. = 2016 MHz
         Select the appropriate ARB file
         */
         unsigned short ms = 1000;

         // GSM Band and Channel information
         short iModeID  =  FTM_MODE_ID_TDSCDMA;       /* TDS-CDMA = 42 */
         short iBand    =  PHONE_MODE_TDSCDMA_B34;    /* For Band = TDSCDMA34*/
         unsigned short iChannel = 10080;

         // Parameter for Set_Burst_Tx_Params
         unsigned char iNumSlots = 1;
         /* 0 - QPSK
         1 - 16-QAM
         */
         unsigned char iModulationType = 0;
         /* Dynamic range for Power on each time slot as per the specification = -50dBm to 23dBm*/
         unsigned short iPowerTS1 = 200;
         unsigned short iPowerTS2 = 0;
         unsigned short iPowerTS3 = 0;
         unsigned short iPowerTS4 = 0;
         unsigned short iPowerTS5 = 0;

         // Parameters for Burst_Tx
         unsigned char iEnable_Tx = 0;

         // Parameters for Burst_Rx
         /* Dynamic range for DL level as per the specification = -1100dBm to -350dBm */
         unsigned short iExpectedDLLevel = 0;
         unsigned char iEnable_Rx = 0;

         // _AddWait: Adds add wait calls in FTM_Sequencer when set to 1
         int _AddWait = 0;

         //Delay Profiling between each command
         int Delay_After_Set_Mode            = 50;
         int Delay_After_Set_Channel         = 50;
         int Delay_After_Tx_ON               = 50;
         int Delay_After_Set_Burst_Tx_Param  = 50;
         int Delay_After_Burst_Tx            = 50;
         int Delay_After_Burst_Rx            = 50;

         // Function return values
         unsigned char bOk = TRUE;

         // GSM Command status
         unsigned short iCLEAR_QUEUE_Status           = DIAG_FTM_STATUS_FAIL;
         unsigned short iADD_WAIT_Status              = DIAG_FTM_STATUS_FAIL;
         unsigned short iSet_Burst_Tx_Param_Status    = DIAG_FTM_STATUS_FAIL;
         unsigned short iBurst_Tx_Status_Enable       = DIAG_FTM_STATUS_FAIL;
         unsigned short iBurst_Tx_Status_Disable      = DIAG_FTM_STATUS_FAIL;
         unsigned short iBurst_Rx_Status_Enable       = DIAG_FTM_STATUS_FAIL;
         unsigned short iBurst_Rx_Status_Disable      = DIAG_FTM_STATUS_FAIL;

         // Flag to enable F3 messages
         unsigned char _bEnableF3Messages= false;

         //Parameters to save Embedded sequencer logs in EFS
         unsigned char filename[40] = "RF_VER_TDSCDMA";
         int Temp = 1;

         printf("\nExecuting TD-SCDMA FTM RF Verification Sequence on Band = %d : channel = %d", iBand, iChannel );

         // Start text log
         sLogFile = "C:\\ftm_rf_verification_test_TDSCDMA_Rx.txt";
         bOk &= QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\n\nLogging to file %s", sLogFile );

         if ( _bEnableF3Messages )
         {
            // Activate FTM F3 Messages
            bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, 23, 0xFF );
         }

         // Select embedded sequence mode
         //bOk &= QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );

         // Enter the batch mode, to queue up subsequent FTM commands
         //bOk &= QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );

         bOk &= QLIB_FTM_SET_MODE_ID( g_hResourceContext, iModeID );

         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, iBand);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Mode*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_CHAN(g_hResourceContext, iChannel);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Channel*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_TDSCDMA_SET_BURST_TX_PARAMS(g_hResourceContext,
                                                     iNumSlots,
                                                     iModulationType,
                                                     iPowerTS1,
                                                     iPowerTS2,
                                                     iPowerTS3,
                                                     iPowerTS4,
                                                     iPowerTS5,
                                                     &iSet_Burst_Tx_Param_Status);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_ON*ms, &iADD_WAIT_Status );

         //Enable Tx Burst
         iEnable_Tx = 1;
         bOk &= QLIB_FTM_TDSCDMA_BURST_TX(g_hResourceContext, iEnable_Tx, &iBurst_Tx_Status_Enable);

         //Dis-able Tx Burst
         iEnable_Tx = 0;
         bOk &= QLIB_FTM_TDSCDMA_BURST_TX(g_hResourceContext, iEnable_Tx, &iBurst_Tx_Status_Disable);

         //Enable Rx Burst
         iExpectedDLLevel = -500;
         iEnable_Rx = 1;
         bOk &= QLIB_FTM_TDSCDMA_BURST_RX(g_hResourceContext, iExpectedDLLevel, iEnable_Rx, &iBurst_Rx_Status_Enable);

         //Dis-able Rx Burst
         iEnable_Rx = 0;
         bOk &= QLIB_FTM_TDSCDMA_BURST_RX(g_hResourceContext, iExpectedDLLevel, iEnable_Rx, &iBurst_Rx_Status_Disable);

         ///////
         // Execute sequence
         ////////
         //bOk &= QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         /*
         Note:- The code controlled by #if condition below is not a part of regular test scenario.
         This piece of code is only enabled for debug purpose i.e. to store the sequence data on UE's EFS memory
         */
#if 0
         ///////////////////////////////////////////////////////////
         //Preparing Test case for QLIB_FTM_STORE_DEBUG_INFO_TO_FILE
         ///////////////////////////////////////////////////////////

         itoa(Temp, Count, 10);
         strcat((char *)filename, Count);
         printf("\nFilename = %s\n", filename);
         Temp++;

         QLIB_FTM_SEQ_STORE_DEBUG_INFO_TO_FILE( g_hResourceContext, filename, &iFTM_Error_Code);
         printf("QLIB_FTM_SEQ_STORE_DEBUG_INFO_TO_FILE --> Filename: %s : FTM_ERROR_CODE = %d\n", filename , iFTM_Error_Code);
#endif

         //QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iFTM_Error_Code );
         //printf("QLIB_FTM_SEQ_CLEAR_QUEUE\n");

         // Turn off sequencer
         //QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         ///////
         // Print results
         ////////

         // Function return values
         printf("\n\nFunction return status: %s\n", bOk  ? "PASS" : "FAIL" );
         // Response status For Conf UL Waveform
         printf("\n\t**********************************\n");
         printf("\t*****  Results for TD-SCDMA RF Verification  *****");
         printf("\n\t*****  Band = %d : Channel = %d  *****", iBand , iChannel);

         printf("\n\t**********************************\n\n");
         printf("\n\t\tSet Mode    - Done");
         printf("\n\t\tSet Channel - Done");
         printf("\n\t\tSet Burst Tx Params (NumSlot = %d : Modulation Type = %d : Power Ts1 = %d : Status = %s)",
                iNumSlots,
                iModulationType,
                iPowerTS1,
                iSet_Burst_Tx_Param_Status == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );

         printf("\n\t\tBurst_Tx - Enable (Status = %s)", iBurst_Tx_Status_Enable == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );

         printf("\n\t\tBurst_Tx - Disable (Status = %s)", iBurst_Tx_Status_Disable == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );

         printf("\n\t\tBurst_Rx - Enable (Status = %s)", iBurst_Rx_Status_Enable == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );

         printf("\n\t\tBurst_Rx - Disable (Status = %s)", iBurst_Rx_Status_Disable == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );

         printf("\n");
         printf("\n\t**********************************");

         QLIB_StopLogging( g_hResourceContext );
      } // end of TD-SCDMA RF Verification

      else if (key == '6')  // LTE TDD RF Verification
      {
         /*
         Equipment Setup
         LTE-TDD
         For Band: BC39
         Channel: 38300
         UL Freq. = 1885 MHz
         DL Freq. = 1885 MHz
         Select the appropriate LTE ARB file
         */
         unsigned short ms = 1000;

         // LTE Band and Channel information
         short iModeID = FTM_MODE_ID_LTE; /* LTE = 29 */
         short iBand = PHONE_MODE_LTE_B39; /* For LTE BC 39 */
         unsigned short iChannel = 38300;

         // Parametes for LTE TDD Set Params.
         char Sub_Frame_Config      =  1;
         char Enable_Continuous_Tx  =  0;

         // Parameters for LTE UL and DL BandWidth settings
         //      0 - RFCOM_BW_LTE_1P4MHz
         //         1 - RFCOM_BW_LTE_3MHz
         //         2 - RFCOM_BW_LTE_5MHz
         //         3 - RFCOM_BW_LTE_10MHz
         //         4 - RFCOM_BW_LTE_15MHz
         //         5 - RFCOM_BW_LTE_20MHz
         unsigned char itxChanBW[6] = {0,1,2,3,4,5};
         unsigned char irxChanBW[6] = {0,1,2,3,4,5};

         //Parameters for set Modulation Type
         //0 - QPSK, 1 - 16 QAM, 2 - 64 QAM
         unsigned long iModulationType  = 0;

         //Parameters to set Tx Waveform
         //0-CW, 1- PUSCH, 2- PUCCH, 3 - PRACH, 4 - SRS, 5 - UpPTS
         unsigned char iTxWaveform         = 1;
         unsigned char inumRBsPUSCH        = 50;
         unsigned char inumRBsPUCCH        = 0;
         unsigned char iPUSCHStartRBIndex  = 0;

         //
         const int TxPowerCount = 5;
         const int RxMeasCount  = 10;

         // Parameters for Set NS command
         unsigned char iNS = 1;

         //Parameters for Set Tx Power DBM
         unsigned char iEnable = TRUE;
         short iPowerLevel[5] = {230, 100, 0, -200, -400};

         //Parameters for Set LNA State
         short iExpected_RX_Power_dBm[5] = {-800, -600, -500, -400, -300};
         unsigned char iSelected_LNA_Range[RxMeasCount] = {9};

         //Parameters for Get Rx Power DBM
         short iRX_AGC[RxMeasCount] = {0};
         short iRX_Power_dBm[RxMeasCount] = {0};

         // _AddWait: Adds add wait calls in FTM_Sequencer when set to 1
         int _AddWait = 0;

         //Delay Profiling between each command
         //int Delay_After_Start = 200;
         int Delay_After_Set_Mode_ID         = 50;
         int Delay_After_Set_Mode            = 50;
         int Delay_After_Tx_BW               = 50;
         int Delay_After_Rx_BW               = 50;
         int Delay_After_Set_Channel         = 50;
         int Delay_After_Set_Modulation_Type = 50;
         int Delay_After_Tx_ON               = 50;
         int Delay_After_Set_Tx_Waveform     = 50;
         int Delay_After_Set_NS              = 50;
         int Delay_After_Set_Tx_Power        = 1000;
         int Delay_After_Set_LNA_State       = 50;
         int Delay_After_Get_Rx_Level        = 100;
         int Delay_After_Tx_OFF              = 50;
         int Delay_After_Set_TDDParams       = 50;
         // Function return values
         unsigned char bOk = TRUE;

         // LTE Command status
         unsigned short iCLEAR_QUEUE_Status                    = DIAG_FTM_STATUS_FAIL;
         unsigned short iADD_WAIT_Status                       = DIAG_FTM_STATUS_FAIL;
         unsigned short iSet_Net_Sig_Status                    = DIAG_FTM_STATUS_FAIL;
         unsigned short iSet_TX_Power_Status                   = DIAG_FTM_STATUS_FAIL;
         unsigned short iSet_LNA_State_DBM_Status[RxMeasCount] = {DIAG_FTM_STATUS_FAIL};
         unsigned short iGet_RX_Level_Status[RxMeasCount]      = {DIAG_FTM_STATUS_FAIL};
         unsigned short iSet_TDD_Params_Status                 = {DIAG_FTM_STATUS_FAIL};
         int i = 0;
         for(i=0; i<RxMeasCount; i++)
         {
            iGet_RX_Level_Status[i] = DIAG_FTM_STATUS_FAIL;
         }

         // Flag to enable F3 messages
         unsigned char _bEnableF3Messages= false;

         //Parameters to save Embedded sequencer logs in EFS
         unsigned char filename[40] = "RF_VER_LTE_";
         int Temp = 1;

         int TxLoop = 0;
         int RxLoop = 0;

         printf("\nExecuting LTE FTM RF Verification Sequence on Band = %d : channel = %d", iBand, iChannel );

         // Start text log
         sLogFile = "C:\\ftm_rf_verification_test_LTE.txt";
         bOk &= QLIB_StartLogging( g_hResourceContext,  sLogFile );
         printf("\n\nLogging to file %s", sLogFile );

         if ( _bEnableF3Messages )
         {
            // Activate FTM F3 Messages
            bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, 23, 0xFF );
         }

         // Select embedded sequence mode
         bOk &= QLIB_FTM_SEQ_SetSequencerMode( g_hResourceContext, QMSL_FTM_SEQUENCER_MODE_EMBEDDED );

         // Enter the batch mode, to queue up subsequent FTM commands
         bOk &= QLIB_FTM_SEQ_ActivateBatchMode( g_hResourceContext );

         bOk &= QLIB_FTM_SET_MODE_ID( g_hResourceContext, iModeID );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Mode_ID*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, iBand);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Mode*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_LTE_SET_TX_BANDWIDTH( g_hResourceContext, itxChanBW[3]);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_BW*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_LTE_SET_RX_BANDWIDTH( g_hResourceContext, irxChanBW[3]);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Rx_BW*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_LTE_SET_TDD_PARAMS(g_hResourceContext, Sub_Frame_Config, Enable_Continuous_Tx, &iSet_TDD_Params_Status);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_TDDParams*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_CHAN(g_hResourceContext, iChannel);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Channel*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_LTE_SET_TX_MODULATION_TYPE(g_hResourceContext, iModulationType);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Modulation_Type*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_LTE_SET_TX_WAVEFORM(g_hResourceContext, iTxWaveform, inumRBsPUSCH, inumRBsPUCCH, iPUSCHStartRBIndex );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Tx_Waveform*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_TX_ON( g_hResourceContext );
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_ON*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_LTE_SET_NET_SIG_VALUE(g_hResourceContext, iNS, &iSet_Net_Sig_Status);
         if(_AddWait)   bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_NS*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_TX_POWER_DBM(g_hResourceContext , iEnable, iPowerLevel[0], &iSet_TX_Power_Status);
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Tx_Power*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_TX_POWER_DBM(g_hResourceContext , iEnable, iPowerLevel[1], &iSet_TX_Power_Status);
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Tx_Power*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_TX_POWER_DBM(g_hResourceContext , iEnable, iPowerLevel[2], &iSet_TX_Power_Status);
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Tx_Power*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_TX_POWER_DBM(g_hResourceContext , iEnable, iPowerLevel[3], &iSet_TX_Power_Status);
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Tx_Power*ms, &iADD_WAIT_Status );

         bOk &= QLIB_FTM_SET_TX_POWER_DBM(g_hResourceContext , iEnable, iPowerLevel[4], &iSet_TX_Power_Status);
         QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Tx_Power*ms, &iADD_WAIT_Status );

         //if(_AddWait) bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_Tx_Power*ms, &iADD_WAIT_Status );

         //if(Enable_Continuous_Tx)
         {
            bOk &= QLIB_FTM_SET_TX_OFF( g_hResourceContext );
            if(_AddWait)    bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_OFF*ms, &iADD_WAIT_Status );
         }
         for(RxLoop = 0; RxLoop < RxMeasCount; RxLoop++)
         {
            bOk &= QLIB_FTM_LTE_SET_LNA_STATE_DBM (g_hResourceContext , iExpected_RX_Power_dBm[1], &iSelected_LNA_Range[RxLoop], &iSet_LNA_State_DBM_Status[RxLoop]);
            if(_AddWait)    bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Set_LNA_State*ms, &iADD_WAIT_Status );

            bOk &= QLIB_FTM_GET_RX_LEVEL_DBM(g_hResourceContext , &iRX_AGC[RxLoop], &iRX_Power_dBm[RxLoop], &iGet_RX_Level_Status[RxLoop]);
            if(_AddWait)    bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Get_Rx_Level*ms, &iADD_WAIT_Status );
         }

         if(!Enable_Continuous_Tx)
         {
            //bOk &= QLIB_FTM_SET_TX_OFF( g_hResourceContext );
            if(_AddWait)    bOk &= QLIB_FTM_SEQ_ADD_WAIT( g_hResourceContext, Delay_After_Tx_OFF*ms, &iADD_WAIT_Status );
         }

         ///////
         // Execute sequence
         ////////
         bOk &= QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hResourceContext, &iFTM_Error_Code, &iTimeOutOcurred );

         /*
         Note:- The code controlled by #if condition below is not a part of regular test scenario.
         This piece of code is only enabled for debug purpose i.e. to store the sequence data on UE's EFS memory
         */
#if 0

         ///////////////////////////////////////////////////////////
         //Preparing Test case for QLIB_FTM_STORE_DEBUG_INFO_TO_FILE
         ///////////////////////////////////////////////////////////

         itoa(Temp, Count, 10);
         strcat((char *)filename, Count);
         printf("\nFilename = %s\n", filename);
         Temp++;

         QLIB_FTM_SEQ_STORE_DEBUG_INFO_TO_FILE( g_hResourceContext, filename, &iFTM_Error_Code);
         printf("QLIB_FTM_SEQ_STORE_DEBUG_INFO_TO_FILE --> Filename: %s : FTM_ERROR_CODE = %d\n", filename , iFTM_Error_Code);

#endif

         QLIB_FTM_SEQ_CLEAR_QUEUE( g_hResourceContext, &iFTM_Error_Code );
         printf("QLIB_FTM_SEQ_CLEAR_QUEUE\n");

         // Turn off sequencer
         QLIB_FTM_SEQ_DeactivateBatchMode( g_hResourceContext );

         ///////
         // Print results
         ////////

         // Function return values
         printf("\n\nFunction return status: %s\n", bOk  ? "PASS" : "FAIL" );
         // Response status For Conf UL Waveform
         printf("\n\t**********************************\n");
         printf("\t***** Results for LTE RF Verification *****");
         printf("\n\t*****      Band = %d : Channel = %d : Modulation Type = %d    *****", iBand , iChannel, iModulationType);

         printf("\n\t**********************************\n\n");
         // Response status For  Set Tx Power
         printf("\n\t[%d] --> TX Power = %d\tStatus: %s\n", 1
                , iPowerLevel[0]
                , iSet_TX_Power_Status == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL" );

         for(RxLoop = 0; RxLoop < RxMeasCount; RxLoop++)
         {
            // Response status For Get Rx Level
            printf("\n\t\t[%d] --> LNA State = %d\tRxAGC = %d\tRx_Power_dBm = %d\tStatus: %s", RxLoop+1,
                   iSelected_LNA_Range[RxLoop],
                   (iRX_AGC[RxLoop]/64),
                   (iRX_Power_dBm[RxLoop] - 65535),
                   ((iGet_RX_Level_Status[RxLoop] || iSet_LNA_State_DBM_Status[RxLoop]) == DIAG_FTM_STATUS_SUCCESS? "PASS" : "FAIL") );
         }

         printf("\n");
         printf("\n\t**********************************");

         QLIB_StopLogging( g_hResourceContext );
      } // end of LTE TDD RF Verification

      else if (key = '7' )
      {
         unsigned char bOk = true;
         FTM_RF_Mode_Enum iRfMode;
         unsigned int iNumDevices = 0, iRfmDeviceId = 0, iLinkType = 0, iChainType = 0, iNumCells = 0, iFtmErrorCode = 0, iScenario = 0;;
         unsigned long iPCellChannel = 0, iScellChannel = 0;
         FTM_RFFE_Device_type iRffeDeviceType = RFFE_DEVICE_INVALID;
         FTM_RFFE_Device_Setting_Request_type  sDeviceSettingRequest;
         FTM_RFFE_Device_Setting_Response_type sDeviceTransactions[RFFE_MAX_DEVICE_INSTANCES];

         sLogFile = "C:\\Temp\\ftm_rffe_get_device_setting.txt";
         bOk &= QLIB_StartLogging( g_hResourceContext, sLogFile );

         printf("\n/************ FTM_RFFE_GET_DEVICE_SETTING() *******************/ ");

         iRfMode = PHONE_MODE_LTE_B1;
         iPCellChannel = 0;
         iScellChannel = 0;
         iRfmDeviceId = 0;
         iLinkType = 1;
         iChainType = 0;
         iNumCells = 1;
         iScenario = 0;
         iRffeDeviceType = RFFE_DEVICE_TUNER;
         printf("\nConstructing the request packet with the following params");
         printf("\nTechnology = LTE," \
                "\nBand = LTE B1,"
                "\nRF Mode = %d,"
                "\nPCellChannel = %d,"
                "\nSCellChannel = %d,"
                "\nLink Type = %d,"
                "\nRFM Device = %d,"
                "\nChain Type = %d,"
                "\nNum Cells = %d,"
                "\nScenario = %d,"
                "\nRFFE Device Type = %d",
                iRfMode, iPCellChannel, iScellChannel, iLinkType, iRfmDeviceId, iChainType, iNumCells, iScenario, iRffeDeviceType );

         sDeviceSettingRequest.iRfMode = (unsigned char) iRfMode;
         sDeviceSettingRequest.iChanList.pCellChan = (unsigned short) iPCellChannel;
         sDeviceSettingRequest.iChanList.sCellChan = (unsigned short) iScellChannel;
         sDeviceSettingRequest.iChainType = (unsigned char) iChainType;
         sDeviceSettingRequest.iLinkType = (unsigned char) iLinkType;
         sDeviceSettingRequest.iNumCells = (unsigned char) iNumCells;
         sDeviceSettingRequest.iRfmDeviceId = (unsigned char) iRfmDeviceId;
         sDeviceSettingRequest.iRffeDeviceType = (unsigned char) iRffeDeviceType;
         sDeviceSettingRequest.iScenario = (unsigned char) iScenario;

         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, (short) iRfMode );
         bOk &= QLIB_FTM_LTE_SET_TX_BANDWIDTH( g_hResourceContext,(unsigned char) 3);
         bOk &= QLIB_FTM_LTE_SET_RX_BANDWIDTH( g_hResourceContext, (unsigned char) 3);
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, 18000 );

         bOk &= QLIB_FTM_RFFE_GET_DEVICE_SETTING( g_hResourceContext,
                                                  (void*) &sDeviceSettingRequest,
                                                  (unsigned char*) &iNumDevices,
                                                  (void*) &sDeviceTransactions,
                                                  (unsigned short*) &iFtmErrorCode );

         if (bOk)
         {
            if(iFtmErrorCode == 0 )
            {
               printf("\n\nResponse packet received. Decoding it...\n\n");
               printf("\nNumber of Devices of type \"%d\"  = %d\n", iRffeDeviceType, iNumDevices );

               for(unsigned int i = 0; i < iNumDevices; ++i )
               {
                  printf("\nDevice %d", i);
                  printf("\n----------------------");
                  printf("\nBus Id =               %d", sDeviceTransactions[i].bus_id);
                  printf("\nSlave Id =             %d", sDeviceTransactions[i].slave_id);
                  printf("\nManufacturer Id =      %d", sDeviceTransactions[i].mfg_id);
                  printf("\nProduct Id =           %d", sDeviceTransactions[i].prd_id);
                  printf("\nProduct Rev =          %d", sDeviceTransactions[i].rev_id);
                  printf("\nNum of transactions =  %d", sDeviceTransactions[i].num_transactions);
                  printf("\n  Address\tData");
                  for(unsigned int j = 0; j < sDeviceTransactions[i].num_transactions; ++j )
                  {
                     printf("\n  0x%x\t\t0x%x", sDeviceTransactions[i].device_transactions[j].address, sDeviceTransactions[i].device_transactions[j].data);
                  }
                  printf("\n");
               }
            }
            else
            {
               printf("\nQLIB_FTM_RFFE_GET_DEVICE_SETTING() returned an error code of %d", iFtmErrorCode );
            }
         }
         else
         {
            printf("\nQLIB_FTM_RFFE_GET_DEVICE_SETTING() execution failed");
            return false;
         }

         QLIB_StopLogging( g_hResourceContext );
      }

      printf("\nPress a key to continue\n");
      while(!kbhit());
   }
   return true;
}

unsigned char Test_LTE_NS( )
{
   unsigned char bOk = 1;
   unsigned char iBand = 12; /*LTE B13*/
   unsigned short iEARFCN = 5230; /*DL Channel number*/
   unsigned long Status;
   unsigned short iCRNTI;
   unsigned char iDLLCID;
   unsigned char iULLCID;
   unsigned char iNetworkSigValue = 1; /*Base station signaled value. In the case of NS we force the mobile to a value of choice. */
   char iBSMaxPowerLimit = 30;
   LTE_PDSCH_BLER_Status m_LTE_PDSCH_BLER_Status;
   LTE_PDSCH_BLER_Status *pLTE_PDSCH_BLER_Status = &m_LTE_PDSCH_BLER_Status;
   unsigned long iMeasurementCount = 100; /*Subframes*/
   unsigned long iTimeOut_ms = 6000;
   unsigned long iTotalBlockReceived, iTotalBlockError, iThroughput;
   char cKey = ' ';

   printf("-------------------\nLTE Non-signaling\n-------------------\n");

   printf("\nWhich ARB?");
   printf("\n1=Anritsu MT8820C");
   printf("\n2=R&S CMW500");
   printf("\n\n");

   cKey = getch();

   if (cKey == '1' )
   {
      /*The DL and UL LCID and CRNTI provided here are for the 8820C. These values are hardcoded in the ARB Waveform.*/
      iCRNTI = 43690;
      iDLLCID = 3;
      iULLCID = 2;
   }
   else if (cKey == '2' )
   {
      /*The DL and UL LCID and CRNTI provided here are for the CMW500. These values are hardcoded in the ARB Waveform.*/
      iCRNTI = 14;
      iDLLCID = 1;
      iULLCID = 2;
   }
   else
   {
      printf("Invalid ARB selected!\n");
      return 0;
   }

   printf("Enter LTE band-> ");
   scanf("%d", &iBand);
   iBand = iBand - 1; // QLIB function maps Band1 to 0...Band13 to 12, etc.

   printf("Enter DL channel number-> ");
   scanf("%d", &iEARFCN);
   printf("\n");

   // Start LTE NS mode
   bOk &= QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_LTE_NS);
   bOk &= QLIB_FTM_LTE_NS_START(g_hResourceContext, &Status);
   if(Status == 0 || Status == 13)
   {
      printf("QLIB_FTM_LTE_NS_START - Successful with status = %d\n", Status);
   }
   else
   {
      printf("QLIB_FTM_LTE_NS_START - Failed with status = %d\n", Status);
      return bOk;
   }

   // Acquire LTE NS downlink signal
   bOk &= QLIB_FTM_LTE_NS_ACQ(g_hResourceContext, iBand, iEARFCN, &Status );
   if(Status == 0 || Status == 13)
   {
      printf("QLIB_FTM_LTE_NS_ACQ(iBand=%d, iEARFCN=%d) - Successful with status = %d\n", iBand, iEARFCN, Status);
   }
   else
   {
      printf("QLIB_FTM_LTE_NS_ACQ(iBand=%d, iEARFCN=%d) - Failed with status = %d\n", iBand, iEARFCN, Status);
      return bOk;
   }

   // Establish the LTE uplink and downlink data paths
   bOk &= QLIB_FTM_LTE_NS_START_DP( g_hResourceContext, iCRNTI, iNetworkSigValue, iBSMaxPowerLimit, iDLLCID, iULLCID, &Status );
   bOk &= QLIB_FTM_LTE_NS_START_DP( g_hResourceContext, iCRNTI, iNetworkSigValue, iBSMaxPowerLimit, iDLLCID, iULLCID, &Status );
   if(Status == 0 || Status == 13)
   {
      printf("QLIB_FTM_LTE_NS_START_DP - Successful with status = %d\n", Status);
   }
   else
   {
      printf("QLIB_FTM_LTE_NS_START_DP - Failed with status = %d\n", Status);
      return bOk;
   }

   // Check if call is up
   bOk &= QLIB_FTM_LTE_NS_IS_CONNECTED(g_hResourceContext, &Status);
   if(Status == 0 || Status == 13)
   {
      printf("QLIB_FTM_LTE_NS_IS_CONNECTED - Successful with status = %d\n", Status);
   }
   else
   {
      printf("QLIB_FTM_LTE_NS_IS_CONNECTED - Failed with status = %d\n", Status);
      return bOk;
   }

   // Measure Throughput/BLER - Asynchronous method
   bOk &= QLIB_LTE_PDSCH_BLER_Start_Log(g_hResourceContext);
   bOk &= QLIB_LTE_PDSCH_BLER_DoMeasurement(g_hResourceContext,pLTE_PDSCH_BLER_Status,iMeasurementCount,iTimeOut_ms);
   bOk &= QLIB_LTE_PDSCH_BLER_Stop_Log(g_hResourceContext);

   if ( bOk )
   {
      printf("\nQLIB_LTE_PDSCH_BLER_DoMeasurement - Successful.\n");
      printf("\tThroughput = %.2f kbps\n", pLTE_PDSCH_BLER_Status->tPut);
      printf("\tBLER = %.2f pcnt\n", pLTE_PDSCH_BLER_Status->tPut_pctFromPeak);
   }
   else
   {
      printf("\nLTE BLER and throughput measurements failed!\n");
      //      QLIB_FTM_LTE_NS_STOP( g_hResourceContext, &Status );
      return bOk;
   }

   // Measure Throughput/BLER - Synchronous method
   bOk &= QLIB_FTM_LTE_NS_RESET_DL_BLER_REPORT( g_hResourceContext, &Status );
   if ( bOk && Status == 0 )
   {
      printf("\nQLIB_FTM_LTE_NS_RESET_DL_BLER_REPORT - Successful\n");
   }
   else
   {
      printf("\nQLIB_FTM_LTE_NS_RESET_DL_BLER_REPORT - Failed with status = %d\n", Status);
      //      QLIB_FTM_LTE_NS_STOP( g_hResourceContext, &Status );
      return bOk;
   }

   unsigned long iStartTime = GetTickCount();
   unsigned long iEndTime;
   unsigned long iElapsedTime_ms = 0;

   while ( bOk && Status == 0 )
   {
      bOk &= QLIB_FTM_LTE_NS_GET_DL_BLER_REPORT( g_hResourceContext, &iTotalBlockReceived, &iTotalBlockError, &iThroughput, &Status );

      if ( bOk && Status == 0 )
      {
         if ( iTotalBlockReceived >= iMeasurementCount )
         {
            printf("QLIB_FTM_LTE_NS_GET_DL_BLER_REPORT - Successful\n");
            printf("\tTotal Blocks Received = %d\n", iTotalBlockReceived);
            printf("\tTotal Block Errors = %d\n", iTotalBlockError);
            printf("\tThroughput = %d bps\n", iThroughput);
            break;
         }
      }
      else
      {
         printf("QLIB_FTM_LTE_NS_GET_DL_BLER_REPORT failed - Failed with status = %d\n", Status);
         //       QLIB_FTM_LTE_NS_STOP( g_hResourceContext, &Status );
         return bOk;
      }

      iEndTime = GetTickCount();
      iElapsedTime_ms = iEndTime - iStartTime;

      if ( iElapsedTime_ms < iTimeOut_ms )
         Sleep(10);
      else
      {
         printf("\n\t%d ms timeout exceeded waiting for QLIB_FTM_LTE_NS_GET_DL_BLER_REPORT to measure %d subframes", iTimeOut_ms, iMeasurementCount);
         //        QLIB_FTM_LTE_NS_STOP( g_hResourceContext, &Status );
         break;
      }
   }
   /*
   printf("\nStop LTE NS mode? (Y or N)\n");
   cKey = toupper(getch());

   if ( cKey == 'Y' )
   {
   // Stop LTE NS mode
   bOk &= QLIB_FTM_LTE_NS_STOP( g_hResourceContext, &Status );
   if(Status == 0 || Status == 13)
   {
   printf("QLIB_FTM_LTE_NS_STOP - Successful with status = %d\n", Status);
   }
   else
   {
   printf("QLIB_FTM_LTE_NS_STOP - Failed with status = %d\n", Status);
   }
   }
   */
   return bOk;
}

unsigned char Test_GSM_BER( void )
{
   unsigned char bOk = true;
   char cKey = ' ';
   unsigned char _iBand = 0;
   unsigned short _iChannel1 = 0;
   unsigned short _iChannel2 = 0;
   unsigned short _iChannel = 0;
   unsigned char  _iStatus = 0;

   unsigned char _bValidKeyHit = 0;
   unsigned char _bNewMethod = 0;

   printf("----------------\nGSM FTM BER Test\n----------------\n");

   printf("\n");
   printf("\n1=GSM900 Test (automated library commands)" );
   printf("\n2=GSM1800 Test (automated library commands)" );
   printf("\n3=GSM1900 Test (automated library commands)" );
   printf("\n4=GSM Start/Stop stress test" );
   cKey = getch();

   if (cKey == '1' )
   {
      printf("\nGSM900 FTM BER Test");
      _iBand = FTM_GSM_BER_EGSM_900;
      _iChannel1 = 32;
      _iChannel2 = 124;

      _bValidKeyHit = true;
   }    // cKey = '1'
   else if (cKey == '2' )
   {
      printf("\nGSM1800 FTM BER Test");
      _iBand = FTM_GSM_BER_DCS_1800;
      _iChannel1 = 512;
      _bValidKeyHit = true;
   }    // cKey = '2'
   else if (cKey == '3' )
   {
      printf("\nGSM1900 FTM BER Test");
      _iBand = FTM_GSM_BER_PCS_1900;
      _iChannel1 = 600;
      _bValidKeyHit = true;
   }    // cKey = '2'
   else if (cKey == '4' )
   {
      printf("\nGSM START/STOP Stress Test");
      _iBand = FTM_GSM_BER_EGSM_900;
      _iChannel1 = 32;
      _iChannel2 = 124;

      _bValidKeyHit = true;
   }    // cKey = '1'

   // If valid key hit...
   if (_bValidKeyHit )
   {
      printf("\n\nCMU-200 Setup:");
      printf("\n  - Select GSM signaling mode for the correct band");
      printf("\n  - BS Signal -> Mode = BCCH and TCCH");
      printf("\n  - BS Signal -> BCCH and TCH = %d", _iChannel1 );
      printf("\n  - BS Signal->Timeslot = 3");
      printf("\n  - Network->Circuit Switched->Signaling Modes->Signaling Channel = None");
      printf("\n  - Measurement Mode->  C=Burst by Burst");
      printf("\n\n  <<<Press any key to continue>>>");
   }

   if ( (cKey == '1' )
         ||    (cKey == '2' )
         ||    (cKey == '3' )
      )
   {
      // printf("\n4=GSM900 Test (automated library commands)" );

      // Get the user key
      getch();

      /////////////////////////////////////////
      // Request call start, with session init
      /////////////////////////////////////////
      bOk = QLIB_FTM_GSM_BER_StartSession(
               g_hResourceContext,
               FTM_NONSIG_DEFAULT_TIMEOUT );

      printf("\n  QLIB_FTM_GSM_BER_StartSession: %s", bOk ? "Pass" : "FAIL" );

      /////////////////////////////////////////
      // Assign BCCH
      /////////////////////////////////////////
      bOk = QLIB_FTM_GSM_BER_AssignBCCH(
               g_hResourceContext,
               _iChannel1,
               _iBand
            );

      printf("\n  QLIB_FTM_GSM_BER_AssignBCCH: %s", bOk ? "Pass" : "FAIL" );

      /////////////////////////////////////////
      // Set SACCH accumulator maximum--this is optional, default value is 40
      /////////////////////////////////////////
      if ( bOk )
      {
         bOk = QLIB_FTM_GSM_BER_SetSACCH_AccumulatorMax( g_hResourceContext, 4 );

         printf("\n  QLIB_FTM_GSM_BER_SetSACCH_AccumulatorMax: %s", bOk ? "Pass" : "FAIL" );
      } // QLIB_FTM_GSM_BER_StartCall

      /////////////////////////////////////////
      // Assign 1st traffic
      /////////////////////////////////////////
      if ( bOk )
      {
         // Now connect
         printf("\n\nSetup call box to \"connect,\" then <<press any key to continue>>...");
         getch();

         bOk = QLIB_FTM_GSM_BER_AssignTraffic(
                  g_hResourceContext,
                  FTM_GSM_BER_Loopback_Type_C,        // Loopback type
                  FTM_GSM_BER_SLOT_3,                 // Slot
                  FTM_GSM_BER_SPEECH_FULL_RATE        // Channel type
               );

         printf("\n  QLIB_FTM_GSM_BER_AssignTraffic: %s", bOk ? "Pass" : "FAIL" );
      } // QLIB_FTM_GSM_BER_StartCall

      /////////////////////////////////////////
      // Get GSM Rx Level
      /////////////////////////////////////////
      // if ( bOk )

      // Some 6275 versions have a failed status on the GSM TCH assignment, the function
      // QLIB_FTM_GSM_BER_IsTrafficValid() shoudl be used to check status of the traffic channel
      // instead of relying upon the return value from QLIB_FTM_GSM_BER_AssignTraffic()
      {
         char _cKey2 = ' ';
         bool _bExit = true;

         do
         {
            _bExit = true;

            // Request call start
            printf("\n\nAdjust Rx level, then press <<R>> for Rx Report report,\n<<L>> for loopback C enable\n<<D>> for loopback disable <<any other key to continue>>");
            _cKey2 = toupper( getch() );

            if ( _cKey2 == 'R')
            {
               //
               // Get the rx level
               //
               unsigned char _iRxLevel;
               unsigned char _iRxQual;

               bOk = QLIB_FTM_GSM_BER_GetRxMetrics( g_hResourceContext, &_iRxLevel,&_iRxQual );

               printf("\n  QLIB_FTM_GSM_BER_GetRxMetrics: %s, Level: %d, Qual: %d", bOk ? "Pass" : "FAIL", _iRxLevel, _iRxQual  );

               //
               // Get the traffic state
               //
               bool _bTrafficValid = QLIB_FTM_GSM_BER_IsTrafficValid( g_hResourceContext ) == TRUE;

               printf("\n  QLIB_FTM_GSM_BER_IsTrafficValid: %s", _bTrafficValid ? "Pass" : "FAIL" );

               //
               // Get the entire status structure and print the SACCH accumulator stats
               //

               // Get the values from the status
               GSM_BER_State _GSM_BER_State;

               // Update the local status copy
               QLIB_FTM_GSM_BER_GetStatus( g_hResourceContext, (unsigned char*) &_GSM_BER_State);

               printf("\n  iGSM_BER_SACCH_Accumulator_Value: %d", _GSM_BER_State.iGSM_BER_SACCH_Accumulator_Value );
               printf("\n  iGSM_BER_SACCH_Accumulator_TotalUpdates: %d", _GSM_BER_State.iGSM_BER_SACCH_Accumulator_TotalUpdates );

               _bExit = false;
            }

            if ( _cKey2 == 'L')
            {
               // Set loopback C
               bOk  = QLIB_FTM_GSM_ConfigLoopbackType( g_hResourceContext, FTM_GSM_BER_Loopback_Type_C);

               printf("\n  QLIB_FTM_GSM_ConfigLoopbackType(LOOPC): %s", bOk  ? "Pass" : "FAIL" );

               _bExit = false;
            }
            if ( _cKey2 == 'D')
            {
               // Set loopback C
               bOk  = QLIB_FTM_GSM_ConfigLoopbackType( g_hResourceContext, FTM_GSM_BER_Disable_Loopback );

               printf("\n  QLIB_FTM_GSM_ConfigLoopbackType(DISABLE): %s", bOk  ? "Pass" : "FAIL" );

               _bExit = false;
            }
         }
         while ( !_bExit );
      } // QLIB_FTM_GSM_BER_ReleaseTCH

      /////////////////////////////////////////
      // Now handover to 2nd channel
      /////////////////////////////////////////
      if ( bOk )
      {
         // gsm1800
         _iBand = FTM_GSM_BER_DCS_1800;
         _iChannel2 = 512;

         printf("\n Change channel to GSM1800-%d, then <<press any key to continue>> for handover", _iChannel2 );
         getch();

         // Set to false for old builds of AMSS, which do not restore the data loopback automatically
         unsigned char _bUseOptimizedFunction = true;

         if ( _bUseOptimizedFunction )
         {
            QLIB_FTM_GSM_BER_Handover_NoLoopbackConfig(
               g_hResourceContext,
               _iChannel2,
               _iBand,
               0,   // _iTSC,
               FTM_GSM_BER_SLOT_3,
               FTM_GSM_BER_SPEECH_FULL_RATE,
               5
            );
         }
         else
         {
            QLIB_FTM_GSM_BER_Handover(
               g_hResourceContext,
               _iChannel2,
               _iBand,
               0,   // _iTSC,
               FTM_GSM_BER_SLOT_3,
               FTM_GSM_BER_SPEECH_FULL_RATE,
               5,   // iPowerLevel
               FTM_GSM_BER_Loopback_Type_C
            );
         }
         printf("\n  QLIB_FTM_GSM_BER_CHANNEL_ASSIGN_V2_REQ: %s", bOk ? "Pass" : "FAIL" );

         /*
         CMU-200:
         Receiver Quality
         -> Test Setup -> Observe the active test setup

         Location 1:
         -> BER -> Setup menu
         BER -> Test # -> Circuit Switched -> Meas. Mode (this is the loop type):

         Location 2:
         -> Meas.Mode button at bottom of screen, next to Test Setup
         Loop A = RBER / FER
         Loop B = BER
         Loop C = Burst by Burst
         */
      } // QLIB_FTM_GSM_BER_AssignBCCH

      /////////////////////////////////////////
      // Now handover to 2nd channel
      /////////////////////////////////////////
      if ( bOk )
      {
         // gsm1900
         _iBand = FTM_GSM_BER_PCS_1900;
         _iChannel2 = 512;

         printf("\n Change channel to GSM1900-%d, then <<press any key to continue>> for handover", _iChannel2 );
         getch();

         QLIB_FTM_GSM_BER_Handover(
            g_hResourceContext,
            _iChannel2,
            _iBand,
            0,  // _iTSC,
            FTM_GSM_BER_SLOT_3,
            FTM_GSM_BER_SPEECH_FULL_RATE,
            5,  // iPowerLevel
            FTM_GSM_BER_Loopback_Type_C
         );

         printf("\n  QLIB_FTM_GSM_BER_CHANNEL_ASSIGN_V2_REQ: %s", bOk ? "Pass" : "FAIL" );

         /*
         CMU-200:
         Receiver Quality
         -> Test Setup -> Observe the active test setup

         Location 1:
         -> BER -> Setup menu
         BER -> Test # -> Circuit Switched -> Meas. Mode (this is the loop type):

         Location 2:
         -> Meas.Mode button at bottom of screen, next to Test Setup
         Loop A = RBER / FER
         Loop B = BER
         Loop C = Burst by Burst
         */
      } // QLIB_FTM_GSM_BER_AssignBCCH

      /////////////////////////////////////////
      // Release 2nd traffic
      /////////////////////////////////////////
      if ( bOk )
      {
         // Request call start
         printf("\n\nPerform testing, then disconnect call and turn off BS power then, <<press any key to stop call >>");
         getch();

         QLIB_FTM_GSM_BER_ReleaseTCH( g_hResourceContext );

         printf("\n QLIB_FTM_GSM_BER_ReleaseTCH: %s", bOk ? "Pass" : "FAIL" );
      } // QLIB_FTM_GSM_BER_ReleaseTCH

      /////////////////////////////////////////
      // Close Session
      /////////////////////////////////////////
      // if ( bOk ) -- do this regardless of the other status
      {
         QLIB_FTM_GSM_BER_StopSession(g_hResourceContext );

         printf("\n QLIB_FTM_GSM_BER_StopCall: %s", bOk ? "Pass" : "FAIL" );
      } // QLIB_FTM_GSM_BER_StopSession
   } // cKey == '1'...'3'
   else if (cKey == '4' )
   {
      unsigned short _iCount = 0;
      char _sTextLogFile[] = "c:\\QMSL_GSM_START_STOP_Stress_Test.txt";
      QLIB_StartLogging( g_hResourceContext, _sTextLogFile);
      printf("\nText log: %s", _sTextLogFile );

      char _sDLF_LogFile[] = "c:\\QMSL_GSM_START_STOP_Stress_Test.dlf";
      QLIB_StartDLF_Logging( g_hResourceContext,  _sDLF_LogFile, false);
      printf("\nDLF log: %s", _sDLF_LogFile );

      // Setup to log all FTM messages
      bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, (short) 23, 0xFF );
      bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, (short) 4001, 0xFF );

      // Enable all phone logging
      bOk &= QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );

      // Clear drop count
      bOk &= QLIB_DIAGDIAG_RESET_DROP_COUNT(
                g_hResourceContext,
                DIAGDIAG_GET_DROP_COUNT_LOG_Q6
             );
      printf("\nbOK after setup: %s", bOk ? "PASS" : "FAIL" );
      /*
      for ( _iCount=0; _iCount < 1000; _iCount++)
      {
      printf("\n Loop count: %d", _iCount );

      /////////////////////////////////////////
      // Request call start, with session init
      /////////////////////////////////////////
      bOk = QLIB_FTM_GSM_BER_StartSession(
      g_hResourceContext,
      1000 );
      if (!bOk ) Sleep( 10000);

      printf("\n  QLIB_FTM_GSM_BER_StartSession: %s", bOk ? "Pass" : "FAIL" );

      // GSM Start/Stop stress test
      QLIB_FTM_GSM_BER_StopSession(g_hResourceContext );

      // Stop on key press
      if (kbhit()) break;

      // Stop on error
      if (!bOk) break;

      Test_NV_FTM_Mode(1);
      } // for ( _iCount=0; _iCount < 10; _iCount++)
      */
      // Get drop count
      unsigned long iDropCount = -1;
      bOk &= QLIB_DIAGDIAG_GET_DROP_COUNT(
                g_hResourceContext,
                DIAGDIAG_GET_DROP_COUNT_LOG_Q6,
                &iDropCount
             );
      printf("\nQLIB_DIAGDIAG_GET_DROP_COUNT:iDropCount = %d", iDropCount);

      printf("Press a key to continue...");
      if (kbhit()) getch();
      QLIB_StopLogging( g_hResourceContext );
      QLIB_StopDLF_Logging(g_hResourceContext );
   }    // else if (    (cKey == '4' )

   return bOk;
}

unsigned char Test_EGPRS_BER( void )
{
   unsigned char bOk = true;
   char cKey = ' ';
   unsigned char _iBand = 0;
   unsigned short _iChannel_BCCH = 0;
   unsigned short _iChannel_TCH = 0;
   unsigned short _iChannel = 0;
   unsigned char  _iStatus = 0;

   unsigned char _bValidKeyHit = 0;
   unsigned char _bNewMethod = 0;

   printf("----------------\nEGPRS FTM BER Test (!!! Still under development!!!!) \n----------------\n");

   printf("\n");
   printf("\n1=GSM900 EGPRS Test (automated library commands)" );
   printf("\n2=GSM1800 EGPRS Test (automated library commands)" );
   printf("\n3=GSM1900 EGPRS Test (automated library commands)" );
   printf("\n7 - EGPRS Non-signaling call (GSM900, BCCH 32, TCH 62, Gamma 13, 2UL(4,5)/2DL(4,5)  with event status");
   printf("\n8 - Change power, Ask user for Gamma, 2UL(4,5)/2DL(4,5)");
   printf("\n9 - EDGE TBF DL/UL TBF V2");
   printf("\nA - Release TBF");
   printf("\nB - Change Channel(TCH63), Ask user for Gamma, 2UL(4,5)/2DL(4,5)");

   cKey = getch();

   if (cKey == '1' )
   {
      printf("\nGSM900 EGPRS FTM BER Test");
      _iBand = FTM_GSM_BER_EGSM_900;
      _iChannel_BCCH = 32;
      _iChannel_TCH = 62;

      _bValidKeyHit = true;
   }    // cKey = '1'
   else if (cKey == '2' )
   {
      printf("\nGSM1800 EGPRS FTM BER Test");
      _iBand = FTM_GSM_BER_DCS_1800;
      _iChannel_BCCH = 512;
      _iChannel_TCH = 513;
      _bValidKeyHit = true;
   }    // cKey = '2'
   else if (cKey == '3' )
   {
      printf("\nGSM1900 EGPRS FTM BER Test");
      _iBand = FTM_GSM_BER_PCS_1900;
      _iChannel_BCCH = 600;
      _iChannel_TCH = 601;
      _bValidKeyHit = true;
   }    // cKey = '2'

   else if (cKey == '7')
   {
      unsigned short iStatus = 0;
      unsigned long _iCallSetupTime = 0;

      QLIB_StartLogging( g_hResourceContext,  "C:\\edge_non_signaling_start.txt");

      _iCallSetupTime = ::GetTickCount();

      // Clear GSM status
      QLIB_FTM_GSM_BER_EVENT_STATUS_CLEAR_REQ( g_hResourceContext, &iStatus);
      printf("QLIB_FTM_GSM_BER_EVENT_STATUS_CLEAR_REQ: status=%d\n", iStatus);

      //  START GSM mode
      QLIB_FTM_GSM_START_MODE_REQ( g_hResourceContext, &iStatus);
      printf("QLIB_FTM_GSM_START_MODE_REQ: iStatus=%d\n", iStatus);

      // Check for GSM mode status
      QLIB_FTM_GSM_BER_EVENT_STATUS_REQ(g_hResourceContext, FTM_GSM_BER_START_GSM_MODE_SYNC_STATUS, 1000, &iStatus);
      printf("QLIB_FTM_GSM_BER_EVENT_STATUS_REQ(FTM_GSM_BER_START_GSM_MODE_SYNC_STATUS, 1000) Status=%d\n", iStatus);

      // BCCH acquire
      unsigned short iChannel = 32;
      unsigned char iBand = FTM_GSM_BER_EGSM_900;
      QLIB_FTM_GSM_SELECT_SPECIFIC_BCCH_REQ(g_hResourceContext, iChannel, iBand, &iStatus);
      printf("QLIB_FTM_GSM_SELECT_SPECIFIC_BCCH_REQ(iChannel=%d, iBand=%d): iStatus=%d\n", iChannel, iBand, iStatus);

      // Check for BCCH acquire status
      QLIB_FTM_GSM_BER_EVENT_STATUS_REQ(g_hResourceContext, FTM_GSM_BER_SELECT_SPECIFIC_BCCH_CNF_SYNC_STATUS, 2000, &iStatus);
      printf("QLIB_FTM_GSM_BER_EVENT_STATUS_REQ(FTM_GSM_BER_SELECT_SPECIFIC_BCCH_CNF_SYNC_STATUS) Status=%d\n", iStatus);

      unsigned char iCStatus1 = 0;

      // Go to IDLE
      QLIB_FTM_EGPRS_BER_START_GPRS_IDLE_MODE_REQ(g_hResourceContext, &iCStatus1);
      printf("QLIB_FTM_EGPRS_BER_START_GPRS_IDLE_MODE_REQ: status=%d\n", iCStatus1);

      // Wait for IDLE
      QLIB_FTM_GSM_BER_EVENT_STATUS_REQ(g_hResourceContext, FTM_GSM_BER_START_IDLE_MODE_SYNC_STATUS, 1000, &iStatus);
      printf("QLIB_FTM_GSM_BER_EVENT_STATUS_REQ(FTM_GSM_BER_START_IDLE_MODE_SYNC_STATUS) status=%d\n", iStatus);

      // Clear EGPRS event
      QLIB_FTM_EGPRS_BER_EVENT_STATUS_CLEAR_REQ(g_hResourceContext, &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_CLEAR_REQ: status=%d\n", iStatus);

      // 2 DL/UL slot configuration
      unsigned char iDL_TSC= 0;
      unsigned char iDL_TFI= 0;
      unsigned short iChannel1 = 62;
      unsigned char bFirstSlotActive= 1;
      unsigned char iFirstSlotNum= 4;
      unsigned char bSecondSlotActive= 1;
      unsigned char iSecondSlotNum= 5;

      // Configure DL
      QLIB_FTM_EGPRS_BER_CONFIGURE_DL_TBF(g_hResourceContext,
                                          iDL_TSC,
                                          iDL_TFI,
                                          iChannel1,
                                          iBand,
                                          bFirstSlotActive,
                                          iFirstSlotNum,
                                          bSecondSlotActive,
                                          iSecondSlotNum,
                                          &iCStatus1 );

      printf("QLIB_FTM_EGPRS_BER_CONFIGURE_DL_TBF(status = %d)\n", iCStatus1);

      unsigned char iUL_TSC = 0;
      unsigned char iUSF = 0;
      unsigned char iGamma = 13;

      QLIB_FTM_EGPRS_BER_CONFIGURE_UL_TBF(  g_hResourceContext,
                                            iUL_TSC,
                                            iUSF,
                                            iGamma,
                                            bFirstSlotActive,
                                            iFirstSlotNum,
                                            bSecondSlotActive,
                                            iSecondSlotNum,
                                            &iCStatus1 );

      printf("QLIB_FTM_EGPRS_BER_CONFIGURE_UL_TBF(Status = %d)", iCStatus1);

      // SRB loop back
      QLIB_FTM_EGPRS_BER_ESTABLISH_SRB_LOOPBACK(g_hResourceContext, &iCStatus1);
      printf("QLIB_FTM_EGPRS_BER_ESTABLISH_SRB_LOOPBACK: Status=%d\n", iCStatus1);

      // Wait for DL
      QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(g_hResourceContext,
                                          FTM_EGPRS_BER_DL_TBF_ESTABLISHED_SYNC_STATUS,
                                          1000,
                                          &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ( FTM_EGPRS_BER_DL_TBF_ESTABLISHED_SYNC_STATUS ): Status=%d\n", iStatus);

      // Wait for UL
      QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(g_hResourceContext,
                                          FTM_EGPRS_BER_UL_TBF_ESTABLISHED_SYNC_STATUS,
                                          1000,
                                          &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ( FTM_EGPRS_BER_UL_TBF_ESTABLISHED_SYNC_STATUS ): Status=%d\n", iStatus);

      // wait for UL success
      if(iStatus == 0)
      {
         printf("EDGE call is up\n");
      }

      _iCallSetupTime = ::GetTickCount() - _iCallSetupTime;
      printf("_iCallSetupTime = %d\n", _iCallSetupTime);

      QLIB_StopLogging( g_hResourceContext );
   }
   else if (cKey == '8')
   {
      // EDGE Change UL power
      unsigned char iUL_TSC= 0;
      unsigned char iUSF = 0;
      unsigned char iGamma = 13;

      unsigned short iChannel1 = 62;
      unsigned char iBand = 1; // GSM 900
      //unsigned char iBand1= 0;
      unsigned char bFirstSlotActive= 1;
      unsigned char iFirstSlotNum= 4;
      unsigned char bSecondSlotActive= 1;
      unsigned char iSecondSlotNum= 5;
      unsigned short iStatus = 0;
      unsigned char iCStatus1 = 0;
      unsigned char _userGamma = 0;

      QLIB_StartLogging( g_hResourceContext,  "C:\\EGPRS_non_signaling_GAMMA_Change.txt");
      printf("\nGAMMA Level ? A = 0, B = 1, C = 2, ...\n");
      _userGamma = getch() - 'A';
      printf("GAMMA is %d\n", _userGamma);

      QLIB_FTM_EGPRS_BER_CONFIGURE_UL_TBF(  g_hResourceContext,
                                            iUL_TSC,
                                            iUSF,
                                            _userGamma,
                                            bFirstSlotActive,
                                            iFirstSlotNum,
                                            bSecondSlotActive,
                                            iSecondSlotNum,
                                            &iCStatus1 );

      printf("QLIB_FTM_EGPRS_BER_CONFIGURE_UL_TBF(Status = %d)", iStatus);

      // Clear EGPRS event
      QLIB_FTM_EGPRS_BER_EVENT_STATUS_CLEAR_REQ(g_hResourceContext, &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_CLEAR_REQ: status=%d\n", iStatus);

      //  SRB loop back
      QLIB_FTM_EGPRS_BER_ESTABLISH_SRB_LOOPBACK(g_hResourceContext, &iCStatus1);
      printf("QLIB_FTM_EGPRS_BER_ESTABLISH_SRB_LOOPBACK: Status=%d\n", iCStatus1);

      // Wait for uplink
      QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(g_hResourceContext,
                                          FTM_EGPRS_BER_UL_TBF_ESTABLISHED_SYNC_STATUS,
                                          1000,
                                          &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ( status = %d)\n", iStatus);

      QLIB_StopLogging( g_hResourceContext );
   }
   else if (cKey == '9')
   {
      // EDGE Change UL power
      unsigned char iDL_TSC= 0;
      unsigned char iDL_TFI= 0;
      unsigned short iChannel1 = 62;
      unsigned char iBand = 1; // GSM 900
      //unsigned char iBand1= 0;
      unsigned short iStatus = 0;
      unsigned char iCStatus1 = 0;
      unsigned char _userGamma = 0;

      GSM_EGPRS_CONFIGURE_DL_TBF_V2_Slot_Configuration oDLSlotConfig = { 0 };
      GSM_EGPRS_CONFIGURE_UL_TBF_V2_Slot_Configuration oULSlotConfig = { 0 };

      int i;

      QLIB_StartLogging( g_hResourceContext,  "C:\\EGPRS_non_signaling_GAMMA_Change_TBF_V2.txt");

      printf("\nGAMMA Level ? A = 0, B = 1, C = 2, ...\n");
      _userGamma = getch() - 'A';
      printf("GAMMA is %d\n", _userGamma);

      // 4UL/1DL DL(4), UL(4,5,6,3)
      for( i = 0; i < 2; i++)
      {
         if(i == 0)
         {
            oDLSlotConfig.slots[i].active = 1 ;
            oDLSlotConfig.slots[i].timeSlotNumber = i+4;
         }
         oULSlotConfig.slots[i].active = 1 ;
         oULSlotConfig.slots[i].timeSlotNumber = i+4;
         oULSlotConfig.slots[i].gamma = _userGamma;;
      }
      oULSlotConfig.slots[3].active = 1 ;
      oULSlotConfig.slots[3].timeSlotNumber = 6;
      oULSlotConfig.slots[3].gamma = _userGamma;;

      oULSlotConfig.slots[4].active = 1 ;
      oULSlotConfig.slots[4].timeSlotNumber = 3;
      oULSlotConfig.slots[4].gamma = _userGamma;;

      // 3UL/2DL DL(4,5), UL(4,5,6)
      for( i = 0; i < 2; i++)
      {
         oDLSlotConfig.slots[i].active = 1 ;
         oDLSlotConfig.slots[i].timeSlotNumber = i+4;
         oULSlotConfig.slots[i].active = 1 ;
         oULSlotConfig.slots[i].timeSlotNumber = i+4;
         oULSlotConfig.slots[i].gamma = _userGamma;;
      }
      oULSlotConfig.slots[3].active = 1 ;
      oULSlotConfig.slots[3].timeSlotNumber = 6;
      oULSlotConfig.slots[3].gamma = _userGamma;;

      // 2UL/2DL DL(4,6), UL(4,6)
      /*
      for( i = 0; i < 2; i++)
      {
      oDLSlotConfig.slots[i].active = 1 ;
      if(i ==0)
      oDLSlotConfig.slots[i].timeSlotNumber = i+4;
      else
      oDLSlotConfig.slots[i].timeSlotNumber = i+5;
      oULSlotConfig.slots[i].active = 1 ;
      if(i==0)
      oULSlotConfig.slots[i].timeSlotNumber = i+4;
      else
      oULSlotConfig.slots[i].timeSlotNumber = i+5;
      oULSlotConfig.slots[i].gamma = _userGamma;;
      }
      */

      /* 2UL/2DL DL(4,5), UL(4,5)
      for( i = 0; i < 2; i++)
      {
      oDLSlotConfig.slots[i].active = 1 ;
      oDLSlotConfig.slots[i].timeSlotNumber = i+4;
      oULSlotConfig.slots[i].active = 1 ;
      oULSlotConfig.slots[i].timeSlotNumber = i+4;
      oULSlotConfig.slots[i].gamma = _userGamma;;
      }

      /***
      for( i = 0; i < 4; i++)
      {
      oDLSlotConfig.slots[i].active = 0 ;
      oDLSlotConfig.slots[i].timeSlotNumber = i;
      oULSlotConfig.slots[i].active = 0 ;
      oULSlotConfig.slots[i].timeSlotNumber = i;
      oULSlotConfig.slots[i].gamma = 0;
      }
      for( i = 4; i < 6; i++)
      {
      oDLSlotConfig.slots[i].active = 1 ;
      oDLSlotConfig.slots[i].timeSlotNumber = i;
      oULSlotConfig.slots[i].active = 1 ;
      oULSlotConfig.slots[i].timeSlotNumber = i;
      oULSlotConfig.slots[i].gamma = _userGamma;
      }
      for( i = 6; i < 8; i++)
      {
      oDLSlotConfig.slots[i].active = 0 ;
      oDLSlotConfig.slots[i].timeSlotNumber = i;
      oULSlotConfig.slots[i].active = 0 ;
      oULSlotConfig.slots[i].timeSlotNumber = i;
      oULSlotConfig.slots[i].gamma = 0;
      }
      ****/

      // Configure DL
      QLIB_FTM_EGPRS_BER_CONFIGURE_DL_TBF_V2(g_hResourceContext,
                                             iDL_TSC,
                                             iDL_TFI,
                                             iChannel1,
                                             iBand,
                                             (void*)&oDLSlotConfig,
                                             &iStatus);

      printf("QLIB_FTM_EGPRS_BER_CONFIGURE_DL_TBF_V2(status = %d)\n", iStatus);

      unsigned char iUL_TSC = 0;
      unsigned char iUSF = 0;
      unsigned char iGamma = _userGamma;

      QLIB_FTM_EGPRS_BER_CONFIGURE_UL_TBF_V2(   g_hResourceContext,
                                                iUL_TSC,
                                                iUSF,
                                                (void*)&oULSlotConfig,
                                                &iStatus );

      printf("QLIB_FTM_EGPRS_BER_CONFIGURE_UL_TBF_V2(Status = %d)", iStatus);

      // Clear EGPRS event
      QLIB_FTM_EGPRS_BER_EVENT_STATUS_CLEAR_REQ(g_hResourceContext, &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_CLEAR_REQ: status=%d\n", iStatus);

      QLIB_FTM_EGPRS_BER_ESTABLISH_SRB_LOOPBACK(g_hResourceContext, &iCStatus1);
      printf("QLIB_FTM_EGPRS_BER_ESTABLISH_SRB_LOOPBACK: Status=%d\n", iCStatus1);

      // Wait for DL
      QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(g_hResourceContext,
                                          FTM_EGPRS_BER_DL_TBF_ESTABLISHED_SYNC_STATUS,
                                          1000,
                                          &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(FTM_EGPRS_BER_DL_TBF_ESTABLISHED_SYNC_STATUS): Status=%d\n", iStatus);

      // Wait for UL
      QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(g_hResourceContext,
                                          FTM_EGPRS_BER_UL_TBF_ESTABLISHED_SYNC_STATUS,
                                          1000,
                                          &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(FTM_EGPRS_BER_UL_TBF_ESTABLISHED_SYNC_STATUS): Status=%d\n", iStatus);
   }
   else if (cKey == 'A')
   {
      unsigned char iCStatus1 = 0;
      unsigned short iStatus = 0;

      // Clear EGPRS event
      QLIB_FTM_EGPRS_BER_EVENT_STATUS_CLEAR_REQ(g_hResourceContext, &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_CLEAR_REQ: status=%d\n", iStatus);

      // Wait for DL
      QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(g_hResourceContext,
                                          FTM_EGPRS_BER_DL_TBF_RELEASED_SYNC_STATUS,
                                          1000,
                                          &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(FTM_EGPRS_BER_DL_TBF_ESTABLISHED_SYNC_STATUS): Status=%d\n", iStatus);

      // Wait for UL
      QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(g_hResourceContext,
                                          FTM_EGPRS_BER_UL_TBF_RELEASED_SYNC_STATUS,
                                          1000,
                                          &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(FTM_EGPRS_BER_UL_TBF_ESTABLISHED_SYNC_STATUS): Status=%d\n", iStatus);

      // Releas TBF
      QLIB_FTM_EGPRS_BER_RELEASE_ALL_TBF(g_hResourceContext,    &iCStatus1);
      printf("QLIB_FTM_EGPRS_BER_ESTABLISH_SRB_LOOPBACK: Status=%d\n", iCStatus1);
      Sleep(100);
   }
   else if (cKey == 'B')
   {
      // EDGE Change UL power
      unsigned char iDL_TSC= 0;
      unsigned char iDL_TFI= 0;
      unsigned short iChannel1 = 63;
      unsigned char iBand = 1; // GSM 900
      //unsigned char iBand1= 0;
      unsigned char bFirstSlotActive= 1;
      unsigned char iFirstSlotNum= 4;
      unsigned char bSecondSlotActive= 1;
      unsigned char iSecondSlotNum= 5;
      unsigned short iStatus = 0;
      unsigned char iCStatus1 = 0;
      unsigned char _userGamma = 0;

      QLIB_StartLogging( g_hResourceContext,  "C:\\EGPRS_non_signaling_GAMMA_Change.txt");
      printf("\nGAMMA Level ? A = 0, B = 1, C = 2, ...\n");
      _userGamma = getch() - 'A';
      printf("GAMMA is %d\n", _userGamma);

      // Releas TBF
      QLIB_FTM_EGPRS_BER_RELEASE_ALL_TBF(g_hResourceContext,    &iCStatus1);

      // Configure DL
      QLIB_FTM_EGPRS_BER_CONFIGURE_DL_TBF(g_hResourceContext,
                                          iDL_TSC,
                                          iDL_TFI,
                                          iChannel1,
                                          iBand,
                                          bFirstSlotActive,
                                          iFirstSlotNum,
                                          bSecondSlotActive,
                                          iSecondSlotNum,
                                          &iCStatus1 );

      printf("QLIB_FTM_EGPRS_BER_CONFIGURE_DL_TBF(status = %d)\n", iCStatus1);

      // Wait for DL
      QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(g_hResourceContext,
                                          2, //FTM_EGPRS_BER_DL_TBF_ESTABLISHED_SYNC_STATUS,
                                          1000,
                                          &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(FTM_EGPRS_BER_UL_TBF_ESTABLISHED_SYNC_STATUS): Status=%d\n", iStatus);

      unsigned char iUL_TSC = 0;
      unsigned char iUSF = 0;
      unsigned char iGamma = _userGamma;
      //unsigned char bFirstSlotActive;
      //unsigned char iFirstSlotNum;
      //unsigned char bSecondSlotActive;
      //unsigned char iSecondSlotNum;

      QLIB_FTM_EGPRS_BER_CONFIGURE_UL_TBF(  g_hResourceContext,
                                            iUL_TSC,
                                            iUSF,
                                            iGamma,
                                            bFirstSlotActive,
                                            iFirstSlotNum,
                                            bSecondSlotActive,
                                            iSecondSlotNum,
                                            &iCStatus1 );

      printf("QLIB_FTM_EGPRS_BER_CONFIGURE_UL_TBF(Status = %d)", iStatus);

      QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ(g_hResourceContext,
                                          0, //FTM_EGPRS_BER_UL_TBF_ESTABLISHED_SYNC_STATUS
                                          1000,
                                          &iStatus);
      printf("QLIB_FTM_EGPRS_BER_EVENT_STATUS_REQ( status = %d)\n", iStatus);

      QLIB_FTM_EGPRS_BER_ESTABLISH_SRB_LOOPBACK(g_hResourceContext, &iCStatus1);
      printf("QLIB_FTM_EGPRS_BER_ESTABLISH_SRB_LOOPBACK: Status=%d\n", iCStatus1);

      QLIB_StopLogging( g_hResourceContext );
   }

   // If valid key hit...
   if (_bValidKeyHit )
   {
      printf("\n\nCMU-200 Setup:");
      printf("\n  - Select GSM signaling mode for the correct band");
      printf("\n  - BS Signal -> Mode = BCCH and TCCH");
      printf("\n  - BS Signal -> BCCH = %d", _iChannel_BCCH );
      printf("\n  - BS Signal -> TCH = %d", _iChannel_TCH );

      printf("\n  Other steps are necessary as well, please follow 80-VA360-1" );

      printf("\n\n  <<<Press any key to continue>>>");
   }

   if ( (cKey == '1' )
         ||    (cKey == '2' )
         ||    (cKey == '3' )
      )
   {
      // printf("\n4=GSM900 Test (automated library commands)" );

      // Get the user key
      getch();

      /////////////////////////////////////////
      // Request call start, with session init
      /////////////////////////////////////////
      bOk = QLIB_FTM_GSM_BER_StartSession(
               g_hResourceContext,
               FTM_NONSIG_DEFAULT_TIMEOUT );

      printf("\n  QLIB_FTM_GSM_BER_StartSession: %s", bOk ? "Pass" : "FAIL" );

      /////////////////////////////////////////
      // Assign BCCH
      /////////////////////////////////////////
      bOk = QLIB_FTM_EGPRS_BER_AssignBCCH(
               g_hResourceContext,
               _iChannel_BCCH,
               _iBand
            );

      printf("\n  QLIB_FTM_EGPRS_BER_AssignBCCH: %s", bOk ? "Pass" : "FAIL" );

      if ( bOk )
      {
         /////////////////////////////////////////
         // Configure Downlink TBF
         /////////////////////////////////////////
         unsigned char iDL_TSC          = 0;
         unsigned char iDL_TFI          = 0;
         unsigned char iBand                = FTM_GSM_BER_EGSM_900;
         unsigned char bFirstSlotActive = true;
         unsigned char iFirstSlotNum        = 3;
         unsigned char bSecondSlotActive    = FALSE;        // Second slot note supported right now
         unsigned char iSecondSlotNum   = 0;

         bOk = QLIB_FTM_EGPRS_BER_Configure_DL_TBF (
                  g_hResourceContext,
                  iDL_TSC,
                  iDL_TFI,
                  _iChannel_TCH,
                  iBand,
                  bFirstSlotActive,
                  iFirstSlotNum,
                  bSecondSlotActive,
                  iSecondSlotNum
               );
         printf("\n  QLIB_FTM_EGPRS_BER_Configure_DL_TBF: %s", bOk ? "Pass" : "FAIL" );
      }

      if ( bOk )
      {
         /////////////////////////////////////////
         // Configure Uplink TBF
         /////////////////////////////////////////

         unsigned char iUL_TSC          = 0;
         unsigned char iUSF             = 0;
         unsigned char iGamma           = 0;
         unsigned char bFirstSlotActive = 1;
         unsigned char iFirstSlotNum        = 3;
         unsigned char bSecondSlotActive    = FALSE;        // Second slot not supported right now
         unsigned char iSecondSlotNum   = 2;

         bOk = QLIB_FTM_EGPRS_BER_Configure_UL_TBF  (
                  g_hResourceContext,
                  iUL_TSC,
                  iUSF,
                  iGamma,
                  bFirstSlotActive,
                  iFirstSlotNum,
                  bSecondSlotActive,
                  iSecondSlotNum
               );
         printf("\n  QLIB_FTM_EGPRS_BER_Configure_DL_TBF: %s", bOk ? "Pass" : "FAIL" );
      }

      if ( bOk )
      {
         // Now connect
         printf("\n\nSetup call box to \"connect,\" then <<press any key to continue>>...");
         getch();

         bOk = QLIB_FTM_EGPRS_BER_Establish_SRB_LOOPBACK ( g_hResourceContext);

         printf("\n  QLIB_FTM_EGPRS_BER_Establish_SRB_LOOPBACK : %s", bOk ? "Pass" : "FAIL" );
      }

      /////////////////////////////////////////
      // Get GSM Rx Level
      /////////////////////////////////////////
      if ( bOk )
      {
         char _cKey2 = ' ';

         do
         {
            // Request call start
            printf("\n\nAdjust Rx level, then press <<R>> for Rx Report report or <<any other key to continue>>");
            _cKey2 = toupper( getch() );

            if ( _cKey2 == 'R')
            {
               // Get the rx level
               unsigned char _iRxLev_average;
               QLIB_FTM_EGPRS_BER_GetRxMetrics(g_hResourceContext, &_iRxLev_average );

               printf("\n  QLIB_FTM_EGPRS_BER_GetRxMetrics: %s, Level: %d", bOk ? "Pass" : "FAIL", _iRxLev_average );
            }
         }
         while ( _cKey2 == 'R');
      } // QLIB_FTM_GSM_BER_ReleaseTCH

      /////////////////////////////////////////
      // Release 2nd traffic
      /////////////////////////////////////////
      if ( bOk )
      {
         // Request call start
         printf("\n\nPerform testing, then disconnect call and turn off BS power then, <<press any key to stop call >>");
         getch();

         QLIB_FTM_EGPRS_BER_Release_All_TBF( g_hResourceContext );

         printf("\n QLIB_FTM_EGPRS_BER_Release_All_TBF: %s", bOk ? "Pass" : "FAIL" );
      } // QLIB_FTM_GSM_BER_ReleaseTCH

      /////////////////////////////////////////
      // Close Session
      /////////////////////////////////////////
      // if ( bOk ) -- do this regardless of the other status
      {
         QLIB_FTM_GSM_BER_StopSession(g_hResourceContext );

         printf("\n QLIB_FTM_GSM_BER_StopCall: %s", bOk ? "Pass" : "FAIL" );
      } // QLIB_FTM_GSM_BER_StopSession
   } // cKey == '1'...'4'

   return bOk;
}
