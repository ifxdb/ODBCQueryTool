////////////////////////////////////////////////////////////////////////
//
// File: SQLDataSet.cpp
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
#include "SQLDataSet.h"
#include "SQLQuery.h"
#include "SQLVariantFormat.h"
#include "SQLInfoDB.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Names for saving datasets to XML in various languages
const char* dataset_names[LN_NUMLANG][NUM_DATASET_NAMES] =
{
  // LN_DUTCH
  { "Naam"          // DATASET_NAME
   ,"Structuur"     // DATASET_STRUCTURE
   ,"Veld"          // DATASET_FIELD
   ,"Record"        // DATASET_RECORD
   ,"Records"       // DATASET_RECORDS
   ,"id"            // DATASET_ID
   ,"type"          // DATASET_TYPE
   ,"typenaam"      // DATASET_TYPENAME
  },
  // LN_ENGLISH
  { "Name"          // DATASET_NAME
   ,"Structure"     // DATASET_STRUCTURE
   ,"Field"         // DATASET_FIELD
   ,"Record"        // DATASET_RECORD
   ,"Records"       // DATASET_RECORDS
   ,"ID"            // DATASET_ID
   ,"Type"          // DATASET_TYPE
   ,"TypeName"      // DATASET_TYPENAME
  },
  // LN_GERMAN
  { "Name"          // DATASET_NAME
   ,"Struktur"      // DATASET_STRUCTURE
   ,"Feld"          // DATASET_FIELD
   ,"Rekord"        // DATASET_RECORD
   ,"Rekords"       // DATASET_RECORDS
   ,"ID"            // DATASET_ID
   ,"Typ"           // DATASET_TYPE
   ,"Typename"      // DATASET_TYPENAME
  },
  // LN_FRENCH
  { "Nom"           // DATASET_NAME
   ,"Structure"     // DATASET_STRUCTURE
   ,"Domaine"       // DATASET_FIELD
   ,"Record"        // DATASET_RECORD
   ,"Records"       // DATASET_RECORDS
   ,"ID"            // DATASET_ID
   ,"Type"          // DATASET_TYPE
   ,"NomDeType"     // DATASET_TYPENAME
  },
};

// The DATASET class

SQLDataSet::SQLDataSet()
           :m_database(NULL)
           ,m_status(SQL_Empty)
           ,m_current(-1)
           ,m_open(false)
{
}

SQLDataSet::SQLDataSet(CString p_name,SQLDatabase* p_database /*=NULL*/)
           :m_name(p_name)
           ,m_database(p_database)
           ,m_status(SQL_Empty)
           ,m_current(-1)
           ,m_open(false)
{
}

SQLDataSet::~SQLDataSet()
{
  Close();
}

// Navigate in the records
int
SQLDataSet::First()
{
  return (m_current = (m_records.size() > 0) ? 0 : -1);
}

int  
SQLDataSet::Next()
{
  if(m_current < (int)m_records.size() - 1)
  {
    return ++m_current;
  }
  return -1;
}

int  
SQLDataSet::Prev()
{
  if(m_current > 0)
  {
    --m_current;
  }
  return m_current;
}

int  
SQLDataSet::Last()
{
  m_current = ((int)m_records.size() > 0) ? (int)m_records.size() - 1 : -1;
  return m_current;
}

int
SQLDataSet::Goto(int p_record)
{
  if(p_record >= 0 && p_record < (int)m_records.size())
  {
    return m_current = p_record;
  }
  return -1;
}

// Is it the first of the current set, or an empty set
bool
SQLDataSet::IsFirst()
{
  return (m_current < 1);
}

// Last of a dataset, or an empty set
bool
SQLDataSet::IsLast()
{
  return m_current == (int)(m_records.size() - 1);
}

void
SQLDataSet::Close()
{
  // Forget the data
  Forget(true);

  // Forget all caches
  m_names.clear();
  m_types.clear();
  m_parameters.clear();
  m_primaryKey.clear();

  // Forget the query
  m_name.Empty();
  m_query.Empty();
  m_selection.Empty();
  m_searchColumn.Empty();
  m_primaryTableName.Empty();

  // Reset the open status
  m_open = false;
}

// Forget the records
// BUT NOT REMOVING THE INFO, Append can be used!
bool         
SQLDataSet::Forget(bool p_force /*=false*/)
{
  if(p_force == false)
  {
    // Still changes to the database pending
    // Cannot simply forget all records
    if(m_status & (SQL_Updates | SQL_Deletions | SQL_Insertions))
    {
      return false;
    }
  }
  // Deleting all records
  for(unsigned int ind = 0; ind < m_records.size(); ++ind)
  {
    SQLRecord* record = m_records[ind];
    delete record;
  }
  // Forget the caches
  m_records.clear();
  m_objects.clear();
  // Set status to empty
  m_status  = SQL_Empty;
  m_current = -1;

  return true;
}

// Forget 1 record and primary is an INTEGER (Fast!!)
bool
SQLDataSet::ForgetObject(int p_primary,bool p_force /*=false*/)
{
  // Find the record to forget
  SQLRecord* record = FindObjectRecord(p_primary);
  if(!record)
  {
    return false;
  }
  return ForgetRecord(record,p_force);
}

// Forget 1 record and primary is a compound key (Slower)
bool
SQLDataSet::ForgetObject(VariantSet& p_primary,bool p_force /*=false*/)
{
  // Find the record to forget
  SQLRecord* record = FindObjectRecord(p_primary);
  if(!record)
  {
    return false;
  }
  return ForgetRecord(record,p_force);
}

void
SQLDataSet::SetStatus(int m_add,int m_delete /*=0*/)
{
  m_status |= m_add;
  if(m_delete)
  {
    m_status &= ~m_delete;
  }
}

void 
SQLDataSet::SetParameter(SQLParameter p_parameter)
{
  // See if the parameter is already there
  for(unsigned int ind = 0;ind < m_parameters.size(); ++ind)
  {
    if(m_parameters[ind].m_name == p_parameter.m_name)
    {
      // Found it: set a new value
      m_parameters[ind].m_value = p_parameter.m_value;
      return;
    }
  }
  // New parameter
  m_parameters.push_back(p_parameter);
}

void 
SQLDataSet::SetParameter(CString p_naam,SQLVariant p_waarde)
{
  // See if the parameter is already there
  for(unsigned int ind = 0;ind < m_parameters.size();++ind)
  {
    if(m_parameters[ind].m_name == p_naam)
    {
      // Found it: set a new value
      m_parameters[ind].m_value = p_waarde;
      return;
    }
  }
  // New parameter
  SQLParameter par;
  par.m_name   = p_naam;
  par.m_value  = p_waarde;
  m_parameters.push_back(par);
}

// Set filters for a query
// Releasing the previous set of filters
void
SQLDataSet::SetFilters(SQLFilterSet& p_filters)
{
  m_filters = p_filters;
}

void         
SQLDataSet::SetPrimaryKeyColumn(WordList& p_list)
{
  m_primaryKey.clear();
  for(auto& column : p_list)
  {
    m_primaryKey.push_back(column);
  }
}

SQLVariant*  
SQLDataSet::GetParameter(CString& p_name)
{
  for(unsigned int ind = 0;ind < m_parameters.size(); ++ind)
  {
    if(m_parameters[ind].m_name == p_name)
    {
      return &(m_parameters[ind].m_value);
    }
  }
  return NULL;
}

// Replace $name for the value of a parameter
// $ signs within 'string$chain' or "String$chain" can NOT be replaced
// This makes it possible to write queries like
// SELECT t.name
//       ,s.number
//       ,'Constant with an $ in it'
//   FROM table t
//       ,$parameter1 s
//  WHERE t.id   =    $parameter2
//    AND t.name LIKE $parameter3
// ;
CString
SQLDataSet::ParseQuery()
{
  bool inQuote = false;
  bool inAphos = false;
  CString query;
  int  pos = 0;

  while(pos < m_query.GetLength())
  {
    int c = m_query.GetAt(pos);
    if(c == '\'')
    {
      inAphos = !inAphos;
    }
    else if(c == '\"')
    {
      inQuote = !inQuote;
    }
    if(c == '$' && !inAphos && !inQuote)
    {
      CString parNaam;
      do 
      {
        c = m_query.GetAt(++pos);
        parNaam += (char) c;
      } 
      while (isalnum(c) || c == '_');
      parNaam = parNaam.Left(parNaam.GetLength() - 1);
      // Found parameter name
      SQLVariant* par = GetParameter(parNaam);
      if(par)
      {
        SQLVariantFormat form(par);
        query += form.FormatVariantForSQL(m_database);
      }
    }
    else
    {
      query += (char)c;
      ++pos;
    }
  }
  return query;
}

// Parse the selection
CString
SQLDataSet::ParseSelection(SQLQuery& p_query)
{
  CString sql("SELECT ");
  sql += m_selection.IsEmpty() ? "*" : m_selection;
  sql += "\n  FROM ";

  if(!m_primarySchema.IsEmpty())
  {
    sql += m_primarySchema + ".";
  }
  sql += m_primaryTableName;

  int number = 0;
  ParameterSet::iterator it;
  for(it = m_parameters.begin();it != m_parameters.end(); ++it)
  {
    sql += (it == m_parameters.begin()) ? "\n WHERE " : "\n   AND ";
    sql += it->m_name;
    if(it->m_value.IsNULL())
    {
      sql += " IS NULL";
    }
    else
    {
      sql += " = ?";
      p_query.SetParameter(++number,&(it->m_value));
    }
  }
  return sql;
}

// Parse the fitlers
CString
SQLDataSet::ParseFilters()
{
  CString query(m_query);
  query.MakeUpper();
  query.Replace("\t"," ");
  bool whereFound = m_query.Find("WHERE ") > 0;
  bool first = true;

  // Restart with original query
  query = m_query;

  // Add all filters
  for(auto& filt : m_filters)
  {
    if(first == true)
    {
      if(!whereFound)
      {
        query += "\nWHERE ";
      }
    }
    else
    {
      query += "\n   AND ";
    }
    query += filt.GetSQLFilter();
  }
  return query;
}


bool
SQLDataSet::Open(bool p_stopIfNoColumns /*=false*/)
{
  bool   result = false;
  CString query = m_query;
  int    number = 0;

  if(m_query.IsEmpty() && m_selection.IsEmpty())
  {
    return false;
  }
  if(m_open)
  {
    Close();
  }
  // Test if possibly modifiable if primary table name and key are givven
  bool modifiable = false;
  if(!m_primaryTableName.IsEmpty() && m_primaryKey.size())
  {
    modifiable = true;
  }
  try
  {
    SQLQuery qr(m_database);
    SQLTransaction trans(m_database,m_name);

    // If parameters, parse them
    if(!m_query.IsEmpty())
    {
      if(m_parameters.size())
      {
        query = ParseQuery();
      }
      else if(m_filters.size())
      {
        query = ParseFilters();
      }
    }
    else
    {
      query = ParseSelection(qr);
    }

    // Do the SELECT query
    qr.DoSQLStatement(query);
    if (p_stopIfNoColumns && qr.GetNumberOfColumns() == 0)
    {
      return false;
    }

    // Read the names of the fields
    ReadNames(qr);
    ReadTypes(qr);

    // Searchable sets prepare primary key column
    int primary = FindSearchColumn();

    // Read all the records
    while(qr.GetRecord())
    {
      // Make a new record
      SQLRecord* record = new SQLRecord(this,modifiable);
      m_records.push_back(record);
      // Get all the columns of the record
      SQLVariant* var = NULL;
      int num = qr.GetNumberOfColumns();
      for(int ind = 1; ind <= num; ++ind)
      {
        var = qr.GetColumn(ind);
        record->AddField(var);

        // Record position of the primary
        if(ind == primary)
        {
          unsigned long prim = var->GetAsULong();
          m_objects[prim] = number++;
        }
      }
    }
    // Reached the end: we are OPEN!
    m_open = true;
    result = true;

    trans.Commit();
  }
  catch(CString s)
  {
    Close();
    throw s;
  }
  if(m_records.size())
  {
    m_current = 0;
    m_status |= SQL_Selections;
  }
  return result;
}

// Append (read extra) into the dataset
// Precondition: Dataset must already be opened by 'Open()'
// Precondition: Different parameter values set to read different data
// Precondition: Results must be of same result set (columns and datatypes)
bool 
SQLDataSet::Append()
{
  bool result = false;

  // See if already opened
  if(!m_open)
  {
    return false;
  }
  // Test if possibly modifiable if primary table name and key are given
  bool modifiable = false;
  if(!m_primaryTableName.IsEmpty() && m_primaryKey.size())
  {
    modifiable = true;
  }
  // Set last record to last current, setting m_current
  Last();
  // Remember size before reading
  size_t sizeBefore = m_records.size();
  int    number     = 1;

  try
  {
    SQLQuery qr(m_database);
    SQLTransaction trans(m_database,m_name);
    CString query(m_query);

    // Fill in the query with parameters / filters
    if(!m_query.IsEmpty())
    {
      if(m_parameters.size())
      {
        query = ParseQuery();
      }
      else if(m_filters.size())
      {
        query = ParseFilters();
      }
    }
    else
    {
      query = ParseSelection(qr);
    }

    // Do the SELECT query
    qr.DoSQLStatement(query);

    // Names and types must be the same as previous queries
    CheckNames(qr);
    CheckTypes(qr);

    // Finding the primary
    int primary = FindSearchColumn();

    while(qr.GetRecord())
    {
      // Make a new record
      SQLRecord* record = new SQLRecord(this,modifiable);
      bool foundNew = true;
      // Get all the columns of the record
      SQLVariant* var = NULL;
      int num = qr.GetNumberOfColumns();
      for(int ind = 1; ind <= num; ++ind)
      {
        var = qr.GetColumn(ind);
        record->AddField(var);

        // Record primary key in object cache
        if(ind == primary)
        {
          unsigned long prim = var->GetAsULong();
          if(m_objects.find(prim) == m_objects.end())
          {
          m_objects[prim] = m_current + number++;
        }
          else
          {
            foundNew = false;
          }
        }
      }
      // If we found a new record: keep it!
      if(foundNew)
      {
        m_records.push_back(record);
      }
    }
    // Legal 0 or more records
    result = true;
    trans.Commit();
  }
  catch(CString s)
  {
    Close();
    throw s;
  }
  // Goto the first freshly read record
  if(m_records.size() > sizeBefore)
  {
    m_status |= SQL_Selections;
    Next();
  }
  return result;
}

// Get all the columns of the record
void
SQLDataSet::ReadNames(SQLQuery& qr)
{
  CString naam;
  int num = qr.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    qr.GetColumnName(ind,naam);
    m_names.push_back(naam);
  }
}

// Get all  the types of the record
void
SQLDataSet::ReadTypes(SQLQuery& qr)
{
  int type;
  int num = qr.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    type = qr.GetColumnType(ind);
    m_types.push_back(type);
  }
}

// Check that all names are the same
void
SQLDataSet::CheckNames(SQLQuery& p_query)
{
  CString name;
  int num = p_query.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    p_query.GetColumnName(ind,name);
    if(m_names[ind-1].CompareNoCase(name))
    {
      throw CString("Append needs exactly the same query column names");
    }
  }
}

// Check that all the types are the same
void
SQLDataSet::CheckTypes(SQLQuery& p_query)
{
  int num = p_query.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    int type = p_query.GetColumnType(ind);
    if(m_types[ind-1] != type)
    {
      throw CString("Append needs exactly the same datatypes for the query columns.");
    }
  }
}

// Find primary key in the column names
int
SQLDataSet::FindSearchColumn()
{
  // Searchable primary requested?
  if(m_searchColumn.IsEmpty())
  {
    return 0;
  }
  // Find the column in the selected set
  for(unsigned ind = 0;ind < m_names.size(); ++ind)
  {
    if(m_searchColumn.CompareNoCase(m_names[ind]) == 0)
    {
      if(m_types[ind] == SQL_C_SLONG ||
         m_types[ind] == SQL_C_ULONG ||
         m_types[ind] == SQL_C_LONG  )
      {
        return ind + 1;
      }
    }
  }
  return 0;
}

// Check that all datatypes are the same
SQLRecord*
SQLDataSet::GetRecord(int p_recnum)
{
  if((unsigned)p_recnum >= 0 && (unsigned)p_recnum < m_records.size())
  {
    return m_records[p_recnum];
  }
  return NULL;
}

// Find the object record of an integer primary key
int
SQLDataSet::FindObjectRecNum(int p_primary)
{
  ObjectMap::iterator it = m_objects.find(p_primary);
  if(it != m_objects.end())
  {
    return it->second;
  }
  return -1;
}

// Find the object record of an integer primary key
SQLRecord*
SQLDataSet::FindObjectRecord(int p_primary)
{
  ObjectMap::iterator it = m_objects.find(p_primary);
  if(it != m_objects.end())
  {
    return m_records[it->second];
  }
  return NULL;
}

// If your primary is a compound key or not INTEGER (Slower)
int
SQLDataSet::FindObjectRecNum(VariantSet& p_primary)
{
  if(!GetPrimaryKeyInfo())
  {
    // No primary key, cannot do the find
    return -1;
  }
  if(!CheckPrimaryKeyColumns())
  {
    // Not all primary key columns are present
    return -1;
  }

  int searching = 0;
  for(int ind = 0;ind < (int) m_primaryKey.size(); ++ind)
  {
    int fieldnum = GetFieldNumber(m_primaryKey[ind]);
    SQLVariant* var = p_primary[ind];

    for(int search = searching; search < (int)m_records.size(); ++search)
    {
      SQLVariant* field = m_records[search]->GetField(fieldnum);
      if(field == var)
      {
        if(ind+1 == (int) m_primaryKey.size())
        {
          // Last field in the primary key. Stop here
          return search;
        }
        else
        {
          // Value found, get on to the next search field
          break;
        }
      }
      ++searching;
    }
  }
  return -1;
}

// If your primary is a compound key or not INTEGER (Slower)
SQLRecord*
SQLDataSet::FindObjectRecord(VariantSet& p_primary)
{
  if(!GetPrimaryKeyInfo())
  {
    // No primary key, cannot do the find
    return nullptr;
  }
  if(!CheckPrimaryKeyColumns())
  {
    // Not all primary key columns are present
    return nullptr;
  }

  int searching = 0;
  for(int ind = 0; ind < (int)m_primaryKey.size(); ++ind)
  {
    int fieldnum = GetFieldNumber(m_primaryKey[ind]);
    SQLVariant* var = p_primary[ind];

    for(int search = searching; search < (int)m_records.size(); ++search)
    {
      SQLVariant* field = m_records[search]->GetField(fieldnum);
      if(field == var)
      {
        if(ind + 1 == (int)m_primaryKey.size())
        {
          // Last field in the primary key. Stop here
          return m_records[search];
        }
        else
        {
          // Value found, get on to the next search field
          break;
        }
      }
      ++searching;
    }
  }
  return nullptr;
}

// Finding an object through a filter set
// Finds the first object. In case of an unique record, it will be the only one
SQLRecord*
SQLDataSet::FindObjectFilter(SQLFilterSet& p_filters,bool p_primary /*=false*/)
{
  SQLRecord* record = nullptr;

  // Optimize for network databases
  if(p_primary && p_filters.size() == 1)
  {
    SQLVariant* prim = p_filters[0].GetValue();
    if(p_filters[0].GetOperator() == OP_Equal && prim->GetDataType() == SQL_C_SLONG )
    {
      return FindObjectRecord(prim->GetAsSLong());
    }
  }

  // Walk the chain of records
  for(auto& rec : m_records)
  {
    record = rec;
    // Walk the chain of filters
    for(auto& filt : p_filters)
    {
      if(! filt.MatchRecord(record))
      {
        record = nullptr;
        break;
      }
    }
    // Result reached at first matching record
    if(record)
    {
      return record;
    }
  }
  return nullptr;
}

// Finding a set of records through a filter set
// Searches the complete recordset for all matches
// Caller must delete the resulting set!!
RecordSet* 
SQLDataSet::FindRecordSet(SQLFilterSet& p_filters)
{
  RecordSet* records = new RecordSet();

  // Walk the chain of records
  for(auto& rec : m_records)
  {
    SQLRecord* record = rec;
    // Walk the chain of filters
    for(auto& filt : p_filters)
    {
      if(! filt.MatchRecord(record))
      {
        record = nullptr;
        break;
      }
    }
    // If record found, keep it
    if(record)
    {
      records->push_back(record);
    }
  }

  // Return complete recordset
  return records;
}


// Get a fieldname
CString    
SQLDataSet::GetFieldName(int p_num)
{
  if(p_num >= 0 && (unsigned)p_num < m_names.size())
  {
    return m_names[p_num];
  }
  return "";
}

int
SQLDataSet::GetFieldType(int p_num)
{
  if(p_num >= 0 && (unsigned)p_num < m_types.size())
  {
    return m_types[p_num];
  }
  return -1;
}

// Get a field number
int
SQLDataSet::GetFieldNumber(CString p_name)
{
  for(unsigned int ind = 0; ind < m_names.size(); ++ind)
  {
    if(p_name.CompareNoCase(m_names[ind]) == 0)
    {
      return ind;
    }
  }
  return -1;
}

SQLVariant*
SQLDataSet::GetCurrentField(int p_num)
{
  if(m_current >= 0 && m_current < (int)m_records.size())
  {
    if(p_num >= 0 && p_num < (int)m_names.size())
    {
      return m_records[m_current]->GetField(p_num);
    }
  }
  return NULL;
}

// Insert new record
SQLRecord* 
SQLDataSet::InsertRecord()
{
  SQLRecord* record = new SQLRecord(this);
  m_records.push_back(record);
  m_current = (int)(m_records.size() - 1);
  m_status |= SQL_Insertions;
  return record;
}

// Insert new field in new record
int
SQLDataSet::InsertField(CString& p_name,SQLVariant* p_value)
{
  m_names.push_back(p_name);
  m_types.push_back(p_value->GetDataType());
  if(m_current >= 0 && m_current < (int)m_records.size())
  {
    SQLRecord* record = m_records[m_current];
    record->AddField(p_value);
    return 0;
  }
  // Internal programming error
  return -1;
}

// Set a field value in the current record
void
SQLDataSet::SetField(CString& p_name,SQLVariant* p_value,int p_mutationID /*=0*/)
{
  int num = GetFieldNumber(p_name);
  if(num >= 0)
  {
    SetField(num,p_value,p_mutationID);
  }
}

// Set a field value in the current record
void       
SQLDataSet::SetField(int p_num,SQLVariant* p_value,int p_mutationID /*=0*/)
{
  if(m_current >= 0)
  {
    m_records[m_current]->SetField(p_num,p_value,p_mutationID);
  }
}

// Cancel the mutations of this mutation ID
void
SQLDataSet::CancelMutation(int p_mutationID)
{
  bool mutated = false; 
  for(unsigned ind = 0;ind < m_records.size();++ind)
  {
    if(m_records[ind]->CancelMutation(p_mutationID))
    {
      mutated = true;
    }
  }
  if(!mutated)
  {
    // Remove the mutated status (leaving empty and selections alone)
    m_status &= ~(SQL_Updates | SQL_Insertions | SQL_Deletions);
  }
}

// Calculate aggregate functions
// COUNTS NULL VALUES AS 0 for the mean-result
int
SQLDataSet::Aggregate(int p_num,AggregateInfo& p_info)
{
  unsigned int total   = (int)m_records.size();
  for(unsigned int ind = 0;ind < total; ++ind)
  {
    SQLVariant* var = m_records[ind]->GetField(p_num);
    if(var && var->IsNULL() == false)
    {
      double waarde = var->GetAsDouble();
      if(p_info.m_max < waarde)
      {
        p_info.m_max = waarde;
      }
      if(p_info.m_min > waarde)
      {
        p_info.m_min = waarde;
      }
      p_info.m_sum += waarde;
      p_info.m_mean = p_info.m_sum / total;
    }
  }
  // Return the number of records processed
  return total;
}

//////////////////////////////////////////////////////////////////////////
//
// THE SYNCHRONIZATION PROCESS
//
//////////////////////////////////////////////////////////////////////////

// Insert / Update / delete records from the database
bool
SQLDataSet::Synchronize(int p_mutationID /*=0*/)
{
  // Needs the primary table name of the dataset
  if(m_primaryTableName.IsEmpty())
  {
    return false;
  }
  // Check if we have mutations
  if((m_status & (SQL_Record_Insert | SQL_Record_Deleted | SQL_Record_Updated)) == 0)
  {
    // Nothing to do: all OK.
    return true;
  }
  // Check preliminary conditions
  if(m_primaryTableName.IsEmpty() ||    // Needs the primary table name of the dataset
     !GetPrimaryKeyInfo()         ||    // Needs primary key info for doing updates/deletes
     !CheckPrimaryKeyColumns()    )     // Needs all of the primary key columns
  {
    // No primary key, cannot do updates/deletes
    return false;
  }

  // Save status before a possible throw
  int oldStatus = m_status;

  try
  {
    // Transaction on the stack for all mutations
    // Defer all constraint checking to the 'real' commit
    SQLTransaction trans(m_database,m_name);
    trans.SetTransactionDeferred();

    // Do all operations or throw
    if(m_status & SQL_Record_Deleted)
    {
      Deletes(p_mutationID);
    }
    if(m_status & SQL_Record_Updated)
    {
      Updates(p_mutationID);
    }
    if(m_status & SQL_Record_Insert)
    {
      Inserts(p_mutationID);
    }

    // Commit the changes to the database
    trans.Commit();

    // After the commit we throw away our changes
    Reduce(p_mutationID);
  }
  catch(CString& error)
  {
    // Automatic rollback will be done now
    m_database->LogPrint(1,"Database synchronization stopped: " + error);
    // Restore original status of the dataset, reduce never done
    m_status = oldStatus;
    return false;
  }
  // Ready
  return true;
}

// Getting the primary key for a table
bool
SQLDataSet::GetPrimaryKeyInfo()
{
  if(m_primaryKey.size() == 0)
  {
    SQLInfoDB*  info = m_database->GetSQLInfoDB();
    MPrimaryMap primaries;
    CString     constraintName;
    if(info->GetPrimaryKeyInfo(m_primaryTableName,constraintName,primaries) == false)
    {
      // No primary key given, and cannot get it from the ODBC driver
      return false;
    }
    // Remember the primary key info
    for(auto& key : primaries)
    {
      m_primaryKey.push_back(key.m_columnName);
    }
  }
  return m_primaryKey.size() > 0;
}

// Check that all primary key columns are in the dataset
bool
SQLDataSet::CheckPrimaryKeyColumns()
{
  NamenMap::iterator key,field;

  for(key = m_primaryKey.begin();key != m_primaryKey.end();++key)
  {
    bool foundKeypart = false;
    for(field = m_names.begin();field != m_names.end();++field)
    {
      if(field->CompareNoCase(*key) == 0)
      {
        foundKeypart = true;
        break;
      }
    }
    if(foundKeypart == false)
    {
      return false;
    }
  }
  // All key parts found
  return true;
}

void
SQLDataSet::Deletes(int p_mutationID)
{
  RecordSet::iterator it = m_records.begin();
  SQLQuery query(m_database);
  int total   = 0;
  int deletes = 0;

  // Loop through all the records
  while(it != m_records.end())
  {
    CString sql;
    SQLRecord* record = *it;
    if(record->GetStatus() & SQL_Record_Deleted)
    {
      ++total;
      MutType type = record->MixedMutations(p_mutationID);
      switch(type)
      {
        case MUT_OnlyOthers: ++it;  // do nothing with record
                             break;
        case MUT_Mixed:      throw CString("Mixed mutations");
        case MUT_NoMutation: // Fall through: Remove record
        case MUT_MyMutation: sql = GetSQLDelete(&query,record);
                             query.DoSQLStatement(sql);
                             // Delete this record, continuing to the next
//                              delete record;
//                              it = m_records.erase(it);
                             ForgetRecord(record,true);
                             ++deletes;
                             break;
      }
    }
    else
    {
      ++it;
    }
  }

  // Adjust the current record if necessary
  if(m_current >= (int)m_records.size())
  {
    m_current = (int)m_records.size() - 1;
  }

  // If we did all records, no more deletes are present
  if(total == deletes)
  {
    m_status &= ~SQL_Deletions;
  }
}

void
SQLDataSet::Updates(int p_mutationID)
{
  RecordSet::iterator it;
  SQLQuery query(m_database);
  int total  = 0;
  int update = 0;

  for(it = m_records.begin();it != m_records.end();++it)
  {
    SQLRecord* record = *it;
    if(record->GetStatus() & SQL_Record_Updated)
    {
      ++total;
      CString sql;
      MutType type = record->MixedMutations(p_mutationID);
      switch(type)
      {
        case MUT_NoMutation: // Fall through: do nothing
        case MUT_OnlyOthers: break;
        case MUT_Mixed:      throw CString("Mixed mutations");
        case MUT_MyMutation: sql = GetSQLUpdate(&query,record);
                             query.DoSQLStatement(sql);
                             ++update;
                             break;
      }
    }
  }

  // If we did all records, no more updates are present
  if(total == update)
  {
    m_status &= ~SQL_Updates;
  }
}

void
SQLDataSet::Inserts(int p_mutationID)
{
  RecordSet::iterator it;
  SQLQuery query(m_database);
  int total  = 0;
  int insert = 0;

  for(it = m_records.begin();it != m_records.end();++it)
  {
    SQLRecord* record = *it;
    if(record->GetStatus() & SQL_Record_Insert)
    {
      ++total;
      CString sql;
      MutType type = record->MixedMutations(p_mutationID);
      switch(type)
      {
        case MUT_NoMutation: // Fall through: Do nothing
        case MUT_OnlyOthers: break;
        case MUT_Mixed:      throw CString("Mixed mutations");
        case MUT_MyMutation: sql = GetSQLInsert(&query,record);
                             query.DoSQLStatement(sql);
                             ++insert;
                             break;
      }
    }
  }

  // If we did all records, no more inserts are present
  if(total == insert)
  {
    m_status &= ~SQL_Insertions;
  }
}

// Throws away my changes from the dataset
// Call only after all database synchronization has been done!
void
SQLDataSet::Reduce(int p_mutationID)
{
  RecordSet::iterator it;
  for(it = m_records.begin(); it != m_records.end(); ++it)
  {
    SQLRecord* record = *it;
    if(record->GetStatus() & (SQL_Record_Insert | SQL_Record_Deleted | SQL_Record_Updated))
    {
      MutType type = record->MixedMutations(p_mutationID);
      switch(type)
      {
        case MUT_NoMutation: break;
        case MUT_OnlyOthers: break;
        case MUT_Mixed:      break;
        case MUT_MyMutation: record->Reduce();
                             break;
      }
    }
  }
}

// In case synchronize doesn't work, ask for mixed mutations
int
SQLDataSet::AllMixedMutations(MutationIDS& p_list,int p_mutationID)
{
  int total = 0;
  RecordSet::iterator it;
  for(it = m_records.begin(); it != m_records.end(); ++it)
  {
    SQLRecord* record = *it;
    total += record->AllMixedMutations(p_list,p_mutationID);
  }
  return total;
}

CString
SQLDataSet::GetSQLDelete(SQLQuery* p_query,SQLRecord* p_record)
{
  // New set of parameters
  p_query->ResetParameters();

  CString sql("DELETE FROM " + m_primaryTableName + "\n");
  int parameter = 1;
  sql += GetWhereClause(p_query,p_record,parameter);
  return sql;
}

CString
SQLDataSet::GetSQLUpdate(SQLQuery* p_query,SQLRecord* p_record)
{
  int parameter = 1;
  CString sql("UPDATE " + m_primaryTableName + "\n");

  // New set of parameters
  p_query->ResetParameters();

  // Check for all fields
  bool first = true;
  for(unsigned ind = 0;ind < m_names.size(); ++ind)
  {
    if(p_record->IsModified(ind))
    {
      SQLVariant* value = p_record->GetField(ind);

      sql += first ? "   SET " : "      ,";
      sql += m_names[ind];
      if(value->IsNULL())
      {
        sql += " = NULL\n";
      }
      else
      {
        sql += " = ?\n";
        p_query->SetParameter(parameter++,value);
      }
    }
  }
  // Adding the WHERE clause
  sql += GetWhereClause(p_query,p_record,parameter);

  return sql;
}

CString
SQLDataSet::GetSQLInsert(SQLQuery* p_query,SQLRecord* p_record)
{
  int parameter = 1;
  CString sql("INSERT INTO " + m_primaryTableName);

  CString fields("(");
  CString params("(");

  // New set of parameters
  p_query->ResetParameters();

  // Do for all fields in the record
  for(unsigned ind = 0;ind < m_names.size(); ++ind)
  {
    SQLVariant* value = p_record->GetField(ind);
    if(value->IsNULL() == false)
    {
      fields += m_names[ind] + ",";
      params += "?,";
      p_query->SetParameter(parameter++,value);
    }
  }
  // Closing
  fields.TrimRight(',');
  params.TrimRight(',');
  fields += ")\n";
  params += ")";
  // Put it together
  sql += fields;
  sql += "VALUES ";
  sql += params;

  return sql;
}

CString
SQLDataSet::GetWhereClause(SQLQuery* p_query,SQLRecord* p_record,int& p_parameter)
{
  CString sql(" WHERE ");

  bool more = false;
  for(unsigned ind = 0;ind < m_primaryKey.size();++ind)
  {
    if(more)
    {
      sql += "\n   AND ";
    }
    more = true;
    int column = GetFieldNumber(m_primaryKey[ind]);
    sql += m_primaryKey[ind];
    SQLVariant* value = p_record->GetField(column);
    if(value->IsNULL())
    {
      sql += " = NULL";
    }
    else
    {
      sql += " = ?";
      p_query->SetParameter(p_parameter++,value);
    }
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
//
// Store in XML format
//
//////////////////////////////////////////////////////////////////////////

bool
SQLDataSet::XMLSave(CString p_filename,CString p_name,XMLEncoding p_encoding /*= XMLEncoding::ENC_UTF8*/)
{
  XMLMessage msg;
  msg.SetRootNodeName(p_name);

  XMLSave(&msg,msg.GetRoot());
  return msg.SaveFile(p_filename,p_encoding);
}

bool
SQLDataSet::XMLLoad(CString p_filename)
{
  XMLMessage msg;
  if(msg.LoadFile(p_filename))
  {
    XMLElement* root = msg.GetRoot();
    if(root)
    {
      XMLLoad(&msg,root);
      return true;
    }
  }
  return false;
}

void
SQLDataSet::XMLSave(XMLMessage* p_msg,XMLElement* p_dataset)
{
                          p_msg->AddElement(p_dataset,dataset_names[g_defaultLanguage][DATASET_NAME],XDT_String,m_name);
  XMLElement* structure = p_msg->AddElement(p_dataset,dataset_names[g_defaultLanguage][DATASET_STRUCTURE],XDT_String,"");
  CString nameField = dataset_names[g_defaultLanguage][DATASET_FIELD];

  // Add record structure
  SQLRecord* record = GetRecord(0);
  if(record)
  {
    for(unsigned int ind = 0;ind < m_names.size(); ++ind)
    {
      CString fieldname = GetFieldName(ind);
      SQLVariant* var   = record->GetField(ind);
      int type = var->GetDataType();

      XMLElement* field = p_msg->AddElement(structure,nameField,XDT_String,fieldname);
      p_msg->SetAttribute(field,dataset_names[g_defaultLanguage][DATASET_ID],(int)ind);
      p_msg->SetAttribute(field,dataset_names[g_defaultLanguage][DATASET_TYPE],type);
      p_msg->SetAttribute(field,dataset_names[g_defaultLanguage][DATASET_TYPENAME],var->FindDatatype(type));
    }
  }

  // Add records of the dataset
  XMLElement* records = p_msg->AddElement(p_dataset,dataset_names[g_defaultLanguage][DATASET_RECORDS],XDT_String,"");
  for(unsigned int ind = 0;ind < m_records.size(); ++ind)
  {
    m_records[ind]->XMLSave(p_msg,records);
  }
}

void
SQLDataSet::XMLLoad(XMLMessage* p_msg,XMLElement* p_dataset)
{
  XMLElement* structur = p_msg->FindElement(p_dataset,dataset_names[g_defaultLanguage][DATASET_STRUCTURE],false);
  XMLElement* records  = p_msg->FindElement(p_dataset,dataset_names[g_defaultLanguage][DATASET_RECORDS],  false);
  if(structur == NULL) throw CString("Structure part missing in the XML dataset.") + m_name;
  if(records  == NULL) throw CString("Records part missing in the XML dataset") + m_name;

  // Read the structure
  XMLElement* field = p_msg->GetElementFirstChild(structur);
  while(field)
  {
    // Remember the name of the field
    CString name = field->GetName();
    m_names.push_back(name);
    // Datatype of the field
    int type = p_msg->GetAttributeInteger(field,dataset_names[g_defaultLanguage][DATASET_TYPE]);
    m_types.push_back(type);
    // Next field
    field = p_msg->GetElementSibling(field);
  }

  // Read records
  XMLElement* record = p_msg->GetElementFirstChild(records);
  while(record)
  {
    SQLRecord* rec = InsertRecord();
    rec->XMLLoad(p_msg,record);
    // Next record
    record = p_msg->GetElementSibling(record);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// FORGETTING ABOUT A RECORD
//
//////////////////////////////////////////////////////////////////////////

// Record pointer cannot be a nullptr
bool
SQLDataSet::ForgetRecord(SQLRecord* p_record,bool p_force)
{
  if(p_force == false)
  {
    if(p_record->GetStatus() & (SQL_Record_Insert | SQL_Record_Updated | SQL_Record_Deleted))
    {
      // Do the synchronization first!
      return false;
    }
  }
  RecordSet::iterator it = find(m_records.begin(),m_records.end(),p_record);
  if(it != m_records.end())
  {
    // Try to release the record
    if(p_record->Release())
    {
      // Remove from m_objects. Maybe does nothing!
      ForgetPrimaryObject(p_record);

      // Remove from m_records
      m_records.erase(it);
    }
    return true;
  }
  return false;
}

// Forget the registration of the optimized primary key
void
SQLDataSet::ForgetPrimaryObject(SQLRecord* p_record)
{
  // See if we have a unique primary key
  int primary = FindSearchColumn();
  if(!primary)
  {
    return;
  }
  // Find the record
  long value = p_record->GetField(primary)->GetAsSLong();
  ObjectMap::iterator it = m_objects.find(value);
  if(it != m_objects.end())
  {
    // Remove the reference to the record
    it = m_objects.erase(it);

    // All other objects shift one place down!
    while(it != m_objects.end())
    {
      it->second--;
      ++it;
    }
    // Reset the current pointer
    First();
  }
}

// End of namespace
}
