#pragma once


// messagecheckpower �Ի���

class messagecheckpower : public CDialogEx
{
	DECLARE_DYNAMIC(messagecheckpower)

public:
	messagecheckpower(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~messagecheckpower();

// �Ի�������
	enum { IDD = MESSAGE_CHECK_POWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
