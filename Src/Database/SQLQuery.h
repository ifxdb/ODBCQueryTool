////////////////////////////////////////////////////////////////////////
//
// File: SQLQuery.h
//
// Copyright (c) 1998-2016 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Last Revision:   14-12-2016
// Version number:  1.3.0
//
#pragma once
#include "SQLVariant.h"
#include "SQLDatabase.h"
#include "Locker.h"
#include <sql.h>
#include <map>

#define SQL_MAX_IDENTIFIER   128

// Arbitrary max limit for a normal string to be retrieved
// Above this limit the stream interface is used
// Can be modified by the "SetBufferSize" method
#define OPTIM_BUFFERSIZE (4*1024)

// After this amount of seconds it's been toooooo long
#define QUERY_TOO_LONG 2.0

class bcd;
class SQLDate;
class SQLDatabase;

typedef std::map<int,    SQLVariant*> ColNumMap;
typedef std::map<CString,SQLVariant*> ColNameMap;
typedef std::map<int,    SQLVariant*> VarMap;

class SQLQuery
{
public:
  // Construct SQL query to be later connected
  SQLQuery();
  // Construct SQL query connected to a database
  SQLQuery(SQLDatabase* p_database);
  SQLQuery(HDBC p_hdbc);
 ~SQLQuery();

  void Init(SQLDatabase* p_database);
  void Init(HDBC p_connection);
  void Close();
  void Open();
  void ResetParameters();

  // BEFORE DOING A QUERY

  // Set the rebind map for datatypes (prior to executing SQL)
  void SetRebindMap(RebindMap* p_map);
  // Set other buffer optimization size
  void SetBufferSize(int p_bufferSize);
  // Set speed threshold for reporting
  void SetSpeedThreshold(double p_seconds);
  // Set maximum rows to get
  void SetMaxRows(int p_maxrows);

  // Set parameters for statement
  void SetParameter  (int p_num,SQLVariant*   p_param,int p_type = SQL_PARAM_INPUT);
  void SetParameter  (int p_num,long          p_param,int p_type = SQL_PARAM_INPUT);
  void SetParameterUL(int p_num,unsigned long p_param,int p_type = SQL_PARAM_INPUT);
  void SetParameter  (int p_num,const char*   p_param,int p_type = SQL_PARAM_INPUT);
  void SetParameter  (int p_num,CString&      p_param,int p_type = SQL_PARAM_INPUT);
  void SetParameter  (int p_num,SQLDate&      p_param,int p_type = SQL_PARAM_INPUT);
  void SetParameter  (int p_num,SQLTime&      p_param,int p_type = SQL_PARAM_INPUT);
  void SetParameter  (int p_num,SQLTimestamp& p_param,int p_type = SQL_PARAM_INPUT);
  void SetParameter  (int p_num,const bcd&    p_param,int p_type = SQL_PARAM_INPUT);

  // SINGLE STATEMENT

  // Do a SQL statement
  void        DoSQLStatement        (const CString& p_statement);
  SQLVariant* DoSQLStatementScalar  (const CString& p_statement);
  int         DoSQLStatementNonQuery(const CString& p_statement);
  // Overrides with one parameter
  void        DoSQLStatement(const CString& p_statement,const int   p_param1);
  void        DoSQLStatement(const CString& p_statement,const char* p_param1);
  void        DoSQLStatement(const CString& p_statement,const bcd&  p_param1);
  // Variants of the DoSQLStatement
  SQLVariant* DoSQLStatementScalar  (const CString& p_statement,const int   p_param1);
  SQLVariant* DoSQLStatementScalar  (const CString& p_statement,const char* p_param1);
  SQLVariant* DoSQLStatementScalar  (const CString& p_statement,const bcd&  p_param1);
  int         DoSQLStatementNonQuery(const CString& p_statement,const int   p_param1);
  int         DoSQLStatementNonQuery(const CString& p_statement,const char* p_param1);
  int         DoSQLStatementNonQuery(const CString& p_statement,const bcd&  p_param1);
  // Variant with a catch to it
  void        TryDoSQLStatement(const CString& p_statement);

  // BOUND STATEMENT
  // Divide a SQL statement in Prepare/Execute/Fetch
  void        DoSQLPrepare(const CString& p_statement);
  void        DoSQLExecute();
  // Get bounded columns from query
  ColNumMap*  GetBoundedColumns();

  // Parallel cancellation of the statement
  void        DoCancelQuery();

  // GET RESULTS FROM AN EXECUTED QUERY

  // Get next record of previously executed select statement
  bool        GetRecord();
  // Get a pointer to a variable of the column
  SQLVariant* GetColumn(int icol);
  // Get the variant data type of the columns
  int         GetColumnType(int icol);
  // Get number of columns in the result set
  int         GetNumberOfColumns() const;
  // Get number of records read so far
  int         GetNumberOfRows();
  // ColumnName -> column number
  int         GetColumnNumber(const char* p_columnName);
  // ColumnNumber -> column name
  bool        GetColumnName(int p_column,CString& p_name);
  // Get length of the column
  int         GetColumnLength(int p_column);
  // Get Display size of the column
  int         GetColumnDisplaySize(int p_column);

  // Getting the results of the query
  SQLVariant& operator[](int p_index);

  // VARIOUS STATUS METHODS
  // Get true if no error found so far
  bool        IsOk() const;
  // Get the error string
  CString     GetError();
  // See if column is NULL value
  bool        IsNull(int col);
  // See if column is EMPTY value
  bool        IsEmpty(int col);
  // Get version for work-around
  int         GetODBCVersion();

  // LEGACY SUPPORT ODBC 1.x AND 2.x
  void DescribeColumn(int           p_col
                     ,CString&      p_columnName
                     ,CString&      p_colLabel
                     ,SQLSMALLINT&  p_sqlType
                     ,SQLUINTEGER&  p_colSize
                     ,SQLSMALLINT&  p_colScale
                     ,SQLSMALLINT&  p_colNullable
                     ,SQLINTEGER&   p_colDispSize);

private:
  // Set parameter for statement
  void  InternalSetParameter(int p_num,SQLVariant*   p_param,int p_type = SQL_PARAM_INPUT);
  // Bind application parameters
  void  BindParameters();
  void  BindColumns();
  void  BindColumnNumeric(SQLSMALLINT p_column,SQLVariant* p_var,int p_type);

  // Reset all column to NULL
  void  ResetColumns();
  // Convert database dependent SQL_XXXX types to C-types SQL_C_XXXX
  short SQLType2CType(short p_sqlType);
  // Provide piece-by-piece data at exec time of the SQLExecute
  int   ProvideAtExecData();
  // Retrieve the piece-by-piece data at exec time of the SQLFetch
  int   RetrieveAtExecData();
  // Get max column length
  int   GetMaxColumnLength();
  // Get the internal error string
  void  GetLastError(CString p_prefix = "");
  // Report timing to logfile
  void  ReportQuerySpeed(LARGE_INTEGER p_start);

  SQLDatabase*  m_database;          // Database
  HDBC          m_connection;        // In CTOR connection handle.
  HSTMT         m_hstmt;             // Statement handle
  RETCODE       m_retCode;           // last SQL (error)code
  CString       m_lastError;         // last error string
  unsigned      m_maxColumnLength;   // Max length
  bool          m_hasLongColumns;    // Use SQLGetData for long columns
  int           m_bufferSize;        // Alternate Buffer size
  int           m_maxRows;           // Maximum rows to fetch
  double        m_speedThreshold;    // After this amount of seconds, it's taken too long

  short         m_numColumns;        // Number of result columns in result set
  SQLLEN        m_rows;              // Number of rows processed in INSERT/UPDATE/DELETE
  long          m_fetchIndex;        // Number of rows fetched
  bool          m_prepareDone;       // Internal prepare flag
  bool          m_boundDone;         // Internal binding flag
  bool          m_isSelectQuery;     // Internal SELECT  flag

  VarMap        m_parameters;        // Parameter map at execute
  RebindMap*    m_rebindParameters;  // Rebind map for datatypes of parameter bindings
  RebindMap*    m_rebindColumns;     // Rebind map for datatypes of result columns
  ColNumMap     m_numMap;            // column maps of the derived result set
  ColNameMap    m_nameMap;           // column by names

  // Lock database for multi-access from other threads
  // For as long as the current statement takes
  Locker<SQLDatabase> m_lock;
};

// Set the rebind map for datatypes (prior to executing SQL)
// Simply registers a std::map<int,int> to rebind types
inline void 
SQLQuery::SetRebindMap(RebindMap* p_map)
{
  m_rebindColumns = p_map;
}

inline void
SQLQuery::SetMaxRows(int p_maxrows)
{
  m_maxRows = p_maxrows;
}

inline bool
SQLQuery::IsOk() const
{
  return SQL_SUCCEEDED(m_retCode);
}

inline int  
SQLQuery::GetNumberOfColumns() const
{
  return (int)m_numMap.size();
}

inline CString
SQLQuery::GetError()
{
  return m_lastError;
}

inline ColNumMap* 
SQLQuery::GetBoundedColumns()
{
  return &m_numMap;
}

inline void 
SQLQuery::SetSpeedThreshold(double p_seconds)
{
  m_speedThreshold = p_seconds;
}

inline SQLVariant&
SQLQuery::operator[](int p_index)
{
  return *(GetColumn(p_index));
}