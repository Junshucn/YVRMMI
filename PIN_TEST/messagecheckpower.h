#pragma once


// messagecheckpower 对话框

class messagecheckpower : public CDialogEx
{
	DECLARE_DYNAMIC(messagecheckpower)

public:
	messagecheckpower(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~messagecheckpower();

// 对话框数据
	enum { IDD = MESSAGE_CHECK_POWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
