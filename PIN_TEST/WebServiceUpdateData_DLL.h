// WebServiceUpdateData_DLL.h : main header file for the WebServiceUpdateData_DLL DLL
//

#pragma once

#ifdef	DLL2_API
#else  
#define DLL2_API		extern "C" _declspec(dllimport)  ////������ʽ���� ��extern "C"Ϊ�������ָı࣬��ͬ����������ʧ��   
#endif

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

//bindSN:�ϴ��󶨵ĺŶ�
DLL2_API BOOL _stdcall _updateBindSN(CString IpAddress,CString mainBoardSN,CString bindSN,CString workOrder,CString &sOutput);
DLL2_API BOOL _stdcall _isUseBindSN(CString IpAddress,CString bindSN,CString &sOutput);


