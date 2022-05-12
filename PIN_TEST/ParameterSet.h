#pragma once
#include "afxwin.h"
#include "COMMON.h"
#include "MMI_TESTDlg.h"


// ParameterSet 对话框

class ParameterSet : public CDialogEx
{
	DECLARE_DYNAMIC(ParameterSet)

public:
	ParameterSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ParameterSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_ParameterSet };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonLogpath();

	CComboBox m_ComboBox_PowerType;
	CComboBox m_ComboBox_BoardADD;
	CComboBox m_ComboBox_GPIBAdd;
	CComboBox m_ComboBox_PHONEPORT;
	CComboBox m_ComboBox_PHONEPORT2;
	CComboBox m_ComboBox_CONTROLBOXPORT;
	virtual BOOL OnInitDialog();

	void SaveConfigToFile();
	void GetConfigFromFile();
	void PutConfigSetToPage();
	CComboBox m_COMB_BaudRate;
	CButton m_CHECK_MES;
	afx_msg void OnBnClickedCheck1();

	CButton m_CHECK_RESET;

	CButton m_CHECK_ShowDown;
	CButton m_CHECK_TESTFLAG;
	CButton m_CHECK_Current;
	CButton m_CHECT_SN;
	CButton m_CHECK_Function;
	CButton m_CHECT_GPS;
	CButton m_CHECT_CREG;
	CButton m_CHECT_CGREG;
	CButton m_CHECT_VERSION;
	CComboBox m_Test_Selection;
	CButton m_CHECT_WIFI;

	CButton m_CHECT_Light_sensitive;
	CButton m_Write_Fixture_ID;
	CButton m_CHECK_IMEI;
	CButton m_CHECK_SIM;
	CButton m_CHECK_CSQ;

	CEdit m_InputFixtureID;
	afx_msg void OnCbnSelchangeComboTestSelection();
	afx_msg void OnBnClickedButtonCfgin();
	afx_msg void OnBnClickedButtonDefault();
	CComboBox m_ComboBox_Pro;

};
