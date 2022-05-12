// WebServiceUpdateData_DLL.h : main header file for the WebServiceUpdateData_DLL DLL
//

#pragma once

#ifdef	DLL2_API
#else  
#define DLL2_API		extern "C" _declspec(dllimport)  ////函数隐式调用 加extern "C"为避免名字改编，不同编译器调用失败   
#endif

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

//bindSN:上传绑定的号段
DLL2_API BOOL _stdcall _updateBindSN(CString IpAddress,CString mainBoardSN,CString bindSN,CString workOrder,CString &sOutput);
DLL2_API BOOL _stdcall _isUseBindSN(CString IpAddress,CString bindSN,CString &sOutput);


