
#pragma once

#ifdef	DLL_API
#else  
#define DLL_API		extern "C" _declspec(dllimport)  ////函数隐式调用 加extern "C"为避免名字改编，不同编译器调用失败   
#endif

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif


DLL_API BOOL _stdcall _testCheck(CString IpAddress,CString SN,CString StationName,CString workOrder,CString SoftVer,CString Line,CString MES_USER,CString FateName,CString JIG_Num,CString &sOutput);

DLL_API BOOL _stdcall _testResult(CString IpAddress,CString SN,CString StationName,CString workOrder,CString SoftVer,CString Line,CString MES_USER,CString FateName,CString JIG_Num,CString Result,CString ERRCODE,CString &sOutput);

DLL_API BOOL _stdcall _LPcheck(CString IpAddress,CString PhoneSN,CString NetSN,CString BSN,CString StationName,CString workOrder,CString SoftVer,CString Line,CString MES_USER,CString FateName,CString JIG_Num,CString &sOutput);

DLL_API BOOL _stdcall _LPresult(CString IpAddress,CString PhoneSN,CString NetSN,CString BSN,CString StationName,CString workOrder,CString SoftVer,CString Line,CString MES_USER,CString FateName,CString JIG_Num,CString FwidevineKey,CString OEMKEY,CString EMMC_ID,CString esn_hex_8, CString esn_dec_11 ,CString meid_dec_18, CString meid_dec_19,CString CPLC,CString Result,CString ERRCODE,CString &sOutput);

DLL_API BOOL _stdcall _LPresult2(CString IpAddress,CString PhoneSN,CString NetSN,CString BSN,CString StationName,CString workOrder,CString SoftVer,CString Line,CString MES_USER,CString FateName,CString JIG_Num,CString FwidevineKey,CString OEMKEY,CString EMMC_ID,CString esn_hex_8, CString esn_dec_11 ,CString meid_dec_18, CString meid_dec_19,CString CPLC,CString Imsi,CString Iccid,CString Dzsn,CString Result,CString ERRCODE,CString &sOutput);

DLL_API BOOL _stdcall _MCcheck(CString IpAddress,CString PhoneSN,CString NetSN,CString SN,CString StationName,CString workOrder,CString SoftVer,CString Line,CString MES_USER,CString FateName,CString JIG_Num,CString &sOutput);

DLL_API BOOL _stdcall _MCcheck2(CString IpAddress,CString PhoneSN,CString NetSN,CString SN,CString IMEI1,CString StationName,CString workOrder,CString SoftVer,CString Line,CString MES_USER,CString FateName,CString JIG_Num,CString &sOutput);

DLL_API BOOL _stdcall _MCresult(CString IpAddress,CString PhoneSN,CString NetSN,CString SN,CString StationName,CString Line,CString workOrder,CString SoftVer,CString MES_USER,CString FateName,CString JIG_Num,CString Result,CString ERRCODE,CString &sOutput);


