
// MMI_TESTDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "FORM_DEBUGLOG.h"
#include "FORM_TESTLOG.h"
#include "COMMON.h"
#include "afxbutton.h"
#include "ParameterSet.h"
#include "ExcelOperate_DLL.h"
#include "MYWORK.h"
#include "MdbControl.h"
#include "DevHound.h"
#include "tlhelp32.h"

#define WM_DEV_HOUND			WM_USER + 203
#define	WM_INIT_FOCUS_SCANER	WM_USER + 204
// CPIN_TESTDlg 对话框
class CPIN_TESTDlg : public CDialogEx
{
// 构造
public:
	CPIN_TESTDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PIN_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CFont BTN_START;
	CFont FONT_TIMER;
	CFont RunStatefont;
	CFont FONT_SN;
	CFont FONT_STATION;

	CDialogEx* m_pDialog_Log[2];
	HBRUSH hbr_TOTAL;
	HBRUSH hbr_PASS;
	HBRUSH hbr_FAIL;
	HBRUSH hbr_RATE;

	HBRUSH hbr_STATE_PASS;
	HBRUSH hbr_STATE_FAIL;
	HBRUSH hbr_STATE_RUN;
	HBRUSH hbr_STATE_READY;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CTabCtrl m_Table;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	void InitMessage();
	void GetSFISConfig();
	int	 RunTime;
	void ShowState(int State);
	void ShowProgress(int iPost);
	void ResultStatistics(int ERRCODE);
	CString GetExeFilePath();
	void UpdateWindow();
	void  OutPutTestLog(TestItem iTestItem,CString TestValue,bool bJudgement);
	void  ClearTestLog();
	void  OutPutDebugLog(CString sLog);
	void  ClearDebugLog();
	void  ShowSN(CString SN);
	void  ShowInfor(CString sInfor);
	void  ShowIMEI(CString IMEI);
	void  SaveOntimeDebugLog(CString log,CString WriteType);
	bool  SaveTestLog(CString SN,int iErrCode);
	bool  GetTestItem_EXCEL();
	bool  GetTestItem_DefaultEXCEL();
	bool  ReadDatebase(CString TableName,CString FieldName,CString &ReadData);
	void  enumUsbPort(ScanPort *lpScanPort);
	bool  GetRandomNumber(int Len,CString &RandomNumber);
	bool  GetRandomNumber_NUM(int Len,CString &RandomNumber);
	//void  GetDataBySpecificChar(CString inputstr,CArray<CString,CString> &OutputData,CString _Delim);
	bool  ReadColumn(CString TableName,CString FieldName,CArray<CString,CString>&ReadData);
	bool  MACSPELLRULE(CString sMAC);
	BOOL  KillProcessFromName(CString strProcessName);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CMFCButton m_BTN_START;
	CProgressCtrl m_Progress;
	afx_msg void Menu_Set();
	afx_msg void OnBnClickedMfcbuttonStart();
	CMFCButton m_BTN_CLEAR;
	afx_msg void OnBnClickedMfcbuttonClear();
	afx_msg LRESULT OnOndevhound(WPARAM wParam, LPARAM lParam);
	CEdit m_InputSn;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit m_InputPhoneSn;
//	CEdit m_InputFixtureID;
	void SetFocus();
	afx_msg void OnEnChangeInputphonesn();
	bool  GetMdbParameter();
	bool getStandardTestItemConfig(CString tableName, CString keyName, StandardTestItem& _item);
};
