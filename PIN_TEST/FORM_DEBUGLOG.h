#pragma once
#include "afxwin.h"
#include "resource.h"


// FORM_DEBUGLOG 对话框

class FORM_DEBUGLOG : public CDialogEx
{
	DECLARE_DYNAMIC(FORM_DEBUGLOG)

public:
	FORM_DEBUGLOG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FORM_DEBUGLOG();

// 对话框数据
	enum { IDD = IDD_DIALOG_DEBUGLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListBox;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	HBRUSH hbr_ListBox;
	virtual void OnOK();
	virtual void OnCancel();
};
