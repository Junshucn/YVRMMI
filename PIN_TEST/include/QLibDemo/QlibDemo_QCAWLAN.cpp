/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/QlibDemo_QCAWLAN.cpp#12 $
 * $DateTime: 2017/07/05 15:35:28 $
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

#include "windows.h"

#include "stdio.h"
#include "stdlib.h"

#include "QMSL_Core.h"

// =================================================================================
// Dut related APIs - Function protocol
enum WLAN_TLV_OPCODE
{
   _OP_TX = 1,
   _OP_RX = 2,
   _OP_CAL = 3,
   _OP_CAL_DONE = 4,
   _OP_TX_STATUS = 10,
   _OP_RX_STATUS = 11,
   _OP_RESET = 15,
   _OP_OTP = 16,
   _OP_CAL_INIT = 23,
};

enum WLAN_TLV2_OPCODE
{
   _OP2_TX = 1,
   _OP2_RX = 2,
   _OP2_SETPHYRFMODE = 169,

};


typedef double( *asyncPMMessageCB )
   (
   unsigned int iGain,
   unsigned int iFreq,
   double dPowerLevel,
   unsigned int iNumAvg,
   unsigned int iTriggerType,
   int iTriggerLevel      // for measure CW tone, set iTriggerLevel = -1000
   );

typedef double( *asyncCWFreqErrMessageCB  )
   (
   unsigned int iCapValue,
   unsigned int iCwFreqMHz,
   double dPowerLevel,
   unsigned int iNumAvg
   );

#define _QLIB_WLAN_ATHEROS_H
#ifndef QLIB_API
#define QLIB_API __declspec(dllimport)
#endif
extern "C" {
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_LoadDUT(HANDLE hResourceContext,unsigned char *DevDLLname, unsigned char *boardDataFName, int iNVMem, unsigned int ssid);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_UNLoadDUT(HANDLE hResourceContext);
   QLIB_API char QLIB_FTM_WLAN_TLV_Create(HANDLE hResourceContext,unsigned char opCode);
   QLIB_API char QLIB_FTM_WLAN_TLV_AddParam(HANDLE hResourceContext, char *pKey, char *pData);
   QLIB_API char QLIB_FTM_WLAN_TLV_Complete(HANDLE hResourceContext);
   QLIB_API char QLIB_FTM_WLAN_TLV_GetRspParam(HANDLE hResourceContext,char *pKey, char *pData);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_Tx_CALIBRATE_INIT(HANDLE hResourceContext, unsigned char *calDLLname, int iCalPoints, int iCalGainMode);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_Tx_CAL(HANDLE hResourceContext, asyncPMMessageCB pAsyncPMMessageCB, unsigned int measAvg);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_xtal_CAL_INIT(HANDLE hResourceContext, int iXtalTargetPPM, unsigned int iXtalTolerancePPM, int iXtalCapDelta, unsigned int iXtalCalSaveOption);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_xtal_CAL(HANDLE hResourceContext, asyncCWFreqErrMessageCB pAsyncCWFreqErrMessageCB, unsigned int measAvg);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_GET_BD_ADDRESS_FROM_OTP(HANDLE hResourceContext, unsigned char *aiBD_Address);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SET_BD_ADDRESS_IN_OTP(HANDLE hResourceContext, unsigned char aiBD_Address[6]);
   QLIB_API unsigned char QLIB_FTM_WLAN_SET_TOP_CONFIG_IN_OTP(HANDLE hResourceContext, unsigned int iAction,unsigned int iValue);
   QLIB_API unsigned char QLIB_FTM_WLAN_GET_TOP_CONFIG_FROM_OTP(HANDLE hResourceContext, unsigned int iAction,unsigned int *iValue);
   QLIB_API unsigned char QLIB_FTM_WLAN_SET_BT_CONFIG_IN_OTP(HANDLE hResourceContext, unsigned int iAction,unsigned int iValue);
   QLIB_API unsigned char QLIB_FTM_WLAN_GET_BT_CONFIG_FROM_OTP(HANDLE hResourceContext, unsigned int iAction,unsigned int *iValue);
   QLIB_API unsigned char QLIB_FTM_WLAN_SET_PCIe_CONFIG_IN_OTP_STREAM(HANDLE g_hResourceContext, unsigned int iAction,unsigned int iValue);
   QLIB_API unsigned char QLIB_FTM_WLAN_GET_PCIe_CONFIG_FROM_OTP_STREAM(HANDLE hResourceContext, unsigned int iAction,unsigned int *iValue);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SET_OTP_REG_DOMAIN( HANDLE hResourceContext, unsigned int aiRegDmn[2]);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_GET_OTP_REG_DOMAIN( HANDLE hResourceContext, unsigned int *aiRegDmn);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_DumpOTPToFile(HANDLE hResourceContext, char *piFilename);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_CompareOTPAgainstFile(HANDLE hResourceContext, char *piFilename, char *piResultsFilename, unsigned int *iResults);
   QLIB_API char QLIB_FTM_WLAN_TLV2_Create(HANDLE hResourceContext,unsigned char opCode);
   QLIB_API char QLIB_FTM_WLAN_TLV2_AddParam(HANDLE hResourceContext, char *pKey, char *pData);
   QLIB_API char QLIB_FTM_WLAN_TLV2_Complete(HANDLE hResourceContext);
   QLIB_API char QLIB_FTM_WLAN_TLV2_GetRspParam(HANDLE hResourceContext,char *pKey, char *pData);

}

// UserDefinedTransport - Function protocol
typedef unsigned long (*_OpenUserDefinedTransport)(char* pIpPort );
typedef void (*_CloseUserDefinedTransport)(char* pIpPort );
typedef unsigned long (*_UserDefinedReceive)(HANDLE hQMSL_ContextID, HANDLE hUserContextID,   unsigned long* piResponseSize,   unsigned char* piResponseBytes);
typedef unsigned long (*_UserDefinedSend)(HANDLE hQMSL_ContextID, HANDLE hUserContextID, unsigned long iRequestSize, unsigned char* piRequestBytes,   unsigned long* piActualWriteSize);
typedef unsigned long (*_UserDefinedFlushTxRx)(HANDLE hQMSL_ContextID, HANDLE hUserContextID);

static unsigned long (*OpenUserDefinedTransport)(char* pIpPort );
static void (*CloseUserDefinedTransport)(char* pIpPort );
static unsigned long (*UserDefinedReceive)(HANDLE hQMSL_ContextID, HANDLE hUserContextID, unsigned long* piResponseSize,   unsigned char* piResponseBytes);
static unsigned long (*UserDefinedSend)(HANDLE hQMSL_ContextID, HANDLE hUserContextID, unsigned long iRequestSize, unsigned char* piRequestBytes,   unsigned long* piActualWriteSize);
static unsigned long (*UserDefinedFlushTxRx)(HANDLE hQMSL_ContextID, HANDLE hUserContextID);

// ROME Chip ID
#define ROME_CHIPID      0x3e
#define BEELINER_CHIPID  0x40
#define DAKOTA_CHIPID    0x40
#define CASCADE_CHIPID   0x46
#define BESRA_CHIPID     0x56
#define KINGFISHER_CHIPID    0x3e

#define ADRASTEA_CHIPID  0xFF

// Loopback IP for local system or any remote system IP with DUT
char UDT_IP[20] = "127.0.0.1";
char UDT_COM[20] = "COM10";

// Chip specific DLL
#define ROME_DLLID       "qc6174"
#define BEELINER_DLLID   "qc9900"
#define DAKOTA_DLLID     "ipq4019"
#define CASCADE_DLLID    "qc9984"
#define BESRA_DLLID      "qc9886"
#define ADRASTEA_DLLID   "qc6180"
#define KINGFISHER_DLLID   "ar6006"

char (*pQLIB_FTM_WLAN_TLV_Create)(HANDLE hResourceContext,unsigned char opCode);
char (*pQLIB_FTM_WLAN_TLV_AddParam)(HANDLE hResourceContext, char *pKey, char *pData);
char (*pQLIB_FTM_WLAN_TLV_Complete)(HANDLE hResourceContext);
char (*pQLIB_FTM_WLAN_TLV_GetRspParam)(HANDLE hResourceContext,char *pKey, char *pData);

// Board data file where depends on chip variation
char BIN_FILE[200];
char ROME_BINFILE[200] =      "C:\\Qualcomm\\WCN\\ProdTests\\refDesigns\\BoardData\\fakeBoardData_AR6320.bin";
char BEELINER_BINFILE[200] =  "C:\\Qualcomm\\WCN\\ProdTests\\refDesigns\\BoardData\\fakeBoardData_AR9900.bin";
char DAKOTA_BINFILE[200] =    "C:\\Qualcomm\\WCN\\ProdTests\\refDesigns\\BoardData\\fakeBoardData_ipq4019.bin";
char CASCADE_BINFILE[200] =   "C:\\Qualcomm\\WCN\\ProdTests\\refDesigns\\BoardData\\fakeBoardData_AR9984.bin";
char BESRA_BINFILE[200] =     "C:\\Qualcomm\\WCN\\ProdTests\\refDesigns\\BoardData\\fakeBoardData_AR9886.bin";
char ADRASTEA_BINFILE[200] =  "C:\\Qualcomm\\WCN\\ProdTests\\refDesigns\\BoardData\\fakeBoardData_AR6180.bin";
char KINGFISHER_BINFILE[200] =  "C:\\Qualcomm\\WCN\\ProdTests\\refDesigns\\BoardData\\boardData_QCA4020_YA560.bin";

//Filenames for OTP Dump and compare functionality
#define OTP_DUMP_FILE       "C:\\log\\otpRawContents.txt"
#define OTP_RESULTS_FILE    "C:\\log\\compareResults.txt"

// Data storage
enum NVMEM_OPTION
{
   DataNone = 0,
   DataFlash,
   DataEeprom,
   DataOtp,
   DataDontLoad,
   DataFile,
   DataDram
};

#define USER_HANDLE   1

static HINSTANCE hUDT = NULL;
static HANDLE g_hResourceContext = NULL;
static int QLIB_WLAN_80P80_SUPPORT = 0;
static int QLIB_WLAN_MULTIPHY_SUPPORT = 0;
// QLib.h
#define QLIB_LIB_MODE_QPHONEMS  0
#define QLIB_TARGET_TYPE_APQ    1

#define QLIB_DIAG_TYPE_STANDARD	0
#define QLIB_DIAG_TYPE_LITE		1

char *chip_select = NULL;
// ======================================================================================================
/// <summary>
/// Load User Defined Transport and setup Callback functions
/// </summary>
bool ConnectStandaloneWlanCard()
{
   //80-WL400-11 Refer Section "Connect - standalone targets"
   hUDT = LoadLibrary("QMSL_WLAN_Transport.dll");  // Must reside same folder as QSPR.exe
   if (hUDT == NULL)
   {
      printf("Can not find QMSL_WLAN_Transport.dll.\n");
      return false;
   }

   OpenUserDefinedTransport = (_OpenUserDefinedTransport)GetProcAddress(hUDT,"OpenUserDefinedTransport");
   CloseUserDefinedTransport =(_CloseUserDefinedTransport)GetProcAddress(hUDT,"CloseUserDefinedTransport");
   UserDefinedReceive = (_UserDefinedReceive)GetProcAddress(hUDT,"UserDefinedReceive");
   UserDefinedSend = (_UserDefinedSend)GetProcAddress(hUDT,"UserDefinedSend");
   UserDefinedFlushTxRx = (_UserDefinedFlushTxRx)GetProcAddress(hUDT,"UserDefinedFlushTxRx");

   if ((OpenUserDefinedTransport == NULL) ||
      (CloseUserDefinedTransport == NULL) ||
      (UserDefinedReceive == NULL) ||
      (UserDefinedSend == NULL))
   {
      printf("Loading QMSL_WLAN_Transport function pointer failed.\n");
      return false;
   }

   if (OpenUserDefinedTransport(UDT_IP) != 0)  // IP can be remote system with DUT or local
   {
      printf("OpenUserDefinedTransport failed. [%s]\n", UDT_IP);
      return false;
   }

   // Setup Callback functions
   g_hResourceContext = QLIB_ConnectServer_UserDefinedTransport((HANDLE) USER_HANDLE,
      UserDefinedSend,
      UserDefinedReceive,
      UserDefinedFlushTxRx,
      true,
      true);
   if (g_hResourceContext == NULL)
   {
      printf("QLIB_ConnectServer_UserDefinedTransport failed.\n");
   }

   return (g_hResourceContext == NULL ? false : true);
}

/// <summary>
/// Load User Dut Type and setup Callback functions
/// </summary>
bool ConnectDiagTypeLiteWlanCard()
{
   QLIB_SetLibraryMode(QLIB_LIB_MODE_QPHONEMS);
   QLIB_SetTargetType((unsigned char)QLIB_TARGET_TYPE_APQ);
   QLIB_SetDiagType(QLIB_DIAG_TYPE_LITE);
   
   return ConnectStandaloneWlanCard();
}


// ======================================================================================================
/// <summary>
/// Unload User Defined Transport
/// </summary>
void DisconnectStandaloneWlanCard()
{
   //80-WL400-11 Refer Section "Disconnect - standalone targets"
   if (g_hResourceContext)
      QLIB_DisconnectServer(g_hResourceContext);

   CloseUserDefinedTransport(UDT_IP);

   if (hUDT)
      FreeLibrary(hUDT);
}

// ======================================================================================================
/// <summary>
/// Load DUT with associated chip specific DLL, board data file and chip ID
/// </summary>
bool LoadDut(char *dllID, char*bin_file, unsigned int chipID)
{
   int LoadOption=0;
   //80-WL400-11 Refer Section "Load DUT"
   if(0 != strncmp(dllID, KINGFISHER_DLLID, sizeof(KINGFISHER_DLLID)))
	{
	   QLIB_SetLibraryMode(QLIB_LIB_MODE_QPHONEMS);
	   QLIB_SetTargetType((unsigned char)QLIB_TARGET_TYPE_APQ);
	}
   if (g_hResourceContext == NULL)
      return false;

   printf("Select Load BoardData File, 0: None    5: Data File\n");
	scanf("%d", &LoadOption);
   if(LoadOption != 0)
   	LoadOption = DataFile;
   unsigned char bRet = QLIB_FTM_WLAN_Atheros_LoadDUT(g_hResourceContext,(unsigned char  *)dllID,(unsigned char *)bin_file, LoadOption, chipID);
   return (bRet ? true : false);
}

// ======================================================================================================
/// <summary>
/// Unload DUT with associated chip specific DLL
/// </summary>
void CloseDut()
{
   //80-WL400-11 Refer Section "Unload DUT"
   QLIB_FTM_WLAN_Atheros_UNLoadDUT(g_hResourceContext);
}

// ======================================================================================================
/// <summary>
/// TxCal call back power measure function
/// </summary>
/// <param name="iGain">gain Index for logging</param>
/// <param name="iFreq">channel frequence in MHz</param>
/// <param name="dPowerLevel">expected measured power level</param>
/// <param name="iNumAvg">number of average for power measurements</param>
/// <param name="iTriggerType">trigger type</param>
/// <param name="iTriggerLevel">trigger level</param>
double asyncPMCB(unsigned int iGain, unsigned int iFreq, double dPowerLevel,
   unsigned int iNumAvg, unsigned int iTriggerType, int iTriggerLevel)
{
   double power[100], pwr;
   double measuredAvgPower = 0;
   unsigned int it = 0;
   while (it < iNumAvg)
   {
      printf("Please enter a power to simulate the measured power\n");
      scanf("%lf",&pwr);
      // get measured power from your instrument for your calibration program
      power[it ++] = pwr;
      measuredAvgPower += power[it-1];
   }
   if (iNumAvg>0)
      measuredAvgPower = measuredAvgPower/iNumAvg; // average the measured iNumAvg powers;
   else
      measuredAvgPower = -999.00;
   return measuredAvgPower;
}
// ======================================================================================================
/// <summary>
/// TxOLPCCal
/// </summary>
/// <param name="channel">channel number</param>
/// <param name="txChain0">Tx Chain to turn on</param>
/// <param name="gainIdx">Gain Index array</param>
/// <param name="dacGain">Dac gain array</param>
/// <param name="paConfig">PA configuration array</param>
/// <param name="paConfig">PA configuration array</param>
void TxOLPCCal(int channel, int txChain0, double measPwrDelta, int maxIt, int numMeasAvg )
{
   char buf[32], key[10];
   int i;

   int rateBitIndex0 = 16; // HT20_MCS0
   int wlanMode = 1;       // HT20_MCS0
   int pktLen0 = 1500;

   //80-WL400-13 Refer 3.2.2 Power Calibration QMSL calling sequence

   // Setting all OLPC calibration initial parameters by tlv calls
   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_CAL);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel",_itoa(channel,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txChain0", _itoa(txChain0,buf,10));             //1(ch0),2(ch1),3(ch01)

   int txMode = 3; // Tx99
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txMode",_itoa(txMode,buf,10));                //3 = Tx99
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rateBitIndex0", _itoa(rateBitIndex0,buf,10));   //Please refer WLAN_tlvDataRateTX, MCS 6 HT20
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"wlanMode", _itoa(wlanMode,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"pktLen0", _itoa(pktLen0,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"shortGuard", "0");           // 0: disable short GI
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"numPackets", "0");          // 0 = continuous, otherwise number of packets
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"aifsn", "0");

   int flags = 0x18;
   //if (ldpcRate)  flags |= DESC_LDPC_ENA_MASK;  // 0x00000002
   //if (dpdMode)   flags |= PAPRD_ENA_MASK;      // 0x00000004
   //if (STBCmode)  flags |= DESC_STBC_ENA_MASK;  // 0x00000001
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"flags", _itoa(flags,buf,10));

   // tpcm TX_PWR=0, FORCED_GAIN=1, TGT_PWR=2, TX_FORCED_GAIN=3, FORCED_GAINIDX=4, FORCED_TGTPWR=5
   int tpcm = 4;   // For OLPC, always force gainIndex
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"tpcm", _itoa(tpcm,buf,10));

   double targetPwr = 18.0;
   double slope=2.0;
   sprintf(buf, "%.2f", targetPwr);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"targetPower0", buf);
   sprintf(buf, "%.2f", measPwrDelta);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"measPowerDelta0", buf);
   sprintf(buf, "%d", maxIt);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"maxIteration0", buf);
   sprintf(buf, "%.2f", slope);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"gainSlope", buf);

   int gainIdx[5] = {15,4,5,0,0};
   int dacGain[5] = {-8,-8,-8,-8,-8};
   int paConfig[5] = {0,0,0,1,2};
   for (i=0; i<5; i++) {
      sprintf(key, "gainIdx%d", i);
      pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,key, _itoa(gainIdx[i],buf,10));
      sprintf(key, "dacGain%d", i);
      pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,key, _itoa(dacGain[i],buf,10));
      sprintf(key, "paConfig%d", i);
      pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,key, _itoa(paConfig[i],buf,10));
   }
   // Completed OLPC calibration initial parameters by TLV_Complete calls
   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);

   // Start to run OLPC calibration iterations by power measurement call back
   asyncPMMessageCB pPMfunc = &asyncPMCB;
   QLIB_FTM_WLAN_Atheros_Tx_CAL(g_hResourceContext, pPMfunc, (unsigned int)numMeasAvg);

   // Pulling back OLPC calibration results.
   double measPwr[5];
   int calGainIndx[5], calDacGain[5], calPAconfig[5];
   for (i=0; i<5; i++) {
      sprintf(key, "measPower%d", i);
      pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      measPwr[i] = atof(buf);
      sprintf(key, "gainIdx%d", i);
      pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      calGainIndx[i] = atoi(buf);
      sprintf(key, "dacGain%d", i);
      pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      calDacGain[i] = atoi(buf);
      sprintf(key, "paConfig%d", i);
      pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      calPAconfig[i] = atoi(buf);
      printf("measPwr:%5.2f    gainIndx:0x%2x     dacGain:%2d     paConfig:%d\n",
         measPwr[i], calGainIndx[i], calDacGain[i], calPAconfig[i] );
   }
   pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"THERMCAL", buf);
   printf("Therm:%s\n", buf);
}

// ======================================================================================================
/// <summary>
/// TxCLPCCal
/// </summary>
/// <param name="channel">channel number</param>
/// <param name="txChain0">Tx Chain to turn on</param>
void TxCLPCCal(int channel, int txChain0, double measPwrDelta, int maxIt, int numMeasAvg )
{
   char buf[32], key[10];
   int i;

   int rateBitIndex0 = 16; // HT20_MCS0
   int wlanMode = 1;       // HT20_MCS0
   int pktLen0 = 1500;

   //80-WL400-13 Refer 3.2.2 Power Calibration QMSL calling sequence

   // Setting all CLPC calibration initial parameters by tlv calls
   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_CAL);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel",_itoa(channel,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txChain0", _itoa(txChain0,buf,10));             //1(ch0),2(ch1),3(ch01)

   int txMode = 8; // for CLPC
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txMode",_itoa(txMode,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rateBitIndex0", _itoa(rateBitIndex0,buf,10));   //Please refer WLAN_tlvDataRateTX, MCS 6 HT20
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"wlanMode", _itoa(wlanMode,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"pktLen0", _itoa(pktLen0,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"shortGuard", "0");           // 0: disable short GI
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"numPackets", "0");          // 0 = continuous, otherwise number of packets
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"aifsn", "0");

   int flags = 0x18;
   //if (ldpcRate)  flags |= DESC_LDPC_ENA_MASK;  // 0x00000002
   //if (dpdMode)   flags |= PAPRD_ENA_MASK;      // 0x00000004
   //if (STBCmode)  flags |= DESC_STBC_ENA_MASK;  // 0x00000001
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"flags", _itoa(flags,buf,10));

   // tpcm TX_PWR=0, FORCED_GAIN=1, TGT_PWR=2, TX_FORCED_GAIN=3, FORCED_GAINIDX=4, FORCED_TGTPWR=5
   int tpcm = 4;   // For OLPC, always force gainIndex
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"tpcm", _itoa(tpcm,buf,10));

   double targetPwr = 20.0;
   sprintf(buf, "%.2f", targetPwr);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"targetPower0", buf);
   for (i=0; i<10; i++) {
      measPwrDelta = 0.5;
      sprintf(buf, "%.2f", measPwrDelta);
      sprintf(key, "measPowerDelta%d", i);
      pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, key, buf);
      sprintf(buf, "%d", maxIt);
      sprintf(key, "maxIteration%d", i);
      pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,key, buf);
   }
   double slope=8.0;
   sprintf(buf, "%.2f", slope);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"gainSlope", buf);
   int clpcSensitivePdadc=200;
   sprintf(buf, "%d", clpcSensitivePdadc);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"clpcSensitivePdadc", buf);

   int gainIdx0 = 17;
   int dacGain0 = 0;
   int paConfig0 = 0;
   sprintf(buf, "%d", gainIdx0);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "gainIdx0", buf);
   sprintf(buf, "%d", dacGain0);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "dacGain0", buf);
   sprintf(buf, "%d", paConfig0);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "paConfig0", buf);
   // Completed CLPC calibration initial parameters by TLV_Complete calls
   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);

   // Start to run CLPC calibration iterations by power measurement call back
   asyncPMMessageCB pPMfunc = &asyncPMCB;
   QLIB_FTM_WLAN_Atheros_Tx_CAL(g_hResourceContext, pPMfunc, (unsigned int)numMeasAvg);

   // Pulling back CLPC calibration results.
   double measPwr[10];
   int calGainIndx[10], calDacGain[10], calPAconfig[10];
   for (i=0; i<10; i++) {
      sprintf(key, "measPower%d", i);
      pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      measPwr[i] = atof(buf);
      sprintf(key, "gainIdx%d", i);
      pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      calGainIndx[i] = atoi(buf);
      sprintf(key, "dacGain%d", i);
      pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      calDacGain[i] = atoi(buf);
      sprintf(key, "paConfig%d", i);
      pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,key, buf);
      calPAconfig[i] = atoi(buf);
      printf("measPwr:%5.2f    gainIndx:0x%2x     dacGain:%2d     paConfig:%d\n",
         measPwr[i], calGainIndx[i], calDacGain[i], calPAconfig[i] );
   }
}

// ======================================================================================================
/// <summary>
/// XTAL cal call back freqErr measure function
/// <summary>
/// call back function for freqErr measurement used in xtal calibration
/// </summary>
/// <param name="freq">channel freq in MHz</param>
/// <param name="powerLevel">CW power level in dBm</param>
/// <param name="avgNum">number of measurements used for FreqErr average</param>
double asyncFreqErrCB(unsigned int capValue, unsigned int freq, double powerLevel, unsigned int avgNum)
{
   double freqErr[100], fErr;
   double measurefreqErr = 0;

   unsigned int avgIteration = 0, num=0;

   while (avgIteration < avgNum)
   {
      printf("Please enter a number to simulate the measured freqErr in PPM\n");
      scanf("%lf",&fErr);
      // get measured frequence error from your instrument for your calibration program
      freqErr[avgIteration] = fErr;
      if (freqErr[avgIteration] > -999)
      {
         measurefreqErr += freqErr[avgIteration];
         num++;
      }
      avgIteration++;
   }
   if (num > 0)
      measurefreqErr = measurefreqErr / num;
   else
      measurefreqErr = -999;
   return measurefreqErr;
}

// ======================================================================================================
/// <summary>
/// XTAL Cal
/// </summary>
/// <param name="channel">channel number</param>
/// <param name="txChain0">Tx Chain to turn on</param>
void XTALCal(int channel, int txChain0, int iXtalTargetPPM, unsigned int iXtalTolerancePPM, int numMeasAvg )
{
   char buf[32];

   //80-WL400-13 Refer 4.2.2 XTAL Calibration QMSL calling sequence

   // Setting all XTAL calibration initial parameters by tlv calls
   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_CAL);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel",_itoa(channel,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txChain0", _itoa(txChain0,buf,10));             //1(ch0),2(ch1),3(ch01)

   int txMode = 1; // for xtal cal CW tone
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txMode",_itoa(txMode,buf,10));
   int wlanMode = 0;       // NO HT
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"wlanMode", _itoa(wlanMode,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"aifsn", "0");

   int flags = 0x18;
   //if (ldpcRate)  flags |= DESC_LDPC_ENA_MASK;  // 0x00000002
   //if (dpdMode)   flags |= PAPRD_ENA_MASK;      // 0x00000004
   //if (STBCmode)  flags |= DESC_STBC_ENA_MASK;  // 0x00000001
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"flags", _itoa(flags,buf,10));

   // tpcm TX_PWR=0, FORCED_GAIN=1, TGT_PWR=2, TX_FORCED_GAIN=3, FORCED_GAINIDX=4, FORCED_TGTPWR=5
   int tpcm = 4;   // For OLPC, always force gainIndex
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"tpcm", _itoa(tpcm,buf,10));

   int gainIdx = 10;
   int dacGain = 0;
   int paConfig = 0;
   sprintf(buf, "%d", gainIdx);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "gainIdx", buf);
   sprintf(buf, "%d", dacGain);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "dacGain", buf);
   sprintf(buf, "%d", paConfig);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "paConfig", buf);
   // Completed XTAL calibration initial parameters by TLV_Complete calls
   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);

   // Start to run XTAL calibration iterations by frequence error measurement call back
   asyncCWFreqErrMessageCB pFErrfunc = &asyncFreqErrCB;
   QLIB_FTM_WLAN_Atheros_xtal_CAL(g_hResourceContext, pFErrfunc, (unsigned int)numMeasAvg);

   // Pulling back XTAL calibration results.
   double freqErrPPM;
   unsigned int capV;
   pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext, "XTAL_CAP_VALUE", buf);
   capV = atoi(buf);
   pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext, "XTAL_FREQERRPPM", buf);
   freqErrPPM = atof(buf);
   printf("cal freqErrPPM:%5.2f    freqErrPPM:%2d\n", freqErrPPM, capV );
}

// ======================================================================================================
/// <summary>
/// Setup TX for transmitting
/// </summary>
/// <param name="txMode">Tx Mode</param>
/// <param name="channel">channel number</param>
/// <param name="rateBitIndex0">Tx Data Rate</param>
/// <param name="wlanMode">wlan Mode</param>
/// <param name="pktLen0">Packet length or PayLoad Size</param>
/// <param name="txChain0">Tx Chain to turn on</param>
/// <param name="shortGuard">true for short guard (11n/11ac) or 11b short</param>
/// <param name="numPackets" Number of packets, 0 = continuous</param>
/// <param name="broadcast">True for broadcast, false for Unicast</param>
/// <param name="flags">flags=0x18(24) with Bit OR combination of ldpcRate=2, dpdMode=4, STBCmode=1</param>
/// <param name="gainIdx">Gain Index</param>
/// <param name="dacGain">Dac gain</param>
/// <param name="paConfig">PA configuration</param>
/// <param name="tpcm">TX_PWR=0, FORCED_GAIN=1, TGT_PWR=2, TX_FORCED_GAIN=3, FORCED_GAINIDX=4, FORCED_TGTPWR=5</param>
void StartTx(int phyRFMode, int phyId, int txMode,int channel,int channel2,int rateBitIndex0,int wlanMode,int pktLen0,int txChain0,int shortGuard,int numPackets,int broadcast,int flags,int gainIdx,int dacGain,int paConfig,int tpcm)
{
   char buf[32];

   //80-WL400-11 Refer Section "Steps to do WLAN RF transmit tests"
   //            Refer Table "QRCT steps to perform the WLAN transmit test"

   if (QLIB_WLAN_MULTIPHY_SUPPORT)
   {
      pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP2_SETPHYRFMODE);
      pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"phyRFMode",_itoa(phyRFMode,buf,10));
      pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
   }

   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_TX);
   if (QLIB_WLAN_MULTIPHY_SUPPORT)
      pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"phyId",_itoa(phyId,buf,10));

   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel",_itoa(channel,buf,10));
   if (channel2 !=0)
     pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel2",_itoa(channel2,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txMode",_itoa(txMode,buf,10));                //3 = Tx99
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rateBitIndex0", _itoa(rateBitIndex0,buf,10));   //Please refer WLAN_tlvDataRateTX, MCS 6 HT20
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"wlanMode", _itoa(wlanMode,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"pktLen0", _itoa(pktLen0,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txChain0", _itoa(txChain0,buf,10));             //1(ch0),2(ch1),3(ch01)
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"shortGuard", _itoa(shortGuard,buf,10));
   if(0 != strncmp(chip_select, KINGFISHER_DLLID, sizeof(KINGFISHER_DLLID)))
       pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"numPackets", _itoa(numPackets,buf,10));         //0 = continuous, otherwise number of packets
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"broadcast",broadcast ? "true" :"False");

   //flags = 0x18;
   //if (ldpcRate)  flags |= DESC_LDPC_ENA_MASK;  // 0x00000002
   //if (dpdMode)   flags |= PAPRD_ENA_MASK;      // 0x00000004
   //if (STBCmode)  flags |= DESC_STBC_ENA_MASK;  // 0x00000001
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"flags", _itoa(flags,buf,10));

   // tpcm TX_PWR=0, FORCED_GAIN=1, TGT_PWR=2, TX_FORCED_GAIN=3, FORCED_GAINIDX=4, FORCED_TGTPWR=5
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"tpcm", _itoa(tpcm,buf,10));
   if(0 != strncmp(chip_select, KINGFISHER_DLLID, sizeof(KINGFISHER_DLLID)))
       pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"gainIdx", _itoa(gainIdx,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"dacGain", _itoa(dacGain,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"paConfig", _itoa(paConfig,buf,10));
   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}

// ======================================================================================================
/// <summary>
/// Stop Tx
/// </summary>
/// <param name="channel">channel number for current Tx</param>
void StopTx(int channel)
{
   char buf[32];

   //80-WL400-11 Refer Section "Steps to do WLAN RF transmit tests"
   //            Refer Table "QRCT steps to perform the WLAN transmit test"
   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_TX);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel",_itoa(channel,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"txMode","0");   //0 = xmit off
   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}

// ======================================================================================================
/// <summary>
/// Stop Tx and get Total packets transmitted and RSSI
/// </summary>
void StopTxWithReport()
{
   char strData[128];

   //80-WL400-11 Refer Section "Steps to do WLAN RF transmit tests"
   //            Refer Table "QRCT steps to perform the WLAN transmit test"
   // Execute _OP_TX_STATUS will stop current RX
   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_TX_STATUS);
   if (QLIB_WLAN_MULTIPHY_SUPPORT)
    pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"needReport","1");

   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);

   printf ("=============================\n");
   if (QLIB_WLAN_MULTIPHY_SUPPORT)
      pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"goodPackets",strData);
   else
     pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"totalPkt",strData);

   printf("Tx Report : totalPkt   = %4s\n",strData);
   pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"rssi",strData);
   printf("Tx Report : rssi       = %4s\n",strData);
}

// ======================================================================================================
/// <summary>
/// Set up Rx for receiving
/// </summary>
/// <param name="channel">channel number</param>
/// <param name="rxMode">Rx Mode</param>
/// <param name="rateMask">Rx Data Rate</param>
/// <param name="rxChain">Rx Chain to turn on</param>
/// <param name="wlanMode"> wlan Mode</param>
void StartRx(int phyRFMode, int phyId, int channel, int channel2, int rxMode, int rateMask, int rxChain, int wlanMode)
{
   char buf[32];

   //80-WL400-11 Refer Section "Steps to do WLAN RF receive tests"
   //            Refer Table "QRCT steps to perform WLAN receive test"

   if (QLIB_WLAN_MULTIPHY_SUPPORT)
   {
      pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP2_SETPHYRFMODE);
      pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"phyRFMode",_itoa(phyRFMode,buf,10));
      pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
   }

   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_RX);
   if (QLIB_WLAN_MULTIPHY_SUPPORT)
      pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"phyId",_itoa(phyId,buf,10));
   if(0 != strncmp(chip_select, KINGFISHER_DLLID, sizeof(KINGFISHER_DLLID)))
       pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel",_itoa(channel,buf,10));
   if (channel2 != 0)
    pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"channel2",_itoa(channel2,buf,10));
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rxMode",_itoa(rxMode,buf,10));      //1 = filter mode
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rateMask", _itoa(rateMask,buf,10));   //Please refer WLAN_tlvDataRateRX,MCS 6 HT20
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"rxChain", _itoa(rxChain,buf,10));    //1(ch0),2(ch1),3(ch01)
   if(0 != strncmp(chip_select, KINGFISHER_DLLID, sizeof(KINGFISHER_DLLID)))
       pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"wlanMode", _itoa(wlanMode,buf,10));  //Refer TCMD_WLAN_MODE
   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}

// ======================================================================================================
/// <summary>
/// Stop Rx and get Total packets received, error packets and RSSI
/// </summary>
void StopRxWithReport()
{
   char strData[128];

   //80-WL400-11 Refer Section "Steps to do WLAN RF receive tests"
   //             Refer Table "QRCT steps to perform WLAN receive test"
   // Execute _OP_RX_STATUS will stop current RX
   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_RX_STATUS);
   if (QLIB_WLAN_MULTIPHY_SUPPORT)
    pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"stopRx","1");

   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);

   //Get Rx status report
   printf ("==============================\n");
   pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"totalPkt",strData);
   printf("Rx Report : totalPkt    = %4s\n",strData);
   pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"goodPackets",strData);
   printf("Rx Report : goodPackets = %4s\n",strData);
   pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"crcErrPkt",strData);
   printf("Rx Report : crcErrPkt   = %4s\n",strData);
   pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"secErrPkt",strData);
   printf("Rx Report : secErrPkt   = %4s\n",strData);
   pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"rssi",strData);
   printf("Rx Report : rssi        = %4s\n",strData);
}

//=====================================================================================
/// <summary>
/// Set BD Address
///
/// </summary>
void SetBDAddress(unsigned char *addr)
{
   QLIB_FTM_WLAN_Atheros_SET_BD_ADDRESS_IN_OTP(g_hResourceContext, addr);
}

//=====================================================================================
/// <summary>
/// Get BD Address
///
/// </summary>
void GetBDAddress(unsigned char *addr)
{
   QLIB_FTM_WLAN_Atheros_GET_BD_ADDRESS_FROM_OTP(g_hResourceContext, addr);
}

//=====================================================================================
/// <summary>
/// Set MAC Address
///
/// </summary>
void SetMacAddress(unsigned char *addr)
{
   char buffer[128];

   //convert MAC address to a string
   sprintf(buffer, "%c%c%c%c%c%c\0", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

   //create the command to send
   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_OTP);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_actions", "6"); //6 for NV Mac address set
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_MAC", buffer);
   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}

//=====================================================================================
/// <summary>
/// Get MAC Address
///
/// </summary>
void GetMacAddress(char *buffer)
{
   //create the command to send
   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_OTP);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_actions", "7"); //7 for NV Mac address get
   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
   pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext,"",buffer);
}

//=====================================================================================
/// <summary>
/// Set Top Configuration Value
///
/// </summary>
void setTopConfigValue(unsigned int action, unsigned int value)
{
   QLIB_FTM_WLAN_SET_TOP_CONFIG_IN_OTP(g_hResourceContext, action, value);
}

//=====================================================================================
/// <summary>
/// Get Top Configuration Value
///
/// </summary>
void getTopConfigValue(unsigned int action, unsigned int *value)
{
   QLIB_FTM_WLAN_GET_TOP_CONFIG_FROM_OTP(g_hResourceContext, action, value);
}

//=====================================================================================
/// <summary>
/// Set BT Configuration Value
///
/// </summary>
void setBTConfigValue(unsigned int action, unsigned int value)
{
   QLIB_FTM_WLAN_SET_BT_CONFIG_IN_OTP(g_hResourceContext, action, value);
}

//=====================================================================================
/// <summary>
/// Get BT Configuration Value
///
/// </summary>
void getBTConfigValue(unsigned int action, unsigned int *value)
{
   QLIB_FTM_WLAN_GET_BT_CONFIG_FROM_OTP(g_hResourceContext, action, value);
}

//=====================================================================================
/// <summary>
/// Set PCIe Configuration Value
///
/// </summary>
void setPCIeConfigValue(unsigned int action, unsigned int value)
{
   QLIB_FTM_WLAN_SET_PCIe_CONFIG_IN_OTP_STREAM(g_hResourceContext, action, value);
}

//=====================================================================================
/// <summary>
/// Get PCIe Configuration Value
///
/// </summary>
void getPCIeConfigValue(unsigned int action, unsigned int *value)
{
   QLIB_FTM_WLAN_GET_PCIe_CONFIG_FROM_OTP_STREAM(g_hResourceContext, action, value);
}

//=====================================================================================
/// <summary>
/// Commit OTP Stream
///
/// </summary>
void commitOTPStream(void)
{
   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_OTP);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_actions", "2"); //2 for NV Stream Commit
   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}

//=====================================================================================
/// <summary>
/// Set Regulatory Domain Values
///
/// </summary>
void setRegDomain(unsigned int *regDmn)
{
   QLIB_FTM_WLAN_Atheros_SET_OTP_REG_DOMAIN(g_hResourceContext, regDmn);
}

//=====================================================================================
/// <summary>
/// Get Regulatory Domain Values
///
/// </summary>
void getRegDomain(unsigned int *regDmn)
{
   QLIB_FTM_WLAN_Atheros_GET_OTP_REG_DOMAIN(g_hResourceContext, regDmn);
}

//=====================================================================================
/// <summary>
/// Dump OTP contents to file
///
/// </summary>
void dumpOTPToFile(char *filename)
{
   QLIB_FTM_WLAN_Atheros_DumpOTPToFile(g_hResourceContext, filename);
}

//=====================================================================================
/// <summary>
/// Compare OTP Contents against File
///
/// </summary>
void CompareOTPAgainstFile(char *filename, char *resultsFilename, unsigned int *result)
{
   QLIB_FTM_WLAN_Atheros_CompareOTPAgainstFile(g_hResourceContext, filename, resultsFilename, result);
}

//=====================================================================================
/// <summary>
/// Read OTP Location (single Byte only)
///
/// </summary>
void readOTPLocation(unsigned int address, unsigned int *value)
{
   char buffer[128];

   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_OTP);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_actions", "0"); //0 for NVMEM read
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_mem", "3");     //set NV memory type as OTP(3)
   sprintf(buffer, "%d\0", address);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_address", buffer);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_size", "1"); //1 byte
   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);

   pQLIB_FTM_WLAN_TLV_GetRspParam(g_hResourceContext, "", buffer);
   sscanf(buffer, "%x", value);
}

//=====================================================================================
/// <summary>
/// Write to OTP Location (single byte only)
///
/// </summary>
void writeOTPLocation(unsigned int address, unsigned int value)
{
   char buffer[128];

   pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_OTP);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_actions", "1"); //0 for NVMEM write
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_mem", "3");     //set NV memory type as OTP(3)
   sprintf(buffer, "%d\0", address);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_address", buffer);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_size", "1"); //1 byte
   sprintf(buffer, "%d\0", value);
   pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext, "NVMEM_value", buffer);
   pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
}

//=====================================================================================
/// <summary>
/// Setup for Tx testing, start Tx, stop Tx with user selected parameters
/// 80-WL400-11 Section 2.2 WLAN RF transmit test  set DUT to FTM Test mode
/// </summary>
void Test_FTM_QCA_WLAN_TX()
{
   // 80-WL400-11 Refer Section Tx test configuration in 2.4G
   //                       Tx test configuration in 5G (HT20)
   //                           Tx test configuration in 5G (HT40)
   int sel;
   int txMode = 3; // Tx99
   int channel = 2422;
   int channel2 = 0;
   int rateBitIndex0 = 22; //Refer WLAN_tlvDataRateTX, MCS 6 HT20
   int wlanMode = 1;
   int pktLen0 = 1500;
   int txChain0 = 1;
   int shortGuard = 0;
   int numPackets = 2000;
   int broadcast = 0;  // true, false
   int flags = 28;
   int tpcm = 4;
   int gainIdx = 5;
   int dacGain = 0;
   int paConfig = 1;
   int phyId = 0;         // 0=PhyA, 1=PhyB
   int phyRFMode = 1;     // 0=DBS, 1=PhyA_only, 2=PhyB_only, 3=SBS

   if (QLIB_WLAN_80P80_SUPPORT) 
   {
       //For Cascade sample parameter
       channel       = 5210;
       channel2      = 5500;
       rateBitIndex0 = 224;
       wlanMode      = 12;
       txChain0      = 5;
       flags         = 24;
       gainIdx       = 13;
       dacGain       = -8;
       paConfig      = 7;
   }

   if (QLIB_WLAN_MULTIPHY_SUPPORT)
   {
      //For Adrastea sample parameter
       channel       = 5825;
       channel2      = 0;
       rateBitIndex0 = 22;
       wlanMode      = 12;
       txChain0      = 1;
       flags         = 24;
       gainIdx       = 13;
       dacGain       = 0;
       paConfig      = 3;
      numPackets    = 0;
   }

   // Cascade Data Rates TX=========
   // tlvRATE_AC_MCS_0_160_1S = 224
   // tlvRATE_AC_MCS_1_160_1S = 225
   // tlvRATE_AC_MCS_2_160_1S = 226
   // tlvRATE_AC_MCS_3_160_1S = 227
   // tlvRATE_AC_MCS_4_160_1S = 228
   // tlvRATE_AC_MCS_5_160_1S = 229
   // tlvRATE_AC_MCS_6_160_1S = 230
   // tlvRATE_AC_MCS_7_160_1S = 231
   // tlvRATE_AC_MCS_8_160_1S = 232
   // tlvRATE_AC_MCS_9_160_1S = 233
   //
   // tlvRATE_AC_MCS_0_160_2S = 234
   // tlvRATE_AC_MCS_1_160_2S = 235
   // tlvRATE_AC_MCS_2_160_2S = 236
   // tlvRATE_AC_MCS_3_160_2S = 237
   // tlvRATE_AC_MCS_4_160_2S = 238
   // tlvRATE_AC_MCS_5_160_2S = 239
   // tlvRATE_AC_MCS_6_160_2S = 240
   // tlvRATE_AC_MCS_7_160_2S = 241
   // tlvRATE_AC_MCS_8_160_2S = 242
   // tlvRATE_AC_MCS_9_160_2S = 243

   // Cascade cbState/wlanMode========
   // BW80p80 = 12,
   // Primary_1st_20_in_BW80p80 = 21
   // Primary_2nd_20_in_BW80p80 = 22
   // Primary_3rd_20_in_BW80p80 = 23
   // Primary_4th_20_in_BW80p80 = 24
   // Primary_5th_20_in_BW80p80 = 25
   // Primary_6th_20_in_BW80p80 = 26
   // Primary_7th_20_in_BW80p80 = 27
   // Primary_8th_20_in_BW80p80 = 28

   do {
      printf ("=======================\n");
      printf (" 1:txMode        = %4d\n",txMode);
      printf (" 2:channel       = %4d\n",channel);
      printf (" 3:rateBitIndex0 = %4d\n",rateBitIndex0);
      printf (" 4:wlanMode      = %4d\n",wlanMode);
      printf (" 5:pktLen0       = %4d\n",pktLen0);
      printf (" 6:txChain0      = %4d\n",txChain0);
      printf (" 7:shortGuard    = %4d\n",shortGuard);
      printf (" 8:numPackets    = %4d\n",numPackets);
      printf (" 9:broadcast     = %4d\n",broadcast);
      printf ("10:gainIdx       = %4d\n",gainIdx);
      printf ("11:dacGain       = %4d\n",dacGain);
      printf ("12:paConfig      = %4d\n",paConfig);
      printf ("13:tcpm          = %4d\n",tpcm);
      if (QLIB_WLAN_80P80_SUPPORT)
      {
        printf ("14:channel2      = %4d\n",channel2);
      }
     if (QLIB_WLAN_MULTIPHY_SUPPORT)
     {
        printf ("15:phyRFMode     = %4d\n",phyRFMode);
        printf ("16:phyId         = %4d\n",phyId);
     }

      printf ("=======================\n");
      printf (" 0:Start TX, 99:Exit\n");
      printf("Please enter selection\n");
      scanf("%d",&sel);
      switch (sel)
      {
      case  1:printf("Please enter new txMode\n");
         scanf("%d",&txMode);
         break;
      case  2:printf("Please enter new channel\n");
         scanf("%d",&channel);
         break;
      case  3:printf("Please enter new rateBitIndex0\n");
         scanf("%d",&rateBitIndex0);
         break;
      case  4:printf("Please enter new wlanMode\n");
         scanf("%d",&wlanMode);
         break;
      case  5:printf("Please enter new pktLen0\n");
         scanf("%d",&pktLen0);
         break;
      case  6:printf("Please enter new txChain0\n");
         scanf("%d",&txChain0);
         break;
      case  7:printf("Please enter new  shortGuard\n");
         scanf("%d",&shortGuard);
         break;
      case  8:printf("Please enter new numPackets\n");
         scanf("%d",&numPackets);
         break;
      case  9:printf("Please enter new broadcast\n");
         scanf("%d",&broadcast);
         break;
      case 10:printf("Please enter new gainIdx\n");
         scanf("%d",&gainIdx);
         break;
      case 11:printf("Please enter new dacGain\n");
         scanf("%d",&dacGain);
         break;
      case 12:printf("Please enter new paConfig\n");
         scanf("%d",&paConfig);
         break;
      case 13:printf("Please enter new tpcm (TX_PWR=0, FORCED_GAIN=1, TGT_PWR=2, TX_FORCED_GAIN=3, FORCED_GAINIDX=4,FORCED_TGTPWR=5)\n");
         scanf("%d",&tpcm);
         break;
      case 14:
         if (QLIB_WLAN_80P80_SUPPORT)
         {
             printf("Please enter new channel2 (For 80p80 only)\n");
             scanf("%d",&channel2);
         } 
         else 
         {
             printf("Invalid selection, please try again!\n");
         }
         break;
     case 15:
       if (QLIB_WLAN_MULTIPHY_SUPPORT)
         {
          printf("Please enter new phyRFMode (0=DBS, 1=PhyA, 2=phyB, 3=SBS)\n");
             scanf("%d",&phyRFMode);
         }
         else
         {
             printf("Invalid selection, please try again.\n");
         }
        break;
     case 16:
       if (QLIB_WLAN_MULTIPHY_SUPPORT)
         {
             printf("Please enter new phyId (0=phyA, 1=PhyB)\n");
             scanf("%d",&phyId);
         }
         else
         {
             printf("Invalid selection, please try again.\n");
         }
        break;
      case  0:StartTx(phyRFMode, phyId, txMode,channel,channel2, rateBitIndex0,wlanMode,pktLen0,txChain0,shortGuard,numPackets,broadcast,flags,gainIdx,dacGain,paConfig,tpcm);
         int txSel;
         do {
            printf("Please enter 1:Stop Tx, 2: Stop Tx and Get Tx Report, 3:Continue Tx\n");
            scanf("%d",&txSel);
            switch (txSel)
            {
            case 1: StopTx(channel);
               break;
            case 2: StopTxWithReport();
               break;
            case 3: // do nothing
               break;
            default:printf("Invalid selection, please try again.\n");
            }
         } while ((txSel < 1) || (txSel > 3));
         break;
      case 99:
         break;
      default:printf("Invalid selection, please try again!\n");
      }
   } while (sel != 99);
}

//=====================================================================================
/// <summary>
/// Setup for Rx testing, start Rx, stop Rx with user selected parameters
/// 80-WL400-11 Section 3.2 WLAN RF Receive test  set DUT to FTM Test mode
/// </summary>
void Test_FTM_QCA_WLAN_RX()
{
   int sel;

   // 80-WL400-11 Section Receiver minimum input sensitivity configuration
   //                     Receiver maximum input level configuration
   int channel = 2422;
   int channel2 = 0;
   int rxMode = 1;      // filter mode
   int rateMask = 21;   //Refer WLAN_tlvDataRateRX ,MCS 6 HT20
   int rxChain = 1;
   int wlanMode = 0;
   int phyId = 0;       // 0=PhyA, 1=PhyB
   int phyRFMode = 1;   // 0=DBS, 1=PhyA_only, 2=PhyB_only, 3=SBS

   if (QLIB_WLAN_80P80_SUPPORT)
   {
       //For Cascade sample parameter
       channel       = 5210;
       channel2      = 5500;
       rateMask      = 183;
       wlanMode      = 12;
       rxChain       = 5;
   } 

   if (QLIB_WLAN_MULTIPHY_SUPPORT)
   {
      //For Adrastea sample parameter
       channel       = 5825;
       channel2      = 0;
      rxChain       = 1;
   }
   
   // Cascade Data Rate RX========
   // RATE_11AC_160_MCS0     = 183
   // RATE_11AC_160_MCS1     = 184
   // RATE_11AC_160_MCS2     = 185
   // RATE_11AC_160_MCS3     = 186
   // RATE_11AC_160_MCS4     = 187
   // RATE_11AC_160_MCS5     = 188
   // RATE_11AC_160_MCS6     = 189
   // RATE_11AC_160_MCS7     = 190
   // RATE_11AC_160_MCS8     = 191
   // RATE_11AC_160_MCS9     = 192
   // RATE_11AC_160_MCS10    = 193
   // RATE_11AC_160_MCS11    = 194
   // RATE_11AC_160_MCS12    = 195
   // RATE_11AC_160_MCS13    = 196
   // RATE_11AC_160_MCS14    = 197
   // RATE_11AC_160_MCS15    = 198
   // RATE_11AC_160_MCS16    = 199
   // RATE_11AC_160_MCS17    = 200
   // RATE_11AC_160_MCS18    = 201
   // RATE_11AC_160_MCS19    = 202

   // Cascade cbState/wlanMode========
   // BW80p80 = 12,
   // Primary_1st_20_in_BW80p80 = 21
   // Primary_2nd_20_in_BW80p80 = 22
   // Primary_3rd_20_in_BW80p80 = 23
   // Primary_4th_20_in_BW80p80 = 24
   // Primary_5th_20_in_BW80p80 = 25
   // Primary_6th_20_in_BW80p80 = 26
   // Primary_7th_20_in_BW80p80 = 27
   // Primary_8th_20_in_BW80p80 = 28


   do {
      printf ("=======================\n");
      printf (" 1:rxMode        = %4d\n",rxMode);
      printf (" 2:channel       = %4d\n",channel);
      printf (" 3:rateMask      = %4d\n",rateMask);
      printf (" 4:wlanMode      = %4d\n",wlanMode);
      printf (" 5:rxChain       = %4d\n",rxChain);
      if (QLIB_WLAN_80P80_SUPPORT)
      {
          printf (" 6:channel2      = %4d\n",channel2);
      }
     if (QLIB_WLAN_MULTIPHY_SUPPORT)
     {
        printf (" 7:phyRFMode     = %4d\n",phyRFMode);
        printf (" 8:phyId         = %4d\n",phyId);
     }
      printf ("=======================\n");
      printf (" 0:Start RX, 99:Exit\n");
      printf("Please enter selection\n");
      scanf("%d",&sel);
      switch (sel)
      {
      case  1:printf("Please enter new rxMode\n");
         scanf("%d",&rxMode);
         break;
      case  2:printf("Please enter new channel\n");
         scanf("%d",&channel);
         break;
      case  3:printf("Please enter new rateMask\n");
         scanf("%d",&rateMask);
         break;
      case  4:printf("Please enter new wlanMode\n");
         scanf("%d",&wlanMode);
         break;
      case  5:printf("Please enter new rxChain\n");
         scanf("%d",&rxChain);
         break;
      case  6:
         if (QLIB_WLAN_80P80_SUPPORT)
         {
             printf("Please enter new channel2 (For 80p80 only)\n");
             scanf("%d",&channel2);
         }
         else
         {
             printf("Invalid selection, please try again.\n");
         }
         break;
     case 7:
       if (QLIB_WLAN_MULTIPHY_SUPPORT)
         {
             printf("Please enter new phyRFMode (0=DBS, 1=PhyA, 2=phyB, 3=SBS)\n");
             scanf("%d",&phyRFMode);
         }
         else
         {
             printf("Invalid selection, please try again.\n");
         }
        break;
     case 8:
       if (QLIB_WLAN_MULTIPHY_SUPPORT)
         {
             printf("Please enter new phyId (0=phyA, 1=PhyB)\n");
             scanf("%d",&phyId);
         }
         else
         {
             printf("Invalid selection, please try again.\n");
         }
        break;

      case  0:StartRx(phyRFMode, phyId, channel, channel2, rxMode, rateMask, rxChain, wlanMode);
         int txSel;
         do {
            printf("Please enter 1:Stop Rx and Get Rx Report, 2:Continue Rx\n");
            scanf("%d",&txSel);
            switch (txSel)
            {
            case 1: StopRxWithReport();
               break;
            case 2: // do nothing
               break;
            default:printf("Invalid selection, please try again.\n");
            }
         } while ((txSel < 1) || (txSel > 2));
         break;
      case 99:
         break;
      default:printf("Invalid selection, please try again!\n");
      }
   } while (sel != 99);
}

//=====================================================================================
/// <summary>
/// Prompt for MAC or BD address from user
///
/// </summary>
int promtForAddress(unsigned char *address)
{
   char buffer[256];
   int temp;
   char delimin[] = " :"; //allow space or : as deliminter
   char *token;
   int index = 0;

   printf("Please enter address in the format 11:22:33:44:55:66\n");
   scanf("%s",&buffer);
   if(strlen((char *)buffer) != 17){
      printf("Error: address must be in the format 11:22:33:44:55:66 and must be 17 characters long\n");
      return 0;
   }
   token = strtok(buffer, delimin);
   while (token) {
      sscanf(token, "%x", &temp);
      address[index] = temp & 0xff;
      token = strtok( NULL, delimin);
      index++;
   }
   return 1;
}

//=====================================================================================
/// <summary>
/// Setup for OTP Access testing.  Exercise numerous OTP QMSL functions with user input
///
/// </summary>
void Test_FTM_QCA_WLAN_OTP()
{
   int sel;
   unsigned char addr[6];
   unsigned int action;
   unsigned int value;
   char buffer[128];
   unsigned int regDmn[2];

   do {
      printf ("=======================\n");
      printf (" 1:Set BD Address\n");
      printf (" 2:Get BD Address\n");
      printf (" 3:Set Top Configuration\n");
      printf (" 4:Get Top Configuration\n");
      printf (" 5:Set BT Configuration\n");
      printf (" 6:Get BT Configuration\n");
      printf (" 7:Set PCIe Configuration\n");
      printf (" 8:Get PCIe Configuration\n");
      printf (" 9:Commit OTP Stream\n");
      printf (" 10:Set Reg Domain\n");
      printf (" 11:Get Reg Domain\n");
      printf (" 12:Set MAC Address\n");
      printf (" 13:Get MAC Address\n");
      printf (" 14:Dump OTP To File\n");
      printf (" 15:Compare DUT OTP Against File\n");
      printf (" 16:Read OTP Location\n");
      printf (" 17:Write OTP Location\n");
      printf ("=======================\n");
      printf (" 99:Exit\n");
      printf("Please enter selection\n");
      scanf("%d",&sel);
      switch (sel)
      {
      case  1:if(!promtForAddress(addr))
              {
                 printf("Error setting the BD address\n");
                 break;
              }
              SetBDAddress(addr);
              break;
      case  2:GetBDAddress(addr);
         printf("BD Address = %x:%x:%x:%x:%x:%x\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
         break;
      case  3:do {
         printf("Please enter 0:xtal settle time, 1:xtal cap in, 2:xtal cap out, 3:xtal sel, 4:SDIO VID, 5:SDIO DID\n");
         scanf("%d",&action);
              } while (action > 5);
         printf("Please enter value to apply\n");
         scanf("%x",&value);
         setTopConfigValue(action, value);
         break;
      case  4:do {
         printf("Please enter 0:xtal settle time, 1:xtal cap in, 2:xtal cap out, 3:xtal sel, 4:SDIO VID, 5:SDIO DID\n");
         scanf("%d",&action);
              } while (action > 5);
         getTopConfigValue(action, &value);
         printf("Value is set to 0x%x\n", value);
         break;
      case  5:do {
         printf("Please enter 0:PID DFU, 1:VID DFU, 2:PID FW, 3:VID FW, 4:PID HID, 5:VID HID\n");
         scanf("%d",&action);
              } while (action > 5);
         printf("Please enter value to apply\n");
         scanf("%x",&value);
         setBTConfigValue(action, value);
         break;
      case  6:do {
         printf("Please enter 0:PID DFU, 1:VID DFU, 2:PID FW, 3:VID FW, 4:PID HID, 5:VID HID\n");
         scanf("%d",&action);
              } while (action > 5);
         getBTConfigValue(action, &value);
         printf("Value is set to 0x%x\n", value);
         break;
      case 7:do {
         printf("Please enter 0:SSID, 1:SVID\n");
         scanf("%d",&action);
             } while (action > 1);
         printf("Please enter value to apply\n");
         scanf("%x",&value);
         setPCIeConfigValue(action, value);
         break;
      case  8:do {
         printf("Please enter 0:SSID, 1:SVID\n");
         scanf("%d",&action);
              } while (action > 1);
         getPCIeConfigValue(action, &value);
         printf("Value is set to 0x%x\n", value);
         break;
      case  9:commitOTPStream();
         break;
      case 10:printf("Please enter regulatory Domain[0]\n");
         scanf("%x",&regDmn[0]);
         regDmn[0]&=0xffff;
         printf("Please enter regulatory Domain[1]\n");
         scanf("%x",&regDmn[1]);
         regDmn[1]&=0xffff;
         setRegDomain(regDmn);
         break;
      case  11:regDmn[0]=0; regDmn[1]=0;
         getRegDomain(regDmn);
         printf("Value of regDmn[0]=0x%x. Value of regDmn[1]=0x%x\n", regDmn[0], regDmn[1]);
         break;
      case  12:if(!promtForAddress(addr))
               {
                  printf("Error setting the MAC address\n");
                  break;
               }
               SetMacAddress(addr);
               break;
      case  13:GetMacAddress(buffer);
         printf("MAC Address = %s\n", buffer);
         break;
      case  14:dumpOTPToFile(OTP_DUMP_FILE);
         break;
      case  15:CompareOTPAgainstFile(OTP_DUMP_FILE, OTP_RESULTS_FILE, &value);
         if(value==0) {
            printf("Compare Passed!\n");
         } else {
            printf("Compare FAILED! Refer to %s for more information\n", OTP_RESULTS_FILE);
         }
         break;
      case 16:printf("Please enter OTP location to read\n");
         scanf("%x",&action);
         readOTPLocation(action, &value);
         printf("Value read 0x%x\n", value);
         break;
      case 17:printf("Please enter OTP location to write to\n");
         scanf("%x",&action);
         printf("Please enter the byte to write to this location\n");
         scanf("%x",&value);
         writeOTPLocation(action, (value&0xff));
         break;
      case 99:
         break;
      default:printf("Invalid selection, please try again!\n");
      }
   } while (sel != 99);
}

//=====================================================================================
/// <summary>
/// 80-WL400-13 Section 2.2 ROME OLPC Power Calibration sample code
/// </summary>
void Test_FTM_QCA_WLAN_CAL()
{
   int calType;

   printf ("=======================\n");
   printf (" 0: OLPC Cal\n");
   printf (" 1: CLPC Cal\n");
   printf (" 2: SCPC Cal    (ROME3.0 up)\n");
   printf (" 3: XTAL Cal\n");
   printf("Please enter to select a calibration type\n");
   scanf("%d",&calType);

   int iXtalTargetPPM=0;   // XTAL Cal Target Freq. error in PPM; default 0 PPM
   unsigned int iXtalTolerancePPM=1;   // XTAL Cal tolerable Freq. error in PPM
   int iXtalCapDelta=0; //  the delta value between the capout and capin.
   unsigned int iXtalCalSaveOption=0; // 0: not save xtal result to otp. 1: save xtal result to otp.
   
   if (calType==0) {      // OLPC
      //80-WL400-13 Refer 2.3.1.2 ROME OLPC Power Cal Initialization QMSL Calling Sequence
      int iCalPoints = 5;
      int iCalGainMode = 0;
      QLIB_FTM_WLAN_Atheros_Tx_CALIBRATE_INIT(g_hResourceContext, (unsigned char*)"cal-5p", iCalPoints, iCalGainMode);
   } else if (calType==1) {    // CLPC
      //80-WL400-13 Refer 3.2.1 ROME CLPC Power Cal Initialization QMSL Calling Sequence
      pQLIB_FTM_WLAN_TLV_Create(g_hResourceContext, _OP_CAL_INIT);
      pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"CalScheme", "3");
      // 3: clpc  use internal Pdet and Ext Coupler with multiple points calibration
      // 4: clpc  use Ext Pdet with FEM with multiple points calibration
      pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"CalMode", "1");
      pQLIB_FTM_WLAN_TLV_AddParam(g_hResourceContext,"CalPoints", "10");
      pQLIB_FTM_WLAN_TLV_Complete(g_hResourceContext);
   } else if (calType==2) {    // SCPC
   } else if (calType==3) {    // XTAL
      //80-WL400-13 Refer 4.2.1 ROME XTAL Cal Initialization QMSL Calling Sequence
      QLIB_FTM_WLAN_Atheros_xtal_CAL_INIT(g_hResourceContext, iXtalTargetPPM, iXtalTolerancePPM, iXtalCapDelta, iXtalCalSaveOption);
   }

   //80-WL400-13 Refer 2.2.2 ROME OLPC Power Cal QMSL Calling Sequence

   int sel;
   // the following Tx setup for OLPC can be selected by user
   int channel = 2412;
   int txChain0 = 1;
   int maxIt=3;
   double measPwrDelta = 1.0;
   int numMeasAvg=1;

   do {
      printf ("=======================\n");
      printf (" 1:channel             = %4d\n",channel);
      printf (" 2:txChain0            = %4d\n",txChain0);
      printf ("=======================\n");
      printf (" 99:Exit\n");
      printf (" 0:Start Cal\n");
      printf("Please enter selection\n");
      scanf("%d",&sel);
      switch (sel)
      {
      case  1:printf("Please enter new channel\n");
         scanf("%d",&channel);
         break;
      case  2:printf("Please enter new txChain0\n");
         scanf("%d",&txChain0);
         break;
      case  0:printf("Calibration Start\n");
         if (calType==0) // OLPC
            TxOLPCCal(channel, txChain0, measPwrDelta, maxIt, numMeasAvg);
         else if (calType==1) // CLPC
            TxCLPCCal(channel, txChain0, measPwrDelta, maxIt, numMeasAvg);
         else if (calType==3) // XTAL
            XTALCal(channel, txChain0, iXtalTargetPPM, iXtalTolerancePPM, numMeasAvg);
         break;
      case 99:
         break;
      default:printf("Invalid selection, please try again!\n");
      }
   } while (sel != 99);
}

//=====================================================================================
/// <summary>
/// WLAN x86 testing menu for Tx, Rx and OTP
/// </summary>
void Test_FTM_QCA_WLAN()
{
   int sel;

   do
   {
      printf("\n===============================\n");
      printf("Please enter 1:TX, 2:RX, 3:OTP, 4:Tx Calibration, 99:Exit\n");
      scanf("%d",&sel);

      switch (sel)
      {
      case 1 :Test_FTM_QCA_WLAN_TX();
         break;
      case 2 :Test_FTM_QCA_WLAN_RX();
         break;
      case 3 :Test_FTM_QCA_WLAN_OTP();
         break;
      case 4 :Test_FTM_QCA_WLAN_CAL();
         break;
      case 99 :// do nothing
         break;
      default:
         printf("Invalid selection! Please 1, 2, 3 or 4\n");
      }
   } while (sel != 99);
}

//=====================================================================================
/// <summary>
/// WLAN x86 testing menu for load DUT
/// </summary>
int Test_FTM_QCA_LOADDUT()
{
   int sel;
   char *bin_file;
   char *dllID;
   unsigned int chipID = 0;

   QLIB_WLAN_80P80_SUPPORT = 0;
   QLIB_WLAN_MULTIPHY_SUPPORT = 0;

   while (chipID == 0)
   {
      printf("\n===============================\n");
      printf("Please enter 1:ROME, 2:BEELINER, 3:DAKOTA, 4:CASCADE, 5:BESRA, 6:ADRASTEA, 7:KingFisher\n");
      scanf("%d",&sel);

     pQLIB_FTM_WLAN_TLV_Create      = &QLIB_FTM_WLAN_TLV_Create;
     pQLIB_FTM_WLAN_TLV_AddParam    = &QLIB_FTM_WLAN_TLV_AddParam;
      pQLIB_FTM_WLAN_TLV_Complete    = &QLIB_FTM_WLAN_TLV_Complete;
     pQLIB_FTM_WLAN_TLV_GetRspParam = &QLIB_FTM_WLAN_TLV_GetRspParam;

      switch (sel)
      {
        case 1 :
            dllID = ROME_DLLID;
            chipID = ROME_CHIPID;
            bin_file = ROME_BINFILE;
        break;
        case 2 :
            dllID = BEELINER_DLLID;
            chipID = BEELINER_CHIPID;
            bin_file = BEELINER_BINFILE;
        break;
        case 3 :        
            dllID = DAKOTA_DLLID;
            chipID = DAKOTA_CHIPID;
            bin_file = DAKOTA_BINFILE;
        break;
        case 4 :
            dllID = CASCADE_DLLID;
            chipID = CASCADE_CHIPID;
            bin_file = CASCADE_BINFILE;
            QLIB_WLAN_80P80_SUPPORT = 1;
        break;
        case 5 :
            dllID = BESRA_DLLID;
            chipID = BESRA_CHIPID;
            bin_file = BESRA_BINFILE;
            QLIB_WLAN_80P80_SUPPORT = 1;
        break;
        case 6 :
            dllID = ADRASTEA_DLLID;
            chipID = ADRASTEA_CHIPID;
            bin_file = ADRASTEA_BINFILE;
            QLIB_WLAN_80P80_SUPPORT = 1;
         QLIB_WLAN_MULTIPHY_SUPPORT = 1;
         pQLIB_FTM_WLAN_TLV_Create      = &QLIB_FTM_WLAN_TLV2_Create;
         pQLIB_FTM_WLAN_TLV_AddParam    = &QLIB_FTM_WLAN_TLV2_AddParam;
            pQLIB_FTM_WLAN_TLV_Complete    = &QLIB_FTM_WLAN_TLV2_Complete;
         pQLIB_FTM_WLAN_TLV_GetRspParam = &QLIB_FTM_WLAN_TLV2_GetRspParam;
        break;
	case 7:
	    dllID = KINGFISHER_DLLID;
	    chipID = KINGFISHER_CHIPID;
	    bin_file = KINGFISHER_BINFILE;
	    chip_select = KINGFISHER_DLLID;
	break;
        default:
            printf("Invalid selection! Please 1, 2, 3, 4, 5, 6, 7\n");
      }
   }

   // use user input board data.
   if (strlen(BIN_FILE))
    bin_file = BIN_FILE;

   if (!LoadDut(dllID, bin_file, chipID))
   {
      printf("LoadDut() failed!\n");
      DisconnectStandaloneWlanCard();
      return (-2);
   }   

   return 0;
}

// ====================================================================================
/// <summary>
/// Setup for loading User Defined Transport, DUT, perform testing and unload
// For Adrastea : QMSL_Demo_MSVC10R -3 COM40 
//                                     This is MTP COM port number
/// </summary>
int Test_WLAN_x86(int argc, char** argv)
{
   hUDT = NULL;
   g_hResourceContext = NULL;
   int COM_Select_Flag = 0;

   printf("Connect Dut Type using Lite or Standard: 0: Standard    1: Lite\n");
       	scanf("%s", &COM_Select_Flag);

   BIN_FILE[0] = '\0';
   if (argc >= 3) {
      strcpy(UDT_IP, argv[2]);
      if (argc >= 4)
         strcpy(BIN_FILE, argv[3]);
   }

   if(COM_Select_Flag) {
   	if(!ConnectDiagTypeLiteWlanCard())
   {
		printf("ConnectDiagTypeLiteWlanCard() failed!\n");
      return(-1);
   }

   }
   else{
   	if (!ConnectStandaloneWlanCard())
   	{
      		printf("ConnectStandaloneWlanCard() failed!\n");
      		return(-1);
   	}
   }

	if (Test_FTM_QCA_LOADDUT() != 0)
   	{
      return (0);
   }

   Test_FTM_QCA_WLAN();

   CloseDut();
   DisconnectStandaloneWlanCard();

   return(0);
}


