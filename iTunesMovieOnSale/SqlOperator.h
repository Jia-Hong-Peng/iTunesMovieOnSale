#pragma once
#include "SQLite.h"  
#pragma comment(lib,"sqlite.lib")   
using namespace SQLite;

class SqlOperator
{
public:
	SqlOperator();
	~SqlOperator();

	BOOL OpenDb(CString dbName);
	void CloseDb();
	void CreateTable();
	Table Select(CString strTable, CString strSelect);
	Table Select(CString strTable, CString strSelect, CString strWhere);
	BOOL Insert(CString strTable, CString strValue);
	BOOL Update(CString strTable, CString strSet, CString strWhere);
	BOOL Update(CString strTable, CString strSet);
	BOOL Delete(CString strTable, CString strWhere);
	void SqlOperator::Execute(CString dbName);

private:
	Database m_db;
};

