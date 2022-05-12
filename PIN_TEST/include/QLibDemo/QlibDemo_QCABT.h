/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/QlibDemo_QCABT.h#1 $
 * $DateTime: 2017/03/13 10:17:17 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2017 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#ifndef _QLIBDEMO_QCABT_H_ 
#define _QLIBDEMO_QCABT_H_ 
#include "windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#include "QMSL_Core.h"
#include "QLib_Defines.h"

#define MAX_COMPORT_LENGTH = 5;
#define USER_HANDLE   1

// Loopback IP for local system or any remote system IP with DUT
char UDT_BT_IP[20] = "127.0.0.1";
char UDT_BT_PORT[20] = "2390";

unsigned short iCSRPacketTypeValue = 0;

int Test_BT_x86(int argc, char** argv);
bool ConnectStandaloneBTDevice(char *sComPort);
void Test_FTM_QCA_BT(void);
int Test_FTM_BT_EPTM();
int Test_FTM_BT_TxRxBurst(char burstType);
int Test_FTM_BT_RxBurstConfig();
int Test_FTM_BT_RxTestStats();
int Test_BT_CSR();
int Test_FTM_BTCSR_PacketType();
int Test_FTM_BTCSR_SetBDAddr();
int Test_FTM_BTCSR_SetTxPwr();
int Test_FTM_BTCSR_TxData();
int Test_FTM_BTCSR_XTAL();
int Test_BT_LE();
int BLE_RxTest();
int BLE_StartRxTest();
int BLE_EndRxTest();
int BLE_TxTest();
void DisconnectStandaloneBTDevice(char *dutIP);

#define QLIB_API __declspec(dllimport)
extern "C" 
{
   QLIB_API unsigned char QLIB_FTM_BT_HCI_Reset(HANDLE hResourceContext);
   QLIB_API unsigned char QLIB_FTM_BT_Enable_Bluetooth( HANDLE hResourceContext );
   QLIB_API unsigned char QLIB_FTM_BT_SET_CHIP_ID ( HANDLE hResourceContext, int bluetoothChipID);
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_TX(HANDLE hResourceContext,
         unsigned char ihopChannels[5],
         unsigned char itransmitPattern,
         unsigned char ipacketType,
         unsigned char idataWhiteningEnable,
         unsigned char itransitOutputPower,
         unsigned char ireceiverHighGainEnable,
         unsigned char itargetDeviceAddress[6],
         unsigned char ihoppingEnable,
         unsigned short ipayloadLength,
         unsigned char ilogicalTransportAddress);
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_RX_ONLY( HANDLE hResourceContext,
         unsigned char ihopChannels[5],
         unsigned char itransmitPattern,
         unsigned char ipacketType,
         unsigned char idataWhiteningEnable,
         unsigned char itransitOutputPower,
         unsigned char ireceiverHighGainEnable,
         unsigned char itragetDeviceAddress[6],
         unsigned char ihoppingEnable,
         unsigned short ipayloadLength,
         unsigned char ilogicalTransportAddress);
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_STATS(HANDLE hResourceContext,
         unsigned char *channel1Stats,
         unsigned char *channel2Stats,
         unsigned char *channel3Stats,
         unsigned char *channel4Stats,
         unsigned char *channel5Stats);
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_RX_BURST_CONFIG( HANDLE hResourceContext,
         unsigned short iNumberofPacketsToReceive);


   //CSR APIs
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_WarmReset(  HANDLE hResourceContext );
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_Pause(  HANDLE hResourceContext );
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_SetPacketType(  HANDLE hResourceContext, 
         unsigned short iPacketType , 
         unsigned short iPayloadLength );
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_SET_BD_ADDR( HANDLE hResourceContext, 
         unsigned char UAP, 
         unsigned char LAP[3] );
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_SetTxPower( HANDLE hResourceContext, 
         unsigned char iTxPowerLevel );
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_Tx_Data_ON( HANDLE hResourceContext, 
         unsigned short iTxFreq, 
         unsigned char iPayloadType, 
         unsigned char packetType );
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_ConfigXtalFTrim( HANDLE hResourceContext, 
         unsigned short iXtalFTrim );

   //BLE
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Transmitter_Test(HANDLE hResourceContext,
      unsigned char iTestFrequency,
      unsigned char iTestPayloadLength,
      unsigned char iTestPayload);
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Enhanced_Transmitter_Test(HANDLE hResourceContext,
      unsigned char iTestFrequency,
      unsigned char iTestPayloadLength,
      unsigned char iTestPayload,
      unsigned char iPhyType);
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Receiver_Test(HANDLE hResourceContext,
      unsigned char iTestFrequency);
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Enhanced_Receiver_Test(HANDLE hResourceContext,
      unsigned char iTestFrequency,
      unsigned char  iPhyType);
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Test_End(HANDLE hResourceContext,
      unsigned short* noOfPackets);
}

#endif //_QLIBDEMO_QCABT_H_