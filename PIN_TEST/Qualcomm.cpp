#include "StdAfx.h"
#include "Qualcomm.h"
#include "MMI_TESTDlg.h"

extern CPIN_TESTDlg*	lpCPIN_TESTDlg;

HANDLE g_hQMSLDevice;

CQualcomm::CQualcomm(void)
{
}

CQualcomm::~CQualcomm(void)
{
}

void  CQualcomm::InitLibrary()
{
	char LibraryVersion[255]={0};
	CString sLibraryVersion;

	lpCPIN_TESTDlg->OutPutDebugLog("QUALCOMM=> InitLibrary");
	QLIB_GetLibraryVersion(LibraryVersion);//获取信息如QLIB V6.0.52,QPST
	sLibraryVersion.Format("QUALCOMM=> QLIB Version %s",LibraryVersion);
	lpCPIN_TESTDlg->OutPutDebugLog(sLibraryVersion);
	Sleep(500);
}

bool CQualcomm::ConnectToPhone()
{
	lpCPIN_TESTDlg->OutPutDebugLog("QUALCOMM=> ConnnectToPhone");
	for (int i=0;i<30;i++)
	{
		g_hQMSLDevice = QLIB_ConnectServerWithWait(QLIB_COM_AUTO_DETECT, 1000);
		if (g_hQMSLDevice==NULL)
			continue;
		else
			break;
	}
	if (g_hQMSLDevice==NULL)
	{
		lpCPIN_TESTDlg->ShowInfor("连接手机失败!");
		lpCPIN_TESTDlg->OutPutDebugLog("QUALCOMM=> ConnnectToPhone fail");
		return false;
	}
	if (!QLIB_IsPhoneConnected(g_hQMSLDevice))
	{
		lpCPIN_TESTDlg->ShowInfor("连接手机失败!");
		lpCPIN_TESTDlg->OutPutDebugLog("QUALCOMM=> Check Phone Connnect fail");
		return false;		
	}
	if (!QLIB_ConfigureTimeOut(g_hQMSLDevice, QMSL_Timeout_Runtime_Mode_Switching, 5000))
	{
		lpCPIN_TESTDlg->ShowInfor("配置手机超时时间出错!");
		lpCPIN_TESTDlg->OutPutDebugLog("QUALCOMM=> Set Phone TimeOut Err");
		return false;
	}
	Sleep(3000);//等待口稳定
	return true;
}

void CQualcomm::DisconnectPhone()
{
	lpCPIN_TESTDlg->OutPutDebugLog("QUALCOMM=> DisconnectPhone");

	try
	{
		if (g_hQMSLDevice != NULL)
		{
			if (!QLIB_IsPhoneConnected(g_hQMSLDevice))
			{
				lpCPIN_TESTDlg->OutPutDebugLog("QUALCOMM=> QLIB_IsPhoneConnected (Disconnect)");
			}
			else
			{
				QLIB_DisconnectServer(g_hQMSLDevice);
			}
		}
		QLIB_DisconnectAllServers();

	}

	catch (...)
	{
		lpCPIN_TESTDlg->OutPutDebugLog("QUALCOMM=> DisconnectPhone Crash");
	}
}

bool CQualcomm::ReadFlag(int NV_num,unsigned char &cFlagResult)  // 根据数组确认check哪些标志位，根据check结果返回Result。
{
	CString sStrdebug;
	unsigned short iStatu = 0;
	unsigned char ItemDate[128]= {'\0'};
	if (!QLIB_DIAG_NV_READ_F(g_hQMSLDevice,2499,ItemDate,128,&iStatu))
	{
		lpCPIN_TESTDlg->OutPutDebugLog("Qualcomm==>QLIB_DIAG_NV_READ_F Flag Fail");
		return false;
	}
	cFlagResult=ItemDate[NV_num];
	sStrdebug.Format("%c",cFlagResult);
	lpCPIN_TESTDlg->OutPutDebugLog(sStrdebug);
	return true;
}

bool CQualcomm::WriteFlag(int NV_num,bool iResult)
{
	unsigned char ItemDate[128]= {'\0'};
	unsigned short iStatu = 0;
	if( !QLIB_DIAG_NV_READ_F(g_hQMSLDevice, 2499, ItemDate, 128, &iStatu) )	
	{
		lpCPIN_TESTDlg->OutPutDebugLog("Qualcomm==>QLIB_DIAG_NV_WRITE_F Flag Fail");
		return false;
	}
	if (!iResult)
	{
		ItemDate[NV_num] = 70;
	}
	else
	{
		ItemDate[NV_num]= 80;
	}
	if (!QLIB_DIAG_NV_WRITE_F(g_hQMSLDevice,2499,ItemDate,128,&iStatu))
	{
		lpCPIN_TESTDlg->OutPutDebugLog("Qualcomm==>QLIB_DIAG_NV_WRITE_F Flag Fail");
		return false;
	}
	if(!EFS(g_hQMSLDevice))
	{
		return false;
		lpCPIN_TESTDlg->OutPutDebugLog("同步失败");
	}
	return true;
}

BOOL CQualcomm::EFS(HANDLE hResourceContext)
{
	lpCPIN_TESTDlg->OutPutDebugLog("Qualcomm==>同步");
	unsigned char ExecutionStatus=255,RetVal=0;
	BYTE ssss[10]="/";

	RetVal=QLIB_EFS2_SyncWithWait(hResourceContext,ssss, 10000,&ExecutionStatus);

	if((RetVal==0) || ExecutionStatus)
	{
		lpCPIN_TESTDlg->OutPutDebugLog("Qualcomm==>同步失败！");
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

bool CQualcomm::ReadSN(CString &sOutPut)
{
	const unsigned short iNVID = 2497;
	const unsigned short iNVID2 = 6860;
	unsigned short iStatus1=NV_DONE_S,iStatus2=NV_DONE_S;
	BYTE sTempSN1[128]={0},snCopy[30] = {0};
	char SN1[128]={0};
	CString sSN1="",sSN2="";
	CString sDebuglog="";
	sOutPut="";
	lpCPIN_TESTDlg->OutPutDebugLog("QUALCOMM=> Read SN");
	if( QLIB_DIAG_NV_READ_F(g_hQMSLDevice, iNVID, sTempSN1, 50, &iStatus1) == TRUE )	
	{
		if(iStatus1==NV_DONE_S)
		{
			strcpy_s(SN1,(char*)sTempSN1);//防止QCN中SN过长造成数组溢出将[30]改成SN1[128]&sTempSN1[128]
			sOutPut.Format("%s",SN1);
			sDebuglog.Format("QUALCOMM=> NV2497 : %s",sOutPut);
			lpCPIN_TESTDlg->OutPutDebugLog(sDebuglog);
		}
		else
		{
			sDebuglog.Format("QUALCOMM=> Read NV2497 fail, Status: %d",iStatus1);
			lpCPIN_TESTDlg->OutPutDebugLog(sDebuglog);
			return false;
		}
	}
	else
	{
		lpCPIN_TESTDlg->OutPutDebugLog("QUALCOMM=> Read NV2497 fail");
		return false;
	}

	return true;
}

bool CQualcomm::CHKChangeFTM(bool bFTM)//bFTM=1:切换到FTM;; bFTM=0:切换到online；
{
	unsigned char bret = FALSE;
	unsigned char _iExecutionStatus = 0;
	unsigned short _iExecutionStatus2 = 0;
	char * _pPath = "/";
	unsigned char pWrite[3] = {0};
	unsigned long nPhoneMode = 0; 
	CString strDebug;
	//QLIB_GetPhoneOperatingMode 
	bret = QLIB_IsPhoneConnected(g_hQMSLDevice);
	if (FALSE == bret)
	{
		lpCPIN_TESTDlg->OutPutDebugLog("QLIB_IsPhoneConnected Fail: Phone disconnected.");
		return FALSE;
	}
	if (bFTM)//切换到FTM,,,修改这个函数，增加查询手机模式的函数
	{
		for (int i = 0; i < 10; i++)
		{
			if (i == 9)
			{
				lpCPIN_TESTDlg->OutPutDebugLog("Switch Phone TO FTM Mode Fail");
				return FALSE;
			}
			Sleep(2000);
			bret = QLIB_DIAG_CONTROL_F(g_hQMSLDevice, MODE_FTM_F);
			if (FALSE == bret)
			{
				Sleep(2000);
				bret = QLIB_DIAG_CONTROL_F(g_hQMSLDevice, MODE_FTM_F);
				if (FALSE == bret)
				{
					lpCPIN_TESTDlg->OutPutDebugLog("QLIB_DIAG_CONTROL_F(MODE_FTM_F)Fail");
					continue;;
				}
			}
			else
			{
				break;
			}
		}
	}
	else
	{   //param pWrite = (1 = FTM mode), (0 = ONLINE mode)
		bret = QLIB_DIAG_NV_WRITE_F(g_hQMSLDevice, 453, pWrite, 1, &_iExecutionStatus2);
		if (FALSE == bret)
		{
			lpCPIN_TESTDlg->OutPutDebugLog("QLIB_DIAG_NV_WRITE_F(453)Fail");
			return FALSE;
		}
	}
	return TRUE;
}

bool CQualcomm::GET_FTM_ONLINE_MODE(unsigned char *Ret_Mode,CString &ERRCODE)
{
	unsigned char RET=0;
	TCHAR BTData[20] = {0};
	unsigned char NV_Data=0;
	const unsigned short iNVID = 453;
	unsigned short iStatus = NV_DONE_S;

	RET=QLIB_DIAG_NV_READ_F(g_hQMSLDevice,iNVID, Ret_Mode, 1, &iStatus);

	if(!RET || iStatus!=0)
	{
		switch(iStatus)
		{
		case NV_BUSY_S :       ERRCODE="NV_READ_Fail: Request is queued";break;
		case NV_BADCMD_S :     ERRCODE="NV_READ_Fail: Unrecognizable command field";break;
		case NV_FULL_S :       ERRCODE="NV_READ_Fail: The NVM is full";break;
		case NV_FAIL_S :       ERRCODE="NV_READ_Fail: Command failed, reason other than NVM was full";break;
		case NV_NOTACTIVE_S :  ERRCODE="NV_READ_Fail: Variable was not active";break;
		case NV_BADPARM_S :    ERRCODE="NV_READ_Fail: Bad parameter in command block";break;
		case NV_READONLY_S :   ERRCODE="NV_READ_Fail: Parameter is write-protected and thus read only";break;
		case NV_BADTG_S :      ERRCODE="NV_READ_Fail: Item not valid for Target";break;
		case NV_NOMEM_S :      ERRCODE="NV_READ_Fail: free memory exhausted";break;
		case NV_NOTALLOC_S :   ERRCODE="NV_READ_Fail: address is not a valid allocation";break;
		}
		return false;
	}

	return true;
}

bool CQualcomm::GetGPSRx(double &dGpsCno)
{
	CString str1, str2, strDebug;
	unsigned char bRet = FALSE;	
	unsigned char piStatus  = 0;
	long EngineRevision = 0;
	unsigned char   Supportted_Contellation_Mask[256] = {0};
	unsigned char   Actual_Constellsation_Mask[256] ={0} ;
	unsigned char  iCollectMode = 0;
	unsigned short  iCaptureSize_kSamples = 0;
	unsigned char  iNumberOfIntegrations = 0;
	double  dCNoDBHz = 0; 
	int  iFreqInHz = 0;
	bRet = QLIB_ConfigureTimeOut(g_hQMSLDevice, 18, 5000);
	if (FALSE == bRet)
	{
		lpCPIN_TESTDlg->OutPutDebugLog("QLIB_ConfigureTimeOut(18,5000): Fail.");
		return FALSE;
	}

	bRet = QLIB_IsPhoneConnected(g_hQMSLDevice);
	if (FALSE == bRet)
	{
		strDebug.Format("QLIB_IsPhoneConnected: Fail.");
		return FALSE;
	}
	//sends the CGPS GEN8 engine GPS Engine Query RF Dev Capability DM Command Config DM command to the phone.
	bRet = QLIB_GPS_GEN8_Get_GNSS_Engine_Revision(g_hQMSLDevice, &EngineRevision);
	strDebug.Format("QLIB_GPS_GEN8_Get_GNSS_Engine_Revision(EngineRevision=0x%x).",EngineRevision);
	if (FALSE == bRet)
	{
	   goto FAIL;
	}
	lpCPIN_TESTDlg->OutPutDebugLog(strDebug);

	bRet = QLIB_IsPhoneConnected(g_hQMSLDevice);
	if (FALSE == bRet)
	{
		strDebug.Format("QLIB_IsPhoneConnected: Fail.");
	    goto FAIL;
	}
	// Bit 0 GPS,Bit 1  Glonass,Bit 2  Beidou,	Bit 4  Galileo
	bRet = QLIB_GPS_GEN8_GNSS_NAV_SET_CONFIG(g_hQMSLDevice, 0, Supportted_Contellation_Mask, Actual_Constellsation_Mask);
	strDebug.Format("QLIB_GPS_GEN8_GNSS_NAV_SET_CONFIG: Supportted_Contellation_Mask=%s,Actual_Constellsation_Mask=%s",Supportted_Contellation_Mask,Actual_Constellsation_Mask);
	if (FALSE == bRet)
	{		
      goto FAIL;
	}
	lpCPIN_TESTDlg->OutPutDebugLog(strDebug);

	bRet = QLIB_IsPhoneConnected(g_hQMSLDevice);
	if (FALSE == bRet)
	{
		strDebug.Format("QLIB_IsPhoneConnected: Fail.");
		goto FAIL;
	}

	bRet = QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(g_hQMSLDevice, 0, &piStatus);
	strDebug.Format("QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait: piStatus=%s.",piStatus);
	if (FALSE == bRet || piStatus ==1 )
	{
		goto FAIL;
	}
	lpCPIN_TESTDlg->OutPutDebugLog(strDebug);

	bRet = QLIB_IsPhoneConnected(g_hQMSLDevice);
	if (FALSE == bRet)
	{
		strDebug.Format("QLIB_IsPhoneConnected: Fail.");
		goto FAIL;
	}

	iCollectMode = COLLECT_MODE_WBIQ_BP1_2MHZ;//GPS primary Rx
	iCaptureSize_kSamples = 8;
	iNumberOfIntegrations = 10;
	// -120dBm and -100dBm.  -110dBm is suggested.
	bRet = QLIB_DIAG_GEN8_GET_EMBEDDED_CALCULATED_CTON(g_hQMSLDevice, iCollectMode, iCaptureSize_kSamples,
		iNumberOfIntegrations, &dCNoDBHz, &iFreqInHz);
	if (FALSE == bRet)
	{
		strDebug.Format("QLIB_IsPhoneConnected: Fail.");
		goto FAIL;
	}

	dGpsCno= dCNoDBHz;//db


	bRet = QLIB_IsPhoneConnected(g_hQMSLDevice);
	if (FALSE == bRet)
	{
		strDebug.Format("QLIB_IsPhoneConnected: Fail.");
		goto FAIL;
	}

	//退出GPS测试模式
	bRet = QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(g_hQMSLDevice, 1, &piStatus);
	if (FALSE == bRet || piStatus ==1 )
	{
		strDebug.Format("QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(1): Fail.");
		goto FAIL;
	}

	return TRUE;

FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(strDebug);
	return FALSE;
}

bool CQualcomm::DiagCommandCamera(CString &sOutPut)
{
	CString temp="";
	sOutPut="";
	int i=0;
	ULONG32 TimeoutMs = 1000;
	unsigned char ResBufferArray[512]={0};
	unsigned char ReqBufferArray[]={75,201,170,255,58,01};//4B C9 AA FF 3A 01
	short rSize = 512;
	short *ResponseSize=&rSize;
	short iQuestSize=sizeof(ReqBufferArray)/sizeof(unsigned char);
	if (!QLIB_SendSync(g_hQMSLDevice,iQuestSize,ReqBufferArray,ResponseSize,ResBufferArray,TimeoutMs))
	{
		sOutPut = "Error:QLIB_SendSync fail";
		lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
		return false;
	}
	else
	{	
		if (ResBufferArray[6]!=01)
		{
			for (i=0;i<7;i++) {temp.Format("%02X",ResBufferArray[i]);
			sOutPut=sOutPut+temp+" "; }	
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			return false;
		}
		else
		{
			for (i=0;i<7;i++) {temp.Format("%02X",ResBufferArray[i]);
			sOutPut=sOutPut+temp+" "; }		
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);

			return true;
		}
	}
}

bool CQualcomm::DiagCommandSend(unsigned char cCMD[],int iCMDSize,CString &sOutPut)
{
	CString temp="";
	int i=0;
	ULONG32 TimeoutMs = 1000;
	unsigned char ResBufferArray[512]={0};
	unsigned char ReqBufferArray[6]={0};

	sOutPut="";
	for (i=0; i<iCMDSize;i++ )
	{
		ReqBufferArray[i]=cCMD[i];
	}
	short rSize = 512;
	short *ResponseSize=&rSize;
	short iQuestSize=sizeof(ReqBufferArray)/sizeof(unsigned char);
	if (!QLIB_SendSync(g_hQMSLDevice,iQuestSize,ReqBufferArray,ResponseSize,ResBufferArray,TimeoutMs))
	{
		sOutPut = "Error:QLIB_SendSync fail";
		lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
		return false;
	}
	else
	{	
		if (ResBufferArray[6]!=01)
		{
			for (i=0;i<7;i++) {temp.Format("%02X",ResBufferArray[i]);
			sOutPut=sOutPut+temp+" "; }	
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			return false;
		}
		else
		{
			for (i=0;i<7;i++) {temp.Format("%02X",ResBufferArray[i]);
			sOutPut=sOutPut+temp+" "; }		
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			return true;
		}
	}
}

bool CQualcomm::m_cmd(char * pCmd, CString& strErrorInfo)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	CString strCmd;
	//加上"cmd /k"是为了能执行类似dir的命令
	//cmd /c dir 是执行完dir命令后关闭命令窗口。
	//cmd /k dir 是执行完dir命令后不关闭命令窗口
	strCmd.Format("cmd /c %s", pCmd);

	//创建命名管道
	if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
		strErrorInfo = "m_cmd: Error On CreatePipe()!";
		return FALSE;
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;//数据输出用的文件句柄
	si.hStdOutput = hWrite;//数据输出用的文件句柄。子进程的输出hStdOutput，写入管道hWrite，反之子进程的输入，要去读取管道
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	if (!CreateProcess(NULL, strCmd.GetBuffer(strCmd.GetLength())//执行cmd命令,并在命名中管道中写入cmd命令返回的串
		,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) 
	{
		strErrorInfo = "m_cmd: Error On CreateProcess()!";
		return FALSE;
	}
	/*if(WaitForSingleObject(pi.hProcess, INFINITE)==WAIT_FAILED)//这里决定是否要等整个命令执行完,如ping等耗时较多的命令.
	{
		//MessageBox("Failed!");
		return FALSE;
	}*/
	CloseHandle(hWrite);
	
	char buffer[2048] ={0} ;
	DWORD bytesRead = 0;
	CString strResult="";
	Sleep(500);

	while(1)
	{
		memset(buffer, 0, 2048);
		if(ReadFile(hRead, buffer, 1024, &bytesRead, NULL) != NULL)//从命名管道中读取数据
		{
			strResult += buffer;

			//  返回值 WAIT_OBJECT_0 表示进程 已经结束。所以这里直接break。
			if(WaitForSingleObject(pi.hProcess, 3000) == WAIT_OBJECT_0)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	CloseHandle(hRead);
	if (strResult!="")	{
		strErrorInfo = strResult;//保存结果
		strResult.MakeUpper();//转化为大写，方便后面find
	}
	//if (-1 != strResult.Find("EXCEPTION") || -1 != strResult.Find("FAIL") || -1 != strResult.Find("ERROR") || -1 != strResult.Find("NO SUCH FILE"))
	if (-1 != strResult.Find("FAIL") || -1 != strResult.Find("NO SUCH FILE") || -1 != strResult.Find("ERROR")) {
		return false;
	}
	return true;
}

bool CQualcomm::DiagCommandGetMCU(CString &sOutPut)
{
	CString temp="";
	int i=0;
	ULONG32 TimeoutMs = 1000;
	unsigned char ResBufferArray[512]={0};
	unsigned char ReqBufferArray[]={75,201,170,255,52,01};//4B C9 AA FF 34 01
	short rSize = 512;
	short *ResponseSize=&rSize;
	short iQuestSize=sizeof(ReqBufferArray)/sizeof(unsigned char);
	
	sOutPut="";
	if (!QLIB_SendSync(g_hQMSLDevice,iQuestSize,ReqBufferArray,ResponseSize,ResBufferArray,TimeoutMs))
	{
		sOutPut = "Error:QLIB_SendSync fail";
		lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
		return false;
	}
	else
	{		 
		if (ResBufferArray[6]!=01)
		{
			for (i=0;i<7;i++) {temp.Format("%02X",ResBufferArray[i]);
			sOutPut=sOutPut+temp+" "; }	
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			return false;
		}
		else
		{
			sOutPut.Format("%02X",ResBufferArray[7]);
			return true;
		}
		
	}
}

bool CQualcomm::DiagFANTest(CString &sOutPut)
{
	CString temp="";
	int i=0;
	ULONG32 TimeoutMs = 1000;
	unsigned char ResBufferArray[512]={0};
	unsigned char ReqBufferArray[]={75,201,170,255,60,04};//4B C9 AA FF 3C 04
	short rSize = 512;
	short *ResponseSize=&rSize;
	short iQuestSize=sizeof(ReqBufferArray)/sizeof(unsigned char);
	sOutPut="";
	if (!QLIB_SendSync(g_hQMSLDevice,iQuestSize,ReqBufferArray,ResponseSize,ResBufferArray,TimeoutMs))
	{
		sOutPut = "Error:QLIB_SendSync fail";
		lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
		return false;
	}
	else
	{		 
		if (ResBufferArray[6]!=01)
		{
			for (i=0;i<7;i++) {temp.Format("%02X",ResBufferArray[i]);
			sOutPut=sOutPut+temp+" "; }	
			lpCPIN_TESTDlg->OutPutDebugLog(sOutPut);
			return false;
		}
		else
		{
			sOutPut.Format("%s",ResBufferArray);
			sOutPut=sOutPut.Right(sOutPut.GetLength()-7).Trim();
			return true;
		}

	}
}