// ComPortEx.cpp : 实现文件
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "ComPortEx.h"
#include "MMI_TESTDlg.h"

extern CPIN_TESTDlg*	lpCPIN_TESTDlg;
// ComPortEx

IMPLEMENT_DYNAMIC(ComPortEx, CWnd)

ComPortEx::ComPortEx()
{
	m_bSyncComm=TRUE;

	m_hCommPort=NULL;

	m_hReadEvent=NULL;
	m_hStopEvent=NULL;
}

ComPortEx::~ComPortEx()
{
	if(m_hStopEvent!=NULL)
	{
		CloseHandle(m_hStopEvent);
		m_hStopEvent=NULL;
	}
}


BEGIN_MESSAGE_MAP(ComPortEx, CWnd)
END_MESSAGE_MAP()



// ComPortEx 消息处理程序


bool ComPortEx::CommInit(int comport,int baudrate)
{

	if(m_hCommPort!=NULL)
	{
		CloseHandle(m_hCommPort);
		m_hCommPort=NULL;
	}


	CString comstring;
	comstring.Format(TEXT("\\\\.\\COM%d"),comport);
	m_hCommPort = CreateFile( comstring,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		NULL,
		NULL );

	if ( m_hCommPort == INVALID_HANDLE_VALUE )
	{
		//return ERROR_OPENCOMFAIL;
		return false;
	}

	if ( SetupComm( m_hCommPort, 1024, 1024 ) != TRUE )
	{
		CommClose();
		//return ERROR_SETUPCOMFAIL;
		return false;
	}

	COMSTAT cComStat;
	DWORD dwError;
	if ( ClearCommError( m_hCommPort, &dwError, &cComStat ) != TRUE )
	{
		CommClose();
		//return ERROR_CLEARCOMERRORFAIL;
		return false;
	}

	DCB dcb;
	dcb.DCBlength=sizeof(DCB);
	if ( GetCommState( m_hCommPort, &dcb ) == FALSE )
	{
		CommClose();
		//return ERROR_GETCOMSTATEFAIL;
		return false;
	}

	dcb.BaudRate=baudrate;
	//dcb.fOutxCtsFlow=TRUE;
	dcb.fAbortOnError=TRUE;
	dcb.fTXContinueOnXoff=TRUE;
	dcb.fRtsControl=RTS_CONTROL_DISABLE;
	dcb.fDtrControl=DTR_CONTROL_ENABLE;
	dcb.fDsrSensitivity=FALSE;
	dcb.XonLim=256;
	dcb.XoffLim=256;
	dcb.ByteSize=8;
	dcb.Parity=NOPARITY;
	dcb.StopBits=ONESTOPBIT;

	if ( SetCommState( m_hCommPort, &dcb ) == FALSE )
	{
		DWORD ggg=GetLastError();
		CommClose();
		return false;
	}

	// Initialization Timeout
	COMMTIMEOUTS ctTimeout;
	ctTimeout.ReadIntervalTimeout			= 0x10;
	ctTimeout.ReadTotalTimeoutConstant		= 0x10;
	ctTimeout.ReadTotalTimeoutMultiplier	= 0x10;
	ctTimeout.WriteTotalTimeoutConstant		= 0x1388;
	ctTimeout.WriteTotalTimeoutMultiplier	= 0;

	if ( SetCommTimeouts( m_hCommPort, &ctTimeout ) == FALSE )
	{
		CommClose();
		//return ERROR_SETCOMTIMEOUTFAIL;
		return false;
	}

	if(PurgeComm(m_hCommPort,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR)==FALSE)
	{
		DWORD ddd= GetLastError();
		//return ERROR_SERIALPURGE;
		return false;
	}

	return true;
}

void ComPortEx::CommClose()
{
	if(m_hCommPort!=NULL)
	{
		CloseHandle(m_hCommPort);
		m_hCommPort=NULL;
	}
}

bool ComPortEx::ReadData(BYTE* pBuffer,DWORD iNumberBytesToRead,DWORD* iNumberBytesRead,float Timeout_s)
{
	clock_t start,stop;
	float fScanTime;
	if(pBuffer==NULL || iNumberBytesToRead<=0)
	{
		//return ERROR_BUFFERISNULL;
		return false;
	}

	if(iNumberBytesRead!=NULL)
		*iNumberBytesRead=0;

	m_olReadOverlapped.Offset		= 0;
	m_olReadOverlapped.OffsetHigh	= 0;
	m_olReadOverlapped.hEvent		= m_hReadEvent;


	COMSTAT cComStat;
	DWORD dwError;

	start=clock();//计时开始毫秒为单位
	while(1)
	{
		stop=clock();
		fScanTime=(float)(stop-start)/CLOCKS_PER_SEC;
		//CString stemp="",sTimeout_s="",stemp2="";
		//stemp.Format("fScanTime=%f",fScanTime);
		//lpCPIN_TESTDlg->OutPutDebugLog(stemp);
		//sTimeout_s.Format("Timeout_s=%f",Timeout_s);
		//lpCPIN_TESTDlg->OutPutDebugLog(sTimeout_s);
		if(fScanTime>Timeout_s)
		{
			return false;
		}
		if ( ClearCommError( m_hCommPort, &dwError, &cComStat ) != TRUE )
		{
			return false;
		}
		DWORD cbInQue;
		cbInQue=cComStat.cbInQue;//输入缓冲区有多少数据？
		/*CString stemp2="";
		stemp2.Format("cbInQue=%d",cbInQue);
		lpCPIN_TESTDlg->OutPutDebugLog(stemp2);*/
		if (cbInQue!=0 &&cbInQue!=1)
		{
			break;
		}
		Sleep(50);
	}
	OVERLAPPED* pOverlapped=(!m_bSyncComm) ? &m_olReadOverlapped : NULL;
	DWORD tttt;
	BOOL res= ReadFile( m_hCommPort,
		pBuffer,
		iNumberBytesToRead,
		&tttt,
		pOverlapped );

	if ( res == FALSE )
	{
		DWORD dwLastError = GetLastError();
		if ( dwLastError != ERROR_IO_PENDING )
		{
			//return ERROR_READCOMFAIL;
			return false;
		}
	}


	if(iNumberBytesRead!=NULL)
	{
		*iNumberBytesRead=tttt;
	}

	return true;
}

bool ComPortEx::WriteData(BYTE* pBuffer,DWORD iNumberBytesToWrite,DWORD* iNumberBytesWritten)
{

	if(pBuffer==NULL || iNumberBytesToWrite<=0)
	{
		//return ERROR_BUFFERISNULL;
		return false;
	}

	BOOL	bResult;
	DWORD	dwLastError;

	m_olWriteOverlapped.Offset		= 0;
	m_olWriteOverlapped.OffsetHigh	= 0;
	m_olWriteOverlapped.hEvent		= NULL;

	OVERLAPPED* pOverlapped= NULL;

	DWORD tttt;
	bResult = WriteFile( m_hCommPort,pBuffer,iNumberBytesToWrite,&tttt,pOverlapped );
	if ( bResult == FALSE )
	{
		dwLastError = GetLastError();
		return false;
	}

	if(iNumberBytesWritten!=NULL)
		*iNumberBytesWritten=tttt;

	return true;
}

void ComPortEx::SetReadEvent(HANDLE hEvent)
{
	m_hReadEvent=hEvent;
}

void ComPortEx::SetStopEvent(HANDLE hEvent)
{
	m_hStopEvent=hEvent;
}

bool ComPortEx::SetDTRStatus(UINT dtrstatus)
{
	DCB dcb;
	dcb.DCBlength=sizeof(DCB);
	BOOL fStatus=GetCommState( m_hCommPort, &(dcb) ) ;
	if(!fStatus)
	{
		//return ERROR_GETCOMSTATEFAIL;
		return false;
	}

	dcb.fDtrControl=dtrstatus;

	fStatus=SetCommState( m_hCommPort, &dcb ) ;

	return true;
}

bool ComPortEx::SetRTSStatus(UINT rtsstatus)
{
	DCB dcb;
	dcb.DCBlength=sizeof(DCB);
	BOOL fStatus=GetCommState( m_hCommPort, &(dcb) ) ;
	if(!fStatus)
	{
		//return ERROR_GETCOMSTATEFAIL;
		return false;
	}

	dcb.fRtsControl=rtsstatus;

	fStatus=SetCommState( m_hCommPort, &dcb ) ;

	return true;
}

bool ComPortEx::Clear()
{
	if (m_hCommPort == INVALID_HANDLE_VALUE)
	{
		//return ERROR_COMNOTOPEN;
		return false;
	}

	if  ((PurgeComm (m_hCommPort, PURGE_TXCLEAR | PURGE_RXCLEAR|PURGE_TXABORT|PURGE_RXABORT)))
	{
		return  true; 
	}
	else
		//return ERROR_SERIALPURGE;	
		return false;
}

bool ComPortEx::ComReadString(BYTE *pReadBuffer, DWORD iReadMaxLength, DWORD* piReadLength, DWORD n_Timeout)
{
	CString DebugString=TEXT("");
	if(!ReadData( pReadBuffer, iReadMaxLength, piReadLength, n_Timeout ))
	{
		return false;
	}
	pReadBuffer[iReadMaxLength]='\0';
	DebugString.Format("DEBUG: COMPORT_READ  %d : %s",m_hCommPort,pReadBuffer);
	OutputDebugString(DebugString);
	return true;
}

bool ComPortEx::ComWriteString(BYTE *pWriteBuffer,DWORD iWriteMaxLength)
{
	CString DebugString=TEXT("");
	DWORD iWriteLength;

	//iWriteMaxLength = strlen( (char*)pReadBuffer );
	if(!WriteData( pWriteBuffer,iWriteMaxLength, &iWriteLength ))
	{
		return false;
	}
	DebugString.Format("DEBUG: COMPORT_WRITE  %d : %s",m_hCommPort,pWriteBuffer);
	OutputDebugString(DebugString);
	return true;
}

bool ComPortEx::ComWriteRead_HEX(int ComPort,int baudrate, CString WriteData_HEX,CString &ReadData_HEX,CString &ERRCODE)
{
	//	写：
	
	CString DebugString=TEXT("");
	BYTE strSendBuffer[MAX_PATH];
	BYTE strBuffer[MAX_PATH];
	DWORD iWriteMaxLength=0;
	

	memset(strSendBuffer, '\0', sizeof(strSendBuffer));    
	memset(strBuffer,NULL,sizeof(strBuffer));
	memcpy(strBuffer, WriteData_HEX, WriteData_HEX.GetLength());
	iWriteMaxLength=WriteData_HEX.GetLength()/2;

	////////写入转换为16进制///////////

	for(int i = 0; i < MAX_PATH; i++)
	{
		if(strBuffer[i] >= 'A' && strBuffer[i] <= 'F')
		{
			strBuffer[i] = strBuffer[i] - 'A' + 0x0a;
		}
		else if(strBuffer[i] >= 'a' && strBuffer[i] <= 'f')
		{
			strBuffer[i] = strBuffer[i] - 'a' + 0x0a;
		}
		else if(strBuffer[i] >= '0' && strBuffer[i] <= '9')
		{
			strBuffer[i] = strBuffer[i] - '0';
		}
	}

	strSendBuffer[0] = ((strBuffer[0] << 4) & 0xF0) + (strBuffer[1]&0x0F);
	strSendBuffer[1] = ((strBuffer[2] << 4) & 0xF0) + (strBuffer[3]&0x0F);
	strSendBuffer[2] = ((strBuffer[4] << 4) & 0xF0) + (strBuffer[5]&0x0F);
	strSendBuffer[3] = ((strBuffer[6] << 4) & 0xF0) + (strBuffer[7]&0x0F);
	strSendBuffer[4] = ((strBuffer[8] << 4) & 0xF0) + (strBuffer[9]&0x0F);
	strSendBuffer[5] = ((strBuffer[10] << 4) & 0xF0) + (strBuffer[11]&0x0F);
	strSendBuffer[6] = ((strBuffer[12] << 4) & 0xF0) + (strBuffer[13]&0x0F);
	strSendBuffer[7] = ((strBuffer[14] << 4) & 0xF0) + (strBuffer[15]&0x0F);
	strSendBuffer[8] = ((strBuffer[16] << 4) & 0xF0) + (strBuffer[17]&0x0F);
	strSendBuffer[9] = ((strBuffer[18] << 4) & 0xF0) + (strBuffer[19]&0x0F);
	strSendBuffer[10] = ((strBuffer[20] << 4) & 0xF0) + (strBuffer[21]&0x0F);
	strSendBuffer[11] = ((strBuffer[22] << 4) & 0xF0) + (strBuffer[23]&0x0F);
	strSendBuffer[12] = ((strBuffer[24] << 4) & 0xF0) + (strBuffer[25]&0x0F);
	strSendBuffer[13] = ((strBuffer[26] << 4) & 0xF0) + (strBuffer[27]&0x0F);
	strSendBuffer[14] = ((strBuffer[28] << 4) & 0xF0) + (strBuffer[29]&0x0F);
	strSendBuffer[15] = ((strBuffer[30] << 4) & 0xF0) + (strBuffer[31]&0x0F);
	strSendBuffer[16] = ((strBuffer[32] << 4) & 0xF0) + (strBuffer[33]&0x0F);
	strSendBuffer[17] = ((strBuffer[34] << 4) & 0xF0) + (strBuffer[35]&0x0F);
	strSendBuffer[18] = ((strBuffer[36] << 4) & 0xF0) + (strBuffer[37]&0x0F);
	strSendBuffer[19] = ((strBuffer[38] << 4) & 0xF0) + (strBuffer[39]&0x0F);
	strSendBuffer[20] = ((strBuffer[40] << 4) & 0xF0) + (strBuffer[41]&0x0F);

	strSendBuffer[21] = ((strBuffer[42] << 4) & 0xF0) + (strBuffer[43]&0x0F);
	strSendBuffer[22] = ((strBuffer[44] << 4) & 0xF0) + (strBuffer[45]&0x0F);
	strSendBuffer[23] = ((strBuffer[46] << 4) & 0xF0) + (strBuffer[47]&0x0F);
	strSendBuffer[24] = ((strBuffer[48] << 4) & 0xF0) + (strBuffer[49]&0x0F);
	strSendBuffer[25] = ((strBuffer[50] << 4) & 0xF0) + (strBuffer[51]&0x0F);
	strSendBuffer[26] = ((strBuffer[52] << 4) & 0xF0) + (strBuffer[53]&0x0F);
	strSendBuffer[27] = ((strBuffer[54] << 4) & 0xF0) + (strBuffer[55]&0x0F);
	strSendBuffer[28] = ((strBuffer[56] << 4) & 0xF0) + (strBuffer[57]&0x0F);
	strSendBuffer[29] = ((strBuffer[58] << 4) & 0xF0) + (strBuffer[59]&0x0F);
	strSendBuffer[30] = ((strBuffer[60] << 4) & 0xF0) + (strBuffer[61]&0x0F);

	strSendBuffer[31] = ((strBuffer[62] << 4) & 0xF0) + (strBuffer[63]&0x0F);
	strSendBuffer[32] = ((strBuffer[64] << 4) & 0xF0) + (strBuffer[65]&0x0F);
	strSendBuffer[33] = ((strBuffer[66] << 4) & 0xF0) + (strBuffer[67]&0x0F);
	strSendBuffer[34] = ((strBuffer[68] << 4) & 0xF0) + (strBuffer[69]&0x0F);
	strSendBuffer[35] = ((strBuffer[70] << 4) & 0xF0) + (strBuffer[71]&0x0F);
	strSendBuffer[36] = ((strBuffer[72] << 4) & 0xF0) + (strBuffer[73]&0x0F);
	strSendBuffer[37] = ((strBuffer[74] << 4) & 0xF0) + (strBuffer[75]&0x0F);
	strSendBuffer[38] = ((strBuffer[76] << 4) & 0xF0) + (strBuffer[77]&0x0F);
	strSendBuffer[39] = ((strBuffer[78] << 4) & 0xF0) + (strBuffer[79]&0x0F);
	strSendBuffer[40] = ((strBuffer[80] << 4) & 0xF0) + (strBuffer[81]&0x0F);
	////////////////////////////////////////////////////////////////////////////
	if(!CommInit(ComPort,baudrate))
	{
		ERRCODE="OPEN COMPORT FAIL";
		return false;
	}

	Clear();

	if(!ComWriteString( (BYTE*)strSendBuffer,iWriteMaxLength ))
	{
		ERRCODE="COMPOR WRITE FAIL";
		CommClose();
		return false;
	}

	Sleep(100);

//读
	
	BYTE strReadBuffer[1024];
	BYTE strgetBuffer[1024];

	DWORD piReadLength = 0;

	memset(strgetBuffer, '\0', sizeof(strgetBuffer));
	memset(strReadBuffer,'\0', sizeof(strReadBuffer));

	if(!ComReadString(strgetBuffer, 200,&piReadLength, 4000))
	{
		ERRCODE="COMPOR READ FAIL";
		CommClose();
		return false;
	}

	//if(piReadLength == 0)
	//{
	//	AfxMessageBox(TEXT("读取失败"));
	//	m_Com.CommClose();
	//	return 0;
	//}

	int j = 0;
	for(int i = 0; i < piReadLength; i++)
	{
		strReadBuffer[j++] = (strgetBuffer[i] & 0xF0) >> 4;
		strReadBuffer[j++] = strgetBuffer[i] & 0x0F;
	}

	for(int i = 0; i < 2*piReadLength; i++)
	{
		if(strReadBuffer[i] < 0 || strReadBuffer[i] > 9)
			strReadBuffer[i] = strReadBuffer[i] - 0x0a + 'A';
		else
			strReadBuffer[i] = strReadBuffer[i] + '0';
	}

	ReadData_HEX=strReadBuffer;

//	关闭：

	CommClose();

	return true;
}

bool ComPortEx::ComRead_HEX(int ComPort,int baudrate,CString &ReadData_HEX,CString &ERRCODE)
{
	CString DebugString="";

	if(!CommInit(ComPort,baudrate))
	{
		ERRCODE="OPEN COMPORT FAIL";
		return false;
	}

	Clear();

	//读

	BYTE strReadBuffer[MAX_PATH];
	BYTE strgetBuffer[MAX_PATH];

	DWORD piReadLength = 0;

	memset(strgetBuffer, '\0', MAX_PATH);
	memset(strReadBuffer,'\0', MAX_PATH);

	
	if(!ComReadString(strgetBuffer, 200,&piReadLength, 5000))
	{
		ERRCODE="COMPOR READ FAIL";
		CommClose();
		return false;
	}

	int j = 0;
	for(int i = 0; i < piReadLength; i++)
	{
		strReadBuffer[j++] = (strgetBuffer[i] & 0xF0) >> 4;
		strReadBuffer[j++] = strgetBuffer[i] & 0x0F;
	}

	for(int i = 0; i < 2*piReadLength; i++)
	{
		if(strReadBuffer[i] < 0 || strReadBuffer[i] > 9)
			strReadBuffer[i] = strReadBuffer[i] - 0x0a + 'A';
		else
			strReadBuffer[i] = strReadBuffer[i] + '0';
	}

	ReadData_HEX=strReadBuffer;

	//	关闭：

	CommClose();

	return true;
}

bool ComPortEx::ComWriteRead_String(int ComPort,int baudrate,CString WriteData_String,CString &ReadData_String,CString &ERRCODE)
{
	//	写：

	CString DebugString=TEXT("");
	BYTE strBuffer[MAX_PATH]={0};
	DWORD iWriteMaxLength=0;
	BYTE strSendBuffer[MAX_PATH]={0};

	iWriteMaxLength=WriteData_String.GetLength();

	memcpy(strSendBuffer,WriteData_String,iWriteMaxLength);
	strSendBuffer[iWriteMaxLength]=0x0D;
	iWriteMaxLength++;

	////////////////////////////////////////////////////////////////////////////
	if(!CommInit(ComPort,baudrate))
	{
		ERRCODE="OPEN COMPORT FAIL";
		return false;
	}

	Clear();

	if(!ComWriteString( strSendBuffer,iWriteMaxLength ))
	{
		ERRCODE="COMPOR WRITE FAIL";
		CommClose();
		return false;
	}

	DebugString.Format("DEBUG: COMPORT_WRITE  %d : %s",ComPort,WriteData_String);
	OutputDebugString(DebugString);
	Sleep(100);

	//读

	BYTE strReadBuffer[1024];
	BYTE strgetBuffer[1024];

	DWORD piReadLength = 0;

	memset(strgetBuffer, '\0', sizeof(strgetBuffer));
	memset(strReadBuffer,'\0', sizeof(strReadBuffer));

	if(!ComReadString(strgetBuffer, 200,&piReadLength, 4000))
	{
		ERRCODE="COMPOR READ FAIL";
		CommClose();
		return false;
	}

	ReadData_String=strgetBuffer;
	DebugString.Format("DEBUG: COMPORT_WRITE  %d : %s",ComPort,ReadData_String);
	OutputDebugString(DebugString);

	//	关闭：

	CommClose();

	return true;
}