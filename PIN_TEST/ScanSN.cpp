// ScanSN.cpp : 实现文件
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "ScanSN.h"
#include "afxdialogex.h"



extern CONFIG m_CONFIG;
// ScanSN 对话框

IMPLEMENT_DYNAMIC(ScanSN, CDialogEx)

ScanSN::ScanSN(CWnd* pParent /*=NULL*/)
	: CDialogEx(ScanSN::IDD, pParent)
{

}

ScanSN::~ScanSN()
{
}

void ScanSN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ScanSN, CDialogEx)
	ON_BN_CLICKED(IDOK, &ScanSN::OnBnClickedOk)
END_MESSAGE_MAP()


// ScanSN 消息处理程序


void ScanSN::OnBnClickedOk()
{
	char cSN[256]={NULL};

	SetDlgItemText(IDC_STATIC_INFO,"");
	GetDlgItemText(IDC_EDIT1,m_CONFIG.SN);
	if (m_CONFIG.SN=="")
	{
		return;
	}
	else
	{
		memcpy(cSN,m_CONFIG.SN,m_CONFIG.SN.GetLength());

		for (int i=0;i<m_CONFIG.SN.GetLength();i++)
		{
			if(!((cSN[i] >= '0' && cSN[i] <= '9') || (cSN[i] >= 'A' && cSN[i] <= 'Z')))
			{
				SetDlgItemText(IDC_STATIC_INFO,"错误： SN存在非法字符!");
				GetDlgItem(IDC_EDIT1)->SetFocus();
				return;
			}
		}
	}
	CDialogEx::OnOK();
}


BOOL ScanSN::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetBackgroundColor(RGB(176,196,222),1);
	GetDlgItem(IDC_EDIT1)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control

}
