#pragma once
#include "MMI_TESTDlg.h"
#include "MYWORK.h"

// MessageDialog 对话框
enum {
	KEYTEST = 0,
	TOUCHTEST
};

enum {
	AX_BUTTON_D1=0,
	BY_BUTTON_D1,
	JS_BUTTON_D1,
	HOME_BUTTON_D1,
	HAND_BUTTON_D1,
	NUM_BUTTON_D1,
};

enum {
	AX_TOUCH_D1 = 0,
	BY_TOUCH_D1,
	JS_TOUCH_D1,
	TRIGGER_TOUCH_D1,
	NUM_TOUCH_D1,
};

enum {
	MASK_TOUCHES_JOYSTICK_D1,
	MASK_TOUCHES_TRIGGER_D1,
	MASK_TOUCHES_BY_D1,
	MASK_TOUCHES_AX_D1,
	MASK_BUTTONS_JOYSTICK_D1,
	MASK_BUTTONS_HAND_D1,
	MASK_BUTTONS_HOME_D1,
	MASK_BUTTONS_BY_D1,
	MASK_BUTTONS_AX_D1,
	NUM_MASK_BUTTONS_D1,
};

class MessageDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MessageDialog)

public:
	MessageDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MessageDialog();

// 对话框数据
	enum { IDD = IDD_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void Thread_TestStart_START();
	bool Key_Test();
	bool Key_Test_All();
	bool Touch_Test();
	UINT nRet;
	UINT nTESTID;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCancel();
	void ExitShow();
	CFont cfont_showmsg;
	CFont FButton_startfont;
	BOOL IsMESSAGE;
};
