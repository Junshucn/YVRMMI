#pragma once
#include "afxwin.h"
#include "resource.h"


// FORM_DEBUGLOG �Ի���

class FORM_DEBUGLOG : public CDialogEx
{
	DECLARE_DYNAMIC(FORM_DEBUGLOG)

public:
	FORM_DEBUGLOG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FORM_DEBUGLOG();

// �Ի�������
	enum { IDD = IDD_DIALOG_DEBUGLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListBox;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	HBRUSH hbr_ListBox;
	virtual void OnOK();
	virtual void OnCancel();
};
