/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/QMSL_NVManager_Demo.cpp#8 $
 * $DateTime: 2017/05/15 18:38:10 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2014-2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#include "QMSL_NVManager_Demo.h"
#include <stdlib.h>
#include <sstream>
#include <conio.h>

// Disable compiler waring 'getch' was declared deprecated, for MSVC8
#pragma warning (disable : 4996)

extern HANDLE g_hResourceContext;

unsigned char GPS_RFIC_IM2_calibration_test( void );
unsigned char Test_NV_Manager( void )
{
   bool bContinue = true;
   int cKey = 0;

   do
   {
      printf("\n\n\n======================================\n");
      printf("\n 1 = MSM6800_DZRF");
      printf("\n 2 = CDMA2000_TxBetaScaling");
      printf("\n 3 = QSC60x0_MSM60xx");
      printf("\n 4 = UMTS_RTR6285_TxCalibration");
      printf("\n 5 = Tx_3U_Calibration");
      printf("\n 6 = GSM_Tx_Polar_Calibration");
      printf("\n 7 = GSM_Rx_Calibration_And_Misc");
      printf("\n 8 = Add_OEM_NV_Item");
      printf("\n 9 = Create_Text_Report");
      printf("\n 0 = Write_NV_To_Mobile");
      printf("\n A = UMTS_RTR6285_Tx2ndFreqComp");
      printf("\n B = CDMA_RTR6500_Calibration");
      printf("\n C = Clear_Data");
      printf("\n D = LinearInterpolation_SlopeAveragingExtrapolation"); 
      printf("\n E = GPS_RFIC_IM2_calibration_test");
      printf("\n F = GSM_Tx_Linear_Calibration");
      printf("\n G = GSM_Tx_Linear_Calibration_V2");
      printf("\n H = WCDMA_RTR8600_Calibration");
      printf("\n I = LTE_RTR8600_Calibration");
     printf("\n J = Enhanced Sweep Calibration NV Generation");
     printf("\n K = Backup RFNV item to NV Manager, Copy RFNV item data from NV Tool to NV Manager");
     printf("\n L = GSM_Tx_Linear_Calibration_MultiChain");
     printf("\n M = GSM_Rx_Calibration_MultiChain");
     printf("\n N = Read and Write Compressed RF NV");
      printf("\n Q = Quit");
      printf("\n ->");
      // Get a key
      cKey = toupper( getch() );

      printf("\n\n\n\n\n\n\n\n");
      switch (cKey)
      {
      case '1':
         MSM6800_DZRF();
         break;
      case '2':
         CDMA2000_TxBetaScaling();
         break;
      case '4':
         UMTS_RTR6285_Tx_Calibration();
         break;
      case '5':
         Tx_3U_Calibration();
         break;
      case '6':
         GSM_Tx_Polar_Calibration();
         break;
      case '7':
         GSM_Rx_Calibration_And_Misc( );
         break;
      case '8':
         Add_OEM_NV_Item();
         break;
      case '9':
         Create_Text_Report( );
         break;
      case '0':
         Write_NV_To_Mobile();
         break;
      case 'A':
         UMTS_RTR6285_Tx2ndFreqComp();
         break;
      case 'B':
         CDMA_RTR6500_Calibration();
         break;
      case 'C':
         Clear_Data();
         break;
      case 'D':
         LinearInterpolation_SlopeAveragingExtrapolation();
         break;
      case 'E':
         GPS_RFIC_IM2_calibration_test();
         break;
      case 'F':
         GSM_Tx_Linear_Calibration();
         break;
      case 'G':
         GSM_Tx_Linear_Calibration_V2();
         break;
      case 'H':
         WCDMA_RTR8600_Calibration();
         break;
      case 'I':
         LTE_RTR8600_Calibration();
         break;
     case 'J':
         Enhanced_Sweep_Calibration_NV_Generation();
         break;
     case 'K':
       BackupRFNV_and_CopyDataFromNVTool();
       break;
     case 'L':
        GSM_Tx_Linear_Calibration_Multichain();
        break;
     case 'M':
        GSM_Rx_Calibration_MultiChain();
        break;
     case 'N':
        Read_Write_Compressed_RFNV();
        break;

      case 'Q':
         bContinue = false;
         break;
      } // switch( cKey)

   } while (bContinue);

   return 1;
}

// CDMA Intelliceiver Platform
unsigned char MSM6800_DZRF( void )
{
   // DVGA LNA gain offset, offset vs freq

   QMSL_CDMA_RxFreq_Measurement_struct aRX_Measured;
   QMSL_CDMA_MeasuredChannels_struct RX_MeasuredChannels;
   QMSL_CDMA_NVChannels_struct NV_Channels;



   //Measurement Setup Rx Parameters
   unsigned short DVGA_PowerMode0[16] = {2229, 2388, 2564, 2200, 2200, 2387, 2365,2218, 2460, 2547, 2046, 2047, 2129, 2600, 2345, 2214};
   unsigned short LNA0_PowerMode0[16] = {106, 103, 113, 121, 123, 122, 120,111, 118,111, 116, 109, 110, 115, 116, 118};
   unsigned short LNA1_PowerMode0[16] = {134, 134, 134, 134, 134, 134, 134,134, 134,134, 134, 134, 134, 134, 134, 134};
   unsigned short LNA2_PowerMode0[16] = {233, 234, 238, 243, 244, 238, 234,232, 229,228, 222, 219, 225, 230, 232, 234};


   unsigned short DVGA_PowerMode1[16] = {2229, 2388, 2564, 2200, 2200, 2387, 2365,2218, 2460, 2547, 2046, 2047, 2129, 2600, 2345, 2214};
   unsigned short LNA0_PowerMode1[16] = {106, 103, 113, 121, 123, 122, 120,111, 118,111, 116, 109, 110, 115, 116, 118};
   unsigned short LNA1_PowerMode1[16] = {134, 134, 134, 134, 134, 134, 134,134, 134,134, 134, 134, 134, 134, 134, 134};
   unsigned short LNA2_PowerMode1[16] = {233, 234, 238, 243, 244, 238, 234,232, 229,228, 222, 219, 225, 230, 232, 234};


   unsigned short DVGA_PowerMode2[16] = {2229, 2388, 2564, 2200, 2200, 2387, 2365,2218, 2460, 2547, 2046, 2047, 2129, 2600, 2345, 2214};
   unsigned short LNA0_PowerMode2[16] = {106, 103, 113, 121, 123, 122, 120,111, 118,111, 116, 109, 110, 115, 116, 118};
   unsigned short LNA1_PowerMode2[16] = {134, 134, 134, 134, 134, 134, 134,134, 134,134, 134, 134, 134, 134, 134, 134};
   unsigned short LNA2_PowerMode2[16] = {233, 234, 238, 243, 244, 238, 234,232, 229,228, 222, 219, 225, 230, 232, 234};


   // Copy measurement data to QMSL's structure
   unsigned short BC0_Channel[16] = {1018,46,98,150,202,254,306,358,410,462,514,566,618,670,722,774};

   int _ch;
   for(_ch = 0; _ch < 16; _ch++)
   {
      aRX_Measured.aiDVGA_Measured[_ch][0]   = DVGA_PowerMode0[_ch];
      aRX_Measured.aiLNA_Measured[_ch][0][0] = LNA0_PowerMode0[_ch];
      aRX_Measured.aiLNA_Measured[_ch][1][0] = LNA1_PowerMode0[_ch];
      aRX_Measured.aiLNA_Measured[_ch][2][0] = LNA2_PowerMode0[_ch];

      aRX_Measured.aiDVGA_Measured[_ch][1]   = DVGA_PowerMode1[_ch];
      aRX_Measured.aiLNA_Measured[_ch][0][1] = LNA0_PowerMode1[_ch];
      aRX_Measured.aiLNA_Measured[_ch][1][1] = LNA1_PowerMode1[_ch];
      aRX_Measured.aiLNA_Measured[_ch][2][1] = LNA2_PowerMode1[_ch];


      aRX_Measured.aiDVGA_Measured[_ch][2]   = DVGA_PowerMode2[_ch];
      aRX_Measured.aiLNA_Measured[_ch][0][2] = LNA0_PowerMode2[_ch];
      aRX_Measured.aiLNA_Measured[_ch][1][2] = LNA1_PowerMode2[_ch];
      aRX_Measured.aiLNA_Measured[_ch][2][2] = LNA2_PowerMode2[_ch];

   }

   // Number of measured LNA states
   aRX_Measured.iNumMeasured_LNA_States = 3;

   /*
   aiLNA_States[n] is the LNA state which the measurment data aiLNA_Measured[n] belongs to.  The highest gain state is 0.
   In this case, the LNA NV will be mapped to LNA_2, LNA_3 and LNA_4 offset

   */
   aRX_Measured.aiLNA_States[0] =  1;
   aRX_Measured.aiLNA_States[1] =  2;
   aRX_Measured.aiLNA_States[2] =  3;

   // Number of pwoer mode
   aRX_Measured.iNumMeasuredPowerModes = 3;

   // Primary Rx
   aRX_Measured.iPath = 0;

   // Fill out the channels which the measurement are made
   for(_ch = 0; _ch < 16; _ch++)
      RX_MeasuredChannels.aiMeasuredChannels[_ch] = BC0_Channel[_ch];
   RX_MeasuredChannels.iNumMeasuredChannels = 16;
   RX_MeasuredChannels.iRefChannel = 358;


   // The channels 
   for(_ch = 0; _ch < 16; _ch++)
   {
      NV_Channels.aiNVChannels[_ch] = BC0_Channel[_ch];
   }      

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_Rx_DVGA_LNA_Cal_NV_Result RX_NV_Result;


   QLIB_RFCAL_CDMA_RxCal_Results(   g_hResourceContext, 
      NV_MODE_BC0,
      1, 
      12,
      (void*)&aRX_Measured, 
      (void*)&RX_MeasuredChannels, 
      (void*)&NV_Channels, 
      (void*) &RX_NV_Result,
      1,         //Write NV cal channel
      1); 


   // Secondary Rx.  Call this function again for diversity path NV

   aRX_Measured.iPath = 1;
   QLIB_RFCAL_CDMA_RxCal_Results(   g_hResourceContext, 
      NV_MODE_BC0,
      1, 
      12,
      (void*)&aRX_Measured, 
      (void*)&RX_MeasuredChannels, 
      (void*)&NV_Channels, 
      (void*) &RX_NV_Result,
      1, //Write NV cal channel
      1); 


   //*****
   //Tx Linearizer Vs Frequency (TX_COMP)
   //*****
   QMSL_CDMA_TxLinVsFreq_struct TxLinVsFreq;
   QMSL_CDMA_MeasuredChannels_struct TxMeasuredChs;


   // Measurement Setup Tx Comp Parameters
   // The power levels across frequencies for the same PDM
   int      TxFreqComCh [16] = {1013, 46, 98, 150, 202, 254, 306, 358, 410, 462, 514, 566, 618, 670, 722, 774};
   double   Tx0_FreqComp [16] = {13.05, 13.29, 13.62, 13.70, 13.51, 13.11, 12.68, 12.55, 12.51, 12.73, 13.30, 13.78, 14.01, 13.85, 13.46, 12.82};
   double   Tx1_FreqComp [16] = {25.01, 24.93, 24.83, 24.55, 24.27, 24.00, 23.78, 23.96, 24.02, 24.30, 24.67, 24.82, 24.71, 24.36, 23.94, 23.48};

   // Fill out the measurment channels 
   int i;
   for(i = 0 ;i < 16; i++)
   {
      TxMeasuredChs.aiMeasuredChannels [i] = TxFreqComCh[i];
   }
   TxMeasuredChs.iRefChannel = 358;
   TxMeasuredChs.iNumMeasuredChannels = 16;

   // Copy measurement data to QMSL's structure
   for(i = 0; i < 16; i++)
   {
      TxLinVsFreq.aiTxMeasuredPower[0][i] = Tx0_FreqComp[i];
      TxLinVsFreq.aiTxMeasuredPower[1][i] = Tx1_FreqComp[i];
   }

   TxLinVsFreq.iNumMeasuredTxGainStates = 2;
   // aiTxMeasToNVMapping[n] is the Tx Gain state which aiTxMeasurementPower[n][16] are made
   TxLinVsFreq.aiTxMeasToNVMapping[0] = 0;
   TxLinVsFreq.aiTxMeasToNVMapping[1] = 1;
   // overwrite static component with zero
   TxLinVsFreq.bC2K_Not_Overwrite_Tx_Pwr_Comp_Static_Component_With_Zero = 0;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_Tx_LinVsFreq_Cal_NV_Result_struct TxLinVsFreq_NV;   

   QLIB_RFCAL_CDMA_Tx_LinVsFreq_Results   (g_hResourceContext, 
      NV_MODE_BC0, 
      (void*)&TxLinVsFreq, 
      (void*)&TxMeasuredChs, 
      (void*)&NV_Channels,
      10,
      QMSL_NV_TX_COMP,
      (void*)&TxLinVsFreq_NV,
      1,  //Write NV cal channel
      1);




   //****
   //Tx Linearizer (LIN_MASTER)
   //****

   // Measurement Setup Tx Lin Parameters

   double Tx0_LinPower[11] = {15, 13, 10, 7, 4, 0, -4, -8, -12, -15, -20}; //dBm
   int      Tx0_LinPDM[11]  = {100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300}; // PDM

   double Tx1_LinPower[12] = {24.05, 22.15, 20.19, 18.19, 16.13, 14.04, 11.90, 9.77, 7.60, 5.40, 3.22, 0.99};  //dBm
   int      Tx1_LinPDM[12]  = {89, 99, 109, 119, 129, 139, 149, 159, 169, 179, 189, 199}; //PDM
   int      Tx1_HDET[12] = {137, 113, 92, 75, 60, 48, 37, 30, 23, 17, 13, 9}; //HDET

   QMSL_CDMA_TxLin_struct TxLinData;


   //Copy measurement setup data to QMSL structures
   for(i = 0; i < 11; i++)
   {
      TxLinData.aiTxLinPDM[0][i]      = Tx0_LinPDM[i];
      TxLinData.aiTxLinPower[0][i]   = Tx0_LinPower[i];
   }
   TxLinData.iNumTxPowerMeasurementsPerRange[0] = 11;

   for(i = 0; i < 12; i++)
   {
      TxLinData.aiTxLinPDM[1][i]      = Tx1_LinPDM[i];
      TxLinData.aiTxLinPower[1][i]   = Tx1_LinPower[i];
      TxLinData.aiTxHDET[i] = Tx1_HDET[i];
   }
   TxLinData.iNumTxPowerMeasurementsPerRange[1] = 12;

   // Number of gain state
   TxLinData.iNumMeasuredTxGainStates  = 2;

   TxLinData.HDET_OffsetPower_dBm = 18;
   TxLinData.HDET_SpanPower_dBm = 26;


   // NV mapping 
   TxLinData.aiTxMeasToNVMapping[0] = 0;
   TxLinData.aiTxMeasToNVMapping[1] = 1;

   TxLinData.iTxMeasIndexUsedForHDET = 1;

   // Not using LPM HDET
   TxLinData.aiTxHDETLPMEnabled = 0;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_TxLin_Cal_Result_struct TxLinNV;

   QLIB_RFCAL_CDMA_Tx_LinCal_Results(g_hResourceContext, NV_MODE_BC0, (void*)&TxLinData, (void*)&TxLinNV,1);


   //****
   //Tx LIM vs Freq
   //****
   // Measurement data for Tx Lim Vs Freq
   double Ch1013_LimVsFreq_Power[] = {23.5, 24.0, 24.5};
   int      Ch1013_LimVsFreq_HDET[] =  {137, 144, 149};
   int      Ch1013_LimVsFreq_PDM[]  =  {105, 115, 120};

   double Ch358_LimVsFreq_Power[] = {23.5, 23.9, 24.5};
   int      Ch358_LimVsFreq_HDET[] =  {128, 136, 141};
   int      Ch358_LimVsFreq_PDM[]  =  {110, 120, 130};

   double Ch774_LimVsFreq_Power[] = {23.5, 24.1, 24.5};
   int      Ch774_LimVsFreq_HDET[] =  {131, 136, 145};
   int      Ch774_LimVsFreq_PDM[]  =  {115, 125, 135};


   QMSL_CDMA_TxLimVsFreq_struct TxLimVsFreqData;
   QMSL_CDMA_MeasuredChannels_struct TxLimVsFreqMeasuredChannels;

   // Copy the measurement data to QMSL structures
   for(i = 0; i < 3; i++)
   {
      TxLimVsFreqData.aiTxMeasuredHDET[0][i] = Ch1013_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[0][i] = Ch1013_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[0][i] = Ch1013_LimVsFreq_PDM[i];
      TxLimVsFreqData.iNumOfTxLimMeasurement[0] = 3;

      TxLimVsFreqData.aiTxMeasuredHDET[1][i] = Ch358_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[1][i] = Ch358_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[1][i] = Ch358_LimVsFreq_PDM[i];
      TxLimVsFreqData.iNumOfTxLimMeasurement[1] = 3;

      TxLimVsFreqData.aiTxMeasuredHDET[2][i] = Ch774_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[2][i] = Ch774_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[2][i] = Ch774_LimVsFreq_PDM[i];
      TxLimVsFreqData.iNumOfTxLimMeasurement[2] = 3;
   }

   // Note that the reference channels measured data needs to be sorted from high to lower power
   for(i = 0; i < 3; i++)
   {
      TxLimVsFreqData.oRefChMeas.aiTxAGC[i] = Ch358_LimVsFreq_PDM[2 - i];
      TxLimVsFreqData.oRefChMeas.aiTxMeasuredHDET[i] = Ch358_LimVsFreq_HDET[2 - i];
      TxLimVsFreqData.oRefChMeas.aiTxMeasuredPower[i] = Ch358_LimVsFreq_Power[2- i];   
      TxLimVsFreqData.oRefChMeas.iNumMeasurement = 3;

   }
   TxLimVsFreqMeasuredChannels.aiMeasuredChannels[0] = 1013;
   TxLimVsFreqMeasuredChannels.aiMeasuredChannels[1] = 358;
   TxLimVsFreqMeasuredChannels.aiMeasuredChannels[2] = 774;

   TxLimVsFreqMeasuredChannels.iRefChannel = 358;
   TxLimVsFreqMeasuredChannels.iNumMeasuredChannels = 3;



   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_Tx_LimVsFreq_NV_struct Tx_LimVsFreq_NV;

   QLIB_RFCAL_CDMA_Tx_LimVsFreq_Results(   g_hResourceContext, 
      NV_MODE_BC0,
      24,      //desired max power
      (void*) &TxLimVsFreqData,
      (void*) &TxLimVsFreqMeasuredChannels,
      (void*) &NV_Channels,
      (void*) &Tx_LimVsFreq_NV,
      1);


   //****
   //Intelliceiver Item
   //****
   QMSL_CDMA_Intelliceiver_Value iIntelliceiver;
   iIntelliceiver.iSizeOfIntelliceiverValue = 128;
   memset(iIntelliceiver.aiIntelliceiverValue, 78, 128);
   QMSL_RFCAL_NV_Item oNVItem;
   QLIB_RFCAL_Rx_Intelliceiver_Cal_Results( g_hResourceContext, NV_MODE_BC0, (void*)&iIntelliceiver, (void*)&oNVItem,1); 


   //****
   //IM2
   //****
   QMSL_CDMA_RxIM2_Measurement_struct aIM2;

   aIM2.iNumMeasuredPowerModes = 3;

   aIM2.aiIM2_I[0] = 4;
   aIM2.aiIM2_I[1] = 5;
   aIM2.aiIM2_I[2] = 6;

   aIM2.aiIM2_Q[0] = 0;
   aIM2.aiIM2_Q[1] = 1;
   aIM2.aiIM2_Q[2] = 2;

   aIM2.aiIM2_G[0] = 1;
   aIM2.aiIM2_G[1] = 1;
   aIM2.aiIM2_G[2] = 1;

   // Primary Rx
   aIM2.iPath = 0;

   QMSL_CDMA_RxIM2_NV_struct aIM2_NV;
   QLIB_RFCAL_Rx_IM2_Results(g_hResourceContext, NV_MODE_BC0, (void*) &aIM2, (void*) &aIM2_NV,1);

   // Secondary Rx
   aIM2.iPath = 1;
   aIM2.iNumMeasuredPowerModes = 1;

   QLIB_RFCAL_Rx_IM2_Results(g_hResourceContext, NV_MODE_BC0, (void*) &aIM2, (void*) &aIM2_NV,1);


   // Store the calibration channel
   QMSL_RFCAL_NV_Item _CalChan;

   // BC0_Channels holds the RF calibration channels
   // This functions generate the NV items that stores RF Calibration channel
   QLIB_RFCAL_CAL_CHAN( g_hResourceContext, NV_MODE_BC0, QMSL_TX_CAL_CHANNEL, BC0_Channel, 16, &_CalChan, 1); 

   QLIB_RFCAL_CAL_CHAN( g_hResourceContext, NV_MODE_BC0, QMSL_RX_CAL_CHANNEL, BC0_Channel, 16, &_CalChan, 1); 

   QLIB_RFCAL_CAL_CHAN( g_hResourceContext, NV_MODE_BC0, QMSL_RX1_CAL_CHANNEL, BC0_Channel, 16, &_CalChan, 1); 


   return 1;
}


// CDMA RTR6500 Platform
unsigned char CDMA_RTR6500_Calibration( void )
{
   // DVGA LNA gain offset, offset vs freq

   QMSL_CDMA_RxFreq_Measurement_struct aRX_Measured;
   QMSL_CDMA_MeasuredChannels_struct RX_MeasuredChannels;
   QMSL_CDMA_NVChannels_struct NV_Channels;



   //Measurement Setup Rx Parameters
   unsigned short DVGA_PowerMode0[16] = {2229, 2388, 2564, 2200, 2200, 2387, 2365,2218, 2460, 2547, 2046, 2047, 2129, 2600, 2345, 2214};
   unsigned short LNA0_PowerMode0[16] = {106, 103, 113, 121, 123, 122, 120,111, 118,111, 116, 109, 110, 115, 116, 118};
   unsigned short LNA1_PowerMode0[16] = {134, 134, 134, 134, 134, 134, 134,134, 134,134, 134, 134, 134, 134, 134, 134};
   unsigned short LNA2_PowerMode0[16] = {233, 234, 238, 243, 244, 238, 234,232, 229,228, 222, 219, 225, 230, 232, 234};
   unsigned short LNA3_PowerMode0[16] = {333, 334, 338, 343, 344, 338, 334,333, 339,338, 333, 319, 335, 330, 333, 334};


   unsigned short DVGA_PowerMode1[16] = {2229, 2388, 2564, 2200, 2200, 2387, 2365,2218, 2460, 2547, 2046, 2047, 2129, 2600, 2345, 2214};
   unsigned short LNA0_PowerMode1[16] = {106, 103, 113, 121, 123, 122, 120,111, 118,111, 116, 109, 110, 115, 116, 118};
   unsigned short LNA1_PowerMode1[16] = {134, 134, 134, 134, 134, 134, 134,134, 134,134, 134, 134, 134, 134, 134, 134};
   unsigned short LNA2_PowerMode1[16] = {233, 234, 238, 243, 244, 238, 234,232, 229,228, 222, 219, 225, 230, 232, 234};
   unsigned short LNA3_PowerMode1[16] = {333, 334, 338, 343, 344, 338, 334,333, 339,338, 333, 319, 335, 330, 333, 334};   

   // Copy measurement data to QMSL's structure
   unsigned short BC0_Channel[16] = {1018,46,98,150,202,254,306,358,410,462,514,566,618,670,722,774};

   int _ch;
   for(_ch = 0; _ch < 16; _ch++)
   {
      aRX_Measured.aiDVGA_Measured[_ch][0]   = DVGA_PowerMode0[_ch];
      aRX_Measured.aiLNA_Measured[_ch][0][0] = LNA0_PowerMode0[_ch];
      aRX_Measured.aiLNA_Measured[_ch][1][0] = LNA1_PowerMode0[_ch];
      aRX_Measured.aiLNA_Measured[_ch][2][0] = LNA2_PowerMode0[_ch];
      aRX_Measured.aiLNA_Measured[_ch][3][0] = LNA3_PowerMode0[_ch];

      aRX_Measured.aiDVGA_Measured[_ch][1]   = DVGA_PowerMode1[_ch];
      aRX_Measured.aiLNA_Measured[_ch][0][1] = LNA0_PowerMode1[_ch];
      aRX_Measured.aiLNA_Measured[_ch][1][1] = LNA1_PowerMode1[_ch];
      aRX_Measured.aiLNA_Measured[_ch][2][1] = LNA2_PowerMode1[_ch];
      aRX_Measured.aiLNA_Measured[_ch][3][1] = LNA3_PowerMode1[_ch];

   }

   // Number of measured LNA states
   aRX_Measured.iNumMeasured_LNA_States = 4;

   /*
   aiLNA_States[n] is the LNA state which the measurment data aiLNA_Measured[n] belongs to.  The highest gain state is 0.
   In this case, the LNA NV will be mapped to LNA_1, LNA_2, LNA_3 and LNA_4 offset

   */
   aRX_Measured.aiLNA_States[0] =  0;
   aRX_Measured.aiLNA_States[1] =  1;
   aRX_Measured.aiLNA_States[2] =  2;
   aRX_Measured.aiLNA_States[3] =  3;

   // Number of pwoer mode
   aRX_Measured.iNumMeasuredPowerModes = 2;

   // Primary Rx
   aRX_Measured.iPath = 0;

   // Fill out the channels which the measurement are made
   for(_ch = 0; _ch < 16; _ch++)
      RX_MeasuredChannels.aiMeasuredChannels[_ch] = BC0_Channel[_ch];
   RX_MeasuredChannels.iNumMeasuredChannels = 16;
   RX_MeasuredChannels.iRefChannel = 358;


   // The channels 
   for(_ch = 0; _ch < 16; _ch++)
   {
      NV_Channels.aiNVChannels[_ch] = BC0_Channel[_ch];
   }      

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_Rx_DVGA_LNA_Cal_NV_Result RX_NV_Result;


   QLIB_RFCAL_CDMA_RxCal_Results(   g_hResourceContext, 
      NV_MODE_BC0,
      1, 
      12,
      (void*)&aRX_Measured, 
      (void*)&RX_MeasuredChannels, 
      (void*)&NV_Channels, 
      (void*) &RX_NV_Result,
      1,         //Write NV cal channel
      1); 


   // Secondary Rx.  Call this function again for diversity path NV

   aRX_Measured.iPath = 1;
   QLIB_RFCAL_CDMA_RxCal_Results(   g_hResourceContext, 
      NV_MODE_BC0,
      1, 
      12,
      (void*)&aRX_Measured, 
      (void*)&RX_MeasuredChannels, 
      (void*)&NV_Channels, 
      (void*) &RX_NV_Result,
      1, //Write NV cal channel
      1); 


   //*****
   //Tx Linearizer Vs Frequency (TX_COMP)
   //*****
   QMSL_CDMA_TxLinVsFreq_struct TxLinVsFreq;
   QMSL_CDMA_MeasuredChannels_struct TxMeasuredChs;


   // Measurement Setup Tx Comp Parameters
   // The power levels across frequencies for the same PDM
   int      TxFreqComCh [16] = {1013, 46, 98, 150, 202, 254, 306, 358, 410, 462, 514, 566, 618, 670, 722, 774};
   double   Tx0_FreqComp [16] = {13.05, 13.29, 13.62, 13.70, 13.51, 13.11, 12.68, 12.55, 12.51, 12.73, 13.30, 13.78, 14.01, 13.85, 13.46, 12.82};
   double   Tx1_FreqComp [16] = {25.01, 24.93, 24.83, 24.55, 24.27, 24.00, 23.78, 23.96, 24.02, 24.30, 24.67, 24.82, 24.71, 24.36, 23.94, 23.48};

   // Fill out the measurment channels 
   int i;
   for(i = 0 ;i < 16; i++)
   {
      TxMeasuredChs.aiMeasuredChannels [i] = TxFreqComCh[i];
   }
   TxMeasuredChs.iRefChannel = 358;
   TxMeasuredChs.iNumMeasuredChannels = 16;

   // Copy measurement data to QMSL's structure
   for(i = 0; i < 16; i++)
   {
      TxLinVsFreq.aiTxMeasuredPower[0][i] = Tx0_FreqComp[i];
      TxLinVsFreq.aiTxMeasuredPower[1][i] = Tx1_FreqComp[i];
   }

   TxLinVsFreq.iNumMeasuredTxGainStates = 2;

   TxLinVsFreq.iTxCompType = 0; // Primary Freq comp only

   TxLinVsFreq.aiTxMeasToNVMapping[0] = 0;
   TxLinVsFreq.aiTxMeasToNVMapping[1] = 1;

   // overwrite static component with zero
   TxLinVsFreq.bC2K_Not_Overwrite_Tx_Pwr_Comp_Static_Component_With_Zero = 0;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_Tx_LinVsFreq_Cal_NV_Result_struct TxLinVsFreq_NV;   

   QLIB_RFCAL_CDMA_Tx_LinVsFreq_Results   (g_hResourceContext, 
      NV_MODE_BC0, 
      (void*)&TxLinVsFreq, 
      (void*)&TxMeasuredChs, 
      (void*)&NV_Channels,
      10,
      QMSL_NV_TX_PWR_COMP,
      (void*)&TxLinVsFreq_NV,
      1,  //Write NV cal channel
      1);




   //****
   //Tx Linearizer 
   //****

   // Measurement Setup Tx Lin Parameters

   double Tx0_LinPower[11] = {6.4, 3.0, 0.0, -7.0, -10.9, -15.0, -20.0, -25.0, -30, -40, -58}; //dBm
   int      Tx0_LinPDM[11]  = {116, 124, 132, 140, 148, 156, 164, 172, 180, 188, 196}; // PDM

   double Tx1_LinPower[12] = {24.05, 22.15, 20.19, 18.19, 16.13, 14.04, 11.90, 9.77, 7.60, 5.40, 3.22, 0.99};  //dBm
   int      Tx1_LinPDM[12]  = {116, 124, 132, 140, 148, 156, 164, 172, 180, 188, 196, 204}; // PDM
   int      Tx1_HDET[12] = {137, 113, 92, 75, 60, 48, 37, 30, 23, 17, 13, 9}; //HDET

   QMSL_CDMA_TxLin_struct TxLinData;


   //Copy measurement setup data to QMSL structures
   for(i = 0; i < 11; i++)
   {
      TxLinData.aiTxLinPDM[0][i]      = Tx0_LinPDM[i];
      TxLinData.aiTxLinPower[0][i]   = Tx0_LinPower[i];
   }
   TxLinData.iNumTxPowerMeasurementsPerRange[0] = 11;

   for(i = 0; i < 12; i++)
   {
      TxLinData.aiTxLinPDM[1][i]      = Tx1_LinPDM[i];
      TxLinData.aiTxLinPower[1][i]   = Tx1_LinPower[i];
      TxLinData.aiTxHDET[i] = Tx1_HDET[i];
   }
   TxLinData.iNumTxPowerMeasurementsPerRange[1] = 12;

   // Number of gain state
   TxLinData.iNumMeasuredTxGainStates  = 2;

   TxLinData.HDET_OffsetPower_dBm = 18;
   TxLinData.HDET_SpanPower_dBm = 26;


   // aiTxMeasToNVMapping[n] is the Tx Gain state which .aiTxLinPower[1] are made
   TxLinData.aiTxMeasToNVMapping[0] = 0;
   TxLinData.aiTxMeasToNVMapping[1] = 1;

   //TxLinData.aiTxLinPower[1] is used to generate HDET vs AGC table
   TxLinData.iTxMeasIndexUsedForHDET = 1;

   // Not using LPM HDET
   TxLinData.aiTxHDETLPMEnabled = 0;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_TxLin_RTR6500_Result_struct TxLinNV;

   QLIB_RFCAL_CDMA_TxLinCal_RTR6500_Results(g_hResourceContext, NV_MODE_BC0, (void*)&TxLinData, (void*)&TxLinNV,1);


   //****
   //Tx LIM vs Freq
   //****
   // Measurement data for Tx Lim Vs Freq
   double Ch1013_LimVsFreq_Power[] = {23.5, 24.0, 24.5};
   int      Ch1013_LimVsFreq_HDET[] =  {137, 144, 149};
   int      Ch1013_LimVsFreq_PDM[]  =  {105, 115, 120};

   double Ch358_LimVsFreq_Power[] = {23.5, 24, 24.5};
   int      Ch358_LimVsFreq_HDET[] =  {128, 136, 141};
   int      Ch358_LimVsFreq_PDM[]  =  {110, 120, 130};

   double Ch774_LimVsFreq_Power[] = {23.5, 24, 24.5};
   int      Ch774_LimVsFreq_HDET[] =  {131, 136, 145};
   int      Ch774_LimVsFreq_PDM[]  =  {115, 125, 135};


   QMSL_CDMA_TxLimVsFreq_struct TxLimVsFreqData;
   QMSL_CDMA_MeasuredChannels_struct TxLimVsFreqMeasuredChannels;

   // Copy the measurement data to QMSL structures
   for(i = 0; i < 3; i++)
   {
      TxLimVsFreqData.aiTxMeasuredHDET[0][i] = Ch1013_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[0][i] = Ch1013_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[0][i] = Ch1013_LimVsFreq_PDM[i];
      TxLimVsFreqData.iNumOfTxLimMeasurement[0] = 3;

      TxLimVsFreqData.aiTxMeasuredHDET[1][i] = Ch358_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[1][i] = Ch358_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[1][i] = Ch358_LimVsFreq_PDM[i];
      TxLimVsFreqData.iNumOfTxLimMeasurement[1] = 3;

      TxLimVsFreqData.aiTxMeasuredHDET[2][i] = Ch774_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[2][i] = Ch774_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[2][i] = Ch774_LimVsFreq_PDM[i];
      TxLimVsFreqData.iNumOfTxLimMeasurement[2] = 3;
   }

   TxLimVsFreqMeasuredChannels.aiMeasuredChannels[0] = 1013;
   TxLimVsFreqMeasuredChannels.aiMeasuredChannels[1] = 358;
   TxLimVsFreqMeasuredChannels.aiMeasuredChannels[2] = 774;

   TxLimVsFreqMeasuredChannels.iRefChannel = 358;
   TxLimVsFreqMeasuredChannels.iNumMeasuredChannels = 3;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_Tx_LimVsFreq_NV_struct Tx_LimVsFreq_NV;

   QLIB_RFCAL_CDMA_Tx_LimVsFreq_Results(   g_hResourceContext, 
      NV_MODE_BC0,
      24,      //desired max power
      (void*) &TxLimVsFreqData,
      (void*) &TxLimVsFreqMeasuredChannels,
      (void*) &NV_Channels,
      (void*) &Tx_LimVsFreq_NV,
      1);


   //****
   //Intelliceiver Item
   //****
   QMSL_CDMA_Intelliceiver_Value iIntelliceiver;
   iIntelliceiver.iSizeOfIntelliceiverValue = 128;
   memset(iIntelliceiver.aiIntelliceiverValue, 78, 128);
   QMSL_RFCAL_NV_Item oNVItem;
   QLIB_RFCAL_Rx_Intelliceiver_Cal_Results( g_hResourceContext, NV_MODE_BC0, (void*)&iIntelliceiver, (void*)&oNVItem,1); 


   // Store the calibration channel
   QMSL_RFCAL_NV_Item _CalChan;

   // BC0_Channels holds the RF calibration channels
   // This functions generate the NV items that stores RF Calibration channel
   QLIB_RFCAL_CAL_CHAN( g_hResourceContext, NV_MODE_BC0, QMSL_TX_CAL_CHANNEL, BC0_Channel, 16, &_CalChan, 1); 

   QLIB_RFCAL_CAL_CHAN( g_hResourceContext, NV_MODE_BC0, QMSL_RX_CAL_CHANNEL, BC0_Channel, 16, &_CalChan, 1); 

   QLIB_RFCAL_CAL_CHAN( g_hResourceContext, NV_MODE_BC0, QMSL_RX1_CAL_CHANNEL, BC0_Channel, 16, &_CalChan, 1); 


   return 1;
}





// 2nd Tx Freq Comp (RTR6285)
unsigned char UMTS_RTR6285_Tx2ndFreqComp( void )
{

   QMSL_CDMA_TxLinVsFreq_struct TxLinVsFreq;
   QMSL_CDMA_MeasuredChannels_struct TxMeasuredChs;

   int      TxFreqComCh [16] = {9621,9638,9656,9673,9691,9708,9726,9743,9761,9778,9796,9813,9831,9848,9866,9883}; // = {1013, 46, 98, 150, 202, 254, 306, 358, 410, 462, 514, 566, 618, 670, 722, 774};

   double   Tx0_FreqComp [16] = {6.05, 6.29, 6.62, 6.70, 6.51, 6.11, 6.68, 6.55, 6.51, 6.73, 6.30, 6.78, 7.01, 6.85, 6.46, 6.82};
   double   Tx1_FreqComp [16] = {19.01, 18.93, 18.83, 18.55, 18.27, 18.00, 17.78, 17.96, 18.02, 18.30, 18.67, 18.82, 18.71, 18.36, 17.94, 17.48};

   double   aiTx_sec_comp_meas [16] =      {7.05, 7.29, 7.62, 7.70, 7.51, 7.11, 6.68, 6.55, 6.51, 6.73, 7.30, 7.78, 8.01, 7.85, 7.86, 7.82};
   double   aiTx_sec_comp_meas_next [16] =   {7.26, 7.73, 7.93, 7.90, 7.87, 7.46, 6.81, 6.96, 7.02, 7.30, 7.67, 7.82, 8.55, 8.36, 8.22, 8.42};

   int i = 0;

   for(i = 0 ;i < 16; i++)
   {
      TxMeasuredChs.aiMeasuredChannels [i] = TxFreqComCh[i];
   }

   TxMeasuredChs.iRefChannel = 9761;
   TxMeasuredChs.iNumMeasuredChannels = 16;

   for(i = 0; i < 16; i++)
   {
      TxLinVsFreq.aiTxMeasuredPower[0][i] = Tx0_FreqComp[i];
      TxLinVsFreq.aiTxMeasuredPower[1][i] = Tx1_FreqComp[i];

      TxLinVsFreq.aiTx_sec_comp_meas [0][i] = aiTx_sec_comp_meas[i];
      TxLinVsFreq.aiTx_sec_comp_meas_next [0][i] = aiTx_sec_comp_meas_next[i];

   }
   TxLinVsFreq.iNumMeasuredTxGainStates = 2;
   TxLinVsFreq.aiTxMeasToNVMapping[0] = 0;  // Array[0] is gain state 0
   TxLinVsFreq.aiTxMeasToNVMapping[1] = 1;  // Array[1] is gate stat 0
   TxLinVsFreq.iTxCompType = 1;        // Derive both primary and secondary Tx Freq comp items


   TxLinVsFreq.a2ndTxCompHasMeas[ 0 ] = 1;  // Gain state 0 has 2nd Freq comp
   TxLinVsFreq.a2ndTxCompHasMeas[ 1 ] = 0;  // Gain state 1 doesn't have 2nd Freq comp

   //! The PDM value used to generate secondary Tx comp table.  Used by WCDMA Secondary Tx Comp vs Freq for RTR6285/RTR6280 (80-V4341-84)  
   TxLinVsFreq.a2ndTxCompPDM[ 0 ] = 180;
   TxLinVsFreq.a2ndTxCompPDM[ 1 ] = 0;


   QMSL_CDMA_Tx_LinVsFreq_Cal_NV_Result_struct TxLinVsFreq_NV;

   unsigned short Channel[16] = {9621,9638,9656,9673,9691,9708,9726,9743,9761,9778,9796,9813,9831,9848,9866,9883};

   QLIB_RFCAL_CDMA_Tx_LinVsFreq_Results   (g_hResourceContext, 
      NV_MODE_WCDMA_IMT, 
      (void*)&TxLinVsFreq, 
      (void*)&TxMeasuredChs,
      (void*)&Channel,
      12,
      QMSL_NV_TX_COMP,
      (void*)&TxLinVsFreq_NV,
      0,
      1);


   return 1;

}

// CDMA target beta scaling calibration (SC2X)
unsigned char CDMA2000_TxBetaScaling( void )
{

   // Measurement Data for Tx Linearizer
   double Tx0_Power[5] = {-10, -21.4, -29.5, -39.5, -45.0};
   int      Tx0_PDM[5] =   {200, 250, 300, 350, 400};

   double Tx1_Power[5] = {23.2, 20.5, 17.3, 14.0, 12.2};
   int      Tx1_PDM[5]   = {100, 120, 140, 160, 180};
   int      Tx1_HDET[5]  = {178, 158, 138, 118, 98};



   QMSL_CDMA_TxLin_BetaScaling_struct TxLinData;

   // Copy the measurement data to QMSL structure
   int i = 0;
   for(i = 0; i < 5; i++)
   {
      TxLinData.aiTxLinPower[0][i] = Tx0_Power[i];
      TxLinData.aiTxLinPDM[0][i] = Tx0_PDM[i];

      TxLinData.aiTxLinPower[1][i] = Tx1_Power[i];
      TxLinData.aiTxLinPDM[1][i] = Tx1_PDM[i];
      TxLinData.aiTxHDET[i] = Tx1_HDET[i];
   }
   TxLinData.iNumTxPowerMeasurementsPerGainState[0] = 5;
   TxLinData.iNumTxPowerMeasurementsPerGainState[1] = 5;


   // The max power for each gain state
   TxLinData.adMaxPower[0] = 15;
   TxLinData.adMaxPower[1] = 24;

   // Extrapolate for max power
   TxLinData.aiExtrapolatedMaxPower[0] = 1;
   TxLinData.aiExtrapolatedMaxPower[1] = 1;

   // The min power for each gain state
   TxLinData.adMinPower[0] = -50;
   TxLinData.adMinPower[1] = 10;

   // Extrapolate for min power
   TxLinData.aiExtrapolatedMinPower[0] = 1;
   TxLinData.aiExtrapolatedMinPower[1] = 1;


   TxLinData.aiTxMeasToNVMapping[0] = 0;
   TxLinData.aiTxMeasToNVMapping[1] = 1;

   //TxLinData.aiTxLinPower[1] is used to generate HDET vs AGC table
   TxLinData.iTxMeasIndexUsedForHDET = 1;

   TxLinData.aiTxSweepStepSize[0] = 4;
   TxLinData.aiTxSweepStepSize[1] = 3;
   TxLinData.iNumMeasuredTxGainStates = 2; 


   TxLinData.iTotalNVItems = 48;

   TxLinData.HDET_OffsetPower_dBm = 18;
   TxLinData.HDET_SpanPower_dBm  =  26;

   TxLinData.iNV_Resolution = 10;

   // Sort by power
   TxLinData.iSortingOrder = 0;

   // decending 
   TxLinData.iSortNVTxLin = 0;

   // It is not a V2 HDET
   TxLinData.iUse_HDET_V2 = 0;

   // Maximum HDET value 
   TxLinData.iHDET_MaxLim = 255;

   // Not using LPM HDET
   TxLinData.aiTxHDETLPMEnabled = 0;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_TxLin_BetaScaling_Result_struct aTx_Result;   

   QLIB_RFCAL_CDMA_TxLinCal_BetaScaling_Results(g_hResourceContext, NV_MODE_BC0 , &TxLinData, &aTx_Result,1);

   return 1;
}


// QSC60x0, MSM60X0, or any CDMA platforms that uses NV_CDMA_xxx or NV_PCS_xxx
unsigned char QSC60X0_MSM60X0(void)
{

   // Measurement data for Rx DVGA/LNA
   unsigned short DVGA[16] = {2229, 2388, 2564, 2200, 2200, 2387, 2365,2218, 2460, 2547, 2046, 2047, 2129, 2600, 2345, 2214};
   unsigned short LNA0[16] = {106, 103, 113, 121, 123, 122, 120,111, 118,111, 116, 109, 110, 115, 116, 118};
   unsigned short LNA1[16] = {134, 134, 134, 134, 134, 134, 134,134, 134,134, 134, 134, 134, 134, 134, 134};
   unsigned short LNA2[16] = {233, 234, 238, 243, 244, 238, 234,232, 229,228, 222, 219, 225, 230, 232, 234};
   unsigned short LNA3[16] = {433, 434, 438, 443, 444, 438, 434,432, 429,428, 422, 419, 425, 430, 432, 434};

   // DVGA LNA gain offset, offset vs freq
   QMSL_CDMA_RxFreq_Measurement_struct aRX_Measured;
   QMSL_CDMA_MeasuredChannels_struct RX_MeasuredChannels;
   QMSL_CDMA_NVChannels_struct NV_Channels;
   unsigned short BC0_Channel[16] = {1018,46,98,150,202,254,306,358,410,462,514,566,618,670,722,774};

   // Copy measurement data to QMSL struture
   int _ch;
   for(_ch = 0; _ch < 16; _ch++)
   {
      aRX_Measured.aiDVGA_Measured[_ch][0]   = DVGA[_ch];
      aRX_Measured.aiLNA_Measured[_ch][0][0] = LNA0[_ch];
      aRX_Measured.aiLNA_Measured[_ch][1][0] = LNA1[_ch];
      aRX_Measured.aiLNA_Measured[_ch][2][0] = LNA2[_ch];
      aRX_Measured.aiLNA_Measured[_ch][2][0] = LNA3[_ch];   

   }

   // Number of measured LNA states
   aRX_Measured.iNumMeasured_LNA_States = 4;

   /*
   aiLNA_States[n] is the LNA state which the measurment data aiLNA_Measured[n] belongs to.  The highest gain state is 0.
   */
   aRX_Measured.aiLNA_States[0] =  0;
   aRX_Measured.aiLNA_States[1] =  1;
   aRX_Measured.aiLNA_States[2] =  2;
   aRX_Measured.aiLNA_States[2] =  3;

   // Number of pwoer mode
   aRX_Measured.iNumMeasuredPowerModes = 1;

   // Primary Rx
   aRX_Measured.iPath = 0;

   // Fill out the channels which the measurement are made

   for(_ch = 0; _ch < 16; _ch++)
      RX_MeasuredChannels.aiMeasuredChannels[_ch] = BC0_Channel[_ch];
   RX_MeasuredChannels.iNumMeasuredChannels = 16;
   RX_MeasuredChannels.iRefChannel = 358;


   // The NV channels 
   for(_ch = 0; _ch < 16; _ch++)
   {
      NV_Channels.aiNVChannels[_ch] = BC0_Channel[_ch];
   }      

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_Rx_DVGA_LNA_Cal_NV_Result RX_NV_Result;


   QLIB_RFCAL_CDMA_RxCal_Results(   g_hResourceContext, 
      NV_MODE_CDMA_800,
      1, 
      12,
      (void*)&aRX_Measured, 
      (void*)&RX_MeasuredChannels, 
      (void*)&NV_Channels, 
      (void*) &RX_NV_Result,
      0,   //Don't Write NV cal channel
      1); 



   //*****
   //Tx Linearizer Vs Frequency (TX_COMP)
   //*****

   // Measurement data for Tx Comp
   // the power levels across frequencies for the same PDM
   int      TxFreqComCh [16] = {1013, 46, 98, 150, 202, 254, 306, 358, 410, 462, 514, 566, 618, 670, 722, 774};
   double   Tx0_FreqComp [16] = {13.05, 13.29, 13.62, 13.70, 13.51, 13.11, 12.68, 12.55, 12.51, 12.73, 13.30, 13.78, 14.01, 13.85, 13.46, 12.82};
   double   Tx1_FreqComp [16] = {25.01, 24.93, 24.83, 24.55, 24.27, 24.00, 23.78, 23.96, 24.02, 24.30, 24.67, 24.82, 24.71, 24.36, 23.94, 23.48};


   QMSL_CDMA_TxLinVsFreq_struct TxLinVsFreq;
   QMSL_CDMA_MeasuredChannels_struct TxMeasuredChs;

   // Copy the measurement data to QMSL structure
   int i;
   for(i = 0; i < 16; i++)
   {
      TxLinVsFreq.aiTxMeasuredPower[0][i] = Tx0_FreqComp[i];
      TxLinVsFreq.aiTxMeasuredPower[1][i] = Tx1_FreqComp[i];
   }

   // Copy the measurement channels to QMSL structure
   for(i = 0 ;i < 16; i++)
   {
      TxMeasuredChs.aiMeasuredChannels [i] = TxFreqComCh[i];
   }
   TxMeasuredChs.iRefChannel = 358;
   TxMeasuredChs.iNumMeasuredChannels = 16;


   TxLinVsFreq.iNumMeasuredTxGainStates = 2;

   TxLinVsFreq.aiTxMeasToNVMapping[0] = 0;
   TxLinVsFreq.aiTxMeasToNVMapping[1] = 1;

   // overwrite static component with zero
   TxLinVsFreq.bC2K_Not_Overwrite_Tx_Pwr_Comp_Static_Component_With_Zero = 0;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_Tx_LinVsFreq_Cal_NV_Result_struct TxLinVsFreq_NV;   

   QLIB_RFCAL_CDMA_Tx_LinVsFreq_Results   (g_hResourceContext, 
      NV_MODE_CDMA_800, 
      (void*)&TxLinVsFreq, 
      (void*)&TxMeasuredChs, 
      (void*)&NV_Channels,
      10,
      QMSL_NV_TX_COMP,
      (void*)&TxLinVsFreq_NV,
      0, //Don't Write NV cal channel
      1);




   //****
   //Tx Linearizer (LIN_MASTER)
   //****
   //   Measurement setup data for Tx Linearizer
   double Tx0_LinPower[11] = {15, 13, 10, 7, 4, 0, -4, -8, -12, -15, -20}; //dBm
   int      Tx0_LinPDM[11]  = {100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300}; // PDM

   double Tx1_LinPower[12] = {24.05, 22.15, 20.19, 18.19, 16.13, 14.04, 11.90, 9.77, 7.60, 5.40, 3.22, 0.99};  //dBm
   int      Tx1_LinPDM[12]  = {89, 99, 109, 119, 129, 139, 149, 159, 169, 179, 189, 199}; //PDM
   int      Tx1_HDET[12] = {137, 113, 92, 75, 60, 48, 37, 30, 23, 17, 13, 9}; //HDET

   QMSL_CDMA_TxLin_struct TxLinData;


   // Copy the measurement data to QMSL structure
   for(i = 0; i < 11; i++)
   {
      TxLinData.aiTxLinPDM[0][i]      = Tx0_LinPDM[i];
      TxLinData.aiTxLinPower[0][i]   = Tx0_LinPower[i];
   }
   TxLinData.iNumTxPowerMeasurementsPerRange[0] = 11;
   for(i = 0; i < 12; i++)
   {
      TxLinData.aiTxLinPDM[1][i]      = Tx1_LinPDM[i];
      TxLinData.aiTxLinPower[1][i]   = Tx1_LinPower[i];
      TxLinData.aiTxHDET[i] = Tx1_HDET[i];
   }
   TxLinData.iNumTxPowerMeasurementsPerRange[1] = 12;

   // Number of gain state
   TxLinData.iNumMeasuredTxGainStates  = 2;

   TxLinData.HDET_OffsetPower_dBm = 18;
   TxLinData.HDET_SpanPower_dBm = 26;

   TxLinData.aiTxMeasToNVMapping[0] = 0;
   TxLinData.aiTxMeasToNVMapping[1] = 1;

   //TxLinData.aiTxLinPower[1] is used to generate HDET vs AGC table
   TxLinData.iTxMeasIndexUsedForHDET = 1;

   // Not using LPM HDET
   TxLinData.aiTxHDETLPMEnabled = 0;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_TxLin_Cal_Result_struct TxLinNV;

   QLIB_RFCAL_CDMA_Tx_LinCal_Results(g_hResourceContext, NV_MODE_CDMA_800, (void*)&TxLinData, (void*)&TxLinNV,1);


   //****
   //Tx LIM vs Freq
   //****
   // Measurement setup data for Tx Lim vs freq
   double Ch1013_LimVsFreq_Power[] = {23.5, 24.0, 24.5};
   int      Ch1013_LimVsFreq_HDET[] =  {137, 144, 149};
   int      Ch1013_LimVsFreq_PDM[]  =  {105, 115, 120};

   double Ch358_LimVsFreq_Power[] = {23.5, 24, 24.5};
   int      Ch358_LimVsFreq_HDET[] =  {128, 136, 141};
   int      Ch358_LimVsFreq_PDM[]  =  {110, 120, 130};

   double Ch774_LimVsFreq_Power[] = {23.5, 24, 24.5};
   int      Ch774_LimVsFreq_HDET[] =  {131, 136, 145};
   int      Ch774_LimVsFreq_PDM[]  =  {115, 125, 135};


   QMSL_CDMA_TxLimVsFreq_struct TxLimVsFreqData;
   QMSL_CDMA_MeasuredChannels_struct TxLimVsFreqMeasuredChannels;

   // Copy the measurement data to QMSL structures
   for(i = 0; i < 3; i++)
   {
      TxLimVsFreqData.aiTxMeasuredHDET[0][i] = Ch1013_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[0][i] = Ch1013_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[0][i] = Ch1013_LimVsFreq_PDM[i];
      TxLimVsFreqData.iNumOfTxLimMeasurement[0] = 3;

      TxLimVsFreqData.aiTxMeasuredHDET[1][i] = Ch358_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[1][i] = Ch358_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[1][i] = Ch358_LimVsFreq_PDM[i];
      TxLimVsFreqData.iNumOfTxLimMeasurement[1] = 3;

      TxLimVsFreqData.aiTxMeasuredHDET[2][i] = Ch774_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[2][i] = Ch774_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[2][i] = Ch774_LimVsFreq_PDM[i];
      TxLimVsFreqData.iNumOfTxLimMeasurement[2] = 3;
   }

   TxLimVsFreqMeasuredChannels.aiMeasuredChannels[0] = 1013;
   TxLimVsFreqMeasuredChannels.aiMeasuredChannels[1] = 358;
   TxLimVsFreqMeasuredChannels.aiMeasuredChannels[2] = 774;

   TxLimVsFreqMeasuredChannels.iRefChannel = 358;
   TxLimVsFreqMeasuredChannels.iNumMeasuredChannels = 3;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_Tx_LimVsFreq_NV_struct Tx_LimVsFreq_NV;

   QLIB_RFCAL_CDMA_Tx_LimVsFreq_Results(   g_hResourceContext, 
      NV_MODE_CDMA_800,
      24,
      (void*) &TxLimVsFreqData,
      (void*) &TxLimVsFreqMeasuredChannels,
      (void*) &NV_Channels,
      (void*) &Tx_LimVsFreq_NV,
      1);

   //****
   //IM2
   //****
   QMSL_CDMA_RxIM2_Measurement_struct aIM2;
   QMSL_CDMA_RxIM2_NV_struct aIM2_NV;
   aIM2.iNumMeasuredPowerModes = 1;

   aIM2.aiIM2_I[0] = 4;
   aIM2.aiIM2_Q[0] = 2;
   aIM2.aiIM2_G[0] = 1;

   // Primary Rx
   aIM2.iPath = 0;

   QLIB_RFCAL_Rx_IM2_Results(g_hResourceContext, NV_MODE_CDMA_800, (void*) &aIM2, (void*) &aIM2_NV,1);

   return 1;
}

// UMTS targets with RTR6285
unsigned char UMTS_RTR6285_Tx_Calibration( void )
{
   // Measurement data for Tx Linearizer
   double Tx0_LinPower[22] = {-52.73, -49.46, -46.88, -44.20, -40.93, -38.05, -35.09, -32.41, -29.21, -26.66, -24.21, -21.30, -18.45, -15.84, -13.37, -10.64, -7.76, -5.11, -2.68, 0.01 , 1.99, 4.56};
   int      Tx0_LinPDM[22]  = {134, 138, 142, 146, 150, 154, 158, 162, 166, 170, 174, 178, 182, 186, 190, 194, 198, 202, 206, 210, 214, 218};

   double Tx1_LinPower[9] = {   -15, -12, -9.0, -5.7, -3.3, 0.1, 2.7, 4.5, 7.2};
   int      Tx1_LinPDM[9]  = {196, 199, 202, 205, 208, 211, 214, 217, 218};

   double Tx3_LinPower[9] = {   0,   3.1, 6.0, 9.0, 12.2, 14.8, 18, 21, 24.2};
   int      Tx3_LinPDM[9]  = {196, 199, 202, 205, 208, 211, 214, 217, 218};
   int    Tx3_HDET[9] = { 86, 90, 94, 98, 102, 106, 110, 114, 118};

   QMSL_CDMA_TxLin_BetaScaling_struct TxLinData;


   memset(&TxLinData, 0, sizeof(QMSL_CDMA_TxLin_BetaScaling_struct));
   // Copy the measurement data to QMSL structrue
   // Note: The data should be in descending order (Max power first)
   int i;
   for(i = 0; i < 22; i++)
   {
      TxLinData.aiTxLinPDM[0][i]      = Tx0_LinPDM[21 - i];
      TxLinData.aiTxLinPower[0][i]   = Tx0_LinPower[21 - i];
   }
   TxLinData.iNumTxPowerMeasurementsPerGainState[0] = 22;

   for(i = 0; i < 9; i++)
   {
      TxLinData.aiTxLinPDM[1][i]      = Tx1_LinPDM[8 - i];
      TxLinData.aiTxLinPower[1][i]   = Tx1_LinPower[8 - i];
   }
   TxLinData.iNumTxPowerMeasurementsPerGainState[1] = 9;

   for(i = 0; i < 9; i++)
   {
      TxLinData.aiTxLinPDM[2][i]      = Tx3_LinPDM[8 - i];
      TxLinData.aiTxLinPower[2][i]   = Tx3_LinPower[8 - i];
      TxLinData.aiTxHDET[i]         = Tx3_HDET[8-i];
   }
   TxLinData.iNumTxPowerMeasurementsPerGainState[2] = 9;

   TxLinData.iNumMeasuredTxGainStates  = 3;
   TxLinData.HDET_OffsetPower_dBm = 18;
   TxLinData.HDET_SpanPower_dBm = 26;


   TxLinData.aiTxMeasToNVMapping[0] = 0;
   TxLinData.aiTxMeasToNVMapping[1] = 1;
   TxLinData.aiTxMeasToNVMapping[2] = 2;

   // TxLinData.aiTxLinPower[2] is used to generate HDET vs AGC table
   TxLinData.iTxMeasIndexUsedForHDET = 2;

   TxLinData.aiTxSweepStepSize[0]= 4;
   TxLinData.aiTxSweepStepSize[1]= 3;
   TxLinData.aiTxSweepStepSize[2]= 3;

   TxLinData.iNumMeasuredTxGainStates = 3;

   TxLinData.aiExtrapolation_PDM_StepSize[0] = 6;
   TxLinData.aiExtrapolation_PDM_StepSize[1] = 6;
   TxLinData.aiExtrapolation_PDM_StepSize[2] = 6;

   TxLinData.aiHeadRoomPDMCountList[0] = 255;
   TxLinData.aiHeadRoomPDMCountList[1] = 255;
   TxLinData.aiHeadRoomPDMCountList[2] = 255;

   TxLinData.iNV_Resolution = 12;

   // Sort by power
   TxLinData.iSortingOrder = 0;

   // decending 
   TxLinData.iSortNVTxLin = 0;

   // Not enchanced NV
   TxLinData.iEnh_Linearizer_0_NV = 0;

   // Not HDET V2
   TxLinData.iUse_HDET_V2 = 0;

   // Max HDET value
   TxLinData.iHDET_MaxLim = 255;

   // Min Tx PDM
   TxLinData.i_TxAGC_PDM_MinGain = 0;

   // Max Tx PDM
   TxLinData.iUMTS_TxAGC_PDM_Max = 255;

   // Extrapolate
   TxLinData.iDo_Not_Extrapolate_Lower_Powers = 0;

   // Not using LPM HDET
   TxLinData.aiTxHDETLPMEnabled = 0;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_TxLin_BetaScaling_Result_struct aTx_Result;

   QLIB_RFCAL_CDMA_TxLinCal_BetaScaling_Results(g_hResourceContext, NV_MODE_WCDMA_IMT , &TxLinData, &aTx_Result,1);

   return 1;

}
// UMTS RTR6250(3U) Platform Tx Calibration
unsigned char Tx_3U_Calibration( void )
{
   QMSL_CDMA_TxLin_struct TxLinData;


   double Tx0_LinPower[6] = {4.88, 4.55,4.10,-46.57,-49.76,-52.73};
   int      Tx0_LinPDM[6]  = {455, 445, 435, 205, 195, 185 };

   double   Tx3_LinPower[3] = {24, 23.5, 22.5};
   int      Tx3_LinPDM[3] = {460, 450, 440};
   int      Tx3_HDET[3] = {178, 156, 142};

   int i;
   for(i = 0; i < 6; i++)
   {
      TxLinData.aiTxLinPDM[0][i]      = Tx0_LinPDM[i];
      TxLinData.aiTxLinPower[0][i]   = Tx0_LinPower[i];
   }

   for(i = 0; i < 3; i++)
   {
      TxLinData.aiTxLinPDM[1][i]      = Tx3_LinPDM[i];
      TxLinData.aiTxLinPower[1][i]   = Tx3_LinPower[i];
      TxLinData.aiTxHDET[i]         = Tx3_HDET[i];
   }

   // aiTxMeasToNVMapping[n] is the Tx Gain state which aiTxLInPower[n] are made
   TxLinData.aiTxMeasToNVMapping[0] = 0;
   TxLinData.aiTxMeasToNVMapping[1] = 3;

   TxLinData.iNumMeasuredTxGainStates  = 2;

   TxLinData.HDET_OffsetPower_dBm = 18;
   TxLinData.HDET_SpanPower_dBm = 26;

   TxLinData.iNumTxPowerMeasurementsPerRange[0] = 6;
   TxLinData.iNumTxPowerMeasurementsPerRange[1] = 3;

   //TxLinData.aiTxLinPower[1] is used for generate HDET vs AGC table
   TxLinData.iTxMeasIndexUsedForHDET = 1;

   // Not using LPM HDET
   TxLinData.aiTxHDETLPMEnabled = 0;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_CDMA_TxLin_Cal_Result_struct TxLinNV;

   QLIB_RFCAL_CDMA_Tx_LinCal_Results(g_hResourceContext,  NV_MODE_WCDMA_IMT , &TxLinData, &TxLinNV, 1);

   return 1;
}

// GSM External Polar Calibration
unsigned char GSM_Tx_Polar_Calibration( void )
{

   int _iNumOfPolarCalDataPt = 300;
   // This is polar calibration sweep measurment data
   // Note that Refercnec DAC (4500) and trigger DAC (14295) measurment data are included

   // DAC value
   int      DAC[300] = { 14295,4500,1715,4500,1800,4500,1885,4500,1970,4500,2055,4500,2140,4500,2225,4500,2310,4500,2395,4500,2480,4500,2565,4500,2650,4500,2735,4500,2820,4500,2905,4500,2990,4500,3075,4500,3160,4500,3245,4500,3330,4500,3415,4500,3500,4500,3585,4500,3670,4500,3755,4500,3840,4500,3925,4500,4010,4500,4095,4500,4180,4500,4265,4500,4350,4500,4435,4500,4520,4500,4605,4500,4690,4500,4775,4500,4860,4500,4945,4500,5030,4500,5115,4500,5200,4500,5285,4500,5370,4500,5455,4500,5540,4500,5625,4500,5710,4500,5795,4500,5880,4500,5965,4500,6050,4500,6135,4500,6220,4500,6305,4500,6390,4500,6475,4500,6560,4500,6645,4500,6730,4500,6815,4500,6900,4500,6985,4500,7070,4500,7155,4500,7240,4500,7325,4500,7410,4500,7495,4500,7580,4500,7665,4500,7750,4500,7835,4500,7920,4500,8005,4500,8090,4500,8175,4500,8260,4500,8345,4500,8430,4500,8515,4500,8600,4500,8685,4500,8770,4500,8855,4500,8940,4500,9025,4500,9110,4500,9195,4500,9280,4500,9365,4500,9450,4500,9535,4500,9620,4500,9705,4500,9790,4500,9875,4500,9960,4500,10045,4500,10130,4500,10215,4500,10300,4500,10385,4500,10470,4500,10555,4500,10640,4500,10725,4500,10810,4500,10895,4500,10980,4500,11065,4500,11150,4500,11235,4500,11320,4500,11405,4500,11490,4500,11575,4500,11660,4500,11745,4500,11830,4500,11915,4500,12000,4500,12085,4500,12170,4500,12255,4500,12340,4500,12425,4500,12510,4500,12595,4500,12680,4500,12765,4500,12850,4500,12935,4500,13020,4500,13105,4500,13190,4500,13275,4500,13360,4500,13445,4500,13530,4500,13615,4500,13700,4500,13785,4500,13870,4500,13955,4500,14040,4500,14125,4500,14210,4500,14295,4500};
   // amplitude data (in dBm)
   double  AM [300] = { 32.896,17.1461,-17.4878,17.213,-17.5046,17.2291,-17.49,17.238,-17.4958,17.242,-17.5525,17.2459,-17.5901,17.2477,-18.1824,17.2499,-17.769,17.2484,-16.0333,17.2489,-13.3436,17.2518,-10.1356,17.2478,-7.10547,17.2498,-4.52939,17.2472,-2.09723,17.251,-0.0302734,17.2503,1.84207,17.2476,3.44665,17.2493,4.9451,17.2483,6.22992,17.248,7.46912,17.2462,8.52063,17.2436,9.53983,17.2446,10.4754,17.243,11.3004,17.2416,12.1057,17.2425,12.8476,17.2425,13.5155,17.2401,14.1652,17.2357,14.7551,17.2416,15.3354,17.2382,15.8636,17.238,16.3874,17.2367,16.8617,17.2325,17.3336,17.2343,17.7879,17.2313,18.2052,17.2317,18.6217,17.2303,19.0207,17.227,19.3875,17.224,19.7613,17.2249,20.1003,17.2216,20.4449,17.2167,20.7644,17.2155,21.084,17.2162,21.3845,17.2133,21.6849,17.2084,21.9818,17.2087,22.2532,17.2064,22.5345,17.204,22.8042,17.2012,23.0559,17.2012,23.3113,17.1989,23.5491,17.1952,23.7944,17.1963,24.0214,17.1918,24.2551,17.1889,24.4712,17.1878,24.6897,17.1841,24.8989,17.1813,25.1134,17.1813,25.3229,17.1769,25.5155,17.174,25.7151,17.1716,25.9111,17.1685,26.0943,17.1664,26.2816,17.161,26.458,17.1573,26.6377,17.1538,26.8105,17.1514,26.9838,17.1488,27.1473,17.142,27.3153,17.1399,27.4805,17.1354,27.6367,17.1321,27.7947,17.1277,27.9511,17.1251,28.1001,17.1219,28.2509,17.1175,28.3914,17.114,28.5401,17.1129,28.6797,17.1082,28.8222,17.105,28.9562,17.0991,29.0933,17.0929,29.2245,17.0916,29.3567,17.0887,29.4897,17.0825,29.6131,17.0815,29.7415,17.0736,29.8675,17.0698,29.9842,17.0655,30.108,17.059,30.2227,17.0529,30.3417,17.0488,30.453,17.0467,30.5711,17.042,30.6811,17.0381,30.7961,17.0333,30.9075,17.0297,31.0129,17.024,31.1201,17.019,31.2261,17.0172,31.3304,17.0109,31.4352,17.0053,31.5306,17.001,31.6332,16.9955,31.729,16.9887,31.8291,16.9832,31.9199,16.9807,32.0151,16.9783,32.1068,16.9727,32.1957,16.9667,32.2841,16.9633,32.3742,16.9574,32.4547,16.9515,32.5384,16.9455,32.6112,16.9437,32.6834,16.939,32.736,16.9348,32.7836,16.9326,32.8109,16.9308,32.8356,16.9278,32.8454,16.9224,32.8573,16.9227,32.8545,16.9177,32.8597,16.9135,32.858,
                  16.9145,32.8618,16.9124,32.8586,16.9114,32.8637,16.9082,32.8521,16.904,32.8663,16.9013,32.8618,16.9007,32.8682,16.9002,32.8558,16.8984,32.868,16.8966,32.8387,16.8968,32.868,16.8932,32.8629,16.8914,32.8656,16.8887,32.8654,16.8885,32.8714,16.8864,32.866,16.888,32.8645,16.885,32.8665,16.8853,32.8691,16.8843,32.8674,16.8833,32.8725,16.8799,32.8655,16.8781};
   // phase value (in Radian)
   double  PM [300] = { 0,-0.323958,2.85004,0.0999005,-2.99174,0.508131,-2.59665,0.916266,-2.19867,1.32354,-1.78143,1.7358,-1.34386,2.15246,-0.599882,2.56184,0.379756,2.97525,1.34434,-2.90814,2.21238,-2.49876,2.96624,-2.08525,-2.65408,-1.67501,-2.06905,-1.26831,-1.53197,-0.857303,-1.0363,-0.449265,-0.55741,-0.0389248,-0.0946274,0.366142,0.352144,0.775427,0.799396,1.18625,1.23188,1.58719,1.65938,1.99523,2.08631,2.39963,2.50729,2.80603,2.92818,-3.07324,-2.94246,-2.66548,-2.52225,-2.26185,-2.10577,-1.85621,-1.69572,-1.45498,-1.28241,-1.05452,-0.876958,-0.64849,-0.463166,-0.242752,-0.0557985,0.155603,0.356938,0.562204,0.764593,0.964395,1.178,1.36908,1.57559,1.76945,1.98468,2.17269,2.39684,2.57517,2.80134,2.97583,-3.07036,-2.8998,-2.66529,-2.49358,-2.25917,-2.09245,-1.85353,-1.68757,-1.44568,-1.28394,-1.03352,-0.883093,-0.638711,-0.482149,-0.231439,-0.086574,0.179188,0.314083,0.578694,0.723176,0.985679,1.11731,1.38423,1.52276,1.79178,1.92265,2.18928,2.31842,2.58754,2.71697,2.99078,3.11686,-2.89108,-2.76673,-2.49166,-2.3747,-2.09427,-1.97414,-1.69735,-1.57875,-1.29267,-1.17752,-0.894215,-0.778208,-0.497968,-0.391261,-0.0960655,0.0150522,0.300469,0.406601,0.700454,0.804094,1.09469,1.20245,1.49295,1.59064,1.88852,1.99571,2.2888,2.39454,2.69214,2.79232,3.08531,-3.09835,-2.79952,-2.70249,-2.40317,-2.30596,-2.0052,-1.90952,-1.60847,-1.51231,-1.21261,-1.11377,-0.816365,-0.724135,-0.420598,-0.329998,-0.0232973,0.0741104,0.373812,0.470165,0.763827,0.859988,1.16276,1.24703,1.55354,1.63925,1.94058,2.03176,2.33827,2.42666,2.72751,2.82195,3.12031,-3.08148,-2.77037,-2.68274,-2.38199,-2.29301,-1.98229,-1.89667,-1.59227,-1.50378,-1.20686,-1.12,-0.814064,-0.72308,-0.423187,-0.338339,-0.0292415,0.0554151,0.354445,0.442841,0.744748,0.829596,1.13083,1.2176,1.51835,1.60368,1.90741,1.98756,2.28966,2.38045,2.67536,2.76088,3.06288,-3.13546,-2.83106,-2.74487,-2.44497,-2.35917,-2.06014,-1.97577,-1.6799,-1.59946,-1.29784,-1.21769,-0.916937,-0.838608,-0.537085,-0.453387,-0.15522,-0.0731517,0.209772,0.289347,0.560958,0.638424,0.889421,0.954711,
                  1.18855,1.24828,1.45479,1.5079,1.68143,1.72017,1.87155,1.89159,2.01536,2.01651,2.12197,2.09609,2.17538,2.10683,2.16828,2.07107,2.12207,1.98775,2.0307,1.89811,1.92802,1.77606,1.80339,1.65277,1.67271,1.51107,1.54031,1.38049,1.39631,1.23476,1.25997,1.10341,1.11348,0.943973,0.959793,0.807545,0.823268,0.659037,0.681567,0.527018,0.539003,0.373908,0.383495,0.237863,0.224153,0.0130388,0.0333641,-0.122814,-0.109776,-0.274583,-0.256846,-0.410723,-0.399219,-0.555301,-0.535072,-0.678978,-0.662009,-0.81905,-0.808791,-0.957492,-0.941289,-1.13946,-1.1268,-1.27579,-1.26429,-1.41452,-1.38931,-1.52995,-1.51107,-1.66312};


   QMSL_GSM_Polar_Cal_Measurement aGSM_Polar_Cal_Measurement; // hold measurement data
   QMSL_GSM_Polar_Cal_Measurement aGSM_Polar_Cal_Processed;   // hold processed data 

   // Copy the measurement data to QMSL structure
   int i = 0;
   for(i = 0; i < _iNumOfPolarCalDataPt; i++)
   {
      aGSM_Polar_Cal_Measurement.aPolar_Cal_Sweep_Result[i].amp_dBm = AM[i];
      aGSM_Polar_Cal_Measurement.aPolar_Cal_Sweep_Result[i].phase_rad = PM[i];
      aGSM_Polar_Cal_Measurement.aPolar_Cal_Sweep_Result[i].dac = DAC[i];
      aGSM_Polar_Cal_Measurement.aPolar_Cal_Sweep_Result[i].duration_counts = 13; // each count is 78.77us long
   }

   aGSM_Polar_Cal_Measurement.iNumOfPoloarCalSweepRecord = _iNumOfPolarCalDataPt;


   // Process sweep data, ref DAC AM/PM data are removed, trigger DAC AM/PM data is removed
   // Equivalent of section 5.2/ProcessTxSweep in 80-V9774-11
   QLIB_RFCAL_GSM_Tx_Polar_ProcessTxSweep( g_hResourceContext,
      &aGSM_Polar_Cal_Measurement,
      &aGSM_Polar_Cal_Processed);

   // Copy the data from ProcessTxSweep to the Polar_Cal_Results structure
   QMSL_GSM_Polar_Cal_Result aGSM_Polar_Cal_Data;

   // copy the number of processed record
   aGSM_Polar_Cal_Data.iNumOfPoloarCalSweepRecord =  aGSM_Polar_Cal_Processed.iNumOfPoloarCalSweepRecord;

   for(i = 0; i < aGSM_Polar_Cal_Processed.iNumOfPoloarCalSweepRecord; i++)
   {
      aGSM_Polar_Cal_Data.aPolar_Cal_Sweep_Result[i].dac = aGSM_Polar_Cal_Processed.aPolar_Cal_Sweep_Result[i].dac;
      aGSM_Polar_Cal_Data.aPolar_Cal_Sweep_Result[i].amp_dBm = aGSM_Polar_Cal_Processed.aPolar_Cal_Sweep_Result[i].amp_dBm;   
      aGSM_Polar_Cal_Data.aPolar_Cal_Sweep_Result[i].phase_rad = aGSM_Polar_Cal_Processed.aPolar_Cal_Sweep_Result[i].phase_rad;
      aGSM_Polar_Cal_Data.aPolar_Cal_Sweep_Result[i].duration_counts = aGSM_Polar_Cal_Processed.aPolar_Cal_Sweep_Result[i].duration_counts;
   }

   // Fill in the measurement channel, max/min power, smooething window size and NV channel
   aGSM_Polar_Cal_Data.iChannel = 369;
   aGSM_Polar_Cal_Data.dNVMaxPower = 32;
   aGSM_Polar_Cal_Data.dNVMinPower = -15;
   aGSM_Polar_Cal_Data.dSmoothingWindowSize = 3;
   aGSM_Polar_Cal_Data.iFreqnecyLocation = QMSL_POLAR_CAL_F1_CHANNEL;


   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_GSM_Polar_Cal_NV_struct aGSM_Polar_Cal_NV;


   // Truncate and smooth the data, generate the NVs
   // Equivalent of section 5.3-5.5 in 80-V9774-11
   QLIB_RFCAL_GSM_Tx_Polar_Cal_Results(   g_hResourceContext,
      NV_MODE_GSM_850, 
      &aGSM_Polar_Cal_Data,
      &aGSM_Polar_Cal_NV,
      1);



   return 1;
}

// Range Gain Calibration, GSM Calibration Channel, GSM Rx IM2, and Thermistor Cal
unsigned char GSM_Rx_Calibration_And_Misc( void )
{
   //GSM Range gain calibration
   QMSL_GSM_Rx_Gain_Range_struct aGSM_Rx_Gain_Range;
   QMSL_GSM_Rx_Gain_Range_NV_struct aGSM_Rx_Gain_Range_NV;

   aGSM_Rx_Gain_Range.iNumRxGainRange = 5;

   aGSM_Rx_Gain_Range.iNumChannel = 8;

   // Calibration power level (dBm)
   aGSM_Rx_Gain_Range.aiCalibrationPwr[0] = -80;  // Highest gain range (0)
   aGSM_Rx_Gain_Range.aiCalibrationPwr[1] = -80;
   aGSM_Rx_Gain_Range.aiCalibrationPwr[2] = -50;
   aGSM_Rx_Gain_Range.aiCalibrationPwr[3] = -50;
   aGSM_Rx_Gain_Range.aiCalibrationPwr[4] = -50;  // Lowest gain range (4)

   // Copy RSSI values (from calibration) from different gain ranges to QMSL structure
   unsigned int i = 0;

   for(i = 0; i < aGSM_Rx_Gain_Range.iNumChannel; i++)
   {
      // for illustration, use same RSSI values for each channel indexed by i
      // in reality, RSSI values will vary across channel
      aGSM_Rx_Gain_Range.aiRSSIValue[0][i] = 1469770;
      aGSM_Rx_Gain_Range.aiRSSIValue[1][i] = 59477;
      aGSM_Rx_Gain_Range.aiRSSIValue[2][i] = 1148634;
      aGSM_Rx_Gain_Range.aiRSSIValue[3][i] = 75059;
      aGSM_Rx_Gain_Range.aiRSSIValue[4][i] = 1202;
   }

   // Convert RSSI value to NVs
   QLIB_RFCAL_GSM_Rx_Gain_Range_Results(g_hResourceContext, NV_MODE_GSM_850, &aGSM_Rx_Gain_Range,& aGSM_Rx_Gain_Range_NV, 1);

   // GSM Rx calibration channel
   unsigned short GSMCalARFCN[] = {128,145,163,180,198,215,233,251};

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes
   QMSL_RFCAL_NV_Item nvItem;

   QLIB_RFCAL_GSM_CAL_ARFCN(g_hResourceContext, NV_MODE_GSM_850, GSMCalARFCN, 8, (void*)&nvItem, 1);

   // GSM Rx IM2 Calibration
   QMSL_GSM_IM2_struct _GSMIM2;
   QMSL_RFCAL_NV_Item _GSM_IM2_NV;

   _GSMIM2.aiChannel[0] = 128;
   _GSMIM2.aiChannel[1] = 356;
   _GSMIM2.aiChannel[2] = 777;

   _GSMIM2.aIM2_I[0] = 0;
   _GSMIM2.aIM2_I[1] = 1;
   _GSMIM2.aIM2_I[2] = 2;

   _GSMIM2.aIM2_Q[0] = 3;
   _GSMIM2.aIM2_Q[1] = 4;
   _GSMIM2.aIM2_Q[2] = 5;

   _GSMIM2.aIM2_Transconductance = 6;

   QLIB_RFCAL_GSM_Rx_IM2_Cal_Results( g_hResourceContext,NV_MODE_GSM_850,&_GSMIM2,&_GSM_IM2_NV,1);


   // Thermistor Cal
   // This structure holds the measurement for NV calculations. It is made available for troubleshooting purposes
   QMSL_Internal_Thermistor_Cal_Meas_struct Int_Therm_Cal_Meas;
   Int_Therm_Cal_Meas.iCalTemp = 25;
   Int_Therm_Cal_Meas.iHKADCAtCalTemp = 100;
   Int_Therm_Cal_Meas.dSlope = -0.333;
   Int_Therm_Cal_Meas.iMinTemp = -20;
   Int_Therm_Cal_Meas.iMaxTemp = 60;
   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes   
   QMSL_Internal_Thermistor_Cal_NV_struct Int_Therm_Cal_NV;

   QLIB_RFCAL_Internal_Thermistor_Calibration_Results(   g_hResourceContext, 
      (void*) &Int_Therm_Cal_Meas,
      (void*) &Int_Therm_Cal_NV,
      1);



   return 1;
}

//Add a custom defined NV item to NV manager
unsigned char Add_OEM_NV_Item( void )
{
   QMSL_RFCAL_NV_Item OEM_NVItem;

   OEM_NVItem.aNVItemData[0] = 0x12;
   OEM_NVItem.aNVItemData[1] = 0x34;
   OEM_NVItem.aNVItemData[2] = 0x56;
   OEM_NVItem.aNVItemData[3] = 0x78;

   OEM_NVItem.iNVenum = 5678;

   strcpy(OEM_NVItem.sNVName, "OEM_NV_ITEM_1");

   QLIB_RFCAL_NV_Manager_Add_OEM_NVItem( g_hResourceContext, &OEM_NVItem);

   return 1;
}


// Print the NV items cached by NV Manager
unsigned char Create_Text_Report( void )
{
   unsigned char bOk = true;

   bOk &= QLIB_RFCAL_NV_Manager_Create_Text(g_hResourceContext, "QMSL_NV_Manager.txt");

   bOk &= QLIB_RFCAL_NV_Manager_Create_XML(g_hResourceContext, "QMSL_NV_Manager.xml");

   bOk &= QLIB_RFCAL_NV_Manager_Create_HTML(g_hResourceContext, "QMSL_NV_Manager.html");

   bOk &= QLIB_RFCAL_NV_Manager_Create_HTML_ByteStream(g_hResourceContext, "QMSL_NV_ByteStream.html");

   bOk &= QLIB_RFCAL_NV_Manager_Create_QCN(g_hResourceContext, "QMSL_NV_Manager.qcn");

   printf("Create_Text_Report: %s\n", bOk ? "PASS" : "FAIL");

   return 1;
}
// Write all NV items cached by NV Manager to Phone
unsigned char Write_NV_To_Mobile( void )
{
   QMSL_RFCal_WriteToPhone_Status _status;
   QLIB_StartLogging(g_hResourceContext, "c:\\batchwrite.txt");

   printf("0 = AUTO-DETECT\n");
   printf("1 = Legacy NV_WRITE with batch mode\n");
   printf("2 = FTM Sequencer with FTM RF NV\n");
   printf("Enter your selection:");

   char cKey = 0;
   cKey = getch();

   if(cKey == '0')
   {
      printf("AUTO DETECT\n");
      // AUTO DETECT (default);
      QLIB_RFCAL_NV_Manager_WriteToPhone(g_hResourceContext, &_status);
   }
   else if(cKey == '1')
   {
      printf("Legacy NV Write with NV Batch\n");
      QLIB_DIAG_NV_WRITE_SetBatchMode(g_hResourceContext, 1);
      
      QLIB_RFCAL_NV_Manager_WriteToPhoneMethod(g_hResourceContext, NV_Manager_WriteToPhone_USE_LEGACY_NV_WRITE);
      
      QLIB_RFCAL_NV_Manager_WriteToPhone(g_hResourceContext, &_status);
      
      unsigned short iLastErrorID;
      unsigned short iLastTotalStatus;
      unsigned char iLastItemStatus;
      
      QLIB_DIAG_NV_WRITE_FlushBatchQueue(g_hResourceContext, &iLastErrorID, &iLastTotalStatus, &iLastItemStatus);
      printf("QLIB_DIAG_NV_WRITE_FlushBatchQueue (iLastErrorID=%d, iLastTotalStatus=%d, iLastItemStatus=%d)", iLastErrorID, iLastTotalStatus, iLastItemStatus);      
      QLIB_RFCAL_NV_Manager_WriteToPhoneMethod(g_hResourceContext, 0);
   }
   else if(cKey == '2')
   {
       printf("FTM Sequencer\n");
      QLIB_RFCAL_NV_Manager_WriteToPhoneMethod(g_hResourceContext, NV_Manager_WriteToPhone_USE_FTM_SEQUENCER);

      QLIB_RFCAL_NV_Manager_WriteToPhone(g_hResourceContext, &_status);
   }


   QLIB_StopLogging(g_hResourceContext);

   unsigned int _i = 0;
   for(_i = 0; _i < _status.iNumOfItemsInCache ; _i++)
   {
      printf("NV ID%d  Write Status%d\n", _status.aNVItemStatus[_i].iNVenum, _status.aNVItemStatus[_i].iStatus); 
   }
   return 1;
}

// Clear all the values to accept the next set of items
unsigned char Clear_Data( void )
{   
   return QLIB_RFCAL_NV_Manager_Clear_Data(g_hResourceContext);
}


unsigned char LinearInterpolation_SlopeAveragingExtrapolation( void )
{
   double X[6] = {24.9, 23.6, 22.2, 21.0, 20.3, 19.9};
   double Y[6] = {100, 110, 120, 130, 140, 150};

   double Xs[8] = {25.5, 24.9, 23.6, 22.2, 21.0, 20.3, 19.8, 19.0};
   double Ys[8];


   QLIB_RFCAL_LinearInterpolation_SlopeAveragingExtrapolation
      (   g_hResourceContext,
      X, 
      Y, 
      6, 
      Xs, 
      Ys, 
      8, 
      3,
      -999, 
      999);


   printf("Ys: ");
   for(int i = 0; i< 8; i++)
   {
      printf("%d ", Ys[i]);
   }
   printf("\n");
   return 1;
}


unsigned char GPS_RFIC_IM2_calibration_test( void )
{
   unsigned short uiI_IM2_DAC_hardcoded_result = 15;
   unsigned short uiQ_IM2_DAC_hardcoded_result = 45;
   unsigned char uiCachedByNVManager = 1;

   unsigned short uiI_IM2_DAC_readback = 0;
   unsigned short uiQ_IM2_DAC_readback = 0;
   short uiI_IM2_DAC_NV_ID_readback = 0;
   short uiQ_IM2_DAC_NV_ID_readback = 0;

   QMSL_GPS_IM2_Cal_Results_struct im2_cal_results;
   QMSL_GPS_IM2_Cal_NV_struct gps_IM2_NV;

   //input from hardcoded values
   im2_cal_results.iGPS_IM2_I_DAC = uiI_IM2_DAC_hardcoded_result;
   im2_cal_results.iGPS_IM2_Q_DAC = uiQ_IM2_DAC_hardcoded_result;

   QLIB_RFCAL_GPS_IM2_Calibration_Results(   g_hResourceContext,
      (void*)& im2_cal_results,
      (void*)& gps_IM2_NV,
      uiCachedByNVManager);

   uiI_IM2_DAC_readback = gps_IM2_NV.oNV_GPS_IM2_I_DAC.aNVItemData[0];
   uiQ_IM2_DAC_readback = gps_IM2_NV.oNV_GPS_IM2_Q_DAC.aNVItemData[0];

   uiI_IM2_DAC_NV_ID_readback = gps_IM2_NV.oNV_GPS_IM2_I_DAC.iNVenum;
   uiQ_IM2_DAC_NV_ID_readback = gps_IM2_NV.oNV_GPS_IM2_Q_DAC.iNVenum;

   printf("GPS RFIC IM2 Values to write: (%d,%d)\n",im2_cal_results.iGPS_IM2_I_DAC , im2_cal_results.iGPS_IM2_Q_DAC);
   printf("GPS RFIC IM2 Values Readback: (%d,%d)\n",uiI_IM2_DAC_readback, uiQ_IM2_DAC_readback);
   printf("GPS RFIC IM2 NV ID Readback:  (%d,%d)\n",uiI_IM2_DAC_NV_ID_readback, uiQ_IM2_DAC_NV_ID_readback);

   return 1;
}

// GSM Linear Calibration
unsigned char GSM_Tx_Linear_Calibration( void )
{

   //Number of Calibration Channels
   unsigned long numChannels = 3;

   //Channel List
   int chanList[] = {128,190,251};

   //Number of PA Ranges (Low Bands: 4, High Bands: 3)
   unsigned long numPaRanges = 4; //Assume working in GSM 850 Band.

   //PA range List
   int paList[] = {0,1,2,3};

   //Num RGI
   unsigned short numRGIs = 32; //0 to 31

   //Number of modes.
   int numModes = 2; //GSM Mode 0, EDGE Mode 1

   //Total Number of measurement data
   int numDaCalibrationSegments = 1 + numPaRanges*numRGIs*numModes; //257; // 4 (pa) * 32 (rgi) * 2 (modes) + 1(trigger)

   //Array to store actual PA Index.
   int paIndex[4];

   // Sample file names
   char fileName[256];
   // Full path to sample files
   char fullPath[256];
   // Directory path to sample files used for Linear PA cal
   char *sampleFilePath = "C:\\Program Files\\Qualcomm\\QDART\\QMSL\\QLibDemo\\Linear_PA_Cal_Sample_Data\\";

   FILE *fp;

   //Step 1: Perform 3 channel DA calibration
   //1a) Send Input_Data_Dur_RGI_Mode_PA.csv to phone using QLIB_FTM_DA_CAL
   {
      const int MAX_SEGMENTS_DA_CAL = 512;
      unsigned short segLenList[MAX_SEGMENTS_DA_CAL];
      unsigned char  rgiList[MAX_SEGMENTS_DA_CAL];
      unsigned char  gsmModeList[MAX_SEGMENTS_DA_CAL];
      unsigned char  paRangeList[MAX_SEGMENTS_DA_CAL];
      strcpy(fileName,"Input_Data_Dur_RGI_Mode_PA.csv");

     memset(fullPath, 0, 256);      // Clear path
     strcat(fullPath, sampleFilePath);
     strcat(fullPath, fileName);

      fp = fopen(fullPath,"r");

      if(fp == NULL)
      {
         printf("File %s not found\n",fullPath);
         return 0;
      }
     else
     {
      printf("Loading %s data to QMSL structue to perform DA Cal\n", fileName);
     }

      int in_dur,in_rgi,in_mode,in_pa;
      int numSegments = 0;

      while(!feof(fp))
      {
         fscanf(fp,"%d,%d,%d,%d\n",&in_dur,&in_rgi,&in_mode,&in_pa);
         segLenList[numSegments] = in_dur;
         rgiList[numSegments]=(unsigned char)in_rgi;
         gsmModeList[numSegments]=in_mode;//EDGE Mode
         paRangeList[numSegments]=(unsigned char)in_pa;
         numSegments++;
      }

      fclose(fp);
      
     
     printf("Perform DA Calibraton for each channel\n");

     // QMSL Sequence to perform PA cal on a single channel

     //QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );//re-store the mode ID
     //QLIB_FTM_SET_CHAN( g_hResourceContext, channel);
     //QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0); // Center TCXO

     //QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
      //QLIB_FTM_DA_CAL( g_hResourceContext, numSegments, segLenList, rgiList, gsmModeList, paRangeList);
      //QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );//re-store the mode ID
   
   
   }
   //1b) Get power measurements from Test Equipment using the Power vs Time measurement personality
   //    On 8820 SWPPREDISTQ, on 8960 PAvT, on CMU200 IQvsSlot
   //    Duration of 273 corresponds to 1008uS, Number of steps measured = numDaCalibrationSegments
   //1c) Sample collected data for three channes available in files
   //    Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch1.csv
   //    Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch2.csv
   //    Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch3.csv

   //Step 2: Store DA Calibration Data
   QMSL_GSM_DA_Cal_NV_Chan_struct daCalNV;
   QMSL_GSM_DA_Cal_Result calResultsToQmsl;

   //Note : the nnumber of PA ranges in this case is = number possible
   //If there are lesser number of PA ranges - care should be taken to fill the array
   calResultsToQmsl.iNumMeasuredTxGainStates = numPaRanges;
   calResultsToQmsl.iNumChannels = numChannels;
   calResultsToQmsl.iNumTxPowerMeasurementsPerRange = numRGIs;

   unsigned short chan;
   //This loop is applicable to all bands - irrespective of number of PA ranges
   for(unsigned long paLoopVar=0;paLoopVar < numPaRanges; paLoopVar++)
   {
      calResultsToQmsl.aiTxMeasToNVMapping[paLoopVar] = paList[paLoopVar];
      paIndex[paList[paLoopVar]] = paLoopVar; // Create reverse mapping to use later
   }

   //Walk through Channels
   //Populate array - noting that pa ranges may be 0,2,3 for high bands
   //But they should still be loaded to 0,1,2 in the structure
   unsigned long chanLoopVar = 0;
   for(chanLoopVar = 0;chanLoopVar < numChannels;chanLoopVar++)
   {
      chan = chanList[chanLoopVar];

      //Populate struct to send to qmsl
      calResultsToQmsl.iChannel[chanLoopVar] = chan;

      //chanLoopVar 0,1,2 maps to QMSL_POLAR_CAL_F(1,2,3)_CHANNEL
      calResultsToQmsl.iFreqMapping[chanLoopVar] = (QMSL_GSM_Polar_Cal_Frequency)(chanLoopVar+1);

      switch(chanLoopVar)
      {
      case 0:
         strcpy(fileName,"Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch1.csv");break;
      case 1:
         strcpy(fileName,"Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch2.csv");break;
      case 2:
         strcpy(fileName,"Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch3.csv");break;
      }

     memset(fullPath, 0, 256);      // Clear path
     strcat(fullPath, sampleFilePath);
     strcat(fullPath, fileName);

      fp = fopen(fullPath,"r");

      if(fp == NULL)
      {
         printf("File %s not found\n",fullPath);
         return 0;
      }
     else
     {
      printf("Load sample DA cal measurement data from %s\n", fileName);
     }

      int chan,dur,rgi,mode,pa;float pwr;

      while(!feof(fp))
      {
         fscanf(fp,"%d,%d,%d,%d,%d,%f\n",&chan,&dur,&rgi,&mode,&pa,&pwr);
         //debug
         //printf("%d,%d,%d,%d,%d,%f\n",chan,dur,rgi,mode,pa,pwr);

         switch(mode)
         {
         case 0:
            calResultsToQmsl.aiGSMTxRGI[chanLoopVar][paIndex[pa]][rgi] = rgi;
            calResultsToQmsl.aiGSMTxPowerMeas[chanLoopVar][paIndex[pa]][rgi] = pwr;
            break;
         case 1:
            calResultsToQmsl.aiEDGETxRGI[chanLoopVar][paIndex[pa]][rgi] = rgi;
            calResultsToQmsl.aiEDGETxPowerMeas[chanLoopVar][paIndex[pa]][rgi] = pwr;
            break;
         }
      }

      fclose(fp);
   }

   //Queue NV
   unsigned char queue_nv_indicator = 1;

   printf("Call QLIB_RFCAL_GSM_Tx_DA_Cal_Results to transform data into NV\n");

   calResultsToQmsl.iSaturationDetectionWindow = 0.0; //For example 0.0

   QLIB_RFCAL_GSM_Tx_DA_Cal_Results(g_hResourceContext,NV_MODE_GSM_850,
      (void*)& calResultsToQmsl, (void*)& daCalNV, queue_nv_indicator );

   //Debug : Validate DA NV
   //for(unsigned long chanLoopVar = 0;chanLoopVar < 3; chanLoopVar++)
   //{
   //   printf("Channel No %d\n",chanLoopVar);
   //   for(unsigned long paLoopVar = 0;paLoopVar < 4; paLoopVar++)
   //   {
   //      printf("PA : %d\n",paLoopVar);
   //      printf("RGI,GSM NV,EDGE NV\n");
   //      for(unsigned long rgiLoopVar = 0;rgiLoopVar < 32; rgiLoopVar++)
   //      {
   //         printf( "%d,%d,%d\n",(int)daCalNV.aDaCalChanNV[chanLoopVar].aGSMRgiPmeasNV[paLoopVar].aiRGI[rgiLoopVar],
   //            (int)daCalNV.aDaCalChanNV[chanLoopVar].aGSMRgiPmeasNV[paLoopVar].aiPower[rgiLoopVar],
   //            (int)daCalNV.aDaCalChanNV[chanLoopVar].aEDGERgiPmeasNV[paLoopVar].aiPower[rgiLoopVar] );
   //      }
   //      Sleep(100);
   //   }
   //}


   //Step 3: Use Data to determine predistortion calibration rgi
   //Find the smallest RGI which can produce predistCalRGIPwr across the three channels

   //EDGE Power at which predistortion calibration is performed
   float predistCalRGIPwr = 27.0;//dBm

   //The final variable that holds the selected RGI
   int rgiAllChan = 0;

   //Intermediate variable to store RGI
   int rgiMatch = 0;

   //Const Predistortion is performed in PA range 0
   int predistCalPARange = 0;

   unsigned long rgiLoopVar = 0;   
   
   for(chanLoopVar = 0;chanLoopVar < numChannels;chanLoopVar++)
   {
     
      for(rgiLoopVar = 0;rgiLoopVar < numRGIs;rgiLoopVar++)
      {
         if(calResultsToQmsl.aiEDGETxPowerMeas[chanLoopVar][paIndex[predistCalPARange]][rgiLoopVar] >  predistCalRGIPwr)
         {
            rgiMatch = calResultsToQmsl.aiEDGETxRGI[chanLoopVar][paIndex[predistCalPARange]][rgiLoopVar];
            break;
         }
      }
      if(rgiMatch > rgiAllChan)
      {
         rgiAllChan = rgiMatch;
      }
   }
   //Debug: Print Results
   //printf("RGI Chosen : %d\n",rgiAllChan);

   //Step 4: Do Predistortion calibration
   //4a) Send the predistortion calibration waveform from the phone 
   //    using QLIB_FTM_TX_CFG2_AMAM_SWEEP with the calculated rgiAllChan as parameter
   //    QLIB_FTM_TX_CFG2_AMAM_SWEEP( g_hResourceContext, dcDuration_QS, edgeDuration_QS, rgiAllChan, &calResult.iEDGETxCalScale );
   //4b) Get power measurements from Test Equipment using the IQ Capture personality
   //4c) Sample collected data for three channes available in files
   //    File Format :: Time,Phase,Power(dBm)
   //    AMAM_Cal__08_53_35_Ch0.csv
   //    AMAM_Cal__08_53_35_Ch1.csv
   //    AMAM_Cal__08_53_35_Ch2.csv

   //Step 5: Store Predisortion NV

   //Populate QMSL struct to pass to QLIB

   static QMSL_GSM_PreDist_Cal_Result calResult;
   QMSL_GSM_PreDist_Cal_NV_Chan_struct calNV;
   calResult.iEDGETxCalScale = 4898; //This is the return value from the QLIB_FTM_TX_CFG2_AMAM_SWEEP API 4a) above

   //The input parameters for cal.
   int dcDuration_QS = 1000;
   int edgeDuration_QS = 1000;

   // Number of samples for the data capture when the cal command was called with 1000,1000
   int numSamples = 19460;
   float samplingRateHz = 10.0e6;
   float symbolrate = 270833.0;

   calResult.iNoiseSamples = 500;//This also configured on the call box (as trigger delay)
   calResult.iNumChannels = numChannels;
   calResult.iNumPredistortionWaveformSamples = numSamples;
   calResult.iSamplingRateHz = (long)samplingRateHz;
   calResult.iCalRgi = rgiAllChan;
   calResult.iDcSamples = (long)((dcDuration_QS/4)*(samplingRateHz/symbolrate));
   calResult.iEdgeSamples =(long)((edgeDuration_QS/4)*(samplingRateHz/symbolrate));

   unsigned char nvArr[128];unsigned short readStatus;
   unsigned short nvID;
   nv_mode_id_type nv_mode = NV_MODE_GSM_850;

   //Note: The NV items 5478,6475,6476,6477 must be populated before calibration is performed
   switch(nv_mode)
   {
   case NV_MODE_GSM_EGSM:
      nvID = 5478; //NV_EDGE_LINEAR_TX_GAIN_PARAM_I
      break;
   case NV_MODE_GSM_850:
      nvID = 6475; //NV_EDGE_850_LINEAR_TX_GAIN_PARAM_I
      break;
   case NV_MODE_GSM_DCS:
      nvID = 6476; //NV_EDGE_1800_LINEAR_TX_GAIN_PARAM_I
      break;
   case NV_MODE_GSM_1900:
      nvID = 6477; //NV_EDGE_1900_LINEAR_TX_GAIN_PARAM_I
      break;
   }
   //Read the NV that has been stored earlier.
   //This ensures that there is one source for the Linear Tx Gain Param
   QLIB_DIAG_NV_READ_F(g_hResourceContext,nvID,nvArr,128,&readStatus);
   calResult.iEDGETxGainParam = *(short *)(&nvArr[0]);

   for(chanLoopVar = 0;chanLoopVar < numChannels;chanLoopVar++)
   {
  
     
      //chanLoopVar 0,1,2 maps to QMSL_POLAR_CAL_F(1,2,3)_CHANNEL
      calResult.iFreqMapping[chanLoopVar] = (QMSL_GSM_Polar_Cal_Frequency)(chanLoopVar+1);

      switch(chanLoopVar)
      {
      case 0:
         strcpy(fileName,"AMAM_Cal__08_53_35_Ch0.csv");break;
      case 1:
         strcpy(fileName,"AMAM_Cal__08_53_35_Ch1.csv");break;
      case 2:
         strcpy(fileName,"AMAM_Cal__08_53_35_Ch2.csv");break;
      }

      float time,phase,ampl_dBm;

      int sampleCounter = 0;
   
     memset(fullPath, 0, 256);      // Clear path
     strcat(fullPath, sampleFilePath);
     strcat(fullPath, fileName);

      fp = fopen(fullPath,"r");

      if(fp == NULL)
      {
         printf("File %s not found\n",fullPath);
         return 0;
      }
     else
     {
      printf("Load sample Predistortion cal IQ data from %s\n", fileName);
     }

      while(!feof(fp))
      {
         fscanf(fp,"%f,%f,%f\n",&time,&phase,&ampl_dBm);
         calResult.adTxAmpl[chanLoopVar][sampleCounter] = ampl_dBm;
         calResult.adTxPhaseArr[chanLoopVar][sampleCounter] = phase;
         sampleCounter++;
      }

      fclose(fp);
     
   }

   calResult.iDCTransientPercent = (float)60.0;
   calResult.iEDGETransientSymbols = 90;

   printf("Call QLIB_RFCAL_GSM_Tx_Linear_Process_PreDist_Cal_Results to transform PreDistortion Calibration data into NV\n");
   QLIB_RFCAL_GSM_Tx_Linear_Process_PreDist_Cal_Results(   g_hResourceContext,nv_mode,(void*)& calResult, (void*)& calNV,
      queue_nv_indicator);

   

   return 1;
}

// GSM Linear Calibration
// The V2 function is for the swapped waveform - EDGE first, DC later
unsigned char GSM_Tx_Linear_Calibration_V2( void )
{

   //Number of Calibration Channels
   unsigned long numChannels = 3;

   //Channel List
   int chanList[] = {128,190,251};

   //Number of PA Ranges (Low Bands: 4, High Bands: 3)
   unsigned long numPaRanges = 4; //Assume working in GSM 850 Band.

   //PA range List
   int paList[] = {0,1,2,3};

   //Num RGI
   unsigned short numRGIs = 32; //0 to 31

   //Number of modes.
   int numModes = 2; //GSM Mode 0, EDGE Mode 1

   //Total Number of measurement data
   int numDaCalibrationSegments = 1 + numPaRanges*numRGIs*numModes; //257; // 4 (pa) * 32 (rgi) * 2 (modes) + 1(trigger)

   //Array to store actual PA Index.
   int paIndex[4];

   // Sample file names
   char fileName[256];
   // Full path to sample files
   char fullPath[256];
   // Directory path to sample files used for Linear PA cal
   char *sampleFilePath = "C:\\Program Files\\Qualcomm\\QDART\\QMSL\\QLibDemo\\Linear_PA_Cal_Sample_Data\\";
   FILE *fp;

   //Step 1: Perform 3 channel DA calibration
   //1a) Send Input_Data_Dur_RGI_Mode_PA.csv to phone using QLIB_FTM_DA_CAL
   {
      const int MAX_SEGMENTS_DA_CAL = 512;
      unsigned short segLenList[MAX_SEGMENTS_DA_CAL];
      unsigned char  rgiList[MAX_SEGMENTS_DA_CAL];
      unsigned char  gsmModeList[MAX_SEGMENTS_DA_CAL];
      unsigned char  paRangeList[MAX_SEGMENTS_DA_CAL];
      strcpy(fileName,"Input_Data_Dur_RGI_Mode_PA.csv");

     memset(fullPath, 0, 256);      // Clear path
     strcat(fullPath, sampleFilePath);
     strcat(fullPath, fileName);

      fp = fopen(fullPath,"r");

      if(fp == NULL)
      {
         printf("File %s not found\n",fullPath);
         return 0;
      }
     else
     {
      printf("Loading %s data to QMSL structue to perform DA Cal\n", fileName);
     }

      int in_dur,in_rgi,in_mode,in_pa;
      int numSegments = 0;

      while(!feof(fp))
      {
         fscanf(fp,"%d,%d,%d,%d\n",&in_dur,&in_rgi,&in_mode,&in_pa);
         segLenList[numSegments] = in_dur;
         rgiList[numSegments]=(unsigned char)in_rgi;
         gsmModeList[numSegments]=in_mode;//EDGE Mode
         paRangeList[numSegments]=(unsigned char)in_pa;
         numSegments++;
      }

      fclose(fp);
      
     
     printf("Perform DA Calibraton for each channel\n");

     // QMSL Sequence to perform PA cal on a single channel

     //QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );//re-store the mode ID
     //QLIB_FTM_SET_CHAN( g_hResourceContext, channel);
     //QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0); // Center TCXO

     //QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
      //QLIB_FTM_DA_CAL( g_hResourceContext, numSegments, segLenList, rgiList, gsmModeList, paRangeList);
      //QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );//re-store the mode ID
   
   
   }
   //1b) Get power measurements from Test Equipment using the Power vs Time measurement personality
   //    On 8820 SWPPREDISTQ, on 8960 PAvT, on CMU200 IQvsSlot
   //    Duration of 273 corresponds to 1008uS, Number of steps measured = numDaCalibrationSegments
   //1c) Sample collected data for three channes available in files
   //    Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch1.csv
   //    Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch2.csv
   //    Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch3.csv

   //Step 2: Store DA Calibration Data
   QMSL_GSM_DA_Cal_NV_Chan_struct daCalNV;
   QMSL_GSM_DA_Cal_Result calResultsToQmsl;

   //Note : the nnumber of PA ranges in this case is = number possible
   //If there are lesser number of PA ranges - care should be taken to fill the array
   calResultsToQmsl.iNumMeasuredTxGainStates = numPaRanges;
   calResultsToQmsl.iNumChannels = numChannels;
   calResultsToQmsl.iNumTxPowerMeasurementsPerRange = numRGIs;

   unsigned short chan;
   //This loop is applicable to all bands - irrespectiveof number of PA ranges
   for(unsigned long paLoopVar=0;paLoopVar < numPaRanges; paLoopVar++)
   {
      calResultsToQmsl.aiTxMeasToNVMapping[paLoopVar] = paList[paLoopVar];
      paIndex[paList[paLoopVar]] = paLoopVar; // Create reverse mapping to use later
   }

   //Walk through Channels
   //Populate array - noting that pa ranges may be 0,2,3 for high bands
   //But they should still be loaded to 0,1,2 in the structure
   unsigned long chanLoopVar = 0;
   for(chanLoopVar = 0;chanLoopVar < numChannels;chanLoopVar++)
   {
      chan = chanList[chanLoopVar];

      //Populate struct to send to qmsl
      calResultsToQmsl.iChannel[chanLoopVar] = chan;

      //chanLoopVar 0,1,2 maps to QMSL_POLAR_CAL_F(1,2,3)_CHANNEL
      calResultsToQmsl.iFreqMapping[chanLoopVar] = (QMSL_GSM_Polar_Cal_Frequency)(chanLoopVar+1);

      switch(chanLoopVar)
      {
      case 0:
         strcpy(fileName,"Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch1.csv");break;
      case 1:
         strcpy(fileName,"Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch2.csv");break;
      case 2:
         strcpy(fileName,"Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch3.csv");break;
      }

     memset(fullPath, 0, 256);      // Clear path
     strcat(fullPath, sampleFilePath);
     strcat(fullPath, fileName);

      fp = fopen(fullPath,"r");

      if(fp == NULL)
      {
         printf("File %s not found\n",fullPath);
         return 0;
      }
     else
     {
      printf("Load sample DA cal measurement data from %s\n", fileName);
     }

      int chan,dur,rgi,mode,pa;float pwr;

      while(!feof(fp))
      {
         fscanf(fp,"%d,%d,%d,%d,%d,%f\n",&chan,&dur,&rgi,&mode,&pa,&pwr);
         //debug
         //printf("%d,%d,%d,%d,%d,%f\n",chan,dur,rgi,mode,pa,pwr);

         switch(mode)
         {
         case 0:
            calResultsToQmsl.aiGSMTxRGI[chanLoopVar][paIndex[pa]][rgi] = rgi;
            calResultsToQmsl.aiGSMTxPowerMeas[chanLoopVar][paIndex[pa]][rgi] = pwr;
            break;
         case 1:
            calResultsToQmsl.aiEDGETxRGI[chanLoopVar][paIndex[pa]][rgi] = rgi;
            calResultsToQmsl.aiEDGETxPowerMeas[chanLoopVar][paIndex[pa]][rgi] = pwr;
            break;
         }
      }

      fclose(fp);
   }

   //Queue NV
   unsigned char queue_nv_indicator = 1;

   calResultsToQmsl.iSaturationDetectionWindow = 0.1; //For example 0.1

   printf("Call QLIB_RFCAL_GSM_Tx_DA_Cal_Results to transform data into NV\n");



   QLIB_RFCAL_GSM_Tx_DA_Cal_Results(g_hResourceContext,NV_MODE_GSM_850,
      (void*)& calResultsToQmsl, (void*)& daCalNV, queue_nv_indicator );

   //Debug : Validate DA NV
   //for(unsigned long chanLoopVar = 0;chanLoopVar < 3; chanLoopVar++)
   //{
   //   printf("Channel No %d\n",chanLoopVar);
   //   for(unsigned long paLoopVar = 0;paLoopVar < 4; paLoopVar++)
   //   {
   //      printf("PA : %d\n",paLoopVar);
   //      printf("RGI,GSM NV,EDGE NV\n");
   //      for(unsigned long rgiLoopVar = 0;rgiLoopVar < 32; rgiLoopVar++)
   //      {
   //         printf( "%d,%d,%d\n",(int)daCalNV.aDaCalChanNV[chanLoopVar].aGSMRgiPmeasNV[paLoopVar].aiRGI[rgiLoopVar],
   //            (int)daCalNV.aDaCalChanNV[chanLoopVar].aGSMRgiPmeasNV[paLoopVar].aiPower[rgiLoopVar],
   //            (int)daCalNV.aDaCalChanNV[chanLoopVar].aEDGERgiPmeasNV[paLoopVar].aiPower[rgiLoopVar] );
   //      }
   //      Sleep(100);
   //   }
   //}


   //Step 3: Use Data to determine predistortion calibration rgi
   //Find the smallest RGI which can produce predistCalRGIPwr across the three channels

   //EDGE Power at which predistortion calibration is performed
   float predistCalRGIPwr = 27.0;//dBm

   //The final variable that holds the selected RGI
   int rgiAllChan = 0;

   //Intermediate variable to store RGI
   int rgiMatch = 0;

   //Const Predistortion is performed in PA range 0
   int predistCalPARange = 0;

   unsigned long rgiLoopVar = 0;   
   
   for(chanLoopVar = 0;chanLoopVar < numChannels;chanLoopVar++)
   {
     
      for(rgiLoopVar = 0;rgiLoopVar < numRGIs;rgiLoopVar++)
      {
         if(calResultsToQmsl.aiEDGETxPowerMeas[chanLoopVar][paIndex[predistCalPARange]][rgiLoopVar] >  predistCalRGIPwr)
         {
            rgiMatch = calResultsToQmsl.aiEDGETxRGI[chanLoopVar][paIndex[predistCalPARange]][rgiLoopVar];
            break;
         }
      }
      if(rgiMatch > rgiAllChan)
      {
         rgiAllChan = rgiMatch;
      }
   }
   //Debug: Print Results
   //printf("RGI Chosen : %d\n",rgiAllChan);

   //Step 4: Do Predistortion calibration
   //4a) Send the predistortion calibration waveform from the phone 
   //    using QLIB_FTM_TX_CFG2_AMAM_SWEEP with the calculated rgiAllChan as parameter
   //    QLIB_FTM_TX_CFG2_AMAM_SWEEP_V2( g_hResourceContext, dcDuration_QS, edgeDuration_QS, rgiAllChan, waveformType, &calResult.iEDGETxCalScale );
   //4b) Get power measurements from Test Equipment using the IQ Capture personality
   //4c) Sample collected data for three channes available in files
   //    File Format :: Time,Phase,Power(dBm)
   //    AMAM_Cal__09_59_51_Ch0.csv
   //    AMAM_Cal__09_59_51_Ch1.csv
   //    AMAM_Cal__09_59_51_Ch2.csv

   //Step 5: Store Predisortion NV

   //Populate QMSL struct to pass to QLIB

   static QMSL_GSM_PreDist_Cal_Result calResult;
   QMSL_GSM_PreDist_Cal_NV_Chan_struct calNV;
   calResult.iEDGETxCalScale = 4898; //This is the return value from the QLIB_FTM_TX_CFG2_AMAM_SWEEP API 4a) above

   //The input parameters for cal.
   int dcDuration_QS = 1000;
   int edgeDuration_QS = 600;

   // Number of samples for the data capture when the cal command was called with 1000,1000
   int numSamples = 15760;
   float samplingRateHz = 10.0e6;
   float symbolrate = 270833.0;

   calResult.iNoiseSamples = 500;//This also configured on the call box (as trigger delay)
   calResult.iNumChannels = numChannels;
   calResult.iNumPredistortionWaveformSamples = numSamples;
   calResult.iSamplingRateHz = (long)samplingRateHz;
   calResult.iCalRgi = rgiAllChan;
   calResult.iDcSamples = (long)((dcDuration_QS/4)*(samplingRateHz/symbolrate));
   calResult.iEdgeSamples =(long)((edgeDuration_QS/4)*(samplingRateHz/symbolrate));

   unsigned short nvID;
   nv_mode_id_type nv_mode = NV_MODE_GSM_850;

   //Note: The NV items 5478,6475,6476,6477 must be populated before calibration is performed
   switch(nv_mode)
   {
   case NV_MODE_GSM_EGSM:
      nvID = 5478; //NV_EDGE_LINEAR_TX_GAIN_PARAM_I
      break;
   case NV_MODE_GSM_850:
      nvID = 6475; //NV_EDGE_850_LINEAR_TX_GAIN_PARAM_I
      break;
   case NV_MODE_GSM_DCS:
      nvID = 6476; //NV_EDGE_1800_LINEAR_TX_GAIN_PARAM_I
      break;
   case NV_MODE_GSM_1900:
      nvID = 6477; //NV_EDGE_1900_LINEAR_TX_GAIN_PARAM_I
      break;
   }
   //Read the NV that has been stored earlier.
   //This ensures that there is one source for the Linear Tx Gain Param
   //unsigned char nvArr[128];unsigned short readStatus;
   //QLIB_DIAG_NV_READ_F(g_hResourceContext,nvID,nvArr,128,&readStatus);
   calResult.iEDGETxGainParam = 93;

   for(chanLoopVar = 0;chanLoopVar < numChannels;chanLoopVar++)
   {
  
     
      //chanLoopVar 0,1,2 maps to QMSL_POLAR_CAL_F(1,2,3)_CHANNEL
      calResult.iFreqMapping[chanLoopVar] = (QMSL_GSM_Polar_Cal_Frequency)(chanLoopVar+1);

      switch(chanLoopVar)
      {
      case 0:
         strcpy(fileName,"AMAM_Cal__09_59_51_Ch0.csv");break;
      case 1:
         strcpy(fileName,"AMAM_Cal__09_59_51_Ch1.csv");break;
      case 2:
         strcpy(fileName,"AMAM_Cal__09_59_51_Ch2.csv");break;
      }

      float time,phase,ampl_dBm;

      int sampleCounter = 0;
   
     memset(fullPath, 0, 256);      // Clear path
     strcat(fullPath, sampleFilePath);
     strcat(fullPath, fileName);

      fp = fopen(fullPath,"r");

      if(fp == NULL)
      {
         printf("File %s not found\n",fullPath);
         return 0;
      }
     else
     {
      printf("Load sample Predistortion cal IQ data from %s\n", fileName);
     }

      while(!feof(fp))
      {
         fscanf(fp,"%f,%f,%f\n",&time,&phase,&ampl_dBm);
         calResult.adTxAmpl[chanLoopVar][sampleCounter] = ampl_dBm;
         calResult.adTxPhaseArr[chanLoopVar][sampleCounter] = phase;
         sampleCounter++;
      }

      fclose(fp);
     
   }

   calResult.iDCTransientPercent = (float)10.0;
   calResult.iEDGETransientSymbols = 20;


   printf("Call QLIB_RFCAL_GSM_Tx_Linear_Process_Swapped_PreDist_Cal_Results to transform PreDistortion Calibration data into NV\n");
   QLIB_RFCAL_GSM_Tx_Linear_Process_Swapped_PreDist_Cal_Results(   g_hResourceContext,nv_mode,(void*)& calResult, (void*)& calNV,
      queue_nv_indicator);

   

   return 1;
}

unsigned char WCDMA_RTR8600_Calibration( void )
{
   printf("WCDMA_RTR8600_Calibration:\n");
   unsigned char bOk = true;

   // WCDMA IMT band
   unsigned short iNV_Mode_ID = PHONE_MODE_WCDMA_IMT;

   // Store generated NV items in NV Manager. NV items stored in NV Manager will be 
   // written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone is called.
   unsigned char  bCachedByNVManager = 1; 
   
   // Write NV cal channels
   unsigned char bWriteNVChList = 1;
      
   // Measured channel lists
   unsigned short MeasuredTxChannels[16] = {9621,9638,9656,9673,9691,9708,9726,9750,9761,9778,9796,9813,9831,9848,9866,9883};
   unsigned short MeasuredRxChannels[16] = {10571,10588,10606,10623,10641,10658,10676,10700,10711,10728,10746,10763,10781,10798,10816,10833};

   // Fill out the NV cal channel lists
   QMSL_CDMA_NVChannels_struct TX_NV_Channels;
   QMSL_CDMA_NVChannels_struct RX_NV_Channels;

   int i, ch;
   for(ch = 0; ch < 16; ch++)
   {
      TX_NV_Channels.aiNVChannels[ch] = MeasuredTxChannels[ch];
      RX_NV_Channels.aiNVChannels[ch] = MeasuredRxChannels[ch];
   }

   ////////////////////
   // Tx Linearizer
   ////////////////////
   int Tx0size = 76;
   int Tx1size = 62;
   int Tx2size = 68;

   // Measured data for Tx Linearizer
   int Tx0_PDM[] = {100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 
                    75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51,
                 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25};

   double Tx0_Power[] = {11.9905, 11.745, 11.5457, 11.1655, 10.8647, 10.6102, 10.2193, 10.1027, 9.77901, 9.40845, 8.08197, 
                        7.03574, 5.79468, 4.64451, 3.52126, 2.53305, 1.73548, 0.180189, -0.399884, -1.95548, -2.13659, 
                    -2.96891, -4.00713, -4.99497, -5.84264, -6.94977, -7.79111, -8.87587, -9.92183, -10.8706, -12.11, 
                    -12.6698, -13.5418, -14.6658, -16.05, -16.6765, -17.5348, -18.9448, -19.2139, -20.6674, -21.0156, 
                    -21.9794, -23.1804, -24.316, -25.3735, -26.389, -27.4761, -28.3522, -29.7559, -30.83, -31.8026, 
                    -32.8075, -33.4693, -34.9255, -35.6011, -36.5251, -37.7329, -39.0376, -39.9262, -40.8044, -41.9713, 
                    -42.4276, -43.7539, -45.0421, -45.789, -46.7666, -47.9516, -48.9998, -49.8592, -50.7591, -51.4787, 
                    -52.9399, -53.7936, -54.6704, -55.9077, -57.1528};

   int Tx1_PDM[] = {100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 
                    75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51,
                  50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39};

   double Tx1_Power[] = {18.7237, 18.4448, 18.2213, 17.8037, 17.4888, 17.2339, 16.8311, 16.7037, 16.3689, 16.0033, 14.6893, 
                      13.6486, 12.4081, 11.2126, 10.0132, 8.91754, 8.02856, 6.28786, 5.67311, 3.96579, 3.79075, 2.85519, 
                    1.75076, 0.729535, -0.157258, -1.31105, -2.22306, -3.35937, -4.40184, -5.40778, -6.68136, -7.22854, 
                    -8.1061, -9.27773, -10.692, -11.3212, -12.2245, -13.5719, -13.9081, -15.2998, -15.7268, -16.7033, 
                   -17.9299, -19.0597, -20.1153, -21.1412, -22.2233, -23.0945, -24.5084, -25.5737, -26.5445, -27.5622, 
                   -28.2139, -29.6755, -30.3455, -31.263, -32.4701, -33.7834, -34.6772, -35.5313, -36.7045, -37.158};

   int   Tx2_PDM[] = {106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 
                  82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 
                 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39};

   double Tx2_Power[] = {31.5093, 31.2733, 30.9259, 30.5967, 30.4114, 30.2254, 29.8786, 29.5346, 29.3309, 29.2823, 28.5919, 
                    28.0339, 27.5514, 26.6665, 26.2566, 25.7429, 24.6043, 23.6982, 22.5561, 21.5769, 20.314, 19.0644, 
                    17.6652, 16.9924, 16.0033, 14.6134, 13.6198, 12.685, 11.6041, 10.6395, 9.74601, 8.62201, 7.68644, 
                    6.56957, 5.56451, 4.55172, 3.32367, 2.75665, 1.8624, 0.680917, -0.700576, -1.35146, -2.29058, 
                    -3.59016, -3.96791, -5.31647, -6.0197, -7.22827, -7.9969, -9.12542, -10.2161, -11.2176, -12.2523, 
                    -13.1484, -14.5552, -15.6497, -16.5689, -17.6078, -18.3061, -19.7186, -20.4209, -21.4002, -22.5433, 
                    -23.8724, -24.7611, -25.6029, -26.7964, -27.2204};

   int   Tx2_HDET[] = {4095, 4087, 3910, 3754, 3677, 3597, 3446, 3300, 3223, 3203, 2954, 2766, 2609, 2362, 2247, 2120, 1856, 
                 1672, 1471, 1323, 1155, 1020, 903, 859, 797, 733, 695, 666, 638, 621, 607, 594, 585, 576, 570, 563, 560, 
                 559, 556, 554, 552, 550, 551, 549, 551, 549, 549, 547, 547, 548, 549, 547, 547, 547, 547, 546, 547, 549, 
                 545, 547, 547, 547, 546, 547, 545, 544, 545, 547};


   // QMSL structure used to store Tx linearizer calibration data
   QMSL_CDMA_TxLin_BetaScaling_struct TxLinData;

   // Copy the measured data to the QMSL structure
   // Low gain Tx linearizer
   for(i = 0; i < Tx0size; i++)
   {
      TxLinData.aiTxLinPower[0][i] = Tx0_Power[i];
      TxLinData.aiTxLinPDM[0][i] = Tx0_PDM[i];
   }

   // Mid gain Tx linearizer
   for(i = 0; i < Tx1size; i++)
   {
      TxLinData.aiTxLinPower[1][i] = Tx1_Power[i];
      TxLinData.aiTxLinPDM[1][i] = Tx1_PDM[i];
   }

   // High gain Tx linearizer
   for(i = 0; i < Tx2size; i++)
   {
      TxLinData.aiTxLinPower[2][i] = Tx2_Power[i];
      TxLinData.aiTxLinPDM[2][i] = Tx2_PDM[i];
      TxLinData.aiTxHDET[i] = Tx2_HDET[i];
   }

    // Not using LPM HDET
   TxLinData.aiTxHDETLPMEnabled = 0;

   TxLinData.iNumMeasuredTxGainStates = 3; 
   TxLinData.iNumTxPowerMeasurementsPerGainState[0] = Tx0size;
   TxLinData.iNumTxPowerMeasurementsPerGainState[1] = Tx1size;
   TxLinData.iNumTxPowerMeasurementsPerGainState[2] = Tx2size;
   
   // TxLinData.aiTxMeasToNVMapping[n] = y
   // where n is the Tx gain state index to use in TxLinData.aiTxLinPower[n][i] 
   // and y is the index of the corresponding NV table
   TxLinData.aiTxMeasToNVMapping[0] = 0;
   TxLinData.aiTxMeasToNVMapping[1] = 1;
   TxLinData.aiTxMeasToNVMapping[2] = 2;   

   // TxLinData.aiTxLinPower[2] is used to generate HDET vs AGC table
   TxLinData.iTxMeasIndexUsedForHDET = 2;

   TxLinData.HDET_OffsetPower_dBm = 19;
   TxLinData.HDET_SpanPower_dBm  =  27;

   TxLinData.aiTxSweepStepSize[0] = 1;
   TxLinData.aiTxSweepStepSize[1] = 1;
   TxLinData.aiTxSweepStepSize[2] = 1;

   TxLinData.aiHeadRoomPDMCountList[0] = 0;
   TxLinData.aiHeadRoomPDMCountList[1] = 0;
   TxLinData.aiHeadRoomPDMCountList[2] = 0;

   TxLinData.iUMTS_TxAGC_PDM_Max = 106;
   TxLinData.i_TxAGC_PDM_MinGain = 0;
   TxLinData.iNV_Resolution = 10;

   TxLinData.iEnh_Linearizer_0_NV = 1;
   TxLinData.iSkip_NonMonotonic_Items = 1;
   TxLinData.iDo_Not_Extrapolate_Lower_Powers = 1;
   TxLinData.iUse_HDET_V2 = 1;
   TxLinData.iHDET_MaxLim = 4095;

   // Sort by power
   TxLinData.iSortingOrder = 0;

   // Sorting order = descending 
   TxLinData.iSortNVTxLin = 0;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes.
   QMSL_CDMA_TxLin_BetaScaling_Result_struct aTx_Result;   

   bOk = QLIB_RFCAL_CDMA_TxLinCal_BetaScaling_Results(g_hResourceContext, 
                                          iNV_Mode_ID,
                                          &TxLinData, 
                                          &aTx_Result,
                                          bCachedByNVManager);

   printf("QLIB_RFCAL_CDMA_TxLinCal_BetaScaling_Results: %s\n", bOk ? "PASS" : "FAIL");

   ////////////////////
   // Tx Lin vs. Freq
   ////////////////////

   // Measured data for Tx frequency compensation (measured Tx power across frequency for the same PDM)
   double Tx0_FreqComp[16] = {3.42055, 3.47059, 3.30358, 3.10099, 2.69724, 2.42989, 2.19503, 1.97894, 2.06164, 2.22788, 2.52696, 2.72239, 2.96918, 3.27054, 3.14593, 2.94};
   double Tx1_FreqComp[16] = {12.0601, 12.2495, 12.2253, 11.9855, 11.336, 10.6476, 9.90405, 9.01796, 8.78278, 8.52968, 8.50401, 8.47509, 8.63137, 9.05066, 9.27947, 9.49171};
   double Tx2_FreqComp[16] = {24.2587, 24.4719, 24.6116, 24.6927, 24.415, 24.0792, 23.6093, 22.9308, 22.7605, 22.5641, 22.5256, 22.4853, 22.5919, 22.948, 23.1388, 23.3493};

   // QMSL structures used to store Tx linearizer vs. frequency calibration data
   QMSL_CDMA_TxLinVsFreq_struct TxLinVsFreq;
   QMSL_CDMA_MeasuredChannels_struct TxMeasuredChs;

   // Copy the measured data to the QMSL structure
   for(ch = 0; ch < 16; ch++)
   {
      TxLinVsFreq.aiTxMeasuredPower[0][ch] = Tx0_FreqComp[ch];
      TxLinVsFreq.aiTxMeasuredPower[1][ch] = Tx1_FreqComp[ch];
      TxLinVsFreq.aiTxMeasuredPower[2][ch] = Tx2_FreqComp[ch];
   }

   // TxLinVsFreq.aiTxMeasToNVMapping[n] = y
   // where n is the Tx gain state index to use in TxLinVsFreq.aiTxMeasuredPower[n][i] 
   // and y is the index of the corresponding NV table
   TxLinVsFreq.aiTxMeasToNVMapping[0] = 0;
   TxLinVsFreq.aiTxMeasToNVMapping[1] = 1;
   TxLinVsFreq.aiTxMeasToNVMapping[2] = 2;

   TxLinVsFreq.iNumMeasuredTxGainStates = 3;
   
   // Primary Tx comp only (no secondary Tx comp)
   TxLinVsFreq.iTxCompType = 0;
   TxLinVsFreq.a2ndTxCompHasMeas[0] = 0;
   TxLinVsFreq.a2ndTxCompHasMeas[1] = 0;
   TxLinVsFreq.a2ndTxCompHasMeas[2] = 0;
   TxLinVsFreq.a2ndTxCompHasMeas[3] = 0;

   // Copy the measurement channels to QMSL structure
   for(ch = 0; ch < 16; ch++)
   {
      TxMeasuredChs.aiMeasuredChannels[ch] = MeasuredTxChannels[ch];
   }

   TxMeasuredChs.iRefChannel = 9750;
   TxMeasuredChs.iNumMeasuredChannels = 16;

   int iNV_Resolution_Type = 10;

   // Generate NV_<bandclass>_TX_COMP_VS_FREQ_<x>_I items
   int iNV_Tx_Comp_Type = QMSL_NV_TX_COMP;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes.
   QMSL_CDMA_Tx_LinVsFreq_Cal_NV_Result_struct TxLinVsFreq_NV;   

   bOk = QLIB_RFCAL_CDMA_Tx_LinVsFreq_Results(g_hResourceContext, 
                                    iNV_Mode_ID,
                                    (void*)&TxLinVsFreq, 
                                    (void*)&TxMeasuredChs, 
                                    (void*)&TX_NV_Channels,
                                    iNV_Resolution_Type,
                                    iNV_Tx_Comp_Type,
                                    (void*)&TxLinVsFreq_NV,
                                    bWriteNVChList,
                                    bCachedByNVManager);

   printf("QLIB_RFCAL_CDMA_Tx_LinVsFreq_Results: %s\n", bOk ? "PASS" : "FAIL");


   ////////////////////
   // Tx Lim vs. Freq
   ////////////////////

   // Measurement setup data for Tx Limiting vs Frequency 
   // (list of PDM values used to take datapoints near maximum Tx power across frequency)
   int   LimVsFreq_PDM[] = {84, 86, 87, 88, 89, 89, 90, 90, 91, 92, 93, 93, 94, 95};

   // Power measured for 16 cal channels at the PDMs listed in LimVsFreq_PDM
   double Ch1_LimVsFreq_Power[]   = {19.525,  22.0018, 23.153,  24.185,  25.1426, 25.1055, 26.1139, 26.0733, 27.0756, 27.3918, 27.9193, 27.8834, 28.6743, 29.0217};
   double Ch2_LimVsFreq_Power[]   = {19.7435, 22.1904, 23.2712, 24.382,  25.2724, 25.2569, 26.2761, 26.2573, 27.2052, 27.5109, 28.0645, 28.0412, 28.8151, 29.1353};
   double Ch3_LimVsFreq_Power[]   = {19.9089, 22.3086, 23.3958, 24.5196, 25.4014, 25.3869, 26.4046, 26.3848, 27.3237, 27.5815, 28.199,  28.1784, 28.8989, 29.2095};
   double Ch4_LimVsFreq_Power[]   = {19.9567, 22.3752, 23.4458, 24.5982, 25.4991, 25.4686, 26.4742, 26.4592, 27.3974, 27.6778, 28.3039, 28.2704, 28.9927, 29.2897};
   double Ch5_LimVsFreq_Power[]   = {19.6163, 22.1007, 23.1868, 24.3255, 25.2805, 25.2495, 26.2192, 26.2039, 27.1976, 27.4926, 28.0861, 28.0538, 28.7785, 29.0973};
   double Ch6_LimVsFreq_Power[]   = {19.2324, 21.7577, 22.8994, 23.9933, 24.9903, 24.9896, 25.9201, 25.8964, 26.9352, 27.2834, 27.8257, 27.7851, 28.5514, 28.8949};
   double Ch7_LimVsFreq_Power[]   = {18.6617, 21.2777, 22.4953, 23.5336, 24.588,  24.564,  25.4827, 25.4641, 26.5573, 26.9637, 27.443,  27.4039, 28.2018, 28.5858};
   double Ch8_LimVsFreq_Power[]   = {17.9273, 20.6185, 21.8887, 22.8634, 23.9783, 23.9509, 24.8657, 24.8318, 25.9821, 26.4381, 26.8438, 26.8098, 27.6521, 28.095};
   double Ch9_LimVsFreq_Power[]   = {17.7196, 20.4322, 21.723,  22.6928, 23.8149, 23.8049, 24.6797, 24.6748, 25.8461, 26.3008, 26.6851, 26.6591, 27.5141, 27.9703};
   double Ch10_LimVsFreq_Power[]  = {17.4944, 20.2241, 21.5523, 22.495,  23.6359, 23.6079, 24.5149, 24.4828, 25.6655, 26.1328, 26.4768, 26.446,  27.3329, 27.8056};
   double Ch11_LimVsFreq_Power[]  = {17.4493, 20.1804, 21.4969, 22.4557, 23.5745, 23.5508, 24.466,  24.4384, 25.6069, 26.063,  26.4098, 26.3867, 27.2694, 27.7488};
   double Ch12_LimVsFreq_Power[]  = {17.3853, 20.0868, 21.4139, 22.3988, 23.49,   23.4687, 24.3968, 24.369,  25.5115, 25.9514, 26.3158, 26.2852, 27.166,  27.6408};
   double Ch13_LimVsFreq_Power[]  = {17.4944, 20.2021, 21.4859, 22.5199, 23.5662, 23.5408, 24.4845, 24.4617, 25.5476, 25.9613, 26.3499, 26.324,  27.1857, 27.6275};
   double Ch14_LimVsFreq_Power[]  = {17.8669, 20.5286, 21.7824, 22.869,  23.8643, 23.8568, 24.8191, 24.7902, 25.838,  26.2219, 26.6623, 26.6327, 27.4586, 27.8788};
   double Ch15_LimVsFreq_Power[]  = {18.059,  20.6865, 21.8711, 23.0342, 23.995,  23.9832, 24.9761, 24.9426, 25.9436, 26.2628, 26.7819, 26.7466, 27.5264, 27.8748};
   double Ch16_LimVsFreq_Power[]  = {18.2906, 20.8797, 22.0303, 23.2563, 24.1936, 24.1811, 25.1673, 25.1357, 26.1099, 26.4089, 26.9833, 26.9512, 27.6903, 28.0271};
   
   // HDET readings for 16 cal channels recorded at the PDMs listed in LimVsFreq_PDM
   int Ch1_LimVsFreq_HDET[]   = { 991, 1267, 1432, 1610, 1794, 1788, 2008, 2001, 2246, 2338, 2487, 2478, 2716, 2836};
   int Ch2_LimVsFreq_HDET[]   = {1027, 1310, 1474, 1671, 1849, 1849, 2077, 2071, 2312, 2399, 2565, 2559, 2793, 2902};
   int Ch3_LimVsFreq_HDET[]   = {1054, 1346, 1512, 1724, 1908, 1899, 2135, 2128, 2374, 2458, 2637, 2631, 2865, 2973};
   int Ch4_LimVsFreq_HDET[]   = {1057, 1357, 1523, 1735, 1925, 1919, 2158, 2151, 2396, 2484, 2670, 2668, 2894, 2994};
   int Ch5_LimVsFreq_HDET[]   = {1039, 1337, 1507, 1711, 1909, 1902, 2131, 2125, 2383, 2476, 2652, 2647, 2880, 2990};
   int Ch6_LimVsFreq_HDET[]   = {1006, 1296, 1468, 1657, 1859, 1856, 2069, 2069, 2330, 2436, 2591, 2583, 2823, 2940};
   int Ch7_LimVsFreq_HDET[]   = { 962, 1242, 1413, 1588, 1787, 1784, 1981, 1983, 2251, 2361, 2494, 2492, 2729, 2863};
   int Ch8_LimVsFreq_HDET[]   = { 921, 1183, 1353, 1505, 1708, 1705, 1888, 1887, 2155, 2275, 2383, 2380, 2628, 2763};
   int Ch9_LimVsFreq_HDET[]   = { 908, 1170, 1338, 1483, 1687, 1681, 1862, 1860, 2126, 2247, 2351, 2346, 2595, 2739};
   int Ch10_LimVsFreq_HDET[]  = { 895, 1149, 1319, 1459, 1659, 1657, 1836, 1829, 2096, 2220, 2310, 2306, 2555, 2697};
   int Ch11_LimVsFreq_HDET[]  = { 892, 1144, 1313, 1455, 1654, 1649, 1828, 1827, 2087, 2209, 2301, 2293, 2540, 2682};
   int Ch12_LimVsFreq_HDET[]  = { 903, 1159, 1330, 1481, 1676, 1674, 1861, 1858, 2116, 2237, 2330, 2327, 2574, 2720};
   int Ch13_LimVsFreq_HDET[]  = { 924, 1191, 1364, 1525, 1720, 1713, 1910, 1907, 2162, 2274, 2385, 2374, 2626, 2761};
   int Ch14_LimVsFreq_HDET[]  = { 951, 1228, 1402, 1585, 1776, 1771, 1980, 1971, 2228, 2333, 2457, 2457, 2699, 2832};
   int Ch15_LimVsFreq_HDET[]  = { 980, 1265, 1437, 1638, 1831, 1823, 2042, 2039, 2286, 2379, 2527, 2526, 2763, 2881};
   int Ch16_LimVsFreq_HDET[]  = { 991, 1280, 1450, 1660, 1850, 1846, 2067, 2066, 2309, 2392, 2560, 2552, 2775, 2884};

   // QMSL structures used to store Tx limiting vs. frequency calibration data
   QMSL_CDMA_TxLimVsFreq_struct TxLimVsFreqData;
   QMSL_CDMA_MeasuredChannels_struct TxLimVsFreqMeasuredChannels;

   // Copy the measured data to the QMSL structure
   for(i = 0; i < 14; i++)
   {
      TxLimVsFreqData.aiTxMeasuredHDET[0][i] = Ch1_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[0][i] = Ch1_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[0][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[1][i] = Ch2_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[1][i] = Ch2_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[1][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[2][i] = Ch3_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[2][i] = Ch3_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[2][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[3][i] = Ch4_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[3][i] = Ch4_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[3][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[4][i] = Ch5_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[4][i] = Ch5_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[4][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[5][i] = Ch6_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[5][i] = Ch6_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[5][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[6][i] = Ch7_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[6][i] = Ch7_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[6][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[7][i] = Ch8_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[7][i] = Ch8_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[7][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[8][i] = Ch9_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[8][i] = Ch9_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[8][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[9][i] = Ch10_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[9][i] = Ch10_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[9][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[10][i] = Ch11_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[10][i] = Ch11_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[10][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[11][i] = Ch12_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[11][i] = Ch12_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[11][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[12][i] = Ch13_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[12][i] = Ch13_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[12][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[13][i] = Ch14_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[13][i] = Ch14_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[13][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[14][i] = Ch15_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[14][i] = Ch15_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[14][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[15][i] = Ch16_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[15][i] = Ch16_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[15][i] = LimVsFreq_PDM[i];
   }

   for(ch = 0; ch < 16; ch++)
   {
      TxLimVsFreqMeasuredChannels.aiMeasuredChannels[ch] = MeasuredTxChannels[ch];
      TxLimVsFreqData.iNumOfTxLimMeasurement[ch] = 14;
   }

   TxLimVsFreqMeasuredChannels.iRefChannel = 9750;
   TxLimVsFreqMeasuredChannels.iNumMeasuredChannels = 16;

   // Note that the reference channel's measured data needs to be sorted from high to lower power
   for(i = 0; i < Tx2size; i++)
   {
      TxLimVsFreqData.oRefChMeas.aiTxAGC[i] = Tx2_PDM[i];
      TxLimVsFreqData.oRefChMeas.aiTxMeasuredHDET[i] = Tx2_HDET[i];
      TxLimVsFreqData.oRefChMeas.aiTxMeasuredPower[i] = Tx2_Power[i];   
      TxLimVsFreqData.oRefChMeas.iNumMeasurement = Tx2size;
   }

   double dDesiredMaxPower = 23;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes.
   QMSL_CDMA_Tx_LimVsFreq_NV_struct Tx_LimVsFreq_NV;

   bOk = QLIB_RFCAL_CDMA_Tx_LimVsFreq_Results(g_hResourceContext, 
                                    iNV_Mode_ID,
                                    dDesiredMaxPower,
                                    (void*)&TxLimVsFreqData,
                                    (void*)&TxLimVsFreqMeasuredChannels,
                                    (void*)&TX_NV_Channels,
                                    (void*)&Tx_LimVsFreq_NV,
                                    bCachedByNVManager);

   printf("QLIB_RFCAL_CDMA_Tx_LimVsFreq_Results: %s\n", bOk ? "PASS" : "FAIL");

   ///////////////////////
   // Rx Cal - Primary Rx
   ///////////////////////

   // Measured DVGA offset/LNA offset data for 16 calibration channels
   unsigned short DVGA[16] = {-198, -196, -196, -197, -196, -196, -195, -197, -196, -198, -197, -196, -196, -197, -199, -198};
   unsigned short LNA1[16] = {230, 235, 233, 232, 232, 233, 235, 232, 234, 233, 233, 236, 237, 237, 235, 236};
   unsigned short LNA2[16] = {269, 271, 271, 269, 270, 270, 270, 270, 270, 270, 270, 270, 271, 271, 268, 270};
   unsigned short LNA3[16] = {376, 381, 380, 381, 380, 380, 381, 380, 382, 376, 376, 380, 378, 376, 373, 374};

   // QMSL structures used to store DVGA offset/LNA offset vs. frequency calibration data
   QMSL_CDMA_RxFreq_Measurement_struct aRX_Measured;
   QMSL_CDMA_MeasuredChannels_struct RX_MeasuredChannels;

   // No IntelliCeiver, so only one power mode
   aRX_Measured.iNumMeasuredPowerModes = 1;

   // Copy measured data to the QMSL struture
   for(ch = 0; ch < 16; ch++)
   {
      aRX_Measured.aiDVGA_Measured[ch][0] = DVGA[ch];
      aRX_Measured.aiLNA_Measured[ch][0][0] = LNA1[ch];
      aRX_Measured.aiLNA_Measured[ch][1][0] = LNA2[ch];
      aRX_Measured.aiLNA_Measured[ch][2][0] = LNA3[ch];
   }

   aRX_Measured.iNumMeasured_LNA_States = 3;

   // aRX_Measured.aiLNA_States[n] = y
   // where n is the index to use in aRX_Measured.aiLNA_Measured[ch][n][0] 
   // and y is the corresponding LNA gain state index
   aRX_Measured.aiLNA_States[0] =  1;
   aRX_Measured.aiLNA_States[1] =  2;
   aRX_Measured.aiLNA_States[2] =  3;

   // Primary Rx
   aRX_Measured.iPath = 0;

   // Fill out the channels at which the measurement were made
   for(ch = 0; ch < 16; ch++)
   {
      RX_MeasuredChannels.aiMeasuredChannels[ch] = MeasuredRxChannels[ch];
      RX_NV_Channels.aiNVChannels[ch] = MeasuredRxChannels[ch];
   }
   
   RX_MeasuredChannels.iNumMeasuredChannels = 16;
   RX_MeasuredChannels.iRefChannel = 10700;

   unsigned char iIsIntelliCeiver = 0;
   double dDVGAOffsetScalingFactor = 1;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes.
   QMSL_CDMA_Rx_DVGA_LNA_Cal_NV_Result RX_NV_Result;

   bOk = QLIB_RFCAL_CDMA_RxCal_Results(g_hResourceContext, 
                               iNV_Mode_ID,
                               iIsIntelliCeiver, 
                               dDVGAOffsetScalingFactor,
                               (void*)&aRX_Measured, 
                               (void*)&RX_MeasuredChannels, 
                               (void*)&RX_NV_Channels, 
                               (void*)&RX_NV_Result,
                               bWriteNVChList,
                               bCachedByNVManager); 

   printf("QLIB_RFCAL_CDMA_RxCal_Results (Primary Rx): %s\n", bOk ? "PASS" : "FAIL");

   ///////////////////////
   // Rx Cal - Diversity Rx
   ///////////////////////

   // Measured DVGA offset/LNA offset data for 16 calibration channels
   unsigned short RX1_DVGA[16] = {-203, -203, -201, -200, -198, -196, -197, -196, -196, -194, -194, -195, -198, -202, -208, -213};
   unsigned short RX1_LNA1[16] = {266, 265, 262, 258, 255, 255, 256, 258, 258, 259, 258, 259, 261, 264, 266, 265};
   unsigned short RX1_LNA2[16] = {317, 315, 311, 308, 306, 307, 308, 310, 310, 311, 310, 311, 314, 316, 318, 316};
   unsigned short RX1_LNA3[16] = {420, 418, 415, 414, 416, 416, 416, 417, 417, 418, 417, 418, 419, 420, 420, 417};

   // Diversity Rx
   aRX_Measured.iPath = 1;

   // Copy measured data to the QMSL struture
   for(ch = 0; ch < 16; ch++)
   {
      aRX_Measured.aiDVGA_Measured[ch][0] = RX1_DVGA[ch];
      aRX_Measured.aiLNA_Measured[ch][0][0] = RX1_LNA1[ch];
      aRX_Measured.aiLNA_Measured[ch][1][0] = RX1_LNA2[ch];
      aRX_Measured.aiLNA_Measured[ch][2][0] = RX1_LNA3[ch];
   }

   bOk = QLIB_RFCAL_CDMA_RxCal_Results(g_hResourceContext, 
                               iNV_Mode_ID,
                               iIsIntelliCeiver, 
                               dDVGAOffsetScalingFactor,
                               (void*)&aRX_Measured, 
                               (void*)&RX_MeasuredChannels, 
                               (void*)&RX_NV_Channels, 
                               (void*)&RX_NV_Result,
                               bWriteNVChList,
                               bCachedByNVManager); 

   printf("QLIB_RFCAL_CDMA_RxCal_Results (Diversity Rx): %s\n", bOk ? "PASS" : "FAIL");

   return 1;
}

unsigned char LTE_RTR8600_Calibration( void )
{
   printf("LTE_RTR8600_Calibration:\n");
   unsigned char bOk = true;

   // LTE Band 13
   unsigned short iNV_Mode_ID = NV_MODE_LTE_B13;

   // Store generated NV items in NV Manager. NV items stored in NV Manager will be 
   // written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone is called.
   unsigned char  bCachedByNVManager = 1; 
   
   // Write NV cal channels
   unsigned char bWriteNVChList = 1; 
   
   // Measured channel lists
   unsigned short MeasuredTxChannels[16] = {23184,23190,23196,23202,23208,23214,23220,23226,23230,23238,23244,23250,23256,23262,23268,23274};
   unsigned short MeasuredRxChannels[16] = {5184,5190,5196,5202,5208,5214,5220,5226,5230,5238,5244,5250,5256,5262,5268,5274};

   // Fill out the NV cal channel lists
   QMSL_CDMA_NVChannels_struct TX_NV_Channels;
   QMSL_CDMA_NVChannels_struct RX_NV_Channels;

   int i, ch;
   for(ch = 0; ch < 16; ch++)
   {
      TX_NV_Channels.aiNVChannels[ch] = MeasuredTxChannels[ch];
      RX_NV_Channels.aiNVChannels[ch] = MeasuredRxChannels[ch];
   }

   ////////////////////
   // Tx Linearizer
   ////////////////////
   const int Tx0size = 55;
   const int Tx1size = 54;
   const int Tx2size = 60;

   // Measured data for Tx Linearizer
   int      Tx0_PDM[]   = {54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
   double   Tx0_Power[]   = {9.26466, 8.55292, 6.97142, 5.55146, 4.70082, 4.14775, 1.3719, 0.608613, 0.563433, -1.27873, -3.05997, -3.71543, -3.71517, -4.56593, -5.34205, -6.23897, -7.5115, -8.47642, -9.4116, -10.5826, -12.5974, -15.1344, -16.2782, -17.5104, -19.2931, -20.4685, -21.4622, -22.645, -23.5163, -24.4233, -25.6016, -26.8455, -27.7697, -28.3625, -29.741, -30.5303, -31.5016, -32.1328, -33.1921, -34.3754, -35.3381, -36.3999, -37.2664, -40.6268, -41.5582, -43.416, -44.1569, -45.2909, -47.2797, -48.4603, -49.628, -50.0639, -51.3377, -53.2398, -55.4791};

   int      Tx1_PDM[]   = {54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
   double   Tx1_Power[]   = {19.196, 18.6197, 17.2734, 16.0239, 15.2758, 14.7479, 12.3916, 11.6802, 11.4531, 9.92815, 8.25821, 7.64957, 7.65398, 6.83792, 6.07846, 5.20278, 3.96986, 3.018, 2.09133, 0.921869, -1.0764, -3.59783, -4.75293, -5.97529, -7.75984, -8.93667, -9.91993, -11.1039, -11.9728, -12.8697, -14.0495, -15.2838, -16.2134, -16.8024, -18.1847, -18.9922, -19.9601, -20.5987, -21.6572, -22.8347, -23.7942, -24.8546, -25.7202, -29.0897, -30.0145, -31.8804, -32.6084, -33.7399, -35.7501, -36.9258, -38.0969, -38.5725, -39.8115, -41.7077};

   int      Tx2_PDM[]      = {60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
   double   Tx2_Power[]      = {31.7821, 31.387, 31.1398, 30.1347, 29.6095, 29.393, 28.8746, 28.2833, 26.8893, 25.6077, 24.8628, 24.1848, 21.9904, 21.2443, 20.6033, 19.4271, 17.7022, 17.0894, 17.109, 16.27, 15.4924, 14.596, 13.3277, 12.3769, 11.4376, 10.2466, 8.25882, 5.74298, 4.61752, 3.40039, 1.59887, 0.417586, -0.543652, -1.72436, -2.57799, -3.45058, -4.64251, -5.87278, -6.8026, -7.38457, -8.77028, -9.54511, -10.517, -11.1459, -12.2073, -13.3854, -14.34, -15.4103, -16.2705, -19.6293, -20.5738, -22.4441, -23.159, -24.2921, -26.2923, -27.482, -28.653, -29.124, -30.3664, -32.2601};
   int      Tx2_HDET[]      = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 3594, 3051, 2811, 2613, 2061, 1929, 1791, 1589, 1349, 1266, 1268, 1171, 1099, 1011, 915, 856, 803, 748, 682, 628, 611, 597, 582, 575, 570, 566, 563, 562, 561, 558, 556, 557, 554, 553, 554, 553, 555, 553, 553, 554, 554, 553, 554, 554, 554, 553, 554, 554, 554, 553, 553, 554};
   int      Tx2_LPMHDET[]   = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4036, 3653, 3152, 2787, 2488, 2161, 1732, 1365, 1248, 1145, 1042, 988, 956, 927, 908, 896, 884, 868, 861, 858, 849, 845, 840, 840, 843, 838, 836, 835, 835, 834, 837, 837, 836, 836, 837, 835, 834, 833, 835, 836};

   // QMSL structure used to store Tx linearizer calibration data
   QMSL_CDMA_TxLin_BetaScaling_struct TxLinData;

   // Copy the measured data to the QMSL structure
   // Low gain Tx linearizer
   for(i = 0; i < Tx0size; i++)
   {
      TxLinData.aiTxLinPower[0][i] = Tx0_Power[i];
      TxLinData.aiTxLinPDM[0][i] = Tx0_PDM[i];
   }

   // Mid gain Tx linearizer
   for(i = 0; i < Tx1size; i++)
   {
      TxLinData.aiTxLinPower[1][i] = Tx1_Power[i];
      TxLinData.aiTxLinPDM[1][i] = Tx1_PDM[i];
   }

   // High gain Tx linearizer
   for(i = 0; i < Tx2size; i++)
   {
      TxLinData.aiTxLinPower[2][i] = Tx2_Power[i];
      TxLinData.aiTxLinPDM[2][i] = Tx2_PDM[i];
      TxLinData.aiTxHDET[i] = Tx2_HDET[i];
      TxLinData.aiTxHDETLPM[i] = Tx2_LPMHDET[i];
   }

    // Using LPM HDET
   TxLinData.aiTxHDETLPMEnabled = 1;

   TxLinData.iNumMeasuredTxGainStates = 3; 
   TxLinData.iNumTxPowerMeasurementsPerGainState[0] = Tx0size;
   TxLinData.iNumTxPowerMeasurementsPerGainState[1] = Tx1size;
   TxLinData.iNumTxPowerMeasurementsPerGainState[2] = Tx2size;
   
   // TxLinData.aiTxMeasToNVMapping[n] = y
   // where n is the Tx gain state index to use in TxLinData.aiTxLinPower[n][i] 
   // and y is the index of the corresponding NV table
   TxLinData.aiTxMeasToNVMapping[0] = 3;
   TxLinData.aiTxMeasToNVMapping[1] = 2;
   TxLinData.aiTxMeasToNVMapping[2] = 0;

   TxLinData.aiHeadRoomPDMCountList[0] = 128;
   TxLinData.aiHeadRoomPDMCountList[1] = 128;
   TxLinData.aiHeadRoomPDMCountList[2] = 128;
   TxLinData.aiExtrapolation_PDM_StepSize[0] = 6;
   TxLinData.aiExtrapolation_PDM_StepSize[1] = 6;
   TxLinData.aiExtrapolation_PDM_StepSize[2] = 6;

   //TxLinData.aiTxLinPower[2] is used to generate HDET vs AGC table
   TxLinData.iTxMeasIndexUsedForHDET = 2;
   TxLinData.HDET_OffsetPower_dBm = 17;
   TxLinData.HDET_SpanPower_dBm  =  25;

   TxLinData.LPM_HDET_OffsetPower_dBm = 17;
   TxLinData.LPM_HDET_Start_Power_dBm = 16;
   TxLinData.LPM_HDET_SpanPower_dBm = 25;

   TxLinData.aiTxSweepStepSize[0] = 1;
   TxLinData.aiTxSweepStepSize[1] = 1;
   TxLinData.aiTxSweepStepSize[2] = 1;

   TxLinData.iUMTS_TxAGC_PDM_Max = 127;
   TxLinData.i_TxAGC_PDM_MinGain = 0;
   TxLinData.iNV_Resolution = 10;

   TxLinData.iEnh_Linearizer_0_NV = 0;
   TxLinData.iSkip_NonMonotonic_Items = 1;
   TxLinData.iDo_Not_Extrapolate_Lower_Powers = 0;
   TxLinData.iUse_HDET_V2 = 0;
   TxLinData.iHDET_MaxLim = 4095;

   // Sort by power
   TxLinData.iSortingOrder = 0;

   // Sorting order = descending 
   TxLinData.iSortNVTxLin = 0;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes.
   QMSL_CDMA_TxLin_BetaScaling_Result_struct aTx_Result;   

   bOk = QLIB_RFCAL_CDMA_TxLinCal_BetaScaling_Results(g_hResourceContext, 
                                          iNV_Mode_ID,
                                          &TxLinData, 
                                          &aTx_Result,
                                          bCachedByNVManager);

   printf("QLIB_RFCAL_CDMA_TxLinCal_BetaScaling_Results: %s\n", bOk ? "PASS" : "FAIL");


   ////////////////////
   // Tx Lin vs. Freq
   ////////////////////

   // Measured data for Tx frequency compensation (measured Tx power across frequency for the same PDM)
   double   Tx0_FreqComp[16] = {-0.750093, -0.464186, -0.330553, -0.322082, -0.574583, -0.785764, -0.999583, -1.33626, -1.53739, -1.73025, -1.82509, -1.83165, -1.70184, -1.53357, -1.26942, -1.04347};
   double   Tx1_FreqComp[16] = {11.5201, 11.5981, 11.5057, 11.2934, 10.8815, 10.5348, 10.2669, 9.93866, 9.73235, 9.63843, 9.62782, 9.74807, 9.99328, 10.2806, 10.5602, 10.7221};
   double   Tx2_FreqComp[16] = {10.1542, 10.5006, 10.7789, 10.9271, 10.8614, 10.8468, 10.8274, 10.6572, 10.4901, 10.2855, 10.0578, 9.90083, 9.83211, 9.83563, 9.94263, 10.0658};

   // QMSL structures used to store Tx linearizer vs. frequency calibration data
   QMSL_CDMA_TxLinVsFreq_struct TxLinVsFreq;
   QMSL_CDMA_MeasuredChannels_struct TxMeasuredChs;

   // Copy the measured data to the QMSL structure
   for(ch = 0; ch < 16; ch++)
   {
      TxLinVsFreq.aiTxMeasuredPower[0][ch] = Tx0_FreqComp[ch];
      TxLinVsFreq.aiTxMeasuredPower[1][ch] = Tx1_FreqComp[ch];
      TxLinVsFreq.aiTxMeasuredPower[2][ch] = Tx2_FreqComp[ch];
   }

   // TxLinVsFreq.aiTxMeasToNVMapping[n] = y
   // where n is the Tx gain state index to use in TxLinVsFreq.aiTxMeasuredPower[n][i] 
   // and y is the index of the corresponding NV table
   TxLinVsFreq.aiTxMeasToNVMapping[0] = 3;
   TxLinVsFreq.aiTxMeasToNVMapping[1] = 2;
   TxLinVsFreq.aiTxMeasToNVMapping[2] = 0;

   TxLinVsFreq.iNumMeasuredTxGainStates = 3;
   
   // Primary Tx comp only (no secondary Tx comp)
   TxLinVsFreq.iTxCompType = 0;
   TxLinVsFreq.a2ndTxCompHasMeas[0] = 0;
   TxLinVsFreq.a2ndTxCompHasMeas[1] = 0;
   TxLinVsFreq.a2ndTxCompHasMeas[2] = 0;
   TxLinVsFreq.a2ndTxCompHasMeas[3] = 0;

   // Copy the measurement channels to QMSL structure
   for(ch = 0; ch < 16; ch++)
   {
      TxMeasuredChs.aiMeasuredChannels[ch] = MeasuredTxChannels[ch];
   }

   TxMeasuredChs.iRefChannel = 23230;
   TxMeasuredChs.iNumMeasuredChannels = 16;

   int iNV_Resolution_Type = 10;

   // Generate NV_<bandclass>_TX_COMP_VS_FREQ_<x>_I items
   int iNV_Tx_Comp_Type = QMSL_NV_TX_COMP;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes.
   QMSL_CDMA_Tx_LinVsFreq_Cal_NV_Result_struct TxLinVsFreq_NV;   

   bOk = QLIB_RFCAL_CDMA_Tx_LinVsFreq_Results(g_hResourceContext, 
                                    iNV_Mode_ID,
                                    (void*)&TxLinVsFreq, 
                                    (void*)&TxMeasuredChs, 
                                    (void*)&TX_NV_Channels,
                                    iNV_Resolution_Type,
                                    iNV_Tx_Comp_Type,
                                    (void*)&TxLinVsFreq_NV,
                                    bWriteNVChList,
                                    bCachedByNVManager);

   printf("QLIB_RFCAL_CDMA_Tx_LinVsFreq_Results: %s\n", bOk ? "PASS" : "FAIL");

   ////////////////////
   // Tx Lim vs. Freq
   ////////////////////

   // Measurement setup data for Tx Limiting vs Frequency 
   // (list of PDM values used to take datapoints near maximum Tx power across frequency)
   int   LimVsFreq_PDM[] = {45, 47, 48, 49, 51, 52};

   // Power measured for 16 cal channels at the PDMs listed in LimVsFreq_PDM
   double Ch1_LimVsFreq_Power[]   = {19.4723, 21.1619, 21.8063, 23.3633, 24.5193, 25.6957};
   double Ch2_LimVsFreq_Power[]   = {19.7888, 21.4396, 22.0935, 23.5241, 24.7806, 25.9284};
   double Ch3_LimVsFreq_Power[]   = {20.0070, 21.6636, 22.3143, 23.8065, 25.0593, 26.2138};
   double Ch4_LimVsFreq_Power[]   = {20.1149, 21.7893, 22.4348, 23.9775, 25.2620, 26.4125};
   double Ch5_LimVsFreq_Power[]   = {20.0275, 21.6995, 22.3589, 24.0353, 25.3178, 26.5004};
   double Ch6_LimVsFreq_Power[]   = {20.0229, 21.6963, 22.3429, 24.1584, 25.4654, 26.6578};
   double Ch7_LimVsFreq_Power[]   = {20.0320, 21.7322, 22.3767, 24.2797, 25.6039, 26.8198};
   double Ch8_LimVsFreq_Power[]   = {19.8930, 21.6102, 22.2743, 24.3163, 25.6577, 26.8656};
   double Ch9_LimVsFreq_Power[]   = {19.7718, 21.5038, 22.1696, 24.2878, 25.6227, 26.8627};
   double Ch10_LimVsFreq_Power[]  = {19.6527, 21.4016, 22.0694, 24.3457, 25.7109, 26.9647};
   double Ch11_LimVsFreq_Power[]  = {19.4814, 21.2305, 21.9094, 24.3483, 25.7204, 26.9844};
   double Ch12_LimVsFreq_Power[]  = {19.3205, 21.1050, 21.8012, 24.3467, 25.7056, 26.9687};
   double Ch13_LimVsFreq_Power[]  = {19.2556, 21.0741, 21.7452, 24.3381, 25.7128, 26.9653};
   double Ch14_LimVsFreq_Power[]  = {19.2339, 21.0274, 21.7269, 24.2872, 25.6751, 26.9355};
   double Ch15_LimVsFreq_Power[]  = {19.3146, 21.0940, 21.8169, 24.2565, 25.6126, 26.8620};
   double Ch16_LimVsFreq_Power[]  = {19.4105, 21.1691, 21.8607, 24.1855, 25.5438, 26.7993};
   
   // HDET readings for 16 cal channels recorded at the PDMs listed in LimVsFreq_PDM
   int Ch1_LimVsFreq_HDET[]   = {1631, 1945, 2095, 2465, 2823, 3219};
   int Ch2_LimVsFreq_HDET[]   = {1658, 1996, 2120, 2485, 2848, 3245};
   int Ch3_LimVsFreq_HDET[]   = {1679, 2003, 2146, 2512, 2890, 3307};
   int Ch4_LimVsFreq_HDET[]   = {1674, 2004, 2155, 2559, 2933, 3350};
   int Ch5_LimVsFreq_HDET[]   = {1664, 2000, 2143, 2578, 2959, 3410};
   int Ch6_LimVsFreq_HDET[]   = {1659, 1993, 2144, 2616, 3007, 3476};
   int Ch7_LimVsFreq_HDET[]   = {1647, 1973, 2120, 2618, 3029, 3483};
   int Ch8_LimVsFreq_HDET[]   = {1637, 1963, 2110, 2657, 3048, 3520};
   int Ch9_LimVsFreq_HDET[]   = {1620, 1950, 2102, 2650, 3068, 3560};
   int Ch10_LimVsFreq_HDET[]  = {1593, 1913, 2062, 2653, 3070, 3560};
   int Ch11_LimVsFreq_HDET[]  = {1558, 1884, 2017, 2636, 3074, 3555};
   int Ch12_LimVsFreq_HDET[]  = {1546, 1865, 2002, 2633, 3074, 3533};
   int Ch13_LimVsFreq_HDET[]  = {1517, 1831, 1972, 2590, 3042, 3509};
   int Ch14_LimVsFreq_HDET[]  = {1509, 1821, 1965, 2563, 2998, 3484};
   int Ch15_LimVsFreq_HDET[]  = {1518, 1836, 1972, 2546, 3004, 3444};
   int Ch16_LimVsFreq_HDET[]  = {1549, 1857, 2002, 2578, 2990, 3468};

   // QMSL structures used to store Tx limiting vs. frequency calibration data
   QMSL_CDMA_TxLimVsFreq_struct TxLimVsFreqData;
   QMSL_CDMA_MeasuredChannels_struct TxLimVsFreqMeasuredChannels;

   // Copy the measured data to the QMSL structure
   for(i = 0; i < 6; i++)
   {
      TxLimVsFreqData.aiTxMeasuredHDET[0][i] = Ch1_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[0][i] = Ch1_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[0][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[1][i] = Ch2_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[1][i] = Ch2_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[1][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[2][i] = Ch3_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[2][i] = Ch3_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[2][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[3][i] = Ch4_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[3][i] = Ch4_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[3][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[4][i] = Ch5_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[4][i] = Ch5_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[4][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[5][i] = Ch6_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[5][i] = Ch6_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[5][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[6][i] = Ch7_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[6][i] = Ch7_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[6][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[7][i] = Ch8_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[7][i] = Ch8_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[7][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[8][i] = Ch9_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[8][i] = Ch9_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[8][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[9][i] = Ch10_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[9][i] = Ch10_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[9][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[10][i] = Ch11_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[10][i] = Ch11_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[10][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[11][i] = Ch12_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[11][i] = Ch12_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[11][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[12][i] = Ch13_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[12][i] = Ch13_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[12][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[13][i] = Ch14_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[13][i] = Ch14_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[13][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[14][i] = Ch15_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[14][i] = Ch15_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[14][i] = LimVsFreq_PDM[i];

      TxLimVsFreqData.aiTxMeasuredHDET[15][i] = Ch16_LimVsFreq_HDET[i];
      TxLimVsFreqData.aiTxMeasuredPower[15][i] = Ch16_LimVsFreq_Power[i];
      TxLimVsFreqData.aiTxMeasuredPDM[15][i] = LimVsFreq_PDM[i];
   }

   for(ch = 0; ch < 16; ch++)
   {
      TxLimVsFreqMeasuredChannels.aiMeasuredChannels[ch] = MeasuredTxChannels[ch];
      TxLimVsFreqData.iNumOfTxLimMeasurement[ch] = 6;
   }

   TxLimVsFreqMeasuredChannels.iRefChannel = 23230;
   TxLimVsFreqMeasuredChannels.iNumMeasuredChannels = 16;

   // Note that the reference channel's measured data needs to be sorted from high to lower power
   for(i = 0; i < Tx2size; i++)
   {
      TxLimVsFreqData.oRefChMeas.aiTxAGC[i] = Tx2_PDM[i];
      TxLimVsFreqData.oRefChMeas.aiTxMeasuredHDET[i] = Tx2_HDET[i];
      TxLimVsFreqData.oRefChMeas.aiTxMeasuredPower[i] = Tx2_Power[i];   
      TxLimVsFreqData.oRefChMeas.iNumMeasurement = Tx2size;
   }

   double dDesiredMaxPower = 23;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes.
   QMSL_CDMA_Tx_LimVsFreq_NV_struct Tx_LimVsFreq_NV;

   bOk = QLIB_RFCAL_CDMA_Tx_LimVsFreq_Results(g_hResourceContext, 
                                    iNV_Mode_ID,
                                    dDesiredMaxPower,
                                    (void*)&TxLimVsFreqData,
                                    (void*)&TxLimVsFreqMeasuredChannels,
                                    (void*)&TX_NV_Channels,
                                    (void*)&Tx_LimVsFreq_NV,
                                    bCachedByNVManager);

   printf("QLIB_RFCAL_CDMA_Tx_LimVsFreq_Results: %s\n", bOk ? "PASS" : "FAIL");


   ///////////////////////
   // Rx Cal - Primary Rx
   ///////////////////////

   // Measured LNA offset data for 16 calibration channels
   unsigned short LNA0[16] = { -7,  -6,  -5,  -5,  -5,  -5,  -6, -6, -7,  -8,  -9,  -9, -10,  -9,  -9,  -7};
   unsigned short LNA1[16] = { -8,  -7,  -6,  -6,  -6,  -6,  -7, -7, -8, -10, -10, -10, -11, -10, -10,  -8};
   unsigned short LNA2[16] = { -9,  -8,  -7,  -6,  -6,  -7,  -8, -8, -8, -10, -11, -11, -11, -10, -11,  -9};
   unsigned short LNA3[16] = {-10,  -9,  -8,  -8,  -8,  -8,  -9, -8, -9, -10, -11, -10, -11, -11, -11, -10};
   unsigned short LNA4[16] = { -3,  -2,  -1,  -1,   0,  -1,  -1,  0,  0,  -1,  -2,  -2,  -4,  -3,  -5,  -3};
   unsigned short LNA5[16] = {-12, -11, -10, -10, -10, -10, -10, -9, -9, -10, -10, -10, -12, -12, -14, -13};

   // QMSL structures used to store LNA offset vs. frequency calibration data
   QMSL_CDMA_RxFreq_Measurement_struct aRX_Measured;
   QMSL_CDMA_MeasuredChannels_struct RX_MeasuredChannels;

   // No IntelliCeiver, so only one power mode
   aRX_Measured.iNumMeasuredPowerModes = 1;
   
   // Copy measured data to the QMSL struture
   for(ch = 0; ch < 16; ch++)
   {
      aRX_Measured.aiLNA_Measured[ch][0][0] = LNA0[ch];
      aRX_Measured.aiLNA_Measured[ch][1][0] = LNA1[ch];
      aRX_Measured.aiLNA_Measured[ch][2][0] = LNA2[ch];
      aRX_Measured.aiLNA_Measured[ch][3][0] = LNA3[ch];
      aRX_Measured.aiLNA_Measured[ch][4][0] = LNA4[ch];
      aRX_Measured.aiLNA_Measured[ch][5][0] = LNA5[ch];
   }

   aRX_Measured.iNumMeasured_LNA_States = 6;

   // aRX_Measured.aiLNA_States[n] = y
   // where n is the index to use in aRX_Measured.aiLNA_Measured[ch][n][0] 
   // and y is the corresponding LNA gain state index
   aRX_Measured.aiLNA_States[0] = 0;
   aRX_Measured.aiLNA_States[1] = 1;
   aRX_Measured.aiLNA_States[2] = 2;
   aRX_Measured.aiLNA_States[3] = 3;
   aRX_Measured.aiLNA_States[4] = 4;
   aRX_Measured.aiLNA_States[5] = 5;

   // Primary Rx
   aRX_Measured.iPath = 0;

   // Fill out the channels at which the measurement were made
   for(ch = 0; ch < 16; ch++)
   {
      RX_MeasuredChannels.aiMeasuredChannels[ch] = MeasuredRxChannels[ch];
   }
   
   RX_MeasuredChannels.iNumMeasuredChannels = 16;
   RX_MeasuredChannels.iRefChannel = 5230;

   unsigned char iIsIntelliCeiver = 0;
   double dDVGAOffsetScalingFactor = 1;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes.
   QMSL_CDMA_Rx_DVGA_LNA_Cal_NV_Result RX_NV_Result;

   bOk = QLIB_RFCAL_CDMA_RxCal_Results(g_hResourceContext, 
                               iNV_Mode_ID,
                               iIsIntelliCeiver, 
                               dDVGAOffsetScalingFactor,
                               (void*)&aRX_Measured, 
                               (void*)&RX_MeasuredChannels, 
                               (void*)&RX_NV_Channels, 
                               (void*)&RX_NV_Result,
                               bWriteNVChList,
                               bCachedByNVManager); 

   printf("QLIB_RFCAL_CDMA_RxCal_Results (Primary Rx): %s\n", bOk ? "PASS" : "FAIL");

   /////////////////////////
   // Rx Cal - Diversity Rx
   /////////////////////////

   // Measured LNA offset data for 16 calibration channels
   unsigned short RX1_LNA0[16] = { -7,  -8,  -9,  -9, -10,  -9,  -9,  -9,  -8, -6, -6, -5, -5, -5, -5, -5};
   unsigned short RX1_LNA1[16] = { -8,  -9, -10, -10, -11, -10, -10, -10, -10, -7, -7, -6, -6, -6, -6, -6};
   unsigned short RX1_LNA2[16] = { -8,  -9, -11, -11, -12, -11, -11, -10,  -9, -8, -8, -7, -6, -7, -6, -7};
   unsigned short RX1_LNA3[16] = {  0,  -1,  -2,  -2,  -3,  -3,  -3,  -3,  -3, -1, -1,  0,  1,  1,  2,  1};
   unsigned short RX1_LNA4[16] = { 12,  11,  10,   9,   8,   7,   7,   6,   6,  8,  9, 10, 11, 11, 13, 13};
   unsigned short RX1_LNA5[16] = {  5,   4,   3,   3,   1,   0,   0,  -2,  -1,  0,  1,  2,  4,  4,  6,  6};

   // Copy measured data to the QMSL struture
   for(ch = 0; ch < 16; ch++)
   {
      aRX_Measured.aiLNA_Measured[ch][0][0] = RX1_LNA0[ch];
      aRX_Measured.aiLNA_Measured[ch][1][0] = RX1_LNA1[ch];
      aRX_Measured.aiLNA_Measured[ch][2][0] = RX1_LNA2[ch];
      aRX_Measured.aiLNA_Measured[ch][3][0] = RX1_LNA3[ch];
      aRX_Measured.aiLNA_Measured[ch][4][0] = RX1_LNA4[ch];
      aRX_Measured.aiLNA_Measured[ch][5][0] = RX1_LNA5[ch];
   }

   // Diversity Rx
   aRX_Measured.iPath = 1;

   bOk = QLIB_RFCAL_CDMA_RxCal_Results(g_hResourceContext, 
                               iNV_Mode_ID,
                               iIsIntelliCeiver, 
                               dDVGAOffsetScalingFactor,
                               (void*)&aRX_Measured, 
                               (void*)&RX_MeasuredChannels, 
                               (void*)&RX_NV_Channels, 
                               (void*)&RX_NV_Result,
                               bWriteNVChList,
                               bCachedByNVManager); 

   printf("QLIB_RFCAL_CDMA_RxCal_Results (Diversity Rx): %s\n", bOk ? "PASS" : "FAIL");

   return 1;
}

unsigned char Enhanced_Sweep_Calibration_NV_Generation(void)
{


   // Sample FTM Request/ Response
   //------------- Linearizer---------------------
   // PA States : 0 , 1
   // Ref Channels : 200 , 358 , 600
   // Upper Bound Channels: 300, 500 , 799
   // Tx Purpose : Voice , SC , FreqComp
   //------------- Freq Comp ---------------------
   // Chan list : 16 channels
   // PA States : 0 , 1
   // RGI Threshold : 60 , 128
   // 2 sets of freq comp
   unsigned char bOk = false;
   unsigned short numRecords = 88;

   FTM_Cal_Sweep_Request ftm_request[1000];
   FTM_Cal_Sweep_Response ftm_response[1000];
   QMSL_Tx_Cal_Complete_NV_Type aTx_NV_Structure;
   QMSL_Tx_Cal_NV_Gen_Info aTx_Lin_Cal_NV_Info;
   int i = 0 ;
   byte rgi_ctr = 50;
   word tx_pwr_ctr = 450;
   // PA State 0 , Ch 200 , Voice , 10 entries
   for ( i=0; i <10 ; i++ )
   {
      ftm_request[i].paState = 0;
      ftm_request[i].paBias = 780;
      ftm_request[i].rgi = rgi_ctr;
      rgi_ctr--;
      ftm_request[i].ulChan = 200;
      ftm_request[i].txPurpose = 0x1;
      ftm_response[i].aiTxPower = tx_pwr_ctr;
      tx_pwr_ctr++;
   }
   // PA State 0 , Ch 563 , Voice , 10 entries
   rgi_ctr= 80;
   tx_pwr_ctr = 550;
   for ( i=10; i <20 ; i++ )
   {
      ftm_request[i].paState = 0;
      ftm_request[i].paBias = 780;
      ftm_request[i].rgi = rgi_ctr;
      rgi_ctr++;
      ftm_request[i].ulChan = 563;
      ftm_request[i].txPurpose = 0x1;
      ftm_response[i].aiTxPower = tx_pwr_ctr;
      tx_pwr_ctr++;
   }
   // PA State 0 , Ch 563 , SC , 10 entries
   rgi_ctr= 85;
   tx_pwr_ctr = 600;
   for ( i=20; i <30 ; i++ )
   {
      ftm_request[i].paState = 0;
      ftm_request[i].paBias = 780;
      ftm_request[i].rgi = rgi_ctr;
      rgi_ctr++;
      ftm_request[i].ulChan = 563;
      ftm_request[i].txPurpose = 0x2;
      ftm_response[i].aiTxPower = tx_pwr_ctr;
      tx_pwr_ctr++;
   }
   // PA State 1 , Ch 400 , Voice , 10 entries
   rgi_ctr= 90;
   tx_pwr_ctr = 650;
   for ( i=30; i <40 ; i++ )
   {
      ftm_request[i].paState = 1;
      ftm_request[i].paBias = 780;
      ftm_request[i].rgi = rgi_ctr;
      rgi_ctr++;
      ftm_request[i].ulChan = 400;
      ftm_request[i].txPurpose = 0x1;
      ftm_response[i].aiTxPower = tx_pwr_ctr;
      tx_pwr_ctr++;
   }
   // PA State 0 , Freq Comp , Ch 200, 230, 260...  , RGI Thresh: 80
   rgi_ctr= 90;
   tx_pwr_ctr = 40;
   word ch_ctr = 200;
   for ( i=40; i <56 ; i++ )
   {
      ftm_request[i].paState = 0;
      ftm_request[i].paBias = 780;
      ftm_request[i].rgi = 70;
      ftm_request[i].ulChan = ch_ctr;
      ch_ctr = ch_ctr + 30;
      ftm_request[i].txPurpose = 0x4;
      ftm_response[i].aiTxPower = tx_pwr_ctr;
      tx_pwr_ctr = tx_pwr_ctr + 5 ;
   }
   // PA State 0 , Freq Comp , Ch 200, 230, 260...  , RGI Thresh: 128
   rgi_ctr= 90;
   tx_pwr_ctr = 41;
   ch_ctr = 200;
   for ( i=56; i <72 ; i++ )
   {
      ftm_request[i].paState = 0;
      ftm_request[i].paBias = 780;
      ftm_request[i].rgi = 80;
      ftm_request[i].ulChan = ch_ctr;
      ch_ctr = ch_ctr + 30;
      ftm_request[i].txPurpose = 0x4;
      ftm_response[i].aiTxPower = tx_pwr_ctr;
      tx_pwr_ctr = tx_pwr_ctr + 5 ;
   }

   // PA State 1 , Freq Comp , Ch 300,330, 360...  , RGI Thresh: 128
   rgi_ctr= 90;
   tx_pwr_ctr = 42;
   ch_ctr = 200;
   int freq_comp_ctr = 0 ;
   for ( i=72; i <88 ; i++ )
   {
      ftm_request[i].paState = 1;
      ftm_request[i].paBias = 780;
      ftm_request[i].rgi = 80;
      ftm_request[i].ulChan = ch_ctr;
      aTx_Lin_Cal_NV_Info.freq_comp_ch_list[freq_comp_ctr] = ch_ctr;
      freq_comp_ctr++;
      ch_ctr = ch_ctr + 30;
      ftm_request[i].txPurpose = 0x4;
      ftm_response[i].aiTxPower = tx_pwr_ctr;
      tx_pwr_ctr = tx_pwr_ctr + 5 ;
   }
   aTx_Lin_Cal_NV_Info.freq_comp_ref_ch_list[0] = 290;
   aTx_Lin_Cal_NV_Info.freq_comp_ref_ch_list[1] = 290;
   aTx_Lin_Cal_NV_Info.ref_chan_list[0][0][0] = 200;
   aTx_Lin_Cal_NV_Info.upper_bound_chan_list[0][0][0] = 500;
   aTx_Lin_Cal_NV_Info.ref_chan_list[0][0][1] = 563;
   aTx_Lin_Cal_NV_Info.upper_bound_chan_list[0][0][1] = 799;
   aTx_Lin_Cal_NV_Info.ref_chan_list[0][1][0] = 563;
   aTx_Lin_Cal_NV_Info.upper_bound_chan_list[0][1][0] = 799;
   aTx_Lin_Cal_NV_Info.ref_chan_list[1][0][0] = 400;
   aTx_Lin_Cal_NV_Info.upper_bound_chan_list[1][0][0] = 799;
   aTx_Lin_Cal_NV_Info.rgi_threshold_list[0][0] = 80;
   aTx_Lin_Cal_NV_Info.freq_comp_rgi_list[0][0] = 70;
   aTx_Lin_Cal_NV_Info.rgi_threshold_list[0][1] = 128;
   aTx_Lin_Cal_NV_Info.freq_comp_rgi_list[0][1] = 80;
   aTx_Lin_Cal_NV_Info.rgi_threshold_list[1][0] = 128;
   aTx_Lin_Cal_NV_Info.freq_comp_rgi_list[1][0] = 80;
   
   unsigned short numLin = 0 ;
   // Decides which NV should be written to. Prx or SV. 0 = PRx, 2= SV Path.
   unsigned short nvPath = 0;      
   bOk= QLIB_RFCAL_Complete_TxRxCal( g_hResourceContext,
                           NV_MODE_BC0,
                           (void*)&ftm_request,
                           (void*)&ftm_response,
                           (void*)&aTx_NV_Structure,
                           numRecords,
                           (void*)&aTx_Lin_Cal_NV_Info,
                           &numLin,
                           nvPath,
                           true );

   return 1;
}


unsigned char BackupRFNV_and_CopyDataFromNVTool()
{
/*  
   This example illustrates 1) how to backup RF NV item data from mobile and 2) how to move RF NV data from NVTool, to RFCal NV Manager.
   The data can later be combined with calibrated NV items, and be written to a QCN file
*/

#define NUM_OF_ITEMS_TO_BACKUP 4
   unsigned short rfnv_list[] = {264, 23642, 20384, 22722};
   unsigned short rfnv_FTM_RFNV_read_status[NUM_OF_ITEMS_TO_BACKUP] = { 0 };
   unsigned short rfnv_list_size = NUM_OF_ITEMS_TO_BACKUP;
   ::QLIB_RFCAL_NV_Manager_BackupNVItems(g_hResourceContext, rfnv_list, rfnv_FTM_RFNV_read_status, rfnv_list_size);

   char * _sNVDef = "C:\\Program Files\\Qualcomm\\QDART\\Databases\\NVFiles\\NvDefinition.xml";
   char * _sNVSource = "C:\\Program Files\\Qualcomm\\QDART\\Databases\\NVFiles\\NvSource_Sample.xml";
   int _iNumOfNVDataTypeLoaded = 0;
   int _iNumOfNVDefLoaded = 0;
   int _iNumOfNVDataItemLoaded = 0;
   int _iResultCode = 0;

   ::QLIB_NV_LoadNVDataTypeDictinary(g_hResourceContext, _sNVDef, &_iNumOfNVDataTypeLoaded, &_iResultCode); 

   ::QLIB_NV_LoadNVDefinitionFile(g_hResourceContext, _sNVDef, &_iNumOfNVDefLoaded, &_iResultCode); 
   printf("Loaded NV Definition File %s\n", _sNVDef);

   ::QLIB_NV_LoadNVsFromSource(g_hResourceContext, _sNVSource, &_iNumOfNVDataItemLoaded, &_iResultCode);
   printf("Loaded NV Source File %s\n", _sNVSource);

   printf("iNum_RFNV_Items_loaded %d\n", _iNumOfNVDataItemLoaded);
   unsigned short _iNumOfItemsCopied = 0;
   
   ::QLIB_RFCAL_NV_Manager_Copy_RFNV_Items_From_NV_Tool(g_hResourceContext, &_iNumOfItemsCopied);
   printf("Number of RFNV items copied from NV Tool %d\n", _iNumOfItemsCopied);
   
   return 1;

}
unsigned char GSM_Tx_Linear_Calibration_Multichain( void )
{

   //Number of Calibration Channels
   unsigned long numChannels = 3;

   //Channel List
   int chanList[] = {128,190,251};

   //Number of PA Ranges (Low Bands: 4, High Bands: 3)
   unsigned long numPaRanges = 4; //Assume working in GSM 850 Band.

   //PA range List
   int paList[] = {0,1,2,3};

   //Num RGI
   unsigned short numRGIs = 32; //0 to 31

   //Number of modes.
   int numModes = 2; //GSM Mode 0, EDGE Mode 1
   //Array to store actual PA Index.
   int paIndex[4];
   unsigned short numSegments = 0;
   unsigned short segLength = 250; //some call box even support 50.
   unsigned int triggerLevRGI = 31;
   unsigned int trigPA = 0;
   bool extraRGI0_afterPA = false;
   bool extraRGI0 = false;
   unsigned int startRGI = 0;
   unsigned int stopRGI = 31;
   unsigned int stepRGI = 1;
   //Total Number of measurement data
   int numDaCalibrationSegments = 1 + numPaRanges*numRGIs*numModes; //257; // 4 (pa) * 32 (rgi) * 2 (modes) + 1(trigger); There are no extra RGI segments

   bool boolPeformPredistCal = true;
   //MultiChain output struct
   QMSL_GSM_Tx_Calibration_NV multiChainResult;
   //Queue NV
   unsigned char queue_nv_indicator = 1;
   nv_mode_id_type nv_mode = NV_MODE_GSM_850;

    FILE *fp;
   // Sample file names
   char fileName[256];
   // Full path to sample files
   char fullPath[256];
   // Directory path to sample files used for Linear PA cal
   char *sampleFilePath = "C:\\Program Files\\Qualcomm\\QDART\\QMSL\\QLibDemo\\Linear_PA_Cal_Sample_Data\\";

   //Step 1: Perform 3 channel DA calibration
   //1a) Send Input_Data_Dur_RGI_Mode_PA.csv to phone using QLIB_FTM_DA_CAL
   {
      const int MAX_SEGMENTS_DA_CAL = 512;
      unsigned short segLenList[MAX_SEGMENTS_DA_CAL];
      unsigned char  rgiList[MAX_SEGMENTS_DA_CAL];
      unsigned char  gsmModeList[MAX_SEGMENTS_DA_CAL];
      unsigned char  paRangeList[MAX_SEGMENTS_DA_CAL];

      unsigned short rgiVar;
      unsigned short paLoopVar;
      //Step 1: Perform 3 channel DA calibration
      for(unsigned long iChanCnt = 0; iChanCnt < numChannels; ++iChanCnt)
      {
#ifdef ON_TARGET
   QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );
   QLIB_FTM_SET_MODE(18); //18 for 850band
   QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );
   QLIB_FTM_SET_CHAN( g_hResourceContext, chanList[iChanCnt]);
   QLIB_FTM_SET_PDM( g_hResourceContext, 0, 0); // Center TCXO
#endif
         //Load Trigger Segment
         {
            segLenList[0] = segLength;
            rgiList[0]     =  static_cast<unsigned char>( triggerLevRGI );
            gsmModeList[0]=1;//Use GSM Mode for trigger segment
            paRangeList[0] =  static_cast<unsigned char>( trigPA );
            ++numSegments;
         }

         for(paLoopVar = 0; paLoopVar < numPaRanges; ++paLoopVar)
         {
            if(paLoopVar!=0)//extraRGI0_afterPA: skipping the first pa range
            {
               if(extraRGI0_afterPA)
               {
                  segLenList[numSegments] = segLength;
                  rgiList[numSegments]=0; // Minimum RGI
                  gsmModeList[numSegments]=0;//EDGE Mode
                  paRangeList[numSegments] =  static_cast<unsigned char>( paList[paLoopVar] );
                  ++numSegments;
               }
            }

            if(startRGI < stopRGI)
            {
               for(rgiVar=startRGI; rgiVar<=stopRGI; rgiVar+=stepRGI)
               {
                  segLenList[numSegments] = segLength;
                  rgiList[numSegments]     =  static_cast<unsigned char>( rgiVar );
                  gsmModeList[numSegments]=0;//EDGE Mode
                  paRangeList[numSegments] =  static_cast<unsigned char>( paList[paLoopVar] );
                  ++numSegments;

                  segLenList[numSegments] = segLength;
                  rgiList[numSegments]     =  static_cast<unsigned char>( rgiVar );
                  gsmModeList[numSegments]=1;//GSM Mode
                  paRangeList[numSegments] =  static_cast<unsigned char>( paList[paLoopVar] );
                  ++numSegments;
               }
            }
            if(extraRGI0)
            {
               segLenList[numSegments] = segLength;
               rgiList[numSegments]=0; // Minimum RGI
               gsmModeList[numSegments]=1;//GSM Mode
               paRangeList[numSegments]=(unsigned char)paList[paLoopVar];
               ++numSegments;
            }
         }
#ifdef ON_TARGET
   QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM_EXTENDED_C );
   QLIB_FTM_DA_CAL( g_hResourceContext, numSegments, &segLenList[0], &rgiList[0], &gsmModeList[0], &paRangeList[0] );
   QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );//re-store the mode ID
#endif
      }
   }
   
   //1b) Get power measurements from Test Equipment using the Power vs Time measurement personality
   //    On 8820 SWPPREDISTQ, on 8960 PAvT, on CMU200 IQvsSlot
   //    Duration of 273 corresponds to 1008uS, Number of steps measured = numDaCalibrationSegments
   //1c) Sample collected data for three channes available in files
   //    Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch1.csv
   //    Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch2.csv
   //    Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch3.csv

   //Step 2: Store DA Calibration Data
   QMSL_GSM_DA_Cal_Result calResultsDAToQmsl;

   //Note : the nnumber of PA ranges in this case is = number possible
   //If there are lesser number of PA ranges - care should be taken to fill the array
   calResultsDAToQmsl.iNumMeasuredTxGainStates = numPaRanges;
   calResultsDAToQmsl.iNumChannels = numChannels;
   calResultsDAToQmsl.iNumTxPowerMeasurementsPerRange = numRGIs;

   unsigned short chan;
   //This loop is applicable to all bands - irrespectiveof number of PA ranges
   for(unsigned long paLoopVar=0;paLoopVar < numPaRanges; paLoopVar++)
   {
      calResultsDAToQmsl.aiTxMeasToNVMapping[paLoopVar] = paList[paLoopVar];
      paIndex[paList[paLoopVar]] = paLoopVar; // Create reverse mapping to use later
   }

   //Walk through Channels
   //Populate array - noting that pa ranges may be 0,2,3 for high bands
   //But they should still be loaded to 0,1,2 in the structure
   unsigned long chanLoopVar = 0;
   for(chanLoopVar = 0;chanLoopVar < numChannels;chanLoopVar++)
   {
      chan = chanList[chanLoopVar];

      //Populate struct to send to qmsl
      calResultsDAToQmsl.iChannel[chanLoopVar] = chan;

      //chanLoopVar 0,1,2 maps to QMSL_POLAR_CAL_F(1,2,3)_CHANNEL
      calResultsDAToQmsl.iFreqMapping[chanLoopVar] = (QMSL_GSM_Polar_Cal_Frequency)(chanLoopVar+1);

      switch(chanLoopVar)
      {
      case 0:
            strcpy(fileName,"Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch1.csv");
            break;
      case 1:
            strcpy(fileName,"Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch2.csv");
            break;
      case 2:
            strcpy(fileName,"Meas_Chan_Dur_Rgi_Mode_Pa_Pwr_Ch3.csv");
            break;
      }

     memset(fullPath, 0, 256);      // Clear path
     strcat(fullPath, sampleFilePath);
     strcat(fullPath, fileName);

      fp = fopen(fullPath,"r");

      if(fp == NULL)
      {
         printf("File %s not found\n",fullPath);
         return 0;
      }
     else
     {
      printf("Load sample DA cal measurement data from %s\n", fileName);
     }

      int chan,dur,rgi,mode,pa;float pwr;

      while(!feof(fp))
      {
         fscanf(fp,"%d,%d,%d,%d,%d,%f\n",&chan,&dur,&rgi,&mode,&pa,&pwr);
         //debug
         //printf("%d,%d,%d,%d,%d,%f\n",chan,dur,rgi,mode,pa,pwr);

         switch(mode)
         {
         case 0:
               calResultsDAToQmsl.aiGSMTxRGI[chanLoopVar][paIndex[pa]][rgi] = rgi;
               calResultsDAToQmsl.aiGSMTxPowerMeas[chanLoopVar][paIndex[pa]][rgi] = pwr;
            break;
         case 1:
               calResultsDAToQmsl.aiEDGETxRGI[chanLoopVar][paIndex[pa]][rgi] = rgi;
               calResultsDAToQmsl.aiEDGETxPowerMeas[chanLoopVar][paIndex[pa]][rgi] = pwr;
            break;
         }
      }

      fclose(fp);
   }


   calResultsDAToQmsl.iSaturationDetectionWindow = 0.1; //For example 0.1
   if(!boolPeformPredistCal)
   {
      printf("Call QLIB_RFCal_GSM_Tx_DA_Cal_Generate_NV to transform DA Calibration data into MultiChain NV\n");

      QLIB_RFCal_GSM_Tx_DA_Cal_Generate_NV(g_hResourceContext,nv_mode, 0, (void*)& calResultsDAToQmsl,(void*)& multiChainResult,
        queue_nv_indicator);
   }
   else
   {
   
   printf("Perform Predistortion\n");


   //Step 3: Use Data to determine predistortion calibration rgi
   //Find the smallest RGI which can produce predistCalRGIPwr across the three channels

   //EDGE Power at which predistortion calibration is performed
   float predistCalRGIPwr = 27.0;//dBm

   //The final variable that holds the selected RGI
   int rgiAllChan = 0;

   //Intermediate variable to store RGI
   int rgiMatch = 0;

   //Const Predistortion is performed in PA range 0
   int predistCalPARange = 0;

   unsigned long rgiLoopVar = 0;   
   
   for(chanLoopVar = 0;chanLoopVar < numChannels;chanLoopVar++)
   {
     
      for(rgiLoopVar = 0;rgiLoopVar < numRGIs;rgiLoopVar++)
      {
            if(calResultsDAToQmsl.aiEDGETxPowerMeas[chanLoopVar][paIndex[predistCalPARange]][rgiLoopVar] >  predistCalRGIPwr)
         {
               rgiMatch = calResultsDAToQmsl.aiEDGETxRGI[chanLoopVar][paIndex[predistCalPARange]][rgiLoopVar];
            break;
         }
      }
      if(rgiMatch > rgiAllChan)
      {
         rgiAllChan = rgiMatch;
      }
   }
   //Debug: Print Results
   //printf("RGI Chosen : %d\n",rgiAllChan);

   //Step 4: Do Predistortion calibration
   //4a) Send the predistortion calibration waveform from the phone 
   //    using QLIB_FTM_TX_CFG2_AMAM_SWEEP with the calculated rgiAllChan as parameter
   //    QLIB_FTM_TX_CFG2_AMAM_SWEEP_V2( g_hResourceContext, dcDuration_QS, edgeDuration_QS, rgiAllChan, waveformType, &calResult.iEDGETxCalScale );
   //4b) Get power measurements from Test Equipment using the IQ Capture personality
   //4c) Sample collected data for three channes available in files
   //    File Format :: Time,Phase,Power(dBm)
   //    AMAM_Cal__09_59_51_Ch0.csv
   //    AMAM_Cal__09_59_51_Ch1.csv
   //    AMAM_Cal__09_59_51_Ch2.csv

  
   //Populate QMSL struct to pass to QLIB

      unsigned short piDigScale = 9800; //This is the return value from the QLIB_FTM_TX_CFG2_AMAM_SWEEP API 4a) above

   //The input parameters for cal.
      int dcDuration_QS = 625;
      int edgeDuration_QS = 625;
      int iWaveformType = 0;

   // Number of samples for the data capture when the cal command was called with 1000,1000
   int numSamples = 15760;
   float samplingRateHz = 10.0e6;
   float symbolrate = 270833.0;

   #ifdef ON_TARGET

        QLIB_FTM_SET_MODE_ID( g_hResourceContext, FTM_MODE_ID_GSM );
        QLIB_FTM_TX_CFG2_AMAM_SWEEP_V2( g_hResourceContext, dcDuration_QS, edgeDuration_QS, rgiAllChan, iWaveformType,  piDigScale );
   #endif

      static QMSL_GSM_PreDist_Cal_Result calResultPredist;
      calResultPredist.iEDGETxCalScale = piDigScale; 
      calResultPredist.iNoiseSamples = 500;//This also configured on the call box (as trigger delay)
      calResultPredist.iNumChannels = numChannels;
      calResultPredist.iNumPredistortionWaveformSamples = numSamples;
      calResultPredist.iSamplingRateHz = (long)samplingRateHz;
      calResultPredist.iCalRgi = rgiAllChan;
      calResultPredist.iDcSamples = (long)((dcDuration_QS/4)*(samplingRateHz/symbolrate));
      calResultPredist.iEdgeSamples =(long)((edgeDuration_QS/4)*(samplingRateHz/symbolrate));

   unsigned short nvID;

   //Note: The NV items 25052 to 25055 must be populated before calibration is performed
   switch(nv_mode)
   {
   case NV_MODE_GSM_EGSM:
      nvID = 25053; //RFNV_GSM_C0_GSM900_LINEAR_TX_GAIN_PARAM_I
      break;
   case NV_MODE_GSM_850:
      nvID = 25052; //RFNV_GSM_C0_GSM850_LINEAR_TX_GAIN_PARAM_I
      break;
   case NV_MODE_GSM_DCS:
      nvID = 25054; //RFNV_GSM_C0_GSM1800_LINEAR_TX_GAIN_PARAM_I
      break;
   case NV_MODE_GSM_1900:
      nvID = 25055; //RFNV_GSM_C0_GSM1900_LINEAR_TX_GAIN_PARAM_I
      break;
   }
   //Read the NV that has been stored earlier.
   //This ensures that there is one source for the Linear Tx Gain Param
   #ifdef ON_TARGET
      unsigned char nvArr[128];unsigned short readStatus; unsigned int nvSize = 128;
      QLIB_DIAG_NV_READ_F(g_hResourceContext,nvID,nvArr,128,&readStatus);
      QLIB_FTM_GSM_API_ReadEnvGainNV (g_hResourceContext, nvID, nvArr, &nvSize, &readStatus, 2 );//2 to get EDGE Env in byte 0 and 1.
   #endif
      calResultPredist.iEDGETxGainParam = 47; //from Linear Tx gain NV

   for(chanLoopVar = 0;chanLoopVar < numChannels;chanLoopVar++)
   {
        //chanLoopVar 0,1,2 maps to QMSL_POLAR_CAL_F(1,2,3)_CHANNEL
        calResultPredist.iFreqMapping[chanLoopVar] = (QMSL_GSM_Polar_Cal_Frequency)(chanLoopVar+1);

      switch(chanLoopVar)
      {
      case 0:
         strcpy(fileName,"AMAM_Cal__09_59_51_Ch0.csv");break;
      case 1:
         strcpy(fileName,"AMAM_Cal__09_59_51_Ch1.csv");break;
      case 2:
         strcpy(fileName,"AMAM_Cal__09_59_51_Ch2.csv");break;
      }

      float time,phase,ampl_dBm;

      int sampleCounter = 0;
   
     memset(fullPath, 0, 256);      // Clear path
     strcat(fullPath, sampleFilePath);
     strcat(fullPath, fileName);

      fp = fopen(fullPath,"r");

      if(fp == NULL)
      {
         printf("File %s not found\n",fullPath);
         return 0;
      }
     else
     {
      printf("Load sample Predistortion cal IQ data from %s\n", fileName);
     }

      while(!feof(fp))
      {
         fscanf(fp,"%f,%f,%f\n",&time,&phase,&ampl_dBm);
          calResultPredist.adTxAmpl[chanLoopVar][sampleCounter] = ampl_dBm;
          calResultPredist.adTxPhaseArr[chanLoopVar][sampleCounter] = phase;
         sampleCounter++;
      }

      fclose(fp);
     
   }

      calResultPredist.iDCTransientPercent = (float)10.0;
      calResultPredist.iEDGETransientSymbols = 20;


   printf("Call QLIB_RFCal_GSM_Tx_DA_Cal_IQ_Predist_Cal_Generate_NV to transform DA and PreDistortion Calibration data into MultiChain NV\n");

      QLIB_RFCal_GSM_Tx_DA_Cal_IQ_Predist_Cal_Generate_NV(g_hResourceContext,nv_mode, 0, (void*)& calResultsDAToQmsl,(void*)& calResultPredist, (void*)& multiChainResult,
      queue_nv_indicator);
   }
   return 1;
}
unsigned char Read_Write_Compressed_RFNV( void )
{
   // ************************************ //
   // This demo shows
   // (1) Read NV definition
   // (2) Read Compressed RF NV from mobile
   //   (you can read from XQCN or XML, too)
   // (3) Read binary uncompressed data from QMSL
   // (4) Set uncompressed binary data to QMSL
   // (5) You can write back to mobile or xqcn or xml
   // ************************************ //
 
   unsigned char uncomressedData[20000];
   int resSize = sizeof ( uncomressedData );
   int result = 0;
   int resize = 0;
   int iNum = 0;
   int rfnv_id = 27779; // LTE B1 Rx NV
   // ************************************ //
   // (1) Read NV definiton
   // ************************************ //
   
   std::string defpath = "C:\\Program Files\\Qualcomm\\QDART\\Databases\\NVFiles\\NvDefinition.xml";
   QLIB_NV_LoadNameFromNVDefinitionFile(g_hResourceContext,true);
   QLIB_NV_LoadNVDataTypeDictinary(g_hResourceContext,defpath.c_str(),&iNum, &result);
   QLIB_NV_LoadNVDefinitionFile(g_hResourceContext, defpath.c_str(),&iNum, &result);

   // ************************************ //
   // (2) Read Compressed RF NV from mobile
   //   (you can read from XQCN or XML, too)
   // ************************************ //
   QLIB_NV_ReadNVItemFromMobileByNVID(g_hResourceContext, rfnv_id, &result );
#if 0
   // (Other option) read from QCN/XQCN
   std::string qcn = "C:\\Qualcomm\\QDART\\Temp\\rxchar.xqcn";
   QLIB_NV_LoadNVsFromQCN(g_hResourceContext, qcn.c_str(), &iNum, &result );

   // (Other option) read from read from XML
   QLIB_NV_LoadNVsFromSource(g_hResourceContext, xml.c_str(), &iNum, &result);
#endif

  // (3) Read binary uncompressed data from QMSL
   QLIB_NV_GetNVValueByteStreamByNVID(g_hResourceContext, rfnv_id, uncomressedData, &resSize, & result);


   // ************************************ //
   // (4) Set uncompressed binary data to QMSL
   // ************************************ //
   std::stringstream ss;
   for (unsigned long i=0; i < static_cast <unsigned long>(resSize); i++)
   {
      if ( i != 0)
      {
         ss << ",";
      }
      char tmp[10];
      memset(tmp, 0, 10);
      itoa(static_cast <int>(uncomressedData[i]), tmp, 10);
      ss << tmp;
   }
   QLIB_NV_SetVariantNVValueStringByNVID(g_hResourceContext, rfnv_id, (char*)(ss.str().c_str()), 0,1 /* byte_stream */, 0 /* dec */, & result);

   // ************************************ //
   // (5) Write back to mobile or xqcn or xml
   // ************************************ //
   int resultCode = 0;
   QLIB_NV_WriteNVsToMobile( g_hResourceContext, &resultCode);
   
   return 1;
}
unsigned char GSM_Rx_Calibration_MultiChain( void )
{
   //GSM Range gain calibration
   QMSL_GSM_Rx_Calibration_Params_and_Meas aGSM_Rx_Gain_Range_MultiChain;
   QMSL_GSM_Rx_Calibration_NV aGSM_Rx_Gain_Range_MultiChain_NV;

   aGSM_Rx_Gain_Range_MultiChain.iChainId = 0;
   aGSM_Rx_Gain_Range_MultiChain.iNvContainer = aGSM_Rx_Gain_Range_MultiChain.iChainId;

   aGSM_Rx_Gain_Range_MultiChain.iNumRxGainRange = 4;

   aGSM_Rx_Gain_Range_MultiChain.iNumChannel = 8;

   // Calibration power level (dBm)
   aGSM_Rx_Gain_Range_MultiChain.aiCalibrationPwr[0] = -90;
   aGSM_Rx_Gain_Range_MultiChain.aiCalibrationPwr[1] = -90;
   aGSM_Rx_Gain_Range_MultiChain.aiCalibrationPwr[2] = -54;
   aGSM_Rx_Gain_Range_MultiChain.aiCalibrationPwr[3] = -54;

   //Channel
   aGSM_Rx_Gain_Range_MultiChain.aiChannelList[0] = 128;
   aGSM_Rx_Gain_Range_MultiChain.aiChannelList[1] = 145;
   aGSM_Rx_Gain_Range_MultiChain.aiChannelList[2] = 163;
   aGSM_Rx_Gain_Range_MultiChain.aiChannelList[3] = 180;
   aGSM_Rx_Gain_Range_MultiChain.aiChannelList[4] = 198;
   aGSM_Rx_Gain_Range_MultiChain.aiChannelList[5] = 215;
   aGSM_Rx_Gain_Range_MultiChain.aiChannelList[6] = 233;
   aGSM_Rx_Gain_Range_MultiChain.aiChannelList[7] = 251;

   // Copy RSSI values (from calibration) from different gain ranges to QMSL structure
   unsigned int i = 0;
   unsigned char toggle_rx = 1;
   for(i = 0; i < aGSM_Rx_Gain_Range_MultiChain.iNumChannel; i++)
   {
#ifdef ON_TARGET
      unsigned long results[8];
      for(int LNARange = 0; LNARange < aGSM_Rx_Gain_Range_MultiChain.iNumRxGainRange; ++LNARange)
      {
         int rng_bit_mask = 1 << LNARange;
         QLIB_FTM_RX_GAIN_RANGE_CAL(g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), static_cast<unsigned char>(5) /*burst avg*/,
                             static_cast<unsigned char>(toggle_rx), results);
         toggle_rx = 0;

      }
#endif
      // for illustration, use same RSSI values for each channel indexed by i
      // in reality, RSSI values will vary across channel
      aGSM_Rx_Gain_Range_MultiChain.aiRSSIValue[0][i] = 1469770;
      aGSM_Rx_Gain_Range_MultiChain.aiRSSIValue[1][i] = 59477;
      aGSM_Rx_Gain_Range_MultiChain.aiRSSIValue[2][i] = 1148634;
      aGSM_Rx_Gain_Range_MultiChain.aiRSSIValue[3][i] = 75059;
   }

   // Convert RSSI value to NVs
   QLIB_GSM_RX_CALIBRATION_GENERATE_NV(g_hResourceContext, NV_MODE_GSM_850,  &aGSM_Rx_Gain_Range_MultiChain, &aGSM_Rx_Gain_Range_MultiChain_NV,1);

   return 1;
}
