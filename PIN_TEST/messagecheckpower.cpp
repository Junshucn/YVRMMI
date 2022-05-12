// messagecheckpower.cpp : 实现文件
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "messagecheckpower.h"
#include "afxdialogex.h"


// messagecheckpower 对话框

IMPLEMENT_DYNAMIC(messagecheckpower, CDialogEx)

messagecheckpower::messagecheckpower(CWnd* pParent /*=NULL*/)
	: CDialogEx(messagecheckpower::IDD, pParent)
{

}

messagecheckpower::~messagecheckpower()
{
}

void messagecheckpower::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(messagecheckpower, CDialogEx)
END_MESSAGE_MAP()


// messagecheckpower 消息处理程序
