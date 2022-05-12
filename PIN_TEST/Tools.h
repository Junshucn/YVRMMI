#pragma  once
#include "stdafx.h"


#ifndef _COMMAN_TOOLS_H
#define _COMMAN_TOOLS_H


#define	 CSTR_LEFT	0
#define  CSTR_RIGHT	1

//»ñÈ¡¼ä¸ô×Ö·û´®
bool Tools_splitStr(CString inputCSTR, CString spliter, CStringArray& outputACSTR);
CString Tools_InttoCstr(int inputInt);

#endif