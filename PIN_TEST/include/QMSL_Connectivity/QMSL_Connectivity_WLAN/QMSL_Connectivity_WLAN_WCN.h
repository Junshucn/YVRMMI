/******************************************************************************/
/**
Program: QMSL_WLAN_WCN

	$Id: //depot/HTE/QDART/QMSL6/inc/QMSL_Connectivity/QMSL_Connectivity_WLAN/QMSL_Connectivity_WLAN_WCN.h#10 $

\code

	- Defines the "C" interface for all Factory Library functions.

	- Table of contents (text search can be done for the following sections)

			- General Information
			- Related Documents
			- Version History

			- WLAN FTM


	------------------------------------------------------


	----------------------
	General Information
	----------------------

	- All data types used in this interface are ANSI C compatible.

	- All returns that indicate "true if succeed, false if fail" are defined
		as:  1 = true = Success , 0 = false = failure

		Functions that are not entirely upper case do not correspond directly to
		an FTM or diagnostic command.

		For example, QLIB_ConnectServer() is not mapped directly to a diagnostic
		command.


	- Variable Names
		Variable names are prefixed in lower case to indicate the data type.
		The following are defined:

			p = Pointer
			i = Integer type, either char, short, or long
			d = Double or floating point
			e = Enumeration (possibly stored as a short, but representing
			                 a fixed list of possibilities)
			b = unsigned char, 0=false, 1=true

	----------------------
	Related Documents
	----------------------

	- The function name convention used in this document is as follows:

		QLIB_ indicates that this function corresponds to library function
		in the QLib project.

		From there, all diagnostic and FTM functions are named exactly as
		the command identifiers are defined in the QLib_Defines.h.

	- For a list of all document numbers referred to by QMSL, please see following section in the QDART Help file:
		1) QDART -> QDART Related Documents
	or  2) QDART -> QMSL -> Getting Started -> References.


\endcode


	-----------------------
	Version History
	-----------------------

 \b QUALCOMM  \b PROPRIETARY

	This document contains propriety information, and except with written
	permission of Qualcomm Technologies INC, such information shall not be
	published, or disclosed to others, or used for any purpose, and the
	document shall not be duplicated in whole or in part.


	Copyright (c) 2004-2012 Qualcomm Technologies Incorporated.
	All Rights Reserved.
	Qualcomm Confidential and Proprietary

\version
Version information is stored in the file "QMSL_Release_Notes.txt"

\note
Compiler:  Microsoft Visual C++ v6.0 SP4
*******************************************************************************/

#if !defined(_QLIB_WLAN_WCN_H)
#define _QLIB_WLAN_WCN_H

#if !defined(_WINDOWS_)
#include "windows.h"
#endif

/**
	Establish whether the library is being compiled into a DLL (exporting),
	or being included from a client (importing)

	When the DLL is built, then QLIB_EXPORTS should be defined
*/
#ifdef QLIB_API
#undef QLIB_API
#endif

#ifdef QLIB_EXPORTS
#define QLIB_API __declspec(dllexport)
#else
#define QLIB_API __declspec(dllimport)
#endif


#if defined(QLIB_STATIC)
#undef QLIB_API
#define QLIB_API
//#define QLIB_API __cdecl
#endif


#ifdef __cplusplus
extern "C" {
#endif


   /******************************************************************************
   						WLAN FTM

     NOTE: Please check the module manfucturer's documentation for specific information
           about the commands or parameters.  The documentation for the module-specific
   		commands is based sources and Qualcomm can give very little support about
   		individual commands and they can also not add new commands unless the
   		module vendor writes the corresponding firmware and provides the command
   		specification.

   *******************************************************************************/

	/**********************************************************************************
                                   GEN6 WLAN Commands
   ***********************************************************************************/

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_START

     Start Test Mode.

   \return true if successful, false if failure

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_START
   (
      HANDLE hResourceContext,
      int wlanID
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_STOP

     Stop Test Mode.

   \return true if successful, false if failure

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_STOP
   (
      HANDLE hResourceContext
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_CHANNEL

     Set the channel.

   \param iChannel  =	Channel Index (1-14)

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_CHANNEL
   (
      HANDLE hResourceContext,
      unsigned int iChannel
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_CHANNEL_V2

     Set the channel.

   \param iChannel  =	Channel Index (1-300)
   \param channelBondingState  = 0: no bonding, 1: channel is primary low, 3: channel is primary high

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_CHANNEL_V2
   (
      HANDLE hResourceContext,
      unsigned int iChannel,
      unsigned int channelBondingState
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_CLOSED_LOOP_POWER

     Set Tx closed loop power.

   \param dPowerIndBm  =	0 to 20 dBm

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_CLOSED_LOOP_POWER
   (
      HANDLE hResourceContext,
      double dPowerIndBm
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP

     Start/stop the Tx Packet generator.

   \param iStart  =	0: stop, 1: start

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP
   (
      HANDLE hResourceContext,
      unsigned char iStart
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_RX_DISABLE_MODE

     Enable/Disable the receiver capabilities.

   \param agPktsDisabled = true:disable A/G packets
   \param bPktsDisabled = true:disable B packets
   \param slrPktsDisabled = true:disable SLR packets

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_RX_DISABLE_MODE
   (
      HANDLE hResourceContext,
      unsigned char agPktsDisabled,
      unsigned char bPktsDisabled,
      unsigned char slrPktsDisabled
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_RX_DISABLE_MODE_V2

     Enable/Disable the receiver capabilities.

   \param mask: bit 0: disable B packets, bit 1: disable A/G packets bit,
   			 bit 2: disable N 40 MHz packets, bit 3: disable AC 80 MHz packets

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_RX_DISABLE_MODE_V2
   (
      HANDLE hResourceContext,
      unsigned long mask
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_TX_FRAME

     Configure the WLAN Tx.

   \param iNumTestFrame = number of test frames to transmit; 0 = infinite
   \param iPayloadSize = size of payload in bytes
   \param iFrameSpacing = spacing between frames in uSec
   \param iFCSCal = true:enable CRC calulations
   \param iRate =
   \code
      ------------------------------
      ---- WCN1312/1314 values -----
      ------------------------------
      --- 802.11b Rates ---
      0: HAL_PHY_RATE_11B_LONG_1_MBPS
      1: HAL_PHY_RATE_11B_LONG_2_MBPS
      2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
      3: HAL_PHY_RATE_11B_LONG_11_MBPS
      4: HAL_PHY_RATE_11B_SHORT_2_MBPS
      5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
      6: HAL_PHY_RATE_11B_SHORT_11_MBPS
      --- SLR Rates ---
      7: HAL_PHY_RATE_SLR_0_25_MBPS
      8: HAL_PHY_RATE_SLR_0_5_MBPS
      --- Spica_Virgo 11A 20MHz Rates ---
      9: HAL_PHY_RATE_11A_6_MBPS
      10: HAL_PHY_RATE_11A_9_MBPS
      11: HAL_PHY_RATE_11A_12_MBPS
      12: HAL_PHY_RATE_11A_18_MBPS
      13: HAL_PHY_RATE_11A_24_MBPS
      14: HAL_PHY_RATE_11A_36_MBPS
      15: HAL_PHY_RATE_11A_48_MBPS
      16: HAL_PHY_RATE_11A_54_MBPS
      --- MCS Index #0-15 (20MHz) ---
      17: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS
      18: HAL_PHY_RATE_MCS_1NSS_13_MBPS
      19: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS
      20: HAL_PHY_RATE_MCS_1NSS_26_MBPS
      21: HAL_PHY_RATE_MCS_1NSS_39_MBPS
      22: HAL_PHY_RATE_MCS_1NSS_52_MBPS
      23: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS
      24: HAL_PHY_RATE_MCS_1NSS_65_MBPS
      25: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS
      26: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS
      27: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS
      28: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS
      29: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS
      30: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      31: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      32: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS
      33: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS
  
      ------------------------------
      ------  WCN3660 values -------
      ------------------------------
      --- 802.11b Rates ---
      0: HAL_PHY_RATE_11B_LONG_1_MBPS
      1: HAL_PHY_RATE_11B_LONG_2_MBPS
      2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
      3: HAL_PHY_RATE_11B_LONG_11_MBPS
      4: HAL_PHY_RATE_11B_SHORT_2_MBPS
      5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
      6: HAL_PHY_RATE_11B_SHORT_11_MBPS
      --- Spica_Virgo 11A 20MHz Rates ---
      7: HAL_PHY_RATE_11A_6_MBPS
      8: HAL_PHY_RATE_11A_9_MBPS
      9: HAL_PHY_RATE_11A_12_MBPS
      10: HAL_PHY_RATE_11A_18_MBPS
      11: HAL_PHY_RATE_11A_24_MBPS
      12: HAL_PHY_RATE_11A_36_MBPS
      13: HAL_PHY_RATE_11A_48_MBPS
      14: HAL_PHY_RATE_11A_54_MBPS
      --- 11A 20MHz Rates ---
      15: HAL_PHY_RATE_11A_DUP_6_MBPS
      16: HAL_PHY_RATE_11A_DUP_9_MBPS
      17: HAL_PHY_RATE_11A_DUP_12_MBPS
      18: HAL_PHY_RATE_11A_DUP_18_MBPS
      19: HAL_PHY_RATE_11A_DUP_24_MBPS
      20: HAL_PHY_RATE_11A_DUP_36_MBPS
      21: HAL_PHY_RATE_11A_DUP_48_MBPS
      22: HAL_PHY_RATE_11A_DUP_54_MBPS
      --- MCS Index #0-7 (20MHz) ---
      23: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS
      24: HAL_PHY_RATE_MCS_1NSS_13_MBPS
      25: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS
      26: HAL_PHY_RATE_MCS_1NSS_26_MBPS
      27: HAL_PHY_RATE_MCS_1NSS_39_MBPS
      28: HAL_PHY_RATE_MCS_1NSS_52_MBPS
      29: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS
      30: HAL_PHY_RATE_MCS_1NSS_65_MBPS
      31: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS
      32: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS
      33: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS
      34: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS
      35: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS
      36: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      37: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS
      38: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS
      --- MCS Index #0-7 (40MHz) ---
      39: HAL_PHY_RATE_MCS_1NSS_CB_13_5_MBPS
      40: HAL_PHY_RATE_MCS_1NSS_CB_27_MBPS
      41: HAL_PHY_RATE_MCS_1NSS_CB_40_5_MBPS
      42: HAL_PHY_RATE_MCS_1NSS_CB_54_MBPS
      43: HAL_PHY_RATE_MCS_1NSS_CB_81_MBPS
      44: HAL_PHY_RATE_MCS_1NSS_CB_108_MBPS
      45: HAL_PHY_RATE_MCS_1NSS_CB_121_5_MBPS
      46: HAL_PHY_RATE_MCS_1NSS_CB_135_MBPS
      47: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_15_MBPS
      48: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_30_MBPS
      49: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_45_MBPS
      50: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_60_MBPS
      51: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_90_MBPS
      52: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_120_MBPS
      53: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_135_MBPS
      54: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_150_MBPS

      ---------------------------------------------
      ------  WCN3680 NV rev 0 and 1 values -------
      ---------------------------------------------
      --- 802.11b Rates ---
      0: HAL_PHY_RATE_11B_LONG_1_MBPS
      1: HAL_PHY_RATE_11B_LONG_2_MBPS
      2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
      3: HAL_PHY_RATE_11B_LONG_11_MBPS
      4: HAL_PHY_RATE_11B_SHORT_2_MBPS
      5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
      6: HAL_PHY_RATE_11B_SHORT_11_MBPS
      -- Spica_Virgo 11A 20MHz Rates --
      7: HAL_PHY_RATE_11A_6_MBPS
      8: HAL_PHY_RATE_11A_9_MBPS
      9: HAL_PHY_RATE_11A_12_MBPS
      10: HAL_PHY_RATE_11A_18_MBPS
      11: HAL_PHY_RATE_11A_24_MBPS
      12: HAL_PHY_RATE_11A_36_MBPS
      13: HAL_PHY_RATE_11A_48_MBPS
      14: HAL_PHY_RATE_11A_54_MBPS
      -- Duplicate 11A 20MHz Rates --
      15: HAL_PHY_RATE_11A_DUP_6_MBPS
      16: HAL_PHY_RATE_11A_DUP_9_MBPS
      17: HAL_PHY_RATE_11A_DUP_12_MBPS
      18: HAL_PHY_RATE_11A_DUP_18_MBPS
      19: HAL_PHY_RATE_11A_DUP_24_MBPS
      20: HAL_PHY_RATE_11A_DUP_36_MBPS
      21: HAL_PHY_RATE_11A_DUP_48_MBPS
      22: HAL_PHY_RATE_11A_DUP_54_MBPS
      -- MCS Index #0-7 (20MHz) -- 
      23: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS
      24: HAL_PHY_RATE_MCS_1NSS_13_MBPS
      25: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS
      26: HAL_PHY_RATE_MCS_1NSS_26_MBPS
      27: HAL_PHY_RATE_MCS_1NSS_39_MBPS
      28: HAL_PHY_RATE_MCS_1NSS_52_MBPS
      29: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS
      30: HAL_PHY_RATE_MCS_1NSS_65_MBPS
      31: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS
      32: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS
      33: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS
      34: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS
      35: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS
      36: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      37: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS
      38: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS
      -- MCS Index #0-7 (40MHz) --
      39: HAL_PHY_RATE_MCS_1NSS_CB_13_5_MBPS
      40: HAL_PHY_RATE_MCS_1NSS_CB_27_MBPS
      41: HAL_PHY_RATE_MCS_1NSS_CB_40_5_MBPS
      42: HAL_PHY_RATE_MCS_1NSS_CB_54_MBPS
      43: HAL_PHY_RATE_MCS_1NSS_CB_81_MBPS
      44: HAL_PHY_RATE_MCS_1NSS_CB_108_MBPS
      45: HAL_PHY_RATE_MCS_1NSS_CB_121_5_MBPS
      46: HAL_PHY_RATE_MCS_1NSS_CB_135_MBPS
      47: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_15_MBPS
      48: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_30_MBPS
      49: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_45_MBPS
      50: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_60_MBPS
      51: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_90_MBPS
      52: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_120_MBPS
      53: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_135_MBPS
      54: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_150_MBPS
      -- 11A duplicate 80MHz Rates --
      55: HAL_PHY_RATE_11AC_DUP_6_MBPS
      56: HAL_PHY_RATE_11AC_DUP_9_MBPS
      57: HAL_PHY_RATE_11AC_DUP_12_MBPS
      58: HAL_PHY_RATE_11AC_DUP_18_MBPS
      59: HAL_PHY_RATE_11AC_DUP_24_MBPS
      60: HAL_PHY_RATE_11AC_DUP_36_MBPS
      61: HAL_PHY_RATE_11AC_DUP_48_MBPS
      62: HAL_PHY_RATE_11AC_DUP_54_MBPS
      -- 11AC rate 20MHZ NGI/SGI --
      63: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_6_5_MBPS
      64: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_13_MBPS
      65: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_19_5_MBPS
      66: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_26_MBPS
      67: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_39_MBPS
      68: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_52_MBPS
      69: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_58_5_MBPS
      70: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_65_MBPS
      71: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_78_MBPS
      72: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_7_2_MBPS
      73: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_14_4_MBPS
      74: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_21_6_MBPS
      75: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_28_8_MBPS
      76: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_43_3_MBPS
      77: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_57_7_MBPS
      78: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_65_MBPS
      79: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_72_2_MBPS
      80: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_86_6_MBPS
      -- 11AC rates 40MHZ NGI/SGI --
      81: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_13_5_MBPS 
      82: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_27_MBPS
      83: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_40_5_MBPS
      84: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_54_MBPS
      85: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_81_MBPS
      86: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_108_MBPS
      87: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_121_5_MBPS
      88: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_135_MBPS
      89: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_162_MBPS
      90: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_180_MBPS
      91: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_15_MBPS 
      92: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_30_MBPS
      93: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_45_MBPS
      94: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_60_MBPS
      95: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_90_MBPS
      96: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_120_MBPS
      97: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_135_MBPS
      98: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_150_MBPS
      99: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_180_MBPS
      100: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_200_MBPS
      -- 11AC rates 80 MHZ NGI/SGI --
      101: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_29_3_MBPS 
      102: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_58_5_MBPS
      103: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_87_8_MBPS
      104: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_117_MBPS
      105: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_175_5_MBPS
      106: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_234_MBPS
      107: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_263_3_MBPS
      108: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_292_5_MBPS
      109: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_351_MBPS
      110: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_390_MBPS
      111: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_32_5_MBPS 
      112: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_65_MBPS
      113: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_97_5_MBPS
      114: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_130_MBPS
      115: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_195_MBPS
      116: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_260_MBPS
      117: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_292_5_MBPS
      118: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_325_MBPS
      119: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_390_MBPS
      120: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_433_3_MBPS
            	
      ---------------------------------------------------
      ------  WCN3680 NV rev 2 and greater values -------
      ---------------------------------------------------
   	--- 802.11b Rates ---
      0: HAL_PHY_RATE_11B_LONG_1_MBPS
      1: HAL_PHY_RATE_11B_LONG_2_MBPS
      2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
      3: HAL_PHY_RATE_11B_LONG_11_MBPS
      4: HAL_PHY_RATE_11B_SHORT_2_MBPS
      5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
      6: HAL_PHY_RATE_11B_SHORT_11_MBPS
      -- Spica_Virgo 11A 20MHz Rates --
      7: HAL_PHY_RATE_11A_6_MBPS
      8: HAL_PHY_RATE_11A_9_MBPS
      9: HAL_PHY_RATE_11A_12_MBPS
      10: HAL_PHY_RATE_11A_18_MBPS
      11: HAL_PHY_RATE_11A_24_MBPS
      12: HAL_PHY_RATE_11A_36_MBPS
      13: HAL_PHY_RATE_11A_48_MBPS
      14: HAL_PHY_RATE_11A_54_MBPS
      -- Duplicate 11A 20MHz Rates --
      15: HAL_PHY_RATE_11A_DUP_6_MBPS
      16: HAL_PHY_RATE_11A_DUP_9_MBPS
      17: HAL_PHY_RATE_11A_DUP_12_MBPS
      18: HAL_PHY_RATE_11A_DUP_18_MBPS
      19: HAL_PHY_RATE_11A_DUP_24_MBPS
      20: HAL_PHY_RATE_11A_DUP_36_MBPS
      21: HAL_PHY_RATE_11A_DUP_48_MBPS
      22: HAL_PHY_RATE_11A_DUP_54_MBPS
      -- MCS Index #0-7 (20MHz) -- 
      23: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS
      24: HAL_PHY_RATE_MCS_1NSS_13_MBPS
      25: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS
      26: HAL_PHY_RATE_MCS_1NSS_26_MBPS
      27: HAL_PHY_RATE_MCS_1NSS_39_MBPS
      28: HAL_PHY_RATE_MCS_1NSS_52_MBPS
      29: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS
      30: HAL_PHY_RATE_MCS_1NSS_65_MBPS
      31: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS
      32: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS
      33: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS
      34: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS
      35: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS
      36: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      37: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS
      38: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS
      -- MCS Index #0-7 (40MHz) --
      39: HAL_PHY_RATE_MCS_1NSS_CB_13_5_MBPS
      40: HAL_PHY_RATE_MCS_1NSS_CB_27_MBPS
      41: HAL_PHY_RATE_MCS_1NSS_CB_40_5_MBPS
      42: HAL_PHY_RATE_MCS_1NSS_CB_54_MBPS
      43: HAL_PHY_RATE_MCS_1NSS_CB_81_MBPS
      44: HAL_PHY_RATE_MCS_1NSS_CB_108_MBPS
      45: HAL_PHY_RATE_MCS_1NSS_CB_121_5_MBPS
      46: HAL_PHY_RATE_MCS_1NSS_CB_135_MBPS
      47: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_15_MBPS
      48: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_30_MBPS
      49: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_45_MBPS
      50: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_60_MBPS
      51: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_90_MBPS
      52: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_120_MBPS
      53: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_135_MBPS
      54: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_150_MBPS
      -- 11A duplicate 80MHz Rates --
      55: HAL_PHY_RATE_11AC_DUP_6_MBPS
      56: HAL_PHY_RATE_11AC_DUP_9_MBPS
      57: HAL_PHY_RATE_11AC_DUP_12_MBPS
      58: HAL_PHY_RATE_11AC_DUP_18_MBPS
      59: HAL_PHY_RATE_11AC_DUP_24_MBPS
      60: HAL_PHY_RATE_11AC_DUP_36_MBPS
      61: HAL_PHY_RATE_11AC_DUP_48_MBPS
      62: HAL_PHY_RATE_11AC_DUP_54_MBPS
      -- 11AC rate 20MHZ NGI/SGI --
      63: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_6_5_MBPS
      64: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_13_MBPS
      65: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_19_5_MBPS
      66: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_26_MBPS
      67: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_39_MBPS
      68: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_52_MBPS
      69: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_58_5_MBPS
      70: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_65_MBPS
      71: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_78_MBPS
      72: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_86_5_MBPS
      73: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_7_2_MBPS
      74: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_14_4_MBPS
      75: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_21_6_MBPS
      76: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_28_8_MBPS
      77: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_43_3_MBPS
      78: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_57_7_MBPS
      79: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_65_MBPS
      80: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_72_2_MBPS
      81: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_86_6_MBPS
      82: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_96_1_MBPS
      -- 11AC rates 40MHZ NGI/SGI --
      83: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_13_5_MBPS 
      84: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_27_MBPS
      85: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_40_5_MBPS
      86: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_54_MBPS
      87: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_81_MBPS
      88: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_108_MBPS
      89: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_121_5_MBPS
      90: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_135_MBPS
      91: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_162_MBPS
      92: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_180_MBPS
      93: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_15_MBPS 
      94: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_30_MBPS
      95: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_45_MBPS
      96: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_60_MBPS
      97: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_90_MBPS
      98: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_120_MBPS
      99: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_135_MBPS
      100: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_150_MBPS
      101: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_180_MBPS
      102: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_200_MBPS
      -- 11AC rates 80 MHZ NGI/SGI --
      103: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_29_3_MBPS 
      104: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_58_5_MBPS
      105: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_87_8_MBPS
      106: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_117_MBPS
      107: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_175_5_MBPS
      108: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_234_MBPS
      109: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_263_3_MBPS
      110: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_292_5_MBPS
      111: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_351_MBPS
      112: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_390_MBPS
      113: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_32_5_MBPS 
      114: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_65_MBPS
      115: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_97_5_MBPS
      116: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_130_MBPS
      117: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_195_MBPS
      118: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_260_MBPS
      119: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_292_5_MBPS
      120: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_325_MBPS
      121: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_390_MBPS
      122: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_433_3_MBPS
   \endcode

   \param iRate =
   \code
      0: PHYDBG_PREAMBLE_OFDM
      1: PHYDBG_PREAMBLE_GREENFIELD
      2: PHYDBG_PREAMBLE_MIXED
      3: PHYDBG_PREAMBLE_SHORTB
      4: PHYDBG_PREAMBLE_LONGB
   \endcode
   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_TX_FRAME
   (
      HANDLE hResourceContext,
      unsigned long iNumTestFrame,
      unsigned short iPayloadSize,
      unsigned long iFrameSpacing,
      unsigned char iFCSCal,
      unsigned long iRate,
      unsigned long iPreamble
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_TX_FRAME_V2

     Configure the WLAN Tx.

   \param iNumTestFrame = number of test frames to transmit; 0 = infinite
   \param iPayloadSize = size of payload in bytes
   \param iPayloadType = 0:None, 1: Fixed Byte, 2: Random, 3: Ramp
   \param iPayloadFixedByte = byte value for fixed byte payload
   \param iFrameSpacing = spacing between frames in uSec
   \param iFCSCal = true:enable CRC calulations
   \param iRate =
   \code
      ------------------------------
      ---- WCN1312/1314 values -----
      ------------------------------
      --- 802.11b Rates ---
      0: HAL_PHY_RATE_11B_LONG_1_MBPS
      1: HAL_PHY_RATE_11B_LONG_2_MBPS
      2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
      3: HAL_PHY_RATE_11B_LONG_11_MBPS
      4: HAL_PHY_RATE_11B_SHORT_2_MBPS
      5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
      6: HAL_PHY_RATE_11B_SHORT_11_MBPS
      --- SLR Rates ---
      7: HAL_PHY_RATE_SLR_0_25_MBPS
      8: HAL_PHY_RATE_SLR_0_5_MBPS
      --- Spica_Virgo 11A 20MHz Rates ---
      9: HAL_PHY_RATE_11A_6_MBPS
      10: HAL_PHY_RATE_11A_9_MBPS
      11: HAL_PHY_RATE_11A_12_MBPS
      12: HAL_PHY_RATE_11A_18_MBPS
      13: HAL_PHY_RATE_11A_24_MBPS
      14: HAL_PHY_RATE_11A_36_MBPS
      15: HAL_PHY_RATE_11A_48_MBPS
      16: HAL_PHY_RATE_11A_54_MBPS
      --- MCS Index #0-15 (20MHz) ---
      17: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS
      18: HAL_PHY_RATE_MCS_1NSS_13_MBPS
      19: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS
      20: HAL_PHY_RATE_MCS_1NSS_26_MBPS
      21: HAL_PHY_RATE_MCS_1NSS_39_MBPS
      22: HAL_PHY_RATE_MCS_1NSS_52_MBPS
      23: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS
      24: HAL_PHY_RATE_MCS_1NSS_65_MBPS
      25: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS
      26: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS
      27: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS
      28: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS
      29: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS
      30: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      31: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      32: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS
      33: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS
  
      ------------------------------
      ------  WCN3660 values -------
      ------------------------------
      --- 802.11b Rates ---
      0: HAL_PHY_RATE_11B_LONG_1_MBPS
      1: HAL_PHY_RATE_11B_LONG_2_MBPS
      2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
      3: HAL_PHY_RATE_11B_LONG_11_MBPS
      4: HAL_PHY_RATE_11B_SHORT_2_MBPS
      5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
      6: HAL_PHY_RATE_11B_SHORT_11_MBPS
      --- Spica_Virgo 11A 20MHz Rates ---
      7: HAL_PHY_RATE_11A_6_MBPS
      8: HAL_PHY_RATE_11A_9_MBPS
      9: HAL_PHY_RATE_11A_12_MBPS
      10: HAL_PHY_RATE_11A_18_MBPS
      11: HAL_PHY_RATE_11A_24_MBPS
      12: HAL_PHY_RATE_11A_36_MBPS
      13: HAL_PHY_RATE_11A_48_MBPS
      14: HAL_PHY_RATE_11A_54_MBPS
      --- 11A 20MHz Rates ---
      15: HAL_PHY_RATE_11A_DUP_6_MBPS
      16: HAL_PHY_RATE_11A_DUP_9_MBPS
      17: HAL_PHY_RATE_11A_DUP_12_MBPS
      18: HAL_PHY_RATE_11A_DUP_18_MBPS
      19: HAL_PHY_RATE_11A_DUP_24_MBPS
      20: HAL_PHY_RATE_11A_DUP_36_MBPS
      21: HAL_PHY_RATE_11A_DUP_48_MBPS
      22: HAL_PHY_RATE_11A_DUP_54_MBPS
      --- MCS Index #0-7 (20MHz) ---
      23: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS
      24: HAL_PHY_RATE_MCS_1NSS_13_MBPS
      25: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS
      26: HAL_PHY_RATE_MCS_1NSS_26_MBPS
      27: HAL_PHY_RATE_MCS_1NSS_39_MBPS
      28: HAL_PHY_RATE_MCS_1NSS_52_MBPS
      29: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS
      30: HAL_PHY_RATE_MCS_1NSS_65_MBPS
      31: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS
      32: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS
      33: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS
      34: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS
      35: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS
      36: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      37: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS
      38: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS
      --- MCS Index #0-7 (40MHz) ---
      39: HAL_PHY_RATE_MCS_1NSS_CB_13_5_MBPS
      40: HAL_PHY_RATE_MCS_1NSS_CB_27_MBPS
      41: HAL_PHY_RATE_MCS_1NSS_CB_40_5_MBPS
      42: HAL_PHY_RATE_MCS_1NSS_CB_54_MBPS
      43: HAL_PHY_RATE_MCS_1NSS_CB_81_MBPS
      44: HAL_PHY_RATE_MCS_1NSS_CB_108_MBPS
      45: HAL_PHY_RATE_MCS_1NSS_CB_121_5_MBPS
      46: HAL_PHY_RATE_MCS_1NSS_CB_135_MBPS
      47: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_15_MBPS
      48: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_30_MBPS
      49: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_45_MBPS
      50: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_60_MBPS
      51: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_90_MBPS
      52: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_120_MBPS
      53: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_135_MBPS
      54: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_150_MBPS

      ---------------------------------------------
      ------  WCN3680 NV rev 0 and 1 values -------
      ---------------------------------------------
      --- 802.11b Rates ---
      0: HAL_PHY_RATE_11B_LONG_1_MBPS
      1: HAL_PHY_RATE_11B_LONG_2_MBPS
      2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
      3: HAL_PHY_RATE_11B_LONG_11_MBPS
      4: HAL_PHY_RATE_11B_SHORT_2_MBPS
      5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
      6: HAL_PHY_RATE_11B_SHORT_11_MBPS
      -- Spica_Virgo 11A 20MHz Rates --
      7: HAL_PHY_RATE_11A_6_MBPS
      8: HAL_PHY_RATE_11A_9_MBPS
      9: HAL_PHY_RATE_11A_12_MBPS
      10: HAL_PHY_RATE_11A_18_MBPS
      11: HAL_PHY_RATE_11A_24_MBPS
      12: HAL_PHY_RATE_11A_36_MBPS
      13: HAL_PHY_RATE_11A_48_MBPS
      14: HAL_PHY_RATE_11A_54_MBPS
      -- Duplicate 11A 20MHz Rates --
      15: HAL_PHY_RATE_11A_DUP_6_MBPS
      16: HAL_PHY_RATE_11A_DUP_9_MBPS
      17: HAL_PHY_RATE_11A_DUP_12_MBPS
      18: HAL_PHY_RATE_11A_DUP_18_MBPS
      19: HAL_PHY_RATE_11A_DUP_24_MBPS
      20: HAL_PHY_RATE_11A_DUP_36_MBPS
      21: HAL_PHY_RATE_11A_DUP_48_MBPS
      22: HAL_PHY_RATE_11A_DUP_54_MBPS
      -- MCS Index #0-7 (20MHz) -- 
      23: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS
      24: HAL_PHY_RATE_MCS_1NSS_13_MBPS
      25: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS
      26: HAL_PHY_RATE_MCS_1NSS_26_MBPS
      27: HAL_PHY_RATE_MCS_1NSS_39_MBPS
      28: HAL_PHY_RATE_MCS_1NSS_52_MBPS
      29: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS
      30: HAL_PHY_RATE_MCS_1NSS_65_MBPS
      31: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS
      32: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS
      33: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS
      34: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS
      35: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS
      36: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      37: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS
      38: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS
      -- MCS Index #0-7 (40MHz) --
      39: HAL_PHY_RATE_MCS_1NSS_CB_13_5_MBPS
      40: HAL_PHY_RATE_MCS_1NSS_CB_27_MBPS
      41: HAL_PHY_RATE_MCS_1NSS_CB_40_5_MBPS
      42: HAL_PHY_RATE_MCS_1NSS_CB_54_MBPS
      43: HAL_PHY_RATE_MCS_1NSS_CB_81_MBPS
      44: HAL_PHY_RATE_MCS_1NSS_CB_108_MBPS
      45: HAL_PHY_RATE_MCS_1NSS_CB_121_5_MBPS
      46: HAL_PHY_RATE_MCS_1NSS_CB_135_MBPS
      47: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_15_MBPS
      48: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_30_MBPS
      49: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_45_MBPS
      50: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_60_MBPS
      51: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_90_MBPS
      52: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_120_MBPS
      53: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_135_MBPS
      54: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_150_MBPS
      -- 11A duplicate 80MHz Rates --
      55: HAL_PHY_RATE_11AC_DUP_6_MBPS
      56: HAL_PHY_RATE_11AC_DUP_9_MBPS
      57: HAL_PHY_RATE_11AC_DUP_12_MBPS
      58: HAL_PHY_RATE_11AC_DUP_18_MBPS
      59: HAL_PHY_RATE_11AC_DUP_24_MBPS
      60: HAL_PHY_RATE_11AC_DUP_36_MBPS
      61: HAL_PHY_RATE_11AC_DUP_48_MBPS
      62: HAL_PHY_RATE_11AC_DUP_54_MBPS
      -- 11AC rate 20MHZ NGI/SGI --
      63: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_6_5_MBPS
      64: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_13_MBPS
      65: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_19_5_MBPS
      66: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_26_MBPS
      67: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_39_MBPS
      68: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_52_MBPS
      69: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_58_5_MBPS
      70: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_65_MBPS
      71: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_78_MBPS
      72: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_7_2_MBPS
      73: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_14_4_MBPS
      74: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_21_6_MBPS
      75: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_28_8_MBPS
      76: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_43_3_MBPS
      77: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_57_7_MBPS
      78: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_65_MBPS
      79: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_72_2_MBPS
      80: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_86_6_MBPS
      -- 11AC rates 40MHZ NGI/SGI --
      81: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_13_5_MBPS 
      82: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_27_MBPS
      83: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_40_5_MBPS
      84: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_54_MBPS
      85: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_81_MBPS
      86: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_108_MBPS
      87: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_121_5_MBPS
      88: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_135_MBPS
      89: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_162_MBPS
      90: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_180_MBPS
      91: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_15_MBPS 
      92: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_30_MBPS
      93: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_45_MBPS
      94: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_60_MBPS
      95: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_90_MBPS
      96: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_120_MBPS
      97: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_135_MBPS
      98: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_150_MBPS
      99: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_180_MBPS
      100: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_200_MBPS
      -- 11AC rates 80 MHZ NGI/SGI --
      101: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_29_3_MBPS 
      102: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_58_5_MBPS
      103: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_87_8_MBPS
      104: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_117_MBPS
      105: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_175_5_MBPS
      106: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_234_MBPS
      107: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_263_3_MBPS
      108: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_292_5_MBPS
      109: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_351_MBPS
      110: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_390_MBPS
      111: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_32_5_MBPS 
      112: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_65_MBPS
      113: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_97_5_MBPS
      114: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_130_MBPS
      115: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_195_MBPS
      116: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_260_MBPS
      117: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_292_5_MBPS
      118: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_325_MBPS
      119: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_390_MBPS
      120: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_433_3_MBPS
            	
      ---------------------------------------------------
      ------  WCN3680 NV rev 2 and greater values -------
      ---------------------------------------------------
   	--- 802.11b Rates ---
      0: HAL_PHY_RATE_11B_LONG_1_MBPS
      1: HAL_PHY_RATE_11B_LONG_2_MBPS
      2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
      3: HAL_PHY_RATE_11B_LONG_11_MBPS
      4: HAL_PHY_RATE_11B_SHORT_2_MBPS
      5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
      6: HAL_PHY_RATE_11B_SHORT_11_MBPS
      -- Spica_Virgo 11A 20MHz Rates --
      7: HAL_PHY_RATE_11A_6_MBPS
      8: HAL_PHY_RATE_11A_9_MBPS
      9: HAL_PHY_RATE_11A_12_MBPS
      10: HAL_PHY_RATE_11A_18_MBPS
      11: HAL_PHY_RATE_11A_24_MBPS
      12: HAL_PHY_RATE_11A_36_MBPS
      13: HAL_PHY_RATE_11A_48_MBPS
      14: HAL_PHY_RATE_11A_54_MBPS
      -- Duplicate 11A 20MHz Rates --
      15: HAL_PHY_RATE_11A_DUP_6_MBPS
      16: HAL_PHY_RATE_11A_DUP_9_MBPS
      17: HAL_PHY_RATE_11A_DUP_12_MBPS
      18: HAL_PHY_RATE_11A_DUP_18_MBPS
      19: HAL_PHY_RATE_11A_DUP_24_MBPS
      20: HAL_PHY_RATE_11A_DUP_36_MBPS
      21: HAL_PHY_RATE_11A_DUP_48_MBPS
      22: HAL_PHY_RATE_11A_DUP_54_MBPS
      -- MCS Index #0-7 (20MHz) -- 
      23: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS
      24: HAL_PHY_RATE_MCS_1NSS_13_MBPS
      25: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS
      26: HAL_PHY_RATE_MCS_1NSS_26_MBPS
      27: HAL_PHY_RATE_MCS_1NSS_39_MBPS
      28: HAL_PHY_RATE_MCS_1NSS_52_MBPS
      29: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS
      30: HAL_PHY_RATE_MCS_1NSS_65_MBPS
      31: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS
      32: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS
      33: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS
      34: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS
      35: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS
      36: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      37: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS
      38: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS
      -- MCS Index #0-7 (40MHz) --
      39: HAL_PHY_RATE_MCS_1NSS_CB_13_5_MBPS
      40: HAL_PHY_RATE_MCS_1NSS_CB_27_MBPS
      41: HAL_PHY_RATE_MCS_1NSS_CB_40_5_MBPS
      42: HAL_PHY_RATE_MCS_1NSS_CB_54_MBPS
      43: HAL_PHY_RATE_MCS_1NSS_CB_81_MBPS
      44: HAL_PHY_RATE_MCS_1NSS_CB_108_MBPS
      45: HAL_PHY_RATE_MCS_1NSS_CB_121_5_MBPS
      46: HAL_PHY_RATE_MCS_1NSS_CB_135_MBPS
      47: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_15_MBPS
      48: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_30_MBPS
      49: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_45_MBPS
      50: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_60_MBPS
      51: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_90_MBPS
      52: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_120_MBPS
      53: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_135_MBPS
      54: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_150_MBPS
      -- 11A duplicate 80MHz Rates --
      55: HAL_PHY_RATE_11AC_DUP_6_MBPS
      56: HAL_PHY_RATE_11AC_DUP_9_MBPS
      57: HAL_PHY_RATE_11AC_DUP_12_MBPS
      58: HAL_PHY_RATE_11AC_DUP_18_MBPS
      59: HAL_PHY_RATE_11AC_DUP_24_MBPS
      60: HAL_PHY_RATE_11AC_DUP_36_MBPS
      61: HAL_PHY_RATE_11AC_DUP_48_MBPS
      62: HAL_PHY_RATE_11AC_DUP_54_MBPS
      -- 11AC rate 20MHZ NGI/SGI --
      63: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_6_5_MBPS
      64: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_13_MBPS
      65: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_19_5_MBPS
      66: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_26_MBPS
      67: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_39_MBPS
      68: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_52_MBPS
      69: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_58_5_MBPS
      70: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_65_MBPS
      71: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_78_MBPS
      72: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_86_5_MBPS
      73: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_7_2_MBPS
      74: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_14_4_MBPS
      75: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_21_6_MBPS
      76: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_28_8_MBPS
      77: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_43_3_MBPS
      78: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_57_7_MBPS
      79: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_65_MBPS
      80: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_72_2_MBPS
      81: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_86_6_MBPS
      82: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_96_1_MBPS
      -- 11AC rates 40MHZ NGI/SGI --
      83: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_13_5_MBPS 
      84: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_27_MBPS
      85: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_40_5_MBPS
      86: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_54_MBPS
      87: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_81_MBPS
      88: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_108_MBPS
      89: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_121_5_MBPS
      90: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_135_MBPS
      91: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_162_MBPS
      92: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_180_MBPS
      93: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_15_MBPS 
      94: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_30_MBPS
      95: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_45_MBPS
      96: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_60_MBPS
      97: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_90_MBPS
      98: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_120_MBPS
      99: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_135_MBPS
      100: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_150_MBPS
      101: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_180_MBPS
      102: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_200_MBPS
      -- 11AC rates 80 MHZ NGI/SGI --
      103: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_29_3_MBPS 
      104: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_58_5_MBPS
      105: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_87_8_MBPS
      106: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_117_MBPS
      107: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_175_5_MBPS
      108: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_234_MBPS
      109: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_263_3_MBPS
      110: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_292_5_MBPS
      111: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_351_MBPS
      112: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_390_MBPS
      113: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_32_5_MBPS 
      114: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_65_MBPS
      115: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_97_5_MBPS
      116: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_130_MBPS
      117: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_195_MBPS
      118: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_260_MBPS
      119: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_292_5_MBPS
      120: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_325_MBPS
      121: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_390_MBPS
      122: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_433_3_MBPS

	\endcode

   \param iRate = 			0: PHYDBG_PREAMBLE_OFDM
   						1: PHYDBG_PREAMBLE_GREENFIELD
   						2: PHYDBG_PREAMBLE_MIXED
   						3: PHYDBG_PREAMBLE_SHORTB
   						4: PHYDBG_PREAMBLE_LONGB

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_TX_FRAME_V2 
   (
      HANDLE hResourceContext,
      unsigned long iNumTestFrame,
      unsigned short iPayloadSize, 
      unsigned char iPayloadType, 
      unsigned char iPayloadFillByte, 
      unsigned long iFrameSpacing, 
      unsigned char iFCSCal, 
      unsigned long iRate, 
      unsigned long iPreamble
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_TX_FRAME_PHONE_TEST_SET_RX_TX

     Configure the WLAN ID 3660 to be used as a test device.
     When set to TX mode to test UUTs RX will count number of acks from UUT
     When set to RX mode will send acks to UUT that is transmitting


   \param iNumTestFrame = number of test frames to transmit; 0 = infinite
   \param iPayloadSize = size of payload in bytes
   \param iFrameSpacing = spacing between frames in uSec
   \param iFCSCal = true:enable CRC calulations
   \param iRate =
   \code
      ------------------------------
      ---- WCN1312/1314 values -----
      ------------------------------
      --- 802.11b Rates ---
      0: HAL_PHY_RATE_11B_LONG_1_MBPS
      1: HAL_PHY_RATE_11B_LONG_2_MBPS
      2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
      3: HAL_PHY_RATE_11B_LONG_11_MBPS
      4: HAL_PHY_RATE_11B_SHORT_2_MBPS
      5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
      6: HAL_PHY_RATE_11B_SHORT_11_MBPS
      --- SLR Rates ---
      7: HAL_PHY_RATE_SLR_0_25_MBPS
      8: HAL_PHY_RATE_SLR_0_5_MBPS
      --- Spica_Virgo 11A 20MHz Rates ---
      9: HAL_PHY_RATE_11A_6_MBPS
      10: HAL_PHY_RATE_11A_9_MBPS
      11: HAL_PHY_RATE_11A_12_MBPS
      12: HAL_PHY_RATE_11A_18_MBPS
      13: HAL_PHY_RATE_11A_24_MBPS
      14: HAL_PHY_RATE_11A_36_MBPS
      15: HAL_PHY_RATE_11A_48_MBPS
      16: HAL_PHY_RATE_11A_54_MBPS
      --- MCS Index #0-15 (20MHz) ---
      17: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS
      18: HAL_PHY_RATE_MCS_1NSS_13_MBPS
      19: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS
      20: HAL_PHY_RATE_MCS_1NSS_26_MBPS
      21: HAL_PHY_RATE_MCS_1NSS_39_MBPS
      22: HAL_PHY_RATE_MCS_1NSS_52_MBPS
      23: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS
      24: HAL_PHY_RATE_MCS_1NSS_65_MBPS
      25: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS
      26: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS
      27: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS
      28: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS
      29: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS
      30: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      31: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      32: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS
      33: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS
  
      ------------------------------
      ------  WCN3660 values -------
      ------------------------------
      --- 802.11b Rates ---
      0: HAL_PHY_RATE_11B_LONG_1_MBPS
      1: HAL_PHY_RATE_11B_LONG_2_MBPS
      2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
      3: HAL_PHY_RATE_11B_LONG_11_MBPS
      4: HAL_PHY_RATE_11B_SHORT_2_MBPS
      5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
      6: HAL_PHY_RATE_11B_SHORT_11_MBPS
      --- Spica_Virgo 11A 20MHz Rates ---
      7: HAL_PHY_RATE_11A_6_MBPS
      8: HAL_PHY_RATE_11A_9_MBPS
      9: HAL_PHY_RATE_11A_12_MBPS
      10: HAL_PHY_RATE_11A_18_MBPS
      11: HAL_PHY_RATE_11A_24_MBPS
      12: HAL_PHY_RATE_11A_36_MBPS
      13: HAL_PHY_RATE_11A_48_MBPS
      14: HAL_PHY_RATE_11A_54_MBPS
      --- 11A 20MHz Rates ---
      15: HAL_PHY_RATE_11A_DUP_6_MBPS
      16: HAL_PHY_RATE_11A_DUP_9_MBPS
      17: HAL_PHY_RATE_11A_DUP_12_MBPS
      18: HAL_PHY_RATE_11A_DUP_18_MBPS
      19: HAL_PHY_RATE_11A_DUP_24_MBPS
      20: HAL_PHY_RATE_11A_DUP_36_MBPS
      21: HAL_PHY_RATE_11A_DUP_48_MBPS
      22: HAL_PHY_RATE_11A_DUP_54_MBPS
      --- MCS Index #0-7 (20MHz) ---
      23: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS
      24: HAL_PHY_RATE_MCS_1NSS_13_MBPS
      25: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS
      26: HAL_PHY_RATE_MCS_1NSS_26_MBPS
      27: HAL_PHY_RATE_MCS_1NSS_39_MBPS
      28: HAL_PHY_RATE_MCS_1NSS_52_MBPS
      29: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS
      30: HAL_PHY_RATE_MCS_1NSS_65_MBPS
      31: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS
      32: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS
      33: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS
      34: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS
      35: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS
      36: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      37: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS
      38: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS
      --- MCS Index #0-7 (40MHz) ---
      39: HAL_PHY_RATE_MCS_1NSS_CB_13_5_MBPS
      40: HAL_PHY_RATE_MCS_1NSS_CB_27_MBPS
      41: HAL_PHY_RATE_MCS_1NSS_CB_40_5_MBPS
      42: HAL_PHY_RATE_MCS_1NSS_CB_54_MBPS
      43: HAL_PHY_RATE_MCS_1NSS_CB_81_MBPS
      44: HAL_PHY_RATE_MCS_1NSS_CB_108_MBPS
      45: HAL_PHY_RATE_MCS_1NSS_CB_121_5_MBPS
      46: HAL_PHY_RATE_MCS_1NSS_CB_135_MBPS
      47: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_15_MBPS
      48: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_30_MBPS
      49: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_45_MBPS
      50: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_60_MBPS
      51: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_90_MBPS
      52: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_120_MBPS
      53: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_135_MBPS
      54: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_150_MBPS

      ---------------------------------------------
      ------  WCN3680 NV rev 0 and 1 values -------
      ---------------------------------------------
      --- 802.11b Rates ---
      0: HAL_PHY_RATE_11B_LONG_1_MBPS
      1: HAL_PHY_RATE_11B_LONG_2_MBPS
      2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
      3: HAL_PHY_RATE_11B_LONG_11_MBPS
      4: HAL_PHY_RATE_11B_SHORT_2_MBPS
      5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
      6: HAL_PHY_RATE_11B_SHORT_11_MBPS
      -- Spica_Virgo 11A 20MHz Rates --
      7: HAL_PHY_RATE_11A_6_MBPS
      8: HAL_PHY_RATE_11A_9_MBPS
      9: HAL_PHY_RATE_11A_12_MBPS
      10: HAL_PHY_RATE_11A_18_MBPS
      11: HAL_PHY_RATE_11A_24_MBPS
      12: HAL_PHY_RATE_11A_36_MBPS
      13: HAL_PHY_RATE_11A_48_MBPS
      14: HAL_PHY_RATE_11A_54_MBPS
      -- Duplicate 11A 20MHz Rates --
      15: HAL_PHY_RATE_11A_DUP_6_MBPS
      16: HAL_PHY_RATE_11A_DUP_9_MBPS
      17: HAL_PHY_RATE_11A_DUP_12_MBPS
      18: HAL_PHY_RATE_11A_DUP_18_MBPS
      19: HAL_PHY_RATE_11A_DUP_24_MBPS
      20: HAL_PHY_RATE_11A_DUP_36_MBPS
      21: HAL_PHY_RATE_11A_DUP_48_MBPS
      22: HAL_PHY_RATE_11A_DUP_54_MBPS
      -- MCS Index #0-7 (20MHz) -- 
      23: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS
      24: HAL_PHY_RATE_MCS_1NSS_13_MBPS
      25: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS
      26: HAL_PHY_RATE_MCS_1NSS_26_MBPS
      27: HAL_PHY_RATE_MCS_1NSS_39_MBPS
      28: HAL_PHY_RATE_MCS_1NSS_52_MBPS
      29: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS
      30: HAL_PHY_RATE_MCS_1NSS_65_MBPS
      31: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS
      32: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS
      33: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS
      34: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS
      35: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS
      36: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      37: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS
      38: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS
      -- MCS Index #0-7 (40MHz) --
      39: HAL_PHY_RATE_MCS_1NSS_CB_13_5_MBPS
      40: HAL_PHY_RATE_MCS_1NSS_CB_27_MBPS
      41: HAL_PHY_RATE_MCS_1NSS_CB_40_5_MBPS
      42: HAL_PHY_RATE_MCS_1NSS_CB_54_MBPS
      43: HAL_PHY_RATE_MCS_1NSS_CB_81_MBPS
      44: HAL_PHY_RATE_MCS_1NSS_CB_108_MBPS
      45: HAL_PHY_RATE_MCS_1NSS_CB_121_5_MBPS
      46: HAL_PHY_RATE_MCS_1NSS_CB_135_MBPS
      47: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_15_MBPS
      48: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_30_MBPS
      49: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_45_MBPS
      50: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_60_MBPS
      51: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_90_MBPS
      52: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_120_MBPS
      53: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_135_MBPS
      54: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_150_MBPS
      -- 11A duplicate 80MHz Rates --
      55: HAL_PHY_RATE_11AC_DUP_6_MBPS
      56: HAL_PHY_RATE_11AC_DUP_9_MBPS
      57: HAL_PHY_RATE_11AC_DUP_12_MBPS
      58: HAL_PHY_RATE_11AC_DUP_18_MBPS
      59: HAL_PHY_RATE_11AC_DUP_24_MBPS
      60: HAL_PHY_RATE_11AC_DUP_36_MBPS
      61: HAL_PHY_RATE_11AC_DUP_48_MBPS
      62: HAL_PHY_RATE_11AC_DUP_54_MBPS
      -- 11AC rate 20MHZ NGI/SGI --
      63: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_6_5_MBPS
      64: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_13_MBPS
      65: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_19_5_MBPS
      66: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_26_MBPS
      67: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_39_MBPS
      68: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_52_MBPS
      69: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_58_5_MBPS
      70: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_65_MBPS
      71: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_78_MBPS
      72: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_7_2_MBPS
      73: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_14_4_MBPS
      74: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_21_6_MBPS
      75: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_28_8_MBPS
      76: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_43_3_MBPS
      77: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_57_7_MBPS
      78: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_65_MBPS
      79: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_72_2_MBPS
      80: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_86_6_MBPS
      -- 11AC rates 40MHZ NGI/SGI --
      81: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_13_5_MBPS 
      82: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_27_MBPS
      83: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_40_5_MBPS
      84: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_54_MBPS
      85: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_81_MBPS
      86: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_108_MBPS
      87: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_121_5_MBPS
      88: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_135_MBPS
      89: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_162_MBPS
      90: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_180_MBPS
      91: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_15_MBPS 
      92: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_30_MBPS
      93: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_45_MBPS
      94: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_60_MBPS
      95: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_90_MBPS
      96: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_120_MBPS
      97: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_135_MBPS
      98: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_150_MBPS
      99: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_180_MBPS
      100: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_200_MBPS
      -- 11AC rates 80 MHZ NGI/SGI --
      101: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_29_3_MBPS 
      102: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_58_5_MBPS
      103: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_87_8_MBPS
      104: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_117_MBPS
      105: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_175_5_MBPS
      106: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_234_MBPS
      107: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_263_3_MBPS
      108: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_292_5_MBPS
      109: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_351_MBPS
      110: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_390_MBPS
      111: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_32_5_MBPS 
      112: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_65_MBPS
      113: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_97_5_MBPS
      114: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_130_MBPS
      115: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_195_MBPS
      116: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_260_MBPS
      117: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_292_5_MBPS
      118: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_325_MBPS
      119: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_390_MBPS
      120: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_433_3_MBPS
            	
      ---------------------------------------------------
      ------  WCN3680 NV rev 2 and greater values -------
      ---------------------------------------------------
   	--- 802.11b Rates ---
      0: HAL_PHY_RATE_11B_LONG_1_MBPS
      1: HAL_PHY_RATE_11B_LONG_2_MBPS
      2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
      3: HAL_PHY_RATE_11B_LONG_11_MBPS
      4: HAL_PHY_RATE_11B_SHORT_2_MBPS
      5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
      6: HAL_PHY_RATE_11B_SHORT_11_MBPS
      -- Spica_Virgo 11A 20MHz Rates --
      7: HAL_PHY_RATE_11A_6_MBPS
      8: HAL_PHY_RATE_11A_9_MBPS
      9: HAL_PHY_RATE_11A_12_MBPS
      10: HAL_PHY_RATE_11A_18_MBPS
      11: HAL_PHY_RATE_11A_24_MBPS
      12: HAL_PHY_RATE_11A_36_MBPS
      13: HAL_PHY_RATE_11A_48_MBPS
      14: HAL_PHY_RATE_11A_54_MBPS
      -- Duplicate 11A 20MHz Rates --
      15: HAL_PHY_RATE_11A_DUP_6_MBPS
      16: HAL_PHY_RATE_11A_DUP_9_MBPS
      17: HAL_PHY_RATE_11A_DUP_12_MBPS
      18: HAL_PHY_RATE_11A_DUP_18_MBPS
      19: HAL_PHY_RATE_11A_DUP_24_MBPS
      20: HAL_PHY_RATE_11A_DUP_36_MBPS
      21: HAL_PHY_RATE_11A_DUP_48_MBPS
      22: HAL_PHY_RATE_11A_DUP_54_MBPS
      -- MCS Index #0-7 (20MHz) -- 
      23: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS
      24: HAL_PHY_RATE_MCS_1NSS_13_MBPS
      25: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS
      26: HAL_PHY_RATE_MCS_1NSS_26_MBPS
      27: HAL_PHY_RATE_MCS_1NSS_39_MBPS
      28: HAL_PHY_RATE_MCS_1NSS_52_MBPS
      29: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS
      30: HAL_PHY_RATE_MCS_1NSS_65_MBPS
      31: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS
      32: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS
      33: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS
      34: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS
      35: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS
      36: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS
      37: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS
      38: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS
      -- MCS Index #0-7 (40MHz) --
      39: HAL_PHY_RATE_MCS_1NSS_CB_13_5_MBPS
      40: HAL_PHY_RATE_MCS_1NSS_CB_27_MBPS
      41: HAL_PHY_RATE_MCS_1NSS_CB_40_5_MBPS
      42: HAL_PHY_RATE_MCS_1NSS_CB_54_MBPS
      43: HAL_PHY_RATE_MCS_1NSS_CB_81_MBPS
      44: HAL_PHY_RATE_MCS_1NSS_CB_108_MBPS
      45: HAL_PHY_RATE_MCS_1NSS_CB_121_5_MBPS
      46: HAL_PHY_RATE_MCS_1NSS_CB_135_MBPS
      47: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_15_MBPS
      48: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_30_MBPS
      49: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_45_MBPS
      50: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_60_MBPS
      51: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_90_MBPS
      52: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_120_MBPS
      53: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_135_MBPS
      54: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_150_MBPS
      -- 11A duplicate 80MHz Rates --
      55: HAL_PHY_RATE_11AC_DUP_6_MBPS
      56: HAL_PHY_RATE_11AC_DUP_9_MBPS
      57: HAL_PHY_RATE_11AC_DUP_12_MBPS
      58: HAL_PHY_RATE_11AC_DUP_18_MBPS
      59: HAL_PHY_RATE_11AC_DUP_24_MBPS
      60: HAL_PHY_RATE_11AC_DUP_36_MBPS
      61: HAL_PHY_RATE_11AC_DUP_48_MBPS
      62: HAL_PHY_RATE_11AC_DUP_54_MBPS
      -- 11AC rate 20MHZ NGI/SGI --
      63: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_6_5_MBPS
      64: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_13_MBPS
      65: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_19_5_MBPS
      66: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_26_MBPS
      67: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_39_MBPS
      68: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_52_MBPS
      69: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_58_5_MBPS
      70: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_65_MBPS
      71: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_78_MBPS
      72: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_NGI_86_5_MBPS
      73: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_7_2_MBPS
      74: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_14_4_MBPS
      75: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_21_6_MBPS
      76: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_28_8_MBPS
      77: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_43_3_MBPS
      78: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_57_7_MBPS
      79: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_65_MBPS
      80: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_72_2_MBPS
      81: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_86_6_MBPS
      82: HAL_PHY_RATE_VHT_20MHZ_MCS_1NSS_SGI_96_1_MBPS
      -- 11AC rates 40MHZ NGI/SGI --
      83: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_13_5_MBPS 
      84: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_27_MBPS
      85: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_40_5_MBPS
      86: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_54_MBPS
      87: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_81_MBPS
      88: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_108_MBPS
      89: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_121_5_MBPS
      90: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_135_MBPS
      91: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_162_MBPS
      92: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_NGI_180_MBPS
      93: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_15_MBPS 
      94: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_30_MBPS
      95: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_45_MBPS
      96: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_60_MBPS
      97: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_90_MBPS
      98: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_120_MBPS
      99: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_135_MBPS
      100: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_150_MBPS
      101: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_180_MBPS
      102: HAL_PHY_RATE_VHT_40MHZ_MCS_1NSS_CB_SGI_200_MBPS
      -- 11AC rates 80 MHZ NGI/SGI --
      103: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_29_3_MBPS 
      104: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_58_5_MBPS
      105: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_87_8_MBPS
      106: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_117_MBPS
      107: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_175_5_MBPS
      108: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_234_MBPS
      109: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_263_3_MBPS
      110: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_292_5_MBPS
      111: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_351_MBPS
      112: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_NGI_390_MBPS
      113: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_32_5_MBPS 
      114: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_65_MBPS
      115: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_97_5_MBPS
      116: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_130_MBPS
      117: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_195_MBPS
      118: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_260_MBPS
      119: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_292_5_MBPS
      120: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_325_MBPS
      121: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_390_MBPS
      122: HAL_PHY_RATE_VHT_80MHZ_MCS_1NSS_CB_SGI_433_3_MBPS

	\endcode
   \param iRate = 			0: PHYDBG_PREAMBLE_OFDM
   						1: PHYDBG_PREAMBLE_GREENFIELD
   						2: PHYDBG_PREAMBLE_MIXED
   						3: PHYDBG_PREAMBLE_SHORTB
   						4: PHYDBG_PREAMBLE_LONGB

   \param iTX_RX_mode =	1: set device to TX
   						2: sets device to RX

   \param piMacAddr1 =	MAC address 1 ie. 0x22, 0x22, 0x55, 0x55, 0x33, 0x33
   \param piMacAddr2 =	MAC address 2 ie. 0x22, 0x22, 0x55, 0x55, 0x33, 0x33
   \param piMacAddr3 =	MAC address 3 ie. 0x22, 0x22, 0x55, 0x55, 0x33, 0x33

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_TX_FRAME_PHONE_TEST_SET_RX_TX
   (
      HANDLE hResourceContext,
      unsigned long iNumTestFrame,
      unsigned short iPayloadSize,
      unsigned long iFrameSpacing,
      unsigned char iFCSCal,
      unsigned long iRate,
      unsigned long iPreamble,
      unsigned short iTX_RX_mode,
      unsigned char* piMacAddr1,
      unsigned char* piMacAddr2,
      unsigned char* piMacAddr3
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_RX_PACKET_COUNTS

     Get the number of frames received since the statistics were last reset.

   \param rxFrameCounter = pointer to variable to hold frame count.
   \param totalMacRxPackets = pointer to variable to hold total mac frame count.
   \param totalMacFcsErrPackets = pointer to variable to hold bad checksum frame count.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_RX_PACKET_COUNTS
   (
      HANDLE hResourceContext,
      unsigned long* rxFrameCounter,
      unsigned long* totalMacRxPackets,
      unsigned long* totalMacFcsErrPackets
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_RX_RSSI_VAL

     Get the RSSI values for the receive chains.

   \param rssiVals = pointer to array to hold RSSI values.
   \param numRxChains = pointer to variable to hold number of Rx chains.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_RX_RSSI_VAL
   (
      HANDLE hResourceContext,
      short* rssiVals,
      unsigned short* numRxChains
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_MAC_ADDR

     Get the MAC address.

   \param macAddr = pointer to 6 byte array to hold the MAC address.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_MAC_ADDR
   (
      HANDLE hResourceContext,
      unsigned char *macAddr
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_MAC_ADDR

     Set the MAC address.

   \param macAddr = pointer to 6 byte array holding the desired MAC address.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_MAC_ADDR
   (
      HANDLE hResourceContext,
      unsigned char* macAddr
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_MAC_ADDR_V2

     Get the MAC addresses.

   \param whichMac = Index of MAC arress to get, 0 to 3.
   \param macAddr = pointer to 6 byte array to hold the MAC address.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   \warning Only for all nv versions > 0x82 (130)
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_MAC_ADDR_V2
	(
      HANDLE hResourceContext,
		int whichMac, 
      unsigned char* macAddr
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_MAC_ADDR_V2

     Set the MAC addresses.

   \param whichMac = Index of MAC arress to set, 0 to 3.
   \param macAddr = pointer to 6 byte array holding the desired MAC address.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   \warning Only for all nv versions > 0x82 (130)
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_MAC_ADDR_V2
	(
      HANDLE hResourceContext,
		int whichMac, 
      unsigned char* macAddr
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_NV3_MAC_TAG_NAMES_V2

     Set the MAC address tag names to values other than default. These tag names
	  should match the ones in the nv xml file under the 'fields' table and are
	  used for getting and setting the mac addresses. C

   \param macTagName0 = Tag name of primary MAC (default "macAddr")
   \param macTagName1 = Tag name of primary MAC (default "macAddr2")
   \param macTagName2 = Tag name of primary MAC (default "macAddr3")
   \param macTagName3 = Tag name of primary MAC (default "macAddr4")

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   \warning Only for all nv versions > 0x82 (130)
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_NV3_MAC_TAG_NAMES_V2
	(
      HANDLE hResourceContext,
		char* macTagName0, 
		char* macTagName1, 
		char* macTagName2, 
		char* macTagName3
	);

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_RESET_RX_PACKET_STATISTICS

     Reset the received frame count.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_RESET_RX_PACKET_STATISTICS
   (
      HANDLE hResourceContext
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_WCN_ANTENNA_SELECT

     Select WLAN Antenna.

   \param antenna = 0 or 1

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_WCN_ANTENNA_SELECT
   (
      HANDLE hResourceContext,
      unsigned int iAntenna
   );

/******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS

     Enable Rx and/or Tx chains.

   \param iChain =
	\code
							4: PHY_CHAIN_SEL_R0_ON
   						5: PHY_CHAIN_SEL_R0R1_ON
   						6: PHY_CHAIN_SEL_T0_ON
   						7: PHY_CHAIN_SEL_T0_R1_ON
   						8: PHY_CHAIN_SEL_R1_ON
   						9: PHY_CHAIN_SEL_NO_RX_TX
	\endcode
   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS
   (
      HANDLE hResourceContext,
      unsigned int iChain
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GRAB_RAM

     Get samples from the 4096 sample IQ buffer.

   \param startSample = Starting sample index, 0 - 4094
   \param numSamples = Number of samples to retrieve
   \param Irx0 = Pointer to buffer to hold Rx0 I samples
   \param Qrx0 = Pointer to buffer to hold Rx0 Q samples
   \param Irx1 = Pointer to buffer to hold Rx1 I samples
   \param Qrx1 = Pointer to buffer to hold Rx1 Q samples

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GRAB_RAM
   (
      HANDLE hResourceContext,
      unsigned long startSample,
      unsigned long numSamples,
      short *Irx0,
      short *Qrx0,
      short *Irx1,
      short *Qrx1
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP

     Open or close the Tx Power Control Loop.

   \param close = 0: open 1: close

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP
   (
      HANDLE hResourceContext,
      unsigned char close
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP_V2

     Open or close the Tx Power Control Loop.

   \param close = 0: open, 1: scpc, 2: clpc

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP_V2
   (
      HANDLE hResourceContext,
      unsigned char close
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_RX_DCO_CAL

     Execute the Rx DC Offset calibration.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_RX_DCO_CAL
   (
      HANDLE hResourceContext
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_DISABLE_AGC_TABLES

     Disable/override the AGC

   \param rx0gain = Gain index (0-79) used to override the AGC
   \param rx1gain = Gain index (0-79) used to override the AGC

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_DISABLE_AGC_TABLES
   (
      HANDLE hResourceContext,
      unsigned char rx0gain,
      unsigned char rx1gain
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_STOP_WAVEFORM

     Stop the Tx waveform generator.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_STOP_WAVEFORM
   (
      HANDLE hResourceContext
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_START_WAVEFORM

     Start the Tx waveform generator.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_START_WAVEFORM
   (
      HANDLE hResourceContext
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GEN_CW

     Generate a CW waveform into the Tx waveform generator.

   \param tone = OFDM tone number (-52 to 52)
   \param ampl = IQ max amplitude (0 to 255)

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GEN_CW
   (
      HANDLE hResourceContext,
      long tone,
      unsigned long ampl
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN

     Sets the Tx gain of the waveform generator.

   \param gain = Set the waveform gain (31 to 255)

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN
   (
      HANDLE hResourceContext,
      unsigned char gain
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_STOP_RF_WAVEFORM

     Stop the Tx RF waveform generator.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_STOP_RF_WAVEFORM
   (
      HANDLE hResourceContext
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_START_RF_WAVEFORM

     Start the Tx RF waveform generator.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_START_RF_WAVEFORM
   (
      HANDLE hResourceContext
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GEN_RF_CW

     Generate an RF CW waveform into the Tx waveform generator.

   \param tone = OFDM tone number (-52 to 52)
   \param ampl = IQ max amplitude (0 to 255)

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GEN_RF_CW
   (
      HANDLE hResourceContext,
      long tone,
      unsigned long ampl
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2

     Sets the Tx gain of the waveform generator.

   \param gain = Set the RF waveform gain  - upper 16 bits - RF Gain (0-31) lower 16,  bits - digital gain (0-31)

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2
   (
      HANDLE hResourceContext,
      unsigned int gain
   );
   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_TX_POWER_REPORT

     Gets the Tx power report that can be used to calibrate CLPC or confirm CLPC
     operation.

   \param rate = Data rate index as defined in QLIB_FTM_WLAN_GEN6_SET_TX_FRAME()
   \param cbState = current Channel bonded state
   \param channelId = Current Channel Index
   \param pwrTemplateIndex = 5-bit template index used for the current rate
   \param gain = 8-bit coarse(bits 4-7) & fine(bits 0-3) gain for the current index
   \param adc = 8-bit power detector ADC sampled during the packet preamble
   \param indexMinMatchLut = minimum LUT matching power that satisfies the
                             power template index setting (7-bit value)
   \param indexMinMatchAbs = minimum LUT matching power that satisfies the
                             power template index setting in dBm
   \param indexMaxMatchLut = maximum LUT matching power that satisfies the
                             power template index setting (7-bit value)
   \param indexMaxMatchAbs = maximum LUT matching power that satisfies the
                             power template index setting in dBm
   \param outputLut = Output Power (7-bit value in the power Lookup Table)
   \param outputAbs = Ouput Power in dBm

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_TX_POWER_REPORT
   (
      HANDLE hResourceContext,
      unsigned long *rate,
      unsigned long *cbState,
      unsigned char *channelId,
      unsigned long *pwrTemplateIndex,
      unsigned char *gain,
      unsigned char *adc,
      unsigned short *rawAdc,
      unsigned char *indexMinMatchLut,
      double *indexMinMatchAbs,
      unsigned char *indexMaxMatchLut,
      double *indexMaxMatchAbs,
      unsigned char *outputLut,
      double *outputAbs
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_QUERY_TX_STATUS

     This function retrieves the number of frames generated in each mode.

   \param legacy = Legacy frame counter
   \param gfSimo20 = Green Field Singe Tx Frame counter
   \param gfMimo20 = Green Field Multi Tx Frame counter
   \param mmSimo20 = Mixed Mode Singe Tx Frame counter
   \param mmMimo20 = Mixed Mode Multi Tx Frame counter
   \param txbShort = Short Frame counter
   \param txbLong = Long Frame counter
   \param txbSlr = SLR Frame counter
   \param total = Total Frames counter
   \param txStatus = 0: counter values are invalid, 1: counter values are valid

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_QUERY_TX_STATUS
   (
      HANDLE hResourceContext,
      unsigned long *legacy,
      unsigned long *gfSimo20,
      unsigned long *gfMimo20,
      unsigned long *mmSimo20,
      unsigned long *mmMimo20,
      unsigned long *txbShort,
      unsigned long *txbLong,
      unsigned long *txbSlr,
      unsigned long *total,
      unsigned char *txStatus
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_HDET_CAL

     This function calibrates HDET

   \param hdetDcocCode = DCOC Code
   \param hdetDcoOffset = DCO offset

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_HDET_CAL
   (
      HANDLE hResourceContext,
      unsigned short *hdetDcocCode,
      unsigned short *hdetDcoOffset
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_PKT_TX_GAIN_INDEX

     This function sets the open loop packet gen Tx gain index

   \param index = Gain Index

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_PKT_TX_GAIN_INDEX
   (
      HANDLE hResourceContext,
      unsigned char index
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_FORCE_PKT_TX_GAIN

     This function forces the tx packet gen open loop gain

   \param chainNum = Tx chain (0 based)
   \param gain = Gain Value (bits 7 - 4: RF Gain, bits 3 - 0: digital gain)

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_FORCE_PKT_TX_GAIN
   (
      HANDLE hResourceContext,
      unsigned char chainNum,
      unsigned char gain
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_FORCE_PKT_TX_GAIN_V2

     This function forces the tx packet gen open loop gain

   \param chainNum = Tx chain (0 based)
   \param gain = Gain Value (bits 31 - 16: RF Gain, bits 15 - 0: digital gain)

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_FORCE_PKT_TX_GAIN_V2
   (
      HANDLE hResourceContext,
      unsigned char chainNum,
      unsigned int gain
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_TEMP_ADC

     This function gets the tempurature ADC value

   \param sensorID = 0: PA, 1: RX temp sensor
   \param adcValue = ADC Value

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_TEMP_ADC
   (
      HANDLE hResourceContext,
      int sensorID,
      unsigned short *adcValue
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_READ_XML_WRITE_NV

     This function reads an xml file containing WCN NV data and writes the
     contents to NV.

   \param fileName = full path file name of XML file containing NV data

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_READ_XML_WRITE_NV(
      HANDLE hResourceContext,
      char *fileName
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_READ_NV_WRITE_XML

     This function reads the WCN NV data and writes the contents to an xml file.

   \param fileName = full path file name of XML file to contain NV data

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_READ_NV_WRITE_XML(
      HANDLE hResourceContext,
      char *fileName
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_NV_TABLE

     This function gets the contents of a WLAN NV table

   \param tableId = 
	\code
			RATE_POWER_SETTINGS    = 2,
			REGULATORY_DOMAINS     = 3,
			DEFAULT_COUNTRY        = 4,
			TPC_POWER_TABLE        = 5,
			TPC_PDADC_OFFSETS      = 6,
			RSSI_CHANNEL_OFFSETS   = 9,
	\endcode
   \param tableData = pointer to a buffer to hold the table data

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_NV_TABLE(
      HANDLE hResourceContext,
      int tableId,
      void *tableData
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_NV_TABLE

     This function sets the contents of a WLAN NV table

   \param tableId = 
	\code
			RATE_POWER_SETTINGS    = 2,
			REGULATORY_DOMAINS     = 3,
			DEFAULT_COUNTRY        = 4,
			TPC_POWER_TABLE        = 5,
			TPC_PDADC_OFFSETS      = 6,
			RSSI_CHANNEL_OFFSETS   = 9,
	\endcode
	\param tableData = pointer to a buffer containing the table data

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_NV_TABLE(
      HANDLE hResourceContext,
      int tableId,
      void *tableData
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_READ_REGISTER

     This function sets the contents of a WLAN NV table

   \param regAddr = 32 bit register address
   \param regValue = pointer to a buffer to contain the register value

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_READ_REGISTER(
      HANDLE hResourceContext,
      unsigned int regAddr,
      unsigned int *regValue
   );


   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_UPDATE_TPC_SPLIT_LUT

     This function controls the tx power LUT

   \param powerRange: 0 = R2P (default), 1 = 11b, 2 = ofdm, 3 = fixed point
   \param powerLutIndex: 0 - 31, power = 8.5 + (index * 0.5)
   		so 0 = 8.5 dBm and 31 = 24 dBm (only meaningful if powerRange = 3)!

   \return true if successful, false if failure

   \warning This function only implemented for WCN1314
   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_UPDATE_TPC_SPLIT_LUT(
      HANDLE hResourceContext,
      int powerRange,
      unsigned int powerLutIndex
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_POWER_LUT

     This function gets the tx power LUT values

   \param txChain: 0 always for WCN1314
   \param powerLutValues: pointer to a 128 byte array to hold power LUT values

   \return true if successful, false if failure

   \warning This function only implemented for WCN1314
   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_POWER_LUT(
      HANDLE hResourceContext,
      int txChain,
      unsigned char *powerLutValues
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_BUILD_RELEASE_NUMBER

     This function gets the WLAN version information

   \param FWmaj: Firmware version major
   \param FWmin: Firmware version minor
   \param FWpatch: Firmware version patch
   \param FWbuild: Firmware version build
   \param DrvMaj: Driver version major
   \param DrvMin: Driver version minor
   \param DrvPatch: Driver version patch
   \param DrvBuild: Driver version build
   \param PttApiMaj: PttApi version major
   \param PttApiMin: PttApi version minor

   \return true if successful, false if failure

   \warning This function only implemented for WCN1314
   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_BUILD_RELEASE_NUMBER(
      HANDLE hResourceContext,
      unsigned int *FWmaj, unsigned int *FWmin, unsigned int *FWpatch, unsigned int *FWbuild,
      unsigned int *DrvMaj, unsigned int *DrvMin, unsigned int *DrvPatch, unsigned int *DrvBuild,
      unsigned int *PttApiMaj, unsigned int *PttApiMin);

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_TX_CARRIER_SUPPRESS_CAL

     This function causes the tx carrier suppression to be performed

   \param IdcoCorrection = Buffer to hold I correction value
   \param QdcoCorrection = Buffer to hold Q correction value
   \param gain = Gain Value (bits 31 - 16: RF Gain, bits 15 - 0: digital gain)

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_TX_CARRIER_SUPPRESS_CAL
   (
      HANDLE hResourceContext,
      unsigned char *IdcoCorrection,
      unsigned char *QdcoCorrection,
      unsigned int gain
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_RX_IQ_CAL_V2

     This function causes the rx iq calibration to be performed

   \param gain = Gain Value (0-15)
   \param iCoeffValues = Buffer of 5 shorts to hold I coefficient values
   \param QdcoCorrection = Buffer of 5 shorts to hold Q coefficient values

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_RX_IQ_CAL_V2
   (
      HANDLE hResourceContext,
      int gain,
      short *coeffValuesI,
      short *coeffValuesQ
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_TX_IQ_CAL_V2

     This function causes the tx iq calibration to be performed

   \param gain = Gain Value (0-31)
   \param iqAmpImbCoeff = Buffer to hold IQ Amp Imbalance Coefficient
   \param loLeakageI = Buffer to hold I Lo Leakage value
   \param loLeakageQ = Buffer to hold Q Lo Leakage value
   \param phaseImbalaceCoeffI = Buffer of 5 shorts to hold I phase imbalance coefficient values
   \param phaseImbalaceCoeffQ = Buffer of 5 shorts to hold Q phase imbalance coefficient values

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_TX_IQ_CAL_V2
   (
      HANDLE hResourceContext,
      int gain,
      short *iqAmpImbCoeff,
      short *loLeakageI,
      short *loLeakageQ,
      short *phaseImbalaceCoeffI,
      short *phaseImbalaceCoeffQ
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_CLPC_CAL_RESTORE

     This function restores state from CLPC setup used for CLPC cal of WCN3660

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_CLPC_CAL_RESTORE
   (
      HANDLE hResourceContext
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_CLPC_CAL_SETUP

     This function does internal calibrations needed for CLPC cal for WCN3660

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_CLPC_CAL_SETUP
   (
      HANDLE hResourceContext
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_CLPC_SW_CAL

     This function does a SW clpc operation. Based on the plut table, it will
     iterate 40 times using the power detecter to set tx power to the target value.

   \param txPower = Target tx poert in 0.5 db increments from min to max power
   \param txMaxPower = Max plut power value
   \param txMinPower = Min plut power value
   \param plut = 1: use plut from nv bin file, 0: use normal plut data

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_CLPC_SW_CAL
   (
      HANDLE hResourceContext,
      double txPower,
      double txMaxPower,
      double txMinPower,
      unsigned char usePlutFromBinFile
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_CLPC_CAL_EXTRA_MEASUREMENT

     This function measures exta low power points during the CLPC calibration.

   \param setup = On (1) to setup for extra points, off (0) during the measurements
   \param setupTxDmdPwrOffset = DMD power offset
   \param measureTotalExtraPt = Total number of extra points
   \param measureCurrentPtIdx = current extra point index
   \param plut = 1: use plut from nv bin file, 0: use normal plut data

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_CLPC_CAL_EXTRA_MEASUREMENT
   (
      HANDLE hResourceContext,
      unsigned short setup,
      unsigned short setupTxDmdPwrOffset,
      unsigned short measureTotalExtraPt,
      unsigned short measureCurrentPtIdx,
      unsigned char plut
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  FTM_WLAN_GEN6_GET_UNICAST_MAC_PKT_RSSI

   Get the MAC Filitered RSSI values for the receive chains.

   \param rssiVals = pointer to array to hold RSSI values.
   \param numRxChains = pointer to variable to hold number of Rx chains.

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_UNICAST_MAC_PKT_RSSI
   (
      HANDLE hResourceContext,
      short* rssiVals,
      unsigned short* numRxChains
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_UNICAST_MAC_PKT_RSSI_CONF

   Conf Rx for MAC Filtered operation.

   \param conf = On (1) to enable MAC filtering, Off (0) to disable

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_UNICAST_MAC_PKT_RSSI_CONF
   (
      HANDLE hResourceContext,
      unsigned long conf
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_ENABLE_DPD

   Conf Rx for MAC Filtered operation.

   \param enable = On (1) to enable DPD, Off (0) to disable

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_ENABLE_DPD
   (
      HANDLE hResourceContext,
      unsigned char enable
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_PWR_INDEX_SOURCE

   Set Tx Power Index Source.

   \param source = 2: FIXED_POWER_DBM, 3: REGULATORY_POWER_LIMITS

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_PWR_INDEX_SOURCE
   (
      HANDLE hResourceContext,
      unsigned long source
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GENERIC_CMD

   Set Tx Power Index Source.

   \param cmdIndx: command index
   \param param1: parameter 1
   \param param2: parameter 2
   \param param3: parameter 3
   \param param4: parameter 4

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GENERIC_CMD
   (
      HANDLE hResourceContext,
      unsigned long cmdIndx, 
	  unsigned long param1, 
	  unsigned long param2, 
	  unsigned long param3, 
	  unsigned long param4
   );


   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_NV_VERSION

     Get the NV version.

   \param version = NV version number

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_NV_VERSION
   (
      HANDLE hResourceContext,
      unsigned char *version
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_NV3_GET_ENUM_VALUE_OFFLINE

     Get the name and value of the indexed enumeration from a binary NV file.

   \param enumFieldIndex =  index of enumeration
   \param enumItemIndex = index of enumeration item
   \param inputBinFileName = full path name of NV binary file
   \param dictionaryFileName = full path name of NV dictionary file
   \param enumName = string containing the name of the indexed enumeration
   \param enumValue = value of the indexed enumeration

   \return true if successful, false if failure

   *******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_NV3_GET_ENUM_VALUE_OFFLINE
	(
		int enumFieldIndex, 
		int enumItemIndex, 
		char *inputBinFileName, 
		char *dictionaryFileName, 
		char *enumName, 
		int *enumValue
	);

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_NV3_GET_ENUM_VALUE

     Get the name and value of the indexed enumeration.

   \param enumFieldIndex =  index of enumeration
   \param enumItemIndex = index of enumeration item
   \param inputBinFileName = full path name of NV binary file
   \param dictionaryFileName = full path name of NV dictionary file
   \param enumName = string containing the name of the indexed enumeration
   \param enumValue = value of the indexed enumeration

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_NV3_GET_ENUM_VALUE
	(
      HANDLE hResourceContext,
		int enumFieldIndex, 
		int enumItemIndex, 
		char *enumName, 
		int *enumValue
	);
   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_READ_XML_WRITE_NVBIN

   Read an XML file containing WLAN NV items and write the contents to an NV 
   binary file.

   \param xmlfileName = File path to XML file to read
   \param binfileName = File path to NV Bin file to write
   \param dictionaryfileName = File path to NV dictionary file

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_READ_XML_WRITE_NVBIN
   (
      char *xmlfileName, 
      char *binfileName, 
      char *dictionaryfileName
   );

   /******************************************************************************/
   /**

   GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_READ_NVBIN_WRITE_XML

   Read an NV binary file containing WLAN NV items and write the contents to an  
   XML file.

   \param xmlfileName = File path to XML file to write
   \param binfileName = File path to NV Bin file to read
   \param dictionaryfileName = File path to NV dictionary file

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_READ_NVBIN_WRITE_XML
   (
      char *xmlfileName, 
      char *binfileName, 
      char *dictionaryfileName
   );

	/******************************************************************************/
	/**

	GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_NV_VERS3_BIN_TO_FILE

	Read the version 3 nv bin file into a file.

	\param fileName = Full path file name of bin file to create

	\return true if successful, false if failure

	\warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
	*******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_NV_VERS3_BIN_TO_FILE
   (
      HANDLE hResourceContext,
      char *fileName
   );

	/******************************************************************************/
	/**

	GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_SET_NV_VERS3_BIN_FROM_FILE

	Send the version 3 nv bin file from a file. 

	\param fileName = Full path file name of bin file to read

	\return true if successful, false if failure

	\warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
	*******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_NV_VERS3_BIN_FROM_FILE
   (
      HANDLE hResourceContext,
		char *fileName
   );

	/******************************************************************************/
	/**

	GEN6 WLAN Commands:  QLIB_FTM_WLAN_GEN6_GET_NV_VERS3_TEMPLATE_TO_FILE

	Read the version 3 nv template file into a file.

	\param fileName = Full path file name of template file to create 

	\return true if successful, false if failure

	\warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
	*******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_NV_VERS3_TEMPLATE_TO_FILE
   (
      HANDLE hResourceContext,
		char *fileName
   );
	/******************************************************************************/
	/**

	GEN6 WLAN Commands:  QLIB_FTM_SET_TX_IQ_CORRECT

	Set the Tx path for IQ corrections

	\param gainVal = variable to store Tx gain value
	\param iqAmpImbCoeff = variable to store IQ amplitude imbalance coefficient
	\param txLoLeakageI = variable to store 'I' value for Tx LO leakage
	\param txLoLeakageQ = variable to store 'Q' value for Tx LO leakage
	\param iqPhasenImbCoeffI = pointer to array to hold 5 short 'I' values for IQ phase imbalance coefficient
	\param iqPhasenImbCoeffQ = pointer to array to hold 5 short 'Q' values for IQ phase imbalance coefficient

	\return true if successful, false if failure

	\warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_SET_TX_IQ_CORRECT
	(
	   HANDLE hResourceContext,
	   int gainVal,
	   short iqAmpImbCoeff,
	   short txLoLeakageI,
	   short txLoLeakageQ,
	   short *iqPhasenImbCoeffI,
	   short *iqPhasenImbCoeffQ
	);
	/******************************************************************************/
	/**

	GEN6 WLAN Commands: QLIB_FTM_GET_TX_IQ_CORRECT

	Get the IQ correction coefficients for Tx path

	\param gainVal = variable to store Tx gain value
	\param iqAmpImbCoeffOut = pointer to variable to hold short value for IQ amplitude imbalance coefficient
	\param txLoLeakageIOut =  pointer to variable to hold short 'I' value for Tx LO leakage
	\param txLoLeakageQOut =  pointer to variable to hold short 'Q' value for Tx LO leakage
	\param iqPhasenImbCoeffIOut = pointer to array to hold 5 short 'I' values for IQ phase imbalance coefficient
	\param iqPhasenImbCoeffQOut = pointer to array to hold 5 short 'Q' values for IQ phase imbalance coefficient

	\return true if successful, false if failure

	\warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_TX_IQ_CORRECT
	(
	   HANDLE hResourceContext,
	   int gainVal,
	   short *iqAmpImbCoeffOut,
	   short *txLoLeakageIOut,
	   short *txLoLeakageQOut,
	   short *iqPhasenImbCoeffIOut,
	   short *iqPhasenImbCoeffQOut
	);
   /******************************************************************************/
	/**

	GEN6 WLAN Commands:  QLIB_FTM_SET_RX_DCO_CORRECT

	Set the Rx path for DCO corrections

	\param gainVal = variable to store Rx gain value
	\param dcoCorrectI = variable to store 'I' value of Rx DCO correction
	\param dcoCorrectQ = variable to store 'Q' value of Rx DCO correction 
	\param dcRange = variable to store dc range

	\return true if successful, false if failure

	\warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
	*******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_RX_DCO_CORRECT
   (
		HANDLE hResourceContext,
		unsigned char gainVal,
		unsigned char dcoCorrectI,
		unsigned char dcoCorrectQ,
		unsigned char dcRange
   );
   /******************************************************************************/
	/**

	GEN6 WLAN Commands:  QLIB_FTM_GET_RX_DCO_CORRECT

	Get the DC Offset corrections for Rx path

	\param gainVal = variable to store Rx gain value
	\param dcoCorrectIOut = pointer to variable to hold 'I' value of Rx DCO correction
	\param dcoCorrectQOut = pointer to variable to hold 'Q' value of Rx DCO correction 
	\param dcRangeOut = pointer to variable to hold dc range

	\return true if successful, false if failure

	\warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_RX_DCO_CORRECT
	  (
			HANDLE hResourceContext,
			unsigned char gainVal,
			unsigned char *dcoCorrectIOut,
			unsigned char *dcoCorrectQOut,
			unsigned char *dcRangeOut
	   );
	/******************************************************************************/
	/**

	GEN6 WLAN Commands: QLIB_FTM_SET_RX_IQ_CORRECT

	Set the IQ Calibration values for Rx Path

	\param gainVal = variable to store Rx gain value
	\param iqCalValuesI = pointer to array to hold 5 short 'I' values for the IQ calibration
	\param iqCalValuesQ = pointer to array to hold 5 short 'Q' values for the IQ calibration 

	\return true if successful, false if failure

	\warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_SET_RX_IQ_CORRECT
	(
	   HANDLE hResourceContext,
	   int gainVal,
	   short *iqCalValuesI,
	   short *iqCalValuesQ    
	);
	/******************************************************************************/
	/**

	GEN6 WLAN Commands: QLIB_FTM_GET_RX_IQ_CORRECT

	Get the IQ Calibration values for Rx Path

	\param gainVal = variable to store Rx gain value
	\param iqCalValuesIOut = pointer to array to hold 5 short 'I' values for the IQ calibration
	\param iqCalValuesQOut = pointer to array to hold 5 short 'Q' values for the IQ calibration 

	\return true if successful, false if failure

	\warning QLIB_FTM_WLAN_GEN6_START() must be called first to start WLAN test mode
	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_GET_RX_IQ_CORRECT
	(
	   HANDLE hResourceContext,
	   int gainVal,
	   short *iqCalValuesIOut,
	   short *iqCalValuesQOut    
	);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif	// defined(_QLIB_WLAN_WCN_H)
