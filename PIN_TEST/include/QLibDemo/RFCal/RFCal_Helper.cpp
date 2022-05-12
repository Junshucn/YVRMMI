/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/RFCal_Helper.cpp#14 $
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
#include <math.h>
#include <set>
#include "Qlib_Defines.h"
#include "RFCal_Helper.h"

unsigned char RFCal::Helper::XML::Read_Str_Attribute(MSXML2::IXMLDOMNodePtr parentNode,const string & stAttribute, string & parameter)
{
   if (parentNode == NULL)
   {
      parameter = "";
      return false;
   }
	MSXML2::IXMLDOMNamedNodeMap* pParentAttributes;
	parentNode->get_attributes(&pParentAttributes);
	MSXML2::IXMLDOMNodePtr pCategoryNameNode = pParentAttributes->getNamedItem((_bstr_t)(stAttribute.c_str()));   
	if (pCategoryNameNode!=NULL)
	{
	   wstring ws = (wstring)(pCategoryNameNode->text);
	   string tmpSt(ws.begin(),ws.end());
      parameter = tmpSt;
		return true;
	}
	//varValue = pParentElement->getAttribute((_bstr_t)(stAttribute.c_str()));
	else
	{
		parameter = ""; 
		return false;
	}
};
int RFCal::Helper::Convert::DLPower_To_ExpectedAgc(unsigned char tech, double cellPower)
{
   double RxMin = 0.0; // LTE
   double offset = 512;
   switch (tech)
   {
      case 0:
         /*
         CalculatedAGCValue = (1024/DynamicRange) * (RxPower - MinRSSIofInterest) -512
         Assuming DynamicRange = 102.4 dB and MinRSSIofInterest = -115 dB, this equation reduces to:
         CalculatedAGCValue = (RxPower + 115 dB) * 10 - 512
         Example: RxPower = -80 dBm CalculatedAGCValue = [-80 - (-115)] * 10 - 512 = -162
         */
         RxMin = -115.0;
         break;
      case 3:
      case 10:
         RxMin = 0.0;
         offset = 0.0;
         break;
      case 1:
      case 4:
      case 11:
      default:
         RxMin = -106.0;
         break;
   }

   return static_cast<int>( (1023 / 102.3) * (cellPower - RxMin) - offset );
}
string RFCal::Helper::Convert::TechID_To_Str (int tech)
{
   string st = "";
   switch (tech)
   {
   case 0:
      st = "CDMA";
      break;
   case 1:
      st = "WCDMA";
      break;
   case 2:
      st = "GSM";
      break;
   case 3:
   case 10:
      st = "LTE";
      break;
   case 4:
   case 11:
      st = "TDSCDMA";
      break;
   default:
      break;
   }
   return st;
}
int RFCal::Helper::Convert::ULch_To_DLch(int tech, int band, int subband, long ulChannel)
{
	int offset = 0;
   unsigned short iBand = 0;
   
	switch (tech)
	{
	   case 1:
		      {
               if (band == 1)
               {
                  offset = 950;
               }
               else if ((band == 2) || (band == 19))
               {
                  offset = 400;
               }
               else if ( band == 9)
               {
                  offset = 475;
               }
               else if ((band >=3) && (band <=10))
               {
                  offset = 225;
               }
               else if ((band == 11) )
               {
                  offset = 225;
               }
				}
				break;
	   case 3: // LTE
      case 10:
		      {
               if ((band >=1) && (band <=28))
               {
                  offset = -18000;
               }
               else if ((band >=30) && (band <=31))
               {
                  offset = -17890;
               }
               else if ( band == 66 )
               {
                  offset = -65536;
               }
               else if ( band == 71 )
               {
                  offset = -64536;
               }
               else // TDD case
               {
                  offset = 0;  // B29 is DL only/
               }
				}
				   break;
	   case 0: // CDMA
	   case 2: // GSM
	   case 4: // TDSCDMA
      case 11:
	   default :
		   // UL = DL
				   break;
	}
	return ulChannel+offset;
}

BAND_CLASS_ENUM RFCal::Helper::Convert::ConverGSMBand_To_BandClassEnum (unsigned int gsmBand)
{
	BAND_CLASS_ENUM bandEnum;

	switch( gsmBand )
	{
	case NV_MODE_GSM_EGSM:
		bandEnum = BH_GSM_BAND_E_GSM;
		break;
	case NV_MODE_GSM_850:
		bandEnum = BH_GSM_BAND_850;
		break;
	case NV_MODE_GSM_DCS:
		bandEnum = BH_GSM_BAND_DCS;
		break;
	case NV_MODE_GSM_1900:
	default:
		bandEnum = BH_GSM_BAND_PCS;
		break;

	}

	return bandEnum;
}
int RFCal::Helper::Convert::GetGSMBand(int band)
{
   int gsmBand = 0;
   switch (band)
   {
      case 18:
        gsmBand = 850;
         break;
      case 10:
         gsmBand = 900;
         break;
      case 11:
         gsmBand= 1800;
         break;
      default:
      case 12:
         gsmBand= 1900;
         break;
      
   }
   return gsmBand;
}

int RFCal::Helper::Convert::DLch_To_ULch(int tech, int band, int subband, long dlChannel)
{
	int offset = 0;
	switch (tech)
	{
	   case 1:
		      {
               if (band == 1)
               {
                  offset = 950;
               }
               else if ((band == 2) || (band == 19))
               {
                  offset = 400;
               }
               else if ( band == 9)
               {
                  offset = 475;
               }
               else if ((band >=3) && (band <=10))
               {
                  offset = 225;
               }
               else if ((band == 11) )
               {
                  offset = 225;
               }
				}
				break;
	   case 3: // LTE
      case 10:
		      {
               if ((band >=1) && (band <=28))
               {
                  offset = -18000;
               }
               else if ((band >=30) && (band <=31))
               {
                  offset = -17890;
               }
               else if ( band == 66 )
               {
                  offset = -65536;
               }
               else // TDD case
               {
                  offset = 0;  // B29 is DL only/
               }
				}
				   break;
	   case 0: // CDMA
	   case 2: // GSM
	   case 4: // TDSCDMA
      case 11:
	   default :
		   // UL = DL
				   break;
	}
	return dlChannel-offset;
}
FTM_RF_Mode_Enum  RFCal::Helper::Convert::GSMBandClassToRFMode(const BAND_CLASS_ENUM bandType)
{
   FTM_RF_Mode_Enum retVal;
   switch(bandType)
   {
      case BH_GSM_BAND_E_GSM:
         retVal = PHONE_MODE_GSM_900;
         break;
      case BH_GSM_BAND_DCS:
         retVal = PHONE_MODE_GSM_1800;
         break;
      case BH_GSM_BAND_PCS:
         retVal = PHONE_MODE_GSM_1900;
         break;
      case BH_GSM_BAND_850:
         retVal = PHONE_MODE_GSM_850;
         break;
      default:
         retVal = PHONE_MODE_MAX;
   }
   return retVal;
}
unsigned char RFCal::Helper::Convert::ULchannel_To_Frequency(int TECH, int band, int ULChannel, double & ULFrequency, double & DLFrequency)
{
	BAND_CLASS_ENUM BandClass;
	switch (TECH)
	{
	case 0:
			BandClass = static_cast<BAND_CLASS_ENUM>(HDR_BAND_ENUM_START + band);
			break;
	case 1:
			BandClass = static_cast<BAND_CLASS_ENUM>(WCDMA_BAND_ENUM_START + band - 1);
			break;
	case 2:
         switch (band)
         {
         case 8: //850
         case PHONE_MODE_GSM_850:
            BandClass = BH_GSM_BAND_850;
            break;
         case 9: //PGSM
            BandClass = BH_GSM_BAND_P_GSM;
            break;
         case 10: //EGSM
            BandClass = BH_GSM_BAND_E_GSM;
            break;
         case 13: //1800 DCS
         case PHONE_MODE_GSM_1800:
            BandClass = BH_GSM_BAND_DCS;
            break;
         case 14: //1900 PCS
         case PHONE_MODE_GSM_1900:
            BandClass = BH_GSM_BAND_PCS;
            break;
         default:
   			BandClass = static_cast<BAND_CLASS_ENUM>(GSM_BAND_ENUM_START + band); // Unsupported
            break;
         }
         break;
	case 3:
   case 10:
			BandClass = static_cast<BAND_CLASS_ENUM>(LTE_BAND_ENUM_START + band - 1);
			break;
	case 4:
   case 11:
			BandClass = static_cast<BAND_CLASS_ENUM>(TDSCDMA_BAND_ENUM_START + band - 33);
			break;
	default:
			return false;
			break;
	}
   unsigned short usBandClass = static_cast<unsigned short>(BandClass);
	ULFrequency = QLIB_Band_Helper_CalculateULFrequency(usBandClass,ULChannel);
   int DLChannel = RFCal::Helper::Convert::ULch_To_DLch(TECH,band,0,ULChannel);
	DLFrequency = QLIB_Band_Helper_CalculateDLFrequency(usBandClass,DLChannel);

	return true;
}
int RFCal::Helper::Convert::GetBandNumber_From_RFMode(FTM_RF_Mode_Enum rfMode)
{
   int bandNumber = -1;
   switch ( rfMode)
   {
  		case PHONE_MODE_GSM_900:
         bandNumber = 2;
			break;
		case PHONE_MODE_GSM_1800:
         bandNumber = 13;
			break;
		case PHONE_MODE_GSM_1900:
         bandNumber = 14;
			break;
		case PHONE_MODE_GSM_850:
         bandNumber = 8;
			break;

      default:
         break;
   }
   return bandNumber;
}        
int RFCal::Helper::Convert::GetTech_From_RFMode(FTM_RF_Mode_Enum rfMode)
{
   int tech = - 1;
   switch ( rfMode )
   {
      case PHONE_MODE_CDMA_800:
      case PHONE_MODE_CDMA_1900:
      case PHONE_MODE_CDMA_1800:
      case PHONE_MODE_CDMA_450:
      case PHONE_MODE_CDMA_IMT:
      case PHONE_MODE_CDMA_1900_EXT:
      case PHONE_MODE_CDMA_BC15:
         tech = 0;
         break;
      case PHONE_MODE_GSM_900:
      case PHONE_MODE_GSM_1800:
      case PHONE_MODE_GSM_1900:
      case PHONE_MODE_GSM_850:
         tech = 2;
         break;
      // To DO... implement if necessary. only in this demo, only GSM requires this function
      default:
         break;
   }
   return tech;
}
int RFCal::Helper::Convert::GetRFMode(int tech, int band, int subband)
{
	int rfmode = PHONE_MODE_MAX;

	if (tech == 0)
	{
		switch (band)
		{
			case 0:
				rfmode = PHONE_MODE_CDMA_800;
				break;
			case 1:
				rfmode = PHONE_MODE_CDMA_1900;
				break;
			case 8:
				rfmode = PHONE_MODE_CDMA_1800;
				break;
			case 3:
				rfmode = PHONE_MODE_J_CDMA;
				break;
			case 5:
				rfmode = PHONE_MODE_CDMA_450;
				break;
			case 6:
				rfmode = PHONE_MODE_CDMA_IMT;
				break;
			case 10:
				rfmode = PHONE_MODE_CDMA_800_SEC;
				break;
			case 14:
				rfmode = PHONE_MODE_CDMA_1900_EXT; // PHONE_MODE_CDMA_450_EXT?
				break;
			case 15:
				rfmode = PHONE_MODE_CDMA_BC15;
				break;

			default:
				break;

		}
	}
	if (tech == 2)
	{
		switch (band)
		{
			case 2:
				rfmode = PHONE_MODE_GSM_900;
				break;
			case 13:
				rfmode = PHONE_MODE_GSM_1800;
				break;
			case 14:
				rfmode = PHONE_MODE_GSM_1900;
				break;
			case 8:
				rfmode = PHONE_MODE_GSM_850;
				break;
			default:
				break;

		}
	}

	if (tech == 1)
	{
		switch (band)
		{
			case 1:
				rfmode = PHONE_MODE_WCDMA_IMT;
				break;
			case 2:
				rfmode = PHONE_MODE_WCDMA_1900B; 
				break;
			case 5:
				rfmode = PHONE_MODE_WCDMA_800;
				break;
			case 3:
				rfmode = PHONE_MODE_WCDMA_800A;
				break;
			case 4:
				rfmode = PHONE_MODE_WCDMA_BC4;
				break;
			case 8:
				rfmode = PHONE_MODE_WCDMA_BC8;
				break;
			case 9:
				rfmode = PHONE_MODE_WCDMA_BC9; 
				break;
			case 11:
				rfmode = PHONE_MODE_WCDMA_1500;
				break;
			case 19:
				rfmode = PHONE_MODE_WCDMA_BC19;
				break;
			default:
				break;

		}
	}

	if (tech == 3 || tech == 10)
	{
		switch (band)
		{
			case 1:
				rfmode = PHONE_MODE_LTE_B1;
				break;
         case 2:
				rfmode = PHONE_MODE_LTE_B2;
				break;
			case 3:
				rfmode = PHONE_MODE_LTE_B3;
				break;
  			case 4:
				rfmode = PHONE_MODE_LTE_B4;
				break;
         case 5:
				rfmode = PHONE_MODE_LTE_B5;
				break;
         case 6:
				rfmode = PHONE_MODE_LTE_B6;
				break;
         case 7:
				rfmode = PHONE_MODE_LTE_B7; 
				break;
			case 8:
				rfmode = PHONE_MODE_LTE_B8; 
				break;
         case 9:
				rfmode = PHONE_MODE_LTE_B9; 
				break;
         case 10:
				rfmode = PHONE_MODE_LTE_B10;
				break;
         case 11:
				rfmode = PHONE_MODE_LTE_B11;
				break;
			case 12:
				rfmode = PHONE_MODE_LTE_B12;
				break;
         case 13:
				rfmode = PHONE_MODE_LTE_B13;
				break;
			case 14:
				rfmode = PHONE_MODE_LTE_B14;
				break;
			case 15:
				rfmode = PHONE_MODE_LTE_B15;
				break;
			case 16:
				rfmode = PHONE_MODE_LTE_B16;
				break;
         case 17:
				rfmode = PHONE_MODE_LTE_B17;
				break;
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
				rfmode = PHONE_MODE_LTE_B18+band-18;
				break;
			case 28:
				if (subband == 1)
				{
					rfmode = PHONE_MODE_LTE_B28;
				} 
				else
				{
					rfmode = PHONE_MODE_LTE_B28;
				}
			case 29:
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
				rfmode = PHONE_MODE_LTE_B29+band-29;
				break;

			case 38:
            rfmode = PHONE_MODE_LTE_B38; 
            break;

			case 39:
				if (subband == 1)
				{
					rfmode = PHONE_MODE_LTE_B39_B;
				} 
				else
				{
					rfmode = PHONE_MODE_LTE_B39;
				}
				break;
			case 40:
				rfmode = PHONE_MODE_LTE_B40;
				break;
			case 41:
				switch (subband)
				{
					case 2:
						rfmode = 102;
						break;
					case 1:
						rfmode = 101;
						break;
					case 0:
					default:
						rfmode = PHONE_MODE_LTE_B41;
						break;
				}
				break;
			case 42:
				rfmode = PHONE_MODE_LTE_B42;
				break;
         case 43:
            rfmode = PHONE_MODE_LTE_B43;
            break;
			case 45:
				rfmode = PHONE_MODE_LTE_B45;
				break;
         case 46:
            rfmode = PHONE_MODE_LTE_B46;
            break;
         case 47:
            rfmode = PHONE_MODE_LTE_B47;
            break;
         case 48:
            rfmode = PHONE_MODE_LTE_B48;
            break;
         case 66:
            rfmode = PHONE_MODE_LTE_B66;
            break;         
         case 71:
            rfmode = PHONE_MODE_LTE_B71;
            break;

         case 250:
            rfmode = PHONE_MODE_LTE_B250;
            break;
         case 252:
            rfmode = PHONE_MODE_LTE_B252;
            break;
         case 253:
            rfmode = 85;
            break;
         case 254:
            rfmode = 87;
            break;
         case 255:
            rfmode = PHONE_MODE_LTE_B255;
            break;


			default:
				break;

		}
	}

	if (tech == 4 || tech == 11)
	{
		switch (band)
		{
			case 34:
				rfmode = PHONE_MODE_TDSCDMA_B34;
				break;
			case 39:
				rfmode = PHONE_MODE_TDSCDMA_B39;
				break;
			case 40:
            if ( 1 == subband )
            {
   				rfmode = 93;
            }
            else
            {
   				rfmode = PHONE_MODE_TDSCDMA_B40;
            }
				break;
			default:
				break;
		}
	}
	return rfmode;
}
unsigned char RFCal::Helper::Log::StartQMSLLogging(HANDLE hPhone, const string & filename, unsigned char bAddAsync)
{
   logStartTime = GetTickCount();

	unsigned short _iLogFlags = LOG_FN | LOG_RET | LOG_IO | LOG_ERR | LOG_C_HIGH_LEVEL_START | LOG_C_HIGH_LEVEL_STOP; 

   if ( bAddAsync )
   {
      _iLogFlags |= LOG_ASYNC;
   }
  	QLIB_StopLogging(hPhone);
   QLIB_SetLogFlags( hPhone, _iLogFlags ); 
	QLIB_DIAG_SetPhoneLoggingState( hPhone, true );
	QLIB_StartLogging(hPhone,(char *)filename.c_str());
   
   return true;
}
unsigned char RFCal::Helper::Log::StopQMSLLogging(HANDLE hPhone)
{
   DWORD curTime = GetTickCount();
   unsigned long diff = static_cast <unsigned long> ( ( curTime > logStartTime )? (curTime - logStartTime): (logStartTime- curTime ));
   stringstream ss;
   ss << "Logging Elapsed Time: " << (diff/1000.0) << " sec";
   AddMessageToQMSLLog( hPhone, ss.str());
   ::QLIB_StopLogging( hPhone);
   return true;
}
void RFCal::Helper::Log::AddMessageToQMSLLog(HANDLE hPhone, const string & Log, unsigned char bShowConsole)
{
   QLIB_AddUserMessageToTextLog(hPhone,LOG_FN,(char *)Log.c_str(), bShowConsole);
}

void RFCal::Helper::Log::AddTitleToQMSLLog(HANDLE hPhone, const string & Log, unsigned char bShowConsole)
{
   RFCal::Helper::Log::AddMessageToQMSLLog(hPhone," ");
	RFCal::Helper::Log::AddMessageToQMSLLog(hPhone,"=========================================================================");
	RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, (char *)Log.c_str(), bShowConsole);
	RFCal::Helper::Log::AddMessageToQMSLLog(hPhone,"=========================================================================");
}
unsigned char RFCal::Helper::DIAG::RebootPhone (HANDLE hPhone)
{
   RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, "Phone is rebooting " );
   QLIB_DIAG_CONTROL_F ( hPhone, MODE_RESET_F );
   ::Sleep (3000); 
   IsPhoneConneted (hPhone, 100, 200);
   ::Sleep(2000); 
   RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, "Phone is back." );
   return true;
}
unsigned char RFCal::Helper::DIAG::GetBuildID(HANDLE hPhone, std::string & stBuildID)
{
	unsigned long iMSM_HW_Version= 0;
	unsigned long iMobModel = 0;
	char sMobSwRev[128] = {0};
   char sModelStr[128] = {0};
	stBuildID = "";
   unsigned char bOK = QLIB_DIAG_EXT_BUILD_ID_F ( hPhone, &iMSM_HW_Version, &iMobModel, sMobSwRev, sModelStr );
   if (bOK)
   {
      stBuildID.assign(sMobSwRev);
   }
   return bOK;
}
unsigned char RFCal::Helper::DIAG::IsPhoneConneted(HANDLE hPhone, int count, int sleepTime)
{
   int i = 0;
   unsigned char bStatus = 0;
   while ( ( bStatus = QLIB_IsPhoneConnected ( hPhone ) == 0 ) && i < count )
   {
      ::Sleep (sleepTime); 
      i++;
   }
   if (bStatus)
   {
      QLIB_IsPhoneConnected ( hPhone );
      QLIB_IsPhoneConnected ( hPhone );
   }
   return bStatus;
}
unsigned char RFCal::Helper::DIAG::Set_DMC_File(HANDLE hPhone, const string & stDmcFilePath)
{
    //**************************************************//
    //          1. Load DMC file (XML file)
    //          (Extended Message)
    //          2. Read (1) Extended Message 
    //          3. Set (1) Extended message
    //
    //          (Diag Log)
    //          4. Read (2) LogMask (for DiagLog)
    //          5. Read (3) OTA log ( this is the same as DiagLog)
    //          6. Set (2) and (3) LogMask/OTA logs
    //
    //          (Event ID)
    //          7. Read (4) Event ID
    //          8. Set (4) Event ID
    //          9. Closing
    //**************************************************//
    
    //**************************************************//
    //          1. Load DMC (XML)
    //**************************************************//
    MSXML2::IXMLDOMDocumentPtr spXMLDoc = NULL;

    CoInitialize(NULL); // Initialize COM
    spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
    VARIANT_BOOL bOK = spXMLDoc->load(stDmcFilePath.c_str());
    if (!spXMLDoc)
    {
        CoUninitialize();
        return false;
    }
    MSXML2::IXMLDOMNodeListPtr pMsgConfigList = NULL;
    MSXML2::IXMLDOMNodeListPtr pLogConfigList = NULL;
    MSXML2::IXMLDOMNodeListPtr pOTAConfigList = NULL;
    MSXML2::IXMLDOMNodeListPtr pEventIDList = NULL;
    


    //**************************************************//
    //          2. Read (1) Extended Message 
    //**************************************************//

    stringstream stMsgConfigXPath;
    stMsgConfigXPath << "/QXDMProfessional/Persistence/*/ISVConfig/MessageLevels|/QXDMProfessional/Persistence/Displays/*/View/ISVConfig/MessageLevels";
    
    pMsgConfigList = RFCal::Helper::XML::GetXMLNodes((MSXML2::IXMLDOMNodePtr)spXMLDoc,stMsgConfigXPath.str());
    
    std::multimap <int, int> iMsgMap;
    if (pMsgConfigList)
    {
        long numMsgConfig = pMsgConfigList->Getlength();

        for (long i=0; i < numMsgConfig; i++)
        {
            MSXML2::IXMLDOMElementPtr pMsgElement =  pMsgConfigList->nextNode();
            _variant_t  varValue;
            HRESULT hr = pMsgElement->get_text(&varValue.bstrVal);
            string tmpSt = RFCal::Helper::Convert::BSTR_to_String(varValue.bstrVal);

            tmpSt += ",";
        
            string delimiter = ",";
            string slashDelimiter = "/";
        
            size_t pos = 0;
            size_t sPos = 0;
            while ((pos = tmpSt.find(delimiter)) != string::npos)
            {
                string token = tmpSt.substr(0,tmpSt.find(delimiter));
                //data.push_back(token);
                if ((sPos = token.find(slashDelimiter)) != string::npos)
                {
                    string MsgCode = token.substr(0,sPos);
                    string MsgLevel = token.substr(sPos+1,token.length() - sPos -1 );
                    int iMsgCode = atoi(MsgCode.c_str());
                    int iMsgLevel = atoi(MsgLevel.c_str());
                    iMsgMap.insert(make_pair(iMsgCode,iMsgLevel));
                }
                tmpSt = tmpSt.substr(pos+1,tmpSt.length() - pos-1);
            }
        }

    }
    
    map <int, int> MsgCodeToLevel;
    
    {
        std::multimap<int, int>::iterator it;
        for (it = iMsgMap.begin(); it != iMsgMap.end(); it++)
        {
            int MsgCode = it->first;
            int MsgLevel = it->second;
            MsgCodeToLevel[MsgCode] += (1 << MsgLevel);
        }
        
    }
    //**************************************************//
    //          3. Set (1) Extended message
    //**************************************************//

    QLIB_DIAG_SetAllMessageFilter(hPhone,0);
    map <int, int>::iterator it;
    for (it = MsgCodeToLevel.begin(); it != MsgCodeToLevel.end(); it++)
    {
        unsigned short iSSID = static_cast<unsigned short>(it->first);
        unsigned long iRT_MASK = static_cast<unsigned short>(it->second);

        QLIB_DIAG_SetMessageFilter(hPhone,iSSID,iRT_MASK);
    }
    
    //**************************************************//
    //          4. Read (2) LogMas (for DiagLog)
    //**************************************************//

    stringstream stLogConfigXPath;
    stLogConfigXPath << "/QXDMProfessional/Persistence/*/ISVConfig/LogCodes|/QXDMProfessional/Persistence/Displays/*/View/ISVConfig/LogCodes";
    
    pLogConfigList =  RFCal::Helper::XML::GetXMLNodes((MSXML2::IXMLDOMNodePtr)spXMLDoc,stLogConfigXPath.str());
    set <int> LogCode;
    if (pLogConfigList)
    {
        long num = pLogConfigList->Getlength();
        for (long i=0; i < num; i++)
        {
            MSXML2::IXMLDOMElementPtr pLogElement = pLogConfigList->nextNode();
            _variant_t  varValue;
            HRESULT hr = pLogElement->get_text(&varValue.bstrVal);
            string tmpSt = RFCal::Helper::Convert::BSTR_to_String(varValue.bstrVal);

            tmpSt += ",";
        
            string delimiter = ",";
            size_t pos = 0;
            while ((pos = tmpSt.find(delimiter)) != string::npos)
            {
                string token = tmpSt.substr(0,tmpSt.find(delimiter));
                int iLogCode =  static_cast<int>(strtol(token.c_str(), NULL, 0)); //atoi(token.c_str());
                tmpSt = tmpSt.substr(pos+1,tmpSt.length() - pos-1);
                LogCode.insert(iLogCode);
            }
        }
    }

    //**************************************************//
    //          5. Read (3) OTA log ( this is the same as DiagLog)
    //**************************************************//

    stringstream stOTAConfigXPath;
    stOTAConfigXPath << "/QXDMProfessional/Persistence/*/ISVConfig/OTATypes||/QXDMProfessional/Persistence/Displays/*/View/ISVConfig/OTATypes";

    pOTAConfigList =  RFCal::Helper::XML::GetXMLNodes((MSXML2::IXMLDOMNodePtr)spXMLDoc,stOTAConfigXPath.str());
    
    if (pOTAConfigList)
    {
        long num = pOTAConfigList->Getlength();
        for (long i=0; i < num; i++)
        {
            MSXML2::IXMLDOMElementPtr pLogElement =  pOTAConfigList->nextNode();
            _variant_t  varValue;
            HRESULT hr = pLogElement->get_text(&varValue.bstrVal);

            string tmpSt = RFCal::Helper::Convert::BSTR_to_String(varValue.bstrVal);

            tmpSt += ",";
        
            string delimiter = ",";
            size_t pos = 0;
            string slashDelimiter = "/";
        
            size_t sPos = 0;
        
        
            while ((pos = tmpSt.find(delimiter)) != string::npos)
            {
                string token = tmpSt.substr(0,tmpSt.find(delimiter));
                //data.push_back(token);
                if ((sPos = token.find(slashDelimiter)) != string::npos)
                {
                    string stLogCode = token.substr(0,sPos);
                    int iLogCode =  static_cast<int>(strtol(stLogCode.c_str(), NULL, 0)); //atoi(token.c_str());
                
                    LogCode.insert(iLogCode);
                }
                tmpSt = tmpSt.substr(pos+1,tmpSt.length() - pos-1);
            
            }
        }
    }

    //**************************************************//
    //          3. Read (2) LogMask (for DiagLog)
    //**************************************************//

    {
        const byte NUM_EQUIPMENTS = 16;
        unsigned int iMaxLogID_Equipment [NUM_EQUIPMENTS];
        memset(iMaxLogID_Equipment,0,NUM_EQUIPMENTS*sizeof(UINT));
        unsigned short iOperationDataSize = NUM_EQUIPMENTS*4;
        ::QLIB_DIAG_SetExtendedLogMask(hPhone,1,(unsigned char *)iMaxLogID_Equipment,&iOperationDataSize);

        set <int>::iterator it;
        vector < vector < byte > > iOperationData;
    
        const byte HEAD=8;
        iOperationData.resize(NUM_EQUIPMENTS);
        for (UINT i=0; i < NUM_EQUIPMENTS; i++)
        {
            UINT iNumBytes =  ( iMaxLogID_Equipment[i] + 7 )/ 8;
            if (iNumBytes > 0)
            {
                iOperationData[i].resize(iNumBytes+HEAD);
                iOperationData[i].assign(iNumBytes+HEAD,0);
            }
        }
        for (it = LogCode.begin(); it != LogCode.end(); it++)
        {
            byte iEquipmentID = static_cast<byte>((*it >> 12)  & (0xF));
            word iLogId =  static_cast<word>(*it  & 0xFFF);
            word iArrayNum = iLogId/8;
            word iBit = iLogId%8;
			if (iOperationData[iEquipmentID].size() > static_cast <uint32>(iArrayNum + 8)){
                iOperationData[iEquipmentID][iArrayNum+8] += ( 1 << iBit);
            }
            else
            {
              stringstream ss;
              ss << "Ignore LogCode " << iLogId << " which is more than max log ID defined in modem";
              RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ss.str() );

            }
        }
    //**************************************************//
    //          6. Set (2) and (3) LogMask/OTA logs
    //**************************************************//  
        for (unsigned char i=0; i < NUM_EQUIPMENTS; i++)
        {
            if (iOperationData[i].size()>0)
            {   
               // if (i==1) // 1 is limit to RF. 
                {
                    UINT *pi;
                    pi = (UINT *)&(iOperationData[i][0]);
                    *pi = static_cast<UINT>(i);
                    pi = (UINT *)&(iOperationData[i][4]);
                    *pi =  static_cast<UINT>(iMaxLogID_Equipment[i]);
                            
                    unsigned short piOperationSize = static_cast<unsigned short>(iOperationData[i].size());
                    // To avoid buffer overflow in the case that modem sends back more than max log id
                    unsigned short buffersize = (piOperationSize > 1000 )? piOperationSize : 1000;
                    iOperationData[i].resize(buffersize);
                    iOperationData[i].push_back(0); // to avoid memory leak. 
                    ::QLIB_DIAG_SetExtendedLogMask(hPhone,3,(unsigned char *)&(iOperationData[i][0]),&piOperationSize);

                }
            }
        }
    }

    //**************************************************//
    //          7. Read (4) Event ID
    //**************************************************//

    stringstream stEventIDXPath;
    stEventIDXPath << "/QXDMProfessional/Persistence/*/ISVConfig/EventIDs|/QXDMProfessional/Persistence/Displays/*/View/ISVConfig/EventIDs";

    pEventIDList =  RFCal::Helper::XML::GetXMLNodes((MSXML2::IXMLDOMNodePtr)spXMLDoc,stEventIDXPath.str());
    set <unsigned short> EventCodes;
    if (pEventIDList)
    {
        long num = pEventIDList->Getlength();
        for (long i=0; i < num; i++)
        {
            MSXML2::IXMLDOMElementPtr pLogElement =  pEventIDList->nextNode();
            _variant_t  varValue;
            HRESULT hr = pLogElement->get_text(&varValue.bstrVal);
            string tmpSt = RFCal::Helper::Convert::BSTR_to_String(varValue.bstrVal);

            tmpSt += ",";
        
            string delimiter = ",";
            size_t pos = 0;
            while ((pos = tmpSt.find(delimiter)) != string::npos)
            {
                string token = tmpSt.substr(0,tmpSt.find(delimiter));
                int iLogCode =  atoi(token.c_str());
                tmpSt = tmpSt.substr(pos+1,tmpSt.length() - pos-1);
                EventCodes.insert(static_cast<word>(iLogCode));
            }
        }
    }

    //**************************************************//
    //          8. Set (4) Event ID
    //**************************************************//
    {
        ::QLIB_DIAG_SetEntireEventMaskState(hPhone, false);

        set <word>::iterator it;
        vector <unsigned short> iEventID;
        iEventID.resize(EventCodes.size());
        UINT iIndex = 0;
        for (it = EventCodes.begin(); it != EventCodes.end(); it++)
        {
            word iEventCode =(*it);
            ::QLIB_DIAG_SetEventMaskBit(hPhone, iEventCode, true); // If Filter enable, use this
            iEventID[iIndex] = iEventCode;
        }
        //QLIB_DIAG_SetEventMaskBits(hPhone,&iEventID[0],iEventID.size(),true); // if fileter is removed. use this.

    }

    //**************************************************//
    //          Release COM
    //**************************************************//

    if (pEventIDList) { pEventIDList.Release(); pEventIDList = NULL; }

    if (pOTAConfigList) { pOTAConfigList.Release(); pOTAConfigList = NULL; }
    if (pMsgConfigList) { pMsgConfigList.Release(); pMsgConfigList = NULL; }
    if (pLogConfigList) { pLogConfigList.Release(); pLogConfigList = NULL; }
    if (spXMLDoc) { spXMLDoc.Release(); spXMLDoc = NULL; }
    CoUninitialize();
    return true;
}

MSXML2::IXMLDOMNodePtr RFCal::Helper::XML::GetSingleXMLNode(MSXML2::IXMLDOMNodePtr  pNode, const string & stXPATH)
{
	try
	{
		MSXML2::IXMLDOMNodePtr pTmp = pNode->selectSingleNode((_bstr_t)stXPATH.c_str()); 
		return pTmp; 
	}
	catch(...)
	{
		return NULL;
	}

}

unsigned char RFCal::Helper::GetPathLossFromCalDB_XML(HANDLE hPhone, const string & calDBXML, const string & calConfigName, word calPathNumber, double FreqBegin,double FreqEnd,map<double,double> & freqToPathLoss)
{
	MSXML2::IXMLDOMDocumentPtr spXMLDoc = NULL;
	// 1. Check parameter
	if (! ValidateGetPathLossParameters(hPhone, spXMLDoc, calDBXML, calConfigName, calPathNumber, FreqBegin,FreqEnd)) { return false;}
	
	string tmpCalConfigName = calConfigName;
	word tmpCalPathNumber = calPathNumber;
	//**************************************************//
	//			Process CalPathLin
	//**************************************************//
	MSXML2::IXMLDOMNodePtr pCalPathLink = NULL;
	
	stringstream stXPATH_CalPathLink;
	stXPATH_CalPathLink << "/CalibrationData/CalConfig[@name='" << calConfigName << "']/CalPathLink[@srcPathNumber='" << calPathNumber << "']";
	pCalPathLink = RFCal::Helper::XML::GetSingleXMLNode((MSXML2::IXMLDOMNodePtr)spXMLDoc,stXPATH_CalPathLink.str());
	if (pCalPathLink)
	{
		
		MSXML2::IXMLDOMNodePtr pSingleCalPathNode;
	
		MSXML2::IXMLDOMElementPtr element = pCalPathLink;
        _variant_t  varValueSrcCalConfig = element->getAttribute(L"srcCalConfig");
        _variant_t  varValuedestPathNumber = element->getAttribute(L"destPathNumber");

		tmpCalConfigName = RFCal::Helper::Convert::BSTR_to_String(varValueSrcCalConfig.bstrVal);
		tmpCalPathNumber = static_cast<word>(varValuedestPathNumber);


	}
	//**************************************************//
	//			Get the root nodes giving tech and band
	//**************************************************//

	MSXML2::IXMLDOMNodePtr pRoot_Node;

	stringstream stXPATH;
	stXPATH << "/CalibrationData/CalConfig[@name='" << tmpCalConfigName << "']/CalPath[@number='" << tmpCalPathNumber << "']";

	pRoot_Node = RFCal::Helper::XML::GetSingleXMLNode((MSXML2::IXMLDOMNodePtr)spXMLDoc,stXPATH.str());

	if (pRoot_Node == NULL) 
	{
		spXMLDoc.Release();
		CoUninitialize();
      stringstream ss;
      ss << "Parameter (" << stXPATH.str() << " not found in the CalDB.";
		RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str() );
      return false;
	}

	MSXML2::IXMLDOMNodeListPtr pCalPathgNodes = NULL;

	pCalPathgNodes = RFCal::Helper::XML::GetXMLNodes(pRoot_Node,"CalPoint");

	long iNumCalConfigNodes;
	HRESULT hr = S_OK; 
	hr = pCalPathgNodes->get_length(&iNumCalConfigNodes);

	// To sort frequency, first read all the freq - pathloss data.

	map <double, double> AllFreqPassLoss;
	for (long i=0; i<iNumCalConfigNodes;i++)
	{
		MSXML2::IXMLDOMNodePtr pSingleCalPathNode;
		hr= pCalPathgNodes->get_item(i,&pSingleCalPathNode);
		
		MSXML2::IXMLDOMElementPtr element = pSingleCalPathNode;

        _variant_t  varValuefreq = element->getAttribute(L"freq");
        _variant_t  varValueloss = element->getAttribute(L"loss");

		double freq = static_cast<double>(varValuefreq);
		double loss = static_cast<double>(varValueloss);

		AllFreqPassLoss[freq] = loss;

	}
	map <double, double>::iterator it;
	for(it = AllFreqPassLoss.begin(); it !=AllFreqPassLoss.end(); it ++)
	{
		double freq = it->first;
		double loss = it->second;
		if (freq >= FreqBegin && freq <= FreqEnd)
		{
			freqToPathLoss[freq] = loss;
		}

	}
	if (pCalPathgNodes) { pCalPathgNodes.Release(); pCalPathgNodes = NULL; }
	if (pRoot_Node) { pRoot_Node.Release(); pRoot_Node = NULL; }
	if (spXMLDoc) { spXMLDoc.Release(); spXMLDoc = NULL; }

//	CoUninitialize();
	return true;
}
unsigned char RFCal::Helper::ValidateGetPathLossParameters(HANDLE hPhone, MSXML2::IXMLDOMDocumentPtr & spXMLDoc, const string & calDBXML, const string & calConfigName, word calPathNumber, double & FreqBegin ,double & FreqEnd)
{
	if ( _access( calDBXML.c_str(), 0 ) != 0 )
	{
		RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, "the file does not exist. ");
		return false;
	}
	
	//**************************************************//
	//			Load XML and get a node with the rf_mode
	//**************************************************//
	CoInitialize(NULL); // Initialize COM
	spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	VARIANT_BOOL bOK = spXMLDoc->load(calDBXML.c_str());
	if (!spXMLDoc)
	{
		CoUninitialize();
		return false;
	}
	const double MAX_FREQ = 100000;
	if (FreqBegin < 0 ) { FreqBegin = 0; }
	if (FreqEnd < 0 ) { FreqEnd = MAX_FREQ; }

	if (FreqBegin > FreqEnd )
	{
		double temp = FreqEnd;
		FreqEnd = FreqBegin;
		FreqBegin = temp;

	}

	return true;
}
string RFCal::Helper::Convert::BSTR_to_String(BSTR & bst)
{
   wstring ws = (wstring)bst;
   string tmpSt(ws.begin(),ws.end());
   return tmpSt;
}
unsigned int RFCal::Helper::Convert::XO_CAL_FT_QUAL_TO_NV(unsigned int QualityIndInFtmResponse)
{
  unsigned int nv;
  switch ( QualityIndInFtmResponse )                                                      
  {
    case _TCXOMGR_FT_QUAL_NOT_CAL:
      nv = _TCXOMGR_CAL_NV_FT_QUAL_NOT_CAL;
      break;
    case _TCXOMGR_FT_QUAL_FAC1: 
      nv = _TCXOMGR_CAL_NV_FT_QUAL_FAC1; 
      break;
    case _TCXOMGR_FT_QUAL_FT1:
      nv = _TCXOMGR_CAL_NV_FT_QUAL_FT1;
      break; 
    case _TCXOMGR_FT_QUAL_COARSE:          
      nv = _TCXOMGR_CAL_NV_FT_QUAL_COARSE;   
      break;          
    case _TCXOMGR_FT_QUAL_FINE:            
      nv = _TCXOMGR_CAL_NV_FT_QUAL_FINE;  
      break;           
    case _TCXOMGR_FT_QUAL_FT2:    
      nv = _TCXOMGR_CAL_NV_FT_QUAL_FT2;
      break;                                                           
    case _TCXOMGR_FT_QUAL_FAILED:                                          
    default:                                                                  
      nv = _TCXOMGR_CAL_NV_FT_QUAL_FAILED;         
      break;                                                                  
  }
  return nv;
}

MSXML2::IXMLDOMNodeListPtr RFCal::Helper::XML::GetXMLNodes(MSXML2::IXMLDOMNodePtr  pNode, const string & stXPATH)
{
	try
	{
		MSXML2::IXMLDOMNodeListPtr pTmp = pNode->selectNodes((_bstr_t)stXPATH.c_str()); 
		return pTmp; 
	}
	catch(...)
	{
		return NULL;
	}
}
unsigned char RFCal::Helper::DIAG::Change_FTM_Mode(HANDLE hPhone, int count, int sleepTime)
{
	unsigned char isFTMmode = false;
	QLIB_IsFTM_Mode(hPhone,&isFTMmode);
	if (!isFTMmode)
	{
		QLIB_DIAG_CONTROL_F(hPhone, MODE_FTM_F);
		do
		{
			Sleep(sleepTime);
			QLIB_IsFTM_Mode(hPhone,&isFTMmode);
			count --;
      }
      while ((!isFTMmode) && count > 0);
	}
	return isFTMmode;
}
unsigned char RFCal::Helper::DIAG::ForcedCrash(HANDLE hPhone)
{
	unsigned char request[] = {75,37,03,00};
   unsigned char pbSupportsDiag = false;
   unsigned char pbSupportsEFS = false;
   unsigned char pbSupportsSwDownload = false;
   unsigned char pbUsingQPST = false;
   QLIB_GetLibraryCapabilities( &pbSupportsDiag, &pbSupportsEFS, &pbSupportsSwDownload, &pbUsingQPST );

   if ( pbUsingQPST )
   {
      // QPST mode
      unsigned char res[100];
      short resSize = sizeof(res)/sizeof(unsigned char);
      QLIB_SendSync(hPhone,sizeof(request)/sizeof(unsigned char),request, &resSize, res,true);
   }
   else
   {
      // QphoneMode
      QLIB_SendASync(hPhone,sizeof(request)/sizeof(unsigned char),request,2000,true);
   }
   return true;
}

#define ROUNDL( d ) ((long)((d) + ((d) > 0 ? 0.5 : -0.5)))
double RFCal::Helper::Convert::ROUND(double numToRound, int precision)
{
   if( precision < 0)
      precision = 0;

   double multiplier = pow(10.0,precision);
   double dNum = multiplier * numToRound;
   dNum = ROUNDL(dNum);

   return dNum/(1.0*multiplier);
}
unsigned char RFCal::Helper::NV::WriteCalRFNVToQCN(HANDLE hPhone, const string & filepath, unsigned char bWriteToDUT)
{
	//*********************************************//
	//		Write NV to a Phone
	//*********************************************//	
	if (bWriteToDUT)
	{
		QMSL_RFCal_WriteToPhone_Status _status;
		unsigned char status_efs;
		string sFilePath = "/";
		QLIB_RFCAL_NV_Manager_WriteToPhone( hPhone,&_status);
		QLIB_EFS2_SyncWithWait(hPhone,(unsigned char*)(sFilePath.c_str()),1000,&status_efs);
	}
	//*********************************************//
	//		Write NV to a QCN
	//*********************************************//
	return QLIB_RFCAL_NV_Manager_Create_QCN( hPhone, (char*)filepath.c_str() );
}
