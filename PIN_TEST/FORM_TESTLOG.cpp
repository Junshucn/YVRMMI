// FORM_TESTLOG.cpp : 实现文件
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "FORM_TESTLOG.h"
#include "afxdialogex.h"


// FORM_TESTLOG 对话框

IMPLEMENT_DYNAMIC(FORM_TESTLOG, CDialogEx)

FORM_TESTLOG::FORM_TESTLOG(CWnd* pParent /*=NULL*/)
	: CDialogEx(FORM_TESTLOG::IDD, pParent)
{

}

FORM_TESTLOG::~FORM_TESTLOG()
{
}

void FORM_TESTLOG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(FORM_TESTLOG, CDialogEx)
	ON_BN_CLICKED(IDOK, &FORM_TESTLOG::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &FORM_TESTLOG::OnBnClickedCancel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &FORM_TESTLOG::OnCustomdrawList)
END_MESSAGE_MAP()


// FORM_TESTLOG 消息处理程序


void FORM_TESTLOG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void FORM_TESTLOG::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


BOOL FORM_TESTLOG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_list.SetExtendedStyle( LVS_EX_FLATSB | LVS_EX_FULLROWSELECT |LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES|LVS_EX_TWOCLICKACTIVATE);   
	m_list.SetBkColor(RGB(255,255,240));
	//m_list.SetTextBkColor(RGB(255,250,205));
	//m_list.SetTextColor(RGB(106,90,205));
	m_list.InsertColumn(0,"NO",LVCFMT_CENTER,35,0);
	m_list.InsertColumn(1,"ITEM",LVCFMT_CENTER,100,0);
	m_list.InsertColumn(2,"MAXIMUM",LVCFMT_CENTER,100,0);
	m_list.InsertColumn(3,"MINIMUM",LVCFMT_CENTER,100,0);
	m_list.InsertColumn(4,"VALUE",LVCFMT_CENTER,380,0);
	m_list.InsertColumn(5,"RESULT",LVCFMT_CENTER,80,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void FORM_TESTLOG::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	 *pResult = CDRF_DODEFAULT;


	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	 {
		 *pResult = CDRF_NOTIFYITEMDRAW;
	 }
	 else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	 {
		 COLORREF crText,crBk;
		
		if (m_list.GetItemText(pLVCD->nmcd.dwItemSpec,5)=="PASS")
		{
			//crText = RGB(106,90,205);
			crText = RGB(0,100,0);
			crBk =RGB(255,255,240);
			//crBk =RGB(124,252,0);
			//crBk =RGB(255,250,205);
		}
		else
		{
			//crText = RGB(106,90,205);
			crText = RGB(255,0,0);
			crBk =RGB(255,255,240);
			//crBk =RGB(255,0,0);
		}


		 pLVCD->clrText = crText;
		 pLVCD->clrTextBk = crBk;

		//设置选择项的颜色
		//if( this->m_listcontrol.GetItemState(pLVCD->nmcd.dwItemSpec, CDIS_SELECTED) )
		//{
		//	 crBk =RGB(75, 149, 229);//itunes//RGB(10, 36, 106);//RGB(0, 0, 64);
		//	 crText = RGB(255,255,255);
		//	 pLVCD->clrText = crText;
		//	 pLVCD->clrTextBk = crBk;
		//	 *pResult = CDRF_NEWFONT;
		// }
		//if(LVIS_SELECTED == m_listcontrol.GetItemState(pLVCD->nmcd.dwItemSpec,LVIS_SELECTED))
		//{
		//	//清除选择状态，如果不清除的话，还是会显示出蓝色的高亮条
		//	 BOOL b = m_listcontrol.SetItemState(pLVCD->nmcd.dwItemSpec,0,LVIS_SELECTED); 
		//	pLVCD->clrText = crText;
		//	 pLVCD->clrTextBk = crBk;
		//	 *pResult = CDRF_NEWFONT;
		//	 return;
	 //   }
		*pResult = CDRF_NEWFONT;
		 //*pResult = CDRF_DODEFAULT;
	  }
}
