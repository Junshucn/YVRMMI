// ControlBox_DLL.h : ControlBox_DLL DLL 的主头文件
//

#pragma once

#ifdef	DLL_API
#else  
#define DLL_API		extern "C" _declspec(dllimport)  
#endif


/***************************************************************
*	单端口控制盒	打开USB
*	PortNum: 端口号
*	ERRCODE：错误信息
*
***************************************************************/
DLL_API BOOL _stdcall ControlBox_OpenUsb(int PortNum,char* ERRCODE);

/***************************************************************
*	单端口控制盒	断开USB
*	PortNum: 端口号
*	ERRCODE：错误信息
*
***************************************************************/
DLL_API BOOL _stdcall ControlBox_CloseUsb(int PortNum,char* ERRCODE);

/***************************************************************
*	单端口控制盒	打开VBAT
*	PortNum: 端口号
*	ERRCODE：错误信息
*
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_OpenBattery(int PortNum,char* ERRCODE);

/***************************************************************
*	单端口控制盒	关闭VBAT
*	PortNum: 端口号
*	ERRCODE：错误信息
*
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_CloseBattery(int PortNum,char* ERRCODE);

/***************************************************************
*	单端口控制盒	获取夹具闭合状态
*	PortNum: 端口号
*	ERRCODE：错误信息
*	State：	返回1闭合 0未闭合
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_GetJigState(int PortNum,int &State,char* ERRCODE);

/***************************************************************
*	单端口控制盒	打开电磁锁
*	PortNum: 端口号
*	ERRCODE：错误信息
*	SlotIndex：电磁锁序号 1~3
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_OpenLock(int PortNum,int SlotIndex,char* ERRCODE);

/***************************************************************
*	单端口控制盒	闭合电磁锁
*	PortNum: 端口号
*	ERRCODE：错误信息
*	SlotIndex：电磁锁序号 1~3
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_CloseLock(int PortNum,int SlotIndex,char* ERRCODE);








/***************************************************************
*	多端口控制盒	打开USB
*	PortNum: 端口号
*	ERRCODE：错误信息
*	SlotIndex：序号 1~8
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_OpenUsb_MultiPort(int PortNum,int SlotIndex,char* ERRCODE);

/***************************************************************
*	多端口控制盒	断开USB
*	PortNum: 端口号
*	ERRCODE：错误信息
*	SlotIndex：序号 1~8
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_CloseUsb_MultiPort(int PortNum,int SlotIndex,char* ERRCODE);

/***************************************************************
*	多端口控制盒	打开VBAT
*	PortNum: 端口号
*	ERRCODE：错误信息
*	SlotIndex：序号 1~8
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_OpenBattery_MultiPort(int PortNum,int SlotIndex,char* ERRCODE);

/***************************************************************
*	多端口控制盒	断开VBAT
*	PortNum: 端口号
*	ERRCODE：错误信息
*	SlotIndex：序号 1~8
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_CloseBattery_MultiPort(int PortNum,int SlotIndex,char* ERRCODE);

/***************************************************************
*	多端口控制盒	获取夹具闭合状态
*	PortNum: 端口号
*	ERRCODE：错误信息
*	SlotIndex：序号 1~8
*	State：	返回1闭合 0未闭合
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_GetJigState_MultiPort(int PortNum,int SlotIndex,int &State,char* ERRCODE);

/***************************************************************
*	多端口控制盒	打开电磁锁
*	PortNum: 端口号
*	ERRCODE：错误信息
*	SlotIndex：序号 1~8
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_OpenLock_MultiPort(int PortNum,int SlotIndex,char* ERRCODE);

/***************************************************************
*	多端口控制盒	闭合电磁锁
*	PortNum: 端口号
*	ERRCODE：错误信息
*	SlotIndex：序号 1~8
***************************************************************/
DLL_API	BOOL _stdcall ControlBox_CloseLock_MultiPort(int PortNum,int SlotIndex,char* ERRCODE);
