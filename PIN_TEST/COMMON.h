#include "Tools.h"
#pragma once
//用于TOOLS类字符串获取
#define BResult(__RESULT)\
	if (((bool)__RESULT)==FALSE)\
		{\
		return FALSE;\
		}

#define _ISTEST	0
#define _RANGE_MIN	1
#define _RANGE_MAX	2

enum
{
	READY=1,
	RUN=2,
	PASS=3,
	FAIL=4
};

enum
{
	D1_YVR,
	D2_YVR,
	ProjectMaxNum,
};

enum
{
	SUCCESS,
	INIT_POWER_FAIL,
	CHECK_FUNCTION_FAIL,
	CheckCurrent_FAIL,
	PHONE_POWER_ON_FAIL,
	CHECK_ATPORT_INSERT_FAIL,
	CHECK_DEVICEID_MES_STATUS_FAIL,
	PHONE_READ_SN_FAIL,
	MES_PassStation_FAIL,
	MES_CheckStation_FAIL,
	MES_UPDATE_DATA_FAIL,
	Save_Log_FAIL,
	READ_PCBASN_FAIL,
	CHECK_BSN_FAIL,
	HMD_CalStation_FAIL,
	HMD_RFCoupled_FAIL,
	HMD_PAIR_FAIL,

	//INIT_POWER_FAIL,
	//PHONE_POWER_ON_FAIL,
	//CHECK_ATPORT_INSERT_FAIL,
	//CHECK_DEVICEID_MES_STATUS_FAIL,
	//PHONE_READ_SN_FAIL,
	//CHECK_VERSION_FAIL,
	//CHECK_FUNCTION_FAIL,
	//Open_Light_Senser_FAIL,
	//CheckGPO_FAIL,
	//CheckCurrent_FAIL,
	//MES_PassStation_FAIL,
	//MES_CheckStation_FAIL,
	//MES_UPDATE_DATA_FAIL,
	//Save_Log_FAIL,
	//WriteSN_FAIL,
	//Read_PSN_FAIL,
	//WriteFixtureID_FAIL,
	//Check_PREFIX_FAIL,
	//READ_PCBASN_FAIL,
	//CHECK_BSN_FAIL,
	//CHECK_PSN_FAIL,
	//CHECK_IMEI_FAIL,
	//CHECK_BT_FAIL,
	//CHECK_ICCID_FAIL,
	//SET_INIT_CLOCK_FAIL,
	//SET_OVER_INIT_CLOCK_FAIL,
	//CHECK_IDLE_FAIL,
	//CHECK_CIP_FAIL,
	//HMD_CalStation_FAIL,
	//HMD_RFCoupled_FAIL
};

enum {
	Agilent,
	KEITHLEY,
};

enum {
	HDL_YVR,
	HDM_YVR,
};

enum 
{
	CHECK_MODE_ONLINE=0,
	CHECK_MODE_FTM=1,
	CHECK_MODE_ERROR=2,};

typedef enum
{
	HANDLE_PCBA_MMI_TEST,
	HANDLE_COUPLING_TEST,
	HDM_COUPLING_TEST,
	HDM_PAIR_TEST,
	HDM_CHECK_PACK,
	QA_PAIR_MESCHECK_PACK,
	PACK_WRITE,
	StationMaxNum
}STATION;

typedef enum
{
	ENUM_READIMEI,
	ENUM_versionsTEST,
	ENUM_FunctionTEST,
	ENUM_PORT,
	ENUM_EXIT,
	ENUM_ALARM,
	ENUM_IDLE,
	ENUM_ICCID,
	ENUM_IMSI,
	ENUM_WRITETEST,
	ENUM_FLAGTEST,
	ENUM_SETPOWERTEST,
	ENUM_IDLETEST,
	ENUM_SETPOWERINIT,
	ENUM_WRITECUSTOMSN,
	ENUM_WRITECFG,
	ENUM_OPENLIGHTSENSER,
	ENUM_MES_CHECK,
	ENUM_MES_Result,
	ENUM_MES_CHECK_DEVICE_ID_STATUS,
	ENUM_MES_UPDATE_Result,
	ENUM_Current,
	ENUM_SEQ,
	ENUM_SPIFLASH,
	ENUM_MCU_VERSION,
	ENUM_MCU_HY_VERSION,
	ENUM_IMEI,
	ENUM_TEMPTER,
	ENUM_EX_TEMPTER,
	ENUM_EX_HUMIDTY,
	ENUM_HUMIDTY,
	ENUM_SIM,
	ENUM_CSQ,
	ENUM_CREG,
	ENUM_CGREG,
	ENUM_WIFI,
	ENUM_BT,
	ENUM_GPS,
	ENUM_Light_Senser,
	ENUM_READBSN,
	ENUM_INIT_CLOCK,
	ENUM_INIT_ENDCLOCK,
	ENUM_READPSN,
	ENUM_Battery_Voltage,
	ENUM_READ_FixtureID,
	ENUM_ENTER_FACTTEST,
	ENUM_Gsensor,
	ENUM_UC15_VERSION,
	ENUM_Vibrate,
	ENUM_LED,
	ENUM_KEY,
	ENUM_INFRARED,
	ENUM_TOUCH,
	ENUM_JoystickX_ADC,
	ENUM_JoystickY_ADC,
	ENUM_Trigger_ADC,
	ENUM_IMU,
}TestItem;

typedef struct
{
	CString sserialnumber;
	CString scontrollertype;
	CString srf_addr_0;
	CString saccbias;
	//CStringArray caAccBias;
	CString saccmatrix;
	//CStringArray caAccMatrix;
	CString sgyrobias;
	//CStringArray caGyroBias;
	CString sgyromatrix;
	//CStringArray caGyroMatrix;
public:
	void setNull() {
		sserialnumber = "";
		scontrollertype = "";
		srf_addr_0 = "";
		saccbias = "";
		saccmatrix = "";
	}
}HANDLEPARAM;

typedef struct {
	CString sProjectNameMessage[10];
	CString sTestSelectionMessage[10];
}Message;

typedef struct {
	CString sConfigPath;
	CString sStationName;
	CString sDeviceID;
	CString sUserID;
}SFISConfig;

typedef struct
{
	CString TotalNumber;
	CString PASSNumber;
	CString FailNumber;
	CString Rate;
	//进度条
	float fItemNumber;
	float fPost;

	bool bCHECK_PHASE;
	CString sExeFilePath;
	CString sConfigPath;
	CString sLogPath;
	CString sPREFIX;
	CString sDatabasePath;
	CString sDefaultExcelPath;
	CString sCFGExcelPath;

	int iPowerType;
	int iBoardAdd;
	int iGPIBAdd;
	int iPhonePort;
	int iPhonePort2;
	int BaudRate;
	int iCtrBOXComPort;
	int Test_Selection;
	int iProject;
	bool	bMES;
	CString sPO;
	CString sMESUser;
	CString sMES_Line;
	CString sMESStationName;
	CString sJigNum;//夹具编号
	CString sMESURL;
	CString sFateName;//夹具名 为空
	CString sMesStep;
	CString SN;
	HANDLEPARAM stu_HANDLEPARAM1;
	HANDLEPARAM stu_HANDLEPARAM2;
	Message stu_Message;
	SFISConfig stu_SFISConfig;
}CONFIG;


typedef struct _tagStandardTestItem
{
	bool needTest;
	float rangeMin;
	float rangeMax;
	_tagStandardTestItem()
	{
		needTest = TRUE;
		rangeMin = 0.0;
		rangeMax = 60.0;
	}
public:
	void setDefaultValue(float _minValue, float _maxValue)
	{
		rangeMax = _maxValue;
		rangeMin = _minValue;
	}
	CString ToString()
	{
		CString sModifyValue = "";
		sModifyValue.Format("%d,%.3f,%.3f", (needTest ? 1 : 0), rangeMin, rangeMax);
		return sModifyValue;
	}
	float MidValue()
	{
		return (float)(rangeMin + rangeMax) / 2;
	}
	bool Judge(CString _inputCheckValueStr)
	{
		float fValue = atof(_inputCheckValueStr);
		if (fValue >= rangeMin && fValue <= rangeMax)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool Judge(float fValue)
	{
		if (fValue >= rangeMin && fValue <= rangeMax)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}StandardTestItem;



typedef struct
{
	bool bcurrent;
	bool bwritesn;
	bool bFunction;
	bool breadsn;

	bool bgps;
	CString GPS_Latitude;
	CString GPS_Longitude;
	bool bcreg;
	bool bcgreg;
	bool bwifi;
	bool bautoWIFIBT;
	bool bMMIUpload;
	bool bBT;
	bool bLight_sensitive;

	bool bGetTestItemSuccess;
	bool bIMEI;
	bool bFlag;
	bool bSIM;
	bool bCSQ;
	bool bTempter;
	bool bEXTempter;
	bool bGsensor;
	bool bWrite_Fixture_ID;
	bool bICCID;
	bool bWDOG;
	bool bCFGIN;

	float TEMP_Min;
	float TEMP_Max;
	float HUMIDTY_Min;
	float HUMIDTY_Max;
	float	Current_Min;
	float   Current_Max;
	float	Battery_VMain;
	float	IDLE_Min;
	float	IDLE_Max;
	int LightSen_MAX;
	int LightSen_MIN;
	CString Total_v;
	CString Mcu_sw_v;
	CString Mcu_hw_v;
	CString FGID;
	int		TestIndex;
	long ENUM_PWR_V_MAX;
	long ENUM_PWR_V_MIN;
	CString	sTESTVOLT;
	CString sBT1;
	CString sBT2;
	CString sBT3;
	CString sWIFI1;
	CString sWIFI2;
	CString sWIFI3;
	CString sWIFI4;
	CString sWIFI5;
	CString sUC15Version;

	bool bSetFlag;
	bool bCheckLastStepFlag;
	StandardTestItem STI_joystickXADC;
	StandardTestItem STI_joystickYADC;
	StandardTestItem STI_triggerADC;
	StandardTestItem STI_iMU;
	StandardTestItem STI_RuninTime;
	StandardTestItem STI_averageRssi;
	CString sAverageRssi;
	StandardTestItem STI_packetLossRate;
	CString sPacketLossRate;
	StandardTestItem STI_reTransRate;
	CString sReTransRate;
	StandardTestItem STI_reAgvCount;
	CString sReAgvCount;
	bool bCheckVersion;
	CString sInterVersion;
	bool bBTMAC;
	bool bWIFIMAC;
	StandardTestItem STI_batteryCap;


}AllTestItem;

typedef struct 
{
	int iPortNumber;
	CString sPortName;
}PortInfo_scsn;

typedef struct 
{
	PortInfo_scsn portinfo_scan[20];
}ScanPort;

typedef struct
{
	CString	POWER_Main;
	CString	POWER_Aux;
	CString	POWER_Solar;

}POWER_VALUE;

typedef struct
{
	CString ATCommond;
	CString TargetReturnValue;
	CString ATQuery;
	CString TimeOut;
	CString Default;
	CString RAW;
	CString Fulll;
	CString Alert;
	CString Name;

	BOOL	ENABLE;
	int TestIndex;
}TestList;

typedef struct
{
	TestList	m_TestList[200];
	CString     sCFGVersion;
	int			TestItemNum;
	int			TestIndex;
	int			MultipleTestNum;
	bool		GetTestListSuccess;
	bool		StopFlag;
	bool		RunState;

}EXCELTestItem;

typedef struct
{
	TestList	m_TestList[2000];
	int			TestItemNum;
	int			TestIndex;
	int			MultipleTestNum;
	bool		GetTestListSuccess;
	bool		StopFlag;
	bool		RunState;
}DefEXCELTestItem;

typedef struct
{
	CString     sIMEI;
	CString		sSEQ;
	CString		sUC15Ver;
	CString		sMCUVer;
	CString     sBVol;
	CString		sTempter;
	CString		sHUMIDTY;
	CString		sEXTempter;
	CString     sEXHUMIDTY;
	CString		sLSENSOR;
	CString		sGsensor;
	CString		sGPS;
	CString		sGPS_latitude;
	CString		sGPS_longitude;
	CString		sGPS_Star;
	CString     sGPS_Coordinate;
	CString		sResult;
}MMIDATA;

struct REPINF
{
	CString sName;
	CString sDefaultB;
	CString sDefaultH;
};



