#include "stdafx.h"
#include "MMI_TEST.h"
#include "MYWORK.h"
#include "MessageDialog.h"
#include "messagecheckpower.h"
#include "WebService_DLL.h"
#include "Qualcomm.h"
#include <fstream>
#include <iostream>
#include "json/json.h"
#include <cassert>
#include <time.h>

#using "..\Release\HYWSMMI200414.dll"
#using "..\Debug\SFISTransfer.dll"

//#include "WebService_DLL.h"
//#pragma comment(lib,"WebService_DLL.lib")

MYWORK* LPMYWORK;

extern CPIN_TESTDlg*	lpCPIN_TESTDlg;
extern CONFIG			m_CONFIG;
extern AllTestItem		m_TestItem;
extern EXCELTestItem    m_EXCELTestItem;
extern DefEXCELTestItem m_DefEXCELTestItem;
extern MMIDATA          m_MMIDATA;
extern MessageBoxDlg*   lpMessageBoxDlg;
MessageDialog           m_MessageDialog;
MessageBoxDlg           m_MessageBoxDlg;
PowerControl			m_PowerControl;
PHONE					m_PHONE;
ScanPort				m_struct_ScanPort;
ScanSN	m_ScanSN;
CQualcomm m_Qualcomm;
BOOL THREAD_WORK=FALSE;

CString			g_sMessageBoxTilte;
extern char	cErrCode[100][100];

// MYWORK
IMPLEMENT_DYNAMIC(MYWORK, CWnd)

MYWORK::MYWORK()
{
	LPMYWORK=this;
}

MYWORK::~MYWORK()
{
}

BEGIN_MESSAGE_MAP(MYWORK, CWnd)
END_MESSAGE_MAP()
// MYWORK 消息处理程序

static UINT TEST_THREAD(LPVOID LPARAM)
{
	LPMYWORK->TEST_THREAD_ENTER_POINT();
	return 0;
}

void MYWORK::ThreadManager() {
	if (!THREAD_WORK) {
		THREAD_WORK=TRUE;
		AfxBeginThread(TEST_THREAD,this,THREAD_PRIORITY_NORMAL,0,0,NULL);	
	}
}

void MYWORK::TEST_THREAD_ENTER_POINT() {
	int RES=SUCCESS;
	lpCPIN_TESTDlg->m_BTN_START.EnableWindow(FALSE);
	lpCPIN_TESTDlg->m_InputSn.EnableWindow(FALSE);
	lpCPIN_TESTDlg->m_InputPhoneSn.EnableWindow(FALSE);
	lpCPIN_TESTDlg->GetDlgItem(IDC_LEFTSN)->EnableWindow(FALSE);
	lpCPIN_TESTDlg->ClearTestLog();
	lpCPIN_TESTDlg->ClearDebugLog();
	lpCPIN_TESTDlg->ShowProgress(0);
	lpCPIN_TESTDlg->ShowState(RUN);
	lpCPIN_TESTDlg->RunTime=0;
	lpCPIN_TESTDlg->SetTimer(1,1000,NULL);
	if (m_CONFIG.Test_Selection == HANDLE_PCBA_MMI_TEST) RES = HANDLE_PCBA_MMI_EXC();//手柄功能
	else if (m_CONFIG.Test_Selection == HANDLE_COUPLING_TEST) RES = HDM_COUPLING_EXC();//耦合测试
	else if (m_CONFIG.Test_Selection == HDM_COUPLING_TEST) RES = HDM_COUPLING_EXC();//耦合测试
	else if(m_CONFIG.Test_Selection == HDM_PAIR_TEST) RES = HDM_PAIR_EXC();//PAIR站位
	else if(m_CONFIG.Test_Selection == HDM_CHECK_PACK) RES = HDM_CHECK_EXC();//Write站位
	else if (m_CONFIG.Test_Selection == QA_PAIR_MESCHECK_PACK) RES = MES_PAIR_INFORMATION_EXC();//查询配对
	if (!RES) lpCPIN_TESTDlg->ShowState(PASS);
	else lpCPIN_TESTDlg->ShowState(FAIL);
	lpCPIN_TESTDlg->ResultStatistics(RES);
	lpCPIN_TESTDlg->KillTimer(1);
	lpCPIN_TESTDlg->m_BTN_START.EnableWindow(TRUE);
	if (m_CONFIG.Test_Selection== HANDLE_PCBA_MMI_TEST || m_CONFIG.Test_Selection== HDM_COUPLING_TEST || m_CONFIG.Test_Selection == HANDLE_COUPLING_TEST
		|| m_CONFIG.Test_Selection== HDM_PAIR_TEST || m_CONFIG.Test_Selection == QA_PAIR_MESCHECK_PACK || m_CONFIG.Test_Selection == HDM_CHECK_PACK) {
		lpCPIN_TESTDlg->m_InputPhoneSn.SetWindowTextA("");
		lpCPIN_TESTDlg->m_InputPhoneSn.EnableWindow(TRUE);
		lpCPIN_TESTDlg->m_InputSn.SetWindowTextA("");
		lpCPIN_TESTDlg->m_InputSn.EnableWindow(TRUE);
		lpCPIN_TESTDlg->GetDlgItem(IDC_LEFTSN)->SetWindowTextA("");
		lpCPIN_TESTDlg->GetDlgItem(IDC_LEFTSN)->EnableWindow(TRUE);
		lpCPIN_TESTDlg->PostMessage(WM_INIT_FOCUS_SCANER,0U,m_CONFIG.Test_Selection);
		lpCPIN_TESTDlg->m_InputPhoneSn.SetFocus();
	} 
	else {
		lpCPIN_TESTDlg->m_InputSn.SetWindowTextA("");
		lpCPIN_TESTDlg->m_InputSn.EnableWindow(TRUE);
		lpCPIN_TESTDlg->PostMessage(WM_INIT_FOCUS_SCANER,0U,m_CONFIG.Test_Selection);
		lpCPIN_TESTDlg->m_InputSn.SetFocus();
	}
}

#pragma region MESMethod
BOOL MYWORK::MES_MMIUPLOAD(CString SN,bool bTestResult,int iERRCODE)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-------------------------WebService1 START-------------------------");
	WebService1 wsHYWSMMI200414;
	//改URL
	CString IpAddress="",sTemp="";
	CString sHYReturn="";
	IpAddress="http://192.168.10.10:8899/WebService1.asmx";//For Chongqi
	//IpAddress="http://183.64.84.26:8899/WebService1.asmx";//For Shanghai
	lpCPIN_TESTDlg->OutPutDebugLog(IpAddress);
	System::String^ strIpAddress = gcnew System::String(IpAddress);
	wsHYWSMMI200414.Url=strIpAddress; 

	if (""==m_MMIDATA.sEXTempter) m_MMIDATA.sEXTempter="NA";
	if (""==m_MMIDATA.sEXHUMIDTY) m_MMIDATA.sEXHUMIDTY="NA";
	System::String^ strHYRetCharp = gcnew System::String(sHYReturn);//上传结果
	System::String^ strBSNCharp = gcnew System::String(SN);
	System::String^ strHWVerCharp = gcnew System::String(m_MMIDATA.sUC15Ver);
	System::String^ strSWVerCharp = gcnew System::String(m_MMIDATA.sMCUVer);
	System::String^ strSEQCharp = gcnew System::String(m_MMIDATA.sSEQ);
	System::String^ strBVolCharp = gcnew System::String(m_MMIDATA.sBVol);
	System::String^ strTempterCharp = gcnew System::String(m_MMIDATA.sTempter);
	System::String^ strHUMIDTYCharp = gcnew System::String(m_MMIDATA.sHUMIDTY);
	System::String^ strEXTempterCharp = gcnew System::String(m_MMIDATA.sEXTempter);
	System::String^ strEXHUMIDTYCharp = gcnew System::String(m_MMIDATA.sEXHUMIDTY);
	System::String^ strLSensorCharp = gcnew System::String(m_MMIDATA.sLSENSOR);
	System::String^ strGSensorCharp = gcnew System::String(m_MMIDATA.sGsensor);
	System::String^ strGPSCoordiCharp = gcnew System::String(m_MMIDATA.sGPS_Coordinate);
	System::String^ strGPSStarCharp = gcnew System::String(m_MMIDATA.sGPS_Star);
	System::String^ strResultCharp = gcnew System::String(m_MMIDATA.sResult);
	try
	{
		lpCPIN_TESTDlg->OutPutDebugLog("MES MMIUpload Start");
		sTemp.Format("SN:%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
			strBSNCharp,strHWVerCharp,strSWVerCharp,strSEQCharp,
			strBVolCharp,strTempterCharp,strHUMIDTYCharp,strEXTempterCharp,
			strEXHUMIDTYCharp,strLSensorCharp,strGSensorCharp,
			strGPSCoordiCharp,strGPSStarCharp);
		lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
		strHYRetCharp=wsHYWSMMI200414.HM003UploadMMIData(strBSNCharp,strHWVerCharp,strSWVerCharp,strSEQCharp,strBVolCharp,strTempterCharp,strHUMIDTYCharp,strEXTempterCharp,strEXHUMIDTYCharp,strLSensorCharp,strGSensorCharp,strGPSCoordiCharp,strGPSStarCharp,"PASS");
		sHYReturn=CString(strHYRetCharp);
		if(sHYReturn.Find("NG")!=-1)	
		{
			lpCPIN_TESTDlg->OutPutDebugLog(sHYReturn);
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result,"-",FALSE);	
			lpCPIN_TESTDlg->OutPutDebugLog("MES MMIUpload Station Fail");
			return false;
		}
		else
		{
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result,"-",TRUE);	
			lpCPIN_TESTDlg->OutPutDebugLog("MES MMIUpload Station Success");
			return true;
		}
	}
	catch (...)
	{
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result,"-",FALSE);	
		lpCPIN_TESTDlg->OutPutDebugLog("MES MMI UPLOAD Link Fail，Please Check NetWork Connection!");
		return false;
	}
}

BOOL MYWORK::MES_PASS_RFSTATION(CString SN, int iERRCODE)
{
	CString sUploadData = "", sType = "", sOutPut = "", sStatus = "", sErrCode = "",sTemp="";
	try {
		lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------MES Result Station-----------------------------");
		System::String^ strSNCharp = gcnew System::String(SN);
		System::String^ strResCharp = "";
		if (m_CONFIG.iProject == D1_YVR) {
			CommonSrv ^WSObject = gcnew CommonSrv();
			int iCurrstep = atoi(m_CONFIG.sMES_Line);
			System::String^ strF_CodeCharp = "";
			System::String^ strORG_IDCharp = gcnew System::String(sType);
			System::String^ strStationNameCharp = gcnew System::String(m_CONFIG.sMESStationName);
			if (!iERRCODE) {
				sStatus = "OK";
				sErrCode = "";
			}
			else {
				sStatus = "NG";
				sErrCode.Format("%s", cErrCode[iERRCODE]);
			}
			System::String^ strerror_reasonCharp = gcnew System::String(sErrCode);
			System::String^ strresultCharp = gcnew System::String(sStatus);
			sTemp.Format("MES insertData,%s,%s,%s", SN, m_CONFIG.sMES_Line, m_CONFIG.sMESStationName);
			lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
			strResCharp = WSObject->insertData("", strF_CodeCharp, "", "", strSNCharp, iCurrstep, strStationNameCharp, strerror_reasonCharp, strORG_IDCharp, strresultCharp, "", "");
		}
		if (m_CONFIG.iProject == D2_YVR) {
			SFISTransfer::SFISTransfer ^SFISObject = gcnew SFISTransfer::SFISTransfer();//和硕MES
			System::String^ strUserID = gcnew System::String(m_CONFIG.stu_SFISConfig.sUserID);//UserID
			System::String^ strsDeviceID = gcnew System::String(m_CONFIG.stu_SFISConfig.sDeviceID);//sDeviceID
			System::String^ strsStationName = gcnew System::String(m_CONFIG.stu_SFISConfig.sStationName);//sStationName
			if (!iERRCODE) {
				sUploadData.Format("%s,1,%s,%f,%f", m_CONFIG.stu_Message.sTestSelectionMessage[m_CONFIG.Test_Selection],
					m_TestItem.sAverageRssi,
					m_TestItem.STI_averageRssi.rangeMin,
					m_TestItem.STI_averageRssi.rangeMax);
				lpCPIN_TESTDlg->OutPutDebugLog(sUploadData);
			}
			else {
				sUploadData.Format("%s,0,%s,%f,%f", m_CONFIG.stu_Message.sTestSelectionMessage[m_CONFIG.Test_Selection],
				m_TestItem.sAverageRssi,
				m_TestItem.STI_averageRssi.rangeMin,
				m_TestItem.STI_averageRssi.rangeMax);
				lpCPIN_TESTDlg->OutPutDebugLog(sUploadData);
			}
			System::String^ strUploadDataCharp = gcnew System::String(sUploadData);
			System::String^ strerror_reasonCharp = gcnew System::String(cErrCode[iERRCODE]);//ErrorCode
			sTemp.Format("MES UploadSFIS,%s,%s,%s,%s,%s", SN, m_CONFIG.stu_SFISConfig.sUserID, m_CONFIG.stu_SFISConfig.sDeviceID, m_CONFIG.stu_SFISConfig.sStationName, cErrCode[iERRCODE]);
			lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
			bool result = SFISObject->UploadSFIS(strSNCharp, strUserID, strsDeviceID, strsStationName, strUploadDataCharp, strerror_reasonCharp, strResCharp);
			if (result == false) {
				lpCPIN_TESTDlg->OutPutDebugLog("CheckRoute error");
				return false;
			}
		}
		sOutPut = CString(strResCharp);
		if ((sOutPut.Find("OK") != -1) || (sOutPut.Find("Pass") != -1)) {
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			lpCPIN_TESTDlg->OutPutDebugLog("MES Result Station Pass");
			return true;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			lpCPIN_TESTDlg->OutPutDebugLog("MES Result Station Fail");
			return false;
		}
	}
	catch (...)
	{
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, "Crash", FALSE);
		lpCPIN_TESTDlg->OutPutDebugLog("MES  Result Station Fail");
		return false;
	}
}

BOOL MYWORK::MES_PASS_PAIRSTATION(CString SN, CString sLeft, CString sRight, bool bTestResult, int iERRCODE)
{
	CString sUploadData = "", sType = "", sOutPut = "", sStatus = "", sErrCode = "",sTemp="";
	try {
		lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------MES Result Station-----------------------------");
		System::String^ strSNCharp = gcnew System::String(SN);
		System::String^ strResCharp = "";
		if (m_CONFIG.iProject == D1_YVR) {
			CommonSrv ^WSObject = gcnew CommonSrv();
			int iCurrstep = atoi(m_CONFIG.sMES_Line);
			System::String^ strF_CodeCharp = "";
			System::String^ strORG_IDCharp = gcnew System::String(sType);
			System::String^ strStationNameCharp = gcnew System::String(m_CONFIG.sMESStationName);
			if (bTestResult) {
				sStatus = "OK";
				sErrCode = "";
			}
			else {
				sStatus = "NG";
				sErrCode.Format("%s", cErrCode[iERRCODE]);
			}
			System::String^ strerror_reasonCharp = gcnew System::String(sErrCode);
			System::String^ strresultCharp = gcnew System::String(sStatus);
			sTemp.Format("MES insertData,%s,%s,%s", SN, m_CONFIG.sMES_Line, m_CONFIG.sMESStationName);
			lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
			strResCharp = WSObject->insertData("", strF_CodeCharp, "", "", strSNCharp, iCurrstep, strStationNameCharp, strerror_reasonCharp, strORG_IDCharp, strresultCharp, "", "");
		}
		if (m_CONFIG.iProject == D2_YVR) {
			SFISTransfer::SFISTransfer ^SFISObject = gcnew SFISTransfer::SFISTransfer();//和硕MES
			System::String^ strUserID = gcnew System::String(m_CONFIG.stu_SFISConfig.sUserID);//UserID
			System::String^ strsDeviceID = gcnew System::String(m_CONFIG.stu_SFISConfig.sDeviceID);//sDeviceID
			System::String^ strsStationName = gcnew System::String(m_CONFIG.stu_SFISConfig.sStationName);//sStationName
			if (bTestResult) {
				sUploadData.Format("%s,1,PASS\r\n#SAVE:SNA,1,%s\r\n#SAVE:SNB,1,%s", m_CONFIG.stu_Message.sTestSelectionMessage[m_CONFIG.Test_Selection],
					                                                                sLeft,sRight);
				lpCPIN_TESTDlg->OutPutDebugLog(sUploadData);
			}
			else {
				sErrCode.Format("%s", cErrCode[iERRCODE]);
			}
			System::String^ strUploadDataCharp = gcnew System::String(sUploadData);
			System::String^ strerror_reasonCharp = gcnew System::String(sErrCode);
			sTemp.Format("MES UploadSFIS,%s,%s,%s,%s", SN, m_CONFIG.stu_SFISConfig.sUserID, m_CONFIG.stu_SFISConfig.sDeviceID, m_CONFIG.stu_SFISConfig.sStationName);
			lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
			bool result = SFISObject->UploadSFIS(strSNCharp, strUserID, strsDeviceID, strsStationName, strUploadDataCharp, strerror_reasonCharp, strResCharp);
			if (result == false) {
				lpCPIN_TESTDlg->OutPutDebugLog("CheckRoute error");
				return false;
			}
		}
		sOutPut = CString(strResCharp);
		if ((sOutPut.Find("OK") != -1) || (sOutPut.Find("Pass") != -1)) {
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			lpCPIN_TESTDlg->OutPutDebugLog("MES Result Station Pass");
			return true;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			lpCPIN_TESTDlg->OutPutDebugLog("MES Result Station Fail");
			return false;
		}
	}
	catch (...)
	{
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, "Crash", FALSE);
		lpCPIN_TESTDlg->OutPutDebugLog("MES  Result Station Fail");
		return false;
	}
}

BOOL MYWORK::MES_PASS_STATION(CString SN,bool bTestResult,int iERRCODE)
{
	CString sUploadData = "", sType = "", sOutPut = "", sStatus="", sErrCode="", sTemp="";
	try {
		lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------MES Result Station-----------------------------");
		System::String^ strSNCharp = gcnew System::String(SN);
		System::String^ strResCharp = "";
		if (m_CONFIG.iProject == D1_YVR) {
			CommonSrv ^WSObject = gcnew CommonSrv();
			int iCurrstep = atoi(m_CONFIG.sMES_Line);
			System::String^ strF_CodeCharp = "";
			System::String^ strORG_IDCharp = gcnew System::String(sType);
			System::String^ strStationNameCharp = gcnew System::String(m_CONFIG.sMESStationName);
			if (bTestResult) {
				sStatus = "OK";
				sErrCode = "";
			}
			else {
				sStatus = "NG";
				sErrCode.Format("%s", cErrCode[iERRCODE]);
			}
			System::String^ strerror_reasonCharp = gcnew System::String(sErrCode);
			System::String^ strresultCharp = gcnew System::String(sStatus);
			sTemp.Format("MES insertData,%s,%s,%s", SN, m_CONFIG.sMES_Line, m_CONFIG.sMESStationName);
			lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
			strResCharp = WSObject->insertData("", strF_CodeCharp, "", "", strSNCharp, iCurrstep, strStationNameCharp, strerror_reasonCharp, strORG_IDCharp, strresultCharp, "", "");
		}
		if (m_CONFIG.iProject == D2_YVR) {
			SFISTransfer::SFISTransfer ^SFISObject = gcnew SFISTransfer::SFISTransfer();//和硕MES
			System::String^ strUserID = gcnew System::String(m_CONFIG.stu_SFISConfig.sUserID);//UserID
			System::String^ strsDeviceID = gcnew System::String(m_CONFIG.stu_SFISConfig.sDeviceID);//sDeviceID
			System::String^ strsStationName = gcnew System::String(m_CONFIG.stu_SFISConfig.sStationName);//sStationName
			if (bTestResult) {
				sUploadData.Format("%s,1,%s,%f,%f",m_CONFIG.stu_Message.sTestSelectionMessage[m_CONFIG.Test_Selection],
					                               m_TestItem.sAverageRssi,
					                               m_TestItem.STI_averageRssi.rangeMin,
					                               m_TestItem.STI_averageRssi.rangeMax  );
			}
			else {
				sErrCode.Format("%s", cErrCode[iERRCODE]);
			}
			System::String^ strUploadDataCharp = gcnew System::String(sUploadData);
			System::String^ strerror_reasonCharp = gcnew System::String(sErrCode);
			sTemp.Format("MES UploadSFIS,%s,%s,%s,%s", SN, m_CONFIG.stu_SFISConfig.sUserID, m_CONFIG.stu_SFISConfig.sDeviceID, m_CONFIG.stu_SFISConfig.sStationName);
			lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
			bool result = SFISObject->UploadSFIS(strSNCharp, strUserID, strsDeviceID, strsStationName, strUploadDataCharp, strerror_reasonCharp, strResCharp);
			if (result == false) {
				lpCPIN_TESTDlg->OutPutDebugLog("CheckRoute error");
				return false;
			}
		}
		sOutPut = CString(strResCharp);
		if ((sOutPut.Find("OK") != -1) || (sOutPut.Find("Pass") != -1)) {
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			lpCPIN_TESTDlg->OutPutDebugLog("MES Result Station Pass");
			return true;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			lpCPIN_TESTDlg->OutPutDebugLog("MES Result Station Fail");
			return false;
		}
	}
	catch (...)
	{
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result,"Crash",FALSE);	
		lpCPIN_TESTDlg->OutPutDebugLog("MES  Result Station Fail");
		return false;
	}
}

BOOL MYWORK::MES_CHECK_STATION(CString SN) {
	CString sType = "", sOutPut = "", sTemp="";
	try {
		lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------MES Check Station-----------------------------");
		System::String^ strSNCharp = gcnew System::String(SN);//SN，共用
		System::String^ strResCharp = "";//返回值,共用
		if (m_CONFIG.iProject == D1_YVR) {
			CommonSrv ^WSObject = gcnew CommonSrv(); //大东MES
			int iCurrstep = atoi(m_CONFIG.sMES_Line);//站别
			System::String^ strF_CodeCharp = "";//F_Code
			System::String^ strORG_IDCharp = gcnew System::String(sType);//ID
			sTemp.Format("MES getData,%s,%s", SN, m_CONFIG.sMES_Line);
			lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
			strResCharp = WSObject->getData(strF_CodeCharp, strSNCharp, iCurrstep, strORG_IDCharp);
		}
		if (m_CONFIG.iProject == D2_YVR) {
			SFISTransfer::SFISTransfer ^SFISObject = gcnew SFISTransfer::SFISTransfer();//和硕MES
			System::String^ strUserID = gcnew System::String(m_CONFIG.stu_SFISConfig.sUserID);//UserID
			System::String^ strsDeviceID = gcnew System::String(m_CONFIG.stu_SFISConfig.sDeviceID);//sDeviceID
			System::String^ strsStationName = gcnew System::String(m_CONFIG.stu_SFISConfig.sStationName);//sStationName
			sTemp.Format("MES UploadSFIS,%s,%s,%s,%s", SN, m_CONFIG.stu_SFISConfig.sUserID, m_CONFIG.stu_SFISConfig.sDeviceID, m_CONFIG.stu_SFISConfig.sStationName);
			lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
			bool result = SFISObject->CheckRoute(strSNCharp, strUserID, strsDeviceID, strsStationName, strResCharp);
			if (result==false) {
				lpCPIN_TESTDlg->OutPutDebugLog("CheckRoute error");
				return false;
			}
		}
		sOutPut = CString(strResCharp);
		if ((sOutPut.Find("OK")!=-1) || (sOutPut.Find("Pass") != -1)) {
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			lpCPIN_TESTDlg->OutPutDebugLog("MES Check Station Pass");
			return true;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			lpCPIN_TESTDlg->OutPutDebugLog("MES Check Station Fail");
			return false;
		}
	}
	catch (...) {
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, "Crash", FALSE);
		lpCPIN_TESTDlg->OutPutDebugLog("MES Check Station Fail");
		return false;
	}
}

BOOL MYWORK::MES_GETMAC_STATION(CString SN, CString& sWIFIMAC, CString& sBTMAC) {
	CString sType = "", sOutPut = "", sTemp = "";
	try {
		lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------MES GETMAC Station-----------------------------");
		System::String^ strSNCharp = gcnew System::String(SN);//SN，共用
		System::String^ strResCharp = "";//返回值,共用
		if (m_CONFIG.iProject == D1_YVR) {
			CommonSrv ^WSObject = gcnew CommonSrv(); //大东MES
			sTemp.Format("MES getMac,%s", SN);
			lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
			strResCharp = WSObject->getMAC(strSNCharp);
			sOutPut = CString(strResCharp);
			if (""!= sOutPut){
				CStringArray ca;
				Tools_splitStr(sOutPut, "|", ca);
				sWIFIMAC = ca.GetAt(0);
				sBTMAC = ca.GetAt(1);
				lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
				lpCPIN_TESTDlg->OutPutDebugLog("MES GetMac Station Pass");
				return true;
			}
			else {
				lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
				lpCPIN_TESTDlg->OutPutDebugLog("MES GetMac Station Fail");
				return false;
			}
		}

		if (m_CONFIG.iProject == D2_YVR) {
			//SFISTransfer::SFISTransfer ^SFISObject = gcnew SFISTransfer::SFISTransfer();//和硕MES
			//System::String^ strUserID = gcnew System::String(m_CONFIG.stu_SFISConfig.sUserID);//UserID
			//System::String^ strsDeviceID = gcnew System::String(m_CONFIG.stu_SFISConfig.sDeviceID);//sDeviceID
			//System::String^ strsStationName = gcnew System::String(m_CONFIG.stu_SFISConfig.sStationName);//sStationName
			//sTemp.Format("MES UploadSFIS,%s,%s,%s,%s", SN, m_CONFIG.stu_SFISConfig.sUserID, m_CONFIG.stu_SFISConfig.sDeviceID, m_CONFIG.stu_SFISConfig.sStationName);
			//lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
			//bool result = SFISObject->CheckRoute(strSNCharp, strUserID, strsDeviceID, strsStationName, strResCharp);
			//if (result == false) {
			//	lpCPIN_TESTDlg->OutPutDebugLog("CheckRoute error");
			//	return false;
			//}
		}

	}
	catch (...) {
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, "Crash", FALSE);
		lpCPIN_TESTDlg->OutPutDebugLog("MES Check Station Fail");
		return false;
	}
}

BOOL MYWORK::MES_MCCHECK_MK_STATION(CString PhoneSN,CString PcbaSN,CString SoftVer,CString sScanIMEI1,CString sScanMEID,CString sScanBTMAC,CString sScanWIFIMAC,CString &sMEBSSN,CString &sMESPhoneSN,CString &sMESIMEI1,CString &sMESMEID,CString &sMESBTMAC,CString &sMESWIFIMAC)
{
	CString output="";
	CString sOutput="";
	char *str_getvalu=NULL;
	CArray<CString,CString>ValueList;
	CString IpAddress=m_CONFIG.sMESURL;
	CString StationName=m_CONFIG.sMESStationName;
	CString WorkOrder=m_CONFIG.sPO;  //工单
	CString Line=m_CONFIG.sMES_Line;
	CString MES_USER=m_CONFIG.sMESUser;
	CString FateName=m_CONFIG.sFateName;
	CString JIG_Num=m_CONFIG.sJigNum;//夹具编号
	CString sinfo="";
	int index=0;
	try
	{
		lpCPIN_TESTDlg->OutPutDebugLog("MES CheckStation and GetNumber...");
		//900113990900001
//SN:M10WMCK917000304;IMEI:868955040003036
//SN:M10WMCK917000304;IMEI:868955040003036

		//HMB1PCK918000333
		//<string xmlns="testInter">OK,,70DEF9968A9D,868955040003036,,,HMB1PCK918000333,M10WMCK917000304</string>
		//<string xmlns="testInter">OK,,70DEF9968A9D,868955040003036,,,,M10WMCK917000304</string>
		//if(!_MCcheckMK(IpAddress,PhoneSN,NULL,PcbaSN,StationName,WorkOrder,SoftVer,Line,MES_USER,FateName,JIG_Num,sOutput))
		//{
		//	lpCPIN_TESTDlg->OutPutDebugLog(sOutput);
		//	lpCPIN_TESTDlg->OutPutDebugLog("MES MMIStation Check Fail");
		//	return false;
		//}
		//else
		//{
		//	string str = sOutput;
		//	CString szSymbol =",";
		//	int st = str.find("OK",0) +3;
		//	string ssnr = str.substr(st,str.length() - st);
		//	output = ssnr.c_str();
		//	for(int i =1; i<= 19; i++)
		//	{
		//		//sMESPhoneSN=GetTextFromStringEx(output,szSymbol,i);		//OK,,70DEF9968A4C,868955040002228,,,,M10WMCK917000223		
		//		if(i == 1)
		//		{
		//			sMESBTMAC=GetTextFromStringEx(output,szSymbol,i);
		//		}
		//		if(i == 2)
		//		{
		//			sMESWIFIMAC=GetTextFromStringEx(output,szSymbol,i);

		//		}
		//		if(i == 3)
		//		{
		//			sMESIMEI1=GetTextFromStringEx(output,szSymbol,i);//IMEI
		//		}
		//		if(i == 6)
		//		{
		//			sMESPhoneSN=GetTextFromStringEx(output,szSymbol,i);//主板SN

		//		}
		//		if(i == 7)
		//		{
		//			sMEBSSN=GetTextFromStringEx(output,szSymbol,i);//模块SN  充当PSN

		//		}
		//		if(i == 17)
		//		{
		//			sMESMEID=GetTextFromStringEx(output,szSymbol,i);

		//		}

		//	}
		//	return true;
		//}

	}
	catch(...)
	{
		lpCPIN_TESTDlg->OutPutDebugLog(output);
		lpCPIN_TESTDlg->OutPutDebugLog("MES CheckStation and GetNumber Fail");
		return false;
	}
}

bool MYWORK::MES_MCRESULT_STATION(CString Fzjsn,CString NetSN,CString PcbaSN,CString SoftVer,CString IMEI1,CString MEID,CString BT_MAC,CString WIFI_MAC,int ERRCODE)
{
	CString sOutput="";
	char *str_getvalu=NULL;
	CArray<CString,CString>ValueList;

	CString IpAddress=m_CONFIG.sMESURL;
	CString StationName=m_CONFIG.sMESStationName;
	CString WorkOrder=m_CONFIG.sPO;  //工单
	CString Line=m_CONFIG.sMES_Line;
	CString MES_USER=m_CONFIG.sMESUser;
	CString FateName=m_CONFIG.sFateName;
	CString JIG_Num=m_CONFIG.sJigNum;//夹具编号

	CString Result="";
	CString sERRCODE="";
	CString sERRINFO="";

	if (ERRCODE == 0)
	{
		Result="OK";
	}
	else
	{
		Result="NG";
		sERRCODE.Format("%s",ERRCODE);
	}

	try
	{
		lpCPIN_TESTDlg->OutPutDebugLog("MES Pass Station...");
		if(!_MCresult(IpAddress,Fzjsn,NetSN,PcbaSN,StationName,Line,WorkOrder,SoftVer,MES_USER,FateName,JIG_Num,Result,sERRCODE,sERRINFO))
		{
			lpCPIN_TESTDlg->OutPutDebugLog("MES PassStation and UpLoad Fail");
			return false;
		}
		else
		{
			lpCPIN_TESTDlg->OutPutDebugLog("MES PassStation and UpLoad Success");
			return true;
		}
	}
	catch (...)
	{
		lpCPIN_TESTDlg->OutPutDebugLog("MES Pass Station Fail");
		return false;
	}

}

BOOL MYWORK::MES_UPDATE_DATA(CString SN, bool bTestResult)
{
	CString sTemp = "", sType = "", sOutPut = "", JIG_Num = "";
	CommonSrv ^WSObject = gcnew CommonSrv();
	System::String^ strSNCharp = gcnew System::String(SN);
	int iCurrstep = atoi(m_CONFIG.sMES_Line);
	System::String^ strF_CodeCharp = "";
	System::String^ strORG_IDCharp = gcnew System::String(sType);
	System::String^ strStationNameCharp = gcnew System::String(m_CONFIG.sMESStationName);

	System::String^ strResCharp = "";
	if (bTestResult) {
		//Result = "OK";
		//ERRCODE = "";
	}
	else {
		//Result = "NG";
		//sERRCODE.Format("%s", iERRCODE);
	}
	try {
		lpCPIN_TESTDlg->OutPutDebugLog("MES Check Station...");
		strResCharp = WSObject->getData(strF_CodeCharp, strSNCharp, iCurrstep, strORG_IDCharp);
		sOutPut = CString(strResCharp);
		if (sOutPut == "OK") {
			lpCPIN_TESTDlg->OutPutDebugLog("MES Check Station Pass");
			return true;
		}
		if (sOutPut == "NG") {
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			lpCPIN_TESTDlg->OutPutDebugLog("MES Check Station Fail");
			return false;
		}
	}
	catch (...) {
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result,"-",FALSE);	
		lpCPIN_TESTDlg->OutPutDebugLog("MES  update data  Fail");
		return false;
	}
}

BOOL MYWORK::MES_CHECK_DEVICE_MES_STATUS(CString bindSN)
{
	
	CString IpAddress="http://192.168.10.10:8899/WebService1.asmx";
	CString sOutput="";
	try
	{
		lpCPIN_TESTDlg->OutPutDebugLog("MES>>CHECK WHETHER THE DEVICE_ID HAS BEEN USED...");
		//if(!_isUseBindSN(IpAddress,bindSN,sOutput))
		//{
		//	lpCPIN_TESTDlg->OutPutDebugLog(sOutput);
		//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK_DEVICE_ID_STATUS,sOutput,FALSE);	
		//	lpCPIN_TESTDlg->OutPutDebugLog("MES CALL INTERFACE FAILED.");
		//	return false;
		//}
		//else
		//{
		//	if (sOutput.Find("OK")!=-1)//ok
		//	{
		//		lpCPIN_TESTDlg->OutPutDebugLog(sOutput);
		//		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK_DEVICE_ID_STATUS,sOutput,TRUE);	
		//		lpCPIN_TESTDlg->OutPutDebugLog("MES>>THE DEVICE_ID IS NOT BEEN USE.");
		//		return TRUE;
		//	}
		//	else//not ok.
		//	{
		//		lpCPIN_TESTDlg->OutPutDebugLog(sOutput);
		//		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK_DEVICE_ID_STATUS,sOutput,false);	
		//		lpCPIN_TESTDlg->OutPutDebugLog("MES>>THE DEVICE_ID HAS BEEN USE.");
		//		return FALSE;
		//	}
		//	
		//}
	}
	catch (...)
	{
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK_DEVICE_ID_STATUS,"调用MES接口出错!",FALSE);	
		lpCPIN_TESTDlg->OutPutDebugLog("MES CALL INTERFACE ERROR, ABOUT TRY CATCH(...).");
		return false;
	}
	return false;
}

bool MYWORK::MES_PHONE_LPcheck(CString Fzjsn,CString Fdbsn,CString SoftVer,CString &IMEI1,CString &MEID,CString &BT_MAC,CString &WIFI_MAC,CString &PSN,CString &sOutput)
{
	CString Fjwsn=NULL;
	CString IMEI2="";
	char *str_getvalu=NULL;
	CArray<CString,CString>ValueList;

	CString IpAddress=m_CONFIG.sMESURL;
	CString StationName=m_CONFIG.sMESStationName;
	CString WorkOrder=m_CONFIG.sPO;  //工单
	CString Line=m_CONFIG.sMES_Line;
	CString MES_USER=m_CONFIG.sMESUser;
	CString FateName=m_CONFIG.sFateName;
	CString JIG_Num=m_CONFIG.sJigNum;//夹具编号
	try
	{
		lpCPIN_TESTDlg->OutPutDebugLog("MES CheckStation and GetNumber...");
		lpCPIN_TESTDlg->OutPutDebugLog("Fzjsn="+Fzjsn+" Fjwsn="+Fjwsn+" Fdbsn="+Fdbsn);
		if(!_LPcheck(IpAddress,Fzjsn,Fjwsn,Fdbsn,StationName,WorkOrder,SoftVer,Line,MES_USER,FateName,JIG_Num,sOutput))
		{
			lpCPIN_TESTDlg->OutPutDebugLog(sOutput);
			return false;
		}
		else
		{
			lpCPIN_TESTDlg->OutPutDebugLog(sOutput);

			if (sOutput.Find("NG")!=-1)
			{
				lpCPIN_TESTDlg->OutPutDebugLog("MES CheckStation and GetNumber Fail");
				return false;
			}

			for (int i=0;i<50;i++)
			{
				str_getvalu=NULL;

				if(i==0)
				{
					str_getvalu=strtok(sOutput.GetBuffer(strlen(sOutput)),",");
					sOutput.ReleaseBuffer();
					ValueList.Add(str_getvalu);
					continue;
				}
				str_getvalu=strtok(NULL,",");
				if (str_getvalu == NULL)
				{
					break;
				}
				ValueList.Add(str_getvalu);
			} 

			if (ValueList.GetSize()<5)//OK,蓝牙,wifi, imei1,IMEI2,MEID，PSN
			{
				//RunLOG("MES Get Number Part Loss");
				lpCPIN_TESTDlg->OutPutDebugLog("MES Get Number Fail");
				return false;
			}
			else if(ValueList.GetSize()==5)//OK,蓝牙,wifi, imei1,，PSN
			{
				BT_MAC=ValueList.GetAt(1);
				lpCPIN_TESTDlg->OutPutDebugLog("BT_MAC: "+BT_MAC);
				WIFI_MAC=ValueList.GetAt(2);
				lpCPIN_TESTDlg->OutPutDebugLog("WIFI_MAC: "+WIFI_MAC);
				IMEI1=ValueList.GetAt(3);
				lpCPIN_TESTDlg->OutPutDebugLog("IMEI1: "+IMEI1);
				PSN=ValueList.GetAt(4);
				lpCPIN_TESTDlg->OutPutDebugLog("ZJSN: "+PSN);
			}
			else if(ValueList.GetSize()==6)//OK,蓝牙,wifi, imei1,IMEI2，PSN
			{
				BT_MAC=ValueList.GetAt(1);
				lpCPIN_TESTDlg->OutPutDebugLog("BT_MAC: "+BT_MAC);
				WIFI_MAC=ValueList.GetAt(2);
				lpCPIN_TESTDlg->OutPutDebugLog("WIFI_MAC: "+WIFI_MAC);
				IMEI1=ValueList.GetAt(3);
				lpCPIN_TESTDlg->OutPutDebugLog("IMEI1: "+IMEI1);
				IMEI2=ValueList.GetAt(4);
				lpCPIN_TESTDlg->OutPutDebugLog("IMEI2: "+IMEI2);
				PSN=ValueList.GetAt(5);
				lpCPIN_TESTDlg->OutPutDebugLog("ZJSN: "+PSN);
			}
			else if(ValueList.GetSize()==7)//OK,蓝牙,wifi, imei1,IMEI2,MEID，PSN
			{
				BT_MAC=ValueList.GetAt(1);
				lpCPIN_TESTDlg->OutPutDebugLog("BT_MAC: "+BT_MAC);
				WIFI_MAC=ValueList.GetAt(2);
				lpCPIN_TESTDlg->OutPutDebugLog("WIFI_MAC: "+WIFI_MAC);
				IMEI1=ValueList.GetAt(3);
				lpCPIN_TESTDlg->OutPutDebugLog("IMEI1: "+IMEI1);
				IMEI2=ValueList.GetAt(4);
				lpCPIN_TESTDlg->OutPutDebugLog("IMEI2: "+IMEI2);
				MEID=ValueList.GetAt(5);
				lpCPIN_TESTDlg->OutPutDebugLog("MEID: "+MEID);
				PSN=ValueList.GetAt(6);
				lpCPIN_TESTDlg->OutPutDebugLog("ZJSN: "+PSN);
			}
			lpCPIN_TESTDlg->OutPutDebugLog("MES CheckStation and GetNumber Pass");
			return true;
		}
	}
	catch (...)
	{
		lpCPIN_TESTDlg->OutPutDebugLog("MES Crash");
		lpCPIN_TESTDlg->OutPutDebugLog("MES CheckStation and GetNumber Fail");
		return false;
	}
}

bool MYWORK::MES_PHONE_PAIRCHECK(CString sScaHMDSN, CString sLeft, CString sRight, CString &sOutPut)
{
	CString sTemp = "";
	CommonSrv ^WSObject = gcnew CommonSrv();
	System::String^ strHMDSNCharp = gcnew System::String(sScaHMDSN);
	System::String^ strHDLLeftCharp = gcnew System::String(sLeft);
	System::String^ strHDLRightCharp = gcnew System::String(sRight);
	System::String^ strResCharp = "";
	try {
		lpCPIN_TESTDlg->OutPutDebugLog("MES PAIR Check ...");
		sTemp.Format("MES Upload Check,%s,%s,%s", sScaHMDSN, sLeft, sRight);
		lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
		strResCharp = WSObject->getBindingResult(strHMDSNCharp, strHDLLeftCharp, strHDLRightCharp);
		sOutPut = CString(strResCharp);
		if (sOutPut == "OK") {
			lpCPIN_TESTDlg->OutPutDebugLog("MES Upload Check Pass");
			return true;
		}
		if (sOutPut == "NG") {
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			lpCPIN_TESTDlg->OutPutDebugLog("MES Upload Check Fail");
			return false;
		}
		lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
		return true;
	}
	catch (...) {
		lpCPIN_TESTDlg->OutPutDebugLog("MES Crash");
		lpCPIN_TESTDlg->OutPutDebugLog("MES Check Station Fail");
		return false;
	}
}

bool MYWORK::MES_PHONE_PAIRUPLOAD(CString sScaHMDSN, CString sLeft, CString sRight, CString &sOutPut)
{
	CString sTemp = "";
	CommonSrv ^WSObject = gcnew CommonSrv();
	System::String^ strHMDSNCharp = gcnew System::String(sScaHMDSN);
	System::String^ strHDLLeftCharp = gcnew System::String(sLeft);
	System::String^ strHDLRightCharp = gcnew System::String(sRight);
	System::String^ strResCharp = "";
	try {
		lpCPIN_TESTDlg->OutPutDebugLog("MES PAIR Upload ...");
		sTemp.Format("MES Upload,%s,%s,%s", sScaHMDSN, sLeft, sRight);
		lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
		strResCharp = WSObject->bindingController(strHMDSNCharp, strHDLLeftCharp, strHDLRightCharp);
		sOutPut = CString(strResCharp);
		if (sOutPut == "OK") {
			lpCPIN_TESTDlg->OutPutDebugLog("MES Upload Pass");
			return true;
		}
		if (sOutPut == "NG") {
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			lpCPIN_TESTDlg->OutPutDebugLog("MES Upload Fail");
			return false;
		}
		lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
		return true;
	}
	catch (...) {
		lpCPIN_TESTDlg->OutPutDebugLog("MES Crash");
		lpCPIN_TESTDlg->OutPutDebugLog("MES Check Station Fail");
		return false;
	}
}

bool MYWORK::MES_PHONE_MCcheck(CString PhoneSN,CString PcbaSN, CString SoftVer,CString &IMEI1,CString &MEID,CString &BT_MAC,CString &WIFI_MAC,CString &MesPSN,CString &MesICCID,CString &Errorlog)
{
	CString sOutput="";	
	char *str_getvalu=NULL;
	CArray<CString,CString>ValueList;
	CString IpAddress=m_CONFIG.sMESURL;
	CString StationName=m_CONFIG.sMESStationName;
	CString WorkOrder=m_CONFIG.sPO;  //工单
	CString Line=m_CONFIG.sMES_Line;
	CString MES_USER=m_CONFIG.sMESUser;
	CString FateName=m_CONFIG.sFateName;
	CString JIG_Num=m_CONFIG.sJigNum;//夹具编号
	CString sinfo="";
	CString NetSN="";
	CString InputImei1="";
	int index=0;
	Errorlog="";
	try
	{
		lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------MES CheckStation and GetNumber-----------------------------");
		lpCPIN_TESTDlg->OutPutDebugLog("PhoneSN="+PhoneSN+" NetSN+"+" PcbaSN="+PcbaSN+" Workorder="+WorkOrder);
		//if(!_MCcheck(IpAddress,PhoneSN,NetSN,PcbaSN,StationName,WorkOrder,SoftVer,Line,MES_USER,FateName,JIG_Num,sOutput))
		if(!_MCcheck2(IpAddress,PhoneSN,NetSN,PcbaSN,InputImei1,StationName,WorkOrder,SoftVer,Line,MES_USER,FateName,JIG_Num,sOutput))
		{
			lpCPIN_TESTDlg->OutPutDebugLog(sOutput);
			Errorlog=sOutput;
			return false;
		}
		else
		{	
			Errorlog=sOutput;
			lpCPIN_TESTDlg->OutPutDebugLog("sOutput: "+Errorlog);
			for (int i=0;i<50;i++)
			{
				str_getvalu=NULL;

				if(i==0)
				{
					str_getvalu=strtok(sOutput.GetBuffer(strlen(sOutput)),",");
					sOutput.ReleaseBuffer();
					ValueList.Add(str_getvalu);
					continue;
				}
				str_getvalu=strtok(NULL,",");
				if (str_getvalu == NULL)
				{
					break;
				}
				ValueList.Add(str_getvalu);
			} 
			//OK,蓝牙,wifi, imei1, ,MEID，PSN,FRP_KEY			
			BT_MAC=ValueList.GetAt(1);
			lpCPIN_TESTDlg->OutPutDebugLog("BT_MAC: "+BT_MAC);
			WIFI_MAC=ValueList.GetAt(2);
			lpCPIN_TESTDlg->OutPutDebugLog("WIFI_MAC: "+WIFI_MAC);
			IMEI1=ValueList.GetAt(3);
			lpCPIN_TESTDlg->OutPutDebugLog("IMEI1: "+IMEI1);
			MesPSN=ValueList.GetAt(4);
			lpCPIN_TESTDlg->OutPutDebugLog("ZJSN: "+MesPSN);
			MesICCID=ValueList.GetAt(6);
			lpCPIN_TESTDlg->OutPutDebugLog("ICCID: "+MesICCID);
			lpCPIN_TESTDlg->OutPutDebugLog("MES CheckStation and GetNumber Pass");
			return true;
		}
	}
	catch (...)
	{
		lpCPIN_TESTDlg->OutPutDebugLog("MES Crash");
		lpCPIN_TESTDlg->OutPutDebugLog("MES CheckStation and GetNumber Fail");
		return false;
	}
}
#pragma endregion

#pragma region WriteSNMethod

BOOL MYWORK::PHONE_Write_CUSTOMSN(CString CUSTOMSN, CString &sStrDebug)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------WRITE  CUSTOMER SN TEST---------------------------");
	CString Commond="",sReceiveData="",ERRINFO="",sSN="";
	int index1=0;
	CString keyValue="OK";
	Commond.Format("AT+CUSTOMSN=%s",CUSTOMSN);
	//if (!CheckNumberType(CUSTOMSN)) goto FAIL;	
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,Commond,sReceiveData,keyValue,3,ERRINFO)) goto FAIL;
	for (int i = 0;i<3;i++)
	{
		if (sReceiveData.Find("OK")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceiveData.Find("=")+1;
			sStrDebug=sReceiveData.Right(sReceiveData.GetLength()-index1).Trim();
			if (sStrDebug.Find("OK")==-1) goto FAIL;	
			else
			{
				index1=sStrDebug.Find("OK");
				sStrDebug=sStrDebug.Left(index1).Trim();
				if (!HDM_Read_PSN(sSN)) goto FAIL;
				if (sStrDebug.CompareNoCase(sSN)==0) return true;
				else goto FAIL;
			}
		}
		Sleep(500);
	}		
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Read_PSN(CString &sPSN)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM READ PSN TEST---------------------------");
	CString sResult = "",sStrDebug="";
	CString sPosFlag = "attached", sPosFlag2= "device";
	int index1 = 0,index2=0;
	if (m_CONFIG.iProject == D1_YVR) {
		if (!m_Qualcomm.m_cmd("adb devices", sResult)) {
			sStrDebug = sResult;
			goto FAIL;
		}
		else {
			index1 = sResult.Find(sPosFlag) + sPosFlag.GetLength();
			sStrDebug = sResult.Right(sResult.GetLength() - index1).Trim();
			if (sStrDebug.Find("device") == -1) goto FAIL;
			else
			{
				index2 = sStrDebug.Find(sPosFlag2);
				sPSN = sStrDebug.Left(index2).Trim();
				//if (!CheckNumberType(sStrDebug)) goto FAIL;	
				return true;
			}
		}
	}
	if (m_CONFIG.iProject == D2_YVR) {
		//scmd.Format("adb shell cat /backup/mlbsn.txt");
		if (!m_Qualcomm.m_cmd("adb shell cat /backup/mlbsn.txt", sResult)) {
			sStrDebug = sResult.Trim();
			goto FAIL;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog(sResult);
			sPSN = sResult.Trim();
			return TRUE;
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Install_BackupApp(CString &sStrDebug)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Install_BackupApp TEST---------------------------");
	CString sResult = "";
	int index1 = 0, index2 = 0;
	if (!m_Qualcomm.m_cmd("adb install -r readbackup.apk", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		lpCPIN_TESTDlg->OutPutDebugLog(sResult);
		if ((sResult.Find("Performing Streamed Install") != -1) && (sResult.Find("Success")!=-1)) {
			lpCPIN_TESTDlg->OutPutDebugLog("Install readbackup.apk success");
			Sleep(2000);
			if (!m_Qualcomm.m_cmd("adb shell am broadcast -a android.yvr.action.readbackup --include-stopped-packages", sResult)) {
				sStrDebug = sResult;
				goto FAIL;
			}
			else {
				lpCPIN_TESTDlg->OutPutDebugLog(sResult);
				if (sResult.Find("readbackup") != -1) lpCPIN_TESTDlg->OutPutDebugLog("Run readbackup.apk success");
				else lpCPIN_TESTDlg->OutPutDebugLog("Run readbackup.apk error");
			}
		}
		else {
			sStrDebug = sResult;
			goto FAIL;
		}
	}
	Sleep(10000);
	if (!m_Qualcomm.m_cmd("adb install -r factoryCmd.apk", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		lpCPIN_TESTDlg->OutPutDebugLog(sResult);
		if ((sResult.Find("Performing Streamed Install") != -1) && (sResult.Find("Success") != -1)) {
			lpCPIN_TESTDlg->OutPutDebugLog("Install factoryCmd.apk success");
			return TRUE;
		}
		else {
			sStrDebug = sResult;
			goto FAIL;
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::PHONE_Write_PSN(CString PSN, CString &sStrDebug)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------WRITE  PSN TEST---------------------------");
	CString Commond="",sReceiveData="",ERRINFO="",sSN="";
	int index1=0;
	CString keyValue="OK";
	Commond.Format("AT+PSN=%s",PSN);
	//if (!CheckNumberType(PSN)) goto FAIL;	
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,Commond,sReceiveData,keyValue,3,ERRINFO)) goto FAIL;
	for (int i = 0;i<3;i++)
	{
		if (sReceiveData.Find("OK")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceiveData.Find("=")+1;
			sStrDebug=sReceiveData.Right(sReceiveData.GetLength()-index1).Trim();
			if (sReceiveData.Find("OK")==-1) goto FAIL;	
			else
			{
				index1=sStrDebug.Find("OK");
				sStrDebug=sStrDebug.Left(index1).Trim();
				if (!PHONE_Read_PSN(sSN)) goto FAIL;
				if (sStrDebug.CompareNoCase(sSN)==0) return true;
				else goto FAIL;
			}
		}
		Sleep(500);
	}		
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::PHONE_Read_PSN(CString &sStrDebug)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------READ PSN TEST---------------------------");
	CString sATcmd="AT+PSN?",sKeyValue="PSN",sReceivedata="";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("PSN")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find("?")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();	
			if (sStrDebug.Find("OK")==-1) goto FAIL;
			else 
			{
				index1=sStrDebug.Find("OK");
				sStrDebug=sStrDebug.Left(index1).Trim();
				//if (!CheckNumberType(sStrDebug)) goto FAIL;	
				return true;
			}			
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::PHONE_Write_BSN(CString BSN, CString &sStrDebug)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------WRITE BSN TEST---------------------------");
	CString Commond="",sReceiveData="",ERRINFO="",ReadSNCommand="",sSN="";
	CString Flag="";
	char cFlag[128]={NULL};
	long data = 0;
	int index1=0,index2=0;
	CString keyValue="OK";
	lpCPIN_TESTDlg->OutPutDebugLog("WRITE BSN TEST...");
	if (BSN.GetLength()!=16) 
	{
		sStrDebug="SN长度不是16位！";
		goto FAIL;	
	}
	Commond.Format("AT+BSN=%s",BSN);
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,Commond,sReceiveData,keyValue,3,ERRINFO)) goto FAIL;
	for (int i = 0;i<3;i++)
	{
		if (sReceiveData.Find("BSN")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceiveData.Find("=")+1;
			sStrDebug=sReceiveData.Right(sReceiveData.GetLength()-index1).Trim();
			if (sReceiveData.Find("OK")==-1) goto FAIL;	
			else
			{
				index1=sStrDebug.Find("OK");
				sStrDebug=sStrDebug.Left(index1).Trim();
				//PHONE_Read_BSN(sSN);
				if (sStrDebug.CompareNoCase(sSN)==0) return true;
				else goto FAIL;
			}
		}
		Sleep(500);
	}		
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HANDLE_Read_BSN(CString &sReadBSN)//主板SN
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HANDLE READ BSN TEST---------------------------");
	CString sATcmd="parameters_print",sKeyValue="Stored parameters",sReceivedata="", sStrDebug="";
	CString sFlag = "serial number:";
	int index1=0,index2=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find(sKeyValue)==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else {
			index1=sReceivedata.Find(sFlag)+ sFlag.GetLength();
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			index2 = sStrDebug.Find(" ");
			sReadBSN=sStrDebug.Left(index2).Trim();
			return true;				
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sReceivedata);
	return FALSE;
}

bool MYWORK::TEST_WRITE_MCUIMEI(CString sIMEI,CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------WRITE MUCIMEI TEST---------------------------");
	CString Commond="",sReceiveData="",ERRINFO="",sSN="";
	int index1=0;
	CString keyValue="OK";
	Commond.Format("AT+MCUIMEI=%s",sIMEI);
	if (!CheckWordType(sIMEI,1)) goto FAIL;
	if(!AllisNum(sIMEI) && sIMEI != ""&& sIMEI.GetLength() == 15) {
		lpCPIN_TESTDlg->OutPutDebugLog("IMEI NUMBER FAIL");
		return FALSE;
	}
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,Commond,sReceiveData,keyValue,3,ERRINFO)) goto FAIL;
	for (int i = 0;i<3;i++) {
		if (sReceiveData.Find("OK")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceiveData.Find("=")+1;
			sStrDebug=sReceiveData.Right(sReceiveData.GetLength()-index1).Trim();
			if (sReceiveData.Find("OK")==-1) goto FAIL;	
			else
			{
				index1=sStrDebug.Find("OK");
				sStrDebug=sStrDebug.Left(index1).Trim();
				//if (!TEST_Read_COMFLAG(sSN)) goto FAIL;
				if (sStrDebug.CompareNoCase(sSN)==0) return true;
				else goto FAIL;
			}
		}
		Sleep(500);
	}		
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::MACTurn(CString sMAC,CString &sOutput)
{
	CString sTemp="";
	if (sMAC.GetLength()!=12) goto FAIL;	
	TCHAR NV_Data[15] = {0};
	memcpy(NV_Data, sMAC, sMAC.GetLength());	
	for(int i=0;i<9;i++)
	{
		sTemp=sTemp+NV_Data[i]+NV_Data[i+1]+":";
		i++;
	}
	sTemp=sTemp+NV_Data[10]+NV_Data[11];
	sOutput=sTemp;
	if( sOutput != ""&& sOutput.GetLength() != 17) goto FAIL;
	return true;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sOutput);
	return FALSE;
}

bool MYWORK::TEST_WRITE_MAC(CString sMAC,CString sMACType,CString &sStrDebug)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------WRITE MAC TEST---------------------------");
	CString sATcmd="",sReceiveData="",ERRINFO="",sSN="",sMACTurn="";
	int index1=0;
	CString sKeyValue="MAC";//WIFI必须是MAC否则抓不到值
	if (!MACTurn(sMAC,sMACTurn)) goto FAIL;
	sATcmd.Format("AT+%s=%s",sMACType,sMACTurn);	
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceiveData,sKeyValue,30,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceiveData.Find("MAC")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceiveData.Find("=")+1;
			sStrDebug=sReceiveData.Right(sReceiveData.GetLength()-index1).Trim();	
			if (sStrDebug.Find("OK")!=-1)
			{
				index1=sStrDebug.Find("OK")-1;
				sStrDebug=sStrDebug.Left(index1).Trim();
			}			
			if( sStrDebug != ""&& sStrDebug.GetLength() != 17) goto FAIL;
			//if (!TEST_READ_MAC(sMACType,sSN)) goto FAIL;
			if (sStrDebug.CompareNoCase(sSN)==0) return true;
			else goto FAIL;
			
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_READ_MAC(CString sMACType,CString &sStrDebug)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------READ MAC TEST---------------------------");
	CString scmd ="", sResult="";
	scmd.Format("adb shell cat /sdcard/%s.txt",sMACType);
	if (!m_Qualcomm.m_cmd((char*)(LPCTSTR)scmd, sResult)) {
		sStrDebug = sResult.Trim();
		goto FAIL;
	}
	else {
		lpCPIN_TESTDlg->OutPutDebugLog(sResult);
		sStrDebug = sResult.Trim();
		return TRUE;
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_READ_BATTCAP(CString &sStrDebug)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_READ_BATTCAP TEST---------------------------");
	CString scmd = "", sResult = "";
	if (!m_Qualcomm.m_cmd("adb shell am broadcast -a  android.yvr.action.battery.get --include-stopped-packages", sResult)) {
		sStrDebug = sResult.Trim();
		goto FAIL;
	}
	else {
		lpCPIN_TESTDlg->OutPutDebugLog(sResult);
		if (!m_Qualcomm.m_cmd("adb shell cat /sdcard/battery.txt", sResult)) {
			sStrDebug = sResult.Trim();
			goto FAIL;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog(sResult);
			sStrDebug = sResult.Trim();
			if (sResult=="") {
				lpCPIN_TESTDlg->OutPutDebugLog("Battery Cap is null");
				goto FAIL;
			}
			else {
				if (m_TestItem.STI_batteryCap.rangeMax < atoi(sStrDebug)) {
					lpCPIN_TESTDlg->OutPutDebugLog("Battery Cap is out of range");
					return false;
				}
				if (m_TestItem.STI_batteryCap.rangeMin > atoi(sStrDebug)) {
					lpCPIN_TESTDlg->OutPutDebugLog("Battery Cap is out of range");
					return false;
				}
				return TRUE;
			}
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

#pragma endregion

#pragma region JasonFile

bool ReadJsonFile(const char *jsonFileName, Json::Value *jsonInfo) {
	CString sTemp = "";
	ifstream jsonFile(jsonFileName, ios::binary);
	if (!jsonFile.is_open()) {
		sTemp.Format("OPEN %s ERROR\n, %s", jsonFileName);
		lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
		return false;
	}
	else {
		Json::Reader reader;
		if (reader.parse(jsonFile, *jsonInfo)) {
			jsonFile.close();
			return true;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog("parse json ERROR\n");
			jsonFile.close();
			return false;
		}
	}
	return true;
}

bool WriteJsonFile(const char *jsonFileName, const Json::Value writeJson) {
	CString sTemp = "";
	ofstream jsonFile(jsonFileName, ios::out);
	if (!jsonFile.is_open()) {
		sTemp.Format("OPEN %s ERROR\n, %s", jsonFileName);
		lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
		return false;
	}
	else {
		Json::StyledWriter sw;
		jsonFile << sw.write(writeJson);
		jsonFile.close();
		return true;
	}
	return true;
}

#pragma endregion

int MYWORK::HANDLE_PCBA_MMI_EXC() {
	CString sScanBSN ="", sReadBSN="",PHONESN="",sRes="",sStrDebug="",sFLAG="";
	CString IMEI="",DeviceID="",sOutput="",sPSN="",sReceiveData="";
	m_CONFIG.fItemNumber = 8;
	m_CONFIG.fPost = 0;
	int RES=SUCCESS;
	lpCPIN_TESTDlg->m_InputSn.GetWindowText(sScanBSN);
	lpCPIN_TESTDlg->m_InputPhoneSn.GetWindowText(sScanBSN);
	if (!CHECK_ATPORT_INSERT_WITH_ONTIME(30,3,m_CONFIG.iPhonePort)) {
		RES=CHECK_ATPORT_INSERT_FAIL;
		goto TESTEND;
	}
	if (!ENTER_FACTTEST(m_CONFIG.iPhonePort, sRes)) {
		RES=READ_PCBASN_FAIL;
		goto TESTEND;
	}
	lpCPIN_TESTDlg->OutPutTestLog(ENUM_PORT, sRes, true);
	if (!HANDLE_Read_BSN(sReadBSN)) {
		RES = READ_PCBASN_FAIL;
		goto TESTEND;
	}
	if (!CHECK_NUMBER(sScanBSN, sReadBSN, sReadBSN)) {
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READBSN, "BSN CHECK FAIL", FALSE);
		RES = READ_PCBASN_FAIL;
		goto TESTEND;
	}
	lpCPIN_TESTDlg->OutPutTestLog(ENUM_READBSN, sReadBSN, true);
	lpCPIN_TESTDlg->ShowSN(sReadBSN);
	if (!RES&&m_TestItem.bCheckLastStepFlag) {//检查标志位
		if (!TEST_CHECK_YVRFLAG(atoi(m_CONFIG.sMES_Line),HDL_YVR)) {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, FALSE);
			RES = READ_PCBASN_FAIL;
			goto TESTEND;
		}
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, true);
	}
	if(m_CONFIG.bMES) {
		if(!MES_CHECK_STATION(sReadBSN)) {//test
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK,"NG", FALSE);
			RES= MES_CheckStation_FAIL;
			goto TESTEND;
		}
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, "OK", true);
	}
	if (!PHONE_Function_Test())
	{
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
TESTEND: 
	if(!RES&&m_CONFIG.bMES) {
		if (!RES&&m_TestItem.bSetFlag) {//写标志位
			if (!HANDLE_Write_COMFLAG(atoi(m_CONFIG.sMES_Line))) {
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, FALSE);
				RES = READ_PCBASN_FAIL;
				goto TESTEND;
			}
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, true);
		}
		if (!MES_PASS_STATION(sReadBSN,true,RES)) {
			RES= MES_PassStation_FAIL;
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, "NG", FALSE);
		}	
		else {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, m_CONFIG.sMES_Line, true);
		}
	}
	if (!lpCPIN_TESTDlg->SaveTestLog(sReadBSN,RES)) {
		RES=Save_Log_FAIL;
	}
	THREAD_WORK=FALSE;
	return RES;
}

int MYWORK::HDM_COUPLING_EXC() {
	CString sScanBSN = "",sRes = "",sReadPSN = "",sFlag="", sTemp="",sStrDebug="", sHMDRFAddress="";
	CStringArray caAccBias, caAccMatrix, caGyroBias, caGyroMatrix;
	m_CONFIG.fItemNumber = 15;
	m_CONFIG.fPost = 0;
	int RES = SUCCESS;
	lpCPIN_TESTDlg->m_InputPhoneSn.GetWindowText(sScanBSN);
	if (!CHECK_ATPORT_INSERT_WITH_ONTIME(30, 3, m_CONFIG.iPhonePort)) {
		RES = CHECK_ATPORT_INSERT_FAIL;
		goto TESTEND;
	}
	else
    	lpCPIN_TESTDlg->OutPutTestLog(ENUM_PORT, Tools_InttoCstr(m_CONFIG.iPhonePort), true);
	if (!ENTER_FACTTEST(m_CONFIG.iPhonePort, sRes)) {
		RES = CHECK_ATPORT_INSERT_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_PORT, sRes, false);
		goto TESTEND;
	}
	lpCPIN_TESTDlg->OutPutTestLog(ENUM_PORT, sRes, true);
	if (m_CONFIG.Test_Selection ==HDM_COUPLING_TEST) {
		if (!HDM_Read_PSN(sReadPSN)) {//读SN
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_READPSN, "NG", FALSE);
			RES = PHONE_READ_SN_FAIL;
			goto TESTEND;
		}
	}
	if (m_CONFIG.Test_Selection == HANDLE_COUPLING_TEST) {
		if (!HANDLE_Read_BSN(sReadPSN)) {//读SN
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_READPSN, "NG", FALSE);
			RES = READ_PCBASN_FAIL;
			goto TESTEND;
		}
	}
	if (!CHECK_NUMBER(sScanBSN, sReadPSN, sReadPSN)) {//对比SN
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READPSN, "PSN error", FALSE);
		RES = READ_PCBASN_FAIL;
		goto TESTEND;
	}
	lpCPIN_TESTDlg->OutPutTestLog(ENUM_READPSN, sReadPSN, true);//显示SN
	lpCPIN_TESTDlg->ShowSN(sReadPSN);
	if (m_CONFIG.bMES) {//检查标志位
		//if (m_TestItem.bCheckLastStepFlag) {
		//	if (HDM_COUPLING_TEST == m_CONFIG.Test_Selection) {
		//		if (!TEST_CHECK_YVRFLAG(atoi(m_CONFIG.sMES_Line), HDM_YVR)) {
		//			RES = MES_CheckStation_FAIL;
		//			lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, FALSE);
		//			goto TESTEND;
		//		}
		//		else lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, true);
		//	}
		//	if (HANDLE_COUPLING_TEST == m_CONFIG.Test_Selection) {
		//		if (!TEST_CHECK_YVRFLAG(atoi(m_CONFIG.sMES_Line), HDL_YVR)) {
		//			RES = MES_CheckStation_FAIL;
		//			lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, FALSE);
		//			goto TESTEND;
		//		}
		//		else lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, true);
		//	}
		//}
		if (!MES_CHECK_STATION(sScanBSN)) {
			RES = MES_CheckStation_FAIL;
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, "NG", FALSE);
			goto TESTEND;
		}
		else {
			if (m_CONFIG.iProject == D1_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, m_CONFIG.sMES_Line, true);
			if (m_CONFIG.iProject == D2_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, m_CONFIG.stu_SFISConfig.sStationName, true);
		}
	}
	if (!HDM_Get_RootPermission(sStrDebug)) {
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Get_RootPermission fail", FALSE);
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Get_RootPermission success", TRUE);
	if (m_CONFIG.bMES) {//检查标志位
		if (m_TestItem.bCheckLastStepFlag) {
			if (HDM_COUPLING_TEST==m_CONFIG.Test_Selection) {
				if (!TEST_CHECK_YVRFLAG(atoi(m_CONFIG.sMES_Line), HDM_YVR)) {
					RES = MES_CheckStation_FAIL;
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, FALSE);
					goto TESTEND;
				}
				else lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, true);
			}
			if (HANDLE_COUPLING_TEST == m_CONFIG.Test_Selection) {
				if (!TEST_CHECK_YVRFLAG(atoi(m_CONFIG.sMES_Line), HDL_YVR)) {
					RES = MES_CheckStation_FAIL;
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, FALSE);
					goto TESTEND;
				}
				else lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, true);
			}
		}
		//if (!MES_CHECK_STATION(sScanBSN)) {
		//	RES = MES_CheckStation_FAIL;
		//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, "NG", FALSE);
		//	goto TESTEND;
		//}
		//else {
		//	if (m_CONFIG.iProject == D1_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, m_CONFIG.sMES_Line, true);
		//	if (m_CONFIG.iProject == D2_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, m_CONFIG.stu_SFISConfig.sStationName, true);
		//}
	}
	if (!HDM_Get_WAKEUP(sStrDebug)) {
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
	if (!HDM_Get_RFAddressInfo(sHMDRFAddress)) {
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
	m_CONFIG.stu_HANDLEPARAM1.setNull();//清空手柄参数
	if (!HANDLE_Get_Parameters(m_CONFIG.iPhonePort, m_CONFIG.stu_HANDLEPARAM1)) {//获取手柄参数
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
	if (!YVR_PAIR_TEST(sHMDRFAddress, m_CONFIG.iPhonePort, m_CONFIG.stu_HANDLEPARAM1)) {//手柄写入头盔RF地址
		RES = HMD_PAIR_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, "YVR_PAIR_TEST NG," + Tools_InttoCstr(m_CONFIG.iPhonePort), false);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "YVR_PAIR_TEST OK," + Tools_InttoCstr(m_CONFIG.iPhonePort), true);
	//////////////////////////////////////////
	//if (!HANDLE_Write_HostRFAddress(sHMDRFAddress, m_CONFIG.iPhonePort)) {//手柄写入头盔RF地址
	//	RES = CHECK_FUNCTION_FAIL;
	//	goto TESTEND;
	//}
	//m_CONFIG.stu_HANDLEPARAM1.setNull();//清空手柄参数
	//if (!HANDLE_Get_Parameters(m_CONFIG.iPhonePort,m_CONFIG.stu_HANDLEPARAM1)) {//获取手柄参数
	//	RES = CHECK_FUNCTION_FAIL;
	//	goto TESTEND;
	//}//头盔文件修改
	//if (!HDM_Write_HandleParamInfo(m_CONFIG.stu_HANDLEPARAM1,caAccBias, caAccMatrix, caGyroBias, caGyroMatrix,sStrDebug)) {
	//	RES = CHECK_FUNCTION_FAIL;
	//	goto TESTEND;
	//}
	//if (!HDM_Write_HostParamInfo(m_CONFIG.stu_HANDLEPARAM1,&caAccBias, &caAccMatrix, &caGyroBias, &caGyroMatrix, sStrDebug)) {
	//	RES = CHECK_FUNCTION_FAIL;
	//	goto TESTEND;
	//}
	//if (!HANDLE_Reboot(m_CONFIG.iPhonePort, sStrDebug)) {
	//	RES = CHECK_FUNCTION_FAIL;
	//	goto TESTEND;
	//}
	///////////////////////////////////////
	if (!HDM_Get_ROOT(sStrDebug)) {
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
	if (!HDM_Get_WAKEUP(sStrDebug)) {
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
	if (!HDM_Get_RFAddressInfo(sHMDRFAddress)) {
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
	if (!HANDLE_Write_HostRFAddress(sHMDRFAddress, m_CONFIG.iPhonePort)) {//手柄写入头盔RF地址
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
	if (!HANDLE_Reboot(m_CONFIG.iPhonePort, sStrDebug)) {
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
	if (!ENTER_FACTTEST(m_CONFIG.iPhonePort, sRes)) {
		RES = CHECK_FUNCTION_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_PORT, sRes, false);
		goto TESTEND;
	}
	lpCPIN_TESTDlg->OutPutTestLog(ENUM_PORT, sRes, true);
	if (!HANDLE_Test_RFCoupled(sStrDebug)) {
		RES = HMD_RFCoupled_FAIL;
		goto TESTEND;
	}
TESTEND:
	//if (!RES&&m_CONFIG.bMES) {//使用这一行FAIL不上传
	if (m_CONFIG.bMES) {//使用这一行FAIL上传
		if (!RES&&m_TestItem.bSetFlag) {//写标志位
			if (HDM_COUPLING_TEST == m_CONFIG.Test_Selection) {
				if (!HDM_Write_ADBFLAG(atoi(m_CONFIG.sMES_Line))) {
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, FALSE);
					RES = MES_PassStation_FAIL;
				}
				else lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, true);
			}
			if (HANDLE_COUPLING_TEST == m_CONFIG.Test_Selection) {
				if (!HANDLE_Write_COMFLAG(atoi(m_CONFIG.sMES_Line))) {
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, FALSE);
					RES = MES_PassStation_FAIL;
				}
				else lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, true);
			}
		}
		if (!MES_PASS_RFSTATION(sReadPSN,RES)) {
			RES = MES_PassStation_FAIL;
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, "NG", FALSE);
		}
		else
		{
			if (m_CONFIG.iProject == D1_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, m_CONFIG.sMES_Line, true);
			if (m_CONFIG.iProject == D2_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, m_CONFIG.stu_SFISConfig.sStationName, true);
		}
	}
	if (!lpCPIN_TESTDlg->SaveTestLog(sReadPSN, RES)) {
		RES = Save_Log_FAIL;
	}
	THREAD_WORK = FALSE;
	lpCPIN_TESTDlg->m_Progress.SetPos(100);
	return RES;
}

BOOL MYWORK::YVR_PAIR_TEST(CString sHMDRFAddress,int iPhonePort, HANDLEPARAM stu_HandleParam) {
	CString sStrDebug = "";
	CStringArray caAccBias, caAccMatrix, caGyroBias, caGyroMatrix;
	if (!HANDLE_Write_HostRFAddress(sHMDRFAddress, iPhonePort)) {//手柄写入头盔RF地址
		sStrDebug.Format("HANDLE_Write_HostRFAddress:%s,Port:%d ", sHMDRFAddress, iPhonePort);
		goto TESTFAIL;
	}
	//头盔文件修改
	if (!HDM_Write_HandleParamInfo(stu_HandleParam, caAccBias, caAccMatrix, caGyroBias, caGyroMatrix, sStrDebug)) {
		sStrDebug.Format("HDM_Write_HandleParamInfo");
		goto TESTFAIL;
	}
	if (!HDM_Write_HostParamInfo(stu_HandleParam, &caAccBias, &caAccMatrix, &caGyroBias, &caGyroMatrix, sStrDebug)) {
		sStrDebug.Format("HDM_Write_HostParamInfo");
		goto TESTFAIL;
	}
	if (!HANDLE_Reboot(iPhonePort, sStrDebug)) {
		sStrDebug.Format("HANDLE_Reboot");
		goto TESTFAIL;
	}
	return TRUE;
TESTFAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::YVR_PAIR_CHECK(CString sHMDRFAddress, int iPhonePort, HANDLEPARAM stu_HandleParam) {
	CString sStrDebug = "",sHDLRFAddress="";
	CStringArray caAccBias, caAccMatrix, caGyroBias, caGyroMatrix;
	//if (!HANDLE_Read_HostRFAddress(sHDLRFAddress, iPhonePort)) {//手柄写入头盔RF地址
	//	sStrDebug.Format("HANDLE_RFAddress:%s,Port:%d ", sHDLRFAddress, iPhonePort);
	//	goto TESTFAIL;
	//}
	//sStrDebug.Format("RFAddress Compare:%s,%s", sHDLRFAddress, sHMDRFAddress);
	//if (!CHECK_NUMBER(sHDLRFAddress, sHMDRFAddress, sHMDRFAddress)) goto TESTFAIL;
	ReadAndWriteJson(stu_HandleParam, caAccBias, caAccMatrix, caGyroBias, caGyroMatrix);
	if (!HDM_Read_HostParamInfo(stu_HandleParam, &caAccBias, &caAccMatrix, &caGyroBias, &caGyroMatrix, sStrDebug)) {
		sStrDebug.Format("HDM_Write_HostParamInfo");
		goto TESTFAIL;
	}
	return TRUE;
TESTFAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

int MYWORK::HDM_PAIR_EXC() {
	CString sScanBSN = "", sRes = "", sReadPSN = "", sFlag = "", sTemp = "", sStrDebug = "", sHMDRFAddress = "";
	int RES = SUCCESS;
	m_CONFIG.fItemNumber = 23;
	m_CONFIG.fPost = 0;
	lpCPIN_TESTDlg->m_InputPhoneSn.GetWindowText(sScanBSN);
	if (!CHECK_ATPORT_INSERT_WITH_ONTIME(30, 3, m_CONFIG.iPhonePort)) {
		RES = CHECK_ATPORT_INSERT_FAIL;
		goto TESTEND;
	}
	else 
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_PORT, Tools_InttoCstr(m_CONFIG.iPhonePort), true);
	if (!ENTER_FACTTEST(m_CONFIG.iPhonePort, sRes)) {
		RES = CHECK_ATPORT_INSERT_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, sRes, FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, sRes, TRUE);
	if (!CHECK_ATPORT_INSERT_WITH_ONTIME(30, 3,m_CONFIG.iPhonePort2)) {
		RES = CHECK_ATPORT_INSERT_FAIL;
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_PORT, Tools_InttoCstr(m_CONFIG.iPhonePort2), true);
	if (!ENTER_FACTTEST(m_CONFIG.iPhonePort2, sRes)) {
		RES = CHECK_ATPORT_INSERT_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, sRes, FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, sRes, TRUE);
	if (!HDM_Read_PSN(sReadPSN)) {//读SN
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READPSN, "NG", FALSE);
		RES = PHONE_READ_SN_FAIL;
		goto TESTEND;
	}
	if (!CHECK_NUMBER(sScanBSN, sReadPSN, sReadPSN)) {//对比SN
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READPSN, "PSN error", FALSE);
		RES = READ_PCBASN_FAIL;
		goto TESTEND;
	}
	lpCPIN_TESTDlg->OutPutTestLog(ENUM_READPSN, sReadPSN, true);//显示SN
	lpCPIN_TESTDlg->ShowSN(sReadPSN);
	if (!HDM_Get_RootPermission(sStrDebug)) {
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Get_RootPermission fail", FALSE);
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Get_RootPermission success", TRUE);
	if (m_TestItem.STI_RuninTime.needTest) {//检查RUNIN
		if (!HDM_Get_RUNINTIME(sStrDebug)) {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_ALARM, sStrDebug, FALSE);
			RES = CHECK_FUNCTION_FAIL;
			goto TESTEND;
		}
		else
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_ALARM, sStrDebug, TRUE);
	}
	if (m_CONFIG.bMES) {//检查标志位
		if (m_TestItem.bCheckLastStepFlag) {
			if (!TEST_CHECK_YVRFLAG(atoi(m_CONFIG.sMES_Line), HDM_YVR)) {
				RES = MES_CheckStation_FAIL;
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, false);
				goto TESTEND;
			}
			else lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, true);
		}
		if (!MES_CHECK_STATION(sScanBSN)) {
			RES = MES_CheckStation_FAIL;
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, "NG", FALSE);
			goto TESTEND;
		}
		else {
			if (m_CONFIG.iProject == D1_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, m_CONFIG.sMES_Line, true);
			if (m_CONFIG.iProject == D2_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, m_CONFIG.stu_SFISConfig.sStationName, true);
		}
	}
	m_CONFIG.stu_HANDLEPARAM1.setNull();
	if (!HANDLE_Get_Parameters(m_CONFIG.iPhonePort, m_CONFIG.stu_HANDLEPARAM1)) {//手柄写入头盔RF地址
		RES = CHECK_FUNCTION_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READ_FixtureID, Tools_InttoCstr(m_CONFIG.iPhonePort), FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READ_FixtureID, Tools_InttoCstr(m_CONFIG.iPhonePort), TRUE);
	m_CONFIG.stu_HANDLEPARAM2.setNull();
	if (!HANDLE_Get_Parameters(m_CONFIG.iPhonePort2, m_CONFIG.stu_HANDLEPARAM2)) {//手柄写入头盔RF地址
		RES = CHECK_FUNCTION_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READ_FixtureID, Tools_InttoCstr(m_CONFIG.iPhonePort2), FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READ_FixtureID, Tools_InttoCstr(m_CONFIG.iPhonePort2), TRUE);
	if (m_CONFIG.stu_HANDLEPARAM1.scontrollertype== m_CONFIG.stu_HANDLEPARAM2.scontrollertype ) {
		RES = CHECK_ATPORT_INSERT_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HANDELS ARE SAME", FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "Handle Check,OK", TRUE);
	if (!HDM_Get_WAKEUP(sStrDebug)) {
		RES = CHECK_FUNCTION_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Get_WAKEUP fail", FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Get_WAKEUP success", TRUE);
	if (!HDM_Get_RFAddressInfo(sHMDRFAddress)) {
		RES = CHECK_FUNCTION_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, "HDM_Get_RFAddressInfo fail", FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Get_RFAddressInfo success", TRUE);

	if (!YVR_PAIR_TEST(sHMDRFAddress, m_CONFIG.iPhonePort, m_CONFIG.stu_HANDLEPARAM1)) {//手柄写入头盔RF地址
		RES = HMD_PAIR_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, "YVR_PAIR_TEST NG,"+ Tools_InttoCstr(m_CONFIG.iPhonePort), FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "YVR_PAIR_TEST OK,"+Tools_InttoCstr(m_CONFIG.iPhonePort), TRUE);

	if (!YVR_PAIR_TEST(sHMDRFAddress, m_CONFIG.iPhonePort2, m_CONFIG.stu_HANDLEPARAM2)) {//手柄写入头盔RF地址
		RES = HMD_PAIR_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, "YVR_PAIR_TEST NG," + Tools_InttoCstr(m_CONFIG.iPhonePort2), FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "YVR_PAIR_TEST OK," + Tools_InttoCstr(m_CONFIG.iPhonePort2), TRUE);
	
TESTEND:
	if (!RES&&m_CONFIG.bMES) {
		CString sLeft = "", sRight = "";
		if (!RES&&m_TestItem.bSetFlag) {//写标志位
			if (!HDM_Write_ADBFLAG(atoi(m_CONFIG.sMES_Line))) {
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, FALSE);
				RES = MES_PassStation_FAIL;
			}
			else lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, true);
		}
		if (0 == atoi(m_CONFIG.stu_HANDLEPARAM1.scontrollertype)) {
			sLeft = m_CONFIG.stu_HANDLEPARAM1.sserialnumber;
			sRight = m_CONFIG.stu_HANDLEPARAM2.sserialnumber;
		}
		else {
			sLeft = m_CONFIG.stu_HANDLEPARAM2.sserialnumber;
			sRight = m_CONFIG.stu_HANDLEPARAM1.sserialnumber;
		}
		if (m_CONFIG.iProject == D1_YVR) {
			if (!MES_PHONE_PAIRUPLOAD(sReadPSN, sLeft, sRight, sStrDebug)) {
				RES = MES_PassStation_FAIL;
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_UPDATE_Result, "PAIRUPLOAD NG", FALSE);
			}
			else 
				 lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_UPDATE_Result, "PAIRUPLOAD OK", true);
		}
		if (!MES_PASS_PAIRSTATION(sReadPSN, sLeft, sRight, true, RES)) {
			RES = MES_PassStation_FAIL;
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, "PASS STATION NG", FALSE);
		}
		else
		{
			if (m_CONFIG.iProject == D1_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, m_CONFIG.sMES_Line, true);
			if (m_CONFIG.iProject == D2_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, m_CONFIG.stu_SFISConfig.sStationName, true);
		}
	}
	if (!lpCPIN_TESTDlg->SaveTestLog(sReadPSN, RES)) {
		RES = Save_Log_FAIL;
	}
	THREAD_WORK = FALSE;
	lpCPIN_TESTDlg->m_Progress.SetPos(100);
	return RES;
}

int MYWORK::HDM_CHECK_EXC()
{
	CString sScanBSN = "", sRes = "", sReadPSN = "", sFlag = "", sTemp = "", sStrDebug = "", sHMDRFAddress = "";
	CString sMESWIFIMac = "", sMESBTMac="", sReadBTMAC="", sReadWIFIMAC="",sLeft="",sRight="",sSWVer="",sLocked="";
	int RES = SUCCESS;
	m_CONFIG.fItemNumber = 30;
	m_CONFIG.fPost = 0;
	lpCPIN_TESTDlg->m_InputPhoneSn.GetWindowText(sScanBSN);
	if (!CHECK_ATPORT_INSERT_WITH_ONTIME(30, 3, m_CONFIG.iPhonePort)) {
		RES = CHECK_ATPORT_INSERT_FAIL;
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_PORT, Tools_InttoCstr(m_CONFIG.iPhonePort), true);
	if (!ENTER_FACTTEST(m_CONFIG.iPhonePort, sRes)) {
		RES = CHECK_ATPORT_INSERT_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, sRes, FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, sRes, TRUE);
	if (!CHECK_ATPORT_INSERT_WITH_ONTIME(30, 3, m_CONFIG.iPhonePort2)) {
		RES = CHECK_ATPORT_INSERT_FAIL;
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_PORT, Tools_InttoCstr(m_CONFIG.iPhonePort2), true);
	if (!ENTER_FACTTEST(m_CONFIG.iPhonePort2, sRes)) {
		RES = CHECK_ATPORT_INSERT_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, sRes, FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, sRes, TRUE);
	if (!HDM_Read_PSN(sReadPSN)) {//读SN
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READPSN, "NG", FALSE);
		RES = PHONE_READ_SN_FAIL;
		goto TESTEND;
	}
	if (!CHECK_NUMBER(sScanBSN, sReadPSN, sReadPSN)) {//对比SN
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READPSN, "PSN error", FALSE);
		RES = READ_PCBASN_FAIL;
		goto TESTEND;
	}
	lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, sReadPSN, true);//显示SN
	lpCPIN_TESTDlg->ShowSN(sReadPSN);
	if (!HDM_Install_BackupApp(sStrDebug)) {
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, "RUN APP,NG", false);
		RES = PHONE_READ_SN_FAIL;
		goto TESTEND;
	}
	else
	{
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ENTER_FACTTEST, "RUN APP,OK", true);
	}
	if (!HDM_Get_ReleasePermission(sStrDebug)) {
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Get_RleasePermission fail", FALSE);
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Get_RleasePermission success", TRUE);
	if (!HDM_Get_WAKEUP(sStrDebug)) {
		RES = CHECK_FUNCTION_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Get_WAKEUP fail", FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Get_WAKEUP success", TRUE);

	//if (!HDM_Read_CameraNum(sStrDebug)) {
	//	RES = CHECK_FUNCTION_FAIL;
	//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Read_CameraNum,NG," + sStrDebug, FALSE);
	//	goto TESTEND;
	//}
	//else
	//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDM_Read_CameraNum,OK,", TRUE);

	if(!RES && m_CONFIG.bMES) {//从PSN带出号段
		if (m_TestItem.bCheckLastStepFlag) {
			if (!TEST_CHECK_YVRFLAG(atoi(m_CONFIG.sMES_Line), HDM_YVR)) {
				RES = MES_CheckStation_FAIL;
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, false);
				goto TESTEND;
			}
			else lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, true);
		}
		if (!MES_CHECK_STATION(sScanBSN)) {
			RES = MES_CheckStation_FAIL;
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, "NG", FALSE);
			goto TESTEND;
		}
		else {
			if (m_CONFIG.iProject == D1_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, m_CONFIG.sMES_Line, true);
			if (m_CONFIG.iProject == D2_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, m_CONFIG.stu_SFISConfig.sStationName, true);
		}
		if (!MES_GETMAC_STATION(sReadPSN, sMESWIFIMac, sMESBTMac)) {
			RES = MES_CheckStation_FAIL;
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, "NG", FALSE);
			goto TESTEND;
		}
		else {
			if (m_CONFIG.iProject == D1_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, sMESWIFIMac+","+ sMESBTMac, true);
		}
	}
	if (!RES && m_TestItem.bCheckVersion) {//读MCU版本号
		if (!HDM_Read_SWVersion(sSWVer, sLocked)) {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MCU_VERSION, sSWVer+","+sLocked, false);
			RES = CHECK_FUNCTION_FAIL;
			goto TESTEND;
		}
		else {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MCU_VERSION, sSWVer + "," +sLocked, true);
		}
	}
	if (!RES && m_TestItem.bWIFIMAC) {
		if (!HDM_READ_MAC("wifi", sReadWIFIMAC)) {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_WIFI,"READNG",FALSE);
			RES = CHECK_FUNCTION_FAIL;
			goto TESTEND;
		}
		else{
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_WIFI, sReadWIFIMAC, true);
		}
	}
	if (!RES && m_TestItem.bBTMAC) {   
		if (!HDM_READ_MAC("bt", sReadBTMAC)) {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_BT, "READNG", FALSE);
			RES = CHECK_FUNCTION_FAIL;
			goto TESTEND;
		}
		else {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_BT, sReadBTMAC, true);
		}
	}
	if (!RES &&m_CONFIG.bMES) {
		sStrDebug.Format("WIFI Compare:%s,%s", sMESWIFIMac, sReadWIFIMAC);
		if (!CHECK_NUMBER(sMESWIFIMac, sReadWIFIMAC, sReadWIFIMAC)) {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_WIFI, sStrDebug, FALSE);
			RES = CHECK_FUNCTION_FAIL;
			goto TESTEND;
		}
		else {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_WIFI, sStrDebug, true);
		}
		sStrDebug.Format("BT Compare:%s,%s", sMESBTMac, sReadBTMAC);
		if (!CHECK_NUMBER(sMESBTMac, sReadBTMAC, sReadBTMAC)) {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_BT, sStrDebug, FALSE);
			RES = CHECK_FUNCTION_FAIL;
			goto TESTEND;
		}
		else {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_BT, sStrDebug, true);
		}
	}
	if (!RES && m_TestItem.STI_batteryCap.needTest) {
		if (!HDM_READ_BATTCAP(sStrDebug)) {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_Battery_Voltage, sStrDebug, false);
			RES = CHECK_FUNCTION_FAIL;
			goto TESTEND;
		}
		else {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_Battery_Voltage, sStrDebug, true);
		}
	}
	m_CONFIG.stu_HANDLEPARAM1.setNull();
	if (!HANDLE_Get_Parameters(m_CONFIG.iPhonePort, m_CONFIG.stu_HANDLEPARAM1)) {//手柄写入头盔RF地址
		RES = CHECK_FUNCTION_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READ_FixtureID, Tools_InttoCstr(m_CONFIG.iPhonePort), FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READ_FixtureID, Tools_InttoCstr(m_CONFIG.iPhonePort), TRUE);
	m_CONFIG.stu_HANDLEPARAM2.setNull();
	if (!HANDLE_Get_Parameters(m_CONFIG.iPhonePort2, m_CONFIG.stu_HANDLEPARAM2)) {//手柄写入头盔RF地址
		RES = CHECK_FUNCTION_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READ_FixtureID, Tools_InttoCstr(m_CONFIG.iPhonePort2), FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READ_FixtureID, Tools_InttoCstr(m_CONFIG.iPhonePort2), TRUE);
	if (m_CONFIG.stu_HANDLEPARAM1.scontrollertype == m_CONFIG.stu_HANDLEPARAM2.scontrollertype) {
		RES = CHECK_ATPORT_INSERT_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HANDELS ARE SAME", FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "Handle Check,OK", TRUE);
	if (!RES &&m_CONFIG.bMES) {
		if (0 == atoi(m_CONFIG.stu_HANDLEPARAM1.scontrollertype)) {
			sLeft = m_CONFIG.stu_HANDLEPARAM1.sserialnumber;
			sRight = m_CONFIG.stu_HANDLEPARAM2.sserialnumber;
		}
		else {
			sLeft = m_CONFIG.stu_HANDLEPARAM2.sserialnumber;
			sRight = m_CONFIG.stu_HANDLEPARAM1.sserialnumber;
		}
		if (m_CONFIG.iProject == D1_YVR) {
			if (!MES_PHONE_PAIRCHECK(sReadPSN, sLeft, sRight, sStrDebug)) {
				RES = MES_PassStation_FAIL;
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_UPDATE_Result, "PAIRCHECK NG", FALSE);
			}
			else
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_UPDATE_Result, "PAIRCHECK OK", true);
		}
	}

	//if (!HDM_Get_RFAddressInfo(sHMDRFAddress)) {//需要有Root权限先取消
	//	RES = CHECK_FUNCTION_FAIL;
	//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST,"HDMRFAddress:"+sHMDRFAddress, false);
	//	goto TESTEND;
	//}
	//else
	//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "HDMRFAddress:" +sHMDRFAddress, true);

	if (!YVR_PAIR_CHECK(sHMDRFAddress, m_CONFIG.iPhonePort, m_CONFIG.stu_HANDLEPARAM1)) {//手柄写入头盔RF地址
		RES = HMD_PAIR_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "YVR_PAIR_CHECK NG," + Tools_InttoCstr(m_CONFIG.iPhonePort), FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "YVR_PAIR_CHECK OK," + Tools_InttoCstr(m_CONFIG.iPhonePort), TRUE);

	if (!YVR_PAIR_CHECK(sHMDRFAddress, m_CONFIG.iPhonePort2, m_CONFIG.stu_HANDLEPARAM2)) {//手柄写入头盔RF地址
		RES = HMD_PAIR_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "YVR_PAIR_CHECK NG," + Tools_InttoCstr(m_CONFIG.iPhonePort2), FALSE);
		goto TESTEND;
	}
	else
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "YVR_PAIR_CHECK OK," + Tools_InttoCstr(m_CONFIG.iPhonePort2), TRUE);

	if (!RES&&m_CONFIG.bMES) {
		if (!RES&&m_TestItem.bSetFlag) {//写标志位
			if (!HDM_Write_ADBFLAG(atoi(m_CONFIG.sMES_Line))) {
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, FALSE);
				RES = MES_PassStation_FAIL;
			}
			else lpCPIN_TESTDlg->OutPutTestLog(ENUM_FLAGTEST, m_CONFIG.sMES_Line, true);
		}
	}

	if (!HDM_Read_SWVersion(sSWVer, sLocked)) {
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MCU_VERSION, "Lock:" + sLocked, false);
		RES = CHECK_FUNCTION_FAIL;
		goto TESTEND;
	}
	else {
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MCU_VERSION, "Lock:" + sLocked, true);
		if (sLocked=="1" || m_CONFIG.iProject == D2_YVR) {
			if (!HDM_Get_Factroyrese(sStrDebug)) {
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN, sStrDebug, false);
				RES = CHECK_FUNCTION_FAIL;
				goto TESTEND;
			}
			else {
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN, sStrDebug, true);
			}
		}
		else if (sLocked=="0" && m_CONFIG.iProject == D1_YVR) {
		//if (sLocked=="1") {
			if (!HDM_Write_Locked(sStrDebug)) {
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN, sStrDebug, false);
				RES = CHECK_FUNCTION_FAIL;
				goto TESTEND;
			}
			else {
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN, sStrDebug, true);
			}
		}
		else {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MCU_VERSION, "Lock:"+sLocked, false);
			RES = CHECK_FUNCTION_FAIL;
			goto TESTEND;
		}
	}
	//if (!HANDLE_Write_Mode(5, m_CONFIG.iPhonePort)) {
	//	RES = CHECK_FUNCTION_FAIL;
	//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECFG, Tools_InttoCstr(m_CONFIG.iPhonePort) + ",HANDLE_Write_Mode 5", FALSE);
	//	goto TESTEND;
	//}
	//else
	//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECFG, Tools_InttoCstr(m_CONFIG.iPhonePort) + ",HANDLE_Write_Mode 5", TRUE);
	//if (!HANDLE_Write_Mode(5, m_CONFIG.iPhonePort2)) {
	//	RES = CHECK_FUNCTION_FAIL;
	//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECFG, Tools_InttoCstr(m_CONFIG.iPhonePort2) + ",HANDLE_Write_Mode 5", FALSE);
	//	goto TESTEND;
	//}
	//else
	//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECFG, Tools_InttoCstr(m_CONFIG.iPhonePort2) + ",HANDLE_Write_Mode 5", TRUE);
	
TESTEND:
	if (!RES&&m_CONFIG.bMES) {
		if (!MES_PASS_STATION(sReadPSN,true, RES)) {
			RES = MES_PassStation_FAIL;
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, "PASS STATION NG", FALSE);
		}
		else
		{
			if (m_CONFIG.iProject == D1_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, m_CONFIG.sMES_Line, true);
			if (m_CONFIG.iProject == D2_YVR) lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result, m_CONFIG.stu_SFISConfig.sStationName, true);
		}
	}
	if (!lpCPIN_TESTDlg->SaveTestLog(sReadPSN, RES)) {
		RES = Save_Log_FAIL;
	}
	THREAD_WORK = FALSE;
	lpCPIN_TESTDlg->m_Progress.SetPos(100);
	return RES;
}

int MYWORK::WRITE_EXC()
{
	//time_t StarTtime,StopTime,StarTtime2,StopTime2;
	int OutTime = 250;
	CString sScanProcessSN="",sBSN="",sOutput="",sReadSoftVer="",sPSN="",sReadICCID="",sMACIMEICIP="";
	CString sMESProcessSN="",sMESIMEI1="",sMESMEID="",sMESBTMAC="",sMESWIFIMAC="",sStrDebug="",sReceiveData="";
	//获取SN
	lpCPIN_TESTDlg->m_InputPhoneSn.GetWindowText(sScanProcessSN);
	int RES=SUCCESS;
	BOOL CONNECT_PHONE=FALSE;
	if (!CHECK_ATPORT_INSERT_WITH_ONTIME(50,3, m_CONFIG.iPhonePort))//检查端口
	{
		RES=CHECK_ATPORT_INSERT_FAIL;
		goto TEST_END;
	}
	//if (!ENTER_FACTTEST(sStrDebug))//进入工程模式
	//{
	//	RES=READ_PCBASN_FAIL;
	//	goto TEST_END;
	//}
	//读BSN
	//if (!PHONE_Read_BSN(sBSN))
	//{
	//	RES = READ_PCBASN_FAIL;
	//	goto TEST_END;
	//}
	lpCPIN_TESTDlg->OutPutTestLog(ENUM_READBSN,sBSN,true);
	//读MCU版本号
	if (!RES && m_TestItem.bCheckVersion)
	{
		if (!TEST_MCU_Version(sReadSoftVer,sStrDebug))
		{
			RES = CHECK_FUNCTION_FAIL;
			goto TEST_END;
		}
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MCU_VERSION,sReadSoftVer,true);
	}
	if(!RES && m_CONFIG.bMES)//从PSN带出号段
	{
		if(!MES_PHONE_LPcheck(sScanProcessSN,sBSN,sReadSoftVer,sMESIMEI1,sMESMEID,sMESBTMAC,sMESWIFIMAC,sMESProcessSN,sOutput))
		{  
		  lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK,sOutput,FALSE);
		  RES= MES_CheckStation_FAIL;
		  goto TEST_END;
		}
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK,sOutput,TRUE);
	}
	//Def_IN
	//if (!RES && m_TestItem.bCFGIN)
	//{
	//	lpCPIN_TESTDlg->GetTestItem_DefaultEXCEL();
	//	if (!PHONE_Write_DefIN(sStrDebug,sReceiveData))
	//	{
	//		RES=Read_PSN_FAIL;
	//		goto TEST_END;
	//	}	
	//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECFG,sStrDebug,true);
	//}
	//CFG_IN
	if (!RES && m_TestItem.bCFGIN)
	{
		lpCPIN_TESTDlg->GetTestItem_EXCEL();
		if (m_EXCELTestItem.sCFGVersion=="")
		{
			RES= CHECK_FUNCTION_FAIL;
			goto TEST_END;
		}
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECFG,m_EXCELTestItem.sCFGVersion,true);
		if (!PHONE_Write_CFGIN(sStrDebug,sReceiveData))
		{
			RES= CHECK_FUNCTION_FAIL;
			goto TEST_END;
		}	
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECFG,sStrDebug,true);
	}
	//写PSN
	if (m_CONFIG.bMES)  sPSN=sMESProcessSN;
	else sPSN=sScanProcessSN.Trim();
	if (!PHONE_Write_CUSTOMSN(sPSN,sStrDebug))
	{
		RES= CHECK_FUNCTION_FAIL;
		goto TEST_END;
	}	
	lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,sStrDebug,true);
	//MCUIMEI
	if (!RES && m_TestItem.bFlag &&m_CONFIG.bMES)
	{
		if (!TEST_WRITE_MCUIMEI(sMESIMEI1,sStrDebug))
		{
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_IMEI,"NG",FALSE);
			RES = CHECK_FUNCTION_FAIL;
			goto TEST_END;
		}
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_IMEI,sStrDebug,true);
	}	
	//写BTMAC
	if (!RES && m_TestItem.bBT &&m_CONFIG.bMES)
	{   
		if (!TEST_WRITE_MAC(sMESBTMAC,"BTMAC",sStrDebug))
		{
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_BT,"NG",FALSE);
			RES = CHECK_FUNCTION_FAIL;
			goto TEST_END;
		}
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_BT,sStrDebug,true);
	}
	//写WIMAC
	if (!RES && m_TestItem.bwifi &&m_CONFIG.bMES)
	{   
		if (!TEST_WRITE_MAC(sMESWIFIMAC,"WIMAC",sStrDebug))
		{
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_WIFI,"NG",FALSE);
			RES = CHECK_FUNCTION_FAIL;
			goto TEST_END;
		}
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_WIFI,sStrDebug,true);
	}
	
	//加密BTMAC+IMEI
	if (!RES && m_TestItem.bBT &&m_CONFIG.bMES)
	{   
		if (!TEST_MACIMEI_CIP(sMESBTMAC,sMESIMEI1,sMACIMEICIP))
		{
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_IMSI,sMACIMEICIP,FALSE);
			RES = CHECK_FUNCTION_FAIL;
			goto TEST_END;
		}
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_IMSI,sMACIMEICIP,true);
	}
	////////////ICCID//////////////////////////////
	//if (!RES && m_TestItem.bICCID &&m_CONFIG.bMES)
	if (1)
	{
		if (!TEST_UC15Open(sStrDebug,sReceiveData))
		{
			//lpCPIN_TESTDlg->OutPutTestLog(ENUM_UC15OPEN,sStrDebug,false);
			RES = CHECK_FUNCTION_FAIL;
			goto TEST_END;
		}
		//lpCPIN_TESTDlg->OutPutTestLog(ENUM_UC15OPEN,sStrDebug,true);
		if (!TEST_CHECK_ICCID(sReadICCID,sReceiveData))
		{
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_ICCID,sReadICCID,false);
			RES = CHECK_FUNCTION_FAIL;
			goto TEST_END;
		}
		if (sReadICCID.Find("898604")!=-1)
		{
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_ICCID,sReadICCID,false);
			lpCPIN_TESTDlg->ShowInfor("发现非法SIM卡 898604 ！！");
			RES = CHECK_FUNCTION_FAIL;
			goto TEST_END;
		}
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_ICCID,sReadICCID,true);
	}
	/////////////////////////////////////END
TEST_END:
	if (!lpCPIN_TESTDlg->SaveTestLog(sBSN,RES))
	{
		RES=Save_Log_FAIL;
	}
	if(!RES&&m_CONFIG.bMES)
	{
		//if (!MES_PHONE_LPresult2(sPSN,"",sBSN,sReadSoftVer,sReadICCID,sMACIMEICIP,true,RES))
		//{
		//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result,"NG",FALSE);
		//	RES= MES_PassStation_FAIL;
		//}	
		//else
		//	lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_Result,"OK",TRUE);
	}
	THREAD_WORK=FALSE;
	return RES;
}

int MYWORK::MES_PAIR_INFORMATION_EXC()
{
	//time_t StarTtime,StopTime,StarTtime2,StopTime2;
	int OutTime = 250;
	CString sScaHMDSN = "", sScanHDLSN = "", sRight="",sLeft="",sOutput = "";
	CString sStrDebug = "", sReceiveData = "";

	//获取SN
	lpCPIN_TESTDlg->m_InputPhoneSn.GetWindowText(sScaHMDSN);
	lpCPIN_TESTDlg->m_InputSn.GetWindowText(sRight);
	lpCPIN_TESTDlg->GetDlgItem(IDC_LEFTSN)->GetWindowText(sLeft);
	int RES = SUCCESS;
	m_CONFIG.fItemNumber = 30;
	m_CONFIG.fPost = 0;
	//if ("" == sScaHMDSN) sScaHMDSN = "111111";
	//else {
	//	sRight = "1111111";
	//	sLeft = "2222222";
	//}
	//if ("" == sScanHDLSN) sScanHDLSN = "111111";
	//else {
	//	char cHDLType= sScanHDLSN.GetAt(5);
	//	if (cHDLType=='L') {
	//		sStrDebug.Format("输入左手柄:%s", sScanHDLSN);
	//		sLeft = sScanHDLSN;
	//		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READBSN, sStrDebug, true);
	//	}
	//	else if (cHDLType == 'R') {
	//		sStrDebug.Format("输入右手柄:%s", sScanHDLSN);
	//		sRight = sScanHDLSN;
	//		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READBSN, sStrDebug, true);
	//	}
	//	else {
	//		sStrDebug.Format("输入手柄无法识别:%s", sScanHDLSN);
	//		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READBSN, sStrDebug, true);
	//		sRight = "1111111";
	//		sLeft = "2222222";
	//	}
	//}
	if (m_CONFIG.bMES) {
		if (!MES_PHONE_PAIRCHECK(sScaHMDSN, sLeft, sRight, sOutput))
		{
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, sOutput, FALSE);
			RES = MES_CheckStation_FAIL;
			goto TEST_END;
		}
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, sOutput, TRUE);
	}
	else {
		RES = MES_CheckStation_FAIL;
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_MES_CHECK, "请链接MES", FALSE);
	}

TEST_END:
	THREAD_WORK = FALSE;
	lpCPIN_TESTDlg->m_Progress.SetPos(100);
	return RES;
}

BOOL MYWORK::PHONE_CHECK_Current()
{
	CString ERRINFO="";
	clock_t start,stop;
	float fScanTime;
	float Outtime = 250;
	CString IDLE_Current="",ERRCODE="",sOutput="",str="";
	float fAvgCurrent=0,fCurrent=0,fMaxCurrent=0;
	int index=0;
	start=clock();
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------CHECK Current TEST-----------------------------");
	if (!m_PowerControl.OpenEqupment(ERRINFO))
	{
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_SETPOWERTEST,"-",false);
		return FALSE;
	}
	for (int i=0;i<9;i++)
	{
		CArray<float,float> GetCurrent;
		GetCurrent.SetSize(0);
		start=clock();
		for (int j=0;j<30;j++)
		{	
			/*stop=clock();
			fScanTime=(float)(stop-start)/CLOCKS_PER_SEC;
			if(fScanTime>30)
				break;*/
			fCurrent=0;
			if(!m_PowerControl.PowerGetCurrent(fCurrent))
			{
				str.Format("%.3fmA",fCurrent);
				sOutput.Format("Current: %.3f mA",fCurrent);
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_Current,str,false);
				lpCPIN_TESTDlg->OutPutDebugLog("CHECK Current TEST FAIL");
				return false;
			}
			str.Format("%.3fmA",fCurrent);
			lpCPIN_TESTDlg->OutPutDebugLog(str);
			GetCurrent.Add(fCurrent);
			Sleep(100);
		}
		index=GetCurrent.GetCount();
		fMaxCurrent=0;
		for (int k=0;k<index;k++)//选最大的
		{
			if(fMaxCurrent<GetCurrent.GetAt(k))
			{
				fMaxCurrent=GetCurrent.GetAt(k);
			}
		}	
		if (fMaxCurrent <=  m_TestItem.Current_Max && fMaxCurrent >= m_TestItem.Current_Min)
		{
			str.Format("%.3f",fMaxCurrent);
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_Current,str,true);
			lpCPIN_TESTDlg->OutPutDebugLog("CHECK Current TEST PASS");
			m_PowerControl.CloseEqupment();
			return true;
		}		
		stop=clock();
		fScanTime=(float)(stop-start)/CLOCKS_PER_SEC;
		if(i==8)
		{
			str.Format("%.3f",fMaxCurrent);
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_Current,str,false);
			lpCPIN_TESTDlg->OutPutDebugLog("CHECK Current TEST false");
			m_PowerControl.CloseEqupment();
			return false;
		}
		Sleep(100);
	}

}

BOOL MYWORK::PHONE_CHECK_CurrentmA()
{
	CString ERRINFO="";
	clock_t start;
	float Outtime = 250;
	CString IDLE_Current="",ERRCODE="",sOutput="",str="";
	float fAvgCurrent=0,fCurrent=0,fMinCurrent=0;
	int index=0;
	start=clock();
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------CHECK Current TEST-----------------------------");
	if (!m_PowerControl.OpenEqupment(ERRINFO))
	{
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_SETPOWERTEST,"-",false);
		return FALSE;
	}
	if (!m_PowerControl.CurrRangmA())
	{
		lpCPIN_TESTDlg->OutPutDebugLog("Set CurrRangemA FAIL");
		return FALSE;
	}
	for (int i=0;i<3;i++)
	{
		CArray<float,float> GetCurrent;
		GetCurrent.SetSize(0);
		start=clock();
		for (int j=0;j<30;j++)
		{	
			fCurrent=0;
			if(!m_PowerControl.PowerGetCurrent(fCurrent))
			{
				str.Format("%.3fmA",fCurrent);
				sOutput.Format("Current: %.3f mA",fCurrent);
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_IDLE,str,false);
				lpCPIN_TESTDlg->OutPutDebugLog("CHECK Current TEST FAIL");
				return false;
			}
			str.Format("%.3fmA",fCurrent);
			lpCPIN_TESTDlg->OutPutDebugLog(str);
			GetCurrent.Add(fCurrent);
			Sleep(100);
		}
		index=GetCurrent.GetCount();
		fMinCurrent=100;
		for (int k=0;k<index;k++)//选最小的
		{
			if(fMinCurrent>GetCurrent.GetAt(k))
			{
				fMinCurrent=GetCurrent.GetAt(k);
			}
		}		
		if (fMinCurrent <=  m_TestItem.IDLE_Max )
		{
			str.Format("%.3f",fMinCurrent);
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_IDLE,str,true);
			lpCPIN_TESTDlg->OutPutDebugLog("CHECK Current TEST PASS");
			m_PowerControl.CloseEqupment();
			return true;
		}	
		if(i==2)
		{
			str.Format("%.3f",fMinCurrent);
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_IDLE,str,false);
			lpCPIN_TESTDlg->OutPutDebugLog("CHECK Current TEST false");
			m_PowerControl.CloseEqupment();
			return false;
		}
		OUT_FACTTEST(ERRINFO);//再进一次低功耗试试
	}
}

BOOL MYWORK::OPEN_POWER(CString Volt) 
{
	CString ERRINFO="";

	lpCPIN_TESTDlg->OutPutDebugLog("OPEN POWER");

	if (!m_PowerControl.OpenEqupment(ERRINFO))
	{
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		return FALSE;
	}

	if (!m_PowerControl.OpenBattery(Volt))
	{
		lpCPIN_TESTDlg->OutPutDebugLog("Set Volt Fail");
		return FALSE;
	}
	m_PowerControl.CloseEqupment();
	return TRUE;
}

BOOL MYWORK::CLOSE_POWER() 
{
	CString ERRINFO="";


	lpCPIN_TESTDlg->OutPutDebugLog("CLOSE POWER");

	if (!m_PowerControl.OpenEqupment(ERRINFO))
	{
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		return FALSE;
	}

	if (!m_PowerControl.CloseBattery())
	{
		lpCPIN_TESTDlg->OutPutDebugLog("Close Out Put Fail");
		return FALSE;
	}

	m_PowerControl.CloseEqupment();

	return TRUE;
}

BOOL MYWORK::INIT_POWER() 
{
	CString ERRINFO="";
	lpCPIN_TESTDlg->OutPutDebugLog("INIT POWER");
	if (!m_PowerControl.OpenEqupment(ERRINFO))
	{
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_SETPOWERINIT,"-",false);
		return FALSE;
	}
	if (!m_PowerControl.RST())
	{
		lpCPIN_TESTDlg->OutPutDebugLog("INIT POWER FAIL");
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_SETPOWERINIT,"-",false);
		return FALSE;
	}
	m_PowerControl.CloseEqupment();
	return TRUE;
}

BOOL MYWORK::POWER_GETCURRENT(float& fCurrent) 
{
	CString ERRINFO="";


	if (!m_PowerControl.OpenEqupment(ERRINFO))
	{
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		return FALSE;
	}

	if (!m_PowerControl.PowerGetCurrent(fCurrent))
	{
		m_PowerControl.CloseEqupment();
		return FALSE;
	}

	m_PowerControl.CloseEqupment();

	return TRUE;
}

BOOL MYWORK::PHONE_POWER_ON(float Timeout) 
{
	float	fCurrent=0;
	CString ERRCODE="",sOutput="";
	clock_t start,stop;
	float fScanTime;
	BOOL RES=TRUE;
	lpCPIN_TESTDlg->OutPutDebugLog("PHONE POWER ON...");
	RES=CLOSE_POWER(); 
	Sleep(500);
	if (RES)
	{
		RES=OPEN_POWER(m_TestItem.sTESTVOLT);
	}
	Sleep(1000);
	start=clock();
	while(1)
	{
		stop=clock();
		fScanTime=(float)(stop-start)/CLOCKS_PER_SEC;
		//超时
		if(fScanTime>Timeout)
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Check PHONE Power ON Time Out");
			return FALSE;
		}
		//获取程控电源电流
		if(!POWER_GETCURRENT(fCurrent))
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Get Current Fail");
			return FALSE;
		}
		else
		{
			sOutput.Format("Current: %.2f mA",fCurrent);
			lpCPIN_TESTDlg->OutPutDebugLog(sOutput);
			if (fCurrent>20)
			{
				break;
			}
		}
	}
	return TRUE;
}

BOOL MYWORK::PHONE_WRITE_FLAG(int RES)
{
	CString Commond="",ReceiveData="",ERRINFO="";
	int index1=0,index2=0;
	CString Flag="";
	char cFlag[129]={NULL};


	lpCPIN_TESTDlg->OutPutDebugLog("WRITEK TEST FLAG...");

	Commond="AT+SFLAGNV?";

	for (int i=0;i<3;i++)
	{
		if (!m_PHONE.SendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,Commond,ReceiveData,3,ERRINFO))
		{
			lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
			if (i==2)
			{
				lpCPIN_TESTDlg->OutPutDebugLog("READ TEST FLAG FAIL");
				return false;
			}
		}
		else
		{
			if (ReceiveData.Find("OK")!=-1  && ReceiveData.Find("ERROR")==-1)
			{
				ReceiveData.Remove('\n');
				ReceiveData.Remove('\r');
				index1=ReceiveData.Find(":");
				index2=ReceiveData.Find("OK");
				Flag=ReceiveData.Mid(index1+2,index2-index1-2);

				if (Flag.GetLength()!=128)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("Flag Length Not 128");
					return false;
				}

				memcpy(cFlag,Flag,128);


				lpCPIN_TESTDlg->OutPutDebugLog("READ TEST FLAG SUCCESS");
				break;
			}
			else
			{
				if(i==2)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("READ TEST FLAG FAIL");
					return false;
				}
			}
		}
		Sleep(500);
	}

	if (RES==SUCCESS)
	{
		cFlag[2]='P';
	}
	else
	{
		cFlag[2]='F';
	}

	Commond.Format("AT+SFLAGNV=%s",cFlag);

	for (int i=0;i<3;i++)
	{
		if (!m_PHONE.SendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,Commond,ReceiveData,3,ERRINFO))
		{
			lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
			if (i==2)
			{
				lpCPIN_TESTDlg->OutPutDebugLog("WRITE TEST FLAG FAIL");
				return false;
			}
		}
		else
		{
			if (ReceiveData.Find("OK")!=-1  && ReceiveData.Find("ERROR")==-1)
			{
				lpCPIN_TESTDlg->OutPutDebugLog("WRITE TEST FLAG SUCCESS");
				return true;
			}
			else
			{
				if(i==2)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("WRITE TEST FLAG FAIL");
					return false;
				}
			}
		}
		Sleep(500);
	}

	return true;
}

BOOL MYWORK::CONTROL_BOX_CLOSE_VBAT() 
{
	CString ERRINFO="";


	lpCPIN_TESTDlg->OutPutDebugLog("CONTROL BOX CLOSE BATTERY");

	if (!ControlBox_CloseBattery(m_CONFIG.iCtrBOXComPort,ERRINFO.GetBuffer(MAX_PATH)))
	{
		ERRINFO.ReleaseBuffer();
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		return FALSE;
	}

	ERRINFO.ReleaseBuffer();



	return TRUE;
}

BOOL MYWORK::CONTROL_BOX_GETJIGSTATE(int &state) 
{
	CString ERRINFO="",LOG="";


	lpCPIN_TESTDlg->OutPutDebugLog("CONTROL BOX GET JIG STATE");

	if (!ControlBox_GetJigState(m_CONFIG.iCtrBOXComPort,state,ERRINFO.GetBuffer(MAX_PATH)))
	{
		ERRINFO.ReleaseBuffer();
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		return FALSE;
	}
	else
	{
		LOG.Format("STATE: %d",state);
	}

	ERRINFO.ReleaseBuffer();



	return TRUE;
}

BOOL MYWORK::CONTROL_BOX_OPEN_LOCK()
{
	CString ERRINFO="";


	lpCPIN_TESTDlg->OutPutDebugLog("CONTROL BOX OPEN LOCK");

	if (!ControlBox_OpenLock(m_CONFIG.iCtrBOXComPort,1,ERRINFO.GetBuffer(MAX_PATH)))
	{
		ERRINFO.ReleaseBuffer();
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		return FALSE;
	}

	ERRINFO.ReleaseBuffer();



	return TRUE;
}

BOOL MYWORK::CONTROL_BOX_CLOSE_LOCK()
{
	CString ERRINFO="";


	lpCPIN_TESTDlg->OutPutDebugLog("CONTROL BOX CLOSE LOCK");

	if (!ControlBox_CloseLock(m_CONFIG.iCtrBOXComPort,1,ERRINFO.GetBuffer(MAX_PATH)))
	{
		ERRINFO.ReleaseBuffer();
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		return FALSE;
	}

	ERRINFO.ReleaseBuffer();



	return TRUE;
}

BOOL MYWORK::CONTROL_BOX_OPEN_LOCK2()
{
	CString ERRINFO="";


	lpCPIN_TESTDlg->OutPutDebugLog("CONTROL BOX OPEN LOCK 2");

	if (!ControlBox_OpenLock(m_CONFIG.iCtrBOXComPort,2,ERRINFO.GetBuffer(MAX_PATH)))
	{
		ERRINFO.ReleaseBuffer();
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		return FALSE;
	}

	ERRINFO.ReleaseBuffer();



	return TRUE;
}

BOOL MYWORK::PHONE_READ_SN(CString &IMEI)
{
	CString ATCOMMOND="",RETURNVALUE="",ERRCODE="";
	int index1=0,index2=0;


	lpCPIN_TESTDlg->OutPutDebugLog("Read IMEI...");


	ATCOMMOND="AT+XFDR";

	for (int i=0;i<10;i++)
	{
		if (!m_PHONE.SendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,ATCOMMOND,RETURNVALUE,3,ERRCODE))
		{
			lpCPIN_TESTDlg->OutPutDebugLog(ERRCODE);
			if (i==4)
			{
				lpCPIN_TESTDlg->OutPutDebugLog("READ IMEI FAIL");
				return FALSE;
			}
		}
		else
		{
			if (RETURNVALUE.Find("OK")!=-1  && RETURNVALUE.Find("ERROR")==-1 && RETURNVALUE.GetLength() >15)
			{
				RETURNVALUE.Remove('\n');
				RETURNVALUE.Remove('\r');
				index1=RETURNVALUE.Find(",");
				index2=RETURNVALUE.Find("+XINVER");

				IMEI = RETURNVALUE.Mid(index1+1,15);

				if(!AllisNum(IMEI) && IMEI != ""&& IMEI.GetLength() == 15)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("READ IMEI FAIL");
					return FALSE;
				}

				lpCPIN_TESTDlg->OutPutDebugLog("READ IMEI SUCCESS");
				return TRUE;
			}
			else
			{
				if(i==19)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("READ IMEI FAIL");
					return FALSE;
				}
			}
		}
		Sleep(800);
	}

	return TRUE;
}

BOOL MYWORK::CHECK_ATPORT_INSERT_WITH_ONTIME(int TimeOut_s, int iScanTimes, int iPhonePort) {
	clock_t start, stop;
	float fScanTime;
	int index=0;
	lpCPIN_TESTDlg->OutPutDebugLog("---------------------------CHECK AT PORT INSERT----------------------");
	for (int i=0;i<20;i++) {//置空
		m_struct_ScanPort.portinfo_scan[i].iPortNumber=0;
		m_struct_ScanPort.portinfo_scan[i].sPortName="";
	}
	start=clock();
	while(1) {
		stop=clock();
		fScanTime=(float)(stop-start)/CLOCKS_PER_SEC;
		if(fScanTime>TimeOut_s) {
			lpCPIN_TESTDlg->OutPutDebugLog("CHECK AT PORT INSERT TIME OUT,"+ Tools_InttoCstr(iPhonePort));
			return FALSE;
		}
		lpCPIN_TESTDlg->enumUsbPort(&m_struct_ScanPort);//枚举Scan系统USB口
		for (int k=0;k<20;k++) {
			if (m_struct_ScanPort.portinfo_scan[k].iPortNumber==iPhonePort) {
				index++;//找到了
				break;
			}
			else //前19个都没有就是真没有了，等待超时吧
				if (k==19) index = 0;
		}
		if (index== iScanTimes) {//iScanTimes循环找到的的次数
			lpCPIN_TESTDlg->OutPutDebugLog("CHECK AT PORT INSERT SUCCESS," + Tools_InttoCstr(iPhonePort));
			return TRUE;
		}
		Sleep(1000);
	}
	return TRUE;
}

BOOL MYWORK::CHECK_ATPORT_DISCONNECT(int TimeOut_s)
{
	clock_t start,stop;
	float fScanTime;
	int index=0;


	lpCPIN_TESTDlg->OutPutDebugLog("---------------------------CHECK AT PORT DISCONNECT----------------------");


	for (int i=0;i<20;i++)
	{
		m_struct_ScanPort.portinfo_scan[i].iPortNumber=0;
		m_struct_ScanPort.portinfo_scan[i].sPortName="";
	}


	start=clock();

	while(1)
	{
		stop=clock();
		fScanTime=(float)(stop-start)/CLOCKS_PER_SEC;
		if(fScanTime>TimeOut_s)
		{
			lpCPIN_TESTDlg->OutPutDebugLog("CHECK AT PORT DISCONNECT TIME OUT");
			return FALSE;
		}

		lpCPIN_TESTDlg->enumUsbPort(&m_struct_ScanPort);

		for (int k=0;k<20;k++)
		{
			if (m_struct_ScanPort.portinfo_scan[k].iPortNumber==m_CONFIG.iPhonePort)
			{
				break;
			}
			else
			{
				if (k==19)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("CHECK AT PORT DISCONNECT SUCCESS");
					return TRUE;
				}
			}
		}
		Sleep(1000);
	}

	return TRUE;
}

VOID MYWORK::GET_STRINT(CString BaseData,CString SearchStart,CString SearchEnd,CString &ReturnData)
{
	CString DATA="";

	int index1=0,index2=0,index3=0;

	index1=BaseData.GetLength();
	index2=BaseData.Find(SearchStart);
	index3=BaseData.Find(SearchEnd);

	if (!SearchStart.IsEmpty() &&!SearchEnd.IsEmpty())
	{
		ReturnData=BaseData.Mid(index2+SearchStart.GetLength(),index3-index2-SearchStart.GetLength());
	}
	else if(SearchEnd.IsEmpty())
	{
		ReturnData=BaseData.Right(index1-index2-SearchStart.GetLength());
	}
	else if(SearchStart.IsEmpty())
	{
		ReturnData=BaseData.Left(index3);
	}
}

BOOL MYWORK::AllisNum(CString str)  
{ 

	for (int i = 0; i < str.GetLength(); i++)
	{
		int tmp = (int)str[i];
		if (tmp >= 48 && tmp <= 57)
		{
			continue;
		}
		else
		{
			return false;
		}
	} 
	return true;
}

BOOL MYWORK::PHONE_Write_FixtureID(CString &FixtureID)
{
	//卡前三位（设备ID)
	//根据之前定义的设备ID的编码原则：HMB001 为900214090600001
	//公司代码：9      机型002     设备ID：139         年月：906     生产流水号：00001
	CString ID=m_CONFIG.sPREFIX;
	int ilength=FixtureID.GetLength();
	CString keyValue= "OK";
	//判断号段位数15位
	if (ilength!=15)
	{
		lpCPIN_TESTDlg->OutPutDebugLog("Create PSN is not 15bits");
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,"ERROR",false);
		return false;
	}
	CString modefiyDeviceID=FixtureID.Mid(4,11);
	CString sInput_3bitID=modefiyDeviceID.Mid(0,3);
	CString Slog="";
	if (ID.CompareNoCase(sInput_3bitID)!=0)//判断设备号（前3位）
	{
		Slog.Format("input psn 3 bit(deviceID) is not %s",ID);
		lpCPIN_TESTDlg->OutPutDebugLog("WRITE DeviceID(PSN) TEST FAIL,"+Slog);
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,"ERROR",false);
		return false;
	}
	CString Commond="",ReceiveData="",ERRINFO="",ReadSNCommand="",s_getDeviceID="";
	CString Flag="";
	char cFlag[128]={NULL};
	long data = 0;
	int index1=0,index2=0;
	lpCPIN_TESTDlg->OutPutDebugLog("WRITE DeviceID TEST...");
	Commond.Format("<SPHYMOST*N:%s>",modefiyDeviceID);

	bool bRet=false;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,Commond,ReceiveData,keyValue,3,ERRINFO))
	{
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		lpCPIN_TESTDlg->OutPutDebugLog("WRITE DeviceID TEST FAIL");
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,"ERROR",false);
		return false;
	}
	else
	{
		for (int i = 0;i<4;i++)
		{
			if (ReceiveData.Find("OK")!=-1)
			{
				lpCPIN_TESTDlg->OutPutDebugLog("WRITE DeviceID TEST PASS");
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,modefiyDeviceID,true);
				return TRUE;
			}
			else
			{
				Sleep(200);
				continue;
			}
			if (i == 3)
			{
				lpCPIN_TESTDlg->OutPutDebugLog("WRITE DeviceID TEST FAIL");
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,"ERROR",false);
				return false;
			}
		}
	
	}
	//return true;
}

BOOL MYWORK::PHONE_Read_FixtureID(CString &FixtureID)
{
	CString Commond="",ReceiveData="",ERRINFO="",ReadSNCommand="",sSN="";
	CString Flag="";
	char cFlag[128]={NULL};
	long data = 0;
	int index1=0,index2=0;
	CString ReceiveBuffer="";
	CString keyValue="OK";
	lpCPIN_TESTDlg->OutPutDebugLog("READ FixtureID ...");
	Commond ="CKHYMOST";
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,Commond,ReceiveData,keyValue,3,ERRINFO))
	{
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		lpCPIN_TESTDlg->OutPutDebugLog("READ FixtureID TEST FAIL");
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_READ_FixtureID,"ERROR",false);
		return false;
	}
	else
	{
		for (int i = 0;i<3;i++)
		{
			if (ReceiveData.Find("OK")==-1)
			{
				lpCPIN_TESTDlg->OutPutDebugLog("READ FixtureID FAIL");
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_READ_FixtureID,"ERROR",false);
				return false;
			}
			else
			{
				index1 = ReceiveData.Find("*N:");
				index2 = ReceiveData.Find("*R:")-index1-3;
				FixtureID = ReceiveData.Mid(index1+3,index2);
				lpCPIN_TESTDlg->OutPutDebugLog("READ FixtureID SUCCESS");
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_READ_FixtureID,FixtureID,true);
				return true;
			}
		}		
	}
	return true;
}

void MYWORK::GetDataBySpecificChar(CString inputstr,CArray<CString,CString> &OutputData,CString _Delim)
{
	char *str_getvalu=NULL;
	char cDelim[100]={NULL};

	memcpy(cDelim,_Delim,strlen(_Delim));

	OutputData.SetSize(0);

	for (int i=0;i<5000;i++)
	{
		str_getvalu=NULL;

		if(i==0)
		{
			str_getvalu=strtok(inputstr.GetBuffer(strlen(inputstr)),cDelim);
			inputstr.ReleaseBuffer();
			if (str_getvalu == NULL)
			{
				break;
			}
			OutputData.Add(str_getvalu);
		}
		else
		{
			str_getvalu=strtok(NULL,cDelim);
			if (str_getvalu == NULL)
			{
				break;
			}
			OutputData.Add(str_getvalu);
		}	
	}
	return ;
}

int MYWORK::round_double(double number)
{
	return (number > 0.0) ? floor(number + 0.5) : ceil(number - 0.5);
}

int MYWORK:: rad(double degree)  
{  
	double PI = 3.141592657;
	return  PI * degree / 180.0;  
}  

double MYWORK:: haverSin(double x)  
{  
	double v = sin(x / 2.0);  
	return v * v;  
}  

double MYWORK:: GetDistance(double lat1, double lng1, double lat2, double lng2)
{
	double EARTH_RADIUS = 6378137;//赤道半径(单位m)
	double radlon1 = rad(lng1);// 求弧度  
	double radlat1 = rad(lat1);  
	double radlon2 = rad(lng2);  
	double radlat2 = rad(lat2);  
	double a = fabs(radlat1 - radlat2);// 纬度  
	double b = fabs(radlon1 - radlon2);  
	//double h = haverSin(b) + cos(lat1)*cos(lat2)*haverSin(a);  
	//double distance = 2 * EARTH_RADIUS * asin(sqrt(h));  
	//return  distance;  
	double s = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radlat1) * cos(radlat2) * pow(sin(b / 2), 2)));  
	s = s * EARTH_RADIUS;  
	return s; 

}

bool MYWORK::CheckWordType(CString str, int iType) {
	CString sOutput = "";
	if (str.IsEmpty()) {
		//lpCConfigToolDlg->OutPutDebugLog("Number is null");
		return false;
	}
	else{
		if (0 == iType) {//不能有逗号
			TCHAR NV_Data[512] = { 0 };
			memcpy(NV_Data, str, str.GetLength());
			for (int i = 0; i < str.GetLength(); i++)
			{
				if (NV_Data[i] == ',')
				{
					//lpCConfigToolDlg->OutPutDebugLog("Illegal Character,No \",\"");
					return false;
				}
			}
		}
		if (1 == iType) {//必须数字
			TCHAR NV_Data[512] = { 0 };
			memcpy(NV_Data, str, str.GetLength());
			for (int i = 0; i < str.GetLength(); i++)
			{
				if (!((NV_Data[i] >= '0' && NV_Data[i] <= '9')))
				{
					//lpCConfigToolDlg->OutPutDebugLog("Illegal Character,Only digit");
					return false;
				}
			}
		}
		if (2 == iType) {//数字和字符
			TCHAR NV_Data[30] = { 0 };
			memcpy(NV_Data, str, str.GetLength());
			for (int i = 0; i < str.GetLength(); i++)
			{
				if (!((NV_Data[i] >= '0' && NV_Data[i] <= '9') || (NV_Data[i] >= 'a' && NV_Data[i] <= 'z') || (NV_Data[i] >= 'A' && NV_Data[i] <= 'Z')))
				{
					//lpCConfigToolDlg->OutPutDebugLog("Illegal Character,0-9 and a-Z");
					return false;
				}
			}
		}
	}
	return true;
}

bool MYWORK::TEST_SEQ(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------SEQ TEST---------------------------");
	CString sATcmd="AT+MCUSEQ?",sKeyValue="OK";
	int index1=0;
	if (m_MessageDialog.DoModal() != 1) {
		//THREAD_WORK = FALSE;
		//lpCConfigToolDlg->ShowBmp(READY);
		//return RES;
	}
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("OK")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find("?")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			index1=sStrDebug.Find("OK");
			sStrDebug=sStrDebug.Left(index1).Trim();
			m_MMIDATA.sSEQ=sStrDebug;
			return true;	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_SPIFLASH(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------SPIFLASH TEST---------------------------");
	CString sATcmd="AT+SPIFLASH",sKeyValue="OK";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("OK")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{  //返回OK即PASS
			sStrDebug=sReceivedata.Trim();
			return true;
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_MCU_Version(CString &sStrDebug,CString &sReceivedata)
{	
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------MCU Ver TEST---------------------------");
	CString sATcmd="AT+MCUVER",sKeyValue="MCUVER";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("MCUVER")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			m_MMIDATA.sMCUVer=sStrDebug;
			if (sStrDebug.Find(m_TestItem.Mcu_sw_v)!=-1) return true;
			else goto FAIL;		
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_UC15_Version(CString &sStrDebug,CString &sReceivedata)
{	
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------UC15 Ver TEST---------------------------");
	CString sATcmd="AT+UCVERSION",sKeyValue="UCVERSION";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("UCVERSION")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			m_MMIDATA.sUC15Ver=sStrDebug;
			if (sStrDebug.Find(m_TestItem.sUC15Version)!=-1) return true;	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_MCU_hw_Version(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------MCU Ver TEST---------------------------");
	CString sATcmd="AT+MCUVER",sKeyValue="MCUVER";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("MCUVER")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			if (sStrDebug.Find(m_TestItem.Mcu_sw_v)!=-1) return true;
			else goto FAIL;		
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_IMEI(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------READ UC15IMEI TEST---------------------------");
	CString sATcmd="AT+IMEI?",sKeyValue="IMEI";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("IMEI")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			if (sStrDebug.GetLength()!=15) goto FAIL;
			return true;	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_EXTEMPTER(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------EXTEMPTER TEST---------------------------");
	CString sATcmd="AT+EX_TEMPTER",sKeyValue="TEMPTER";
	int index1=0;
	float fStrDebug=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("TEMPTER")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			m_MMIDATA.sEXTempter=sStrDebug;
			fStrDebug=atof(sStrDebug);
			if ((fStrDebug>m_TestItem.TEMP_Min)&&(fStrDebug<m_TestItem.TEMP_Max)) return true;
			else goto FAIL;	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_EXHUMIDTY(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------EXHUMIDTY TEST---------------------------");
	CString sATcmd="AT+EX_HUMIDTY",sKeyValue="HUMIDTY";
	int index1=0;
	float fStrDebug=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("HUMIDTY")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			m_MMIDATA.sEXHUMIDTY=sStrDebug;
			fStrDebug=atof(sStrDebug);
			if ((fStrDebug>m_TestItem.HUMIDTY_Min)&&(fStrDebug<m_TestItem.HUMIDTY_Max)) return true;
			else goto FAIL;	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_Tempter(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------TEMPTER TEST---------------------------");
	CString sATcmd="AT+TEMPTER",sKeyValue="TEMPTER";
	int index1=0;
	float fStrDebug=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("TEMPTER")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			m_MMIDATA.sTempter=sStrDebug;
			fStrDebug=atof(sStrDebug);
			if ((fStrDebug>m_TestItem.TEMP_Min)&&(fStrDebug<m_TestItem.TEMP_Max)) return true;
			else goto FAIL;	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_HUMIDTY(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HUMIDTY TEST---------------------------");
	CString sATcmd="AT+HUMIDTY",sKeyValue="HUMIDTY";
	int index1=0;
	float fStrDebug=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("HUMIDTY")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			m_MMIDATA.sHUMIDTY=sStrDebug;
			fStrDebug=atof(sStrDebug);
			if ((fStrDebug>m_TestItem.HUMIDTY_Min)&&(fStrDebug<m_TestItem.HUMIDTY_Max)) return true;
			else goto FAIL;	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_Gsensor(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------Gsensor TEST---------------------------");
	CString sATcmd="AT+GSENSOR",sKeyValue="GSENSOR",sX="",sY="",sZ="";
	int index1=0,index2=0;
	float fX=0,fY=0,fZ=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("GSENSOR")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			sStrDebug=sReceivedata;
			index1=sReceivedata.Find("x")+2;
			sX=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();//去掉x前面字串			
			index2=sX.Find("y")-2;//去掉Y前面字串
			sX=sX.Left(index2);
			fX=atof(sX);
			if (fabs(fX)>60.00) goto FAIL;
			index1=sReceivedata.Find("y")+2;
			sY=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();	
			index2=sY.Find("z")-2;
			sY=sY.Left(index2);
			fY=atof(sY);
			if (fabs(fY)>60.00) goto FAIL;
			index1=sReceivedata.Find("z")+2;
			sZ=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();		
			sZ=sZ.Left(sZ.GetLength()-1).Trim();
			fZ=atof(sZ);
			if ((fabs(fZ)>1040.00)||(fabs(fZ)<920.00)) goto FAIL;
			sZ.Format("%.2f",fZ);
			m_MMIDATA.sGsensor="X:"+sX+" Y:"+sY+" Z:"+sZ;
			sStrDebug=m_MMIDATA.sGsensor;
			lpCPIN_TESTDlg->OutPutDebugLog(m_MMIDATA.sGsensor);
			return true;	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_SIM(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------SIM TEST---------------------------");
	CString sATcmd="AT+SIMTEST",sKeyValue="CPIN";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("CPIN")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			if (sStrDebug=="1") return true;
			else goto FAIL;	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_ICCID(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------ICCID TEST---------------------------");
	CString sATcmd="AT+SIMICCID",sKeyValue="ICCID";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("ICCID")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			if (sStrDebug.GetLength()==20) return true;
			else goto FAIL;	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_UC15Open(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------UC15Open TEST---------------------------");
	CString sATcmd="AT+UC15EN",sKeyValue="OK";
	int index1=0;
	if (!m_PHONE.HM003SendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,60,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("OK")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			return true;
			/*if (sStrDebug=="0") return true;
			else goto FAIL;	*/	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_WDOG(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------WDOG TEST---------------------------");
	CString sRes="";
	CString sATcmd="AT+WDTTEST",sKeyValue="power on";
	int index1=0;
	if (!m_PHONE.WDOGTestCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,60,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("power on")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			Sleep(500);
			CString scmd="AT+FACTTEST",sValue="OK";
			if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,scmd,sRes,sValue,30,sStrDebug)) goto FAIL;
			for (int i = 0;i<3;i++)
			{  
				if (sRes.Find("OK")==-1)
				{
					if (i==2) goto FAIL;
					continue;
				}
				else
				{
					Sleep(500);
					sATcmd="AT+GCRESOURCE";
					sKeyValue="OK";
					if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sRes,sKeyValue,3,sStrDebug)) goto FAIL;
					sStrDebug=sRes;
					lpCPIN_TESTDlg->ShowInfor("进入工厂模式成功!");
					return true;	
				}
				Sleep(500);
			}
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_CSQ(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------CSQ TEST---------------------------");
	CString sATcmd="AT+CSQ",sKeyValue="CSQ";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("CSQ")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			if ((atoi(sStrDebug)<5)||(atoi(sStrDebug)>31)) goto FAIL;
			else return true;	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_CREG(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------CREG TEST---------------------------");
	CString sATcmd="AT+CREG",sKeyValue="CREG";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("CREG")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			if (sStrDebug=="1") return true;
			else goto FAIL;		
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_CGREG(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------CGREG TEST---------------------------");
	CString sATcmd="AT+CGREG",sKeyValue="CGREG";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("CGREG")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			if (sStrDebug=="1") return true;
			else goto FAIL;		
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_GPS(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------GPS TEST---------------------------");
	char *str_getvalu=NULL;
	CArray<CString,CString>ValueList;
	CString sATcmd="AT+GPSTEST",sKeyValue="gps",sTemp="";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,80,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("gps")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			m_MMIDATA.sGPS=sStrDebug;
			if (sStrDebug.GetLength()>=9 && (sStrDebug.Find(m_TestItem.GPS_Latitude)!=-1) && (sStrDebug.Find(m_TestItem.GPS_Longitude)!=-1 )) 
			{  //分割
				for (int i=0;i<50;i++)
				{
					str_getvalu=NULL;
					if(i==0)
					{
						str_getvalu=strtok(sStrDebug.GetBuffer(strlen(sStrDebug)),",");
						sStrDebug.ReleaseBuffer();
						ValueList.Add(str_getvalu);
						continue;
					}
					str_getvalu=strtok(NULL,",");
					if (str_getvalu == NULL)
					{
						break;
					}
					ValueList.Add(str_getvalu);
				} 
				if (3==ValueList.GetSize())
				{
					m_MMIDATA.sGPS_latitude=ValueList.GetAt(0);
					sTemp.Format("GPS_latitude: "+m_MMIDATA.sGPS_latitude);
					lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
					m_MMIDATA.sGPS_longitude=ValueList.GetAt(1);
					sTemp.Format("GPS_longitude: "+m_MMIDATA.sGPS_longitude);
					lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
					m_MMIDATA.sGPS_Star=ValueList.GetAt(2);
					sTemp.Format("GPS_Start: "+m_MMIDATA.sGPS_Star);
					lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
					m_MMIDATA.sGPS_Coordinate.Format("%s,%s",m_MMIDATA.sGPS_latitude,m_MMIDATA.sGPS_longitude);
					if (atoi(m_MMIDATA.sGPS_Star)>=2)	return true;
				}
				else goto FAIL;		
			}
			else goto FAIL;		
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(m_MMIDATA.sGPS);
	return FALSE;
}

bool MYWORK::TEST_CHECK_WIFI(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------WIFI TEST---------------------------");
	CString sATcmd="AT+WIFITEST",sKeyValue="WIFITEST";
	int index1=0;
	clock_t start,stop;
	float fScanTime,Timeout_s=10;//控制循环读取次数
	if (!m_PHONE.SendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,3,sStrDebug)) goto FAIL;
	Sleep(50);
	start=clock();//计时开始毫秒为单位
	while(1)
	{
		stop=clock();
		fScanTime=(float)(stop-start)/CLOCKS_PER_SEC;
		if(fScanTime>Timeout_s)
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Can't Find The Matched MAC");
			return false;
		}
		m_PHONE.ReadDataOnly(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sReceivedata,30,sStrDebug);
		if (sReceivedata.Find("WIFITEST")!=-1)
		{		
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
		    if (sStrDebug.Find(m_TestItem.sWIFI1)!=-1) return true;
			if (sStrDebug.Find(m_TestItem.sWIFI2)!=-1) return true;
			if (sStrDebug.Find(m_TestItem.sWIFI3)!=-1) return true;
			if (sStrDebug.Find(m_TestItem.sWIFI4)!=-1) return true;
			if (sStrDebug.Find(m_TestItem.sWIFI5)!=-1) return true;
			return false;
		}
	}	
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_BT(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------BT TEST---------------------------");
	CString sATcmd="AT+BTTEST=1",sKeyValue="OK";
	/*if (!m_PHONE.SendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,10,sStrDebug)) goto FAIL;
	Sleep(500);
	sATcmd="AT+BTTEST?";*/
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,10,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("rssi")==-1)
		{
			if (!READ_BT(sReceivedata))
			{
				if (i==2) goto FAIL;
				continue;
			}
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			if (sStrDebug!="")
			{
				sATcmd="AT+BTTEST=0";
				if (!m_PHONE.SendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,10,sStrDebug)) goto FAIL;
				Sleep(500);
				if (sStrDebug.Find(m_TestItem.sBT1)!=-1) return true;
				if (sStrDebug.Find(m_TestItem.sBT2)!=-1) return true;
				if (sStrDebug.Find(m_TestItem.sBT3)!=-1) return true;
			}
			else 
			{
					goto FAIL;
			}
		}
		Sleep(200);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_Battery(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------BATT TEST---------------------------");
	CString sATcmd="AT+BATV",sKeyValue="BATV";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("BATV")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find(":")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			float fStrDebug=atof(sStrDebug)/1000;
			m_MMIDATA.sBVol.Format("%.3fV",fStrDebug);
			sStrDebug=m_MMIDATA.sBVol;
			if (fStrDebug>m_TestItem.Battery_VMain) return true;
			else goto FAIL;		
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

int MYWORK::GetTextPosFromString(CString szString, CString szSymbol, int nIndex)
{
	int i;
	int nSymbolCount,nFindPos;
	int nRet;

	nSymbolCount = 0;
	nFindPos = 0;
	nRet = -1;

	if(nIndex < 1)
	{
		return -2;
	}

	if(szString.Right(1) != szSymbol)
	{
		szString += szSymbol;
	}

	for(i=0; i<szString.GetLength(); i++)
	{
		nFindPos = szString.Find(szSymbol, nFindPos);

		if(nFindPos == -1)
		{
			break;
		}
		else
		{
			nSymbolCount++;

			if(nSymbolCount == nIndex)
			{
				nRet = nFindPos;
				break;
			}

			nFindPos++;
		}
	}

	return nRet;
}

CString MYWORK::GetTextFromStringEx(CString szString, CString szSymbol, int nIndex)
{
	int nStartPos,nEndPos;
	CString szRet("");

	nStartPos = 0;
	nEndPos = 0;

	if(nIndex > 0)
	{
		nEndPos = GetTextPosFromString(szString, szSymbol, nIndex);

		if(nEndPos > 0)
		{
			if(nIndex > 1)
			{
				nStartPos = GetTextPosFromString(szString, szSymbol, (nIndex - 1)) + 1;
			}
		}
	}
	//找到
	if(nEndPos > 0)
	{
		szRet = szString.Mid(nStartPos, (nEndPos - nStartPos));
	}

	return szRet;
}

bool MYWORK::CHECK_BSN(CString sBSN,CString sReadBSN,CString sMESBSN)
{
	lpCPIN_TESTDlg->OutPutDebugLog("Compare BSN ...");
	if (m_CONFIG.bMES)
	{
		if (strcmp(sBSN,sReadBSN)!=0 || strcmp(sBSN,sMESBSN)!=0)
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Compare BSN FAIL");
			return false;
		}
		else
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Compare BSN PASS");
			return true;
		}
	}
	else
	{
		if (strcmp(sBSN,sReadBSN)!=0)
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Compare BSN FAIL");
			return false;
		}
		else
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Compare BSN PASS");
			return true;
		}
	}
}

bool MYWORK::CHECK_NUMBER(CString sScanSN,CString sSN,CString sMESSN)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------Compare NUMBER-----------------------------");
	CString sTemp="";
	sTemp.Format("sScanSN:%s, sSN:%s, sMESSN:%s",sScanSN,sSN,sMESSN);
	lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
	sScanSN.MakeUpper();
	sSN.MakeUpper();
	sMESSN.MakeUpper();
	if (m_CONFIG.bMES)
	{
		if (strcmp(sScanSN,sSN)!=0 || strcmp(sScanSN,sMESSN)!=0)
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Compare FAIL");
			return false;
		}
		else
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Compare PASS");
			return true;
		}
	}
	else
	{
		if (strcmp(sScanSN,sSN)!=0)
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Compare SN FAIL");
			return false;
		}
		else
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Compare SN PASS");
			return true;
		}
	}
}

bool MYWORK::CHECK_IMEI(CString &sIMEI,CString sMESIMEI)
{
	lpCPIN_TESTDlg->OutPutDebugLog("Compare IMEI ...");
	CString Commond="TEST",ReceiveData="",ERRINFO="";
	if (!m_PHONE.HMBSendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,Commond,ReceiveData,3,ERRINFO))
	{
		lpCPIN_TESTDlg->OutPutDebugLog(ReceiveData);
		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
		return false;
	}
	else
	{
		lpCPIN_TESTDlg->OutPutDebugLog(ReceiveData);
		if (ReceiveData.Find(sMESIMEI)!=-1)
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Compare IMEI PASS");
			return true;
		}
		else
		{
			return false;
		}
	}	
}

bool MYWORK::CHECK_FixtureID(CString sScanFixtureID,CString sFixtureID)
{
	lpCPIN_TESTDlg->OutPutDebugLog("Compare FixtureID ...");
	if (m_CONFIG.bMES)
	{
		//CString sMESFixtureID = sMESPSN.Mid(4,11);
		if (strcmp(sScanFixtureID,sFixtureID)!=0)
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Compare FixtureID FAIL");
			return false;
		}
		else
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Compare FixtureID PASS");
			return true;
		}
	} 
	else
	{
		if (strcmp(sScanFixtureID,sFixtureID)!=0)
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Compare FixtureID PASS");
			return true;
		}
	}
	return true;
}

bool MYWORK::IDLE_Current()
{
	CString IDLE_Current="",ERRCODE="",sOutput="",sMaxCurrent="";
	CString str;
	float fAvgCurrent=0,fCurrent=0,fMaxCurrent=0;
	lpCPIN_TESTDlg->OutPutDebugLog("------------------------------------------------------------------------");
	lpCPIN_TESTDlg->OutPutDebugLog("IDLE CurrentTest...");
	lpCPIN_TESTDlg->OutPutDebugLog("------------------------------------------------------------------------");
	//进入低功耗模式
	if (!MTK_EnterIDLE_AT())
	{
		lpCPIN_TESTDlg->OutPutDebugLog("ENTER IDLE FAIL");
		return false;
	}
	Sleep(2000);
	for (int i = 0;i<8;i++)
	{
		if(!m_PowerControl.PowerGetuACurrent(fCurrent))
		{
			lpCPIN_TESTDlg->OutPutDebugLog("GET IDLE VOLT FAIL");
			return false;
		}
		else
		{
			if (i == 3)
			{
				str.Format("%.3f",fCurrent);
				lpCPIN_TESTDlg->OutPutDebugLog("IDLE VOLT TEST FAIL");
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_IDLE,str,false);
				return false;
			}
			if (fCurrent>20)
			{
				Sleep(50);
				continue;
			}
			if (fCurrent>=m_TestItem.Current_Min||fCurrent<=m_TestItem.Current_Max)
			{
				str.Format("%.3f",fCurrent);
				lpCPIN_TESTDlg->OutPutDebugLog("IDLE VOLT TEST PASS");				
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_IDLE,str,true);
				return true;
			}
			else
			{
				Sleep(50);
				continue;
			}
			
		}
	}
	
}

bool MYWORK::MTK_EnterIDLE_AT()
{
	CString sOutput="",ERRCODE="",ATCOMMOND="";
	CString Readdata="";
	ATCOMMOND="<ENTER LPM>";
	if(!m_PHONE.SendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,ATCOMMOND,Readdata,3,ERRCODE))
	{
		sOutput.Format("AT Fail :%s",ERRCODE);
		lpCPIN_TESTDlg->OutPutDebugLog(sOutput);
		return false;
	}
	else
	{
		lpCPIN_TESTDlg->OutPutDebugLog("send ENTER LPM PASS");
		lpCPIN_TESTDlg->OutPutDebugLog("ENTER IDLE PASS");
		return true;
	}
}

bool MYWORK::sendCmdSccess(int iTimes)
{
	//##Total_ver:M1000V1008G0000,
	//抓取到版本表示成功启动
	CString Commond="TEST",ReceiveData="",ERRINFO="";
	CString resVersion="";
	int iTestTimes=0;
	bool bRun=false;
	lpCPIN_TESTDlg->OutPutDebugLog("检查产品是否启动完全...");
	do 
	{
		iTestTimes++;
		if (!m_PHONE.HMBSendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,Commond,ReceiveData,5,ERRINFO))
		{
			lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
			
		}
		else
		{
			if (ReceiveData.Find("##Total_ver:")!=-1)
			{
				return true;
			}
			else
			{
				
			}
		}
		Sleep(2000);
	} while (iTestTimes<iTimes);
	
	if (!bRun)
	{
		return false;
	}
	return false;
}

bool MYWORK::AUTOWIFIBT(CString&sWifi,CString&sBt)
{
	SYSTEMTIME pSystemtime;
	GetLocalTime(&pSystemtime);
	CString Day ="",Hour ="",Minute ="",Second ="";
	Day.Format("%d",pSystemtime.wDay);
	Hour.Format("%d",pSystemtime.wHour);
	Minute.Format("%d",pSystemtime.wMinute);
	Second.Format("%d",pSystemtime.wSecond);
	if (Day.GetLength() ==1)
	{
		Day = "0"+Day;
	}
	if (Hour.GetLength() ==1)
	{
		Hour = "0"+Hour;
	}
	if (Minute.GetLength() ==1)
	{
		Minute = "0"+Minute;
	}
	if (Second.GetLength() ==1)
	{
		Second = "0"+Second;
	}
	sWifi.Format("%s%s%s%s%s%s","68","D0",Day,Hour,Minute,Second);
	Second.Format("%d",pSystemtime.wSecond+1);
	if (Second.GetLength() ==1)
	{
		Second = "0"+Second;
	}
	sBt.Format("%s%s%s%s%s%s","68","D0",Day,Hour,Minute,Second);//修改前缀为68-》F6
	return true;
}

bool MYWORK::ENTER_FACTTEST(int iPhonePort, CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("---------------------------Open UartPort---------------------------");
	CString sATcmd = "log halt", sKeyValue = "32muart", sReceivedata="";
	int index1 = 0;
	if (!m_PHONE.KeySendAtCommond(iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	for (int i = 0; i < 3; i++) {
		if (sReceivedata.Find(sKeyValue) == -1) {
			if (i == 2) goto FAIL;
			continue;
		}
		else {
			sStrDebug = sReceivedata;
            return true;
		}
		Sleep(500);
	}
FAIL:
	sStrDebug = sReceivedata;
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::READ_BT(CString &sReceiveData)
{
	lpCPIN_TESTDlg->OutPutDebugLog("WAIT TO READ BT");
	CString Commond="",sRes="",ERRINFO="",sStrDebug="";
	clock_t start,stop;
	float fScanTime,Timeout_s=10;
	lpCPIN_TESTDlg->ShowInfor("WAIT BT");
	start=clock();//计时开始毫秒为单位
	sReceiveData="";
	do 
	{
		stop=clock();
		fScanTime=(float)(stop-start)/CLOCKS_PER_SEC;
		if(fScanTime>Timeout_s)
		{
			lpCPIN_TESTDlg->ShowInfor("等待超时!");
			return false;
		}
		//if (sReceiveData.Find("OK")!=-1) break;
		if (sReceiveData!="") lpCPIN_TESTDlg->OutPutDebugLog(sReceiveData);
		m_PHONE.ReadDataOnly(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sReceiveData,10,ERRINFO);
		if (sReceiveData!="") lpCPIN_TESTDlg->OutPutDebugLog(sReceiveData);
		if (sReceiveData.Find("rssi")!=-1)
		{			
			
		   lpCPIN_TESTDlg->ShowInfor("READ DONE");
		   return true;	

FAIL:
			lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
			return FALSE;
		}
	} while (1);
}

bool MYWORK::REBOOT_CHECK(CString &sReceiveData)
{
	lpCPIN_TESTDlg->OutPutDebugLog("---------------------------REBOOT AND CHECK---------------------------");
	CString Commond="",sRes="",ERRINFO="",sStrDebug="";
	CString sATcmd="AT+REBOOT",sKeyValue="OK";
	clock_t start,stop;
	float fScanTime,Timeout_s=15;
	lpCPIN_TESTDlg->ShowInfor("重启中!");
	if (!m_PHONE.SendCommondOnly(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sRes,1,sStrDebug)) goto FAIL;
	start=clock();//计时开始毫秒为单位
	do 
	{
		stop=clock();
		fScanTime=(float)(stop-start)/CLOCKS_PER_SEC;
		if(fScanTime>Timeout_s)
		{
			lpCPIN_TESTDlg->ShowInfor("等待超时，重启没有成功!");
			return false;
		}
		if (sReceiveData.Find("OK")!=-1) break;
		if (sReceiveData!="") lpCPIN_TESTDlg->OutPutDebugLog(sReceiveData);
		m_PHONE.ReadDataOnly(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sReceiveData,30,ERRINFO);
		if (sReceiveData!="") lpCPIN_TESTDlg->OutPutDebugLog(sReceiveData);
		if (sReceiveData.Find("power on")!=-1)//For Godi
		{			
			Sleep(500);
			lpCPIN_TESTDlg->ShowInfor("重启成功!");
			return true;	
FAIL:
			lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
			return FALSE;
		}
	} while (1);
}

bool MYWORK::OUT_FACTTEST(CString &sReceivedata)
{	
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------OUT_FACTTEST TEST---------------------------");
	CString sATcmd="AT+ENTERLPM",sKeyValue="OK",sStrDebug="";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,60,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("OK")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			lpCPIN_TESTDlg->ShowInfor("进低功耗模式成功!");
			return true;

		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::GCRESource_Test(CString &sReceivedata)
{	
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------GCRESource_TEST---------------------------");
    CString Commond="",sRes="",ERRINFO="",sStrDebug="";
	CString sATcmd="AT+GCRESOURCE",sKeyValue="OK";
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sRes,sKeyValue,3,sStrDebug)) goto FAIL;
	sStrDebug=sRes;
	for (int i = 0;i<3;i++)
	{  
		if (sRes.Find("OK")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			return true;
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::Reboot_Test(CString &sReceivedata)
{	
	//lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------Reboot_TEST---------------------------");
	CString Commond="",sRes="",ERRINFO="",sStrDebug="";
	CString sATcmd="AT+REBOOT",sKeyValue="OK";
	if (!m_PHONE.SendCommondOnly(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sRes,1,sStrDebug)) goto FAIL;
	sStrDebug=sRes;	
	return true;
FAIL:
	//lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::PHONE_Write_CFGIN(CString &sStrDebug,CString &sReceivedata)
{	
	lpCPIN_TESTDlg->ShowInfor("出货参数导入中，请等待!");
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------CFGIN TEST---------------------------");
	CString sLOG="",sATcmd="",sKeyValue="",sKeyWord=""; 
	for(int i=0;i<m_EXCELTestItem.TestIndex;i++)
	{
		if (m_EXCELTestItem.m_TestList[i].ENABLE)
		{
			sLOG.Format("----------CFGIN ITEM: %d----------",i);
			lpCPIN_TESTDlg->OutPutDebugLog(sLOG);
			sATcmd=m_EXCELTestItem.m_TestList[i].ATCommond,sKeyValue=m_EXCELTestItem.m_TestList[i].TargetReturnValue;
			sKeyWord.Format("KeyWord is:%s",sKeyValue);
			lpCPIN_TESTDlg->OutPutDebugLog(sKeyWord);	
			if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,5,sStrDebug)) goto FAIL;
			if ((sReceivedata.Find(sKeyValue)==-1)||(sReceivedata.Find("FAIL")!=-1))
			{
				goto FAIL;
			}
			else
			{
				if (i==m_EXCELTestItem.TestIndex-1) 
				{
					sStrDebug.Format("出货参数导入成功");
					lpCPIN_TESTDlg->ShowInfor(sStrDebug);
					return true;
				}
			}
		}
	}
FAIL:
	sStrDebug.Format("出货参数导入失败！！！");
	lpCPIN_TESTDlg->ShowInfor(sStrDebug);
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::PHONE_Write_DefIN(CString &sStrDebug,CString &sReceivedata)
{	
	lpCPIN_TESTDlg->ShowInfor("默认参数导入中，请等待!");
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------DEFIN TEST---------------------------");
	CString sLOG="",sATcmd="",sKeyValue="",sKeyWord=""; 
	for(int i=0;i<m_DefEXCELTestItem.TestIndex;i++)
	{
		if (m_DefEXCELTestItem.m_TestList[i].ENABLE)
		{
			sLOG.Format("----------DEFIN ITEM: %d----------",i);
			lpCPIN_TESTDlg->OutPutDebugLog(sLOG);
			sATcmd=m_DefEXCELTestItem.m_TestList[i].ATCommond,sKeyValue=m_DefEXCELTestItem.m_TestList[i].TargetReturnValue;
			sKeyWord.Format("KeyWord is:%s",sKeyValue);
			lpCPIN_TESTDlg->OutPutDebugLog(sKeyWord);
			if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,5,sStrDebug)) goto FAIL;
			if (sReceivedata.Find(sKeyValue)==-1)
			{
				goto FAIL;
			}
			else
			{
				if (i==m_DefEXCELTestItem.TestIndex-1) 
				{
					sStrDebug.Format("默认参数导入成功");
					lpCPIN_TESTDlg->ShowInfor(sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
					return true;
				}
			}
		}
	}
FAIL:
	sStrDebug.Format("默认参数导入失败！！！");
	lpCPIN_TESTDlg->ShowInfor(sStrDebug);
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::PHONE_Write_CFGQue(CString &sStrDebug,CString &sReceivedata)
{	
	lpCPIN_TESTDlg->ShowInfor("客户出货参数核对中，请等待!");
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------CFGQue TEST---------------------------");
	CString sLOG="",sATcmd="",sKeyValue="",sKeyWord=""; 
	for(int i=0;i<m_EXCELTestItem.TestIndex;i++)
	{
		if (m_EXCELTestItem.m_TestList[i].ENABLE)
		{
			sLOG.Format("----------CFGQue ITEM: %d----------",i);
			lpCPIN_TESTDlg->OutPutDebugLog(sLOG);
			sATcmd=m_EXCELTestItem.m_TestList[i].ATQuery,sKeyValue=m_EXCELTestItem.m_TestList[i].TargetReturnValue;
			sKeyWord.Format("KeyWord is:%s",sKeyValue);
			lpCPIN_TESTDlg->OutPutDebugLog(sKeyWord);	
			if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,5,sStrDebug)) goto FAIL;
			if ((sReceivedata.Find(sKeyValue)==-1)||(sReceivedata.Find("FAIL")!=-1))
			{
				goto FAIL;
			}
			else
			{
				if (i==m_EXCELTestItem.TestIndex-1) 
				{
					sStrDebug.Format("客户出货参数核对成功");
					lpCPIN_TESTDlg->ShowInfor("客户出货参数核对成功!");
					return true;
				}
			}
		}
	}
FAIL:
	lpCPIN_TESTDlg->ShowInfor("客户出货参数核对失败!!!");
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::PHONE_Write_Trans(REPINF *pHRepInf,REPINF *pSRepInf,REPINF *pTRepInf)
{	
	lpCPIN_TESTDlg->ShowInfor("参数计算中，请等待!");
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------Tran TEST---------------------------");
	CString sLOG="",sATcmd="",sKeyValue="",sKeyWord="",sTemp="",sStrDebug=""; 
	CString sHDefaultB="", sHDefaultH="",sSDefaultB="", sSDefaultH="",sTDefaultB="", sTDefaultH="";
	int x=0,v=0,iBitNum=0;
	char bs[33]={0};
	char b[33]={0};
	char hs[9]={0};
	char h[18]={0};
	char s[4]={0};
	char *e={0};
	for(int i=0;i<m_EXCELTestItem.TestIndex;i++)
	{
		sLOG.Format("----------Tran ITEM: %d,%s----------",i,m_EXCELTestItem.m_TestList[i].Name);
		lpCPIN_TESTDlg->OutPutDebugLog(sLOG);
		if (m_EXCELTestItem.m_TestList[i].Name.Find("H")!=-1||m_EXCELTestItem.m_TestList[i].Name.Find("S")!=-1||
			m_EXCELTestItem.m_TestList[i].Name.Find("A")!=-1||m_EXCELTestItem.m_TestList[i].Name.Find("P")!=-1)
		{
			iBitNum++;
			if (iBitNum<=32)
			{
				pHRepInf->sName=m_EXCELTestItem.m_TestList[i].Name;//索引名称
				sTemp.Format("%d",atoi(m_EXCELTestItem.m_TestList[i].Default));//值
				sHDefaultB=sHDefaultB+sTemp;//二进制原数字，后读取的为低位
				pHRepInf->sDefaultB=sHDefaultB;//记录
				lpCPIN_TESTDlg->OutPutDebugLog(sHDefaultB);	//输出LOG
				char *stopstring; 
				const char *cBin = (LPSTR)(LPCTSTR)sHDefaultB; 
				UINT iValue;  
				iValue =strtoul(cBin, &stopstring, 2); //转换成无符号的长整型数(unsigned long),一定要是无符号的
				char c[20]; 
				sprintf(c, "%x", iValue); 
				CString  str(c); 
				sHDefaultH.Format("%s",str);
				//strcpy(bs,sHDefaultB);
				//x=strtol("10000000000000000000010000000000",&e,2);//字符串转换成2进制长整型数bs = "11110011011000000001011010011111"
				//ltoa(x,h,16);//十六进制，长整形数转换为任意进制的字符串
				//ltoa("11110011011000000001011010011111",h,16);
				//sprintf(hs,"%08s",h);
				//sHDefaultH.Format("%s",hs);//转为十六进制数
				sHDefaultH.MakeUpper();
				pHRepInf->sDefaultH=sHDefaultH;
				lpCPIN_TESTDlg->OutPutDebugLog(sHDefaultH);	
			}
			if (iBitNum>32&&iBitNum<=64)
			{
				pSRepInf->sName=m_EXCELTestItem.m_TestList[i].Name;//索引名称
				sTemp.Format("%d",atoi(m_EXCELTestItem.m_TestList[i].Default));//值
				sSDefaultB=sSDefaultB+sTemp;//二进制原数字，后读取的为低位
				pSRepInf->sDefaultB=sSDefaultB;//记录
				lpCPIN_TESTDlg->OutPutDebugLog(sSDefaultB);	//输出LOG

				char *stopstring; 
				const char *cBin = (LPSTR)(LPCTSTR)sSDefaultB; 
				UINT iValue;  
				iValue =strtoul(cBin, &stopstring, 2); //转换成无符号的长整型数(unsigned long),一定要是无符号的
				char c[20]; 
				sprintf(c, "%x", iValue); 
				CString  str(c); 
				sSDefaultH.Format("%s",str);
				sSDefaultH.MakeUpper();
				pSRepInf->sDefaultH=sSDefaultH;
				lpCPIN_TESTDlg->OutPutDebugLog(sSDefaultH);	
			}
			if (iBitNum>64&&iBitNum<=96)
			{
				pTRepInf->sName=m_EXCELTestItem.m_TestList[i].Name;//索引名称
				sTemp.Format("%d",atoi(m_EXCELTestItem.m_TestList[i].Default));//值
				sTDefaultB=sTDefaultB+sTemp;//二进制原数字，后读取的为低位
				pTRepInf->sDefaultB=sTDefaultB;//记录
				lpCPIN_TESTDlg->OutPutDebugLog(sTDefaultB);	//输出LOG
				char *stopstring; 
				const char *cBin = (LPSTR)(LPCTSTR)sTDefaultB; 
				UINT iValue;  
				iValue =strtoul(cBin, &stopstring, 2); //转换成无符号的长整型数(unsigned long),一定要是无符号的
				char c[20]; 
				sprintf(c, "%x", iValue); 
				CString  str(c); 
				sTDefaultH.Format("%s",str);
				sTDefaultH.MakeUpper();
				pTRepInf->sDefaultH=sTDefaultH;
				lpCPIN_TESTDlg->OutPutDebugLog(sTDefaultH);	
			}
		}
	}
	lpCPIN_TESTDlg->ShowInfor("参数计算完成!");
	return true;
//FAIL:
	sStrDebug.Format("出货参数导入失败！！！");
	lpCPIN_TESTDlg->ShowInfor(sStrDebug);
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::PHONE_Read_DefValue(CString &sStrDebug,CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------DefValue TEST---------------------------");
	CString sATcmd="AT+DEFREPFD?",sKeyValue="OK";
	int index1=0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,sATcmd,sReceivedata,sKeyValue,3,sStrDebug)) goto FAIL;
	for (int i = 0;i<3;i++)
	{  
		if (sReceivedata.Find("OK")==-1)
		{
			if (i==2) goto FAIL;
			continue;
		}
		else
		{
			index1=sReceivedata.Find("?")+1;
			sStrDebug=sReceivedata.Right(sReceivedata.GetLength()-index1).Trim();
			int i=sStrDebug.GetLength();			
			if (36!=sStrDebug.GetLength()) goto FAIL;
			else return true;	
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::PHONE_DefValue_Count(CString sDefVal)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------DefValue Count---------------------------");
	CString sHHex="",sSHex="",sTHex="",sStrDebug="";
	char cHbs[33]={0};
	char cSbs[33]={0};
	char cTbs[33]={0};
	char *stopstring; 
	const char *cBin = "";
	UINT iValue=0;  
	char c[20]={0}; 
	char b[33]={0};
	char bs[33]={0};
	CString  str; 

	sHHex=sDefVal.Left(8);
	sSHex=sDefVal.Mid(8,8);
	sTHex=sDefVal.Mid(16,8);

	cBin = (LPSTR)(LPCTSTR)sHHex;
	iValue =strtoul(cBin, &stopstring, 16); //把const char *cBin按16进制转为10无符号长整型进制
	//ltoa(iValue,b,2);
	ultoa(iValue,b,2);
	sprintf(cHbs,"%032s",b);
	sStrDebug.Format("%s,%s",sHHex,cHbs);

	cBin = (LPSTR)(LPCTSTR)sSHex;
	iValue =strtoul(cBin, &stopstring, 16); //把const char *cBin按16进制转为10无符号长整型进制
	//ltoa(iValue,b,2);
	ultoa(iValue,b,2);
	sprintf(cSbs,"%032s",b);
	sStrDebug.Format("%s,%s",sSHex,cSbs);

	cBin = (LPSTR)(LPCTSTR)sTHex;
	iValue =strtoul(cBin, &stopstring, 16); //把const char *cBin按16进制转为10无符号长整型进制
	//ltoa(iValue,b,2);
	ultoa(iValue,b,2);
	sprintf(cTbs,"%032s",b);

	sStrDebug.Format("%s,%s",sHHex,cHbs);
	lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,sStrDebug,true);
	sStrDebug.Format("%s,%s",sSHex,cSbs);
	lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,sStrDebug,true);
	sStrDebug.Format("%s,%s",sTHex,cTbs);
	lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,sStrDebug,true);

	for (int i=0;i<32;i++)
	{
		if (i<26)
		{
			sStrDebug.Format("H%d: %c",i+1,cHbs[i]);
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,sStrDebug,true);
		}
		else
		{
			sStrDebug.Format("S%d: %c",i-25,cHbs[i]);
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,sStrDebug,true);
		}
	}
	for (int i=0;i<32;i++)
	{
		sStrDebug.Format("S%d: %c",i+7,cSbs[i]);
		lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,sStrDebug,true);	
	}
	for (int i=0;i<32;i++)
	{
		if (i<1)
		{
			sStrDebug.Format("S%d: %c",i+39,cTbs[i]);
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,sStrDebug,true);
		}
		if (i>=1&&i<=11)
		{
			sStrDebug.Format("A%d: %c",i,cTbs[i]);
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,sStrDebug,true);
		}	
		if (i>11)
		{
			sStrDebug.Format("P%d: %c",i-11,cTbs[i]);
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_WRITECUSTOMSN,sStrDebug,true);
		}	
	}
	return true;
//FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_MACIMEI_CIP(CString sMESBTMAC,CString sMESIMEI1,CString &sStrDebug)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------MACIMEI CIP TEST---------------------------");
	CString sATcmd="",sReceiveData="",ERRINFO="",sSN="",sMACTurn="";
	int index1=0;

	CString sMacInput="",sMacInv="",sTemp="",sOutPut_Ipnut="",sOutPut_Oput="",sOutPut_Cipher="",sCipherCode="";
	char strInput[32] ={0};
	int Cipherlength=0;

	typedef BOOL(__stdcall*lpCipherFun)(CString sMacInput ,CString &sCipherCode);
	HINSTANCE hDll;
	lpCipherFun m_Cipher,m_InvCipher;
	hDll=LoadLibrary("MacCipher_DLL.dll");
	m_Cipher = (lpCipherFun)GetProcAddress(hDll,"PerformMac_Cipher");
	if (NULL == m_Cipher) MessageBox("获取函数地址失败！");

	m_InvCipher = (lpCipherFun)GetProcAddress(hDll,"PerformMac_InvCipher");
	if (NULL == m_Cipher) MessageBox("获取函数地址失败！");

	sMacInput.Format("%s%s",sMESBTMAC,sMESIMEI1);
	if (sMacInput.GetLength()!=27) goto FAIL;

	if (!m_Cipher(sMacInput,sCipherCode))
	{
		return false;
	}
	sStrDebug.Format("%s",sCipherCode);
	lpCPIN_TESTDlg->OutPutDebugLog(sCipherCode);

	if (!m_InvCipher(sCipherCode,sMacInv))
	{
		return false;
	}
	lpCPIN_TESTDlg->OutPutDebugLog(sMacInv);
	if (sMacInv!=sMacInput) goto FAIL;
	FreeLibrary(hDll);
	return true;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	FreeLibrary(hDll);
	return FALSE;
}

bool MYWORK::TEST_MACIMEI_CIP_INV(CString sMacInput,CString &sMacInv)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------MACIMEI CIP INV TEST---------------------------");
	CString sATcmd="",sReceiveData="",ERRINFO="",sSN="",sMACTurn="";
	int index1=0;

	CString sTemp="",sOutPut_Ipnut="",sOutPut_Oput="",sOutPut_Cipher="",sCipherCode="";
	char strInput[32] ={0};
	int Cipherlength=0;

	typedef BOOL(__stdcall*lpCipherFun)(CString sMacInput ,CString &sCipherCode);
	HINSTANCE hDll;
	lpCipherFun m_Cipher,m_InvCipher;
	hDll=LoadLibrary("MacCipher_DLL.dll");
	m_Cipher = (lpCipherFun)GetProcAddress(hDll,"PerformMac_Cipher");
	if (NULL == m_Cipher) MessageBox("获取函数地址失败！");

	m_InvCipher = (lpCipherFun)GetProcAddress(hDll,"PerformMac_InvCipher");
	if (NULL == m_Cipher) MessageBox("获取函数地址失败！");

	if (!m_InvCipher(sMacInput,sMacInv))
	{
		return false;
	}
	lpCPIN_TESTDlg->OutPutDebugLog(sMacInv);
	FreeLibrary(hDll);
	return true;

//FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog("解码失败");
	FreeLibrary(hDll);
	return FALSE;
}

BOOL MYWORK::AT_TEST_EXC(TestList *pTestList)
{
	CString Commond="",ReceiveData="",ERRINFO="",Log="",TargetValue="",ReturnValue="";
	float	TimeOut=0;

	//Commond=pTestList->ATCommond;
	//TimeOut=atoi(pTestList->TimeOut);

	//for (int i=0;i<3;i++)
	//{
	//	if (!m_PHONE.SendAtCommond(m_CONFIG.iPhonePort,m_CONFIG.BaudRate,Commond,ReceiveData,10,ERRINFO))
	//	{
	//		lpCPIN_TESTDlg->OutPutDebugLog(ERRINFO);
	//		if (i==2)
	//		{
	//			//lpCPIN_TESTDlg->OutPutTestLog(pTestList->ATCommond);
	//			return FALSE;
	//		}
	//	}
	//	else
	//	{
	//		TargetValue=pTestList->TargetReturnValue;
	//		TargetValue.Remove('\n');
	//		TargetValue.Remove('\r');

	//		ReturnValue=ReceiveData;
	//		ReturnValue.Remove('\n');
	//		ReturnValue.Remove('\r');

	//		Log.Format("Target Value:%s",TargetValue);
	//		lpCPIN_TESTDlg->OutPutDebugLog(Log);
	//		Log.Format("Return Value:%s",ReturnValue);
	//		lpCPIN_TESTDlg->OutPutDebugLog(Log);

	//		if (m_CONFIG.CompleteMatching)
	//		{
	//			Log.Format("Check Type:  Complete Matching");
	//			lpCPIN_TESTDlg->OutPutDebugLog(Log);

	//			if (strcmp(ReturnValue,TargetValue)==0)
	//			{
	//				lpCPIN_TESTDlg->OutPutDebugLog("CHECK RETURN VALUE SUCCESS");
	//				lpCPIN_TESTDlg->OutPutTestLog(pTestList->ATCommond,true);
	//				return TRUE;
	//			}
	//			else
	//			{
	//				if(i==2)
	//				{
	//					lpCPIN_TESTDlg->OutPutDebugLog("CHECK RETURN VALUE FAIL");
	//					lpCPIN_TESTDlg->OutPutTestLog(pTestList->ATCommond,false);
	//					return FALSE;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			Log.Format("Check Type:  Fuzzy Matching");
	//			lpCPIN_TESTDlg->OutPutDebugLog(Log);

	//			if (ReturnValue.Find(TargetValue)!=-1 )
	//			{
	//				lpCPIN_TESTDlg->OutPutDebugLog("CHECK RETURN VALUE SUCCESS");
	//				lpCPIN_TESTDlg->OutPutTestLog(pTestList->ATCommond,true);
	//				return TRUE;
	//			}
	//			else
	//			{
	//				if(i==2)
	//				{
	//					lpCPIN_TESTDlg->OutPutDebugLog("CHECK RETURN VALUE FAIL");
	//					lpCPIN_TESTDlg->OutPutTestLog(pTestList->ATCommond,false);
	//					return FALSE;
	//				}
	//			}
	//		}
	//	}
	//	Sleep(500);
	//}

	return TRUE;
}

bool MYWORK::Test_CheckFTMMode(CString &sStrDebug)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------CheckFTMMode TEST---------------------------");
	unsigned char MobileMode;//mode
	CString ERRCODE;
	if (!m_Qualcomm.GET_FTM_ONLINE_MODE(&MobileMode,ERRCODE))
	{
		lpCPIN_TESTDlg->OutPutDebugLog(ERRCODE);
		goto FAIL;
	}
	sStrDebug.Format("MODE:%s",(MobileMode==CHECK_MODE_ONLINE)?"ONLINE":"FTM");
	if(MobileMode!=CHECK_MODE_ONLINE)
	{
		//检查手机是否在FTM模式，若不是则切换
		lpCPIN_TESTDlg->OutPutDebugLog("Check Online mode Fail...and Change ");	
		if (FALSE ==m_Qualcomm.CHKChangeFTM(FALSE)) return FALSE;
		//goto FAIL; //直接切换不再报错
	}
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return true;
FAIL:
	lpCPIN_TESTDlg->ShowInfor(sStrDebug);
	return FALSE;
}

void MYWORK::DecToBsStr(CString sStrDec, CString &sBs, int iBit) {
	UINT iValue = 0;
	char b[33] = { 0 };
	char bs[33] = { 0 };
	iValue = atoi(sStrDec);
	ltoa(iValue, b, 2);
    if (iBit == 2) sprintf_s(bs, "%02s", b);
	if (iBit == 3) sprintf_s(bs, "%03s", b);
    if (iBit == 4) sprintf_s(bs, "%04s", b);
	if (iBit == 5) sprintf_s(bs, "%05s", b);
	if (iBit == 8) sprintf_s(bs, "%08s", b);
	if (iBit == 32) sprintf_s(bs, "%032s", b);
	sBs.Format("%s", bs);
}

void MYWORK::BsToDecStr(CString sStrBs, CString &sDec) {
	UINT iValue = 0;
	char *e;
	char bs[33] = { 0 };
	int i = 0;
	strcpy(bs, sStrBs);
	iValue = strtol(bs, &e, 2);
	sDec.Format("%d", iValue);
}

void MYWORK::HexToBsStr(CString sStrH, CString &sBs, int iBit) {
	const char *cBin = "";
	UINT iValue = 0;
	char b[33] = { 0 };
	char bs[33] = { 0 };
	char *stopstring;
	char cHbs[33] = { 0 };
	cBin = (LPSTR)(LPCTSTR)sStrH;
	iValue = strtoul(cBin, &stopstring, 16); //把const char *cBin按16进制转为10无符号长整型进制
	//ltoa(iValue,b,2);
	_ultoa_s(iValue, b, 2);
	if (iBit == 2) sprintf_s(cHbs, "%02s", b);
	if (iBit == 3) sprintf_s(cHbs, "%03s", b);
	if (iBit == 4) sprintf_s(cHbs, "%04s", b);
	if (iBit == 5) sprintf_s(cHbs, "%05s", b);
	if (iBit == 7) sprintf_s(cHbs, "%07s", b);
	if (iBit == 8) sprintf_s(cHbs, "%08s", b);
	if (iBit == 9) sprintf_s(cHbs, "%09s", b);
	else if (iBit == 14)
	{
		sprintf_s(cHbs, "%014s", b);
	}
	else if (iBit == 15)
	{
		sprintf_s(cHbs, "%015s", b);
	}
	else if (iBit == 16)
	{
		sprintf_s(cHbs, "%016s", b);
	}
	else if (iBit == 20)
	{
		sprintf_s(cHbs, "%020s", b);
	}
	else if (iBit == 32)
	{
		sprintf_s(cHbs, "%032s", b);
	}
	sBs.Format("%s", cHbs);
}

void MYWORK::BsToHexStr(CString sHDefaultB, CString &sHDefaultH, int iLth)
{
	char *stopstring;
	const char *cBin = (LPSTR)(LPCTSTR)sHDefaultB;
	UINT iValue;
	iValue = strtoul(cBin, &stopstring, 2); //转换成无符号的长整型数(unsigned long),一定要是无符号的
	char c[36];
	sprintf_s(c, "%x", iValue);
	CString  str(c);
	if (1 == iLth)
	{
		sHDefaultH.Format("%s", str);
		sHDefaultH.MakeUpper();
	}
	if (2 == iLth)
	{
		sHDefaultH.Format("%02s", str);
		sHDefaultH.MakeUpper();
	}
	if (4 == iLth)
	{
		sHDefaultH.Format("%04s", str);
		sHDefaultH.MakeUpper();
	}
	if (5 == iLth)
	{
		sHDefaultH.Format("%05s", str);
		sHDefaultH.MakeUpper();
	}
	if (8 == iLth)
	{
		sHDefaultH.Format("%08s", str);
		sHDefaultH.MakeUpper();
	}
}

bool MYWORK::GetKeyValue(CString sValue, CString sStartValue, CString sEndValue, CString &sKeyValue) {
	try {
		int index1 = 0, index2 = 0;
		CString sTemp = "";
		index1 = sValue.Find(sStartValue) + sStartValue.GetLength();
		sTemp = sValue.Right(sValue.GetLength() - index1).Trim();
		index2 = sTemp.Find(sEndValue);
		sKeyValue = sTemp.Left(index2).Trim();
	}
	catch (...) {
		return false;
	}
	return true;
}

bool MYWORK::SplitKeyValue(CString sValue, CStringArray &caKeyValue) {
	try {
		CStringArray ca;
		CString sTemp = "", sReadBuffer="";
		int index1 = 0;
		sTemp = sValue.Right(sValue.GetLength() - 1).Trim();
		sReadBuffer = sTemp.Left(sTemp.GetLength() - 1);
		Tools_splitStr(sReadBuffer, ",", ca);
		for (int i = 0; i < ca.GetCount(); i++) {
			sTemp.Format("%f", atof(ca[i]));
			caKeyValue.Add(sTemp);
		}
	}
	catch (...) {
		return false;
	}
	return true;
}

bool MYWORK::SplitKeyValueSQ(CString sValue, CStringArray &caKeyValue) {
	try {
		CStringArray caSQ;
		CStringArray ca[3];
		Tools_splitStr(sValue, "  ", caSQ);
		for (int i = 0; i < caSQ.GetCount(); i++) {
			CString sReadBuffer = caSQ.GetAt(i);
			SplitKeyValue(sReadBuffer, ca[i]);
			caKeyValue.Append(ca[i]);
		}
	}
	catch (...) {
		return false;
	}
	return true;
}

bool MYWORK::ReadAndWriteJson(HANDLEPARAM stu_HandleParam, CStringArray &caAccBias, CStringArray &caAccMatrix, CStringArray &caGyroBias, CStringArray &caGyroMatrix) {
	try {
		SplitKeyValue(stu_HandleParam.saccbias, caAccBias);
		for (int i = 0; i < caAccBias.GetCount(); i++) lpCPIN_TESTDlg->OutPutDebugLog("caAccBias:" + caAccBias.GetAt(i));
		SplitKeyValueSQ(stu_HandleParam.saccmatrix, caAccMatrix);
		for (int i = 0; i < caAccMatrix.GetCount(); i++) lpCPIN_TESTDlg->OutPutDebugLog("caAccMatrix:" + caAccMatrix.GetAt(i));
		SplitKeyValue(stu_HandleParam.sgyrobias, caGyroBias);
		for (int i = 0; i < caGyroBias.GetCount(); i++) lpCPIN_TESTDlg->OutPutDebugLog("caGyroBias:" + caGyroBias.GetAt(i));
		SplitKeyValueSQ(stu_HandleParam.sgyromatrix, caGyroMatrix);
		for (int i = 0; i < caGyroMatrix.GetCount(); i++) lpCPIN_TESTDlg->OutPutDebugLog("caGyroMatrix:" + caGyroMatrix.GetAt(i));
	}
	catch (...) {
		return false;
	}
	return true;
}

bool MYWORK::TEST_CHECK_IMU(CString &sStrDebug, CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------IMU TEST---------------------------");
	CString sATcmd = "test pcba TEST_IMU", sKeyValue = "OK";
	CStringArray ValueList;
	int iAccel_x = 0, iAccel_y=0, iAccel_z=0,iGyro_x=0, iGyro_y = 0, iGyro_z = 0;
	int index1 = 0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	for (int i = 0; i < 3; i++) {
		if (sReceivedata.Find(sKeyValue) == -1) {
			if (i == 2) goto FAIL;
			continue;
		}
		else {
			if (!Tools_splitStr(sReceivedata, " ", ValueList)) {
				lpCPIN_TESTDlg->OutPutDebugLog("Tools_splitStr ERROR DATA");
				goto FAIL;
			}
			for (int i=0;i< ValueList.GetSize();i++) {
				if (ValueList.GetAt(i) == "accel_x") iAccel_x = atoi(ValueList.GetAt(i + 1));
				if (ValueList.GetAt(i) == "accel_y") iAccel_y = atoi(ValueList.GetAt(i + 1));
				if (ValueList.GetAt(i) == "accel_z") iAccel_z = atoi(ValueList.GetAt(i + 1));
				if (ValueList.GetAt(i) == "gyro_x") iGyro_x = atoi(ValueList.GetAt(i + 1));
				if (ValueList.GetAt(i) == "gyro_y") iGyro_y = atoi(ValueList.GetAt(i + 1));
				if (ValueList.GetAt(i) == "gyro_z") iGyro_z = atoi(ValueList.GetAt(i + 1));
			}
			sStrDebug.Format("accel_x:%d,accel_y:%d,accel_z:%d,gyro_x:%d,gyro_y:%d,gyro_z:%d", iAccel_x, iAccel_y, iAccel_z, iGyro_x, iGyro_y, iGyro_z);
			sATcmd = "get_imu_model", sKeyValue = "IMU";
			if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
			if (-1!=sReceivedata.Find("42686")) {
				if (abs(iAccel_x) > 200 && abs(iAccel_x) < 65335) {
					sStrDebug.Format("iAccel_x:%d is out of range", iAccel_x);
					return false;
				}
				if (abs(iAccel_y) > 200 && abs(iAccel_y) < 65335) {
					sStrDebug.Format("iAccel_y:%d is out of range", iAccel_y);
					return false;
				}
				if (iAccel_z < -1255 || iAccel_z > -855) {
					sStrDebug.Format("iAccel_z:%d is out of range", iAccel_z);
					return false;
				}
				if (abs(iGyro_x) > 200 && abs(iGyro_x) < 65335) {
					sStrDebug.Format("iGyro_x:%d is out of range", iGyro_x);
					return false;
				}
				if (abs(iGyro_y) > 200 && abs(iGyro_y) < 65335) {
					sStrDebug.Format("iGyro_y:%d is out of range", iGyro_y);
					return false;
				}
				if (abs(iGyro_z) > 200 && abs(iGyro_z) < 65335) {
					sStrDebug.Format("iGyro_z:%d is out of range", iGyro_z);
					return false;
				}
				int iMUsqrt = sqrt((float)(iGyro_x*iGyro_x + iGyro_y * iGyro_y + iGyro_z * iGyro_z));
				if (iMUsqrt > m_TestItem.STI_iMU.rangeMax) {
					sStrDebug.Format("iMUsqrt:%d is out of range", iMUsqrt);
					return false;
				}
			}
			else {
				if (abs(iAccel_x) > 200 && abs(iAccel_x) < 65335) {
					sStrDebug.Format("iAccel_x:%d is out of range", iAccel_x);
					return false;
				}
				if (abs(iAccel_y) > 200 && abs(iAccel_y) < 65335) {
					sStrDebug.Format("iAccel_y:%d is out of range", iAccel_y);
					return false;
				}
				if (iAccel_z < -2255 || iAccel_z > -1855) {
					sStrDebug.Format("iAccel_z:%d is out of range", iAccel_z);
					return false;
				}
				if (abs(iGyro_x) > 200 && abs(iGyro_x) < 65335) {
					sStrDebug.Format("iGyro_x:%d is out of range", iGyro_x);
					return false;
				}
				if (abs(iGyro_y) > 200 && abs(iGyro_y) < 65335) {
					sStrDebug.Format("iGyro_y:%d is out of range", iGyro_y);
					return false;
				}
				if (abs(iGyro_z) > 200 && abs(iGyro_z) < 65335) {
					sStrDebug.Format("iGyro_z:%d is out of range", iGyro_z);
					return false;
				}
			}
			return true;
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_JoystickYADC(CString &sStrDebug, CString &sReceiveData)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------Joystick Y ADC TEST---------------------------");
	CString sATcmd = "test pcba TEST_JOYSTICK_Y";
	CString sKeyValue = "TEST_JOYSTICK_Y";
	CString sTemp = "";
	int index1 = 0, index2 = 0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceiveData, sKeyValue, 3, sStrDebug)) goto FAIL;
	for (int i = 0; i < 3; i++) {
		if (sReceiveData.Find(sKeyValue) == -1) {
			if (i == 2) goto FAIL;
			continue;
		}
		else {
			index1 = sReceiveData.Find("value") + 5;
			sTemp = sReceiveData.Right(sReceiveData.GetLength() - index1).Trim();
			index2 = sTemp.Find("mv");
			sStrDebug = sTemp.Left(index2);
			if ((atoi(sStrDebug) > m_TestItem.STI_joystickYADC.rangeMin) && (atoi(sStrDebug) < m_TestItem.STI_joystickYADC.rangeMax)) return true;
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_JoystickXADC(CString &sStrDebug, CString &sReceiveData)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------Joystick X ADC TEST---------------------------");
	CString sATcmd = "test pcba TEST_JOYSTICK_X";
	CString sKeyValue = "TEST_JOYSTICK_X";
	CString sTemp = "";
	int index1 = 0, index2 = 0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceiveData, sKeyValue, 3, sStrDebug)) goto FAIL;
	for (int i = 0; i < 3; i++) {
		if (sReceiveData.Find(sKeyValue) == -1) {
			if (i == 2) goto FAIL;
			continue;
		}
		else {
			index1 = sReceiveData.Find("value") + 5;
			sTemp = sReceiveData.Right(sReceiveData.GetLength() - index1).Trim();
			index2 = sTemp.Find("mv");
			sStrDebug = sTemp.Left(index2);
			if ((atoi(sStrDebug) > m_TestItem.STI_joystickXADC.rangeMin) && (atoi(sStrDebug) < m_TestItem.STI_joystickXADC.rangeMax)) return true;
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_CHECK_TriggerADC(CString &sStrDebug, CString &sReceiveData)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------Trgger ADC TEST---------------------------");
	CString sATcmd = "test pcba TEST_TRIGGER";
	CString sKeyValue = "TEST_TRIGGER";
	CString sTemp = "";
	int index1 = 0, index2 = 0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceiveData, sKeyValue, 3, sStrDebug)) goto FAIL;
	for (int i = 0; i < 3; i++) {
		if (sReceiveData.Find(sKeyValue) == -1) {
			if (i == 2) goto FAIL;
			continue;
		}
		else {
			index1 = sReceiveData.Find("value") + 5;
			sTemp = sReceiveData.Right(sReceiveData.GetLength() - index1).Trim();
			index2 = sTemp.Find("mv");
			sStrDebug = sTemp.Left(index2);
			if ((atoi(sStrDebug) > m_TestItem.STI_joystickXADC.rangeMin) && (atoi(sStrDebug) < m_TestItem.STI_joystickXADC.rangeMax)) return true;
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_KEY(CString &sStrDebug, CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------KEY TEST---------------------------");
	lpCPIN_TESTDlg->ShowInfor("按键测试, 等待按键触发...");
	m_MessageDialog.nTESTID = KEYTEST;
	if (m_MessageDialog.DoModal() != 1) goto FAIL;
	sStrDebug = "按键测试PASS";
	return true;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_TOUCH(CString &sStrDebug, CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------TOUCH TEST---------------------------");
	lpCPIN_TESTDlg->ShowInfor("触控测试, 等待按键触摸...");
	m_MessageDialog.nTESTID = TOUCHTEST;
	if (m_MessageDialog.DoModal() != 1) goto FAIL;
	sStrDebug = "触控测试PASS";
	return true;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_Vibrate(CString &sStrDebug, CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------TEST_Vibrate ---------------------------");
	lpCPIN_TESTDlg->ShowInfor("测试震动中, 请观察震动状况...");
RETEST:
	for (int i = 0; i <= 3; i++) {
		CString sATcmdOpen = "test pcba TEST_VIBRATOR", sKeyValue = "OK";
		if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmdOpen, sReceivedata, sKeyValue, 10, sStrDebug)) goto FAIL;
		Sleep(500);
	}
	g_sMessageBoxTilte = "马达是否可以通过测试？";
	UINT nRet = m_MessageBoxDlg.DoModal();
	if (IDRETRY == nRet) goto RETEST;
	if (IDNO == nRet) {
		sStrDebug = "判定马达未通过测试！";
		goto FAIL;
	}
	sStrDebug = sReceivedata;
	return true;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

bool MYWORK::TEST_LED(CString &sStrDebug, CString &sReceivedata)
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------LED TEST---------------------------");
	lpCPIN_TESTDlg->ShowInfor("测试三色LED灯中, 请观察三色LED灯状况...");
	CString sTurnOff[3] = { "test pcba TEST_LED_RED 0","test pcba TEST_LED_GREEN 0","test pcba TEST_LED_BLUE 0" };
	CString sTurnOn[3] = { "test pcba TEST_LED_RED 1","test pcba TEST_LED_GREEN 1","test pcba TEST_LED_BLUE 1" };
	CString sKeyValue = "OK";
	CString sATcmd = "";
RETEST:
	for (int i=0;i<3;i++) {
		if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sTurnOff[i], sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	}
	for (int i = 0; i < 3; i++) {
		sATcmd = "test pcba TEST_LED_RED 1";
		if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
		sATcmd = "test pcba TEST_LED_RED 0";
		if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
		sATcmd = "test pcba TEST_LED_GREEN 1";
		if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
		sATcmd = "test pcba TEST_LED_GREEN 0";
		if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
		sATcmd = "test pcba TEST_LED_BLUE 1";
		if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
		sATcmd = "test pcba TEST_LED_BLUE 0";
		if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	}
	g_sMessageBoxTilte = "LED三色灯是否可以通过测试？";
	UINT nRet = m_MessageBoxDlg.DoModal();
	if (IDRETRY == nRet) goto RETEST;
	if (IDNO == nRet) {
		sStrDebug = "判定LED未通过测试！";
		goto FAIL;
	}
	sStrDebug = sReceivedata;
	return true;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sReceivedata);
	return FALSE;
}

bool MYWORK::TEST_INFRARED(CString &sStrDebug, CString &sReceivedata) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------LED TEST---------------------------");
	lpCPIN_TESTDlg->ShowInfor("测试红外灯中, 请观察红外灯状况...");
	//SetTimer(30,1000,NULL);
	CString sATcmdOpen = "test pcba TEST_INFRARED 0", sKeyValue = "OK";
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmdOpen, sReceivedata, sKeyValue, 10, sStrDebug)) goto FAIL;
	Sleep(500);
RETEST:
	for (int i = 0; i <= 2; i++) {
		CString sATcmdOpen = "test pcba TEST_INFRARED 1", sKeyValue = "OK";
		if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmdOpen, sReceivedata, sKeyValue, 10, sStrDebug)) goto FAIL;
		Sleep(500);
		CString sATcmdClose = "test pcba TEST_INFRARED 2";
		if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmdClose, sReceivedata, sKeyValue, 10, sStrDebug)) goto FAIL;
		Sleep(500);
	}
	g_sMessageBoxTilte = "红外灯是否可以通过测试？";
	UINT nRet = m_MessageBoxDlg.DoModal();
	if (IDRETRY == nRet) goto RETEST;
	if (IDNO == nRet) {
		sStrDebug = "判定红外灯未通过测试！";
		goto FAIL;
	}
	sStrDebug = sReceivedata;
	return true;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sReceivedata);
	return FALSE;
}

BOOL MYWORK::TEST_CHECK_YVRFLAG(int iStation,int iType) {
	CString Commond = "", ReceiveData = "", sStrDebug = "";
	int index1 = 0, index2 = 0;
	CString sFlag = "";
	lpCPIN_TESTDlg->OutPutDebugLog("CHECK TEST FLAG...");
	if (HDL_YVR==iType) {
		if (!HANDLE_Read_COMFLAG(sFlag)) {
			lpCPIN_TESTDlg->OutPutDebugLog("HANDLE READ TEST FLAG FAIL");
			return false;
		}
	}
	if (HDM_YVR == iType) {
		if (!HDM_Read_ADBFLAG(sFlag)) {
			lpCPIN_TESTDlg->OutPutDebugLog("HDM READ TEST FLAG FAIL");
			return false;
		}
	}
	TCHAR NV_Data[512] = { 0 };
	CString sTemp = "";
	memcpy(NV_Data, sFlag, sFlag.GetLength());
	for (int i = 0; i < iStation; i++) {
		if (NV_Data[(sFlag.GetLength() - i)] == '0') {
			sStrDebug.Format("STATION %d IS NO PASS",i);
			lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
			return FALSE;
		}
	}
	return true;
}

BOOL MYWORK::HANDLE_Read_COMFLAG(CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HANDLE READ COMFLAG TEST---------------------------");
	CString sATcmd = "config read CFG_TEST_FLAG", sKeyValue = "CFG_TEST_FLAG", sReceivedata = "";
	CString sReadComFlag = "";
	int index1 = 0, index2 = 0;
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	for (int i = 0; i < 3; i++) {
		if (sReceivedata.Find(sKeyValue) == -1) {
			if (i == 2) goto FAIL;
			continue;
		}
		else {
			index1 = sReceivedata.Find("0x") + 2;
			sStrDebug = sReceivedata.Right(sReceivedata.GetLength() - index1).Trim();
			TCHAR NV_Data[512] = { 0 };
			CString sTemp = "";
			memcpy(NV_Data, sStrDebug, sStrDebug.GetLength());
			for (int i = 0; i < sStrDebug.GetLength(); i++) {
				if (((NV_Data[i] >= '0' && NV_Data[i] <= '9') || (NV_Data[i] >= 'a' && NV_Data[i] <= 'f') || (NV_Data[i] >= 'A' && NV_Data[i] <= 'F'))) {
					sTemp = sTemp + NV_Data[i];
				}
				else {
					break;
				}
			}
			HexToBsStr(sTemp, sReadComFlag, 32);
			sStrDebug = sReadComFlag;
			return true;
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HANDLE_Write_COMFLAG(int iStation) {
	CString Commond = "", sStrDebug = "";
	CString sFlag = "",sNewFlag="", sFlagHex="";
	CString sTemp = "";
	CString sATcmd = "", sKeyValue = "", sReceivedata = "";
	int index1 = 0, index2 = 0;
	char cFlag[128] = { NULL };
	lpCPIN_TESTDlg->OutPutDebugLog("WRITE TEST FLAG...");
	if (!HANDLE_Read_COMFLAG(sFlag)) {
		lpCPIN_TESTDlg->OutPutDebugLog("READ TEST FLAG FAIL");
		return false;
	}
	else {
		for (int i=0;i<sFlag.GetLength();i++) {
			if (i == iStation) {
				sTemp = "1" + sTemp;
				continue;
			}
			sTemp =sFlag.GetAt(sFlag.GetLength()-i)+ sTemp;
		}
		sNewFlag = sTemp;
	}
	BsToHexStr(sNewFlag, sStrDebug,8);
	sFlagHex.Format("0x%s", sStrDebug);
	sATcmd.Format("config write CFG_TEST_FLAG %s", sFlagHex);
	sKeyValue = "success";
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	return true;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Read_ADBFLAG(CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM READ ADBFLAG TEST---------------------------");
	CString sResult = "", sFlag = "";
	//if (!m_Qualcomm.m_cmd("adb root", sResult)) {
	//	sStrDebug = sResult;
	//	goto FAIL;
	//}
	//else lpCPIN_TESTDlg->OutPutDebugLog(sResult);
	if (!m_Qualcomm.m_cmd("adb shell cat /backup/TestFlags.txt", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		sResult=sResult.Trim();
		sStrDebug.Format("Read TestFlags.txt is %s, success", sResult);
		lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
		if (!CheckWordType(sResult, 1)) {//都是数字
			sStrDebug = "CheckWordType IS ERROR";
		}
		else {
			DecToBsStr(sResult, sStrDebug, 32);
			return TRUE;
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Write_ADBFLAG(int iStation) {
	CString sResult = "", sStrDebug = "";
	CString sFlag = "", sNewFlag = "", sFlagDec = "";
	CString sTemp = "",sADBCMD="";
	int index1 = 0, index2 = 0;
	char cFlag[128] = { NULL };
	lpCPIN_TESTDlg->OutPutDebugLog("WRITE TEST FLAG...");
	if (!HDM_Read_ADBFLAG(sFlag)) {
		lpCPIN_TESTDlg->OutPutDebugLog("HDM READ TEST FLAG FAIL");
		return false;
	}
	else {
		for (int i = 0; i < sFlag.GetLength(); i++) {
			if (i == iStation) {
				sTemp = "1" + sTemp;
				continue;
			}
			sTemp = sFlag.GetAt(sFlag.GetLength() - i) + sTemp;//取字串右边值
		}
		sNewFlag = sTemp;
	}
	BsToDecStr(sNewFlag, sFlagDec);
	sADBCMD.Format("adb shell \"echo %s > /backup/TestFlags.txt\"", sFlagDec);
	if (!m_Qualcomm.m_cmd((char*)(LPCTSTR)sADBCMD,sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		if (!m_Qualcomm.m_cmd("adb shell cat /backup/TestFlags.txt", sResult)) {
			sStrDebug = sResult;
			goto FAIL;
		}
		else {
			if (sFlagDec == sResult.Trim()) {
				sStrDebug.Format("Write TestFlags.txt is %s, success", sResult);
				lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
				return TRUE;
			}
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Get_WAKEUP(CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Get_WAKEUP TEST---------------------------");
	CString sResult = "";
	if (!m_Qualcomm.m_cmd("adb shell mcupowertool wakeup", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		lpCPIN_TESTDlg->OutPutDebugLog(sResult);
		if (-1 == sResult.Find("mcu wakeup success")) {
			sStrDebug = "mcupowertool wakeup FAIL";
			goto FAIL;
		}
		else {
			Sleep(2000);//等待2秒，否则会影响后面的动作
			return TRUE;
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Read_SWVersion(CString &sSW,CString &sLocked) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Read_SWVersion TEST---------------------------");
	CString sResult = "",sTemp="";
	if (!m_Qualcomm.m_cmd("adb shell getprop ro.build.display.id", sResult)) {
		sSW = sResult;
		goto FAIL;
	}
	else {
		sTemp.Format("ReadSWVersion:%s", sResult);
		lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
		if (sResult.Trim() == m_TestItem.sInterVersion) {
			sSW = sResult.Trim();
			if (!m_Qualcomm.m_cmd("adb shell getprop ro.boot.flash.locked", sResult)) {
				sLocked = sResult.Trim();
				goto FAIL;
			}
			else {
				sTemp.Format("ReadLocked:%s", sResult);
				lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
				sLocked = sResult.Trim();
				//if (sResult.Trim() == "1") {
				//	sLocked = sResult.Trim();
				//	return TRUE;
				//}
				//else {
				//	sLocked = sResult.Trim();
				//	goto FAIL;
				//}
			}
			return true;
		}
		else {
			sSW.Format("Compare NG:%s,%s", sResult.Trim(), m_TestItem.sInterVersion);
			goto FAIL;
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sSW);
	return FALSE;
}

BOOL MYWORK::HDM_Get_ROOT(CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Get_ROOT TEST---------------------------");
	CString sResult = "";
	if (!m_Qualcomm.m_cmd("adb root", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		lpCPIN_TESTDlg->OutPutDebugLog(sResult);
		if (-1 == sResult.Find("as root")) {
			sStrDebug = "adb root FAIL";
			goto FAIL;
		}
		else {
			return TRUE;
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Write_Locked(CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Write_Locked TEST---------------------------");
	CString sResult = "";
	if (!m_Qualcomm.m_cmd("adb reboot bootloader", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	for (size_t i = 0; i < 10; i++) {
		Sleep(5000);
		if (!m_Qualcomm.m_cmd("fastboot devices", sResult)) {
			sStrDebug = sResult;
		}
		if (sResult.Find("fastboot")) {
			lpCPIN_TESTDlg->OutPutDebugLog(sResult);
			break; //进入fastboot跳出
		}
		if (i == 9) goto FAIL;
	}
	if (!m_Qualcomm.m_cmd("fastboot oem device - info", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		if (!m_Qualcomm.m_cmd("fastboot flashing lock", sResult)) {
			sStrDebug = sResult;
			goto FAIL;
		}
		if (!m_Qualcomm.m_cmd("fastboot flashing lock_critical", sResult)) {
			sStrDebug = sResult;
			goto FAIL;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog(sResult);
			if (-1 == sResult.Find("OKAY")) {
				sStrDebug = "fastboot flashing lock_critical FAIL";
				goto FAIL;
			}
			else {
				return TRUE;
			}
		}
		if (!m_Qualcomm.m_cmd("fastboot oem device-info", sResult)) {
			sStrDebug = sResult;
			goto FAIL;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog(sResult);
			if (-1 == sResult.Find("OKAY")) {
				sStrDebug = "fastboot flashing lock_critical FAIL";
				goto FAIL;
			}
			else {
				return TRUE;
			}
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Get_Factroyrese(CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Get_Factroyrese TEST---------------------------");
	CString sResult = "";
	if (!m_Qualcomm.m_cmd("adb shell am broadcast -a android.yvr.action.wipe_data --include-stopped-packages", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		lpCPIN_TESTDlg->OutPutDebugLog(sResult);
		if (-1 == sResult.Find("android.yvr.action.wipe_data flg")) {
			sStrDebug = "FACTROYRESET CMD EXE FAIL";
			goto FAIL;
		}
		else {
			CString sReadBTMAC = "";
			Sleep(20000);
			sStrDebug = "CAN READ WIFI MAC FAIL";
			for (int iTimes = 0; iTimes <= 5; iTimes++) {
				if (!HDM_READ_MAC("wifi", sReadBTMAC)) {
					sStrDebug = "CAN NOT READ WIFI";
					lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
					break;
				}
				else {
					if (sReadBTMAC.GetLength() == 12) {
						if (iTimes >= 5) goto FAIL;
						Sleep(5000);
						continue;
					}
					else{
						sStrDebug = "CAN NOT READ WIFI,FACTROYRESET CMD EXE OK";
						lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
						break;
					}
				}
			}
			return TRUE;
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Get_RUNINTIME(CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Get_RUNINTIME TEST---------------------------");
	CString sResult = "";
	if (!m_Qualcomm.m_cmd("adb shell settings get system run_in_test_time", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		lpCPIN_TESTDlg->OutPutDebugLog(sResult);
		if (-1 == sResult.Find(":")) {
			sStrDebug = "HDM_Get_RUNINTIME FAIL";
			goto FAIL;
		}
		else {
			CStringArray ca;
			Tools_splitStr(sResult,":", ca);
			if (3== ca.GetSize()) {
				int iTime=0,iMinutes=0;
				CString sTemp = "",sStart="",sEnd="";
				sStart= ca.GetAt(0);
				sStart = sStart.Right(sStart.GetLength() - 4).Trim();
				sStart = sStart.Left(sStart.GetLength() - 3).Trim();
				sEnd = ca.GetAt(2);
				sEnd = sEnd.Right(sEnd.GetLength()-4).Trim();
				sEnd = sEnd.Left(sEnd.GetLength() - 3).Trim();
				iTime = atoi(sEnd) - atoi(sStart);
				iMinutes = iTime / 60;
				if (iMinutes >= m_TestItem.STI_RuninTime.rangeMin)  {
					sStrDebug.Format("RUNINTIME,%d", iMinutes);
					//lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST,"RUNINTIME,"+ Tools_InttoCstr(iMinutes), true);
					return true;
				}
				else {
					sStrDebug.Format("RUNINTIME,%d", iMinutes);
					//lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "RUNINTIME," + Tools_InttoCstr(iMinutes), false);
					return false;
				}
			}
			else {
				sStrDebug = "Value Error";
				//lpCPIN_TESTDlg->OutPutTestLog(ENUM_FunctionTEST, "RUNINTIME,Value Error" , true);
				goto FAIL;
			}
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Get_RFAddressInfo(CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Get_RFAddressInfo TEST---------------------------");
	CString sResult = "";
	CString sPosFlag = "RF_ADDR:";
	int index1 = 0;
	if (!m_Qualcomm.m_cmd("adb wait-for-device", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	if (!m_Qualcomm.m_cmd("adb shell mcuversion", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		lpCPIN_TESTDlg->OutPutDebugLog(sResult);
		if (-1 == sResult.Find("RF_ADDR:")) {
			sStrDebug = "HDM get addr error";
			goto FAIL;
		}
		else {
			index1 = sResult.Find(sPosFlag) + sPosFlag.GetLength();
			sStrDebug = sResult.Right(sResult.GetLength() - index1).Trim();
			if (sStrDebug=="") goto FAIL;
			return TRUE;
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Get_RootPermission(CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------Get RootPermission---------------------------");
	CString sResult = "", sFlag = "", sPosFlag="attached";
	int index1 = 0;
	if (!m_Qualcomm.m_cmd("adb devices", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		lpCPIN_TESTDlg->OutPutDebugLog(sResult);
		index1 = sResult.Find(sPosFlag) + sPosFlag.GetLength();
		sStrDebug = sResult.Right(sResult.GetLength() - index1).Trim();
		if (sStrDebug.Find("device") == -1) goto FAIL;
	}
	lpCPIN_TESTDlg->OutPutDebugLog("adb wait-for-device");
	if (!m_Qualcomm.m_cmd("adb wait-for-device", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	lpCPIN_TESTDlg->OutPutDebugLog("adb shell setprop service.dev.mode 1");
	if (!m_Qualcomm.m_cmd("adb shell setprop service.dev.mode 1", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	lpCPIN_TESTDlg->OutPutDebugLog("adb wait-for-device");
	if (!m_Qualcomm.m_cmd("adb wait-for-device", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	if (!m_Qualcomm.m_cmd("adb root", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else lpCPIN_TESTDlg->OutPutDebugLog(sResult);
	if (!m_Qualcomm.m_cmd("adb reboot", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else lpCPIN_TESTDlg->OutPutDebugLog(sResult);
	Sleep(7000);
	lpCPIN_TESTDlg->OutPutDebugLog("adb wait-for-device");
	if (!m_Qualcomm.m_cmd("adb wait-for-device", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	Sleep(15000);
	lpCPIN_TESTDlg->OutPutDebugLog("adb shell setprop service.dev.mode 1");
	if (!m_Qualcomm.m_cmd("adb shell setprop service.dev.mode 1", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	lpCPIN_TESTDlg->OutPutDebugLog("adb wait-for-device");
	if (!m_Qualcomm.m_cmd("adb wait-for-device", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	if (!m_Qualcomm.m_cmd("adb root", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		lpCPIN_TESTDlg->OutPutDebugLog(sResult);
	}
	return TRUE;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Get_ReleasePermission(CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Get_RleasePermission---------------------------");
	CString sResult = "", sFlag = "", sPosFlag = "attached";
	int index1 = 0;
	lpCPIN_TESTDlg->OutPutDebugLog("adb shell setprop service.dev.mode 1");
	if (!m_Qualcomm.m_cmd("adb shell setprop service.dev.mode 1", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else lpCPIN_TESTDlg->OutPutDebugLog(sResult);
	Sleep(5000);
	return TRUE;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Read_CameraNum(CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Read_CameraNum---------------------------");
	CString sResult = "", sFlag = "",sTemp="";
	int index1 = 0,iCameraNum=0;
	lpCPIN_TESTDlg->OutPutDebugLog("adb shell \"dmesg | grep slot\"");
	if (!m_Qualcomm.m_cmd("adb shell \"dmesg | grep slot\"", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	else {
		lpCPIN_TESTDlg->OutPutDebugLog(sResult);
		for (int i=0;i<4;i++)	{
			sFlag.Format(",slot:%d,",i);
			if (-1!= sResult.Find(sFlag)) {
				iCameraNum++;
				sTemp.Format("FIND OK,%s", sFlag);
				lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
			}
			else {
				sTemp.Format("NOT FIND,%s", sFlag);
				lpCPIN_TESTDlg->OutPutDebugLog(sTemp);
			}
		}
		if (iCameraNum==4) {
			return TRUE;
		}
		else {
			sStrDebug.Format("%d", iCameraNum);
			goto FAIL;
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HANDLE_Write_HostRFAddress(CString sHMDRFAddress,int iPhonePort) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------WRITE HostRFAddress TEST---------------------------");
	CString sATcmd = "", sKeyValue = "write CFG_RF_ADDR_1 success", sReceivedata = "", sStrDebug="";
	CString sReadComFlag = "";
	int index1 = 0, index2 = 0;
	sATcmd.Format("config write CFG_RF_ADDR_1 %s", sHMDRFAddress);
	if (!m_PHONE.KeySendAtCommond(iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	for (int i = 0; i < 3; i++) {
		if (sReceivedata.Find(sKeyValue) == -1) {
			if (i == 2) goto FAIL;
			continue;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog("Write HostRFaddress to Handle，success");
			return true;
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HANDLE_Write_Mode(int iModeType, int iPhonePort) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HANDLE_Write_Mode TEST---------------------------");
	CString sATcmd = "", sKeyValue = "success", sReceivedata = "", sStrDebug = "";
	CString sReadComFlag = "";
	int index1 = 0, index2 = 0;
	sATcmd.Format("config write CFG_FACTORY_MODE %d", iModeType);
	if (!m_PHONE.KeySendAtCommond(iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	for (int i = 0; i < 3; i++) {
		if (sReceivedata.Find(sKeyValue) == -1) {
			if (i == 2) goto FAIL;
			continue;
		}
		else {
			Sleep(100);
			if (!HANDLE_Read_Mode(iModeType, iPhonePort)) goto FAIL;
			if (!HANDLE_Reboot(iPhonePort, sStrDebug)) goto FAIL;
			return true;
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HANDLE_Read_Mode(int iModeType, int iPhonePort) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HANDLE_Read_Mode TEST---------------------------");
	CString sATcmd = "", sKeyValue = "CFG_FACTORY_MODE =", sReceivedata = "", sStrDebug = "",sModeType="";
	CString sReadComFlag = "";
	int index1 = 0,index2=0;
	sATcmd="config read CFG_FACTORY_MODE";
	if (!m_PHONE.KeySendAtCommond(iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	for (int i = 0; i < 3; i++) {
		if (sReceivedata.Find(sKeyValue) == -1) {
			if (i == 2) goto FAIL;
			continue;
		}
		else {
			index1 = sReceivedata.Find("=")+1;
			sStrDebug = sReceivedata.Right(sReceivedata.GetLength() - index1).Trim();
			index2 = sStrDebug.Find("\r\n");
			sModeType=sStrDebug.Left(index2);
			if (iModeType==atoi(sModeType))	{
				sStrDebug.Format("read CFG_FACTORY_MODE ,%s", sModeType);
				lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
				return true;
			}
			else {
				sStrDebug.Format("compare CFG_FACTORY_MODE fail,%s,%d", sModeType, iModeType);
				lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
				return false;
			}
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HANDLE_Read_HostRFAddress(CString &sHMDRFAddress, int iPhonePort) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------Read HandleRFAddress TEST---------------------------");
	CString sATcmd = "", sKeyValue = "CFG_RF_ADDR_1", sReceivedata = "", sStrDebug = "";
	int index1 = 0,index2=0;
	sATcmd.Format("config read CFG_RF_ADDR_1");
	if (!m_PHONE.KeySendAtCommond(iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	for (int i = 0; i < 3; i++) {
		if (sReceivedata.Find(sKeyValue) == -1) {
			if (i == 2) goto FAIL;
			continue;
		}
		else {
			index1 =sReceivedata.Find("=")+1;
			sStrDebug =sReceivedata.Right(sReceivedata.GetLength() - index1).Trim();
			index2 = sStrDebug.Find("\r\n");
			sHMDRFAddress=sStrDebug.Left(index2);
			lpCPIN_TESTDlg->OutPutDebugLog(sHMDRFAddress);
			return true;
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HANDLE_Test_RFCoupled(CString sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HANDLE_Test_RFCoupled START---------------------------");
	CString sATcmd = "rf_coupled_test start", sKeyValue = "RF coupled test", sReceivedata = "";
	CString sTransmittedPackets = "", sReTransmittedPackets = "", sReTransmittedCount = "", sFailedTransmittedPackets = "";
	CString sRSSITemp;
	CStringArray ca;
	m_TestItem.sAverageRssi = "";
	m_TestItem.sPacketLossRate = "";
	m_TestItem.sReTransRate = "";
	m_TestItem.sReAgvCount = "";
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	lpCPIN_TESTDlg->OutPutDebugLog("RF coupled test Started，success");
	Sleep(10000);
	Sleep(5000);
	sATcmd = "rf_coupled_test stop";
	sKeyValue = "RF coupled test result";
	if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	GetKeyValue(sReceivedata, "Total transmitted packets:", "\r\n", sTransmittedPackets);
	lpCPIN_TESTDlg->OutPutDebugLog("Total transmitted packets:" + sTransmittedPackets);
	GetKeyValue(sReceivedata, "Total re-transmitted packets:", "\r\n", sReTransmittedPackets);
	lpCPIN_TESTDlg->OutPutDebugLog("Total re-transmitted packets:" + sReTransmittedPackets);
	GetKeyValue(sReceivedata, "Total re-transmitted count:", "\r\n", sReTransmittedCount);
	lpCPIN_TESTDlg->OutPutDebugLog("Total re-transmitted count:" + sReTransmittedCount);
	GetKeyValue(sReceivedata, "Total failed transmitted packets:", "\r\n", sFailedTransmittedPackets);
	lpCPIN_TESTDlg->OutPutDebugLog("Total failed transmitted packets:" + sFailedTransmittedPackets);
	GetKeyValue(sReceivedata, "RSSI (min,avg,max):", "Total transmitted packets", sRSSITemp);        //min
	lpCPIN_TESTDlg->OutPutDebugLog("RSSI (min,avg,max):" + sRSSITemp);
	Tools_splitStr(sRSSITemp, ",", ca);
	if (m_TestItem.STI_averageRssi.needTest) {
		if (ca.GetAt(1) == "") return false;
		m_TestItem.sAverageRssi = ca.GetAt(1); 
		if ((atof(ca.GetAt(1)) >= m_TestItem.STI_averageRssi.rangeMin) && (atof(ca.GetAt(1)) <= m_TestItem.STI_averageRssi.rangeMax)) {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_CSQ, ca.GetAt(1), true);
		}
		else {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_CSQ, ca.GetAt(1), false);
			return false;
		}
	}
	if (m_TestItem.STI_packetLossRate.needTest) {
		if ((atoi(sTransmittedPackets) + atoi(sFailedTransmittedPackets))>0) {
			float ffailrate = atof(sFailedTransmittedPackets) / (atof(sFailedTransmittedPackets) + atof(sTransmittedPackets));
			m_TestItem.sPacketLossRate.Format("%f", ffailrate);
			if (ffailrate <= m_TestItem.STI_packetLossRate.rangeMax) {
				sStrDebug.Format("%f", ffailrate);
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_CREG, sStrDebug, true);
			}
			else {
				sStrDebug.Format("%f", ffailrate);
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_CREG, sStrDebug, false);
				return false;
			}
		}
		else {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_CREG, "NG", false);
			return false;
		}
	}
	if (m_TestItem.STI_reTransRate.needTest) {
		if (atoi(sTransmittedPackets) > 0) {
			float ffailrate = atof(sReTransmittedPackets) / atof(sTransmittedPackets);
			m_TestItem.sReTransRate.Format("%f", ffailrate);
			if (ffailrate <= m_TestItem.STI_reTransRate.rangeMax) {
				sStrDebug.Format("%f", ffailrate);
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_CGREG, sStrDebug, true);
			}
			else {
				sStrDebug.Format("%f", ffailrate);
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_CGREG, sStrDebug, false);
				return false;
			}
		}
		else {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_CGREG, "NG", false);
			return false;
		}
	}
	if (m_TestItem.STI_reAgvCount.needTest) {
		if (atoi(sReTransmittedPackets) > 0) {
			float ffailrate = atof(sFailedTransmittedPackets) / atof(sReTransmittedPackets);
			m_TestItem.sReAgvCount.Format("%f", ffailrate);
			if (ffailrate <= m_TestItem.STI_reAgvCount.rangeMax) {
				sStrDebug.Format("%f", ffailrate);
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_SEQ, sStrDebug, true);
			}
			else {
				sStrDebug.Format("%f", ffailrate);
				lpCPIN_TESTDlg->OutPutTestLog(ENUM_SEQ, sStrDebug, false);
				return false;
			}
		}
		else {
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_SEQ, "NG", false);
			return false;
		}
	}
	sATcmd = "rf_pair remove";
	if (!m_PHONE.SendCommondOnly(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, 3, sStrDebug)) goto FAIL;
	Sleep(2000);//等待手柄启动，否则会影响后面测试
	return true;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HANDLE_Get_Parameters(int iPhonePort, HANDLEPARAM &stu_HandleParam) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HANDLE_Get_Parameters TEST---------------------------");
	CString sATcmd = "parameters_print", sKeyValue = "Stored parameters", sReceivedata = "", sStrDebug = "";
	int index1 = 0, index2 = 0;
	if (!m_PHONE.KeySendAtCommond(iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 3, sStrDebug)) goto FAIL;
	for (int i = 0; i < 3; i++) {
		if (sReceivedata.Find(sKeyValue) == -1) {
			if (i == 2) goto FAIL;
			continue;
		}
		else {
			GetKeyValue(sReceivedata, "serial number:"," ", stu_HandleParam.sserialnumber);
			GetKeyValue(sReceivedata, "controller type:", " ", stu_HandleParam.scontrollertype);
			GetKeyValue(sReceivedata, "rf_addr_0:", " ", stu_HandleParam.srf_addr_0);
			GetKeyValue(sReceivedata, "acc bias [x y z]:", "acc matrix:", stu_HandleParam.saccbias);
			GetKeyValue(sReceivedata, "acc matrix:", "gyro bias", stu_HandleParam.saccmatrix);
			GetKeyValue(sReceivedata, "gyro bias [x y z]:", "gyro matrix", stu_HandleParam.sgyrobias);
			GetKeyValue(sReceivedata, "gyro matrix:", "uart", stu_HandleParam.sgyromatrix);
			return true;
		}
		Sleep(500);
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HANDLE_Reboot(int iPhonePort,CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HANDLE_Reboot TEST---------------------------");
	CString sATcmd = "reboot", sKeyValue = "Reb", sReceivedata = "";
	if (!m_PHONE.SendCommondOnly(iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, 1, sStrDebug)) goto FAIL;
	sStrDebug = sReceivedata;
	Sleep(3000);//等待2秒启动，否则会影响后面的动作
	return true;

FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Write_HandleParamInfo(HANDLEPARAM stu_HandleParam,CStringArray& caAccBias, CStringArray& caAccMatrix, CStringArray& caGyroBias, CStringArray& caGyroMatrix,CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Write_HandleParamInfo TEST---------------------------");
	CString sResult = "",sADBCMD="";
	Json::Value jsonInfo;
	CString sJsonFilePath = m_CONFIG.sExeFilePath + "imu_info.json";
	int index1 = 0;
	if (!m_Qualcomm.m_cmd("adb wait-for-device", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	sADBCMD.Format("CD %s", m_CONFIG.sExeFilePath);
	if (!m_Qualcomm.m_cmd((char*)(LPCTSTR)sADBCMD, sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	//if (!m_Qualcomm.m_cmd("adb pull /backup/imu_info.json", sResult)) {
	//	sStrDebug = sResult;
	//	goto FAIL;
	//}
	//else {//检查pull文件
		//lpCPIN_TESTDlg->OutPutDebugLog(sResult);
		//if (-1 == sResult.Find("1 file pulled")) {
		//	sStrDebug = "pull file error";
		//	goto FAIL;
		//}
		//else {//有文件pull出来后,分解stu_HANDLEPARAM1
			ReadAndWriteJson(stu_HandleParam, caAccBias, caAccMatrix, caGyroBias, caGyroMatrix);
			if (!PathFileExists(sJsonFilePath)) {
				sStrDebug = "Json file error";
				goto FAIL;
			}
			if (ReadJsonFile(sJsonFilePath, &jsonInfo) != false) {
				CString sLeftOrRight = "";
				if (0 == atoi(stu_HandleParam.scontrollertype)) sLeftOrRight = "left";
				else if (1 == atoi(stu_HandleParam.scontrollertype)) sLeftOrRight = "right";
				else {
					sStrDebug.Format("scontrollertype value error,%d", stu_HandleParam.scontrollertype);
					goto FAIL;
				}
				jsonInfo[sLeftOrRight]["serial_no"] = stu_HandleParam.sserialnumber.GetString();
				Json::Value Obj_acc_bias;	
				//Obj_acc_bias.clear();
				for (int i = 0; i < caAccBias.GetCount(); i++) {
					Obj_acc_bias.append(atof(caAccBias[i]));
				}
				jsonInfo[sLeftOrRight]["acc_bias"] = Obj_acc_bias;

				Json::Value Obj_acc_mat;
				for (int i = 0; i < caAccMatrix.GetCount(); i++) {
					Obj_acc_mat.append(atof(caAccMatrix[i]));
				}
				jsonInfo[sLeftOrRight]["acc_mat"] = Obj_acc_mat;

				Json::Value Obj_gyro_bias;
				for (int i = 0; i < caGyroBias.GetCount(); i++) {
					Obj_gyro_bias.append(atof(caGyroBias[i]));
				}
				jsonInfo[sLeftOrRight]["gyro_bias"] = Obj_gyro_bias;

				Json::Value Obj_gyro_mat;
				for (int i = 0; i < caGyroMatrix.GetCount(); i++) {
					Obj_gyro_mat.append(atof(caGyroMatrix[i]));
				}
				jsonInfo[sLeftOrRight]["gyro_mat"] = Obj_gyro_mat;
			}
			if (WriteJsonFile(sJsonFilePath, jsonInfo) != false) {
				if (!m_Qualcomm.m_cmd("adb push imu_info.json /backup/imu_info.json", sResult)) {
					sStrDebug = sResult;
					goto FAIL;
				}
				else {
					lpCPIN_TESTDlg->OutPutDebugLog(sResult);
					if (-1 == sResult.Find("1 file pushed")) {
						sStrDebug = "imu_info.json: push file error";
						goto FAIL;
					}
					return TRUE;
				}
			}
		//}
	//}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::WriteKeyValue(CString sType, CString sControlType, CString sParam, CStringArray*ca ) {
	CString sResult = "", sADBCMD = "", sStrDebug="";
	if (""==sParam) {
		for (int i = 0; i < ca->GetCount(); i++) {
			sADBCMD.Format("adb shell mcuconfig write %s%s%d %s", sControlType, sType,i, ca->GetAt(i));
			if (!m_Qualcomm.m_cmd((char*)(LPCTSTR)sADBCMD, sResult)) {
				sStrDebug = sResult;
				goto FAIL;
			}
			else {
				lpCPIN_TESTDlg->OutPutDebugLog(sResult);
				if (-1 == sResult.Find("success")) {
					sStrDebug.Format("adb shell mcuconfig write %s(first) FAIL!!!", sType);
					//goto FAIL;
				}
				else {
					if (!m_Qualcomm.m_cmd((char*)(LPCTSTR)sADBCMD, sResult)) {
						sStrDebug = sResult;
						goto FAIL;
					}
					else {
						lpCPIN_TESTDlg->OutPutDebugLog(sResult);
						if (-1 == sResult.Find("success")) {
							sStrDebug.Format("adb shell mcuconfig write %s(second) FAIL!!!", sType);
							goto FAIL;
						}
					}
				}
			}
		}
	}
	else {
		sADBCMD.Format("adb shell mcuconfig write %s%s %s", sControlType, sType, sParam);
		if (!m_Qualcomm.m_cmd((char*)(LPCTSTR)sADBCMD, sResult)) {
			sStrDebug = sResult;
			goto FAIL;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog(sResult);
			if (-1 == sResult.Find("success")) {
				sStrDebug.Format("adb shell mcuconfig write %s(first) FAIL!!!", sType);
				//goto FAIL;
			}
			else {
				if (!m_Qualcomm.m_cmd((char*)(LPCTSTR)sADBCMD, sResult)) {
					sStrDebug = sResult;
					goto FAIL;
				}
				else {
					lpCPIN_TESTDlg->OutPutDebugLog(sResult);
					if (-1 == sResult.Find("success")) {
						sStrDebug.Format("adb shell mcuconfig write %s(second) FAIL!!!", sType);
						goto FAIL;
					}
				}
			}
		}
	}
	return TRUE;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::CheckKeyValue(CString sType, CString sControlType, CString sParam, CStringArray*ca) {
	CString sResult = "", sADBCMD = "", sStrDebug = "",sTemp="";
	if ("" == sParam) {
		for (int i = 0; i < ca->GetCount(); i++) {
			sADBCMD.Format("adb shell mcuconfig read %s%s%d", sControlType, sType, i);
			if (!m_Qualcomm.m_cmd((char*)(LPCTSTR)sADBCMD, sResult)) {
				sStrDebug = sResult;
				goto FAIL;
			}
			else {
				lpCPIN_TESTDlg->OutPutDebugLog(sResult);
				BResult(GetKeyValue(sResult," =", "\r\n", sTemp));
				if (sTemp == ca->GetAt(i)) continue; //相同PASS
				else {
					double dTempValue = fabs(atof(sTemp) - atof(ca->GetAt(i)));
					if (dTempValue >= 0.0001) {
						sStrDebug.Format("adb shell mcuconfig read %s,compare FAIL!!! %s,%s", sADBCMD, sTemp, ca->GetAt(i));
						goto FAIL;
					}
					else { 
						continue;//误差小于0.0001也PASS
					}
				}
			}
		}
		return TRUE;
	}
	else {
		sADBCMD.Format("adb shell mcuconfig read %s%s", sControlType, sType);
		if (!m_Qualcomm.m_cmd((char*)(LPCTSTR)sADBCMD, sResult)) {
			sStrDebug = sResult;
			goto FAIL;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog(sResult);
			BResult(GetKeyValue(sResult, sType+" =", "\r\n", sTemp));
			if (sTemp == sParam) return TRUE; //相同PASS
			else {
				double dTempValue = fabs(atof(sTemp) - atof(sParam));
				if (dTempValue > 0.0001) {
					sStrDebug.Format("adb shell mcuconfig read %s,compare FAIL!!! %s,%s", sType, sTemp, sParam);
					goto FAIL;
				}
				else return TRUE; //误差小于0.0001也PASS
			}
		}
	}
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Write_HostParamInfo(HANDLEPARAM stu_HandleParam, CStringArray* caAccBias, CStringArray* caAccMatrix, CStringArray* caGyroBias, CStringArray* caGyroMatrix,CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Write_HostParamInfo TEST---------------------------");
	CString sResult = "", sADBCMD = "", sControlType="";
    CStringArray* caDummy=nullptr;
	if (0 == atoi(stu_HandleParam.scontrollertype)) sControlType = "LEFT";
	else if (1 == atoi(stu_HandleParam.scontrollertype)) sControlType = "RIGHT";
	else {
		sStrDebug.Format("scontrollertype value error,%d", stu_HandleParam.scontrollertype);
		goto FAIL;
	}
	if (!m_Qualcomm.m_cmd("adb wait-for-device", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}

	Sleep(2000);
	BResult(WriteKeyValue("_SERIAL_NUMBER", sControlType, stu_HandleParam.sserialnumber, caDummy));
	BResult(WriteKeyValue("_ACC_X_BIAS", sControlType, caAccBias->GetAt(0),caDummy));
	BResult(WriteKeyValue("_ACC_Y_BIAS", sControlType, caAccBias->GetAt(1),caDummy));
	BResult(WriteKeyValue("_ACC_Z_BIAS", sControlType, caAccBias->GetAt(2), caDummy));
	BResult(WriteKeyValue("_GYRO_X_BIAS", sControlType, caGyroBias->GetAt(0), caDummy));
	BResult(WriteKeyValue("_GYRO_Y_BIAS", sControlType, caGyroBias->GetAt(1), caDummy));
	BResult(WriteKeyValue("_GYRO_Z_BIAS", sControlType, caGyroBias->GetAt(2), caDummy));
	BResult(WriteKeyValue("_ACC_MAT_", sControlType, "", caAccMatrix));
	BResult(WriteKeyValue("_GYRO_MAT_", sControlType, "", caGyroMatrix));

	BResult(CheckKeyValue("_SERIAL_NUMBER", sControlType, stu_HandleParam.sserialnumber, caDummy));
	BResult(CheckKeyValue("_ACC_X_BIAS", sControlType, caAccBias->GetAt(0), caDummy));
	BResult(CheckKeyValue("_ACC_Y_BIAS", sControlType, caAccBias->GetAt(1), caDummy));
	BResult(CheckKeyValue("_ACC_Z_BIAS", sControlType, caAccBias->GetAt(2), caDummy));
	BResult(CheckKeyValue("_GYRO_X_BIAS", sControlType, caGyroBias->GetAt(0), caDummy));
	BResult(CheckKeyValue("_GYRO_Y_BIAS", sControlType, caGyroBias->GetAt(1), caDummy));
	BResult(CheckKeyValue("_GYRO_Z_BIAS", sControlType, caGyroBias->GetAt(2), caDummy));
	BResult(CheckKeyValue("_ACC_MAT_", sControlType, "", caAccMatrix));
	BResult(CheckKeyValue("_GYRO_MAT_",sControlType, "", caGyroMatrix));
	return TRUE;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::HDM_Read_HostParamInfo(HANDLEPARAM stu_HandleParam, CStringArray* caAccBias, CStringArray* caAccMatrix, CStringArray* caGyroBias, CStringArray* caGyroMatrix, CString &sStrDebug) {
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------HDM_Read_HostParamInfo TEST---------------------------");
	CString sResult = "", sADBCMD = "", sControlType = "";
	CStringArray* caDummy = nullptr;
	if (0 == atoi(stu_HandleParam.scontrollertype)) sControlType = "LEFT";
	else if (1 == atoi(stu_HandleParam.scontrollertype)) sControlType = "RIGHT";
	else {
		sStrDebug.Format("scontrollertype value error,%d", stu_HandleParam.scontrollertype);
		goto FAIL;
	}
	if (!m_Qualcomm.m_cmd("adb wait-for-device", sResult)) {
		sStrDebug = sResult;
		goto FAIL;
	}
	Sleep(2000);
	BResult(CheckKeyValue("_SERIAL_NUMBER", sControlType, stu_HandleParam.sserialnumber, caDummy));
	BResult(CheckKeyValue("_ACC_X_BIAS", sControlType, caAccBias->GetAt(0), caDummy));
	BResult(CheckKeyValue("_ACC_Y_BIAS", sControlType, caAccBias->GetAt(1), caDummy));
	BResult(CheckKeyValue("_ACC_Z_BIAS", sControlType, caAccBias->GetAt(2), caDummy));
	BResult(CheckKeyValue("_GYRO_X_BIAS", sControlType, caGyroBias->GetAt(0), caDummy));
	BResult(CheckKeyValue("_GYRO_Y_BIAS", sControlType, caGyroBias->GetAt(1), caDummy));
	BResult(CheckKeyValue("_GYRO_Z_BIAS", sControlType, caGyroBias->GetAt(2), caDummy));
	BResult(CheckKeyValue("_ACC_MAT_", sControlType, "", caAccMatrix));
	BResult(CheckKeyValue("_GYRO_MAT_", sControlType, "", caGyroMatrix));
	return TRUE;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	return FALSE;
}

BOOL MYWORK::PHONE_Function_Test()
{
	CString QRCODE="",Number="",DATAITEM="",Commandend ="",CommandLightstart ="",CommandLightend="",MDL="",str;
	CArray<CString,CString> DATA;
	int index1=0,index2=0;
	CString Commond="",ReceiveData="",ERRINFO="",sStrDebug="";
	CString Flag="";
	CString keyValue ="OK";
	char cFlag[128]={NULL};
	//保存测试状态
	bool Check_SEQ = false;
	bool Check_Mcu_sw_ver = false;
	bool Check_Mcu_hw_ver = false;
	bool Check_SIM = false;
	bool Check_ICCID = false;
	bool Check_CSQ = false;
	bool Check_CREG = false;
	bool Check_CGREG = false;
	bool Check_GPS = false;
	bool Check_WIFI = false;
	bool Check_BT = false;
	bool Check_Battery_voltage = false;
	bool Check_Test = false;
	bool Check_IMEI1 = false;
	bool Check_Tempter = false;
	bool Check_HUMIDTY = false;	
	bool Check_EX_TEMPTER= false;	
	bool Check_EX_HUMIDTY=false;
	bool Check_Gsensor=false;
	bool Check_WDOG=false;
	bool Check_UC15Open=false;
	bool Check_LED=false;
	bool Check_UC15_ver= false;
	bool Check_SPIFLASH=false;
	bool Check_FTM=false;
	bool Check_joystickXADC = false;
	bool Check_joystickYADC = false;
	bool Check_triggerADC = false;
	bool Check_iMU= false;
	if (1) {	//测试LED////////////////////////
		if (false == TEST_Vibrate(sStrDebug, ReceiveData))
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Check_Vibrate TEST FAIL");
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_Vibrate, sStrDebug, false);
			return false;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog("Check_Vibrate :" + sStrDebug);
			lpCPIN_TESTDlg->OutPutDebugLog("Check_Vibrate TEST PASS");
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_Vibrate, sStrDebug, true);
			Check_LED = true;
		}
		if (false == TEST_KEY(sStrDebug, ReceiveData)) {
			lpCPIN_TESTDlg->OutPutDebugLog("Check_KEY TEST FAIL");
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_KEY, sStrDebug, false);
			return false;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog("Check_KEY :" + sStrDebug);
			lpCPIN_TESTDlg->OutPutDebugLog("Check_KEY TEST PASS");
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_KEY, sStrDebug, true);
			Check_LED = true;
		}
		if (false == TEST_LED(sStrDebug, ReceiveData))
		{
			lpCPIN_TESTDlg->OutPutDebugLog("Check_LED TEST FAIL");
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_LED, sStrDebug, false);
			return false;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog("Check_LED :" + sStrDebug);
			lpCPIN_TESTDlg->OutPutDebugLog("Check_LED TEST PASS");
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_LED, sStrDebug, true);
			Check_LED = true;
		}

		if (false == TEST_INFRARED(sStrDebug, ReceiveData)) {
			lpCPIN_TESTDlg->OutPutDebugLog("Check_INFRARED TEST FAIL");
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_INFRARED, sStrDebug, false);
			return false;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog("Check_INFRARED :" + sStrDebug);
			lpCPIN_TESTDlg->OutPutDebugLog("Check_INFRARED TEST PASS");
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_INFRARED, sStrDebug, true);
			Check_LED = true;
		}

		if (false == TEST_TOUCH(sStrDebug, ReceiveData)) {
			lpCPIN_TESTDlg->OutPutDebugLog("Check_TOUCH TEST FAIL");
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_TOUCH, sStrDebug, false);
			return false;
		}
		else {
			lpCPIN_TESTDlg->OutPutDebugLog("Check_TOUCH :" + sStrDebug);
			lpCPIN_TESTDlg->OutPutDebugLog("Check_TOUCH TEST PASS");
			lpCPIN_TESTDlg->OutPutTestLog(ENUM_TOUCH, sStrDebug, true);
			Check_LED = true;
		}
	}	
	//自动测试开始
	int iMAXcOUNT=3;
	for (int i = 0;i<iMAXcOUNT;i++) {
		if (m_TestItem.STI_joystickXADC.needTest) {
			if (Check_joystickXADC ==false) {
				if (i == (iMAXcOUNT-1) && Check_joystickXADC ==false) {
					lpCPIN_TESTDlg->OutPutDebugLog("JoystickXADC TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_JoystickX_ADC,"ERROR",false);
					return false;
				}
				if (false == TEST_CHECK_JoystickXADC(sStrDebug,ReceiveData)) {
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_JoystickX_ADC,sStrDebug,false);
					continue;
				}
				else {
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_JoystickX_ADC,sStrDebug,true);
					Check_joystickXADC = true;
				}
			}
		}	
		if (m_TestItem.STI_joystickYADC.needTest) {
			if (Check_joystickYADC == false) {
				if (i == (iMAXcOUNT - 1) && Check_joystickYADC == false) {
					lpCPIN_TESTDlg->OutPutDebugLog("JoystickYADC TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_JoystickY_ADC, "ERROR", false);
					return false;
				}
				if (false == TEST_CHECK_JoystickYADC(sStrDebug, ReceiveData)) {
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_JoystickY_ADC, sStrDebug, false);
					continue;
				}
				else {
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_JoystickY_ADC, sStrDebug, true);
					Check_joystickYADC = true;
				}
			}
		}
		if (m_TestItem.STI_triggerADC.needTest) {
			if (Check_triggerADC == false) {
				if (i == (iMAXcOUNT - 1) && Check_triggerADC == false) {
					lpCPIN_TESTDlg->OutPutDebugLog("Trigger ADC TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_Trigger_ADC, "ERROR", false);
					return false;
				}
				if (false == TEST_CHECK_TriggerADC(sStrDebug, ReceiveData)) {
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_Trigger_ADC, sStrDebug, false);
					continue;
				}
				else {
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_Trigger_ADC, sStrDebug, true);
					Check_triggerADC = true;
				}
			}
		}
		if (m_TestItem.STI_iMU.needTest) {
			if (Check_iMU == false) {
				if (i == (iMAXcOUNT - 1) && Check_iMU == false) {
					lpCPIN_TESTDlg->OutPutDebugLog("IMU TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_IMU, "ERROR", false);
					return false;
				}
				if (false == TEST_CHECK_IMU(sStrDebug, ReceiveData)) {
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_IMU, sStrDebug, false);
					continue;
				}
				else {
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_IMU, sStrDebug, true);
					Check_iMU = true;
					return true;
				}
			}
		}
		if (m_TestItem.bCheckVersion)
		{//////////读取SEQ//////////
			if (Check_SEQ==false)
			{
				if (i==(iMAXcOUNT-1) && Check_SEQ==false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("SEQ :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("SEQ TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_SEQ,sStrDebug,false);
					return false;
				}
				if (false == TEST_SEQ(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("SEQ :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("SEQ TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_SEQ,sStrDebug,true);
					Check_SEQ=true;
				}				
			}
			//////////写和读操作是否成功//////////
			if (Check_SPIFLASH==false)
			{
				if (i==(iMAXcOUNT-1) && Check_SPIFLASH==false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("SPIFLASH :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("SPIFLASH TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_SPIFLASH,sStrDebug,false);
					return false;
				}
				if (false == TEST_SPIFLASH(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("SPIFLASH :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("SPIFLASH TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_SPIFLASH,sStrDebug,true);
					Check_SPIFLASH=true;
				}				
			}
			/////////mcu软件版本////////
			if (Check_Mcu_sw_ver==false)
			{
				if (i==(iMAXcOUNT-1) && Check_Mcu_sw_ver==false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("Mcu_sw_ver :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("Mcu_sw_ver TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_MCU_VERSION,sStrDebug,false);
					return false;
				}
				if (false== TEST_MCU_Version(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				} 
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("Mcu_sw_ver :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("Mcu_sw_ver TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_MCU_VERSION,sStrDebug,true);
					Check_Mcu_sw_ver = true;
				}			
			}
		}
		/////////WIFI TEST//////////////
		if (m_TestItem.bwifi)
		{
			if (Check_WIFI==false)
			{
				if (i == (iMAXcOUNT-1) && Check_WIFI ==false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("WIFI TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_WIFI,sStrDebug,false);
					return false;
				}
				if (false == TEST_CHECK_WIFI(sStrDebug,ReceiveData))
				{
					Sleep(4000);
					continue;
				}
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("WIFI TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_WIFI,sStrDebug,true);
					Check_WIFI = true;
				}
			}
		}
		/////////BT TEST//////////////
		if (m_TestItem.bBT)
		{
			if (Check_BT==false)
			{
				if (i == (iMAXcOUNT-1) && Check_BT ==false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("BT TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_BT,sStrDebug,false);
					return false;
				}
				if (false == TEST_CHECK_BT(sStrDebug,ReceiveData))
				{
					Sleep(4000);
					continue;
				}
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("BT TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_BT,sStrDebug,true);
					Check_BT = true;
				}
			}
		}
		///////////battery value//////////////
		if (m_TestItem.Battery_VMain)
		{
			if (Check_Battery_voltage==false)
			{
				if (i == (iMAXcOUNT-1) && Check_Battery_voltage ==false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("Battery_Voltage :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("Battery_Voltage TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_Battery_Voltage,sStrDebug,false);
					return false;
				}
				if (false == TEST_CHECK_Battery(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("Battery_Voltage :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("Battery_Voltage TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_Battery_Voltage,sStrDebug,true);
					Check_Battery_voltage = true;
				}
			}
		}
		///////Check TEMPTER//////////
		if (m_TestItem.bTempter)
		{
			if (Check_Tempter==false)
			{
				if (i == (iMAXcOUNT-1) && Check_Tempter == false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("Tempter :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("Tempter TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_TEMPTER,sStrDebug,false);
					return false;
				}
				else if (false==TEST_CHECK_Tempter(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					Check_Tempter = true;
					lpCPIN_TESTDlg->OutPutDebugLog("Tempter :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("Tempter TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_TEMPTER,sStrDebug,true);
				}				
			}		
			if (Check_HUMIDTY==false)
			{
				if (i == (iMAXcOUNT-1) && Check_HUMIDTY == false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("HUMIDTY :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("HUMIDTY TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_HUMIDTY,sStrDebug,false);
					return false;
				}
				else if (false==TEST_CHECK_HUMIDTY(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					Check_HUMIDTY = true;
					lpCPIN_TESTDlg->OutPutDebugLog("HUMIDTY :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("HUMIDTY TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_HUMIDTY,sStrDebug,true);
				}				
			}	
		}
		if (m_TestItem.bEXTempter)
		{
			if (Check_EX_TEMPTER==false)
			{
				if (i == (iMAXcOUNT-1) && Check_EX_TEMPTER == false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("EX_TEMPTER :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("EX_TEMPTER TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_EX_TEMPTER,sStrDebug,false);
					return false;
				}
				else if (false==TEST_CHECK_EXTEMPTER(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					Check_EX_TEMPTER = true;
					lpCPIN_TESTDlg->OutPutDebugLog("EX_TEMPTER :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("EX_TEMPTER TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_EX_TEMPTER,sStrDebug,true);
				}				
			}	
			if (Check_EX_HUMIDTY==false)
			{
				if (i == (iMAXcOUNT-1) && Check_EX_HUMIDTY == false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("EX_HUMIDTY :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("EX_HUMIDTY TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_EX_HUMIDTY,sStrDebug,false);
					return false;
				}
				else if (false==TEST_CHECK_EXHUMIDTY(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					Check_EX_HUMIDTY = true;
					lpCPIN_TESTDlg->OutPutDebugLog("EX_HUMIDTY :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("EX_HUMIDTY TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_EX_HUMIDTY,sStrDebug,true);
				}				
			}	
		}
		//Gsensor   
		if (m_TestItem.bGsensor)
		{
			if (Check_Gsensor==false)
			{
				if (i == (iMAXcOUNT-1) && Check_Gsensor ==false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("Gsensor :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("Gsensor TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_Gsensor,sStrDebug,false);
					return false;
				}
				if (false == TEST_CHECK_Gsensor(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("Gsensor :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("Gsensor TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_Gsensor,sStrDebug,true);
					Check_Gsensor = true;
				}
			}
		}
		//WDOG   
		if (m_TestItem.bWDOG)
		{
			if (Check_WDOG==false)
			{
				if (i == (iMAXcOUNT-1) && Check_WDOG ==false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("WDOG :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("WDOG TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_ALARM,sStrDebug,false);
					return false;
				}
				if (false == TEST_CHECK_WDOG(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("WDOG :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("WDOG TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_ALARM,sStrDebug,true);
					Check_WDOG = true;
				}
			}
		}
		/////////GPS TEST/////////////
		if (m_TestItem.bgps)
		{
			if (Check_GPS==false)
			{
				if (i == (iMAXcOUNT-1) && Check_GPS == false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("GPS TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_GPS,sStrDebug,false);
					return false;
				}
				if (false == TEST_CHECK_GPS(sStrDebug,ReceiveData))
				{
					Sleep(4000);
					continue;
				}
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("GPS TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_GPS,ReceiveData,true);
					Check_GPS = true;
				}
			}
		}
		////////////UC15OPEN//////////////////////////////
		if (1)
		{
			if (Check_UC15Open==false)
			{
				if (i==(iMAXcOUNT-1) && Check_UC15Open==false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("UC15Open :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("UC15Open TEST FAIL");
					//lpCPIN_TESTDlg->OutPutTestLog(ENUM_UC15OPEN,sStrDebug,false);
					return false;
				}
				if (false==TEST_UC15Open(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					if (sStrDebug.Find("重启")!=-1) return false;
					continue;
				} 
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("UC15Open :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("UC15Open TEST PASS");
					//lpCPIN_TESTDlg->OutPutTestLog(ENUM_UC15OPEN,sStrDebug,true);
					Check_UC15Open = true;
				}	
			}
		}
		//////////////////////
#ifdef _HM04 //Check FTMMODE
		//if (stu_TestPlanSet.bCheckFTMMode)
		if (1)
		{
			if (Check_FTM==false)
			{
				if (i==(iMAXcOUNT-1) && Check_FTM==false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("FTM :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("FTM TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_UC15OPEN,sStrDebug,false);
					return false;
				}
				//加载高通库连接手机
				m_Qualcomm.InitLibrary(); 
				if(!m_Qualcomm.ConnectToPhone()) 
				{
					lpCPIN_TESTDlg->OutPutDebugLog("ConnectToPhone FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_UC15OPEN,"ConnectToPhone",false);
					return false; 
				}
				if (FALSE == Test_CheckFTMMode(sStrDebug)) 
				{
					lpCPIN_TESTDlg->OutPutDebugLog("FTM TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_UC15OPEN,sStrDebug,false);
					return false;
				}
				else 
				{
					lpCPIN_TESTDlg->OutPutDebugLog("FTM TEST PASS");
					Check_FTM = true;
				}
			}
		}
#endif
		///////SIM TEST/////////
		if (m_TestItem.bSIM)
		{
			if (Check_SIM==false)
			{
				if (i == (iMAXcOUNT-1) && Check_SIM == false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("SIM TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_SIM,sStrDebug,false);
					return false;
				}
				if (false==TEST_CHECK_SIM(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("SIM TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_SIM,sStrDebug,true);
					Check_SIM = true;
				}			
			}
		}
		///////ICCID READ/////////
		if (m_TestItem.bICCID)
		{
			if (Check_ICCID==false)
			{
				if (i == (iMAXcOUNT-1) && Check_ICCID == false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("ICCID TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_ICCID,sStrDebug,false);
					return false;
				}
				if (false==TEST_CHECK_ICCID(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("ICCID :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("ICCID TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_ICCID,sStrDebug,true);
					Check_ICCID = true;
				}			
			}
		}
		/////////CSQ TEST/////////
		if (m_TestItem.bCSQ)
		{
			if (Check_CSQ==false)
			{
				if (i == (iMAXcOUNT-1) && Check_CSQ == false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("CSQ TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_CSQ,sStrDebug,false);
					return false;
				}
				if (false==TEST_CHECK_CSQ(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				} 
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("CSQ TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_CSQ,sStrDebug,true);
					Check_CSQ = true;
				}
			}			
		}
		////////CREG TEST/////////
		if (m_TestItem.bcreg)
		{
			Sleep(2000);
			if (Check_CREG==false)
			{
				if (i == (iMAXcOUNT-1) && Check_CREG ==false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("CREG TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_CREG,sStrDebug,false);
					return false;
				}
				if (false == TEST_CHECK_CREG(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("CREG TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_CREG,sStrDebug,true);
					Check_CREG = true;
				}

			}
		}
		///////CGREG TEST///////////////
		if (m_TestItem.bcgreg)
		{
			if (Check_CGREG==false)
			{
				if (i == (iMAXcOUNT-1) && Check_CGREG == false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("CGREG TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_CGREG,sStrDebug,false);
					return false;
				}
				if (false == TEST_CHECK_CGREG(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("CGREG TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_CGREG,sStrDebug,true);
					Check_CGREG = true;
				}

			}
		}
		///////Check IMEI//////////
		if (m_TestItem.bFlag)
		{
			if (Check_IMEI1==false)
			{
				if (i == (iMAXcOUNT-1) && Check_IMEI1 == false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("IMEI :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("IMEI TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_IMEI,sStrDebug,false);
					return false;
				}
				else if (false==TEST_CHECK_IMEI(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				}
				else
				{
					Check_IMEI1 = true;
					lpCPIN_TESTDlg->OutPutDebugLog("IMEI :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("IMEI TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_IMEI,sStrDebug,true);
				}				
			}			
		}
		////////////UC15版本///////
		if (1)
		{		
			if (Check_UC15_ver== false)
			{
				if (i==(iMAXcOUNT-1) && Check_UC15_ver==false)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("UC15_ver :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("UC15_ver TEST FAIL");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_UC15_VERSION,sStrDebug,false);
					return false;
				}
				if (false==TEST_UC15_Version(sStrDebug,ReceiveData))
				{
					Sleep(1000);
					continue;
				} 
				else
				{
					lpCPIN_TESTDlg->OutPutDebugLog("UC15_ver :"+sStrDebug);
					lpCPIN_TESTDlg->OutPutDebugLog("UC15_ver TEST PASS");
					lpCPIN_TESTDlg->OutPutTestLog(ENUM_UC15_VERSION,sStrDebug,true);
					Check_UC15_ver = true;
					return true;
					//break;
				}				
			}
		}
	}
}





