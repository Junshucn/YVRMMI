/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/CMeasurementManager_QSCPI.cpp#8 $
 * $DateTime: 2017/09/11 22:55:00 $
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

#include "CMeasurementManager_QSCPI.h"
#include <algorithm>

RFCal::CMeasurementManager_QSCPI::CMeasurementManager_QSCPI() 
{ this->qspi_version = 0.0; /* Use GetVersion() to update */ };

double RFCal::CMeasurementManager_QSCPI::Get_Version()
{
   string response;
   if ( this->qspi_version > 0.0 )
   {
      return this->qspi_version;
   }

   double tmp = 0.0;
   this->Read("QCOM:SYSTem:VERSION?",response);
   try
   {
     // tmp = std::stod(response);
      tmp = atof(response.c_str());
     
      this->qspi_version = tmp;
   }
   catch(...)
   {
      this->qspi_version = 0.0;
   }
   return tmp;
}

unsigned char RFCal::CMeasurementManager_QSCPI::Set_PathLoss(map <double, double> mapUPLoss,map <double, double> mapDLLoss,map <double, double> mapAUXLoss)
{
   this->Add_QMSLLogSection("\tRFCalInit");
   this->Write("QCOM:SYSTem:PRESet");
   this->RepeatUntilResponse("QCOM:SYSTem:OPC?","1",3);
   this->Write("*CLS");
   
   this->Write("QCOM:SYSTem:OPC?");

   this->Write("QCOM:MMEM:DIR:CORRection? 'UplinkLosses'");
   this->Write("QCOM:MMEM:DELete:CORRection 'UplinkLosses'");
   ::Sleep(10);
   this->Write("QCOM:MMEM:DIR:CORRection? 'DownlinkLosses'");
   this->Write("QCOM:MMEM:DELete:CORRection 'DownlinkLosses'");
   ::Sleep(10);
   this->Write("QCOM:MMEM:DIR:CORRection? 'AuxDownlinkLosses'");
   this->Write("QCOM:MMEM:DELete:CORRection 'AuxDownlinkLosses'");

   this->Write("QCOM:SYSTem:OPC?");
   this->Set_OutputPort ( false );
   this->Write("QCOM:UNIT:TIME MS");
   this->Write("QCOM:UNIT:FREQuency HZ");
   this->Write("QCOM:MMEM:DIR:CORRection? 'UplinkLosses'");
   this->Write("QCOM:MMEM:DIR:CORRection? 'DownlinkLosses'");
   this->Write("QCOM:MMEM:DIR:CORRection? 'AuxDownlinkLosses'");
   this->Write("QCOM:SYSTem:OPC?");

   // UL Loss
   {
      int size = mapUPLoss.size();
      if (size > 0)
      {
         string stResponse;
         this->Write("QCOM:MMEM:STORe:CORRection 'UplinkLosses'");
         this->Write("QCOM:SENSe:CORRection 'UplinkLosses', 1, 1");
         this->Write("QCOM:SYSTem:OPC?");

         this->Read("QCOM:SOURce:FREQuency:MAXimum?",stResponse);
         double MaxFrequency = this->ConvertStrToDouble(stResponse);

         stringstream ss;
         ss << "QCOM:MMEM:ADD:CORRection 'UplinkLosses'";
         map <double, double>::iterator it;
         for (it = mapUPLoss.begin(); it != mapUPLoss.end(); it++)
         {
            double freq = (it->first)*1000000.0;
            double loss = it->second;
            ss << ", " << freq << ", " << loss;

         }
         this->Write(ss.str());
         this->Write("QCOM:SYSTem:OPC?");
      }
   }
   // DL Loss
   {
      int size = mapDLLoss.size();
      if (size > 0)
      {
         string stResponse;
         
         this->Write("QCOM:MMEM:STORe:CORRection 'DownlinkLosses'");
         this->Write("QCOM:SOURce:CORRection 'DownlinkLosses', 1, 1");
         this->Write("QCOM:SYSTem:OPC?");

         this->Read("QCOM:SOURce:FREQuency:MAXimum?",stResponse);
         double MaxFrequency = this->ConvertStrToDouble(stResponse);

         stringstream ss;
         ss << "QCOM:MMEM:ADD:CORRection 'DownlinkLosses'";
         map <double, double>::iterator it;
         for (it = mapDLLoss.begin(); it != mapDLLoss.end(); it++)
         {
            double freq = (it->first)*1000000.0;
            double loss = it->second;
            ss << ", " << freq << ", " << loss;

         }
         this->Write(ss.str());
         this->Write("QCOM:SYSTem:OPC?");
      }
   }
   // AUX Loss
   if ( mapAUXLoss.empty () )
   {
      mapAUXLoss = mapDLLoss;
   }
   {
      if (!mapAUXLoss.empty())
      {
         string stResponse;
         
         this->Write("QCOM:MMEM:STORe:CORRection 'AuxDownlinkLosses'");
         this->Write("QCOM:SOURce:CORRection 'AuxDownlinkLosses', 1, 2");
         this->Write("QCOM:SYSTem:OPC?");

         this->Read("QCOM:SOURce:FREQuency:MAXimum?",stResponse);
         double MaxFrequency = this->ConvertStrToDouble(stResponse);         
         stringstream ss;
         ss << "QCOM:MMEM:ADD:CORRection 'AuxDownlinkLosses'";
         map <double, double>::iterator it;
         for (it = mapAUXLoss.begin(); it != mapAUXLoss.end(); it++)
         {
            double freq = (it->first)*1000000.0;
            double loss = it->second;
            ss << ", " << freq << ", " << loss;

         }
         this->Write(ss.str());
         this->Write("QCOM:SYSTem:OPC?");
      }
   }
   return true;
}

void RFCal::CMeasurementManager_QSCPI::Set_DL_Mod_Type(MOD_TYPE mod)
{
   if ( this->LinkState.modType != mod )
   {
      stringstream ss;
      ss << "QCOM:GENErator:WAVEform '";
      if (MOD_TYPE_RFCAL_CW == mod)
      {  
         ss << "CW";
      }
      ss << "'";
      this->Write(ss.str());
      this->RepeatUntilResponse("QCOM:SYSTem:OPC?","1",3); 
      this->LinkState.modType = mod;
   }
}

void RFCal::CMeasurementManager_QSCPI::Set_DL_Level(double power)
{
   if ( this->LinkState.dlPower != power )
   {
      stringstream ss;
      ss << "QCOM:SOURce:POWer ";
      ss << power;
      this->Write(ss.str());
      this->RepeatUntilResponse("QCOM:SYSTem:OPC?","1",3);
      this->LinkState.dlPower = power;
   }
}

void RFCal::CMeasurementManager_QSCPI::Set_DL_Channel(unsigned long ulchannel, double dloffset, double uloffset)
{
   double ULFreq = 0.0, DLFreq = 0.0;
   RFCal::Helper::Convert::ULchannel_To_Frequency(this->Get_Tech(), this->Get_Band(), ulchannel,ULFreq, DLFreq);
   
   // Update channel info
   this->Set_ULChannel(ulchannel);

   stringstream ss;
   ss << "QCOM:SOURce:FREQ:CENTer ";
   ss << (DLFreq *1000000 + 1000*dloffset);
   this->Write(ss.str());
   this->RepeatUntilResponse("QCOM:SYSTem:OPC?","1",3);
   ss.str("");
   
   ss << "QCOM:SENSe:FREQ:CENTer ";
   ss.setf(ios::fixed);
   ss << (ULFreq *1000000 + 1000*uloffset);
   this->Write(ss.str());
   this->RepeatUntilResponse("QCOM:SYSTem:OPC?","1",3);
   
}
void RFCal::CMeasurementManager_QSCPI::Set_DL_State(unsigned char on_off)
{
   if ( this->LinkState.bDL_State != on_off)
   {
      if (on_off)
      {  
         this->Write("QCOM:SOURce:STATE ON");
      }
      else
      {
         this->Write("QCOM:SOURce:STATE OFF");
      }
      this->RepeatUntilResponse("QCOM:SYSTem:OPC?","1",3);
      this->LinkState.bDL_State = on_off;
   }
}
void RFCal::CMeasurementManager_QSCPI::Set_UL_Expected_Level (double power)
{
   stringstream ss;
   ss << "QCOM:SENSe:POWer:REFerence " << power;
   this->RepeatUntilResponse("QCOM:SYSTem:OPC?","1",3);
}
void RFCal::CMeasurementManager_QSCPI::Set_Segment_Meas_Duration(double segmentDuration, double measOffset, double measDuration)
{
   stringstream ss;
   ss << "QCOM:SENSe:LIST:SEGMent:DURation " << segmentDuration;
   this->Write( ss.str() );

   ss.str("");
   ss << "QCOM:SENSe:LIST:MOFFset " << measOffset;
   this->Write( ss.str() );

   ss.str("");
   if(measDuration < 0.1) measDuration = 0.1;
   ss << "QCOM:SENSe:LIST:MDURation " << measDuration;
   this->Write( ss.str() );
}
void RFCal::CMeasurementManager_QSCPI::Start_GSM_Tx_Sweep()
{
   this->Write("QCOM:INIT:POWer:LIST");
   this->RepeatUntilResponse("QCOM:SYSTem:OPC?","1",10);
}

void RFCal::CMeasurementManager_QSCPI::Config_GSM_Tx_Sweep( const CGSM_Tx_Sweep_Cfg_Params_TE & cfgParams )
{
   //Set Frequency
   double ULFreq = 0.0, DLFreq = 0.0;
   RFCal::Helper::Convert::ULchannel_To_Frequency(this->Get_Tech(), this->Get_Band(), this->Get_ULChannel(),ULFreq, DLFreq);
   stringstream ss;
   ss << "QCOM:SENSe:LIST:SEGMent:FREQuency:CENTer ";
   ss << (ULFreq *1000000);
   this->Write(ss.str());

   Set_Segment_Meas_Duration
   (
      (cfgParams.m_step_length_us * 1e-3),     // Segment Duration ms
      (cfgParams.m_trigger_offset_us * 1e-3),  // Measurement Offset ms
      (cfgParams.m_meas_length_us) * 1e-3      // Measurement Duration ms
   );

   // Number of Segments
   ss.str("");
   ss << "QCOM:SENSe:LIST:SEGMent:COUNt " << cfgParams.m_num_steps;
   this->Write(ss.str());

   // Set Reference level
   ss.str("");
   ss << "QCOM:SENSe:LIST:SEGMent:RLEVel " << cfgParams.m_ul_max_lvl;
   this->Write(ss.str());

   // Set filter type
   this->Write("QCOM:SENSe:FILTer:TYPE 'Gaussian_500kHZ'");
//   Set_Filter_Type( m_technology, BW_1P4MHz );

   // Trigger Type/Mode
   this->Write("QCOM:SENSe:TRIGger:TYPE IF");
   this->Write("QCOM:SENSe:TRIGger:MODE RISING");

   // Trigger Level: Needs Review ( using relative value)
   this->Write("QCOM:SENSe:TRIGger:LEVel -30");
   
   // Trigger Offset (Not required for List mode power; but setting 0 to be safe)
   this->Write("QCOM:SENSe:TRIGger:OFFSet 0");

  // m_timeout_ms = 5.0e3 + cfgParams.m_num_steps * cfgParams.m_step_length_us * 1e-3;
};
void RFCal::CMeasurementManager_QSCPI::Get_GSM_Tx_Sweep_Results( const int& numSteps, vector< double >& sweepData, bool doBinaryTransfer )
{
   RFCal::CMeasurementManager_QSCPI::Wait_For_Meas_To_Complete(1000,"POWer");

   string response;
   this->Read("QCOM:FETCh:SENSe:POWer:LIST?",response);
   vector <double> data;
   data.reserve(100);
   this->ParseResponse(response,data);
   if( static_cast<unsigned int>(numSteps) > (data.size()/2) )
   {
      return;
   }
   sweepData.clear();
   sweepData.reserve(data.size()/2);

   for (vector <double>::iterator it = data.begin(); it!= data.end(); it++)
   {
      if (0 == static_cast<int>(*it))
      {
         it++;
         if (it != data.end())
            sweepData.push_back(*it);
      }
      else
      {
         // Error
         return;
      }
   }

};
unsigned char RFCal::CMeasurementManager_QSCPI::Wait_For_Meas_To_Complete ( int toMS, const string& measID )
{
   if( toMS < 0 )
   {
      toMS = 100; //Minimum wait time
   }
   this->Write("*CLS");

   int timer = 0;
   string resp;
   bool done = false;

   const DWORD startTimeLoop = GetTickCount();

   // query whether the result is ready, do not exceed TIMEOUT
   stringstream ss;
   do
   {
      ss.str("");
      ss<<"QCOM:SENSe:"<<measID<<":LIST:STATus?";
      string response;

      this->Read(ss.str(),response);
      if( resp.empty())
      {
         ///throw CApplicationException( g_exc_general_exception, "Received no response to query for measurement status." );
         return false;
      }
      else if( resp.find( "0" ) == string::npos )
      {
         done = true;
      }
      else
      {
         Sleep( 10 ); //polling interval
      }
      const DWORD currentTick = GetTickCount();
      timer = currentTick - startTimeLoop;
   }
   while( !done && (timer<toMS) );

   if( !done )
   {
      stringstream ss;
      ss.str("");
      ss<<"Measurement did not complete."
          <<" in the allowed timeout of "<<toMS<<" ms. "<<__FUNCTION__;
   }
   return done;
};
void RFCal::CMeasurementManager_QSCPI::Config_PreDist_Capture (
   unsigned long waveformDurationQS, 
   unsigned long & numSamples, 
   unsigned long & samplingRate )
{
   //Setup IQ
   float symbolrate = 270833.0;
   samplingRate = 10000000;

   // 1000 before and 1000 after. 2e7 sampling rate. /4 is conversion for QS to symbols
   // Please dont change the paranthesis as there is a probability of overrunning the samplingRate
   // (unsigned long)
   numSamples = 500 + (unsigned long)((waveformDurationQS/4.0)*(samplingRate/symbolrate)) + 500;

   //Set Frequency
   double ULFreq = 0.0, DLFreq = 0.0;
   RFCal::Helper::Convert::ULchannel_To_Frequency(this->Get_Tech(), this->Get_Band(), this->Get_ULChannel(),ULFreq, DLFreq);
   stringstream ss;
   ss << "QCOM:SENSe:LIST:SEGMent:FREQuency:CENTer " << ( ULFreq * 1e6 );
   this->Write( ss.str() );

   // Set filter type
   this->Write("QCOM:SENSe:FILTer:TYPE 'Gaussian_500kHZ'");
//   Set_Filter_Type( m_technology, BW_1P4MHz );

   // Trigger Type/Mode
   this->Write( "QCOM:SENSe:TRIGger:TYPE IF" );
   this->Write( "QCOM:SENSe:TRIGger:MODE RISING" );

   // Trigger Offset
   // Use -500 sample triggerDelay
   double triggerDelay_ms = -500.0 * ( 1.0 / samplingRate ) * 1.0e3;
   ss.str("");
   ss <<  "QCOM:SENSe:TRIGger:OFFSet " << triggerDelay_ms;
   this->Write( ss.str() );

   // Trigger Level: Needs Review ( using relative value)
   this->Set_Trigger ( -30.0 );

   // IQ Parameters
   ss.str("");
   ss <<  "QCOM:SENSe:IQ:RATE " << samplingRate;
   this->Write( ss.str() );

   ss.str("");
   ss <<  "QCOM:SENSe:IQ:COUNt " << numSamples;
   this->Write( ss.str() );
   
   this->RepeatUntilResponse("QCOM:SYSTem:OPC?","1",2);
}
void RFCal::CMeasurementManager_QSCPI::Set_Trigger (double relative_pwr)
{
   stringstream ss;
   if (this->qspi_version > 1.8)
   {
      ss << "QCOM:SENSe:TRIGger:LEVel " << relative_pwr;
   }
   else
   {
      ss << "QCOM:SENSe:TRIGger:LEVEl " << relative_pwr;
   }
   this->Write(ss.str());
}
void RFCal::CMeasurementManager_QSCPI::Start_PreDist_Capture( void )
{
   this->Write("QCOM:INIT:IQ");
   this->RepeatUntilResponse("QCOM:SYSTem:OPC?","1",2);
}
void RFCal::CMeasurementManager_QSCPI::Get_PreDist_Capture_Data(unsigned long & numSamples, float* phaseArr, float* amplArr )
{
   // Number of bytes to search for start of data key
   const unsigned int MAX_DATA_KEY_SEARCH = 256;

   // Protect the heap and limit the maximum IQ data capture to 64000 * 8 which is the max pre distortion capture size in bytes defined by MAX_PREDIST_CAPTURE_SIZE in the GSM algorithm
   const unsigned int MAX_BUFFER_SIZE = 512000;

   // Used for converstions
   istringstream iss;

   // Fetch the data
   if( this->qspi_version >= 1.8 )
   {
      this->Write("QCOM:FETCh:SENSe:IQ?", false);
   }
   else
   {
      this->Write("QCOM:FETCH:SENSe:IQ?", false);
   }

   //
   // Return IQ Data format
   // [0] : #
   // [1] : Number of digits (x) in the length of the data size
   // [2 to x] : Length of the IQ data in bytes
   // [x+1 to n] : IQ data (8 bytes each I & Q pair)
   //

   // Search for hash key
   char dataStartKey = 0;
   unsigned int searchCount = 0;
   while( dataStartKey != '#' || searchCount > MAX_DATA_KEY_SEARCH )
   {
      this->ReadFromEquipment( &dataStartKey, sizeof(dataStartKey) );

      if( searchCount++ > MAX_DATA_KEY_SEARCH )
      {
         // Give up - it's not coming
         stringstream oss;
         oss <<"ERROR: IQ Data key not found after "<<MAX_DATA_KEY_SEARCH <<" bytes .";
         this->Add_QMSLLog(oss.str());
         return;
      }
   }

   // Get number of digits in the length of the data
   char tmp = 0;
   unsigned int numDigitsInLength = 0;
   this->ReadFromEquipment( &tmp, sizeof(tmp) );
   char terminatedTmp[2];
   terminatedTmp[0] = tmp;
   terminatedTmp[1] = 0;
   iss.str( string(terminatedTmp) );
   iss >> numDigitsInLength;
   iss.clear();

   // Get the length of the data stream in bytes
   unsigned int bufferSizeBytes = 0;
   char * bufferSizeStr = new char [numDigitsInLength + 1];
   if (!bufferSizeStr)
   {
      return;
   }
   this->ReadFromEquipment( bufferSizeStr, numDigitsInLength );
   //null terminate
   bufferSizeStr[numDigitsInLength] = 0;
   iss.str( string(bufferSizeStr) );
   iss >> bufferSizeBytes;
   iss.clear();
   bufferSizeBytes = min<unsigned int>(bufferSizeBytes, MAX_BUFFER_SIZE);

   // Read the entire IQ data
   unsigned char * buffer = new unsigned char [bufferSizeBytes];
   if(!buffer)
   {
      return;
   }
   this->ReadFromEquipment( (char *)buffer, bufferSizeBytes );

   numSamples = 0;
   float iData = 0.0;
   float qData = 0.0;
   float aData = 0.0;
   float pData = 0.0;
   for( unsigned int i=0; i<bufferSizeBytes; i+=8 )
   {
      iData = *((float *)(&buffer[i]));
      qData = *((float *)(&buffer[i+4]));

      // The IQ Data returned is in Vrms.  The following formula is to convert it back to dBm.
      // Conversion Formulas:
      //    Amplitude = sqrt( i^2 + q^2 )
      //    P(W) = V^2 / R    where R = 50 ohm
      //    P(dBm) = 10 * log10( P(W) ) + 30
      aData = (float)(10.0 * log10( (iData*iData + qData*qData) / 50.0 ) + 30);

      // Phase(rad) = InverseTan(q/i)
      // Phase(deg) = Phase(rad) * 180 / PI
      pData = (float)(atan2(qData,iData) * (180.0 / 3.1415926535897932384626433832795));

      amplArr[numSamples] = max<float>(aData, -100.0);  // -100 Noise
      phaseArr[numSamples] = pData;

      ++numSamples;
   }

   // Each binary stream is terminated with a NL.  Must manually read the NL
   // out of the TE buffer to avoid conflicts of future reads.
   char newlineTerm;
   this->ReadFromEquipment( &newlineTerm, sizeof(newlineTerm) );

   // Clean up memory
   delete [] bufferSizeStr;
   bufferSizeStr = NULL;
   delete [] buffer;
   buffer = NULL;

 }
void RFCal::CMeasurementManager_QSCPI::Start_Cal_Sweep ( )
{
   this->Write ( "QCOM:INIT:SOURce:LIST");
   this->Write ( "QCOM:INIT:POWer:LIST");
   this->RepeatUntilResponse ("QCOM:SYSTem:OPC?","1",3);
}

void RFCal::CMeasurementManager_QSCPI::Config_Cal_Sweep ( const std::vector <TE_Swp_Params> & teRecords )
{
   if ( teRecords.empty())
   {
      return;
   }
   vector < double > vecULFreq;
   vector < double > vecDLFreq;
   vector < int > expectedTxLvl;
   vector < int > vecOperation;
   vector < string > vecState;
   vector < int > vecDlPwr;
   // Tx Meas config
   bool bFound = false;
   for ( std::vector <TE_Swp_Params>::const_iterator it = teRecords.begin(); it != teRecords.end(); it++)
   {
      if (!bFound)
      {
         if (! it->m_teAction)
         {
            continue;
         }
         bFound = true;
      }
      //Set Frequency
      double ULFreq = 0.0, DLFreq = 0.0;
      RFCal::Helper::Convert::ULchannel_To_Frequency( it->tech, it->band, it->m_ul_chan, ULFreq, DLFreq);

      vecULFreq.push_back ( ULFreq * 1000000 );
      vecDLFreq.push_back ( ( DLFreq + it->m_offset_in_kHz/1000.0) * 1000000 );
      if ( it->m_ul_lvl < -80.0 )
      {
         expectedTxLvl.push_back ( static_cast <int> ( 10 ));
      }
      else
      {
      expectedTxLvl.push_back ( static_cast <int> ( it->m_ul_lvl ) + 5 );
      }
      vecOperation.push_back ( (it->m_teAction)?1:0);
      vecState.push_back ( "ON" );
      vecDlPwr.push_back ( static_cast <int> ( it->m_dl_lvl ));
   }
   this->Write ( this->Get_ListModeConfigString ("QCOM:SENSe:LIST:SEGMent:FREQuency:CENTer", vecULFreq));
   this->Write ( this->Get_ListModeConfigString ("QCOM:SENSe:LIST:SEGMent:RLEVel", expectedTxLvl));
   this->Write ( this->Get_ListModeConfigString ("QCOM:SENSe:LIST:SEGMent:OPERation", vecOperation));

   this->Write ( this->Get_ConfigString ("QCOM:SENSe:LIST:SEGMent:DURation", static_cast < int> (teRecords.begin()->m_step_width/1000)));
   this->Write ( this->Get_ConfigString ("QCOM:SENSe:LIST:MOFFset", static_cast < double > (teRecords.begin()->m_step_width/10000)));
   this->Write ( this->Get_ConfigString ("QCOM:SENSe:LIST:MDURation", static_cast < double > (teRecords.begin()->m_step_width/1000*0.8)));
   this->Write ( this->Get_ConfigString ("QCOM:SENSe:LIST:SEGMent:COUNt", static_cast < int > (vecULFreq.size())));
 
   this->Set_Filter_Type ( teRecords.begin()->tech, teRecords.begin()->m_lte_tx_chan_bw );
 
   this->Write ( "QCOM:SENSe:TRIGger:TYPE IF");
   this->Write ( "QCOM:SENSe:TRIGger:MODE RISING");
   this->Write ( "QCOM:SENSe:TRIGger:LEVel -25");
   this->Write ( "QCOM:SENSe:TRIGger:OFFSet 0");

   bool isForceCW =  (teRecords.begin()->m_dl_modulation_type == 0)? true : false;
   this->Config_DL_Waveform (  teRecords.begin()->tech, false, true, isForceCW );

   this->Write ( this->Get_ConfigString ("QCOM:SOURce:LIST:SEGMent:COUNt", static_cast < int > (vecULFreq.size())));
   this->Write ( this->Get_ConfigString ("QCOM:SOURce:LIST:SEGMent:DURation", static_cast < int> (teRecords.begin()->m_step_width/1000) ));
   this->Write ( "QCOM:SOURce:POWer:LEVel:MAXimum? 2700,1,1");
   this->Write ( "QCOM:SOURce:POWer:LEVel:MINimum? 2700,1,1");

   this->Write ( this->Get_ListModeConfigString ("QCOM:SOURce:LIST:SEGMent:FREQuency", vecDLFreq));
   this->Write ( this->Get_ListModeConfigString ("QCOM:SOURce:LIST:SEGMent:POWer", vecDlPwr));
   this->Write ( this->Get_ListModeConfigString ("QCOM:SOURce:LIST:SEGMent:STATe", vecState));
   this->Write ( this->Get_ListModeConfigString ("QCOM:SENSe:LIST:SEGMent:OPERation", vecOperation));

}
unsigned char RFCal::CMeasurementManager_QSCPI::Wait_For_Tx_Sweep_To_Complete (int meastime )
{
   this->Write ("*CLS");
   this->RepeatUntilResponse ("QCOM:SYSTem:OPC?","1",3);
   const unsigned long sleepTime = 20;
 
   ::Sleep( meastime );

   unsigned long iTimeout = static_cast < int > ( meastime / sleepTime );
   return this->RepeatUntilResponse ("QCOM:SENSe:POWer:LIST:STATus?","1",iTimeout, sleepTime);
}
void RFCal::CMeasurementManager_QSCPI::Fecth_Sweep_Result ( std::vector < double > & result )
{
   string stResponse;
   if ( this->Get_Version() >= 1.8)
   {
      this->Read("QCOM:FETCh:SENSe:POWer:LIST?", stResponse);
   }
   else
   {
      this->Read("QCOM:FETCH:SENSe:POWer:LIST?", stResponse);
   }

   std::vector < double > tmpResult; 
   tmpResult.reserve (400);
   this->ParseResponse(stResponse, tmpResult);
   result.clear();
   result.reserve ( tmpResult.size() /2);
   for ( vector < double>::iterator it = tmpResult.begin(); it != tmpResult.end(); it++)
   {
      // Rx
      it++;

      if (it != tmpResult.end())
      {
         result.push_back ( (*it ));
      }
      else
      {
         return;
      }

   }
}
void RFCal::CMeasurementManager_QSCPI::Set_OutputPort (unsigned char isAuxport)
{
   if (( (isAuxport) && (this->Get_DLPort() == RF_PORT_AUX ))
      || ( (!isAuxport) && (this->Get_DLPort() == RF_PORT_MAIN )))
   {
      return;
   }
   this->Set_DLPort ( (isAuxport)? RF_PORT_AUX : RF_PORT_MAIN );

  stringstream ss;
   if ( this->Get_Version() >= 1.81)
   {
      ss << "QCOM:SOURce:ROUTe:PORT:CLOSe (@" << this->Get_DLPort()  << ")";
   }
   else if ( this->Get_Version() >= 1.52 )
   {
      ss << "QCOM:SOURce:OUTput:PORT " << this->Get_DLPort() ;
   }
   else
   {
      // No Support
      return;
   }
   this->Write ( ss.str() );

};
void RFCal::CMeasurementManager_QSCPI::Set_Filter_Type(int technology, int bw)
{
   stringstream ss;
   ss << "QCOM:SENSe:FILTer:TYPE ";
   if ( this->Get_Version() >= 1.5 )
   {
      ss << "'";
   }
   switch ( technology )
   {
      case 0:
         if ( this->Get_Version() >= 1.6 )
         {
            ss.str("");
            ss<<"QCOM:CDMA:SENSe:FILTer:TYPE '";
         }
         ss << "CDMA'";
         break;
      case 3:
      case 10:
         switch ( bw )
         {
         case 0: //BW_1P4MHz :
         case 1: //BW_3MHz :
            ss<<"BANDPASS_3MHz";
            break;
         case 2: // BW_5MHz :
            ss<<"BANDPASS_5MHz";
            break;
         case 3: //BW_10MHz :
            ss <<"BANDPASS_10MHz";
            break;
         case 4: //BW_15MHz :
            ss <<"BANDPASS_15MHz";
            break;
         case 5: // BW_20MHz :
            ss <<"BANDPASS_20MHz";
            break;
         case 6: //BW_40MHz :
         case 7: // BW_20MHz_plus_20MHz :
         case 128: // BW_20MHz_plus_20MHz_EDGE_CHAN :
            if(this->Get_Version() >= 1.81)
            {
               ss <<"BANDPASS_40MHz";
            }
            else
            {
               ss .str("");
               ss <<" QTI SCPI Version 1.81 is required for BW_40MHz option" <<__FUNCTION__;           
            }
            break;
         }
         break;
      case 4:
      case 11:
         ss << "BANDPASS_3MHz";
         break;
      case 2:
         switch(bw)
         {
         case 0: //BW_1P4MHz:
               if( this->Get_Version() >= 1.8 )
               {
                  ss<<"Gaussian_500kHZ";
               }
               else
               {
                  ss<<"BANDPASS_3MHz"; // We need 500kHz, but QTI SCPI < V1.8 does not define it. The best we can do is 3MHz. 100KHz is too narrow.
               }
               break;
            default:
               ss<<"Gaussian_100kHZ";
               break;
         }
      case 1: // WCDMA
      default:
         if(  this->Get_Version()  >= 1.6 )
         {
            ss.str("");
            ss<<"QCOM:WCDMa:SENSe:FILTer:TYPE '";
         }
         // Filter Settings for WCDMA Waveform
         ss<<"WCDMA";         
         break;

   }
   if( this->Get_Version() >= 1.5 )
   {
      ss<<"'";
   }
   this->Write (ss.str());
}
void RFCal::CMeasurementManager_QSCPI::Config_DL_Waveform ( int technology, bool pilotOnly, bool mod_onoff, bool isForceCW )
{
   stringstream ss;
   stringstream wfmStr;
   ss << "QCOM:SOURce:LIST:SEGMent:WAVEform ";

   if( technology == TECHNOLOGY_CDMA )
   {
      if( this->Get_Version() >= 1.8 )
      {
         ss.str("");
         ss << "QCOM:CDMA:SOURce:LIST:SEGMent:WAVEform '";
      }
      wfmStr << "CDMA2000";
   }
   else if( technology == TECHNOLOGY_WCDMA )
   {
      if( pilotOnly && (this->Get_Version() >= 1.51) )
      {
         if( this->Get_Version() >= 1.8 )
         {
            ss.str("");
            ss << "QCOM:WCDMa:SOURce:LIST:SEGMent:WAVEform '";
         }
         wfmStr << "WCDMA_R99_CPICH";
      }
      else
      {
         if( this->Get_Version() >= 1.8 )
         {
            ss.str("");
            ss << "QCOM:WCDMa:SOURce:LIST:SEGMent:WAVEform '";
         }
         wfmStr << "WCDMA_R99";
      }
   }
   else if( ( technology == TECHNOLOGY_LTE )
            ||( technology == TECHNOLOGY_TDSCDMA) )
   {
      if( this->Get_Version() >= 1.8 )
      {
         ss.str("");
         ss << "QCOM:SOURce:LIST:SEGMent:WAVEform '";
      }
      wfmStr << "CW";
   }
   else if ( technology == TECHNOLOGY_GSM )
   {
      if( this->Get_Version() >= 1.8 )
      {
         ss.str("");
         ss << "QCOM:GSM:SOURce:LIST:SEGMent:WAVEform '";
      }
      wfmStr << "GMSK";
   }
   else
   {
      return;
   }

   if( isForceCW )
   {
      wfmStr.str("");
      wfmStr<<"CW";
   }

   ss<<wfmStr.str();

   if( this->Get_Version() >= 1.8 )
   {
      ss << "'";
   }

   this->Write( ss.str() );
   this->RepeatUntilResponse ("QCOM:SYSTem:OPC?","1",3);
}
void RFCal::CMeasurementManager_QSCPI::Config_GSM_V4_FBRx_Sweep(  CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, TE_GSMV4_Sweep_Params& swpParams,
            bool &skipFirstRxMeas )
{
   map< int, map< int, double>> freqMap_UL_Hz;
   map< int, map< int, double>> freqMap_DL_Hz;

   // compute the uplink and downlink frequencies
   ComputeFreqFromARFCN( TECHNOLOGY_GSM, freqMap_UL_Hz, freqMap_DL_Hz, swpParams.chanVec, swpParams.bandVec, false, 0 /* Unit Hz */ );

   // configure the Tx Sweep
   Config_GSM_V4_Tx_Sweep( cfgParams, freqMap_UL_Hz, swpParams );

   // only set up Rx Sweep if Rx slots are requested
   if ( swpParams.performRxCal )
   {
     // TE_Config_GSM_V4_Rx_Sweep( cfgParams, freqMap_DL_Hz, swpParams );
   }

   // Send the Path Loss Table to the box (this relies upon RFCAL_Init_V3 being run)
   //UpdateULOffsets(); // this actually updates both the uplink and downlink loss tables - no need to run UpdateDLOffsets() too

}


void RFCal::CMeasurementManager_QSCPI::Config_GSM_V4_TxRx_Sweep( const CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, const TE_GSMV4_Sweep_Params& swpParams,
                                                                              bool &skipFirstRxMeas)
{

   map< int, map< int, double>> freqMap_UL_Hz;
   map< int, map< int, double>> freqMap_DL_Hz;

   // compute the uplink and downlink frequencies
   ComputeFreqFromARFCN( TECHNOLOGY_GSM, freqMap_UL_Hz, freqMap_DL_Hz, swpParams.chanVec, swpParams.bandVec, false, 0 );

   // configure the Tx Sweep
   Config_GSM_V4_Tx_Sweep( cfgParams, freqMap_UL_Hz, swpParams );

   // only set up Rx Sweep if Rx slots are requested
   if ( swpParams.performRxCal )
   {
      Config_GSM_V4_Rx_Sweep( cfgParams, freqMap_DL_Hz, swpParams );
   }

   // Send the Path Loss Table to the box (this relies upon RFCAL_Init_V3 being run)
//   UpdateULOffsets(); // this actually updates both the uplink and downlink loss tables - no need to run UpdateDLOffsets() too

}


void RFCal::CMeasurementManager_QSCPI::Config_GSM_V4_Rx_Sweep( const CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, map< int, map< int, double>>& freqMap_DL_Hz,
                                                                            const TE_GSMV4_Sweep_Params& swpParams )
{

   // Configure the waveform for GSM
   Config_DL_Waveform(TECHNOLOGY_GSM, false, true, false);

   // Size vectors to max number of steps
   unsigned int maxNumberOfSteps = swpParams.numTotalFrames * GSM_V4_MAX_NUM_OF_SLOTS;
   vector<double> dlFreqMHz_Vec;
   dlFreqMHz_Vec.resize(maxNumberOfSteps);
   vector<double> dlLevel_Vec;
   dlLevel_Vec.resize(maxNumberOfSteps);
   vector<double> duration_Vec;
   duration_Vec.resize(maxNumberOfSteps);

   // First, fill the with default values, utilizing the lowest available power level, an out-of-band frequency, and an invalid duration
   const double dlFreqHz_Noop = 750000000;   // 750 MHz
   const double dlMinPow_Noop  = -90;
   const double duration_Noop  = 0;
   fill( dlFreqMHz_Vec.begin(), dlFreqMHz_Vec.end(), dlFreqHz_Noop );
   fill( dlLevel_Vec.begin(),   dlLevel_Vec.end(),   dlMinPow_Noop );
   fill( duration_Vec.begin(),   duration_Vec.end(),   duration_Noop );

   // # of frames to skip after filling Rx frames for a band
   unsigned short numRxFramesToSkip = swpParams.numDpdFramesPerChan * swpParams.numChanPerBand * GSM_V4_MAX_NUM_OF_SLOTS;
   unsigned short totalRxDelay = numRxFramesToSkip + (swpParams.intraBandFrameDelay * GSM_V4_MAX_NUM_OF_SLOTS);

   unsigned short stepCntr  = 0;    // counts the step (slot) index for the call box setup
   bool firstSequence = true;

   double currentFrequency = -1; //Invalid value
   double currentLevel = -999; //Invalid value

   for (unsigned short bandIdx = 0 ; bandIdx < swpParams.numBand ; ++bandIdx)
   {
      unsigned int curBand = swpParams.bandVec[bandIdx];
      for(unsigned short chanIdx = 0 ; chanIdx < swpParams.numChanPerBand ; ++chanIdx)
      {
         unsigned int curChan = swpParams.chanVec[bandIdx][chanIdx];
         unsigned short calIterations = (swpParams.performDpdCal)? 2 : 1;
         // First iteration DA Cal
         // Second iteration DPD Cal
         for(unsigned short calIterationCntr = 0 ; calIterationCntr < calIterations ; ++calIterationCntr)
         {
            unsigned short numOfRxPower = static_cast<unsigned short>( swpParams.rxPowerList.size() );
            for ( unsigned short rxPowerCntr = 0 ; rxPowerCntr < numOfRxPower ; rxPowerCntr++)
            {
               unsigned short numOfSlots = GSM_V4_MAX_NUM_OF_SLOTS;
               if(firstSequence)
               {
                  firstSequence = false;
                  unsigned short rxShiftAmt = swpParams.txSlotShiftAmt; // default value
                  const unsigned short MIN_RX_SLOT_MARGIN = 2;
                  if (MIN_RX_SLOT_MARGIN > swpParams.rxSlotOffset)
                  {
                     rxShiftAmt += (MIN_RX_SLOT_MARGIN - swpParams.rxSlotOffset);
                  }
                  numOfSlots -= rxShiftAmt;
               }
               for (unsigned short slotCntr = 0 ; slotCntr < numOfSlots ; ++slotCntr)
               {
                  if( currentFrequency != freqMap_DL_Hz[curBand][curChan] || currentLevel != swpParams.rxPowerList[rxPowerCntr])
                  {
                     currentFrequency = freqMap_DL_Hz[curBand][curChan];
                     currentLevel = swpParams.rxPowerList[rxPowerCntr];
                     dlFreqMHz_Vec[stepCntr] = currentFrequency;
                     dlLevel_Vec[stepCntr]   = currentLevel;
                     duration_Vec[stepCntr] = ((cfgParams.m_step_length_us) * 1e-3);
                     ++stepCntr;
                  }
                  else if (stepCntr > 0)
                  {
                     duration_Vec[stepCntr-1] += ((cfgParams.m_step_length_us) * 1e-3);
                  }
               }
            }
         }
      }
      if(bandIdx < (swpParams.numBand-1))
      {
         // Start moving the TE to next frequency at a low power level.

         unsigned int nextBand = swpParams.bandVec[bandIdx+1];
         unsigned int nextChan = swpParams.chanVec[bandIdx+1][0];

         dlFreqMHz_Vec[stepCntr] = freqMap_DL_Hz[nextBand][nextChan];
         dlLevel_Vec[stepCntr] = dlMinPow_Noop;
         duration_Vec[stepCntr] = ((cfgParams.m_step_length_us) * 1e-3) * totalRxDelay;
         stepCntr ++;
	  }
   }
   if( this->Get_Version() >= 1.6 )
   {
	   stringstream ss;
	   ss << "QCOM:SOURce:LIST:SEGMent:COUNt " << stepCntr;
	   this->Write( ss.str() );
   }
   else
   {
	   stringstream ss;
	   ss << "QCOM:SOURce:LIST:SEGMent:COUNT " << stepCntr;
	   this->Write ( ss.str() );

   }

   ostringstream ostr_dl_freq, ostr_dl_lev, ostr_dl_state, ostr_seg_dur;
   ostr_dl_freq.str("");
   ostr_dl_freq<<"QCOM:SOURce:LIST:SEGMent:FREQuency ";
   ostr_dl_lev.str("");
   ostr_dl_lev<<"QCOM:SOURce:LIST:SEGMent:POWer ";
   ostr_dl_state.str("");
   ostr_dl_state<<"QCOM:SOURce:LIST:SEGMent:STATe ";
   ostr_seg_dur.str("");
   ostr_seg_dur<<"QCOM:SOURce:LIST:SEGMent:DURation ";

   // Create comma delimited list
   // Fill the first elements
   unsigned short segmentIndex = 0;
   ostr_dl_freq   << dlFreqMHz_Vec[segmentIndex];
   ostr_dl_lev    << dlLevel_Vec[segmentIndex];
   ostr_dl_state  << "ON";
   ostr_seg_dur   << duration_Vec[segmentIndex]; //((cfgParams.m_step_length_us) * 1e-3);

   // Add the rest of the elements separated by a comma
   for ( segmentIndex = 1 ; segmentIndex < stepCntr ; ++segmentIndex)
   {
      ostr_dl_freq   << "," << dlFreqMHz_Vec[segmentIndex];
      ostr_dl_lev    << "," << dlLevel_Vec[segmentIndex];
      ostr_dl_state  << "," << "ON";
      ostr_seg_dur   << "," << duration_Vec[segmentIndex];
   }

   // Send the Frequency list to the box
   this->Write (ostr_dl_freq.str());
   // Send the DL Level list to the box
   this->Write( ostr_dl_lev.str() );
   // Send the Mod list to the box
   this->Write( ostr_dl_state.str() );
   // Send the Segment Duration list to the box
   this->Write( ostr_seg_dur.str() );

}



void RFCal::CMeasurementManager_QSCPI::Start_GSM_V4_Sweep( const TE_GSMV4_Sweep_Params& swpParams )
{

   Start_List_Mode_Sweep();

}

void RFCal::CMeasurementManager_QSCPI::Start_List_Mode_Sweep( void )
{
	ostringstream ostr;
	
	switch(this->Get_Tech())
   {
      case TECHNOLOGY_CDMA:
      case TECHNOLOGY_WCDMA:
      case TECHNOLOGY_LTE:
      case TECHNOLOGY_TDSCDMA:
      case TECHNOLOGY_GSM:
         ostr.str("");
		 if( this->Get_Version() >= 1.90 )
         {
            ostr << "QCOM:SOURce:LIST:INITiate";
         }
         else
         {
            ostr << "QCOM:INIT:SOURce:LIST";
         }
         this->Write( ostr.str() );
         ostr.str("");
		 if( this->Get_Version() >= 1.90 )
         {
            ostr << "QCOM:POWer:LIST:INITiate";
         }
         else
         {
            ostr << "QCOM:INIT:POWer:LIST";
         }
         this->Write( ostr.str() );
         this->RepeatUntilResponse ("QCOM:SYSTem:OPC?","1",3);
         break;
      default:
         ostr.str("");
		 ostr<<"Technology "<< this->Get_Tech()
             <<" not yet supported. "<<__FUNCTION__;
		 this->Add_QMSLLog(ostr.str());
		 return;
   }

   
}

void RFCal::CMeasurementManager_QSCPI::Get_GSM_V4_Tx_Sweep_Results( const int& numSteps, vector<unsigned char>& slotConfig, vector<double>& outputPowVec,
	unsigned short numCongituousTxMeasPerBand, unsigned short intraBandSegDelay )
{
	stringstream ss;
	Wait_For_Meas_To_Complete( static_cast<int>(m_timeout_ms), "POWer" );

	string resultsStr;
	if( this->Get_Version() >= 1.8 )
	{
		resultsStr = this->Write( "QCOM:FETCh:SENSe:POWer:LIST?" );
	}
	else
	{
		resultsStr = this->Write( "QCOM:FETCH:SENSe:POWer:LIST?" );
	}

	vector<string> resultsList;
	resultsList.clear();

	std::vector < double > tmpResult;
	// What size should be reserved?
	tmpResult.reserve (2000);
	this->ParseResponse(resultsStr, tmpResult);

	// Check if the expected number of results are being returned.
	// We expect twice the number of steps because each result comes with a status string.
	if (static_cast<int>(resultsList.size()) != (numSteps * 2))
	{
		ss.str("");
		ss << "GSM Tx measurement read failed: Expected " << numSteps << " bytes, received " << resultsList.size() << " bytes." <<__FUNCTION__;
		this->Add_QMSLLog(ss.str());
		return;
	}

	vector<double> sweepData;
	sweepData.clear();

	vector<string>::iterator resultsItr = resultsList.begin();
	for (; resultsItr!=resultsList.end() ; ++resultsItr)
	{
		int status = atoi((*resultsItr).c_str());
		++resultsItr;

		if(resultsItr == resultsList.end())
		{
			ss.str("");
			ss<< " Corrupt result data. "
				<< __FUNCTION__;
			this->Add_QMSLLog(ss.str());
			return;
		}

		if( status == 0 ) //Power result is good
		{
			sweepData.push_back(atof( (*resultsItr).c_str() ));
		}
		else
		{
			return;
		}
	}

	//------------------------------------ Extract Only the Desired/True Uplink Measurements ---------------------------------------//
	// This was previously in GSM_V4_Cal_Alg, but this step is call box specific, so should be done in the measurement manager

	vector<unsigned char> slotIdxOffset;              // contains the slot Idx offsets (location of Tx slots within a frame)

	// get the slot index offsets for Tx slots
	for (unsigned char i=0; i<GSM_V4_MAX_NUM_OF_SLOTS; ++i)
	{
		if ( slotConfig[i] > 0 )
		{
			slotIdxOffset.push_back(i);
		}
	}

	// check if there were no Tx slots, and exit
	if ( slotIdxOffset.empty() )
	{
		ss.str("");
		ss << "No Tx slots were found in GSM V4 cal slot config, but execution is in a Tx function. Further exectution will fail." << __FUNCTION__;
		this->Add_QMSLLog (ss.str());
		return;
	}

	// modify slotIdxOffset, so that all indexes are relative to the first slot (recoreded measurement by the call box)
	transform( slotIdxOffset.begin(), slotIdxOffset.end(), slotIdxOffset.begin(), bind2nd(std::minus<unsigned char>(), slotIdxOffset[0]) );

	unsigned int numTxSlots = slotIdxOffset.size();
	unsigned int stepIdx    = 0;   // index of the current measurement step in inputPowVec
	unsigned int slotIdx    = 0;   // index of the current slot in the current frame
	unsigned int outPowIdx  = 0;   // index of the output power array

	while ( stepIdx < static_cast<unsigned int>(numSteps) )
	{
		for (slotIdx=0; slotIdx<numTxSlots; ++slotIdx)
		{
			outputPowVec[outPowIdx] = sweepData[ stepIdx + slotIdxOffset[slotIdx] ];
			++outPowIdx;
		}
		// check if the number of contiguous Tx measurements per band has been obtained
		if ( ( outPowIdx  % numCongituousTxMeasPerBand ) == 0 )
		{
			stepIdx += GSM_V4_MAX_NUM_OF_SLOTS; // increment to the next frame
			stepIdx += intraBandSegDelay;    // increment to the next segment, containing real measurement results
		}
		else
		{
			stepIdx += GSM_V4_MAX_NUM_OF_SLOTS; // increment to the next frame
		}
	}

}

void RFCal::CMeasurementManager_QSCPI::Config_GSM_V4_Tx_Sweep( const CGSM_Tx_Sweep_Cfg_Params_TE& cfgParams, map< int, map< int, double>>& freqMap_UL_Hz,
            const TE_GSMV4_Sweep_Params& swpParams )
{
   int m_usingPowerListModeForGsmTx = true;

   unsigned int numberOfSteps = swpParams.numTotalFrames * GSM_V4_MAX_NUM_OF_SLOTS;

   vector<double> ulFreqMHz_Vec;
   ulFreqMHz_Vec.resize( numberOfSteps );

   vector<unsigned short> measOperation_Vec;
   measOperation_Vec.resize( numberOfSteps );

   unsigned int curBand = swpParams.bandVec[0];
   unsigned int curChan = swpParams.chanVec[0][0];

   // Initialize ulFreqMHz_Vec with the first frequency
   fill( ulFreqMHz_Vec.begin(), ulFreqMHz_Vec.end(), freqMap_UL_Hz[curBand][curChan] );
   fill( measOperation_Vec.begin(), measOperation_Vec.end(), 0 );

   // Create the sweep for TX measurements. We sweep all channels in all bands with a constant reference level
   // And we only measure some slots of the frame. For a given frame, if the last slots don't need to be measured,
   // we move the test equipment to the next frequency.
   if (swpParams.performDaCal || swpParams.performDpdCal)
   {
      unsigned short stepCntr  = 0;    // counts the step (frame) index for the call box setup
      for (unsigned short bandIdx = 0 ; bandIdx < swpParams.numBand ; ++bandIdx)
      {
         curBand = swpParams.bandVec[bandIdx];
         // Fill DA Cal Frames
         if (swpParams.performDaCal)
         {
            for(unsigned short chanIdx = 0 ; chanIdx < swpParams.numChanPerBand ; ++chanIdx)
            {
               curChan = swpParams.chanVec[bandIdx][chanIdx];
               for (unsigned short frameCntr = 0 ; frameCntr < swpParams.numDaFramesPerChan ; ++frameCntr)
               {
                  unsigned short slotItr = GSM_V4_MAX_NUM_OF_SLOTS;
                  for (unsigned short slotCntr = 0 ; slotCntr < slotItr ; ++slotCntr)
                  {
                     // Select the frequency for this slot
                     ulFreqMHz_Vec[stepCntr] = freqMap_UL_Hz[curBand][curChan];

                     // If this slot is to be measured, set meas op to 1
                     if(swpParams.slotConfig[slotCntr] > 0)
                     {
                        measOperation_Vec[stepCntr] = 1;
                     }
                     else if( (frameCntr + 1 >= swpParams.numDaFramesPerChan) && ((slotCntr + 2) >= slotItr) )
                     {
                        // If this slot is not going to be measured:
                        // For the last two slots of this channel, move to the next frequency
                        unsigned int nextChan = 0;
                        if( (chanIdx+1) < swpParams.numChanPerBand)
                        {
                           // Move to the next channel
                           nextChan = swpParams.chanVec[bandIdx][chanIdx+1];
                           ulFreqMHz_Vec[stepCntr] = freqMap_UL_Hz[curBand][nextChan];
                        }
                        else
                        {
                           // For the last channel, move to channel 0 to be ready for DPD cal.
                           nextChan = swpParams.chanVec[bandIdx][0];
                           if ( swpParams.performDpdCal )
                           {
                              // Move to the next frequency if DPD will be performed. If not, intraband delay will take care of the frequency change
                              ulFreqMHz_Vec[stepCntr] = freqMap_UL_Hz[curBand][nextChan];
                           }
                        }
                     }
                     ++stepCntr;
                  }
               }
            }
         }
         // Repeat the previous sweep for DPD Cal Frames
         if ( swpParams.performDpdCal )
         {
            for(unsigned short chanIdx = 0 ; chanIdx < swpParams.numChanPerBand ; ++chanIdx)
            {
               curChan = swpParams.chanVec[bandIdx][chanIdx];
               for (unsigned short frameCntr = 0 ; frameCntr < swpParams.numDpdFramesPerChan ; ++frameCntr)
               {
                  unsigned short slotItr = GSM_V4_MAX_NUM_OF_SLOTS;
                  for (unsigned short slotCntr = 0 ; slotCntr < slotItr ; ++slotCntr)
                  {
                     ulFreqMHz_Vec[stepCntr] = freqMap_UL_Hz[curBand][curChan];
                     if(swpParams.slotConfig[slotCntr] > 0)
                     {
                        measOperation_Vec[stepCntr] = 1;
                     }
                     else if( (frameCntr + 1 >= swpParams.numDpdFramesPerChan) && ((slotCntr + 2) >= slotItr) )
                     {
                        // If no meas op, for the last two slots, move to the next frequency
                        unsigned int nextChan = 0;
                        if( (chanIdx+1) < swpParams.numChanPerBand)
                        {
                           nextChan = swpParams.chanVec[bandIdx][chanIdx+1];
                           ulFreqMHz_Vec[stepCntr] = freqMap_UL_Hz[curBand][nextChan];
                        }
                        else
                        {
                           // Don't change frequency here. frequency change will happen during intra-band delay
                        }
                     }
                     ++stepCntr;
                  }
               }
            }
         }
         if(bandIdx+1 < swpParams.numBand)
         {
            // ADD INTRA-BAND FRAME DELAY
            for(unsigned int frameDelayCounter = 0 ; frameDelayCounter < static_cast<unsigned int>(swpParams.intraBandFrameDelay *GSM_V4_MAX_NUM_OF_SLOTS) ; frameDelayCounter++)
            {
               // During intra-band frame delay, the TE is configured to the next band to be ready to measure
               curBand = swpParams.bandVec[bandIdx+1];
               curChan = swpParams.chanVec[bandIdx+1][0];
               ulFreqMHz_Vec[stepCntr] = freqMap_UL_Hz[curBand][curChan];
               stepCntr++;
            }
         }
      }
   }

   // Channel and Input Level list mode streams
   ostringstream ostr_ul_freq, ostr_ul_lev, ostr_meas_op;
   ostr_ul_freq.str("");
   ostr_ul_freq << "QCOM:SENSe:LIST:SEGMent:FREQuency:CENTer ";
   ostr_ul_lev.str("");
   if( this->Get_Version() >= 1.7 )
   {
      ostr_ul_lev << "QCOM:SENSe:LIST:SEGMent:RLEVel ";
   }
   else
   {
      ostr_ul_lev << "QCOM:SENSe:LIST:SEGMent:RLEVl ";
   }
   ostr_meas_op.str("");
   ostr_meas_op<<"QCOM:SENSe:LIST:SEGMent:OPERation ";

   // Create comma delimited list
   // Fill the first elements
   unsigned short segmentIndex = 0;
   ostr_ul_freq   << ulFreqMHz_Vec[segmentIndex];
   ostr_ul_lev    << cfgParams.m_ul_max_lvl;
   ostr_meas_op    << measOperation_Vec[segmentIndex];

   // Add the rest of the elements separated by a comma
   for ( segmentIndex = 1 ; segmentIndex < numberOfSteps ; ++segmentIndex)
   {
      ostr_ul_freq   << "," << ulFreqMHz_Vec[segmentIndex];
      ostr_ul_lev    << "," << cfgParams.m_ul_max_lvl;
      ostr_meas_op   << "," << measOperation_Vec[segmentIndex];
   }

   // Send the Frequency list to the box
   this->Write( ostr_ul_freq.str() );
   // Send the Input Level list to the box
   this->Write( ostr_ul_lev.str() );
   // Send the Measurement Operation list to the box
   this->Write( ostr_meas_op.str() );

   // Segment duration
   Set_Segment_Meas_Duration
   (
      ((cfgParams.m_step_length_us) * 1e-3),     // Segment Duration ms
      ((cfgParams.m_trigger_offset_us) * 1e-3),  // Measurement Offset ms
      ((cfgParams.m_meas_length_us)) * 1e-3      // Measurement Duration ms
   );

   // Number of segments
   stringstream ss;
   ss << "QCOM:SENSe:LIST:SEGMent:COUNt " << numberOfSteps;
  this->Write( ss.str() );

   // Set filter type

   this->Write( "QCOM:SENSe:FILTer:TYPE 'Gaussian_500kHZ'" );

   // Configure triggering
   this->Write( "QCOM:SENSe:TRIGger:TYPE IF" );
   this->Write( "QCOM:SENSe:TRIGger:MODE RISING" );
   this->Write( "QCOM:SENSe:TRIGger:OFFSet 0");
   this->Write( "QCOM:SENSe:TRIGger:LEVel -40");


   // Set timeout to sequence duration + 5s
   m_timeout_ms = 5.0e3 + (numberOfSteps * (cfgParams.m_step_length_us * 1e-3));

}
void RFCal::CMeasurementManager_QSCPI::Start_GSM_V4_FBRx_Sweep( const TE_GSMV4_Sweep_Params& swpParams, bool enable_rx )
{
   stringstream ostr;
   if(enable_rx == true)
	{
	ostr.str("");
	if( this->Get_Version() >= 1.90 )
	{
		ostr << "QCOM:SOURce:LIST:INITiate";
	}
	else
	{
		ostr << "QCOM:INIT:SOURce:LIST";
	}
	this->Write( ostr.str() );
	}
   ostr.str("");
   if( this->Get_Version() >= 1.90 )
   {
      ostr << "QCOM:POWer:LIST:INITiate";
   }
   else
   {
      ostr << "QCOM:INIT:POWer:LIST";
   }
	this->Write( ostr.str() );
   this->Write("QCOM:SYSTem:OPC?");
}
void RFCal::CMeasurementManager_QSCPI::Get_GSM_V4_FBRx_Sweep_Results( const int& numSteps, unsigned int numChanperBand, unsigned int numFramesperChan, vector<double>& outputPowVec,
            unsigned short numCongituousTxMeasPerBand, unsigned short intraBandSegDelay )
{

   Wait_For_Meas_To_Complete( static_cast<int>(5000), "POWer" );

   string resultsStr;
   if(this->Get_Version() >= 1.8 )
   {
      this->Read( "QCOM:FETCh:SENSe:POWer:LIST?",resultsStr );
   }
   else
   {
      this->Read( "QCOM:FETCH:SENSe:POWer:LIST?",resultsStr );
   }

   
   std::vector < double > tmpResult; 
   tmpResult.reserve (400);
   this->ParseResponse(resultsStr, tmpResult);
   outputPowVec.clear();
   outputPowVec.reserve ( tmpResult.size() /2);
   
   // Check if the expected number of results are being returned.
   // We expect twice the number of steps because each result comes with a status string.

   vector<double> sweepData;
   sweepData.clear();
   sweepData.reserve ( tmpResult.size() /2);

   for ( vector < double>::iterator it = tmpResult.begin(); it != tmpResult.end(); it++)
   {
      // Rx
      it++;

      if (it != tmpResult.end())
      {
         sweepData.push_back ( (*it ));
      }
      else
      {
         return;
      }

   }
   //------------------------------------ Extract Only the Desired/True Uplink Measurements ---------------------------------------//
   // This was previously in GSM_V4_Cal_Alg, but this step is call box specific, so should be done in the measurement manager

   vector<unsigned char> slotIdxOffset;              // contains the slot Idx offsets (location of Tx slots within a frame)

   //will need to add if there is more than one FBRx gain state
   slotIdxOffset.push_back(0);

   // check if there were no Tx slots, and exit
   if ( slotIdxOffset.empty() )
   {
      stringstream ostr;
      ostr << "No Tx slots were found in GSM V4 cal slot config, but execution is in a Tx function. Further exectution will fail." << __FUNCTION__;
      this->Add_QMSLLog (ostr.str());
      return;
   }

   // modify slotIdxOffset, so that all indexes are relative to the first slot (recoreded measurement by the call box)
   transform( slotIdxOffset.begin(), slotIdxOffset.end(), slotIdxOffset.begin(), bind2nd(std::minus<unsigned char>(), slotIdxOffset[0]) );

   unsigned int numTxSlots = slotIdxOffset.size();
   unsigned int stepIdx    = 0;   // index of the current measurement step in inputPowVec
   unsigned int slotIdx    = 0;   // index of the current slot in the current frame
   unsigned int outPowIdx  = 0;   // index of the output power array
   numCongituousTxMeasPerBand = static_cast<unsigned short>(numCongituousTxMeasPerBand*numChanperBand);

   while ( stepIdx < static_cast<unsigned int>(numSteps) )
   {
      for (slotIdx=0; slotIdx<numTxSlots; ++slotIdx)
      {
         outputPowVec.push_back( sweepData[ stepIdx + slotIdxOffset[slotIdx] ] );
         ++outPowIdx;
      }
      // check if the number of contiguous Tx measurements per band has been obtained
      if ( ( outPowIdx  % numCongituousTxMeasPerBand ) == 0 )
      {
         stepIdx += GSM_V4_MAX_NUM_OF_SLOTS; // increment to the next frame
         stepIdx += intraBandSegDelay;    // increment to the next segment, containing real measurement results
      }
      else
      {
         stepIdx += GSM_V4_MAX_NUM_OF_SLOTS; // increment to the next frame
      }
   }
}