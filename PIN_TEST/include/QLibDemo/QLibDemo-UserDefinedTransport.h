/******************************************************************************
 * $Header$
 * $DateTime$
 *
 * DESCRIPTION: Definition of a user defined transport layer which uses the windows serial port driver
 ******************************************************************************
 *
 * Copyright (c) 2014-2015 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#ifndef _QLIBDEMO_USERDEFINEDTRANSPORT_H_ 
#define _QLIBDEMO_USERDEFINEDTRANSPORT_H_ 

#include <windows.h>


/**
   Call back for user-defined send (PC to mobile) function.  
   To be used with QLIB_ConnectServer_UserDefinedTransport()


   \param hQMSL_ContextID = the QMSL that is associated with the connection. 
   \param hUserContextID = the context ID that was passed to QLIB_ConnectServer_UserDefinedTransport()
   \param iRequestSize = Number of bytes to be sent in the request packet.
   \param piRequestBytes = Pointer to a buffer of request packet contents.
   \param piActualWriteSize = to be updated by the user function, the actual # of bytes that were written

   \return 0 if successful (ERROR_SUCCESS) otherwise, an error code as defined by WINERR.H error codes

     http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/system_error_codes__0-499_.asp
*/
unsigned long UserDefinedSend
(
   HANDLE hQMSL_ContextID,
   HANDLE hUserContextID,
   unsigned long iRequestSize,
   unsigned char* piRequestBytes,
   unsigned long* piActualWriteSize
);


/**
   Call back for user-defined receive (from mobile to PC) function.  
   To be used with QLIB_ConnectServer_UserDefinedTransport()

   \param hQMSL_ContextID = the QMSL that is associated with the connection. 
   \param hUserContextID = the context ID that was passed to QLIB_ConnectServer_UserDefinedTransport()
   \param iRequestSize = Number of bytes to be sent in the request packet.
   \param piRequestBytes = Pointer to a buffer of request packet contents.
   \param piResponseSize = Pointer to number of bytes received in the response packet
                     NOTE: when calling this function, the value must be equal to the
                     maximum size of the recieve buffer.  When the function returns, the
                     value will be the actual number of bytes filled into the receive buffer.
   \param iResponseBytes = Pointer to a buffer to store the response packet contents.

   \return 0 if successful (ERROR_SUCCESS) otherwise, an error code as defined by WINERR.H error codes

     http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/system_error_codes__0-499_.asp

*/
unsigned long UserDefinedReceive  
(
   HANDLE hQMSL_ContextID,
   HANDLE hUserContextID,
   unsigned long* piResponseSize,
   unsigned char* piResponseBytes
);

/**
   Call back for user-defined function to flush Rx buffer.

   \param hQMSL_ContextID = the QMSL that is associated with the connection. 
   \param hUserContextID = the context ID that was passed to QLIB_ConnectServer_UserDefinedTransport()
   \return 0 if successful (ERROR_SUCCESS) otherwise, an error code as defined by WINERR.H error codes

     http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/system_error_codes__0-499_.asp

*/
unsigned long UserDefinedFlushTxRx
(
   HANDLE hQMSL_ContextID,
   HANDLE hUserContextID

);

/**
   Open the user defined transport resources

   \param iComPort = COM port number

   \return 0 if successful (ERROR_SUCCESS) otherwise, an error code as defined by WINERR.H error codes

     http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/system_error_codes__0-499_.asp
*/
unsigned long OpenUserDefinedTransport( int iComPort );

/**
   Close the user defined transport resources

*/
void CloseUserDefinedTransport( void );


#endif // #ifndef _QLIBDEMO_USERDEFINEDTRANSPORT_H_ 
