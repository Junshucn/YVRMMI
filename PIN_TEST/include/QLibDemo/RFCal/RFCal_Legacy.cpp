/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/RFCal_Legacy.cpp#2 $
 * $DateTime: 2017/09/11 23:23:54 $
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
#include "RFCal_Legacy.h"
#include "RFCal_ESCV4.h"
#include <conio.h>
#include <iostream>
extern HANDLE g_hResourceContext;
using namespace std;
unsigned char Test_RFCal_V3()
{
   unsigned char _bOk = FALSE;

   /* This is a complete example showing V3 RFCAL API to calibrate Tx Linearizer and Freq compensation in serial fashion */

   // Step 1. Calibrate the reference channel Tx linearizer with full bias
   //
   //  UMTS BC 1
   //  Ref Channel : 9750
   //  No of gain ranges : 3
   //  PA Gain State 0 RGI value 92 to 24  (lowest power range)
   //  PA Gain State 1 RGI value 88 to 56
   //  PA Gain State 2 RGI value 100 to 58
   //  Sweep time 20 ms
   int txSweepTime = 20 * 1000;
   int PA_State_0_Start_RGI = 94;
   int PA_State_0_Stop_RGI = 24;
   int PA_0_Step = PA_State_0_Start_RGI - PA_State_0_Stop_RGI + 1;
   int PA_0_RGI_List[100] = {0};

   int i = 0;
   for(i = 0; i < PA_0_Step; i++)
      PA_0_RGI_List[i] = PA_State_0_Start_RGI - i;

   // First Sweep for the lowest gain state
   FTM_Cal_Sweep_Request tx_Sweep[100] = {0};

   // Step 1a. Tx Sweep for the lowest gain state with full bias

   // Initialize the data structure with some default value first, these values can stay constant for Tx Sweep
   i =0;
   for( i = 0; i < 100; i++)
   {
      tx_Sweep[i].segLen = txSweepTime;
      tx_Sweep[i].masterDev = 1;
      tx_Sweep[i].slaveDev = -15;
      tx_Sweep[i].txBW = 3; // don't care for WCDMA
      tx_Sweep[i].rxBW = 3; // don't care for WCDMA
      tx_Sweep[i].chanOffset = 0;
      tx_Sweep[i].txWaveform = 0xFF;
      tx_Sweep[i].numRBs = 1;
      tx_Sweep[i].startRB = 1;
      tx_Sweep[i].txFlag = 1;
      tx_Sweep[i].numHdetAvg = 0;
      tx_Sweep[i].numLpmHdetAvg = 0; // NA
      tx_Sweep[i].rxActions = 0xffff;
      tx_Sweep[i].rxPM = 0;
      tx_Sweep[i].expRxAGC = 0;
      tx_Sweep[i].rxFlag = 0;
   }

   // Segment 0, Set channel and mode, do not turn on PA
   tx_Sweep[0].opcode = 0x0001;
   tx_Sweep[0].ulChan = 9750;
   tx_Sweep[0].rfmode = PHONE_MODE_WCDMA_IMT;
   tx_Sweep[0].paState = 255; // no-op
   tx_Sweep[0].paBias = 511;
   tx_Sweep[0].rgi = 0;

   // Segment 1,2, no op for Set mode channel to take effect
   int j = 1;
   for(j = 1; j < 3; j++)
   {
      tx_Sweep[j].opcode = 0x0000;
      tx_Sweep[j].ulChan = 9750;
      tx_Sweep[j].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_Sweep[j].paState = 255; // no-op
      tx_Sweep[j].paBias = 511;
      tx_Sweep[j].rgi = 0;
   }

   // Fill out the Tx measurment segments for the lowest gain state sweep

   for (j = 3, i = 0; j < 3 + PA_0_Step; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008;  // Tx only
      tx_Sweep[j].ulChan = 9750;
      tx_Sweep[j].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_Sweep[j].paState = 0; // gain state 0
      tx_Sweep[j].paBias = 780;  // full bias
      tx_Sweep[j].rgi = PA_0_RGI_List[i];
   }

#ifdef ON_TARGET
   // Do the sweep now
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, 3 +  PA_0_Step, true, true);
#endif
   //
   double PA_0_Full_Bias_TxSweepPower [] = {11.3073,11.1085,10.3263,9.40605,8.52212,7.64832,6.7948,5.54797,4.84576,3.30863,2.26802,0.681982 ,-0.680539 ,-1.93018 ,-3.20909 ,-3.80326 ,-4.72046 ,-5.96836 ,-7.40409 ,-8.46133 ,-9.31835 ,-10.613,-11.5872 ,-12.1776,-13.4433 ,-14.6568 ,-15.7627 ,-16.4599 ,-17.8257 ,-18.5306 ,-19.4421 ,-20.1641  ,-21.1417 ,-22.0893 ,-23.1426 ,-24.2322 ,-25.1349 ,-26.3931 ,-27.382,-28.3529 ,-29.6546 ,-30.5533 ,-31.5913 ,-32.588,-33.3519 ,-34.6479 ,-35.4996 ,-36.3917 ,-37.6081 ,-38.7961 ,-39.6655 ,-40.7097 ,-41.649,-42.1643 ,-43.4134 ,-44.4205 ,-45.6775 ,-46.3545 ,-47.7381 ,-48.6947 ,-49.4765 ,-50.4999 ,-51.2719 ,-52.8444 ,-53.5628 ,-54.3752 ,-55.5677 ,-56.4503 ,-57.0001, -58.00, -59.00};

   // Step 1b. Tx Sweep for the mid gain state with full bias

   // Fill out the Tx measurment segments for the mid gain state sweep

   int PA_State_1_Start_RGI = 88;
   int PA_State_1_Stop_RGI = 56;
   int PA_1_Step = PA_State_1_Start_RGI - PA_State_1_Stop_RGI + 1;
   int PA_1_RGI_List[100] = {0};

   for(i = 0; i < PA_1_Step; i++)
      PA_1_RGI_List[i] = PA_State_1_Start_RGI - i;

   for (j = 3,i=0 ; j < 3 + PA_1_Step; j++,i++)
   {
      tx_Sweep[j].opcode = 0x0008;  // Tx only
      tx_Sweep[j].ulChan = 9750;
      tx_Sweep[j].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_Sweep[j].paState = 1; // gain state 1
      tx_Sweep[j].paBias = 780;  // full bias
      tx_Sweep[j].rgi = PA_1_RGI_List[i];
   }

#ifdef ON_TARGET
   // Do the sweep now
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, 3 +  (PA_State_1_Start_RGI - PA_State_1_Stop_RGI + 1), true, true);
#endif
   double PA_1_Full_Bias_TxSweepPower [] = {17.9859,17.0781,16.1894,14.9659,14.2183,12.5902,11.659,10.0364,9.03667,7.83073,6.55035,5.86259,4.94074,3.68435,2.32647,1.24407,0.348305 ,-0.889938 ,-1.9073,-2.49533 ,-3.71631 ,-4.9891,-6.20284 ,-6.94702 ,-8.38484 ,-9.17325 ,-10.1187 ,-10.9375 ,-11.9455 ,-12.9494 ,-14.058,-15.201,-16.1316 };

   // Fill out the Tx measurment segments for the highest gain state sweep

   int PA_State_2_Start_RGI = 100;
   int PA_State_2_Stop_RGI = 58;
   int PA_2_Step = PA_State_2_Start_RGI - PA_State_2_Stop_RGI + 1;
   int PA_2_RGI_List[100] = {0};

   for(i = 0; i < PA_2_Step; i++)
      PA_2_RGI_List[i] = PA_State_2_Start_RGI - i;

   for (j=3,i=0 ; j < 3 + PA_2_Step ; j++,i++)
   {
      tx_Sweep[j].opcode = 0x0008 | 0x0010;  // Tx only + HDET
      tx_Sweep[j].ulChan = 9750;
      tx_Sweep[j].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_Sweep[j].paState = 2; // gain state 1
      tx_Sweep[j].paBias = 780;  // full bias
      tx_Sweep[j].rgi = PA_2_RGI_List[i];
   }
#ifdef ON_TARGET
   // Do the sweep now
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, 3 +  (PA_State_2_Start_RGI - PA_State_2_Stop_RGI + 1), true, true);
#endif
   double PA_2_Full_Bias_TxSweepPower [] = {28.0809,28.0327,28.0079,27.9645,27.8797,27.7472,27.6505,27.4032,27.084,26.8496,26.2173,25.2984,24.4916,23.4588,22.4198,21.0196,20.0503,19.1171,17.8396,16.4744,15.5705,14.4194,13.2002,12.5551,11.6693,10.4813,9.20122,8.16376,7.3062,6.11219,5.12689,4.55511,3.36967,2.13347,0.936465 ,0.197406 ,-1.22543 ,-2.00547 ,-2.9504,-3.75347 ,-5.00606 ,-6.2295,-6.90237};

   // Step 2. Intrepolate the variable bias from APT characterization table (aka XML file)
   // PA_x_APT_Bias and PA_x_APT_Power is stored in XML file

   int PA_0_APT_Bias[] = {480,420,400,380,380,360,320,320,280,280,240,240,200,200,160,140,120,100,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60};
   double PA_0_APT_Power[] = {5.433867,4.116132,2.983078,2.033866,1.081464,-0.0997,-1.4541,-2.28083,-3.67938,-4.5101,-6.11619,-7.04209,-8.7524,-9.54831,-11.3396,-13.0561,-15.571,-18.0931,-29.4008,-30.2083,-31.2841,-32.151,-33.2694,-33.8432,-34.3986,-36.0445,-36.9354,-38.0216,-39.2292,-39.8021,-40.5624,-41.6455,-43.5973,-44.3158,-45.3047,-45.7548,-47.0205,-46.76,-47.9527,-48.7175,-49.7061};
   int PA_0_APT_Table_Size = 41;

   double PA_1_APT_Power[] = {13.11615,11.53298,9.836257,8.128462,6.450947,4.576341,2.474556,1.032771,-0.60761,-2.30493,-3.34674,-5.30305,-7.67902,-8.45372,-11.1359,-12.1641,-16.7438,-36.2594};
   int PA_1_APT_Bias[] = {540,460,400,340,300,260,220,200,180,160,160,140,120,120,100,100,80,40};
   int PA_1_APT_Table_Size = 18;

   double   PA_2_APT_Power  [] =   {27.13036,   26.68166,25.9205,25.17268,24.38837,22.36934,21.20683,19.73007,18.52205,16.92891,15.35257,14.02675,12.76735,11.64804,9.489923,7.610068,5.861471,4.189881,2.197023,-0.38291,-0.86796,-1.80032,-5.26277,-6.29092,-11.2984,-12.1616,-12.9575,-14.0143};
   int      PA_2_APT_Bias   [] = {1020, 1020, 1020,760,720,600,540,480,440,380,340,320,300,280,240,200,180,160,140,120,120,120,100,100,80,80,80,80};
   int      PA_2_APT_Table_Size = 28;

   int PA_0_Interpolated_Bias[100] = {0};
   int PA_1_Interpolated_Bias[100] = {0};
   int PA_2_Interpolated_Bias[100] = {0};

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_0_APT_Power,
                                    PA_0_APT_Bias,
                                    PA_0_APT_Table_Size,
                                    PA_0_Full_Bias_TxSweepPower,
                                    PA_0_Interpolated_Bias,
                                    PA_0_Step,
                                    0,
                                    480); // max bias from APT characterization

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_1_APT_Power,
                                    PA_1_APT_Bias,
                                    PA_1_APT_Table_Size,
                                    PA_1_Full_Bias_TxSweepPower,
                                    PA_1_Interpolated_Bias,
                                    PA_1_Step,
                                    0,
                                    540);

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_2_APT_Power,
                                    PA_2_APT_Bias,
                                    PA_2_APT_Table_Size,
                                    PA_2_Full_Bias_TxSweepPower,
                                    PA_2_Interpolated_Bias,
                                    PA_2_Step,
                                    0,
                                    1020);

   // Step 3. Do APT sweep

   // Step 3a. Fill out APT bias for lowest gain state

   for (j = 3, i = 0 ; j < 3 + PA_0_Step; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008;  // Tx only
      tx_Sweep[j].ulChan = 9750;
      tx_Sweep[j].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_Sweep[j].paState = 0; // gain state 0
      tx_Sweep[j].paBias = PA_0_Interpolated_Bias[i];
      tx_Sweep[j].rgi = PA_0_RGI_List[i];
   }
#ifdef ON_TARGET
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, 3 +  PA_0_Step, true, true);
#endif
   // Take the measurment now
   // PA_0_APT_TxSweepPower is the tx power measurment from call box with APT bias applied
   double PA_0_APT_TxSweepPower [] = {11.2233,11.0263,10.2479,9.33088,8.45754,7.58718,6.73514,5.49106,4.79581,3.26336,2.22763,0.648227,-0.708466,-1.94969,-3.22224,-3.81357,-4.7371,-5.97922,-7.4138,-8.46871,-9.3311,-10.6145,-11.5955,-12.1783,-13.4395,-14.6453,-15.7577,-16.4528,-17.8058,-18.5227,-19.427,-20.1546,-21.1417,-22.0747,-23.13,-24.2222,-25.1162,-26.374,-27.3717,-28.3447,-29.6422,-30.5466,-31.5971,-32.587,-33.3498,-34.6432,-35.4997,-36.3923,-37.6046,-38.79,-39.6657,-40.714,-41.6453,-42.1585,-43.4085,-44.4165,-45.6828,-46.3583,-47.755,-48.6919,-49.4685,-50.4926,-51.2698,-52.8179,-53.5593,-54.377,-55.5307,-56.4636,-57.0173, -58.232, -59.232};

   for (j = 3, i = 0 ; j < 3 + PA_1_Step; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008;  // Tx only
      tx_Sweep[j].ulChan = 9750;
      tx_Sweep[j].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_Sweep[j].paState = 1; // gain state 1
      tx_Sweep[j].paBias = PA_1_Interpolated_Bias[i];  // full bias
      tx_Sweep[j].rgi = PA_1_RGI_List[i];
   }
#ifdef ON_TARGET
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, 3 +  PA_1_Step, true, true);
#endif
   // Take the measurment now
   // PA_1_APT_TxSweepPower is the tx power measurment from call box with APT bias applied
   double PA_1_APT_TxSweepPower [] = {17.9671,17.0501,16.1578,14.9328,14.1859,12.5563,11.6339,10.0023,9.00028,7.8028,6.52831,5.84559,4.91298,3.66889,2.31378,1.22268,0.330287,-0.908478,-1.93042,-2.51327,-3.72631,-5.00556,-6.21686,-6.95938,-8.40609,-9.198,-10.1356,-10.9591,-11.971,-12.9657,-14.0721,-15.2113,-16.1497};

   for (j = 3, i = 0 ; j < 3 + PA_2_Step ; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008 | 0x0010;  // Tx only + HDET
      tx_Sweep[j].ulChan = 9750;
      tx_Sweep[j].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_Sweep[j].paState = 2; // gain state 2
      tx_Sweep[j].paBias = PA_2_Interpolated_Bias[i];  // full bias
      tx_Sweep[j].rgi = PA_2_RGI_List[i];
      tx_Sweep[i].numHdetAvg = 5;
   }
#ifdef ON_TARGET
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, 3 +  PA_2_Step, true, true);
#endif
   // Take the measurment now
   // PA_2_APT_TxSweepPower is the tx power measurment from call box with APT bias applied
   double PA_2_APT_TxSweepPower[] = {28.0763,28.023,27.995,27.9527,27.8654,27.7327,27.6362,27.3874,27.0628,26.8288,26.1902,25.2782,24.4688,23.4343,22.393,20.9947,20.0224,19.0999,17.8212,16.4593,15.5538,14.3995,13.1819,12.5399,11.6579,10.4721,9.18352,8.1477,7.29736,6.09944,5.11969,4.54309,3.35679,2.11891,0.923091,0.181213,-1.23352,-2.02318,-2.95674,-3.7634,-5.00823,-6.24356,-6.9174};

   // HDET_From_APT_TxSweep is retrieved from mobile during APT Tx sweep
   int HDET_From_APT_TxSweep[] = {3135,3089,3083,3071,3042,2987,2957,2859,2753,2675,2474,2218,2016,1784,1584,1350,1217,1110,982,882,825,769,724,704,680,656,637,624,616,607,601,599,594,590,586,585,583,584,582,581,580,580,580};

   /*
   Step 4: Figure out the APT bias used for freq compensation

   During APT characterization, user should figure out the  "maximum power offset" from regular frequency compensation.

   For example, assume the freq compensation at full bias (780) at PA State 0 is as follow

   Ch list                  :9621,9638,9656,9673,9691,9708,9726,9750,9761,9778,9796,9813,9831,9848,9866,9883
   Freq comp power (dBm)    2.1, 2.2, 2.3, 3.5, 4.5, 2.8, 2.5, 2.0, -1.9,-1.6,-1.6,-1.4,-1.3,-1.6,-1.6,-1.6

   With target power 2.0dB at Ref Ch 9750, the "maximum power offset" is 2.5 dB (4.5dBm -2.0dBm) in this case

   This "maximum power offset" is equivalent form the parameter "Tx Power Optmization: Power Offset List" in XTT

   Derive the Freq comp APT bias from "PA_0_APT_TxSweepPower vs PA_0_Interpolated_Bias" table with power @ target power + "maximum power offset"

   */

   double PA_0_Target_Freq_Comp_Power = 2.0; // dBm;
   double PA_1_Target_Freq_Comp_Power = 10.0; // dBm;
   double PA_2_Target_Freq_Comp_Power = 15.0; // dBm;

   double PA_0_Maximum_Power_Offset = 2.5;
   double PA_1_Maximum_Power_Offset = 2.5;
   double PA_2_Maximum_Power_Offset = 2.5;

   double PA_0_FreqCompMax = PA_0_Target_Freq_Comp_Power + PA_0_Maximum_Power_Offset;
   double PA_1_FreqCompMax = PA_1_Target_Freq_Comp_Power + PA_1_Maximum_Power_Offset;
   double PA_2_FreqCompMax = PA_2_Target_Freq_Comp_Power + PA_2_Maximum_Power_Offset;
   /*
   Step 4a. Derive the RGI from APT sweep to achieve target freq comp power
   */

   int PA_0_Freq_Comp_RGI = 0;
   int PA_1_Freq_Comp_RGI = 0;
   int PA_2_Freq_Comp_RGI = 0;

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_0_APT_TxSweepPower,
                                    PA_0_RGI_List,
                                    PA_0_Step,
                                    &PA_0_Target_Freq_Comp_Power,
                                    &PA_0_Freq_Comp_RGI,
                                    1,
                                    0,
                                    128);

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_1_APT_TxSweepPower,
                                    PA_1_RGI_List,
                                    PA_1_Step,
                                    &PA_1_Target_Freq_Comp_Power,
                                    &PA_1_Freq_Comp_RGI,
                                    1,
                                    0,
                                    128);

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_2_APT_TxSweepPower,
                                    PA_2_RGI_List,
                                    PA_2_Step,
                                    &PA_2_Target_Freq_Comp_Power,
                                    &PA_2_Freq_Comp_RGI,
                                    1,
                                    0,
                                    128);

   /*
   Step 4b. Derive the APT bias from APT sweep to achieve max freq comp power
   */
   int PA_0_Freq_Comp_APT_Bias = 0;
   int PA_1_Freq_Comp_APT_Bias = 0;
   int PA_2_Freq_Comp_APT_Bias = 0;

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_0_APT_TxSweepPower,
                                    PA_0_Interpolated_Bias,
                                    PA_0_Step,
                                    &PA_0_FreqCompMax,
                                    &PA_0_Freq_Comp_APT_Bias,
                                    1,
                                    0,
                                    480);

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_1_APT_TxSweepPower,
                                    PA_1_Interpolated_Bias,
                                    PA_1_Step,
                                    &PA_1_Target_Freq_Comp_Power,
                                    &PA_1_Freq_Comp_APT_Bias,
                                    1,
                                    0,
                                    540);

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_2_APT_TxSweepPower,
                                    PA_2_Interpolated_Bias,
                                    PA_2_Step,
                                    &PA_2_Target_Freq_Comp_Power,
                                    &PA_2_Freq_Comp_APT_Bias,
                                    1,
                                    0,
                                    1020);

   int channel[] = {9621,9638,9656,9673,9691,9708,9726,9750,9761,9778,9796,9813,9831,9848,9866,9883};

   int iNumChannel = 16;  // 16 calibration channels

   int refChannel = channel[7];
   // The PA gain range for the PA state.  This is hardware design specific
   int PA_Gain_Range_PA_0 = 0;
   int PA_Gain_Range_PA_1 = 1;
   int PA_Gain_Range_PA_2 = 3;

   /*
   Step 4c. To do the actual freq comp now
   */
   for(i = 0; i < 16; i++)
   {
#ifdef ON_TARGET
      QLIB_FTM_SET_CHAN(g_hResourceContext, channel[i]);

      // PA State 0 Freq comp
      QLIB_FTM_SET_PA_RANGE(g_hResourceContext, PA_Gain_Range_PA_0);
      QLIB_FTM_SET_SMPS_PA_BIAS_VAL(g_hResourceContext, PA_0_Freq_Comp_APT_Bias);
      QLIB_FTM_SET_PDM(g_hResourceContext, 2, PA_0_Freq_Comp_RGI);
      // Measure power by call box now

      // PA State 1 Freq comp
      QLIB_FTM_SET_PA_RANGE(g_hResourceContext, PA_Gain_Range_PA_1);
      QLIB_FTM_SET_SMPS_PA_BIAS_VAL(g_hResourceContext, PA_1_Freq_Comp_APT_Bias);
      QLIB_FTM_SET_PDM(g_hResourceContext, 2, PA_1_Freq_Comp_RGI);
      // Measure power by call box now

      // PA State 2 Freq comp
      QLIB_FTM_SET_PA_RANGE(g_hResourceContext, PA_Gain_Range_PA_2);
      QLIB_FTM_SET_SMPS_PA_BIAS_VAL(g_hResourceContext, PA_2_Freq_Comp_APT_Bias);
      QLIB_FTM_SET_PDM(g_hResourceContext, 2, PA_2_Freq_Comp_RGI);
      // Measure power by call box now
#endif
   }

   // Measurment data for freq comp across channel after APT bias applied
   // down
   double PA_0_Freq_Comp_Power [] = {3.0, 2.9, 2.8, 2.7, 2.6, 2.5, 2.4, 2.0, 1.9, 1.8, 1.7, 1.5, 1.3, 1.2, 1.1, 0.9};

   // down and up pattern
   double PA_1_Freq_Comp_Power [] = {13.5, 13.5, 13.5, 13.5, 13.5, 13.5, 15.5, 12.0, 12.5, 12.5, 12.5, 12.5, 12.5,12.5, 12.5, 12.5};

   // up
   double PA_2_Freq_Comp_Power [] = {14.0, 14.0, 14.0, 14.7, 14.8, 15.0, 15.0, 15.0, 15.4, 15.8, 15.7, 15.5, 15.8,15.8, 16.1, 16.2};

   int ref_channel_index = 7;

   /*
   Step 5. We now have the following data

   a. Tx Linearizer sweep from full bias
   b. Tx Linearizer sweep from APT bias
   c. Tx Freq compenstation from APT bias

   Let's genereate NV with QMSL helper function or manually
   */

   bool bUseQMSLToFormatNV = true;   // True to call QLIB_RFCAL_Complete_TxRxCal helper function to generate NV

   if(bUseQMSLToFormatNV == false)
   {
      // freq compenation data;
      QMSL_Tx_Band_Cal_NV_Type band_cal_data;

      // zero out the data structure
      memset((void*)&band_cal_data, 0, sizeof(QMSL_Tx_Band_Cal_NV_Type));

      // fill out channel list first
      for( i = 0; i < iNumChannel; i++)
         band_cal_data.tx_cal_chan[i] = channel[i];

      // fill out PA 0 freq comp 0 data, calculate the power offset in dB*10
      // In this case, we only have primary freq comp, hence rgi_threshold is max.

      band_cal_data.tx_pa_state_cal_data[0].tx_freq_offsets[0].rgi_threshold = 128;
      for(i = 0; i < iNumChannel; i++)
         band_cal_data.tx_pa_state_cal_data[0].tx_freq_offsets[0].freq_comp[i] = int ((PA_0_Freq_Comp_Power[i] - PA_0_Freq_Comp_Power[ref_channel_index])*10);

      // fill the Tx linear table address for (voice + pri freq comp)
      band_cal_data.tx_pa_state_cal_data[0].linearizer_index[0][0].table_offset = 0;  ///first table
      band_cal_data.tx_pa_state_cal_data[0].linearizer_index[0][0].upper_bound_chan = 9892; // the last operating channel of the band

      // fill out PA 1 freq comp 0 data, calculate the power offset in dB*10
      // In this case, we only have primary freq comp, hence rgi_threshold is max.

      band_cal_data.tx_pa_state_cal_data[1].tx_freq_offsets[0].rgi_threshold = 128;
      for(i = 0; i <iNumChannel; i++)
         band_cal_data.tx_pa_state_cal_data[1].tx_freq_offsets[0].freq_comp[i] = int ((PA_1_Freq_Comp_Power[i] - PA_1_Freq_Comp_Power[ref_channel_index])*10);

      // fill the Tx linear table address for (voice + pri freq comp)
      band_cal_data.tx_pa_state_cal_data[1].linearizer_index[0][0].table_offset = 1;  // second table
      band_cal_data.tx_pa_state_cal_data[1].linearizer_index[0][0].upper_bound_chan = 9892; // the last operating channel of the band

      // fill out PA 2 freq comp 0 data, calculate the power offset in dB*10
      // In this case, we only have primary freq comp, hence rgi_threshold is max.

      band_cal_data.tx_pa_state_cal_data[2].tx_freq_offsets[0].rgi_threshold = 128;
      for(i = 0; i < iNumChannel; i++)
         band_cal_data.tx_pa_state_cal_data[2].tx_freq_offsets[0].freq_comp[i] = int ((PA_2_Freq_Comp_Power[i] - PA_2_Freq_Comp_Power[ref_channel_index])*10);

      // fill the Tx linear table address for (voice + pri freq comp)
      band_cal_data.tx_pa_state_cal_data[2].linearizer_index[0][0].table_offset = 2;  // third table
      band_cal_data.tx_pa_state_cal_data[2].linearizer_index[0][0].upper_bound_chan = 9892; // the last operating channel of the band

      // Step 5 :: Initialize the linearizer table first

      QMSL_Tx_Cal_Linearizer_Table_NV_Type tx_lin_table[3]; // 3 linearizer tables

      for(i = 0; i < 3; i++)
      {
         tx_lin_table[i].tx_chan = 0;
         int j = 0;
         for(j = 0; j < 64; j++)
         {
            tx_lin_table[i].apt[j] = 65535;
            tx_lin_table[i].power[j] = 32767;
            tx_lin_table[i].rgi[j] = 65535;
         }
      }

      // Fill out PA 0 Linearizer table, RGI, Power and APT Bias in ascending order
      tx_lin_table[0].tx_chan = channel[ref_channel_index];
      for(j = 0, i = PA_0_Step -1; j < 64 && i>=0 ; j++, i--)
      {
         tx_lin_table[0].rgi[j] = PA_0_RGI_List[i];
         tx_lin_table[0].power[j] = (int) PA_0_APT_TxSweepPower[i] * 10;
         tx_lin_table[0].apt[j] = PA_0_Interpolated_Bias[i];
      }

      // Fill out PA 1 Linearizer table, RGI, Power and APT Bias in ascending order
      tx_lin_table[1].tx_chan = channel[ref_channel_index];
      for(j = 0, i = PA_1_Step -1; j < 64 && i>=0 ; j++, i--)
      {
         tx_lin_table[1].rgi[j] = PA_1_RGI_List[i];
         tx_lin_table[1].power[j] = (int) PA_1_APT_TxSweepPower[i] * 10;
         tx_lin_table[1].apt[j] = PA_1_Interpolated_Bias[i];
      }

      // Fill out PA 2 Linearizer table, RGI, Power and APT Bias in ascending order
      tx_lin_table[2].tx_chan = channel[ref_channel_index];
      for(j = 0, i = PA_2_Step -1; j < 64 && i>=0 ; j++, i--)
      {
         tx_lin_table[2].rgi[j] = PA_2_RGI_List[i];
         tx_lin_table[2].power[j] = (int) PA_2_APT_TxSweepPower[i] * 10;
         tx_lin_table[2].apt[j] = PA_2_Interpolated_Bias[i];
      }

      // Create a data structure which host all the tables.  In this design, we only have 3 tables.
      typedef struct
      {
         QMSL_Tx_Band_Cal_NV_Type band_cal_data;
         QMSL_Tx_Cal_Linearizer_Table_NV_Type tx_lin_table[3]; // 3 linearizer tables
      } NV_Complete_3_PA_States_Single_Channel;

      NV_Complete_3_PA_States_Single_Channel NV_Table;
      memset((void*)&NV_Table, 0, sizeof(NV_Complete_3_PA_States_Single_Channel));

      memcpy((void*)&NV_Table.band_cal_data, (void*)&band_cal_data, sizeof(QMSL_Tx_Band_Cal_NV_Type));
      memcpy((void*)&NV_Table.tx_lin_table, (void*)&tx_lin_table, 3* sizeof(QMSL_Tx_Cal_Linearizer_Table_NV_Type));

      unsigned char* dest = new unsigned char[sizeof(NV_Complete_3_PA_States_Single_Channel)];
      short iDestSize = sizeof(NV_Complete_3_PA_States_Single_Channel);

      /* Now we have the NV table, run zlib to compress;

      compress((Bytef*)dest, (uLongf*)&iDestSize, (Bytef*)NV_Table, (uLongf)sizeOf(NV_Complete_3_PA_States_Single_Channel));

      */

      /*
      Call FTM_RFNV_WRITE to write the compressed data
      */

#define RFNV_WCDMA_2100_TX_MULTI_LIN_DATA_I 22975  // See NVDefinition.xml
      unsigned short iFTM_Error = 0;
      unsigned short iRFNV_ID = RFNV_WCDMA_2100_TX_MULTI_LIN_DATA_I;
#ifdef ON_TARGET
      QLIB_FTM_RFNV_WRITE(g_hResourceContext, iRFNV_ID, dest, iDestSize, &iFTM_Error);
#endif
   }
   else
      // Use QMSL to genrate NV
   {
      FTM_Cal_Sweep_Request ftm_request[1000];
      FTM_Cal_Sweep_Response ftm_response[1000];
      QMSL_Tx_Cal_Complete_NV_Type aTx_NV_Structure;
      QMSL_Tx_Cal_NV_Gen_Info aTx_Lin_Cal_NV_Info;

      memset((void*) &aTx_NV_Structure, 0, sizeof(QMSL_Tx_Cal_Complete_NV_Type));

      // i is used to keep track the number of records in ftm_resquest/response

      // For Tx linearizer, fill in Tx power data from APT sweep in ascending order
      for(i = 0, j = 0; i < PA_0_Step; j++, i++)
      {
         ftm_request[i].txPurpose = 0x1;
         ftm_request[i].paState = 0;
         ftm_request[i].rgi = PA_0_RGI_List[PA_0_Step - j -1];
         ftm_request[i].paBias = PA_0_Interpolated_Bias[PA_0_Step - j-1];
         ftm_request[i].ulChan = refChannel;
         ftm_response[i].aiTxPower =  (word)(PA_0_APT_TxSweepPower[PA_0_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
      }
      for(j = 0; j < PA_1_Step; j++, i++)
      {
         ftm_request[i].txPurpose = 0x1;
         ftm_request[i].paState = 1;
         ftm_request[i].rgi = PA_1_RGI_List[PA_1_Step - j-1];
         ftm_request[i].paBias = PA_1_Interpolated_Bias[PA_1_Step - j-1];
         ftm_request[i].ulChan = refChannel;
         ftm_response[i].aiTxPower =  (word)(PA_1_APT_TxSweepPower[PA_1_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
      }
      for(j = 0; j < PA_2_Step; j++, i++)
      {
         ftm_request[i].txPurpose = 0x1;
         ftm_request[i].paState = 2;
         ftm_request[i].rgi = PA_2_RGI_List[PA_2_Step - j-1];
         ftm_request[i].paBias = PA_2_Interpolated_Bias[PA_2_Step - j-1];
         ftm_request[i].ulChan = refChannel;
         ftm_response[i].aiTxPower =  (word)(PA_2_APT_TxSweepPower[PA_2_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
      }

      for(j = 0; j < iNumChannel; j++, i++)
      {
         ftm_request[i].txPurpose = 0x4;    // freq comp
         ftm_request[i].paState = 0;
         ftm_request[i].rgi = PA_0_Freq_Comp_RGI;
         ftm_request[i].paBias = PA_0_Freq_Comp_APT_Bias;
         ftm_response[i].aiTxPower = (word)(PA_0_Freq_Comp_Power[j] * 100);     //for freq comp data. The unit is dB100
         ftm_request[i].ulChan =channel[j];
      }

      for(j = 0; j < iNumChannel; j++, i++)
      {
         ftm_request[i].txPurpose = 0x4;    // freq comp
         ftm_request[i].paState = 1;
         ftm_request[i].rgi = PA_1_Freq_Comp_RGI;
         ftm_request[i].paBias = PA_1_Freq_Comp_APT_Bias;
         ftm_response[i].aiTxPower = (word)(PA_1_Freq_Comp_Power[j] * 100); //for freq comp data. The unit is dB100
         ftm_request[i].ulChan =channel[j];
      }

      for(j = 0; j < iNumChannel; j++, i++)
      {
         ftm_request[i].txPurpose = 0x4;    // freq comp
         ftm_request[i].paState = 2;
         ftm_request[i].rgi = PA_2_Freq_Comp_RGI;
         ftm_request[i].paBias = PA_2_Freq_Comp_APT_Bias;
         ftm_response[i].aiTxPower = (word)(PA_2_Freq_Comp_Power[j] * 100); //for freq comp data. The unit is dB100
         ftm_request[i].ulChan =channel[j];
      }
      memset((void*)&aTx_Lin_Cal_NV_Info, 0, sizeof(QMSL_Tx_Cal_NV_Gen_Info));

      for(j = 0; j < iNumChannel; j++)
         aTx_Lin_Cal_NV_Info.freq_comp_ch_list[j] = channel[j];  // Fill the channel list

      aTx_Lin_Cal_NV_Info.freq_comp_ref_ch_list[0] = refChannel;
      aTx_Lin_Cal_NV_Info.freq_comp_ref_ch_list[1] = refChannel;
      aTx_Lin_Cal_NV_Info.freq_comp_ref_ch_list[2] = refChannel;

      aTx_Lin_Cal_NV_Info.rgi_threshold_list[0][0] = 128;
      aTx_Lin_Cal_NV_Info.rgi_threshold_list[1][0] = 128;
      aTx_Lin_Cal_NV_Info.rgi_threshold_list[2][0] = 128;

      aTx_Lin_Cal_NV_Info.freq_comp_rgi_list[0][0] = PA_0_Freq_Comp_RGI;
      aTx_Lin_Cal_NV_Info.freq_comp_rgi_list[1][0] = PA_1_Freq_Comp_RGI;
      aTx_Lin_Cal_NV_Info.freq_comp_rgi_list[2][0] = PA_2_Freq_Comp_RGI;

      aTx_Lin_Cal_NV_Info.upper_bound_chan_list[0][0][0] = 9892; // the last operating channel of the band
      aTx_Lin_Cal_NV_Info.upper_bound_chan_list[1][0][0] = 9892; // the last operating channel of the band
      aTx_Lin_Cal_NV_Info.upper_bound_chan_list[2][0][0] = 9892; // the last operating channel of the band

      aTx_Lin_Cal_NV_Info.ref_chan_list[0][0][0] = refChannel;
      aTx_Lin_Cal_NV_Info.ref_chan_list[1][0][0] = refChannel;
      aTx_Lin_Cal_NV_Info.ref_chan_list[2][0][0] = refChannel;

      unsigned short numRecords = i; // the number of record in the request/response

      unsigned short numLin = 0 ;
      // Decides which NV should be written to. Prx or SV. 0 = PRx, 2= SV Path.
      unsigned short nvPath = 0;
      _bOk = QLIB_RFCAL_Complete_TxRxCal( g_hResourceContext,
                                          NV_MODE_WCDMA_IMT,
                                          (void*)&ftm_request,
                                          (void*)&ftm_response,
                                          (void*)&aTx_NV_Structure,
                                          numRecords,
                                          (void*)&aTx_Lin_Cal_NV_Info,
                                          &numLin,
                                          nvPath,
                                          true );

      int dummy = 1;
   }
   return (_bOk!=0);
}

void Test_RFCal_FreqCompSweep_V3()
{
   bool _bOk = false;

   /* This is a untested example showing V3 RFCAL API to calibrate Tx/Rx Freq Comp in Cal v3 Fashion */

   // Step 1. Calibrate the reference channel Tx linearizer with full bias
   //
   //  UMTS BC 1
   //  Ref Channel : 9750
   //  No of gain ranges : 3

   //Segments
   //1. RFMode Config (needs 60ms)
   //2. 60ms of RFMode (so 14 NOP segments of 4ms each)
   //Per Channel
   //3. PA Hi Lim vs. Freq Seg + LNA0 (DVGA)
   //4. PA Hi Lin vs. Freq Seg + LNA 1
   //5. PA Md Lin vs. Freq Seg + LNA 2
   //6. PA Md ILPC vs. Freq Seg + LNA 3
   //7. PA Lo Lin vs. Freq Seg + No Rx
   //8. Retune if not last channel - else teardown

   //  Sweep time 4 ms
   int txSweepTime = 4 * 1000;
   int PER_FREQ_PA_LIST[] = {2,2,1,1,0};
   int PER_FREQ_RGI_LIST[] = {90,80,90,60,80}; // Note : This should come from power converted to RGI based on APT linearizer data
   int PER_FREQ_BIAS_LIST[] = {780,600,500,400,300};//Note : This should come from second sweep linearizer + offset to accountfor freq comp.
   int CHAN_LIST[] = {9621,9638,9656,9673,9691,9708,9726,9750,9761,9778,9796,9813,9831,9848,9866,9883};
   int NUM_MEAS_SEGS_PER_FREQ = 5;
   int NUM_CHANS = 16;

   int PER_FREQ_EXP_AGC[] = {-192,-152,-52,88,0};
   int PER_FREQ_LNA[]= {0,1,2,3,0};

#ifdef BUFFER_SIZE
#undef BUFFER_SIZE
#endif
#define  BUFFER_SIZE 200
   // First Sweep for the lowest gain state
   FTM_Cal_Sweep_Request tx_rx_sweep[BUFFER_SIZE] = {0};

   // Step 1a. Tx Sweep for the lowest gain state with full bias
   int i = 0;
   int j = 0;

   // Initialize the data structure with some default value first, these values can stay constant for Tx Sweep
   for( i = 0; i < BUFFER_SIZE; i++)
   {
      tx_rx_sweep[i].segLen = txSweepTime;
      tx_rx_sweep[i].masterDev = 1;
      tx_rx_sweep[i].slaveDev = -15;
      tx_rx_sweep[i].txBW = 3;  // don't care for WCDMA
      tx_rx_sweep[i].rxBW = 3;  // don't care for WCDMA
      tx_rx_sweep[i].chanOffset = 0;
      tx_rx_sweep[i].txWaveform = 0xFF;
      tx_rx_sweep[i].numRBs = 1;
      tx_rx_sweep[i].startRB = 1;
      tx_rx_sweep[i].txFlag = 1;
      tx_rx_sweep[i].numHdetAvg = 0;
      tx_rx_sweep[i].numLpmHdetAvg = 0; // NA
      tx_rx_sweep[i].rxActions = 0xffff;
      tx_rx_sweep[i].rxPM = 0;
      tx_rx_sweep[i].expRxAGC = 0;
      tx_rx_sweep[i].rxFlag = 0;
   }

   int seg_num = 0;
   // Segment 0, Set channel and mode, do not turn on PA
   tx_rx_sweep[seg_num].opcode = 0x0001;
   tx_rx_sweep[seg_num].ulChan = CHAN_LIST[0];
   tx_rx_sweep[seg_num].rfmode = PHONE_MODE_WCDMA_IMT;
   tx_rx_sweep[seg_num].paState = 255; // no-op
   tx_rx_sweep[seg_num].paBias = 511;
   tx_rx_sweep[seg_num].rgi = 0;

   int NUM_NOP_SEGMENTS = 14;

   // Segment 1,2, no op for Set mode channel to take effect
   for(j = 0; j < NUM_NOP_SEGMENTS; j++)
   {
      seg_num++;
      tx_rx_sweep[seg_num].opcode = 0x0000;
      tx_rx_sweep[seg_num].ulChan = 9750;
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_rx_sweep[seg_num].paState = 255; // no-op
      tx_rx_sweep[seg_num].paBias = 511;
      tx_rx_sweep[seg_num].rgi = 0;
   }

   // Fill out the Tx measurment segments for the lowest gain state sweep

   for (i=0; i<NUM_CHANS; i++)
   {
      seg_num++;
      int per_freq_seg_num = 0;

      tx_rx_sweep[seg_num].opcode = 0x0078;  // Tx+Rx+HDET only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 0
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  // full bias
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x2 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;//No LPM HPM for WCDMA (only 1x)
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x3; //PRx + DRx
      tx_rx_sweep[seg_num].numHdetAvg = 5;

      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0068;  // Tx+Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 0
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  // full bias
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x2 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;//No LPM HPM for WCDMA (only 1x)
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x3; //PRx + DRx

      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0068;  // Tx+Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 0
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  // full bias
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x2 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;//No LPM HPM for WCDMA (only 1x)
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x3; //PRx + DRx

      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0068;  // Tx+Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 0
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  // full bias
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x2 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;//No LPM HPM for WCDMA (only 1x)
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x3; //PRx + DRx

      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0008;  // Tx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_WCDMA_IMT;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 0
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  // full bias
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x0;
      tx_rx_sweep[seg_num].rxPM = 0;//No LPM HPM for WCDMA (only 1x)
      tx_rx_sweep[seg_num].expRxAGC = 0x0;
      tx_rx_sweep[seg_num].rxFlag = 0x0; //PRx + DRx

      if(i<NUM_CHANS-1)
      {
         seg_num++;
         //Retune Segment
         tx_rx_sweep[seg_num].opcode = 0x0002;  // Retune
         tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i+1];
         tx_rx_sweep[seg_num].rfmode = PHONE_MODE_WCDMA_IMT;
      }
   }

#ifdef ON_TARGET
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_rx_sweep, seg_num+1, true, true);
#endif
}

unsigned char Test_RFCal_V3_LTE()
{
   unsigned char _bOk = FALSE;

   /* This is a complete example showing V3 RFCAL API to calibrate Tx Linearizer and Freq compensation */

   // Step 1. Calibrate the reference channel Tx linearizer with full bias
   //
   //  LTE BC 1
   //  Ref Channel : 18300
   //  No of gain ranges : 3
   //  PA Gain State 1 RGI value 105 to 20 (highest power range)
   //  PA Gain State 2 RGI value 96 to 2
   //  PA Gain State 3 RGI value 96 to 2
   //  Sweep time 4 ms
   int txSweepTime = 4* 1000;
   int PA_State_3_Start_RGI = 96;
   int PA_State_3_Stop_RGI = 2;
   int PA_3_Step = PA_State_3_Start_RGI - PA_State_3_Stop_RGI + 1;
   int PA_3_RGI_List[100] = {0};

   int i = 0;
   for(i = 0; i < PA_3_Step; i++)
      PA_3_RGI_List[i] = PA_State_3_Start_RGI - i;

   // First Sweep for the full bias
   FTM_Cal_Sweep_Request tx_Sweep[400] = {0};

   // Initialize the data structure with some default value first, these values can stay constant for Tx Sweep

   for( i = 0; i < 400; i++)
   {
      tx_Sweep[i].segLen = txSweepTime;
      tx_Sweep[i].masterDev = 1;
      tx_Sweep[i].slaveDev = 0xFFFFFFF1;
      tx_Sweep[i].txBW = 3; // 3 stands for 10MHz for LTE
      tx_Sweep[i].rxBW = 3; // 3 stands for 10MHz for LTE
      tx_Sweep[i].chanOffset = 0;
      tx_Sweep[i].txWaveform = 255;//
      tx_Sweep[i].numRBs = 12;
      tx_Sweep[i].startRB = 19; // resource block setting for LTE
      tx_Sweep[i].txFlag = 1;
      tx_Sweep[i].numLpmHdetAvg = 0; // NA
      tx_Sweep[i].numHdetAvg = 0;// HDET samples
      tx_Sweep[i].rxActions = 0xffff;
      tx_Sweep[i].rxPM = 0;
      tx_Sweep[i].expRxAGC = 0;
      tx_Sweep[i].rxFlag = 3;
   }

   // Segment 0, Set channel and mode, do not turn on PA
   tx_Sweep[0].opcode = 0x0001;
   tx_Sweep[0].ulChan = 18300;
   tx_Sweep[0].rfmode = PHONE_MODE_LTE_B1;
   tx_Sweep[0].paState = 255; // no-op
   tx_Sweep[0].paBias = 511;
   tx_Sweep[0].rgi = 0;

   // Segment 1-14, no op for Set mode channel to take effect.  We need 60ms for segment 0 to complete, so adding 14 no op segments (1 + 14) * 4ms = 60ms.
   int j = 1;
   for(j = 1; j < 15; j++)
   {
      tx_Sweep[j].opcode = 0x0000;
      tx_Sweep[j].ulChan = 18300;
      tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
      tx_Sweep[j].paState = 255; // no-op
      tx_Sweep[j].paBias = 511;
      tx_Sweep[j].rgi = 0;
   }

   // Segment 15 Config the Tx waveform LTE only

   tx_Sweep[j].opcode = 0x0004;
   tx_Sweep[j].ulChan = 18300;
   tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
   tx_Sweep[j].paState = 255; // no-op
   tx_Sweep[j].paBias = 511;
   tx_Sweep[j].txWaveform = 1;// LTE waveform 1
   tx_Sweep[j].rgi = 0;
   ++j;

   // Fill out the Tx measurment segments for the lowest gain state sweep
   int start_index=j; // index marked the starting seg number of PA state 3
   for ( i = 0; j < start_index + PA_3_Step; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008;  // Tx only
      tx_Sweep[j].ulChan = 18300;
      tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
      tx_Sweep[j].paState = 3; // gain state 3
      tx_Sweep[j].txPurpose=3;// 1st sweep tx purpose 3
      tx_Sweep[j].paBias = 780;  // full bias
      tx_Sweep[j].rgi = PA_3_RGI_List[i];
   }
   //insert a NO OP segment between two different PAs

   tx_Sweep[j].opcode = 0x0000;
   tx_Sweep[j].ulChan = 18300;
   tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
   tx_Sweep[j].paState = 255; // no-op
   tx_Sweep[j].paBias = 511;
   tx_Sweep[j].rgi = 0;
   j++;
   // Step 1b. Tx Sweep for the mid gain state with full bias

   // Fill out the Tx measurment segments for the mid gain state sweep

   int PA_State_2_Start_RGI = 96;
   int PA_State_2_Stop_RGI = 2;
   int PA_2_Step = PA_State_2_Start_RGI - PA_State_2_Stop_RGI + 1;
   int PA_2_RGI_List[100] = {0};

   for(i = 0; i < PA_2_Step; i++)
      PA_2_RGI_List[i] = PA_State_2_Start_RGI - i;

   int start_index_1=j;// index marked the starting seg number of PA state 2
   for (i = 0; j < start_index_1 + PA_2_Step; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008;  // Tx only
      tx_Sweep[j].ulChan = 18300;
      tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
      tx_Sweep[j].paState = 2; // gain state 3
      tx_Sweep[j].txPurpose=3;// 1st sweep tx purpose 3
      tx_Sweep[j].paBias = 780;  // full bias
      tx_Sweep[j].rgi = PA_2_RGI_List[i];
   }

   //insert NO OP between two different PAs

   tx_Sweep[j].opcode = 0x0000;
   tx_Sweep[j].ulChan = 18300;
   tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
   tx_Sweep[j].paState = 255; // no-op
   tx_Sweep[j].paBias = 511;
   tx_Sweep[j].rgi = 0;
   j++;
   // Fill out the Tx measurment segments for the highest gain state sweep

   int PA_State_1_Start_RGI = 105;
   int PA_State_1_Stop_RGI = 20;
   int PA_1_Step = PA_State_1_Start_RGI - PA_State_1_Stop_RGI + 1;
   int PA_1_RGI_List[100] = {0};

   for(i = 0; i < PA_1_Step; i++)
      PA_1_RGI_List[i] = PA_State_1_Start_RGI - i;

   int start_index_2=j;// index marked the starting seg number of PA state 1
   for (i = 0; j < start_index_2 + PA_1_Step; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008|0x0010;  // Tx only + HDET
      tx_Sweep[j].ulChan = 18300;
      tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
      tx_Sweep[j].paState = 1; // gain state 3
      tx_Sweep[j].numHdetAvg=1; // HDET samples
      tx_Sweep[j].txPurpose=3;// 1st sweep tx purpose 3
      tx_Sweep[j].paBias = 780;  // full bias
      tx_Sweep[j].rgi = PA_1_RGI_List[i];
   }

   int end_index=j-1; // index marked the end of full bias sweep for all 3 PA state

#ifdef ON_TARGET
   // Do the  1st sweep now
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, end_index+1, true, true);
#endif
   //
   // dummy Tx lin measurement  for NV generation
   double PA_3_Full_Bias_TxSweepPower []= {7.81479,7.81043,7.72089,7.60785,7.42194,7.23065,6.97519,6.64151,6.22082,5.65265,5.01157,4.17023,3.34479,2.38797,1.38742,0.364777,-0.892853,-1.9057,
                                           -2.90573,-3.85254,-5.03052,-6.17355,-7.26797,-8.21588,-9.1294,-10.2238,-11.1719,-12.3235,-13.4491,-14.375,-15.4767,-16.4898,-17.4868,-18.394,-19.2564,-20.3115,
                                           -21.4104,-22.4112,-23.4912,-24.6369,-25.437,-26.4684,-27.4964,-28.4389,-29.2786,-30.0962,-31.1085,-31.8421,-32.7512,-33.7799,-34.7404,-35.7302,-36.4937,-37.3467,
                                           -37.8856,-38.5851,-39.2618,-39.9399,-40.3528,-40.8546,-41.1864,-41.5328,41.8702,-42.1551,-42.3171,-42.522,-42.6988,-42.7953,-42.9203,-42.9744,-43.0111,-43.0779,
                                           -43.1151,-43.122,-43.1997,-43.1565,-43.1886,-43.194,-43.2014,-43.2596,-43.2305,-43.214,-43.2142,-43.196,-43.2498,-43.2345,-43.2052,-43.2007,-43.2255,-43.2467,
                                           -43.2199,-43.2424,-43.2606,-43.2489,-43.2457
                                          };

   double PA_2_Full_Bias_TxSweepPower []= {17.3493,17.3224,16.9133,16.4498,15.7094,14.9769,14.0698,13.0291,11.9262,10.7511,9.70602,8.59665,7.62711,6.60742,5.60284,4.62579,3.46036,2.50565,1.57608,0.692963,-0.407044,
                                           -1.4942,-2.5585,-3.48267,-4.38687,-5.4494,-6.40125,-7.54953,-8.6803,-9.59229,-10.7016,-11.7151,-12.7109,-13.6076,-14.4589,-15.5202,-16.6148,-17.6095,-18.6574,-19.7934,-20.5883,
                                           -21.5999,-22.6056,-23.4661,-24.3105,-25.0737,-26.0128,-26.6762,-27.4772,-28.4046,-29.2459,-30.0987,-30.6604,-31.3409,-31.744,-32.2666,-32.7368,-33.1566,-33.4036,-33.7063,-33.8776,
                                           -34.1021,-34.3025,-34.4376,-34.5371,-34.6215,-34.6728,-34.7351,-34.8129,-34.775,-34.8478,-34.8605,-34.9432,-34.8963,-34.9527,-34.9787,-34.9109,-34.9714,-34.9449,-34.973,-34.9285,
                                           -34.9678,-34.9439,-34.958,-34.9655,-34.9476,-34.9509,-34.9558,-34.9884,-34.9485,-34.9563,-34.9515,-34.9155,-34.8977,-34.9753
                                          };

   double PA_1_Full_Bias_TxSweepPower [] = {23.5417,23.5315,23.5253,23.517,23.5161,23.5094,23.5078,23.5101,23.507,23.5005,23.4998,23.2621,23.0117,22.6149,22.2036,21.6413,
                                            20.9302,20.0985,19.1506,18.2752,17.2932,16.411,15.4609,14.5071,13.5564,12.398,11.4433,10.511,9.63773,8.36755,7.26856,6.21439,
                                            5.30777,4.42542,3.38062,2.44891,1.31744,0.186432,-0.71283,-1.80301,-2.79688,-3.77618,-4.67154,-5.51852,-6.61554,-7.69815,-8.68515,
                                            -9.6398,-10.8807,-11.676,-12.6958,-13.7045,-14.5884,-15.484,-16.2064,-17.2112,-17.9077,-18.7039,-19.789,-20.7502,-21.7844,-22.514,
                                            -23.3628,-23.87,-24.6382,-25.21,-25.9232,-26.3166,-26.8673,-27.1179,-27.4804,-27.7405,-28.017,-28.2003,-28.4378,-28.5003,-28.68,-28.7548,
                                            -28.8143,-28.879,-28.9716,-28.9907,-29.0464,-29.0875,-29.0911,-29.1123
                                           };

   // HDET_From_full bias _TxSweep is retrieved from mobile during Tx sweep
   int HDET_From_full_TxSweep[] = {3135,3089,3083,3071,3042,2987,2957,2859,2753,2675,2474,2218,2016,1784,1584,1350,1217,1110,982,882,825,769,724,704,680,656,637,624,616,607,601,599,594,590,586,585,583,584,582,581,580,580,580};

   // Step 2. Intrepolate the variable bias from APT characterization table (aka XML file)
   // PA_x_APT_Bias and PA_x_APT_Power is stored in XML file

   int PA_3_APT_Bias[] = {620,520,480,420,420,420,340,280,240,220,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200};
   double PA_3_APT_Power[] = {5.38,3.87,3.46,2.17,1.76,-0.15,-0.3,-1.61,-2.83,-3.42,-4.87,-5.81,-6.74,-7.96,-8.65,-9.7,-10.54,-11.08,-12.24,-12.37,-13.95,-14.89,-15.6,-16.61,-18.02,-18.76,-19.44,-20.26,-21.13,-22.79,-23.43,-25,-26.61,-27.57,-28.68,-29.79,-30.87,-31.68,-32.57,-33.65,-34.89};
   int PA_3_APT_Table_Size = 41;

   double PA_2_APT_Power[] = {15.52,14.16,13.75,12.22,12.19,10.52,9.97,8.77,7.66,7.09,5.72,4.81,3.93,2.76,2.09,1.03,0.19,-0.65,-1.62,-1.71,-3.23,-4.15,-4.86,-5.87,-7.26,-7.99,-8.68,-9.5,-10.38,-12.05,-12.7,-14.26,-15.85,-16.82,-17.96,-19.04,-20.1,-20.92,-21.8,-22.89,-24.14};
   int PA_2_APT_Bias[] = {780,680,560,500,480,440,380,340,300,280,260,240,220,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200};
   int PA_2_APT_Table_Size = 41;

   double   PA_1_APT_Power  [] =   {25.58,24.88,23.89,21.84,23.35,22.38,21.2,20.81,19.71,19.34,17.71,17.69,16.44,15.35,14.77,13.68,12.65,11.72,10.52,9.88,8.83,8.01,7.2,6.26,6.19,4.68,3.77,3.07,1.98,0.68,-0.13,-0.72,-1.61,-2.41,-4.07,-4.72};
   int      PA_1_APT_Bias   [] = {780,780,780,780,780,780,660,620,540,500,440,420,360,340,300,280,260,240,220,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200};
   int      PA_1_APT_Table_Size = 36;

   int PA_3_Interpolated_Bias[100] = {0};
   int PA_2_Interpolated_Bias[100] = {0};
   int PA_1_Interpolated_Bias[100] = {0};

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_3_APT_Power,
                                    PA_3_APT_Bias,
                                    PA_3_APT_Table_Size,
                                    PA_3_Full_Bias_TxSweepPower,
                                    PA_3_Interpolated_Bias,
                                    PA_3_Step,
                                    0,
                                    620); // max bias from APT characterization

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_2_APT_Power,
                                    PA_2_APT_Bias,
                                    PA_2_APT_Table_Size,
                                    PA_2_Full_Bias_TxSweepPower,
                                    PA_2_Interpolated_Bias,
                                    PA_2_Step,
                                    0,
                                    780);

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_1_APT_Power,
                                    PA_1_APT_Bias,
                                    PA_1_APT_Table_Size,
                                    PA_1_Full_Bias_TxSweepPower,
                                    PA_1_Interpolated_Bias,
                                    PA_1_Step,
                                    0,
                                    780);

   // Step 3. Do APT sweep

   FTM_Cal_Sweep_Request apt_tx_Sweep[400] = {0};

   for( i = 0; i < 400; i++)
   {
      apt_tx_Sweep[i].segLen = txSweepTime;
      apt_tx_Sweep[i].masterDev = 1;
      apt_tx_Sweep[i].slaveDev = 0xFFFFFFF1;
      apt_tx_Sweep[i].txBW = 3; // 3 stands for 10MHz for LTE
      apt_tx_Sweep[i].rxBW = 3; // 3 stands for 10MHz for LTE
      apt_tx_Sweep[i].chanOffset = 0;
      apt_tx_Sweep[i].txWaveform = 255;//
      apt_tx_Sweep[i].numRBs = 12;
      apt_tx_Sweep[i].startRB = 19; // resource block setting for LTE
      apt_tx_Sweep[i].txFlag = 1;
      apt_tx_Sweep[i].numLpmHdetAvg = 0; // NA
      apt_tx_Sweep[i].rxActions = 0xffff;
      apt_tx_Sweep[i].rxPM = 0;
      apt_tx_Sweep[i].expRxAGC = 0;
      apt_tx_Sweep[i].rxFlag = 3;
   }

   // Segment 0, Set channel and mode, do not turn on PA
   apt_tx_Sweep[0].opcode = 0x0001;
   apt_tx_Sweep[0].ulChan = 18300;
   apt_tx_Sweep[0].rfmode = PHONE_MODE_LTE_B1;
   apt_tx_Sweep[0].paState = 255; // no-op
   apt_tx_Sweep[0].paBias = 511;
   apt_tx_Sweep[0].rgi = 0;

   // Segment 1-14, no op for Set mode channel to take effect.  We need 60ms for segment 0 to complete, so adding 14 no op segments (1 + 14) * 4ms = 60ms.
   for(j = 1; j < 15; j++)
   {
      apt_tx_Sweep[j].opcode = 0x0000;
      apt_tx_Sweep[j].ulChan = 18300;
      apt_tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
      apt_tx_Sweep[j].paState = 255; // no-op
      apt_tx_Sweep[j].paBias = 511;
      apt_tx_Sweep[j].rgi = 0;
   }

   // Segment 15 Config the Tx waveform LTE only

   apt_tx_Sweep[j].opcode = 0x0004;
   apt_tx_Sweep[j].ulChan = 18300;
   apt_tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
   apt_tx_Sweep[j].paState = 255; // no-op
   apt_tx_Sweep[j].paBias = 511;
   apt_tx_Sweep[j].txWaveform = 1;// LTE waveform 1
   apt_tx_Sweep[j].rgi = 0;
   ++j;

   // Fill out the Tx measurment segments for the lowest gain state sweep
   int apt_start_index=j; // index marked the starting seg number of PA state 3 in APT sweep
   for ( i = 0; j < apt_start_index + PA_3_Step; j++, i++)
   {
      apt_tx_Sweep[j].opcode = 0x0008;  // Tx only
      apt_tx_Sweep[j].ulChan = 18300;
      apt_tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
      apt_tx_Sweep[j].paState = 3; // gain state 3
      apt_tx_Sweep[j].txPurpose=2;// 2st sweep tx purpose 2
      apt_tx_Sweep[j].paBias = PA_3_Interpolated_Bias[i];  // APT bias
      apt_tx_Sweep[j].rgi = PA_3_RGI_List[i];
   }
   //insert NO OP between two different PA

   apt_tx_Sweep[j].opcode = 0x0000;
   apt_tx_Sweep[j].ulChan = 18300;
   apt_tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
   apt_tx_Sweep[j].paState = 255; // no-op
   apt_tx_Sweep[j].paBias = 511;
   apt_tx_Sweep[j].rgi = 0;
   j++;

   int apt_start_index_1=j;// index marked the starting seg number of PA state 2 in APT sweep
   for (i = 0; j < apt_start_index_1 + PA_2_Step; j++, i++)
   {
      apt_tx_Sweep[j].opcode = 0x0008;  // Tx only
      apt_tx_Sweep[j].ulChan = 18300;
      apt_tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
      apt_tx_Sweep[j].paState = 2; // gain state 2
      apt_tx_Sweep[j].txPurpose=2;// 2st sweep tx purpose 2
      apt_tx_Sweep[j].paBias = PA_2_Interpolated_Bias[i];  // APT bias
      apt_tx_Sweep[j].rgi = PA_2_RGI_List[i];
   }

   //insert NO OP between two different PA

   apt_tx_Sweep[j].opcode = 0x0000;
   apt_tx_Sweep[j].ulChan = 18300;
   apt_tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
   apt_tx_Sweep[j].paState = 255; // no-op
   apt_tx_Sweep[j].paBias = 511;
   apt_tx_Sweep[j].rgi = 0;
   j++;
   // Fill out the Tx measurment segments for the highest gain state sweep

   int apt_start_index_2=j; // index marked the starting seg number of PA state 1 in APT sweep
   for (i = 0; j < apt_start_index_2 + PA_1_Step; j++, i++)
   {
      apt_tx_Sweep[j].opcode = 0x0008|0x0010;  // Tx only + HDET
      apt_tx_Sweep[j].ulChan = 18300;
      apt_tx_Sweep[j].rfmode = PHONE_MODE_LTE_B1;
      apt_tx_Sweep[j].paState = 1; // gain state 3
      apt_tx_Sweep[j].numHdetAvg=1; // HDET samples
      apt_tx_Sweep[j].txPurpose=2;// 2st sweep tx purpose 2
      apt_tx_Sweep[j].paBias = PA_1_Interpolated_Bias[i];  // APT bias
      apt_tx_Sweep[j].rgi = PA_1_RGI_List[i];
   }

   int apt_end_index=j-1;

#ifdef ON_TARGET
   // Do the  apt sweep now
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)apt_tx_Sweep, apt_end_index+1, true, true);
#endif
   //
   //
   // Take the measurment now
   // PA_3_APT_TxSweepPower is dummy tx power measurment from call box with APT bias applied
   double PA_3_APT_TxSweepPower []= {7.81479,7.81043,7.72089,7.60785,7.42194,7.23065,6.97519,6.64151,6.22082,5.65265,5.01157,4.17023,3.34479,2.38797,1.38742,0.364777,-0.892853,-1.9057,
                                     -2.90573,-3.85254,-5.03052,-6.17355,-7.26797,-8.21588,-9.1294,-10.2238,-11.1719,-12.3235,-13.4491,-14.375,-15.4767,-16.4898,-17.4868,-18.394,-19.2564,-20.3115,
                                     -21.4104,-22.4112,-23.4912,-24.6369,-25.437,-26.4684,-27.4964,-28.4389,-29.2786,-30.0962,-31.1085,-31.8421,-32.7512,-33.7799,-34.7404,-35.7302,-36.4937,-37.3467,
                                     -37.8856,-38.5851,-39.2618,-39.9399,-40.3528,-40.8546,-41.1864,-41.5328,41.8702,-42.1551,-42.3171,-42.522,-42.6988,-42.7953,-42.9203,-42.9744,-43.0111,-43.0779,
                                     -43.1151,-43.122,-43.1997,-43.1565,-43.1886,-43.194,-43.2014,-43.2596,-43.2305,-43.214,-43.2142,-43.196,-43.2498,-43.2345,-43.2052,-43.2007,-43.2255,-43.2467,
                                     -43.2199,-43.2424,-43.2606,-43.2489,-43.2457
                                    };

   double PA_2_APT_TxSweepPower []= {17.3493,17.3224,16.9133,16.4498,15.7094,14.9769,14.0698,13.0291,11.9262,10.7511,9.70602,8.59665,7.62711,6.60742,5.60284,4.62579,3.46036,2.50565,1.57608,0.692963,-0.407044,
                                     -1.4942,-2.5585,-3.48267,-4.38687,-5.4494,-6.40125,-7.54953,-8.6803,-9.59229,-10.7016,-11.7151,-12.7109,-13.6076,-14.4589,-15.5202,-16.6148,-17.6095,-18.6574,-19.7934,-20.5883,
                                     -21.5999,-22.6056,-23.4661,-24.3105,-25.0737,-26.0128,-26.6762,-27.4772,-28.4046,-29.2459,-30.0987,-30.6604,-31.3409,-31.744,-32.2666,-32.7368,-33.1566,-33.4036,-33.7063,-33.8776,
                                     -34.1021,-34.3025,-34.4376,-34.5371,-34.6215,-34.6728,-34.7351,-34.8129,-34.775,-34.8478,-34.8605,-34.9432,-34.8963,-34.9527,-34.9787,-34.9109,-34.9714,-34.9449,-34.973,-34.9285,
                                     -34.9678,-34.9439,-34.958,-34.9655,-34.9476,-34.9509,-34.9558,-34.9884,-34.9485,-34.9563,-34.9515,-34.9155,-34.8977,-34.9753
                                    };

   double PA_1_APT_TxSweepPower [] = {23.5417,23.5315,23.5253,23.517,23.5161,23.5094,23.5078,23.5101,23.507,23.5005,23.4998,23.2621,23.0117,22.6149,22.2036,21.6413,
                                      20.9302,20.0985,19.1506,18.2752,17.2932,16.411,15.4609,14.5071,13.5564,12.398,11.4433,10.511,9.63773,8.36755,7.26856,6.21439,
                                      5.30777,4.42542,3.38062,2.44891,1.31744,0.186432,-0.71283,-1.80301,-2.79688,-3.77618,-4.67154,-5.51852,-6.61554,-7.69815,-8.68515,
                                      -9.6398,-10.8807,-11.676,-12.6958,-13.7045,-14.5884,-15.484,-16.2064,-17.2112,-17.9077,-18.7039,-19.789,-20.7502,-21.7844,-22.514,
                                      -23.3628,-23.87,-24.6382,-25.21,-25.9232,-26.3166,-26.8673,-27.1179,-27.4804,-27.7405,-28.017,-28.2003,-28.4378,-28.5003,-28.68,-28.7548,
                                      -28.8143,-28.879,-28.9716,-28.9907,-29.0464,-29.0875,-29.0911,-29.1123
                                     };

   // HDET_From_APT_TxSweep is retrieved from mobile during APT Tx sweep
   int HDET_From_APT_TxSweep[] = {3135,3089,3083,3071,3042,2987,2957,2859,2753,2675,2474,2218,2016,1784,1584,1350,1217,1110,982,882,825,769,724,704,680,656,637,624,616,607,601,599,594,590,586,585,583,584,582,581,580,580,580};

   // V3 Freq

   /* This is example showing V3 RFCAL API to calibrate Tx/Rx Freq Comp in Cal v3 Fashion */

   //  LTE BC 1
   //  Ref Channel : 18300
   //  No of LNA gain ranges : 6
   //

   //Segments
   //1. RFMode Config (needs 60ms)
   //2. 60ms of RFMode (so 14 NOP segments of 4ms each)
   //Per Channel
   //3. PA Hi Lim vs. Freq Seg + LNA 0
   //4. PA Hi Lin vs. Freq Seg + LNA 1
   //5. PA Md Lin vs. Freq Seg + LNA 2
   //6. PA Lo Lin vs. Freq Seg + LNA 3
   //7. RX LNA 4
   //8. RX LNA 5
   //0. Retune
   //  Sweep time 4 ms

   int tx_rx_SweepTime = 4 * 1000;
   int PER_FREQ_PA_LIST[] = {1,1,2,3,255,255}; // last two elemment is for Rx segment so PA is 255
   int PER_FREQ_RGI_LIST[] = {93,82,87,82,0,0}; // Note : This should come from power converted to RGI based on APT linearizer data , last two for RX RGI 0
   int PER_FREQ_BIAS_LIST[] = {780,451,780,456,780,780};//Note : This should come from second sweep linearizer + offset to accountfor freq comp. last two for RX
   int CHAN_LIST[] = {18001,18038,18076,18114,18152,18190,18228,18266,18300,18342,18380,18418,18456,18494,18532,18570};
   int NUM_MEAS_SEGS_PER_FREQ = 6;
   int NUM_CHANS = 16;
   int refChannel=18300;
   int PER_FREQ_EXP_AGC[] = {-61,-60,-50,-40,-33,-33};
   int PER_FREQ_LNA[]= {0,1,2,3,4,5};

   //#define  BUFFER_SIZE 150

   FTM_Cal_Sweep_Request tx_rx_sweep[BUFFER_SIZE] = {0};
   FTM_Cal_Sweep_Response tx_rx_response[BUFFER_SIZE] = {0};

   i = 0;
   j = 0;
   for( i = 0; i < BUFFER_SIZE; i++)
   {
      tx_rx_sweep[i].segLen = tx_rx_SweepTime;
      tx_rx_sweep[i].masterDev = 1;
      tx_rx_sweep[i].slaveDev = 0xFFFFFFF1;
      tx_rx_sweep[i].txBW = 3;
      tx_rx_sweep[i].rxBW = 3;
      tx_rx_sweep[i].chanOffset = 0;
      tx_rx_sweep[i].txWaveform = 0xFF;
      tx_rx_sweep[i].txPurpose=0xFF;
      tx_rx_sweep[i].numRBs = 12;
      tx_rx_sweep[i].startRB = 19;
      tx_rx_sweep[i].txFlag = 1;
      tx_rx_sweep[i].hdetFlag = 1;
      tx_rx_sweep[i].numHdetAvg = 0;
      tx_rx_sweep[i].numLpmHdetAvg = 0; // NA
      tx_rx_sweep[i].rxActions = 0xffff;
      tx_rx_sweep[i].rxPM = 0;
      tx_rx_sweep[i].expRxAGC = 0;
      tx_rx_sweep[i].rxFlag = 3;
   }

   int seg_num = 0;
   // Segment 0, Set channel and mode, do not turn on PA
   tx_rx_sweep[seg_num].opcode = 0x0001;
   tx_rx_sweep[seg_num].ulChan = CHAN_LIST[0];
   tx_rx_sweep[seg_num].rfmode = PHONE_MODE_LTE_B1;
   tx_rx_sweep[seg_num].paState = 255; // no-op
   tx_rx_sweep[seg_num].paBias = 511;
   tx_rx_sweep[seg_num].rgi = 0;

   int NUM_NOP_SEGMENTS = 14;

   // Segment 1-14, no op for Set mode channel to take effect
   for(j = 0; j < NUM_NOP_SEGMENTS; j++)
   {
      seg_num++;
      tx_rx_sweep[seg_num].opcode = 0x0000;
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[0];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_LTE_B1;
      tx_rx_sweep[seg_num].paState = 255; // no-op
      tx_rx_sweep[seg_num].paBias = 511;
      tx_rx_sweep[seg_num].rgi = 0;
   }

   // Fill out the Tx measurment segments for the Highest gain state sweep
   for (i=0; i<NUM_CHANS; i++)
   {
      if (i==0) // first channel op4 waveform
      {
         seg_num++;
         tx_rx_sweep[seg_num].opcode = 0x0004;
         tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
         tx_rx_sweep[seg_num].rfmode = PHONE_MODE_LTE_B1;
         tx_rx_sweep[i].txWaveform = 0x01;
         tx_rx_sweep[seg_num].paState = 255; // no-op
         tx_rx_sweep[seg_num].paBias = 511;
         tx_rx_sweep[seg_num].rgi = 0;
      }
      else // retune op2
      {
         seg_num++;
         tx_rx_sweep[seg_num].opcode = 0x0002;
         tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
         tx_rx_sweep[seg_num].rfmode = PHONE_MODE_LTE_B1;
         tx_rx_sweep[seg_num].paState = 255; // no-op
         tx_rx_sweep[seg_num].paBias = 511;
         tx_rx_sweep[seg_num].rgi = 0;

         // add no-op for changing CH
         seg_num++;
         tx_rx_sweep[seg_num].opcode = 0x0000;
         tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
         tx_rx_sweep[seg_num].rfmode = PHONE_MODE_LTE_B1;
         tx_rx_sweep[seg_num].paState = 255; // no-op
         tx_rx_sweep[seg_num].paBias = 511;
         tx_rx_sweep[seg_num].rgi = 0;
      }

      seg_num++;
      int per_freq_seg_num = 0;

      tx_rx_sweep[seg_num].opcode = 0x0078;  // Tx+Rx+HDET only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_LTE_B1;
      tx_rx_sweep[seg_num].txPurpose=5;  // Tx lim vs freq
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 1
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  // full bias
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x1 << PER_FREQ_LNA[per_freq_seg_num];//rxAction is a bitmap  1 means Rx action 0 in the cal tree 2 is Rx action 1.
      tx_rx_sweep[seg_num].rxPM = 0;//No LPM HPM for LTE
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x3; //PRx + DRx
      tx_rx_sweep[seg_num].numHdetAvg = 5;

      // dummy measurement results, for NV  generation demo only. Those vaule are got after we excuting the sweeep
      tx_rx_response[seg_num].aiTxPower=(23+(CHAN_LIST[i]-refChannel)/1000)*100;//Unit dB100 this is different with Tx lin. dummy Tx measurement 23dB , this should be get from callbox after excuting the sweep
      tx_rx_response[seg_num].aiHDETResults= 1350; // dummy HDET result from device.
      tx_rx_response[seg_num].aiRxResults[0][PER_FREQ_LNA[per_freq_seg_num]]=-61+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for primary and LNA0
      tx_rx_response[seg_num].aiRxResults[1][PER_FREQ_LNA[per_freq_seg_num]]=-61+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for diversity and LNA0

      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0068;  // Tx+Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_LTE_B1;
      tx_rx_sweep[seg_num].txPurpose=4; //Tx lin vs freq
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 1
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x1 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;//No LPM HPM
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x3; //PRx + DRx

      // dummy measurement results, for NV demo only
      tx_rx_response[seg_num].aiTxPower=(11+(CHAN_LIST[i]-refChannel)/100)*100;//Unit dB100 this is different with Tx lin. dummy Tx measurement 11dB , this should be get from callbox after excuting the sweep
      tx_rx_response[seg_num].aiRxResults[0][PER_FREQ_LNA[per_freq_seg_num]]=-60+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for primary and LNA1
      tx_rx_response[seg_num].aiRxResults[1][PER_FREQ_LNA[per_freq_seg_num]]=-60+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for diversity and LNA1
      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0068;  // Tx+Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_LTE_B1;
      tx_rx_sweep[seg_num].txPurpose=4; //Tx lin vs freq
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 2
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x1 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;//No LPM HPM
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x3; //PRx + DRx

      // dummy measurement results, for NV demo only
      tx_rx_response[seg_num].aiTxPower=(10+(CHAN_LIST[i]-refChannel)/100)*100;//Unit dB100 this is different with Tx lin. dummy Tx measurement 10dB , this should be get from callbox after excuting the sweep
      tx_rx_response[seg_num].aiRxResults[0][PER_FREQ_LNA[per_freq_seg_num]]=-50+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for primary and LNA2
      tx_rx_response[seg_num].aiRxResults[1][PER_FREQ_LNA[per_freq_seg_num]]=-50+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for diversity and LNA2
      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0068;  // Tx+Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_LTE_B1;
      tx_rx_sweep[seg_num].txPurpose=4; //Tx lin vs freq
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state3
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x1 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;//No LPM HPM
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x3; //PRx + DRx

      // dummy measurement results, for NV demo only
      tx_rx_response[seg_num].aiTxPower=(0+(CHAN_LIST[i]-refChannel)/100)*100;//Unit dB100 this is different with Tx lin. dummy Tx measurement 0dB , this should be get from callbox after excuting the sweep
      tx_rx_response[seg_num].aiRxResults[0][PER_FREQ_LNA[per_freq_seg_num]]=-40+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for primary and LNA3
      tx_rx_response[seg_num].aiRxResults[1][PER_FREQ_LNA[per_freq_seg_num]]=-40+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for diversity and LNA3
      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0060;  // Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_LTE_B1;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // dummy gain state
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x1 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;//No LPM HPM
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x3; //PRx + DRx

      // dummy measurement results, for NV demo only
      tx_rx_response[seg_num].aiRxResults[0][PER_FREQ_LNA[per_freq_seg_num]]=-33+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for primary and LNA4
      tx_rx_response[seg_num].aiRxResults[1][PER_FREQ_LNA[per_freq_seg_num]]=-33+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for diversity and LNA4
      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0060;  // Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_LTE_B1;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; //dummy gain state,
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x1 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;//No LPM HPM
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x3; //PRx + DRx

      // dummy measurement results, for NV demo only
      tx_rx_response[seg_num].aiRxResults[0][PER_FREQ_LNA[per_freq_seg_num]]=-33+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for primary and LNA5
      tx_rx_response[seg_num].aiRxResults[1][PER_FREQ_LNA[per_freq_seg_num]]=-33+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for diversity and LNA5
   }

   int tx_rx_index=seg_num;
#ifdef ON_TARGET
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_rx_sweep, seg_num+1, true, true);
#endif
   /*
   Step 5. We now have the following data

   a. Tx Linearizer sweep from full bias
   b. Tx Linearizer sweep from APT bias
   c. Tx Freq compenstation from APT bias

   Let's genereate NV with QMSL helper function
   */

   // We using QMSL to generate NV
   FTM_Cal_Sweep_Request ftm_request[1000];
   FTM_Cal_Sweep_Response ftm_response[1000];
   QMSL_Tx_Cal_Complete_NV_Type aTx_NV_Structure;
   QMSL_Tx_Cal_NV_Gen_Info aTx_Lin_Cal_NV_Info;

   memset((void*) &aTx_NV_Structure, 0, sizeof(QMSL_Tx_Cal_Complete_NV_Type));

   // i is used to keep track the number of records in ftm_resquest/response
   // fill in first Tx sweep
   // For Tx linearizer, fill in Tx power data from  Non-APT sweep in ascending order

   for(i=0,j=0; j<start_index; j++,i++) // config, no-op and waveform segments, no results
   {
      ftm_request[i]=tx_Sweep[j];
   }

   for (j=0; j<PA_3_Step; j++, i++)
   {
      ftm_request[i]=tx_Sweep[PA_3_Step-j-1+start_index];
      ftm_response[i].aiTxPower = (word)(PA_3_Full_Bias_TxSweepPower[PA_3_Step-j-1]*10); //for Tx linearizer. The unit is dB10
   }

   ftm_request[i]=tx_Sweep[j+start_index];
   j++,i++;    // no op code between PAs

   for(j = 0; j < PA_2_Step; j++, i++)
   {
      ftm_request[i]=tx_Sweep[PA_2_Step - j-1+start_index_1];
      ftm_response[i].aiTxPower =  (word)(PA_2_Full_Bias_TxSweepPower[PA_2_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
   }

   ftm_request[i]=tx_Sweep[j+start_index_1];
   j++,i++;    // no op code between PAs

   for(j = 0; j < PA_1_Step; j++, i++)
   {
      ftm_request[i]=tx_Sweep[PA_1_Step - j-1+start_index_2];
      ftm_response[i].aiTxPower = (word)(PA_1_Full_Bias_TxSweepPower[PA_1_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
      ftm_response[i].aiHDETResults=HDET_From_full_TxSweep[PA_1_Step-j-1]; // HDET for high PA;
   }

   // For Tx linearizer, fill in Tx power data from APT sweep in ascending order

   for(j=0; j<apt_start_index; j++,i++) // config, no-op and waveform segments, no results
   {
      ftm_request[i]=apt_tx_Sweep[j];
   }

   for (j=0; j<PA_3_Step; j++, i++)
   {
      ftm_request[i]=apt_tx_Sweep[PA_3_Step-j-1+apt_start_index];
      ftm_response[i].aiTxPower = (word)(PA_3_APT_TxSweepPower[PA_3_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
   }

   ftm_request[i]=apt_tx_Sweep[j+apt_start_index];
   j++,i++;    // no op code between PAs

   for(j = 0; j < PA_2_Step; j++, i++)
   {
      ftm_request[i]=apt_tx_Sweep[PA_2_Step - j-1+apt_start_index_1];
      ftm_response[i].aiTxPower = (word)(PA_2_APT_TxSweepPower[PA_2_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
   }

   ftm_request[i]=apt_tx_Sweep[j+apt_start_index_1];
   j++,i++;    // no op code between PAs

   for(j = 0; j < PA_1_Step; j++, i++)
   {
      ftm_request[i]=apt_tx_Sweep[PA_1_Step-j-1+apt_start_index_2];
      ftm_response[i].aiTxPower = (word)(PA_1_APT_TxSweepPower[PA_1_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
      ftm_response[i].aiHDETResults=HDET_From_APT_TxSweep[PA_1_Step-j-1]; // HDET for high PA;
   }

   //fill in V3 tx/rx segments

   for(j=0; j<seg_num; j++,i++)
   {
      ftm_request[i]=tx_rx_sweep[j];
      // fill in the dummy tx/rx measurement results, it should be replaced my real value
      ftm_response[i]=tx_rx_response[j];
   }

   memset((void*)&aTx_Lin_Cal_NV_Info, 0, sizeof(QMSL_Tx_Cal_NV_Gen_Info));

   for(j = 0; j < NUM_CHANS; j++)
      aTx_Lin_Cal_NV_Info.freq_comp_ch_list[j] = CHAN_LIST[j];  // Fill the channel list

   aTx_Lin_Cal_NV_Info.freq_comp_ref_ch_list[1] = refChannel;
   aTx_Lin_Cal_NV_Info.freq_comp_ref_ch_list[2] = refChannel;
   aTx_Lin_Cal_NV_Info.freq_comp_ref_ch_list[3] = refChannel;

   aTx_Lin_Cal_NV_Info.rgi_threshold_list[1][0] = 128;
   aTx_Lin_Cal_NV_Info.rgi_threshold_list[2][0] = 128;
   aTx_Lin_Cal_NV_Info.rgi_threshold_list[3][0] = 128;

   aTx_Lin_Cal_NV_Info.freq_comp_rgi_list[1][0] = 82;
   aTx_Lin_Cal_NV_Info.freq_comp_rgi_list[2][0] = 87;
   aTx_Lin_Cal_NV_Info.freq_comp_rgi_list[3][0] = 82;

   for(j=0; j<3; j++)
   {
      aTx_Lin_Cal_NV_Info.upper_bound_chan_list[1][j][0] = 18599; // the last operating channel of the band
      aTx_Lin_Cal_NV_Info.upper_bound_chan_list[2][j][0] = 18599; // the last operating channel of the band
      aTx_Lin_Cal_NV_Info.upper_bound_chan_list[3][j][0] = 18599; // the last operating channel of the band

      aTx_Lin_Cal_NV_Info.ref_chan_list[1][j][0] = refChannel;
      aTx_Lin_Cal_NV_Info.ref_chan_list[2][j][0] = refChannel;
      aTx_Lin_Cal_NV_Info.ref_chan_list[3][j][0] = refChannel;
   }

   unsigned short numRecords = i; // the number of record in the request/response

   unsigned short numLin = 0 ;
   // Decides which NV should be written to. Prx or SV. 0 = PRx, 2= SV Path.
   unsigned short nvPath = 0;
   _bOk = QLIB_RFCAL_Complete_TxRxCal( g_hResourceContext,
                                       NV_MODE_LTE_B1,
                                       (void*)ftm_request,
                                       (void*)ftm_response,
                                       (void*)&aTx_NV_Structure,
                                       numRecords,
                                       (void*)&aTx_Lin_Cal_NV_Info,
                                       &numLin,
                                       nvPath,
                                       true );

   return (_bOk!=0);
}

unsigned char Test_RFCal_V3_TDSCDMA()
{
   unsigned char _bOk = FALSE;

   /* This is a complete example showing V3 RFCAL API to calibrate
   */

   // Step 1. Calibrate the reference channel Tx linearizer with full bias
   //
   //  TDSCDMA B34
   //  Ref Channel : 10081
   //  No of gain ranges : 3
   //  PA Gain State 2 RGI value 100 to 46 (highest power range)
   //  PA Gain State 1 RGI value 102 to 46
   //  PA Gain State 0 RGI value 96 to 18
   //  Sweep time 1ms/2/s
   int txSweepTime = 1* 1000;
   int PA_State_2_Start_RGI = 100;
   int PA_State_2_Stop_RGI = 46;
   int PA_2_Step = PA_State_2_Start_RGI - PA_State_2_Stop_RGI + 1;
   int PA_2_RGI_List[100] = {0};

   int i = 0;
   for(i = 0; i < PA_2_Step; i++)
      PA_2_RGI_List[i] = PA_State_2_Start_RGI - i;
   // First Sweep for the full bias
   FTM_Cal_Sweep_Request tx_Sweep[400] = {0};
   //FTM_Cal_Sweep_Response tx_Sweep_Resp[140] = {0};
   // Initialize the data structure with some default value first,
   // these values can stay constant for Tx Sweep
   i =0;
   for( i = 0; i < 400; i++)
   {
      tx_Sweep[i].segLen = txSweepTime;
      tx_Sweep[i].masterDev = 1;
      tx_Sweep[i].slaveDev = 0xFFFFFFF1;
      tx_Sweep[i].txBW = 3; // Don't care for TDSCDMA
      tx_Sweep[i].rxBW = 3; //
      tx_Sweep[i].chanOffset = 0;
      tx_Sweep[i].txWaveform = 255;//
      tx_Sweep[i].numRBs = 1;
      tx_Sweep[i].startRB = 1; // Don't care for TDS
      tx_Sweep[i].txFlag = 1;
      tx_Sweep[i].numLpmHdetAvg = 0; // NA
      tx_Sweep[i].numHdetAvg=0; // HDET samples
      tx_Sweep[i].rxActions = 0xffff;
      tx_Sweep[i].rxPM = 0;
      tx_Sweep[i].expRxAGC = 0;
      tx_Sweep[i].rxFlag = 1; // primaary
   }
   // Segment 0, Set channel and mode, do not turn on PA
   tx_Sweep[0].opcode = 0x0001;
   tx_Sweep[0].ulChan = 10081;
   tx_Sweep[0].rfmode = PHONE_MODE_TDSCDMA_B34;
   tx_Sweep[0].paState = 255; // no-op
   tx_Sweep[0].paBias = 511;
   tx_Sweep[0].rgi = 0;
   int j = 1;
   for(j = 1; j < 19; j++)// Segment 1-19, no op for Set mode channel to take effect.  We need 20ms for segment 0 to complete, so adding 19 no op segments (1 + 19) * 1ms = 20ms.
   {
      tx_Sweep[j].opcode = 0x0000;
      tx_Sweep[j].ulChan = 10081;
      tx_Sweep[j].rfmode = PHONE_MODE_TDSCDMA_B34;
      tx_Sweep[j].paState = 255; // no-op
      tx_Sweep[j].paBias = 511;
      tx_Sweep[j].rgi = 0;
   }    // Fill out the Tx measurment segments for the highest gain state sweep

   int start_index=j;
   for ( i = 0; j < start_index + PA_2_Step; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008|0x0010;  // Tx only + HDET
      tx_Sweep[j].ulChan = 10081;
      tx_Sweep[j].rfmode = PHONE_MODE_TDSCDMA_B34;
      tx_Sweep[j].paState = 2; // gain state 2
      tx_Sweep[i].numHdetAvg=1; // HDET samples
      tx_Sweep[j].txPurpose=3; // 1st sweep tx purpose 3
      tx_Sweep[j].paBias = 780;  // full bias
      tx_Sweep[j].rgi = PA_2_RGI_List[i];
   }

   //insert a NO OP segment between two different PAs

   tx_Sweep[j].opcode = 0x0000;
   tx_Sweep[j].ulChan = 10081;
   tx_Sweep[j].rfmode = PHONE_MODE_TDSCDMA_B34;
   tx_Sweep[j].paState = 255; // no-op
   tx_Sweep[j].paBias = 511;
   tx_Sweep[j].rgi = 0;
   j++;
   // Step 1b. Tx Sweep for the mid gain state with full bias

   // Fill out the Tx measurment segments for the mid gain state sweep

   int PA_State_1_Start_RGI = 102;
   int PA_State_1_Stop_RGI = 46;
   int PA_1_Step = PA_State_1_Start_RGI - PA_State_1_Stop_RGI + 1;
   int PA_1_RGI_List[100] = {0};

   for(i = 0; i < PA_1_Step; i++)
      PA_1_RGI_List[i] = PA_State_1_Start_RGI - i;
   int start_index_1=j;
   for ( i = 0; j < start_index_1 + PA_1_Step; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008;  // Tx only
      tx_Sweep[j].ulChan = 10081;
      tx_Sweep[j].rfmode = PHONE_MODE_TDSCDMA_B34;
      tx_Sweep[j].paState = 1; // gain state 1
      tx_Sweep[j].txPurpose=3; // 1st sweep tx purpose 3
      tx_Sweep[j].paBias = 780;  // full bias
      tx_Sweep[j].rgi = PA_1_RGI_List[i];
   }

   //insert a NO OP segment between two different PAs

   tx_Sweep[j].opcode = 0x0000;
   tx_Sweep[j].ulChan = 10081;
   tx_Sweep[j].rfmode = PHONE_MODE_TDSCDMA_B34;
   tx_Sweep[j].paState = 255; // no-op
   tx_Sweep[j].paBias = 511;
   tx_Sweep[j].rgi = 0;
   j++;

   // Fill out the Tx measurment segments for the lowest gain state sweep

   int PA_State_0_Start_RGI = 96;
   int PA_State_0_Stop_RGI = 18;
   int PA_0_Step = PA_State_0_Start_RGI - PA_State_0_Stop_RGI + 1;
   int PA_0_RGI_List[100] = {0};

   for(i = 0; i < PA_0_Step; i++)
      PA_0_RGI_List[i] = PA_State_0_Start_RGI - i;
   int start_index_2=j;
   for (i = 0; j < start_index_2 + PA_0_Step; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008;  // Tx only
      tx_Sweep[j].ulChan = 10081;
      tx_Sweep[j].rfmode = PHONE_MODE_TDSCDMA_B34;
      tx_Sweep[j].paState = 0; // gain state 0
      tx_Sweep[j].txPurpose=3; // 1st sweep tx purpose 3
      tx_Sweep[j].paBias = 780;  // full bias
      tx_Sweep[j].rgi = PA_0_RGI_List[i];
   }

   int end_index=j-1;

#ifdef ON_TARGET
   // Do the  1st sweep now
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, end_index+1, true, true);
#endif
   //
   //

   // dummy Tx lin measurement  for NV generation.

   double PA_2_Full_Bias_TxSweepPower []= {29.21,28.93,28.71,28.33,28.15,27.7,27.21,26.79,26.29,25.36,24.62,24.24,23.7,23.03,22.52,21.67,20.57,19.66,18.42,17.14,16.16,14.96,14.15,13.18,12.14,11.01,10.05,8.86,7.73,6.61,
                                           6.11,4.93,3.92,2.77,1.43,0.49,-0.26,-1.01,-1.86,-2.7,-4.06,-4.77,-5.62,-6.52,-7.22,-8.06,-9.03,-9.83,-9.97,-10.97,-11.71,-13.13,-14.43,-15.74,-16.69
                                          };

   int HDET_From_full_TxSweep[] = {2469,2415,2357,2265,2217,2110,2004,1922,1822,1675,1564,1519,1450,1386,1333,1264,1189,1138,1086,1040,1013,989,977,965,954,946,939,934,929,922,925,919,920,918,917,916,916,915,914,915,913,912,912,
                                   912,910,910,907,911,912,912,914,913,911,909,911
                                  };

   double PA_1_Full_Bias_TxSweepPower []= {18.35,17.85,17.56,17.1,16.75,16.19,15.95,15.36,14.79,14.31,13.78,12.88,12.19,11.83,11.33,9.86,9.22,7.94,7.4,6.61,5.76,4.76,3.56,2.74,1.71,1,0.04,-1.2,-1.94,-2.85,-4.11,-5.07,-6.15,
                                           -6.63,-7.78,-8.78,-9.28,-10.28,-11.21,-11.96,-12.72,-13.57,-14.4,-15.75,-16.46,-17.3,-18.9,-19.1,-19.76,-20.73,-21.69,-22.64,-23.37,-24.77,-26.01,-27.26,-28.13
                                          };

   double PA_0_Full_Bias_TxSweepPower [] = {11.23,10.8,10.32,9.85,9.3,8.33,7.56,7.14,6.57,4.7,4.02,2.6,2.06,1.3,0.45,-0.47,-1.61,-2.34,-3.3,-3.94,-4.84,-6.03,-6.66,-7.5,-8.7,-9.56,-10.59,-11.04,-12.13,-13.09,-13.52,-14.53,
                                            -15.46,-16.17,-16.91,-17.75,-18.56,-19.89,-20.58,-21.43,-23,-23.23,-23.93,-24.85,-25.77,-26.77,-27.52,-28.98,-30.24,-31.62,-32.5,-33.46,-34.05,-35.16,-36.22,-37.52,-38.6,-38.96,-39.48
                                            -41.24,-41.86,-42.01,-43.67,-44.29,-44.44,-44.88,-45.5,-47.64,-48.04,-49.51,-50.14,-50.4,-51.51,-52.18,-53.69,-54.58,-55.23,-56.29,-57.18
                                           };

   // Step 2. Intrepolate the variable bias from APT characterization table (aka XML file)
   // PA_x_APT_Bias and PA_x_APT_Power is stored in XML file

   int PA_0_APT_Bias[] = {780,520,480,420,420,420,340,280,240,220,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200};
   double PA_0_APT_Power[] = {5.38,3.87,3.46,2.17,1.76,-0.15,-0.3,-1.61,-2.83,-3.42,-4.87,-5.81,-6.74,-7.96,-8.65,-9.7,-10.54,-11.08,-12.24,-12.37,-13.95,-14.89,-15.6,-16.61,-18.02,-18.76,-19.44,-20.26,-21.13,-22.79,-23.43,-25,-26.61,-27.57,-28.68,-29.79,-30.87,-31.68,-32.57,-33.65,-34.89,-58.0};
   int PA_0_APT_Table_Size = 41;

   double PA_1_APT_Power[] = {15.52,14.16,13.75,12.22,12.19,10.52,9.97,8.77,7.66,7.09,5.72,4.81,3.93,2.76,2.09,1.03,0.19,-0.65,-1.62,-1.71,-3.23,-4.15,-4.86,-5.87,-7.26,-7.99,-8.68,-9.5,-10.38,-12.05,-12.7,-14.26,-15.85,-16.82,-17.96,-19.04,-20.1,-20.92,-21.8,-22.89,-24.14};
   int PA_1_APT_Bias[] = {780,680,560,500,480,440,380,340,300,280,260,240,220,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200};
   int PA_1_APT_Table_Size = 41;

   double   PA_2_APT_Power  [] =   {25.58,24.88,23.89,21.84,23.35,22.38,21.2,20.81,19.71,19.34,17.71,17.69,16.44,15.35,14.77,13.68,12.65,11.72,10.52,9.88,8.83,8.01,7.2,6.26,6.19,4.68,3.77,3.07,1.98,0.68,-0.13,-0.72,-1.61,-2.41,-4.07,-4.72};
   int      PA_2_APT_Bias   [] = {780,780,780,780,780,780,660,620,540,500,440,420,360,340,300,280,260,240,220,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200};
   int      PA_2_APT_Table_Size = 36;

   int PA_0_Interpolated_Bias[100] = {0};
   int PA_1_Interpolated_Bias[100] = {0};
   int PA_2_Interpolated_Bias[100] = {0};

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_0_APT_Power,
                                    PA_0_APT_Bias,
                                    PA_0_APT_Table_Size,
                                    PA_0_Full_Bias_TxSweepPower,
                                    PA_0_Interpolated_Bias,
                                    PA_0_Step,
                                    0,
                                    780); // max bias from APT characterization

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_1_APT_Power,
                                    PA_1_APT_Bias,
                                    PA_1_APT_Table_Size,
                                    PA_1_Full_Bias_TxSweepPower,
                                    PA_1_Interpolated_Bias,
                                    PA_1_Step,
                                    0,
                                    780);

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_2_APT_Power,
                                    PA_2_APT_Bias,
                                    PA_2_APT_Table_Size,
                                    PA_2_Full_Bias_TxSweepPower,
                                    PA_2_Interpolated_Bias,
                                    PA_2_Step,
                                    0,
                                    780);

   // Step 3. Do APT sweep

   FTM_Cal_Sweep_Request apt_tx_Sweep[400] = {0};

   i =0;
   for( i = 0; i < 400; i++)
   {
      apt_tx_Sweep[i].segLen = txSweepTime;
      apt_tx_Sweep[i].masterDev = 1;
      apt_tx_Sweep[i].slaveDev = 0xFFFFFFF1;
      apt_tx_Sweep[i].txBW = 3; // Don't care for TDSCDMA
      apt_tx_Sweep[i].rxBW = 3; //
      apt_tx_Sweep[i].chanOffset = 0;
      apt_tx_Sweep[i].txWaveform = 255;//
      apt_tx_Sweep[i].numRBs = 1;
      apt_tx_Sweep[i].startRB = 1; // Don't care for TDS
      apt_tx_Sweep[i].txFlag = 1;
      apt_tx_Sweep[i].numLpmHdetAvg = 0; // NA
      apt_tx_Sweep[i].numHdetAvg=0; // HDET samples
      apt_tx_Sweep[i].rxActions = 0xffff;
      apt_tx_Sweep[i].rxPM = 0;
      apt_tx_Sweep[i].expRxAGC = 0;
      apt_tx_Sweep[i].rxFlag = 1; // primaary
   }
   // Segment 0, Set channel and mode, do not turn on PA
   apt_tx_Sweep[0].opcode = 0x0001;
   apt_tx_Sweep[0].ulChan = 10081;
   apt_tx_Sweep[0].rfmode = PHONE_MODE_TDSCDMA_B34;
   apt_tx_Sweep[0].paState = 255; // no-op
   apt_tx_Sweep[0].paBias = 511;
   apt_tx_Sweep[0].rgi = 0;

   for(j = 1; j < 19; j++)// Segment 1-19, no op for Set mode channel to take effect.  We need 20ms for segment 0 to complete, so adding 19 no op segments (1 + 19) * 1ms = 20ms.
   {
      apt_tx_Sweep[j].opcode = 0x0000;
      apt_tx_Sweep[j].ulChan = 10081;
      apt_tx_Sweep[j].rfmode = PHONE_MODE_TDSCDMA_B34;
      apt_tx_Sweep[j].paState = 255; // no-op
      apt_tx_Sweep[j].paBias = 511;
      apt_tx_Sweep[j].rgi = 0;
   }    // Fill out the Tx measurment segments for the highest gain state sweep

   int apt_start_index=j;
   for ( i = 0; j < apt_start_index + PA_2_Step; j++, i++)
   {
      apt_tx_Sweep[j].opcode = 0x0008|0x0010;  // Tx only + HDET
      apt_tx_Sweep[j].ulChan = 10081;
      apt_tx_Sweep[j].rfmode = PHONE_MODE_TDSCDMA_B34;
      apt_tx_Sweep[j].paState = 2; // gain state 2
      apt_tx_Sweep[i].numHdetAvg=1; // HDET samples
      apt_tx_Sweep[j].txPurpose=1; // APT sweep tx purpose 1
      apt_tx_Sweep[j].paBias = PA_2_Interpolated_Bias[i];  // APT bias
      apt_tx_Sweep[j].rgi = PA_2_RGI_List[i];
   }

   //insert a NO OP segment between two different PAs

   apt_tx_Sweep[j].opcode = 0x0000;
   apt_tx_Sweep[j].ulChan = 10081;
   apt_tx_Sweep[j].rfmode = PHONE_MODE_TDSCDMA_B34;
   apt_tx_Sweep[j].paState = 255; // no-op
   apt_tx_Sweep[j].paBias = 511;
   apt_tx_Sweep[j].rgi = 0;
   j++;
   // Step 1b. Tx Sweep for the mid gain state with full bias

   // Fill out the Tx measurment segments for the mid gain state sweep

   int apt_start_index_1=j;
   for ( i = 0; j < apt_start_index_1 + PA_1_Step; j++, i++)
   {
      apt_tx_Sweep[j].opcode = 0x0008;  // Tx only
      apt_tx_Sweep[j].ulChan = 10081;
      apt_tx_Sweep[j].rfmode = PHONE_MODE_TDSCDMA_B34;
      apt_tx_Sweep[j].paState = 1; // gain state 1
      apt_tx_Sweep[j].txPurpose=1; // APT sweep tx purpose 1
      apt_tx_Sweep[j].paBias = PA_1_Interpolated_Bias[i];  // APT bias
      apt_tx_Sweep[j].rgi = PA_1_RGI_List[i];
   }

   //insert a NO OP segment between two different PAs

   apt_tx_Sweep[j].opcode = 0x0000;
   apt_tx_Sweep[j].ulChan = 10081;
   apt_tx_Sweep[j].rfmode = PHONE_MODE_TDSCDMA_B34;
   apt_tx_Sweep[j].paState = 255; // no-op
   apt_tx_Sweep[j].paBias = 511;
   apt_tx_Sweep[j].rgi = 0;
   j++;
   int apt_start_index_2=j;
   for (i = 0; j < apt_start_index_2 + PA_0_Step; j++, i++)
   {
      apt_tx_Sweep[j].opcode = 0x0008;  // Tx only
      apt_tx_Sweep[j].ulChan = 10081;
      apt_tx_Sweep[j].rfmode = PHONE_MODE_TDSCDMA_B34;
      apt_tx_Sweep[j].paState = 0; // gain state 0
      apt_tx_Sweep[j].txPurpose=1; // APT sweep tx purpose 1
      apt_tx_Sweep[j].paBias = PA_0_Interpolated_Bias[i];  // full bias
      apt_tx_Sweep[j].rgi = PA_0_RGI_List[i];
   }

   int apt_end_index=j-1;

#ifdef ON_TARGET
   // Do the  apt sweep now
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)apt_tx_Sweep, apt_end_index+1, true, true);
#endif
   //
   //

   // dummy Tx lin measurement  for NV generation.

   double PA_2_APT_TxSweepPower []= {29.18,28.9,28.67,28.29,28.11,27.66,27.17,26.74,26.24,25.21,24.23,23.72,22.97,22.05,21.36,20.21,18.72,17.54,16.27,15.09,14.17,13.03,12.27,11.33,10.33,9.23,8.28,7.11,5.99,4.88,
                                     4.39,3.21,2.21,1.06,-0.28,-1.22,-1.96,-2.71,-3.57,-4.4,-5.76,-6.47,-7.31,-8.21,-8.92,-9.75,-10.73,-11.52,-11.65,-12.67,-13.38,-14.82,-16.14,-17.43,-18.34
                                    };

   int HDET_From_APT_TxSweep[] = {2461,2407,2344,2251,2204,2097,1993,1910,1816,1649,1512,1453,1372,1289,1236,1165,1089,1048,1018,992,976,962,953,945,939,931,926,921,921,917,918,913,911,913,913,911,910,910,911,
                                  911,910,911,909,911,911,911,911,910,909,911,910,911,913,912,910
                                 };

   double PA_1_APT_TxSweepPower []= {18.34,17.83,17.54,17.08,16.73,16.17,15.85,15.08,14.33,13.7,13.01,11.8,10.86,10.4,9.87,8.14,7.22,5.88,5.37,4.6,3.78,2.83,1.66,0.88,-0.14,-0.84,-1.77,-3,-3.73,-4.64,-5.89,-6.84,
                                     -7.92,-8.39,-9.53,-10.53,-11.03,-12.03,-12.97,-13.71,-14.44,-15.3,-16.14,-17.5,-18.2,-19.04,-20.64,-20.84,-21.48,-22.43,-23.37,-24.35,-25.07,-26.42,-27.69,-28.9,-29.75
                                    };

   double PA_0_APT_TxSweepPower [] = {11.2,10.76,10.28,9.81,9.27,8.11,7.18,6.69,6.04,3.79,3.01,1.47,0.95,0.21,-0.62,-1.52,-2.63,-3.35,-4.28,-4.9,-5.78,-6.94,-7.57,-8.39,-9.57,-10.42,-11.44,-11.88,-12.96,-13.92,
                                      -14.34,-15.36,-16.28,-16.99,-17.74,-18.56,-19.38,-20.72,-21.39,-22.25,-23.83,-24.04,-24.75,-25.66,-26.58,-27.58,-28.34,-29.8,-31.05,-32.42,-33.29,-34.26,-34.86,-35.97,-37.03,
                                      -38.32,-39.4,-39.77,-40.28,-42.05,-42.67,-42.82,-44.47,-45.09,-45.24,-45.67,-46.3,-48.44,-48.85,-50.31,-50.92,-51.2,-52.3,-52.96,-54.48,-55.36,-56.01,-57.07,-57.96
                                     };

   // V3 Freq

   /* This is a untested example showing V3 RFCAL API to calibrate Tx/Rx Freq Comp in Cal v3 Fashion */

   // Step 1. Calibrate the reference channel Tx linearizer with full bias
   //
   //   TDSCDMA BC34
   //  Ref Channel : 10081
   //  No of gain ranges : 3
   // NO RX Div

   //Segments
   //1. RFMode Config (needs 20ms)
   //2. 20ms of RFMode (so 10 NOP segments of 2ms each)
   //Per Channel
   //3. PA Hi Lim vs. Freq Seg
   //4. PA Hi Lin vs. Freq Seg
   //5. PA Md Lin vs. Freq Seg
   //6. PA Lo Lin vs. Freq Seg
   //7. RX LNA 0
   //8. RX LNA 1
   //9  RX LNA 2
   //10 RX LNA 3
   //  Sweep time 2 ms

   int tx_rx_SweepTime = 2 * 1000;
   int PER_FREQ_PA_LIST[] = {2,2,1,0,255,255,255,255,255,255}; // last six elemment is for Rx segment or retune so PA is 255
   int PER_FREQ_RGI_LIST[] = {89,80,89,87,0,0,0,0,0,0}; // Note : This should come from power converted to RGI based on  linearizer data
   int PER_FREQ_BIAS_LIST[] = {780,780,780,780,0,0,0,0,511,511};//
   int CHAN_LIST[] = {10054,10063,10072,10081,10090,10099,10108,10121,65535,65535,65535,65535,65535,65535,65535,65535};
   int NUM_MEAS_SEGS_PER_FREQ = 10;
   int NUM_CHANS = 8;
   int refChannel=10081;
   int NUM_RX_MEAS=0;
   int PER_FREQ_EXP_AGC[] = {48,48,48,98};
   int PER_FREQ_LNA[]= {0,1,2,3};

   //#define  BUFFER_SIZE 300
   FTM_Cal_Sweep_Request tx_rx_sweep[BUFFER_SIZE] = {0};
   FTM_Cal_Sweep_Response tx_rx_response[BUFFER_SIZE] = {0};
   i = 0;
   j = 0;
   for( i = 0; i < BUFFER_SIZE; i++)
   {
      tx_rx_sweep[i].segLen = tx_rx_SweepTime;
      tx_rx_sweep[i].masterDev = 1;
      tx_rx_sweep[i].slaveDev = -15;
      tx_rx_sweep[i].txBW = 3;
      tx_rx_sweep[i].rxBW = 3;
      tx_rx_sweep[i].chanOffset = 0;
      tx_rx_sweep[i].txWaveform = 0xFF;
      tx_rx_sweep[i].numRBs = 1;
      tx_rx_sweep[i].startRB = 1;
      tx_rx_sweep[i].txFlag = 1;
      tx_rx_sweep[i].numHdetAvg = 0;
      tx_rx_sweep[i].numLpmHdetAvg = 0; // NA
      tx_rx_sweep[i].rxActions = 0xffff;
      tx_rx_sweep[i].rxPM = 0;
      tx_rx_sweep[i].expRxAGC = 0;
      tx_rx_sweep[i].rxFlag = 1;
   }

   int seg_num = 0;
   // Segment 0, Set channel and mode, do not turn on PA
   tx_rx_sweep[seg_num].opcode = 0x0001;
   tx_rx_sweep[seg_num].ulChan = CHAN_LIST[0];
   tx_rx_sweep[seg_num].rfmode = PHONE_MODE_TDSCDMA_B34;
   tx_rx_sweep[seg_num].paState = 255; // no-op
   tx_rx_sweep[seg_num].paBias = 511;
   tx_rx_sweep[seg_num].rgi = 0;

   int NUM_NOP_SEGMENTS = 9;

   // Segment 1-9, no op for Set mode channel to take effect
   for(j = 0; j < NUM_NOP_SEGMENTS; j++)
   {
      seg_num++;
      tx_rx_sweep[seg_num].opcode = 0x0000;
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[0];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_TDSCDMA_B34;
      tx_rx_sweep[seg_num].paState = 255; // no-op
      tx_rx_sweep[seg_num].paBias = 511;
      tx_rx_sweep[seg_num].rgi = 0;
   }

   // Fill out the Tx measurment segments for the Highest gain state sweep
   for (i=0; i<NUM_CHANS; i++)
   {
      if (i!=0) // retune op2
      {
         tx_rx_sweep[seg_num].opcode = 0x0002;
         tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
         tx_rx_sweep[seg_num].rfmode = PHONE_MODE_TDSCDMA_B34;
         tx_rx_sweep[seg_num].paState = 255; // no-op
         tx_rx_sweep[seg_num].paBias = 511;
         tx_rx_sweep[seg_num].rgi = 0;

         // add no-op for changing CH
         seg_num++;
         tx_rx_sweep[seg_num].opcode = 0x0000;
         tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
         tx_rx_sweep[seg_num].rfmode = PHONE_MODE_TDSCDMA_B34;
         tx_rx_sweep[seg_num].paState = 255; // no-op
         tx_rx_sweep[seg_num].paBias = 511;
         tx_rx_sweep[seg_num].rgi = 0;
      }
      seg_num++;

      int per_freq_seg_num = 0;
      tx_rx_sweep[seg_num].opcode = 0x0018;  // Tx+HDET only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_TDSCDMA_B34;;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 2
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  // full bias
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].numHdetAvg = 1;
      tx_rx_sweep[seg_num].txPurpose = 5;

      // dummy measurement results, for NV  generation demo only. Those vaule are got after we excuting the sweeep
      tx_rx_response[seg_num].aiTxPower=(word)((23+(double) (CHAN_LIST[i]-refChannel)/100.00)*100);//Unit dB100 this is different with Tx lin. dummy Tx measurement 23dB , this should be get from callbox after excuting the sweep
      tx_rx_response[seg_num].aiHDETResults= 1350; // dummy HDET result from device.

      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0008;  // Tx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_TDSCDMA_B34;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 2
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].txPurpose = 4;

      // dummy measurement results, for NV demo only
      tx_rx_response[seg_num].aiTxPower=(word)((11+ (double) (CHAN_LIST[i]-refChannel)/100.00)*100);//Unit dB100 this is different with Tx lin. dummy Tx measurement 11dB , this should be get from callbox after excuting the sweep

      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0008;  // Tx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_TDSCDMA_B34;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 1
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].txPurpose = 4;

      // dummy measurement results, for NV demo only
      tx_rx_response[seg_num].aiTxPower=(word)((10+(double) (CHAN_LIST[i]-refChannel)/100.00)*100);//Unit dB100 this is different with Tx lin. dummy Tx measurement 10dB , this should be get from callbox after excuting the sweep

      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0008;  // Tx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_TDSCDMA_B34;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 0
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].txPurpose = 4;

      // dummy measurement results, for NV demo only
      tx_rx_response[seg_num].aiTxPower=(word)((0+(double) (CHAN_LIST[i]-refChannel)/100.00)*100);//Unit dB100 this is different with Tx lin. dummy Tx measurement 0dB , this should be get from callbox after excuting the sweep

      per_freq_seg_num++;
      seg_num++;
      NUM_RX_MEAS=0;
      tx_rx_sweep[seg_num].opcode = 0x0060;  // Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_TDSCDMA_B34;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // dummy gain state
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x1 << PER_FREQ_LNA[NUM_RX_MEAS];
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[NUM_RX_MEAS];

      // dummy measurement results, for NV  generation demo only. Those vaule are got after we excuting the sweeep
      tx_rx_response[seg_num].aiRxResults[0][PER_FREQ_LNA[NUM_RX_MEAS]]=-61+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for primary and LNA0

      per_freq_seg_num++;
      seg_num++;
      NUM_RX_MEAS++;

      tx_rx_sweep[seg_num].opcode = 0x0060;  // Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_TDSCDMA_B34;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // dummy gain state
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x1 << PER_FREQ_LNA[NUM_RX_MEAS];
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[NUM_RX_MEAS];

      // dummy measurement results, for NV  generation demo only. Those vaule are got after we excuting the sweeep
      tx_rx_response[seg_num].aiRxResults[0][PER_FREQ_LNA[NUM_RX_MEAS]]=-41+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for primary and LNA1

      per_freq_seg_num++;
      seg_num++;
      NUM_RX_MEAS++;

      tx_rx_sweep[seg_num].opcode = 0x0060;  // Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_TDSCDMA_B34;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // dummy gain state
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x1 << PER_FREQ_LNA[NUM_RX_MEAS];
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[NUM_RX_MEAS];

      // dummy measurement results, for NV  generation demo only. Those vaule are got after we excuting the sweeep
      tx_rx_response[seg_num].aiRxResults[0][PER_FREQ_LNA[NUM_RX_MEAS]]=-31+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for primary and LNA2

      per_freq_seg_num++;
      seg_num++;
      NUM_RX_MEAS++;

      tx_rx_sweep[seg_num].opcode = 0x0060;  // Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_TDSCDMA_B34;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // dummy gain state
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x1 << PER_FREQ_LNA[NUM_RX_MEAS];
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[NUM_RX_MEAS];

      // dummy measurement results, for NV  generation demo only. Those vaule are got after we excuting the sweeep
      tx_rx_response[seg_num].aiRxResults[0][PER_FREQ_LNA[NUM_RX_MEAS]]=-21+(CHAN_LIST[i]-refChannel)/100; // dummy Rx result from device, for primary and LNA3
   }

   int tx_rx_index=seg_num;
#ifdef ON_TARGET
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_rx_sweep, seg_num+1, true, true);
#endif
   /*   Step 5. We now have the following data

   a. Tx Linearizer sweep from full bias
   b. Tx Linearizer sweep from APT bias
   c. Tx Freq compenstation from APT bias

   Let's genereate NV with QMSL helper function
   */

   // We using QMSL to generate NV
   FTM_Cal_Sweep_Request ftm_request[1000];
   FTM_Cal_Sweep_Response ftm_response[1000];
   QMSL_Tx_Cal_Complete_NV_Type aTx_NV_Structure;
   QMSL_Tx_Cal_NV_Gen_Info aTx_Lin_Cal_NV_Info;

   memset((void*) &aTx_NV_Structure, 0, sizeof(QMSL_Tx_Cal_Complete_NV_Type));

   // i is used to keep track the number of records in ftm_resquest/response

   // fill in first Tx sweep

   // For Tx linearizer, fill in Tx power data from  Non-APT sweep in ascending order

   for(i=0,j=0; j<start_index; j++,i++) // config, no-op and waveform segments, no results
   {
      ftm_request[i]=tx_Sweep[j];
   }

   for (j=0; j<PA_2_Step; j++, i++)
   {
      ftm_request[i]=tx_Sweep[PA_2_Step-j-1+start_index];
      ftm_response[i].aiTxPower = (word)(PA_2_Full_Bias_TxSweepPower[PA_2_Step-j-1]*10); //for Tx linearizer. The unit is dB10
      ftm_response[i].aiHDETResults=HDET_From_full_TxSweep[PA_2_Step-j-1]; // HDET for high PA;
   }

   ftm_request[i]=tx_Sweep[j+start_index];
   j++,i++;    // no op code between PAs

   for(j = 0; j < PA_1_Step; j++, i++)
   {
      ftm_request[i]=tx_Sweep[PA_1_Step - j-1+start_index_1];
      ftm_response[i].aiTxPower = (word)(PA_1_Full_Bias_TxSweepPower[PA_1_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
   }

   ftm_request[i]=tx_Sweep[j+start_index_1];
   j++,i++;    // no op code between PAs

   for(j = 0; j < PA_0_Step; j++, i++)
   {
      ftm_request[i]=tx_Sweep[PA_0_Step - j-1+start_index_2];
      ftm_response[i].aiTxPower = (word)(PA_0_Full_Bias_TxSweepPower[PA_0_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
   }

   // For Tx linearizer, fill in Tx power data from  APT sweep in ascending order

   for(j=0; j<apt_start_index; j++,i++) // config, no-op and waveform segments, no results
   {
      ftm_request[i]=apt_tx_Sweep[j];
   }

   for (j=0; j<PA_2_Step; j++, i++)
   {
      ftm_request[i]=apt_tx_Sweep[PA_2_Step-j-1+apt_start_index];
      ftm_response[i].aiTxPower=(word)(PA_2_APT_TxSweepPower[PA_2_Step-j-1]*10); //for Tx linearizer. The unit is dB10
      ftm_response[i].aiHDETResults=HDET_From_APT_TxSweep[PA_2_Step-j-1]; // HDET for high PA;
   }

   ftm_request[i]=apt_tx_Sweep[j+apt_start_index];
   j++,i++;    // no op code between PAs

   for(j = 0; j < PA_1_Step; j++, i++)
   {
      ftm_request[i]=apt_tx_Sweep[PA_1_Step - j-1+apt_start_index_1];
      ftm_response[i].aiTxPower = (word)(PA_1_APT_TxSweepPower[PA_1_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
   }

   ftm_request[i]=apt_tx_Sweep[j+apt_start_index_1];
   j++,i++;    // no op code between PAs

   for(j = 0; j < PA_0_Step; j++, i++)
   {
      ftm_request[i]=apt_tx_Sweep[PA_0_Step - j-1+apt_start_index_2];
      ftm_response[i].aiTxPower = (word)(PA_0_APT_TxSweepPower[PA_0_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
   }

   //fill in V3 tx/rx segments

   for(j=0; j<seg_num; j++,i++)
   {
      ftm_request[i]=tx_rx_sweep[j];
      // fill in the dummy tx/rx measurement results, it should be replaced my real value
      ftm_response[i]=tx_rx_response[j];
   }

   memset((void*)&aTx_Lin_Cal_NV_Info, 0, sizeof(QMSL_Tx_Cal_NV_Gen_Info));

   for(j = 0; j < 16; j++)
      aTx_Lin_Cal_NV_Info.freq_comp_ch_list[j] = CHAN_LIST[j];  // Fill the channel list

   aTx_Lin_Cal_NV_Info.freq_comp_ref_ch_list[0] = refChannel;
   aTx_Lin_Cal_NV_Info.freq_comp_ref_ch_list[1] = refChannel;
   aTx_Lin_Cal_NV_Info.freq_comp_ref_ch_list[2] = refChannel;

   aTx_Lin_Cal_NV_Info.rgi_threshold_list[0][0] = 128;
   aTx_Lin_Cal_NV_Info.rgi_threshold_list[1][0] = 128;
   aTx_Lin_Cal_NV_Info.rgi_threshold_list[2][0] = 128;

   aTx_Lin_Cal_NV_Info.freq_comp_rgi_list[0][0] = 87;
   aTx_Lin_Cal_NV_Info.freq_comp_rgi_list[1][0] = 89;
   aTx_Lin_Cal_NV_Info.freq_comp_rgi_list[2][0] = 80; // must be the same as in the Tx/Rx request

   aTx_Lin_Cal_NV_Info.freq_comp_bias_list[0][0] = 780;
   aTx_Lin_Cal_NV_Info.freq_comp_bias_list[1][0] = 780;
   aTx_Lin_Cal_NV_Info.freq_comp_bias_list[2][0] = 780;

   for(j=0; j<3; j++)
   {
      aTx_Lin_Cal_NV_Info.upper_bound_chan_list[0][j][0] = 10121; // the last operating channel of the band
      aTx_Lin_Cal_NV_Info.upper_bound_chan_list[1][j][0] = 10121; // the last operating channel of the band
      aTx_Lin_Cal_NV_Info.upper_bound_chan_list[2][j][0] = 10121; // the last operating channel of the band

      aTx_Lin_Cal_NV_Info.ref_chan_list[0][j][0] = refChannel;
      aTx_Lin_Cal_NV_Info.ref_chan_list[1][j][0] = refChannel;
      aTx_Lin_Cal_NV_Info.ref_chan_list[2][j][0] = refChannel;
   }

   unsigned short numRecords = i; // the number of record in the request/response

   unsigned short numLin = 0 ;
   // Decides which NV should be written to. Prx or SV. 0 = PRx, 2= SV Path.
   unsigned short nvPath = 0;
   _bOk= QLIB_RFCAL_Complete_TxRxCal( g_hResourceContext,
                                      NV_MODE_TDSCDMA_B34,
                                      (void*)ftm_request,
                                      (void*)ftm_response,
                                      (void*)&aTx_NV_Structure,
                                      numRecords,
                                      (void*)&aTx_Lin_Cal_NV_Info,
                                      &numLin,
                                      nvPath,
                                      true );

   return (_bOk!=0);
}


unsigned char Test_RFCal_V3_CDMA()
{
   unsigned char _bOk = FALSE;

   /* This is a complete example showing V3 RFCAL API to calibrate Tx Linearizer and Freq compensation in CDMA SV */

   // Step 1. Calibrate the reference channel Tx linearizer with full bias
   //
   //  CDMA SV BC 0
   //  Ref Channel : 358
   //  No of gain ranges : 3

   //  PA Gain State 0 RGI value 90 to 8 (lowest power range)
   //  PA Gain State 1 RGI value 98 to 46
   //  PA Gain State 2 RGI value 104 to 46
   //  Sweep time 4 ms
   int txSweepTime = 4 * 1000;
   int PA_State_0_Start_RGI = 90;
   int PA_State_0_Stop_RGI = 8;
   int PA_0_Step = (PA_State_0_Start_RGI - PA_State_0_Stop_RGI)/2 + 1;
   int PA_0_RGI_List[100] = {0};

   int i = 0;
   for(i = 0; i < PA_0_Step; i++)
      PA_0_RGI_List[i] = PA_State_0_Start_RGI - i*2; //step of 2

   // First Sweep for the lowest gain state
   FTM_Cal_Sweep_Request tx_Sweep[100] = {0};

   // Step 1a. Tx Sweep for the lowest gain state with full bias

   // Initialize the data structure with some default value first, these values can stay constant for Tx Sweep
   i =0;
   for( i = 0; i < 100; i++)
   {
      tx_Sweep[i].segLen = txSweepTime;
      tx_Sweep[i].masterDev = 4;
      tx_Sweep[i].slaveDev = -1;
      tx_Sweep[i].txBW = 3; // don't care for CDMA
      tx_Sweep[i].rxBW = 3; // don't care for CDMA
      tx_Sweep[i].chanOffset = 0;
      tx_Sweep[i].txWaveform = 0xFF; //don't care for CDMA
      tx_Sweep[i].numRBs = 1;  //don't care for CDMA
      tx_Sweep[i].startRB = 1;  //don't care for CDMA
      tx_Sweep[i].txFlag = 4;     //ASK: TPL log show, but why?
      tx_Sweep[i].numLpmHdetAvg = 0; // NA
      tx_Sweep[i].rxActions = 0xffff;
      tx_Sweep[i].rxPM = 0;
      tx_Sweep[i].expRxAGC = 0;
      tx_Sweep[i].rxFlag = 4; //
   }

   // Segment 0, Set channel and mode, do not turn on PA
   tx_Sweep[0].opcode = 0x0001;
   tx_Sweep[0].ulChan = 358;
   tx_Sweep[0].rfmode = PHONE_MODE_CDMA_800;
   tx_Sweep[0].paState = 255; // no-op
   tx_Sweep[0].paBias = 511;
   tx_Sweep[0].rgi = 0;

   // Segment 1,2,..., 14 no op for Set mode channel to take effect
   int j = 1;
   for(j = 1; j < 15; j++)
   {
      tx_Sweep[j].opcode = 0x0000;
      tx_Sweep[j].ulChan = 358;
      tx_Sweep[j].rfmode = PHONE_MODE_CDMA_800;
      tx_Sweep[j].paState = 255; // no-op
      tx_Sweep[j].paBias = 511;
      tx_Sweep[j].rgi = 0;
   }

   // Fill out the Tx measurment segments for the lowest gain state sweep

   for (j = 15, i = 0; j < 15 + PA_0_Step; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008;  // Tx only
      tx_Sweep[j].ulChan = 358;
      tx_Sweep[j].rfmode = PHONE_MODE_CDMA_800;
      tx_Sweep[j].paState = 0; // gain state 0
      tx_Sweep[j].paBias = 780;  // full bias
      tx_Sweep[j].rgi = PA_0_RGI_List[i];
   }

#ifdef ON_TARGET
   // Do the sweep now
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, 15 +  PA_0_Step, true, true);
#endif
   //
   double PA_0_Full_Bias_TxSweepPower[] = {8.7922,8.3392,10.3263,9.40605,8.52212,7.64832,6.7948,5.54797,4.84576,3.30863,2.26802,0.681982 ,-0.680539 ,-1.93018 ,-3.20909 ,-3.80326 ,-4.72046 ,-5.96836 ,-7.40409 ,-8.46133 ,-9.31835 ,-10.613,-11.5872 ,-12.1776,-13.4433 ,-14.6568 ,-15.7627 ,-16.4599 ,-17.8257 ,-18.5306 ,-19.4421 ,-20.1641  ,-21.1417 ,-22.0893 ,-23.1426 ,-24.2322 ,-25.1349 ,-26.3931 ,-27.382,-28.3529 ,-29.6546 ,-30.5533 ,-31.5913 ,-32.588,-33.3519 ,-34.6479 ,-35.4996 ,-36.3917 ,-37.6081 ,-38.7961 ,-39.6655 ,-40.7097 ,-41.649,-42.1643 ,-43.4134 ,-44.4205 ,-45.6775 ,-46.3545 ,-47.7381 ,-48.6947 ,-49.4765 ,-50.4999 ,-51.2719 ,-52.8444 ,-53.5628 ,-54.3752 ,-55.5677 ,-56.4503 ,-57.0001, -58.00, -59.00};

   // Step 1b. Tx Sweep for the mid gain state with full bias

   // Fill out the Tx measurment segments for the mid gain state sweep

   int PA_State_1_Start_RGI = 98;
   int PA_State_1_Stop_RGI = 46;
   int PA_1_Step = PA_State_1_Start_RGI - PA_State_1_Stop_RGI + 1;
   int PA_1_RGI_List[100] = {0};

   for(i = 0; i < PA_1_Step; i++)
      PA_1_RGI_List[i] = PA_State_1_Start_RGI - i;

   for (j = 15,i=0 ; j < 15 + PA_1_Step; j++,i++)
   {
      tx_Sweep[j].opcode = 0x0008;  // Tx only
      tx_Sweep[j].ulChan = 358;
      tx_Sweep[j].rfmode = PHONE_MODE_CDMA_800;
      tx_Sweep[j].paState = 1; // gain state 1
      tx_Sweep[j].paBias = 780;  // full bias
      tx_Sweep[j].rgi = PA_1_RGI_List[i];
   }

#ifdef ON_TARGET
   // Do the sweep now
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, 15 +  (PA_State_1_Start_RGI - PA_State_1_Stop_RGI + 1), true, true);
#endif
   double PA_1_Full_Bias_TxSweepPower [] = {18.6136,183563,16.1894,14.9659,14.2183,12.5902,11.659,10.0364,9.03667,7.83073,6.55035,5.86259,4.94074,3.68435,2.32647,1.24407,0.348305 ,-0.889938 ,-1.9073,-2.49533 ,-3.71631 ,-4.9891,-6.20284 ,-6.94702 ,-8.38484 ,-9.17325 ,-10.1187 ,-10.9375 ,-11.9455 ,-12.9494 ,-14.058,-15.201,-16.1316 };

   // Fill out the Tx measurment segments for the highest gain state sweep

   int PA_State_2_Start_RGI = 104;
   int PA_State_2_Stop_RGI = 46;
   int PA_2_Step = PA_State_2_Start_RGI - PA_State_2_Stop_RGI + 1;
   int PA_2_RGI_List[100] = {0};

   for (i=0; i < PA_2_Step; i++)
   {
      PA_2_RGI_List[i] = PA_State_2_Start_RGI - i;
   }

   for (j=15,i=0 ; j < 15 + PA_2_Step ; j++,i++)
   {
      tx_Sweep[j].opcode = 0x0008 | 0x0010;  // Tx only + HDET
      tx_Sweep[j].ulChan = 358;
      tx_Sweep[j].rfmode = PHONE_MODE_CDMA_800;
      tx_Sweep[j].paState = 2; // gain state 1
      tx_Sweep[j].paBias = 780;  // full bias
      tx_Sweep[j].rgi = PA_2_RGI_List[i];
   }

#ifdef ON_TARGET
   // Do the sweep now
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, 15 +  (PA_State_2_Start_RGI - PA_State_2_Stop_RGI + 1), true, true);
#endif
   double PA_2_Full_Bias_TxSweepPower [] = {29.553,29.3768,28.0079,27.9645,27.8797,27.7472,27.6505,27.4032,27.084,26.8496,26.2173,25.2984,24.4916,23.4588,22.4198,21.0196,20.0503,19.1171,17.8396,16.4744,15.5705,14.4194,13.2002,12.5551,11.6693,10.4813,9.20122,8.16376,7.3062,6.11219,5.12689,4.55511,3.36967,2.13347,0.936465 ,0.197406 ,-1.22543 ,-2.00547 ,-2.9504,-3.75347 ,-5.00606 ,-6.2295,-6.90237};

   // Step 2. Intrepolate the variable bias from APT characterization table (aka XML file)
   // PA_x_APT_Bias and PA_x_APT_Power is stored in XML file

   int PA_0_APT_Bias[] = {480,420,400,380,380,360,320,320,280,280,240,240,200,200,160,140,120,100,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60};
   double PA_0_APT_Power[] = {5.433867,4.116132,2.983078,2.033866,1.081464,-0.0997,-1.4541,-2.28083,-3.67938,-4.5101,-6.11619,-7.04209,-8.7524,-9.54831,-11.3396,-13.0561,-15.571,-18.0931,-29.4008,-30.2083,-31.2841,-32.151,-33.2694,-33.8432,-34.3986,-36.0445,-36.9354,-38.0216,-39.2292,-39.8021,-40.5624,-41.6455,-43.5973,-44.3158,-45.3047,-45.7548,-47.0205,-46.76,-47.9527,-48.7175,-49.7061};
   int PA_0_APT_Table_Size = 41;

   double PA_1_APT_Power[] = {13.11615,11.53298,9.836257,8.128462,6.450947,4.576341,2.474556,1.032771,-0.60761,-2.30493,-3.34674,-5.30305,-7.67902,-8.45372,-11.1359,-12.1641,-16.7438,-36.2594};
   int PA_1_APT_Bias[] = {540,460,400,340,300,260,220,200,180,160,160,140,120,120,100,100,80,40};
   int PA_1_APT_Table_Size = 18;

   double   PA_2_APT_Power  [] =   {27.13036,   26.68166,25.9205,25.17268,24.38837,22.36934,21.20683,19.73007,18.52205,16.92891,15.35257,14.02675,12.76735,11.64804,9.489923,7.610068,5.861471,4.189881,2.197023,-0.38291,-0.86796,-1.80032,-5.26277,-6.29092,-11.2984,-12.1616,-12.9575,-14.0143};
   int      PA_2_APT_Bias   [] = {1020, 1020, 1020,760,720,600,540,480,440,380,340,320,300,280,240,200,180,160,140,120,120,120,100,100,80,80,80,80};
   int      PA_2_APT_Table_Size = 28;

   int PA_0_Interpolated_Bias[100] = {0};
   int PA_1_Interpolated_Bias[100] = {0};
   int PA_2_Interpolated_Bias[100] = {0};

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_0_APT_Power,
                                    PA_0_APT_Bias,
                                    PA_0_APT_Table_Size,
                                    PA_0_Full_Bias_TxSweepPower,
                                    PA_0_Interpolated_Bias,
                                    PA_0_Step,
                                    0,
                                    480); // max bias from APT characterization

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_1_APT_Power,
                                    PA_1_APT_Bias,
                                    PA_1_APT_Table_Size,
                                    PA_1_Full_Bias_TxSweepPower,
                                    PA_1_Interpolated_Bias,
                                    PA_1_Step,
                                    0,
                                    540);

   QLIB_RFCAL_LinearInterpolation(  g_hResourceContext,
                                    PA_2_APT_Power,
                                    PA_2_APT_Bias,
                                    PA_2_APT_Table_Size,
                                    PA_2_Full_Bias_TxSweepPower,
                                    PA_2_Interpolated_Bias,
                                    PA_2_Step,
                                    0,
                                    1020);

   // Step 3. Do APT sweep

   // Step 3a. Fill out APT bias for lowest gain state

   //3, 1 for config, 2 for no-op
   for (j = 15, i = 0 ; j < 15 + PA_0_Step; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008;  // Tx only
      tx_Sweep[j].ulChan = 358;
      tx_Sweep[j].rfmode = PHONE_MODE_CDMA_800;
      tx_Sweep[j].paState = 0; // gain state 0
      tx_Sweep[j].paBias = PA_0_Interpolated_Bias[i];
      tx_Sweep[j].rgi = PA_0_RGI_List[i];
   }

#ifdef ON_TARGET
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, 15 +  PA_0_Step, true, true);
#endif
   // Take the measurment now

   // PA_0_APT_TxSweepPower is the tx power measurment from call box with APT bias applied
   double PA_0_APT_TxSweepPower [] = {11.2233,11.0263,10.2479,9.33088,8.45754,7.58718,6.73514,5.49106,4.79581,3.26336,2.22763,0.648227,-0.708466,-1.94969,-3.22224,-3.81357,-4.7371,-5.97922,-7.4138,-8.46871,-9.3311,-10.6145,-11.5955,-12.1783,-13.4395,-14.6453,-15.7577,-16.4528,-17.8058,-18.5227,-19.427,-20.1546,-21.1417,-22.0747,-23.13,-24.2222,-25.1162,-26.374,-27.3717,-28.3447,-29.6422,-30.5466,-31.5971,-32.587,-33.3498,-34.6432,-35.4997,-36.3923,-37.6046,-38.79,-39.6657,-40.714,-41.6453,-42.1585,-43.4085,-44.4165,-45.6828,-46.3583,-47.755,-48.6919,-49.4685,-50.4926,-51.2698,-52.8179,-53.5593,-54.377,-55.5307,-56.4636,-57.0173, -58.232, -59.232};

   for (j = 15, i = 0 ; j < 15 + PA_1_Step; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008;  // Tx only
      tx_Sweep[j].ulChan = 358;
      tx_Sweep[j].rfmode = PHONE_MODE_CDMA_800;
      tx_Sweep[j].paState = 1; // gain state 1
      tx_Sweep[j].paBias = PA_1_Interpolated_Bias[i];  // APT bias
      tx_Sweep[j].rgi = PA_1_RGI_List[i];
   }
#ifdef ON_TARGET
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, 15 +  PA_1_Step, true, true);
#endif
   // Take the measurment now
   // PA_1_APT_TxSweepPower is the tx power measurment from call box with APT bias applied
   double PA_1_APT_TxSweepPower [] = {17.9671,17.0501,16.1578,14.9328,14.1859,12.5563,11.6339,10.0023,9.00028,7.8028,6.52831,5.84559,4.91298,3.66889,2.31378,1.22268,0.330287,-0.908478,-1.93042,-2.51327,-3.72631,-5.00556,-6.21686,-6.95938,-8.40609,-9.198,-10.1356,-10.9591,-11.971,-12.9657,-14.0721,-15.2113,-16.1497};

   for (j = 15, i = 0 ; j < 15 + PA_2_Step ; j++, i++)
   {
      tx_Sweep[j].opcode = 0x0008 | 0x0010;  // Tx only + HDET
      tx_Sweep[j].ulChan = 358;
      tx_Sweep[j].rfmode = PHONE_MODE_CDMA_800;
      tx_Sweep[j].paState = 2; // gain state 2
      tx_Sweep[j].paBias = PA_2_Interpolated_Bias[i];  // APT bias
      tx_Sweep[j].rgi = PA_2_RGI_List[i];
   }
#ifdef ON_TARGET
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_Sweep, 15 +  PA_2_Step, true, true);
#endif
   // Take the measurment now
   // PA_2_APT_TxSweepPower is the tx power measurment from call box with APT bias applied
   double PA_2_APT_TxSweepPower[] = {28.0763,28.023,27.995,27.9527,27.8654,27.7327,27.6362,27.3874,27.0628,26.8288,26.1902,25.2782,24.4688,23.4343,22.393,20.9947,20.0224,19.0999,17.8212,16.4593,15.5538,14.3995,13.1819,12.5399,11.6579,10.4721,9.18352,8.1477,7.29736,6.09944,5.11969,4.54309,3.35679,2.11891,0.923091,0.181213,-1.23352,-2.02318,-2.95674,-3.7634,-5.00823,-6.24356,-6.9174};

   // HDET_From_APT_TxSweep is retrieved from mobile during APT Tx sweep
   int HDET_From_APT_TxSweep[] = {3135,3089,3083,3071,3042,2987,2957,2859,2753,2675,2474,2218,2016,1784,1584,1350,1217,1110,982,882,825,769,724,704,680,656,637,624,616,607,601,599,594,590,586,585,583,584,582,581,580,580,580};

   // V3 Freq

   /* This is a untested example showing V3 RFCAL API to calibrate Tx/Rx Freq Comp in Cal v3 Fashion */

   // Step 1. Calibrate the reference channel Tx linearizer with full bias
   //
   //  CDMA BC 0
   //  Ref Channel : 358
   //  No of gain ranges : 3

   //Segments
   //1. RFMode Config (needs 60ms)
   //2. 60ms of RFMode (so 14 NOP segments of 4ms each)
   //Per Channel
   //3. PA Hi Lim vs. Freq Seg + LNA0 (DVGA)
   //4. PA Hi Lin vs. Freq Seg + LNA 1
   //5. PA Md Lin vs. Freq Seg + LNA 2
   //6. PA Lo Lin vs. Freq Seg + LNA 3
   //0. Retune if not last channel - else teardown

   //  Sweep time 4 ms

   txSweepTime = 4 * 1000;
   int PER_FREQ_PA_LIST[] = {2,2,1,0};
   int PER_FREQ_RGI_LIST[] = {91,67,86,76}; // Note : This should come from power converted to RGI based on APT linearizer data
   int PER_FREQ_BIAS_LIST[] = {780,500,780,560};//Note : This should come from second sweep linearizer + offset to accountfor freq comp.
   int CHAN_LIST[] = {1018,46,98,150,202,254,306,358,410,462,514,566,618,670,722,774};
   int NUM_MEAS_SEGS_PER_FREQ = 4;
   int NUM_CHANS = 16;

   int PER_FREQ_EXP_AGC[] = {-70,-70,-50,-42};
   int PER_FREQ_LNA[]= {0,1,2,3};

#ifdef BUFFER_SIZE
#undef BUFFER_SIZE
#endif
#define  BUFFER_SIZE 300
   // First Sweep for the lowest gain state
   FTM_Cal_Sweep_Request tx_rx_sweep[BUFFER_SIZE] = {0};

   // Step 1a. Tx Sweep for the lowest gain state with full bias
   i = 0;
   j = 0;

   for( i = 0; i < BUFFER_SIZE; i++)
   {
      tx_rx_sweep[i].segLen = txSweepTime;
      tx_rx_sweep[i].masterDev = 4;
      tx_rx_sweep[i].slaveDev = -1;
      tx_rx_sweep[i].txBW = 3;   //don't care for CDMA
      tx_rx_sweep[i].rxBW = 3;   //don't care for CDMA
      tx_rx_sweep[i].chanOffset = 0;
      tx_rx_sweep[i].txWaveform = 0xFF;  //don't care for CDMA
      tx_rx_sweep[i].numRBs = 12;  //don't care for CDMA
      tx_rx_sweep[i].startRB = 19; //don't care for CDMA
      tx_rx_sweep[i].txFlag = 1;
      tx_rx_sweep[i].numHdetAvg = 0;
      tx_rx_sweep[i].numLpmHdetAvg = 0; // NA
      tx_rx_sweep[i].rxActions = 0xffff;
      tx_rx_sweep[i].rxPM = 0;
      tx_rx_sweep[i].expRxAGC = 0;
      tx_rx_sweep[i].rxFlag = 0;
   }

   int seg_num = 0;
   // Segment 0, Set channel and mode, do not turn on PA
   tx_rx_sweep[seg_num].opcode = 0x0001;
   tx_rx_sweep[seg_num].ulChan = CHAN_LIST[0];
   tx_rx_sweep[seg_num].rfmode = PHONE_MODE_CDMA_800;
   tx_rx_sweep[seg_num].paState = 255; // no-op
   tx_rx_sweep[seg_num].paBias = 511;
   tx_rx_sweep[seg_num].rgi = 0;

   int NUM_NOP_SEGMENTS = 14;

   // Segment 1-14, no op for Set mode channel to take effect
   for(j = 0; j < NUM_NOP_SEGMENTS; j++)
   {
      seg_num++;
      tx_rx_sweep[seg_num].opcode = 0x0000;
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[0];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_CDMA_800;
      tx_rx_sweep[seg_num].paState = 255; // no-op
      tx_rx_sweep[seg_num].paBias = 511;
      tx_rx_sweep[seg_num].rgi = 0;
   }

   // Fill out the Tx measurment segments for the Highest gain state sweep
   for (i=0; i<NUM_CHANS; i++)
   {
      if (i==0) // first channel op4 waveform
      {
         tx_rx_sweep[seg_num].opcode = 0x0004;
         tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
         tx_rx_sweep[seg_num].rfmode = PHONE_MODE_CDMA_800;
         tx_rx_sweep[i].txWaveform = 0x01;
         tx_rx_sweep[seg_num].paState = 255; // no-op
         tx_rx_sweep[seg_num].paBias = 511;
         tx_rx_sweep[seg_num].rgi = 0;
      }
      else // retune op2
      {
         tx_rx_sweep[seg_num].opcode = 0x0004;
         tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
         tx_rx_sweep[seg_num].rfmode = PHONE_MODE_CDMA_800;
         tx_rx_sweep[seg_num].paState = 255; // no-op
         tx_rx_sweep[seg_num].paBias = 511;
         tx_rx_sweep[seg_num].rgi = 0;
      }
      // add no-op for changing CH
      seg_num++;
      tx_rx_sweep[seg_num].opcode = 0x0000;
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_CDMA_800;
      tx_rx_sweep[seg_num].paState = 255; // no-op
      tx_rx_sweep[seg_num].paBias = 511;
      tx_rx_sweep[seg_num].rgi = 0;

      seg_num++;
      int per_freq_seg_num = 0;

      tx_rx_sweep[seg_num].opcode = 0x0078;  // Tx+Rx+HDET only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_CDMA_800;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 2
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  // full bias
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x2 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x4; //PRx + DRx
      tx_rx_sweep[seg_num].numHdetAvg = 5;

      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0068;  // Tx+Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_CDMA_800;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 2
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x2 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x4; //PRx + DRx

      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0068;  // Tx+Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_CDMA_800;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 1
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x2 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x4; //PRx + DRx

      per_freq_seg_num++;
      seg_num++;

      tx_rx_sweep[seg_num].opcode = 0x0068;  // Tx+Rx only
      tx_rx_sweep[seg_num].ulChan = CHAN_LIST[i];
      tx_rx_sweep[seg_num].rfmode = PHONE_MODE_CDMA_800;
      tx_rx_sweep[seg_num].paState = PER_FREQ_PA_LIST[per_freq_seg_num]; // gain state 0
      tx_rx_sweep[seg_num].paBias = PER_FREQ_BIAS_LIST[per_freq_seg_num];  //
      tx_rx_sweep[seg_num].rgi = PER_FREQ_RGI_LIST[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxActions = 0x2 << PER_FREQ_LNA[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxPM = 0;
      tx_rx_sweep[seg_num].expRxAGC = PER_FREQ_EXP_AGC[per_freq_seg_num];
      tx_rx_sweep[seg_num].rxFlag = 0x4; //PRx + DRx
   }

#ifdef ON_TARGET
   QLIB_FTM_CFG_AND_EXEC_CAL_SWEEP(g_hResourceContext, (void*)tx_rx_sweep, seg_num+1, true, true);
#endif

   /*
   Step 5. We now have the following data

   a. Tx Linearizer sweep from full bias
   b. Tx Linearizer sweep from APT bias

   Let's genereate NV with QMSL helper function or manually
   */

   // We using QMSL to generate NV
   FTM_Cal_Sweep_Request ftm_request[1000];
   FTM_Cal_Sweep_Response ftm_response[1000];
   QMSL_Tx_Cal_Complete_NV_Type aTx_NV_Structure;
   QMSL_Tx_Cal_NV_Gen_Info aTx_Lin_Cal_NV_Info;

   memset((void*) &aTx_NV_Structure, 0, sizeof(QMSL_Tx_Cal_Complete_NV_Type));

   int refChannel=358;
   // i is used to keep track the number of records in ftm_resquest/response

   // For Tx linearizer, fill in Tx power data from APT sweep in ascending order
   for(i = 0, j = 0; i < PA_0_Step; j++, i++)
   {
      ftm_request[i].txPurpose = 0x1;
      ftm_request[i].paState = 0;
      ftm_request[i].rgi = PA_0_RGI_List[PA_0_Step - j -1];
      ftm_request[i].paBias = PA_0_Interpolated_Bias[PA_0_Step - j-1];
      ftm_request[i].ulChan = refChannel;
      ftm_response[i].aiTxPower = (word)(PA_0_APT_TxSweepPower[PA_0_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
   }
   for(j = 0; j < PA_2_Step; j++, i++)
   {
      ftm_request[i].txPurpose = 0x1;
      ftm_request[i].paState = 2;
      ftm_request[i].rgi = PA_2_RGI_List[PA_2_Step - j-1];
      ftm_request[i].paBias = PA_2_Interpolated_Bias[PA_2_Step - j-1];
      ftm_request[i].ulChan = refChannel;
      ftm_response[i].aiTxPower = (word)(PA_2_APT_TxSweepPower[PA_2_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
   }
   for(j = 0; j < PA_1_Step; j++, i++)
   {
      ftm_request[i].txPurpose = 0x1;
      ftm_request[i].paState = 1;
      ftm_request[i].rgi = PA_1_RGI_List[PA_1_Step - j-1];
      ftm_request[i].paBias = PA_1_Interpolated_Bias[PA_1_Step - j-1];
      ftm_request[i].ulChan = refChannel;
      ftm_response[i].aiTxPower = (word)(PA_1_APT_TxSweepPower[PA_1_Step - j-1] * 10); //for Tx linearizer. The unit is dB10
   }
   memset((void*)&aTx_Lin_Cal_NV_Info, 0, sizeof(QMSL_Tx_Cal_NV_Gen_Info));
   unsigned short numRecords = i; // the number of record in the request/response

   unsigned short numLin = 0 ;
   // Decides which NV should be written to. Prx or SV. 0 = PRx, 2= SV Path.
   unsigned short nvPath = 2;
   _bOk= QLIB_RFCAL_Complete_TxRxCal( g_hResourceContext,
                                      NV_MODE_LTE_B1,
                                      (void*)&ftm_request,
                                      (void*)&ftm_response,
                                      (void*)&aTx_NV_Structure,
                                      numRecords,
                                      (void*)&aTx_Lin_Cal_NV_Info,
                                      &numLin,
                                      nvPath,
                                      true );

   return (_bOk!=0);
}

unsigned char ESC_Parser(bool parse_resp)
{
   char reqfile[1000];
   char resfile[1000];
   char outputfile[1000];

   FILE * fp;
   FILE * fp1;
   FILE * fp2;

   int i = 0;
   int j = 0;
   byte*  ftm_req_data;             /* Write buffer                             */
   byte*  ftm_resp_data;             /* Write buffer                             */
   byte buffer[4000];
   byte buffer_resp[4000];
   int numberOfChar = 0;
   int numberOfChar_resp = 0;
   word opcode_list[4000];
   dword subopcode_list[4000];//used for response packet parser
   byte hdet[4000];
   byte lpmhdet[4000];
   byte rxflag[4000];
   word rxactions[4000];
   byte rxpm[4000];
   word chanOffset[4000];

   ftm_cal_sweep_config_req_pkt_type* header; /* Header */
   ftm_calibration_config_packet_type* cal_pkt; /* Calibration Packet */
   byte* payload; /* Actual Payload Start */
   ftm_cal_instr_payload_config_radio_type* cfgSeg_payload;
   ftm_cal_instr_payload_retune_radio_type* retuneSeg_payload;
   ftm_cal_instr_payload_config_tx_wf_type* cfgWfmSeg_payload;
   ftm_cal_instr_payload_config_tx_type* cfgTxSeg_payload;
   ftm_cal_instr_payload_meas_hdet_type* measHDETSeg_payload;
   ftm_cal_instr_payload_config_rx_type* cfgRxSeg_payload;
   ftm_cal_instr_payload_meas_rx_type* measRxSeg_payload;
   //ftm_cal_instr_payload_ept_config_type* ept_config_payload;
   FTM_Cal_Sweep_Sub_Opcode_Req* subopcode;
   FTM_Cal_Sweep_XPT_Debug_Mode_Req* xpt_debug_mode;
   FTM_Cal_Sweep_EPT_Config_Req* ept_config_payload;
   FTM_Cal_Sweep_EPT_IQ_Cap_Req* ept_capture_payload;
   FTM_Cal_Sweep_EPT_Apply_DPD_Req* ept_applydpd_payload;
   FTM_Cal_Sweep_XPT_FB_Config* ept_fbrx_config_payload;
   FTM_Cal_Sweep_XPT_Tx_Path_Gain* ept_tx_path_gain_payload;
   ftm_cal_sweep_config_resp_pkt_type* header_resp; /* Header */
   byte* payload_resp;  /* Actual Payload Start for Response Packet */

   //Get Output File
   printf("Enter Output File Location : ");
   scanf("%s", outputfile);
   fp1 = fopen(outputfile,"w");
   if( fp1 == NULL )
   {
      printf("Error opening Output File");
      return false;
   }

   //Get Request File
   printf("Enter Request File Location : ");
   scanf("%s", reqfile);
   fp = fopen(reqfile,"r");
   if( fp == NULL )
   {
      printf("Error opening Request File");
      return false;
   }
   fprintf(fp1,"Parsing Request File: %s \n",reqfile);

   //Get Response File if required
   if( parse_resp )
   {
      printf("Enter Result File Location : ");
      scanf("%s", resfile);
      fp2 = fopen(resfile,"r");
      if( fp2 == NULL )
      {
         printf("Error opening Result File");
         return false;
      }
      fprintf(fp1,"Parsing Result File: %s \n",resfile);
   }

   i = 0;
   while(!feof(fp))
   {
      numberOfChar += fscanf(fp,"%02x",&buffer[i]);
      // Debug
      //fprintf(fp1,"(0x%02x) - Number of Char read = %d, Char = %d\n", buffer[i], numberOfChar, buffer[i]);
      i++;
   }
   numberOfChar++; // Because fscanf returns a -1 for the last byte read attempt
   fprintf(fp1,"Number of Request Bytes Read = %d.\n", numberOfChar);

   //ftm_req_data = (byte *) malloc(numberOfChar * sizeof(byte));
   ftm_req_data = &buffer[0];

   if(parse_resp == true)
   {
      j = 0;
      while(!feof(fp2))
      {
         numberOfChar_resp += fscanf(fp2,"%02x",&buffer_resp[j]);
         // Debug
         //fprintf(fp1,"(0x%02x) - Number of Char read = %d, Char = %x\n", numberOfChar, buffer[i]);
         j++;
      }
      numberOfChar_resp++; // Because fscanf returns a -1 for the last byte read attempt
      fprintf(fp1,"Number of Result Bytes Read = %d.\n", numberOfChar_resp);

      //ftm_resp_data = (byte *) malloc(numberOfChar_resp * sizeof(byte));
      ftm_resp_data = &buffer_resp[0];
   }

   // REQUEST PACKET PARSING
   /* Typecast header to extract the header info. */
   fprintf(fp1,"\n");
   fprintf(fp1,"\n");
   fprintf(fp1,"----------------------------------------\n");
   fprintf(fp1,"----------------------------------------\n");
   fprintf(fp1,"        REQUEST PACKET PARSING          \n");
   fprintf(fp1,"----------------------------------------\n");
   fprintf(fp1,"----------------------------------------\n");
   header = (ftm_cal_sweep_config_req_pkt_type*) ftm_req_data;
   fprintf(fp1,"Diag Packet Header Info:\n");
   fprintf(fp1,"(0x%02x) - Header Command : %d\n", header->cmd, header->cmd);
   fprintf(fp1,"(0x%02x) - Header SubSys : %d\n", header->sub_sys, header->sub_sys);
   fprintf(fp1,"(0x%04x) - Header Mode : %d\n", header->mode, header->mode);
   fprintf(fp1,"(0x%04x) - Header CDMA Command : %d\n", header->cdma_cmd, header->cdma_cmd);
   fprintf(fp1,"(0x%04x) - Header ReqLen : %d\n", header->req_len, header->req_len);
   fprintf(fp1,"(0x%04x) - Header RespLen : %d\n", header->rsp_len, header->rsp_len);
   fprintf(fp1,"\n");
   fprintf(fp1,"\n");

   // Debug
   //fprintf(fp1,"(0x%02x) - Header Cal Param : %d\n", header->cal_param);

   /* Typecast cal_param to extract the command header */
   cal_pkt = (ftm_calibration_config_packet_type*)(&(header->cal_param));

   fprintf(fp1,"Cal V3 Config Packet Header Info:\n");
   fprintf(fp1,"(0x%02x) - Cal Packet Start Cal Cmd : %d\n", cal_pkt->packet_header.start_cal_cmd, cal_pkt->packet_header.start_cal_cmd);
   fprintf(fp1,"(0x%02x) - Cal Packet Last Sweep : %d\n", cal_pkt->packet_header.last_sweep, cal_pkt->packet_header.last_sweep);
   fprintf(fp1,"(0x%02x) - Cal Packet Debug Mode : %d\n", cal_pkt->packet_header.debug_mode, cal_pkt->packet_header.debug_mode);
   fprintf(fp1,"(0x%04x) - Cal Packet Num of Segments : %d\n", cal_pkt->packet_header.num_of_segments, cal_pkt->packet_header.num_of_segments);
   fprintf(fp1,"(0x%04x) - Cal Packet Instr Size : %d\n", cal_pkt->packet_header.tot_instr_size, cal_pkt->packet_header.tot_instr_size);
   fprintf(fp1,"(0x%04x) - Cal Packet Cal Data Length : %d\n", cal_pkt->packet_header.cal_data_len, cal_pkt->packet_header.cal_data_len);
   fprintf(fp1,"\n");

   /* Get the actualy payload pointer */
   //payload = (char *) malloc(numberOfChar * sizeof(char));
   //fprintf(fp1,"(0x%02x) - Memory Allocated.\n");
   payload = &(cal_pkt->instr_ptr);
   // Debug
   //fprintf(fp1,"(0x%02x) - Payload Begin Address = %d\n", payload);
   /* Number of bytes in payload = numOfChars - Header */
   int header_len = 19;
   int num_bytes_parsed = 0;
   word* opcode;
   word* interval;
   int seg_count = 0;
   while(num_bytes_parsed < (numberOfChar - header_len))
   {
      fprintf(fp1,"\n");
      opcode = (word*)(payload);

      if( *opcode != 0xDAAD )
      {
         fprintf(fp1,"(0x%04x) - Opcode = b%016x\n", *opcode, ConvertUint16HextoBinary(*opcode));
         opcode_list[seg_count] = *opcode;
      }
      else
      {
         fprintf(fp1,"(0x%04x)", *opcode);
         seg_count--;
      }
      payload += 2;
      num_bytes_parsed += 2;

      if( *opcode != 0xDAAD )
      {
         interval = (word*)(payload);
         fprintf(fp1,"(0x%04x) - Interval = %d\n", *interval, *interval);
         payload += 2;
         num_bytes_parsed += 2;
      }

      switch(*opcode)
      {
         case 0x00:
         {
            fprintf(fp1,"NOP Segment\n");
            break;
         }
         case 0x01:
         {
            fprintf(fp1,"Config Segment\n");
            cfgSeg_payload = (ftm_cal_instr_payload_config_radio_type*)payload;
            fprintf(fp1,"(0x%02x) - Master Device : b%08x\n", cfgSeg_payload->device_master, ConvertUint8HextoBinary(cfgSeg_payload->device_master));
            fprintf(fp1,"(0x%08x) - Slave Device ID: %x\n", cfgSeg_payload->device_slave, cfgSeg_payload->device_slave);
            fprintf(fp1,"(0x%02x) - RF Mode : %d\n", cfgSeg_payload->rf_mode, cfgSeg_payload->rf_mode);
            fprintf(fp1,"(0x%04x) - Channel : %d\n", cfgSeg_payload->channel, cfgSeg_payload->channel);
            fprintf(fp1,"(0x%04x) - Rx BW : %d\n", cfgSeg_payload->rx_bandwidth, cfgSeg_payload->rx_bandwidth);
            fprintf(fp1,"(0x%04x) - Tx BW : %d\n", cfgSeg_payload->tx_bandwidth, cfgSeg_payload->tx_bandwidth);
            fprintf(fp1,"(0x%04x) - DC Offset : %d\n", cfgSeg_payload->dc_channel_offset, cfgSeg_payload->dc_channel_offset);
            chanOffset[seg_count] = cfgSeg_payload->dc_channel_offset;
            payload += sizeof(ftm_cal_instr_payload_config_radio_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_radio_type);
         }
         break;
         case 0x02:
         {
            fprintf(fp1,"Retune Segment\n");
            retuneSeg_payload = (ftm_cal_instr_payload_retune_radio_type*)payload;
            fprintf(fp1,"(0x%02x) - Master Device : b%08x\n", retuneSeg_payload->device_master, ConvertUint8HextoBinary(retuneSeg_payload->device_master));
            fprintf(fp1,"(0x%04x) - Channel : %d\n", retuneSeg_payload->channel, retuneSeg_payload->channel);
            fprintf(fp1,"(0x%04x) - DC Offset : %d\n", retuneSeg_payload->dc_channel_offset, retuneSeg_payload->dc_channel_offset);
            chanOffset[seg_count] = retuneSeg_payload->dc_channel_offset;
            payload += sizeof(ftm_cal_instr_payload_retune_radio_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_retune_radio_type);
         }
         break;
         case 0x04:
         {
            fprintf(fp1,"Waveform Segment\n");
            cfgWfmSeg_payload = (ftm_cal_instr_payload_config_tx_wf_type*)payload;
            fprintf(fp1,"(0x%02x) - Waveform : %d\n", cfgWfmSeg_payload->waveform, cfgWfmSeg_payload->waveform);
            fprintf(fp1,"(0x%02x) - Number of RBs : %d\n", cfgWfmSeg_payload->num_of_rb, cfgWfmSeg_payload->num_of_rb);
            fprintf(fp1,"(0x%02x) - Start RB : %d\n", cfgWfmSeg_payload->start_rb, cfgWfmSeg_payload->start_rb);
            payload += sizeof(ftm_cal_instr_payload_config_tx_wf_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_tx_wf_type);
         }
         break;
         case 0x08:
         {
            fprintf(fp1,"Config Tx Segment\n");
            cfgTxSeg_payload = (ftm_cal_instr_payload_config_tx_type*)payload;
            fprintf(fp1,"(0x%02x) - Cal Action : b%08x\n", cfgTxSeg_payload->cal_action, ConvertUint8HextoBinary(cfgTxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - PA State : %d\n", cfgTxSeg_payload->pa_state, cfgTxSeg_payload->pa_state);
            fprintf(fp1,"(0x%02x) - RGI : %d\n", cfgTxSeg_payload->rgi, cfgTxSeg_payload->rgi);
            fprintf(fp1,"(0x%08x) - PA Bias : %d\n", cfgTxSeg_payload->pa_bias, cfgTxSeg_payload->pa_bias);
            fprintf(fp1,"(0x%02x) - Tx Device : b%08x\n", cfgTxSeg_payload->device_flag, ConvertUint8HextoBinary(cfgTxSeg_payload->device_flag));
            payload += sizeof(ftm_cal_instr_payload_config_tx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_tx_type);
         }
         break;
         case 0x18:
         {
            fprintf(fp1,"Config Tx and HDET Segment\n");
            cfgTxSeg_payload = (ftm_cal_instr_payload_config_tx_type*)payload;
            fprintf(fp1,"(0x%02x) - Cal Action : b%08x\n", cfgTxSeg_payload->cal_action, ConvertUint8HextoBinary(cfgTxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - PA State : %d\n", cfgTxSeg_payload->pa_state, cfgTxSeg_payload->pa_state);
            fprintf(fp1,"(0x%02x) - RGI : %d\n", cfgTxSeg_payload->rgi, cfgTxSeg_payload->rgi);
            fprintf(fp1,"(0x%08x) - PA Bias : %d\n", cfgTxSeg_payload->pa_bias, cfgTxSeg_payload->pa_bias);
            fprintf(fp1,"(0x%02x) - Tx Device : b%08x\n", cfgTxSeg_payload->device_flag, ConvertUint8HextoBinary(cfgTxSeg_payload->device_flag));
            payload += sizeof(ftm_cal_instr_payload_config_tx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_tx_type);

            measHDETSeg_payload = (ftm_cal_instr_payload_meas_hdet_type*)payload;
            fprintf(fp1,"(0x%02x) - HDET Samples : %d\n", measHDETSeg_payload->hdet_sample, measHDETSeg_payload->hdet_sample);
            fprintf(fp1,"(0x%02x) - LPM HDET Samples : %d\n", measHDETSeg_payload->lpm_hdet_sample, measHDETSeg_payload->lpm_hdet_sample);
            fprintf(fp1,"(0x%02x) - Tx Device : b%08x\n", measHDETSeg_payload->device_flag, ConvertUint8HextoBinary(measHDETSeg_payload->device_flag));
            hdet[seg_count] = measHDETSeg_payload->hdet_sample;
            lpmhdet[seg_count] = measHDETSeg_payload->lpm_hdet_sample;
            payload += sizeof(ftm_cal_instr_payload_meas_hdet_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_meas_hdet_type);
         }
         break;
         case 0x68:
         {
            fprintf(fp1,"Config Tx and Config Rx and Meas Rx Segment\n");
            cfgTxSeg_payload = (ftm_cal_instr_payload_config_tx_type*)payload;
            fprintf(fp1,"(0x%02x) - Cal Action : b%08x\n", cfgTxSeg_payload->cal_action, ConvertUint8HextoBinary(cfgTxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - PA State : %d\n", cfgTxSeg_payload->pa_state, cfgTxSeg_payload->pa_state);
            fprintf(fp1,"(0x%02x) - RGI : %d\n", cfgTxSeg_payload->rgi, cfgTxSeg_payload->rgi);
            fprintf(fp1,"(0x%08x) - PA Bias : %d\n", cfgTxSeg_payload->pa_bias, cfgTxSeg_payload->pa_bias);
            fprintf(fp1,"(0x%02x) - Tx Device : b%08x\n", cfgTxSeg_payload->device_flag, ConvertUint8HextoBinary(cfgTxSeg_payload->device_flag));
            payload += sizeof(ftm_cal_instr_payload_config_tx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_tx_type);

            cfgRxSeg_payload = (ftm_cal_instr_payload_config_rx_type*)payload;
            fprintf(fp1,"(0x%04x) - Cal Action : b%016x\n", cfgRxSeg_payload->cal_action, ConvertUint16HextoBinary(cfgRxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - Rx Power Mode : %d\n", cfgRxSeg_payload->power_mode, cfgRxSeg_payload->power_mode);
            fprintf(fp1,"(0x%02x) - Rx Device : b%08x\n", cfgRxSeg_payload->device_flag, ConvertUint8HextoBinary(cfgRxSeg_payload->device_flag));
            rxactions[seg_count] = cfgRxSeg_payload->cal_action;
            rxflag[seg_count] = cfgRxSeg_payload->device_flag;
            rxpm[seg_count] = cfgRxSeg_payload->power_mode;
            payload += sizeof(ftm_cal_instr_payload_config_rx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_rx_type);

            measRxSeg_payload = (ftm_cal_instr_payload_meas_rx_type*)payload;
            fprintf(fp1,"(0x%04x) - Cal Action : b%016x\n", measRxSeg_payload->cal_action, ConvertUint16HextoBinary(measRxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - Rx Power Mode : %d\n", measRxSeg_payload->power_mode, measRxSeg_payload->power_mode);
            fprintf(fp1,"(0x%04x) - Expected RxAGC : %d\n", measRxSeg_payload->exp_rxagc, measRxSeg_payload->exp_rxagc);
            fprintf(fp1,"(0x%02x) - Rx Device : b%08x\n", measRxSeg_payload->device_flag, ConvertUint8HextoBinary(measRxSeg_payload->device_flag));
            payload += sizeof(ftm_cal_instr_payload_meas_rx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_meas_rx_type);
         }
         break;
         case 0x78:
         {
            fprintf(fp1,"Config Tx and HDET and Config Rx and Meas Rx Segment\n");
            cfgTxSeg_payload = (ftm_cal_instr_payload_config_tx_type*)payload;
            fprintf(fp1,"(0x%02x) - Cal Action : b%08x\n", cfgTxSeg_payload->cal_action, ConvertUint8HextoBinary(cfgTxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - PA State : %d\n", cfgTxSeg_payload->pa_state, cfgTxSeg_payload->pa_state);
            fprintf(fp1,"(0x%02x) - RGI : %d\n", cfgTxSeg_payload->rgi, cfgTxSeg_payload->rgi);
            fprintf(fp1,"(0x%08x) - PA Bias : %d\n", cfgTxSeg_payload->pa_bias, cfgTxSeg_payload->pa_bias);
            fprintf(fp1,"(0x%02x) - Tx Device : b%08x\n", cfgTxSeg_payload->device_flag, ConvertUint8HextoBinary(cfgTxSeg_payload->device_flag));
            payload += sizeof(ftm_cal_instr_payload_config_tx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_tx_type);

            measHDETSeg_payload = (ftm_cal_instr_payload_meas_hdet_type*)payload;
            fprintf(fp1,"(0x%02x) - HDET Samples : %d\n", measHDETSeg_payload->hdet_sample, measHDETSeg_payload->hdet_sample);
            fprintf(fp1,"(0x%02x) - LPM HDET Samples : %d\n", measHDETSeg_payload->lpm_hdet_sample, measHDETSeg_payload->lpm_hdet_sample);
            fprintf(fp1,"(0x%02x) - Tx Device : b%08x\n", measHDETSeg_payload->device_flag, ConvertUint8HextoBinary(measHDETSeg_payload->device_flag));
            hdet[seg_count] = measHDETSeg_payload->hdet_sample;
            lpmhdet[seg_count] = measHDETSeg_payload->lpm_hdet_sample;
            payload += sizeof(ftm_cal_instr_payload_meas_hdet_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_meas_hdet_type);

            cfgRxSeg_payload = (ftm_cal_instr_payload_config_rx_type*)payload;
            fprintf(fp1,"(0x%04x) - Cal Action : b%016x\n", cfgRxSeg_payload->cal_action, ConvertUint16HextoBinary(cfgRxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - Rx Power Mode : %d\n", cfgRxSeg_payload->power_mode, cfgRxSeg_payload->power_mode);
            fprintf(fp1,"(0x%02x) - Rx Device : b%08x\n", cfgRxSeg_payload->device_flag, ConvertUint8HextoBinary(cfgRxSeg_payload->device_flag));
            rxactions[seg_count] = cfgRxSeg_payload->cal_action;
            rxflag[seg_count] = cfgRxSeg_payload->device_flag;
            rxpm[seg_count] = cfgRxSeg_payload->power_mode;
            payload += sizeof(ftm_cal_instr_payload_config_rx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_rx_type);

            measRxSeg_payload = (ftm_cal_instr_payload_meas_rx_type*)payload;
            fprintf(fp1,"(0x%04x) - Cal Action : b%016x\n", measRxSeg_payload->cal_action, ConvertUint16HextoBinary(measRxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - Rx Power Mode : %d\n", measRxSeg_payload->power_mode, measRxSeg_payload->power_mode);
            fprintf(fp1,"(0x%04x) - Expected RxAGC : %d\n", measRxSeg_payload->exp_rxagc, measRxSeg_payload->exp_rxagc);
            fprintf(fp1,"(0x%02x) - Rx Device : b%08x\n", measRxSeg_payload->device_flag, ConvertUint8HextoBinary(measRxSeg_payload->device_flag));
            payload += sizeof(ftm_cal_instr_payload_meas_rx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_meas_rx_type);
         }
         break;
         case 0x60:
         {
            fprintf(fp1,"Config Rx and Meas Rx Segment\n");
            cfgRxSeg_payload = (ftm_cal_instr_payload_config_rx_type*)payload;
            fprintf(fp1,"(0x%04x) - Cal Action : b%016x\n", cfgRxSeg_payload->cal_action, ConvertUint16HextoBinary(cfgRxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - Rx Power Mode : %d\n", cfgRxSeg_payload->power_mode, cfgRxSeg_payload->power_mode);
            fprintf(fp1,"(0x%02x) - Rx Device : b%08x\n", cfgRxSeg_payload->device_flag, ConvertUint8HextoBinary(cfgRxSeg_payload->device_flag));
            rxactions[seg_count] = cfgRxSeg_payload->cal_action;
            rxflag[seg_count] = cfgRxSeg_payload->device_flag;
            rxpm[seg_count] = cfgRxSeg_payload->power_mode;
            payload += sizeof(ftm_cal_instr_payload_config_rx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_rx_type);

            measRxSeg_payload = (ftm_cal_instr_payload_meas_rx_type*)payload;
            fprintf(fp1,"(0x%04x) - Cal Action : b%016x\n", measRxSeg_payload->cal_action, ConvertUint16HextoBinary(measRxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - Rx Power Mode : %d\n", measRxSeg_payload->power_mode, measRxSeg_payload->power_mode);
            fprintf(fp1,"(0x%04x) - Expected RxAGC : %d\n", measRxSeg_payload->exp_rxagc, measRxSeg_payload->exp_rxagc);
            fprintf(fp1,"(0x%02x) - Rx Device : b%08x\n", measRxSeg_payload->device_flag, ConvertUint8HextoBinary(measRxSeg_payload->device_flag));
            payload += sizeof(ftm_cal_instr_payload_meas_rx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_meas_rx_type);
         }
         break;
         case 0x80: // EPT
         {
            fprintf(fp1,"XPT FB Rx and Tx Path Gain Segment\n");
            xpt_debug_mode = (FTM_Cal_Sweep_XPT_Debug_Mode_Req*)payload;
            fprintf(fp1,"(0x%02x) -Debug Mode : b%08x\n", xpt_debug_mode->debug_mode, ConvertUint8HextoBinary(xpt_debug_mode->debug_mode));
            payload += sizeof(FTM_Cal_Sweep_XPT_Debug_Mode_Req);
            num_bytes_parsed += sizeof(FTM_Cal_Sweep_XPT_Debug_Mode_Req);

            subopcode = (FTM_Cal_Sweep_Sub_Opcode_Req*)payload;
            fprintf(fp1,"(0x%08x) -SubOpcode : b%032x\n", subopcode->sub_opcode, ConvertUint32HextoBinary(subopcode->sub_opcode));
            subopcode_list[seg_count]= subopcode->sub_opcode;//Used during the response packet parser
            payload += sizeof(FTM_Cal_Sweep_Sub_Opcode_Req);
            num_bytes_parsed += sizeof(FTM_Cal_Sweep_Sub_Opcode_Req);

            if( subopcode->sub_opcode & 0x08 ) /* FB Path */
            {
               ept_fbrx_config_payload = (FTM_Cal_Sweep_XPT_FB_Config*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_fbrx_config_payload->version, ept_fbrx_config_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_fbrx_config_payload->flag, ept_fbrx_config_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_fbrx_config_payload->prim_device, ept_fbrx_config_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_fbrx_config_payload->fb_device, ept_fbrx_config_payload->fb_device);
               fprintf(fp1,"(0x%04x) -FB Component : %d\n", ept_fbrx_config_payload->fb_component, ept_fbrx_config_payload->fb_component);
               fprintf(fp1,"(0x%04x) -FB Com. State : %d\n", ept_fbrx_config_payload->fb_component_state, ept_fbrx_config_payload->fb_component_state);
               payload += sizeof(FTM_Cal_Sweep_XPT_FB_Config);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_XPT_FB_Config);
            }
            if( subopcode->sub_opcode & 0x10 ) /* Tx_path_gain */
            {
               ept_tx_path_gain_payload = (FTM_Cal_Sweep_XPT_Tx_Path_Gain*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_tx_path_gain_payload->version, ept_tx_path_gain_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_tx_path_gain_payload->flag, ept_tx_path_gain_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_tx_path_gain_payload->prim_device, ept_tx_path_gain_payload->prim_device);
               fprintf(fp1,"(0x%02x) -Tx_Comp : %d\n", ept_tx_path_gain_payload->tx_component, ept_tx_path_gain_payload->tx_component);
               fprintf(fp1,"(0x%08x) -IQ_Gain : %d\n", ept_tx_path_gain_payload->TxCompGain[0], ept_tx_path_gain_payload->TxCompGain[0]);
               fprintf(fp1,"(0x%08x) -Env Scale Gain : %d\n", ept_tx_path_gain_payload->TxCompGain[1], ept_tx_path_gain_payload->TxCompGain[1]);
               payload += sizeof(FTM_Cal_Sweep_XPT_Tx_Path_Gain);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_XPT_Tx_Path_Gain);
            }
         }
         break;
         case 0x88: // EPT + Config Tx
         {
            fprintf(fp1,"Config Tx and EPT Segment\n");
            cfgTxSeg_payload = (ftm_cal_instr_payload_config_tx_type*)payload;
            fprintf(fp1,"(0x%02x) - Cal Action : b%08x\n", cfgTxSeg_payload->cal_action, ConvertUint8HextoBinary(cfgTxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - PA State : %d\n", cfgTxSeg_payload->pa_state, cfgTxSeg_payload->pa_state);
            fprintf(fp1,"(0x%02x) - RGI : %d\n", cfgTxSeg_payload->rgi, cfgTxSeg_payload->rgi);
            fprintf(fp1,"(0x%08x) - PA Bias : %d\n", cfgTxSeg_payload->pa_bias, cfgTxSeg_payload->pa_bias);
            fprintf(fp1,"(0x%02x) - Tx Device : b%08x\n", cfgTxSeg_payload->device_flag, ConvertUint8HextoBinary(cfgTxSeg_payload->device_flag));
            payload += sizeof(ftm_cal_instr_payload_config_tx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_tx_type);

            xpt_debug_mode = (FTM_Cal_Sweep_XPT_Debug_Mode_Req*)payload;
            fprintf(fp1,"(0x%02x) -Debug Mode : b%08x\n", xpt_debug_mode->debug_mode, ConvertUint8HextoBinary(xpt_debug_mode->debug_mode));
            payload += sizeof(FTM_Cal_Sweep_XPT_Debug_Mode_Req);
            num_bytes_parsed += sizeof(FTM_Cal_Sweep_XPT_Debug_Mode_Req);

            subopcode = (FTM_Cal_Sweep_Sub_Opcode_Req*)payload;
            fprintf(fp1,"(0x%08x) -SubOpcode : b%032x\n", subopcode->sub_opcode, ConvertUint32HextoBinary(subopcode->sub_opcode));
            subopcode_list[seg_count]= subopcode->sub_opcode;//Used during the response packet parser
            payload += sizeof(FTM_Cal_Sweep_Sub_Opcode_Req);
            num_bytes_parsed += sizeof(FTM_Cal_Sweep_Sub_Opcode_Req);

            if( subopcode->sub_opcode &  0x01 ) /*EPT Config */
            {
               ept_config_payload = (FTM_Cal_Sweep_EPT_Config_Req*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_config_payload->version, ept_config_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_config_payload->flag, ept_config_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_config_payload->prim_device, ept_config_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_config_payload->fb_device, ept_config_payload->fb_device);
               payload += sizeof(FTM_Cal_Sweep_EPT_Config_Req);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_EPT_Config_Req);
            }
            if( subopcode->sub_opcode &  0x02 ) /*EPT Capture */
            {
               ept_capture_payload = (FTM_Cal_Sweep_EPT_IQ_Cap_Req*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_capture_payload->version, ept_capture_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_capture_payload->flag, ept_capture_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_capture_payload->prim_device, ept_capture_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_capture_payload->fb_device, ept_capture_payload->fb_device);
               fprintf(fp1,"(0x%04x) -Num_Samples : %d\n", ept_capture_payload->num_samples, ept_capture_payload->num_samples);
               fprintf(fp1,"(0x%02x) -First Trig : %d\n", ept_capture_payload->first_trigger, ept_capture_payload->first_trigger);
               fprintf(fp1,"(0x%02x) -Last Trig : %d\n", ept_capture_payload->last_trigger, ept_capture_payload->last_trigger);
               fprintf(fp1,"(0x%04x) -DPD Index : %d\n", ept_capture_payload->dpd_index, ept_capture_payload->dpd_index);
               payload += sizeof(FTM_Cal_Sweep_EPT_IQ_Cap_Req);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_EPT_IQ_Cap_Req);
            }
            if( subopcode->sub_opcode & 0x04 ) /* EPT Apply DPD */
            {
               ept_applydpd_payload = (FTM_Cal_Sweep_EPT_Apply_DPD_Req*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_applydpd_payload->version, ept_applydpd_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_applydpd_payload->flag, ept_applydpd_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_applydpd_payload->prim_device, ept_applydpd_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_applydpd_payload->fb_device, ept_applydpd_payload->fb_device);
               fprintf(fp1,"(0x%04x) -DPD Index : %d\n", ept_applydpd_payload->dpd_index, ept_applydpd_payload->dpd_index);
               payload += sizeof(FTM_Cal_Sweep_EPT_Apply_DPD_Req);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_EPT_Apply_DPD_Req);
            }
            if( subopcode->sub_opcode & 0x08 ) /* FB Path */
            {
               ept_fbrx_config_payload = (FTM_Cal_Sweep_XPT_FB_Config*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_fbrx_config_payload->version, ept_fbrx_config_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_fbrx_config_payload->flag, ept_fbrx_config_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_fbrx_config_payload->prim_device, ept_fbrx_config_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_fbrx_config_payload->fb_device, ept_fbrx_config_payload->fb_device);
               fprintf(fp1,"(0x%04x) -FB Component : %d\n", ept_fbrx_config_payload->fb_component, ept_fbrx_config_payload->fb_component);
               fprintf(fp1,"(0x%04x) -FB Com. State : %d\n", ept_fbrx_config_payload->fb_component_state, ept_fbrx_config_payload->fb_component_state);
               payload += sizeof(FTM_Cal_Sweep_XPT_FB_Config);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_XPT_FB_Config);
            }
            if( subopcode->sub_opcode & 0x10 ) /* Tx_path_gain */
            {
               ept_tx_path_gain_payload = (FTM_Cal_Sweep_XPT_Tx_Path_Gain*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_tx_path_gain_payload->version, ept_tx_path_gain_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_tx_path_gain_payload->flag, ept_tx_path_gain_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_tx_path_gain_payload->prim_device, ept_tx_path_gain_payload->prim_device);
               fprintf(fp1,"(0x%02x) -Tx_Comp : %d\n", ept_tx_path_gain_payload->tx_component, ept_tx_path_gain_payload->tx_component);
               fprintf(fp1,"(0x%08x) -IQ_Gain : %d\n", ept_tx_path_gain_payload->TxCompGain[0], ept_tx_path_gain_payload->TxCompGain[0]);
               fprintf(fp1,"(0x%08x) -Env_Scale_Gain : %d\n", ept_tx_path_gain_payload->TxCompGain[1], ept_tx_path_gain_payload->TxCompGain[1]);
               fprintf(fp1,"(0x%08x) -pathGain[2] : %d\n", ept_tx_path_gain_payload->TxCompGain[2], ept_tx_path_gain_payload->TxCompGain[2]);
               payload += sizeof(FTM_Cal_Sweep_XPT_Tx_Path_Gain);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_XPT_Tx_Path_Gain);
            }
         }
         break;
         case 0x98:
         {
            fprintf(fp1,"EPT + Config Tx and HDET Segment\n");
            cfgTxSeg_payload = (ftm_cal_instr_payload_config_tx_type*)payload;
            fprintf(fp1,"(0x%02x) - Cal Action : b%08x\n", cfgTxSeg_payload->cal_action, ConvertUint8HextoBinary(cfgTxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - PA State : %d\n", cfgTxSeg_payload->pa_state, cfgTxSeg_payload->pa_state);
            fprintf(fp1,"(0x%02x) - RGI : %d\n", cfgTxSeg_payload->rgi, cfgTxSeg_payload->rgi);
            fprintf(fp1,"(0x%08x) - PA Bias : %d\n", cfgTxSeg_payload->pa_bias, cfgTxSeg_payload->pa_bias);
            fprintf(fp1,"(0x%02x) - Tx Device : b%08x\n", cfgTxSeg_payload->device_flag, ConvertUint8HextoBinary(cfgTxSeg_payload->device_flag));
            payload += sizeof(ftm_cal_instr_payload_config_tx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_tx_type);

            measHDETSeg_payload = (ftm_cal_instr_payload_meas_hdet_type*)payload;
            fprintf(fp1,"(0x%02x) - HDET Samples : %d\n", measHDETSeg_payload->hdet_sample, measHDETSeg_payload->hdet_sample);
            fprintf(fp1,"(0x%02x) - LPM HDET Samples : %d\n", measHDETSeg_payload->lpm_hdet_sample, measHDETSeg_payload->lpm_hdet_sample);
            fprintf(fp1,"(0x%02x) - Tx Device : b%08x\n", measHDETSeg_payload->device_flag, ConvertUint8HextoBinary(measHDETSeg_payload->device_flag));
            hdet[seg_count] = measHDETSeg_payload->hdet_sample;
            lpmhdet[seg_count] = measHDETSeg_payload->lpm_hdet_sample;
            payload += sizeof(ftm_cal_instr_payload_meas_hdet_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_meas_hdet_type);

            xpt_debug_mode = (FTM_Cal_Sweep_XPT_Debug_Mode_Req*)payload;
            fprintf(fp1,"(0x%02x) -Debug Mode : b%08x\n", xpt_debug_mode->debug_mode, xpt_debug_mode->debug_mode);
            payload += sizeof(FTM_Cal_Sweep_XPT_Debug_Mode_Req);
            num_bytes_parsed += sizeof(FTM_Cal_Sweep_XPT_Debug_Mode_Req);

            subopcode = (FTM_Cal_Sweep_Sub_Opcode_Req*)payload;
            fprintf(fp1,"(0x%08x) -SubOpcode : b%032x\n", subopcode->sub_opcode, ConvertUint32HextoBinary(subopcode->sub_opcode));
            subopcode_list[seg_count]= subopcode->sub_opcode;//Used during the response packet parser
            payload += sizeof(FTM_Cal_Sweep_Sub_Opcode_Req);
            num_bytes_parsed += sizeof(FTM_Cal_Sweep_Sub_Opcode_Req);

            if( subopcode->sub_opcode &  0x01 ) /*EPT Config */
            {
               ept_config_payload = (FTM_Cal_Sweep_EPT_Config_Req*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_config_payload->version, ept_config_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_config_payload->flag, ept_config_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_config_payload->prim_device, ept_config_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_config_payload->fb_device, ept_config_payload->fb_device);
               payload += sizeof(FTM_Cal_Sweep_EPT_Config_Req);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_EPT_Config_Req);
            }
            if( subopcode->sub_opcode &  0x02 ) /*EPT Capture */
            {
               ept_capture_payload = (FTM_Cal_Sweep_EPT_IQ_Cap_Req*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_capture_payload->version, ept_capture_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_capture_payload->flag, ept_capture_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_capture_payload->prim_device, ept_capture_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_capture_payload->fb_device, ept_capture_payload->fb_device);
               fprintf(fp1,"(0x%04x) -Num_Samples : %d\n", ept_capture_payload->num_samples, ept_capture_payload->num_samples);
               fprintf(fp1,"(0x%02x) -First Trig : %d\n", ept_capture_payload->first_trigger, ept_capture_payload->first_trigger);
               fprintf(fp1,"(0x%02x) -Last Trig : %d\n", ept_capture_payload->last_trigger, ept_capture_payload->last_trigger);
               fprintf(fp1,"(0x%04x) -DPD Index : %d\n", ept_capture_payload->dpd_index, ept_capture_payload->dpd_index);
               payload += sizeof(FTM_Cal_Sweep_EPT_IQ_Cap_Req);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_EPT_IQ_Cap_Req);
            }
            if( subopcode->sub_opcode & 0x04 ) /* EPT Apply DPD */
            {
               ept_applydpd_payload = (FTM_Cal_Sweep_EPT_Apply_DPD_Req*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_applydpd_payload->version, ept_applydpd_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_applydpd_payload->flag, ept_applydpd_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_applydpd_payload->prim_device, ept_applydpd_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_applydpd_payload->fb_device, ept_applydpd_payload->fb_device);
               fprintf(fp1,"(0x%04x) -DPD Index : %d\n", ept_applydpd_payload->dpd_index, ept_applydpd_payload->dpd_index);
               payload += sizeof(FTM_Cal_Sweep_EPT_Apply_DPD_Req);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_EPT_Apply_DPD_Req);
            }
            if( subopcode->sub_opcode & 0x08 ) /* FB Path */
            {
               ept_fbrx_config_payload = (FTM_Cal_Sweep_XPT_FB_Config*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_fbrx_config_payload->version, ept_fbrx_config_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_fbrx_config_payload->flag, ept_fbrx_config_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_fbrx_config_payload->prim_device, ept_fbrx_config_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_fbrx_config_payload->fb_device, ept_fbrx_config_payload->fb_device);
               fprintf(fp1,"(0x%04x) -FB Component : %d\n", ept_fbrx_config_payload->fb_component, ept_fbrx_config_payload->fb_component);
               fprintf(fp1,"(0x%04x) -FB Com. State : %d\n", ept_fbrx_config_payload->fb_component_state, ept_fbrx_config_payload->fb_component_state);
               payload += sizeof(FTM_Cal_Sweep_XPT_FB_Config);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_XPT_FB_Config);
            }
            if( subopcode->sub_opcode & 0x10 ) /* Tx_path_gain */
            {
               ept_tx_path_gain_payload = (FTM_Cal_Sweep_XPT_Tx_Path_Gain*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_tx_path_gain_payload->version, ept_tx_path_gain_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_tx_path_gain_payload->flag, ept_tx_path_gain_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_tx_path_gain_payload->prim_device, ept_tx_path_gain_payload->prim_device);
               fprintf(fp1,"(0x%02x) -Tx_Comp : %d\n", ept_tx_path_gain_payload->tx_component, ept_tx_path_gain_payload->tx_component);
               fprintf(fp1,"(0x%08x) -IQ_Gain : %d\n", ept_tx_path_gain_payload->TxCompGain[0], ept_tx_path_gain_payload->TxCompGain[0]);
               fprintf(fp1,"(0x%08x) -Env_Scale_Gain : %d\n", ept_tx_path_gain_payload->TxCompGain[1], ept_tx_path_gain_payload->TxCompGain[1]);
               fprintf(fp1,"(0x%08x) -pathGain[2] : %d\n", ept_tx_path_gain_payload->TxCompGain[2], ept_tx_path_gain_payload->TxCompGain[2]);
               payload += sizeof(FTM_Cal_Sweep_XPT_Tx_Path_Gain);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_XPT_Tx_Path_Gain);
            }
         }
         break;
         case 0x00E8: //Rx will only be turned on with Apply DPD
         {
            fprintf(fp1,"Config Tx and Config Rx and Meas Rx and EPT Apply DPD Segment\n");
            cfgTxSeg_payload = (ftm_cal_instr_payload_config_tx_type*)payload;
            fprintf(fp1,"(0x%02x) - Cal Action : b%08x\n", cfgTxSeg_payload->cal_action, ConvertUint8HextoBinary(cfgTxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - PA State : %d\n", cfgTxSeg_payload->pa_state, cfgTxSeg_payload->pa_state);
            fprintf(fp1,"(0x%02x) - RGI : %d\n", cfgTxSeg_payload->rgi, cfgTxSeg_payload->rgi);
            fprintf(fp1,"(0x%08x) - PA Bias : %d\n", cfgTxSeg_payload->pa_bias, cfgTxSeg_payload->pa_bias);
            fprintf(fp1,"(0x%02x) - Tx Device : b%08x\n", cfgTxSeg_payload->device_flag, ConvertUint8HextoBinary(cfgTxSeg_payload->device_flag));
            payload += sizeof(ftm_cal_instr_payload_config_tx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_tx_type);

            cfgRxSeg_payload = (ftm_cal_instr_payload_config_rx_type*)payload;
            fprintf(fp1,"(0x%04x) - Cal Action : b%016x\n", cfgRxSeg_payload->cal_action, ConvertUint16HextoBinary(cfgRxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - Rx Power Mode : %d\n", cfgRxSeg_payload->power_mode, cfgRxSeg_payload->power_mode);
            fprintf(fp1,"(0x%02x) - Rx Device : b%08x\n", cfgRxSeg_payload->device_flag, ConvertUint8HextoBinary(cfgRxSeg_payload->device_flag));
            rxactions[seg_count] = cfgRxSeg_payload->cal_action;
            rxflag[seg_count] = cfgRxSeg_payload->device_flag;
            rxpm[seg_count] = cfgRxSeg_payload->power_mode;
            payload += sizeof(ftm_cal_instr_payload_config_rx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_config_rx_type);

            measRxSeg_payload = (ftm_cal_instr_payload_meas_rx_type*)payload;
            fprintf(fp1,"(0x%04x) - Cal Action : b%016x\n", measRxSeg_payload->cal_action, ConvertUint16HextoBinary(measRxSeg_payload->cal_action));
            fprintf(fp1,"(0x%02x) - Rx Power Mode : %d\n", measRxSeg_payload->power_mode, measRxSeg_payload->power_mode);
            fprintf(fp1,"(0x%04x) - Expected RxAGC : %d\n", measRxSeg_payload->exp_rxagc, measRxSeg_payload->exp_rxagc);
            fprintf(fp1,"(0x%02x) - Rx Device : b%08x\n", measRxSeg_payload->device_flag, ConvertUint8HextoBinary(measRxSeg_payload->device_flag));
            payload += sizeof(ftm_cal_instr_payload_meas_rx_type);
            num_bytes_parsed += sizeof(ftm_cal_instr_payload_meas_rx_type);

            xpt_debug_mode = (FTM_Cal_Sweep_XPT_Debug_Mode_Req*)payload;
            fprintf(fp1,"(0x%02x) -Debug Mode : b%08x\n", xpt_debug_mode->debug_mode, ConvertUint8HextoBinary(xpt_debug_mode->debug_mode));
            payload += sizeof(FTM_Cal_Sweep_XPT_Debug_Mode_Req);
            num_bytes_parsed += sizeof(FTM_Cal_Sweep_XPT_Debug_Mode_Req);

            subopcode = (FTM_Cal_Sweep_Sub_Opcode_Req*)payload;
            fprintf(fp1,"(0x%08x) -SubOpcode : b%032x\n", subopcode->sub_opcode, ConvertUint32HextoBinary(subopcode->sub_opcode));
            subopcode_list[seg_count]= subopcode->sub_opcode;//Used during the response packet parser
            payload += sizeof(FTM_Cal_Sweep_Sub_Opcode_Req);
            num_bytes_parsed += sizeof(FTM_Cal_Sweep_Sub_Opcode_Req);

            if( subopcode->sub_opcode &  0x01 ) /*EPT Config */
            {
               ept_config_payload = (FTM_Cal_Sweep_EPT_Config_Req*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_config_payload->version, ept_config_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_config_payload->flag, ept_config_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_config_payload->prim_device, ept_config_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_config_payload->fb_device, ept_config_payload->fb_device);
               payload += sizeof(FTM_Cal_Sweep_EPT_Config_Req);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_EPT_Config_Req);
            }
            if( subopcode->sub_opcode &  0x02 ) /*EPT Capture */
            {
               ept_capture_payload = (FTM_Cal_Sweep_EPT_IQ_Cap_Req*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_capture_payload->version, ept_capture_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_capture_payload->flag, ept_capture_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_capture_payload->prim_device, ept_capture_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_capture_payload->fb_device, ept_capture_payload->fb_device);
               fprintf(fp1,"(0x%04x) -Num_Samples : %d\n", ept_capture_payload->num_samples, ept_capture_payload->num_samples);
               fprintf(fp1,"(0x%02x) -First Trig : %d\n", ept_capture_payload->first_trigger, ept_capture_payload->first_trigger);
               fprintf(fp1,"(0x%02x) -Last Trig : %d\n", ept_capture_payload->last_trigger, ept_capture_payload->last_trigger);
               fprintf(fp1,"(0x%04x) -DPD Index : %d\n", ept_capture_payload->dpd_index, ept_capture_payload->dpd_index);
               payload += sizeof(FTM_Cal_Sweep_EPT_IQ_Cap_Req);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_EPT_IQ_Cap_Req);
            }
            if( subopcode->sub_opcode & 0x04 ) /* EPT Apply DPD */
            {
               ept_applydpd_payload = (FTM_Cal_Sweep_EPT_Apply_DPD_Req*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_applydpd_payload->version, ept_applydpd_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_applydpd_payload->flag, ept_applydpd_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_applydpd_payload->prim_device, ept_applydpd_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_applydpd_payload->fb_device, ept_applydpd_payload->fb_device);
               fprintf(fp1,"(0x%04x) -DPD Index : %d\n", ept_applydpd_payload->dpd_index, ept_applydpd_payload->dpd_index);
               payload += sizeof(FTM_Cal_Sweep_EPT_Apply_DPD_Req);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_EPT_Apply_DPD_Req);
            }
            if( subopcode->sub_opcode & 0x08 ) /* FB Path */
            {
               ept_fbrx_config_payload = (FTM_Cal_Sweep_XPT_FB_Config*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_fbrx_config_payload->version, ept_fbrx_config_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_fbrx_config_payload->flag, ept_fbrx_config_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_fbrx_config_payload->prim_device, ept_fbrx_config_payload->prim_device);
               fprintf(fp1,"(0x%02x) -FB_dev : %d\n", ept_fbrx_config_payload->fb_device, ept_fbrx_config_payload->fb_device);
               fprintf(fp1,"(0x%04x) -FB Component : %d\n", ept_fbrx_config_payload->fb_component, ept_fbrx_config_payload->fb_component);
               fprintf(fp1,"(0x%04x) -FB Com. State : %d\n", ept_fbrx_config_payload->fb_component_state, ept_fbrx_config_payload->fb_component_state);
               payload += sizeof(FTM_Cal_Sweep_XPT_FB_Config);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_XPT_FB_Config);
            }
            if( subopcode->sub_opcode & 0x10 ) /* Tx_path_gain */
            {
               ept_tx_path_gain_payload = (FTM_Cal_Sweep_XPT_Tx_Path_Gain*)payload;
               fprintf(fp1,"(0x%02x) -Version : %d\n", ept_tx_path_gain_payload->version, ept_tx_path_gain_payload->version);
               fprintf(fp1,"(0x%04x) -Flag : %d\n", ept_tx_path_gain_payload->flag, ept_tx_path_gain_payload->flag);
               fprintf(fp1,"(0x%02x) -Prim_Dev : %d\n", ept_tx_path_gain_payload->prim_device, ept_tx_path_gain_payload->prim_device);
               fprintf(fp1,"(0x%02x) -Tx_Comp : %d\n", ept_tx_path_gain_payload->tx_component, ept_tx_path_gain_payload->tx_component);
               fprintf(fp1,"(0x%08x) -IQ_Gain : %d\n", ept_tx_path_gain_payload->TxCompGain[0], ept_tx_path_gain_payload->TxCompGain[0]);
               fprintf(fp1,"(0x%08x) -Env_Scale_Gain : %d\n", ept_tx_path_gain_payload->TxCompGain[1], ept_tx_path_gain_payload->TxCompGain[1]);
               fprintf(fp1,"(0x%08x) -pathGain[2] : %d\n", ept_tx_path_gain_payload->TxCompGain[2], ept_tx_path_gain_payload->TxCompGain[2]);
               payload += sizeof(FTM_Cal_Sweep_XPT_Tx_Path_Gain);
               num_bytes_parsed += sizeof(FTM_Cal_Sweep_XPT_Tx_Path_Gain);
            }
         }
         break;
         case 0xDAAD:
         {
            fprintf(fp1," - Check Bits. End of Request File\n");
         }
         break;
         default:
         {
            fprintf(fp1,"Unknown Opcode. CHECK!!\n");
         }
         break;
      }
      seg_count++;
   }

   fprintf(fp1,"\n");
   fprintf(fp1,"\n");
   fprintf(fp1,"\n");

   // RESPONSE PACKET PARSING IF AVAILABLE
   if(parse_resp == true)
   {
      fprintf(fp1,"----------------------------------------\n");
      fprintf(fp1,"----------------------------------------\n");
      fprintf(fp1,"         RESULT PACKET PARSING          \n");
      fprintf(fp1,"----------------------------------------\n");
      fprintf(fp1,"----------------------------------------\n");
      /* Typecast header to extract the header info. */
      header_resp = (ftm_cal_sweep_config_resp_pkt_type*) ftm_resp_data;
      fprintf(fp1,"Diag Packet Header Info:\n");
      fprintf(fp1,"(0x%02x) - Header Command : %d\n", header_resp->cmd, header->cmd);
      fprintf(fp1,"(0x%02x) - Header SubSys : %d\n", header_resp->sub_sys, header->sub_sys);
      fprintf(fp1,"(0x%04x) - Header Mode : %d\n", header_resp->mode, header->mode);
      fprintf(fp1,"(0x%04x) - Header CDMA Command : %d\n", header_resp->cdma_cmd, header->cdma_cmd);
      fprintf(fp1,"(0x%04x) - Header ReqLen : %d\n", header_resp->req_len, header->req_len);
      fprintf(fp1,"(0x%04x) - Header RespLen : %d\n", header_resp->rsp_len, header->rsp_len);
      fprintf(fp1,"\n");
      fprintf(fp1,"\n");

      // Debug
      //fprintf(fp1,"(0x%02x) - Header Cal Param : %d\n", header->cal_param);

      fprintf(fp1,"Result Packet Extra Header Info:\n");
      ftm_resp_data += sizeof(ftm_cal_sweep_config_resp_pkt_type);
      fprintf(fp1,"(0x%02x) - Error Code : %d\n", *ftm_resp_data, *ftm_resp_data);
      ftm_resp_data += sizeof(char);
      dword* error_code1;
      error_code1 = (dword*)ftm_resp_data;
      fprintf(fp1,"(0x%08x) - Error Code 1 : %d\n", ConvertUint32HextoBinary(*error_code1), ConvertUint32HextoBinary(*error_code1));
      ftm_resp_data += sizeof(int);
      dword* error_code2;
      error_code2 = (dword*)ftm_resp_data;
      fprintf(fp1,"(0x%08x) - Error Code 2 : %d\n", ConvertUint32HextoBinary(*error_code2), ConvertUint32HextoBinary(*error_code2));
      ftm_resp_data += sizeof(int);
      word* err_seg_num;
      err_seg_num = (word*)ftm_resp_data;
      fprintf(fp1,"(0x%04x) - Error Seg Number : %d\n", *err_seg_num, *err_seg_num);
      ftm_resp_data += sizeof(word);
      fprintf(fp1,"(0x%02x) - Buffer Full Flag : %d\n", *ftm_resp_data, *ftm_resp_data);
      ftm_resp_data += sizeof(byte);
      word* payload_len;
      payload_len = (word*)ftm_resp_data;
      fprintf(fp1,"(0x%04x) - Payload Length : %d\n", *payload_len, *payload_len);
      ftm_resp_data += sizeof(word);

      // Debug
      //fprintf(fp1,"(0x%04x) - Opcode Array 0 : %d\n", ConvertUint16HextoBinary(opcode_list[0]), opcode_list[0]);
      //fprintf(fp1,"(0x%04x) - Opcode Array 1 : %d\n", ConvertUint16HextoBinary(opcode_list[1]), opcode_list[1]);

      //byte* payload;
      payload_resp = ftm_resp_data;

      int k;
      int chain, chain_mc;
      int gain_range = 0;
      int num_gain_ranges = 0;
      char chain_mask;
      int gr_mask;
      int ptrVar = 0;

      for( k = 0; k < seg_count; k++ )
      {
         fprintf(fp1,"\n");
         // Check the payload bytes for result values based on opcode for each segment.
         if( (opcode_list[k] & 0x0040) && !(opcode_list[k] & 0x0010) && !(opcode_list[k] & 0x0080)) //Rx Results only + No HDET + No EPT
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);

            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);

            chain = 0;
            chain_mc = 0;

            for( chain_mask = 0x01, chain = 0; chain < 8; chain_mask <<= 1, chain++ )
            {
               if( chain_mask & rxflag[k] )
               {
                  if( rxpm[k] == 0 )
                  {
                     // The first 8 bits indicate the 8 possible gain ranges supported
                     for( gr_mask = 0x0001, gain_range = 0; gain_range < 8; gr_mask <<= 1, gain_range++ )
                     {
                        if( gr_mask & rxactions[k] )
                        {
                           if( chanOffset[k] == 0 )
                           {
                              // PM0 Non DC Result
                              signed short* rx_res;
                              rx_res = (signed short*)payload_resp;
                              fprintf(fp1,"(0x%04x) - Rx Result Chain %d (PM0,SC) : %d\n", *rx_res, chain, *rx_res);
                              payload_resp += sizeof(word);
                           }
                           else
                           {
                              // PM0 DC Result
                              signed short* rx_res;
                              rx_res = (signed short*)payload_resp;
                              fprintf(fp1,"(0x%04x) - Rx Result Chain %d (PM0,DC) : %d\n", *rx_res, chain, *rx_res);
                              payload_resp += sizeof(word);
                           }
                        }
                     }
                  }
                  else
                  {
                     // The first 8 bits indicate the 8 possible gain ranges supported
                     for( gr_mask = 0x0001, gain_range = 0; gain_range < 8; gr_mask <<= 1, gain_range++ )
                     {
                        if( gr_mask & rxactions[k] )
                        {
                           signed short* rx_res_mc;
                           rx_res_mc = (signed short*)payload_resp;
                           fprintf(fp1,"(0x%04x) - Rx Result Chain %d (PM1,DC) : %d\n", *rx_res_mc, chain, *rx_res_mc);
                           payload_resp += sizeof(word);
                        }
                     }
                  }
               }
            }
         }
         else if( (opcode_list[k] & 0x0010) && !(opcode_list[k] & 0x0040) && !(opcode_list[k] & 0x0080)) //HDET Results only + No Rx + No EPT
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);

            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);

            if( *seg_len == 2 )
            {
               if( hdet[k] > 0 )
               {
                  word* hdet_res;
                  hdet_res = (word*)payload_resp;
                  fprintf(fp1,"(0x%04x) - HDET Result : %d\n", *hdet_res, *hdet_res);
                  payload_resp += sizeof(word);
               }
               else if( lpmhdet[k] > 0 )
               {
                  word* lpmhdet_res;
                  lpmhdet_res = (word*)payload_resp;
                  fprintf(fp1,"(0x%04x) - LPM HDET Result : %d\n", *lpmhdet_res, *lpmhdet_res);
                  payload_resp += sizeof(word);
               }
            }
            else if( *seg_len == 4 )
            {
               word* hdet_res;
               hdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - HDET Result : %d\n", *hdet_res, *hdet_res);
               payload_resp += sizeof(word);

               word* lpmhdet_res;
               lpmhdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - LPM HDET Result : %d\n", *lpmhdet_res, *lpmhdet_res);
               payload_resp += sizeof(word);
            }
         }
         //Rx Results and HDET Results + No EPT
         else if( (opcode_list[k] & 0x0040) && (opcode_list[k] & 0x0010)
                  && !(opcode_list[k] & 0x0080))
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);

            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);

            chain = 0;
            chain_mc = 0;

            for( chain_mask = 0x01, chain = 0; chain < 8; chain_mask <<= 1, chain++ )
            {
               if( chain_mask & rxflag[k] )
               {
                  if( rxpm[k] == 0 )
                  {
                     // The first 8 bits indicate the 8 possible gain ranges supported
                     for( gr_mask = 0x0001, gain_range = 0; gain_range < 8; gr_mask <<= 1, gain_range++ )
                     {
                        if( gr_mask & rxactions[k] )
                        {
                           if( chanOffset[k] == 0 )
                           {
                              // PM0 Non DC Result
                              signed short* rx_res;
                              rx_res = (signed short*)payload_resp;
                              fprintf(fp1,"(0x%04x) - Rx Result Chain %d (PM0,SC) : %d\n", *rx_res, chain, *rx_res);
                              payload_resp += sizeof(word);
                           }
                           else
                           {
                              // PM0 DC Result
                              signed short* rx_res;
                              rx_res = (signed short*)payload_resp;
                              fprintf(fp1,"(0x%04x) - Rx Result Chain %d (PM0,DC) : %d\n", *rx_res, chain, *rx_res);
                              payload_resp += sizeof(word);
                           }
                        }
                     }
                  }
                  else
                  {
                     // The first 8 bits indicate the 8 possible gain ranges supported
                     for( gr_mask = 0x0001, gain_range = 0; gain_range < 8; gr_mask <<= 1, gain_range++ )
                     {
                        if( gr_mask & rxactions[k] )
                        {
                           signed short* rx_res_mc;
                           rx_res_mc = (signed short*)payload_resp;
                           fprintf(fp1,"(0x%04x) - Rx Result Chain %d (PM1,DC) : %d\n", *rx_res_mc, chain, *rx_res_mc);
                           payload_resp += sizeof(word);
                        }
                     }
                  }
               }
            }

            if( (hdet[k] > 0) && !(lpmhdet[k] > 0) )
            {
               word* hdet_res;
               hdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - HDET Result : %d\n", *hdet_res, *hdet_res);
               payload_resp += sizeof(word);
            }
            else if( (lpmhdet[k] > 0) && !(hdet[k] > 0) )
            {
               word* lpmhdet_res;
               lpmhdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - LPM HDET Result : %d\n", *lpmhdet_res, *lpmhdet_res);
               payload_resp += sizeof(word);
            }
            else if( (lpmhdet[k] > 0) && (hdet[k] > 0) )
            {
               word* hdet_res;
               hdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - HDET Result : %d\n", *hdet_res, *hdet_res);
               payload_resp += sizeof(word);

               word* lpmhdet_res;
               lpmhdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - LPM HDET Result : %d\n", *lpmhdet_res, *lpmhdet_res);
               payload_resp += sizeof(word);
            }
         }

         //EPT ApplyDPD + HDET + RxMeas
         else if( ((opcode_list[k] & 0x0080) && (subopcode_list[k] & 0x0004)) //EPT + Apply DPD
                  && (opcode_list[k] & 0x0010) //HDET
                  && (opcode_list[k] & 0x0040)) //RxMeas
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);

            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);

            //Rx
            chain = 0;
            chain_mc = 0;

            for( chain_mask = 0x01, chain = 0; chain < 8; chain_mask <<= 1, chain++ )
            {
               if( chain_mask & rxflag[k] )
               {
                  if( rxpm[k] == 0 )
                  {
                     // The first 8 bits indicate the 8 possible gain ranges supported
                     for( gr_mask = 0x0001, gain_range = 0; gain_range < 8; gr_mask <<= 1, gain_range++ )
                     {
                        if( gr_mask & rxactions[k] )
                        {
                           if( chanOffset[k] == 0 )
                           {
                              // PM0 Non DC Result
                              signed short* rx_res;
                              rx_res = (signed short*)payload_resp;
                              fprintf(fp1,"(0x%04x) - Rx Result Chain %d (PM0,SC) : %d\n", *rx_res, chain, *rx_res);
                              payload_resp += sizeof(word);
                           }
                           else
                           {
                              // PM0 DC Result
                              signed short* rx_res;
                              rx_res = (signed short*)payload_resp;
                              fprintf(fp1,"(0x%04x) - Rx Result Chain %d (PM0,DC) : %d\n", *rx_res, chain, *rx_res);
                              payload_resp += sizeof(word);
                           }
                        }
                     }
                  }
                  else
                  {
                     // The first 8 bits indicate the 8 possible gain ranges supported
                     for( gr_mask = 0x0001, gain_range = 0; gain_range < 8; gr_mask <<= 1, gain_range++ )
                     {
                        if( gr_mask & rxactions[k] )
                        {
                           signed short* rx_res_mc;
                           rx_res_mc = (signed short*)payload_resp;
                           fprintf(fp1,"(0x%04x) - Rx Result Chain %d (PM1,DC) : %d\n", *rx_res_mc, chain, *rx_res_mc);
                           payload_resp += sizeof(word);
                        }
                     }
                  }
               }
            }
            //HDET
            if( (hdet[k] > 0) && !(lpmhdet[k] > 0) )
            {
               word* hdet_res;
               hdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - HDET Result : %d\n", *hdet_res, *hdet_res);
               payload_resp += sizeof(word);
            }
            else if( (lpmhdet[k] > 0) && !(hdet[k] > 0) )
            {
               word* lpmhdet_res;
               lpmhdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - LPM HDET Result : %d\n", *lpmhdet_res, *lpmhdet_res);
               payload_resp += sizeof(word);
            }
            else if( (lpmhdet[k] > 0) && (hdet[k] > 0) )
            {
               word* hdet_res;
               hdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - HDET Result : %d\n", *hdet_res, *hdet_res);
               payload_resp += sizeof(word);

               word* lpmhdet_res;
               lpmhdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - LPM HDET Result : %d\n", *lpmhdet_res, *lpmhdet_res);
               payload_resp += sizeof(word);
            }
            //EPT Apply DPD
            //Sub Opcode
            dword* subopcode;
            subopcode = (dword* )payload_resp;
            fprintf(fp1, "(0x%08x) - XPT Apply DPD, subcopde: b%032x\n", *subopcode, ConvertUint32HextoBinary(*subopcode));
            payload_resp += sizeof(dword);
            //XPT Error Code
            qword* xpt_errorcode;
            xpt_errorcode = (qword* )payload_resp;
            fprintf(fp1, "(0x%016x) - XPT Apply DPD, Error Code: %d\n", *xpt_errorcode, *xpt_errorcode);
            payload_resp += sizeof(qword);
            //Version
            byte* xpt_version;
            xpt_version = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Result Packet Version: %d\n", *xpt_version, *xpt_version);
            payload_resp += sizeof(byte);
            //Validity of AMAM Coeff
            byte* amam_validity;
            amam_validity = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Validity of AMAM Coefficient: %d\n", *amam_validity, *amam_validity);
            payload_resp += sizeof(byte);
            //Number of AMAM coeff
            byte* num_amam_coeff;
            num_amam_coeff = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Number of AMAM Coefficients: %d\n", *num_amam_coeff, *num_amam_coeff);
            payload_resp += sizeof(byte);
            //Parse each AMAM coeff
            for(ptrVar = 0 ; ptrVar < (*num_amam_coeff); ++ptrVar )
            {
               dword* amam_coeff;
               amam_coeff = (dword* )payload_resp;
               fprintf(fp1, "(0x%08x) - XPT Apply DPD, AMAM Cofficient for index %d is %d\n", *amam_coeff, ptrVar, *amam_coeff);
               payload_resp += sizeof(dword);
            }
            //Number of AMPM coeff
            byte* num_ampm_coeff;
            num_ampm_coeff = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Number of AMPM Coefficients: %d\n", *num_ampm_coeff, *num_ampm_coeff);
            payload_resp += sizeof(byte);
            //Parse each AMPM coeff
            for(ptrVar = 0 ; ptrVar < (*num_ampm_coeff); ++ptrVar )
            {
               dword* ampm_coeff;
               ampm_coeff = (dword* )payload_resp;
               fprintf(fp1, "(0x%08x) - XPT Apply DPD, AMPM Cofficient for index %d is %d\n", *ampm_coeff, ptrVar, *ampm_coeff);
               payload_resp += sizeof(dword);
            }
            //Number of Phase_Shift Coefficients
            byte* num_phase_shift_coeff;
            num_phase_shift_coeff = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Number of Phase Shift Coefficients: %d\n", *num_phase_shift_coeff, *num_phase_shift_coeff);
            payload_resp += sizeof(byte);
            //Parse each Phase Shift Coeff
            for(ptrVar = 0 ; ptrVar < (*num_phase_shift_coeff); ++ptrVar )
            {
               signed char* phase_shift_coeff;
               phase_shift_coeff = (signed char* )payload_resp;
               fprintf(fp1, "(0x%02x) - XPT Apply DPD, Phase Shift Cofficient for index %d is %d\n", *phase_shift_coeff, ptrVar, *phase_shift_coeff);
               payload_resp += sizeof(byte);
            }
         }
         //HDET + EPT Apply DPD
         else if( ((opcode_list[k] & 0x0080) && (subopcode_list[k] & 0x0004)) //EPT + Apply DPD
                  && (opcode_list[k] & 0x0010)) //HDET
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);

            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);
            //HDET
            if( (hdet[k] > 0) && !(lpmhdet[k] > 0) )
            {
               word* hdet_res;
               hdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - HDET Result : %d\n", *hdet_res, *hdet_res);
               payload_resp += sizeof(word);
            }
            else if( (lpmhdet[k] > 0) && !(hdet[k] > 0) )
            {
               word* lpmhdet_res;
               lpmhdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - LPM HDET Result : %d\n", *lpmhdet_res, *lpmhdet_res);
               payload_resp += sizeof(word);
            }
            else if( (lpmhdet[k] > 0) && (hdet[k] > 0) )
            {
               word* hdet_res;
               hdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - HDET Result : %d\n", *hdet_res, *hdet_res);
               payload_resp += sizeof(word);

               word* lpmhdet_res;
               lpmhdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - LPM HDET Result : %d\n", *lpmhdet_res, *lpmhdet_res);
               payload_resp += sizeof(word);
            }
            //EPT Apply DPD
            //Sub Opcode
            dword* subopcode;
            subopcode = (dword* )payload_resp;
            fprintf(fp1, "(0x%08x) - XPT Apply DPD, subcopde: b%032x\n", *subopcode, ConvertUint32HextoBinary(*subopcode));
            payload_resp += sizeof(dword);
            //XPT Error Code
            qword* xpt_errorcode;
            xpt_errorcode = (qword* )payload_resp;
            fprintf(fp1, "(0x%016x) - XPT Apply DPD, Error Code: %d\n", *xpt_errorcode, *xpt_errorcode);
            payload_resp += sizeof(qword);
            //Version
            byte* xpt_version;
            xpt_version = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Result Packet Version: %d\n", *xpt_version, *xpt_version);
            payload_resp += sizeof(byte);
            //Validity of AMAM Coeff
            byte* amam_validity;
            amam_validity = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Validity of AMAM Coefficient: %d\n", *amam_validity, *amam_validity);
            payload_resp += sizeof(byte);
            //Number of AMAM coeff
            byte* num_amam_coeff;
            num_amam_coeff = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Number of AMAM Coefficients: %d\n", *num_amam_coeff, *num_amam_coeff);
            payload_resp += sizeof(byte);
            //Parse each AMAM coeff
            for(ptrVar = 0 ; ptrVar < (*num_amam_coeff); ++ptrVar )
            {
               dword* amam_coeff;
               amam_coeff = (dword* )payload_resp;
               fprintf(fp1, "(0x%08x) - XPT Apply DPD, AMAM Cofficient for index %d is %d\n", *amam_coeff, ptrVar, *amam_coeff);
               payload_resp += sizeof(dword);
            }
            //Number of AMPM coeff
            byte* num_ampm_coeff;
            num_ampm_coeff = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Number of AMPM Coefficients: %d\n", *num_ampm_coeff, *num_ampm_coeff);
            payload_resp += sizeof(byte);
            //Parse each AMPM coeff
            for(ptrVar = 0 ; ptrVar < (*num_ampm_coeff); ++ptrVar )
            {
               dword* ampm_coeff;
               ampm_coeff = (dword* )payload_resp;
               fprintf(fp1, "(0x%08x) - XPT Apply DPD, AMPM Cofficient for index %d is %d\n", *ampm_coeff, ptrVar, *ampm_coeff);
               payload_resp += sizeof(dword);
            }
            //Number of Phase_Shift Coefficients
            byte* num_phase_shift_coeff;
            num_phase_shift_coeff = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Number of Phase Shift Coefficients: %d\n", *num_phase_shift_coeff, *num_phase_shift_coeff);
            payload_resp += sizeof(byte);
            //Parse each Phase Shift Coeff
            for(ptrVar = 0 ; ptrVar < (*num_phase_shift_coeff); ++ptrVar )
            {
               signed char* phase_shift_coeff;
               phase_shift_coeff = (signed char* )payload_resp;
               fprintf(fp1, "(0x%02x) - XPT Apply DPD, Phase Shift Cofficient for index %d is %d\n", *phase_shift_coeff, ptrVar, *phase_shift_coeff);
               payload_resp += sizeof(byte);
            }
         }
         //HDET + EPT(with No Op)
         else if( (opcode_list[k] & 0x0080) //EPT
                  && (opcode_list[k] & 0x0010) //HDET
                  && (subopcode_list[k] & 0x0000) )//No-Op
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);
            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);
            //HDET
            if( (hdet[k] > 0) && !(lpmhdet[k] > 0) )
            {
               word* hdet_res;
               hdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - HDET Result : %d\n", *hdet_res, *hdet_res);
               payload_resp += sizeof(word);
            }
            else if( (lpmhdet[k] > 0) && !(hdet[k] > 0) )
            {
               word* lpmhdet_res;
               lpmhdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - LPM HDET Result : %d\n", *lpmhdet_res, *lpmhdet_res);
               payload_resp += sizeof(word);
            }
            else if( (lpmhdet[k] > 0) && (hdet[k] > 0) )
            {
               word* hdet_res;
               hdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - HDET Result : %d\n", *hdet_res, *hdet_res);
               payload_resp += sizeof(word);
               word* lpmhdet_res;
               lpmhdet_res = (word*)payload_resp;
               fprintf(fp1,"(0x%04x) - LPM HDET Result : %d\n", *lpmhdet_res, *lpmhdet_res);
               payload_resp += sizeof(word);
            }
            //EPT No Op
         }
         //EPT Apply DPD + No HDET + No Rx
         else if ( (opcode_list[k] & 0x0080) && (subopcode_list[k] & 0x0004 ) //EPT Apply DPD
                   && !(opcode_list[k] & 0x0010) && !(opcode_list[k] & 0x0040)) //No HDET + No Rx
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);
            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);
            //EPT Apply DPD
            //Sub Opcode
            dword* subopcode;
            subopcode = (dword* )payload_resp;
            fprintf(fp1, "(0x%08x) - XPT Apply DPD, subcopde: b%032x\n", *subopcode, ConvertUint32HextoBinary(*subopcode));
            payload_resp += sizeof(dword);
            //XPT Error Code
            qword* xpt_errorcode;
            xpt_errorcode = (qword* )payload_resp;
            fprintf(fp1, "(0x%016x) - XPT Apply DPD, Error Code: %d\n", *xpt_errorcode, *xpt_errorcode);
            payload_resp += sizeof(qword);
            //Version
            byte* xpt_version;
            xpt_version = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Result Packet Version: %d\n", *xpt_version, *xpt_version);
            payload_resp += sizeof(byte);
            //Validity of AMAM Coeff
            byte* amam_validity;
            amam_validity = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Validity of AMAM Coefficient: %d\n", *amam_validity, *amam_validity);
            payload_resp += sizeof(byte);
            //Number of AMAM coeff
            byte* num_amam_coeff;
            num_amam_coeff = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Number of AMAM Coefficients: %d\n", *num_amam_coeff, *num_amam_coeff);
            payload_resp += sizeof(byte);
            //Parse each AMAM coeff
            for(ptrVar = 0 ; ptrVar < (*num_amam_coeff); ++ptrVar )
            {
               dword* amam_coeff;
               amam_coeff = (dword* )payload_resp;
               fprintf(fp1, "(0x%08x) - XPT Apply DPD, AMAM Cofficient for index %d is %d\n", *amam_coeff, ptrVar, *amam_coeff);
               payload_resp += sizeof(dword);
            }
            //Number of AMPM coeff
            byte* num_ampm_coeff;
            num_ampm_coeff = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Number of AMPM Coefficients: %d\n", *num_ampm_coeff, *num_ampm_coeff);
            payload_resp += sizeof(byte);
            //Parse each AMPM coeff
            for(ptrVar = 0 ; ptrVar < (*num_ampm_coeff); ++ptrVar )
            {
               dword* ampm_coeff;
               ampm_coeff = (dword* )payload_resp;
               fprintf(fp1, "(0x%08x) - XPT Apply DPD, AMPM Cofficient for index %d is %d\n", *ampm_coeff, ptrVar, *ampm_coeff);
               payload_resp += sizeof(dword);
            }
            //Number of Phase_Shift Coefficients
            byte* num_phase_shift_coeff;
            num_phase_shift_coeff = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Number of Phase Shift Coefficients: %d\n", *num_phase_shift_coeff, *num_phase_shift_coeff);
            payload_resp += sizeof(byte);
            //Parse each Phase Shift Coeff
            for(ptrVar = 0 ; ptrVar < (*num_phase_shift_coeff); ++ptrVar )
            {
               signed char* phase_shift_coeff;
               phase_shift_coeff = (signed char* )payload_resp;
               fprintf(fp1, "(0x%02x) - XPT Apply DPD, Phase Shift Cofficient for index %d is %d\n", *phase_shift_coeff, ptrVar, *phase_shift_coeff);
               payload_resp += sizeof(byte);
            }
         }
         // EPT APPLY DPD + NO HDET + RX
         else if ( (opcode_list[k] & 0x0080) && (subopcode_list[k] & 0x0004 ) //EPT Apply DPD
                   && !(opcode_list[k] & 0x0010) && (opcode_list[k] & 0x0040)) //No HDET + Rx
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);
            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);
            //Rx Meas
            chain = 0;
            chain_mc = 0;
            for( chain_mask = 0x01, chain = 0; chain < 8; chain_mask <<= 1, chain++ )
            {
               if( chain_mask & rxflag[k] )
               {
                  if( rxpm[k] == 0 )
                  {
                     // The first 8 bits indicate the 8 possible gain ranges supported
                     for( gr_mask = 0x0001, gain_range = 0; gain_range < 8; gr_mask <<= 1, gain_range++ )
                     {
                        if( gr_mask & rxactions[k] )
                        {
                           if( chanOffset[k] == 0 )
                           {
                              // PM0 Non DC Result
                              signed short* rx_res;
                              rx_res = (signed short*)payload_resp;
                              fprintf(fp1,"(0x%04x) - Rx Result Chain %d (PM0,SC) : %d\n", *rx_res, chain, *rx_res);
                              payload_resp += sizeof(word);
                           }
                           else
                           {
                              // PM0 DC Result
                              signed short* rx_res;
                              rx_res = (signed short*)payload_resp;
                              fprintf(fp1,"(0x%04x) - Rx Result Chain %d (PM0,DC) : %d\n", *rx_res, chain, *rx_res);
                              payload_resp += sizeof(word);
                           }
                        }
                     }
                  }
                  else
                  {
                     // The first 8 bits indicate the 8 possible gain ranges supported
                     for( gr_mask = 0x0001, gain_range = 0; gain_range < 8; gr_mask <<= 1, gain_range++ )
                     {
                        if( gr_mask & rxactions[k] )
                        {
                           signed short* rx_res_mc;
                           rx_res_mc = (signed short*)payload_resp;
                           fprintf(fp1,"(0x%04x) - Rx Result Chain %d (PM1,DC) : %d\n", *rx_res_mc, chain, *rx_res_mc);
                           payload_resp += sizeof(word);
                        }
                     }
                  }
               }
            }
            //EPT Apply DPD
            //Sub Opcode
            dword* subopcode;
            subopcode = (dword* )payload_resp;
            fprintf(fp1, "(0x%08x) - XPT Apply DPD, subcopde: b%032x\n", *subopcode, ConvertUint32HextoBinary(*subopcode));
            payload_resp += sizeof(dword);
            //XPT Error Code
            qword* xpt_errorcode;
            xpt_errorcode = (qword* )payload_resp;
            fprintf(fp1, "(0x%016x) - XPT Apply DPD, Error Code: %d\n", *xpt_errorcode, *xpt_errorcode);
            payload_resp += sizeof(qword);
            //Version
            byte* xpt_version;
            xpt_version = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Result Packet Version: %d\n", *xpt_version, *xpt_version);
            payload_resp += sizeof(byte);
            //Validity of AMAM Coeff
            byte* amam_validity;
            amam_validity = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Validity of AMAM Coefficient: %d\n", *amam_validity, *amam_validity);
            payload_resp += sizeof(byte);
            //Number of AMAM coeff
            byte* num_amam_coeff;
            num_amam_coeff = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Number of AMAM Coefficients: %d\n", *num_amam_coeff, *num_amam_coeff);
            payload_resp += sizeof(byte);
            //Parse each AMAM coeff
            for(ptrVar = 0 ; ptrVar < (*num_amam_coeff); ++ptrVar )
            {
               dword* amam_coeff;
               amam_coeff = (dword* )payload_resp;
               fprintf(fp1, "(0x%08x) - XPT Apply DPD, AMAM Cofficient for index %d is %d\n", *amam_coeff, ptrVar, *amam_coeff);
               payload_resp += sizeof(dword);
            }
            //Number of AMPM coeff
            byte* num_ampm_coeff;
            num_ampm_coeff = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Number of AMPM Coefficients: %d\n", *num_ampm_coeff, *num_ampm_coeff);
            payload_resp += sizeof(byte);
            //Parse each AMPM coeff
            for(ptrVar = 0 ; ptrVar < (*num_ampm_coeff); ++ptrVar )
            {
               dword* ampm_coeff;
               ampm_coeff = (dword* )payload_resp;
               fprintf(fp1, "(0x%08x) - XPT Apply DPD, AMPM Cofficient for index %d is %d\n", *ampm_coeff, ptrVar, *ampm_coeff);
               payload_resp += sizeof(dword);
            }
            //Number of Phase_Shift Coefficients
            byte* num_phase_shift_coeff;
            num_phase_shift_coeff = (byte* )payload_resp;
            fprintf(fp1, "(0x%02x) - XPT Apply DPD, Number of Phase Shift Coefficients: %d\n", *num_phase_shift_coeff, *num_phase_shift_coeff);
            payload_resp += sizeof(byte);
            //Parse each Phase Shift Coeff
            for(ptrVar = 0 ; ptrVar < (*num_phase_shift_coeff); ++ptrVar )
            {
               signed char* phase_shift_coeff;
               phase_shift_coeff = (signed char* )payload_resp;
               fprintf(fp1, "(0x%02x) - XPT Apply DPD, Phase Shift Cofficient for index %d is %d\n", *phase_shift_coeff, ptrVar, *phase_shift_coeff);
               payload_resp += sizeof(byte);
            }
         }
         //EPT FBRx + TxPathGain
         else if ( (opcode_list[k] & 0x0080) && (subopcode_list[k] & 0x0008 ) //EPT + FBRx
                   && (subopcode_list[k] & 0x00010) ) // Tx Path gain
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);
            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);

            //XPT Results
            //XPT SubOpcde
            dword* subopcode;
            subopcode = (dword* )payload_resp;
            fprintf(fp1, "(0x%08x) - XPT FB Rx and Tx Path Gain, subcopde: b%032x\n", *subopcode, ConvertUint32HextoBinary(*subopcode));
            payload_resp += sizeof(dword);
            //XPT Error Code
            qword* xpt_errorcode;
            xpt_errorcode = (qword* )payload_resp;
            fprintf(fp1, "(0x%016x) - XPT FB Rx and Tx Path Gain, Error Code: %d\n", *xpt_errorcode, *xpt_errorcode);
            payload_resp += sizeof(qword);
            //FB Rx
            /*Currently Result version is not sent for any subopcodes(except XPT APPLY DPD). Uncomment this once embedded side sends version
            //FB Rx:Version of the Results Packet
            byte* version_res_pkt;
            version_res_pkt = (byte* )payload_resp;
            fprintf(fp1,"(0x%02x) - XPT FB Rx, Result Packet Version is : %d\n", *version_res_pkt, *version_res_pkt);
            payload_resp += sizeof(byte);

            //TxPath Gain: Version of the Results Packet
            version_res_pkt = (byte* )payload_resp;
            fprintf(fp1,"(0x%02x) - XPT TxPath Gain, Result Packet Version is : %d\n", *version_res_pkt, *version_res_pkt);
            payload_resp += sizeof(byte);
            */
         }
         //EPT TxPathGain
         else if ( (opcode_list[k] & 0x0080) //EPT
                   && (subopcode_list[k] & 0x00010) ) // Tx Path gain
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);
            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);
            //XPT SubOpcde
            dword* subopcode;
            subopcode = (dword* )payload_resp;
            fprintf(fp1, "(0x%08x) - XPT TxPath Gain, subcopde: b%032x\n", *subopcode, ConvertUint32HextoBinary(*subopcode));
            payload_resp += sizeof(dword);
            //XPT Error Code
            qword* xpt_errorcode;
            xpt_errorcode = (qword* )payload_resp;
            fprintf(fp1, "(0x%016x) - XPT TxPath Gain, Error Code: %d\n", *xpt_errorcode, *xpt_errorcode);
            payload_resp += sizeof(qword);
            /*Currently Result version is not sent for any subopcodes(except XPT APPLY DPD). Uncomment this once embedded side sends version
            //TxPath Gain: Version of the Results Packet
            byte* version_res_pkt;
            version_res_pkt = (byte* )payload_resp;
            fprintf(fp1,"(0x%02x) - XPT TxPath Gain, Result Packet Version is : %d\n", *version_res_pkt, *version_res_pkt);
            payload_resp += sizeof(byte);
            */
         }
         //EPT IQCatpture
         else if( (opcode_list[k] & 0x0080) //EPT
                  && (subopcode_list[k] & 0x0002) && !(subopcode_list[k] & 0x0001)) // IQCapture and not XPT Config
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);
            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);
            //XPT SubOpcde
            dword* subopcode;
            subopcode = (dword* )payload_resp;
            fprintf(fp1, "(0x%08x) - XPT IQ Capture, subcopde: b%032x\n", *subopcode, ConvertUint32HextoBinary(*subopcode));
            payload_resp += sizeof(dword);
            //XPT Error Code
            qword* xpt_errorcode;
            xpt_errorcode = (qword* )payload_resp;
            fprintf(fp1, "(0x%016x) - XPT IQ Capture, Error Code: %d\n", *xpt_errorcode, *xpt_errorcode);
            payload_resp += sizeof(qword);
            /*Currently Result version is not sent for any subopcodes(except XPT APPLY DPD). Uncomment this once embedded side sends version
            //IQCapture: Version of the Results Packet
            byte* version_res_pkt;
            version_res_pkt = (byte* )payload_resp;
            fprintf(fp1,"(0x%02x) - XPT IQ Capture, Result Packet Version is : %d\n", *version_res_pkt, *version_res_pkt);
            payload_resp += sizeof(byte);
            */
         }
         else if( (opcode_list[k] & 0x0080) //XPT
                  && (subopcode_list[k] & 0x0001) //XPT_Config
                  && (subopcode_list[k] & 0x0002)) // IQCapture
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);
            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);
            //XPT SubOpcde
            dword* subopcode;
            subopcode = (dword* )payload_resp;
            fprintf(fp1, "(0x%08x) - XPT Config and IQ Capture, subcopde: b%032x\n", *subopcode, ConvertUint32HextoBinary(*subopcode));
            payload_resp += sizeof(dword);
            //XPT Error Code
            qword* xpt_errorcode;
            xpt_errorcode = (qword* )payload_resp;
            fprintf(fp1, "(0x%016x) - XPT Config and IQ Capture, Error Code: %d\n", *xpt_errorcode, *xpt_errorcode);
            payload_resp += sizeof(qword);
            /*Currently Result version is not sent for any subopcodes(except XPT APPLY DPD). Uncomment this once embedded side sends version
            //XPT Config
            byte* version_res_pkt;
            version_res_pkt = (byte* )payload_resp;
            fprintf(fp1,"(0x%02x) - XPT Config, Result Packet Version is : %d\n", *version_res_pkt, *version_res_pkt);
            payload_resp += sizeof(byte);
            //XPT IQCapture(Version of the Results Packet(1 byte for each segment))
            version_res_pkt = (byte* )payload_resp;
            fprintf(fp1,"(0x%02x) - XPT IQ Capture, Result Packet Version is : %d\n", *version_res_pkt, *version_res_pkt);
            payload_resp += sizeof(byte);
            */
         }
         //EPT + NoOp: Populate Unity DPD values
         else if ( (opcode_list[k] & 0x0080)
                   && (subopcode_list[k] & 0x0000) )
         {
            word* seg_num;
            seg_num = (word*)payload_resp;
            fprintf(fp1,"(0x%04x) - Result Seg Number : %d\n", *seg_num, *seg_num);
            payload_resp += sizeof(word);
            byte* seg_len;
            seg_len = (byte*)payload_resp;
            fprintf(fp1,"(0x%02x) - Result Seg Length : %d\n", *seg_len, *seg_len);
            payload_resp += sizeof(byte);
         }
         else
         {
            fprintf(fp1,"No result expected for Segment Number : %d as Opcode is (b%04x)\n", k, ConvertUint16HextoBinary(opcode_list[k]));
         }
      }
   }
   fprintf(fp1,"\n");
   fprintf(fp1,"\n");
   fprintf(fp1,"----------------------------------------\n");
   fprintf(fp1,"*********** PARSING COMPLETE ***********\n");
   fprintf(fp1,"----------------------------------------\n");
   fprintf(fp1,"\n");

   fclose(fp);
   fclose(fp1);

   if( parse_resp )
      fclose(fp2);

   return true;
}

void Test_ESC_Parser()
{
   unsigned char bContinue = true;      // Variable for menu
   char cKey = ' ';                 // Key pressed in menu

   do
   {
      printf("\n\n\n======================================\n");
      printf("Select one option below: \n");
      printf("1 : Parse Request File only\n");
      printf("2 : Parse Request and Result File\n");
      printf("Q : Quit\n");

      // Get a key
      cKey = toupper( _getch() );

      printf("\n\n\n\n");

      if(cKey == '1')
      {
         bContinue = ESC_Parser(false);
      }
      else if(cKey == '2')
      {
         bContinue = ESC_Parser(true);
      }
      else if(cKey == 'Q')
      {
         bContinue = false;
      }
   }
   while (bContinue);
}
// Helper Functions

unsigned long ConvertUint8HextoBinary( byte hexVal )
{
   unsigned long retVal = 0x0;
   int i;
   for(i=0; i<8; i++)
      if(hexVal & (0x1 <<i)) retVal |= (0x1 << 4*i);

   return retVal;
}

unsigned long ConvertUint16HextoBinary( word hexVal )
{
   unsigned long retVal = 0x0;
   int i;
   for(i=0; i<16; i++)
      if(hexVal & (0x1 <<i)) retVal |= (0x1 << 4*i);

   return retVal;
}

unsigned long ConvertUint32HextoBinary( dword hexVal )
{
   unsigned long retVal = 0x0;
   int i;
   for(i=0; i<32; i++)
      if(hexVal & (0x1 <<i)) retVal |= (0x1 << 4*i);

   return retVal;
}
unsigned char Test_RFCal_TxRsb_TxLo_Internal_Cal()
{
   unsigned char _bOK      = false;
   unsigned char TECH      = 1; // WCDMA

   printf("Test_RFCal_TxRsb_TxLo_Internal_Cal\n\n");

   //********************************************************//
   //             Set MODE
   //********************************************************//

   _bOK = ::QLIB_FTM_SET_MODE_ID(g_hResourceContext,FTM_MODE_ID_WCDMA);
   _bOK = ::QLIB_FTM_SET_MODE(g_hResourceContext,PHONE_MODE_WCDMA_IMT);
   printf("//********************************************************//\n");
   printf("//            Set MODE and Set Cal State On               //\n");
   printf("//********************************************************//\n\n");

   printf("QLIB_FTM_SET_MODE_ID(g_hResourceContext,FTM_MODE_ID_WCDMA);\n");
   printf("QLIB_FTM_SET_MODE(g_hResourceContext,PHONE_MODE_WCDMA_IMT);\n");

   //********************************************************//
   //             Set Calibration state ON
   //********************************************************//

   _bOK = ::QLIB_FTM_SET_CALIBRATION_STATE(g_hResourceContext,true);
   printf("QLIB_FTM_SET_CALIBRATION_STATE(g_hResourceContext,true);\n\n");

   //********************************************************//
   //             Parameter configuration
   // Band information is a sample set. Please assign appropriate band information according to your RF card
   //********************************************************//

   unsigned char RF_MODE_WCDMA[] = {9, 16, 25, 28, 22, 29}; // WCDMA B1/2/3/5/8
   unsigned char RF_MODE_LTE[]      = {34, 44, 45, 35, 47, 50, 36, 51, 57, 56, 61, 62, 38, 76}; // LTE B1/2/3/4/5/7/8/12/13/14/17/20/25/26/38/41
   unsigned char RF_MODE_TDSCDMA[]  = {92}; // TDSCDMA B40

   unsigned char* RF_MODE;
   unsigned short iNumBands;

   if (TECH == 1)
   {
      iNumBands   = sizeof(RF_MODE_WCDMA)/sizeof(unsigned char);
      RF_MODE     = RF_MODE_WCDMA;
   }
   else if (TECH == 3)
   {
      iNumBands   = sizeof(RF_MODE_LTE)/sizeof(unsigned char);
      RF_MODE     = RF_MODE_LTE;
   }
   else if (TECH == 4)
   {
      iNumBands   = sizeof(RF_MODE_TDSCDMA)/sizeof(unsigned char);
      RF_MODE     = RF_MODE_TDSCDMA;
   }
   else
   {
      cout  << "TECH " << (int)TECH << " is not supported in this demo." << endl;
      return false;
   }

   //********************************************************//
   //    Assign parameters and Execute QLIB_FTM_DO_RSB_TXLO_CAL
   //********************************************************//

   printf("//********************************************************//\n");
   printf("// Assign parameters and Execute QLIB_FTM_DO_RSB_TXLO_CAL //\n");
   printf("//********************************************************//\n\n");

   for (int i=0; i < iNumBands; i++)
   {
      unsigned char  responseCode = 0;

      QMSL_FTM_RSB_TXLO_PKT_TYPE rsbLoPayload;

      rsbLoPayload.iRfMode       = (unsigned char)RF_MODE[i];
      rsbLoPayload.iOverrideStg  = (unsigned char)0;
      rsbLoPayload.iPrxDevice    = (unsigned char)0;
      rsbLoPayload.iDrxDevice    = (unsigned char)1;
      rsbLoPayload.iExitRequired = (unsigned char)1;

#ifdef ON_TARGET
      _bOK = QLIB_FTM_DO_RSB_TXLO_CAL( g_hResourceContext, (void*)&rsbLoPayload, &responseCode );
#endif

      if (_bOK)
      {
         cout << "QLIB_FTM_DO_RSB_TXLO_CAL:\tRF_MODE:" << (int)RF_MODE[i] << "\tResponse Code:" << (long)responseCode << endl;
      }
      else
      {
         cout << "QLIB_FTM_DO_RSB_TXLO_CAL failure \tRF_MODE:" << (int)RF_MODE[i] << endl;
      }
   }
   printf("\n End of Test_RFCal_TxRsb_TxLo_Internal_Cal()\n");
   return true;
}


unsigned char Test_RFCal_V2()
{
   unsigned char bContinue = true;     // Variable for menu
   do
   {
      // Print options
      printf("\n\n\nRFCal V2 Menu ======================================\n");
      printf("\n 1 = LTE B29 (SCELL) cal");
      printf("\n Q = Return to the main menu");

      // Get a key
      char cKey = toupper( _getch() );

      printf("\n\n\n\n\n\n\n\n");
      switch (cKey)
      {
         case '1':
            // LTE B1 ET
            testLTECA_B29SCELLCal() ;
            break;

         case 'Q':
            bContinue = false;
            break;
      } // switch( cKey)
   }
   while (bContinue);

   return true;
}
unsigned char testLTECA_B29SCELLCal()
{
   printf("\n\n");
   printf("***************************************************\n");
   printf("LTE CA B29 SCEL Cal Demo\n");
   printf("***************************************************\n");

   //*********************************//
   //    Parameters
   //*********************************//

   printf("\n\n");
   printf("- Parameter assignment\n");

   unsigned char _bOK         = false;
   unsigned short iChannelB2  = 19000;
   const int iNumLNA          = 6;
   int iChannelList[]         = {9660,9667,9674,9681,9688,9695,9702,9709,9716,9723,9730,9737,9744,9751,9758,9765};
   const int iNumCh           = sizeof(iChannelList)/sizeof(int);

   short piLNAOffsetValueC3[iNumCh][iNumLNA] = {0};
   short piLNAOffsetValueC4[iNumCh][iNumLNA] = {0};

   short iRxPwrList[iNumLNA]  = {-61,-60,-50,-40,-33,-33};
   short iExpectedRX_AGC[iNumLNA];
   for (int i = 0; i < iNumLNA; i++)
   {
      iExpectedRX_AGC[i] = iRxPwrList[i] * 10;
   }

   short iRxFCompLNAOffset_Min = -80;
   short iRxFCompLNAOffset_Max = 80;

   //*********************************//
   //    LTE_B2_PCell_TUNE
   //*********************************//
   printf("- LTE_B2_PCell_TUNE\n");

   _bOK = QLIB_FTM_SET_CALIBRATION_STATE(g_hResourceContext,true);

   _bOK = QLIB_FTM_SET_MODE_ID(g_hResourceContext, FTM_MODE_ID_LTE);

   _bOK = QLIB_FTM_SET_MODE(g_hResourceContext, PHONE_MODE_LTE_B2);

   _bOK = QLIB_FTM_LTE_SET_TX_BANDWIDTH(g_hResourceContext, 3); // 3 is RFCOM_BW_LTE_10MHz

   _bOK = QLIB_FTM_LTE_SET_RX_BANDWIDTH(g_hResourceContext, 3); // 3 is RFCOM_BW_LTE_10MHz

   _bOK = QLIB_FTM_SET_CHAN(g_hResourceContext, iChannelB2);

   //*********************************//
   //    LTE_B29_Rx_SCell
   //*********************************//
   printf("- LTE_B29_Rx_SCell (Cal)\n");

   _bOK = QLIB_FTM_SET_CALIBRATION_STATE(g_hResourceContext,true);

   _bOK = QLIB_FTM_SET_MODE_ID(g_hResourceContext, FTM_MODE_ID_LTE_C3);

   _bOK = QLIB_FTM_SET_MODE(g_hResourceContext, PHONE_MODE_LTE_B29);

   _bOK = QLIB_FTM_LTE_SET_TX_BANDWIDTH(g_hResourceContext, 3); // 3 is RFCOM_BW_LTE_10MHz

   _bOK = QLIB_FTM_LTE_SET_RX_BANDWIDTH(g_hResourceContext, 3); // 3 is RFCOM_BW_LTE_10MHz

   for (int iChIdx = 0; iChIdx < iNumCh; iChIdx++)
   {
      _bOK = QLIB_FTM_SET_MODE_ID(g_hResourceContext, FTM_MODE_ID_LTE_C3);

      _bOK = QLIB_FTM_SET_CHAN(g_hResourceContext, iChannelList[iChIdx]);

      for (unsigned short iLNAIdx = 0; iLNAIdx < iNumLNA; iLNAIdx++)
      {
         // Insert Callbox DL Pwr code here

         _bOK = QLIB_FTM_SET_MODE_ID(g_hResourceContext, FTM_MODE_ID_LTE_C3);

         _bOK = QLIB_FTM_GET_LNA_OFFSET(g_hResourceContext, iLNAIdx,iExpectedRX_AGC[iLNAIdx], &piLNAOffsetValueC3[iChIdx][iLNAIdx]);

         // Insert Callbox DL Pwr code here

         _bOK = QLIB_FTM_SET_MODE_ID(g_hResourceContext, FTM_MODE_ID_LTE_C4);

         _bOK = QLIB_FTM_GET_LNA_OFFSET(g_hResourceContext, iLNAIdx,iExpectedRX_AGC[iLNAIdx], &piLNAOffsetValueC4[iChIdx][iLNAIdx]);
      }
   }

   //*********************************//
   //    Dummy result LTE_B29_Rx_SCell
   //   (only for demo purpose)
   //*********************************//

   short dummyResultC3[][iNumLNA] =
   {
      {-9,  -8, -15, -22, -15,-9},
      {-10, -10, -17, -24, -16,-10},
      {-9, -10, -17, -23, -17,-12},
      {-8, -9, -15, -24, -19, -11},
      {-6, -6, -14, -23, -18, -10},
      {-5, -5, -11, -23, -15, -9},
      {-4, -4, -12, -23, -16, -8},
      {-4, -4, -11, -22, -12, -7},
      {-4, -5, -13, -22, -11, -7},
      {-5, -7, -14, -22, -11, -6},
      {-8, -8, -16, -22, -12, -8},
      {-10, -10, -19, -24, -12, -9},
      {-11, -11, -20, -25, -16, -11},
      {-12, -13, -20, -25, -17, -13},
      {-11, -12, -19, -26, -18, -14},
      {-11, -11, -19, -25, -18, -13},
   };

   short dummyResultC4[][iNumLNA] =
   {
      {-8, -9, -17, -9, -7, 0},
      {-10, -9, -18, -11, -6, -2},
      {-11, -12, -18, -10, -8, -3},
      {-12, -13, -18, -11, -8, -4},
      {-11, -12, -18, -12, -9, -5},
      {-10, -11, -19, -12, -10, -4},
      {-11, -10, -16, -12, -8, -4},
      {-8, -8, -17, -12, -9, -3},
      {-7, -10, -16, -10, -7, -1},
      {-7, -8, -17, -10, -6, -1},
      {-8, -9, -18, -9, -7, -2},
      {-9, -10, -18, -10, -7, -3},
      {-10, -11, -19, -11, -8, -3},
      {-10, -11, -17, -12, -8, -3},
      {-10, -11, -18, -12, -9, -3},
      {-7, -8, -16, -11, -9, -3}
   };

   //for (int iChIdx = 0; iChIdx < iNumCh; iChIdx++)
   //{
   // for (int iLNAIdx = 0; iLNAIdx < iNumLNA; iLNAIdx++)
   // {
   //    piLNAOffsetValueC3[iChIdx][iLNAIdx] = dummyResultC3[iChIdx][iLNAIdx];
   //    piLNAOffsetValueC4[iChIdx][iLNAIdx] = dummyResultC4[iChIdx][iLNAIdx];
   // }
   //}
   memcpy(piLNAOffsetValueC3,dummyResultC3,sizeof(short)*iNumCh*iNumLNA);
   memcpy(piLNAOffsetValueC4,dummyResultC4,sizeof(short)*iNumCh*iNumLNA);

   printf("- Offset Limit check\n");

   //*********************************//
   //   Offset Limit check
   //*********************************//
   _bOK = true;
   for (int iChIdx = 0; iChIdx < iNumCh; iChIdx++)
   {
      for (int iLNAIdx = 0; iLNAIdx < iNumLNA; iLNAIdx++)
      {
         if (( piLNAOffsetValueC3[iChIdx][iLNAIdx] < iRxFCompLNAOffset_Min) || ( piLNAOffsetValueC3[iChIdx][iLNAIdx] > iRxFCompLNAOffset_Max))
         {
            _bOK = false;
            printf("Offset Limit failure (C3) at Channel %d LNA %d \n",iChannelList[iChIdx], iLNAIdx);
         }
         if (( piLNAOffsetValueC4[iChIdx][iLNAIdx] < iRxFCompLNAOffset_Min) || ( piLNAOffsetValueC4[iChIdx][iLNAIdx] > iRxFCompLNAOffset_Max))
         {
            _bOK = false;
            printf("Offset Limit failure (C4) at Channel %d LNA %d \n",iChannelList[iChIdx], iLNAIdx);
         }
      }
   }

   if (!_bOK)
   {
      printf ("failed\n");
   }

   printf("- NV Generation for LTE_B29_Rx_SCell\n");

   //*********************************//
   //   NV Generation for LTE_B29_Rx_SCell
   //*********************************//

   unsigned short iNV_Mode_ID = NV_MODE_LTE_B29;

   // Store generated NV items in NV Manager. NV items stored in NV Manager will be
   // written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone is called.
   unsigned char  bCachedByNVManager = 1;

   // Write NV cal channels
   unsigned char bWriteNVChList = 1;

   // QMSL structures used to store DVGA offset/LNA offset vs. frequency calibration data
   QMSL_CDMA_RxFreq_Measurement_struct aRX_Measured;
   QMSL_CDMA_MeasuredChannels_struct RX_MeasuredChannels;

   QMSL_CDMA_NVChannels_struct RX_NV_Channels;

   // CA Enabled
   aRX_Measured.iCAenabled       = 1;

   // No IntelliCeiver, so only one power mode
   aRX_Measured.iNumMeasuredPowerModes = 1;

   // Copy measured data to the QMSL struture
   for(int ch = 0; ch < iNumCh; ch++)
   {
      for (int iLNAIdx = 0; iLNAIdx < iNumLNA; iLNAIdx++)
         aRX_Measured.aiLNA_Measured[ch][iLNAIdx][0] = piLNAOffsetValueC3[ch][iLNAIdx];
   }

   aRX_Measured.iNumMeasured_LNA_States = iNumLNA;

   // aRX_Measured.aiLNA_States[n] = y
   // where n is the index to use in aRX_Measured.aiLNA_Measured[ch][n][0]
   // and y is the corresponding LNA gain state index
   for (int iLNAIdx = 0; iLNAIdx < iNumLNA; iLNAIdx++)
      aRX_Measured.aiLNA_States[iLNAIdx] =  iLNAIdx;

   // Primary Rx
   aRX_Measured.iPath = 0;

   // Fill out the channels at which the measurement were made
   for(int ch = 0; ch < 16; ch++)
   {
      RX_MeasuredChannels.aiMeasuredChannels[ch] = iChannelList[ch];
      RX_NV_Channels.aiNVChannels[ch] = iChannelList[ch];
   }

   RX_MeasuredChannels.iNumMeasuredChannels = iNumCh;
   RX_MeasuredChannels.iRefChannel = 9709;

   unsigned char iIsIntelliCeiver = 0;
   double dDVGAOffsetScalingFactor = 1;

   // This structure holds the result of the NV calculations. It is made available for troubleshooting purposes.
   QMSL_CDMA_Rx_DVGA_LNA_Cal_NV_Result RX_NV_Result;

   aRX_Measured.iPath            = 2;

   _bOK = QLIB_RFCAL_CDMA_RxCal_Results(g_hResourceContext,
                                        iNV_Mode_ID,
                                        iIsIntelliCeiver,
                                        dDVGAOffsetScalingFactor,
                                        (void*)&aRX_Measured,
                                        (void*)&RX_MeasuredChannels,
                                        (void*)&RX_NV_Channels,
                                        (void*)&RX_NV_Result,
                                        bWriteNVChList,
                                        bCachedByNVManager);

   aRX_Measured.iPath            = 3;

   for(int ch = 0; ch < iNumCh; ch++)
   {
      for (int iLNAIdx = 0; iLNAIdx < iNumLNA; iLNAIdx++)
         aRX_Measured.aiLNA_Measured[ch][iLNAIdx][0] = piLNAOffsetValueC4[ch][iLNAIdx];
   }

   _bOK = QLIB_RFCAL_CDMA_RxCal_Results(g_hResourceContext,
                                        iNV_Mode_ID,
                                        iIsIntelliCeiver,
                                        dDVGAOffsetScalingFactor,
                                        (void*)&aRX_Measured,
                                        (void*)&RX_MeasuredChannels,
                                        (void*)&RX_NV_Channels,
                                        (void*)&RX_NV_Result,
                                        bWriteNVChList,
                                        bCachedByNVManager);
   printf("\nEnd of Calibration\n");

   return true;
}

unsigned char Test_FTM_DO_SELF_CAL()
{
   unsigned char _bOK      = false;

   printf("\n--------------------\n\n");
   printf("Test_FTM_DO_SELF_CAL\n");
   printf("--------------------\n\n");

   //********************************************************//
   //             Set Calibration state ON
   //********************************************************//

   _bOK = ::QLIB_FTM_SET_CALIBRATION_STATE(g_hResourceContext,true);
   printf("QLIB_FTM_SET_CALIBRATION_STATE(g_hResourceContext,true);\n\n");

   //********************************************************//
   //            FTM DO SELF_CAL
   //********************************************************//

   _bOK = ::QLIB_FTM_DO_SELF_CAL(g_hResourceContext,0);
   printf("QLIB_FTM_DO_SELF_CAL(g_hResourceContext, iCal_Type = 0 );\n\n");

   //********************************************************//
   //            QLIB_EFS2_SyncWithWait
   //********************************************************//
   unsigned char piStatus = 0;
   _bOK = ::QLIB_EFS2_SyncWithWait(g_hResourceContext,(unsigned char *)"/",10000,&piStatus);
   printf("QLIB_EFS2_SyncWithWait(g_hResourceContext, (unsigned char *)\"/\",10000,&piStatus;\n\n");
 
   return _bOK;
}


unsigned char Test_FTM_WCDMA_ICI_CAL( void )
{
   QMSL_Rx_ICI_Cal_Meas_struct measurements;
   QMSL_Rx_ICI_Cal_NV_struct nv;
   const int numberOfCoefficients = 16;
   unsigned short iFtm_error_code =  0;
   measurements.snr = -999.0;
   double snrThreshold = 25.0; //snr threshold in dB

   unsigned char bOk = true;
   unsigned char  cKey = ' ';

   //nienchun: Added
   int iLNARange = 1;
   int iDLPower = -50;
   int iChannel = 0;
   unsigned char iMultiCarrierEnabled = 0;
   unsigned char iCarrierID = 0;
   unsigned short iChain =0;
   short iMode = PHONE_MODE_WCDMA_IMT;  // see which enum in Qlib_Defines.h

   printf("\n----------------\nWCDMA FTM ICI CAL Test\n----------------\n");

   while( iDLPower > -25 && iDLPower < -106 );
   {
      printf( "\nEnter Downlink Power in dBm(recommended value is -50)\n" );
      scanf( "%d", &iDLPower );
   }
   while( iLNARange > 4 && iLNARange < 0 );
   {
      printf( "\nEnter LNA Range (recommended value is 1):" );
      scanf( "%d", &iLNARange );
   }

   printf( "\nEnter Band:" );
   scanf( "%d", &iMode );

   printf( "\nEnter Channel:" );
   scanf( "%d", &iChannel );

   printf( "\nMulticarrier Enabled?(0=No, 1=Yes)" );
   scanf( "%d", &iMultiCarrierEnabled );

   if( iMultiCarrierEnabled )
   {
      printf( "\nCarrier ID (0=Carrier0, 1=Carrier1 )" );
      scanf( "%d", &iCarrierID );
   }

   printf( "\nEnter Chain (0=PRx, 1=DRx):" );
   scanf( "%d", &iChain );

   printf( "\nEnter SNR Threshold in dB:" );
   scanf( "%f", &snrThreshold );

   //nienchun: Show the configuration
   printf( "\nDownlink power is %d", iDLPower );
   printf( "\nLNA Range is %d", iLNARange );
   printf( "\nBand is %d", iMode);
   printf( "\nChannel is %d", iChannel);
   printf( "\nChain is %d", iChain);

   printf( "\n\nPlease configure call processor as follows:" );
   printf( "\n4.Primary Scrambling Code 4" );
   printf( "\n5.CPICH Power 0 dB" ); //We need a CPICH only signal
   printf( "\n6.All other WCDMA Physical Channels OFF" );
   printf( "\n7.Ouput port based on receive chain selection." ); //Primary or secondary
   printf( "\n\nPress a key to continue." );

   cKey = getch();

   QLIB_FTM_SET_MODE_ID (g_hResourceContext, FTM_MODE_ID_WCDMA);

   //Set WCDMA Mode on UUT
   if ( !QLIB_FTM_SET_MODE( g_hResourceContext, iMode ))
   {
      bOk = false;
      printf("\nQLIB_FTM_SET_MODE failed\n");
   }

   // Set Receive Chain (primary or secondary)
   bOk &= QLIB_FTM_SET_SECONDARY_CHAIN(g_hResourceContext, iChain);

   if (iChain == 1)
   {
      QLIB_FTM_SET_MODE_ID (g_hResourceContext, FTM_MODE_WCDMA_RX1);
   }

   // Set the uplink channel
   if ( !QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel))
   {
      bOk = false;
      printf("\nQLIB_FTM_SET_CHAN failed\n");
   }

   // Set LNA Range
   bOk &= QLIB_FTM_SET_LNA_RANGE( g_hResourceContext, (unsigned char)iLNARange );

   printf( "\nWCDMA Channel has been set to %d", iChannel );
   printf( "\nReceive Chain has been set to %d", iChain );

   // Must set the filter length to 16
   measurements.filterLength = 16;

   //Obtain IQ data from UUT
   bOk &= QLIB_FTM_GET_ICI_CAL_DATA( g_hResourceContext, iMultiCarrierEnabled, iCarrierID, &iFtm_error_code, measurements.ino_data );

   // Get ICI filter coefficients
   bOk &= QLIB_RFCAL_Rx_ICI_Calibration_Algorithm( g_hResourceContext,  &measurements );

   //Print to screen
   printf( "\n\nComputed filter coeffcients are as follows:" );
   printf( "\nReal\tImaginary" );
   int real;
   int imaginary;
   for( int index = 0; index < 32; index += 2 )
   {
      real = measurements.coefficients[index];
      imaginary = measurements.coefficients[index+1];
      printf( "\n%d\t%d", real, imaginary );
   }

   //Compute NV
   measurements.rx_path = iChain;
   bOk &= QLIB_RFCAL_Rx_ICI_Calibration_Results( g_hResourceContext, iMode, (void*)& measurements, (void*)& nv, 0 );

   if( bOk )
   {
      if( measurements.snr < snrThreshold )
      {
         printf( "\nSNR is %f dB which is below the desired threshold of %f dB. Please use default filter coefficients.", measurements.snr, snrThreshold );
         bOk = 0;
      }
      else
      {
         printf( "\nSNR after ICI calibration is: %f dB.", measurements.snr );
         printf( "\n\nICI Calibration successful. Coefficients are listed below:\n" );
         int index;
         int nv_value;
         for( index = 0; index < numberOfCoefficients; index++ )
         {
            CopyMemory( (void*)& nv_value, (void*)& nv.oNV_ICI_CAL_Coefficients[index].aNVItemData[0], sizeof(int) );
            printf( "Index: %d\tCoefficient NV value: %d\n", index, nv_value );
         }
         // index is now 16, aka 17th NV item
         CopyMemory( (void*)& nv_value, (void*)& nv.oNV_ICI_CAL_Coefficients[index].aNVItemData[0], sizeof(int) );
         nv_value &= 0xFF; //Four bytes were copied, keep only the least significant byte for trunc bits
         printf( "\nTruncation bits NV value: %d", nv_value );
      }
   }
   else
   {
      printf( "\nICI Calibration failed. Verify the setup and make sure the UUT supports this routine." );
   }

   printf( "\n\nPress a key to continue." );
   cKey = getch();

   return bOk;
}

unsigned char Test_FTM_WCDMA_ICI_IQ_CAL(void)
{
   unsigned char bOk = true;
   unsigned char bContinue = true;
   unsigned char  cKey = ' ';

   printf("\n----------------\nWCDMA FTM ICI and IQ Mismatch CAL Test\n----------------\n");

   do
   {
      printf( "\n\nEnter 0 for ICI calibration");
      printf( "\nEnter 1 for IQ Mismatch calibration");
      printf( "\nEnter Q to exit");

      // Get a key
      cKey = toupper( getch() );

      printf("\n\n\n\n\n\n\n\n");
      switch (cKey)
      {
         case '0':
            bOk = Test_FTM_WCDMA_ICI_CAL();
            break;
         case '1':
            bOk = Test_FTM_WCDMA_IQ_MISMATCH_CAL();
            break;
         case 'Q':
            bContinue = false;
            break;
      }
   }
   while ( bContinue );

   return bOk;
}

unsigned char Test_FTM_WCDMA_IQ_MISMATCH_CAL( void )
{
   QMSL_RxIQMismatch_Cal_Meas_struct measurements;
   QMSL_RxIQMismatch_Cal_NV_struct nv;

   unsigned char bOk = true;
   unsigned char  cKey = ' ';
   unsigned short iFtm_error_code = 0;
   unsigned short iMisCompA = 0;
   unsigned short iMisCompB = 0;

   //nienchun: Default values
   int iLNARange = 0;
   int iDLPower = -65;
   int iChannel = 9750;
   unsigned short iChain =0;
   short iMode = PHONE_MODE_WCDMA_IMT;  // see which enum in Qlib_Defines.h

   printf("\n----------------\nWCDMA FTM IQ Mismatch CAL Test\n----------------\n");

   printf( "\nEnter RFMode (i.e. IMT=9):" );
   scanf( "%d", &iMode );

   printf( "\nEnter Channel:" );
   scanf( "%d", &iChannel );

   printf( "\nEnter Chain (0=PRx, 1=DRx):" );
   scanf( "%d", &iChain );

   //nienchun: Show the configuration
   printf( "\nChain is %d", iChain);
   printf( "\nBand is %d", iMode);
   printf( "\nChannel is %d", iChannel);
   printf( "\nLNA Range is %d", iLNARange );

   printf( "\n\nPlease configure call processor as follows:" );
   printf( "\n1.Set Band (i.e. IMT)" );
   printf( "\b2.Set Channel (i.e. chan=9750)" );
   printf( "\n3.Set SigGen to output CW with 1MHz positive offset (i.e. for chan=9750" );
   printf( "\n4.Set SigGen output power to -65dBm" );
   printf( "\n5.Ouput port based on receive chain selection." ); //Primary or secondary
   printf( "\n\nPress a key to continue." );

   cKey = getch();

   if ( !QLIB_FTM_SET_MODE_ID (g_hResourceContext, FTM_MODE_ID_WCDMA))
   {
      bOk = false;
      printf ("\nQLIB_FTM_SET_MODE_ID failed\n");
   }

   //Set WCDMA Mode on UUT
   if ( !QLIB_FTM_SET_MODE( g_hResourceContext, iMode ))
   {
      bOk = false;
      printf("\nQLIB_FTM_SET_MODE failed\n");
   }

   // Set Receive Chain (primary or secondary)
   bOk &= QLIB_FTM_SET_SECONDARY_CHAIN(g_hResourceContext, iChain);

   if (iChain == 1)
   {
      bOk &= QLIB_FTM_SET_MODE_ID (g_hResourceContext, FTM_MODE_WCDMA_RX1);
   }

   // Set the uplink channel
   if ( !QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel))
   {
      bOk = false;
      printf("\nQLIB_FTM_SET_CHAN failed\n");
   }

   // Set LNA Range
   bOk &= QLIB_FTM_SET_LNA_RANGE( g_hResourceContext, (unsigned char)iLNARange );

   //Obtain IQ data from UUT
   bOk &= QLIB_FTM_DO_IQ_MISMATCH_CAL ( g_hResourceContext, &iFtm_error_code, &iMisCompA, &iMisCompB );

   //Compute NV
   measurements.mismatch_value_A = iMisCompA;
   measurements.mismatch_value_A = iMisCompB;
   measurements.rx_path = iChain;

   bOk &= QLIB_RFCAL_Rx_IQMismatch_Calibration_Results( g_hResourceContext, iMode, (void*)& measurements, (void*)& nv, 1 );

   if( bOk )
   {
      int a=0;
      int b=0;

      a = (int)iMisCompA;
      b = (int)iMisCompB;

      printf( "\nMisCompA = %d, MisCompB = %d", a, b );
   }
   else
   {
      printf( "\nIQ Mismatch Calibration failed. Verify the setup and make sure the UUT supports this routine." );
   }

   return bOk;
}

unsigned char WCDMA_Daul_Carrier_and_ICI_Cal( void )
{
   unsigned char bOk = true;
   int _iComPort = 0;
   unsigned char _cKey = '\0';

   printf("\n\n==============================\nWCDMA Dual Carrier and ICI Calibration:\n==============================");

   printf("\n1=WCDMA Dual Carrier");
   printf("\n2=WCDMA ICI Cal");

   _cKey = toupper( getch());

   if ( _cKey == '1' )
   {
      Test_Dual_Carrier();
   }
   if ( _cKey == '2' )
   {
      Test_FTM_WCDMA_ICI_IQ_CAL();
   }
   return bOk;
}
unsigned char Test_Dual_Carrier()
{
   unsigned char bOk = true;
   unsigned char  cKey = ' ';
   unsigned short iFtm_error_code = 0;

   unsigned int iChannel = 9750;
   unsigned short iChain =0;
   short iMode = PHONE_MODE_WCDMA_IMT;
   short iRX_AGC = 0;

   printf("\n----------------\nWCDMA FTM Dual Carrier Test\n----------------\n");

   printf( "\nEnter RFMode (i.e. IMT=9):" );
   scanf( "%hd", &iMode );

   printf( "\nEnter Carrier0 UL Channel:" );
   scanf( "%d", &iChannel );

   //Show configuration vars
   printf( "\nRF Mode is %d", iMode);
   printf( "\nChannel is %d", iChannel);

   //Set the system mode ID
   if ( !QLIB_FTM_SET_MODE_ID (g_hResourceContext, FTM_MODE_ID_WCDMA))
   {
      bOk = false;
      printf ("\nQLIB_FTM_SET_MODE_ID failed\n");
      return bOk;
   }

   //Set WCDMA Mode on UUT
   if ( !QLIB_FTM_SET_MODE( g_hResourceContext, iMode ))
   {
      bOk = false;
      printf("\nQLIB_FTM_SET_MODE failed\n");
      return bOk;
   }

   // Set the UUT into dual carrier mode
   if ( !QLIB_FTM_SET_MULTI_CHAN( g_hResourceContext, iChannel, iChannel+25))
   {
      bOk = false;
      printf("\nQLIB_FTM_SET_MULTI_CHAN failed\n");
      return bOk;
   }

   // Get the carrier1 RxAGC
   if ( !QLIB_FTM_GET_CAGC_DUAL_CARRIER_RX_AGC( g_hResourceContext, &iRX_AGC ) )
   {
      bOk = false;
      printf("\nQLIB_FTM_GET_CAGC_DUAL_CARRIER_RX_AGC failed\n");
      return bOk;
   }
   else
   {
      printf( "\nDual-carrier Carrier1 RxAGC is: %d", iRX_AGC );
   }
   return bOk;
}
unsigned char Test_FTM_CAL_API_V2 (void )
{
   unsigned char bOk=true;
   unsigned char cKey;
   int i = 0;

   printf("----------------\nFTM CDMA RF Calibration API V2\n----------------\n");

   if ( bOk )
   {
      printf("\n\n1=Commit RF calibration to NV");
      printf("\n2=Calibrate DVGA");
      printf("\n3=Calibrate LNA");
      printf("\n4=Calibrate IM2");
      printf("\n5=Calibrate IntelliCeiver");
      cKey = getch();

      if (cKey == '1' )
      {
         unsigned short iBandClass = 0;
         bOk = QLIB_FTM_CDMA_API2_COMMIT_RF_TO_NV( g_hResourceContext, iBandClass);
         printf("\nQLIB_FTM_CDMA_API2_COMMIT_RF_TO_NV: Band Class %d :%s", iBandClass, bOk ? "PASS" : "FAIL\n" );
      }

      if (cKey == '2' )
      {
         unsigned short RxAGC = - 512 + 10 *(-80 + 115) ; // -80dBm
         int count = 3;
         unsigned short *array = new unsigned short[count];
         // Primary chain
         bOk = QLIB_FTM_CDMA_API2_CALIBRATE_DVGA( g_hResourceContext, 0, RxAGC, (unsigned char*) &count, array);
         printf("\nQLIB_FTM_CDMA_API2_CALIBRATE_DVGA(primary): %s \nValue:", bOk ? "PASS" : "FAIL\n" );
         for(i = 0; i < count; i++)
            printf("array[i] ");
         // Diversity chain
         bOk = QLIB_FTM_CDMA_API2_CALIBRATE_DVGA( g_hResourceContext, 1, RxAGC, (unsigned char*) &count, array);
         printf("\nQLIB_FTM_CDMA_API2_CALIBRATE_DVGA(diversity): %s \nValue:", bOk ? "PASS" : "FAIL\n" );
         for(i = 0; i < count; i++)
            printf("array[i] ");
      }
      if (cKey == '3' )
      {
         unsigned short RxAGC = - 512 + 10 *(-80 + 115) ; // -80dBm
         int count = 3;
         unsigned short *array = new unsigned short[count];
         // Primary chain
         bOk = QLIB_FTM_CDMA_API2_CALIBRATE_LNA( g_hResourceContext, 0, RxAGC, 0, (unsigned char*) &count, array);
         printf("\nQLIB_FTM_CDMA_API2_CALIBRATE_LNA 0 (primary): %s \nValue:", bOk ? "PASS" : "FAIL\n" );
         for(i = 0; i < count; i++)
            printf("array[i] ");
         // Diversity chain
         bOk = QLIB_FTM_CDMA_API2_CALIBRATE_LNA( g_hResourceContext, 1, RxAGC, 0, (unsigned char*) &count, array);
         printf("\nQLIB_FTM_CDMA_API2_CALIBRATE_LNA 0 (diversity): %s \nValue:", bOk ? "PASS" : "FAIL\n" );
         for(i = 0; i < count; i++)
            printf("array[i] ");
      }

      if (cKey == '4' )
      {
         unsigned short suppression = 0;

         int count = 3;
         unsigned char *array = new unsigned char[count];
         // Primary chain
         bOk = QLIB_FTM_CDMA_API2_CALIBRATE_IM2( g_hResourceContext, 0, &suppression,  (unsigned char*) &count, array);
         printf("\nQLIB_FTM_CDMA_API2_CALIBRATE_IM2 0 (primary): %s \n Suppression &d \nValue:",
                bOk ? "PASS" : "FAIL\n",
                suppression);
         for(i = 0; i < count; i++)
            printf("array[i] ");
         // Diversity chain
         bOk = QLIB_FTM_CDMA_API2_CALIBRATE_IM2( g_hResourceContext, 1, &suppression,  (unsigned char*) &count, array);
         printf("\nQLIB_FTM_CDMA_API2_CALIBRATE_IM2 0 (diversity): %s \n Suppression &d \nValue:",
                bOk ? "PASS" : "FAIL\n",
                suppression);
         for(i = 0; i < count; i++)
            printf("array[i] ");
      }
      if (cKey == '5' )
      {
         unsigned short RxAGC = - 512 + 10 *(-80 + 115) ; // -80dBm

         unsigned char array[128];
         unsigned char arraySize = 128;
         bOk = QLIB_FTM_CDMA_API2_CALIBRATE_INTELLICEIVER( g_hResourceContext, 0, RxAGC,  &arraySize, array);
      }
   }
   return 0;
}

void Init_GSM_IP2_Params (GSM_IP2_Params& params)
{
    params.use_default_params = 1;
    params.band = 0; //0 - 850; 1- 900; 2-1800; 3-1900
    params.startICode = 45;
    params.startQCode = 55;
    params.unitStepSize = 1;
    params.width = 2;
    params.ARFCN = 128; //128- 850 band; 1- 900 band; 512 - PCS/DCS
    params.txRgi = 29;
    params.rxLNARange = 1;
    params.numberOfIteration = 3;
    params.stepDuration = 150; //in quarter-symbols
    params.dcSettlingTime = 50; //in quarter-symbols
}
unsigned char Test_GSM_IP2_Cal()
{
    GSM_IP2_Params gsmIp2Params;
    Init_GSM_IP2_Params(gsmIp2Params);
    FTM_RF_Mode_Enum mode;
    mode = PHONE_MODE_GSM_850;

    short final_q_code = 0;
    short final_i_code = 0;
    int min_im2_result = 0;
    unsigned char status = 0;
#ifdef ON_TARGET
    QLIB_FTM_SET_MODE( g_hResourceContext, mode);// Set mode done only once. i.e no need to do it for every band.

  // Call IIP2 Cal
   QLIB_FTM_GSM_DO_IP2_CAL( g_hResourceContext, 
      gsmIp2Params.use_default_params,
       gsmIp2Params.startICode,
       gsmIp2Params.startQCode,
       gsmIp2Params.band,
       gsmIp2Params.unitStepSize,
       gsmIp2Params.width,
       gsmIp2Params.ARFCN,
       gsmIp2Params.txRgi,
       gsmIp2Params.rxLNARange,
       gsmIp2Params.numberOfIteration,
       gsmIp2Params.stepDuration,
       gsmIp2Params.dcSettlingTime,
       final_q_code,
       final_i_code,
       min_im2_result,
       status);
#endif
   return true;
}
/***************************GSM IP2 cal ends***************************************/

//#define ON_TARGET
char_DPD_Tbl_V3_V4 charTableV3_V4[3][4]; //3 channel, 4RGI
char_DPD_Tbl charTable[3][4]; //3 channel, 4RGI
int charChannel[NUM_CHAR_CHAN] = {975,37,124};
unsigned char gsmV4CmdBuf[6000];
int cmdBufferSize = 0;
double charV4_V3_Delta[4]; //for 4 bands
double charDAPwr[3][2][2][32]=
{
   {
      {  {7.23333,7.73333,8.93333,9.56667,10.7667,11.7333,12.8,13.8333,14.8333,16,17,18.1,19.5,20.7333,21.6,22.9667,24.2333,25.2667,26.2667,27.4667,28.5,29.5667,30.3,31.2667,31.7,32.1333,32.4667,32.6667,32.8,32.9,33,33.066}, \
         {6.4,6.8,7.93333,8.56667,9.8,10.7667,11.8333,12.9,13.9667,15.2,16.2,17.3,18.8,20.0667,20.9667,22.4333,23.7333,24.8333,25.8333,27,28.0333,29.1,29.8,30.7,31.1,31.4667,31.7,31.8,31.9667,32,32.0667,32.1}
      }, \

      {  {6.13333,6.36667,7.33333,8.33333,9.23333,10.4333,11.4333,12.5,13.5667,14.6,15.8333,16.9667,18.1667,19.7333,21.0667,22.2333,23.3333,24.4667,25.5333,26.6333,27.6333,28.7667,29.7667,30.6333,31.3667,31.8333,32.1333,32.3667,32.5333,32.6667,32.7667,32.8333}, \
         {5.3,5.5,6.36667,7.33333,8.23333,9.43333,10.4,11.5667,12.6,13.7,14.9667,16.1667,17.4,19.0667,20.4333,21.6667,22.7333,23.9333,25.0667,26.1333,27.1333,28.2333,29.2333,30.0667,30.7667,31.1667,31.4333,31.6333,31.7667,31.8333,31.9,31.9333}
      } \
   },
   {
      {  {6.9,7.36667,8.53333,9.2,10.4,11.4,12.4,13.4333,14.5,15.6,16.5667,17.5667,18.9667,20.1667,21.0333,22.4,23.6333,24.7333,25.7333,26.9333,28.0667,29.2667,30.1333,31.2667,31.7,32.2333,32.4667,32.6667,32.7333,32.8333,32.8667,32.8667},\
         {5.96667,6.43333,7.56667,8.23333,9.4,10.4,11.4333,12.5,13.6,14.7333,15.7667,16.8,18.2667,19.5333,20.4333,21.8333,23.1,24.2,25.2667,26.5333,27.6,28.8,29.6333,30.7333,31.1333,31.5667,31.7667,31.8333,31.9333,31.9667,31.9667,31.9667}
      }, \

      {  {5.8,6,7,8,8.96667,10.1333,11.1,12.2,13.2,14.3,15.4333,16.5667,17.7,19.2333,20.5333,21.7,22.8,24,25.0333,26.1,27.2,28.4,29.4667,30.4333,31.3,31.8,32.1667,32.3667,32.5333,32.6667,32.7333,32.7333}, \
         {4.96667,5.1,6,6.96667,7.9,9.1,10.0667,11.1667,12.2667,13.3667,14.5667,15.7333,16.9333,18.5333,19.9,21.1333,22.2,23.4333,24.5667,25.6333,26.7333,27.9,28.9667,29.9,30.7,31.1333,31.4333,31.6333,31.7667,31.8333,31.8667,31.9333}
      } \
   }, \
   {
      {  {6.2,6.73333,7.9,8.63333,9.8,10.8,11.8,12.8333,13.9,14.8667,15.8667,16.8333,18.2,19.3333,20.2333,21.5333,22.7333,23.8333,24.8333,26.1,27.2333,28.5333,29.4667,30.7667,31.1333,31.6,31.8,31.9,31.9333,32,32.0333,32.0333} ,\
         {5.33333,5.73333,6.93333,7.6,8.8,9.76667,10.8333,11.9,12.9667,14,15.0333,16.0333,17.4667,18.6667,19.5667,20.9333,22.2333,23.3333,24.3333,25.6333,26.8,28.1,29.0333,30.2333,30.5667,31,31.1,31.1667,31.2333,31.2333,31.2333,31.2333}
      },\

      {  {5.16667,5.43333,6.4,7.36667,8.4,9.6,10.4667,11.5667,12.6333,13.7333,14.7667,15.8333,16.9333,18.4667,19.7667,20.9333,21.9667,23.1667,24.2667,25.3333,26.4667,27.7,28.8333,29.8,30.6667,31.1667,31.4333,31.6667,31.8,31.8667,31.9,31.9667},\
         {4.26667,4.5,5.43333,6.36667,7.33333,8.56667,9.46667,10.6,11.6667,12.8,13.9,15.0333,16.2,17.7667,19.1,20.3,21.4,22.6333,23.7667,24.8667,26,27.2,28.3333,29.3,30.1333,30.5667,30.8333,31,31.1,31.1667,31.1667,31.2333}
      } \
   }
};
char_Rx_Tbl charRx[NUM_RX_CHAR_CHAN] = {{1, {0,1,2,3},{142.666875,130.729375,130.666875,106.270625}},\
   {31,{0,1,2,3},{142.604375,130.708125,130.604375,105.291875}},\
   {62, {0,1,2,3},{142.0625,130.125,130.020625,105.520625}},\
   {92, {0,1,2,3}, {142.354375,130.458125,130.375,105.645625}},\
   {124, {0,1,2,3}, {141.375,129.479375,129.333125,104.625}},\
   {975, {0,1,2,3}, {142.145625,130.25,130.145625,104.833125}},\
   {1000, {0,1,2,3}, {142.708125,130.770625,130.708125,105.4375}},\
   {1023, {0,1,2,3}, {142.645625,130.729375,130.645625,106.270625}}
};

/*Init_GSM_V4_DPD_Params is for initializing the DPD cal params*/
void Init_GSM_V4_DPD_Params(GSM_V4_DPD_Params &gsmV4DPDParams, unsigned int uiNumBands, unsigned int uiNumChannels)
{
   gsmV4DPDParams.boolPerformDPD = true;
   gsmV4DPDParams.boolSlopeAlignLow = false;
   /*Only one of slope correction or piecewise linear should be enabled. If both are set to true,
   the slope correction is chosen*/
   gsmV4DPDParams.boolDoSlopeCorrection = true;
   gsmV4DPDParams.boolDoPieceWiseLinear = false;
   gsmV4DPDParams.boolDoPowerAdaptation = true;
   gsmV4DPDParams.boolDoAdaptiveMaxPwrEst = false; //false for platforms where V3 and V4 digital gain powers have delta; true for others
   /*If adaptive max power estimation is made true when there is a delta, then the max power estimation will be incorrect. Hence make it false.*/
   gsmV4DPDParams.usDigGainForPredistRGI = 47;

   vector<unsigned int> digiGain;
   digiGain.push_back(5);
   digiGain.push_back(15);
   digiGain.push_back(40);
   digiGain.push_back(47);
   digiGain.push_back(60);
   unsigned int bandIdx;
   for(bandIdx = 0; bandIdx < uiNumBands; bandIdx++)
   {
      gsmV4DPDParams.uiDpdDigGainList.push_back(digiGain);
   }
   gsmV4DPDParams.uiNumDpdDigGain = gsmV4DPDParams.uiDpdDigGainList[0].size();

   unsigned short startRGI[1] = {19}; //add more for more than 1 band;
   vector<unsigned int> rgiList;
   gsmV4DPDParams.uiNumDpdRgi = 4;
   //RGIs used in this example 19,20,21,22
   for(bandIdx = 0; bandIdx < uiNumBands; bandIdx++)
   {
      rgiList.clear();
      for(unsigned int rgiIdx = 0; rgiIdx < gsmV4DPDParams.uiNumDpdRgi; rgiIdx++)
      {
         rgiList.push_back(startRGI[bandIdx] + rgiIdx);
      }
      gsmV4DPDParams.uiDpdRgiList.push_back(rgiList);
   }

   gsmV4DPDParams.dCalRGIPowerLowBand = (double) 27.0;
   gsmV4DPDParams.dCalRGIPowerHighBand = (double) 26.0;

   gsmV4DPDParams.uiCalPaRange = 0; //PA range 0 for DPD sweep

   gsmV4DPDParams.dDacScale = (double)4.0;

   for(unsigned int chanIdx = 0; chanIdx < uiNumChannels; chanIdx++)
   {
      gsmV4DPDParams.uiNvChanMap.push_back(chanIdx+1); //1,2,3
   }

   //Digital gains used for slope correction
   digiGain.clear();
   digiGain.push_back(5);
   digiGain.push_back(15);
   digiGain.push_back(40);
   for(bandIdx = 0; bandIdx < uiNumBands; bandIdx++)
   {
      gsmV4DPDParams.uiSlopeCorrectionDigGainList.push_back(digiGain);
   }

   gsmV4DPDParams.dMinDpdPowDelta = (double)1.5;

    
    gsmV4DPDParams.boolUseV3CharData = true; 

    gsmV4DPDParams.boolDoRGIRunOnly = false;

    gsmV4DPDParams.boolUseAdjacentRGI = false;
}
/*Init_GSM_V4_DA_Params() initializes the settings/parameters for DA (PCL) sweep*/
void Init_GSM_V4_DA_Params(GSM_V4_DA_Params &gsmV4DAParams, unsigned int uiNumBands)
{
   vector<unsigned int> pclIdxVec;

   gsmV4DAParams.boolPerformDA = true;
   /*If true, saves the PCL error in separate entries in TX_DATA_CAL NV.
   If false then the error will be adapted to RGI vs power table and TX_CAL_DATA NV won't contain the PCL error entries*/
   gsmV4DAParams.boolUsePCLErrorNV = true;
   gsmV4DAParams.uiNumMods = 2; //GSMK and 8PSK
   gsmV4DAParams.uiModList.push_back(4); //MCS4
   gsmV4DAParams.uiModList.push_back(9); //MCS9
   gsmV4DAParams.uiNumPCLs = 16;

   for(unsigned int bandIdx = 0; bandIdx < uiNumBands; bandIdx++)
   {
      pclIdxVec.clear();
      for(unsigned int pclIdx = 0; pclIdx < gsmV4DAParams.uiNumPCLs; pclIdx++)
      {
         pclIdxVec.push_back(pclIdx + gsmV4_minLowBand_PCL); //PCL4 to PCL19
      }
      gsmV4DAParams.uiPclList.push_back(pclIdxVec);
   }
   unsigned short curPCLIdx = gsmV4_maxNumPCL - 1;

   // fill the low band map
   for (unsigned short curPCLVal=gsmV4_minLowBand_PCL; curPCLVal<=gsmV4_maxLowBand_PCL; ++curPCLVal)
   {
      //PCL 4 to Index 15, PCL 5 to Index 14 .. PCL 19 to Index 0
      //  gsmV4DAParams.usBandPCl_map.insert( pair<unsigned short, unsigned short>(curPCLVal, curPCLIdx) );
      gsmV4DAParams.usBandPCl_map.insert( make_pair (curPCLVal, curPCLIdx) );

      --curPCLIdx;
   }
   unsigned short curIdx;
   vector<unsigned short> bandPCL;
   for(unsigned int bandIdx = 0; bandIdx < uiNumBands; bandIdx++)
   {
      bandPCL.clear();
      for (unsigned short i=0; i<gsmV4DAParams.uiNumPCLs; ++i)
      {
         curIdx = static_cast<unsigned short>( gsmV4DAParams.uiPclList[bandIdx][i] );
         bandPCL.push_back(gsmV4DAParams.usBandPCl_map[curIdx ]); //bandPCL - 15,14,..0
      }
      gsmV4DAParams.usPclIdxList.push_back(bandPCL);
   }
   gsmV4DAParams.dPclNvPowOffset = (double)0.0;
   gsmV4DAParams.dMaxPclPowDelta = (double)5.0;

   //Mod list Index 3 (MCS 4) and 8 (MCS 9)
   for (unsigned int i=0; i<gsmV4DAParams.uiNumMods; ++i)
   {
      gsmV4DAParams.usModIdxList.push_back(static_cast<unsigned short>( gsmV4DAParams.uiModList[i] - gsmV4_minMCS ));
   }
}

/* Init_GSM_V4_Rx_Params intializes the cal parameters for Rx*/
void Init_GSM_V4_Rx_Params(GSM_V4_Rx_Params &gsmV4RxParams)
{
   gsmV4RxParams.boolPerformRxCal = true;
   gsmV4RxParams.uiNumRxPower = 4;
   gsmV4RxParams.uiNumLNAState = 4;
   //Call box powers for different LNA gain ranges*/
   gsmV4RxParams.iRxPowerList.push_back(-90);
   gsmV4RxParams.iRxPowerList.push_back(-90);
   gsmV4RxParams.iRxPowerList.push_back(-54);
   gsmV4RxParams.iRxPowerList.push_back(-54);

   gsmV4RxParams.boolPerformHiLinRxCal = false;
   gsmV4RxParams.uiNumLNAStateHiLin = 4;
   gsmV4RxParams.uiNumLNAPowerHiLin = 0;

   for(unsigned int lnaIdx = 0; lnaIdx < gsmV4RxParams.uiNumLNAState; lnaIdx++)
   {
      gsmV4RxParams.uiLNAStateList.push_back(lnaIdx); //0,1,2,3
      gsmV4RxParams.uiLNAStateListHiLin.push_back(lnaIdx);
   }

   gsmV4RxParams.lnaPowerListAllMode = gsmV4RxParams.iRxPowerList;
   gsmV4RxParams.lnaStateListAllMode = gsmV4RxParams.uiLNAStateList;

   if(gsmV4RxParams.boolPerformHiLinRxCal)
   {
       for( unsigned int index = 0; index < gsmV4RxParams.uiNumLNAState; ++index)
       {
            unsigned int lna = gsmV4RxParams.uiLNAStateList[ index ];
            if( find(gsmV4RxParams.uiLNAStateListHiLin.begin(), gsmV4RxParams.uiLNAStateListHiLin.end(), lna) != gsmV4RxParams.uiLNAStateListHiLin.end() )     
            {            
               gsmV4RxParams.iLNAPowerListHiLin.push_back( gsmV4RxParams.iRxPowerList[ index ] );
            }
       }
       gsmV4RxParams.uiNumLNAPowerHiLin = gsmV4RxParams.iLNAPowerListHiLin.size();
       for(unsigned int index = 0; index < gsmV4RxParams.uiNumLNAStateHiLin; index++)
       {
            //bump up high linearity state by 128 to differentiate from low linearity.
            gsmV4RxParams.lnaStateListAllMode.push_back(gsmV4RxParams.uiLNAStateListHiLin[index] + 128); 
            gsmV4RxParams.lnaPowerListAllMode.push_back(gsmV4RxParams.iLNAPowerListHiLin[index]);
       }
   }
}

/*Init_GSM_V4_Cal_Params initializes the common calibration parameters
and calls functions to initialize parameters for DA, DPD and Rx cal*/
void Init_GSM_V4_Cal_Params(GSM_V4_Cal_Params &gsmV4CalParams)
{
   gsmV4CalParams.uiNumBands = 1;
   gsmV4CalParams.uiBandList.push_back(10); //10-EGSM band
   gsmV4CalParams.uiNumChan = 3;

   vector<unsigned int> chanList;
   /*Channel list fo EGSM band*/
   chanList.push_back(975);
   chanList.push_back(37);
   chanList.push_back(124);

   gsmV4CalParams.uiChanList.push_back(chanList);

   /*No. of chains used in this example is 1 (i.e chain 0)*/
   gsmV4CalParams.uiNumTxChains = 1;
   gsmV4CalParams.uiNumRxChains = 1;
   gsmV4CalParams.uiTxChainList.push_back(0);
   gsmV4CalParams.uiRxChainList.push_back(0);

   /*Functions to initialize DA, DPD and Rx parameters*/
   Init_GSM_V4_DA_Params(gsmV4CalParams.gsmV4DAParams, gsmV4CalParams.uiNumBands);
   Init_GSM_V4_DPD_Params(gsmV4CalParams.gsmV4DPDParams, gsmV4CalParams.uiNumBands, gsmV4CalParams.uiNumChan);
   Init_GSM_V4_Rx_Params(gsmV4CalParams.gsmV4RxParams);

   /*No of Tx slots 4 and No of Rx slot 1 in each frame*/
   gsmV4CalParams.uiNumTxSlots = 4;
   gsmV4CalParams.uiNumRxSlots = 1;

   gsmV4CalParams.ucTxSlotConfig.resize(GSM_V4_MAX_NUM_OF_SLOTS);
   // Note: only [1,1,1,1,0,0,0,0] or [0,1,1,1,1,0,0,0] work due to the 3 slot shift on UUT side
   // Note: If 2 chains are specified, then only [1,1,1,1,0,0,0,0] works.
   // Chain 2 is specified in the same slot as Chain 0 in the command config, but is shifted by 4 slots on the UE.

   gsmV4CalParams.ucTxSlotConfig.resize(sizeof(char) * 8);
   gsmV4CalParams.ucTxSlotConfig[0] = 1;
   gsmV4CalParams.ucTxSlotConfig[1] = 1;
   gsmV4CalParams.ucTxSlotConfig[2] = 1;
   gsmV4CalParams.ucTxSlotConfig[3] = 1;
   gsmV4CalParams.ucTxSlotConfig[4] = 0;
   gsmV4CalParams.ucTxSlotConfig[5] = 0;
   gsmV4CalParams.ucTxSlotConfig[6] = 0;
   gsmV4CalParams.ucTxSlotConfig[7] = 0;

   // Add the Call Box Slot Configuration
   // Default to copying over the slot configuration --> case for 1 Tx chain
   gsmV4CalParams.ucTxSlotConfig_CB = gsmV4CalParams.ucTxSlotConfig;
   // if 2 chains are used, add slots for second chain (shifted by 4 relative to first chain)
   if (gsmV4CalParams.uiNumTxChains > 1)
   {
      // (GSM_V4_MAX_NUM_OF_SLOTS - GSM_V4_SEC_CHAIN_TX_SHIFT_ON_PHONE + 1) insures vector bounds
      for (unsigned char slotIdx=0; slotIdx < (GSM_V4_MAX_NUM_OF_SLOTS - GSM_V4_SEC_CHAIN_TX_SHIFT_ON_PHONE + 1); ++slotIdx)
      {
         if (gsmV4CalParams.ucTxSlotConfig[slotIdx] > 0)
         {
            gsmV4CalParams.ucTxSlotConfig_CB[slotIdx + GSM_V4_SEC_CHAIN_TX_SHIFT_ON_PHONE] = 1;
         }
      }
   }

   gsmV4CalParams.ucRxSlotConfig.resize(GSM_V4_MAX_NUM_OF_SLOTS);
   // Note: Only 1 Rx slot can be used
   // Note: Rx slots can only go in 0 or 1 with Tx configration of [1,1,1,1,0,0,0,0]
   // Note: Rx slots can only go in 1 or 2 with Tx configration of [0,1,1,1,1,0,0,0]
   gsmV4CalParams.ucRxSlotOffset = 0;
   if ( gsmV4CalParams.gsmV4RxParams.boolPerformRxCal )
   {
      gsmV4CalParams.ucRxSlotConfig[0] = 1;
      gsmV4CalParams.ucRxSlotOffset = 0;
   }
}

/*Compute_Frame_Count computes DA, DPD and Total frame count based on the cal parameter settings*/
void Compute_Frame_Count(GSM_V4_Cal_Params &gsmV4CalParams)
{
   unsigned int iTemp,numDpdFramesPerChannel = 0;
   gsmV4CalParams.uiNumDAFrames = 0;
   gsmV4CalParams.uiNumDPDFrames = 0;
   gsmV4CalParams.uiNumDPDFramesPerChannel = 0;

   if(!gsmV4CalParams.gsmV4DAParams.boolPerformDA && gsmV4CalParams.gsmV4RxParams.boolPerformRxCal)
   {
      /*If Rx cal only, then the DA frame count is LNA state * No. of Mods * No.of channels * No. of bands*/
      gsmV4CalParams.uiNumDAFrames = gsmV4CalParams.gsmV4RxParams.uiNumLNAState * gsmV4CalParams.gsmV4DAParams.uiNumMods * gsmV4CalParams.uiNumBands * gsmV4CalParams.uiNumChan;
   }
   else
   {
      /*If DA cal true, then DA frame count is (Num PCLs/Num Tx slots) * No.of mods * No. of channels * No. of bands*/
      gsmV4CalParams.uiNumDAFrames = (gsmV4CalParams.gsmV4DAParams.uiNumPCLs/gsmV4CalParams.uiNumTxSlots) * gsmV4CalParams.gsmV4DAParams.uiNumMods * gsmV4CalParams.uiNumBands * gsmV4CalParams.uiNumChan;
   }

   /*If DPD is performed*/
   if(gsmV4CalParams.gsmV4DPDParams.boolPerformDPD || gsmV4CalParams.gsmV4DPDParams.boolDoRGIRunOnly)
   {
      iTemp = (gsmV4CalParams.gsmV4DPDParams.uiNumDpdDigGain * gsmV4CalParams.gsmV4DPDParams.uiNumDpdRgi); //total DPD slots
      numDpdFramesPerChannel = iTemp/gsmV4CalParams.uiNumTxSlots;
      gsmV4CalParams.uiNumDPDFrames = (iTemp/gsmV4CalParams.uiNumTxSlots) * gsmV4CalParams.uiNumBands * gsmV4CalParams.uiNumChan;

      // add an extra frame if the total DPD slots isn't evenly divisible by No.of Tx slots
      // For example: No.of Tx slots typically equals 4
      //              4 RGIs x 5 DigitalGains = 20 --> 5 frames
      //              5 RGIs x 5 DigitalGains = 25 --> 6 frames + 1 extra slots --> so, use 7 frames
      if(iTemp%gsmV4CalParams.uiNumTxSlots)
      {
         gsmV4CalParams.uiNumDPDFrames += (gsmV4CalParams.uiNumBands * gsmV4CalParams.uiNumChan);
         numDpdFramesPerChannel += 1;
      }
   }
   gsmV4CalParams.uiNumDPDFramesPerChannel = numDpdFramesPerChannel;
   gsmV4CalParams.uiNumFrames = gsmV4CalParams.uiNumDAFrames + gsmV4CalParams.uiNumDPDFrames;

   if ( gsmV4CalParams.uiNumFrames > GSM_V4_MAX_NUM_OF_FRAMES )   // exception
   {
      //throw exception "Number of frames,  exceeds allowed number of frames (200), Modify the GSM V4 params to reduce the number of frames. "
   }
   //number of frames from call box perspective (i.e with interband delay)
   //UE automatically inserts 4 frame delay between the bands.
   gsmV4CalParams.uiNumFrames_CB = gsmV4CalParams.uiNumFrames + GSMV4_INTRA_BAND_FRAME_DELAY*(gsmV4CalParams.uiNumBands - 1);

   gsmV4CalParams.uiNumFramesPerChannel = gsmV4CalParams.uiNumFrames/gsmV4CalParams.uiNumBands/gsmV4CalParams.uiNumChan;
   gsmV4CalParams.uiNumDAFramesPerChannel = gsmV4CalParams.uiNumFramesPerChannel - gsmV4CalParams.uiNumDPDFramesPerChannel;
}

void Build_DPD_Sweep_Containers(GSM_V4_Cal_Params &gsmV4CalParams)
{
   unsigned int numElemDpdSweep = gsmV4CalParams.gsmV4DPDParams.uiNumDpdRgi * gsmV4CalParams.gsmV4DPDParams.uiNumDpdDigGain;

   unsigned int uiCurBand  = 0;
   unsigned int uiDigGainIdx  = 0;
   unsigned int uiRgiIdx      = 0;
   unsigned short usVectorIdx = 0;
   for(unsigned int uiBandIdx = 0; uiBandIdx < gsmV4CalParams.uiNumBands; ++uiBandIdx)
   {
      uiCurBand = gsmV4CalParams.uiBandList[uiBandIdx];
      gsmV4CalParams.gsmV4DPDParams.dpdDigGainMapVec[uiCurBand].resize(numElemDpdSweep);
      gsmV4CalParams.gsmV4DPDParams.dpdRGIMapVec[uiCurBand].resize(numElemDpdSweep);
      /*If the digi gains listed are 5,15,40,47,60 and RGIs are 19,20,21,22
      then diggainMapvec contains 5,5,5,5,15,15,...60,60,60,60; RGImapvec contains 19,20,21,22,19,20,21,22...19,20,21,22*/
      for (uiDigGainIdx=0; uiDigGainIdx < gsmV4CalParams.gsmV4DPDParams.uiNumDpdDigGain; ++uiDigGainIdx)
      {
         for (uiRgiIdx=0; uiRgiIdx < gsmV4CalParams.gsmV4DPDParams.uiNumDpdRgi; ++uiRgiIdx)
         {
            gsmV4CalParams.gsmV4DPDParams.dpdDigGainMapVec[uiCurBand][usVectorIdx] = gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[uiBandIdx][uiDigGainIdx];
            gsmV4CalParams.gsmV4DPDParams.dpdRGIMapVec[uiCurBand][usVectorIdx] = gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[uiBandIdx][uiRgiIdx];
            ++usVectorIdx;
         }
      }
      usVectorIdx = 0;
   }
}
void Create_Config_Command_FrameDuplication(GSM_V4_Cal_Params &gsmV4CalParams, unsigned char cptrGSMCmdBuf[], unsigned int &uiCmdBufSize, unsigned short usNumFrameDup)
{
   // frame duplication causes twice the number of frames
   unsigned short numFrames = static_cast<unsigned short>( gsmV4CalParams.uiNumFrames ) * usNumFrameDup;
   GSM_V4_Cal_Config_Header cmdHeader;

   //Header
   cmdHeader.numFrames = static_cast<unsigned short>(gsmV4CalParams.uiNumFrames);
   cmdHeader.numChains = static_cast<unsigned short>(gsmV4CalParams.uiNumTxChains);
   for (unsigned char i=0; i< gsmV4CalParams.uiNumTxChains; ++i)
   {
      cmdHeader.a_devId[i] = static_cast<unsigned char>( gsmV4CalParams.uiTxChainList[i] );  // add device (chain ID) list
   }
   cmdHeader.reserved = 0;
   memcpy( cptrGSMCmdBuf, &cmdHeader, sizeof(GSM_V4_Cal_Config_Header) );
   cptrGSMCmdBuf += sizeof(GSM_V4_Cal_Config_Header);

   // # of slots used per frame
   const unsigned char  constNumDaCalSlots  = static_cast<unsigned char>( gsmV4CalParams.uiNumRxSlots * gsmV4CalParams.uiNumRxChains + gsmV4CalParams.uiNumTxSlots * gsmV4CalParams.uiNumTxChains );
   const unsigned char  constNumDpdCalSlots = static_cast<unsigned char>( gsmV4CalParams.uiNumTxSlots * gsmV4CalParams.uiNumTxChains);
   // 8 slots per TDMA frame
   const unsigned char  constNumSlotsPerFrame = GSM_V4_MAX_NUM_OF_SLOTS;
   // # of DPD tables being added per channel
   const unsigned char constNumDpdTblPerChan = static_cast<unsigned char>( gsmV4CalParams.uiNumDPDFramesPerChannel );

   // DPD table settings - these are constant
   const unsigned char constDpdPaState = static_cast<unsigned char>( gsmV4CalParams.gsmV4DPDParams.uiCalPaRange );   // PA range - nominally is PA0, but is user input dependent
   const unsigned char constDpdModType = 1;   // 0 - 8PSK, 1 - GMSK

   unsigned char ucFrameSlotIdx  = 0;   // the frame slot index, 0 to 7
   unsigned char ucCurPclIdx     = 0;   // the current PCL index
   unsigned char ucCurLnaIdx     = 0;   // the current LNA index
   unsigned char ucCurChanIdx    = 0;   // the current channel index
   unsigned char ucCurBandIdx    = 0;   // the current band index
   unsigned char ucDpdIdx        = 0;
   unsigned short usFrameIdx     = 0;   // the current frame index

   unsigned char ucDpdSweepSize = static_cast<unsigned char>(gsmV4CalParams.gsmV4DPDParams.uiNumDpdRgi * gsmV4CalParams.gsmV4DPDParams.uiNumDpdDigGain);

   unsigned short usPclSweepFrameMarker = static_cast<unsigned short>(gsmV4CalParams.uiNumDAFramesPerChannel);
   unsigned short usDpdSweepFrameMarker = static_cast<unsigned short>(gsmV4CalParams.uiNumDPDFramesPerChannel);

   bool boolDoneWithFirstMod = false;   // flag indicating whether GSM or EDGE is to be used

   unsigned char ucCurTxSlotFunc = 0;   // the current slot function: Rx/Tx/NoOp
   unsigned char ucCurRxSlotFunc = 0;   // the current slot function: Rx/Tx/NoOp

   GSM_V4_Frame_Header curFrameHeader; // current frame header; values updated for each frame
   GSM_V4_Slot_Payload curSlotPayload; // current slot payload; added to each frame for each slot
   memset( &curFrameHeader, 0, sizeof(GSM_V4_Frame_Header) );
   memset( &curSlotPayload, 0, sizeof(GSM_V4_Slot_Payload) );

   // Required Items for Frame Duplication
   // min size = (4 bytes for frame header) + (7 bytes per slot * max of 16 slots) = 116 bytes
   unsigned char tempFramePayload[120];
   unsigned char* tempFramePyld_Ptr = tempFramePayload;
   unsigned short tempFramePyld_Size = 0;

   unsigned short frameDupIdx = 0;     // index for frame duplication

   //--------------------------------------------------- The BAND LOOP -----------------------------------------------------------//
   for (ucCurBandIdx=0; ucCurBandIdx < gsmV4CalParams.uiNumBands; ++ucCurBandIdx)
   {
      //---------------------------------------- The PCL/Rx CHANNEL LOOP ------------------------------------------------//
      for (ucCurChanIdx=0; ucCurChanIdx < gsmV4CalParams.uiNumChan; ++ucCurChanIdx)
      {
         // Fill DA/Rx sweep
         while (usFrameIdx < usPclSweepFrameMarker)
         {
            //------------------------------ fill the frame header ------------------------------//
            curFrameHeader.band    = static_cast<unsigned char>( gsmV4CalParams.uiBandList[ucCurBandIdx] );
            curFrameHeader.channel = static_cast<unsigned short>( gsmV4CalParams.uiChanList[ucCurBandIdx][ucCurChanIdx] );
            curFrameHeader.numSlotsInPayload  = constNumDaCalSlots;
            // copy the frame header to the tempFramePaylod
            memcpy( tempFramePyld_Ptr, &curFrameHeader, sizeof(GSM_V4_Frame_Header) );
            tempFramePyld_Ptr += sizeof(GSM_V4_Frame_Header);
            tempFramePyld_Size += sizeof(GSM_V4_Frame_Header);

            //------------- loop through slot configuration: m_frameSlotConfig --------------//
            for ( ucFrameSlotIdx=0; ucFrameSlotIdx < constNumSlotsPerFrame; ++ucFrameSlotIdx )
            {
               ucCurTxSlotFunc = gsmV4CalParams.ucTxSlotConfig[ucFrameSlotIdx];
               ucCurRxSlotFunc = gsmV4CalParams.ucRxSlotConfig[ucFrameSlotIdx];

               //--------- TX SLOTS --------//
               if ( ucCurTxSlotFunc > 0 )
               {
                  // ---------- Add chainID[0] slot
                  // fill the slot header
                  curSlotPayload.devID    = cmdHeader.a_devId[0];
                  curSlotPayload.slotNum  = ucFrameSlotIdx;
                  curSlotPayload.slotOpID = Tx_PCL_Meas_Op;
                  // add the modulation
                  curSlotPayload.slotPayload.pclPayload.modType = static_cast<unsigned char>( gsmV4CalParams.gsmV4DAParams.usModIdxList[0] );    // modType[0] - from tree
                  if ( boolDoneWithFirstMod )
                  {
                     curSlotPayload.slotPayload.pclPayload.modType = static_cast<unsigned char>( gsmV4CalParams.gsmV4DAParams.usModIdxList[1] ); // modType[1] - from tree
                  }
                  curSlotPayload.slotPayload.pclPayload.PCL_Index = static_cast<unsigned char>( gsmV4CalParams.gsmV4DAParams.usPclIdxList[ucCurBandIdx][ucCurPclIdx] );
                  // copy the slot payload over
                  memcpy( tempFramePyld_Ptr, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                  tempFramePyld_Ptr += sizeof(GSM_V4_Slot_Payload);
                  tempFramePyld_Size += sizeof(GSM_V4_Slot_Payload);

                  // ---------- Add chainID[1] slot
                  if (gsmV4CalParams.uiNumTxChains > 1)
                  {
                     // fill the slot header
                     curSlotPayload.devID    = cmdHeader.a_devId[1];

                     // copy the slot payload over
                     memcpy( tempFramePyld_Ptr, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                     tempFramePyld_Ptr += sizeof(GSM_V4_Slot_Payload);
                     tempFramePyld_Size += sizeof(GSM_V4_Slot_Payload);
                  }//if (gsmV4CalParams.uiNumTxChains > 1)

                  // increment to the next PCL
                  ++ucCurPclIdx;
               }//if ( ucCurTxSlotFunc > 0 )
               // reset the PCL index and modulation scheme if limit is reached
               if ( ucCurPclIdx >= gsmV4CalParams.gsmV4DAParams.uiNumPCLs)
               {
                  ucCurPclIdx = 0;
                  if ( gsmV4CalParams.gsmV4DAParams.uiNumMods > 1 )
                  {
                     boolDoneWithFirstMod = !boolDoneWithFirstMod;  // toggle the modulation scheme
                  }
               }
            }//for ( ucFrameSlotIdx=0; ucFrameSlotIdx < constNumSlotsPerFrame; ++ucFrameSlotIdx )
            // Copy the tempFramePayload numFrameDup times
            for (frameDupIdx=0; frameDupIdx< usNumFrameDup; ++frameDupIdx)
            {
               memcpy( cptrGSMCmdBuf, tempFramePayload, tempFramePyld_Size );
               cptrGSMCmdBuf += tempFramePyld_Size;
               uiCmdBufSize        += tempFramePyld_Size;
            }
            ++usFrameIdx;
         } //while (usFrameIdx < usPclSweepFrameMarker)
         usFrameIdx = 0;
      }//for (ucCurChanIdx=0; ucCurChanIdx < gsmV4CalParams.uiNumChan; ++ucCurChanIdx)

      if (gsmV4CalParams.gsmV4DPDParams.boolPerformDPD || gsmV4CalParams.gsmV4DPDParams.boolDoRGIRunOnly)
      {
         // reset frameIdx
         usFrameIdx = 0;

         //---------------------------------------- The DPD CHANNEL LOOP ------------------------------------------------//
         for (ucCurChanIdx=0; ucCurChanIdx< gsmV4CalParams.uiNumChan; ++ucCurChanIdx)
         {
            // Fill DPD Sweep
            while (usFrameIdx < usDpdSweepFrameMarker)
            {
               // reset tempFramePayload
               tempFramePyld_Ptr  = tempFramePayload;
               tempFramePyld_Size = 0;
               //------------------------------ fill the frame header ------------------------------//
               curFrameHeader.band    = static_cast<unsigned char>( gsmV4CalParams.uiBandList[ucCurBandIdx] );
               curFrameHeader.channel = static_cast<unsigned short>( gsmV4CalParams.uiChanList[ucCurBandIdx][ucCurChanIdx] );
               curFrameHeader.numSlotsInPayload  = constNumDpdCalSlots;
               // copy the frame header to the GSM V4 CMD payload
               memcpy( cptrGSMCmdBuf, &tempFramePyld_Ptr, sizeof(GSM_V4_Frame_Header) );
               tempFramePyld_Ptr += sizeof(GSM_V4_Frame_Header);
               tempFramePyld_Size += sizeof(GSM_V4_Frame_Header);

               //------------- loop through slot configuration: m_frameSlotConfig --------------//
               for ( ucFrameSlotIdx=0; ucFrameSlotIdx< constNumSlotsPerFrame; ++ucFrameSlotIdx )
               {
                  ucCurTxSlotFunc = gsmV4CalParams.ucTxSlotConfig[ucFrameSlotIdx];
                  ucCurRxSlotFunc = gsmV4CalParams.ucRxSlotConfig[ucFrameSlotIdx];

                  if ( ucCurTxSlotFunc > 0 )
                  {
                     // ---------- Add chainID[0] slot
                     // fill the slot header
                     curSlotPayload.devID                           = cmdHeader.a_devId[0];
                     curSlotPayload.slotNum                         = ucFrameSlotIdx;
                     curSlotPayload.slotOpID                        = Tx_RGI_Meas_Op;
                     // fill the slot payload
                     curSlotPayload.slotPayload.rgiPayload.RGI      = gsmV4CalParams.gsmV4DPDParams.dpdRGIMapVec[gsmV4CalParams.uiBandList[ucCurBandIdx]][ucDpdIdx];
                     curSlotPayload.slotPayload.rgiPayload.envScale = gsmV4CalParams.gsmV4DPDParams.dpdDigGainMapVec[gsmV4CalParams.uiBandList[ucCurBandIdx]][ucDpdIdx];
                     curSlotPayload.slotPayload.rgiPayload.paState  = constDpdPaState;
                     curSlotPayload.slotPayload.rgiPayload.modType  = constDpdModType;

                     // copy the slot payload over to tempFramePyld_Ptr
                     memcpy( tempFramePyld_Ptr, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                     tempFramePyld_Ptr += sizeof(GSM_V4_Slot_Payload);
                     tempFramePyld_Size  += sizeof(GSM_V4_Slot_Payload);

                     // ---------- Add chainID[1] slot
                     if (gsmV4CalParams.uiNumTxChains > 1)
                     {
                        // fill the slot header
                        curSlotPayload.devID   = cmdHeader.a_devId[1];
                        // copy the slot payload over
                        memcpy( cptrGSMCmdBuf, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                        cptrGSMCmdBuf += sizeof(GSM_V4_Slot_Payload);
                        uiCmdBufSize  += sizeof(GSM_V4_Slot_Payload);;
                     }//if (gsmV4CalParams.uiNumTxChains > 1)
                     ++ucDpdIdx;
                     if (ucDpdIdx >= ucDpdSweepSize)
                     {
                        ucDpdIdx = 0;
                     }
                  }//if ( ucCurTxSlotFunc > 0 )
               }//for ( ucFrameSlotIdx=0; ucFrameSlotIdx< constNumSlotsPerFrame; ++ucFrameSlotIdx )
               // Copy the tempFramePayload numFrameDup times
               for (frameDupIdx=0; frameDupIdx< usNumFrameDup; ++frameDupIdx)
               {
                  memcpy( cptrGSMCmdBuf, tempFramePayload, tempFramePyld_Size );
                  cptrGSMCmdBuf += tempFramePyld_Size;
                  uiCmdBufSize    += tempFramePyld_Size;
               }
               ++usFrameIdx;
            }//while (usFrameIdx < usDpdSweepFrameMarker)
            usFrameIdx = 0;
            ucDpdIdx = 0;
         }//for (ucCurChanIdx=0; ucCurChanIdx< gsmV4CalParams.uiNumChan; ++ucCurChanIdx)
      }//if (gsmV4CalParams.gsmV4DPDParams.boolPerformDPD)
      ucDpdIdx = 0;
   }//for (ucCurBandIdx=0; ucCurBandIdx < gsmV4CalParams.uiNumBands; ++ucCurBandIdx)
}
void Create_Config_Command(GSM_V4_Cal_Params &gsmV4CalParams, unsigned char cptrGSMCmdBuf[], unsigned int &uiCmdBufSize,map< unsigned int, vector<unsigned short> >& RX_BURST_FRAME_IDENTIFIER)
{
   GSM_V4_Cal_Config_Header cmdHeader;

   //Header
   cmdHeader.numFrames = static_cast<unsigned short>(gsmV4CalParams.uiNumFrames);
   //cmdHeader.a_devId[0] = 0;
   //cmdHeader.a_devId[1] = 0;
   cmdHeader.numChains = static_cast<unsigned short>(gsmV4CalParams.uiNumTxChains);
   for (unsigned char i=0; i< gsmV4CalParams.uiNumTxChains; ++i)
   {
      cmdHeader.a_devId[i] = static_cast<unsigned char>( gsmV4CalParams.uiTxChainList[i] );  // add device (chain ID) list
   }
   cmdHeader.reserved = 0;
   memcpy( cptrGSMCmdBuf, &cmdHeader, sizeof(GSM_V4_Cal_Config_Header) );
   cptrGSMCmdBuf += sizeof(GSM_V4_Cal_Config_Header);
   uiCmdBufSize += sizeof(GSM_V4_Cal_Config_Header);

   // # of slots used per frame
   const unsigned char  constNumDaCalSlots  = static_cast<unsigned char>( gsmV4CalParams.uiNumRxSlots * gsmV4CalParams.uiNumRxChains + gsmV4CalParams.uiNumTxSlots * gsmV4CalParams.uiNumTxChains );
   unsigned char  numDpdCalSlots = 0;
   if( gsmV4CalParams.gsmV4RxParams.boolPerformRxCal && gsmV4CalParams.gsmV4RxParams.boolPerformHiLinRxCal )
   {
      //Do RX on DPD frames only when high linearity is enabled
      numDpdCalSlots = static_cast<unsigned char>( gsmV4CalParams.uiNumRxSlots * gsmV4CalParams.uiNumRxChains + gsmV4CalParams.uiNumTxSlots * gsmV4CalParams.uiNumTxChains );
   }
   else
   {
      numDpdCalSlots = static_cast<unsigned char>( gsmV4CalParams.uiNumTxSlots * gsmV4CalParams.uiNumTxChains );
   }

   // 8 slots per TDMA frame
   const unsigned char  constNumSlotsPerFrame = GSM_V4_MAX_NUM_OF_SLOTS;
   // # of DPD tables being added per channel
   const unsigned char constNumDpdTblPerChan = static_cast<unsigned char>( gsmV4CalParams.uiNumDPDFramesPerChannel );

   // DPD table settings - these are constant
   const unsigned char constDpdPaState = static_cast<unsigned char>( gsmV4CalParams.gsmV4DPDParams.uiCalPaRange );   // PA range - nominally is PA0, but is user input dependent
   const unsigned char constDpdModType = 1;   // 0 - 8PSK, 1 - GMSK

   unsigned char ucFrameSlotIdx  = 0;   // the frame slot index, 0 to 7
   unsigned char ucCurPclIdx     = 0;   // the current PCL index
   unsigned char ucCurLnaIdx     = 0;   // the current LNA index
   unsigned char ucCurChanIdx    = 0;   // the current channel index
   unsigned char ucCurBandIdx    = 0;   // the current band index
   unsigned char ucDpdIdx        = 0;
   unsigned short usFrameIdx     = 0;   // the current frame index

   unsigned char ucDpdSweepSize = static_cast<unsigned char>(gsmV4CalParams.gsmV4DPDParams.uiNumDpdRgi * gsmV4CalParams.gsmV4DPDParams.uiNumDpdDigGain);

   unsigned short usPclSweepFrameMarker = static_cast<unsigned short>(gsmV4CalParams.uiNumDAFramesPerChannel);
   unsigned short usDpdSweepFrameMarker = static_cast<unsigned short>(gsmV4CalParams.uiNumDPDFramesPerChannel);

   bool boolDoneWithFirstMod = false;   // flag indicating whether modulation 1 or 2 is to be used

   unsigned char ucCurTxSlotFunc = 0;   // the current slot function: Rx/Tx/NoOp
   unsigned char ucCurRxSlotFunc = 0;   // the current slot function: Rx/Tx/NoOp

   GSM_V4_Frame_Header curFrameHeader; // current frame header; values updated for each frame
   GSM_V4_Slot_Payload curSlotPayload; // current slot payload; added to each frame for each slot
   memset( &curFrameHeader, 0, sizeof(GSM_V4_Frame_Header) );
   memset( &curSlotPayload, 0, sizeof(GSM_V4_Slot_Payload) );

   unsigned short rxFrameCntr = 0;  // identifies the frames in which an Rx burst was scheduled; used for result extraction in Process_Rx_Meas_Results

   //--------------------------------------------------- The BAND LOOP -----------------------------------------------------------//
   for (ucCurBandIdx=0; ucCurBandIdx < gsmV4CalParams.uiNumBands; ++ucCurBandIdx)
   {
      //---------------------------------------- The PCL/Rx CHANNEL LOOP ------------------------------------------------//
      for (ucCurChanIdx=0; ucCurChanIdx < gsmV4CalParams.uiNumChan; ++ucCurChanIdx)
      {
         // Fill DA/Rx sweep
         while (usFrameIdx < usPclSweepFrameMarker)
         {
            //------------------------------ fill the frame header ------------------------------//
            curFrameHeader.band    = static_cast<unsigned char>( gsmV4CalParams.uiBandList[ucCurBandIdx] );
            curFrameHeader.channel = static_cast<unsigned short>( gsmV4CalParams.uiChanList[ucCurBandIdx][ucCurChanIdx] );
            curFrameHeader.numSlotsInPayload  = constNumDaCalSlots;
            // copy the frame header to the GSM V4 CMD payload
            memcpy( cptrGSMCmdBuf, &curFrameHeader, sizeof(GSM_V4_Frame_Header) );
            cptrGSMCmdBuf += sizeof(GSM_V4_Frame_Header);
            uiCmdBufSize += sizeof(GSM_V4_Frame_Header);

            //------------- loop through slot configuration: m_frameSlotConfig --------------//
            for ( ucFrameSlotIdx=0; ucFrameSlotIdx < constNumSlotsPerFrame; ++ucFrameSlotIdx )
            {
               ucCurTxSlotFunc = gsmV4CalParams.ucTxSlotConfig[ucFrameSlotIdx];
               ucCurRxSlotFunc = gsmV4CalParams.ucRxSlotConfig[ucFrameSlotIdx];

               //--------- RX SLOTS -------//
               if (ucCurRxSlotFunc > 0 && gsmV4CalParams.gsmV4RxParams.boolPerformRxCal)
               {
                  // ---------- Add chainID[0] slot
                  // fill the slot header
                  curSlotPayload.devID    = cmdHeader.a_devId[0];
                  curSlotPayload.slotNum  = ucFrameSlotIdx;
                  curSlotPayload.slotOpID = Rx_RSSI_Meas_Op;
                  // add the LNA state
                  curSlotPayload.slotPayload.rssiPayload.lnaState = static_cast<unsigned char>( gsmV4CalParams.gsmV4RxParams.lnaStateListAllMode[ucCurLnaIdx] );
                  // copy the slot payload over
                  memcpy( cptrGSMCmdBuf, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                  cptrGSMCmdBuf += sizeof(GSM_V4_Slot_Payload);
                  uiCmdBufSize += sizeof(GSM_V4_Slot_Payload);

                  // ---------- Add chainID[1] slot
                  if (gsmV4CalParams.uiNumRxChains > 1)
                  {
                     // fill the slot header
                     curSlotPayload.devID    = cmdHeader.a_devId[1];
                     // copy the slot payload over
                     memcpy( cptrGSMCmdBuf, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                     cptrGSMCmdBuf += sizeof(GSM_V4_Slot_Payload);
                     uiCmdBufSize += sizeof(GSM_V4_Slot_Payload);
                  }//if (gsmV4CalParams.uiNumRxChains > 1)

                  // Update the RX_BURST_FRAME_IDENTIFIER
                  RX_BURST_FRAME_IDENTIFIER[curFrameHeader.band].push_back(rxFrameCntr);
                  // increment to the next LNA state
                  ++ucCurLnaIdx;
                  // reset the LNA index if limit is reached
                    if ( ucCurLnaIdx >= gsmV4CalParams.gsmV4RxParams.lnaStateListAllMode.size())
                    {
                         ucCurLnaIdx = 0;
                    }
               }//if (ucCurRxSlotFunc > 0 && gsmV4CalParams.gsmV4RxParams.boolPerformRxCal)
               //--------- TX SLOTS --------//
               if ( ucCurTxSlotFunc > 0 )
               {
                  // ---------- Add chainID[0] slot
                  // fill the slot header
                  curSlotPayload.devID    = cmdHeader.a_devId[0];
                  curSlotPayload.slotNum  = ucFrameSlotIdx;
                  curSlotPayload.slotOpID = Tx_PCL_Meas_Op;
                  // add the modulation
                  curSlotPayload.slotPayload.pclPayload.modType = static_cast<unsigned char>( gsmV4CalParams.gsmV4DAParams.usModIdxList[0] );    // modType[0] - from tree
                  if ( boolDoneWithFirstMod )
                  {
                     curSlotPayload.slotPayload.pclPayload.modType = static_cast<unsigned char>( gsmV4CalParams.gsmV4DAParams.usModIdxList[1] ); // modType[1] - from tree
                  }
                  curSlotPayload.slotPayload.pclPayload.PCL_Index = static_cast<unsigned char>( gsmV4CalParams.gsmV4DAParams.usPclIdxList[ucCurBandIdx][ucCurPclIdx] );
                  // copy the slot payload over
                  memcpy( cptrGSMCmdBuf, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                  cptrGSMCmdBuf += sizeof(GSM_V4_Slot_Payload);
                  uiCmdBufSize += sizeof(GSM_V4_Slot_Payload);

                  // ---------- Add chainID[1] slot
                  if (gsmV4CalParams.uiNumTxChains > 1)
                  {
                     // fill the slot header
                     curSlotPayload.devID    = cmdHeader.a_devId[1];

                     // copy the slot payload over
                     memcpy( cptrGSMCmdBuf, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                     cptrGSMCmdBuf += sizeof(GSM_V4_Slot_Payload);
                     uiCmdBufSize += sizeof(GSM_V4_Slot_Payload);
                  }//if (gsmV4CalParams.uiNumTxChains > 1)

                  // increment to the next PCL
                  ++ucCurPclIdx;
               }//if ( ucCurTxSlotFunc > 0 )
               // reset the PCL index and modulation scheme if limit is reached
               if ( ucCurPclIdx >= gsmV4CalParams.gsmV4DAParams.uiNumPCLs)
               {
                  ucCurPclIdx = 0;
                  if ( gsmV4CalParams.gsmV4DAParams.uiNumMods > 1 )
                  {
                     boolDoneWithFirstMod = !boolDoneWithFirstMod;  // toggle the modulation scheme
                  }
               }

            }//for ( ucFrameSlotIdx=0; ucFrameSlotIdx < constNumSlotsPerFrame; ++ucFrameSlotIdx )
            ++usFrameIdx;
            // Increment the rxFrameCntr
            ++rxFrameCntr;
         } //while (usFrameIdx < usPclSweepFrameMarker)
         usFrameIdx = 0;
      }//for (ucCurChanIdx=0; ucCurChanIdx < gsmV4CalParams.uiNumChan; ++ucCurChanIdx)

       /***
      At each channel, RX measurement is accommodated in TX frames (DA and DPD), 1 RX per frame.
      Depending on the number of LNA states, the states may not be evenly accommodated in the DA frames. 
      So in the following DPD section, accommodation starts at the states left over from DA section. 
      */
      unsigned char lnaIdxToStartWith = ucCurLnaIdx;

      if (gsmV4CalParams.gsmV4DPDParams.boolPerformDPD || gsmV4CalParams.gsmV4DPDParams.boolDoRGIRunOnly)
      {
         // reset frameIdx
         usFrameIdx = 0;

         //---------------------------------------- The DPD CHANNEL LOOP ------------------------------------------------//
         for (ucCurChanIdx=0; ucCurChanIdx< gsmV4CalParams.uiNumChan; ++ucCurChanIdx)
         {
            // Fill DPD Sweep
            while (usFrameIdx < usDpdSweepFrameMarker)
            {
               //------------------------------ fill the frame header ------------------------------//
               curFrameHeader.band    = static_cast<unsigned char>( gsmV4CalParams.uiBandList[ucCurBandIdx] );
               curFrameHeader.channel = static_cast<unsigned short>( gsmV4CalParams.uiChanList[ucCurBandIdx][ucCurChanIdx] );
               curFrameHeader.numSlotsInPayload  = numDpdCalSlots;
               // copy the frame header to the GSM V4 CMD payload
               memcpy( cptrGSMCmdBuf, &curFrameHeader, sizeof(GSM_V4_Frame_Header) );
               cptrGSMCmdBuf += sizeof(GSM_V4_Frame_Header);
               uiCmdBufSize += sizeof(GSM_V4_Frame_Header);

               //------------- loop through slot configuration: m_frameSlotConfig --------------//
               for ( ucFrameSlotIdx=0; ucFrameSlotIdx< constNumSlotsPerFrame; ++ucFrameSlotIdx )
               {
                  ucCurTxSlotFunc = gsmV4CalParams.ucTxSlotConfig[ucFrameSlotIdx];
                  ucCurRxSlotFunc = gsmV4CalParams.ucRxSlotConfig[ucFrameSlotIdx];
                  //--------- RX SLOTS -------//
                   if (ucCurRxSlotFunc > 0 && gsmV4CalParams.gsmV4RxParams.boolPerformRxCal && gsmV4CalParams.gsmV4RxParams.boolPerformHiLinRxCal)
                   {
                      // ---------- Add chainID[0] slot
                      // fill the slot header
                      curSlotPayload.devID    = cmdHeader.a_devId[0];
                      curSlotPayload.slotNum  = ucFrameSlotIdx;
                      curSlotPayload.slotOpID = Rx_RSSI_Meas_Op;
                      // add the LNA state
                      curSlotPayload.slotPayload.rssiPayload.lnaState = static_cast<unsigned char>( gsmV4CalParams.gsmV4RxParams.lnaStateListAllMode[ucCurLnaIdx] );
                      // copy the slot payload over
                      memcpy( cptrGSMCmdBuf, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                      cptrGSMCmdBuf += sizeof(GSM_V4_Slot_Payload);
                      uiCmdBufSize += sizeof(GSM_V4_Slot_Payload);

                      // ---------- Add chainID[1] slot
                      if (gsmV4CalParams.uiNumRxChains > 1)
                      {
                         // fill the slot header
                         curSlotPayload.devID    = cmdHeader.a_devId[1];
                         // copy the slot payload over
                         memcpy( cptrGSMCmdBuf, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                         cptrGSMCmdBuf += sizeof(GSM_V4_Slot_Payload);
                         uiCmdBufSize += sizeof(GSM_V4_Slot_Payload);
                      }//if (gsmV4CalParams.uiNumRxChains > 1)

                      // Update the RX_BURST_FRAME_IDENTIFIER
                      RX_BURST_FRAME_IDENTIFIER[curFrameHeader.band].push_back(rxFrameCntr);
                      // increment to the next LNA state
                      ++ucCurLnaIdx;
                      // reset the LNA index if limit is reached
                        if ( ucCurLnaIdx >= gsmV4CalParams.gsmV4RxParams.lnaStateListAllMode.size())
                        {
                             ucCurLnaIdx = 0;
                        }
                   }//if (ucCurRxSlotFunc > 0 && gsmV4CalParams.gsmV4RxParams.boolPerformRxCal)

                  if ( ucCurTxSlotFunc > 0 )
                  {
                     // ---------- Add chainID[0] slot
                     // fill the slot header
                     curSlotPayload.devID                           = cmdHeader.a_devId[0];
                     curSlotPayload.slotNum                         = ucFrameSlotIdx;
                     curSlotPayload.slotOpID                        = Tx_RGI_Meas_Op;
                     // fill the slot payload
                     curSlotPayload.slotPayload.rgiPayload.RGI      = gsmV4CalParams.gsmV4DPDParams.dpdRGIMapVec[gsmV4CalParams.uiBandList[ucCurBandIdx]][ucDpdIdx];
                     curSlotPayload.slotPayload.rgiPayload.envScale = gsmV4CalParams.gsmV4DPDParams.dpdDigGainMapVec[gsmV4CalParams.uiBandList[ucCurBandIdx]][ucDpdIdx];
                     curSlotPayload.slotPayload.rgiPayload.paState  = constDpdPaState;
                     curSlotPayload.slotPayload.rgiPayload.modType  = constDpdModType;

                     // copy the slot payload over
                     memcpy( cptrGSMCmdBuf, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                     cptrGSMCmdBuf += sizeof(GSM_V4_Slot_Payload);
                     uiCmdBufSize  += sizeof(GSM_V4_Slot_Payload);

                     // ---------- Add chainID[1] slot
                     if (gsmV4CalParams.uiNumTxChains > 1)
                     {
                        // fill the slot header
                        curSlotPayload.devID   = cmdHeader.a_devId[1];
                        // copy the slot payload over
                        memcpy( cptrGSMCmdBuf, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                        cptrGSMCmdBuf += sizeof(GSM_V4_Slot_Payload);
                        uiCmdBufSize  += sizeof(GSM_V4_Slot_Payload);;
                     }//if (gsmV4CalParams.uiNumTxChains > 1)

                     ++ucDpdIdx;
                     if (ucDpdIdx >= ucDpdSweepSize)
                     {
                        ucDpdIdx = 0;
                     }
                  }//if ( ucCurTxSlotFunc > 0 )
               }//for ( ucFrameSlotIdx=0; ucFrameSlotIdx< constNumSlotsPerFrame; ++ucFrameSlotIdx )
               ++usFrameIdx;
               // Increment the rxFrameCntr
               ++rxFrameCntr;
            }//while (usFrameIdx < usDpdSweepFrameMarker)
            usFrameIdx = 0;
            ucDpdIdx = 0;
         }//for (ucCurChanIdx=0; ucCurChanIdx< gsmV4CalParams.uiNumChan; ++ucCurChanIdx)
      }//if (gsmV4CalParams.gsmV4DPDParams.boolPerformDPD)
      ucDpdIdx = 0;
   }//for (ucCurBandIdx=0; ucCurBandIdx < gsmV4CalParams.uiNumBands; ++ucCurBandIdx)
}

void Create_Config_Command_RxOnly(GSM_V4_Cal_Params &gsmV4CalParams, unsigned char cptrGSMCmdBuf[], unsigned int &uiCmdBufSize, map< unsigned int, vector<unsigned short> >& RX_BURST_FRAME_IDENTIFIER)
{
   GSM_V4_Cal_Config_Header cmdHeader;

   //Header
   cmdHeader.numFrames = static_cast<unsigned short>(gsmV4CalParams.uiNumFrames);
   //cmdHeader.a_devId[0] = 0;
   //cmdHeader.a_devId[1] = 0;
   cmdHeader.numChains = static_cast<unsigned short>(gsmV4CalParams.uiNumRxChains);
   for (unsigned char i=0; i< gsmV4CalParams.uiNumRxChains; ++i)
   {
      cmdHeader.a_devId[i] = static_cast<unsigned char>( gsmV4CalParams.uiRxChainList[i] );  // add device (chain ID) list
   }
   cmdHeader.reserved = 0;
   memcpy( cptrGSMCmdBuf, &cmdHeader, sizeof(GSM_V4_Cal_Config_Header) );
   cptrGSMCmdBuf += sizeof(GSM_V4_Cal_Config_Header);
   uiCmdBufSize += sizeof(GSM_V4_Cal_Config_Header);

   // # of slots used per frame
   const unsigned char  constNumSlots  = static_cast<unsigned char>( gsmV4CalParams.uiNumRxSlots * gsmV4CalParams.uiNumRxChains);
   const unsigned short constNumFramesPerChannel = static_cast<unsigned short>( gsmV4CalParams.uiNumFramesPerChannel );
   // 8 slots per TDMA frame
   const unsigned char  constNumSlotsPerFrame = GSM_V4_MAX_NUM_OF_SLOTS;

   unsigned char ucFrameSlotIdx  = 0;   // the frame slot index, 0 to 7
   unsigned char ucCurPclIdx     = 0;   // the current PCL index
   unsigned char ucCurLnaIdx     = 0;   // the current LNA index
   unsigned char ucCurChanIdx    = 0;   // the current channel index
   unsigned char ucCurBandIdx    = 0;   // the current band index
   unsigned char ucDpdIdx        = 0;
   unsigned short usFrameIdx     = 0;   // the current frame index

   // uplink trigger segment parameters
   const unsigned char constTxTriggerSlotIdx = 0;   // slot index of the Uplink trigger segment
   const unsigned char constTxTriggerPclIdx  = 15;  // PCL index power for the uplink trigger segment - max power

   unsigned char ucInitFrameSlotIdx = 0;   // the initial frame slot index

   unsigned char ucCurRxSlotFunc = 0;   // the current slot function: Rx/Tx/NoOp

   GSM_V4_Frame_Header curFrameHeader; // current frame header; values updated for each frame
   GSM_V4_Slot_Payload curSlotPayload; // current slot payload; added to each frame for each slot
   memset( &curFrameHeader, 0, sizeof(GSM_V4_Frame_Header) );
   memset( &curSlotPayload, 0, sizeof(GSM_V4_Slot_Payload) );

   //--------------------------------------------------- The BAND LOOP -----------------------------------------------------------//
   for (usFrameIdx=0; usFrameIdx < gsmV4CalParams.uiNumFrames; ++usFrameIdx)
   {
      //------------------------------ fill the frame header ------------------------------//
      curFrameHeader.band    = static_cast<unsigned char>( gsmV4CalParams.uiBandList[ucCurBandIdx] );
      curFrameHeader.channel = static_cast<unsigned short>( gsmV4CalParams.uiChanList[ucCurBandIdx][ucCurChanIdx] );
      curFrameHeader.numSlotsInPayload  = constNumSlots;
      if (usFrameIdx == 0)
      {
         curFrameHeader.numSlotsInPayload  = constNumSlots + 1;
      }
      // copy the frame header to the GSM V4 CMD payload
      memcpy( cptrGSMCmdBuf, &curFrameHeader, sizeof(GSM_V4_Frame_Header) );
      cptrGSMCmdBuf += sizeof(GSM_V4_Frame_Header);
      uiCmdBufSize += sizeof(GSM_V4_Frame_Header);

      // add 1 Tx slot for CB trigger
      if (usFrameIdx == 0)
      {
         // fill the slot header
         curSlotPayload.devID    = cmdHeader.a_devId[0];
         curSlotPayload.slotNum  = gsmV4_TriggerSlotIdx;
         curSlotPayload.slotOpID = Tx_RGI_Meas_Op;
         // add slot specifics
         curSlotPayload.slotPayload.rgiPayload.RGI      = gsmV4_TriggerRGI;
         curSlotPayload.slotPayload.rgiPayload.envScale = gsmV4_TriggerDigGain;
         curSlotPayload.slotPayload.rgiPayload.paState  = gsmV4_TriggerPA;
         curSlotPayload.slotPayload.rgiPayload.modType  = gsmV4_Desired_DPD_Mod_Type;
#if 0
         curSlotPayload.slotNum  = constTxTriggerSlotIdx;
         curSlotPayload.slotOpID = Tx_PCL_Meas_Op;
         // add the modulation
         curSlotPayload.slotPayload.pclPayload.modType   = gsmV4_GMSK_MCS_Idx;
         curSlotPayload.slotPayload.pclPayload.PCL_Index = constTxTriggerPclIdx;
#endif
         // copy the slot payload over
         memcpy( cptrGSMCmdBuf, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
         cptrGSMCmdBuf += sizeof(GSM_V4_Slot_Payload);
         uiCmdBufSize        += sizeof(GSM_V4_Slot_Payload);
      }

      //------------- loop through slot configuration: m_frameSlotConfig --------------//
      for ( ucFrameSlotIdx=0; ucFrameSlotIdx < constNumSlotsPerFrame; ++ucFrameSlotIdx )
      {
         ucCurRxSlotFunc = gsmV4CalParams.ucRxSlotConfig[ucFrameSlotIdx];

         //--------- RX SLOTS -------//
         if (ucCurRxSlotFunc > 0l)
         {
            // ---------- Add chainID[0] slot
            // fill the slot header
            curSlotPayload.devID    = cmdHeader.a_devId[0];
            curSlotPayload.slotNum  = ucFrameSlotIdx;
            curSlotPayload.slotOpID = Rx_RSSI_Meas_Op;
            // add the LNA state
            curSlotPayload.slotPayload.rssiPayload.lnaState = static_cast<unsigned char>( gsmV4CalParams.gsmV4RxParams.lnaStateListAllMode[ucCurLnaIdx] );
            // copy the slot payload over
            memcpy( cptrGSMCmdBuf, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
            cptrGSMCmdBuf += sizeof(GSM_V4_Slot_Payload);
            uiCmdBufSize += sizeof(GSM_V4_Slot_Payload);

            // ---------- Add chainID[1] slot
            if (gsmV4CalParams.uiNumRxChains > 1)
            {
               // fill the slot header
               curSlotPayload.devID    = cmdHeader.a_devId[1];
               // copy the slot payload over
               memcpy( cptrGSMCmdBuf, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
               cptrGSMCmdBuf += sizeof(GSM_V4_Slot_Payload);
               uiCmdBufSize += sizeof(GSM_V4_Slot_Payload);
            }//if (gsmV4CalParams.uiNumRxChains > 1)

            // increment to the next LNA state
            ++ucCurLnaIdx;
         }//if (ucCurRxSlotFunc > 0 )
         // reset the LNA index if limit is reached
         if ( ucCurLnaIdx >= gsmV4CalParams.gsmV4RxParams.lnaStateListAllMode.size())
         {
            ucCurLnaIdx = 0;
         }
      }//for ( ucFrameSlotIdx=0; ucFrameSlotIdx < constNumSlotsPerFrame; ++ucFrameSlotIdx )
      // update the current channel
      if ( ( (usFrameIdx + 1) % constNumFramesPerChannel ) == 0 )
      {
         ++ucCurChanIdx;
         if ( ucCurChanIdx >= gsmV4CalParams.uiNumChan ) // indicates the number of channels per band has been exceeded
         {
            ucCurChanIdx = 0;   // reset the channel index
            ++ucCurBandIdx;     // increment to the next band index
         }
      }
      // Update the RX_BURST_FRAME_IDENTIFIER
      RX_BURST_FRAME_IDENTIFIER[curFrameHeader.band].push_back(usFrameIdx);
   }//for (usFrameIdx=0; usFrameIdx < gsmV4CalParams.uiNumFrames; ++usFrameIdx)
}

/*Get Tx measurements from phone*/
void Get_Rx_Results(GSM_V4_FTM_CMD_Response_Type &gsmV4_measResults, GSM_V4_Cal_Params &gsmV4CalParams)
{
#ifdef ON_TARGET
   QLIB_FTM_SET_MODE_ID( g_hResourceContext, static_cast<short>(FTM_MODE_ID_GSM_EXTENDED_C) );
   QLIB_FTM_GSM_V4_CAL_GET_RESULTS(g_hResourceContext, (void *)&gsmV4_measResults);
#else

   /*The values here are hard coded as if the measurements are read form the phone*/
   gsmV4_measResults.numPayload = gsmV4CalParams.uiNumFrames; //36 = 3 channels * (4 gmsk frames + 4 8psk frames + dpdframes);
   /*only first 24 Rx entries are valide, Rx meas during DPD should be eliminated because RSSI is completely not correct. The elimination be taken care in the code
   using std deviation check*/

   int cnt;
   for(cnt = 0; cnt <gsmV4_measResults.numPayload; cnt++)
   {
      gsmV4_measResults.payload[cnt].frameID = cnt;
      gsmV4_measResults.payload[cnt].devID = 0;
      gsmV4_measResults.payload[cnt].slotNum = 0;
      gsmV4_measResults.payload[cnt].numResults = 1;
      gsmV4_measResults.payload[cnt].result[0].opID = Rx_RSSI_Meas_Op;
   }

   unsigned int uiChannelCnt;
   unsigned int uiLNAIdx;
   unsigned int uiBandCnt;
   unsigned int uiDAFrameIdx;
   unsigned int uiDPDFrameIdx;

   cnt = 0;
   for(uiBandCnt = 0; uiBandCnt < gsmV4CalParams.uiNumBands; uiBandCnt++)
   {
      for(uiChannelCnt = 0; uiChannelCnt < gsmV4CalParams.uiNumChan; uiChannelCnt++)
      {
         uiLNAIdx = 0;
         for(uiDAFrameIdx = 0; uiDAFrameIdx < gsmV4CalParams.uiNumDAFramesPerChannel; uiDAFrameIdx++)
         {
            gsmV4_measResults.payload[cnt].result[0].opResult.rssiResult.channel = gsmV4CalParams.uiChanList[uiBandCnt][uiChannelCnt];
            gsmV4_measResults.payload[cnt].result[0].opResult.rssiResult.lnaState = gsmV4CalParams.gsmV4RxParams.uiLNAStateList[uiLNAIdx];

            if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
            {
                gsmV4_measResults.payload[cnt].result[1].opResult.rssiResult.channel = gsmV4CalParams.uiChanList[uiBandCnt][uiChannelCnt];
                gsmV4_measResults.payload[cnt].result[1].opResult.rssiResult.lnaState = gsmV4CalParams.gsmV4RxParams.uiLNAStateList[uiLNAIdx];
            }
            uiLNAIdx++;

            if(uiLNAIdx >= gsmV4CalParams.gsmV4RxParams.uiNumLNAState)
            {
               uiLNAIdx = 0;
            }
            cnt++;
         }
      }
      //There is an extra set of readings with LNA state 3 for 3 channels..coming from DPD.
      for(uiChannelCnt = 0; uiChannelCnt < gsmV4CalParams.uiNumChan; uiChannelCnt++)
      {
         for(uiDPDFrameIdx = 0; uiDPDFrameIdx < gsmV4CalParams.uiNumDPDFramesPerChannel; uiDPDFrameIdx++)
         {
            gsmV4_measResults.payload[cnt].result[0].opResult.rssiResult.channel = gsmV4CalParams.uiChanList[uiBandCnt][uiChannelCnt];
            gsmV4_measResults.payload[cnt].result[0].opResult.rssiResult.lnaState = gsmV4CalParams.gsmV4RxParams.uiLNAStateList[3];
            if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
            {
                gsmV4_measResults.payload[cnt].result[0].opResult.rssiResult.channel = gsmV4CalParams.uiChanList[uiBandCnt][uiChannelCnt];
                gsmV4_measResults.payload[cnt].result[0].opResult.rssiResult.lnaState = gsmV4CalParams.gsmV4RxParams.uiLNAStateList[3];
            }
            cnt++;
         }
      }
   }
   //currently this data is for 1 band, 3 channels only (This example data does not have High Linearity measurements)

   /*************************channel 0*************************/
   /************during DA or Rx Only*************/
   gsmV4_measResults.payload[0].result[0].opResult.rssiResult.rssiMeasResult = 191799;
   gsmV4_measResults.payload[0].result[0].opResult.rssiResult.rxPwr_dBm16 = -1448;

   gsmV4_measResults.payload[1].result[0].opResult.rssiResult.rssiMeasResult = 11441;
   gsmV4_measResults.payload[1].result[0].opResult.rssiResult.rxPwr_dBm16 = -1452;

   gsmV4_measResults.payload[2].result[0].opResult.rssiResult.rssiMeasResult = 48332768;
   gsmV4_measResults.payload[2].result[0].opResult.rssiResult.rxPwr_dBm16 = -873;

   gsmV4_measResults.payload[3].result[0].opResult.rssiResult.rssiMeasResult = 150385;
   gsmV4_measResults.payload[3].result[0].opResult.rssiResult.rxPwr_dBm16 = -874;

   gsmV4_measResults.payload[4].result[0].opResult.rssiResult.rssiMeasResult = 191251;
   gsmV4_measResults.payload[4].result[0].opResult.rssiResult.rxPwr_dBm16 = -1448;

   gsmV4_measResults.payload[5].result[0].opResult.rssiResult.rssiMeasResult = 11993;
   gsmV4_measResults.payload[5].result[0].opResult.rssiResult.rxPwr_dBm16 = -1449;

   gsmV4_measResults.payload[6].result[0].opResult.rssiResult.rssiMeasResult = 48794351;
   gsmV4_measResults.payload[6].result[0].opResult.rssiResult.rxPwr_dBm16 = -872;

   gsmV4_measResults.payload[7].result[0].opResult.rssiResult.rssiMeasResult = 142783;
   gsmV4_measResults.payload[7].result[0].opResult.rssiResult.rxPwr_dBm16 = -877;

   /*************************channel 1*************************/
   /************during DA or Rx Only*************/
   gsmV4_measResults.payload[8].result[0].opResult.rssiResult.rssiMeasResult = 207935;
   gsmV4_measResults.payload[8].result[0].opResult.rssiResult.rxPwr_dBm16 = -1449;

   gsmV4_measResults.payload[9].result[0].opResult.rssiResult.rssiMeasResult = 13138;
   gsmV4_measResults.payload[9].result[0].opResult.rssiResult.rxPwr_dBm16 = -1452;

   gsmV4_measResults.payload[10].result[0].opResult.rssiResult.rssiMeasResult = 52362628;
   gsmV4_measResults.payload[10].result[0].opResult.rssiResult.rxPwr_dBm16 = -875;

   gsmV4_measResults.payload[11].result[0].opResult.rssiResult.rssiMeasResult = 163467;
   gsmV4_measResults.payload[11].result[0].opResult.rssiResult.rxPwr_dBm16 = -875;

   gsmV4_measResults.payload[12].result[0].opResult.rssiResult.rssiMeasResult = 208614;
   gsmV4_measResults.payload[12].result[0].opResult.rssiResult.rxPwr_dBm16 = -1449;

   gsmV4_measResults.payload[13].result[0].opResult.rssiResult.rssiMeasResult = 12272;
   gsmV4_measResults.payload[13].result[0].opResult.rssiResult.rxPwr_dBm16 = -1456;

   gsmV4_measResults.payload[14].result[0].opResult.rssiResult.rssiMeasResult = 52224444;
   gsmV4_measResults.payload[14].result[0].opResult.rssiResult.rxPwr_dBm16 = -875;

   gsmV4_measResults.payload[15].result[0].opResult.rssiResult.rssiMeasResult = 161949;
   gsmV4_measResults.payload[15].result[0].opResult.rssiResult.rxPwr_dBm16 = -876;

   /*************************channel 2*************************/
   /************during DA or Rx Only*************/
   gsmV4_measResults.payload[16].result[0].opResult.rssiResult.rssiMeasResult = 158875;
   gsmV4_measResults.payload[16].result[0].opResult.rssiResult.rxPwr_dBm16 = -1448;

   gsmV4_measResults.payload[17].result[0].opResult.rssiResult.rssiMeasResult = 10073;
   gsmV4_measResults.payload[17].result[0].opResult.rssiResult.rxPwr_dBm16 = -1450;

   gsmV4_measResults.payload[18].result[0].opResult.rssiResult.rssiMeasResult = 40610556;
   gsmV4_measResults.payload[18].result[0].opResult.rssiResult.rxPwr_dBm16 = -872;

   gsmV4_measResults.payload[19].result[0].opResult.rssiResult.rssiMeasResult = 131674;
   gsmV4_measResults.payload[19].result[0].opResult.rssiResult.rxPwr_dBm16 = -879;

   gsmV4_measResults.payload[20].result[0].opResult.rssiResult.rssiMeasResult = 158216;
   gsmV4_measResults.payload[20].result[0].opResult.rssiResult.rxPwr_dBm16 = -1448;

   gsmV4_measResults.payload[21].result[0].opResult.rssiResult.rssiMeasResult = 10048;
   gsmV4_measResults.payload[21].result[0].opResult.rssiResult.rxPwr_dBm16 = -1450;

   gsmV4_measResults.payload[22].result[0].opResult.rssiResult.rssiMeasResult = 40259028;
   gsmV4_measResults.payload[22].result[0].opResult.rssiResult.rxPwr_dBm16 = -873;

   gsmV4_measResults.payload[23].result[0].opResult.rssiResult.rssiMeasResult = 141654;
   gsmV4_measResults.payload[23].result[0].opResult.rssiResult.rxPwr_dBm16 = -873;

   /*Values from DPD sweep are invalid since there is no Rx meas scheduled*/
   /*************************channel 0*************************/
   /************during DPD*************/
   gsmV4_measResults.payload[24].result[0].opResult.rssiResult.rssiMeasResult = 20;
   gsmV4_measResults.payload[24].result[0].opResult.rssiResult.rxPwr_dBm16 = -1490;

   gsmV4_measResults.payload[25].result[0].opResult.rssiResult.rssiMeasResult = 20;
   gsmV4_measResults.payload[25].result[0].opResult.rssiResult.rxPwr_dBm16 = -1490;

   gsmV4_measResults.payload[26].result[0].opResult.rssiResult.rssiMeasResult = 18;
   gsmV4_measResults.payload[26].result[0].opResult.rssiResult.rxPwr_dBm16 = -1497;

   gsmV4_measResults.payload[27].result[0].opResult.rssiResult.rssiMeasResult = 19;
   gsmV4_measResults.payload[27].result[0].opResult.rssiResult.rxPwr_dBm16 = -1494;

   gsmV4_measResults.payload[28].result[0].opResult.rssiResult.rssiMeasResult = 20;
   gsmV4_measResults.payload[28].result[0].opResult.rssiResult.rxPwr_dBm16 = -1490;

   /*************************channel 1*************************/
   /************during DPD*************/
   gsmV4_measResults.payload[29].result[0].opResult.rssiResult.rssiMeasResult = 22;
   gsmV4_measResults.payload[29].result[0].opResult.rssiResult.rxPwr_dBm16 = -1483;

   gsmV4_measResults.payload[30].result[0].opResult.rssiResult.rssiMeasResult = 19;
   gsmV4_measResults.payload[30].result[0].opResult.rssiResult.rxPwr_dBm16 = -1494;

   gsmV4_measResults.payload[31].result[0].opResult.rssiResult.rssiMeasResult = 25;
   gsmV4_measResults.payload[31].result[0].opResult.rssiResult.rxPwr_dBm16 = -1472;

   gsmV4_measResults.payload[32].result[0].opResult.rssiResult.rssiMeasResult = 18;
   gsmV4_measResults.payload[32].result[0].opResult.rssiResult.rxPwr_dBm16 = -1497;

   gsmV4_measResults.payload[33].result[0].opResult.rssiResult.rssiMeasResult = 19;
   gsmV4_measResults.payload[33].result[0].opResult.rssiResult.rxPwr_dBm16 = -1494;
   /*************************channel 2*************************/
   /************during DPD*************/
   gsmV4_measResults.payload[34].result[0].opResult.rssiResult.rssiMeasResult = 20;
   gsmV4_measResults.payload[34].result[0].opResult.rssiResult.rxPwr_dBm16 = -1490;

   gsmV4_measResults.payload[35].result[0].opResult.rssiResult.rssiMeasResult = 24;
   gsmV4_measResults.payload[35].result[0].opResult.rssiResult.rxPwr_dBm16 = -1476;

   gsmV4_measResults.payload[36].result[0].opResult.rssiResult.rssiMeasResult = 19;
   gsmV4_measResults.payload[36].result[0].opResult.rssiResult.rxPwr_dBm16 = -1494;

   gsmV4_measResults.payload[37].result[0].opResult.rssiResult.rssiMeasResult = 21;
   gsmV4_measResults.payload[37].result[0].opResult.rssiResult.rxPwr_dBm16 = -1487;

   gsmV4_measResults.payload[38].result[0].opResult.rssiResult.rssiMeasResult = 24;
   gsmV4_measResults.payload[38].result[0].opResult.rssiResult.rxPwr_dBm16 = -1476;

   /*Using the same data for diversity just for example purpose*/
   if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
   {
       gsmV4_measResults.payload[0].result[1].opResult.rssiResult.rssiMeasResult = 191799;
       gsmV4_measResults.payload[0].result[1].opResult.rssiResult.rxPwr_dBm16 = -1448;

       gsmV4_measResults.payload[1].result[1].opResult.rssiResult.rssiMeasResult = 11441;
       gsmV4_measResults.payload[1].result[1].opResult.rssiResult.rxPwr_dBm16 = -1452;

       gsmV4_measResults.payload[2].result[1].opResult.rssiResult.rssiMeasResult = 48332768;
       gsmV4_measResults.payload[2].result[1].opResult.rssiResult.rxPwr_dBm16 = -873;

       gsmV4_measResults.payload[3].result[1].opResult.rssiResult.rssiMeasResult = 150385;
       gsmV4_measResults.payload[3].result[1].opResult.rssiResult.rxPwr_dBm16 = -874;

       gsmV4_measResults.payload[4].result[1].opResult.rssiResult.rssiMeasResult = 191251;
       gsmV4_measResults.payload[4].result[1].opResult.rssiResult.rxPwr_dBm16 = -1448;

       gsmV4_measResults.payload[5].result[1].opResult.rssiResult.rssiMeasResult = 11993;
       gsmV4_measResults.payload[5].result[1].opResult.rssiResult.rxPwr_dBm16 = -1449;

       gsmV4_measResults.payload[6].result[1].opResult.rssiResult.rssiMeasResult = 48794351;
       gsmV4_measResults.payload[6].result[1].opResult.rssiResult.rxPwr_dBm16 = -872;

       gsmV4_measResults.payload[7].result[1].opResult.rssiResult.rssiMeasResult = 142783;
       gsmV4_measResults.payload[7].result[1].opResult.rssiResult.rxPwr_dBm16 = -877;

       /*************************channel 1*************************/
       /************during DA or Rx Only*************/
       gsmV4_measResults.payload[8].result[1].opResult.rssiResult.rssiMeasResult = 207935;
       gsmV4_measResults.payload[8].result[1].opResult.rssiResult.rxPwr_dBm16 = -1449;

       gsmV4_measResults.payload[9].result[1].opResult.rssiResult.rssiMeasResult = 13138;
       gsmV4_measResults.payload[9].result[1].opResult.rssiResult.rxPwr_dBm16 = -1452;

       gsmV4_measResults.payload[10].result[1].opResult.rssiResult.rssiMeasResult = 52362628;
       gsmV4_measResults.payload[10].result[1].opResult.rssiResult.rxPwr_dBm16 = -875;

       gsmV4_measResults.payload[11].result[1].opResult.rssiResult.rssiMeasResult = 163467;
       gsmV4_measResults.payload[11].result[1].opResult.rssiResult.rxPwr_dBm16 = -875;

       gsmV4_measResults.payload[12].result[1].opResult.rssiResult.rssiMeasResult = 208614;
       gsmV4_measResults.payload[12].result[1].opResult.rssiResult.rxPwr_dBm16 = -1449;

       gsmV4_measResults.payload[13].result[1].opResult.rssiResult.rssiMeasResult = 12272;
       gsmV4_measResults.payload[13].result[1].opResult.rssiResult.rxPwr_dBm16 = -1456;

       gsmV4_measResults.payload[14].result[1].opResult.rssiResult.rssiMeasResult = 52224444;
       gsmV4_measResults.payload[14].result[1].opResult.rssiResult.rxPwr_dBm16 = -875;

       gsmV4_measResults.payload[15].result[1].opResult.rssiResult.rssiMeasResult = 161949;
       gsmV4_measResults.payload[15].result[1].opResult.rssiResult.rxPwr_dBm16 = -876;

       /*************************channel 2*************************/
       /************during DA or Rx Only*************/
       gsmV4_measResults.payload[16].result[1].opResult.rssiResult.rssiMeasResult = 158875;
       gsmV4_measResults.payload[16].result[1].opResult.rssiResult.rxPwr_dBm16 = -1448;

       gsmV4_measResults.payload[17].result[1].opResult.rssiResult.rssiMeasResult = 10073;
       gsmV4_measResults.payload[17].result[1].opResult.rssiResult.rxPwr_dBm16 = -1450;

       gsmV4_measResults.payload[18].result[1].opResult.rssiResult.rssiMeasResult = 40610556;
       gsmV4_measResults.payload[18].result[1].opResult.rssiResult.rxPwr_dBm16 = -872;

       gsmV4_measResults.payload[19].result[1].opResult.rssiResult.rssiMeasResult = 131674;
       gsmV4_measResults.payload[19].result[1].opResult.rssiResult.rxPwr_dBm16 = -879;

       gsmV4_measResults.payload[20].result[1].opResult.rssiResult.rssiMeasResult = 158216;
       gsmV4_measResults.payload[20].result[1].opResult.rssiResult.rxPwr_dBm16 = -1448;

       gsmV4_measResults.payload[21].result[1].opResult.rssiResult.rssiMeasResult = 10048;
       gsmV4_measResults.payload[21].result[1].opResult.rssiResult.rxPwr_dBm16 = -1450;

       gsmV4_measResults.payload[22].result[1].opResult.rssiResult.rssiMeasResult = 40259028;
       gsmV4_measResults.payload[22].result[1].opResult.rssiResult.rxPwr_dBm16 = -873;

       gsmV4_measResults.payload[23].result[1].opResult.rssiResult.rssiMeasResult = 141654;
       gsmV4_measResults.payload[23].result[1].opResult.rssiResult.rxPwr_dBm16 = -873;

       /*Values from DPD sweep are invalid since there is no Rx meas scheduled*/
       /*************************channel 0*************************/
       /************during DPD*************/
       gsmV4_measResults.payload[24].result[1].opResult.rssiResult.rssiMeasResult = 20;
       gsmV4_measResults.payload[24].result[1].opResult.rssiResult.rxPwr_dBm16 = -1490;

       gsmV4_measResults.payload[25].result[1].opResult.rssiResult.rssiMeasResult = 20;
       gsmV4_measResults.payload[25].result[1].opResult.rssiResult.rxPwr_dBm16 = -1490;

       gsmV4_measResults.payload[26].result[1].opResult.rssiResult.rssiMeasResult = 18;
       gsmV4_measResults.payload[26].result[1].opResult.rssiResult.rxPwr_dBm16 = -1497;

       gsmV4_measResults.payload[27].result[1].opResult.rssiResult.rssiMeasResult = 19;
       gsmV4_measResults.payload[27].result[1].opResult.rssiResult.rxPwr_dBm16 = -1494;

       gsmV4_measResults.payload[28].result[1].opResult.rssiResult.rssiMeasResult = 20;
       gsmV4_measResults.payload[28].result[1].opResult.rssiResult.rxPwr_dBm16 = -1490;

       /*************************channel 1*************************/
       /************during DPD*************/
       gsmV4_measResults.payload[29].result[1].opResult.rssiResult.rssiMeasResult = 22;
       gsmV4_measResults.payload[29].result[1].opResult.rssiResult.rxPwr_dBm16 = -1483;

       gsmV4_measResults.payload[30].result[1].opResult.rssiResult.rssiMeasResult = 19;
       gsmV4_measResults.payload[30].result[1].opResult.rssiResult.rxPwr_dBm16 = -1494;

       gsmV4_measResults.payload[31].result[1].opResult.rssiResult.rssiMeasResult = 25;
       gsmV4_measResults.payload[31].result[1].opResult.rssiResult.rxPwr_dBm16 = -1472;

       gsmV4_measResults.payload[32].result[1].opResult.rssiResult.rssiMeasResult = 18;
       gsmV4_measResults.payload[32].result[1].opResult.rssiResult.rxPwr_dBm16 = -1497;

       gsmV4_measResults.payload[33].result[1].opResult.rssiResult.rssiMeasResult = 19;
       gsmV4_measResults.payload[33].result[1].opResult.rssiResult.rxPwr_dBm16 = -1494;
       /*************************channel 2*************************/
       /************during DPD*************/
       gsmV4_measResults.payload[34].result[1].opResult.rssiResult.rssiMeasResult = 20;
       gsmV4_measResults.payload[34].result[1].opResult.rssiResult.rxPwr_dBm16 = -1490;

       gsmV4_measResults.payload[35].result[1].opResult.rssiResult.rssiMeasResult = 24;
       gsmV4_measResults.payload[35].result[1].opResult.rssiResult.rxPwr_dBm16 = -1476;

       gsmV4_measResults.payload[36].result[1].opResult.rssiResult.rssiMeasResult = 19;
       gsmV4_measResults.payload[36].result[1].opResult.rssiResult.rxPwr_dBm16 = -1494;

       gsmV4_measResults.payload[37].result[1].opResult.rssiResult.rssiMeasResult = 21;
       gsmV4_measResults.payload[37].result[1].opResult.rssiResult.rxPwr_dBm16 = -1487;

       gsmV4_measResults.payload[38].result[1].opResult.rssiResult.rssiMeasResult = 24;
       gsmV4_measResults.payload[38].result[1].opResult.rssiResult.rxPwr_dBm16 = -1476;
   }
#endif
}

void Compute_MeasInfo(GSM_V4_Meas_Info &measInfo, GSM_V4_Cal_Params &gsmV4CalParams)
{
   // compute number of frames must be called prior to this

   // Number of call box measurement steps.
   // There are an additional m_intraBandTxSegDelay steps added for each band change.
   int iNumEquipSteps = gsmV4CalParams.uiNumFrames *GSM_V4_MAX_NUM_OF_SLOTS + constIntraBandSegDelay*(gsmV4CalParams.uiNumFrames - 1);

   measInfo.numContiguousDPD_Meas = static_cast<unsigned short>( gsmV4CalParams.gsmV4DPDParams.uiNumDpdRgi * gsmV4CalParams.gsmV4DPDParams.uiNumDpdDigGain * gsmV4CalParams.uiNumTxChains);
   measInfo.numContiguousDA_Meas  = static_cast<unsigned short>( gsmV4CalParams.gsmV4DAParams.uiNumMods * gsmV4CalParams.gsmV4DAParams.uiNumPCLs * gsmV4CalParams.uiNumTxChains);

   //callbox
   measInfo.numContiguousDPD_Meas_CB = static_cast<unsigned short>( gsmV4CalParams.uiNumDPDFramesPerChannel * gsmV4CalParams.uiNumTxSlots * gsmV4CalParams.uiNumTxChains);
   if (!gsmV4CalParams.gsmV4DAParams.boolPerformDA)
   {
      measInfo.numContiguousDA_Meas = 0;
   }
   if (!gsmV4CalParams.gsmV4DPDParams.boolPerformDPD && !gsmV4CalParams.gsmV4DPDParams.boolDoRGIRunOnly)
   {
      measInfo.numContiguousDPD_Meas = 0;
      //callbox
      measInfo.numContiguousDPD_Meas_CB = 0;
   }

   measInfo.numTotalDPD_Meas = static_cast<unsigned short>( measInfo.numContiguousDPD_Meas * gsmV4CalParams.uiNumChan * gsmV4CalParams.uiNumBands );
   measInfo.numTotalDA_Meas  = static_cast<unsigned short>( measInfo.numContiguousDA_Meas * gsmV4CalParams.uiNumChan * gsmV4CalParams.uiNumBands );

   // total number of expected Tx measurements
   measInfo.numExpTxMeas = static_cast<unsigned short>(measInfo.numTotalDPD_Meas + measInfo.numTotalDA_Meas);

   //callbox
   unsigned short numTotalDPD_Meas_CB = static_cast<unsigned short>( measInfo.numContiguousDPD_Meas_CB * gsmV4CalParams.uiNumChan * gsmV4CalParams.uiNumBands );
   //callbox
   // total number of expected Tx measurements for the Call Box  - we only need this for the call box perspective. It is irrelavant for the data processing perspective.
   measInfo.numExpTxMeas_CB = numTotalDPD_Meas_CB + measInfo.numTotalDA_Meas;

   // total number of expected Rx measurements
   measInfo.numExpRxMeas = 0;
   if ( gsmV4CalParams.gsmV4RxParams.boolPerformRxCal )
   {
      measInfo.numExpRxMeas = static_cast<unsigned short>( gsmV4CalParams.gsmV4RxParams.uiNumLNAState * gsmV4CalParams.gsmV4DAParams.uiNumMods * gsmV4CalParams.uiNumChan * gsmV4CalParams.uiNumBands * gsmV4CalParams.uiNumRxChains );
   }
}
void Segment_DPD_And_DA_Pwr(map<unsigned int, vector<double> >& dDpdPowMap_Chain, map<unsigned int,vector<double> >& dDaPowMap_Chain, GSM_V4_Meas_Info& measInfo, GSM_V4_Cal_Params& gsmV4CalParams)
{
   //TE outputs values in all 8 slots in a frame, but valid data is present based on TxSlotConfig; i.e if TxSlotConfig is { 1,1,1,1,0,0,0,0}
   //then valid data is present only in 1st 4 slots per frame, populate the output vector
   //After getting the output vector which contains
   //band 0 chan 0 DA chan 1 DA chan 2 DA, chan 0 DPD chan 1 DPD chan 2 DPD; band 1....

   /*Values are hardcoded here as if read from the call box for example purpose only*/
   double inputArr[] =
   {
      /*example-1*/
      29.4,29.32,27.85,25.71,26.4761,24.5234,22.805,20.854,18.861,16.828,14.4725,12.672,10.213,8.177,6.173,4.172,\
      29.7,29.5,27.9,27.0,26.4,24.5,22.7,20.677,19.346,16.4636,14.859,12.631,9.996,7.717,6.4073,4.18, \
      29.37,29.3,27.86,25.73,26.5592,24.659,22.799,20.854,18.776,16.71,14.4799,12.674,10.306,8.298,6.4058,4.398, \
      29.6,29.4,27.9,27.1,26.5,24.5,22.7,20.877,18.337,16.757,14.611,12.4281,10.5731,8.357,5.65,4.077, \
      29.35,29.28,28.08,26.08,26.665,24.601,22.692,20.769,18.808,16.781,14.49888,12.662,10.34,8.32,6.4286,4.4164, \
      29.7,29.5,28.2,27.2,26.8,24.7,22.8,21.116,18.839,16.5879,14.5917,12.5889,10.5926,8.261,6.13,4.237, \
      6.301,7.531,8.977,9.528,16.05,17.23,18.44,19.67,24.99,26.07,27.29,28.17,26.61,27.47,28.54,29.43,27.91,28.85,29.88,29.9,\
      6.293,7.38,8.496,10.16,15.19,16.46,18.55,19.55,24.61,26.03,26.69,28.03,26.46,27.35,28.08,28.61,27.85,28.57,29.21,30.07, \
      5.692,6.927,8.385,8.95,15.14,16.32,17.51,18.73,23.99,25.1,26.33,27.21,25.73,26.63,27.73,28.58,27.49,28.44,29.42,29.36
   };

   vector<double> inputPowVec;
   inputPowVec.assign(inputArr,inputArr + sizeof(inputArr)/sizeof(double));
   vector<double> daPowVec(measInfo.numTotalDA_Meas);
   vector<double> dpdPowVec(measInfo.numTotalDPD_Meas);

   // The sweep consists of doing all DA measurements per band, followed by all Dpd measurements per band
   unsigned short numDaPerBand  = measInfo.numContiguousDA_Meas  * static_cast<unsigned short>( gsmV4CalParams.uiNumChan );

   // The call box can return extra measurements.
   // For example, 5 Digital Gains x 5 RGIs = 25 measurements, but requires 7 frames (4 slots per frame)
   // 7 frames yields 28 measurements.
   // So, numDpdToRemovePerChan = 28 - 25 = 3
   unsigned short numDpdToRemovePerChan = measInfo.numContiguousDPD_Meas_CB - measInfo.numContiguousDPD_Meas;

   vector<double>::const_iterator inputStartIter = inputPowVec.begin();
   vector<double>::const_iterator inputEndIter   = inputPowVec.begin() + numDaPerBand;

   vector<double>::iterator daVecIter  = daPowVec.begin();
   vector<double>::iterator dpdVecIter = dpdPowVec.begin();

   bool exitWhileLoop = false;

   //----------------- 1) Extract DA and DPD measurements from the inputPowVec -----------------------//
   if (  measInfo.numContiguousDPD_Meas > 0 )   // indicates both DA and DPD cal were performed
   {
      while (1)
      {
         //---------------- copy the DA powers Per Band ---------------//
         copy( inputStartIter, inputEndIter, daVecIter );
         inputStartIter += numDaPerBand;
         if ( inputStartIter == inputPowVec.end() )
         {
            break;
         }
         inputEndIter += measInfo.numContiguousDPD_Meas_CB;
         daVecIter    += numDaPerBand;

         //---------------- copy the DPD powers per channel ---------------//
         for (unsigned int chanIdx=0; chanIdx<gsmV4CalParams.uiNumChan; ++chanIdx)
         {
            // copy only up to the desired # of elements
            copy( inputStartIter, ( inputEndIter - numDpdToRemovePerChan ), dpdVecIter );
            inputStartIter += measInfo.numContiguousDPD_Meas_CB;
            if ( inputStartIter == inputPowVec.end() )
            {
               // this indicates a requirement to break out of the entire loop, not just the channel loop
               exitWhileLoop = true;
               break;
            }
            dpdVecIter += measInfo.numContiguousDPD_Meas;
            // don't execute this for the last channel
            if (chanIdx == (gsmV4CalParams.uiNumChan - 1) )
            {
               break;
            }
            inputEndIter += measInfo.numContiguousDPD_Meas_CB;
         }

         if (exitWhileLoop)
         {
            break;
         }

         inputEndIter += numDaPerBand;
      }
   }
   else  // indicates only DA cal was performed
   {
      while (1)
      {
         //-------------- copy the DA powers Per Band -----------------//
         copy( inputStartIter, inputEndIter, daVecIter );
         inputStartIter += numDaPerBand;
         if ( inputStartIter == inputPowVec.end() )
         {
            break;
         }
         inputEndIter   += numDaPerBand;
         daVecIter      += numDaPerBand;
      }
   }

   //----------------- 2) Segment the Results by Chain -----------------------//
   if ( gsmV4CalParams.uiNumTxChains == 1 )
   {
      //--- Just copy the first chain vector
      dDaPowMap_Chain[ gsmV4CalParams.uiTxChainList[0] ]  = daPowVec;
      dDpdPowMap_Chain[ gsmV4CalParams.uiTxChainList[0] ] = dpdPowVec;
   }
   else
   {
      //--- Segment the 2 Tx chains
      unsigned int indDaSize = measInfo.numTotalDA_Meas / gsmV4CalParams.uiNumTxChains;
      dDaPowMap_Chain[ gsmV4CalParams.uiTxChainList[0] ].resize(indDaSize);
      dDaPowMap_Chain[ gsmV4CalParams.uiTxChainList[1] ].resize(indDaSize);

      unsigned int indDpdSize = measInfo.numTotalDPD_Meas / gsmV4CalParams.uiNumTxChains;
      dDpdPowMap_Chain[ gsmV4CalParams.uiTxChainList[0] ].resize(indDpdSize);
      dDpdPowMap_Chain[ gsmV4CalParams.uiTxChainList[1] ].resize(indDpdSize);

      daVecIter  = daPowVec.begin();
      dpdVecIter = dpdPowVec.begin();

      vector<double>::iterator da_chain1_Iter  = dDaPowMap_Chain[ gsmV4CalParams.uiTxChainList[0] ].begin();
      vector<double>::iterator da_chain2_Iter  = dDaPowMap_Chain[ gsmV4CalParams.uiTxChainList[1] ].begin();
      vector<double>::iterator dpd_chain1_Iter = dDpdPowMap_Chain[ gsmV4CalParams.uiTxChainList[0] ].begin();
      vector<double>::iterator dpd_chain2_Iter = dDpdPowMap_Chain[gsmV4CalParams.uiTxChainList[1] ].begin();

      // copy DA data over to the appropriate Tx chain map
      while ( daVecIter < daPowVec.end() )
      {
         copy( daVecIter, daVecIter + gsmV4CalParams.uiNumTxSlots, da_chain1_Iter );
         daVecIter      += gsmV4CalParams.uiNumTxSlots;
         da_chain1_Iter += gsmV4CalParams.uiNumTxSlots;
         copy( daVecIter, daVecIter + gsmV4CalParams.uiNumTxSlots, da_chain2_Iter );
         daVecIter      += gsmV4CalParams.uiNumTxSlots;
         da_chain2_Iter += gsmV4CalParams.uiNumTxSlots;
      }

      // copy DPD data over to the appropriate Tx chain map
      while ( dpdVecIter < dpdPowVec.end() )
      {
         copy( dpdVecIter, dpdVecIter + gsmV4CalParams.uiNumTxSlots, dpd_chain1_Iter );
         dpdVecIter      += gsmV4CalParams.uiNumTxSlots;
         dpd_chain1_Iter += gsmV4CalParams.uiNumTxSlots;
         copy( dpdVecIter, dpdVecIter + gsmV4CalParams.uiNumTxSlots, dpd_chain2_Iter );
         dpdVecIter      += gsmV4CalParams.uiNumTxSlots;
         dpd_chain2_Iter += gsmV4CalParams.uiNumTxSlots;
      }
   }
}

bool DetermineIfLowBand( unsigned int uiCurBand )
{
   bool boolDoingLowBand = true;

   if ( (uiCurBand == NV_MODE_GSM_DCS) || (uiCurBand == NV_MODE_GSM_1900) )
   {
      boolDoingLowBand = false;
   }

   return boolDoingLowBand;
}

void Compute_PCL_Deltas( const vector<double>& dInPowVec, int *iPCLNVValues, GSM_V4_PCL_Sweep_Record& ioPowDeltaMap,
                         const vector<unsigned short>& usPclList, bool boolIsLowBand, bool boolIsEdge, const double& dPclNvOffset, const double& dMaxPclPowDelta, unsigned short &usPowDeltaPowIdx)
{
   unsigned int uiNumPCL = usPclList.size();   // # of actual PCLs used for calibration

   // EDGE (8PSK) PCL error computation can be treated differently than GMSK. This is because the static NV does not contain predistortion data.
   // Therefore, any PCL indexes corresponding to power above the predisotrtion switch point yield invalid power measurements.
   // if true, don't compute all Edge PCL errors
   const bool constComputePartialEdgePclErrors = true;

   unsigned char ucMaxEdgePCL = gsmV4_max8PSK_pclIdx_HB;
   if (boolIsLowBand)
   {
      ucMaxEdgePCL = gsmV4_max8PSK_pclIdx_LB;
   }
   unsigned char ucMaxPCL = static_cast<unsigned char>(uiNumPCL);   // default for GMSK

   // only modify maxPCL to less than numPCL if it is EDGE, and it is desired to ignore high-power PCLs
   if (boolIsEdge && constComputePartialEdgePclErrors)
   {
      ucMaxPCL = ucMaxEdgePCL;
   }
   // Note: m_pclDeltaPowIdx is an index for dInPowVec - it accesses elements for dInPowVec across multiple channels and bands - retains states upon multiple
   // calls to Compute_PCL_Deltas()
   unsigned char  ucPclListIdx = 0;   // index for pclList -> the list of PCL indexes actually used for calibration
   unsigned short usPclIdx     = 0;   // index for pclNV -> the actual PCL powers for each PCL index, read from the NV

   const double constPclConvFac = 100.0; // NV values in pclVecNV are multiplied by 100

   while ( ucPclListIdx < uiNumPCL )
   {
      usPclIdx = usPclList[ucPclListIdx];
      // add maxPCL condidtion in case of EDGE - may want to not compute PCL errors for PCL indexes above predistortion limit
      if (usPclIdx < ucMaxPCL)
      {
         // fill the power delta
         ioPowDeltaMap.pclPwrDelta[usPclIdx] = dInPowVec[usPowDeltaPowIdx] - ( (static_cast<double>( iPCLNVValues[usPclIdx] ) / constPclConvFac) + dPclNvOffset );
         // fill the actual measured power
         ioPowDeltaMap.pclMeasPwr[usPclIdx] = dInPowVec[usPowDeltaPowIdx];
      }
      else
      {
         ioPowDeltaMap.pclPwrDelta[usPclIdx] = 0;
         ioPowDeltaMap.pclMeasPwr[usPclIdx] = 0;
      }
      ++usPowDeltaPowIdx;
      ++ucPclListIdx;
   }//while ( ucPclListIdx < uiNumPCL )
}

void Get_PCL_FromNV( int *pclValues, unsigned short usCurBand, unsigned int uiChainID )
{
   const unsigned char numBytesPerPCL = 2;

   unsigned short nvID       = 0;
   unsigned short nvSize     = RFNV_PCL_NUM_ITEMS*numBytesPerPCL;
   unsigned short readStatus = 0;
   bool bResult = true;

   unsigned char nvArr[RFNV_PCL_NUM_ITEMS*numBytesPerPCL]  = {0x00};

   switch(usCurBand)
   {
      case NV_MODE_GSM_EGSM:
         nvID = RFNV_GSM_C0_GSM900_POWER_LEVELS_I;
         if (uiChainID == 2)
         {
            nvID = RFNV_GSM_C2_GSM900_POWER_LEVELS_I;
         }
         break;
      case NV_MODE_GSM_850:
         nvID = RFNV_GSM_C0_GSM850_POWER_LEVELS_I;
         if (uiChainID == 2)
         {
            nvID = RFNV_GSM_C2_GSM850_POWER_LEVELS_I;
         }
         break;
      case NV_MODE_GSM_DCS:
         nvID = RFNV_GSM_C0_GSM1800_POWER_LEVELS_I;
         if (uiChainID == 2)
         {
            nvID = RFNV_GSM_C2_GSM1800_POWER_LEVELS_I;
         }
         break;
      case NV_MODE_GSM_1900:
         nvID = RFNV_GSM_C0_GSM1900_POWER_LEVELS_I;
         if (uiChainID == 2)
         {
            nvID = RFNV_GSM_C2_GSM1900_POWER_LEVELS_I;
         }
         break;
      default:
         //throw exception unknown NV mode
         break;
   }

   bResult = (QLIB_FTM_RFNV_READ( g_hResourceContext, nvID, nvArr, &nvSize, &readStatus ) != 0);

   if(readStatus != 0 || !bResult)
   {
      //throw exception; read NV failed
   }

   // Extract the PCL power levels for each PCL index
   unsigned short curPCL = 0;
   unsigned char  nvIdx  = 0;
   for (unsigned char pclIdx=0; pclIdx<RFNV_PCL_NUM_ITEMS; ++pclIdx)
   {
      curPCL = nvArr[nvIdx] + (nvArr[nvIdx+1] << 8); // each pcl value is 2 bytes, so combine them. Returned values are in LSByte order
      pclValues[pclIdx] = curPCL;
      nvIdx += numBytesPerPCL;
   }
}
void Fill_PCL_Results( GSM_V4_PCL_Results_Band_Chan& gsmV4_PCL_Results, GSM_V4_Cal_Params& gsmV4CalParams, vector<double> dDaPwrVec, unsigned int curChainID)
{
   GSM_V4_PCL_Sweep_Record    pclPwrDeltaRecord;
   GSM_V4_PCL_Results_Channel chanRecord;

   // number of bands, channels, modulations
   unsigned int uiNumBands  = gsmV4CalParams.uiNumBands;
   unsigned int uiNumChans  = gsmV4CalParams.uiNumChan;
   unsigned int uiNumMods   = gsmV4CalParams.gsmV4DAParams.uiNumMods;

   // current value indicators: band, channel, modulation
   unsigned short usCurBand = 0;
   unsigned short usCurChan = 0;
   unsigned short usCurMod  = 0;

   // indicators of the current modulation scheme
   const unsigned short constGsmMod  = QMSL_GSM_PwrErr_Per_PCL_GMSK_Type;
   const unsigned short constEdgeMod = QMSL_GSM_PwrErr_Per_PCL_8PSK_Type;

   vector<vector<unsigned short> > pclIdxVec( 2, vector<unsigned short>(gsmV4CalParams.gsmV4DAParams.usPclIdxList[0]) );

   bool boolIsEdge = false;
   bool boolIsLowBand = false;

   // initialize m_pclDeltaPowIdx to 0
   //m_pclDeltaPowIdx = 0;

   //PCL values are read from the POWER_LEVEL NVs
   int iPCLNVVal[16] = {450,650,850,1050,1250,1450,1650,1850,2050,2250,2450,2650,2850,3050,3200,3200/*3250,3250*/}; /*EGSM*/
   /*{450,650,850,1050,1250,1450,1650,1850,2050,2250,2450,2650,2850,3050,3200,3200},\
   {0,150,350,550,750,950,1150,1350,1550,1750,1950,2150,2350,2550,2750,2950},\
   {0,150,350,550,750,950,1150,1350,1550,1750,1950,2150,2350,2550,2750,2950}}; */

   map<unsigned short, vector<int> > pclNVMap;
   vector<int> pclNVVec;
   pclNVVec.insert(pclNVVec.begin(),iPCLNVVal[0],iPCLNVVal[0]+16);
   pclNVMap[NV_MODE_GSM_EGSM] = pclNVVec;

   unsigned short usPowDeltaPowIdx = 0;
   // loop through bands
   for (unsigned short usBandIdx=0; usBandIdx< uiNumBands; ++usBandIdx)
   {
      usCurBand = static_cast<unsigned short>( gsmV4CalParams.uiBandList[usBandIdx] );

#ifdef ON_TARGET
      Get_PCL_FromNV( &iPCLNVVal[0], usCurBand, curChainID);
#endif
      boolIsLowBand = DetermineIfLowBand( usCurBand );
      // loop through channels
      for (unsigned short usChanIdx=0; usChanIdx < uiNumChans; ++usChanIdx)
      {
         usCurChan = static_cast<unsigned short>( gsmV4CalParams.uiChanList[usBandIdx][usChanIdx] );

         // loop through modulations
         for (unsigned short usModIdx=0; usModIdx < uiNumMods; ++usModIdx)
         {
            usCurMod = ( (gsmV4CalParams.gsmV4DAParams.usModIdxList[usModIdx] > gsmV4_maxGMSK_mcsIdx) ? constEdgeMod : constGsmMod ) ;
            if (usCurMod == constEdgeMod)
            {
               boolIsEdge = true;
            }
            else
            {
               boolIsEdge = false;
            }
            /*compute the delta between the measured and the power level NV*/
            Compute_PCL_Deltas(dDaPwrVec, &(iPCLNVVal[0]), pclPwrDeltaRecord,
                               pclIdxVec[usCurMod], boolIsLowBand, boolIsEdge, gsmV4CalParams.gsmV4DAParams.dPclNvPowOffset, gsmV4CalParams.gsmV4DAParams.dMaxPclPowDelta, usPowDeltaPowIdx);

            // fill the GSM_V4_PCL_Results_Channel record with the GSM_V4_PCL_Sweep_Record
            if (usCurMod == constGsmMod)
            {
               chanRecord.gsmPCLSweep[usCurChan] = pclPwrDeltaRecord;
            }
            else
            {
               chanRecord.edgePCLSweep[usCurChan] = pclPwrDeltaRecord;
            }
            pclPwrDeltaRecord.pclMeasPwr.clear();
            pclPwrDeltaRecord.pclPwrDelta.clear();
         }//for (unsigned short usModIdx=0; usModIdx < uiNumMods; ++usModIdx)
      }//for (unsigned short usChanIdx=0; usChanIdx < uiNumChans; ++usChanIdx)
      // fill the GSM_V4_PCL_Results_Band_Chan record with the GSM_V4_PCL_Results_Channel record
      gsmV4_PCL_Results.bandSweepData[usCurBand] = chanRecord;
      chanRecord.edgePCLSweep.clear();
      chanRecord.gsmPCLSweep.clear();
   }//for (unsigned short usBandIdx=0; usBandIdx< uiNumBands; ++usBandIdx)
}

// ASSUMPTION - assumes the DPD sweep occurs in this order: DigitalGain - RGI
// For Example: Frame 1, Slots = [0,1,2,3], DigitalGain = [5,5,5,5], RGIs = [19,20,21,22]
void Fill_DPD_Results( GSM_V4_DPD_Results_Band_Chan& gsmV4DPDResults, GSM_V4_Cal_Params& gsmV4CalParams,
                       vector<double> dDpdPowerVec )
{
   GSM_DA_Cal_DigGain_Record digGainPowRecord;
   GSM_V4_DPD_Sweep_Record   digGainRGISweepRecord;

   // number of bands, channels, digital gains, RGIs
   unsigned int uiNumBands   = gsmV4CalParams.uiNumBands;
   unsigned int uiNumChans   = gsmV4CalParams.uiNumChan;
   unsigned int uiNumDigGain = gsmV4CalParams.gsmV4DPDParams.uiNumDpdDigGain;
   unsigned int uiNumRGI     = gsmV4CalParams.gsmV4DPDParams.uiNumDpdRgi;

   // current value indicators: band, channel, digital gain, RGI
   unsigned short usCurBand     = 0;
   unsigned short usCurChan     = 0;
   unsigned short usCurDigGain  = 0;
   unsigned short usCurRGI      = 0;

   unsigned short usDpdPowIdx = 0;

   // loop through bands
   for (unsigned short usBandIdx=0; usBandIdx < uiNumBands; ++usBandIdx)
   {
      usCurBand = static_cast<unsigned short>( gsmV4CalParams.uiBandList[usBandIdx] );
      // loop through channels
      for (unsigned short usChanIdx=0; usChanIdx < uiNumChans; ++usChanIdx)
      {
         usCurChan = static_cast<unsigned short>( gsmV4CalParams.uiChanList[usBandIdx][usChanIdx] );
         for (unsigned short usDigGainIdx=0; usDigGainIdx < uiNumDigGain; ++usDigGainIdx)
         {
            usCurDigGain = static_cast<unsigned short>( gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[usBandIdx][usDigGainIdx] );

            for (unsigned short usRgiIdx=0; usRgiIdx < uiNumRGI; ++usRgiIdx)
            {
               usCurRGI = static_cast<unsigned short>( gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[usBandIdx][usRgiIdx] );

               digGainPowRecord.digGainPwr[usCurRGI] = dDpdPowerVec[usDpdPowIdx];
               ++usDpdPowIdx;
            }  // End RGI loop

            digGainRGISweepRecord.digGainSweep[usCurDigGain] = digGainPowRecord;
            digGainPowRecord.digGainPwr.clear();          // must clear because the records keys can change between channels
         }//for (unsigned short usDigGainIdx=0; usDigGainIdx < uiNumDigGain; ++usDigGainIdx)
         gsmV4DPDResults.band_Chan_sweepResults[usCurBand][usCurChan] = digGainRGISweepRecord;
         digGainRGISweepRecord.digGainSweep.clear();        // must clear because the records keys can change between bands/channels
      }//for (unsigned short usChanIdx=0; usChanIdx < uiNumChans; ++usChanIdx)
   }//for (unsigned short usBandIdx=0; usBandIdx < uiNumBands; ++usBandIdx)
}

#if 0
void Process_Rx_Meas_Results( const GSM_V4_FTM_CMD_Response_Type& gsmV4RxMeasResults, GSM_V4_Rx_Results_Chain& rssiMaps_Chain,
                              GSM_V4_Cal_Params& gsmV4CalParams, map<unsigned int, vector<unsigned short> >& RX_BURST_FRAME_IDENTIFIER )
{
   unsigned short numExpectedPayloads = static_cast<unsigned short>( gsmV4CalParams.uiNumFrames * gsmV4CalParams.uiNumRxChains );
   if ( gsmV4RxMeasResults.numPayload < numExpectedPayloads)
   {
      //throw exception "Expected and received payloads differ"
   }
   unsigned short usNumChan          = static_cast<unsigned short>(gsmV4CalParams.uiNumChan);
   unsigned int uiNumLNA             = gsmV4CalParams.gsmV4RxParams.uiNumLNAState;
   unsigned short usNumBand          = static_cast<unsigned short>(gsmV4CalParams.uiNumBands);
   unsigned int uiNumDev             = gsmV4CalParams.uiNumRxChains;
   unsigned int uiNumFrames          = gsmV4CalParams.uiNumFrames;
   unsigned short usNumFramesPerChan = static_cast<unsigned short>( gsmV4CalParams.uiNumFramesPerChannel );

   unsigned char ucCurDevID = 0;               // the current device ID   ( the device ID is also known as the Tx/Rx chain )
   unsigned int uiDesDevID  = 0;               // desired device ID

   unsigned int uiCurBand    = 0;              // the current band
   unsigned int uiCurChan    = 0;              // the current channel
   unsigned int uiCurLNA     = 0;              // the current LNA state
   unsigned int uiCurRSSI    = 0;              // the current RSSI value

   unsigned short usCurNumMeas            = 0; // keeps track of the number of measurements obtained
   unsigned short uiNumExpecRxMeasPerChan = usNumExpecRxMeas / usNumBand / usNumChan;

   unsigned short usInitFrameIdx   = 0;        // initial frame index

   if ( gsmV4CalParams.boolSkipFirstRxMeas )
   {
      usInitFrameIdx = 1 * gsmV4CalParams.uiNumRxChains; // ignore the measurement for frame 0, if told to skip the first Rx measurement
      usCurNumMeas = 1;   // initialize curNumMeas to 1 so that bogus RSSI results occurring during DPD frames are not used
      // The UUT returns RSSI results for every frame, whether requested or not.
   }

   // the frame index that indicates a new band has begun
   unsigned short usNumFramesPerBand      = usNumFramesPerChan * usNumChan;
   unsigned short usStartNewBandFrameIdx  = usNumFramesPerBand;

   unsigned short usBandIdx  = 0;
   unsigned short usChanIdx  = 0;
   unsigned short usFrameIdx = 0;
   unsigned short usLnaIdx   = 0;

   // map< channel, multimap< LNA state, All RSSI values > >
   // A multimap is used because multiple RSSI measurements can occur for a single LNA state.
   typedef multimap<unsigned int, unsigned int> LNARSSI_type;
   typedef struct
   {
      LNARSSI_type data;
   } LNARSSI;

   typedef map<unsigned int, LNARSSI> ChanLNARSSI_type;
   typedef struct
   {
      ChanLNARSSI_type data;
   } ChanLNARSSI;

   typedef map<unsigned int, ChanLNARSSI> BandChanLNARSSI_type;
   BandChanLNARSSI_type BandChanLNARSSI;

   uiNumDev = 1;

   // loop through gsmV4_Rx_MeasResults and copy relevant data to rssiMaps
   for (unsigned short usDevIdx = 0; usDevIdx < uiNumDev; ++usDevIdx)
   {
      uiDesDevID = gsmV4CalParams.uiRxChainList[usDevIdx];
      for (usFrameIdx = usInitFrameIdx; usFrameIdx < uiNumFrames; ++usFrameIdx)
      {
         if (usFrameIdx == usStartNewBandFrameIdx )
         {
            ++usBandIdx;
            usStartNewBandFrameIdx += usNumFramesPerBand;
         }

         ucCurDevID = gsmV4RxMeasResults.payload[usFrameIdx].devID;
         // check if the current device ID matches what we desire
         if ( ucCurDevID == uiDesDevID )
         {
            uiCurBand = gsmV4CalParams.uiBandList[usBandIdx];
            uiCurChan = gsmV4RxMeasResults.payload[usFrameIdx].result[0].opResult.rssiResult.channel;
            uiCurLNA  = gsmV4RxMeasResults.payload[usFrameIdx].result[0].opResult.rssiResult.lnaState;
            uiCurRSSI = gsmV4RxMeasResults.payload[usFrameIdx].result[0].opResult.rssiResult.rssiMeasResult;

            ChanLNARSSI_type currBanddata = BandChanLNARSSI[uiCurBand].data;
            // reset the number of measurements per channel if a new channel is being added
            if ( (currBanddata.find(uiCurChan) == currBanddata.end())
                  && (usFrameIdx > usInitFrameIdx) )
            {
               usCurNumMeas = 0;
            }

            ++usCurNumMeas;
            // skip the frames that aren't supposed to have real measurement data.
            if ( usCurNumMeas <= uiNumExpecRxMeasPerChan)
            {
               // add data to rssiMaps
               currBanddata[uiCurChan].data.insert( pair<unsigned int, unsigned int>(uiCurLNA, uiCurRSSI) );
            }
         }//if ( ucCurDevID == uiDesDevID )
      }//for (usFrameIdx = usInitFrameIdx; usFrameIdx < uiNumFrames; ++usFrameIdx)
   }//for (unsigned short usDevIdx = 0; usDevIdx < uiNumDev; ++usDevIdx)

   // iterator pair that identifies the range of RSSI values with the same LNA state, for a given channel
   pair <multimap<unsigned int, unsigned int>::iterator, multimap<unsigned int, unsigned int>::iterator> lnaRange;
   // iterates over RSSI values for the same LNA state for a given channel.
   multimap<unsigned int, unsigned int>::iterator rssiIter;

   // reset curNumMeas to be used again
   usCurNumMeas = 0;

   // statistics used for filtering out bad RSSI measurements.
   unsigned int uiCurMeanRSSI   = 0;     // the mean RSSI measurement for a given band/channel/LNA
   float fTempMeanDiffRSSI    = 0.0;   // temporary variable for (xN - xMean)
   float fCurSumMeanDiffRSSI  = 0.0;   // Sum[ (xN - xMean)^2 ]
   float fCurCoVRSSI          = 0.0;   // current coefficient of variation ( StDev / Mean)
   const double MAX_COV_RSSI  = 0.35;  // maximum allowed CoV

   const unsigned int MAX_RSSI = 100000000;   // maximum allowed RSSI - used for filtering out bad RSSI measurements

   // stores all of the RSSI measurements for the current band/channel/LNA
   vector<unsigned int> uiCurRSSIVec;

   // Now, verify that each channel/lna combination exits.
   // Also, average the RSSI results for each channel/lna combination.
   for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
   {
      uiCurBand = gsmV4CalParams.uiBandList[usBandIdx];
      for (usChanIdx = 0; usChanIdx < usNumChan; ++usChanIdx)
      {
         uiCurChan = gsmV4CalParams.uiChanList[usBandIdx][usChanIdx];

         ChanLNARSSI_type currBanddata = BandChanLNARSSI[uiCurBand].data;

         // verify that curChan exists
         if ( currBanddata.find(uiCurChan) != currBanddata.end() )
         {
            for (usLnaIdx = 0; usLnaIdx < uiNumLNA; ++usLnaIdx)
            {
               uiCurLNA = gsmV4CalParams.gsmV4RxParams.uiLNAStateList[usLnaIdx];
               lnaRange = currBanddata[uiCurChan].data.equal_range(uiCurLNA);
               // verify that the LNA state exists
               if ( lnaRange.first == lnaRange.second )
               {
                  //throw exception "Error: GSM V4 Cal - Rx, no measurment results for channel and LNA "
               }

               // Add all RSSI values together for the same LNA state for a given channel
               for (rssiIter = lnaRange.first; rssiIter != lnaRange.second; ++rssiIter)
               {
                  uiCurRSSIVec.push_back(rssiIter->second);
                  uiCurMeanRSSI += rssiIter->second;
                  ++usCurNumMeas;
               }
               // average the results
               uiCurMeanRSSI /= usCurNumMeas;
               // compute the stdDev
               for (rssiIter = lnaRange.first; rssiIter != lnaRange.second; ++rssiIter)
               {
                  fTempMeanDiffRSSI = static_cast<float>(rssiIter->second) - static_cast<float>(uiCurMeanRSSI);
                  fCurSumMeanDiffRSSI += fTempMeanDiffRSSI * fTempMeanDiffRSSI;
               }
               // (coefficient of variation) = (standard deviation) / (mean)
               fCurCoVRSSI  = sqrt(fCurSumMeanDiffRSSI / usCurNumMeas) / uiCurMeanRSSI;

               // Filter out bad RSSI results.
               // If the coefficient of variation is too large, take the good RSSI.
               // Otherwise, take the mean RSSI.
               if ( fCurCoVRSSI > MAX_COV_RSSI )
               {
                  unsigned int uiMinRSSI = *min_element( uiCurRSSIVec.begin(), uiCurRSSIVec.end() );
                  unsigned int uiMaxRSSI = *max_element( uiCurRSSIVec.begin(), uiCurRSSIVec.end() );

                  if (uiMaxRSSI > MAX_RSSI)
                  {
                     rssiMaps.rssi_Map[uiCurBand].chanToLNA_AvgRSSI[uiCurChan][uiCurLNA] = uiMinRSSI;
                  }
                  else
                  {
                     rssiMaps.rssi_Map[uiCurBand].chanToLNA_AvgRSSI[uiCurChan][uiCurLNA] = uiMaxRSSI;
                  }
               }//if ( fCurCoVRSSI > MAX_COV_RSSI )
               else
               {
                  rssiMaps.rssi_Map[uiCurBand].chanToLNA_AvgRSSI[uiCurChan][uiCurLNA] = uiCurMeanRSSI;
               }
               // reset required variables
               usCurNumMeas          = 0;
               fTempMeanDiffRSSI   = 0.0;
               fCurSumMeanDiffRSSI = 0.0;
               uiCurMeanRSSI         = 0;
               fCurCoVRSSI         = 0.0;
               uiCurRSSIVec.clear();
            }//for (usLnaIdx = 0; usLnaIdx < uiNumLNA; ++usLnaIdx)
         }//if ( chanToLNAAllRSSI[uiCurBand].find(uiCurChan) != chanToLNAAllRSSI[uiCurBand].end() )
         else
         {
            //throw exception "Error: GSM V4 Cal - Rx, no measurment results for channel "
         }
      }//for (usChanIdx = 0; usChanIdx < usNumChan; ++usChanIdx)
   }//for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
}

#endif

void Filter_RSSIs( GSM_V4_Cal_Params& gsmV4CalParams,ChainBandChanLNARSSI_type& chanToLNA_AllRSSI, unsigned int curBand, unsigned int curChan,
      unsigned int chainID, GSM_V4_Rx_Results_Chain& rssiMaps, bool isHiLinearity )
{

  // iterator pair that identifies the range of RSSI values with the same LNA state, for a given channel
   pair <multimap<unsigned int, unsigned int>::iterator, multimap<unsigned int, unsigned int>::iterator> lnaRange;
   // iterates over RSSI values for the same LNA state for a given channel.
   multimap<unsigned int, unsigned int>::iterator rssiIter;

   unsigned short curNumMeas = 0;
   unsigned int curLNA       = 0;   // the current LNA state

   // statistics used for filtering out bad RSSI measurements.
   // with some call boxes, occasionally the 2nd of the 2 RSSI measurements for a given LNA/pow comgination is bad (essentially, no donwlink power)
   unsigned int curMeanRSSI   = 0;     // the mean RSSI measurement for a given band/channel/LNA
   float tempMeanDiff_RSSI    = 0.0;   // temporary variable for (xN - xMean)
   float curSumMeanDiff_RSSI  = 0.0;   // Sum[ (xN - xMean)^2 ]
   float curCoV_RSSI          = 0.0;   // current coefficient of variation ( StDev / Mean)

   static const double MAX_COV_RSSI   = 0.35;         // maximum allowed CoV
   static const unsigned int MAX_RSSI = 100000000;    // maximum allowed RSSI - used for filtering out bad RSSI measurements

   // stores all of the RSSI measurements for the current band/channel/LNA
   vector<unsigned int> curRSSI_Vec;


   vector<unsigned int> lnaList;
   if( isHiLinearity )
   {
      lnaList = gsmV4CalParams.gsmV4RxParams.uiLNAStateListHiLin;
   }
   else
   {
      lnaList = gsmV4CalParams.gsmV4RxParams.uiLNAStateList;
   }

   // verify that curChan exists
   if ( chanToLNA_AllRSSI[chainID].data[curBand].data.find(curChan) != chanToLNA_AllRSSI[chainID].data[curBand].data.end() )
   {
      for (unsigned short lnaIdx = 0; lnaIdx < lnaList.size(); ++lnaIdx)
      {
         curLNA   = lnaList[lnaIdx];
         lnaRange = chanToLNA_AllRSSI[chainID].data[curBand].data[curChan].data.equal_range(curLNA);
         // verify that the LNA state exists
         if ( lnaRange.first == lnaRange.second )
         {
            //throw exception "No measurement results"
         }

         // Add all RSSI values together for the same LNA state for a given channel
         for (rssiIter = lnaRange.first; rssiIter != lnaRange.second; ++rssiIter)
         {
            curRSSI_Vec.push_back(rssiIter->second);
            curMeanRSSI += rssiIter->second;
            ++curNumMeas;
         }
         // average the results
         curMeanRSSI /= curNumMeas;

         // compute the stdDev
         for (rssiIter = lnaRange.first; rssiIter != lnaRange.second; ++rssiIter)
         {
            tempMeanDiff_RSSI = static_cast<float>(rssiIter->second) - static_cast<float>(curMeanRSSI);
            curSumMeanDiff_RSSI += tempMeanDiff_RSSI * tempMeanDiff_RSSI;
         }
         // (coefficient of variation) = (standard deviation) / (mean)
         curCoV_RSSI  = sqrt(curSumMeanDiff_RSSI / curNumMeas) / curMeanRSSI;

         // Filter out bad RSSI results. Occasionally, especially with the 8820C, one of the 2 RSSI measurements is bad.
         // If the coefficient of variation is too large, take the good RSSI.
         // Otherwise, take the mean RSSI.
         if ( curCoV_RSSI > MAX_COV_RSSI )
         {
            unsigned int minRSSI = *min_element( curRSSI_Vec.begin(), curRSSI_Vec.end() );
            unsigned int maxRSSI = *max_element( curRSSI_Vec.begin(), curRSSI_Vec.end() );

            if (maxRSSI > MAX_RSSI)
            {
               rssiMaps.chainData[chainID].rssi_Map[curBand].chanToLNA_AvgRSSI[curChan][curLNA] = minRSSI;
            }
            else
            {
               rssiMaps.chainData[chainID].rssi_Map[curBand].chanToLNA_AvgRSSI[curChan][curLNA] = maxRSSI;
            }
            //warning: large covariance encountered
         }
         else
         {
            rssiMaps.chainData[chainID].rssi_Map[curBand].chanToLNA_AvgRSSI[curChan][curLNA] = curMeanRSSI;
         }
         // print the RSSI result to the screen

         // reset required variables
         curNumMeas          = 0;
         tempMeanDiff_RSSI   = 0.0;
         curSumMeanDiff_RSSI = 0.0;
         curMeanRSSI         = 0;
         curCoV_RSSI         = 0.0;
         curRSSI_Vec.clear();
      }
   }
   else
   {
      //throw exception "No measurement results"
   }
}
/*Process the Rx measurements and filter out bad RSSI measurements*/
void Process_Rx_Meas_Results( const GSM_V4_FTM_CMD_Response_Type& gsmV4RxMeasResults, GSM_V4_Rx_Results_Chain& rssiMaps_Chain,GSM_V4_Rx_Results_Chain& rssiMapsHiLin_Chain,
                              GSM_V4_Cal_Params& gsmV4CalParams, map<unsigned int, vector<unsigned short> >& RX_BURST_FRAME_IDENTIFIER )
{
   // See if the returned results has the correct # of payloads
   unsigned short numExpectedPayloads = static_cast<unsigned short>( gsmV4CalParams.uiNumFrames * gsmV4CalParams.uiNumRxChains );
   if ( gsmV4RxMeasResults.numPayload < numExpectedPayloads)
   {
      //throw exception "Expected and Received payloads differ"
   }

   unsigned int curLNA     = 0;              // the current LNA state
   unsigned int curRSSI    = 0;              // the current RSSI value

   unsigned short initPayloadIdx   = 0;        // initial frame index
   if ( gsmV4CalParams.boolSkipFirstRxMeas )
   {
      // ignore the measurement for first payloads for each device, if told to skip the first Rx measurement
      initPayloadIdx = 1 * static_cast<unsigned short>(gsmV4CalParams.uiNumRxChains);
   }

   unsigned short bandIdx    = 0;   // band loop index

   // map< band, map< channel, multimap< LNA state, All RSSI values > > >
   // A multimap is used because multiple RSSI measurements can occur for a single LNA state.

   ChainBandChanLNARSSI_type ChainBandChanLNARSSI;
   ChainBandChanLNARSSI_type ChainBandChanLNARSSIHiLin;
   //------------ Extract Desired RSSI Measurements from gsmV4_Rx_MeasResults -----------//
   // Note: gsmV4RxMeasResults contains measurements for every frame, regardless of whether bursts where scheduled there.
   // The frameID of gsmV4RxMeasResults can occassionally be out of order.
   // This Extraction assumes no order
   unsigned int curBand, curChainID, curChan;
   for (bandIdx = 0; bandIdx < gsmV4CalParams.uiNumBands; ++bandIdx)
   {
      curBand = gsmV4CalParams.uiBandList[bandIdx];

      for (unsigned short rxSchedIdx=0; rxSchedIdx < static_cast<unsigned short>(RX_BURST_FRAME_IDENTIFIER[curBand].size() ); ++ rxSchedIdx)
      {
         // counter indicating whether all desired frames were found
         unsigned short foundFrameCntr = 0;
         // the current desired frame ID
         unsigned short curDesFrame    = RX_BURST_FRAME_IDENTIFIER[curBand][rxSchedIdx];

         // search through payload for desired frames
         for (unsigned short payloadIdx = initPayloadIdx; payloadIdx < gsmV4RxMeasResults.numPayload; ++payloadIdx)
         {
            if (curDesFrame == gsmV4RxMeasResults.payload[payloadIdx].frameID)
            {
               curChainID = gsmV4RxMeasResults.payload[payloadIdx].devID;
               curBand    = gsmV4CalParams.uiBandList[bandIdx];
               curChan    = gsmV4RxMeasResults.payload[payloadIdx].result[0].opResult.rssiResult.channel;
               curLNA       = gsmV4RxMeasResults.payload[payloadIdx].result[0].opResult.rssiResult.lnaState;
               curRSSI      = gsmV4RxMeasResults.payload[payloadIdx].result[0].opResult.rssiResult.rssiMeasResult;

               /***
               if the MSB is set to 1, this means high linearity.               
               */
               if( curLNA >= 128 )
               {
                  ChainBandChanLNARSSIHiLin[curChainID].data[curBand].data[curChan].data.insert( pair<unsigned int, unsigned int>(curLNA - 128, curRSSI) );
               }
               else
               {
               ChainBandChanLNARSSI[curChainID].data[curBand].data[curChan].data.insert( pair<unsigned int, unsigned int>(curLNA, curRSSI) );
               }

               if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
               {
                   unsigned int dRxChain = curChainID + 1;
                    curChan    = gsmV4RxMeasResults.payload[payloadIdx].result[1].opResult.rssiResult.channel;
                    curLNA       = gsmV4RxMeasResults.payload[payloadIdx].result[1].opResult.rssiResult.lnaState;
                    curRSSI      = gsmV4RxMeasResults.payload[payloadIdx].result[1].opResult.rssiResult.rssiMeasResult;
                    if( curLNA >= 128 )
                    {
                        ChainBandChanLNARSSIHiLin[dRxChain].data[curBand].data[curChan].data.insert( pair<unsigned int, unsigned int>(curLNA - 128, curRSSI) );
                    }
                    else
                    {
                        ChainBandChanLNARSSI[dRxChain].data[curBand].data[curChan].data.insert( pair<unsigned int, unsigned int>(curLNA, curRSSI) );
                    }
               }
               ++foundFrameCntr;
            }
            // increment the counter that specifies how many chains were found
            if (foundFrameCntr == gsmV4CalParams.uiNumRxChains)
            {
               break;
            }
         }
      }
   }

   // iterator pair that identifies the range of RSSI values with the same LNA state, for a given channel
   pair <multimap<unsigned int, unsigned int>::iterator, multimap<unsigned int, unsigned int>::iterator> lnaRange;
   // iterates over RSSI values for the same LNA state for a given channel.
   multimap<unsigned int, unsigned int>::iterator rssiIter;

   unsigned short curNumMeas = 0;

   // Now, verify that each channel/lna combination exits.
   // Also, average the RSSI results for each channel/lna combination.
   for (unsigned char devIdx=0; devIdx < gsmV4CalParams.uiNumRxChains; ++devIdx)
   {
      curChainID = gsmV4CalParams.uiRxChainList[devIdx];
      for (bandIdx = 0; bandIdx < gsmV4CalParams.uiNumBands; ++bandIdx)
      {
         curBand = gsmV4CalParams.uiBandList[bandIdx];
         for (unsigned short chanIdx = 0; chanIdx < gsmV4CalParams.uiNumChan; ++chanIdx)
         {
            curChan = gsmV4CalParams.uiChanList[bandIdx][chanIdx];
            Filter_RSSIs(gsmV4CalParams,ChainBandChanLNARSSI, curBand, curChan,curChainID, rssiMaps_Chain, false );

            if(gsmV4CalParams.gsmV4RxParams.boolPerformHiLinRxCal)
            {
                Filter_RSSIs(gsmV4CalParams,ChainBandChanLNARSSIHiLin, curBand, curChan,curChainID, rssiMapsHiLin_Chain, true );
            }
            if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
               {
                unsigned int dRxChain = curChainID+1;
                Filter_RSSIs(gsmV4CalParams,ChainBandChanLNARSSI, curBand, curChan,dRxChain, rssiMaps_Chain, false );

                if(gsmV4CalParams.gsmV4RxParams.boolPerformHiLinRxCal)
                  {
                    Filter_RSSIs(gsmV4CalParams,ChainBandChanLNARSSIHiLin, curBand, curChan,dRxChain, rssiMapsHiLin_Chain, true );
                  }
                  }

         }  // end chan loop
      }  // end band loop
   }  // End DEV Loop
}

BAND_CLASS_ENUM ConverBand_To_BandClassEnum( unsigned int uiGsmBand )
{
   BAND_CLASS_ENUM bandEnum;

   switch( uiGsmBand )
   {
      case NV_MODE_GSM_EGSM:
         bandEnum = BH_GSM_BAND_E_GSM;
         break;
      case NV_MODE_GSM_850:
         bandEnum = BH_GSM_BAND_850;
         break;
      case NV_MODE_GSM_DCS:
         bandEnum = BH_GSM_BAND_DCS;
         break;
      case NV_MODE_GSM_1900:
         bandEnum = BH_GSM_BAND_PCS;
         break;
      default:
         //throw exception
         break;
   }
   return bandEnum;
}

double CalculateGSMULFrequency( BAND_CLASS_ENUM band, long lChannel)
{
   double dFreq = -999.;

   //BuildChannelMaps();

   switch( band )
   {
      case BH_GSM_BAND_E_GSM:
         if( lChannel >= 0 && lChannel <= 124 )
            dFreq = 890. + 0.2 * lChannel;
         else if( lChannel >= 975 && lChannel <= 1023 )
            dFreq = 890. + 0.2 * (lChannel - 1024);
         else
         {
            //throw exception
         }
         break;
      case BH_GSM_BAND_P_GSM:
         dFreq = 890. + 0.2 * lChannel;
         break;
      case BH_GSM_BAND_DCS:
         dFreq = 1710.2 + 0.2 * (lChannel - 512);
         break;
      case BH_GSM_BAND_PCS:
         dFreq = 1850.2 + 0.2 * (lChannel - 512);
         break;
      case BH_GSM_BAND_850:
         dFreq = 824.2 + 0.2 * (lChannel - 128);
         break;
      default:
         //throw exception
         break;
   }
   return dFreq;
}
double CalculateGSMDLFrequency( BAND_CLASS_ENUM band, long lChannel)
{
   double dFreq = -999.;

   //BuildChannelMaps();

   // calculate Uplink Frequecy and then use the UL freq and the Tx/Rx offset to calculate the DL freq
   dFreq = CalculateGSMULFrequency( band, lChannel );

   switch( band )
   {
      case BH_GSM_BAND_E_GSM:
      case BH_GSM_BAND_P_GSM:
      case BH_GSM_BAND_850:
         dFreq += 45.;
         break;
      case BH_GSM_BAND_DCS:
         dFreq += 95.;
         break;
      case BH_GSM_BAND_PCS:
         dFreq += 80.;
         break;

      default:
         //throw exception
         break;
   }
   return dFreq;
}

/*Get the Rx char Data from XML*/
/*For example purpose, Rx char data has been hardcoded in charRx variable*/
void GetCharRxData(GSMRxCharTable &charLnaGain)
{
   charRx[NUM_RX_CHAR_CHAN];
   map<int, double> lnaGainMap;
   unsigned int uiChanIdx;
   unsigned int uiLNAIdx;
   std::map<int, double>::iterator Iter;

   for(uiChanIdx = 0; uiChanIdx < NUM_RX_CHAR_CHAN; uiChanIdx++)
   {
      lnaGainMap.clear();
      Iter = lnaGainMap.begin();
      for(uiLNAIdx = 0; uiLNAIdx < 4; uiLNAIdx++)
      {
         lnaGainMap.insert(Iter,std::pair<int,double>(charRx[uiChanIdx].iLNAState[uiLNAIdx],(double)charRx[uiChanIdx].fGain[uiLNAIdx]));
         Iter = lnaGainMap.end();
      }
      charLnaGain.insert(charLnaGain.end(),std::pair<int,map<int, double> >(charRx[uiChanIdx].iChanNum,lnaGainMap));
   }
}
void Estimate_RSSI_From_CharData( GSM_V4_Cal_Params& gsmV4CalParams, GSM_V4_Rx_Results_Band_Chan& rssiMaps,
   GSM_V4_Rx_RSSI_Estimates_Band_Chan& bandToChanEstRSSIMap, unsigned int chainID, bool boolIsHiLin )
{
   unsigned short usNumChan  = static_cast<unsigned short>( rssiMaps.rssi_Map[ gsmV4CalParams.uiBandList[0] ].chanToLNA_AvgRSSI.size() );
   unsigned int uiNumLNA     = boolIsHiLin? gsmV4CalParams.gsmV4RxParams.uiNumLNAStateHiLin : gsmV4CalParams.gsmV4RxParams.uiNumLNAState;
   unsigned short usNumBand  = static_cast<unsigned short>(gsmV4CalParams.uiNumBands);

   unsigned int uiCurBand    = 0;            // the current band
   unsigned int uiCurChan    = 0;            // the current channel
   unsigned int uiCurLNA     = 0;            // the current LNA state

   // indexes
   unsigned short usBandIdx = 0;
   unsigned short usChanIdx = 0;
   unsigned short usLnaIdx  = 0;

   vector<unsigned int> lnaStateList = boolIsHiLin? gsmV4CalParams.gsmV4RxParams.uiLNAStateListHiLin : gsmV4CalParams.gsmV4RxParams.uiLNAStateList;
   vector<int>          lnaPowerList = boolIsHiLin? gsmV4CalParams.gsmV4RxParams.iLNAPowerListHiLin : gsmV4CalParams.gsmV4RxParams.iRxPowerList;

   //---------------------------- Compute Meas Rx (downlink) Gains --------------------------------//
   // map of LNA state to Rx (downlink) level
   map<unsigned int, int> uiLnaToRxLevMap;
   for (usLnaIdx = 0; usLnaIdx < uiNumLNA; ++usLnaIdx)
   {
      uiLnaToRxLevMap[ lnaStateList[usLnaIdx] ] = lnaPowerList[usLnaIdx];
   }

   // Create a map< band, map<channel, downlink frequency> >
   // First fill it with calibrated channels, then add the Char channels next.
   map< unsigned int, map<unsigned int, double> > chanToDlFreq;
   BAND_CLASS_ENUM curBandEnum = BH_GSM_BAND_E_GSM;

   // GSMRxCharTable = map< channel, map<lna state, gain> >
   map<unsigned int, GSMRxCharTable> bandToChanMeasLnaGain;   // map< band, GSMRxCharTable >

   unsigned int uiCurRSSI  = 0;            // the current RSSI value
   double       dCurRxLev = 0.0;

   /*Compute the gain for the measured channels using RSSI and target power*/
   for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
   {
      uiCurBand     = gsmV4CalParams.uiBandList[usBandIdx];
      curBandEnum = ConverBand_To_BandClassEnum(uiCurBand);
      for (usChanIdx = 0; usChanIdx < usNumChan; ++usChanIdx )
      {
         uiCurChan = gsmV4CalParams.uiChanList[usBandIdx][usChanIdx];
         chanToDlFreq[uiCurBand][uiCurChan] = CalculateGSMDLFrequency( curBandEnum, uiCurChan);
         for (usLnaIdx = 0; usLnaIdx < uiNumLNA; ++usLnaIdx)
         {
            uiCurLNA   = lnaStateList[usLnaIdx];
            uiCurRSSI  = rssiMaps.rssi_Map[uiCurBand].chanToLNA_AvgRSSI[uiCurChan][uiCurLNA];
            dCurRxLev = static_cast<double>( uiLnaToRxLevMap[uiCurLNA] );
            //gain = 10lgo10(RSSI)-target power
            bandToChanMeasLnaGain[uiCurBand][uiCurChan][uiCurLNA] = ( 10*log10( uiCurRSSI / 1.0 ) - dCurRxLev );
         }
      }//for (usChanIdx = 0; usChanIdx < usNumChan; ++usChanIdx )
   }//for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)

   // ---------------------------- Read in the Char data --------------------------//
   GSMRxCharTable charLnaGain;                                 // Map of map< channel, map<lna state, gain> >
   map<unsigned int, GSMRxCharTable> bandToChanCharLnaGain;   // map of map< band, GSMRxCharTable >
   bool v2RxCharExists = false;

   for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
   {
      FTM_RF_Mode_Enum mode = static_cast<FTM_RF_Mode_Enum>( gsmV4CalParams.uiBandList[usBandIdx] );
      GetCharRxData(charLnaGain);

      if( charLnaGain.size() == 0 )
      {
         //throw exception "Error: GSM V4 Cal - Rx, characterized Rx table unavailable for band"
      }

      uiCurBand = gsmV4CalParams.uiBandList[usBandIdx];

      bandToChanCharLnaGain[uiCurBand] = charLnaGain;
   } //for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)

   // Determine the channel list.
   // Also, add channels to the map<channel, freq>
   map<int, map<int, double> >::iterator chanIter;
   map< unsigned int, vector<unsigned int> > charChanMap;
   for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
   {
      uiCurBand     = gsmV4CalParams.uiBandList[usBandIdx];
      curBandEnum = ConverBand_To_BandClassEnum(uiCurBand);
      charLnaGain = bandToChanCharLnaGain[uiCurBand];
      for(chanIter = charLnaGain.begin(); chanIter!=charLnaGain.end(); ++chanIter)
      {
         charChanMap[uiCurBand].push_back(chanIter->first);
         chanToDlFreq[uiCurBand][chanIter->first] = CalculateGSMDLFrequency( curBandEnum, chanIter->first);
      }
   } //for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)

   // Determine the Reference Channels.
   // Note: only 2 or 1 reference channels can be used.
   const unsigned int constMinNumRefChan = 1;  // the minimum number of reference channels to have
   const unsigned int constMaxNumRefChan = 2;  // maximum number of reference channels to have
   map< unsigned int, vector<unsigned int> > refChanMap;

   // Also, check if the characterized data contains all channels that need to calibrate.
   // Also, assume that all LNA ranges are always available.
   for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
   {
      uiCurBand     = gsmV4CalParams.uiBandList[usBandIdx];
      for (usChanIdx = 0; usChanIdx < usNumChan; ++usChanIdx )
      {
         uiCurChan = gsmV4CalParams.uiChanList[usBandIdx][usChanIdx];

         if ( find( charChanMap[uiCurBand].begin(), charChanMap[uiCurBand].end(), uiCurChan ) != charChanMap[uiCurBand].end() )
         {
            refChanMap[uiCurBand].push_back(uiCurChan);
         }
      }
      // throw an exception if too few ref channels were found
      if (refChanMap[uiCurBand].size() < constMinNumRefChan)
      {
         //throw exception " less number of reference channels found"
      }
      // Sort the reference channels to ensure index 1 is always the largest channel.
      // This is useful for the RSSI estimation interpolation formula used later.
      sort( refChanMap[uiCurBand].begin(), refChanMap[uiCurBand].end() );
      // remove the middle element if more than 2 reference channels were found
      if ( refChanMap[uiCurBand].size() > constMaxNumRefChan )
      {
         refChanMap[uiCurBand].erase( refChanMap[uiCurBand].begin() + 1 );
      }
   }//for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)

   // ------------------------------------------ Process with the Char Data and Meas Data --------------------------------------//
   map<unsigned int, GSMRxCharTable> bandToChanRefGainDeltas;

   unsigned int uiNumRefChan = refChanMap[uiCurBand].size();

   //----- 1) Compute the Reference Channel Gain Deltas.
   for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
   {
      uiCurBand     = gsmV4CalParams.uiBandList[usBandIdx];
      for (usChanIdx = 0; usChanIdx < refChanMap[uiCurBand].size(); ++usChanIdx )
      {
         uiCurChan = refChanMap[uiCurBand][usChanIdx];
         for (usLnaIdx = 0; usLnaIdx < uiNumLNA; ++usLnaIdx)
         {
            uiCurLNA   = lnaStateList[usLnaIdx];
            // compute the gain delta: gainDelta = (char Gain) - (Meas Gain)
            bandToChanRefGainDeltas[uiCurBand][uiCurChan][uiCurLNA] = bandToChanCharLnaGain[uiCurBand][uiCurChan][uiCurLNA] - bandToChanMeasLnaGain[uiCurBand][uiCurChan][uiCurLNA];
         }
      } //for (usChanIdx = 0; usChanIdx < usNumChan; ++usChanIdx )
   }//for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)

   //----- 2) Compute the estimated RSSIs for all channels
   //map< unsigned int, map< int, vector<GSM_RX_MEAS_TYPE> > > bandToChan_EstRSSI_Map;
   GSM_RX_MEAS_TYPE tempRxMeas;

   unsigned int uiNumCharChan  = 0;
   unsigned int uiRefChan1     = 0;
   unsigned int uiRefChan2     = 0;

   double dRefChan1GainDelta = 0.0;
   double dRefChan2GainDelta = 0.0;
   double dCurGain            = 0.0;
   double dInterpGainDelta   = 0.0;

   int iCurEstRSSI = 0;
   if (uiNumRefChan == 2)
   {
      double dCurFreq  = 0.0;
      double dRefFreq1 = 0.0;
      double dRefFreq2 = 0.0;

      for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
      {
         uiCurBand     = gsmV4CalParams.uiBandList[usBandIdx];
         uiNumCharChan = charChanMap[uiCurBand].size();
         // reference channels are only updated once per band
         uiRefChan1 = refChanMap[uiCurBand][0];
         uiRefChan2 = refChanMap[uiCurBand][1];
         dRefFreq1 = chanToDlFreq[uiCurBand][uiRefChan1];
         dRefFreq2 = chanToDlFreq[uiCurBand][uiRefChan2];
         // Make sure that refFreq2 is higher in frequency than refFreq1
         if (dRefFreq2 < dRefFreq1)
         {
            // reverse the definition of refChan1 and refChan2
            uiRefChan1 = refChanMap[uiCurBand][1];
            uiRefChan2 = refChanMap[uiCurBand][0];
            dRefFreq1 = chanToDlFreq[uiCurBand][uiRefChan1];
            dRefFreq2 = chanToDlFreq[uiCurBand][uiRefChan2];
         }
         for (usChanIdx = 0; usChanIdx < uiNumCharChan; ++usChanIdx )
         {
            uiCurChan = charChanMap[uiCurBand][usChanIdx];//gsmV4CalParams.uiChanList[usBandIdx][usChanIdx];
            dCurFreq  = chanToDlFreq[uiCurBand][uiCurChan];
            for (usLnaIdx = 0; usLnaIdx < uiNumLNA; ++usLnaIdx)
            {
               uiCurLNA   = lnaStateList[usLnaIdx];
               dCurRxLev = static_cast<double>( uiLnaToRxLevMap[uiCurLNA] );
               dRefChan1GainDelta = bandToChanRefGainDeltas[uiCurBand][uiRefChan1][uiCurLNA];
               dRefChan2GainDelta = bandToChanRefGainDeltas[uiCurBand][uiRefChan2][uiCurLNA];

               tempRxMeas.m_lna = uiCurLNA;

               //------------ estimate the RSSI -----------//
               if ( (dCurFreq == dRefFreq2) || (dCurFreq == dRefFreq1) )
               {
                  // already know the RSSI for reference channels, so don't recompute
                  iCurEstRSSI = rssiMaps.rssi_Map[uiCurBand].chanToLNA_AvgRSSI[uiCurChan][uiCurLNA];
               }
               else  // all channels other than reference channels
               {
                  if (dCurFreq > dRefFreq2)
                  {
                     dCurGain = bandToChanCharLnaGain[uiCurBand][uiCurChan][uiCurLNA] - dRefChan2GainDelta;
                  }
                  else if (dCurFreq < dRefFreq1)
                  {
                     dCurGain = bandToChanCharLnaGain[uiCurBand][uiCurChan][uiCurLNA] - dRefChan1GainDelta;
                  }
                  else // use linear interpolation for frequencies between refChan1 and refChan2 gain deltas
                  {
                     dInterpGainDelta = ( (dRefChan2GainDelta - dRefChan1GainDelta) / (dRefFreq2 - dRefFreq1) ) * (dCurFreq - dRefFreq1) + dRefChan1GainDelta;
                     dCurGain = bandToChanCharLnaGain[uiCurBand][uiCurChan][uiCurLNA] - dInterpGainDelta;
                  }

                  iCurEstRSSI = static_cast<int>( pow(10.0, (dCurGain + dCurRxLev)/10.0 ) );
               }
               tempRxMeas.m_rssi = iCurEstRSSI;
               bandToChanEstRSSIMap.estRSSI_Map[uiCurBand][uiCurChan].push_back( tempRxMeas );
            }//for (usLnaIdx = 0; usLnaIdx < uiNumLNA; ++usLnaIdx)
         }//for (usChanIdx = 0; usChanIdx < usNumChan; ++usChanIdx )
      }//for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
   }//if (uiNumRefChan == 2)
   else if (uiNumRefChan == 1)
   {
      for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
      {
         uiCurBand     = gsmV4CalParams.uiBandList[usBandIdx];
         uiNumCharChan = charChanMap[uiCurBand].size();
         // reference channels are only updated once per band
         uiRefChan1 = refChanMap[uiCurBand][0];
         for (usChanIdx = 0; usChanIdx < usNumChan; ++usChanIdx )
         {
            uiCurChan = gsmV4CalParams.uiChanList[usBandIdx][usChanIdx];
            for (usLnaIdx = 0; usLnaIdx < uiNumLNA; ++usLnaIdx)
            {
               uiCurLNA   = lnaStateList[usLnaIdx];
               dCurRxLev = static_cast<double>( uiLnaToRxLevMap[uiCurLNA] );
               dRefChan1GainDelta = bandToChanRefGainDeltas[uiCurBand][uiRefChan1][uiCurLNA];
               tempRxMeas.m_lna = uiCurLNA;

               //------------- estimate the RSSI -----------//
               if (uiCurChan == uiRefChan1)
               {
                  // already know the RSSI for reference channels, so don't recompute
                  iCurEstRSSI = rssiMaps.rssi_Map[uiCurBand].chanToLNA_AvgRSSI[uiCurChan][uiCurLNA];
               }
               else
               {
                  dCurGain    = bandToChanCharLnaGain[uiCurBand][uiCurChan][uiCurLNA] - dRefChan1GainDelta;
                  iCurEstRSSI = static_cast<int>( pow(10.0, (dCurGain + dCurRxLev)/10.0 )  );
               }
               tempRxMeas.m_rssi = iCurEstRSSI;
               bandToChanEstRSSIMap.estRSSI_Map[uiCurBand][uiCurChan].push_back( tempRxMeas );
            }//for (usLnaIdx = 0; usLnaIdx < uiNumLNA; ++usLnaIdx)
         }//for (usChanIdx = 0; usChanIdx < usNumChan; ++usChanIdx )
      }//for (usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
   }//else if (uiNumRefChan == 1)
}
double Determine_PCL_Pow_Error_For_DaChar( const double& constCurDaPow, unsigned short usNumPCL, map<unsigned short, double>& pclPowMap,
                                           map<unsigned short, double>& pclPowError)
{
   unsigned short usPclPowIdx  = 0;    // pclPowMap index
   unsigned short usPclPowIdx2 = 0;    // second range PCL power index

   double dInterpError = 0.0;
   double dInterpSlope = 0.0;

   // next, loop through all indexes except for the last, one to find the appropriate PCL index
   //For the given RGI power, determine the error based on PCL errors
   for (usPclPowIdx = 0; usPclPowIdx < usNumPCL-1; ++usPclPowIdx)
   {
      usPclPowIdx2 = usPclPowIdx + 1;
      // find the appropriate power index range
      // - check if equal to lower range power
      if ( pclPowMap[usPclPowIdx] == constCurDaPow )
      {
         return pclPowError[usPclPowIdx];
      }
      // - check if equal to upper range power
      else if ( pclPowMap[usPclPowIdx2] == constCurDaPow )
      {
         return pclPowError[usPclPowIdx2];
      }
      // - interpolate between lower and upper range if range contains the curDaPow
      else if ( (pclPowMap[usPclPowIdx] < constCurDaPow) && (pclPowMap[usPclPowIdx2] > constCurDaPow) )
      {
         dInterpSlope = ( pclPowError[usPclPowIdx2] - pclPowError[usPclPowIdx] ) / ( pclPowMap[usPclPowIdx2] - pclPowMap[usPclPowIdx] );
         dInterpError = dInterpSlope*(constCurDaPow - pclPowMap[usPclPowIdx]) + pclPowError[usPclPowIdx];
         return dInterpError;
      }
   }

   // At this point, all PCL indexes were exhausted, indicating curDaPow is > pclPowMap[pclPowIdx]
   return pclPowError[usPclPowIdx];
}

unsigned short Determine_PreDist_Cal_RGI_from_DPD( const vector<unsigned int>& constChanList, map<unsigned int, GSM_V4_DPD_Sweep_Record>& chanDigGainPwr,
   const double constPreDistCalPower, const unsigned short& usDigGainForRgiCalc, bool boolUseV3DPDDelta )
{
   bool boolChanRgiReached       = false;
   double dCurPower           = -1000.0;
   unsigned short usRgiMatch   = 0;
   unsigned short usRgiAllChan = 0;
   unsigned short usCurChan    = 0;
   double V4Discrepancy = 0.0;

   GSM_V4_DPD_Sweep_Record* powMeas;

   // buffer to prevent very near preDistCalPower from causing this to fail.
   // For example, if preDistCalPower = 27 dBm, 26.98 dBm shouldn't cause it to fail.
   // Additionally, the modem software uses 0.5 dB backoff form target. So, a target of 27 dBm is really 26.5 dBm in PCL terms.
   const double constPreDistPowerBuffer = 0.5;
    /*account for the delta (V4-V3) to determine the RGI. In this example, since only one band is shown, the delta is only
    stored in index 0 in ComputeV4_V3_Delta()*/
   if(boolUseV3DPDDelta)
   {
       V4Discrepancy = charV4_V3_Delta[0];
   }

   const double constPowerThresh = constPreDistCalPower - constPreDistPowerBuffer + V4Discrepancy;

   for(unsigned char ucChanIdx = 0; ucChanIdx < constChanList.size(); ++ucChanIdx)
   {
      usCurChan = static_cast<unsigned short>( constChanList[ucChanIdx] );
      powMeas = &chanDigGainPwr[usCurChan];

      boolChanRgiReached  = false;

      if ( powMeas->digGainSweep.find(usDigGainForRgiCalc) != powMeas->digGainSweep.end() )
      {
         // search through RGI list for this digital gain
         // ma<RGI, power>
         map<unsigned short, double>::iterator rgiPowIter;
         for ( rgiPowIter = powMeas->digGainSweep[usDigGainForRgiCalc].digGainPwr.begin();
               rgiPowIter != powMeas->digGainSweep[usDigGainForRgiCalc].digGainPwr.end();
               ++rgiPowIter )
         {

            dCurPower = rgiPowIter->second ;
            if (dCurPower >= constPowerThresh)
            {
               usRgiMatch = rgiPowIter->first;
               boolChanRgiReached = true;
               break;
            }
         }
         if(!boolChanRgiReached)
         {
            //throw excepiton "Failed to acheive target predistortion power ";
         }
         if(usRgiMatch > usRgiAllChan)
         {
            usRgiAllChan = usRgiMatch;
         }
      }//if ( powMeas->digGainSweep.find(usDigGainForRgiCalc) != powMeas->digGainSweep.end() )
      else
      {
         //throw excepiton "DUT dig gain list doesn't contain the required digital gain"
      }
   }//for(unsigned char ucChanIdx = 0; ucChanIdx < constChanList.size(); ++ucChanIdx)
   return usRgiAllChan;
}

unsigned short Determine_PreDist_Cal_RGI_from_DPD_AvgPow( const vector<unsigned int>& constChanList, map<unsigned int, GSM_V4_DPD_Sweep_Record>& chanDigGainPwr,const vector<unsigned int>& rgiList,
   const double constPreDistCalPower, const unsigned short& usDigGainForRgiCalc )
{
   double dCurPower           = -1000.0;
   unsigned short usRgiMatch   = 0;
   unsigned short usRgiAllChan = 0;
   unsigned short usCurChan    = 0;

   GSM_V4_DPD_Sweep_Record* powMeas;
   const double MAX_POW_DELTA = 1.0;

    const double constPowerThresh = constPreDistCalPower;

   unsigned short curRgi = 0;

   // Stores the average power deltas 
   vector<double> avgRgiPowDelta_Vec( rgiList.size() );

   double curPowerDelta  = 0.0;   
   double power          = 0.0;
   
   for ( unsigned short rgiIdx=0; rgiIdx<rgiList.size(); ++rgiIdx )
   {
      curRgi = static_cast<unsigned short>( rgiList[rgiIdx] );
       for(unsigned char ucChanIdx = 0; ucChanIdx < constChanList.size(); ++ucChanIdx)
       {
          usCurChan = static_cast<unsigned short>( constChanList[ucChanIdx] );
          powMeas = &chanDigGainPwr[usCurChan];

            if ( powMeas->digGainSweep.find(usDigGainForRgiCalc) == powMeas->digGainSweep.end() )
            {
                //throw exception
            }
            curPowerDelta = fabs(powMeas->digGainSweep[usDigGainForRgiCalc].digGainPwr[curRgi] - constPreDistCalPower);
            // convert to intenities, in order to average properly
            power += pow(10.0, curPowerDelta/20.0 );
 
        }//for(unsigned char ucChanIdx = 0; ucChanIdx < constChanList.size(); ++ucChanIdx)
          // average the power, and convert back to dB
          avgRgiPowDelta_Vec[rgiIdx] = 20*log10(power / constChanList.size());
          // reset power for each RGI
          power = 0.0;
    }//for ( unsigned short rgiIdx=0; rgiIdx<rgiList.size(); ++rgiIdx )

        //---------------------------------- Step 2: Find the Minimum Absolute Power Delta and Predist RGI  ---------------------------------//
   // Look at the power deltas for each RGI in avgRgiPowDelta_Vec[rgiIdx]
   vector<double>::iterator smallestElem = min_element( avgRgiPowDelta_Vec.begin(), avgRgiPowDelta_Vec.end() );

   unsigned short smallestIdx = static_cast<unsigned short>( distance( avgRgiPowDelta_Vec.begin(), smallestElem ) );

   // verify that the smallest power delta isn't too large
   if ( avgRgiPowDelta_Vec[smallestIdx] > MAX_POW_DELTA )
   {
      //throw exception failed to acheive target predistoriton power
   }

   usRgiAllChan = static_cast<unsigned short>( rgiList[smallestIdx] );
   return usRgiAllChan;
}

//! Determine_Adjacent_Cal_RGI
unsigned short Determine_Adjacent_Cal_RGI( unsigned short curCalRgi, const vector<unsigned int>& candRgiList )
{

   const unsigned short initRGI = 0;

   unsigned short adjRGI = initRGI;

   // Exit if curCalRgi is in the candiate list
   vector<unsigned int>::const_iterator rgiIt = find(candRgiList.begin(), candRgiList.end(), curCalRgi);
   if (rgiIt != candRgiList.end())
   {
      adjRGI = curCalRgi;
      return adjRGI;
   }

   unsigned int numRGI = candRgiList.size();

   // determing if the RGI list is in ascending or descending order
   bool increasingRgiOrder = true;
   // assumes gsmV4_minNumDpdRgi > 1
   if (candRgiList[0] > candRgiList[1])
   {
      increasingRgiOrder = false;
   }

   // Find an adjacent RGI to use
   if (increasingRgiOrder) // acending order
   {
      if ( curCalRgi == ( candRgiList[0] - 1 ) )
      {
         adjRGI = static_cast<unsigned short>(candRgiList[0]);
      }
      else if ( curCalRgi == ( candRgiList[numRGI-1] + 1 ) )
      {
         adjRGI = static_cast<unsigned short>(candRgiList[numRGI-1]);
      }
   }
   else                    // descending order
   {
      if ( curCalRgi == ( candRgiList[0] + 1 ) )
      {
         adjRGI= static_cast<unsigned short>(candRgiList[0]);
      }
      else if ( curCalRgi == ( candRgiList[numRGI-1] - 1 ) )
      {
         adjRGI = static_cast<unsigned short>(candRgiList[numRGI-1]);
      }
   }

   if (adjRGI != initRGI)
   {
      //throw exception "Original calibration RGI not found in RGI list. "
   }
   else
   {
      //throw exception Adjacent RGI not found. Change DPD RGI list
   }

   return adjRGI;
}
//! Determine_PreDist_Cal_RGI
unsigned short Determine_PreDist_Cal_RGI( GSM_V4_Cal_Params& gsmV4CalParams, GSM_DA_Cal_Results_Channel& daCalData, double preDistCalPower, unsigned int curBandIdx )
{

   GSM_DA_Cal_Sweep_Record preDistRangeEdgeDaCalRecord;

   unsigned short rgiMatch   = 0;
   unsigned short rgiAllChan = 0;
   double power             = -1000.0;   // initialize to unlikely value
   bool chanRgiReached      = false;

   // buffer to prevent very near preDistCalPower from causing this to fail.
   // For example, if preDistCalPower = 27 dBm, 26.98 dBm shouldn't cause it to fail.
   const double preDistPowerBuffer = 0.5;

   const unsigned short preDistCalPaRange = static_cast<unsigned short>(gsmV4CalParams.gsmV4DPDParams.uiCalPaRange );

   const double powerThresh = preDistCalPower - preDistPowerBuffer;
   unsigned int curChan;

   map<unsigned short,double>::iterator iter;
   for(unsigned char chanIdx = 0; chanIdx < gsmV4CalParams.uiNumChan; ++chanIdx)
   {
      curChan = gsmV4CalParams.uiChanList[curBandIdx][chanIdx];

      preDistRangeEdgeDaCalRecord = daCalData.channelSweepData[static_cast<unsigned short>(curChan)].edgeDaSweep[preDistCalPaRange];

      chanRgiReached = false;
      for( iter=preDistRangeEdgeDaCalRecord.rgiPwr.begin(); iter!=preDistRangeEdgeDaCalRecord.rgiPwr.end(); ++iter )
      {
         power = iter->second;
         if(power >= powerThresh)
         {
            rgiMatch = iter->first;
            chanRgiReached = true;
            break;
         }
      }
      if(!chanRgiReached)
      {
         //throw exception "failed to achieve targer power"
      }

      if(rgiMatch > rgiAllChan)
      {
         rgiAllChan = rgiMatch;
      }
   }

   return rgiAllChan;

}
void Load_Ops_Daqs(vector<struct opPwrRecord>& ops, vector<struct digRecord>& daqs,  unsigned short usCalRgi, unsigned short usBand, unsigned short usChan, unsigned short usChainID)
{
   opPwrRecord tempOps(0.0,0.0,0.0);
   digRecord tempDaqs(0.0,0.0);

   bool boolFoundRecord = false;
   unsigned short usChanIdx;
   unsigned short usRgiIdx;

   std::vector<struct opPwrRecord>::iterator opsIter;
   std::vector<struct digRecord>::iterator daqsIter;

   int iTableIdx;
   int iDaqIdx;
   ops.clear();
   daqs.clear();
   //In this current example, CharTable contains AMAM/AMPM, Digital gain vs power values only for 4 RGIs. Actual XML file contains more.
   for(usChanIdx = 0; usChanIdx < 3; usChanIdx++)
   {
      if(!boolFoundRecord)
      {
         if(usChan == charTable[usChanIdx][0].charChannel)
         {
            for(usRgiIdx = 0; usRgiIdx < 4; usRgiIdx++)
            {
               if(usCalRgi == charTable[usChanIdx][usRgiIdx].charRGI)
               {
                  for(iTableIdx = 0; iTableIdx < 2*QMSL_NV_MANAGER_POLAR_CAL_AMAM_TABLE_SIZE; iTableIdx++)
                  {
                     tempOps.amam = (float)charTable[usChanIdx][usRgiIdx].charAMAM[iTableIdx];
                     tempOps.ampm = (float)charTable[usChanIdx][usRgiIdx].charAMPM[iTableIdx];
                     tempOps.oppwr = (float)charTable[usChanIdx][usRgiIdx].charPwr[iTableIdx];
                     ops.push_back(tempOps);
                  }
                  for(iDaqIdx = 0; iDaqIdx < 4; iDaqIdx++)
                  {
                     tempDaqs.digGain = (float)charTable[usChanIdx][usRgiIdx].charDigGain[iDaqIdx];
                     tempDaqs.pwr = (float)charTable[usChanIdx][usRgiIdx].charDigPwr[iDaqIdx];
                     daqs.push_back(tempDaqs);
                  }
                  boolFoundRecord = true;
                  break;
               }//if(usCalRgi == charTable[usChanIdx][usRgiIdx].charRGI)
            }// for(usRgiIdx = 0; usRgiIdx < 4; usRgiIdx++)
         }//if(usChan == charTable[usChanIdx][0].charChannel)
      }
      else
      {
         break;
      }
   }
}

void Load_Ops_Daqs_V4(vector<struct opPwrRecord>& ops, vector<struct digRecord>& daqs,  unsigned short usCalRgi, unsigned short usBand, unsigned short usChan, unsigned short usChainID)
{
   opPwrRecord tempOps(0.0,0.0,0.0);
   digRecord tempDaqs(0.0,0.0);

   bool boolFoundRecord = false;
   unsigned short usChanIdx;
   unsigned short usRgiIdx;

   std::vector<struct opPwrRecord>::iterator opsIter;
   std::vector<struct digRecord>::iterator daqsIter;

   int iTableIdx;
   int iDaqIdx;
   ops.clear();
   daqs.clear();
   //In this current example, CharTable contains AMAM/AMPM, Digital gain vs power values only for 4 RGIs. Actual XML file contains more.
   for(usChanIdx = 0; usChanIdx < 3; usChanIdx++)
   {
      if(!boolFoundRecord)
      {
         if(usChan == charTableV3_V4[usChanIdx][0].charChannel)
         {
            for(usRgiIdx = 0; usRgiIdx < 4; usRgiIdx++)
            {
               if(usCalRgi == charTableV3_V4[usChanIdx][usRgiIdx].charRGI)
               {
                  for(iTableIdx = 0; iTableIdx < 2*QMSL_NV_MANAGER_POLAR_CAL_AMAM_TABLE_SIZE; iTableIdx++)
                  {
                     tempOps.amam = (float)charTableV3_V4[usChanIdx][usRgiIdx].charAMAM[iTableIdx];
                     tempOps.ampm = (float)charTableV3_V4[usChanIdx][usRgiIdx].charAMPM[iTableIdx];
                     tempOps.oppwr = (float)charTableV3_V4[usChanIdx][usRgiIdx].charPwr[iTableIdx];
                     ops.push_back(tempOps);
                  }
                  for(iDaqIdx = 0; iDaqIdx < 5; iDaqIdx++)
                  {
                     tempDaqs.digGain = (float)charTableV3_V4[usChanIdx][usRgiIdx].charDigGain_V4[iDaqIdx];
                     tempDaqs.pwr = (float)charTableV3_V4[usChanIdx][usRgiIdx].charDigPwr_V4[iDaqIdx];
                     daqs.push_back(tempDaqs);
                  }
                  boolFoundRecord = true;
                  break;
               }//if(usCalRgi == charTable[usChanIdx][usRgiIdx].charRGI)
            }// for(usRgiIdx = 0; usRgiIdx < 4; usRgiIdx++)
         }//if(usChan == charTable[usChanIdx][0].charChannel)
      }
      else
      {
         break;
      }
   }
}
unsigned short Select_CharDPD_DataSet( unsigned short usCalRgi, unsigned short usBand, unsigned short usChan, unsigned short usChainID, bool boolUseAdaptiveSelection,
                                       vector<struct opPwrRecord>& ops, vector<struct digRecord>& daqs,  map<unsigned short, GSM_DA_Cal_DigGain_Record>& dutDigGainSweep, bool boolV4Cal )
{
   //BAND_CLASS_ENUM bandClass = RFModeToBandType( static_cast<FTM_RF_Mode_Enum>(usBand) );

   // Just get the data for calRgi, if not using adaptive char DPD selection
   if (!boolUseAdaptiveSelection)
   {
      /*Read the char table*/
      if(!boolV4Cal) //Load V3 digital gain vs power table, along with AMAM/AMPM for V3 cal
      {
         Load_Ops_Daqs(ops,daqs,usCalRgi,usBand, usChan, usChainID);
      }
      else //Load V4 digital gain vs power table, along with AMAM/AMPM
      {
         Load_Ops_Daqs_V4(ops,daqs,usCalRgi,usBand, usChan, usChainID);
      }

      return usCalRgi;
   }

   // Parameters for adaptive char dataset selection with initial values.
   bool boolFoundCharRgi         = false;
   double dTarPreDistPow      = 27.0;
   double dCurPredistPow      = 0.0;
   unsigned short usPrevRgi    = usCalRgi;          // previous RGI used for char DPD selection
   unsigned short usCurRgi     = usCalRgi;          // current RGI used for char DPD selection
   double dInitPrevDelta      = 20.0;            // initial value of prevDelta
   double dPrevDelta          = dInitPrevDelta;   // power delta from previous iteration
   double dCurDelta           = 0.0;             // power delta from current iteration
   unsigned int uiDaqIdx       = 2;               // index of digital gain used for computing power deltas
   unsigned short usDaqDigGain = 40;              // digital gain used for computing power deltas
   unsigned short usCounter    = 0;               // used to make sure the while() loop is broken out of

   const unsigned short constMaxCntr = 7;

   // Adaptively determine the char DPD data set to use
   while (!boolFoundCharRgi)
   {
      ops.clear();
      daqs.clear();

      if(!boolV4Cal) //Load V3 digital gain vs power table, along with AMAM/AMPM for V3 cal
      {
         Load_Ops_Daqs(ops,daqs,usCurRgi,usBand, usChan, usChainID);
      }
      else //Load V4 digital gain vs power table, along with AMAM/AMPM
      {
         Load_Ops_Daqs_V4(ops,daqs,usCurRgi,usBand, usChan, usChainID);
      }
      uiDaqIdx        = daqs.size() - 2;                    // set daqIdx to the 2nd to last element
      dCurPredistPow = daqs[uiDaqIdx].pwr; //pwr from char table
      usDaqDigGain    = static_cast<unsigned short>(daqs[uiDaqIdx].digGain);

      // verify daqDigGain is included in the DUT dig gain list
      if ( dutDigGainSweep.find(usDaqDigGain) == dutDigGainSweep.end() )
      {
         //throw exception
      }

      dTarPreDistPow = dutDigGainSweep[usDaqDigGain].digGainPwr[usCalRgi]; //measured pwr
      dCurDelta      = dCurPredistPow - dTarPreDistPow;
      //if char and measured pwr diff is 0, then choose the char for that RGI
      if (dCurDelta == 0)
      {
         boolFoundCharRgi = true;
         break;
      }
      //if prev delta is less than current delta, then choose the previous char table
      if (fabs(dPrevDelta) <= fabs(dCurDelta))
      {
         boolFoundCharRgi = true;
         usCurRgi = usPrevRgi;
         if(!boolV4Cal) //Load V3 digital gain vs power table, along with AMAM/AMPM for V3 cal
         {
            Load_Ops_Daqs(ops,daqs,usCurRgi,usBand, usChan, usChainID);
         }
         else //Load V4 digital gain vs power table, along with AMAM/AMPM
         {
            Load_Ops_Daqs_V4(ops,daqs,usCurRgi,usBand, usChan, usChainID);
         }
         break;
      }

      // if desired Rgi not found yet.
      usPrevRgi   = usCurRgi;
      dPrevDelta = dCurDelta;
      //if current delta is greater than 0, then pick up the next RGI to check again.
      if (dCurDelta > 0)
      {
         usCurRgi = usPrevRgi - 1;
         // check to ensure loop exit
         if (usCurRgi < 0 )
         {
            usCurRgi = usCalRgi;
            boolFoundCharRgi = true;
            break;
         }
         /*Note this is just for this example purpose, since for this example, only 4 RGI char tables are used
         and this check is to prevent this pgm from crash.
         For nomral tool implementation, char table will have more RGIs and hence this check is not needed*/
         else if(usCurRgi < 18) // since there is only 4 RGI in this example char list
         {
            usCurRgi = usPrevRgi;
            boolFoundCharRgi = true;
            break;
         }
      }
      else //if cur delta is less than 0, try the next higher RGI
      {
         usCurRgi = usPrevRgi + 1;
         // check to ensure loop exit
         if (usCurRgi > 31 )
         {
            usCurRgi = usCalRgi;
            boolFoundCharRgi = true;
            break;
         }
         /*Note this is just for this example purpose, since for this example, only 4 RGI char tables are used
         and this check is to prevent this pgm from crash.
         For nomral tool implementation, char table will have more RGIs and hence this check is not needed*/
         else if(usCurRgi > 21) // since there is only 4 RGI in this example char list
         {
            usCurRgi = usPrevRgi;
            boolFoundCharRgi = true;
            break;
         }
      }
      // check to ensure loop exit
      ++usCounter;
      if (usCounter > constMaxCntr)
      {
         break;
      }
   }//   while (!boolFoundCharRgi)
   return usCurRgi;
}

float DetermineDacAtDigGainPwr( const vector<struct opPwrRecord>& ops, float fDigGainPwrIn, unsigned int uiNumDAC )
{
   // The DAC value associated with digGainIn and digGainPwrIn
   float fMaxDac = 0;

   // upper position for ops values for interpolation
   unsigned int uiUpperPos = 0;

   // slope used for interpolation
   float fSlope = 0;

   /*Find the AMAM Dac for the digital gain at Max digital gain power*/
   for(unsigned int uiLowerPos = uiNumDAC-2; uiLowerPos>=0; --uiLowerPos)
   {
      uiUpperPos = uiLowerPos + 1;

      // return DAC at upperPos
      if ( fDigGainPwrIn == ops[uiUpperPos].oppwr )
      {
         fMaxDac = ops[uiUpperPos].amam;
         break;
      }

      // return DAC at lowerPos
      if ( fDigGainPwrIn == ops[uiLowerPos].oppwr )
      {
         fMaxDac = ops[uiLowerPos].amam;
         break;
      }

      // interpolate between DACs at lowerPos and upperPos
      if ( (fDigGainPwrIn < ops[uiUpperPos].oppwr) && (fDigGainPwrIn > ops[uiLowerPos].oppwr) )
      {
         float fNum = ops[uiUpperPos].amam - ops[uiLowerPos].amam;
         float fDen =  ops[uiUpperPos].oppwr - ops[uiLowerPos].oppwr;
         fSlope = fNum / fDen;

         fMaxDac = fSlope*(fDigGainPwrIn - ops[uiLowerPos].oppwr) + ops[uiLowerPos].amam;
         break;
      }
   }

   return fMaxDac;
}

R2MATRIX *R2MatrixAllocate(unsigned int numRows, unsigned int numCols, int elementSize)
{
   unsigned int i;
   R2MATRIX *A;
   float **floatMatrix;

   A = (R2MATRIX *)calloc(1,sizeof(R2MATRIX));
   if (!A)
   {
      //throw exception
   }
   A->numRows = numRows;
   A->numCols = numCols;
   A->elemSize = elementSize;

   floatMatrix = (float**)calloc(numRows,sizeof(float *));
   if (!floatMatrix)
   {
      //throw exception
   }
   for (i=0; i<numRows; i++)
   {
      floatMatrix[i] = (float*)calloc(numCols,sizeof(float));
      if (!floatMatrix[i])
      {
         //throw exception
      }
   }
   A->dataPtr = (char**)floatMatrix;

   return (A);
}

R2MATRIX *R2MatrixTranspose(R2MATRIX *A)
{
   if(!A->dataPtr)
   {
      //throw exception
   }

   R2MATRIX *A_T = R2MatrixAllocate(A->numCols, A->numRows, A->elemSize);

   float **A_T_ptr = (float**)A_T->dataPtr;

   float **A_ptr = (float**)A->dataPtr;

   for (unsigned int rowIdx=0; rowIdx<A->numRows; ++rowIdx)
   {
      for (unsigned int colIdx=0; colIdx<A->numCols; ++colIdx)
      {
         A_T_ptr[rowIdx][colIdx] = A_ptr[colIdx][rowIdx];
      }
   }

   return (A_T);
}

R2MATRIX *R2MatrixMult(R2MATRIX *A, R2MATRIX *B)
{
   if(B->numRows != A->numCols)
   {
      //throw exception
   }

   if(!A->dataPtr)
   {
      //throw exception
   }
   if(!B->dataPtr)
   {
      //throw exception
   }

   // C = A * B
   R2MATRIX *C;

   // simplify the row and col variables
   int aR = A->numRows;
   int aC = A->numCols;
   int bC = B->numCols;

   // allocate C to be of the highest ranking type of A and B
   // - largest element size gives the highest rank
   if( (A->elemSize) > (B->elemSize) )
   {
      C = R2MatrixAllocate(aR, bC, A->elemSize);
   }
   else
   {
      C = R2MatrixAllocate(aR, bC, B->elemSize);
   }

   MULT_R2MATRIX(A, B, C, aR, aC, bC, float, float, float);

   return(C);
}

void R2MatrixFree(R2MATRIX *A)
{
   if (!A || !A->dataPtr || !A->numRows || !A->numCols)
   {
      //throw exception
   }

   char **a = A->dataPtr;

   for (unsigned int i=0; i< A->numRows; i++)
   {
      free(a[i]);
   }

   free((char*)a);
   a = 0;
   free((char*)A);

   return;
}

R2MATRIX *R2MatrixInvert(R2MATRIX *A)
{
   // make sure the matrix is square
   if (A->numRows != A->numCols)
   {
      //throw exception
   }
   if (!A->dataPtr)
   {
      //throw exception
   }

   float big, pivotInverse, temp, absElement;
   int *pivotFlag, *swapCol, *swapRow;
   int i, n, row, col, swap, irow = 0, icol = 0;

   n = A->numRows;

   R2MATRIX *Ai = R2MatrixAllocate(n, n, sizeof(float));
   SCALE_R2MATRIX(A, Ai, 1, n, n, 0, 0, float, float)
   float **a = (float**)Ai->dataPtr;

   pivotFlag = (int*)calloc(n, sizeof(int));
   swapRow   = (int*)calloc(n, sizeof(int));
   swapCol   = (int*)calloc(n, sizeof(int));

   if (!pivotFlag || !swapRow || !swapCol)
   {
      //throw exception
   }

   for (i=0; i<n; i++)
   {
      big = 0.0;
      for (row=0; row<n; row++)
      {
         if (!pivotFlag[row])
         {
            for (col=0; col<n; col++)
            {
               if (!pivotFlag[col])
               {
                  absElement = fabs(a[row][col]);
                  if (absElement >= big)
                  {
                     big = absElement;
                     irow = row;
                     icol = col;
                  }
               }
            }
         }
      }

      pivotFlag[icol]++;

      if (irow != icol)
      {
         for (col=0; col<n; col++)
         {
            temp = a[irow][col];
            a[irow][col] = a[icol][col];
            a[icol][col] = temp;
         }
      }

      swapRow[i] = irow;
      swapCol[i] = icol;

      if (a[icol][icol] == 0.0)
      {
         //throw exception
      }

      pivotInverse = static_cast<float>(1.0/a[icol][icol]);
      a[icol][icol] = 1.0;
      for (col=0; col<n; col++)
      {
         a[icol][col] = a[icol][col]*pivotInverse;
      }
      for (row=0; row<n; row++)
      {
         if (row != icol)
         {
            temp = a[row][icol];
            a[row][icol] = 0.0;
            for (col=0; col<n; col++)
            {
               a[row][col] = a[row][col] - a[icol][col]*temp;
            }
         }
      }
   }

   for (swap=n-1; swap>=0; swap--)
   {
      if (swapRow[swap] != swapCol[swap])
      {
         for (row=0; row <n; row++)
         {
            temp = a[row][swapRow[swap]];
            a[row][swapRow[swap]] = a[row][swapCol[swap]];
            a[row][swapCol[swap]] = temp;
         }
      }
   }

   free((char*)pivotFlag);
   free((char*)swapRow);
   free((char*)swapCol);

   return (Ai);
}

//! EstimatePowerAtMaxDigGain
float EstimatePowerAtMaxDigGain( float fMaxCharAMAM, float fMaxCharPwr, vector<struct digRecord>& charDaqs,
                                 map<unsigned short, GSM_DA_Cal_DigGain_Record>& dutDigGainSweep, unsigned short usCalRgi,
                                 float fDacAtMaxDigGain, /*bool boolApplyMaxDigGainOffset,*/ unsigned short usEdgeDigGain, float fDacScale )
{
   unsigned int uiNumDigGain = charDaqs.size();

   // digital gain at the last digital gain index
   unsigned short digGain = static_cast<unsigned short>( charDaqs[uiNumDigGain-1].digGain );
   //--------------------- First, make sure that maxCharAMAM != dacAtMaxDigGain ----------------------//
   // If so, break out of this function, and default to the direct digital gain method.
   if ( fMaxCharAMAM == fDacAtMaxDigGain )
   {
      // use (digGain + 1) only for the purpose of having a unique element.
      // we still need to have the same # of elements per channel. Other channels may have numDigGain + 1 elements.
      unsigned short maxDigGain = digGain + 1;

      //---------- Add the maxDutDigGain and maxDutPwr to the DUT data set.
      // copy the same power over
      dutDigGainSweep[maxDigGain].digGainPwr[usCalRgi] = dutDigGainSweep[digGain].digGainPwr[usCalRgi] ;

      //---------- Add a digitalGain vs. Pow record to char data
      // resuse the charPower @ the maximum digital gain
      struct digRecord maxChar(maxDigGain, charDaqs[uiNumDigGain-1].pwr );
      charDaqs.push_back(maxChar);
   }
   // The required minimum dimension for performing a polynomial fit
   const unsigned int numRow = 4;
   if (uiNumDigGain < numRow)
   {
      //throw exception
   }
   // column matrix dimension
   unsigned int numCol = numRow;


    // determine the indexes to use for the charDaqs for polynomial estimation
    unsigned short idx0 = 0;
    unsigned short idx1 = 1;
    unsigned short idx2 = 2;
    unsigned short idx3 = 3;
    if (uiNumDigGain > 4)
    {
        idx3 = static_cast<unsigned short>(uiNumDigGain - 1);
    }
   // digital gains in dB
   float digGain_dB_Vec[4] = { 20*log10(charDaqs[idx0].digGain), 20*log10(charDaqs[idx1].digGain), 20*log10(charDaqs[idx2].digGain), 20*log10(charDaqs[idx3].digGain) };

   //------------------------------------- Estimate maxDigGaindB ----------------------------------//
   // Purpose: Estimate the digital gain corresponding to the maximum AMAM/DAC from the char data set
   // Method:  Interpolate between the estimated DAC corresponding to the digital gain of 60 in the char data
   //          and the max possible DAC and digital gain combination.

   const float maxDigGain   = static_cast<float>( ( usEdgeDigGain * 2 ) - 1 );      // Max possible digital gain for 8PSK modulation
   const float maxDac       = 16384 * fDacScale - 1;                               // Max possible DAC/AMAM value
   float maxDac_dB          = 20*log10(maxDac);
   float dacAtMaxDigGain_dB = 20*log10(fDacAtMaxDigGain);
   float maxCharAMAM_dB     = 20*log10(fMaxCharAMAM);        // dB of the maximum AMAM/DAC from the char data

   float digGainEstSlope = ( 20*log10(maxDigGain) - digGain_dB_Vec[3] ) / ( maxDac_dB - dacAtMaxDigGain_dB );

   float maxDigGain_dB = digGainEstSlope*(maxCharAMAM_dB - dacAtMaxDigGain_dB) + digGain_dB_Vec[3];

   float maxDutDigGain = pow( static_cast<float>(10.0), static_cast<float>(maxDigGain_dB / 20.0) );



   //---------------------------------------- Estimate maxDutPwr --------------------------------------//
   // Purpose: Estimate maxDutPwr, which is the DUT power corresponding to maxDutDigGain
   // Method:  Use a polynomial extrapalation of the measured digital gain vs power data, ot obtain the maxDutPwr

   float maxDutPwr = 0.0;
   // X_mat = digital gain matrix for polynomial estimation
   R2MATRIX* X_mat;
   X_mat = R2MatrixAllocate(numRow, numCol, sizeof(float));
   float** X_ptr = (float**)X_mat->dataPtr;

   // Fill the digital gain matrix for polynomial coefficient determination
   float curDigGain = 0.0;

   for (unsigned int rowIdx=0; rowIdx<numRow; ++rowIdx)
   {
      X_ptr[rowIdx][0] = 1;
      curDigGain = digGain_dB_Vec[rowIdx];
      for (unsigned int colIdx=1; colIdx<numCol; ++colIdx)
      {
         X_ptr[rowIdx][colIdx] = X_ptr[rowIdx][colIdx-1]*curDigGain;
      }
   }

   // Y = matrix of power values for power estimation
   R2MATRIX* Y = R2MatrixAllocate(numRow, 1, sizeof(float));
   float** y = (float**)Y->dataPtr;

   float pow0 = static_cast<float>( dutDigGainSweep[ static_cast<unsigned short>(charDaqs[idx0].digGain) ].digGainPwr[usCalRgi] );
   float pow1 = static_cast<float>( dutDigGainSweep[ static_cast<unsigned short>(charDaqs[idx1].digGain) ].digGainPwr[usCalRgi] );
   float pow2 = static_cast<float>( dutDigGainSweep[ static_cast<unsigned short>(charDaqs[idx2].digGain) ].digGainPwr[usCalRgi] );
   float pow3 = static_cast<float>( dutDigGainSweep[ static_cast<unsigned short>(charDaqs[idx3].digGain) ].digGainPwr[usCalRgi] );
   y[0][0] = pow0;
   y[1][0] = pow1;
   y[2][0] = pow2;
   y[3][0] = pow3;

   // X_mat_T = transpose of X_mat
   R2MATRIX* X_mat_T = R2MatrixTranspose(X_mat);
   // C_mat = X_mat_T * X_mat
   R2MATRIX* C_mat = R2MatrixMult(X_mat_T, X_mat);
   R2MatrixFree(X_mat);
   // Cinv_mat = inverse(C_mat)
   R2MATRIX* Cinv_mat = R2MatrixInvert(C_mat);
   R2MatrixFree(C_mat);

   // E = Cinv_mat * X_mat_T
   R2MATRIX* E = R2MatrixMult(Cinv_mat, X_mat_T);
   R2MatrixFree(Cinv_mat);
   R2MatrixFree(X_mat_T);

   // Find the estmated polynomial coefficients - Estimation for Power (dB) vs digital gain (dB)
   R2MATRIX* A = R2MatrixMult(E, Y);
   R2MatrixFree(E);
   R2MatrixFree(Y);
   float** a = (float**)A->dataPtr;
   float c3 = a[3][0];
   float c2 = a[2][0];
   float c1 = a[1][0];
   float c0 = a[0][0];

   R2MatrixFree(A);

   // find the estimated DUT power at max digital gain
   maxDutPwr = c3*(maxDigGain_dB*maxDigGain_dB*maxDigGain_dB) + c2*(maxDigGain_dB*maxDigGain_dB) + c1*(maxDigGain_dB) + c0;

   // compute max power error
   //float estPwr = c3*(digGain_dB_Vec[3]*digGain_dB_Vec[3]*digGain_dB_Vec[3]) + c2*(digGain_dB_Vec[3]*digGain_dB_Vec[3]) + c1*(digGain_dB_Vec[3]) + c0;
   //float perr =  static_cast<float>( dutDigGainSweep[digGain].digGainPwr[calRgi] ) - estPwr;
   //maxDutPwr += perr;

   // make sure the maxDutPwr is not less than the max power already present
   if ( maxDutPwr < dutDigGainSweep[digGain].digGainPwr[usCalRgi] )
   {
      maxDutPwr = static_cast<float>( dutDigGainSweep[digGain].digGainPwr[usCalRgi] );
   }

   //---------- Add the maxDutDigGain and maxDutPwr to the DUT data set.
   dutDigGainSweep[static_cast<unsigned short>(maxDutDigGain)].digGainPwr[usCalRgi] = maxDutPwr;

   //---------- Add a digitalGain vs. Pow record to char data
   struct digRecord maxChar(maxDutDigGain, fMaxCharPwr);
   charDaqs.push_back(maxChar);

   return maxDutPwr;
}
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
//! Read_GSM_TxCx_NV_Payload
// Note: the payloadSize returned from this function is inaccurate.
void Read_GSM_TxCx_NV_Payload(unsigned int nvMode, bool useFTMCmd, QMSL_GSM_Tx_Calibration_NV& readGsmNV, unsigned long& payloadSize,
                              unsigned char chainID )
{
   const unsigned int maxNvArrSize = GSM_TX_CAL_NV_REV0_MAX_PAYLOAD_SIZE;

   unsigned short nvID       = 0;
   unsigned short nvSize     = maxNvArrSize;   // returned by QLIB call
   unsigned short readStatus = 0;              // returned by QLIB call

   unsigned char bResult = false;

   unsigned char nvArr[maxNvArrSize]  = {0x00};

   switch(nvMode)
   {
      case NV_MODE_GSM_850:
         nvID = RFNV_GSM_C0_GSM850_TX_CAL_DATA_I;
         if (chainID == 2)
         {
            nvID = RFNV_GSM_C2_GSM850_TX_CAL_DATA_I;
         }
         break;
      case NV_MODE_GSM_EGSM:
         nvID = RFNV_GSM_C0_GSM900_TX_CAL_DATA_I;
         if (chainID == 2)
         {
            nvID = RFNV_GSM_C2_GSM900_TX_CAL_DATA_I;
         }
         break;
      case NV_MODE_GSM_DCS:
         nvID = RFNV_GSM_C0_GSM1800_TX_CAL_DATA_I;
         if (chainID == 2)
         {
            nvID = RFNV_GSM_C2_GSM1800_TX_CAL_DATA_I;
         }
         break;
      case NV_MODE_GSM_1900:
         nvID = RFNV_GSM_C0_GSM1900_TX_CAL_DATA_I;
         if (chainID == 2)
         {
            nvID = RFNV_GSM_C2_GSM1900_TX_CAL_DATA_I;
         }
         break;
      default:
         //throw exception
         ;
   }

   bResult = QLIB_FTM_RFNV_READ( g_hResourceContext, nvID, nvArr, &nvSize, &readStatus );

   if(readStatus != 0 || bResult == 0)
   {
      //throw exception
   }

   payloadSize = nvSize;

   // fill the readGsmNV payload byte by byte
   memcpy( readGsmNV.nvRev0.aPayload, nvArr, payloadSize);
}

//! GSM_TX_Cx_NV_Extract_Header
void GSM_TX_Cx_NV_Extract_Header( QMSL_GSM_Tx_Calibration_NV* calNV, unsigned long& orgPayloadSize )
{
   unsigned short chanIdx = 0;
   unsigned char* tempPtr = calNV->nvRev0.aPayload;

   // original number of channels
   unsigned long numChan = tempPtr[0];

   calNV->nvRev0.aNvHeader.iChannelListSize = static_cast<unsigned char>( numChan );

   // obtain original channel list
   ++tempPtr;
   for (chanIdx=0; chanIdx<numChan; ++chanIdx)
   {
      calNV->nvRev0.aNvHeader.iChannelList[chanIdx] = tempPtr[0] + (tempPtr[1] << 8);
      tempPtr += GSM_TX_CAL_NV_REV0_NUM_BYTES_CHAN;
   }

   // obtain AMAM_MAX_dBm
   calNV->nvRev0.aNvHeader.iamamMaxPowerDbm100   = tempPtr[0] + (tempPtr[1] << 8);
   tempPtr += GSM_TX_CAL_NV_REV0_NUM_BYTES_AMAM_MAX;

   // extract the predistortion RGI
   calNV->nvRev0.aNvHeader.iPredistortionRgi  = tempPtr[0];
   ++tempPtr;

   // extract the version
   calNV->nvRev0.aVariantMarker.iVersion = tempPtr[0] + (tempPtr[1] << 8);
   tempPtr += GSM_TX_CAL_NV_REV0_NUM_BYTES_VERSION;

   // extract the original number of elements
   calNV->nvRev0.aVariantMarker.iNumElements = tempPtr[0] + (tempPtr[1] << 8);
   tempPtr += GSM_TX_CAL_NV_REV0_NUM_BYTES_NUMELEM;
   // tempPtr now points past the header, to the first data table

   //!!! Now, remove the header from the old payload.
   unsigned char* payloadPtr = calNV->nvRev0.aPayload;

   orgPayloadSize -= GSM_TX_CAL_NV_REV0_NUM_BYTES_IN_PAYLOAD_HEADER;
   memcpy( payloadPtr, tempPtr, orgPayloadSize );
}

//! GSM_TX_Cx_NV_Extract_EDGE_DA_Cal_Data
// only extract the DA cal data that is for predistPaRange for each channel
void GSM_TX_Cx_NV_Extract_EDGE_DA_Cal_Data( QMSL_GSM_Tx_Calibration_NV& calNV, unsigned long& updatedPayloadSize, GSM_DA_Cal_Results_Channel& daCalData,
                                            const unsigned int& predistPaRange, unsigned short& numRGI )
{
   // pointer to the static NV payload
   unsigned char* payloadPtr = calNV.nvRev0.aPayload;

   // NOTE:  orgPayloadSize does not accurately reflect the size of the static NV in terms of valid data.
   // Therefore, orgPayloadSize can not be relied upon at this stage for traversing the static NV payload
   // actualPayloadSize will be used to more accurately reflect the valid data in the static NV.
   unsigned long actualPayloadSize = 0;

   // indicates the variant data type in the DA Char static NV
   unsigned short curDataType_DaChar = 0;

   // used for determining PA states listed in the static NV payload
   list<unsigned short> paStateList;

   //-------------------------------- fill the RGI struct -----------------------------------//
   // NOTE: This assumes the RGI list should be the first element in the static NV,
   // and the RGI list is the same for each PMEAS table. Also, it assumes the RGI lists are consecutive. This is how the NV is written in QMSL.
   // If PMEAS tables and RGI tables are interdispersed, problems will arise.
   QMSL_GSM_RGI_List_Variant_Type tempRGI_List;

   // the RGI list should be the first element in the static NV
   curDataType_DaChar = payloadPtr[0] + (payloadPtr[1] << 8);
   if (curDataType_DaChar == QMSL_RFNV_DATA_TYPE_ID_RGI_LIST)        // RGI List Table
   {
      memcpy( &tempRGI_List, payloadPtr, sizeof(QMSL_GSM_RGI_List_Variant_Type) );

      payloadPtr        += sizeof(QMSL_GSM_RGI_List_Variant_Type);
      actualPayloadSize += sizeof(QMSL_GSM_RGI_List_Variant_Type);
   }
   else
   {
      //throw excepiton "GSM V4 DA Cal static NV did not contain RGI list. "
   }

   // determine the number of RGIs
   numRGI = tempRGI_List.iNumValidRGI;

   // add the first PA element to the paStateList
   paStateList.push_back(tempRGI_List.iPaState);

   //-------------------------------- Determine the # of PA states in the static NV -----------------------------------//

   unsigned char rgiTableCntr = 1;   // initialize to 1 based on the above finding of the RGI list. There is 1 RGI list for each PA state and modulation
   unsigned short numPaStates = 0;
   const unsigned char numMod = 2;   // There must be 2 modulations in the static NV.

   while(1)
   {
      curDataType_DaChar = payloadPtr[0] + (payloadPtr[1] << 8);
      if (curDataType_DaChar != QMSL_RFNV_DATA_TYPE_ID_RGI_LIST)
      {
         break;
      }
      ++rgiTableCntr;
      paStateList.push_back( payloadPtr[3] );
      payloadPtr        += sizeof(QMSL_GSM_RGI_List_Variant_Type);
      actualPayloadSize += sizeof(QMSL_GSM_RGI_List_Variant_Type);
   }

   // compute the number of PA states
   numPaStates = rgiTableCntr / numMod;
   // only keep the unique PA states
   paStateList.unique();

   /*
   ostr.str("");
   ostr << "Number of PA states in static NV determined to be: " << numPaStates << endl;
   m_pUIM->PrintToDebug( ostr.str() );
   */

   // the static NV shoud contain only RGI and PMEAS tables
   unsigned char expectedNumPmeasTables = static_cast<unsigned char>( calNV.nvRev0.aVariantMarker.iNumElements ) - rgiTableCntr;
   unsigned char actualNumPmeasTables   = calNV.nvRev0.aNvHeader.iChannelListSize * static_cast<unsigned char>(numPaStates) * numMod;

   if (actualNumPmeasTables != expectedNumPmeasTables)
   {
      /*"Warning: Static NV - The expected number of DA PMEAS tables does not equal the actual number of PMEAS tables. " <<
      "This indicates the static NV contains tables other than DA PMEAS."*/
   }

   // update the number of elements to reflect only the PMEAS and RGI tables
   calNV.nvRev0.aVariantMarker.iNumElements = actualNumPmeasTables + rgiTableCntr;

   //------------------------ fill the GSM_DA_Cal_Results_Channel ----------------------//
   // NOTE: chanTriggerLevel can be neglected in the GSM_DA_Cal_Results_Channel struct
   // payloadPtr is pointing to the next data table in the payload at this point
   QMSL_GSM_PMEAS_List_Variant_Type tempPMEAS_List;

   unsigned char  curMod     = 0;
   unsigned short curChan    = 0;
   unsigned short curPaState = 0;
   unsigned short curRGI     = 0;

   bool nonDACalItemsEncountered = false;

   unsigned char pMeasCntr = 0;        // keeps track of the # of PMEAS tables accessed

   const double powConFact   = 100.0;  // converts dBm*100 (as stored in static NV) to dBm

   // NOTE: the payloadPtr is currently pointing at the next table element - this table element should be a PMEAS table.
   // All of the RGI tables shoud have been passed.
   // This loop is broken out of once pMeasCntr == actualNumPmeasTables
   while( 1 )
   {
      curDataType_DaChar = payloadPtr[0] + (payloadPtr[1] << 8);

      if (curDataType_DaChar == QMSL_RFNV_DATA_TYPE_ID_PMEAS_LIST) // check if a PMEAS table
      {
         // increment the PMEAS table counter
         ++pMeasCntr;
         // copy data to the PMEAS table
         memcpy( &tempPMEAS_List, payloadPtr, sizeof(QMSL_GSM_PMEAS_List_Variant_Type) );

         // now, copy the PMEAS table to the GSM_DA_Cal_Results_Channel struct
         curMod     = tempPMEAS_List.iModulationType;  // 0 = GMSK, 1 = 8PSK
         curChan    = calNV.nvRev0.aNvHeader.iChannelList[ tempPMEAS_List.iChannelIndex ];
         curPaState = static_cast<unsigned short>( tempPMEAS_List.iPaState );

         // only fill the DA Cal struct if the curPaState is the predistPaRange and the curMod is 8pSK
         if ( (curPaState == predistPaRange) && (curMod == QMSL_GSM_PwrErr_Per_PCL_8PSK_Type) )
         {
            for (unsigned char rgiIdx=0; rgiIdx<tempRGI_List.iNumValidRGI; ++rgiIdx)
            {
               curRGI = tempRGI_List.aRgiList[rgiIdx];
               daCalData.channelSweepData[curChan].edgeDaSweep[curPaState].rgiPwr[curRGI] = tempPMEAS_List.aPmeasList[rgiIdx] / powConFact;;
            }
         }
         // Continue looping in spite of having found all desired PMEAS tables.
         // This is so as to determine an accurate updatedPayloadSize.
         // increment the payload pointer
         payloadPtr        += sizeof(QMSL_GSM_PMEAS_List_Variant_Type);
         actualPayloadSize += sizeof(QMSL_GSM_PMEAS_List_Variant_Type);
      }
      else if (curDataType_DaChar == QMSL_RFNV_DATA_TYPE_ID_AMAM_LIST )
      {
         nonDACalItemsEncountered = true;
         payloadPtr += sizeof(QMSL_GSM_DPD_AMAM_List_Variant_Type);
      }
      else if (curDataType_DaChar == QMSL_RFNV_DATA_TYPE_ID_AMPM_LIST )
      {
         nonDACalItemsEncountered = true;
         payloadPtr += sizeof(QMSL_GSM_DPD_AMPM_List_Variant_Type);
      }
      else if (curDataType_DaChar == QMSL_RFNV_DATA_TYPE_ID_TX_PWR_ERR_LIST )
      {
         nonDACalItemsEncountered = true;
         payloadPtr += sizeof(QMSL_GSM_PwrErr_Per_PCL_List_Variant_Type);
      }
      // break out of the loop as soon as all PMEAS tables have been copied to GSM_DA_Cal_Results_Channel struct
      // actualPayloadSize >= updatedPayloadSize is added for loop breaking out safety
      if ( (pMeasCntr >= actualNumPmeasTables) || (actualPayloadSize >= updatedPayloadSize) )
      {
         break;
      }
   }

   // Update the payloadSize to reflect all of the RGI tables and PMEAS tables already in the static NV
   updatedPayloadSize = actualPayloadSize;
}

void Apply_PCL_CorrectionTo_EdgeCharDA(GSM_V4_Cal_Params& gsmV4CalParams, GSM_DA_Cal_Results_Channel& daCalData, GSM_V4_PCL_Results_Band_Chan& gsmV4_PCL_Results,
                                                         unsigned short numRGI, unsigned int curBand, unsigned int curBandIdx )
{

   unsigned short preDistPaRange = static_cast<unsigned short>(gsmV4CalParams.gsmV4DPDParams.uiCalPaRange);

   // number of PCL indexes for 8PSK - there are less valid PCL indexes for 8PSK than GMSK, so reduce numEdgePCL to eliminate unnecessary searching
   unsigned short numEdgePCL = gsmV4_max8PSK_pclIdx_LB;  // default to low band setting

   bool isLowBand = DetermineIfLowBand( curBand );
   if (!isLowBand)
   {
      numEdgePCL = gsmV4_max8PSK_pclIdx_HB; //determine the PCL based on the PA switch point and power level NV
   }

   // used for testing applying the PCL correction using entire range, or only the subset of PCL indexes
   //numEdgePCL = 16;
   unsigned int curChan;

   // perform the PCl correction for 8PSK modulated data for PA range matching peDistPaRange
   for ( unsigned short chanIdx=0; chanIdx<gsmV4CalParams.uiNumChan; ++chanIdx )
   {
      curChan = gsmV4CalParams.uiChanList[curBandIdx][chanIdx];

      unsigned short shortChan = static_cast<unsigned short>(curChan);

      for (unsigned short rgiIdx=0; rgiIdx<numRGI; ++rgiIdx)
      {
         //------ Only applies correction to 8PSK
         double curDaCharPower = daCalData.channelSweepData[shortChan].edgeDaSweep[preDistPaRange].rgiPwr[rgiIdx];
         double curPowerCorr   = Determine_PCL_Pow_Error_For_DaChar( curDaCharPower, numEdgePCL, gsmV4_PCL_Results.bandSweepData[curBand].edgePCLSweep[curChan].pclMeasPwr,
                                                                     gsmV4_PCL_Results.bandSweepData[curBand].edgePCLSweep[curChan].pclPwrDelta );
         // apply the correction
         daCalData.channelSweepData[shortChan].edgeDaSweep[preDistPaRange].rgiPwr[rgiIdx] = curDaCharPower + curPowerCorr;
      }
   }
}
void Process_GSM_Tx_Cx_NV_From_V4_Cal_PCL( GSM_V4_Cal_Params& gsmV4CalParams, GSM_V4_PCL_Results_Chain& gsmV4PCLResults,
                                           GSM_V4_DPD_Results_Chain& gsmV4DPDResults,  GSM_V4_Tx_Cx_NV_Results& tx_NV_Results)
{
   unsigned short usNumBand = static_cast<unsigned short>( gsmV4CalParams.uiNumBands);

   unsigned short usCurBand = 0;
   unsigned short usCurChan = 0;
   unsigned int   uiChanIdx = 0;
   unsigned short usPclIdx  = 0;
   unsigned int   uiOpsPos  = 0;
   unsigned int   uiDaqPos  = 0;

   unsigned short usDigGain = 0;

   unsigned char ucCurChainID; // = static_cast<unsigned char>( gsmV4CalParams.uiTxChainList[0] );

   unsigned long ulOrgPayloadSize = 0;   // represents the total size of the static NV payload

   QMSL_GSM_Tx_Calibration_NV    calNV;               // calNV struct for a single band/device combination
   QMSL_GSM_V4_PCL_Cal_Result    calPCLToQMSL;       // QMSL struct for result passing PCL cal data to QMSL
   QMSL_GSM_EnhLinTx_Cal_Control calControlToQMSL;   // calControl data for passing to QMSL
   QMSL_GSM_EnhLinTx_Cal_Data    calDPDToQMSL;       // DPD data for passing to QMSL

   // cache the NV items for NV write later
   unsigned char ucCacheNVItems =  true; //(gsmV4_CalParams.m_nv_action & RFCAL_NV_ACTION_QUEUE_NV) ? 1 : 0;

   double dPreDistCalPower     = 0.0;
   // CHARACTERIZED DATA RGI
   // predistoriton RGI. This RGI is used to select the DPD char data.
   unsigned short usCalRgi      = 0;
   // DUT DATA RGI
   // DPD sweep RGI. Nominally equals calRgi. Used to select the DUT dpd sweep data for power/slope adaptation.
   // This differs from calRgi only if the calRgi isn't in the DPD sweep params, and an adjacent RGI can be found.
   unsigned short usDpdSweepRgi = 0;
   // CHAR DATA RGI
   // Comes into play only when adaptive char DPD data selection is enabled.
   unsigned short usCurRgi = usCalRgi;

   // the char DAC corresponding to the maximum digital gain output power in the char data set
   float fDacAtMaxDigGain = 0;

   // if true, pick char data set based upon power measurements.
   // if false, pick char data set absed solely on calRgi
   bool boolUseAdaptiveCharRgiSelection = true;
   // if true, adaptively determine the max power delta for power adaptation
   // if false, use the power at the fixed highest digital gain from the tree params for determining the max power delta
   const bool constUseAdaptiveMaxPowerEstimation = gsmV4CalParams.gsmV4DPDParams.boolDoAdaptiveMaxPwrEst;

   // the number of RGIs found in the static NV RGI tables
   unsigned short usNumRGI  = 0;

   unsigned int uiNumDAC = 2*QMSL_NV_MANAGER_POLAR_CAL_AMAM_TABLE_SIZE;

   for (unsigned char curChainIdx=0; curChainIdx<gsmV4CalParams.uiNumTxChains; ++curChainIdx)
   {
      ucCurChainID = gsmV4CalParams.uiTxChainList[curChainIdx];
      //-------------------------------------------------- START BAND LOOP ------------------------------------------------------------------//
      for (unsigned short usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
      {
         // the current band
         usCurBand = static_cast<unsigned short>( gsmV4CalParams.uiBandList[usBandIdx] );

         //--------------------------------------- Preliminary Static NV Processing ------------------------------------------//
         // reset the orgPayloadSize
         ulOrgPayloadSize = 0; //TODO: find exactly how much it is

         GSM_DA_Cal_Results_Channel daCalData;

         // Read in the calNV from the phone. Will append PCL data to the NV and rewrite.

         memset( &calNV, 0, sizeof(QMSL_GSM_Tx_Calibration_NV) );                      // initialize to 0

#ifndef ON_TARGET
         calNV.nvRev0.aNvHeader.iamamMaxPowerDbm100 = 32767;
         calNV.nvRev0.aNvHeader.iChannelList[0] = 975;
         calNV.nvRev0.aNvHeader.iChannelList[1] = 37;
         calNV.nvRev0.aNvHeader.iChannelList[2] = 124;
         calNV.nvRev0.aNvHeader.iChannelListSize = 3;
         calNV.nvRev0.aNvHeader.iPredistortionRgi = 255;

         calNV.nvRev0.aVariantMarker.iNumElements = 16;
         calNV.nvRev0.aVariantMarker.iVersion = 0;

         calNV.iqCalData.aDelayEstimate[0] = 0;
         calNV.iqCalData.aDelayEstimate[1] = 0;
         calNV.iqCalData.aDelayEstimate[2] = 0;

         calNV.iqCalData.afreqErrEstimate[0] = 0;
         calNV.iqCalData.afreqErrEstimate[1] = 0;
         calNV.iqCalData.afreqErrEstimate[2] = 0;

         calNV.iqCalData.aMinPowerInEdgeDuration[0] = 0.0;
         calNV.iqCalData.aMinPowerInEdgeDuration[1] = 0.0;
         calNV.iqCalData.aMinPowerInEdgeDuration[2] = 0.0;

         QMSL_GSM_RGI_List_Variant_Type rgiList;

         int loc = 0;
         int paRange;
         int modType;
         int rgiNum;
         for(paRange = 0; paRange < 2; paRange++) // PA range 0 and 1
         {
            for(modType =0; modType < 2; modType++)// 0- GMSK, 1-8PSK
            {
               rgiList.iVariantHeader = QMSL_RFNV_DATA_TYPE_ID_RGI_LIST;
               rgiList.iPaState = paRange;
               rgiList.iModulationType = modType;
               rgiList.iNumValidRGI = 32;
               for(rgiNum = 0; rgiNum < 32; rgiNum++)
               {
                  rgiList.aRgiList[rgiNum] = rgiNum;
               }
               memcpy(&(calNV.nvRev0.aPayload[loc]),(char *) (&rgiList), sizeof(QMSL_GSM_RGI_List_Variant_Type));
               loc += sizeof(QMSL_GSM_RGI_List_Variant_Type);
            }
         }

         QMSL_GSM_PMEAS_List_Variant_Type pmeasList;

         for(int chanCnt = 0; chanCnt < 3; chanCnt++) //3 channels
         {
            for(paRange = 0; paRange < 2; paRange++) // PA range 0 and 1
            {
               for(modType =0; modType < 2; modType++) // 0- GMSK, 1-8PSK
               {
                  pmeasList.iVariantHeader = QMSL_RFNV_DATA_TYPE_ID_PMEAS_LIST;
                  pmeasList.iChannelIndex = chanCnt;
                  pmeasList.iModulationType = modType;
                  pmeasList.iPaState = paRange;
                  for(rgiNum = 0; rgiNum < 32; rgiNum++)
                  {
                     pmeasList.aPmeasList[rgiNum] = (short) (charDAPwr[chanCnt][modType][paRange][rgiNum] * 100); // power values are in dbm100 units in NV
                  }
                  memcpy(&(calNV.nvRev0.aPayload[loc]),(char *) (&pmeasList), sizeof(QMSL_GSM_PMEAS_List_Variant_Type));
                  loc += sizeof(QMSL_GSM_PMEAS_List_Variant_Type);
               }
            }
         }

         ulOrgPayloadSize = 3700;

#else
         Read_GSM_TxCx_NV_Payload( gsmV4CalParams.uiBandList[usBandIdx], false, calNV, ulOrgPayloadSize, 0/*curChainID*/ );  // read the calNV payload from the phone; ulOrgPayloadSize = 3714

         GSM_TX_Cx_NV_Extract_Header( &calNV, ulOrgPayloadSize ); //reading NV from phone includes Header and Variant mkr info into the calNV.nvRev0.aPayload itself. Hence need to
         //separate the actual payload.ulOrgPayloadSize = 3700;

#endif
         //--------------------------------------- DA (Characterized RGI power) Processing ------------------------------------------//

         GSM_TX_Cx_NV_Extract_EDGE_DA_Cal_Data( calNV, ulOrgPayloadSize, daCalData, gsmV4CalParams.gsmV4DPDParams.uiCalPaRange, usNumRGI ); //ulOrgPayloadSize = 976

         if(gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation)
         {
            Apply_PCL_CorrectionTo_EdgeCharDA(gsmV4CalParams, daCalData, gsmV4PCLResults.chainData[ucCurChainID],usNumRGI, usCurBand, usBandIdx );
         }

         //--------------------------------------------- Determine Predistortion RGI -----------------------------------------------//
         // Note: The PCL correction, is used, must have been applied prior to this section.

         dPreDistCalPower = gsmV4CalParams.gsmV4DPDParams.dCalRGIPowerLowBand; // default to low-band power

         if ( !DetermineIfLowBand( usCurBand ) )
         {
            dPreDistCalPower = gsmV4CalParams.gsmV4DPDParams.dCalRGIPowerHighBand;
         }

         if(gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation)
         {
            usCalRgi = Determine_PreDist_Cal_RGI( gsmV4CalParams, daCalData, dPreDistCalPower, usBandIdx );
         }
         else
         {
         // use DigGain/RGI method
            usCalRgi = Determine_PreDist_Cal_RGI_from_DPD_AvgPow( gsmV4CalParams.uiChanList[usBandIdx], gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand],gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[usBandIdx],
                                                        dPreDistCalPower, gsmV4CalParams.gsmV4DPDParams.usDigGainForPredistRGI);
         }

         // default the dpdSweepRGI = calRgi
         usDpdSweepRgi = usCalRgi;

          // check if the computed predistortion RGI is in the RGI candidate list
           // use an adjacent RGI, if it is not in the candidate list
           // If an adjacent RGI is unavailable, an exception is thrown
           if ( gsmV4CalParams.gsmV4DPDParams.boolUseAdjacentRGI )
           {
              usDpdSweepRgi = Determine_Adjacent_Cal_RGI( usCalRgi, gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[usBandIdx] );
           }
           if(! gsmV4CalParams.gsmV4DPDParams.boolUseAdjacentRGI && gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation && gsmV4CalParams.gsmV4DPDParams.boolPerformDPD)
           {
               const map<unsigned short, GSM_DA_Cal_DigGain_Record> dutDigGainSweep = gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand][gsmV4CalParams.uiChanList[usBandIdx][0]].digGainSweep;
               if ( dutDigGainSweep.begin()->second.digGainPwr.find(usDpdSweepRgi) == dutDigGainSweep.begin()->second.digGainPwr.end() )
               {
                 //throw exception "DUT digital gain vs. RGI list does not include the predistortion RGI of <usDpdSweepRgi>
               }
           }
         //----------------------------------------------- DA (PCL Error) Processing -----------------------------------------------//

         memset( &calPCLToQMSL, 0, sizeof(QMSL_GSM_V4_PCL_Cal_Result) );

         // copy data from gsmV4_CalParams and gsmV4_PCL_Results to calPCL_ToQMSL
         // ---- channels and PCLs
         calPCLToQMSL.iNumChannels    = gsmV4CalParams.uiNumChan;
         calPCLToQMSL.iNumMeasuredPCL = gsmV4CalParams.gsmV4DAParams.uiNumPCLs;

         for ( uiChanIdx=0; uiChanIdx<gsmV4CalParams.uiNumChan; ++uiChanIdx )
         {
            usCurChan = static_cast<unsigned short>( gsmV4CalParams.uiChanList[usBandIdx][uiChanIdx] );

            calPCLToQMSL.iChannel[uiChanIdx] = usCurChan;

            // ---- PCL indexes and measured power for each index
            for ( usPclIdx=0; usPclIdx < gsmV4CalParams.gsmV4DAParams.uiNumPCLs; ++usPclIdx )
            {
               // Assuming same EDGE and GSM PCL list
               // PCL indexes
               calPCLToQMSL.aiGSM_pclIdx[uiChanIdx][usPclIdx]  = usPclIdx;
               calPCLToQMSL.aiEDGE_pclIdx[uiChanIdx][usPclIdx] = usPclIdx;
               // PCL Delta values
               calPCLToQMSL.aiGSMTxPowerMeasErr[uiChanIdx][usPclIdx]  = gsmV4PCLResults.chainData[ucCurChainID].bandSweepData[usCurBand].gsmPCLSweep[usCurChan].pclPwrDelta[usPclIdx];
               calPCLToQMSL.aiEDGETxPowerMeasErr[uiChanIdx][usPclIdx] = gsmV4PCLResults.chainData[ucCurChainID].bandSweepData[usCurBand].edgePCLSweep[usCurChan].pclPwrDelta[usPclIdx];
            }
         }

         //---------------------------------------------------------- DPD Processing -----------------------------------------------------------------//

         memset( &calControlToQMSL, 0, sizeof(QMSL_GSM_EnhLinTx_Cal_Control) );
         memset( &calDPDToQMSL, 0, sizeof(QMSL_GSM_EnhLinTx_Cal_Data) );

         calControlToQMSL.bSlopeCorrAlignLow     = gsmV4CalParams.gsmV4DPDParams.boolSlopeAlignLow;
         calControlToQMSL.bDoPowerAdaptation     = gsmV4CalParams.gsmV4DPDParams.boolDoPowerAdaptation;
         calControlToQMSL.bDoSlopeCorrection     = gsmV4CalParams.gsmV4DPDParams.boolDoSlopeCorrection;
         calControlToQMSL.bDoPiecewiseAdaptation = gsmV4CalParams.gsmV4DPDParams.boolDoPieceWiseLinear;
         calControlToQMSL.iAlgType               = 1;

         calDPDToQMSL.iCalRgi = usCalRgi;
#if 0
         // GSMV4 - low band power, as seen before, is strikingly different when using GMSK modulation, as compared with other bands
         // This causes the selected calRgi to be 1 higher than it should be, when GMSK modulation is used to select the predistortion RGI, via digital gain of 47.
         if (( usCurBand == NV_MODE_GSM_EGSM || usCurBand == NV_MODE_GSM_850))
         {
            calDPDToQMSL.iCalRgi -= 1;   // only modify the RGI written to NV. Still use the same calRgi for char data selection
         }
#endif
         calDPDToQMSL.dDacScale    = static_cast<float>( gsmV4CalParams.gsmV4DPDParams.dDacScale );
         calDPDToQMSL.iNumChannels = gsmV4CalParams.uiNumChan;

         BAND_CLASS_ENUM bandEnum = ConverBand_To_BandClassEnum( usCurBand );

         // Pack characterized data
         for ( uiChanIdx=0; uiChanIdx < calDPDToQMSL.iNumChannels; ++uiChanIdx )
         {
            calDPDToQMSL.iFreqMapping[uiChanIdx] = static_cast<QMSL_GSM_Polar_Cal_Frequency>( gsmV4CalParams.gsmV4DPDParams.uiNvChanMap[uiChanIdx] );

            usCurChan = static_cast<unsigned short>( gsmV4CalParams.uiChanList[usBandIdx][uiChanIdx] );

            vector<struct opPwrRecord> ops;
            vector<struct digRecord>   daqs;

            usCurRgi = Select_CharDPD_DataSet( usCalRgi, usCurBand, usCurChan, ucCurChainID, boolUseAdaptiveCharRgiSelection, ops, daqs,
                                               gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand][usCurChan].digGainSweep, true );

            if (constUseAdaptiveMaxPowerEstimation)
            {
               bool boolApplyMaxDigGainOffset = false;
               fDacAtMaxDigGain = DetermineDacAtDigGainPwr( ops, daqs[daqs.size()-1].pwr, uiNumDAC );
               // Estimate the DUT power at the maximum char Digital Gain
               // Function returns the esimated power, but also adds it to the DUT digGainSweep
               EstimatePowerAtMaxDigGain( ops[uiNumDAC-1].amam, ops[uiNumDAC-1].oppwr, daqs, gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand][usCurChan].digGainSweep,
                                           usDpdSweepRgi, fDacAtMaxDigGain, /*boolApplyMaxDigGainOffset,*/ 
                                          gsmV4CalParams.gsmV4DPDParams.usDigGainForPredistRGI, calDPDToQMSL.dDacScale );
            }

            // Sanity check number of records
            if (ops.size() != uiNumDAC)
            {
               //throw exception "Input characterization curve does not have expected number of points"
            }

            // Pack characterized predistortion curve
            for( uiOpsPos=0; uiOpsPos< uiNumDAC; ++uiOpsPos )
            {
               calDPDToQMSL.aCharCurves[uiChanIdx].dCharDac[uiOpsPos]   = ops[uiOpsPos].amam;
               calDPDToQMSL.aCharCurves[uiChanIdx].dCharPower[uiOpsPos] = ops[uiOpsPos].oppwr;
               calDPDToQMSL.aCharCurves[uiChanIdx].dCharAmpm[uiOpsPos]  = ops[uiOpsPos].ampm;
            }

            //------------------------ Pack characterized digital gain table ------------------------//
            bool boolUseAllPoints = gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].empty();

            if (daqs.size() > QMSL_NV_MANAGER_ENH_CAL_DIG_GAIN_TABLE_SIZE)
            {
               //throw exception "Input characterization digital gains table has more than 64 entries"
            }

            calDPDToQMSL.aCharDigGain[uiChanIdx].iNumEntries = daqs.size();
            for ( uiDaqPos=0; uiDaqPos< daqs.size(); ++uiDaqPos )
            {
               usDigGain = static_cast<unsigned short>( daqs[uiDaqPos].digGain );

               bool boolUsePoint = find( gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].begin(),
                                         gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].end(),
                                         usDigGain )
                                   != gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].end();

               calDPDToQMSL.aCharDigGain[uiChanIdx].iDigGain[uiDaqPos] = usDigGain;
               calDPDToQMSL.aCharDigGain[uiChanIdx].dPower[uiDaqPos]   = daqs[uiDaqPos].pwr;
               calControlToQMSL.bSlopeCorrUseCharPoint[uiDaqPos]      = boolUsePoint || boolUseAllPoints;
            }

            //------------------------------- Pack measured digital gains from DUT ------------------------------//
            calDPDToQMSL.aDutDigGain[uiChanIdx].iNumEntries = 0;
            unsigned int uiDigGainTablePos = 0;
            GSM_V4_DPD_Sweep_Record* channelSweepResults = &gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand][usCurChan];

            map<unsigned short, GSM_DA_Cal_DigGain_Record>::iterator digGainIter;

            for (digGainIter=channelSweepResults->digGainSweep.begin(); digGainIter!=channelSweepResults->digGainSweep.end(); ++digGainIter )
            {
               if (digGainIter->second.digGainPwr.find(usDpdSweepRgi) != digGainIter->second.digGainPwr.end())
               {
                  usDigGain = static_cast<unsigned short>( digGainIter->first );
                  bool boolUsePoint = find( gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].begin(),
                                            gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].end(),
                                            usDigGain )
                                      != gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].end();

                  if (uiDigGainTablePos > QMSL_NV_MANAGER_ENH_CAL_DIG_GAIN_TABLE_SIZE)
                  {
                     //throw exception "Input characterization digital gains table has"
                  }

                  calDPDToQMSL.aDutDigGain[uiChanIdx].iDigGain[uiDigGainTablePos] = usDigGain;
                  calDPDToQMSL.aDutDigGain[uiChanIdx].dPower[uiDigGainTablePos] = static_cast<float>( digGainIter->second.digGainPwr[usDpdSweepRgi] );
                  calControlToQMSL.bSlopeCorrUseDutPoint[uiDigGainTablePos] = boolUsePoint || boolUseAllPoints;
                  calDPDToQMSL.aDutDigGain[uiChanIdx].iNumEntries += 1;
                  uiDigGainTablePos += 1;
               }//if (digGainIter->second.digGainPwr.find(usDpdSweepRgi) != digGainIter->second.digGainPwr.end())
            }//for (digGainIter=channelSweepResults->digGainSweep.begin(); digGainIter!=channelSweepResults->digGainSweep.end(); ++digGainIter )

            if ( calDPDToQMSL.aDutDigGain[uiChanIdx].iNumEntries < 1 && gsmV4CalParams.gsmV4DPDParams.boolDoPowerAdaptation )
            {
               //throw exception "DUT digital gain table should contain atleast 1 entry to do power adapation"
            }
            if ( calDPDToQMSL.aDutDigGain[uiChanIdx].iNumEntries < 2 && (gsmV4CalParams.gsmV4DPDParams.boolDoSlopeCorrection || gsmV4CalParams.gsmV4DPDParams.boolDoPieceWiseLinear) )
            {
               //throw exception "Need atleast 2 entries in DUT table for slope correction/piecewise linear"
            }
            if ( calDPDToQMSL.aCharDigGain[uiChanIdx].iNumEntries < 1 && gsmV4CalParams.gsmV4DPDParams.boolDoPowerAdaptation )
            {
               //throw exception "Need at least 1 entry in char table for power adaption"
            }
            if ( calDPDToQMSL.aCharDigGain[uiChanIdx].iNumEntries < 2 && (gsmV4CalParams.gsmV4DPDParams.boolDoSlopeCorrection || gsmV4CalParams.gsmV4DPDParams.boolDoPieceWiseLinear) )
            {
               //throw exception "Need atleast 2 entries in char table for slope correction/piecewise linear"
            }
         }//for ( uiChanIdx=0; uiChanIdx < calDPDToQMSL.iNumChannels; ++uiChanIdx )

         //---------------------------------------------------------- NV GENERATION -----------------------------------------------------------------//

         QLIB_RFCal_GSM_V4_Cal_Tx_Generate_NV(
            g_hResourceContext,
            usCurBand,
            ucCurChainID,
            (void*)& calPCLToQMSL,
            (void*)& calControlToQMSL,
            (void*)& calDPDToQMSL,
            (void*)& calNV,
            ulOrgPayloadSize,
            ucCacheNVItems ); //cacheNVitmes = 1

         // push the calNV onto the multi-band NV map
         tx_NV_Results.chainNV[ucCurChainID][usCurBand] = calNV;
      }//for (unsigned short usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
   }
}


void Process_GSM_Tx_Cx_NV_From_V4_Cal_DA_Only_PCL( GSM_V4_Cal_Params& gsmV4CalParams, GSM_V4_PCL_Results_Chain& gsmV4PCLResults,
   GSM_V4_DPD_Results_Chain& gsmV4DPDResults,  GSM_V4_Tx_Cx_NV_Results& tx_NV_Results)
{
   unsigned short usNumBand = static_cast<unsigned short>( gsmV4CalParams.uiNumBands);

   unsigned short usCurBand = 0;
   unsigned short usCurChan = 0;
   unsigned int   uiChanIdx = 0;
   unsigned short usPclIdx  = 0;
   unsigned int   uiOpsPos  = 0;
   unsigned int   uiDaqPos  = 0;

   unsigned short usDigGain = 0;

   unsigned char ucCurChainID; // = static_cast<unsigned char>( gsmV4CalParams.uiTxChainList[0] );

   unsigned long ulOrgPayloadSize = 0;   // represents the total size of the static NV payload

   QMSL_GSM_Tx_Calibration_NV    calNV;               // calNV struct for a single band/device combination
   QMSL_GSM_V4_PCL_Cal_Result    calPCLToQMSL;       // QMSL struct for result passing PCL cal data to QMSL

   // cache the NV items for NV write later
   unsigned char ucCacheNVItems =  true; //(gsmV4_CalParams.m_nv_action & RFCAL_NV_ACTION_QUEUE_NV) ? 1 : 0;

   double dPreDistCalPower     = 0.0;
   // CHARACTERIZED DATA RGI
   // predistoriton RGI. This RGI is used to select the DPD char data.
   unsigned short usCalRgi      = 0;
   // DUT DATA RGI
   // DPD sweep RGI. Nominally equals calRgi. Used to select the DUT dpd sweep data for power/slope adaptation.
   // This differs from calRgi only if the calRgi isn't in the DPD sweep params, and an adjacent RGI can be found.
   unsigned short usDpdSweepRgi = 0;
   // CHAR DATA RGI
   // Comes into play only when adaptive char DPD data selection is enabled.
   unsigned short usCurRgi = usCalRgi;

   // the char DAC corresponding to the maximum digital gain output power in the char data set
   float fDacAtMaxDigGain = 0;

   // if true, pick char data set based upon power measurements.
   // if false, pick char data set absed solely on calRgi
   bool boolUseAdaptiveCharRgiSelection = true;
   // if true, adaptively determine the max power delta for power adaptation
   // if false, use the power at the fixed highest digital gain from the tree params for determining the max power delta
   const bool constUseAdaptiveMaxPowerEstimation = gsmV4CalParams.gsmV4DPDParams.boolDoAdaptiveMaxPwrEst;

   // the number of RGIs found in the static NV RGI tables
   unsigned short usNumRGI  = 0;

   unsigned int uiNumDAC = 2*QMSL_NV_MANAGER_POLAR_CAL_AMAM_TABLE_SIZE;

   for (unsigned char curChainIdx=0; curChainIdx<gsmV4CalParams.uiNumTxChains; ++curChainIdx)
   {
      ucCurChainID = gsmV4CalParams.uiTxChainList[curChainIdx];
      //-------------------------------------------------- START BAND LOOP ------------------------------------------------------------------//
      for (unsigned short usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
      {
         // the current band
         usCurBand = static_cast<unsigned short>( gsmV4CalParams.uiBandList[usBandIdx] );

         //--------------------------------------- Preliminary Static NV Processing ------------------------------------------//
         // reset the orgPayloadSize
         ulOrgPayloadSize = 0; //TODO: find exactly how much it is

         GSM_DA_Cal_Results_Channel daCalData;

         // Read in the calNV from the phone. Will append PCL data to the NV and rewrite.

         memset( &calNV, 0, sizeof(QMSL_GSM_Tx_Calibration_NV) );                      // initialize to 0

#ifndef ON_TARGET
         calNV.nvRev0.aNvHeader.iamamMaxPowerDbm100 = 32767;
         calNV.nvRev0.aNvHeader.iChannelList[0] = 975;
         calNV.nvRev0.aNvHeader.iChannelList[1] = 37;
         calNV.nvRev0.aNvHeader.iChannelList[2] = 124;
         calNV.nvRev0.aNvHeader.iChannelListSize = 3;
         calNV.nvRev0.aNvHeader.iPredistortionRgi = 255;

         calNV.nvRev0.aVariantMarker.iNumElements = 16;
         calNV.nvRev0.aVariantMarker.iVersion = 0;

         calNV.iqCalData.aDelayEstimate[0] = 0;
         calNV.iqCalData.aDelayEstimate[1] = 0;
         calNV.iqCalData.aDelayEstimate[2] = 0;

         calNV.iqCalData.afreqErrEstimate[0] = 0;
         calNV.iqCalData.afreqErrEstimate[1] = 0;
         calNV.iqCalData.afreqErrEstimate[2] = 0;

         calNV.iqCalData.aMinPowerInEdgeDuration[0] = 0.0;
         calNV.iqCalData.aMinPowerInEdgeDuration[1] = 0.0;
         calNV.iqCalData.aMinPowerInEdgeDuration[2] = 0.0;

         QMSL_GSM_RGI_List_Variant_Type rgiList;

         int loc = 0;
         int paRange;
         int modType;
         int rgiNum;
         for(paRange = 0; paRange < 2; paRange++) // PA range 0 and 1
         {
            for(modType =0; modType < 2; modType++)// 0- GMSK, 1-8PSK
            {
               rgiList.iVariantHeader = QMSL_RFNV_DATA_TYPE_ID_RGI_LIST;
               rgiList.iPaState = paRange;
               rgiList.iModulationType = modType;
               rgiList.iNumValidRGI = 32;
               for(rgiNum = 0; rgiNum < 32; rgiNum++)
               {
                  rgiList.aRgiList[rgiNum] = rgiNum;
               }
               memcpy(&(calNV.nvRev0.aPayload[loc]),(char *) (&rgiList), sizeof(QMSL_GSM_RGI_List_Variant_Type));
               loc += sizeof(QMSL_GSM_RGI_List_Variant_Type);
            }
         }

         QMSL_GSM_PMEAS_List_Variant_Type pmeasList;

         for(int chanCnt = 0; chanCnt < 3; chanCnt++) //3 channels
         {
            for(paRange = 0; paRange < 2; paRange++) // PA range 0 and 1
            {
               for(modType =0; modType < 2; modType++) // 0- GMSK, 1-8PSK
               {
                  pmeasList.iVariantHeader = QMSL_RFNV_DATA_TYPE_ID_PMEAS_LIST;
                  pmeasList.iChannelIndex = chanCnt;
                  pmeasList.iModulationType = modType;
                  pmeasList.iPaState = paRange;
                  for(rgiNum = 0; rgiNum < 32; rgiNum++)
                  {
                     pmeasList.aPmeasList[rgiNum] = (short) (charDAPwr[chanCnt][modType][paRange][rgiNum] * 100); // power values are in dbm100 units in NV
                  }
                  memcpy(&(calNV.nvRev0.aPayload[loc]),(char *) (&pmeasList), sizeof(QMSL_GSM_PMEAS_List_Variant_Type));
                  loc += sizeof(QMSL_GSM_PMEAS_List_Variant_Type);
               }
            }
         }

         ulOrgPayloadSize = 3700;

#else
         Read_GSM_TxCx_NV_Payload( gsmV4CalParams.uiBandList[usBandIdx], false, calNV, ulOrgPayloadSize, 0/*curChainID*/ );  // read the calNV payload from the phone; ulOrgPayloadSize = 3714

         GSM_TX_Cx_NV_Extract_Header( &calNV, ulOrgPayloadSize ); //reading NV from phone includes Header and Variant mkr info into the calNV.nvRev0.aPayload itself. Hence need to
         //separate the actual payload.ulOrgPayloadSize = 3700;

#endif
         //--------------------------------------- DA (Characterized RGI power) Processing ------------------------------------------//

         GSM_TX_Cx_NV_Extract_EDGE_DA_Cal_Data( calNV, ulOrgPayloadSize, daCalData, gsmV4CalParams.gsmV4DPDParams.uiCalPaRange, usNumRGI ); //ulOrgPayloadSize = 976

         if(gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation)
         {
            Apply_PCL_CorrectionTo_EdgeCharDA(gsmV4CalParams, daCalData, gsmV4PCLResults.chainData[ucCurChainID],usNumRGI, usCurBand, usBandIdx );
         }
         //--------------------------------------------- Determine Predistortion RGI -----------------------------------------------//
         // Note: The PCL correction, is used, must have been applied prior to this section.

         dPreDistCalPower = gsmV4CalParams.gsmV4DPDParams.dCalRGIPowerLowBand; // default to low-band power

         if ( !DetermineIfLowBand( usCurBand ) )
         {
            dPreDistCalPower = gsmV4CalParams.gsmV4DPDParams.dCalRGIPowerHighBand;
         }
         if(gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation)
         {
            usCalRgi = Determine_PreDist_Cal_RGI( gsmV4CalParams, daCalData, dPreDistCalPower, usBandIdx );
         }
         else
         {
            // use DigGain/RGI method
            usCalRgi = Determine_PreDist_Cal_RGI_from_DPD_AvgPow( gsmV4CalParams.uiChanList[usBandIdx], gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand],gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[usBandIdx],
                dPreDistCalPower, gsmV4CalParams.gsmV4DPDParams.usDigGainForPredistRGI);
         }

         // default the dpdSweepRGI = calRgi
         usDpdSweepRgi = usCalRgi;

          // check if the computed predistortion RGI is in the RGI candidate list
           // use an adjacent RGI, if it is not in the candidate list
           // If an adjacent RGI is unavailable, an exception is thrown
           if ( gsmV4CalParams.gsmV4DPDParams.boolUseAdjacentRGI )
           {
              usDpdSweepRgi = Determine_Adjacent_Cal_RGI( usCalRgi, gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[usBandIdx] );
           }
           if(! gsmV4CalParams.gsmV4DPDParams.boolUseAdjacentRGI && gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation && gsmV4CalParams.gsmV4DPDParams.boolPerformDPD)
           {
               const map<unsigned short, GSM_DA_Cal_DigGain_Record> dutDigGainSweep = gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand][gsmV4CalParams.uiChanList[usBandIdx][0]].digGainSweep;
               if ( dutDigGainSweep.begin()->second.digGainPwr.find(usDpdSweepRgi) == dutDigGainSweep.begin()->second.digGainPwr.end() )
               {
                 //throw exception "DUT digital gain vs. RGI list does not include the predistortion RGI of <usDpdSweepRgi>
               }
           }
         //----------------------------------------------- DA (PCL Error) Processing -----------------------------------------------//

         memset( &calPCLToQMSL, 0, sizeof(QMSL_GSM_V4_PCL_Cal_Result) );

         // copy data from gsmV4_CalParams and gsmV4_PCL_Results to calPCL_ToQMSL
         // ---- channels and PCLs
         calPCLToQMSL.iNumChannels    = gsmV4CalParams.uiNumChan;
         calPCLToQMSL.iNumMeasuredPCL = gsmV4CalParams.gsmV4DAParams.uiNumPCLs;

         for ( uiChanIdx=0; uiChanIdx<gsmV4CalParams.uiNumChan; ++uiChanIdx )
         {
            usCurChan = static_cast<unsigned short>( gsmV4CalParams.uiChanList[usBandIdx][uiChanIdx] );

            calPCLToQMSL.iChannel[uiChanIdx] = usCurChan;

            // ---- PCL indexes and measured power for each index
            for ( usPclIdx=0; usPclIdx < gsmV4CalParams.gsmV4DAParams.uiNumPCLs; ++usPclIdx )
            {
               // Assuming same EDGE and GSM PCL list
               // PCL indexes
               calPCLToQMSL.aiGSM_pclIdx[uiChanIdx][usPclIdx]  = usPclIdx;
               calPCLToQMSL.aiEDGE_pclIdx[uiChanIdx][usPclIdx] = usPclIdx;
               // PCL Delta values
               calPCLToQMSL.aiGSMTxPowerMeasErr[uiChanIdx][usPclIdx]  = gsmV4PCLResults.chainData[ucCurChainID].bandSweepData[usCurBand].gsmPCLSweep[usCurChan].pclPwrDelta[usPclIdx];
               calPCLToQMSL.aiEDGETxPowerMeasErr[uiChanIdx][usPclIdx] = gsmV4PCLResults.chainData[ucCurChainID].bandSweepData[usCurBand].edgePCLSweep[usCurChan].pclPwrDelta[usPclIdx];
            }
         }


         //---------------------------------------------------------- NV GENERATION -----------------------------------------------------------------//

         QLIB_RFCal_GSM_Tx_DA_Cal_Generate_NV_PCL(
            g_hResourceContext,
            usCurBand,
            ucCurChainID,
            (void*)& calPCLToQMSL,
            (void*)& calNV,
            ulOrgPayloadSize,
            ucCacheNVItems ); //cacheNVitmes = 1

         // push the calNV onto the multi-band NV map
         tx_NV_Results.chainNV[ucCurChainID][usCurBand] = calNV;
      }//for (unsigned short usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
   }
}
void Apply_PCL_CorrectionTo_CharDA( GSMDAChar_Chan& charData, GSM_V4_PCL_Results_Channel& gsmV4PCLResults, const vector<unsigned int>& constChanList,
                                    QMSL_DA_Measurement* gsmMeas, QMSL_DA_Measurement* edgeMeas, unsigned int& uiGsmMeasIndex, unsigned int& uiEdgeMeasIndex, GSM_DA_Cal_Results_Channel& daCalData,
                                    unsigned short curBand )
{
   unsigned short usCurChan = 0;
   unsigned short usCurMod  = 0;
   unsigned short usCurPA   = 0;
   unsigned short usCurRgi  = 0;
   unsigned short usNumChan = static_cast<unsigned short>( constChanList.size() );
   double dCurDaCharPower = 0.0;
   double dCurPowerCorr   = 0.0;

   bool boolIsEdge = false;

   // number of PCL indexes for 8PSK - there are less valid PCL indexes for 8PSK than GMSK, so reduce numEdgePCL to eliminate unnecessary searching
   bool boolIsLowBand = true;
   unsigned short usNumEdgePCL = gsmV4_max8PSK_pclIdx_LB;  // default to low band setting

   unsigned short usNumGmskPcl = 16;

   boolIsLowBand = DetermineIfLowBand( curBand );
   if (!boolIsLowBand)
   {
      usNumEdgePCL = gsmV4_max8PSK_pclIdx_HB;
   }

   // used for testing applying the PCL correction using entire range, or only the subset of PCL indexes
   //numEdgePCL = 16;

   QMSL_GSM_Polar_Cal_Frequency nvMappings[3] = {QMSL_POLAR_CAL_F1_CHANNEL, QMSL_POLAR_CAL_F2_CHANNEL, QMSL_POLAR_CAL_F3_CHANNEL};

   map< int, GSMDAChar_PA >::iterator  modIter;
   map< int, GSMDAChar_RGI >::iterator paIter;
   map< int, double >::iterator rgiIter;

   for ( unsigned short usChanIdx=0; usChanIdx < usNumChan; ++usChanIdx )
   {
      usCurChan = static_cast<unsigned short>( constChanList[usChanIdx] );

      for (modIter=charData.data[usCurChan].data.begin(); modIter!=charData.data[usCurChan].data.end(); ++modIter)
      {
         usCurMod = static_cast<unsigned short>(modIter->first);
         if (usCurMod == QMSL_GSM_PwrErr_Per_PCL_8PSK_Type)
         {
            boolIsEdge = true;
         }
         else
         {
            boolIsEdge = false;
         }

         for (paIter=modIter->second.data.begin(); paIter!=modIter->second.data.end(); ++paIter)
         {
            usCurPA = static_cast<unsigned short>(paIter->first);
            for (rgiIter=paIter->second.rgiVsPwr.begin(); rgiIter!=paIter->second.rgiVsPwr.end(); ++rgiIter)
            {
               usCurRgi         = static_cast<unsigned short>(rgiIter->first);
               dCurDaCharPower = rgiIter->second;

               if (boolIsEdge)  // 8PSK
               {
                  /*For each RGI, determine the power error based on the PCL error and add that power error to the RGI power*/
                  dCurPowerCorr = Determine_PCL_Pow_Error_For_DaChar( dCurDaCharPower, usNumEdgePCL, gsmV4PCLResults.edgePCLSweep[usCurChan].pclMeasPwr,
                                                                      gsmV4PCLResults.edgePCLSweep[usCurChan].pclPwrDelta );

                  edgeMeas[uiEdgeMeasIndex].chan        = usCurChan;
                  edgeMeas[uiEdgeMeasIndex].freqMapping = nvMappings[usChanIdx];
                  edgeMeas[uiEdgeMeasIndex].pa          = usCurPA;
                  edgeMeas[uiEdgeMeasIndex].rgi         = usCurRgi;
                  edgeMeas[uiEdgeMeasIndex].power       = dCurDaCharPower + dCurPowerCorr;

                  // add to the EDGE da Cal data
                  daCalData.channelSweepData[usCurChan].edgeDaSweep[usCurPA].rgiPwr[usCurRgi] = edgeMeas[uiEdgeMeasIndex].power;

                  ++uiEdgeMeasIndex;
               }
               else         // GMSK
               {
                  /*For each RGI, determine the power error based on the PCL error and add that power error to the RGI power*/
                  dCurPowerCorr = Determine_PCL_Pow_Error_For_DaChar( dCurDaCharPower, usNumGmskPcl, gsmV4PCLResults.gsmPCLSweep[usCurChan].pclMeasPwr,
                                                                      gsmV4PCLResults.gsmPCLSweep[usCurChan].pclPwrDelta );

                  gsmMeas[uiGsmMeasIndex].chan        = usCurChan;
                  gsmMeas[uiGsmMeasIndex].freqMapping = nvMappings[usChanIdx];
                  gsmMeas[uiGsmMeasIndex].pa          = usCurPA;
                  gsmMeas[uiGsmMeasIndex].rgi         = usCurRgi;
                  gsmMeas[uiGsmMeasIndex].power       = dCurDaCharPower + dCurPowerCorr;
                  
                  // add to the GMSK da Cal data
                  daCalData.channelSweepData[usCurChan].gsmDaSweep[usCurPA].rgiPwr[usCurRgi] = gsmMeas[uiGsmMeasIndex].power;

                  ++uiGsmMeasIndex;
               }
            }//for (rgiIter=paIter->second.rgiVsPwr.begin(); rgiIter!=paIter->second.rgiVsPwr.end(); ++rgiIter)
         }//for (paIter=modIter->second.data.begin(); paIter!=modIter->second.data.end(); ++paIter)
      }//for (modIter=charData.data[usCurChan].data.begin(); modIter!=charData.data[usCurChan].data.end(); ++modIter)
   }//for ( unsigned short usChanIdx=0; usChanIdx < usNumChan; ++usChanIdx )
}
void Process_GSM_Tx_Cx_NV_From_V4_Cal_DA( GSM_V4_Cal_Params& gsmV4CalParams, GSM_V4_PCL_Results_Chain& gsmV4PCLResults,
                                          GSM_V4_DPD_Results_Chain& gsmV4DPDResults, GSM_V4_Tx_Cx_NV_Results& tx_NV_Results )
{
   unsigned short usNumBand = static_cast<unsigned short>( gsmV4CalParams.uiNumBands );

   unsigned short usCurBand = 0;
   unsigned short usCurChan = 0;
   unsigned int uiChanIdx   = 0;
   unsigned int uiOpsPos    = 0;
   unsigned int uiDaqPos    = 0;

   unsigned short usDigGain = 0;

   unsigned int uiGsmMeasIndex = 0, uiEdgeMeasIndex = 0;

   unsigned char ucCurChainID; // = static_cast<unsigned char>( gsmV4CalParams.uiTxChainList[0] );

   QMSL_GSM_Tx_Calibration_NV    calNV;               // calNV struct for a single band/device combination
   QMSL_GSM_EnhLinTx_Cal_Control calControlToQMSL;   // calControl data for passing to QMSL
   QMSL_GSM_EnhLinTx_Cal_Data    calDPDToQMSL;       // DPD data for passing to QMSL

   // cache the NV items for NV write later
   unsigned char ucCacheNVItems = true; //(gsmV4CalParams.m_nv_action & RFCAL_NV_ACTION_QUEUE_NV) ? 1 : 0;

   double dPreDistCalPower     = 0.0;
   // CHARACTERIZED DATA RGI
   // predistoriton RGI. This RGI is used to select the DPD char data.
   unsigned short usCalRgi      = 0;
   // DUT DATA RGI
   // DPD sweep RGI. Nominally equals calRgi. Used to select the DUT dpd sweep data for power/slope adaptation.
   unsigned short usDpdSweepRgi = 0;
   // CHAR DATA RGI
   // Comes into play only when adaptive char DPD data selection is enabled.
   unsigned short usCurRgi     = usCalRgi;

   // the char DAC corresponding to the maximum digital gain output power in the char data set
   float fDacAtMaxDigGain = 0;

   // if true, pick char data set based upon power measurements.
   // if false, pick char data set absed solely on calRgi
   bool boolUseAdaptiveCharRgiSelection = true;
   // if true, adaptively determine the max power delta for power adaptation
   // if false, use the power at the fixed highes digital gain from the tree params for determining the max power delta
   const bool boolUseAdaptiveMaxPowerEstimation = gsmV4CalParams.gsmV4DPDParams.boolDoAdaptiveMaxPwrEst;

   unsigned int uiNumDAC = 2*QMSL_NV_MANAGER_POLAR_CAL_AMAM_TABLE_SIZE;

   QMSL_DA_Measurement gsmMeas[QMSL_GSM_MAX_TX_CAL_CHANNELS*QMSL_GSM_MAX_TX_GAIN_STATES*QMSL_GSM_MAX_RGI_STEPS];
   QMSL_DA_Measurement edgeMeas[QMSL_GSM_MAX_TX_CAL_CHANNELS*QMSL_GSM_MAX_TX_GAIN_STATES*QMSL_GSM_MAX_RGI_STEPS];

   for (unsigned char curChainIdx=0; curChainIdx<gsmV4CalParams.uiNumTxChains; ++curChainIdx)
   {
      ucCurChainID = gsmV4CalParams.uiTxChainList[curChainIdx];

      //-------------------------------------------------- START BAND LOOP ------------------------------------------------------------------//
      for (unsigned short usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
      {
         // the current band
         usCurBand = static_cast<unsigned short>( gsmV4CalParams.uiBandList[usBandIdx] );

         //--------------------------------------- DA (Characterized RGI power) Processing ------------------------------------------//
         // apply PCL error correction to char DA data and rewrite to the NV, instead of using PCL errors
         GSMDAChar_Chan charData;
         GSMDAChar_Mod charModDataPerChannel;
         GSMDAChar_PA charPADataPerMod;
         GSMDAChar_RGI tempRGIPwrMap;

         //In this example, only one band, one chain info is populated.
         unsigned short usPAIdx, usRgiIdx, usModIdx;
         std::map<int,double>::iterator Iter1;
         std::map< int, GSMDAChar_RGI >::iterator Iter2;
         std::map< int, GSMDAChar_PA >::iterator Iter3;
         /*In this demo code, char DA data is stored in charDAPwr array for convenience. This is read from XML file in real scenario.*/
         for ( uiChanIdx=0; uiChanIdx<gsmV4CalParams.uiNumChan; ++uiChanIdx )
         {
                unsigned int uiCurChan = gsmV4CalParams.uiChanList[usBandIdx][uiChanIdx];
            charModDataPerChannel.data.clear();
            Iter3 = charModDataPerChannel.data.begin();
            for(usModIdx = 0 ; usModIdx < gsmV4CalParams.gsmV4DAParams.uiNumMods; usModIdx++)
            {
               charPADataPerMod.data.clear();
               Iter2 = charPADataPerMod.data.begin();
               for(usPAIdx = 0;  usPAIdx < 2 /*2 PA states*/; usPAIdx++)
               {
                  tempRGIPwrMap.rgiVsPwr.clear();
                  Iter1 = tempRGIPwrMap.rgiVsPwr.begin();
                  for(usRgiIdx = 0; usRgiIdx < gsmV4_maxNumRGI/*num RGI*/; usRgiIdx++)
                  {
                     tempRGIPwrMap.rgiVsPwr.insert(Iter1,std::pair<int,double>(usRgiIdx, ((double)charDAPwr[uiChanIdx][usModIdx][usPAIdx][usRgiIdx])));
                     Iter1 = tempRGIPwrMap.rgiVsPwr.end();
                  }
                  charPADataPerMod.data.insert(Iter2,std::pair<int,GSMDAChar_RGI>(usPAIdx,tempRGIPwrMap));
                  Iter2 = charPADataPerMod.data.end();
               }//for(usPAIdx = 0;  usPAIdx < 2 /*2 PA states*/; usPAIdx++)
               charModDataPerChannel.data.insert(Iter3,std::pair<int,GSMDAChar_PA>(usModIdx,charPADataPerMod));
               Iter3 = charModDataPerChannel.data.end();
            }//for(usModIdx = 0 ; usModIdx < gsmV4CalParams.gsmV4DAParams.uiNumMods; usModIdx++)
            charData.data.insert(charData.data.end(),std::pair<int,GSMDAChar_Mod>(uiCurChan,charModDataPerChannel));
         }//for ( uiChanIdx=0; uiChanIdx<gsmV4CalParams.uiNumChan; ++uiChanIdx )

         tempRGIPwrMap.rgiVsPwr.clear();
         charPADataPerMod.data.clear();
         charModDataPerChannel.data.clear();

         if (charData.data.size() == 0)
         {
            //throw exception "Char DA data is empty. Please check the char XML input."
         }

         //------ Apply PCL error corrections to characterized DA Cal data
         // this will apply a PCL error correction to the PMEAS data for the deisred PA state and Modulation

         // positions in array gsmMeas, edgeMeas being filled
         uiGsmMeasIndex  = 0;
         uiEdgeMeasIndex = 0;

         // this data will be written to the NV
         GSM_DA_Cal_Results_Channel daCalData;
         Apply_PCL_CorrectionTo_CharDA( charData, gsmV4PCLResults.chainData[ucCurChainID].bandSweepData[usCurBand], gsmV4CalParams.uiChanList[usBandIdx],
                                        gsmMeas, edgeMeas, uiGsmMeasIndex, uiEdgeMeasIndex, daCalData, usCurBand );

         //--------------------------------------------- Determine Predistortion RGI -----------------------------------------------//
         dPreDistCalPower = gsmV4CalParams.gsmV4DPDParams.dCalRGIPowerHighBand; // default to high-band power

         if ( DetermineIfLowBand( usCurBand ) )
         {
            dPreDistCalPower = gsmV4CalParams.gsmV4DPDParams.dCalRGIPowerLowBand;
         }

         if(gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation)
         {
            usCalRgi = Determine_PreDist_Cal_RGI( gsmV4CalParams, daCalData, dPreDistCalPower, usBandIdx );
         }
         else
         {
         // use DigGain/RGI method
            usCalRgi = Determine_PreDist_Cal_RGI_from_DPD_AvgPow( gsmV4CalParams.uiChanList[usBandIdx], gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand],gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[usBandIdx],
                                                        dPreDistCalPower, gsmV4CalParams.gsmV4DPDParams.usDigGainForPredistRGI );
         }

         // default the dpdSweepRGI = calRgi
         usDpdSweepRgi = usCalRgi;

            // check if the computed predistortion RGI is in the RGI candidate list
           // use an adjacent RGI, if it is not in the candidate list
           // If an adjacent RGI is unavailable, an exception is thrown
           if ( gsmV4CalParams.gsmV4DPDParams.boolUseAdjacentRGI )
           {
              usDpdSweepRgi = Determine_Adjacent_Cal_RGI( usCalRgi, gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[usBandIdx] );
           }
           if(! gsmV4CalParams.gsmV4DPDParams.boolUseAdjacentRGI && gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation && gsmV4CalParams.gsmV4DPDParams.boolPerformDPD)
           {
               const map<unsigned short, GSM_DA_Cal_DigGain_Record> dutDigGainSweep = gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand][gsmV4CalParams.uiChanList[usBandIdx][0]].digGainSweep;
               if ( dutDigGainSweep.begin()->second.digGainPwr.find(usDpdSweepRgi) == dutDigGainSweep.begin()->second.digGainPwr.end() )
               {
                 //throw exception "DUT digital gain vs. RGI list does not include the predistortion RGI of <usDpdSweepRgi>
               }
           }
         //---------------------------------------------------------- DPD Processing -----------------------------------------------------------------//

         memset( &calControlToQMSL, 0, sizeof(QMSL_GSM_EnhLinTx_Cal_Control) );
         memset( &calDPDToQMSL, 0, sizeof(QMSL_GSM_EnhLinTx_Cal_Data) );

         calControlToQMSL.bSlopeCorrAlignLow     = gsmV4CalParams.gsmV4DPDParams.boolSlopeAlignLow;
         calControlToQMSL.bDoPowerAdaptation     = gsmV4CalParams.gsmV4DPDParams.boolDoPowerAdaptation;
         calControlToQMSL.bDoSlopeCorrection     = gsmV4CalParams.gsmV4DPDParams.boolDoSlopeCorrection;
         calControlToQMSL.bDoPiecewiseAdaptation = gsmV4CalParams.gsmV4DPDParams.boolDoPieceWiseLinear;
         calControlToQMSL.iAlgType               = 1;

         calDPDToQMSL.iCalRgi = usCalRgi;

#if 0
         // GSMV4 - low band power, as seen before, is strikingly different when using GMSK modulation, as compared with other bands
         // This causes the selected calRgi to be 1 higher than it should be, when GMSK modulation is used to select the predistortion RGI, via digital gain of 47.
         if (( usCurBand == NV_MODE_GSM_EGSM || usCurBand == NV_MODE_GSM_850))
         {
            calDPDToQMSL.iCalRgi -= 1;   // only modify the RGI written to NV. Still use the same calRgi for char data selection
         }
#endif
         calDPDToQMSL.dDacScale    = static_cast<float>( gsmV4CalParams.gsmV4DPDParams.dDacScale );
         calDPDToQMSL.iNumChannels = gsmV4CalParams.uiNumChan;

         BAND_CLASS_ENUM bandEnum = ConverBand_To_BandClassEnum( usCurBand );

         // Pack characterized data
         for (uiChanIdx=0; uiChanIdx < calDPDToQMSL.iNumChannels; ++uiChanIdx)
         {
            calDPDToQMSL.iFreqMapping[uiChanIdx] = static_cast<QMSL_GSM_Polar_Cal_Frequency>( gsmV4CalParams.gsmV4DPDParams.uiNvChanMap[uiChanIdx] );
            usCurChan = static_cast<unsigned short>( gsmV4CalParams.uiChanList[usBandIdx][uiChanIdx] );

            vector<struct opPwrRecord> ops;
            vector<struct digRecord>   daqs;

            usCurRgi = Select_CharDPD_DataSet( usCalRgi, usCurBand, usCurChan, ucCurChainID, boolUseAdaptiveCharRgiSelection, ops, daqs,
                                               gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand][usCurChan].digGainSweep, true );

            if (boolUseAdaptiveMaxPowerEstimation)
            {
               bool boolApplyMaxDigGainOffset = false;
               fDacAtMaxDigGain = DetermineDacAtDigGainPwr( ops, daqs[daqs.size()-1].pwr, uiNumDAC );
               // Estimate the DUT power at the maximum char Digital Gain
               // Function returns the esimated power, but also adds it to the DUT digGainSweep
               EstimatePowerAtMaxDigGain( ops[uiNumDAC-1].amam, ops[uiNumDAC-1].oppwr, daqs, gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand][usCurChan].digGainSweep,
                        usDpdSweepRgi, fDacAtMaxDigGain, /*boolApplyMaxDigGainOffset,*/ 
                                          gsmV4CalParams.gsmV4DPDParams.usDigGainForPredistRGI, calDPDToQMSL.dDacScale );
            }
            // Sanity check number of records
            if (ops.size() != uiNumDAC)
            {
               //throw exception "Input characterization curve does not have expected number of points"
            }

            // Pack characterized predistortion curve
            for(uiOpsPos=0; uiOpsPos < uiNumDAC; ++uiOpsPos)
            {
               calDPDToQMSL.aCharCurves[uiChanIdx].dCharDac[uiOpsPos]   = ops[uiOpsPos].amam;
               calDPDToQMSL.aCharCurves[uiChanIdx].dCharPower[uiOpsPos] = ops[uiOpsPos].oppwr;
               calDPDToQMSL.aCharCurves[uiChanIdx].dCharAmpm[uiOpsPos]  = ops[uiOpsPos].ampm;
            }

            //------------------------ Pack characterized digital gain table ------------------------//
            bool boolUseAllPoints = gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].empty();

            if (daqs.size() > QMSL_NV_MANAGER_ENH_CAL_DIG_GAIN_TABLE_SIZE)
            {
               //throw exception "NUmber of entries is more than 64"
            }

            calDPDToQMSL.aCharDigGain[uiChanIdx].iNumEntries = daqs.size();

            for ( uiDaqPos=0; uiDaqPos < daqs.size(); ++uiDaqPos )
            {
               usDigGain = static_cast<unsigned short>( daqs[uiDaqPos].digGain );

               bool boolUsePoint = find( gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].begin(),
                                         gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].end(),
                                         usDigGain )
                                   != gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].end();

               calDPDToQMSL.aCharDigGain[uiChanIdx].iDigGain[uiDaqPos] = usDigGain;
               calDPDToQMSL.aCharDigGain[uiChanIdx].dPower[uiDaqPos]   = daqs[uiDaqPos].pwr;
               calControlToQMSL.bSlopeCorrUseCharPoint[uiDaqPos]     = boolUsePoint || boolUseAllPoints;
            }

            //------------------------------- Pack measured digital gains from DUT ------------------------------//
            calDPDToQMSL.aDutDigGain[uiChanIdx].iNumEntries = 0;
            unsigned int uiDigGainTablePos = 0;
            GSM_V4_DPD_Sweep_Record* channelSweepResults = &gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand][usCurChan];

            map<unsigned short, GSM_DA_Cal_DigGain_Record>::iterator digGainIter;

            for (digGainIter= channelSweepResults->digGainSweep.begin(); digGainIter!=channelSweepResults->digGainSweep.end(); ++digGainIter )
            {
               if (digGainIter->second.digGainPwr.find(usDpdSweepRgi) != digGainIter->second.digGainPwr.end())
               {
                  usDigGain = static_cast<unsigned short>( digGainIter->first );

                  bool boolUsePoint = find( gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].begin(),
                                            gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].end(),
                                            usDigGain )
                                      != gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[usBandIdx].end();

                  if (uiDigGainTablePos > QMSL_NV_MANAGER_ENH_CAL_DIG_GAIN_TABLE_SIZE)
                  {
                     //throw exception "NUmber of entries is more than 64"
                  }
                  calDPDToQMSL.aDutDigGain[uiChanIdx].iDigGain[uiDigGainTablePos] = usDigGain;
                  calDPDToQMSL.aDutDigGain[uiChanIdx].dPower[uiDigGainTablePos] = static_cast<float>( digGainIter->second.digGainPwr[usDpdSweepRgi] );
                  calControlToQMSL.bSlopeCorrUseDutPoint[uiDigGainTablePos] = boolUsePoint || boolUseAllPoints;
                  calDPDToQMSL.aDutDigGain[uiChanIdx].iNumEntries += 1;
                  uiDigGainTablePos += 1;
               }//if (digGainIter->second.digGainPwr.find(usDpdSweepRgi) != digGainIter->second.digGainPwr.end())
            }//for (digGainIter= channelSweepResults->digGainSweep.begin(); digGainIter!=channelSweepResults->digGainSweep.end(); ++digGainIter )
            if ( calDPDToQMSL.aDutDigGain[uiChanIdx].iNumEntries < 1 && gsmV4CalParams.gsmV4DPDParams.boolDoPowerAdaptation )
            {
               //throw exception "DUT digital gain table should contain atleast 1 entry to do power adapation"
            }
            if ( calDPDToQMSL.aDutDigGain[uiChanIdx].iNumEntries < 2 && (gsmV4CalParams.gsmV4DPDParams.boolDoSlopeCorrection || gsmV4CalParams.gsmV4DPDParams.boolDoPieceWiseLinear) )
            {
               //throw exception "Need atleast 2 entries in DUT table for slope correction/piecewise linear"
            }
            if ( calDPDToQMSL.aCharDigGain[uiChanIdx].iNumEntries < 1 && gsmV4CalParams.gsmV4DPDParams.boolDoPowerAdaptation )
            {
               //throw exception "Need at least 1 entry in char table for power adaption"
            }
            if ( calDPDToQMSL.aCharDigGain[uiChanIdx].iNumEntries < 2 && (gsmV4CalParams.gsmV4DPDParams.boolDoSlopeCorrection || gsmV4CalParams.gsmV4DPDParams.boolDoPieceWiseLinear) )
            {
               //throw exception "Need atleast 2 entries in char table for slope correction/piecewise linear"
            }
         }//for (uiChanIdx=0; uiChanIdx < calDPDToQMSL.iNumChannels; ++uiChanIdx)

         QLIB_RFCal_GSM_Tx_Enh_Cal_Generate_NV(
            g_hResourceContext,
            usCurBand,
            ucCurChainID,
            uiGsmMeasIndex,
            (void*)& gsmMeas,
            uiEdgeMeasIndex,
            (void*)& edgeMeas,
            (void*)& calControlToQMSL,
            (void*)& calDPDToQMSL,
            (void*)& calNV,
            ucCacheNVItems );

         // push the calNV onto the multi-band NV map
         tx_NV_Results.chainNV[ucCurChainID][usCurBand] = calNV;
      }// for (unsigned short usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
   } //chain loop
}

void Pack_GSM_DA_For_QMSL( GSM_V4_Cal_Params& gsmV4CalParams, unsigned int curBandIdx, QMSL_GSM_DA_Cal_Result& daCalToQMSL, GSM_DA_Cal_Results_Channel& daCalData )
{

   //------------------ copy the DA cal data over to the required QMSL struct -----------------------//
   daCalToQMSL.iNumChannels               = gsmV4CalParams.uiNumChan;
   daCalToQMSL.iSaturationDetectionWindow = 0.0;   // set to zero for now, unless we intend to use this later.

   map<unsigned short,GSM_DA_Cal_Sweep_Record>::iterator paIter;
   map<unsigned short,double>::iterator                  rgiIter;

   unsigned short numPA  = 0;
   unsigned short numRGI = 0;;

   for ( unsigned short chanIdx=0; chanIdx<gsmV4CalParams.uiNumChan; ++chanIdx )
   {
      unsigned short curChan = static_cast<unsigned short>( gsmV4CalParams.uiChanList[curBandIdx][chanIdx] );

      daCalToQMSL.iChannel[chanIdx]     = curChan;
      daCalToQMSL.iFreqMapping[chanIdx] = (QMSL_GSM_Polar_Cal_Frequency)gsmV4CalParams.gsmV4DPDParams.uiNvChanMap[chanIdx];

      numPA = static_cast<unsigned short>(daCalData.channelSweepData[curChan].edgeDaSweep.size() );

      daCalToQMSL.iNumMeasuredTxGainStates = numPA;

      paIter = daCalData.channelSweepData[curChan].edgeDaSweep.begin();
            
      for( unsigned short paIdx = 0; paIdx<numPA; ++paIdx, ++paIter )
      {
         unsigned short curPA                   = paIter->first;
         daCalToQMSL.aiTxMeasToNVMapping[paIdx] = curPA;

         GSM_DA_Cal_Sweep_Record gsmSweep  = daCalData.channelSweepData[curChan].gsmDaSweep[curPA];
         GSM_DA_Cal_Sweep_Record edgeSweep = daCalData.channelSweepData[curChan].edgeDaSweep[curPA];

         unsigned int rgiIdx = 0;
         numRGI = static_cast<unsigned short>( gsmSweep.rgiPwr.size() );

         for( rgiIter = gsmSweep.rgiPwr.begin(); rgiIter!=gsmSweep.rgiPwr.end(); ++rgiIter )
         {
            daCalToQMSL.aiGSMTxRGI[chanIdx][paIdx][rgiIdx] = rgiIter->first;
            daCalToQMSL.aiGSMTxPowerMeas[chanIdx][paIdx][rgiIdx] = rgiIter->second;
            ++rgiIdx;
         }

         rgiIdx = 0;
         for( rgiIter = edgeSweep.rgiPwr.begin(); rgiIter != edgeSweep.rgiPwr.end(); ++rgiIter )
         {
            daCalToQMSL.aiEDGETxRGI[chanIdx][paIdx][rgiIdx] = rgiIter->first;
            daCalToQMSL.aiEDGETxPowerMeas[chanIdx][paIdx][rgiIdx] = rgiIter->second;
            ++rgiIdx;
         }
      }
   }

   // update the non-channel dependent values
   daCalToQMSL.iNumMeasuredTxGainStates        = numPA;
   daCalToQMSL.iNumTxPowerMeasurementsPerRange = numRGI; 
   
}

void Process_GSM_Tx_Cx_NV_From_V4_Cal_DA_Only( GSM_V4_Cal_Params& gsmV4CalParams, GSM_V4_PCL_Results_Chain& gsmV4PCLResults,
   GSM_V4_DPD_Results_Chain& gsmV4DPDResults, GSM_V4_Tx_Cx_NV_Results& tx_NV_Results )
{
   unsigned short usNumBand = static_cast<unsigned short>( gsmV4CalParams.uiNumBands );

   unsigned short usCurBand = 0;
   unsigned short usCurChan = 0;
   unsigned int uiChanIdx   = 0;
   unsigned int uiOpsPos    = 0;
   unsigned int uiDaqPos    = 0;

   unsigned short usDigGain = 0;

   unsigned int uiGsmMeasIndex = 0, uiEdgeMeasIndex = 0;

   unsigned char ucCurChainID; // = static_cast<unsigned char>( gsmV4CalParams.uiTxChainList[0] );

   QMSL_GSM_Tx_Calibration_NV    calNV;               // calNV struct for a single band/device combination

   // cache the NV items for NV write later
   unsigned char ucCacheNVItems = true; //(gsmV4CalParams.m_nv_action & RFCAL_NV_ACTION_QUEUE_NV) ? 1 : 0;

   double dPreDistCalPower     = 0.0;
   // CHARACTERIZED DATA RGI
   // predistoriton RGI. This RGI is used to select the DPD char data.
   unsigned short usCalRgi      = 0;
   // DUT DATA RGI
   // DPD sweep RGI. Nominally equals calRgi. Used to select the DUT dpd sweep data for power/slope adaptation.
   unsigned short usDpdSweepRgi = 0;
   // CHAR DATA RGI
   // Comes into play only when adaptive char DPD data selection is enabled.
   unsigned short usCurRgi     = usCalRgi;

   // the char DAC corresponding to the maximum digital gain output power in the char data set
   float fDacAtMaxDigGain = 0;

   // if true, pick char data set based upon power measurements.
   // if false, pick char data set absed solely on calRgi
   bool boolUseAdaptiveCharRgiSelection = true;
   // if true, adaptively determine the max power delta for power adaptation
   // if false, use the power at the fixed highes digital gain from the tree params for determining the max power delta
   const bool boolUseAdaptiveMaxPowerEstimation = gsmV4CalParams.gsmV4DPDParams.boolDoAdaptiveMaxPwrEst;

   unsigned int uiNumDAC = 2*QMSL_NV_MANAGER_POLAR_CAL_AMAM_TABLE_SIZE;

   QMSL_DA_Measurement gsmMeas[QMSL_GSM_MAX_TX_CAL_CHANNELS*QMSL_GSM_MAX_TX_GAIN_STATES*QMSL_GSM_MAX_RGI_STEPS];
   QMSL_DA_Measurement edgeMeas[QMSL_GSM_MAX_TX_CAL_CHANNELS*QMSL_GSM_MAX_TX_GAIN_STATES*QMSL_GSM_MAX_RGI_STEPS];

   QMSL_GSM_DA_Cal_Result        daCalToQMSL;            // daCal data for sending to QMSL

   for (unsigned char curChainIdx=0; curChainIdx<gsmV4CalParams.uiNumTxChains; ++curChainIdx)
   {
      ucCurChainID = gsmV4CalParams.uiTxChainList[curChainIdx];

      //-------------------------------------------------- START BAND LOOP ------------------------------------------------------------------//
      for (unsigned short usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
      {
         // the current band
         usCurBand = static_cast<unsigned short>( gsmV4CalParams.uiBandList[usBandIdx] );

         //--------------------------------------- DA (Characterized RGI power) Processing ------------------------------------------//
         // apply PCL error correction to char DA data and rewrite to the NV, instead of using PCL errors
         GSMDAChar_Chan charData;
         GSMDAChar_Mod charModDataPerChannel;
         GSMDAChar_PA charPADataPerMod;
         GSMDAChar_RGI tempRGIPwrMap;

         //In this example, only one band, one chain info is populated.
         unsigned short usPAIdx, usRgiIdx, usModIdx;
         std::map<int,double>::iterator Iter1;
         std::map< int, GSMDAChar_RGI >::iterator Iter2;
         std::map< int, GSMDAChar_PA >::iterator Iter3;
         /*In this demo code, char DA data is stored in charDAPwr array for convenience. This is read from XML file in real scenario.*/
         for ( uiChanIdx=0; uiChanIdx<gsmV4CalParams.uiNumChan; ++uiChanIdx )
         {
                unsigned int uiCurChan = gsmV4CalParams.uiChanList[usBandIdx][uiChanIdx];
            charModDataPerChannel.data.clear();
            Iter3 = charModDataPerChannel.data.begin();
            for(usModIdx = 0 ; usModIdx < gsmV4CalParams.gsmV4DAParams.uiNumMods; usModIdx++)
            {
               charPADataPerMod.data.clear();
               Iter2 = charPADataPerMod.data.begin();
               for(usPAIdx = 0;  usPAIdx < 2 /*2 PA states*/; usPAIdx++)
               {
                  tempRGIPwrMap.rgiVsPwr.clear();
                  Iter1 = tempRGIPwrMap.rgiVsPwr.begin();
                  for(usRgiIdx = 0; usRgiIdx < gsmV4_maxNumRGI/*num RGI*/;usRgiIdx++)
                  {
                     tempRGIPwrMap.rgiVsPwr.insert(Iter1,std::pair<int,double>(usRgiIdx, ((double)charDAPwr[uiChanIdx][usModIdx][usPAIdx][usRgiIdx])));
                     Iter1 = tempRGIPwrMap.rgiVsPwr.end();
                  }
                  charPADataPerMod.data.insert(Iter2,std::pair<int,GSMDAChar_RGI>(usPAIdx,tempRGIPwrMap));
                  Iter2 = charPADataPerMod.data.end();
               }//for(usPAIdx = 0;  usPAIdx < 2 /*2 PA states*/; usPAIdx++)
               charModDataPerChannel.data.insert(Iter3,std::pair<int,GSMDAChar_PA>(usModIdx,charPADataPerMod));
               Iter3 = charModDataPerChannel.data.end();
            }//for(usModIdx = 0 ; usModIdx < gsmV4CalParams.gsmV4DAParams.uiNumMods; usModIdx++)
            charData.data.insert(charData.data.end(),std::pair<int,GSMDAChar_Mod>(uiCurChan,charModDataPerChannel));
         }//for ( uiChanIdx=0; uiChanIdx<gsmV4CalParams.uiNumChan; ++uiChanIdx )

         tempRGIPwrMap.rgiVsPwr.clear();
         charPADataPerMod.data.clear();
         charModDataPerChannel.data.clear();

         if (charData.data.size() == 0)
         {
            //throw exception "Char DA data is empty. Please check the char XML input."
         }

         //------ Apply PCL error corrections to characterized DA Cal data
         // this will apply a PCL error correction to the PMEAS data for the deisred PA state and Modulation

         // positions in array gsmMeas, edgeMeas being filled
         uiGsmMeasIndex  = 0;
         uiEdgeMeasIndex = 0;

         // this data will be written to the NV
         GSM_DA_Cal_Results_Channel daCalData;
         Apply_PCL_CorrectionTo_CharDA( charData, gsmV4PCLResults.chainData[ucCurChainID].bandSweepData[usCurBand], gsmV4CalParams.uiChanList[usBandIdx],
            gsmMeas, edgeMeas, uiGsmMeasIndex, uiEdgeMeasIndex, daCalData, usCurBand );

         //--------------------------------------------- Determine Predistortion RGI -----------------------------------------------//
         dPreDistCalPower = gsmV4CalParams.gsmV4DPDParams.dCalRGIPowerHighBand; // default to high-band power

         if ( DetermineIfLowBand( usCurBand ) )
         {
            dPreDistCalPower = gsmV4CalParams.gsmV4DPDParams.dCalRGIPowerLowBand;
         }

         if(gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation)
         {
            usCalRgi = Determine_PreDist_Cal_RGI( gsmV4CalParams, daCalData, dPreDistCalPower, usBandIdx );
         }
         else
         {
            // use DigGain/RGI method
            usCalRgi = Determine_PreDist_Cal_RGI_from_DPD_AvgPow( gsmV4CalParams.uiChanList[usBandIdx], gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand],gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[usBandIdx],
                dPreDistCalPower, gsmV4CalParams.gsmV4DPDParams.usDigGainForPredistRGI);
         }

         // default the dpdSweepRGI = calRgi
         usDpdSweepRgi = usCalRgi;

          // check if the computed predistortion RGI is in the RGI candidate list
           // use an adjacent RGI, if it is not in the candidate list
           // If an adjacent RGI is unavailable, an exception is thrown
           if ( gsmV4CalParams.gsmV4DPDParams.boolUseAdjacentRGI )
           {
              usDpdSweepRgi = Determine_Adjacent_Cal_RGI( usCalRgi, gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[usBandIdx] );
           }

           if(! gsmV4CalParams.gsmV4DPDParams.boolUseAdjacentRGI && gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation && gsmV4CalParams.gsmV4DPDParams.boolPerformDPD)
           {
               const map<unsigned short, GSM_DA_Cal_DigGain_Record> dutDigGainSweep = gsmV4DPDResults.chainData[ucCurChainID].band_Chan_sweepResults[usCurBand][gsmV4CalParams.uiChanList[usBandIdx][0]].digGainSweep;
               if ( dutDigGainSweep.begin()->second.digGainPwr.find(usDpdSweepRgi) == dutDigGainSweep.begin()->second.digGainPwr.end() )
               {
                 //throw exception "DUT digital gain vs. RGI list does not include the predistortion RGI of <usDpdSweepRgi>
               }
           }
         Pack_GSM_DA_For_QMSL(gsmV4CalParams, usBandIdx,daCalToQMSL,daCalData);

         QLIB_RFCal_GSM_Tx_DA_Cal_Generate_NV(
            g_hResourceContext,
            usCurBand,
            ucCurChainID,
           (void*)& daCalToQMSL,
           (void*)& calNV,
            ucCacheNVItems );

         // push the calNV onto the multi-band NV map
         tx_NV_Results.chainNV[ucCurChainID][usCurBand] = calNV;
      }// for (unsigned short usBandIdx = 0; usBandIdx < usNumBand; ++usBandIdx)
   } //chain loop
}
void Process_Ind_Rx_Cx_NV( unsigned char cacheNVItems, GSM_V4_Rx_Cx_NV_Results& rx_NV_Results, GSM_V4_Cal_Params& gsmV4CalParams,
   GSM_V4_RSSI_Est_Chain& gsmV4RSSIResults, GSM_V4_Rx_Cx_Cal_Meas& rx_Cal_Meas, unsigned int chainID, unsigned int curBand, bool boolIsHiLin)
{
   map< int, vector<GSM_RX_MEAS_TYPE> >::const_iterator itr;
   vector<int> lnaPowerList = boolIsHiLin? gsmV4CalParams.gsmV4RxParams.iLNAPowerListHiLin : gsmV4CalParams.gsmV4RxParams.iRxPowerList;

   unsigned char gainIdx = 0;
   unsigned char chanIdx = 0;

   // fill band_rx_cal_meas
   rx_Cal_Meas.chainMeas[chainID][curBand].iChainId        = static_cast<unsigned char>(chainID);
   rx_Cal_Meas.chainMeas[chainID][curBand].iNvContainer    = static_cast<unsigned char>(rx_Cal_Meas.chainMeas[chainID][curBand].iChainId);

   rx_Cal_Meas.chainMeas[chainID][curBand].iNumChannel     = gsmV4RSSIResults.chainData[chainID].estRSSI_Map[curBand].size();
   rx_Cal_Meas.chainMeas[chainID][curBand].iNumRxGainRange = gsmV4RSSIResults.chainData[chainID].estRSSI_Map[curBand].begin()->second.size();

   for(itr =  gsmV4RSSIResults.chainData[chainID].estRSSI_Map[curBand].begin();
         itr != gsmV4RSSIResults.chainData[chainID].estRSSI_Map[curBand].end(); ++itr)
   {
      rx_Cal_Meas.chainMeas[chainID][curBand].aiChannelList[chanIdx] = static_cast<unsigned short>(itr->first);

      for(gainIdx=0; gainIdx < itr->second.size(); ++gainIdx)
      {
         rx_Cal_Meas.chainMeas[chainID][curBand].aiRSSIValue[gainIdx][chanIdx] = itr->second[gainIdx].m_rssi;
         rx_Cal_Meas.chainMeas[chainID][curBand].aiCalibrationPwr[gainIdx]     = static_cast<double>(lnaPowerList[gainIdx] );
      }
      ++chanIdx;
   }

   if( boolIsHiLin )
   {
      QLIB_GSM_RX_CALIBRATION_GENERATE_NV_HI_LIN(
         g_hResourceContext,
         static_cast<unsigned short>(curBand),
         (void*)&rx_Cal_Meas.chainMeas[chainID][curBand],
         (void*)&rx_NV_Results.chainNV[chainID][curBand],
         cacheNVItems
      );
   }
   else
   {
      QLIB_GSM_RX_CALIBRATION_GENERATE_NV(
         g_hResourceContext,
         static_cast<unsigned short>(curBand),
         (void*)&rx_Cal_Meas.chainMeas[chainID][curBand],
         (void*)&rx_NV_Results.chainNV[chainID][curBand],
         cacheNVItems
      );
   }
}

//! Process_GSM_Rx_Cx_NV_From_V4_Cal
void Process_GSM_Rx_Cx_NV_From_V4_Cal( GSM_V4_Rx_Cx_NV_Results& rx_NV_Results, GSM_V4_Cal_Params& gsmV4CalParams,
   GSM_V4_RSSI_Est_Chain& gsmV4RSSIResults, GSM_V4_Rx_Cx_Cal_Meas& rx_Cal_Meas, bool boolIsHiLin)
{
   map< int, vector<GSM_RX_MEAS_TYPE> >::const_iterator itr;

   unsigned char ucGainIdx = 0;
   unsigned char ucChanIdx = 0;
   unsigned int uiCurBand  = 0;
   unsigned int uiChainIdx = 0;
   unsigned int dRxChain = 0;

   // cache the NV items for NV write later
   unsigned char ucCacheNVItems = true;// (gsmV4CalParams.m_nv_action & RFCAL_NV_ACTION_QUEUE_NV) ? 1 : 0;

   unsigned int curChainID;
   for (unsigned int uiBandIdx = 0; uiBandIdx < gsmV4CalParams.uiNumBands; ++uiBandIdx)
   {
      uiCurBand = gsmV4CalParams.uiBandList[uiBandIdx];

      for (uiChainIdx = 0; uiChainIdx < gsmV4CalParams.uiNumRxChains; ++uiChainIdx)
      {

         curChainID = gsmV4CalParams.uiRxChainList[uiChainIdx];
        Process_Ind_Rx_Cx_NV( ucCacheNVItems, rx_NV_Results, gsmV4CalParams, gsmV4RSSIResults, rx_Cal_Meas, curChainID, uiCurBand, boolIsHiLin);
        if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
            {
            dRxChain = curChainID + 1;
            Process_Ind_Rx_Cx_NV( ucCacheNVItems, rx_NV_Results, gsmV4CalParams, gsmV4RSSIResults, rx_Cal_Meas, dRxChain, uiCurBand, boolIsHiLin);
            }
      }//for (uiChainIdx = 0; uiChainIdx < gsmV4CalParams.uiNumRxChains; ++uiChainIdx)
   }//for (unsigned int uiBandIdx = 0; uiBandIdx < gsmV4CalParams.uiNumBands; ++uiBandIdx)
}

/*Initializing the DPD char table to be used for calculations later on*/
/*
<GSMCharDPD_Table rf_mode="10" Channel="975" RGI="19" chain="0" instance="0">
<OpPwrTable>
<OpRec AMAM="293.392" Pwr="-13.1137" AMPM="733.959"/>
<OpRec AMAM="622.782" Pwr="-7.09307" AMPM="699.557"/>
<OpRec AMAM="967.537" Pwr="-3.57124" AMPM="639.985"/>
...
<OpRec AMAM="40981.3" Pwr="28.8937" AMPM="333.674"/>
<OpRec AMAM="41271.5" Pwr="28.9624" AMPM="332.165"/>
<OpRec AMAM="41558.9" Pwr="29.03" AMPM="330.682"/>
</OpPwrTable>
<DaqTable>
<OpRec DigGain="5" Pwr="5.9"/>
<OpRec DigGain="15" Pwr="14.8"/>
<OpRec DigGain="40" Pwr="24.325"/>
<OpRec DigGain="60" Pwr="28.45"/>
</DaqTable>
</GSMCharDPD_Table>
*/

void InitCharV4Table()
{
   /*Char Table is hard coded here. For actual tool developement, values are read from the XML file using a parser*/
   //char DPD
   char_DPD_Tbl_V3_V4 charV4TableV3_V4[3][4] = {{{
            975,18,{280.715,599.014,932.282,1276.05,1627.64,1985.88,2349.49,2717.86,3091.34,3468.45,3854.03,4248.23,4649.25,5056.6,5469.6,5886.35,6304.9,6724.05,7141.83,7556.87,7969.35,8379.24,8786.49,9190.03,9589.89,9985.74,10376.3,10763.6,11145.7,11524,11898.2,12269.1,12636.8,13001.6,13363.6,13723,14080,14434.3,14785.6,15134.8,15481.2,15825.1,16165.7,16504.5,16840.6,17173.4,17504.7,17833.3,18159.5,18483.7,18806.5,19126.8,19445.2,19762.7,20078.6,20392.9,20705.4,21017.4,21328.2,21637.8,21945.9,22253.3,22560,22865.7,23170.7,23474.6,23778.1,24081.1,24383,24684.7,24986,25286.5,25586.5,25886.2,26185.8,26484.7,26782.9,27081.7,27379.9,27677.7,27974.5,28271.2,28567.9,28864.1,29160.2,29455.2,29749.6,30043.9,30336.8,30629.6,30921.5,31212.3,31503.1,31793.1,32081.6,32368.9,32655.7,32941.9,33227.3,33512.6,33797.1,34079.2,34360.5,34642,34924.3,35205.8,35487.4,35768,36048.3,36327.6,36608.5,36891.3,37174.2,37456.9,37739.4,38021.7,38306.7,38593.3,38879.5,39165.3,39450.6,39736,40021,40307.4,40597,40890.5,41184.1,41474.9}, \
            {-14.2237,-8.20307,-4.68124,-2.18247,-0.24427,1.33936,2.67829,3.83813,4.86118,5.77633,6.60418,7.35995,8.0552,8.69889,9.29816,9.85873,10.3853,10.8818,11.3514,11.7969,12.2207,12.6248,13.0109,13.3806,13.7351,14.0758,14.4036,14.7195,15.0243,15.3188,15.6036,15.8793,16.1466,16.4059,16.6577,16.9024,17.1404,17.372,17.5976,17.8175,18.032,18.2413,18.4457,18.6454,18.8406,19.0315,19.2183,19.4012,19.5803,19.7557,19.9277,20.0964,20.2618,20.4242,20.5836,20.7401,20.8938,21.0449,21.1934,21.3394,21.4829,21.6242,21.7631,21.8999,22.0346,22.1672,22.2978,22.4265,22.5533,22.6783,22.8015,22.923,23.0428,23.161,23.2776,23.3926,23.5061,23.6182,23.7289,23.8381,23.946,24.0526,24.1579,24.2619,24.3647,24.4663,24.5667,24.666,24.7641,24.8612,24.9572,25.0521,25.146,25.2389,25.3308,25.4218,25.5118,25.6009,25.689,25.7763,25.8628,25.9483,26.0331,26.117,26.2001,26.2824,26.364,26.4448,26.5249,26.6042,26.6828,26.7607,26.8379,26.9144,26.9903,27.0655,27.14,27.214,27.2873,27.36,27.432,27.5035,27.5744,27.6448,27.7145,27.7837,27.8524,27.92}, \
            {846.502,815.306,741.335,684.921,641.126,607.13,577.998,552.336,530.199,511.958,497.793,481.971,457.57,428.056,395.845,363.173,331.776,301.919,273.536,246.185,219.414,193.535,169.099,146.428,125.656,106.923,90.6313,76.6674,64.6254,54.4372,46.1425,38.965,32.641,27.5284,23.1648,19.4756,16.4036,13.9053,11.9143,10.3895,9.75607,9.74823,10.2386,11.2622,12.3811,14.7043,17.4594,20.4121,24.336,28.5285,32.9007,37.9287,43.3541,48.8631,54.3854,60.2662,66.5954,72.9043,79.4523,86.1694,92.9775,99.6978,106.408,113.342,120.233,127.008,133.598,140.022,146.387,152.435,158.144,163.983,169.736,174.842,179.65,184.318,188.997,193.567,197.527,201.193,204.825,208.485,211.841,214.827,217.82,220.934,223.697,226.304,228.99,231.753,234.602,237.473,240.328,243.115,245.722,248.504,251.561,254.65,257.644,260.587,263.743,266.885,269.797,272.564,275.168,277.719,280.276,282.895,285.574,288.108,289.668,290.57,291.413,292.164,292.845,293.297,293.117,292.416,291.719,291.107,290.491,289.769,289.038,287.69,285.494,282.708,279.849,276.965}, \
            {5,15,40,60},{5.1,13.925,23.325,27.375},
            {5,15,40,47,60},{4.7,13.4,22.85,24.3,26.92}
         },\

         {
            975,19,{293.392,622.782,967.537,1322.31,1684.73,2053.43,2427.62,2806.07,3187.55,3574.84,3978.31,4389.57,4807.69,5232.08,5661.95,6094.3,6525.67,6953.62,7377.62,7797.91,8214.5,8627.85,9037.35,9442.86,9843.85,10240,10631.1,11017.1,11398.9,11775.7,12148.9,12518.1,12884.4,13247.8,13608.6,13966.1,14321.2,14673.3,15022.7,15369.1,15713,16053.7,16391.8,16727.4,17060,17389.9,17718.1,18043.3,18366.5,18688.8,19008.9,19327.4,19644.5,19961.4,20276.5,20590.8,20904.5,21217.8,21530,21841.6,22152.5,22462.6,22772.3,23081.6,23389.6,23696.8,24003.3,24309.2,24613.4,24916.8,25219.5,25521.4,25821.7,26120.7,26419.1,26716.3,27011.9,27306.9,27600.8,27893.7,28186,28477.5,28767.1,29056,29344.3,29632.4,29919.5,30206.1,30491.1,30775.6,31060.2,31344.6,31628.6,31911.3,32193.6,32475.2,32757.2,33039.2,33320.5,33601.5,33882.8,34163.1,34443.7,34725,35005.6,35286.1,35565.8,35845.7,36126.9,36409.9,36692.5,36975.7,37257.9,37539.9,37822.1,38104.6,38390.1,38678.4,38966.1,39253.6,39540.4,39827.1,40114,40401.6,40691.4,40981.3,41271.5,41558.9}, \
            {-13.1137,-7.09307,-3.57124,-1.07247,0.86573,2.44936,3.78829,4.94813,5.97118,6.88633,7.71418,8.46995,9.1652,9.80889,10.4082,10.9687,11.4953,11.9918,12.4614,12.9069,13.3307,13.7348,14.1209,14.4906,14.8451,15.1858,15.5136,15.8295,16.1343,16.4288,16.7136,16.9893,17.2566,17.5159,17.7677,18.0124,18.2504,18.482,18.7076,18.9275,19.142,19.3513,19.5557,19.7554,19.9506,20.1415,20.3283,20.5112,20.6903,20.8657,21.0377,21.2064,21.3718,21.5342,21.6936,21.8501,22.0038,22.1549,22.3034,22.4494,22.5929,22.7342,22.8731,23.0099,23.1446,23.2772,23.4078,23.5365,23.6633,23.7883,23.9115,24.033,24.1528,24.271,24.3876,24.5026,24.6161,24.7282,24.8389,24.9481,25.056,25.1626,25.2679,25.3719,25.4747,25.5763,25.6767,25.776,25.8741,25.9712,26.0672,26.1621,26.256,26.3489,26.4408,26.5318,26.6218,26.7109,26.799,26.8863,26.9728,27.0583,27.1431,27.227,27.3101,27.3924,27.474,27.5548,27.6349,27.7142,27.7928,27.8707,27.9479,28.0244,28.1003,28.1755,28.25,28.324,28.3973,28.47,28.542,28.6135,28.6844,28.7548,28.8245,28.8937,28.9624,29.03}, \
            {733.959,699.557,639.985,597.214,563.558,536.445,514.131,494.327,476.944,462.462,453.205,439.319,417.2,387.651,353.081,316.714,281.342,248.353,217.976,190.252,165.031,141.841,120.823,102.136,85.5572,71.4642,59.8793,50.2737,42.5303,36.4882,31.505,27.5539,24.635,22.3814,20.4499,19.3564,18.6344,18.3306,18.5316,19.4058,21.0072,23.2665,26.1287,29.5427,33.4935,37.9865,42.8307,48.2354,54.1436,59.9695,66.0399,72.4376,78.6911,85.0342,91.2174,97.2271,103.273,109.293,115.16,120.727,126.318,131.805,136.907,142.077,147.167,152.141,157.103,161.974,166.771,171.456,176.078,180.846,185.477,189.955,194.427,198.96,203.48,207.967,212.399,216.76,221.244,225.764,229.995,234.155,238.4,242.661,246.892,250.995,254.943,259.131,263.478,267.543,271.439,275.311,279.081,282.727,286.554,290.325,293.986,297.544,301.111,304.524,307.711,310.916,314.23,317.49,320.473,323.313,325.841,327.861,329.71,331.583,333.295,334.932,336.472,337.677,338.228,338.17,338.095,337.92,337.608,337.358,337.06,336.345,335.073,333.674,332.165,330.682}, \
            {5,15,40,60},{5.9,14.8,24.325,28.45},
            {5,15,40,47,60},{5.45,14.4,23.85,25.42,27.96}
         },\

         {
            975,20,{301.592,637.476,987.113,1346.23,1712.59,2084.85,2462.19,2843.53,3228.6,3620.99,4039.33,4462.05,4889.31,5320.36,5753.97,6187.97,6619.63,7047.69,7471.59,7891.78,8307.45,8718.72,9124.3,9523.85,9917.46,10304.8,10687,11063.4,11436,11803.8,12168.4,12529.3,12887.5,13243,13596.1,13946.7,14294.5,14640.2,14983.1,15324,15662.8,15998.5,16332.1,16664,16993.2,17320.3,17645.5,17969.3,18290.6,18610.5,18928.9,19246,19561.7,19876.7,20190.5,20503,20814.3,21125.3,21435.2,21744.6,22052.5,22360,22666.7,22972.5,23277.3,23581.1,23883.8,24185.4,24485.9,24785.6,25084.1,25381.5,25677.1,25972.3,26265.9,26558.9,26850.7,27140.6,27430,27718.5,28006.6,28294.2,28580.2,28865.2,29150.5,29435.3,29719.7,30003.8,30287.4,30571.6,30856.2,31141.2,31425.4,31709.4,31993.5,32279.6,32566.8,32854.5,33141.5,33428.5,33715.5,34003.4,34294.8,34586.5,34877.2,35167.9,35458.4,35748.7,36040.5,36334.5,36627.9,36921.1,37214,37506.3,37798.7,38091.4,38385.1,38679.7,38974,39268,39561.5,39855,40148,40441.2,40733.4,41026,41318.3,41607.9}, \
            {-12.0737,-6.05307,-2.53124,-0.0324699,1.90573,3.48936,4.82829,5.98813,7.01118,7.92633,8.75418,9.50995,10.2052,10.8489,11.4482,12.0087,12.5353,13.0318,13.5014,13.9469,14.3707,14.7748,15.1609,15.5306,15.8851,16.2258,16.5536,16.8695,17.1743,17.4688,17.7536,18.0293,18.2966,18.5559,18.8077,19.0524,19.2904,19.522,19.7476,19.9675,20.182,20.3913,20.5957,20.7954,20.9906,21.1815,21.3683,21.5512,21.7303,21.9057,22.0777,22.2464,22.4118,22.5742,22.7336,22.8901,23.0438,23.1949,23.3434,23.4894,23.6329,23.7742,23.9131,24.0499,24.1846,24.3172,24.4478,24.5765,24.7033,24.8283,24.9515,25.073,25.1928,25.311,25.4276,25.5426,25.6561,25.7682,25.8789,25.9881,26.096,26.2026,26.3079,26.4119,26.5147,26.6163,26.7167,26.816,26.9141,27.0112,27.1072,27.2021,27.296,27.3889,27.4808,27.5718,27.6618,27.7509,27.839,27.9263,28.0128,28.0983,28.1831,28.267,28.3501,28.4324,28.514,28.5948,28.6749,28.7542,28.8328,28.9107,28.9879,29.0644,29.1403,29.2155,29.29,29.364,29.4373,29.51,29.582,29.6535,29.7244,29.7948,29.8645,29.9337,30.0024,30.07},\
            {911.848,863.588,766.775,698.316,644.485,601.077,565.443,534.116,506.981,484.719,465.673,437.796,402.63,362.243,319.433,278.082,240.841,208.222,179.729,154.599,132.207,111.754,93.2388,76.8157,62.6553,50.8761,41.7414,35.1149,30.2321,26.9076,24.8971,23.6988,23.4002,24.1935,25.3796,26.8261,28.7462,31.3599,34.1774,37.4537,41.157,45.162,49.5688,54.0002,58.6154,63.6403,68.8494,74.2181,79.6331,85.0065,90.5379,96.2924,101.941,107.598,113.351,118.977,124.584,130.269,135.857,141.364,146.887,152.408,157.837,163.294,168.83,174.173,179.366,184.783,190.38,195.709,200.821,206.035,211.251,216.513,221.674,226.754,231.797,236.84,241.828,246.631,251.284,255.997,260.773,265.387,269.874,274.114,278.21,282.322,286.234,289.914,293.426,296.846,300.133,303.211,306.206,308.952,311.353,313.639,315.905,318.161,320.312,322.019,323.53,324.848,326.056,327.196,328.487,329.776,330.444,331.178,331.852,332.344,332.932,333.74,334.487,335.065,335.454,335.786,336.285,336.738,337.104,337.591,338.049,338.564,339.566,340.665,341.793,342.91},\
            {5,15,40,60}, {6.825,15.725,25.4,29.425},
            {5,15,40,47,60},{6.45,15.3,24.85,26.42,28.96}
         },\

         {
            975,21,{313.171,655.386,1008.74,1370.23,1737.07,2109.08,2484.82,2864.07,3246.46,3638.34,4056.32,4479.41,4906.85,5336.6,5767.64,6197.59,6625.2,7047.74,7464.67,7874.79,8278.98,8676.96,9068.92,9454.23,9834.1,10209.6,10580.2,10946.5,11309.3,11667.7,12023.4,12375.2,12724.6,13071.1,13414.7,13755.9,14094.6,14431.3,14765.6,15098,15428.2,15756.9,16083.5,16408.3,16731.5,17053.2,17373.4,17692,18008.8,18324.2,18638.3,18950.9,19261.4,19571.1,19878.7,20185.1,20489.4,20792.9,21094.6,21395,21693.1,21990.9,22287.2,22582.2,22875.5,23168,23459.9,23750.7,24040.7,24329.7,24618.4,24906.8,25194.7,25482,25768.8,26055.8,26342.6,26628.7,26914.9,27201.6,27488.5,27775.4,28061.6,28347.5,28634.7,28922.7,29210.9,29498.6,29786.1,30073.9,30362.4,30652.8,30943.4,31233,31522.9,31812.6,32102.9,32396.5,32689.9,32983.1,33276.3,33568.6,33861.5,34157.4,34455.1,34752.5,35049.8,35346.6,35643.3,35940.8,36241.7,36543.9,36845.7,37147.5,37449,37750.5,38051.9,38356.5,38662.3,38968.3,39273.5,39579.2,39884.4,40190,40497.3,40808.9,41120.7,41429.8}, \
            {-11.0837,-5.06307,-1.54124,0.95753,2.89573,4.47936,5.81829,6.97813,8.00118,8.91633,9.74418,10.5,11.1952,11.8389,12.4382,12.9987,13.5253,14.0218,14.4914,14.9369,15.3607,15.7648,16.1509,16.5206,16.8751,17.2158,17.5436,17.8595,18.1643,18.4588,18.7436,19.0193,19.2866,19.5459,19.7977,20.0424,20.2804,20.512,20.7376,20.9575,21.172,21.3813,21.5857,21.7854,21.9806,22.1715,22.3583,22.5412,22.7203,22.8957,23.0677,23.2364,23.4018,23.5642,23.7236,23.8801,24.0338,24.1849,24.3334,24.4794,24.6229,24.7642,24.9031,25.0399,25.1746,25.3072,25.4378,25.5665,25.6933,25.8183,25.9415,26.063,26.1828,26.301,26.4176,26.5326,26.6461,26.7582,26.8689,26.9781,27.086,27.1926,27.2979,27.4019,27.5047,27.6063,27.7067,27.806,27.9041,28.0012,28.0972,28.1921,28.286,28.3789,28.4708,28.5618,28.6518,28.7409,28.829,28.9163,29.0028,29.0883,29.1731,29.257,29.3401,29.4224,29.504,29.5848,29.6649,29.7442,29.8228,29.9007,29.9779,30.0544,30.1303,30.2055,30.28,30.354,30.4273,30.5,30.572,30.6435,30.7144,30.7848,30.8545,30.9237,30.9924,31.06},\
            {938.566,882.083,768.153,692.191,632.857,583.823,544.04,509.343,479.102,454.196,428.1,396.707,361.558,324.443,286.258,248.47,214.012,183.523,156.832,133.404,112.724,95.5345,81.6112,70.1057,61.0558,54.6467,50.68,48.5427,47.7891,48.2651,49.7561,51.7922,54.5021,57.8851,61.6604,65.8556,69.9664,74.1526,78.3245,82.8328,87.5076,92.0667,96.3824,100.644,105.355,109.893,114.375,118.926,123.674,128.508,133.131,137.917,143.226,148.483,153.619,159.357,165.465,171.845,178.095,184.383,191.084,197.899,204.576,211.269,218.109,224.977,231.705,238.278,244.727,251.093,257.422,263.722,269.569,275.06,280.118,285.151,290.07,294.885,299.387,303.382,306.899,310.285,313.706,317.164,320.316,322.444,324.541,326.81,329.057,331.242,332.877,334.242,335.693,337.187,338.537,339.841,341.148,342.433,343.596,344.741,345.842,346.842,347.885,348.926,350.093,351.479,352.788,353.969,355.222,356.669,358.32,360.225,362.202,364.298,366.339,368.515,370.789,373.401,376.586,379.882,383.097,386.319,389.562,392.714,396.083,400.163,404.248,408.297},\
            {5,15,40,60}, {7.7,16.75,26.525,30.35},
            {5,15,40,47,60},{7.25,15.3,26.15,27.72,29.96}
         }
      },\

      {  {
            37,18,{301.863,644.94,1005.19,1377.33,1758.46,2146.58,2541.18,2941.84,3347.66,3762.63,4187.01,4617.12,5053.42,5496.28,5943.33,6390.98,6837.74,7283.14,7727.24,8170.06,8611.69,9051.09,9487.56,9920.48,10349.4,10773.5,11192.3,11606.7,12016.6,12422.1,12824.2,13221.7,13616.5,14008,14396.6,14782.4,15165.5,15545.6,15922.9,16298.1,16670.1,17039.7,17406.5,17770.8,18132.6,18491.7,18849.2,19203.9,19556.2,19907,20255.1,20601.1,20945.3,21288.1,21628.6,21967.6,22305.6,22641.8,22976.5,23309.9,23642.8,23974.5,24304.8,24634.5,24963.9,25292.7,25620.6,25948.4,26275.8,26602.5,26929,27255.5,27581.6,27907.2,28232.4,28557.3,28882.4,29206.9,29530.7,29853.5,30176.3,30498.9,30820.1,31140.7,31460.6,31779.9,32098.6,32416.3,32732.5,33048.7,33363.9,33678.4,33991.9,34303.6,34615.1,34926.3,35236.8,35546.5,35854.8,36161.9,36468.8,36775.3,37081.8,37387.4,37692.7,37996.8,38299.9,38604.3,38909.2,39213.6,39517.5,39821.2,40124.7,40428.4,40734.9,41041.2,41346.9,41652.9,41958.1,42263.7,42570.4,42878.8,43189.5,43500,43810,44119.7,44429.2,44736.2}, \
            {-14.2237,-8.20307,-4.68124,-2.18247,-0.24427,1.33936,2.67829,3.83813,4.86118,5.77633,6.60418,7.35995,8.0552,8.69889,9.29816,9.85873,10.3853,10.8818,11.3514,11.7969,12.2207,12.6248,13.0109,13.3806,13.7351,14.0758,14.4036,14.7195,15.0243,15.3188,15.6036,15.8793,16.1466,16.4059,16.6577,16.9024,17.1404,17.372,17.5976,17.8175,18.032,18.2413,18.4457,18.6454,18.8406,19.0315,19.2183,19.4012,19.5803,19.7557,19.9277,20.0964,20.2618,20.4242,20.5836,20.7401,20.8938,21.0449,21.1934,21.3394,21.4829,21.6242,21.7631,21.8999,22.0346,22.1672,22.2978,22.4265,22.5533,22.6783,22.8015,22.923,23.0428,23.161,23.2776,23.3926,23.5061,23.6182,23.7289,23.8381,23.946,24.0526,24.1579,24.2619,24.3647,24.4663,24.5667,24.666,24.7641,24.8612,24.9572,25.0521,25.146,25.2389,25.3308,25.4218,25.5118,25.6009,25.689,25.7763,25.8628,25.9483,26.0331,26.117,26.2001,26.2824,26.364,26.4448,26.5249,26.6042,26.6828,26.7607,26.8379,26.9144,26.9903,27.0655,27.14,27.214,27.2873,27.36,27.432,27.5035,27.5744,27.6448,27.7145,27.7837,27.8524,27.92}, \
            {660.883,633.972,583.675,549.203,523.538,501.932,483.945,468.567,456.493,445.841,430.548,414.346,395.894,373.109,345.293,314.273,282.525,252.897,225.824,201.255,178.329,156.617,136.042,116.767,99.2134,83.6848,70.0801,58.3383,48.4142,40.1623,33.3521,27.6099,22.7165,18.66,15.9445,13.8405,12.2913,11.563,11.5403,12.2679,13.3971,15.3337,18.3291,21.6297,25.6654,30.2425,35.0232,40.227,46.1246,52.4904,59.2626,66.5597,74.0224,81.4644,89.1366,96.9731,104.88,112.948,120.96,128.759,136.587,144.236,151.757,158.969,165.867,172.812,179.654,185.595,191.289,196.954,202.616,207.852,212.545,217.103,221.852,226.093,229.908,233.927,237.848,241.516,245.009,248.429,251.936,255.292,258.569,261.838,265.153,268.472,271.726,275.147,278.477,281.574,284.547,287.428,290.288,293.004,295.571,298.206,300.941,303.456,305.049,306.803,308.524,310.194,311.86,313.407,314.777,315.89,316.906,317.914,318.756,319.361,320.033,320.599,320.604,320.33,320.077,319.712,319.212,318.657,317.813,316.621,315.155,313.489,311.713,309.953,308.3,306.581} ,\
            {5,15,40,60}, {4.45,13.275,22.575,26.6},
            {5,15,40,47,60},{4.05,12.82,22.15,23.72,26.22}
         },\

         {
            37,19, {313.841,667.893,1038.97,1421.37,1812.57,2210.74,2614.94,3024.14,3439.35,3873.29,4314.29,4757.66,5205.68,5658.8,6115.66,6573.95,7031.07,7486.58,7939.91,8390.83,8838.08,9281.27,9719.46,10150.9,10576.7,10997,11411.8,11820.8,12226.3,12627.5,13025.6,13419.8,13811.6,14200.1,14585.8,14968.6,15348.6,15725.7,16099.7,16470.3,16838.5,17203.5,17566.3,17926.1,18283.2,18638.4,18991.5,19342,19691.4,20038.5,20384.3,20728.6,21071.3,21413.3,21753.6,22092.7,22430.9,22768.1,23104.3,23439.5,23773.8,24107.7,24440,24771.4,25102.5,25432.7,25762.5,26091.6,26419.5,26747,27074.1,27400.2,27725.6,28050.3,28374.3,28697.3,29019.3,29340.4,29661.3,29980.9,30299,30615.3,30930.6,31245.3,31559.2,31871.6,32182,32490.7,32798.3,33105.7,33412.2,33717.2,34019.9,34321.6,34623.5,34925.2,35225.8,35525.7,35824.2,36123.7,36424.6,36725.7,37026.9,37327.4,37627.5,37927.8,38228.9,38530.7,38832.3,39133.3,39433.7,39734,40036.5,40339.6,40645.8,40953.9,41261.6,41569.4,41876.7,42183.7,42489,42794.1,43097.8,43401.2,43704.2,44006.7,44308.8,44608.4}, \
            {-13.1137,-7.09307,-3.57124,-1.07247,0.86573,2.44936,3.78829,4.94813,5.97118,6.88633,7.71418,8.46995,9.1652,9.80889,10.4082,10.9687,11.4953,11.9918,12.4614,12.9069,13.3307,13.7348,14.1209,14.4906,14.8451,15.1858,15.5136,15.8295,16.1343,16.4288,16.7136,16.9893,17.2566,17.5159,17.7677,18.0124,18.2504,18.482,18.7076,18.9275,19.142,19.3513,19.5557,19.7554,19.9506,20.1415,20.3283,20.5112,20.6903,20.8657,21.0377,21.2064,21.3718,21.5342,21.6936,21.8501,22.0038,22.1549,22.3034,22.4494,22.5929,22.7342,22.8731,23.0099,23.1446,23.2772,23.4078,23.5365,23.6633,23.7883,23.9115,24.033,24.1528,24.271,24.3876,24.5026,24.6161,24.7282,24.8389,24.9481,25.056,25.1626,25.2679,25.3719,25.4747,25.5763,25.6767,25.776,25.8741,25.9712,26.0672,26.1621,26.256,26.3489,26.4408,26.5318,26.6218,26.7109,26.799,26.8863,26.9728,27.0583,27.1431,27.227,27.3101,27.3924,27.474,27.5548,27.6349,27.7142,27.7928,27.8707,27.9479,28.0244,28.1003,28.1755,28.25,28.324,28.3973,28.47,28.542,28.6135,28.6844,28.7548,28.8245,28.8937,28.9624,29.03}, \
            {798.838,744.46,668.315,613.447,571.251,536.649,507.66,483.511,465.802,452.242,431.489,397.238,358.379,320.582,285.525,252.041,220.084,189.691,161.184,135.198,111.744,90.4072,71.5866,55.2974,41.3768,29.6055,20.6362,13.7975,8.67189,5.00926,2.44294,0.572102,0,0.253338,0.937088,2.20373,4.40907,6.92403,9.92627,13.5997,17.7757,22.4342,27.1561,32.6443,38.5459,44.3571,50.7008,57.4651,64.3282,71.4375,78.8369,86.2848,93.8238,101.504,109.47,117.557,125.473,133.527,141.685,149.496,157.214,164.848,172.304,179.477,186.356,192.911,199.284,205.424,211.3,217.019,222.146,227.007,231.682,236.308,240.71,244.699,248.736,252.684,256.571,260.503,264.398,268.328,272.402,276.47,280.526,284.75,289.337,294.127,298.947,303.476,308.056,312.575,317.354,322.194,327.157,331.955,336.683,341.345,345.67,349.456,352.783,356.059,359.505,362.929,366.098,369.309,372.414,375.412,378.093,380.916,383.713,386.389,388.469,389.7,390.387,390.63,390.974,391.386,391.798,392.378,393.009,393.85,394.783,395.907,396.944,397.93,399.044,400.117}, \
            {5,15,40,60}, {5.3,14.15,23.575,27.7},
            {5,15,40,47,60},{4.85,13.75,23.15,24.75,27.26}
         }, \

         {
            37, 20, {327.852,692.317,1071.53,1460.56,1857.29,2260.26,2668.39,3080.64,3497.33,3933.79,4376,4822.97,5277.18,5739.22,6206.06,6671.63,7132.88,7587.33,8036.17,8479.66,8918.93,9353.94,9784.47,10209.2,10627.4,11039.9,11447,11848.2,12245.8,12639.6,13029.9,13417.4,13802.5,14184.5,14564.3,14941.3,15315.3,15686.4,16054.5,16419.1,16781.1,17139.8,17495.5,17849,18199.8,18548.4,18895.2,19240.6,19584.1,19926.6,20268,20608.9,20948.6,21288.2,21627.2,21965.2,22302.3,22639.3,22976,23311.6,23645.8,23979.3,24311.1,24642.2,24972.2,25300.7,25627.2,25952.4,26276.3,26599.6,26920.5,27239.4,27557.3,27874.4,28190.7,28505.1,28817.6,29129.6,29441.3,29752,30061.5,30370.4,30679.1,30987.9,31295.7,31603.2,31909.8,32216.7,32524.4,32832.4,33139.2,33445.5,33752,34059.2,34367.1,34674.9,34982.5,35289.4,35595.9,35903.4,36212.6,36522,36831,37139.7,37448,37756.1,38064.9,38377.1,38689.8,39001.8,39313.8,39625.6,39937,40248.5,40560.5,40872.8,41184.7,41496.6,41808,42119.1,42430.9,42743.2,43058.1,43373.6,43688.4,44003.2,44317.8,44629.6}, \
            {-12.0737,-6.05307,-2.53124,-0.0324699,1.90573,3.48936,4.82829,5.98813,7.01118,7.92633,8.75418,9.50995,10.2052,10.8489,11.4482,12.0087,12.5353,13.0318,13.5014,13.9469,14.3707,14.7748,15.1609,15.5306,15.8851,16.2258,16.5536,16.8695,17.1743,17.4688,17.7536,18.0293,18.2966,18.5559,18.8077,19.0524,19.2904,19.522,19.7476,19.9675,20.182,20.3913,20.5957,20.7954,20.9906,21.1815,21.3683,21.5512,21.7303,21.9057,22.0777,22.2464,22.4118,22.5742,22.7336,22.8901,23.0438,23.1949,23.3434,23.4894,23.6329,23.7742,23.9131,24.0499,24.1846,24.3172,24.4478,24.5765,24.7033,24.8283,24.9515,25.073,25.1928,25.311,25.4276,25.5426,25.6561,25.7682,25.8789,25.9881,26.096,26.2026,26.3079,26.4119,26.5147,26.6163,26.7167,26.816,26.9141,27.0112,27.1072,27.2021,27.296,27.3889,27.4808,27.5718,27.6618,27.7509,27.839,27.9263,28.0128,28.0983,28.1831,28.267,28.3501,28.4324,28.514,28.5948,28.6749,28.7542,28.8328,28.9107,28.9879,29.0644,29.1403,29.2155,29.29,29.364,29.4373,29.51,29.582,29.6535,29.7244,29.7948,29.8645,29.9337,30.0024,30.07}, \
            {665.835,636.65,585.229,551.884,525.081,503.158,484.714,468.751,455.787,445.888,431.695,406.39,370.704,327.954,284.807,246.662,214.988,188.148,164.234,141.999,121.242,102.441,86.1419,72.8495,62.7719,55.6891,51.2007,48.8345,47.7703,47.6996,48.5178,49.9343,51.6979,53.7456,56.223,59.1441,62.5987,66.6706,71.4483,76.8802,82.7509,89.4577,96.5823,103.862,111.837,119.851,127.778,135.786,143.83,151.832,159.563,167.274,174.791,181.908,188.878,195.925,202.722,209.227,215.491,221.486,227.382,233.452,239.326,245.059,250.747,256.247,261.821,267.336,272.814,278.138,283.563,289.076,294.594,299.893,305.078,310.278,315.243,320.143,325.264,330.434,335.331,340.021,344.721,349.389,354.089,358.786,363.233,367.555,371.608,375.716,379.999,384.113,387.903,391.454,394.911,398.458,401.954,405.49,408.842,411.803,414.237,416.526,418.803,421,423.13,425.194,427.078,428.197,428.914,429.465,430.126,430.689,431.089,431.615,432.231,432.895,433.528,434.148,434.882,435.629,436.402,437.579,439.045,440.851,442.507,444.036,445.696,447.299}, \
            {5,15,40,60}, {6.225,15.15,24.675,28.775},
            {5,15,40,47,60},{5.85,14.72,24.25,25.82,28.36}
         }, \

         {
            37, 21, {331.266,695.635,1072.89,1459.27,1852.57,2251.4,2654.48,3061.36,3473.06,3911.83,4359.79,4811.28,5266.3,5723.62,6181.29,6636.42,7087.09,7532.52,7971.64,8405.68,8833.85,9255.96,9673.1,10083.9,10488.4,10888.1,11282.5,11671.7,12057,12438.5,12816.6,13191.3,13563.5,13932.5,14299.8,14663.7,15026,15385.6,15743,16098.6,16451.7,16802.8,17152,17499.2,17844.9,18188.6,18530.3,18871.1,19210,19546.8,19882.6,20217.1,20549.3,20881.1,21211.3,21540,21867.4,22193.8,22518.9,22842.4,23164.7,23486.4,23806.8,24125.5,24443.3,24760.2,25076,25390.4,25704,26016.5,26328.3,26639.1,26948.8,27257.5,27565.9,27873.4,28180.4,28486.8,28793,29099,29404.7,29709.9,30014.8,30320.3,30627.4,30934.3,31240.1,31546,31851.9,32160.6,32471.1,32781,33090.5,33399.6,33709.4,34020.9,34335.2,34649.3,34963,35276.3,35590,35904,36219.5,36536,36852.3,37167.8,37483.9,37799.8,38119.1,38439.7,38760.2,39081,39401.4,39721.4,40039.5,40357.3,40677.9,41000.7,41322.9,41645.4,41967.1,42289.6,42614.7,42944.1,43279,43614.2,43949.2,44281.6}, \
            {-11.0837,-5.06307,-1.54124,0.95753,2.89573,4.47936,5.81829,6.97813,8.00118,8.91633,9.74418,10.5,11.1952,11.8389,12.4382,12.9987,13.5253,14.0218,14.4914,14.9369,15.3607,15.7648,16.1509,16.5206,16.8751,17.2158,17.5436,17.8595,18.1643,18.4588,18.7436,19.0193,19.2866,19.5459,19.7977,20.0424,20.2804,20.512,20.7376,20.9575,21.172,21.3813,21.5857,21.7854,21.9806,22.1715,22.3583,22.5412,22.7203,22.8957,23.0677,23.2364,23.4018,23.5642,23.7236,23.8801,24.0338,24.1849,24.3334,24.4794,24.6229,24.7642,24.9031,25.0399,25.1746,25.3072,25.4378,25.5665,25.6933,25.8183,25.9415,26.063,26.1828,26.301,26.4176,26.5326,26.6461,26.7582,26.8689,26.9781,27.086,27.1926,27.2979,27.4019,27.5047,27.6063,27.7067,27.806,27.9041,28.0012,28.0972,28.1921,28.286,28.3789,28.4708,28.5618,28.6518,28.7409,28.829,28.9163,29.0028,29.0883,29.1731,29.257,29.3401,29.4224,29.504,29.5848,29.6649,29.7442,29.8228,29.9007,29.9779,30.0544,30.1303,30.2055,30.28,30.354,30.4273,30.5,30.572,30.6435,30.7144,30.7848,30.8545,30.9237,30.9924,31.06}, \
            {1048.69,954.393,823.167,732.634,662.939,605.866,557.695,516.302,479.941,446.855,414.674,381.701,345.842,307.522,268.559,232.197,200.474,173.898,151.553,132.368,116.007,102.129,90.6655,82.0635,76.0091,72.3467,71.0308,71.9105,73.7093,76.6375,80.637,85.264,90.3394,95.7443,101.426,107.317,113.577,120.177,126.683,133.765,140.969,148.403,155.83,163.296,171.111,179.136,187.021,194.745,202.681,210.452,218.105,225.798,233.324,240.706,247.958,255.181,262.375,269.45,276.285,283.161,290.169,296.996,303.703,310.332,316.747,323.135,329.451,335.589,341.837,348.095,354.092,359.986,365.894,371.507,377.032,382.49,387.757,392.904,397.666,402.372,407.128,411.64,415.92,419.987,423.802,427.246,430.708,434.149,437.158,439.999,442.572,444.892,447.149,449.482,451.697,453.647,455.526,457.138,458.65,460.269,461.766,463.671,465.582,467.477,469.446,471.209,473.053,475.41,477.945,480.896,484.212,487.617,491,494.361,497.655,501.539,506.431,512.037,517.578,523.063,528.535,533.953,539.534,545.34,551.17,556.876,562.578,568.235}, \
            {5,15,40,60}, {7.1,16.15,25.8,29.775},
            {5,15,40,47,60},{6.65,15.7,25.4,27.02,29.35}
         }
      },\

      {  {
            124, 18, {324.613,694.077,1081.77,1482.16,1892.13,2310.02,2735.14,3166.76,3603.75,4056.94,4511.94,4972.64,5440.45,5915.07,6393.6,6872.51,7350.16,7825.96,8299.26,8769.86,9238.32,9703.72,10164.9,10622.3,11075.8,11524.9,11970.5,12412,12850.9,13286,13718.4,14146.5,14572.4,14994.3,15412.9,15827.8,16238.8,16646.9,17051.3,17452.4,17849.7,18244.6,18637.1,19026.6,19413.6,19799.2,20181.9,20562.8,20942.3,21320.2,21695.9,22070.5,22443.6,22815.1,23185.4,23554.4,23921.9,24287.9,24652.8,25016.8,25379.3,25740.8,26101.3,26460.8,26819.1,27176.4,27532.9,27888.7,28243.7,28597.7,28950.7,29303.4,29655.6,30006.8,30357.9,30707.4,31057,31406.5,31755.3,32103.1,32450.2,32797.5,33144.1,33490.1,33835.7,34181,34525.7,34869.2,35211.8,35553.7,35895.3,36236.7,36577.1,36916.1,37253.4,37590.1,37925.8,38260.5,38595.1,38929.9,39265.4,39600.3,39933.4,40266,40594.1,40919.8,41246.4,41575.5,41904.3,42232.7,42561.4,42891.2,43224.8,43559.9,43894.8,44228.9,44563.3,44900.3,45238.2,45577.4,45917.8,46258.3,46598.3,46938.6,47278,47617.1,47956.5,48292.6}, \
            {-14.2237,-8.20307,-4.68124,-2.18247,-0.24427,1.33936,2.67829,3.83813,4.86118,5.77633,6.60418,7.35995,8.0552,8.69889,9.29816,9.85873,10.3853,10.8818,11.3514,11.7969,12.2207,12.6248,13.0109,13.3806,13.7351,14.0758,14.4036,14.7195,15.0243,15.3188,15.6036,15.8793,16.1466,16.4059,16.6577,16.9024,17.1404,17.372,17.5976,17.8175,18.032,18.2413,18.4457,18.6454,18.8406,19.0315,19.2183,19.4012,19.5803,19.7557,19.9277,20.0964,20.2618,20.4242,20.5836,20.7401,20.8938,21.0449,21.1934,21.3394,21.4829,21.6242,21.7631,21.8999,22.0346,22.1672,22.2978,22.4265,22.5533,22.6783,22.8015,22.923,23.0428,23.161,23.2776,23.3926,23.5061,23.6182,23.7289,23.8381,23.946,24.0526,24.1579,24.2619,24.3647,24.4663,24.5667,24.666,24.7641,24.8612,24.9572,25.0521,25.146,25.2389,25.3308,25.4218,25.5118,25.6009,25.689,25.7763,25.8628,25.9483,26.0331,26.117,26.2001,26.2824,26.364,26.4448,26.5249,26.6042,26.6828,26.7607,26.8379,26.9144,26.9903,27.0655,27.14,27.214,27.2873,27.36,27.432,27.5035,27.5744,27.6448,27.7145,27.7837,27.8524,27.92}, \
            {676.992,640.605,590.831,556.148,529.482,507.627,489.395,474.694,469.729,458.103,431.72,399.672,367.788,338.05,310.222,283.171,255.979,229.012,202.343,177.036,153.032,131.338,112.025,95.2598,80.9243,68.5703,57.8123,48.464,39.9635,32.2867,25.2465,18.6218,12.6302,7.85303,4.15566,1.40553,0,0.0133438,0.936534,2.87039,5.83255,9.95074,14.6131,19.9992,26.1514,32.6492,39.4584,46.6205,53.8346,61.1292,68.701,76.4458,84.0025,91.5443,99.0991,106.607,113.945,121.161,128.151,135.047,142.015,148.814,155.654,162.083,168.477,174.861,180.798,186.589,192.23,197.668,203.163,208.561,213.712,218.882,223.824,228.648,233.387,237.95,242.309,246.674,250.843,254.776,258.728,262.601,266.23,269.72,272.829,275.882,278.976,281.96,284.866,287.357,289.546,291.598,293.473,295.34,297.026,298.555,299.467,300.382,301.457,302.491,303.197,303.944,304.79,305.593,306.425,307.267,307.975,308.678,309.476,310.175,310.837,311.308,311.645,311.94,312.156,312.397,312.483,312.483,312.483,312.483,312.483,312.483,312.483,312.483,312.483,312.483}, \
            {5,15,40,60}, {3.875, 12.7, 21.85, 25.875},
            {5,15,40,47,60},{3.45,12.3,21.45,23.05,25.46}
         },\

         {
            124, 19, {335.404,715.383,1113.62,1524.33,1944.71,2372.91,2807.35,3245.02,3693.28,4160.67,4634.81,5114.63,5599.91,6089.33,6580.25,7069.78,7555.17,8036.57,8514.08,8988.18,9459.25,9926.94,10391.2,10850.9,11306.2,11757.1,12203.8,12646.1,13084,13517.8,13947.3,14373.2,14795.2,15213.1,15627.8,16038.2,16445.5,16849.6,17250.2,17648.1,18042.7,18434.7,18824.1,19211.2,19595.5,19977.7,20357.9,20735.9,21112.4,21486.2,21858.8,22230.2,22599.8,22967.6,23334.6,23700.4,24065.1,24428.7,24791.5,25153.6,25514.5,25875.3,26235.1,26594.7,26953.4,27311.9,27669.4,28026.4,28383,28738.8,29094.3,29448.5,29801.6,30153.6,30505.4,30855.9,31204.4,31551.8,31898.7,32243.7,32588,32930.5,33271.5,33611.4,33950.3,34286.6,34622.3,34957.4,35291.4,35624,35955.6,36284.3,36612.4,36940.8,37268.7,37595.8,37922.2,38247.4,38569.9,38892.7,39215.9,39538.2,39861.2,40183.3,40505.5,40830.3,41157,41484.2,41810.8,42137.7,42466,42794.2,43125.5,43459.1,43792.6,44125.7,44458.3,44792.7,45127.6,45462.1,45796.2,46130,46464,46797.7,47130.6,47463.1,47795.8,48125.2},\
            {-13.1137,-7.09307,-3.57124,-1.07247,0.86573,2.44936,3.78829,4.94813,5.97118,6.88633,7.71418,8.46995,9.1652,9.80889,10.4082,10.9687,11.4953,11.9918,12.4614,12.9069,13.3307,13.7348,14.1209,14.4906,14.8451,15.1858,15.5136,15.8295,16.1343,16.4288,16.7136,16.9893,17.2566,17.5159,17.7677,18.0124,18.2504,18.482,18.7076,18.9275,19.142,19.3513,19.5557,19.7554,19.9506,20.1415,20.3283,20.5112,20.6903,20.8657,21.0377,21.2064,21.3718,21.5342,21.6936,21.8501,22.0038,22.1549,22.3034,22.4494,22.5929,22.7342,22.8731,23.0099,23.1446,23.2772,23.4078,23.5365,23.6633,23.7883,23.9115,24.033,24.1528,24.271,24.3876,24.5026,24.6161,24.7282,24.8389,24.9481,25.056,25.1626,25.2679,25.3719,25.4747,25.5763,25.6767,25.776,25.8741,25.9712,26.0672,26.1621,26.256,26.3489,26.4408,26.5318,26.6218,26.7109,26.799,26.8863,26.9728,27.0583,27.1431,27.227,27.3101,27.3924,27.474,27.5548,27.6349,27.7142,27.7928,27.8707,27.9479,28.0244,28.1003,28.1755,28.25,28.324,28.3973,28.47,28.542,28.6135,28.6844,28.7548,28.8245,28.8937,28.9624,29.03}, \
            {971.1,886.676,770.826,691.602,631.659,582.451,540.671,504.439,471.995,449.044,420.22,386.942,350.737,315.186,282.461,252.773,225.302,199.224,173.957,149.789,127.229,106.713,89.0819,74.1397,61.6338,51.2682,42.5873,35.5825,30.1352,25.9189,23.1152,21.4193,20.8242,21.3065,22.8326,24.873,28.0718,31.8539,36.8458,42.4127,48.267,55.027,62.2586,69.7694,77.6827,86.0293,94.5891,103.582,112.785,122.106,131.624,141.097,150.571,160.012,169.267,178.349,187.262,195.919,204.363,212.649,220.437,227.81,234.814,241.507,247.764,253.786,259.469,264.704,269.7,274.532,279.083,283.548,287.758,291.966,296.053,300.217,304.389,308.346,312.351,316.489,320.695,325.028,329.383,333.577,337.911,342.379,346.844,351.3,355.792,360.156,364.701,369.555,374.031,378.281,382.304,386.483,390.747,394.765,398.505,401.919,404.805,407.627,410.296,412.91,415.361,417.615,420.096,422.552,424.795,426.946,429.052,431.138,433.154,434.671,436.112,437.525,438.726,439.795,440.578,441.211,441.439,441.439,441.439,441.439,441.439,441.439,441.439,441.439}, \
            {5,15,40,60}, {4.725,13.575,22.9, 27},
            {5,15,40,47,60},{4.35,13.12,22.45,24.04,26.58}
         }, \

         {
            124, 20, {347.864,736.885,1142.2,1558.91,1984.34,2416.78,2854.89,3297.81,3752.27,4221.7,4694.92,5174.8,5662.47,6156.44,6651.5,7144.16,7632.13,8115.5,8594.5,9068.66,9537.2,9999.99,10456.9,10907.7,11353.3,11793.1,12228.6,12659.9,13087.4,13512,13933,14350.7,14765.6,15176.7,15584.3,15988.3,16388.7,16785.9,17179.6,17570.3,17958.5,18342.8,18725.6,19106,19483.5,19860,20234.8,20607.8,20979.5,21350.6,21719.8,22088.2,22455.3,22821.9,23187.3,23551.3,23914.5,24277.3,24638.9,24999.1,25358.6,25717.9,26075.7,26432.3,26787.9,27143,27497.6,27851.1,28203.3,28554.8,28905.1,29254.7,29602.9,29949.8,30295.8,30639.8,30982.5,31324.3,31665.2,32004,32340.9,32676.1,33010.6,33344.5,33676.6,34007.9,34335.5,34663.2,34991.5,35319.3,35646.2,35972,36297.6,36624.1,36952.3,37281,37609.4,37938.5,38267.8,38600.9,38932.7,39262.6,39592.8,39924.3,40257.6,40591.4,40930.7,41271.2,41611.3,41950.8,42289.2,42626.1,42962.5,43298.1,43633.3,43968.2,44302.6,44637.1,44971.9,45306.6,45638.8,45970.4,46301.6,46632.8,46963.1,47293.4,47623.6,47950.8}, \
            {-12.0737,-6.05307,-2.53124,-0.0324699,1.90573,3.48936,4.82829,5.98813,7.01118,7.92633,8.75418,9.50995,10.2052,10.8489,11.4482,12.0087,12.5353,13.0318,13.5014,13.9469,14.3707,14.7748,15.1609,15.5306,15.8851,16.2258,16.5536,16.8695,17.1743,17.4688,17.7536,18.0293,18.2966,18.5559,18.8077,19.0524,19.2904,19.522,19.7476,19.9675,20.182,20.3913,20.5957,20.7954,20.9906,21.1815,21.3683,21.5512,21.7303,21.9057,22.0777,22.2464,22.4118,22.5742,22.7336,22.8901,23.0438,23.1949,23.3434,23.4894,23.6329,23.7742,23.9131,24.0499,24.1846,24.3172,24.4478,24.5765,24.7033,24.8283,24.9515,25.073,25.1928,25.311,25.4276,25.5426,25.6561,25.7682,25.8789,25.9881,26.096,26.2026,26.3079,26.4119,26.5147,26.6163,26.7167,26.816,26.9141,27.0112,27.1072,27.2021,27.296,27.3889,27.4808,27.5718,27.6618,27.7509,27.839,27.9263,28.0128,28.0983,28.1831,28.267,28.3501,28.4324,28.514,28.5948,28.6749,28.7542,28.8328,28.9107,28.9879,29.0644,29.1403,29.2155,29.29,29.364,29.4373,29.51,29.582,29.6535,29.7244,29.7948,29.8645,29.9337,30.0024,30.07}, \
            {1168.5,1052.21,908.428,808.374,733.614,672.619,620.78,577.403,542.12,506.945,463.079,417.994,375.58,336.703,299.58,264.032,230.741,200.383,173.864,151.592,133.174,118.27,106.634,97.7155,91.3265,87.0815,84.652,83.0677,82.6087,83.3968,84.8055,86.8035,89.5516,92.9945,97.0933,101.639,107.177,113.171,119.681,126.815,134.502,142.336,150.584,159.138,168.065,177.184,186.35,195.794,205.364,214.856,224.461,234.033,243.616,252.892,262.082,271.056,279.784,288.29,296.57,304.325,311.763,318.897,325.706,332.156,338.37,344.284,349.565,354.635,359.581,364.279,368.733,373.164,377.597,381.819,385.993,390.379,394.969,399.701,404.396,409.213,414.314,419.503,424.865,430.312,435.955,441.775,447.624,453.621,459.404,464.985,470.789,476.686,482.496,487.292,491.52,495.757,500.018,504.135,507.641,510.972,514.134,517.237,520.365,523.268,525.245,526.721,527.787,528.951,530.033,531.17,532.614,534.676,537.522,541.057,544.614,547.902,551.15,554.172,556.629,558.227,559.245,559.249,559.249,559.249,559.249,559.249,559.249,559.249},
            {5,15,40,60}, {5.675,14.575,24,28.1},
            {5,15,40,47,60},{5.15,14.13,23.85,25.35,27.69}
         },\

         {
            124, 21, {346.53,732.022,1133.73,1546.21,1967.04,2394.12,2826.54,3262.28,3710.19,4181.27,4658.75,5143.04,5634.06,6128.94,6622.27,7108.97,7588.1,8060.72,8528.28,8991.21,9449.2,9901.25,10347.7,10787.5,11220,11646.9,12068.4,12485.9,12899.4,13309.6,13716.9,14121.5,14523.4,14922.5,15318.8,15712,16102.4,16489.3,16873.9,17254.5,17632.4,18008.2,18380.5,18751.6,19120.6,19487.3,19853,20217.5,20581.2,20943.6,21305.3,21667,22028.2,22387.9,22747.4,23106.9,23464.8,23822.1,24178.2,24533.5,24887.3,25239.7,25590.2,25939.2,26286.8,26632.8,26977.1,27318.8,27659.2,27998.8,28336.9,28673.7,29007.9,29341.6,29675.1,30007.6,30339.3,30670.1,31000.7,31331.5,31662.5,31993.1,32323.4,32654.1,32986.1,33317.6,33649.4,33981.9,34314.7,34649.7,34984.1,35318.6,35653.3,35988.5,36325.1,36663.9,37002.1,37340,37677.6,38015.8,38354.3,38696.6,39041,39384.7,39728.8,40072.1,40415.4,40759.3,41104.3,41447.5,41791.2,42134.3,42478.8,42824.6,43174.2,43524.2,43872.5,44220.8,44568.9,44915,45258,45590.2,45919.4,46248.6,46576.9,46905.3,47233.4,47558.5,}, \
            {-11.0837,-5.06307,-1.54124,0.95753,2.89573,4.47936,5.81829,6.97813,8.00118,8.91633,9.74418,10.5,11.1952,11.8389,12.4382,12.9987,13.5253,14.0218,14.4914,14.9369,15.3607,15.7648,16.1509,16.5206,16.8751,17.2158,17.5436,17.8595,18.1643,18.4588,18.7436,19.0193,19.2866,19.5459,19.7977,20.0424,20.2804,20.512,20.7376,20.9575,21.172,21.3813,21.5857,21.7854,21.9806,22.1715,22.3583,22.5412,22.7203,22.8957,23.0677,23.2364,23.4018,23.5642,23.7236,23.8801,24.0338,24.1849,24.3334,24.4794,24.6229,24.7642,24.9031,25.0399,25.1746,25.3072,25.4378,25.5665,25.6933,25.8183,25.9415,26.063,26.1828,26.301,26.4176,26.5326,26.6461,26.7582,26.8689,26.9781,27.086,27.1926,27.2979,27.4019,27.5047,27.6063,27.7067,27.806,27.9041,28.0012,28.0972,28.1921,28.286,28.3789,28.4708,28.5618,28.6518,28.7409,28.829,28.9163,29.0028,29.0883,29.1731,29.257,29.3401,29.4224,29.504,29.5848,29.6649,29.7442,29.8228,29.9007,29.9779,30.0544,30.1303,30.2055,30.28,30.354,30.4273,30.5,30.572,30.6435,30.7144,30.7848,30.8545,30.9237,30.9924,31.06}, \
            {1045.87,963.085,866.438,798.483,747.736,706.419,671.511,642.399,619.571,598.752,563.52,522.377,479.477,438.713,401.142,367.541,338.056,312.582,290.987,273.231,258.674,247.12,238.729,233.338,230.921,230.867,232.705,235.642,239.869,245.176,250.978,257.273,263.825,270.635,277.925,285.583,293.254,301.584,310.236,318.968,327.795,336.889,346.213,355.649,365.105,374.437,383.792,392.825,401.617,410.587,419.423,427.837,436.081,444.387,452.412,460.223,467.715,475.232,482.681,490.083,497.353,504.407,511.529,518.699,525.799,532.739,539.796,546.553,553.094,559.713,566.502,573.041,579.286,585.551,591.903,598.162,604.256,610.04,615.654,621.177,626.719,632.222,637.666,642.667,647.448,652.178,656.978,661.871,666.652,671.111,675.528,679.956,684.18,688.613,693.07,697.464,701.739,705.92,710.117,714.534,719.375,724.792,730.391,735.786,740.932,746.189,752.182,758.566,765.099,771.694,778.516,785.353,792.511,800.354,809.013,817.72,825.394,832.776,840.107,846.75,851.471,854.005,854.437,854.437,854.437,854.437,854.437,854.437}, \
            {5,15,40,60}, {6.6,15.65,25.2,29.2},
            {5,15,40,47,60},{6.18,15.24,24.8,26.39,28.81}
         }
      }
   };

   memcpy(charTableV3_V4, charV4TableV3_V4, sizeof(char_DPD_Tbl_V3_V4) * 3 * 4);
}

void ComputeV4_V3_Delta()
{
   int iTotalCnt = 0;
   double dCumDiff = 0.0;

   //For multiple bands, find the cummulative difference and average for each band
   /*This example is for one band*/
   for(int chanCnt = 0; chanCnt < 3; chanCnt++)
   {
      for(int rgiCnt = 0; rgiCnt < 4; rgiCnt++)
      {
         for(int V3DigIdx = 0; V3DigIdx < 4; V3DigIdx++) //in this example there are 4 V3 digital gains, 5,15,40,60
         {
            int V3DigGain = charTableV3_V4[chanCnt][rgiCnt].charDigGain_V3[V3DigIdx];
            for(int V4DigIdx = 0; V4DigIdx < 5; V4DigIdx++) //in this example there are 5 V4 digital gains, 5,15,40,47,60
            {
               int V4DigGain = charTableV3_V4[chanCnt][rgiCnt].charDigGain_V4[V4DigIdx];
               //check for digital gain matching and check for V3 power between 11.5 and 29.5dBm
               if(V3DigGain == V4DigGain && (charTableV3_V4[chanCnt][rgiCnt].charDigPwr_V3[V3DigIdx] >= minDigGainDeltaPwr_dB && charTableV3_V4[chanCnt][rgiCnt].charDigPwr_V3[V3DigIdx]<= maxDigGainDeltaPwr_dB))
               {
                  dCumDiff += (charTableV3_V4[chanCnt][rgiCnt].charDigPwr_V4[V4DigIdx] - charTableV3_V4[chanCnt][rgiCnt].charDigPwr_V3[V3DigIdx]);
                  iTotalCnt++;
                  break;
               }
            }
         }
      }
   }
   if(iTotalCnt != 0)
   {
      charV4_V3_Delta[0] = dCumDiff/(double)iTotalCnt; //only one band in this example and hence index is 0.
   }
}

void ValidateCalAlg( GSM_V4_Cal_Params& gsmV4CalParams )
{
   ostringstream ostr;

   if (!gsmV4CalParams.gsmV4DAParams.boolPerformDA && !gsmV4CalParams.gsmV4RxParams.boolPerformRxCal)
   {
      //throw exception Error: Both DA and Rx cal are disabled, which is not allowed. "
   }
   if (!gsmV4CalParams.gsmV4DAParams.boolPerformDA && gsmV4CalParams.gsmV4DPDParams.boolPerformDPD)
   {
      //throw exception "Error: DPD cal can not be enabled without DA cal. "
   }
   // make sure this is false if DA cal is not being run.
   if(!gsmV4CalParams.gsmV4DAParams.boolPerformDA)
   {
       gsmV4CalParams.gsmV4DPDParams.boolDoRGIRunOnly = false;
   }
}

void ValidateChannels(GSM_V4_Cal_Params& gsmV4CalParams )
{
   if(gsmV4CalParams.uiNumChan < gsmV4_minNumChan || gsmV4CalParams.uiNumChan > gsmV4_maxNumChan)
   {
      //throw exception Number of channels must be between 1 and 3
   }

   unsigned int curChan     = 0;
   unsigned int curBand     = 0;
   bool isValidChannel = true;
   //verify channels for each band
   for (unsigned int bandIdx=0; bandIdx<gsmV4CalParams.uiNumBands; ++bandIdx)
   {
      curBand = gsmV4CalParams.uiBandList[bandIdx];

      // verify the same # of channels per band
      if( gsmV4CalParams.uiChanList[bandIdx].size() != gsmV4CalParams.uiNumChan )
      {
         //throw exception "Error: Channel List should have the same # of channels per band. "
      }

      for (unsigned int chanIdx=0; chanIdx<gsmV4CalParams.uiNumChan; ++chanIdx)
      {
         curChan = gsmV4CalParams.uiChanList[bandIdx][chanIdx];
         switch( curBand )
         {
            case NV_MODE_GSM_EGSM:
               if ( curChan < gsmV4_e900_minChan || curChan > gsmV4_e900_maxChan )
               {
                  isValidChannel = false;
               }
               break;
            case NV_MODE_GSM_850:
               if ( curChan < gsmV4_g850_minChan || curChan > gsmV4_g850_maxChan )
               {
                  isValidChannel = false;
               }
               break;
            case NV_MODE_GSM_DCS:
               if ( curChan < gsmV4_g1800_minChan || curChan > gsmV4_g1800_maxChan )
               {
                  isValidChannel = false;
               }
               break;
            case NV_MODE_GSM_1900:
               if ( curChan < gsmV4_g1900_minChan  || curChan > gsmV4_g1900_maxChan )
               {
                  isValidChannel = false;
               }
               break;
         }
         if ( !isValidChannel )
         {
            //throw exceptin "Error: Invalid channel";
         }
      }
   }
}

void ValidatePCLs(GSM_V4_Cal_Params& gsmV4CalParams )
{
   // verify that the number of PCLs is an even factor of 4
   if ( (gsmV4CalParams.gsmV4DAParams.uiNumPCLs % 4) > 0 )
   {
      //throw exception "Error: "PCL List" should have 4, 8, 12, or 16 elements. "
   }

   // if Rx is enabled, more PCLs are required
   if (gsmV4CalParams.gsmV4RxParams.boolPerformRxCal)
   {
      // if only 1 modulation, than gsmV4_maxNumPCL PCLs are required
      if (gsmV4CalParams.gsmV4DAParams.uiNumMods == 1)
      {
         if( gsmV4CalParams.gsmV4DAParams.uiNumPCLs != gsmV4_maxNumPCL )
         {
            //throw exception "Error: "PCL List" must have 16 elements, when Rx is enabled, with 1 Tx modulation.  "
         }
      }
      if (gsmV4CalParams.gsmV4DAParams.uiNumMods == 2)
      {
         if( gsmV4CalParams.gsmV4DAParams.uiNumPCLs != 8 && gsmV4CalParams.gsmV4DAParams.uiNumPCLs != gsmV4_maxNumPCL )
         {
            //throw exception "Error: "PCL List" must have 8 or 16 elements, when Rx is enabled, with 2 Tx modulations.  "
         }
      }
   }

   unsigned int nvMode   = 0;
   unsigned short minPCL = gsmV4_minLowBand_PCL;
   unsigned short maxPCL = gsmV4_maxLowBand_PCL;
   unsigned int curPCL   = minPCL;

   for (unsigned char bandIdx=0; bandIdx<gsmV4CalParams.uiNumBands; ++bandIdx)
   {
      // verify the same # of PCLs for each band
      if ( gsmV4CalParams.gsmV4DAParams.uiPclList[bandIdx].size() != gsmV4CalParams.gsmV4DAParams.uiNumPCLs )
      {
         //throw exception "Error: "PCL List" should have the same # of PCLs per band. "
      }

      nvMode = gsmV4CalParams.uiBandList[bandIdx];
      switch(nvMode)
      {
         // checking the low band PCLs
         case NV_MODE_GSM_EGSM:
         case NV_MODE_GSM_850:
            // the low band values are the default
            break;
         case NV_MODE_GSM_DCS:
         case NV_MODE_GSM_1900:
            minPCL = gsmV4_minHighBand_PCL;
            maxPCL = gsmV4_maxHighBand_PCL;
            break;
         default:
            //throw exception "Unknown NV Mode"
            break;
      }

      // validate the PCL list
      for (unsigned char i=0; i < gsmV4CalParams.gsmV4DAParams.uiNumPCLs; ++i)
      {
         curPCL = gsmV4CalParams.gsmV4DAParams.uiPclList[bandIdx][i];

         if ( (curPCL<minPCL) || (curPCL>maxPCL) )
         {
            //throw exception: PCL is not within min and max
         }
      }
   }
}

void ValidateMCS(GSM_V4_Cal_Params& gsmV4CalParams )
{
   if (gsmV4CalParams.gsmV4DAParams.uiNumMods == 0)
   {
      //throw exception "Error: No MCS specified - DA Cal is enabled. "
   }

   // forcing the number of modulations to be 2
   if( gsmV4CalParams.gsmV4DAParams.uiNumMods !=  gsmV4_maxNumMods )
   {
      //throw exception "Error: "Number of Modulations" should equal 2");
   }

   unsigned int curMCS = gsmV4_minMCS;

   // validate the MCS List
   for (unsigned char i=0; i<gsmV4CalParams.gsmV4DAParams.uiNumMods; ++i)
   {
      curMCS = gsmV4CalParams.gsmV4DAParams.uiModList[i];
      if ( (curMCS<gsmV4_minMCS) || (curMCS > gsmV4_maxMCS) )
      {
         //throw exception"MCS is not between 1 and 9"
      }
   }
}

void ValidateDPD( GSM_V4_Cal_Params& gsmV4CalParams )
{
   if (!gsmV4CalParams.gsmV4DPDParams.boolPerformDPD && !gsmV4CalParams.gsmV4DPDParams.boolDoRGIRunOnly)
   {
      // The params must be resize to numBands in order for MultiSweep to work
      gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList.resize(gsmV4CalParams.uiNumBands);
      gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList.resize(gsmV4CalParams.uiNumBands);
      gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList.resize(gsmV4CalParams.uiNumBands);
      gsmV4CalParams.gsmV4DPDParams.uiNumDpdDigGain = 0;
      gsmV4CalParams.gsmV4DPDParams.uiNumDpdRgi = 0;
      return;
   }
   if(gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList.size() == 0)
   {
      //throw exception DPD RGI list cannot be empty
   }
   if(gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList.size() == 0)
   {
      //throw exception DPD digital gain list cannot be empty
   }

   if( (gsmV4CalParams.gsmV4DPDParams.uiNumDpdRgi > gsmV4_maxNumDpdRgi) || (gsmV4CalParams.gsmV4DPDParams.uiNumDpdRgi < gsmV4_minNumDpdRgi) )
   {
      //throw exception "Error: "DPD RGI List" should have between 3 and 5"
   }

   // checking RGIs
   for (unsigned int bandIdx=0; bandIdx<gsmV4CalParams.uiNumBands; ++bandIdx)
   {
      if( gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[bandIdx].size() != gsmV4CalParams.gsmV4DPDParams.uiNumDpdRgi)
      {
         //throw exception "Error: "DPD RGI List" should have the same # of RGIs per band. "
      }
      for (unsigned short i=0; i<gsmV4CalParams.gsmV4DPDParams.uiNumDpdRgi; ++i)
      {
         if (gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[bandIdx][i] < gsmV4_minRGI || gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[bandIdx][i] > gsmV4_maxRGI)
         {
            //throw exception "Error: Invalid DPD Cal RGI"
         }
      }
   }

   // checking Digi gains
   for (unsigned int bandIdx=0; bandIdx<gsmV4CalParams.uiNumBands; ++bandIdx)
   {
       if(gsmV4CalParams.gsmV4DPDParams.boolDoRGIRunOnly)
       {
            if(gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx].size() != gsmV4_reqNumDpdGainRGI_Only)
            {
                //trhow exception "DPD digi gain list should have 1 item
            }
       }
       else
       {
          if( gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx].size() > gsmV4_maxNumDpdGain || gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx].size() < gsmV4_minNumDpdGain)
          {
             //throw exception "Error: "DPD digi gain List" should have elements between <min> and  <max> "
          }
       }
      for (unsigned short i=0; i<gsmV4CalParams.gsmV4DPDParams.uiNumDpdDigGain; ++i)
      {
         if (gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx][i] < gsmV4_minDpdGain || gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx][i] > gsmV4_maxDpdGain)
         {
            //throw exception "Error: Invalid DPD digi gain"
         }
      }
   }

   //----- Check the digital gain for predistortion RGI selection
   if(!gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation)
   {
   if (gsmV4CalParams.gsmV4DPDParams.usDigGainForPredistRGI < gsmV4_minDpdGain || gsmV4CalParams.gsmV4DPDParams.usDigGainForPredistRGI > gsmV4_maxDpdGain)
   {
      //throw exception "Error: Invalid DPD Digital Gain for predist RGI selection"
   }
   // verify the digGainForPredistRGI is in the digital gain list
   vector<unsigned int>::iterator it;

   for (unsigned int bandIdx=0; bandIdx<gsmV4CalParams.uiNumBands; ++bandIdx)
   {
      for (unsigned int digGainIdx=0; digGainIdx<gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx].size(); ++digGainIdx)
      {
         it = find(gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx].begin(), gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx].end(), gsmV4CalParams.gsmV4DPDParams.usDigGainForPredistRGI );

         if ( it == gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx].end() )
         {
            //throw exception "Error: DPD Digital Gain for predist RGI is not included in the digital gain list list for band: "
         }
      }
   }
   }
      // use default slope correction settings for case of params.m_DPD_Params.doRGIRunOnly
   if(gsmV4CalParams.gsmV4DPDParams.boolDoRGIRunOnly)
   {
       // these values aren't really used in this case. They are just placeholders.
      vector<unsigned int> defaultSlopCorrVec(3);
      defaultSlopCorrVec[0] = 5;
      defaultSlopCorrVec[1] = 15;
      defaultSlopCorrVec[2] = 40;

      gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList.resize(gsmV4CalParams.uiNumBands);

      for (unsigned int bandIdx=0; bandIdx<gsmV4CalParams.uiNumBands; ++bandIdx)
      {
         gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[bandIdx] = defaultSlopCorrVec;
      }
   }

   // verify the slope correction digital gains are included in the digital gain list
   if ( gsmV4CalParams.gsmV4DPDParams.boolDoSlopeCorrection ) // no need to check, if slope correction isn't being performed
   {
      vector<unsigned int>::iterator it;

      for (unsigned int bandIdx=0; bandIdx<gsmV4CalParams.uiNumBands; ++bandIdx)
      {
         for (unsigned int digGainIdx=0; digGainIdx<gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[bandIdx].size(); ++digGainIdx)
         {
            it = find(gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx].begin(), gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx].end(), gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[bandIdx][digGainIdx] );

            if ( it == gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx].end() )
            {
               //throw exception "Error: Slope correction digital gain list contains values not included in the DPD digital gain list for band: "
            }
         }
      }
   }
}
void ValidateGSMV4LNA(GSM_V4_Cal_Params& gsmV4CalParams)
{
    if (!gsmV4CalParams.gsmV4RxParams.boolPerformRxCal)
    {
        return;
    }

   if(gsmV4CalParams.gsmV4RxParams.uiNumLNAState == 0)
   {
      // throw exception "Error: No LNA states specified - Rx cal is enabled. "
   }
   // force to have 4 LNA states for now
   if( gsmV4CalParams.gsmV4RxParams.uiNumLNAState !=  gsmV4_maxNumLNA  )
   {
      //throw exception  "Error: Number of LNA States should be equal to " gsmV4_maxNumLNA  ;
   }

   for (unsigned int lnaIdx=0; lnaIdx < gsmV4CalParams.gsmV4RxParams.uiNumLNAState; ++lnaIdx)
   {
      if ( gsmV4CalParams.gsmV4RxParams.uiLNAStateList[lnaIdx] > gsmV4_maxLNA )
      {
         // throw exception "Error: LNA state is invalid.";
      }
   }
}

void ValidateGSMV4Slots( GSM_V4_Cal_Params& gsmV4CalParams )
{

   //--------- Tx Slots ---------//
   if (gsmV4CalParams.gsmV4DAParams.boolPerformDA)
   {
      if( gsmV4CalParams.uiNumTxSlots > gsmV4_maxNumTxSlots )
      {
         //throw exception Error: Number of Tx slots should have less than or equal to gsmV4_maxNumTxSlots;
      }

      if (gsmV4CalParams.uiNumTxSlots == 0)
      {
         //throw exception "Error: No Tx slots specified - DA/DPD cal is enabled. "
      }
   }

   //--------- Rx Slots ---------//
   if (gsmV4CalParams.gsmV4RxParams.boolPerformRxCal)
   {
      if( gsmV4CalParams.uiNumRxSlots > gsmV4_maxNumRxSlots )
      {
         // throw exception Error: Number of Rx slots should have less than or equal to << gsmV4_maxNumRxSlots;
      }
      if (gsmV4CalParams.uiNumRxSlots == 0)
      {
         // throw exception "Error: No Rx slots specified - Rx cal is enabled. ";
      }
   }

}
void ValidateGSMV4RxPow( GSM_V4_Cal_Params& gsmV4CalParams )
{
   if (!gsmV4CalParams.gsmV4RxParams.boolPerformRxCal)
   {
      return;
   }

   if (gsmV4CalParams.gsmV4RxParams.uiNumRxPower == 0)
   {
      // throw exception "Error: No LNA powers specified - Rx cal is enabled. ";
}

   for (unsigned int powIdx=0; powIdx < gsmV4CalParams.gsmV4RxParams.uiNumRxPower; ++powIdx)
   {
      if (gsmV4CalParams.gsmV4RxParams.iRxPowerList[powIdx] > gsmV4_maxRxPower )
      {
         //throw exception "Error: Rx power is invalid.";
      }
   }
}
void ValidateGSMV4Params(GSM_V4_Cal_Params& gsmV4CalParams)
{
    ValidateCalAlg(gsmV4CalParams);
    ValidateChannels(gsmV4CalParams);
    if(gsmV4CalParams.gsmV4DAParams.boolPerformDA)
    {
        ValidateMCS(gsmV4CalParams);
        ValidatePCLs(gsmV4CalParams);
        if (gsmV4CalParams.gsmV4DAParams.dPclNvPowOffset > gsmV4_maxPclNvPowOffset)
        {
            gsmV4CalParams.gsmV4DAParams.dPclNvPowOffset = gsmV4_maxPclNvPowOffset;
        }
        gsmV4CalParams.gsmV4DAParams.dMaxPclPowDelta = fabs(gsmV4CalParams.gsmV4DAParams.dMaxPclPowDelta);
    }
    else
    {
        gsmV4CalParams.gsmV4DAParams.uiNumMods = 2;
    }
    if(gsmV4CalParams.gsmV4RxParams.boolPerformRxCal)
    {
        ValidateGSMV4LNA(gsmV4CalParams);
        ValidateGSMV4RxPow(gsmV4CalParams);

        // verify that the number of Rx powers equals the number of LNA states
        if (gsmV4CalParams.gsmV4RxParams.uiNumRxPower != gsmV4CalParams.gsmV4RxParams.uiNumLNAState)
        {
            //"The number of Rx powers must equal the number of LNA states. "
        }
    }
    if (gsmV4CalParams.gsmV4DPDParams.boolDoSlopeCorrection && gsmV4CalParams.gsmV4DPDParams.boolDoPieceWiseLinear)
    {
        gsmV4CalParams.gsmV4DPDParams.boolDoPieceWiseLinear = false;
        //"Piecewise-linear and Slope adaptation can't both be enabled: defaulting to Slope.";
    }

    // make sure both piecewise and power aren't enabled at the same time
    if (gsmV4CalParams.gsmV4DPDParams.boolDoPowerAdaptation && gsmV4CalParams.gsmV4DPDParams.boolDoPieceWiseLinear)
    {
        gsmV4CalParams.gsmV4DPDParams.boolDoPowerAdaptation = false;
        //"Piecewise-linear and Power adaptation can't both be enabled: disabling Power Adapt." ;
    }

    gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation = false;
    // If we are doing DA-only cal, without dpd-based RGI select, then set DA adaptation based predist RGI selection to true.
   if ( gsmV4CalParams.gsmV4DAParams.boolPerformDA && !gsmV4CalParams.gsmV4DPDParams.boolPerformDPD && !gsmV4CalParams.gsmV4DPDParams.boolDoRGIRunOnly )
   {
      gsmV4CalParams.gsmV4DPDParams.boolDAAdaptation = true;
   }

    ValidateDPD(gsmV4CalParams);
    if (gsmV4CalParams.gsmV4DPDParams.boolDoAdaptiveMaxPwrEst && (gsmV4CalParams.gsmV4DPDParams.uiNumDpdDigGain < 4) && gsmV4CalParams.gsmV4DPDParams.boolPerformDPD )
    {
        gsmV4CalParams.gsmV4DPDParams.boolDoAdaptiveMaxPwrEst = false;
        //"Adaptive Max Power Estimation can't both be enabled with < 4 digital gains.";
    }
    ValidateGSMV4Slots(gsmV4CalParams);
}
/*Call box capabilities*/
void InitTECapabilities(TECapabilities& teCapabilities, GSM_V4_Cal_Params& gsmV4CalParams)
{
   teCapabilities.boolSupportsFrameSweep = true; //8820B and CMU does not support frame sweep; true for others
   teCapabilities.boolSupportSimultaneousTxRx = true; //false of 8960; Rx runs separately in 8960
   teCapabilities.iNumFramesPerStep = 1; //4 for 8960 (frame duplication)
   teCapabilities.iMaxFramesPerSweep = 2000/8; //200 for 8820, 8870, 50 - 8960, 100 -CMU, 2000/8 - CMW
   teCapabilities.iMaxSlotsPerFrame = 8; //8-CMW, 7-8820_C,8870A, 8960 (based on options)
   gsmV4CalParams.boolSkipFirstRxMeas = false; //true for 8820, 8870; false for the rest
}
bool Build_GSM_V4_SweepConfig( GSM_V4_Cal_Params& gsmV4CalParams, bool& skipDoGsmV4Cal, TECapabilities& teCapabilities, GSMV4_SweepConfig& m_sweepConfig )
{
   //check callbox capabilities

   // first, check if frame-sweep support exists
   if (!teCapabilities.boolSupportsFrameSweep)
   {
      //throw exception "Call box not supported for GSM V4. ";
   }

   //----- 1) Copy over the original band-enumerated and chain-enumerated parameters
   m_sweepConfig.orgNumBand = static_cast<unsigned short>(gsmV4CalParams.uiNumBands);
   //m_sweepConfig.orgNumChan = static_cast<unsigned short>(gsmV4CalParams.uiNumChan);

   m_sweepConfig.orgBandVec = gsmV4CalParams.uiBandList;
   m_sweepConfig.orgChanVec = gsmV4CalParams.uiChanList;

   m_sweepConfig.orgGainOffset_MinLim_List      = gsmV4CalParams.gsmV4RxParams.iGainOffsetMinLimList;
   m_sweepConfig.orgGainOffset_MaxLim_List      = gsmV4CalParams.gsmV4RxParams.iGainOffsetMaxLimList;
   m_sweepConfig.orgPclIdxList                  = gsmV4CalParams.gsmV4DAParams.usPclIdxList;
   m_sweepConfig.orgDpd_RgiList                 = gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList;
   m_sweepConfig.orgDpd_DigGainList             = gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList;
   m_sweepConfig.orgSlopeCorrectionDigGainList  = gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList;

   m_sweepConfig.orgNumRxChain = static_cast<unsigned short>(gsmV4CalParams.uiNumRxChains);
   m_sweepConfig.orgNumTxChain = static_cast<unsigned short>(gsmV4CalParams.uiNumTxChains);
   m_sweepConfig.orgRxChainIDs = gsmV4CalParams.uiRxChainList;
   m_sweepConfig.orgTxChainIDs = gsmV4CalParams.uiTxChainList;
   // The number of frames requested by the Tree Params for the call box perspective.
   // Nominally numRequestedFrames = numRequiredFrames
   unsigned int numRequestedFrames = gsmV4CalParams.uiNumFrames_CB; //includes intraframe delay

   // The number of required frames
   // - This is the # of frames required by the UE
   // - Nominally, this equals numRequestedFrames, but can be larger.
   unsigned int numRequiredFrames = numRequestedFrames;

   //---2  determine if a hopping frame is required
   m_sweepConfig.addHoppingFrame = false;
   if (teCapabilities.iNumFramesPerStep > 1)
   {
      m_sweepConfig.addHoppingFrame = true;
      // update the # of frames requested by the tree params to reflect the hopping frame requirement
      numRequiredFrames *= teCapabilities.iNumFramesPerStep;
   }
   m_sweepConfig.iNumFramesPerStep_CB = teCapabilities.iNumFramesPerStep;

   //---3 determine if non-frame Rx sweep is required
   if ( !teCapabilities.boolSupportSimultaneousTxRx )
   {
      gsmV4CalParams.gsmV4RxParams.boolPerformRxCal = false;
      gsmV4CalParams.uiNumRxSlots = 0;
   }

   //------ 4) Determine if Rx-Only calibration is being performed along with non-frame-sweep Rx
   // Break out of multi-sweep if only Rx cal is being performed, and teCapabilities.boolSupportSimultaneousTxRx is false
   // There is no point in going through the multi-sweep configuration or Do_GSM_V4_Cal in this case
   if ( !gsmV4CalParams.gsmV4DAParams.boolPerformDA && !gsmV4CalParams.gsmV4DPDParams.boolPerformDPD &&  !teCapabilities.boolSupportSimultaneousTxRx)
   {
      skipDoGsmV4Cal = true;
      return false;
   }

   //----- 5) Determine if Chain-segmentation of sweeps is required
   m_sweepConfig.segmentMultiChain = false;
   // if multiple chains are requested, see if the call box is capable of sweeping 2 chains in a single sweep
   //i.e call box should be able to make measurment is all 8 slots
   //Multichain support in phone SW will be available in future. For now segment multichain.
   if ( m_sweepConfig.orgNumTxChain > 1 /*&& teCapabilities.iMaxSlotsPerFrame < GSM_V4_MAX_NUM_OF_SLOTS*/ )
   {
      m_sweepConfig.segmentMultiChain = true;
   }

   //----- 6) Determine if Band-segmentation of sweeps is required
   // Frame Requirements:
   // 1) The number of requested frames can't exceed the call box capabilities
   // 2) The number of required frames can't exceed the FTM API capabilites
   bool segmentFrames = true;
   if ( (numRequestedFrames <= teCapabilities.iMaxFramesPerSweep) && (numRequiredFrames <= GSM_V4_MAX_NUM_OF_FRAMES)  )
   {
      // No need to break up the requested sweep into smaller sweeps in this case
      segmentFrames = false;
   }
   //------------ Possible Sweep Combinations: -----------//
   // A.) !segmentFrames && !m_sweepConfig.segmentMultiChain
   //      - this means that all bands/chains are swept in a single sweep
   //       Example: Sweep 1 --> Bands (1,2,3,4) Chains(0,2)
   // B.) segmentFrames && !m_sweepConfig.segmentMultiChain
   //      - this means the bands are broken up into multiple sweeps, but the chains are not
   //       Example: Sweep 1 --> Bands (1,2) Chains(0,2)
   //                Sweep 2 --> Bands (3,4) Chains(0,2)
   // C.) segmentFrames && m_sweepConfig.segmentMultiChain
   //      - this means both the bands and chains are broken up over multiple sweeps
   //       Example: Sweep 1 --> Bands (1,2) Chains(0)
   //                Sweep 2 --> Bands (3,4) Chains(0)
   //                Sweep 3 --> Bands (1,2) Chains(2)
   //                Sweep 4 --> Bands (3,4) Chains(2)

   //----- 7) Break out if no need to segment by band or segment by chain
   if ( !segmentFrames && !m_sweepConfig.segmentMultiChain )
   {
      // No need to break up the requested sweep into smaller sweeps in this case
      // Return false so that non-multi-sweep code is run
      // There will be only 1 sweep - All bands/chains are run at once
      m_sweepConfig.numSweeps        = 1;
      m_sweepConfig.numBandsPerSweep = m_sweepConfig.orgNumBand;

      return false;
   }

   //----- 8) Determine band break-up if segment by band is required
   if ( segmentFrames )
   {
      // Need to break up the requested # of frames into sub-sweeps

      // There will be multiple sweeps
      m_sweepConfig.numSweeps = static_cast<unsigned short>(
                                   ceil( static_cast<float>(numRequiredFrames) / static_cast<float>(teCapabilities.iMaxFramesPerSweep) ) );

      // make sure numSweeps and numBands are evenly divisible
      // sweeps are only subdivided into bands
      if ( (m_sweepConfig.orgNumBand % m_sweepConfig.numSweeps) != 0)
      {
         m_sweepConfig.numSweeps = m_sweepConfig.orgNumBand;
      }

      m_sweepConfig.numBandsPerSweep = m_sweepConfig.orgNumBand / m_sweepConfig.numSweeps;

      // Prepare m_curCalParam for Frame-Sweep Iteration
      // Band-Enumerated Parameters
      gsmV4CalParams.uiNumBands = m_sweepConfig.numBandsPerSweep;
      gsmV4CalParams.uiBandList.resize( m_sweepConfig.numBandsPerSweep );
      gsmV4CalParams.uiChanList.resize( m_sweepConfig.numBandsPerSweep );

      gsmV4CalParams.gsmV4RxParams.iGainOffsetMinLimList.resize( m_sweepConfig.numBandsPerSweep );
      gsmV4CalParams.gsmV4RxParams.iGainOffsetMaxLimList.resize( m_sweepConfig.numBandsPerSweep );
      gsmV4CalParams.gsmV4DAParams.usPclIdxList.resize( m_sweepConfig.numBandsPerSweep );
      gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList.resize( m_sweepConfig.numBandsPerSweep );
      gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList.resize( m_sweepConfig.numBandsPerSweep );
      gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList.resize( m_sweepConfig.numBandsPerSweep );
   }
   else
   {
      // There will be only 1 sweep - All bands are run at once
      m_sweepConfig.numSweeps        = 1;
      m_sweepConfig.numBandsPerSweep = m_sweepConfig.orgNumBand;
   }

   //----- 8) Determine chain break up if segment by chain is required
   if ( m_sweepConfig.segmentMultiChain )
   {
      m_sweepConfig.numChainsPerSweep = 1;

      gsmV4CalParams.uiNumRxChains = 1;
      gsmV4CalParams.uiNumTxChains = 1;
      gsmV4CalParams.uiRxChainList.resize( m_sweepConfig.numChainsPerSweep );
      gsmV4CalParams.uiTxChainList.resize( m_sweepConfig.numChainsPerSweep );

      //callbox
      // must also resize the call-box slot configuration to match that of the MTP, since there will only be uplink bursts in the 1st set of slots for chain 0
      gsmV4CalParams.ucTxSlotConfig_CB = gsmV4CalParams.ucTxSlotConfig;
   }

   return true;
}

void Do_NonFrameSweep_Char_Rx_Cal(GSM_V4_Cal_Params& gsmV4CalParams, GSM_V4_RSSI_Est_Chain& gsmV4RSSIResults, GSM_V4_RSSI_Est_Chain& gsmV4RSSIResultsHiLin)
{
   // must call TE_Preset.
   // Otherwise more than 1 run of this node will cause this Rx section to fail

   // Band and MODE variables
   FTM_RF_Mode_Enum mode;
   BAND_CLASS_ENUM  bandEnum;

   // The current channel, used as the reference channel
   unsigned int refChan = 0;

   // # of burst averages
   const int numBurstAvg = 5;

   int num_measurements = 0;

   // Group together LNA state (gain ranges) intended to measured the same power level
   map< int, vector<int> > batches;
   map< int, vector<int> >::iterator batches_iter;
   map< int, vector<int> > batches_HiLin;
   map< int, vector<int> >::iterator batches_iter_HiLin;

   vector<int> ranges;     // list of gain ranges or LNA states
   int curLevel = 0;       // the current power level
   int curLNA   = 0;       // the current LNA state or gain range

   // map< band, map< channel, map< LNA state, RSSI > > >
   GSM_V4_Rx_Results_Band_Chan bandChan_RssiMap_prx;
   GSM_V4_Rx_Results_Band_Chan bandChan_RssiMap_prx_HiLin;
   GSM_V4_Rx_Results_Band_Chan bandChan_RssiMap_drx;
   GSM_V4_Rx_Results_Band_Chan bandChan_RssiMap_drx_HiLin;
   // Pack all LNA states with the same power level together. This is done to minimize the # of call box triggers.
   // For example, if -90 dBm is used for LNA states 0 and 1, the # of measurements are cut in half.
   for( unsigned short rxIdx=0; rxIdx<gsmV4CalParams.gsmV4RxParams.uiNumRxPower; ++rxIdx )
   {
      ranges.clear();
      curLevel = gsmV4CalParams.gsmV4RxParams.iRxPowerList[rxIdx];

      curLNA = gsmV4CalParams.gsmV4RxParams.uiLNAStateList[ rxIdx ];

      //If already exists, get it to just expand
      if( batches.count( curLevel ) > 0 )
      {
         ranges = (*batches.lower_bound( curLevel )).second;
      }
      ranges.push_back( curLNA );
      batches[curLevel ] = ranges;
   }

   unsigned int m_curBand,curChainID;

   for (unsigned short curChainIdx=0; curChainIdx<gsmV4CalParams.uiNumRxChains; ++curChainIdx)
   {
      curChainID = gsmV4CalParams.uiRxChainList[curChainIdx];

      //------------------------------- Obtain all Reference Channel RSSIs -------------------------------//
      for (unsigned short bandIdx=0; bandIdx<gsmV4CalParams.uiNumBands; ++bandIdx)
      {
         m_curBand  = static_cast<unsigned short>( gsmV4CalParams.uiBandList[bandIdx] );
         mode     = static_cast<FTM_RF_Mode_Enum>( m_curBand );
         bandEnum = ConverBand_To_BandClassEnum( m_curBand);

#ifdef ON_TARGET
         //Set_RF_Mode( mode, curChainID );                  // set UUT chainID and Band
         QLIB_FTM_SET_MODE( g_hResourceContext, mode);
         // set TE band
#endif
         // pick the reference channel
         refChan = gsmV4CalParams.uiChanList[bandIdx][0];

         //--------------------------- UE/TE Configurations ----------------------//
         //! Setup equipment digital modulation
         //! Set Rx Path
#ifdef ON_TARGET
         //! Disable UUT Tx
         QLIB_FTM_SET_TX_OFF(g_hResourceContext);//Set_Tx_State( false );
         //! Configure GSM Rx Burst off
         QLIB_FTM_SET_RX_BURST( g_hResourceContext, 0 /*slot index*/, static_cast<short>(RX_FUNCTION_STOP_CONTINOUS_RX) );//GSM_Rx_Burst( 0, (enum_rx_function_type) RX_FUNCTION_STOP_CONTINOUS_RX );
         //!set the chain to correct value
         //Set_Rx_Path_Only( curChainID );

#endif
         //!set test equipment channel

#ifdef ON_TARGET
         QLIB_FTM_SET_MODE_ID( g_hResourceContext, static_cast<short>(FTM_MODE_ID_GSM) );
         QLIB_FTM_SET_CHAN( g_hResourceContext, static_cast<unsigned short>(refChan) );//Set_UL_Channel( refChan );
#endif
         num_measurements = 0;

         batches_iter = batches.begin();

         vector<int> rssis_prx;
         vector<int> rssis_prx_HiLin;
         vector<int> rssis_drx;
         vector<int> rssis_drx_HiLin;
         //------------------------------- Obtain the Measurements -------------------------------//
         for( unsigned short batchIdx=0; batchIdx<batches.size(); ++batchIdx, ++batches_iter)
         {
            //! Confige RF Generator level
            curLevel = (*batches_iter).first;
            //TE_Set_DL_Level( curLevel ); - set downlink level
            //TE_Set_DL_State( true ); - set downlink state

            //! Do GSM Gain Range Cal

#ifdef ON_TARGET
            vector<int> range_list = (*batches_iter).second;
            char rng_bit_mask = 0;
            unsigned long prxResults[8];
            unsigned long drxResults[8]
            for(UINT i=0; i<range_list.size(); ++i)
            {
               rng_bit_mask |= (char)pow(2.0, range_list[i]);
            }
            if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
            {
                QLIB_FTM_RX_GAIN_RANGE_CAL_PRX_DRX( g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), 5 /*burst avg*/,static_cast<unsigned char>((num_measurements==0)), prxResults,drxResults);
            }
            else
            {
                QLIB_FTM_RX_GAIN_RANGE_CAL( g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), 5 /*burst avg*/,static_cast<unsigned char>((num_measurements==0)), prxResults);
            }
            rssis_prx.clear();
            rssis_drx.clear();
            int range = 0;

            for(UINT range_index = 0; range_index<range_list.size(); ++range_index)
            {
               range = range_list[ range_index ];
               //the range value corresponds to the bit position
               //which in turn is the corresponding measurement index
               rssis_prx.push_back( prxresults[ range ] );
                if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
                {
                    rssis_drx.push_back( drxresults[ range ] );
                }
            }
#else
            //for rx level of -90
            if(curLevel == -90.0)
            {
               rssis_prx.push_back(191799);
               rssis_prx.push_back(11441);
            }
            else //-54.0
            {
               rssis_prx.push_back(48332768);
               rssis_prx.push_back(150385);
            }
            if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
            {
                if(curLevel == -90.0)
                {
                    rssis_drx.push_back(191700);
                    rssis_drx.push_back(11400);
            }
            else //-54.0
            {
                    rssis_drx.push_back(48332700);
                    rssis_drx.push_back(150300);
                }
            }
#endif
            ++num_measurements;

            for(unsigned short rssiIdx=0; rssiIdx<rssis_prx.size(); ++rssiIdx)
            {
               curLNA = (*batches_iter).second[rssiIdx];

               bandChan_RssiMap_prx.rssi_Map[m_curBand].chanToLNA_AvgRSSI[refChan][curLNA] = rssis_prx[rssiIdx];
               if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
               {
                   bandChan_RssiMap_drx.rssi_Map[m_curBand].chanToLNA_AvgRSSI[refChan][curLNA] = rssis_drx[rssiIdx];
               }
            }
         }
         if( gsmV4CalParams.gsmV4RxParams.boolPerformHiLinRxCal )
         {
            batches_iter_HiLin = batches_HiLin.find( curLevel );
            if( batches_iter_HiLin != batches_HiLin.end() )
            {
               //switch the DUT to high linearity mode
#ifdef ON_TARGET
                QLIB_FTM_RX_SET_LINEARITY(g_hResourceContext, static_cast<unsigned short>(mode), refChan, 1);// 1 - high lieanrity; 0 - low linearity

                vector<int> range_list = (*batches_iter).second;
                char rng_bit_mask = 0;
                unsigned long prxResults[8];
                unsigned long drxResults[8]
                for(UINT i=0; i<range_list.size(); ++i)
                {
                   rng_bit_mask |= (char)pow(2.0, range_list[i]);
                }
                if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
                {
                    QLIB_FTM_RX_GAIN_RANGE_CAL_PRX_DRX( g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), 5 /*burst avg*/,static_cast<unsigned char>((num_measurements==0)), prxResults,drxResults);
                }
                else
                {
                    QLIB_FTM_RX_GAIN_RANGE_CAL( g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), 5 /*burst avg*/,static_cast<unsigned char>((num_measurements==0)), prxResults);
                }
                rssis_prx_HiLin.clear();
                rssis_drx_HiLin.clear();
                int range = 0;

                for(UINT range_index = 0; range_index<range_list.size(); ++range_index)
                {
                   range = range_list[ range_index ];
                   //the range value corresponds to the bit position
                   //which in turn is the corresponding measurement index
                   rssis_prx_HiLin.push_back( prxresults[ range ] );
                    if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
                    {
                        rssis_drx_HiLin.push_back( drxresults[ range ] );
                    }
                }
#else
                //for rx level of -90
                if(curLevel == -90.0)
                {
                   rssis_prx.push_back(211799);
                   rssis_prx.push_back(13441);
                }
                else //-54.0
                {
                   rssis_prx.push_back(50332768);
                   rssis_prx.push_back(170385);
                }
                if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
                {
                    if(curLevel == -90.0)
                    {
                        rssis_drx.push_back(211700);
                        rssis_drx.push_back(13400);
                    }
                    else //-54.0
                    {
                        rssis_drx.push_back(50332700);
                        rssis_drx.push_back(170300);
                    }
                }
#endif
                for(unsigned short rssiIdx = 0; rssiIdx < rssis_prx_HiLin.size(); ++rssiIdx)
                {
                    curLNA = (*batches_iter_HiLin).second[rssiIdx];

                    bandChan_RssiMap_prx_HiLin.rssi_Map[m_curBand].chanToLNA_AvgRSSI[refChan][curLNA] = rssis_prx_HiLin[rssiIdx];
                    if ( gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
                    {
                      bandChan_RssiMap_drx_HiLin.rssi_Map[m_curBand].chanToLNA_AvgRSSI[refChan][curLNA] = rssis_drx_HiLin[rssiIdx];
                    }
                }
#ifdef ON_TARGET
                 //switch the DUT back to low linearity mode
                QLIB_FTM_RX_SET_LINEARITY(g_hResourceContext, static_cast<unsigned short>(mode), refChan, 1);// 1 - high lieanrity; 0 - low linearity
#endif
            }
         }
      }
#ifdef ON_TARGET
      //set Rx Off
      QLIB_FTM_SET_RX_BURST( g_hResourceContext, static_cast<unsigned char>(0), static_cast<short>(RX_FUNCTION_STOP_CONTINOUS_RX) );
#endif

      //----------------------------------  Process the RSSI results --------------------------------//
      Estimate_RSSI_From_CharData( gsmV4CalParams, bandChan_RssiMap_prx, gsmV4RSSIResults.chainData[curChainID], curChainID,false );
      if( gsmV4CalParams.gsmV4RxParams.boolPerformHiLinRxCal )
      {
          Estimate_RSSI_From_CharData( gsmV4CalParams, bandChan_RssiMap_prx_HiLin, gsmV4RSSIResultsHiLin.chainData[curChainID], curChainID, true );
      }
      if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
      {
          unsigned int dRxChain = curChainID + 1;
          Estimate_RSSI_From_CharData( gsmV4CalParams, bandChan_RssiMap_prx, gsmV4RSSIResults.chainData[dRxChain], dRxChain,false );
          if( gsmV4CalParams.gsmV4RxParams.boolPerformHiLinRxCal )
          {
              Estimate_RSSI_From_CharData( gsmV4CalParams, bandChan_RssiMap_prx_HiLin, gsmV4RSSIResultsHiLin.chainData[dRxChain], dRxChain, true );
          }
      }
   }
}

/*Do_GSM_V4_Cal fucntion calls other functions to configure the sweep packets, execute and collect measurement results*/
void Do_GSM_V4_Cal(GSM_V4_Cal_Params& gsmV4CalParams, GSM_V4_PCL_Results_Chain& gsmV4PCLResults,
   GSM_V4_DPD_Results_Chain& gsmV4DPDResults, GSM_V4_RSSI_Est_Chain& gsmV4RSSIResults, GSM_V4_RSSI_Est_Chain& gsmV4RSSIResultsHiLin, GSM_V4_Meas_Info& measInfo, GSMV4_SweepConfig& m_sweepConfig)
{
   unsigned int uiCmdBufSize = 0;
   unsigned char GSMCmdBuf[GSM_V4_TEMP_CMD_BUFF_SIZE];

   // identifies which frames had Rx burst set in them
   map<unsigned int, vector<unsigned short> > RX_BURST_FRAME_IDENTIFIER;

   /*Create the frame sweep packet*/
   if(gsmV4CalParams.gsmV4DAParams.boolPerformDA)
   {
      //If the call box needs a hopping/dummy frame then do frame duplication
      if (m_sweepConfig.addHoppingFrame)
      {
         Create_Config_Command_FrameDuplication(gsmV4CalParams, GSMCmdBuf, uiCmdBufSize,  m_sweepConfig.iNumFramesPerStep_CB);
      }
      else
      {
         Create_Config_Command(gsmV4CalParams, GSMCmdBuf, uiCmdBufSize,RX_BURST_FRAME_IDENTIFIER);
      }
   }
   else //if only Rx cal is performed
   {
      Create_Config_Command_RxOnly(gsmV4CalParams, GSMCmdBuf, uiCmdBufSize,RX_BURST_FRAME_IDENTIFIER);
   }

   //------------------------------------ Create Call Box Command -------------------------------------------//

   //Set up TE and start the sweep
   //------------------------------------ Set up Test Equipment ---------------------------------------//

   ////----------------------------- Execute command to run the sweep ----------------------------------//
#ifdef ON_TARGET
   //for each chain
   QLIB_FTM_SET_MODE_ID( g_hResourceContext, static_cast<short>(FTM_MODE_ID_GSM_EXTENDED_C) );
   QLIB_FTM_GSM_V4_DISABLE_PREDIST( g_hResourceContext,0); //0 is the chain idx

   //send config pkt to UE
   QLIB_FTM_SET_MODE_ID( g_hResourceContext, static_cast<short>(FTM_MODE_ID_GSM_EXTENDED_C) );
   QLIB_FTM_GSM_V4_CAL(g_hResourceContext, GSMCmdBuf, uiCmdBufSize); //fill up buffer and buffer size form above

   //excute sweep
   QLIB_FTM_SET_MODE_ID( g_hResourceContext, static_cast<short>(FTM_MODE_ID_GSM_EXTENDED_C) );
   QLIB_FTM_GSM_V4_CAL_EXECUTE_SWEEP(g_hResourceContext);
#endif

   //Add a delay of numFrames * 5 msec for CMW while performing Rx cal only

   if(gsmV4CalParams.gsmV4DAParams.boolPerformDA)
   {
      /////get Tx sweep results from TE
      //TE provides data for all 3 channels DA first and 3 channels DPD followed by intraband delay which is 4 frames.
      //Read all the data from TE and store only those slots where TxSlotConfig is set to 1. So, the output vector should contain
      //DA data for all 3 channels then DPD data for all 3 channels.
   }

   ////--------------------------- Get the Rx (Downlink) measurements ----------------------------------//

   GSM_V4_FTM_CMD_Response_Type gsmV4RxMeasResults;
   if(gsmV4CalParams.gsmV4RxParams.boolPerformRxCal)
   {
      Get_Rx_Results(gsmV4RxMeasResults, gsmV4CalParams);
   }
   else
   {
#ifdef ON_TARGET
      //cleanup
      QLIB_FTM_SET_MODE_ID( g_hResourceContext, static_cast<short>(FTM_MODE_ID_GSM_EXTENDED_C) );
      QLIB_FTM_GSM_V4_CAL_CLEANUP(g_hResourceContext);
#endif
   }

   // .......................stop the call box sweep...........................

   //-------------------------------- Tx Measurement Processing ------------------------------------//
   //segregate DA and DPD measurements
   unsigned int curChainID;
   if(gsmV4CalParams.gsmV4DAParams.boolPerformDA)
   {
      map< unsigned int, vector<double> > dDpdPowMap_Chain;
      map< unsigned int, vector<double> > dDaPowMap_Chain;

      vector<double> dDpdPowVec;
      dDpdPowVec.resize( measInfo.numTotalDPD_Meas);
      vector<double> dDaPowVec;
      dDaPowVec.resize( measInfo.numTotalDA_Meas );

      Segment_DPD_And_DA_Pwr(dDpdPowMap_Chain, dDaPowMap_Chain, measInfo, gsmV4CalParams);
      // Loop through Tx Chains to process Tx Results
      for (unsigned short curChainIdx=0; curChainIdx<gsmV4CalParams.uiNumTxChains; ++curChainIdx)
      {
         curChainID = gsmV4CalParams.uiTxChainList[curChainIdx];
         //-- Obtain the PCL Power Deltas
         Fill_PCL_Results( gsmV4PCLResults.chainData[curChainID], gsmV4CalParams, dDaPowMap_Chain[curChainID],curChainID);
         if(gsmV4CalParams.gsmV4DPDParams.boolPerformDPD || gsmV4CalParams.gsmV4DPDParams.boolDoRGIRunOnly)
         {
            Fill_DPD_Results(gsmV4DPDResults.chainData[curChainID],gsmV4CalParams, dDpdPowMap_Chain[curChainID]);
         }
      }
   }

   if(gsmV4CalParams.gsmV4RxParams.boolPerformRxCal)
   {
      GSM_V4_Rx_Results_Chain rssiMaps_Chain;
      GSM_V4_Rx_Results_Chain rssiMapsHiLin_Chain;
      /*Process the Rx measurements and filter out bad RSSI readings*/
      Process_Rx_Meas_Results( gsmV4RxMeasResults, rssiMaps_Chain,rssiMapsHiLin_Chain, gsmV4CalParams, RX_BURST_FRAME_IDENTIFIER);
      for (unsigned short chainIdx=0; chainIdx<gsmV4CalParams.uiNumRxChains; ++chainIdx)
      {
         /*Estimate RSSI for other channels based on char Data and measured reference channels data*/
         curChainID = gsmV4CalParams.uiRxChainList[chainIdx];
         Estimate_RSSI_From_CharData( gsmV4CalParams, rssiMaps_Chain.chainData[curChainID], gsmV4RSSIResults.chainData[curChainID], curChainID, false );
         if( gsmV4CalParams.gsmV4RxParams.boolPerformHiLinRxCal )
         {
            Estimate_RSSI_From_CharData( gsmV4CalParams, rssiMapsHiLin_Chain.chainData[curChainID], gsmV4RSSIResultsHiLin.chainData[curChainID], curChainID, true );
         }
         if(gsmV4CalParams.gsmV4RxParams.boolPerformDRxCal)
         {
             unsigned int dRxChain = curChainID+1;
             Estimate_RSSI_From_CharData( gsmV4CalParams, rssiMaps_Chain.chainData[dRxChain], gsmV4RSSIResults.chainData[dRxChain], dRxChain, false );
             if( gsmV4CalParams.gsmV4RxParams.boolPerformHiLinRxCal )
             {
                Estimate_RSSI_From_CharData( gsmV4CalParams, rssiMapsHiLin_Chain.chainData[dRxChain], gsmV4RSSIResultsHiLin.chainData[dRxChain], dRxChain, true );
             }
         }

      }
   }
}
//! Do_MultiSweep
void Do_MultiSweep( GSM_V4_Cal_Params& gsmV4CalParams, GSM_V4_PCL_Results_Chain& gsmV4PCLResults,
   GSM_V4_DPD_Results_Chain& gsmV4DPDResults, GSM_V4_RSSI_Est_Chain& gsmV4RSSIResults, GSM_V4_RSSI_Est_Chain& gsmV4RSSIResultsHiLin, GSM_V4_Meas_Info& measInfo, GSMV4_SweepConfig& m_sweepConfig)
{
   // index for the saved original band configuration
   unsigned short curOrgBandIdx   = 0;

   // index for current sweep configuration
   unsigned short bandIdx = 0;

   // Assuming the sweeps are only broken up by bands/chains, there are an equal # of frames per frameSweepIdx.
   // Therefore, these functions only need to be called once in Do_MultiSweep.
   Compute_Frame_Count(gsmV4CalParams);
   Compute_MeasInfo(measInfo, gsmV4CalParams);

   // default chainIdx to assume segmentation of multiChain
   // initializing to zero implies chainIdx will be incremented
   unsigned short initChainIdx = 0;
   if ( !m_sweepConfig.segmentMultiChain )
   {
      // initialize so that Multiple Frame Sweep Loop is run through only once.
      initChainIdx = m_sweepConfig.orgNumTxChain - 1;
   }

   //m_sweepConfig contains the entire original setting and gsmV4CalParams contains settings for each sweep*/
   for (unsigned short chainIdx = initChainIdx; chainIdx<m_sweepConfig.orgNumTxChain; ++chainIdx)
   {
      // select the appropriate Tx/Rx chain - The # of Tx/Rx chains was already updated in Build_GSM_V4_SweepConfig
      if ( m_sweepConfig.segmentMultiChain )
      {
         gsmV4CalParams.uiRxChainList[0] = m_sweepConfig.orgRxChainIDs[chainIdx];
         gsmV4CalParams.uiTxChainList[0] = m_sweepConfig.orgTxChainIDs[chainIdx];
      }
      //--------------------------------- The Multiple Frame Sweep Loop ------------------------------------//
      for (unsigned short frameSweepIdx=0; frameSweepIdx<m_sweepConfig.numSweeps; ++frameSweepIdx)
      {
         //------------------------------- Update gsmV4_CalParams ------------------------------//

         // copy the band and channel list
         curOrgBandIdx = frameSweepIdx*m_sweepConfig.numBandsPerSweep;

         for (bandIdx=0; bandIdx<m_sweepConfig.numBandsPerSweep; ++bandIdx)
         {
            gsmV4CalParams.uiBandList[bandIdx] = m_sweepConfig.orgBandVec[curOrgBandIdx];
            gsmV4CalParams.uiChanList[bandIdx] = m_sweepConfig.orgChanVec[curOrgBandIdx];

            gsmV4CalParams.gsmV4RxParams.iGainOffsetMinLimList[bandIdx]      = m_sweepConfig.orgGainOffset_MinLim_List[curOrgBandIdx];
            gsmV4CalParams.gsmV4RxParams.iGainOffsetMaxLimList[bandIdx]      = m_sweepConfig.orgGainOffset_MaxLim_List[curOrgBandIdx];
            gsmV4CalParams.gsmV4DAParams.usPclIdxList[bandIdx]                  = m_sweepConfig.orgPclIdxList[curOrgBandIdx];
            gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList[bandIdx]                = m_sweepConfig.orgDpd_RgiList[curOrgBandIdx];
            gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList[bandIdx]            = m_sweepConfig.orgDpd_DigGainList[curOrgBandIdx];
            gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList[bandIdx] = m_sweepConfig.orgSlopeCorrectionDigGainList[curOrgBandIdx];

            ++curOrgBandIdx;
         }

         //------------------------------ DP GSM V4 CAl ----------------------------------//
         Do_GSM_V4_Cal( gsmV4CalParams, gsmV4PCLResults, gsmV4DPDResults, gsmV4RSSIResults, gsmV4RSSIResultsHiLin, measInfo, m_sweepConfig);
      }
   }
   // reset the gsmV4CalParams to the original for NV processing and XML log generation
   gsmV4CalParams.uiBandList = m_sweepConfig.orgBandVec;
   gsmV4CalParams.uiChanList = m_sweepConfig.orgChanVec;

   gsmV4CalParams.uiNumBands = m_sweepConfig.orgNumBand;
   //gsmV4CalParams.uiNumChan  = m_sweepConfig.orgNumChan;

   gsmV4CalParams.uiRxChainList = m_sweepConfig.orgRxChainIDs;
   gsmV4CalParams.uiTxChainList = m_sweepConfig.orgTxChainIDs;
   gsmV4CalParams.uiNumRxChains = m_sweepConfig.orgNumRxChain;
   gsmV4CalParams.uiNumTxChains = m_sweepConfig.orgNumTxChain;

   gsmV4CalParams.gsmV4RxParams.iGainOffsetMinLimList      = m_sweepConfig.orgGainOffset_MinLim_List;
   gsmV4CalParams.gsmV4RxParams.iGainOffsetMaxLimList      = m_sweepConfig.orgGainOffset_MaxLim_List;
   gsmV4CalParams.gsmV4DAParams.usPclIdxList                  = m_sweepConfig.orgPclIdxList;
   gsmV4CalParams.gsmV4DPDParams.uiDpdRgiList                = m_sweepConfig.orgDpd_RgiList;
   gsmV4CalParams.gsmV4DPDParams.uiDpdDigGainList            = m_sweepConfig.orgDpd_DigGainList;
   gsmV4CalParams.gsmV4DPDParams.uiSlopeCorrectionDigGainList = m_sweepConfig.orgSlopeCorrectionDigGainList;

   Compute_Frame_Count(gsmV4CalParams);
   Compute_MeasInfo(measInfo, gsmV4CalParams);
}

/*Test_GSM_V4_Cal() is the main (first entry point) GSM V4 function. This function call Initialize parameter functions,
calibration and process measurement functions*/
unsigned char Test_GSM_V4_Cal()
{
   GSM_V4_Cal_Params gsmV4CalParams;
   GSM_V4_Meas_Info measInfo;
   GSM_V4_PCL_Results_Chain gsmV4PCLResults;
   GSM_V4_DPD_Results_Chain gsmV4DPDResults;
   GSM_V4_RSSI_Est_Chain gsmV4RSSIResults;
   GSM_V4_RSSI_Est_Chain gsmV4RSSIResultsHiLin;

   GSMV4_SweepConfig m_sweepConfig;
   TECapabilities teCapabilities;

   /*Initialize GSM V4 params (DA, DPD, Rx setup similar to QSPR tree)*/
   Init_GSM_V4_Cal_Params(gsmV4CalParams);
   /*Validation of cal params*/
    ValidateGSMV4Params(gsmV4CalParams);



   /*Char DPD table initialization*/
   InitCharV4Table();

   /*Compute V4-V3 delta*/
    charV4_V3_Delta[0] = charV4_V3_Delta[1] = charV4_V3_Delta[2] = charV4_V3_Delta[3] = 0.0;
    if(gsmV4CalParams.gsmV4DPDParams.boolUseV3CharData)
    {
   ComputeV4_V3_Delta();
    }

   /*Initializes call box capabilities*/
   InitTECapabilities(teCapabilities,gsmV4CalParams);

   /*Compute the frame count based on the cal parameter settings*/
   Compute_Frame_Count(gsmV4CalParams);
   Compute_MeasInfo(measInfo, gsmV4CalParams);

   /*Fill out the Digital gain and RGI sequence*/
   if(gsmV4CalParams.gsmV4DPDParams.boolPerformDPD || gsmV4CalParams.gsmV4DPDParams.boolDoRGIRunOnly)
   {
        Build_DPD_Sweep_Containers(gsmV4CalParams);
   }

   bool skipDoGsmV4Cal = false;
   /*Configures the sweep parameters based on number of frames, FTM capability and call box capabilities*/
   bool boolMultisweepReq = Build_GSM_V4_SweepConfig(  gsmV4CalParams, skipDoGsmV4Cal, teCapabilities, m_sweepConfig );

   if (boolMultisweepReq)
   {
      Do_MultiSweep(gsmV4CalParams,  gsmV4PCLResults, gsmV4DPDResults,  gsmV4RSSIResults,gsmV4RSSIResultsHiLin,  measInfo,  m_sweepConfig);
   }
   else
   {
      //------------------------------ DO GSM V4 CAL ----------------------------------//
      // Don't run this if doing Rx-only cal in non-frame-sweep mode.
      // Do run this if doing Rx-only cal in frame-sweep mode.
      if(!skipDoGsmV4Cal)
      {
         Do_GSM_V4_Cal(gsmV4CalParams, gsmV4PCLResults,  gsmV4DPDResults, gsmV4RSSIResults,gsmV4RSSIResultsHiLin,  measInfo,  m_sweepConfig);
      }
   }
   // Do a V2/V3 Char RX sweep if call box doesn't support frame-sweep cal
   if (!teCapabilities.boolSupportSimultaneousTxRx)
   {
      Do_NonFrameSweep_Char_Rx_Cal(gsmV4CalParams, gsmV4RSSIResults, gsmV4RSSIResultsHiLin );
   }
   //---------------------------------------------- Process the Tx NV ------------------------------------------------//
   GSM_V4_Tx_Cx_NV_Results tx_NV_Results;
   // GSMV4 - make a DA/PCL cal only NV processor if necessary - resuse current code
   // - currently enforce DA and DPD being run together
   if ( gsmV4CalParams.gsmV4DAParams.boolPerformDA && gsmV4CalParams.gsmV4DPDParams.boolPerformDPD)
   {
      if (gsmV4CalParams.gsmV4DAParams.boolUsePCLErrorNV)
      {
         Process_GSM_Tx_Cx_NV_From_V4_Cal_PCL( gsmV4CalParams, gsmV4PCLResults, gsmV4DPDResults,  tx_NV_Results );
      }
      else
      {
         Process_GSM_Tx_Cx_NV_From_V4_Cal_DA( gsmV4CalParams, gsmV4PCLResults, gsmV4DPDResults,  tx_NV_Results );
      }
   }
   else if(gsmV4CalParams.gsmV4DAParams.boolPerformDA)
   {
       if (gsmV4CalParams.gsmV4DAParams.boolUsePCLErrorNV)
      {
         Process_GSM_Tx_Cx_NV_From_V4_Cal_DA_Only_PCL( gsmV4CalParams, gsmV4PCLResults, gsmV4DPDResults,  tx_NV_Results );
      }
      else
      {
         Process_GSM_Tx_Cx_NV_From_V4_Cal_DA_Only( gsmV4CalParams, gsmV4PCLResults, gsmV4DPDResults,  tx_NV_Results );
      }
   }

   //---------------------------------------------- Process the Rx NV -----------------------------------------------//
   GSM_V4_Rx_Cx_Cal_Meas   rx_Cal_Meas;
   GSM_V4_Rx_Cx_NV_Results rx_NV_Results;
   GSM_V4_Rx_Cx_Cal_Meas   rx_Cal_Meas_HiLin;
   GSM_V4_Rx_Cx_NV_Results rx_NV_Results_HiLin;
   if ( gsmV4CalParams.gsmV4RxParams.boolPerformRxCal || !teCapabilities.boolSupportSimultaneousTxRx )
   {
      Process_GSM_Rx_Cx_NV_From_V4_Cal( rx_NV_Results, gsmV4CalParams, gsmV4RSSIResults, rx_Cal_Meas, false );
      if( gsmV4CalParams.gsmV4RxParams.boolPerformHiLinRxCal )
      {
         Process_GSM_Rx_Cx_NV_From_V4_Cal( rx_NV_Results_HiLin, gsmV4CalParams, gsmV4RSSIResultsHiLin, rx_Cal_Meas_HiLin, true );
      }
   }

   //--------------------------------------------------Create QCN-----------------------------------------------------------
   unsigned short nv_items_list_size;
   unsigned char  nv_write_to_QCN_status;
   unsigned short numItemsCopied;

   ostringstream qcn_file_path;
   qcn_file_path << "C:\\temp\\QLIBDemo-GSMV4-TxRx.qcn";

   nv_items_list_size = 18;
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_Copy_RFNV_Items_From_NV_Tool(g_hResourceContext, &numItemsCopied);

   unsigned short* backupNVStatus   = (unsigned short*) malloc(nv_items_list_size*sizeof(unsigned short));
   unsigned short* backupNVItemIDs  = (unsigned short*) malloc(nv_items_list_size*sizeof(unsigned short));

   // Copy requested AUTO CAL NV Items to RFCAL NV Manger
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_BackupNVItems(g_hResourceContext, backupNVItemIDs, backupNVStatus, nv_items_list_size);

   free(backupNVStatus);
   free(backupNVItemIDs);

   // Dump to QCN
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_Create_QCN( g_hResourceContext, (char*)qcn_file_path.str().c_str() );

   return true;
}

/*---------------------------------------------------------GSM Tx Char predistortion (GSM V3)-----------------------------------------*/

struct GSM_EnhLinTx_Predist_Cal_Results
{
   vector<double> dutDigGainDbPoints;  // Measured Digital Gain (dB) component of each DigGain-Pwr point
   vector<double> dutPowerPoints;      // Measured Power component of each DigGain-Pwr point

   vector<double> charDigGainDbPoints; // Characterized Digital Gain (dB) component of each DigGain-Pwr point
   vector<double> charPowerPoints;     // Characterized Power component of each DigGain-Pwr point
};

//Enh Tx Lin Cal Measurement
//map<chan,map<paRange, map<rgi,Pwr>>>
struct GSM_EnhLinTx_cal_Results
{
   map<unsigned short, GSM_DA_Cal_Results> channelSweepResults;
   map<unsigned short, double> chanFirstSegmentLevel;
   map<unsigned short, GSM_EnhLinTx_Predist_Cal_Results> channelEnhLinPredistResults;
};

struct GSM_EnhLinTx_Cal_Params
{
public:
   nv_mode_id_type m_nv_mode;
   //! controls NV logging and writing to UUT, bitmask of RFCAL_NV_ACTION_TYPE bits
   unsigned int m_nv_action;

   vector<int> m_channels;

   //! Master Switch
   bool m_performEnhLinTxCal;
   long m_perfPreEnhLinTxLong;

   //! Submaster switches
   long m_doPowerAdaptationLong;
   bool m_doPowerAdaptation;
   long m_doSlopeCorrectionLong;
   bool m_doSlopeCorrection;        // this will be set to false during parameter verification if sweep list <= 1
   long m_doPiecewiseLinearAdaptLong;
   bool m_doPiecewiseLinearAdapt;

   bool m_doAdaptiveMaxPwrEst;

   //! Switch to GPRF Mode -
   bool m_useGprfPowerListMeas;
   long m_useGprfPowerListMeasLong;

   //! Data processing options
   long m_slopeCorrectionAlignLowLong;
   bool m_slopeCorrectionAlignLow;

   char * m_slopeCorrectionDigGainStr;
   vector<unsigned short> m_slopeCorrectionDigGainList;

   //! Test conditions.
   double m_inputLevel;

   //! Step Duration
   unsigned long m_stepDuration;

   //! Power of RGI calibration
   double m_calRGIPower;
   long m_calPaRange;

   //! TE setup
   double m_measLength;
   double m_measOffset;
   double m_measStartDelay;

   //!Sanity Check Limits for trigger
   double m_firstSegMinLevel;
   double m_firstSegMaxLevel;

   //! Enh Lin Tx Calibration Params
   vector<unsigned short> m_rgiList;
   vector<unsigned short> m_paList;
   vector<unsigned short> m_modeList;

   //! Saturation Detect and Limit RGI
   double m_saturationDetectionWindow;

   //! NV Mapping
   vector<unsigned long> m_nvMappings; //example 1 for F1, 2 for F2 etc.

   double m_dacScale;
};

const double GSM_TX_DA_SEGMENT_DURATION_US = 1008.0/273.0;//US = microseconds

//#define RFNV_PCL_NUM_ITEMS          16    // the number of elements in RFNV_GSM_Cx_GSMxxxx_POWER_LEVELS_I
#define RFNV_ENV_GAIN_ITEMS         16    // number of elemements in the RFNV_GSM_*_CHAR_PREDIST_ENV_GAIN_I array.
#define RFNV_ENV_GAIN_SIZE          64    // number of elemements in the RFNV_GSM_*_CHAR_PREDIST_ENV_GAIN_I array.
#define RFNV_ENV_GAIN_MODE_BEGIN    2     // mode index corresponding to RFNV_GSM_*_CHAR_PREDIST_ENV_GAIN_I[0]

double measEdgeDAPwr[1][3][3][32] = {{{{8.92264, 9.09576, 10.0533, 10.9227, 11.8953, 12.9839, 14.004, 15.0754, 16.08, 17.0182, 18.2463, 19.311, 20.4124, 21.8543, 23.1348, 24.2613, 25.267, 26.4082, 27.6064, 28.8392, 30.0019, 31.2171, 32.2231, 32.9412, 33.4649, 33.7789, 34.0115, 34.2003, 34.3561, 34.4721, 34.5584, 34.6179},\
         {8.17058, 8.19119, 9.11481, 9.96722, 10.9297, 12.0268, 13.0851, 14.1895, 15.2217, 16.199, 17.4832, 18.5996, 19.7585, 21.2654, 22.5952, 23.7663, 24.8055, 25.9795, 27.2091, 28.4664, 29.6303, 30.8153, 31.7599, 32.4293, 32.8914, 33.1557, 33.3437, 33.4887, 33.612, 33.697, 33.7536, 33.7866},\
         {6.90229, 6.88702, 7.81396, 8.64915, 9.64851, 10.7726, 11.8909, 13.0574, 14.1501, 15.185, 16.5514, 17.7533, 18.9792, 20.5716, 21.9597, 23.1905, 24.2726, 25.5151, 26.786, 28.0378, 29.2089, 30.439, 31.4289, 32.16, 32.6796, 32.9765, 33.1889, 33.3507, 33.4885, 33.5918, 33.6634, 33.7077}
      },\

      {  {8.21584, 8.42713, 9.40113, 10.2699, 11.2943, 12.4055, 13.3394, 14.4424, 15.4693, 16.4347, 17.568, 18.5868, 19.6489, 21.0783, 22.3405, 23.4589, 24.4586, 25.6322, 26.8463, 28.0892, 29.2997, 30.6346, 31.7719, 32.6417, 33.2248, 33.5521, 33.7665, 33.9477, 34.0884, 34.1785, 34.2368, 34.2715},\
         {7.43845, 7.52824, 8.48763, 9.34013, 10.3402, 11.4797, 12.4361, 13.5403, 14.6075, 15.6259, 16.8039, 17.8643, 18.979, 20.4686, 21.7835, 22.9422, 23.9826, 25.1916, 26.4246, 27.707, 28.9341, 30.2546, 31.3391, 32.1471, 32.6668, 32.9434, 33.1218, 33.2612, 33.3634, 33.4193, 33.4532, 33.4755},\
         {6.19539, 6.22034, 7.17802, 8.0335, 9.05712, 10.2228, 11.2171, 12.3925, 13.5057, 14.5781, 15.8443, 16.9725, 18.1679, 19.7306, 21.1103, 22.3271, 23.4054, 24.6628, 25.9698, 27.2696, 28.4948, 29.84, 30.9799, 31.8625, 32.4687, 32.7826, 32.9894, 33.1501, 33.2719, 33.3461, 33.3917, 33.4178}
      },\

      {  {7.72795, 7.94443, 8.94454, 9.84054, 10.8925, 12.0629, 12.8875, 14.0085, 15.0549, 16.0577, 17.115, 18.0878, 19.1112, 20.5159, 21.7552, 22.871, 23.8519, 25.0492, 26.2608, 27.5125, 28.743, 30.1565, 31.4019, 32.3936, 33.03, 33.355, 33.5496, 33.7121, 33.8175, 33.9016, 33.9412, 33.9659}, \
         {6.94184, 7.05075, 8.0183, 8.90373, 9.94868, 11.1281, 11.9676, 13.1174, 14.1809, 15.2318, 16.3293, 17.3483, 18.4158, 19.8864, 21.1763, 22.3291, 23.3546, 24.5915, 25.8366, 27.1032, 28.3688, 29.7764, 30.9902, 31.914, 32.4978, 32.7787, 32.9378, 33.0619, 33.142, 33.1811, 33.2056, 33.2239}, \
         {5.70808, 5.74818, 6.74359, 7.61037, 8.66698, 9.86787, 10.7599, 11.95, 13.0704, 14.1677, 15.3487, 16.4199, 17.5608, 19.1098, 20.4629, 21.6721, 22.743, 24.0243, 25.3254, 26.6443, 27.9261, 29.3356, 30.6059, 31.6119, 32.3105, 32.643, 32.8308, 32.9789, 33.08, 33.1289, 33.1591, 33.1775}
      }
   }
};

double measGSMDAPwr[1][3][3][32] = {{{{10.1124, 10.4243, 11.5958, 12.3272, 13.5074, 14.484, 15.3578, 16.3875, 17.2906, 18.5298, 19.4123, 20.5063, 21.7985, 22.9101, 23.7396, 25.0431, 26.1951, 27.2246, 28.1586, 29.3332, 30.3702, 31.4634, 32.0732, 32.8672, 33.3808, 33.7538, 34.106, 34.2933, 34.4652, 34.5961, 34.6959, 34.7588}, \
         {9.25088, 9.52282, 10.6626, 11.3809, 12.5854, 13.5771, 14.4824, 15.5394, 16.4903, 17.7801, 18.7128, 19.8592, 21.2077, 22.3745, 23.2269, 24.5774, 25.7696, 26.825, 27.8047, 28.9913, 30.0356, 31.0961, 31.6782, 32.4077, 32.8439, 33.1533, 33.4244, 33.5719, 33.6968, 33.777, 33.8345, 33.8735}, \
         {7.9836, 8.24507, 9.40301, 10.1544, 11.3776, 12.4237, 13.3809, 14.493, 15.5008, 16.8889, 17.8675, 19.0877, 20.5186, 21.7393, 22.6339, 24.0417, 25.2976, 26.4132, 27.4032, 28.609, 29.6744, 30.7939, 31.4026, 32.1892, 32.6621, 32.9977, 33.2952, 33.4567, 33.5989, 33.6984, 33.7708, 33.8162}
      }, \

      {  {9.38865, 9.71432, 10.8895, 11.6327, 12.8143, 13.8107, 14.6986, 15.7312, 16.6829, 17.7917, 18.6886, 19.7235, 20.9795, 22.0647, 22.8725, 24.1611, 25.316, 26.3479, 27.2869, 28.4736, 29.5706, 30.8014, 31.517, 32.4956, 33.0547, 33.4787, 33.8159, 34.003, 34.1523, 34.2495, 34.3198, 34.3677}, \
         {8.52824, 8.81522, 9.96802, 10.7014, 11.8985, 12.91, 13.8234, 14.8889, 15.8729, 17.0441, 17.9744, 19.0609, 20.3716, 21.4989, 22.3388, 23.6783, 24.8719, 25.9291, 26.8956, 28.13, 29.2388, 30.4608, 31.1527, 32.0637, 32.5499, 32.9018, 33.1698, 33.309, 33.4063, 33.4626, 33.5046, 33.5366}, \
         {7.2575, 7.52025, 8.69963, 9.45234, 10.6819, 11.7275, 12.6864, 13.815, 14.8522, 16.101, 17.0926, 18.2454, 19.6374, 20.8233, 21.7007, 23.0988, 24.3363, 25.4592, 26.4788, 27.7281, 28.8526, 30.1221, 30.851, 31.8326, 32.3718, 32.762, 33.0601, 33.2167, 33.3337, 33.4055, 33.4558, 33.489}
      }, \

      {  {8.84973, 9.18329, 10.3936, 11.1272, 12.3357, 13.3414, 14.2473, 15.3086, 16.2726, 17.2907, 18.1852, 19.1648, 20.3812, 21.4332, 22.2164, 23.5, 24.6391, 25.6648, 26.5971, 27.7893, 28.8905, 30.2309, 31.0344, 32.1856, 32.7795, 33.2575, 33.5716, 33.7474, 33.8722, 33.9462, 34.0014, 34.0389}, \
         {7.99726, 8.29371, 9.46618, 10.2083, 11.4271, 12.4263, 13.363, 14.4482, 15.4527, 16.5183, 17.4559, 18.4867, 19.7529, 20.8448, 21.6628, 22.99, 24.1703, 25.2231, 26.1824, 27.4049, 28.5542, 29.8896, 30.6885, 31.7757, 32.3047, 32.713, 32.964, 33.0893, 33.1665, 33.2119, 33.247, 33.2719}, \
         {6.73314, 7.01655, 8.2071, 8.96202, 10.2043, 11.245, 12.2123, 13.3544, 14.4117, 15.5478, 16.5367, 17.6236, 18.9687, 20.1265, 20.98, 22.3725, 23.5926, 24.6911, 25.7035, 26.9862, 28.1413, 29.5059, 30.3447, 31.5222, 32.1186, 32.5881, 32.8735, 33.0206, 33.1132, 33.167, 33.2074, 33.233}
      }
   }
};

//3 channel, 4rgi 4digigain
double digiGainPwr[3][16] = {{7.66622, 16.6069, 25.8851, 29.8364, 8.68358, 17.6448, 27.1149, 30.9852, 9.66068, 18.7182, 28.2926, 31.873, 10.7473, 19.9486, 29.6145, 32.5883}, \
   {7.04701, 15.9962, 25.1345, 29.0821, 8.08256, 17.06, 26.382, 30.3375, 9.14623, 18.149, 27.6048, 31.3492, 10.2933, 19.4293, 28.9852, 32.2329}, \
   {6.60679, 15.5698, 24.578, 28.4893, 7.67278, 16.6461, 25.824, 29.7728, 8.7322, 17.757, 27.0753, 30.898, 9.99092, 19.0704, 28.4907, 31.9087}
};

/*************************************************************************
Function: Init_Enh_Cal_Sweep_Params
Input: GSM_EnhLinTx_Cal_Params (cal params)
Output: None
Description: This function initializes the cal params (cal tree settings)
********************************************************************************/
void Init_Enh_Cal_Sweep_Params(GSM_EnhLinTx_Cal_Params &enhTxLinCalParams)
{
   enhTxLinCalParams.m_channels.push_back(975);
   enhTxLinCalParams.m_channels.push_back(37);
   enhTxLinCalParams.m_channels.push_back(124);

   enhTxLinCalParams.m_calPaRange = 0;

   enhTxLinCalParams.m_calRGIPower = 27.0;

   enhTxLinCalParams.m_dacScale = 4;

   enhTxLinCalParams.m_doAdaptiveMaxPwrEst = true;

   //if piecewise is true, make doslope correction to false and vice versa.
   enhTxLinCalParams.m_doPiecewiseLinearAdapt = true; //false;

    //Piecewise and power adaptation cannot be true at the same time.
    enhTxLinCalParams.m_doPowerAdaptation = false; //true;

   enhTxLinCalParams.m_doSlopeCorrection = false;// true;
   for(int pastate = 0; pastate < 3; pastate++) //assuming 3 PA states
   {
      for(int i = 0; i < 32; i++)
      {
         enhTxLinCalParams.m_modeList.push_back(0);
         enhTxLinCalParams.m_modeList.push_back(1);
      }
   }
   for(int rgiList = 0; rgiList < 4; rgiList++) //4 rig for digital gain sweep
   {
      for(int i = 2; i < 6; i++) //4 digital gain
      {
         enhTxLinCalParams.m_modeList.push_back(i);
      }
   }
   enhTxLinCalParams.m_nvMappings.push_back(1);
   enhTxLinCalParams.m_nvMappings.push_back(2);
   enhTxLinCalParams.m_nvMappings.push_back(3);

   for(int pastate = 0; pastate < 3; pastate++)
   {
      for(int i = 0; i < 64; i++) //32 rgi * 2 modes (gsm, edge)
      {
         enhTxLinCalParams.m_paList.push_back(pastate);
      }
   }
   for(int cnt =0; cnt < 16 /*4 * 4*/; cnt++) //4 rgi, 4 digi gain
   {
      enhTxLinCalParams.m_paList.push_back(0);
   }
   enhTxLinCalParams.m_performEnhLinTxCal = true; // indicates GSM V3 calibration

   for(int pastate = 0; pastate < 3; pastate++)
   {
      for(int rgiCnt = 0; rgiCnt < 32; rgiCnt++)
      {
         for(int modCnt = 0; modCnt < 2; modCnt++)
         {
            enhTxLinCalParams.m_rgiList.push_back(rgiCnt);
         }
      }
   }
   int startRgi = 18;
   for(int rgiList = 0; rgiList < 4 ; rgiList++)
   {
      for(int digigain = 2; digigain < 6; digigain++)
      {
         enhTxLinCalParams.m_rgiList.push_back(startRgi + rgiList);
      }
   }

   enhTxLinCalParams.m_saturationDetectionWindow = 0;

   enhTxLinCalParams.m_slopeCorrectionAlignLow = false;

   enhTxLinCalParams.m_slopeCorrectionDigGainList.push_back(5);
   enhTxLinCalParams.m_slopeCorrectionDigGainList.push_back(15);
   enhTxLinCalParams.m_slopeCorrectionDigGainList.push_back(40);

   enhTxLinCalParams.m_stepDuration = 250; //some call boxes support 50 also.

   enhTxLinCalParams.m_measLength = 100;

   enhTxLinCalParams.m_measOffset = 30;

   enhTxLinCalParams.m_measStartDelay = 50;

   // Determine number of enhanced sweep points
   int enhModeCount = 0;
   for (unsigned int modeIter=0; modeIter< enhTxLinCalParams.m_modeList.size(); modeIter++)
   {
      if (enhTxLinCalParams.m_modeList[modeIter] >= RFNV_ENV_GAIN_MODE_BEGIN &&
            enhTxLinCalParams.m_modeList[modeIter] < RFNV_ENV_GAIN_MODE_BEGIN + RFNV_ENV_GAIN_ITEMS)
      {
         enhModeCount++;
      }
   }

   if (enhTxLinCalParams.m_performEnhLinTxCal)
   {
      // POWER ADAPTATION
      if (enhModeCount <= 0)
      {
         enhTxLinCalParams.m_doPowerAdaptation = false;
         //"EnhLinTx Cal will not perform power adaptation: there are 0 enhanced points in the PA/RGI/Mode sweep list."
      }

      // SLOPE CORRECTION
      if (enhModeCount < 2)
      {
         enhTxLinCalParams.m_doSlopeCorrection = false;
         //"EnhLinTx Cal will not perform slope correction: there are less than 2 enhanced points in the PA/RGI/Mode sweep list."
      }

      // PIECEWISE LINEAR ADAPTATION
      if (enhModeCount < 2)
      {
         enhTxLinCalParams.m_doPiecewiseLinearAdapt = false;
         //"EnhLinTx Cal will not perform piecewise-linear adaptation: there are less than 2 enhanced points in the PA/RGI/Mode sweep list."
      }

      // MAX POWER ESTIMATION
      if (enhModeCount < 4)
      {
         enhTxLinCalParams.m_doAdaptiveMaxPwrEst = false;
         // "EnhLinTx Cal will not perform max power estimation: there are less than 4 enhanced points in the PA/RGI/Mode sweep list."
      }

      // make sure both slope and piecewise adaptation aren't enabled at the same time
      if (enhTxLinCalParams.m_doSlopeCorrection && enhTxLinCalParams.m_doPiecewiseLinearAdapt)
      {
         enhTxLinCalParams.m_doSlopeCorrection = false;
         //"Piecewise-linear and Slope adaptation can't both be enabled: defaulting to Piecewise-Linear."
      }

      // make sure both piecewise and power adaptation aren't enabled at the same time
      if (enhTxLinCalParams.m_doPowerAdaptation && enhTxLinCalParams.m_doPiecewiseLinearAdapt)
      {
         enhTxLinCalParams.m_doPowerAdaptation = false;
         // "Piecewise-linear and Power adaptation can't both be enabled: disabling Power Adapt."
      }
   }
}

void Validate_GSM_V3_Tx_Params(GSM_EnhLinTx_Cal_Params &enhTxLinCalParams)
{
   //Sanity Checks on Parameters

   if(enhTxLinCalParams.m_stepDuration <= 0)
   {
      //DA sweep is less than 0. throw exception
   }

   if(enhTxLinCalParams.m_measOffset < 0.)
   {
      //measure offset is less than 0. throw exception
   }

   if(enhTxLinCalParams.m_measLength <= 0.)
   {
      //meas length less than 0. throw exception
   }
   if(enhTxLinCalParams.m_measLength > (GSM_TX_DA_SEGMENT_DURATION_US *  enhTxLinCalParams.m_stepDuration))
   {
      //Meas length cannot be greater than step duration. throw exception
   }
   if((enhTxLinCalParams.m_measLength + enhTxLinCalParams.m_measOffset) > (GSM_TX_DA_SEGMENT_DURATION_US *  enhTxLinCalParams.m_stepDuration))
   {
      //Meas length + offset cannot be greater than step duration. throw exception.
   }

   if(enhTxLinCalParams.m_measStartDelay < 0.)
   {
      //meas start delay cannot be less than 0. throw exception
   }

   if(enhTxLinCalParams.m_channels.size() > 3)
   {
      //channel size cannot be greater than 3. throw exception
   }
   if(enhTxLinCalParams.m_channels.size() < 2)
   {
      //channel size cannot be less than 2. throw exception.
   }
   if(enhTxLinCalParams.m_channels.size() != enhTxLinCalParams.m_nvMappings.size())
   {
      //channel size and NV mapping size cannot be different. throw exception
   }
   if(*max_element(enhTxLinCalParams.m_nvMappings.begin(),enhTxLinCalParams.m_nvMappings.end()) > 3)
   {
      // Max element of NV mapping cannot be greater than 3. throw exception
   }
   if(*min_element(enhTxLinCalParams.m_nvMappings.begin(),enhTxLinCalParams.m_nvMappings.end()) < 1)
   {
      //min value of NV mapping cannot be less than 1. throw exception.
   }

   //Number of Segments
   if( (enhTxLinCalParams.m_rgiList.size() != enhTxLinCalParams.m_paList.size()) || (enhTxLinCalParams.m_rgiList.size() != enhTxLinCalParams.m_modeList.size()) || (enhTxLinCalParams.m_rgiList.size() != enhTxLinCalParams.m_paList.size()) )
   {
      //size of RGI list, PA list and Mode list should all be the same. Else throw exception
   }
}

/*************************************************************************
Function: Get_Enh_Cal_Sweep_Results
Input: GSM_EnhLinTx_Cal_Params (cal params), vector<double> (sweep data),unsigned short (channel)
Output: None
Description: This function fills in the sweep vector (mimics reading from call box)
********************************************************************************/
void Get_Enh_Cal_Sweep_Results(GSM_EnhLinTx_Cal_Params &enhTxLinCalParams, vector<double> &enhLinTxSweepPwrs, unsigned short chanCnt)
{
   enhLinTxSweepPwrs.clear();
   //for(int iChanCnt = 0; iChanCnt < enhTxLinCalParams.m_channels.size(); iChanCnt++)
   //{
   for(int iPACnt = 0; iPACnt < 3; iPACnt++)
   {
      for(int iRGICnt = 0; iRGICnt < 32; iRGICnt++)
      {
         enhLinTxSweepPwrs.push_back(measEdgeDAPwr[0][chanCnt][iPACnt][iRGICnt]);
         enhLinTxSweepPwrs.push_back(measGSMDAPwr[0][chanCnt][iPACnt][iRGICnt]);
      }
   }
   for(int iRGIDigiCnt = 0; iRGIDigiCnt < 16; iRGIDigiCnt++)
   {
      enhLinTxSweepPwrs.push_back(digiGainPwr[chanCnt][iRGIDigiCnt]);
   }
   //}
}

/*************************************************************************
Function: Process_EDGE_EnhTxLin_Cal_DA_NV_Intermediate
Input: GSM_EnhLinTx_Cal_Params (cal params), GSM_EnhLinTx_cal_Results (cal data), QMSL_DA_Measurement (for GSM),
QMSL_DA_Measurement (for EDGE), unsigned int (gsm idx), unsigned int (edge idx)
Output: None
Description: This function stores the DA cal data appropriately for NV generation
********************************************************************************/
void Process_EDGE_EnhTxLin_Cal_DA_NV_Intermediate(GSM_EnhLinTx_Cal_Params& enhTxLinCalParams, GSM_EnhLinTx_cal_Results& calibratedEnhLinCalData,
                                                  QMSL_DA_Measurement* gsmMeas, QMSL_DA_Measurement* edgeMeas, unsigned int& gsmMeasIndex, unsigned int& edgeMeasIndex, unsigned char& queue_nv_indicator )
{
   map<unsigned short, GSM_DA_Cal_Sweep_Record>::iterator paIter;

   map<unsigned short, double>::iterator rgiIter;

   unsigned int numChan   = enhTxLinCalParams.m_channels.size();
   unsigned short curChan = 0;;

   QMSL_GSM_Polar_Cal_Frequency curNvMapping;

   // Scan through calibration results and put them in a QMSL friendly form
   for(unsigned int chanIdx=0; chanIdx<numChan; ++chanIdx)
   {
      curChan      = static_cast<unsigned short>( enhTxLinCalParams.m_channels[chanIdx] );
      curNvMapping = static_cast<QMSL_GSM_Polar_Cal_Frequency>( enhTxLinCalParams.m_nvMappings[chanIdx] );

      for(paIter=calibratedEnhLinCalData.channelSweepResults[curChan].gsmDaSweep.begin(); paIter!=calibratedEnhLinCalData.channelSweepResults[curChan].gsmDaSweep.end(); ++paIter)
      {
         for (rgiIter = paIter->second.rgiPwr.begin(); rgiIter != paIter->second.rgiPwr.end(); ++rgiIter)
         {
            gsmMeas[gsmMeasIndex].chan        = curChan;
            gsmMeas[gsmMeasIndex].freqMapping = curNvMapping;
            gsmMeas[gsmMeasIndex].pa          = paIter->first;
            gsmMeas[gsmMeasIndex].rgi         = rgiIter->first;
            gsmMeas[gsmMeasIndex].power       = rgiIter->second;

            ++gsmMeasIndex;
         }
      }
      for(paIter=calibratedEnhLinCalData.channelSweepResults[curChan].edgeDaSweep.begin(); paIter!=calibratedEnhLinCalData.channelSweepResults[curChan].edgeDaSweep.end(); ++paIter)
      {
         for (rgiIter = paIter->second.rgiPwr.begin(); rgiIter != paIter->second.rgiPwr.end(); ++rgiIter)
         {
            edgeMeas[edgeMeasIndex].chan        = curChan;
            edgeMeas[edgeMeasIndex].freqMapping = curNvMapping;
            edgeMeas[edgeMeasIndex].pa          = paIter->first;
            edgeMeas[edgeMeasIndex].rgi         = rgiIter->first;
            edgeMeas[edgeMeasIndex].power       = rgiIter->second;

            ++edgeMeasIndex;
         }
      }
   }

   queue_nv_indicator = 1;//(enhTxLinCalParams.m_nv_action & RFCAL_NV_ACTION_QUEUE_NV) ? 1 : 0;
}

/*************************************************************************
Function: Determine_EnhTxLin_Cal_RGI
Input: GSM_EnhLinTx_Cal_Params (cal params), GSM_EnhLinTx_cal_Results (cal data)
Output: unsigned short (cal rgi)
Description: This function determines the RGI based on the DA sweep data
********************************************************************************/
unsigned short Determine_EnhTxLin_Cal_RGI( GSM_EnhLinTx_Cal_Params& enhTxLinParams, GSM_EnhLinTx_cal_Results& enhTxLinData )
{
   unsigned short rgiMatch   = 0;       // RGI meeting preDistCalPower requirements for current channel
   unsigned short rgiAllChan = 0;       // RGI meeting preDistCalPower requirements for ALL channels
   double power              = -1000.0; // initialize to unlikely value
   bool chanRgiReached       = false;

   // minimum power required for predistortion calibration
   // Chose the max of the RGIs that just meet this requirement across all channels
   double preDistCalPower = enhTxLinParams.m_calRGIPower;

   GSM_DA_Cal_Sweep_Record rgiSweepRecord;

   map<unsigned short, GSM_DA_Cal_Results>::iterator chanIter;    // iterator over channelSweepResults
   map<unsigned short,double>::iterator rgiIter;                  // iterator over DA cal results (RGI vs. Power)

   // buffer to prevent very near preDistCalPower from causing this to fail.
   // For example, if preDistCalPower = 27 dBm, 26.98 dBm shouldn't cause it to fail.
   const double preDistPowerBuffer = 0.5;

   const double powerThresh = preDistCalPower - preDistPowerBuffer;

   /*Find the maximum of RGI for all channels that achieves the target power*/
   for (chanIter = enhTxLinData.channelSweepResults.begin(); chanIter != enhTxLinData.channelSweepResults.end(); ++chanIter)
   {
      rgiSweepRecord = chanIter->second.edgeDaSweep[ static_cast<unsigned short>(enhTxLinParams.m_calPaRange) ];

      chanRgiReached = false; // update "discovery of predist RGI" boolean

      for (rgiIter = rgiSweepRecord.rgiPwr.begin(); rgiIter != rgiSweepRecord.rgiPwr.end(); ++rgiIter)
      {
         power = rgiIter->second;

         if (power >= powerThresh)
         {
            rgiMatch = rgiIter->first;
            chanRgiReached = true;
            break;
         }
      }
      // throw an exception if the target power was not found
      if( !chanRgiReached )
      {
         //throw exception
      }

      // if the rgiMatch exceeds rgiAllChan, update it.
      if(rgiMatch > rgiAllChan)
      {
         rgiAllChan = rgiMatch;
      }
   }
   return rgiAllChan;
}

/*************************************************************************
Function: GSMBandTypeToRFMode
Input: BAND_CLASS_ENUM
Output: FTM_RF_Mode_Enum
Description: This function returns the RF Mode enum for the Band class enum
********************************************************************************/
FTM_RF_Mode_Enum GSMBandTypeToRFMode(const BAND_CLASS_ENUM bandType)
{
   FTM_RF_Mode_Enum retVal;
   switch(bandType)
   {
      case BH_GSM_BAND_E_GSM:
         retVal = PHONE_MODE_GSM_900;
         break;
      case BH_GSM_BAND_DCS:
         retVal = PHONE_MODE_GSM_1800;
         break;
      case BH_GSM_BAND_PCS:
         retVal = PHONE_MODE_GSM_1900;
         break;
      case BH_GSM_BAND_850:
         retVal = PHONE_MODE_GSM_850;
         break;
      default:
         retVal = PHONE_MODE_MAX;
   }
   return retVal;
}
/*************************************************************************
Function: Process_EDGE_EnhTxLin_Cal_Predist_NV_Intermediate
Input: GSM_EnhLinTx_Cal_Params (cal params), GSM_EnhLinTx_cal_Results (cal data), QMSL_GSM_EnhLinTx_Cal_Control (Cal control),
QMSL_GSM_Tx_Calibration_NV (cal NV),unsigned char (queue nv indicator)
Output: None
Description: This function process the calibration data for predistortion
********************************************************************************/
void Process_EDGE_EnhTxLin_Cal_Predist_NV_Intermediate(GSM_EnhLinTx_Cal_Params& enhLinTxParams, GSM_EnhLinTx_cal_Results& enhLinTxData,
                                                       QMSL_GSM_EnhLinTx_Cal_Control& calControl, QMSL_GSM_EnhLinTx_Cal_Data& calData, unsigned char& queue_nv_indicator )
{
   // convert m_band from BAND_CLASS_ENUM to FTM_RF_MODE_ENUM
   FTM_RF_Mode_Enum curBand = GSMBandTypeToRFMode(BH_GSM_BAND_E_GSM /*m_band*/ );

   // Find minimum RGI to achieve the required power level
   unsigned short calRgi = Determine_EnhTxLin_Cal_RGI(enhLinTxParams,enhLinTxData);

   // Pack data structures for QMSL control information
   calControl.bSlopeCorrAlignLow     = enhLinTxParams.m_slopeCorrectionAlignLow;
   calControl.bDoPowerAdaptation     = enhLinTxParams.m_doPowerAdaptation;
   calControl.bDoSlopeCorrection     = enhLinTxParams.m_doSlopeCorrection;
   calControl.bDoPiecewiseAdaptation = enhLinTxParams.m_doPiecewiseLinearAdapt;
   calControl.iAlgType               = 0;

   // Pack data structures for QMSL characterized & DUT data
   calData.iCalRgi      = calRgi;
   calData.dDacScale    = static_cast<float>(enhLinTxParams.m_dacScale);
   calData.iNumChannels = enhLinTxParams.m_channels.size();

   // size of AMAM/AMPM NV used throughout
   unsigned int numDAC = 2*QMSL_NV_MANAGER_POLAR_CAL_AMAM_TABLE_SIZE;

   // Get the Edge digital gain
   //unsigned short retType = 2;   // nvArr[0] points to EDGE envGain
   //bResult = ( QLIB_FTM_GSM_API_ReadEnvGainNV( m_phoneHandle, nvID, nvArr, &nvSize, &readStatus, retType ) != 0 );
   unsigned short edgeDigGain = 47;

   // channel variable used throughout
   unsigned short chan = static_cast<unsigned short>(enhLinTxParams.m_channels[0]);

   // the RGI of the char DPD selection
   unsigned short curRgi = calRgi;

   // the char DAC corresponding to the maximum digital gain output power in the char data set
   float dacAtMaxDigGain = 0;

   // if true, pick char data set based upon power measurements.
   // if false, pick char data set based solely on calRgi
   const bool useAdaptiveCharRgiSelection   = true;
   // if true, adaptively determine the max power delta for power adaptation
   // if false, use the power at the fixed highest digital gain from the tree params for determining the max power delta
   const bool useAdaptiveMaxPowerEstimation = enhLinTxParams.m_doAdaptiveMaxPwrEst;

   // Pack characterized data
   for (unsigned int chanIter=0; chanIter<enhLinTxParams.m_channels.size(); ++chanIter)
   {
      chan = static_cast<unsigned short>(enhLinTxParams.m_channels[chanIter]);

      calData.iFreqMapping[chanIter] = static_cast<QMSL_GSM_Polar_Cal_Frequency>( enhLinTxParams.m_nvMappings[chanIter] );

      vector<struct opPwrRecord> ops;
      vector<struct digRecord> daqs;

      /*Select the char AMAM/AMPM table and digi gain vs pwr table based on calRgi and useAdaptiveCharRgiSelection*/
      curRgi = Select_CharDPD_DataSet( calRgi, static_cast<unsigned short>(curBand), chan, 0 /*m_chainID*/, useAdaptiveCharRgiSelection, ops, daqs,
                                       enhLinTxData.channelSweepResults[chan].digGainSweep, false );
      // Estimate the DUT power at the maximum char Digital Gain
      // Function returns the esimated power, but also adds it to the DUT digGainSweep
      if (useAdaptiveMaxPowerEstimation)
      {
         dacAtMaxDigGain = DetermineDacAtDigGainPwr( ops, daqs[daqs.size()-1].pwr, numDAC );
         EstimatePowerAtMaxDigGain( ops[numDAC-1].amam, ops[numDAC-1].oppwr, daqs, enhLinTxData.channelSweepResults[chan].digGainSweep, calRgi,
                  dacAtMaxDigGain, edgeDigGain, calData.dDacScale );
      }

      // Sanity check number of records
      if (ops.size() != numDAC)
      {
         //throw exception
      }

      //--------------------------- Pack characterized predistortion curve ---------------------------//
      for(unsigned int opsPos=0; opsPos<numDAC; ++opsPos)
      {
         calData.aCharCurves[chanIter].dCharDac[opsPos]   = ops[opsPos].amam;
         calData.aCharCurves[chanIter].dCharPower[opsPos] = ops[opsPos].oppwr;
         calData.aCharCurves[chanIter].dCharAmpm[opsPos]  = ops[opsPos].ampm;
      }

      // Pack characterized digital gain table
      bool useAllPoints = enhLinTxParams.m_slopeCorrectionDigGainList.empty();

      if (daqs.size() > QMSL_NV_MANAGER_ENH_CAL_DIG_GAIN_TABLE_SIZE)
      {
         //throw exception
      }

      calData.aCharDigGain[chanIter].iNumEntries = daqs.size();

      for (unsigned int daqPos=0; daqPos<daqs.size(); ++daqPos)
      {
         unsigned short digGain = (unsigned short)daqs[daqPos].digGain;
         bool usePoint = find(enhLinTxParams.m_slopeCorrectionDigGainList.begin(),
                              enhLinTxParams.m_slopeCorrectionDigGainList.end(),
                              digGain) != enhLinTxParams.m_slopeCorrectionDigGainList.end();
         calData.aCharDigGain[chanIter].iDigGain[daqPos] = digGain;
         calData.aCharDigGain[chanIter].dPower[daqPos] = daqs[daqPos].pwr;
         calControl.bSlopeCorrUseCharPoint[daqPos] = usePoint || useAllPoints;
      }

      //--------------------------- Pack measured digital gains from DUT -----------------------//
      calData.aDutDigGain[chanIter].iNumEntries = 0;
      unsigned int digGainTablePos              = 0;

      GSM_DA_Cal_Results* channelSweepResults = &enhLinTxData.channelSweepResults[chan];

      map<unsigned short,GSM_DA_Cal_DigGain_Record>::iterator digGainIter = channelSweepResults->digGainSweep.begin();

      for (; digGainIter!=channelSweepResults->digGainSweep.end(); digGainIter++)
      {
         if (digGainIter->second.digGainPwr.find(calRgi) != digGainIter->second.digGainPwr.end())
         {
            unsigned short digGain = digGainIter->first;
            bool usePoint = find(enhLinTxParams.m_slopeCorrectionDigGainList.begin(),
                                 enhLinTxParams.m_slopeCorrectionDigGainList.end(),
                                 digGain) != enhLinTxParams.m_slopeCorrectionDigGainList.end();

            if (digGainTablePos > QMSL_NV_MANAGER_ENH_CAL_DIG_GAIN_TABLE_SIZE)
            {
               //throw exception
            }

            calData.aDutDigGain[chanIter].iDigGain[digGainTablePos] = digGain;
            calData.aDutDigGain[chanIter].dPower[digGainTablePos]   = static_cast<float>( digGainIter->second.digGainPwr[calRgi] );
            calControl.bSlopeCorrUseDutPoint[digGainTablePos]       = usePoint || useAllPoints;
            calData.aDutDigGain[chanIter].iNumEntries += 1;
            digGainTablePos += 1;
         }
      }
      if (calData.aDutDigGain[chanIter].iNumEntries < 1 && enhLinTxParams.m_doPowerAdaptation)
      {
         //throw exception
      }
      if (calData.aDutDigGain[chanIter].iNumEntries < 2 && (enhLinTxParams.m_doSlopeCorrection || enhLinTxParams.m_doPiecewiseLinearAdapt) )
      {
         //throw exception
      }
      if (calData.aCharDigGain[chanIter].iNumEntries < 1 && enhLinTxParams.m_doPowerAdaptation)
      {
         //throw exception
      }
      if (calData.aCharDigGain[chanIter].iNumEntries < 2 && (enhLinTxParams.m_doSlopeCorrection || enhLinTxParams.m_doPiecewiseLinearAdapt) )
      {
         //throw exception
      }
   }

   queue_nv_indicator = 1;// (enhLinTxParams.m_nv_action & RFCAL_NV_ACTION_QUEUE_NV) ? 1 : 0;
}

/*************************************************************************
Function: Process_GSM_Tx_Cx_NV_From_DA_and_Enh_Predistortion_Cal
Input: GSM_EnhLinTx_Cal_Params (cal params), GSM_EnhLinTx_cal_Results (cal data), QMSL_GSM_Tx_Calibration_NV (cal NV)
Output: None
Description: This function process the calibration data and generate NV
********************************************************************************/
void Process_GSM_Tx_Cx_NV_From_DA_and_Enh_Predistortion_Cal( GSM_EnhLinTx_Cal_Params& enhTXCalParams,
                                                             GSM_EnhLinTx_cal_Results& enhTXCalData, QMSL_GSM_Tx_Calibration_NV& calNV )
{
   //------------------ DA Cal Processing ----------------//
   QMSL_DA_Measurement gsmMeas[QMSL_GSM_MAX_TX_CAL_CHANNELS*QMSL_GSM_MAX_TX_GAIN_STATES*QMSL_GSM_MAX_RGI_STEPS];
   QMSL_DA_Measurement edgeMeas[QMSL_GSM_MAX_TX_CAL_CHANNELS*QMSL_GSM_MAX_TX_GAIN_STATES*QMSL_GSM_MAX_RGI_STEPS];

   // positions in array gsmMeas, edgeMeas being filled
   unsigned int gsmMeasIndex = 0, edgeMeasIndex = 0;

   unsigned char queue_nv_indicator_da = 0x0;

   /*Process DA data*/
   Process_EDGE_EnhTxLin_Cal_DA_NV_Intermediate( enhTXCalParams, enhTXCalData, gsmMeas, edgeMeas, gsmMeasIndex, edgeMeasIndex, queue_nv_indicator_da );

   //------------------ Enhanced PreDist Processing ----------------//
   QMSL_GSM_EnhLinTx_Cal_Control calControl;
   QMSL_GSM_EnhLinTx_Cal_Data calData;
   unsigned char queue_nv_indicator_predist = 0x0;

   Process_EDGE_EnhTxLin_Cal_Predist_NV_Intermediate( enhTXCalParams,  enhTXCalData, calControl, calData, queue_nv_indicator_predist );

   QLIB_RFCal_GSM_Tx_Enh_Cal_Generate_NV( g_hResourceContext,
                                          static_cast<unsigned short>(NV_MODE_GSM_EGSM),
                                          0, //chain ID
                                          gsmMeasIndex,
                                          (void*)& gsmMeas,
                                          edgeMeasIndex,
                                          (void*)& edgeMeas,
                                          (void*)& calControl,
                                          (void*)& calData,
                                          (void*)& calNV,
                                          queue_nv_indicator_predist );
}

void UUT_FTM_SET_MODE_ID_GSM(unsigned short chainID, FTM_Mode_Id_Enum gsmModeID)
{
   unsigned char status = 0;
   string modeStr("");

   // Only chainID 0 and 2 are allowed.
   if( (chainID != 0) && (chainID != 2) )
   {
      //throw exception
   }

   if (gsmModeID == FTM_MODE_ID_GSM)
   {
      // value for chainID == 0
      modeStr = "FTM_MODE_ID_GSM";
      if (chainID == 2)
      {
         modeStr = "FTM_MODE_ID_GSM_C2";
         gsmModeID = FTM_MODE_ID_GSM_C2;
      }
   }

   if (gsmModeID == FTM_MODE_ID_GSM_EXTENDED_C)
   {
      // value for chainID == 0
      modeStr = "FTM_MODE_ID_GSM_EXTENDED_C";
      if (chainID == 2)
      {
         modeStr = "FTM_MODE_ID_GSM_EXTENDED_C2";
         gsmModeID = FTM_MODE_ID_GSM_EXTENDED_C2;
      }
   }

   status = QLIB_FTM_SET_MODE_ID( g_hResourceContext, static_cast<short>(gsmModeID) );

   if(status == 0)
   {
      //throw exception
   }
}

void UUT_Do_GSM_EnhLinTx_Sweep( vector<unsigned short> paRangeListIn, vector<unsigned short> rgiListIn,
                                vector<unsigned short> modeList, unsigned short segLength, unsigned short chainID )
{
   unsigned short numSegments = static_cast<unsigned short>(paRangeListIn.size());

   const int MAX_SEGMENTS_DA_CAL = 512;

   unsigned char status = 1;

   FTM_Mode_Id_Enum gsmModeID = FTM_MODE_ID_GSM_EXTENDED_C;
   UUT_FTM_SET_MODE_ID_GSM(chainID, gsmModeID);

   unsigned short segLenList[MAX_SEGMENTS_DA_CAL];
   unsigned char  rgiList[MAX_SEGMENTS_DA_CAL];
   unsigned char  gsmModeList[MAX_SEGMENTS_DA_CAL];
   unsigned char  paRangeList[MAX_SEGMENTS_DA_CAL];

   for(unsigned int segmentNum = 0; segmentNum < numSegments; segmentNum++)
   {
      segLenList[segmentNum] = segLength;
      rgiList[segmentNum]    =(unsigned char)rgiListIn[segmentNum];
      gsmModeList[segmentNum]=(unsigned char)modeList[segmentNum];
      paRangeList[segmentNum]=(unsigned char)paRangeListIn[segmentNum];
   }

   status = QLIB_FTM_DA_CAL( g_hResourceContext, numSegments, segLenList, rgiList, gsmModeList, paRangeList);

   // re-store the mode ID
   gsmModeID = FTM_MODE_ID_GSM;
   UUT_FTM_SET_MODE_ID_GSM(chainID, gsmModeID);

   if(status == 0)
   {
      //throw exception
   }
}
#define  RFNV_GSM_850_CHAR_PREDIST_ENV_GAIN_I      (24194)
#define  RFNV_GSM_900_CHAR_PREDIST_ENV_GAIN_I      (24195)
#define  RFNV_GSM_1800_CHAR_PREDIST_ENV_GAIN_I     (24196)
#define  RFNV_GSM_1900_CHAR_PREDIST_ENV_GAIN_I     (24197)

map<unsigned short, unsigned short> GetDigGainMap( unsigned int nvMode )
{
   // Retrieve meanings of DigGain=2...17 RFNV_ENV_GAIN_SIZE
   map<unsigned short, unsigned short> digGainMap;  // map<mode={2...17}, DigGain>

   unsigned short nvID       = 0;
   unsigned short nvSize     = 128;
   unsigned char nvArr[128]  = {0xde, 0xad, 0xbe, 0xa7};
   unsigned short readStatus = 0;
   bool bResult              = true;

   switch(nvMode)
   {
      case NV_MODE_GSM_EGSM:
         nvID = RFNV_GSM_900_CHAR_PREDIST_ENV_GAIN_I;
         break;
      case NV_MODE_GSM_850:
         nvID = RFNV_GSM_850_CHAR_PREDIST_ENV_GAIN_I;
         break;
      case NV_MODE_GSM_DCS:
         nvID = RFNV_GSM_1800_CHAR_PREDIST_ENV_GAIN_I;
         break;
      case NV_MODE_GSM_1900:
         nvID = RFNV_GSM_1900_CHAR_PREDIST_ENV_GAIN_I;
         break;
      default:
         //throw exception
         break;
   }

   // sets the mode ID - default to C0
   // function handles properly setting FTM_MODE_ID_GSM_EXTENDED_CX
   UUT_FTM_SET_MODE_ID_GSM( 0, FTM_MODE_ID_GSM_EXTENDED_C );

   unsigned short retType = 3;   // nvArr[0] points to digGainMap

   bResult = ( QLIB_FTM_GSM_API_ReadEnvGainNV( g_hResourceContext, nvID, nvArr, &nvSize, &readStatus, retType ) != 0 );

   if(readStatus != 0 || !bResult)
   {
      //throw exception
   }

   // Generate mode -> digital gain map
   for (unsigned short digGainIter=0; digGainIter<RFNV_ENV_GAIN_ITEMS; digGainIter++)
   {
      digGainMap[RFNV_ENV_GAIN_MODE_BEGIN+digGainIter] = nvArr[digGainIter];
   }

   return digGainMap;
}

void InitCharV3Table()
{
   char_DPD_Tbl charV3Table[3][4] =
   {
      {  {
            975,18,{323.931, 671.018, 1027.28, 1389.57, 1756.84, 2127.31, 2501.07, 2877.8, 3257.19, 3641.84, 4034.66, 4429.24, 4827.51, 5231.39, 5641.41, 6057.38, 6475.02, 6890.36, 7301.7, 7707.69, 8109.45, 8506.59, 8899.9, 9290.15, 9676.43, 10059.1, 10438.1, 10812.2, 11183.7, 11550.5, 11914.8, 12275.5, 12634.6, 12990.7, 13345.6, 13698.6, 14050.2, 14400.1, 14748.7, 15095.9, 15441.1, 15784.8, 16126, 16465.8, 16804, 17139.5, 17473.4, 17806, 18136, 18464.8, 18792.3, 19118.1, 19442.4, 19766.2, 20088.7, 20409.8, 20730.5, 21050, 21369.2, 21687.2, 22004.6, 22322.5, 22638.3, 22955, 23270.9, 23586.4, 23901.4, 24216.2, 24530.8, 24845.2, 25159.1, 25473, 25787, 26101, 26414.4, 26727, 27039.8, 27353.2, 27665.7, 27978.3, 28290.4, 28602.1, 28913.9, 29225.3, 29536.5, 29847.2, 30157.7, 30466.9, 30776.4, 31086, 31394.9, 31703.2, 32011.3, 32318.3, 32624.9, 32931.7, 33237.8, 33543.4, 33848.9, 34154.1, 34459.3, 34764.5, 35069.4, 35373.2, 35677.1, 35981.2, 36287.5, 36595.8, 36903.7, 37211.4, 37518.8, 37826.3, 38135.5, 38450.7, 38770.8, 39092.6, 39414.5, 39736.1, 40057.6, 40378.8, 40707.5, 41042.4, 41377.3, 41712.8, 42048.1, 42383.5, 42719.3, 43052.3},\
            {-12.4637, -6.44307, -2.92124, -0.42247, 1.51573, 3.09936, 4.43829, 5.59813, 6.62118, 7.53633, 8.36418, 9.11996, 9.8152, 10.4589, 11.0582, 11.6187, 12.1453, 12.6418, 13.1114, 13.5569, 13.9807, 14.3848, 14.7709, 15.1406, 15.4951, 15.8358, 16.1636, 16.4795, 16.7843, 17.0788, 17.3636, 17.6393, 17.9066, 18.1659, 18.4177, 18.6624, 18.9004, 19.132, 19.3576, 19.5775, 19.792, 20.0013, 20.2057, 20.4054, 20.6006, 20.7915, 20.9783, 21.1612, 21.3403, 21.5157, 21.6877, 21.8564, 22.0218, 22.1842, 22.3436, 22.5001, 22.6538, 22.8049, 22.9534, 23.0994, 23.2429, 23.3842, 23.5231, 23.6599, 23.7946, 23.9272, 24.0578, 24.1865, 24.3133, 24.4383, 24.5615, 24.683, 24.8028, 24.921, 25.0376, 25.1526, 25.2661, 25.3782, 25.4889, 25.5981, 25.706, 25.8126, 25.9179, 26.0219, 26.1247, 26.2263, 26.3267, 26.426, 26.5241, 26.6212, 26.7172, 26.8121, 26.906, 26.9989, 27.0908, 27.1818, 27.2718, 27.3609, 27.449, 27.5363, 27.6228, 27.7083, 27.7931, 27.877, 27.9601, 28.0424, 28.124, 28.2048, 28.2849, 28.3642, 28.4428, 28.5207, 28.5979, 28.6744, 28.7503, 28.8255, 28.9, 28.974, 29.0473, 29.12, 29.192, 29.2635, 29.3344, 29.4048, 29.4745, 29.5437, 29.6124, 29.68},\
            {708.892, 665.226, 576.858, 522.528, 477.604, 440.817, 410.473, 384.344, 361.858, 343.712, 328.111, 305.364, 278.583, 250.024, 221.438, 194.282, 168.673, 144.713, 122.258, 101.103, 81.4063, 63.1795, 46.8159, 32.8744, 21.1194, 12.1783, 5.93665, 1.74466, 0, 0.496704, 2.09584, 4.30276, 7.03442, 10.1643, 13.6084, 17.3439, 21.283, 25.3512, 29.7684, 34.6244, 39.9318, 45.3262, 51.2099, 57.4055, 63.8279, 71.0083, 78.3113, 85.674, 93.4168, 101.433, 109.57, 117.77, 126.151, 134.482, 142.795, 151.208, 159.648, 168.299, 176.846, 185.18, 193.557, 201.821, 209.894, 217.786, 225.491, 233.049, 240.229, 247.068, 253.608, 260.105, 266.111, 271.7, 276.96, 281.584, 285.98, 290.206, 294.026, 297.38, 300.085, 302.579, 304.854, 306.891, 308.623, 310.007, 311.279, 312.414, 313.321, 313.888, 314.443, 315.127, 315.827, 316.526, 317.225, 317.618, 317.918, 318.617, 319.608, 320.37, 321.006, 321.987, 323.137, 324.386, 325.778, 327.14, 328.407, 329.622, 331.035, 332.457, 333.955, 335.564, 337.141, 338.737, 340.331, 341.963, 343.514, 344.876, 346.233, 347.556, 348.734, 349.844, 350.725, 351.485, 352.838, 354.481, 356.151, 357.821, 359.493, 361.371},\
            {5,15,40,60},{6.46667,15.6,24.9333,28.8}
         },\

         {
            975,19,{341.473, 702.175, 1069.67, 1442.23, 1818.28, 2197.07, 2578.5, 2962.03, 3351.33, 3751.38, 4156.75, 4565.86, 4978.78, 5395.6, 5814.75, 6234.12, 6651.46, 7065.09, 7473.76, 7877.63, 8276.51, 8670.86, 9060.26, 9445.84, 9827.15, 10204.2, 10578.3, 10947.7, 11314.5, 11677.6, 12038, 12395.8, 12750.8, 13103.9, 13454.8, 13803.7, 14150.6, 14495.7, 14838.8, 15180.4, 15520.4, 15858, 16194, 16528.7, 16861.1, 17191.8, 17521.4, 17849.4, 18175.5, 18500.3, 18824.4, 19147.3, 19468.8, 19789.8, 20110.3, 20429.6, 20748.3, 21066.5, 21384.3, 21701.3, 22018, 22334.2, 22649.5, 22965.2, 23279.7, 23593.9, 23907.8, 24220.8, 24533.2, 24845.9, 25157.5, 25468.3, 25778.5, 26088.2, 26397.9, 26707, 27014.7, 27322, 27629.1, 27935.1, 28241.4, 28547.2, 28852.2, 29157.5, 29462.1, 29766.2, 30070, 30373.3, 30678.4, 30983.3, 31287.6, 31591.3, 31895.2, 32199.6, 32506.7, 32814.5, 33121.5, 33428.2, 33734.6, 34042.7, 34355.9, 34671.1, 34986.5, 35301.1, 35615.4, 35930.9, 36253.6, 36583.9, 36914.2, 37244.2, 37573.9, 37904, 38238.8, 38587.6, 38939.5, 39291.7, 39643.7, 39996.2, 40348.5, 40706.5, 41066.1, 41426.1, 41786.4, 42147.2, 42507.7, 42869.6, 43232.7, 43593.3},\
            {-11.2537, -5.23307, -1.71124, 0.78753, 2.72573, 4.30936, 5.64829, 6.80813, 7.83118, 8.74633, 9.57418, 10.33, 11.0252, 11.6689, 12.2682, 12.8287, 13.3553, 13.8518, 14.3214, 14.7669, 15.1907, 15.5948, 15.9809, 16.3506, 16.7051, 17.0458, 17.3736, 17.6895, 17.9943, 18.2888, 18.5736, 18.8493, 19.1166, 19.3759, 19.6277, 19.8724, 20.1104, 20.342, 20.5676, 20.7875, 21.002, 21.2113, 21.4157, 21.6154, 21.8106, 22.0015, 22.1883, 22.3712, 22.5503, 22.7257, 22.8977, 23.0664, 23.2318, 23.3942, 23.5536, 23.7101, 23.8638, 24.0149, 24.1634, 24.3094, 24.4529, 24.5942, 24.7331, 24.8699, 25.0046, 25.1372, 25.2678, 25.3965, 25.5233, 25.6483, 25.7715, 25.893, 26.0128, 26.131, 26.2476, 26.3626, 26.4761, 26.5882, 26.6989, 26.8081, 26.916, 27.0226, 27.1279, 27.2319, 27.3347, 27.4363, 27.5367, 27.636, 27.7341, 27.8312, 27.9272, 28.0221, 28.116, 28.2089, 28.3008, 28.3918, 28.4818, 28.5709, 28.659, 28.7463, 28.8328, 28.9183, 29.0031, 29.087, 29.1701, 29.2524, 29.334, 29.4148, 29.4949, 29.5742, 29.6528, 29.7307, 29.8079, 29.8844, 29.9603, 30.0355, 30.11, 30.184, 30.2573, 30.33, 30.402, 30.4735, 30.5444, 30.6148, 30.6845, 30.7537, 30.8224, 30.89},\
            {875.348, 799.22, 674.54, 595.527, 533.367, 481.831, 438.309, 401.432, 368.291, 337.685, 309.793, 284.179, 255.91, 222.947, 187.617, 153.313, 123.309, 98.8695, 79.163, 62.449, 47.5602, 33.927, 22.1743, 12.3658, 5.2933, 0.861267, 0, 1.31079, 4.35986, 9.25414, 14.6018, 20.5651, 26.9648, 33.0996, 39.6455, 46.2115, 52.6031, 59.256, 65.8552, 72.6382, 79.7809, 86.7685, 94.1562, 101.893, 109.462, 117.454, 125.71, 133.95, 142.376, 151.083, 159.576, 168.043, 176.78, 185.68, 194.589, 203.495, 212.196, 220.692, 229.015, 237.554, 245.814, 253.779, 261.52, 269.12, 276.562, 283.629, 290.614, 297.315, 303.448, 309.403, 315.33, 320.879, 325.923, 330.8, 335.56, 340.318, 344.873, 349.191, 353.41, 357.615, 361.753, 365.711, 369.54, 373.409, 377.486, 381.415, 385.181, 388.913, 392.896, 397.157, 401.329, 405.481, 409.489, 413.62, 418.147, 422.702, 427.188, 431.765, 436.257, 440.723, 445.454, 450.352, 455.169, 459.907, 464.736, 469.505, 474.009, 478.525, 482.935, 487.211, 491.447, 496.025, 501.178, 507.014, 512.357, 517.503, 522.872, 528.419, 533.824, 539.612, 545.718, 552.016, 558.415, 564.973, 571.607, 578.326, 585.289, 592.491},\
            {5,15,40,60},{7.46667,16.6667,26.1333,29.8667}
         },\

         {
            975,20,{341.667, 699.058, 1062.63, 1429.56, 1800.21, 2172.42, 2547.16, 2923.4, 3303.63, 3697.43, 4102.23, 4509.45, 4919, 5332.15, 5747.33, 6161.85, 6571.92, 6976.02, 7373.43, 7765.05, 8151.57, 8533.75, 8911.57, 9285.28, 9655.21, 10021.4, 10383.3, 10742, 11097.1, 11449, 11797.7, 12143.4, 12486.6, 12827.8, 13166.4, 13503.5, 13838, 14171.5, 14503, 14832.6, 15161, 15487.4, 15812.5, 16136.5, 16459.1, 16780.3, 17100.4, 17419.4, 17737.2, 18053.9, 18370, 18685.1, 18999.4, 19312.4, 19624.9, 19936.5, 20247.5, 20557.1, 20866.5, 21174.7, 21482.9, 21789.7, 22095.7, 22401.8, 22706.8, 23011.2, 23314.9, 23617.9, 23920.6, 24222.7, 24524.7, 24826.4, 25127.5, 25428.4, 25728.7, 26029.5, 26330.3, 26630.8, 26931.2, 27230.8, 27532.1, 27833.9, 28135.4, 28436.4, 28738.7, 29042.1, 29347.6, 29653.4, 29958.4, 30264, 30572, 30884.2, 31197.2, 31510.6, 31823.4, 32137.3, 32456, 32782, 33108.8, 33435.3, 33761.7, 34089.5, 34426.2, 34772.6, 35119.7, 35466.4, 35813.7, 36163.5, 36525.2, 36899.3, 37273.4, 37648, 38022.7, 38401.6, 38798.5, 39200.1, 39602.9, 40006.9, 40411.4, 40833.7, 41264.9, 41698.3, 42132.7, 42568, 43022, 43479.9, 43938.9, 44395.4},\
            {-10.4037, -4.38307, -0.861244, 1.63753, 3.57573, 5.15936, 6.49829, 7.65813, 8.68118, 9.59633, 10.4242, 11.18, 11.8752, 12.5189, 13.1182, 13.6787, 14.2053, 14.7018, 15.1714, 15.6169, 16.0407, 16.4448, 16.8309, 17.2006, 17.5551, 17.8958, 18.2236, 18.5395, 18.8443, 19.1388, 19.4236, 19.6993, 19.9666, 20.2259, 20.4777, 20.7224, 20.9604, 21.192, 21.4176, 21.6375, 21.852, 22.0613, 22.2657, 22.4654, 22.6606, 22.8515, 23.0383, 23.2212, 23.4003, 23.5757, 23.7477, 23.9164, 24.0818, 24.2442, 24.4036, 24.5601, 24.7138, 24.8649, 25.0134, 25.1594, 25.3029, 25.4442, 25.5831, 25.7199, 25.8546, 25.9872, 26.1178, 26.2465, 26.3733, 26.4983, 26.6215, 26.743, 26.8628, 26.981, 27.0976, 27.2126, 27.3261, 27.4382, 27.5489, 27.6581, 27.766, 27.8726, 27.9779, 28.0819, 28.1847, 28.2863, 28.3867, 28.486, 28.5841, 28.6812, 28.7772, 28.8721, 28.966, 29.0589, 29.1508, 29.2418, 29.3318, 29.4209, 29.509, 29.5963, 29.6828, 29.7683, 29.8531, 29.937, 30.0201, 30.1024, 30.184, 30.2648, 30.3449, 30.4242, 30.5028, 30.5807, 30.6579, 30.7344, 30.8103, 30.8855, 30.96, 31.034, 31.1073, 31.18, 31.252, 31.3235, 31.3944, 31.4648, 31.5345, 31.6037, 31.6724, 31.74},\
            {917.247, 831.13, 684.912, 593.347, 520.471, 460.042, 409.474, 366.97, 328.502, 292.501, 257.25, 229.242, 202.665, 175.247, 146.225, 116.819, 89.5235, 66.6047, 48.0093, 33.6798, 23.2507, 15.2886, 9.23372, 4.99059, 1.90076, 0.215439, 0, 0.982056, 3.40552, 6.70602, 10.8308, 15.6683, 21.2148, 27.3014, 34.096, 40.9841, 48.2026, 55.8342, 63.7396, 71.6632, 79.4827, 87.5258, 95.5522, 103.618, 111.995, 120.361, 128.858, 137.191, 145.371, 153.559, 161.857, 170.191, 178.562, 186.79, 194.982, 203.137, 211.175, 218.915, 226.771, 234.591, 242.292, 249.889, 257.404, 264.82, 272.052, 279.179, 286.266, 293.312, 300.369, 307.175, 313.977, 320.751, 327.384, 333.905, 340.184, 346.389, 352.764, 358.983, 365.268, 371.577, 377.577, 383.511, 389.497, 395.767, 401.974, 408.043, 414.049, 420.152, 426.357, 432.59, 438.877, 445.173, 451.531, 457.943, 464.35, 470.889, 477.598, 484.512, 491.761, 498.866, 505.775, 513.096, 520.749, 528.557, 536.579, 544.602, 552.578, 560.833, 569.51, 578.638, 588.047, 597.464, 606.697, 616.12, 626.599, 637.919, 649.156, 660.241, 671.403, 683.424, 695.612, 707.714, 719.628, 731.243, 742.604, 753.332, 763.644, 773.989},\
            {5,15,40,60},{8.4,17.7667,27.3,30.7}
         },\

         {
            975,21,{345.825, 698.424, 1053.27, 1409.24, 1766.68, 2124.69, 2483.38, 2842.63, 3203.38, 3575.15, 3964.79, 4354, 4742.72, 5132.1, 5522.96, 5913.98, 6303.35, 6688.92, 7068.88, 7443.28, 7811.57, 8174.58, 8532.19, 8884.53, 9233.19, 9577.74, 9918.68, 10256.9, 10591.3, 10923.6, 11253.1, 11580.7, 11906.1, 12229.5, 12550.5, 12869.7, 13187.5, 13504, 13818.5, 14131.8, 14443.9, 14754.8, 15064.8, 15373, 15680.6, 15987.1, 16292.2, 16596.8, 16900.3, 17202.8, 17504.9, 17805.5, 18105.1, 18404, 18701.9, 18999.4, 19295.2, 19590.6, 19884.7, 20178.7, 20471.2, 20762.9, 21053.6, 21343.6, 21633.5, 21922, 22209.5, 22496.9, 22784.4, 23071, 23357.4, 23643.8, 23929.8, 24216.4, 24502.7, 24789.3, 25077, 25365.3, 25653.7, 25943.1, 26232.7, 26526.1, 26820, 27113.5, 27408.9, 27705.9, 28006.9, 28311.2, 28614.6, 28919.1, 29227.8, 29538.3, 29857.2, 30176.4, 30495.4, 30818.3, 31144.2, 31480.1, 31819.5, 32159.6, 32500.9, 32848.6, 33205.5, 33570.9, 33938.1, 34306.5, 34680.6, 35068.7, 35468.1, 35871.4, 36276.3, 36688.5, 37130.6, 37578.3, 38034.3, 38495.7, 38988.8, 39518.4, 40050, 40584, 41174.4, 41795.1, 42420, 43028.7, 43584.3, 44109.4, 44576.7, 44977.9},\
            {-9.80367, -3.78307, -0.261244, 2.23753, 4.17573, 5.75936, 7.09829, 8.25813, 9.28118, 10.1963, 11.0242, 11.78, 12.4752, 13.1189, 13.7182, 14.2787, 14.8053, 15.3018, 15.7714, 16.2169, 16.6407, 17.0448, 17.4309, 17.8006, 18.1551, 18.4958, 18.8236, 19.1395, 19.4443, 19.7388, 20.0236, 20.2993, 20.5666, 20.8259, 21.0777, 21.3224, 21.5604, 21.792, 22.0176, 22.2375, 22.452, 22.6613, 22.8657, 23.0654, 23.2606, 23.4515, 23.6383, 23.8212, 24.0003, 24.1757, 24.3477, 24.5164, 24.6818, 24.8442, 25.0036, 25.1601, 25.3138, 25.4649, 25.6134, 25.7594, 25.9029, 26.0442, 26.1831, 26.3199, 26.4546, 26.5872, 26.7178, 26.8465, 26.9733, 27.0983, 27.2215, 27.343, 27.4628, 27.581, 27.6976, 27.8126, 27.9261, 28.0382, 28.1489, 28.2581, 28.366, 28.4726, 28.5779, 28.6819, 28.7847, 28.8863, 28.9867, 29.086, 29.1841, 29.2812, 29.3772, 29.4721, 29.566, 29.6589, 29.7508, 29.8418, 29.9318, 30.0209, 30.109, 30.1963, 30.2828, 30.3683, 30.4531, 30.537, 30.6201, 30.7024, 30.784, 30.8648, 30.9449, 31.0242, 31.1028, 31.1807, 31.2579, 31.3344, 31.4103, 31.4855, 31.56, 31.634, 31.7073, 31.78, 31.852, 31.9235, 31.9944, 32.0648, 32.1345, 32.2037, 32.2724, 32.34},\
            {804.771, 733.244, 608.01, 531.359, 468.655, 416.718, 374.321, 337.727, 304.968, 275.699, 254.399, 229.637, 198.684, 164.84, 131.913, 101.383, 75.549, 54.7251, 38.1546, 25.1353, 14.6728, 7.14459, 2.41159, 0, 0.0717468, 2.13879, 5.75671, 10.3756, 15.8738, 22.346, 29.0808, 36.178, 43.8151, 51.4941, 59.1661, 67.0391, 74.993, 83.1076, 91.3643, 99.3965, 107.603, 116.142, 124.366, 132.397, 140.624, 148.752, 156.785, 164.725, 172.505, 180.256, 187.772, 195.157, 202.429, 209.355, 216.163, 222.895, 229.783, 236.682, 243.346, 250.263, 257.278, 264.115, 271.029, 278.395, 285.561, 292.67, 300.171, 307.805, 315.455, 323.081, 330.797, 338.75, 346.749, 354.949, 363.231, 371.338, 379.66, 388.184, 396.465, 404.649, 413.012, 421.603, 430.091, 438.651, 447.293, 455.823, 464.398, 473.296, 482.231, 491.162, 500.018, 509.209, 519.036, 528.936, 538.89, 548.767, 558.65, 568.979, 580.289, 591.996, 603.72, 615.421, 627.569, 640.615, 654.52, 668.577, 682.714, 697.317, 712.618, 728.534, 744.4, 760.475, 777.219, 793.805, 810.715, 827.029, 843.435, 860.086, 875.411, 889.003, 902.561, 915.221, 925.935, 935.335, 943.486, 950.716, 956.486, 961.463},\
            {5,15,40,60},{9.36667,18.9333,28.5333,31.3667}
         }
      },\

      {  {
            37,18,{338.823, 704.928, 1080.81, 1463.64, 1852.02, 2244.85, 2641.07, 3040.32, 3441.91, 3853.14, 4270.36, 4692.29, 5119.18, 5552.34, 5989.09, 6425.64, 6858.61, 7287.96, 7713.92, 8137.77, 8560.05, 8980.33, 9398.53, 9813.46, 10223.8, 10630.7, 11031.6, 11428.4, 11821.3, 12210.7, 12597.9, 12980.9, 13362.9, 13742.7, 14120.8, 14496.2, 14870.4, 15242.2, 15612, 15980, 16344.8, 16708.7, 17069.8, 17429.3, 17787.2, 18142.3, 18495.8, 18848.6, 19199, 19548, 19896, 20242.9, 20588.6, 20933, 21276.6, 21619, 21960.5, 22301.4, 22641.8, 22981.2, 23319.6, 23657.4, 23994.6, 24331.6, 24667.9, 25003.5, 25339.1, 25674.4, 26009.2, 26343.8, 26677.8, 27011.9, 27346.1, 27680.1, 28013.5, 28346.5, 28679.2, 29012.7, 29345.3, 29677.8, 30009.6, 30341.2, 30672.7, 31003.8, 31334.4, 31664.9, 31994.3, 32323.4, 32651.9, 32980.1, 33307.7, 33635.3, 33962.1, 34288.1, 34612.8, 34938, 35262.6, 35586.5, 35910.5, 36234, 36557.3, 36879.9, 37203, 37525.6, 37848.2, 38169.7, 38493.5, 38818.7, 39145.2, 39471.9, 39798.1, 40124.5, 40450.8, 40781, 41114, 41448.8, 41784.1, 42119.6, 42454.6, 42789.6, 43128.3, 43468.7, 43808.8, 44149, 44488.6, 44828.3, 45167.9, 45494.7},\
            {-12.4637, -6.44307, -2.92124, -0.42247, 1.51573, 3.09936, 4.43829, 5.59813, 6.62118, 7.53633, 8.36418, 9.11996, 9.8152, 10.4589, 11.0582, 11.6187, 12.1453, 12.6418, 13.1114, 13.5569, 13.9807, 14.3848, 14.7709, 15.1406, 15.4951, 15.8358, 16.1636, 16.4795, 16.7843, 17.0788, 17.3636, 17.6393, 17.9066, 18.1659, 18.4177, 18.6624, 18.9004, 19.132, 19.3576, 19.5775, 19.792, 20.0013, 20.2057, 20.4054, 20.6006, 20.7915, 20.9783, 21.1612, 21.3403, 21.5157, 21.6877, 21.8564, 22.0218, 22.1842, 22.3436, 22.5001, 22.6538, 22.8049, 22.9534, 23.0994, 23.2429, 23.3842, 23.5231, 23.6599, 23.7946, 23.9272, 24.0578, 24.1865, 24.3133, 24.4383, 24.5615, 24.683, 24.8028, 24.921, 25.0376, 25.1526, 25.2661, 25.3782, 25.4889, 25.5981, 25.706, 25.8126, 25.9179, 26.0219, 26.1247, 26.2263, 26.3267, 26.426, 26.5241, 26.6212, 26.7172, 26.8121, 26.906, 26.9989, 27.0908, 27.1818, 27.2718, 27.3609, 27.449, 27.5363, 27.6228, 27.7083, 27.7931, 27.877, 27.9601, 28.0424, 28.124, 28.2048, 28.2849, 28.3642, 28.4428, 28.5207, 28.5979, 28.6744, 28.7503, 28.8255, 28.9, 28.974, 29.0473, 29.12, 29.192, 29.2635, 29.3344, 29.4048, 29.4745, 29.5437, 29.6124, 29.68},\
            {333.149, 325.679, 314.961, 307.887, 302.592, 298.071, 294.312, 291.268, 289.356, 287.319, 283.847, 276.356, 262.389, 239.749, 210.025, 177.212, 146.223, 119.683, 98.6557, 81.3695, 66.3404, 52.7692, 40.2081, 28.9527, 19.2827, 11.0396, 5.50827, 2.09985, 0.390594, 0, 0.862152, 2.54776, 4.78015, 7.36346, 10.3943, 13.8527, 17.5779, 21.514, 26.2925, 31.2053, 36.56, 42.4877, 48.8242, 56.0218, 63.4343, 71.0969, 79.1371, 87.2792, 95.8369, 104.684, 113.555, 122.586, 131.83, 141.207, 150.786, 160.239, 169.788, 179.386, 189.175, 198.751, 208.089, 217.526, 226.919, 235.851, 244.363, 252.882, 261.293, 269.246, 276.585, 283.777, 290.794, 297.404, 303.416, 308.829, 314.118, 319.35, 324.462, 328.9, 332.81, 336.581, 340.541, 344.037, 347.248, 350.597, 353.509, 356.311, 359.272, 362.238, 365.194, 367.974, 370.653, 373.422, 376.219, 379.018, 381.779, 384.605, 387.41, 390.208, 393.081, 395.97, 398.781, 401.561, 404.205, 406.928, 409.935, 412.88, 415.804, 418.944, 421.944, 425.105, 428.492, 431.896, 435.165, 438.597, 442.098, 445.492, 448.502, 451.515, 454.672, 457.837, 461.099, 464.333, 466.873, 468.485, 469.882, 471.115, 472.208, 473.136},\
            {5,15,40,60},{6.0333,15.2333,24.4667,28.3667}
         },\

         {
            37,19,{356.322, 733.985, 1120.53, 1512.2, 1908.14, 2307.75, 2710.25, 3115.04, 3525.7, 3946.93, 4373.91, 4805.08, 5239.18, 5675.26, 6113.68, 6552.46, 6990.34, 7424.74, 7855.09, 8280.51, 8701.72, 9117.81, 9529.38, 9936.57, 10338.9, 10736.5, 11131.1, 11520.4, 11907.1, 12290.3, 12670.6, 13048.2, 13424, 13797.5, 14169.1, 14538.3, 14906, 15271.4, 15634.9, 15996.5, 16355.4, 16713.3, 17068.4, 17421.3, 17773.5, 18122.8, 18470.7, 18817.4, 19162.2, 19505.6, 19848.2, 20189.4, 20529.8, 20869, 21207.9, 21545.9, 21882.9, 22219.3, 22555.2, 22891, 23225.9, 23560.2, 23893.9, 24227.8, 24560.8, 24893.6, 25225.5, 25557.7, 25889.3, 26219.9, 26550.7, 26881, 27211.1, 27540.4, 27869.2, 28197.1, 28525.1, 28852.4, 29179.3, 29505.3, 29830.7, 30155.7, 30480.2, 30804.2, 31127.9, 31450.8, 31773.4, 32095, 32416.3, 32738, 33059.1, 33379.9, 33700.2, 34020, 34340.9, 34662.2, 34984.6, 35306.6, 35627.9, 35949.5, 36274.1, 36602.6, 36933.2, 37264.3, 37595.2, 37925.5, 38257.2, 38599.2, 38947.7, 39297.4, 39646.7, 39996.5, 40345.8, 40699.4, 41060.2, 41421.3, 41782.1, 42143.4, 42504.6, 42865.7, 43224.1, 43576.1, 43926.2, 44276.8, 44626.8, 44976.8, 45318.3, 45645.8},\
            {-11.2537, -5.23307, -1.71124, 0.78753, 2.72573, 4.30936, 5.64829, 6.80813, 7.83118, 8.74633, 9.57418, 10.33, 11.0252, 11.6689, 12.2682, 12.8287, 13.3553, 13.8518, 14.3214, 14.7669, 15.1907, 15.5948, 15.9809, 16.3506, 16.7051, 17.0458, 17.3736, 17.6895, 17.9943, 18.2888, 18.5736, 18.8493, 19.1166, 19.3759, 19.6277, 19.8724, 20.1104, 20.342, 20.5676, 20.7875, 21.002, 21.2113, 21.4157, 21.6154, 21.8106, 22.0015, 22.1883, 22.3712, 22.5503, 22.7257, 22.8977, 23.0664, 23.2318, 23.3942, 23.5536, 23.7101, 23.8638, 24.0149, 24.1634, 24.3094, 24.4529, 24.5942, 24.7331, 24.8699, 25.0046, 25.1372, 25.2678, 25.3965, 25.5233, 25.6483, 25.7715, 25.893, 26.0128, 26.131, 26.2476, 26.3626, 26.4761, 26.5882, 26.6989, 26.8081, 26.916, 27.0226, 27.1279, 27.2319, 27.3347, 27.4363, 27.5367, 27.636, 27.7341, 27.8312, 27.9272, 28.0221, 28.116, 28.2089, 28.3008, 28.3918, 28.4818, 28.5709, 28.659, 28.7463, 28.8328, 28.9183, 29.0031, 29.087, 29.1701, 29.2524, 29.334, 29.4148, 29.4949, 29.5742, 29.6528, 29.7307, 29.8079, 29.8844, 29.9603, 30.0355, 30.11, 30.184, 30.2573, 30.33, 30.402, 30.4735, 30.5444, 30.6148, 30.6845, 30.7537, 30.8224, 30.89},\
            {659.212, 603.682, 524.341, 472.083, 433.104, 400.401, 372.588, 349.406, 331.121, 316.498, 293.949, 264.634, 231.646, 197.805, 165.314, 136.959, 112.393, 90.9825, 71.828, 54.9756, 39.5644, 26.0959, 15.5599, 7.58416, 2.32736, 0, 0.493149, 2.95343, 6.7399, 11.3934, 16.8465, 22.6899, 28.7858, 34.99, 41.1972, 47.6599, 54.2635, 60.9297, 67.9716, 75.1345, 82.8547, 90.9797, 99.469, 108.53, 117.678, 127.233, 136.972, 146.775, 156.735, 167.005, 177.585, 188.075, 198.502, 209.241, 220.025, 230.66, 241.602, 252.433, 262.978, 273.363, 283.699, 293.992, 304.254, 313.9, 323.161, 332.25, 341.046, 349.68, 357.903, 365.428, 373.045, 380.246, 387.06, 393.329, 399.068, 404.829, 410.547, 415.929, 420.985, 425.964, 431.09, 436.1, 440.994, 445.843, 450.877, 456.06, 461.364, 466.463, 471.74, 477.465, 483.682, 489.842, 495.8, 502.077, 508.728, 515.748, 523.123, 530.727, 538.206, 545.671, 553.488, 561.805, 570.239, 578.586, 586.947, 595.182, 603.524, 612.271, 621.276, 630.41, 639.615, 648.643, 657.422, 665.783, 673.637, 681.456, 689.164, 696.882, 704.739, 712.631, 720.609, 727.365, 732.614, 737.318, 741.922, 746.247, 749.935, 752.438},\
            {5,15,40,60},{7.0667,16.3,25.7,29.5333}
         },\

         {
            37,20,{357.766, 732.167, 1112.36, 1497.25, 1884.96, 2275.3, 2667.52, 3061.58, 3461.33, 3873.87, 4293.86, 4719.18, 5147.28, 5578.18, 6008.99, 6436.5, 6859.56, 7277.41, 7689.64, 8097.21, 8500.67, 8900.34, 9295.44, 9686.64, 10073.6, 10456.3, 10834.8, 11209.6, 11579.9, 11948.1, 12312.6, 12674.2, 13033.5, 13391.2, 13746.4, 14100.2, 14452.3, 14802.7, 15150.9, 15498, 15843.7, 16187.3, 16529.3, 16870.7, 17209.8, 17547.7, 17884, 18219.5, 18552.9, 18885.6, 19217, 19547.6, 19877.2, 20206, 20533.4, 20860.4, 21186.5, 21511.8, 21836.9, 22161, 22484.4, 22807.5, 23129.7, 23451.4, 23772.4, 24093.4, 24413.5, 24733, 25052.3, 25371.1, 25689.1, 26007.1, 26324.3, 26641.7, 26958.1, 27274.1, 27590.1, 27905.7, 28221.4, 28536.7, 28852.6, 29168.3, 29483.9, 29799.8, 30115.7, 30432.4, 30751, 31070.2, 31390.1, 31710.1, 32029.8, 32351.4, 32678.1, 33005.8, 33334.3, 33663.4, 33991.6, 34323.6, 34664.2, 35008, 35352.5, 35697.1, 36042.4, 36393.9, 36757.6, 37125.3, 37494.9, 37864.6, 38236.7, 38619, 39012.8, 39413.2, 39814.9, 40217.6, 40620.5, 41031.3, 41443.6, 41856.9, 42270.5, 42684.4, 43103.6, 43511.2, 43915, 44319.4, 44723.6, 45123.9, 45480.1, 45834},\
            {-10.4037, -4.38307, -0.861244, 1.63753, 3.57573, 5.15936, 6.49829, 7.65813, 8.68118, 9.59633, 10.4242, 11.18, 11.8752, 12.5189, 13.1182, 13.6787, 14.2053, 14.7018, 15.1714, 15.6169, 16.0407, 16.4448, 16.8309, 17.2006, 17.5551, 17.8958, 18.2236, 18.5395, 18.8443, 19.1388, 19.4236, 19.6993, 19.9666, 20.2259, 20.4777, 20.7224, 20.9604, 21.192, 21.4176, 21.6375, 21.852, 22.0613, 22.2657, 22.4654, 22.6606, 22.8515, 23.0383, 23.2212, 23.4003, 23.5757, 23.7477, 23.9164, 24.0818, 24.2442, 24.4036, 24.5601, 24.7138, 24.8649, 25.0134, 25.1594, 25.3029, 25.4442, 25.5831, 25.7199, 25.8546, 25.9872, 26.1178, 26.2465, 26.3733, 26.4983, 26.6215, 26.743, 26.8628, 26.981, 27.0976, 27.2126, 27.3261, 27.4382, 27.5489, 27.6581, 27.766, 27.8726, 27.9779, 28.0819, 28.1847, 28.2863, 28.3867, 28.486, 28.5841, 28.6812, 28.7772, 28.8721, 28.966, 29.0589, 29.1508, 29.2418, 29.3318, 29.4209, 29.509, 29.5963, 29.6828, 29.7683, 29.8531, 29.937, 30.0201, 30.1024, 30.184, 30.2648, 30.3449, 30.4242, 30.5028, 30.5807, 30.6579, 30.7344, 30.8103, 30.8855, 30.96, 31.034, 31.1073, 31.18, 31.252, 31.3235, 31.3944, 31.4648, 31.5345, 31.6037, 31.6724, 31.74},\
            {811.739, 724.758, 604.591, 525.688, 466.486, 416.712, 374.677, 338.615, 307.58, 281.246, 254.173, 224.582, 193.944, 163.234, 133.452, 106.242, 82.1084, 61.5289, 44.6792, 31.0575, 20.1389, 11.717, 5.45296, 1.48068, 0, 0.541504, 2.77188, 6.3428, 11.4672, 17.485, 24.0908, 31.2392, 38.7946, 46.8312, 55.2761, 63.496, 71.8109, 80.441, 89.2796, 98.3135, 107.402, 116.651, 126.474, 136.41, 146.146, 156.33, 166.559, 176.81, 187.309, 198.001, 208.717, 219.304, 229.708, 240.165, 250.456, 260.716, 271.07, 281.293, 291.223, 300.797, 310.25, 319.645, 328.657, 337.449, 346.151, 354.661, 362.988, 370.969, 378.621, 386.359, 393.978, 401.222, 408.09, 414.958, 421.657, 428.314, 435.114, 441.755, 448.225, 454.723, 461.151, 467.941, 474.941, 481.993, 488.915, 495.764, 503.158, 510.976, 519.498, 527.857, 535.9, 544.405, 553.815, 563.558, 573.624, 583.873, 594.191, 604.89, 616.439, 628.542, 640.934, 653.334, 665.68, 678.49, 692.108, 706.409, 720.651, 734.883, 749.089, 763.856, 779.497, 795.829, 811.769, 827.81, 843.85, 860.144, 876.923, 893.944, 910.992, 927.956, 944.783, 958.351, 968.541, 976.41, 984.185, 991.261, 996.687, 1000.68},\
            {5,15,40,60},{8.0667,17.4,26.9,30.4333}
         },\

         {
            37,21,{346.937, 704.399, 1066.45, 1431.09, 1797.86, 2166.2, 2536.41, 2907.03, 3281.56, 3669.15, 4071.71, 4475.49, 4880.44, 5286.57, 5693.27, 6098.1, 6499.34, 6895.58, 7286.03, 7670.9, 8050.58, 8424.99, 8794.45, 9160.18, 9520.93, 9877.53, 10231.4, 10582, 10929.4, 11274.4, 11617.1, 11957.3, 12295.6, 12631.6, 12965.9, 13298, 13629, 13957.4, 14284.9, 14610.3, 14934.7, 15257.7, 15578.8, 15899.3, 16219, 16536.5, 16853.5, 17169.5, 17483.9, 17797.6, 18110.4, 18422.3, 18732.9, 19043.3, 19352.1, 19660.2, 19967.2, 20273.6, 20578.8, 20883.2, 21186.4, 21489.3, 21791.1, 22091.7, 22391.7, 22691.2, 22990.9, 23289.3, 23587.1, 23884.8, 24182.8, 24480.4, 24778.2, 25075.7, 25373.6, 25672.2, 25970.5, 26270.3, 26570.9, 26871.8, 27173.3, 27475.8, 27780.4, 28086.1, 28393.9, 28701.6, 29012, 29324.1, 29641.1, 29959.5, 30279.1, 30599.8, 30925.2, 31255.4, 31590.3, 31927.1, 32264.7, 32606.9, 32955.8, 33312.3, 33673.9, 34036.4, 34401.2, 34776.8, 35164.8, 35558.5, 35956.4, 36357.5, 36769, 37201, 37639.4, 38085.7, 38539.6, 39012.6, 39514.6, 40022, 40544.3, 41087.2, 41692.5, 42301.8, 42906.1, 43470.3, 44004.6, 44540.9, 45048.9, 45465.4, 45882.9, 46297.5},\
            {-9.80367, -3.78307, -0.261244, 2.23753, 4.17573, 5.75936, 7.09829, 8.25813, 9.28118, 10.1963, 11.0242, 11.78, 12.4752, 13.1189, 13.7182, 14.2787, 14.8053, 15.3018, 15.7714, 16.2169, 16.6407, 17.0448, 17.4309, 17.8006, 18.1551, 18.4958, 18.8236, 19.1395, 19.4443, 19.7388, 20.0236, 20.2993, 20.5666, 20.8259, 21.0777, 21.3224, 21.5604, 21.792, 22.0176, 22.2375, 22.452, 22.6613, 22.8657, 23.0654, 23.2606, 23.4515, 23.6383, 23.8212, 24.0003, 24.1757, 24.3477, 24.5164, 24.6818, 24.8442, 25.0036, 25.1601, 25.3138, 25.4649, 25.6134, 25.7594, 25.9029, 26.0442, 26.1831, 26.3199, 26.4546, 26.5872, 26.7178, 26.8465, 26.9733, 27.0983, 27.2215, 27.343, 27.4628, 27.581, 27.6976, 27.8126, 27.9261, 28.0382, 28.1489, 28.2581, 28.366, 28.4726, 28.5779, 28.6819, 28.7847, 28.8863, 28.9867, 29.086, 29.1841, 29.2812, 29.3772, 29.4721, 29.566, 29.6589, 29.7508, 29.8418, 29.9318, 30.0209, 30.109, 30.1963, 30.2828, 30.3683, 30.4531, 30.537, 30.6201, 30.7024, 30.784, 30.8648, 30.9449, 31.0242, 31.1028, 31.1807, 31.2579, 31.3344, 31.4103, 31.4855, 31.56, 31.634, 31.7073, 31.78, 31.852, 31.9235, 31.9944, 32.0648, 32.1345, 32.2037, 32.2724, 32.34},\
            {788.918, 716.651, 594.608, 518.456, 457.864, 407.524, 365.377, 330.048, 299.093, 275.507, 250.662, 217.074, 180.82, 146.36, 115.687, 88.9367, 66.2069, 47.294, 31.5541, 19.3382, 10.5406, 4.41543, 0.787743, 0, 1.53005, 4.70006, 9.5693, 16.0661, 23.8477, 32.7057, 42.2539, 52.1223, 62.275, 72.6055, 83.0671, 93.6558, 104.054, 114.154, 124.159, 134.199, 144.168, 154.17, 164.057, 173.795, 183.437, 192.849, 202.171, 211.542, 220.888, 230.062, 238.967, 247.818, 256.686, 265.566, 274.527, 283.344, 292.074, 300.957, 309.911, 318.953, 327.956, 336.958, 346.034, 355.35, 364.827, 374.237, 383.677, 393.25, 402.893, 412.434, 422.024, 431.626, 441.233, 450.992, 460.806, 470.573, 480.588, 490.617, 500.671, 510.888, 521.007, 531.298, 541.609, 552.181, 563.285, 574.428, 585.695, 596.92, 608.635, 620.796, 633.585, 646.555, 659.648, 673.107, 687.401, 702.56, 718.141, 733.858, 749.907, 767.061, 785.086, 804.017, 823.154, 842.517, 863.057, 884.807, 907.096, 929.516, 952.487, 976.493, 1000.56, 1025.07, 1049.69, 1074.58, 1100.52, 1127.17, 1154.8, 1181.57, 1210.18, 1235.72, 1258.98, 1279.55, 1297.33, 1312.63, 1324.43, 1333.87, 1342.73, 1351.38},\
            {5,15,40,60},{9.0667,18.6,28.2,31.2}
         }
      },\

      {  {
            124,18,{360.404, 748.033, 1146.11, 1551.19, 1961.77, 2376.41, 2794.98, 3216.38, 3642.93, 4076.89, 4515.45, 4957.89, 5403.57, 5852.47, 6304.01, 6756.68, 7210.59, 7663.7, 8114.14, 8562.09, 9006.3, 9446.43, 9882.56, 10314.7, 10743.1, 11167.5, 11587.8, 12005.3, 12419.3, 12831.1, 13240.6, 13647.7, 14053.8, 14456.9, 14859.3, 15259.1, 15656.3, 16052.2, 16444.2, 16835, 17221.5, 17606.7, 17989.7, 18369.1, 18747.6, 19123.8, 19497.8, 19871, 20242.5, 20612.6, 20981.8, 21350, 21717.2, 22083.1, 22448.7, 22813.7, 23177.5, 23541, 23903.7, 24266, 24627.5, 24988.4, 25348.4, 25708.7, 26068.3, 26426.9, 26785.4, 27143.5, 27501.6, 27858.6, 28215.3, 28571.6, 28927.7, 29283.4, 29638.9, 29993.1, 30347, 30701.5, 31054.7, 31407.9, 31759.8, 32111.9, 32463.7, 32814.6, 33165.3, 33515.9, 33865.2, 34214.5, 34563.6, 34912.3, 35261.1, 35609, 35956.5, 36304.4, 36651.2, 36998.7, 37346.4, 37693, 38039.7, 38386.3, 38733, 39080.5, 39429.1, 39777.7, 40125.8, 40473.6, 40822.1, 41168.9, 41512.5, 41854.8, 42197, 42539, 42880.8, 43227.8, 43576, 43927.4, 44280.1, 44632.9, 44985.2, 45337.4, 45683.1, 46028, 46372.3, 46717.5, 47061.6, 47405.5, 47749.7, 48090.9},\
            {-12.4637, -6.44307, -2.92124, -0.42247, 1.51573, 3.09936, 4.43829, 5.59813, 6.62118, 7.53633, 8.36418, 9.11996, 9.8152, 10.4589, 11.0582, 11.6187, 12.1453, 12.6418, 13.1114, 13.5569, 13.9807, 14.3848, 14.7709, 15.1406, 15.4951, 15.8358, 16.1636, 16.4795, 16.7843, 17.0788, 17.3636, 17.6393, 17.9066, 18.1659, 18.4177, 18.6624, 18.9004, 19.132, 19.3576, 19.5775, 19.792, 20.0013, 20.2057, 20.4054, 20.6006, 20.7915, 20.9783, 21.1612, 21.3403, 21.5157, 21.6877, 21.8564, 22.0218, 22.1842, 22.3436, 22.5001, 22.6538, 22.8049, 22.9534, 23.0994, 23.2429, 23.3842, 23.5231, 23.6599, 23.7946, 23.9272, 24.0578, 24.1865, 24.3133, 24.4383, 24.5615, 24.683, 24.8028, 24.921, 25.0376, 25.1526, 25.2661, 25.3782, 25.4889, 25.5981, 25.706, 25.8126, 25.9179, 26.0219, 26.1247, 26.2263, 26.3267, 26.426, 26.5241, 26.6212, 26.7172, 26.8121, 26.906, 26.9989, 27.0908, 27.1818, 27.2718, 27.3609, 27.449, 27.5363, 27.6228, 27.7083, 27.7931, 27.877, 27.9601, 28.0424, 28.124, 28.2048, 28.2849, 28.3642, 28.4428, 28.5207, 28.5979, 28.6744, 28.7503, 28.8255, 28.9, 28.974, 29.0473, 29.12, 29.192, 29.2635, 29.3344, 29.4048, 29.4745, 29.5437, 29.6124, 29.68},\
            {360.763, 353.668, 341.024, 332.592, 326.376, 321.194, 316.761, 313.369, 311.823, 313.61, 304.201, 285.359, 258.643, 227.332, 195.614, 167.434, 144.721, 127.215, 111.39, 95.8057, 80.133, 64.4249, 49.5909, 36.6167, 25.7867, 17.388, 11.3278, 7.02002, 3.93079, 1.64165, 0.718803, 0, 0.152435, 1.04615, 2.32757, 4.07224, 6.64509, 9.85592, 13.9931, 18.5984, 23.8926, 29.7453, 36.1553, 43.313, 50.7688, 58.5357, 66.7232, 75.4377, 84.3381, 93.1035, 102.295, 111.755, 121.052, 130.534, 140.026, 149.602, 159.064, 168.821, 178.058, 187.121, 196.242, 204.906, 213.679, 222.255, 230.465, 238.485, 246.381, 253.909, 261.061, 268.129, 275.128, 282.079, 288.64, 294.84, 300.886, 307.105, 312.881, 318.417, 324.268, 329.767, 334.876, 339.745, 344.777, 349.803, 354.75, 359.407, 363.898, 368.187, 372.428, 376.454, 380.195, 383.896, 387.813, 391.561, 394.96, 397.972, 400.845, 403.712, 406.561, 409.677, 413.083, 416.598, 420.005, 423.657, 427.338, 431.017, 434.793, 438.555, 442.124, 445.754, 449.525, 453.4, 457.273, 461.188, 464.759, 467.303, 469.198, 470.752, 472.123, 473.136, 473.725, 473.738, 473.738, 473.738, 473.738, 473.738, 473.738, 473.738},\
            {5,15,40,60},{5.6,14.7667,23.9333,27.7667}
         },\

         {
            124,19,{367.985, 761.741, 1165.82, 1576.41, 1992.15, 2411.72, 2834.68, 3261.48, 3695.5, 4138.19, 4587.15, 5041.9, 5502.2, 5966.09, 6430.86, 6893.25, 7352.34, 7807.12, 8256.92, 8700.81, 9139.99, 9574.06, 10003.5, 10429, 10850.4, 11268.7, 11682.8, 12094.5, 12503.5, 12910.4, 13314.6, 13716.4, 14116.7, 14514, 14909.4, 15302.1, 15692.2, 16080.1, 16464.9, 16847.9, 17227.1, 17604.8, 17980.7, 18353.3, 18725.5, 19095.4, 19463.2, 19830.4, 20196.1, 20559.8, 20923, 21285.7, 21646.8, 22007.2, 22367.1, 22726.1, 23083.7, 23441.2, 23798, 24154.5, 24509.6, 24864.7, 25219, 25573, 25927, 26279.9, 26632.7, 26984.9, 27337.7, 27689.6, 28041, 28392.2, 28742.9, 29093.8, 29444.1, 29793.7, 30143.1, 30491.5, 30839.6, 31187.6, 31534.7, 31881.5, 32227.4, 32572.1, 32916.9, 33260.9, 33604.4, 33947.4, 34289.6, 34631, 34972, 35313.5, 35654.2, 35994.6, 36335.6, 36676.8, 37019.2, 37361.6, 37703.4, 38045.2, 38387, 38736.3, 39088, 39440.3, 39792.4, 40143.8, 40495.1, 40853.4, 41218.9, 41581.1, 41943.9, 42306.5, 42668.9, 43031.5, 43393.7, 43753.2, 44112, 44470.3, 44828.8, 45187.2, 45533.9, 45875.2, 46216.2, 46556.9, 46896.9, 47236.9, 47576.6, 47913.6},\
            {-11.2537, -5.23307, -1.71124, 0.78753, 2.72573, 4.30936, 5.64829, 6.80813, 7.83118, 8.74633, 9.57418, 10.33, 11.0252, 11.6689, 12.2682, 12.8287, 13.3553, 13.8518, 14.3214, 14.7669, 15.1907, 15.5948, 15.9809, 16.3506, 16.7051, 17.0458, 17.3736, 17.6895, 17.9943, 18.2888, 18.5736, 18.8493, 19.1166, 19.3759, 19.6277, 19.8724, 20.1104, 20.342, 20.5676, 20.7875, 21.002, 21.2113, 21.4157, 21.6154, 21.8106, 22.0015, 22.1883, 22.3712, 22.5503, 22.7257, 22.8977, 23.0664, 23.2318, 23.3942, 23.5536, 23.7101, 23.8638, 24.0149, 24.1634, 24.3094, 24.4529, 24.5942, 24.7331, 24.8699, 25.0046, 25.1372, 25.2678, 25.3965, 25.5233, 25.6483, 25.7715, 25.893, 26.0128, 26.131, 26.2476, 26.3626, 26.4761, 26.5882, 26.6989, 26.8081, 26.916, 27.0226, 27.1279, 27.2319, 27.3347, 27.4363, 27.5367, 27.636, 27.7341, 27.8312, 27.9272, 28.0221, 28.116, 28.2089, 28.3008, 28.3918, 28.4818, 28.5709, 28.659, 28.7463, 28.8328, 28.9183, 29.0031, 29.087, 29.1701, 29.2524, 29.334, 29.4148, 29.4949, 29.5742, 29.6528, 29.7307, 29.8079, 29.8844, 29.9603, 30.0355, 30.11, 30.184, 30.2573, 30.33, 30.402, 30.4735, 30.5444, 30.6148, 30.6845, 30.7537, 30.8224, 30.89},\
            {608.87, 569.348, 513.865, 476.547, 449.349, 426.821, 407.499, 391.215, 379.092, 369.678, 348.573, 320.024, 286.56, 251.507, 218.849, 189.102, 163.168, 140.867, 121.96, 105.892, 92.1927, 80.4965, 70.9345, 63.4635, 58.1336, 54.7928, 53.5199, 53.9393, 55.4378, 57.9725, 61.4446, 65.8682, 70.8994, 76.5954, 82.6501, 89.0705, 95.9936, 103.237, 110.79, 118.642, 126.957, 135.417, 143.777, 152.577, 161.958, 171.614, 181.747, 191.761, 202.122, 212.988, 224.108, 235.234, 246.345, 257.899, 269.618, 281.233, 292.512, 303.95, 315.095, 325.99, 336.777, 347.086, 356.885, 366.466, 375.643, 384.07, 392.13, 399.797, 407.256, 414.12, 420.561, 426.719, 432.526, 437.977, 443.219, 448.453, 453.646, 458.714, 463.725, 468.854, 474.059, 479.316, 484.594, 490.167, 495.893, 501.525, 507.517, 513.703, 519.999, 526.725, 533.545, 540.535, 547.433, 554.469, 561.978, 569.782, 577.932, 586.356, 594.468, 602.607, 610.937, 620.074, 631.329, 643, 654.607, 665.899, 677.05, 688.044, 698.392, 707.961, 717.405, 726.849, 736.391, 746.313, 755.528, 763.355, 768.26, 773.156, 777.872, 781.812, 784.652, 785.167, 785.167, 785.167, 785.167, 785.167, 785.167, 785.167},\
            {5,15,40,60},{6.6333,15.8667,25.1333,29.0333}
         },\

         {
            124,20,{370.124, 760.504, 1158.59, 1561.6, 1968.2, 2378.45, 2790.79, 3206.39, 3629.36, 4063.2, 4502.03, 4945.77, 5394.21, 5845.46, 6297.51, 6747.87, 7193.51, 7634.1, 8069.5, 8500.13, 8924.28, 9343.47, 9758.26, 10168, 10573.2, 10974.5, 11371.9, 11765.4, 12155.6, 12542.9, 12927.5, 13310.1, 13690.5, 14069, 14445, 14819.7, 15192.7, 15562.7, 15932.2, 16299, 16664.2, 17027.8, 17389.1, 17749.5, 18108, 18464.4, 18820, 19174.6, 19527.5, 19879, 20230.1, 20579.9, 20929.3, 21277.6, 21625.6, 21972.8, 22319, 22664.8, 23010.7, 23355.3, 23700, 24043.8, 24387, 24729.7, 25072.1, 25413.9, 25755.6, 26096.7, 26436.8, 26776.9, 27116.4, 27455.6, 27794.1, 28132.6, 28470.5, 28808, 29146, 29482.9, 29820.2, 30157, 30493.8, 30831.1, 31169.2, 31507, 31845.1, 32182.7, 32520.5, 32861.1, 33202.1, 33543.8, 33885.2, 34226.8, 34571.1, 34919.4, 35270.3, 35621.3, 35971.4, 36322.4, 36683.1, 37050.4, 37420.8, 37791, 38161.8, 38534.3, 38922.8, 39323.1, 39726.2, 40129.7, 40533.7, 40938.9, 41353.4, 41754.9, 42154.4, 42554.2, 42954.8, 43352.6, 43738.3, 44118.8, 44499.7, 44880, 45257.8, 45603.8, 45949.6, 46295, 46640.1, 46984.8, 47329.6, 47671.3},\
            {-10.4037, -4.38307, -0.861244, 1.63753, 3.57573, 5.15936, 6.49829, 7.65813, 8.68118, 9.59633, 10.4242, 11.18, 11.8752, 12.5189, 13.1182, 13.6787, 14.2053, 14.7018, 15.1714, 15.6169, 16.0407, 16.4448, 16.8309, 17.2006, 17.5551, 17.8958, 18.2236, 18.5395, 18.8443, 19.1388, 19.4236, 19.6993, 19.9666, 20.2259, 20.4777, 20.7224, 20.9604, 21.192, 21.4176, 21.6375, 21.852, 22.0613, 22.2657, 22.4654, 22.6606, 22.8515, 23.0383, 23.2212, 23.4003, 23.5757, 23.7477, 23.9164, 24.0818, 24.2442, 24.4036, 24.5601, 24.7138, 24.8649, 25.0134, 25.1594, 25.3029, 25.4442, 25.5831, 25.7199, 25.8546, 25.9872, 26.1178, 26.2465, 26.3733, 26.4983, 26.6215, 26.743, 26.8628, 26.981, 27.0976, 27.2126, 27.3261, 27.4382, 27.5489, 27.6581, 27.766, 27.8726, 27.9779, 28.0819, 28.1847, 28.2863, 28.3867, 28.486, 28.5841, 28.6812, 28.7772, 28.8721, 28.966, 29.0589, 29.1508, 29.2418, 29.3318, 29.4209, 29.509, 29.5963, 29.6828, 29.7683, 29.8531, 29.937, 30.0201, 30.1024, 30.184, 30.2648, 30.3449, 30.4242, 30.5028, 30.5807, 30.6579, 30.7344, 30.8103, 30.8855, 30.96, 31.034, 31.1073, 31.18, 31.252, 31.3235, 31.3944, 31.4648, 31.5345, 31.6037, 31.6724, 31.74},\
            {905.266, 817.353, 704.72, 629.08, 573.213, 527.461, 488.273, 454.049, 424.382, 399.367, 372.029, 342.081, 308.628, 274.048, 241.46, 213.141, 190.253, 172.415, 157.682, 145.153, 134.68, 126.486, 120.312, 116.478, 115.105, 115.885, 118.406, 122.457, 127.674, 133.703, 140.348, 147.403, 154.403, 161.825, 169.555, 177.957, 186.365, 194.766, 203.573, 213.046, 222.707, 232.684, 243.187, 253.911, 264.923, 276.295, 287.708, 299.154, 310.865, 322.796, 334.537, 346.226, 358.073, 369.959, 381.915, 393.696, 405.148, 416.192, 427.202, 438.059, 448.527, 458.66, 468.679, 478.577, 487.667, 496.536, 505.28, 513.732, 521.585, 529.05, 536.368, 543.529, 550.468, 557.168, 563.622, 570.076, 576.707, 583.321, 590.122, 596.763, 603.686, 610.928, 618.803, 626.94, 635.042, 643.339, 652.095, 661.888, 672.037, 682.938, 693.887, 705.43, 717.486, 730.418, 744.305, 758.538, 772.698, 787.246, 802.637, 818.859, 835.539, 852.594, 869.663, 886.743, 904.89, 924.672, 945.214, 965.655, 986.071, 1006.34, 1025.38, 1042.31, 1057.71, 1072.99, 1088.54, 1104.36, 1117.68, 1127.37, 1134.43, 1140.89, 1145.92, 1148.76, 1149.54, 1149.54, 1149.54, 1149.54, 1149.54, 1149.54},\
            {5,15,40,60},{7.66667,16.9667,26.3667,30.0667}
         },\

         {
            124,21,{359.515, 732.593, 1110.64, 1491.74, 1875.43, 2261.05, 2647.71, 3036.56, 3429.78, 3839.39, 4256.48, 4674.13, 5093.63, 5515.24, 5938.7, 6360.07, 6778.08, 7190.83, 7597.47, 7998.94, 8395.52, 8787.98, 9176.14, 9560.59, 9940.5, 10317.2, 10689.4, 11058, 11423.5, 11785.2, 12144.8, 12500.8, 12855.1, 13207.5, 13558.6, 13907.6, 14255.4, 14601.7, 14947.3, 15291.1, 15632.8, 15974, 16313.7, 16651.9, 16988.8, 17324.2, 17658.1, 17991.7, 18323.3, 18653.8, 18983.9, 19313, 19640.6, 19967.4, 20293.9, 20619.5, 20944, 21268.1, 21591.6, 21914.8, 22236.6, 22558.6, 22879.4, 23200.1, 23519.9, 23839.5, 24158.6, 24477.6, 24796.3, 25114.4, 25432.5, 25750.9, 26068.2, 26386.4, 26704.9, 27023.1, 27341.3, 27659.9, 27979.3, 28299.9, 28621.2, 28942.7, 29265.5, 29590.3, 29917.6, 30246.8, 30576, 30907, 31240.2, 31580.4, 31923.2, 32266.3, 32609, 32958.7, 33315.1, 33679.3, 34044.6, 34410.1, 34779.2, 35159.4, 35554.4, 35952.9, 36351.8, 36753, 37166.4, 37604.6, 38049.4, 38495.7, 38946.5, 39416.5, 39915.5, 40424.1, 40935.5, 41456.5, 42022.7, 42572, 43101.9, 43626.1, 44131.4, 44616.8, 45102.9, 45498.3, 45870.4, 46242.2, 46613.8, 46985.3, 47357.4, 47726.3},\
            {-9.80367, -3.78307, -0.261244, 2.23753, 4.17573, 5.75936, 7.09829, 8.25813, 9.28118, 10.1963, 11.0242, 11.78, 12.4752, 13.1189, 13.7182, 14.2787, 14.8053, 15.3018, 15.7714, 16.2169, 16.6407, 17.0448, 17.4309, 17.8006, 18.1551, 18.4958, 18.8236, 19.1395, 19.4443, 19.7388, 20.0236, 20.2993, 20.5666, 20.8259, 21.0777, 21.3224, 21.5604, 21.792, 22.0176, 22.2375, 22.452, 22.6613, 22.8657, 23.0654, 23.2606, 23.4515, 23.6383, 23.8212, 24.0003, 24.1757, 24.3477, 24.5164, 24.6818, 24.8442, 25.0036, 25.1601, 25.3138, 25.4649, 25.6134, 25.7594, 25.9029, 26.0442, 26.1831, 26.3199, 26.4546, 26.5872, 26.7178, 26.8465, 26.9733, 27.0983, 27.2215, 27.343, 27.4628, 27.581, 27.6976, 27.8126, 27.9261, 28.0382, 28.1489, 28.2581, 28.366, 28.4726, 28.5779, 28.6819, 28.7847, 28.8863, 28.9867, 29.086, 29.1841, 29.2812, 29.3772, 29.4721, 29.566, 29.6589, 29.7508, 29.8418, 29.9318, 30.0209, 30.109, 30.1963, 30.2828, 30.3683, 30.4531, 30.537, 30.6201, 30.7024, 30.784, 30.8648, 30.9449, 31.0242, 31.1028, 31.1807, 31.2579, 31.3344, 31.4103, 31.4855, 31.56, 31.634, 31.7073, 31.78, 31.852, 31.9235, 31.9944, 32.0648, 32.1345, 32.2037, 32.2724, 32.34},\
            {960.781, 876.395, 745.54, 660.996, 596.705, 542.963, 497.498, 458.721, 426.116, 395.846, 368.006, 335.747, 303.235, 271.935, 242.471, 216.59, 194.936, 177.144, 162.323, 150.289, 140.961, 134.321, 130.213, 128.426, 128.748, 131.283, 135.931, 142.57, 151.07, 160.773, 171.135, 181.883, 193.009, 204.182, 215.378, 226.458, 237.253, 247.568, 257.539, 267.595, 277.723, 287.997, 298.23, 308.494, 319.033, 329.775, 340.218, 351.023, 362.087, 372.921, 383.765, 394.728, 405.723, 416.6, 427.355, 438.126, 448.913, 459.466, 469.904, 480.312, 490.589, 500.852, 510.829, 520.67, 530.473, 540.319, 550.082, 559.886, 569.661, 579.328, 588.921, 598.583, 608.465, 618.273, 628.102, 638.206, 648.477, 658.667, 668.913, 679.674, 690.848, 702.305, 713.866, 725.799, 738.544, 751.833, 765.887, 779.832, 794.186, 809.436, 825.935, 842.914, 860.823, 878.988, 898.148, 918.679, 940.631, 963.092, 986.041, 1010.24, 1036.14, 1063.4, 1091.65, 1119.99, 1149.19, 1180.36, 1212.79, 1245.79, 1278.73, 1312.54, 1348.75, 1385.42, 1422.4, 1459.28, 1497.94, 1531.13, 1554.22, 1570.5, 1585.46, 1595.28, 1599.33, 1601.33, 1602.06, 1602.06, 1602.06, 1602.06, 1602.06, 1602.06},\
            {5,15,40,60},{8.76667,18.2667,27.7333,30.9}
         }
      }
   };

   memcpy(charTable, charV3Table, sizeof(char_DPD_Tbl) * 3 * 4);
}

/************************************************
Function: Test_GSM_V3_Cal
Input: None
Output: unsigned char (true) if success
Description: This is the main function for GSM Char Predistortion calibration
******************************************************/
unsigned char Test_GSM_V3_Cal()
{
   GSM_EnhLinTx_Cal_Params enhTxLinCalParams;
   GSM_EnhLinTx_cal_Results calibratedEnhLinCalData;
   vector<double> enhLinTxSweepPwrs;
   GSM_DA_Cal_Results daCalResults;
   GSM_DA_Cal_Sweep_Record gsmRecord,edgeRecord;

   unsigned short stepLoopVar,chanLoopVar;
   unsigned short rgiVar, paVar, modeVar;
   unsigned short chan;

   /*Initialize the sweep params (same as cal tree parameters)*/
   Init_Enh_Cal_Sweep_Params(enhTxLinCalParams);
   //validate the params
   Validate_GSM_V3_Tx_Params(enhTxLinCalParams);

   InitCharV3Table();

   // Retrieve DigGain meanings from phone NV
   map<unsigned short, unsigned short> digGainMap;
#ifdef ON_TARGET
   digGainMap = GetDigGainMap( NV_MODE_GSM_EGSM);
#else
   //This has to be read from the Tx_Linear NV item, items 2 to 15 correspond to char predistortion digi gain. Only 4 are used.
   digGainMap[2] = 5;
   digGainMap[3] = 15;
   digGainMap[4] = 40;
   digGainMap[5] = 60;
#endif
   /*For each channel do the sweep, get the cal results and store them in appropriate format/structure for easy processing*/
   for(chanLoopVar = 0; chanLoopVar < enhTxLinCalParams.m_channels.size(); chanLoopVar++)
   {
      chan = (unsigned short)enhTxLinCalParams.m_channels[chanLoopVar];
      //--------------------set up the call box

      //----------------------sweep the call box

      //-------------------------sweep the UE
      //TODO: if on target
#ifdef ON_TARGET
      UUT_Do_GSM_EnhLinTx_Sweep( enhTxLinCalParams.m_paList, enhTxLinCalParams.m_rgiList,
                                 enhTxLinCalParams.m_modeList, (unsigned short)enhTxLinCalParams.m_stepDuration, 0 );
      //QLIB_FTM_DA_CAL( g_hResourceContext, numSegments, segLenList, rgiList, gsmModeList, paRangeList);
#endif
      //----------------------get sweep results

      Get_Enh_Cal_Sweep_Results(enhTxLinCalParams, enhLinTxSweepPwrs,chanLoopVar);

      stepLoopVar = 0;
      //Store each channel's First Segment level (trigger segment)
      //For this Qlib Demo, trigger segment is absent. But in actual there is a trigger segment and hence needs to be taken
      //care of by commented code below.
      /*
      calibratedEnhLinCalData.chanFirstSegmentLevel[chanLoopVar] = enhLinTxSweepPwrs[stepLoopVar];
      stepLoopVar++;
      */

      //Log the rest of the data
      unsigned int numSegments = enhTxLinCalParams.m_paList.size();

      for(unsigned int segmentNum = 0; segmentNum < numSegments-1; segmentNum++)
      {
         paVar = enhTxLinCalParams.m_paList[stepLoopVar];
         rgiVar = enhTxLinCalParams.m_rgiList[stepLoopVar];
         modeVar = enhTxLinCalParams.m_modeList[stepLoopVar];

         /*Based on GSM/EDGE, store the data accordingly*/
         if (enhTxLinCalParams.m_modeList[stepLoopVar] == 0)
         {
            //EDGE mode
            calibratedEnhLinCalData.channelSweepResults[chan].edgeDaSweep[paVar].rgiPwr[rgiVar] = enhLinTxSweepPwrs[stepLoopVar];
         }
         else if (enhTxLinCalParams.m_modeList[stepLoopVar] == 1)
         {
            //GSM mode
            calibratedEnhLinCalData.channelSweepResults[chan].gsmDaSweep[paVar].rgiPwr[rgiVar] = enhLinTxSweepPwrs[stepLoopVar];
         }
         /*This is for Digi Gain sweep*/
         else if (enhTxLinCalParams.m_modeList[stepLoopVar] >= RFNV_ENV_GAIN_MODE_BEGIN &&
                  enhTxLinCalParams.m_modeList[stepLoopVar] < (RFNV_ENV_GAIN_MODE_BEGIN + RFNV_ENV_GAIN_ITEMS))
         {
            if (digGainMap.find(modeVar) == digGainMap.end())
            {
               //throw exception
            }
            unsigned short digGain = digGainMap[modeVar];
            if (digGain == 0)
            {
               //throw exception
            }
            calibratedEnhLinCalData.channelSweepResults[chan].digGainSweep[digGain].digGainPwr[rgiVar] = enhLinTxSweepPwrs[stepLoopVar];
         }
         else
         {
            // Digital Gain not in phone NV - crash
            //throw exception
         }

         stepLoopVar++;
      }
   }
   QMSL_GSM_Tx_Calibration_NV gsmTxCalNv;

   /*Process and Generate NV*/
   Process_GSM_Tx_Cx_NV_From_DA_and_Enh_Predistortion_Cal(enhTxLinCalParams,calibratedEnhLinCalData, gsmTxCalNv );

   //--------------------------------------------------Create QCN-----------------------------------------------------------
   unsigned short nv_items_list_size;
   unsigned char  nv_write_to_QCN_status;
   unsigned short numItemsCopied;

   ostringstream qcn_file_path;
   qcn_file_path << "C:\\temp\\QLIBDemo-GSMV3CharPredist.qcn";

   nv_items_list_size = 18;
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_Copy_RFNV_Items_From_NV_Tool(g_hResourceContext, &numItemsCopied);

   unsigned short* backupNVStatus   = (unsigned short*) malloc(nv_items_list_size*sizeof(unsigned short));
   unsigned short* backupNVItemIDs  = (unsigned short*) malloc(nv_items_list_size*sizeof(unsigned short));

   // Copy requested AUTO CAL NV Items to RFCAL NV Manger
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_BackupNVItems(g_hResourceContext, backupNVItemIDs, backupNVStatus, nv_items_list_size);

   free(backupNVStatus);
   free(backupNVItemIDs);

   // Dump to QCN
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_Create_QCN( g_hResourceContext, (char*)qcn_file_path.str().c_str() );

   return true;
}

/************************************************GSM Char Rx********************************************************/
struct GSM_Rx_FreqComp_Params
{
public:
   nv_mode_id_type m_nv_mode;
   //! controls NV logging and writing to UUT, bitmask of RFCAL_NV_ACTION_TYPE bits
   unsigned int m_nv_action;

   bool m_write_new_chain_enumerated_nv;

   long m_rx_chainID_selection;

   int m_rx_path;
   vector<int> m_channels;
   vector<int> m_rx_gain_ranges;
   vector<double> m_rx_levels;
   int m_burst_avgs;

   //Characterized Calibration
   long m_rx_gain_measurement_channel;

   vector<int> m_rx_gain_offsets_min_lims;
   vector<int> m_rx_gain_offsets_max_lims;

    bool m_rx_do_hi_lin;
   vector<int> m_rx_gain_ranges_hi_lin;
   int m_channel_hi_lin;
   vector<double> m_rx_levels_hi_lin;
   vector<int> m_rx_gain_offsets_min_lims_hi_lin;
   vector<int> m_rx_gain_offsets_max_lims_hi_lin;

   bool m_rx_do_diveristy;
};

void Initialize_Rx_Params(GSM_Rx_FreqComp_Params &gsm_Rx_Params)
{
   gsm_Rx_Params.m_burst_avgs = 5;
   //1,31,62,92,124,975,1000,1023
   gsm_Rx_Params.m_channels.push_back(1);
   gsm_Rx_Params.m_channels.push_back(31);
   gsm_Rx_Params.m_channels.push_back(62);
   gsm_Rx_Params.m_channels.push_back(92);
   gsm_Rx_Params.m_channels.push_back(124);
   gsm_Rx_Params.m_channels.push_back(975);
   gsm_Rx_Params.m_channels.push_back(1000);
   gsm_Rx_Params.m_channels.push_back(1023);

   gsm_Rx_Params.m_nv_action = 1; //cache

   gsm_Rx_Params.m_rx_chainID_selection = 0; //chain 0

   gsm_Rx_Params.m_rx_gain_measurement_channel = 1; //use channel 1 as reference. This has to be one of the channels in the channel list

   //2800,2550,2200,2000
   gsm_Rx_Params.m_rx_gain_offsets_max_lims.push_back(2800);
   gsm_Rx_Params.m_rx_gain_offsets_max_lims.push_back(2550);
   gsm_Rx_Params.m_rx_gain_offsets_max_lims.push_back(2200);
   gsm_Rx_Params.m_rx_gain_offsets_max_lims.push_back(2000);

   //1801,1600,1650,1200
   gsm_Rx_Params.m_rx_gain_offsets_min_lims.push_back(1801);
   gsm_Rx_Params.m_rx_gain_offsets_min_lims.push_back(1600);
   gsm_Rx_Params.m_rx_gain_offsets_min_lims.push_back(1650);
   gsm_Rx_Params.m_rx_gain_offsets_min_lims.push_back(1200);
   //0,1,2,3 - lna gain range
   gsm_Rx_Params.m_rx_gain_ranges.push_back(0);
   gsm_Rx_Params.m_rx_gain_ranges.push_back(1);
   gsm_Rx_Params.m_rx_gain_ranges.push_back(2);
   gsm_Rx_Params.m_rx_gain_ranges.push_back(3);

   //-90,-90,-54,-54 - power levels
   gsm_Rx_Params.m_rx_levels.push_back(-90.0);
   gsm_Rx_Params.m_rx_levels.push_back(-90.0);
   gsm_Rx_Params.m_rx_levels.push_back(-54.0);
   gsm_Rx_Params.m_rx_levels.push_back(-54.0);

    gsm_Rx_Params.m_rx_do_hi_lin = false;
       //0,1,2,3 - lna gain range
   gsm_Rx_Params.m_rx_gain_ranges_hi_lin.push_back(0);
   gsm_Rx_Params.m_rx_gain_ranges_hi_lin.push_back(1);
   gsm_Rx_Params.m_rx_gain_ranges_hi_lin.push_back(2);
   gsm_Rx_Params.m_rx_gain_ranges_hi_lin.push_back(3);
   
   gsm_Rx_Params.m_channel_hi_lin = 1;


   //2800,2550,2200,2000
   gsm_Rx_Params.m_rx_gain_offsets_max_lims_hi_lin.push_back(2800);
   gsm_Rx_Params.m_rx_gain_offsets_max_lims_hi_lin.push_back(2550);
   gsm_Rx_Params.m_rx_gain_offsets_max_lims_hi_lin.push_back(2200);
   gsm_Rx_Params.m_rx_gain_offsets_max_lims_hi_lin.push_back(2000);

   //1801,1600,1650,1200
   gsm_Rx_Params.m_rx_gain_offsets_min_lims_hi_lin.push_back(1801);
   gsm_Rx_Params.m_rx_gain_offsets_min_lims_hi_lin.push_back(1600);
   gsm_Rx_Params.m_rx_gain_offsets_min_lims_hi_lin.push_back(1650);
   gsm_Rx_Params.m_rx_gain_offsets_min_lims_hi_lin.push_back(1200);

   gsm_Rx_Params.m_rx_do_diveristy = false;
}

void Validate_GSM_Rx_Params(GSM_Rx_FreqComp_Params &gsm_Rx_Params)
{
   if(gsm_Rx_Params.m_rx_gain_ranges.size() <= 0)
   {
      //throw exception
   }
   if(gsm_Rx_Params.m_rx_levels.size() != gsm_Rx_Params.m_rx_gain_ranges.size())
   {
      //throw exception
   }

   if(gsm_Rx_Params.m_rx_gain_offsets_max_lims.size() != gsm_Rx_Params.m_rx_gain_ranges.size())
   {
      //throw exception
   }

   if(gsm_Rx_Params.m_rx_gain_offsets_min_lims.size() != gsm_Rx_Params.m_rx_gain_ranges.size())
   {
      //throw exception
   }

   if(gsm_Rx_Params.m_rx_do_hi_lin)
   {
       if(gsm_Rx_Params.m_rx_gain_ranges_hi_lin.size() <= 0)
       {
          //throw exception
       }
       /*check if hi lin channel is present in channel list*/
       if(find(gsm_Rx_Params.m_channels.begin(),gsm_Rx_Params.m_channels.end(),gsm_Rx_Params.m_channel_hi_lin) == gsm_Rx_Params.m_channels.end())
       {
          //throw exception
       }
       if(gsm_Rx_Params.m_channel_hi_lin != gsm_Rx_Params.m_rx_gain_measurement_channel)
       {
           //throw exception "Hi Lin channel should be same as measurement channel"
       }

         //cell power levels
        for( UINT rx_lvl_index = 0; rx_lvl_index < gsm_Rx_Params.m_rx_levels.size(); ++rx_lvl_index)
        {
            int range = gsm_Rx_Params.m_rx_gain_ranges[ rx_lvl_index ];     
                  
            //for the same LNA gain range, high and low linearity CAL are done at the same cell power.
            if( find(gsm_Rx_Params.m_rx_gain_ranges_hi_lin.begin(), gsm_Rx_Params.m_rx_gain_ranges_hi_lin.end(), range) != gsm_Rx_Params.m_rx_gain_ranges_hi_lin.end() )
            {            
            gsm_Rx_Params.m_rx_levels_hi_lin.push_back( gsm_Rx_Params.m_rx_levels[ rx_lvl_index ] );
            }
        }

        if(gsm_Rx_Params.m_rx_gain_offsets_max_lims_hi_lin.size() != gsm_Rx_Params.m_rx_gain_ranges_hi_lin.size())
        {
            //throw exception
        }

        if(gsm_Rx_Params.m_rx_gain_offsets_min_lims_hi_lin.size() != gsm_Rx_Params.m_rx_gain_ranges_hi_lin.size())
        {
            //throw exception
        }
   }

   //Requirement : Gain Measurement Channel must be part of channel list
   if(find(gsm_Rx_Params.m_channels.begin(),gsm_Rx_Params.m_channels.end(),gsm_Rx_Params.m_rx_gain_measurement_channel) == gsm_Rx_Params.m_channels.end())
   {
      //throw exception
   }

   gsm_Rx_Params.m_write_new_chain_enumerated_nv = 1; //chain enumerated (multi chain) NV
}

void GroupGainRanges( const GSM_Rx_FreqComp_Params& params,  map< double, vector<int> >& batches, map< double, vector<int> >& batchesHiLin)
{

   const double INVALID_RX_LEV = -9999.99;
   double rx_level_across_channels = INVALID_RX_LEV;

   double rx_level = 0.0;
   int range       = 0;

   vector<int> ranges;
   vector<int> rangesHiLin;

   // Group together the gain ranges containing the same level
   //
   for( UINT rx_lvl_index=0; rx_lvl_index<params.m_rx_levels.size(); ++rx_lvl_index)
   {
      ranges.clear();
      rangesHiLin.clear();
      rx_level = params.m_rx_levels[ rx_lvl_index ];
      range    = params.m_rx_gain_ranges[ rx_lvl_index ];


      //If already exists, get it to just expand
      if(batches.count( rx_level ) > 0)
      {
         ranges = (*batches.lower_bound( rx_level )).second;
      }
      ranges.push_back( range );
      batches[ rx_level ] = ranges;

      if(params.m_rx_do_hi_lin)
      {
         //check if we want to do high linearity CAL at the same LNA gain range.
         //for the same LNA gain range, high and low linearity CAL are done at the same cell power.
         if( find(params.m_rx_gain_ranges_hi_lin.begin(), params.m_rx_gain_ranges_hi_lin.end(), range) != params.m_rx_gain_ranges_hi_lin.end() )
         {
            if( batchesHiLin.count( rx_level ) > 0 )
            {
               rangesHiLin = (*batchesHiLin.lower_bound( rx_level )).second;
            }
            rangesHiLin.push_back( range );
            batchesHiLin[ rx_level ] = rangesHiLin;
         }
      }
   }  

   //return rx_level_across_channels;

}
void Process_Char_Rx_Data( const GSM_Rx_FreqComp_Params& params, map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list,
      map<int, double> gain_across_grx, unsigned short chainID )
{
   //-------------------------------- Get a Map of characterized data ---------------------------------//
   map< int, map<int, double> > chanLnaGain; //from charRx, NUM_RX_CHAR_CHAN
   map<int,double> lnaGain;
   for(int chan = 0; chan < NUM_RX_CHAR_CHAN; chan++)
   {
      lnaGain.clear();
      for(int iLnaGain = 0; iLnaGain < 4 /*4 lna gain in the char table*/; iLnaGain++)
      {
          /*Ideally, the char values should be read based on chain ID. Currently only one chain is used for example*/
         lnaGain[charRx[chan].iLNAState[iLnaGain]]= charRx[chan].fGain[iLnaGain];
      }
      chanLnaGain[charRx[chan].iChanNum] = lnaGain;
      lnaGain.clear();
   }

   if( chanLnaGain.size() == 0 )
   {
      //throw exception
   }

   map<int, map<int, double> >::iterator chanIter;
   map<int, double>::iterator lnaIter;
   vector<int> charChanList;
   for(chanIter = chanLnaGain.begin(); chanIter!=chanLnaGain.end(); chanIter++)
   {
      charChanList.push_back(chanIter->first);
   }

    int grx        = 0;                                      // LNA state index
   int channel = params.m_rx_gain_measurement_channel;   // reference channel
   int curChannel = 0;                                      // all calibrated channels
   //Check if the characterized data contains all channels that need to calibrate
   //i.e check the char table channel list with the parameter channel list
   //Assume that all LNA ranges are always available
   int iChan;
   for(unsigned int chan_ind = 0; chan_ind < params.m_channels.size(); ++chan_ind )
   {
      iChan = params.m_channels[ chan_ind ];
      if(find(charChanList.begin(),charChanList.end(),iChan) == charChanList.end())
      {
         //throw exception
      }
   }

   //find the offset between the char data and the measured data for the ref channel
   map<int,double> gain_offsets_across_grx;
   map<int,double> rx_level_across_grx;
   for(unsigned int grx_index=0; grx_index<params.m_rx_gain_ranges.size(); ++grx_index)
   {
      grx = params.m_rx_gain_ranges[ grx_index ];
      gain_offsets_across_grx[grx] = chanLnaGain[channel][grx] - gain_across_grx[grx];
      rx_level_across_grx[grx] = params.m_rx_levels[grx_index];
   }

   double current_gain  = 0.0;
   int current_rssi_est = 0;
   //loop through the channel and apply the offset for each gain range and estimate the RSSI
   for(unsigned int chan_ind = 0; chan_ind < params.m_channels.size(); ++chan_ind )
   {
      iChan = params.m_channels[ chan_ind ];
      for(unsigned int grx_index=0; grx_index<params.m_rx_gain_ranges.size(); ++grx_index)
      {
         grx = params.m_rx_gain_ranges[ grx_index ];

         GSM_RX_MEAS_TYPE rx_meas;
         rx_meas.m_lna = grx;

         current_gain     = chanLnaGain[iChan][grx] - gain_offsets_across_grx[grx];
         current_rssi_est = static_cast<int>( pow(10.0,((current_gain+rx_level_across_grx[grx])/10.0)) );
         rx_meas.m_rssi = current_rssi_est;

         chan_rssis_list[iChan].push_back( rx_meas );
      }
   }
}
#define ROUNDL( d ) ((long)((d) + ((d) > 0 ? 0.5 : -0.5)))
double ROUNDD(double numToRound, int precision)
{
   if(precision<0)
      precision = 0;

   double multiplier = pow(10.0,precision);
   double dNum = multiplier * numToRound;
   dNum = ROUNDL(dNum);

   return dNum/(1.0*multiplier);
}
void ExtrapolateHiLinMeasurement( const GSM_Rx_FreqComp_Params& params, map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list, map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list_hi_lin )
{
   GSM_RX_MEAS_TYPE rx_meas;

   for( UINT lnaIndex = 0; lnaIndex < params.m_rx_gain_ranges_hi_lin.size(); lnaIndex++ )
   {
      int lna = params.m_rx_gain_ranges_hi_lin[lnaIndex];
      int rssiRefHiLin = chan_rssis_list_hi_lin[params.m_channel_hi_lin][lnaIndex].m_rssi;
      int rssiRefLoLin = chan_rssis_list[params.m_channel_hi_lin][lna].m_rssi; //the vector in chan_rssis_list is indexed by LNA state: state 0 at index 0, state 1 at index 1, ...
      double ratio = ((double)rssiRefHiLin / rssiRefLoLin); //use ratio to avoid logarithmic and exponential calculation

      for( UINT chan_ind = 0; chan_ind < params.m_channels.size(); ++chan_ind )
      {
         int channel = params.m_channels[ chan_ind ];
         if( channel == params.m_channel_hi_lin )              
            continue;

         rx_meas = chan_rssis_list[channel][lna];
         rx_meas.m_rssi = static_cast<int>( ROUNDD( (double)rx_meas.m_rssi * ratio, 0 ) );
         chan_rssis_list_hi_lin[channel].push_back(rx_meas);
      }
   }
}
void Do_GSM_Rx_FreqComp_w_CharData
(
   const GSM_Rx_FreqComp_Params& params,
   map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list,
   map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list_hi_lin
)
{
   map< double, vector<int> > batches;
   map< double, vector<int> >::iterator batches_iter;
   map< double, vector<int> > batchesHiLin;
   map< double, vector<int> >::iterator batches_iter_hi_lin;

   const double INVALID_RX_LEV = -9999.99;
   double rx_level_across_channels = INVALID_RX_LEV;

   vector<int> ranges;

   double rx_level = 0.0;
   int range       = 0;

    GroupGainRanges( params, batches, batchesHiLin);

   //-------------------------set up call box-------------------------

   //-------------------------set up mobile--------------------------
#ifdef ON_TARGET
   typedef enum
   {
      RX_FUNCTION_GET_RSSI,
      RX_FUNCTION_GET_DC_OFFSETS,
      RX_FUNCTION_STOP_CONTINOUS_RX
   } enum_rx_function_type;
   QLIB_FTM_SET_TX_OFF( g_hResourceContext );
   QLIB_FTM_SET_RX_BURST( g_hResourceContext, static_cast<unsigned char>(0), static_cast<short>(RX_FUNCTION_STOP_CONTINOUS_RX) );
#endif

   chan_rssis_list.clear();
   chan_rssis_list_hi_lin.clear();

   map<int,double> gain_across_grx;

   int num_measurements = 0;

   vector<int> rssis;

   int grx = 0;

   //! For each channel...
   int channel = params.m_rx_gain_measurement_channel;

   //set channel in the call box

   //set channel in the mobile
#ifdef ON_TARGET
   QLIB_FTM_SET_MODE_ID( g_hResourceContext, static_cast<short>(FTM_MODE_ID_GSM) );
   QLIB_FTM_SET_CHAN( g_hResourceContext, static_cast<unsigned short>(channel) );
#endif

   batches_iter = batches.begin();
   for( UINT batch_index=0; batch_index<batches.size(); ++batch_index, ++batches_iter)
   {
      rx_level = (*batches_iter).first;

      //set the level in call box

      // get rssis from the device
#ifdef ON_TARGET

      vector<int> range_list = (*batches_iter).second;
      char rng_bit_mask = 0;
      unsigned long results[8];
      for(UINT i=0; i<range_list.size(); ++i)
      {
         rng_bit_mask |= (char)pow(2.0, range_list[i]);
      }
      QLIB_FTM_RX_GAIN_RANGE_CAL( g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), static_cast<unsigned char>(params.m_burst_avgs),static_cast<unsigned char>((num_measurements==0)), results);
      ++num_measurements;
      rssis.clear();
      int range = 0;

      for(UINT range_index = 0; range_index<range_list.size(); ++range_index)
      {
         range = range_list[ range_index ];
         //the range value corresponds to the bit position
         //which in turn is the corresponding measurement index
         rssis.push_back( results[ range ] );
      }
#else
      rssis.clear();
      //for rx level of -90
      if(rx_level == -90.0)
      {
         rssis.push_back(191799);
         rssis.push_back(11441);
      }
      else //-54.0
      {
         rssis.push_back(48332768);
         rssis.push_back(150385);
      }
#endif
      for(UINT rssi_index=0; rssi_index<rssis.size(); ++rssi_index)
      {
         grx = (*batches_iter).second[rssi_index];
         // Gain Units = 10*Log10(Rssi) - RxLvldBm;
         gain_across_grx[grx] = ( 10*log10( rssis[rssi_index]/1.0 ) - rx_level);
      }

      if( true == params.m_rx_do_hi_lin )
      {
         batches_iter_hi_lin = batchesHiLin.find(rx_level);
         if( batches_iter_hi_lin != batchesHiLin.end() )
         {
            map< int, vector<GSM_RX_MEAS_TYPE> > dummy;
            //DoHiLinMeasurement( params, refChannel, (*batches_iter_hi_lin).second, false, chan_rssis_list_hi_lin, dummy );
#ifdef ON_TARGET
             QLIB_FTM_RX_SET_LINEARITY(g_hResourceContext,static_cast<unsigned short>(rfMode), channel, 1);
             for(UINT i=0; i<range_list.size(); ++i)
              {
                 rng_bit_mask |= (char)pow(2.0, range_list[i]);
              }
              QLIB_FTM_RX_GAIN_RANGE_CAL( g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), static_cast<unsigned char>(params.m_burst_avgs),static_cast<unsigned char>((num_measurements==0)), results);
              rssis.clear();
              range = 0;

              for(UINT range_index = 0; range_index<range_list.size(); ++range_index)
              {
                 range = range_list[ range_index ];
                 //the range value corresponds to the bit position
                 //which in turn is the corresponding measurement index
                 rssis.push_back( results[ range ] );
              }
#else
              rssis.clear();
              //for rx level of -90
              if(rx_level == -90.0)
              {
                 rssis.push_back(191899);
                 rssis.push_back(11541);
              }
              else //-54.0
              {
                 rssis.push_back(48332868);
                 rssis.push_back(150485);
              }
#endif
              GSM_RX_MEAS_TYPE rx_meas;
               for(UINT rssi_index=0; rssi_index < rssis.size(); ++rssi_index)
              {
                 rx_meas.m_lna = (*batches_iter).second[rssi_index];
                 rx_meas.m_rssi = rssis[ rssi_index ];
                 chan_rssis_list_hi_lin[channel].push_back( rx_meas );
              }
               
#ifdef ON_TARGET
             QLIB_FTM_RX_SET_LINEARITY(g_hResourceContext,static_cast<unsigned short>(rfMode), channel, 0);
#endif
         }
      }
   }

 Process_Char_Rx_Data( params,chan_rssis_list, gain_across_grx, (unsigned short)params.m_rx_chainID_selection /*chainID*/ );

#ifdef ON_TARGET
   //set Rx Off
   QLIB_FTM_SET_RX_BURST( g_hResourceContext, static_cast<unsigned char>(0), static_cast<short>(RX_FUNCTION_STOP_CONTINOUS_RX) );
#endif

      //populate high linearity data for all channels based on the ratio between hi and lo linearity at reference channel.
   if( true == params.m_rx_do_hi_lin )
   {
      ExtrapolateHiLinMeasurement( params, chan_rssis_list, chan_rssis_list_hi_lin );
   }
}

void Do_GSM_Rx_FreqCompPRxDRx_w_CharData
(
   const GSM_Rx_FreqComp_Params& params,
   map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list_prx,
   map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list_prx_hi_lin, 
   map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list_drx, 
   map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list_drx_hi_lin
)
{
   map< double, vector<int> > batches;
   map< double, vector<int> >::iterator batches_iter;
   map< double, vector<int> > batchesHiLin;
   map< double, vector<int> >::iterator batches_iter_hi_lin;

   const double INVALID_RX_LEV = -9999.99;
   double rx_level_across_channels = INVALID_RX_LEV;

   vector<int> ranges;

   double rx_level = 0.0;
   int range       = 0;

    GroupGainRanges( params, batches, batchesHiLin);

   //-------------------------set up call box-------------------------

   //-------------------------set up mobile--------------------------
#ifdef ON_TARGET
   typedef enum
   {
      RX_FUNCTION_GET_RSSI,
      RX_FUNCTION_GET_DC_OFFSETS,
      RX_FUNCTION_STOP_CONTINOUS_RX
   } enum_rx_function_type;
   QLIB_FTM_SET_TX_OFF( g_hResourceContext );
   QLIB_FTM_SET_RX_BURST( g_hResourceContext, static_cast<unsigned char>(0), static_cast<short>(RX_FUNCTION_STOP_CONTINOUS_RX) );
#endif

   chan_rssis_list_prx.clear();
   chan_rssis_list_drx.clear();
   chan_rssis_list_prx_hi_lin.clear();
   chan_rssis_list_drx_hi_lin.clear();

   map<int, double> gain_across_grx_prx; // map of LNA state to Gain 
   map<int, double> gain_across_grx_drx; // map of LNA state to Gain 

   vector<int> rssis_prx;
   vector<int> rssis_drx;


   int num_measurements = 0;

   int grx = 0;

   //! For each channel...
   int channel = params.m_rx_gain_measurement_channel;

   //set channel in the call box

   //set channel in the mobile
#ifdef ON_TARGET
   QLIB_FTM_SET_MODE_ID( g_hResourceContext, static_cast<short>(FTM_MODE_ID_GSM) );
   QLIB_FTM_SET_CHAN( g_hResourceContext, static_cast<unsigned short>(channel) );
#endif

   batches_iter = batches.begin();
   for( UINT batch_index=0; batch_index<batches.size(); ++batch_index, ++batches_iter)
   {
      rx_level = (*batches_iter).first;

      //set the level in call box

      // get rssis from the device
#ifdef ON_TARGET

      vector<int> range_list = (*batches_iter).second;
      char rng_bit_mask = 0;
      unsigned long prxResults[buffer_size]; // primary Rx results
      unsigned long drxResults[buffer_size]; // diversity Rx results
      for(UINT i=0; i<range_list.size(); ++i)
      {
         rng_bit_mask |= (char)pow(2.0, range_list[i]);
      }
      QLIB_FTM_RX_GAIN_RANGE_CAL_PRX_DRX( g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), static_cast<unsigned char>(params.m_burst_avgs),static_cast<unsigned char>((num_measurements==0)), prxResults,drxResults);
      ++num_measurements;
      rssis_prx.clear();
      rssis_drx.clear();
      int range = 0;

      for(UINT range_index = 0; range_index<range_list.size(); ++range_index)
      {
         range = range_list[ range_index ];
         //the range value corresponds to the bit position
         //which in turn is the corresponding measurement index
         rssis_prx.push_back( prxResults[ range ] );
         rssis_drx.push_back( drxResults[ range ] );
      }
#else
      rssis_prx.clear();
      rssis_drx.clear();
      //for rx level of -90
      if(rx_level == -90.0)
      {
         rssis_prx.push_back(191799);
         rssis_prx.push_back(11441);
         rssis_drx.push_back(191699);
         rssis_drx.push_back(11341);
      }
      else //-54.0
      {
         rssis_prx.push_back(48332768);
         rssis_prx.push_back(150385);
         rssis_drx.push_back(48332668);
         rssis_drx.push_back(150285);
      }
#endif
      for(UINT rssi_index=0; rssi_index<rssis_prx.size(); ++rssi_index)
      {
         grx = (*batches_iter).second[rssi_index];
         // Gain Units = 10*Log10(Rssi) - RxLvldBm;
         gain_across_grx_prx[grx] = ( 10*log10( rssis_prx[rssi_index]/1.0 ) - rx_level);
         gain_across_grx_drx[grx] = ( 10*log10( rssis_drx[rssi_index]/1.0 ) - rx_level);
      }

      if( true == params.m_rx_do_hi_lin )
      {
         batches_iter_hi_lin = batchesHiLin.find(rx_level);
         if( batches_iter_hi_lin != batchesHiLin.end() )
         {
            map< int, vector<GSM_RX_MEAS_TYPE> > dummy;
            //DoHiLinMeasurement( params, refChannel, (*batches_iter_hi_lin).second, false, chan_rssis_list_hi_lin, dummy );
#ifdef ON_TARGET
             QLIB_FTM_RX_SET_LINEARITY(g_hResourceContext,static_cast<unsigned short>(rfMode), channel, 1);
            for(UINT i=0; i<range_list.size(); ++i)
            {
                rng_bit_mask |= (char)pow(2.0, range_list[i]);
            }
            QLIB_FTM_RX_GAIN_RANGE_CAL_PRX_DRX( g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), static_cast<unsigned char>(params.m_burst_avgs),static_cast<unsigned char>((num_measurements==0)), prxResults,drxResults);
            rssis_prx.clear();
            rssis_drx.clear();
            int range = 0;

            for(UINT range_index = 0; range_index<range_list.size(); ++range_index)
            {
                range = range_list[ range_index ];
                //the range value corresponds to the bit position
                //which in turn is the corresponding measurement index
                rssis_prx.push_back( prxResults[ range ] );
                rssis_drx.push_back( drxResults[ range ] );
            }
#else
              rssis_prx.clear();
              rssis_drx.clear();
              //for rx level of -90
              if(rx_level == -90.0)
              {
                 rssis_prx.push_back(191899);
                 rssis_prx.push_back(11541);
                 rssis_drx.push_back(191799);
                 rssis_drx.push_back(11441);
              }
              else //-54.0
              {
                 rssis_prx.push_back(48332868);
                 rssis_prx.push_back(150485);
                 rssis_drx.push_back(48332768);
                 rssis_drx.push_back(150385);
              }
#endif
              GSM_RX_MEAS_TYPE rx_meas;
              GSM_RX_MEAS_TYPE rx_meas_drx;
               for(UINT rssi_index=0; rssi_index < rssis_prx.size(); ++rssi_index)
              {
                 rx_meas.m_lna = (*batches_iter).second[rssi_index];
                 rx_meas.m_rssi = rssis_prx[ rssi_index ];
                 chan_rssis_list_prx_hi_lin[channel].push_back( rx_meas );

                 rx_meas_drx.m_lna = (*batches_iter).second[rssi_index];
                 rx_meas_drx.m_rssi = rssis_drx[ rssi_index ];
                 chan_rssis_list_drx_hi_lin[channel].push_back( rx_meas_drx );
              }
#ifdef ON_TARGET
             QLIB_FTM_RX_SET_LINEARITY(g_hResourceContext,static_cast<unsigned short>(rfMode), channel, 0);
#endif
         }
      }
   }

 // PRx
   Process_Char_Rx_Data( params, chan_rssis_list_prx, gain_across_grx_prx, (unsigned short)params.m_rx_chainID_selection /*PRx chain ID*/ );
   // DRx
   Process_Char_Rx_Data( params, chan_rssis_list_drx, gain_across_grx_drx, (unsigned short)(params.m_rx_chainID_selection + 1) /*DRx chain ID*/ );

#ifdef ON_TARGET
   //set Rx Off
   QLIB_FTM_SET_RX_BURST( g_hResourceContext, static_cast<unsigned char>(0), static_cast<short>(RX_FUNCTION_STOP_CONTINOUS_RX) );
#endif

      //populate high linearity data for all channels based on the ratio between hi and lo linearity at reference channel.
   if( true == params.m_rx_do_hi_lin )
   {
        ExtrapolateHiLinMeasurement( params, chan_rssis_list_prx, chan_rssis_list_prx_hi_lin );
        ExtrapolateHiLinMeasurement( params, chan_rssis_list_drx, chan_rssis_list_drx_hi_lin );
   }
}

void  Process_GSM_Rx_Cal_Cx_NV
(
   const GSM_Rx_FreqComp_Params& params,
   const map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list,
   QMSL_GSM_Rx_Calibration_Params_and_Meas& gsm_rx_cal_meas,
   QMSL_GSM_Rx_Calibration_NV& gsm_rx_cal_nv, bool hi_linearity
)
{
   //set<int> ranges;
   map< int, vector<GSM_RX_MEAS_TYPE> >::const_iterator itr;
   unsigned int gain_index;

   // Select the Rx Chain ID, C0, C1, C2, etc
   gsm_rx_cal_meas.iChainId = static_cast<unsigned char>( params.m_rx_path );
   gsm_rx_cal_meas.iNvContainer = static_cast<unsigned char>(  gsm_rx_cal_meas.iChainId );

   // build qmsl data structure
   gsm_rx_cal_meas.iNumChannel = chan_rssis_list.size();
   if( chan_rssis_list.size() )
   {
      gsm_rx_cal_meas.iNumRxGainRange = chan_rssis_list.begin()->second.size();
   }
   else
   {
      gsm_rx_cal_meas.iNumRxGainRange = 0; //ranges.size();
   }

   unsigned int ch_index = 0;
   for(itr = chan_rssis_list.begin(); itr != chan_rssis_list.end(); ++itr)
   {
      gsm_rx_cal_meas.aiChannelList[ch_index ] = static_cast<unsigned short>(itr->first);
      for(gain_index=0; gain_index < itr->second.size(); ++gain_index)
      {
         gsm_rx_cal_meas.aiRSSIValue[ gain_index ][ ch_index ] = itr->second[ gain_index ].m_rssi;
         if( hi_linearity)
         {
            gsm_rx_cal_meas.aiCalibrationPwr[ gain_index ] = params.m_rx_levels_hi_lin[ gain_index ];
         }
         else
         {
         gsm_rx_cal_meas.aiCalibrationPwr[ gain_index ] = params.m_rx_levels[ gain_index ];
      }
         
      }
      ++ch_index;
   }

   bool queue_nv_indicator = 1; //cache

   if( hi_linearity)
   {
      QLIB_GSM_RX_CALIBRATION_GENERATE_NV_HI_LIN(
         g_hResourceContext,
         static_cast<unsigned short>(params.m_nv_mode),
         (void*)&gsm_rx_cal_meas,
         (void*)&gsm_rx_cal_nv,
         static_cast<unsigned char>(queue_nv_indicator)
      );
   }
   else
   {
   QLIB_GSM_RX_CALIBRATION_GENERATE_NV(
      g_hResourceContext,
      static_cast<unsigned short>(NV_MODE_GSM_EGSM),
      (void*)&gsm_rx_cal_meas,
      (void*)&gsm_rx_cal_nv,
      static_cast<unsigned char>(queue_nv_indicator)
   );
   }
}
unsigned char Test_GSM_V3_Rx_Cal()
{
   GSM_Rx_FreqComp_Params gsm_Rx_Params;
   Initialize_Rx_Params(gsm_Rx_Params);
   Validate_GSM_Rx_Params(gsm_Rx_Params);
   map< int, vector<GSM_RX_MEAS_TYPE> > chan_rssis_list;
   map< int, vector<GSM_RX_MEAS_TYPE> > chan_rssis_list_hi_lin;
   // DRx RSSI Results
   map< int, vector<GSM_RX_MEAS_TYPE> > chan_rssis_list_drx;
   // DRx RSSI Results at high linearity
   map< int, vector<GSM_RX_MEAS_TYPE> > chan_rssis_list_drx_hi_lin;
   if(!gsm_Rx_Params.m_rx_do_diveristy)
   {
        Do_GSM_Rx_FreqComp_w_CharData(gsm_Rx_Params, chan_rssis_list,chan_rssis_list_hi_lin);
   }
   else
   {
       Do_GSM_Rx_FreqCompPRxDRx_w_CharData( gsm_Rx_Params, chan_rssis_list, chan_rssis_list_hi_lin, chan_rssis_list_drx, chan_rssis_list_drx_hi_lin );
   }

   gsm_Rx_Params.m_rx_path = gsm_Rx_Params.m_rx_chainID_selection;
   QMSL_GSM_Rx_Calibration_Params_and_Meas gsm_rx_cal_meas;
   QMSL_GSM_Rx_Calibration_NV gsm_rx_cal_nv;
   Process_GSM_Rx_Cal_Cx_NV( gsm_Rx_Params, chan_rssis_list, gsm_rx_cal_meas, gsm_rx_cal_nv, false );

   if(gsm_Rx_Params.m_rx_do_diveristy)
   {
        QMSL_GSM_Rx_Calibration_Params_and_Meas gsm_rx_cal_meas;
        QMSL_GSM_Rx_Calibration_NV gsm_rx_cal_nv;
        // The diversity chain is always the primary chain plus 1.
        gsm_Rx_Params.m_rx_path = gsm_Rx_Params.m_rx_chainID_selection + 1;
        Process_GSM_Rx_Cal_Cx_NV( gsm_Rx_Params, chan_rssis_list_drx, gsm_rx_cal_meas, gsm_rx_cal_nv, false );
   }

   if(gsm_Rx_Params.m_rx_do_hi_lin)
   {
        QMSL_GSM_Rx_Calibration_Params_and_Meas gsm_rx_cal_meas_hi_lin;
        QMSL_GSM_Rx_Calibration_NV gsm_rx_cal_nv_hi_lin;
        gsm_Rx_Params.m_rx_path = gsm_Rx_Params.m_rx_chainID_selection;
        Process_GSM_Rx_Cal_Cx_NV( gsm_Rx_Params, chan_rssis_list_hi_lin, gsm_rx_cal_meas_hi_lin, gsm_rx_cal_nv_hi_lin, true ); // process main path @ hi lin

        if(gsm_Rx_Params.m_rx_do_diveristy)
       {
            QMSL_GSM_Rx_Calibration_Params_and_Meas gsm_rx_cal_meas_hi_lin;
            QMSL_GSM_Rx_Calibration_NV gsm_rx_cal_nv_hi_lin;
            // The diversity chain is always the primary chain plus 1.
            gsm_Rx_Params.m_rx_path = gsm_Rx_Params.m_rx_chainID_selection + 1;
            Process_GSM_Rx_Cal_Cx_NV( gsm_Rx_Params, chan_rssis_list_drx_hi_lin, gsm_rx_cal_meas_hi_lin, gsm_rx_cal_nv_hi_lin, true );
       }
   }
   unsigned short nv_items_list_size;
   unsigned char  nv_write_to_QCN_status;
   unsigned short numItemsCopied;

   ostringstream qcn_file_path;
   qcn_file_path << "C:\\temp\\QLIBDemo-GSMV3CharRx.qcn";

   nv_items_list_size = 18;
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_Copy_RFNV_Items_From_NV_Tool(g_hResourceContext, &numItemsCopied);

   unsigned short* backupNVStatus   = (unsigned short*) malloc(nv_items_list_size*sizeof(unsigned short));
   unsigned short* backupNVItemIDs  = (unsigned short*) malloc(nv_items_list_size*sizeof(unsigned short));

   // Copy requested AUTO CAL NV Items to RFCAL NV Manger
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_BackupNVItems(g_hResourceContext, backupNVItemIDs, backupNVStatus, nv_items_list_size);

   free(backupNVStatus);
   free(backupNVItemIDs);

   // Dump to QCN
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_Create_QCN( g_hResourceContext, (char*)qcn_file_path.str().c_str() );

   return true;
}
/***************************GSM Rx Regular Cal with Hi/Lo Lin starts*********************/
void Do_GSM_Rx_FreqComp( const GSM_Rx_FreqComp_Params& params,
   map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list,
   map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list_hi_lin)
{
      map< double, vector<int> > batches;
   map< double, vector<int> >::iterator batches_iter;
   map< double, vector<int> > batchesHiLin;
   map< double, vector<int> >::iterator batches_iter_hi_lin;

   const double INVALID_RX_LEV = -9999.99;
   double rx_level_across_channels = INVALID_RX_LEV;

   vector<int> ranges;

   double rx_level = 0.0;
   int range       = 0;

   int channel = 0;
   GSM_RX_MEAS_TYPE rx_meas;
    
   GroupGainRanges( params, batches, batchesHiLin);

   //-------------------------set up call box-------------------------

   //-------------------------set up mobile--------------------------
#ifdef ON_TARGET
   typedef enum
   {
      RX_FUNCTION_GET_RSSI,
      RX_FUNCTION_GET_DC_OFFSETS,
      RX_FUNCTION_STOP_CONTINOUS_RX
   } enum_rx_function_type;
   QLIB_FTM_SET_TX_OFF( g_hResourceContext );
   QLIB_FTM_SET_RX_BURST( g_hResourceContext, static_cast<unsigned char>(0), static_cast<short>(RX_FUNCTION_STOP_CONTINOUS_RX) );
#endif

   chan_rssis_list.clear();
   chan_rssis_list_hi_lin.clear();

   map<int,double> gain_across_grx;

   int num_measurements = 0;

   vector<int> rssis;

   int grx = 0;

   //! For each channel...
   for(UINT chan_ind = 0; chan_ind < params.m_channels.size(); ++chan_ind )
   {
      channel = params.m_channels[ chan_ind ];

       //set channel in the call box

       //set channel in the mobile
    #ifdef ON_TARGET
       QLIB_FTM_SET_MODE_ID( g_hResourceContext, static_cast<short>(FTM_MODE_ID_GSM) );
       QLIB_FTM_SET_CHAN( g_hResourceContext, static_cast<unsigned short>(channel) );
    #endif

       batches_iter = batches.begin();
       for( UINT batch_index=0; batch_index<batches.size(); ++batch_index, ++batches_iter)
       {
          rx_level = (*batches_iter).first;

          //set the level in call box

          // get rssis from the device
    #ifdef ON_TARGET

          vector<int> range_list = (*batches_iter).second;
          char rng_bit_mask = 0;
          unsigned long results[8];
          for(UINT i=0; i<range_list.size(); ++i)
          {
             rng_bit_mask |= (char)pow(2.0, range_list[i]);
          }
          QLIB_FTM_RX_GAIN_RANGE_CAL( g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), static_cast<unsigned char>(params.m_burst_avgs),static_cast<unsigned char>((num_measurements==0)), results);
          ++num_measurements;
          rssis.clear();
          int range = 0;

          for(UINT range_index = 0; range_index<range_list.size(); ++range_index)
          {
             range = range_list[ range_index ];
             //the range value corresponds to the bit position
             //which in turn is the corresponding measurement index
             rssis.push_back( results[ range ] );
          }
    #else
          rssis.clear();
          //for rx level of -90
          if(rx_level == -90.0)
          {
             rssis.push_back(191799);
             rssis.push_back(11441);
          }
          else //-54.0
          {
             rssis.push_back(48332768);
             rssis.push_back(150385);
          }
    #endif
          for(UINT rssi_index=0; rssi_index<rssis.size(); ++rssi_index)
          {
            rx_meas.m_lna = (*batches_iter).second[ rssi_index ];
            rx_meas.m_rssi = rssis[ rssi_index ];
            chan_rssis_list[channel].push_back( rx_meas );
          }

          if( true == params.m_rx_do_hi_lin && params.m_channel_hi_lin == channel)
          {
             batches_iter_hi_lin = batchesHiLin.find(rx_level);
             if( batches_iter_hi_lin != batchesHiLin.end() )
             {
                map< int, vector<GSM_RX_MEAS_TYPE> > dummy;
                //DoHiLinMeasurement( params, refChannel, (*batches_iter_hi_lin).second, false, chan_rssis_list_hi_lin, dummy );
    #ifdef ON_TARGET
                 QLIB_FTM_RX_SET_LINEARITY(g_hResourceContext,static_cast<unsigned short>(rfMode), channel, 1);
                 for(UINT i=0; i<range_list.size(); ++i)
                  {
                     rng_bit_mask |= (char)pow(2.0, range_list[i]);
                  }
                  QLIB_FTM_RX_GAIN_RANGE_CAL( g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), static_cast<unsigned char>(params.m_burst_avgs),static_cast<unsigned char>((num_measurements==0)), results);
                  rssis.clear();
                  range = 0;

                  for(UINT range_index = 0; range_index<range_list.size(); ++range_index)
                  {
                     range = range_list[ range_index ];
                     //the range value corresponds to the bit position
                     //which in turn is the corresponding measurement index
                     rssis.push_back( results[ range ] );
                  }
    #else
                  rssis.clear();
                  //for rx level of -90
                  if(rx_level == -90.0)
                  {
                     rssis.push_back(191899);
                     rssis.push_back(11541);
                  }
                  else //-54.0
                  {
                     rssis.push_back(48332868);
                     rssis.push_back(150485);
                  }
    #endif
                  GSM_RX_MEAS_TYPE rx_meas;
                   for(UINT rssi_index=0; rssi_index < rssis.size(); ++rssi_index)
                  {
                     rx_meas.m_lna = (*batches_iter).second[rssi_index];
                     rx_meas.m_rssi = rssis[ rssi_index ];
                     chan_rssis_list_hi_lin[channel].push_back( rx_meas );
                  }
               
    #ifdef ON_TARGET
                 QLIB_FTM_RX_SET_LINEARITY(g_hResourceContext,static_cast<unsigned short>(rfMode), channel, 0);
    #endif
             }
          }
       }
   }

#ifdef ON_TARGET
   //set Rx Off
   QLIB_FTM_SET_RX_BURST( g_hResourceContext, static_cast<unsigned char>(0), static_cast<short>(RX_FUNCTION_STOP_CONTINOUS_RX) );
#endif

      //populate high linearity data for all channels based on the ratio between hi and lo linearity at reference channel.
   if( true == params.m_rx_do_hi_lin )
   {
      ExtrapolateHiLinMeasurement( params, chan_rssis_list, chan_rssis_list_hi_lin );
   }
}

void Do_GSM_Rx_FreqCompPRxDRx
(
   const GSM_Rx_FreqComp_Params& params,
   map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list_prx,
   map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list_prx_hi_lin, 
   map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list_drx, 
   map< int, vector<GSM_RX_MEAS_TYPE> >& chan_rssis_list_drx_hi_lin
)
{
   map< double, vector<int> > batches;
   map< double, vector<int> >::iterator batches_iter;
   map< double, vector<int> > batchesHiLin;
   map< double, vector<int> >::iterator batches_iter_hi_lin;

   const double INVALID_RX_LEV = -9999.99;
   double rx_level_across_channels = INVALID_RX_LEV;

   vector<int> ranges;

   double rx_level = 0.0;
   int range       = 0;

   int channel = 0;
    GroupGainRanges( params, batches, batchesHiLin);

   //-------------------------set up call box-------------------------

   //-------------------------set up mobile--------------------------
#ifdef ON_TARGET
   typedef enum
   {
      RX_FUNCTION_GET_RSSI,
      RX_FUNCTION_GET_DC_OFFSETS,
      RX_FUNCTION_STOP_CONTINOUS_RX
   } enum_rx_function_type;
   QLIB_FTM_SET_TX_OFF( g_hResourceContext );
   QLIB_FTM_SET_RX_BURST( g_hResourceContext, static_cast<unsigned char>(0), static_cast<short>(RX_FUNCTION_STOP_CONTINOUS_RX) );
#endif

   chan_rssis_list_prx.clear();
   chan_rssis_list_drx.clear();
   chan_rssis_list_prx_hi_lin.clear();
   chan_rssis_list_drx_hi_lin.clear();

   map<int, double> gain_across_grx_prx; // map of LNA state to Gain 
   map<int, double> gain_across_grx_drx; // map of LNA state to Gain 

   vector<int> rssis_prx;
   vector<int> rssis_drx;


   int num_measurements = 0;

   int grx = 0;

   GSM_RX_MEAS_TYPE rx_meas_prx, rx_meas_drx;

   //! For each channel...
   //! For each channel...
   for(UINT chan_ind = 0; chan_ind < params.m_channels.size(); ++chan_ind )
   {
      channel = params.m_channels[ chan_ind ];

   //set channel in the call box

       //set channel in the mobile
    #ifdef ON_TARGET
       QLIB_FTM_SET_MODE_ID( g_hResourceContext, static_cast<short>(FTM_MODE_ID_GSM) );
       QLIB_FTM_SET_CHAN( g_hResourceContext, static_cast<unsigned short>(channel) );
    #endif

       batches_iter = batches.begin();
       for( UINT batch_index=0; batch_index<batches.size(); ++batch_index, ++batches_iter)
       {
          rx_level = (*batches_iter).first;

          //set the level in call box

          // get rssis from the device
    #ifdef ON_TARGET

          vector<int> range_list = (*batches_iter).second;
          char rng_bit_mask = 0;
          unsigned long prxResults[buffer_size]; // primary Rx results
          unsigned long drxResults[buffer_size]; // diversity Rx results
          for(UINT i=0; i<range_list.size(); ++i)
          {
             rng_bit_mask |= (char)pow(2.0, range_list[i]);
          }
          QLIB_FTM_RX_GAIN_RANGE_CAL_PRX_DRX( g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), static_cast<unsigned char>(params.m_burst_avgs),static_cast<unsigned char>((num_measurements==0)), prxResults,drxResults);
          ++num_measurements;
          rssis_prx.clear();
          rssis_drx.clear();
          int range = 0;

          for(UINT range_index = 0; range_index<range_list.size(); ++range_index)
          {
             range = range_list[ range_index ];
             //the range value corresponds to the bit position
             //which in turn is the corresponding measurement index
             rssis_prx.push_back( prxResults[ range ] );
             rssis_drx.push_back( drxResults[ range ] );
          }
    #else
          rssis_prx.clear();
          rssis_drx.clear();
          //for rx level of -90
          if(rx_level == -90.0)
          {
             rssis_prx.push_back(191799);
             rssis_prx.push_back(11441);
             rssis_drx.push_back(191699);
             rssis_drx.push_back(11341);
          }
          else //-54.0
          {
             rssis_prx.push_back(48332768);
             rssis_prx.push_back(150385);
             rssis_drx.push_back(48332668);
             rssis_drx.push_back(150285);
          }
    #endif
          for(UINT rssi_index=0; rssi_index<rssis_prx.size(); ++rssi_index)
          {
            // PRx
            rx_meas_prx.m_lna  = (*batches_iter).second[ rssi_index ];
            rx_meas_prx.m_rssi = rssis_prx[ rssi_index ];
            chan_rssis_list_prx[channel].push_back( rx_meas_prx );
            // DRx
            rx_meas_drx.m_lna  = (*batches_iter).second[ rssi_index ];
            rx_meas_drx.m_rssi = rssis_drx[ rssi_index ];
            chan_rssis_list_drx[channel].push_back( rx_meas_drx );
          }

          if( true == params.m_rx_do_hi_lin && params.m_channel_hi_lin == channel)
          {
             batches_iter_hi_lin = batchesHiLin.find(rx_level);
             if( batches_iter_hi_lin != batchesHiLin.end() )
             {
                map< int, vector<GSM_RX_MEAS_TYPE> > dummy;
                //DoHiLinMeasurement( params, refChannel, (*batches_iter_hi_lin).second, false, chan_rssis_list_hi_lin, dummy );
    #ifdef ON_TARGET
                 QLIB_FTM_RX_SET_LINEARITY(g_hResourceContext,static_cast<unsigned short>(rfMode), channel, 1);
                for(UINT i=0; i<range_list.size(); ++i)
                {
                    rng_bit_mask |= (char)pow(2.0, range_list[i]);
                }
                QLIB_FTM_RX_GAIN_RANGE_CAL_PRX_DRX( g_hResourceContext,static_cast<unsigned char>(rng_bit_mask), static_cast<unsigned char>(params.m_burst_avgs),static_cast<unsigned char>((num_measurements==0)), prxResults,drxResults);
                rssis_prx.clear();
                rssis_drx.clear();
                int range = 0;

                for(UINT range_index = 0; range_index<range_list.size(); ++range_index)
                {
                    range = range_list[ range_index ];
                    //the range value corresponds to the bit position
                    //which in turn is the corresponding measurement index
                    rssis_prx.push_back( prxResults[ range ] );
                    rssis_drx.push_back( drxResults[ range ] );
                }
    #else
                  rssis_prx.clear();
                  rssis_drx.clear();
                  //for rx level of -90
                  if(rx_level == -90.0)
                  {
                     rssis_prx.push_back(191899);
                     rssis_prx.push_back(11541);
                     rssis_drx.push_back(191799);
                     rssis_drx.push_back(11441);
                  }
                  else //-54.0
                  {
                     rssis_prx.push_back(48332868);
                     rssis_prx.push_back(150485);
                     rssis_drx.push_back(48332768);
                     rssis_drx.push_back(150385);
                  }
    #endif
                  GSM_RX_MEAS_TYPE rx_meas;
                  GSM_RX_MEAS_TYPE rx_meas_drx;
                   for(UINT rssi_index=0; rssi_index < rssis_prx.size(); ++rssi_index)
                  {
                     rx_meas.m_lna = (*batches_iter).second[rssi_index];
                     rx_meas.m_rssi = rssis_prx[ rssi_index ];
                     chan_rssis_list_prx_hi_lin[channel].push_back( rx_meas );

                     rx_meas_drx.m_lna = (*batches_iter).second[rssi_index];
                     rx_meas_drx.m_rssi = rssis_drx[ rssi_index ];
                     chan_rssis_list_drx_hi_lin[channel].push_back( rx_meas_drx );
                  }
    #ifdef ON_TARGET
                 QLIB_FTM_RX_SET_LINEARITY(g_hResourceContext,static_cast<unsigned short>(rfMode), channel, 0);
    #endif
             }
          }
       }

   }
 
#ifdef ON_TARGET
   //set Rx Off
   QLIB_FTM_SET_RX_BURST( g_hResourceContext, static_cast<unsigned char>(0), static_cast<short>(RX_FUNCTION_STOP_CONTINOUS_RX) );
#endif

      //populate high linearity data for all channels based on the ratio between hi and lo linearity at reference channel.
   if( true == params.m_rx_do_hi_lin )
   {
        ExtrapolateHiLinMeasurement( params, chan_rssis_list_prx, chan_rssis_list_prx_hi_lin );
        ExtrapolateHiLinMeasurement( params, chan_rssis_list_drx, chan_rssis_list_drx_hi_lin );
   }
}
unsigned char Test_GSM_Rx_Cal()
{
   GSM_Rx_FreqComp_Params gsm_Rx_Params;
   Initialize_Rx_Params(gsm_Rx_Params);
   Validate_GSM_Rx_Params(gsm_Rx_Params);
   map< int, vector<GSM_RX_MEAS_TYPE> > chan_rssis_list;
   map< int, vector<GSM_RX_MEAS_TYPE> > chan_rssis_list_hi_lin;
   // DRx RSSI Results
   map< int, vector<GSM_RX_MEAS_TYPE> > chan_rssis_list_drx;
   // DRx RSSI Results at high linearity
   map< int, vector<GSM_RX_MEAS_TYPE> > chan_rssis_list_drx_hi_lin;

     if(!gsm_Rx_Params.m_rx_do_diveristy)
   {
        Do_GSM_Rx_FreqComp(gsm_Rx_Params, chan_rssis_list,chan_rssis_list_hi_lin);
   }
   else
   {
       Do_GSM_Rx_FreqCompPRxDRx( gsm_Rx_Params, chan_rssis_list, chan_rssis_list_hi_lin, chan_rssis_list_drx, chan_rssis_list_drx_hi_lin );
   }

   gsm_Rx_Params.m_rx_path = gsm_Rx_Params.m_rx_chainID_selection;
   QMSL_GSM_Rx_Calibration_Params_and_Meas gsm_rx_cal_meas;
   QMSL_GSM_Rx_Calibration_NV gsm_rx_cal_nv;
   Process_GSM_Rx_Cal_Cx_NV( gsm_Rx_Params, chan_rssis_list, gsm_rx_cal_meas, gsm_rx_cal_nv, false );

   if(gsm_Rx_Params.m_rx_do_diveristy)
   {
        QMSL_GSM_Rx_Calibration_Params_and_Meas gsm_rx_cal_meas;
        QMSL_GSM_Rx_Calibration_NV gsm_rx_cal_nv;
        // The diversity chain is always the primary chain plus 1.
        gsm_Rx_Params.m_rx_path = gsm_Rx_Params.m_rx_chainID_selection + 1;
        Process_GSM_Rx_Cal_Cx_NV( gsm_Rx_Params, chan_rssis_list_drx, gsm_rx_cal_meas, gsm_rx_cal_nv, false );
   }

   if(gsm_Rx_Params.m_rx_do_hi_lin)
   {
        QMSL_GSM_Rx_Calibration_Params_and_Meas gsm_rx_cal_meas_hi_lin;
        QMSL_GSM_Rx_Calibration_NV gsm_rx_cal_nv_hi_lin;
        gsm_Rx_Params.m_rx_path = gsm_Rx_Params.m_rx_chainID_selection;
        Process_GSM_Rx_Cal_Cx_NV( gsm_Rx_Params, chan_rssis_list_hi_lin, gsm_rx_cal_meas_hi_lin, gsm_rx_cal_nv_hi_lin, true ); // process main path @ hi lin

        if(gsm_Rx_Params.m_rx_do_diveristy)
       {
            QMSL_GSM_Rx_Calibration_Params_and_Meas gsm_rx_cal_meas_hi_lin;
            QMSL_GSM_Rx_Calibration_NV gsm_rx_cal_nv_hi_lin;
            // The diversity chain is always the primary chain plus 1.
            gsm_Rx_Params.m_rx_path = gsm_Rx_Params.m_rx_chainID_selection + 1;
            Process_GSM_Rx_Cal_Cx_NV( gsm_Rx_Params, chan_rssis_list_drx_hi_lin, gsm_rx_cal_meas_hi_lin, gsm_rx_cal_nv_hi_lin, true );
       }
   }
   unsigned short nv_items_list_size;
   unsigned char  nv_write_to_QCN_status;
   unsigned short numItemsCopied;

   ostringstream qcn_file_path;
   qcn_file_path << "C:\\temp\\QLIBDemo-GSMRx.qcn";

   nv_items_list_size = 18;
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_Copy_RFNV_Items_From_NV_Tool(g_hResourceContext, &numItemsCopied);

   unsigned short* backupNVStatus   = (unsigned short*) malloc(nv_items_list_size*sizeof(unsigned short));
   unsigned short* backupNVItemIDs  = (unsigned short*) malloc(nv_items_list_size*sizeof(unsigned short));

   // Copy requested AUTO CAL NV Items to RFCAL NV Manger
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_BackupNVItems(g_hResourceContext, backupNVItemIDs, backupNVStatus, nv_items_list_size);

   free(backupNVStatus);
   free(backupNVItemIDs);

   // Dump to QCN
   nv_write_to_QCN_status = QLIB_RFCAL_NV_Manager_Create_QCN( g_hResourceContext, (char*)qcn_file_path.str().c_str() );

   return true;
}
unsigned char Test_GSMV4_WriteCharNV()
{
   HANDLE hPhone = g_hResourceContext;
   const unsigned char cacheNVItems = 0;  // set cache NV items to false
   const unsigned int reqNumMod     = 2;  // required number of modulations per band/chain/channel

   string filename = "C:\\Program Files (x86)\\Qualcomm\\QDART\\XTT\\SubSysRFCalibration\\Multimodes\\WTR1625\\RFCALInput\\WTR1625_NAEU_CA_ESC_Char.xml";
   //**************************************************//
   //       Load XML and get a node with the rf_mode
   //**************************************************//
   CoInitialize(NULL); // Initialize COM
   MSXML2::IXMLDOMDocumentPtr spXMLDoc;
   spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
   spXMLDoc->load(filename.c_str());
   if (!spXMLDoc)
   {
      CoUninitialize();
      return false; //throw RFCal_Helper_Exception("The char XML not loaded.",  __FILE__, __LINE__ );
   }

   stringstream stChar_Param_XPATH;

   stChar_Param_XPATH << "/RFCal_Data/GSMDaCharTable";//[@rf_mode='" <<  param.RF_Mode << "' and @device='" << device << "']/APTTable";
   MSXML2::IXMLDOMNodeListPtr pGSMDaCharTable = spXMLDoc->selectNodes((_bstr_t)stChar_Param_XPATH.str().c_str());

   if (!pGSMDaCharTable)
   {
      spXMLDoc.Release();
      CoUninitialize();
      return false; //throw RFCal_Helper_Exception("GSM DA char table not found.",  __FILE__, __LINE__ );
   }

   // ***************************************************************//
   //    Read each of nodes and assign APT table map
   //************************************++**************************//

   for (int i= 0; i < pGSMDaCharTable->length; i++)
   {
      MSXML2::IXMLDOMNodePtr pNode;

      vector <APTRec> vecAPTRec;
      pNode = pGSMDaCharTable->nextNode();
      unsigned short rf_mode;
      byte chain;
      byte instance;
      RFCal_Helper_Read_Attribute(pNode, "rf_mode", rf_mode);
      RFCal_Helper_Read_Attribute(pNode, "chain", chain);
      RFCal_Helper_Read_Attribute(pNode, "instance", instance);
      MSXML2::IXMLDOMNodeListPtr pGSMChanModPaTable = pNode->selectNodes((_bstr_t)"GSMChanModPa");
      QMSL_GSM_DA_Cal_Result nv_cal_result;
      QMSL_GSM_Tx_Calibration_NV    writeGsmNV;
      memset(&nv_cal_result,0,sizeof(QMSL_GSM_DA_Cal_Result));
      memset(&writeGsmNV,0,sizeof(QMSL_GSM_Tx_Calibration_NV));

      map <UINT, UINT> chanToIndex;

      map <UINT, UINT> paToIndex;
      unsigned int packetSize = sizeof(QMSL_GSM_Tx_Cal_NV_Header_Type) + sizeof(QMSL_Variant_Marker_Type);
      if (pGSMChanModPaTable)
      {
         for (int j= 0; j < pGSMChanModPaTable->length; j++)
         {
            MSXML2::IXMLDOMNodePtr pNode = pGSMChanModPaTable->nextNode();
            UINT channel;
            UINT mod;
            UINT pa;
            RFCal_Helper_Read_Attribute(pNode, "channel", channel);

            if (channel == 975 || channel == 37 || channel == 124)
            {
               // EGSM only
               chanToIndex.insert(make_pair(975,0));
               chanToIndex.insert(make_pair(37,1));
               chanToIndex.insert(make_pair(124,2));
            }
            RFCal_Helper_Read_Attribute(pNode, "mod", mod);
            RFCal_Helper_Read_Attribute(pNode, "pa", pa);

            UINT size = chanToIndex.size();
            chanToIndex.insert(make_pair(channel,size) );
            byte chanIndex = chanToIndex[channel];

            UINT paSize = paToIndex.size();
            paToIndex.insert(make_pair(pa,paSize) );
            byte paIndex = paToIndex[pa];

            nv_cal_result.iChannel[chanIndex] = static_cast<unsigned short>(channel);
            nv_cal_result.iFreqMapping[chanIndex] = static_cast<QMSL_GSM_Polar_Cal_Frequency >(chanIndex + 1);

            MSXML2::IXMLDOMNodeListPtr pGSMRgiPwrTable = pNode->selectNodes((_bstr_t)"GSMRgiPwr");
            if (pGSMRgiPwrTable)
            {
               UINT index = 0;

               for (int k= 0; k < pGSMRgiPwrTable->length; k++)
               {
                  MSXML2::IXMLDOMNodePtr pNode = pGSMRgiPwrTable->nextNode();
                  int rgi;
                  double pwr;
                  RFCal_Helper_Read_Attribute(pNode, "rgi", rgi);
                  RFCal_Helper_Read_Attribute(pNode, "pwr", pwr);

                  if (mod == 0)
                  {
                     nv_cal_result.aiGSMTxPowerMeas[chanIndex][paIndex][index] = pwr;
                     nv_cal_result.aiGSMTxRGI[chanIndex][paIndex][index] = rgi;
                     index++;
                  }
                  else
                  {
                     nv_cal_result.aiEDGETxPowerMeas[chanIndex][paIndex][index] = pwr;
                     nv_cal_result.aiEDGETxRGI[chanIndex][paIndex][index] = rgi;
                     index++;
                  }
               }
            }
         }
         packetSize += ( sizeof(QMSL_GSM_PMEAS_List_Variant_Type)*paToIndex.size()*reqNumMod*chanToIndex.size());
         packetSize += ( sizeof(QMSL_GSM_RGI_List_Variant_Type)*paToIndex.size()*reqNumMod);
      }

      nv_cal_result.iNumChannels = chanToIndex.size();
      nv_cal_result.iNumMeasuredTxGainStates = paToIndex.size(); //
      nv_cal_result.iNumTxPowerMeasurementsPerRange = 32; //
      nv_cal_result.iSaturationDetectionWindow = 0; //

      for (map<UINT,UINT>::iterator it = paToIndex.begin(); it != paToIndex.end(); it++)
      {
         nv_cal_result.aiTxMeasToNVMapping[it->second]  = it->first;
      }
      QLIB_RFCal_GSM_Tx_DA_Cal_Generate_NV( hPhone, rf_mode,chain,&nv_cal_result,&writeGsmNV,cacheNVItems);
      unsigned short writeStatus;
      QLIB_FTM_CALL_RFNV_WRITE_V2( hPhone, GetGsmNvID(rf_mode,chain), writeGsmNV.oNVitem.aRFNVItemData, static_cast<unsigned short>(packetSize), GSM_V4_MAX_PACKET_SIZE,&writeStatus );
   }
   pGSMDaCharTable.Release();
   spXMLDoc.Release();

   CoUninitialize();
   return true;
}
unsigned short GetGsmNvID( unsigned short rfMode, unsigned short chainID)
{
   unsigned short nvID = 0;

   switch(rfMode)
   {
      case NV_MODE_GSM_850:
         nvID = RFNV_GSM_C0_GSM850_TX_CAL_DATA_I;
         if (chainID == 2)
         {
            nvID = RFNV_GSM_C2_GSM850_TX_CAL_DATA_I;
         }
         break;
      case NV_MODE_GSM_EGSM:
         nvID = RFNV_GSM_C0_GSM900_TX_CAL_DATA_I;
         if (chainID == 2)
         {
            nvID = RFNV_GSM_C2_GSM900_TX_CAL_DATA_I;
         }
         break;
      case NV_MODE_GSM_DCS:
         nvID = RFNV_GSM_C0_GSM1800_TX_CAL_DATA_I;
         if (chainID == 2)
         {
            nvID = RFNV_GSM_C2_GSM1800_TX_CAL_DATA_I;
         }
         break;
      case NV_MODE_GSM_1900:
         nvID = RFNV_GSM_C0_GSM1900_TX_CAL_DATA_I;
         if (chainID == 2)
         {
            nvID = RFNV_GSM_C2_GSM1900_TX_CAL_DATA_I;
         }
         break;
      default:
         break;
   }

   return nvID;
}
unsigned char Test_FTM_DIVERSITY_CAL( void )
{
   unsigned char bOk=true;
   unsigned char cKey;

   printf("\n----------------\nFTM Diversity CAL API\n----------------\n");

   if ( bOk )
   {
      printf("\n\n1=Set RF Mode CDMA 800");
      printf("\n2=Set Channel 384");
      printf("\n3=GET DVGA diversity, -80dBm, ");
      printf("\n4=GET LNA diversity, offset 1, -80dBm");
      printf("\n5=GET IM2 diversity");
      printf("\n6=GET IM2 with suppression diversity");
      printf("\n7=Get CtoN diversity ");
      printf("\n8=Get Rx AGC  diversity");
      cKey = getch();

      if ( cKey == '1')
      {
         // Set CDMA1X BC0 mode, this must be done first for the primary
         unsigned short modeId = PHONE_MODE_CDMA_800;

         bOk = QLIB_FTM_SET_MODE(g_hResourceContext, PHONE_MODE_CDMA_800);
         printf("\nQLIB_FTM_SET_MODE(): %s, for primary", bOk ? "PASS" : "FAIL" );

         // Activate the diversity path, only need to do this one time
         unsigned short chainMode = 1; // Diversity Mode
         bOk = QLIB_FTM_SET_SECONDARY_CHAIN( g_hResourceContext, chainMode);
         printf("\nQLIB_FTM_SET_SECONDARY_CHAIN: Mode %d", chainMode);

         // This must be done so that subsequent RF commands will operate on the diversity
         // path [ including QLIB_FTM_SET_MODE()  ]
         unsigned short modeID = FTM_MODE_ID_CDMA_1X_RX1;
         bOk = QLIB_FTM_SET_MODE_ID(g_hResourceContext, modeID );
         printf("\nQLIB_FTM_SET_MODE_ID: Mode %d (1x Diversity)", modeID);

         // Initialize the RF mode for the diversity path, this only needs to be done once.
         bOk = QLIB_FTM_SET_MODE( g_hResourceContext, modeId);  // set cdma 800 mode
         printf("\nQLIB_FTM_SET_MODE(RF Mode): MODE: %d, %s", modeId, bOk ? "PASS" : "FAIL\n");
      }
      if ( cKey == '2')
      {
         unsigned short channel = 384;
         bOk = QLIB_FTM_SET_CHAN( g_hResourceContext, channel); // Set channel
         printf("\nQLIB_FTM_SET_CHAN_DIVERSITY: Channel%d %s ", channel , bOk ? "PASS" : "FAIL");
      }
      if (cKey == '3' )
      {
         unsigned short RxAGC = - 512 + 10 *(-80 + 115) ; // -80dBm
         short DVGA = 0;
         bOk = QLIB_FTM_GET_DVGA_OFFSET( g_hResourceContext, RxAGC, &DVGA);
         printf("\nQLIB_FTM_GET_DVGA_OFFSET: %s DVGA %d", bOk ? "PASS" : "FAIL\n", DVGA );
      }
      if (cKey == '4' )
      {
         unsigned short RxAGC = - 512 + 10 *(-80 + 115) ; // -80dBm
         short LNA = 1;
         bOk = QLIB_FTM_GET_LNA_OFFSET( g_hResourceContext, 0, RxAGC, &LNA);
         printf("\nQLIB_FTM_GET_LNA_OFFSET: %s LNA %d", bOk ? "PASS" : "FAIL\n", LNA );
      }
      if (cKey == '5' )
      {
         unsigned char I, Q, TransconductanceValue = 0;
         bOk = QLIB_FTM_GET_CDMA_IM2( g_hResourceContext, &I, &Q, &TransconductanceValue);
         printf("\nQLIB_FTM_GET_CDMA_IM2: %s I %d, Q %d, Trans %d", bOk ? "PASS" : "FAIL\n",
                I, Q, TransconductanceValue );
      }
      if (cKey == '6' )
      {
         unsigned char I, Q, TransconductanceValue;
         unsigned short suppression = 0;
         bOk = QLIB_FTM_GET_CDMA_IM2_WITH_SUPPRESSSION( g_hResourceContext, &I, &Q,
                                                        &TransconductanceValue, &suppression);
         printf("\nQLIB_FTM_GET_CDMA_IM2_WITH_SUPPRESSSION: %s I %d, Q %d, Trans %d suppresion",
                bOk ? "PASS" : "FAIL\n",
                I, Q, TransconductanceValue, suppression );
      }
      if ( cKey == '7')
      {
         long iCtoN = 0;
         bOk = QLIB_FTM_GET_CTON(g_hResourceContext, &iCtoN);
         printf("\nQLIB_FTM_GET_CTON: %d", iCtoN);
      }
      if (cKey == '8' )
      {
         unsigned short RxAGC = 0;
         bOk = QLIB_FTM_GET_RX_AGC( g_hResourceContext, &RxAGC);
         printf("\nQLIB_FTM_GET_RX_AGC: %s RX AGC %d", bOk ? "PASS" : "FAIL\n", RxAGC );
      }
   }
   return 0;
}
unsigned char Test_FTM_cdma2000( )
{
   unsigned char bOk = true;
   int iChannel =384;
   unsigned short iADC_Value;
   unsigned short iADC_Channel = 5;
   unsigned short iRxAGC = 0;
   unsigned char iSynthLock = 0;
   short iExpectedRX_AGC = 0;
   short iDVGAoffset;
   short iLNAoffsetValue = 0;
   unsigned short iHDETvalue = 0;
   unsigned char  cKey = ' ';
   short iRxAGC_Signed  = 0;
   double _dRxPowerLevels[] = {-73.0, -69.0, -48.0,  -32.0 };
   short _iRxAgcLevels[]    = {-115,  -67,   184,    376   };
   short iMode = PHONE_MODE_CDMA_800 ;

   printf("----------------\ncdma2000 FTM Test\n----------------\n");

   if ( bOk )
   {
      // RF Mode

      /*--for PCS
      iChannel = 9262;
      iMode = PHONE_MODE_WCDMA_1900B;
      */

      if ( !QLIB_FTM_SET_MODE( g_hResourceContext, iMode ))
      {
         bOk = false;
         printf("\nQLIB_FTM_SET_MODE failed\n");
      }

      // Set the frequency channel
      if ( !QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel))
      {
         bOk = false;
         printf("\nQLIB_FTM_SET_CHAN failed\n");
      }

      // Verify synths are locked
      if ( !QLIB_FTM_GET_SYNTH_STATE( g_hResourceContext, &iSynthLock ))
      {
         bOk = false;
         printf("\nQLIB_FTM_GET_SYNTH_STATE failed\n");
      }

      printf("\nSynth State: %s", ( iSynthLock==1 ? "Locked" : "Unlocked" ) );

      printf("\ncdma2000 is on channel %d", iChannel);

      printf("\n\n1=Tx RF");
      printf("\n2=Rx");
      printf("\n3=Tx Rx Frequency Calibration");
      printf("\n4=Test Secondary Rx Chain Sensivity with CtoN");

      cKey = getch();

      if (cKey == '1' )
      {
         // Set Tx AGC PDM to 300
         bOk &= QLIB_FTM_SET_PDM(g_hResourceContext, 2, 300);

         // PA Range
         bOk &= QLIB_FTM_SET_PA_RANGE( g_hResourceContext, 0 );

         // Set Tx On
         bOk &= QLIB_FTM_SET_TX_ON( g_hResourceContext );

         bOk &= QLIB_FTM_CDMA_CW_WAVEFORM( g_hResourceContext, 0 );

         if ( !bOk )
         {
            printf("Error occurred setting up transmitter.");
         }
         else
         {
            // Get a key
            printf("\ncdma2000 Tx is on press any key to continue.\n" );
            getch();

            bOk &= QLIB_FTM_CDMA_CW_WAVEFORM( g_hResourceContext, 1 );
            printf("\nCW Tx is on channel %d, press any key to continue.\n", iChannel);
            getch();

            // Read ADC on channel 5
            bOk &= QLIB_FTM_GET_ADC_VAL( g_hResourceContext, iADC_Channel, &iADC_Value );
            printf("\nADC on Channel %d is %d", iADC_Channel, iADC_Value );

            // Do the Tx Sweep cal
            printf("\nPress any key for Tx Sweep cal.\n" );
            getch();
            bOk &= QLIB_FTM_TX_SWEEP_CAL( g_hResourceContext, 1 );      // On
            bOk &= QLIB_FTM_TX_SWEEP_CAL( g_hResourceContext, 0 );      // Off

            // Read the first HDET value
            bOk &= QLIB_FTM_GET_HDET_FROM_TX_SWEEP_CAL( g_hResourceContext, 0, &iHDETvalue );
            printf("\nTx Sweep cal HDET[0] = % d\n", iHDETvalue );

            // Set Tx On
            bOk &= QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         } // !bOk
      }// cKey == 1
      else if (cKey == '2')
      {
         double _dRxPowerLevels[] = {-73.0, -69.0, -48.0,  -32.0 };
         short _iRxAgcLevels[]    = {-115,  -67,   184,    376   };

         // Get the DVGA Offset
         printf("\nSet DVGA Power (%6.2fdBm) then press any key to continue.\n", _dRxPowerLevels[0], iChannel);
         getch();
         iExpectedRX_AGC = _iRxAgcLevels[0];
         bOk &= QLIB_FTM_GET_DVGA_OFFSET( g_hResourceContext, iExpectedRX_AGC, &iDVGAoffset );
         printf("\nDVGA Offset: %d, FTM expected parameter: %d", iDVGAoffset,iExpectedRX_AGC );

         // Get the LNA offset
         for (int iLnaRange = 1; iLnaRange <=3; iLnaRange++)
         {
            printf("\n\nSet DL power (%6.2fdBm) for LNA range %d then press any key to continue.", _dRxPowerLevels[iLnaRange ],iLnaRange );
            getch();
            bOk &= QLIB_FTM_GET_LNA_OFFSET( g_hResourceContext, iLnaRange, _iRxAgcLevels[iLnaRange], &iLNAoffsetValue );
            printf("\nLNA %d Offset: %d", iLnaRange, iLNAoffsetValue  );
         }

         // Get the RX AGC Value
         QLIB_FTM_GET_RX_AGC( g_hResourceContext, &iRxAGC );

         // Convert to a signed number

         iRxAGC_Signed = iRxAGC;

         // Check for sign bit
         if ( iRxAGC  & 0x200 )
         {
            // convert from 2's comp
            iRxAGC_Signed &= 0x1ff;
            iRxAGC_Signed = iRxAGC_Signed - 512;
         }

         printf("\n  Measured Rx AGC value (signed): %d", iRxAGC_Signed );
         printf("\nMeasured Rx AGC value (unsigned): %d", iRxAGC );
      } // cKey == 2
      else if (cKey == '3' )
      {
         printf("\nStarting TX RX Frequency Sweep...Use QMSL text log to view results, include 0x08 in the log level");

         // Create the param/result  structure
         FTM_Tx_Rx_Freq_Cal_Sweep_Request_Response _oParams;

         /*
         Adapted from the FTM GUI file, tx_rx_freq_cal_params_1x_cell.txt.  File contents:
         16
         5
         655
         1018,46,98,150,202,254,306,358,410,462,514,566,618,670,722,774
         60,107,125,125, 511
         -92,64,251,405, 0
         -92,64,251,405, 0
         0, 2, 2, 2, 255
         0, 1, 0, 0, 0
         100, 1, 2, 3, 255
         */

         const int c_iNumChannels = 16;
         unsigned short _aiChannelList[ c_iNumChannels]  = { 1018,46,98,150,202,254,306,358,410,462,514,566,618,670,722,774};

         // Maximum segments is 6, but for this example, will use 5
         const int c_iNumSteps = 15;
         unsigned short _aiTxPDM_List[ c_iNumSteps ]    = {83, 89, 105, 103, 100, 97, 95, 92, 89, 86, 83, 83, 83, 83, 83 };
         short          _aiPrimaryExpectedAGC_ValList[ c_iNumSteps ] = {-162,-162,-162,-62, -62, -62,137,137,137,237,237,237,337,337,337 };
         unsigned char  _aiPA_RangeList[ c_iNumSteps ]  = {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 255};
         unsigned char  _aiReadHDET_List[ c_iNumSteps ] = {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 };
         unsigned char  _aiRxActionList[ c_iNumSteps ]  = {100, 100,100, 1, 1, 1, 2, 2, 2, 3, 3, 3, 255,255, 255 };

         // These fields apply to all of the setups above.
         unsigned char  _iRxChain = FTM_TX_RX_FREQ_RX_CHAIN_0;

         unsigned char  _aiRxPowerMode[ c_iNumSteps ]   = { FTM_TX_RX_FREQ_RX_PM_High,
                                                            FTM_TX_RX_FREQ_RX_PM_Medium,
                                                            FTM_TX_RX_FREQ_RX_PM_Low,
                                                            FTM_TX_RX_FREQ_RX_PM_High,
                                                            FTM_TX_RX_FREQ_RX_PM_Medium,
                                                            FTM_TX_RX_FREQ_RX_PM_Low,
                                                            FTM_TX_RX_FREQ_RX_PM_High,
                                                            FTM_TX_RX_FREQ_RX_PM_Medium,
                                                            FTM_TX_RX_FREQ_RX_PM_Low,
                                                            FTM_TX_RX_FREQ_RX_PM_High,
                                                            FTM_TX_RX_FREQ_RX_PM_Medium,
                                                            FTM_TX_RX_FREQ_RX_PM_Low,
                                                            FTM_TX_RX_FREQ_RX_PM_Low,
                                                            FTM_TX_RX_FREQ_RX_PM_Low,
                                                            FTM_TX_RX_FREQ_RX_PM_Low
                                                          };

         int _iIndex = 0;

         //
         // Copy data into the request structure
         //

         // Fill the basic parameters
         _oParams.iRxChain = _iRxChain;
         _oParams.iNumFreqs = c_iNumChannels;
         _oParams.iNumSteps = c_iNumSteps;
         _oParams.iStepLength = 655;

         // aiChannelList
         for ( _iIndex = 0; _iIndex < c_iNumChannels; _iIndex++ )
            _oParams.aiChannelList[ _iIndex ]  = _aiChannelList[ _iIndex ];

         // _iNumSteps
         for ( _iIndex = 0; _iIndex < c_iNumSteps; _iIndex++ )
            _oParams.aiTxPDM_List[ _iIndex ] = _aiTxPDM_List[ _iIndex ];

         // aiPrimaryExpectedAGC_ValList
         for ( _iIndex = 0; _iIndex < c_iNumSteps; _iIndex++ )
            _oParams.aiExpectedRxAGC_ValList[ _iIndex ] = _aiPrimaryExpectedAGC_ValList[ _iIndex ];

         //aiPA_RangeList
         for ( _iIndex = 0; _iIndex < c_iNumSteps; _iIndex++ )
            _oParams.aiPA_RangeList[ _iIndex ] = _aiPA_RangeList[ _iIndex ];

         // aiPrimaryExpectedAGC_ValList
         for ( _iIndex = 0; _iIndex < c_iNumSteps; _iIndex++ )
            _oParams.aiReadHDET_List[ _iIndex ] = _aiReadHDET_List[ _iIndex ];

         // aiRxActionList
         for ( _iIndex = 0; _iIndex < c_iNumSteps; _iIndex++ )
            _oParams.aiRxActionList[ _iIndex ] = _aiRxActionList[ _iIndex ];

         // aiRxPowerMode
         for ( _iIndex = 0; _iIndex < c_iNumSteps; _iIndex++ )
            _oParams.aiRxPowerMode[ _iIndex ] = _aiRxPowerMode[ _iIndex ];

         // Call the frequency sweep function, cast the parameters as void pointer
         bOk = QLIB_FTM_TX_RX_FREQ_CAL_SWEEP( g_hResourceContext, (void*) &_oParams );
         printf("\nQLIB_FTM_TX_RX_FREQ_CAL_SWEEP: %s", (bOk ? "PASS" : "FAIL") );
      }// cKey == 3
      else if (cKey == '4' )
      {
         unsigned char bOk;

         // Set the packet mode id field to CDMA primary
         // No command is sent to the mobile
         short eShortId = FTM_MODE_ID_CDMA_1X;
         bOk = QLIB_FTM_SET_MODE_ID(g_hResourceContext, eShortId );
         printf("\nQLIB_FTM_SET_MODE_ID(%d) %s", eShortId,  bOk ? "PASS" : "FAIL\n");

         // Set primary chain to CDMA800 mode
         short eModeId = PHONE_MODE_CDMA_800;
         bOk = QLIB_FTM_SET_MODE( g_hResourceContext, eModeId );
         printf("\nQLIB_FTM_SET_MODE(%d) %s", eModeId,  bOk ? "PASS" : "FAIL\n");

         // 2 = Off frequency search
         short eSecondaryChainMode = 2;
         bOk = QLIB_FTM_SET_SECONDARY_CHAIN( g_hResourceContext, eSecondaryChainMode );
         printf("\nQLIB_FTM_SET_SECONDARY_CHAIN(%d) %s", eSecondaryChainMode,  bOk ? "PASS" : "FAIL\n");

         // Set the packet mode id field to CDMA RX1 (secondary chain)
         eShortId = FTM_MODE_ID_CDMA_1X_RX1;
         bOk = QLIB_FTM_SET_MODE_ID(g_hResourceContext, eShortId );
         printf("\nQLIB_FTM_SET_MODE_ID(%d) %s", eShortId,  bOk ? "PASS" : "FAIL\n");

         // Set secondary chain to CDMA800mode
         eModeId = PHONE_MODE_CDMA_800;
         bOk = QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_CDMA_800 );
         printf("\nQLIB_FTM_SET_MODE(%d) %s", eModeId,  bOk ? "PASS" : "FAIL\n");

         // Set 2nd chain to channel 387
         unsigned short iChannel = 387;
         bOk = QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel);
         printf("\nQLIB_FTM_SET_CHAN(%d) %s", iChannel,  bOk ? "PASS" : "FAIL\n");

         // Set 2nd chain LNA to highest gain state
         unsigned char iLNARange = 0;
         bOk = QLIB_FTM_SET_LNA_RANGE( g_hResourceContext, iLNARange);
         printf("\nQLIB_FTM_SET_LNA_RANGE(%d) %s", iLNARange,  bOk ? "PASS" : "FAIL\n");

         /*
         Set base station power
         sGPIBcmd = "call:rfgenerator:frequency 881.71e6";
         ibwrt(h_Inst, sGPIBcmd.GetBuffer(0), sGPIBcmd.GetLength());
         //  8960 BC0 channel 387 + 100kHz CW

         sGPIBcmd = "call:power -80";
         ibwrt(h_Inst, sGPIBcmd.GetBuffer(0), sGPIBcmd.GetLength());
         // -80 dBm

         Sleep (100);
         // Let the power settle
         */
         printf("Set base station emulator at Ch%d + 100kHz at -80dBm\n", iChannel);
         printf("Press any key to continue\n");
         getch();

         long _dCtoN = 0;
         bOk = QLIB_FTM_GET_CTON( g_hResourceContext, &_dCtoN);
         printf("\nQLIB_FTM_GET_CTON() %s",  bOk ? "PASS" : "FAIL\n");
         printf("Note: The value below is 10 * (C/N) dB\n");
         printf("BC0 C/N is %d\n", _dCtoN );

         /*======= PCS example =====================
         // Set secondary chain to CDMA1900 mode
         eModeId = PHONE_MODE_CDMA_1900;
         bOk = QLIB_FTM_SET_MODE( g_hResourceContext, eModeId );
         printf("\nQLIB_FTM_SET_MODE(%d) %s", eModeId,  bOk ? "PASS" : "FAIL\n");

         // Set 2nd chain to channel 500
         iChannel = 500;
         bOk = QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel);
         printf("\nQLIB_FTM_SET_CHAN(%d) %s", iChannel,  bOk ? "PASS" : "FAIL\n");

         // Set 2nd chain LNA to highest gain state
         iLNARange = 0;
         bOk = QLIB_FTM_SET_LNA_RANGE( g_hResourceContext, iLNARange);
         printf("\nQLIB_FTM_SET_LNA_RANGE(%d) %s", iLNARange,  bOk ? "PASS" : "FAIL\n");

         /*  Set base station power
         sGPIBcmd = "call:rfgenerator:frequency 1955.1e6";
         ibwrt(h_Inst, sGPIBcmd.GetBuffer(0), sGPIBcmd.GetLength());
         //  8960 BC1 channel 500 + 100kHz CW

         sGPIBcmd = "call:power -80";
         ibwrt(h_Inst, sGPIBcmd.GetBuffer(0), sGPIBcmd.GetLength());
         // -80 dBm

         Sleep (100); // Let the power settle
         */

         /*========= PCS example continue =================
         printf("Set base station emulator at Ch%d + 100kHz at -80dBm\n", iChannel);
         printf("Press any key to continue\n");
         getch();

         bOk = QLIB_FTM_GET_CTON( g_hResourceContext, &_dCtoN);
         printf("\nQLIB_FTM_GET_CTON() %s",  bOk ? "PASS" : "FAIL\n");
         // Get C/N note: The value returned is 10 * (C/N) dB
         printf("PCS C/N is %d\n", _dCtoN );
         /*========= End of PCS example continue =================*/
      } // cKey == 4
   }

   return bOk;
}
unsigned char Test_FTM_GSM( void )
{
   unsigned short iLNAindex = 1;
   unsigned long iRSSI = 0;
   unsigned short iChannel = 1;
   unsigned char bOk = true;
   unsigned short iStartDAC = 400;
   unsigned char iPowerLevelIndex = 0;
   unsigned char iLNArange = 0;
   char cKey = ' ';

   printf("----------------\nGSM FTM Test\n----------------\n");

   // RF Mode, GSM900
   bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_GSM_900 );
   printf("\nRF FTM Mode = GSM 900");

   // Set the frequency channel
   bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel);
   printf("\nARFCN = %d", iChannel);
   printf("\nNote:  for ARFCN1, DL = 935.2MHz, UL = 890.2MHz");

   if ( bOk )
   {
      // Select Rx or Tx
      printf("\n\n\n1 = Tx");
      printf("\n2 = Rx");
      printf("\n3 = QLIB_FTM_SET_PATH_DELAY");
      printf("\n4 = QLIB_FTM_SET_TWOTONE_FREQ_OFFSET");
      printf("\n5 = QLIB_FTM_SET_OPLL_BW");
      printf("\n6 = QLIB_FTM_BASEBAND_BW_CAL");
      printf("\n7 = QLIB_FTM_GET_RSSI");
      printf("\n8 = QLIB_FTM_DO_GSM_AUTOCAL");
      printf("\n9 = QLIB_FTM_GSM_POLAR_TX_RSB_DC_SWEEP");
      printf("\nA = QLIB_FTM_RX_GAIN_RANGE_CAL");
      printf("\nB = QLIB_FTM_GSM_POLAR_TX_SWEEP_CAL");
      printf("\nC = QLIB_FTM_GSM_TX_4X_FMOD_SWEEP");
      printf("\nD = QLIB_FTM_SET_TX_FRAME_MATRIX ");

      cKey = toupper( getch() );

      if (cKey == '1' )
      {
         // Set the DAC level
         bOk &= QLIB_FTM_SET_PA_DAC_INPUT( g_hResourceContext, iStartDAC );
         // bOk &= QLIB_FTM_SET_TX_POWER_LEVEL( g_hResourceContext, 14 );

         // Setup the Tx burst
         bOk &= QLIB_FTM_SET_TRANSMIT_BURST
                (
                   g_hResourceContext,
                   0,  // iSlotNum = 0
                   0,  // iDataSource = 0 - TX_DATA_SOURCE_PSDRND
                   0,  // iTSCindex = 0
                   0,  // iNumBursts = 0
                   1 // bIsInfiniteDuration = 1
                );
         // Set Tx On
         bOk &= QLIB_FTM_SET_TX_ON( g_hResourceContext );

         printf("\nTX setup: %s", (bOk ? "PASS" : "FAIL") );

         printf("\n>>Observe Tx, then press any key for DAC change to 0<<");
         getch();

         bOk &= QLIB_FTM_SET_PA_DAC_INPUT( g_hResourceContext, iStartDAC );

         // Loop through a range of DAC values
         for ( iPowerLevelIndex = 0; iPowerLevelIndex <=14; iPowerLevelIndex++)
         {
            // Set the power control index
            bOk &= QLIB_FTM_SET_TX_POWER_LEVEL(g_hResourceContext, iPowerLevelIndex );

            printf(" \nPower Index= %d...press a key...", iPowerLevelIndex );
            getch();
         }

         // Set Tx Off
         bOk &= QLIB_FTM_SET_TX_OFF( g_hResourceContext );
      } // cKey = '1'
      else if (cKey == '2')
      {
         // RF Mode, GSM900
         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_GSM_900 );
         printf("\nRF FTM Mode = GSM 900");

         // Set the frequency channel
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel);
         printf("\nARFCN = %d", iChannel);
         printf("\nNote:  for ARFCN1, DL = 935.2MHz, UL = 890.2MHz");

         // Setup for Rx continuous, with RSSI report
         bOk &= QLIB_FTM_SET_RX_BURST
                (
                   g_hResourceContext,
                   0,      // iSlotNum = 0
                   0       // iRxFunction = 0  - RX_FUNCTION_GET_RSSI
                );

         // Test LNA states
         if ( bOk )
         {
            // Reset the OK flag
            bOk = true;

            for (iLNArange = 0; iLNArange <=3; iLNArange++)
            {
               // Set the LNA range--the = instead of &= is intentional
               bOk = QLIB_FTM_SET_LNA_RANGE( g_hResourceContext, iLNArange );

               if (!bOk )
                  printf("\nQLIB_FTM_SET_LNA_RANGE %d failed.", iLNArange);

               // Change through LNA states
               // Measure RSSI
               bOk &= QLIB_FTM_GET_RSSI( g_hResourceContext, &iRSSI );

               // print report
               printf("\nLNA Range %d, RSSI=%d, Status = %s", iLNArange, iRSSI, bOk ? "PASS" : "FAIL" );
            } // for( iLNArage...)

            // Stop Rx Rx continuous
            bOk &= QLIB_FTM_SET_RX_BURST
                   (
                      g_hResourceContext,
                      0,       // iSlotNum = 0
                      2        // iRxFunction = 2  - RX_FUNCTION_STOP_CONTINOUS_RX
                   );
            if (!bOk)
               printf("\nErrors occurred while setting up GSM Rx operations on LNA state %d.", iLNArange);
         }
         else
         {
            printf("\nQLIB_FTM_SET_RX_BURST failed.");
         }  // bOk

         printf("\n");
      } // cKey == '2'
      else if (cKey == '3')
      {
         // printf("\n3 = QLIB_FTM_SET_PATH_DELAY");
         short _iDelay_user = 0;
         printf("\n\nPlease enter path delay: ");
         scanf("%hd", &_iDelay_user );

         short _iDelay_output = _iDelay_user;

         // The first send uses version 1 of the API, which uses the same variable to hold both
         // input and output data
         bOk = QLIB_FTM_SET_PATH_DELAY( g_hResourceContext, &_iDelay_output );
         printf("\nQLIB_FTM_SET_PATH_DELAY( iDelay_output = %d) with iDelay_input = %d: %s", _iDelay_output, _iDelay_user, bOk ? "PASS" : "FAIL" );

         // The send uses version 2 of the API, which uses different variables to hold
         // the input and output data
         _iDelay_output = 0;
         bOk = QLIB_FTM_SET_PATH_DELAY_V2( g_hResourceContext, _iDelay_user, &_iDelay_output );
         printf("\nQLIB_FTM_SET_PATH_DELAY_V2(%d): %s", _iDelay_output, bOk ? "PASS" : "FAIL" );
      } // cKey == '3'
      else if (cKey == '4')
      {
         // printf("\n4 = QLIB_FTM_SET_TWOTONE_FREQ_OFFSET, 100kHz");

         // Set Tx Off
         bOk &= QLIB_FTM_SET_TX_OFF( g_hResourceContext );

         unsigned short _iOffset_kHz = 0;
         printf("\n\nPlease enter frequency offset (kHz): ");

         scanf("%hd", &_iOffset_kHz );

         bOk = QLIB_FTM_SET_TWOTONE_FREQ_OFFSET( g_hResourceContext, _iOffset_kHz );

         printf("\nQLIB_FTM_SET_TWOTONE_FREQ_OFFSET(%d): %s", _iOffset_kHz, bOk ? "PASS" : "FAIL" );

         /*
         // Setup data source as 2-tone buffer

         */
         bOk = QLIB_FTM_SET_TRANSMIT_CONT( g_hResourceContext, 1, FTM_GSM_TX_DATA_SOURCE_TWOTONE, 0,0, 1 );
         printf("\nQLIB_FTM_SET_TRANSMIT_CONT(): %s", bOk ? "PASS" : "FAIL" );

         // Set Tx On
         bOk &= QLIB_FTM_SET_TX_ON( g_hResourceContext );
      } // cKey == '4'
      else if (cKey == '5')
      {
         // printf("\n5 = QLIB_FTM_SET_OPLL_BW");

         unsigned char _iOPLL_BW = 0;
         unsigned short _iOPLL_BW_short = 0;
         printf("\n\nPlease enter OPLL BW Setting: ");

         scanf("%hd", &_iOPLL_BW_short );

         _iOPLL_BW = (unsigned char) _iOPLL_BW_short;

         bOk = QLIB_FTM_SET_OPLL_BW( g_hResourceContext, _iOPLL_BW );

         printf("\nQLIB_FTM_SET_OPLL_BW(%d): %s", _iOPLL_BW, bOk ? "PASS" : "FAIL" );
      } // cKey == '5'
      else if (cKey == '6')
      {
         // printf("\n6 = QLIB_FTM_BASEBAND_BW_CAL");

         unsigned char _iBaseband_BW = 0;
         unsigned char _iBaseband_BW_short = 0;
         printf("\n\nPlease enter Baseband BW Setting: ");

         scanf("%d", &_iBaseband_BW_short );
         _iBaseband_BW = (unsigned char) _iBaseband_BW_short;

         bOk = QLIB_FTM_BASEBAND_BW_CAL( g_hResourceContext, _iBaseband_BW );

         printf("\nQLIB_FTM_BASEBAND_BW_CAL(%d): %s", _iBaseband_BW, bOk ? "PASS" : "FAIL" );
      } // cKey == '6'
      else if (cKey == '7')
      {
         // printf("\n7 = QLIB_FTM_GET_RSSI");

         bOk = true;

         // RF Mode, GSM900
         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_GSM_900 );
         printf("\nRF FTM Mode = GSM 900");

         // Set the frequency channel
         iChannel = 10;

         unsigned long _iRSSI = 0;

         while ( !kbhit() && bOk )
         {
            bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel);
            printf("\nARFCN = %d", iChannel);
            printf("\nNote:  for ARFCN1, DL = 935.2MHz, UL = 890.2MHz");

            // Setup for Rx continuous, with RSSI report
            bOk &= QLIB_FTM_SET_RX_BURST
                   (
                      g_hResourceContext,
                      0,       // iSlotNum = 0
                      0        // iRxFunction = 0  - RX_FUNCTION_GET_RSSI
                   );

            bOk = QLIB_FTM_GET_RSSI( g_hResourceContext, &_iRSSI );

            printf("\nQLIB_FTM_GET_RSSI() = %s, RSSI =  %d", bOk ? "PASS" : "FAIL", _iRSSI );
         }

         // Stop Rx Rx continuous
         bOk &= QLIB_FTM_SET_RX_BURST
                (
                   g_hResourceContext,
                   0,      // iSlotNum = 0
                   2       // iRxFunction = 2  - RX_FUNCTION_STOP_CONTINOUS_RX
                );

         // Clear PC Keypress queue
         if ( kbhit()) getch();
      } // cKey == '7'
      else if (cKey == '8')
      {
         // printf("\n8 = QLIB_FTM_DO_GSM_AUTOCAL");

         unsigned short iPower          = 3250;
         unsigned short iPA_DAC_Value   = 12000;
         unsigned long iMax_Power       = 3250;
         unsigned long iMin_Power       = -2000;
         unsigned char iWriteNVFlag     = 0;
         unsigned short iRSB_Cor_A_R0   = 0;
         unsigned short iRSB_Cor_A_R1   = 0;
         unsigned short iRSB_Cor_B_R0   = 0;
         unsigned short iRSB_Cor_B_R1   = 0;

         bOk = QLIB_FTM_DO_GSM_AUTOCAL(
                  g_hResourceContext,
                  iPower,
                  iPA_DAC_Value,
                  iMax_Power,
                  iMin_Power,
                  iWriteNVFlag,
                  iRSB_Cor_A_R0,
                  iRSB_Cor_A_R1,
                  iRSB_Cor_B_R0,
                  iRSB_Cor_B_R1);

         printf("\nQLIB_FTM_DO_GSM_AUTOCAL: %s", bOk ? "PASS" : "FAIL" );

         if ( bOk )
         {
            // Return values
            unsigned short iNumPoints;
            unsigned short aiDAC_Values[ FTM_GSM_TX_POLAR_INTERNAL_AUTOCAL_MAX_DATA_ROWS ];
            float afAMAM_Values[ FTM_GSM_TX_POLAR_INTERNAL_AUTOCAL_MAX_DATA_ROWS ];
            float afAMPM_Values[ FTM_GSM_TX_POLAR_INTERNAL_AUTOCAL_MAX_DATA_ROWS ];

            bOk = QLIB_FTM_DO_GSM_AUTOCAL_GetResults(
                     g_hResourceContext,
                     &iNumPoints,
                     aiDAC_Values,
                     afAMAM_Values,
                     afAMPM_Values
                  );

            printf("\nQLIB_FTM_DO_GSM_AUTOCAL_GetResults: %s", bOk ? "PASS" : "FAIL" );

            // Print the results
            printf("\n          Index\tDAC\tAMAM\tAMPM" );
            printf("\n          -----\t---\t----\t----" );
            for ( int _iIndex = 0; _iIndex < iNumPoints; _iIndex++ )
            {
               printf("\n          %d\t%d\t%6.3f\t%6.3f",
                      _iIndex,
                      aiDAC_Values [ _iIndex ],
                      afAMAM_Values[ _iIndex ],
                      afAMPM_Values[ _iIndex ]
                     );
            }   // for ( int _iIndex
         }  // if ( bOk )
      } // cKey == '8'
      else if (cKey == '9')
      {
         // printf("\n9 = QLIB_FTM_GSM_POLAR_TX_RSB_DC_SWEEP");

         // Setup parameters, using list size = 10, but usual list sizes are much larger
         const unsigned long c_iListSize = 10;

         // Create an array & specify data for each parameter.  The data in these arrays is not functional, but
         // it useful for analyzing the request packet construction
         short _aiAlpha[c_iListSize] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
         short _aiBeta[c_iListSize] = {-1, -2, -3, -4, -5, -6, -7, -8, -9, -10};
         char _aiI_DC_Correction[c_iListSize] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
         char _aiQ_DC_Correction[c_iListSize] = {-11, -12, -13, -14, -15, -16, -17, -18, -19, -20};
         unsigned short _aiSegmentLength[c_iListSize] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };
         unsigned short _iStatus = DIAG_FTM_STATUS_FAIL;

         // Set the pass/fail flag
         bOk = true;

         // RF Mode, GSM900
         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_GSM_900 );
         printf("\nQLIB_FTM_SET_MODE() = %s", bOk ? "PASS" : "FAIL" );

         // Set the channel
         iChannel = 10;
         bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel );
         printf("\nQLIB_FTM_SET_CHAN( ARFCN = %d) = %s", iChannel, bOk ? "PASS" : "FAIL" );

         if ( bOk )
         {
            // Call the function
            bOk = QLIB_FTM_TX_RSB_DC_SWEEP(
                     g_hResourceContext,
                     c_iListSize,
                     _aiAlpha,
                     _aiBeta,
                     _aiI_DC_Correction,
                     _aiQ_DC_Correction,
                     _aiSegmentLength,
                     &_iStatus
                  );

            printf("\nQLIB_FTM_TX_RSB_DC_SWEEP() = %s, status= %d", bOk ? "PASS" : "FAIL", _iStatus );
         } // if ( bOk )
      } // cKey == '9'
      else if (cKey == 'A')
      {
         // printf("\nA = QLIB_FTM_RX_GAIN_RANGE_CAL");

         // Rx Range bit mask
         unsigned char _iRxRangeBitMask =
            (
               FTM_GSM_RX_GAIN_RANGE_0
               |   FTM_GSM_RX_GAIN_RANGE_1
               |   FTM_GSM_RX_GAIN_RANGE_2
               |   FTM_GSM_RX_GAIN_RANGE_3
            );

         // Number of bursts to average over
         unsigned char _iNumAverages = 4;

         // Toggle flag
         unsigned char _bToggleRx = false;

         // Channel list size
         const int c_iChannelListSize = 4;

         // Channel list -- last one is used only to turn off Rx
         const unsigned short c_aiChannelList[ c_iChannelListSize ] = { 10, 32, 48, 10 };

         bOk = true;

         // RF Mode, GSM900
         bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_GSM_900 );
         printf("\nQLIB_FTM_SET_MODE() = %s", bOk ? "PASS" : "FAIL" );

         // RSSI results
         unsigned long _aiRSSI_Results[ FTM_GSM_RX_GAIN_RANGE_CAL_NUM_RETURN_VALUES ] = {0, 0, 0, 0, 0, 0, 0, 0 };

         // Loop through the channels
         for ( int _iChannelIndex = 0; _iChannelIndex < c_iChannelListSize; _iChannelIndex++ )
         {
            // Set the next channel
            bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, c_aiChannelList[ _iChannelIndex ] );
            printf("\nChannel %d/%d,  ARFCN = %d", _iChannelIndex, c_iChannelListSize, c_aiChannelList[ _iChannelIndex ] );

            // The first time the function is called, then toggle must be TRUE to turn on the Rx
            // The last time the function is called, the toggle should be TRUE again, to turn RX off
            _bToggleRx = (  ( _iChannelIndex == 0 )
                            ||   ( _iChannelIndex == c_iChannelListSize - 1 )
                         );

            // Clear RSSI array
            for ( int _iRSSI_Index = 0; _iRSSI_Index < FTM_GSM_RX_GAIN_RANGE_CAL_NUM_RETURN_VALUES; _iRSSI_Index++ )
               _aiRSSI_Results[ _iRSSI_Index ] = 0;

            // Call the calibration function
            bOk = QLIB_FTM_RX_GAIN_RANGE_CAL(
                     g_hResourceContext,
                     _iRxRangeBitMask,
                     _iNumAverages,
                     _bToggleRx,
                     _aiRSSI_Results
                  );
            // Display results

            printf("\nQLIB_FTM_RX_GAIN_RANGE_CAL() = %s, RSSI[0..3] =  {%d, %d, %d, %d}",
                   bOk ? "PASS" : "FAIL",
                   _aiRSSI_Results[0],
                   _aiRSSI_Results[1],
                   _aiRSSI_Results[2],
                   _aiRSSI_Results[3]
                  );
         }
      } // cKey == 'A'
      else if (cKey == 'B')
      {
         // printf("\nB = QLIB_FTM_GSM_POLAR_TX_SWEEP_CAL");

         unsigned long _iAmplitudeRef_DAC = 14500;
         unsigned long _iPhaseRef_DAC = 4500;

         const int c_iDAC_ListSize = 149;
         unsigned long _aiDAC_List[c_iDAC_ListSize] = {1800,1885,1970,2055,2140,2225,2310,2395,2480,2565,2650,2735,2820,2905,2990,3075,3160,3245,3330,3415,3500,3585,3670,3755,3840,3925,4010,4095,4180,4265,4350,4435,4520,4605,4690,4775,4860,4945,5030,5115,5200,5285,5370,5455,5540,5625,5710,5795,5880,5965,6050,6135,6220,6305,6390,6475,6560,6645,6730,6815,6900,6985,7070,7155,7240,7325,7410,7495,7580,7665,7750,7835,7920,8005,8090,8175,8260,8345,8430,8515,8600,8685,8770,8855,8940,9025,9110,9195,9280,9365,9450,9535,9620,9705,9790,9875,9960,10045,10130,10215,10300,10385,10470,10555,10640,10725,10810,10895,10980,11065,11150,11235,11320,11405,11490,11575,11660,11745,11830,11915,12000,12085,12170,12255,12340,12425,12510,12595,12680,12765,12850,12935,13020,13105,13190,13275,13360,13445,13530,13615,13700,13785,13870,13955,14040,14125,14210,14295,14380};

         const int c_iDAC_FullListSize = 300;
         unsigned long _aiFullDAC_List[ c_iDAC_FullListSize ];

         // Duration of each segment
         const unsigned long _iDAC_Duration = 13;
         // Array of durations
         unsigned long _aiFullDurationList[ c_iDAC_FullListSize ];

         // Index into the full list
         int _iFullListIndex = 0;

         // Now fill in the full list, first a Amplitude reference
         _aiFullDAC_List[ _iFullListIndex ] = _iAmplitudeRef_DAC;
         _aiFullDurationList[ _iFullListIndex++ ] = _iDAC_Duration;

         // Loop through the orignal list, interleaving the phase
         int _iDAC_Count = 0;
         for ( _iDAC_Count = 0; _iDAC_Count < c_iDAC_ListSize; _iDAC_Count++ )
         {
            // Phase reference
            _aiFullDurationList[ _iFullListIndex ] = _iDAC_Duration;
            _aiFullDAC_List[ _iFullListIndex ] = _iPhaseRef_DAC;

            // Go to the next index
            _iFullListIndex++;

            // Next DAC
            _aiFullDurationList[ _iFullListIndex ] = _iDAC_Duration;
            _aiFullDAC_List[ _iFullListIndex ] = _aiDAC_List[_iDAC_Count ];

            // Go to the next index
            _iFullListIndex++;
         }

         // Print values in array
         // Loop through the orignal list, interleaving the phase
         printf("Index\tDAC  \tDuration");
         printf("-----\t---- \t--------");
         for ( _iDAC_Count = 0; _iDAC_Count < _iFullListIndex; _iDAC_Count++ )
         {
            printf("\n%d\t%d\t%d", _iDAC_Count, _aiFullDAC_List[_iDAC_Count], _aiFullDurationList[_iDAC_Count] );
         }

         unsigned char _bOk = QLIB_FTM_GSM_POLAR_TX_SWEEP_CAL(
                                 g_hResourceContext,
                                 c_iDAC_FullListSize,
                                 _aiFullDAC_List,
                                 _aiFullDurationList );
      } // cKey == 'B'
      else if (cKey == 'C')
      {
         // printf("\nC = QLIB_FTM_GSM_TX_4X_FMOD_SWEEP");

         unsigned short _iSegLength = 273;
         unsigned short _iDACLevel = 800;

         bOk = QLIB_FTM_GSM_TX_4X_FMOD_SWEEP(
                  g_hResourceContext,
                  _iSegLength,
                  _iDACLevel
               );

         printf("\nQLIB_FTM_GSM_TX_4X_FMOD_SWEEP(): %s", bOk ? "PASS" : "FAIL" );
      } // cKey == 'C'
      else if (cKey == 'D')
      {
         // printf("\nD = QLIB_FTM_SET_TX_FRAME_MATRIX ");

         // Set Tx On
         bOk &= QLIB_FTM_SET_TX_ON( g_hResourceContext );
         printf("\nQLIB_FTM_SET_TX_ON(): %s", bOk ? "PASS" : "FAIL" );

         unsigned char _iSlotNum = 3;
         unsigned char _iTurnSlotOn = 1;
         unsigned short _iPwrLevel = 3000; // dBm/100
         unsigned char _iModScheme = 0;

         bOk = QLIB_FTM_SET_TX_FRAME_MATRIX (
                  g_hResourceContext,
                  _iSlotNum,
                  _iTurnSlotOn,
                  _iPwrLevel,
                  _iModScheme);
         printf("\nQLIB_FTM_SET_TX_FRAME_MATRIX(): %s", bOk ? "PASS" : "FAIL" );
      } // cKey == 'D'
   }

   return bOk;
}

unsigned char Test_FTM_Common( )
{
   //! Note: the frequency sweep API is in the FTM Common group, but it is demonstrated in Test_FTM_WCDMA()

   unsigned char bOk = true;
   char cKey = '\0';

   unsigned short testRFNVItem = 1002 + newRfNvItemOffset;  //20000 (New offset) + 1749
   unsigned char  _iNVValue[128];
   unsigned short iFTM_error_code = 0;
   bool exitThisMenu = false;

   do
   {
      printf("----------------\nFTM Common Test\n----------------\n");

      printf("\n1=QLIB_FTM_LOAD_RF_NV");
      printf("\n2=QLIB_FTM_DO_INTERNAL_DEVICE_CAL");
      printf("\n3=QLIB_FTM_RFNV_WRITE (NV ID:%d)", testRFNVItem);
      printf("\n4=QLIB_FTM_RFNV_READ (NV ID:%d)", testRFNVItem);
      printf("\n5=QLIB_FTM_RFNV_READ M items starting from N: ");
      printf("\n6=QLIB_FTM_RFNV_WRITE M items starting from N: ");
      printf("\n7=QLIB_FTM_RFNV_WRITE Item of 512 bytes, ID 50000: ");
      printf("\n8=QLIB_FTM_RFNV_READ Item of 512 bytes, ID 50000: ");
      printf("\n9=QLIB_FTM_DO_RSB_TXLO_CAL");
      printf("\nQ=EXIT FROM THIS MENU");

      printf("\nPlease select an option: " );
      cKey = getch();
      printf("\n");

      switch (cKey)
      {
         case '1':
         {
            unsigned short _iStatus = 0;
            bOk = QLIB_FTM_LOAD_RF_NV( g_hResourceContext, &_iStatus );
            printf("\nQLIB_FTM_LOAD_RF_NV() = %s, Status = %d", bOk ? "PASS" : "FAIL", _iStatus );
         }
         break;
         case '2':
         {
            bOk = QLIB_FTM_DO_INTERNAL_DEVICE_CAL( g_hResourceContext, &iFTM_error_code );
            printf("\nQLIB_FTM_DO_INTERNAL_DEVICE_CAL() = %s", bOk ? "PASS" : "FAIL" );
         }
         break;
         case '3':
         {
            memset(_iNVValue, 0, 128);
            _iNVValue[0] = 0x34;
            _iNVValue[1] = 0x12;
            bOk = QLIB_FTM_RFNV_WRITE(g_hResourceContext, testRFNVItem, _iNVValue, 128, &iFTM_error_code);
            printf("\nQLIB_FTM_RFNV_WRITE() = %s, FTM_Error_Code =%d", bOk ? "PASS" : "FAIL", iFTM_error_code );
         }
         break;
         case '4':
         {
            static int lastItemRead = 21002;
            unsigned short _iNVValueSz = 128;

            printf("\n");

            //printf("\nEnter RFNV ID (%d) ", lastItemRead);
            //scanf("%d", &lastItemRead);
            //printf("\n");

            memset(_iNVValue, 0, 128);
            bOk = QLIB_FTM_RFNV_READ(g_hResourceContext, lastItemRead, _iNVValue, &_iNVValueSz, &iFTM_error_code);
            printf("\nQLIB_FTM_RFNV_READ() = %s, FTM_Error_Code =%d", bOk ? "PASS" : "FAIL", iFTM_error_code );
            printf("\nNV data:\n");
         }
         break;
         case '5':
         {
            int startItemId = 0;
            int numberOfItems = 0;
            unsigned short _iNVValueSz = 128;

            printf("\n");

            do
            {
               printf("\nEnter start ID (%d - %d): ", newRfNvItemOffset, maxRfNvItemId);
               scanf("%d", &startItemId);
            }
            while(startItemId<newRfNvItemOffset || maxRfNvItemId<startItemId);
            printf("\nEnter number of items: ");
            scanf("%d", &numberOfItems);

            printf("\n");
            for (int i=startItemId; i<startItemId+numberOfItems && i<=maxRfNvItemId; i++)
            {
               unsigned short _iNVValueSz = 128;
               DWORD dwOldTime = 0;
               DWORD dwTimeElapsed = 0;
               memset(_iNVValue, 0, 128);
               printf("\nItem: %d, ", i);
               dwOldTime = GetTickCount();
               bOk = QLIB_FTM_RFNV_READ(g_hResourceContext, i, _iNVValue, &_iNVValueSz, &iFTM_error_code);
               dwTimeElapsed = GetTickCount()-dwOldTime;

               if( false == bOk || 0 != iFTM_error_code)
               {
                  printf ("FAIL %d ms\n", dwTimeElapsed);
               }
               else
               {
                  printf ("PASS %d ms\n", dwTimeElapsed);
                  printf ("Data:\n");
               }
               if(kbhit())
               {
                  cKey = getch();
                  if(27 == cKey)
                  {
                     break;
                  }
               }
            }
         }
         break;
         case '6':
         {
            int startItemId = 0;
            int numberOfItems = 0;
            unsigned short _iNVValueSz = 128;

            printf("\n");

            do
            {
               printf("\nEnter start ID (%d - %d): ", newRfNvItemOffset, maxRfNvItemId);
               scanf("%d", &startItemId);
            }
            while(startItemId<newRfNvItemOffset || maxRfNvItemId<startItemId);
            printf("\nEnter number of items: ");
            scanf("%d", &numberOfItems);

            printf("\n");
            for (int i=startItemId; i<startItemId+numberOfItems && i<=maxRfNvItemId; i++)
            {
               unsigned short _iNVValueSz = 128;
               DWORD dwOldTime = 0;
               DWORD dwTimeElapsed = 0;
               memset(_iNVValue, 0, 128);
               printf("\nItem: %d, ", i);
               dwOldTime = GetTickCount();
               bOk = QLIB_FTM_RFNV_WRITE(g_hResourceContext, i, _iNVValue, _iNVValueSz, &iFTM_error_code);
               dwTimeElapsed = GetTickCount()-dwOldTime;

               if( false == bOk || 2 != iFTM_error_code)
               {
                  printf ("FAIL %d ms\n", dwTimeElapsed);
               }
               else
               {
                  printf ("PASS %d ms\n", dwTimeElapsed);
               }
               if(kbhit())
               {
                  char dKey = '\0';
                  dKey = getch();
                  if(27 == dKey)
                  {
                     break;
                  }
               }
            }
         }
         break;
         case '7':
         {
            const int bigItemSize = 512;
            unsigned char bigItem[bigItemSize];
            memset(bigItem, 0, bigItemSize);
            bigItem[0] = 0x34;
            bigItem[1] = 0x12;
            bOk = QLIB_FTM_RFNV_WRITE(g_hResourceContext, 50000, bigItem, bigItemSize, &iFTM_error_code);
            printf("\nQLIB_FTM_RFNV_WRITE() = %s, FTM_Error_Code =%d", bOk ? "PASS" : "FAIL", iFTM_error_code );
         }
         break;
         case '8':
         {
            const int bigItemSize = 512;
            unsigned short itemsRead = bigItemSize;
            unsigned char bigItem[bigItemSize];
            memset(bigItem, 0, bigItemSize);

            bOk = QLIB_FTM_RFNV_READ(g_hResourceContext, 50000, bigItem, &itemsRead, &iFTM_error_code);
            printf("\nQLIB_FTM_RFNV_READ() = %s, FTM_Error_Code =%d", bOk ? "PASS" : "FAIL", iFTM_error_code );
            printf("\nNV data:\n");
         }
         break;
         case '9':
         {
            Test_RFCal_TxRsb_TxLo_Internal_Cal();
         }
         break;
         case 'Q':
         case 'q':
         case 27 : //ESC
            exitThisMenu = true;
            break;
         default:
         {
            printf("Not implemented!");
         }
         break;
      }
      printf("\n");
   }
   while(false == exitThisMenu);
   return bOk;
}

