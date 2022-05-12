// GPIBOperat.cpp : 实现文件
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "GPIBOperat.h"
#include "ni488.h"



extern CPIN_TESTDlg*	lpCPIN_TESTDlg;

// GPIBOperat

IMPLEMENT_DYNAMIC(GPIBOperat, CWnd)

GPIBOperat::GPIBOperat()
{

}

GPIBOperat::~GPIBOperat()
{
}


BEGIN_MESSAGE_MAP(GPIBOperat, CWnd)
END_MESSAGE_MAP()



// GPIBOperat 消息处理程序



bool GPIBOperat::OpenDevices(int &hDevroot,int iBoard,int iAddress,CString &EquipmentName)
{
	//hDevroot=ibdev(iBoard,iAddress,0,11,1,0);//打开并初始化设备
	//ibclr(hDevroot);//此函数对指定仪器进行清零,使仪器功能处于初始设置状态

	if (!GPIBWrite(hDevroot,"*IDN?"))
	{
		return false;
	}
	else
	{
		if(!GPIBRead(hDevroot,EquipmentName))
		{
			return false;
		}
	}

	return true;
}

bool GPIBOperat::CloseDevices(int hDevroot)
{
	//ibonl(hDevroot,0);//置接口板在线或离线
	//hDevroot=0;
	return true;
}

bool GPIBOperat::GPIBWrite(int hDevroot,CString sWriteBuffer)
{
	CString DebugString="";
	char PutBuffer[200];
	memset(PutBuffer,NULL,sizeof(PutBuffer));
	memcpy(PutBuffer,sWriteBuffer,sWriteBuffer.GetLength());

	DebugString.Format("DEBUG: GPIB_WRITE %d : %s",hDevroot,sWriteBuffer);
	OutputDebugString(DebugString);
	DebugString.Format("GPIB: GPIB_WRITE %d : %s",hDevroot,sWriteBuffer);
	lpCPIN_TESTDlg->OutPutDebugLog(DebugString);

	//ibwrt(hDevroot,PutBuffer,strlen(PutBuffer));
	//if (ibsta&ERR)
	//{
	//	return false;
	//}
	return true;
}

bool GPIBOperat::GPIBRead(int hDevroot,CString &GetBuffer)
{
	CString DebugString="";
	char getchar[300];
	memset(getchar,NULL,sizeof(getchar));
	//ibrd(hDevroot,getchar,sizeof(getchar));
	//if (ibsta&ERR)
	//{
	//	return false;
	//}
	GetBuffer=getchar;
	DebugString.Format("DEBUG: GPIB_READ %d : %s",hDevroot,GetBuffer);
	OutputDebugString(DebugString);
	DebugString.Format("GPIB: GPIB_READ %d : %s",hDevroot,GetBuffer);
	lpCPIN_TESTDlg->OutPutDebugLog(DebugString);
	return true;
}



