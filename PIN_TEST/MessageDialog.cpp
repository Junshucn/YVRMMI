// MessageDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "MessageDialog.h"
#include "afxdialogex.h"


// MessageDialog 对话框
extern CPIN_TESTDlg*	lpCPIN_TESTDlg;
extern PHONE			m_PHONE;
extern CONFIG			m_CONFIG;
extern MessageDialog    m_MessageDialog;
extern MYWORK			m_MYWORK;


MessageDialog* lpMessageDialog;
int g_iStatus[NUM_BUTTON_D1] = { 0 };

IMPLEMENT_DYNAMIC(MessageDialog, CDialogEx)

MessageDialog::MessageDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(MessageDialog::IDD, pParent)
{
	cfont_showmsg.CreatePointFont(180, "Verdana");
	FButton_startfont.CreatePointFont(120, "Verdana");
}

MessageDialog::~MessageDialog() 
{
}

void MessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MessageDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &MessageDialog::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDCANCEL, &MessageDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// MessageDialog 消息处理程序

BOOL MessageDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	lpMessageDialog = &m_MessageDialog;//窗口寻址
	SetBackgroundColor(RGB(176, 196, 222), 1);
	GetDlgItem(IDC_STATIC_Message)->SetFont(&cfont_showmsg);
	GetDlgItem(IDC_STATIC_AX)->SetFont(&cfont_showmsg);
	GetDlgItem(IDC_STATIC_AX2)->SetFont(&cfont_showmsg);
	GetDlgItem(IDC_STATIC_BY)->SetFont(&cfont_showmsg);
	GetDlgItem(IDC_STATIC_BY2)->SetFont(&cfont_showmsg);
	GetDlgItem(IDC_STATIC_JS)->SetFont(&cfont_showmsg);
	GetDlgItem(IDC_STATIC_JS2)->SetFont(&cfont_showmsg);
	GetDlgItem(IDC_STATIC_HOME)->SetFont(&cfont_showmsg);
	GetDlgItem(IDC_STATIC_HOME2)->SetFont(&cfont_showmsg);
	GetDlgItem(IDC_STATIC_HAND)->SetFont(&cfont_showmsg);
	GetDlgItem(IDC_STATIC_HAND2)->SetFont(&cfont_showmsg);
	IsMESSAGE = TRUE;
	if (KEYTEST == nTESTID) {
		SetDlgItemText(IDC_STATIC_Message, "请20秒内完成5个按键：");
	}
	if (TOUCHTEST == nTESTID) {
		SetDlgItemText(IDC_STATIC_Message, "请20秒内完成4个触摸键 ：");
		SetDlgItemText(IDC_STATIC_HOME, "TRIGGER");
		GetDlgItem(IDC_STATIC_HAND)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_HAND2)->ShowWindow(SW_HIDE);
	}
	//((CMFCButton*)GetDlgItem(ID))->SetTextColor(RGB(0, 0, 205));
	//((CMFCButton*)GetDlgItem(ID))->SetFaceColor(RGB(176, 196, 222), 1);	
	Thread_TestStart_START();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

static UINT KeyTest_THREAD(LPVOID LPARAM){
	lpMessageDialog->Key_Test_All();
	Sleep(500);
	for (int i = 0; i < NUM_BUTTON_D1; i++) g_iStatus[i] = 0; //将测试状态回归
	if (lpMessageDialog->IsMESSAGE) lpMessageDialog->ExitShow();
	return 0;
}

void MessageDialog::Thread_TestStart_START()
{
	AfxBeginThread(KeyTest_THREAD, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
}

bool MessageDialog::Key_Test()
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------KEY TEST---------------------------");
	clock_t start, stop;
	float fScanTime, Timeout_s = 20;
	start = clock();//计时开始毫秒为单位
	CString sReceivedata = "", sStrDebug = "";
	CString sKeyValue = "TEST_BUTTON";
	CString sBUTCMD[NUM_BUTTON_D1] = { "test pcba TEST_BUTTON_AX","test pcba TEST_BUTTON_BY" ,"test pcba TEST_BUTTON_JOYSTICK","test pcba TEST_BUTTON_HOME","test pcba TEST_BUTTON_HAND" };
	CString sPRESSED[NUM_BUTTON_D1] = { "AX key pressed","BY key pressed","JOYSTICK key pressed","HOME key pressed","HAND key pressed" };
	CString sRELEASE[NUM_BUTTON_D1] = { "AX key release","BY key release","JOYSTICK key release","HOME key release","HAND key release" };
	UINT CTLSTC[NUM_BUTTON_D1] = { IDC_STATIC_AX2 ,IDC_STATIC_BY2,IDC_STATIC_JS2,IDC_STATIC_HOME2,IDC_STATIC_HAND2 };
	for (int i=0;i< NUM_BUTTON_D1;i++) {
		start = clock();//计时开始毫秒为单位
		while (IsMESSAGE) {
			stop = clock();
			fScanTime = (float)(stop - start) / CLOCKS_PER_SEC;
			if (fScanTime > Timeout_s) {
				goto FAIL;
			}
			if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sBUTCMD[i], sReceivedata, sKeyValue, 2, sStrDebug)) goto FAIL;
			if (g_iStatus[i] == 0) {
				if (-1 != sReceivedata.Find(sPRESSED[i])) {
					g_iStatus[i] = 1;
				}
			}
			if (g_iStatus[i] == 1) {
				if (-1 != sReceivedata.Find(sRELEASE[i])) {
					g_iStatus[i] = 2;
					SetDlgItemText(CTLSTC[i], "PASS");
					break;
				}
			}
	    }		
	}
	//for (int i = 0; i < NUM_BUTTON_D1; i++) g_iStatus[i] = 0;
	nRet = IDOK;
	return true;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
    nRet = IDNO;
	return FALSE;
}

bool MessageDialog::Touch_Test()
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------TOUCH TEST---------------------------");
	clock_t start, stop;
	float fScanTime, Timeout_s = 20;
	start = clock();//计时开始毫秒为单位
	CString sReceivedata = "", sStrDebug = "";
	CString sKeyValue = "TEST_TOUCH";
	CString sTOUCHCMD[NUM_TOUCH_D1] = { "test pcba TEST_TOUCH_AX","test pcba TEST_TOUCH_BY" ,"test pcba TEST_TOUCH_JOYSTICK","test pcba TEST_TOUCH_TRIGGER"};
	CString sTouched[NUM_TOUCH_D1] = { "AX touch touched","BY touch touched","JOYSTICK touch touched","TRIGGER touch touched"};
	CString sRELEASE[NUM_TOUCH_D1] = { "AX touch release","BY touch release","JOYSTICK touch release","TRIGGER touch release"};
	UINT CTLSTC[NUM_TOUCH_D1] = { IDC_STATIC_AX2 ,IDC_STATIC_BY2,IDC_STATIC_JS2,IDC_STATIC_HOME2 };
	for (int i = 0; i < NUM_TOUCH_D1; i++) {
		start = clock();//计时开始毫秒为单位
		while (IsMESSAGE) {
			stop = clock();
			fScanTime = (float)(stop - start) / CLOCKS_PER_SEC;
			if (fScanTime > Timeout_s) goto FAIL;	
			if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sTOUCHCMD[i], sReceivedata, sKeyValue, 2, sStrDebug)) goto FAIL;
			if (g_iStatus[i] == 0) {
				if (-1 != sReceivedata.Find(sTouched[i])) {
					g_iStatus[i] = 1;
				}
			}
			if (g_iStatus[i] == 1) {
				if (-1 != sReceivedata.Find(sRELEASE[i])) {
					g_iStatus[i] = 2;
					SetDlgItemText(CTLSTC[i], "PASS");
					break;
				}
			}
		}
	}
	nRet = IDOK;
	return true;
FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	nRet = IDNO;
	return FALSE;
}

bool MessageDialog::Key_Test_All()
{
	lpCPIN_TESTDlg->OutPutDebugLog("-----------------------------KEY AND TOUCH TEST All---------------------------");
	clock_t start, stop;
	float fScanTime, Timeout_s = 25;
	start = clock();//计时开始毫秒为单位
	CString sReceivedata = "", sStrDebug = "", sAllBTNMask="";
	CString sATcmd = "test pcba TEST_ALL_BUTTON", sKeyValue = "TEST_ALL_BUTTON";
	UINT CTLSTC[NUM_BUTTON_D1] = { IDC_STATIC_AX2 ,IDC_STATIC_BY2,IDC_STATIC_JS2,IDC_STATIC_HOME2,IDC_STATIC_HAND2 };
	int index1 = 0;
	while (IsMESSAGE) {
		stop = clock();
		fScanTime = (float)(stop - start) / CLOCKS_PER_SEC;
		if (fScanTime > Timeout_s) goto FAIL;
		if (!m_PHONE.KeySendAtCommond(m_CONFIG.iPhonePort, m_CONFIG.BaudRate, sATcmd, sReceivedata, sKeyValue, 2, sStrDebug)) goto FAIL;
		index1 = sReceivedata.Find("0x") + 2;
		sStrDebug = sReceivedata.Right(sReceivedata.GetLength() - index1).Trim();
		TCHAR NV_Data[512] = { 0 };
		CString sTemp = "";
		memcpy(NV_Data, sStrDebug, sStrDebug.GetLength());
		for (int i = 0; i < sStrDebug.GetLength(); i++) {
			if (((NV_Data[i] >= '0' && NV_Data[i] <= '9') || (NV_Data[i] >= 'a' && NV_Data[i] <= 'f') || (NV_Data[i] >= 'A' && NV_Data[i] <= 'F'))) {
				sTemp = sTemp + NV_Data[i];
			}
			else {
				break;
			}
		}
		m_MYWORK.HexToBsStr(sTemp, sAllBTNMask, 9);
		char g_charStatus[NUM_BUTTON_D1] = {};
		if (KEYTEST == lpMessageDialog->nTESTID) {
			g_charStatus[AX_BUTTON_D1] = sAllBTNMask.GetAt(MASK_BUTTONS_AX_D1);
			g_charStatus[BY_BUTTON_D1] = sAllBTNMask.GetAt(MASK_BUTTONS_BY_D1);
			g_charStatus[HOME_BUTTON_D1] = sAllBTNMask.GetAt(MASK_BUTTONS_HOME_D1);
			g_charStatus[HAND_BUTTON_D1] = sAllBTNMask.GetAt(MASK_BUTTONS_HAND_D1);
			g_charStatus[JS_BUTTON_D1] = sAllBTNMask.GetAt(MASK_BUTTONS_JOYSTICK_D1);
			for (int i = 0; i < NUM_BUTTON_D1; i++) {//将按钮的状态都查询一遍
				if (g_iStatus[i] == 2) continue;//已经PASS的不再查询直接continue
				if (g_charStatus[i] == '0') g_iStatus[i] = 1;
				if (g_iStatus[i] == 1) {
					if (g_charStatus[i] == '1') {
						g_iStatus[i] = 2;
						SetDlgItemText(CTLSTC[i], "PASS");
					}
				}
			}//全PASS就Return
			if (g_iStatus[AX_BUTTON_D1] == 2 && g_iStatus[BY_BUTTON_D1] == 2 &&
				g_iStatus[HOME_BUTTON_D1] == 2 && g_iStatus[HAND_BUTTON_D1] == 2 &&
				g_iStatus[JS_BUTTON_D1] == 2
				) {
				nRet = IDOK;
				return true;
			}
		}
		if (TOUCHTEST == lpMessageDialog->nTESTID) {
			g_charStatus[AX_TOUCH_D1] = sAllBTNMask.GetAt(MASK_TOUCHES_AX_D1);
			g_charStatus[BY_TOUCH_D1] = sAllBTNMask.GetAt(MASK_TOUCHES_BY_D1);
			g_charStatus[TRIGGER_TOUCH_D1] = sAllBTNMask.GetAt(MASK_TOUCHES_TRIGGER_D1);
			g_charStatus[JS_TOUCH_D1] = sAllBTNMask.GetAt(MASK_TOUCHES_JOYSTICK_D1);
			for (int i = 0; i < NUM_TOUCH_D1; i++) {//将按钮的状态都查询一遍
				if (g_iStatus[i] == 2) continue;//已经PASS的不再查询直接continue
				if (g_charStatus[i] == '0') g_iStatus[i] = 1;
				if (g_iStatus[i] == 1) {
					if (g_charStatus[i] == '1') {
						g_iStatus[i] = 2;
						SetDlgItemText(CTLSTC[i], "PASS");
					}
				}
			}//全PASS就Return
			if (g_iStatus[AX_TOUCH_D1] == 2 && g_iStatus[BY_TOUCH_D1] == 2 &&
				g_iStatus[TRIGGER_TOUCH_D1] == 2 && g_iStatus[JS_TOUCH_D1] == 2 
				) {
				nRet = IDOK;
				return true;
			}
		}
	}

FAIL:
	lpCPIN_TESTDlg->OutPutDebugLog(sStrDebug);
	nRet = IDNO;
	return FALSE;
}



HBRUSH MessageDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_AX2) {
		switch (g_iStatus[AX_BUTTON_D1]) {
		case	2:
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)::CreateSolidBrush(RGB(0, 255, 0));
			break;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_BY2) {
		switch (g_iStatus[BY_BUTTON_D1]) {
		case	2:
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)::CreateSolidBrush(RGB(0, 255, 0));
			break;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_JS2) {
		switch (g_iStatus[JS_BUTTON_D1]) {
		case	2:
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)::CreateSolidBrush(RGB(0, 255, 0));
			break;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_HOME2) {
		switch (g_iStatus[HOME_BUTTON_D1]) {
		case	2:
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)::CreateSolidBrush(RGB(0, 255, 0));
			break;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_HAND2) {
		switch (g_iStatus[HAND_BUTTON_D1]) {
		case	2:
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)::CreateSolidBrush(RGB(0, 255, 0));
			break;
		}
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void MessageDialog::OnBnClickedOk()
{
	//EndDialog(IDOK);    //关闭窗口 
	//CDialog::OnClose();
	//Sleep(100);
	//Sleep(1000);
	//CDialogEx::OnOK();
}

void MessageDialog::OnBnClickedCancel()
{
	IsMESSAGE = FALSE;
	Sleep(100);
	CDialogEx::OnCancel();
}

void MessageDialog::ExitShow()
{
	// TODO: 在此添加控件通知处理程序代码
	IsMESSAGE = FALSE;
	Sleep(200);
	EndDialog(nRet);    //关闭窗口 
	CDialogEx::OnClose();

}
