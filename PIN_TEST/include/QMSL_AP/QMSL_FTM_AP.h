/******************************************************************************/
/**
Program: QMSL

	$Id: //depot/HTE/QDART/QMSL6/inc/QMSL_AP/QMSL_FTM_AP.h#4 $1 $

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
#if !defined(_QMSL_FTM_AP_H)
#define _QMSL_FTM_AP_H

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
   Factory Test Mode(FFBM/MMOS) : Application processor test command: FTM_AP_TEST_APP_STATUS
   
   Get MMI Test Application Status

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   
   \param iFTM_Error_Code FTM Error Code
                          0 = Success
						  1 = Error

   \param iState  State of MMI test execution
					0 = Idle
	                1 = Executing
           
   \param iFail_Count The number of test failed in MMI application
 
   \return true if successful, false if fail.

   \note. The user must reset the device

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_AP_TEST_APP_STATUS ( HANDLE hResourceContext, unsigned short* iFTM_Error_Code, unsigned char* iState, unsigned int* iFail_Count);
   /******************************************************************************/
   /**
   Factory Test Mode(FFBM/MMOS) : Application processor test command: FTM_AP_SELECT_SEQUENCE

   Specify the test sequence file (.seq/.cfg) to load.  Any error in loading .seq/.cfg file will cause 
   MMI Test application to use the default sequence file (MMI.seq).  
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   
   \param iFTM_Error_Code FTM Error Code
                          0 = Success
						  1 = Error

   \param iNum_Tests = The number of test available in the sequence

   \return true if successful, false if fail.

   \note. The user must reset the device

   *******************************************************************************/   
   QLIB_API unsigned char QLIB_FTM_AP_SELECT_SEQUENCE ( HANDLE hResourceContext, char* sSequenceFile, unsigned short* iFTM_Error_Code, unsigned short *iNum_Test );
   /******************************************************************************/
   /**
   Factory Test Mode(FFBM/MMOS) : Application processor test command: FTM_AP_CLEAR_RESULTS

   Erase the content of the test result file (.res) for the current selected sequence loaded by command FTM_AP_SELECT_SEQUENCE
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   
   \param iFTM_Error_Code FTM Error Code
                          0 = Success
						  1 = Error

   \return true if successful, false if fail.

   \note. The user must reset the device

   *******************************************************************************/   
   QLIB_API unsigned char QLIB_FTM_AP_CLEAR_RESULTS ( HANDLE hResourceContext, unsigned short* iFTM_Error_Code);
   /******************************************************************************/
   /**
   Factory Test Mode(FFBM/MMOS) : Application processor test command: FTM_AP_EXECUTE_SINGLE_TEST

   Execute a single MMI test case specified by the test name
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   
   \param iFTM_Error_Code FTM Error Code
                          0 = Success
						  1 = Error

   \param sTestCase Test Case Name in ASCII

   \return true if successful, false if fail.

   \note. The user must reset the device

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_AP_EXECUTE_SINGLE_TEST ( HANDLE hResourceContext, unsigned short* iFTM_Error_Code, char* sTestCase);
   /******************************************************************************/
   /**
   Factory Test Mode(FFBM/MMOS) : Application processor test command: FTM_AP_EXECUTE_SINGLE_TEST

   Execute a single MMI test case specified by the test name
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   
   \param iFTM_Error_Code FTM Error Code
                          0 = Success
						  1 = Error

   \param sTestCase Test Case Name in ASCII

   \param iTestStatus, The test status returned by the MMI test case

   \param iTimeoutInS, Timeout value in seconds.  If the timeout value is 0, this API will not wait for asychronus log message returned.

   \param iTimeoutOcurred, 1 = Time out occurred.
                           0 = Time out did not occur.

   \return true if successful, false if fail.

   \note. The user must reset the device

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_AP_EXECUTE_SINGLE_TEST_WithWait ( HANDLE hResourceContext, unsigned short* iFTM_Error_Code, char* sTestCase, unsigned long* iTestStatus,unsigned short iTimeoutInS, unsigned char*iTimeoutOcurred);
   /******************************************************************************/
   /**
   Factory Test Mode(FFBM/MMOS) : Application processor test command: FTM_AP_EXECUTE_ALL_TESTS

   Start the execution of tests in the selected sequence based on the sequence file (.seq) loaded by command FTM_AP_SELECT_SEQUENCE
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   
   \param iFTM_Error_Code FTM Error Code
                          0 = Success
						  1 = Error

   \return true if successful, false if fail.

   \note. The user must reset the device

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_AP_EXECUTE_ALL_TESTS ( HANDLE hResourceContext, unsigned short* iFTM_Error_Code);
   QLIB_API unsigned char QLIB_FTM_AP_EXECUTE_ALL_TESTS_WithWait ( HANDLE hResourceContext, unsigned short* iFTM_Error_Code, unsigned long* iTestStatus,unsigned short iTimeoutInS, unsigned char*iTimeoutOcurred);
   /******************************************************************************/
   /**
   Factory Test Mode(FFBM/MMOS) : Application processor test command: FTM_AP_READ_FILE

   Download a file from the FTM_AP folder to the PC
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   
   \param sRemoteFileName, the remote filename on the target

   \param sLocalFileName, the local filename on the PC

   \param iFTM_Error_Code FTM Error Code
                          0 = Success
						  1 = Error

   \return true if successful, false if fail.

   \note. The user must reset the device

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_AP_READ_FILE ( HANDLE hResourceContext, unsigned short* iFTM_Error_Code, char * sRemoteFileName, char * sLocalFileName );
   /******************************************************************************/
   /**
   Factory Test Mode(FFBM/MMOS) : Application processor test command: FTM_AP_WRITE_FILE

   Upload a file from the PC to the FTM_AP folder on target
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
 
   \param sRemoteFileName, the remote filename on the target

   \param sLocalFileName, the local filename on the PC

   \param iFTM_Error_Code FTM Error Code
                          0 = Success
						  1 = Error

   \return true if successful, false if fail.

   \note. The user must reset the device

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_AP_WRITE_FILE ( HANDLE hResourceContext, unsigned short* iFTM_Error_Code, char * sRemoteFileName, char * sLocalFileName );
   /******************************************************************************/
   /**
   Factory Test Mode(FFBM/MMOS) : Application processor test command: FTM_AP_ERASE_ALL_FILES

   Erase all files in FTM_AP directory
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   
   \param iFTM_Error_Code FTM Error Code
                          0 = Success
						  1 = Error

   \return true if successful, false if fail.

   \note. The user must reset the device

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_AP_ERASE_ALL_FILES ( HANDLE hResourceContext, unsigned short* iFTM_Error_Code);
   /******************************************************************************/
   /**
   Factory Test Mode(FFBM/MMOS) : Application processor test command: FTM_AP_TEST_LIST_TO_FILE

   Publish the list of available test identified by TEST_NAME and VERSION to a file called TestList.Txt, in the FTM_AP directory
   
   \param iFTM_Error_Code FTM Error Code
                          0 = Success
						  1 = Error

   \return true if successful, false if fail.

   \note. The user must reset the device

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_AP_TEST_LIST_TO_FILE ( HANDLE hResourceContext, unsigned short* iFTM_Error_Code);
   /******************************************************************************/
   /**
   Factory Test Mode(FFBM/MMOS) : Application processor test command: FTM_AP_DIR_TO_FILE

   List the FTM_AP directory contents to a file Directory.Txt, in the FTM_AP directory.  
   The Directory.Txt file can later be retrieved to desktop with FTM_AP_READ_FILE command.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   
   \param iFTM_Error_Code FTM Error Code
                          0 = Success
						  1 = Error

   \return true if successful, false if fail.

   \note. The user must reset the device

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_AP_DIR_TO_FILE ( HANDLE hResourceContext, unsigned short* iFTM_Error_Code);
#ifdef __cplusplus
}	// extern "C"
#endif

#endif	// defined(_QMSL_FTM_AP_H)
