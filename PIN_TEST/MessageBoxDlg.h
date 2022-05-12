#pragma once


// MessageBoxDlg 对话框

class MessageBoxDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MessageBoxDlg)

public:
	MessageBoxDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MessageBoxDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MessageBox };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	UINT nRet;
	CString sTitil;
	//virtual void OnOK();
	//virtual void OnCancel();
	virtual BOOL OnInitDialog();
	CFont cfont_showmsg;
	CFont FButton_startfont;//状态框字体，不能设全局重画会丢失
	CMFCButton m_MfcButton_OK;
	CMFCButton m_MfcButton_CANCEL;
	CMFCButton m_MfcButton_RETRY;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonRetry();
	afx_msg void OnBnClickedCancel();
};
