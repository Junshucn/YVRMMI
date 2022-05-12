#pragma once
#include "COMMON.h"
#include "MMI_TESTDlg.h"
#include "ComPortEx.h"

// PHONE

class PHONE : public CWnd
{
	DECLARE_DYNAMIC(PHONE)

public:
	PHONE();
	virtual ~PHONE();

protected:
	DECLARE_MESSAGE_MAP()

public:
	BOOL SendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,float TimeOut_s,CString &ERRCODE);
	BOOL HMBSendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,float TimeOut_s,CString &ERRCODE);
	BOOL KeySendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,CString &ReadKeyValue,float TimeOut_s,CString &ERRCODE);
	BOOL Function_RendAtCommond(int comport,int baudrate,CString &ReadData_String,float TimeOut_s,CString &ERRCODE);
	BOOL Function_SendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,float TimeOut_s,CString &ERRCODE);
	BOOL Function_Close_AtCommond(int comport,int baudrate,float TimeOut_s,CString &ERRCODE);
	BOOL ReadDataOnly(int comport,int baudrate,CString &ReadData_String,float TimeOut_s,CString &ERRCODE);
	BOOL UC15ENSendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,CString &ReadKeyValue,float TimeOut_s,CString &ERRCODE);
	BOOL HM003SendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,CString &ReadKeyValue,float TimeOut_s,CString &ERRCODE);
	BOOL WDOGSendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,CString &ReadKeyValue,float TimeOut_s,CString &ERRCODE);
	BOOL SendCommondOnly(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,float TimeOut_s,CString &ERRCODE);
	BOOL WDOGTestCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,CString &ReadKeyValue,float TimeOut_s,CString &ERRCODE);
};


