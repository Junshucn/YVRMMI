#pragma once
#include "MMI_TESTDlg.h"

// GPIBOperat

class GPIBOperat : public CWnd
{
	DECLARE_DYNAMIC(GPIBOperat)

public:
	GPIBOperat();
	virtual ~GPIBOperat();

	bool OpenDevices(int &hDevroot,int iBoard,int iAddress,CString &EquipmentName);
	bool GPIBWrite(int hDevroot,CString sWriteBuffer);
	bool GPIBRead(int hDevroot,CString &GetBuffer);
	bool CloseDevices(int hDevroot);


protected:
	DECLARE_MESSAGE_MAP()
};


