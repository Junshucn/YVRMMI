/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/QlibDemo_QCABT.cpp#1 $
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


#include "QlibDemo_QCABT.h"

// UserDefinedTransport - Function protocol
typedef unsigned long (*_OpenUserDefinedTransport)(char* pIpPort );
typedef void (*_CloseUserDefinedTransport)(char* pIpPort );
typedef unsigned long (*_UserDefinedReceive)(HANDLE hQMSL_ContextID, HANDLE hUserContextID,   unsigned long* piResponseSize,   unsigned char* piResponseBytes);
typedef unsigned long (*_UserDefinedSend)(HANDLE hQMSL_ContextID, HANDLE hUserContextID, unsigned long iRequestSize, unsigned char* piRequestBytes,   unsigned long* piActualWriteSize);
typedef unsigned long (*_UserDefinedFlushTxRx)(HANDLE hQMSL_ContextID, HANDLE hUserContextID);

static unsigned long (*OpenUserDefinedTransport)(char* pConnectionString );
static void (*CloseUserDefinedTransport)(char* pConnectionString );
static unsigned long (*UserDefinedReceive)(HANDLE hQMSL_ContextID, HANDLE hUserContextID, unsigned long* piResponseSize,   unsigned char* piResponseBytes);
static unsigned long (*UserDefinedSend)(HANDLE hQMSL_ContextID, HANDLE hUserContextID, unsigned long iRequestSize, unsigned char* piRequestBytes,   unsigned long* piActualWriteSize);
static unsigned long (*UserDefinedFlushTxRx)(HANDLE hQMSL_ContextID, HANDLE hUserContextID);

static HINSTANCE hUDT = NULL;
static HANDLE g_hResourceContext = NULL;

// ======================================================================================================
/// <summary>
/// Setup for loading User Defined Transport and perform testing
/// </summary>
int Test_BT_x86(int argc, char** argv)
{
   hUDT = NULL;
   g_hResourceContext = NULL;
   char sComPort[10] = "6";
   char *dutIP = UDT_BT_IP;
   
   printf("======== Bluetooth ===========\n");

   if (argc >= 3) 
   {
      strcpy(sComPort, argv[2]);
   }
   else
   {
      printf("\nPlease enter the COM port/USB ID #[Eg: COM16 or \\\\.\\ATH0]: ");
      scanf("%s", & sComPort );
   }
   if (argc >= 4)
   {
      strcpy(dutIP, argv[3]);
   }

   printf("Connecting to port = %s in IP = %s\n\n", sComPort, dutIP);
   if (!ConnectStandaloneBTDevice(sComPort))
   {
      printf("ConnectStandaloneBTDevice() failed!\n");
      return(-1);
   }

   Test_FTM_QCA_BT();

   DisconnectStandaloneBTDevice(dutIP);

   return 0;
}

// ======================================================================================================
/// <summary>
/// Load User Defined Transport and setup Callback functions
/// </summary>
bool ConnectStandaloneBTDevice(char *sComPort)
{
   //80-WL400-11 Refer Section "Connect - standalone targets"
   hUDT = LoadLibrary("QMSL_BT_Transport.dll");  // Must reside in default QDART\bin folder
   if (hUDT == NULL)
   {
      printf("Can not find QMSL_BT_Transport.dll.\n");
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
      printf("Loading QMSL_BT_Transport function pointer failed.\n");
      return false;
   }
   char *ioType = "USB";
   if( strstr(sComPort, "com") || strstr(sComPort, "COM"))
   {
      ioType = "SERIAL";
   }


   //Compose User Defined Transport BT connection string in the format "connectiondetails=COM16,iotype=SERIAL,PORT=2390,IPADDRESS=127.0.0.1"
   char connectionString[255];
   strcpy(connectionString, "connectiondetails=");
   strcat(connectionString, sComPort);
   strcat(connectionString, ",iotype=");
   strcat(connectionString, ioType);
   strcat(connectionString, ",PORT=");
   strcat(connectionString, UDT_BT_PORT);
   strcat(connectionString, ",IPADDRESS=");
   strcat(connectionString , UDT_BT_IP);

   if (OpenUserDefinedTransport(connectionString) != 0)  // IP can be remote system with DUT or local
   {
      printf("OpenUserDefinedTransport failed.\n");
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


// ======================================================================================================
/// <summary>
/// Bluetooth test menu
/// </summary>
void Test_FTM_QCA_BT(void)
{
   char sel = ' ';
   int ret = 0;

   bool bContinue = true;
   do
   {
      printf("\n==== Bluetooth test menu ====\n");
      printf("Please select \n");
      printf("1. HCI Reset\n");
      printf("2. HCI DUT Mode\n");
      printf("3. BT EPTM\n");
      printf("4. Bluetooth CSR\n");
      printf("5. Bluetooth Low Energy\n");
      
      printf("Q. Quit\n\n");

      printf("Selection: ");
      fflush(stdin);
      scanf("%c",&sel);
      sel = toupper(sel);

      switch (sel)
      {
         case '1' : 
            ret = QLIB_FTM_BT_HCI_Reset(g_hResourceContext);
            printf("QLIB_FTM_BT_HCI_Reset %s\n",(ret == 0) ? "FAILED" : "PASSED");
            break;

         case '2' : 
            ret = QLIB_FTM_BT_Enable_Bluetooth(g_hResourceContext);
            printf("QLIB_FTM_BT_Enable_Bluetooth %s\n",(ret == 0) ? "FAILED" : "PASSED");
            break;

         case '3' : 
            ret = Test_FTM_BT_EPTM();
            break;

         case '4' :
            ret = Test_BT_CSR();
            break;

         case '5' :
            ret = Test_BT_LE();
            break;

         case 'Q':
            bContinue = false;
            break;
      }
   }
   while(bContinue);
}

// ======================================================================================================
/// <summary>
/// Main menu for EPTM
/// </summary>
int Test_FTM_BT_EPTM()
{
   bool bContinue = true;
   int ret = 0;

   printf("===========Test_FTM_BT_TxBurst==============\n");
   printf("Please select chip ID\n");
   printf("1. QCA6174/WCN2243/36xx\n");
   printf("2. AR3002\n");
   
   char chipID;
   fflush(stdin);
   scanf("%c", &chipID);

   switch(chipID)
   {
      case '1' : 
         ret = QLIB_FTM_BT_SET_CHIP_ID ( g_hResourceContext, 0);
         break;

      case '2' :
         ret = QLIB_FTM_BT_SET_CHIP_ID ( g_hResourceContext, 3002);
         break;
      
      default :
         printf("Chip ID not recognized.\n");
         return 0;
   }

   do
   {
      printf("\n\nPlease select option\n");
      printf("1. Tx Only Burst\n");
      printf("2. Rx Only Burst\n");
      printf("3. Rx Burst Config\n");
      printf("4. Rx Test Stats\n");
      printf("Q. Go back to main menu\n");

      char menuOption;
      fflush(stdin);
      scanf("%c", &menuOption);
      menuOption = toupper(menuOption);

      switch(menuOption)
      {
         case '1' :
         case '2' :
            ret = Test_FTM_BT_TxRxBurst(menuOption);
            break;
         case '3' :
            ret = Test_FTM_BT_RxBurstConfig();
            break;
         case '4' : 
            ret = Test_FTM_BT_RxTestStats();
            break;
         case 'Q' :
            bContinue = false;
            break;
         default :
            printf("Menu option invalid.\n");
            ret = 0;
            break;
      }
   } while (bContinue);
   

   return ret;
}

int Test_FTM_BT_RxBurstConfig()
{
   int iRxPktNumber = 0;
   int ret = 0;

   printf("\n==== Rx Burst Config ====\n\n");
   printf("Please enter Rx packet number or 0 for continuous reception\n");
   fflush(stdin);
   scanf("%d", &iRxPktNumber);

   ret = QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_RX_BURST_CONFIG( g_hResourceContext, iRxPktNumber);
   printf("QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_RX_BURST_CONFIG %s\n",(ret == 0) ? "FAILED" : "PASSED");

   return ret;
}

// ======================================================================================================
/// <summary>
/// Get input parameters and set Tx or Rx only burst.
/// </summary>
int Test_FTM_BT_TxRxBurst(char burstType)
{
   printf("Enter channel hopping list as 5 comma separated values:\n");
   char channelList [30];
   unsigned char cHopChannels[5];
   scanf("%s", &channelList);

   int ret = 0;
   int channel = 0;
   int j = 0;
   for(int i = 0; i < 20; i++)
   {
      if(j < 5)
      {
         if((channelList[i] != ',') && (channelList[i] != '\0'))
         {
            if((channelList[i] >= '0') && (channelList[i] <= '9'))
            {
               channel = (channel * 10) + (channelList[i] - '0');
            }
            else
            {
               printf("ERROR: Channel list contains an invalid character = '%c'", channelList[i]);
               return 0;
            }
         }
         else
         {
            cHopChannels[j++] = channel;
            channel = 0;
         }
      }
   }

   unsigned char targetBDAddr[6] = {0xbd, 0x35, 0x9c, 0xbd, 0x35, 0x9c};
   unsigned char itargetDeviceAddress[6];
   for(int i = 0,j=5; i < 6; i++, j--)
   {
      //reversing the order of the bytes
      itargetDeviceAddress[i] =  targetBDAddr[j];
   }

   unsigned char idataWhiteningEnable = false;
   unsigned char itransitOutputPower = 9;
   unsigned char ireceiverHighGainEnable = 1;
   unsigned char ihoppingEnable = false;
   unsigned short iPayloadLength = 339;
   unsigned char ilogicalTransportAddress = 0;


   switch (burstType)
   {
      case '1' : 
         ret = QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_TX(g_hResourceContext, cHopChannels, 
            PSEUDO_RANDOM, BT_DH5, idataWhiteningEnable, itransitOutputPower, 
            ireceiverHighGainEnable, itargetDeviceAddress, ihoppingEnable, iPayloadLength, ilogicalTransportAddress);
         Sleep(500); //wait for data coming in from DUT
         printf("QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_TX %s\n",(ret == 0) ? "FAILED" : "PASSED");
         break;

      case '2' :
         ret = QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_RX_ONLY(g_hResourceContext, cHopChannels, 
            PSEUDO_RANDOM, BT_DH5, idataWhiteningEnable, itransitOutputPower, 
            ireceiverHighGainEnable, itargetDeviceAddress, ihoppingEnable, iPayloadLength, ilogicalTransportAddress);
         Sleep(500); //wait for data coming in from DUT
         printf("QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_RX_ONLY %s\n",(ret == 0) ? "FAILED" : "PASSED");
         break;

      default :
         printf("Invalid burst type selection.\n");
         return 0;            
   }      
   return ret;
}

// ======================================================================================================
/// <summary>
/// Get Rx test statistics
/// </summary>
int Test_FTM_BT_RxTestStats()
{
   printf("===========Test_FTM_BT_RxTestStats==============\n\n");
   
   int ret = 0;
   const int MAX_CHANNELS = 5;
   BlueTooth_Prod_Test_Channel_Stat channelStats[MAX_CHANNELS];

   ret = QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_STATS(g_hResourceContext,
            (unsigned char *)&channelStats[0],
            (unsigned char *)&channelStats[1],
            (unsigned char *)&channelStats[2],
            (unsigned char *)&channelStats[3],
            (unsigned char *)&channelStats[4]);

   printf("QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_STATS %s\n",(ret == 0) ? "FAILED" : "PASSED");

   //There will be no results to print if API call fails.
   if (ret == 0) return ret;

   for (int i = 0; i < MAX_CHANNELS; i++)
   {
      printf("Channel %d\n",i+1);
      printf("---------------------\n");
      printf("Packet received: %d\n", channelStats[i].packetReceived);
      printf("Packet access error: %d\n", channelStats[i].packetAccessError);
      printf("HEC error: %d\n", channelStats[i].hecError);
      printf("CRC error: %d\n", channelStats[i].crcError);
      printf("Total packet bit errors: %d\n", channelStats[i].totalPacketBitErrors);
      printf("First half bit errors: %d\n", channelStats[i].firstHalfBitErrors);
      printf("Last half bit errors: %d\n", channelStats[i].lastHalfBitErrors);
      printf("RSSI reading: %d\n\n", channelStats[i].rssiReading);
   }

   return ret;
}

// ======================================================================================================
/// <summary>
/// Main menu for CSR targets.
/// </summary>
int Test_BT_CSR()
{
   char cOption;
   int ret = 0;
   bool bContinue = true;

   do
   {
      printf("\n\n==== Bluetooth CSR ====\n\n");
      printf("1. Send Warm Reset\n");
      printf("2. Pause\n");
      printf("3. Set Packet Type\n");
      printf("4. Set BD Addr\n");
      printf("5. Set Tx Power\n");
      printf("6. Tx Data ON\n");
      printf("7. Set XTAL TRIM\n");
      printf("Q. Go back to main menu\n");

      fflush(stdin);
      scanf("%c", &cOption);
      cOption = toupper(cOption);

      switch(cOption)
      {
         case '1' :
            ret = QLIB_FTM_BT_CSR8811_WarmReset(  g_hResourceContext );
            printf("QLIB_FTM_BT_CSR8811_WarmReset %s\n",(ret == 0) ? "FAILED" : "PASSED");
            break;
         case '2' :
            ret = QLIB_FTM_BT_CSR8811_Pause( g_hResourceContext );
            printf("QLIB_FTM_BT_CSR8811_Pause %s\n",(ret == 0) ? "FAILED" : "PASSED");
            break;
         case '3' :
            ret = Test_FTM_BTCSR_PacketType();
            break;
         case '4' :
            ret = Test_FTM_BTCSR_SetBDAddr();
            break;
         case '5' :
            ret = Test_FTM_BTCSR_SetTxPwr();
            break;
         case '6' :
            ret = Test_FTM_BTCSR_TxData();
            break;
         case '7' :
            ret = Test_FTM_BTCSR_XTAL();
            break;
         case 'Q' :
            bContinue = false;
            break;
         default :
            printf("Invalid selection\n");
            ret = 0;
            break;
      }
   }while (bContinue);
   
   return ret;
}


// ======================================================================================================
/// <summary>
/// Sends command to target and sets global variable iCSRPacketTypeValue that is used in other API calls.
/// </summary>
int Test_FTM_BTCSR_PacketType()
{
   unsigned short iPacketType;
   unsigned short iPayloadLength;
   printf("Please select packet type:\n");
   printf("1. BT_DH1\n");
   printf("2. BT_DH3\n");
   printf("3. BT_DH5\n");
   printf("4. BT2_DH1\n");
   printf("5. BT2_DH3\n");
   printf("6. BT2_DH5\n");
   printf("7. BT3_DH1\n");
   printf("8. BT3_DH3\n");
   printf("9. BT3_DH5\n");
   fflush(stdin);
   scanf("%hu", &iPacketType);

   switch(iPacketType)
   {
      case 1: iCSRPacketTypeValue = 4; break;
      case 2: iCSRPacketTypeValue = 0xB; break;
      case 3: iCSRPacketTypeValue = 0xF; break;
      case 4: iCSRPacketTypeValue = 0x14; break;
      case 5: iCSRPacketTypeValue = 0x1A; break;
      case 6: iCSRPacketTypeValue = 0x1E; break;
      case 7: iCSRPacketTypeValue = 0x18; break;
      case 8: iCSRPacketTypeValue = 0x1B; break;
      case 9: iCSRPacketTypeValue = 0x1F; break;
      default:
         printf("Invalid packet type selection\n");
         return 0;
   }

   printf("Please input payload length\n");
   fflush(stdin);
   scanf("%hu", &iPayloadLength);
   
   int ret = QLIB_FTM_BT_CSR8811_SetPacketType ( g_hResourceContext, iCSRPacketTypeValue, iPayloadLength );
   printf("QLIB_FTM_BT_CSR8811_Pause %s\n",(ret == 0) ? "FAILED" : "PASSED"); 

   return ret;
}

// ======================================================================================================
/// <summary>
/// Read input UAP and LAP, use QMSL API to set BD address.
/// </summary>
int Test_FTM_BTCSR_SetBDAddr()
{
   const unsigned short LAP_LENGTH = 3;

   int ret;

   unsigned char iUap;
   unsigned int tempUap;
   printf("\n==== Set BD Address ====\n\n");
   printf("Please enter UAP in hex format\n");
   fflush(stdin);
   scanf("%x", &tempUap);

   iUap = (unsigned char) tempUap;

   char sLap[15];
   printf("Please enter LAP as 3 comma separated hex values.\n");
   scanf("%s", &sLap);
      
   unsigned char lap[LAP_LENGTH] = {0,};
   unsigned int iLap[LAP_LENGTH];

   //convert sLap from string to int array
   sscanf(sLap, "%x,%x,%x", &iLap[0], &iLap[1], &iLap[2]);

   for(int i = 0; i < LAP_LENGTH; i++)
   {
      lap[i] = (unsigned char)(iLap[i]);
   }

   ret = QLIB_FTM_BT_CSR8811_SET_BD_ADDR( g_hResourceContext, iUap, lap );
   printf("QLIB_FTM_BT_CSR8811_SET_BD_ADDR %s\n",(ret == 0) ? "FAILED" : "PASSED"); 

   return ret;
}

// ======================================================================================================
/// <summary>
/// Read input parameters and set TX power in dBm.
/// </summary>
int Test_FTM_BTCSR_SetTxPwr()
{
   int ret;

   unsigned char iTxPwrLevel;
   unsigned int iTempTxPwr;
   printf("\n\n==== Set Tx Power ====\n\n");
   printf("Please enter Tx power in dBm:\n");
   fflush(stdin);
   scanf("%hu", &iTempTxPwr);

   iTxPwrLevel = (unsigned char) iTempTxPwr;

   ret = QLIB_FTM_BT_CSR8811_SetTxPower( g_hResourceContext, iTxPwrLevel);
   printf("QLIB_FTM_BT_CSR8811_SetTxPower %s\n",(ret == 0) ? "FAILED" : "PASSED"); 

   return ret;
}

// ======================================================================================================
/// <summary>
/// Read input parameters and set TX Data On.
/// Pre-requisite: QLIB_FTM_BT_CSR8811_SetPacketType should be called previously to set the packet type.
/// </summary>
int Test_FTM_BTCSR_TxData()
{
   if(iCSRPacketTypeValue == 0)
   {
      printf("Packet type not set. Please run Set Packet Type before Tx data ON\n");
      return 0;
   }

   unsigned short iTxFreq;
   printf("\n\n==== Tx Data ON ====\n\n");
   printf("Please enter Tx frequency in MHz\n");
   fflush(stdin);
   scanf("%hu", &iTxFreq);

   unsigned char payloadType;
   printf("Please select payload type:\n");
   printf("1. BT_PRBS9\n");
   printf("2. BT_10101010\n");
   printf("3. BT_11110000\n");
   fflush(stdin);
   scanf("%c", &payloadType);

   //set payload type byte per user selection
   unsigned char iPayloadTypeValue;
   switch(payloadType)
   {
      case '1' : iPayloadTypeValue = 4; break;
      case '2' : iPayloadTypeValue = 6; break;
      case '3' : iPayloadTypeValue = 7; break;
      default :
         printf("Invalid selection\n");
         return 0;
   }

   int ret = QLIB_FTM_BT_CSR8811_Tx_Data_ON( g_hResourceContext, iTxFreq, iPayloadTypeValue, (unsigned char)iCSRPacketTypeValue );
   printf("QLIB_FTM_BT_CSR8811_Tx_Data_ON %s\n",(ret == 0) ? "FAILED" : "PASSED");
   
   return ret;   
}

// ======================================================================================================
/// <summary>
/// Read Xtal value from user input and send it to target using QMSL API
/// </summary>
int Test_FTM_BTCSR_XTAL()
{
   unsigned short iXtalValue;
   printf("\n\n==== XTAL FTrim Calibration ====\n\n");
   printf("Please enter XTAL_FTrim_Value (Decimal):\n");
   fflush(stdin);
   scanf("%hu", &iXtalValue);

   int ret = QLIB_FTM_BT_CSR8811_ConfigXtalFTrim( g_hResourceContext, iXtalValue );
   printf("QLIB_FTM_BT_CSR8811_ConfigXtalFTrim %s\n",(ret == 0) ? "FAILED" : "PASSED");

   return ret;
}

// ======================================================================================================
/// <summary>
/// Main menu for BLE targets.
/// </summary>
int Test_BT_LE()
{
   char cOption;
   int ret = 0;
   bool bContinue = true;

   do
   {
      printf("\n\n==== Bluetooth Low Energy ====\n\n");
      printf("1. BT LE Transmitter Test\n");
      printf("2. BT LE Receiver Test\n");
      printf("Q. Go back to main menu\n");
      
      fflush(stdin);
      scanf("%c", &cOption);
      cOption = toupper(cOption);
      
      switch(cOption)
      {
         case '1' :
            ret = BLE_TxTest();
            break;
         case '2' :
            ret = BLE_RxTest();
            break;
         case 'Q' :
            ret = 0;
            bContinue = false;
            break;
         default :
            printf("Invalid selection\n");
            ret = 0;
            break;
      }
   } while (bContinue);

   return ret;
}

// ======================================================================================================
/// <summary>
/// Read input parameters and trigger Tx Test
/// </summary>
int BLE_TxTest()
{
   int ret = 0;

   unsigned short iTxFreq;
   printf("\n\n==== BLE Tx Test ====\n\n");
   printf("Please enter Test frequency in MHz\n");
   fflush(stdin);
   scanf("%hu", &iTxFreq);

   unsigned char cTxFreq = (unsigned char)((iTxFreq - 2402) / 2);

   unsigned short iPayloadLength;
   printf("\nPlease enter Test Payload Length\n");
   fflush(stdin);
   scanf("%hu", &iPayloadLength);

   if(iPayloadLength > 255)
   {
      printf("Payload Length too big for 1 byte. Please adjust.\n");
      return 0;
   }

   char isLE2M;
   printf("Enable LE 2M? [Y/N]\n");
   fflush(stdin);
   scanf("%c", &isLE2M);
   isLE2M = toupper(isLE2M);

   unsigned char iTestPayload;
   printf("\nPlease select test payload:\n");
   printf("1. Pseudo-Random bit sequence 9\n");
   printf("2. Pattern of alternating bits \'11110000\'\n");
   printf("3. Pattern of alternating bits \'10101010\'\n");
   printf("4. Pseudo-Random bit sequence 9\n");
   printf("5. Pattern of all \'1\' bits\n");
   printf("6. Pattern of all \'0\' bits\n");
   printf("7. Pattern of alternating bits \'00001111\'\n");
   printf("8. Pattern of alternating bits \'00001111\'\n");
   fflush(stdin);
   scanf("%c", &iTestPayload);
   
   //convert char to number value
   iTestPayload = iTestPayload - '0';

   if((iTestPayload > 8) || (iTestPayload < 1))
   {
      printf("Invalid payload type selection: %d\n", iTestPayload);
      return 0;
   }

   if(isLE2M == 'N')
   {
      ret = QLIB_FTM_BT_LE_HCI_Transmitter_Test(g_hResourceContext, cTxFreq, (unsigned char)iPayloadLength, iTestPayload-1);
      printf("QLIB_FTM_BT_LE_HCI_Transmitter_Test %s\n",(ret == 0) ? "FAILED" : "PASSED");
   }
   else if(isLE2M == 'Y')
   {
      unsigned char iPhyType = 2; //1 = 1LE and 2 = 2LE
      ret = QLIB_FTM_BT_LE_HCI_Enhanced_Transmitter_Test(g_hResourceContext, cTxFreq, (unsigned char)iPayloadLength, iTestPayload-1, iPhyType);
      printf("QLIB_FTM_BT_LE_HCI_Enhanced_Transmitter_Test %s\n",(ret == 0) ? "FAILED" : "PASSED");
   }

   return ret;
}

// ======================================================================================================
/// <summary>
/// Menu for RX Test on BLR targets
/// </summary>
int BLE_RxTest()
{
   int ret = 0;
   
   unsigned char cOption;
   printf("\n\n==== BLE Rx Test ====\n\n");
   printf("1. Start Rx Test\n");
   printf("2. End Rx Test (Get number of packets)\n");
   scanf(" %c", &cOption);

   switch (cOption)
   {
      case '1' :
         ret = BLE_StartRxTest();
         break;
      case '2' :
         ret = BLE_EndRxTest();
         break;
      default:
         printf("Invalid selection\n");
         break;
   }   

   return ret;
}

// ======================================================================================================
/// <summary>
/// Read input parameters and trigger Rx test on BLE targets
/// </summary>
int BLE_StartRxTest()
{
   int ret = 0;

   unsigned short iTxFreq;
   printf("Please enter Test frequency in MHz\n");
   fflush(stdin);
   scanf("%hu", &iTxFreq);

   unsigned char cTxFreq = (unsigned char)((iTxFreq - 2402) / 2);
   
   char isLE2M;
   printf("Enable LE 2M? [Y/N]\n");
   fflush(stdin);
   scanf("%c", &isLE2M);
   isLE2M = toupper(isLE2M);
   
   if(isLE2M == 'N')
   {
      ret = QLIB_FTM_BT_LE_HCI_Receiver_Test(g_hResourceContext, cTxFreq);
      printf("QLIB_FTM_BT_LE_HCI_Transmitter_Test %s\n",(ret == 0) ? "FAILED" : "PASSED");
   }
   else
   {
      unsigned char iPhyType = 2; //1 = 1LE and 2 = 2LE
      ret = QLIB_FTM_BT_LE_HCI_Enhanced_Receiver_Test(g_hResourceContext, cTxFreq, iPhyType);
      printf("QLIB_FTM_BT_LE_HCI_Enhanced_Receiver_Test %s\n",(ret == 0) ? "FAILED" : "PASSED");
   }

   return ret;
}

// ======================================================================================================
/// <summary>
/// End Rx test on BLE targets and get the number of packets received during test execution.
/// </summary>
int BLE_EndRxTest()
{
   int ret = 0;
   unsigned short iNumRxPackets;

   ret = QLIB_FTM_BT_LE_HCI_Test_End(g_hResourceContext, &iNumRxPackets);

   printf("QLIB_FTM_BT_LE_HCI_Test_End %s\n",(ret == 0) ? "FAILED" : "PASSED");

   if(ret)
   {
      printf("No. of packets = %d\n",iNumRxPackets);
   }
   return ret;
}

// ======================================================================================================
/// <summary>
/// Unload User Defined Transport
/// </summary>
void DisconnectStandaloneBTDevice(char *dutIP)
{
   //80-WL400-11 Refer Section "Disconnect - standalone targets"
   if (g_hResourceContext)
   {
      QLIB_DisconnectServer(g_hResourceContext);
   }

   CloseUserDefinedTransport(dutIP);

   if (hUDT)
   {
      FreeLibrary(hUDT);
   }
}