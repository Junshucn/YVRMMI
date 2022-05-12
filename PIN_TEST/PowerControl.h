#pragma once
#include "GPIBOperat.h"
#include "COMMON.h"
#include "MMI_TESTDlg.h"

// PowerControl

class PowerControl : public CWnd
{
	DECLARE_DYNAMIC(PowerControl)

public:
	PowerControl();
	virtual ~PowerControl();


protected:
	DECLARE_MESSAGE_MAP()
	int hDevroot;
public:
	bool OpenEqupment(CString &ERRCODE);
	void CloseEqupment();
	bool RST();
	bool OpenBattery(CString VOLT);
	bool CurrRangmA();
	bool CloseBattery();
	bool PowerGetCurrent(float &Current);
	bool PowerGetuACurrent(float &Current);
	bool OpenBatterymA(CString VOLT);
};


