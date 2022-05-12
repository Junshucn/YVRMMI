
// MMI_TEST.h : PROJECT_NAME 应用程序的主头文件
//
#include "MessageBoxDlg.h"
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#define MB_YESNORETRY		0x10000000

// CPIN_TESTApp:
// 有关此类的实现，请参阅 MMI_TEST.cpp
//

class CPIN_TESTApp : public CWinApp
{
public:
	CPIN_TESTApp();

// 重写
public:
	virtual BOOL InitInstance();
	//MessageBoxDlg m_MessageBoxDlg;

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
};

extern CPIN_TESTApp theApp;