/******************************************************************************/
/**
Program: QMSL

	$Id: //depot/HTE/QDART/QMSL6/inc/QMSL_WP/QMSL_FTM_WP.h#1 $1 $

 \b QUALCOMM  \b PROPRIETARY

   This document contains propriety information, and except with written
   permission of Qualcomm Technologies, INC, such information shall not be
   published, or disclosed to others, or used for any purpose, and the
   document shall not be duplicated in whole or in part.


	Copyright (c) 2004-2013 QUALCOMM Technologies, Incorporated.
   All Rights Reserved.
   Qualcomm Confidential and Proprietary

\version
Version information is stored in the file "QMSL_Release_Notes.txt"

\note
Compiler:  Microsoft Visual C++ v6.0 SP4
*******************************************************************************/

#if !defined(_QMSL_FTM_WP_H)
#define _QMSL_FTM_WP_H

#if !defined(_WINDOWS_)
#include "windows.h"
#endif

/**
	Establish whether the library is being compiled into a DLL (exporting),
	or being included from a client (importing)

	When the DLL is built, then QLIB_EXPORTS should be defined
*/
#ifdef QLIB_API
#undef QLIB_API
#endif

#ifdef QLIB_EXPORTS
#define QLIB_API __declspec(dllexport)
#else
#define QLIB_API __declspec(dllimport)
#endif

#if defined(QLIB_STATIC)
#undef QLIB_API
#define QLIB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/******************************************************************************/
	/**
	WINDOWS PHONE 8.1 method 

	Factory Test Mode(FFBM/MMOS) : Windows Phone 8.1 test command: FTM_WP_FUSE_CHECK

	Check if fuses are blown correctly. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param sData this returns a 8 byte return value related to the fuses blown
   
	\param iFTM_Error_Code FTM Error Code
							0 = Success
						 
	\return true if successful, false if fail.

	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_WP_FUSE_CHECK(HANDLE hResourceContext, unsigned char* sData, unsigned long* iFTM_Error_Code);

	/******************************************************************************/

	/**
	WINDOWS PHONE 8.1 method 

	Factory Test Mode(FFBM/MMOS) : Windows Phone test command: FTM_WP_PROVISION_FILE

	Provision single file onto device provisioning partition(DPP)

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param sFileName This is the filename of the candidate file,t he candidate file is required to be uploaded to device
					prior to this command.

	\param sFile_DPP_Type, this parameter denotes the type of the DPP item the sFileName is to be 
							provisioned as, where 0 = QCOM and 1 = OEM

	\param iFTM_Error_Code FTM Error Code
							0 = Success						  

	\return true if successful, false if fail.

   
	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_WP_PROVISION_FILE(HANDLE hResourceContext, char* sFileName, unsigned short* sFile_DPP_Type, unsigned long* iFTM_ERROR_CODE );

	/******************************************************************************/

	/**
	WINDOWS PHONE 8.1 method 

	Factory Test Mode(FFBM/MMOS) : Windows Phone test command: FTM_WP_PROVISION_SSD

	Provision Secure Software Download (SSD) keystore.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param sFileName, this is the SSD filename, the SSD key is required to be uploaded to device 
					before calling this command.
	.

	\param iFTM_Error_Code FTM Error Code
							0 = Success						  

	\return true if successful, false if fail.

   
	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_WP_PROVISION_SSD(HANDLE hResourceContext, char* sFileName, unsigned long* iFTM_ERROR_CODE);

	/******************************************************************************/
	/**
	WINDOWS PHONE 8.1 method 

	Factory Test Mode(FFBM/MMOS) : Windows Phone test command: FTM_WP_PROVISION_HDCP_CEK

	Provision HDCP 2.x CEK key

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param sFileName this is the filename of the file containing the HDCP 2.x CEK key we wish to provision onto the windows phone.
					the file should be uploaded onto the device before calling this command.

	\param iFTM_Error_Code FTM Error Code
							0 = Success						  

	\return true if successful, false if fail.

   
	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_WP_PROVISION_HDCP_CEK(HANDLE hResourceContext, char* sFileName, unsigned long* iFTM_ERROR_CODE);

	/******************************************************************************/
	/**
	WINDOWS PHONE 8.1 method 

	Factory Test Mode(FFBM/MMOS) : Windows Phone test command: FTM_WP_PROVISION_HDCP_DATA

	Provision HDCP 2.x DATA key.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param sFileName this is the filename of the file containing the HDCP 2.x DATA key we wish to provision onto the windows phone.
					the file should be uploaded onto the device before calling this command.

	\param iFTM_Error_Code FTM Error Code
							0 = Success						  

	\return true if successful, false if fail.

   
	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_WP_PROVISION_HDCP_DATA(HANDLE hResourceContext, char* sFileName, unsigned long* iFTM_ERROR_CODE);

	/******************************************************************************/
	/**
	WINDOWS PHONE 8.1 method 

	Factory Test Mode(FFBM/MMOS) : Windows Phone test command: FTM_WP_PROVISION_OPM_PUB

	Provision PVP-OPM public key. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param sFileName this is the filename of the file containing the PVP-OPM public key we wish to provision onto the windows phone.
					the file should be uploaded onto the device before calling this command

	\param iFTM_Error_Code FTM Error Code
							0 = Success						  

	\return true if successful, false if fail.

   
	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_WP_PROVISION_OPM_PUB(HANDLE hResourceContext, char* sFileName, unsigned long* iFTM_ERROR_CODE);

	/******************************************************************************/
	/**
	WINDOWS PHONE 8.1 method 

	Factory Test Mode(FFBM/MMOS) : Windows Phone test command: FTM_WP_PROVISION_OPM_PRIV

	Provision PVP-OPM private key. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param sFileName this is the filename of the file containing the PVP-OPM private key we wish to provision onto the windows phone.
					the file should be uploaded onto the device before calling this command

	\param iFTM_Error_Code FTM Error Code
							0 = Success						  

	\return true if successful, false if fail.

	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_WP_PROVISION_OPM_PRIV(HANDLE hResourceConttext, char* sFileName, unsigned long* iFTM_ERROR_CODE);

	/******************************************************************************/
	/**
	WINDOWS PHONE 8.1 method 

	Factory Test Mode(FFBM/MMOS) : Windows Phone test command: FTM_WP_GET_DEVICE_UNIQUE_ID

	Get the unique device id for the windows phone. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param ovk_variable this is the unique 32 byte id that is generated by the phone.

	\param iFTM_Error_Code FTM Error Code
							0 = Success						  

	\return true if successful, false if fail.

   
	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_WP_GET_DEVICE_UNIQUE_ID(HANDLE hResourceContext, unsigned char* ovk_variable, unsigned long* iFTM_ERROR_CODE);

	/******************************************************************************/
	/**
	WINDOWS PHONE 8.1 method 

	Factory Test Mode(FFBM/MMOS) : Windows Phone test command: FTM_WP_PROVISION_OVK_HASH

	Provision PVP-OPM private key. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param sFileName this is the filename of the file containing the signed OVK hash private key we wish to provision onto the windows phone.
					the file should be uploaded onto the device before calling this command

	\param iFTM_Error_Code FTM Error Code
							0 = Success						  

	\return true if successful, false if fail.

	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_WP_PROVISION_OVK_HASH(HANDLE hResourceContext, char* sFileName, unsigned long* iFTM_ERROR_CODE);

	/******************************************************************************/
	/**
	WINDOWS PHONE 8.1 method 

	Factory Test Mode(FFBM/MMOS) : Windows Phone test command: FTM_WP_PROVISION_OVK_HASH

	Disable DIAG and factory services, switch USB transport from Serial composite mode to MTP mode, disable test 
	signing, enable secureboot. 
 
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param iFTM_Error_Code FTM Error Code
							0 = Success						  

	\return true if successful, false if fail.

	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_WP_LOCK_DEVICE(HANDLE hResourceContext, unsigned long* iFTM_ERROR_CODE);

	/******************************************************************************/


#ifdef __cplusplus
}	
#endif

#endif	
