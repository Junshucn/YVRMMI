// PowerControl.cpp : 实现文件
//

#include "stdafx.h"
#include "MMI_TEST.h"
#include "PowerControl.h"

extern CPIN_TESTDlg*	lpCPIN_TESTDlg;
extern CONFIG			m_CONFIG;

GPIBOperat m_GPIBOperat;

// PowerControl

IMPLEMENT_DYNAMIC(PowerControl, CWnd)

PowerControl::PowerControl()
{

}

PowerControl::~PowerControl()
{
}


BEGIN_MESSAGE_MAP(PowerControl, CWnd)
END_MESSAGE_MAP()



// PowerControl 消息处理程序


bool PowerControl::OpenEqupment(CString &ERRCODE)
{
	CString Putchar="",sEquipmentName="";


	if(!m_GPIBOperat.OpenDevices(hDevroot,m_CONFIG.iBoardAdd,m_CONFIG.iGPIBAdd,sEquipmentName))
	{
		ERRCODE="Open Power Equipment Fail";
		m_GPIBOperat.CloseDevices(hDevroot);
		return false;
	}

	switch(m_CONFIG.iPowerType)
	{
	case Agilent:
		if (sEquipmentName.Find("Agilent")==-1 && sEquipmentName.Find("HEWLETT")==-1&& sEquipmentName.Find("663")==-1)
		{
			ERRCODE="Equipment Type Not Agilent";
			m_GPIBOperat.CloseDevices(hDevroot);
			return false;
		}
		break;

	case KEITHLEY:
		if (sEquipmentName.Find("KEITHLEY")==-1&& sEquipmentName.Find("230")==-1)
		{
			ERRCODE="Equipment Type Not KEITHLEY";
			m_GPIBOperat.CloseDevices(hDevroot);
			return false;
		}
		break;
	}
	return true;
}

void PowerControl::CloseEqupment()
{

	m_GPIBOperat.CloseDevices(hDevroot);

	return;
}

bool PowerControl::RST()
{

	if (!m_GPIBOperat.GPIBWrite(hDevroot,"*RST"))
	{
		return false;
	}
	return true;
}

bool PowerControl::OpenBattery(CString VOLT)
{
	CString Putchar="",sEquipmentName="";

	switch(m_CONFIG.iPowerType)
	{
	case Agilent:
		Putchar.Format("VOLT %s;*OPC?",VOLT);
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;

		Putchar.Format("CURR 3;*OPC?");
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;

		//Putchar.Format("SENS:CURR:RANG 0.02;*OPC?");//0.002A是2毫安2000微安
		//if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
		//	return false;
		//if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
		//	return false;

		Putchar.Format("OUTP ON;*OPC?");
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;
		break;

	case KEITHLEY:
		Putchar.Format("VOLT %s;*OPC?",VOLT);
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;
		Putchar.Format("CURR:LIMIT 2;*OPC?");
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;
		Putchar.Format("OUTP ON;*OPC?");
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;
		break;
	}

	return true;
}

bool PowerControl::CurrRangmA()
{
	CString Putchar="",sEquipmentName="",ERRINFO="";
	switch(m_CONFIG.iPowerType)
	{
	case Agilent:
		Putchar.Format("SENS:CURR:RANG 0.02;*OPC?");//0.002A是2毫安2000微安
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;
		break;

	case KEITHLEY:
		Putchar.Format("SENS:CURR:RANG 0.02;*OPC?");//0.002A是2毫安2000微安
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;
		break;
	}
	return true;
}

bool PowerControl::CloseBattery()
{
	CString Putchar="",EquipmentName="";

	Putchar.Format("OUTP OFF;*OPC?");
	if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
		return false;
	if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
		return false;

	return true;
}

bool PowerControl::PowerGetCurrent(float &Current)//mA
{
	CString sWriteChar="",EquipmentName="";
	CString GetBuffer="";
	GetBuffer="";
	sWriteChar.Format("MEAS:CURR?");
	if (!m_GPIBOperat.GPIBWrite(hDevroot,sWriteChar))
		return false;
	if (!m_GPIBOperat.GPIBRead(hDevroot,GetBuffer))
		return false;
	Current=atof(GetBuffer)*1000;
	return true;
}

bool PowerControl::PowerGetuACurrent(float &Current)//A
{
	CString sWriteChar="",EquipmentName="";
	CString GetBuffer="";
	GetBuffer="";
	sWriteChar.Format("MEAS:CURR?");
	if (!m_GPIBOperat.GPIBWrite(hDevroot,sWriteChar))
		return false;
	if (!m_GPIBOperat.GPIBRead(hDevroot,GetBuffer))
		return false;
	Current=atof(GetBuffer);
	return true;
}

bool PowerControl::OpenBatterymA(CString VOLT)
{
	CString Putchar="",sEquipmentName="";

	switch(m_CONFIG.iPowerType)
	{
	case Agilent:
		Putchar.Format("VOLT %s;*OPC?",VOLT);
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;

		Putchar.Format("CURR 3;*OPC?");
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;

		Putchar.Format("SENS:CURR:RANG 0.02;*OPC?");//0.002A是2毫安2000微安
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;

		Putchar.Format("OUTP ON;*OPC?");
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;
		break;

	case KEITHLEY:
		Putchar.Format("VOLT %s;*OPC?",VOLT);
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;
		Putchar.Format("CURR:LIMIT 2;*OPC?");
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;

		Putchar.Format("SENS:CURR:RANG 0.02;*OPC?");//0.002A是2毫安2000微安
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;

		Putchar.Format("OUTP ON;*OPC?");
		if (!m_GPIBOperat.GPIBWrite(hDevroot,Putchar))
			return false;
		if (!m_GPIBOperat.GPIBRead(hDevroot,Putchar))
			return false;
		break;
	}

	return true;
}

