/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/CMeasurementManager.cpp#8 $
 * $DateTime: 2016/12/05 18:58:54 $
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

#include "CMeasurementManager.h"

RFCal::CMeasurementManager::CMeasurementManager(int board_id, int primaryAddress, int secondaryAddress, bool simulationMode)
{
   if (!simulationMode)
   {
#ifdef USE_GPIB_32_DLL
   if ( NULL == hGPIB_32_DLL )
   {
      hGPIB_32_DLL = LoadLibraryA("GPIB-32.dll");
      if ( hGPIB_32_DLL )
      {
        ibdev_dll = (int(__stdcall *)(int,int,int,int,int,int))GetProcAddress(hGPIB_32_DLL, (LPCSTR)"ibdev");
        ibwrt_dll = (int(__stdcall *)(int, PVOID, long))GetProcAddress(hGPIB_32_DLL, (LPCSTR)"ibwrt");
        ibrd_dll = (int(__stdcall *)(int, PVOID, long))GetProcAddress(hGPIB_32_DLL, (LPCSTR)"ibrd");
        if ( NULL == ibdev_dll)
        {
           FreeLibrary ( hGPIB_32_DLL );
           hGPIB_32_DLL = NULL;
        }
        else
        {
           this->DeviceConfig.hInst = (HANDLE)(ibdev_dll(board_id, primaryAddress, secondaryAddress, 14, 1,0));
        }
      }
   }
#else
      this->DeviceConfig.hInst = (HANDLE)ibdev(board_id, primaryAddress, secondaryAddress, 14, 1, 0);
#endif
   }
   this->Set_SimulationMode(simulationMode);
   this->Set_Tech(0);
   this->DeviceConfig.PrimaryAddress = primaryAddress;
   this->DeviceConfig.SecondaryAddress = secondaryAddress;
   this->DeviceConfig.SecondaryAddress = board_id;

}
RFCal::CDeviceManager::~CDeviceManager()
{
#ifdef USE_GPIB_32_DLL
   if ( NULL != hGPIB_32_DLL )
   {
      FreeLibrary ( hGPIB_32_DLL );
      hGPIB_32_DLL = NULL;
   }
#endif
}
HANDLE RFCal::CDeviceManager::Connect(HANDLE hPhone, int gpib_board_id, int primaryAddress, int secondaryAddress)
{
   HANDLE tmpHandle = NULL;
#ifdef USE_GPIB_32_DLL
   if ( NULL == hGPIB_32_DLL )
   {
      hGPIB_32_DLL = LoadLibraryA("GPIB-32.dll");
      if ( hGPIB_32_DLL )
      {
        ibdev_dll = (int(__stdcall *)(int,int,int,int,int,int))GetProcAddress(hGPIB_32_DLL, (LPCSTR)"ibdev");
        ibwrt_dll = (int(__stdcall *)(int, PVOID, long))GetProcAddress(hGPIB_32_DLL, (LPCSTR)"ibwrt");
        ibrd_dll = (int(__stdcall *)(int, PVOID, long))GetProcAddress(hGPIB_32_DLL, (LPCSTR)"ibrd");

        if ( NULL == ibdev_dll)
        {
           FreeLibrary ( hGPIB_32_DLL );
           hGPIB_32_DLL = NULL;
        }
        else
        {
           tmpHandle = (HANDLE)(ibdev_dll(gpib_board_id, primaryAddress, secondaryAddress, 14, 1,0));
        }
      }
   }
#else
   tmpHandle = (HANDLE)ibdev(gpib_board_id, primaryAddress, secondaryAddress, 14, 1, 0);
#endif
   if (NULL != tmpHandle)
   {
      this->DeviceConfig.PrimaryAddress = primaryAddress;
      this->DeviceConfig.SecondaryAddress = secondaryAddress;
      this->DeviceConfig.hInst = tmpHandle;
      this->hDUT = hPhone;
   }
   return tmpHandle;
}

void RFCal::CDeviceManager::initialize()
{
   memset(&this->buffer,0,(size_t)this->BUFFSIZE);
}

void RFCal::CDeviceManager::Add_QMSLLog(const std::string & Log)
{
   QLIB_AddUserMessageToTextLog(this->Get_DUTHandle(),LOG_IO,(char *)Log.c_str(), this->bShowLogConsole);
}

void RFCal::CDeviceManager::Add_QMSLLogSection(const std::string & Log)
{
   string st = "===============================================================================";
   HANDLE tmpH = this->Get_DUTHandle();
   QLIB_AddUserMessageToTextLog(this->Get_DUTHandle(),LOG_IO,(char *)st.c_str(), this->bShowLogConsole);
   QLIB_AddUserMessageToTextLog(this->Get_DUTHandle(),LOG_IO,(char *)Log.c_str(), this->bShowLogConsole);
   QLIB_AddUserMessageToTextLog(this->Get_DUTHandle(),LOG_IO,(char *)st.c_str(), this->bShowLogConsole);
}


unsigned char RFCal::CDeviceManager::Write(const std::string & cmd, bool enableResponse)
{
   if (cmd.empty () )
   {
      return false;
   }
   string tmp;
   this->Read(cmd, tmp, enableResponse);

   return true;
}
void RFCal::CDeviceManager::Set_DUTHandle(HANDLE tmpHand)
{
   this->hDUT = tmpHand;
}

HANDLE RFCal::CDeviceManager::Get_DUTHandle()
{
   return this->hDUT;
}


unsigned char RFCal::CDeviceManager::Read(const std::string & cmd, std::string & Response, bool enableQueryProcess)
{
   if ( cmd.empty() )
   {
      return false;
   }
   // Write Query
   Response = "";

   memset(&this->command,0,(size_t)this->BUFFSIZE);
   strcpy_s(command,(size_t)this->BUFFSIZE, cmd.c_str());

   WriteToEquip(command);
   {
      stringstream Log;
//      Log << "GPIB_SEND[" << this->DeviceConfig.PrimaryAddress << "]: " << cmd ;
        Log << "SCPI_SEND: " << cmd ;

      this->Add_QMSLLog(Log.str());
   }
   // Check if the cmd is query or not
   if (!enableQueryProcess)
   {
      return true;
   }
   bool bFound = false;
   for (UINT i=0; i <= cmd.length() -1; i++)
   {
      if (cmd.at(i) == '?')
      {
         bFound = true;
         break;
      }
   }

   if (!bFound) { return true;}

   // Read Response
   this->ReadFromEquipment(Response);

   // Write response to a QMSL log
   {
      stringstream Log;
      Log << "SCPI_RECV: " << Response;
      this->Add_QMSLLog(Log.str());
   }

   // Set response string into an internal variable
   this->Set_Response(Response);

   return true;
}
unsigned char RFCal::CDeviceManager::Read(const std::string & cmd)
{
   string tmp;
   this->Read(cmd,tmp);
   return true;
}
// Write a NULL terminated string to equipment
unsigned char RFCal::CDeviceManager::WriteToEquip(char *sStr)
{
   /// Write 
   if (!this->DeviceConfig.bSimulationMode)
   {
      if (this->DeviceConfig.hInst){
#ifdef USE_GPIB_32_DLL
         ibwrt_dll((int)(this->DeviceConfig.hInst), sStr, (long)strlen(sStr));
#else
         ibwrt((int)(this->DeviceConfig.hInst), sStr, (long)strlen(sStr));
#endif
      }
   }
   return true;
}
unsigned char RFCal::CDeviceManager::ReadFromEquipment(char * sStr, long iSizeOfStr)
{

      memset(sStr, 0, iSizeOfStr);
#ifdef USE_GPIB_32_DLL
      ibrd_dll((int)(this->DeviceConfig.hInst),sStr, iSizeOfStr);
#else
      ibrd((int)(this->DeviceConfig.hInst), sStr, iSizeOfStr);
#endif
//      memcpy(sStr, this->buffer, iSizeOfStr);
      return true;
#if 0
   }
   if (iSizeOfStr < BUFFSIZE)
   {
      memset(sStr, 0, BUFFSIZE);
#ifdef USE_GPIB_32_DLL
      ibrd_dll((int)(this->DeviceConfig.hInst), this->buffer, iSizeOfStr);
#else
      ibrd((int)(this->DeviceConfig.hInst), this->buffer, iSizeOfStr);
#endif
      memcpy(sStr, this->buffer, iSizeOfStr);
      return true;
   }
   else
   {
      char * pBuffer = new char[iSizeOfStr];
      if (pBuffer)
      {
         memset(pBuffer, 0, iSizeOfStr);
#ifdef USE_GPIB_32_DLL
         ibrd_dll((int)(this->DeviceConfig.hInst), pBuffer, iSizeOfStr);
#else
         ibrd((int)(this->DeviceConfig.hInst), pBuffer, iSizeOfStr);
#endif
         memcpy(sStr, pBuffer, iSizeOfStr);
         return true;
      }
      else
      {
         return false;
      }
   }
#endif
 }
unsigned char RFCal::CDeviceManager::ReadFromEquipment(string & response)
{
   if (!this->DeviceConfig.bSimulationMode)
   {
      if (this->DeviceConfig.hInst)
      {   
         memset(this->buffer, 0, BUFFSIZE);
#ifdef USE_GPIB_32_DLL
         ibrd_dll((int)(this->DeviceConfig.hInst), this->buffer, BUFFSIZE);
#else
         ibrd((int)(this->DeviceConfig.hInst), this->buffer, BUFFSIZE);
#endif
         string tmpSt(this->buffer);
         response = tmpSt;
         if (response.length() > 0 && (char)response.at(response.length()-1) == (char)10)
         {
            response = response.substr(0,response.length()-1);
         }
         if (response.length() > 0 && (char)response.at(response.length()-1) == (char)13)
         {
            response = response.substr(0,response.length()-1);
         }
      }
   }
   return true;
}

///
// Store the previous response.
//
void RFCal::CDeviceManager::Set_Response(string tmp)
{
   this->bufResponse = tmp;
}
std::string RFCal::CDeviceManager::Get_DeviceName( unsigned char forceExecuteIDN)
{
   if ( forceExecuteIDN || this->deviceName.empty() )
   {
      string devName("");
      this->Read ("*IDN?", devName );
      this->deviceName = devName;
   }
   return this->deviceName;  
}
void RFCal::CDeviceManager::Get_Response(string & response)
{
   if (this->bufResponse.length() == 0)
   {
      response = "";
   }
   else
   {
      response = this->bufResponse;
   }
}


double RFCal::CDeviceManager::ConvertStrToDouble(const string & orgSt)
{
   double INVALID = -99999;
   double tmp = INVALID;
   
   if (orgSt == "NCAP") { return INVALID;}
   string delimiter = "E";
   size_t pos = 0;
   pos = orgSt.find(delimiter);
   if (pos == string::npos) { return atof(orgSt.c_str());}

   string stUpper = orgSt.substr(0,pos);
   double dsign = +1;
   if (orgSt.at(pos+1) == '+'){
      dsign = +1;
   } else if (orgSt.at(pos+1) == '-') {
      dsign = -1;
   }
   string stLower = orgSt.substr(pos+2,orgSt.length() - pos -2);

   double upper = atof(stUpper.c_str());
   double lower = atof(stLower.c_str())*dsign;

   tmp = upper * (pow(10,lower));
   return tmp;
}
unsigned char RFCal::CDeviceManager::ParseResponse(const string & response, vector <double> & data)
{
   string tmpResponse = response+",";
   data.resize(0);

   string delimiter = ",";
   size_t pos = 0;
   while ((pos = tmpResponse.find(delimiter)) != string::npos)
   {
      string token = tmpResponse.substr(0,tmpResponse.find(delimiter));
      data.push_back(ConvertStrToDouble(token));
      tmpResponse = tmpResponse.substr(pos+1,tmpResponse.length() - pos-1);
   }
   return true;
}
unsigned char RFCal::CDeviceManager::ParseResponse(const string & response, vector <string> & data)
{
   string tmpResponse = response+",";
   data.resize(0);

   string delimiter = ",";
   size_t pos = 0;
   while ((pos = tmpResponse.find(delimiter)) != string::npos)
   {
      string token = tmpResponse.substr(0,tmpResponse.find(delimiter));
      data.push_back(token);
      tmpResponse = tmpResponse.substr(pos+1,tmpResponse.length() - pos-1);
   }
   return true;
}
unsigned char RFCal::CDeviceManager::RepeatUntilResponse(const string & stQuery, const string & expectedResponse,  unsigned long  iTimeoutCount,  unsigned long  iSleepTime)
{
   string stResponse = "";
   unsigned char bSuccess = 0;
   if (this->DeviceConfig.bSimulationMode) { return true; }

   while ((!bSuccess) && (iTimeoutCount-- > 0))
   {

      this->Read(stQuery, stResponse);
      if (stResponse.compare(expectedResponse) == 0)
      {
         bSuccess = 1;
      }
      else
      {
         ::Sleep(iSleepTime);
      }
   }
   return bSuccess;
}
unsigned char RFCal::CMeasurementManager::Set_RFMode(FTM_RF_Mode_Enum rfMode)
{
   int tech = RFCal::Helper::Convert::GetTech_From_RFMode ( rfMode );
   int band = RFCal::Helper::Convert::GetBandNumber_From_RFMode ( rfMode );
   this->Set_Tech ( tech );
   this->Set_Band ( band );
   return true;
}
void RFCal::CMeasurementManager::ComputeFreqFromARFCN( int tech, map<int, map<int, double>>& freqMap_UL_MHz,
                                                              map< int, map< int, double>>& freqMap_DL_MHz, const vector<vector<int > > & chanList, const vector<int>& bandList,
                                                              bool addFreqOffset, int outputFreqUnit )
{
   
   double UnitConv = 1000000.0;
   if ( outputFreqUnit == 1 )
   {
      UnitConv = 1000.0;
   } 
   else if ( outputFreqUnit == 2 )
   {
      UnitConv = 1.0;
   }

   
   for ( unsigned int iBandIdx = 0 ; iBandIdx < bandList.size(); iBandIdx++ )
   {
      for (unsigned int iChanIdx = 0; iChanIdx < chanList[iBandIdx].size(); iChanIdx++ )
      {
         double ULFreq = 0.0, DLFreq = 0.0;
         RFCal::Helper::Convert::ULchannel_To_Frequency(tech, bandList[iBandIdx], chanList[iBandIdx][iChanIdx], ULFreq, DLFreq);
         int CurBand = bandList[iBandIdx];
         int CurChan = chanList[iBandIdx][iChanIdx];
         freqMap_UL_MHz[CurBand][CurChan] = ULFreq*UnitConv;
         freqMap_DL_MHz[CurBand][CurChan] = DLFreq*UnitConv;
      }
   }
}
double RFCal::CMeasurementManager::GetCalPointLoss (const map < double, double > & freqLossMap, const double freq)
{
   double loss = 0.0;
   double nextLoss;
   double nextFreq;
   double prevLoss;
   double prevFreq;
   ostringstream ostr;

   // Actual frequency is in map, no interpolation
   map < double, double >::const_iterator freq_itr =  freqLossMap.find( freq );
   if( freqLossMap.end() != freq_itr ) 
   {
      loss = freq_itr->second;
   }
   else // Frequency not in map, need to interpolate loss
   {
      // Find the first frequency value that is greater than the requested.
      map < double, double >::const_iterator lossPointItr = freqLossMap.lower_bound( freq );

      // Make sure the requested frequency is between the ranges of frequencies in DB
      if( lossPointItr == freqLossMap.begin() || lossPointItr == freqLossMap.end() )
      {
         return 0.0;
      }

      // Get values loss values for frequencies before and after requested freq
      nextFreq = lossPointItr->first;
      nextLoss = lossPointItr->second;
      do
      {
         --lossPointItr;
         prevFreq = (lossPointItr)->first;
         prevLoss = (lossPointItr)->second;
      }
      while( prevFreq == nextFreq );         // make sure freqs are different to avoid divide-by-zero

      // linear interpolation
      loss = prevLoss + (nextLoss - prevLoss)*(freq - prevFreq)/(nextFreq - prevFreq);
   }

   // add additional loss if enabled by addAdditionalLoss()
#if 0
   if( m_enableAdditionalLoss == true )
   {
      loss += m_additionalLoss;
   }
#endif
   return loss;
}

unsigned char RFCal::CMeasurementMatrixModeManager::Set_MatrixMode_Config (  const std::map < int, std::vector <TE_Swp_Params> > & frame_teRecords)
{
   if ( frame_teRecords.find ( 1 ) == frame_teRecords.end() ) // if there is no base frame, then no processing
   {
      return false ;
   }
   this->m_baseFrame.clear();
   this->m_baseFrame.reserve ( frame_teRecords.find(1)->second.size() );
   this->m_ChanFreqList.clear();
   this->m_ChanFreqList.reserve ( frame_teRecords.size() - 1 );

   // (1) Find baseframes
   // Copy baseframe (not reference)
   this->m_baseFrame = frame_teRecords.find ( 1 )->second;
   
   // Delete last element
   this->m_baseFrame.pop_back();

   for ( std::map < int, std::vector <TE_Swp_Params> >::const_iterator itr = frame_teRecords.begin(); itr != frame_teRecords.end(); itr++ )
   {
      if ( itr != frame_teRecords.begin() )
      {
         CChanFreq tmp;
         tmp.channel = itr->second.front().m_ul_chan;
         double ULFreq = 0.0, DLFreq = 0.0;
         RFCal::Helper::Convert::ULchannel_To_Frequency(itr->second.begin()->tech, itr->second.begin()->band, tmp.channel, ULFreq, DLFreq);
         tmp.ul_freq = ULFreq;
         tmp.dl_freq = DLFreq + itr->second.begin()->m_offset_in_kHz/1000.0;

         this->m_ChanFreqList.push_back ( tmp );
      }
   }

   return ( (!this->m_baseFrame.empty()) && ( !this->m_ChanFreqList.empty() ));
}
