////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoOracle.h
//
// Copyright (c) 1992- 2014 ir. W.E. Huisman
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
#pragma once
#include "SQLInfoDB.h"

namespace SQLComponents
{

class SQLInfoOracle : public SQLInfoDB
{
public:
  SQLInfoOracle(SQLDatabase* p_database);
 ~SQLInfoOracle();
 
  //////////////////////////////////////////////////////////////////////////
  //
  // GENERALS (Strings & Booleans) 
  //
  //////////////////////////////////////////////////////////////////////////

  // Get the database type
  DatabaseType GetRDBMSDatabaseType() const;

  // The name of the database vendor
  CString GetRDBMSVendorName() const;

  // Get the physical database name
  CString GetRDBMSPhysicalDatabaseName() const;

  // System catalog is stored in uppercase in the database?
  bool    GetRDBMSIsCatalogUpper() const;

  // System catalog supports full ISO schemas (same tables per schema)
  bool    GetRDBMSUnderstandsSchemas() const;

  // Supports database/ODBCdriver comments in SQL
  bool    GetRDBMSSupportsComments() const;

  // Database can defer constraints until the end of a transaction
  bool    GetRDBMSSupportsDeferredConstraints() const;

  // Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
  bool    GetRDBMSSupportsOrderByExpression() const;

  // Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
  bool    GetRDBMSSupportsODBCCallEscapes() const;

  // If the database does not support the datatype TIME, it can be implemented as a DECIMAL
  bool    GetRDBMSSupportsDatatypeTime() const;

  // If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
  bool    GetRDBMSSupportsDatatypeInterval() const;

  // Gets the maximum length of an SQL statement
  unsigned long GetRDBMSMaxStatementLength() const;

  // KEYWORDS

  // Keyword for the current date and time
  CString GetKEYWORDCurrentTimestamp() const;

  // String for the current date
  CString GetKEYWORDCurrentDate() const;

  // Get the concatenation operator
  CString GetKEYWORDConcatanationOperator() const;

  // Get quote character for strings
  CString GetKEYWORDQuoteCharacter() const;

  // Get default NULL for parameter list input
  CString GetKEYWORDParameterDefaultNULL() const;

  // Parameter is for INPUT and OUTPUT in parameter list
  CString GetKEYWORDParameterINOUT() const;

  // Parameter is for OUTPUT only in parameter list
  CString GetKEYWORDParameterOUT() const;

  // Get datatype of the IDENTITY primary key in a Network database
  CString GetKEYWORDNetworkPrimaryKeyType() const;

  // Get datatype for timestamp (year to second)
  CString GetKEYWORDTypeTimestamp() const;

  // Prefix for a parameter in a stored procedure
  CString GetKEYWORDParameterPrefix() const;

  // Get select part to add new record identity to a table
  // Can be special column like 'OID' or a sequence select
  CString GetKEYWORDIdentityString(CString& p_tablename,CString p_postfix = "_seq") const;

  // Gets the UPPER function
  CString GetKEYWORDUpper(CString& p_expression) const;

  // Gets the construction for 1 minute ago
  CString GetKEYWORDInterval1MinuteAgo() const;

  // Gets the Not-NULL-Value statement of the database
  CString GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const;

  //////////////////////////////////////////////////////////////////////////
  //
  // CATALOG
  //
  //////////////////////////////////////////////////////////////////////////



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

  // BOOLEANS EN STRINGS
  // ===================

  // Get a query to create a temporary table from a select statement
  CString GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const;

  // Get the query to remove a temporary table indefinetly
  CString GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const;

  // Get a query to select into a temp table
  CString GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const;

  // Replace the Identity column OID by a sequence.nextval
  CString GetReplaceColumnOIDbySequence(CString p_columns,CString p_tablename,CString p_postfix = "_seq") const;

  // Remove catalog dependencies for stored procedures
  CString GetSQLRemoveProcedureDependencies(CString p_procname) const;

  // Remove field dependencies for calculated fields (bug in Firebird)
  CString GetSQLRemoveFieldDependencies(CString p_tablename) const;

  // Gets the table definition-form of a primary key
  CString GetPrimaryKeyDefinition(CString p_schema,CString p_tableName,bool p_temporary) const;

  // Get the constraint form of a primary key to be added to a table after creation of that table
  CString GetPrimaryKeyConstraint(CString p_schema,CString p_tablename,CString p_primary) const;
  CString GetPrimaryKeyConstraint(MPrimaryMap& p_primaries) const;

  // Get the SQL to add a foreign key to a table
  CString GetSQLForeignKeyConstraint(DBForeign&   p_foreign) const;
  CString GetSQLForeignKeyConstraint(MForeignMap& p_foreigns) const;

  // Get the SQL (if possible) to change the foreign key constraint
  CString GetSQLAlterForeignKey(DBForeign& p_origin,DBForeign& p_requested) const;

  // Performance parameters to be added to the database
  CString GetSQLPerformanceSettings() const;

  // SQL To set the caching mode of SQL results
  CString GetSQLCacheModeSetting(const CString& p_mode) const;

  // Needed for storing numbers in stored procedures
  CString GetSQLNlsNumericCharacters() const;

  // Gives the statement to alter a table columns' name
  CString GetSQLModifyColumnName(CString p_tablename,CString p_oldName,CString p_newName,CString p_datatype);
  
  // Gets the prefix needed for altering the datatype of a column in a MODIFY/ALTER
  CString GetModifyDatatypePrefix() const;

  // Code to define a table in row-locking mode
  CString GetCodeLockModeRow() const;

  // Code to create a temporary table with no logging
  CString GetCodeTempTableWithNoLog() const;

  // Granting all rights on a table (In a NON-ANSI database)
  CString GetSQLGrantAllOnTable(CString p_schema,CString p_tableName,bool p_grantOption = false);

  // Code prefix for a select-into-temp
  CString GetSelectIntoTempClausePrefix(CString p_tableName) const;

  // Code suffix for after a select-into-temp
  CString GetSelectIntoTempClauseSuffix(CString p_tableName) const;

  // Gets the fact if an IF statement needs to be bordered with BEGIN/END
  bool    GetCodeIfStatementBeginEnd() const;

  // Gets the end of an IF statement
  CString GetCodeEndIfStatement() const;

  // Gets a complete assignment statement.
  CString GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const;

  // Get SQL keyword to alter a column in a table
  CString GetCodeAlterColumn() const;
  
  // Get the code to start a WHILE-loop
  CString GetStartWhileLoop(CString p_condition) const;

  // Get the code to end a WHILE-loop
  CString GetEndWhileLoop() const;

  // Gets the fact if a SELECT must be in between parenthesis for an assignment
  bool    GetAssignmentSelectParenthesis() const;

  // Gets the construction / select for generating a new serial identity
  CString GetSQLGenerateSerial(CString p_table) const;

  // Gets the construction / select for the resulting effective generated serial
  CString GetSQLEffectiveSerial(CString p_identity) const;

  // Gets the sub transaction commands
  CString GetStartSubTransaction   (CString p_savepointName) const;
  CString GetCommitSubTransaction  (CString p_savepointName) const;
  CString GetRollbackSubTransaction(CString p_savepointName) const;

  // SQL CATALOG QUERIES
  // ===================

  // Get SQL to check if a stored procedure already exists in the database
  CString GetSQLStoredProcedureExists(CString& p_name) const;

  // Part of a query to select only 1 (one) record
  CString GetDualTableName () const;

  // FROM-Part for a query to select only 1 (one) record
  CString GetDualClause() const;

  // Gets DEFERRABLE for a constraint (or nothing)
  CString GetConstraintDeferrable () const;

  // Defer Constraints until the next COMMIT;
  CString GetSQLDeferConstraints() const;

  // Reset constraints back to immediate
  CString GetSQLConstraintsImmediate() const;

  // Get SQL to check if a table already exists in the database
  CString GetSQLTableExists(CString p_schema,CString p_tablename) const;

  // Get SQL to select all columns of a table from the catalog
  CString GetSQLGetColumns(CString& p_user,CString& p_tableName) const;

  // Get SQL to select all constraints on a table from the catalog
  CString GetSQLGetConstraintsForTable(CString& p_tableName) const;

  // Get SQL to read all indices for a table
  CString GetSQLTableIndices(CString p_user,CString p_tableName) const;

  // Get SQL to create an index for a table
  CString GetSQLCreateIndex(CString p_user,CString p_tableName,DBIndex* p_index) const;
  CString GetSQLCreateIndex(MStatisticsMap& p_indices) const;

  // Get extra filter expression for an index column
  CString GetIndexFilter(MetaStatistics& p_index) const;

  // Get SQL to drop an index
  CString GetSQLDropIndex(CString p_user,CString p_indexName) const;

  // Get SQL to read the referential constaints from the catalog
  CString GetSQLTableReferences(CString p_schema,CString p_tablename,CString p_constraint = "",int p_maxColumns = SQLINFO_MAX_COLUMNS) const;

  // Get the SQL to determine the sequence state in the database
  CString GetSQLSequence(CString p_schema,CString p_tablename,CString p_postfix = "_seq") const;

  // Create a sequence in the database
  CString GetSQLCreateSequence(CString p_schema,CString p_tablename,CString p_postfix = "_seq",int p_startpos = 1) const;

  // Remove a sequence from the database
  CString GetSQLDropSequence(CString p_schema,CString p_tablename,CString p_postfix = "_seq") const;

  // Gets the SQL for the rights on the sequence
  CString GetSQLSequenceRights(CString p_schema,CString p_tableName,CString p_postfix = "_seq") const;

  // Remove a stored procedure from the database
  void    DoRemoveProcedure(CString& p_procedureName) const;

  // Get SQL for your session and controlling terminal
  CString GetSQLSessionAndTerminal() const;

  // Get SQL to check if session number exists
  CString GetSQLSessionExists(CString sessieId) const;

  // Get SQL for unique session ID
  CString GetSQLUniqueSessionId(const CString& p_databaseName,const CString& p_sessionTable) const;

  // Get SQL for searching a session
  CString GetSQLSearchSession(const CString& p_databaseName,const CString& p_sessionTable) const;
  
  // See if a column exists within a table
  bool    DoesColumnExistsInTable(CString& p_owner,CString& p_tableName,CString& p_column) const;

  // Get SQL to get all the information about a Primary Key constraint
  CString GetSQLPrimaryKeyConstraintInformation(CString p_schema,CString p_tableName) const;

  // Does the named constraint exist in the database
  bool    DoesConstraintExist(CString p_constraintName) const;

  // Get SQL to lock a table
  CString GetSQLLockTable(CString& p_tableName,bool p_exclusive) const;

  // Get query to optimize the table statistics
  CString GetSQLOptimizeTable(CString& p_owner,CString& p_tableName,int& p_number);

  // Getting the fact that there is only **one** (1) user session in the database
  bool    GetOnlyOneUserSession();

  // Gets the triggers for a table
  CString GetSQLTriggers(CString m_schema,CString p_table) const;

  // SQL DDL STATEMENTS
  // ==================

  // Add a column to a table
  CString GetCreateColumn(CString p_schema,CString p_tablename,CString p_columnName,CString p_typeDefinition,bool p_notNull);

  // Drop a column from a table
  CString GetSQLDropColumn(CString p_schema,CString p_tablename,CString p_columnName) const;

  // Add a foreign key to a table
  CString GetCreateForeignKey(CString p_tablename,CString p_constraintname,CString p_column,CString p_refTable,CString p_primary);

  // Modify a column's definition in a table
  CString GetModifyColumnType(CString p_schema,CString p_tablename,CString p_columnName,CString p_typeDefinition);
  CString GetModifyColumnNull(CString p_schema,CString p_tablename,CString p_columnName,bool p_notNull);

  // Get the SQL to drop a view. If precursor is filled: run that SQL first!
  CString GetSQLDropView(CString p_schema,CString p_view,CString& p_precursor);

  // Create or replace a database view
  CString GetSQLCreateOrReplaceView(CString p_schema,CString p_view,CString p_asSelect) const;

  // Create or replace a trigger
  CString CreateOrReplaceTrigger(MetaTrigger& p_trigger) const;

  // SQL DDL OPERATIONS
  // ==================

  // Do the commit for the DDL commands in the catalog
  void    DoCommitDDLcommands() const;

  // Do the commit for the DML commands in the database
  void    DoCommitDMLcommands() const;

  // Remove a column from a table
  void    DoDropColumn(CString p_tableName,CString p_columName);

  // Does the named view exists in the database
  bool    DoesViewExists(CString& p_viewName);

  // Must create temporary tables runtime 
  bool    GetMustMakeTemptablesAtRuntime() const;

  // Create a temporary table in an optimized manner with the given index column
  void    DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const;

  // Remove a temporary table
  void    DoRemoveTemporaryTable(CString& p_tableName) const;

  // Create a procedure in the database
  void    DoMakeProcedure(CString& p_procName,CString p_table,bool p_noParameters,CString& p_codeBlock);

  // Rename a database table 
  void    DoRenameTable(CString& p_oldName,CString& p_newName) const;

  // PERSISTENT-STORED MODULES (SPL / PL/SQL)
  // ====================================================================

  // Get the user error text from the database
  CString GetUserErrorText(CString& p_procName) const;

  // Get assignment to a variable in SPL
  CString GetSPLAssignment(CString p_variable) const;

  // Get the start of a SPL While loop
  CString GetSPLStartWhileLoop(CString p_condition) const;

  // Get the end of a SPL while loop
  CString GetSPLEndWhileLoop() const;

  // Get stored procedure call
  CString GetSQLSPLCall(CString p_procName) const;

  // Build a parameter list for calling a stored procedure
  CString GetBuildedParameterList(size_t p_numOfParameters) const;

  // Parameter type for stored procedure for a given column type for parameters and return types
  CString GetParameterType(CString &p_type) const;

  // Makes a SQL string from a given string, with all the right quotes
  CString GetSQLString(const CString& p_string) const;

  // Get date string in engine format
  CString GetSQLDateString(int p_year,int p_month,int p_day) const;

  // Get time string in database engine format
  CString GetSQLTimeString(int p_hour,int p_minute,int p_second) const;
  
  // Get date-time string in database engine format
  CString GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const;

  // Get date-time bound parameter string in database format
  CString GetSQLDateTimeBoundString() const;

  // Stripped data for the parameter binding
  CString GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const;

  // Get the SPL sourcecode for a stored procedure as registered in the database
  CString GetSPLSourcecodeFromDatabase(const CString& p_owner,const CString& p_procName) const;

  // Get the SPL datatype for integer
  CString GetSPLIntegerType() const;

  // Get the SPL datatype for a decimal
  CString GetSPLDecimalType() const;
  
  // Get the SPL declaration for a cursor
  CString GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const;

  // Get the SPL cursor found row variable
  CString GetSPLCursorFound(CString& p_cursorName) const;

  // Get the SPL cursor row-count variable
  CString GetSPLCursorRowCount(CString& p_variable) const;

  // Get the SPL datatype for a declaration of a row-variable
  CString GetSPLCursorRowDeclaration(CString& p_cursorName,CString& p_variableName) const;

  CString GetSPLFetchCursorIntoVariables(CString                p_cursorName
                                        ,CString                p_variableName
                                        ,std::vector<CString>&  p_columnNames
                                        ,std::vector<CString>&  p_variableNames) const;

  // Fetch the current SPL cursor row into the row variable
  CString GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const;

  // Get the SPL no-data exception clause
  CString GetSPLNoDataFoundExceptionClause() const;

  // Get the SPL form of raising an exception
  CString GetSPLRaiseException(CString p_exceptionName) const;

  // Get the fact that the SPL has server functions that return more than 1 value
  bool    GetSPLServerFunctionsWithReturnValues() const;

  // Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
  SQLVariant* DoSQLCall(SQLQuery* p_query,CString& p_schema,CString& p_procedure);

  // SPECIALS
  // ==========================================================================
  
  // Translate database-errors to a human readable form
  CString TranslateErrortext(int p_error,CString p_errorText) const;
};

// End of namespace
}
