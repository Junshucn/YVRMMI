// FORM_DEBUGLOG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "FORM_DEBUGLOG.h"
#include "afxdialogex.h"


// FORM_DEBUGLOG �Ի���

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


// FORM_DEBUGLOG ��Ϣ�������


BOOL FORM_DEBUGLOG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	hbr_ListBox=  (HBRUSH)::CreateSolidBrush(RGB(255,255,240));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH FORM_DEBUGLOG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������

	if(pWnd->GetDlgCtrlID()==IDC_LISTBOX)
	{
		//pDC->SetTextColor(RGB(47,79,79));//����������ɫ��
		pDC->SetBkMode(TRANSPARENT);//���ñ���ģʽ����������Ϊ͸��
		return hbr_ListBox;//�����ı����ɫ����ͨ�����ػ�ˢ�ı��ɫ��	
	}

	return hbr;
}


void FORM_DEBUGLOG::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}


void FORM_DEBUGLOG::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnCancel();
}
