#pragma once

#include "COMMON.h"


// ScanSN 对话框

class ScanSN : public CDialogEx
{
	DECLARE_DYNAMIC(ScanSN)

public:
	ScanSN(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ScanSN();

// 对话框数据
	enum { IDD = IDD_Dlg_SCANSN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
