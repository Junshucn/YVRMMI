/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/QlibDemo_Femto.cpp#6 $
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
#include "QLib_Defines.h"
#include "Qlib.h"
#include <stdio.h>
#include "QMSL_Misc\QMSL_Femto\inc\QMSL_Femto.h"
#include <exception>
using std::exception;

#define NVBUFSIZ 2048
static char NVBUF[NVBUFSIZ];

void DoTxCal(HANDLE h) {
   int txpath = 2;
   int mode = 514;
   int channel = 9800;
   QLIB_FEMTO_TXCAL_INIT(h, txpath, mode, channel);
}

struct TxDCOCResultItem {
   char ip;
   char im;
   char qp;
   char qm;
};
TxDCOCResultItem txDcocRes[64];

#define _QLIB_WRAP(cmd) {int _qlib_check_ret; _qlib_check_ret = cmd; if (!_qlib_check_ret) {throw exception("QLIB function returned an error: " #cmd);} }

#define MAX_TRX_SWEEP_CHANNELS          20
#define MAX_TRX_SWEEP_SEGMENTS          80

struct TrxSweepControlData {
   int status;
   int mode;
   int txPath; /* { 1, 2, 3, 4} */
   int rxPath; /* {11, 12, 13, 14} */
   int txDfe;     /* {1, 2, 3, 4} */
   int rxDfe;     /* {11, 12, 13, 14, 15} */
   int txDfeGain;
   int segmentTime_ms;
   int waveformId;
   int flags;
   int numSegments;
   int numChannels;
   int txGainStateList[MAX_TRX_SWEEP_SEGMENTS];
   int rxGainStateList[MAX_TRX_SWEEP_SEGMENTS];
   int channelList[MAX_TRX_SWEEP_CHANNELS];
};

unsigned char Test_Femto(int argc, char **argv) {
   //char resourceId[100];
   //printf("Enter the IP address of the Femto unit: ");
   //scanf("%s", resourceId);
   char *resourceId = argv[2];

   char *qmslFemtoTransportDllName = "QMSL_Femto_Transport.dll";
   int retryTimeoutSec = 10;

   HANDLE handle = QLIB_FEMTO_ConnectServer(qmslFemtoTransportDllName, resourceId, retryTimeoutSec);

   if (!handle) {
      printf(QLIB_FEMTO_GetLastError());
      return false;
   }

   try {
      //unsigned short _iLogFlags = LOG_FN | LOG_RET |  LOG_ERR | LOG_C_HIGH_LEVEL_START  | LOG_C_HIGH_LEVEL_STOP | LOG_ASYNC | LOG_IO;
      unsigned short _iLogFlags = LOG_FN | LOG_RET |  LOG_ERR | LOG_C_HIGH_LEVEL_START  | LOG_C_HIGH_LEVEL_STOP | LOG_ASYNC;
      QLIB_SetLogFlags( handle, _iLogFlags );
      QLIB_StartLogging( handle,  "C:\\QMSL_Femto_Factory_log.txt");
      //while (1) {
      printf("\n----------------\nFemto Commands\n----------------\n");

      printf("\n");
      printf("1=Read RSSI\n" );
      printf("2=Send GPIO Command for PA on\n" );
      printf("3=Tx Cal\n");
      printf("4=DCOC test\n");

      printf("0=Quit\n");
      printf(">");

      int _cmd;
      //scanf("%d", &_cmd);
      _cmd = atoi(argv[3]);

      if (_cmd == 1 ) {
         int dpvalue;
         QLIB_FEMTO_FTM_READ_DIGITAL_POWER(handle, 11, &dpvalue);
         printf("rssi=%d\n", dpvalue);
      }
      else if (_cmd == 2) {
         int ret = QLIB_FEMTO_FTM_WRITE_GPIO(handle, 2, 0x78000, 0x78000);
         printf("ret=%d", ret);
      }
      else if (_cmd == 3) {
         DoTxCal(handle);
      }
      else if (_cmd == 4) {
         int N = sizeof(TxDCOCResultItem)*64;
         memset(txDcocRes, 0, N);

         int tip, tim, tqp, tqm, null5, null6;
         int path = 2;
         int band = 2;
         int channel = atoi(argv[4]) ; // = 9938;
         //printf("channel number:");
         //scanf("%d", &channel);
         int startIndex = 0;
         int stopIndex = 47;
         //printf("Enter: path band channel startIndex stopIndex\n:");
         //if (scanf("%d %d %d %d %d", &path, &band, &channel, &startIndex, &stopIndex) != 5) {
         //   printf("Invalid input. Enter params with space in between");
         //   continue;
         //}
         int MAXIND = 47;
         if (startIndex < 0 || startIndex > MAXIND || stopIndex < 0 || stopIndex > MAXIND) {
            printf("Invalid input for stop or start index");
         }

         for (int i=startIndex; i<=stopIndex; i++){
            QLIB_FEMTO_FTM_SELF_CAL_TX_DCOC(handle, path, i, band, channel, 0, &tip, &tim, &tqp, &tqm, &null5, &null6);
            printf("%d, %d, %d, %d: %d %d %d %d\n", path, i, band, channel, tip, tim, tqp, tqm);
            txDcocRes[i].im = (char)tim;
            txDcocRes[i].ip = (char)tip;
            txDcocRes[i].qm = (char)tqm;
            txDcocRes[i].qp = (char)tqp;
         }
         //printf("Write results for band 1? (0=no, 1=yes\n?");
         int doBand = 1;
         int nvid = 0x2207;
         int offset = 0;
         //if (scanf("%d", &doBand)) {
         if (doBand == 1) {
            int mode = 513;

            QLIB_FEMTO_FTM_SET_MODE(handle, mode);
            QLIB_FEMTO_FTM_LOAD_NV_PARAMS(handle, mode);
            int calVer = -3;
            QLIB_FEMTO_NV_Read_Ex(handle, 0x3f0e, 0, 4, (unsigned char*)&calVer);
            printf("current cal ver=%d\n", calVer);
            calVer++;

            QLIB_FEMTO_NV_Write_Ex(handle, 0x3f0e, 0, 4, (unsigned char*)&calVer);

            if (doBand == 1) {
               for (int gainInd=0; gainInd<=MAXIND; gainInd++) {
                  offset = gainInd * sizeof(TxDCOCResultItem) * 16;
                  for (int chanInd=0; chanInd<16; chanInd++) {
                     printf(".");
                     int res = QLIB_FEMTO_NV_Write_Ex(handle, nvid, offset, sizeof(TxDCOCResultItem), (unsigned char*)&txDcocRes[gainInd]);
                     offset += sizeof(TxDCOCResultItem);
                  }
                  printf("\n");
               }
            }

            QLIB_FEMTO_FTM_COMMIT_NV_PARAMS(handle, mode);
         }

         //char buf[1024];
         //QLIB_FEMTO_NV_Read_Ex(handle, 0x2207, 0, 64, (unsigned char*)buf);
         //for (int i=0; i<16; i++) {
         //   printf("%02x ", buf[i]);
         //}
      }
      //else if (_cmd == 5) {
      //   int N = sizeof(TxDCOCResultItem)*64;
      //   for (int i=0; i<16; i++) {
      //      printf("%02x ", *( ((char*)txDcocRes) + i ));
      //   }
      //}
      else if (_cmd == 5) {
         /*int mode = 513;
         int band = 1;
         int channel = 10700;
         int hhChain = 0;
         int dac = 1;
         int adc = 0;
         int testId = 1;

         _QLIB_WRAP ( QLIB_FEMTO_FTM_RFCAL_RF_INIT(handle, mode));
         _QLIB_WRAP ( QLIB_FEMTO_FTM_SET_MODE(handle, mode));
         _QLIB_WRAP ( QLIB_FEMTO_FTM_LOAD_NV_PARAMS(handle, mode));
         _QLIB_WRAP ( QLIB_FEMTO_FTM_RFCAL_SETUP_DVT(handle, band, channel, channel, hhChain, dac, adc));
         _QLIB_WRAP ( QLIB_FEMTO_FTM_DVT_UMTS_START_TX(handle, 0, 0, testId));
         _QLIB_WRAP( QLIB_FEMTO_TXDVT_INIT(handle, 2, 513, 10700, 1) );
         _QLIB_WRAP( QLIB_FEMTO_FTM_WRITE_GPIO(handle, 2, 0x78000, 0x78000) );
         _QLIB_WRAP( QLIB_FEMTO_FTM_SET_TX_POWER(handle, 2, 0) );*/
      }
      else if (_cmd == 6) {
         for (int regVal=0x53; regVal<= 0x56; regVal++) {
            int val;
            _QLIB_WRAP( QLIB_FEMTO_FTM_READ_HW_REGISTER(handle,2,1,1,regVal, &val) );
            printf("reg 0x%02x = %d\n", regVal, val);
         }

         //QLIB_FEMTO_NV_Read_Ex(handle, nvid, offset, len, (unsigned char*)NVBUF);
         //for (int i=0; i<16; i++) {
         //   printf("%02x ", buf[i]);
         //}
      }
      else if (_cmd == 7) {
      }
      else if (_cmd == 8) {
      }
      else if (_cmd == 0) {
         //            break;
      }
      else {
         printf("unknown command");
      }
      //}
      QLIB_DisconnectServer(handle);
   }
   catch(...) {
      printf("exception occurred");
      QLIB_DisconnectServer(handle);
   }

   return true;
}
