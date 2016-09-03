////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoPostgreSQL.cpp
//
// Copyright (c) 1998- 2014 ir. W.E. Huisman
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
// Last Revision:   01-01-2015
// Version number:  1.1.0
//
#include "stdafx.h"
#include "SQLInfoPostgreSQL.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constructor. Doet nog niets
SQLInfoPostgreSQL::SQLInfoPostgreSQL(SQLDatabase* p_database)
                  :SQLInfoDB(p_database)
{
}

// Destructor. Doet nog niets.
SQLInfoPostgreSQL::~SQLInfoPostgreSQL()
{
}

// Geef SQLDatabaseType
DatabaseType 
SQLInfoPostgreSQL::GetDatabaseType() const
{
  return RDBMS_POSTGRESQL;
}

// BOOLEANS AND STRINGS
// ====================================================================

CString
SQLInfoPostgreSQL::GetDatabaseVendorName() const
{
  return "PostgreSQL";
}

// Geef de fysieke database naam
CString 
SQLInfoPostgreSQL::GetFysicalDatabaseName() const
{
  SQLQuery sql(m_database);
  CString query = "SELECT current_database()";
  sql.DoSQLStatement(query);
  if(sql.GetRecord())
  {
    return sql.GetColumn(1)->GetAsChar();
  }
  return "";
}

// Wordt de inhoud van de systeemcatalogus in uppercase opgeslagen?
bool 
SQLInfoPostgreSQL::IsCatalogUpper () const
{
  return false;
}

// Ondersteund database/ODBCdriver commentaar in sql
bool 
SQLInfoPostgreSQL::SupportsDatabaseComments() const
{
  return false;
}

// Database is in staat constraints op te schorten tot aan het eind v.d. transactie
bool 
SQLInfoPostgreSQL::SupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED aanwezig
  return true;
}

// Database kan ORDER BY met een expressie aan, dus bijv. UPPER(kolom). Alternatief
// is het selecteren van een kolom expressie AS naampje en dan ORDER BY naampje.
bool
SQLInfoPostgreSQL::SupportsOrderByExpression() const
{
  return true;
}

// Catalogus tabel met alle default waarden van een kolom in de database
CString 
SQLInfoPostgreSQL::GetSQLStringDefaultValue(CString p_tableName,CString p_columnName) const
{
  CString query = "SELECT adsrc\n"
                  "  FROM pg_attrdef, pg_attribute, pg_class\n"
                  " WHERE pg_attrdef.adrelid    = pg_attribute.attrelid\n"
                  "   AND pg_attrdef.adnum      = pg_attribute.attnum\n"
                  "   AND pg_attribute.attrelid = pg_class.oid\n"
                  "   AND pg_class.relname      = '" + p_tableName + "'\n"
                  "   AND pg_attribute.attname  = '" + p_columnName + "'";                 
  return query;
}

// Keyword voor huidige datum en tijd
CString 
SQLInfoPostgreSQL::GetSystemDateTimeKeyword() const
{
  return "current_timestamp(0)";
} 

// String voor huidige datum
CString
SQLInfoPostgreSQL::GetSystemDateString() const
{
  return "current_date";
}

// Geeft het feit of TIJD op de database als een DECIMAL is geimplementeerd
bool 
SQLInfoPostgreSQL::GetTimeIsDecimal() const
{
  // TIJD is geimplementeerd als TIME en NIET als DECIMAL
  return false;
}

// Geeft het feit of INTERVAL op de database als een DECIMAL is geimlementeerd
bool 
SQLInfoPostgreSQL::GetIntervalIsDecimal() const
{
  // Interval is geimplementeerd als INTERVAL en NIET als DECIMAL
  return false;
}

// Geef het karakter voor string concatenatie
CString 
SQLInfoPostgreSQL::GetConcatanationOperator() const
{
  return "||";    
}

// Geef quote (hoe schrijf je dat) karakter voor strings
CString 
SQLInfoPostgreSQL::GetQuoteCharacter() const
{
  return "'";    
}

// Geef default NULL voor invoer parameterlijst
CString 
SQLInfoPostgreSQL::GetDefaultNULL() const
{
  return "";    
}

// Geef INOUT voor invoer/uitvoer parameterlijst
CString 
SQLInfoPostgreSQL::GetParameterINOUT() const
{
  return "";    
}
  
// Geef OUT voor invoer/uitvoer parameterlijst
CString 
SQLInfoPostgreSQL::GetParameterOUT() const
{
  return "";    
}

// Geef datatype van "h_gebruiker" in een stored procedure als parameter
CString 
SQLInfoPostgreSQL::GetAuditUserDatatype() const
{
  return "VARCHAR";      // of char(8) ? Volgens Lo wel. Lo op 9-1 veranderd in 5.4
} 

// Geef datatype van "h_gebruiker" in een stored procedure (extern) als variabele
CString 
SQLInfoPostgreSQL::GetAuditUserDatatypeAsVariable() const
{
  return "VARCHAR(50)";
} 

// Geef datatype van de primary key
CString 
SQLInfoPostgreSQL::GetPrimaryKeyType() const
{
  return "INTEGER";
}

// Geef datatype voor Moment
CString 
SQLInfoPostgreSQL::GetDatetimeYearToSecondType() const
{
  return "TIMESTAMP";
}

// Seperator tussen twee alter-constraints in een alter-table statement
CString
SQLInfoPostgreSQL::GetAlterConstraintSeparator() const
{
  return ",";
}

// Inner Join Keyword
CString 
SQLInfoPostgreSQL::GetInnerJoinKeyword() const
{
  return "INNER JOIN ";
}

// Outer join keyword
CString  
SQLInfoPostgreSQL::GetOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN ";
}

// Inner Join Keyword for use in views.
CString 
SQLInfoPostgreSQL::GetViewInnerJoinKeyword() const
{
  return "INNER JOIN";
}

// Outer join keyword for use in views
CString 
SQLInfoPostgreSQL::GetViewOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN";
}

// Geef closure voor outer-join
CString 
SQLInfoPostgreSQL::GetOuterJoinClosure() const
{
  return "";
}

// Outer join teken
CString  
SQLInfoPostgreSQL::GetOuterJoinSign() const
{
  return "";
}

// Prefix voor gebruik van een parameter in een stored procedure
CString
SQLInfoPostgreSQL::GetSPParamPrefix() const
{
  return "";
}

// Geef de query om een nieuw record toe te voegen in de kolom OID
CString 
SQLInfoPostgreSQL::GetIdentityString(CString& p_tablename,CString p_postfix /*="_seq"*/) const
{
  return "nextval('" + p_tablename + p_postfix + "')";
}

// Geef de query om een tijdelijke tabel uit een select statement te maken
CString 
SQLInfoPostgreSQL::GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const
{
  return "CREATE GLOBAL TEMPORARY TABLE " + p_tablename + "\nAS " + p_select;
}

// Geef de query om een tijdelijke tabel definitief te verwijderen
// LET OP moet uitgevoerd met DBRecordSet::VoerSqlCommandosUit!!!
CString
SQLInfoPostgreSQL::GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const
{
  // Dit werkt in PostgreSQL alleen zo, omdat er anders een foutmelding komt dat
  // de tijdelijke tabel nog in gebruik is en de tabeldefinitie dan in de
  // catalog van de RDBMS blijft staan. De definitie is dan strijdig met een
  // mogelijke volgende definitie onder deze naam
  p_number += 3;
  return "DELETE FROM "    + p_tablename + ";\n"
         "TRUNCATE TABLE " + p_tablename + ";\n"
         "DROP TABLE "     + p_tablename + ";\n";
}

// Geef de query om in een tijdelijke tabel te selecteren
CString 
SQLInfoPostgreSQL::GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const
{
   return "INSERT INTO " + p_tablename + "\n" + p_select + ";\n";
}

// Vervang de OID kolom door een sequence.nextval
CString 
SQLInfoPostgreSQL::GetReplaceColumnOIDbySequence(CString p_columns,CString p_tablename,CString p_postfix /*="_seq"*/) const
{
  int pos;
  int len = p_columns.GetLength();
  pos = p_columns.Find("OID,");
  if(pos == 0)
  {
    CString newkolom = ( p_tablename + p_postfix + ".nextval");
    newkolom += p_columns.Right(len - 3);
    return newkolom;
  }
  if(pos > 0)
  {
    pos = p_columns.Find(",OID,");
    CString newkolom = p_columns.Left(pos) + "," + p_tablename + p_postfix + ".nextval";
    newkolom += p_columns.Right(len - pos - 5);
    return newkolom;
  }
  return p_columns;
}

// Geef opslagruimte clausule voor tabellen
// Indien er geen initialisatie is ingesteld, dan wordt teruggevallen op
// de defaultwaarde "PRONTODATA". Dit is niet discriminatoir t.o.v. de 
// te bouwen applicaties.
CString
SQLInfoPostgreSQL::GetTablesTablespace(CString p_tablespace /*=""*/) const
{
  if(!p_tablespace.IsEmpty())
  {
    return " WITHOUT OIDS TABLESPACE \"" + p_tablespace + "\"";
  }
  return " WITHOUT OIDS TABLESPACE \"DATA\"";
}

// Geef opslagruimte clausule voor indexen.
// Indien er geen initialisatie is ingesteld, dan wordt via het keyword DEFAULT
// teruggevallen op de default tablespace van de ingelogde systeemgebruiker
CString
SQLInfoPostgreSQL::GetIndexTablespace(CString p_tablespace /*=""*/) const
{
  return " TABLESPACE \"INDEX\"";
}

// Get the storage name for indici
CString 
SQLInfoPostgreSQL::GetStorageSpaceNameForIndexes() const
{
  return "INDEX"; // Settings::OpslagRuimte::OracleIndexTablespace;
}

// Geef de opslagruimte voor tijdelijke tabellen
CString
SQLInfoPostgreSQL::GetStorageSpaceNameForTempTables(CString /*p_tablename*/) const
{
  // PostgreSQL gebruikt impliciete TEMP TABLESPACE door gebruiker instellingen
  return "";
}

// Verwijder catalog afhankelijkheden
CString 
SQLInfoPostgreSQL::GetSQLRemoveProcedureDependencies(CString p_procname) const
{
  return "";
}

// Verwijder afhankelijkheden van berekende velden (bug in Firebird)
CString 
SQLInfoPostgreSQL::GetSQLRemoveFieldDependencies(CString p_tablename) const
{
  // Niet nodig/bestaat niet in Informix
  return "";
}

// Geeft de tabeldefinitie-vorm van een primary key en constraint
CString 
SQLInfoPostgreSQL::GetPrimaryKeyDefinition(CString p_tableName,bool /*p_temporary*/) const
{
  // Primary key-constraint niet direct genereren: deze wordt apart gegenereerd,
  // zodat de bijbehorende index in de juiste tablespace terechtkomt.
  return GetPrimaryKeyType() + " NOT NULL\n";
}

// Geef de constraint-vorm van een primary key definitie (achteraf toevoegen aan tabel)
CString
SQLInfoPostgreSQL::GetPrimaryKeyConstraint(CString p_tablename,bool /*p_temporary*/) const
{
  return "ADD CONSTRAINT pk_" + p_tablename + " PRIMARY KEY(oid)\n";
}

// Performance paramters in de database zetten
CString
SQLInfoPostgreSQL::GetSQLPerformanceSettings() const
{
  // Om geoptimaliseerde queries uit te kunnen voeren op views met outer-joins
  // en filters die op die views gelegd worden is de volgende parameter
  // van levensbelang voor de performance
  return "";
}

// SQL To set the caching mode of SQL results
CString 
SQLInfoPostgreSQL::GetSQLCacheModeSetting(const CString& /*p_mode*/) const
{
  return "";
}

// Getallen opslaan in stored-procs hebben dit nodig
CString
SQLInfoPostgreSQL::GetSQLNlsNumericCharacters() const
{
  return "";
}

// Geeft de manier om een kolomnaam te wijzigen, kan meerdere SQL's bevatten
CString
SQLInfoPostgreSQL::GetSQLModifyColumnName(CString p_tablename,CString p_oldName,CString p_newName,CString p_datatype)
{
  CString sqlCode = "ALTER TABLE " + p_tablename + "\n"
                    "  ADD " + p_newName + " " + p_datatype + ";\n";
  sqlCode += "UPDATE " + p_tablename + "\n"
             "   SET " + p_newName + " = " + p_oldName + ";\n";
  sqlCode += "ALTER TABLE " + p_tablename + "\n"
             " DROP COLUMN " + p_oldName + ";";
  return sqlCode;
}

// Geeft de maximale lengte van een sql statement
unsigned long 
SQLInfoPostgreSQL::GetMaxStatementLength() const
{
	return 0;		//	Geen limiet
}

// Prefix voor een add constraint DDL commando voor SQLAtlerTableGenerator
CString 
SQLInfoPostgreSQL::GetAddConstraintPrefix(CString p_constraintName) const
{
  return "ADD CONSTRAINT " + p_constraintName + " ";
}

// Suffix voor een add constraint DDL commando voor SQLAtlerTableGenerator
CString 
SQLInfoPostgreSQL::GetAddConstraintSuffix(CString p_constraintName) const
{
  return "";
}

// Prefix voor een drop constraint DDL commando voor SQLAlterTableGenerator
CString 
SQLInfoPostgreSQL::GetDropConstraintPrefix() const
{
  return "DROP CONSTRAINT ";
}

// Suffix voor een drop constraint DDL commando voor SQLAlterTableGenerator
CString 
SQLInfoPostgreSQL::GetDropConstraintSuffix() const
{
  return "";
}

// Clausule separator tussen twee ADD of DROP clausules in een ALTER TABLE
CString 
SQLInfoPostgreSQL::GetAlterTableClauseSeparator() const
{
  return ",";
}

// Groupering van meerdere kolommen in een ADD/MODIFY/DROP clausule
bool   
SQLInfoPostgreSQL::GetClauseGroupingPossible() const
{
  // Kan meerdere ADD (kolomdef ,.... ) doen
  return false;
}

// Geeft de prefix voor het wijzigen van het datatype in MODIFY/ALTER
CString 
SQLInfoPostgreSQL::GetModifyDatatypePrefix() const
{
  // Bij het veranderen van het datatype gewoon het nieuwe type opgeven
  // DUS: MODIFY <kolomnaam> TYPE <datatypenaam>
  return "TYPE ";
}

// Code om een tijdelijke tabel aan te maken (qualifier)
CString 
SQLInfoPostgreSQL::GetCodeTemporaryTable() const
{
  return "GLOBAL TEMPORARY";
}

// Code om de locking in de tabel op rij niveau te zetten
CString 
SQLInfoPostgreSQL::GetCodeLockModeRow() const
{
  return "";
}

// Code om de tabel zonder logging aan te maken
CString 
SQLInfoPostgreSQL::GetCodeTempTableWithNoLog() const
{
  return "\nWITHOUT OIDS\nON COMMIT PRESERVE ROWS";
}

// Code om alle rechten op de tabel open te zetten (NON-ANSI database)
CString 
SQLInfoPostgreSQL::GetSQLGrantAllOnTable(CString p_tableName)
{
  return "GRANT ALL ON " + p_tableName + " TO " + GetGrantedUsers();
}


// Code voor de select into temp
CString
SQLInfoPostgreSQL::GetSelectIntoTempClausePrefix(CString p_tableName) const
{
  return "CREATE GLOBAL TEMPORARY TABLE " + p_tableName + "\nWITHOUT OIDS AS\n";
}

// Code na de select into temp
CString
SQLInfoPostgreSQL::GetSelectIntoTempClauseSuffix(CString p_tableName) const
{
  return "";
}

bool
SQLInfoPostgreSQL::GetCodeIfStatementBeginEnd() const
{
  // IF THEN ELSE END IF; heeft niet perse BEGIN/END nodig
  return false;
}

// Geef het einde van een IF statement
CString 
SQLInfoPostgreSQL::GetCodeEndIfStatement() const
{
  return "END IF;\n";
}

CString 
SQLInfoPostgreSQL::GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const
{
  return p_destiny + " := " + p_source + ";";
}

// Geef SQL keyword om een kolom te wijzigen
CString 
SQLInfoPostgreSQL::GetCodeAlterColumn() const
{
  return "ALTER ";
}

// Geef de code om een WHILE-loop te starten
CString
SQLInfoPostgreSQL::GetStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Geef de code om een WHILE-loop te eindigen
CString
SQLInfoPostgreSQL::GetEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Geef het feit of een SELECT tussen parenthesis moet staan voor een assignment
bool 
SQLInfoPostgreSQL::GetAssignmentSelectParenthesis() const
{
  // waarde = SELECT max kenmerk FROM tabel;
  return false;
}

// Gets the UPPER function
CString 
SQLInfoPostgreSQL::GetUpperFunction(CString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString 
SQLInfoPostgreSQL::GetInterval1MinuteAgo() const
{
  return "ERROR";
}

// Gets the construction / select for generating a new serial identity
CString 
SQLInfoPostgreSQL::GetSQLGenerateSerial(CString p_table) const
{
  return "SELECT " + p_table + "_seq.nextval FROM DUAL";
}

// Gets the construction / select for the resulting effective generated serial
CString 
SQLInfoPostgreSQL::GetSQLEffectiveSerial(CString p_identity) const
{
  // Already the correct value
  return p_identity;
}

// Gets the Not-NULL-Value statement of the database
CString 
SQLInfoPostgreSQL::GetNVLStatement(CString& p_test,CString& p_isnull) const
{
  return "{fn IFNULL(" + p_test + "," + p_isnull + ")}";
}

// Gets the subtransaction commands
CString 
SQLInfoPostgreSQL::GetStartSubTransaction(CString p_savepointName) const
{
  return CString("SAVEPOINT ") + p_savepointName;
}

CString 
SQLInfoPostgreSQL::GetCommitSubTransaction(CString p_savepointName) const
{
  // Geen commit voor subtransaction
  return CString("");
}

CString 
SQLInfoPostgreSQL::GetRollbackSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TO SAVEPOINT ") + p_savepointName;
}

// SQL CATALOG QUERIES
// ===================================================================

// Geef SQL om te kijken of storedprocedure in de database bestaat
CString 
SQLInfoPostgreSQL::GetSQLStoredProcedureExists(CString& p_name) const
{
  return "SELECT count(*)\n"
         "  FROM pg_proc\n"
         " WHERE proname = '" + p_name + "'\n;";
}

// Deel van query om 1 record te selecteren
CString
SQLInfoPostgreSQL::GetDualTableName() const
{
  return "";
}

// FROM-Part for a query to select only 1 (one) record
CString 
SQLInfoPostgreSQL::GetDualClause() const
{
  return "";
}

// Geef DEFERRABLE (of niet dus)
CString
SQLInfoPostgreSQL::GetConstraintDeferrable() const
{
  return " DEFERRABLE";
}

// Constraints tijdelijk opschorten tot aan de volgende COMMIT;
CString 
SQLInfoPostgreSQL::GetSQLDeferConstraints() const
{
  return "set transaction deferrable";
}

// Constraints weer op onmiddellijk terugzetten
CString 
SQLInfoPostgreSQL::GetSQLConstraintsImmediate() const
{
  return "set transaction not deferrable";
}

// Geef de query die controleert of de tabel al bestaat in de database
CString 
SQLInfoPostgreSQL::GetSQLTableExists(CString p_tablename) const
{
  CString lowerName(p_tablename);
  lowerName.MakeLower();
  CString query = "SELECT count(*)\n"
                  "  FROM pg_class\n"
                  " WHERE relname = '" + lowerName + "'";
  return query;
}

// Geef de query die de kolommen van een tabel uit de catalogus haalt
CString 
SQLInfoPostgreSQL::GetSQLGetColumns(CString& /*p_user*/,CString& p_tableName) const
{
  CString tableName(p_tableName);
  tableName.MakeLower();
  // naam, nummer, type, lengte, nullable
  CString query =  "SELECT attname\n"
                   "      ,attnum\n"
                   "      ,typname\n"
                   "      ,typlen\n"
                   "      ,attnotnull\n"
                   "      ,(atttypmod - 4) / 65536\n"
                   "      ,(atttypmod - 4) % 65536\n"
                   "  FROM pg_class, pg_attribute, pg_type\n"
                   " WHERE pg_class.relname = '" + tableName  + "'\n"
                   "   AND pg_class.relkind = 'r'\n"
                   "   AND pg_class.oid = pg_attribute.attrelid\n"
                   "   AND pg_attribute.atttypid = pg_type.oid\n"
                   "   AND attnum > 0\n"
                   "   AND typlen < 0\n"
                   "   AND typname = 'numeric'\n"
                   "UNION\n"
                   "SELECT attname\n"
                   "      ,attnum\n"
                   "      ,typname\n"
                   "      ,(atttypmod - 4) AS typlen\n"
                   "      ,attnotnull\n"
                   "      ,0\n"
                   "      ,0\n"
                   "  FROM pg_class, pg_attribute, pg_type\n"
                   " WHERE pg_class.relname = '" + tableName  + "'\n"
                   "   AND pg_class.relkind = 'r'\n"
//                       "   AND pg_class.relowner = pg_shadow.usesysid\n"
//                       "   AND pg_shadow.usename = '" + eigenaar + "'\n"
                   "   AND pg_class.oid = pg_attribute.attrelid\n"
                   "   AND pg_attribute.atttypid = pg_type.oid\n"
                   "   AND attnum > 0\n"
                   "   AND typlen < 0\n"
                   "   AND typname = 'varchar'\n"
                   "UNION\n"
                   "SELECT attname\n"
                   "      ,attnum\n"
                   "      ,typname\n"
                   "      ,typlen\n"
                   "      ,attnotnull\n"
                   "      ,0\n"
                   "      ,0\n"
                   "  FROM pg_class, pg_attribute, pg_type\n"
                   " WHERE pg_class.relname = '" + tableName  + "'\n"
                   "   AND pg_class.relkind = 'r'\n"
                   "   AND pg_class.oid = pg_attribute.attrelid\n"
                   "   AND pg_attribute.atttypid = pg_type.oid\n"
                   "   AND attnum > 0\n"
                   "   AND typlen > 0\n"
                   "   AND typname not in ('numeric', 'varchar')\n"
                   " ORDER BY attnum";
  return query;
}

// Geef de query om de Check en Unique constraints op te halen
CString 
SQLInfoPostgreSQL::GetSQLGetConstraintsForTable(CString& p_tableName) const
{
  // [EH] Uitgebreid met een beperking op SYS_ omdat dit ervoor
  // zorgt dat de NOT NULL constraints niet twee keer gedropped worden
  // [EH] Type 'R' weggehaald, omdat anders alle refrence constraints
  // ook mee 'gedropped' worden bij het droppen van check constraints
  CString lowerName(p_tableName);
  lowerName.MakeLower();
  CString contabel = "SELECT con.conname\n"
                     "      ,cla.relname\n"
                     "  FROM pg_constraint con, pg_class cla\n"
                     " WHERE con.contype in ('c','u')\n"
                     "   AND con.conrelid = cla.oid\n"
                     "   AND cla.relname = '" + lowerName + "'\n";
  return contabel;
}

// Lees de bestaande indexen van een tabel
CString 
SQLInfoPostgreSQL::GetSQLTableIndexes(CString& p_user,CString& p_tableName) const
{
  CString lowerTable(p_tableName);
  CString lowerUser (p_user);
  lowerTable.MakeLower();
  lowerUser .MakeLower();

  CString query =  "SELECT cli.relname\n"
                   "      ,idx.indisunique\n"
                   "      ,att.attnum\n"
                   "      ,att.attname\n"
                   "      ,'ASCENDING'\n"
                   "  FROM pg_user sha\n"
                   "      ,pg_class clr\n"
                   "      ,pg_index idx\n"
                   "      ,pg_class cli\n"
                   "      ,pg_attribute att\n"
                   " WHERE sha.usename = '" + lowerUser + "'\n"
                   "   AND sha.usesysid = clr.relowner\n"
                   "   AND clr.relname = '" + lowerTable + "'\n"
                   "   AND clr.relkind = 'r'\n"
                   "   AND clr.oid = idx.indrelid\n"
                   "   AND idx.indexrelid = cli.oid\n"
                   "   AND cli.relkind = 'i'\n"
                   "   AND cli.oid = att.attrelid\n"
                   " ORDER BY cli.relname, att.attnum";
  return query;
}

// Geef de query om de referential constaints uit de catalogus te lezen.
CString 
SQLInfoPostgreSQL::GetSQLTableReferences(CString& p_tablename) const
{
  CString lowerName(p_tablename);
  lowerName.MakeLower();
  CString query = "SELECT con.conname\n"
                  "      ,cla.relname\n"
                  "  FROM pg_constraint con, pg_class cla\n"
                  " WHERE con.contype = 'f'\n"
                  "   AND con.conrelid = cla.oid\n"
                  "   AND cla.relname  = '" + lowerName + "'";
  return query;
}

// Maak een synonym
CString
SQLInfoPostgreSQL::GetSQLMakeSynonym(CString& /*p_objectName*/) const
{
  // Let op: public synonym zonder qualifier, for qualifier.tabelnaam
	return "";
}

// Verwijder een synonym
CString 
SQLInfoPostgreSQL::GetSQLDropSynonym(CString& /*p_objectName*/) const
{
  return "";
}

 // Maak een sequence
void
SQLInfoPostgreSQL::DoCreateSequence(CString& p_sequenceName,int p_startpos) 
{
  CString create = "CREATE SEQUENCE " + p_sequenceName;
  if(p_startpos)
  {
    create.AppendFormat(" START WITH %d",p_startpos);
  }
  SQLQuery sql(m_database);
  sql.DoSQLStatement(create);
}

// Verwijder een sequence
void 
SQLInfoPostgreSQL::DoRemoveSequence(CString& p_sequenceName) const
{
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement("DROP SEQUENCE " + p_sequenceName);
}

// Geef de query om een stored procedure te verwijderen
void 
SQLInfoPostgreSQL::DoRemoveProcedure(CString& /*p_procedureName*/) const
{
//   if (procnaam.Bevat("_ins_proc") || procnaam.Bevat("_upd_proc") || procnaam.Bevat("_fupd_proc"))
//   {
//     String subsys = procnaam.AllesVoor("_");
//     String alias  = procnaam.AllesNa("_").AllesVoor("_");
//     String tabelnaam = subsys + "_" + alias;
//     return "DROP FUNCTION " + procnaam + "(" + tabelnaam + ");\n";
//   }
//   else
//   {
//     if (procnaam.Bevat("_del_proc"))
//     {
//       return "DROP FUNCTION " + procnaam + "(int4, \"timestamp\", \"timestamp\", \"varchar\");\n";
//     }
//     else
//     {
//       return "DROP FUNCTION " + procnaam + "(" + procnaam + "_proc_in_par);\n" +
//         "DROP TYPE " + procnaam + "_proc_in_par;\n" +
//         "DROP TYPE " + procnaam + "_proc_out_par;\n";
//     }
//   }
}

void 
SQLInfoPostgreSQL::DoCreateNextSequence(const CString& p_tableName,CString p_postfix /*="_seq"*/)
{
  // In PostgreSQL worden de oid's gegenereerd uit een SEQUENCE. Deze moet eerst worden
  // aangemaakt. Niet meer droppen, want dan zijn er al oid's uitgegeven

  // @DJS Wel droppen als sequence nummer fout is. Is momenteel geval door eerdere
  // problemen met genereren.
  // EH: Wel droppen in verband met verkeerde sequences
  // door acties van consultants en converteurs. Anderen rekenen op het 
  // vermogen om de sequence te herstellen
  SQLQuery sql(m_database);
  CString sequenceName = p_tableName + p_postfix;
  DoRemoveSequence(sequenceName);
  long maxOid  = 0;
  try
  {
    // We zoeken het maximum oid in de tabel plus 1 als beginwaarde.
    CString query = "SELECT MAX(oid) FROM " + p_tableName;

    sql.DoSQLStatement(query);
    if(sql.GetRecord())
    {
      maxOid = sql.GetColumn(1)->GetAsSLong();
    }
    ++maxOid;
  }
  catch (...) {}
  // Hoogste oid meegeven aan de create sequence als start pos.
  DoCreateSequence(sequenceName,maxOid);
  // Nu nog rechten en synoniemen:
  sql.TryDoSQLStatement(GetSQLSequenceRights(p_tableName,p_postfix));  
}

// Geef de query om rechten op een sequence toe te kennen
CString 
SQLInfoPostgreSQL::GetSQLSequenceRights(const CString& p_tableName,CString p_postfix /*="_seq"*/) const
{
  return "GRANT SELECT, UPDATE ON " + p_tableName + p_postfix + " TO " + GetGrantedUsers();
}

CString
SQLInfoPostgreSQL::GetSQLSessionAndTerminal() const
{
  CString query = "SELECT procpid\n"
                  "      ,datname\n"
                  "  FROM pg_stat_activity\n"
                  " WHERE procpid = pg_backend_pid()";    
  return query;
}

// Get SQL to check if sessionnumber exists
CString 
SQLInfoPostgreSQL::GetSQLSessionExists(CString sessieId) const
{
  return "";
}

// Geef query voor uniek sessie id.
CString 
SQLInfoPostgreSQL::GetSQLUniqueSessionId(const CString& /*p_databaseName*/,const CString& /*p_sessionTable*/) const
{
  // In PostgreSQL is de database de engine. Databasenaam is dus niet relevant
  CString query = "SELECT DISTINCT procpid\n"
                  "  FROM pg_stat_activity\n"
                  " WHERE procpid <> 0";
  return query;
}

// Geef query voor opzoeken sessie.
CString 
SQLInfoPostgreSQL::GetSQLSearchSession(const CString& /*p_databaseName*/,const CString& p_sessionTable) const
{
  // In PostgreSQL is de database de engine. Databasenaam is dus niet relevant
  CString query = "SELECT  procpid\n"
                  "       ,usename\n"
                  "       ,datname\n"
                  "  FROM pg_stat_activity\n"
                  " WHERE procpid <> 0\n"
                  "   AND NOT procpid IN\n"
                  "     ( SELECT sessie_nr\n"
                  "         FROM "+ p_sessionTable + ")";
  return query;
}

// Kijk of tabel/kolom combinatie bestaat
bool   
SQLInfoPostgreSQL::DoesColumnExistsInTable(CString& p_owner,CString& p_tableName,CString& p_column) const
{
  int number = 0;
  CString lowerOwner(p_owner);
  CString lowerTable(p_tableName);
  CString lowerColumn(p_column);
  lowerOwner.MakeLower();
  lowerTable.MakeLower();
  lowerColumn.MakeLower();

  CString query = "SELECT count(*)\n"
                 "  FROM pg_class a, pg_attribute b, pg_user c\n"
                 " WHERE a.relname  = '" + lowerTable + "'\n"
                 "   AND a.OID = b.attrelid\n"
                 "   AND b.attname = '" + lowerColumn + "'\n"
                 "   AND a.relowner = c.usesysid\n"
                 "   AND c.usename = '" + lowerOwner  + "'";
  SQLQuery sql(m_database);
  sql.DoSQLStatement(query);
  if (sql.GetRecord())
  {
    number = sql.GetColumn(1)->GetAsSLong();
  }
  return (number == 1);
}

// Query om te bepalen of de tabel al een primary key heeft
CString
SQLInfoPostgreSQL::GetSQLPrimaryKeyConstraintInformation(CString& p_tableName) const
{
  CString table(p_tableName);
  table.MakeLower();
  CString query = "SELECT count(*)\n"
                  "  FROM pg_class a, pg_constraint b\n"
                  " WHERE a.relname = '" + table + "'\n"
                  "   AND a.oid = b.conrelid\n"
                  "   AND b.contype = 'p'";
  return query;
}

// Does the named constraint exist in the database
bool    
SQLInfoPostgreSQL::DoesConstraintExist(CString /*p_constraintName*/) const
{
  return true;
}

// Geeft een lock-table query
CString 
SQLInfoPostgreSQL::GetSQLLockTable(CString& p_tableName,bool p_exclusive) const
{
  CString query = "LOCK TABLE " + p_tableName + " IN "  ;
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Geef query om de statistics te optimaliseren / analyseren
CString 
SQLInfoPostgreSQL::GetSQLOptimizeTable(CString& p_owner,CString& p_tableName,int& p_number)
{
  // Optimaliseer de tabel
  CString optim = "VACUUM ANALYZE " + p_owner + "." + p_tableName + ";\n";
  p_number += 1;
  return optim;
}

// Getting the fact that there is only **one** (1) user session in the database
bool
SQLInfoPostgreSQL::GetOnlyOneUserSession()
{
  // Yet to implement
  return true;
}

// SQL DDL ACTIONS
// ===================================================================

// Verwerk DDL commandos in de catalog
void 
SQLInfoPostgreSQL::DoCommitDDLcommands() const
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement("commit");
}

// Expliciet DML commandos verwerken
// ODBC driver autoocommit mode aanzetten gaat vaak fout
void
SQLInfoPostgreSQL::DoCommitDMLcommands() const
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement("commit");
}

// Creeer een view op een geoptimaliseerde manier vanuit een bestaande tabel
void 
SQLInfoPostgreSQL::DoCreateOrReplaceView(CString p_code,CString p_viewName)
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement(p_code);
  sql.DoSQLStatement("GRANT SELECT ON " + p_viewName + " TO " + GetGrantedUsers());
}

// Verwijder een view uit de database
void 
SQLInfoPostgreSQL::DoDropView(CString p_viewName)
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement("DROP VIEW " + p_viewName);
  DoCommitDDLcommands();
}

// Does the named view exists in the database
bool
SQLInfoPostgreSQL::DoesViewExists(CString& p_viewName)
{
  CString viewname(p_viewName);
  viewname.MakeUpper();
  SQLQuery query(m_database);
  query.DoSQLStatement("SELECT COUNT(*)\n"
                       "  FROM pg_views\n"  // TODO: Check the correct name
                       " WHERE view_name = '" + viewname + "'");
  if(query.GetRecord())
  {
    return query.GetColumn(1)->GetAsSLong() == 1;
  }
  return false;
}



// Creeer tijdelijke klassen runtime
bool 
SQLInfoPostgreSQL::GetMustMakeTemptablesAtRuntime() const
{
  return false;
}

// Creeer een tijdelijke tabel op een geoptimaliseerde manier vanuit een meegegeven info
void
SQLInfoPostgreSQL::DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const
{
  SQLQuery sql(m_database);

  sql.TryDoSQLStatement("TRUNCATE TABLE " + p_tableName);
  sql.TryDoSQLStatement("DROP TABLE " + p_tableName);
  CString create = "CREATE GLOBAL TEMPORARY TABLE " + p_tableName + "\n"
                   "WITHOUT OIDS\n"
                   "ON COMMIT PRESERVE ROWS\n" + p_content;
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
    // Kan geen tijdelijke tabel maken: %s 
    throw CString("Cannot create temporary table: " + p_tableName);
  }
}

// Verwijder tijdelijke tabel weer
void
SQLInfoPostgreSQL::DoRemoveTemporaryTable(CString& p_tableName) const
{
  SQLQuery sql(m_database);
  // Alles mag genegeerd worden. Kan nog in gebruik zijn bij een andere
  // gebruiker en/of sessie. De tabelinhoud wordt hierbij door deze sessie
  // verwijderd. (getruncate)
  sql.TryDoSQLStatement("DELETE FROM "    + p_tableName);
  sql.TryDoSQLStatement("TRUNCATE TABLE " + p_tableName);
  sql.TryDoSQLStatement("DROP TABLE "     + p_tableName);
}

// Indien de tabel een tijdelijke tabel is, verwijder hem
void
SQLInfoPostgreSQL::DoRemoveTemporaryTableWithCheck(CString& p_tableName) const
{
  CString tableName(p_tableName);
  int number = 0;
  tableName.MakeLower();

  CString query = "SELECT count(*)\n"
                  "  FROM pg_class cl\n"
                  "      ,pg_namespace ns\n"
                  " WHERE cl.relnamespace = ns.oid\n"
                  "   AND cl.relname = '" + tableName + "'\n"
                  "   AND ns.nspname like like_escape('pg!_temp!_%','!')";
  SQLQuery sql(m_database);
  sql.DoSQLStatement(query);
  if(sql.GetRecord())
  {
    number = sql.GetColumn(1)->GetAsSLong();
  }
  // Als de tabel WEL voorkomt in de catalog, dan mag je hem proberen te droppen
  // Als hij NIET voorkomt, dan bestaat hij niet, en hoeft hij niet gedropped te worden
  if(number == 1)
  {
    DoRemoveTemporaryTable(p_tableName);
  }
}

// Maak een procedure aan in de database
void 
SQLInfoPostgreSQL::DoMakeProcedure(CString& p_procName,CString p_table,bool p_noParameters,CString& p_codeBlock)
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement(p_codeBlock);
  CString grant_statement = "GRANT EXECUTE ON FUNCTION " + p_procName + "(";
  if (p_procName.Find("_ins_proc")  > 0 || 
      p_procName.Find("_upd_proc")  > 0 || 
      p_procName.Find("_fupd_proc") > 0 )
  {
    grant_statement += p_table + ")";
  }
  else
  {
    if (p_procName.Find("_del_proc") > 0)
    {
      grant_statement += "int4, \"timestamp\", \"timestamp\", \"varchar\")";
    }
    else if (p_noParameters)
    {
      grant_statement += ")";
    }
    else
    {
      grant_statement += p_procName + "_proc_in_par)";
    }
  }
  grant_statement += " TO PUBLIC;";
  sql.DoSQLStatement(grant_statement);
}

// Wijze van de database om een tabel van naam te veranderen
void 
SQLInfoPostgreSQL::DoRenameTable(CString& p_oldName,CString& p_newName) const
{
  SQLQuery sql(m_database);
  // Let op: Zonder 'TABLE' in het statement
  CString rename = "rename " + p_oldName + " to " + p_newName;
  sql.DoSQLStatement(rename);
  DoCommitDDLcommands();
}

// PERSISTENT-STORED MODULES (SPL / PL/SQL)
// ====================================================================


// Geef de user-errors uit de database:
CString 
SQLInfoPostgreSQL::GetUserErrorText(CString& p_procName) const
{
  (void)p_procName;   // niet beschikbaar in postgreSQL.
  return "";
}

// Geef toekenning aan een variabele in SPL
CString 
SQLInfoPostgreSQL::GetSPLAssignment(CString p_variable) const
{
  return p_variable + " := ";
}

// Geef de start van een SPL While loop
CString 
SQLInfoPostgreSQL::GetSPLStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Geef het einde van een SPL While loop
CString 
SQLInfoPostgreSQL::GetSPLEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Geef stored procedureaanroep regel
CString 
SQLInfoPostgreSQL::GetSQLSPLCall(CString p_procName) const
{
  CString subsys = p_procName.Left(p_procName.Find("?"));
//   if (subsys.Bevat("ins_proc(") || subsys.Bevat("upd_proc("))
//   {
//     subsys = p_procName.AllesVoor("_");
//     CString alias  = p_procName.AllesNa("_").AllesVoor("_");
//     p_procName.Replace("(", "(CAST(ROW(");
//     p_procName.Replace(")", ") AS ");
//     p_procName += subsys + "_" + alias + "))";
//   }
  return "SELECT " + p_procName + ";";
}

// Lengte parameters bij binden:
int 
SQLInfoPostgreSQL::GetParameterLength(int p_SQLType) const
{
  int retval;

  switch (p_SQLType)
  {
    case SQL_CHAR:          retval = 2000; break;
    case SQL_VARCHAR:       retval = 4000; break;
    case SQL_LONGVARCHAR:   retval = 32000;break;
    case SQL_DECIMAL:       retval = 32000;break;
	  case SQL_SMALLINT:      retval = 0;    break;
	  case SQL_INTEGER:       retval = sizeof(long);break;
	  case SQL_REAL:          retval = 0;    break;
	  case SQL_DOUBLE:        retval = 0;    break;
	  case SQL_FLOAT:         retval = 0;    break;
	  case SQL_BINARY:        retval = 0;    break;
	  case SQL_VARBINARY:     retval = 0;    break;
	  case SQL_LONGVARBINARY: retval = 0;    break;
	  case SQL_DATE:          retval = 0;    break;
    case SQL_TIME:          retval = 0;    break;
    case SQL_TIMESTAMP:     retval = 19;   break;
	  case SQL_NUMERIC:       retval = 0;    break;
	  case SQL_BIGINT:        retval = 0;    break;
	  case SQL_TINYINT:       retval = 0;    break;
	  case SQL_BIT:           retval = 0;    break;
    case SQL_INTERVAL_YEAR:
    case SQL_INTERVAL_YEAR_TO_MONTH:
    case SQL_INTERVAL_MONTH:
    case SQL_INTERVAL_DAY:
    case SQL_INTERVAL_DAY_TO_HOUR:
    case SQL_INTERVAL_DAY_TO_MINUTE:
    case SQL_INTERVAL_DAY_TO_SECOND:
    case SQL_INTERVAL_HOUR:
    case SQL_INTERVAL_HOUR_TO_MINUTE:
    case SQL_INTERVAL_HOUR_TO_SECOND:
    case SQL_INTERVAL_MINUTE:
    case SQL_INTERVAL_MINUTE_TO_SECOND:
    case SQL_INTERVAL_SECOND:           retval = 25;    break;
    default:                            retval = 0;     break;
  }
  return retval;
}

// Bouw een parameterlijst op voor aanroep van de stored-procedure
CString 
SQLInfoPostgreSQL::GetBuildedParameterList(size_t p_numOfParameters) const
{
  // dit staat voor de parameters, de vraagtekens die worden meegegeven aan de
  // stored procedure
  // INDIEN GEEN paraemters, dan ook GEEN ellipsis
  CString strParamLijst;
  if(p_numOfParameters >= 0)
  {
    for (size_t i = 0; i < p_numOfParameters; i++)
    {
      if(i != 0) 
      {
        strParamLijst += ",";
      }
      else
      {
        strParamLijst += "(";
      }
      strParamLijst += "?";
    }
    if(p_numOfParameters > 0)
    {
      strParamLijst += ")";
    }
  }
  return strParamLijst;
}

CString
SQLInfoPostgreSQL::GetParameterType(CString& p_type) const
{
  // char, varchar -> varchar
  // decimal -> number

  CString retval;
  if (_strnicmp(p_type,"char",4) == 0 ||
      _strnicmp(p_type,"varchar",7) == 0 )
  {
    retval = "varchar";
  }
  else if (_strnicmp(p_type,"decimal",7) == 0 )
  {
    retval = "decimal";
  }
  else 
  {
    retval = p_type;
  }
  return retval;
}

CString 
SQLInfoPostgreSQL::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetQuoteCharacter();
  return  kwoot + s + kwoot;
}

CString 
SQLInfoPostgreSQL::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  CString dateString;
  dateString.Format("TO_DATE('%04d-%02d-%02d','YYYY-MM-DD')",p_year,p_month,p_day);
  return dateString;
}  
  
CString 
SQLInfoPostgreSQL::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  CString timeString;
  timeString.Format("'%02d:%02d:%02d'",p_hour,p_minute,p_second);
  return timeString;
}  
  
CString 
SQLInfoPostgreSQL::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("to_timestamp('%04d-%02d-%02d %02d:%02d:%02d','YYYY-MM-DD HH24:MI:SS')::timestamp"
               ,p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}  
  
// Get date-time bound parameter string in database format
CString 
SQLInfoPostgreSQL::GetSQLDateTimeBoundString() const
{
  return "to_timestamp(?,'YYYY-MM-DD HH24:MI:SS')::timestamp";
}

// Stripped data for the parameter binding
CString
SQLInfoPostgreSQL::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("%04d-%02d-%02d %02d:%02d:%02d"
                ,p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

CString 
SQLInfoPostgreSQL::GetSPLSourcecodeFromDatabase(const CString& p_owner,const CString& p_procName) const
{
  CString sQuery;  
  sQuery = "SELECT TEXT from ALL_SOURCE "
           "WHERE type = 'FUNCTION' "
           "AND name  = LOWER('" + p_procName +  "') "
           "AND owner = LOWER('" + p_owner + "')";
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement(sQuery);
  CString sProcBody = "CREATE OR REPLACE ";
  while (sql.GetRecord())
  {
	  sProcBody += sql.GetColumn(1)->GetAsChar();
	}
  return sProcBody;
}

// Get the SPL datatype for integer
CString 
SQLInfoPostgreSQL::GetSPLIntegerType() const
{
  return "integer";
}

// Get the SPL datatype for a decimal
CString 
SQLInfoPostgreSQL::GetSPLDecimalType() const
{
  return "decimal";
}

// Get the SPL declaration for a cursor
CString 
SQLInfoPostgreSQL::GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const
{
  return "cursor " + p_variableName + " is " + p_query + ";";
}

// Get the SPL cursor found row parameter
CString 
SQLInfoPostgreSQL::GetSPLCursorFound(CString& /*p_cursorName*/) const
{
  // TODO: To be implemented
  return "";
}

// Get the SPL cursor row-count variable
CString 
SQLInfoPostgreSQL::GetSPLCursorRowCount(CString& /*p_variable*/) const
{
  // TODO: To be implemented
  return "";
}

// Get the SPL datatype for a declaration of a row-variable
CString 
SQLInfoPostgreSQL::GetSPLCursorRowDeclaration(CString& /*p_cursorName*/,CString& /*p_variableName*/) const
{
  // TODO: To be implemented
  return "";
}

CString 
SQLInfoPostgreSQL::GetSPLFetchCursorIntoVariables(CString               p_cursorName
                                                 ,CString             /*p_variableName*/
                                                 ,std::vector<CString>& p_columnNames
                                                 ,std::vector<CString>& p_variableNames) const
{
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
SQLInfoPostgreSQL::GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const
{ 
  // TODO: Check
  return "FETCH " + p_cursorName + " INTO " + p_variableName+ ";";
}

// Get the SPL no-data exception clause
CString 
SQLInfoPostgreSQL::GetSPLNoDataFoundExceptionClause() const
{
  // TODO: Check
  return "WHEN NO_DATA_FOUND THEN";
}

// Get the SPL form of raising an exception
CString 
SQLInfoPostgreSQL::GetSPLRaiseException(CString p_exceptionName) const
{
  // TODO: Check
  return "RAISE " + p_exceptionName + ";";
}

// Get the fact that the SPL has server functions that return more than 1 value
bool    
SQLInfoPostgreSQL::GetSPLServerFunctionsWithReturnValues() const
{
  return true;
}

// SPECIALS
// ==========================================================================

// Translate database-errors to a human readable form
CString 
SQLInfoPostgreSQL::TranslateErrortext(int p_error,CString p_errorText) const
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

