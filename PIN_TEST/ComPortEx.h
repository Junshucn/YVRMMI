#pragma once

//typedef enum RetCodetag
//{
//	Result_OK=0,					//0		SUCCESS
//	Result_ERROR,					//1		一般错误
//	ERROR_FILENOTEXISTED,			//13	文件不存在
//	ERROR_FILEOPENFAIL,				//14	打开文件失败
//	ERROR_FILETOOSMALL,				//15	文件长度不够
//	ERROR_FILEREADFAIL,				//16	读取文件失败
//	ERROR_OPENCOMFAIL,				//65	打开串口失败
//	ERROR_GETCOMSTATEFAIL,			//66	获取串口状态失败
//	ERROR_SETCOMSTATEFAIL,			//67	设置串口状态失败
//	ERROR_SETCOMTIMEOUTFAIL,		//68	设置串口超时状态失败
//	ERROR_SETCOMMASKFAIL,			//69	设置COMMASK失败
//	ERROR_SETUPCOMFAIL,				//70	SetupCom失败
//	ERROR_PURGECOMFAIL,				//71	PurgeCom失败
//	ERROR_COMNOTOPEN,				//72	串口未打开
//	ERROR_SERIALPURGE,				//73	PurgeCom失败
//	ERROR_CLEARCOMERRORFAIL,		//74	ClearComError失败
//	ERROR_READCOMFAIL,				//75	读取串口失败
//	ERROR_READNOCHAR,				//76	没有从串口读到值
//	ERROR_SERIALPORTWRITE,			//77	写串口失败
//	ERROR_SERIALWRITEDATA,			//78	未能向串口写入指定数量数据
//	ERROR_GETNUMFAIL,				//79	从串口接收到的数据中获取数字失败
//	ERROR_DIOEXCEPTION,				//80	DIO操作异常
//	ERROR_DIOINITFAIL,				//81	DIO初始化失败
//	
//}RetCode;
// ComPortEx

class ComPortEx : public CWnd
{
	DECLARE_DYNAMIC(ComPortEx)

public:
	ComPortEx();
	virtual ~ComPortEx();
	bool	CommInit(int comport,int baudrate);
	void	CommClose();
	bool	ReadData(BYTE* pBuffer,DWORD iNumberBytesToRead,DWORD* iNumberBytesRead,float Timeout_s);
	bool	WriteData(BYTE* pBuffer,DWORD iNumberBytesToWrite,DWORD* iNumberBytesWritten);
	bool	ComReadString(BYTE *pReadBuffer, DWORD iReadMaxLength, DWORD* piReadLength, DWORD n_Timeout);
	bool	ComWriteString(BYTE *pWriteBuffer,DWORD iWriteMaxLength);

	void	SetReadEvent(HANDLE hEvent);
	void	SetStopEvent(HANDLE hEvent);

	bool	SetDTRStatus(UINT dtrstatus);
	bool	SetRTSStatus(UINT rtsstatus);
	bool	Clear();
	bool	ComWriteRead_HEX(int ComPort,int baudrate, CString WriteData_HEX,CString &ReadData_HEX,CString &ERRCODE);
	bool	ComWriteRead_String(int ComPort,int baudrate,CString WriteData_String,CString &ReadData_String,CString &ERRCODE);
	bool	ComRead_HEX(int ComPort,int baudrate,CString &ReadData_HEX,CString &ERRCODE);


protected:
	DECLARE_MESSAGE_MAP()
	HANDLE	m_hCommPort;
	BOOL	m_bSyncComm;//是否是同步串口通信

	OVERLAPPED  m_olWriteOverlapped;//异步通信使用
	OVERLAPPED  m_olReadOverlapped;//异步通信使用

	HANDLE	m_hReadEvent;//异步通信使用
	HANDLE	m_hStopEvent;//异步通信使用
};


