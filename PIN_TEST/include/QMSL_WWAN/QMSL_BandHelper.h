/******************************************************************************
 * $File: //depot/HTE/QDART/QMSL6/inc/QMSL_WWAN/QMSL_BandHelper.h $
 *
 * DESCRIPTION: QMSL_Bandhelper
 ******************************************************************************
 *
 * Copyright (c) 2014 QUALCOMM Technologies Incorporated.
 * All rights reserved.
 * Qualcomm Confidential and Proprietary
 *
 ******************************************************************************
 *
 * $Header: //depot/HTE/QDART/QMSL6/inc/QMSL_WWAN/QMSL_BandHelper.h#3 $
 * $DateTime: 2014/01/15 10:29:33 $
 *
 */
#if !defined(_QLIB_BandHelper_H)
#define _QLIB_BandHelper_H

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
                     Band Class Helper
   *******************************************************************************/

   /******************************************************************************/
   /**
      Calculate the downlink (forward) frequency

      \param iBand, Band class enumeration.  See BAND_CLASS_ENUM in QLib_Defines.h

      \param iChannel, downlink channel number

      \return frequencey in MHz.  0 is returned for invalid band class or channel number

   *******************************************************************************/
   QLIB_API double QLIB_Band_Helper_CalculateDLFrequency(unsigned short iBand, unsigned long iChannel);

   /******************************************************************************/
   /**
      Calculate the uplink (reverse) frequency

      \param iBand, Band class enumeration.  See BAND_CLASS_ENUM in QLib_Defines.h

      \param iChannel, uplink channel number

      \return frequencey in MHz.  0 is returned for invalid band class or channel number

   *******************************************************************************/
   QLIB_API double QLIB_Band_Helper_CalculateULFrequency(unsigned short iBand, unsigned long iChannel);



#ifdef __cplusplus
}	// extern "C"
#endif

#endif	// defined(_QLIB_FBandHelper_H)
