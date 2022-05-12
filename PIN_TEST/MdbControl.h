#pragma once

#import ".\msado15(2007).dll" no_namespace rename("EOF","adoEOF")
// CMdbControl

class CMdbControl : public CWnd
{
	DECLARE_DYNAMIC(CMdbControl)

public:
	CMdbControl();
	virtual ~CMdbControl();



	/****************************************************************
	* 描述：打开MDB数据库文件
	* FilePath（文件路径）
	* PASSWORD（数据库文件密码，如无密码赋“”）
	* ErrCode (执行失败返回失败信息）
	*****************************************************************/
	bool OpenDatabase(CString FilePath,CString PASSWORD,CString &ErrCode);

	/****************************************************************
	* 描述：删除表内所有数据
	* TableName（表名）
	* ErrCode (执行失败返回失败信息）
	*****************************************************************/
	bool DeleteTable(CString TableName,CString &ErrCode);

	/****************************************************************
	* 描述：删除表内特定行-1组条件
	* TableName（表名）
	* FieldName（抬头名）
	* FieldValue（特定行信息）
	* ErrCode (执行失败返回失败信息）
	*****************************************************************/
	bool DeleteTableItem(CString TableName,CString FieldName,CString FieldValue,CString &ErrCode);

	/****************************************************************
	* 描述：删除表内特定行-2组条件
	* TableName（表名）
	* FieldName1（抬头名1）
	* FieldValue1（特定行信息1）
	* FieldName2（抬头名2）
	* FieldValue2（特定行信息2）
	* ErrCode (执行失败返回失败信息）
	*****************************************************************/
	bool DeleteTableItem(CString TableName,CString FieldName1,CString FieldValue1,CString FieldName2,CString FieldValue2,CString &ErrCode);

	/****************************************************************
	* 描述：删除表内特定行-3组条件
	* TableName（表名）
	* FieldName1（抬头名1）
	* FieldValue1（特定行信息1）
	* FieldName2（抬头名2）
	* FieldValue2（特定行信息2）
	* FieldName3（抬头名3）
	* FieldValue3（特定行信息3）
	* ErrCode (执行失败返回失败信息）
	*****************************************************************/
	bool DeleteTableItem(CString TableName,CString FieldName1,CString FieldValue1,CString FieldName2,CString FieldValue2,CString FieldName3,CString FieldValue3,CString &ErrCode);


	/****************************************************************
	* 描述：新建一行 - 调用 AddDatebase（）增加数据时需先调用AddLine（）
	* TableName（表名）
	* ErrCode (执行失败返回失败信息）
	*****************************************************************/
	bool AddLine(CString TableName,CString &ErrCode);

	/****************************************************************
	* 描述：写入数据- 多数据需重复调用
	* FieldName（抬头名）
	* FielValue（写入值）
	* ErrCode (执行失败返回失败信息）
	*****************************************************************/
	bool AddDatebase(CString FieldName,CString FielValue,CString &ErrCode);

	/****************************************************************
	* 描述：读取数据-无筛选条件
	* TableName（表名）
	* FieldName（抬头名）
	* ReadBuffer（读取数据保存）
	* ErrCode (执行失败返回失败信息）
	*****************************************************************/
	bool ReadDatebase(CString TableName,CString FieldName,CArray<CString,CString>&ReadBuffer,CString &ErrCode);

	/****************************************************************
	* 描述：读取数据-有筛选条件
	* TableName（表名）
	* FieldName（抬头名）
	* ConditionFieldName（条件抬头名）
	* SearchCondition（条件）
	* ReadBuffer（读取数据保存）
	* ErrCode (执行失败返回失败信息）
	*****************************************************************/
	bool ReadDatebase(CString TableName,CString FieldName,CString ConditionFieldName,CString SearchCondition,CArray<CString,CString>&ReadBuffer,CString &ErrCode);

	/****************************************************************
	* 描述：关闭MDB数据库文件
	* ErrCode (执行失败返回失败信息）
	*****************************************************************/
	bool CloseDatebase(CString &ErrCode);



protected:
	DECLARE_MESSAGE_MAP()
};


