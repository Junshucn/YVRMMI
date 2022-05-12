/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFVerify/RFVerify.h#2 $
 * $DateTime: 2017/08/28 02:44:19 $
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
#pragma once

// Disable compiler waring 'getch' was declared deprecated, for MSVC8
#pragma warning (disable : 4996)

#include <vector>
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

using namespace std;
//-------------------Start ARA------------------------------//

enum PATH_TYPE :
unsigned char
{
   PATH_TYPE_PRX = 0,
   PATH_TYPE_DRX = 1,
   PATH_TYPE_TX = 2,
   PATH_TYPE_MAX = PATH_TYPE_TX
};

 enum TECHNOLOGY_FAMILY_TYPE
{
   TECHNOLOGY_CDMA,
   TECHNOLOGY_WCDMA,
   TECHNOLOGY_GSM,
   TECHNOLOGY_BLUETOOTH,
   TECHNOLOGY_MEDIAFLO,
   TECHNOLOGY_DVBH,
   TECHNOLOGY_ANALOG,
   TECHNOLOGY_GPS,
   TECHNOLOGY_ISDBT,
   TECHNOLOGY_1xEVDO,
   TECHNOLOGY_LTE,
   TECHNOLOGY_TDSCDMA,
   TECHNOLOGY_WLAN,
   TECHNOLOGY_NFC,
   TECHNOLOGY_MULTIMEDIA,
   TECHNOLOGY_FM_RADIO,
   TECHNOLOGY_WIGIG,
   //IF ADDING MORE ENUMS, ALSO ADD AN ENTRY IN TechnologyTypeToName METHOD
   TECHNOLOGY_TYPE_MAX = 0xFFFFFFFF
};


//FTM-RF technology type
enum ftm_rf_technology_type
{
   FTM_RF_TECH_CDMA = 0,                 /* RF is in CDMA tech     */
   FTM_RF_TECH_WCDMA = 1,                /* RF is in WCDMA tech    */
   FTM_RF_TECH_GSM = 2,                  /* RF is in GSM tech      */
   FTM_RF_TECH_LTE = 3,                  /* RF is in LTE tech      */
   FTM_RF_TECH_TDSCDMA = 4,              /* RF is in TDSCDMA tech  */
   FTM_RF_TECH_HDR = 5,                  /* RF is in HDR tech      */
   FTM_RF_TECH_UNKNOWN = 0xFF
};

void PopulateARAProperties( rf_path_assignment &rfpaths, ftm_rf_test_property_data* requestData );

bool AssignRadioAllocations( HANDLE hResourceContext, RFPathInformationPacketVersion1 Rfpathv1,TECHNOLOGY_FAMILY_TYPE tech, int band );

void Test_ARA();

//-------------------end ARA------------------------------//

void Test_QLINK();
