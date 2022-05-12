// MdbControl.cpp : 实现文件
//

#include "stdafx.h"
#include "MdbControl.h"
#include "afxcmn.h"
#include "afxwin.h"


_ConnectionPtr m_pConnection;
_RecordsetPtr m_pRecordset;

// CMdbControl

IMPLEMENT_DYNAMIC(CMdbControl, CWnd)

CMdbControl::CMdbControl()
{

}

CMdbControl::~CMdbControl()
{
}


BEGIN_MESSAGE_MAP(CMdbControl, CWnd)
END_MESSAGE_MAP()



// CMdbControl 消息处理程序


bool CMdbControl::OpenDatabase(CString FilePath,CString PASSWORD,CString &ErrCode)
{

	// 初始化COM,创建ADO连接等操作
	
	try                 
	{ 
		// 打开本地Access库
		HRESULT hr = 0;
		CoInitialize(NULL); 
		hr = m_pConnection.CreateInstance("ADODB.Connection");///创建 Connection 对象
		if(SUCCEEDED(hr))
		{
			CString str;
			if (PASSWORD!="")
			{
				str.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Persist Security Info=False;Jet OLEDB:Database Password=%s",FilePath,PASSWORD);
			}
			else
			{
				str.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Persist Security Info=False",FilePath);
			}
			


			m_pConnection->Open(str.GetBuffer(str.GetLength()),TEXT(""),TEXT(""),adModeUnknown);
			str.ReleaseBuffer(str.GetLength());
		}
		else
		{
			ErrCode="创建 Connection 对象失败!";
			return FALSE;
		}
	}
	catch(_com_error e)
	{
		ErrCode="数据库连接失败，请确认路径!";
		return FALSE;
	}

	try
	{
		if (m_pRecordset==NULL)
		{
			m_pRecordset.CreateInstance("ADODB.Recordset");
		}		
	}
	catch(...)
	{
		ErrCode="创建CreateInstance对象失败";
		return FALSE;
	}
	return true;
}

bool CMdbControl::DeleteTable(CString TableName,CString &ErrCode)
{

	try
	{	
		CString Table;
		Table.Format(TEXT("DELETE * FROM %s"),TableName);
		OutputDebugString(TEXT("DEBUG: ") + Table);
		m_pRecordset->Open(Table.GetBuffer(Table.GetLength()),                // 删除表中所有字段
			m_pConnection.GetInterfacePtr(),  // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
		Table.ReleaseBuffer(Table.GetLength());
		return true;
	}
	catch(...)
	{
		ErrCode.Format(TEXT("删除表%s出现未知的错误！"),TableName);
		return false;
	}

}

bool CMdbControl::DeleteTableItem(CString TableName,CString FieldName,CString FieldValue,CString &ErrCode)

{
	CString Table="";
	try
	{	
		Table.Format(TEXT("DELETE * FROM  %s  WHERE  %s='%s'"),TableName,FieldName,FieldValue);
		m_pRecordset->Open(Table.GetBuffer(Table.GetLength()),                // 删除表中所有字段
			m_pConnection.GetInterfacePtr(),  // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
		Table.ReleaseBuffer(Table.GetLength());
		m_pRecordset->Close();
		return true;
	}
	catch(...)
	{
		ErrCode.Format("执行 %s出现未知的错误",Table);
		return false;
	}
}

bool CMdbControl::DeleteTableItem(CString TableName,CString FieldName1,CString FieldValue1,CString FieldName2,CString FieldValue2,CString &ErrCode)

{
	CString Table="";
	try
	{	
		Table.Format(TEXT("DELETE * FROM %s  WHERE  %s='%s'  AND %s='%s'"),TableName,FieldName1,FieldValue1,FieldName2,FieldValue2);
		m_pRecordset->Open(Table.GetBuffer(Table.GetLength()),                // 删除表中所有字段
			m_pConnection.GetInterfacePtr(),  // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
		Table.ReleaseBuffer(Table.GetLength());
		//m_pRecordset->Close();
		return true;
	}
	catch(...)
	{
		ErrCode.Format("执行 %s出现未知的错误",Table);
		return false;
	}
}

bool CMdbControl::DeleteTableItem(CString TableName,CString FieldName1,CString FieldValue1,CString FieldName2,CString FieldValue2,CString FieldName3,CString FieldValue3,CString &ErrCode)

{
	CString Table="";
	try
	{	
		Table.Format(TEXT("DELETE * FROM  %s  WHERE %s='%s' AND %s='%s'  AND %s='%s'"),TableName,FieldName1,FieldValue1,FieldName2,FieldValue2,FieldName3,FieldValue3);
		m_pRecordset->Open(Table.GetBuffer(Table.GetLength()),                // 删除表中所有字段
			m_pConnection.GetInterfacePtr(),  // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
		Table.ReleaseBuffer(Table.GetLength());
		m_pRecordset->Close();
		return true;
	}
	catch(...)
	{
		ErrCode.Format("执行 %s出现未知的错误",Table);
		return false;
	}
}

bool CMdbControl::AddLine(CString TableName,CString &ErrCode)
{
	/////打开表//////////

	try
	{	
		CString Table;
		Table.Format("SELECT * FROM %s",TableName);
		m_pRecordset->Open(Table.GetBuffer(Table.GetLength()),                // 查询DemoTable表中所有字段
			m_pConnection.GetInterfacePtr(),  // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
		Table.ReleaseBuffer(Table.GetLength());
	}
	catch(...)
	{
		ErrCode.Format("查询表%s出现未知的错误！",TableName);
		return false;
	}
	////////////////

	try
	{
		if (m_pRecordset!=NULL)
		{
			m_pRecordset->AddNew();
			m_pRecordset->Update();
			return true;
		}
		else
		{
			ErrCode="添加行出现未知错误！";
			return false;
		}
	}
	catch(...)
	{
		ErrCode="添加行出现未知错误！";
		return false;
	}
}

bool CMdbControl::AddDatebase(CString FieldName,CString FielValue,CString &ErrCode)
{
	try
	{

		m_pRecordset->PutCollect(FieldName.GetBuffer(FieldName.GetLength()),FielValue.GetBuffer(FielValue.GetLength()));
		m_pRecordset->Update();

		FieldName.ReleaseBuffer(FieldName.GetLength());
		FielValue.ReleaseBuffer(FielValue.GetLength());
		return true;
	}

	catch(...)
	{
		ErrCode="插入字段出现未知的错误,请首先确认字段是否存在";
		return false;
	}
}

bool CMdbControl::ReadDatebase(CString TableName,CString FieldName,CArray<CString,CString>&ReadBuffer,CString &ErrCode)
{
	CString Table="";
	try
	{		
		Table.Format("SELECT * FROM  %s",TableName);
		m_pRecordset->Open(Table.GetBuffer(Table.GetLength()),// 查询DemoTable表中所有字段
							m_pConnection.GetInterfacePtr(),  // 获取库接库的IDispatch指针
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);

		Table.ReleaseBuffer(Table.GetLength());
	}
	catch(...)
	{
		ErrCode.Format("执行%s出现未知的错误",Table);
		return false;
	}
	////////////////////////////////////////////

	_variant_t var;
	try
	{
		if(!m_pRecordset->BOF)
			m_pRecordset->MoveFirst();
		else
		{
			CString str;
			ErrCode.Format("%s表内数据为空！",TableName);
			m_pRecordset->Close();
			return true;	
			//return false;
		}

		while(!m_pRecordset->adoEOF)
		{
			var = m_pRecordset->GetCollect(FieldName.GetBuffer(FieldName.GetLength()));
			FieldName.ReleaseBuffer(FieldName.GetLength());
			if(var.vt != VT_NULL)
			{
				ReadBuffer.Add((CString)var);
			}
			
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
		return true;	
	}

	catch(...)
	{
		ErrCode.Format("读取字段%s出现未知的错误",FieldName);
		return false;
	}
}

bool CMdbControl::ReadDatebase(CString TableName,CString FieldName,CString ConditionFieldName,CString SearchCondition,CArray<CString,CString>&ReadBuffer,CString &ErrCode)
{
	CString Table="";

	try
	{		
		Table.Format("SELECT * FROM %s WHERE %s='%s'",TableName,ConditionFieldName,SearchCondition);
		m_pRecordset->Open(Table.GetBuffer(Table.GetLength()),                // 查询DemoTable表中所有字段
			m_pConnection.GetInterfacePtr(),  // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);

		Table.ReleaseBuffer(Table.GetLength());
	}
	catch(...)
	{
		ErrCode.Format("执行%s出现未知的错误",Table);
		return false;
	}
	////////////////////////////////////////////

	_variant_t var;
	try
	{
		if(!m_pRecordset->BOF)
			m_pRecordset->MoveFirst();
		else
		{
			CString str;
			ErrCode.Format("%s表内数据为空！",TableName);
			m_pRecordset->Close();
			return true;	
		}

		while(!m_pRecordset->adoEOF)
		{
			var = m_pRecordset->GetCollect(FieldName.GetBuffer(FieldName.GetLength()));
			FieldName.ReleaseBuffer(FieldName.GetLength());
			if(var.vt != VT_NULL)
			{
				ReadBuffer.Add((CString)var);
			}

			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
		return true;	
	}

	catch(...)
	{
		ErrCode.Format("读取字段%s出现未知的错误",FieldName);
		return false;
	}
}

bool CMdbControl::CloseDatebase(CString &ErrCode)
{

	try
	{
		if(m_pConnection==NULL)
		{
			return true;
		}

		if(m_pConnection->State)
		{
			if(m_pRecordset->State)
			{
				m_pRecordset->Close();
			}

			m_pConnection->Close();
		}

		if(m_pRecordset!=NULL)
		{
			m_pRecordset.Release();
			m_pRecordset=NULL;
		}

		m_pConnection.Release();
		m_pConnection=NULL;
	}

	catch(_com_error e)
	{
		ErrCode="断开数据库连接失败";
		return FALSE;
	}
	return true;
}



