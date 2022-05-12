#include "Tools.h"
//静态工具代码函数
#include "COMMON.h"
#include "stdafx.h"

bool Tools_splitStr(CString inputCSTR, CString spliter, CStringArray& outputACSTR)
{
	int iSpliterLength = spliter.GetLength();
	if (inputCSTR.IsEmpty())
	{
		return false;
	}
	CStringArray ca;
	ca.RemoveAll();
	int index = 0;
	while ((index = inputCSTR.Find(spliter)) != -1)//got it
	{
		ca.Add(inputCSTR.Mid(0, index));
		inputCSTR = inputCSTR.Mid(index + iSpliterLength);

	}
	if (!inputCSTR.IsEmpty())
	{
		ca.Add(inputCSTR);
	}
	outputACSTR.Copy(ca);
	return true;
}

CString Tools_InttoCstr(int inputInt) 
{
	CString sRes = "";
	sRes.Format("%d", inputInt);
	return sRes;
}