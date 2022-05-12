/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFVerify/RFVerify_Legacy.h#1 $
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
#pragma once
// Disable compiler waring 'getch' was declared deprecated, for MSVC8
#pragma warning (disable : 4996)

#include <ostream>
#include <vector>
#include <conio.h>
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
unsigned char Test_RF_Verification();
unsigned char Test_WCDMA_FTM_RF_Tx();
unsigned char Test_WCDMA_FTM_RF_Rx();
unsigned char Test_LTE_FTM_RF_Tx();
unsigned char Test_LTE_FTM_RF_Rx();
unsigned char Test_GSM_FTM_RF_Tx();
unsigned char Test_GSM_FTM_RF_Rx();
unsigned char Test_FTM_cdma2000( void );
unsigned char Test_GSM_BER( void );
unsigned char Test_WCDMA_BER( void );
unsigned char Test_EGPRS_BER( void );
unsigned char Test_HSDPA_BER( void );
unsigned char Test_LTE_NS( void );
unsigned char Test_FTM_WCDMA( int iChannel );
unsigned char Test_FTM_GSM( void );
unsigned char Test_FTM_Sequencer( void );
unsigned char Test_FTM_Sequencer_AddWait(
   unsigned long iWaitPeriod,
   unsigned char bUseSmallWaits,
   unsigned short* pStatus);

void PrintWCDMA_AGC( void );

unsigned char Test_FTM_cdma2k_NON_SIGNALING( void );
unsigned char Test_FTM_1XEVDO_NON_SIGNALING( void );

