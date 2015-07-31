#include "stdafx.h"
#include "SqlOperator.h"


CString strFormatTemp = NULL;

SqlOperator::SqlOperator()
{
}


SqlOperator::~SqlOperator()
{
}


BOOL SqlOperator::OpenDb(CString dbName)
{
	if (m_db.IsOpen())
	{
		return TRUE;
	}
	else
	{
		if (m_db.Open(dbName) == SQLITE_OK)
		{
			return TRUE;
		}
		return FALSE;
	}
}


void SqlOperator::Execute(CString strSQL)
{	
	m_db.ExecuteSQL(strSQL);
}

Table SqlOperator::Select(CString strTable, CString strSelect)
{
	strFormatTemp.Format(_T("SELECT %s FROM %s; "), strSelect, strTable);
	Table tb = m_db.QuerySQL(strFormatTemp);
	return tb;
}

Table SqlOperator::Select(CString strTable, CString strSelect, CString strWhere)
{
	strFormatTemp.Format(_T("SELECT %s FROM %s WHERE %s; "), strSelect, strTable, strWhere);
	Table tb = m_db.QuerySQL(strFormatTemp);
	return tb;
}


BOOL SqlOperator::Update(CString strTable, CString strSet, CString strWhere)
{
	strFormatTemp.Format(_T("UPDATE '%s' SET %s WHERE %s; "), strTable, strSet, strWhere);
	if (m_db.IsOpen())
	{
		if (m_db.ExecuteSQL(strFormatTemp) == SQLITE_OK) return TRUE;
		else return FALSE;
	}
}


BOOL SqlOperator::Update(CString strTable, CString strSet)
{
	strFormatTemp.Format(_T("UPDATE '%s' SET %s;"), strTable, strSet);
	if (m_db.IsOpen())
	{
		if (m_db.ExecuteSQL(strFormatTemp) == SQLITE_OK) return TRUE;
		else return FALSE;
	}
}

BOOL SqlOperator::Insert(CString strTable, CString strValue)
{

	strFormatTemp.Format(_T("INSERT INTO '%s' VALUES(%s);"), strTable, strValue);
	if (m_db.IsOpen())
	{
		if (m_db.ExecuteSQL(strFormatTemp) == SQLITE_OK) return TRUE;
		else return FALSE;
	}
}


BOOL SqlOperator::Delete(CString strTable, CString strWhere)
{

	strFormatTemp.Format(_T("DELETE FROM '%s' WHERE %s;"), strTable, strWhere);
	if (m_db.IsOpen())
	{
		if (m_db.ExecuteSQL(strFormatTemp) == SQLITE_OK) return TRUE;
		else return FALSE;
	}
}

void SqlOperator::CloseDb()
{
	if (m_db.IsOpen())
	{
		m_db.Close();
	}
}
