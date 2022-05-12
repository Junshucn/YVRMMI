// MessageBoxDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "MessageBoxDlg.h"
#include "afxdialogex.h"


// MessageBoxDlg 对话框
extern MessageBoxDlg  m_MessageBoxDlg;
extern CString		g_sMessageBoxTilte;
MessageBoxDlg* lpMessageBoxDlg;

IMPLEMENT_DYNAMIC(MessageBoxDlg, CDialogEx)

MessageBoxDlg::MessageBoxDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MessageBox, pParent)
{
	cfont_showmsg.CreatePointFont(180, "Verdana");
	FButton_startfont.CreatePointFont(120, "Verdana");
}

MessageBoxDlg::~MessageBoxDlg()
{
}

void MessageBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_MfcButton_OK);
	DDX_Control(pDX, IDCANCEL, m_MfcButton_CANCEL);
	DDX_Control(pDX, IDC_BUTTON_RETRY, m_MfcButton_RETRY);
}


BEGIN_MESSAGE_MAP(MessageBoxDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RETRY, &MessageBoxDlg::OnBnClickedButtonRetry)
	ON_BN_CLICKED(IDOK, &MessageBoxDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &MessageBoxDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// MessageBoxDlg 消息处理程序


BOOL MessageBoxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundColor(RGB(176, 196, 222), 1);
	lpMessageBoxDlg = &m_MessageBoxDlg;//窗口寻址

	GetDlgItem(IDC_STATIC_MessageBox)->SetFont(&cfont_showmsg);
	GetDlgItem(IDC_STATIC_MessageBox)->SetWindowText(g_sMessageBoxTilte);

	m_MfcButton_OK.SetFont(&FButton_startfont, 1);
	m_MfcButton_OK.SetFaceColor(RGB(176, 196, 222), 1);
	m_MfcButton_CANCEL.SetFont(&FButton_startfont, 1);
	m_MfcButton_CANCEL.SetFaceColor(RGB(176, 196, 222), 1);
	m_MfcButton_RETRY.SetFont(&FButton_startfont, 1);
	m_MfcButton_RETRY.SetFaceColor(RGB(176, 196, 222), 1);
	GetDlgItem(IDC_STATIC_MessageBox)->SetFont(&cfont_showmsg, 1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void MessageBoxDlg::OnBnClickedButtonRetry()
{
	nRet = IDRETRY;
	EndDialog(nRet);    //关闭窗口 
	CDialogEx::OnClose();
}


void MessageBoxDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	nRet = IDOK;
	EndDialog(nRet);    //关闭窗口 
	CDialogEx::OnClose();
}


void MessageBoxDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	nRet = IDNO;
	EndDialog(nRet);    //关闭窗口 
	CDialogEx::OnClose();
}
