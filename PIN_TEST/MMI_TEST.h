
// MMI_TEST.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//
#include "MessageBoxDlg.h"
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#define MB_YESNORETRY		0x10000000

// CPIN_TESTApp:
// �йش����ʵ�֣������ MMI_TEST.cpp
//

class CPIN_TESTApp : public CWinApp
{
public:
	CPIN_TESTApp();

// ��д
public:
	virtual BOOL InitInstance();
	//MessageBoxDlg m_MessageBoxDlg;

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
};

extern CPIN_TESTApp theApp;