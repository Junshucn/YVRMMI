/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Connectivity/inc/QMSL_Connectivity_WPAN15p4/QMSL_Connectivity_WPAN15p4.h#3 $
 * $DateTime: 2017/01/30 16:03:57 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#if !defined(_QLIB_WPAN15p4_H)
#define _QLIB_WPAN15p4_H

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
                     WPAN15p4 FTM
   *******************************************************************************/   


    /******************************************************************************/ 
   /**  
   Enables the WPAN15p4 HCI event logging mode for QRCT , logCode = 0x19AB

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
    
   \return true if successful, false if fail.

   \warning QLIB_FTM_SET_MODE( PHONE_MODE_WPAN15p4 ) must be called first

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_EnableLogging
   ( HANDLE hResourceContext);  

   /******************************************************************************/
   /**
   Factory Test Mode WPAN15p4 Commands 

   Send raw byte stream to a device

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param pBuffer = pointer to a series of bytes that will be sent as an HCI command.
   \param iLength = number of bytes to send, from pBuffer

   \return true if successful, false if fail.

   \warning QLIB_FTM_SET_MODE( PHONE_MODE_WPAN15p4 ) must be called first

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_HCI_USER_CMD
   ( HANDLE hResourceContext, unsigned char* pBuffer, unsigned char iLength);

    

    /******************************************************************************/
   /**

   This command issues a enabled WPAN15p4 HCI event logging for 0x19AB log code       

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()   
   \param pWPAN15p4HCI_Logs
        1. First byte number of HCI logs
        2. Next byte = length of log payload, followed by payload bytes
          3. Step 2 continues till number of logs are not complete
    \param iNumberOfBytes
        Total number of received bytes

   \return true if successful, false if fail.
  

   \warning Will reset the log code and logging state

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_GetHCILogs( HANDLE hResourceContext, unsigned char* pWPAN15p4HCI_Logs , unsigned short* iNumberOfBytes);
   

   /******************************************************************************/
   /**

   This function sends WPAN15p4 HCI command and returns the first HCI event/response received

   Call QLIB_ConfigureTimeout( hResourceContext, QMSL_Timeout_WPAN15p4_HCI_Response, TimeOut) to configure the timeout value for waiting for WPAN15p4 response

   See Test_FTM_WPAN15p4() in QLibDemo.cpp on how to use this function

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param pHCI_Buffer [in] = HCI command buffer

   \param iHCI_Length [in] = HCI command buffer length

   \param pWPAN15p4ResponseBuffer [in/out] = HCI response buffer

   \param iWPAN15p4ResponseLength [in/out] = [in] is the size of pWPAN15p4ResponseBuffer,
                              [out] is the length of valid data returned by pWPAN15p4ResponseBuffer, 0 means that there is no WPAN15p4 response

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_HCI_USER_CMD_WithEventResponse( HANDLE hResourceContext, unsigned char* pHCI_Buffer, unsigned short iHCI_Length, unsigned char* pWPAN15p4ResponseBuffer, unsigned short* iWPAN15p4ResponseLength );
   /******************************************************************************/
   /**

   This function sends WPAN15p4 HCI command and returns the first and second HCI event/response received

   Call QLIB_ConfigureTimeout( hResourceContext, QMSL_Timeout_WPAN15p4_HCI_Response, TimeOut) to configure the timeout value for waiting for WPAN15p4 response

   See Test_FTM_WPAN15p4() in QLibDemo.cpp on how to use this function

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param pHCI_Buffer [in] = HCI command buffer

   \param iHCI_Length [in] = HCI command buffer length

   \param pWPAN15p4ResponseBuffer [in/out] = HCI response buffer

   \param iWPAN15p4ResponseLength [in/out] = [in] is the size of pWPAN15p4ResponseBuffer,
                              [out] is the length of valid data returned by pWPAN15p4ResponseBuffer, 0 means that there is no WPAN15p4 response

   \param pSecondWPAN15p4ResponseBuffer2 [in/out] = Second HCI response buffer

   \param iSecondWPAN15p4ResponseLength2 [in/out] = [in] is the size of pSecondWPAN15p4ResponseBuffer,
                              [out] is the length of valid data returned by pSecondWPAN15p4ResponseBuffer, 0 means that there is no WPAN15p4 response

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_HCI_USER_CMD_WithTwoEventResponse( HANDLE hResourceContext, unsigned char* pHCI_Buffer, unsigned short iHCI_Length, unsigned char* pWPAN15p4ResponseBuffer, unsigned short* iWPAN15p4ResponseLength, unsigned char* pSecondWPAN15p4ResponseBuffer2, unsigned short* iSecondWPAN15p4ResponseLength );

   /******************************************************************************/
   /**

   This function sends WPAN15p4 HCI command and returns the first and second HCI event/response received

   Call QLIB_ConfigureTimeout( hResourceContext, QMSL_Timeout_WPAN15p4_HCI_Response, TimeOut) to configure the timeout value for waiting for WPAN15p4 response

   See Test_FTM_WPAN15p4() in QLibDemo.cpp on how to use this function

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param pHCI_Buffer [in] = HCI command buffer

   \param iHCI_Length [in] = HCI command buffer length

   \param pWPAN15p4ResponseBuffer [in/out] = HCI response buffer

   \param iWPAN15p4ResponseLength [in/out] = [in] is the size of pWPAN15p4ResponseBuffer,
                              [out] is the length of valid data returned by pWPAN15p4ResponseBuffer, 0 means that there is no WPAN15p4 response

   \param pSecondWPAN15p4ResponseBuffer2 [in/out] = Second HCI response buffer

   \param iSecondWPAN15p4ResponseLength2 [in/out] = [in] is the size of pSecondWPAN15p4ResponseBuffer,
                              [out] is the length of valid data returned by pSecondWPAN15p4ResponseBuffer, 0 means that there is no WPAN15p4 response

   \param pThirdWPAN15p4ResponseBuffer3 [in/out] = Third HCI response buffer

   \param iThirdWPAN15p4ResponseLength3 [in/out] = [in] is the size of pThirdWPAN15p4ResponseBuffer,
                              [out] is the length of valid data returned by pThirdWPAN15p4ResponseBuffer, 0 means that there is no WPAN15p4 response


   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_HCI_USER_CMD_WithThreeEventResponse( HANDLE hResourceContext, unsigned char* pHCI_Buffer, unsigned short iHCI_Length, unsigned char* pWPAN15p4ResponseBuffer, unsigned short* iWPAN15p4ResponseLength,unsigned char* pSecondWPAN15p4ResponseBuffer, unsigned short* iSecondWPAN15p4ResponseLength, unsigned char* pThirdWPAN15p4ResponseBuffer, unsigned short* iThirdWPAN15p4ResponseLength  );

   /******************************************************************************/
   /**

   This function will reset PIBS and PHY of WPAN15p4 controller.  This method should be utilized to reset PIBs and PHY.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_MLME_Reset(HANDLE hResourceContext);

   /******************************************************************************/
   /**

   This function will enable DUT(device under test) mode of WPAN15p4 .  This method is required before any TX/RX tests

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_VS_DUT_ENABLE(HANDLE hResourceContext);

    /******************************************************************************/
   /**

   This function will end TX/RX modes of WPAN15p4 controller.  

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_VS_DUT_TEST_END(HANDLE hResourceContext);

    /******************************************************************************/
   /**

   This function will enable DUT TX mode with various input parameters.  

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_VS_DUT_TX_TEST(HANDLE hResourceContext,
	unsigned char iflags,
	unsigned char itransmitMode,		 
    unsigned char itransmitChannel,
    unsigned char itransmitPower,
    unsigned short ipayloadLength,
    unsigned char ipacketType);

   
    /******************************************************************************/
   /**

   This function will enable DUT RX mode with various input parameters.  

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_VS_DUT_RX_TEST(HANDLE hResourceContext,
	unsigned char iflags,
	unsigned char ireceiveMode,		 
    unsigned char ireceiveChannel,    
    unsigned short ipayloadLength,
    unsigned char ipacketType);

   /******************************************************************************/
   /**

   This function will get RX stats for RX tests like packets received, packets in error and BITs in error.  

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_VS_DUT_RX_STATS(HANDLE hResourceContext,unsigned long* packetsReceived,unsigned long* packetsInError,unsigned long* bitsInError);
   
    /******************************************************************************/
   /**

   This method downloads the nvm from the desired location.
  
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param invmFileLocation [in] = the nvm file location for downloading

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WPAN15p4_NVM_DOWNLOAD(HANDLE hResourceContext,
      char *invmFileLocation);
#ifdef __cplusplus
}   // extern "C"
#endif

#endif   // defined(_QLIB_WPAN15p4_H)
