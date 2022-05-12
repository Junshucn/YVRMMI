/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/QMSL_NVManager_Demo.h#7 $
 * $DateTime: 2017/07/05 15:35:28 $
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
#include "QLib.h"
#include "QLib_Defines.h"
//#include <stdlib.h>
#include <stdio.h>

unsigned char Test_NV_Manager( void );

//CDMA Intelliceiver Platform
unsigned char MSM6800_DZRF( void );

// QSC60x0, MSM60X0, or any CDMA platforms that uses NV_CDMA_xxx or NV_PCS_xxx
unsigned char QSC60X0_MSM60X0(void);

// CDMA target beta scaling calibration (SC2X)
unsigned char CDMA2000_TxBetaScaling(void );
      
// UMTS targets with RTR6285
unsigned char UMTS_RTR6285_Tx_Calibration(  void);
         
// UMTS RTR6250(3U) Platform Tx Calibration
unsigned char Tx_3U_Calibration( void );

// GSM External Polar Calibration
unsigned char GSM_Tx_Polar_Calibration( void);
         
// Range Gain Calibration, GSM Calibration Channel, GSM Rx IM2, and Thermistor Cal
unsigned char GSM_Rx_Calibration_And_Misc(  void);

// UMTS with RTR6285 2nd Frequency compensation
unsigned char UMTS_RTR6285_Tx2ndFreqComp();

// 1x targets with RTR6500 Tx Calibration
unsigned char CDMA_RTR6500_Calibration();

//Add a custom defined NV item to V manager
unsigned char Add_OEM_NV_Item( void);

// Write all NV items cached by NV Manager to Phone
unsigned char Write_NV_To_Mobile(  void);

// Clear all the values to accept the next set of items
unsigned char Clear_Data( void);

// Print the NV items cached by NV Manager
unsigned char Create_Text_Report( void );

// Do linear interpolation and slope averaging extrapolation
unsigned char LinearInterpolation_SlopeAveragingExtrapolation (void);

// GSM Linear Calibration
unsigned char GSM_Tx_Linear_Calibration( void);

// GSM Linear Calibration V2
unsigned char GSM_Tx_Linear_Calibration_V2( void);

// WCDMA RTR8600 Tx/Rx Calibration
unsigned char WCDMA_RTR8600_Calibration(void);

// LTE RTR8600 Tx/Rx Calibration
unsigned char LTE_RTR8600_Calibration(void);

//Enhanced Sweep Calibration NV
unsigned char Enhanced_Sweep_Calibration_NV_Generation(void);

// Backup RFNV item to NV Manager
// Copy RFNV item data from NV Tool to NV Manager
unsigned char BackupRFNV_and_CopyDataFromNVTool();

//GSM Linear Calibration Multichain
unsigned char GSM_Tx_Linear_Calibration_Multichain();

//GSM Rx Calibration Multichain
unsigned char GSM_Rx_Calibration_MultiChain();

// Read and Write compressed RFNV demo
// Read compressed RFNV from DUT as uncompressed binary data, and write back to NV manager 
unsigned char Read_Write_Compressed_RFNV( void );
