// ParameterSet.cpp : 实现文件
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "ParameterSet.h"
#include "afxdialogex.h"

extern CONFIG			m_CONFIG;
extern CPIN_TESTDlg*	lpCPIN_TESTDlg;
extern AllTestItem		m_TestItem;
// ParameterSet 对话框
IMPLEMENT_DYNAMIC(ParameterSet, CDialogEx)
ParameterSet::ParameterSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(ParameterSet::IDD, pParent)
{

}

ParameterSet::~ParameterSet()
{
}

void ParameterSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_POWERTYPE, m_ComboBox_PowerType);
	DDX_Control(pDX, IDC_COMBO_BOARDADD, m_ComboBox_BoardADD);
	DDX_Control(pDX, IDC_COMBO_GPIBAD, m_ComboBox_GPIBAdd);
	DDX_Control(pDX, IDC_COMBO_PHONEPORT, m_ComboBox_PHONEPORT);
	DDX_Control(pDX, IDC_COMBO_PHONEPORT2, m_ComboBox_PHONEPORT2);
	DDX_Control(pDX, IDC_COMBO_USBCTRPORT, m_ComboBox_CONTROLBOXPORT);
	DDX_Control(pDX, IDC_COMBO_BaudRate, m_COMB_BaudRate);
	DDX_Control(pDX, IDC_CHECK1, m_CHECK_MES);
	DDX_Control(pDX, IDC_CHECK_TESTFLAG, m_CHECK_TESTFLAG);
	DDX_Control(pDX, IDC_CHECK_Current, m_CHECK_Current);
	DDX_Control(pDX, IDC_CHECK_SN, m_CHECT_SN);
	DDX_Control(pDX, IDC_CHECK_FUNCTION, m_CHECK_Function);
	DDX_Control(pDX, IDC_CHECK_GPS, m_CHECT_GPS);
	DDX_Control(pDX, IDC_CHECK_CREG, m_CHECT_CREG);
	DDX_Control(pDX, IDC_CHECK_CGREG, m_CHECT_CGREG);
	DDX_Control(pDX, IDC_CHECK_VERSION, m_CHECT_VERSION);
	DDX_Control(pDX, IDC_COMBO_Test_Selection, m_Test_Selection);
	DDX_Control(pDX, IDC_CHECK_WIFI, m_CHECT_WIFI);
	DDX_Control(pDX, IDC_CHECK_Light_sensitive, m_CHECT_Light_sensitive);
	DDX_Control(pDX, IDC_Write_Fixture_ID, m_Write_Fixture_ID);
	DDX_Control(pDX, IDC_CHECK_IMEI, m_CHECK_IMEI);
	DDX_Control(pDX, IDC_CHECK_SIM, m_CHECK_SIM);
	DDX_Control(pDX, IDC_CHECK_CSQ, m_CHECK_CSQ);
	DDX_Control(pDX, IDC_EDIT_FIXTURENUMBER, m_InputFixtureID);
	DDX_Control(pDX, IDC_COMBO_ProjectName, m_ComboBox_Pro);

}

BEGIN_MESSAGE_MAP(ParameterSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &ParameterSet::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ParameterSet::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_LOGPATH, &ParameterSet::OnBnClickedButtonLogpath)
	ON_BN_CLICKED(IDC_CHECK1, &ParameterSet::OnBnClickedCheck1)

	ON_CBN_SELCHANGE(IDC_COMBO_Test_Selection, &ParameterSet::OnCbnSelchangeComboTestSelection)
	ON_BN_CLICKED(IDC_BUTTON_CFGIN, &ParameterSet::OnBnClickedButtonCfgin)
	ON_BN_CLICKED(IDC_BUTTON_Default, &ParameterSet::OnBnClickedButtonDefault)
END_MESSAGE_MAP()

// ParameterSet 消息处理程序

BOOL ParameterSet::OnInitDialog() {
	CDialogEx::OnInitDialog();
	this->SetBackgroundColor(RGB(176,196,222));
	for (int i = 0; i < StationMaxNum; i++) {
		m_Test_Selection.InsertString(i, m_CONFIG.stu_Message.sTestSelectionMessage[i]);
	}
	m_Test_Selection.SetCurSel(0);
	for (int i = 0; i < ProjectMaxNum; i++) {
		m_ComboBox_Pro.InsertString(i, m_CONFIG.stu_Message.sProjectNameMessage[i]);
	}
	m_ComboBox_Pro.SetCurSel(0);
	m_ComboBox_PowerType.InsertString(Agilent,"Agilent");
	m_ComboBox_PowerType.InsertString(KEITHLEY,"KEITHLEY");
	m_ComboBox_PowerType.SetCurSel(0);
	m_ComboBox_BoardADD.InsertString(0,"0");
	m_ComboBox_BoardADD.InsertString(1,"1");
	m_ComboBox_BoardADD.SetCurSel(0);
	CString Number;
	for (int i=0;i<21;i++) {
		itoa(i,Number.GetBuffer(MAX_PATH),10);
		m_ComboBox_GPIBAdd.InsertString(i,Number);
		Number.ReleaseBuffer();
		Number="";
	}
	m_ComboBox_GPIBAdd.SetCurSel(5);
	for (int i=0;i<256;i++) {
		itoa(i,Number.GetBuffer(MAX_PATH),10);
		m_ComboBox_PHONEPORT.InsertString(i,Number);
		m_ComboBox_PHONEPORT2.InsertString(i,Number);
		Number.ReleaseBuffer();
		Number="";
	}
	m_ComboBox_PHONEPORT.SetCurSel(0);
	m_ComboBox_PHONEPORT2.SetCurSel(0);
	m_COMB_BaudRate.InsertString(0,"9600");
	m_COMB_BaudRate.InsertString(1,"115200");
	m_COMB_BaudRate.SetCurSel(1);
	PutConfigSetToPage();
	OnCbnSelchangeComboTestSelection();
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void ParameterSet::OnBnClickedOk() {
	CString LogPath="",sPREFIX="";
	m_CONFIG.bMES=m_CHECK_MES.GetCheck();
	GetDlgItemText(IDC_EDIT_PREFIX,sPREFIX);
	GetDlgItemText(IDC_EDIT_LOGPATH,LogPath);
	GetDlgItemText(IDC_EDIT_MesUserName,m_CONFIG.sMESUser);
	GetDlgItemText(IDC_EDIT_MES_Line,m_CONFIG.sMES_Line);
	GetDlgItemText(IDC_EDIT_MesStep,m_CONFIG.sMESStationName);//
	m_CONFIG.iProject = m_ComboBox_Pro.GetCurSel();
	GetDlgItemText(IDC_EDIT_PO,m_CONFIG.sPO); //工单号
	GetDlgItemText(IDC_EDIT_FIXTURENUMBER,m_CONFIG.sJigNum);
	GetDlgItemText(IDC_EDIT_MES_URL,m_CONFIG.sMESURL);
	if (LogPath.IsEmpty() ||!PathFileExists(LogPath)) {
		AfxMessageBox("LOG保存路径不存在，请重新选择");
		return;
	}
	m_CONFIG.sLogPath=LogPath;
	m_CONFIG.sPREFIX=sPREFIX;
	m_CONFIG.iPowerType=m_ComboBox_PowerType.GetCurSel();
	m_CONFIG.iBoardAdd=m_ComboBox_BoardADD.GetCurSel();
	m_CONFIG.iGPIBAdd=m_ComboBox_GPIBAdd.GetCurSel();
	m_CONFIG.iPhonePort=m_ComboBox_PHONEPORT.GetCurSel();
	m_CONFIG.iPhonePort2 = m_ComboBox_PHONEPORT2.GetCurSel();
	m_CONFIG.iCtrBOXComPort=m_ComboBox_CONTROLBOXPORT.GetCurSel();
	m_CONFIG.Test_Selection=m_Test_Selection.GetCurSel();

	if (m_CONFIG.iProject == D2_YVR) lpCPIN_TESTDlg->GetSFISConfig();//获取和硕MES配置
	lpCPIN_TESTDlg->PostMessage(WM_INIT_FOCUS_SCANER,0U,m_CONFIG.Test_Selection);
	SaveConfigToFile();
	lpCPIN_TESTDlg->UpdateWindow();
	//lpCPIN_TESTDlg->SetFocus();
	CDialogEx::OnOK();
}

void ParameterSet::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void ParameterSet::OnBnClickedButtonLogpath()
{
	// TODO: 在此添加控件通知处理程序代码
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString sPath="";
	CString str;

	ZeroMemory(szPath, sizeof(szPath));  

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;				// 父窗口句柄
	bi.pidlRoot = NULL;					// 要显示的文件夹的根(Root)
	bi.pszDisplayName = szPath;			// 保存被选取的文件夹路径的缓冲区
	bi.lpszTitle = "请选择LOG保存路径";// 显示位于对话框左上部的标题   
	bi.ulFlags = BIF_RETURNONLYFSDIRS;	// 指定对话框的外观和功能的标志
	bi.lpfn = NULL;						// 处理事件的回调函数
	bi.lParam = 0;						// 应用程序传给回调函数的参数
	bi.iImage = 0;						// 保存被选取的文件夹的图片索引
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);  

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		sPath.Format("%s",szPath);

		if (sPath.Right(1)!='\\')
		{
			sPath=sPath+"\\";
		}
		SetDlgItemText(IDC_EDIT_LOGPATH,sPath);
		return;
	}
	else   
	{
		AfxMessageBox("无效的目录，请重新选择");   
		return;
	}
}

void ParameterSet::SaveConfigToFile()
{
	CString swritefile;
	//CONFIG
	WritePrivateProfileString("CONFIG","LogPath",m_CONFIG.sLogPath,m_CONFIG.sConfigPath);
	WritePrivateProfileString("CONFIG","CFGExcelPath",m_CONFIG.sCFGExcelPath,m_CONFIG.sConfigPath);
	WritePrivateProfileString("CONFIG","DefExcelPath",m_CONFIG.sDefaultExcelPath,m_CONFIG.sConfigPath);
	WritePrivateProfileString("CONFIG","FixtureID",m_CONFIG.sPREFIX,m_CONFIG.sConfigPath);
	swritefile.Format("%d",m_CONFIG.iPowerType);
	WritePrivateProfileString("CONFIG","PowerType",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%d",m_CONFIG.iBoardAdd);
	WritePrivateProfileString("CONFIG","BoardAdd",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%d",m_CONFIG.iGPIBAdd);
	WritePrivateProfileString("CONFIG","GPIBAdd",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%d",m_CONFIG.iCtrBOXComPort);
	WritePrivateProfileString("CONFIG","CtrBOXComPort",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%d",m_CONFIG.iPhonePort);
	WritePrivateProfileString("CONFIG","PHONEPORT",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%d", m_CONFIG.iPhonePort2);
	WritePrivateProfileString("CONFIG", "PHONEPORT2", swritefile, m_CONFIG.sConfigPath);
	swritefile.Format("%d",m_CONFIG.BaudRate);
	WritePrivateProfileString("CONFIG","BaudRate",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%d",m_CONFIG.bMES);
	WritePrivateProfileString("CONFIG","UseMES",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%s",m_CONFIG.sPO);
	WritePrivateProfileString("CONFIG","PO",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%s",m_CONFIG.sMESUser);
	WritePrivateProfileString("CONFIG","MesUserName",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%s",m_CONFIG.sMES_Line);
	WritePrivateProfileString("CONFIG","Line",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%s",m_CONFIG.sMESStationName);
	WritePrivateProfileString("CONFIG","MesStation",swritefile,m_CONFIG.sConfigPath);

	swritefile.Format("%d",m_CONFIG.iProject);
	WritePrivateProfileString("CONFIG","ProjectName",swritefile,m_CONFIG.sConfigPath);

	swritefile.Format("%s",m_CONFIG.sJigNum);
	WritePrivateProfileString("CONFIG","sJigNum",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%s",m_CONFIG.sFateName);
	WritePrivateProfileString("CONFIG","FateName",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%s",m_CONFIG.sJigNum);
	WritePrivateProfileString("CONFIG","FIXnumber",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%s",m_CONFIG.sMesStep);
	WritePrivateProfileString("CONFIG","MesStep",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%s",m_CONFIG.sMESURL);
	WritePrivateProfileString("CONFIG","URL",swritefile,m_CONFIG.sConfigPath);
	swritefile.Format("%d",m_CONFIG.Test_Selection);
	WritePrivateProfileString("CONFIG","TestSelection",swritefile,m_CONFIG.sConfigPath);
	return;
}

void ParameterSet::GetConfigFromFile()
{
	CString sReadfile;
	//CONFIG
	GetPrivateProfileString("CONFIG","LogPath","",m_CONFIG.sLogPath.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sLogPath.ReleaseBuffer();

	GetPrivateProfileString("CONFIG","CFGExcelPath","",m_CONFIG.sCFGExcelPath.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sCFGExcelPath.ReleaseBuffer();

	GetPrivateProfileString("CONFIG","DefExcelPath","",m_CONFIG.sDefaultExcelPath.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sDefaultExcelPath.ReleaseBuffer();

	GetPrivateProfileString("CONFIG","FixtureID","",m_CONFIG.sPREFIX.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sPREFIX.ReleaseBuffer();

	GetPrivateProfileString("CONFIG","TestSelection","0",sReadfile.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	sReadfile.ReleaseBuffer();
	m_CONFIG.Test_Selection =atoi(sReadfile);

	GetPrivateProfileString("CONFIG","PowerType","0",sReadfile.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	sReadfile.ReleaseBuffer();
	m_CONFIG.iPowerType=atoi(sReadfile);
	GetPrivateProfileString("CONFIG","BoardAdd","0",sReadfile.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	sReadfile.ReleaseBuffer();
	m_CONFIG.iBoardAdd=atoi(sReadfile);

	GetPrivateProfileString("CONFIG","GPIBAdd","0",sReadfile.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	sReadfile.ReleaseBuffer();
	m_CONFIG.iGPIBAdd=atoi(sReadfile);

	GetPrivateProfileString("CONFIG","CtrBOXComPort","0",sReadfile.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	sReadfile.ReleaseBuffer();
	m_CONFIG.iCtrBOXComPort=atoi(sReadfile);

	GetPrivateProfileString("CONFIG","PHONEPORT","0",sReadfile.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	sReadfile.ReleaseBuffer();
	m_CONFIG.iPhonePort=atoi(sReadfile);

	GetPrivateProfileString("CONFIG", "PHONEPORT2", "0", sReadfile.GetBuffer(MAX_PATH), MAX_PATH, m_CONFIG.sConfigPath);
	sReadfile.ReleaseBuffer();
	m_CONFIG.iPhonePort2 = atoi(sReadfile);

	GetPrivateProfileString("CONFIG","BaudRate","115200",sReadfile.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	sReadfile.ReleaseBuffer();
	m_CONFIG.BaudRate=atoi(sReadfile);

	GetPrivateProfileString("CONFIG","TOTAL","0",m_CONFIG.TotalNumber.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.TotalNumber.ReleaseBuffer();
	GetPrivateProfileString("CONFIG","PASS","0",m_CONFIG.PASSNumber.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.PASSNumber.ReleaseBuffer();
	GetPrivateProfileString("CONFIG","FAIL","0",m_CONFIG.FailNumber.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.FailNumber.ReleaseBuffer();
	GetPrivateProfileString("CONFIG","RATE","0%",m_CONFIG.Rate.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.Rate.ReleaseBuffer();

	GetPrivateProfileString("CONFIG","UseMES","0",sReadfile.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	sReadfile.ReleaseBuffer();
	m_CONFIG.bMES=atoi(sReadfile);

	GetPrivateProfileString("CONFIG","PO","0",m_CONFIG.sPO.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sPO.ReleaseBuffer();
	GetPrivateProfileString("CONFIG","MesUserName","0",m_CONFIG.sMESUser.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sMESUser.ReleaseBuffer();
	GetPrivateProfileString("CONFIG","Line","0",m_CONFIG.sMES_Line.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sMES_Line.ReleaseBuffer();
	GetPrivateProfileString("CONFIG","MesStation","0",m_CONFIG.sMESStationName.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sMESStationName.ReleaseBuffer();

	GetPrivateProfileString("CONFIG", "ProjectName", "0", sReadfile.GetBuffer(MAX_PATH), MAX_PATH, m_CONFIG.sConfigPath);
	sReadfile.ReleaseBuffer();
	m_CONFIG.iProject = atoi(sReadfile);

	GetPrivateProfileString("CONFIG","sJigNum","0",m_CONFIG.sJigNum.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sJigNum.ReleaseBuffer();
	GetPrivateProfileString("CONFIG","FateName","0",m_CONFIG.sFateName.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sFateName.ReleaseBuffer();
	GetPrivateProfileString("CONFIG","FIXnumber","0",m_CONFIG.sJigNum.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sJigNum.ReleaseBuffer();
	GetPrivateProfileString("CONFIG","MesStep","0",m_CONFIG.sMesStep.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sMesStep.ReleaseBuffer();
	GetPrivateProfileString("CONFIG","URL","0",m_CONFIG.sMESURL.GetBuffer(MAX_PATH),MAX_PATH,m_CONFIG.sConfigPath);
	m_CONFIG.sMESURL.ReleaseBuffer();
	return;
}

void ParameterSet::PutConfigSetToPage()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_LOGPATH,m_CONFIG.sLogPath);
	SetDlgItemText(IDC_EDIT_CFGIN,m_CONFIG.sCFGExcelPath);
	SetDlgItemText(IDC_EDIT_Default,m_CONFIG.sDefaultExcelPath);
	SetDlgItemText(IDC_EDIT_PREFIX,m_CONFIG.sPREFIX);
	m_ComboBox_PowerType.SetCurSel(m_CONFIG.iPowerType);
	m_ComboBox_BoardADD.SetCurSel(m_CONFIG.iBoardAdd);
	m_ComboBox_GPIBAdd.SetCurSel(m_CONFIG.iGPIBAdd);
	m_ComboBox_PHONEPORT.SetCurSel(m_CONFIG.iPhonePort);
	m_ComboBox_PHONEPORT2.SetCurSel(m_CONFIG.iPhonePort2);
	m_ComboBox_CONTROLBOXPORT.SetCurSel(m_CONFIG.iCtrBOXComPort);
	m_CHECK_Current.SetCheck(m_TestItem.bcurrent);
	m_CHECT_SN.SetCheck(m_TestItem.bwritesn);
	m_CHECK_Function.SetCheck(m_TestItem.bFunction);
	m_CHECT_GPS.SetCheck(m_TestItem.bgps);
	m_CHECT_CREG.SetCheck(m_TestItem.bcreg);
	m_CHECT_CGREG.SetCheck(m_TestItem.bcgreg);
	m_CHECT_WIFI.SetCheck(m_TestItem.bwifi);
	m_CHECT_Light_sensitive.SetCheck(m_TestItem.bLight_sensitive);
	m_Write_Fixture_ID.SetCheck(m_TestItem.bWrite_Fixture_ID);
	//m_TestItem.breadsn
	//m_CHECT_VERSION.SetCheck(m_TestItem.bCheckVersion);
	m_Test_Selection.SetCurSel(m_CONFIG.Test_Selection);
	m_CHECK_IMEI.SetCheck(m_TestItem.bFlag);
	m_CHECK_SIM.SetCheck(m_TestItem.bSIM);
	m_CHECK_CSQ.SetCheck(m_TestItem.bCSQ);
	m_CHECK_MES.SetCheck(m_CONFIG.bMES);
	OnBnClickedCheck1();
	SetDlgItemText(IDC_EDIT_MesUserName,m_CONFIG.sMESUser);
	SetDlgItemText(IDC_EDIT_MES_Line,m_CONFIG.sMES_Line);
	SetDlgItemText(IDC_EDIT_MesStep,m_CONFIG.sMESStationName);
	 m_ComboBox_Pro.SetCurSel(m_CONFIG.iProject);
	SetDlgItemText(IDC_EDIT_PO,m_CONFIG.sPO);
	SetDlgItemText(IDC_EDIT_FIXTURENUMBER,m_CONFIG.sJigNum);
	SetDlgItemText(IDC_EDIT_MES_URL,m_CONFIG.sMESURL);

	return;
}

void ParameterSet::OnBnClickedCheck1() {
	if (m_CHECK_MES.GetCheck()) {
		GetDlgItem(IDC_EDIT_MesUserName)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MES_Line)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MesStep)->EnableWindow(TRUE);
		//GetDlgItem(IDC_EDIT_ProjectName)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FIXTURENUMBER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MES_URL)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_EDIT_MesUserName)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MES_Line)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MesStep)->EnableWindow(FALSE);
		//GetDlgItem(IDC_EDIT_ProjectName)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FIXTURENUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MES_URL)->EnableWindow(FALSE);
	}
}

void ParameterSet::OnCbnSelchangeComboTestSelection()
{
	m_CONFIG.Test_Selection = m_Test_Selection.GetCurSel();
	if (m_CONFIG.Test_Selection == HDM_COUPLING_TEST || m_CONFIG.Test_Selection == HANDLE_COUPLING_TEST ||
		m_CONFIG.Test_Selection == HANDLE_PCBA_MMI_TEST ) {
		GetDlgItem(IDC_COMBO_PHONEPORT2)->EnableWindow(FALSE);
	}
	else {
		GetDlgItem(IDC_COMBO_PHONEPORT2)->EnableWindow(TRUE);
	}
	return;
}

void ParameterSet::OnBnClickedButtonCfgin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPath="";
	CString FileName="",FileExt="",FileTitle="";
	if (strPath=="")
	{
		strPath=m_CONFIG.sCFGExcelPath;
	}
	else
	{
		strPath=strPath+"\\";
	}

	CFileDialog file(TRUE,NULL,strPath,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("TestListFile|*.xlsx|TestListFile|*.xls|"),AfxGetMainWnd());
	if(file.DoModal()==2)
	{
		return;
	}
	FileName=file.GetPathName(); 
	SetDlgItemText(IDC_EDIT_CFGIN,FileName);	
	m_CONFIG.sCFGExcelPath=FileName;
	if (!lpCPIN_TESTDlg->GetTestItem_EXCEL())
	{
		return;
	}
}

void ParameterSet::OnBnClickedButtonDefault()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPath="";
	CString FileName="",FileExt="",FileTitle="";

	if (strPath=="")
	{
		strPath=m_CONFIG.sDefaultExcelPath;
	}
	else
	{
		strPath=strPath+"\\";
	}

	CFileDialog file(TRUE,NULL,strPath,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("TestListFile|*.xlsx|TestListFile|*.xls|"),AfxGetMainWnd());
	if(file.DoModal()==2)
	{
		return;
	}
	FileName=file.GetPathName(); 
	SetDlgItemText(IDC_EDIT_Default,FileName);	
	m_CONFIG.sDefaultExcelPath=FileName;
	if (!lpCPIN_TESTDlg->GetTestItem_DefaultEXCEL())
	{
		return;
	}
}
