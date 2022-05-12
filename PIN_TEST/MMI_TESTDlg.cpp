
// MMI_TESTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "MMI_TESTDlg.h"
#include "afxdialogex.h"
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib,"ExcelOperate_DLL")
#pragma comment(lib,"ControlBox_DLL")

CPIN_TESTDlg*	lpCPIN_TESTDlg;
FORM_TESTLOG	m_FORM_TESTLOG;
FORM_DEBUGLOG	m_FORM_DEBUGLOG;
MYWORK			m_MYWORK;
ParameterSet	m_ParameterSet;

CONFIG			m_CONFIG;
AllTestItem		m_TestItem;
EXCELTestItem   m_EXCELTestItem;
DefEXCELTestItem   m_DefEXCELTestItem;
CMdbControl		m_MdbControl;
CDevHound		m_DevHound;
MMIDATA         m_MMIDATA;

int				m_CurSelTab;
int				istate;
char			cErrCode[100][100];
CString			TOOL_VERSION;
BOOL			AUTO_START=FALSE;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPIN_TESTDlg 对话框

CPIN_TESTDlg::CPIN_TESTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPIN_TESTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONWY);
}

void CPIN_TESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Table);
	DDX_Control(pDX, IDC_MFCBUTTON_START, m_BTN_START);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_MFCBUTTON_CLEAR, m_BTN_CLEAR);
	DDX_Control(pDX, IDC_INPUTSN, m_InputSn);
	DDX_Control(pDX, IDC_INPUTPHONESN, m_InputPhoneSn);
	//DDX_Control(pDX, IDC_INPUTFixtureID, m_InputFixtureID);
}

BEGIN_MESSAGE_MAP(CPIN_TESTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CPIN_TESTDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPIN_TESTDlg::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CPIN_TESTDlg::OnTcnSelchangeTab)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_COMMAND(ID_32771, &CPIN_TESTDlg::Menu_Set)
	ON_BN_CLICKED(IDC_MFCBUTTON_START, &CPIN_TESTDlg::OnBnClickedMfcbuttonStart)
	ON_BN_CLICKED(IDC_MFCBUTTON_CLEAR, &CPIN_TESTDlg::OnBnClickedMfcbuttonClear)
	ON_MESSAGE(WM_DEV_HOUND, &CPIN_TESTDlg::OnOndevhound)
	//ON_MESSAGE(WM_INIT_FOCUS_SCANER,&CPIN_TESTDlg::PreTranslateMessage)
	ON_EN_CHANGE(IDC_INPUTPHONESN, &CPIN_TESTDlg::OnEnChangeInputphonesn)
END_MESSAGE_MAP()


// CPIN_TESTDlg 消息处理程序

BOOL CPIN_TESTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	lpCPIN_TESTDlg=this;
	this->SetBackgroundColor(RGB(176,196,222));
	InitMessage();//工具信息定义
	//初始化进度条风格
	m_Progress.SetBarColor(RGB(0, 255, 0));
	hbr_TOTAL= (HBRUSH)::CreateSolidBrush(RGB(255,185,15));	
	hbr_PASS=  (HBRUSH)::CreateSolidBrush(RGB(50,205,50));
	hbr_FAIL=  (HBRUSH)::CreateSolidBrush(RGB(255,0,0));	
	hbr_RATE=  (HBRUSH)::CreateSolidBrush(RGB(179,238,58));	

	hbr_STATE_PASS= (HBRUSH)::CreateSolidBrush(RGB(0,139,0));
	hbr_STATE_FAIL= (HBRUSH)::CreateSolidBrush(RGB(255,0,0));
	hbr_STATE_RUN= (HBRUSH)::CreateSolidBrush(RGB(255,193,37));
	hbr_STATE_READY= (HBRUSH)::CreateSolidBrush(RGB(0,191,255));

	BTN_START.CreatePointFont(200,"仿宋");
	RunStatefont.CreatePointFont(400,"宋体");
	FONT_TIMER.CreatePointFont(200,"宋体");
	FONT_STATION.CreatePointFont(400,"宋体");
	FONT_SN.CreatePointFont(130,"宋体");

	m_BTN_START.SetFont(&BTN_START);
	GetDlgItem(IDC_STATIC_STATE)->SetFont(&RunStatefont);
	GetDlgItem(IDC_STATIC_TIME)->SetFont(&FONT_TIMER);
	GetDlgItem(IDC_STATIC_SN)->SetFont(&FONT_SN);
	GetDlgItem(IDC_STATIC_Infor)->SetFont(&FONT_SN);
	GetDlgItem(IDC_STATIC_STATION)->SetFont(&FONT_STATION);
	           
	m_BTN_START.SetFaceColor(RGB(176, 196, 222), 1);
	m_BTN_CLEAR.SetFaceColor(RGB(176, 196, 222), 1);

	//---------------------------------------------------------------------//
	m_Table.InsertItem(0,"TEST LOG");
	m_Table.InsertItem(1,"DEBUG LOG");

	m_FORM_TESTLOG.Create(IDD_DIALOG_TESTLOG, &m_Table);
	m_FORM_DEBUGLOG.Create(IDD_DIALOG_DEBUGLOG, &m_Table);

	m_pDialog_Log[0] = &m_FORM_TESTLOG;
	m_pDialog_Log[1] = &m_FORM_DEBUGLOG;

	CRect rs,rsl;
	m_Table.GetClientRect(&rs);
	rsl=rs;
	rs.top += 25;
	rs.bottom -= 7;
	rs.left += 5;
	rs.right -= 6;

	rsl.top += 2;
	rsl.bottom -= 30;
	rsl.left += 0;
	rsl.right -= 9;
	//
	m_FORM_TESTLOG.MoveWindow(&rs);
	m_FORM_TESTLOG.m_list.MoveWindow(&rsl);
	m_FORM_DEBUGLOG.MoveWindow(&rs);
	m_FORM_DEBUGLOG.m_ListBox.MoveWindow(&rsl);

	//将子对话框放置到tab控件上
	m_FORM_TESTLOG.ShowWindow(TRUE);
	m_FORM_DEBUGLOG.ShowWindow(FALSE);
	m_Table.SetCurSel(0);
	m_CurSelTab= 0;
	//-----------------------------------------------------//
	ShowState(READY);
	m_CONFIG.sExeFilePath=GetExeFilePath();
	m_CONFIG.sConfigPath=m_CONFIG.sExeFilePath+"CONFIG.ini";
	m_ParameterSet.GetConfigFromFile();
	if (m_CONFIG.iProject == D2_YVR) GetSFISConfig();
	TOOL_VERSION="V1.00";
	CString TOOL_SUB_VERSION="02";
	//CString title = m_CONFIG.stu_Message.sProjectNameMessage[m_CONFIG.iProject] + "_TEST_"+TOOL_VERSION+"_"+TOOL_SUB_VERSION+"_20220302";
	CString title = "TEST_" + TOOL_VERSION + "_" + TOOL_SUB_VERSION + "_20220504";
	this->SetWindowText(title);
	if (!GetMdbParameter()) ExitProcess(1);
	UpdateWindow();//各个站别窗口显示设定
	m_DevHound.SetReceiver((DWORD)this->GetSafeHwnd(),WM_DEV_HOUND,FALSE);
	m_DevHound.Start();
	SetFocus(); //设置焦点
	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#pragma region SystemMethod
void CPIN_TESTDlg::InitMessage() {
	m_CONFIG.stu_Message.sProjectNameMessage[D1_YVR] = "D1";
	m_CONFIG.stu_Message.sProjectNameMessage[D2_YVR] = "D2";
	m_CONFIG.stu_Message.sTestSelectionMessage[HANDLE_PCBA_MMI_TEST] = "HANDLE_PCBA_MMI_TEST";
	m_CONFIG.stu_Message.sTestSelectionMessage[HANDLE_COUPLING_TEST] = "HANDLE_COUPLING_TEST";
	m_CONFIG.stu_Message.sTestSelectionMessage[HDM_COUPLING_TEST] = "HDM_COUPLING_TEST";
	m_CONFIG.stu_Message.sTestSelectionMessage[HDM_PAIR_TEST] = "HDM_PAIR_TEST";
	m_CONFIG.stu_Message.sTestSelectionMessage[HDM_CHECK_PACK] = "HDM_CHECK_PACK";
	m_CONFIG.stu_Message.sTestSelectionMessage[QA_PAIR_MESCHECK_PACK] = "QA_PAIR_MESCHECK_PACK";
	m_CONFIG.stu_Message.sTestSelectionMessage[PACK_WRITE] = "PACK_WRITESN";
	sprintf_s(cErrCode[INIT_POWER_FAIL], "%s", "INIT_POWER_FAIL");
	sprintf_s(cErrCode[CHECK_FUNCTION_FAIL], "%s", "CHECK_FUNCTION_FAIL");
	sprintf_s(cErrCode[CheckCurrent_FAIL], "%s", "CheckCurrent_FAIL");
	sprintf_s(cErrCode[PHONE_POWER_ON_FAIL], "%s", "PHONE_POWER_ON_FAIL");
	sprintf_s(cErrCode[CHECK_ATPORT_INSERT_FAIL], "%s", "CHECK_ATPORT_INSERT_FAIL");
	sprintf_s(cErrCode[CHECK_DEVICEID_MES_STATUS_FAIL], "%s", "CHECK_DEVICEID_MES_STATUS_FAIL");
	sprintf_s(cErrCode[PHONE_READ_SN_FAIL], "%s", "PHONE_READ_SN_FAIL");
	sprintf_s(cErrCode[MES_PassStation_FAIL], "%s", "MES_PassStation_FAIL");
	sprintf_s(cErrCode[MES_CheckStation_FAIL], "%s", "MES_CheckStation_FAIL");
	sprintf_s(cErrCode[MES_UPDATE_DATA_FAIL], "%s", "MES_UPDATE_DATA_FAIL");
	sprintf_s(cErrCode[Save_Log_FAIL], "%s", "Save_Log_FAIL");
	sprintf_s(cErrCode[READ_PCBASN_FAIL], "%s", "READ_PCBASN_FAIL");
	sprintf_s(cErrCode[CHECK_BSN_FAIL], "%s", "CHECK_BSN_FAIL");
	sprintf_s(cErrCode[HMD_CalStation_FAIL], "%s", "HMD_CalStation_FAIL");
	sprintf_s(cErrCode[HMD_RFCoupled_FAIL], "%s", "HMD_RFCoupled_FAIL");
	sprintf_s(cErrCode[HMD_PAIR_FAIL], "%s", "HMD_PAIR_FAIL");
}

void CPIN_TESTDlg::GetSFISConfig() {
	m_CONFIG.stu_SFISConfig.sConfigPath = m_CONFIG.sExeFilePath + "Settings.ini";
	GetPrivateProfileString("Env_Setting", "StationName", "", m_CONFIG.stu_SFISConfig.sStationName.GetBuffer(MAX_PATH), MAX_PATH, m_CONFIG.stu_SFISConfig.sConfigPath);
	m_CONFIG.stu_SFISConfig.sStationName.ReleaseBuffer();//StationName
	GetPrivateProfileString("Env_Setting", "Device_ID", "", m_CONFIG.stu_SFISConfig.sDeviceID.GetBuffer(MAX_PATH), MAX_PATH, m_CONFIG.stu_SFISConfig.sConfigPath);
	m_CONFIG.stu_SFISConfig.sDeviceID.ReleaseBuffer();//Device_ID
	GetPrivateProfileString("Env_Setting", "UserID", "", m_CONFIG.stu_SFISConfig.sUserID.GetBuffer(MAX_PATH), MAX_PATH, m_CONFIG.stu_SFISConfig.sConfigPath);
	m_CONFIG.stu_SFISConfig.sUserID.ReleaseBuffer();//UserID
}
#pragma endregion

void CPIN_TESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CPIN_TESTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CPIN_TESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPIN_TESTDlg::OnBnClickedMfcbuttonStart()
{
	CString isn="",phonesn="",FixtureID="",sTemp="";
	m_InputSn.GetWindowText(isn);
	m_InputPhoneSn.GetWindowText(phonesn);
	if(isn.GetLength() != 0 || phonesn.GetLength() !=0) {
		if (m_CONFIG.Test_Selection == HDM_COUPLING_TEST || m_CONFIG.Test_Selection == HANDLE_COUPLING_TEST || m_CONFIG.Test_Selection == HDM_PAIR_TEST ||
			m_CONFIG.Test_Selection == HANDLE_PCBA_MMI_TEST || m_CONFIG.Test_Selection == HDM_CHECK_PACK || m_CONFIG.Test_Selection == QA_PAIR_MESCHECK_PACK) {
			if (phonesn.Find(m_CONFIG.sPREFIX) != -1) {
				m_MYWORK.ThreadManager();
			}
			else {
				sTemp.Format("SN can't find prefix,%s", m_CONFIG.sPREFIX);
				OutPutDebugLog(sTemp);
			}
		}
		else {
			if (isn.Find(m_CONFIG.sPREFIX) != -1) {
				m_MYWORK.ThreadManager();
			}
			else {
				sTemp.Format("SN can't find prefix,%s", m_CONFIG.sPREFIX);
				OutPutDebugLog(sTemp);
			}
		}
		SetFocus();
	}
}

void CPIN_TESTDlg::OnBnClickedMfcbuttonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	CString ConfigFilePath="";

	m_CONFIG.TotalNumber="0";
	m_CONFIG.PASSNumber="0";
	m_CONFIG.FailNumber="0";
	m_CONFIG.Rate="0%";

	ConfigFilePath=m_CONFIG.sConfigPath;
	WritePrivateProfileString("CONFIG","TOTAL",m_CONFIG.TotalNumber,ConfigFilePath);
	WritePrivateProfileString("CONFIG","PASS",m_CONFIG.PASSNumber,ConfigFilePath);
	WritePrivateProfileString("CONFIG","FAIL",m_CONFIG.FailNumber,ConfigFilePath);
	WritePrivateProfileString("CONFIG","RATE",m_CONFIG.Rate,ConfigFilePath);
	SetDlgItemText(IDC_STATIC_TOTAL,m_CONFIG.TotalNumber);
	SetDlgItemText(IDC_STATIC_PASS,m_CONFIG.PASSNumber);
	SetDlgItemText(IDC_STATIC_FAIL,m_CONFIG.FailNumber);
	SetDlgItemText(IDC_STATIC_RATE,m_CONFIG.Rate);
}

void CPIN_TESTDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	/*CWnd *pWnd=GetFocus();
	int focusID=pWnd ->GetDlgCtrlID();

	switch(focusID)
	{
	case IDC_INPUTPHONESN:
		if (GetDlgItem(IDC_INPUTFixtureID)->IsWindowVisible())
		{
			GetDlgItem(IDC_INPUTFixtureID)->SetFocus();
			break;
		}
		else 
		{
			GetDlgItem(IDC_INPUTSN)->SetFocus();
			break;
		}
	case IDC_INPUTFixtureID:
		if (GetDlgItem(IDC_INPUTSN)->IsWindowVisible())
		{
			GetDlgItem(IDC_INPUTSN)->SetFocus();
			break;
		}
	}*/
}

void CPIN_TESTDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CPIN_TESTDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	//把当前的页面隐藏起来
	m_pDialog_Log[m_CurSelTab]->ShowWindow(SW_HIDE);

	//得到新的页面索引    
	m_CurSelTab = m_Table.GetCurSel();    
	//把新的页面显示出来     
	m_pDialog_Log[m_CurSelTab]->ShowWindow(SW_SHOW);   

	*pResult = 0;
}

HBRUSH CPIN_TESTDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->GetDlgCtrlID()==IDC_STATIC_STATE)
	{
		pDC->SetTextColor(RGB(248,248,255));//设置字体颜色：
		pDC->SetBkMode(TRANSPARENT);//设置背景模式：这里设置为透明

		switch(istate)
		{
		case READY:	return hbr_STATE_READY;	 break;//设置文本框底色：（通过返回画刷改变底色）	
		case RUN:	return hbr_STATE_RUN;	 break;
		case PASS:	return hbr_STATE_PASS;	 break;
		case FAIL:	return hbr_STATE_FAIL;	 break;
		}	
	}

	if(pWnd->GetDlgCtrlID()==IDC_STATIC_TIME||pWnd->GetDlgCtrlID()==IDC_STATIC_PASS_CAP||pWnd->GetDlgCtrlID()==IDC_STATIC_FAIL_CAP||pWnd->GetDlgCtrlID()==IDC_STATIC_TOTAL_CAP||pWnd->GetDlgCtrlID()==IDC_STATIC_RATE_CAP)
	{
		pDC->SetTextColor(RGB(139,129,76));//设置字体颜色：
		pDC->SetBkMode(TRANSPARENT);//设置背景模式：这里设置为透明
	}

	if(pWnd->GetDlgCtrlID()==IDC_STATIC_SN)
	{
		pDC->SetTextColor(RGB(106,90,205));//设置字体颜色：
		pDC->SetBkMode(TRANSPARENT);//设置背景模式：这里设置为透明
	}

	if(pWnd->GetDlgCtrlID()==IDC_STATIC_Infor)
	{
		pDC->SetTextColor(RGB(0,0,139));//设置字体颜色：
		pDC->SetBkMode(TRANSPARENT);//设置背景模式：这里设置为透明
	}

	if(pWnd->GetDlgCtrlID()==IDC_STATIC_PASS)
	{
		pDC->SetTextColor(RGB(0,139,0));//设置字体颜色：
		pDC->SetBkMode(TRANSPARENT);//设置背景模式：这里设置为透明
	}

	if(pWnd->GetDlgCtrlID()==IDC_STATIC_FAIL)
	{
		pDC->SetTextColor(RGB(255,0,0));//设置字体颜色：
		pDC->SetBkMode(TRANSPARENT);//设置背景模式：这里设置为透明
	}

	if(pWnd->GetDlgCtrlID()==IDC_STATIC_TOTAL)
	{
		pDC->SetTextColor(RGB(0,0,205));//设置字体颜色：
		pDC->SetBkMode(TRANSPARENT);//设置背景模式：这里设置为透明
	}

	if(pWnd->GetDlgCtrlID()==IDC_STATIC_STATION)
	{
		if (m_CONFIG.bMES)
		{
			pDC->SetTextColor(RGB(0,100,0));//设置字体颜色：
		}
		else
		{
			pDC->SetTextColor(RGB(205,173,0));//设置字体颜色：
		}

		pDC->SetBkMode(TRANSPARENT);//设置背景模式：这里设置为透明
	}


	if(pWnd->GetDlgCtrlID()==IDC_STATIC_RATE)
	{
		CString RATE="";
		float fRate=0;
		int index=0;

		GetDlgItemText(IDC_STATIC_RATE,RATE);
		index=RATE.GetLength();
		RATE=RATE.Left(index-2);
		fRate=atof(RATE);

		if (fRate<=60)
		{
			pDC->SetTextColor(RGB(255,0,0));//设置字体颜色：
		}
		else if (fRate>60&&fRate<90)
		{
			pDC->SetTextColor(RGB(205,149,12));//设置字体颜色：
		}
		else
		{
			pDC->SetTextColor(RGB(0,100,0));//设置字体颜色：
		}

		pDC->SetBkMode(TRANSPARENT);//设置背景模式：这里设置为透明
	}

	if(pWnd->GetDlgCtrlID()==IDC_PROGRESS1)
	{
		pDC->SetBkMode(TRANSPARENT);//设置背景模式：这里设置为透明
		pDC->SetTextColor(RGB(139,117,0)); 
	}
	return hbr;
}

void CPIN_TESTDlg::ShowState(int State)
{

	istate=State;
	switch(State)
	{
	case READY:	SetDlgItemText(IDC_STATIC_STATE,"READY"); 
		break;
	case RUN:	SetDlgItemText(IDC_STATIC_STATE,"RUN"); 
		break;
	case PASS:	SetDlgItemText(IDC_STATIC_STATE,"PASS"); 
		break;
	case FAIL:	SetDlgItemText(IDC_STATIC_STATE,"FAIL"); 
		break;		    
	}
	GetDlgItem(IDC_STATIC_STATE)->RedrawWindow();	
}

void CPIN_TESTDlg::ShowProgress(int iPost)
{
	int Rate=0;

	Rate=((float)iPost/(float)m_TestItem.TestIndex)*100;
	m_Progress.SetPos(Rate);
}

void CPIN_TESTDlg::ResultStatistics(int ERRCODE)
{
	long total=0,pass=0,fail=0,Rate=0;
	float fRate=0;
	CString ConfigFilePath="";


	pass=atol(m_CONFIG.PASSNumber);
	fail=atol(m_CONFIG.FailNumber);

	if (ERRCODE==SUCCESS)
	{
		pass++;
	}
	else
	{
		fail++;
	}

	total=pass+fail;

	m_CONFIG.TotalNumber.Format("%d",total);
	m_CONFIG.PASSNumber.Format("%d",pass);
	m_CONFIG.FailNumber.Format("%d",fail);

	if (total==0)
	{
		m_CONFIG.Rate="0 \%";
	}
	else
	{
		fRate=((float)pass/(float)total)*100;
		m_CONFIG.Rate.Format("%.1f",fRate);
		m_CONFIG.Rate=m_CONFIG.Rate+" \%";
	}

	ConfigFilePath=m_CONFIG.sConfigPath;
	WritePrivateProfileString("CONFIG","TOTAL",m_CONFIG.TotalNumber,ConfigFilePath);
	WritePrivateProfileString("CONFIG","PASS",m_CONFIG.PASSNumber,ConfigFilePath);
	WritePrivateProfileString("CONFIG","FAIL",m_CONFIG.FailNumber,ConfigFilePath);
	WritePrivateProfileString("CONFIG","RATE",m_CONFIG.Rate,ConfigFilePath);
	SetDlgItemText(IDC_STATIC_TOTAL,m_CONFIG.TotalNumber);
	SetDlgItemText(IDC_STATIC_PASS,m_CONFIG.PASSNumber);
	SetDlgItemText(IDC_STATIC_FAIL,m_CONFIG.FailNumber);
	SetDlgItemText(IDC_STATIC_RATE,m_CONFIG.Rate);
	
}

void CPIN_TESTDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CString TIME="";

	RunTime++;
	TIME.Format("%d",RunTime);
	SetDlgItemText(IDC_STATIC_TIME,TIME); 

	CDialogEx::OnTimer(nIDEvent);
}

CString CPIN_TESTDlg::GetExeFilePath()
{
	CString filepath="";
	GetModuleFileName(NULL,filepath.GetBuffer(MAX_PATH+1),MAX_PATH);
	filepath.ReleaseBuffer();
	int length=filepath.GetLength();
	int index=filepath.ReverseFind('\\');
	filepath=filepath.Left(index+1);
	return filepath;
}

void CPIN_TESTDlg::Menu_Set()
{
	// TODO: 在此添加命令处理程序代码
	m_ParameterSet.DoModal();
}

void CPIN_TESTDlg::UpdateWindow() {
	CString sTemp = "";
	SetDlgItemText(IDC_STATIC_TOTAL, m_CONFIG.TotalNumber);
	SetDlgItemText(IDC_STATIC_PASS, m_CONFIG.PASSNumber);
	SetDlgItemText(IDC_STATIC_FAIL, m_CONFIG.FailNumber);
	SetDlgItemText(IDC_STATIC_RATE, m_CONFIG.Rate);
	//if (m_CONFIG.Test_Selection == HANDLE_PCBA_MMI_TEST) {
	//	GetDlgItem(IDC_INPUTSN)->ShowWindow(TRUE);
	//	GetDlgItem(IDC_SN_STATIC)->ShowWindow(TRUE);
	//	GetDlgItem(IDC_SN_STATIC)->SetWindowText("Handle SN");
	//	GetDlgItem(IDC_INPUTSN)->SetFocus();
	//	GetDlgItem(IDC_INPUTPHONESN)->ShowWindow(FALSE);
	//	GetDlgItem(IDC_PHONESN_STATIC)->ShowWindow(FALSE);
	//}
	//else 
	if (m_CONFIG.Test_Selection == HDM_COUPLING_TEST || m_CONFIG.Test_Selection == HANDLE_COUPLING_TEST ||
		m_CONFIG.Test_Selection == HANDLE_PCBA_MMI_TEST || m_CONFIG.Test_Selection == HDM_PAIR_TEST  ||
		m_CONFIG.Test_Selection == HDM_CHECK_PACK
		) {
		GetDlgItem(IDC_INPUTPHONESN)->ShowWindow(TRUE);//显示IDC_INPUTPHONESN编辑框
		GetDlgItem(IDC_PHONESN_STATIC)->ShowWindow(TRUE);
		GetDlgItem(IDC_PHONESN_STATIC)->SetWindowText("Devices SN");
		GetDlgItem(IDC_INPUTPHONESN)->SetFocus();
		GetDlgItem(IDC_INPUTSN)->ShowWindow(false); //隐藏IDC_INPUTPHONESN编辑框
		GetDlgItem(IDC_SN_STATIC)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_LeftSN)->ShowWindow(false);
		GetDlgItem(IDC_LEFTSN)->ShowWindow(false);
	} 
	//else if (m_CONFIG.Test_Selection== HDM_CHECK_PACK) //查号
	//{
		//GetDlgItem(IDC_INPUTSN)->ShowWindow(FALSE);
		//GetDlgItem(IDC_SN_STATIC)->ShowWindow(FALSE);
		//GetDlgItem(IDC_SN_STATIC)->SetWindowText("BSN");

		//GetDlgItem(IDC_INPUTPHONESN)->ShowWindow(TRUE);
		//GetDlgItem(IDC_PHONESN_STATIC)->ShowWindow(TRUE);
		//GetDlgItem(IDC_PHONESN_STATIC)->SetWindowText("机身标二维码：");
		//GetDlgItem(IDC_FixtureID_STATIC)->ShowWindow(FALSE);
		//GetDlgItem(IDC_INPUTFixtureID)->ShowWindow(FALSE);
	//}
	//else if (m_CONFIG.Test_Selection== PACK_WRITE) //写号
	//{
	//	GetDlgItem(IDC_INPUTSN)->ShowWindow(FALSE);
	//	GetDlgItem(IDC_SN_STATIC)->ShowWindow(FALSE);
	//	GetDlgItem(IDC_SN_STATIC)->SetWindowText("BSN");
	//	GetDlgItem(IDC_INPUTPHONESN)->ShowWindow(TRUE);
	//	GetDlgItem(IDC_PHONESN_STATIC)->ShowWindow(TRUE);
	//	GetDlgItem(IDC_PHONESN_STATIC)->SetWindowText("PSN");
	//	//GetDlgItem(IDC_FixtureID_STATIC)->ShowWindow(FALSE);
	//	//GetDlgItem(IDC_INPUTFixtureID)->ShowWindow(FALSE);
	//}
	else if (m_CONFIG.Test_Selection == QA_PAIR_MESCHECK_PACK) { //MES检查
		GetDlgItem(IDC_INPUTPHONESN)->ShowWindow(TRUE);
		GetDlgItem(IDC_PHONESN_STATIC)->ShowWindow(TRUE);
		GetDlgItem(IDC_PHONESN_STATIC)->SetWindowText("HMDSN");
		GetDlgItem(IDC_INPUTPHONESN)->SetFocus();

		GetDlgItem(IDC_STATIC_LeftSN)->ShowWindow(TRUE);
		GetDlgItem(IDC_LEFTSN)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_LeftSN)->SetWindowText("Left SN");

		GetDlgItem(IDC_INPUTSN)->ShowWindow(TRUE);
		GetDlgItem(IDC_SN_STATIC)->ShowWindow(TRUE);
		GetDlgItem(IDC_SN_STATIC)->SetWindowText("Right SN");
	}

	if (m_CONFIG.bMES) {
		sTemp.Format("%s %s OnLine", m_CONFIG.stu_Message.sProjectNameMessage[m_CONFIG.iProject], 
			                         m_CONFIG.stu_Message.sTestSelectionMessage[m_CONFIG.Test_Selection]);
		SetDlgItemText(IDC_STATIC_STATION, sTemp);
	}
	else {
		sTemp.Format("%s %s OffLine", m_CONFIG.stu_Message.sProjectNameMessage[m_CONFIG.iProject],
			                          m_CONFIG.stu_Message.sTestSelectionMessage[m_CONFIG.Test_Selection]);
		SetDlgItemText(IDC_STATIC_STATION, sTemp);
	}
}

void  CPIN_TESTDlg::OutPutTestLog(TestItem iTestItem,CString TestValue,bool bJudgement)
{
	int index;
	CString sJudgement="",strindex="";
	CString sTestItem="";
	CString VALUE_MAX="",VALUE_MIN="";
	if (bJudgement)
	{
		sJudgement="PASS";
	}
	else
	{
		sJudgement="FAIL";
	}
	switch(iTestItem)
	{	
		case ENUM_READIMEI:			sTestItem="READ IMEI";			
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;
		case ENUM_Current:		sTestItem="Current";	
			VALUE_MAX.Format("%.3f",m_TestItem.Current_Max);
			VALUE_MIN.Format("%.3f",m_TestItem.Current_Min);
			break;
		case ENUM_IDLE:			sTestItem="待机电流";
			VALUE_MAX.Format("%.3f",m_TestItem.IDLE_Max);
			VALUE_MIN.Format("%.3f",m_TestItem.IDLE_Min);
			break;	
		case ENUM_ICCID:		sTestItem="ICCID";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_MES_UPDATE_Result:	sTestItem="MES_Update_result";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_IMSI:		sTestItem="MACIMEI加密码";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;		
		case ENUM_WRITETEST:			sTestItem="WRITE SN";			
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;
		case ENUM_WRITECUSTOMSN:			sTestItem="WRITECUSTOM";			
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;		
		case ENUM_WRITECFG:			sTestItem="WRITECFG";			
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_OPENLIGHTSENSER:			sTestItem="Open Light-Senser";			
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;			
		case ENUM_FLAGTEST:			sTestItem="FLAG TEST";			
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;
		case ENUM_versionsTEST:			sTestItem="versions TEST";			
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;
		case ENUM_FunctionTEST:			sTestItem="FUNCTION TEST";			
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;		
		case ENUM_MES_CHECK_DEVICE_ID_STATUS:		sTestItem="DEVICE_ID_STATUS";			
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;
		case ENUM_MES_Result:		sTestItem="MES Result";			
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;
		case ENUM_SETPOWERTEST:		sTestItem="POWER TEST";			
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;
		case ENUM_SETPOWERINIT:		sTestItem="POWER Initialize";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;		
		case ENUM_PORT:				sTestItem="PORT INIT";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_ALARM:			sTestItem="RUNIN";
			VALUE_MAX.Format("%.1f", m_TestItem.STI_RuninTime.rangeMax);
			VALUE_MIN.Format("%.1f", m_TestItem.STI_RuninTime.rangeMin);
			break;	
		case ENUM_MES_CHECK:	sTestItem="MES CHECK";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case  ENUM_MCU_VERSION:		sTestItem="SWVersion";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case  ENUM_MCU_HY_VERSION:	sTestItem="MCU辉烨版本号";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_IMEI:				sTestItem="IMEI";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case  ENUM_SIM:				sTestItem="SIM";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_READBSN:			sTestItem="READ BSN";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_INIT_CLOCK:			sTestItem="INIT_CLOCK";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_INIT_ENDCLOCK:			sTestItem="INIT_END_CLOCK";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_READPSN:			sTestItem="READ PSN";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_READ_FixtureID:	sTestItem = "READ ID";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_CSQ:				sTestItem ="RSSI";
			VALUE_MAX.Format("%f", m_TestItem.STI_averageRssi.rangeMax);
			VALUE_MIN.Format("%f", m_TestItem.STI_averageRssi.rangeMin);
			break;	
		case ENUM_CREG:				sTestItem = "LossRate";
			VALUE_MAX.Format("%f", m_TestItem.STI_packetLossRate.rangeMax);
			VALUE_MIN.Format("%f", m_TestItem.STI_packetLossRate.rangeMin);
			break;	
		case ENUM_CGREG:			sTestItem = "ReTransRate";
			VALUE_MAX.Format("%f", m_TestItem.STI_reTransRate.rangeMax);
			VALUE_MIN.Format("%f", m_TestItem.STI_reTransRate.rangeMin);
			break;	
		case ENUM_SEQ:			sTestItem = "ReAgvCount";
			VALUE_MAX.Format("%f", m_TestItem.STI_reAgvCount.rangeMax);
			VALUE_MIN.Format("%f", m_TestItem.STI_reAgvCount.rangeMin);
			break;
		case ENUM_WIFI:				sTestItem = "WIFI";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_BT:				sTestItem = "BT";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;	
		case ENUM_Light_Senser:				sTestItem = "Light_Senser";
			VALUE_MAX.Format("%d",m_TestItem.LightSen_MAX);
			VALUE_MIN.Format("%d",m_TestItem.LightSen_MIN);
			break;	
		case ENUM_Battery_Voltage:	sTestItem = "BATTERY CAP";
			VALUE_MAX.Format("%.1f", m_TestItem.STI_batteryCap.rangeMax);
			VALUE_MIN.Format("%.1f",m_TestItem.STI_batteryCap.rangeMin);
			break;
		case ENUM_ENTER_FACTTEST:	sTestItem = "ENTER_FACTTEST";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;
		case ENUM_TEMPTER:	sTestItem = "Tempter";
			VALUE_MAX.Format("%.1f",m_TestItem.TEMP_Max);
			VALUE_MIN.Format("%.1f",m_TestItem.TEMP_Min);
			break;
		case ENUM_HUMIDTY:	sTestItem = "HUMIDTY";
			VALUE_MAX.Format("%.1f",m_TestItem.HUMIDTY_Max);
			VALUE_MIN.Format("%.1f",m_TestItem.HUMIDTY_Min);
			break;
		case ENUM_IMU:	sTestItem = "IMU";
			VALUE_MAX.Format("%.1f", m_TestItem.STI_iMU.rangeMax);
			VALUE_MIN = "-";
			break;
		case ENUM_Trigger_ADC:	sTestItem = "Trigger_ADC";
			VALUE_MAX.Format("%.1f", m_TestItem.STI_triggerADC.rangeMax);
			VALUE_MIN.Format("%.1f", m_TestItem.STI_triggerADC.rangeMin);
			break;
		case ENUM_JoystickY_ADC:	sTestItem = "JoystickY_ADC";
			VALUE_MAX.Format("%.1f", m_TestItem.STI_joystickYADC.rangeMax);
			VALUE_MIN.Format("%.1f", m_TestItem.STI_joystickYADC.rangeMin);
			break;
		case ENUM_JoystickX_ADC:	sTestItem = "JoystickX_ADC";
			VALUE_MAX.Format("%.1f", m_TestItem.STI_joystickXADC.rangeMax);
			VALUE_MIN.Format("%.1f", m_TestItem.STI_joystickXADC.rangeMin);
			break;
		case ENUM_Vibrate:	sTestItem = "Vibrate";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;
		case ENUM_LED:	sTestItem = "LED";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;
		case ENUM_KEY:	sTestItem = "KEY";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;
		case ENUM_SPIFLASH:	sTestItem = "SPIFLASH";
			VALUE_MAX="-";
			VALUE_MIN="-";
			break;
		case ENUM_INFRARED:	sTestItem = "INFRARED";
			VALUE_MAX = "-";
			VALUE_MIN = "-";
			break;
		case ENUM_TOUCH:	sTestItem = "TOUCH";
			VALUE_MAX = "-";
			VALUE_MIN = "-";
			break;
		default:
			break;
	}
	index=m_FORM_TESTLOG.m_list.GetItemCount();
	strindex.Format("%d",index);
	m_FORM_TESTLOG.m_list.InsertItem(index,strindex);
	m_FORM_TESTLOG.m_list.SetItemText(index,5,sJudgement);//提前写入，方便判断显示的行颜色
	m_FORM_TESTLOG.m_list.SetItemText(index,1,sTestItem);
	m_FORM_TESTLOG.m_list.SetItemText(index,2,VALUE_MAX);
	m_FORM_TESTLOG.m_list.SetItemText(index,3,VALUE_MIN);
	m_FORM_TESTLOG.m_list.SetItemText(index,4,TestValue);

	m_FORM_TESTLOG.m_list.Update(index);//刷新，否则设置的行颜色显示异常
	m_FORM_TESTLOG.m_list.EnsureVisible(m_FORM_TESTLOG.m_list.GetItemCount()-1,FALSE);
	m_CONFIG.fPost++;
	int Rate = (int)((m_CONFIG.fPost / m_CONFIG.fItemNumber) * 100);
	m_Progress.SetPos(Rate);
}

void CPIN_TESTDlg::ClearTestLog()
{
	m_FORM_TESTLOG.m_list.DeleteAllItems();
}

void  CPIN_TESTDlg::OutPutDebugLog(CString sLog) {
	SaveOntimeDebugLog(sLog,"a");
	m_FORM_DEBUGLOG.m_ListBox.AddString(sLog.GetBuffer(sLog.GetLength()));
	sLog.ReleaseBuffer();
	m_FORM_DEBUGLOG.m_ListBox.SetTopIndex(m_FORM_DEBUGLOG.m_ListBox.GetCount()- 1);

}

void CPIN_TESTDlg::ClearDebugLog()
{

	while (m_FORM_DEBUGLOG.m_ListBox.GetCount())
	{
		m_FORM_DEBUGLOG.m_ListBox.DeleteString(0);
	}

	SaveOntimeDebugLog("","w");
}

void CPIN_TESTDlg::ShowSN(CString SN)
{
	SetDlgItemText(IDC_STATIC_SN,SN);
}

void CPIN_TESTDlg::ShowInfor(CString sInfor)
{
	SetDlgItemText(IDC_STATIC_Infor,sInfor);
}

void CPIN_TESTDlg::ShowIMEI(CString IMEI)
{
	SetDlgItemText(IDC_SHOW_IMEI,IMEI);
}

void CPIN_TESTDlg::SaveOntimeDebugLog(CString log,CString WriteType)
{
	int index=0;
	int ilength=0;
	CString sLogPath="",sTime="",Mylog="";
	FILE* fp;
	CTime time = CTime::GetCurrentTime();
	sTime = time.Format("%Y-%m-%d_%H-%M-%S");  
	Mylog.Format("%s :\t%s\n",sTime,log);
	sLogPath=m_CONFIG.sExeFilePath+"OnTimeLog\\";
	if(!PathFileExists(sLogPath))//文件夹不存在则创建
	{
		CreateDirectory(sLogPath,NULL);
	}
	sLogPath=sLogPath+"OntimeDebugLog.txt";
	fp=fopen(sLogPath,WriteType);
	fwrite(Mylog,Mylog.GetLength(),1,fp);
	fclose(fp);
	return;
}

bool CPIN_TESTDlg::SaveTestLog(CString SN,int iErrCode)
{
	int index=0;
	int ilength=0;
	CString slog="",getchar="",sLogPath="",sErrCode="",sTime="",sindex="",sGetText="",sLOG="";
	FILE* fp;
	OutPutDebugLog("------------------------------------------------------------------------");
	OutPutDebugLog("Save Run Log ...");
	OutPutDebugLog("------------------------------------------------------------------------"); 
	CTime time = CTime::GetCurrentTime();
	sTime = time.Format("%Y-%m-%d_%H-%M-%S");  
	sLogPath=m_CONFIG.sLogPath+"RunLOG\\";
	if(!PathFileExists(sLogPath)) {
		if(!CreateDirectory(sLogPath,NULL)) {
			OutPutDebugLog("CreateDirectory Fail:"+sLogPath);
			return false;  
		}
	}
	if (iErrCode==0) {
		sLogPath=sLogPath+"\\"+SN+"_"+sTime+"_PASS"+".txt";
	}
	else {
		sErrCode.Format("%s",cErrCode[iErrCode]);
		sLogPath=sLogPath+SN+"_"+sTime+"_FAIL_"+sErrCode+".txt";
	}
	index=m_FORM_DEBUGLOG.m_ListBox.GetCount();
	for (int i=0;i<index;i++) {
		getchar="";
		m_FORM_DEBUGLOG.m_ListBox.GetText(i,getchar);
		itoa(i,sindex.GetBuffer(MAX_PATH),10);
		sindex.ReleaseBuffer();
		getchar=sindex+".\t"+getchar+"\n";
		slog=slog+getchar;
	}
	fp=fopen(sLogPath,"w");
	if(fp==NULL) {
		OutPutDebugLog("Open Log Path Fail: "+sLogPath);
		return false;
	}
	fwrite(slog,slog.GetLength(),1,fp);
	fclose(fp);
	OutPutDebugLog("Save Run Log Success: "+sLogPath);
	return true;
}

LRESULT CPIN_TESTDlg::OnOndevhound(WPARAM wParam, LPARAM lParam)
{
	CString PortName="";
	_TCHAR szName[DEV_NAME_MAX_LEN] = {0};
	unsigned int nPort = (unsigned int)wParam;
	m_DevHound.GetPortName(nPort,szName);
	PortName=szName;

	switch(lParam)
	{
	case 0:
		break;

	case 1:
		break;
	}
	return 0;
}

void CPIN_TESTDlg::enumUsbPort(ScanPort *lpScanPort)
{
	int Portnum=0,PortIndex=0;
	CString PortName="";
	vector<T_DEV_INFO> vPortInfo_SerialPort;
	vector<T_DEV_INFO> vPortInfo_MODEM;

	for (int i=0;i<20;i++)
	{
		lpScanPort->portinfo_scan[i].iPortNumber=0;
		lpScanPort->portinfo_scan[i].sPortName="";
	}

	m_DevHound.ScanPort(vPortInfo_SerialPort,TYPE_DT_PORT);

	for (int i=0;i<vPortInfo_SerialPort.size();i++)
	{
		lpScanPort->portinfo_scan[i].iPortNumber=vPortInfo_SerialPort[i].nPortNum;
		lpScanPort->portinfo_scan[i].sPortName=vPortInfo_SerialPort[i].szDescription;
		PortIndex++;
	}

	m_DevHound.ScanPort(vPortInfo_MODEM,TYPE_DT_MODEM);

	for (int i=0;i<vPortInfo_MODEM.size();i++)
	{
		lpScanPort->portinfo_scan[i+PortIndex].iPortNumber=vPortInfo_MODEM[i].nPortNum;
		lpScanPort->portinfo_scan[i+PortIndex].sPortName=vPortInfo_MODEM[i].szDescription;
	}

}

bool CPIN_TESTDlg::GetRandomNumber(int Len,CString &RandomNumber)
{
	//随机取盐值
	CString ErrorCode="";
	/*HCRYPTPROV hCryptProv;
	DWORD dwErrorCode = 0;
	BYTE cRandomNumber[1024]={NULL};
	if( !CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) ) 
	{
		dwErrorCode = ::GetLastError();
		ErrorCode.Format(_T("DEBUG: Error Code(%d) occurs during CryptAcquireContext function!"), dwErrorCode);
		OutputDebugString(ErrorCode);
		return false;
	}

	if( !CryptGenRandom(hCryptProv, Len, cRandomNumber) ) 
	{
		dwErrorCode = ::GetLastError();
		ErrorCode.Format(_T("DEBUG: Error Code(%d) occurs during CryptGenRandom function!"), dwErrorCode);
		OutputDebugString(ErrorCode);
		if (NULL != hCryptProv)
		{
			CryptReleaseContext(hCryptProv, 0);
		}
		return false;
	}

	if (NULL != hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
	}

	for (int i=0; i<Len; i++)
	{
		switch (cRandomNumber[i]%3)
		{
		case 1:
			cRandomNumber[i] = cRandomNumber[i]%26 + 65;
			break;

		case 2:
			cRandomNumber[i] = cRandomNumber[i]%26 + 97;
			break;

		default:
			cRandomNumber[i] = cRandomNumber[i]%10 + 48;
			break;
		}
	}

	RandomNumber.Format("%s",cRandomNumber);*/

	return true;
}

bool CPIN_TESTDlg::GetRandomNumber_NUM(int Len,CString &RandomNumber)
{
	//随机取盐值
	CString ErrorCode="";
	/*HCRYPTPROV hCryptProv;
	DWORD dwErrorCode = 0;
	BYTE cRandomNumber[1024]={NULL};
	if( !CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) ) 
	{
		dwErrorCode = ::GetLastError();
		ErrorCode.Format(_T("DEBUG: Error Code(%d) occurs during CryptAcquireContext function!"), dwErrorCode);
		OutputDebugString(ErrorCode);
		return false;
	}

	if( !CryptGenRandom(hCryptProv, Len, cRandomNumber) ) 
	{
		dwErrorCode = ::GetLastError();
		ErrorCode.Format(_T("DEBUG: Error Code(%d) occurs during CryptGenRandom function!"), dwErrorCode);
		OutputDebugString(ErrorCode);
		if (NULL != hCryptProv)
		{
			CryptReleaseContext(hCryptProv, 0);
		}
		return false;
	}

	if (NULL != hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
	}

	for (int i=0; i<Len; i++)
	{
		cRandomNumber[i] = cRandomNumber[i]%10 + 48;
	}

	RandomNumber.Format("%s",cRandomNumber);

	return true;
}

void CPIN_TESTDlg::GetDataBySpecificChar(CString inputstr,CArray<CString,CString> &OutputData,CString _Delim)
{
	char *str_getvalu=NULL;
	char cDelim[100]={NULL};

	memcpy(cDelim,_Delim,strlen(_Delim));

	OutputData.SetSize(0);

	for (int i=0;i<5000;i++)
	{
		str_getvalu=NULL;

		if(i==0)
		{
			str_getvalu=strtok(inputstr.GetBuffer(strlen(inputstr)),cDelim);
			inputstr.ReleaseBuffer();
			if (str_getvalu == NULL)
			{
				break;
			}
			OutputData.Add(str_getvalu);
		}
		else
		{
			str_getvalu=strtok(NULL,cDelim);
			if (str_getvalu == NULL)
			{
				break;
			}
			OutputData.Add(str_getvalu);
		}	
	}*/
	return TRUE;
}

BOOL CPIN_TESTDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message ==WM_INIT_FOCUS_SCANER) {
		int idc_id;
		switch(pMsg->lParam) {//设置窗口焦点
		case HDM_COUPLING_TEST:idc_id =IDC_INPUTPHONESN;break;
		case HANDLE_PCBA_MMI_TEST:idc_id = IDC_INPUTSN;break;
		case HDM_PAIR_TEST:idc_id = IDC_INPUTPHONESN;break;
		case HDM_CHECK_PACK:idc_id = IDC_INPUTPHONESN;break;
		default:idc_id = IDC_INPUTPHONESN;break;
		}
		GetDlgItem(idc_id)->SetFocus();
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {//回车
		if (m_CONFIG.Test_Selection == QA_PAIR_MESCHECK_PACK) {
			if (GetFocus()->GetDlgCtrlID() == IDC_INPUTPHONESN) {
				GetDlgItem(IDC_LEFTSN)->SetFocus();
				return TRUE;
			}
			if (GetFocus()->GetDlgCtrlID() == IDC_LEFTSN) {
				GetDlgItem(IDC_INPUTSN)->SetFocus();
				return TRUE;
			}
			if (GetFocus()->GetDlgCtrlID() == IDC_INPUTSN) {
				OnBnClickedMfcbuttonStart();
				return TRUE;
			}	
		}
		else {
			if (GetFocus()->GetDlgCtrlID() == IDC_INPUTPHONESN) {
				OnBnClickedMfcbuttonStart();
				return TRUE;
			}
		}
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CPIN_TESTDlg::SetFocus()
{
	if (GetDlgItem(IDC_INPUTPHONESN)->IsWindowVisible()==TRUE)
	{
		GetDlgItem(IDC_INPUTPHONESN)->SetFocus();
	}
	/*else if (GetDlgItem(IDC_INPUTFixtureID)->IsWindowVisible()==TRUE)
	{
		GetDlgItem(IDC_INPUTFixtureID)->SetFocus();
	}*/
	else if (GetDlgItem(IDC_INPUTSN)->IsWindowVisible()==TRUE)
	{
		GetDlgItem(IDC_INPUTSN)->SetFocus();
	}
}

void CPIN_TESTDlg::OnEnChangeInputphonesn()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

bool CPIN_TESTDlg::GetTestItem_EXCEL()
{
	int index=0;
	CString GetBuffer="",ExcelVersion="",sATSetBufer="",sTargetBuff="",sEnableBuff="",sATQueryBufer="",sTemp="",sCFGVersion="";
	CArray<CString,CString>OutputData;
	int iRow=0,iColumn=0,iSetRo=0,iSetCo=0,iVerRo=0,iVerCo=0;
	m_EXCELTestItem.GetTestListSuccess=false;
	if(!PathFileExists(m_CONFIG.sCFGExcelPath))//文件夹不存在则创建
	{
		sTemp.Format("%s测试列表文件不存在，请重新选择",m_CONFIG.sCFGExcelPath);
		AfxMessageBox(sTemp);
		return false;  
	}
	//结构体一定要清零，否则会出错
	KillProcessFromName("Excel.exe");//关闭Excel先，防止读取出错
	for (int i=0;i<200;i++)
	{
		m_EXCELTestItem.m_TestList[i].Name="";
		m_EXCELTestItem.m_TestList[i].ATCommond="";
		m_EXCELTestItem.m_TestList[i].ATQuery="";
		m_EXCELTestItem.m_TestList[i].TargetReturnValue="";
		m_EXCELTestItem.m_TestList[i].ENABLE=FALSE;
	}
	m_EXCELTestItem.TestIndex=0;
	m_EXCELTestItem.TestItemNum=0;
	m_EXCELTestItem.sCFGVersion="";

	if (!InitExcel(FALSE,ExcelVersion))
	{
		AfxMessageBox("初始化EXCEL失败");
		return false;
	}
	if(!OpenExcel(m_CONFIG.sCFGExcelPath))  
	{  
		AfxMessageBox("测试列表文件打开失败");
		return false;  
	}
	if(!LoadSheet(false,"Configuration Check List"))  //TestItem  Configuration Check List
	{  
		CloseExcel();
		AfxMessageBox("加载Configuration Check List页面失败");
		return false;  
	}  

	iRow=GetRowCount();
	iColumn=GetColumnCount();
	if (iRow<2)
	{
		CloseExcel();
		AfxMessageBox("测试列表文件为空");
		return false;
	}
	for (int i=2;i<iRow+1;i++)//i是行，K是列
	{
		for (int k=1;k<iColumn;k++)
		{
			GetBuffer="";
			GetCell(i, k, GetBuffer);
			if (GetBuffer.Find("Cfg Version")!=-1) //Ver的位置找到了
			{
				iVerRo=i;
				iVerCo=k;
			}
			if (GetBuffer=="Set") //Set的位置找到了
			{
				iSetRo=i;
				iSetCo=k;
			}
		}
	}
	if (0==iVerRo||0==iVerCo) 
	{
		CloseExcel();
		AfxMessageBox("CFG文件版本没有找到！");
		return false;
	}
	GetCell(iVerRo, iVerCo+2, m_EXCELTestItem.sCFGVersion);

	if (0==iSetRo||0==iSetCo) 
	{
		CloseExcel();
		AfxMessageBox("CFG文件格式错误没有找到关键字");
		return false;
	}

	for (int i=iSetRo;i<iRow+1;i++)//获取Set列的字串
	{
		sATSetBufer="";
		sTargetBuff="";
		sEnableBuff="";
		sATQueryBufer="";
		GetCell(i+1, iSetCo, sATSetBufer);//Set
		GetCell(i+1, iSetCo+1, sATQueryBufer);//Query
		GetCell(i+1, iSetCo+2, sTargetBuff);//Value
		GetCell(i+1, iSetCo+3, sEnableBuff);//Enable
		if (sATSetBufer=="NULL")
		{
			CloseExcel();
			break;
		}
		m_EXCELTestItem.m_TestList[i-iSetRo].ATCommond=sATSetBufer;//SET
		m_EXCELTestItem.m_TestList[i-iSetRo].ATQuery=sATQueryBufer;//Query
		m_EXCELTestItem.m_TestList[i-iSetRo].TargetReturnValue=sTargetBuff;//Value
		m_EXCELTestItem.m_TestList[i-iSetRo].ENABLE=atoi(sEnableBuff);//Enable
	    m_EXCELTestItem.m_TestList[i-iSetRo].TestIndex++;//全部编号 从1开会编号
		m_EXCELTestItem.TestIndex++;//全部编号 从1编号
		if (m_EXCELTestItem.m_TestList[i-iSetRo].ENABLE) m_EXCELTestItem.TestItemNum++;	//个数
	}
	if (m_EXCELTestItem.TestItemNum==0)
	{
		CloseExcel();
		AfxMessageBox("测试列表文件为空");
		return false;
	}

	m_EXCELTestItem.GetTestListSuccess=true;
	CloseExcel();
	return true;
}

bool CPIN_TESTDlg::GetTestItem_DefaultEXCEL()
{
	int index=0;
	CString GetBuffer="",ExcelVersion="",sATSetBufer="",sTargetBuff="",sEnableBuff="",sATQueryBufer="",sTemp="";
	CArray<CString,CString>OutputData;
	int iRow=0,iColumn=0,iSetRo=0,iSetCo=0;
	m_DefEXCELTestItem.GetTestListSuccess=false;
	if(!PathFileExists(m_CONFIG.sDefaultExcelPath))//文件夹不存在则创建
	{
		sTemp.Format("%s测试列表文件不存在，请重新选择",m_CONFIG.sDefaultExcelPath);
		AfxMessageBox(sTemp);
		return false;  
	}
	//结构体一定要清零，否则会出错
	KillProcessFromName("Excel.exe");//关闭Excel先，防止读取出错
	for (int i=0;i<200;i++)
	{
		m_DefEXCELTestItem.m_TestList[i].Name="";
		m_DefEXCELTestItem.m_TestList[i].ATCommond="";
		m_DefEXCELTestItem.m_TestList[i].TargetReturnValue="";
		m_DefEXCELTestItem.m_TestList[i].ENABLE=FALSE;
	}
	m_DefEXCELTestItem.TestIndex=0;
	m_DefEXCELTestItem.TestItemNum=0;

	if (!InitExcel(FALSE,ExcelVersion))
	{
		AfxMessageBox("初始化EXCEL失败");
		return false;
	}
	if(!OpenExcel(m_CONFIG.sDefaultExcelPath))  
	{  
		AfxMessageBox("测试列表文件打开失败");
		return false;  
	}
	if(!LoadSheet(false,"Default value"))  //TestItem  Configuration Check List
	{  
		CloseExcel();
		AfxMessageBox("加载Default value页面失败");
		return false;  
	}  
	m_DefEXCELTestItem.TestIndex=0;
	m_DefEXCELTestItem.TestItemNum=0;
	iRow=GetRowCount();
	iColumn=GetColumnCount();
	if (iRow<2)
	{
		CloseExcel();
		AfxMessageBox("测试列表文件为空");
		return false;
	}
	for (int i=2;i<iRow+1;i++)//i是行，K是列
	{
		for (int k=1;k<iColumn;k++)
		{
			GetBuffer="";
			GetCell(i, k, GetBuffer);
			if (GetBuffer=="default value") //Set的位置找到了
			{
				iSetRo=i;
				iSetCo=k;
			}
		}
	}
	if (0==iSetRo||0==iSetCo) 
	{
		CloseExcel();
		AfxMessageBox("Default文件格式错误没有找到关键字");
		return false;
	}
	for (int i=iSetRo;i<iRow+1;i++)//获取Set列的字串
	{
		sATSetBufer="";
		sTargetBuff="";
		sEnableBuff="";
		sATQueryBufer="";
		GetCell(i+1, iSetCo, sATSetBufer);//default
		GetCell(i+1, iSetCo+1, sTargetBuff);//Value
		GetCell(i+1, iSetCo+2, sEnableBuff);//Enable
		//GetCell(i+1, iSetCo+3, sEnableBuff);//
		if (sATSetBufer=="NULL")
		{
			CloseExcel();
			break;
		}
		m_DefEXCELTestItem.m_TestList[i-iSetRo].ATCommond=sATSetBufer;//default
		//m_DefEXCELTestItem.m_TestList[i-iSetRo].ATQuery=sATQueryBufer;//Query
		m_DefEXCELTestItem.m_TestList[i-iSetRo].TargetReturnValue=sTargetBuff;//Value
		m_DefEXCELTestItem.m_TestList[i-iSetRo].ENABLE=atoi(sEnableBuff);//Enable
		m_DefEXCELTestItem.m_TestList[i-iSetRo].TestIndex++;//内部编号 从1编号
		m_DefEXCELTestItem.TestIndex++;//全部编号 从1编号
		if (m_DefEXCELTestItem.m_TestList[i-iSetRo].ENABLE) m_DefEXCELTestItem.TestItemNum++;	//个数
	}

	if (m_DefEXCELTestItem.TestItemNum==0)
	{
		CloseExcel();
		AfxMessageBox("测试列表文件为空");
		return false;
	}
	m_DefEXCELTestItem.GetTestListSuccess=true;
	CloseExcel();
	return true;
}

bool CPIN_TESTDlg::ReadColumn(CString TableName,CString FieldName,CArray<CString,CString>&ReadData)
{
	CArray<CString,CString>ReadBuffer;
	CString Message="",ErrCode="";
	ReadBuffer.SetSize(0);
	if (!m_MdbControl.ReadDatebase(TableName,FieldName,ReadBuffer,ErrCode))
	{
		Message.Format("读取 %s 失败:%s",FieldName,ErrCode);
		MessageBox(Message,"错误",MB_OK);
		return false;
	}
	else
	{
		if (ReadBuffer.GetSize()!=0)
		{
			ReadData.Copy(ReadBuffer);
		}
		else
		{
			ErrCode="No Data";
			Message.Format("读取 %s 失败:%s",FieldName,ErrCode);
			MessageBox(Message,"错误",MB_OK);
			return false;
		}
	}
	return true;
}

bool CPIN_TESTDlg::MACSPELLRULE(CString sMAC)
{
	string strText=sMAC.GetBuffer(MAX_PATH);		
	sMAC.ReleaseBuffer();
	regex testRegex("([A-Fa-f0-9]{2}:){5}[A-Fa-f0-9]{2}");
	cmatch mtstr_result;
	if (regex_search(strText.c_str(), mtstr_result, testRegex, regex_constants::format_default))return true;
	else MessageBox("请在<<BTWIFI.mdb>>文件BTUUID不能为空，输入WIFIMAC正确格式，11:22:33:44:55:66");
	return false;	
};

BOOL CPIN_TESTDlg::KillProcessFromName(CString strProcessName)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	if(!Process32First(hSnapShot,&pe))
	{
		return FALSE;
	}
	strProcessName.MakeLower();
	while (Process32Next(hSnapShot,&pe))
	{
		CString scTmp = pe.szExeFile;
		scTmp.MakeLower();
		if(!scTmp.Compare(strProcessName))
		{
			DWORD dwProcessID = pe.th32ProcessID;
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,dwProcessID);
			::TerminateProcess(hProcess,0);
			CloseHandle(hProcess);
			return TRUE;
		}
		scTmp.ReleaseBuffer();
	}
	strProcessName.ReleaseBuffer();
	return FALSE;
}

bool CPIN_TESTDlg::getStandardTestItemConfig(CString tableName, CString keyName, StandardTestItem& _item)
{
	CString ReadBuffer = "";
	CStringArray caTmp;
	BResult(ReadDatebase(tableName, keyName, ReadBuffer))
		Tools_splitStr(ReadBuffer, ",", caTmp);
	if (caTmp.GetCount() < 3)
	{
		MessageBox("配置文件Temperature设置异常！", "错误", MB_OK);
		return false;
	}
	_item.needTest = atof(caTmp.GetAt(_ISTEST));
	_item.rangeMin = atof(caTmp.GetAt(_RANGE_MIN));
	_item.rangeMax = atof(caTmp.GetAt(_RANGE_MAX));
	return true;
}

bool CPIN_TESTDlg::GetMdbParameter()
{
	CString sOutput = "", Message = "", ConfigPath = "", ReadBuffer = "";
	CString sTmp = ""; CStringArray caTmp;
	ConfigPath = m_CONFIG.sExeFilePath + "WYSetting.mdb";
	m_TestItem.bGetTestItemSuccess = false;
	m_TestItem.TestIndex = 0;
	if (!m_MdbControl.OpenDatabase(ConfigPath, "WY888", sOutput)) {
		Message.Format("打开测试项配置文件失败:%s", sOutput);
		MessageBox(Message, "错误", MB_OK);
		return false;
	}
	if (!ReadDatebase("Common", "Tools_version", ReadBuffer)) {
		m_MdbControl.CloseDatebase(sOutput);
		return false;
	}
	else {
		if (strcmp(TOOL_VERSION, ReadBuffer) != 0) {
			MessageBox("配置文件与工具版本不一致！", "错误", MB_OK);
			m_MdbControl.CloseDatebase(sOutput);
			return false;
		}
	}
	//字符串读取
	BResult(ReadDatebase("Common", "InputVoltValue", m_TestItem.sTESTVOLT))
	BResult(ReadDatebase("Common", "CheckFlag", ReadBuffer))m_TestItem.bCheckLastStepFlag = atoi(ReadBuffer);
    BResult(ReadDatebase("Common", "SetFlag", ReadBuffer))	m_TestItem.bSetFlag = atoi(ReadBuffer);
	BResult(getStandardTestItemConfig("Common", "JoystickXADC", m_TestItem.STI_joystickXADC));
	BResult(getStandardTestItemConfig("Common", "JoystickYADC", m_TestItem.STI_joystickYADC));
	BResult(getStandardTestItemConfig("Common", "TriggerADC", m_TestItem.STI_triggerADC));
	BResult(getStandardTestItemConfig("Common", "IMU", m_TestItem.STI_iMU))
	BResult(getStandardTestItemConfig("Common", "RSSI", m_TestItem.STI_averageRssi));
	BResult(getStandardTestItemConfig("Common", "PacketLossRate", m_TestItem.STI_packetLossRate));
	BResult(getStandardTestItemConfig("Common", "ReTransRate", m_TestItem.STI_reTransRate));
	BResult(getStandardTestItemConfig("Common", "ReAgvCount", m_TestItem.STI_reAgvCount));
	BResult(getStandardTestItemConfig("Common", "ReAgvCount", m_TestItem.STI_reAgvCount));
	BResult(getStandardTestItemConfig("Common", "RUNIN_TIME", m_TestItem.STI_RuninTime));
	BResult(ReadDatebase("Common", "CheckVersion", ReadBuffer))m_TestItem.bCheckVersion = atoi(ReadBuffer);
	BResult(ReadDatebase("Common", "INTVR", m_TestItem.sInterVersion))
	BResult(ReadDatebase("Common", "BT", ReadBuffer))m_TestItem.bBTMAC = atoi(ReadBuffer);
	BResult(ReadDatebase("Common", "WIFI", ReadBuffer))m_TestItem.bWIFIMAC = atoi(ReadBuffer);
	BResult(getStandardTestItemConfig("Common", "BATTCAP", m_TestItem.STI_batteryCap));


	//current config
	//BResult(ReadDatebase("Common", "Current", ReadBuffer))	m_TestItem.currentConfig.bcurrent = atoi(ReadBuffer);
	//BResult(getStandardTestItemConfig("Common", "PwrOnCurr", m_TestItem.currentConfig.PwrOnCurr))
	//BResult(getStandardTestItemConfig("Common", "IdleCurr", m_TestItem.currentConfig.IdleCurr))
	//BResult(getStandardTestItemConfig("Common", "PwrOffCurr", m_TestItem.currentConfig.PwrOffCurr))
	//BResult(getStandardTestItemConfig("Common", "ChargeCurr", m_TestItem.currentConfig.ChargeCurr))
	//CSQ
	//BResult(getStandardTestItemConfig("Common", "CSQ", m_TestItem.csqConfig))
	//function config

	//BResult(ReadDatebase("Common", "Pwr_off", ReadBuffer))		m_TestItem.bPwrOff = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "WriteBsn", ReadBuffer))		m_TestItem.bWriteBSN = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "WritePsn", ReadBuffer))		m_TestItem.bWritePsn = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "LED", ReadBuffer))			m_TestItem.bLED = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "BT", ReadBuffer))				m_TestItem.bBT = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "SPIFLASH", ReadBuffer))		m_TestItem.bSpiFlash = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "CSQ", ReadBuffer))			m_TestItem.bcsq = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "GPS", ReadBuffer))			m_TestItem.bgps = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "CREG", ReadBuffer))			m_TestItem.bcreg = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "CGREG", ReadBuffer))			m_TestItem.bcgreg = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "SIM", ReadBuffer))			m_TestItem.bSIM = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "Asensor", ReadBuffer))		m_TestItem.bAsensor = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "USE_Script", ReadBuffer))		m_TestItem.bUseScript = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "CheckVersion", ReadBuffer))	m_TestItem.productVersionConfig.bVersion = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "INTVR", m_TestItem.productVersionConfig.mcuInternalVersion))
	//BResult(ReadDatebase("Common", "ICCID", ReadBuffer))			m_TestItem.bIccid = atoi(ReadBuffer);
	//BResult(ReadDatebase("Common", "WDT", ReadBuffer))			m_TestItem.bWdt = atoi(ReadBuffer);
	//关闭数据库读取
	BResult(m_MdbControl.CloseDatebase(sOutput))
	m_TestItem.bGetTestItemSuccess = true;
	return true;
}

bool CPIN_TESTDlg::ReadDatebase(CString TableName, CString FieldName, CString &ReadData)
{
	CArray<CString, CString>ReadBuffer;
	CString Message = "", ErrCode = "";
	ReadBuffer.SetSize(0);
	ReadData = "";
	if (!m_MdbControl.ReadDatebase("Common", "Value", "Name", FieldName, ReadBuffer, ErrCode))
	{
		Message.Format("读取 %s 失败:%s", FieldName, ErrCode);
		MessageBox(Message, "错误", MB_OK);
		return false;
	}
	else
	{
		if (ReadBuffer.GetSize() != 0)
		{
			ReadData = ReadBuffer.GetAt(0);
			ReadData.Trim();
		}
		else
		{
			ErrCode = "No Data";
			Message.Format("读取 %s 失败:%s", FieldName, ErrCode);
			MessageBox(Message, "错误", MB_OK);
			return false;
		}
	}
	return true;
}


