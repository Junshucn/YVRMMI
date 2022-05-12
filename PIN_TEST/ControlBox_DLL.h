// ControlBox_DLL.h : ControlBox_DLL DLL ����ͷ�ļ�
//

#pragma once

#ifdef	DLL_API
#else  
#define DLL_API		extern "C" _declspec(dllimport)  
#endif


/***************************************************************
*	���˿ڿ��ƺ�	��USB
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*
***************************************************************/
DLL_API BOOL _stdcall ControlBox_OpenUsb(int PortNum,char* ERRCODE);

/***************************************************************
*	���˿ڿ��ƺ�	�Ͽ�USB
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*
***************************************************************/
DLL_API BOOL _stdcall ControlBox_CloseUsb(int PortNum,char* ERRCODE);

/***************************************************************
*	���˿ڿ��ƺ�	��VBAT
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_OpenBattery(int PortNum,char* ERRCODE);

/***************************************************************
*	���˿ڿ��ƺ�	�ر�VBAT
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_CloseBattery(int PortNum,char* ERRCODE);

/***************************************************************
*	���˿ڿ��ƺ�	��ȡ�о߱պ�״̬
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*	State��	����1�պ� 0δ�պ�
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_GetJigState(int PortNum,int &State,char* ERRCODE);

/***************************************************************
*	���˿ڿ��ƺ�	�򿪵����
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*	SlotIndex���������� 1~3
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_OpenLock(int PortNum,int SlotIndex,char* ERRCODE);

/***************************************************************
*	���˿ڿ��ƺ�	�պϵ����
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*	SlotIndex���������� 1~3
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_CloseLock(int PortNum,int SlotIndex,char* ERRCODE);








/***************************************************************
*	��˿ڿ��ƺ�	��USB
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*	SlotIndex����� 1~8
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_OpenUsb_MultiPort(int PortNum,int SlotIndex,char* ERRCODE);

/***************************************************************
*	��˿ڿ��ƺ�	�Ͽ�USB
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*	SlotIndex����� 1~8
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_CloseUsb_MultiPort(int PortNum,int SlotIndex,char* ERRCODE);

/***************************************************************
*	��˿ڿ��ƺ�	��VBAT
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*	SlotIndex����� 1~8
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_OpenBattery_MultiPort(int PortNum,int SlotIndex,char* ERRCODE);

/***************************************************************
*	��˿ڿ��ƺ�	�Ͽ�VBAT
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*	SlotIndex����� 1~8
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_CloseBattery_MultiPort(int PortNum,int SlotIndex,char* ERRCODE);

/***************************************************************
*	��˿ڿ��ƺ�	��ȡ�о߱պ�״̬
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*	SlotIndex����� 1~8
*	State��	����1�պ� 0δ�պ�
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_GetJigState_MultiPort(int PortNum,int SlotIndex,int &State,char* ERRCODE);

/***************************************************************
*	��˿ڿ��ƺ�	�򿪵����
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*	SlotIndex����� 1~8
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_OpenLock_MultiPort(int PortNum,int SlotIndex,char* ERRCODE);

/***************************************************************
*	��˿ڿ��ƺ�	�պϵ����
*	PortNum: �˿ں�
*	ERRCODE��������Ϣ
*	SlotIndex����� 1~8
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_CloseLock_MultiPort(int PortNum,int SlotIndex,char* ERRCODE);
