/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_GPIB/QMSL_GPIB_MSVC7/QMSL_GPIB_Demo.cpp#5 $
 * $DateTime: 2016/04/04 17:51:51 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2014-2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#include "stdafx.h"
#include <conio.h>

//#include "QMSL_GPIB.h"
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
*/

// Include NI GPIB header. You must setup the MSVC include and library paths to the NI files.
// usually under C:\Program Files\National Instruments\Shared\ExternalCompilerSupport\C
#include "decl-32.h"

// Include QMSL 
#include "qlib.h"
#include "qlib_defines.h"

#include <stdio.h>
#include <stdlib.h>

#include "QSEQ.h"

// Test Equipment Setup 
int g_iInstPrimaryAddress = 5;  // Instrument GPIB address
int g_iInstSecondaryAddress = 0;  // Instrument GPIB address
int g_hInst = 0;          // Instrument handle

// QMSL Set up
int g_iUseQPST = 1; //1 to use QMSL in QPST mode
HANDLE g_hQMSL = 0x0;
int g_iCOMport = -1;  // COM PORT can be QLIB_COM_AUTO_DETECT
char* g_sLogFile = "C:\\QMSL_GPIB_Log.txt";

#define LIST_SIZE 5


static unsigned int channels[LIST_SIZE]; // = {387, 387, 387, 387, 387};
static double expectedPower_dBm[] = {33.0, 30.0, 26.0, 23.0, 20.0};
double uplinkFreqs[50] = {0};
double downlinkFreqs[50] = {0};

// buffer
char sBuffer[1000];

void WriteToEquip(char *sStr);
void ReadFromEquipment(char *sStr, long iSizeOfStr);

// The function set up CMW500 for triggering and measurment
void Cmw1CdmaMelmAnalyzerConfigure();
void CalcFreqsFromChannels(unsigned int channel);
void ArmCMW500Trigger();
void WaitForMeasurement();

void ParseStringToDouble(char* string, double *doubleArray, int& sizeOfArray);

unsigned char Test_FTM_Sequencer( );
unsigned char Reserved();

// Write a NULL terminated string to equipment
void WriteToEquip(char *sStr)
{
   ASSERT(g_hInst);
   ibwrt(g_hInst, sStr, (long)strlen(sStr));
}

void ReadFromEquipment(char *sStr, long iSizeOfStr)
{
   ASSERT(g_hInst);
   char *pBuffer = new char[iSizeOfStr];
   memset(pBuffer, 0, iSizeOfStr);
   ibrd(g_hInst, pBuffer, iSizeOfStr);
   memcpy(sStr, pBuffer, iSizeOfStr);
   delete [] pBuffer;
}

////////////////////////////////////////////////////////////

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
   if ( QLIB_FTM_GET_COMMAND_CODE( g_hQMSL, &_iCommandCode  ) )
   {
      // This command only works for command mode 75
      if ( _iCommandCode == 75 )
      {
         if ( QLIB_GetPhoneOperatingMode( g_hQMSL, &_iPhoneOperatingMode ) )
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

         }   // if ( QLIB_GetPhoneOperatingMode( g_hQMSL, &_iPhoneOperatingMode ) )

         if ( QLIB_GetPhoneCallState(g_hQMSL, &_iCallState, &_iSystemMode) )
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
      }   // if ( _iCommandCode == 75 )

   }   // if ( QLIB_FTM_GET_COMMAND_CODE( g_hQMSL, _iCommandCode  ) )


   unsigned char _iFTM_Mode = 0;
   unsigned short _iStatus = 0;

   // NV ID, Data buffer, size, status
   _bOk = QLIB_DIAG_NV_READ_F(  g_hQMSL, 453, &_iFTM_Mode, 1, &_iStatus );

   if (_bOk )
   {
      printf("\n                FTM Mode: %s", _iFTM_Mode ? "ON" : "OFF" );
   }
   else
   {
      printf("\nQLIB_DIAG_NV_READ_F(NV_FTM_MODE_I): FAIL");
   }   // if (_bOk)


   // Determine the QPST phone status
   unsigned long _ePhoneStatus;
   const int c_iNumStatus = 12;
   _bOk = QLIB_GetQpstPhoneStatus( g_hQMSL, &_ePhoneStatus );
   if (   (_bOk )
      &&   ( _ePhoneStatus <= c_iNumStatus )
      )
   {
      // String list of status
      char* _asSWD_StatusStrings[ c_iNumStatus ] =
      {
         { "SWD_modeNone"               },
         { "SWD_modeDataServices"         },
         { "SWD_modeDownload"            },
         { "SWD_modeDiagnostic"            },
         { "SWD_modeDiagnosticOffline"      },
         { "SWD_modeReset"               },
         { "SWD_modeStreamDownload"         },
         { "SWD_modeMspDownload"            },
         { "SWD_modeMspDiagnostic"         },
         { "SWD_modeMspDiagnosticOffline"   },
         { "SWD_modeMspReset"            },
         { "SWD_modeMspStreamDownload"      }
      };

      printf("\n\nQPST Phone Status: %s", _asSWD_StatusStrings[ _ePhoneStatus ] );

   }


   return (_bOk );

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

   unsigned char _bDownload = false;   // Indicates download mode
   unsigned char _bDiag = false;      // Indicates diagnostic mode6

   char sLibraryVersion[50];
   // Get the library version
   QLIB_GetLibraryVersion(sLibraryVersion);
   printf("\nQLIB DLL Version: %s", sLibraryVersion );


   // Print the COM port number
   unsigned short _iComPort = -1;
   QLIB_GetComPortNumber( g_hQMSL, &_iComPort );

   // If still auto detect, then try to find the COM port
   if ( _iComPort == QLIB_COM_AUTO_DETECT )
   {
      printf("\nAuto detect selected, no phone has been found, searching...\n");
      // Auto was selected, but a phone has not been detected
      g_hQMSL  = QLIB_ConnectServer( _iComPort );

   }   // if ( _iComPort == QLIB_COM_AUTO_DETECT )


   // Get the COM port number
   QLIB_GetComPortNumber( g_hQMSL, &_iComPort );
   if ( _iComPort == QLIB_COM_AUTO_DETECT )
   {
      printf("\nCOM Port: AUTO, NOT DETECTED\n"  );
      return;
   }

   printf("\nCOM Port: %d\n", _iComPort );

   _bDiag=QLIB_IsPhoneConnected( g_hQMSL );


   if (!_bDiag )
   {
      // Clear the serial Rx buffer-- don't check return value because it is not enabled for QPST version
      QLIB_FlushRxBuffer( g_hQMSL  );

      printf("<<Waiting for phone, press any key to exit>>\n");

      while ( !_bDownload && !_bDiag && !kbhit() )
      {
         _bDiag = QLIB_IsPhoneConnected( g_hQMSL );

         // Disabled for now because pinging the phone in download mode isn't working.  Need further research
         // _bDownload = QLIB_DOWNLOAD_Ping(  g_hQMSL );

         // Now try the QPST SW Download
         unsigned long _ePhoneMode;
         bool _bQpstPhoneStatusOk = QLIB_GetQpstPhoneStatus( g_hQMSL, &_ePhoneMode ) == TRUE;

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
      if (QLIB_IsPhoneConnected( g_hQMSL ) )
      {
         // Force the phone into ONLINE mode
         // Go into FTM mode
         if ( !QLIB_DIAG_VERNO_F( g_hQMSL,
            comp_date,   comp_time,   rel_date,   rel_time, ver_dir,
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
            printf("Release Date: %s\n", rel_date      );
            printf("Release Time: %s\n", rel_time      );
            printf(" Version Dir: %s\n", ver_dir      );

            printf("\n\n");

            printf("   Station Class Mark: %d\n", scm            );
            printf("              CAI Rev: %d\n", mob_cai_rev      );
            printf("         Mobile Model: %d\n", mob_model         );
            printf("         Firmware Rev: %d\n", mob_firm_rev      );
            printf("     Slot Cycle Index: %d\n", slot_cycle_index   );
            printf("Vocoder major version: %d\n", voc_maj         );
            printf("Vocoder minor version: %d\n", voc_min         );

            // Get extended version info
            unsigned long _iMSM_HW_Version = 0;
            unsigned long _iMobModel = 0;
            char _sMobSwRev[512];
            char _sModelStr[512];


            bOk = QLIB_DIAG_EXT_BUILD_ID_F( g_hQMSL, &_iMSM_HW_Version, &_iMobModel, _sMobSwRev, _sModelStr );

            // Print the results
            if (bOk)
            {
               printf("\n");
               printf("      Ext MSM HW Version: 0x%X\n", _iMSM_HW_Version   );
               printf("       Ext Mobile Model#: 0x%X\n", _iMobModel      );
               printf("Ext Mobile Mobile SW Rev: %s\n", _sMobSwRev      );
               printf("            Ext Model ID: %s\n", _sModelStr      );

               // Report the phone status
               ReportPhoneState();
            }
            else
               printf("\nQLIB_DIAG_EXT_BUILD_ID_F = FAIL");

            printf("\n");


         }   // !QLIB_DIAG_VERNO_F
      }
      else
      {
         printf("\n Phone is not connected.");
      }   // if (QLIB_IsPhoneConnected() )
   } // if (_bDiag )
   else if ( _bDownload)
   {
      printf("\n Phone is in download mode.");

      ReportPhoneState();
   }
}

// Function to cycle through technology non-signaling modes
void TechnologyNS_Cycle(void)
{
   unsigned short _iCount = 0;
   unsigned char bOk = TRUE;
   unsigned long iBand_Class = FTM_EVDO_BAND_CLASS_CELLULAR;
   unsigned short iCDMA_Freq = 1013;

   char _sTextLogFile[] = "QMSL_NS_Stress_Test.txt";
   QLIB_StartLogging( g_hQMSL, _sTextLogFile);       
   printf("\nText log: %s", _sTextLogFile );

   char _sDLF_LogFile[] = "QMSL_NS_Stress_Test.dlf";
   QLIB_StartDLF_Logging( g_hQMSL,  _sDLF_LogFile, false);
   printf("\nDLF log: %s", _sDLF_LogFile );

   // Setup to log all FTM messages
   bOk &= QLIB_DIAG_SetMessageFilter( g_hQMSL, (short) 23, 0xFF );
   bOk &= QLIB_DIAG_SetMessageFilter( g_hQMSL, (short) 4001, 0xFF );

   // Enable all phone logging
   bOk &= QLIB_DIAG_SetPhoneLoggingState( g_hQMSL, true );
   
   printf("\nbOK after setup: %s", bOk ? "PASS" : "FAIL" );

   
   
   for ( _iCount=0; _iCount < 100; _iCount++)
   {

      printf("\n Loop count: %d", _iCount );

      /* set to Online mode */
      // Test_NV_FTM_Mode( 0 );
      bOk = QLIB_DIAG_CONTROL_F( g_hQMSL, MODE_ONLINE_F );
      printf("\nQLIB_DIAG_CONTROL_F(  MODE_ONLINE_F ): %s", bOk ? "PASS" : "FAIL\n" );

      printf("\n\nPower cycling device now..");
      Sleep( 2000 );
      WriteToEquip("OUTP OFF;");
      Sleep( 500 );

      //!!! debug start
      WriteToEquip( "*OPC?" );
      char buffer[512];
      ReadFromEquipment(buffer, 512);
      //!!! debug end

      WriteToEquip("OUTP ON;");
      Sleep( 5000 );

      TestVersion();

      // Go back into FTM mode 
      bOk = QLIB_DIAG_CONTROL_F( g_hQMSL, MODE_FTM_F );
      printf("\nQLIB_DIAG_CONTROL_F(  MODE_FTM_F ): %s", bOk ? "PASS" : "FAIL\n" );
      Sleep( 2000 );

      // Stop on error
      if (!bOk) break;

      // Start the CDMA2000 NS subystem, this will activate log codes that are needed for
      // sychronization, status, and statistics. If this is not done, the CDMA2000 NS commands
      // will operate in a request/response mode only, and status & statistics will not be updated.
      //

      unsigned long iACQ_Mode = FTM_CDMA2000_NS_FTM_ACQ_MODE_FULL;

      bOk = QLIB_FTM_CDMA2000_PILOT_ACQ(
                  g_hQMSL,
                  iBand_Class,
                  iCDMA_Freq,
                  iACQ_Mode);

      printf("\nQLIB_FTM_CDMA2000_PILOT_ACQ: %s", bOk ? "PASS" : "FAIL\n" );
      if (!bOk) break;
      Sleep(100); 

      bOk = QLIB_FTM_CDMA2000_DEMOD_SYNC(g_hQMSL);

      printf("\nQLIB_FTM_CDMA2000_DEMOD_SYNC: %s", bOk ? "PASS" : "FAIL\n" );
      if (!bOk) break;
      Sleep(200); 

      bOk = QLIB_FTM_CDMA2000_CMD_RELEASE( g_hQMSL );
      printf("\nQLIB_FTM_CDMA2000_CMD_RELEASE: %s", bOk ? "PASS" : "FAIL\n" );
      if (!bOk) break;   

      Sleep(2000); 

      /////////////////////////////////////////
      // Request call start, with session init
      /////////////////////////////////////////
      bOk = QLIB_FTM_GSM_BER_StartSession(
         g_hQMSL,
         1000 );

      Sleep(200);

      printf("\n  QLIB_FTM_GSM_BER_StartSession: %s", bOk ? "Pass" : "FAIL" );
      if (!bOk) break;

      // GSM Start/Stop stress test
      
         bOk =QLIB_FTM_GSM_BER_StopSession(g_hQMSL );
         printf("\n  QLIB_FTM_GSM_BER_StopSession: %s", bOk ? "Pass" : "FAIL" );
      if (!bOk) break;
      Sleep(200); 

      if (kbhit()) break;

      // Stop on error
      if (!bOk) break;

            
   }   // for ( _iCount=0; _iCount < 10; _iCount++)

   QLIB_StopLogging( g_hQMSL );       
   QLIB_StopDLF_Logging(g_hQMSL );

}


// Function to profile power supply on and mobile diagnostic connection 
void PowerSuppyMobilePowerUpTimeProfile( void )
{
   // Number of loop iterations
   unsigned long _iMaxLoopIterations = 1;

   // Sum of all iterations
   unsigned long _iElapsedTime_Total_ms = 0;

   // Number of completed iterations
   unsigned long _iNumCompletedIterations = 0;

   // OK flag
   unsigned char _bOk = true;

   // Disconnect the QMSL handle. If QPST is being used then some other QPST application needs to be
   // started so that QPST Server does not quit. Otherwise the profile times will include QPST startup
   QLIB_DisconnectServer( g_hQMSL );

   // To start a log file automatically the file "QMSL_Auto_Start_Log.txt" must be in the working directory.
   // The file QMSL_Log.txt will be created automatically when conneting

   printf("\nHow many iterations? ");
   scanf("%d", &_iMaxLoopIterations );
   
   printf("\nStarting power supply control loop...");

   while (      (_iNumCompletedIterations < _iMaxLoopIterations )
         &&   ( _bOk )
         )
   {
      
      //
      // Power Supply OFF then ON
      //
      WriteToEquip("OUTP OFF");
      WriteToEquip("OUTP ON");

      // Time for the current iteration
      unsigned long _iStartTime = GetTickCount();

      // Get a new mobile handle. This is done instead of using the old handle because QPST will
      g_hQMSL  = QLIB_ConnectServerWithWait( g_iCOMport , 30000 );

      // Elapsed time
      unsigned long _iElapsedTime_ms = GetTickCount() - _iStartTime;
   
      g_hQMSL  = QLIB_ConnectServerWithWait( g_iCOMport , 30000 );

      if ( g_hQMSL )
      {
         // QLIB_AddUserMessageToTextLog( g_hQMSL , LOG_FN, "Phone detected", false );

         QLIB_DisconnectServer( g_hQMSL  );
      }

      //
      // Power Supply OFF
      //
      WriteToEquip("OUTP OFF");


      // Increment number of completed iterations
      _iNumCompletedIterations++;

      // Increment the total elapsed time
      _iElapsedTime_Total_ms += _iElapsedTime_ms;

      // Print current status
      printf(   "\nIteration #%d time was %dms",
            _iNumCompletedIterations,
            _iElapsedTime_ms
         );   

      // Sleep to allow time for the connection to be lost on QPST
      Sleep( 1000 );
   
   }   // while (      (_iNumCompletedIterations < _iMaxLoopIterations )

   // Print overall status
   if ( _iNumCompletedIterations > 0 )
   {
      printf(   "\nTotal time for #%d interations was %dms, average time is %6.2fms",
            _iNumCompletedIterations,
            _iElapsedTime_Total_ms,
            (double) _iElapsedTime_Total_ms / (double) _iNumCompletedIterations
         );   
   
   }   // if ( _iNumCompletedIterations > 0 )

}

// Function to constantly search for a phone on the desired COM port
void ConstantPhoneSearch( void )
{
   // Number of loop iterations
   unsigned long _iMaxLoopIterations = 100;

   // Sum of all iterations
   unsigned long _iElapsedTime_Total_ms = 0;

   // Number of completed iterations
   unsigned long _iNumCompletedIterations = 0;

   // OK flag
   unsigned char _bOk = true;

   // Next key
   char _cNextKey = '\0';

   // Disconnect the QMSL handle. If QPST is being used then some other QPST application needs to be
   // started so that QPST Server does not quit. Otherwise the profile times will include QPST startup
   QLIB_DisconnectServer( g_hQMSL );

   // To start a log file automatically the file "QMSL_Auto_Start_Log.txt" must be in the working directory.
   // The file QMSL_Log.txt will be created automatically when conneting
   
   
   while (      (_iNumCompletedIterations < _iMaxLoopIterations )
         &&   ( _bOk )
         &&  ( _cNextKey != 'Q')
         )
   {
      printf("\nPress key for next search...");
      _cNextKey = toupper( getch() );

      if ( _cNextKey != 'Q' )
      {
         printf("\nStarting search...");
         // Time for the current iteration
         unsigned long _iStartTime = GetTickCount();

         // Get a new mobile handle. This is done instead of using the old handle because QPST will
         g_hQMSL  = QLIB_ConnectServerWithWait( g_iCOMport , 30000 );

         // Elapsed time
         unsigned long _iElapsedTime_ms = GetTickCount() - _iStartTime;

         // Increment number of completed iterations
         _iNumCompletedIterations++;

         // Increment the total elapsed time
         _iElapsedTime_Total_ms += _iElapsedTime_ms;

         // Print current status
         printf(   "\nIteration #%d time was %dms",
               _iNumCompletedIterations,
               _iElapsedTime_ms
            );   

         // Sleep to allow time for the connection to be lost on QPST
         Sleep( 1000 );
      }   

      QLIB_DisconnectServer( g_hQMSL );   
   }   // while (      (_iNumCompletedIterations < _iMaxLoopIterations )

   // Print overall status
   if ( _iNumCompletedIterations > 0 )
   {
      printf(   "\nTotal time for #%d interations was %dms, average time is %6.2fms",
            _iNumCompletedIterations,
            _iElapsedTime_Total_ms,
            (double) _iElapsedTime_Total_ms / (double) _iNumCompletedIterations
         );   
   
   }   // if ( _iNumCompletedIterations > 0 )

}

int main(int argc, char* argv[])
{
   int nRetCode = 0;
 
   if(argc == 4)
   {
      g_iUseQPST = atoi(argv[1]);
      g_iCOMport = atoi(argv[2]);
      g_iInstPrimaryAddress = atoi(argv[3]);

      printf("Use QPST %d\n", g_iUseQPST);
      printf("COM PORT %d\n", g_iCOMport);
      printf("Primary GPIB address  %d\n", g_iInstPrimaryAddress);
   }
   else
   {
      printf("QMSL_GPIB_Demo.exe <USE_QPST>, <COM_PORT>, <PRI_GPIB_ADDRESS>\n");
      printf("USE_QPST, 0==No, 1==Yes\n");
      printf("COM_PORT, -1==AutoDetect\n");
      printf("PRI_GPIB_ADDRESS, primary GPIB address\n");
      return 0;
   }

   // (int boardID, int pad, int sad, int tmo, int eot, int eos);
   g_hInst = ibdev(0, g_iInstPrimaryAddress, g_iInstSecondaryAddress, 13, 1, 0); 

   // Use QPST server , false to NOT use QPST
   QLIB_SetLibraryMode (g_iUseQPST);  

   // Get a mobile handle
   g_hQMSL  = QLIB_ConnectServer( g_iCOMport );   

   if(g_hQMSL && g_hInst)
   {
      QLIB_SetLogFlags (g_hQMSL, LOG_ALL );

      // Set up log file
      QLIB_StartLogging (g_hQMSL, "QMSL_GPIB_Demo_Log.txt");
   
      unsigned char _bExit = false;

      while ( !_bExit )
      {
         printf("\n\nQMSL Demonstration program for GPIB");
         // Get equipment name
         memset(sBuffer, 1000, 0);
         WriteToEquip("*IDN?");
         ReadFromEquipment(sBuffer, 1000);
         printf("\nInstrument Name: %s\n", sBuffer);
         printf("\nInstrument primary address: %d", g_iInstPrimaryAddress );
         printf("\nInstrument secondary address: %d", g_iInstSecondaryAddress );
         printf("\nQMSL COM Port Number: %d",  g_iCOMport );
         printf("\nQMSL Libray Mode: %s", ( g_iUseQPST == 1) ? "QPST" : "QPHONEMS") ;

         // Console menu
         printf("\n");
         printf("\n1=Power Supply ON and mobile power up time profile");
         printf("\n2=Rename EFSO /diag.bin to /.diagin");
         printf("\n3=Constant Phone Search");
         printf("\n4=Test FTM Sequencer");
         printf("\n5=FTM Sequencer - CDMA (obsolete, see QSEQ)");
         printf("\n6=FTM Sequencer - WCDMA (obsolete, see QSEQ)");
         printf("\n7=Reserved");
         printf("\n8=Technology non-signaling cycle");

         // Add more options here
         printf("\nQ=Quit");

         printf("\n Enter your selection->");
         char _cKey = toupper( getch() );

         if ( _cKey == 'Q' )
         {
            _bExit = true;
         }
         else if ( _cKey == '1' )
         {
            PowerSuppyMobilePowerUpTimeProfile();
         }
         else if ( _cKey == '2' )
         {
            unsigned long _iError = 0;
            unsigned char _bOk = QLIB_EFS2_DIAG_RENAME( 
                  g_hQMSL, 
                  "/diagin.bin", 
                  "/.diagin",
                  &_iError
                  );   
            printf("EFS Error code = %d", _iError );
         }
         else if ( _cKey == '3' )
         {
            ConstantPhoneSearch( );
         }
         else if ( _cKey == '4' )
         {
            Test_FTM_Sequencer();
         }
         else if ( _cKey == '5')
         {
            QSEQ_Sequencer_Prototype ( 0, g_hQMSL, g_hInst );   // CDMA
         }
         else if ( _cKey == '6')
         {
            QSEQ_Sequencer_Prototype ( 1, g_hQMSL, g_hInst );   // WCDMA
         }
         else if ( _cKey == '7')
         {
            Reserved();
         }
         else if ( _cKey == '8')
         {
            // printf("\n8=Technology non-signaling cycle");
            TechnologyNS_Cycle();
         }
         
         else
         {
         }
      }   // while ( !_bExit )
      
      // Stop Logging
      QLIB_StopLogging (g_hQMSL);
      // Clean up 
      QLIB_DisconnectServer(g_hQMSL);

      QLIB_DisconnectAllServers();
   }
   else
   {
      printf(   "Invalid QMSL handle (0x%X) or GPIB handle (0x%X)",
            (unsigned long) g_hQMSL,
            (unsigned long) g_hInst);
   }

   return nRetCode;
}


unsigned char Test_FTM_Sequencer( void )
{
   unsigned short iFTM_Error_Code;

   // This configuration is for MSM7x27
   unsigned short PDM[] = {154, 170, 186, 202, 218};

   unsigned short PDMsize = 5; // size of PDM List
   
   unsigned long iWaitms = 100;

   unsigned long _iChannel = 387;

   unsigned char _iStatus = 0;

   unsigned short _eMode = PHONE_MODE_CDMA_800;

   unsigned short _iPARange = 3;

   unsigned short _iLowPowerPDM = 511;

   unsigned char _logFileName[] = "C:\\ftm_sequencer.txt";

   unsigned char iSequenceCompleted = false;
   int i = 0;
   
   printf("\n\nTest_FTM_Sequencer\n");
   printf("=======================\n");

   //sprintf(_logFileName, "C:\\ftm_sequencer.txt" );
   QLIB_StartLogging( g_hQMSL,  (char*)_logFileName);
   printf("QLIB_StartLogging( %s )\n", _logFileName);

   _iStatus = QLIB_DIAG_CONTROL_F ( g_hQMSL, 3 );
   printf("QLIB_DIAG_CONTROL_F ( FTM_MODE) : Status = %d\n", _iStatus)  ;

   _iStatus = QLIB_FTM_SET_MODE( g_hQMSL, _eMode );
   printf("QLIB_FTM_SET_MODE( %d ): Status: %d\n", _eMode, _iStatus); 

   _iStatus = QLIB_FTM_SET_CHAN( g_hQMSL, (unsigned short)_iChannel );
   printf("QLIB_FTM_SET_CHAN ( %d ): Status: %d\n", _iChannel, _iStatus); 
   
   CalcFreqsFromChannels(_iChannel);

   _iStatus = QLIB_FTM_SET_TX_ON( g_hQMSL );
   printf("QLIB_FTM_SET_TX_ON (): Status: %d\n", _iStatus); 

   // 3 is high gain range on MSM7x27/RTR6500
   _iStatus = QLIB_FTM_SET_PA_RANGE( g_hQMSL, _iPARange  );
   printf("QLIB_FTM_SET_PA_RANGE ( %d ): Status: %d\n", _iPARange, _iStatus); 

   // Set to lower power first
   _iStatus = QLIB_FTM_SET_PDM( g_hQMSL, 2, _iLowPowerPDM);   
   printf("QLIB_FTM_SET_PDM ( %d ): Status: %d\n", _iLowPowerPDM, _iStatus); 
   
   // PC mode
   _iStatus = QLIB_FTM_SEQ_SetSequencerMode( g_hQMSL, 1);
   printf("QLIB_FTM_SEQ_SetSequencerMode ( 1 ): Status: %d\n", _iStatus); 

   _iStatus = QLIB_FTM_SEQ_ActivateBatchMode( g_hQMSL );
   printf("QLIB_FTM_SEQ_ActivateBatchMode (): Status: %d\n", _iStatus); 

   // Setup CMW500
   Cmw1CdmaMelmAnalyzerConfigure();
   
   for (i = 0; i < PDMsize; i++)
   {
      _iStatus = QLIB_FTM_SET_PDM( g_hQMSL, 2, PDM[i]);   
      printf("QLIB_FTM_SET_PDM(%d): Status: %d\n", PDM[i], _iStatus);  
      
      _iStatus = QLIB_FTM_SEQ_ADD_WAIT( g_hQMSL, iWaitms * 1000, &iFTM_Error_Code);
      printf("QLIB_FTM_SEQ_ADD_WAIT(%d micro seconds): Status: %d\n",  iWaitms * 1000, _iStatus);
   
   }
   
   _iStatus = QLIB_FTM_SET_TX_OFF( g_hQMSL );
   printf("QLIB_FTM_SET_TX_OFF(): Status = %d\n", _iStatus);
   
   ArmCMW500Trigger();
   
   _iStatus = QLIB_FTM_SEQ_EXECUTE_QUEUE( g_hQMSL, &iFTM_Error_Code, &iSequenceCompleted );
   printf("QLIB_FTM_SEQ_EXECUTE_QUEUE(): Status = %d\n", _iStatus);
   
   _iStatus = QLIB_FTM_SEQ_CLEAR_QUEUE( g_hQMSL, &iFTM_Error_Code );
   printf("QLIB_FTM_SEQ_CLEAR_QUEUE(): Status = %d\n", _iStatus);
   
   _iStatus = QLIB_FTM_SEQ_DeactivateBatchMode( g_hQMSL );
   printf("QLIB_FTM_SEQ_DeactivateBatchMode(): Status = %d\n", _iStatus);

   WaitForMeasurement();

   QLIB_StopLogging( g_hQMSL );

   return true;
}



void Cmw1CdmaMelmAnalyzerConfigure()
{
    int status = 0;
    int index = 0;
    char listIndex[10];
    char expPower[50];
    char freq_MHz[50];
    char command[512];
//    char dwell_msec[50];
//    char localBuffer[1024];
    char listQtyString[10];
   int listQty = LIST_SIZE;
   char buffer[512] = {0};

   //WriteToEquip( "SYST:ERR?;*OPC?" );
   //WriteToEquip( "ROUT:CDMA:MEAS:SCEN:SAL RF1C, RX1" );
   //WriteToEquip( "CONF:CDMA:MEAS:RCON R12Q" );
   //WriteToEquip( "CONF:CDMA:MEAS:MEV:ACP:FOFF 0.870000,0.885000,1.000000,1.250000,1.500000,1.980000,2.050000,3.000000,3.500000,4.000000" );
   //WriteToEquip( 
   
    // check for errors in the SCPI error buffer
    strcpy(command,"SYST:ERR?;*OPC?");
   //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
    WriteToEquip( command );
   
   /*
   strcpy(localBuffer, "");
    status = viRead (cmw1, localBuffer, 2000, &retCount);
   printf("%*s\n",retCount,localBuffer);
   */

    
    //Configure the CMW analyzer for a CDMA MELM capture
    strcpy(command, "ROUT:CDMA:MEAS:SCEN:SAL RF1C, RX1");
    WriteToEquip( command );
   //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
    

    strcpy(command, "CONF:CDMA:MEAS:RCON R12Q");
    WriteToEquip( command );
   //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
   
    strcpy(command, "CONF:CDMA:MEAS:MEV:ACP:FOFF 0.870000,0.885000,1.000000,1.250000,1.500000,1.980000,2.050000,3.000000,3.500000,4.000000");
    WriteToEquip( command );
   //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
   
    strcpy(command, "CONF:CDMA:MEAS:MEV:MOEX ON");
    WriteToEquip( command );
   //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
   
    strcpy(command, "CONF:CDMA:MEAS:MEV:LIST ON");
    WriteToEquip( command );
   //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
   
    sprintf(listQtyString, "%d", listQty);
    strcpy(command, "CONF:CDMA:MEAS:MEV:LIST:COUN ");
    strcat(command, listQtyString);
    //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
    WriteToEquip( command );

    strcpy(command, "TRIG:CDMA:MEAS:SOUR '");
    strcat(command, "IF Auto Sync");
    strcat(command, "'");
    //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
    WriteToEquip( command );

    strcpy(command, "TRIG:CDMA:MEAS:TOUT 5000");
    //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
   WriteToEquip( command );

    strcpy(command, "TRIG:CDMA:MEAS:THR  -30.00");
    //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
   WriteToEquip( command );

    strcpy(command, "TRIG:CDMA:MEAS:EOFF 8");
    //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
    WriteToEquip( command );

    // Loop through the array of uplink frequencies, and populate the CMW analyzer list configuration for each list segment
    for (index = 0; index < listQty; index++)
    {
        sprintf(listIndex, "%d", index+1);
        sprintf(expPower, "%f", expectedPower_dBm[index]);
        
        sprintf(freq_MHz, "%f", uplinkFreqs[index]);
        strcat(freq_MHz, "e6");

        strcpy(command, "CONF:CDMA:MEAS:MEV:LIST:SEGM");
        strcat(command, listIndex);
        strcat(command, ":SET 80, ");
        strcat(command, expPower);
        strcat(command, ", ");
        strcat(command, freq_MHz);
        //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
        WriteToEquip( command );

        strcpy(command, "CONF:CDMA:MEAS:MEV:LIST:SEGM");
        strcat(command, listIndex);
        strcat(command, ":MOD 10,ON,ON,ON,ON,ON,ON,ON,ON,ON,ON");
        //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
      WriteToEquip( command );

        strcpy(command, "CONF:CDMA:MEAS:MEV:LIST:SEGM");
        strcat(command, listIndex);
        strcat(command, ":SPEC 10,ON,ON");
        //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
      WriteToEquip( command );
   }

    // check for errors in the SCPI error buffer
    strcpy(command,"SYST:ERR?;*OPC?");
   //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
    WriteToEquip( command );
   
   /*
   //strcpy(localBuffer, "");
    //status = viRead (cmw1, localBuffer, 2000, &retCount);
   //printf("%*s\n",retCount,localBuffer);
   */
   ReadFromEquipment(buffer, 512);
    printf("\n");
    printf("CMW1 Signal Analyzer configured\n");
   
}



void ArmCMW500Trigger()
{
   printf("\nArm CMW500 Trigger\n");
   char buffer[256] = {0};
   WriteToEquip( "INIT:CDMA:MEAS:MEV;*OPC?" );
   ReadFromEquipment(buffer, 256);
   printf("Arm CMW500 Trigger Result %s\n", buffer);

}

void WaitForMeasurement()
{
   unsigned int index = 0;
   unsigned int listQty = LIST_SIZE;
   char buffer[256] = {0};
   char command[512];
   char buffer2[1000];

   double modResult[18];
   double acprResult[30];
   int modResultSz = 18;
   int acprResultSz = 30;
    WriteToEquip("FETC:CDMA:MEAS:MEV:STAT:ALL?");
    Sleep(3000);
    WriteToEquip("FETC:CDMA:MEAS:MEV:STAT:ALL?");
    ReadFromEquipment(buffer, 256);

    //printf("WaitForMeasurement Status %d", buffer);
    
       // Retrieve the modulation measurement results
    printf("\n==================================================================\n");   
    printf("\n");
    printf("Modulation Results\n");
    printf("\n");
    for (index = 0; index < listQty; index++)
    {
        char listIndex[10];
        
        sprintf(listIndex, "%d", index + 1);
       
        strcpy(command, "FETC:CDMA:MEAS:MEV:LIST:SEGM");
        strcat(command, listIndex);
        strcat(command, ":MOD:AVER?");
        //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
        WriteToEquip(command);
      //status = viRead (cmw1, buffer, 2000, &retCount);
      ReadFromEquipment(buffer2, 1000);
        //printf("%s\n", buffer2);

      ParseStringToDouble(buffer2, modResult, modResultSz);
      printf("Segment=%d Power (dBm)=%6.2f, waveform quality=%6.2f\n", index, modResult[12], modResult[14]);
      
      //printf("%*s\n",retCount,buffer);
    }

    printf("==================================================================\n");   
    printf("\n");
    printf("Spectrum Results\n");
    printf("\n");
   
    for (index = 0; index < listQty; index++)
    {
        char listIndex[10];
        
        sprintf(listIndex, "%d", index + 1);
       
        strcpy(command, "FETC:CDMA:MEAS:MEV:LIST:SEGM");
        strcat(command, listIndex);
        strcat(command, ":SPEC:AVER?");
      WriteToEquip(command);
      
      //status = viWrite (cmw1, (ViBuf)command, (ViUInt32)strlen(command), &writeCount);
        //status = viRead (cmw1, buffer, 2000, &retCount);
        ReadFromEquipment(buffer2, 1000);
      //printf("%s\n", buffer2);
      
      ParseStringToDouble(buffer2, acprResult, acprResultSz);
      
      printf("Segment=%d ACPR (dB) at +0.87MHz =%6.2f at -0.87MHz=%6.2f\n", index, acprResult[11], acprResult[13] );
      
      //printf("%*s\n",retCount,buffer);
   }
    
    printf("\n");
}

void CalcFreqsFromChannels(unsigned int _iChannel)
{
    int index = 0;

   for(index =0; index < LIST_SIZE; index++)
   {
      channels[index] = _iChannel;
   }
    /*
    printf("Calculate the DL and UL Frequencies from the channel numbers");
    printf("============================================================");
   */
    for (index = 0; index < sizeof(channels)/sizeof(channels[0]); index++)
    {
        double dl_MHz = 0.03 * channels[index] + 870;
        double ul_MHz = 0.03 * channels[index] + 825;
       // printf("Channel: %i   DL (MHz): %f   UL (MHz): %f\n", index, dl_MHz, ul_MHz); 
        
        downlinkFreqs[index] = dl_MHz;
        uplinkFreqs[index] = ul_MHz;
    }
    //printf("\n");
    //printf("\n");
}       

unsigned char Reserved()
{
   return 1;
}

void ParseStringToDouble(char* string, double *doubleArray, int& sizeOfArray)
{
   char* pch;
   int index = 0;
   pch = strtok(string, ",");

   while((pch!= NULL) && (index < sizeOfArray))
   {
      doubleArray[index++] = atof(pch);
      pch = strtok(NULL, ",");
   }
   sizeOfArray = index;
}
