/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/QLibDemo.cpp#63 $
 * $DateTime$
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2014-2017 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // For _O_BINARY
#include <math.h>   // for log10()
#include <iostream>
#include <string>

using namespace std;

#define _USE_MATH_DEFINES // for C++

// Disable compiler waring 'getch' was declared deprecated, for MSVC8
#pragma warning (disable : 4996)
// Disable certain compiler warnings for .NET
#pragma warning (disable:4313)
#pragma warning (disable:4312)
#pragma warning (disable:4267)

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

// This file is required only for software download operations
#include "QMSL_Core\QpstInterface\QLib_SoftwareDownloadDefines.h"

// Included for demonstration of the user-defined transport layer
#include "QLibDemo-UserDefinedTransport.h"

// Include NV Manager demo
#include "QMSL_NVManager_Demo.h"

// Include RFCal QSC demo (ESC V5)
#include "RFCal\RFCal.h"

// GSM V2 Tx/Rx, GSM V5 FBRx for Atlas
#include "RFCal\RFCal_GSM.h"

// ARA, Qlink Test
#include "RFVerify\RFVerify.h"

// ESC V4 cal 
#include "RFCal\RFCal_ESCV4.h"

// Legacy cal and verificaiton
#include "RFCal\RFCal_Legacy.h"
#include "RFVerify\RFVerify_Legacy.h"

// Test sequences
unsigned char TestSeq_Diag( void );

void Test_NV_FTM_Mode( char iNewFtmMode );
void TestSendSync( void );
void TestVersion( void );
void TestSeq_FTM_State( void );

unsigned char Test_Gnss_Dispatch();
unsigned char Test_FTM_AGPS( void );
unsigned char Test_CGPS( void );
unsigned char Test_CGPS_Gen8( void );
unsigned char Test_GNSS_SOC( void );

unsigned char Test_FTM_Fprint( void );
unsigned char Test_FTM_BT( void );
unsigned char Test_FTM_PMIC( void );

unsigned char Test_FTM_DetectCommand( void );

unsigned char Test_Misc( void );

unsigned char Test_Download( void );
unsigned char Test_EFS( void );

unsigned char Test_Logging( void );
unsigned char Test_Audio( void );
unsigned char Test_Camera( void );
unsigned char Test_Simlock( void );

unsigned char Test_Library( void );

unsigned char Test_Security( void );

unsigned char Test_GSM_Diag( void );

unsigned char Test_Handset( void );

unsigned char Test_UserDefinedTransport( void );

unsigned char Test_FTM_WLAN( void );

unsigned char Test_NV_Tool( void );
unsigned char Test_NV_BackupRestore_Without_NV_Definition_File( void );

unsigned char ReportPhoneState( void );

unsigned char SetBitState_DWORD_Reg( unsigned long iAddress, unsigned long iMask, unsigned short iValue );
unsigned char SetBitState_BYTE_Reg( unsigned long iAddress, unsigned char iMask, unsigned short bOnOff );

unsigned char StartAsyncCapture( unsigned long iSSID, long iRT_MASK );

void DumpAsync( void );

void InterpretAsyncInLog( void );

unsigned char Test_AT_Commands( void );

unsigned char Test_FTM_GPS( void );
unsigned char Test_IS801_GPS( void );

// FM Braodcast
unsigned char Test_FTM_FM( );
unsigned char Test_FTM_FM_RX();
unsigned char Test_FTM_FM_TX();
unsigned char Test_FTM_FM_AF();
unsigned char Test_FTM_FM_Audio();

// Blowing QFPROM in Download Mode
unsigned char Test_Blow_QFPROM_in_Download_Mode();

unsigned char Test_FTM_ANT();
unsigned char Test_FTM_NFC();

/* RF */
unsigned char Test_RF_Calibration (void);
unsigned char Test_RF_Verification_Menu (void);
HANDLE deviceMutex; // For RF Cal callbox (used QSC cal and GSM cal for atlas using QSCPI callbox)

// Variables to hold names of files that have been directoried
const int gc_iMaxUserFiles = 500;       // Maximum number of files to be stored in the user filename list
int g_iUserFileCount = 0;               // Current number of files found in the user folder
const int gc_iUserFileNameLengthMax = 200;  // Maximum number of characters in the user filename
char g_sUserfiles[ gc_iMaxUserFiles ][gc_iUserFileNameLengthMax];   // User file name list

/******************************************************************************/
/**

Function to fill in a character buffer with a hex string reprepsentation of the
bytes in a binary buffer of a specified size.

\param sString = string buffer to be filled in
\param piBytes = binary buffer reference
\param iSize = number of bytes in buffer

\return none

*******************************************************************************/
void PrepareByteListString( char* sString, byte* piBytes, int iSize);

struct MessageReport
{
   unsigned char CMD_CODE;
   unsigned short iQuantity;
   unsigned long iDropCount;
   unsigned long iTotalMessages;
   unsigned char iMessageLevel;
   char sFileName[13];
   unsigned short iLineNumber;
   char sFormatStr[40];
   short iCode1;
   short iCode2;
   short iCode3;
   unsigned char aiTimeStamp[8];
};

/**
Callback Functions
*/

//! Call back for an EFS directory element
unsigned char EfsDirCallBack
(
   char name[QLIB_EFS_MAX_FILENAME_LEN],
   unsigned char isFile,

   //attributes follow below
   unsigned short iAttributeMask,
   unsigned char iBufferingOption,
   unsigned char iCleanupOption,
   unsigned long iCreateDate,
   unsigned long iFileSize,
   HANDLE hContextID
);

//! Call back for an EFS transfer
unsigned char EfsCallback_demo
(
   char srcname[512],
   char dstname[512],
   int oper,
   int suboper,
   unsigned long bytestransferred,
   unsigned long filesize,
   HANDLE hContext
);

//! Callback for generic download events
unsigned char generalDownloadEventCallBack
(
   unsigned char* pGeneralSwDownloadEvent_union
);

//! Callback for async messages
void asyncMessageCallBack
(
   unsigned short iMessageSize,
   unsigned char* iMessageBuffer,
   HANDLE hContextID
);

//! Callback function for text messages
void textMessageCallBack
(
   unsigned short iMessageSize,
   unsigned char* piMessageBuffer,
   HANDLE hContextID
);

//! Callback function for SWDL QPHONEMS mode
void swdlCallBack
(
   HANDLE hContextID,
   char * sMsg,
   unsigned short iMsgSize
);

//! Resource context used for all calls in this file
HANDLE g_hResourceContext = NULL;

//! Flag to indicate whether async message callback should print things to the screen
unsigned char g_bAsyncCallbackVerbose = 0;

//! Flag to indicate whether text message callback should print things to the screen
unsigned char g_bTextCallbackVerbose = 0;

// Defined in QlibDemo_Femto.cpp
unsigned char Test_Femto(int argc, char** argv);

// Defined in QlibDemo_QCAWLAN.cpp
int Test_WLAN_x86(int argc, char** argv);

// Defined in QlibDemo_QCABT.cpp
int Test_BT_x86(int argc, char** argv);

/**
main() function handler
*/
int main(int argc, char* argv[])
{
   // This the default COM port, but a COM port can be specified as a command line parameter.
   unsigned short iComPort = 6;
   unsigned short iTargetType = QLIB_TARGET_TYPE_MSM_MDM;
   unsigned char bContinue = true;      // Variable for menu
   char cKey = ' ';                 // Key pressed in menu
   unsigned short iConnectServerWithHandShake = 0; // 0 = QLIB_ConnectServer, 1 = QLIB_ConnectServerWithHandShake

   char sLibraryVersion[50];

   // This will get filled out based on command line arguments
   unsigned short _bUserSelectQPST_short = false;
   unsigned char _bUserSelectQPST = false;

   // These will get filled in by the library interface
   unsigned char _bSupportsDiag = false;
   unsigned char _bSupportsEFS = false;
   unsigned char _bSupportsSwDownload = false;
   unsigned char _bUsingQPST = false;

   // Check the number of input arguments.  If 0, then use the hard coded COM port.
   //    Otherwise parse the first argument to determine the COM port number
   if ( argc > 1 )
   {
      // Convert the first argument from string to integer
      iComPort = atoi( argv[1] );
   }
   else
   {
      printf("\n QLIB Demo--No COM port specified from command line!\n\nPlease enter the COM port (-1 = auto detect), or enter -2 for Femto tool, or enter -3 for WLAN x86, or enter -4 for Bluetooth x86 #: ");
      scanf("%hd", & iComPort );
   }

   if (iComPort == (word)-2)
   {
      Test_Femto(argc, argv);
      exit(0);
   }

   if (iComPort == (word)-3)
   {
      // -3 10.234.16.212 C:\Qualcomm\WCN\ProdTests\refDesigns\BoardData\fakeBoardData_AR6320.bin
      Test_WLAN_x86(argc, argv);
      exit(0);
   }

   if (iComPort == (word)-4)
   {
      Test_BT_x86(argc,argv);
      exit(0);
   }

   // Check the number of input arguments.  If 0, then use the hard coded COM port.
   //    Otherwise parse the first argument to determine the COM port number
   if ( argc > 2 )
   {
      // Convert the first argument from string to integer
      _bUserSelectQPST = atoi( argv[2] );
   }
   else
   {
      if ( iComPort == QLIB_COM_NULL )
      {
         printf("User Requested NULL COM Port, forcing mode to QPHONEMS\n" );
         _bUserSelectQPST_short = QLIB_LIB_MODE_QPHONEMS;
      }
      else
      {
         printf("\nPlease library type (0=QPhoneMS, 1=QPST): ");
         scanf("%hd", & _bUserSelectQPST_short );
      }

      // An extra level is needed here because there is not a scanf() for a single byte integer
      _bUserSelectQPST = (unsigned char) _bUserSelectQPST_short;
   }

   if ( argc > 3 )
   {
      iConnectServerWithHandShake = atoi( argv[3] );
   }
   else
   {
      printf("\nPlease choose connection method\n");
      printf("0 == QLIB_ConnectServer\n1 == QLIB_ConnectServerWithHandShake\n\n");
      printf("Enter 0 for Brew, Linux or Windows Mobile Full boot\n");
      printf("Enter 1 for Windows Mobile FTM boot\n");
      printf("Enter (0 or 1)->");
      scanf("%hd", &iConnectServerWithHandShake );
   }

   if( argc > 4 )
   {
      iTargetType = atoi(argv[4]);
   }
   else
   {
      printf("\nPlease choose connection method\n");
      printf("0 == QLIB_TARGET_TYPE_MSM_MDM\n1 == QLIB_TARGET_TYPE_APQ\n\n");
      printf("Enter 0 for MSM/MDM target.  AMSS with WWAN modem\n");
      printf("Enter 1 for APQ only target.  AMSS without WWAN modem\n");
      printf("Enter (0 or 1)->");
      scanf("%hd", &iTargetType );
   }

   // Set the library mode--QPST or QPHONEMS
   QLIB_SetLibraryMode( _bUserSelectQPST ? QLIB_LIB_MODE_QPST : QLIB_LIB_MODE_QPHONEMS );

   // Set TargetType
   QLIB_SetTargetType((unsigned char)iTargetType);

   // Get the library version
   QLIB_GetLibraryVersion(sLibraryVersion);

   printf("QLibrary Demo\n\n");
   printf("QLIB DLL Version: %s\n\n", sLibraryVersion );

   if ( iComPort == QLIB_COM_AUTO_DETECT )
      printf("User Requested COM Port: AUTO\n" );
   else if ( iComPort == QLIB_COM_NULL )
      printf("User Requested COM Port: NULL\n" );
   else
      printf("User Requested COM Port: %d\n", iComPort );

   // Get the capabilities
   QLIB_GetLibraryCapabilities( &_bSupportsDiag, &_bSupportsEFS, &_bSupportsSwDownload, &_bUsingQPST );

   printf("\n       Supports DIAG: %s", _bSupportsDiag          ? "YES" : "NO" );
   printf("\n        Supports EFS: %s", _bSupportsEFS           ? "YES" : "NO" );
   printf("\nSupports SW Download: %s", _bSupportsSwDownload    ? "YES" : "NO" );
   printf("\n          Using QPST: %s", _bUsingQPST         ? "YES" : "NO" );

   // Connect to the desired COM port
   if(  iConnectServerWithHandShake == 0)
      // Brew, Linux, Windows Mobile full boot target
   {
      g_hResourceContext  = QLIB_ConnectServer( iComPort );
   }
   else
      // Win Mobile boots into FTM mode directly
   {
      g_hResourceContext  = QLIB_ConnectServerWithHandShake( iComPort );
   }

   if ( _bUsingQPST)
   {
      // Configure call back functions
      QLIB_ConfigureCallBacks( g_hResourceContext, NULL, generalDownloadEventCallBack, asyncMessageCallBack  );
   }
   else
   {
      // Configure call back functions
      QLIB_ConfigureCallBacks( g_hResourceContext, NULL, NULL, asyncMessageCallBack  );
   }

   // Setup EFS call backs--this handles both the directory & file transfer callbacks
   QLIB_ConfigureEfs2CallBacks( g_hResourceContext, EfsCallback_demo, EfsDirCallBack );

   // Configure text message call back
   QLIB_ConfigureLibraryTextLogCallBack(g_hResourceContext, textMessageCallBack );

   if ( g_hResourceContext )
   {
      printf("\nContext creation: PASS\n");

      // Clear the serial Rx buffer-- don't check return value because it is not enabled for QPST version
      QLIB_FlushRxBuffer( g_hResourceContext  );

      // Check online status
      if ( QLIB_IsPhoneConnected( g_hResourceContext ) )
      {
         // Print the mobile's info
         TestVersion( );
      } // if ( QLIB_DIAG_IsOnline() )
      else
      {
         printf("\nPhone Connection Status: FAIL\n");
      } // if ( QLIB_DIAG_IsOnline() )..else
   }
   else
   {
      printf("\nServer connection: FAIL\n");
   }    // if ( QLIB_SetComPort( iComPort ) )

   do
   {
      // Print options
      printf("\n\n\n======================================\n");
      printf("\n 1 = Diag");
      printf("\n 2 = Send Sync (Key Press PWR_ON)");
      printf("\n 3 = Version Info");
      printf("\n 4 = FTM State Control");
      printf("\n 8 = IzAt GNSS Tests");
      printf("\n 9 = FTM Bluetooth");
      printf("\n 0 = At commands");
      printf("\n A = FTM PMIC");

      if( _bSupportsSwDownload)
         printf("\n B = Download");

      if ( _bSupportsEFS )
      {
         printf("\n C = EFS");
      }
      printf("\n D = Miscellaneous");
      printf("\n E = Test Phone Logging");
      printf("\n F = FTM Audio");
      printf("\n G = FTM Camera");
      printf("\n H = SIMLOCK test");
      printf("\n I = Backup/Restore NV to/from QCN/xQCN without NV Definition file");
      printf("\n J = Test Library");
      printf("\n K = Security (GSDI) ");
      printf("\n M = RF Calibration (C2K/GSM/WCDMA/LTE/TDS)");
      printf("\n N = RF Verification (C2K/GSM/WCDMA/LTE/TDS)");
      printf("\n P = Test_FTM_DetectCommand");
      printf("\n R = GSM Diag");
      printf("\n S = Handset Diag");
      printf("\n Z = User-defined transport layer");
      printf("\n @ = FTM WLAN");
      printf("\n # = FTM Common");
      printf("\n ) = NV Manager");
      printf("\n + = NV Tool");
      printf("\n / = FTM FM (Broadcast Radio)");
      printf("\n | = FTM Sequencer");
      printf("\n } = Blow QFPROM in Download Mode");
      printf("\n > = ESC Parser" );
      printf("\n \" = FTM ANT");
      printf("\n ^ = FTM NFC");
      printf("\n . = SECURITY ID SENSOR (FPRINT)");
      printf("\n Q = Quit\n");

      // Get a key
      cKey = toupper( getch() );

      printf("\n\n\n\n\n\n\n\n");
      switch (cKey)
      {
         case '1':
            // Test Diag Sequence
            TestSeq_Diag();
            break;

         case '2':
            // Test the SyncSync() operation
            TestSendSync();
            break;

         case '3':
            // Version info
            TestVersion( );
            break;
         case '4':
            // Test the SyncSync() operation
            // Go out of FTM mode
            TestSeq_FTM_State( );
            break;

         case '8':
            // AGPS
            Test_Gnss_Dispatch( );
            break;

         case '9':
            // BT
            Test_FTM_BT( );
            break;

         case '0':
            Test_AT_Commands();
            break;

         case 'A':
            // PMIC
            Test_FTM_PMIC( );
            break;

         case 'B':
            //if (_bSupportsSwDownload)
            Test_Download();
            break;

         case 'C':
            if (_bSupportsEFS)
               Test_EFS();
            break;

         case 'D':
            Test_Misc();
            break;

         case 'E':
            Test_Logging();
            break;

         case 'F':
            Test_Audio();
            break;

         case 'G':
            Test_Camera();
            break;

         case 'H':
            Test_Simlock();
            break;
        
         case 'I':
            Test_NV_BackupRestore_Without_NV_Definition_File();
            break;

         case 'J':
            Test_Library();
            break;
         case 'K':
            Test_Security();
            break;
         case 'M':
            Test_RF_Calibration();
            break;
         case 'N':
            Test_RF_Verification_Menu();
            break;
         case 'P':
            Test_FTM_DetectCommand();
            break;
         case 'R':
            Test_GSM_Diag();
            break;
         case 'S':
            Test_Handset();
            break;
         case 'Z':
            Test_UserDefinedTransport();
            break;
         case '@':
            // FTM WLAN
            Test_FTM_WLAN();
            break;

         case '#':
            // FTM Common
            Test_FTM_Common();
            break;

         case '+':
            Test_NV_Tool();
            break;

         case ')':
            Test_NV_Manager();
            break;
         case 'Q':
            bContinue = false;
            break;
         case '/':
            // FTM FM Broadcast Radio
            Test_FTM_FM( );
            break;

         case '|':
            Test_FTM_Sequencer();
            break;
         case '}':
            Test_Blow_QFPROM_in_Download_Mode();
            break;
         case '>':
            Test_ESC_Parser();
            break;
         case '"':
            Test_FTM_ANT();
            break;
         case '^':
            Test_FTM_NFC();
            break;
         case '.':
            Test_FTM_Fprint();
            break;
      } // switch( cKey)
   }
   while (bContinue);

   // This should be done before the application completes
   QLIB_DisconnectServer( g_hResourceContext );

   // This forces all connections to be closed, in the event that autodetections
   // were done during the course of the application.
   QLIB_DisconnectAllServers();

   return 0;
}

/**
TestSeq_Diag()  - Collection
*/
unsigned char TestSeq_Diag( void )
{
   printf("----------------\nDiagnostic Mode Test\n----------------\n");

   unsigned char bOk=true;
   unsigned char cKey;

   // unsigned long iAddress = 0X20000000;      // LCD Driver
   // unsigned long iAddress = 0X20100000;      // LCD Data
   // unsigned long iAddress = 0X84000164;      // GPIO Out 4
   // unsigned long iAddress = 0X84000158;      // GPIO Output Enable 4
   // unsigned long iAddress = 0x8400015c;  // HWIO_GPIO_OUT_0_ADDR
   // unsigned long iAddress = 0x84000160;  // HWIO_GPIO_OUT_1_ADDR
   // unsigned long iAddress = 0x84000164;  // HWIO_GPIO_OUT_4_ADDR
   // unsigned long iAddress = 0x48000008;  // HWIO_GPIO_OUT_2_ADDR
   // unsigned long iAddress = 0x4800000c;  // HWIO_GPIO_OUT_3_ADDR
   // unsigned long iAddress = 0x84000118;  // USB
   // unsigned long iAddress = 0x61800018;  // MMCC_READ_TO
   unsigned long iAddress = 0X84000168;     // for GPIO 17

   // unsigned long iAddress = 0X00001000;

   // unsigned long iRegValue = 0x01020304;
   // unsigned long iRegValue = 0x1c49e000;     // for GPIO Out 4
   // unsigned long iRegValue = 0xDC000189;         // for HWIO_GPIO_OUT_0_ADDR
   // unsigned long iRegValue = 0x1CC9E001; // for HWIO_GPIO_OUT_4_ADDR

   unsigned long iRegValue = 0x83BF83BE;    // for HWIO_GPIO_OUT_2_ADDR

   unsigned long iBitMask  = 0x00000001;        // for GPIO Out 4

   unsigned char sAddressDescription[] = "Test Register";
   /*
   sub EnableBootSRAM {
   MemWrite(0x600001B0,0x20,"long");
   }

   sub DisableBootSRAM {
   MemWrite(0x600001B0,0x1,"long");
   }
   */

   char _sMainFormActive[] = "IdleMainForm is active";

   if ( bOk )
   {
      printf("\n\n1=Keypress ( 1 )");
      printf("\n2=SPC( 000000 )");
      printf("\nZ=SPC( 111111 )");
      printf("\n3=POKE_32 0x%X (%s) = 0x%x)", iAddress, sAddressDescription, iRegValue);
      printf("\n4=PEEK_32 0x%X (%s))", iAddress, sAddressDescription );
      printf("\n5=POKE_16 0x%X (%s) = 0x%x)", iAddress, sAddressDescription, (unsigned short) iRegValue);
      printf("\n6=PEEK_16 0x%X (%s))", iAddress, sAddressDescription );
      printf("\n7=POKE_8 0x%X (%s) = 0x%x)", iAddress, sAddressDescription, (unsigned char) iRegValue);
      printf("\n8=PEEK_8 0x%X (%s))", iAddress, sAddressDescription );
      printf("\n9=GPIO 36 (W5 Backlight EN) ON/OFF, 8 bit PEEK/POKE" );
      printf("\nA=GPIO 36 (W5 Backlight EN) ON/OFF, 32 bit PEEK/POKE" );
      printf("\nZ=GPIO 36 (W5 Backlight EN) ON/OFF, 16 bit PEEK/POKE" );
      printf("\nB=Set Backlight active (for testing 9 and A)");
      printf("\nC=DIAG Error Report");
      printf("\nD=DIAG Error Report Clear");
      printf("\nE=NV Write 5 items to NV_DIAL_I  (DIAG_NV_WRITE_F method)");
      printf("\nF=NV Read 5 items from NV_DIAL_I (SendSync method)");
      printf("\nG=NV Write 5 items to NV_DIAL_I  (DIAG_NV_WRITE_ARRAY_F method)");
      printf("\nH=NV Read 5 items from NV_DIAL_I (DIAG_NV_READ_ARRAY_F method)");
      printf("\nI=Async Command send/receive");
      printf("\nJ=CNV Backup");
      printf("\nK=Test Log File Start Playback");
      printf("\nM=Test Log File Stop Playback");
      printf("\nN=Charger On");
      printf("\nO=Charger OFF");
      printf("\nP=Multiple NV Write");
      printf("\nR=Disable Battery ID Checking");
      printf("\nS=Wait for main form extended message (%s)", _sMainFormActive);
      printf("\nT=Read SPC lock info ");
      printf("\nU=Port list, all available ports");
      printf("\nV=Port list, all ports with connected phones");
      printf("\nW=Auto detect phones, then connect to logical port #1");
      printf("\nX=Wait for 60 seconds, for any phone to become connected to a COM port");
      printf("\n#=Change COM port #");
      printf("\n$=Set NV_VCO_DEFAULT_I to 0");
      printf("\n&=Read an NV item");
      printf("\n*=Read time out values");
      printf("\n(=Read ESN");
      printf("\n)=Test loop for connect/disconnect");
      printf("\n-=Test receiver thread for rapid many commands responses");
      printf("\n[=Time profile for NV Write");

      cKey = toupper(getch());

      printf("\n\n");

      if (cKey == '1' )
      {
         printf("Phone mode = ONLINE: PASS\n" );

         // Press the 1 key
         bOk = QLIB_DIAG_HS_KEY_F( g_hResourceContext, '1', 0 );

         printf("   Key press '1': %s\n", bOk ? "PASS" : "FAIL" );
      }// cKey == 1
      else if(cKey == '2')
      {
         unsigned char _iSPC[6] = {'0','0','0','0','0','0'};
         unsigned char _iSPC_Result = 0;

         bOk = QLIB_DIAG_SPC_F( g_hResourceContext, _iSPC, &_iSPC_Result );

         // Print status
         printf("\n   Command Status: %s", bOk ? "PASS" : "FAIL\n" );
         printf("\n   SPC Status: %s", _iSPC_Result ? "PASS" : "FAIL\n" );
      }
      else if(cKey == 'Z')
      {
         unsigned char _iSPC[6] = {'1','1','1','1','1','1'};
         unsigned char _iSPC_Result = 0;

         bOk = QLIB_DIAG_SPC_F( g_hResourceContext, _iSPC, &_iSPC_Result );

         // Print status
         printf("\n   Command Status: %s", bOk ? "PASS" : "FAIL\n" );
         printf("\n   SPC Status: %s", _iSPC_Result ? "PASS" : "FAIL\n" );
      }
      else if(cKey == '3')
      {
         unsigned short _iNumBytes = sizeof(unsigned short);

         bOk = QLIB_DIAG_MEMORY_POKE_DWORD_F( g_hResourceContext, iAddress, _iNumBytes, &iRegValue );

         printf("\n   Command Status: %s", bOk ? "PASS" : "FAIL\n" );
      }
      else if(cKey == '4')
      {
         unsigned short _iNumBytes = sizeof(unsigned long);
         unsigned long  _iPeekStorage = 0;

         bOk = QLIB_DIAG_MEMORY_PEEK_DWORD_F( g_hResourceContext, iAddress, _iNumBytes, &_iPeekStorage );

         printf("\n   Command Status: %s", bOk ? "PASS" : "FAIL\n" );
         printf("\n   Peek Value: 0x%X", _iPeekStorage );
      }
      else if(cKey == '5')
      {
         unsigned short _iNumBytes = sizeof(unsigned short);

         bOk = QLIB_DIAG_MEMORY_POKE_WORD_F( g_hResourceContext, iAddress, _iNumBytes, (word*) &iRegValue );

         printf("\n   Command Status: %s", bOk ? "PASS" : "FAIL\n" );
      }
      else if(cKey == '6')
      {
         unsigned short _iNumBytes = sizeof(unsigned short);
         unsigned long  _iPeekStorage = 0;

         bOk = QLIB_DIAG_MEMORY_PEEK_F( g_hResourceContext, iAddress, _iNumBytes, (byte*) &_iPeekStorage );

         printf("\n   Command Status: %s", bOk ? "PASS" : "FAIL\n" );
         printf("\n   Peek Value: 0x%X", (word) _iPeekStorage );
      }
      else if(cKey == '7')
      {
         unsigned short _iNumBytes = sizeof(unsigned char);

         bOk = QLIB_DIAG_MEMORY_POKE_F( g_hResourceContext, iAddress, _iNumBytes, (byte*) &iRegValue );

         printf("\n   Command Status: %s", bOk ? "PASS" : "FAIL\n" );
      }
      else if(cKey == '8')
      {
         unsigned short _iNumBytes = sizeof(unsigned char);
         unsigned long  _iPeekStorage = 0;

         bOk = QLIB_DIAG_MEMORY_PEEK_F( g_hResourceContext, iAddress, _iNumBytes, (byte*) &_iPeekStorage );

         printf("\n   Command Status: %s", bOk ? "PASS" : "FAIL\n" );
         printf("\n   Peek Value: 0x%X", (byte) _iPeekStorage );
      }
      else if ( (cKey == '9')
                || (cKey == 'A')
                || (cKey == 'Z')
              )
      {
         unsigned short _iNumBytes = sizeof(unsigned char);
         unsigned char _iGPIO_Status = 0;
         unsigned long _iGPIO_Status_DW = 0;
         unsigned short _iGPIO_Status_W = 0;

         unsigned char _bOn = (cKey == '9');

         unsigned long iAddress = 0x84000160;   // GPIO_OUT_1

         while (!kbhit() )
         {
            if ( cKey == '9')
            {
               // Read the state of GPIO_OUT_1
               bOk = QLIB_DIAG_MEMORY_PEEK_F( g_hResourceContext, iAddress, _iNumBytes, &_iGPIO_Status );

               printf("\n   QLIB_DIAG_MEMORY_PEEK_F(0x%X): %s", iAddress, bOk ? "PASS" : "FAIL\n" );
               printf("\n   GPIO_OUT_1 (8-bit): 0x%X", (byte) _iGPIO_Status );

               if ( _bOn )
               {
                  // Turn the GPIO36 bit on, bit 5
                  _iGPIO_Status |= 0x10;
                  _bOn = false;
               }
               else
               {
                  // Turn the GPIO36 bit off, bit 5
                  _iGPIO_Status &= 0xEF;
                  _bOn = true;
               }

               // Poke the value back into GPIO_OUT_1
               bOk = QLIB_DIAG_MEMORY_POKE_F( g_hResourceContext, iAddress, _iNumBytes, (byte*) &_iGPIO_Status );
               printf("\n   QLIB_DIAG_MEMORY_POKE_F(0x%X, 0x%X): %s", iAddress, _iGPIO_Status, bOk ? "PASS" : "FAIL\n" );

               bOk = QLIB_DIAG_MEMORY_PEEK_F( g_hResourceContext, iAddress, _iNumBytes, &_iGPIO_Status );
               printf("\n   QLIB_DIAG_MEMORY_PEEK_F(0x%X): %s", iAddress, bOk ? "PASS" : "FAIL\n" );
               printf("\n   GPIO_OUT_1 (8-bit): 0x%X", (byte) _iGPIO_Status );
            }

            if ( cKey == 'Z' )
            {
               bOk = QLIB_DIAG_MEMORY_PEEK_WORD_F( g_hResourceContext, iAddress, _iNumBytes, &_iGPIO_Status_W );
               printf("\n   QLIB_DIAG_MEMORY_PEEK_DWORD_F(0x%X): %s", iAddress, bOk ? "PASS" : "FAIL\n" );
               printf("\n   GPIO_OUT_1: 0x%X", (byte) _iGPIO_Status_DW );

               if ( _bOn )
               {
                  // Turn the GPIO36 bit on, bit 5
                  _iGPIO_Status_DW |= 0x00000010;
                  _bOn = false;
               }
               else
               {
                  // Turn the GPIO36 bit off, bit 5
                  _iGPIO_Status_DW &= 0xFFFFFFEF;
                  _bOn = true;
               }

               // Poke the value back into GPIO_OUT_1
               bOk = QLIB_DIAG_MEMORY_POKE_DWORD_F( g_hResourceContext, iAddress, _iNumBytes, &_iGPIO_Status_DW );
               printf("\n   QLIB_DIAG_MEMORY_POKE_DWORD_F(0x%X, 0x%X): %s", iAddress, _iGPIO_Status_DW, bOk ? "PASS" : "FAIL\n" );

               bOk = QLIB_DIAG_MEMORY_PEEK_DWORD_F( g_hResourceContext, iAddress, _iNumBytes, &_iGPIO_Status_DW );
               printf("\n   QLIB_DIAG_MEMORY_PEEK_DWORD_F(0x%X): %s", iAddress, bOk ? "PASS" : "FAIL\n" );
               printf("\n   GPIO_OUT_1): 0x%X", (byte) _iGPIO_Status_DW );
            }

            if ( cKey == 'A' )
            {
               bOk = QLIB_DIAG_MEMORY_PEEK_WORD_F( g_hResourceContext, iAddress, _iNumBytes, &_iGPIO_Status_W );
               printf("\n   QLIB_DIAG_MEMORY_PEEK_WORD_F(0x%X): %s", iAddress, bOk ? "PASS" : "FAIL\n" );
               printf("\n   GPIO_OUT_1: 0x%X", (byte) _iGPIO_Status_W );

               if ( _bOn )
               {
                  // Turn the GPIO36 bit on, bit 5
                  _iGPIO_Status_W |= 0x0010;
                  _bOn = false;
               }
               else
               {
                  // Turn the GPIO36 bit off, bit 5
                  _iGPIO_Status_W &= 0xFFEF;
                  _bOn = true;
               }

               // Poke the value back into GPIO_OUT_1
               bOk = QLIB_DIAG_MEMORY_POKE_WORD_F( g_hResourceContext, iAddress, _iNumBytes, &_iGPIO_Status_W );
               printf("\n   QLIB_DIAG_MEMORY_POKE_WORD_F(0x%X, 0x%X): %s", iAddress, _iGPIO_Status_W, bOk ? "PASS" : "FAIL\n" );

               bOk = QLIB_DIAG_MEMORY_PEEK_WORD_F( g_hResourceContext, iAddress, _iNumBytes, &_iGPIO_Status_W );
               printf("\n   QLIB_DIAG_MEMORY_PEEK_WORD_F(0x%X): %s", iAddress, bOk ? "PASS" : "FAIL\n" );
               printf("\n   GPIO_OUT_1): 0x%X", (byte) _iGPIO_Status_W );
            }

            Sleep( 10);
         }

         // Clear the PC's keypress queue
         getch();
      }
      else if (cKey == 'B')
      {
         // Put the phone into FTM mode
         bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_FTM_F );
         printf("\nQLIB_DIAG_CONTROL_F(  MODE_FTM_F ): %s", bOk ? "PASS" : "FAIL\n" );
      }
      else if (cKey == 'C')
      {
         // Structure to hold the error report
         QLIB_DIAG_Err_Read _oErrorRead;

         // Read Error
         bOk = QLIB_DIAG_ERR_READ_F( g_hResourceContext, (unsigned char*) &_oErrorRead );
         printf("\nQLIB_DIAG_ERR_READ_F(  MODE_FTM_F ): %s", bOk ? "PASS" : "FAIL\n" );

         // If ok,then print the contents
         if (bOk )
         {
            printf("\n\nError Report");
            printf("\n------------");
            printf("\n      Error Count: %d",_oErrorRead.iErrorCount);
            printf("\n   Errors Ignored: %d",_oErrorRead.iErrorIgnored);

            printf("\n\nIndex\tCount\tFile Name\tLine\tFatal");
            printf("\n-----\t-----\t---------\t----\t-----");

            // Loop through the error list, print one line per entry
            for (int _iCount = 0; _iCount < _oErrorRead.iErrorCount; _iCount++)
            {
               printf("\n%d\t%d\t%c%c%c%c%c%c%c%c\t%d\t%s",
                      _oErrorRead.aErrorRecords[_iCount].iAddress,
                      _oErrorRead.aErrorRecords[_iCount].iCount ,
                      _oErrorRead.aErrorRecords[_iCount].sFileName[0] ,
                      _oErrorRead.aErrorRecords[_iCount].sFileName[1] ,
                      _oErrorRead.aErrorRecords[_iCount].sFileName[2] ,
                      _oErrorRead.aErrorRecords[_iCount].sFileName[3] ,
                      _oErrorRead.aErrorRecords[_iCount].sFileName[4] ,
                      _oErrorRead.aErrorRecords[_iCount].sFileName[5] ,
                      _oErrorRead.aErrorRecords[_iCount].sFileName[6] ,
                      _oErrorRead.aErrorRecords[_iCount].sFileName[7] ,
                      _oErrorRead.aErrorRecords[_iCount].iLineNum ,
                      _oErrorRead.aErrorRecords[_iCount].bFatal ? "YES" : "NO"
                     );
            }
         }
      }
      else if (cKey == 'D')
      {
         bOk = QLIB_DIAG_ERR_CLEAR_F( g_hResourceContext, 0xFF );
         printf("\nQLIB_DIAG_ERR_CLEAR_F( ALL ): %s", bOk ? "PASS" : "FAIL\n" );
      }
      else if (cKey == 'E')
      {
         // Setup a loop for time profiling
         for ( int _iCount = 0; _iCount < 400/5; _iCount++ )
         {
            printf("\n  Loop Count: %d", _iCount );

            const unsigned short c_iNV_DIAL_I = 57;
            const unsigned short c_iSizeNvData = 47;
            unsigned char _aNV_Data[c_iSizeNvData] =
            {
               0,   // Address
               0,   // Status,
               5,   // Num Digits
               0, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Digits, 32 bytes
               0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31   // Name = 12 digits
            };

            unsigned short _iStatus;

            // Write 5 items to NV_DIAL_I
            for (int _iCount = 0; _iCount < 5; _iCount++)
            {
               // Set Array address
               _aNV_Data[0] = _iCount;

               // Set 1st digit to a unique value
               _aNV_Data[3] = _iCount+0x30;
               _aNV_Data[4] = _iCount+0x30;
               _aNV_Data[5] = _iCount+0x30;
               _aNV_Data[6] = _iCount+0x30;
               _aNV_Data[7] = _iCount+0x30;

               // NV_WRITE
               bOk = QLIB_DIAG_NV_WRITE_F( g_hResourceContext, c_iNV_DIAL_I,  _aNV_Data, c_iSizeNvData, &_iStatus );
               printf("\nQLIB_DIAG_NV_WRITE( %d ): %s", _iCount, bOk ? "PASS" : "FAIL\n" );
               printf("\t- Status of QLIB_DIAG_NV_WRITE( %d ): %s", _iCount, _iStatus == NV_DONE_S ? "DONE" : "FAIL\n" );
            }
         } // for ( int _iCount = 0; _iCount < 400/5; _iCount++ )
      }
      else if (cKey == 'F')
      {
         const unsigned short c_iNV_DIAL_I = 57;
         const unsigned short c_iSizeRequest = 4;
         unsigned char _aRequestPacket[c_iSizeRequest] =
         {
            _DIAG_NV_READ_F,        // CMD ID
            c_iNV_DIAL_I & 0XFF,    // NV ID
            0,                      // Upper byte off NV ID
            0                       // Array address
         };

         const unsigned short c_iMaxSizeResponse = 512;
         short _iSizeResponse = c_iMaxSizeResponse;
         unsigned char _aResponsePacket[ c_iMaxSizeResponse ];

         // Use SendSync to Read 5 items to NV_DIAL_I
         for (int _iCount = 0; _iCount < 5; _iCount++)
         {
            // Set Array address
            _aRequestPacket[3] = _iCount;

            // SendSync
            bOk = QLIB_SendSync( g_hResourceContext, c_iSizeRequest, _aRequestPacket, &_iSizeResponse, _aResponsePacket, 2000  );
            printf("\nQLIB_SendSync( %d ): %s", _iCount, bOk ? "PASS" : "FAIL\n" );
            // printf("\t- Status of QLIB_DIAG_NV_READ_F( %d ): %s", _iCount, _iStatus == NV_DONE_S ? "DONE" : "FAIL\n" );

            printf("\t- Data Header #%d: ", _iCount );
            for (int _iByteIndex = 0; _iByteIndex < 30; _iByteIndex++)
            {
               printf("%d ", _aResponsePacket[ _iByteIndex ] );
            }
         }
      }
      else if (cKey == 'G')
      {
         const unsigned short c_iNV_DIAL_I = 57;
         const unsigned short c_iSizeNvData = 46;
         unsigned char _aNV_Data[c_iSizeNvData] =
         {
            0,  // Status,
            5,  // Num Digits
            0x30, 0x31, 0x32, 0x33, 0x34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Digits, 32 bytes
            0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31  // Name = 12 digits
         };

         unsigned short _iStatus;

         // Write 5 items to NV_DIAL_I
         for (int _iCount = 0; _iCount < 5; _iCount++)
         {
            // Set 1st digit to a unique value
            _aNV_Data[3] = _iCount+0x30;

            // NV_WRITE
            bOk = QLIB_DIAG_NV_WRITE_ARRAY_F( g_hResourceContext, c_iNV_DIAL_I,  _iCount, _aNV_Data, c_iSizeNvData, &_iStatus );
            printf("\nQLIB_DIAG_NV_WRITE_ARRAY_F( %d ): %s", _iCount, bOk ? "PASS" : "FAIL\n" );
            printf("\t- Status of QLIB_DIAG_NV_WRITE_ARRAY_F( %d ): %s", _iCount, _iStatus == NV_DONE_S ? "DONE" : "FAIL\n" );
         }
      }
      else if (cKey == 'H')
      {
         const unsigned short c_iNV_DIAL_I = 57;

         const unsigned short c_iMaxNVResponse = 128;
         short _iSizeNvData = c_iMaxNVResponse;
         unsigned char _aNV_Data[ c_iMaxNVResponse ];
         unsigned short _iStatus;

         // Read 5 items to NV_DIAL_I
         for (int _iCount = 0; _iCount < 5; _iCount++)
         {
            // Put the phone into FTM mode
            bOk = QLIB_DIAG_NV_READ_ARRAY_F( g_hResourceContext, c_iNV_DIAL_I,  _iCount, _aNV_Data, _iSizeNvData, &_iStatus );
            printf("\nQLIB_DIAG_NV_READ_ARRAY_F( %d ): %s", _iCount, bOk ? "PASS" : "FAIL\n" );
            printf("\t- Status of QLIB_DIAG_NV_READ_F( %d ): %s", _iCount, _iStatus == NV_DONE_S ? "DONE" : "FAIL\n" );
            printf("\t- Data item #5: %d", _aNV_Data[4]);

            printf("\n Data: ");
            for ( int _iByteIndex = 0; _iByteIndex < _iSizeNvData; _iByteIndex++)
               printf("0x%X, ", _aNV_Data[ _iByteIndex ] );
         }
      }
      else if (cKey == 'I')
      {
         // Send the version command
         printf("\n\nSending version command #0");

         // Data for the command
         unsigned short _iRequestSize = 1;
         unsigned char _piRequestBytes[1] = {_DIAG_VERNO_F};

         // Clear the command response queu
         bOk = QLIB_DIAG_ClearCommandResponseQueue( g_hResourceContext );
         printf("\nQLIB_DIAG_ClearCommandResponseQueue(): %s", bOk ? "PASS" : "FAIL" );

         // Send command asynchronously
         bOk = QLIB_SendASync( g_hResourceContext, _iRequestSize, _piRequestBytes,200, 0 );
         printf("\nQLIB_SendASync(): %s", bOk ? "PASS" : "FAIL" );

         if (bOk )
         {
            // Declare storage for the queue size & next message
            unsigned long _iQueueSize = 0;
            unsigned short _iCmdRspSize = 0;
            unsigned char _iCmdRspData[2000];

            // Wait for a response
            while ( !QLIB_DIAG_GetCommandResponseQueueSize( g_hResourceContext, &_iQueueSize )  && !kbhit() )
            {}

            // Print the queue size
            printf("\n   Queue Size: %ld", _iQueueSize );

            // Get the command response message
            QLIB_DIAG_GetNextCommandResponse
            (
               g_hResourceContext,
               &_iCmdRspSize,
               _iCmdRspData,
               200
            );

            // Print command response size
            printf("\n   Command Response Size: %d", _iCmdRspSize );

            //Print it out
            printf("\n   Command Response Data: ");

            // Loop through all bytes in the command response
            for ( int _iIndex = 0; _iIndex <_iCmdRspSize; _iIndex++)
            {
               printf("0x%X ", _iCmdRspData[ _iIndex ] );
            }
         } // if (bOk )
      }
      else if (cKey == 'J')
      {
         // printf("\nJ=CNV Backup");

         // Send the version command
         printf("\n\nCNV Backup");

         // Clear the command response queu
         bOk = QLIB_DIAG_NV_CNV_DIAG_BACKUP_F( g_hResourceContext );
         printf("\nQLIB_DIAG_NV_CNV_DIAG_BACKUP_F(): %s", bOk ? "PASS" : "FAIL" );
      }
      else if (cKey == 'K')
      {
         // printf("\nK=Test Log File Playback");

         char _sLogFile[] = "C:\\Factory_Log_Playback.txt";

         // Send the version command
         printf("\n\nLog File Playback");
         printf("\n\nPress Q to exit or any other key to begin playback on the log file: %s", _sLogFile );

         cKey = toupper(getch());
         if ( cKey != 'Q' )
         {
            // Determine QPST
            printf("\n\nPress Y to indicate this log file was mode with QPST mode or any other key to indicate it was not" );

            cKey = toupper(getch());

            unsigned char _bQpstMode = (cKey=='Y');

            bOk = QLIB_StartLogFilePlaybackMode( g_hResourceContext, _sLogFile, _bQpstMode );
            printf("\nQLIB_StartLogFilePlaybackMode(): %s", bOk ? "PASS" : "FAIL" );
         }
      }
      else if (cKey == 'M')
      {
         // printf("\nM=Test Log File Stop Playback");

         bOk = QLIB_StopLogFilePlaybackMode( g_hResourceContext );
         printf("\nQLIB_StopLogFilePlaybackMode(): %s", bOk ? "PASS" : "FAIL" );

         // Clear the command response queu
      }
      else if (cKey == 'N')
      {
         // printf("\nN=Charger On");

         unsigned long iAddress = 0X84000164;       // GPIO Out 4
         unsigned long _iMask = 0x000000200;
         unsigned char _iMask2 = 0x02;

         //bOk = SetBitState_DWORD_Reg( iAddress, _iMask, 1 );
         //printf("\nSetBitState_DWORD_Reg(): %s", bOk ? "PASS" : "FAIL" );

         bOk = SetBitState_BYTE_Reg( iAddress+1, _iMask2, 1 );
         printf("\nSetBitState_BYTE_Reg(): %s", bOk ? "PASS" : "FAIL" );

         // Clear the command response queu
      }
      else if (cKey == 'O')
      {
         // printf("\nO=Charger OFF");

         unsigned long iAddress = 0X84000164;       // GPIO Out 4
         unsigned long _iMask = 0x000000200;
         unsigned char _iMask2 = 0x02;

         // bOk = SetBitState_DWORD_Reg( iAddress, _iMask, 0 );
         //printf("\nSetBitState_DWORD_Reg(): %s", bOk ? "PASS" : "FAIL" );

         bOk = SetBitState_BYTE_Reg( iAddress+1, _iMask2, 0 );
         printf("\nSetBitState_BYTE_Reg(): %s", bOk ? "PASS" : "FAIL" );

         // Clear the command response queue
      }
      else if (cKey == 'P')
      {
         // printf("\nP=Multiple NV Write");
         bOk = QLIB_DIAG_NV_WRITE_SetBatchMode( g_hResourceContext, true );
         printf("\nQLIB_DIAG_NV_WRITE_SetBatchMode(): %s", bOk ? "PASS" : "FAIL" );

         // Write to the factory data area, for an example
         unsigned char _aNvBuffer[DIAG_NV_ITEM_DATA_SIZE];

         unsigned long _iTimer = QLIB_StartTimer(g_hResourceContext);

         unsigned short _iNV_RF_CAL_DATE = 571;

         // Array of NV items
         const int c_iNumNvItems = 5;
         const unsigned short c_aiNvItems[c_iNumNvItems] = {2497, 2498, 2499, 2500, _iNV_RF_CAL_DATE }; // Factory data 1..4, and RF_CAL_DATE
         const short  c_iNvSize= 128;   //   DIAG_NV_ITEM_DATA_SIZE

         const int c_iLoopTotal = 100;

         // Loop through these NV items a certain number of times
         for ( int _iLoopCount = 1; _iLoopCount <= c_iLoopTotal; _iLoopCount++ )
         {
            printf("\nLooping to build large list, pass %d/%d", _iLoopCount, c_iLoopTotal );

            // Loop through the number of items
            for ( int _iItemCount = 0; _iItemCount < c_iNumNvItems; _iItemCount++)
            {
               // Set the NV buffer to a counter value.  Use static, so multiple calls in the
               // same session will yield different results.
               byte _iCount = 0X00;
               // _iCount++;
               memset( _aNvBuffer, _iCount, c_iNvSize );

               unsigned short _iItemStatus;

               // Queue this NV items
               bOk = QLIB_DIAG_NV_WRITE_F( g_hResourceContext, c_aiNvItems[ _iItemCount ], _aNvBuffer, c_iNvSize, &_iItemStatus );
               printf("\nQLIB_DIAG_NV_WRITE_F(%d): %s, status: %d",  c_aiNvItems[ _iItemCount ], bOk ? "PASS" : "FAIL", _iItemStatus  );
            }   // for ( int _iItemcount = 0; _iItemcount < c_iNumNvItems; _iItemcount++)
         }  // for ( int _iLoopCount = 0; _iLoopCount < 10; _iLoopCount++ )

         // Return values for the FLUSH function
         unsigned short _iLastErrorNvId;
         unsigned short _iLastTotalStatus;
         unsigned char _iLastItemStatus;

         // Specify that the NV_RF_CAL_DATE_I (573) is allowed to be readonly, only 1 entry in this list
         QLIB_DIAG_NV_WRITE_BatchQueue_SetRO_List( g_hResourceContext, &_iNV_RF_CAL_DATE, 1 );

         // Flush the items queued during batch mode
         bOk = QLIB_DIAG_NV_WRITE_FlushBatchQueue(
                  g_hResourceContext, &_iLastErrorNvId, &_iLastTotalStatus, &_iLastItemStatus  );

         printf("\nQLIB_DIAG_NV_WRITE_FlushBatchQueue(): %s", bOk ? "PASS" : "FAIL" );

         QLIB_PrintTimer(g_hResourceContext, _iTimer, "Multiple NV Write time: ");
      }
      else if (cKey == 'R')
      {
         const unsigned short c_iNV_BM_BAT_STATS_I = 3044;
         const unsigned short c_iSizeNvData = 11;
         unsigned char _aNV_Data[c_iSizeNvData] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8 };

         unsigned short _iStatus;

         // NV_WRITE
         bOk = QLIB_DIAG_NV_WRITE_F( g_hResourceContext, c_iNV_BM_BAT_STATS_I,  _aNV_Data, c_iSizeNvData, &_iStatus );
         printf("\nQLIB_DIAG_NV_WRITE(): %s", bOk ? "PASS" : "FAIL\n" );
         printf("\t- Status of QLIB_DIAG_NV_WRITE(): %s", _iStatus == NV_DONE_S ? "DONE" : "FAIL\n" );
      }
      else if (cKey == 'S')
      {
         // printf("\nS=Wait for main form extended message (%s)", _sMainFormActive);

         // Clear the async queue
         bOk = QLIB_DIAG_ClearPhoneLogQueue( g_hResourceContext  );
         printf("\nQLIB_DIAG_ClearPhoneLogQueue(): %s", bOk ? "PASS" : "FAIL\n" );

         // Return storage for status
         unsigned char _bFoundMessage = false;

         // Return storage for message arguments
         unsigned long _iArgs[3];

         // Set message filter
         unsigned short _iSSID = 6002;  // User interface = 6002
         unsigned long _iRT_MASK = 0x04;    // High level = 0x04

         bOk = QLIB_DIAG_SetMessageFilter( g_hResourceContext, (short) _iSSID, _iRT_MASK );
         printf("\nQLIB_DIAG_SetMessageFilter() = %s", bOk ? "PASS" : "FAIL" );

         // Enable all phone logging
         bOk = QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
         printf("\nQLIB_DIAG_SetPhoneLoggingState = %s", bOk ? "PASS" : "FAIL" );

         // Wait for the message
         while (        ( !kbhit() )
                        &&  !_bFoundMessage
               )
         {
            // Look for the next message
            QLIB_DIAG_FindDebugMessage( g_hResourceContext, _sMainFormActive, &_bFoundMessage,
                                        &_iArgs[0], &_iArgs[1], &_iArgs[2]);

            // Print a comp
            if ( _bFoundMessage )
            {
               printf(" Message (%s) found with arguments: %d, %d, %d ", _sMainFormActive, _iArgs[0], _iArgs[1], _iArgs[2] );
            }
         }

         // Clear keyboard queue
         if ( kbhit() ) getch();
      }
      else if (cKey == 'T')
      {
         const unsigned short c_iNV_DIAG_SPC_UNLOCK_TTL_I = 3252;
         unsigned char _iSPC_UnlockCount = 1;
         unsigned char _iSPC_UnlockCount_ReadOk = false;
         unsigned short _iStatus = NV_DONE_S;

         unsigned char _bSPC_Locked = true;
         unsigned char _bSPC_Error = false;

         // First read the SPC unlock counter
         // NV ID, Data buffer, size, status
         if (QLIB_DIAG_NV_READ_F(  g_hResourceContext, c_iNV_DIAG_SPC_UNLOCK_TTL_I, &_iSPC_UnlockCount, sizeof(_iSPC_UnlockCount), &_iStatus ) )
         {
            printf("NV READ NV_DIAG_SPC_UNLOCK_TTL_I: PASS, value: %d, status: %d\n", _iSPC_UnlockCount, _iStatus);
            _iSPC_UnlockCount_ReadOk = ( _iStatus == NV_DONE_S);
         }
         else
            printf("NV READ NV_DIAG_SPC_UNLOCK_TTL_I: FAIL\n");

         // Update the overall error flag
         _bSPC_Error = !_iSPC_UnlockCount_ReadOk;

         // Make sure the unlock count is >0
         if (   ( _iSPC_UnlockCount > 0 )
                &&  ( !_bSPC_Error )
            )
         {}

         {
            // Now read the SPC code
            const unsigned short c_iNV_SEC_CODE_I = 85;
            const unsigned short c_iSize_NV_SEC_CODE_I = 6;
            unsigned char _iNV_SEC_CODE_I[c_iSize_NV_SEC_CODE_I];
            unsigned char _iNV_SEC_CODE_I_ReadOk = false;

            // NV ID, Data buffer, size, status
            if (QLIB_DIAG_NV_READ_F(  g_hResourceContext, c_iNV_SEC_CODE_I, _iNV_SEC_CODE_I, c_iSize_NV_SEC_CODE_I, &_iStatus ) )
            {
               printf("NV READ NV_SEC_CODE_I: PASS, value: %c%c%c%c%c%c, status: %d\n",
                      _iNV_SEC_CODE_I[0],
                      _iNV_SEC_CODE_I[1],
                      _iNV_SEC_CODE_I[2],
                      _iNV_SEC_CODE_I[3],
                      _iNV_SEC_CODE_I[4],
                      _iNV_SEC_CODE_I[5],
                      _iStatus);
               _iNV_SEC_CODE_I_ReadOk = ( _iStatus == NV_DONE_S);
            }
            else
               printf("NV READ NV_SEC_CODE_I: FAIL\n");

            // Update the overall error flag
            _bSPC_Error &= (!_iNV_SEC_CODE_I_ReadOk);

            // The phone is not locked if no error has occured up to this point
            _bSPC_Locked =  !_bSPC_Error;
         }  // if ( _iSPC_UnlockCount > 0 )

         // print final status
         printf("\n SPC Status = %s", _bSPC_Locked ? "LOCKED" : "UNLOCKED" );
         printf("\n SPC Status Error = %s", _bSPC_Error ? "ERROR" : "NO ERROR" );
      } // else if (cKey == 'T')
      else if (cKey == 'U')
      {
         // printf("\nU=Port list, all available ports");

         printf("\nPort list, all available ports");
         const unsigned char c_iPortListSize = 20;
         unsigned short _iPortListSize = 20;
         unsigned short _aiPortList[ c_iPortListSize ];

         // Get the port list
         if ( QLIB_GetAllPortList( &_iPortListSize, _aiPortList ) )
         {
            printf("\n\n # Ports on computer: %d", _iPortListSize );

            printf("\n Port list: ");

            // Print the values
            for ( int _iIndex = 0; _iIndex < _iPortListSize; _iIndex++)
            {
               printf("%d", _aiPortList[ _iIndex ] );

               // Add on the ", " if this is not the last item
               if ( _iIndex < _iPortListSize -1)
                  printf(", ");
            }   // for ( int _iIndex = 0; _iIndex < _iPortListSize; _iIndex++)
         }  // if ( QLIB_GetAllPortList( &_iPortListSize, _aiPortList ) )
      } /// else if (cKey == 'U')
      else if ( (cKey == 'V')
                || (cKey == 'W')
              )
      {
         // printf("\n=Port list, all ports with connected phones");

         printf("\nPort list, all ports with connected phones");
         const unsigned char c_iPortListSize = 20;
         unsigned short _iPortListSize = 20;
         unsigned short _aiPortList[ c_iPortListSize ];

         // If ports are to be ignored, they can be put into this list.  This will make the
         // process of autodetecting faster, if fewer ports are being scanned.
         //  Values of 0 are "don't cares" since this is an invalid port #
         const unsigned short c_iIgnorePortListSize = 2;
         unsigned short c_aiIgnorePortList[ c_iIgnorePortListSize ] = {1,2};

         // Get the port list
         if ( QLIB_GetAvailablePhonesPortList( &_iPortListSize, _aiPortList, c_iIgnorePortListSize, &c_aiIgnorePortList[0] ) )
         {
            printf("\n\n # Ports with phones: %d", _iPortListSize );

            printf("\n Port list: ");

            // Print the values
            for ( int _iIndex = 0; _iIndex < _iPortListSize; _iIndex++)
            {
               printf("%d", _aiPortList[ _iIndex ] );

               // Add on the ", " if this is not the last item
               if ( _iIndex < _iPortListSize -1)
                  printf(", ");
            }   // for ( int _iIndex = 0; _iIndex < _iPortListSize; _iIndex++)

            // printf("\nW=Auto detect phones, then connect to logical port #1");
            if ( cKey == 'W' )
            {
               unsigned short _iLogicalPort = 0;
               unsigned short _iPhysicalPort = 0;

               // Clear the port mapping
               bOk = QLIB_ClearPortAliasList();
               printf("\n QLIB_ClearPortAliasList(): %s", bOk ? "PASS" : "FAIL" );

               printf("\n\nSetting up logical port mapping...");

               // Loop through found ports
               for ( unsigned short _iPortListIndex = 0; _iPortListIndex < _iPortListSize; _iPortListIndex++ )
               {
                  _iLogicalPort = _iPortListIndex + 1;
                  _iPhysicalPort = _aiPortList[ _iPortListIndex ];
                  printf("\n   Logical Port: %d, Physical Port: %d", _iLogicalPort, _aiPortList[ _iPhysicalPort ] );

                  QLIB_AddPortAliasEntry( _iLogicalPort, _iPhysicalPort );
                  printf("\n QLIB_AddPortAliasEntry(): %s", bOk ? "PASS" : "FAIL" );
               }    // for ( unsigned short _iPortListIndex = 0; _iPortListIndex <= _iPortListSize; _iPortListIndex++ )

               // Try for logical port #1
               _iLogicalPort = 1;

               // Now connect to logical port #1
               printf("\n\nConnecting to logical port #%d", _iLogicalPort);
               g_hResourceContext = QLIB_ConnectServer_LogicalPort( _iLogicalPort );

               printf("\n QLIB_ConnectServer_LogicalPort(): %s", g_hResourceContext ? "PASS" : "FAIL" );
            }   // if ( cKey == 'W' )
         }  // if ( QLIB_GetAllPortList( &_iPortListSize, _aiPortList ) )
      } /// else if (cKey == 'V')
      else if (cKey == 'X')
      {
         // printf("\n#=Change COM port #");

         printf("\n\nConnect to COM port, with wait...");

         // Get the user input
         unsigned short _iNewPort = -1;
         printf("\n  Enter new port # (-1 for auto detect): ");
         scanf("%hd", & _iNewPort );

         // Remember the current handle, so it can be closed...don't close until
         // after the new connection is made though, so that the QPST server won't shut down
         // when the reference counters go to zero
         HANDLE _hOldHandle = g_hResourceContext;

         // Time out value
         unsigned long _iWait_ms = 60000;

         // Now connect to logical port #1
         printf("\n\nConnecting to COM port #%d, waiting %d ms", _iNewPort, _iWait_ms);
         g_hResourceContext = QLIB_ConnectServerWithWait( _iNewPort, _iWait_ms );

         printf("\n QLIB_ConnectServerWithWait(): %s", g_hResourceContext ? "PASS" : "FAIL" );

         // Disconnect old port
         QLIB_DisconnectServer( _hOldHandle );
      } /// else if (cKey == '#')

      else if (cKey == '#')
      {
         // printf("\n#=Change COM port #");

         printf("\n\nChange COM port...");

         // Get the user input
         unsigned short _iNewPort = -1;
         printf("\n  Enter new port #: ");
         scanf("%hd", & _iNewPort );

         // Remember the current handle, so it can be closed...don't close until
         // after the new connection is made though, so that the QPST server won't shut down
         // when the reference counters go to zero
         HANDLE _hOldHandle = g_hResourceContext;

         // Now connect to logical port #1
         printf("\n\nConnecting to COM port #%d", _iNewPort);
         g_hResourceContext = QLIB_ConnectServerWithWait( _iNewPort, 30 * 1000 );

         printf("\n QLIB_ConnectServer(): %s", g_hResourceContext ? "PASS" : "FAIL" );

         // Disconnect old port
         QLIB_DisconnectServer( _hOldHandle );
      } /// else if (cKey == '#')
      else if (cKey == '$')
      {
         // printf("\n$=Set NV_VCO_DEFAULT_I to 0");

         const unsigned short c_iNV_VCO_DEFAULT_I = 1792;
         const unsigned short c_iSizeNvData = 1;
         unsigned char _iNV_Data = 0;

         unsigned short _iStatus;

         // NV_WRITE
         bOk = QLIB_DIAG_NV_WRITE_F( g_hResourceContext, c_iNV_VCO_DEFAULT_I,  &_iNV_Data, c_iSizeNvData, &_iStatus );
         printf("\nQLIB_DIAG_NV_WRITE(): %s", bOk ? "PASS" : "FAIL\n" );
         printf("\t- Status of QLIB_DIAG_NV_WRITE(): %s", _iStatus == NV_DONE_S ? "DONE" : "FAIL\n" );
      }
      else if (cKey == '^')
      {
         // printf("\n^=Set NV_VCO_DEFAULT_I to 14");
         const unsigned short c_iNV_VCO_DEFAULT_I = 1792;
         const unsigned short c_iSizeNvData = 1;
         unsigned char _iNV_Data = 14;

         unsigned short _iStatus;

         // NV_WRITE
         bOk = QLIB_DIAG_NV_WRITE_F( g_hResourceContext, c_iNV_VCO_DEFAULT_I,  &_iNV_Data, c_iSizeNvData, &_iStatus );
         printf("\nQLIB_DIAG_NV_WRITE(): %s", bOk ? "PASS" : "FAIL\n" );
         printf("\t- Status of QLIB_DIAG_NV_WRITE(): %s", _iStatus == NV_DONE_S ? "DONE" : "FAIL\n" );
      }
      else if (cKey == '&')
      {
         unsigned short _iNV_Item = 0;
         const unsigned short c_iSizeRequest = 128;
         unsigned char _iNV_Data[ c_iSizeRequest ];

         // Get the item number
         printf(" Read NV item #: ");
         scanf( "%hd", &_iNV_Item );

         unsigned short _iStatus = NV_DONE_S;

         // First read the SPC unlock counter
         // NV ID, Data buffer, size, status
         if (QLIB_DIAG_NV_READ_F(  g_hResourceContext, _iNV_Item, _iNV_Data, c_iSizeRequest, &_iStatus ) )
         {
            printf("NV READ %d: PASS, status: %d\n", _iNV_Item, _iStatus);

            printf("\n  Data : " );
            for (int _iByteIndex = 0; _iByteIndex < c_iSizeRequest; _iByteIndex++)
            {
               printf("%d ", _iNV_Data[ _iByteIndex ] );
            }
         }
         else
            printf("NV READ %d: FAIL\n", _iNV_Item );
      }
      else if (cKey == '*')
      {
         printf("\n\nTimeout values (ms)");
         printf("\n Timeout[ QMSL_Timeout_General          ] = %d", QLIB_GetTimeOut( g_hResourceContext, QMSL_Timeout_General ) );
         printf("\n Timeout[ QMSL_Timeout_IsPhoneConnected ] = %d", QLIB_GetTimeOut( g_hResourceContext, QMSL_Timeout_IsPhoneConnected ) );
         printf("\n Timeout[ QMSL_Timeout_QPST_Connect     ] = %d", QLIB_GetTimeOut( g_hResourceContext, QMSL_Timeout_Connect ) );
         printf("\n Timeout[ QMSL_Timeout_Nonsignaling     ] = %d", QLIB_GetTimeOut( g_hResourceContext, QMSL_Timeout_Nonsignaling ) );
         printf("\n Timeout[ QMSL_Timeout_GSDI             ] = %d", QLIB_GetTimeOut( g_hResourceContext, QMSL_Timeout_GSDI ) );
         printf("\n Timeout[ QMSL_Timeout_CNV              ] = %d", QLIB_GetTimeOut( g_hResourceContext, QMSL_Timeout_CNV ) );
         printf("\n Timeout[ QMSL_Timeout_CDMA_FreqSweep   ] = %d", QLIB_GetTimeOut( g_hResourceContext, QMSL_Timeout_CDMA_FreqSweep ) );
         printf("\n Timeout[ QMSL_Timeout_SoftwareDownloadActivity  ] = %d", QLIB_GetTimeOut( g_hResourceContext, QMSL_Timeout_SoftwareDownloadActivity ) );
      }
      else if (cKey == '(')
      {
         // printf("\n(=Read ESN");

         unsigned long _iESN = 0;

         // Read the ESN, size, status
         if (QLIB_DIAG_READ_ESN_F(  g_hResourceContext, &_iESN ) )
         {
            printf("\nQLIB_DIAG_READ_ESN_F: PASS, ESN = 0x%X", _iESN );
         }
         else
            printf("\nQLIB_DIAG_READ_ESN_F: FAIL");
      }
      else if (cKey == ')')
      {
         // printf("\n)=Test loop for connect/disconnect");

         // Get the current COM port number
         unsigned short _iComPort = -1;
         QLIB_GetComPortNumber( g_hResourceContext, &_iComPort );

         // Disconnect the current connection
         QLIB_DisconnectServer( g_hResourceContext  );

         printf("\n Checking rapid open/close on COM%d, to test for releasing of threads, handles, and memory.  Press any key to quit",
                _iComPort );

         int _iCount = 0;

         while (!kbhit())
         {
            printf("\nOpen/Close loop iteration: %d", _iCount++ );

            // Connect to the desired COM port
            g_hResourceContext  = QLIB_ConnectServer( _iComPort );

            printf("\n   Connect Status = %s", QLIB_IsPhoneConnected(g_hResourceContext)  ? "CONNECTED" : "DISCONNECTED" );
            QLIB_DisconnectServer( g_hResourceContext  );
         };

         // Clear keyboard queue
         getch();
         unsigned long _iESN = 0;

         // Read the ESN, size, status
         if (QLIB_DIAG_READ_ESN_F(  g_hResourceContext, &_iESN ) )
         {
            printf("\nQLIB_DIAG_READ_ESN_F: PASS, ESN = 0x%X", _iESN );
         }
         else
            printf("\nQLIB_DIAG_READ_ESN_F: FAIL");
      }
      else if (cKey == '-')
      {
         // printf("\n-=Test receiver thread for rapid many commands responses");
         const int c_iCount = 10000;
         // Send the version command, which is easy to send and as a large response packet
         unsigned char _iRequest = _DIAG_EXT_BUILD_ID_F;

         // Counter
         unsigned long _iCount = 0;

         // Timer around the connection functions
         dword _hTimer1 = QLIB_StartTimer(g_hResourceContext );

         printf("\nAsync Sending Diag version command 1000 %d times", c_iCount );
         for ( _iCount = 0; _iCount < c_iCount; _iCount ++)
         {
            printf("\nloop iteration: %d", _iCount++ );

            // Send async
            QLIB_SendASync
            (
               g_hResourceContext,
               1,               // iRequestSize,
               &_iRequest,
               100,         // iTimeout,
               true         // bStartFlag
            );
         };

         // Print the timer
         QLIB_PrintTimer( g_hResourceContext, _hTimer1, "Loop time:");
      } // else if (cKey == '-')
      else if (cKey == '[')
      {
         // Automatically start the text log
         unsigned short _iLogFlags = LOG_FN | LOG_RET |  LOG_ERR | LOG_C_HIGH_LEVEL_START  | LOG_C_HIGH_LEVEL_STOP | LOG_ASYNC | LOG_IO;
         QLIB_SetLogFlags( g_hResourceContext, _iLogFlags );
         QLIB_StartLogging( g_hResourceContext,  "C:\\Factory_log.txt");

         // printf("\n[=Time profile for NV Write");
         const int c_iMaxCount = 1000;

         // Counter
         unsigned long _iCount = 0;

         // NV Setup
         const unsigned short c_iNV_FACTORY_DATA_1_I = 2497;
         const unsigned short c_iSizeNvData = 128;
         unsigned char _aNV_Data[c_iSizeNvData] =
         {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
         };

         // Status return value from NV Write function
         unsigned short _iItemStatus = NV_DONE_S;

         // Flag to control screen update during loop
         unsigned char _bShowLoopUpdate = true;

         // Timer around the connection functions
         dword _hTimer1 = QLIB_StartTimer(g_hResourceContext );

         unsigned long _iStartTime = 0;
         unsigned long _iStopTime = 0;

         printf("\nSending %d NV Write commands", c_iMaxCount );
         for ( _iCount = 0; _iCount < c_iMaxCount; _iCount ++)
         {
            // Get start time
            _iStartTime = QLIB_GetTimerElapsed( g_hResourceContext, _hTimer1 );

            // Send NV Write command
            bOk = QLIB_DIAG_NV_WRITE_F( g_hResourceContext, c_iNV_FACTORY_DATA_1_I, _aNV_Data, c_iSizeNvData, &_iItemStatus );

            // Get stop time
            _iStopTime = QLIB_GetTimerElapsed( g_hResourceContext, _hTimer1 );

            // Show the iteration number on the screen
            if ( _bShowLoopUpdate )
               printf("\nloop iteration: %d, took %d ms", _iCount, _iStopTime - _iStartTime );

            if ( _iItemStatus != NV_DONE_S )
            {
               printf("\nIteration %d failed NV Write", _iCount );
            }

            // break out if a key is pressed
            if (kbhit() )
               break;
         };

         // Calculate final values
         unsigned long _iTotalTime = QLIB_GetTimerElapsed( g_hResourceContext, _hTimer1 );
         double _dAverageTime = _iTotalTime / c_iMaxCount;

         printf("\nTotal time = %d (ms)", _iTotalTime );
         printf("\nTotal iterations = %d", c_iMaxCount );
         printf("\nAverage time = %6.2f (ms)", _dAverageTime );

         QLIB_StopTimer( g_hResourceContext, _hTimer1 );

         // Clear keyboard queue if a key was pressed
         if (kbhit() )
            getch();
      } // else if (cKey == '[')

      // printf("\nX=Wait for 60 seconds, for any phone to become connected");
   } // if (bOk )

   return bOk;
}

/**
TestSeq_Diag()  - Collection
*/
void TestSeq_FTM_State( void )
{
   printf("----------------\nFTM State Test\n----------------\n");

   unsigned char bOk=true;
   char cKey;

   if ( bOk )
   {
      printf("\n\n1=NV FTM OFF");
      printf("\n2=NV FTM ON");
      printf("\n3=MODE FTM");
      printf("\n4=MODE ONLINE");
      printf("\n5=MODE OFFLINE");
      printf("\n6=MODE RESET");
      printf("\n7=Switch to Download Mode");
      printf("\n8=NV_BT_ACTIVE_I = 0");
      printf("\n9=NV_BT_ACTIVE_I = 1");
      printf("\nA=MODE OFF");
      printf("\nB=IS FTM MODE");
      printf("\nC=Change FTM mode by Bootmode");
      printf("\nD=Change FTM mode by Runtime");
      printf("\nE=Set/Get client token value");
      printf("\nEnter Your choice->");
      cKey = toupper( getch() );

      if (cKey == '1' )
      {
         Test_NV_FTM_Mode( 0 );
      }// cKey == 1
      else if(cKey == '2')
      {
         Test_NV_FTM_Mode( 1 );
      }
      else if(cKey == '3')
      {
         bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_FTM_F );
         printf("\nQLIB_DIAG_CONTROL_F(  MODE_FTM_F ): %s", bOk ? "PASS" : "FAIL\n" );
      }
      else if(cKey == '4')
      {
         bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_ONLINE_F );
         printf("\nQLIB_DIAG_CONTROL_F(  MODE_ONLINE_F ): %s", bOk ? "PASS" : "FAIL\n" );
      }
      else if(cKey == '5')
      {
         bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_OFFLINE_D_F );
         printf("\nQLIB_DIAG_CONTROL_F(  MODE_OFFLINE_D_F ): %s", bOk ? "PASS" : "FAIL\n" );
      }
      else if(cKey == '6')
      {
         bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_RESET_F );
         printf("\nQLIB_DIAG_CONTROL_F(  MODE_RESET_F ): %s", bOk ? "PASS" : "FAIL\n" );
      }
      else if(cKey == '7')
      {
         bOk = QLIB_DIAG_DLOAD_F( g_hResourceContext  );
         printf("\nQLIB_DIAG_DLOAD_F( ): %s", bOk ? "PASS" : "FAIL\n" );
      }
      else if(cKey == '8')
      {
         unsigned char _iBT_Active = 0;
         unsigned short _iStatus;
         if ( QLIB_DIAG_NV_WRITE_F( g_hResourceContext, 2839, &_iBT_Active, sizeof( _iBT_Active), &_iStatus ) )
         {
            printf("NV NV Write(NV_BT_ACTIVe_I)=0: PASS\n");

            // Check the status
            if ( _iStatus == NV_DONE_S )
            {
               printf("NV Write Status: PASS\n");
            }
            else
            {
               printf("NV Write Status: FAIL\n");
            }
         }
         else
            printf("NV Write: FAIL\n");
      }
      else if(cKey == '9')
      {
         unsigned char _iBT_Active = 1;
         unsigned short _iStatus;
         if ( QLIB_DIAG_NV_WRITE_F( g_hResourceContext, 2839, &_iBT_Active, sizeof( _iBT_Active), &_iStatus ) )
         {
            printf("NV Write(NV_BT_ACTIVe_I)=1: PASS\n");

            // Check the status
            if ( _iStatus == NV_DONE_S )
            {
               printf("NV Write Status: PASS\n");
            }
            else
            {
               printf("NV Write Status: FAIL\n");
            }
         }
         else
            printf("NV Write: FAIL\n");
      }
      else if(cKey == 'A')
      {
         bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_POWER_OFF_F );
         printf("\nQLIB_DIAG_CONTROL_F(  MODE_POWER_OFF_F ): %s", bOk ? "PASS" : "FAIL\n" );
      }
      else if(cKey == 'B')
      {
         printf("\nEnter the command mode 0 or 1 (0 == FTM_COMMAND_75, 1 == FTM_COMMAND_59) ->");
         cKey = getch();
         int eCommandMode = FTM_COMMAND_75;
         if(cKey == '0')
            eCommandMode = FTM_COMMAND_75;
         else if(cKey == '1')
            eCommandMode = FTM_COMMAND_59;
         bOk = QLIB_FTM_SET_COMMAND_CODE( g_hResourceContext, eCommandMode);
         printf("\nQLIB_FTM_SET_COMMAND_CODE(  Cmd mode:%d ): %s", eCommandMode, bOk ? "PASS" : "FAIL\n" );
         unsigned char bFTM_Mode = 2;
         bOk = QLIB_IsFTM_Mode(g_hResourceContext, &bFTM_Mode);
         printf("\nQLIB_IsFTM_Mode returns bFTM_Mode = %d : %s", bFTM_Mode, bOk ? "PASS" : "FAIL\n" );
      }
      else if(cKey == 'C')
      {
         printf("\nEnter NV_FTM_MODE Value( 0 or 1 )->");
         cKey = getch();
         int iNV_FTM_Value = 1;
         if(cKey == '0')
            iNV_FTM_Value = 0;
         else if(cKey == '1')
            iNV_FTM_Value = 1;

         printf("\nReset after writing NV_FTM_MODE Value?( 0=No Reset or 1=Reset )->");
         cKey = getch();
         int bReset = 1;
         if(cKey == '0')
            bReset = 0;
         else if(cKey == '1')
            bReset = 1;
         bOk = QLIB_ChangeFTM_BootMode(g_hResourceContext, iNV_FTM_Value, bReset );
         printf("\nQLIB_ChangeFTM_BootMode(  NV_FTM_MODE:%d, RESET:%d ): %s", iNV_FTM_Value, bReset,  bOk ? "PASS" : "FAIL\n" );
      }
      else if(cKey == 'D')
      {
         printf("\nEnter FTM (1) or ONLINE (0) mode->");
         cKey = getch();
         int iMode = 1;
         if(cKey == '0')
            iMode = 0;
         else if(cKey == '1')
            iMode = 1;
         bOk = QLIB_ChangeFTM_ModeRuntime(g_hResourceContext, iMode );
         printf("\nQLIB_ChangeFTM_ModeRuntime(  Mode:%d ): %s", iMode, bOk ? "PASS" : "FAIL\n" );
      }
      else if(cKey == 'E')
      {
         // printf("\nE=Set/Get client token value");
         /*
         This number represents a number that is meaningful to the client software, such
         as a pointer to a class.
         */
         unsigned long _iClientTokenValue = 0x12345678;
         unsigned long _iClientTokenValue_readBack = 0x0;

         // Set the token
         bOk = QLIB_SetClientToken( g_hResourceContext, _iClientTokenValue );
         printf(        "\nQLIB_SetClientToken(  iToken = 0x%X ): %s",
                        _iClientTokenValue,
                        bOk ? "PASS" : "FAIL\n"
               );

         // Get the token
         bOk = QLIB_GetClientToken( g_hResourceContext, &_iClientTokenValue_readBack);
         printf(        "\nQLIB_GetClientToken( ): %s, value = 0x%X",
                        bOk ? "PASS" : "FAIL\n",
                        _iClientTokenValue_readBack
               );
      }
   } // if (bOk )

   return;
}

void Test_NV_FTM_Mode( char iNewFtmMode )
{
   // Write NV -- FTM Mode = 0x00
   unsigned char _iFTM_Mode = iNewFtmMode;
   unsigned short _iStatus = -1;
   long iCount = 0;

   const unsigned short c_iNV_FTM_MODE_I = 453;

   printf("\n----------------\nNV FTM = %s\n----------------\n", iNewFtmMode==1 ? "ON" : "OFF" );

   // Mode OFFLINE-DIGITAL
   if (QLIB_DIAG_CONTROL_F( g_hResourceContext, 1 ) )
      printf("Mode OFFLINE-D: PASS\n");
   else
      printf("Mode OFFLINE-D': FAIL\n");

   // NV Write
   if ( QLIB_DIAG_NV_WRITE_F( g_hResourceContext, c_iNV_FTM_MODE_I, &_iFTM_Mode, sizeof( _iFTM_Mode), &_iStatus ) )
   {
      printf("NV Write: PASS\n");

      // Check the status
      if ( _iStatus == NV_DONE_S )
      {
         printf("NV Write Status: PASS\n");
      }
      else
      {
         printf("NV Write Status: FAIL\n");
      }
   }
   else
      printf("NV Write: FAIL\n");

   // Mode RESET
   if (QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_RESET_F ) )
      printf("Mode RESET: PASS\n");
   else
      printf("Mode RESET': FAIL\n");

   printf("Waiting for phone to reset..");

   // Need to delay here for a moment
   for (iCount = 1; iCount< 600000000; iCount++) {}

   // Wait for phone to come back online--might need a time-out here
   while ( !QLIB_IsPhoneConnected(g_hResourceContext) && !kbhit() )
   {
      Sleep(10);
   };

   // Clear keypress from queue if necessary
   if (kbhit()) getch();

   // Force a new line since the status was running before
   printf("\n");

   // Note, having a problem here if the delay time is not long enough, an
   // exception will be thrown

   // NV ID, Data buffer, size, status
   if (QLIB_DIAG_NV_READ_F(  g_hResourceContext, 453, &_iFTM_Mode, 1, &_iStatus ) )
      printf("NV READ FTM MODE: PASS\n");
   else
      printf("NV READ FTM MODE: FAIL\n");

   if (_iFTM_Mode == iNewFtmMode)
      printf("NV READ FTM MODE = %d: PASS\n",iNewFtmMode);
   else
      printf("NV READ FTM MODE = %d: FAIL\n",iNewFtmMode);
}

/**
TestVersion()
*/
void TestVersion( void )
{
   printf("\n----------------\nVersion Report\n----------------\n");

   // On the string fields, include an extra character for a NULL termination
   char comp_date[ 12 ];
   char comp_time[ 9 ];
   char rel_date [ 12 ];
   char rel_time [ 9 ];
   char ver_dir  [ 9 ];
   unsigned char scm;
   unsigned char mob_cai_rev;
   unsigned char mob_model;
   unsigned short mob_firm_rev;
   unsigned char slot_cycle_index;
   unsigned char voc_maj;
   unsigned char voc_min;
   unsigned char bOk;

   unsigned char _bDownload = false;    // Indicates download mode
   unsigned char _bDiag = false;        // Indicates diagnostic mode6

   char sLibraryVersion[50];
   // Get the library version
   QLIB_GetLibraryVersion(sLibraryVersion);
   printf("\nQLIB DLL Version: %s", sLibraryVersion );

   // Print the COM port number
   unsigned short _iComPort = -1;
   QLIB_GetComPortNumber( g_hResourceContext, &_iComPort );

   // If still auto detect, then try to find the COM port
   if ( _iComPort == QLIB_COM_AUTO_DETECT )
   {
      printf("\nAuto detect selected, no phone has been found, searching...\n");
      // Auto was selected, but a phone has not been detected
      g_hResourceContext  = QLIB_ConnectServer( _iComPort );
   }    // if ( _iComPort == QLIB_COM_AUTO_DETECT )

   // Get the COM port number
   QLIB_GetComPortNumber( g_hResourceContext, &_iComPort );
   if ( _iComPort == QLIB_COM_AUTO_DETECT )
   {
      printf("\nCOM Port: AUTO, NOT DETECTED\n"  );
      return;
   }

   printf("\nCOM Port: %d\n", _iComPort );

   _bDiag=QLIB_IsPhoneConnected( g_hResourceContext );

   if (!_bDiag )
   {
      // Clear the serial Rx buffer-- don't check return value because it is not enabled for QPST version
      QLIB_FlushRxBuffer( g_hResourceContext  );

      printf("<<Waiting for phone, press any key to exit>>\n");

      while ( !_bDownload && !_bDiag && !kbhit() )
      {
         _bDiag = QLIB_IsPhoneConnected( g_hResourceContext );

         // Disabled for now because pinging the phone in download mode isn't working.  Need further research
         // _bDownload = QLIB_DOWNLOAD_Ping(  g_hResourceContext );

         // Now try the QPST SW Download
         unsigned long _ePhoneMode;
         bool _bQpstPhoneStatusOk = QLIB_GetQpstPhoneStatus( g_hResourceContext, &_ePhoneMode ) == TRUE;

         _bDownload = (_bQpstPhoneStatusOk && ( _ePhoneMode != 0 ) );

         // Without this, the CPU will be 100%
         Sleep( 10 );
      }

      // clear the keyboard queue
      if (kbhit())
         getch();
   }

   if ( _bDiag )
   {
      if (QLIB_IsPhoneConnected( g_hResourceContext ) )
      {
         // Force the phone into ONLINE mode
         // Go into FTM mode
         if ( !QLIB_DIAG_VERNO_F( g_hResourceContext,
                                  comp_date,  comp_time,  rel_date,   rel_time, ver_dir,
                                  &scm, &mob_cai_rev, &mob_model, &mob_firm_rev,&slot_cycle_index,
                                  &voc_maj, &voc_min
                                )
            )
         {
            printf("Version Number: FAIL\n" );
         }
         else
         {
            // Set null terminations for each string
            comp_date[11] = '\0';
            comp_time[ 8 ] = '\0';
            rel_date [ 11 ] = '\0';
            rel_time [ 8 ] = '\0';
            ver_dir  [ 8 ] = '\0';

            // Print values
            printf("Mobile Version report: PASS\n"      );
            printf("Compile Date: %s\n", comp_date      );
            printf("Compile Time: %s\n", comp_time      );
            printf("Release Date: %s\n", rel_date       );
            printf("Release Time: %s\n", rel_time       );
            printf(" Version Dir: %s\n", ver_dir        );

            printf("\n\n");

            printf("   Station Class Mark: %d\n", scm               );
            printf("              CAI Rev: %d\n", mob_cai_rev       );
            printf("         Mobile Model: %d\n", mob_model         );
            printf("         Firmware Rev: %d\n", mob_firm_rev      );
            printf("     Slot Cycle Index: %d\n", slot_cycle_index  );
            printf("Vocoder major version: %d\n", voc_maj           );
            printf("Vocoder minor version: %d\n", voc_min           );

            // Get extended version info
            unsigned long _iMSM_HW_Version = 0;
            unsigned long _iMobModel = 0;
            char _sMobSwRev[512];
            char _sModelStr[512];

            bOk = QLIB_DIAG_EXT_BUILD_ID_F( g_hResourceContext, &_iMSM_HW_Version, &_iMobModel, _sMobSwRev, _sModelStr );

            // Print the results
            if (bOk)
            {
               printf("\n");
               printf("      Ext MSM HW Version: 0x%X\n", _iMSM_HW_Version  );
               printf("       Ext Mobile Model#: 0x%X\n", _iMobModel        );
               printf("Ext Mobile Mobile SW Rev: %s\n", _sMobSwRev      );
               printf("            Ext Model ID: %s\n", _sModelStr      );

               // Report the phone status
               ReportPhoneState();
            }
            else
               printf("\nQLIB_DIAG_EXT_BUILD_ID_F = FAIL");

            printf("\n");
         }  // !QLIB_DIAG_VERNO_F
      }
      else
      {
         printf("\n Phone is not connected.");
      } // if (QLIB_IsPhoneConnected() )
   } // if (_bDiag )
   else if ( _bDownload)
   {
      printf("\n Phone is in download mode.");

      ReportPhoneState();
   }
}

void TestSendSync( void )
{
   char cKey = HS_PWR_ON_K; //  HS_PWR_OFF_K;
   static unsigned char aiRequestBuffer  [ DIAG_MAX_PACKET_SIZE ];
   static unsigned char aiResponseBuffer [ DIAG_MAX_PACKET_SIZE ];
   static short iRequestSize  = 3;  // Size of request packet
   static short iResponseSize = 0;  // This will get updated by SendSync

   printf("----------------\nSend Sync Test\n----------------\n");

   // Check for phone connected
   if (QLIB_IsPhoneConnected(g_hResourceContext) )
   {
      printf("Phone Connected = PASS\n");

      // Prepare a packet for Send Sync
      aiRequestBuffer[0] = _DIAG_VERNO_F;               // CMD_CODE = version info
      iRequestSize  = 1;
      iResponseSize = DIAG_MAX_PACKET_SIZE;

      // Send the command
      if ( QLIB_SendSync( g_hResourceContext, iRequestSize, aiRequestBuffer, &iResponseSize, aiResponseBuffer,2000 ) )
      {
         printf("\nSend Sync (version info): PASS\n");
      }
      else
      {
         printf("\nSend Sync (version info): FAIL\n");
      }

      // Prepare a packet for Send Sync
      aiRequestBuffer[0] = _DIAG_HS_KEY_F;      // CMD_CODE = DIAG_HS_KEY_F
      aiRequestBuffer[1] = 0;                       // HOLD = 0
      aiRequestBuffer[2] = cKey;                    // KEY = variable
      iRequestSize  = 3;
      iResponseSize = DIAG_MAX_PACKET_SIZE;

      // Send the command
      if ( QLIB_SendSync( g_hResourceContext, iRequestSize, aiRequestBuffer, &iResponseSize, aiResponseBuffer,2000 ) )
      {
         printf("\nSend Sync: PASS\n");
      }
      else
      {
         printf("\nSend Sync (version info): FAIL\n");
      }
   }
   else
      printf("Phone Connected = FAIL\n");
}

unsigned char Test_FTM_WCDMA( int iChannel )
{
   unsigned char bOk = true;
   unsigned short iADC_Value;
   unsigned short iADC_Channel = 5;
   unsigned short iRxAGC = 0;
   unsigned char iSynthLock = 0;
   short iExpectedRX_AGC = 0;
   short iDVGAoffset;
   short iLNAoffsetValue = 0;
   unsigned short iHDETvalue = 0;
   unsigned char  cKey = ' ';
   short iRxAGC_Signed  = 0;
   double _dRxPowerLevels[] = {-73.0, -69.0, -48.0,  -32.0 };
   short _iRxAgcLevels[]    = {-115,  -67,   184,    376   };
   short iMode = PHONE_MODE_WCDMA_IMT ;

   printf("----------------\nWCDMA FTM Test\n----------------\n");

   if ( bOk )
   {
      // RF Mode

      /*--for PCS
      iChannel = 9262;
      iMode = PHONE_MODE_WCDMA_1900B;
      */

      if ( !QLIB_FTM_SET_MODE( g_hResourceContext, iMode ))
      {
         bOk = false;
         printf("\nQLIB_FTM_SET_MODE failed\n");
      }

      // Set the frequency channel
      if ( !QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel))
      {
         bOk = false;
         printf("\nQLIB_FTM_SET_CHAN failed\n");
      }

      // Verify synths are locked
      if ( !QLIB_FTM_GET_SYNTH_STATE( g_hResourceContext, &iSynthLock ))
      {
         bOk = false;
         printf("\nQLIB_FTM_GET_SYNTH_STATE failed\n");
      }

      printf("\nSynth State: %s", ( iSynthLock==1 ? "Locked" : "Unlocked" ) );

      printf("\nWCDMA is on channel %d", iChannel);
      printf("\nHelper: ch 9739 is 1947.8MHz.\n" );

      printf("\n\n1=Tx RF");
      printf("\n2=Rx");
      printf("\n3=Tx Rx Frequency Calibration");
      printf("\n4=WCDMA AGC Report");
      printf("\n5=QLIB_FTM_DO_DC_CAL");
      printf("\n6=QLIB_FTM_GET_CTON");

      cKey = getch();

      if (cKey == '1' )
      {
         // Set Tx AGC PDM to 300
         bOk &= QLIB_FTM_SET_PDM(g_hResourceContext, 2, 300);

         // PA Range
         bOk &= QLIB_FTM_SET_PA_RANGE( g_hResourceContext, 0 );

         // Set Tx On
         bOk &= QLIB_FTM_SET_TX_ON( g_hResourceContext );

         bOk &= QLIB_FTM_CDMA_CW_WAVEFORM( g_hResourceContext, 0 );

         if ( !bOk )
         {
            printf("Error occurred setting up transmitter.");
         }
         else
         {
            // Get a key
            printf("\nWCDMA Tx is on press any key to continue.\n" );
            getch();

            bOk &= QLIB_FTM_CDMA_CW_WAVEFORM( g_hResourceContext, 1 );
            printf("\nCW Tx is on channel %d, press any key to continue.\n", iChannel);
            getch();

            // Read ADC on channel 5
            bOk &= QLIB_FTM_GET_ADC_VAL( g_hResourceContext, iADC_Channel, &iADC_Value );
            printf("\nADC on Channel %d is %d", iADC_Channel, iADC_Value );

            // Configure Tx Sweep PDM step size--this is not on older 6250 targets
            bOk = QLIB_FTM_SET_TX_SWEEP_STEP_SIZE( g_hResourceContext, 10 );    // TX_AGC_PDM step = 10
            printf("\nQLIB_FTM_SET_TX_SWEEP_STEP_SIZE: - %s", bOk ? "PASS" : "FAIL" );

            // Do the Tx Sweep cal
            printf("\nPress any key for Tx Sweep cal.\n" );
            getch();
            bOk = QLIB_FTM_TX_SWEEP_CAL( g_hResourceContext, 1 );       // On

            // Do the Tx Sweep cal
            printf("\nPress any key to Tx Sweep cal mode.\n" );
            getch();

            bOk &= QLIB_FTM_TX_SWEEP_CAL( g_hResourceContext, 0 );      // Off

            // Read the first HDET value
            bOk &= QLIB_FTM_GET_HDET_FROM_TX_SWEEP_CAL( g_hResourceContext, 0, &iHDETvalue );
            printf("\nTx Sweep cal HDET[0] = % d\n", iHDETvalue );

            // Set Tx On
            bOk &= QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         } // !bOk
      }// cKey == 1
      else if (cKey == '2')
      {
         double _dRxPowerLevels[] = {-73.0, -69.0, -48.0,  -32.0 };
         short _iRxAgcLevels[]    = {-115,  -67,   184,    376   };

         // Get the DVGA Offset
         printf("\nHelper:  Downlink for CH 9739 is 2137MHz\n" );
         printf("\nSet DVGA Power (%6.2fdBm) then press any key to continue.\n", _dRxPowerLevels[0], iChannel);
         getch();
         iExpectedRX_AGC = _iRxAgcLevels[0];
         bOk &= QLIB_FTM_GET_DVGA_OFFSET( g_hResourceContext, iExpectedRX_AGC, &iDVGAoffset );
         printf("\nDVGA Offset: %d, FTM expected parameter: %d", iDVGAoffset,iExpectedRX_AGC );

         // Get the LNA offset
         for (int iLnaRange = 1; iLnaRange <=3; iLnaRange++)
         {
            printf("\n\nSet DL power (%6.2fdBm) for LNA range %d then press any key to continue.", _dRxPowerLevels[iLnaRange ],iLnaRange );
            getch();
            bOk &= QLIB_FTM_GET_LNA_OFFSET( g_hResourceContext, iLnaRange, _iRxAgcLevels[iLnaRange], &iLNAoffsetValue );
            printf("\nLNA %d Offset: %d", iLnaRange, iLNAoffsetValue  );
         }

         // Get the RX AGC Value
         QLIB_FTM_GET_RX_AGC( g_hResourceContext, &iRxAGC );

         // Convert to a signed number

         iRxAGC_Signed = iRxAGC;

         // Check for sign bit
         if ( iRxAGC  & 0x200 )
         {
            // convert from 2's comp
            iRxAGC_Signed &= 0x1ff;
            iRxAGC_Signed = iRxAGC_Signed - 512;
         }

         printf("\n  Measured Rx AGC value (signed): %d", iRxAGC_Signed );
         printf("\nMeasured Rx AGC value (unsigned): %d", iRxAGC );
      } // cKey == 2
      else if (cKey == '3' )
      {
         printf("\nStarting TX RX Frequency Sweep...Use QMSL text log to view results, include 0x08 in the log level");

         // Create the param/result  structure
         FTM_Tx_Rx_Freq_Cal_Sweep_Request_Response _oParams;

         /*
         Adapted from the FTM GUI file, tx_rx_freq_cal_params_wcdma.txt.  File contents:
         16
         5
         655
         9621,9638,9656,9673,9691,9708,9726,9743,9761,9778,9796,9813,9831,9848,9866,9883
         450,475, 415,430,0
         -115,-67,184,376,0
         0, 0, 0, 0, 0
         1, 1, 3, 3, 255
         0, 1, 1, 1, 0
         100, 1, 2, 3, 255
         */

         const int c_iNumChannels = 16;
         unsigned short _aiChannelList[ c_iNumChannels]  = { 9621,9638,9656,9673,9691,9708,9726,9743,9761,9778,9796,9813,9831,9848,9866,9883};

         // Maximum segments is 6, but for this example, will use 5
         const int c_iNumSteps = 5;
         unsigned short _aiTxPDM_List[ c_iNumSteps ]    = {450,475, 415,430,0};
         short          _aiPrimaryExpectedAGC_ValList[ c_iNumSteps ] = {-115,-67,184,376,0};
         unsigned char  _aiPA_RangeList[ c_iNumSteps ]  = {1, 1, 3, 3, 255};
         unsigned char  _aiReadHDET_List[ c_iNumSteps ] = {0, 1, 1, 1, 0};
         unsigned char  _aiRxActionList[ c_iNumSteps ]  = {100, 1, 2, 3, 255};

         /* 10 steps instead of 5
         10
         16
         10
         655
         9621,9638,9656,9673,9691,9708,9726,9743,9761,9778,9796,9813,9831,9848,9866,9883
         450,475,415,430,410,415,420,425,430,0
         -115,-67,184,376,0,0,0,0,0,0
         0, 0, 0, 0, 0,0,0,0,0,0
         1, 1, 3, 3, 3,3,3,3,3,255
         0, 1, 1, 1, 1,1,1,1,1,0
         100, 1, 2, 3, 255, 255, 255, 255, 255
         */
         /*
         const int c_iNumChannels = 16;
         unsigned short _aiChannelList[ c_iNumChannels]  = { 9621,9638,9656,9673,9691,9708,9726,9743,9761,9778,9796,9813,9831,9848,9866,9883};

         // Maximum segments is 6, but for this example, will use 5
         const int c_iNumSteps = 8;
         unsigned short _aiTxPDM_List[ c_iNumSteps ]    = {450,475,415,425,430,435,440,0};
         short          _aiPrimaryExpectedAGC_ValList[ c_iNumSteps ] = {-115,-67,184,376,0,0,0,0};
         unsigned char  _aiPA_RangeList[ c_iNumSteps ]  = {1, 1, 3, 3, 3,3,3,255};
         unsigned char  _aiReadHDET_List[ c_iNumSteps ] = {1, 1, 1, 1, 1,1,1,1};
         unsigned char  _aiRxActionList[ c_iNumSteps ]  = {100, 1, 2, 3, 255, 255, 255};
         */

         /* 5 steps, WCDMA PCS
         16
         5
         655
         9262,9280,9299,9317,9336,9354,9372,9391,9409,9428,9446,9464,9483,9501,9520,9538
         450,440, 430,420,0
         -115,-67,184,376,0
         0, 0, 0, 0, 0
         1, 1, 3, 3, 255
         0, 1, 1, 1, 0
         100, 1, 2, 3, 255
         */
         /*
         const int c_iNumChannels = 16;
         //unsigned short _aiChannelList[ c_iNumChannels]  = { 9262,9280,9299,9317,9336,9354,9372,9391,9409,9428,9446,9464,9483,9501,9520,9538};
         unsigned short _aiChannelList[ c_iNumChannels]  = { 9621,9638,9656,9673,9691,9708,9726,9743,9761,9778,9796,9813,9831,9848,9866,9883};

         // Maximum segments is 6, but for this example, will use 5
         const int c_iNumSteps = 5;
         unsigned short _aiTxPDM_List[ c_iNumSteps ]    = {450,440, 430,420,0};
         short          _aiPrimaryExpectedAGC_ValList[ c_iNumSteps ] = {-115,-67,184,376,0};
         unsigned char  _aiPA_RangeList[ c_iNumSteps ]  = {1, 1, 3, 3, 255};
         unsigned char  _aiReadHDET_List[ c_iNumSteps ] = {0, 1, 1, 1, 0};
         unsigned char  _aiRxActionList[ c_iNumSteps ]  = {100, 1, 2, 3, 255};
         */

         // These fields apply to all of the setups above.
         unsigned char  _iRxChain = FTM_TX_RX_FREQ_RX_CHAIN_0;

         unsigned char  _aiRxPowerMode[ c_iNumSteps ]   = { FTM_TX_RX_FREQ_RX_PM_High,
                                                            FTM_TX_RX_FREQ_RX_PM_High,
                                                            FTM_TX_RX_FREQ_RX_PM_High,
                                                            FTM_TX_RX_FREQ_RX_PM_High,
                                                            FTM_TX_RX_FREQ_RX_PM_High
                                                          };

         int _iIndex = 0;

         //
         // Copy data into the request structure
         //

         // Fill the basic parameters
         _oParams.iRxChain = _iRxChain;
         _oParams.iNumFreqs = c_iNumChannels;
         _oParams.iNumSteps = c_iNumSteps;
         _oParams.iStepLength = 655;

         // aiChannelList
         for ( _iIndex = 0; _iIndex < c_iNumChannels; _iIndex++ )
            _oParams.aiChannelList[ _iIndex ]  = _aiChannelList[ _iIndex ];

         // _iNumSteps
         for ( _iIndex = 0; _iIndex < c_iNumSteps; _iIndex++ )
            _oParams.aiTxPDM_List[ _iIndex ] = _aiTxPDM_List[ _iIndex ];

         // aiPrimaryExpectedAGC_ValList
         for ( _iIndex = 0; _iIndex < c_iNumSteps; _iIndex++ )
            _oParams.aiExpectedRxAGC_ValList[ _iIndex ] = _aiPrimaryExpectedAGC_ValList[ _iIndex ];

         //aiPA_RangeList
         for ( _iIndex = 0; _iIndex < c_iNumSteps; _iIndex++ )
            _oParams.aiPA_RangeList[ _iIndex ] = _aiPA_RangeList[ _iIndex ];

         // aiPrimaryExpectedAGC_ValList
         for ( _iIndex = 0; _iIndex < c_iNumSteps; _iIndex++ )
            _oParams.aiReadHDET_List[ _iIndex ] = _aiReadHDET_List[ _iIndex ];

         // aiRxActionList
         for ( _iIndex = 0; _iIndex < c_iNumSteps; _iIndex++ )
            _oParams.aiRxActionList[ _iIndex ] = _aiRxActionList[ _iIndex ];

         // aiRxPowerMode
         for ( _iIndex = 0; _iIndex < c_iNumSteps; _iIndex++ )
            _oParams.aiRxPowerMode[ _iIndex ] = _aiRxPowerMode[ _iIndex ];

         // Call the frequency sweep function, cast the parameters as void pointer
         bOk = QLIB_FTM_TX_RX_FREQ_CAL_SWEEP( g_hResourceContext, (void*) &_oParams );
         printf("\nQLIB_FTM_TX_RX_FREQ_CAL_SWEEP: %s", (bOk ? "PASS" : "FAIL") );
      }// cKey == 3
      else if (cKey == '4' )
      {
         PrintWCDMA_AGC();
      } // cKey == 4
      else if (cKey == '5' )
      {
         // DO DC Calibration
         bOk &= QLIB_FTM_DO_DC_CAL(g_hResourceContext );
         printf("\nQLIB_FTM_DO_DC_CAL(): %s", bOk ? "PASS" : "FAIL" );
      }// cKey == 5
      else if (cKey == '6' )
      {
         // printf("\n6=QLIB_FTM_GET_CTON");

         long _iCtoN = 0;
         bOk = QLIB_FTM_GET_CTON( g_hResourceContext, &_iCtoN);
         printf("\nQLIB_FTM_GET_CTON() %s",  bOk ? "PASS" : "FAIL\n");
         if ( bOk )
         {
            printf("Note: The value below is 10 * (C/N) dB\n");
            printf("C/N is %d\n", _iCtoN );
         }
      }// cKey == 5
   }

   return bOk;
}


unsigned char Test_Gnss_Dispatch()
{
   unsigned char bOk=true;
   unsigned char cKey;

   if ( bOk )
   {
      do
      {
         printf("\n\n----------------\nIzAt GNSS Engine Tests\n----------------\n");
         printf("\n1 = FTM AGPS");
         printf("\n2 = FTM GPS");
         printf("\n3 = IS801 GPS");
         printf("\n4 = gpsOne Gen 7 Engine Test");
         printf("\n5 = gpsOne Gen 8 Engine Test");
         printf("\n6 = IzAt GNSS_SOC Engine Test");
         printf("\nQ = Exit this menu");

         cKey = toupper( getch() );

         // Reset the OK flag
         bOk = true;

         printf("\n\n\n\n");
         switch (cKey)
         {
            case '1':
               // AGPS
               Test_FTM_AGPS( );
               break;
            case '2':
               // FTM GPS
               Test_FTM_GPS( );
               break;
            case '3':
               // IS801 GPS test and calibration
               Test_IS801_GPS( );
               break;
            case '4':
               // CGPS Gen 7 (Aries)
               Test_CGPS( );
               break;
            case '5':
               // CGPS Gen 8 (Callisto)
               Test_CGPS_Gen8( );
               break;
            case '6':
               // IzAt GNSS_SOC (Griffon, etc.)
               Test_GNSS_SOC( );
               break;
            default:
               // Do nothing
               break;
         }
      }
      while (cKey != 'Q' );
   } // if (bOk )

   return bOk;
}

unsigned char Test_FTM_AGPS( void )
{
   unsigned char bOk=true;
   unsigned char cKey;

   char _sOutputFile[] = "c:\\agps_iq_capture.txt";

   printf("----------------\nAGPS FTM Test\n----------------\n");

   if ( bOk )
   {
      printf("\n\n1=FTM_AGPS_SET_TEST_MODE( ON )\n2=FTM_AGPS_SET_TEST_MODE( OFF )\n3=FTM_AGPS_SELF_TEST\n4=FTM_AGPS_IDLE_MODE\n5=FTM_AGPS_STANDALONE_TEST_MODE\n6=FTM_AGPS_SV_TRACKING(1)");
      printf("\n7=Capture AGPS Log Messages");
      printf("\n8=ONLINE Start GPS Test and Capture AGPS Log Messages");
      printf("\n9=AGPS C/N ");
      printf("\nA=AGPS C/N using QLIB_SendSync()");
      printf("\nB=AGPS IQ Capture to file %s", _sOutputFile);
      cKey = toupper( getch() );

      if (cKey == '1' )
      {
         bOk = QLIB_FTM_AGPS_SET_TEST_MODE( g_hResourceContext, 1 );
      }// cKey == 1
      else if (cKey == '2')
      {
         bOk = QLIB_FTM_AGPS_SET_TEST_MODE( g_hResourceContext, 0 );
      } // cKey == 2
      else if (cKey == '3')
      {
         byte _bSelfTestPass = false;
         bOk = QLIB_FTM_AGPS_SELF_TEST( g_hResourceContext, &_bSelfTestPass );

         printf("\nSelf Test Status: %d", _bSelfTestPass );
      } // cKey == 3
      else if (cKey == '4')
      {
         bOk = QLIB_FTM_AGPS_IDLE_MODE( g_hResourceContext );
      } // cKey == 4
      else if (cKey == '5')
      {
         bOk = QLIB_FTM_AGPS_STANDALONE_TEST_MODE( g_hResourceContext );
      } // cKey == 5
      else if (cKey == '6')
      {
         bOk = QLIB_FTM_AGPS_SV_TRACKING( g_hResourceContext, 1);
      } // cKey == 6
      else if (cKey == '7')
      {
         unsigned short iChannel = 9739;

         // Setup WCDMA mode and send the GPS commands
         printf("\n\nSetting up WCDMA mode and GPS Standalone...\n\n");

         // RF Mode, WCDMA - IMT
         bOk = QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_WCDMA_IMT );
         printf("\nQLIB_FTM_SET_MODE(WCDMA): %s", bOk ? "PASS" : "FAIL" );

         // Set the frequency channel
         bOk = QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel);
         printf("\nQLIB_FTM_SET_CHAN(%d): %s", iChannel, bOk ? "PASS" : "FAIL" );

         /* // enable this code to track all SV's
         bOk = QLIB_FTM_AGPS_STANDALONE_TEST_MODE( g_hResourceContext );
         printf("\nQLIB_FTM_AGPS_STANDALONE_TEST_MODE: %s", bOk ? "PASS" : "FAIL" );
         */

         // Track only SV ID 1
         bOk = QLIB_FTM_AGPS_SV_TRACKING( g_hResourceContext, 1 );
         printf("\nQLIB_FTM_AGPS_SV_TRACKING(1): %s", bOk ? "PASS" : "FAIL" );

         // See below for setup of logging for capturing C/N and other metrics
      }
      else if (cKey == '8')
      {
         bOk = QLIB_GPS_SET_TEST_MODE_PARAMS(
                  g_hResourceContext,
                  1,  // iDeleteAlm
                  1,  // iDeleteEph,
                  1,  // char iDeletePos
                  1,  // iDeleteTime,
                  1,  // iDeleteIono,
                  0,  // iTimeUnc,
                  0,  // iPosUnc
                  0,  // iTimeOffset
                  0   //short iPosOffset
               );
         printf("\nQLIB_GPS_SET_TEST_MODE_PARAMS(): %s", bOk ? "PASS" : "FAIL" );
      } // cKey == 8

      // This section sets up the log capturing for GPS Measurements & displays messages
      if (  (cKey == '7')
            || (cKey == '8')
         )
      {
         unsigned short _aiAGPS_Logs[] =
         {
            _DIAG_LOG_AAGPS_MEASUREMENT,        // Enable the version 1 and version 3 log codes
            _DIAG_LOG_AAGPS_MEASUREMENT_V3
         };

         // Enable multiple logs because the measurement report can come
         bOk = QLIB_DIAG_AddExtendedLogCodes(
                  g_hResourceContext,
                  LOG_EQUIP_ID_UMTS,
                  _aiAGPS_Logs,
                  2   // Number of log items
               );
         printf("\nQLIB_DIAG_AddExtendedLogCodes(): %s", bOk ? "PASS" : "FAIL" );

         const unsigned short c_iDM_MaxLogSize = DIAG_MAX_PACKET_SIZE;
         unsigned char pLogBuffer[c_iDM_MaxLogSize];
         unsigned short iLogSize = c_iDM_MaxLogSize;
         const unsigned long c_iTimeOut = 20000;
         char _pTextOutput[10000];      // For printing to screen
         unsigned long _iIterationCount = 0;

         bOk = QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
         printf("\nQLIB_DIAG_SetPhoneLoggingState(  ON): %s", bOk ? "PASS" : "FAIL" );

         printf("\nCapturing...press any key to quit");

         // Wait for keypress on PC
         while ( !kbhit() )
         {
            if ( QLIB_DIAG_GetNextPhoneLog( g_hResourceContext, &iLogSize, pLogBuffer, c_iTimeOut ) )
            {
               printf("\n-------------------------------------------------------\n");

               // Make sure at least one byte is returned
               if (( iLogSize > 0)  )
               {
                  // The possibilities for message types are:
                  //   1) Value _DIAG_MSG_F=31, for a streamed message
                  //   2) Value _DIAG_LOG_F=16, for a log message
                  //   3) Value _DIAG_EVENT_REPORT_F=96, for an event message
                  //   4) Value _DIAG_EXT_MSG_F=121, for an exteneded

                  // Prepare a text buffer
                  PrepareByteListString( _pTextOutput, pLogBuffer, iLogSize );

                  // Don't pring the entire byte buffer unless necessary for debug
                  // printf("\nItem #: %d", ++_iIterationCount);
                  // printf("\n  Data: %s\n", _pTextOutput );

                  // Declare different structures for accessing the log data
                  LogMessage* _pLogMessage = (LogMessage*) pLogBuffer;

                  if ( _pLogMessage->CMD_CODE ==_DIAG_LOG_F )
                  {
                     printf("\nLog Message 0x%X detected, size: %d bytes", _pLogMessage->log_item, _pLogMessage->length);

                     // Check for the AGPS Measurement Log Message
                     if ( _pLogMessage->log_item == _DIAG_LOG_AAGPS_MEASUREMENT )
                     {
                        // Assign a structure to the log data, to facilitate interpratation
                        MMGPS_ReportMeasurement* _pMMGPS_Measurement = (MMGPS_ReportMeasurement*) &_pLogMessage->iLogMsgData;

                        printf("  MMGPS FCOUNT: %d", _pMMGPS_Measurement->FCOUNT );

                        printf("\nSV_ID\tHEALTH\tSNR\tCNO\tLATENCY\tMEAS_STATUS");
                        printf("\n-----\t------\t---\t---\t-------\t-----------");

                        // Loop through the SV's
                        for ( int _iSV_Index = 0; _iSV_Index < DIAG_MMGPS_NUM_MEASUREMENT_REPORTS; _iSV_Index++ )
                        {
                           // Filter for only SV 1
                           if ( _pMMGPS_Measurement->aMeasurements[_iSV_Index].SV_ID >0)
                           {
                              printf("\n0x%X\t%d\t%6.2f\t%6.2f\t0x%X\t0x%X",
                                     _pMMGPS_Measurement->aMeasurements[_iSV_Index].SV_ID,
                                     _pMMGPS_Measurement->aMeasurements[_iSV_Index].HEALTH,
                                     (double) _pMMGPS_Measurement->aMeasurements[_iSV_Index].SNR * 0.1,
                                     (double) _pMMGPS_Measurement->aMeasurements[_iSV_Index].CNO * 0.1,
                                     _pMMGPS_Measurement->aMeasurements[_iSV_Index].LATENCY_MS,
                                     _pMMGPS_Measurement->aMeasurements[_iSV_Index].MEAS_STATUS
                                    );
                           }
                        }
                     }  // if ( _pLogMessage->log_item == _DIAG_LOG_AAGPS_MEASUREMENT )

                     // Check for the AGPS Measurement Log Message, version 3
                     if ( _pLogMessage->log_item == _DIAG_LOG_AAGPS_MEASUREMENT_V3 )
                     {
                        // Assign a structure to the log data, to facilitate interpratation
                        MMGPS_ReportMeasurement_V3* _pMMGPS_Measurement = (MMGPS_ReportMeasurement_V3*) &_pLogMessage->iLogMsgData;

                        printf("  MMGPS FCOUNT: %d", _pMMGPS_Measurement->FCOUNT );
                        printf("  Num SV's: %d", _pMMGPS_Measurement->NUMBER_SVS );

                        printf("\nSV_ID\tHEALTH\tSNR\tCNO\tLATENCY\tMEAS_STATUS");
                        printf("\n-----\t------\t---\t---\t-------\t-----------");

                        // Loop through the SV's
                        for ( int _iSV_Index = 0; _iSV_Index < _pMMGPS_Measurement->NUMBER_SVS; _iSV_Index++ )
                        {
                           // Filter for only SV 1
                           if ( _pMMGPS_Measurement->aMeasurements[_iSV_Index].SV_ID >0)
                           {
                              printf("\n0x%X\t%d\t%6.2f\t%6.2f\t0x%X\t0x%X",
                                     _pMMGPS_Measurement->aMeasurements[_iSV_Index].SV_ID,
                                     _pMMGPS_Measurement->aMeasurements[_iSV_Index].HEALTH,
                                     (double) _pMMGPS_Measurement->aMeasurements[_iSV_Index].SNR * 0.1,
                                     (double) _pMMGPS_Measurement->aMeasurements[_iSV_Index].CNO * 0.1,
                                     _pMMGPS_Measurement->aMeasurements[_iSV_Index].LATENCY_MS,
                                     _pMMGPS_Measurement->aMeasurements[_iSV_Index].MEAS_STATUS
                                    );
                           }
                        }
                     }  // if ( _pLogMessage->log_item == _DIAG_LOG_AAGPS_MEASUREMENT )
                  } // switch ( pLogBuffer[0] )
               }    // if (( iLogSize > 0) && (_pLog->iMessageLength > 0 ))
            }   // if ( QLIB_DIAG_GetNextPhoneLog(...
         }  // while ( !kbhit() )

         bOk = QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, false );
         printf("\nQLIB_DIAG_SetPhoneLoggingState(  ON): %s", bOk ? "PASS" : "FAIL" );

         // Stop trakcing
         bOk = QLIB_FTM_AGPS_IDLE_MODE( g_hResourceContext );

         // Flush the PC's keypress buffer
         getch();
         printf("\n\n");
      } // cKey == 7 or 8
      else if ( cKey == '9' )
      {
         // printf("\n9=AGPS C/N");
         unsigned short iChannel = 9739;

         // Setup WCDMA mode and send the GPS commands
         printf("\n\nSetting up WCDMA mode and GPS Standalone...\n\n");

         // RF Mode, WCDMA - IMT
         bOk = QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_WCDMA_IMT );
         printf("\nQLIB_FTM_SET_MODE(WCDMA): %s", bOk ? "PASS" : "FAIL" );

         // Set the frequency channel
         bOk = QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel);
         printf("\nQLIB_FTM_SET_CHAN(%d): %s", iChannel, bOk ? "PASS" : "FAIL" );

         // Return value
         long _iCTON;
         double _dCTON = 0.0;

         // Send the command
         bOk = QLIB_FTM_AGPS_GET_CTON( g_hResourceContext, &_iCTON );

         printf("\nQLIB_FTM_AGPS_GET_CTON(): %s", bOk ? "PASS" : "FAIL" );

         if ( bOk )
         {
            // Convert from 1/10dB to dB
            _dCTON = ( _iCTON / 10.0 );

            printf("\nQLIB_FTM_AGPS_GET_CTON() = %6.2fdB", _dCTON );
         }  // if ( bOk )
      } // if ( cKey == '9' )
      else if ( cKey == 'A' )
      {
         //
         // This defintion must be packed
         //
#pragma pack( push, PACK_STATE)
#define PACKED
#pragma pack(1)

         // Define structure to parse the packet
         typedef PACKED struct
         {
            unsigned char aHeader[ 10];
            unsigned long iCtoN;
            unsigned short iErrorCode;
         } AGPS_CtoN_Response;

#pragma pack( pop, PACK_STATE)
         //
         //
         //

         // printf("\n9=AGPS C/N (not all targets have this)");
         static int const c_iPacketSize = 16;
         static unsigned char aiRequestBuffer  [ c_iPacketSize ] = { 0x4B, 0x0B, 0x0A, 0x00, 0x7A, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
         static unsigned char aiResponseBuffer [ c_iPacketSize ] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
         static short iRequestSize  = c_iPacketSize;    // Size of request packet
         static short iResponseSize = c_iPacketSize;    // This will get updated by SendSync

         // Return value
         double _dCTON = 0.0;

         // Send the command
         if ( QLIB_SendSync( g_hResourceContext, iRequestSize, aiRequestBuffer, &iResponseSize, aiResponseBuffer,2000 ) )
         {
            printf("\nQLIB_SendSync( AGPS CtoN ): PASS\n");

            // Cast the response buffer to the structure type
            AGPS_CtoN_Response* _pAGPS_Response = (AGPS_CtoN_Response*) aiResponseBuffer;

            // Check error code
            if ( _pAGPS_Response->iErrorCode == DIAG_FTM_STATUS_SUCCESS )
            {
               // Print the result
               printf("QLIB_FTM_AGPS_GET_CTON( iCTON = %d (0x%X) )", _pAGPS_Response->iCtoN, _pAGPS_Response->iCtoN );

               // Convert from 1/10dB to dB
               _dCTON = ( _pAGPS_Response->iCtoN / 10.0 );

               printf("\nQLIB_FTM_AGPS_GET_CTON() = %6.2f", _dCTON );
            }
            else
            {
               // Print the result
               printf("QLIB_FTM_AGPS_GET_CTON() -- failed with status value %d", _pAGPS_Response->iErrorCode);
            }
         }
         else
         {
            printf("\nSend Sync (version info): FAIL\n");
         }
      } // if ( cKey == 'A' )
      else if ( cKey == 'B' )
      {
         // printf("\nB=AGPS IQ Capture");

         // Parameters for IQ capture
         unsigned long iCenterFreq_Hz = 0;      // No center freq because auto is used
         unsigned long iSampleFreq_Hz = 1;      // 1=I/Q Capture Sampling frequency 1MHz
         unsigned short iStartDelay = 4500;     // ms to detla
         unsigned char iAutoCenter = TRUE;      // Center frequency is automatically set
         unsigned short _iMaxSampleSize = 4096; // # of IQ samples to capture, this is
         // hardcoded to 4096 on the embedded side

         printf("\n\nCapturing AGPS IQ, will take about 3 seconds...");

         // Send the command
         bOk = QLIB_FTM_AGPS_IQ_CAPTURE
               (
                  g_hResourceContext, iCenterFreq_Hz, iSampleFreq_Hz,
                  iStartDelay, iAutoCenter, &_iMaxSampleSize
               );

         printf("\nQLIB_FTM_AGPS_IQ_CAPTURE(): %s", bOk ? "PASS" : "FAIL" );

         // Create a buffer to hold response data
         const unsigned short c_iSamplesPerRecord = 256;        // 128 I and Q samples per record, total of 256 entries
         short _aiIQ_Samples[ c_iSamplesPerRecord ];            // buffer to store samples
         unsigned short _iNextRecordNum = 0;

         // # of records
         const unsigned short c_iMaxRecordToTransfer = _iMaxSampleSize  / c_iSamplesPerRecord;

         printf("\nTransferring %d records to file %s...", c_iMaxRecordToTransfer, _sOutputFile );

         // Select text mode for the IO library
         _fmode = _O_TEXT;

         // Setup file pointer
         FILE* _hIQ_File = NULL;

         // Now store this data into a file
         if (_hIQ_File = fopen( _sOutputFile,"w"))
         {
            // Print file header
            fprintf( _hIQ_File, "Record#,Sample#,I,Q");

            // Iterate through the records
            for ( unsigned short _iRecordIndex = 0; _iRecordIndex < c_iMaxRecordToTransfer; _iRecordIndex++ )
            {
               // This value is going to be updated by the function
               _iNextRecordNum = _iRecordIndex;

               // Get the next buffer
               bOk = QLIB_FTM_AGPS_GET_IQ_DATA( g_hResourceContext, _aiIQ_Samples, &_iNextRecordNum );

               // Print status if a fail
               if ( !bOk)
               {
                  printf("\nQLIB_FTM_AGPS_GET_IQ_DATA(): %s", bOk ? "PASS" : "FAIL" );
               }    // if ( !bOk)

               // Print message if record numbers do not match
               if ( ( bOk )
                     &&    ( _iNextRecordNum != _iRecordIndex )
                  )
               {
                  bOk = false;
                  printf("\nReported record number (%d) does not match requested record number (%d)", _iNextRecordNum, _iRecordIndex);
               }    // if ( _iNextRecordNum != _iRecordIndex )

               // Exit loop
               if ( !bOk)
                  break;

               // Storage for one sample at a time
               short _iI_Sample = 0;
               short _iQ_Sample = 0;

               // Loop through the IQ samples
               for ( int _iSampleIndex = 0; _iSampleIndex < c_iSamplesPerRecord; _iSampleIndex+=2 )
               {
                  _iI_Sample = _aiIQ_Samples[_iSampleIndex];
                  _iQ_Sample = _aiIQ_Samples[_iSampleIndex+1];

                  // Print the I and Q components to a comma delimited file
                  // Write the data to a file
                  fprintf( _hIQ_File, "\n%d,%d,%d,%d", _iRecordIndex, _iSampleIndex /2, _iI_Sample, _iQ_Sample );
               } // for ( int _iSampleIndex = 0; _iSampleIndex < ; _iSampleIndex++ )
            }   // for ( int _iRecordIndex = 0; _iRecordIndex < c_iMaxRecords; _iRecordIndex )

            if ( bOk )
            {
               printf("\nIQ Capture completed, please view results in file %s", _sOutputFile );
            }
         }
         else
         {
            printf("\nFile could not be opened.");
         }  // fopen()

         // Close the file
         if (_hIQ_File != NULL )
            fclose( _hIQ_File );
      } // if ( cKey == 'A' )
   } // if (bOk )

   // Print status
   printf("\nCommand Status: %s", bOk ? "PASS" : "FAIL\n" );

   return bOk;
}

/////////////////////////////////////////////////////////
void Test_CGPS_Write_IQ_Measured( char* sFileName )
{
   // Select text mode for the IO library
   _fmode = _O_TEXT;

   // Setup file pointer
   FILE* _hFile = NULL;

   // Declare memory for a copy of the CGPS status structure
   CGPS_Status_Struct _oCGPS_Status;

   // Get a copy of the CGPS status
   QLIB_CGPS_GetStatus( g_hResourceContext, &_oCGPS_Status );

   /*
   Declare memory to store the samples--this could be done based on the reported sample size instead
   of the maximum
   */
   char _aI_Samples[ CGPS_MAX_IQ_SAMPLES_PER_CAPTURE ];
   char _aQ_Samples[ CGPS_MAX_IQ_SAMPLES_PER_CAPTURE ];
   unsigned long _iNumSamples = 0;
   unsigned long _iSampleIndex = 0;

   // Verify a capture has occured and that there is valid data
   if ( ( _oCGPS_Status.abHasEventOccurred[ CGPS_IQ_Test_Complete ] )       // Event has occured
         &&    ( _oCGPS_Status.oIQ_FFT_DataConfig.iNum_IQ_Measured_Samples > 0)    // There is at least one sample
      )
   {
      // Get the data from QMSL
      QLIB_CGPS_GetIQ_Data( g_hResourceContext, _aI_Samples, _aQ_Samples, &_iNumSamples );

      // Check number of samples
      if ( _iNumSamples )
      {
         // Now store this data into a file
         if (_hFile = fopen( sFileName,"w"))
         {
            // Print header
            fprintf( _hFile, "Sample#,I,Q");

            // Print data
            for ( _iSampleIndex = 0; _iSampleIndex < _iNumSamples; _iSampleIndex++ )
            {
               fprintf( _hFile,"\n%d,%d, %d", _iSampleIndex, _aI_Samples, _aQ_Samples );
            }

            // Close file
            fclose( _hFile );
            printf("\nOperation completed, please view results in file %s", sFileName );
         }
         else
         {
            printf("\nFile %s could not be opened.", sFileName);
         }  // fopen()
      }
      else
      {
      }  // if ( _iNumSamples )
   }
   else
      printf("\nThere is not valid IQ data" );
}

/////////////////////////////////////////////////////////
void Test_CGPS_Write_FFT_Measured( char* sFFT_File )
{
}

/////////////////////////////////////////////////////////
void Test_CGPS_Write_FFT_Calculated( char* sFFT_File )
{
   // Declare memroy for a copy of the CGPS status structure
   CGPS_Status_Struct _oCGPS_Status;

   // Get a copy of the CGPS status
   QLIB_CGPS_GetStatus( g_hResourceContext, &_oCGPS_Status );

   // Verify a capture has occured and that there is valid data
   if ( ( _oCGPS_Status.abHasEventOccurred[ CGPS_IQ_Test_Complete ] )       // Event has occured
         &&    ( _oCGPS_Status.oIQ_FFT_DataConfig.iNum_IQ_Measured_Samples > 0)    // There is at least one sample
      )
   {
      // Now perform the FFT calculation on the IQ data
      QLIB_CGPS_CalculateFFT_Data( g_hResourceContext );

      // Define an array to store data
      double* _paFFT_Amplitude = new double[ CGPS_MAX_CALCULATED_FFT_SAMPLES_PER_CAPTURE ];
      double* _paFFT_Frequency = new double[ CGPS_MAX_CALCULATED_FFT_SAMPLES_PER_CAPTURE ];
      unsigned long _iFFT_SampleSize = 0;

      // Get FFT Data
      QLIB_CGPS_GetFFT_CalculatedData(
         g_hResourceContext,
         _paFFT_Amplitude,
         _paFFT_Frequency,
         &_iFFT_SampleSize
      );

      // Select text mode for the IO library
      _fmode = _O_TEXT;

      // Setup file pointer
      FILE* _hFile = NULL;

      // Variables to store the max level
      double _dMax_dB = -9999.9;
      double _dMax_Hz = 0.0;
      unsigned long _iMaxIndex = 0;

      // Check number of samples
      if ( _iFFT_SampleSize )
      {
         // Now store this data into a file
         if (_hFile = fopen( sFFT_File,"w"))
         {
            // Print header
            fprintf( _hFile, "%%Sample#,Freq (Hz),dB");

            // Print data
            for ( unsigned long _iSampleIndex = 0; _iSampleIndex < _iFFT_SampleSize; _iSampleIndex++ )
            {
               fprintf( _hFile,
                        "\n%d,%f, %f",
                        _iSampleIndex,
                        _paFFT_Frequency[ _iSampleIndex ],
                        _paFFT_Amplitude[ _iSampleIndex ]
                      );

               if ( _paFFT_Amplitude[ _iSampleIndex ] > _dMax_dB )
               {
                  _dMax_dB = _paFFT_Amplitude[ _iSampleIndex ];
                  _dMax_Hz = _paFFT_Frequency[ _iSampleIndex ];
                  _iMaxIndex = _iSampleIndex;
               }    // if ( _paFFT_Amplitude[ _iSampleIndex ] > _dMax_dB )
            }

            // Close file
            fclose( _hFile );
            printf("\nOperation completed, please view results in file %s", sFFT_File );

            printf("\n\nPeak of %6.2fdb found at index %d, frequency %fHz",
                   _dMax_dB,
                   _iMaxIndex,
                   _dMax_Hz
                  );
         }
         else
         {
            printf("\nFile %s could not be opened.", sFFT_File  );
         }  // fopen()
      }
      else
      {
      }  // if ( _iNumSamples )

      // Delete the data
      delete _paFFT_Amplitude;
      delete _paFFT_Frequency;
   }
   else
      printf("\nThere is not valid IQ data yet, please perform an IQ capture" );
}

unsigned char Test_CGPS( void )
{
   unsigned char bOk=true;
   unsigned char cKey;
   unsigned char iStatus;

   // This should not be set to 0 because prescribed dwells will not work
   unsigned char iSV = 1;

   // File names
   char sWB_IQ_File[] = "C:\\CGPS_IQ_WB_COLLECT.TXT";
   char sNB_IQ_File[] = "C:\\CGPS_IQ_NB_COLLECT.TXT";
   char sFFT_Measured_File[] = "C:\\CGPS_FFT_WB_COLLECT.TXT";
   char sFFT_Calculated_File[] = "C:\\CGPS_FFT_WB.TXT";

   // For development, logging will be activated automatically, remove this for release
   // when they are removed, also remove the corresponding function calls to stop the log
   // at the end of the function
   unsigned short _iLogFlags = LOG_FN | LOG_RET |  LOG_ERR | LOG_C_HIGH_LEVEL_START  | LOG_C_HIGH_LEVEL_STOP | LOG_ASYNC;

   // Include the following line in order to see IO information.  There tends to be a lot of log messages with
   // CGPS, so the IO information can be too much.
   _iLogFlags |= LOG_IO;

   QLIB_SetLogFlags( g_hResourceContext, _iLogFlags );
   QLIB_StartLogging( g_hResourceContext,  "C:\\Factory_log.txt");
   QLIB_StartDLF_Logging( g_hResourceContext,  "C:\\Factory_log.dlf", false);

   if ( bOk )
   {
      do
      {
         printf("\n\n----------------\ngpsOne Gen 7 Engine Test\n----------------\n");
         printf("\n1 = Enter RF Verification Mode");
         printf("\n2 = Exit RF Verification Mode");
         printf("\n3 = Start Track SV %d NOCLEAR", iSV );
         printf("\n4 = Stop Track SV CLEAR ALL");
         printf("\n5 = Perform multiple prescribed dwell for SV %d", iSV );
         printf("\n6 = Wideband IQ and FFT capture + 200kHz CW offset");
         printf("\n7 = Narrowband capture IQ and FFT capture + 100kHz CW offset");
         printf("\n9 = Write FFT Measured Data to file (%s)", sFFT_Measured_File);
         printf("\nA = Write FFT Calculated Data to file (%s)", sFFT_Calculated_File);
         printf("\nB = AGC Test (based on BP Mean from CGPS RF Status log)");
         printf("\nC = C/N and doppler (based on CGPS RF Measurement log)");
         printf("\nD = Start Track SV %d CLEAR_NO_BITEDGE_DETECTION and BER report", iSV );
         printf("\nE = Execute MATLAB script for WB capture" );
         printf("\nF = Execute MATLAB script for NB capture" );
         printf("\nG = C/N with a CW at + 200kHz offset" );
         printf("\nH = Scan multiple settings: C/N with a CW at + 200kHz offset" );
         printf("\nI = Execute MATLAB script for WB capture using COM interface" );
         printf("\nK = Wideband capture and Mobile reported C/N" );

         printf("\nQ=Exit this menu");

         cKey = toupper( getch() );

         // Reset the OK flag
         bOk = true;

         // Setup condtions to determine what items will be executed

         // Start verification upon direct request, dwell, iq capture or track
         unsigned char _bStartVerificationMode = ( cKey=='1') ;

         // Stop verification mode upon direct request
         unsigned char _bStopVerificationMode = ( cKey=='2') ;

         // Indicates a track start
         unsigned char _bStartTrackSV = (cKey == '3');

         // Indicates a track stop
         unsigned char _bStopTrackSV = (cKey == '4');

         // Indicates a dwell
         unsigned char _bStartDwell = cKey == '5';

         // Indicates a Wideband IQ capture
         unsigned char _bStartWB_IQ = cKey == '6';

         // Indicates a Narrowband IQ capture
         unsigned char _bStartNB_IQ = cKey == '7';

         // Write measured WB IQ data to file
         unsigned char _bWrite_WB_IQ_MeasuredData = cKey == '8';

         // Write measured NB IQ data to file
         unsigned char _bWrite_NB_IQ_MeasuredData = cKey == 'H';

         // Write measured FFT data to file
         unsigned char _bWriteFFT_MeasuredData = cKey == '9';

         // Write calculated IQ data to file
         unsigned char _bWriteFFT_CalculatedData = cKey == 'A';

         // Indicates an AGC test
         unsigned char _bStartAGC_Test = cKey == 'B';

         // Indicates an C/N and dopper test
         unsigned char _bStartC_N_Test = cKey == 'C';

         // Indicates an BER test
         unsigned char _bStartBER_Test = cKey == 'D';

         // MATLAB WB Analysis
         unsigned char _bMATLAB_WB_Analysis (cKey == 'E');
         unsigned char _bMATLAB_WB_Analysis_DLL = (cKey == 'I');

         // MATLAB NB Analysis
         unsigned char _bMATLAB_NB_Analysis (cKey == 'F');

         // Indicates a C/N with CW
         unsigned char _bCW_CtoN = (cKey == 'G');

         // Indicates a C/N with CW
         unsigned char _bCW_CtoN_Scan = (cKey == 'H');

         // Indicates a C/N with CW
         unsigned char _bCW_CtoN_MobileReported = (cKey == 'K');

         if ( _bStartVerificationMode )
         {
            // Set timeout the timeout short enough to catch most events, except for IQ Capture
            bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_CGPS_Event, 3000);
            printf("\nQLIB_ConfigureTimeOut(): %s", bOk ? "PASS" : "FAIL" );

            bOk = QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_ENTER_MODE, &iStatus );
            printf("\nQLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );
         }  // _bStartVerificationMode

         if ( bOk && _bStartTrackSV )
         {
            unsigned char iCommand = GPS_SV_TRACK_START_NO_CLEAR;
            bOk = QLIB_DIAG_GPS_SV_TRACK( g_hResourceContext, iSV, iCommand, &iStatus );

            printf("\nQLIB_DIAG_GPS_SV_TRACK(iSV = %d): %s, status %d",
                   iSV,
                   bOk ? "PASS" : "FAIL",
                   iStatus );
         }// _bStartTrackSV

         if ( bOk && _bStopTrackSV )
         {
            unsigned char iCommand = GPS_SV_TRACK_STOP_CLEAR_ALL;
            bOk = QLIB_DIAG_GPS_SV_TRACK( g_hResourceContext, iSV, iCommand, &iStatus );

            printf("\nQLIB_DIAG_GPS_SV_TRACK(iSV = %d): %s, status %d",
                   0,   // iSV = 0
                   bOk ? "PASS" : "FAIL",
                   iStatus );
         }// _bStartTrackSV

         if ( bOk && _bStartDwell)
         {
            bool _bEnableEvents = true;

            /*
            This will start the log message associated with the BP Mean.
            While this message updates, QMSL will update a data structure with
            the statistics of BP Mean from the RF Status log (CGPS_RF_STATUS_REPORT_LOG)
            */
            bOk = QLIB_DIAG_GPS_StartCGPS_AsyncMessages( g_hResourceContext, _bEnableEvents );

            // Clear status for dwell completions and dwell status
            QLIB_CGPS_ClearStatus_Specific( g_hResourceContext, CGPS_Dwell_Complete );
            QLIB_CGPS_ClearStatus_Specific( g_hResourceContext, CGPS_Dwell_Status );

            // Set timeout the timeout log enough to allow for all data to be transported back to the PC
            // set timeout to 1 second for each 1k samples + 5 seconds
            bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_CGPS_Event, 10000 );

            printf("\nQLIB_ConfigureTimeOut(): %s", bOk ? "PASS" : "FAIL" );

            // Parameters taken from development PERL file, StartPrescDwell.pl
            unsigned short iFlags = GPS_PRESCRIBED_DWELL_SEND_RESULTS | GPS_PRESCRIBED_DWELL_CHIPX2| GPS_PRESCRIBED_DWELL_NO_INCOHER_SUMS | GPS_PRESCRIBED_DWELL_NO_COHER_SUMS | GPS_PRESCRIBED_DWELL_START_NOW | GPS_PRESCRIBED_DWELL_USE_RF | GPS_PRESCRIBED_DWELL_NO_INIT;
            unsigned long iTOA = 254;   // This value is not used when GPS_PRESCRIBED_DWELL_START_NOW is specified
            unsigned char iSearchMode = GPS_PRESCRIBED_DWELL_SEARCH_MODE_1;
            unsigned short iPOST_DET_N = 600;
            unsigned long iTASK_MASK1 = 0xFFFFFFFF;
            unsigned char iTASK_MASK2 = 0x0;
            unsigned long iDOPP_FREQ = 0;       // ($dopp*5.25*65.536);# (1000/65536) Hz/bit. Signed. Range is [-1.023MHz:+1.023MHz]. Negative doppler on 4100 corresponds to a positive value in this field.
            unsigned long iDOPP_FREQ_RATE = 0;  // (1000/65536) Hz/sec/bit. Signed.
            unsigned long iSUB_MS = 0;
            unsigned short iCNO_Threshold = 0x0;
            unsigned short iJammerThreshold = 0x0;

            // Loop until user presses a key
            unsigned long _iLoopCount = 0;
            while ( !kbhit() )
            {
               // Increment loop count
               _iLoopCount++;

               printf("\n\nSending prescribed dwell command #%d and waiting for response event...", _iLoopCount );

               bOk = QLIB_DIAG_GPS_PRESCRIBED_DWELL_WithWait( g_hResourceContext,
                                                              iFlags,
                                                              iTOA,
                                                              iSV,
                                                              iSearchMode,
                                                              iPOST_DET_N,
                                                              iTASK_MASK1,
                                                              iTASK_MASK2,
                                                              iDOPP_FREQ,
                                                              iDOPP_FREQ_RATE,
                                                              iSUB_MS,
                                                              iCNO_Threshold,
                                                              iJammerThreshold,
                                                              &iStatus );

               printf("\nQLIB_DIAG_GPS_PRESCRIBED_DWELL(): %s, status %d",
                      bOk ? "PASS" : "FAIL",
                      iStatus );

               // Create a status structure
               CGPS_Status_Struct _oCGPS_Status;

               // Get the status from QMSL
               QLIB_CGPS_GetStatus( g_hResourceContext, (void*) &_oCGPS_Status );

               // Print total attempts and number of successful
               printf("\nOf %d attempts, there have been %d reports, with %d valid peaks and %6.2fdB average C/No.",
                      _iLoopCount,
                      _oCGPS_Status.oDwellStatus.iNumEventReports,
                      _oCGPS_Status.oDwellStatus.iNumValidPeaks,
                      _oCGPS_Status.oDwellStatus.dAverageCNO
                     );

               // NOTE: See the comments for CGPS_PrescribedDwell_StatusInfo in QLibDefines.h
               // There are several metrics that are not being calcluated right now, including the number of successful dwells
               // and the C/No statistics.  Those will be provided in QMSL at a later date.
            }   // while ( !kbhit() )
         } // _bStartDwell

         if ( bOk && _bStartWB_IQ)
         {
            gps_start_iq_test_struct_type _oIQ_Test;
            _oIQ_Test.iRequests = GPS_START_IQ_TEST_REQ_NO_FFT | GPS_START_IQ_TEST_REQ_IQ_REPORT_SAMPLES | GPS_START_IQ_TEST_REQ_NO_NARROWBAND_PEAK_PROFILE | GPS_START_IQ_TEST_REQ_RTC_GPS | GPS_START_IQ_TEST_REQ_START_IMMED;
            _oIQ_Test.iFCount = 0x00;
            _oIQ_Test.iTestSourceInfo = 0x00; //GPS_START_IQ_TEST_TS_INPUT_TS | GPS_START_IQ_TEST_TS_SV_ID_0 | GPS_START_IQ_TEST_TS_DATA_ON | GPS_START_IQ_TEST_TS_NOISE_OFF;
            _oIQ_Test.iTestSourceSignalStrength = 0x00;
            _oIQ_Test.iTestSourceDoppler = 0x00;
            _oIQ_Test.iKiloSamples = 256;
            _oIQ_Test.iFFT_Integration = 10;    //
            _oIQ_Test.iSM_Point = GPS_START_IQ_TEST_SM_SINGLE_POINT_OTHER;
            _oIQ_Test.iCollectPoint = GPS_START_IQ_TEST_COLLECT_OTHER_SAMPLE_SERVER_2MHZ; //GPS_START_IQ_TEST_COLLECT_OTHER_SAMPLE_SERVER_1KHZ
            _oIQ_Test.iScale = 0;
            _oIQ_Test.iStartDelay = 5000;
            _oIQ_Test.iWidebandCenterFreq = 0;
            _oIQ_Test.iNarrowbandCenterFreq =0x80000000;
            _oIQ_Test.iNumWidebandPeaks = 0;
            _oIQ_Test.iNumWidebandAdjSamples = 0;
            _oIQ_Test.iNumNarrowbandPeaks = 0;
            _oIQ_Test.iNumNarrowbandAdjSamples = 0;

            // Set timeout the timeout log enough to allow for all data to be transported back to the PC
            bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_CGPS_Event, 25000 );
            printf("\nQLIB_ConfigureTimeOut(): %s", bOk ? "PASS" : "FAIL" );

            printf("\nSending WB IQ Capture command and collecting results...");
            bOk = QLIB_DIAG_GPS_START_IQ_TEST_CollectResults(
                     g_hResourceContext,
                     &_oIQ_Test,
                     sWB_IQ_File,
                     sFFT_Measured_File,
                     &iStatus );
            printf("\nQLIB_DIAG_GPS_START_IQ_TEST(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );

            if ( bOk)
            {
               printf("\nMATLAB IQ input file stored in %s", sWB_IQ_File );
            }   // if ( bOk)
         } // _bStartWB_IQ

         if ( bOk && _bStartNB_IQ)
         {
            gps_start_iq_test_struct_type _oIQ_Test;
            _oIQ_Test.iRequests = GPS_START_IQ_TEST_REQ_NO_FFT | GPS_START_IQ_TEST_REQ_IQ_REPORT_SAMPLES | GPS_START_IQ_TEST_REQ_NO_NARROWBAND_PEAK_PROFILE | GPS_START_IQ_TEST_REQ_RTC_GPS | GPS_START_IQ_TEST_REQ_START_IMMED;
            _oIQ_Test.iFCount = 0x00;
            _oIQ_Test.iTestSourceInfo = 0x00; //GPS_START_IQ_TEST_TS_INPUT_TS | GPS_START_IQ_TEST_TS_SV_ID_0 | GPS_START_IQ_TEST_TS_DATA_ON | GPS_START_IQ_TEST_TS_NOISE_OFF;
            _oIQ_Test.iTestSourceSignalStrength = 0x00;
            _oIQ_Test.iTestSourceDoppler = 0x00;
            _oIQ_Test.iKiloSamples = 60;    // Set to 60 for a 60 second integration
            _oIQ_Test.iFFT_Integration = 1; // Must be 1 for NB
            _oIQ_Test.iSM_Point = GPS_START_IQ_TEST_SM_SINGLE_POINT_OTHER;
            _oIQ_Test.iCollectPoint = GPS_START_IQ_TEST_COLLECT_OTHER_SAMPLE_SERVER_1KHZ; //GPS_START_IQ_TEST_COLLECT_OTHER_SAMPLE_SERVER_1KHZ
            _oIQ_Test.iScale = 0;
            _oIQ_Test.iStartDelay = 5000;
            _oIQ_Test.iWidebandCenterFreq = 0;
            _oIQ_Test.iNarrowbandCenterFreq =0x80000000;
            _oIQ_Test.iNumWidebandPeaks = 0;
            _oIQ_Test.iNumWidebandAdjSamples = 0;
            _oIQ_Test.iNumNarrowbandPeaks = 0;
            _oIQ_Test.iNumNarrowbandAdjSamples = 0;

            // Set timeout the timeout log enough to allow for all data to be transported back to the PC
            // set timeout to 1 second for each 1k samples + 5 seconds
            bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_CGPS_Event, _oIQ_Test.iKiloSamples * 1000 + 5000 );

            printf("\nQLIB_ConfigureTimeOut(): %s", bOk ? "PASS" : "FAIL" );

            printf("\nSending NB IQ Capture command and collecting results...");
            bOk = QLIB_DIAG_GPS_START_IQ_TEST_CollectResults(
                     g_hResourceContext,
                     &_oIQ_Test,
                     sNB_IQ_File,
                     sFFT_Measured_File,
                     &iStatus );
            printf("\nQLIB_DIAG_GPS_START_IQ_TEST(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );

            if ( bOk)
            {
               printf("\nMATLAB IQ input file stored in %s", sNB_IQ_File );
            }   // if ( bOk)
         } // _bStartNB_IQ

         if ( _bStartAGC_Test )
         {
            bool _bEnableEvents = true;

            /*
            This will start the log message associated with the BP Mean.
            While this message updates, QMSL will update a data structure with
            the statistics of BP Mean from the RF Status log (CGPS_RF_STATUS_REPORT_LOG)
            */
            bOk = QLIB_DIAG_GPS_StartCGPS_AsyncMessages( g_hResourceContext, _bEnableEvents );

            printf("\nQLIB_DIAG_GPS_StartCGPS_AsyncMessages(): %s",
                   bOk ? "PASS" : "FAIL"
                  );

            // Clear the CGPS statistics for the RF Status structure
            bOk = QLIB_CGPS_ClearStatus_Specific( g_hResourceContext, CGPS_RF_Status_Recieved );

            printf("\nQLIB_CGPS_ClearStatus_Specific( CGPS_RF_Status_Recieved ): %s",
                   bOk ? "PASS" : "FAIL"
                  );

            char _cKeyAGC = '\0';
            printf("\n\nStarting AGC measurement, press 'R' for report, any other key to exit loop");
            printf("\n");

            // Now loop to update measurement report
            do
            {
               // Get a key press
               _cKeyAGC = toupper( getch() );

               if (  _cKeyAGC == 'R' )
               {
                  // Create a status structure
                  CGPS_Status_Struct _oCGPS_Status;

                  // Get the status from QMSL
                  QLIB_CGPS_GetStatus( g_hResourceContext, (void*) &_oCGPS_Status );

                  // Print the BP Mean stats
                  printf( "\n-----------------------------------------------------");
                  printf( "\nAGC BP Mean values processed from %d RF logs\n", _oCGPS_Status.oRF_Status_AGC.iNumReports );
                  printf( "Mean/Min/Max/STD I  %6.0f %6.0f %6.0f %6.3f\n",
                          _oCGPS_Status.oRF_Status_AGC.dAvg_BP_Mean_I ,
                          _oCGPS_Status.oRF_Status_AGC.dMin_BP_Mean_I,
                          _oCGPS_Status.oRF_Status_AGC.dMax_BP_Mean_I,
                          _oCGPS_Status.oRF_Status_AGC.dStdDev_BP_Mean_I );

                  printf( "Mean/Min/Max/STD Q  %6.0f %6.0f %6.0f %6.3f\n",
                          _oCGPS_Status.oRF_Status_AGC.dAvg_BP_Mean_Q ,
                          _oCGPS_Status.oRF_Status_AGC.dMin_BP_Mean_Q,
                          _oCGPS_Status.oRF_Status_AGC.dMax_BP_Mean_Q,
                          _oCGPS_Status.oRF_Status_AGC.dStdDev_BP_Mean_Q
                        );

                  // Print the BP Amplitude stats
                  printf( "\nAGC BP Amplitude values processed from %d RF logs\n", _oCGPS_Status.oRF_Status_AGC.iNumReports );
                  printf( "Mean/Min/Max/STD I  %6.0f %6.0f %6.0f %6.3f\n",
                          _oCGPS_Status.oRF_Status_AGC.dAvg_BP_Ampl_I ,
                          _oCGPS_Status.oRF_Status_AGC.dMin_BP_Ampl_I,
                          _oCGPS_Status.oRF_Status_AGC.dMax_BP_Ampl_I,
                          _oCGPS_Status.oRF_Status_AGC.dStdDev_BP_Ampl_I );

                  printf( "Mean/Min/Max/STD Q  %6.0f %6.0f %6.0f %6.3f\n",
                          _oCGPS_Status.oRF_Status_AGC.dAvg_BP_Ampl_Q ,
                          _oCGPS_Status.oRF_Status_AGC.dMin_BP_Ampl_Q,
                          _oCGPS_Status.oRF_Status_AGC.dMax_BP_Ampl_Q,
                          _oCGPS_Status.oRF_Status_AGC.dStdDev_BP_Ampl_Q
                        );
               }    // if (  _cKeyAGC == 'R' )
            }
            while ( _cKeyAGC == 'R' );
         }  // if ( _bStartAGC_Test )

         if ( _bStartC_N_Test )
         {
            bool _bEnableEvents = true;

            /*
            This will start the log message associated with the BP Mean.
            While this message updates, QMSL will update a data structure with
            the statistics of BP Mean from the RF Status log (CGPS_RF_STATUS_REPORT_LOG)
            */
            bOk = QLIB_DIAG_GPS_StartCGPS_AsyncMessages( g_hResourceContext, _bEnableEvents );

            printf("\nQLIB_DIAG_GPS_StartCGPS_AsyncMessages(): %s",
                   bOk ? "PASS" : "FAIL"
                  );

            // Clear the CGPS statistics for the GPS Measurement structure
            bOk = QLIB_CGPS_ClearStatus_Specific( g_hResourceContext, CGPS_GPS_MeasurementReceived );

            printf("\nQLIB_CGPS_ClearStatus_Specific( CGPS_RF_Status_Recieved ): %s",
                   bOk ? "PASS" : "FAIL"
                  );

            char _cKeyAGC = '\0';
            printf("\n\nStarting C/N measurement, press 'R' for report, any other key to exit loop");
            printf("\n");

            // Now loop to update measurement report
            do
            {
               // Get a key press
               _cKeyAGC = toupper( getch() );

               if (  _cKeyAGC == 'R' )
               {
                  // Create a status structure
                  CGPS_Status_Struct _oCGPS_Status;

                  unsigned char _bValidReportFound = false;

                  // Get the status from QMSL
                  QLIB_CGPS_GetStatus( g_hResourceContext, (void*) &_oCGPS_Status );

                  // Loop through all SV's
                  for ( int _iSV_ID = CGPS_MIN_SV_ID; _iSV_ID <= CGPS_MAX_SV_ID; _iSV_ID++ )
                  {
                     // Verify the number of reports for this SV ID is > 0
                     if ( _oCGPS_Status.oMeasurementReports[ _iSV_ID].iNumReports > 0)
                     {
                        _bValidReportFound = true;

                        // Print the stats
                        printf("\n------------------------" );
                        printf("\nSv: %2d Num of meas: %5d\n",
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].iSV_ID,
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].iNumReports
                              );
                        printf("\n   C/No Avg %5.2f dB/Hz  Sigma %5.2f  Max %5.2f  Min %5.2f",
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].dC_N_Average_dB,
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].dC_N_StdDev_dB,
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].dC_N_Max_dB,
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].dC_N_Min_dB
                              );
                        printf("\n   Speed: Avg %5.1f m/s  Sigma %5.1f  Max %5.1f  Min %5.1f",
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].dSpeed_Average_mps,
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].dSpeed_StdDev_mps,
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].dSpeed_Max_mps,
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].dSpeed_Min_mps
                              );
                        printf("\n   Max Speed Discontinuity: %5.3f m/s/s at %5.1f sec",
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].dMaxAcceleration,
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].dMaxAccelerationTime
                              );
                        printf("\n   Last Acceleration: %5.3f m/s/s",
                               _oCGPS_Status.oMeasurementReports[ _iSV_ID].dLastAcceleration
                              );

                        printf("\n" );

                        /*
                        It would be possible to check limits here
                        */
                     }  // _oCGPS_Status.oMeasurementReports[ _iSV_ID].iNumReports
                  } // for ( int _iSV_ID = CGPS_MIN_SV_ID; _iSV_ID <= CGPS_MAX_SV_ID; _iSV_ID++ )

                  // Print message if no reports found
                  if ( !_bValidReportFound )
                     printf("\nNo valid GPS Measurements reports found. Phone is not tracking an SV");
               }    // if (  _cKeyAGC == 'R' )
            }
            while ( _cKeyAGC == 'R' );
         }  // if ( _bStartC_N_Test )

         if ( _bWriteFFT_MeasuredData )
         {
            Test_CGPS_Write_FFT_Measured( sFFT_Measured_File );
         }  // _bWriteFFT_MeasuredData

         if ( _bWriteFFT_CalculatedData )
         {
            Test_CGPS_Write_FFT_Calculated( sFFT_Calculated_File );
         }

         if ( bOk && _bStopVerificationMode)
         {
            bOk = QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_EXIT_MODE, &iStatus );
            printf("\nQLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );
         } // _bStopVerificationMode

         if ( _bStartBER_Test )
         {
            bool _bEnableEvents = true;

            /*
            This will start the log message associated with the BP Mean.
            While this message updates, QMSL will update a data structure with
            the statistics of BP Mean from the RF Status log (CGPS_RF_STATUS_REPORT_LOG)
            */
            bOk = QLIB_DIAG_GPS_StartCGPS_AsyncMessages( g_hResourceContext, _bEnableEvents );

            printf("\nQLIB_DIAG_GPS_StartCGPS_AsyncMessages(): %s",
                   bOk ? "PASS" : "FAIL"
                  );

            // Start SV Tracking
            unsigned char iCommand = GPS_SV_TRACK_START_CLEAR_ALL_NO_BITEDGE_DETECTION;
            bOk = QLIB_DIAG_GPS_SV_TRACK( g_hResourceContext, iSV, iCommand, &iStatus );

            printf("\nQLIB_DIAG_GPS_SV_TRACK(iSV = %d): %s, status %d",
                   iSV,
                   bOk ? "PASS" : "FAIL",
                   iStatus );

            // Clear the DLF file because it needs to be ran through GDPARSE for an exact comparison between
            // QMSL and the PERL methods.  This is not necessary for customers of QMSL though.
            QLIB_StopDLF_Logging( g_hResourceContext );
            QLIB_StartDLF_Logging( g_hResourceContext,  "C:\\Factory_log.dlf", false);

            // Clear the CGPS statistics for the GPS Measurement structure
            bOk = QLIB_CGPS_ClearStatus_Specific( g_hResourceContext, CGPS_Demodulation_Soft_Decision );

            printf("\nQLIB_CGPS_ClearStatus_Specific( CGPS_Demodulation_Soft_Decision ): %s",
                   bOk ? "PASS" : "FAIL"
                  );

            char _cKeyAGC = '\0';
            printf("\n\nStarting BER  measurement, press 'R' for report, any other key to exit loop");
            printf("\n");

            // Now loop to update measurement report
            do
            {
               // Get a key press
               _cKeyAGC = toupper( getch() );

               // Create a status structure
               CGPS_Status_Struct _oCGPS_Status;

               unsigned char _bValidReportFound = false;

               // Get the status from QMSL
               QLIB_CGPS_GetStatus( g_hResourceContext, (void*) &_oCGPS_Status );

               // Print the stats
               printf("\n------------------------" );
               printf("\nLogs: %d ,Total Bits: %d, Bit Errors: %d, BER = %6.2f%%",
                      _oCGPS_Status.oBER_Status.iNumLogReports,
                      _oCGPS_Status.oBER_Status.iNumBits,
                      _oCGPS_Status.oBER_Status.iNumBitErrors,
                      _oCGPS_Status.oBER_Status.dBER * 100.0

                     );
               printf("\n" );
            }
            while ( _cKeyAGC == 'R' );

            // Stop the DLF, but don't start again because this file needs to be ran through the  GDPARSE.EXE
            // for comparison
            QLIB_StopDLF_Logging( g_hResourceContext );
         }  // if ( _bStartC_N_Test )

         if ( _bCW_CtoN )
         {
            // Input values
            unsigned short _iCaptureSize_kSamples = 32;
            unsigned short _iNumberOfIntegrations = 10;
            unsigned char _iAutoSetGPS_VerificationMode = true;

            // Output storage
            double _dSNR = 0.0;

            printf("\nMeasuring CGPS SNR...");
            // Call the function
            bOk = QLIB_DIAG_CGPS_SNR(
                     g_hResourceContext,
                     _iCaptureSize_kSamples,
                     _iAutoSetGPS_VerificationMode,
                     _iNumberOfIntegrations,
                     sWB_IQ_File,
                     &_dSNR
                  );

            // Print the result
            printf("\nQLIB_DIAG_CGPS_CSNR( _iCaptureSize_kSamples = %d, _iAutoSetGPS_VerificationMode=%d, _sIQ_FileName = '%s', _dSNR = %6.2fdB ): %s",
                   _iCaptureSize_kSamples,
                   _iAutoSetGPS_VerificationMode,
                   sWB_IQ_File,
                   _dSNR,
                   bOk ? "PASS" : "FAIL"
                  );

            printf("\n\n Choose option 'F = Execute MATLAB script for WB capture' to graph the results if MATLAB runtime is installed");
         }  // if ( _bCW_CtoN )

         if ( _bCW_CtoN_Scan )
         {
            // Note: must enter CGPS verificatino mode first, option #1

            // Stop the log files because there is going to be a lot of data
            QLIB_StopLogging( g_hResourceContext );
            QLIB_StopDLF_Logging( g_hResourceContext );

            // Header
            printf("\nKsamples\tIntegrations\tCount\tTime(ms)\tResult");
            printf("\n--------\t------------\t-----\t--------\t------");

            // Input values
            unsigned short _iCaptureSize_kSamples = 4;
            unsigned char _iAutoSetGPS_VerificationMode = true; // If set to true, then the CGPS mode will automatically be set
            unsigned short _iNumberOfIntegrations = 4;

            // Exit flag, in case user presseskey
            bool _bExit = false;

            // Timer handle
            unsigned long _hTimer = NULL;

            // Elapsed time
            unsigned long _iElapsedTime = 0;

            // # of samples per capture size/integration count combination
            const unsigned short c_iNumSamplesPerSetting = 10;

            const int c_iCaptureIndexListSize = 4;
            const unsigned short c_aCaptureSizeList[c_iCaptureIndexListSize] = {4,8,16,32};
            // Loop through capture size
            for ( int _iCaptureIndex = 0; _iCaptureIndex < c_iCaptureIndexListSize; _iCaptureIndex++ )
            {
               // Determine capture size
               _iCaptureSize_kSamples = c_aCaptureSizeList[_iCaptureIndex];

               // Loop through integration count
               for ( unsigned short _iNumberOfIntegrations = 2;  _iNumberOfIntegrations <= 10; _iNumberOfIntegrations+=2)
               {
                  // Loop through a certain number samples per setting combination
                  for ( unsigned short _iSampleCount = 1; _iSampleCount <= c_iNumSamplesPerSetting; _iSampleCount++ )
                  {
                     // Output storage
                     double _dSNR = 0.0;

                     // Start a timer
                     _hTimer = QLIB_StartTimer( g_hResourceContext );

                     // Call the function
                     bOk = QLIB_DIAG_CGPS_SNR(
                              g_hResourceContext,
                              _iCaptureSize_kSamples,
                              _iAutoSetGPS_VerificationMode,
                              _iNumberOfIntegrations,
                              sWB_IQ_File,
                              &_dSNR
                           );

                     // Get elapsed time
                     _iElapsedTime = QLIB_GetTimerElapsed( g_hResourceContext, _hTimer );

                     // Stop timer
                     QLIB_StopTimer( g_hResourceContext, _hTimer );

                     // Print the result
                     // printf("Ksamples\tIntegrations\tCount\tResult");
                     printf("\n%d\t%d\t%d\t%d\t%.2f\t%s",
                            _iCaptureSize_kSamples,
                            _iNumberOfIntegrations,
                            _iSampleCount,
                            _iElapsedTime,
                            _dSNR,

                            bOk ? "PASS" : "FAIL"
                           );

                     _bExit = kbhit()==1;

                     if (_bExit) break;
                  } // for ( _iSampleCount = 0; _iSampleCount < c_iNumSamplesPerSetting; _iSampleCount++ )
                  if (_bExit) break;
               }    // for ( unsigned short _iNumberOfIntegrations = 0;  _iNumberOfIntegrations < 10; _iNumberOfIntegrations++)
               if (_bExit) break;
            }   // for ( int _iCaptureIndex = 0; _iCaptureIndex < c_iCaptureIndexListSize; _iCaptureIndex++ )
            // if set to false then you must call QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait() first
         }  // if ( _bCW_CtoN_Scan )

         // MATLAB WB Analysis

         if ( _bCW_CtoN_MobileReported )
         {
            // Input values
            unsigned short _iCaptureSize_kSamples = 1;
            unsigned short _iNumberOfIntegrations = 10;
            unsigned char _iAutoSetGPS_VerificationMode = true;

            // Output storage
            double _dCtoN = 0.0;
            int    _iFreqOffset = 0;
            printf("\nGet Mobile Reported C/N...");
            // Call the function
            bOk = QLIB_DIAG_CGPS_GET_EMBEDDED_CALCULATED_CTON(
                     g_hResourceContext,
                     _iCaptureSize_kSamples,
                     _iAutoSetGPS_VerificationMode,
                     _iNumberOfIntegrations,
                     sWB_IQ_File,
                     &_dCtoN,
                     &_iFreqOffset
                  );

            // Print the result
            printf("\nQLIB_DIAG_CGPS_GET_EMBEDDED_CALCULATED_CTON( _iCaptureSize_kSamples = %d, _iAutoSetGPS_VerificationMode=%d, _sIQ_FileName = '%s', _dCtoN = %6.2fdB, _iFreqOffset=%d Hz ): %s",
                   _iCaptureSize_kSamples,
                   _iAutoSetGPS_VerificationMode,
                   sWB_IQ_File,
                   _dCtoN,
                   _iFreqOffset,
                   bOk ? "PASS" : "FAIL"
                  );

            printf("\n\n Choose option 'F = Execute MATLAB script for WB capture' to graph the results if MATLAB runtime is installed");
         }  // if ( _bCW_CtoN )

         if (   ( _bMATLAB_WB_Analysis )
                ||  ( _bMATLAB_WB_Analysis_DLL  )
            )
         {
            double dRF_NoiseFigure_dB = 6.2;
            double dSigFreq_kHz = 200.0;
            double dSigLevel_dBm = -120.0;
            char* sOutputFolder = "c:\\";
            unsigned char bAutoClose = false;   // false = leave windows open so users can see the interactive "figure"
            char* sStandardOutputFile = NULL; // "CGPS_MATLAB_WB_IQ_Results.txt";
            unsigned char bWaitForCompletion = true;    // wait until the MATLAB windows are completed before returning from the QMSL function
            unsigned char iPhoneState = 0; //default: OOS
            char* sStdFolder = "C:\\"; //standard output file for COM interface and Matlab .m file.
            printf("\n\nAttemting to launch MATLAB deployment project via QMSL....");
            printf("\nWarnings:");
            printf("\n  - The MATLAB runtime environment (MCRInstaller.exe) must be installed.");
            printf("\n  - It takes about 15 seconds for the MATLAB graphs to be shown");
            printf("\n  - All graph windows must be closed before control is returned to the ");
            printf("\n    demo program (if 'AutoClose' is not used).");
            printf("\n  - The working folder must include the .EXE file that is being ran. If " );
            printf("\n    debugging in MSVC then set the 'Working Folder' in the debug ");
            printf("\n    settings to ..\\..\\bin" );
            printf("\n  - First test MATLAB using the batch file CGPS_IqTest_IqFileProc-NB.bat");
            printf("\n    after a WB IQ capture has been done with the demo program.");
            printf("\n\nAfter MATLAB closes, the data files will be here: %s", sOutputFolder );
            printf("\nIf Matlab encounters any issue, the error reporting file will be here: %s\\McrErrLogs.txt", sStdFolder );

            //init return result to false.
            bOk = false;

            if ( _bMATLAB_WB_Analysis_DLL )
            {
               // This version calls via a DLL instead of command line app
               bOk = QLIB_CGPS_MATLAB_IQ_FILE_PROC_COM(
                        g_hResourceContext,
                        dRF_NoiseFigure_dB,
                        dSigFreq_kHz,
                        dSigLevel_dBm,
                        sWB_IQ_File,      // Defined at the top of the funciton
                        sOutputFolder,
                        bAutoClose,
                        iPhoneState,
                        sStdFolder
                     );

               printf("\nQLIB_CGPS_MATLAB_IQ_FILE_PROC_COM( dRF_NoiseFigure_dB = %6.2f, dSigFreq_kHz = %6.2f, dSigLevel_dBm = %6.2f, sIQ_FileName = '%s', sOutputFolder = '%s', bAutoClose = %d, iPhoneState = %d, sStdFolder = %s) : %s",
                      dRF_NoiseFigure_dB,
                      dSigFreq_kHz,
                      dSigLevel_dBm,
                      sWB_IQ_File == NULL ? "" : sWB_IQ_File,
                      sOutputFolder == NULL ? "" : sOutputFolder,
                      bAutoClose,
                      iPhoneState,
                      sStdFolder == NULL? "" : sStdFolder,
                      bOk ? "PASS" : "FAIL"
                     );
            }
            else
            {
               // This is the version that calls teh command line application
               bOk = QLIB_CGPS_MATLAB_IQ_FILE_PROC(
                        g_hResourceContext,
                        dRF_NoiseFigure_dB,
                        dSigFreq_kHz,
                        dSigLevel_dBm,
                        sWB_IQ_File,      // Defined at the top of the funciton
                        sOutputFolder,
                        bAutoClose,
                        sStandardOutputFile,
                        bWaitForCompletion,
                        iPhoneState,
                        sStdFolder
                     );

               printf("\nQLIB_CGPS_MATLAB_IQ_FILE_PROC( dRF_NoiseFigure_dB = %6.2f, dSigFreq_kHz = %6.2f, dSigLevel_dBm = %6.2f, sIQ_FileName = '%s', sOutputFolder = '%s', bAutoClose = %d, sStandardOutputFile = %s, iPhoneState = %d, sStdFolder = %s) : %s",
                      dRF_NoiseFigure_dB,
                      dSigFreq_kHz,
                      dSigLevel_dBm,
                      sWB_IQ_File == NULL ? "" : sWB_IQ_File,
                      sOutputFolder == NULL ? "" : sOutputFolder,
                      bAutoClose,
                      sStandardOutputFile,
                      iPhoneState,
                      sStdFolder == NULL? "" : sStdFolder,
                      bOk ? "PASS" : "FAIL"
                     );
            }
         }  // if ( _bMATLAB_WB_Analysis )

         // MATLAB NB Analysis
         if ( _bMATLAB_NB_Analysis )
         {
            // printf("\nG = Execute MATLAB script for NB capture" );

            double dRF_NoiseFigure_dB = 6.2;
            double dSigFreq_kHz = 100.0;
            double dSigLevel_dBm = -110.0;
            char* sOutputFolder = "c:\\";
            unsigned char bAutoClose = true;    // false = leave windows open so users can see the interactive "figure"
            char* sStandardOutputFile = NULL;
            unsigned char bWaitForCompletion = true;
            unsigned char iPhoneState = 0; //default: OOS
            char* sStdFolder = "C:\\"; //standard output file for COM interface and Matlab .m file.

            printf("\n\nAttemting to launch MATLAB deployment project via QMSL....");
            printf("\nWarnings:");
            printf("\n  - The MATLAB runtime environment (MCRInstaller.exe) must be installed.");
            printf("\n  - It takes about 15 seconds for the MATLAB graphs to be shown");
            printf("\n  - All graph windows must be closed before control is returned to the ");
            printf("\n    demo program (if 'AutoClose' is not used).");
            printf("\n  - The working folder must include the .EXE file that is being ran. If " );
            printf("\n    debugging in MSVC then set the 'Working Folder' in the debug ");
            printf("\n    settings to ..\\..\\bin" );
            printf("\n  - First test MATLAB using the batch file CGPS_IqTest_IqFileProc-NB.bat");
            printf("\n    after a NB IQ capture has been done with the demo program.");
            printf("\n\nAfter MATLAB closes, the data files will be here: %s", sOutputFolder );
            printf("\nIf Matlab encounters any issue, the error reporting file will be here: %s\\McrErrLogs.txt", sStdFolder );

            // printf("\nF = Execute MATLAB script for NB capture" );
            //init return result to false.
            bOk = false;

            bOk = QLIB_CGPS_MATLAB_IQ_FILE_PROC(
                     g_hResourceContext,
                     dRF_NoiseFigure_dB,
                     dSigFreq_kHz,
                     dSigLevel_dBm,
                     sNB_IQ_File,     // Defined at the top of the funciton
                     sOutputFolder,
                     bAutoClose,
                     sStandardOutputFile,
                     bWaitForCompletion,
                     iPhoneState,
                     sStdFolder
                  );

            printf("\nQLIB_CGPS_MATLAB_IQ_FILE_PROC( dRF_NoiseFigure_dB = %6.2f, dSigFreq_kHz = %6.2f, dSigLevel_dBm = %6.2f, sIQ_FileName = '%s', sOutputFolder = '%s', bAutoClose = %d, sStandardOutputFile = %s, iPhoneState = %d, sStdFolder = %s) : %s",
                   dRF_NoiseFigure_dB,
                   dSigFreq_kHz,
                   dSigLevel_dBm,
                   sNB_IQ_File == NULL ? "" : sNB_IQ_File,
                   sOutputFolder == NULL ? "" : sOutputFolder,
                   bAutoClose,
                   sStandardOutputFile,
                   iPhoneState,
                   sStdFolder == NULL? "" : sStdFolder,
                   bOk ? "PASS" : "FAIL"
                  );
         }  // if ( _bMATLAB_NB_Analysis )
      }
      while (cKey != 'Q' );
   } // if (bOk )

   // Disable logging
   QLIB_StopLogging( g_hResourceContext );
   QLIB_StopDLF_Logging( g_hResourceContext );

   // Print status
   printf("\nCommand Status: %s", bOk ? "PASS" : "FAIL\n" );

   return bOk;
}

unsigned char Test_CGPS_Gen8( void )
{
   unsigned char bOk=true;
   unsigned char cKey;
   unsigned char iStatus;

   // For development, logging will be activated automatically, remove this for release
   // when they are removed, also remove the corresponding function calls to stop the log
   // at the end of the function
   unsigned short _iLogFlags = LOG_FN | LOG_RET |  LOG_ERR | LOG_C_HIGH_LEVEL_START  | LOG_C_HIGH_LEVEL_STOP ;

   QLIB_SetLogFlags( g_hResourceContext, _iLogFlags );

   if ( bOk )
   {
      do
      {
         printf("\n\n----------------\ngpsOne Gen 8 Engine Test\n----------------\n");
         printf("\n1 = Enter RF Verification Mode (gpsOne Gen 8)");
         printf("\n2 = Exit RF Verification Mode (gpsOne Gen 8)");
         printf("\n3 = Mobile-calculated C/No and frequency offset for GPS (gpsOne Gen 8)");
         printf("\n4 = Perform prescribed dwells in a loop for GPS SV 1 (gpsOne Gen 8)" );
         printf("\n5 = Perform GPS+GLO Tracking (gpsOne Gen 8)" );
         printf("\n6 = Perform GNSS TCXO Factory Calibration (gpsOne Gen 8)");
         printf("\n7 = Perform Mobile-calculated C/No and frequency offset for all constellations");
         printf("\n8 = Perform Factory ADC IQ Test with LTE B13 Wideband Jammer check");
         printf("\nQ=Exit this menu");

         cKey = toupper( getch() );

         // Reset the OK flag
         bOk = true;

         // Start verification upon direct request
         unsigned char _bStartVerificationMode = ( cKey=='1') ;

         // Stop verification mode upon direct request
         unsigned char _bStopVerificationMode = ( cKey=='2') ;

         // Report mobile-calculated C/No and frequency offset for GPS constellation (CW signal needed)
         unsigned char _bCtoN_MobileCalculated_GPS = (cKey == '3');

         // Indicates a prescribed dwell for GPS Gen8
         unsigned char _bStartDwell = ( cKey == '4' );

         // Indicates a SV Track for GPS Gen8
         unsigned char _bStartTracking = ( cKey == '5' );

         // Indicates a TCXO Factory Cal for GPS Gen8
         unsigned char _bTCXOFactoryCal = (cKey == '6');

         // Indicates a mobile-calculated C/No for all constellations
         unsigned char _bCtoN_MobileCalculated_ALL = (cKey == '7');

         // Indicates a factory ADC IQ test
         unsigned char _bFactory_IQ = (cKey == '8');

         if ( _bStartVerificationMode )
         {
            // Set timeout short enough to catch most events, except for IQ Capture
            bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_CGPS_Event, 3000);

            // Enter Standalone RF verification mode
            bOk = QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_ENTER_MODE, &iStatus );
            printf("\nQLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );
         }  // _bStartVerificationMode

         if ( bOk && _bStopVerificationMode)
         {
            // Exit Standalone RF verification mode
            bOk = QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_EXIT_MODE, &iStatus );
            printf("\nQLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );
         }   // _bStopVerificationMode

         if ( bOk && _bCtoN_MobileCalculated_GPS)
         {
            unsigned char iCollectMode = COLLECT_MODE_WBIQ_BP1_2MHZ; //Selects the I/Q collect point for GPS primary chain
            unsigned short iCaptureSize_kSamples = 32;
            unsigned char iNumberOfIntegrations = 10;
            int iCount=1; //Loop counter

            double dCNoDBHz = 0.0; //default initialization value for C/No
            int    iFreqInHz = 0;  //default initialization value for Freq. Offset

            printf("\nEnter number of times to retrieve mobile C/No (1 to 100):");
            scanf("%d", &iCount);
            if(iCount>100) iCount = 100;
            if(iCount<1) iCount = 1;
            printf("\nEnter Capture Size (in ksamples) (1,2,4,...,32):");
            scanf("%d", &iCaptureSize_kSamples);
            if(iCaptureSize_kSamples>32) iCaptureSize_kSamples = 32;
            if(iCaptureSize_kSamples<1) iCaptureSize_kSamples = 1;
            printf("\nNumber of integrations (1 to 10): ");
            scanf("%d", &iNumberOfIntegrations );
            if(iNumberOfIntegrations>10) iNumberOfIntegrations = 10;
            if(iNumberOfIntegrations<1) iNumberOfIntegrations = 1;

            for(int i =0; i<iCount && bOk != 0 ; i++)
            {
               bOk = QLIB_DIAG_GEN8_GET_EMBEDDED_CALCULATED_CTON(g_hResourceContext,
                                                                 iCollectMode,
                                                                 iCaptureSize_kSamples,
                                                                 iNumberOfIntegrations,
                                                                 &dCNoDBHz,
                                                                 &iFreqInHz
                                                                );
               printf("\nGPS mobile C/No = %6.1fdB, Freq. Offset = %6.3fkHz", dCNoDBHz, (double)iFreqInHz/1000 );
            }
            printf("\nQLIB_DIAG_GEN8_GET_EMBEDDED_CALCULATED_CTON(): %s", bOk ? "PASS" : "FAIL" );
         }   // _bCtoN_MobileCalculated_GPS
#if 1
         if (bOk && _bCtoN_MobileCalculated_ALL)
         {
            unsigned char iCollectMode = COLLECT_MODE_WBIQ_BP1_2MHZ; //Selects the I/Q collect point for GPS primary chain
            unsigned short iCaptureSize_kSamples = 32;
            unsigned char iNumberOfIntegrations = 10;
            int iCount = 1; //Loop counter

            double dCNoDBHz = 0.0; //default initialization value for C/No
            int    iFreqInHz = 0;  //default initialization value for Freq. Offset

            printf("\nEnter number of times to retrieve mobile C/No (1 to 100):");
            scanf("%d", &iCount);
            if (iCount > 100)
               iCount = 100;
            if (iCount < 1)
               iCount = 1;
            printf("\nEnter Capture Size (in ksamples) (1,2,4,...,32):");
            scanf("%d", &iCaptureSize_kSamples);
            if (iCaptureSize_kSamples > 32)
               iCaptureSize_kSamples = 32;
            if (iCaptureSize_kSamples < 1)
               iCaptureSize_kSamples = 1;
            printf("\nNumber of integrations (1 to 10): ");
            scanf("%d", &iNumberOfIntegrations);
            if (iNumberOfIntegrations > 10)
               iNumberOfIntegrations = 10;
            if (iNumberOfIntegrations < 1)
               iNumberOfIntegrations = 1;

            for (int i = 0; i < iCount && bOk != 0; i++)
            {
               GNSS_WB_FFT_Stats_v2 oFFT_Results;
               bOk = QLIB_DIAG_GNSS_GET_EMBEDDED_CALCULATED_CTON_ALL_CONSTELLATIONS(g_hResourceContext, 
                                                                                    iCollectMode,
                                                                                    iCaptureSize_kSamples,
                                                                                    iNumberOfIntegrations,
                                                                                    0,1,true,
                                                                                    &oFFT_Results
                                                                );

               printf("\nGPS mobile C/No = %6.1fdB, Freq. Offset = %6.3fkHz", (double)oFFT_Results.iGPSCNoDBHz / 10.0, (double)oFFT_Results.iGPSFreq / 1000); 
               printf("\nGLO mobile C/No = %6.1fdB, Freq. Offset = %6.3fkHz", (double)oFFT_Results.iGLOCNoDBHz / 10.0, (double)oFFT_Results.iGLOFreq / 1000);
               printf("\nBDS mobile C/No = %6.1fdB, Freq. Offset = %6.3fkHz", (double)oFFT_Results.iBDSCNoDBHz / 10.0, (double)oFFT_Results.iBDSFreq / 1000);
               printf("\nGAL mobile C/No = %6.1fdB, Freq. Offset = %6.3fkHz", (double)oFFT_Results.iGALCNoDBHz / 10.0, (double)oFFT_Results.iGALFreq / 1000);
            }
            printf("\nQLIB_DIAG_GEN8_GET_EMBEDDED_CALCULATED_CTON(): %s", bOk ? "PASS" : "FAIL");
         }   // _bCtoN_MobileCalculated_ALL

         if (bOk && _bFactory_IQ)
         {
            GNSS_Factory_IQ_Test_params testParams;
            testParams.FLAGS                         =  0x61;
            testParams.COLLECT_MODE                  =  COLLECT_MODE_81_MHZ_ADC_IQ;
            testParams.GLO_R1_CHAN                   =  0x00;
            testParams.GLO_HW_CHAN                   =  0x00;
            testParams.SAMPLE_COUNT_1K               =  32;
            testParams.INTEGRATIONS                  =  10;
            testParams.ADC_IQ_SAT_THRESH             =  5000;
            testParams.ADC_IQ_WB_JAMMER_START_REQ_HZ =  155500000;
            testParams.ADC_IQ_WB_JAMMER_STOP_REQ_HZ  =  157300000;
            testParams.ADC_IQ_REF_TONE_PWR_DBM       = -120;

            QLIB_CGPS_ClearStatus(g_hResourceContext);

         char iStatus2;

            bOk = QLIB_DIAG_GNSS_START_FACTORY_IQ_TEST_CollectResults(g_hResourceContext, (void *)&testParams, "factoryIQ.bin", "factoryIQ.fft", iStatus2); 

            if (bOk && (iStatus2 == DIAG_FTM_STATUS_SUCCESS)) 
            {
               // Create a status structure
               CGPS_Status_Struct _oCGPS_Status;

               QLIB_CGPS_GetStatus(g_hResourceContext, (void *)&_oCGPS_Status);

               printf("\nGPS mobile C/No = %6.1fdB, Freq. Offset = %6.3fkHz", (double)_oCGPS_Status.oFactory_IQ_Test_results.GPS_CNO / 10.0, (double)_oCGPS_Status.oFactory_IQ_Test_results.GPS_FREQ / 1000);
               printf("\nGLO mobile C/No = %6.1fdB, Freq. Offset = %6.3fkHz", (double)_oCGPS_Status.oFactory_IQ_Test_results.GLO_CNO / 10.0, (double)_oCGPS_Status.oFactory_IQ_Test_results.GLO_FREQ / 1000);
               printf("\nBDS mobile C/No = %6.1fdB, Freq. Offset = %6.3fkHz", (double)_oCGPS_Status.oFactory_IQ_Test_results.BDS_CNO / 10.0, (double)_oCGPS_Status.oFactory_IQ_Test_results.BDS_FREQ / 1000);
               printf("\nGAL mobile C/No = %6.1fdB, Freq. Offset = %6.3fkHz", (double)_oCGPS_Status.oFactory_IQ_Test_results.GAL_CNO / 10.0, (double)_oCGPS_Status.oFactory_IQ_Test_results.GAL_FREQ / 1000); 
               printf("\n");
               printf("\nADC Mean I = %6.1fmV, ADC Mean Q = %6.1fmV", (double)_oCGPS_Status.oFactory_IQ_Test_results.ADC_MEAN_I / 10.0, (double)_oCGPS_Status.oFactory_IQ_Test_results.ADC_MEAN_Q / 10.0);
               printf("\nADC I Amplitude = %6.1fmV, ADC Q Amplitue = %6.1fmV", (double)_oCGPS_Status.oFactory_IQ_Test_results.ADC_AMP_I / 10.0, (double)_oCGPS_Status.oFactory_IQ_Test_results.ADC_AMP_Q / 10.0);
               switch (_oCGPS_Status.oFactory_IQ_Test_results.ADC_IQ_SAT_PER_STATUS) 
               {
                  case 0:
                     printf("\nADC IQ Saturation percentage is not valid");
                     break;
                  case 3:
                     printf("\n0 Samples in ADC IQ test - Saturation percangate cannot be reported");
                     break;
                  case 4:
                     printf("\nADC I Saturation percentage = %10.5f%%", (double)_oCGPS_Status.oFactory_IQ_Test_results.I_SAMPLE_SATURATION_PERCENTAGE / 1000.0);
                     printf("\nADC Q Saturation percentage = %10.5f%%", (double)_oCGPS_Status.oFactory_IQ_Test_results.Q_SAMPLE_SATURATION_PERCENTAGE / 1000.0); 
                     break;
                  default:
                     printf("\nInvalid ADC IQ Saturation Status");
               }
               printf("\nADC IQ Wideband Jammer Power Estimate = %6.1fdB", (double)_oCGPS_Status.oFactory_IQ_Test_results.ADC_WB_JAMMER_POWER_EST_DB);
            }
         else
         {
            printf("\nFactory IQ Test Failed");
         }
         } // _bFactory_IQ
#endif
         if (bOk && _bStartDwell) 
         {
            bool _bEnableEvents = true;

            CGPS_Gen8_PrescribedDwell_struct_type _oPrescDwellConfig; //structure to store the input parameters
            unsigned char _iStatus = 0;

            //This will start the log message collection
            bOk = QLIB_DIAG_GPS_StartCGPS_AsyncMessages( g_hResourceContext, _bEnableEvents );

            // Clear status for dwell completions and dwell status
            QLIB_CGPS_ClearStatus_Specific( g_hResourceContext, CGPS_Dwell_Complete );
            QLIB_CGPS_ClearStatus_Specific( g_hResourceContext, CGPS_Dwell_Status );

            // Set timeout the timeout log enough to allow for all data to be transported back to the PC
            bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_CGPS_Event, 10000 );
            printf("\nQLIB_ConfigureTimeOut(): %s", bOk ? "PASS" : "FAIL" );

            _oPrescDwellConfig.CMD_CONTROL_FLAG = 1; //1 means "Enable GPS Prescribed Dwell". This activates GPS signal processing.
            _oPrescDwellConfig.SEQ_NUM = 0; //Reserved field.
            _oPrescDwellConfig.GPS_RxD_FLAG = 0; //0 means "No RxD", i.e. only primary GPS chain is used.
            _oPrescDwellConfig.GPS_CONTROL_FLAG = 0; //0 means "Manual code phase and Doppler centering".
            _oPrescDwellConfig.GPS_SV = 1; // 1 means GPS SV ID is 1 (GPS signal simulator must be configured to simulate GPS SV 1).
            _oPrescDwellConfig.GPS_DOPP_HZ = 0;  //Important when _oPrescDwellConfig.GPS_CONTROL_FLAG = 0. (GPS signal simulator must be configured to simulate 0m/s Doppler velocity).
            _oPrescDwellConfig.GPS_DOPP_WIN_HZ = 500;  //Important when _oPrescDwellConfig.GPS_CONTROL_FLAG = 0.
            _oPrescDwellConfig.GPS_CODE_PHASE_CENTER_CHIPX40 = 0; //Important when _oPrescDwellConfig.GPS_CONTROL_FLAG = 0. Specifies where to place the search window in the time domain. Chipx40 is the units.
            _oPrescDwellConfig.GPS_CODE_PHASE_WIN_CHIPX1 = 1023;  //Important when _oPrescDwellConfig.GPS_CONTROL_FLAG = 0. This is the size of the search window in the time domain. Chipx1 is the units.
            _oPrescDwellConfig.GPS_SRCH_MODE = 3; //Search mode 3 means "GPS LBW Chipx2 20ms PDI"
            _oPrescDwellConfig.GPS_NUM_POSTD = 100; //This says how many times the Pre-Detection cycles must be repeated. Search mode 3 has a Pre-Detection of 20ms. So with 100 Post-Detectections, the signal integration duration is 100times*20ms = 2sec.
            _oPrescDwellConfig.GPS_PFA = 4; //4 means the probability of false alarm is 10e-4. This field defines the exponent without the negative sign.
            _oPrescDwellConfig.GPS_RESERVED = 0; //This value is not used.

            /*The GLO parameters are ignored because _oPrescDwellConfig.CMD_CONTROL_FLAG = 1 (1 is for GPS).
            For GLO parameters to become relevant, we need _oPrescDwellConfig.CMD_CONTROL_FLAG = 2 (2 is for GLO). */
            _oPrescDwellConfig.GLO_CONTROL_FLAG = 0;
            _oPrescDwellConfig.GLO_FREQ_K = 8; //R1 frequency in terms of frequency "K" for GLO. Range is -7 to +6.
            _oPrescDwellConfig.GLO_DOPP_HZ = 0; //The Doppler center frequency to search at. Not applicable to Auto dwells.
            _oPrescDwellConfig.GLO_DOPP_WIN_HZ = 32; //Two sided frequency window
            _oPrescDwellConfig.GLO_CODE_PHASE_CENTER_CHIPX40 = 32; //Code phase center in chipx40. Not applicable to Auto dwells. This param is ignored since _oPrescDwellConfig.GLO_CONTROL_FLAG = 1
            _oPrescDwellConfig.GLO_CODE_PHASE_WIN_CHIPX1 = 511; //Code phase window size in units of chips.
            _oPrescDwellConfig.GLO_SRCH_MODE = 17;  //17 means GLO search mode is "GLO Chipx1 20ms PDI".
            _oPrescDwellConfig.GLO_NUM_POSTD = 100; //Number of Post-Detections for GLO
            _oPrescDwellConfig.GLO_HW_CHAN = 1; //GLO HW channel to use. Not applicable to GPS dwells.
            _oPrescDwellConfig.GLO_PFA = 4; //4 means the probability of false alarm is 10e-4. This field defines the exponent without the negative sign.
            _oPrescDwellConfig.GLO_RESERVED = 0; //This value is not used.
            _oPrescDwellConfig.DWELL_CNT = 0;
            _oPrescDwellConfig.TOTAL_NUM_DWELLS = 0;
            _oPrescDwellConfig.PASS_DWELL_CNT = 0;
            _oPrescDwellConfig.RESERVED = 0; //This value is not used.

            // Loop until user presses a key
            unsigned long _iLoopCount = 0;

            while ( !kbhit() )
            {
               // Increment loop count
               _iLoopCount++;

               printf("\n\nSending gpsOne Gen 8 prescribed dwell command #%d", _iLoopCount );

               bOk = QLIB_DIAG_GPS_GEN8_PRESCRIBED_DWELL_WithWait( g_hResourceContext, &_oPrescDwellConfig, &iStatus );

               printf("\nQLIB_DIAG_GPS_GEN8_PRESCRIBED_DWELL_WithWait(): %s", bOk ? "PASS" : "FAIL" );

               // Create a status structure
               CGPS_Status_Struct _oCGPS_Status;

               // Get the status from QMSL
               QLIB_CGPS_GetStatus( g_hResourceContext, (void*) &_oCGPS_Status );

               // Print Number of attempts, Number of reports, Number of valid peaks, Instantaneous C/No, Instantaneous Doppler Frequency
               printf("\nAttempts=%d, Logs=%d, Valid peaks=%d, InstantC/No=%3.1fdB/Hz, Doppler Freq=%5.0fHz",
                      _iLoopCount,
                      _oCGPS_Status.oDwellStatus.iNumEventReports,
                      _oCGPS_Status.oDwellStatus.iNumValidPeaks,
                      _oCGPS_Status.oDwellStatus.oGen8LastPrescribedDwellStatus.CNO_DBHZ,
                      _oCGPS_Status.oDwellStatus.oGen8LastPrescribedDwellStatus.DOPP_HZ
                     );
            }   // while ( !kbhit() )
         } // _bStartDwell

         if ( bOk && _bStartTracking)
         {
            bool _bEnableEvents = true;

            CGPS_Gen8_SvTrack_Struct_type SvTrackParams;

            unsigned char _iStatus = 0;

            //This will start the log message collection
            bOk = QLIB_DIAG_GPS_StartCGPS_AsyncMessages( g_hResourceContext, _bEnableEvents );

            // Enable ELNA
            QLIB_FTM_GNSS_EXTERNAL_LNA(g_hResourceContext,  1 );

            // Enter SA VERif Mode
            bOk = QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_ENTER_MODE, &iStatus );
            printf("\nQLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );

            // Clear status for dwell completions and dwell status
            QLIB_CGPS_ClearStatus_Specific( g_hResourceContext, GNSS_GPS_MeasurementReceived );
            QLIB_CGPS_ClearStatus_Specific( g_hResourceContext, GNSS_GLONASS_MeasurementReceived );

            // Set timeout the timeout log enough to allow for all data to be transported back to the PC
            bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_CGPS_Event, 10000 );
            printf("\nQLIB_ConfigureTimeOut(): %s", bOk ? "PASS" : "FAIL" );

            // Setup the Start Tracking command.
            SvTrackParams.u_TestMode = 2 ;
            SvTrackParams.u_GloHwChan = 0;
            SvTrackParams.u_GpsOrSbasSvId = 1;
            SvTrackParams.b_GloFreqencyId = 0;
            SvTrackParams.u_GpsSbasBitEdgeAction = 1;
            SvTrackParams.u_GloBitEdgeAction = 1;
            SvTrackParams.u_Command =0;

            bOk = QLIB_DIAG_GPS_GEN8_SV_TRACK( g_hResourceContext, &SvTrackParams, &iStatus );

            printf("\n QLIB_DIAG_GPS_GEN8_SV_TRACK(): %s", bOk ? "PASS" : "FAIL" );

            // Loop until user presses a key
            unsigned long _iLoopCount = 0;

            while ( !kbhit() )
            {
               // Increment loop count
               _iLoopCount++;

               // Create a status structure
               CGPS_Status_Struct _oCGPS_Status;

               // Get the status from QMSL
               QLIB_CGPS_GetStatus( g_hResourceContext, (void*) &_oCGPS_Status );

               // Print Number of attempts, Number of reports, Number of valid peaks, Instantaneous C/No, Instantaneous Doppler Frequency
               printf("\n GPS Logs=%d, Average C/No = %f, Min C/No =%f, Max C/No =%f",
                      _oCGPS_Status.oMeasurementReports[1].iNumReports,
                      _oCGPS_Status.oMeasurementReports[1].dC_N_Average_dB,
                      _oCGPS_Status.oMeasurementReports[1].dC_N_Min_dB,
                      _oCGPS_Status.oMeasurementReports[1].dC_N_Max_dB
                     );

               // Print Number of attempts, Number of reports, Number of valid peaks, Instantaneous C/No, Instantaneous Doppler Frequency
               printf("\n GLO Logs=%d, Average C/No = %f, Min C/No =%f, Max C/No =%f",
                      _oCGPS_Status.oGloMeasurementReports[7].iNumReports,
                      _oCGPS_Status.oGloMeasurementReports[7].dC_N_Average_dB,
                      _oCGPS_Status.oGloMeasurementReports[7].dC_N_Min_dB,
                      _oCGPS_Status.oGloMeasurementReports[7].dC_N_Max_dB
                     );

               Sleep(1000);
            }   // while ( !kbhit() )

            // Stop Tracking
            SvTrackParams.u_Command = 3;

            bOk = QLIB_DIAG_GPS_GEN8_SV_TRACK( g_hResourceContext, &SvTrackParams, &iStatus );

            // Disable ELNA
            QLIB_FTM_GNSS_EXTERNAL_LNA(g_hResourceContext,  0 );

            // Exit SA VERif Mode
            bOk = QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_EXIT_MODE, &iStatus );
            printf("\nQLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );
         } // _bStartTracking

         if (bOk && _bTCXOFactoryCal)
         {
            dword centerFreq = 1575420;  // 1575,420 KHz = 1575.42 MHz  - GPS NBIQ center freq
            long  offset     = 200000;  // 200,000 Hz
            long  powerLevel =   -110;  // dBm
            byte  xtal       = (byte)GNSS_TCXOMGR_XTAL_2016;

            printf("\nSending TCXO Coarse Cal Request:");
            printf("\n\tCenter Frequency = %d KHz", centerFreq);
            printf("\n\tOffset           = %d Hz", offset);
            printf("\n\tPower Level      = %d dBm", powerLevel);
            printf("\n\tXTAL:            = TCXOMGR_XTAL_2016");

            unsigned char iStatus = 0;

            if (QLIB_GNSS_XO_Factory_Cal_Request_WithWait(g_hResourceContext,
                                                          centerFreq,
                                                          offset,
                                                          (short)powerLevel,
                                                          xtal,
                                                          &iStatus,
                                                          10000))
            {
               printf("\n\nResults received:");
               printf("\n\tStatus: %d", iStatus);

               // Create a status structure
               CGPS_Status_Struct _oCGPS_Status;

               QLIB_CGPS_GetStatus(g_hResourceContext, (void *)&_oCGPS_Status);

               // Result data in _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response
               printf("\n\tPre factory calibration error: %d (HRPPM)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.INIT_XO_FREQ_ERROR);
               printf("\n\tPost coarse calibration error: %d (HRPPM)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.DC_XO_FREQ_ERROR);
               printf("\n\tTemperature before TCXO: %d (Degree C * 1024)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.INIT_TEMP);
               printf("\n\tTemperature after TCXO: %d (Degree C * 1024)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.FINAL_TEMP);
               printf("\n\tC3 Coefficient: %d (2^-32 ppm/C^3)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.C3);
               printf("\n\tC2 Coefficient: %d (2^-28 ppm/C^2)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.C2);
               printf("\n\tC1 Coefficient: %d (2^-24 ppm/C)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.C1);
               printf("\n\tC0 Coefficient: %d (2^-12 ppm)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.C0);
               printf("\n\tReference XO Temperature Term: %d (Degree C * 1024)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.T0);
               printf("\n\tReference PMIC Temperature: %d (Degree C * 1024)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.T0P);
               printf("\n\tPMIC Compression Factor: %d (HRPPM)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.P);
               printf("\n\tPre factory calibration XO trim capacitance: %d (???)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.INIT_XO_TRIM);
               printf("\n\tPost XO coarse calibration trim capacitance: %d (???)", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.DC_XO_TRIM);
               printf("\n\tFailure Reason: %d", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.FAIL_REASON);
               printf("\n\tCoefficient quality indicator: %d", _oCGPS_Status.o_GNSS_XO_FACTORY_Cal_Response.FT_QUAL_IND);
            } // if (QLIB_GNSS_XO_Factory_Cal_Request_WithWait ...)
         } // if (bOk && _bTCXOFactoryCal)
      }
      while (cKey != 'Q' );
   } // if (bOk )

   return bOk;
}   //unsigned char Test_CGPS_Gen8( void )

unsigned char Test_GNSS_SOC (void )
{
   unsigned char bOk=true;
   unsigned char cKey;
   unsigned char iStatus;

   // For development, logging will be activated automatically, remove this for release
   // when they are removed, also remove the corresponding function calls to stop the log
   // at the end of the function
   unsigned short _iLogFlags = LOG_FN | LOG_RET |  LOG_ERR | LOG_C_HIGH_LEVEL_START  | LOG_C_HIGH_LEVEL_STOP ;

   QLIB_SetLogFlags( g_hResourceContext, _iLogFlags );

   if ( bOk )
   {
      do
      {
         printf("\n\n----------------\nIzAt GNSS_SOC Engine Test\n----------------\n");
         printf("\n1 = Enter RF Verification Mode (IzAt GNSS_SOC)");
         printf("\n2 = Exit RF Verification Mode (IzAt GNSS_SOC)");
         printf("\n3 = Perform Single GPS or GLO SV tracking (IzAt GNSS_SOC");
         printf("\n4 = Perform Multiple SV GPS+GLO Tracking (IzAt GNSS_SOC)" );
         printf("\n5 = Perform Overlapping Multiple SV GPS+GLO Tracking (IzAt GNSS_SOC)");
         printf("\nQ=Exit this menu");

         cKey = toupper( getch() );

         // Reset the OK flag
         bOk = true;

         // Start verification upon direct request
         unsigned char _bStartVerificationMode = ( cKey=='1') ;

         // Stop verification mode upon direct request
         unsigned char _bStopVerificationMode = ( cKey=='2') ;

         // Start a single SV Track
         unsigned char _bSingleSvTrack = (cKey == '3');

         // Start a multiple SV Track
         unsigned char _bMultiSvTrack = ( cKey == '4' );

         // Start a overlapping multiple SV Track
         unsigned char _bOverlapSvTrack = (cKey == '5');

         if ( _bStartVerificationMode )
         {
            bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_CGPS_Event, 30000);

            // Enter Standalone RF verification mode
            bOk = QLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_ENTER_MODE, &iStatus );
            printf("\nQLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );
         }  // _bStartVerificationMode

         if ( bOk && _bStopVerificationMode)
         {
            bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_CGPS_Event, 30000);

            // Exit Standalone RF verification mode
            bOk = QLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_EXIT_MODE, &iStatus );
            printf("\nQLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );
         }   // _bStopVerificationMode

         if ( bOk && _bSingleSvTrack)
         {
            bool _bEnableEvents = true;

            GNSS_SOC_SINGLE_SvTrack_Struct_type SvTrackParams;

            unsigned char _iStatus = 0;

            //This will start the log message collection
            bOk = QLIB_DIAG_GPS_StartCGPS_AsyncMessages( g_hResourceContext, _bEnableEvents );
            printf("\nQLIB_ConfigureTimeOut(): %s", bOk ? "PASS" : "FAIL" );

            bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_CGPS_Event, 30000);

            // Enter SA VERif Mode
            bOk = QLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_ENTER_MODE, &iStatus );
            printf("\nQLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );

            if (bOk)
            {
               // Clear status for dwell completions and dwell status
               QLIB_CGPS_ClearStatus_Specific( g_hResourceContext, CGPS_GPS_MeasurementReceived );  // Reused for GNSS_SOC

               // Setup the Start Tracking command.
               SvTrackParams.u_duration         = 60;  // Seconds of track duration
               SvTrackParams.u_TestMode         =  1;  // GPS Track (0 - GLO Track)
               SvTrackParams.u_GpsSvId          =  1;  // GPS SV ID
               SvTrackParams.b_GloFreqencyId    =  0;  // GLO SV Frequency ID

               bOk = QLIB_DIAG_GNSS_SOC_SINGLE_SV_TRACK( g_hResourceContext, &SvTrackParams, &iStatus );

               printf("\n QLIB_DIAG_GNSS_SOC_GEN8_SV_TRACK(): %s", bOk ? "PASS" : "FAIL" );

               // Loop until 45 measurements are received for at least one SV/PRN
               unsigned long _iLoopCount = 0;

               int  i_LogsReceived = -1;

               while ( (i_LogsReceived < 45) && ( !kbhit()) )
               {
                  // Increment loop count
                  _iLoopCount++;

                  // Create a status structure
                  CGPS_Status_Struct _oCGPS_Status;

                  // Get the status from QMSL
                  QLIB_CGPS_GetStatus( g_hResourceContext, (void*) &_oCGPS_Status );

                  i_LogsReceived = _oCGPS_Status.o_GNSS_SOC_GainData.iReportCount;
                  printf("\n\nTotal Logs Received: %d", i_LogsReceived);

                  GNSS_SOC_MeasurementSummary * _pGNSS_SOCSummary = (GNSS_SOC_MeasurementSummary *) &_oCGPS_Status.o_GNSS_SOC_MeasurementSummary[0];
                  int     i_prn           = _pGNSS_SOCSummary->iprn;
                  double  dCNo_Mean       = _pGNSS_SOCSummary->dCNo_Mean;
                  double  dCNo_StdDev     = _pGNSS_SOCSummary->dCNo_StdDev;
                  double  dSPR_Mean       = _pGNSS_SOCSummary->dSPR_Mean;
                  double  dSPR_StdDev     = _pGNSS_SOCSummary->dSPR_StdDev;
                  double  dDoppler_Mean   = _pGNSS_SOCSummary->dDoppler_Mean;
                  double  dDoppler_StdDev = _pGNSS_SOCSummary->dDoppler_StdDev;

                  printf("\nSummary for PRN %d: Mean C/No                   = %3.5f, Std Dev C/No    = %3.5f\n"
                         "                   Mean System Processing Loss = %3.5f, Std Dev SPL     = %3.5f\n"
                         "                   Mean Doppler                = %3.5f, Std Dev Doppler = %3.5f",
                         i_prn, dCNo_Mean, dCNo_StdDev, dSPR_Mean, dSPR_StdDev, dDoppler_Mean, dDoppler_StdDev);

                  Sleep(1000);
               }    // while ( !kbhit() )
            }

            // Exit SA VERif Mode
            bOk = QLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_EXIT_MODE, &iStatus );
            printf("\nQLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );
         } // _bStartTracking

         if ( bOk && _bMultiSvTrack)
         {
            bool _bEnableEvents = true;

            CGPS_Gen8_SvTrack_Struct_type SvTrackParams;

            unsigned char _iStatus = 0;

            //This will start the log message collection
            bOk = QLIB_DIAG_GPS_StartCGPS_AsyncMessages( g_hResourceContext, _bEnableEvents );
            printf("\nQLIB_ConfigureTimeOut(): %s", bOk ? "PASS" : "FAIL" );

            bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_CGPS_Event, 30000);

            // Enter SA VERif Mode
            bOk = QLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_ENTER_MODE, &iStatus );
            printf("\nQLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );

            if (bOk)
            {
               // Clear status for dwell completions and dwell status
               QLIB_CGPS_ClearStatus_Specific( g_hResourceContext, CGPS_GPS_MeasurementReceived );  // Reused for GNSS_SOC

               // Setup the Start Tracking command.
               SvTrackParams.u_TestMode = 5 ;  // Reused for track duration
               SvTrackParams.u_GloHwChan = 0;  // All others set to 0 (don't care)
               SvTrackParams.u_GpsOrSbasSvId = 0;
               SvTrackParams.b_GloFreqencyId = 0;
               SvTrackParams.u_GpsSbasBitEdgeAction = 0;
               SvTrackParams.u_GloBitEdgeAction = 0;
               SvTrackParams.u_Command =0;

               bOk = QLIB_DIAG_GNSS_SOC_GEN8_SV_TRACK( g_hResourceContext, &SvTrackParams, &iStatus );

               printf("\n QLIB_DIAG_GNSS_SOC_GEN8_SV_TRACK(): %s", bOk ? "PASS" : "FAIL" );

               // Loop until 60 measurements are received for at least one SV/PRN
               unsigned long _iLoopCount = 0;

               int  i_LogsReceived = -1;

               while ( (i_LogsReceived < 60) && ( !kbhit()) )
               {
                  // Increment loop count
                  _iLoopCount++;

                  // Create a status structure
                  CGPS_Status_Struct _oCGPS_Status;

                  // Get the status from QMSL
                  QLIB_CGPS_GetStatus( g_hResourceContext, (void*) &_oCGPS_Status );

                  i_LogsReceived = _oCGPS_Status.o_GNSS_SOC_GainData.iReportCount;
                  printf("\n\nTotal Logs Received: %d", i_LogsReceived);

                  for (int i_PrnIndex = 0;
                        (i_PrnIndex < CGPS_MAX_NUM_SV_IN_MEASUREMENT_REPORT) && (_oCGPS_Status.o_GNSS_SOC_MeasurementSummary[i_PrnIndex].iprn != -1);
                        i_PrnIndex++)
                  {
                     GNSS_SOC_MeasurementSummary * _pGNSS_SOCSummary = (GNSS_SOC_MeasurementSummary *) &_oCGPS_Status.o_GNSS_SOC_MeasurementSummary[0];
                     int     i_prn           = _pGNSS_SOCSummary->iprn;
                     double  dCNo_Mean       = _pGNSS_SOCSummary->dCNo_Mean;
                     double  dCNo_StdDev     = _pGNSS_SOCSummary->dCNo_StdDev;
                     double  dSPR_Mean       = _pGNSS_SOCSummary->dSPR_Mean;
                     double  dSPR_StdDev     = _pGNSS_SOCSummary->dSPR_StdDev;
                     double  dDoppler_Mean   = _pGNSS_SOCSummary->dDoppler_Mean;
                     double  dDoppler_StdDev = _pGNSS_SOCSummary->dDoppler_StdDev;

                     printf("\nSummary for PRN %d: Mean C/No                   = %3.5f, Std Dev C/No    = %3.5f\n"
                            "                   Mean System Processing Loss = %3.5f, Std Dev SPL     = %3.5f\n"
                            "                   Mean Doppler                = %3.5f, Std Dev Doppler = %3.5f",
                            i_prn, dCNo_Mean, dCNo_StdDev, dSPR_Mean, dSPR_StdDev, dDoppler_Mean, dDoppler_StdDev);
                  }

                  Sleep(1000);
               }    // while ( !kbhit() )
            }

            // Exit SA VERif Mode
            bOk = QLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_EXIT_MODE, &iStatus );
            printf("\nQLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );
         } // _bMultiSvTrack

         if ( bOk && _bOverlapSvTrack)
         {
            bool _bEnableEvents = true;

            CGPS_Gen8_SvTrack_Struct_type SvTrackParams;

            unsigned char _iStatus = 0;

            //This will start the log message collection
            bOk = QLIB_DIAG_GPS_StartCGPS_AsyncMessages( g_hResourceContext, _bEnableEvents );
            printf("\nQLIB_ConfigureTimeOut(): %s", bOk ? "PASS" : "FAIL" );

            bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_CGPS_Event, 30000);

            // Enter SA VERif Mode
            bOk = QLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_ENTER_MODE, &iStatus );
            printf("\nQLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );

            if (bOk)
            {
               for (int trackCount = 0; trackCount < 3; trackCount++)
               {
                  // Clear status for dwell completions and dwell status
                  QLIB_CGPS_ClearStatus_Specific( g_hResourceContext, CGPS_GPS_MeasurementReceived );  // Reused for GNSS_SOC

                  // Setup the Start Tracking command.
                  SvTrackParams.u_TestMode = 1;  // Reused for track duration
                  SvTrackParams.u_GloHwChan = 0;  // All others set to 0 (don't care)
                  SvTrackParams.u_GpsOrSbasSvId = 0;
                  SvTrackParams.b_GloFreqencyId = 0;
                  SvTrackParams.u_GpsSbasBitEdgeAction = 0;
                  SvTrackParams.u_GloBitEdgeAction = 0;
                  SvTrackParams.u_Command = 0;

                  bOk = QLIB_DIAG_GNSS_SOC_GEN8_SV_TRACK( g_hResourceContext, &SvTrackParams, &iStatus );

                  printf("\n Iteration %d - QLIB_DIAG_GNSS_SOC_GEN8_SV_TRACK(): %s", trackCount, bOk ? "PASS" : "FAIL" );

                  Sleep(2000);  // Wait 2 seconds before expecting results.

                  // Loop until 60 measurements are received for at least one SV/PRN
                  unsigned long _iLoopCount = 0;

                  int  i_LogsReceived = -1;
                  int  i_LastLogCount = -2;
                  int  i_SameLogCount =  0;

                  while ( (i_LogsReceived < 60) && (i_SameLogCount < 10) && ( !kbhit()) )
                  {
                     // Increment loop count
                     _iLoopCount++;

                     // Create a status structure
                     CGPS_Status_Struct _oCGPS_Status;

                     // Get the status from QMSL
                     QLIB_CGPS_GetStatus( g_hResourceContext, (void*) &_oCGPS_Status );

                     i_LastLogCount = i_LogsReceived;

                     i_LogsReceived = _oCGPS_Status.o_GNSS_SOC_GainData.iReportCount;
                     printf("\n\nTotal Logs Received: %d", i_LogsReceived);

                     if (i_LastLogCount == i_LogsReceived)
                     {
                        i_SameLogCount += 1;
                     }
                     else
                     {
                        i_SameLogCount = 0;
                     }

                     for (int i_PrnIndex = 0;
                           (i_PrnIndex < CGPS_MAX_NUM_SV_IN_MEASUREMENT_REPORT) && (_oCGPS_Status.o_GNSS_SOC_MeasurementSummary[i_PrnIndex].iprn != -1);
                           i_PrnIndex++)
                     {
                        GNSS_SOC_MeasurementSummary * _pGNSS_SOCSummary = (GNSS_SOC_MeasurementSummary *) &_oCGPS_Status.o_GNSS_SOC_MeasurementSummary[i_PrnIndex];
                        int     i_prn           = _pGNSS_SOCSummary->iprn;
                        double  dCNo_Mean       = _pGNSS_SOCSummary->dCNo_Mean;
                        double  dCNo_StdDev     = _pGNSS_SOCSummary->dCNo_StdDev;
                        double  dSPR_Mean       = _pGNSS_SOCSummary->dSPR_Mean;
                        double  dSPR_StdDev     = _pGNSS_SOCSummary->dSPR_StdDev;
                        double  dDoppler_Mean   = _pGNSS_SOCSummary->dDoppler_Mean;
                        double  dDoppler_StdDev = _pGNSS_SOCSummary->dDoppler_StdDev;

                        printf("\nSummary for PRN %d: Mean C/No                   = %3.5f, Std Dev C/No    = %3.5f\n"
                               "                   Mean System Processing Loss = %3.5f, Std Dev SPL     = %3.5f\n"
                               "                   Mean Doppler                = %3.5f, Std Dev Doppler = %3.5f",
                               i_prn, dCNo_Mean, dCNo_StdDev, dSPR_Mean, dSPR_StdDev, dDoppler_Mean, dDoppler_StdDev);
                     }

                     Sleep(1000);
                  } // while ( !kbhit() )

                  if(trackCount < 3)
                  {
                     printf("\nWaiting 10 seconds for GNSS system to rest");
                     Sleep(10000);  // Wait 10 seconds between tracks.
                  }
               } // for trackCount
            }

            // Exit SA VERif Mode
            bOk = QLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH_WithWait( g_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_EXIT_MODE, &iStatus );
            printf("\nQLIB_DIAG_GNSS_SOC_SA_RF_VERIF_MODE_SWITCH(): %s, status %d",
                   bOk ? "PASS" : "FAIL",
                   iStatus );
         } // _bOverlapSvTrack
      }
      while (cKey != 'Q' );
   } // if (bOk )

   return bOk;
}   //unsigned char Test_GNSS_SOC( void )

unsigned char Test_FTM_BT( void )
{
   unsigned char bOk=true;
   unsigned char cKey;

   printf("----------------\nBluetooth FTM Test\n----------------\n");

   if ( bOk )
   {
      printf("\n\n1=Activate BT");
      printf("\n2=Bluetooth Tx on");
      printf("\n3=Bluetooth Address = 0x010203040506");
      printf("\n4=Get Module Local_Version ");
      printf("\n5=QLIB_FTM_BT_HCI_USER_CMD_WithEventResponse");
      printf("\n6=HCI_Reset");
      printf("\n7=Retrieve WCN 36xx self test (pin connectivity results");

      cKey = getch();

      if (cKey == '1' )
      {
         // Enable Bluetooth
         bOk = QLIB_FTM_BT_Enable_Bluetooth( g_hResourceContext );
         printf("\nQLIB_FTM_BT_Enable_Bluetooth: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == 1
      else if (cKey == '2' )
      {
         unsigned char iChannel = 0;

         // Set BT Tx on
         bOk = QLIB_FTM_BT_SetCW( g_hResourceContext,  iChannel );

         printf("\nQLIB_FTM_BT_SetCW: %s", bOk ? "PASS" : "FAIL" );

         printf("\n\n View signal at 2402MHz");
      }// cKey == 2
      else if (cKey == '3' )
      {
         unsigned char iAddr[6] = {0x19,0x82, 0x73, 0x64, 0x56, 0x47};

         // Set BT address
         bOk = QLIB_FTM_BT_SetAddr( g_hResourceContext,  iAddr );

         printf("\nQLIB_FTM_BT_SetAddr: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == 3
      else if (cKey == '4' )
      {
         Bluetooth_LocalVersion_struct _oVersionStorage;

         // Set BT address
         bOk = QLIB_BT_GetModuleVersion( g_hResourceContext,  (unsigned char*) &_oVersionStorage );

         printf("\nQLIB_BT_GetModuleVersion: %s", bOk ? "PASS" : "FAIL" );

         if (bOk)
         {
            // Print the results
            printf("\n         HCI_Version: %d", _oVersionStorage.HCI_Version       );
            printf("\n        HCI_Revision: %d", _oVersionStorage.HCI_Revision      );
            printf("\n         LMP_Version: %d", _oVersionStorage.LMP_Version       );
            printf("\n   Manufacturer_Name: %d", _oVersionStorage.Manufacturer_Name );
            printf("\n      LMP_Subversion: %d", _oVersionStorage.LMP_Subversion    );
         }
      }// cKey == 4

      else if (cKey == '5' )
      {
         const byte _iHCI_Enable_DUT_len = 4;
         unsigned char _aiHCI_Enable_DUT[ _iHCI_Enable_DUT_len] = { 1, 3, 24, 0 };
         int _i = 0;

         unsigned char _aResponse[258];  // 255 + 3 (BT HCI command parameters + header)

         unsigned short _iResponseSz = 258;

         printf("\nSend HCI Enable DUT Command (HEX): ");
         for(_i = 0; _i < _iHCI_Enable_DUT_len; _i++)
         {
            printf("0x%X ", _aiHCI_Enable_DUT[_i]);
         }
         printf("\n");

         // Send a BT command and wait for a response
         // Call QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_BT_HCI_Response, new time out in MS) if needed
         QLIB_FTM_BT_HCI_USER_CMD_WithEventResponse(g_hResourceContext, _aiHCI_Enable_DUT, _iHCI_Enable_DUT_len, _aResponse, &_iResponseSz);

         printf("\nBT HCI Event/Response Size (DEC)= %d Data (HEX):", _iResponseSz);

         for (_i = 0; _i < _iResponseSz; _i++)
         {
            printf("0x%X ", _aResponse[_i]);
         }
         printf("\n");
      }// cKey == 5
      else if (cKey == '6' )
      {
         // Enable Bluetooth
         bOk = QLIB_FTM_BT_HCI_Reset( g_hResourceContext );
         printf("\nQLIB_FTM_BT_HCI_Reset: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == 6
      else if (cKey == '7' )
      {
         const byte _iHCI_WCN_36xx_Self_Test_Cmd_Sz = 5;

         // On WCN36xx, the device will perform a self test during bootup.
         // The following HCI can be used to retrieve the self test result.
         // Please refer to 80-WL300-13 for this command
         unsigned char _iHCI_WCN_36xx_Self_Test_Cmd[ _iHCI_WCN_36xx_Self_Test_Cmd_Sz] = { 0x01, 0x0C, 0xFC, 0x01, 0x0B };
         int _i = 0;

         unsigned char _aResponse[258];  // 255 + 3 (BT HCI command parameters + header)

         unsigned short _iResponseSz = 258;

         printf("\nSend Get WCN 36xx Sef Test Result Command (HEX): ");
         for(_i = 0; _i < _iHCI_WCN_36xx_Self_Test_Cmd_Sz; _i++)
         {
            printf("0x%X ", _iHCI_WCN_36xx_Self_Test_Cmd[_i]);
         }
         printf("\n");

         // Send a BT command and wait for a response
         // Call QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_BT_HCI_Response, new time out in MS) if needed
         QLIB_FTM_BT_HCI_USER_CMD_WithEventResponse(g_hResourceContext, _iHCI_WCN_36xx_Self_Test_Cmd, _iHCI_WCN_36xx_Self_Test_Cmd_Sz, _aResponse, &_iResponseSz);

         printf("\nBT HCI Event/Response Size (DEC)= %d Data (HEX):", _iResponseSz);

         for (_i = 0; _i < _iResponseSz; _i++)
         {
            printf("0x%X ", _aResponse[_i]);
         }
         printf("\n");
      }// cKey == 7
   }

   return bOk;
}

unsigned char Test_FTM_PMIC( void )
{
   unsigned char bOk=true;
   unsigned char cKey;

   printf("----------------\nPMIC FTM Test\n----------------\n");

   if ( bOk )
   {
      printf("\n\n1=FTM GET RTC");
      printf("\n2=Disable VREG_MSME (will cause reset)");
      printf("\n3=Vibe ON");
      printf("\n4=Vibe OFF");
      printf("\n5=BT (WLAN) ON");
      printf("\n6=BT (WLAN) OFF");
      printf("\n7=FTM SET RTC, 1980");
      printf("\n8=FTM SET RTC, 2005");
      printf("\n9=GET Vreg Enumeration List");
      cKey = getch();

      if (cKey == '1' )
      {
         unsigned char iReadWrite = 1; // one for get
         unsigned char iTimeMode = 0;
         unsigned char iMonth = 0;
         unsigned char iDay = 0;
         unsigned short iYear = 0;
         unsigned char iHour = 0;
         unsigned char iMinute = 0;
         unsigned char iSecond = 0;

         bOk = QLIB_FTM_PMIC_RTC_SET_GET_TIME( g_hResourceContext, iReadWrite, &iTimeMode, &iMonth, &iDay, &iYear, &iHour, &iMinute, &iSecond);

         printf("\nFTM GET RTC: %s", bOk ? "PASS" : "FAIL\n" );

         if (bOk )
         {
            printf("\nTime Mode: %d", iTimeMode );
            printf("\nMonth: %d", iMonth );
            printf("\nDay: %d", iDay );
            printf("\nYear: %d", iYear );
            printf("\nHour: %d", iHour );
            printf("\nMinute: %d", iMinute );
            printf("\nSecond: %d", iSecond );
         }
      }// cKey == 1
      else if (cKey == '2')
      {
         bOk = QLIB_FTM_PMIC_VREG_CONTROL( g_hResourceContext, 0, 0x00000004);

         printf("\nQLIB_FTM_PMIC_VREG_CONTROL(0,4): %s", bOk ? "PASS" : "FAIL" );
      } // cKey == 2
      else if (cKey == '3')
      {
         // VIBE ON. 2 = VIBE, 20 = 200mA
         bOk = QLIB_FTM_SET_PMIC_DRIVE_LEVEL( g_hResourceContext, 2, 20);

         printf("\nQLIB_FTM_SET_PMIC_DRIVE_LEVEL(2,20): %s", bOk ? "PASS" : "FAIL" );
      } // cKey == 3
      else if (cKey == '4')
      {
         // VIBE OFF. 2 = VIBE, 0 = 0mA
         bOk = QLIB_FTM_SET_PMIC_DRIVE_LEVEL( g_hResourceContext, 2, 0);

         printf("\nQLIB_FTM_SET_PMIC_DRIVE_LEVEL(2,0): %s", bOk ? "PASS" : "FAIL" );
      } // cKey == 4
      else if (cKey == '5')
      {
         const unsigned short c_iVREG_WLAN = 0x00008000;
         const unsigned char c_iState = 0x01;

         bOk = QLIB_FTM_PMIC_VREG_CONTROL( g_hResourceContext, c_iState, c_iVREG_WLAN );

         printf("\nQLIB_FTM_PMIC_VREG_CONTROL(iState = %d,VREG = 0x%X): %s",c_iState, c_iVREG_WLAN , bOk ? "PASS" : "FAIL" );
      } // cKey == 5
      else if (cKey == '6')
      {
         const unsigned short c_iVREG_WLAN = 0x00008000;
         const unsigned char c_iState = 0x00;

         bOk = QLIB_FTM_PMIC_VREG_CONTROL( g_hResourceContext, c_iState, c_iVREG_WLAN );

         printf("\nQLIB_FTM_PMIC_VREG_CONTROL(iState = %d,VREG = 0x%X): %s",c_iState, c_iVREG_WLAN , bOk ? "PASS" : "FAIL" );
      } // cKey == 5
      else if (cKey == '7' )
      {
         unsigned char iReadWrite = 0; // zero for set
         unsigned char iTimeMode = 2;
         unsigned char iMonth = 7;
         unsigned char iDay = 7;
         unsigned short iYear = 1980;
         unsigned char iHour = 0;
         unsigned char iMinute = 0;
         unsigned char iSecond = 0;

         bOk = QLIB_FTM_PMIC_RTC_SET_GET_TIME( g_hResourceContext, iReadWrite, &iTimeMode, &iMonth, &iDay, &iYear, &iHour, &iMinute, &iSecond);

         printf("\nFTM SET RTC: %s", bOk ? "PASS" : "FAIL\n" );
      }// cKey == 7
      else if (cKey == '8' )
      {
         unsigned char iReadWrite = 0; // zero for set
         unsigned char iTimeMode = 2;
         unsigned char iMonth = 1;
         unsigned char iDay = 1;
         unsigned short iYear = 2005;
         unsigned char iHour = 1;
         unsigned char iMinute = 1;
         unsigned char iSecond = 1;

         bOk = QLIB_FTM_PMIC_RTC_SET_GET_TIME( g_hResourceContext, iReadWrite, &iTimeMode, &iMonth, &iDay, &iYear, &iHour, &iMinute, &iSecond);

         printf("\nFTM SET RTC: %s", bOk ? "PASS" : "FAIL\n" );
      }// cKey == 8
      else if (cKey == '9' )
      {
         unsigned short iNumVreg = 0; // zero for set
         short iBufferSize = 1024;
         unsigned char *pBuffer = new unsigned char[iBufferSize];
         unsigned short iStatus = 0;

         bOk = QLIB_FTM_PMIC_VREG_GET_ENUM_LIST( g_hResourceContext, &iNumVreg, &iBufferSize, pBuffer, &iStatus);

         printf("\nQLIB_FTM_PMIC_VREG_GET_ENUM_LIST :%s, iNumVreg=%d, List=%s", bOk ? "PASS":"FAIL", iNumVreg, pBuffer);
      }// cKey == 9
   }

   return bOk;
}

/*
Purpose
- Technique to configure/capture asynchronous diagnostic messages when the customer
software  cannot support such messages
Functionality
- Use a Windows computer with a cross over UART cable to the a Test Application computer

- Special diagnostic router software would be need, to pass only synchronous traffic,
while logging async traffic.

*/

#define LOG_TIME 60000
unsigned char SerialPortRouter( void )
{
   /*
   Create a new connection, which will model the "Router port", which is connected to the external desktop computer using QLIB_ConnectServer()

   Start Text logging on "Router port"
   Start Text logging on "DUT port"
   Start DLF logging on "DUT port"

   loop while ! completed
   read synchronous command data from "Router port" using QLIB_DIAG_GetNextCommandResponse()
   if received then write the command data to "DUT port" using QLIB_SendASync()

   read synchronous command data from the "DUT port" using QLIB_DIAG_GetNextCommandResponse()
   if received then write the command data to the "Router port" using QLIB_SendASync()

   read asynchronous command data from the "DUT port" using QLIB_DIAG_GetNextPhoneLog()
   if received then clear the async queue using QLIB_DIAG_ClearPhoneLogQueue()

   */

   // Create a new connection, which will model the "Router port", which is connected to the external desktop computer using QLIB_ConnectServer()
   HANDLE _hRouterContext = NULL;

   // Completed flag
   unsigned char _bContinue = TRUE;

   // Temporary buffer
   unsigned short _iBufferSize = 0;
   unsigned char _aBuffer[ DIAG_MAX_PACKET_SIZE ];

   unsigned char _bRouterMessageReceived = FALSE;
   unsigned char _bDUTMessageReceived = FALSE;
   unsigned char _bDUTAsyncReceived = FALSE;

   char _RouterPort_LogFile[] = "c:\\temp\\QMSL_Router_Port";
   char _DUTPort_LogFile[] = "c:\\temp\\QMSL_DUT_Port";
   char _DUTPort_DLFLogFile[] = "c:\\temp\\QMSL_DUT_Port";
   char _tempbuf[100];  //buffer to store file names temporarily
   char _dir_path[] = "c:\\temp\\SPR_log";

   //timer declarations
   unsigned long _hTimer = NULL;
   unsigned long _hTimer2 = NULL;
   unsigned long _jElapsedTimer = 0;
   unsigned long _iElapsedTime = 0;
   unsigned int _run_count = 0;

   unsigned short _iRouterComPort = 0;
   int _iRouterComPort_int = 0;
   unsigned short _pause_flag = 0;

   printf("Enter the Router COM port: ");
   scanf("%d",&_iRouterComPort_int);
   _iRouterComPort = (unsigned short) _iRouterComPort_int;

   QLIB_SetLibraryMode(QLIB_LIB_MODE_QPHONEMS); // enable the qphonems mode for the router port

   _hRouterContext  = QLIB_ConnectServer( _iRouterComPort );

   if ( _hRouterContext != NULL )
   {
      printf("\nRouter port opened");

      printf("\nSetting up log files...\n");

      while(_bContinue)
      {
         //start the log timer
         _hTimer = QLIB_StartTimer( g_hResourceContext );

         // Start Text logging on "Router port"

         sprintf(_tempbuf,"%s.txt",_RouterPort_LogFile);
         QLIB_StartLogging( _hRouterContext,(char *)_tempbuf);
         printf("\nCollecting Router Port logs in the file - %s",_tempbuf);
         memset(_tempbuf,0,sizeof(_tempbuf));   // clear the buffer

         // Start Text logging on "DUT port"

         sprintf(_tempbuf,"%s.txt",_DUTPort_LogFile);
         QLIB_StartLogging( g_hResourceContext,(char *)_tempbuf);
         printf("\nCollecting DUT Port logs in the file - %s",_tempbuf);
         memset(_tempbuf,0,sizeof(_tempbuf));   // clear the buffer

         // Start DLF logging on "DUT port"

         sprintf(_tempbuf,"%s.dlf",_DUTPort_DLFLogFile);
         QLIB_StartDLF_Logging( g_hResourceContext, (char *)_tempbuf, false);
         printf("\nCollecting DUT Port DLF logs in the file - %s\n",_tempbuf);
         memset(_tempbuf,0,sizeof(_tempbuf));   // clear the buffer

         //timer loop starts

         while((((unsigned long) QLIB_GetTimerElapsed( g_hResourceContext, _hTimer)) < LOG_TIME) && _bContinue)
         {
            // force buffer size to zero
            _iBufferSize = 0;

            // Clear temporary buffer, for debug
            memset( _aBuffer,0xCC, DIAG_MAX_PACKET_SIZE );

            // read synchronous command data from "Router port" using QLIB_DIAG_GetNextCommandResponse()
            _bRouterMessageReceived = QLIB_DIAG_GetNextCommandResponse
                                      (
                                         _hRouterContext,
                                         &_iBufferSize,
                                         _aBuffer,
                                         0 // iTimeOut
                                      );

            // if received then write the command data to "DUT port" using QLIB_SendASync()
            if (    ( _bRouterMessageReceived )
                    &&   ( _iBufferSize > 2 )
               )
            {
               _pause_flag = 0;
               QLIB_SendASync
               (
                  g_hResourceContext,
                  _iBufferSize - 2, // remove CRC, 2 bytes
                  _aBuffer,
                  0, // iTimeout,
                  FALSE // bStartFlag
               );
               printf("R"); // R for Router port
            }

            // if no command data is received from the "Router port", wait here, to avoid empty log files that maybe generated
            /*
            else
            {
            _pause_flag == 1;
            }

            if(_pause_flag && !((unsigned long) QLIB_GetTimerElapsed( g_hResourceContext, _hTimer2)) )
            {
            _hTimer2 = QLIB_StartTimer( g_hResourceContext );
            }

            // if (((unsigned long) QLIB_GetTimerElapsed( g_hResourceContext, _hTimer2)) > 5000)
            {
            QLIB_StopTimer( g_hResourceContext, _hTimer2);
            while(!(QLIB_DIAG_GetNextCommandResponse
            (
            _hRouterContext,
            &_iBufferSize,
            _aBuffer,
            0 // iTimeOut
            )));
            }
            */

            // Clear temporary buffer, for debug
            memset( _aBuffer,0xCC, DIAG_MAX_PACKET_SIZE );

            // read synchronous command data from the "DUT port" using QLIB_DIAG_GetNextCommandResponse()
            _bDUTMessageReceived = QLIB_DIAG_GetNextCommandResponse
                                   (
                                      g_hResourceContext,
                                      &_iBufferSize,
                                      _aBuffer,
                                      0 // iTimeOut
                                   );

            // if received then write the command data to the "Router port" using QLIB_SendASync()
            if (    ( _bDUTMessageReceived )
                    &&   ( _iBufferSize > 2 )
               )
            {
               QLIB_SendASync
               (
                  _hRouterContext,
                  _iBufferSize - 2, // remove CRC, 2 bytes
                  _aBuffer,
                  0, // iTimeout,
                  FALSE // bStartFlag
               );
               printf("D"); // D for DUT port
            }

            // read asynchronous command data from the "DUT port" using QLIB_DIAG_GetNextPhoneLog()
            _bDUTAsyncReceived = QLIB_DIAG_GetNextPhoneLog
                                 (g_hResourceContext,
                                  &_iBufferSize,
                                  _aBuffer,
                                  0
                                 );

            // if received then clear the async queue using QLIB_DIAG_ClearPhoneLogQueue()
            if (    ( _bDUTAsyncReceived )
                    &&   ( _iBufferSize > 2 )
               )
            {
               QLIB_DIAG_ClearPhoneLogQueue(g_hResourceContext);
            }

            // Let the OS perform other tasks
            Sleep( 1 );

            //test code to check queue size
            // Check if keyboard hit
            if(kbhit())
            {
               unsigned char cKey = toupper(getch());
               unsigned long _PhoneLogQueueSize = (unsigned long)_iBufferSize;
               if(cKey == '1')
               {
                  printf("\nPhone Log Queue size: %d",(int)QLIB_DIAG_GetPhoneLogQueueSize(g_hResourceContext,&_PhoneLogQueueSize));
                  printf("\nCommand Response Queue Size: %d",(int)QLIB_DIAG_GetCommandResponseQueueSize(_hRouterContext,&_PhoneLogQueueSize ));
                  _bContinue = TRUE;
               }

               else
               {
                  _bContinue = FALSE;
               }
            }
         } //while (timer_elapsed time < LOG_TIME && _bContinue)

         QLIB_StopLogging(_hRouterContext);
         QLIB_StopLogging(g_hResourceContext);

         _iElapsedTime =(unsigned long) QLIB_GetTimerElapsed( g_hResourceContext, _hTimer );
         QLIB_StopTimer( g_hResourceContext, _hTimer);

         _run_count++;
         //sprintf(_tempbuf,"mkdir %s%3.3d",_dir_path,_run_count);
         //system(_tempbuf);
         memset(_tempbuf,0,sizeof(_tempbuf));   // clear the buffer
         sprintf(_tempbuf,"mv %s.txt %s%3.3d.txt",_RouterPort_LogFile,_RouterPort_LogFile, _run_count);
         system(_tempbuf);
         memset(_tempbuf,0,sizeof(_tempbuf));   // clear the buffer
         sprintf(_tempbuf,"mv %s.txt %s%3.3d.txt",_DUTPort_LogFile, _DUTPort_LogFile, _run_count);
         system(_tempbuf);
         memset(_tempbuf,0,sizeof(_tempbuf));   // clear the buffer
         sprintf(_tempbuf,"mv %s.dlf %s%3.3d.dlf",_DUTPort_DLFLogFile, _DUTPort_DLFLogFile, _run_count);
         system(_tempbuf);
         memset(_tempbuf,0,sizeof(_tempbuf));   // clear the buffer
      }// while(_bContinue)
   } // ( _hRouterContext != NULL )

   else
   {
      printf("\nRouter port (COM%d) failed to open", _iRouterComPort );
   } // else ( _hRouterContext != NULL )

   // clear 1 character from keyboard buffer
   if ( kbhit() )
      QLIB_DisconnectServer(_hRouterContext);  // disconnect the router context
   getch();

   return 0;
}   // Serial port Router

unsigned char DiagStressTest( void )
{
   /*
   static unsigned char aiRequestBuffer  [ DIAG_MAX_PACKET_SIZE ] = {0x7B,0x00,0x00,0x25,0x00,0x00,0x00,0x44,0x05,0x0C,0x00,0x47,0x00,0x12,0x00,0xE1,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x01,0x03,0x05,0x00,0x00,0x0A,0x00,0xE2,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x0E,0x00,0xE3,0x00,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x20,0xBF,0x02,0x00,0x0A,0x00,0xE4,0x00,0x00,0x00,0x4B,0x0B,0x1B,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0xE5,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0xE6,0x00,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0xD0,0x6C,0x04,0x00,0x0A,0x00,0xE7,0x00,0x00,0x00,0x4B,0x0B,0x1B,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0xE8,0x00,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x10,0x27,0x00,0x00,0x12,0x00,0xE9,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x01,0x03,0x0D,0x00,0x00,0x0A,0x00,0xEA,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x0E,0x00,0xEB,0x00,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0xB0,0xAD,0x01,0x00,0x0A,0x00,0xEC,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0xED,0x00,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x20,0x4E,0x00,0x00,0x12,0x00,0xEE,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x01,0x03,0x12,0x00,0x00,0x0A,0x00,0xEF,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x0E,0x00,0xF0,0x00,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0xB0,0xAD,0x01,0x00,0x0A,0x00,0xF1,0x00,0x00,0x00,0x4B,0x0B,0x1B,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0xF2,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0xF3,0x00,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x60,0xEA,0x00,0x00,0x0A,0x00,0xF4,0x00,0x00,0x00,0x4B,0x0B,0x1B,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0xF5,0x00,0x00,0x00,0x4B,0x0B,0x25,0x00,0x0
   1,0x00,0x04,0x00,0x0C,0x00,0x10,0x27,0x00,0x00,0x13,0x00,0xF6,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x95,0x02,0x02,0x01,0x03,0x00,0x00,0x12,0x00,0xF7,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x01,0x03,0x12,0x00,0x00,0x0A,0x00,0xF8,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x0E,0x00,0xF9,0x00,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0xA0,0x86,0x01,0x00,0x12,0x00,0xFA,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x01,0x03,0x05,0x00,0x00,0x0A,0x00,0xFB,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x0E,0x00,0xFC,0x00,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x20,0xBF,0x02,0x00,0x0A,0x00,0xFD,0x00,0x00,0x00,0x4B,0x0B,0x1B,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0xFE,0x00,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0xFF,0x00,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0xD0,0x6C,0x04,0x00,0x0A,0x00,0x00,0x01,0x00,0x00,0x4B,0x0B,0x1B,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x01,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x10,0x27,0x00,0x00,0x12,0x00,0x02,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x01,0x03,0x0D,0x00,0x00,0x0A,0x00,0x03,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x0E,0x00,0x04,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0xB0,0xAD,0x01,0x00,0x0A,0x00,0x05,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x06,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x20,0x4E,0x00,0x00,0x12,0x00,0x07,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x01,0x03,0x12,0x00,0x00,0x0A,0x00,0x08,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x0E,0x00,0x09,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0xB0,0xAD,0x01,0x00,0x0A,0x00,0x0A,0x01,0x00,0x00,0x4B,0x0B,0x1B,0x00,0x0
   5,0
   x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x0B,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x0C,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x60,0xEA,0x00,0x00,0x0A,0x00,0x0D,0x01,0x00,0x00,0x4B,0x0B,0x1B,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x0E,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x10,0x27,0x00,0x00,0x13,0x00,0x0F,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x2A,0x03,0x02,0x01,0x03,0x00,0x00,0x12,0x00,0x10,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x01,0x03,0x12,0x00,0x00,0x0A,0x00,0x11,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x0E,0x00,0x12,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0xA0,0x86,0x01,0x00,0x12,0x00,0x13,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x01,0x03,0x05,0x00,0x00,0x0A,0x00,0x14,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x0E,0x00,0x15,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x20,0xBF,0x02,0x00,0x0A,0x00,0x16,0x01,0x00,0x00,0x4B,0x0B,0x1B,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x17,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x18,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0xD0,0x6C,0x04,0x00,0x0A,0x00,0x19,0x01,0x00,0x00,0x4B,0x0B,0x1B,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x1A,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x10,0x27,0x00,0x00,0x12,0x00,0x1B,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x01,0x03,0x0D,0x00,0x00,0x0A,0x00,0x1C,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x0E,0x00,0x1D,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0xB0,0xAD,0x01,0x00,0x0A,0x00,0x1E,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x1F,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x20,0x4E,0x00,0x00,0x12,0x00,0x20,0
   x01
   ,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x01,0x03,0x12,0x00,0x00,0x0A,0x00,0x21,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x0E,0x00,0x22,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0xB0,0xAD,0x01,0x00,0x0A,0x00,0x23,0x01,0x00,0x00,0x4B,0x0B,0x1B,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x24,0x01,0x00,0x00,0x4B,0x0B,0x0D,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x25,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x60,0xEA,0x00,0x00,0x0A,0x00,0x26,0x01,0x00,0x00,0x4B,0x0B,0x1B,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x27,0x01,0x00,0x00,0x4B,0x0B,0x25,0x00,0x01,0x00,0x04,0x00,0x0C,0x00,0x10,0x27,0x00,0x00,0xD2,0x2D};
   static unsigned char aiResponseBuffer [ DIAG_MAX_PACKET_SIZE ];
   static short iRequestSize  = 1300;   // Size of request packet
   static short iResponseSize = 0;  // This will get updated by SendSync*/
   unsigned char bOk = FALSE;

   /*   unsigned int _iLoopCount = 0;

   // Data initialization loop

   QLIB_StartLogging( g_hResourceContext,"c:\\temp\\Diag_Stresstest_Log.txt");

   for (_iLoopCount = 0; _iLoopCount < 1000; _iLoopCount++ )
   {
   aiRequestBuffer[2]++;
   if( aiRequestBuffer[2] == 0xFF)
   {
   aiRequestBuffer[2] = 0x00;
   aiRequestBuffer[1]++;
   }

   printf("\nLoop count: %d", _iLoopCount );

   //////////////////////////////////////////////////////////
   // Send the command
   //////////////////////////////////////////////////////////
   iResponseSize = DIAG_MAX_PACKET_SIZE;    // # of bytes available for storage
   bOk = QLIB_SendSync( g_hResourceContext, iRequestSize, aiRequestBuffer, &iResponseSize, aiResponseBuffer,2000 );

   if (aiResponseBuffer[2] != aiRequestBuffer[2] )
   {
   QLIB_AddUserMessageToTextLog(g_hResourceContext, LOG_FN|LOG_IO, "   Mismatch found", true );
   } // if (aiResponseBuffer[2] != aiRequestBuffer[2] )

   if (kbhit()) break;
   }    // for (_iLoopCount = 0; _iLoopCount < 1000; _iLoopCount++ )

   if (kbhit()) getch();
   */
   return bOk;
}

void Test_Timer(void)
{
   unsigned long _hTimer3 = NULL;
   unsigned long _iElapsedTime = 0;
   unsigned int i=0;
   char _testfilename[50] = "c:\\temp\\test";
   char _tempbuf[100];
   unsigned int count=0;

   while(!kbhit())
   {
      _hTimer3 = QLIB_StartTimer( g_hResourceContext );

      //Sleep(2);
      // Get elapsed time
      _iElapsedTime = QLIB_GetTimerElapsed( g_hResourceContext, _hTimer3 );
      sprintf(_tempbuf,"%s.txt",_testfilename);
      QLIB_StartLogging( g_hResourceContext,(char *)_tempbuf);

      do
      {
         //do logging tasks
         Sleep (3);
      }
      while(((unsigned long) QLIB_GetTimerElapsed( g_hResourceContext, _hTimer3 )) < LOG_TIME);

      _iElapsedTime =(unsigned long) QLIB_GetTimerElapsed( g_hResourceContext, _hTimer3 );
      QLIB_StopLogging(g_hResourceContext);

      //check if the elapsed time has reached 6 seconds
      if(_iElapsedTime > LOG_TIME)
      {
         count++;
         sprintf(_tempbuf,"mv %s.txt %s%3.3d.txt",_testfilename,_testfilename,count);
         system(_tempbuf);
      }

      // Stop timer
      QLIB_StopTimer( g_hResourceContext, _hTimer3 );
   } //kbhit()

   getch();
}

unsigned char Test_Misc( void )
{
   unsigned char bOk=true;
   unsigned char cKey;
   char sVersionFile[] = "\\version.txt";
   char _sLogFile[] = "c:\\temp\\factory_log.txt";
   char _sDLF_LogFile[] = "c:\\temp\\factory_log.dlf";
   const int iVersionSize = 12;
   unsigned char iVersionInfo[iVersionSize] =  {'V','e','r','s','i','o','n',' ','T','e','s','t'};
   char sBMP_File[128]  = "/FactoryTest/Pictures/Contrast_TFT_Wolf5_155-2.bmp";

   printf("----------------\nProduction Test\n----------------\n");

   // if ( bOk )
   if ( true )  // Allow this menu to be displayed regardless of FTM pass, so that
      // the user can control the DTR lines
   {
      printf("\nB=Set DTR High");
      printf("\nC=Set DTR Low");
      printf("\nD=RTS ON" );
      printf("\nE=RTS OFF" );
      printf("\n   --------------------");;
      printf("\nG=Start Text Logging (%s)", _sLogFile);
      printf("\nH=Stop Text" );
      printf("\nI=List Extended Text Message Categories" );
      printf("\nJ=Enable all Extended Text Message Categories" );
      printf("\nK=Start DLF file %s", _sDLF_LogFile  );
      printf("\nL=Stop DLF");
      printf("\n   --------------------");;
      printf("\nM=Enable 'Serial Port Router'");
      printf("\nN=Diag stress test'");
      printf("\nT=general Timer Test");
      printf("\nO=Wait For QPST Server Event");

      cKey = toupper( getch() );
      printf("\n");

      if ( (cKey == 'b' ) || (cKey == 'B' ) )
      {
         // Set DTR High
         bOk = QLIB_SetDTR_State( g_hResourceContext, true );

         printf("\nSet DTR High: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == B
      else if ( (cKey == 'c' ) || (cKey == 'C' ) )
      {
         // Set DTR Low
         bOk = QLIB_SetDTR_State( g_hResourceContext, false );

         printf("\nSet DTR Low: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == C
      else if (cKey == 'D' )
      {
         // Set RTS High
         bOk = QLIB_SetRTS_State( g_hResourceContext, true );

         printf("\nSet RTS High: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == D
      else if (cKey == 'E' )
      {
         // Set RTS Low
         bOk = QLIB_SetRTS_State( g_hResourceContext, false );

         printf("\nSet RTS Low: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == E
      else if (cKey == 'G' )
      {
         unsigned short _iLogFlags = 0;

         printf("\n\n  Log Flags  in decimal value (0 for default, LOG_DEFAULT, 65535 for LOG_ALL): ");
         scanf("%hd", &_iLogFlags );

         // Set the log flag conditions
         QLIB_SetLogFlags( g_hResourceContext,  _iLogFlags == 0 ? LOG_DEFAULT : _iLogFlags);

         // Activate text logging
         bOk = QLIB_StartLogging( g_hResourceContext,  _sLogFile );

         printf("\nStart Logging: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == G
      else if (cKey == 'H' )
      {
         // Activate text logging
         bOk = QLIB_StopLogging( g_hResourceContext );

         printf("\nStop Logging: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == H
      else if (cKey == 'I' )
      {
         // printf("\nI=List Extended Text Message Categories" );

         // Locals
         unsigned short _iListSize;
         char _sCategoryName[ QMSL_EXTENDED_TEXT_MESSAGE_CATEGORY_NAME_SIZE ];
         unsigned char _bEnabled = false;

         // Get the list size
         QLIB_ExtendedTextMessage_GetCategoryListSize(
            g_hResourceContext,
            &_iListSize
         );

         // Iterate through the list, printing the category names
         printf("\nQMSL Extended Text Message List");
         printf("\n");
         printf("\nEnabled\tName");
         printf("\n-------\t-------------");
         for ( int _iListIndex = 0; _iListIndex < _iListSize; _iListIndex++ )
         {
            if ( QLIB_ExtendedTextMessage_GetCategoryListItem(
                     g_hResourceContext,
                     _iListIndex,
                     _sCategoryName,
                     &_bEnabled
                  )
               )
            {
               printf(  "\n%s\t%s",
                        _bEnabled ? "ON" : "OFF",
                        _sCategoryName
                     );
            }
            else
            {
               printf("\nQLIB_ExtendedTextMessage_GetCategoryListItem() -- error");
            }
         }  // for ( int _iListIndex = 0; _iListIndex <= _iListSize; _iListIndex++ )
      }// cKey == I
      else if (cKey == 'J' )
      {
         // printf("\nJ=Enable all Extended Text Message Categories" );

         // Locals
         unsigned short _iListSize;
         unsigned char _bEnabled = true;

         // Get the list size
         QLIB_ExtendedTextMessage_GetCategoryListSize(
            g_hResourceContext,
            &_iListSize
         );

         // Iterate through the list, printing the category names
         for ( int _iListIndex = 0; _iListIndex < _iListSize; _iListIndex++ )
         {
            bOk = QLIB_ExtendedTextMessage_SetCategoryEnable(
                     g_hResourceContext,
                     _iListIndex,
                     _bEnabled
                  );
            printf("\nQLIB_ExtendedTextMessage_SetCategoryEnable(): %s",
                   bOk ? "PASS" : "FAIL" );
         }  // for ( int _iListIndex = 0; _iListIndex <= _iListSize; _iListIndex++ )
      }// cKey == J
      else if (cKey == 'K' )
      {
         // printf("\nK=Start DLF file %s", _sDLF_LogFile  );

         bool _bAlwaysUsePC_Time = true;

         bOk = QLIB_StartDLF_Logging(
                  g_hResourceContext,
                  _sDLF_LogFile,
                  _bAlwaysUsePC_Time
               );

         printf("\nQLIB_StartDLF_Logging(): %s",
                bOk ? "PASS" : "FAIL" );

         // Also turn on phone logging
         bOk = QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
         printf("\nQLIB_DIAG_SetPhoneLoggingState = %s", bOk ? "PASS" : "FAIL" );
      }// cKey == K
      else if (cKey == 'L' )
      {
         // printf("\nL=Stop DLF");
         bOk = QLIB_StopDLF_Logging( g_hResourceContext );

         printf("\nQLIB_StopDLF_Logging(): %s",
                bOk ? "PASS" : "FAIL" );
      }// cKey == K

      else if (cKey == 'M' )
      {
         bOk =  SerialPortRouter();
         // Enable 'Serial Port Router'"
      }// cKey == M

      else if (cKey == 'N' )
      {
         bOk =  DiagStressTest();
         // Diagnostic stress test
      }// cKey == O
      else if (cKey == 'O' )
      {
         unsigned long _iTriggerTime = ::GetTickCount();
         // The event will be trigger if someone remove the phone from USB cable
         bOk =  ::QLIB_QPST_DefineEventTrigger(g_hResourceContext, QPST_serverEventUpdatePhoneStatus);
         printf(" Disconnect the phone from USB port now\n");

         unsigned long iWaitStatus = 0;
         bOk = ::QLIB_QPST_WaitForEventTrigger(g_hResourceContext, 20000, &iWaitStatus);
         _iTriggerTime = ::GetTickCount() - _iTriggerTime;

         printf("Trigger Time %d ms, Wait Status=%d\n", _iTriggerTime, iWaitStatus);
      }// cKey == N
      else if (cKey == 'T')
      {
         Test_Timer();
      }
   }

   else
   {
      printf("\nFTM Mode Failed\n");
   }    // if ( bOk )

   return bOk;
}


unsigned char Test_FTM_DetectCommand( void )
{
   unsigned char bOk = 0x0;
   short commandType = -1;
   bOk = QLIB_FTM_DETECT_COMMAND_CODE( g_hResourceContext, &commandType );
   printf("QLIB_FTM_DECTECT_COMMAND :%s Command Type %d",  bOk ? "PASS" : "FAIL\n", commandType);;

   return 0;
}

unsigned char Test_Download( void )
{
   unsigned char bOk=true;
   unsigned char cKey;
   char sHexFile[] = "C:\\MMSZ_scramped.hex";
   char sQcnFile[] = "C:\\000.QCN";
   char sBootFile[] = "C:\\BOOT_LOADER.hex";
   char sCEFS_File[] = "C:\\CEFS.mbn";

   char _sPartitionFile[] = "C:\\partition.mbn";

   char _sOTBPL_FileName[] = "C:\\obl.mbn";
   char _sPBL_FileName[] = "C:\\pbl.mbn";
   char _sQCSBL_FileName[] = "C:\\qcsbl.mbn";
   char _sQCSBL_HeaderName[] = "C:\\qcsblhd_cfgdata.mbn";
   char _sOEMSBL_FileName[] = "C:\\oemsbl.mbn";
   char _sOEMSBL_HeaderName[] = "C:\\oemsblhd.mbn";
   char _sModemFileName[] = "C:\\amss.mbn";
   char _sModemHeaderName[] = "C:\\amsshd.mbn";
   char _sWM_AppsImageName[] = "C:\\flash.bin";
   char _sStorageFile[] = "";   // Leave this empty to no do auto backup/restore.  Recommend doing this
   // and having a separate backup/restore procedure to be used before and after
   // the multiimage download
   char* _sAppsFileName = "C:\\apps.mbn";
   char* _sAppsHeaderName = "C:\\appshd.mbn";
   char* _sAppsBLFileName = "C:\\appsboot.mbn";
   char* _sAppsBLHeaderName = "C:\\appsboothd.mbn";

   char* _sDeviceBootLoader_FileName = "c:\\dbl.mbn";
   char* _sQFailSafeBOotLoader_FileName = "c:\\fsbl.mbn";
   char* _sOSBootLoader_FileName = "c:\\osbl.mbn";
   char* _sDSP1FileName = "c:\\dsp1.mbn";
   char* _sDSP2FileName = "c:\\dsp2.mbn";
   char* _cefsModem_FileName1 = "C:\\cefs_modem.mbn";

   // Timeout variable, to be calculated based on which operation is being done, default 10 minutes
   int _iTimeOut = 10 * 60 * 1000;

   printf("----------------\nDownload Test\n----------------\n");

   if ( bOk )
   {
      printf("\n\n1=Download QCN FILE from Phone to PC (%s)", sQcnFile);
      printf("\n2=Upload HEX File from PC to Phone\t(%s)", sHexFile );
      printf("\n3=Upload QCN file from PC to Phone(%s)", sQcnFile );
      printf("\n4=Upload Boot file from PC to Phone(%s)", sBootFile );
      printf("\n5=Upload CEFS file from PC to Phone(%s) using partition file (%s)", sCEFS_File, _sPartitionFile );
      printf("\n6=Upload Multi-image OBL from PC to Phone using files in C:\\" );
      printf("\n7=Upgrade from single image to multi-image, files in C:\\" );
      printf("\n8=Upload Multi-image OBL from PC to Phone using files in C:\\, until key is pressed or error occurs." );
      printf("\n9=Upload CEFS file %s using partition %s and user-defined ARMPRG file, from PC to Phone", sCEFS_File, _sPartitionFile );
      printf("\nA=Download QCN FILE from Phone to PC (%s) until key is pressed", sQcnFile);
      printf("\nB=Upload Multi-image (no OTP boot loader, multi-image 1.0) from PC to Phone using files in C:\\" );
      printf("\nC=Upload Multi-image (Multi-image 1.0)for Windows Mobile from PC to Phone using files C:\\%s", _sWM_AppsImageName );
      printf("\nD=Use Emergency Download, Download OEM/QCOM Boot loader"  );
      printf("\nE=Upload Multi-image(Secure Boot 2.0) from PC to Phone using files c:\\"  );
      printf("\nF=QPHONEMS SW download"  );
      printf("\nG=Upload image via Sahara + Firehose from PC to Phone using files C:\\images\n");
      cKey = toupper(getch());

      if (cKey == '1' )
      {
         printf("\nDownloading QCN file: %s from phone to PC...\n", sQcnFile );

         // Download a QCN file from the phone to the PC
         bOk = QLIB_DownloadQcnFile( g_hResourceContext, sQcnFile,"000000");
         printf("\nQLIB_DownloadQcnFile: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == 1
      else if (cKey == '2')
      {
         printf("\nUploading HEX file: %s from PC to phone...\n", sHexFile );

         // Upload a HEX file from PC to phone
         QLIB_UploadHexFile
         (
            g_hResourceContext,
            sHexFile,   // sFileName,
            "000000",   // sSPC
            true,       // bClearErrorLog
            true,       // bOverrideModelCheck
            true,       // bSkipReset
            false,      // bUseCustomArmProg
            true,       // bDownloadArmProg
            false,      // bDoAutoRestoreBackup
            ""          // sAutoRestoreBackupName
         );
      } // cKey == 2
      else if (cKey == '3' )
      {
         printf("\nUploading QCN file: %s from PC to phone...\n", sQcnFile );
         // Upload a QCN file from the PC to the phone
         bOk = QLIB_UploadQcnFile(
                  g_hResourceContext,
                  sQcnFile,           // sFileName
                  "000000",           // sSPC
                  true,               // bAutoRestore
                  false,              // bSkipReset
                  false,              // bAllowEsnMismatch
                  true                // bIgnorePhoneModel
               );
         printf("\nQLIB_UploadQcnFile: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == 3
      else if (cKey == '4' )
      {
         printf("\nUploading BOOT file: %s from PC to phone...\n", sBootFile );
         QLIB_UploadBootLoaderOnlyHexFile( g_hResourceContext, sBootFile, "000000", true, true );
      }// cKey == 4
      else if (cKey == '5' )
      {
         printf("\nUploading CEFS file: %s from PC to phone, using %s partition file...\n", sCEFS_File, _sPartitionFile );
         QLIB_UploadCEFS_File( g_hResourceContext, sCEFS_File, _sPartitionFile );
      }// cKey == 5
      else if ( (cKey == '6' )
                || (cKey == '8' )
              )
      {
         // This is the bit mask of files that will be enabled...
         long _iDownloadBitMask = (
                                     // + miType_None
                                     miTypePrtnFile
                                     + miTypePblFile
                                     + miTypeQcSblFile
                                     + miTypeQcSblHdFile
                                     + miTypeOemSblFile
                                     + miTypeOemSblHdFile
                                     + miTypeAmssFile
                                     + miTypeAmssHdFile
                                     // + miTypeAppsFile
                                     // + miTypeAppsHdFile
                                     + miTypeOblFile
                                     // + miTypeAppsBlFile
                                     // + miTypeAppsBlHdFile
                                     // + miType_All
                                     // + miType_AllExApps

                                  );

         printf("\nUploading Multimage: \n" );

         int _iCount = 0;

         // 10 minutes * 60 seconds * 1000milliseconds/seconds
         _iTimeOut = 10 * 60 * 1000;

         // Set timeout, debug, set very low to force error,
         bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_SoftwareDownloadActivity, _iTimeOut  );
         printf("\nQLIB_ConfigureTimeOut(%d): %s", _iTimeOut, bOk ? "PASS" : "FAIL" );

         do
         {
            printf("\n\n  Download Counter = %d", ++_iCount );

            // Call to the multi image downloader
            bOk = QLIB_UploadMultiImage
                  (
                     g_hResourceContext,
                     _sPartitionFile,
                     _sOTBPL_FileName,
                     _sPBL_FileName,
                     _sQCSBL_FileName,
                     _sQCSBL_HeaderName,
                     _sOEMSBL_FileName,
                     _sOEMSBL_HeaderName,
                     _sModemFileName,
                     _sModemHeaderName,
                     _sAppsFileName,
                     _sAppsHeaderName,
                     _sAppsBLFileName,
                     _sAppsBLHeaderName,
                     true,        // unsigned char bOverridePrtnTable,
                     false,       // unsigned char bUseTrustedMode,
                     "000000",    // char* sSPC,
                     true,        // unsigned char bClearErrorLog,
                     _sStorageFile,       // char* sStorageFile,
                     true,        // unsigned char bOverrideModelCheck,
                     false,       // unsigned char bSkipReset,
                     false,       // unsigned char bUseCustomArmprg,
                     0,           // long iDloadArmprg
                     _iDownloadBitMask
                  );
         }
         while (  (cKey == '8' )
                  &&  ( bOk )
                  &&  ( !kbhit() )
               );

         // Clear PC keypress queue
         if ( kbhit() )
            getch();
      }// cKey == 6
      else if (cKey == '7' )
      {
         // printf("\n7=Upgrade from single image to multi-image, files in C:\\" );

         // First put the phone into download mode
         bOk = QLIB_DIAG_DLOAD_F( g_hResourceContext  );
         printf("\nQLIB_DIAG_DLOAD_F( ): %s", bOk ? "PASS" : "FAIL\n" );

         // Need to take a moment here to allow QPST time to update the Mobile's mode
         // If the download is issued to quickly, then QPST will think the mobile is not
         // in download mode
         printf("\n\n<<Make sure phone is in download mode on the QPST screen, then press any key>>");
         getch();

         // This is the bit mask of files that will be enabled...for some reason & is not working right
         long _iDownloadBitMask = (
                                     // + miType_None
                                     miTypePrtnFile
                                     + miTypePblFile
                                     + miTypeQcSblFile
                                     + miTypeQcSblHdFile
                                     + miTypeOemSblFile
                                     + miTypeOemSblHdFile
                                     + miTypeAmssFile
                                     + miTypeAmssHdFile
                                     // + miTypeAppsFile
                                     // + miTypeAppsHdFile
                                     + miTypeOblFile
                                     // + miTypeAppsBlFile
                                     // + miTypeAppsBlHdFile
                                     // + miType_All
                                     // + miType_AllExApps

                                  );

         printf("\nUploading Multimage: \n" );

         // Call to the multi image downloader
         QLIB_UploadMultiImage
         (
            g_hResourceContext,
            _sPartitionFile,
            _sOTBPL_FileName,
            _sPBL_FileName,
            _sQCSBL_FileName,
            _sQCSBL_HeaderName,
            _sOEMSBL_FileName,
            _sOEMSBL_HeaderName,
            _sModemFileName,
            _sModemHeaderName,
            _sAppsFileName,
            _sAppsHeaderName,
            _sAppsBLFileName,
            _sAppsBLHeaderName,
            true,       // unsigned char bOverridePrtnTable,
            false,      // unsigned char bUseTrustedMode,
            "000000",   // char* sSPC,
            true,       // unsigned char bClearErrorLog,
            _sStorageFile,      // char* sStorageFile,
            true,       // unsigned char bOverrideModelCheck,
            false,      // unsigned char bSkipReset,
            1,          // unsigned char bUseCustomArmprg = true
            apMSM6250Sec,   // long iDloadArmprg
            _iDownloadBitMask
         );
      }// cKey == 7
      else if (cKey == '9' )
      {
         // printf("\nA=Upload CEFS file %s using partition %s and user-defined ARMPRG file, from PC to Phone", sCEFS_File, _sPartitionFile );

         long _iDloadArmprg = 0;

         printf("\n ARMPRG ID's are in QPST Server code SharedInc\\ArmprgType.h or in QMSL QLib.h->QLIB_UploadCEFS_File2().  19=6250 Secure ");
         printf("\n Please enter ARMPROG ID: ");
         scanf( "%d", &_iDloadArmprg );

         if ( _iDloadArmprg >= 0 )
         {
            printf("\nUploading CEFS file: %s from PC to Phone, using partition: %s and armprog: %d\n", sCEFS_File, _sPartitionFile, _iDloadArmprg );
            QLIB_UploadCEFS_File2( g_hResourceContext, sCEFS_File, _sPartitionFile, _iDloadArmprg  );
         }
         else
         {
            printf("Invalid ARMPRG");
         }
      }// cKey == 9
      else if (cKey == 'A' )
      {
         int _iCount = 0;
         do
         {
            printf("\n\n  Download Counter = %d", ++_iCount );

            // printf("\nA=Download QCN FILE from Phone to PC (C:\\%s) until key is pressed", sQcnFile);
            printf("\nDownloading QCN file: %s from phone to PC...\n", sQcnFile );

            // Download a QCN file from the phone to the PC
            bOk = QLIB_DownloadQcnFile( g_hResourceContext, sQcnFile,"000000");
            printf("\nQLIB_DownloadQcnFile: %s", bOk ? "PASS" : "FAIL" );
         }
         while (
            ( bOk )
            &&  ( !kbhit() )
         );

         // Clear PC keypress queue
         if ( kbhit() )
            getch();
      }// cKey == 1
      else if (cKey == 'B' )
      {
         // This is the bit mask of files that will be enabled...
         long _iDownloadBitMask = (
                                     // + miType_None
                                     miTypePrtnFile
                                     + miTypePblFile
                                     + miTypeQcSblFile
                                     + miTypeQcSblHdFile
                                     + miTypeOemSblFile
                                     + miTypeOemSblHdFile
                                     + miTypeAmssFile
                                     + miTypeAmssHdFile
                                     // + miTypeAppsFile
                                     // + miTypeAppsHdFile
                                     // + miTypeOblFile
                                     // + miTypeAppsBlFile
                                     // + miTypeAppsBlHdFile
                                     // + miType_All
                                     // + miType_AllExApps

                                  );

         printf("\nUploading Multimage: \n" );

         int _iCount = 0;

         // 10 minutes * 60 seconds * 1000milliseconds/seconds
         _iTimeOut = 10 * 60 * 1000;

         // Set timeout, debug, set very low to force error,
         bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_SoftwareDownloadActivity, _iTimeOut  );
         printf("\nQLIB_ConfigureTimeOut(%d): %s", _iTimeOut, bOk ? "PASS" : "FAIL" );

         // Call to the multi image downloader
         bOk = QLIB_UploadMultiImage2
               (
                  g_hResourceContext,
                  _sPartitionFile,
                  _sPBL_FileName,
                  _sQCSBL_FileName,
                  _sQCSBL_HeaderName,
                  _sOEMSBL_FileName,
                  _sOEMSBL_HeaderName,
                  _sModemFileName,
                  _sModemHeaderName,
                  _sAppsFileName,
                  _sAppsHeaderName,
                  _sAppsBLFileName,
                  _sAppsBLHeaderName,
                  true,       // unsigned char bOverridePrtnTable,
                  false,      // unsigned char bUseTrustedMode,
                  "000000",   // char* sSPC,
                  false,      // unsigned char bClearErrorLog,
                  _sStorageFile,      // char* sStorageFile,
                  true,       // unsigned char bOverrideModelCheck,
                  false,      // unsigned char bSkipReset,
                  false,      // unsigned char bUseCustomArmprg,
                  0,          // long iDloadArmprg
                  _iDownloadBitMask
               );
      }
      // printf("\nB=Upload Multi-image (no OTP boot loader) from PC to Phone using files in C:\\" );
      else if (cKey == 'C' )
      {
         //printf("\nC=Upload Multi-image for Windows Mobile from PC to Phone using files C:\\%s", _sWM_AppsImageName );

         // This is the bit mask of files that will be enabled...
         long _iDownloadBitMask = (
                                     // + miType_None
                                     miTypePrtnFile
                                     // miTypePblFile
                                     + miTypeQcSblFile
                                     + miTypeQcSblHdFile
                                     + miTypeOemSblFile
                                     + miTypeOemSblHdFile
                                     + miTypeAmssFile
                                     + miTypeAmssHdFile
                                     // + miTypeAppsFile
                                     // + miTypeAppsHdFile
                                     // + miTypeOblFile
                                     + miTypeAppsBlFile
                                     + miTypeAppsBlHdFile
                                     + miTypeWinMobile
                                     // + miType_All
                                     // + miType_AllExApps

                                  );

         //
         char _sAppsBLFileName[] = "c:\\appsboot.mbn";
         char _sAppsBLHeaderName[] = "c:\\appsboothd.mbn";
         char _sPBL_FileName[] = "";

         printf("\nUploading WM Multimage: \n" );

         int _iCount = 0;

         // 10 minutes * 60 seconds * 1000milliseconds/seconds
         _iTimeOut = 10 * 60 * 1000;

         // Set timeout, debug, set very low to force error,
         bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_SoftwareDownloadActivity, _iTimeOut  );
         printf("\nQLIB_ConfigureTimeOut(%d): %s", _iTimeOut, bOk ? "PASS" : "FAIL" );

         // Use this command to place the phone into download mode before sending the command to download new binaries
         if ( true )
         {
            // First put the phone into download mode--this is necessary if the build of SW does not transition well and requires a reset before
            // successfully going into download mode
            bOk = QLIB_DIAG_DLOAD_F( g_hResourceContext  );
            printf("\nQLIB_DIAG_DLOAD_F( ): %s", bOk ? "PASS" : "FAIL\n" );
         }

         // Need to take a moment here to allow QPST time to update the Mobile's mode
         // If the download is issued to quickly, then QPST will think the mobile is not
         // in download mode
         printf("\n\n<<Make sure phone is in download mode on the QPST screen, then 'q' to quit or press any key>>");
         char _cKey = toupper(getch());

         /*
         If the phone was in Windows Mobile mode, then the COM port will be switched.
         To handle this, disconnect the COM port and try another connection with auto detection
         */
         QLIB_DisconnectServer( g_hResourceContext );

         g_hResourceContext  = QLIB_ConnectServer( QLIB_COM_AUTO_DETECT );

         if ( g_hResourceContext
               &&  (_cKey != 'Q' )
            )
         {
            // Configure call back functions
            QLIB_ConfigureCallBacks( g_hResourceContext, NULL, generalDownloadEventCallBack, asyncMessageCallBack  );

            // This is necessary if phone is put into download mode before the following function is called.  When
            // the phone is in download mode, QPST will not be able to figure out the phone info automatically
            char bUseCustomArmprg = TRUE;
            long iDloadArmprg = apMSM7200a;

            // Call to the multi image downloader
            bOk = QLIB_UploadMultiImage3
                  (
                     g_hResourceContext,
                     _sPartitionFile,
                     _sPBL_FileName,
                     _sQCSBL_FileName,
                     _sQCSBL_HeaderName,
                     _sOEMSBL_FileName,
                     _sOEMSBL_HeaderName,
                     _sModemFileName,
                     _sModemHeaderName,
                     "",
                     "",
                     _sAppsBLFileName,
                     _sAppsBLHeaderName,
                     _sWM_AppsImageName,
                     false,       // unsigned char bOverridePrtnTable,
                     false,       // unsigned char bUseTrustedMode,
                     "000000",    // char* sSPC,
                     false,       // unsigned char bClearErrorLog,
                     _sStorageFile,       // char* sStorageFile,
                     true,        // unsigned char bOverrideModelCheck,
                     false,       // unsigned char bSkipReset--this parameter is disabled inside QPST
                     bUseCustomArmprg,        // unsigned char bUseCustomArmprg,
                     iDloadArmprg,            // long iDloadArmprg
                     _iDownloadBitMask
                  );

            /**
            At this point, WM changes COM ports and QPST will not find it on the new COM port
            The only way around this is to let the phone time out.  There is a timeout value
            that can be tuned:
            QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_SoftwareDownloadActivity, _iTimeOut  );

            The check for the final status, it is possible to use the messages coming back through
            the callback function, to confirm the download is completed.
            */
         }  // if ( _cKey != 'Q' )
      }
      else if (cKey == 'D' )
      {
         //printf("\nC=Upload Multi-image for Windows Mobile from PC to Phone using files C:\\%s", _sWM_AppsImageName );

         // This is the bit mask of files that will be enabled...
         long _iDownloadBitMask = (
                                     // + miType_None
                                     miTypePrtnFile
                                     // miTypePblFile
                                     + miTypeQcSblFile
                                     + miTypeQcSblHdFile
                                     + miTypeOemSblFile
                                     + miTypeOemSblHdFile
                                     //+ miTypeAmssFile
                                     //+ miTypeAmssHdFile
                                     // + miTypeAppsFile
                                     // + miTypeAppsHdFile
                                     // + miTypeOblFile
                                     //+ miTypeAppsBlFile
                                     //+ miTypeAppsBlHdFile
                                     //+ miTypeWinMobile
                                     // + miType_All
                                     // + miType_AllExApps

                                  );

         //
         char _sAppsBLFileName[] = "UNKNOWN";
         char _sAppsBLHeaderName[] = "UNKNOWN";
         char _sPBL_FileName[] = "UNKNOWN";

         printf("\nUploading WM Multimage: \n" );

         int _iCount = 0;

         // 10 minutes * 60 seconds * 1000milliseconds/seconds
         _iTimeOut = 10 * 60 * 1000;

         // Set timeout, debug, set very low to force error,
         //bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_SoftwareDownloadActivity, _iTimeOut  );
         //printf("\nQLIB_ConfigureTimeOut(%d): %s", _iTimeOut, bOk ? "PASS" : "FAIL" );

         // Use this command to place the phone into download mode before sending the command to download new binaries
         /*
         if ( true )
         {
         // First put the phone into download mode--this is necessary if the build of SW does not transition well and requires a reset before
         // successfully going into download mode
         bOk = QLIB_DIAG_DLOAD_F( g_hResourceContext  );
         printf("\nQLIB_DIAG_DLOAD_F( ): %s", bOk ? "PASS" : "FAIL\n" );
         }
         */
         // Need to take a moment here to allow QPST time to update the Mobile's mode
         // If the download is issued to quickly, then QPST will think the mobile is not
         // in download mode
         printf("\n\n<<Make sure phone is in download mode on the QPST screen, then 'q' to quit or press any key>>");
         char _cKey = toupper(getch());

         /*
         If the phone was in Windows Mobile mode, then the COM port will be switched.
         To handle this, disconnect the COM port and try another connection with auto detection
         */
         QLIB_DisconnectServer( g_hResourceContext );

         g_hResourceContext  = QLIB_ConnectServer( 1 );

         if ( g_hResourceContext
               &&  (_cKey != 'Q' )
            )
         {
            // Configure call back functions
            QLIB_ConfigureCallBacks( g_hResourceContext, NULL, generalDownloadEventCallBack, asyncMessageCallBack  );

            // This is necessary if phone is put into download mode before the following function is called.  When
            // the phone is in download mode, QPST will not be able to figure out the phone info automatically
            char bUseCustomArmprg = TRUE;
            long iDloadArmprg = apMSM7600;

            // Call to the multi image downloader
            bOk = QLIB_UploadMultiImage5
                  (
                     g_hResourceContext,
                     _sPartitionFile,
                     _sPBL_FileName,
                     _sQCSBL_FileName,
                     _sQCSBL_HeaderName,
                     _sOEMSBL_FileName,
                     _sOEMSBL_HeaderName,
                     "",
                     "",
                     "",
                     "",
                     "",
                     "",
                     "",

                     "",          // DSP 1 Name
                     "",          // DSP 2 Name
                     true,       // bool  bUseEmergDL,
                     "",          // MBR File Namechar* sMBRFileName,

                     false,       // unsigned char bOverridePrtnTable,
                     true,        // unsigned char bUseTrustedMode,
                     "000000",    // char* sSPC,
                     false,       // unsigned char bClearErrorLog,
                     "",      // char* sStorageFile, no backup
                     true,        // unsigned char bOverrideModelCheck,
                     false,       // unsigned char bSkipReset--this parameter is disabled inside QPST
                     true,        // unsigned char bUseCustomArmprg,
                     apMSM7600,           // long iDloadArmprg
                     _iDownloadBitMask
                  );
         }  // if ( _cKey != 'Q' )
      }
      else if (cKey == 'E' )
      {
         printf("\nSecure Boot 2.0 Multi-image Download:\n");

         // 10 minutes * 60 seconds * 1000milliseconds/seconds
         _iTimeOut = 10 * 60 * 1000;

         // Set timeout, debug, set very low to force error,
         bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_SoftwareDownloadActivity, _iTimeOut  );
         printf("\nQLIB_ConfigureTimeOut(%d): %s", _iTimeOut, bOk ? "PASS" : "FAIL" );

         // The target is a QSC6240/6270 FFA
         long iDloadArmprg = apMSMQSC6270x40NAND;

         bOk = QLIB_UploadSB2MultiImage(
                  g_hResourceContext,
                  _sPartitionFile,
                  _sDeviceBootLoader_FileName,
                  _sQFailSafeBOotLoader_FileName,
                  _sOSBootLoader_FileName,
                  _sModemFileName,
                  "",         //sApps_FileName,
                  "",         //sAppsBootLoader_FileName,
                  "",         //sWinMobile_FileName,
                  "",         //sDSP1FileName,
                  "",         //sDSP2FileName,
                  "",         //sMBR_FileName,
                  "",         //sADSP_FileName,
                  "",         //sTrustZone,
                  "",         //sROFS1,
                  "",         //sROFS2,
                  "",         //sROFS3,
                  0,          //bUseEmergDL,
                  1,          //bOverridePrtnTable,
                  1,          // bUseTrustedMode,
                  "000000",   // SPC
                  1,          //bClearErrorLog,
                  "",         //sStorageFils,
                  1,          //bOverrideModelCheck,
                  0,          // bSkipReset--this parameter is disabled inside QPST
                  1,          //bUseCustomArmprg,
                  iDloadArmprg);
      }
      else if (cKey == 'F' )
      {
         unsigned long iErrorCode = 0;

         // Do not enable LOG_IO, otherwise a 100MB Log file will be created
         QLIB_SetLogFlags(g_hResourceContext, LOG_INF | LOG_C_HIGH_LEVEL_START | LOG_FN | LOG_IO);
         QLIB_ExtendedTextMessage_SetCategoryEnable(g_hResourceContext, QMSL_ExtTextMsgCat_QPHONEMS_SW_Download, 1);
         QLIB_QPHONEMS_UploadSBMultiImage_ConfigureCallBack(g_hResourceContext, swdlCallBack);
         unsigned char bOverridePrtnTable = 1;
         unsigned char bUseTrustedMode = 1;
         unsigned char bSkipGoToDownload = 0;
         unsigned long iSleepTimeToDOWNLOOADmode = 5000;
         unsigned long iSleepTimeToGOcommand = 3000;

         unsigned long _iTimeSpan = 0 ;

         printf("\n Choose your target:");
         printf("\n 1 == SCMM(QSC6x95)");
         printf("\n 2 == MDM9K");
         printf("\n 3 == MSM7x25");
         printf("\n 4 == LCU (QSC6240/70)");
         printf("\n 5 == 9x15 (DMMS/Streaming)");
         printf("\n 6 == 9x25 (Software Download: Sahara/Streaming)");
         printf("\n 7 == 9x25 (Software Upgrade: Sahara/Streaming)");
         printf("\n 8 == 8x30 (Software Download: DMSS/Streaming)");
         //printf("\n Q == Quit");
         printf("\n --> ");
         cKey = getch();
         if(cKey == '1')
            printf("SCMM(QSC6x95)\n");
         else if(cKey == '2')
            printf("MDM9K\n");
         else if(cKey == '3')
            printf("MSM7x25");
         else if(cKey == '4')
            printf("LCU (QSC6240/70)");
         else if (cKey == '5')
            printf("9x15 (DMSS/Streaming)");
         else if (cKey == '6')
            printf("9x25 (Software Download: Sahara/Streaming)");
         else if (cKey == '7')
            printf("9x25 (Software Upgrade: Sahara/Streaming)");

         QLIB_StartLogging(g_hResourceContext, "c:\\QLIB_QPHONEMS_UploadSBMultiImage.txt");
         _iTimeSpan = ::GetTickCount();

         if (cKey != '6' && cKey != '7' && cKey != '8')
         {
            printf("Skip Go to Download ? 1 or 0\n");
            int goTo = 0;
            scanf("%d", &goTo);
            bSkipGoToDownload = goTo;
         }

         if (cKey == '8')
         {
             //release existing connection if exists
             if ( g_hResourceContext )
             {
                 QLIB_StopLogging(g_hResourceContext);
                 QLIB_DisconnectServer( g_hResourceContext );
                 g_hResourceContext = NULL;
                 printf("\nQLIB_DisconnectServer() was called to release the current handle");
                 printf("\n\n<<Make sure that QPST has completely closed");
             } // if ( g_hResourceContext )

             //These methods run in QPHONEMS mode
             QLIB_SetLibraryMode( QLIB_LIB_MODE_QPHONEMS );

             unsigned short _iComPort = 0;
             char* _sSCMM_ARMPRG_FileName = "C:\\images\\8x30\\MPRG8930.hex";
             char* _sSCMM_Partition_FileName = "C:\\images\\8x30\\8930_msimage.mbn";
             char* _sCDT_Rawprogram = "C:\\images\\8x30\\rawprogram2.xml";
             char* _sCDT_Patch = "C:\\images\\8x30\\patch2.xml";

             printf("\n\nPlease confirm the image information before downloading:\n");
             printf("ARM Programer: %s\n", _sSCMM_ARMPRG_FileName);
             printf("Partition File: %s\n", _sSCMM_Partition_FileName);

             // Get the COM port
             printf("\nPlease enter the COM port # (QDLoader for download): ");
             scanf("%hd", &_iComPort);

             //Software upgrade requires a switch to download mode before Sahara
             g_hResourceContext  = QLIB_ConnectServer ( _iComPort );
             if ( ! g_hResourceContext )
             {
                 printf("QLIB_ConnectServer failed\n");
                 return false;
             }

             // Configure call back functions
             QLIB_QPHONEMS_UploadSBMultiImage_ConfigureCallBack(g_hResourceContext, swdlCallBack);

             //Enable logging
             QLIB_SetLogFlags( g_hResourceContext, LOG_ALL );
             QLIB_ExtendedTextMessage_SetCategoryEnable(g_hResourceContext, QMSL_ExtTextMsgCat_QPHONEMS_SW_Download, 1);

             printf("\n----------------QLIB_QPHONEMS_UploadEmmcUserParitionImageWithCDT-----------------------\n");
             bOk = QLIB_QPHONEMS_UploadEmmcUserParitionImageWithCDT (   g_hResourceContext,
                                                                        _sSCMM_ARMPRG_FileName,
                                                                        _sCDT_Rawprogram,
                                                                        _sCDT_Patch,
                                                                        NULL,//_sSCMM_Partition_FileName,
                                                                        0, // Not use Truseted Mode
                                                                        5,
                                                                        5,
                                                                        0,
                                                                        30000,
                                                                        0,
                                                                        &iErrorCode);

             if (!bOk)
             {
                 printf("QLIB_QPHONEMS_UploadEmmcUserParitionImage failed\n");
                 return false;
             }

             printf("\n----------------QLIB_DisconnectServer-----------------------\n");
             QLIB_DisconnectServer( g_hResourceContext );
             g_hResourceContext = NULL;

             printf("\nResult: %s", bOk ? "PASS" : "FAIL");
             printf(", ErrorCode=%d\n", iErrorCode);
         }
         else if (cKey == '6' || cKey == '7')
         {
            //release existing connection if exists
            if ( g_hResourceContext )
            {
               QLIB_StopLogging(g_hResourceContext);
               QLIB_DisconnectServer( g_hResourceContext );
               g_hResourceContext = NULL;
               printf("\nQLIB_DisconnectServer() was called to release the current handle");
               printf("\n\n<<Make sure that QPST has completely closed");
            } // if ( g_hResourceContext )

            //These methods run in QPHONEMS mode
            QLIB_SetLibraryMode( QLIB_LIB_MODE_QPHONEMS );

            char* _sSCMM_ARMPRG_FileName;
            char* _sSCMM_Partition_FileName = "C:\\images\\9x25_flatbuild\\partition.mbn";

            HANDLE phoneHandle = NULL;
            unsigned long version = 0;
            unsigned long sn = 0;
            unsigned long msmid = 0;
            char pkHash[200];
            unsigned long timeout = 200;
            unsigned char bGetInfo = 1;
            float imageSizeInMB = 0.0;
            float throughput = 0.0;
            unsigned short _iComPort = 0;
            int mode = 1;

            if (cKey == '6')   //case software download
            {
               mode = 0;//indicate phone is blank phone in emergency download
               _sSCMM_ARMPRG_FileName = "C:\\images\\9x25_flatbuild\\ENPRG9x25.mbn";

               //if in emergency download, phone must be freshly started before continuing
               printf("\nPower cycle the phone and connect it at this time");
               printf("\nSahara must be the first software to connect to com port after the phone powers on to get Sahara HELLO");
            }
            else if (cKey == '7')     //case software upgrade
            {
               mode = 2; //indicate phone already has images and is a software upgrade
               _sSCMM_ARMPRG_FileName = "C:\\images\\9x25_flatbuild\\NPRG9x25.mbn";
            }

            printf("\n\nPlease confirm the image information before downloading:\n");
            printf("ARM Programer: %s\n", _sSCMM_ARMPRG_FileName);
            printf("Partition File: %s\n", _sSCMM_Partition_FileName);

            // Get the COM port
            printf("\nPlease enter the COM port # (QDLoader for download) or (Diagnostics for upgrade): ");
            scanf("%hd", &_iComPort);

            if (cKey == '7')   //case software upgrade
            {
               //Software upgrade requires a switch to download mode before Sahara
               g_hResourceContext  = QLIB_ConnectServer ( _iComPort );
               if ( ! g_hResourceContext )
               {
                  printf("QLIB_ConnectServer failed\n");
                  return false;
               }
               // Configure call back functions
               QLIB_QPHONEMS_UploadSBMultiImage_ConfigureCallBack(g_hResourceContext, swdlCallBack);

               //Enable logging
               QLIB_SetLogFlags( g_hResourceContext, LOG_ALL );
               QLIB_ExtendedTextMessage_SetCategoryEnable(g_hResourceContext, QMSL_ExtTextMsgCat_QPHONEMS_SW_Download, 1);

               printf("\n----------------QLIB_QPHONEMS_SwitchToDownloadMode_OptionalValidation-----------------------\n");
               bOk = QLIB_QPHONEMS_SwitchToDownloadMode_OptionalValidation  (  g_hResourceContext,
                                                                               1000,  false, &iErrorCode )  ;

               printf("\n----------------QLIB_DisconnectServer-----------------------\n");
               QLIB_DisconnectServer( g_hResourceContext );
               g_hResourceContext = NULL;
               if (!bOk)
               {
                  printf("QLIB_QPHONEMS_SwitchToDownloadMode_OptionalValidation failed\n");
                  return false;
               }

               //Time for the device to switch to Sahara mode
               Sleep (5000);
            }

            printf("\n----------------QLIB_QPHONEMS_ConnectServer_Sahara-----------------------\n");
            phoneHandle = QLIB_QPHONEMS_ConnectServer_Sahara(_iComPort, &version, &sn, &msmid, pkHash, bGetInfo, mode, timeout, swdlCallBack);
            QLIB_StartLogging(phoneHandle, "c:\\Qualcomm\\QLIB_QPHONEMS_Sahara.txt");

            if (phoneHandle == 0)
            {
               printf("\nQLIB_QPHONEMS_ConnectServer_Sahara failed\n");
               return false;
            }
            printf("Version: %d, MSMId: %d", version, msmid);

            printf("\n----------------QLIB_QPHONEMS_Sahara_FlashProgrammer---------------------\n");
            bOk = QLIB_QPHONEMS_Sahara_FlashProgrammer(phoneHandle, _sSCMM_ARMPRG_FileName);
            if (!bOk)
            {
               printf("\nUnable to download Flash Programmer using Sahara Protocol\n");
               QLIB_DisconnectServer_Sahara(phoneHandle);
               return false;
            }
            QLIB_StartLogging(phoneHandle, "c:\\Qualcomm\\QLIB_QPHONEMS_Sahara.txt");

            printf("\n----------------QLIB_DisconnectServer_Sahara-----------------------------\n");
            QLIB_DisconnectServer_Sahara(phoneHandle);

            SWDL_UserPartitionList pPartitonList;
            //Please ensure to null terminate the path and the user partition name.
            char *sbl = "0:SBL\0";
            char *sbl_path = "C:\\images\\9x25_flatbuild\\sbl1.mbn\0";
            memcpy(pPartitonList.oPartitionList[0].sPartitionName, sbl, strlen(sbl) + 1);
            memcpy(pPartitonList.oPartitionList[0].sPartitionMBNPath, sbl_path, strlen(sbl_path) + 1);

            //Please ensure to null terminate the path and the user partition name.
            char *tz = "0:TZ\0";
            char *tz_path = "C:\\images\\9x25_flatbuild\\tz.mbn\0";
            memcpy(pPartitonList.oPartitionList[1].sPartitionName, tz, strlen(tz) + 1);
            memcpy(pPartitonList.oPartitionList[1].sPartitionMBNPath, tz_path, strlen(tz_path)+1);

            //Please ensure to null terminate the path and the user partition name.
            char *sdi = "0:SDI\0";
            char *sdi_path = "C:\\images\\9x25_flatbuild\\sdi.mbn\0";
            memcpy(pPartitonList.oPartitionList[2].sPartitionName, sdi, strlen(sdi) + 1);
            memcpy(pPartitonList.oPartitionList[2].sPartitionMBNPath, sdi_path, strlen(sdi_path)+1);

            //Please ensure to null terminate the path and the user partition name.
            char *mba = "0:MBA\0";
            char *mba_path = "C:\\images\\9x25_flatbuild\\mba.mbn\0";
            memcpy(pPartitonList.oPartitionList[3].sPartitionName, mba, strlen(mba) + 1);
            memcpy(pPartitonList.oPartitionList[3].sPartitionMBNPath, mba_path, strlen(mba_path)+1);

            //Please ensure to null terminate the path and the user partition name.
            char *rpm = "0:RPM\0";
            char *rpm_path = "C:\\images\\9x25_flatbuild\\rpm.mbn\0";
            memcpy(pPartitonList.oPartitionList[4].sPartitionName, rpm, strlen(rpm) + 1);
            memcpy(pPartitonList.oPartitionList[4].sPartitionMBNPath, rpm_path, strlen(rpm_path)+1);

            //Please ensure to null terminate the path and the user partition name.
            char *adsp = "0:ADSP\0";
            char *adsp_path = "C:\\images\\9x25_flatbuild\\dsp3.mbn\0";
            memcpy(pPartitonList.oPartitionList[5].sPartitionName, adsp, strlen(adsp) + 1);
            memcpy(pPartitonList.oPartitionList[5].sPartitionMBNPath, adsp_path, strlen(adsp_path)+1);

            //Please ensure to null terminate the path and the user partition name.
            char *qdsp = "0:QDSP\0";
            char *qdsp_path = "C:\\images\\9x25_flatbuild\\qdsp6sw.mbn\0";
            memcpy(pPartitonList.oPartitionList[6].sPartitionName, qdsp, strlen(qdsp) + 1);
            memcpy(pPartitonList.oPartitionList[6].sPartitionMBNPath, qdsp_path, strlen(qdsp_path)+1);

            //Please ensure to null terminate the path and the user partition name.
            char *acdb = "0:ACDB\0";
            char *acdb_path = "C:\\images\\9x25_flatbuild\\acdb.mbn\0";
            memcpy(pPartitonList.oPartitionList[7].sPartitionName, acdb, strlen(acdb) + 1);
            memcpy(pPartitonList.oPartitionList[7].sPartitionMBNPath, acdb_path, strlen(acdb_path)+1);

            //Please ensure to null terminate the path and the user partition name.
            char *apps = "0:APPS\0";
            char *apps_path = "C:\\images\\9x25_flatbuild\\apps.mbn\0";
            memcpy(pPartitonList.oPartitionList[8].sPartitionName, apps, strlen(apps) + 1);
            memcpy(pPartitonList.oPartitionList[8].sPartitionMBNPath, apps_path, strlen(apps_path)+1);

            //ensure to provide the correct number of files listed.
            pPartitonList.iNumOfPartitionEntry = 9;

            //Note: Connects to the same port number here.
            //If the port number renumerates, then be sure to modify to connect to the new port number
            g_hResourceContext  = QLIB_ConnectServer( _iComPort );
            if ( ! g_hResourceContext )
            {
               printf("QLIB_ConnectServer failed\n");
               return false;
            }
            // Configure call back functions and logging
            QLIB_QPHONEMS_UploadSBMultiImage_ConfigureCallBack(g_hResourceContext, swdlCallBack);
            QLIB_SetLogFlags( g_hResourceContext, LOG_ALL );
            QLIB_ExtendedTextMessage_SetCategoryEnable(g_hResourceContext, QMSL_ExtTextMsgCat_QPHONEMS_SW_Download, 1);

            printf("\n----------------QLIB_QPHONEMS_DownloadUserPartitions-----------------------------\n");
            bSkipGoToDownload = 1; //skip switch to download already switched
            _sSCMM_ARMPRG_FileName = "\0"; //skip programmer already downloaded by Sahara

            unsigned char result = false;
            result = QLIB_QPHONEMS_DownloadUserPartitions
                     (
                        g_hResourceContext,
                        _sSCMM_ARMPRG_FileName, //empty
                        _sSCMM_Partition_FileName,
                        &pPartitonList,
                        bOverridePrtnTable,
                        bUseTrustedMode,
                        bSkipGoToDownload, //1
                        iSleepTimeToDOWNLOOADmode,
                        iSleepTimeToGOcommand,
                        &iErrorCode
                     );
            printf("\nResult: %s", result ? "PASS" : "FAIL");
            printf(", ErrorCode=%d\n", result, iErrorCode);
         }
         else if (cKey == '5')
         {
            //please use the following file (eHostdl) if the device is in emergency download mode.
            //char* _sSCMM_ARMPRG_FileName = "C:\\SwImages\\9x15\\M9615ACEHWMAZD4020A.1_Min\\FlatBuild\\ENPRG9x15.hex";

            char* _sSCMM_ARMPRG_FileName = "C:\\SwImages\\9x15\\M9615ACEHWMAZD4020A.1_Min\\FlatBuild\\NPRG9x15.hex";
            char* _sSCMM_Partition_FileName = "C:\\SwImages\\9x15\\M9615ACEHWMAZD4020A.1_Min\\FlatBuild\\partition.mbn";

            //Following Structs are not defined in .h file; therefore, user will need to define them here.
            // the limit on the number of files is 10 as seen below.
            //typedef struct
            //{
            //   char sPartitionName[10];                      // PartitionName For example, 0:SBL1
            //   char sPartitionMBNPath[256];                // Path to the mbn files
            //}SWDL_UserPartitionEntry;

            //typedef struct
            //{
            //   SWDL_UserPartitionEntry oPartitionList[10];
            //   int  iNumOfPartitionEntry;
            //}SWDL_UserPartitionList;

            SWDL_UserPartitionList pPartitonList;

            //Please ensure to null terminate the path and the user partition name.
            char *sbl1 = "0:SBL1\0";
            char *sbl1_path = "C:\\SwImages\\9x15\\M9615ACEHWMAZD4020A.1_Min\\FlatBuild\\sbl1.mbn\0";
            int str_size = strlen(sbl1) + 1;
            memcpy((char*)pPartitonList.oPartitionList[0].sPartitionName, sbl1, str_size);
            str_size = strlen(sbl1_path) + 1;
            memcpy(pPartitonList.oPartitionList[0].sPartitionMBNPath, sbl1_path, str_size);

            //Please ensure to null terminate the path and the user partition name.
            char *sbl2 = "0:SBL2\0";
            char *sbl2_path = "C:\\SwImages\\9x15\\M9615ACEHWMAZD4020A.1_Min\\FlatBuild\\sbl2.mbn\0";
            memcpy(pPartitonList.oPartitionList[1].sPartitionName, sbl2, strlen(sbl2) + 1);
            memcpy(pPartitonList.oPartitionList[1].sPartitionMBNPath, sbl2_path, strlen(sbl2_path)+1);

            //Please ensure to null terminate the path and the user partition name.
            char *apps = "0:APPS\0";
            char *apps_path = "C:\\SwImages\\9x15\\M9615ACEHWMAZD4020A.1_Min\\FlatBuild\\apps.mbn\0";
            memcpy(pPartitonList.oPartitionList[2].sPartitionName, apps, strlen(apps) + 1);
            memcpy(pPartitonList.oPartitionList[2].sPartitionMBNPath, apps_path, strlen(apps_path)+1);

            //Please ensure to null terminate the path and the user partition name.
            char *rpm = "0:RPM\0";
            char *rpm_path = "C:\\SwImages\\9x15\\M9615ACEHWMAZD4020A.1_Min\\FlatBuild\\rpm.mbn\0";
            memcpy(pPartitonList.oPartitionList[3].sPartitionName, rpm, strlen(rpm) + 1);
            memcpy(pPartitonList.oPartitionList[3].sPartitionMBNPath, rpm_path, strlen(rpm_path)+1);

            //Please ensure to null terminate the path and the user partition name.
            char *dsp1 = "0:DSP1\0";
            char *dsp1_path = "C:\\SwImages\\9x15\\M9615ACEHWMAZD4020A.1_Min\\FlatBuild\\dsp1.mbn\0";
            memcpy(pPartitonList.oPartitionList[4].sPartitionName, dsp1, strlen(dsp1) + 1);
            memcpy(pPartitonList.oPartitionList[4].sPartitionMBNPath, dsp1_path, strlen(dsp1_path)+1);

            //Please ensure to null terminate the path and the user partition name.
            char *dsp2 = "0:DSP2\0";
            char *dsp2_path = "C:\\SwImages\\9x15\\M9615ACEHWMAZD4020A.1_Min\\FlatBuild\\dsp2.mbn\0";
            memcpy(pPartitonList.oPartitionList[5].sPartitionName, dsp2, strlen(dsp2) + 1);
            memcpy(pPartitonList.oPartitionList[5].sPartitionMBNPath, dsp2_path, strlen(dsp2_path)+1);

            //Please ensure to null terminate the path and the user partition name.
            char *dsp3 = "0:DSP3\0";
            char *dsp3_path = "C:\\SwImages\\9x15\\M9615ACEHWMAZD4020A.1_Min\\FlatBuild\\dsp3.mbn\0";
            memcpy(pPartitonList.oPartitionList[6].sPartitionName, dsp3, strlen(dsp3) + 1);
            memcpy(pPartitonList.oPartitionList[6].sPartitionMBNPath, dsp3_path, strlen(dsp3_path)+1);

            //ensure to provide the correct number of files listed.
            pPartitonList.iNumOfPartitionEntry = 7;

            unsigned char result = false;

            //If device is in emergency download mode then please ensure that device is responding to the nop packet
            //please also ensure that this is the first communication sent to the device after opening the comport
            //result = QLIB_QPHONEMS_DLoad_SendNop(g_hResourceContext, 1000);

            result = QLIB_QPHONEMS_DownloadUserPartitions
                     (
                        g_hResourceContext,
                        _sSCMM_ARMPRG_FileName,
                        _sSCMM_Partition_FileName,
                        &pPartitonList,
                        bOverridePrtnTable,
                        bUseTrustedMode,
                        bSkipGoToDownload,
                        iSleepTimeToDOWNLOOADmode,
                        iSleepTimeToGOcommand,
                        &iErrorCode
                     );
         }
         else if(cKey == '1')
         {
            char* _sSCMM_ARMPRG_FileName = "c:\\NPRG6695.hex";
            char* _sSCMM_Partition_FileName = "C:\\partition.mbn";

            printf("\nQPHONEMS SW download (SCMM)\n");

            bOk = QLIB_QPHONEMS_UploadSB2MultiImage(
                     g_hResourceContext,
                     _sSCMM_ARMPRG_FileName,
                     _sSCMM_Partition_FileName,//m_sPartitionFile,
                     NULL,
                     NULL,
                     NULL,
                     NULL,
                     NULL,            //sApps_FileName,
                     NULL,            //sAppsBootLoader_FileName,
                     NULL,            //sWinMobile_FileName,
                     NULL,            //sDSP1FileName,
                     NULL,            //sDSP2FileName,
                     "",          //sMBR_FileName,
                     "",          //sADSP_FileName,
                     "",          //sTrustZone,
                     "",          //sROFS1,
                     "",          //sROFS2,
                     "",          //sROFS3,
                     NULL,    //cefs_modem
                     bOverridePrtnTable,
                     bUseTrustedMode,
                     bSkipGoToDownload,
                     iSleepTimeToDOWNLOOADmode,
                     iSleepTimeToGOcommand,
                     &iErrorCode);

            //bOk = QLIB_QPHONEMS_UploadSB2MultiImage(
            //   g_hResourceContext,
            //   _sSCMM_ARMPRG_FileName,
            //   _sPartitionFile,
            //   _sDeviceBootLoader_FileName,
            //   _sQFailSafeBOotLoader_FileName,
            //   _sOSBootLoader_FileName,
            //   _sModemFileName,
            //   NULL,          //sApps_FileName,
            //   NULL,          //sAppsBootLoader_FileName,
            //   NULL,          //sWinMobile_FileName,
            //   NULL,          //sDSP1FileName,
            //   NULL,          //sDSP2FileName,
            //   "",            //sMBR_FileName,
            //   "",            //sADSP_FileName,
            //   "",            //sTrustZone,
            //   "",            //sROFS1,
            //   "",            //sROFS2,
            //   "",            //sROFS3,
            //   NULL,  //cefs_modem
            //   bOverridePrtnTable,
            //   bUseTrustedMode,
            //   bSkipGoToDownload,
            //   iSleepTimeToDOWNLOOADmode,
            //   iSleepTimeToGOcommand,
            //   &iErrorCode);
         }
         else if(cKey == '2')
         {
            char* _sMDM9k_ARMPRG = "c:\\NPRG9x00.hex";

            printf("\nQPHONEMS SW download (MDM9K)\n");

            bOk = QLIB_QPHONEMS_UploadSB2MultiImage(
                     g_hResourceContext,
                     _sMDM9k_ARMPRG,
                     _sPartitionFile,
                     _sDeviceBootLoader_FileName,
                     _sQFailSafeBOotLoader_FileName,
                     _sOSBootLoader_FileName,
                     _sModemFileName,
                     NULL,            //sApps_FileName,
                     NULL,            //sAppsBootLoader_FileName,
                     NULL,            //sWinMobile_FileName,
                     _sDSP1FileName,          //sDSP1FileName,
                     _sDSP2FileName,          //sDSP2FileName,
                     "",          //sMBR_FileName,
                     "",          //sADSP_FileName,
                     "",          //sTrustZone,
                     "",          //sROFS1,
                     "",          //sROFS2,
                     "",          //sROFS3,
                     NULL,            //cefs_modem file.
                     bOverridePrtnTable,
                     bUseTrustedMode,
                     bSkipGoToDownload,
                     iSleepTimeToDOWNLOOADmode,
                     iSleepTimeToGOcommand,
                     &iErrorCode);
         }
         else if (cKey == '3')
         {
            char* _sARMPRG_MSM7x25_FileName = "c:\\NPRG7625.hex";

            printf("\nQPHONEMS SW download (MSM7x25)\n");

            bOk = QLIB_QPHONEMS_UploadSB1MultiImage(
                     g_hResourceContext,
                     _sARMPRG_MSM7x25_FileName,
                     _sPartitionFile,
                     //_sPBL_FileName,
                     NULL,
                     _sQCSBL_FileName,
                     _sQCSBL_HeaderName,
                     _sOEMSBL_FileName,
                     _sOEMSBL_HeaderName,
                     _sModemFileName,
                     _sModemHeaderName, NULL, NULL,
                     /*_sAppsFileName,
                     _sAppsHeaderName,*/
                     _sAppsBLFileName,
                     _sAppsBLHeaderName,
                     NULL, //sWinMobile_FileName,
                     NULL, // sDSP1_FileName,
                     NULL, // sMBR_FileName,
                     NULL, //sADSP_FileName,
                     NULL,    //cefs_modem file upload.
                     //1, // bOverridePrtnTable,
                     0, // bOverridePrtnTable,
                     1, // bUseTrustedMode,
                     0, // bSkipGoToDownload,
                     5000, //iSleepTimeToDOWNLOADmode,
                     3000, //iSleepTimeToGOcommand,
                     &iErrorCode);
         }
         else if (cKey == '4')
         {
            char* _sSCMM_ARMPRG_FileName = "c:\\NPRG6270.hex";

            printf("\nQPHONEMS SW download (LCU QSC6240/6270)\n");

            bOk = QLIB_QPHONEMS_UploadSB2MultiImage(
                     g_hResourceContext,
                     _sSCMM_ARMPRG_FileName,
                     _sPartitionFile,
                     _sDeviceBootLoader_FileName,
                     _sQFailSafeBOotLoader_FileName,
                     _sOSBootLoader_FileName,
                     _sModemFileName,
                     NULL,            //sApps_FileName,
                     NULL,            //sAppsBootLoader_FileName,
                     NULL,            //sWinMobile_FileName,
                     NULL,            //sDSP1FileName,
                     NULL,            //sDSP2FileName,
                     "",          //sMBR_FileName,
                     "",          //sADSP_FileName,
                     "",          //sTrustZone,
                     "",          //sROFS1,
                     "",          //sROFS2,
                     "",          //sROFS3,
                     NULL,    //cefs_modem file upload
                     bOverridePrtnTable,
                     bUseTrustedMode,
                     bSkipGoToDownload,
                     iSleepTimeToDOWNLOOADmode,
                     iSleepTimeToGOcommand,
                     &iErrorCode);
         }

         _iTimeSpan = ::GetTickCount() - _iTimeSpan;
         printf("\nSoftware Download takes %d ms: ErrorCode=%d\n", _iTimeSpan, iErrorCode);
         QLIB_StopLogging(g_hResourceContext);
      }
      else if (cKey == 'G' )
      {
         //const variable, may need to modfiy when runing
         char *firehoseProgrammer = "c:\\images\\prog_emmc_firehose_8x26.mbn";
         char *rawprogramfile = "c:\\images\\rawprogram_unsparse.xml";
         char *patchfile = "c:\\images\\patch0.xml";
         char *cdtRawprogramfile = "c:\\images\\rawprogram2.xml";
         char *cdtPatchfile = "c:\\images\\patch2.xml";

         //parameters for firehose configuration
         char *MaxPayloadSizeToTargetInBytes = "49152";
         char *memory_name = "eMMC";
         char *target_name = "8x26";
         char *AckPacketNumber = "100";
         unsigned char bUseAckRawDataAttr = false;
         unsigned char bUseSkipWriteAttr = false;
         unsigned char bUseAlwaysValidateAttr = false;
         unsigned char bUseVerboseAttr = false;
         unsigned char bUseAckRawDataEveryNumPackets = false;

         HANDLE phoneHandle = NULL;
         unsigned long iErrorCode = 0;
         unsigned long _iTimeSpan = 0 ;
         unsigned long version = 0;
         unsigned long sn = 0;
         unsigned long msmid = 0;
         char pkHash[200];
         unsigned long timeout = 0;
         unsigned char bGetInfo = 0;
         unsigned char downloadCDT = 0;
         float imageSizeInMB = 0.0;
         float throughput = 0.0;
         int _iComPort = 0;
         int mode = 0;

         //QLIB_ExtendedTextMessage_SetCategoryEnable(g_hResourceContext, QMSL_ExtTextMsgCat_QPHONEMS_SW_Download, 1);
         QLIB_SetLogFlags( g_hResourceContext, LOG_ALL );
         QLIB_StartLogging( g_hResourceContext,  "C:\\QlibDemo_Sahara_Firehose.txt");

         printf("\n\nPlease confirm the image information before downloading:\n");
         printf("Firehose Programer: %s\n", firehoseProgrammer);
         printf("Raw program file : %s\n", rawprogramfile);
         printf("Patch file: %s\n", patchfile);
         printf("\nDownload CDT? enter \'Y\' for yes or \'N\' for not: ");
         downloadCDT = toupper(getch());

         if (downloadCDT == 'Y')
         {
            printf("\nRaw program file for CDT : %s\n", cdtRawprogramfile);
            printf("Patch file for CDT: %s\n", cdtPatchfile);
         }

         // Get the COM port
         printf("\nPlease enter the COM port #: ");
         scanf("%hd", &_iComPort);

         _iTimeSpan = ::GetTickCount();

         QLIB_QPHONEMS_SaharaConfigureCallback(g_hResourceContext, swdlCallBack);

         printf("\n----------------QLIB_QPHONEMS_ConnectServer_Sahara-----------------------\n");
         phoneHandle = QLIB_QPHONEMS_ConnectServer_Sahara(_iComPort, &version, &sn, &msmid, pkHash, bGetInfo, mode, timeout, swdlCallBack);
         if (phoneHandle == 0)
         {
            printf("\nQLIB_QPHONEMS_ConnectServer_Sahara failed\n");
            return false;
         }

         printf("\n----------------QLIB_QPHONEMS_Sahara_FlashProgrammer---------------------\n");
         bOk = QLIB_QPHONEMS_Sahara_FlashProgrammer(phoneHandle, firehoseProgrammer);
         if (!bOk)
         {
            printf("\nUnable to download Flash Programmer using Sahara Protocol\n");
            QLIB_DisconnectServer_Sahara(phoneHandle);
            return false;
         }

         printf("\n----------------QLIB_DisconnectServer_Sahara-----------------------------\n");
         QLIB_DisconnectServer_Sahara(phoneHandle);

         printf("\n----------------QLIB_QPHONEMS_FireHoseConfigureCallback------------------\n");
         QLIB_QPHONEMS_FireHoseConfigureCallback(g_hResourceContext, swdlCallBack);

         printf("\n----------------QLIB_QPHONEMS_ConnectServer_FireHose---------------------\n");
         phoneHandle = QLIB_QPHONEMS_ConnectServer_FireHose(_iComPort, swdlCallBack);
         if (phoneHandle == 0)
         {
            printf("QLIB_QPHONEMS_ConnectServer_FireHose failed\n");
            return false;
         }

         printf("\n----------------QLIB_QPHONEMS_FireHoseNOP--------------------------------\n");
         bOk = QLIB_QPHONEMS_FireHoseNOP(phoneHandle);
         if (!bOk)
         {
            printf("\nPing firehose device falied\n");
            return false;
         }

         printf("\n----------------QLIB_QPHONEMS_FireHoseConfigure--------------------------\n");

         bOk = QLIB_QPHONEMS_FireHoseConfigure( phoneHandle,
                                                MaxPayloadSizeToTargetInBytes,
                                                memory_name,
                                                target_name,
                                                AckPacketNumber,
                                                bUseAckRawDataAttr,
                                                bUseSkipWriteAttr,
                                                bUseAlwaysValidateAttr,
                                                bUseVerboseAttr,
                                                bUseAckRawDataEveryNumPackets
                                              );
         if (!bOk)
         {
            printf("\nQLIB_QPHONEMS_FireHoseConfigure falied\n");
            return false;
         }

         if (downloadCDT == 'Y')
         {
            printf("\n----------------QLIB_QPHONEMS_UploadEmmcImage_FireHose(CDT)--------------\n");
            printf("\nDownloading CDT...\n");
            bOk = QLIB_QPHONEMS_UploadEmmcImage_FireHose(phoneHandle, cdtRawprogramfile, cdtPatchfile, &imageSizeInMB, &throughput);
            if (!bOk)
            {
               printf("Download CDT falied\n");
               return false;
            }
         }

         printf("\n----------------QLIB_QPHONEMS_UploadEmmcImage_FireHose-------------------\n");
         printf("\nDownloading image files... it will take several minutes, please wait\n");
         bOk = QLIB_QPHONEMS_UploadEmmcImage_FireHose(phoneHandle, rawprogramfile, patchfile, &imageSizeInMB, &throughput);
         if (!bOk)
         {
            printf("QLIB_QPHONEMS_UploadEmmcImage_FireHose falied\n");
            return false;
         }
         printf("\nDonwloading successfully\n");
         printf("\nImageSizeInMB = %f MB: Throughput = %f MB/Sec\n", imageSizeInMB, throughput);

         printf("\n----------------QLIB_DisconnectServer_FireHose---------------------------\n");
         QLIB_DisconnectServer_FireHose(phoneHandle);

         _iTimeSpan = ::GetTickCount() - _iTimeSpan;
         printf("\nSoftware Download takes %d ms: ErrorCode=%d\n", _iTimeSpan, iErrorCode);
         QLIB_StopLogging(g_hResourceContext);
      }
   }    // if ( bOk )

   // Print the error message
   const int iErrorMessageLength = 100;
   char sErrorMessage[iErrorMessageLength];
   unsigned char bErrorOccurred = 0;
   QLIB_GetLastSoftwareDownloadErrorInfo( g_hResourceContext, &bErrorOccurred, sErrorMessage, iErrorMessageLength );

   // Print EFS Error status
   printf("\n  Test Status: %s\n", bErrorOccurred ? "FAIL" : "PASS");

   // Check for error
   if ( bErrorOccurred )
   {
      // Print Error Information
      printf("  SW Download Error: %s\n", sErrorMessage );
   }

   return bOk;
}

unsigned char Test_EFS( void )
{
   unsigned char bOk=true;
   unsigned char cKey;
   char sEfsFolder_withSlash[] = "/FactoryTest/Pictures";
   char sEfsFolder_noSlash[] = "FactoryTest/Pictures";
   char sEfsCreateFolder1[] = "brew/test";
   char sEfsCreateFolder2[] = "FactoryTest";
   char sEfsCreateFolder3[] = "FactoryTest/Pictures";
   char sEfsFolder_NVM_Alpha[] = "/nv/item_files";
   char sEfsFolder_NVM_Alpha_rfnv[] = "/nv/item_files/rfnv";
   char sEfsDir2[] = "brew/test";
   char sEfsFile[] = "brew/test/app.ico";
   char sLocalFile[] = "C:\\Program Files\\Qualcomm\\QDART\\bin\\app.ico";
   char _sCEFS_File[] = "C:\\CEFS.mbn";
   char _sPRL_File[] = "C:\\000.RL0";
   short _iPRL_NAM_Index = 0;

   char sMobileUserFolder[] = "brew/user/Pictures";
   char sLocalUserFolder[] = "c:\\temp";
   char sEFS_DeleteFolder[] = "Test/rm_folder";

   printf("----------------\nEFS Test\n----------------\n");

   // Check the EFS error when completed?
   unsigned char bCheckEFS_Error = false;

   if ( bOk )
   {
      printf("\n1=EFS Directory %s", sEfsFolder_withSlash );
      printf("\n2=EFS Write file %s to EFS:%s", sLocalFile, sEfsFile );
      printf("\n3=EFS Read EFS file %s to %s", sEfsFile, sLocalFile );
      printf("\n4=Create EFS folder: %s", sEfsCreateFolder1 );
      printf("\n5=Create EFS folder: %s", sEfsCreateFolder2 );
      printf("\n6=Create EFS folder: %s", sEfsCreateFolder3 );
      printf("\n7=Copy factory pictures from C:\\ODM\\Sirius\\EFS_Files\\W5\\ to %s", sEfsCreateFolder3);
      printf("\n8=EFS Directory %s", sEfsDir2 );
      printf("\n9=Remove File: %s", sEfsFile );
      printf("\nA=Remove EFS Directory: %s", sEfsCreateFolder1 );
      printf("\nB=Exercise EFS");
      printf("\nC=EFS2_DIAG_DEV_INFO");
      printf("\nD=Download  CEFS file %s, from phone to PC", _sCEFS_File );
      printf("\nE=Download PRL file %s, NAM%d, from phone to PC", _sPRL_File, _iPRL_NAM_Index );
      printf("\nF=Upload PRL file %s, NAM%d, from PC to phone", _sPRL_File, _iPRL_NAM_Index );
      printf("\nG=Extended EFS Info");
      printf("\nH=Move all files from phone's folder: %s", sMobileUserFolder, sLocalUserFolder);
      printf("\nI=Remove tree, folder: %s", sEFS_DeleteFolder );
      printf("\nJ=Read NV and Write NV items to EFS");
      cKey = toupper( getch() );

      // Set the payload size for EFS read and write operations
      //
      // Example measured time savings:
      //
      //  18.39 seconds, QPST, with 1000 bytes, write
      //  17.313 seconds with 1500 bytes
      //  25.4 seconds with 512 bytes
      const unsigned short c_iEFS_Read_Size = 1000;
      const unsigned short c_iEFS_Write_Size = 1500;
      QLIB_SetEfsReadAndWritePacketSize(
         g_hResourceContext,
         c_iEFS_Read_Size,
         c_iEFS_Write_Size );

      // For QPHONEMS, set maximum size per write to the serial device...if this number is
      // smaller than c_iEFS_Write_Size, then the packet will be broken into smaller sections
      // when written to the serial device.
      QLIB_SetSendDataSerialWriteSizeLimit( g_hResourceContext, c_iEFS_Write_Size );

      if (cKey == '1' )
      {
         printf("\nEFS directory of folder: '%s'\n", sEfsFolder_withSlash );

         // Perform a directory of the EFS, using the folder name pointed to by sEfsFolder
         if ( QLIB_EfsDirectory(
                  g_hResourceContext,
                  sEfsFolder_noSlash
               )
            )
         {
            printf("\nEFS DIR: PASS\n");
         }
         else
         {
            printf("\nEFS DIR: FAIL\n");
         }  // if ( QLIB_EfsDirectory()...
      }// cKey == 1
      else if (cKey == '2' )
      {
         printf("\nEFS Write: %s to EFS:%s\n", sLocalFile, sEfsFile );

         // Upload a QCN file from the PC to the phone
         if ( QLIB_EfsWrite(
                  g_hResourceContext,
                  sLocalFile,
                  sEfsFile
               )
            )
         {
            printf("\nEFS WRITE: PASS\n");
         }
         else
         {
            printf("\nEFS WRITE: FAIL\n");
         }  // if ( QLIB_EfsDirectory()...
      }// cKey == 2
      else if (cKey == '3' )
      {
         printf("\nEFS Read:  EFS: %s to Local:%s\n", sEfsFile, sLocalFile );

         // Upload a QCN file from the PC to the phone
         if ( QLIB_EfsRead(
                  g_hResourceContext,
                  sEfsFile,
                  sLocalFile
               )
            )
         {
            printf("\nEFS READ: PASS\n");
         }
         else
         {
            printf("\nEFS READ: FAIL\n");
         }  // if ( QLIB_EfsDirectory()...
      }// cKey == 3

      else if (cKey == '4' )
      {
         printf("\nEFS Create Folder:  %s \n", sEfsCreateFolder1 );

         // Upload a QCN file from the PC to the phone
         if ( QLIB_EfsMkDir(
                  g_hResourceContext,
                  sEfsCreateFolder1
               )
            )
         {
            printf("\nEFS Create Folder: PASS\n");
         }
         else
         {
            printf("\nEFS Create Folder: FAIL\n");
         }  // if ( QLIB_EfsDirectory()...
      }// cKey == 4
      else if (cKey == '5' )
      {
         printf("\nEFS Create Folder:  %s \n", sEfsCreateFolder2 );

         // Upload a QCN file from the PC to the phone
         if ( QLIB_EfsMkDir(
                  g_hResourceContext,
                  sEfsCreateFolder2
               )
            )
         {
            printf("\nEFS Create Folder: PASS\n");
         }
         else
         {
            printf("\nEFS Create Folder: FAIL\n");
         }  // if ( QLIB_EfsDirectory()...
      }// cKey == 5
      else if (cKey == '6' )
      {
         printf("\nEFS Create Folder:  %s \n", sEfsCreateFolder3 );

         // Upload a QCN file from the PC to the phone
         if ( QLIB_EfsMkDir(
                  g_hResourceContext,
                  sEfsCreateFolder3
               )
            )
         {
            printf("\nEFS Create Folder: PASS\n");
         }
         else
         {
            printf("\nEFS Create Folder: FAIL\n");
         }  // if ( QLIB_EfsDirectory()...
      }// cKey == 6
      else if (cKey == '7' )
      {
         const int c_iNumPictures = 12;

         char _sLocalFileNames[c_iNumPictures][128] =
         {
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Blue_Wolf5.jpg"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Green_Wolf5.jpg"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Red_Wolf5.jpg"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_RowColumnTest_Wolf5.jpg"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\Contrast_TFT_Wolf5_155.jpg"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Blue_Wolf5.bmp"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Black_Wolf5.bmp"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_White_Wolf5.bmp"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Green_Wolf5.bmp"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Red_Wolf5.bmp"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_RowColumnTest_Wolf5.bmp"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\Contrast_TFT_Wolf5_155.bmp"}
         };

         char _sEFSFileNames[c_iNumPictures][128] =
         {
            {"/FactoryTest/Pictures/TFT_Blue_Wolf5.jpg"},
            {"/FactoryTest/Pictures/TFT_Green_Wolf5.jpg"},
            {"/FactoryTest/Pictures/TFT_Red_Wolf5.jpg"},
            {"/FactoryTest/Pictures/TFT_RowColumnTest_Wolf5.jpg"},
            {"/FactoryTest/Pictures/Contrast_TFT_Wolf5_155.jpg"},
            {"/FactoryTest/Pictures/TFT_Blue_Wolf5.bmp"},
            {"/FactoryTest/Pictures/TFT_Black_Wolf5.bmp"},
            {"/FactoryTest/Pictures/TFT_White_Wolf5.bmp"},
            {"/FactoryTest/Pictures/TFT_Green_Wolf5.bmp"},
            {"/FactoryTest/Pictures/TFT_Red_Wolf5.bmp"},
            {"/FactoryTest/Pictures/TFT_RowColumnTest_Wolf5.bmp"},
            {"/FactoryTest/Pictures/Contrast_TFT_Wolf5_155.bmp"}
         };

         // Loop thorugh the pictures
         for ( int _iPictureIndex = 0;  _iPictureIndex < c_iNumPictures; _iPictureIndex++)
         {
            printf("\n Copying: %s...", _sLocalFileNames[ _iPictureIndex] );

            // Copy file
            // Upload a QCN file from the PC to the phone
            bOk = QLIB_EfsWrite( g_hResourceContext, _sLocalFileNames[ _iPictureIndex], _sEFSFileNames[ _iPictureIndex] );
            printf("\nQLIB_EfsWrite(%s): %s", _sLocalFileNames[ _iPictureIndex], bOk ? "PASS" : "FAIL" );
         }
      }// cKey == 7
      if (cKey == '8' )
      {
         printf("\nEFS directory of folder: '%s'\n", sEfsDir2 );

         // Perform a directory of the EFS, using the folder name pointed to by sEfsFolder
         if ( QLIB_EfsDirectory(
                  g_hResourceContext,
                  sEfsDir2
               )
            )
         {
            printf("\nEFS DIR: PASS\n");
         }
         else
         {
            printf("\nEFS DIR: FAIL\n");
         }  // if ( QLIB_EfsDirectory()...
      }// cKey == 8
      if (cKey == '9' )
      {
         printf("\nEFS delete file: '%s'\n", sEfsFile );

         // Perform a directory of the EFS, using the folder name pointed to by sEfsFolder
         bOk = QLIB_EfsDelete(  g_hResourceContext, sEfsFile);
         printf("\nQLIB_EfsDelete: %s\n", bOk ? "PASS" : "FAIL" );
      }// cKey == 9
      if (cKey == 'A' )
      {
         printf("\nEFS remove folder file: '%s'\n", sEfsCreateFolder1 );

         // Perform a directory of the EFS, using the folder name pointed to by sEfsFolder
         bOk = QLIB_EfsRmDir(   g_hResourceContext, sEfsCreateFolder1);
         printf("\nQLIB_EfsRmDir: %s\n", bOk ? "PASS" : "FAIL" );
      }// cKey == A
      else if (cKey == 'B' )
      {
         // Set the log flag conditions
         QLIB_SetLogFlags( g_hResourceContext,  LOG_IO );

         // Activate text logging
         bOk = QLIB_StartLogging( g_hResourceContext,  "C:\\EFS_Debug.txt");

         printf("\nStart Logging: %s", bOk ? "PASS" : "FAIL" );

         // Create the first folder
         printf("\nEFS Create Folder:  %s \n", sEfsCreateFolder2 );

         // Upload a QCN file from the PC to the phone
         if ( QLIB_EfsMkDir(
                  g_hResourceContext,
                  sEfsCreateFolder2
               )
            )
         {
            printf("\nEFS Create Folder: PASS\n");
         }
         else
         {
            printf("\nEFS Create Folder: FAIL\n");
         }  // if ( QLIB_EfsDirectory()...

         // Create the second folder

         printf("\nEFS Create Folder:  %s \n", sEfsCreateFolder3 );

         // Upload a QCN file from the PC to the phone
         if ( QLIB_EfsMkDir(
                  g_hResourceContext,
                  sEfsCreateFolder3
               )
            )
         {
            printf("\nEFS Create Folder: PASS\n");
         }
         else
         {
            printf("\nEFS Create Folder: FAIL\n");
         }  // if ( QLIB_EfsDirectory()...

         const int c_iNumPictures = 12;

         char _sLocalFileNames[c_iNumPictures][128] =
         {
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Blue_Wolf5.jpg"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Green_Wolf5.jpg"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Red_Wolf5.jpg"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_RowColumnTest_Wolf5.jpg"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\Contrast_TFT_Wolf5_155.jpg"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Blue_Wolf5.bmp"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Black_Wolf5.bmp"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_White_Wolf5.bmp"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Green_Wolf5.bmp"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_Red_Wolf5.bmp"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\TFT_RowColumnTest_Wolf5.bmp"},
            {"C:\\ODM\\Sirius\\EFS_Files\\W5\\Contrast_TFT_Wolf5_155.bmp"}
         };

         char _sEFSFileNames[c_iNumPictures][128] =
         {
            {"/FactoryTest/Pictures/TFT_Blue_Wolf5.jpg"},
            {"/FactoryTest/Pictures/TFT_Green_Wolf5.jpg"},
            {"/FactoryTest/Pictures/TFT_Red_Wolf5.jpg"},
            {"/FactoryTest/Pictures/TFT_RowColumnTest_Wolf5.jpg"},
            {"/FactoryTest/Pictures/Contrast_TFT_Wolf5_155.jpg"},
            {"/FactoryTest/Pictures/TFT_Blue_Wolf5.bmp"},
            {"/FactoryTest/Pictures/TFT_Green_Wolf5.bmp"},
            {"/FactoryTest/Pictures/TFT_Red_Wolf5.bmp"},
            {"/FactoryTest/Pictures/TFT_RowColumnTest_Wolf5.bmp"},
            {"/FactoryTest/Pictures/Contrast_TFT_Wolf5_155.bmp"}
         };

         // Loop through this next part until a keypress occurs
         while (!kbhit())
         {
            // Get the phone state
            if (!ReportPhoneState())
               break;

            // Loop thorugh the pictures
            for ( int _iPictureIndex = 0;  _iPictureIndex < c_iNumPictures; _iPictureIndex++)
            {
               if (!kbhit())
               {
                  // Perform a directory of the EFS, using the folder name pointed to by sEfsFolder
                  printf("\n Erasing: %s...", _sEFSFileNames[ _iPictureIndex] );

                  bOk = QLIB_EfsDelete( g_hResourceContext, _sEFSFileNames[ _iPictureIndex]);
                  printf("%s", bOk ? "PASS" : "FAIL" );

                  printf("\n Copying: %s...", _sLocalFileNames[ _iPictureIndex] );

                  // Copy file
                  // Upload a QCN file from the PC to the phone
                  bOk = QLIB_EfsWrite( g_hResourceContext, _sLocalFileNames[ _iPictureIndex], _sEFSFileNames[ _iPictureIndex] );
                  printf("%s", bOk ? "PASS" : "FAIL" );
               }    // if (!kbhit())
            }   //  for ( int _iPictureIndex
         }  // while (!kbhit())

         getch();   // Flush 1 key from the keyboard queue
      }// cKey == B
      else if (cKey == 'C')
      {
         // printf("C=EFS2_DIAG_DEV_INFO");
         unsigned long iErrorNo;
         unsigned long iTotalNumOfBlocks;
         unsigned long iNumPagesPerBlock;
         unsigned long iPageSize;
         unsigned long iTotalPageSize;
         unsigned long iMakerId;
         unsigned long iDeviceId;
         unsigned char iDeviceType;
         char psDeviceName[100];

         // Call the function
         bOk = QLIB_EFS2_DIAG_DEV_INFO(
                  g_hResourceContext,  &iErrorNo, &iTotalNumOfBlocks, &iNumPagesPerBlock,
                  &iPageSize, &iTotalPageSize, &iMakerId, &iDeviceId, &iDeviceType, psDeviceName );
         printf("\n\nEFS2_DIAG_DEV_INFO: %s", bOk ? "PASS" : "FAIL" );

         if (bOk)
         {
            // Print the results
            printf("\n            iErrorNo: %d", iErrorNo );
            printf("\n   iTotalNumOfBlocks: %d", iTotalNumOfBlocks );
            printf("\n   iNumPagesPerBlock: %d", iNumPagesPerBlock );
            printf("\n           iPageSize: %d", iPageSize );
            printf("\n      iTotalPageSize: %d", iTotalPageSize );
            printf("\n            iMakerId: %d", iMakerId );
            printf("\n           iDeviceId: %d", iDeviceId );
            printf("\n         iDeviceType: %d", iDeviceType );
            printf("\n          DeviceName: %s", psDeviceName );
         }
      }
      else if (cKey == 'D' )
      {
         // printf("\nD=Download  CEFS file %s, from phone to PC", sCEFS_File );

         printf("\nDownloading CEFS file: %s from phone to PC\n", _sCEFS_File );
         QLIB_DownloadCEFS_File( g_hResourceContext, _sCEFS_File );
      }// cKey == D
      else if (cKey == 'E' )
      {
         // printf("\nE=Download PRL file %s, from phone to PC", _sPRL_File );

         printf("\nDownloading PRL file %s, NAM%d, from PC to phone", _sPRL_File, _iPRL_NAM_Index);
         bOk = QLIB_DownloadPRL( g_hResourceContext, _iPRL_NAM_Index, _sPRL_File );
         printf(" QLIB_DownloadPRL: %s", bOk ? "PASS" : "FAIL" );

         bCheckEFS_Error = true;
      }// cKey == E
      else if (cKey == 'F' )
      {
         // printf("\nF=Upload PRL file %s, from PC to phone", _sPRL_File );

         printf("\nUploading PRL file %s, NAM%d, from PC to phone", _sPRL_File, _iPRL_NAM_Index);
         bOk = QLIB_UploadPRL( g_hResourceContext, _iPRL_NAM_Index, _sPRL_File );
         printf(" QLIB_UploadPRL: %s", bOk ? "PASS" : "FAIL" );

         bCheckEFS_Error = true;
      }// cKey == F
      else if (cKey == 'H' )
      {
         // printf("\nH=Move all files from phone's folder: %s", sMobileUserFolder, sLocalUserFolder);

         printf("\nEFS directory of folder: '%s'\n", sMobileUserFolder );

         g_iUserFileCount = 0;  // Clear the current number of files found in the user folder

         // Perform a directory of the EFS, using the folder name pointed to by sEfsFolder
         if ( QLIB_EfsDirectory(
                  g_hResourceContext,
                  sMobileUserFolder
               )
            )
         {
            printf("\nEFS DIR: PASS\n");
         }
         else
         {
            printf("\nEFS DIR: FAIL\n");
         }  // if ( QLIB_EfsDirectory()...

         printf("\n\n Found: %d files", g_iUserFileCount);

         // File index
         int _iFileIndex = 0;

         // Pointer to the file name of the next file (w/o the path)
         char *_psFileNameOnly = NULL;

         int _iNameOffset = 0;

         // Full local path name, next file
         char _sNextLocalPath[500];

         for ( _iFileIndex = 0; _iFileIndex < g_iUserFileCount; _iFileIndex++)
         {
            // Determine the file name by setting a pointer to the end of the string and
            // loopback back towards the start, looking for a "/" delimeter
            _iNameOffset = strlen( g_sUserfiles[ _iFileIndex] ) - 1;

            while (     ( _iNameOffset > 1 )
                        &&   ( g_sUserfiles[ _iFileIndex ][_iNameOffset -1] != '/')
                  )
               _iNameOffset--;

            // Set the pointer to the portion that is only the file name.
            _psFileNameOnly = &g_sUserfiles[ _iFileIndex ][_iNameOffset ];

            //
            // Copy the next file from Mobile to PC
            //

            // Local file name
            sprintf(_sNextLocalPath, "%s\\%s", sLocalUserFolder,  _psFileNameOnly );

            // Copy the file
            printf("\nEFS Read:  EFS: %s to Local:%s\n", g_sUserfiles[ _iFileIndex ], _sNextLocalPath );

            // Upload a QCN file from the PC to the phone
            if ( QLIB_EfsRead(
                     g_hResourceContext,
                     g_sUserfiles[ _iFileIndex ],
                     _sNextLocalPath
                  )
               )
            {
               printf("\nEFS READ: PASS\n");

               printf("\nEFS delete file: '%s'\n", g_sUserfiles[ _iFileIndex ]);

               // Perform a directory of the EFS, using the folder name pointed to by sEfsFolder
               bOk = QLIB_EfsDelete(    g_hResourceContext, g_sUserfiles[ _iFileIndex ]);
               printf("\nQLIB_EfsDelete: %s\n", bOk ? "PASS" : "FAIL" );
            }
            else
            {
               printf("\nEFS READ: FAIL\n");
            }   // if ( QLIB_EfsDirectory()...
         }  // for ( _iFileIndex = 0
      }// cKey == H
      else if (cKey == 'I' )
      {
         printf("\nEFS directory of folder: '%s'\n", sEFS_DeleteFolder );

         // Perform a directory of the EFS, using the folder name pointed to by sEfsFolder
         if ( QLIB_EfsDirectory(
                  g_hResourceContext,
                  sEFS_DeleteFolder
               )
            )
         {
            printf("\nEFS DIR: PASS\n");
         }
         else
         {
            printf("\nEFS DIR: FAIL\n");
         }  // if ( QLIB_EfsDirectory()...

         // printf("\nI=Remove tree, folder: %s", sEFS_DeleteFolder );
         bOk = QLIB_EfsRmTree( g_hResourceContext, sEFS_DeleteFolder );

         printf("\nQLIB_EfsRmTree(%s): %s", sEFS_DeleteFolder, bOk ? "PASS" : "FAIL" );
      }// cKey == I

      else if (cKey == 'J' )
      {
         printf("\nRead NV and Write NV items to EFS\n");
         unsigned int _nvIndex = 0;

         unsigned int _i = 0;

         unsigned char _itemData[128];

         unsigned short _iStatus;

         char _fileName[256];

         char _efsFileName[256];

         FILE *fp = NULL;
         QLIB_EfsMkDir( g_hResourceContext, sEfsFolder_NVM_Alpha );

         QLIB_EfsMkDir( g_hResourceContext, sEfsFolder_NVM_Alpha_rfnv );

         for(_nvIndex = 0; _nvIndex < 7200; _nvIndex++)
         {
            memset(_itemData, 0, 128);

            QLIB_DIAG_NV_READ_F(g_hResourceContext, _nvIndex, _itemData, 128, &_iStatus);

            if(_iStatus == NV_DONE_S)
            {
               memset(_fileName, 0, 256);
               sprintf(_fileName, "c:\\temp\\nvdata\\%d", _nvIndex + newRfNvItemOffset);

               fp = fopen(_fileName, "w");

               if(fp)
               {
                  for(_i = 0; _i < 128; _i++)
                     fputs((const char*)&_itemData[_i], fp);

                  fclose(fp);

                  memset(_efsFileName, 0, 256);
                  sprintf(_efsFileName, "%s/%d", sEfsFolder_NVM_Alpha_rfnv, _nvIndex + newRfNvItemOffset);

                  QLIB_EfsWrite(g_hResourceContext, _fileName, _efsFileName);
               }
            }
         }
      }
   }    // if ( bOk )

   if ( bCheckEFS_Error )
   {
      // Print the error message
      const int iErrorMessageLength = 100;
      char sErrorMessage[iErrorMessageLength];
      unsigned char bErrorOccurred = 0;
      QLIB_GetLastEfsErrorInfo( g_hResourceContext, &bErrorOccurred, sErrorMessage, iErrorMessageLength );

      // Print EFS Error status
      printf("\n  EFS Error Status: %s\n", bErrorOccurred ? "FAIL" : "PASS");

      // Check for error
      if ( bErrorOccurred )
      {
         // Print Error Information
         printf("  EFS Error: %s\n", sErrorMessage );
      }
   }

   return bOk;
}

//! Call back for an EFS directory element
unsigned char EfsDirCallBack
(
   char name[QLIB_EFS_MAX_FILENAME_LEN],
   unsigned char isFile,

   //attributes follow below
   unsigned short iAttributeMask,
   unsigned char iBufferingOption,
   unsigned char iCleanupOption,
   unsigned long iCreateDate,
   unsigned long iFileSize,
   HANDLE hContextID
)
{
   printf("-----------------\n");
   printf("          Name: %s\n", name );
   printf("          Type: %s\n", isFile ? "File" : "Folder" );
   printf("Attribute Mask: %d\n", iAttributeMask);
   printf(" Buffer Option: %d\n", iBufferingOption);
   printf("Cleanup Option: %d\n", iCleanupOption);
   printf("   Create Date: %d\n", iCreateDate);
   printf("     File Size: %d\n", iFileSize);
   printf("       Context: %d\n", hContextID);

   // Check to see if the number of user files is less than the maximum
   if ( g_iUserFileCount < gc_iMaxUserFiles )
   {
      // Check to see if this is a file
      if ( isFile )
      {
         // Make sure the length is ok
         if (strlen( name) < gc_iUserFileNameLengthMax)
         {
            // Copy the file name
            strcpy( g_sUserfiles[g_iUserFileCount], name );

            // Increment the index
            g_iUserFileCount++;
         }  // if (strlen( name) < gc_iUserFileLengthMax ))
      } // if ( isFile )
   }    //if ( g_iUserFileCount < gc_iMaxUserFiles )

   return 1;
}

unsigned char EfsCallback_demo
(
   char srcname[512],
   char dstname[512],
   int oper,
   int suboper,
   unsigned long bytestransferred,
   unsigned long filesize,
   HANDLE hContext
)
{
   printf("-----------------\n");
   printf("         SRC Name: %s\n", srcname );
   printf("         DST Name: %s\n", dstname );
   printf("        Operation: %d\n", oper);
   printf("       Sub Option: %d\n", suboper );
   printf("Bytes Transferred: %d\n", bytestransferred);
   printf("        File Size: %d\n", filesize);
   printf("          Context: %d\n", hContext );

   return 1;
}

//! Callback for generic download events
unsigned char generalDownloadEventCallBack
(
   unsigned char* pGeneralSwDownloadEvent_union
)
{
   // Cast to the specific union
   generalSwDownloadEvent_struct* _pEvent= (generalSwDownloadEvent_struct*)pGeneralSwDownloadEvent_union;

   // Check for the event type
   //
   // Put checks here for each type of event
   //
   switch (_pEvent->eEventType)
   {
      case SWD_downloadEvent:
         //    uses struct downloadEvent_struct;

         printf("\n Event: SWD_downloadEvent");
         printf("-----------------\n"              );
         printf("    Status: %d\n", _pEvent->uEventData.downloadEvent.status           );
         printf("     Error: %d\n", _pEvent->uEventData.downloadEvent.error            );
         printf("%%Complete: %d\n", _pEvent->uEventData.downloadEvent.percentCompleted );
         printf("     Block: %d\n", _pEvent->uEventData.downloadEvent.block            );
         printf("   Address: %d\n", _pEvent->uEventData.downloadEvent.address          );
         printf("      Data: %s\n", _pEvent->uEventData.downloadEvent.dataString       );
         printf("   Context: %d\n", _pEvent->hContextID        );

         break;

      case SWD_nvBackupEvent:
         //    uses struct nvBackupEvent_struct;

         printf("\n Event: SWD_nvBackupEvent");

         // if ( percentCompleted != 0xFFFF )
         {
            printf("    Status: %d\n", _pEvent->uEventData.nvBackupEvent.status            );
            printf("     Error: %d\n", _pEvent->uEventData.nvBackupEvent.error         );
            printf("%%Complete: %d\n", _pEvent->uEventData.nvBackupEvent.percentCompleted  );
            printf("   NV Item: %d\n", _pEvent->uEventData.nvBackupEvent.nvItem            );

            if ( _pEvent->uEventData.nvBackupEvent.dataString[0] != 0x00 )
               printf("      Data: %s\n", _pEvent->uEventData.nvBackupEvent.dataString     );
            printf("   Context: %d\n", _pEvent->hContextID     );
         }
         break;

      case SWD_doAutoRestore:
         //  No data
         printf("\n Event: SWD_doAutoRestore");
         break;

      case SWD_bootDownloadStart:
         //    uses struct bootDownloadStart_struct;
         printf("\n Event: SWD_bootDownloadStart");
         printf("\n    FLASH name: %s", _pEvent->uEventData.bootDownloadStart.flashName );
         printf("\n    Loader Size: %d", _pEvent->uEventData.bootDownloadStart.loaderSize);

         break;

      case SWD_reportLoaderStatus:
         //    uses struct bootDownloadStatus_struct;
         printf("\n Event: SWD_reportLoaderStatus");
         printf("\n    Loader name: %s", _pEvent->uEventData.reportLoaderStatus.bsLoaderName );
         printf("\n    Custom Loader: %d", _pEvent->uEventData.reportLoaderStatus.customLoader );
         printf("\n       Local File: %d", _pEvent->uEventData.reportLoaderStatus.localFile );
         break;

      case SWD_bootReportLoaderStatus:
         //    uses struct bootReportLoaderStatus_struct;
         printf("\n Event: SWD_bootReportLoaderStatus");
         printf("\n    Loader name: %s", _pEvent->uEventData.bootReportLoaderStatus.bsLoaderName);
         printf("\n    Custom Loader: %d", _pEvent->uEventData.bootReportLoaderStatus.customLoader);
         printf("\n  Local File name: %d", _pEvent->uEventData.bootReportLoaderStatus.localFile);
         break;

      case SWD_bootDownloadStatus:
         //    uses struct SWD_bootDownloadStatus_struct;
         printf("\n Event: SWD_bootReportLoaderStatus");
         printf("\n    Status: %d", _pEvent->uEventData.bootDownloadStatus.statusCode );
         printf("\n    Detail: %d", _pEvent->uEventData.bootDownloadStatus.detailCode );
         break;

      case SWD_bootDownloadProgress:
         //    uses struct bootDownloadProgress_struct;
         printf("\n Event: SWD_bootDownloadProgress");
         printf("\n    Bytes written: %d", _pEvent->uEventData.bootDownloadProgress.bytesWritten );
         break;

      case SWD_bootDownloadComplete:
         //    no data
         printf("\n Event: SWD_bootDownloadComplete");
         break;

      case SWD_MIReportLoaderStatus:
         //    uses struct MIReportLoaderStatus_struct;
         printf("\n Event: SWD_MIReportLoaderStatus");
         printf("\n    Loader name: %s", _pEvent->uEventData.MIReportLoaderStatus.bsLoaderName);
         printf("\n    Custom Loader: %d", _pEvent->uEventData.MIReportLoaderStatus.customLoader);
         printf("\n   Local File name: %d", _pEvent->uEventData.MIReportLoaderStatus.localFile);
         break;

      case SWD_MIReportFlashStatus:
         //    uses struct MIReportFlashStatus_struct;
         printf("\n Event: SWD_MIReportFlashStatus");
         printf("\n    FLASH name: %s", _pEvent->uEventData.MIReportFlashStatus.flashName);
         break;

      case SWD_MIStartDownload:
         //    uses struct MIStartDownload_struct;
         printf("\n Event: SWD_MIStartDownload");
         printf("\n    Image Size: %d", _pEvent->uEventData.MIStartDownload.imageSize);
         printf("\n    Image Type: %d", _pEvent->uEventData.MIStartDownload.imageType);
         break;

      case SWD_MIDownloadProgress:
         //    uses struct MIDownloadProgress_struct;
         printf("\n Event: SWD_MIDownloadProgress");
         printf("\n    Bytes Written: %d", _pEvent->uEventData.MIDownloadProgress.bytesWritten);
         break;
      case SWD_MIDownloadStatus:
         //    uses struct MIDownloadStatus_struct;
         printf("\n Event: SWD_MIDownloadStatus");
         printf("\n    Detail Code: %d", _pEvent->uEventData.MIDownloadStatus.detailCode );
         printf("\n    Status Code: %d", _pEvent->uEventData.MIDownloadStatus.statusCode);
         break;
      case SWD_MIDownloadComplete:
         // No data
         printf("\n Event: SWD_MIDownloadComplete");

         break;

      default:
         printf("\n Event: Default");
         break;
   }    // switch (eEventType)

   return 1;
}

unsigned char Test_Logging( void )
{
   unsigned char bOk=true;
   unsigned char cKey;

   int c_iMsgIterationLimit = 10;
   int _iIterationCount = 0;

   static unsigned char aiRequestBuffer  [ DIAG_MAX_PACKET_SIZE ];
   static unsigned char aiResponseBuffer [ DIAG_MAX_PACKET_SIZE ];
   static short iRequestSize  = 3;  // Size of request packet
   static short iResponseSize = 0;  // This will get updated by SendSync
   char c_sDLF_File[] = "C:\\Factory_log.dlf";

   printf("----------------\nTest Phone Logging\n----------------\n");

   // if ( bOk )
   if ( true )  // Allow this menu to be displayed regardless of FTM pass, so that
      // the user can control the DTR lines
   {
      printf("\n0=Enable Phone Logging" );
      printf("\n1=Capture phone log message to console & QLIB log file -- (press any key to stop)" );
      printf("\n2=Get Available Response Messages" );
      printf("\n3=Set log code, all" );
      printf("\n4=Message Level-HIGH" );
      printf("\n5=Message Level-OFF" );
      printf("\n6=Capture Keypresses" );
      printf("\n7=Keypress capture & report (press any key to stop)" );
      printf("\n8=Start capture of debug messages for a userdefined SSID" );
      printf("\n9=Interpret Async Message in a QLIB log file" );
      printf("\nA=Start FTM logging for GSM BER" );
      printf("\nB=Event logging on" );
      printf("\nC=Enable async message callback function" );
      printf("\nD=Start FTM logging for WCDMA AGC" );
      printf("\nE=Test multiple log codes" );
      printf("\nF=Enable text message callback function" );
      printf("\nG=Enable GPS Events (404, 405, 406)" );
      printf("\nH=Enable GSM L1 logs, FTM messages, and DLF file" );
      printf("\nI=Configure keypresses for string 'ECode:' used by 1X targets such as 6801" );
      printf("\nJ=Playback asynchronous messages from %s", c_sDLF_File );

      cKey = toupper( getch() );
      printf("\n");

      if ( (cKey == '0' ) )
      {
         // Enable all phone logging
         bOk = QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
         printf("\nQLIB_DIAG_SetPhoneLoggingState = %s", bOk ? "PASS" : "FAIL" );
      }
      else if ( (cKey == '1' ) )
      {
         const unsigned short c_iDM_MaxLogSize = DIAG_MAX_PACKET_SIZE;

         /*--assume QLIB text logging is already started
         // Set the text log flag conditions
         QLIB_SetLogFlags( g_hResourceContext,  LOG_IO | LOG_FN );

         // Activate text logging
         bOk = QLIB_StartLogging( g_hResourceContext,  _sLogFile );

         printf("\nEnable Logging = %s", bOk ? "PASS" : "FAIL" );
         */

         printf("\nPress M for marker or any other key to exit." );

         printf("\nLog Output:\n");

         // Flag to determine exit
         unsigned char _bExit = false;

         // Counter for number of markers
         int _iMarker = 0;

         unsigned char bSendToConsole = true;
         unsigned char bSendToQlibLogFile = true;
         unsigned char  bShowBinaryData = false;
         char _sBuffer[200];

         // Get the next message
         while ( !_bExit)
         {
            // Dump available messages to a text log
            QLIB_DumpAsyncMessageToTextLog( g_hResourceContext, LOG_IO, bSendToConsole, bSendToQlibLogFile, bShowBinaryData );

            // Check for exit condition
            if  ( kbhit() )
            {
               // Test for the M key, to set a marker
               if ( toupper(getch())=='M')
               {
                  // Increment a marker
                  _iMarker++;

                  // Prepare text buffer
                  sprintf(_sBuffer,  "\n\n>>>>>>>  Marker: %d\n\n", _iMarker);

                  // Add buffer to log file
                  QLIB_AddUserMessageToTextLog(g_hResourceContext, LOG_FN|LOG_IO, _sBuffer, true );

                  // Place a marker in the log
               }
               else
                  _bExit = true;
            }   // if  ( kbhit() )

            // Sleep in order to lower CPU usage
            Sleep(10);
         }  // while (!kbhit())

         // This will flush the keyboard buffer.  This needs to be done because "kbhit()"
         // only checks to see if a key has been hit, but it does not pop it off the buffer.
         // getch();
      }// cKey == 1
      else if ( (cKey == '2' ) )        // Get on-demand messages
      {
         printf("\nLog Output:\n");

         DumpAsync();
      }// cKey == 2
      else if ( (cKey == '3' ) )        // Get on-demand messages
      {
         printf("\nSetting Log Mask...");

         const int c_iOperationDataMaxSize = 512;
         // unsigned char* _aiOperationData[ c_iOperationDataMaxSize ];

         const unsigned long c_iOperationDisableLogging = 0;
         const unsigned long c_iRetrieveID_Ranges = 1;
         const unsigned long c_iSetLogMask = 3;

         const unsigned long c_iLastItem = 100; // # of items for specific equipment ID
         const unsigned long c_iEquipmentID = 4;
         const unsigned short _iOperationSize = 10;

         // QLIB_DIAG_SetExtendedLogMask( g_hResourceContext, c_iSetLogMask, _aiOperationData);
      }// cKey == 3
      else if ( (cKey == '4' ) )        // Message Level-HIGH
      {
         bOk = QLIB_DIAG_SetMessageLevel(g_hResourceContext, 0x0002);
         printf("\nSet Message Level HIGH = %s", bOk ? "PASS" : "FAIL" );
      }// cKey == 4
      else if ( (cKey == '5' ) )        // Message Level-OFF
      {
         bOk = QLIB_DIAG_SetMessageLevel(g_hResourceContext, 0x00FF);
         printf("\nSet Message Level OFF = %s", bOk ? "PASS" : "FAIL" );
      }// cKey == 5
      else if ( (cKey == '6' ) )        // Capture key presses
      {
         bOk = QLIB_DIAG_KeyPress_StartCapture(g_hResourceContext );
         printf("\nCapture Key Presses = %s", bOk ? "PASS" : "FAIL" );
      }// cKey == 6
      else if ( (cKey == '7' ) )        // key press report
      {
         unsigned long _iKeypressQueueSize = 0;
         unsigned short _iKeyPressID = 0;
         unsigned char _iKeyPressEvent = 0 ;

         printf("\nPress keys on phone to observe keypress report.  Press any key on PC to exit.");

         // Clear the keypress queue
         bOk = QLIB_DIAG_KeyPress_ClearQueue(g_hResourceContext);
         printf("\nQLIB_DIAG_KeyPress_ClearQueue(): %s", bOk ? "PASS" : "FAIL" );

         //
         // In the QLIB_DIAG_KeyPress_ConfigureKeyDetect() lines below, enable the correct one for your target.  See the
         // QMSL Function reference for Keypresses for more information about how to determine the correct method for
         // your target.
         //

         // Setup keypress detection parameters--this line is used for subsystem 0, Legacy
         bOk = QLIB_DIAG_KeyPress_ConfigureKeyDetect( g_hResourceContext, "AEEKey_RemovePressed: ", "AEEKey_AddPressed: ", 0, 8 );

         // Setup keypress detection parameters--this line is used for subsystem 0, Legacy.  Used for 1X targets like 6801
         bOk = QLIB_DIAG_KeyPress_ConfigureKeyDetect( g_hResourceContext, "#*p:ECode:102, Ke", "#*p:ECode:101, Ke", 0, 8 );

         // Setup keypress detection parameters--this line is used for subsystem 6002, User Interface
         bOk = QLIB_DIAG_KeyPress_ConfigureKeyDetect( g_hResourceContext, "AEEKey_RemovePressed: ", "AEEKey_AddPressed: ", 6002, 8 );

         // Enable phone to report key presses
         bOk = QLIB_DIAG_KeyPress_StartCapture(g_hResourceContext );

         // Enable capturing of phone log messages on the PC
         bOk = QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );

         // Wait for keypress on PC
         while ( !kbhit() )
         {
            Sleep(1);

            // Place simuli processing here, such as key press controller activity

            // Get the keypress queue size
            QLIB_DIAG_KeyPress_GetQueueSize(g_hResourceContext, &_iKeypressQueueSize);

            // Make sure the queue size is > 0
            while ( _iKeypressQueueSize > 0 )
            {
               // Get the next key event
               if ( QLIB_DIAG_KeyPress_GetNextKeyEvent(g_hResourceContext,  &_iKeyPressID, &_iKeyPressEvent ) )
               {
                  // Print the result
                  printf("\nKey ID: %d\tEvent ID: %d", _iKeyPressID, _iKeyPressEvent  );
               }    // if ( QLIB_DIAG_KeyPress_GetNextKeyEvent(...

               // Get the keypress queue size
               QLIB_DIAG_KeyPress_GetQueueSize(g_hResourceContext, &_iKeypressQueueSize);
            }   // while ( _iKeypressQueueSize > 0 )
         }  // while ( !kbhit() )

         bOk = QLIB_DIAG_KeyPress_StopCapture( g_hResourceContext );
         printf("\nQLIB_DIAG_KeyPress_StopCapture( ): %s", bOk ? "PASS" : "FAIL" );

         // Flush the PC's keypress buffer
         getch();
         printf("\n\n");
      }// cKey == 7
      else if (cKey == '8')
      {
         unsigned long _iSSID = 0;
         long _iRT_MASK;

         printf("\n\nPlease enter an SSID.  Common options are:");
         printf("\n           Legacy = 0");
         printf("\n          UMTS L1 = 3001");
         printf("\n           GSM L1 = 4001");
         printf("\n   User Interface = 6002");
         printf("\n     SSID? ");
         scanf("%d", &_iSSID );

         printf("\n\nPlease enter a bit mask.  Common options are:");
         printf("\n          0 = OFF");
         printf("\n          1 (2^0) = Low");
         printf("\n          2 (2^1) = Med");
         printf("\n          4 (2^2) = High");
         printf("\n          8 (2^3) = Error");
         printf("\n          16(2^4)  = Fatal");
         printf("\n          -1 = all");
         printf("\n     MASK: ");
         scanf("%d", &_iRT_MASK );

         printf("\n\n   SSID = 0x%X", (short) _iSSID);
         printf("\n\nRT_MASK = 0x%X", _iRT_MASK);

         bOk = QLIB_DIAG_SetMessageFilter( g_hResourceContext, (short) _iSSID, _iRT_MASK );
         printf("\nQLIB_DIAG_SetMessageFilter() = %s", bOk ? "PASS" : "FAIL" );

         // Enable all phone logging
         bOk = QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
         printf("\nQLIB_DIAG_SetPhoneLoggingState = %s", bOk ? "PASS" : "FAIL" );
      } // cKey == 8
      else if (cKey == '9')
      {
         InterpretAsyncInLog();
      }
      else if (cKey == 'A')
      {
         // printf("\nA=Start FTM logging" );

         //
         // This first command enables FTM to generate the log messsages
         //
         bOk = QLIB_FTM_LOG_StartFTM_Log( g_hResourceContext, LOG_FTM2_LOG_GSM_BER );
         printf("\nQLIB_FTM_LOG_StartFTM_Log = %s", bOk ? "PASS" : "FAIL" );
      }
      else if (cKey == 'B')
      {
         // printf("\nB=Event logging on" );

         bOk = QLIB_DIAG_EventReportControl( g_hResourceContext, TRUE);
         printf("\nQLIB_DIAG_EventReportControl = %s", bOk ? "PASS" : "FAIL" );
      }
      else if (cKey == 'C')
      {
         // printf("\nC=Enable async message callback function" );
         g_bAsyncCallbackVerbose = true;
      }
      else if (cKey == 'D')
      {
         // printf("\nD=Start FTM logging for WCDMA AGC" );

         PrintWCDMA_AGC();
      }
      else if (cKey == 'E')
      {
         // printf("\nE=Test multiple log codes" );

         int _iLogIndex = 0;

         // Enable these log codes:
         // 0x1046 = BT Audio Gateway
         // 0x103E = BT General
         //
         const unsigned short c_iLogCode1 = 0x1046;
         const unsigned short c_iLogCode2 = 0x103E;

         bOk = QLIB_DIAG_ClearAllLogCodes( g_hResourceContext, LOG_EQUIP_ID_1X );
         printf("\nQLIB_DIAG_ClearAllLogCodes = %s", bOk ? "PASS" : "FAIL" );

         // Setup the log code list
         const unsigned short c_iLogCodeListSize = 2;
         unsigned short _aiLogCodeList[ c_iLogCodeListSize ] =  { c_iLogCode1, c_iLogCode2 };

         // Add codes to the phone's log code mask
         bOk = QLIB_DIAG_AddExtendedLogCodes( g_hResourceContext, LOG_EQUIP_ID_1X, _aiLogCodeList, c_iLogCodeListSize );
         printf("\nQLIB_DIAG_AddExtendedLogCodes = %s", bOk ? "PASS" : "FAIL" );

         // Declare memory for log buffers
         const int c_iNumLogs = 5;
         void* _aLogs[ c_iNumLogs ];
         word _aSizes[ c_iNumLogs ];

         // Declare memory for each log
         for ( _iLogIndex = 0; _iLogIndex < c_iNumLogs; _iLogIndex++ )
         {
            // Declare memory
            _aLogs[_iLogIndex] = malloc( DIAG_MAX_PACKET_SIZE );

            _aSizes[_iLogIndex] = 0;
         }

         // Timeout
         const unsigned long c_iTimeOut_ms = 10000;

         unsigned short _iNumLogs = c_iNumLogs;

         // Until a key is pressed, print status of logs
         do
         {
            printf("\nWaiting for more logs, press any key to exit...");

            //
            // Get multiple logs first
            //

            // Set the maximum number of logs, must be done each time because this value is overwritten
            _iNumLogs = c_iNumLogs;

            // Get multiple logs
            bOk = QLIB_DIAG_GetMultipleLogs( g_hResourceContext, LOG_EQUIP_ID_1X, c_iLogCode1, &_iNumLogs, c_iTimeOut_ms, _aLogs, _aSizes );
            printf("\nQLIB_DIAG_GetMultipleLogs(code = 0x%X, count = %d, timeout = %d) = %s",
                   c_iLogCode1,
                   c_iNumLogs,
                   c_iTimeOut_ms,
                   bOk ? "PASS" : "FAIL" );

            printf("\nQLIB_DIAG_GetMultipleLogs() - # of logs found: %d", _iNumLogs);

            for ( _iLogIndex = 0; _iLogIndex < _iNumLogs; _iLogIndex++)
            {
               // Structure to sort out the log message
               LogMessage* _pLogMessage = (LogMessage*) _aLogs[ _iLogIndex ] ;

               printf("\n   Log #%d, id=0x%X, size = %d", _iLogIndex, _pLogMessage->log_item, _aSizes[ _iLogIndex ] );
            }   // for ( _iLogIndex = 0; _iLogIndex < _iNumLogs; _iLogIndex++)

            //
            // Get a signel log
            //

            // Set the maximum number of logs, must be done each time because this value is overwritten

            // Get multiple logs
            bOk = QLIB_DIAG_GetSingleLog( g_hResourceContext, LOG_EQUIP_ID_1X, c_iLogCode2, c_iTimeOut_ms, _aLogs[0], &_aSizes[0] );
            printf("\nQLIB_DIAG_GetSingleLog(code = 0x%X, timeout = %d) = %s",
                   c_iLogCode2,
                   c_iTimeOut_ms,
                   bOk ? "PASS" : "FAIL" );

            if ( bOk )
            {
               // Structure to sort out the log message
               LogMessage* _pLogMessage = (LogMessage*) _aLogs[ 0 ] ;

               printf("\n   Log id=0x%X, size = %d", _pLogMessage->log_item, _aSizes[  0] );
            }   // if ( bOk )
         }
         while ( !kbhit() );

         // Clear PC keypress queue
         if (kbhit())
            getch();

         // Erase the memory for the logs
         for ( _iLogIndex = 0; _iLogIndex < c_iNumLogs; _iLogIndex++ )
         {
            // Declare memory
            delete _aLogs[_iLogIndex];
         }  // for ( _iLogIndex
      }
      else if (cKey == 'F')
      {
         // printf("\nF=Enable text message callback function" );
         g_bTextCallbackVerbose = true;
      }
      else if (cKey == 'G')
      {
         // printf("\nG=Enable GPS Events (404, 405, 406)" );

         //
         // Enable event logging
         //
         bOk = QLIB_DIAG_EventReportControl( g_hResourceContext, TRUE);
         printf("\nQLIB_DIAG_EventReportControl = %s", bOk ? "PASS" : "FAIL" );

         bool _bEventBitMaskState = false;  // false = turn off all bits in event mask
         bOk = QLIB_DIAG_SetEntireEventMaskState( g_hResourceContext, false);
         printf("\nQLIB_DIAG_SetEntireEventMaskState() = %s", bOk ? "PASS" : "FAIL" );

         //
         // The following turns on certain bits in the event mask
         //
         const unsigned short c_iEventListSize = 3;
         unsigned short _aiEventList[c_iEventListSize] = {404, 405, 406};
         unsigned char _bActivateEvents = true;

         bOk = QLIB_DIAG_SetEventMaskBits(
                  g_hResourceContext,
                  _aiEventList,
                  c_iEventListSize,
                  _bActivateEvents );

         printf("\nQLIB_DIAG_SetEventMaskBits() = %s", bOk ? "PASS" : "FAIL" );
      }
      else if (cKey == 'H')
      {
         //printf("\nH=Enable GSM L1 logs, FTM messages, and DLF file" );

         /*
         Below is a sample setup for logging both debug messages and specific log messages
         to a DLF and QMSL text log.  This technique is often needed for getting detailed
         mobile asynchronous messages when QXDM cannot be ran.
         */

         //
         // GSM L1 logs
         //
         bOk = QLIB_DIAG_AddExtendedLogCode(  g_hResourceContext, LOG_EQUIP_ID_GSM, 0x5231  );
         bOk &= QLIB_DIAG_AddExtendedLogCode(  g_hResourceContext, LOG_EQUIP_ID_GSM, 0x5234  );
         bOk &= QLIB_DIAG_AddExtendedLogCode(  g_hResourceContext, LOG_EQUIP_ID_GSM, 0x5072  );
         bOk = QLIB_DIAG_AddExtendedLogCode(  g_hResourceContext, LOG_EQUIP_ID_GSM, 0x5231  );

         //
         // FTM messages (subsytem 23, mask 0xFF)
         //
         bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, 23, 0xFF );

         printf("\nEnable of GSM Logs, FTM Messages, and DLF log file: %s",
                bOk ? "PASS" : "FAIL" );

         //
         // Set text file logging
         //
         QLIB_SetLogFlags( g_hResourceContext, LOG_DEFAULT );
         QLIB_StartLogging( g_hResourceContext, "c:\\factory_log.txt");

         //
         // Start DLF--this must happen after QLIB_StartLogging()
         //
         bOk &= QLIB_StartDLF_Logging( g_hResourceContext,  c_sDLF_File, false);

         // Send a custom annotation into the DLF file
         QLIB_AnnotateDLF_Log( g_hResourceContext, "This is a user defined message which will show up in the DLF as an ANNOTATION entry" );
      }
      else if (cKey == 'I')
      {
         // printf("\nI=Configure keypresses for string 'ECode:' used by 1X targets such as 6801" );
      }
      else if ( cKey == 'J')
      {
         // printf("\nJ=Playback asynchronous messages from %s", c_sDLF_File );
         bOk &= QLIB_Playback_DLF( g_hResourceContext, c_sDLF_File );
      }
   }
   else
   {
      printf("\nFTM Mode Failed\n");
   }    // if ( bOk )

   return bOk;
}

void PrepareByteListString( char* sString, byte* piBytes, int iSize)
{
   int _iStringIndex = 0;
   char _sHexValues[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
   int _iNextHexValue = 0;

   // Loop through all characters
   for (int _iBinIndex = 0; _iBinIndex < iSize; _iBinIndex++)
   {
      // Calculate UPPER byte
      _iNextHexValue = (piBytes[ _iBinIndex ] & 0xF0 ) >> 4;

      // Add the upper byte into the string
      sString[ _iStringIndex++ ] = _sHexValues[ _iNextHexValue ];

      // Calculate LOWER byte
      _iNextHexValue = (piBytes[ _iBinIndex ] & 0x0F );

      // Add the upper byte into the string
      sString[ _iStringIndex++ ] = _sHexValues[ _iNextHexValue ];

      // Add a space
      sString[ _iStringIndex++ ] = ' ' ;
   }

   // At the end, there must be a NULL terminating character
   sString[ _iStringIndex++ ] = '\0' ;
}

unsigned char Test_Audio( void )
{
   unsigned char bOk = true;
   char cKey = ' ';
   const char sWaveFile[] = "C:\\FactoryTest.wav";
   const unsigned short c_iPCM_Capture_Buffers = 100;
   const unsigned short c_iPCM_Capture_Size = c_iPCM_Capture_Buffers * 320;
   const unsigned short c_iNV_VOC_HS_MIC_GAIN_ADJUST_I = 2827;

   printf("----------------\nAUDIO FTM Test\n----------------\n");

   if ( bOk )
   {
      printf("\n");
      printf("\n1=Audio Device - Handset");
      printf("\n2=Audio Device - Speaker phone");
      printf("\n3=Audio Device - Analog handsfree kit");
      printf("\n4=Audio Device - Mono headset");
      printf("\nR=Audio Device - FM headset");
      printf("\nS=Audio Device - User Defined");
      printf("\n---------------");
      printf("\n5=DSP Loopback On");
      printf("\n6=DSP Loopback Off");
      printf("\n---------------");
      printf("\n7=PCM Loopback On");
      printf("\n8=PCM Loopback Off");
      printf("\n---------------");
      printf("\nA=Volume 0");
      printf("\nB=Volume 1");
      printf("\nC=Volume 2");
      printf("\nD=Volume 3");
      printf("\nE=Volume 4");
      printf("\nQ=Volume 5");
      printf("\n---------------");
      printf("\nF=Tone 1@500Hz");
      printf("\nG=Tone 1@500Hz, Tone 2@700Hz");
      printf("\nH=Tones OFF");
      printf("\n---------------");
      printf("\nI=Noise Supressor ON");
      printf("\nJ=Noise Supressor OFF");
      printf("\n---------------");
      printf("\nK=PCM Capture %d buffers to %s", c_iPCM_Capture_Buffers, sWaveFile);
      printf("\n---------------");
      printf("\nL=FTM Set CodecTxGain Adjust = -600 (-6dB)" );
      printf("\nM=FTM Set CodecTxGain Adjust =  600 (+6dB)" );
      printf("\n---------------");
      printf("\nN=Set NV CodecTxGain Adjust = -600 (-6dB)" );
      printf("\nO=Set NV CodecTxGain Adjust =  600 (+6dB)" );
      printf("\nP=Read NV CodecTxGain Adjust" );

      cKey = toupper(getch());

      if (cKey == '1' )
      {
         bOk = QLIB_FTM_AUDIO_SET_PATH( g_hResourceContext, FTM_AUDIO_PATH_HANDSET );

         printf("\nAudio Set Path = %s", bOk ? "PASS" : "FAIL");
      } // cKey = '1'
      else if (cKey == '2')
      {
         bOk = QLIB_FTM_AUDIO_SET_PATH( g_hResourceContext, FTM_AUDIO_PATH_SPEAKER_PHONE );

         printf("\nAudio Set Path = %s", bOk ? "PASS" : "FAIL");
      } // cKey = '2'
      else if (cKey == '3')
      {
         bOk = QLIB_FTM_AUDIO_SET_PATH( g_hResourceContext, FTM_AUDIO_PATH_ANALOG_HANDSFREE );

         printf("\nAudio Set Path = %s", bOk ? "PASS" : "FAIL");
      } // cKey = '3'
      else if (cKey == '4')
      {
         bOk = QLIB_FTM_AUDIO_SET_PATH( g_hResourceContext, FTM_AUDIO_PATH_MONO_HEADSET );

         printf("\nAudio Set Path = %s", bOk ? "PASS" : "FAIL");
      } // cKey = '4'
      else if (cKey == 'R')
      {
         bOk = QLIB_FTM_AUDIO_SET_PATH( g_hResourceContext, FTM_AUDIO_PATH_FM_MONO_HS );

         printf("\nAudio Set Path = %s", bOk ? "PASS" : "FAIL");
      } // cKey = 'R'
      else if (cKey == 'S')
      {
         // printf("\nS=Audio Device - User Defined");

         unsigned short _iPath;
         printf("\n\nEnter Audio Path number: ");
         scanf("%hd", & _iPath );

         bOk = QLIB_FTM_AUDIO_SET_PATH( g_hResourceContext, _iPath );

         printf("\nAudio Set Path (%d) = %s", _iPath, bOk ? "PASS" : "FAIL");
      } // cKey = 'S'
      else if (cKey == '5')
      {
         bOk = QLIB_FTM_AUDIO_DSP_LOOPBACK( g_hResourceContext, 1 );

         printf("\nAudio DSP Loopaback ON = %s", bOk ? "PASS" : "FAIL");
      } // cKey = '5'
      else if (cKey == '6')
      {
         bOk = QLIB_FTM_AUDIO_DSP_LOOPBACK( g_hResourceContext, 0 );

         printf("\nAudio DSP Loopaback OFF = %s", bOk ? "PASS" : "FAIL");
      } // cKey = '6'
      else if (cKey == '7')
      {
         bOk = QLIB_FTM_AUDIO_PCM_LOOPBACK( g_hResourceContext, 1 );

         printf("\nAudio PCM Loopaback ON = %s", bOk ? "PASS" : "FAIL");
      } // cKey = '7'
      else if (cKey == '8')
      {
         bOk = QLIB_FTM_AUDIO_PCM_LOOPBACK( g_hResourceContext, 0 );

         printf("\nAudio PCM Loopaback OFF = %s", bOk ? "PASS" : "FAIL");
      } // cKey = '8'
      else if ( (cKey == 'a' ) || (cKey == 'A' ) )
      {
         bOk = QLIB_FTM_AUDIO_SET_VOLUME( g_hResourceContext, 0 );

         printf("\nAudio Volume 0 = %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'b' ) || (cKey == 'B' ) )
      {
         bOk = QLIB_FTM_AUDIO_SET_VOLUME( g_hResourceContext, 1 );

         printf("\nAudio Volume 1 = %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'c' ) || (cKey == 'C' ) )
      {
         bOk = QLIB_FTM_AUDIO_SET_VOLUME( g_hResourceContext, 2 );

         printf("\nAudio Volume 2 = %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'd' ) || (cKey == 'D' ) )
      {
         bOk = QLIB_FTM_AUDIO_SET_VOLUME( g_hResourceContext, 3 );

         printf("\nAudio Volume 3 = %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'e' ) || (cKey == 'E' ) )
      {
         bOk = QLIB_FTM_AUDIO_SET_VOLUME( g_hResourceContext, 4 );

         printf("\nAudio Volume 4 = %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'q' ) || (cKey == 'Q' ) )
      {
         bOk = QLIB_FTM_AUDIO_SET_VOLUME( g_hResourceContext, 5 );

         printf("\nAudio Volume 5 = %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'f' ) || (cKey == 'F' ) )
      {
         // Loop through a set of frequencies
         for ( unsigned short _iFreqUp = 30; _iFreqUp < 3000; _iFreqUp+= 10)
         {
            printf("\n  Audio Tone = %dHz", _iFreqUp );
            bOk = QLIB_FTM_AUDIO_TONES_PLAY( g_hResourceContext, _iFreqUp, 0, (unsigned short) FTM_AUDIO_SND_METHOD_VOICE  );
         }
         bOk = QLIB_FTM_AUDIO_TONES_PLAY( g_hResourceContext, 500, 0, (unsigned short) FTM_AUDIO_SND_METHOD_VOICE  );

         printf("\nAudio Tones ON = %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'g' ) || (cKey == 'G' ) )
      {
         bOk = QLIB_FTM_AUDIO_TONES_PLAY( g_hResourceContext, 500, 700, (unsigned short) FTM_AUDIO_SND_METHOD_VOICE  );

         printf("\nAudio Tones ON = %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'h' ) || (cKey == 'H' ) )
      {
         bOk = QLIB_FTM_AUDIO_TONES_STOP( g_hResourceContext );

         printf("\nAudio Tones OFF = %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'i' ) || (cKey == 'I' ) )
      {
         bOk = QLIB_FTM_AUDIO_NS_CONTROL( g_hResourceContext, 1 );

         printf("\nAudio Noise supressor ON = %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'j' ) || (cKey == 'J' ) )
      {
         bOk = QLIB_FTM_AUDIO_NS_CONTROL( g_hResourceContext, 0 );

         printf("\nAudio Noise supressor OFF = %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'l' ) || (cKey == 'L' ) )
      {
         bOk = QLIB_FTM_AUDIO_SET_CODECTXGAIN_ADJUST( g_hResourceContext, -600 );

         printf("\nSet CodecTxGainAdjust = -600, %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'm' ) || (cKey == 'M' ) )
      {
         bOk = QLIB_FTM_AUDIO_SET_CODECTXGAIN_ADJUST( g_hResourceContext, 600 );

         printf("\nSet CodecTxGainAdjust = 600, %s", bOk ? "PASS" : "FAIL");
      }
      else if ( (cKey == 'k' ) || (cKey == 'K' ) )
      {
         unsigned char* _pWavData = NULL;
         const unsigned char bCreateWAVformat = true;
         const int c_iWaveHeaderSize = 44;
         const unsigned short c_iDataTransferSize = 512;
         const int c_iDelay_ms = 2000;

         // Allocate memory
         _pWavData = (unsigned char*) malloc( c_iPCM_Capture_Size + c_iWaveHeaderSize );

         printf("\n\n...Capturing Audio...");

         bOk = QLIB_FTM_AUDIO_CapturePCMandTransferData( g_hResourceContext, c_iPCM_Capture_Buffers, c_iDelay_ms, _pWavData, c_iDataTransferSize, bCreateWAVformat );

         printf("\nAudio PCM Capture = %s", bOk ? "PASS" : "FAIL");

         if (bOk)
         {
            if ( _pWavData != NULL )
            {
               printf("\nStoring WAV (%s)...", sWaveFile );

               // Select binary mode for the IO library
               _fmode = _O_BINARY;

               // Setup file pointer
               FILE* _hWavFile = NULL;

               // Now store this data into a file
               if (_hWavFile = fopen( sWaveFile,"w"))
               {
                  // Write the data to a file
                  fwrite( _pWavData, c_iPCM_Capture_Size + c_iWaveHeaderSize, 1, _hWavFile);
               }    // fopen()

               // Close the file
               if (_hWavFile != NULL )
                  fclose( _hWavFile );
            }   // if ( _piPicData != NULL )

            // Free the memory
            free( _pWavData);
         }
      } // else if ( (cKey == 'k' ) || (cKey == 'K' ) )
      else if(cKey == 'N')
      {
         const short _iState = -600;
         unsigned short _iStatus;
         if ( QLIB_DIAG_NV_WRITE_F( g_hResourceContext, c_iNV_VOC_HS_MIC_GAIN_ADJUST_I, (unsigned char*) &_iState, sizeof( _iState), &_iStatus ) )
         {
            printf("\nNV Write(%d)=%d: PASS\n", c_iNV_VOC_HS_MIC_GAIN_ADJUST_I, _iState );

            // Check the status
            if ( _iStatus == NV_DONE_S )
            {
               printf("\nNV Write Status: PASS\n");
            }
            else
            {
               printf("\nNV Write Status: FAIL\n");
            }
         }
         else
            printf("\nNV Write: FAIL\n");
      }
      else if(cKey == 'O')
      {
         const short _iState = 600;
         unsigned short _iStatus;
         if ( QLIB_DIAG_NV_WRITE_F( g_hResourceContext, c_iNV_VOC_HS_MIC_GAIN_ADJUST_I, (unsigned char*) &_iState, sizeof( _iState), &_iStatus ) )
         {
            printf("\nNV Write(%d)=%d: PASS\n", c_iNV_VOC_HS_MIC_GAIN_ADJUST_I, _iState );

            // Check the status
            if ( _iStatus == NV_DONE_S )
            {
               printf("\nNV Write Status: PASS\n");
            }
            else
            {
               printf("\nNV Write Status: FAIL\n");
            }
         }
         else
            printf("\nNV Write: FAIL\n");
      }
      else if(cKey == 'P')
      {
         short _iState = 0;
         unsigned short _iStatus;
         if ( QLIB_DIAG_NV_READ_F( g_hResourceContext, c_iNV_VOC_HS_MIC_GAIN_ADJUST_I, (unsigned char*) &_iState, sizeof( _iState), &_iStatus ) )
         {
            printf("\nRead (%d)=%d: PASS\n", c_iNV_VOC_HS_MIC_GAIN_ADJUST_I, _iState );

            // Check the status
            if ( _iStatus == NV_DONE_S )
            {
               printf("\nNV Read Status: PASS\n");
            }
            else
            {
               printf("\nNV Read Status: FAIL\n");
            }
         }
         else
            printf("\nNV Read: FAIL\n");
      } // if (bOk)
   }

   return bOk;
}

unsigned char Test_Camera( void )
{
   unsigned char bOk = true;
   char cKey = ' ';
   char _sPictureFileName[100];

   printf("----------------\nCamera FTM Test\n----------------\n");

   if ( bOk )
   {
      printf("\n");
      printf("\n1=Select camera 1");
      printf("\n2=Select camera 2");
      printf("\n3=Take multiple pictures to c:\\FactoryTest_#.jpg", _sPictureFileName );
      printf("\n4=Power camera 1");
      printf("\n5=Power camera 2");
      printf("\n6=Force resolution to 640x480, save to c:\\FactoryTest_#.jpg", _sPictureFileName );
      printf("\n7=Force resolution to 800x600, save to c:\\FactoryTest_#.jpg", _sPictureFileName );

      cKey = getch();

      unsigned char _bAutoPictureSize = true;   // Auto determine picture width
      unsigned short    _iPictureWidth = 1600;          // not used because auto-
      unsigned short    _iPictureHeight = 1200;

      unsigned short _iStatus;

      if (cKey == '1' )
      {
         bOk = QLIB_FTM_CAMERA_SET_SENSOR_ID(g_hResourceContext, 0, &_iStatus);
         printf("\n QLIB_FTM_CAMERA_SET_SENSOR_ID = %s", bOk ? "PASS" : "FAIL" );
      }
      else if (cKey == '2' )
      {
         bOk = QLIB_FTM_CAMERA_SET_SENSOR_ID(g_hResourceContext,1, &_iStatus);
         printf("\n QLIB_FTM_CAMERA_SET_SENSOR_ID = %s", bOk ? "PASS" : "FAIL" );
      }

      // If option 6, then set the option for 640x480
      if (cKey == '6' )
      {
         _bAutoPictureSize = false; // Auto determine picture width
         _iPictureWidth = 640;          // not used because auto-
         _iPictureHeight = 480;
      }

      // If option 7, then set the option for 640x480
      if (cKey == '7' )
      {
         _bAutoPictureSize = false; // Auto determine picture width
         _iPictureWidth = 800;          // not used because auto-
         _iPictureHeight = 600;
      }

      if (  (cKey == '3' )
            || (cKey == '6' )
            || (cKey == '7' )
            || (cKey == '4' )
         )
      {
         ////////////////////////////////////
         // Added for debug of camera code
         //

         // Setup to log certain legacy messages
         unsigned long _iSSID = 0;  // 0 = Legacy sub system
         long _iRT_MASK = 24;       // 24 = 8 + 16 = error + fatal
         bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, (short) _iSSID, _iRT_MASK );

         // Setup to log all FTM messages
         _iSSID = 23;           // 23 = FTM
         _iRT_MASK = 255;       // 255 = all
         bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, (short) _iSSID, _iRT_MASK );

         // Enable phone events
         bOk &= QLIB_DIAG_EventReportControl( g_hResourceContext, true );

         // Enable all phone logging
         bOk &= QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );

         // Clear the aysnc queue
         QLIB_DIAG_ClearPhoneLogQueue( g_hResourceContext);

         ////////////////////////////////////
      } // Key = 3,4, or 6

      if (  (cKey == '3' )
            || (cKey == '6' )
            || (cKey == '7' )
         )
      {
         // Setup variables
         unsigned short _iDelay_ms = 2000;          // 2 seconds
         unsigned long  _iQuality = 50;             // % quality (0-99)
         unsigned short _iFormat = CAMERA_JPEG;     // JPEG format
         unsigned long  _iPicDataSize = 0;          // Will get updated when picture is taken
         unsigned short _iDataTransferSize = 500;   // Number of bytes transferred at a time
         void*  _piPicData = NULL;
         unsigned short _iCameraStatus = CAMERA_SUCCESS;

         // Ask user how many pictures to take
         int _iPicMax = 0;
         printf("\n\n# of pictures to take: ");
         scanf("%d",&_iPicMax);

         // Take multiple pictures
         for ( int _iPicNum=0; _iPicNum<_iPicMax; _iPicNum++)
         {
            printf("\nTaking picture %d...", _iPicNum);

            bOk = QLIB_FTM_CAMERA_PerformTakePictureSequence
                  (
                     g_hResourceContext,
                     _bAutoPictureSize,
                     _iPictureWidth,
                     _iPictureHeight,
                     _iDelay_ms,
                     _iQuality,
                     _iFormat,
                     &_iPicDataSize,
                     &_iCameraStatus
                  );
            printf("\nQLIB_FTM_CAMERA_PerformTakePictureSequence = %s, (status = %d)", bOk ? "PASS" : "FAIL", _iCameraStatus );

            // Now transfer the picture into memory
            if (    ( bOk )
                    &&   (_iPicDataSize != 0 )
               )
            {
               // Round picture size up, to the nearest 512 bytes
               // _iPicDataSize = (_iPicDataSize % _iDataTransferSize + 1) * _iDataTransferSize;

               // Allocate memory
               _piPicData = malloc( _iPicDataSize );

               if ( _piPicData != NULL )
               {
                  printf("\nRetrieving picture (%d bytes)...", _iPicDataSize );

                  bOk = QLIB_FTM_CAMERA_GetEntirePicture
                        (
                           g_hResourceContext,
                           _iPicDataSize,
                           _iDataTransferSize,
                           (unsigned char*) _piPicData,
                           &_iCameraStatus
                        );

                  printf("\nQLIB_FTM_CAMERA_GetEntirePicture = %s", bOk ? "PASS" : "FAIL");

                  if (bOk)
                  {
                     // Select binary mode for the IO library
                     _fmode = _O_BINARY;

                     // Setup file pointer
                     FILE* _hGraphicFile = NULL;

                     sprintf(_sPictureFileName, "C:\\FactoryTest_%d.jpg", _iPicNum );

                     // Now store this data into a file
                     if (_hGraphicFile = fopen( _sPictureFileName,"w"))
                     {
                        // Write the data to a file
                        fwrite( _piPicData, _iPicDataSize, 1, _hGraphicFile);
                     }  // fopen()

                     // Close the file
                     if (_hGraphicFile != NULL )
                        fclose( _hGraphicFile );
                  } // if (bOk)
               }    // if ( _piPicData != NULL )

               // Free the memory
               free( _piPicData);
            }   //
            else
            {
               break;
            }   // if (bOk)
         }  // for ( int _iPicNum=0; _iPicNum<10; _iPicNum++)
      } // cKey = '3'
      else if (cKey == '4' )
      {
         /*
         unsigned long _iMask = 0x00000001;

         bOk = SetBitState_DWORD_Reg( iAddress, _iMask, 1 );
         bOk = SetBitState_DWORD_Reg( iAddress, _iMask, 0 );
         */

         unsigned long iAddress = 0X8400015C;       // GPIO Out 0

         unsigned char _iCam1_Mask = 0x01;
         unsigned char _iCam2_Mask = 0x02;

         // Camera 1 on
         bOk = SetBitState_BYTE_Reg( iAddress, _iCam1_Mask, 1 );

         // Camera 1 off
         bOk = SetBitState_BYTE_Reg( iAddress, _iCam1_Mask, 0 );

         // Camera 2 on
         bOk = SetBitState_BYTE_Reg( iAddress, _iCam2_Mask, 1 );

         // Camera 2 off
         bOk = SetBitState_BYTE_Reg( iAddress, _iCam2_Mask, 0 );
      }
   }

   // Setup to log all FTM messages
   unsigned long _iSSID2 = 0;   // 0 = Legacy sub system
   long _iRT_MASK2 = 0;     // 0 = none
   bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, (short) _iSSID2, _iRT_MASK2 );

   // Disable all phone logging
   bOk &= QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, false );

   //////////////////////////////////

   return bOk;
}

unsigned char SetBitState_DWORD_Reg( unsigned long iAddress, unsigned long iMask, unsigned short bOnOff )
{
   unsigned short _iNumOps = 1;
   unsigned long  _iPeekStorage = 0x00000000;
   unsigned long  _iPokeStorage = 0;
   unsigned char bOk_Peek1 = false;
   unsigned char bOk_Poke = false;
   unsigned char bOk_Peek2 = false;

   bOk_Peek1 = QLIB_DIAG_MEMORY_PEEK_DWORD_F( g_hResourceContext, iAddress, _iNumOps, &_iPeekStorage );

   printf("\nQLIB_DIAG_MEMORY_PEEK_DWORD_F: %s", bOk_Peek1 ? "PASS" : "FAIL\n" );
   printf("\n   Peek Value: 0x%X", _iPeekStorage );

   if ( bOnOff)
   {
      // Turn on the desired bit
      _iPokeStorage = _iPeekStorage | iMask;
   }
   else
   {
      // Turn on the desired bit
      _iPokeStorage = _iPeekStorage & ~iMask;
   }
   bOk_Poke = QLIB_DIAG_MEMORY_POKE_DWORD_F( g_hResourceContext, iAddress, _iNumOps, &_iPokeStorage );

   printf("\n   QLIB_DIAG_MEMORY_POKE_DWORD_F: %s", bOk_Poke ? "PASS" : "FAIL\n" );

   bOk_Peek2 = QLIB_DIAG_MEMORY_PEEK_DWORD_F( g_hResourceContext, iAddress, _iNumOps, &_iPeekStorage );

   printf("\nQLIB_DIAG_MEMORY_PEEK_DWORD_F: %s", bOk_Peek2 ? "PASS" : "FAIL\n" );
   printf("\n   Peek Value: 0x%X", _iPeekStorage );
   return (bOk_Peek1 && bOk_Peek2 && bOk_Poke );
}

unsigned char SetBitState_BYTE_Reg( unsigned long iAddress, unsigned char iMask, unsigned short bOnOff )
{
   unsigned short _iNumOps = 1;
   unsigned char _iPeekStorage = 0x00000000;
   unsigned char  _iPokeStorage = 0;
   unsigned char bOk_Peek1 = false;
   unsigned char bOk_Poke = false;
   unsigned char bOk_Peek2 = false;

   bOk_Peek1 = QLIB_DIAG_MEMORY_PEEK_F( g_hResourceContext, iAddress, _iNumOps, &_iPeekStorage );

   printf("\nQLIB_DIAG_MEMORY_PEEK_F: %s", bOk_Peek1 ? "PASS" : "FAIL\n" );
   printf("\n   Peek Value: 0x%X", _iPeekStorage );

   if ( bOnOff)
   {
      // Turn on the desired bit
      _iPokeStorage = _iPeekStorage | iMask;
   }
   else
   {
      // Turn on the desired bit
      _iPokeStorage = _iPeekStorage & ~iMask;
   }
   bOk_Poke = QLIB_DIAG_MEMORY_POKE_F( g_hResourceContext, iAddress, _iNumOps, &_iPokeStorage );

   printf("\n   QLIB_DIAG_MEMORY_POKE_F: %s", bOk_Poke ? "PASS" : "FAIL\n" );

   bOk_Peek2 = QLIB_DIAG_MEMORY_PEEK_F( g_hResourceContext, iAddress, _iNumOps, &_iPeekStorage );

   printf("\nQLIB_DIAG_MEMORY_PEEK_F: %s", bOk_Peek2 ? "PASS" : "FAIL\n" );
   printf("\n   Peek Value: 0x%X", _iPeekStorage );
   return (bOk_Peek1 && bOk_Peek2 && bOk_Poke );
}

unsigned char Test_Simlock( void )
{
    const int MAX_CK_LEN = 16;
    const int MAX_SALT_LEN = 16;
    const int MAX_HCK_LEN = 32;
    char ck[MAX_CK_LEN + 1] = {0};
    unsigned char salt[MAX_SALT_LEN] = {0};
    unsigned char bOk = true;
    HANDLE handle = NULL;
    char cKey = ' ';
   

    printf("----------------\nSimlock Test\n----------------\n");

    int ck_len = 0;
    printf("Please input the ck length->\n");
    scanf("%d", &ck_len);

    printf("\n----------------QLIB_SIMLOCK_GenerateCK----------------\n");
    bOk = QLIB_SIMLOCK_GenerateCK(handle, ck_len, ck);
    if (bOk == false)
    {
        printf("QLIB_SIMLOCK_GenerateCK failed\n");
        return false;
    }
    printf("The generated random ck is: %s\n", ck);
    

    printf("\n----------------QLIB_SIMLOCK_GenerateSalt----------------\n");
    bOk = QLIB_SIMLOCK_GenerateSalt(handle, salt);
    if (bOk == false)
    {
        printf("QLIB_SIMLOCK_GenerateSalt failed\n");
        return false;
    }
    printf("The generated Salt data are: \n");
    for (int i=0; i<MAX_SALT_LEN; i++)
    {
        printf("0x%02X ", salt[i]);
    }

    printf("\nNow using the const data to verify the API\n");
    char const_ck[MAX_CK_LEN + 1] = "1234567876543210";
    unsigned char const_salt[MAX_SALT_LEN] = {0x70, 0x61, 0xae, 0xb8, 0x93, 0xf8, 0xec, 0x2e, 0xc2, 0x5d, 0xa3, 0x55, 0x66, 0xe9, 0x1, 0x98};
    unsigned int iterations = 25000;
    unsigned char hck[MAX_HCK_LEN] = {0};

    printf("\n----------------QLIB_SIMLOCK_crypto_run_pbkdf2_algorithm----------------\n");
    bOk = QLIB_SIMLOCK_crypto_run_pbkdf2_algorithm(
                            handle, 
                            (unsigned char *)const_ck, 
                            MAX_CK_LEN, 
                            const_salt, 
                            MAX_SALT_LEN,
                            iterations,
                            hck,
                            MAX_HCK_LEN);
    if (bOk == false)
    {
        printf("QLIB_SIMLOCK_crypto_run_pbkdf2_algorithm failed\n");
        return false;
    }

    printf("----------------\nTEST\n----------------\n");
    printf("[CK]\t: %s\n", const_ck);
    printf("[Salt]\t: ");
    for (int i=0; i<MAX_SALT_LEN; i++)
    {
        printf("0x%02X ", const_salt[i]);
    }

    printf("\n[HCK]\t: ");
    for (int i=0; i<MAX_HCK_LEN; i++)
    {
        printf("0x%02X ", hck[i]);
    }

    return bOk;
}

unsigned char Test_NV_BackupRestore_Without_NV_Definition_File( void )
{
    unsigned char bOk = true;
    char *qcnPath = "c:\\temp\\QCNBackup.qcn";
    char *xqcnPath = "c:\\temp\\QCNBackup.xqcn";
    char *pQcnPath = qcnPath;
    char cKey = ' ';
    int _iComPort = 0;
      
    typedef enum
    {
        BACKUP_QCN = 0,
        BACKUP_XQCN,
        RESTORE_QCN,
        RESTORE_XQCN
    }QCN_OPERATION;

    QCN_OPERATION qcnQpration = BACKUP_QCN;

    printf("----------------\nQCN backup and restore Test\n----------------\n");

    printf("\n");
    printf("\n0=Backup qcn at C:\\temp\\QCNBackup.qcn");
    printf("\n1=Backup xqcn at C:\\temp\\QCNBackup.xqcn");
    printf("\n2=Restore qcn from C:\\temp\\QCNBackup.qcn");
    printf("\n3=Restore xqcn from C:\\temp\\QCNBackup.xqcn");
    printf("\nPlease select one of above options\n");

    cKey = getch();
    qcnQpration = (QCN_OPERATION)(cKey - '0');

    if (qcnQpration == BACKUP_QCN || qcnQpration == RESTORE_QCN)
    {
        pQcnPath = qcnPath;
    }   
    else
    {
        pQcnPath = xqcnPath;
    }

    //release existing connection if exists
    if ( g_hResourceContext )
    {
        QLIB_StopLogging(g_hResourceContext);
        QLIB_DisconnectServer( g_hResourceContext );
        g_hResourceContext = NULL;
        printf("\nQLIB_DisconnectServer() was called to release the current handle");
        printf("\n\n<<Make sure that QPST has completely closed");
    } // if ( g_hResourceContext )

    //These methods run in QPHONEMS mode
    QLIB_SetLibraryMode( QLIB_LIB_MODE_QPHONEMS );

    // Get the COM port
    printf("\nPlease enter the Diagnostics COM port # : ");
    scanf("%hd", &_iComPort);
    printf("\n-------------QLIB_ConnectServer-------------------\n");
    g_hResourceContext  = QLIB_ConnectServer ( _iComPort );
    if ( ! g_hResourceContext )
    {
        printf("QLIB_ConnectServer failed\n");
        return false;
    }

    printf("\n-------------Send default SPC-------------------\n");
    unsigned char _iSPC[6] = {'0','0','0','0','0','0'};
    unsigned char _iSPC_Result = 0;

    bOk = QLIB_DIAG_SPC_F( g_hResourceContext, _iSPC, &_iSPC_Result );

    printf("\n   Command Status: %s", bOk ? "PASS" : "FAIL\n" );
    printf("\n   SPC Status: %s", _iSPC_Result ? "PASS\n" : "FAIL\n" );
    if (bOk == false)
    {
        return false;
    }

    printf("\n-------------QLIB_NV_SetTargetSupportMultiSIM-------------------\n");
    bOk = QLIB_NV_SetTargetSupportMultiSIM(g_hResourceContext, true);
    if (bOk == false)
    {
        printf("QLIB_NV_SetTargetSupportMultiSIM failed\n");
        return false;
    }
    int iResultCode = 0;
    if (qcnQpration == BACKUP_QCN || qcnQpration == BACKUP_XQCN)
    {
        printf("\nBackuping QCN...");
        printf("\n-------------QLIB_BackupNVFromMobileToQCN-------------------\n");
        bOk = QLIB_BackupNVFromMobileToQCN(g_hResourceContext, pQcnPath, &iResultCode);
        if (bOk == false)
        {
            printf("Failed QLIB_BackupNVFromMobileToQCN, ResultCode = %d\n", iResultCode);
            return false;
        }
        printf("Backup QCN successfully\n");
    }
    else if (qcnQpration == RESTORE_QCN || qcnQpration == RESTORE_XQCN)
    {
        printf("\nRestoring QCN...");
        printf("\n-------------QLIB_NV_LoadNVsFromQCN-------------------\n");
        int iNumOfNVItemValuesLoaded = 0;
        bOk = QLIB_NV_LoadNVsFromQCN(g_hResourceContext, pQcnPath, &iNumOfNVItemValuesLoaded, &iResultCode);
        if (bOk == false)
        {
            printf("Failed QLIB_NV_LoadNVsFromQCN, ResultCode = %d\n", iResultCode);
            return false;
        }
        printf("\n-------------QLIB_NV_WriteNVsToMobile-------------------\n");
        bOk = QLIB_NV_WriteNVsToMobile(g_hResourceContext, &iResultCode);
        if (bOk == false)
        {
            printf("Failed QLIB_NV_WriteNVsToMobile, ResultCode = %d\n", iResultCode);
            return false;
        }

        printf("Restore QCN successfully\n");
    }

    return bOk;
}


unsigned char Test_Library( void )
{
   unsigned char bOk = true;
   char cKey = ' ';

   printf("----------------\nLibrary Test\n----------------\n");

   printf("\n");
   printf("\n1=Test a variety of commands in sequence (until key is pressed)" );
   printf("\n2=Benchmark Test" );
   printf("\n3=Tx ON/OFF" );
   cKey = getch();

   if (cKey == '1' )
   {
      while (!kbhit() )
      {
         unsigned short iADC_Value = 0;

         bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_FTM_F );
         printf("\nQLIB_DIAG_CONTROL_F(  MODE_FTM_F ): %s", bOk ? "PASS" : "FAIL\n" );

         if (!bOk )
            break;

         printf("\nCommand Variety Test");

         bOk = QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_WCDMA_IMT );
         printf("\nQLIB_FTM_SET_MODE: %s", bOk ? "PASS" : "FAIL");

         bOk = QLIB_FTM_SET_CHAN( g_hResourceContext, 9613);
         printf("\nQLIB_FTM_SET_CHAN: %s", bOk ? "PASS" : "FAIL");

         // Set Tx AGC PDM to 300
         bOk = QLIB_FTM_SET_PDM(g_hResourceContext, 2, 300);
         printf("\nQLIB_FTM_SET_PDM: %s", bOk ? "PASS" : "FAIL");

         // PA Range
         bOk = QLIB_FTM_SET_PA_RANGE( g_hResourceContext, 0 );
         printf("\nQLIB_FTM_SET_PA_RANGE: %s", bOk ? "PASS" : "FAIL");

         // Set Tx On
         bOk = QLIB_FTM_SET_TX_ON( g_hResourceContext );
         printf("\nQLIB_FTM_SET_TX_ON: %s", bOk ? "PASS" : "FAIL");

         bOk = QLIB_FTM_CDMA_CW_WAVEFORM( g_hResourceContext, 0 );
         printf("\nQLIB_FTM_CDMA_CW_WAVEFORM: %s", bOk ? "PASS" : "FAIL");

         bOk = QLIB_FTM_CDMA_CW_WAVEFORM( g_hResourceContext, 1 );
         printf("\nQLIB_FTM_CDMA_CW_WAVEFORM: %s", bOk ? "PASS" : "FAIL");

         // Read ADC on channel 5
         bOk = QLIB_FTM_GET_ADC_VAL( g_hResourceContext, 5, &iADC_Value );
         printf("\nQLIB_FTM_GET_ADC_VAL: %s", bOk ? "PASS" : "FAIL");

         bOk &= QLIB_FTM_TX_SWEEP_CAL( g_hResourceContext, 1 );     // On
         printf("\nQLIB_FTM_TX_SWEEP_CAL(1): %s", bOk ? "PASS" : "FAIL");

         bOk &= QLIB_FTM_TX_SWEEP_CAL( g_hResourceContext, 0 );     // Off
         printf("\nQLIB_FTM_TX_SWEEP_CAL(0): %s", bOk ? "PASS" : "FAIL");

         // Read the first HDET value
         unsigned short iHDETvalue;
         bOk &= QLIB_FTM_GET_HDET_FROM_TX_SWEEP_CAL( g_hResourceContext, 0, &iHDETvalue );
         printf("\nQLIB_FTM_GET_HDET_FROM_TX_SWEEP_CAL: %s", bOk ? "PASS" : "FAIL");

         // Set Tx On
         bOk &= QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         printf("\nQLIB_FTM_SET_TX_OFF: %s", bOk ? "PASS" : "FAIL");

         double _dRxPowerLevels[] = {-73.0, -69.0, -48.0,  -32.0 };
         short _iRxAgcLevels[]    = {-115,  -67,   184,    376   };

         // Get the DVGA Offset
         short iExpectedRX_AGC = _iRxAgcLevels[0];
         short iDVGAoffset;
         short iLNAoffsetValue;
         bOk &= QLIB_FTM_GET_DVGA_OFFSET( g_hResourceContext, iExpectedRX_AGC, &iDVGAoffset );
         printf("\nQLIB_FTM_GET_DVGA_OFFSET: %s", bOk ? "PASS" : "FAIL");

         // Get the LNA offset
         for (int iLnaRange = 1; iLnaRange <=3; iLnaRange++)
         {
            bOk &= QLIB_FTM_GET_LNA_OFFSET( g_hResourceContext, iLnaRange, _iRxAgcLevels[iLnaRange], &iLNAoffsetValue );
            printf("\nQLIB_FTM_GET_LNA_OFFSET: %s", bOk ? "PASS" : "FAIL");
         }

         unsigned short iRxAGC = 0;

         // Get the RX AGC Value
         QLIB_FTM_GET_RX_AGC( g_hResourceContext, &iRxAGC );
      } //  while (!kbhit() )

      // clear keypress from queue
      getch();
   }    // cKey = '1'
   else if (cKey == '2' )
   {
      printf("\nBenchmark");
   }    // cKey = '2'
   else if (cKey == '3' )
   {
      unsigned short iStartDAC = 300;
      char _sLogFile[] = "c:\\factory_log.txt";

      printf("\nTest Tx on/off");

      // RF Mode, WCDMA - IMT
      bOk &= QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_GSM_900 );
      printf("\nRF FTM Mode = GSM 900");

      // Set the frequency channel
      bOk &= QLIB_FTM_SET_CHAN( g_hResourceContext, 1 );

      int _iCounter = 0;

      ////////////////////////////
      // Start setup of phone async logging

      // Setup to log all FTM messages
      unsigned long _iSSID = 23;    // 23 = FTM sub system
      long _iRT_MASK = -1;      // -1 = all messages

      StartAsyncCapture( _iSSID, _iRT_MASK );

      // Setup to log all legacy messages
      _iSSID = 0;           // 0 = Legacy sub system
      _iRT_MASK = -1;       // -1 = all messages

      StartAsyncCapture( _iSSID, _iRT_MASK );

      // End setup of phone async logging
      ////////////////////////////

      ////////////////////////////
      // Start setup of TXT file logging

      // Set the log flag conditions
      // Allow the pre-existing settings to remain
      // QLIB_SetLogFlags( g_hResourceContext,  LOG_IO | LOG_FN | LOG_ASYNC );

      // Activate text logging
      bOk = QLIB_StartLogging( g_hResourceContext,  _sLogFile );
      printf("\nQLIB_StartLogging: %s", bOk ? "PASS" : "FAIL" );

      // End setup of TXT file logging
      ////////////////////////////

      while (!kbhit() && bOk )
      {
         printf("========================\nPass %d", ++_iCounter );

         // Set Tx On
         bOk = QLIB_FTM_SET_TX_ON( g_hResourceContext );
         printf("\nQLIB_FTM_SET_TX_ON: %s", bOk ? "PASS" : "FAIL" );

         bOk &= QLIB_FTM_SET_PA_DAC_INPUT( g_hResourceContext, iStartDAC );

         printf("\nQLIB_FTM_SET_PA_DAC_INPUT: %s", (bOk ? "PASS" : "FAIL") );

         // Loop through a range of DAC values
         for ( int iPowerLevelIndex = 0; iPowerLevelIndex <=14; iPowerLevelIndex++)
         {
            // Set the DAC value
            bOk &= QLIB_FTM_SET_TX_POWER_LEVEL(g_hResourceContext, iPowerLevelIndex );

            printf(" \nPower Index= %d...%s", iPowerLevelIndex, bOk ? "PASS" : "FAIL" );
         }

         // Set Tx On
         bOk &= QLIB_FTM_SET_TX_OFF( g_hResourceContext );
         printf("\nQLIB_FTM_SET_TX_OFF: %s", bOk ? "PASS" : "FAIL" );

         DumpAsync();
      }

      // clear keypress from queue
      getch();
   }    // cKey = '2'

   return bOk;
}

unsigned char Test_Security( void )
{
   unsigned char bOk = true;
   char cKey = ' ';

   // Root key length
   const int c_iRootKeyMaxLen = 260;

   // SIM data length
   const unsigned long c_iSimSecureLen = 515;

   // Values common to all commands
   unsigned char _iSlotId = 0x01;
   unsigned char _iSIM_AppIdLen = 16;
   unsigned char _aiSIM_AppId[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   unsigned long _iCommandStatus = 0;
   unsigned short _iTransactionID;
   unsigned long _iStatus = 0;
   char _sLogFile[] = "c:\\factory_log_mmgdsi.txt";

   printf("----------------\nSecurity (GSDI)\n----------------\n");

   printf("\n");
   printf("\n1=GSDIDIAG_ROOT_KEY_WRITE_CMD" );
   printf("\n2=GSDIDIAG_ROOT_KEY_READ_CMD" );
   printf("\n3=GSDIDIAG_SECURE_DEVICE_INFO_CMD" );
   printf("\n4=GSDIDIAG_SIM_SECURE_STORE_CMD (%d bytes)", c_iSimSecureLen );
   printf("\n5=GSDIDIAG_SIM_SECURE_READ_CMD" );
   printf("\n6=GSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD (Virtual SIM)");
   printf("\n7=Set IMEI={1 2 3 4 5 6 7 8 9}");
   printf("\n8=Set DEVICE_SERIAL_NO={1 2 3 4 5 6 7 8 9 a b c d e f}");
   printf("\n9=Read DEVICE_SERIAL_NO");
   printf("\nA=QLIB_GSDIDIAG_GET_FEATURE_INDICATION_CMD");
   printf("\nB=Test several event-based GSDI commands");
   printf("\nC=QLIB_MMGSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD_FixedConfig (Virtual SIM)");
   printf("\nD=Procedure for security provisioning and shut down (with SIMLOCK write)" );
   printf("\nE=Procedure for security provisioning and shut down (without SIMLOCK write)" );
   printf("\nF=Report SIM Applications" );
   printf("\nG=Write Dual SIM IMEI" );
   printf("\nH=Read Dual SIM IMEI" );

   cKey = toupper( getch() );

   // Set the log flag conditions
   QLIB_SetLogFlags( g_hResourceContext,  LOG_DEFAULT );

   // Activate text logging
   bOk = QLIB_StartLogging( g_hResourceContext,  _sLogFile );

   // Set async capture of legacy messages...2nd parameter - 0=none, -1=all
   StartAsyncCapture( 0, 0);

   if (cKey == '1' )
   {
      const int c_iActualRootKeyLen = 260;

      /*
      // Root key data
      unsigned char _aiRootKeyData[ c_iActualRootKeyLen ] =
      {
      0x01, 0x02, 0x03, 0x04, 0xB8, 0x15, 0x71, 0x47, 0xAD, 0xF9, 0x1C, 0xA3, 0xC7, 0x3E, 0x0F, 0x61, 0x60, 0x76,
      0x64, 0xED, 0xF5, 0x46, 0x3C, 0x9B, 0xB1, 0xF7, 0xC8, 0x6E, 0x24, 0xBE, 0x47, 0x68, 0xEA, 0x6A, 0xA3, 0x04,
      0x47, 0x86, 0xCB, 0x6A, 0x72, 0x70, 0xB0, 0x19, 0x60, 0xA2, 0x9D, 0x81, 0x42, 0x49, 0x63, 0x28, 0x3A, 0xA6,
      0xA7, 0x0C, 0x8F, 0x31, 0x65, 0xF0, 0x72, 0xC3, 0xCA, 0x04, 0xD4, 0x4F, 0x3E, 0xC2, 0xF6, 0x74, 0x59, 0x18,
      0x3F, 0x76, 0x24, 0x09, 0xBD, 0x5C, 0x40, 0x27, 0x08, 0x74, 0xCF, 0x80, 0x00, 0x3F, 0x29, 0x99, 0x5B, 0xD9,
      0xF8, 0x03, 0x54, 0xD3, 0x43, 0xB8, 0x9A, 0x62, 0x55, 0x96, 0xA3, 0x04, 0xF6, 0xC9, 0xD4, 0xA6, 0x1B, 0x69,
      0xA9, 0xCA, 0xC4, 0x1B, 0x5B, 0xF9, 0x63, 0x9F, 0xFC, 0xDF, 0x77, 0x45, 0x51, 0x7F, 0x19, 0xD1, 0x3F, 0x4E,
      0x21, 0x85, 0xB1, 0x45, 0x42, 0xDE, 0x9F, 0x3B, 0x74, 0xDF, 0xD1, 0x73, 0x42, 0xFD, 0x50, 0x16, 0x9D, 0xE2,
      0x77, 0x6C, 0xF4, 0xEF, 0x3A, 0x94, 0x8D, 0x4D, 0x46, 0x3D, 0xB8, 0x3B, 0x43, 0x2D, 0xA2, 0x22, 0x66, 0x7B,
      0xC9, 0x38, 0x24, 0x62, 0x1B, 0x0F, 0x83, 0x0F, 0x57, 0xE8, 0x38, 0x23, 0x79, 0x1B, 0xC4, 0xB5, 0xB0, 0x55,
      0xBE, 0x23, 0x75, 0xA8, 0x17, 0x72, 0x48, 0xD2, 0xDB, 0x08, 0xFD, 0xEC, 0x3E, 0x75, 0x98, 0x1F, 0x79, 0xBC,
      0x1B, 0x92, 0xAD, 0xEA, 0x18, 0x5C, 0xCC, 0xCB, 0xD3, 0xF8, 0x8B, 0x41, 0x6B, 0x11, 0xEB, 0x73, 0x77, 0x8F,
      0xC7, 0x44, 0x0B, 0x4E, 0x67, 0x2A, 0x46, 0x5F, 0xBD, 0xF8, 0x63, 0xB7, 0x32, 0x5A, 0xA2, 0x17, 0x30, 0x5D,
      0x83, 0x9C, 0xB5, 0x0C, 0x78, 0xFE, 0xFA, 0xFB, 0x2B, 0x73, 0x1E, 0x9A, 0x0D, 0x44, 0x51, 0x44, 0x0C, 0xAC,
      0xDC, 0x8B, 0x32, 0xA9, 0xDB, 0x5B, 0x05, 0xBD
      };
      */

      // Root key from Aug 2005
      unsigned char _aiRootKeyData[ c_iActualRootKeyLen ] =
      {
         0x01, 0x02, 0x03, 0x04, 0xE4, 0x3F, 0xDF, 0x73, 0x89, 0x85, 0xA2, 0xD0, 0x66, 0x4B, 0x2D, 0x56, 0xC2, 0x5D, 0x43, 0xEC, 0x61,
         0xFB, 0xE6, 0x66, 0x06, 0xAD, 0x3E, 0x93, 0x75, 0x23, 0xC0, 0x76, 0x0D, 0x6F, 0xC5, 0xB7, 0xB2, 0x02, 0x72, 0x7C, 0x4A, 0x32,
         0x69, 0xB3, 0x99, 0x31, 0xAB, 0x95, 0x65, 0x88, 0xEC, 0x2C, 0xE8, 0x3E, 0xCF, 0x2F, 0xDE, 0x71, 0xC8, 0xDA, 0x75, 0x3F,
         0x8C, 0x14, 0xC3, 0x46, 0x86, 0x86, 0xE3, 0xFF, 0x7D, 0xBB, 0x18, 0xE1, 0x09, 0x76, 0x7C, 0x99,
         0x79, 0xFF, 0x86, 0x74, 0x7B, 0xF0, 0xE8, 0xFB, 0x4E, 0x73, 0xFF, 0xB7, 0x27, 0x7C, 0x8F, 0x3F,
         0x8F, 0x38, 0x06, 0x9B, 0xEF, 0x20, 0x53, 0x32, 0x89, 0x86, 0xDD, 0xA6, 0xB5, 0xEB, 0x55, 0x7E,
         0x64, 0x1E, 0x6D, 0x8E, 0x06, 0xBA, 0x17, 0xA3, 0x3C, 0xDB, 0x08, 0xC6, 0x07, 0xE5, 0xE6, 0x7A,
         0x1D, 0x42, 0x66, 0xEF, 0x38, 0x7C, 0xD6, 0x67, 0x35, 0xB4, 0x9A, 0x47, 0xA6, 0xB6, 0x1C, 0xFD, 0xE2,
         0xDF, 0xEA, 0x99, 0xCD, 0x07, 0xD7, 0x3F, 0x11, 0x36, 0xBF, 0xA6, 0xAD, 0x50, 0xF3, 0x67, 0x79, 0x0F,
         0x44, 0xD8, 0x57, 0x92, 0xAF, 0x44, 0x32, 0xBB, 0x16, 0x2F, 0x0B, 0x6D, 0x3E, 0xF8, 0x7C, 0xCF,
         0xB5, 0x07, 0xA8, 0x62, 0x3D, 0x3B, 0x4D, 0x07, 0x33, 0xC4, 0x5A, 0xDD, 0xCB, 0xEE, 0x91, 0xCC, 0x76,
         0x28, 0xE0, 0x48, 0x49, 0x2C, 0x48, 0xBE, 0xD0, 0x9F, 0xB4, 0x83, 0xD1, 0xA2, 0x7F, 0x04, 0x4F, 0xD2,
         0x92, 0x0F, 0x5B, 0xF5, 0x88, 0x8C, 0xBD, 0x43, 0x21, 0x86, 0x98, 0x7B, 0xA5, 0x9F, 0x29, 0xB5,
         0xEA, 0xBE, 0x0E, 0x77, 0xE2, 0xFE, 0xC9, 0x4D, 0xD6, 0xF4, 0x71, 0xCD, 0xDE, 0xEE, 0x64, 0xA1, 0x99,
         0x7C, 0xA5, 0x1F, 0xFF, 0xF2, 0x12, 0x63, 0x8E, 0xA8, 0x97, 0xE5, 0x77, 0x8C, 0x78, 0xF4, 0xE6, 0x1F
      };

      // Send GSDI command
      bOk = QLIB_GSDIDIAG_ROOT_KEY_WRITE_CMD
            (
               g_hResourceContext, _iSlotId, _iSIM_AppIdLen, _aiSIM_AppId, c_iActualRootKeyLen,
               _aiRootKeyData, &_iCommandStatus, &_iStatus
            );

      printf("\nQLIB_GSDIDIAG_ROOT_KEY_WRITE_CMD: %s", bOk ? "PASS" : "FAIL" );
      printf("\n Command Status: %d", _iCommandStatus );
      printf("\n Status: %d\n", _iStatus );
   }    // cKey = '1'
   else if (cKey == '2' )
   {
      // Root key data
      unsigned char _aiRootKeyData[ c_iRootKeyMaxLen ];

      // Root key length, variable
      unsigned long _iRootKeyLen = 0;

      // Send GSDI command
      bOk = QLIB_GSDIDIAG_ROOT_KEY_READ_CMD
            (
               g_hResourceContext, _iSlotId, _iSIM_AppIdLen, _aiSIM_AppId, &_iRootKeyLen,
               _aiRootKeyData, &_iCommandStatus, &_iStatus
            );

      printf("\nQLIB_GSDIDIAG_ROOT_KEY_READ_CMD: %s", bOk ? "PASS" : "FAIL" );
      printf("\n Command Status: %d", _iCommandStatus );

      if (bOk)
      {
         printf("\n Root Key Length: %d\n", _iRootKeyLen );
         printf("\n Data: \n" );
         for (unsigned long _iIndex = 1; _iIndex < _iRootKeyLen; _iIndex++)
         {
            printf("0x%X ", _aiRootKeyData[_iIndex] );
         }
      }
   }    // cKey == '2'
   else if (cKey == '3' )
   {
      // printf("\n3=GSDIDIAG_SECURE_DEVICE_INFO_CMD" );

      // Send GSDI command
      bOk = QLIB_GSDIDIAG_SECURE_DEVICE_INFO_CMD
            (
               g_hResourceContext, _iSlotId, _iSIM_AppIdLen, _aiSIM_AppId, &_iCommandStatus, &_iStatus
            );

      printf("\nQLIB_GSDIDIAG_SECURE_DEVICE_INFO_CMD: %s", bOk ? "PASS" : "FAIL" );
      printf("\n Command Status: %d", _iCommandStatus );
      printf("\n Status: %d\n", _iStatus );
   }
   else if (cKey == '4' )
   {
      // printf("\n4=GSDIDIAG_SIM_SECURE_STORE_CMD" );

      // Root key data
      unsigned char _aiSimSecureData[ c_iSimSecureLen ] =
      {
         0x02, 0x00, 0x00, 0x77, 0x70, 0x35, 0xfa, 0x34, 0x2e, 0x33, 0x7b, 0xfc, 0x39, 0x18, 0x14, 0xe6, 0x7c,
         0x76, 0x20, 0xcf, 0x2a, 0xcb, 0x2f, 0x7a, 0x4f, 0xb8, 0x59, 0xc0, 0x24, 0x8a, 0xfb, 0xba, 0x9c,
         0xec, 0xdf, 0x77, 0x70, 0x35, 0xfa, 0x34, 0x2e, 0x33, 0x7b, 0x01, 0x0e, 0x5e, 0x74, 0x1d, 0xa9,
         0xef, 0x06, 0xd5, 0x7b, 0x02, 0xa0, 0x23, 0x5d, 0xab, 0x6c, 0x00, 0xc2, 0xb7, 0x13, 0x81, 0xcf,
         0xfb, 0x83, 0xf4, 0x9b, 0x6a, 0x61, 0x1c, 0xbe, 0x31, 0x6b, 0x46, 0xdd, 0x47, 0x08, 0x44, 0xf6,
         0x6b, 0x4c, 0x71, 0xa3, 0xd8, 0x5d, 0x70, 0xfb, 0x64, 0xe5, 0x65, 0x5f, 0xf8, 0x31, 0xb3, 0x3b,
         0x7a, 0x02, 0xba, 0x2a, 0xb0, 0xf4, 0xe6, 0x8e, 0x1d, 0x9c, 0x74, 0xb3, 0xfd, 0x76, 0x0f, 0x58,
         0x90, 0x73, 0xa4, 0x6f, 0x11, 0xf3, 0x59, 0x8c, 0x79, 0x5b, 0x81, 0x84, 0x72, 0x72, 0x7c, 0x19,
         0xf0, 0xa3, 0xc8, 0xad, 0xb0, 0x28, 0xd1, 0xa4, 0x69, 0x3b, 0xc8, 0x59, 0x9e, 0xea, 0xa4, 0xfa,
         0x7c, 0xeb, 0x65, 0x30, 0xd4, 0x90, 0x2d, 0xc9, 0x81, 0xf5, 0x52, 0x21, 0x17, 0x2c, 0x0c, 0x15,
         0xe0, 0x86, 0x52, 0x91, 0x91, 0x11, 0x11, 0x58, 0x87, 0xbc, 0xf1, 0xaa, 0xbe, 0xe0, 0xc3, 0x83,
         0x49, 0x65, 0x89, 0x38, 0x6f, 0xd1, 0x5e, 0x70, 0xcf, 0xd2, 0x8c, 0x73, 0xe5, 0x05, 0x2d, 0x58,
         0x21, 0xec, 0x48, 0xb5, 0x4a, 0xfc, 0x1f, 0x1a, 0x11, 0x22, 0x5b, 0x38, 0xb2, 0x64, 0x7b, 0xd2,
         0x4d, 0xd2, 0x11, 0x9e, 0x4b, 0xd4, 0xeb, 0xb8, 0xda, 0x9e, 0x8b, 0x1b, 0x00, 0x04, 0xfe, 0xbd,
         0xfa, 0xd0, 0x77, 0x70, 0x35, 0xfa, 0x34, 0x2e, 0x33, 0x7b, 0x01, 0x0e, 0x5e, 0x8b, 0xb0, 0xcf,
         0xd2, 0x31, 0xe8, 0xcf, 0xe1, 0xcf, 0xfa, 0xd9, 0xed, 0x1d, 0x82, 0x7d, 0x5f, 0x11, 0x01, 0xac,
         0x61, 0x94, 0xc1, 0xf3, 0xe6, 0x4d, 0x07, 0x33, 0x1f, 0xbc, 0x54, 0x5a, 0x28, 0xcb, 0x14, 0x4c,
         0x93, 0xba, 0x86, 0x41, 0xd2, 0xb8, 0xc1, 0x34, 0xa8, 0x87, 0xaf, 0xf3, 0xb7, 0x14, 0xa6, 0x54,
         0xbf, 0x49, 0x24, 0x26, 0x27, 0xa6, 0x01, 0xf3, 0x93, 0xe4, 0xd2, 0x72, 0xcf, 0xff, 0xc7, 0xb7,
         0xc8, 0xec, 0x07, 0xca, 0x6f, 0x43, 0x41, 0x87, 0x82, 0xdb, 0x02, 0xff, 0xe4, 0x59, 0x69, 0xc3,
         0xce, 0x82, 0x8d, 0x2a, 0xd7, 0x4d, 0xeb, 0xfa, 0xf6, 0x90, 0x3b, 0x11, 0xa0, 0x75, 0x38, 0x08,
         0xfa, 0x65, 0x28, 0x25, 0x14, 0x72, 0xfc, 0x91, 0xb9, 0xba, 0xe4, 0x0c, 0x55, 0x7e, 0x39, 0xcd,
         0x6a, 0x44, 0x67, 0x3c, 0x02, 0xf8, 0xac, 0xac, 0x84, 0xe3, 0xec, 0x62, 0x44, 0x56, 0x7b, 0x44,
         0x69, 0x8e, 0xd4, 0x96, 0xd4, 0x7d, 0xa4, 0x2b, 0xa0, 0xeb, 0xe3, 0x11, 0xec, 0xe5, 0x35, 0x82,
         0x47, 0x1b, 0x8d, 0x32, 0xea, 0xed, 0x41, 0x21, 0xb6, 0xa4, 0x2a, 0x63, 0xd9, 0x5c, 0x35, 0x37,
         0xab, 0xec, 0x99, 0xaf, 0xbe, 0x8b, 0xc0, 0x02, 0x8c, 0x79, 0x28, 0x5c, 0xd7, 0x6a, 0x96, 0xef,
         0x49, 0xb7, 0x0a, 0x08, 0xe3, 0xb5, 0xd2, 0x23, 0xf3, 0xb0, 0x8e, 0xb5, 0x23, 0x65, 0x54, 0xd6,
         0x2d, 0x89, 0xc0, 0x06, 0x65, 0x99, 0xfd, 0xac, 0x7b, 0xca, 0xd6, 0xc0, 0x71, 0xc8, 0x6a, 0x73,
         0x7e, 0x2d, 0x6d, 0x04, 0x00, 0x00, 0xff, 0xbe, 0xd5, 0xec, 0x2a, 0x73, 0x44, 0x9a, 0xcc, 0x0b,
         0x84, 0x81, 0x7c, 0xd5, 0xa9, 0x62, 0x0d, 0x53, 0xd6, 0x89, 0x7d, 0x9c, 0xa9, 0xde, 0xbc, 0xe6,
         0x66, 0x74, 0x7e, 0xc8, 0x78, 0x8e, 0x4c, 0xf7, 0x63, 0xec, 0x12, 0xa5, 0xd1, 0xb4, 0xae, 0x81,
         0xe4, 0xd6, 0x78, 0x39, 0x45, 0x2c, 0xe3, 0x38, 0xe5, 0x71, 0x65, 0x6e, 0xc9, 0xe5, 0x3f, 0x8a,
         0x60, 0x9f
      };

      printf("\n Sending SIM secure data");
      // Fill in root key data
      for (int _iKeyIndex = 0;  _iKeyIndex < c_iSimSecureLen; _iKeyIndex++)
         printf(" 0x%X", _aiSimSecureData[ _iKeyIndex ]);

      // Send GSDI command
      bOk = QLIB_GSDIDIAG_SIM_SECURE_STORE_CMD
            (
               g_hResourceContext, _iSlotId, _iSIM_AppIdLen, _aiSIM_AppId, c_iSimSecureLen,
               _aiSimSecureData, &_iCommandStatus, &_iStatus
            );

      printf("\nQLIB_GSDIDIAG_SIM_SECURE_STORE_CMD: %s", bOk ? "PASS" : "FAIL" );
      printf("\n Command Status: %d", _iCommandStatus );
      printf("\n Status: %d\n", _iStatus );
   }
   else if (cKey == '5' )
   {
      // printf("\n5=GSDIDIAG_SIM_SECURE_READ_CMD" );

      // SIM data length
      unsigned long _iSimSecureLen = 500;

      // Root key data
      unsigned char _aiSimSecureData[ c_iSimSecureLen ];

      // Send GSDI command
      bOk = QLIB_GSDIDIAG_SIM_SECURE_READ_CMD
            (
               g_hResourceContext, _iSlotId, _iSIM_AppIdLen, _aiSIM_AppId, &_iSimSecureLen,
               _aiSimSecureData, &_iCommandStatus, &_iStatus
            );

      printf("\nQLIB_GSDIDIAG_SIM_SECURE_STORE_CMD: %s", bOk ? "PASS" : "FAIL" );
      printf("\n Command Status: %d", _iCommandStatus );
      printf("\n Status: %d\n", _iStatus );

      if (bOk)
      {
         printf("\n SIM Secure Length: %d\n", _iSimSecureLen );
         printf("\n SIM Secure Data: \n" );
         for (unsigned long _iIndex = 1; _iIndex < _iSimSecureLen; _iIndex++)
         {
            printf("0x%X ", _aiSimSecureData[_iIndex] );
         }
      }
   }

   else if (cKey == '6' )
   {
      // printf("\n6=GSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD (Virtual SIM)");

      // Send GSDI command
      bOk = QLIB_GSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD
            (
               g_hResourceContext, _iSlotId, _iSIM_AppIdLen, _aiSIM_AppId, &_iCommandStatus, &_iStatus
            );

      printf("\nGSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD: %s", bOk ? "PASS" : "FAIL" );
      printf("\n Command Status: %d", _iCommandStatus );
      printf("\n Status: %d\n", _iStatus );
   }
   else if (cKey == '7' )
   {
      // printf("\n7=Set IMEI={1 2 3 4 5 6 7 8 9}");

      const unsigned short c_iIMEI_id = 550;
      const unsigned short c_iIMEI_size = 9;
      unsigned char _aIMEI[ c_iIMEI_size] = {8, 1, 2, 3, 4, 5, 6, 7, 8 };
      unsigned short _iStatus;
      if ( QLIB_DIAG_NV_WRITE_F( g_hResourceContext, c_iIMEI_id, _aIMEI, c_iIMEI_size, &_iStatus ) )
      {
         printf("NV NV Write(%d)=0: PASS\n",c_iIMEI_id);

         // Check the status
         if ( _iStatus == NV_DONE_S )
         {
            printf("NV Write Status: PASS\n");
         }
         else
         {
            printf("NV Write Status: FAIL\n");
         }
      }
      else
         printf("NV Write: FAIL\n");
   }
   else if (cKey == '8' )
   {
      // printf("\n8=Set DEVICE_SERIAL_NO={1 2 3 4 5 6 7 8 9 a b c d e f}");
      const unsigned short c_iDSN_id = 2824;
      const unsigned short c_iDSN_size = 16;
      unsigned char _aDSN[ c_iDSN_size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 , 11, 12, 13, 14, 15};
      unsigned short _iStatus;
      if ( QLIB_DIAG_NV_WRITE_F( g_hResourceContext, c_iDSN_id, _aDSN, c_iDSN_size, &_iStatus ) )
      {
         printf("NV NV Write(%d)=0: PASS\n",c_iDSN_id);

         // Check the status
         if ( _iStatus == NV_DONE_S )
         {
            printf("NV Write Status: PASS\n");
         }
         else
         {
            printf("NV Write Status: FAIL\n");
         }
      }
      else
         printf("NV Write: FAIL\n");

      /*
      unsigned short _iDSN_size;
      if ( QLIB_DIAG_NV_READ_F( g_hResourceContext, c_iDSN_id, _aDSN, &_iDSN_size, &_iStatus ) )
      {
      printf("NV NV Read (%d)=0: PASS\n",c_iDSN_id);

      // Check the status
      if ( _iStatus == NV_DONE_S )
      {
      printf("NV Read Status: PASS\n");

      printf("\n IMEI Data: \n" );
      for (unsigned long _iIndex = 1; _iIndex < _iSimSecureLen; _iIndex++)
      {
      printf("0x%X ", _aDsn[_iIndex] );
      }
      }
      else
      {
      printf("NV Read Status: FAIL\n");
      }
      }
      else
      printf("NV Read : FAIL\n");
      */
   }

   else if (cKey == '9' )
   {
      // printf("\n9=Read DEVICE_SERIAL_NO");
      const unsigned short c_iDSN_id = 2824;
      const unsigned short c_iDSN_size = 16;
      unsigned char _aDSN[ c_iDSN_size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 , 11, 12, 13, 14, 15};
      unsigned short _iStatus;

      if ( QLIB_DIAG_NV_READ_F( g_hResourceContext, c_iDSN_id, _aDSN, c_iDSN_size, &_iStatus ) )
      {
         printf("NV NV Read (%d)=0: PASS\n",c_iDSN_id);

         // Check the status
         if ( _iStatus == NV_DONE_S )
         {
            printf("NV Read Status: PASS\n");

            printf("\n Device SN Data: \n" );
            for (unsigned long _iIndex = 1; _iIndex < c_iDSN_size; _iIndex++)
            {
               printf("0x%X ", _aDSN[_iIndex] );
            }
         }
         else
         {
            printf("NV Read Status: FAIL\n");
         }
      }
      else
         printf("NV Read : FAIL\n");
   }

   else if (cKey == 'A' )
   {
      // printf("\nA=QLIB_GSDIDIAG_GET_FEATURE_INDICATION_CMD");

      printf("\n\n1 = Use QMSL specific function");
      printf("\n2 = Use QMSL generic function");
      cKey = toupper( getch() );

      if ( cKey == '1')
      {
         // Call the specific function
         bOk = QLIB_GSDIDIAG_GET_FEATURE_INDICATION_CMD
               (
                  g_hResourceContext,
                  _iSlotId,
                  &_iTransactionID,
                  &_iCommandStatus
               );
         printf("\nQLIB_GSDIDIAG_GET_FEATURE_INDICATION_CMD: %s, transaction: %d, status: %d",
                bOk ? "PASS" : "FAIL",
                _iTransactionID,
                _iCommandStatus);
      }
      else
      {
         // Call the generic function for sending GSDI event requests
         bOk = QLIB_GSDIDIAG_GET_FEATURE_INDICATION_CMD
               (
                  g_hResourceContext,
                  _iSlotId,
                  &_iTransactionID,
                  &_iCommandStatus
               );

         // No parameters--
         QLIB_GSDIDIAG_SendUserDefineEventResponseCommand(
            g_hResourceContext,
            _GSDIDIAG_GET_FEATURE_INDICATION_CMD,   // unsigned short iSUBSYS_CMD_CODE,
            _iSlotId,
            NULL,   //  (void*) _aiUserRequestData,
            0,      // iUserRequestDataSize,
            &_iTransactionID,
            &_iCommandStatus
         );
         printf("\nQLIB_GSDIDIAG_SendUserDefineEventResponseCommand: %s, transaction: %d, status: %d",
                bOk ? "PASS" : "FAIL",
                _iTransactionID,
                _iCommandStatus);
      }
      // Get the event response
      if (bOk)
      {
         // Set the event timeout default is 3000ms, this does not need to be done unless a different timeout is desired
         QLIB_ConfigureTimeOut( g_hResourceContext, QMSL_Timeout_GSDI_Event, 3000 );

         // Event structure
         QMSL_Event_Element_Struct _oEvent;

         bOk = QLIB_GSDIDIAG_GetEventResponse(
                  g_hResourceContext,
                  _iTransactionID,
                  EVENT_GSDI_GET_FEATURE_IND,
                  (void*) &_oEvent
               );

         printf("\nQLIB_GSDIDIAG_GetEventResponse: %s", bOk ? "PASS" : "FAIL" );

         // Check to see if the event is found
         if ( bOk )
         {
            // Structure for parsing EVENT_GSDI_GET_FEATURE_IND
            // This structure must be declared by the user because the GSDI event structures are not provided in QLib_Defines.h
            struct EVENT_GSDI_GET_FEATURE_IND_struct
            {
               unsigned long CMD_STATUS;        //!<' GSDI_SUCCESS = success, otherwise failure codes in chapter 5 of GSDI ICD
               unsigned short TRANSACTION_ID;   //!<' Transaction ID returned from request response.
               unsigned char NW_FEATURE_INDICATION; //!<' Indicates if the Network personalization feature is on or off.
               unsigned char NS_FEATURE_INDICATION; //!<' Indicates if the Network Subset personalization feature is on or off:
               unsigned char SP_FEATURE_INDICATION; //!<' Indicates if the Service Provider personalization feature is on or off:
               unsigned char CP_FEATURE_INDICATION; //!<' Indicates if the Corporate personalization feature is on or off:
               unsigned char SIM_FEATURE_INDICATION; //!<' Indicates if the SIM/USIM personalization feature is on or off
            } *_pEventStruct;

            // Assign event structure to the event payload
            _pEventStruct = (EVENT_GSDI_GET_FEATURE_IND_struct*) _oEvent.payload_data;

            // Print values
            printf("\n Event results:");
            printf("\n  NW_FEATURE_INDICATION  = %d", _pEventStruct->NW_FEATURE_INDICATION);
            printf("\n  NS_FEATURE_INDICATION  = %d", _pEventStruct->NS_FEATURE_INDICATION);
            printf("\n  SP_FEATURE_INDICATION  = %d", _pEventStruct->SP_FEATURE_INDICATION);
            printf("\n  CP_FEATURE_INDICATION  = %d", _pEventStruct->CP_FEATURE_INDICATION);
            printf("\n  SIM_FEATURE_INDICATION = %d", _pEventStruct->SIM_FEATURE_INDICATION);
         }  // event is found
      } // if (bOk), gsdi function ok
   }
   else if (cKey == 'B' )
   {
      // printf("\nB=Test several event-based GSDI commands");

      /*
      NOTE: The purpose of this section is to test the synchronous send portion of these commands
      to make sure they are implemented correctly.  The parameters used may not be correct for
      all use cases.
      */

      // Variables used for command parameters
      unsigned long _iSIM_Elementary_File = GSDIDIAG_GSM_LP;
      unsigned char _iSIM_SimpleReq = GSDIDIAG_SIM_SIMPLE_REQ_ALL_STEPS;    // All data is combined into a single output file
      unsigned char _iSIM_IndexMode = GSDIDIAG_NO_INDEX;
      unsigned char _iSIM_PersoFeature = 0x00;  //  unsigned char iSIM_PIN_ID,  // from GSDI_DIAG_PersoFeature_Enum = , 0x00 = PIN1 (SIM/RUIM) PIN1 Application 1 (USIM)
      char* _sSIM_PersoKey = "0000";
      unsigned char _aiSIM_PersoData[512];
      unsigned short _iSIM_PersoDataSize = 0;
      char _sEFS_File[] = "/user/temp.bin";

      // QLIB_GSDIDIAG_VERIFY_PIN_CMD
      bOk = QLIB_GSDIDIAG_VERIFY_PIN_CMD
            (
               g_hResourceContext,
               _iSlotId,
               _iSIM_AppIdLen,
               _aiSIM_AppId,
               _iSIM_PersoFeature,
               "0000",                // char* sSIM_PIN,
               &_iTransactionID,
               &_iCommandStatus
            );
      printf("\nLIB_GSDIDIAG_VERIFY_PIN_CMD: %s, transaction: %d, status: %d",
             bOk ? "PASS" : "FAIL",
             _iTransactionID,
             _iCommandStatus);

      // QLIB_GSDIDIAG_SIM_READ_CMD
      bOk = QLIB_GSDIDIAG_SIM_READ_CMD
            (
               g_hResourceContext,
               _iSlotId,
               _iSIM_AppIdLen,
               _aiSIM_AppId,
               _iSIM_Elementary_File,
               _iSIM_SimpleReq,
               _iSIM_IndexMode,
               0,                 // unsigned short iSIM_Offset,
               0,                 // unsigned short iSIM_RecNum,
               0,                 // unsigned short iSIM_DataLen,
               _sEFS_File,
               &_iTransactionID,
               &_iCommandStatus
            );
      printf("\nQLIB_GSDIDIAG_SIM_READ_CMD: %s, transaction: %d, status: %d",
             bOk ? "PASS" : "FAIL",
             _iTransactionID,
             _iCommandStatus);

      // QLIB_GSDIDIAG_SIM_WRITE_CMD
      bOk = QLIB_GSDIDIAG_SIM_WRITE_CMD
            (
               g_hResourceContext,
               _iSlotId,
               _iSIM_AppIdLen,
               _aiSIM_AppId,
               _iSIM_Elementary_File,
               _iSIM_SimpleReq,
               _iSIM_IndexMode,
               0,                 // unsigned short iSIM_Offset,
               0,                 // unsigned short iSIM_RecNum,
               0,                 // unsigned short iSIM_DataLen,
               _sEFS_File,
               &_iTransactionID,
               &_iCommandStatus
            );
      printf("\nQLIB_GSDIDIAG_SIM_WRITE_CMD: %s, transaction: %d, status: %d",
             bOk ? "PASS" : "FAIL",
             _iTransactionID,
             _iCommandStatus);
      /*
      In testing, this one recieved a status value of 6, even though all parameters in
      request and response packet are OK.

      request:

      cmd: 4B
      subsys: 21
      sub_cmd_code: 01 00
      slot: 01
      app id len: 10
      app: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
      elementary file = 0x00000200: 00 02 00 00
      simple requ: 01
      index mode: 00
      offset: 00 00
      record: 00 00
      data len: 00 00
      efs file: 2F 75 73 65 72 2F 74 65 6D 70 2E 62 69 6E 00

      response:
      4B 21 01 00 06 00 00 00 3A 00 01 10 00 00 00 00 00 00 00 00 00
      00 00 00 00 00 00 00 00 02 00 00 9E 00 00 00 00 00 00 00 2F 75
      73 65 72 2F 74 65 6D 70 2E 62 69 6E 00 C5 A1
      */

      // QLIB_GSDIDIAG_GET_FILE_ATTRIBUTES_CMD
      bOk = QLIB_GSDIDIAG_GET_FILE_ATTRIBUTES_CMD
            (
               g_hResourceContext,
               _iSlotId,
               _iSIM_AppIdLen,
               _aiSIM_AppId,
               _iSIM_Elementary_File,
               _sEFS_File,
               &_iTransactionID,
               &_iCommandStatus
            );
      printf("\nQLIB_GSDIDIAG_GET_FILE_ATTRIBUTES_CMD: %s, transaction: %d, status: %d",
             bOk ? "PASS" : "FAIL",
             _iTransactionID,
             _iCommandStatus);

      // Add a user defined string to the QMSL log
      char _sBuffer[50];
      sprintf( _sBuffer,"Sim Personality Key = '%s'", _sSIM_PersoKey );
      QLIB_AddUserMessageToTextLog( g_hResourceContext, LOG_FN, _sBuffer, FALSE );

      // QLIB_GSDIDIAG_DEACTIVATE_FEATURE_INDICATOR_CMD
      bOk = QLIB_GSDIDIAG_DEACTIVATE_FEATURE_INDICATOR_CMD
            (
               g_hResourceContext,
               _iSlotId,
               _iSIM_PersoFeature,
               _sSIM_PersoKey,
               &_iTransactionID,
               &_iCommandStatus
            );
      printf("\nQLIB_GSDIDIAG_DEACTIVATE_FEATURE_INDICATOR_CMD: %s, transaction: %d, status: %d",
             bOk ? "PASS" : "FAIL",
             _iTransactionID,
             _iCommandStatus);

      // QLIB_GSDIDIAG_GET_FEATURE_INDICATION_CMD
      bOk = QLIB_GSDIDIAG_GET_FEATURE_INDICATION_CMD
            (
               g_hResourceContext,
               _iSlotId,
               &_iTransactionID,
               &_iCommandStatus
            );
      printf("\nQLIB_GSDIDIAG_GET_FEATURE_INDICATION_CMD: %s, transaction: %d, status: %d",
             bOk ? "PASS" : "FAIL",
             _iTransactionID,
             _iCommandStatus);

      // QLIB_GSDIDIAG_SET_FEATURE_INDICATOR_DATA_CMD
      bOk = QLIB_GSDIDIAG_SET_FEATURE_INDICATOR_DATA_CMD
            (
               g_hResourceContext,
               _iSlotId,
               _iSIM_PersoFeature,
               _aiSIM_PersoData,
               _iSIM_PersoDataSize,
               &_iTransactionID,
               &_iCommandStatus
            );
      printf("\nQLIB_GSDIDIAG_SET_FEATURE_INDICATOR_DATA_CMD: %s, transaction: %d, status: %d",
             bOk ? "PASS" : "FAIL",
             _iTransactionID,
             _iCommandStatus);

      // QLIB_GSDIDIAG_GET_FEATURE_INDICATOR_KEY_CMD
      bOk = QLIB_GSDIDIAG_GET_FEATURE_INDICATOR_KEY_CMD
            (
               g_hResourceContext,
               _iSlotId,
               _iSIM_PersoFeature,
               _sEFS_File,
               &_iTransactionID,
               &_iCommandStatus
            );
      printf("\nQLIB_GSDIDIAG_GET_FEATURE_INDICATOR_KEY_CMD: %s, transaction: %d, status: %d",
             bOk ? "PASS" : "FAIL",
             _iTransactionID,
             _iCommandStatus);

      // QLIB_GSDIDIAG_UNBLOCK_FEATURE_INDICATOR_CMD
      bOk = QLIB_GSDIDIAG_UNBLOCK_FEATURE_INDICATOR_CMD
            (
               g_hResourceContext,
               _iSlotId,
               _iSIM_PersoFeature,
               _sSIM_PersoKey,
               &_iTransactionID,
               &_iCommandStatus
            );
      printf("\nQLIB_GSDIDIAG_UNBLOCK_FEATURE_INDICATOR_CMD: %s, transaction: %d, status: %d",
             bOk ? "PASS" : "FAIL",
             _iTransactionID,
             _iCommandStatus);

      // QLIB_GSDIDIAG_GET_PERM_FEATURE_INDICATION_CMD
      bOk = QLIB_GSDIDIAG_GET_PERM_FEATURE_INDICATION_CMD
            (
               g_hResourceContext,
               _iSlotId,
               &_iTransactionID,
               &_iCommandStatus
            );
      printf("\nQLIB_GSDIDIAG_GET_PERM_FEATURE_INDICATION_CMD: %s, transaction: %d, status: %d",
             bOk ? "PASS" : "FAIL",
             _iTransactionID,
             _iCommandStatus);

      // QLIB_GSDIDIAG_PERM_DISABLE_FEATURE_INDICATOR_CMD
      bOk = QLIB_GSDIDIAG_PERM_DISABLE_FEATURE_INDICATOR_CMD
            (
               g_hResourceContext,
               _iSlotId,
               _iSIM_PersoFeature,
               _sSIM_PersoKey,
               &_iTransactionID,
               &_iCommandStatus
            );
      printf("\nQLIB_GSDIDIAG_PERM_DISABLE_FEATURE_INDICATOR_CMD: %s, transaction: %d, status: %d",
             bOk ? "PASS" : "FAIL",
             _iTransactionID,
             _iCommandStatus);

      /*
      QLIB_GSDIDIAG_PERM_DISABLE_FEATURE_INDICATOR_CMD
      (
      HANDLE hResourceContext,
      unsigned char iSlotId,
      unsigned char iSIM_PersoFeature,
      char* sSIM_PersoKey,
      unsigned short* piTransactionID,
      unsigned long* piCommandStatus
      );
      */
   }
   else if (cKey == 'C')
   {
      // printf("\nC=QLIB_MMGSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD_FixedConfig (Virtual SIM)");

      // Send GSDI command
      bOk = QLIB_MMGSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD_FixedConfig
            (
               g_hResourceContext
            );

      printf("\n QLIB_MMGSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD_FixedConfig: %s", bOk ? "PASS" : "FAIL" );
   }
   else if ( (cKey == 'D' ) || ( cKey == 'E') )
   {
      // printf("\nD=Procedure for security provisioning and shut down" );
      // printf("\nE=Procedure for security provisioning and shut down (without SIMLOCK write) );

      /*
      The funciton implements steps 5-15 of the security programming procedure below:

      1) Boot mode pin 3 =1 (Pull up resistor on PCB)
      2) Load AMSS
      3) Phone powers up in FTM mode
      4) RF Calibration

      <<This function starts here>>
      5) Write RF Cal NV, but do not put the phone into OFFLINE mode
      6) Write  primordial seed
      7) Blow all fuses except for trusted.root, master fuse
      8) NV_WRITE NV_FTM_MODE_I = 1
      9) MODE RESET
      10) Phone powers up into FTM mode again
      11) Verify Fuse is blow by performing QFUSE read
      12) Provision IMEI
      13) Blow remaining fuse & master fuse
      14) Program SIM lock (optional)
      15) NV_WRITE NV_FTM_MODE_I = 0
      16) Reset and verify the phone is online
      */

      unsigned char _bWriteSIM_LOCK = cKey == 'D';

      char _sLogFile[] = "c:\\factory_log.txt"; // Location of debug log

      // Setup register values for the first and second fuse blowing operations
      const int c_iConfigRegisterLenMax = 5;
      unsigned long   aConfigRegister_1[c_iConfigRegisterLenMax] = {0,0,0,0,0}; // First fuse blowing
      unsigned long   aConfigRegister_2[c_iConfigRegisterLenMax] = {0,0,0,0,0}; // Second fuse blowing

      // Determine whether this is MSM6xxx or MSM7xxx
      printf("\n\nPlease select the MSM type");
      printf("\n   1 = MSM6xxx");
      printf("\n   2 = MSM7xxx");
      char _cMSM_TypeKey = getch();

      if ( _cMSM_TypeKey == '1' )
      {
         // Values for MSM6xxx, first fuse blow
         aConfigRegister_1[0] = 0x00100000;
         aConfigRegister_1[1] = 0x00000C00;
         aConfigRegister_1[2] = 0x00000000;
         aConfigRegister_1[3] = 0x00000000;
         aConfigRegister_1[4] = 0x00000000;

         // Values for MSM6xxx, second fuse blow
         aConfigRegister_2[0] = 0x00000000;
         aConfigRegister_2[1] = 0x00000200;
         aConfigRegister_2[2] = 0x00000000;
         aConfigRegister_2[3] = 0x00000000;
         aConfigRegister_2[4] = 0x00000000;
      }
      else
      {
         // Values for MSM7xxx, first fuse blow
         aConfigRegister_1[0] = 0x00;
         aConfigRegister_1[1] = 0x00;
         aConfigRegister_1[2] = 0x00;
         aConfigRegister_1[3] = 0x00;
         aConfigRegister_1[4] = 0x00;

         // Values for MSM7xxx, second fuse blow
         aConfigRegister_2[0] = 0x00;
         aConfigRegister_2[1] = 0x00;
         aConfigRegister_2[2] = 0x00;
         aConfigRegister_2[3] = 0x00;
         aConfigRegister_2[4] = 0x00;
      }

      // Set the log flag conditions to defaul
      QLIB_SetLogFlags( g_hResourceContext,  LOG_DEFAULT );

      // Activate text logging
      bOk = QLIB_StartLogging( g_hResourceContext,  _sLogFile );

      unsigned short _iStatus = -1;     // NV Status

      // First go to FTM mode, to simulate having powered up in FTM
      bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_FTM_F );
      printf("\n\nQLIB_DIAG_CONTROL_F(  MODE_FTM_F ): %s", bOk ? "PASS" : "FAIL" );

      ///////////////////////
      // 5) Write RF Cal NV
      ///////////////////////

      // writing NV while the phone is in FTM mode, do not put into OFFLINE mode

      ///////////////////////
      // 6) Write  primordial seed
      ///////////////////////
      const unsigned short c_INV_SEC_CSPRNG_INIT_SEED_id = 4184;
      unsigned long _iSeed = 0x12345678;    // Seed should be a random number
      _iStatus = -1;

      // This will have a failed status if the IMEI has already been written
      if ( QLIB_DIAG_NV_WRITE_F(
               g_hResourceContext,
               c_INV_SEC_CSPRNG_INIT_SEED_id,
               (unsigned char*) &_iSeed,
               sizeof( _iSeed ), &_iStatus )
         )
      {
         printf("\n\nWriting primordial seed...");
         printf("\nNV NV Write(%d)=0: PASS\n",c_INV_SEC_CSPRNG_INIT_SEED_id);

         // Check the status
         if ( _iStatus == NV_DONE_S )
         {
            printf("\nNV Write Status: PASS");
            // Read back primordial seed
            printf("Read primordial seed again to confirm\n");
            unsigned long _iSeedReadBack = 0;
            if( QLIB_DIAG_NV_READ_F (   g_hResourceContext,
                                        c_INV_SEC_CSPRNG_INIT_SEED_id,
                                        (unsigned char*)&_iSeedReadBack,
                                        sizeof(_iSeedReadBack),
                                        &_iStatus))
            {
               if ( _iStatus == NV_DONE_S )
               {
                  printf("\nNV_SEC_CSPRNG_INIT_SEED Read Back 0x%X\n", _iSeedReadBack);
                  printf("\n");
               }
               else
                  printf("NV_SEC_CSPRNG_INIT_SEED Read Back Failed\n");
            }
         }
         else
         {
            printf("\nNV Write Status: FAIL");
         }
      }
      else
         printf("\nNV Write: FAIL");

      ///////////////////////
      // 7) Blow all fuses except for trusted.root, master fuse
      ///////////////////////
      unsigned short    iChainSelect = 0;
      unsigned short iWriteStatus = 1;                  // Set to failure

      printf("\n\nBlowing all fuses except for trusted.root and master fuses...");

      // Setup parameters for X register
      bOk = QLIB_FTM_QFUSE_WRITE
            (
               g_hResourceContext,
               iChainSelect,
               &iWriteStatus,
               aConfigRegister_1
            );
      printf("\nQLIB_FTM_QFUSE_WRITE(  blow fuses ): %s, status = %d", bOk ? "PASS" : "FAIL", iWriteStatus );

      ///////////////////////
      // 8) NV_WRITE NV_FTM_MODE_I = 1
      // 9) MODE RESET
      // 10) Phone powers up into FTM mode again
      ///////////////////////

      printf("\n\nResetting phone so that fuses take effect...");
      // This function sets the phone offline, writes the NV_FTM_MODE_I item, resets
      Test_NV_FTM_Mode( 1);

      ///////////////////////
      // 11) Verify Fuse is blow by performing QFUSE read
      ///////////////////////
      printf("\n\nThe phone should boot up");
      printf("\n\nIssue a QLIB_DIAG_CONTROL_F command to put phone in FTM mode");
      unsigned long _iCurrentTimeOut = QLIB_GetTimeOut( g_hResourceContext, QMSL_Timeout_Delay_ONLINE_FTM);

      bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_Delay_ONLINE_FTM, 500);

      bOk = QLIB_DIAG_CONTROL_F( g_hResourceContext, 3);
      printf("\n\nQLIB_DIAG_CONTROL( FTM_MODE): %s",  bOk ? "PASS" : "FAIL");

      Sleep(500);
      bOk = QLIB_ConfigureTimeOut(g_hResourceContext, QMSL_Timeout_Delay_ONLINE_FTM, _iCurrentTimeOut);

      printf("\n\nVerify Fuse is blow by performing QFUSE read...");
      unsigned short _iReadStatus = -1;
      unsigned long   _aConfigRegisterRead[5] = {0,0,0,0,0};
      bOk = QLIB_FTM_QFUSE_READ
            (
               g_hResourceContext,
               0,
               &_iReadStatus,
               _aConfigRegisterRead
            );
      printf("\nQLIB_FTM_QFUSE_READ(): %s, status = %d, values = { 0x%X, 0x%X, 0x%X, 0x%X, 0x%X }",
             bOk ? "PASS" : "FAIL",
             iWriteStatus,
             _aConfigRegisterRead[0],
             _aConfigRegisterRead[1],
             _aConfigRegisterRead[2],
             _aConfigRegisterRead[3],
             _aConfigRegisterRead[4]
            );
      if ( bOk )
      {
         // Verify the values match
         for ( int _iRegisterCheckIndex = 0; _iRegisterCheckIndex < c_iConfigRegisterLenMax; _iRegisterCheckIndex++)
         {
            if ( aConfigRegister_1 [ _iRegisterCheckIndex ] != _aConfigRegisterRead[_iRegisterCheckIndex ] )
            {
               printf("\n  ERROR: QFUSE Register mismatch, index %d.  Expected 0x%X, measured 0x%X",
                      _iRegisterCheckIndex,
                      aConfigRegister_1 [ _iRegisterCheckIndex ],
                      _aConfigRegisterRead[_iRegisterCheckIndex ]
                     );
            }   // if ( aConfigRegister_1 [ _iRegisterCheckIndex ] != _aConfigRegisterRead[_iRegisterCheckIndex ] )
         }  // for ( int _iRegisterCheckIndex = 0; _iRegisterCheckIndex < ; _iRegisterCheckIndex++)
      } // if ( bOk )

      ///////////////////////
      // 12) Provision IMEI
      ///////////////////////
      // This will set the IMEI to {1 2 3 4 5 6 7 8}");

      const unsigned short c_iIMEI_id = 550;
      const unsigned short c_iIMEI_size = 9;
      unsigned char _aIMEI[ c_iIMEI_size] = {8, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8 };
      _iStatus = -1;

      printf("\n\nProvisioning IMEI...");

      // This will have a failed status if the IMEI has already been written
      if ( QLIB_DIAG_NV_WRITE_F( g_hResourceContext, c_iIMEI_id, _aIMEI, c_iIMEI_size, &_iStatus ) )
      {
         printf("\nNV NV Write(%d)=0: PASS\n",c_iIMEI_id);
         // Check the status
         if ( _iStatus == NV_DONE_S )
         {
            printf("\nNV Write Status: PASS");
            // Read back IMEI
            printf("Read IMEI again to confirm\n");
            unsigned char _aIMEI_ReadBack [ c_iIMEI_size ];
            memset(_aIMEI_ReadBack, 0x0, c_iIMEI_size);
            if( QLIB_DIAG_NV_READ_F ( g_hResourceContext, c_iIMEI_id, _aIMEI_ReadBack, c_iIMEI_size, &_iStatus))
            {
               if ( _iStatus == NV_DONE_S )
               {
                  printf("NV item IMEI: ");
                  for(int i = 0; i < c_iIMEI_size; i++)
                     printf("%d ", _aIMEI_ReadBack[i]);
                  printf("\n");
               }
               else
                  printf("NV Item IMEI Read Back Failed\n");
            }
         }
         else
         {
            printf("\nNV Write Status: FAIL, the phone probably has a IMEI written");
         }
      }
      else
         printf("\nNV Write: FAIL");

      ///////////////////////
      // 13) Blow remaining fuse & master fuse
      ///////////////////////

      printf("\n\nBlowing remaining fuses & master fuse...");

      bOk = QLIB_FTM_QFUSE_WRITE
            (
               g_hResourceContext,
               iChainSelect,
               &iWriteStatus,
               aConfigRegister_2
            );
      printf("\nQLIB_FTM_QFUSE_WRITE(  blow master and trusted.root fuses): %s, status = %d", bOk ? "PASS" : "FAIL", iWriteStatus );

      ///////////////////////
      // 14) Program SIM lock
      ///////////////////////

      if ( _bWriteSIM_LOCK )
      {
         printf("\n\nProgram SIM lock...");
      }
      else
      {
         printf("\n\nProgram SIM lock is skipped...");
      }

      ///////////////////////
      // 15) NV_WRITE NV_FTM_MODE_I = 0, so the phone will power into ONLINE mode
      // 16) Reset into ONLINE mode
      ///////////////////////
      printf("\n\nResetting into ONLINE mode...");
      Test_NV_FTM_Mode( 0 );

      printf("\n\nProcedure completed.");
   }// cKey == d
   else if (cKey == 'F')
   {
      // printf("\nF=Report SIM Applications" );

      unsigned char _iNumSlots = 0;
      unsigned char _bSlot1Inserted = 0;
      unsigned char _iSlot1Apps = 0;
      unsigned char _bSlot2Inserted = 0;
      unsigned char _iSlot2Apps = 0;

      // Send GSDI command
      bOk = QLIB_GSDIDIAG_GET_AVAILABLE_APPS
            (
               g_hResourceContext,
               &_iNumSlots,
               &_bSlot1Inserted,
               &_iSlot1Apps,
               &_bSlot2Inserted,
               &_iSlot2Apps
            );

      printf("\n QLIB_GSDIDIAG_GET_AVAILABLE_APPS: %s", bOk ? "PASS" : "FAIL" );

      if (bOk)
      {
         printf("\n       Number of slots: %d", _iNumSlots );
         printf("\n\n       Slot 1 inserted: %s", (_bSlot1Inserted==TRUE ) ? "INSERTED" : "NOT INSERTED" );
         printf("\n           Slot 1 apps: 0x%X", _iSlot1Apps );

         printf("\n \n      Slot 2 inserted: %s", (_bSlot2Inserted==TRUE ) ? "INSERTED" : "NOT INSERTED" );
         printf("\n           Slot 2 apps: 0x%X", _iSlot2Apps );
         printf("\n\n Applications bit mask definitions:");
         printf("\nGSDI_DIAG_Apps_Telecom  = 0x01");
         printf("\nGSDI_DIAG_Apps_GSM      = 0x02");
         printf("\nGSDI_DIAG_Apps_USIM     = 0x04");
         printf("\nGSDI_DIAG_Apps_RUIM     = 0x08");
         printf("\nGSDI_DIAG_Apps_EMV      = 0x10");
         printf("\nGSDI_DIAG_Apps_DCS1800  = 0x20");
      }
   }    // else if (cKey == 'F')

   else if (cKey == 'G' )
   {
      // printf("\nG=Set Dual SIM IMEI Subscriber 0 ={8 1 2 3 4 5 6 7 8 }");
      // printf("\nG=Set Dual SIM IMEI Subscriber 1 ={8 7 6 5 4 3 2 1 8}");

      const unsigned short c_iIMEI_id = 550;
      const unsigned short c_iIMEI_size = 9;
      unsigned char _aIMEI[ c_iIMEI_size] = {8, 1, 2, 3, 4, 5, 6, 7, 8 };
      unsigned char _bIMEI[ c_iIMEI_size] = {8, 7, 6, 5, 4, 3, 2, 1, 8 };
      unsigned short _iSubID = 0;

      unsigned short _iStatus;

      // Dual SIM Subscriber Index 0 IMEI write
      if ( QLIB_DIAG_NV_WRITE_EXT_F( g_hResourceContext, c_iIMEI_id, _aIMEI, _iSubID, c_iIMEI_size, &_iStatus ) )
      {
         printf("NV Dual SIM Subscriber Index 0 Write(%d)=0: PASS\n",c_iIMEI_id);

         // Check the status
         if ( _iStatus == NV_DONE_S )
         {
            printf("NV Dual SIM Subscriber Index 0 Write Status: PASS\n");
         }
         else
         {
            printf("NV Dual SIM Subscriber Index 0 Write Status: FAIL\n");
         }
      }
      else
         printf("NV Dual SIM Subscriber Index 0 Write: FAIL\n");

      // Dual SIM Subscriber Index 1 IMEI write
      _iSubID = 1;

      if ( QLIB_DIAG_NV_WRITE_EXT_F( g_hResourceContext, c_iIMEI_id, _bIMEI, _iSubID, c_iIMEI_size, &_iStatus ) )
      {
         printf("NV Dual SIM Subscriber Index 1 Write(%d)=0: PASS\n",c_iIMEI_id);

         // Check the status
         if ( _iStatus == NV_DONE_S )
         {
            printf("NV Dual SIM Subscriber Index 1 Write Status: PASS\n");
         }
         else
         {
            printf("NV Write Status: FAIL\n");
         }
      }
      else
         printf("NV Write: FAIL\n");
   }

   else if (cKey == 'H' )
   {
      const unsigned short c_iIMEI_id = 550;
      const unsigned short c_iIMEI_size = 9;
      unsigned char _aIMEI[ c_iIMEI_size];
      unsigned short _iStatus;
      unsigned short _iSubID = 0;

      // Dual SIM Subscriber Index 0 IMEI Read
      //HANDLE hResourceContext, unsigned short iItemID, unsigned char* pItemData, unsigned short iContextID, int iLength, unsigned short* iStatus
      if ( QLIB_DIAG_NV_READ_EXT_F( g_hResourceContext, c_iIMEI_id, &_aIMEI[0], _iSubID, c_iIMEI_size, &_iStatus ) )
      {
         printf("NV Dual SIM Subscriber Index 0 Read(%d)=0: PASS\n",c_iIMEI_id);

         // Check the status
         if ( _iStatus == NV_DONE_S )
         {
            printf("NV Dual SIM Subscriber Index 0 Read Status: PASS\n");

            printf("NV Dual SIM Subscriber Index 0 = ");

            for(int i = 0; i < c_iIMEI_size; i++)
               printf(" 0x%x ", _aIMEI[i]);

            printf("\n");
         }
         else
         {
            if ( _iStatus == NV_NOTACTIVE_S )
            {
               printf("NV Dual SIM Subscriber Index 0 Read Status: INACTIVE\n");
            }
            else
            {
               printf("NV Dual SIM Subscriber Index 0 Read Status: FAIL\n");
            }
         }
      }
      else
         printf("NV Dual SIM Subscriber Index 0 Read: FAIL\n");

      // Dual SIM Subscriber Index 1 IMEI Read
      _iSubID = 1;
      if ( QLIB_DIAG_NV_READ_EXT_F( g_hResourceContext, c_iIMEI_id, _aIMEI, _iSubID, c_iIMEI_size, &_iStatus ) )
      {
         printf("NV Dual SIM Subscriber Index 1 Read(%d)=0: PASS\n",c_iIMEI_id);

         // Check the status
         if ( _iStatus == NV_DONE_S )
         {
            printf("NV Dual SIM Subscriber Index 1 Read Status: PASS\n");

            printf("Dual SIM Subscriber Index 1 = ");

            for(int i = 0; i < c_iIMEI_size; i++)
               printf(" 0x%x ", _aIMEI[i]);

            printf("\n");
         }
         else
         {
            if ( _iStatus == NV_NOTACTIVE_S )
            {
               printf("NV Dual SIM Subscriber Index 1 Read Status: INACTIVE\n");
            }
            else
            {
               printf("NV Dual SIM Subscriber Index 1 Read Status: FAIL\n");
            }
         }
      }
      else
         printf("NV Dual SIM Subscriber Index 1 Read: FAIL\n");
   }

   return bOk;
}

unsigned char ReportPhoneState( void )
{
   unsigned char _bOk = true;

   unsigned long _iPhoneOperatingMode = SYS_OPRT_MODE_NONE;
   unsigned long _iCallState = CM_CALL_STATE_NONE;
   unsigned long _iSystemMode = SYS_SYS_MODE_NONE;
   char* _pOprtModeStatusText = "Unknown";

   char* _aSystemModeStatusText[ SYS_SYS_MODE_MAX  + 1] =
   {
      "SYS_SYS_MODE_NO_SRV",
      "SYS_SYS_MODE_AMPS",
      "SYS_SYS_MODE_CDMA",
      "SYS_SYS_MODE_GSM",
      "SYS_SYS_MODE_HDR",
      "SYS_SYS_MODE_WCDMA",
      "SYS_SYS_MODE_GPS",
      "SYS_SYS_MODE_GW",
      "Invalid"
   };

   char* _aCallStateStatusText[ CM_CALL_STATE_MAX  + 1] =
   {
      "CM_CALL_STATE_IDLE",
      "CM_CALL_STATE_ORIG",
      "CM_CALL_STATE_INCOM",
      "CM_CALL_STATE_CONV",
      "CM_CALL_STATE_CC_IN_PROGRESS",
      "Invalid"
   };

   short _iCommandCode = 0;
   if ( QLIB_FTM_GET_COMMAND_CODE( g_hResourceContext, &_iCommandCode  ) )
   {
      // This command only works for command mode 75
      if ( _iCommandCode == 75 )
      {
         if ( QLIB_GetPhoneOperatingMode( g_hResourceContext, &_iPhoneOperatingMode ) )
         {
            // Determine the appropriate text
            switch( _iPhoneOperatingMode )
            {
               case SYS_OPRT_MODE_OFFLINE:
               case SYS_OPRT_MODE_OFFLINE_CDMA:
               case SYS_OPRT_MODE_OFFLINE_AMPS:
                  _pOprtModeStatusText = "OFFLINE";
                  break;
               case SYS_OPRT_MODE_ONLINE:
                  _pOprtModeStatusText = "ONLINE";
                  break;
               case SYS_OPRT_MODE_LPM:
                  _pOprtModeStatusText = "LOW POWER MODE";
                  break;
               case SYS_OPRT_MODE_FTM:
                  _pOprtModeStatusText = "FTM";
                  break;

               default:
                  _pOprtModeStatusText = "Not in list";
                  break;
            }

            // Print the status text
            printf("\n     Phone Operating Mode: %s", _pOprtModeStatusText );
         }  // if ( QLIB_GetPhoneOperatingMode( g_hResourceContext, &_iPhoneOperatingMode ) )

         if ( QLIB_GetPhoneCallState(g_hResourceContext, &_iCallState, &_iSystemMode) )
         {
            // Validate call state
            if ( _iCallState >= SYS_SYS_MODE_MAX )
            {
               _iCallState = SYS_SYS_MODE_MAX;
            }

            // Validate call system mode
            if ( _iSystemMode >= CM_CALL_STATE_MAX )
            {
               _iSystemMode = CM_CALL_STATE_MAX;
            }

            // Print the status text
            printf("\n           Phone Call State: %s", _aCallStateStatusText[ _iCallState ] );
            printf("\n    Phone System Mode State: %s", _aSystemModeStatusText[ _iSystemMode ] );
         }
      } // if ( _iCommandCode == 75 )
   }    // if ( QLIB_FTM_GET_COMMAND_CODE( g_hResourceContext, _iCommandCode  ) )

   unsigned char _iFTM_Mode = 0;
   unsigned short _iStatus = 0;

   // NV ID, Data buffer, size, status
   _bOk = QLIB_DIAG_NV_READ_F(  g_hResourceContext, 453, &_iFTM_Mode, 1, &_iStatus );

   if (_bOk )
   {
      printf("\n                FTM Mode: %s", _iFTM_Mode ? "ON" : "OFF" );
   }
   else
   {
      printf("\nQLIB_DIAG_NV_READ_F(NV_FTM_MODE_I): FAIL");
   }    // if (_bOk)

   // Determine the QPST phone status
   unsigned long _ePhoneStatus;
   const int c_iNumStatus = 12;
   _bOk = QLIB_GetQpstPhoneStatus( g_hResourceContext, &_ePhoneStatus );
   if ( (_bOk )
         &&    ( _ePhoneStatus <= c_iNumStatus )
      )
   {
      // String list of status
      char* _asSWD_StatusStrings[ c_iNumStatus ] =
      {
         { "SWD_modeNone"                   },
         { "SWD_modeDataServices"           },
         { "SWD_modeDownload"               },
         { "SWD_modeDiagnostic"             },
         { "SWD_modeDiagnosticOffline"      },
         { "SWD_modeReset"                  },
         { "SWD_modeStreamDownload"         },
         { "SWD_modeMspDownload"                },
         { "SWD_modeMspDiagnostic"          },
         { "SWD_modeMspDiagnosticOffline"   },
         { "SWD_modeMspReset"               },
         { "SWD_modeMspStreamDownload"      }
      };

      printf("\n\nQPST Phone Status: %s", _asSWD_StatusStrings[ _ePhoneStatus ] );
   }

   return (_bOk );
}

unsigned char Test_GSM_Diag( void )
{
   unsigned char bOk = true;
   char cKey = ' ';

   printf("----------------\nGSM Diag Test\n----------------\n");

   printf("\n");
   printf("\n1=GSM Status");
   printf("\n2=Start call to '555'");
   printf("\n3=End Call" );
   printf("\n4=Start GSM L1 Burst Metrics log");
   printf("\n5=Start WCDMA AGC log");

   cKey = getch();

   if (cKey == '1' )
   {
      // Storage areas
      unsigned char _aiIMEI[9];
      unsigned char _aiIMSI[9];
      unsigned char _aiLAI[5];
      unsigned short _iCellID;
      unsigned char _iCM_CallState;
      unsigned char _iCM_OperationalMode;
      unsigned char _iCM_SystemMode;

      bOk = QLIB_DIAG_GSM_STATUS_F( g_hResourceContext, _aiIMEI, _aiIMSI,  _aiLAI,
                                    &_iCellID, &_iCM_CallState, &_iCM_OperationalMode, &_iCM_SystemMode );

      printf("\n QLIB_DIAG_GSM_STATUS_F = %s", bOk ? "PASS" : "FAIL" );

      if ( bOk )
      {
         int _iIndex = 1;

         printf("\n        IMEI: " );
         for ( _iIndex = 1 ; _iIndex <= _aiIMEI[0]; _iIndex++ )
            printf("0x%X,", _aiIMEI[_iIndex] );

         printf("\n        IMSI: " );
         for ( _iIndex = 1 ; _iIndex <= _aiIMSI[0]; _iIndex++ )
            printf("0x%X,", _aiIMSI[_iIndex] );

         printf("\n         LAI: " );
         for ( _iIndex = 0 ; _iIndex < GSM_DIAG_LAI_SIZE; _iIndex++ )
            printf("0x%X,", _aiLAI[_iIndex] );

         printf("\n     Cell ID: %d", _iCellID      );
         printf("\n  Call State: %d", _iCM_CallState    );
         printf("\n    Op State: %d", _iCM_OperationalMode      );
         printf("\n   Sys State: %d", _iCM_SystemMode   );
      }
   }    // cKey == '1'
   else if (cKey == '2' )
   {
      // Setup the IMSI
      unsigned char _aiIMSI[GSM_DIAG_CALL_ORG_IMSI_SIZE];
      unsigned char _iIMSI_Size = 5;

      for ( int _iIndex = 0; _iIndex < _iIMSI_Size; _iIndex++)
         _aiIMSI[ _iIndex ] = '5';

      // Make the call
      bOk = QLIB_DIAG_GSM_CALL_ORIG_F( g_hResourceContext, _iIMSI_Size, _aiIMSI );

      printf("\n QLIB_DIAG_GSM_CALL_ORIG_F = %s", bOk ? "PASS" : "FAIL" );
   }
   else if (cKey == '3' )
   {
      // Make the call
      bOk = QLIB_DIAG_GSM_CALL_END_F( g_hResourceContext );

      printf("\n QLIB_DIAG_GSM_CALL_END_F = %s", bOk ? "PASS" : "FAIL" );
   }
   else if (cKey == '4')
   {
      printf("\n4=Start GSM L1 Burst Metrics log");
      const unsigned short c_iGSM_L1_BURST_Metrics_Log = 0x506C;

      // Clear all log codes for WCDMA, so the log bit mask is clean
      bOk = QLIB_DIAG_ClearAllLogCodes( g_hResourceContext, LOG_EQUIP_ID_WCDMA );
      printf("\nQLIB_DIAG_ClearAllLogCodes(): %s",
             bOk ? "PASS" : "FAIL" );

      bOk = QLIB_DIAG_AddExtendedLogCode(  g_hResourceContext, LOG_EQUIP_ID_GSM, c_iGSM_L1_BURST_Metrics_Log  );
      printf("\nQLIB_DIAG_AddExtendedLogCode(  %d, 0x%X ): %s",
             LOG_EQUIP_ID_UMTS,
             c_iGSM_L1_BURST_Metrics_Log,
             bOk ? "PASS" : "FAIL" );

      bOk = QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
      printf("\nQLIB_DIAG_SetPhoneLoggingState(  ON): %s", bOk ? "PASS" : "FAIL" );
   }
   else if (cKey == '5')
   {
      // Clear all log codes for WCDMA, so the log bit mask is clean
      bOk = QLIB_DIAG_ClearAllLogCodes( g_hResourceContext, LOG_EQUIP_ID_WCDMA );
      printf("\nQLIB_DIAG_ClearAllLogCodes(): %s",
             bOk ? "PASS" : "FAIL" );

      // printf("\n5=Start WCDMA AGC log");
      const unsigned short c_iWCDMA_AGC_Log = 0x4105;

      bOk = QLIB_DIAG_AddExtendedLogCode(  g_hResourceContext, LOG_EQUIP_ID_WCDMA, c_iWCDMA_AGC_Log  );
      printf("\nQLIB_DIAG_AddExtendedLogCode(  %d, 0x%X ): %s",
             LOG_EQUIP_ID_UMTS,
             c_iWCDMA_AGC_Log,
             bOk ? "PASS" : "FAIL" );

      bOk = QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
      printf("\nQLIB_DIAG_SetPhoneLoggingState(  ON): %s", bOk ? "PASS" : "FAIL" );
   }

   return bOk;
}

unsigned char Test_Handset( void )
{
   unsigned char bOk = true;
   char cKey = ' ';

   printf("----------------\nHandset Diag Test\n----------------\n");

   char _sCaptureFile[] = "/Brew/Usr/screen.bmp";
   unsigned short _iDimensions = 12*4;

   printf("\n");
   printf("\n1=Get Primary Display Parameters");
   printf("\n2=Get Secondary Display Parameters");
   printf("\n3=Capture primary display (0,0) - (%d,%d) to EFS file '%s'", _iDimensions, _iDimensions, _sCaptureFile );
   printf("\n4=Capture secondary display (0,0) - (%d,%d) to EFS file '%s'", _iDimensions, _iDimensions, _sCaptureFile );

   cKey = getch();

   if ( ( cKey == '1' )
         ||    ( cKey == '2' )
      )
   {
      // Response values
      unsigned char _iDisplayID  = ( cKey ==  '1') ? 0 : 1;
      unsigned short _iDisplayHeight;
      unsigned short _iDisplayWidth;
      unsigned char _iPixelDepth;
      unsigned char _iPanelOrientation;

      bOk = QLIB_DIAG_HS_DISPLAY_GET_PARAMS_CMD(
               g_hResourceContext,
               _iDisplayID, &_iDisplayHeight, &_iDisplayWidth,
               &_iPixelDepth, &_iPanelOrientation
            );

      printf("\n QLIB_DIAG_HS_DISPLAY_GET_PARAMS_CMD = %s", bOk ? "PASS" : "FAIL" );

      if ( bOk )
      {
         printf("\n    Display ID: %d", _iDisplayID         );
         printf("\n        Height: %d", _iDisplayHeight     );
         printf("\n         Width: %d", _iDisplayWidth      );
         printf("\n     Bit depth: %d", _iPixelDepth            );
         printf("\n   Orientation: %d", _iPanelOrientation  );
      }
   }    // cKey == '1'
   else if (    ( cKey == '3' )
                ||    ( cKey == '4' )
           )

   {
      unsigned char _iDisplayID  = ( cKey ==  '3') ? 0 : 1;

      unsigned short _iStartingRow = 0 ;
      unsigned short _iStartingCol = 0;
      unsigned short _iHeight = _iDimensions;
      unsigned short _iWidth = _iDimensions;

      bOk = QLIB_HS_DISPLAY_CAPTURE_DELAYED_CMD
            (
               g_hResourceContext, _iDisplayID,
               _iStartingRow, _iStartingCol, _iHeight, _iWidth,
               _sCaptureFile
            );

      printf("\n QLIB_DIAG_HS_DISPLAY_GET_PARAMS_CMD = %s", bOk ? "PASS" : "FAIL" );

      printf("\n Not implemented" );
   }

   return bOk;
}

unsigned char Test_UserDefinedTransport( void )
{
   unsigned char bOk = true;
   int _iComPort = 0;
   unsigned char _cKey = '\0';

   printf("\n\n==============================\nUser Defined Transport:\n==============================");

   printf("\n1=Connect to user defined transport");
   printf("\n2=QLIB_DisconnectServer()");
   printf("\n3=Disconnect from user defined transport");
   _cKey = toupper( getch());

   if ( _cKey == '1' )
   {
      // Disconnect server if the current handle is not NULL
      if ( g_hResourceContext )
      {
         QLIB_DisconnectServer( g_hResourceContext );
         g_hResourceContext = NULL;
         printf("\nQLIB_DisconnectServer() was called to release the current handle");
         printf("\n\n<<Make sure that QPST has completely closed, then press any key to continue>>");
         getch();
      } // if ( g_hResourceContext )

      // Get the COM port
      printf("\n Please enter the COM port #: ");
      scanf("%hd", & _iComPort );

      // Try to open the COM port via the user defined open function
      if ( OpenUserDefinedTransport( _iComPort ) == NO_ERROR )
      {
         printf("\nOpenUserDefinedTransport( iCOM_Port = %d ): PASS", _iComPort );

         // Open QMSL using the user-defined callback functions
         g_hResourceContext = QLIB_ConnectServer_UserDefinedTransport(
                                 (HANDLE) _iComPort, // For this example, use the COM port number as the handle, but this could be some other arbitrary number
                                 &UserDefinedSend,
                                 &UserDefinedReceive  ,
                                 &UserDefinedFlushTxRx,
                                 true,   //  bQMSL_HandlesTxHDLC, set to true if QMSL should handle HDLC for data going to phone
                                 true    //  bQMSL_HandlesRxHDLC, set to true if QMSL should handle HDLC for data coming from phone
                              );
         printf("\nQLIB_ConnectServer_UserDefinedTransport(%d): %s", _iComPort, g_hResourceContext ? "PASS" : "FAIL" );

         // Check online status
         if ( QLIB_IsPhoneConnected( g_hResourceContext ) )
         {
            // Include LOG_INF_DEV in the log flags, for extended debug in the background receive thread
            QLIB_SetLogFlags( g_hResourceContext, LOG_DEFAULT | LOG_INF_DEV );
            QLIB_StartLogging( g_hResourceContext,  "C:\\Factory_log.txt");

            // Print the mobile's info
            TestVersion( );

            // Test the pause and resume
            if ( QLIB_PauseDataReceive( g_hResourceContext ) )
            {
               printf("\nQLIB_PauseDataReceive(): OK");

               printf("\nPress 'C' to close the port or any other key to go on with the user defined port opened");
               // right here there should be no hits to the user defined receive function
               char _cKey = toupper( getch() );

               if ( _cKey == 'C' )
               {
                  // This should be done before the application completes
                  QLIB_DisconnectServer( g_hResourceContext );
               }
               else
               {
                  if ( QLIB_ResumeDataReceive( g_hResourceContext ) )
                  {
                     printf("\nQLIB_ResumeDataReceive(): OK");
                  }
                  else
                  {
                     printf("\nQLIB_ResumeDataReceive(): NOT OK");
                  } // if ( QLIB_ResumeDataReceive( g_hResourceContext ) )
               }
            }
            else
            {
               printf("\nQLIB_PauseDataReceive(): NOT OK");
            }   // if ( QLIB_PauseDataReceive( g_hResourceContext ) )
         }  // if ( QLIB_DIAG_IsOnline() )

         // Set return value
         bOk = (g_hResourceContext  != NULL );
      }
      else
      {
         printf("\nOpenUserDefinedTransport( iCOM_Port = %d) has failed?", _iComPort );
      } // if ( OpenUserDefinedTransport( _iComPort ) )
   }    // if ( _cKey == '1' )
   else if ( _cKey == '2' )
   {
      QLIB_DisconnectServer( g_hResourceContext );
      printf("\nQLIB_DisconnectServer(): COMPLETED" );
   }
   else if ( _cKey == '3' )
   {
      CloseUserDefinedTransport();
      printf("\nCloseUserDefinedTransport() has been called");

      bOk = true;
   }

   printf("\n");
   return bOk;
}


int wlanID;

unsigned char Test_FTM_WLAN( )
{
   unsigned char bOk = true;
   char cKey = '\0';

   // Philips status return
   unsigned char _iPhilipsStatus = 0x00;

   printf("----------------\nFTM WLAN Test\n----------------\n");

   if ( bOk )
   {
      printf("\n1=Philips WLAN Sequence");
      printf("\n2=Philips Get/Set MAC Address");
      printf("\n3=Philips STOP");
      printf("\n4=Philips Rx Continuous");
      printf("\n5=Philips Tx Burst");
      printf("\n6=Philips Tx Continuous");
      printf("\n7=Philips Tx Power Control");
      printf("\n8=Philips Read Statistics");
      printf("\n9=Philips 32kHz clock status");
      printf("\na=Atheros Tests");
      printf("\nb=Atheros TLV Tests");
      printf("\nd=QWLAN (Gen6) Tests");
      cKey = getch();
      printf("\nYou chose %c\n", cKey);

      if (cKey == 'd' )
      {
         bool wlanGen6Loop = true;

         while(wlanGen6Loop)
         {
            printf("\n\n Choose QWLAN FTM test you would like to do");

            printf("\n1=QLIB_FTM_WLAN_GEN6_START");
            printf("\n2=QLIB_FTM_WLAN_GEN6_STOP");
            printf("\n3=QLIB_FTM_WLAN_GEN6_SET_CHANNEL");
            printf("\n4=QLIB_FTM_WLAN_GEN6_SET_CLOSED_LOOP_POWER");
            printf("\n5=QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP");
            printf("\n6=QLIB_FTM_WLAN_GEN6_SET_RX_DISABLE_MODE");
            printf("\n7=QLIB_FTM_WLAN_GEN6_SET_TX_FRAME");
            printf("\n8=QLIB_FTM_WLAN_GEN6_GET_RX_PACKET_COUNTS");
            printf("\n9=QLIB_FTM_WLAN_GEN6_GET_RX_RSSI_VAL");
            printf("\na=QLIB_FTM_WLAN_GEN6_SET_MAC_ADDR");
            printf("\nb=QLIB_FTM_WLAN_GEN6_GET_MAC_ADDR");
            printf("\nc=QLIB_FTM_WLAN_GEN6_RESET_RX_PACKET_STATISTICS");
            printf("\nd=QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS");
            printf("\ne=QLIB_FTM_WLAN_GEN6_GRAB_RAM");
            printf("\nf=QLIB_FTM_WLAN_GEN6_GEN_CW");
            printf("\ng=QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN");
            printf("\nh=QLIB_FTM_WLAN_GEN6_START_WAVEFORM");
            printf("\ni=QLIB_FTM_WLAN_GEN6_STOP_WAVEFORM");
            printf("\nj=QLIB_FTM_WLAN_GEN6_GET_TX_POWER_REPORT");
            printf("\nk=QLIB_FTM_WLAN_GEN6_QUERY_TX_STATUS");
            printf("\nl=QLIB_FTM_WLAN_GEN6_GET_NV_TABLE");
            printf("\nm=QLIB_FTM_WLAN_GEN6_SET_NV_TABLE");
            printf("\nn=QLIB_FTM_WLAN_GEN6_UPDATE_TPC_SPLIT_LUT");
            printf("\no=QLIB_FTM_WLAN_GEN6_GET_POWER_LUT");
            printf("\np=QLIB_FTM_WLAN_GEN6_GET_NV_FIELD");
            printf("\nr=QLIB_FTM_WLAN_GEN6_READ_NV_WRITE_XML");
            printf("\ns=QLIB_FTM_WLAN_GEN6_TX_CARRIER_SUPPRESS_CAL");
            printf("\nt=HAL/PHY Dump (QLIB_FTM_WLAN_GEN6_GRAB_RAM overload - 3660)");
            printf("\nv=QLIB_FTM_WLAN_GEN6_GET_BUILD_RELEASE_NUMBER");
            printf("\nw=QLIB_FTM_WLAN_GEN6_READ_XML_WRITE_NV");
            printf("\nx=QLIB_FTM_WLAN_GEN6_RX_IQ_CAL_V2");
            printf("\ny=QLIB_FTM_WLAN_GEN6_SET_TX_FRAME_PHONE_TEST_SET_RX_TX");
            printf("\nz=QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2");
            printf("\n!=QLIB_FTM_WLAN_GEN6_TX_IQ_CAL_V2");
            printf("\n@=QLIB_FTM_WLAN_GEN6_READ_REGISTER");
            printf("\n#=QLIB_FTM_WLAN_GEN6_CLPC_SW_CAL");
            printf("\n$=QLIB_FTM_WLAN_GEN6_GET_UNICAST_MAC_PKT_RSSI");
            printf("\n%%=QLIB_FTM_WLAN_GEN6_GET_UNICAST_MAC_PKT_RSSI_CONF");
            printf("\n^=QLIB_FTM_WLAN_GEN6_CLPC_CAL_EXTRA_MEASUREMENT");
            printf("\n&=QLIB_FTM_WLAN_GEN6_ENABLE_DPD");
            printf("\n*=QLIB_FTM_WLAN_GEN6_GENERIC_CMD");

            printf("\n Choose a test case to execute, q to exit submenu");

            cKey = getch();
            printf("\nYou chose %c\n", cKey);
            switch (cKey)
            {
               case '1':
               {
                  wlanID = 1314;
                  printf("\n Enter Wlan ID (1312,1314 or 3660)=");
                  scanf("%d",&wlanID);
                  bOk = QLIB_FTM_WLAN_GEN6_START( g_hResourceContext,wlanID);
                  printf("\n QLIB_FTM_WLAN_GEN6_START() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '2':
                  bOk = QLIB_FTM_WLAN_GEN6_STOP( g_hResourceContext);
                  printf("\n QLIB_FTM_WLAN_GEN6_STOP() = %s", bOk ? "PASS" : "FAIL" );
                  break;

               case '3':
               {
                  unsigned long channel = 0;
                  printf("\n Enter Channel=");
                  scanf("%d",&channel);

                  unsigned long channelBondingState = 0;
                  printf("\n Enter Channel Bonding State (0:none, 1:Primary Low, 3:Primary High)=");
                  scanf("%d",&channelBondingState);

                  bOk = QLIB_FTM_WLAN_GEN6_SET_CHANNEL_V2( g_hResourceContext, channel, channelBondingState);
                  printf("\n QLIB_FTM_WLAN_GEN6_SET_CHANNEL_V2() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '4':
               {
                  double pwr = 0;
                  printf("\n Enter power level =");
                  scanf("%lf",&pwr);
                  bOk = QLIB_FTM_WLAN_GEN6_SET_CLOSED_LOOP_POWER( g_hResourceContext, pwr);
                  printf("\n QLIB_FTM_WLAN_GEN6_SET_CLOSED_LOOP_POWER() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '5':
               {
                  unsigned char inp = 0;
                  byte iStart = 0;
                  printf("\n 0:Stop PktGen , 1:Start PktGen=");
                  scanf("\n%c",&inp);
                  iStart = ((inp == '0')?0:1);
                  bOk = QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP( g_hResourceContext, iStart);
                  printf("\n QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '6':
               {
                  bool agPktsDisabled = false, bPktsDisabled = false, slrPktsDisabled = false;
                  char mode = 1;
                  printf("\n Enter values for disabling the modes. 0:Don't Disable , 1:Disable");
                  printf("\n agPkts mode: Disable?");
                  scanf("\n%c", &mode);
                  agPktsDisabled = (mode=='1'?true:false);
                  printf("\n bPkts mode: Disable?");
                  scanf("\n%c", &mode);
                  bPktsDisabled = (mode=='1'?true:false);
                  printf("\n slrPkts mode: Disable?");
                  scanf("\n%c", &mode);
                  slrPktsDisabled = (mode=='1'?true:false);
                  bOk = QLIB_FTM_WLAN_GEN6_SET_RX_DISABLE_MODE(g_hResourceContext, agPktsDisabled,
                                                               bPktsDisabled, slrPktsDisabled);
                  printf("\n QLIB_FTM_WLAN_GEN6_SET_RX_DISABLE_MODE() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '7':
               {
                  unsigned long iNumTestFrame;
                  unsigned int iPayloadSize;
                  unsigned long iFrameSpacing;
                  unsigned int iFCSCal;
                  unsigned long iRate;
                  unsigned long iPreamble;
                  char inputStr[80];

                  printf("\n iNumTestFrame val=");
                  scanf("%ul",&iNumTestFrame);

                  printf("\n iPayloadSize val=");
                  scanf("%u",&iPayloadSize);

                  printf("\n iFrameSpacing val(uSec)=");
                  scanf("%ul",&iFrameSpacing);

                  printf("\n iFCSCal val=");
                  scanf("\n%d",&iFCSCal);

                  while( true )
                  {
                     printf("\n iRate val (type '?' for value list)=");
                     scanf("%s", inputStr );
                     if( inputStr[0] == '?' )
                     {
                        if( wlanID == 1312 || wlanID == 1314 )
                        {
                           printf("\n --- 802.11b Rates --- ");
                           printf("\n 0: HAL_PHY_RATE_11B_LONG_1_MBPS");
                           printf("\n 1: HAL_PHY_RATE_11B_LONG_2_MBPS");
                           printf("\n 2: HAL_PHY_RATE_11B_LONG_5_5_MBPS");
                           printf("\n 3: HAL_PHY_RATE_11B_LONG_11_MBPS");
                           printf("\n 4: HAL_PHY_RATE_11B_SHORT_2_MBPS");
                           printf("\n 5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS");
                           printf("\n 6: HAL_PHY_RATE_11B_SHORT_11_MBPS");
                           printf("\n --- SLR Rates --- ");
                           printf("\n 7: HAL_PHY_RATE_SLR_0_25_MBPS");
                           printf("\n 8: HAL_PHY_RATE_SLR_0_5_MBPS");
                           printf("\n --- Spica_Virgo 11A 20MHz Rates --- ");
                           printf("\n 9: HAL_PHY_RATE_11A_6_MBPS");
                           printf("\n10: HAL_PHY_RATE_11A_9_MBPS");
                           printf("\n11: HAL_PHY_RATE_11A_12_MBPS");
                           printf("\n12: HAL_PHY_RATE_11A_18_MBPS");
                           printf("\n13: HAL_PHY_RATE_11A_24_MBPS");
                           printf("\n14: HAL_PHY_RATE_11A_36_MBPS");
                           printf("\n15: HAL_PHY_RATE_11A_48_MBPS");
                           printf("\n16: HAL_PHY_RATE_11A_54_MBPS");
                           printf("\n --- MCS Index #0-15 (20MHz) --- ");
                           printf("\n17: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS");
                           printf("\n18: HAL_PHY_RATE_MCS_1NSS_13_MBPS");
                           printf("\n19: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS");
                           printf("\n20: HAL_PHY_RATE_MCS_1NSS_26_MBPS");
                           printf("\n21: HAL_PHY_RATE_MCS_1NSS_39_MBPS");
                           printf("\n22: HAL_PHY_RATE_MCS_1NSS_52_MBPS");
                           printf("\n23: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS");
                           printf("\n24: HAL_PHY_RATE_MCS_1NSS_65_MBPS");
                           printf("\n25: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS");
                           printf("\n26: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS");
                           printf("\n27: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS");
                           printf("\n28: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS");
                           printf("\n29: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS");
                           printf("\n30: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS");
                           printf("\n31: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS");
                           printf("\n32: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS");
                           printf("\n33: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS");
                           printf("\n");
                        }
                        else
                        {
                           printf("\n --- 802.11b Rates --- ");
                           printf("\n 0: HAL_PHY_RATE_11B_LONG_1_MBPS");
                           printf("\n 1: HAL_PHY_RATE_11B_LONG_2_MBPS");
                           printf("\n 2: HAL_PHY_RATE_11B_LONG_5_5_MBPS");
                           printf("\n 3: HAL_PHY_RATE_11B_LONG_11_MBPS");
                           printf("\n 4: HAL_PHY_RATE_11B_SHORT_2_MBPS");
                           printf("\n 5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS");
                           printf("\n 6: HAL_PHY_RATE_11B_SHORT_11_MBPS");
                           printf("\n --- Spica_Virgo 11A 20MHz Rates --- ");
                           printf("\n 7: HAL_PHY_RATE_11A_6_MBPS");
                           printf("\n 8: HAL_PHY_RATE_11A_9_MBPS");
                           printf("\n 9: HAL_PHY_RATE_11A_12_MBPS");
                           printf("\n10: HAL_PHY_RATE_11A_18_MBPS");
                           printf("\n11: HAL_PHY_RATE_11A_24_MBPS");
                           printf("\n12: HAL_PHY_RATE_11A_36_MBPS");
                           printf("\n13: HAL_PHY_RATE_11A_48_MBPS");
                           printf("\n14: HAL_PHY_RATE_11A_54_MBPS");
                           printf("\n --- 11A Duplicate 40MHz Rates --- ");
                           printf("\n15: HAL_PHY_RATE_11A_DUP_6_MBPS");
                           printf("\n16: HAL_PHY_RATE_11A_DUP_9_MBPS");
                           printf("\n17: HAL_PHY_RATE_11A_DUP_12_MBPS");
                           printf("\n18: HAL_PHY_RATE_11A_DUP_18_MBPS");
                           printf("\n19: HAL_PHY_RATE_11A_DUP_24_MBPS");
                           printf("\n20: HAL_PHY_RATE_11A_DUP_36_MBPS");
                           printf("\n21: HAL_PHY_RATE_11A_DUP_48_MBPS");
                           printf("\n22: HAL_PHY_RATE_11A_DUP_54_MBPS");
                           printf("\n --- MCS Index #0-15 (20MHz) --- ");
                           printf("\n23: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS");
                           printf("\n24: HAL_PHY_RATE_MCS_1NSS_13_MBPS");
                           printf("\n25: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS");
                           printf("\n26: HAL_PHY_RATE_MCS_1NSS_26_MBPS");
                           printf("\n27: HAL_PHY_RATE_MCS_1NSS_39_MBPS");
                           printf("\n28: HAL_PHY_RATE_MCS_1NSS_52_MBPS");
                           printf("\n29: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS");
                           printf("\n30: HAL_PHY_RATE_MCS_1NSS_65_MBPS");
                           printf("\n31: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS");
                           printf("\n32: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS");
                           printf("\n33: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS");
                           printf("\n34: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS");
                           printf("\n35: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS");
                           printf("\n36: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS");
                           printf("\n37: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS");
                           printf("\n38: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS");
                           printf("\n39: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS");
                           printf("\n --- MCS Index #0-15 (40MHz) --- ");
                           printf("\n40: HAL_PHY_RATE_MCS_1NSS_CB_13_5_MBPS");
                           printf("\n41: HAL_PHY_RATE_MCS_1NSS_CB_27_MBPS");
                           printf("\n42: HAL_PHY_RATE_MCS_1NSS_CB_40_5_MBPS");
                           printf("\n43: HAL_PHY_RATE_MCS_1NSS_CB_54_MBPS");
                           printf("\n44: HAL_PHY_RATE_MCS_1NSS_CB_81_MBPS");
                           printf("\n45: HAL_PHY_RATE_MCS_1NSS_CB_108_MBPS");
                           printf("\n46: HAL_PHY_RATE_MCS_1NSS_CB_121_5_MBPS");
                           printf("\n47: HAL_PHY_RATE_MCS_1NSS_CB_135_MBPS");
                           printf("\n48: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_15_MBPS");
                           printf("\n49: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_30_MBPS");
                           printf("\n50: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_45_MBPS");
                           printf("\n51: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_60_MBPS");
                           printf("\n52: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_90_MBPS");
                           printf("\n53: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_120_MBPS");
                           printf("\n54: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_135_MBPS");
                           printf("\n55: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_150_MBPS");
                           printf("\n");
                        }
                     }
                     else
                     {
                        sscanf(inputStr, "\n%ul",&iRate);
                        break;
                     }
                  }
                  while( true )
                  {
                     printf("\n iPreamble val (type '?' for value list)=");
                     scanf("%s", inputStr );
                     if( inputStr[0] == '?' )
                     {
                        printf("\n 0: PHYDBG_PREAMBLE_OFDM");
                        printf("\n 1: PHYDBG_PREAMBLE_GREENFIELD");
                        printf("\n 2: PHYDBG_PREAMBLE_MIXED");
                        printf("\n 3: PHYDBG_PREAMBLE_SHORTB");
                        printf("\n 4: PHYDBG_PREAMBLE_LONGB");
                        printf("\n");
                     }
                     else
                     {
                        sscanf(inputStr, "%ul",&iPreamble);
                        break;
                     }
                  }

                  bOk = QLIB_FTM_WLAN_GEN6_SET_TX_FRAME(g_hResourceContext, iNumTestFrame, (unsigned short)iPayloadSize, iFrameSpacing, (unsigned char)iFCSCal, iRate, iPreamble);
                  printf("\n QLIB_FTM_WLAN_GEN6_SET_TX_FRAME() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '8':
               {
                  unsigned long rxFrameCounter;
                  unsigned long totalMacRxPackets;
                  unsigned long totalMacFcsErrPackets;

                  bOk = QLIB_FTM_WLAN_GEN6_GET_RX_PACKET_COUNTS(g_hResourceContext, &rxFrameCounter, &totalMacRxPackets, &totalMacFcsErrPackets );
                  printf("\n QLIB_FTM_WLAN_GEN6_GET_RX_PACKET_COUNTS() = %s", bOk ? "PASS" : "FAIL" );
                  printf("\n    total packets: %d", rxFrameCounter );
                  printf("\n    total MAC packets: %d", totalMacRxPackets );
                  printf("\n    total MAQC Error packets: %d", totalMacFcsErrPackets );
               }
               break;

               case '9':
               {
                  short rssiVals[2];
                  unsigned short numRxChains;
                  bOk = QLIB_FTM_WLAN_GEN6_GET_RX_RSSI_VAL(g_hResourceContext, rssiVals, &numRxChains);
                  printf("\n QLIB_FTM_WLAN_GEN6_GET_RX_RSSI_VAL() = %s", bOk ? "PASS" : "FAIL" );
                  printf("\n    rssi 0: %d  rssi 1: %d", rssiVals[0]-105, rssiVals[1]-105 );
               }
               break;

               case 'a':
               case 'A':
               {
                  byte macAddr[6] = {0x11, 0x11, 0x22, 0x22, 0x33, 0x33};
                  bOk = QLIB_FTM_WLAN_GEN6_SET_MAC_ADDR(g_hResourceContext, macAddr);
                  printf("\n QLIB_FTM_WLAN_GEN6_SET_MAC_ADDR() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'b':
               case 'B':
               {
                  byte macAddr[6];
                  bOk = QLIB_FTM_WLAN_GEN6_GET_MAC_ADDR(g_hResourceContext, macAddr);
                  printf("\nMAC Addr: 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x",
                         macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
                  printf("\n QLIB_FTM_WLAN_GEN6_GET_MAC_ADDR() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'c':
               case 'C':
               {
                  bOk = QLIB_FTM_WLAN_GEN6_RESET_RX_PACKET_STATISTICS(g_hResourceContext);
                  printf("\n QLIB_FTM_WLAN_GEN6_RESET_RX_PACKET_STATISTICS() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'd':
               case 'D':
               {
                  unsigned int iChainSelection;
                  char inputStr[80];

                  while( true )
                  {
                     printf("\n iChainSelection val (type '?' for value list)=");
                     scanf("%s", inputStr );
                     if( inputStr[0] == '?' )
                     {
                        printf("\n 4: PHY_CHAIN_SEL_R0_ON");
                        printf("\n 5: PHY_CHAIN_SEL_R0R1_ON");
                        printf("\n 6: PHY_CHAIN_SEL_T0_ON");
                        printf("\n 7: PHY_CHAIN_SEL_T0_R1_ON");
                        printf("\n 8: PHY_CHAIN_SEL_R1_ON");
                        printf("\n 9: PHY_CHAIN_SEL_NO_RX_TX");
                        printf("\n");
                     }
                     else
                     {
                        sscanf(inputStr, "%u",&iChainSelection);
                        break;
                     }
                  }

                  bOk = QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(g_hResourceContext, iChainSelection);
                  printf("\n QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'e':
               case 'E':
               {
#define NUM_SAMPLES_PRIMA   512
#define NUM_SAMPLES_PER_CALL_PRIMA  128
                  unsigned long channel = 0;
                  if( wlanID == 3660 )
                  {
                     unsigned short numSamples = NUM_SAMPLES_PER_CALL_PRIMA;
                     short irx0[NUM_SAMPLES_PRIMA], qrx0[NUM_SAMPLES_PRIMA], irx1[NUM_SAMPLES_PRIMA], qrx1[NUM_SAMPLES_PRIMA];
                     for( unsigned short i = 0; i < NUM_SAMPLES_PRIMA; i += NUM_SAMPLES_PER_CALL_PRIMA )
                     {
                        bOk &= QLIB_FTM_WLAN_GEN6_GRAB_RAM(g_hResourceContext, i, numSamples, &irx0[i], &qrx0[i], &irx1[i], &qrx1[i] );
                     }
                     printf("\n QLIB_FTM_WLAN_GEN6_GRAB_RAM() = %s", bOk ? "PASS" : "FAIL" );
                     FILE *fp;
                     fp = fopen("c:\\WLANIQSamples.csv", "w");
                     if( fp)
                     {
                        for( int i = 0; i < NUM_SAMPLES_PRIMA; i++ )
                        {
                           fprintf(fp, "%d%s%di\n", irx0[i], ((qrx0[i] < 0)?"":"+"), qrx0[i]);
                        }
                        fclose(fp);
                     }
                  }
                  else
                  {
                     printf("\n Enter Channel=");
                     scanf("%d",&channel);

                     unsigned long channelBondingState = 0;
                     printf("\n Enter Channel Bonding State (0:none, 1:Primary Low, 3:Primary High)=");
                     scanf("%d",&channelBondingState);

                     bOk = QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP(g_hResourceContext,1);
                     bOk &= QLIB_FTM_WLAN_GEN6_SET_RX_DISABLE_MODE(g_hResourceContext,true,true,true);
                     bOk &= QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(g_hResourceContext,4);
                     bOk &= QLIB_FTM_WLAN_GEN6_SET_CHANNEL_V2(g_hResourceContext,channel,channelBondingState);
                     bOk &= QLIB_FTM_WLAN_GEN6_RX_DCO_CAL(g_hResourceContext);
                     bOk &= QLIB_FTM_WLAN_GEN6_DISABLE_AGC_TABLES(g_hResourceContext,90,90);

#define NUM_SAMPLES 4096
#define NUM_SAMPLES_PER_CALL 256
                     unsigned short numSamples = NUM_SAMPLES_PER_CALL;
                     short irx0[NUM_SAMPLES], qrx0[NUM_SAMPLES], irx1[NUM_SAMPLES], qrx1[NUM_SAMPLES];
                     memset( (void *)irx0, 0xaa, NUM_SAMPLES * sizeof(short));

                     for( unsigned short i = 0; i < NUM_SAMPLES; i += NUM_SAMPLES_PER_CALL )
                     {
                        bOk &= QLIB_FTM_WLAN_GEN6_GRAB_RAM(g_hResourceContext, i, numSamples, &irx0[i], &qrx0[i], &irx1[i], &qrx1[i] );
                     }
                     printf("\n QLIB_FTM_WLAN_GEN6_GRAB_RAM() = %s", bOk ? "PASS" : "FAIL" );
                     FILE *fp;
                     fp = fopen("c:\\WLANIQSamples.csv", "w");
                     if( fp)
                     {
                        for( int i = 0; i < 4096; i++ )
                        {
                           fprintf(fp, "%d%s%di\n", irx0[i], ((qrx0[i] < 0)?"":"+"), qrx0[i]);
                        }
                        fclose(fp);
                     }
                  }
               }
               break;

               case 'f':
               case 'F':
               {
                  long tone = 0;
                  printf("\n Enter Tone=");
                  scanf("%d",&tone);
                  unsigned long ampl = 0;
                  printf("\n Enter Ampl=");
                  scanf("%d",&ampl);

                  //bOk &= QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP(g_hResourceContext,0);
                  bOk &= QLIB_FTM_WLAN_GEN6_GEN_CW(g_hResourceContext, tone, ampl);

                  printf("\n QLIB_FTM_WLAN_GEN6_GEN_CW() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'g':
               case 'G':
               {
                  unsigned char gain = 0;
                  printf("\n Enter Tx Gain (31 to 256)=");
                  scanf("%d",&gain);

                  bOk &= QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN(g_hResourceContext, gain);
                  printf("\n QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'h':
               case 'H':
               {
                  bOk = QLIB_FTM_WLAN_GEN6_START_WAVEFORM(g_hResourceContext);
                  printf("\n QLIB_FTM_WLAN_GEN6_START_WAVEFORM() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'i':
               case 'I':
               {
                  bOk = QLIB_FTM_WLAN_GEN6_STOP_WAVEFORM(g_hResourceContext);
                  printf("\n QLIB_FTM_WLAN_GEN6_STOP_WAVEFORM() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'j':
               case 'J':
               {
                  unsigned long rate;
                  unsigned long cbState;
                  unsigned char chanID;
                  unsigned long pwrTemplateInx;
                  unsigned char gain;
                  unsigned char adc;
                  unsigned short rawadc;
                  unsigned char inxMinLut;
                  double inxMinAbs;
                  unsigned char inxMaxLut;
                  double inxMaxAbs;
                  unsigned char outLut;
                  double outAbs;

                  bOk = QLIB_FTM_WLAN_GEN6_GET_TX_POWER_REPORT(g_hResourceContext,&rate,&cbState,
                                                               &chanID, &pwrTemplateInx, &gain, &adc, &rawadc, &inxMinLut,
                                                               &inxMinAbs, &inxMaxLut, &inxMaxAbs, &outLut, &outAbs);

                  printf("\n QLIB_FTM_WLAN_GEN6_GET_TX_POWER_REPORT() = %s", bOk ? "PASS" : "FAIL" );
                  if( bOk )
                  {
                     printf("\n  rate: %u, chanID: %d, cbState: %u", rate, chanID, cbState);
                     printf("\n  pwrTemplateInx: %u, gain: %d, adc: %u, rawadc: %d", pwrTemplateInx, gain, adc, rawadc);
                     printf("\n  inxMinLut: %u, inxMinAbs: %6.2f", inxMinLut, inxMinAbs);
                     printf("\n  inxMaxLut: %u, inxMaxAbs: %6.2f", inxMaxLut, inxMaxAbs);
                     printf("\n  outLut: %u, outAbs: %6.2f", outLut, outAbs);
                  }
               }
               break;

               case 'k':
               case 'K':
               {
                  unsigned long legacy;
                  unsigned long gfSimo20;
                  unsigned long gfMimo20;
                  unsigned long mmSimo20;
                  unsigned long mmMimo20;
                  unsigned long txbShort;
                  unsigned long txbLong;
                  unsigned long txbSlr;
                  unsigned long total;
                  unsigned char txStatus;

                  bOk = QLIB_FTM_WLAN_GEN6_QUERY_TX_STATUS(g_hResourceContext,&legacy,
                                                           &gfSimo20, &gfMimo20, &mmSimo20, &mmMimo20,
                                                           &txbShort, &txbLong, &txbSlr, &total, &txStatus);

                  printf("\n QLIB_FTM_WLAN_GEN6_QUERY_TX_STATUS() = %s", bOk ? "PASS" : "FAIL" );
                  if( bOk )
                  {
                     printf("\n  legacy: %u, gfSimo20: %u, gfMimo20: %u", legacy, gfSimo20, gfMimo20);
                     printf("\n  mmSimo20: %u, mmMimo20: %u", mmSimo20, mmMimo20);
                     printf("\n  txbShort: %u, txbLong: %u, txbSlr: %u", txbShort, txbLong, txbSlr);
                     printf("\n  total: %u, txStatus: %u", total, txStatus);
                  }
               }
               break;
               case 'l':
               case 'L':
               {
                  if( wlanID == 3660 )
                  {
                     int tableId;
                     unsigned char table[80*256];   // Worst Case Table Size (pdChar)
                     char inputStr[80];
                     while( true )
                     {
                        printf("\n tableId val (type '?' for value list)=");
                        scanf("%s", inputStr );
                        if( inputStr[0] == '?' )
                        {
                           printf("RATE_POWER_SETTINGS  = 2\n");
                           printf("REGULATORY_DOMAINS   = 3\n");
                           printf("DEFAULT_COUNTRY      = 4\n");
                           printf("TPC_POWER_TABLE      = 5\n");
                           printf("TPC_PDADC_OFFSETS    = 6\n");
                           printf("RSSI_CHANNEL_OFFSETS = 9\n");
                           printf("\n");
                        }
                        else
                        {
                           sscanf(inputStr, "\n%d",&tableId);
                           break;
                        }
                     }
                     bOk = QLIB_FTM_WLAN_GEN6_GET_NV_TABLE(g_hResourceContext, tableId, table);

                     printf("\n QLIB_FTM_WLAN_GEN6_GET_NV_TABLE() = %s", bOk ? "PASS" : "FAIL" );

                     if( bOk )
                     {
                        if( tableId == 5 )
                        {
                           unsigned char ptbl[80][256];
                           memcpy( (void *)ptbl, (void *)table, 256*80);
                           for( int cval = 0; cval < 80; cval++ )
                           {
                              printf("\n");
                              for( int pval = 0; pval < 256; pval++ )
                              {
                                 printf("%02d ", ptbl[cval][pval]);
                              }
                           }
                        }
                        else if( tableId == 6 )
                        {
                           unsigned short padcOffsettbl[80];
                           memcpy( (void *)padcOffsettbl, (void *)table, 2*80);
                           printf("\n");
                           for( int cval = 0; cval < 80; cval++ )
                           {
                              printf("%02d ", padcOffsettbl[cval]);
                           }
                           printf("\n");
                        }
                     }
                  }
                  else
                  {
                     int tableId;
                     unsigned char table[14*128];
                     char inputStr[80];
                     while( true )
                     {
                        printf("\n tableId val (type '?' for value list)=");
                        scanf("%s", inputStr );
                        if( inputStr[0] == '?' )
                        {
                           printf("RATE_POWER_SETTINGS  = 2\n");
                           printf("REGULATORY_DOMAINS   = 3\n");
                           printf("DEFAULT_COUNTRY      = 4\n");
                           printf("TPC_POWER_TABLE      = 5\n");
                           printf("TPC_PDADC_OFFSETS    = 6\n");
                           printf("RSSI_CHANNEL_OFFSETS = 9\n");
                           printf("\n");
                        }
                        else
                        {
                           sscanf(inputStr, "\n%d",&tableId);
                           break;
                        }
                     }
                     bOk = QLIB_FTM_WLAN_GEN6_GET_NV_TABLE(g_hResourceContext, tableId, table);

                     printf("\n QLIB_FTM_WLAN_GEN6_GET_NV_TABLE() = %s", bOk ? "PASS" : "FAIL" );

                     if( bOk )
                     {
                        if( tableId == 5 )
                        {
                           unsigned char ptbl[14][128];
                           memcpy( (void *)ptbl, (void *)table, 128*14);
                           for( int pval = 0; pval < 128; pval++ )
                           {
                              printf("\n");
                              for( int cval = 0; cval < 14; cval++ )
                              {
                                 printf("%02d ", ptbl[cval][pval]);
                              }
                           }
                        }
                        else if( tableId == 6 )
                        {
                           unsigned short padcOffsettbl[14];
                           memcpy( (void *)padcOffsettbl, (void *)table, 2*14);
                           printf("\n");
                           for( int cval = 0; cval < 14; cval++ )
                           {
                              printf("%02d ", padcOffsettbl[cval]);
                           }
                           printf("\n");
                        }
                     }
                  }
               }
               break;
               case 'm':
               case 'M':
               {
                  int tableId;
                  char inputStr[80];
                  while( true )
                  {
                     printf("\n tableId val (type '?' for value list)=");
                     scanf("%s", inputStr );
                     if( inputStr[0] == '?' )
                     {
                        printf("RATE_POWER_SETTINGS  = 2\n");
                        printf("REGULATORY_DOMAINS   = 3\n");
                        printf("DEFAULT_COUNTRY      = 4\n");
                        printf("TPC_POWER_TABLE      = 5\n");
                        printf("TPC_PDADC_OFFSETS    = 6\n");
                        printf("RSSI_CHANNEL_OFFSETS = 9\n");
                        printf("\n");
                     }
                     else
                     {
                        sscanf(inputStr, "\n%d",&tableId);
                        break;
                     }
                  }

                  if( wlanID == 3660 )
                  {
                     if( tableId == 5 )
                     {
                        unsigned char ptbl[80][256];
                        for( int cval = 0; cval < 80; cval++ )
                        {
                           for( int pval = 0; pval < 256; pval++ )
                           {
                              ptbl[cval][pval] = (unsigned char)pval;
                           }
                        }
                        bOk = QLIB_FTM_WLAN_GEN6_SET_NV_TABLE(g_hResourceContext, tableId, (void *)ptbl);
                     }
                     else if( tableId == 6 )
                     {
                        unsigned short padcOffsettbl[80];
                        for( int i = 0; i < 80; i++ )
                        {
                           padcOffsettbl[i] = (unsigned short)i;
                        }
                        bOk = QLIB_FTM_WLAN_GEN6_SET_NV_TABLE(g_hResourceContext, tableId, (void *)padcOffsettbl);
                     }
                  }
                  else
                  {
                     if( tableId == 5 )
                     {
                        unsigned char ptbl[14][128];
                        for( int pval = 0; pval < 128; pval++ )
                        {
                           for( int cval = 0; cval < 14; cval++ )
                           {
                              ptbl[cval][pval] = (unsigned char)pval;
                           }
                        }
                        bOk = QLIB_FTM_WLAN_GEN6_SET_NV_TABLE(g_hResourceContext, tableId, (void *)ptbl);
                     }
                     else if( tableId == 6 )
                     {
                        unsigned short padcOffsettbl[14];
                        for( int i = 0; i < 14; i++ )
                        {
                           padcOffsettbl[i] = (unsigned short)i;
                        }
                        bOk = QLIB_FTM_WLAN_GEN6_SET_NV_TABLE(g_hResourceContext, tableId, (void *)padcOffsettbl);
                     }
                  }
                  printf("\n QLIB_FTM_WLAN_GEN6_SET_NV_TABLE() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'n':
               case 'N':
               {
                  unsigned char powerRange = 0;
                  printf("\n Enter Power Range (0=r2p (default), 1=11b, 2=ofdm, 3=fixed point)=");
                  scanf("%d",&powerRange);

                  unsigned char powerIndex = 0;
                  if( powerRange == 3 )
                  {
                     printf("\n Enter Power Index (0 to 31, Power dBm = 8.5 + (0.5 * Power Index) )=");
                     scanf("%d",&powerIndex);
                  }

                  bOk = QLIB_FTM_WLAN_GEN6_UPDATE_TPC_SPLIT_LUT(g_hResourceContext, powerRange, powerIndex);
                  printf("\n QLIB_FTM_WLAN_GEN6_UPDATE_TPC_SPLIT_LUT() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'o':
               case 'O':
               {
                  unsigned char lutValues[128];

                  bOk = QLIB_FTM_WLAN_GEN6_GET_POWER_LUT(g_hResourceContext, 0, lutValues);
                  printf("\n QLIB_FTM_WLAN_GEN6_GET_POWER_LUT() = %s", bOk ? "PASS" : "FAIL" );

                  printf("\n\n Power LUT values:\n");
                  for( int i = 0; i < 16; i++ )
                  {
                     for( int j = 0; j < 8; j++ )
                     {
                        printf("0x%02x", lutValues[ i*8 + j]);
                        if( !(i == 15 && j == 7) )
                        {
                           printf(", ");
                        }
                     }
                     printf("\n");
                  }
               }
               break;

               case 'r':
               case 'R':
               {
                  char fileName[80];
                  printf("\n Enter File Name=");
                  scanf("%s",&fileName);
                  bOk = QLIB_FTM_WLAN_GEN6_READ_NV_WRITE_XML(g_hResourceContext,fileName);
                  printf("\n QLIB_FTM_WLAN_GEN6_READ_NV_WRITE_XML() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 's':
               case 'S':
               {
                  int rfGain;
                  printf("\n Enter RF Gain (0-31)=");
                  scanf("%d",&rfGain);

                  unsigned char iVal, qVal;
                  bOk = QLIB_FTM_WLAN_GEN6_TX_CARRIER_SUPPRESS_CAL(g_hResourceContext,&iVal, &qVal, (unsigned int)rfGain);
                  printf("\n QLIB_FTM_WLAN_GEN6_TX_CARRIER_SUPPRESS_CAL() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 't':
               case 'T':
               {
                  if( wlanID == 3660 )
                  {
                     unsigned int arg1;
                     printf("\n Enter Arg 1 in hex (no 0x prefix)=");
                     scanf("%x",&arg1);
                     unsigned int arg2;
                     printf("\n Enter Arg 2 in hex (no 0x prefix)=");
                     scanf("%x",&arg2);
                     short dumpBuffer[512];
                     short dumpBuffer2[512];
                     short dumpBuffer3[512];
                     short dumpBuffer4[512];

                     bOk &= QLIB_FTM_WLAN_GEN6_GRAB_RAM(g_hResourceContext, arg1, arg2, &dumpBuffer[0], &dumpBuffer2[0], &dumpBuffer3[0], &dumpBuffer4[0] );

                     printf("\n QLIB_FTM_WLAN_GEN6_GRAB_RAM() = %s", bOk ? "PASS" : "FAIL" );
                     printf("\n" );
                     for( int i = 0; i < 512; i++ )
                     {
                        if( (i % 4) == 0 )
                        {
                           printf("\n" );
                        }
                        printf("%02x %02x %02x %02x ", (byte)((dumpBuffer[i] >> 8) & 0xff),
                               (byte)(dumpBuffer[i] & 0xff), (byte)((dumpBuffer2[i] >> 8) & 0xff), (byte)(dumpBuffer2[i] & 0xff) );
                     }
                  }
                  else
                  {
                     printf("\n HAP/PHY Dump command is only for WCN3660!");
                  }
               }
               break;

               case 'v':
               case 'V':
               {
                  unsigned int fwmaj, fwmin, fwpatch, fwbuild, drvmaj, drvmin, drvpatch, drvbuild, pttapimaj, pttapimin;

                  bOk = QLIB_FTM_WLAN_GEN6_GET_BUILD_RELEASE_NUMBER(g_hResourceContext, &fwmaj, &fwmin, &fwpatch, &fwbuild, &drvmaj, &drvmin, &drvpatch, &drvbuild, &pttapimaj, &pttapimin);
                  printf("\n QLIB_FTM_WLAN_GEN6_GET_BUILD_RELEASE_NUMBER() = %s", bOk ? "PASS" : "FAIL" );

                  printf("\n\nVersion Information:");
                  printf("\nFirmware: Version %d.%d Patch: %d Build %d", fwmaj, fwmin, fwpatch, fwbuild);
                  printf("\nDriver: Version %d.%d Patch: %d Build %d", drvmaj, drvmin, drvpatch, drvbuild);
                  printf("\nPttApi: Version %d.%d\n", pttapimaj, pttapimin);
               }
               break;

               case 'w':
               case 'W':
               {
                  char fileName[80];
                  printf("\n Enter File Name=");
                  scanf("%s",&fileName);
                  bOk = QLIB_FTM_WLAN_GEN6_READ_XML_WRITE_NV(g_hResourceContext,fileName);
                  printf("\n QLIB_FTM_WLAN_GEN6_READ_XML_WRITE_NV() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'x':
               case 'X':
               {
                  int gain;
                  printf("\n Enter Gain (0-15)=");
                  scanf("%d",&gain);

                  short coeffValuesI[5];
                  short coeffValuesQ[5];

                  bOk = QLIB_FTM_WLAN_GEN6_RX_IQ_CAL_V2(g_hResourceContext, gain, coeffValuesI, coeffValuesQ);
                  printf("\n QLIB_FTM_WLAN_GEN6_RX_IQ_CAL_V2() = %s", bOk ? "PASS" : "FAIL" );

                  printf("\n");
                  int i = 0;
                  for( i = 0; i < 5; i++ )
                  {
                     printf("\ncoeffValuesI[%d] = %d", i, coeffValuesI[i]);
                  }
                  for( i= 0; i < 5; i++ )
                  {
                     printf("\ncoeffValuesQ[%d] = %d", i, coeffValuesQ[i]);
                  }
                  printf("\n");
               }
               break;

               case 'y': //add case 'y' for  WLAN GOLDEN phone testing
               case 'Y':
               {
                  unsigned long iNumTestFrame;
                  unsigned int iPayloadSize;
                  unsigned long iFrameSpacing;
                  unsigned int iFCSCal;
                  unsigned long iRate;
                  unsigned long iPreamble;
                  unsigned long iTX_RX_mode;
                  char inputStr[80];

                  printf("\n iNumTestFrame val=");
                  scanf("%ul",&iNumTestFrame);

                  printf("\n iPayloadSize val=");
                  scanf("%u",&iPayloadSize);

                  printf("\n iFrameSpacing val(uSec)=");
                  scanf("%ul",&iFrameSpacing);

                  printf("\n iFCSCal val=");
                  scanf("\n%d",&iFCSCal);

                  while( true )
                  {
                     printf("\n iRate val (type '?' for value list)=");
                     scanf("%s", inputStr );
                     if( inputStr[0] == '?' )
                     {
                        printf("\n --- 802.11b Rates --- ");
                        printf("\n 0: HAL_PHY_RATE_11B_LONG_1_MBPS");
                        printf("\n 1: HAL_PHY_RATE_11B_LONG_2_MBPS");
                        printf("\n 2: HAL_PHY_RATE_11B_LONG_5_5_MBPS");
                        printf("\n 3: HAL_PHY_RATE_11B_LONG_11_MBPS");
                        printf("\n 4: HAL_PHY_RATE_11B_SHORT_2_MBPS");
                        printf("\n 5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS");
                        printf("\n 6: HAL_PHY_RATE_11B_SHORT_11_MBPS");
                        printf("\n --- Spica_Virgo 11A 20MHz Rates --- ");
                        printf("\n 7: HAL_PHY_RATE_11A_6_MBPS");
                        printf("\n 8: HAL_PHY_RATE_11A_9_MBPS");
                        printf("\n 9: HAL_PHY_RATE_11A_12_MBPS");
                        printf("\n10: HAL_PHY_RATE_11A_18_MBPS");
                        printf("\n11: HAL_PHY_RATE_11A_24_MBPS");
                        printf("\n12: HAL_PHY_RATE_11A_36_MBPS");
                        printf("\n13: HAL_PHY_RATE_11A_48_MBPS");
                        printf("\n14: HAL_PHY_RATE_11A_54_MBPS");
                        printf("\n --- 11A Duplicate 40MHz Rates --- ");
                        printf("\n15: HAL_PHY_RATE_11A_DUP_6_MBPS");
                        printf("\n16: HAL_PHY_RATE_11A_DUP_9_MBPS");
                        printf("\n17: HAL_PHY_RATE_11A_DUP_12_MBPS");
                        printf("\n18: HAL_PHY_RATE_11A_DUP_18_MBPS");
                        printf("\n19: HAL_PHY_RATE_11A_DUP_24_MBPS");
                        printf("\n20: HAL_PHY_RATE_11A_DUP_36_MBPS");
                        printf("\n21: HAL_PHY_RATE_11A_DUP_48_MBPS");
                        printf("\n22: HAL_PHY_RATE_11A_DUP_54_MBPS");
                        printf("\n --- MCS Index #0-15 (20MHz) --- ");
                        printf("\n23: HAL_PHY_RATE_MCS_1NSS_6_5_MBPS");
                        printf("\n24: HAL_PHY_RATE_MCS_1NSS_13_MBPS");
                        printf("\n25: HAL_PHY_RATE_MCS_1NSS_19_5_MBPS");
                        printf("\n26: HAL_PHY_RATE_MCS_1NSS_26_MBPS");
                        printf("\n27: HAL_PHY_RATE_MCS_1NSS_39_MBPS");
                        printf("\n28: HAL_PHY_RATE_MCS_1NSS_52_MBPS");
                        printf("\n29: HAL_PHY_RATE_MCS_1NSS_58_5_MBPS");
                        printf("\n30: HAL_PHY_RATE_MCS_1NSS_65_MBPS");
                        printf("\n31: HAL_PHY_RATE_MCS_1NSS_MM_SG_7_2_MBPS");
                        printf("\n32: HAL_PHY_RATE_MCS_1NSS_MM_SG_14_4_MBPS");
                        printf("\n33: HAL_PHY_RATE_MCS_1NSS_MM_SG_21_7_MBPS");
                        printf("\n34: HAL_PHY_RATE_MCS_1NSS_MM_SG_28_9_MBPS");
                        printf("\n35: HAL_PHY_RATE_MCS_1NSS_MM_SG_43_3_MBPS");
                        printf("\n36: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS");
                        printf("\n37: HAL_PHY_RATE_MCS_1NSS_MM_SG_57_8_MBPS");
                        printf("\n38: HAL_PHY_RATE_MCS_1NSS_MM_SG_65_MBPS");
                        printf("\n39: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS");
                        printf("\n --- MCS Index #0-15 (40MHz) --- ");
                        printf("\n40: HAL_PHY_RATE_MCS_1NSS_CB_13_5_MBPS");
                        printf("\n41: HAL_PHY_RATE_MCS_1NSS_CB_27_MBPS");
                        printf("\n42: HAL_PHY_RATE_MCS_1NSS_CB_40_5_MBPS");
                        printf("\n43: HAL_PHY_RATE_MCS_1NSS_CB_54_MBPS");
                        printf("\n44: HAL_PHY_RATE_MCS_1NSS_CB_81_MBPS");
                        printf("\n45: HAL_PHY_RATE_MCS_1NSS_CB_108_MBPS");
                        printf("\n46: HAL_PHY_RATE_MCS_1NSS_CB_121_5_MBPS");
                        printf("\n47: HAL_PHY_RATE_MCS_1NSS_CB_135_MBPS");
                        printf("\n48: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_15_MBPS");
                        printf("\n49: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_30_MBPS");
                        printf("\n50: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_45_MBPS");
                        printf("\n51: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_60_MBPS");
                        printf("\n52: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_90_MBPS");
                        printf("\n53: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_120_MBPS");
                        printf("\n54: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_135_MBPS");
                        printf("\n55: HAL_PHY_RATE_MCS_1NSS_MM_SG_CB_150_MBPS");
                        printf("\n");
                     }
                     else
                     {
                        sscanf(inputStr, "\n%ul",&iRate);
                        break;
                     }
                  }
                  while( true )
                  {
                     printf("\n iPreamble val (type '?' for value list)=");
                     scanf("%s", inputStr );
                     if( inputStr[0] == '?' )
                     {
                        printf("\n 0: PHYDBG_PREAMBLE_OFDM");
                        printf("\n 1: PHYDBG_PREAMBLE_GREENFIELD");
                        printf("\n 2: PHYDBG_PREAMBLE_MIXED");
                        printf("\n 3: PHYDBG_PREAMBLE_SHORTB");
                        printf("\n 4: PHYDBG_PREAMBLE_LONGB");
                        printf("\n");
                     }
                     else
                     {
                        sscanf(inputStr, "%ul",&iPreamble);
                        break;
                     }
                  }

                  while( true )
                  {
                     printf("\n select TX or RX (type '?' for value list)=");
                     scanf("%s", inputStr );
                     if( inputStr[0] == '?' )
                     {
                        printf("\n 1: Set TX mode");
                        printf("\n 2: Set RX mode");
                        printf("\n");
                     }
                     else
                     {
                        sscanf(inputStr, "%ul",&iTX_RX_mode);
                        break;
                     }
                  }

                  int __mac1[6];
                  printf("\n Enter MAC1 address (comma separated hex values with no '0x' prefix): =");

                  scanf("%x,%x,%x,%x,%x,%x",&__mac1[0],&__mac1[1],&__mac1[2],&__mac1[3],&__mac1[4],&__mac1[5]);

                  unsigned char mac1[6];
                  int i = 0;
                  for( i = 0; i < 6; i++ )
                  {
                     mac1[i] = (unsigned char)__mac1[i];
                  }

                  int __mac2[6];
                  printf("\n Enter MAC2 address (comma separated hex values with no '0x' prefix): =");

                  scanf("%x,%x,%x,%x,%x,%x",&__mac2[0],&__mac2[1],&__mac2[2],&__mac2[3],&__mac2[4],&__mac2[5]);

                  unsigned char mac2[6];
                  for( i = 0; i < 6; i++ )
                  {
                     mac2[i] = (unsigned char)__mac2[i];
                  }

                  int __mac3[6];
                  printf("\n Enter MAC3 address (comma separated hex values with no '0x' prefix): =");

                  scanf("%x,%x,%x,%x,%x,%x",&__mac3[0],&__mac3[1],&__mac3[2],&__mac3[3],&__mac3[4],&__mac3[5]);

                  unsigned char mac3[6];
                  for( i = 0; i < 6; i++ )
                  {
                     mac3[i] = (unsigned char)__mac3[i];
                  }

                  bOk = QLIB_FTM_WLAN_GEN6_SET_TX_FRAME_PHONE_TEST_SET_RX_TX(g_hResourceContext, iNumTestFrame, (unsigned short)iPayloadSize, iFrameSpacing, (unsigned char)iFCSCal, iRate, iPreamble, (unsigned short)iTX_RX_mode, mac1, mac2, mac3);
                  printf("\n QLIB_FTM_WLAN_GEN6_SET_TX_FRAME_PHONE_TEST_SET_RX_TX() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'z':
               case 'Z':
               {
                  int digGain;
                  printf("\n Enter Digital Gain (0-23)=");
                  scanf("%d",&digGain);
                  int rfGain;
                  printf("\n Enter RF Gain (0-31)=");
                  scanf("%d",&rfGain);

                  int gain = (rfGain << 16) | digGain;

                  bOk = QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2(g_hResourceContext, gain);
                  printf("\n QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '!':
               {
                  int gain;
                  printf("\n Enter Gain (0-31)=");
                  scanf("%d",&gain);

                  short iqAmpImbCoeff;
                  short loLeakageI;
                  short loLeakageQ;
                  short phaseImbalaceCoeffI[5];
                  short phaseImbalaceCoeffQ[5];

                  bOk = QLIB_FTM_WLAN_GEN6_TX_IQ_CAL_V2(g_hResourceContext, gain, &iqAmpImbCoeff, &loLeakageI, &loLeakageQ, phaseImbalaceCoeffI, phaseImbalaceCoeffQ);
                  printf("\n QLIB_FTM_WLAN_GEN6_TX_IQ_CAL_V2() = %s", bOk ? "PASS" : "FAIL" );

                  printf("\n");
                  printf("\niqAmpImbCoeff = %d", iqAmpImbCoeff);
                  printf("\nloLeakageI = %d", loLeakageI);
                  printf("\nloLeakageQ = %d", loLeakageQ);

                  int i = 0;
                  for( i= 0; i < 5; i++ )
                  {
                     printf("\nphaseImbalaceCoeffI[%d] = %d", i, phaseImbalaceCoeffI[i]);
                  }
                  for( i= 0; i < 5; i++ )
                  {
                     printf("\nphaseImbalaceCoeffQ[%d] = %d", i, phaseImbalaceCoeffQ[i]);
                  }
                  printf("\n");
               }
               break;

               case '@':
               {
                  int addr;
                  printf("\n Enter Register Address in hex (no 0x prefix)=");
                  scanf("%x",&addr);
                  unsigned int regValue;

                  bOk = QLIB_FTM_WLAN_GEN6_READ_REGISTER(g_hResourceContext, addr, &regValue);
                  printf("\n QLIB_FTM_WLAN_GEN6_READ_REGISTER() = %s", bOk ? "PASS" : "FAIL" );
                  if( bOk )
                  {
                     printf("\n\nReg Address 0x%x = 0x%x", addr, regValue );
                  }
               }
               break;

               case '#':
               {
                  double txPower;
                  printf("\n Enter Tx Power in 0.5 db increments=");
                  scanf("%d",&txPower);

                  unsigned char plut = 1;

                  bOk = QLIB_FTM_WLAN_GEN6_CLPC_SW_CAL(g_hResourceContext, txPower, 22.0, 6.0, plut);
                  printf("\n QLIB_FTM_WLAN_GEN6_CLPC_SW_CAL() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '$':
               {
                  //FTM_WLAN_GEN6_GET_UNICAST_MAC_PKT_RSSI
                  short rssiVals[2];
                  unsigned short numRxChains;
                  bOk = QLIB_FTM_WLAN_GEN6_GET_UNICAST_MAC_PKT_RSSI(g_hResourceContext, rssiVals, &numRxChains);
                  printf("\n QLIB_FTM_WLAN_GEN6_GET_UNICAST_MAC_PKT_RSSI() = %s", bOk ? "PASS" : "FAIL" );
                  if( numRxChains == 1 )
                  {
                     printf("\n    rssi: %d", rssiVals[0] );
                  }
                  else
                  {
                     printf("\n    rssi 0: %d  rssi 1: %d", rssiVals[0], rssiVals[1] );
                  }
               }
               break;

               case '%':
               {
                  //FTM_WLAN_GEN6_GET_UNICAST_MAC_PKT_RSSI_CONF
                  unsigned long arg;
                  printf("\n Enter Argument value=");
                  scanf("%ld",&arg);

                  bOk = QLIB_FTM_WLAN_GEN6_GET_UNICAST_MAC_PKT_RSSI_CONF(g_hResourceContext, arg);
                  printf("\n QLIB_FTM_WLAN_GEN6_GET_UNICAST_MAC_PKT_RSSI_CONF() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;
               //FTM_WLAN_GEN6_CLPC_CAL_EXTRA_MEASUREMENT
               case '^':
               {
                  int setup;
                  printf("\n Enter setup value=");
                  scanf("%d",&setup);
                  int setupTxDmdPwrOffset;
                  printf("\n Enter setupTxDmdPwrOffset value=");
                  scanf("%d",&setupTxDmdPwrOffset);
                  int measureTotalExtraPt;
                  printf("\n Enter measureTotalExtraPt value=");
                  scanf("%d",&measureTotalExtraPt);
                  int measureCurrentPtIdx;
                  printf("\n Enter measureCurrentPtIdx value=");
                  scanf("%d",&measureCurrentPtIdx);

                  bOk = QLIB_FTM_WLAN_GEN6_CLPC_CAL_EXTRA_MEASUREMENT(g_hResourceContext, (unsigned short)setup,(unsigned short)setupTxDmdPwrOffset,(unsigned short)measureTotalExtraPt,(unsigned short)measureCurrentPtIdx,0 );
                  printf("\n QLIB_FTM_WLAN_GEN6_CLPC_CAL_EXTRA_MEASUREMENT() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;
               case '&':
               {
                  //QLIB_FTM_WLAN_GEN6_ENABLE_DPD
                  int enable;
                  printf("\n Enter 0 to disable, 1 to enable=");
                  scanf("%d",&enable);
                  unsigned char benable = (unsigned char)enable;

                  bOk = QLIB_FTM_WLAN_GEN6_ENABLE_DPD(g_hResourceContext, enable);
                  printf("\n QLIB_FTM_WLAN_GEN6_ENABLE_DPD() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '*':
               {
                  //QLIB_FTM_WLAN_GEN6_GENERIC_CMD
                  unsigned int cmdIndex;
                  unsigned int parm1;
                  unsigned int parm2;
                  unsigned int parm3;
                  unsigned int parm4;
                  printf("\n Enter command index=");
                  scanf("%u",&cmdIndex);
                  printf("\n Enter parameter1=");
                  scanf("%u",&parm1);
                  printf("\n Enter parameter2=");
                  scanf("%u",&parm2);
                  printf("\n Enter parameter3=");
                  scanf("%u",&parm3);
                  printf("\n Enter parameter4=");
                  scanf("%u",&parm4);

                  bOk = QLIB_FTM_WLAN_GEN6_GENERIC_CMD(g_hResourceContext, cmdIndex, parm1, parm2, parm3, parm4);
                  printf("\n QLIB_FTM_WLAN_GEN6_GENERIC_CMD() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'q':
               case 'Q':
               {
                  wlanGen6Loop = false;;
               }
               break;

               default:
                  printf("\n Invalid test case choice!!!");
                  break;
            }
         }
      }
      else if (cKey == '1' )
      {
         bOk = QLIB_FTM_WLAN_SetModuleType( g_hResourceContext, FTM_WLAN_ModuleType_Philips);
         printf("\nQLIB_FTM_WLAN_SetModuleType() = %s", bOk ? "PASS" : "FAIL" );

         /////////////////////////
         // START
         /////////////////////////
         bOk = QLIB_FTM_WLAN_Philips_START( g_hResourceContext, &_iPhilipsStatus );
         printf("\n\nQLIB_FTM_WLAN_Philips_START() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         /////////////////////////
         // TX_CONTINUOUS
         /////////////////////////
         bOk = QLIB_FTM_WLAN_Philips_TX_CONTINUOUS
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  1,                              // unsigned char iChannelNo,
                  0,                              // unsigned char iAntenna,
                  FTM_WLAN_Philips_RATE_1_MBPS,   // unsigned short iRates,--IGNORED
                  0,                              // unsigned char iMode,--IGNORED
                  0x10,                           // unsigned char iTxGain,--Always this value
                  FTM_WLAN_Philips_SINGLETONE_100KHZ
               );
         printf("\n\nQLIB_FTM_WLAN_Philips_TX_CONTINUOUS() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         /////////////////////////
         // TX_BURST
         /////////////////////////
         bOk = QLIB_FTM_WLAN_Philips_TX_BURST
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  1,                              // unsigned char iChannelNo,
                  1,                              // unsigned char iAntenna,
                  FTM_WLAN_Philips_RATE_1_MBPS,   // unsigned char iRates,
                  0,                              // unsigned char bPreamble,
                  0,                              // unsigned short iPacketLength,
                  0,                              // unsigned short iPacketInterval,
                  0                               // unsigned long iNumberOfPackets (0=infinite)
               );
         printf("\n\nQLIB_FTM_WLAN_Philips_TX_BURST() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         /////////////////////////
         // TX_POWER_CONTROL
         /////////////////////////
         bOk = QLIB_FTM_WLAN_Philips_TX_POWER_CONTROL
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  16384,                          // unsigned short iTxBB,
                  1,                              // unsigned char iTxLF,
                  2                               // unsigned char iTxRF
               );
         printf("\n\nQLIB_FTM_WLAN_Philips_TX_POWER_CONTROL() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         /////////////////////////
         // RX_CONTINUOUS
         /////////////////////////
         bOk = QLIB_FTM_WLAN_Philips_RX_CONTINUOUS
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  2,                      // unsigned char iChannelNo,
                  1                       // unsigned char iAntenna
               );
         printf("\n\nQLIB_FTM_WLAN_Philips_RX_CONTINUOUS() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         /////////////////////////
         // GET_MAC_ADDRESS
         /////////////////////////
         unsigned char _aiMAC_Address[ FTM_WLAN_MAC_ADRESS_SIZE ];
         bOk = QLIB_FTM_WLAN_Philips_GET_MAC_ADDRESS
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  _aiMAC_Address
               );
         printf("\n\nQLIB_FTM_WLAN_Philips_GET_MAC_ADDRESS() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );
         if ( bOk)
            printf("\nQLIB_FTM_WLAN_Philips_GET_MAC_ADDRESS( 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X )",
                   _aiMAC_Address[0],
                   _aiMAC_Address[1],
                   _aiMAC_Address[2],
                   _aiMAC_Address[3],
                   _aiMAC_Address[4],
                   _aiMAC_Address[5]
                  );

         /////////////////////////
         // SET_MAC_ADDRESS
         /////////////////////////
         unsigned char _aiMAC_Address_set[ FTM_WLAN_MAC_ADRESS_SIZE ] = {1,2,3,4,5,6 };
         bOk = QLIB_FTM_WLAN_Philips_SET_MAC_ADDRESS
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  _aiMAC_Address_set
               );
         printf("\n\nQLIB_FTM_WLAN_Philips_SET_MAC_ADDRESS() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         /////////////////////////
         // GET_HWSW_VERSIONS
         /////////////////////////
         unsigned short _iSwVer = 0;
         unsigned short _iHwBBChipVer = 0;
         unsigned short _iHwRFChipVer = 0;
         unsigned short _iEEPROMVerFirmware = 0;
         unsigned short _iEEPROMVerManufacture = 0;
         unsigned short _iEEPROMVerCustomer = 0;
         bOk = QLIB_FTM_WLAN_Philips_GET_HWSW_VERSIONS
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  &_iSwVer,
                  &_iHwBBChipVer,
                  &_iHwRFChipVer,
                  &_iEEPROMVerFirmware,
                  &_iEEPROMVerManufacture,
                  & _iEEPROMVerCustomer
               );

         printf("\n\nQLIB_FTM_WLAN_Philips_GET_HWSW_VERSIONS() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );
         if ( bOk )
            printf("\ni     SwVer = %d, iHwBBChipVer = %d, iHwRFChipVer  = %d )", _iSwVer, _iHwBBChipVer, _iHwRFChipVer );

         /////////////////////////
         // TX_POWER_SET_CALIBRATED
         /////////////////////////
         unsigned long _iAddress = 0x0;
         bOk = QLIB_FTM_WLAN_Philips_TX_POWER_SET_CALIBRATED
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  10,//  iPower_dBm
                  &_iAddress
               );

         printf("\n\nQLIB_FTM_WLAN_Philips_TX_POWER_SET_CALIBRATED() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         /////////////////////////
         // READ_STATISTICS_Partial
         /////////////////////////
         unsigned long _iNumRxFrames_ant1 = 0;
         unsigned long _iNumRxBytesOk_ant1 = 0;
         unsigned long _iRSSI_OK_ant1 = 0;
         unsigned long _iRSSI_NOK_ant1 = 0;
         unsigned long _iNumRxFrames_ant2 = 0;
         unsigned long _iNumRxBytesOk_ant2 = 0;
         unsigned long _iRSSI_OK_ant2 = 0;
         unsigned long _iRSSI_NOK_ant2 = 0;

         bOk = QLIB_FTM_WLAN_Philips_READ_STATISTICS_Partial
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  &_iNumRxFrames_ant1,
                  &_iNumRxBytesOk_ant1,
                  &_iRSSI_OK_ant1,
                  &_iRSSI_NOK_ant2,
                  &_iNumRxFrames_ant2,
                  &_iNumRxBytesOk_ant2,
                  &_iRSSI_OK_ant2,
                  &_iRSSI_NOK_ant2
               );

         printf("\n\nQLIB_FTM_WLAN_Philips_READ_STATISTICS_Partial() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         /////////////////////////
         // RESET_STATISTICS
         /////////////////////////
         bOk = QLIB_FTM_WLAN_Philips_RESET_STATISTICS(
                  g_hResourceContext,
                  &_iPhilipsStatus
               );
         printf("\n\nQLIB_FTM_WLAN_Philips_RESET_STATISTICS() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         /////////////////////////
         // SLEEP_MODE
         /////////////////////////
         bOk = QLIB_FTM_WLAN_Philips_SLEEP_MODE
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  FTM_WLAN_Philips_PCTI_TIMED_SLEEP,      //  unsigned char iMode,
                  2000,                                   // unsigned long iSleepTime_ms,
                  0                                       // unsigned char iReserved
               );
         printf("\n\nQLIB_FTM_WLAN_Philips_SLEEP_MODE() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         /////////////////////////
         // LOOPBACK_MODE
         /////////////////////////
         bOk = QLIB_FTM_WLAN_Philips_LOOPBACK_MODE
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  1,                                      // unsigned char iChannel,
                  0                                       // unsigned long iIfs
               );
         printf("\n\nQLIB_FTM_WLAN_Philips_LOOPBACK_MODE() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         /////////////////////////
         // DEBUG_LEVEL
         /////////////////////////
         bOk = QLIB_FTM_WLAN_Philips_DEBUG_LEVEL
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  1                                       //unsigned char iBitmask
               );
         printf("\n\nQLIB_FTM_WLAN_Philips_DEBUG_LEVEL() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         /////////////////////////
         // User specified command buffer
         /////////////////////////
         const unsigned short c_iRequestSize  = 2;  // Size of WLAN request packet
         unsigned short iResponseSize = 0;  // This will get updated by SendSync

         // This request buffer is for the Philips START command
         static unsigned char aiRequestBuffer  [ c_iRequestSize  ] = { FTM_WLAN_Philips_PCTI_START, FTM_WLAN_Philips_SOURCE };
         static unsigned char aiResponseBuffer [ DIAG_MAX_PACKET_SIZE ];

         bOk = QLIB_FTM_WLAN_USER_CMD
               (
                  g_hResourceContext,
                  c_iRequestSize,
                  aiRequestBuffer,
                  &iResponseSize,
                  aiResponseBuffer
               );
         printf("\n\nQLIB_FTM_WLAN_USER_CMD() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );
      }// cKey == 1
      else if (cKey == '2')
      {
         bOk = QLIB_FTM_WLAN_SetModuleType( g_hResourceContext, FTM_WLAN_ModuleType_Philips);
         printf("\nQLIB_FTM_WLAN_SetModuleType() = %s", bOk ? "PASS" : "FAIL" );

         unsigned char _aiMAC_Address[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
         bOk = ::QLIB_FTM_WLAN_Philips_SET_MAC_ADDRESS
               (   g_hResourceContext,
                   &_iPhilipsStatus,
                   _aiMAC_Address
               );
         printf("\n\nQLIB_FTM_WLAN_Philips_SET_MAC_ADDRESS() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );

         unsigned char _aoMAC_Address[ FTM_WLAN_MAC_ADRESS_SIZE ];
         bOk = QLIB_FTM_WLAN_Philips_GET_MAC_ADDRESS
               (
                  g_hResourceContext,
                  &_iPhilipsStatus,
                  _aoMAC_Address
               );
         printf("\n\nQLIB_FTM_WLAN_Philips_GET_MAC_ADDRESS() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );
         if ( bOk)
            printf("\nQLIB_FTM_WLAN_Philips_GET_MAC_ADDRESS( 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X )",
                   _aoMAC_Address[0],
                   _aoMAC_Address[1],
                   _aoMAC_Address[2],
                   _aoMAC_Address[3],
                   _aoMAC_Address[4],
                   _aoMAC_Address[5]
                  );
      } // cKey == 2
      else if (cKey == '3')
      {
         bOk = ::QLIB_FTM_WLAN_Philips_STOP(g_hResourceContext, &_iPhilipsStatus);
         printf("\n\nQLIB_FTM_WLAN_Philips_STOP() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );
      }
      else if (cKey == '4')
      {
         unsigned char _iChannel = 1;
         unsigned char _iAntenna = 1;
         bOk = QLIB_FTM_WLAN_Philips_RX_CONTINUOUS(g_hResourceContext, &_iPhilipsStatus, _iChannel, _iAntenna);
         printf("\n\nQLIB_FTM_WLAN_Philips_RX_CONTINUOUS() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );
      }
      else if (cKey == '5')
      {
         bOk = QLIB_FTM_WLAN_Philips_TX_BURST(g_hResourceContext, &_iPhilipsStatus, 5, 1, FTM_WLAN_Philips_RATE_54_MBPS, 0x1, 1000, 1000, 0x0);
         printf("\n\nQLIB_FTM_WLAN_Philips_TX_BURST() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );
      }

      else if (cKey == '6')
      {
         bOk = QLIB_FTM_WLAN_Philips_TX_CONTINUOUS(g_hResourceContext, &_iPhilipsStatus, 1, 1, 0, 0, 0x10, FTM_WLAN_Philips_SINGLETONE_5d5MHZ);
         printf("\n\nQLIB_FTM_WLAN_Philips_TX_CONTINUOUS() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );
      }
      else if (cKey == '7')
      {
         bOk = QLIB_FTM_WLAN_Philips_TX_POWER_CONTROL(g_hResourceContext, &_iPhilipsStatus, 0x4000,0x7, 0x7);
         printf("\n\nQLIB_FTM_WLAN_Philips_TX_POWER_CONTROL() = %s, status = %d", bOk ? "PASS" : "FAIL", _iPhilipsStatus );
      }
      else if (cKey == '8')
      {
         unsigned char _aiStatBuffer[128];
         bOk = ::QLIB_FTM_WLAN_Philips_READ_STATISTICS_Full(g_hResourceContext, &_iPhilipsStatus, _aiStatBuffer);
         printf("\n\nQLIB_FTM_WLAN_Philips_READ_STATISTICS_Full() = %s, status = %d\n", bOk ? "PASS" : "FAIL", _iPhilipsStatus );
         if(bOk)
         {
            unsigned long * pAddress = (unsigned long*)_aiStatBuffer;
            int i = 0;
            for(i = 0; i < 32; i ++)
            {
               unsigned long value = 0;

               value += (*(_aiStatBuffer + (i*4)));
               value += ((*(_aiStatBuffer + (i*4) + 1)) << 8);
               value += ((*(_aiStatBuffer + (i*4) + 2)) << 16);
               value += ((*(_aiStatBuffer + (i*4) + 3)) << 24);

               printf("Address=0x%X value = 0x%X\n", i*4, value);
            }
         }
      }
      else if (cKey == '9')
      {
         // printf("\n9=Philips 32kHz clock status");

         unsigned char _iClockStatus;
         bOk = QLIB_FTM_WLAN_Philips_CHECK_EXT_32KHZ_CLOCK(g_hResourceContext, &_iPhilipsStatus, &_iClockStatus );
         printf("\n\nQLIB_FTM_WLAN_Philips_CHECK_EXT_32KHZ_CLOCK() = %s, status = %d\n", bOk ? "PASS" : "FAIL", _iPhilipsStatus );
         if(bOk)
         {
            printf("Clock Status = %d", _iClockStatus );
         }

         // Status check
         QLIB_FTM_WLAN_Philips_GetLastStatus(
            g_hResourceContext,
            &_iPhilipsStatus
         );
         printf("\n\nQLIB_FTM_WLAN_Philips_GetLastStatus(), status = %d\n", _iPhilipsStatus );
      }
      // Atheros CONT_TRANSMIT
      else if(cKey == 'a')
      {
         bool AtherosTestLoop = true;

         while(AtherosTestLoop)
         {
            printf("\n\n Choose Atheros WLAN FTM test you would like to do\n");

            printf("\n1=QLIB_FTM_WLAN_Atheros_V4_CONT_TRANSMIT");
            printf("\n2=QLIB_FTM_WLAN_Atheros_V3_CONT_RECEIVE");
            printf("\n3=QLIB_FTM_WLAN_Atheros_V4_RECEIVE_REPORT");
            printf("\n4=QLIB_FTM_WLAN_Atheros_V3_SET_MAC_ADDRESS FOR RX FILTER");
            printf("\n5=QLIB_FTM_WLAN_Atheros_V3_SLEEP_WAKE");
            printf("\n6=QLIB_FTM_WLAN_Atheros_V3_SET_NIC_MAC_ADDRESS");
            printf("\n7=QLIB_FTM_WLAN_Atheros_V3_SET_CALIBRATION_FILE_INDEX");
            printf("\n8=QLIB_FTM_WLAN_Atheros_GET_THERM");
            printf("\n\n Choose a test case to execute, q to exit submenu");

            cKey = getch();
            printf("\nYou chose %c\n", cKey);
            switch (cKey)
            {
               case '1':
               {
                  unsigned long mode = 0;
                  printf("\n Enter Mode (0: OFF, 1: SINE, 2: FRAME, 3: TX99, 4: TX100) =");
                  scanf("%ld",&mode);

                  unsigned long freq = 0;
                  printf("\n Enter Freq in MHz =");
                  scanf("%ld",&freq);

                  unsigned long dataRate = 0;
                  printf("\n Enter Data Rate =");
                  scanf("%ld",&dataRate);

                  double txPower = 0.0;
                  printf("\n Enter Tx Power in 0.5 dB steps =");
                  scanf("%lf",&txPower);

                  unsigned long scrambler = 0;
                  printf("\n Enter 1 for scrambler off, 0 for scramber on =");
                  scanf("%ld",&scrambler);

                  unsigned long tpcm = 0;
                  printf("\n Enter 0 txpower, 1 forced gain =");
                  scanf("%ld",&tpcm);

                  unsigned long antenna = 1;
                  unsigned long enANI = 0;
                  unsigned long scramblerOff = scrambler;
                  unsigned long aifsn = 0;
                  unsigned long pktSz = 1500;
                  unsigned long pattern = 3;

                  bOk = QLIB_FTM_WLAN_Atheros_V4_CONT_TRANSMIT( g_hResourceContext, mode, freq, dataRate,
                                                                txPower, antenna, enANI, scramblerOff, aifsn, pktSz, pattern, 0, 0, 1, tpcm);
                  printf("\n QLIB_FTM_WLAN_Atheros_V4_CONT_TRANSMIT() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '2':
               {
                  unsigned long action = 0;
                  printf("\n Enter Mode (0: PROMISCUOUS, 1: MAC FILTERED, 2: OFF) =");
                  scanf("%ld",&action);

                  unsigned long freq = 0;
                  printf("\n Enter Freq in MHz =");
                  scanf("%ld",&freq);

                  bOk = QLIB_FTM_WLAN_Atheros_V3_CONT_RECEIVE( g_hResourceContext, action, 0, freq, 1);
                  printf("\n QLIB_FTM_WLAN_Atheros_V3_CONT_RECEIVE() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '3':
               {
                  unsigned long totalPkts, crcErrPkts, secErrPkts;
                  double rssiInDbm;

                  bOk = QLIB_FTM_WLAN_Atheros_V4_RECEIVE_REPORT( g_hResourceContext, &totalPkts, &rssiInDbm, &crcErrPkts, &secErrPkts );
                  if( bOk != 0 )
                  {
                     printf("\n Total Packet Count = %d", totalPkts );
                     printf("\n Rssi = %lf dBm", rssiInDbm  );
                     printf("\n Crc Error Packet Count = %d", crcErrPkts );
                     printf("\n Sec Error Packet Count = %d", secErrPkts );
                  }
                  printf("\n QLIB_FTM_WLAN_Atheros_V4_RECEIVE_REPORT() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '4':
               {
                  int __mac[6];
                  printf("\n Enter MAC (comma separated hex values with no '0x' prefix): =");
                  scanf("%x,%x,%x,%x,%x,%x",&__mac[0],&__mac[1],&__mac[2],&__mac[3],&__mac[4],&__mac[5]);

                  unsigned char mac[6];
                  for( int i = 0; i < 6; i++ )
                  {
                     mac[i] = (unsigned char)__mac[i];
                  }

                  bOk = QLIB_FTM_WLAN_Atheros_V3_SET_MAC_ADDRESS( g_hResourceContext, mac);
                  printf("\n QLIB_FTM_WLAN_Atheros_V3_SET_MAC_ADDRESS() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '5':
               {
                  int __mode;
                  printf("\n Enter mode (1: wakeup, 2: sleep, 3: deep sleep): =");
                  scanf("%d",&__mode);
                  unsigned char mode = (unsigned char)__mode;

                  bOk = QLIB_FTM_WLAN_Atheros_V3_SLEEP_WAKE( g_hResourceContext, mode);
                  printf("\n QLIB_FTM_WLAN_Atheros_V3_SLEEP_WAKE() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '6':
               {
                  int __mac[6];
                  printf("\n Enter MAC (comma separated hex values with no '0x' prefix): =");
                  scanf("%x,%x,%x,%x,%x,%x",&__mac[0],&__mac[1],&__mac[2],&__mac[3],&__mac[4],&__mac[5]);

                  unsigned char mac[6];
                  for( int i = 0; i < 6; i++ )
                  {
                     mac[i] = (unsigned char)__mac[i];
                  }

                  bOk = QLIB_FTM_WLAN_Atheros_V3_SET_NIC_MAC_ADDRESS( g_hResourceContext, mac);
                  printf("\n QLIB_FTM_WLAN_Atheros_V3_SET_NIC_MAC_ADDRESS() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '7':
               {
                  int calFileIndex;
                  printf("\n Enter Calibration File Index: =");
                  scanf("%d",&calFileIndex);

                  bOk = QLIB_FTM_WLAN_Atheros_V3_SET_CALIBRATION_FILE_INDEX( g_hResourceContext, calFileIndex);
                  printf("\n QLIB_FTM_WLAN_Atheros_V3_SET_CALIBRATION_FILE_INDEX() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '8':
               {
                  unsigned char thermVal;

                  bOk = QLIB_FTM_WLAN_Atheros_GET_THERM( g_hResourceContext, &thermVal);
                  printf("\n QLIB_FTM_WLAN_Atheros_GET_THERM() = %s", bOk ? "PASS" : "FAIL" );
                  if( bOk != 0 )
                  {
                     printf("\n Therm Val = %d", thermVal );
                  }
               }
               break;

               case 'q':
               case 'Q':
               {
                  AtherosTestLoop = false;;
               }
               break;

               default:
                  printf("\n Invalid test case choice!!!");
                  break;
            }
         }
      }
      else if(cKey == 'b')
      {
         bool AtherosTestLoop = true;

         while(AtherosTestLoop)
         {
            printf("\n\n Choose Atheros WLAN FTM test you would like to do\n");

            printf("\n1=QLIB_FTM_WLAN_Atheros_Tx");
            printf("\n2=QLIB_FTM_WLAN_Atheros_Rx");
            printf("\n3=QLIB_FTM_WLAN_Atheros_Rx_Report");
            printf("\n\n Choose a test case to execute, q to exit submenu");

            cKey = getch();
            printf("\nYou chose %c\n", cKey);
            switch (cKey)
            {
               case '1':
               {
                  unsigned long mode = 0;
                  printf("\n Enter Mode (0: OFF, 1: SINE, 2: FRAME, 3: TX99, 4: TX100) =");
                  scanf("%ld",&mode);

                  unsigned long freq = 0;
                  printf("\n Enter Freq in MHz =");
                  scanf("%ld",&freq);

                  unsigned long dataRate = 0;
                  printf("\n Enter Data Rate =");
                  scanf("%ld",&dataRate);

                  double txPower = 0.0;
                  printf("\n Enter Tx Power in 0.5 dB steps =");
                  scanf("%lf",&txPower);

                  unsigned long scrambler = 0;
                  printf("\n Enter 1 for scrambler off, 0 for scramber on =");
                  scanf("%ld",&scrambler);

                  unsigned long tpcm = 0;
                  printf("\n Enter 0 txpower, 1 forced gain =");
                  scanf("%ld",&tpcm);

                  unsigned long txChain = 0;
                  printf("\n Enter txChain = ");
                  scanf("%ld",&txChain);

                  unsigned long numPackets = 0;
                  printf("\n Enter numPackets = ");
                  scanf("%ld",&numPackets);

                  unsigned long antenna = 1;
                  unsigned long enANI = 0;
                  unsigned long scramblerOff = scrambler;
                  unsigned long aifsn = 0;
                  unsigned long pktSz = 1500;
                  unsigned long pattern = 3;

                  bOk = QLIB_FTM_WLAN_Atheros_Tx( g_hResourceContext, mode, freq, dataRate,
                                                  txPower, antenna, enANI, scramblerOff, aifsn, pktSz, pattern, 0, numPackets, 1, tpcm, txChain);
                  printf("\n QLIB_FTM_WLAN_Atheros_V4_CONT_TRANSMIT() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '2':
               {
                  unsigned long action = 0;
                  printf("\n Enter Mode (0: PROMISCUOUS, 1: MAC FILTERED, 2: OFF) =");
                  scanf("%ld",&action);

                  unsigned long freq = 0;
                  printf("\n Enter Freq in MHz =");
                  scanf("%ld",&freq);

                  unsigned long rxChain = 0;
                  printf("\n Enter Rx Chain =");
                  scanf("%ld",&rxChain);

                  bOk = QLIB_FTM_WLAN_Atheros_Rx( g_hResourceContext, action, 0, freq, 1, rxChain);
                  printf("\n QLIB_FTM_WLAN_Atheros_Rx() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case '3':
               {
                  unsigned long totalPkts, crcErrPkts, secErrPkts;
                  double rssiInDbm;

                  bOk = QLIB_FTM_WLAN_Atheros_Rx_Report( g_hResourceContext, &totalPkts, &rssiInDbm, &crcErrPkts, &secErrPkts );
                  if( bOk != 0 )
                  {
                     printf("\n Total Packet Count = %d", totalPkts );
                     printf("\n Rssi = %lf dBm", rssiInDbm  );
                     printf("\n Crc Error Packet Count = %d", crcErrPkts );
                     printf("\n Sec Error Packet Count = %d", secErrPkts );
                  }
                  printf("\n QLIB_FTM_WLAN_Atheros_Rx_Report() = %s", bOk ? "PASS" : "FAIL" );
               }
               break;

               case 'q':
               case 'Q':
               {
                  AtherosTestLoop = false;;
               }
               break;

               default:
                  printf("\n Invalid test case choice!!!");
                  break;
            }
         }
      }
   }
   return bOk;
}

typedef struct 
{
    unsigned int ChipID;
    unsigned int SerialNumber;
} DigitalTestResultStruct;


typedef struct 
{
    unsigned short SNR;
    unsigned short Delta;
} ImageQualityTestResultStruct;


typedef struct 
{
    unsigned int Phase;
    unsigned int Delay;
} PsfCalibrationResultStruct;

unsigned char Test_FTM_Fprint( )
{
    bool exitFprint = false;
    unsigned char bOk = true;
    char cKey = '\0';

    unsigned short iStatus;
    unsigned short testID;
    unsigned char testStatus;
    unsigned char testResult;
    unsigned short lastTestId;
    unsigned short testResultDataLength;
    
    // Define a buffer for built-in test results
    const unsigned short maxBufferLength = 256;
    unsigned char buffer[maxBufferLength];

    do
    {
        printf("----------------\nFTM Security ID Sensor Test\n----------------\n");

        printf("\n1=DIGITAL TEST");
        printf("\n2=BG ESTIMATION");
        printf("\n3=RESET");
        printf("\n4=IMAGE QUALITY TEST");
        printf("\n5=PSF CALIBRATION");
        printf("\n6=READ SENSOR ID");
        printf("\n7=SET TEST PARAMETER");
        printf("\nQ=EXIT FROM THIS MENU");

        printf("\nPlease select an option: " );
        cKey = getch();
        printf("\n");

        switch (cKey)
        {
            case '1':
                {
                    testID = 2; // DIGITAL TEST
                    bOk = QLIB_FTM_SECURITY_ID_SENSOR_EXEC( g_hResourceContext, &iStatus, testID );
                    
                    do{
                        if ( bOk )
                        {
                            bOk = QLIB_FTM_SECURITY_ID_SENSOR_READ_EXEC_STATUS( g_hResourceContext, &iStatus, &lastTestId, &testStatus, &testResult, maxBufferLength, &testResultDataLength, buffer ); 
                        }

                    } while (( testStatus != 0 ) && ( bOk ));

                    if ( bOk )
                    {
                        DigitalTestResultStruct* testResultData = (DigitalTestResultStruct*)buffer;

                        printf("\nDIGITAL TEST : " );
                        if ( testResult )
                        {
                            printf("PASS - ChipID = %08X Serial Number = %d", testResultData->ChipID, testResultData->SerialNumber );
                        }
                        else
                        {
                            printf("FAIL - ChipID = %08X Serial Number = %d", testResultData->ChipID, testResultData->SerialNumber );
                        }
                    } 
                    else
                    {
                        printf(" EXECUTE DIGITAL TEST FAILED" );
                    }


                }
                break;

            case '2':
                {
                    testID = 18; // BG ESTIMATION
                    bOk = QLIB_FTM_SECURITY_ID_SENSOR_EXEC( g_hResourceContext, &iStatus, testID );
                    
                    do{
                        if ( bOk )
                        {
                            bOk = QLIB_FTM_SECURITY_ID_SENSOR_READ_EXEC_STATUS( g_hResourceContext, &iStatus, &lastTestId, &testStatus, &testResult, maxBufferLength, &testResultDataLength, buffer ); 
                        }

                    } while (( testStatus != 0 ) && ( bOk ));

                    if ( bOk )
                    {
                        printf("\nBG ESTIMATION : " );
                        if ( testResult )
                        {
                            printf("PASS" );
                        }
                        else
                        {
                            printf("FAIL" );
                        }
                    } 
                    else
                    {
                        printf(" EXECUTE BG ESTIMATION FAILED" );
                    }
                }
                break;

            case '3':
                {
                    testID = 16; // RESET
                    bOk = QLIB_FTM_SECURITY_ID_SENSOR_EXEC( g_hResourceContext, &iStatus, testID );
                    
                    do{
                        if ( bOk )
                        {
                            bOk = QLIB_FTM_SECURITY_ID_SENSOR_READ_EXEC_STATUS( g_hResourceContext, &iStatus, &lastTestId, &testStatus, &testResult, maxBufferLength, &testResultDataLength, buffer ); 
                        }

                    } while (( testStatus != 0 ) && ( bOk ));

                    if ( bOk )
                    {
                        printf("\nRESET : " );
                        if ( testResult )
                        {
                            printf("PASS" );
                        }
                        else
                        {
                            printf("FAIL" );
                        }
                    } 
                    else
                    {
                        printf(" EXECUTE RESET FAILED" );
                    }
                }
                break;

            case '4':
                {
                    testID = 9; // IMAGE QUALITY TEST
                    bOk = QLIB_FTM_SECURITY_ID_SENSOR_EXEC( g_hResourceContext, &iStatus, testID );
                    
                    do{
                        if ( bOk )
                        {
                            bOk = QLIB_FTM_SECURITY_ID_SENSOR_READ_EXEC_STATUS( g_hResourceContext, &iStatus, &lastTestId, &testStatus, &testResult, maxBufferLength, &testResultDataLength, buffer ); 
                        }

                    } while (( testStatus != 0 ) && ( bOk ));

                    if ( bOk )
                    {
                        ImageQualityTestResultStruct* testResultData = (ImageQualityTestResultStruct*)buffer;

                        printf("\nIMAGE QUALITY TEST : " );
                        if ( testResult )
                        {
                            printf("PASS - SNR = %f Delta = %d", (float)(testResultData->SNR)/1000, (float)(testResultData->Delta)/1000 );
                        }
                        else
                        {
                            printf("FAIL - SNR = %f Delta = %d", (float)(testResultData->SNR)/1000, (float)(testResultData->Delta)/1000 );
                        }
                    } 
                    else
                    {
                        printf(" EXECUTE IMAGE QUALITY FAILED" );
                    }
                }
                break;
                    
            case '5':
                {
                    testID = 21; // PSF CALIBRATION
                    bOk = QLIB_FTM_SECURITY_ID_SENSOR_EXEC( g_hResourceContext, &iStatus, testID );
                    
                    do{
                        if ( bOk )
                        {
                            bOk = QLIB_FTM_SECURITY_ID_SENSOR_READ_EXEC_STATUS( g_hResourceContext, &iStatus, &lastTestId, &testStatus, &testResult, maxBufferLength, &testResultDataLength, buffer ); 
                        }

                    } while (( testStatus != 0 ) && ( bOk ));

                    if ( bOk )
                    {
                        PsfCalibrationResultStruct* testResultData = (PsfCalibrationResultStruct*)buffer;

                        printf("\nPSF CALIBRATION : " );
                        if ( testResult )
                        {
                            printf("PASS - Phase = %f rad Delay = %f us", ((double)(testResultData->Phase)) * 2 * M_PI / 256, ((double)(testResultData->Delay)) / 16384 );
                        }
                        else
                        {
                            printf("FAIL - Phase = %f rad Delay = %f us", ((double)(testResultData->Phase)) * 2 * M_PI / 256, ((double)(testResultData->Delay)) / 16384);
                        }
                    } 
                    else
                    {
                        printf(" EXECUTE PSF CALIBRATION FAILED" );
                    }
                }
                break;

            // READ SENSOR ID
            case '6':
                {
                    unsigned short entryID = 100; // 100 = SENSOR ID
                    const unsigned short IDMaxLength = 25;
                    unsigned short dataLength;
                    byte buffer[IDMaxLength+1];
                    memset( buffer, 0, sizeof( buffer ));

                    bOk = QLIB_FTM_SECURITY_ID_SENSOR_READ_MFG_DATA( g_hResourceContext, &iStatus, entryID, IDMaxLength, &dataLength, buffer );

                    if ( bOk )
                    {
                        printf("\nREAD SENSOR ID : PASS - SENSOR ID = %s", buffer );
                    } 
                    else
                    {
                        printf(" READ SENSOR ID : FAILED" );
                    }
                }
                break;

            // SET TEST PARAMETER
            case '7':
                {
                    short testParameterID;
                    short testParameterValue;
                    stringstream conversionStream;
                    string input;
                    bool validInput = false;
                    
                    while ( kbhit() )
                    {
                        getch();
                    }

                    do 
                    {
                        printf("\n17 - TEST_IQ_LL (x1000)" );
                        printf("\n18 - TEST_IQ_UL (x1000)" );
                        printf("\n55 - TEST_IQ_SNR_TEMPLATE" );
                        printf("\nEnter Test Parameter ID (-1 = Cancel) :" );

                        getline( cin, input );

                        validInput = isdigit( input.c_str()[0] ) != 0;

                        if ( validInput )
                        {
                             testParameterID = atoi( input.c_str() );
                        }

                    } while ( !validInput );

                    validInput = false;

                    if ( testParameterID >= 0 )
                    {
                        do 
                        {
                            printf("\nEnter New Value (-1 = Cancel) :" );
                            getline( cin, input );

                            validInput = isdigit( input.c_str()[0] ) != 0;

                            if ( validInput )
                            {
                                 testParameterValue = atoi( input.c_str() );
                            }

                        } while ( !validInput );


                        if ( testParameterValue >= 0 )
                        {
                            printf( "\nSetting Test Parameter %d to new value %d", testParameterID, testParameterValue );

                            bOk = QLIB_FTM_SECURITY_ID_SENSOR_SET_TEST_PARAMETER( g_hResourceContext, &iStatus, (unsigned short)testParameterID, sizeof(short), (unsigned char*)&testParameterValue );

                            if ( bOk )
                            {
                                printf("\nSUCCESS" );
                            } 
                            else
                            {
                                printf("\nERROR" );
                            }
                        }
                    }
                }
                break;


            case 'Q' : 
                {
                    exitFprint = true;
                }
                break;
        }

        printf("\n\n");

    } while ( exitFprint == false );

    return bOk;
}


unsigned char StartAsyncCapture( unsigned long iSSID, long iRT_MASK )
{
   unsigned char bOk = true;

   printf("\n\n==============================\nSetting Async Message Capture:\n==============================");
   printf("\n   SSID = 0x%X", (short) iSSID);
   printf("\nRT_MASK = 0x%X", iRT_MASK);

   bOk &= QLIB_DIAG_SetMessageFilter( g_hResourceContext, (short) iSSID, iRT_MASK );
   printf("\nQLIB_DIAG_SetMessageFilter() = %s", bOk ? "PASS" : "FAIL" );

   // Enable all phone logging
   bOk &= QLIB_DIAG_SetPhoneLoggingState( g_hResourceContext, true );
   printf("\nQLIB_DIAG_SetPhoneLoggingState = %s", bOk ? "PASS" : "FAIL" );

   printf("\n");
   return bOk;
}

void DumpAsync( void )
{
   ////////////////////////////
   // start dump asyc messages

   unsigned char bSendToConsole = true;
   unsigned char bSendToQlibLogFile = true;
   unsigned char  bShowBinaryData = false;
   char _sUserMessage1[] = "\n==============================\nAsync Dump:";
   char _sUserMessage2[] = "\nAsync Dump Complete\n==============================";

   // Add user message 1
   QLIB_AddUserMessageToTextLog ( g_hResourceContext, LOG_IO, _sUserMessage1, bSendToConsole );

   // Dump available messages to a text log
   QLIB_DumpAsyncMessageToTextLog( g_hResourceContext, LOG_IO, bSendToConsole, bSendToQlibLogFile, bShowBinaryData );

   // Add user message 2
   QLIB_AddUserMessageToTextLog ( g_hResourceContext, LOG_IO, _sUserMessage2, bSendToConsole );

   // End dump async messages
   ///////////////////////////////
}

void InterpretAsyncInLog( void )
{
   char _sFileName[200] = "C:\\temp\\DumpAsyncMsg.txt";

   const int c_iLineMaxLen = 50000;         // Maximum # of text characters in line
   char _sNextLine[ c_iLineMaxLen ];            // Line contents
   int _iTextIndex = 0;                     // current text index
   char _cNextChar = 0;                     // next character

   int _iLineLen = 0;                   // Current line length

   int c_iMinLineLen = 10;              // # of characters that skip the date/time stamp

   int c_iBinArrayMaxLen = 4000;            // Maximum # of bytes represented by text
   unsigned char _aBinValues[ c_iLineMaxLen ];  // binary contents
   int _iBinValueIndex = 0;
   unsigned char _iNextBinValue = 0;

   printf("\n\n File name nust be: %s", _sFileName);
   printf("\n   <<<<Press any key to continue>>>>");
   getch();

   // Open the file
   FILE* _TextInFile = fopen(_sFileName,"r");

   // Test file
   if ( _TextInFile )
   {
      // Go until EOF
      while ( !feof( _TextInFile ) )
      {
         // Get the next line
         if ( fgets(_sNextLine, c_iLineMaxLen, _TextInFile ) )
         {
            // Determine length of line
            _iLineLen = strlen( _sNextLine );

            if ( _iLineLen > c_iMinLineLen )
            {
               // Start at the minimum text index....
               _iTextIndex = 0;

               // Binary index = 0
               _iBinValueIndex = 0;

               // Look for a tab--increment until a "\t" is found
               while ( (_sNextLine[ _iTextIndex++ ] != '\t') && _iTextIndex < _iLineLen );

               // Print the line as-is
               QLIB_AddUserMessageToTextLog( g_hResourceContext, LOG_IO, _sNextLine, true );

               // Test for an async message
               if ( _sNextLine[ _iTextIndex ] == 'A')
               {
                  // increment until a ":" is found
                  while ( _sNextLine[ _iTextIndex++ ] != ':');

                  // Skip one
                  _iTextIndex++;

                  // Loop through the rest of the string
                  while ( _iTextIndex < _iLineLen )
                  {
                     //
                     // Convert 1st hex digit to hex
                     //
                     _cNextChar = toupper(_sNextLine[ _iTextIndex++ ]);

                     // Check for A-F
                     if (   ( _cNextChar >= 'A')
                            &&  ( _cNextChar <= 'G')
                        )
                     {
                        _iNextBinValue = (_cNextChar - 'A' + 10) *16;
                     }
                     else
                     {
                        _iNextBinValue = (_cNextChar - '0' ) *16;
                     }

                     //
                     // Convert 2nd hex digit to hex
                     //
                     _cNextChar = toupper(_sNextLine[ _iTextIndex++ ]);

                     // Check for A-F
                     if (   ( _cNextChar >= 'A')
                            &&  ( _cNextChar <= 'G')
                        )
                     {
                        _iNextBinValue += (_cNextChar - 'A' + 10);
                     }
                     else
                     {
                        _iNextBinValue += (_cNextChar - '0' );
                     }

                     // Set the next binary value
                     _aBinValues[ _iBinValueIndex++ ] = _iNextBinValue;

                     // Skip the space between digits
                     _iTextIndex++;
                  } // while ( _iTextIndex < _iLineLen )

                  // Process the current binary message
                  QLIB_AddUserAsyncLogMessageToTextLog( g_hResourceContext, LOG_IO, _aBinValues, true, true );
               }    // if ( _sNextLine[ _iTextIndex ] == 'A')
            }   // if ( _iLineLen > c_iMinLineLen )
         }  // if ( fgets(_sNextLine, c_iLineMaxLen, _TextInFile ) )
      } // while ( !feof( _TextInFile ) )

      fclose( _TextInFile );
   }
   else
   {
      printf("\n Could not open file.");
   } // if ( _TextInFile )
}

void asyncMessageCallBack
(
   unsigned short iMessageSize,
   unsigned char* iMessageBuffer,
   HANDLE hContextID
)
{
   // If verbose is enabled, then print message
   if ( g_bAsyncCallbackVerbose )
   {
      printf("\nAsync message received by C callback.  Context = %d, Size = %d", hContextID, iMessageSize );

      LogMessage* _pLogMessage = (LogMessage*) iMessageBuffer;

      // First check to see if it is a log command response
      if ( _pLogMessage->CMD_CODE == _DIAG_LOG_F )
      {
         // Now look for the specical QLIB log type
         if ( _pLogMessage->log_item == QLIB_TEXT_LOG_CODE )
         {
            // Decode the text log structure
            QMSL_TextLog_struct* _pTextLog = (QMSL_TextLog_struct*) _pLogMessage->iLogMsgData;

            // Print the log type
            printf("\n  QMSL Text Log found...");
            printf("\n      Log Type: %d", _pTextLog->iLogLevel );
            printf("\n      Log Text: %s", _pTextLog->sLogText  );
         }  //  if ( _pLogMessage->log_item == QLIB_TEXT_LOG_CODE )
      } // if ( _pLogMessage->CMD_CODE == _DIAG_LOG_F )

      // Decode normal text messages
   }
}

void textMessageCallBack
(
   unsigned short iMessageSize,
   unsigned char* piMessageBuffer,
   HANDLE hContextID
)
{
   // If verbose is enabled, then print message
   if ( g_bTextCallbackVerbose )
   {
      QMSL_TextLog_struct* _pTextMessage = (QMSL_TextLog_struct*) piMessageBuffer;

      // Print the log type
      printf("\n  QMSL Text Log found...");
      printf("\n      Log Type: %d", _pTextMessage->iLogLevel );
      printf("\n      Log Text: %s", _pTextMessage->sLogText  );
   }
}

void swdlCallBack(
   HANDLE hContextID,
   char * sMsg,
   unsigned short iMsgSize)
{
   printf("\n%s", sMsg);
}


unsigned char Test_AT_Commands( void )
{
   unsigned char bOk = true;
   unsigned char bContinue = true;
   unsigned char cKey;

   printf("\n----------------\nAT Commands API\n----------------\n");

   do
   {
      printf("\n\n1=Set packet mode: Diag to Diag");
      printf("\n2=Set packet mode: Diag to modem");
      printf("\n3=Set packet mode: Modem to modem");
      printf("\n4=Send test AT command");
      printf("\n5=Send test Diag command (check phone version)");
      printf("\n6=Send custom AT command");
      printf("\nQ=Quit AT command API\n");
      cKey = getch();

      if (cKey == '1' )
      {
         bOk = QLIB_SetPacketMode( g_hResourceContext, 0 );
         printf("\nQLIB_SetPacketMode(0) %s", bOk ? "PASS" : "FAIL\n" );
      }

      if (cKey == '2' )
      {
         bOk = QLIB_SetPacketMode(g_hResourceContext, 1);
         printf("\nQLIB_SetPacketMode(1) %s", bOk ? "PASS" : "FAIL\n" );
      }

      if (cKey == '3' )
      {
         bOk = QLIB_SetPacketMode( g_hResourceContext, 2 );
         printf("\nQLIB_SetPacketMode(2) %s", bOk ? "PASS" : "FAIL\n" );
      }

      if (cKey == '4' )
      {
         short iRequestSize  = 8;   // Size of request packet
         short iResponseSize = 0;   // This will get updated by SendSync
         unsigned char aiRequestBuffer  [ DIAG_MAX_PACKET_SIZE ];
         static unsigned char aiResponseBuffer [ DIAG_MAX_PACKET_SIZE ];

         // Prepare a packet for Send Sync
         //             wsprintf(aiRequestBuffer,"AT+CBC\r\0");
         aiRequestBuffer[0] = 'A';
         aiRequestBuffer[1] = 'T';
         aiRequestBuffer[2] = '+';
         aiRequestBuffer[3] = 'C';
         aiRequestBuffer[4] = 'B';
         aiRequestBuffer[5] = 'C';
         aiRequestBuffer[6] = '\r';
         aiRequestBuffer[7] = '\0';
         iRequestSize  = 8;
         iResponseSize = DIAG_MAX_PACKET_SIZE;

         if ( QLIB_SendRAW( g_hResourceContext, iRequestSize, aiRequestBuffer, &iResponseSize, aiResponseBuffer,2000 ) )
         {
            printf("\nSend RAW (AT+CBC): PASS\n");
            printf("\n%s\n", aiResponseBuffer);
         }
         else
         {
            printf("\nSend RAW (AT+CBC): FAIL\n");
         }
      }

      if (cKey == '5' )
      {
         // On the string fields, include an extra character for a NULL termination
         char comp_date[ 12 ];
         char comp_time[ 9 ];
         char rel_date [ 12 ];
         char rel_time [ 9 ];
         char ver_dir  [ 9 ];
         unsigned char scm;
         unsigned char mob_cai_rev;
         unsigned char mob_model;
         unsigned short mob_firm_rev;
         unsigned char slot_cycle_index;
         unsigned char voc_maj;
         unsigned char voc_min;

         // Force the phone into ONLINE mode
         // Go into FTM mode
         if ( !QLIB_DIAG_VERNO_F( g_hResourceContext,
                                  comp_date,  comp_time,  rel_date,   rel_time, ver_dir,
                                  &scm, &mob_cai_rev, &mob_model, &mob_firm_rev,&slot_cycle_index,
                                  &voc_maj, &voc_min
                                )
            )
         {
            printf("Version Number: FAIL\n" );
            bOk = false;
         }
         else
         {
            // Set null terminations for each string
            comp_date[11] = '\0';
            comp_time[ 8 ] = '\0';
            rel_date [ 11 ] = '\0';
            rel_time [ 8 ] = '\0';
            ver_dir  [ 8 ] = '\0';

            // Print values
            printf("Mobile Version report: PASS\n"      );
            printf("Compile Date: %s\n", comp_date      );
            printf("Compile Time: %s\n", comp_time      );
            printf("Release Date: %s\n", rel_date       );
            printf("Release Time: %s\n", rel_time       );
            printf(" Version Dir: %s\n", ver_dir        );

            printf("\n\n");

            printf("   Station Class Mark: %d\n", scm               );
            printf("              CAI Rev: %d\n", mob_cai_rev       );
            printf("         Mobile Model: %d\n", mob_model         );
            printf("         Firmware Rev: %d\n", mob_firm_rev      );
            printf("     Slot Cycle Index: %d\n", slot_cycle_index  );
            printf("Vocoder major version: %d\n", voc_maj           );
            printf("Vocoder minor version: %d\n", voc_min           );

            bOk = true;
         }
      }

      if (cKey == '6' )
      {
         short iRequestSize  = 0;   // Size of request packet
         short iResponseSize = DIAG_MAX_PACKET_SIZE;    // This will get updated by SendSync
         unsigned char aiRequestBuffer  [ DIAG_MAX_PACKET_SIZE ];
         unsigned char aiResponseBuffer [ DIAG_MAX_PACKET_SIZE ];

         for( int i = 0; i < DIAG_MAX_PACKET_SIZE; ++i )
            aiResponseBuffer[ i ] = 0;

         // Prepare a packet for Send Sync
         printf("\nWrite AT command:\n");
         scanf("%s", &aiRequestBuffer);
         while( aiRequestBuffer[ iRequestSize++ ] != 0 );
         aiRequestBuffer[ iRequestSize - 1 ] = '\r';
         aiRequestBuffer[ iRequestSize++ ] = '\0';

         if ( QLIB_SendRAW( g_hResourceContext, iRequestSize, aiRequestBuffer, &iResponseSize, aiResponseBuffer,2000 ) )
         {
            printf("\nSend RAW: PASS\n");
            printf("\n%s\n", aiResponseBuffer);
         }
         else
         {
            printf("\nSend RAW: FAIL\n");
         }
      }

      if (cKey == 'Q' || cKey == 'q' )
         bContinue = false;
   }
   while (bContinue);

   return 0;
}

unsigned char Test_FTM_GPS( void )
{
   unsigned char bOk=true;
   unsigned char cKey;
   unsigned char iStatus;

   // For development, logging will be activated automatically, remove this for release
   // when they are removed, also remove the corresponding function calls to stop the log
   // at the end of the function
   unsigned short _iLogFlags = LOG_FN | LOG_RET |  LOG_ERR | LOG_C_HIGH_LEVEL_START  | LOG_C_HIGH_LEVEL_STOP | LOG_ASYNC;

   // Include the following line in order to see IO information.  There tends to be a lot of log messages with
   // GPS, so the IO information can be too much.
   _iLogFlags |= LOG_IO;

   QLIB_SetLogFlags( g_hResourceContext, _iLogFlags );
   QLIB_StartLogging( g_hResourceContext,  "C:\\Factory_log.txt");
   QLIB_StartDLF_Logging( g_hResourceContext,  "C:\\Factory_log.dlf", false);

   if ( bOk )
   {
      printf("\n\n----------------\nFTM GPS Test\n----------------\n");
      printf("\n1 = gpsOne Generation 5c (CDMA TimeMultiplex)");
      printf("\n2 = gpsOne Generation 5w (UMTS Standalone)");
      printf("\n3 = gpsOne Generation 6c (CDMA SGPS)");
      printf("\n4 = gpsOne Generation 6w (UMTS Standalone)");
      printf("\n5 = gpsOne Generation 7" );
      printf("\n" );

      cKey = toupper( getch() );

      // Reset the OK flag
      bOk = true;

      // gpsOne Generation 5c
      if(cKey == '1' || cKey == '3')
      {
         printf("\n\nTest FTM GPS on the secondary chain");
         printf("\nProcedure:");
         printf("\n-> Set up 1st and 2nd chain for RF initialization");
         printf("\n-> Set Secondary Chain to GPS");
         printf("\n-> Set LNA to high gain");
         printf("\n-> Set up test equipment");
         printf("\n-> Get FTM C/N");
         printf("\nPresss any key to continue\n");
         getch();

         // Set the packet mode id field to CDMA primary
         // No command is sent to the mobile
         short eShortId = FTM_MODE_ID_CDMA_1X;
         bOk = QLIB_FTM_SET_MODE_ID(g_hResourceContext, eShortId );
         printf("\nQLIB_FTM_SET_MODE_ID(%d) %s", eShortId,  bOk ? "PASS" : "FAIL\n");

         // Set primary chain to CDMA800 mode
         short eModeId = PHONE_MODE_CDMA_800;
         bOk = QLIB_FTM_SET_MODE( g_hResourceContext, eModeId );
         printf("\nQLIB_FTM_SET_MODE(%d) %s", eModeId,  bOk ? "PASS" : "FAIL\n");

         // 2 = Off frequency search
         short eSecondaryChainMode = 2;
         bOk = QLIB_FTM_SET_SECONDARY_CHAIN( g_hResourceContext, eSecondaryChainMode );
         printf("\nQLIB_FTM_SET_SECONDARY_CHAIN(%d) %s", eSecondaryChainMode,  bOk ? "PASS" : "FAIL\n");

         // Set the packet mode id field to CDMA RX1 (secondary chain)
         eShortId = FTM_MODE_ID_CDMA_1X_RX1;
         bOk = QLIB_FTM_SET_MODE_ID(g_hResourceContext, eShortId );
         printf("\nQLIB_FTM_SET_MODE_ID(%d) %s", eShortId,  bOk ? "PASS" : "FAIL\n");

         // Set secondary chain to CDMA800mode
         eModeId = PHONE_MODE_CDMA_800;
         bOk = QLIB_FTM_SET_MODE( g_hResourceContext, PHONE_MODE_CDMA_800 );
         printf("\nQLIB_FTM_SET_MODE(%d) %s", eModeId,  bOk ? "PASS" : "FAIL\n");

         // Set 2nd chain to channel 387
         unsigned short iChannel = 387;
         bOk = QLIB_FTM_SET_CHAN( g_hResourceContext, iChannel);
         printf("\nQLIB_FTM_SET_CHAN(%d) %s", iChannel,  bOk ? "PASS" : "FAIL\n");

         // Set mode to GPS
         eModeId = PHONE_MODE_GPS;
         bOk = QLIB_FTM_SET_MODE( g_hResourceContext, eModeId);
         printf("\nQLIB_FTM_SET_MODE(%d) %s", eModeId,  bOk ? "PASS" : "FAIL\n");

         // Set 2nd chain LNA to highest gain state
         unsigned char iLNARange = 0;
         bOk = QLIB_FTM_SET_LNA_RANGE( g_hResourceContext, iLNARange);
         printf("\nQLIB_FTM_SET_LNA_RANGE(%d) %s", iLNARange,  bOk ? "PASS" : "FAIL\n");

         /*
         Set base station power
         sGPIBcmd = "call:rfgenerator:frequency 1575550000";
         ibwrt(h_Inst, sGPIBcmd.GetBuffer(0), sGPIBcmd.GetLength());
         //  GPS offset CW

         sGPIBcmd = "call:power -80";
         ibwrt(h_Inst, sGPIBcmd.GetBuffer(0), sGPIBcmd.GetLength());
         // -80 dBm

         Sleep(100);
         // Let the power settle
         */
         printf("nn\nSet base station emulator at 1575.55MHz (GPS) at -80dBm\n", iChannel);
         printf("Press any key to continue\n");
         getch();

         long _dCtoN = 0;
         bOk = QLIB_FTM_GET_CTON( g_hResourceContext, &_dCtoN);
         printf("\nQLIB_FTM_GET_CTON() %s",  bOk ? "PASS" : "FAIL");

         // Get C/N note: The value returned is 10 * (C/N) dB
         printf("\nGPS C/N is %d\n", _dCtoN );
         printf("\n\nPress any key to continue\n");
         getch();
      }
      else if(cKey == '2' || cKey == '4')
      {
         printf("\nExample can be found in 'AGPS C/N' under 'AGPS FTM Test' from the main menu\n");
         printf("\n\nPress any key to continue\n");
         getch();
      }
      // gpsOne Generation 7
      else if(cKey == '5')
      {
         do
         {
            printf("\n\n----------------\nFTM GPS Test\n----------------\n");

            printf("\n0 = Set Low Linearity Mode");
            printf("\n1 = Set High Linearity Mode");
            printf("\n2 = Set High Linearity Mode for AWS band");

            printf("\n3 = Enable Periodic Steady-State DC correction");
            printf("\n4 = Disable Periodic Steady-State DC correction");

            printf("\n5 = Initialize RFIC for the IM2 calibration");

            printf("\n6 = Do single IM2 cal iteration (with Channel_ID = I)");
            printf("\n7 = Do single IM2 cal iteration (with Channel_ID = Q)");

            printf("\n8 = Get GPS BP_Mean and BP_Amplitude");

            printf("\n9 = Get GPS C/No");

            printf("\nQ=Exit this menu");

            cKey = toupper( getch() );

            // Reset the OK flag
            bOk = true;

            // Setup condtions to determine what items will be executed

            // Set Low Linearity Mode
            unsigned char _bSet_Low_Linearity_Mode = ( cKey=='0') ;

            // Set High Linearity Mode
            unsigned char _bSet_High_Linearity_Mode = ( cKey=='1') ;

            // Set High Linearity Mode for AWS band
            unsigned char _bSet_High_Linearity_Mode_for_AWS = ( cKey=='2') ;

            // Enable Periodic Steady-State DC correction
            unsigned char _bEnablePeriodicSStateDC_corr = ( (cKey=='3') ) ;

            // Disable Periodic Steady-State DC correction
            unsigned char _bDisablePeriodicSStateDC_corr = ( (cKey=='4') ) ;

            // Initialize RFIC for the IM2 calibration
            unsigned char _bInitialize_RFIC_for_IM2cal = ( (cKey=='5') ) ;

            // Do single IM2 cal iteration (with Channel_ID = I)
            unsigned char _bDoSingleIM2cal_iter_channel_I = ( (cKey=='6') ) ;

            // Do single IM2 cal iteration (with Channel_ID = Q)
            unsigned char _bDoSingleIM2cal_iter_channel_Q = ( (cKey=='7') ) ;

            // Get GPS BP_Mean and BP_Amplitude
            unsigned char _bGetGPS_BP_Mean_Ampl = ( (cKey=='8') ) ;

            //  Get GPS C/No
            unsigned char _bGetGPS_CN0 = ( (cKey=='9') ) ;

            if ( _bSet_Low_Linearity_Mode )
            {
               bOk = QLIB_FTM_GPS_SET_LINEARITY_MODE( g_hResourceContext, FTM_GPS_SET_LINEARITY_MODE_LL, &iStatus );
               printf("\nQLIB_FTM_GPS_SET_LINEARITY_MODE(): %s, status %d",
                      bOk ? "PASS" : "FAIL",
                      iStatus );
            }   // _bSet_Low_Linearity_Mode

            if ( _bSet_High_Linearity_Mode )
            {
               bOk = QLIB_FTM_GPS_SET_LINEARITY_MODE( g_hResourceContext, FTM_GPS_SET_LINEARITY_MODE_HL, &iStatus );
               printf("\nQLIB_FTM_GPS_SET_LINEARITY_MODE(): %s, status %d",
                      bOk ? "PASS" : "FAIL",
                      iStatus );
            }   // _bSet_High_Linearity_Mode

            if ( _bSet_High_Linearity_Mode_for_AWS )
            {
               bOk = QLIB_FTM_GPS_SET_LINEARITY_MODE( g_hResourceContext, FTM_GPS_SET_LINEARITY_MODE_HL_AWS, &iStatus );
               printf("\nQLIB_FTM_GPS_SET_LINEARITY_MODE(): %s, status %d",
                      bOk ? "PASS" : "FAIL",
                      iStatus );
            }   // _bSet_High_Linearity_Mode_for_AWS

            if ( _bEnablePeriodicSStateDC_corr )
            {
               bOk = QLIB_FTM_GPS_ENABLE_SS_DC_CORRECTION( g_hResourceContext, FTM_GPS_ENABLE_SS_DC_CORRECTION, &iStatus );
               printf("\nQLIB_FTM_GPS_ENABLE_SS_DC_CORRECTION(): %s, status %d",
                      bOk ? "PASS" : "FAIL",
                      iStatus );
            }   // _bEnablePeriodicSStateDC_corr

            if ( _bDisablePeriodicSStateDC_corr )
            {
               bOk = QLIB_FTM_GPS_ENABLE_SS_DC_CORRECTION( g_hResourceContext, FTM_GPS_DISABLE_SS_DC_CORRECTION, &iStatus );
               printf("\nQLIB_FTM_GPS_ENABLE_SS_DC_CORRECTION(): %s, status %d",
                      bOk ? "PASS" : "FAIL",
                      iStatus );
            }   // _bDisablePeriodicSStateDC_corr

            if ( _bInitialize_RFIC_for_IM2cal )
            {
               bOk = QLIB_FTM_GPS_INIT_FOR_IM2_CAL( g_hResourceContext, &iStatus );
               printf("\nQLIB_FTM_GPS_INIT_FOR_IM2_CAL(): %s, status %d",
                      bOk ? "PASS" : "FAIL",
                      iStatus );
            }   // _bInitialize_RFIC_for_IM2cal

            if ( _bDoSingleIM2cal_iter_channel_I )
            {
               unsigned short iCurrent_I_DC_DAC_optimum_value;
               unsigned short iCurrent_Q_DC_DAC_optimum_value;

               bOk = QLIB_FTM_GPS_DO_SINGLE_IM2_CAL_ITERATION( g_hResourceContext, FTM_GPS_DO_SINGLE_IM2_CAL_ITERATION_CHANNEL_I, FTM_GPS_DO_SINGLE_IM2_CAL_ITERATION_METHOD_WITH_DC_DACS, FTM_GPS_DO_SINGLE_IM2_CAL_ITERATION_FULL_SEARCH_SPACE, 12, 13, &iCurrent_I_DC_DAC_optimum_value, &iCurrent_Q_DC_DAC_optimum_value, &iStatus );
               printf("\nQLIB_FTM_GPS_DO_SINGLE_IM2_CAL_ITERATION(): %s, status %d",
                      bOk ? "PASS" : "FAIL",
                      iStatus );
            }   // _bDoSingleIM2cal_iter_channel_I

            if ( _bDoSingleIM2cal_iter_channel_Q )
            {
               unsigned short iCurrent_I_DC_DAC_optimum_value;
               unsigned short iCurrent_Q_DC_DAC_optimum_value;
               bOk = QLIB_FTM_GPS_DO_SINGLE_IM2_CAL_ITERATION( g_hResourceContext, FTM_GPS_DO_SINGLE_IM2_CAL_ITERATION_CHANNEL_Q, FTM_GPS_DO_SINGLE_IM2_CAL_ITERATION_METHOD_WITH_IM2_DACS, FTM_GPS_DO_SINGLE_IM2_CAL_ITERATION_VICINITY_SEARCH_SPACE, 14, 15, &iCurrent_I_DC_DAC_optimum_value, &iCurrent_Q_DC_DAC_optimum_value, &iStatus );
               printf("\nQLIB_FTM_GPS_DO_SINGLE_IM2_CAL_ITERATION(): %s, status %d",
                      bOk ? "PASS" : "FAIL",
                      iStatus );
            }   // _bDoSingleIM2cal_iter_channel_Q

            if ( _bGetGPS_BP_Mean_Ampl )
            {
               short iBP_Mean_I = 0;
               short iBP_Mean_Q = 0;
               short iBP_Amplitude_I = 0;
               short iBP_Amplitude_Q = 0;

               bOk = QLIB_FTM_GPS_GET_BP_MEAN_AND_AMPL( g_hResourceContext, &iBP_Mean_I, &iBP_Mean_Q, &iBP_Amplitude_I, &iBP_Amplitude_Q, &iStatus );
               printf("\nQLIB_FTM_GPS_GET_BP_MEAN_AND_AMPL(): %s, status %d",
                      bOk ? "PASS" : "FAIL",
                      iStatus );
            }   // _bGetGPS_BP_Mean_Ampl

            if ( _bGetGPS_CN0 )
            {
               short iCNo_tenths_dB = 0;

               bOk = QLIB_FTM_GPS_GET_CN0( g_hResourceContext, &iCNo_tenths_dB, &iStatus );
               printf("\nQLIB_FTM_GPS_GET_CN0(): %s, status %d",
                      bOk ? "PASS" : "FAIL",
                      iStatus );
            }   // _bGetGPS_CN0
         }
         while (cKey != 'Q' );
      } // if (cKey = 5)
   } // if (bOk )

   // Disable logging
   QLIB_StopLogging( g_hResourceContext );
   QLIB_StopDLF_Logging( g_hResourceContext );

   // Print status
   printf("\nCommand Status: %s", bOk ? "PASS" : "FAIL\n" );

   return bOk;
}

unsigned char Test_IS801_GPS( void )
{
   short cKey = -1;
   static unsigned char aiRequestBuffer  [ DIAG_MAX_PACKET_SIZE ];
   static unsigned char aiResponseBuffer [ DIAG_MAX_PACKET_SIZE ];
   unsigned short iRequestSize = 0;
   short iResponseSize = 0;
   const unsigned short c_iEventListSize = 3;
   unsigned short _aiEventList[c_iEventListSize] = {404, 405, 406};
   const unsigned short c_iLogCode1 = 0x1036;
   const unsigned short c_iLogCode2 = 0x1037;
   int _iLogIndex = 0;

   //////////////////////////////////////////////////////////
   // Reset the OK flag
   //////////////////////////////////////////////////////////
   unsigned char bOk = true;

   printf("\n\n================= IS801 GPS Sample Sequence ==================");
   printf("\nProcedure:");
   printf("\n-> Sequence from CL93-2244-1(2) document");
   printf("\n-> Set up GSS4100/6100 GPS simulator");
   printf("\n-> Enable IS801 log item 0x1036 and 0x1037");
   printf("\n-> Send IS801 message accordingly to QMSL_Debug file ");
   printf("\n-> Parse GPS result data");
   printf("\n\nPress any key to continue\n");

   cKey = toupper( getch() );

   if ( cKey )
   {
      //////////////////////////////////////////////////////////
      //Provide time to setup test equipment. The setup can be found
      //in CL93-2244-1(2) document or from QDART TPL sequence file
      //////////////////////////////////////////////////////////
      printf("\n-> Set up GSS4100/6100 GPS simulator now");
      printf("\n\nPress any key to continue\n");
      cKey = toupper( getch() );

      //////////////////////////////////////////////////////////
      //Enable IS801 log
      //Event is used as debug tool to see whether IS801 messages have been received
      //There is no data to be extracted from event. Therefore, the
      //sample code does not contain samples to enable events
      //Log is used to store GPS measurement and position data
      //////////////////////////////////////////////////////////
      printf("\n\n-> Enable IS801 log 0x1036 and 0x1037");
      printf("\n\nPress any key to continue\n");
      cKey = toupper( getch() );

      //////////////////////////////////////////////////////////
      //
      // Enable IS801 forward and reverse log
      //
      //////////////////////////////////////////////////////////

      //Enable 0x1036 = IS-801 Forward Link
      //Enable 0x1037 = IS-801 Reverse Link

      bOk = QLIB_DIAG_ClearAllLogCodes( g_hResourceContext, LOG_EQUIP_ID_1X );
      printf("\nQLIB_DIAG_ClearAllLogCodes = %s", bOk ? "PASS" : "FAIL" );

      // Setup the log code list
      const unsigned short c_iLogCodeListSize = 2;
      unsigned short _aiLogCodeList[ c_iLogCodeListSize ] =  { c_iLogCode1, c_iLogCode2 };

      // Add codes to the phone's log code mask
      bOk = QLIB_DIAG_AddExtendedLogCodes( g_hResourceContext, LOG_EQUIP_ID_1X, _aiLogCodeList, c_iLogCodeListSize );
      printf("\nQLIB_DIAG_AddExtendedLogCodes = %s", bOk ? "PASS" : "FAIL" );

      //////////////////////////////////////////////////////////
      //
      //Send the IS801 files to mobile based on QDART sequence file
      //The detailed QDART low level IS801 information can be found in
      //..\Program Files\Qualcomm\QDART\Logs\<GPS Test Tree>\QMSL_Debug\...
      //Users can match the time stamp to differentiate the GPIB commands
      //and IS801 commands
      //Sample here is only a demonstration on how to use QMSL to gather
      //IS801 measurement information. It is not a direct representation of
      //the test sequence.
      //
      //////////////////////////////////////////////////////////
      printf("\n\n-> Send IS801 message accordingly to QMSL_Debug file");
      printf("\n\nPress any key to continue\n");
      cKey = toupper( getch() );

      //////////////////////////////////////////////////////////
      //
      // SendSync information from QMSL_Debug file
      //
      // 56:18:472  ,LOG_C_HIGH_LEVEL_START,QLIB_SendSync
      // 56:18:472  ,LOG_IO,SEND: 6C 07 01 00 24 00
      // 56:18:472  ,LOG_IO,REC: 6C 07
      // 56:18:472  ,LOG_IO,STACK TIME (ms):    0
      // 56:19:723  ,LOG_C_HIGH_LEVEL_START,QLIB_SendSync
      // 56:19:723  ,LOG_IO,SEND: 6C 07 01 00 04 00
      // 56:19:723  ,LOG_IO,REC: 6C 07
      // 56:19:723  ,LOG_IO,STACK TIME (ms):    0
      // 56:24:973  ,LOG_C_HIGH_LEVEL_START,QLIB_SendSync
      // 56:24:973  ,LOG_IO,SEND: 6C 07 01 00 13 00
      // 56:24:973  ,LOG_IO,REC: 6C 07
      // 56:24:973  ,LOG_IO,STACK TIME (ms):    0
      // 56:27:004  ,LOG_C_HIGH_LEVEL_START,QLIB_SendSync
      // 56:27:004  ,LOG_IO,SEND: 6C 01 11 00 80 00 01 14 0B 00 0E 00 40 00 00 10 00 00 D0 00 00
      // 56:27:004  ,LOG_IO,REC: 6C 01
      //
      //////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////
      // Prepare a packet for Send Sync
      // 56:18:472  ,LOG_IO,SEND: 6C 07 01 00 24 00
      //////////////////////////////////////////////////////////
      aiRequestBuffer[0] = 0x6c;
      aiRequestBuffer[1] = 0x07;
      aiRequestBuffer[2] = 0x01;
      aiRequestBuffer[3] = 0x00;
      aiRequestBuffer[4] = 0x24;
      aiRequestBuffer[5] = 0x00;

      iRequestSize  = 6;
      iResponseSize = DIAG_MAX_PACKET_SIZE;

      //////////////////////////////////////////////////////////
      // Send the command
      //////////////////////////////////////////////////////////
      bOk = QLIB_SendSync( g_hResourceContext, iRequestSize, aiRequestBuffer, &iResponseSize, aiResponseBuffer,2000 );

      if( bOk )
         printf("\nSend Sync 6C 07 01 00 24 00: PASS");
      else
         printf("\nSend Sync 6C 07 01 00 24 00: FAIL");

      //////////////////////////////////////////////////////////
      // Prepare a packet for Send Sync
      // 56:18:472  ,LOG_IO,SEND: 6C 07 01 00 04 00
      //////////////////////////////////////////////////////////
      if( bOk )
      {
         aiRequestBuffer[0] = 0x6c;
         aiRequestBuffer[1] = 0x07;
         aiRequestBuffer[2] = 0x01;
         aiRequestBuffer[3] = 0x00;
         aiRequestBuffer[4] = 0x04;
         aiRequestBuffer[5] = 0x00;

         iRequestSize  = 6;

         // Send the command
         bOk = QLIB_SendSync( g_hResourceContext, iRequestSize, aiRequestBuffer, &iResponseSize, aiResponseBuffer,2000 );

         if( bOk )
            printf("\nSend Sync 6C 07 01 00 04 00: PASS");
         else
            printf("\nSend Sync 6C 07 01 00 04 00: FAIL");
      }

      //////////////////////////////////////////////////////////
      // Prepare a packet for Send Sync
      // 56:18:472  ,LOG_IO,SEND: 6C 07 01 00 13 00
      //////////////////////////////////////////////////////////
      if( bOk )
      {
         aiRequestBuffer[0] = 0x6c;
         aiRequestBuffer[1] = 0x07;
         aiRequestBuffer[2] = 0x01;
         aiRequestBuffer[3] = 0x00;
         aiRequestBuffer[4] = 0x13;
         aiRequestBuffer[5] = 0x00;

         iRequestSize  = 6;

         // Send the command
         bOk = QLIB_SendSync( g_hResourceContext, iRequestSize, aiRequestBuffer, &iResponseSize, aiResponseBuffer,2000 );

         if( bOk )
            printf("\nSend Sync 6C 07 01 00 13 00: PASS");
         else
            printf("\nSend Sync 6C 07 01 00 13 00: FAIL");
      }

      //////////////////////////////////////////////////////////
      // Prepare a packet for Send Sync
      // 56:27:004  ,LOG_IO,SEND: 6C 01 11 00 80 00 01 14 0B 00 0E 00 40 00 00 10 00 00 D0 00 00
      //////////////////////////////////////////////////////////
      if( bOk )
      {
         aiRequestBuffer[0] = 0x6c;
         aiRequestBuffer[1] = 0x01;
         aiRequestBuffer[2] = 0x11;
         aiRequestBuffer[3] = 0x00;
         aiRequestBuffer[4] = 0x80;
         aiRequestBuffer[5] = 0x00;
         aiRequestBuffer[6] = 0x01;
         aiRequestBuffer[7] = 0x14;
         aiRequestBuffer[8] = 0x0B;
         aiRequestBuffer[9] = 0x00;
         aiRequestBuffer[10] = 0x0E;
         aiRequestBuffer[11] = 0x00;
         aiRequestBuffer[12] = 0x40;
         aiRequestBuffer[13] = 0x00;
         aiRequestBuffer[14] = 0x00;
         aiRequestBuffer[15] = 0x10;
         aiRequestBuffer[16] = 0x00;
         aiRequestBuffer[17] = 0x00;
         aiRequestBuffer[18] = 0xD0;
         aiRequestBuffer[19] = 0x00;
         aiRequestBuffer[20] = 0x00;

         iRequestSize  = 21;

         // Send the command
         bOk = QLIB_SendSync( g_hResourceContext, iRequestSize, aiRequestBuffer, &iResponseSize, aiResponseBuffer,2000 );

         if( bOk )
            printf("\nSend Sync 6C 01 11 00 80 00 01 14 0B 00 0E 00 40 00 00 10 00 00 D0 00 00: PASS\n");
         else
            printf("\nSend Sync 6C 01 11 00 80 00 01 14 0B 00 0E 00 40 00 00 10 00 00 D0 00 00: FAIL\n");
      }

      //////////////////////////////////////////////////////////
      // Get GPS IS801 Logs
      // Declare memory for log buffers
      //////////////////////////////////////////////////////////
      const int c_iNumLogs = 1;
      const int c_iNumLogsStore = 20;
      void* _aLogs[ c_iNumLogs ];
      LogMessage* _aIS801ForwardLogs[ c_iNumLogsStore ]; //Store max 20 records for parsing
      LogMessage* _aIS801ReverseLogs[ c_iNumLogsStore ]; //Store max 20 records for parsing
      word _aSizes[ c_iNumLogs ];

      //////////////////////////////////////////////////////////
      // Declare memory for each log
      //////////////////////////////////////////////////////////
      for ( _iLogIndex = 0; _iLogIndex < c_iNumLogs; _iLogIndex++ )
      {
         // Declare memory
         _aLogs[_iLogIndex] = malloc( DIAG_MAX_PACKET_SIZE );

         _aSizes[_iLogIndex] = 0;
      }

      //////////////////////////////////////////////////////////
      // Timeout 1ms. 1ms timeout value here is used to prevent
      // dropping other logs due to long timeout. In QSPR recommendation
      // The timeout is usually set to 20sec, therefore, a counter
      // could be implemented here to simulate a 20sec timeout
      //////////////////////////////////////////////////////////
      const unsigned long c_iTimeOut_ms = 1;
      unsigned short _iNumLogs = c_iNumLogs;
      unsigned short _iIS801ForwardLogIndex = 0;
      unsigned short _iIS801ReverseLogIndex = 0;

      //////////////////////////////////////////////////////////
      // Until a key is pressed, print status of logs
      //////////////////////////////////////////////////////////
      printf("\nWaiting for logs ...\n");
      do
      {
         //////////////////////////////////////////////////////////
         //
         // Get 0x1036 logs
         //
         //////////////////////////////////////////////////////////

         // Set the maximum number of logs, must be done each time because this value is overwritten
         _iNumLogs = c_iNumLogs;

         QLIB_DIAG_GetMultipleLogs( g_hResourceContext, LOG_EQUIP_ID_1X, c_iLogCode1, &_iNumLogs, c_iTimeOut_ms, _aLogs, _aSizes );

         if( _iNumLogs > 0 )
         {
            printf("\nQLIB_DIAG_GetMultipleLogs(code = 0x%X, count = %d, timeout = %d) = %s",
                   c_iLogCode1,
                   c_iNumLogs,
                   c_iTimeOut_ms,
                   bOk ? "PASS" : "FAIL" );

            printf("\nQLIB_DIAG_GetMultipleLogs() - # of logs found: %d", _iNumLogs);

            for ( _iLogIndex = 0; _iLogIndex < _iNumLogs; _iLogIndex++, _iIS801ForwardLogIndex++)
            {
               // Structure to sort out the log message
               _aIS801ForwardLogs[ _iIS801ForwardLogIndex ] = (LogMessage*)_aLogs[ _iLogIndex ] ;

               printf("\n   Log #%d, id=0x%X, size = %d", _iLogIndex, _aIS801ForwardLogs[ _iIS801ForwardLogIndex ]->log_item, _aSizes[ _iLogIndex ] );
            }   // for ( _iLogIndex = 0; _iLogIndex < _iNumLogs; _iLogIndex++)
         }

         //////////////////////////////////////////////////////////
         //
         // Get 0x1037 logs
         //
         //////////////////////////////////////////////////////////
         _iNumLogs = c_iNumLogs;

         // Get multiple logs
         QLIB_DIAG_GetMultipleLogs( g_hResourceContext, LOG_EQUIP_ID_1X, c_iLogCode2, &_iNumLogs, c_iTimeOut_ms, _aLogs, _aSizes );

         if( _iNumLogs > 0 )
         {
            printf("\nQLIB_DIAG_GetMultipleLogs(code = 0x%X, count = %d, timeout = %d) = %s",
                   c_iLogCode2,
                   c_iNumLogs,
                   c_iTimeOut_ms,
                   bOk ? "PASS" : "FAIL" );

            printf("\nQLIB_DIAG_GetMultipleLogs() - # of logs found: %d", _iNumLogs);

            for ( _iLogIndex = 0; _iLogIndex < _iNumLogs; _iLogIndex++)
            {
               // Structure to sort out the log message
               _aIS801ReverseLogs[ _iIS801ReverseLogIndex ] = (LogMessage*)_aLogs[ _iLogIndex ] ;

               printf("\n   Log #%d, id=0x%X, size = %d\n", _iLogIndex, (_aIS801ReverseLogs[ _iIS801ReverseLogIndex ])->log_item, _aSizes[ _iLogIndex ] );

               _iIS801ReverseLogIndex++;
            }   // for ( _iLogIndex = 0; _iLogIndex < _iNumLogs; _iLogIndex++)
         }
      }
      while ( !kbhit() );

      //////////////////////////////////////////////////////////
      // Clear PC keypress queue
      //////////////////////////////////////////////////////////
      if (kbhit())
         getch();

      //////////////////////////////////////////////////////////
      //
      //Once the GPS IS801 log is received, users can parse the data based on
      //log information given in AMSS ICD document 80-V1294-1, Log item 0x1096 and 0x1097
      //The sample here only parse the reverse link log message to simplify the
      //demonstration.
      //
      //////////////////////////////////////////////////////////
      printf("\n\n-> Parse GPS result data from IS801 loggs, press any key to continue");
      cKey = toupper( getch() );

      //////////////////////////////////////////////////////////
      //
      //Parse Reverse Link IS801 Log
      //Information can be found in TIA/EIA/IS-801 section 3.2.4 Reverse Link Message Format
      //
      //////////////////////////////////////////////////////////
      for( _iLogIndex = 0; _iLogIndex < _iIS801ReverseLogIndex; _iLogIndex++ )
      {
         //Parse packet one at a time
         printf("\nPacket %d:",_iLogIndex);
         printf("\n\nRaw bytes information:");

         //first 12 bytes are time stamp information
         for( unsigned short _iDataIndex = 0; _iDataIndex < (_aIS801ReverseLogs[ _iLogIndex ]->length-12); _iDataIndex++ )
         {
            printf("%X ", *(_aIS801ReverseLogs[ _iLogIndex ]->iLogMsgData+_iDataIndex) );
         }
      } // for ( _iLogIndex

      //////////////////////////////////////////////////////////
      // Erase the memory for the logs
      //////////////////////////////////////////////////////////
      for ( _iLogIndex = 0; _iLogIndex < c_iNumLogs; _iLogIndex++ )
      {
         // Declare memory
         delete _aLogs[_iLogIndex];
      } // for ( _iLogIndex
   }
   else
   {
      return bOk;
   }

   return bOk;
}

unsigned char Test_Blow_QFPROM_in_Download_Mode()
{
   unsigned char bOk = true;
   unsigned char cKey;

   unsigned long _iAddress = 0;
   unsigned long _iAddressType = 0;
   unsigned long _iMSB = 0;
   unsigned long _iLSB = 0;
   unsigned long _iErrorCode = 0;
   unsigned char _iStatus = 0;

   char _cConfirm;

   // Step 1. Download emergency downloader into RAM
   //         For test automation,  use QLIB_UploadSB2MultiImage in QPST mode to download emergency downloader.

   // Step 2. call QMSL APIs to blow QPFROM in Donload mode.  QMSL must run in QPHONEMS mode

   while(1)
   {
      printf("\n\n  Blowing QFPROM in DL mode");
      printf("\n===============================");
      printf("\n 2 = Write QFPROM in Download Mode");
      printf("\n 3 = Read  QFPROM in Download Mode");
      printf("\n 4 = Quit");
      printf("\n");

      cKey = toupper(getch());
      if(cKey == '2')
      {
         printf("Write QFPROM\n");
         printf("Enter address in hex (eg.1234ABCD)->");
         scanf("%x", &_iAddress);
         printf("Address 0x%x\n", _iAddress);

         printf("MSB in hex->");
         scanf("%x", &_iMSB);
         printf("MSB value =0x%x\n", _iMSB);

         printf("LSB in hex->");
         scanf("%x", &_iLSB);
         printf("LSB value =0x%x\n", _iLSB);

         printf("Going to write QFPROM at 0x%x, MSB=0x%x, LSB=0x%x\n", _iAddress, _iMSB, _iLSB);
         printf("Enter y to confirm, n to cancel->");

         sscanf("%c", &_cConfirm);

         _cConfirm = getch();

         if(_cConfirm == 'y')
         {
            printf("\nWrote QFPROM\n");
            _iStatus = QLIB_DOWNLOAD_QFprom_Write( g_hResourceContext,  _iAddress, _iMSB, _iLSB,  &_iErrorCode);
            printf("QLIB_DOWNLOAD_QFprom_Write():Status = %d, ErrorCode=%d\n", _iStatus, _iErrorCode);
         }
      }
      else if(cKey == '3')
      {
         printf("Read QFPROM\n");
         printf("Enter address in hex (eg.1234ABCD)->");
         scanf("%x", &_iAddress);
         printf("Address 0x%x\n", _iAddress);

         printf("Enter address tyoe (0=Corrected ,1 = Raw)->");
         scanf("%d", &_iAddressType);
         printf("Address type =%d\n", _iAddressType);

         printf("Going to read QFPROM at 0x%x, address type= %d\n", _iAddress, _iAddressType);
         printf("Enter y to confirm, n to cancel->");

         sscanf("%c", &_cConfirm);
         _cConfirm = getch();

         if(_cConfirm == 'y')
         {
            printf("\nRead QFPROM\n");
            _iStatus = QLIB_DOWNLOAD_QFprom_Read( g_hResourceContext,   _iAddress, _iAddressType, &_iMSB, &_iLSB,  &_iErrorCode);
            printf("QLIB_DOWNLOAD_QFprom_Read():Status = %d, MSB=0x%x, LSB=0x%x, ErrorCode=%d\n", _iStatus, _iMSB, _iLSB, _iErrorCode);
         }
      }
      else if (cKey == '4' )
      {
         return bOk;
      }
   }
   return true;
}

unsigned char Test_FTM_NFC( void )
{
   while(1)
   {
      unsigned char bOk=true;
      unsigned char cKey;

      printf("----------------\nNFC FTM Test\n----------------\n");

      printf("\n 0 = Enable NFC log packets");
      printf("\n 1 = NFC I2C Slave Write");
      printf("\n 2 = NFC I2C Slave Read");
      printf("\n 3 = NFCC Command-Response (hard coded example; 20 00 01 01 = NCI Core Reset)");
      printf("\n 4 = NFCC Command-Response (hard coded example; 20 01 00    = NCI Core Init)");
      printf("\n 5 = NFCC Command-Response (user entry)");
      printf("\n 6 = Display NCI notification log packets (get one packet at a time)");
      printf("\n 7 = Display NCI notification log packets (get all packets as a string)");
      printf("\n 8 = Clear NCI notification log packet buffer");
      printf("\n 9 = SEND DATA (user entry)");
      printf("\n P = Polling Mode (all types)");
      printf("\n A = Polling Mode Type A only");
      printf("\n B = Polling Mode Type B only");
      printf("\n F = Polling Mode Type F only");
      printf("\n D = Detect Tag");
      printf("\n L = Listening Mode (all types)");
      printf("\n G = Listening Mode Type A only");
      printf("\n H = Listening Mode Type B only");
      printf("\n J = Listening Mode Type F only");
      printf("\n K = Detect Field");
      printf("\n R = Run NCI sequence file");
      printf("\n N = Turn ON  TX CW");
      printf("\n M = Turn OFF TX CW");
      printf("\n S = Detect SWP1");
      printf("\n T = Self Test");
      printf("\n Q = Quit\n\n");

      cKey = toupper(getch());

      if (cKey == 'R' )
      {
         // QLIB_FTM_NFC_ExecuteNciSequenceFile

         char sFilename[1024] = {0};
         printf("\nFilename? " );
         scanf("%s", sFilename );

         char sOutputStr[64000] = {0};
         int iErrorCode = 0;
         int iErrorLineNum = 0;

         bOk = QLIB_FTM_NFC_ExecuteNciSequenceFile( g_hResourceContext, sFilename, sOutputStr, &iErrorCode, &iErrorLineNum );
         bOk &= (iErrorCode == 0);

         printf("\nQLIB_FTM_NFC_ExecuteNciSequenceFile: %s", bOk ? "PASS" : "FAIL" );
         printf("\niErrorCode: %d", iErrorCode );
         printf("\niErrorLineNum: %d", iErrorLineNum );
         printf("\n%s", sOutputStr );
         printf("\n\n");
      }

      if (cKey == '1' )
      {
         // QLIB_FTM_NFC_I2C_SLAVE_WRITE (hard coded example)

         unsigned char Nfc_i2c_slave_id = 0x0E;
         unsigned char Nfc_nb_reg_writes = 0;
         unsigned char Nfc_i2c_reg_addr[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
         unsigned char Nfc_i2c_reg_val[15]  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
         unsigned char Nfc_i2c_slave_status = 254;
         unsigned short buf_input = 0;

         // get Nfc_i2c_slave_id
         do
         {
            printf("\nPlease enter Nfc_i2c_slave_id (00 to 7F hexadecimal): ");
            scanf("%hx", & buf_input );
         }
         while (buf_input > 0x7F);
         Nfc_i2c_slave_id = (unsigned char) buf_input;

         // get Nfc_nb_reg_writes
         do
         {
            printf("\nPlease enter Nfc_nb_reg_writes (1 to 15 decimal): ");
            scanf("%hd", & buf_input );
         }
         while ((buf_input < 1) || (buf_input > 15));
         Nfc_nb_reg_writes = (unsigned char) buf_input;

         // get addresses and values to write
         int ii=0;
         for (ii=0; ii<Nfc_nb_reg_writes; ii++)
         {
            unsigned short thisByte = 255;
            do
            {
               printf("\nPlease enter Nfc_i2c_reg_addr[%d] (0 to FF hexadecimal): ", ii);
               scanf("%hx", & thisByte );
            }
            while (thisByte > 255);
            Nfc_i2c_reg_addr[ii] = (unsigned char) thisByte;

            do
            {
               printf("\nPlease enter Nfc_i2c_reg_val[%d] (0 to FF hexadecimal): ", ii);
               scanf("%hx", & thisByte );
            }
            while (thisByte > 255);
            Nfc_i2c_reg_val[ii] = (unsigned char) thisByte;
         }

         bOk = QLIB_FTM_NFC_I2C_SLAVE_WRITE(
                  g_hResourceContext,
                  Nfc_i2c_slave_id,
                  Nfc_nb_reg_writes,
                  Nfc_i2c_reg_addr,
                  Nfc_i2c_reg_val,
                  &Nfc_i2c_slave_status);

         printf("\nQLIB_FTM_NFC_I2C_SLAVE_WRITE: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Nfc_i2c_slave_id: 0x%02X", Nfc_i2c_slave_id );
         printf("\n  Nfc_nb_reg_writes: %d", Nfc_nb_reg_writes );
         for (ii=0; ii<Nfc_nb_reg_writes; ii++)
         {
            printf("\n  Addr%02d=0x%02X, Val%02d=0x%02X", ii, Nfc_i2c_reg_addr[ii], ii,Nfc_i2c_reg_val[ii] );
         }
         printf("\n  Nfc_i2c_slave_status: 0x%02X\n\n", Nfc_i2c_slave_status );
      }
      else if (cKey == '2' )
      {
         // QLIB_FTM_NFC_I2C_SLAVE_READ (hard coded example)

         unsigned char Nfc_i2c_slave_id = 0x0E;
         unsigned char Nfc_nb_reg_reads_in = 0;
         unsigned char Nfc_i2c_reg_addr_in[15] =  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
         unsigned char Nfc_i2c_reg_addr_out[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
         unsigned char Nfc_i2c_reg_val_out[15]  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
         unsigned char Nfc_i2c_slave_status = 255;
         unsigned char Nfc_nb_reg_reads_out = 0;
         unsigned short buf_input = 0;

         // get Nfc_i2c_slave_id
         do
         {
            printf("\nPlease enter Nfc_i2c_slave_id (00 to 7F hexadecimal): ");
            scanf("%hx", & buf_input );
         }
         while (buf_input > 0x7F);
         Nfc_i2c_slave_id = (unsigned char) buf_input;

         // get Nfc_nb_reg_reads_in
         do
         {
            printf("\nPlease enter Nfc_nb_reg_reads_in (1 to 15 decimal): ");
            scanf("%hd", & buf_input );
         }
         while ((buf_input < 1) || (buf_input > 15));
         Nfc_nb_reg_reads_in = (unsigned char) buf_input;

         // get addresses to read
         int ii=0;
         for (ii=0; ii<Nfc_nb_reg_reads_in; ii++)
         {
            unsigned short thisByte = 255;
            do
            {
               printf("\nPlease enter Nfc_i2c_reg_addr_in[%d] (0 to FF hexadecimal): ", ii);
               scanf("%hx", & thisByte );
            }
            while (thisByte > 255);
            Nfc_i2c_reg_addr_in[ii] = (unsigned char) thisByte;
         }

         bOk = QLIB_FTM_NFC_I2C_SLAVE_READ(
                  g_hResourceContext,
                  Nfc_i2c_slave_id,
                  Nfc_nb_reg_reads_in,
                  Nfc_i2c_reg_addr_in,
                  &Nfc_i2c_slave_status,
                  &Nfc_nb_reg_reads_out,
                  Nfc_i2c_reg_addr_out,
                  Nfc_i2c_reg_val_out );

         printf("\nQLIB_FTM_NFC_I2C_SLAVE_READ: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Nfc_i2c_slave_id: 0x%02X", Nfc_i2c_slave_id );

         // display addresses to read
         printf("\n  Nfc_nb_reg_reads_in: %d", Nfc_nb_reg_reads_in );
         char sNfc_i2c_reg_addr_in[1024] = "";
         char bytestr[4] = {0};
         for (ii=0; ii<Nfc_nb_reg_reads_in; ii++)
         {
            _snprintf(bytestr, sizeof(bytestr), "%02X ", Nfc_i2c_reg_addr_in[ii]);
            strncat(sNfc_i2c_reg_addr_in, bytestr, sizeof(sNfc_i2c_reg_addr_in)-strlen(sNfc_i2c_reg_addr_in)-1);
         }
         printf("\n  Nfc_i2c_reg_addr_in: %s", sNfc_i2c_reg_addr_in );

         // display data read
         printf("\n  Nfc_nb_reg_reads_out: %d", Nfc_nb_reg_reads_out );
         for (ii=0; ii<Nfc_nb_reg_reads_out; ii++)
         {
            printf("\n    Addr%02d=0x%02X, Val%02d=0x%02X", ii, Nfc_i2c_reg_addr_out[ii], ii,Nfc_i2c_reg_val_out[ii] );
         }
         printf("\n  Nfc_i2c_slave_status: 0x%02X\n\n", Nfc_i2c_slave_status );
      }
      else if (cKey == '3' )
      {
         // QLIB_FTM_NFC_NFCC_CMD_RSP (hard coded example - NCI Core Reset)
         //
         // send a NFCC cmd and get rsp, but do not wait for alog packet.
         // this is for nci commands that do not return notification packets.

         unsigned short Nfc_nfcc_cmd_len = 4;
         unsigned char  Nfc_nfcc_cmd[4] = {0x20, 0x00, 0x01, 0x01};
         unsigned short Nfc_nfcc_rsp_len = 0;
         unsigned char  Nfc_nfcc_rsp[258];

         bOk = QLIB_FTM_NFC_NFCC_CMD_RSP( g_hResourceContext, Nfc_nfcc_cmd_len, Nfc_nfcc_cmd, &Nfc_nfcc_rsp_len, Nfc_nfcc_rsp );

         printf("\nQLIB_FTM_NFC_NFCC_CMD_RSP: %s", bOk ? "PASS" : "FAIL" );

         // display Nfc_nfcc_cmd
         printf("\n  Nfc_nfcc_cmd_len: %d", Nfc_nfcc_cmd_len );
         char bytestr[4] = {0};
         char hexstr[1024];
         hexstr[0] = 0;
         int ii = 0;
         for (ii=0; ii<Nfc_nfcc_cmd_len; ii++)
         {
            _snprintf(bytestr, sizeof(bytestr), "%02X ", Nfc_nfcc_cmd[ii] );
            strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);
         }
         printf("\n  Nfc_nfcc_cmd: %s", hexstr );

         // display Nfc_nfcc_rsp
         printf("\n  Nfc_nfcc_rsp_len: %d", Nfc_nfcc_rsp_len );
         hexstr[0] = 0;
         for (ii=0; ii<Nfc_nfcc_rsp_len; ii++)
         {
            _snprintf(bytestr, sizeof(bytestr), "%02X ", Nfc_nfcc_rsp[ii] );
            strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);
         }
         printf("\n  Nfc_nfcc_rsp: %s\n\n", hexstr );
      }
      else if (cKey == '4' )
      {
         // QLIB_FTM_NFC_NFCC_CMD_RSP (hard coded example - NCI Core Init)
         //
         // send a NFCC cmd and get rsp, but do not wait for alog packet.
         // this is for nci commands that do not return notification packets.

         unsigned short Nfc_nfcc_cmd_len = 3;
         unsigned char  Nfc_nfcc_cmd[3] = {0x20, 0x01, 0x00};
         unsigned short Nfc_nfcc_rsp_len = 0;
         unsigned char  Nfc_nfcc_rsp[258];

         bOk = QLIB_FTM_NFC_NFCC_CMD_RSP( g_hResourceContext, Nfc_nfcc_cmd_len, Nfc_nfcc_cmd, &Nfc_nfcc_rsp_len, Nfc_nfcc_rsp );

         printf("\nQLIB_FTM_NFC_NFCC_CMD_RSP: %s", bOk ? "PASS" : "FAIL" );

         // display Nfc_nfcc_cmd
         printf("\n  Nfc_nfcc_cmd_len: %d", Nfc_nfcc_cmd_len );
         char bytestr[4] = {0};
         char hexstr[1024];
         hexstr[0] = 0;
         int ii = 0;
         for (ii=0; ii<Nfc_nfcc_cmd_len; ii++)
         {
            _snprintf(bytestr, sizeof(bytestr), "%02X ", Nfc_nfcc_cmd[ii] );
            strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);
         }
         printf("\n  Nfc_nfcc_cmd: %s", hexstr );

         // display Nfc_nfcc_rsp
         printf("\n  Nfc_nfcc_rsp_len: %d", Nfc_nfcc_rsp_len );
         hexstr[0] = 0;
         for (ii=0; ii<Nfc_nfcc_rsp_len; ii++)
         {
            _snprintf(bytestr, sizeof(bytestr), "%02X ", Nfc_nfcc_rsp[ii] );
            strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);
         }
         printf("\n  Nfc_nfcc_rsp: %s\n\n", hexstr );
      }
      else if (cKey == '5' )
      {
         // QLIB_FTM_NFC_NFCC_CMD_RSP( user entry, one byte at a time, hexadecimal)
         //
         // send a NFCC cmd and get rsp, but do not wait for alog packet.
         // this is for nci commands that do not return notification packets.

         unsigned short Nfc_nfcc_cmd_len=0;
         do
         {
            printf("\nPlease enter Nfc_nfcc_cmd_len (1 to 255 decimal): ");
            scanf("%hd", & Nfc_nfcc_cmd_len );
         }
         while ((Nfc_nfcc_cmd_len < 1) || (Nfc_nfcc_cmd_len > 255));

         unsigned char Nfc_nfcc_cmd[258];
         int ii=0;
         for (ii=0; ii<Nfc_nfcc_cmd_len; ii++)
         {
            unsigned short thisByte = 255;
            do
            {
               printf("\nPlease enter Nfc_nfcc_cmd[%d] (0 to FF hexadecimal): ", ii);
               scanf("%hx", & thisByte );
            }
            while (thisByte > 255);
            Nfc_nfcc_cmd[ii] = (unsigned char) thisByte;
         }
         unsigned short Nfc_nfcc_rsp_len = 0;
         unsigned char  Nfc_nfcc_rsp[258];

         bOk = QLIB_FTM_NFC_NFCC_CMD_RSP( g_hResourceContext, Nfc_nfcc_cmd_len, Nfc_nfcc_cmd, &Nfc_nfcc_rsp_len, Nfc_nfcc_rsp );

         printf("\nQLIB_FTM_NFC_NFCC_CMD_RSP: %s", bOk ? "PASS" : "FAIL" );

         // display Nfc_nfcc_cmd
         printf("\n  Nfc_nfcc_cmd_len: %d", Nfc_nfcc_cmd_len );
         char bytestr[4] = {0};
         char hexstr[1024];
         hexstr[0] = 0;
         for (ii=0; ii<Nfc_nfcc_cmd_len; ii++)
         {
            _snprintf(bytestr, sizeof(bytestr), "%02X ", Nfc_nfcc_cmd[ii] );
            strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);
         }
         printf("\n  Nfc_nfcc_cmd: %s", hexstr );

         // display Nfc_nfcc_rsp
         printf("\n  Nfc_nfcc_rsp_len: %d", Nfc_nfcc_rsp_len );
         hexstr[0] = 0;
         for (ii=0; ii<Nfc_nfcc_rsp_len; ii++)
         {
            _snprintf(bytestr, sizeof(bytestr), "%02X ", Nfc_nfcc_rsp[ii] );
            strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);
         }
         printf("\n  Nfc_nfcc_rsp: %s\n\n", hexstr );
      }
      else if (cKey == '6' )
      {
         // Display NCI notification log packets (get one packet at a time)

         unsigned char NFC_NTF_Packet[258] = {0};
         unsigned short iPacketSize = 0;
         unsigned int iNumNfcNtfLogs = 0;

         // get # of log packets
         iNumNfcNtfLogs = QLIB_FTM_NFC_GetNumNtfLogs( g_hResourceContext );
         printf("\nQLIB_FTM_NFC_GetNumNtfLogs" );
         printf("\n  iNumNfcNtfLogs: %d", iNumNfcNtfLogs );
         printf("\nQLIB_FTM_NFC_GetOneNtfLog will be called %d times to retrieve buffered log packets with NCI notification data",  iNumNfcNtfLogs);

         unsigned int ii=0;
         for (ii=0; ii < iNumNfcNtfLogs; ii++)
         {
            // get packet #ii
            memset(NFC_NTF_Packet, 0, FTM_NFC_LOG_PAYLOAD_MAX_SIZE);
            QLIB_FTM_NFC_GetOneNtfLog( g_hResourceContext, ii, &iPacketSize, NFC_NTF_Packet );

            // display packet #ii
            char bytestr[4] = {0};
            char hexstr[1024] = {0};
            hexstr[0] = 0;
            int jj=0;
            for (jj=0; jj < iPacketSize; jj++)
            {
               _snprintf(bytestr, sizeof(bytestr), "%02X ", NFC_NTF_Packet[jj] );
               strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);
            }
            printf("\n  NTF %d: %s", ii+1, hexstr );
         }
         printf("\n\n");
      }
      else if (cKey == '7' )
      {
         // Display NCI notification log packets (get all packet data in one string)

         char sBigStr[256000] = {0};

         bOk = QLIB_FTM_NFC_GetAllNtfLogs_String( g_hResourceContext, sBigStr );

         printf("\nQLIB_FTM_NFC_GetAllNtfLogs_String:");
         printf("\n%s\n\n", sBigStr );
      }
      else if (cKey == '8' )
      {
         // Clear NCI notification log packet buffer

         bOk = QLIB_FTM_NFC_ClearNtfLogs( g_hResourceContext );
         printf("\nQLIB_FTM_NFC_ClearNtfLogs: %s\n\n", bOk ? "PASS" : "FAIL" );
      }
      else if (cKey == '0' )
      {
         // Enable NFC log packets

         bOk = QLIB_FTM_NFC_EnableLogs( g_hResourceContext );
         printf("\nQLIB_FTM_NFC_EnableLogs: %s\n\n", bOk ? "PASS" : "FAIL" );
      }
      else if (cKey == '9' )
      {
         //QLIB_FTM_NFC_SEND_DATA

         unsigned short Nfc_nci_pkt_len = 0;
         unsigned char Nfc_nci_payload[258];
         memset(Nfc_nci_payload, 0, 258);

         do
         {
            printf("\nPlease enter Nfc_nci_pkt_len (1 to 258 decimal): ");
            scanf("%hd", & Nfc_nci_pkt_len );
         }
         while ((Nfc_nci_pkt_len < 1) || (Nfc_nci_pkt_len > FTM_NFC_DATA_PAYLOAD_MAX_SIZE));

         int ii=0;
         for (ii=0; ii<Nfc_nci_pkt_len; ii++)
         {
            unsigned short thisByte = 255;
            do
            {
               printf("\nPlease enter Nfc_nci_payload[%d] (0 to FF hexadecimal): ", ii);
               scanf("%hx", & thisByte );
            }
            while (thisByte > 255);
            Nfc_nci_payload[ii] = (unsigned char) thisByte;
         }

         bOk = QLIB_FTM_NFC_SEND_DATA( g_hResourceContext, Nfc_nci_pkt_len, Nfc_nci_payload );

         printf("\nQLIB_FTM_NFC_SEND_DATA: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Nfc_nci_pkt_len: %d", Nfc_nci_pkt_len );

         char bytestr[4] = {0};
         char hexstr[1024];
         hexstr[0] = 0;
         for (ii=0; ii<Nfc_nci_pkt_len; ii++)
         {
            _snprintf(bytestr, sizeof(bytestr), "%02X ", Nfc_nci_payload[ii]);
            strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);
         }
         printf("\n  Nfc_nci_payload: %s\n\n", hexstr );
      }
      else if (cKey == 'P' )
      {
         // Polling Mode all types

         int iErrorCode = 0;
         bOk = QLIB_FTM_NFC_SetPollingMode( g_hResourceContext, &iErrorCode );
         bOk &= (iErrorCode == 0);
         printf("\nQLIB_FTM_NFC_SetPollingMode: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  iErrorCode: %d\n\n", iErrorCode );
      }
      else if (cKey == 'A' )
      {
         // Polling Mode Type A

         int iErrorCode = 0;
         bOk = QLIB_FTM_NFC_PollTypeA( g_hResourceContext, &iErrorCode );
         bOk &= (iErrorCode == 0);
         printf("\nQLIB_FTM_NFC_PollTypeA: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  iErrorCode: %d\n\n", iErrorCode );
      }
      else if (cKey == 'B' )
      {
         // Polling Mode Type B

         int iErrorCode = 0;
         bOk = QLIB_FTM_NFC_PollTypeB( g_hResourceContext, &iErrorCode );
         bOk &= (iErrorCode == 0);
         printf("\nQLIB_FTM_NFC_PollTypeB: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  iErrorCode: %d\n\n", iErrorCode );
      }
      else if (cKey == 'F' )
      {
         // Polling Mode Type F

         int iErrorCode = 0;
         bOk = QLIB_FTM_NFC_PollTypeF( g_hResourceContext, &iErrorCode );
         bOk &= (iErrorCode == 0);
         printf("\nQLIB_FTM_NFC_PollTypeF: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  iErrorCode: %d\n\n", iErrorCode );
      }
      else if (cKey == 'D' )
      {
         // Detect Tag

         char sTagIdNtfPkt[16000] = {0};
         int iErrorCode = 0;

         bOk = QLIB_FTM_NFC_DetectTag( g_hResourceContext, sTagIdNtfPkt, &iErrorCode );
         bOk &= (iErrorCode == 0);

         printf("\nQLIB_FTM_NFC_DetectTag: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  iErrorCode: %d", iErrorCode );
         printf("\n%s\n\n", sTagIdNtfPkt );
      }
      else if (cKey == 'L' )
      {
         // Listening Mode all types

         int iErrorCode = 0;
         bOk = QLIB_FTM_NFC_SetListeningMode( g_hResourceContext, &iErrorCode );
         bOk &= (iErrorCode == 0);
         printf("\nQLIB_FTM_NFC_SetListeningMode: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  iErrorCode: %d\n\n", iErrorCode );
      }
      else if (cKey == 'G' )
      {
         // Listening Mode Type A

         int iErrorCode = 0;
         bOk = QLIB_FTM_NFC_ListenTypeA( g_hResourceContext, &iErrorCode );
         bOk &= (iErrorCode == 0);
         printf("\nQLIB_FTM_NFC_ListenTypeA: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  iErrorCode: %d\n\n", iErrorCode );
      }
      else if (cKey == 'H' )
      {
         // Listening Mode Type B

         int iErrorCode = 0;
         bOk = QLIB_FTM_NFC_ListenTypeB( g_hResourceContext, &iErrorCode );
         bOk &= (iErrorCode == 0);
         printf("\nQLIB_FTM_NFC_ListenTypeB: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  iErrorCode: %d\n\n", iErrorCode );
      }
      else if (cKey == 'J' )
      {
         // Listening Mode Type F

         int iErrorCode = 0;
         bOk = QLIB_FTM_NFC_ListenTypeF( g_hResourceContext, &iErrorCode );
         bOk &= (iErrorCode == 0);
         printf("\nQLIB_FTM_NFC_ListenTypeF: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  iErrorCode: %d\n\n", iErrorCode );
      }
      else if (cKey == 'K' )
      {
         // Detect Field

         char sNtfPkts[16000] = {0};
         int iErrorCode = 0;

         bOk = QLIB_FTM_NFC_DetectField( g_hResourceContext, sNtfPkts, &iErrorCode );
         bOk &= (iErrorCode == 0);

         printf("\nQLIB_FTM_NFC_DetectField: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  iErrorCode: %d", iErrorCode );
         printf("\n%s\n\n", sNtfPkts );
      }
      else if (cKey == 'N' )
      {
         // Turn ON TX CW

         bOk = QLIB_FTM_NFC_Turn_Tx_CW_On( g_hResourceContext );
         printf("\nQLIB_FTM_NFC_Turn_Tx_CW_On: %s\n\n", bOk ? "PASS" : "FAIL" );
      }
      else if (cKey == 'M' )
      {
         // Turn OFF TX CW

         bOk = QLIB_FTM_NFC_Turn_Tx_CW_Off( g_hResourceContext );
         printf("\nQLIB_FTM_NFC_Turn_Tx_CW_Off: %s\n\n", bOk ? "PASS" : "FAIL" );
      }
      else if (cKey == 'S' )
      {
         // Detect SWP1

         char sSWP1Detect[256000] = {0};
         int iSWP1Detected = 2;
         int iErrorCode = 0;

         bOk = QLIB_FTM_NFC_SWP1_Detect( g_hResourceContext, &iSWP1Detected, sSWP1Detect, &iErrorCode );
         bOk &= (iErrorCode == 0);

         printf("\nQLIB_FTM_NFC_SWP1_Detect: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  iErrorCode: %d", iErrorCode );
         printf("\n  iSWP1Detected: %d", iSWP1Detected );
         printf("\n%s\n\n", sSWP1Detect );
      }
      else if (cKey == 'T' )
      {
         // Self Test

         char sSelfTestResult[32000] = {0};
         unsigned char bCheckSWP1 = 1;
         unsigned char bCheckSWP2 = 1;
         unsigned int iRfOscLL = 102912;     // A = -2 = aSelfTestBytes[9]
         unsigned int iRfOscUL = 141824;     // B =  2 = aSelfTestBytes[10]
         unsigned char iErrorCodeNFCEE = 0;
         unsigned char iErrorCodeRfOsc = 0;
         unsigned char iErrorCodeCarrierDet = 0;
         int iNFCEE = 0;
         int iRfOsc = 0;
         int iCarrierDet = 0;

         bOk = QLIB_FTM_NFC_Self_Test(
                  g_hResourceContext,
                  bCheckSWP1,
                  bCheckSWP2,
                  iRfOscLL,
                  iRfOscUL,
                  sSelfTestResult,
                  &iErrorCodeNFCEE,
                  &iErrorCodeRfOsc,
                  &iErrorCodeCarrierDet,
                  &iNFCEE,
                  &iRfOsc,
                  &iCarrierDet );

         printf("\nQLIB_FTM_NFC_Self_Test: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  iErrorCodeNFCEE: 0x%02X", iErrorCodeNFCEE );
         printf("\n  iErrorCodeRfOsc: 0x%02X", iErrorCodeRfOsc );
         printf("\n  iErrorCodeCarrierDet: 0x%02X", iErrorCodeCarrierDet );
         printf("\n  iNFCEE: 0x%08X", iNFCEE );
         printf("\n  iRfOsc: 0x%08X (%d)", iRfOsc, iRfOsc );
         printf("\n  iCarrierDet: 0x%08X (%d)", iCarrierDet, iCarrierDet );
         printf("\n%s\n\n", sSelfTestResult );
      }

      // return if 'Q' key, otherwise stay in the NFC menu
      if (cKey == 'Q' )
      {
         break;
      }
   } // end while

   return true;
}

unsigned char Test_FTM_FM( void )
{
   while(1)
   {
      unsigned char bOk=true;
      unsigned char cKey;

      printf("----------------\nFM FTM Test\n----------------\n");

      printf("\n 1 = FM Rx");
      printf("\n 2 = FM Tx");
      printf("\n 3 = FM Alternate Frequency (AF)");
      printf("\n 4 = FM Audio");
      printf("\n Q = Quit\n\n");

      cKey = toupper(getch());

      if (cKey == '1' )
      {
         // FM Rx submenu
         bOk = Test_FTM_FM_RX();
      }
      else if (cKey == '2' )
      {
         // FM Tx submenu
         bOk = Test_FTM_FM_TX();
      }
      else if (cKey == '3' )
      {
         // FM Misc submenu
         bOk = Test_FTM_FM_AF();
      }
      else if (cKey == '4' )
      {
         // FM Misc submenu
         bOk = Test_FTM_FM_Audio();
      }

      // return if 'Q' key, otherwise stay in the FM menu
      if (cKey == 'Q' )
      {
         break;
      }
   } // end while

   return true;
}

unsigned char Test_FTM_FM_RX( void )
{
   while(1)
   {
      unsigned char bOk=true;
      unsigned char cKey;

      printf("\n----------------\nFM RX FTM Test\n----------------\n");

      printf("\n ! = Enable Receiver (FM_US_EUROPE_BAND=0, FM_DE_EMP75=0, FM_CHSPACE_200_KHZ=0, FM_RDS_STD_RBDS=0, 0, 0)");
      printf("\n 1 = Disable Receiver");
      printf("\n 2 = Configure Receiver (FM_US_EUROPE_BAND=0, FM_DE_EMP75=0, FM_CHSPACE_200_KHZ=0, FM_RDS_STD_NONE=2, 0, 0)");
      printf("\n 3 = Configure Receiver (FM_US_EUROPE_BAND=0, FM_DE_EMP75=0, FM_CHSPACE_200_KHZ=0, FM_RDS_STD_RBDS=0, 0, 0)");
      printf("\n 4 = Configure Receiver (FM_US_EUROPE_BAND=0, FM_DE_EMP50=1, FM_CHSPACE_100_KHZ=1, FM_RDS_STD_RDS=1,  0, 0)");
      printf("\n 5 = Configure Receiver (FM_JAPAN_STANDARD_BAND=1, FM_DE_EMP75=0, FM_CHSPACE_100_KHZ=1, FM_RDS_STD_NONE=2, 0, 0)");

      printf("\n 6 = Set Mute Mode (FM_RX_NOMUTE=0)");
      printf("\n 7 = Set Mute Mode (FM_RX_MUTELEFT=1)");
      printf("\n 8 = Set Mute Mode (FM_RX_MUTERIGHT=2)");
      printf("\n 9 = Set Mute Mode (FM_RX_MUTEBOTH=3)");

      printf("\n 0 = Set Stereo Mode (FM_RX_AUTO_MONO_STEREO_MODE=0)");
      printf("\n - = Set Stereo Mode (FM_RX_FORCE_MONO_ONLY_MODE=1)");

      printf("\n = = Get Station Parameters");
      printf("\n + = Get Scaled RSSI");

      printf("\n A = Set Station (96500 KHz)");
      printf("\n S = Set Station (94100 KHz)");
      printf("\n D = Set Station (94900 KHz)");
      printf("\n F = Set Station (100700 KHz)");
      printf("\n G = Set Station (105300 KHz)");
      printf("\n H = Set Station (91100 KHz)");

      printf("\n J = Seek next station   (FM_RX_SRCH_MODE_SEEK=0, FM_RX_DWELL_PERIOD_0S, FM_RX_SEARCHDIR_UP=0)");
      printf("\n K = Search all stations (FM_RX_SRCH_MODE_SCAN=1, FM_RX_DWELL_PERIOD_7S, FM_RX_SEARCHDIR_UP=0)");
      printf("\n L = Cancel Search");
      printf("\n N = Set Signal Threshold");
      printf("\n M = Get Signal Threshold");

      printf("\n Z = Scan 87500 to 107900 KHz with 200 KHz steps (with Set_Station & Get_Station_Parameters)");
      printf("\n X = Scan 87500 to 107900 KHz with 200 KHz steps (with Set_Station & Get_Station_Parameters), 4 sec per channel");
      printf("\n C = Scan 76000 to  90000 KHz with 100 KHz steps (with Set_Station & Get_Station_Parameters)");
      printf("\n V = Scan 76000 to  90000 KHz with 100 KHz steps (with Set_Station & Get_Station_Parameters), 4 sec per channel");

      printf("\n [ = Set Antenna (FM_ANTENNA_WIRED_HEADSET=0)");
      printf("\n ] = Set Antenna (FM_ANTENNA_PWB=1)");
      printf("\n { = Set Soft Mute (FM_RX_SOFTMUTE_DISABLED=0)");
      printf("\n } = Set Soft Mute (FM_RX_SOFTMUTE_ENABLED=1)");
      printf("\n ; = Set Highside/Lowside Injection");
      printf("\n : = Set Notch Filter");
      printf("\n < = Get RDS Group Counters (with reset)");
      printf("\n > = Get RDS Group Counters");
      printf("\n ? = Get RDS Program Service");
      printf("\n / = Get RDS Radio Text");

      printf("\n P = Get FM Pin Connectivity");

      printf("\n Q = Quit\n\n");

      cKey = toupper(getch());

      if (cKey == '!' )
      {
         // QLIB_FTM_FM_RX_ENABLE_RECEIVER

         FTM_FM_BandLimits_Enum iRadioBand  = FM_US_EUROPE_BAND;
         FTM_FM_Emphasis_Enum   iEmphasis   = FM_DE_EMP75;
         FTM_FM_ChanSpacing_Enum iChSpacing = FM_CHSPACE_200_KHZ;
         FTM_FM_RdsStd_Enum     iRdsStd     = FM_RDS_STD_RBDS;
         unsigned long iUserDefinedBandFreqMin = 0;
         unsigned long iUserDefinedBandFreqMax = 0;

         bOk = QLIB_FTM_FM_RX_ENABLE_RECEIVER(g_hResourceContext,
                                              (unsigned char) iRadioBand,
                                              (unsigned char) iEmphasis,
                                              (unsigned char) iChSpacing,
                                              (unsigned char) iRdsStd,
                                              iUserDefinedBandFreqMin,
                                              iUserDefinedBandFreqMax );

         printf("\nQLIB_FTM_FM_RX_ENABLE_RECEIVER: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Radio Band: %d", iRadioBand );
         printf("\n  Emphasis: %d", iEmphasis );
         printf("\n  Ch Spacing: %d", iChSpacing );
         printf("\n  Rds Std: %d", iRdsStd );
         printf("\n  User Defined Band Freq Min: %d", iUserDefinedBandFreqMin );
         printf("\n  User Defined Band Freq Max: %d\n", iUserDefinedBandFreqMax );
      }
      else if (cKey == '1' )
      {
         // QLIB_FTM_FM_RX_DISABLE_RECEIVER
         bOk = QLIB_FTM_FM_RX_DISABLE_RECEIVER(g_hResourceContext);
         printf("\nQLIB_FTM_FM_RX_DISABLE_RECEIVER: %s\n", bOk ? "PASS" : "FAIL" );
      }
      else if (cKey == '2' )
      {
         // QLIB_FTM_FM_RX_CONFIGURE_RECEIVER

         FTM_FM_BandLimits_Enum iRadioBand  = FM_US_EUROPE_BAND;
         FTM_FM_Emphasis_Enum   iEmphasis   = FM_DE_EMP75;
         FTM_FM_ChanSpacing_Enum iChSpacing = FM_CHSPACE_200_KHZ;
         FTM_FM_RdsStd_Enum     iRdsStd     = FM_RDS_STD_NONE;
         unsigned long iUserDefinedBandFreqMin = 0;
         unsigned long iUserDefinedBandFreqMax = 0;

         bOk = QLIB_FTM_FM_RX_CONFIGURE_RECEIVER(g_hResourceContext,
                                                 (unsigned char) iRadioBand,
                                                 (unsigned char) iEmphasis,
                                                 (unsigned char) iChSpacing,
                                                 (unsigned char) iRdsStd,
                                                 iUserDefinedBandFreqMin,
                                                 iUserDefinedBandFreqMax );

         printf("\nQLIB_FTM_FM_RX_CONFIGURE_RECEIVER: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Radio Band: %d", iRadioBand );
         printf("\n  Emphasis: %d", iEmphasis );
         printf("\n  Ch Spacing: %d", iChSpacing );
         printf("\n  Rds Std: %d", iRdsStd );
         printf("\n  User Defined Band Freq Min: %d", iUserDefinedBandFreqMin );
         printf("\n  User Defined Band Freq Max: %d\n", iUserDefinedBandFreqMax );
      }
      else if (cKey == '3' )
      {
         // QLIB_FTM_FM_RX_CONFIGURE_RECEIVER

         FTM_FM_BandLimits_Enum iRadioBand  = FM_US_EUROPE_BAND;
         FTM_FM_Emphasis_Enum   iEmphasis   = FM_DE_EMP75;
         FTM_FM_ChanSpacing_Enum iChSpacing = FM_CHSPACE_200_KHZ;
         FTM_FM_RdsStd_Enum     iRdsStd     = FM_RDS_STD_RBDS;
         unsigned long iUserDefinedBandFreqMin = 0;
         unsigned long iUserDefinedBandFreqMax = 0;

         bOk = QLIB_FTM_FM_RX_CONFIGURE_RECEIVER(g_hResourceContext,
                                                 (unsigned char) iRadioBand,
                                                 (unsigned char) iEmphasis,
                                                 (unsigned char) iChSpacing,
                                                 (unsigned char) iRdsStd,
                                                 iUserDefinedBandFreqMin,
                                                 iUserDefinedBandFreqMax );

         printf("\nQLIB_FTM_FM_RX_CONFIGURE_RECEIVER: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Radio Band: %d", iRadioBand );
         printf("\n  Emphasis: %d", iEmphasis );
         printf("\n  Ch Spacing: %d", iChSpacing );
         printf("\n  Rds Std: %d", iRdsStd );
         printf("\n  User Defined Band Freq Min: %d", iUserDefinedBandFreqMin );
         printf("\n  User Defined Band Freq Max: %d\n", iUserDefinedBandFreqMax );
      }
      else if (cKey == '4' )
      {
         // QLIB_FTM_FM_RX_CONFIGURE_RECEIVER

         FTM_FM_BandLimits_Enum iRadioBand  = FM_US_EUROPE_BAND;
         FTM_FM_Emphasis_Enum   iEmphasis   = FM_DE_EMP50;
         FTM_FM_ChanSpacing_Enum iChSpacing = FM_CHSPACE_100_KHZ;
         FTM_FM_RdsStd_Enum     iRdsStd     = FM_RDS_STD_RDS;
         unsigned long iUserDefinedBandFreqMin = 0;
         unsigned long iUserDefinedBandFreqMax = 0;

         bOk = QLIB_FTM_FM_RX_CONFIGURE_RECEIVER(g_hResourceContext,
                                                 (unsigned char) iRadioBand,
                                                 (unsigned char) iEmphasis,
                                                 (unsigned char) iChSpacing,
                                                 (unsigned char) iRdsStd,
                                                 iUserDefinedBandFreqMin,
                                                 iUserDefinedBandFreqMax );

         printf("\nQLIB_FTM_FM_RX_CONFIGURE_RECEIVER: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Radio Band: %d", iRadioBand );
         printf("\n  Emphasis: %d", iEmphasis );
         printf("\n  Ch Spacing: %d", iChSpacing );
         printf("\n  Rds Std: %d", iRdsStd );
         printf("\n  User Defined Band Freq Min: %d", iUserDefinedBandFreqMin );
         printf("\n  User Defined Band Freq Max: %d\n", iUserDefinedBandFreqMax );
      }
      else if (cKey == '5' )
      {
         // QLIB_FTM_FM_RX_CONFIGURE_RECEIVER

         FTM_FM_BandLimits_Enum iRadioBand  = FM_JAPAN_STANDARD_BAND;
         FTM_FM_Emphasis_Enum   iEmphasis   = FM_DE_EMP75;
         FTM_FM_ChanSpacing_Enum iChSpacing = FM_CHSPACE_100_KHZ;
         FTM_FM_RdsStd_Enum     iRdsStd     = FM_RDS_STD_NONE;
         unsigned long iUserDefinedBandFreqMin = 0;
         unsigned long iUserDefinedBandFreqMax = 0;

         bOk = QLIB_FTM_FM_RX_CONFIGURE_RECEIVER(g_hResourceContext,
                                                 (unsigned char) iRadioBand,
                                                 (unsigned char) iEmphasis,
                                                 (unsigned char) iChSpacing,
                                                 (unsigned char) iRdsStd,
                                                 iUserDefinedBandFreqMin,
                                                 iUserDefinedBandFreqMax );

         printf("\nQLIB_FTM_FM_RX_CONFIGURE_RECEIVER: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Radio Band: %d", iRadioBand );
         printf("\n  Emphasis: %d", iEmphasis );
         printf("\n  Ch Spacing: %d", iChSpacing );
         printf("\n  Rds Std: %d", iRdsStd );
         printf("\n  User Defined Band Freq Min: %d", iUserDefinedBandFreqMin );
         printf("\n  User Defined Band Freq Max: %d\n", iUserDefinedBandFreqMax );
      }
      else if (cKey == '6' )
      {
         // QLIB_FTM_FM_RX_SET_MUTE_MODE
         FTM_FM_RxMute_Enum iRxMute = FM_RX_NOMUTE;
         bOk = QLIB_FTM_FM_RX_SET_MUTE_MODE( g_hResourceContext, (unsigned char) iRxMute);
         printf("\nQLIB_FTM_FM_RX_SET_MUTE_MODE: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Rx Mute Mode: %d\n", iRxMute );
      }
      else if (cKey == '7' )
      {
         // QLIB_FTM_FM_RX_SET_MUTE_MODE
         FTM_FM_RxMute_Enum iRxMute = FM_RX_MUTELEFT;
         bOk = QLIB_FTM_FM_RX_SET_MUTE_MODE( g_hResourceContext, (unsigned char) iRxMute);
         printf("\nQLIB_FTM_FM_RX_SET_MUTE_MODE: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Rx Mute Mode: %d\n", iRxMute );
      }
      else if (cKey == '8' )
      {
         // QLIB_FTM_FM_RX_SET_MUTE_MODE
         FTM_FM_RxMute_Enum iRxMute = FM_RX_MUTERIGHT;
         bOk = QLIB_FTM_FM_RX_SET_MUTE_MODE( g_hResourceContext, (unsigned char) iRxMute);
         printf("\nQLIB_FTM_FM_RX_SET_MUTE_MODE: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Rx Mute Mode: %d\n", iRxMute );
      }
      else if (cKey == '9' )
      {
         // QLIB_FTM_FM_RX_SET_MUTE_MODE
         FTM_FM_RxMute_Enum iRxMute = FM_RX_MUTEBOTH;
         bOk = QLIB_FTM_FM_RX_SET_MUTE_MODE( g_hResourceContext, (unsigned char) iRxMute);
         printf("\nQLIB_FTM_FM_RX_SET_MUTE_MODE: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Rx Mute Mode: %d\n", iRxMute );
      }
      else if (cKey == '0' )
      {
         // QLIB_FTM_FM_RX_SET_STEREO_MODE
         FTM_FM_RxStereoMonoMode_Enum iStereo = FM_RX_AUTO_MONO_STEREO_MODE;
         bOk = QLIB_FTM_FM_RX_SET_STEREO_MODE( g_hResourceContext, (unsigned char) iStereo);
         printf("\nQLIB_FTM_FM_RX_SET_STEREO_MODE: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Rx Stereo Mode: %d\n", iStereo );
      }
      else if (cKey == '-' )
      {
         // QLIB_FTM_FM_RX_SET_STEREO_MODE
         FTM_FM_RxStereoMonoMode_Enum iStereo = FM_RX_FORCE_MONO_ONLY_MODE;
         bOk = QLIB_FTM_FM_RX_SET_STEREO_MODE( g_hResourceContext, (unsigned char) iStereo);
         printf("\nQLIB_FTM_FM_RX_SET_STEREO_MODE: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Rx Stereo Mode: %d\n", iStereo );
      }
      else if (cKey == '=' )
      {
         // QLIB_FTM_FM_RX_GET_STATION_PARAMETERS

         unsigned long iStationFreq=0;
         unsigned char iServAvble=0;
         unsigned char iRssi=0;
         unsigned char iStereoProgram=0;
         unsigned char iRdsSyncStatus=0;
         unsigned char iMuteMode=0;

         bOk = QLIB_FTM_FM_RX_GET_STATION_PARAMETERS( g_hResourceContext,
                                                      &iStationFreq,
                                                      &iServAvble,
                                                      &iRssi,
                                                      &iStereoProgram,
                                                      &iRdsSyncStatus,
                                                      &iMuteMode );

         printf("\nQLIB_FTM_FM_RX_GET_STATION_PARAMETERS: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iStationFreq: %d",  iStationFreq);
         printf("\n  iServAvble: %d",  iServAvble);
         printf("\n  iRssi: %d",  iRssi);
         printf("\n  iStereoProgram: %d",  iStereoProgram);
         printf("\n  iRdsSyncStatus: %d",  iRdsSyncStatus);
         printf("\n  iMuteMode: %d\n",  iMuteMode);
      }
      else if (cKey == 'A' )
      {
         // QLIB_FTM_FM_RX_SET_STATION
         unsigned long iFreqkHz = 96500;
         bOk = QLIB_FTM_FM_RX_SET_STATION( g_hResourceContext, iFreqkHz );
         printf("\nQLIB_FTM_FM_RX_SET_STATION: %s", bOk ? "PASS" : "FAIL");
         printf("\n  FrequencyKHz: %d\n",  iFreqkHz);
      }
      else if (cKey == 'S' )
      {
         // QLIB_FTM_FM_RX_SET_STATION
         unsigned long iFreqkHz = 94100;
         bOk = QLIB_FTM_FM_RX_SET_STATION( g_hResourceContext, iFreqkHz );
         printf("\nQLIB_FTM_FM_RX_SET_STATION: %s", bOk ? "PASS" : "FAIL");
         printf("\n  FrequencyKHz: %d\n",  iFreqkHz);
      }
      else if (cKey == 'D' )
      {
         // QLIB_FTM_FM_RX_SET_STATION
         unsigned long iFreqkHz = 94900;
         bOk = QLIB_FTM_FM_RX_SET_STATION( g_hResourceContext, iFreqkHz );
         printf("\nQLIB_FTM_FM_RX_SET_STATION: %s", bOk ? "PASS" : "FAIL");
         printf("\n  FrequencyKHz: %d\n",  iFreqkHz);
      }
      else if (cKey == 'F' )
      {
         // QLIB_FTM_FM_RX_SET_STATION
         unsigned long iFreqkHz = 100700;
         bOk = QLIB_FTM_FM_RX_SET_STATION( g_hResourceContext, iFreqkHz );
         printf("\nQLIB_FTM_FM_RX_SET_STATION: %s", bOk ? "PASS" : "FAIL");
         printf("\n  FrequencyKHz: %d\n",  iFreqkHz);
      }
      else if (cKey == 'G' )
      {
         // QLIB_FTM_FM_RX_SET_STATION
         unsigned long iFreqkHz = 105300;
         bOk = QLIB_FTM_FM_RX_SET_STATION( g_hResourceContext, iFreqkHz );
         printf("\nQLIB_FTM_FM_RX_SET_STATION: %s", bOk ? "PASS" : "FAIL");
         printf("\n  FrequencyKHz: %d\n",  iFreqkHz);
      }
      else if (cKey == 'H' )
      {
         // QLIB_FTM_FM_RX_SET_STATION
         unsigned long iFreqkHz = 91100;
         bOk = QLIB_FTM_FM_RX_SET_STATION( g_hResourceContext, iFreqkHz );
         printf("\nQLIB_FTM_FM_RX_SET_STATION: %s", bOk ? "PASS" : "FAIL");
         printf("\n  FrequencyKHz: %d\n",  iFreqkHz);
      }
      else if (cKey == 'J' )
      {
         // Seek next station with QLIB_FTM_FM_RX_SEARCH_STATIONS

         FTM_FM_RxSearchMode_Enum       iSearchMode = FM_RX_SRCH_MODE_SEEK;
         FTM_FM_RxSearchDwellTime_Enum  iScanDwellTime = FM_RX_DWELL_PERIOD_0S;
         FTM_FM_RxSearchDir_Enum            iSearchDirection = FM_RX_SEARCHDIR_UP;
         bOk = QLIB_FTM_FM_RX_SEARCH_STATIONS( g_hResourceContext, (unsigned char) iSearchMode, (unsigned char) iScanDwellTime, (unsigned char) iSearchDirection);
         printf("\nQLIB_FTM_FM_RX_SEARCH_STATIONS: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iSearchMode: %d",  iSearchMode);
         printf("\n  iScanDwellTime: %d",  iScanDwellTime);
         printf("\n  iSearchDirection: %d\n",  iSearchDirection);
      }
      else if (cKey == 'K' )
      {
         // Scan all stations with QLIB_FTM_FM_RX_SEARCH_STATIONS

         FTM_FM_RxSearchMode_Enum       iSearchMode = FM_RX_SRCH_MODE_SCAN;
         FTM_FM_RxSearchDwellTime_Enum  iScanDwellTime = FM_RX_DWELL_PERIOD_7S;
         FTM_FM_RxSearchDir_Enum            iSearchDirection = FM_RX_SEARCHDIR_UP;
         bOk = QLIB_FTM_FM_RX_SEARCH_STATIONS( g_hResourceContext, (unsigned char) iSearchMode, (unsigned char) iScanDwellTime, (unsigned char) iSearchDirection);
         printf("\nQLIB_FTM_FM_RX_SEARCH_STATIONS: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iSearchMode: %d",  iSearchMode);
         printf("\n  iScanDwellTime: %d",  iScanDwellTime);
         printf("\n  iSearchDirection: %d\n",  iSearchDirection);
      }
      else if (cKey == 'L' )
      {
         // QLIB_FTM_FM_RX_CANCEL_SEARCH
         unsigned char iSearchMode = 0;     // seek next station
         unsigned char iScanDwellTime = 5;  // dwell 5 seconds per station found
         unsigned char iSearchDirection = 0; // increasing frequencies
         bOk = QLIB_FTM_FM_RX_CANCEL_SEARCH( g_hResourceContext );
         printf("\nQLIB_FTM_FM_RX_CANCEL_SEARCH: %s\n", bOk ? "PASS" : "FAIL");
      }
      else if (cKey == 'N' )
      {
         // QLIB_FTM_FM_RX_SET_SIGNAL_THRESHOLD

         //         unsigned char iEntry=0;
         unsigned short iEntry=0;
         do
         {
            printf("\nPlease enter a signal threshold(0 to 3): ");
            scanf("%hd", & iEntry );
         }
         while ((iEntry < 0) || (iEntry > 3));

         // iEntry is now in the proper range (0 to 3)
         FTM_FM_RxSignalThreshold_Enum iThreshold;
         iThreshold = (FTM_FM_RxSignalThreshold_Enum) iEntry;

         bOk = QLIB_FTM_FM_RX_SET_SIGNAL_THRESHOLD( g_hResourceContext, (unsigned char) iThreshold );
         printf("\nQLIB_FTM_FM_RX_SET_SIGNAL_THRESHOLD: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iThreshold: %d\n",  iThreshold);
      }
      else if (cKey == 'M' )
      {
         // QLIB_FTM_FM_RX_GET_SIGNAL_THRESHOLD

         unsigned char iThreshold=0;
         bOk = QLIB_FTM_FM_RX_GET_SIGNAL_THRESHOLD( g_hResourceContext, &iThreshold);
         printf("\nQLIB_FTM_FM_RX_GET_SIGNAL_THRESHOLD: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iThreshold: %d\n",  iThreshold);
      }
      else if (cKey == 'Z' )
      {
         // sweep frequencies and get station parameters
         unsigned long iFreqkHz;
         unsigned long iStationFreq=0;
         unsigned char iServAvble=0;
         unsigned char iRssi=0;
         unsigned char iStereoProgram=0;
         unsigned char iRdsSyncStatus=0;
         unsigned char iMuteMode=0;
         printf("\nFreq\tService\tRSSI\tStereo\tRDS\tMuteMode\n");

         for (iFreqkHz=87500; iFreqkHz < 108000; iFreqkHz += 200)
         {
            bOk = QLIB_FTM_FM_RX_SET_STATION( g_hResourceContext, iFreqkHz );
            Sleep(500);
            int GetParamsCount = 0;
            do
            {
               // Get station parameters
               // if RSSI==100 then requery station parameters
               bOk = QLIB_FTM_FM_RX_GET_STATION_PARAMETERS(g_hResourceContext,&iStationFreq,&iServAvble,&iRssi,&iStereoProgram,&iRdsSyncStatus,&iMuteMode );
               Sleep(1);
               GetParamsCount++;
            }
            while ((iRssi == 100) && (GetParamsCount < 200));
            printf("%d\t%d\t%d\t%d\t%d\t%d\n",iFreqkHz,iServAvble,iRssi,iStereoProgram,iRdsSyncStatus,iMuteMode);
         }
      }
      else if (cKey == 'X' )
      {
         // sweep frequencies and get station parameters
         unsigned long iFreqkHz;
         unsigned long iStationFreq=0;
         unsigned char iServAvble=0;
         unsigned char iRssi=0;
         unsigned char iStereoProgram=0;
         unsigned char iRdsSyncStatus=0;
         unsigned char iMuteMode=0;
         printf("\nFreq\tService\tRSSI\tStereo\tRDS\tMuteMode\n");

         for (iFreqkHz=87500; iFreqkHz < 108000; iFreqkHz += 200)
         {
            unsigned long iStartTime = GetTickCount();
            unsigned long iEndTime;
            unsigned long iElapsedTime_ms = 0;

            bOk = QLIB_FTM_FM_RX_SET_STATION( g_hResourceContext, iFreqkHz );
            Sleep(2000);
            int GetParamsCount = 0;
            do
            {
               // Get station parameters
               // if RSSI==100 then requery station parameters
               bOk = QLIB_FTM_FM_RX_GET_STATION_PARAMETERS(g_hResourceContext,&iStationFreq,&iServAvble,&iRssi,&iStereoProgram,&iRdsSyncStatus,&iMuteMode );
               Sleep(1);
               GetParamsCount++;
            }
            while ((iRssi == 100) && (GetParamsCount < 200));
            printf("%d\t%d\t%d\t%d\t%d\t%d\n",iFreqkHz,iServAvble,iRssi,iStereoProgram,iRdsSyncStatus,iMuteMode);

            // ensure that we are at each channel exactly 4 seconds
            // so that we can stay in sync with a sweeping signal generator
            do
            {
               Sleep(1);
               iEndTime = GetTickCount();
               iElapsedTime_ms = iEndTime - iStartTime;
            }
            while (iElapsedTime_ms < 4000);
         }
      }
      else if (cKey == 'C' )
      {
         // sweep frequencies and get station parameters
         unsigned long iFreqkHz;
         unsigned long iStationFreq=0;
         unsigned char iServAvble=0;
         unsigned char iRssi=0;
         unsigned char iStereoProgram=0;
         unsigned char iRdsSyncStatus=0;
         unsigned char iMuteMode=0;
         printf("\nFreq\tService\tRSSI\tStereo\tRDS\tMuteMode\n");

         for (iFreqkHz=76000; iFreqkHz <= 90000; iFreqkHz += 100)
         {
            bOk = QLIB_FTM_FM_RX_SET_STATION( g_hResourceContext, iFreqkHz );
            Sleep(500);
            int GetParamsCount = 0;
            do
            {
               // Get station parameters
               // if RSSI==100 then requery station parameters
               bOk = QLIB_FTM_FM_RX_GET_STATION_PARAMETERS(g_hResourceContext,&iStationFreq,&iServAvble,&iRssi,&iStereoProgram,&iRdsSyncStatus,&iMuteMode );
               Sleep(1);
               GetParamsCount++;
            }
            while ((iRssi == 100) && (GetParamsCount < 200));
            printf("%d\t%d\t%d\t%d\t%d\t%d\n",iFreqkHz,iServAvble,iRssi,iStereoProgram,iRdsSyncStatus,iMuteMode);
         }
      }
      else if (cKey == 'V' )
      {
         // sweep frequencies and get station parameters
         unsigned long iFreqkHz;
         unsigned long iStationFreq=0;
         unsigned char iServAvble=0;
         unsigned char iRssi=0;
         unsigned char iStereoProgram=0;
         unsigned char iRdsSyncStatus=0;
         unsigned char iMuteMode=0;
         printf("\nFreq\tService\tRSSI\tStereo\tRDS\tMuteMode\n");

         for (iFreqkHz=76000; iFreqkHz <= 90000; iFreqkHz += 100)
         {
            unsigned long iStartTime = GetTickCount();
            unsigned long iEndTime;
            unsigned long iElapsedTime_ms = 0;

            bOk = QLIB_FTM_FM_RX_SET_STATION( g_hResourceContext, iFreqkHz );
            Sleep(2000);
            int GetParamsCount = 0;
            do
            {
               // Get station parameters
               // if RSSI==100 then requery station parameters
               bOk = QLIB_FTM_FM_RX_GET_STATION_PARAMETERS(g_hResourceContext,&iStationFreq,&iServAvble,&iRssi,&iStereoProgram,&iRdsSyncStatus,&iMuteMode );
               Sleep(1);
               GetParamsCount++;
            }
            while ((iRssi == 100) && (GetParamsCount < 200));
            printf("%d\t%d\t%d\t%d\t%d\t%d\n",iFreqkHz,iServAvble,iRssi,iStereoProgram,iRdsSyncStatus,iMuteMode);

            // ensure that we are at each channel exactly 4 seconds
            // so that we can stay in sync with a sweeping signal generator
            do
            {
               Sleep(1);
               iEndTime = GetTickCount();
               iElapsedTime_ms = iEndTime - iStartTime;
            }
            while (iElapsedTime_ms < 4000);
         }
      }
      else if (cKey == ';' )
      {
         // QLIB_FTM_FM_RX_SET_HLSI

         unsigned short iEntry=0;
         do
         {
            printf("\n0=Auto, 1=Lowside, 2=Highside");
            printf("\nPlease enter a highside/lowside value (0 to 2): ");
            scanf("%hd", & iEntry );
         }
         while ((iEntry < 0) || (iEntry > 2));

         // iEntry is now in the proper range (0 to 2)
         FTM_FM_Hlsi_Enum iHlsi;
         iHlsi = (FTM_FM_Hlsi_Enum) iEntry;

         bOk = QLIB_FTM_FM_RX_SET_HLSI( g_hResourceContext, (unsigned char) iHlsi );
         printf("\nQLIB_FTM_FM_RX_SET_HLSI: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iHlsi: %d\n",  iHlsi);
      }
      else if (cKey == ':' )
      {
         // QLIB_FTM_FM_RX_ENABLE_NOTCH

         unsigned short iEntry=0;
         do
         {
            printf("\n0=Disable Notch, 1=Low Notch, 2=High Notch");
            printf("\nPlease enter a Notch filter setting (0 to 2): ");
            scanf("%hd", & iEntry );
         }
         while ((iEntry < 0) || (iEntry > 2));

         // iEntry is now in the proper range (0 to 2)
         FTM_FM_NotchFilter_Enum iNotch;
         iNotch = (FTM_FM_NotchFilter_Enum) iEntry;

         bOk = QLIB_FTM_FM_RX_ENABLE_NOTCH( g_hResourceContext, (unsigned char) iNotch );
         printf("\nQLIB_FTM_FM_RX_ENABLE_NOTCH: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iNotch: %d\n",  iNotch);
      }
      else if (cKey == '[' )
      {
         // QLIB_FTM_FM_RX_SET_ANTENNA (FM_ANTENNA_WIRED_HEADSET=0)

         FTM_FM_Antenna_Enum iAntenna = FM_ANTENNA_WIRED_HEADSET;
         bOk = QLIB_FTM_FM_RX_SET_ANTENNA( g_hResourceContext,(unsigned char) iAntenna );
         printf("\nQLIB_FTM_FM_RX_SET_ANTENNA: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iAntenna: %d\n",  iAntenna);
      }
      else if (cKey == ']' )
      {
         // QLIB_FTM_FM_RX_SET_ANTENNA (FM_ANTENNA_PWB=1)

         FTM_FM_Antenna_Enum iAntenna = FM_ANTENNA_PWB;
         bOk = QLIB_FTM_FM_RX_SET_ANTENNA( g_hResourceContext,(unsigned char) iAntenna );
         printf("\nQLIB_FTM_FM_RX_SET_ANTENNA: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iAntenna: %d\n",  iAntenna);
      }

      else if (cKey == '{' )
      {
         // QLIB_FTM_FM_RX_SET_SOFTMUTE (FM_RX_SOFTMUTE_DISABLED=0)

         FTM_FM_SoftMute_Enum iSoftMute = FM_RX_SOFTMUTE_DISABLED;
         bOk = QLIB_FTM_FM_RX_SET_SOFTMUTE( g_hResourceContext,(unsigned char) iSoftMute );
         printf("\nQLIB_FTM_FM_RX_SET_SOFTMUTE: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iSoftMute: %d\n",  iSoftMute);
      }
      else if (cKey == '}' )
      {
         // QLIB_FTM_FM_RX_SET_SOFTMUTE (FM_RX_SOFTMUTE_ENABLED=1)

         FTM_FM_SoftMute_Enum iSoftMute = FM_RX_SOFTMUTE_ENABLED;
         bOk = QLIB_FTM_FM_RX_SET_SOFTMUTE( g_hResourceContext,(unsigned char) iSoftMute );
         printf("\nQLIB_FTM_FM_RX_SET_SOFTMUTE: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iSoftMute: %d\n",  iSoftMute);
      }
      else if (cKey == '<' )
      {
         // QLIB_FTM_FM_RX_READ_RDS_GROUP_COUNTERS (with reset)

         unsigned char bResetCounters = 1;          // reset the counters
         unsigned long iTotalRdsBlockErrors = 0;
         unsigned long iTotalRdsGroups = 0;
         unsigned long iTotalRdsGroup0 = 0;
         unsigned long iTotalRdsGroup2 = 0;
         unsigned long iTotalRdsBlockB = 0;
         unsigned long iTotalRdsProcessedGroup0 = 0;
         unsigned long iTotalRdsProcessedGroup2 = 0;
         unsigned long iTotalRdsGroupFiltered = 0;
         unsigned long iTotalRdsChangeFiltered = 0;

         bOk = QLIB_FTM_FM_RX_READ_RDS_GROUP_COUNTERS(
                  g_hResourceContext,
                  bResetCounters,
                  &iTotalRdsBlockErrors,
                  &iTotalRdsGroups,
                  &iTotalRdsGroup0,
                  &iTotalRdsGroup2,
                  &iTotalRdsBlockB,
                  &iTotalRdsProcessedGroup0,
                  &iTotalRdsProcessedGroup2,
                  &iTotalRdsGroupFiltered,
                  &iTotalRdsChangeFiltered );

         printf("\nQLIB_FTM_FM_RX_READ_RDS_GROUP_COUNTERS: %s", bOk ? "PASS" : "FAIL");
         printf("\n  bResetCounters: %d",  bResetCounters);
         printf("\n  iTotalRdsBlockErrors: %d",  iTotalRdsBlockErrors);
         printf("\n  iTotalRdsGroups: %d",  iTotalRdsGroups);
         printf("\n  iTotalRdsGroup0: %d",  iTotalRdsGroup0);
         printf("\n  iTotalRdsGroup2: %d",  iTotalRdsGroup2);
         printf("\n  iTotalRdsBlockB: %d",  iTotalRdsBlockB);
         printf("\n  iTotalRdsProcessedGroup0: %d",  iTotalRdsProcessedGroup0);
         printf("\n  iTotalRdsProcessedGroup2: %d",  iTotalRdsProcessedGroup2);
         printf("\n  iTotalRdsGroupFiltered: %d",  iTotalRdsGroupFiltered);
         printf("\n  iTotalRdsChangeFiltered: %d\n",  iTotalRdsChangeFiltered);
      }
      else if (cKey == '>' )
      {
         // QLIB_FTM_FM_RX_READ_RDS_GROUP_COUNTERS (with reset)

         unsigned char bResetCounters = 0;          // NO reset of counters
         unsigned long iTotalRdsBlockErrors = 0;
         unsigned long iTotalRdsGroups = 0;
         unsigned long iTotalRdsGroup0 = 0;
         unsigned long iTotalRdsGroup2 = 0;
         unsigned long iTotalRdsBlockB = 0;
         unsigned long iTotalRdsProcessedGroup0 = 0;
         unsigned long iTotalRdsProcessedGroup2 = 0;
         unsigned long iTotalRdsGroupFiltered = 0;
         unsigned long iTotalRdsChangeFiltered = 0;

         bOk = QLIB_FTM_FM_RX_READ_RDS_GROUP_COUNTERS(
                  g_hResourceContext,
                  bResetCounters,
                  &iTotalRdsBlockErrors,
                  &iTotalRdsGroups,
                  &iTotalRdsGroup0,
                  &iTotalRdsGroup2,
                  &iTotalRdsBlockB,
                  &iTotalRdsProcessedGroup0,
                  &iTotalRdsProcessedGroup2,
                  &iTotalRdsGroupFiltered,
                  &iTotalRdsChangeFiltered );

         printf("\nQLIB_FTM_FM_RX_READ_RDS_GROUP_COUNTERS: %s", bOk ? "PASS" : "FAIL");
         printf("\n  bResetCounters: %d",  bResetCounters);
         printf("\n  iTotalRdsBlockErrors: %d",  iTotalRdsBlockErrors);
         printf("\n  iTotalRdsGroups: %d",  iTotalRdsGroups);
         printf("\n  iTotalRdsGroup0: %d",  iTotalRdsGroup0);
         printf("\n  iTotalRdsGroup2: %d",  iTotalRdsGroup2);
         printf("\n  iTotalRdsBlockB: %d",  iTotalRdsBlockB);
         printf("\n  iTotalRdsProcessedGroup0: %d",  iTotalRdsProcessedGroup0);
         printf("\n  iTotalRdsProcessedGroup2: %d",  iTotalRdsProcessedGroup2);
         printf("\n  iTotalRdsGroupFiltered: %d",  iTotalRdsGroupFiltered);
         printf("\n  iTotalRdsChangeFiltered: %d\n",  iTotalRdsChangeFiltered);
      }
      else if (cKey == '?' )
      {
         // QLIB_FTM_FM_RX_GET_RDS_PS

         unsigned char iLength=0;
         const int iMaxLength=108;
         char sPS[iMaxLength];
         memset(sPS, 0, iMaxLength );
         bOk = QLIB_FTM_FM_RX_GET_RDS_PS( g_hResourceContext, &iLength, sPS);
         printf("\nQLIB_FTM_FM_RX_GET_RDS_PS: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iLength: %d",  iLength);
         printf("\n  sPS: %s\n",  sPS);
      }
      else if (cKey == '/' )
      {
         // QLIB_FTM_FM_RX_GET_RDS_RT

         unsigned char iLength=0;
         const int iMaxLength=64;
         char sRT[iMaxLength];
         memset(sRT, 0, iMaxLength );
         bOk = QLIB_FTM_FM_RX_GET_RDS_RT( g_hResourceContext, &iLength, sRT);
         printf("\nQLIB_FTM_FM_RX_GET_RDS_RT: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iLength: %d",  iLength);
         printf("\n  sRT: \"%s\"\n",  sRT);
      }
      else if (cKey == '+' )
      {
         // QLIB_FTM_FM_RX_Get_Scaled_Rssi

         int iScaledRssi=0;

         bOk = QLIB_FTM_FM_RX_Get_Scaled_Rssi( g_hResourceContext, &iScaledRssi );

         printf("\nQLIB_FTM_FM_RX_Get_Scaled_Rssi: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iScaledRssi: %d dBm\n",  iScaledRssi);
      }
      else if (cKey == 'P' )
      {
         // QLIB_FTM_FM_GET_PIN_CONNECTIVITY

         unsigned short iBtTestResult = 0xFFFF;
         unsigned short iFmTestResult = 0xFFFF;

         bOk = QLIB_FTM_FM_GET_PIN_CONNECTIVITY( g_hResourceContext, &iBtTestResult, &iFmTestResult );
         printf("\nQLIB_FTM_FM_GET_PIN_CONNECTIVITY: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iBtTestResult: 0x%X",  iBtTestResult);
         printf("\n  iFmTestResult: 0x%X\n",  iFmTestResult);
      }

      // return if 'Q' key, otherwise stay in the FM RX menu
      if (cKey == 'Q' )
      {
         break;
      }
   } // end while

   return true;
}

unsigned char Test_FTM_FM_TX( void )
{
   while(1)
   {
      unsigned char bOk=true;
      unsigned char cKey;

      printf("\n----------------\nFM TX FTM Test\n----------------\n");

      printf("\n ! = Enable Transmitter (FM_JAPAN_WIDE_BAND=2, FM_DE_EMP75=0, FM_CHSPACE_200_KHZ=0, FM_RDS_STD_RBDS=0, 0, 0)");
      printf("\n 1 = Disable Transmitter");
      printf("\n 2 = Configure Transmitter (FM_US_EUROPE_BAND=0, FM_DE_EMP75=0, FM_CHSPACE_200_KHZ=0, FM_RDS_STD_NONE=2, 0, 0)");
      printf("\n 3 = Configure Transmitter (FM_US_EUROPE_BAND=0, FM_DE_EMP75=0, FM_CHSPACE_200_KHZ=0, FM_RDS_STD_RBDS=0, 0, 0)");
      printf("\n 4 = Configure Transmitter (FM_US_EUROPE_BAND=0, FM_DE_EMP50=1, FM_CHSPACE_100_KHZ=1, FM_RDS_STD_RDS=1,  0, 0)");
      printf("\n 5 = Configure Transmitter (FM_JAPAN_STANDARD_BAND=1, FM_DE_EMP75=0, FM_CHSPACE_100_KHZ=1, FM_RDS_STD_NONE=2, 0, 0)");
      printf("\n 6 = Set Station ( 92500 KHz)");
      printf("\n 7 = Set Station (107900 KHz)");
      printf("\n 8 = Set TX Internal Tone Generator");
      printf("\n Q = Quit\n\n");

      cKey = toupper(getch());

      if (cKey == '!' )
      {
         // QLIB_FTM_FM_TX_ENABLE_TRANSMITTER

         FTM_FM_BandLimits_Enum iRadioBand  = FM_JAPAN_WIDE_BAND;
         FTM_FM_Emphasis_Enum   iEmphasis   = FM_DE_EMP75;
         FTM_FM_ChanSpacing_Enum iChSpacing = FM_CHSPACE_200_KHZ;
         FTM_FM_RdsStd_Enum     iRdsStd     = FM_RDS_STD_RBDS;
         unsigned long iUserDefinedBandFreqMin = 0;
         unsigned long iUserDefinedBandFreqMax = 0;

         bOk = QLIB_FTM_FM_TX_ENABLE_TRANSMITTER( g_hResourceContext,
                                                  (unsigned char) iRadioBand,
                                                  (unsigned char) iEmphasis,
                                                  (unsigned char) iChSpacing,
                                                  (unsigned char) iRdsStd,
                                                  iUserDefinedBandFreqMin,
                                                  iUserDefinedBandFreqMax );

         printf("\nQLIB_FTM_FM_TX_ENABLE_TRANSMITTER: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Radio Band: %d", iRadioBand );
         printf("\n  Emphasis: %d", iEmphasis );
         printf("\n  Ch Spacing: %d", iChSpacing );
         printf("\n  Rds Std: %d", iRdsStd );
         printf("\n  User Defined Band Freq Min: %d", iUserDefinedBandFreqMin );
         printf("\n  User Defined Band Freq Max: %d\n", iUserDefinedBandFreqMax );
      }
      else if (cKey == '1' )
      {
         // QLIB_FTM_FM_TX_DISABLE_TRANSMITTER
         bOk = QLIB_FTM_FM_TX_DISABLE_TRANSMITTER(g_hResourceContext);
         printf("\nQLIB_FTM_FM_TX_DISABLE_TRANSMITTER: %s\n", bOk ? "PASS" : "FAIL" );
      }
      else if (cKey == '2' )
      {
         // QLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER

         FTM_FM_BandLimits_Enum iRadioBand  = FM_US_EUROPE_BAND;
         FTM_FM_Emphasis_Enum   iEmphasis   = FM_DE_EMP75;
         FTM_FM_ChanSpacing_Enum iChSpacing = FM_CHSPACE_200_KHZ;
         FTM_FM_RdsStd_Enum     iRdsStd     = FM_RDS_STD_NONE;
         unsigned long iUserDefinedBandFreqMin = 0;
         unsigned long iUserDefinedBandFreqMax = 0;

         bOk = QLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER(    g_hResourceContext,
                                                        (unsigned char) iRadioBand,
                                                        (unsigned char) iEmphasis,
                                                        (unsigned char) iChSpacing,
                                                        (unsigned char) iRdsStd,
                                                        iUserDefinedBandFreqMin,
                                                        iUserDefinedBandFreqMax );

         printf("\nQLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Radio Band: %d", iRadioBand );
         printf("\n  Emphasis: %d", iEmphasis );
         printf("\n  Ch Spacing: %d", iChSpacing );
         printf("\n  Rds Std: %d", iRdsStd );
         printf("\n  User Defined Band Freq Min: %d", iUserDefinedBandFreqMin );
         printf("\n  User Defined Band Freq Max: %d\n", iUserDefinedBandFreqMax );
      }
      else if (cKey == '3' )
      {
         // QLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER

         FTM_FM_BandLimits_Enum iRadioBand  = FM_US_EUROPE_BAND;
         FTM_FM_Emphasis_Enum   iEmphasis   = FM_DE_EMP75;
         FTM_FM_ChanSpacing_Enum iChSpacing = FM_CHSPACE_200_KHZ;
         FTM_FM_RdsStd_Enum     iRdsStd     = FM_RDS_STD_RBDS;
         unsigned long iUserDefinedBandFreqMin = 0;
         unsigned long iUserDefinedBandFreqMax = 0;

         bOk = QLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER(    g_hResourceContext,
                                                        (unsigned char) iRadioBand,
                                                        (unsigned char) iEmphasis,
                                                        (unsigned char) iChSpacing,
                                                        (unsigned char) iRdsStd,
                                                        iUserDefinedBandFreqMin,
                                                        iUserDefinedBandFreqMax );

         printf("\nQLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Radio Band: %d", iRadioBand );
         printf("\n  Emphasis: %d", iEmphasis );
         printf("\n  Ch Spacing: %d", iChSpacing );
         printf("\n  Rds Std: %d", iRdsStd );
         printf("\n  User Defined Band Freq Min: %d", iUserDefinedBandFreqMin );
         printf("\n  User Defined Band Freq Max: %d\n", iUserDefinedBandFreqMax );
      }
      else if (cKey == '4' )
      {
         // QLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER

         FTM_FM_BandLimits_Enum iRadioBand  = FM_US_EUROPE_BAND;
         FTM_FM_Emphasis_Enum   iEmphasis   = FM_DE_EMP50;
         FTM_FM_ChanSpacing_Enum iChSpacing = FM_CHSPACE_100_KHZ;
         FTM_FM_RdsStd_Enum     iRdsStd     = FM_RDS_STD_RDS;
         unsigned long iUserDefinedBandFreqMin = 0;
         unsigned long iUserDefinedBandFreqMax = 0;

         bOk = QLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER(    g_hResourceContext,
                                                        (unsigned char) iRadioBand,
                                                        (unsigned char) iEmphasis,
                                                        (unsigned char) iChSpacing,
                                                        (unsigned char) iRdsStd,
                                                        iUserDefinedBandFreqMin,
                                                        iUserDefinedBandFreqMax );

         printf("\nQLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Radio Band: %d", iRadioBand );
         printf("\n  Emphasis: %d", iEmphasis );
         printf("\n  Ch Spacing: %d", iChSpacing );
         printf("\n  Rds Std: %d", iRdsStd );
         printf("\n  User Defined Band Freq Min: %d", iUserDefinedBandFreqMin );
         printf("\n  User Defined Band Freq Max: %d\n", iUserDefinedBandFreqMax );
      }
      else if (cKey == '5' )
      {
         // QLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER

         FTM_FM_BandLimits_Enum iRadioBand  = FM_JAPAN_STANDARD_BAND;
         FTM_FM_Emphasis_Enum   iEmphasis   = FM_DE_EMP75;
         FTM_FM_ChanSpacing_Enum iChSpacing = FM_CHSPACE_100_KHZ;
         FTM_FM_RdsStd_Enum     iRdsStd     = FM_RDS_STD_NONE;
         unsigned long iUserDefinedBandFreqMin = 0;
         unsigned long iUserDefinedBandFreqMax = 0;

         bOk = QLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER(    g_hResourceContext,
                                                        (unsigned char) iRadioBand,
                                                        (unsigned char) iEmphasis,
                                                        (unsigned char) iChSpacing,
                                                        (unsigned char) iRdsStd,
                                                        iUserDefinedBandFreqMin,
                                                        iUserDefinedBandFreqMax );

         printf("\nQLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Radio Band: %d", iRadioBand );
         printf("\n  Emphasis: %d", iEmphasis );
         printf("\n  Ch Spacing: %d", iChSpacing );
         printf("\n  Rds Std: %d", iRdsStd );
         printf("\n  User Defined Band Freq Min: %d", iUserDefinedBandFreqMin );
         printf("\n  User Defined Band Freq Max: %d\n", iUserDefinedBandFreqMax );
      }
      else if (cKey == '6' )
      {
         // QLIB_FTM_FM_TX_SET_STATION
         unsigned long iFreqkHz = 92500;
         bOk = QLIB_FTM_FM_TX_SET_STATION( g_hResourceContext, iFreqkHz );
         printf("\nQLIB_FTM_FM_TX_SET_STATION: %s", bOk ? "PASS" : "FAIL");
         printf("\n  FrequencyKHz: %d\n",  iFreqkHz);
      }
      else if (cKey == '7' )
      {
         // QLIB_FTM_FM_TX_SET_STATION
         unsigned long iFreqkHz = 107900;
         bOk = QLIB_FTM_FM_TX_SET_STATION( g_hResourceContext, iFreqkHz );
         printf("\nQLIB_FTM_FM_TX_SET_STATION: %s", bOk ? "PASS" : "FAIL");
         printf("\n  FrequencyKHz: %d\n",  iFreqkHz);
      }
      else if (cKey == '8' )
      {
         // QLIB_FTM_FM_TX_SET_INTERNAL_TONE

         unsigned short iEntry=0;
         do
         {
            printf("\n0 =  No audio signal");
            printf("\n1 =  1 kHz tone @ 0 dBFS with Left=Right");
            printf("\n2 =  1 kHz tone @ 0 dBFS,  Left channel only");
            printf("\n3 =  1 kHz tone @ 0 dBFS, Right channel only");
            printf("\n4 =  1 kHz tone @ -18 dBFS with Left=Right");
            printf("\n5 = 15 kHz tone @ -18 dBFS with Left=Right");
            printf("\nPlease enter a TX tone setting (0 to 5): ");
            scanf("%hd", & iEntry );
         }
         while ((iEntry < 0) || (iEntry > 5));

         // iEntry is now in the proper range (0 to 5)
         FTM_FM_TxAudioTone_Enum iTone;
         iTone = (FTM_FM_TxAudioTone_Enum) iEntry;

         bOk = QLIB_FTM_FM_TX_SET_INTERNAL_TONE( g_hResourceContext, (unsigned char) iTone );
         printf("\nQLIB_FTM_FM_TX_SET_INTERNAL_TONE: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iTone: %d\n",  iTone);
      }

      // return if 'Q' key, otherwise stay in the FM TX menu
      if (cKey == 'Q' )
      {
         break;
      }
   } // end while

   return true;
}

unsigned char Test_FTM_FM_AF( void )
{
   while(1)
   {
      unsigned char bOk=true;
      unsigned char cKey;

      printf("\n----------------\nFM Alternate Frequency FTM Test\n----------------\n");

      printf("\n 1 = Set AF Threshold");
      printf("\n 2 = Get AF Threshold");
      printf("\n 3 = Set RSSI Check Timer");
      printf("\n 4 = Get RSSI Check Timer");
      printf("\n 5 = Set RDS PI Timer");
      printf("\n 6 = Get RDS PI Timer");
      printf("\n 7 = Enable/Disable RDS AF Jump (with FTM_FM_RX_GROUP_PROC_OPTIONS)");
      printf("\n Q = Quit\n\n");

      cKey = toupper(getch());

      if (cKey == '1' )
      {
         // QLIB_FTM_FM_RX_SET_AF_THRESHOLD

         short   iAfThresholdLog=0;
         printf("\nPlease enter an AF threshold value (-101 to -70): ");
         scanf("%hd", & iAfThresholdLog );
         bOk = QLIB_FTM_FM_RX_SET_AF_THRESHOLD( g_hResourceContext,iAfThresholdLog );
         printf("\nQLIB_FTM_FM_RX_SET_AF_THRESHOLD: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iAfThresholdLog: %d\n",  iAfThresholdLog);
      }
      else if (cKey == '2' )
      {
         // QLIB_FTM_FM_RX_GET_AF_THRESHOLD

         short   iAfThresholdLog=0;
         bOk = QLIB_FTM_FM_RX_GET_AF_THRESHOLD( g_hResourceContext, &iAfThresholdLog);
         printf("\nQLIB_FTM_FM_RX_GET_AF_THRESHOLD: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iAfThresholdLog: %d\n",  iAfThresholdLog);
      }
      else if (cKey == '3' )
      {
         // QLIB_FTM_FM_RX_SET_RSSI_CHECK_TIMER

         unsigned char iChCondTimer=0;
         printf("\nPlease enter a RSSI Check Timer value (0 to 255, @5 ms per count): ");
         scanf("%hd", & iChCondTimer );
         bOk = QLIB_FTM_FM_RX_SET_RSSI_CHECK_TIMER( g_hResourceContext,iChCondTimer );
         printf("\nQLIB_FTM_FM_RX_SET_RSSI_CHECK_TIMER: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iChCondTimer: %d\n",  iChCondTimer);
      }
      else if (cKey == '4' )
      {
         // QLIB_FTM_FM_RX_GET_RSSI_CHECK_TIMER

         unsigned char iChCondTimer=0;
         bOk = QLIB_FTM_FM_RX_GET_RSSI_CHECK_TIMER( g_hResourceContext, &iChCondTimer);
         printf("\nQLIB_FTM_FM_RX_GET_RSSI_CHECK_TIMER: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iChCondTimer: %d\n",  iChCondTimer);
      }
      else if (cKey == '5' )
      {
         // QLIB_FTM_FM_RX_SET_RDS_PI_TIMER

         unsigned char iRdsTimeout=0;
         printf("\nPlease enter a RDS PI Timer value (0 to 255, @5 ms per count): ");
         scanf("%hd", & iRdsTimeout );
         bOk = QLIB_FTM_FM_RX_SET_RDS_PI_TIMER( g_hResourceContext,iRdsTimeout );
         printf("\nQLIB_FTM_FM_RX_SET_RDS_PI_TIMER: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iRdsTimeout: %d\n",  iRdsTimeout);
      }
      else if (cKey == '6' )
      {
         // QLIB_FTM_FM_RX_GET_RDS_PI_TIMER

         unsigned char iRdsTimeout=0;
         bOk = QLIB_FTM_FM_RX_GET_RDS_PI_TIMER( g_hResourceContext, &iRdsTimeout);
         printf("\nQLIB_FTM_FM_RX_GET_RDS_PI_TIMER: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iRdsTimeout: %d\n",  iRdsTimeout);
      }
      else if (cKey == '7' )
      {
         // Enable/Disable RDS AF Jump with QLIB_FTM_FM_RX_RDS_GROUP_PROC_OPTIONS

         // Enable RDS/RBDS Radio Text processing?
         unsigned long iRT=0;
         printf("\nEnable RDS/RBDS Radio Text processing? (0/1): ");
         scanf("%hd", & iRT );

         // Enable RDS/RBDS Program Service processing?
         unsigned long iPS=0;
         printf("Enable RDS/RBDS Program Service processing? (0/1): ");
         scanf("%hd", & iPS );

         // Enable RDS/RBDS SimpleProgram Service processing?
         unsigned long iSimplePS=0;
         printf("Enable RDS/RBDS Simple Program Service processing? (0/1): ");
         scanf("%hd", & iSimplePS );

         // Enable RDS/RBDS Alternative Frequency processing?
         unsigned long iAF=0;
         printf("Enable RDS/RBDS Alternative Frequency processing? (0/1): ");
         scanf("%hd", & iAF );

         // Enable RDS/RBDS Alternative Frequency Auto Jump?
         unsigned long iAfAutoJump=0;
         printf("Enable RDS/RBDS Alternative Frequency Auto Jump? (0/1): ");
         scanf("%hd", & iAfAutoJump );

         // build the parameter and send the cmd
         unsigned long iRdsProcOptions = 0;
         iRdsProcOptions = iRT + (iPS << 1) + (iSimplePS << 2) + (iAF << 3) + (iAfAutoJump << 4);

         bOk = QLIB_FTM_FM_RX_RDS_GROUP_PROC_OPTIONS( g_hResourceContext,iRdsProcOptions );
         printf("\nQLIB_FTM_FM_RX_RDS_GROUP_PROC_OPTIONS: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iRdsProcOptions: %d\n",  iRdsProcOptions);
      }

      // return if 'Q' key, otherwise stay in the FM Misc menu
      if (cKey == 'Q' )
      {
         break;
      }
   } // end while

   return true;
}

unsigned char Test_FTM_FM_Audio( void )
{
   while(1)
   {
      unsigned char bOk=true;
      unsigned char cKey;

      printf("\n----------------\nFM Audio FTM Test\n----------------\n");

      printf("\n 0 = Set FM Audio Route (FM_API_AUDIO_OFF = 0)");
      printf("\n 1 = Set FM Audio Route (FM_API_AUDIO_OUT_ANALOG = 1)");
      printf("\n 2 = Set FM Audio Route (FM_API_AUDIO_OUT_DIGITAL_M = 2)");
      printf("\n 3 = Set FM Audio Route (FM_API_AUDIO_OUT_DIGITAL_S = 3)");
      printf("\n 4 = Set FM Audio Route (FM_API_AUDIO_OUT_ANALOG_DIGITAL_M = 4)");
      printf("\n 5 = Set FM Audio Route (FM_API_AUDIO_OUT_ANALOG_DIGITAL_S = 5)");
      printf("\n 6 = Set FM Audio Route (FM_API_AUDIO_IN_DIGITAL_M = 6)");
      printf("\n 7 = Set FM Audio Route (FM_API_AUDIO_IN_DIGITAL_S = 7)");
      printf("\n E = Enable WCNSS Audio Log Packets");
      printf("\n R = Disable WCNSS Audio Log Packets");
      printf("\n A = WCNSS Audio Capture (  125 ms)");
      printf("\n S = WCNSS Audio Capture (  250 ms)");
      printf("\n D = WCNSS Audio Capture (  500 ms)");
      printf("\n F = WCNSS Audio Capture ( 1000 ms)");
      printf("\n G = WCNSS Audio Capture ( 2000 ms)");
      printf("\n H = WCNSS Audio Capture ( 4000 ms)");
      printf("\n J = WCNSS Audio Capture ( 6000 ms)");
      printf("\n K = WCNSS Audio Capture ( 8000 ms)");
      printf("\n L = WCNSS Audio Capture (10000 ms)");
      printf("\n X = All FM setup and capture audio immediately after bootup  (enable wcnss audio logs, enable fm rx, tune station, get params, get rssi, capture audio)");

      printf("\n V = Enable FM Audio (FM_RX_AUDIO_TO_HEADSET = 0)");
      printf("\n B = Enable FM Audio (FM_RX_AUDIO_TO_SPEAKER = 1)");
      printf("\n N = Disable FM Audio");
      printf("\n M = Set FM Audio Volume");

      printf("\n Q = Quit\n\n");

      cKey = toupper(getch());
      int iDuration_ms = 1000;

      if (cKey == 'X' )
      {
         //
         // This sequence is to test the bringup sequence as fast as possible
         //   USE THIS IMMEDIATELY AFTER POWERUP
         //
         // 1. Enable WCNSS Audio log packets
         // 2. Enable FM RX
         // 3. Tune station
         // 4. Get station parameters to verify that station is being received
         // 5. Get scaled RSSI
         // 6. Capture Fm RX audio
         //

         // QLIB_FTM_LOG_StartWCNSSAudio_Log( 0x1558 )
         unsigned short iLogCode = 0x1558;
         bOk = QLIB_FTM_LOG_StartWCNSSAudio_Log( g_hResourceContext );
         printf("\nQLIB_FTM_LOG_StartWCNSSAudio_Log: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iLogCode: %X\n",  iLogCode);

         // QLIB_FTM_FM_RX_ENABLE_RECEIVER

         FTM_FM_BandLimits_Enum iRadioBand  = FM_US_EUROPE_BAND;
         FTM_FM_Emphasis_Enum   iEmphasis   = FM_DE_EMP75;
         FTM_FM_ChanSpacing_Enum iChSpacing = FM_CHSPACE_200_KHZ;
         FTM_FM_RdsStd_Enum     iRdsStd     = FM_RDS_STD_RBDS;
         unsigned long iUserDefinedBandFreqMin = 88100;
         unsigned long iUserDefinedBandFreqMax = 107900;

         bOk = QLIB_FTM_FM_RX_ENABLE_RECEIVER(g_hResourceContext,
                                              (unsigned char) iRadioBand,
                                              (unsigned char) iEmphasis,
                                              (unsigned char) iChSpacing,
                                              (unsigned char) iRdsStd,
                                              iUserDefinedBandFreqMin,
                                              iUserDefinedBandFreqMax );

         printf("\nQLIB_FTM_FM_RX_ENABLE_RECEIVER: %s", bOk ? "PASS" : "FAIL" );
         printf("\n  Radio Band: %d", iRadioBand );
         printf("\n  Emphasis: %d", iEmphasis );
         printf("\n  Ch Spacing: %d", iChSpacing );
         printf("\n  Rds Std: %d", iRdsStd );
         printf("\n  User Defined Band Freq Min: %d", iUserDefinedBandFreqMin );
         printf("\n  User Defined Band Freq Max: %d\n", iUserDefinedBandFreqMax );

         Sleep(6000);

         // QLIB_FTM_FM_RX_SET_STATION

         unsigned long iFreqkHz = 94100;
         bOk = QLIB_FTM_FM_RX_SET_STATION( g_hResourceContext, iFreqkHz );
         printf("\nQLIB_FTM_FM_RX_SET_STATION: %s", bOk ? "PASS" : "FAIL");
         printf("\n  FrequencyKHz: %d\n",  iFreqkHz);

         Sleep(500);

         // QLIB_FTM_FM_RX_GET_STATION_PARAMETERS

         unsigned long iStationFreq=0;
         unsigned char iServAvble=0;
         unsigned char iRssi=0;
         unsigned char iStereoProgram=0;
         unsigned char iRdsSyncStatus=0;
         unsigned char iMuteMode=0;

         bOk = QLIB_FTM_FM_RX_GET_STATION_PARAMETERS( g_hResourceContext,
                                                      &iStationFreq,
                                                      &iServAvble,
                                                      &iRssi,
                                                      &iStereoProgram,
                                                      &iRdsSyncStatus,
                                                      &iMuteMode );

         printf("\nQLIB_FTM_FM_RX_GET_STATION_PARAMETERS: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iStationFreq: %d",  iStationFreq);
         printf("\n  iServAvble: %d",  iServAvble);
         printf("\n  iRssi: %d",  iRssi);
         printf("\n  iStereoProgram: %d",  iStereoProgram);
         printf("\n  iRdsSyncStatus: %d",  iRdsSyncStatus);
         printf("\n  iMuteMode: %d\n",  iMuteMode);

         // QLIB_FTM_FM_RX_Get_Scaled_Rssi

         int iScaledRssi=0;

         bOk = QLIB_FTM_FM_RX_Get_Scaled_Rssi( g_hResourceContext, &iScaledRssi );

         printf("\nQLIB_FTM_FM_RX_Get_Scaled_Rssi: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iScaledRssi: %d dBm\n",  iScaledRssi);

         // QLIB_WCNSS_Audio_Capture_FM_RX

         iDuration_ms = 4000;
         char sFilename[80] = "";
         int iCreateDirResult = CreateDirectory("C:\\fm_audio", NULL);
         _snprintf(sFilename, sizeof(sFilename), "C:\\fm_audio\\wcnss_audio_%dms.wav", iDuration_ms);
         int iMissedPackets = 0;
         int iTotalPackets = 0;

         printf("\nQLIB_WCNSS_Audio_Capture_FM_RX (%d ms) started...", iDuration_ms);
         bOk = QLIB_WCNSS_Audio_Capture_FM_RX( g_hResourceContext, sFilename, iDuration_ms, &iMissedPackets, &iTotalPackets);
         printf("\nQLIB_WCNSS_Audio_Capture_FM_RX: %s", bOk ? "PASS" : "FAIL");
         printf("\n  sFilename: %s",  sFilename);
         printf("\n  iDuration_ms: %d",  iDuration_ms);
         printf("\n  iMissedPackets: %d",  iMissedPackets);
         printf("\n  iTotalPackets: %d\n",  iTotalPackets);
      }

      if (cKey == 'E' )
      {
         // QLIB_FTM_LOG_StartWCNSSAudio_Log( 0x1558 )

         unsigned short iLogCode = 0x1558;
         bOk = QLIB_FTM_LOG_StartWCNSSAudio_Log( g_hResourceContext );
         printf("\nQLIB_FTM_LOG_StartWCNSSAudio_Log: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iLogCode: 0x%X\n",  iLogCode);
      }
      else if (cKey == 'R' )
      {
         // QLIB_FTM_LOG_StopWCNSSAudio_Log( 0x1558 )

         unsigned short iLogCode = 0x1558;
         bOk = QLIB_FTM_LOG_StopWCNSSAudio_Log( g_hResourceContext );
         printf("\nQLIB_FTM_LOG_StopWCNSSAudio_Log: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iLogCode: 0x%X\n",  iLogCode);
      }

      if (cKey == 'A' )
      {
         // QLIB_WCNSS_Audio_Capture_FM_RX (125 ms)
         iDuration_ms = 125;
      }
      else if (cKey == 'S' )
      {
         // QLIB_WCNSS_Audio_Capture_FM_RX (250 ms)
         iDuration_ms = 250;
      }
      else if (cKey == 'D' )
      {
         // QLIB_WCNSS_Audio_Capture_FM_RX (500 ms)
         iDuration_ms = 500;
      }
      else if (cKey == 'F' )
      {
         // QLIB_WCNSS_Audio_Capture_FM_RX (1000 ms)
         iDuration_ms = 1000;
      }
      else if (cKey == 'G' )
      {
         // QLIB_WCNSS_Audio_Capture_FM_RX (2000 ms)
         iDuration_ms = 2000;
      }
      else if (cKey == 'H' )
      {
         // QLIB_WCNSS_Audio_Capture_FM_RX (4000 ms)
         iDuration_ms = 4000;
      }
      else if (cKey == 'J' )
      {
         // QLIB_WCNSS_Audio_Capture_FM_RX (6000 ms)
         iDuration_ms = 6000;
      }
      else if (cKey == 'K' )
      {
         // QLIB_WCNSS_Audio_Capture_FM_RX (8000 ms)
         iDuration_ms = 8000;
      }
      else if (cKey == 'L' )
      {
         // QLIB_WCNSS_Audio_Capture_FM_RX (10000 ms)
         iDuration_ms = 10000;
      }

      if (   (cKey == 'A') || (cKey == 'S') || (cKey == 'D')
             || (cKey == 'F') || (cKey == 'G') || (cKey == 'H')
             || (cKey == 'J') || (cKey == 'K') || (cKey == 'L'))
      {
         // QLIB_WCNSS_Audio_Capture_FM_RX

         char sFilename[80] = "";
         int iCreateDirResult = CreateDirectory("C:\\fm_audio", NULL);
         _snprintf(sFilename, sizeof(sFilename), "C:\\fm_audio\\wcnss_audio_%dms.wav", iDuration_ms);
         int iMissedPackets = 0;
         int iTotalPackets = 0;

         printf("\nQLIB_WCNSS_Audio_Capture_FM_RX (%d ms) started...", iDuration_ms);
         bOk = QLIB_WCNSS_Audio_Capture_FM_RX( g_hResourceContext, sFilename, iDuration_ms, &iMissedPackets, &iTotalPackets);
         printf("\nQLIB_WCNSS_Audio_Capture_FM_RX: %s", bOk ? "PASS" : "FAIL");
         printf("\n  sFilename: %s",  sFilename);
         printf("\n  iDuration_ms: %d",  iDuration_ms);
         printf("\n  iMissedPackets: %d",  iMissedPackets);
         printf("\n  iTotalPackets: %d\n",  iTotalPackets);
      }

      if (cKey == '0' )
      {
         // QLIB_FTM_FM_SET_ROUTE_AUDIO

         FTM_FM_AudioRoute_Enum iFmAudioRoute = FM_API_AUDIO_OFF;
         bOk = QLIB_FTM_FM_SET_ROUTE_AUDIO( g_hResourceContext,(unsigned char) iFmAudioRoute );
         printf("\nQLIB_FTM_FM_SET_ROUTE_AUDIO: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iFmAudioRoute: %d\n",  iFmAudioRoute);
      }
      else if (cKey == '1' )
      {
         // QLIB_FTM_FM_SET_ROUTE_AUDIO

         FTM_FM_AudioRoute_Enum iFmAudioRoute = FM_API_AUDIO_OUT_ANALOG;
         bOk = QLIB_FTM_FM_SET_ROUTE_AUDIO( g_hResourceContext,(unsigned char) iFmAudioRoute );
         printf("\nQLIB_FTM_FM_SET_ROUTE_AUDIO: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iFmAudioRoute: %d\n",  iFmAudioRoute);
      }
      else if (cKey == '2' )
      {
         // QLIB_FTM_FM_SET_ROUTE_AUDIO

         FTM_FM_AudioRoute_Enum iFmAudioRoute = FM_API_AUDIO_OUT_DIGITAL_M;
         bOk = QLIB_FTM_FM_SET_ROUTE_AUDIO( g_hResourceContext,(unsigned char) iFmAudioRoute );
         printf("\nQLIB_FTM_FM_SET_ROUTE_AUDIO: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iFmAudioRoute: %d\n",  iFmAudioRoute);
      }
      else if (cKey == '3' )
      {
         // QLIB_FTM_FM_SET_ROUTE_AUDIO

         FTM_FM_AudioRoute_Enum iFmAudioRoute = FM_API_AUDIO_OUT_DIGITAL_S;
         bOk = QLIB_FTM_FM_SET_ROUTE_AUDIO( g_hResourceContext,(unsigned char) iFmAudioRoute );
         printf("\nQLIB_FTM_FM_SET_ROUTE_AUDIO: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iFmAudioRoute: %d\n",  iFmAudioRoute);
      }
      else if (cKey == '4' )
      {
         // QLIB_FTM_FM_SET_ROUTE_AUDIO

         FTM_FM_AudioRoute_Enum iFmAudioRoute = FM_API_AUDIO_OUT_ANALOG_DIGITAL_M;
         bOk = QLIB_FTM_FM_SET_ROUTE_AUDIO( g_hResourceContext,(unsigned char) iFmAudioRoute );
         printf("\nQLIB_FTM_FM_SET_ROUTE_AUDIO: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iFmAudioRoute: %d\n",  iFmAudioRoute);
      }
      else if (cKey == '5' )
      {
         // QLIB_FTM_FM_SET_ROUTE_AUDIO

         FTM_FM_AudioRoute_Enum iFmAudioRoute = FM_API_AUDIO_OUT_ANALOG_DIGITAL_S;
         bOk = QLIB_FTM_FM_SET_ROUTE_AUDIO( g_hResourceContext,(unsigned char) iFmAudioRoute );
         printf("\nQLIB_FTM_FM_SET_ROUTE_AUDIO: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iFmAudioRoute: %d\n",  iFmAudioRoute);
      }
      else if (cKey == '6' )
      {
         // QLIB_FTM_FM_SET_ROUTE_AUDIO

         FTM_FM_AudioRoute_Enum iFmAudioRoute = FM_API_AUDIO_IN_DIGITAL_M;
         bOk = QLIB_FTM_FM_SET_ROUTE_AUDIO( g_hResourceContext,(unsigned char) iFmAudioRoute );
         printf("\nQLIB_FTM_FM_SET_ROUTE_AUDIO: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iFmAudioRoute: %d\n",  iFmAudioRoute);
      }
      else if (cKey == '7' )
      {
         // QLIB_FTM_FM_SET_ROUTE_AUDIO

         FTM_FM_AudioRoute_Enum iFmAudioRoute = FM_API_AUDIO_IN_DIGITAL_S;
         bOk = QLIB_FTM_FM_SET_ROUTE_AUDIO( g_hResourceContext,(unsigned char) iFmAudioRoute );
         printf("\nQLIB_FTM_FM_SET_ROUTE_AUDIO: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iFmAudioRoute: %d\n",  iFmAudioRoute);
      }
      else if (cKey == 'V' )
      {
         // QLIB_FTM_FM_RX_ENABLE_AUDIO

         FTM_FM_Audio_Destiniation_Enum iAudioDestination = FM_RX_AUDIO_TO_HEADSET;
         bOk = QLIB_FTM_FM_RX_ENABLE_AUDIO ( g_hResourceContext, (unsigned char) iAudioDestination);
         printf("\nQLIB_FTM_FM_RX_ENABLE_AUDIO: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iAudioDestination: %d\n",  iAudioDestination);
      }
      else if (cKey == 'B' )
      {
         // QLIB_FTM_FM_RX_ENABLE_AUDIO

         FTM_FM_Audio_Destiniation_Enum iAudioDestination = FM_RX_AUDIO_TO_SPEAKER;
         bOk = QLIB_FTM_FM_RX_ENABLE_AUDIO ( g_hResourceContext, (unsigned char) iAudioDestination);
         printf("\nQLIB_FTM_FM_RX_ENABLE_AUDIO: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iAudioDestination: %d\n",  iAudioDestination);
      }
      else if (cKey == 'N' )
      {
         // QLIB_FTM_FM_RX_DISABLE_AUDIO

         bOk = QLIB_FTM_FM_RX_DISABLE_AUDIO ( g_hResourceContext );
         printf("\nQLIB_FTM_FM_RX_DISABLE_AUDIO: %s\n", bOk ? "PASS" : "FAIL");
      }
      else if (cKey == 'M' )
      {
         // QLIB_FTM_FM_RX_SET_AUDIO_VOLUME

         // get volume setting
         unsigned short buf_input = 0;
         do
         {
            printf("\nPlease enter a volume setting (0 to 255 decimal): ");
            scanf("%hd", & buf_input );
         }
         while ((buf_input < 0) || (buf_input > 255));
         unsigned char iVolume = (unsigned char) buf_input;

         bOk = QLIB_FTM_FM_RX_SET_AUDIO_VOLUME ( g_hResourceContext, iVolume);
         printf("\nQLIB_FTM_FM_RX_SET_AUDIO_VOLUME: %s", bOk ? "PASS" : "FAIL");
         printf("\n  iVolume: %d\n",  iVolume);
      }

      // return if 'Q' key, otherwise stay in the FM Misc menu
      if (cKey == 'Q' )
      {
         break;
      }
   } // end while

   return true;
}

unsigned char Test_NV_Tool( void )
{
   unsigned char bOk = false;

   int iNumOfNVItemValuesLoaded = 0;
   int iResultCode = 0;
   int iNumNVDefLoaded = 0;

   printf("\nTest_NV_Tool()");

   /* -- this is not needed becuase I disabled the checking inside the NV Tool
   bOk = QLIB_ExtendedTextMessage_SetCategoryEnable(
   g_hResourceContext,
   QMSL_ExtTextMsgCat_NV_Tool_Debug,
   TRUE
   );
   */
   bOk = QLIB_DIAG_CONTROL_F(g_hResourceContext, MODE_FTM_F );
   printf("\nQLIB_DIAG_CONTROL_F = %d", bOk);

   bOk = QLIB_NV_ClearNVValues(g_hResourceContext );
   printf("\nQLIB_NV_ClearNVValues = %d", bOk);

   bOk = QLIB_NV_LoadNVDefinitionFile(
            g_hResourceContext,
            "C:\\NvDefinition.xml",
            &iNumNVDefLoaded,
            &iResultCode
         );
   printf("\nQLIB_NV_LoadNVDefinitionFile = %d, iNumNVDefLoaded = %d, iResultCode = %d",
          bOk,
          iNumNVDefLoaded,
          iResultCode);

   bOk = QLIB_NV_LoadNVsFromQCN(
            g_hResourceContext,
            "C:\\defaultQCN.qcn",
            &iNumOfNVItemValuesLoaded,
            &iResultCode );

   printf(  "\nQLIB_NV_LoadNVsFromQCN = %d, iResultCode = %d",
            bOk,
            iResultCode
         );

   QLIB_NV_WriteNVsToMobile(
      g_hResourceContext,
      &iResultCode);
   printf(  "\nQLIB_NV_WriteNVsToMobile = %d, iResultCode = %d",
            bOk,
            iResultCode
         );

   if ( g_hResourceContext )
   {
      for ( int _iIndex = 0; _iIndex < 4; _iIndex++ )
      {
         QLIB_DisconnectServer(g_hResourceContext );
         g_hResourceContext = QLIB_ConnectServer( 5 );

         bOk = QLIB_NV_ClearNVValues(g_hResourceContext );
         printf("\nQLIB_NV_ClearNVValues = %d", bOk);
         bOk = QLIB_NV_LoadNVsFromQCN(
                  g_hResourceContext,
                  "C:\\defaultQCN.qcn",
                  &iNumOfNVItemValuesLoaded,
                  &iResultCode );

         printf(    "\nQLIB_NV_LoadNVsFromQCN = %d, iResultCode = %d",
                    bOk,
                    iResultCode
               );

         QLIB_NV_WriteNVsToMobile(
            g_hResourceContext,
            &iResultCode);
         printf(    "\nQLIB_NV_WriteNVsToMobile = %d, iResultCode = %d",
                    bOk,
                    iResultCode
               );

         QLIB_NV_WriteNVsToMobile(
            g_hResourceContext,
            &iResultCode);
         printf(    "\nQLIB_NV_WriteNVsToMobile = %d, iResultCode = %d",
                    bOk,
                    iResultCode
               );
      } // for ( int _iIndex = 0; _iIndex < 100; _iIndex++ )
   }    // if ( g_hResourceContext )

   return bOk;
}



unsigned char Test_FTM_ANT()
{
   unsigned char bOk=true;
   unsigned char cKey;

   printf("----------------\nANT FTM Test\n----------------\n");

   if ( bOk )
   {
      printf("\n\n1=Start receiver test");
      printf("\n2=Start transmitter test");
      printf("\n3=Reset running test");
      printf("\n4=Get receiver test status / result");
      printf("\n5=Get transmitter test status / result");

      cKey = getch();

      if (cKey == '1' )
      {
         unsigned char _aiDataPayload[8] = {0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0};
         bOk = QLIB_FTM_ANT_Receiver_Test( g_hResourceContext, 6, 250, 100, 55, 11, 208, _aiDataPayload );
         printf("\nQLIB_FTM_ANT_Receiver_Test: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == 1
      else if (cKey == '2' )
      {
         unsigned char _aiDataPayload[8] = {0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0};
         bOk = QLIB_FTM_ANT_Transmitter_Test( g_hResourceContext, 6, 250, 100, 55, 11, 208, _aiDataPayload );
         printf("\nQLIB_FTM_ANT_Transmitter_Test: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == 2
      else if (cKey == '3' )
      {
         bOk = QLIB_FTM_ANT_Reset_Test( g_hResourceContext );
         printf("\nQLIB_FTM_ANT_Reset_Test: %s", bOk ? "PASS" : "FAIL" );
      }// cKey == 3
      else if (cKey == '4' )
      {
         unsigned short _iPacketsReceived, _iPacketsLost, _iPacketsCRCError, _iNoOfSyncLosses;
         unsigned long _iTotalBitErrors;
         unsigned char _iStatus;
         bOk = QLIB_FTM_ANT_Receiver_Test_Result_Status( g_hResourceContext, &_iPacketsReceived, &_iPacketsLost, &_iPacketsCRCError, &_iTotalBitErrors, &_iNoOfSyncLosses, &_iStatus );
         printf("\nQLIB_FTM_ANT_Receiver_Test_Result_Status: %s", bOk ? "PASS" : "FAIL" );
         if (bOk)
         {
            // Print the results
            printf("\n      Packets Received: %d", _iPacketsReceived                        );
            printf("\n          Packets Lost: %d", _iPacketsLost                            );
            printf("\nPackets with CRC error: %d", _iPacketsCRCError                        );
            printf("\n      Total bit errors: %d", _iTotalBitErrors                         );
            printf("\n    No. of sync losses: %d", _iNoOfSyncLosses                         );
            printf("\n           Test Status: %s", (_iStatus == 0) ? "Completed" : "Running");
         }
      }// cKey == 4
      else if (cKey == '5' )
      {
         unsigned short _iPacketsTransmitted;
         unsigned char _iStatus;
         bOk = QLIB_FTM_ANT_Transmitter_Test_Result_Status( g_hResourceContext, &_iPacketsTransmitted, &_iStatus );
         printf("\nQLIB_FTM_ANT_Transmitter_Test_Result_Status: %s", bOk ? "PASS" : "FAIL" );
         if (bOk)
         {
            // Print the results
            printf("\nPackets Transmitted: %d", _iPacketsTransmitted                     );
            printf("\n        Test Status: %s", (_iStatus == 0) ? "Completed" : "Running");
         }
      }// cKey == 5
   }

   return bOk;
}
unsigned char Test_RF_Verification_Menu ()
{
   unsigned char bMainLoop = true;
   do // bMainLoop
   {
      // Print options
      printf("\nRF Verification Demo Menu ======================================\n\n");
      printf("------------------------------------------------------------\n");
      printf(" 0 = FTM RF WCDMA Tx verification\n");
      printf(" 1 = FTM RF WCDMA Rx verification\n");
      printf(" 2 = WCDMA BER\n");
      printf(" 3 = GSM BER\n");
      printf(" 4 = EGPRS BER\n");
      printf(" 5 = HSDPA BER\n");
      printf(" 6 = C2K BER\n");
      printf(" 7 = 1xEVDO BER\n");
      printf(" 8 = LTE BER\n");
      printf(" 9 = VFS\n");
	  printf(" A = Test_LTE_FTM_RF_Tx\n");
	  printf(" B = Test_LTE_FTM_RF_Rx\n");
	  printf(" C = Test_GSM_FTM_RF_Tx\n");
	  printf(" D = Test_GSM_FTM_RF_Rx\n");
	  printf(" E = ARA\n");
	  printf(" F = Qlink test\n");
	  printf("\n Q = QUIT. Back to the main menu\n");
      printf("------------------------------------------------------------\n");

      printf("\n\n Please enter the key (0 - 9 / A - E  or Q)\n");

      // Get a key
      char cKey = toupper( _getch() );

      switch (cKey)
      {
         case '0':
            Test_WCDMA_FTM_RF_Tx();
            printf("WCDMA FTM RF verification completed. Press any key.");
            toupper( getch() );
            break;
         case '1':
            Test_WCDMA_FTM_RF_Rx();
            printf("WCDMA FTM RF verification completed. Press any key.");
            toupper( getch() );
            break;
         case '2':
            Test_WCDMA_BER();
            break;
         case '3':
            Test_GSM_BER();
            break;
         case '4':
            Test_EGPRS_BER();
            break;
         case '5':
            Test_HSDPA_BER();
            break;
         case '6':
            Test_FTM_cdma2k_NON_SIGNALING( );
            break;
         case '7':
            Test_FTM_1XEVDO_NON_SIGNALING( );
            break;
         case '8':
            Test_LTE_NS();
            break;
         case '9':
            Test_RF_Verification();
            break;
         case 'A':
            Test_LTE_FTM_RF_Tx();
            printf("LTE FTM RF verification completed. Press any key.");
            toupper( getch() );
            break;
         case 'B':
            Test_LTE_FTM_RF_Rx();
            printf("LTE FTM RF verification completed. Press any key.");
            toupper( getch() );
            break;
         case 'C':
            Test_GSM_FTM_RF_Tx();
            printf("GSM FTM RF verification completed. Press any key.");
            toupper( getch() );
            break;
         case 'D':
            Test_GSM_FTM_RF_Rx();
            printf("GSM FTM RF verification completed. Press any key.");
            toupper( getch() );
		 case 'E':
            Test_ARA();
            printf("ARA completed. Press any key.");
            toupper( getch() );
		 case 'F':
            Test_QLINK();
            printf("Test_QLINK completed. Press any key.");
            toupper( getch() );	
         case 'q':
         case 'Q':
            printf("\n Go back to main menu");
            return true;
            break;
         default:
            break;
      }
   }
   while (bMainLoop);
   return true;
}
unsigned char Test_RF_Calibration (void)
{
   unsigned char bMainLoop = true;

   do // bMainLoop
   {
      // Print options
      printf("\nRF Calibration Demo Menu ======================================\n\n");
      printf("------------------------------------------------------------\n");
      printf(" 0 = ESC ET (V4) Cal\n");
      printf(" 1 = GSM V4 Cal\n");
      printf(" 2 = FBRx DC cal (Enh Internal Device Cal)\n");
      printf(" 3 = Rx RSB cal (Enh Internal Device Cal)\n");
      printf(" 4 = GSM DA Char NV writing\n");
      printf(" 5 = GSM V3 Tx Cal\n");
      printf(" 6 = GSM Char Rx Cal\n");
      printf(" 7 = XPT Capture\n");
      printf(" 8 = LTE B29 SCELL Cal (V2 Cal)\n");
      printf(" 9 = Internal Device Cal (QLIB_FTM_DO_INTERNAL_DEVICE_CAL)\n");
      printf(" A = Tx_LO_RSB Cal\n");
      printf(" B = ESC V3 WCDMA Cal\n");
      printf(" C = ESC V3 LTE Cal\n");
      printf(" D = ESC V3 CDMA Cal\n");
      printf(" E = ESC V3 TDSCDMA Cal\n");
      printf(" F = V2 Calibration\n");
      printf(" G = FTM diversity calibration, 6500\n");
      printf(" H = RF Test WCDMA\n");
      printf(" I = RF Test GSM\n");
      printf(" J = RF Test C2K\n");
      printf(" K = WCDMA Dual Carrier and WCDMA ICI Cal Test\n");
      printf(" L = GSM IIP2 Cal\n");
      printf(" M = GSM Rx Cal\n");
      printf(" N = FB Droop Cal\n");
      printf(" O = Internal Device Cal (QLIB_FTM_DO_SELF_CAL)\n");
      printf(" P = QSC Cal (ESC V5 Cal)(for Atlas with QSPCI callbox)\n");
      printf(" R = GSM V2 Tx Cal (for Atlas with QSCPI callbox)\n");
      printf(" S = GSM V2 Rx Cal (for Atlas with QSCPI callbox)\n");
      printf(" T = GSM FBRx Cal (for Atlas with QSCPI callbox)\n");

      printf("\n Q = QUIT. Back to the main menu\n");
      printf("------------------------------------------------------------\n");

      printf("\n\n Please enter the key (0 - 9 / A - N  or Q)\n");

      // Get a key
      char cKey = toupper( _getch() );
      unsigned char _bOK = true;
      switch (cKey)
      {
         case '0':
            Test_RFCal_ESC_ET();
            break;
         case '1':
            _bOK = Test_GSM_V4_Cal();
            break;
         case '2':
            _bOK = Test_FBDC_Cal_Enh_Internal_Device_Cal();
            break;
         case '3':
            _bOK = Test_RxRSB_Cal_Enh_Internal_Device_Cal();
            break;
         case '4':
            _bOK = Test_GSMV4_WriteCharNV();
            break;
         case '5':
            _bOK = Test_GSM_V3_Cal();
            break;
         case '6':
            _bOK = Test_GSM_V3_Rx_Cal();
            break;
         case '7':
            _bOK = Test_XPT_Capture_Sweep();
            break;
         case '8':
            _bOK = testLTECA_B29SCELLCal();
            break;
         case '9':
         {
            unsigned short iFTM_error_code = 0;
            _bOK = QLIB_FTM_DO_INTERNAL_DEVICE_CAL( g_hResourceContext, &iFTM_error_code );
            printf("\nQLIB_FTM_DO_INTERNAL_DEVICE_CAL( g_hResourceContext, &iFTM_error_code );\n");
         }
         break;
         case 'A':
         case 'a':
            _bOK = Test_RFCal_TxRsb_TxLo_Internal_Cal();
            break;
         case 'B':
         case 'b':
            _bOK = Test_RFCal_V3();//WCDMA
            break;
         case 'C':
         case 'c':
            _bOK = Test_RFCal_V3_LTE();//Test_RFCal_V3_LTE
            break;
         case 'D':
         case 'd':
            _bOK = Test_RFCal_V3_CDMA();//Test_RFCal_V3_CDMA
            break;
         case 'E':
         case 'e':
            _bOK = Test_RFCal_V3_TDSCDMA();// Test_RFcal_V3_TDSCDMA
            break;
         case 'F':
         case 'f':
            _bOK = Test_FTM_CAL_API_V2 ();
            break;
         case 'G':
         case 'g':
            _bOK = Test_FTM_DIVERSITY_CAL( );
            break;
         case 'H':
         case 'h':
            _bOK = Test_FTM_WCDMA( 9739 );
            break;
         case 'I':
         case 'i':
            _bOK = Test_FTM_GSM( );
            break;
         case 'J':
         case 'j':
            _bOK = Test_FTM_cdma2000();
            break;
         case 'K':
         case 'k':
            _bOK = WCDMA_Daul_Carrier_and_ICI_Cal( );
            break;
         case 'L':
         case 'l':
             _bOK = Test_GSM_IP2_Cal();
             break;
         case 'M':
         case 'm':
             _bOK = Test_GSM_Rx_Cal();
             break;
         case 'N':
         case 'n':
             _bOK = Test_FB_Droop_Cal_Enh_Internal_Device_Cal();
             break;
         case 'O':
         case 'o':
          _bOK = Test_FTM_DO_SELF_CAL();
             break;
         case 'P':
         case 'p':
          _bOK =  RFCal::QSC::QSC_CAL(g_hResourceContext);
             break;
         case 'r':
         case 'R':
          _bOK =  RFCal::GSM_Cal::DO_GSM_V2_TX_Cal(g_hResourceContext);
             break;
         case 's':
         case 'S':
          _bOK =  RFCal::GSM_Cal::DO_GSM_V2_RX_Cal(g_hResourceContext);
             break;
         case 't':
         case 'T':
          _bOK =  RFCal::GSM_Cal::DO_GSM_FBRX_CAL(g_hResourceContext);
             break;

       case 'q':
         case 'Q':
            printf("\n Go back to main menu");
            return true;
            break;
         default:
            break;
      }
      if (cKey != '0')
      {
         if (_bOK)
            printf("Success.\n");
         else
            printf("Fail.\n");

         printf("\n\nFinished.\nPlease enter any key...\n");

         // Get a key
         toupper( _getch() );
      }
   }
   
   while (bMainLoop);

   return true;
}


