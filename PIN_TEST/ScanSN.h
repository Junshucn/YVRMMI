#pragma once

#include "COMMON.h"


// ScanSN �Ի���

class ScanSN : public CDialogEx
{
	DECLARE_DYNAMIC(ScanSN)

public:
	ScanSN(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ScanSN();

// �Ի�������
	enum { IDD = IDD_Dlg_SCANSN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
