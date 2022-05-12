#pragma once
#include "afxcmn.h"


// FORM_TESTLOG 对话框

class FORM_TESTLOG : public CDialogEx
{
	DECLARE_DYNAMIC(FORM_TESTLOG)

public:
	FORM_TESTLOG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FORM_TESTLOG();

// 对话框数据
	enum { IDD = IDD_DIALOG_TESTLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
};
