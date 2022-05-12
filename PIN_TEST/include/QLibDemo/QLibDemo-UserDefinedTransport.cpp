/******************************************************************************
 * $Header$
 * $DateTime$
 *
 * DESCRIPTION: Implementation of a user defined transport layer which uses the windows serial port driver
 ******************************************************************************
 *
 * Copyright (c) 2014-2015 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#include "QLibDemo-UserDefinedTransport.h"

#include <QLib.h>
#include <windows.h>
#include <stdio.h>

// Static globals for use in the functions in this file
HANDLE g_hCOM_Handle = NULL;
BOOL g_bIsConnected = false;


#define QRXQUEUE 4096
#define QTXQUEUE 4096

#define COMPORT_MIN     ((int) 1)
#define COMPORT_MAX     ((int) 255)


//===========================================================================
// Valid Parity Option:
//       EVENPARITY, MARKPARITY, NOPARITY, ODDPARITY

// Valid StopBits Option:
//       ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS
//
// Valid ByteSize Option:
//       4-8
//===========================================================================
const int NOCHANGE = -1;
const int FOREVER = -1;
const int DISABLE = 0;
const int CLEAR = 0;
const int ENABLE = 1;
const int SET = 1;
const int RESET = 1;
const int REMOTE_CONTROL = -1;


///////////////////////////////////////////////////////////
unsigned long UserDefinedReceive  
(
   HANDLE hQMSL_ContextID,
   HANDLE hUserContextID,
   unsigned long* piResponseSize,
   unsigned char* piResponseBytes
)
{
   unsigned long _iActualResponseSize = 0;

   if ( g_bIsConnected )
   {
      // Clear out errors
      ::SetLastError( ERROR_SUCCESS );

      if(::ReadFile(g_hCOM_Handle, piResponseBytes, *piResponseSize, &_iActualResponseSize, NULL /*&overlapped*/))
      
      // Assign response size to the input pointer
      *piResponseSize = _iActualResponseSize;

      // For debug, will print an R for each time data is received
      if ( _iActualResponseSize > 0 )
         printf("r");

      return ::GetLastError();
   }   // if ( g_bIsConnected )

   return ERROR_SERIAL_NO_DEVICE;

}

///////////////////////////////////////////////////////////
unsigned long UserDefinedSend
(
   HANDLE hQMSL_ContextID,
   HANDLE hUserContextID,
   unsigned long iRequestSize,
   unsigned char* piRequestBytes,
   unsigned long* piActualWriteSize
)
{
   if ( g_bIsConnected )
   {
      // Clear out errors
      ::SetLastError( ERROR_SUCCESS );

      ::WriteFile(g_hCOM_Handle, piRequestBytes, iRequestSize, piActualWriteSize, 0/* &overlapped*/);

      // For debug, will print an w for each time data is written
      printf("w");

      return ::GetLastError();
   }   // if ( g_bIsConnected )

   return ERROR_SERIAL_NO_DEVICE;

}


///////////////////////////////////////////////////////////
unsigned long UserDefinedFlushTxRx
(
   HANDLE hQMSL_ContextID,
   HANDLE hUserContextID

)
{
   if ( g_bIsConnected )
   {
      // Clear out errors
      ::SetLastError( ERROR_SUCCESS );

      ::PurgeComm(g_hCOM_Handle, PURGE_TXCLEAR | PURGE_RXCLEAR);

      return ::GetLastError();
   }   // if ( g_bIsConnected )

   return ERROR_SERIAL_NO_DEVICE;
}


///////////////////////////////////////////////////////////
unsigned long OpenUserDefinedTransport( int iComPort )
{
    char szcommname[10];
   DCB dcb;
   COMMTIMEOUTS to;
   COMMPROP cp;

   // Defaults for RS-232 and USB
   LONG iBaudRate = 115200;
   int parity = NOPARITY;
   int byte_size = 8;
   int stop_bits = ONESTOPBIT;
   int dtr = SET;
   int rts = SET;

   unsigned long error_status = ERROR_SUCCESS;

    // Close previous connection
   CloseUserDefinedTransport();

    wsprintf(szcommname, "\\\\.\\COM%d", iComPort);

   try
   {
      g_hCOM_Handle = ::CreateFile( szcommname,GENERIC_READ | GENERIC_WRITE, 0, NULL,
                      OPEN_EXISTING,0 /*FILE_FLAG_OVERLAPPED*/, NULL);
   }
   catch(...)
   {
      g_hCOM_Handle = INVALID_HANDLE_VALUE;
   }

    if(g_hCOM_Handle == INVALID_HANDLE_VALUE) 
    {
       return ::GetLastError();;
    }

   // Set this flag, now that the port has been opened
    g_bIsConnected = TRUE;                                 // Set connected flag

   if(!::SetupComm(g_hCOM_Handle,(DWORD)QRXQUEUE,(DWORD)QTXQUEUE))
    {
      error_status = ::GetLastError();;
      CloseUserDefinedTransport();
      return error_status;
    }
    
    cp.wPacketLength = sizeof(COMMPROP);
    ::GetCommProperties(g_hCOM_Handle, &cp);

    if(!(cp.dwProvCapabilities & PCF_INTTIMEOUTS))
    {
      // Some USB, do not set this bit, so it is not being checked
    }

    memset(&to,0,sizeof(to));

   // Get the curren timeout status
   ::GetCommTimeouts(g_hCOM_Handle, &to);

   // Initialize time-out values, copied from the QPHONEMS implementation for QMSL
    to.ReadIntervalTimeout = MAXDWORD;
   to.ReadTotalTimeoutConstant = 0;
   to.ReadTotalTimeoutMultiplier = 0;
   to.ReadIntervalTimeout        = MAXDWORD;
   to.ReadTotalTimeoutMultiplier = 0;      
   to.WriteTotalTimeoutMultiplier = 1 ;    // 1 mS per byte
   to.WriteTotalTimeoutConstant = 50 ;     // plus 50 mS

   // Set the timeout values
    ::SetCommTimeouts(g_hCOM_Handle, &to);

   // Get DCB state
   ::GetCommState(g_hCOM_Handle, &dcb);

   dcb.fOutxCtsFlow = FALSE;
   dcb.fOutxDsrFlow = FALSE;
   dcb.fDsrSensitivity = FALSE;
   dcb.fRtsControl = RTS_CONTROL_ENABLE;
   dcb.fDtrControl = DTR_CONTROL_DISABLE;
   dcb.fDsrSensitivity = FALSE;
   dcb.fTXContinueOnXoff = TRUE;
   dcb.fOutX = FALSE;
   dcb.fInX = FALSE;
   dcb.fErrorChar = FALSE;
   dcb.fNull = FALSE;
   dcb.fAbortOnError = FALSE;
   dcb.XonLim = 0;
   dcb.XoffLim = 0;
   dcb.EofChar = 0;

   
   dcb.fBinary = TRUE;
    dcb.fParity = TRUE;
    dcb.fNull = FALSE;
    
   dcb.BaudRate = (DWORD) iBaudRate;
   dcb.ByteSize = (BYTE) byte_size;
   dcb.Parity = (BYTE) parity;
   dcb.StopBits = (BYTE) stop_bits;

   if (!::SetCommState(g_hCOM_Handle, &dcb) )                  // Set the control block
   {
      error_status = ::GetLastError();;
      CloseUserDefinedTransport();
      return error_status;
   }


   ::EscapeCommFunction(g_hCOM_Handle, SETRTS);               // Set RTS high
   ::EscapeCommFunction(g_hCOM_Handle, CLRDTR);               // Set DTR Low

   ::PurgeComm(g_hCOM_Handle, PURGE_TXCLEAR | PURGE_RXCLEAR);      // Flush the channel


    return ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////
void CloseUserDefinedTransport( void )
{
    if (g_bIsConnected)
    {

         g_bIsConnected = FALSE;
      ::CloseHandle(g_hCOM_Handle);
    }
}
