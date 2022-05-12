// PHONE.cpp : 实现文件
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "PHONE.h"

ComPortEx m_ComPortEx;
extern CPIN_TESTDlg*	lpCPIN_TESTDlg;
// PHONE

IMPLEMENT_DYNAMIC(PHONE, CWnd)

	PHONE::PHONE()
{

}

PHONE::~PHONE()
{
}


BEGIN_MESSAGE_MAP(PHONE, CWnd)
END_MESSAGE_MAP()



// PHONE 消息处理程序
BOOL PHONE::Function_SendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,float TimeOut_s,CString &ERRCODE)
{
	CString DebugString=TEXT("");
	BYTE strBuffer[MAX_PATH]={0};
	DWORD iWriteMaxLength=0;
	BYTE strSendBuffer[MAX_PATH]={0};



	////////////////////////////////////////////////////////////////////////////
	if(!m_ComPortEx.CommInit(comport,baudrate))
	{
		ERRCODE.Format("PHONE_AT: OPEN_COMPORT_FAIL  %d",comport);
		return FALSE;
	}

	Sleep(50);
	m_ComPortEx.Clear();

	iWriteMaxLength=WriteData_String.GetLength();
	memcpy(strSendBuffer,WriteData_String,iWriteMaxLength);

	DebugString.Format("PHONE_AT: COMPORT_WRITE  %d : %s",comport,WriteData_String);
	lpCPIN_TESTDlg->OutPutDebugLog(DebugString);

	if(!m_ComPortEx.ComWriteString( strSendBuffer,iWriteMaxLength ))
	{
		ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}

	Sleep(50);
	m_ComPortEx.Clear();

	memset(strSendBuffer,NULL,260);
	strSendBuffer[0]=0x0D;
	strSendBuffer[1]=0x0A;

	if(!m_ComPortEx.ComWriteString( strSendBuffer,2 ))
	{
		ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}

	Sleep(500);

	return TRUE;
}

BOOL PHONE::Function_RendAtCommond(int comport,int baudrate,CString &ReadData_String,float TimeOut_s,CString &ERRCODE)
{
	CString DebugString=TEXT("");
	BYTE strBuffer[MAX_PATH]={0};
	DWORD iWriteMaxLength=0;
	BYTE strSendBuffer[MAX_PATH]={0};

	BYTE strReadBuffer[2048];
	BYTE strgetBuffer[2048];

	DWORD piReadLength = 0;

	memset(strgetBuffer, '\0', sizeof(strgetBuffer));
	memset(strReadBuffer,'\0', sizeof(strReadBuffer));


	if(!m_ComPortEx.ComReadString(strgetBuffer, 2047,&piReadLength,TimeOut_s))
	{
		ERRCODE.Format("PHONE_AT: COMPOR_READ FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}

	ReadData_String=strgetBuffer;

	DebugString.Format("PHONE_AT: COMPORT_READ  %d : %s",comport,ReadData_String);
	lpCPIN_TESTDlg->OutPutDebugLog(DebugString);

	return TRUE;
}

BOOL PHONE::Function_Close_AtCommond(int comport,int baudrate,float TimeOut_s,CString &ERRCODE)
{
	m_ComPortEx.CommClose();
	return TRUE;
}

BOOL PHONE::SendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,float TimeOut_s,CString &ERRCODE)
{
	CString DebugString=TEXT("");
	BYTE strBuffer[MAX_PATH]={0};
	DWORD iWriteMaxLength=0;
	BYTE strSendBuffer[MAX_PATH]={0};

	////////////////////////////////////////////////////////////////////////////
	if(!m_ComPortEx.CommInit(comport,baudrate))
	{
		ERRCODE.Format("PHONE_AT: OPEN_COMPORT_FAIL  %d",comport);
		return FALSE;
	}

	Sleep(50);
	m_ComPortEx.Clear();

	iWriteMaxLength=WriteData_String.GetLength();
	memcpy(strSendBuffer,WriteData_String,iWriteMaxLength);

	DebugString.Format("PHONE_AT: COMPORT_WRITE  %d : %s",comport,WriteData_String);
	lpCPIN_TESTDlg->OutPutDebugLog(DebugString);

	if(!m_ComPortEx.ComWriteString( strSendBuffer,iWriteMaxLength ))
	{
		ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}

	Sleep(50);
	m_ComPortEx.Clear();

	memset(strSendBuffer,NULL,260);
	strSendBuffer[0]=0x0D;
	strSendBuffer[1]=0x0A;

	if(!m_ComPortEx.ComWriteString( strSendBuffer,2 ))
	{
		ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}

	Sleep(50);

	//读

	BYTE strReadBuffer[1024];
	BYTE strgetBuffer[1024];

	DWORD piReadLength = 0;

	memset(strgetBuffer, '\0', sizeof(strgetBuffer));
	memset(strReadBuffer,'\0', sizeof(strReadBuffer));


	if(!m_ComPortEx.ComReadString(strgetBuffer, 1023,&piReadLength,TimeOut_s))
	{
		ERRCODE.Format("PHONE_AT: COMPOR READ FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}

	ReadData_String=strgetBuffer;

	DebugString.Format("PHONE_AT: COMPORT_READ  %d : %s",comport,ReadData_String);
	lpCPIN_TESTDlg->OutPutDebugLog(DebugString);


	//	关闭：

	m_ComPortEx.CommClose();

	return TRUE;
}
//抓完整TEST数据
BOOL PHONE::HMBSendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,float TimeOut_s,CString &ERRCODE)
{
	CString DebugString=TEXT("");
	BYTE strBuffer[MAX_PATH]={0};
	DWORD iWriteMaxLength=0;
	BYTE strSendBuffer[MAX_PATH]={0};



	////////////////////////////////////////////////////////////////////////////
	if(!m_ComPortEx.CommInit(comport,baudrate))
	{
		ERRCODE.Format("PHONE_AT: OPEN_COMPORT_FAIL  %d",comport);
		return FALSE;
	}

	Sleep(50);
	m_ComPortEx.Clear();

	iWriteMaxLength=WriteData_String.GetLength();
	memcpy(strSendBuffer,WriteData_String,iWriteMaxLength);

	DebugString.Format("PHONE_AT: COMPORT_WRITE  %d : %s",comport,WriteData_String);
	lpCPIN_TESTDlg->OutPutDebugLog(DebugString);

	if(!m_ComPortEx.ComWriteString( strSendBuffer,iWriteMaxLength ))
	{
		ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}

	Sleep(50);
	m_ComPortEx.Clear();

	memset(strSendBuffer,NULL,260);
	strSendBuffer[0]=0x0D;
	strSendBuffer[1]=0x0A;

	if(!m_ComPortEx.ComWriteString( strSendBuffer,2 ))
	{
		ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}


	Sleep(500);

	//读

	BYTE strReadBuffer[1024];
	BYTE strgetBuffer[1024];

	DWORD piReadLength = 0;

	memset(strgetBuffer, '\0', sizeof(strgetBuffer));
	memset(strReadBuffer,'\0', sizeof(strReadBuffer));

	int ncount = 0;
	while (1)
	{
		
		if(!m_ComPortEx.ComReadString(strgetBuffer, 1023,&piReadLength,TimeOut_s))
		{
			ERRCODE.Format("PHONE_AT: COMPOR READ FAIL  %d",comport);
			m_ComPortEx.CommClose();
			return FALSE;
		}

		ReadData_String=strgetBuffer;
		if (ReadData_String.Find("Total_ver")!=-1&&ReadData_String.Find("Battery_voltage")!=-1)
		{
			DebugString.Format("PHONE_AT: COMPORT_READ  %d : %s",comport,ReadData_String);
			lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
			break;
		}
		else
		{		
			Sleep(500);
		}
		if(ncount==5)
		{
			ERRCODE.Format("Can Not Catch The Valid Value");
			m_ComPortEx.CommClose();
			return FALSE;
		}
		ncount++;

	}
	/*if(!m_ComPortEx.ComReadString(strgetBuffer, 1023,&piReadLength,TimeOut_s))
	{
		ERRCODE.Format("PHONE_AT: COMPOR READ FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}

	ReadData_String=strgetBuffer;

	DebugString.Format("PHONE_AT: COMPORT_READ  %d : %s",comport,ReadData_String);
	lpCPIN_TESTDlg->OutPutDebugLog(DebugString);*/


	//	关闭：

	m_ComPortEx.CommClose();

	return TRUE;
}

BOOL PHONE::KeySendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,CString &ReadKeyValue,float TimeOut_s,CString &ERRCODE)
{
	CString DebugString=TEXT("");
	BYTE strBuffer[MAX_PATH]={0};
	DWORD iWriteMaxLength=0;
	BYTE strSendBuffer[MAX_PATH]={0};
	////////////////////////////////////////////////////////////////////////////
	int ncount = 0;
	while (1)
	{
		if(!m_ComPortEx.CommInit(comport,baudrate))
		{
			ERRCODE.Format("PHONE_AT: OPEN_COMPORT_FAIL  %d",comport);
			return FALSE;
		}
		Sleep(50);
		m_ComPortEx.Clear();
		iWriteMaxLength=WriteData_String.GetLength();
		memcpy(strSendBuffer,WriteData_String,iWriteMaxLength);
		if(!m_ComPortEx.ComWriteString( strSendBuffer,iWriteMaxLength ))
		{
			ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
			m_ComPortEx.CommClose();
			return FALSE;
		}
		DebugString.Format("PHONE_AT: COMPORT_WRITE  %d : %s",comport,WriteData_String);
		lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
		Sleep(50);
		m_ComPortEx.Clear();
		memset(strSendBuffer,NULL,260);
		strSendBuffer[0]=0x0D;
		strSendBuffer[1]=0x0A;
		if(!m_ComPortEx.ComWriteString( strSendBuffer,2 ))
		{
			ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
			m_ComPortEx.CommClose();
			return FALSE;
		}
		Sleep(500);
		//读
		BYTE strReadBuffer[1024];
		BYTE strgetBuffer[1024];
		DWORD piReadLength = 0;
		memset(strgetBuffer, '\0', sizeof(strgetBuffer));
		memset(strReadBuffer,'\0', sizeof(strReadBuffer));
		if(!m_ComPortEx.ComReadString(strgetBuffer, 1023,&piReadLength,TimeOut_s))
		{
			ERRCODE.Format("PHONE_AT: COMPOR READ FAIL  %d",comport);
			m_ComPortEx.CommClose();
			return FALSE;
		}
		ReadData_String=strgetBuffer;
		DebugString.Format("PHONE_AT: COMPORT_READ  %d : %s",comport,ReadData_String);
		lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
		if (ReadData_String.Find(ReadKeyValue)!=-1)
		{
			DebugString.Format("PHONE_AT: KeyValue \"%s\", Is Matched : %s",ReadKeyValue,ReadData_String);
			lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
			break;
		}
		else
		{		
			Sleep(1000);
		}
		if(ncount==10)
		{
			ERRCODE.Format("Can Not Catch The Valid Value");
			m_ComPortEx.CommClose();
			return FALSE;
		}
		ncount++;
	}
	m_ComPortEx.CommClose();
	return TRUE;
}

BOOL PHONE::UC15ENSendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,CString &ReadKeyValue,float TimeOut_s,CString &ERRCODE)
{
	CString DebugString=TEXT("");
	BYTE strBuffer[MAX_PATH]={0};
	DWORD iWriteMaxLength=0;
	BYTE strSendBuffer[MAX_PATH]={0};
	////////////////////////////////////////////////////////////////////////////
	int ncount = 0;
	while (1)
	{
		if(!m_ComPortEx.CommInit(comport,baudrate))
		{
			ERRCODE.Format("PHONE_AT: OPEN_COMPORT_FAIL  %d",comport);
			return FALSE;
		}

		Sleep(50);
		m_ComPortEx.Clear();

		iWriteMaxLength=WriteData_String.GetLength();
		memcpy(strSendBuffer,WriteData_String,iWriteMaxLength);
		//写入
		if(!m_ComPortEx.ComWriteString( strSendBuffer,iWriteMaxLength ))
		{
			ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
			m_ComPortEx.CommClose();
			return FALSE;
		}
		DebugString.Format("PHONE_AT: COMPORT_WRITE  %d : %s",comport,WriteData_String);
		lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
		Sleep(50);
		m_ComPortEx.Clear();
		memset(strSendBuffer,NULL,260);
		strSendBuffer[0]=0x0D;
		strSendBuffer[1]=0x0A;
		if(!m_ComPortEx.ComWriteString( strSendBuffer,2 ))
		{
			ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
			m_ComPortEx.CommClose();
			return FALSE;
		}
		Sleep(500);
		
		//读取
		CString sRes="",sStrDebug="";
		ReadData_String="";
		do 
		{
			if (ReadData_String.Find("OK")!=-1) break;
			if (ReadData_String!="") lpCPIN_TESTDlg->OutPutDebugLog(ReadData_String);
			ReadDataOnly(comport,baudrate,ReadData_String,60,ERRCODE);
			if (ReadData_String!="") lpCPIN_TESTDlg->OutPutDebugLog(ReadData_String);
			if (ReadData_String.Find("+OK")!=-1) break;
			if (ReadData_String.Find("ERROR")!=-1) break;
			/*if (ReadData_String.Find("KM Goldie power on")!=-1)
			{			
				CString sATcmd="AT+FACTTEST",sKeyValue="OK";
				if (!KeySendAtCommond(comport,baudrate,sATcmd,sRes,sKeyValue,3,sStrDebug)) goto FAIL;
				for (int i = 0;i<3;i++)
				{  
					if (sRes.Find("OK")==-1)
					{
						if (i==2) goto FAIL;
						continue;
					}
					else
					{
						sATcmd="AT+GCRESOURCE",sKeyValue="OK";
						if (!KeySendAtCommond(comport,baudrate,sATcmd,sRes,sKeyValue,3,sStrDebug)) goto FAIL;
						sStrDebug=sRes;
						lpCPIN_TESTDlg->ShowInfor("进入工厂模式成功!");
						return true;	
					}
					Sleep(500);
				}
           FAIL:
				lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
				return FALSE;
			}*/
		} while (1);
		if (ReadData_String.Find(ReadKeyValue)!=-1)//判断是循环
		{
			DebugString.Format("Read AT KeyWord: %s, LOOP TEST BREAD ",ReadData_String);
			lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
			break;
		}
		else
		{		
			DebugString.Format("Not AT KeyWord: %s, LOOP TEST CONTINUE",ReadData_String);
			lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
			Sleep(1000);
		}
		if(ncount==10)
		{
			ERRCODE.Format("Can Not Catch The Valid Value");
			m_ComPortEx.CommClose();
			return FALSE;
		}
		ncount++;
	}
	m_ComPortEx.CommClose();
	return TRUE;
}

BOOL PHONE::ReadDataOnly(int comport,int baudrate,CString &ReadData_String,float TimeOut_s,CString &ERRCODE)
{
	CString DebugString=TEXT("");
	BYTE strBuffer[MAX_PATH]={0};
	DWORD iWriteMaxLength=0;
	BYTE strSendBuffer[MAX_PATH]={0};
	
	////////////////////////////////////////////////////////////////////////////
	if(!m_ComPortEx.CommInit(comport,baudrate))
	{
		ERRCODE.Format("PHONE_AT: OPEN_COMPORT_FAIL  %d",comport);
		return FALSE;
	}
	Sleep(50);
	m_ComPortEx.Clear();
	//////////////////////////////////////////////////////////////////////////
	//读
	BYTE strReadBuffer[1024];
	BYTE strgetBuffer[1024];
	DWORD piReadLength = 0;

	memset(strgetBuffer, '\0', sizeof(strgetBuffer));
	memset(strReadBuffer,'\0', sizeof(strReadBuffer));

	if(!m_ComPortEx.ComReadString(strgetBuffer, 1023,&piReadLength,TimeOut_s))//TimeOut_s控制读取的时间
	{
		ERRCODE.Format("PHONE_AT: COMPOR READ FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}
	ReadData_String=strgetBuffer;

	//	关闭：

	m_ComPortEx.CommClose();

	return TRUE;
}

BOOL PHONE::HM003SendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,CString &ReadKeyValue,float TimeOut_s,CString &ERRCODE)
{
	CString DebugString=TEXT("");
	BYTE strBuffer[MAX_PATH]={0};
	DWORD iWriteMaxLength=0;
	BYTE strSendBuffer[MAX_PATH]={0};
	////////////////////////////////////////////////////////////////////////////
	int ncount = 0;
	while (1)
	{
		if(!m_ComPortEx.CommInit(comport,baudrate))
		{
			ERRCODE.Format("PHONE_AT: OPEN_COMPORT_FAIL  %d",comport);
			return FALSE;
		}

		Sleep(50);
		m_ComPortEx.Clear();

		iWriteMaxLength=WriteData_String.GetLength();
		memcpy(strSendBuffer,WriteData_String,iWriteMaxLength);
		//写入
		if(!m_ComPortEx.ComWriteString( strSendBuffer,iWriteMaxLength ))
		{
			ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
			m_ComPortEx.CommClose();
			return FALSE;
		}
		DebugString.Format("PHONE_AT: COMPORT_WRITE  %d : %s",comport,WriteData_String);
		lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
		Sleep(50);
		m_ComPortEx.Clear();
		memset(strSendBuffer,NULL,260);
		strSendBuffer[0]=0x0D;
		strSendBuffer[1]=0x0A;
		if(!m_ComPortEx.ComWriteString( strSendBuffer,2 ))
		{
			ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
			m_ComPortEx.CommClose();
			return FALSE;
		}
		//Sleep(500);
		
		//读取
		CString sRes="",sStrDebug="";
		ReadData_String="";
		do 
		{
			if (ReadData_String.Find("OK")!=-1) break;
			if (ReadData_String!="") lpCPIN_TESTDlg->OutPutDebugLog(ReadData_String);
			ReadDataOnly(comport,baudrate,ReadData_String,TimeOut_s,ERRCODE);
			if (ReadData_String!="") lpCPIN_TESTDlg->OutPutDebugLog(ReadData_String);
			if (ReadData_String.Find("+OK")!=-1) break;
			if (ReadData_String.Find("ERROR")!=-1) break;
			if (ReadData_String.Find("M Goldie power on")!=-1) break;//有可能重启的
		} while (1);
		if (ReadData_String.Find("M Goldie power on")!=-1)
		{			
			ERRCODE.Format("发现UC15模块Enable时非法重启!");
			lpCPIN_TESTDlg->ShowInfor("发现UC15Enable重启!");
			m_ComPortEx.CommClose();
			return FALSE;
		}
		if (ReadData_String.Find(ReadKeyValue)!=-1)//判断是循环
		{
			DebugString.Format("Read AT KeyWord: %s, LOOP TEST BREAK ",ReadData_String);
			lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
			break;
		}
		else
		{		
			DebugString.Format("Not AT KeyWord: %s, LOOP TEST CONTINUE",ReadData_String);
			lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
			Sleep(1000);
		}
		if(ncount==0)
		{
			ERRCODE.Format("Can Not Catch The Valid Value");
			m_ComPortEx.CommClose();
			return FALSE;
		}
		ncount++;
	}
	m_ComPortEx.CommClose();
	return TRUE;
}

BOOL PHONE::WDOGTestCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,CString &ReadKeyValue,float TimeOut_s,CString &ERRCODE)
{
	CString DebugString=TEXT("");
	BYTE strBuffer[MAX_PATH]={0};
	DWORD iWriteMaxLength=0;
	BYTE strSendBuffer[MAX_PATH]={0};
	////////////////////////////////////////////////////////////////////////////
	int ncount = 0;
	while (1)
	{
		if(!m_ComPortEx.CommInit(comport,baudrate))
		{
			ERRCODE.Format("PHONE_AT: OPEN_COMPORT_FAIL  %d",comport);
			return FALSE;
		}

		Sleep(50);
		m_ComPortEx.Clear();

		iWriteMaxLength=WriteData_String.GetLength();
		memcpy(strSendBuffer,WriteData_String,iWriteMaxLength);
		//写入
		if(!m_ComPortEx.ComWriteString( strSendBuffer,iWriteMaxLength ))
		{
			ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
			m_ComPortEx.CommClose();
			return FALSE;
		}
		DebugString.Format("PHONE_AT: COMPORT_WRITE  %d : %s",comport,WriteData_String);
		lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
		Sleep(50);
		m_ComPortEx.Clear();
		memset(strSendBuffer,NULL,260);
		strSendBuffer[0]=0x0D;
		strSendBuffer[1]=0x0A;
		if(!m_ComPortEx.ComWriteString( strSendBuffer,2 ))
		{
			ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
			m_ComPortEx.CommClose();
			return FALSE;
		}
		//Sleep(500);

		//读取
		CString sRes="",sStrDebug="";
		ReadData_String="";
		do 
		{
			//if (ReadData_String.Find("OK")!=-1) break;
			if (ReadData_String!="") lpCPIN_TESTDlg->OutPutDebugLog(ReadData_String);
			ReadDataOnly(comport,baudrate,ReadData_String,TimeOut_s,ERRCODE);
			if (ReadData_String!="") lpCPIN_TESTDlg->OutPutDebugLog(ReadData_String);
			//if (ReadData_String.Find("+OK")!=-1) break;
			//if (ReadData_String.Find("ERROR")!=-1) break;
			if (ReadData_String.Find("power on")!=-1) break;
		} while (1);
		if (ReadData_String.Find(ReadKeyValue)!=-1)//判断是否循环
		{
			DebugString.Format("Read AT KeyWord: %s, LOOP TEST BREAK ",ReadData_String);
			lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
			break;
		}
		else
		{		
			DebugString.Format("Not AT KeyWord: %s, LOOP TEST CONTINUE",ReadData_String);
			lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
			Sleep(1000);
		}
		if(ncount==1)
		{
			ERRCODE.Format("Can Not Catch The Valid Value");
			m_ComPortEx.CommClose();
			return FALSE;
		}
		ncount++;
	}
	m_ComPortEx.CommClose();
	return TRUE;
}

BOOL PHONE::WDOGSendAtCommond(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,CString &ReadKeyValue,float TimeOut_s,CString &ERRCODE)
{
	CString DebugString=TEXT("");
	BYTE strBuffer[MAX_PATH]={0};
	DWORD iWriteMaxLength=0;
	BYTE strSendBuffer[MAX_PATH]={0};

	////////////////////////////////////////////////////////////////////////////
	if(!m_ComPortEx.CommInit(comport,baudrate))
	{
		ERRCODE.Format("PHONE_AT: OPEN_COMPORT_FAIL  %d",comport);
		return FALSE;
	}
	Sleep(50);
	m_ComPortEx.Clear();
	iWriteMaxLength=WriteData_String.GetLength();
	memcpy(strSendBuffer,WriteData_String,iWriteMaxLength);
	if(!m_ComPortEx.ComWriteString( strSendBuffer,iWriteMaxLength ))
	{
		ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}
	DebugString.Format("PHONE_AT: COMPORT_WRITE  %d : %s",comport,WriteData_String);
	lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
	//////////////////////////////
	Sleep(50);
	m_ComPortEx.Clear();
	memset(strSendBuffer,NULL,260);
	strSendBuffer[0]=0x0D;
	strSendBuffer[1]=0x0A;
	if(!m_ComPortEx.ComWriteString( strSendBuffer,2 ))
	{
		ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}
	//////////////////////////////
	Sleep(500);
	int ncount = 0;
	while (1)
	{
		//读
		BYTE strReadBuffer[1024];
		BYTE strgetBuffer[1024];
		DWORD piReadLength = 0;
		memset(strgetBuffer, '\0', sizeof(strgetBuffer));
		memset(strReadBuffer,'\0', sizeof(strReadBuffer));
		if(!m_ComPortEx.ComReadString(strgetBuffer, 1023,&piReadLength,TimeOut_s))
		{
			ERRCODE.Format("PHONE_AT: COMPOR READ FAIL  %d",comport);
			m_ComPortEx.CommClose();
			return FALSE;
		}
		ReadData_String=strgetBuffer;
		DebugString.Format("PHONE_AT: COMPORT_READ  %d : %s",comport,ReadData_String);
		lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
		if (ReadData_String!="\0") continue;
		if (ReadData_String.Find(ReadKeyValue)!=-1)
		{
			DebugString.Format("PHONE_AT: KeyValue \"%s\", Is Matched : %s",ReadKeyValue,ReadData_String);
			lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
			break;
		}
		else
		{		
			Sleep(1000);
		}
		if(ncount==10)
		{
			ERRCODE.Format("Can Not Catch The Valid Value");
			m_ComPortEx.CommClose();
			return FALSE;
		}
		ncount++;
	}
	m_ComPortEx.CommClose();
	return TRUE;
}

BOOL PHONE::SendCommondOnly(int comport,int baudrate,CString WriteData_String,CString &ReadData_String,float TimeOut_s,CString &ERRCODE)
{
	CString DebugString=TEXT("");
	BYTE strBuffer[MAX_PATH]={0};
	DWORD iWriteMaxLength=0;
	BYTE strSendBuffer[MAX_PATH]={0};
	if(!m_ComPortEx.CommInit(comport,baudrate))
	{
		ERRCODE.Format("PHONE_AT: OPEN_COMPORT_FAIL  %d",comport);
		return FALSE;
	}
	Sleep(50);
	m_ComPortEx.Clear();
	iWriteMaxLength=WriteData_String.GetLength();
	memcpy(strSendBuffer,WriteData_String,iWriteMaxLength);
	DebugString.Format("PHONE_AT: COMPORT_WRITE  %d : %s",comport,WriteData_String);
	lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
	if(!m_ComPortEx.ComWriteString( strSendBuffer,iWriteMaxLength ))
	{
		ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}
	Sleep(50);
	m_ComPortEx.Clear();
	memset(strSendBuffer,NULL,260);
	strSendBuffer[0]=0x0D;
	strSendBuffer[1]=0x0A;
	if(!m_ComPortEx.ComWriteString( strSendBuffer,2 ))
	{
		ERRCODE.Format("PHONE_AT: COMPOR WRITE FAIL  %d",comport);
		m_ComPortEx.CommClose();
		return FALSE;
	}
	Sleep(500);
	//	关闭：
	m_ComPortEx.CommClose();
	return TRUE;
}