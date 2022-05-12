#pragma once
#include "ComPortEx.h"
#include "ControlBox_DLL.h"
#include "MMI_TESTDlg.h"
#include "PHONE.h"
#include "PowerControl.h"
#include "COMMON.h"
#include "ScanSN.h"
#include <math.h>
#include <cmath>
#include <string>

// MYWORK

class MYWORK : public CWnd
{
	DECLARE_DYNAMIC(MYWORK)

public:
	MYWORK();
	virtual ~MYWORK();


protected:
	DECLARE_MESSAGE_MAP()

public:

	void ThreadManager();
	void TEST_THREAD_ENTER_POINT();

	BOOL CLOSE_POWER();
	BOOL INIT_POWER();
	BOOL OPEN_POWER(CString Volt);
	BOOL POWER_GETCURRENT(float& fCurrent);
	BOOL PHONE_READ_SN(CString &IMEI);
	BOOL CHECK_ATPORT_INSERT_WITH_ONTIME(int TimeOut_s,int iScanTimes, int iPhonePort);
	BOOL CHECK_ATPORT_DISCONNECT(int TimeOut_s);
	BOOL AllisNum(CString str);
	BOOL PHONE_CHECK_Connectivity();
	BOOL SHOUTDOWN_TEST();
	BOOL PHONE_POWER_ON(float Timeout);
	BOOL PHONE_WRITE_FLAG(int RES);
	BOOL CONTROL_BOX_CLOSE_VBAT();
	BOOL CONTROL_BOX_GETJIGSTATE(int &state);
	BOOL CONTROL_BOX_OPEN_LOCK();
	BOOL CONTROL_BOX_CLOSE_LOCK();
	BOOL CONTROL_BOX_OPEN_LOCK2();

	VOID GET_STRINT(CString BaseData,CString SearchStart,CString SearchEnd,CString &ReturnData);

	BOOL MES_MCCHECK_MK_STATION(CString PhoneSN,CString PcbaSN,CString SoftVer,CString sScanIMEI1,CString sScanMEID,CString sScanBTMAC,CString sScanWIFIMAC,CString &sMEBSSN,CString &sMESPhoneSN,CString &sMESIMEI1,CString &sMESMEID,CString &sMESBTMAC,CString &sMESWIFIMAC);
	bool MES_MCRESULT_STATION(CString Fzjsn,CString NetSN,CString PcbaSN,CString SoftVer,CString IMEI1,CString MEID,CString BT_MAC,CString WIFI_MAC,int ERRCODE);
	BOOL MES_CHECK_STATION(CString SN);
	BOOL MES_GETMAC_STATION(CString SN, CString& sWIFIMAC, CString& sBTMAC);
	BOOL MES_MMIUPLOAD(CString SN,bool bTestResult,int iERRCODE);
	BOOL MES_PASS_STATION(CString SN,bool bTestResult,int iERRCODE);
	BOOL MES_PASS_RFSTATION(CString SN, int iERRCODE);
	BOOL MES_PASS_PAIRSTATION(CString SN, CString sLeft, CString sRight, bool bTestResult, int iERRCODE);
	BOOL MES_UPDATE_DATA(CString SN, bool bTestResult);
	BOOL MES_CHECK_DEVICE_MES_STATUS(CString bindSN);
	bool MES_PHONE_LPcheck(CString Fzjsn,CString Fdbsn,CString SoftVer,CString &IMEI1,CString &MEID,CString &BT_MAC,CString &WIFI_MAC,CString &PSN,CString &sOutput);
	bool MES_PHONE_PAIRUPLOAD(CString sScaHMDSN, CString sLeft, CString sRight, CString &sOutPut);
	bool MES_PHONE_PAIRCHECK(CString sScaHMDSN, CString sLeft, CString sRight, CString &sOutPut);
	bool MES_PHONE_MCcheck(CString PhoneSN,CString PcbaSN, CString SoftVer,CString &IMEI1,CString &MEID,CString &BT_MAC,CString &WIFI_MAC,CString &MesPSN,CString &MesICCID,CString &Errorlog);
	void GetDataBySpecificChar(CString inputstr,CArray<CString,CString> &OutputData,CString _Delim);
	double GetDistance(double lat1, double lng1, double lat2, double lng2);
	int round_double(double number);
	int rad(double degree); 
	double haverSin(double x);


	BOOL PHONE_CHECK_Current();
	BOOL PHONE_Function_Test();
	BOOL PHONE_Write_BSN(CString BSN,CString &sOutput);
	BOOL PHONE_Write_PSN(CString PSN, CString &sStrDebug);
	BOOL PHONE_Write_CUSTOMSN(CString PSN, CString &sStrDebug);
	BOOL PHONE_Write_FixtureID(CString &FixtureID);
	BOOL HANDLE_Read_BSN(CString &BSN);
	BOOL PHONE_Read_PSN(CString &PSN);
	BOOL HDM_Read_PSN(CString &PSN);
	BOOL HDM_Install_BackupApp(CString &PSN);
	BOOL HDM_Get_RootPermission(CString &sStrDebug);
	BOOL HDM_Get_ReleasePermission(CString &sStrDebug);
	BOOL HDM_Read_CameraNum(CString &sStrDebug);
	BOOL HDM_Get_ROOT(CString &sStrDebug);
	BOOL HDM_Write_Locked(CString &sStrDebug);
	BOOL HDM_Get_Factroyrese(CString &sStrDebug);
	BOOL HDM_Get_RUNINTIME(CString &sStrDebug);
	bool TEST_WRITE_MCUIMEI(CString sIMEI,CString &sStrDebug);
	BOOL HDM_Read_ADBFLAG(CString &sFLAG);
	BOOL HDM_Get_WAKEUP(CString &sStrDebug);
	BOOL HDM_Read_SWVersion(CString &sStrDebug, CString &sLocked);
	BOOL HDM_Get_RFAddressInfo(CString &sHMDRFAddress);
	BOOL HDM_READ_MAC(CString sMACType, CString &sStrDebug);
	BOOL HDM_READ_BATTCAP(CString &sStrDebug);
	BOOL HANDLE_Read_COMFLAG(CString &sFLAG);
	BOOL HANDLE_Write_HostRFAddress(CString sHMDRFAddress,int iPort);
	BOOL HANDLE_Write_Mode(int iModeType, int iPhonePort);
	BOOL HANDLE_Read_Mode(int iModeType, int iPhonePort);
	BOOL HANDLE_Read_HostRFAddress(CString &sHMDRFAddress, int iPhonePort);
	bool GetKeyValue(CString sValue, CString sStartValue, CString sEndValue, CString &sKeyValue);
	bool SplitKeyValue(CString sValue, CStringArray &caKeyValue);
	bool SplitKeyValueSQ(CString sValue, CStringArray &caKeyValue);
	bool ReadAndWriteJson(HANDLEPARAM stu_HandleParam, CStringArray &caAccBias, CStringArray &caAccMatrix, CStringArray &caGyroBias, CStringArray &caGyroMatrix);
	BOOL HANDLE_Get_Parameters(int iPhonePort, HANDLEPARAM &stu_HandleParam);
	BOOL HANDLE_Reboot(int iPhonePort, CString &sStrDebug);
	BOOL HANDLE_Test_RFCoupled(CString sStrDebug);
	BOOL HDM_Write_HandleParamInfo(HANDLEPARAM stu_HandleParam,CStringArray &caAccBias, CStringArray& caAccMatrix, CStringArray& caGyroBias, CStringArray& caGyroMatrix, CString &sStrDebug);
	BOOL WriteKeyValue(CString sType, CString sControlType, CString sParam, CStringArray* ca);
	BOOL CheckKeyValue(CString sType, CString sControlType, CString sParam, CStringArray* ca);
	BOOL HDM_Write_HostParamInfo(HANDLEPARAM stu_HandleParam, CStringArray* caAccBias, CStringArray* caAccMatrix, CStringArray* caGyroBias, CStringArray* caGyroMatrix, CString &sStrDebug);
	BOOL HDM_Read_HostParamInfo(HANDLEPARAM stu_HandleParam, CStringArray* caAccBias, CStringArray* caAccMatrix, CStringArray* caGyroBias, CStringArray* caGyroMatrix, CString &sStrDebug);
	BOOL TEST_CHECK_YVRFLAG(int iStation,int iType);
	BOOL YVR_PAIR_TEST(CString sHMDRFAddress, int iPhonePort, HANDLEPARAM stu_HandleParam);
	BOOL YVR_PAIR_CHECK(CString sHMDRFAddress, int iPhonePort, HANDLEPARAM stu_HandleParam);
	int HANDLE_PCBA_MMI_EXC();
	int HDM_COUPLING_EXC();
	int HDM_PAIR_EXC();
	int HDM_CHECK_EXC();
	int WRITE_EXC();
	int MES_PAIR_INFORMATION_EXC();
	BOOL HANDLE_Write_COMFLAG(int iStation);
	BOOL HDM_Write_ADBFLAG(int iStation);

	bool TEST_WRITE_MAC(CString sMAC,CString sMACType,CString &sStrDebug);
	bool MACTurn(CString sMAC,CString &sStrDebug);

	BOOL PHONE_Read_FixtureID(CString &FixtureID);
	
	bool CheckWordType(CString str, int iType);

    bool TEST_SEQ(CString &sStrDebug,CString &sReceivedata);
	bool TEST_SPIFLASH(CString &sStrDebug,CString &sReceivedata);
	bool TEST_MCU_Version(CString &sStrDebug,CString &sReceivedata);
	bool TEST_MCU_hw_Version(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_IMEI(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_Tempter(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_EXTEMPTER(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_EXHUMIDTY(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_HUMIDTY(CString &sStrDebug,CString &sReceivedata);	
	bool TEST_CHECK_SIM(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_ICCID(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_CSQ(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_CREG(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_CGREG(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_GPS(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_WIFI(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_BT(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_Battery(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_Gsensor(CString &sStrDebug, CString &sReceivedata);
	bool TEST_CHECK_JoystickYADC(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_JoystickXADC(CString &sStrDebug, CString &sReceiveData);
	bool TEST_CHECK_TriggerADC(CString &sStrDebug, CString &sReceiveData);
	bool TEST_KEY(CString &sStrDebug,CString &Receivedata);
	bool TEST_TOUCH(CString &sStrDebug, CString &sReceivedata);
	bool TEST_LED(CString &sStrDebug,CString &Receivedata);
	bool TEST_INFRARED(CString &sStrDebug, CString &sReceivedata);
	bool ENTER_FACTTEST(int iPhonePort, CString &sReceivedata);
	bool READ_BT(CString &sReceivedata);	
	bool REBOOT_CHECK(CString &sReceiveData);
	bool OUT_FACTTEST(CString &sReceivedata);
	bool TEST_UC15Open(CString &sStrDebug,CString &sReceivedata);
	bool TEST_CHECK_WDOG(CString &sStrDebug,CString &sReceivedata);
	bool PHONE_Write_CFGIN(CString &sStrDebug,CString &sReceivedata);
	bool PHONE_Write_DefIN(CString &sStrDebug,CString &sReceivedata);
	bool PHONE_Write_CFGQue(CString &sStrDebug,CString &sReceivedata);
	bool PHONE_Write_Trans(REPINF *pHRepInf,REPINF *pSRepInf,REPINF *pTRepInf);
	BOOL AT_TEST_EXC(TestList *pTestList);

	bool TEST_UC15_Version(CString &sStrDebug,CString &sReceivedata);
	int GetTextPosFromString(CString szString, CString szSymbol, int nIndex);
	CString GetTextFromStringEx(CString szString, CString szSymbol, int nIndex);
	bool AUTOWIFIBT(CString&sWifi,CString&sBt);
	bool GCRESource_Test(CString &sReceivedata);
	bool Reboot_Test(CString &sReceivedata);
	bool CHECK_BSN(CString sScanBSN,CString sBSN,CString sMESBSN);
	bool CHECK_NUMBER(CString sScanPSN,CString sPSN,CString sMESPSN);
	bool CHECK_IMEI(CString &sIMEI,CString sMESIMEI);
	//比对输入的设备ID+流水号（11位）和读取的设备ID+流水号（11位）
	bool CHECK_FixtureID(CString sScanFixtureID,CString sFixtureID);
	bool CHECK_VERSION();
	bool OPEN_WIFI();
	bool CLOSE_WIFI();
	bool IDLE_Current();
	bool MTK_EnterIDLE_AT();
	bool sendCmdSccess(int iTimes);
	bool PHONE_Read_DefValue(CString &sStrDebug,CString &sReceivedata);
	bool PHONE_DefValue_Count(CString sStrDebug);
	BOOL PHONE_CHECK_CurrentmA();
	bool TEST_MACIMEI_CIP(CString sMESBTMAC,CString sMESIMEI1,CString &sStrDebug);
	bool TEST_MACIMEI_CIP_INV(CString sMacInput,CString &sMacInv);
	bool Test_CheckFTMMode(CString &sStrDebug);
	void DecToBsStr(CString sStrDec, CString &sBs, int iBit);
	void HexToBsStr(CString sStrH, CString &sBs, int iBit);
	void BsToHexStr(CString sHDefaultB, CString &sHDefaultH, int iLth);
	void BsToDecStr(CString sStrBs, CString &sDec);
	bool TEST_Vibrate(CString &sStrDebug, CString &sReceivedata);
	bool TEST_CHECK_IMU(CString &sStrDebug, CString &sReceivedata);
};


