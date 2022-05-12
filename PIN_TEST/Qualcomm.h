#pragma once

#include "include/QLib.h"
#include "include/QMSL_Core.h"
#include "include/QMSL.h"
#include "include/QMSL_EFS/QMSL_EFS.h"
#include "include/QMSL_Misc/QMSL_GSDI_Diag.h"
#include "include/Qlib_Defines.h"
//#include "QLib_SoftwareDownloadDefines.h"

class CQualcomm
{
public:
	CQualcomm(void);
	~CQualcomm(void);
	BOOL EFS(HANDLE hResourceContext);
	void  InitLibrary();
	bool ConnectToPhone();
	void DisconnectPhone();
	bool ReadSN(CString &OutPut);
	bool CHKChangeFTM(bool bFTM);
	bool GET_FTM_ONLINE_MODE(unsigned char *Ret_Mode,CString &ERRCODE);
	bool GetGPSRx(double &dGpsCno);
    bool ReadFlag(int NV_num,unsigned char &FlagResult);
	bool WriteFlag(int NV_num,bool iResult);
	bool m_cmd(char * pCmd, CString& strErrorInfo);
	bool DiagCommandCamera(CString &sOutPut);
	bool DiagCommandSend(unsigned char cCMD[],int iCMDSize,CString &sOutPut);
	bool DiagCommandGetMCU(CString &sOutPut);
	bool DiagFANTest(CString &sOutPut);
};

