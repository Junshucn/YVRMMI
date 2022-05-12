#pragma once

#include <tchar.h>

#pragma warning(push, 3 )
#include <vector>
#include <map>
#pragma warning(pop )

using namespace std;


#define DEV_NAME_MAX_LEN 256

typedef enum _DevType
{
	TYPE_DT_PORT,		// Serial port
	TYPE_DT_MODEM,		// Modem
}SCAN_DEV_TYPE;

typedef enum _tagDevType
{
	DT_UNKNOWN,	    // Unknown type
	DT_PORT,		// Serial port
	DT_MODEM,		// Modem
	DT_MAX
}E_DEV_TYPE;

typedef struct _tagDevInfo
{
	unsigned int nPortNum;				    // Port number
	TCHAR szPortName[DEV_NAME_MAX_LEN];		// Short name for the port,in format like "COM X"
	TCHAR szFriendlyName[DEV_NAME_MAX_LEN];	// Friendly name for the port device
	TCHAR szDescription[DEV_NAME_MAX_LEN];	// Description of the port
	E_DEV_TYPE nType;				            // Port type
	unsigned int  bExist;
}T_DEV_INFO;

class CDevHound  
{
public:
	CDevHound();
	virtual ~CDevHound();
    void SetReceiver(DWORD dwHandle, DWORD dwMsgID, BOOL bThread = TRUE);
	void Start();
	void Stop();
	bool GetPortName(unsigned int nPort,TCHAR szName[DEV_NAME_MAX_LEN]);
	int  ScanPort(vector<T_DEV_INFO> &vPortInfo,SCAN_DEV_TYPE DEV_TYPE);
	int  ScanModemPort(vector<T_DEV_INFO> &vPortInfo);

protected:
	static DWORD WINAPI GetHoundThreadProc(LPVOID lpParam);
	DWORD WINAPI HoundThreadProc();	
	bool  CheckPortInfo(T_DEV_INFO& epi);
	bool  InsertPortInfo(T_DEV_INFO& epi,bool bInit);	
	E_DEV_TYPE CheckDeviceClass(TCHAR* lpszClass);
	void  SendToUplayer(T_DEV_INFO& epi);
	void  ClearState();
	bool  InsertPortInfo(T_DEV_INFO& epi,vector<T_DEV_INFO> &vPortInfo);	

private:
	vector<T_DEV_INFO> m_vPortInfo;
	map<unsigned int,T_DEV_INFO> m_mPortInfo;
	HWND   m_hWnd;
	DWORD  m_dwThreadID;
	DWORD  m_dwMsgID;
	
	HANDLE m_hHundThread;
	DWORD  m_dwHundThreadID;
	HANDLE m_hHundEvent;
	BOOL   m_bWork;
};
