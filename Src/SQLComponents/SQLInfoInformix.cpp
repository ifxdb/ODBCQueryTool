////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoInformix.cpp
//
// Copyright (c) 1998-2017 ir. W.E. Huisman
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
// Last Revision:   08-01-2017
// Version number:  1.4.0
//
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLInfoInformix.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{
  
// Constructor. 
SQLInfoInformix::SQLInfoInformix(SQLDatabase* p_database)
                :SQLInfoDB(p_database)
{
}

// Destructor. Does nothing
SQLInfoInformix::~SQLInfoInformix()
{
}

//////////////////////////////////////////////////////////////////////////
//
// GENERALS (Strings & Booleans) 
//
//////////////////////////////////////////////////////////////////////////

// Get the database type
// DatabaseType GetDatabaseType() const;
DatabaseType
SQLInfoInformix::GetRDBMSDatabaseType() const
{
  return RDBMS_INFORMIX;
}

// The name of the database vendor
CString
SQLInfoInformix::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "IBM-Informix";
}

// Get the physical database name
CString
SQLInfoInformix::GetRDBMSPhysicalDatabaseName() const
{
  // Oops. We have an INFORMIX CLI bug
  // Versions of the INFORMIX CLI above 2.5 reflect in the connection string
  // only the name of the service of the DNS and not the 'real' database
  // That is why we read the database name from the sysmaster database
  // Beware that this only works for and above INFORMIX 7.x.y
  CString query = "SELECT scs_currdb\n"
                  "  FROM sysmaster:syssqlcurses\n"
                  " WHERE scs_sessionid = DBINFO('sessionid')";
  SQLQuery qry(m_database);
  qry.DoSQLStatement(query);
  if(qry.GetRecord())
  {
    return qry.GetColumn(1)->GetAsChar();
  }
  return CString("");
}

// System catalog is stored in uppercase in the database?
bool
SQLInfoInformix::GetRDBMSIsCatalogUpper() const
{
  return false;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoInformix::GetRDBMSUnderstandsSchemas() const
{
  return false;
}

// Supports database/ODBCdriver comments in SQL
bool
SQLInfoInformix::GetRDBMSSupportsComments() const
{
  // Does not work anymore from and above version 3.34 of KIT 2.70-TC3
  return false;
}

// Database can defer constraints until the end of a transaction
bool
SQLInfoInformix::GetRDBMSSupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED is supported
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
bool
SQLInfoInformix::GetRDBMSSupportsOrderByExpression() const
{
  return false;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoInformix::GetRDBMSSupportsODBCCallEscapes() const
{
  return true;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool
SQLInfoInformix::GetRDBMSSupportsDatatypeTime() const
{
  // Time can be implemented as TIME
  return true;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool
SQLInfoInformix::GetRDBMSSupportsDatatypeInterval() const
{
  // Interval supported
  return true;
}

// Gets the maximum length of an SQL statement
unsigned long 
SQLInfoInformix::GetRDBMSMaxStatementLength() const
{
  // The current limit of the INFORMIX ODBC driver is MAX_UINT16
  // We limit somewhat lower, just to be on the sure side
  return 64000;
}

// KEYWORDS

// Keyword for the current date and time
CString
SQLInfoInformix::GetKEYWORDCurrentTimestamp() const
{
  return "current";
}

// String for the current date
CString
SQLInfoInformix::GetKEYWORDCurrentDate() const
{
  return "current_date";
}

// Get the concatenation operator
CString
SQLInfoInformix::GetKEYWORDConcatanationOperator() const
{
  return "||";
}

// Get quote character for strings
CString
SQLInfoInformix::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get default NULL for parameter list input
CString
SQLInfoInformix::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return "";
}

// Parameter is for INPUT and OUTPUT in parameter list
CString
SQLInfoInformix::GetKEYWORDParameterINOUT() const
{
  return "";
}

// Parameter is for OUTPUT only in parameter list
CString
SQLInfoInformix::GetKEYWORDParameterOUT() const
{
  return "";
}

// Get datatype of the IDENTITY primary key in a Network database
CString
SQLInfoInformix::GetKEYWORDNetworkPrimaryKeyType() const
{
  return "serial";
}

// Get datatype for timestamp (year to second)
CString
SQLInfoInformix::GetKEYWORDTypeTimestamp() const
{
  return "datetime year to second";
}

// Prefix for a parameter in a stored procedure
CString
SQLInfoInformix::GetKEYWORDParameterPrefix() const
{
  return "";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
CString
SQLInfoInformix::GetKEYWORDIdentityString(CString& /*p_tablename*/,CString /*p_postfix*/ /*= "_seq"*/) const
{
  // Insert 0 for a SERIAL column
  return "0";
}

// Gets the UPPER function
CString
SQLInfoInformix::GetKEYWORDUpper(CString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString
SQLInfoInformix::GetKEYWORDInterval1MinuteAgo() const
{
  return "(CURRENT - INTERVAL (1) MINUTE TO MINUTE)";
}

// Gets the Not-NULL-Value statement of the database
CString
SQLInfoInformix::GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const
{
  return CString("NVL(") + p_test + "," + p_isnull + ")";
}

// Code prefix for a select-into-temp
CString
SQLInfoInformix::GetSQLSelectIntoTempPrefix(CString p_tableName) const
{
  return "";
}

// Code suffix for after a select-into-temp
CString
SQLInfoInformix::GetSQLSelectIntoTempSuffix(CString p_tableName) const
{
  return "INTO TEMP " + p_tableName + " WITH NO LOG\n";
}

// Gets the construction / select for generating a new serial identity
CString 
SQLInfoInformix::GetSQLGenerateSerial(CString p_table) const
{
  // Is generated by the database type SERIAL
  return "0";
}

// Gets the construction / select for the resulting effective generated serial
CString 
SQLInfoInformix::GetSQLEffectiveSerial(CString p_identity) const
{
  return "SELECT DBINFO('sqlca.sqlerrd1')\n"
         "  FROM systables WHERE tabid = 1";
}

// Gets the sub transaction commands
CString
SQLInfoInformix::GetSQLStartSubTransaction(CString p_savepointName) const
{
  return CString("SAVEPOINT ") + p_savepointName;
}

CString
SQLInfoInformix::GetSQLCommitSubTransaction(CString p_savepointName) const
{
  return CString("COMMIT TRANSACTION ") + p_savepointName;
}

CString
SQLInfoInformix::GetSQLRollbackSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TO SAVEPOINT ") + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
CString
SQLInfoInformix::GetSQLFromDualClause() const
{
  // Systables entry in systables is guaranteed to always be there!
  return " FROM systables WHERE tabid = 1";
}

//////////////////////////////////////////////////////////////////////////
//
// CATALOG
// o GetCATALOG<Object[s]><Function>
//   Objects
//   - Table
//   - Column
//   - Index
//   - PrimaryKey
//   - ForeignKey
//   - Trigger
//   - TemporaryTable 
//   - View
//   - Sequence
//  Functions per object type
//   - Exists
//   - List
//   - Attributes
//   - Create
//   - Alter  (where possible)
//   - Rename (where possible)
//   - Drop
//
//////////////////////////////////////////////////////////////////////////

// Get SQL to check if a table already exists in the database
CString
SQLInfoInformix::GetCATALOGTableExists(CString /*p_schema*/,CString p_tablename) const
{
  p_tablename.MakeLower();
  CString query = "SELECT count(*)\n"
                  "  FROM systables\n"
                  " WHERE tabname = '" + p_tablename + "'";
  return query;
}

CString
SQLInfoInformix::GetCATALOGTablesList(CString /*p_schema*/,CString p_pattern) const
{
  p_pattern.MakeLower();
  CString query = "SELECT tabname\n"
                  "  FROM systables\n"
                  " WHERE tabname like '" + p_pattern + "'";
  return query;
}

bool
SQLInfoInformix::GetCATALOGTableAttributes(CString /*p_schema*/,CString /*p_tablename*/,MetaTable& /*p_table*/) const
{
  // Getting the temp table status
  return false;
}

CString
SQLInfoInformix::GetCATALOGTableCreate(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return "";
}

// Rename a database table 
CString
SQLInfoInformix::GetCATALOGTableRename(CString /*p_schema*/,CString p_tablename,CString p_newname) const
{
  // Beware: 'TABLE' keyword in the statement
  CString sql("RENAME TABLE " + p_tablename + " TO " + p_newname);
  return sql;
}

CString
SQLInfoInformix::GetCATALOGTableDrop(CString /*p_schema*/,CString p_tablename) const
{
  return "DROP TABLE " + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

CString 
SQLInfoInformix::GetCATALOGColumnExists(CString p_schema,CString p_tablename,CString p_columnname) const
{
  p_tablename.MakeLower();
  p_columnname.MakeLower();
  CString query = "SELECT count(*)\n"
                  "  FROM systables  tab\n"
                  "      ,syscolumns col\n"
                  " WHERE tab.tabid = col.tabid\n"
                  "   AND tab.tabname = '" + p_tablename  + "'\n"
                  "   AND col.colname = '" + p_columnname + "'";
  return query;
}

CString 
SQLInfoInformix::GetCATALOGColumnList(CString p_schema,CString p_tablename) const
{
  CString sql = GetCATALOGColumnAttributes(p_schema,p_tablename,"");

  int pos = sql.ReverseFind('\n');
  sql = sql.Mid(pos + 1) + " ORDER BY col.colno";

  return sql;
}

CString 
SQLInfoInformix::GetCATALOGColumnAttributes(CString /*p_schema*/,CString p_tablename,CString p_columnname) const
{
  p_tablename.MakeLower();
  p_columnname.MakeLower();
  CString select = "SELECT col.colname\n"       // 1 -> Name
                   "      ,col.colno\n"         // 2 -> Position
                   "      ,col.coltype\n"       // 3 -> datatype
                   "      ,col.collength\n"     // 4 -> Length
                   "      ,def.default\n"       // 5 -> Default
                   "  FROM systables   tab\n"
                   "      ,syscolumns  col\n"
                   "      ,sysdefaults def\n"
                   " WHERE col.tabid   = tab.tabid\n"
                   "   AND def.tabid   = tab.tabid\n"
                   "   AND def.colno   = col.colno\n"
                   "   AND tab.tabname = '" + p_tablename  + "'\n"
                   "   AND col.colname = '" + p_columnname + "'";
                   // Beware: columnname filter must be last for previous method!
  return select;
}

CString 
SQLInfoInformix::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  CString sql = "ALTER TABLE "  + p_column.m_table  + "\n";
                "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  // m_default  not used
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

CString 
SQLInfoInformix::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  // The MODIFY keyword is a-typical
  CString sql = "ALTER  TABLE  " + p_column.m_table  + "\n";
                "MODIFY COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

CString 
SQLInfoInformix::GetCATALOGColumnRename(CString /*p_schema*/,CString p_tablename,CString p_columnname,CString p_newname,CString /*p_datatype*/) const
{
  CString sql("ALTER  TABLE  " + p_tablename + "\n"
              "RENAME " + p_columnname + " TO " + p_newname + "\n");
  return sql;
}

CString
SQLInfoInformix::GetCATALOGColumnDrop(CString p_schema,CString p_tablename,CString p_columnname) const
{
  CString sql("ALTER TABLE "  + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
CString
SQLInfoInformix::GetCATALOGIndexExists(CString p_schema,CString p_tablename,CString p_indexname) const
{
  return "";
}

CString
SQLInfoInformix::GetCATALOGIndexList(CString p_schema,CString p_tablename)   const
{
  CString query;
  p_tablename.MakeLower();

  // Reads all current indices in the database in a list
  // So we can figure out if an index need to be generated
  for(int ind = 1; ind <= INFORMIX_IDS_MAX_INDEX_COLUMNS; ++ind)
  {
    if(!query.IsEmpty())
    {
      query += "\nUNION ALL\n";
    }
    query.AppendFormat("SELECT idx.idxname AS index_name\n"
                       "      ,col.colname\n"
                       "      ,%d   AS index_column\n"
                       "      ,CASE WHEN idx.idxtype = 'D' THEN 0\n"
                       "            WHEN idx.idxtype = 'U' THEN 1\n"
                       "            ELSE 0 END AS is_unique\n"
                       "      ,CASE WHEN idx.part%d < 0 THEN 1\n"
                       "            ELSE 0 END AS descending\n"
                       "  FROM sysindexes idx\n"
                       "      ,systables  tab\n"
                       "      ,syscolumns col\n"
                       " WHERE tab.tabid = idx.tabid\n"
                       "   AND tab.tabname = '%s'\n"
                       "   AND col.tabid   = idx.tabid\n"
                       "   AND col.colno   = abs(idx.part%d)\n"
                       "   AND idx.idxname[1] != ' '"
                       ,ind
                       ,ind
                       ,p_tablename
                       ,ind);
  }
  return query;
}

CString
SQLInfoInformix::GetCATALOGIndexAttributes(CString p_schema,CString p_tablename,CString p_indexname) const
{
  return "";
}

CString
SQLInfoInformix::GetCATALOGIndexCreate(MIndicesMap& p_indices) const
{
  // Get SQL to create an index for a table
  // CREATE [UNIQUE] INDEX [<schema>.]indexname ON [<schema>.]tablename(column [ASC|DESC] [,...]);
  CString query;
  for(auto& index : p_indices)
  {
    if(index.m_position == 1)
    {
      // New index
      query = "CREATE ";
      if(index.m_unique)
      {
        query += "UNIQUE ";
      }
      query += "INDEX ";
      if(!index.m_schemaName.IsEmpty())
      {
        query += index.m_schemaName + ".";
      }
      query += index.m_indexName;
      query += " ON ";
      if(!index.m_schemaName.IsEmpty())
      {
        query += index.m_schemaName + ".";
      }
      query += index.m_tableName;
      query += "(";
    }
    else
    {
      query += ",";
    }
    query += index.m_columnName;
    if(index.m_ascending != "A")
    {
      query += " DESC";
    }
  }
  query += ")";
  return query;
}

CString
SQLInfoInformix::GetCATALOGIndexDrop(CString /*p_schema*/,CString /*p_tablename*/,CString p_indexname) const
{
  CString sql = "DROP INDEX " + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
CString
SQLInfoInformix::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

CString
SQLInfoInformix::GetCATALOGPrimaryExists(CString /*p_schema*/,CString p_tablename) const
{
  p_tablename.MakeLower();
  CString query = "SELECT COUNT(*)\n"
                  "  FROM systables      tab\n"
                  "      ,sysconstraints con\n"
                  " WHERE tab.tabid      = con.tabid\n"
                  "   AND tab.tabname    = '" + p_tablename + "'\n"
                  "   AND con.constrtype = 'P'";
  return query;
}

CString
SQLInfoInformix::GetCATALOGPrimaryAttributes(CString p_schema,CString p_tablename) const
{
  p_tablename.MakeLower();
  CString sql = "SELECT constrname\n"
                "      ,'' as indexname\n"
                "      ,'Y' as deferrable\n"
                "      ,'N' as initially_deferred\n"
                "  FROM systables      tab\n"
                "      ,sysconstraints con\n"
                " WHERE con.constrtype = 'P'\n"
                "   AND con.tabid      = tab.tabid\n"
                "   AND tab.tabname    = '" + p_tablename + "'";
  return sql;
}

CString
SQLInfoInformix::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  CString query("ALTER TABLE ");
  CString constraintName;

  for(auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
      if(!prim.m_schema.IsEmpty())
      {
        query += prim.m_schema + ".";
      }
      query += prim.m_table + "\n";
      query += "  ADD CONSTRAINT PRIMARY KEY (";
    }
    else
    {
      query += ",";
    }
    query += prim.m_columnName;
    constraintName = prim.m_constraintName;
  }
  query += ")\n";
  query += "      CONSTRAINT " + constraintName;

  return query;
}

CString
SQLInfoInformix::GetCATALOGPrimaryDrop(CString /*p_schema*/,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

CString
SQLInfoInformix::GetCATALOGForeignExists(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  return "";
}

CString
SQLInfoInformix::GetCATALOGForeignList(CString p_schema,CString p_tablename) const
{
  return "";
}

CString
SQLInfoInformix::GetCATALOGForeignAttributes(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  return "";
}

CString
SQLInfoInformix::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
  // Get first record
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct tablename
  CString table(foreign.m_fkTableName);
  CString primary(foreign.m_pkTableName);
  if(!foreign.m_fkSchemaName.IsEmpty())
  {
    table = foreign.m_fkSchemaName + "." + table;
  }
  if(!foreign.m_pkSchemaName.IsEmpty())
  {
    primary = foreign.m_pkSchemaName + "." + primary;
  }

  // The base foreign key command
  CString query = "ALTER TABLE " + table + "\n"
                  "  ADD CONSTRAINT FOREIGN KEY (";

  // Add the foreign key columns
  bool extra = false;
  for(auto& key : p_foreigns)
  {
    if(extra) query += ",";
    query += key.m_fkColumnName;
    extra  = true;
  }

  // Add references primary table
  query += ")\n      REFERENCES " + primary + "(";

  // Add the primary key columns
  extra = false;
  for(auto& key : p_foreigns)
  {
    if(extra) query += ",";
    query += key.m_pkColumnName;
    extra  = true;
  }
  query += ")\n";
  query += "      CONSTRAINT " + foreign.m_foreignConstraint;

  // Add all relevant options
  switch(foreign.m_deferrable)
  {
    case SQL_INITIALLY_DEFERRED:  query += "\n      NOVALIDATE"; break;
    default:                      break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE: query += "\n      ON DELETE CASCADE"; break;
    default:          // In essence: ON DELETE RESTRICT, but that's already the default
                      break;
  }
  return query;
}

CString
SQLInfoInformix::GetCATALOGForeignDrop(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// SQL/PSM
//
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//
// OLD INTERFACE
//
//////////////////////////////////////////////////////////////////////////

// BOOLEANS AND STRINGS
// ===================================================================

// Get a query to create a temporary table from a select statement
CString 
SQLInfoInformix::GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const
{
  return p_select + " INTO TEMP " + p_tablename + " WITH NO LOG";
}

// Get the query to remove a temporary table indefinetly
// BEWARE: Must be executed with a multi-statement stack!
CString
SQLInfoInformix::GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const
{
  // 'Simply' a drop of a table
  ++p_number;
  return "DROP TABLE " + p_tablename + ";\n";
}

// Get a query to select into a temp table
CString 
SQLInfoInformix::GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const
{
  return p_select + " INTO TEMP " + p_tablename;
}

// Get the sql (if possible) to change the foreign key constraint
CString 
SQLInfoInformix::GetSQLAlterForeignKey(DBForeign& p_origin,DBForeign& p_requested) const
{
  CString query = "SET CONSTRAINTS " + p_origin.m_constraintname + " ";

  // Add all relevant options
  if(p_origin.m_initiallyDeffered != p_requested.m_initiallyDeffered)
  {
    query += p_requested.m_initiallyDeffered ? "DEFERRED" : "IMMEDIATE";
  }
  else if(p_origin.m_enabled != p_requested.m_enabled)
  {
    query += p_requested.m_enabled ? "ENABLED" : "DISABLED";
  }
  return query;
}

// Gets the fact if an IF statement needs to be bordered with BEGIN/END
bool
SQLInfoInformix::GetCodeIfStatementBeginEnd() const
{
  // IF THEN ELSE END IF; does not need a BEGIN/END per se.
  return false;
}

// Gets the end of an IF statement
CString 
SQLInfoInformix::GetCodeEndIfStatement() const
{
  return "END IF;\n";
}

// Gets a complete assignment statement.
CString 
SQLInfoInformix::GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const
{
  return "LET " + p_destiny + " = " + p_source + ";";
}

// Get the code to start a WHILE-loop
CString
SQLInfoInformix::GetStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + "\n";
}

// Get the code to end a WHILE-loop
CString
SQLInfoInformix::GetEndWhileLoop() const
{
  return "END WHILE;\n";
}

// Gets the fact if a SELECT must be in between parenthesis for an assignment
bool 
SQLInfoInformix::GetAssignmentSelectParenthesis() const
{
  // waarde = (SELECT max kenmerk FROM tabel);
  return true;
}

// SQL CATALOG QUERIES
// ==================================================================

// Get SQL to check if a stored procedure already exists in the database
CString
SQLInfoInformix::GetSQLStoredProcedureExists(CString& p_name) const
{
  CString nameLower(p_name);
  nameLower.MakeLower();
  return    "select count(*)\n"
            "  from sysprocedures\n"
            " where procname='" + nameLower + "'";
}

// Gets DEFERRABLE for a constraint (or nothing)
CString
SQLInfoInformix::GetConstraintDeferrable() const
{
  return "";
}

// Defer Constraints until the next COMMIT;
CString 
SQLInfoInformix::GetSQLDeferConstraints() const
{
  return "SET CONSTRAINTS ALL DEFERRED";
}

// Reset constraints back to immediate
CString 
SQLInfoInformix::GetSQLConstraintsImmediate() const
{
  return "SET CONSTRAINTS ALL IMMEDIATE";
}

// Get SQL to select all constraints on a table from the catalog
CString 
SQLInfoInformix::GetSQLGetConstraintsForTable(CString& p_tableName) const
{
  CString lowerName(p_tableName);
  lowerName.MakeLower();
  CString contabel = "SELECT con.constrname "
                    "      ,con.tabid "
                    "      ,con.constrid "
                    "  FROM sysconstraints con "
                    "      ,systables tab "
                    " WHERE tab.tabid   = con.tabid "
                    "   AND con.constrtype in ('C', 'U') "
                    "   AND tab.tabname = '" + lowerName + "'";
  return contabel;
}

// Get SQL to read the referential constraints from the catalog
CString 
SQLInfoInformix::GetSQLTableReferences(CString p_schema
                                      ,CString p_tablename
                                      ,CString p_constraint /*=""*/
                                      ,int     p_maxColumns /*= SQLINFO_MAX_COLUMNS*/) const
{
  CString query;
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraint.MakeLower();

  for(int ind = 1;ind <= p_maxColumns; ++ind)
  {
    CString part;
    part.Format("SELECT con.constrname  AS foreign_key_constraint\n"
                "      ,tab.owner       AS schema_name\n"
                "      ,tab.tabname     AS table_name\n"
                "      ,col.colname     AS column_name\n"
                "      ,pri.tabname     AS primary_table_name\n"
                "      ,pcl.colname     AS primary_column_name\n"
                "      ,1               AS deferrable\n"
                "      ,0               AS initially_deferred\n"
                "      ,1               AS enabled\n"
                "      ,CASE WHEN ref.matchtype = 'N' THEN 0\n"
                "            WHEN ref.matchtype = 'P' THEN 1\n"
                "            ELSE 0\n"
                "       END  AS match_option\n"
                "      ,CASE WHEN ref.updrule = 'R' THEN 1\n"
                "            WHEN ref.updrule = 'C' THEN 0\n"
                "            WHEN ref.updrule = 'N' THEN 2\n"
                "            WHEN ref.updrule = 'D' THEN 4\n"
                "            ELSE 0\n"
                "       END  AS update_rule\n"
                "      ,CASE WHEN ref.delrule = 'R' THEN 1\n"
                "            WHEN ref.delrule = 'C' THEN 0\n"
                "            WHEN ref.delrule = 'N' THEN 2\n"
                "            WHEN ref.delrule = 'D' THEN 4\n"
                "            ELSE 0\n"
                "       END  AS delete_rule\n"
                "  FROM sysconstraints con\n"
                "      ,systables      tab\n"
                "      ,syscolumns     col\n"
                "      ,sysreferences  ref\n"
                "      ,systables      pri\n"
                "      ,sysindexes     idx\n"
                "      ,sysconstraints pcn\n"
                "      ,sysindexes     pix\n"
                "      ,syscolumns     pcl\n"
                " WHERE tab.tabid      = con.tabid\n"
                "   AND con.constrid   = ref.constrid\n"
                "   AND ref.ptabid     = pri.tabid\n"
                "   AND con.idxname    = idx.idxname\n"
                "   AND col.tabid      = tab.tabid\n"
                "   AND col.colno      = idx.part%d\n"
                "   AND pcn.tabid      = pri.tabid\n"
                "   AND pix.idxname    = pcn.idxname\n"
                "   AND pcl.tabid      = pri.tabid\n"
                "   AND pcl.colno      = pix.part%d\n"
                "   and con.constrtype = 'R'\n"
                "   AND pcn.constrtype = 'P'\n"
               ,ind
               ,ind);
    if(!p_schema.IsEmpty())
    {
      part += "\n    AND tab.owner = '" + p_schema + "'";
    }
    if(!p_tablename.IsEmpty())
    {
      part += "\n   AND tab.tabname    = '" + p_tablename + "'";
    }
    if(!p_constraint.IsEmpty())
    {
      part += "\n    AND con.constrname = '" + p_constraint + "'";
    }
    // Add to the query
    if(!query.IsEmpty())
    {
      query += "\nUNION ALL\n";
    }
    query += part;
  }
  return query;
}

// Get the SQL to determine the sequence state in the database
CString 
SQLInfoInformix::GetSQLSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/) const
{
  p_schema.MakeLower();
  CString sequence = p_tablename + p_postfix;
  sequence.MakeLower();
  CString sql = "SELECT dom.name as sequence_name\n"
                "      ,seq.start_val as current_value"
                "      ,decode(seq.inc_val,1,1,0) *\n"
                "       decode(seq.cycle,'0',1,0) *\n"
                "       decode(seq.cache, 0, 1,0) *\n"
                "       decode(seq.order,'1',1,0) as is_correct\n"
                "  FROM syssequences seq\n"
                "      ,sysdomains   dom\n"
                " WHERE dom.id = seq.id\n"
                "   AND dom.owner = '" + p_schema + "'\n"
                "   AND dom.name  = '" + sequence + "'\n";
              //"   AND dom.type  = 3"; ??
  return sql;
}

// Create a sequence in the database
CString 
SQLInfoInformix::GetSQLCreateSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/,int p_startpos) const
{
  CString sql("CREATE SEQUENCE ");
  CString sequence = p_tablename + p_postfix;

  if(!p_schema.IsEmpty())
  {
    sql += p_schema + ".";
  }
  sql += sequence;
  sql.AppendFormat(" START WITH %d",p_startpos);
  sql += " NOCYCLE NOCACHE ORDER";
  return sql;
}

// Remove a sequence from the database
CString 
SQLInfoInformix::GetSQLDropSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/) const
{
  CString sequence = p_tablename + p_postfix;
  CString sql;
  sql = "DROP SEQUENCE " + p_schema + "." + sequence;
  return sql;
}

// Gets the SQL for the rights on the sequence
CString
SQLInfoInformix::GetSQLSequenceRights(CString p_schema,CString p_tablename,CString p_postfix /*="_seq"*/) const
{
  CString sequence = p_tablename + p_postfix;
  if(!p_schema.IsEmpty())
  {
    sequence = p_schema + "." + sequence;
  }
  CString sql = "GRANT SELECT ON " + sequence + " TO " + GetGrantedUsers();
  return sql;
}

// Remove a stored procedure from the database
void
SQLInfoInformix::DoRemoveProcedure(CString& p_procedureName) const
{
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement("DROP PROCEDURE " + p_procedureName);

}

// Get SQL for your session and controlling terminal
CString
SQLInfoInformix::GetSQLSessionAndTerminal() const
{
  CString query = "SELECT sid\n"
                  "      ,tty\n"
                  "  FROM sysmaster:syssessions\n"
                  " WHERE sid =\n"
                  "     ( SELECT dbinfo('sessionid')\n"
                  "         FROM systables\n"
                  "        WHERE tabid=1)";
  return query;
}

// Get SQL to check if session number exists
CString 
SQLInfoInformix::GetSQLSessionExists(CString sessieId) const
{
  return "SELECT COUNT(*)\n"
         "  FROM sysmaster:syssessions\n"
         " WHERE sid = " + sessieId;
}

// Get SQL for unique session ID
CString 
SQLInfoInformix::GetSQLUniqueSessionId(const CString& p_databaseName,const CString& /*p_sessionTable*/) const
{
  // INFORMIX has sessions on more than one database in the sysmaster table
  // so we must limit on the current database that's in use
  return "SELECT UNIQUE(odb_sessionid)\n"
         "  FROM sysmaster:sysopendb\n"
         " WHERE odb_dbname = '" + p_databaseName + "'";
}

// Get SQL for searching a session
CString 
SQLInfoInformix::GetSQLSearchSession(const CString& p_databaseName,const CString& p_sessionTable) const
{
  return "SELECT  sid\n"
         "       ,username\n"
         "       ,tty\n"
         "  FROM  sysmaster:syssessions\n"
         " WHERE  sid IN\n"
         "      ( SELECT odb_sessionid\n"
         "          FROM sysmaster:sysopendb\n"
         "         WHERE odb_dbname = '" + p_databaseName + "'\n"
         "           AND NOT odb_sessionid IN\n"
         "             ( SELECT sessie_nr\n"
         "                 FROM " + p_sessionTable +"))";
}

// Does the named constraint exist in the database
bool    
SQLInfoInformix::DoesConstraintExist(CString p_constraintName) const
{
  CString constraint(p_constraintName);
  constraint.MakeLower();

  SQLQuery qry(m_database);
  qry.DoSQLStatement("SELECT 1\n"
                     "  FROM sysconstraints\n"
                     " WHERE constrname = '" + p_constraintName + "'\n");
  return qry.GetRecord();
}

// Gets the lock-table query
CString 
SQLInfoInformix::GetSQLLockTable(CString& p_tableName,bool p_exclusive) const
{
  CString query = "LOCK TABLE " + p_tableName + " IN "  ;
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
CString 
SQLInfoInformix::GetSQLOptimizeTable(CString& /*p_owner*/,CString& p_tableName,int& p_number)
{
  CString optim;
  optim = "UPDATE STATISTICS LOW  FOR TABLE " + p_tableName + " DROP DISTRIBUTIONS;\n"
          "UPDATE STATISTICS HIGH FOR TABLE " + p_tableName + ";\n";
  p_number += 2;
  return optim;
}

// Getting the fact that there is only **one** (1) user session in the database
bool
SQLInfoInformix::GetOnlyOneUserSession()
{
  // Yet to implement
  return true;
}

// Gets the triggers for a table
CString
SQLInfoInformix::GetSQLTriggers(CString p_schema,CString p_table) const
{
  return "";
}

// SQL DDL STATEMENTS
// ==================

// Add a foreign key to a table
CString 
SQLInfoInformix::GetCreateForeignKey(CString p_tablename,CString p_constraintname,CString p_column,CString p_refTable,CString p_primary)
{
  CString sql = "ALTER TABLE " + p_tablename + "\n"
                "  ADD CONSTRAINT FOREIGN KEY (" + p_column + ")\n"
                "      REFERENCES " + p_refTable + "(" + p_primary + ")\n"
                "      CONSTRAINT + " + p_constraintname;
  return sql;
}

// Get the SQL to drop a view. If precursor is filled: run that SQL first!
CString 
SQLInfoInformix::GetSQLDropView(CString /*p_schema*/,CString p_view,CString& p_precursor)
{
  p_precursor.Empty();
  return "DROP VIEW " + p_view;
}

// Create or replace a database view
CString 
SQLInfoInformix::GetSQLCreateOrReplaceView(CString /*p_schema*/,CString p_view,CString p_asSelect) const
{
  return "CREATE VIEW " + p_view + "\n" + p_asSelect;
}

// Create or replace a trigger
CString
SQLInfoInformix::CreateOrReplaceTrigger(MetaTrigger& /*p_trigger*/) const
{
  return "";
}

// SQL DDL ACTIONS
// ====================================================================

// Do the commit for the DDL commands in the catalog
void 
SQLInfoInformix::DoCommitDDLcommands() const
{
  // Does NOTHING in INFOMRIX and cannot do something
  // Commit follows from a BEGIN/COMMIT transaction by AUTOCOMMIT from the driver
}

// Do the commit for the DML commands in the database
// ODBC driver auto commit mode will go wrong!!
void
SQLInfoInformix::DoCommitDMLcommands() const
{
  // Does NOTHING in INFOMRIX and cannot do something
  // Commit follows from a BEGIN/COMMIT transaction by AUTOCOMMIT from the driver
}

// Does the named view exists in the database
bool
SQLInfoInformix::DoesViewExists(CString& p_viewName)
{
  CString lowerName(p_viewName);
  lowerName.MakeLower();

  CString query = "SELECT count(*)\n"
                  "  FROM sysviews\n"
                  " WHERE viewname = '" + lowerName + "'";
  SQLQuery sql(m_database);
  sql.DoSQLStatement(query);
  if(sql.GetRecord())
  {
    return sql.GetColumn(1)->GetAsSLong() > 0;
  }
  return false;
}

// Must create temporary tables runtime 
bool 
SQLInfoInformix::GetMustMakeTemptablesAtRuntime() const
{
  // FALSE: GLOBAL TEMPORARY TABLES IN THE ENGINE
  // TRUE:  TRUE SESSION TEMPTABLES MUST ALWAYS BE CREATED (Informix)
  return true;
}

// Create a temporary table in an optimized manner with the givven index column
void
SQLInfoInformix::DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const
{
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement("DROP TABLE " + p_tableName);
  CString create = "CREATE TEMP TABLE " + p_tableName + " " + p_content;
  try
  {
    sql.DoSQLStatement(create);
    if(p_indexColumn != "")
    {
      create = "CREATE INDEX " + p_tableName + "_" + p_indexColumn + " ON " + p_tableName + "(" + p_indexColumn + ")";
      sql.DoSQLStatement(create);
    }
  }
  catch(...)
  {
    throw CString("Cannot make temporary table: " + p_tableName);
  }
}

// Remove a temporary table
void
SQLInfoInformix::DoRemoveTemporaryTable(CString& p_tableName) const
{
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement("DROP TABLE " + p_tableName);
}

// Create a procedure in the database
void 
SQLInfoInformix::DoMakeProcedure(CString& p_procName,CString /*p_table*/,bool /*p_noParameters*/,CString& p_codeBlock)
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement(p_codeBlock);
  sql.DoSQLStatement("GRANT EXECUTE ON " + p_procName + " TO " + GetGrantedUsers());
}

// PERSISTENT-STORED MODULES (SPL / PL/SQL)
// =====================================================================

// Get the user error text from the database
CString 
SQLInfoInformix::GetUserErrorText(CString& p_procName) const
{
  (void)p_procName;   // Not in Informix.
  return "";
}

// Get assignment to a variable in SPL
CString 
SQLInfoInformix::GetSPLAssignment(CString p_variable) const
{
  return "LET " + p_variable + " = ";
}

// Get the start of a SPL While loop
CString 
SQLInfoInformix::GetSPLStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + "\n";
}

// Get the end of a SPL while loop
CString 
SQLInfoInformix::GetSPLEndWhileLoop() const
{
  return "END WHILE;\n";
}

// Get stored procedure call
CString 
SQLInfoInformix::GetSQLSPLCall(CString p_procName) const
{
  return "execute procedure " + p_procName;
}

// Build a parameter list for calling a stored procedure
CString 
SQLInfoInformix::GetBuildedParameterList(size_t p_numOfParameters) const
{
  // Stands for the '?' binding parameters in a stored-procedure
  // ALWAYS an ellipsis, even if there are no parameters!
  CString strParamLijst = "(";
  for (size_t i = 0; i < p_numOfParameters; i++)
  {
    if(i) 
    {
      strParamLijst += ",";
    }
    strParamLijst += "?";
  }
  strParamLijst += ")";

  return strParamLijst;
}

// Parameter type for stored procedure for a given column type for parameters and return types
CString
SQLInfoInformix::GetParameterType(CString& p_type) const
{
  return p_type;
}

// GENERAL SQL ACTIONS
// =================================================================

// Makes a SQL string from a given string, with all the right quotes
CString 
SQLInfoInformix::GetSQLString(const CString& p_string) const
{
  CString str = p_string;
  str.Replace("'","''");
  CString kwoot = GetKEYWORDQuoteCharacter();
  return kwoot + str + kwoot;
}

// Get date string in engine format
CString 
SQLInfoInformix::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  // Informix used to be depended on the DBFORMAT parameter
  // This form is independent of it's setting!
  CString dateString;
  dateString.Format("DATETIME(%04d-%02d-%02d) YEAR TO DAY",p_year,p_month,p_day);
  return dateString;
}

// Get time string in database engine format
CString 
SQLInfoInformix::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("DATETIME (%02d:%02d:%02d) HOUR TO SECOND",p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
CString 
SQLInfoInformix::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  // Informix used to be depended on the DBFORMAT parameter
  // This form is independent of it's setting!
  CString string;
  string.Format("DATETIME(%04d-%02d-%02d %02d:%02d:%02d) YEAR TO SECOND"
                ,p_day,p_month,p_year
                ,p_hour,p_minute,p_second);
  return string;
}

// Get date-time bound parameter string in database format
CString 
SQLInfoInformix::GetSQLDateTimeBoundString() const
{
  return "TO_DATE(?,'%d-%m-%Y %H:%M:%S')";
}

// Stripped data for the parameter binding
CString
SQLInfoInformix::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString string;
  string.Format("%04d-%02d-%02d %02d:%02d:%02d"
                ,p_day,p_month,p_year
                ,p_hour,p_minute,p_second);
  return string;
}

// Get the SPL sourcecode for a stored procedure as registered in the database
CString 
SQLInfoInformix::GetSPLSourcecodeFromDatabase(const CString& /*p_owner*/,const CString& p_procName) const
{
  CString sQuery;  
  CString sName(p_procName);
  sName.MakeLower();
  sQuery = "SELECT sbody.data\n"
           "  FROM sysprocbody sbody\n"
           "      ,sysprocedures sproc\n"
           " WHERE sbody.procid   = sproc.procid\n"
           "   AND sproc.procname = '" + sName + "'\n"
           "   AND datakey        = 'T'";
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement(sQuery);
  CString sProcBody;
  while (sql.GetRecord())
  {
    sProcBody = sql.GetColumn(1)->GetAsChar();
  }
  return sProcBody;
}

// Get the SPL datatype for integer
CString 
SQLInfoInformix::GetSPLIntegerType() const
{
  return "integer";
}

// Get the SPL datatype for a decimal
CString 
SQLInfoInformix::GetSPLDecimalType() const
{
  return "decimal";
}

// Get the SPL declaration for a cursor
CString 
SQLInfoInformix::GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const
{
  return "CURSOR " + p_variableName + " IS " + p_query + ";";
}

// Get the SPL cursor found row parameter
CString 
SQLInfoInformix::GetSPLCursorFound(CString& /*p_cursorName*/) const
{
  // TODO: To be implemented
  return "";
}

// Get the SPL cursor row-count variable
CString 
SQLInfoInformix::GetSPLCursorRowCount(CString& /*p_variable*/) const
{
  // TODO: To be implemented
  return "";
}

// Get the SPL datatype for a declaration of a row-variable
CString 
SQLInfoInformix::GetSPLCursorRowDeclaration(CString& /*p_cursorName*/,CString& /*p_variableName*/) const
{
  // TODO: To be implemented
  return "";
}

CString 
SQLInfoInformix::GetSPLFetchCursorIntoVariables(CString               p_cursorName
                                               ,CString             /*p_variableName*/
                                               ,std::vector<CString>& p_columnNames
                                               ,std::vector<CString>& p_variableNames) const
{
  // TODO: CHeck
  CString query = "FETCH " + p_cursorName + " INTO ";  

  std::vector<CString>::iterator cNames;
  std::vector<CString>::iterator vNames;
  bool moreThenOne = false;

  for(cNames  = p_columnNames.begin(), vNames  = p_variableNames.begin();
    cNames != p_columnNames.end() && vNames != p_variableNames.end();
    ++cNames, ++vNames)
  {
    query += (moreThenOne ? "," : "") + *vNames;
  }
  query += ";";
  return query;
}

// Fetch the current SPL cursor row into the row variable
CString 
SQLInfoInformix::GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const
{ 
  return "FETCH " + p_cursorName + " INTO " + p_variableName+ ";";
}

// Get the SPL no-data exception clause
CString 
SQLInfoInformix::GetSPLNoDataFoundExceptionClause() const
{
  // TODO: Check
  return "WHEN NO_DATA THEN";
}

// Get the SPL form of raising an exception
CString 
SQLInfoInformix::GetSPLRaiseException(CString p_exceptionName) const
{
  // TODO: Check
  return "RAISE " + p_exceptionName + ";";
}

// Get the fact that the SPL has server functions that return more than 1 value
bool    
SQLInfoInformix::GetSPLServerFunctionsWithReturnValues() const
{
  return true;
}

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoInformix::DoSQLCall(SQLQuery* /*p_query*/,CString& /*p_schema*/,CString& /*p_procedure*/)
{
  return nullptr;
}

// SPECIALS
// ==========================================================================

// Translate database-errors to a human readable form
CString 
SQLInfoInformix::TranslateErrortext(int p_error,CString p_errorText) const
{
  // Check if we have work to do
  if(p_error == 0)
  {
    return p_errorText;
  }

  CString errorText;
  errorText.Format("ODBC error [%d:%s]",p_error,p_errorText);
  return errorText;
}

// End of namespace
}
