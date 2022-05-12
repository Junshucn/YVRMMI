/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/RFCal_Helper.h#11 $
 * $DateTime: 2017/09/11 22:55:00 $
 *
*
******************************************************************************
*
 * Copyright (c) 2016 - 2017 Qualcomm Technologies, Inc.
* All rights reserved.
* Qualcomm Technologies, Inc. Confidential and Proprietary.
*
 ******************************************************************************
 */

#pragma once
#include <typeinfo>
#import <msxml6.dll> 
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <io.h>   // For access().
#ifndef __QLIB_H
#define __QLIB_H
#include "QLib.h"
#endif

#ifndef __QLIB_DEFINES_H
#define __QLIB_DEFINES_H
#include "QLib_Defines.h"
#endif

using namespace std;
namespace RFCal
{
   namespace Helper
   {
      namespace XML
      {
         template <typename T> unsigned char Read_Attribute(MSXML2::IXMLDOMNodePtr parentNode,const string & stAttribute, T & parameter);
         /* Read attributes on XPATH from the node*/
         template <typename T> unsigned char Read_Attribute(MSXML2::IXMLDOMNodePtr parentNode,const string & XPath, const string & stAttribute, T & parameter);
         /* Read 2-D vector type parameters on XPATH from the node*/
         template <typename T> unsigned char Read_Param(MSXML2::IXMLDOMNodePtr parentNode,const string & XPath, vector < vector <T> > & parameter);
         /* Read 1-D vector type parameters on XPATH */
         template <typename T> unsigned char Read_Param(MSXML2::IXMLDOMNodePtr parentNode,const string & XPath, vector <T> & parameter);
         /* Read parameter on XPATH */
         template <typename T> unsigned char Read_Param(MSXML2::IXMLDOMNodePtr parentNode,const string & XPath, T & parameter); // Remove default parameter
         /* Read result data type = 0 is input node, type = 1 is output node */
         
         unsigned char Read_Str_Attribute(MSXML2::IXMLDOMNodePtr parentNode,const string & stAttribute, string & parameter);
         unsigned char GetXMLRootNode(const string & stFilePath, const string & stXPath); 
         MSXML2::IXMLDOMNodePtr GetSingleXMLNode(MSXML2::IXMLDOMNodePtr  pNode, const string & stXPATH);
         MSXML2::IXMLDOMNodeListPtr GetXMLNodes(MSXML2::IXMLDOMNodePtr  pNode, const string & stXPATH);
      }
      namespace Log
      {
         static DWORD logStartTime;
         void AddMessageToQMSLLog( HANDLE hPhone, const string & Log, unsigned char bShowConsole = true );
         void AddTitleToQMSLLog(HANDLE hPhone, const string & Log, unsigned char bShowConsole = true);
         unsigned char StartQMSLLogging(HANDLE hPhone, const string & filename, unsigned char bAddAsync = 0);
         unsigned char StopQMSLLogging(HANDLE hPhone);
      }
      namespace Convert
      {
         int ULch_To_DLch(int tech, int band, int subband, long ulChannel);
         int DLch_To_ULch(int tech, int band, int subband, long ulChannel);
         unsigned char ULchannel_To_Frequency(int TECH, int band, int ULChannel, double & ULFrequency, double & DLFrequency);
         int DLPower_To_ExpectedAgc(unsigned char tech, double cellPower);
         int GetRFMode(int tech, int band, int subband);
         int GetTech_From_RFMode(FTM_RF_Mode_Enum rfMode);
         int GetBandNumber_From_RFMode(FTM_RF_Mode_Enum rfMode);
         int GetGSMBand(int band);
         string BSTR_to_String(BSTR & bst);
		 string TechID_To_Str (int tech);
		 unsigned int XO_CAL_FT_QUAL_TO_NV(unsigned int QualityIndInFtmResponse);
		 BAND_CLASS_ENUM ConverGSMBand_To_BandClassEnum (unsigned int gsmBand);
		 FTM_RF_Mode_Enum  GSMBandClassToRFMode(const BAND_CLASS_ENUM bandType);
		 double ROUND(double numToRound, int precision);

         template<typename T> unsigned char Str_To_Value(const string & stValue, T & value);

         template<typename T> unsigned char Str_To_Vector( vector < T > & res, const char * cStr);
         template<typename T> unsigned char Str_To_Vector( vector < T > & res, string & str);
         template<typename T> unsigned char Str_To_Vector( vector < vector < T > > & res, string & str);
      }
      namespace DIAG
      {
         /* These are diag..., not FTM */
         unsigned char RebootPhone (HANDLE hPhone);
         unsigned char Change_FTM_Mode(HANDLE hPhone, int count, int sleepTime);
         unsigned char ForcedCrash(HANDLE hPhone);
         unsigned char IsPhoneConneted(HANDLE hPhone, int count, int sleepTime);
         unsigned char GetBuildID(HANDLE hPhone, std::string & stBuildID);
         unsigned char Set_DMC_File(HANDLE hPhone, const string & stDmcFilePath);
      }
      unsigned char GetPathLossFromCalDB_XML(HANDLE hPhone, const string & calDBXML, const string & calConfigName, word calPathNumber, double FreqBegin,double FreqEnd, std::map<double,double> & freqToPathLoss);
      unsigned char ValidateGetPathLossParameters(HANDLE hPhone, MSXML2::IXMLDOMDocumentPtr & spXMLDoc, const string & calDBXML, const string & calConfigName, word calPathNumber, double & FreqBegin ,double & FreqEnd);
      template  <typename T> T GetNextIterator ( T Itr )
      {
         return Itr++;
      }
      namespace NV
      {
         unsigned char WriteCalRFNVToQCN(HANDLE hPhone, const string & filepath, unsigned char bWriteToDUT);
      }
   }
}

// Template implementation 

template<typename T> unsigned char RFCal::Helper::XML::Read_Attribute<T>(MSXML2::IXMLDOMNodePtr parentNode,const string & stAttribute, T & parameter)
{
   if (parentNode == NULL)
   {
      parameter = static_cast<T>(0);
      return false;
   }
	MSXML2::IXMLDOMNamedNodeMap* pParentAttributes;
	parentNode->get_attributes(&pParentAttributes);
	MSXML2::IXMLDOMNodePtr pCategoryNameNode = pParentAttributes->getNamedItem((_bstr_t)(stAttribute.c_str()));   
	if (pCategoryNameNode!=NULL)
	{
		_variant_t varValue;
		pCategoryNameNode->get_nodeValue(&varValue);

		if ( varValue.vt == VT_BSTR && SysStringLen(varValue.bstrVal) == 0)
		{
		   return false;
		}

		parameter = static_cast<T>(varValue);
		return true;
	}
	//varValue = pParentElement->getAttribute((_bstr_t)(stAttribute.c_str()));
	else
	{

		parameter = static_cast<T>(0); 
		return false;
	}
};
template<typename T> unsigned char RFCal::Helper::XML::Read_Attribute(MSXML2::IXMLDOMNodePtr parentNode,const string & XPath, const string & stAttribute, T & parameter)
{
   if(parentNode == NULL)
   {
      return false;
   }
	//**************************************************//
	//			Get a text from given XPath node 
	//**************************************************//
	MSXML2::IXMLDOMNodePtr pXPathNode = parentNode->selectSingleNode((_bstr_t)(XPath.c_str()));
	if (pXPathNode == NULL) return false;

	MSXML2::IXMLDOMNamedNodeMap* pParentAttributes;
	pXPathNode->get_attributes(&pParentAttributes);
	MSXML2::IXMLDOMNodePtr pCategoryNameNode = pParentAttributes->getNamedItem((_bstr_t)(stAttribute.c_str()));   
	if (pCategoryNameNode!=NULL)
	{
      
      wstring ws = (wstring)(pCategoryNameNode->text);
	   string tmpSt(ws.begin(),ws.end());
      
      if ( tmpSt.find ("0x") != std::string::npos )
      {
         stringstream ss;
         ss << tmpSt;
         ss >> parameter;
      }
      else
      {
         RFCal::Helper::Convert::Str_To_Value(tmpSt, parameter );
      }
      
   	return true;
	}

	pXPathNode.Release();
	return true;
}
template<typename T> unsigned char RFCal::Helper::XML::Read_Param(MSXML2::IXMLDOMNodePtr parentNode,const string & XPath, vector < vector <T> > & parameter)
{
   if(parentNode == NULL)
   {
      return false;
   }
	//**************************************************//
	//			Get a text from given XPath node 
	//**************************************************//
	MSXML2::IXMLDOMNodePtr pXPathNode = parentNode->selectSingleNode((_bstr_t)(XPath.c_str()));
	if (pXPathNode == NULL) return false;
	MSXML2::IXMLDOMElementPtr pParentElement(pXPathNode);
	BSTR text;
	pParentElement->get_text(&text);

	//**************************************************//
	//			Split the text a vector < string >
	//       Delimiter is ';'. If not, try ','
	//**************************************************//
	wstring ws = (wstring)text;
	string tmpSt(ws.begin(),ws.end());
	stringstream  ss(tmpSt);
    string item;
	vector <string> elems;
   while (std::getline(ss, item, ';'))
   {
		elems.push_back(item);
   }
	if (elems.size() == 1)
	{
		elems.clear();
		string tmpSt(ws.begin(),ws.end());
		stringstream  ss(tmpSt);
		string item;
		
      while (std::getline(ss, item, ','))
      {
			elems.push_back(item);
		}
	}
	parameter.clear();
	parameter.resize(elems.size());

	//**************************************************//
	//			Convert each of string to T type
	//**************************************************//
	for (UINT i=0; i < elems.size(); i++)
	{
		if ((elems[i].length() > 7) && (elems[i].substr(0,3).compare("SSS") == 0))
		{
			stringstream value (elems[i].substr(4,elems[i].length()));
			string strStartRGI;
			string strEndRGI;
			string strStepRGI;
			std::getline(value,strStartRGI,',');
			std::getline(value,strEndRGI,',');
			std::getline(value,strStepRGI,')');
			int tmpVal[3];
			RFCal::Helper::Convert::Str_To_Value(strStartRGI, tmpVal[0]);
			RFCal::Helper::Convert::Str_To_Value(strEndRGI, tmpVal[1]);
			RFCal::Helper::Convert::Str_To_Value(strStepRGI, tmpVal[2]);
			for (int index = tmpVal[0]; index >= tmpVal[1]; index += tmpVal[2])
			{
				parameter[i].push_back(static_cast<T>(index));
			}
		} 
		else if ((elems[i].length() > 6) && (elems[i].substr(0,2).compare("MC") == 0))
		{
			stringstream value (elems[i].substr(3,elems[i].length()));
			string strValue;
			string strNumRepeats;
			std::getline(value,strValue,',');
			std::getline(value,strNumRepeats,')');
			int tmpVal[2];
			RFCal::Helper::Convert::Str_To_Value(strValue, tmpVal[0]);
			RFCal::Helper::Convert::Str_To_Value(strNumRepeats, tmpVal[1]);

			for (int index = 0; index < tmpVal[1]; index ++)
			{
				parameter[i].push_back(static_cast<T>( tmpVal[0] ));
			}
		}
		else 
		{
			vector <string> sub_elems;
			stringstream  sub_ss(elems[i]);
			string sub_item;
		
			while (std::getline(sub_ss, sub_item, ','))
			{
				sub_elems.push_back(sub_item);
			}
			for (UINT j=0; j < sub_elems.size(); j++)
			{
				T tmpVal;
				RFCal::Helper::Convert::Str_To_Value(sub_elems[j], tmpVal);
				
				parameter[i].push_back(static_cast<T>( tmpVal ));
			}
		}
	}
	pXPathNode.Release();
	return true;
}
template<typename T> unsigned char RFCal::Helper::XML::Read_Param(MSXML2::IXMLDOMNodePtr parentNode, const string & XPath, vector <T> & parameter)
{
   if(parentNode == NULL)
   {
      return false;
   }
	//**************************************************//
	//			Get a text from given XPath node 
	//**************************************************//
	MSXML2::IXMLDOMNodePtr pXPathNode = parentNode->selectSingleNode((_bstr_t)(XPath.c_str()));
	if (pXPathNode == NULL) return false;
	MSXML2::IXMLDOMElementPtr pParentElement(pXPathNode);

	BSTR text;
	pParentElement->get_text(&text);

	//**************************************************//
	//			Split the text into a vector <string>
	//**************************************************//
	wstring ws = (wstring)text;
	string tmpSt(ws.begin(),ws.end());
   parameter.clear();
   RFCal::Helper::Convert::Str_To_Vector(parameter, tmpSt);

	//stringstream  ss(tmpSt);
 //  string item;
	//vector <string> elems;
 //  while (std::getline(ss, item, ','))
 //  {
 //       elems.push_back(item);
 //   }
	//parameter.clear();

	////**************************************************//
	////			Convert each of string to T type
	////**************************************************//
	//for (UINT i=0; i < elems.size(); i++)
	//{
	//	T tmp;
	//	RFCal::Helper::Convert::Str_To_Value(elems[i] , tmp);
	//	parameter.push_back(tmp);
	//}
	pXPathNode.Release();
	return true;
}
template<typename T> unsigned char RFCal::Helper::XML::Read_Param(MSXML2::IXMLDOMNodePtr parentNode, const string & XPath, T& parameter)
{
   if(parentNode == NULL)
   {
      return false;
   }
	//**************************************************//
	//			Get a text from given XPath node 
	//**************************************************//
	MSXML2::IXMLDOMNodePtr pXPathNode = parentNode->selectSingleNode((_bstr_t)(XPath.c_str()));
   if (pXPathNode == NULL)
   {
      return false;
   }
	MSXML2::IXMLDOMElementPtr pParentElement(pXPathNode);
	BSTR text;
	pParentElement->get_text(&text);
	wstring ws = (wstring)text;
	string tmpSt(ws.begin(),ws.end());
	//**************************************************//
	//			Convert the text to T type
	//**************************************************//
	T tmp;
	RFCal::Helper::Convert::Str_To_Value(tmpSt, tmp);
	parameter = tmp;
	pXPathNode.Release();
	return true;
}
template<typename T> unsigned char RFCal::Helper::Convert::Str_To_Value(const string & stValue, T & value)
{
   if (stValue.compare("TRUE") == 0  || stValue.compare("true") == 0 )
   {
      value = true;
      return true;
   }
   if (stValue.compare("FALSE") == 0 || stValue.compare("false") == 0 )
   {
      value = false;
      return true;
   }

	if ( typeid(T) == typeid(double) || typeid(T) == typeid(float) )
	{
		istringstream (stValue) >> value;
	}
	else
	{
		long lTmp;		
		istringstream (stValue) >> lTmp;
		value = static_cast<T>(lTmp);
	}
	return true;
}
 template<typename T> unsigned char  RFCal::Helper::Convert::Str_To_Vector(vector < T > & res, const char * cStr)
 {
    string tmpSt(cStr);
    return RFCal::Helper::Convert::Str_To_Vector( res, tmpSt);
 }
template<typename T> unsigned char RFCal::Helper::Convert::Str_To_Vector(vector < T > & res, string & str )
{
   if (str.empty() || str.length() == 0)
   {
      return false;
   }

	stringstream  ss(str);
   string item;
	vector <string> elems;
   while (std::getline(ss, item, ','))
   {
        elems.push_back(item);
    }


	//**************************************************//
	//			Convert each of string to T type
	//**************************************************//
	for (UINT i=0; i < elems.size(); i++)
	{
		T tmp;
		RFCal::Helper::Convert::Str_To_Value(elems[i] , tmp);
		res.push_back(tmp);
	}
   return true;
}

template<typename T> unsigned char RFCal::Helper::Convert::Str_To_Vector(vector < vector < T > > & res, string & str )
{ 
   //**************************************************//
	//			Split the text a vector < string >
	//       Delimiter is ';'. If not, try ','
	//**************************************************//
  
	stringstream  ss(str);
   string item;
	vector <string> elems;
   while (std::getline(ss, item, ';'))
   {
		elems.push_back(item);
   }
	if (elems.size() == 1)
	{
		elems.clear();
		string tmpSt(str.begin(),str.end());
		stringstream  ss(tmpSt);
		string item;
		
      while (std::getline(ss, item, ','))
      {
			elems.push_back(item);
		}
	}
	res.clear();
	res.resize(elems.size());

	//**************************************************//
	//			Convert each of string to T type
	//**************************************************//
	for (UINT i=0; i < elems.size(); i++)
	{
		if ((elems[i].length() > 7) && (elems[i].substr(0,3).compare("SSS") == 0))
		{
			stringstream value (elems[i].substr(4,elems[i].length()));
			string strStartRGI;
			string strEndRGI;
			string strStepRGI;
			std::getline(value,strStartRGI,',');
			std::getline(value,strEndRGI,',');
			std::getline(value,strStepRGI,')');
			int tmpVal[3];
			RFCal::Helper::Convert::Str_To_Value(strStartRGI, tmpVal[0]);
			RFCal::Helper::Convert::Str_To_Value(strEndRGI, tmpVal[1]);
			RFCal::Helper::Convert::Str_To_Value(strStepRGI, tmpVal[2]);
			for (int index = tmpVal[0]; index >= tmpVal[1]; index += tmpVal[2])
			{
				res[i].push_back(static_cast<T>(index));
			}
		} 
		else if ((elems[i].length() > 6) && (elems[i].substr(0,2).compare("MC") == 0))
		{
			stringstream value (elems[i].substr(3,elems[i].length()));
			string strValue;
			string strNumRepeats;
			std::getline(value,strValue,',');
			std::getline(value,strNumRepeats,')');
			int tmpVal[2];
			RFCal::Helper::Convert::Str_To_Value(strValue, tmpVal[0]);
			RFCal::Helper::Convert::Str_To_Value(strNumRepeats, tmpVal[1]);

			for (int index = 0; index < tmpVal[1]; index ++)
			{
				res[i].push_back(static_cast<T>( tmpVal[0] ));
			}
		}
		else 
		{
			vector <string> sub_elems;
			stringstream  sub_ss(elems[i]);
			string sub_item;
		
			while (std::getline(sub_ss, sub_item, ','))
			{
				sub_elems.push_back(sub_item);
			}
			for (UINT j=0; j < sub_elems.size(); j++)
			{
				T tmpVal;
				RFCal::Helper::Convert::Str_To_Value(sub_elems[j], tmpVal);
				
				res[i].push_back(static_cast<T>( tmpVal ));
			}
		}
	}
   return true;
}