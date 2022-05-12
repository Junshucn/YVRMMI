/******************************************************************************/
/**
Program: QMSL_Connectivity

	$Id: //depot/HTE/QDART/QMSL6/inc/QMSL_Connectivity/QMSL_Connectivity.h#4 $

\code

	- Defines the "C" interface for all Factory Library functions.

	- Table of contents (text search can be done for the following sections)

			- General Information
			- Related Documents
			- Version History

			- WLAN FTM
			- Bluetooth FTM
			- FM FTM (FM Broadcast Radio)
			- Audio FTM

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
#include "QLib/QMSL_Connectivity/QMSL_Connectivity_WLAN/QMSL_Connectivity_WLAN_WCN.h"
#include "QLib/QMSL_Connectivity/QMSL_Connectivity_WLAN/QMSL_Connectivity_WLAN_Atheros.h"
#include "QLib/QMSL_Connectivity/QMSL_Connectivity_WLAN/QMSL_Connectivity_WLAN_Philips.h"
#include "QLib/QMSL_Connectivity/QMSL_Connectivity_BT/QMSL_Connectivity_Bluetooth.h"
#include "QLib/QMSL_Connectivity/QMSL_Connectivity_FM/QMSL_Connectivity_FM.h"
#include "QLib/QMSL_Connectivity/QMSL_Connectivity_ANT/QMSL_Connectivity_ANT.h"
#include "QLib/QMSL_Connectivity/QMSL_Connectivity_NFC/QMSL_Connectivity_NFC.h"