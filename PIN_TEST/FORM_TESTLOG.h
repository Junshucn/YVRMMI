#pragma once
#include "afxcmn.h"


// FORM_TESTLOG �Ի���

class FORM_TESTLOG : public CDialogEx
{
	DECLARE_DYNAMIC(FORM_TESTLOG)

public:
	FORM_TESTLOG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FORM_TESTLOG();

// �Ի�������
	enum { IDD = IDD_DIALOG_TESTLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
};
