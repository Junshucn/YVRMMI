// FORM_DEBUGLOG.cpp : 实现文件
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "FORM_DEBUGLOG.h"
#include "afxdialogex.h"


// FORM_DEBUGLOG 对话框

IMPLEMENT_DYNAMIC(FORM_DEBUGLOG, CDialogEx)

FORM_DEBUGLOG::FORM_DEBUGLOG(CWnd* pParent /*=NULL*/)
	: CDialogEx(FORM_DEBUGLOG::IDD, pParent)
{

}

FORM_DEBUGLOG::~FORM_DEBUGLOG()
{
}

void FORM_DEBUGLOG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
}


BEGIN_MESSAGE_MAP(FORM_DEBUGLOG, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// FORM_DEBUGLOG 消息处理程序


BOOL FORM_DEBUGLOG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	hbr_ListBox=  (HBRUSH)::CreateSolidBrush(RGB(255,255,240));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH FORM_DEBUGLOG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	if(pWnd->GetDlgCtrlID()==IDC_LISTBOX)
	{
		//pDC->SetTextColor(RGB(47,79,79));//设置字体颜色：
		pDC->SetBkMode(TRANSPARENT);//设置背景模式：这里设置为透明
		return hbr_ListBox;//设置文本框底色：（通过返回画刷改变底色）	
	}

	return hbr;
}


void FORM_DEBUGLOG::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void FORM_DEBUGLOG::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}
