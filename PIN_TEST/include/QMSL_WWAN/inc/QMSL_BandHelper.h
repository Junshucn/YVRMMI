/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_WWAN/inc/QMSL_BandHelper.h#6 $
 * $DateTime: 2017/07/05 15:35:28 $
 *
 * DESCRIPTION: QMSL_Bandhelper
 ******************************************************************************
 *
 * Copyright (c) 2014-2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
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

   /******************************************************************************/
   /**
      Calculate the downlink (reverse) channel number

      \param iBand, Band class enumeration.  See BAND_CLASS_ENUM in QLib_Defines.h

      \param iUpLinkd_Channel, uplink channel number

      \return Downlink channel number.  0 is returned for invalid band class or uplink channel number

   *******************************************************************************/
   QLIB_API unsigned long QLIB_Band_Helper_CalculateDLChannel(unsigned short iBand, unsigned long iUpLink_Channel);

   /******************************************************************************/
   /**
      Calculate the UL channel number according to band and bandwidth

      \param iBand, Band class enumeration.  See BAND_CLASS_ENUM in QLib_Defines.h

      \param iRange, Channel range enumeration.  See CHANNEL_RANGE_ENUM in QLib_Defines.h

      \param bandwidth, channel bandwidth, only required for LTE

      \return Uplink channel number. 65535 is returned for invalid band class or uplink channel number

   *******************************************************************************/
   QLIB_API unsigned long QLIB_Band_Helper_CalculateULChanBW(unsigned short usBand, unsigned int iRange, unsigned int iChBW);

   /******************************************************************************/
   /**
      Calculate the DL channel number according to band and bandwidth

      \param iBand, Band class enumeration.  See BAND_CLASS_ENUM in QLib_Defines.h

      \param iRange, Channel range enumeration.  See CHANNEL_RANGE_ENUM in QLib_Defines.h

      \param bandwidth, channel bandwidth, only required for LTE

      \return Uplink channel number. 65535 is returned for invalid band class or uplink channel number

   *******************************************************************************/
   QLIB_API unsigned long QLIB_Band_Helper_CalculateDLChanBW(unsigned short usBand, unsigned int iRange, unsigned int iChBW);



#ifdef __cplusplus
}   // extern "C"
#endif

#endif   // defined(_QLIB_FBandHelper_H)
