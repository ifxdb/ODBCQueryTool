#include "StdAfx.h"
#include "DDLCreateTable.h"

namespace SQLComponents
{

DDLCreateTable::DDLCreateTable(SQLInfoDB* p_info)
               :m_info(p_info)
{
}

CString 
DDLCreateTable::GetTableDDL(CString p_tableName)
{
  // Remember what was asked of us
  m_tableName = p_tableName;

  try
  {
    GetTableInfo();
    GetColumnInfo();
    GetIndexInfo();
    GetPrimaryKeyInfo();
    GetForeignKeyInfo();
    GetAccessInfo();
  }
  catch(CString& error)
  {
    m_ddl.Empty();
    throw CString("ERROR in getting table DDL: ") + error;
  }
  return m_ddl;
}

bool
DDLCreateTable::SaveDDL(CString p_filename)
{
  FILE* file = nullptr;
  fopen_s(&file,p_filename,"w");
  if(file)
  {
    fputs(m_ddl,file);
    fclose(file);
    return true;
  }
  return false;
}

void   
DDLCreateTable::GetTableInfo()
{
  // Get primary table info
  m_tables.clear();
  CString errors;

  // Find table info
  m_tables.clear();
  if(!m_info->MakeInfoTableTablepart(m_tableName,m_tables,errors) || m_tables.empty())
  {
    throw CString("Cannot find table: ") + m_tableName + " : " + errors;
  }

  // Some engines get a synonym AND a table/view record
  // The table record always comes last.
  MetaTable& table = m_tables.back();

  // Construct table name
  if(!table.m_schema.IsEmpty())
  {
    m_tableName = table.m_schema + "." + table.m_table;
  }
  else
  {
    m_tableName = table.m_table;
  }

  // Optional remarks to begin with
  if(!table.m_remarks.IsEmpty())
  {
    m_ddl = "-- " + table.m_remarks;
  }

  // Do our DDL part
  m_ddl += "CREATE " + table.m_objectType + " " + m_tableName;
  m_ddl += "\n(\n";
}

void   
DDLCreateTable::GetColumnInfo()
{
  CString errors;
  bool    first = true;

  // Find column info
  m_columns.clear();
  if(!m_info->MakeInfoTableColumns(m_columns,errors) || m_columns.empty())
  {
    throw CString("Cannot find columns for table: ") + m_tableName + " : " + errors;
  }

  // Calculate max length of a column
  int length = CalculateColumnLength(m_columns);

  // Add columns
  for(auto& column : m_columns)
  {
    CString line("   ");
    line += first ? " " : ",";

    // Column name and type
    line += FormatColumnName(column.m_column,length);
    line += " ";
    line += column.m_typename;

    TypeInfo* type = m_info->GetTypeInfo(column.m_datatype);
    if(type)
    {
      line += ReplaceLengthPrecScale(type->m_create_params
                                    ,column.m_length
                                    ,column.m_precision
                                    ,column.m_scale);
    }
    // optional default value
    if(!column.m_default.IsEmpty() && column.m_default.CompareNoCase("null"))
    {
      line += " DEFAULT ";
      line += column.m_default;
    }
    // optional NOT NULL status
    if(column.m_nullable == SQL_NO_NULLS)
    {
      line += " NOT NULL";
    }
    // Optional remarks
    if(!column.m_remarks.IsEmpty())
    {
      line += " -- " + column.m_remarks;
    }

    // Stash the line
    StashTheLine(line);
    first = false;
  }
  m_ddl += ");\n\n";
}

void
DDLCreateTable::GetIndexInfo()
{
  CString errors;
  CString line;

  // Find column info
  m_indices.clear();
  m_info->MakeInfoTableStatistics(m_indices,nullptr,errors);
  if(!errors.IsEmpty())
  {
    throw CString("Cannot find indices for table: ") + m_tableName + " : " + errors;
  }

  // Walk the list of indices
  MStatisticsMap theIndex;
  for(auto& index : m_indices)
  {
    // Skip the table statistics record
    if(index.m_indexType == SQL_TABLE_STAT)
    {
      continue;
    }

    // Eventually create a new index
    if(index.m_position == 1)
    {
      if(!theIndex.empty())
      {
        line = m_info->GetSQLCreateIndex(theIndex);
        StashTheLine(line,";",2);
      }
      // Create new index statement
      theIndex.clear();
    }
    // Add the column line
    FindIndexFilter(index);
    theIndex.push_back(index);
  }
  if(!theIndex.empty())
  {
    line = m_info->GetSQLCreateIndex(theIndex);
    StashTheLine(line,";",2);
  }
}

void
DDLCreateTable::GetPrimaryKeyInfo()
{
  CString errors;

  // Find column info
  m_primaries.clear();
  m_info->MakeInfoTablePrimary(m_primaries,errors);
  if(!errors.IsEmpty())
  {
    throw CString("Cannot find the primary key for table: ") + m_tableName + " : " + errors;
  }

  if(!m_primaries.empty())
  {
    // Getting the alter table statement
    CString line = m_info->GetPrimaryKeyConstraint(m_primaries);
    StashTheLine(line,";",2);
  }
}

void
DDLCreateTable::GetForeignKeyInfo()
{
  CString errors;
  CString line;

  // Find column info
  m_foreigns.clear();
  m_info->MakeInfoTableForeign(m_foreigns,errors);
  if(!errors.IsEmpty())
  {
    throw CString("Cannot find the foreign keys for table: ") + m_tableName + " : " + errors;
  }

  // Do all foreign keys
  MForeignMap oneFKey;
  CString lastPrimTable;
  for(auto& foreign : m_foreigns)
  {
    if(foreign.m_pkTableName.Compare(lastPrimTable))
    {
      // start a new foreign key
      lastPrimTable = foreign.m_pkTableName;

      if(!oneFKey.empty())
      {
        line = m_info->GetSQLForeignKeyConstraint(oneFKey);
        StashTheLine(line,";",2);
      }
      oneFKey.clear();
    }
    // Keep this column
    oneFKey.push_back(foreign);
  }
  if(!oneFKey.empty())
  {
    line = m_info->GetSQLForeignKeyConstraint(oneFKey);
    StashTheLine(line,";",2);
  }
}

void
DDLCreateTable::GetAccessInfo()
{
  CString errors;
  CString line;

  // Find column info
  m_access.clear();
  m_info->MakeInfoTablePrivileges(m_access,errors);
  if(!errors.IsEmpty())
  {
    throw CString("Cannot find the privileges for table: ") + m_tableName + " : " + errors;
  }

  // Print all privileges
  for(auto& priv : m_access)
  {
    CString object;
    if(priv.m_schemaName.IsEmpty())
    {
      object = priv.m_tableName;
    }
    else
    {
      object = priv.m_schemaName + "." + priv.m_tableName;
    }

    // Primary privilege
    line.Format("GRANT %s ON %s TO %s",priv.m_privilege,object,priv.m_grantee);
    if(priv.m_grantable)
    {
      line += " WITH GRANT OPTION";
    }
    StashTheLine(line,";");
  }
}

//////////////////////////////////////////////////////////////////////////
//
// SERVICE ROUTINES
//
//////////////////////////////////////////////////////////////////////////

void
DDLCreateTable::StashTheLine(CString p_line,CString p_extraEnd /*=""*/,int p_newlines /*=1*/)
{
  // See if we have work to do
  if(p_line.IsEmpty())
  {
    return;
  }

  // Add the line
  m_ddl += p_line;

  // Optional extra end
  if(!p_extraEnd.IsEmpty())
  {
    m_ddl += p_extraEnd;
  }

  // Add the newlines
  for(int ind = 0; ind < p_newlines; ++ind)
  {
    m_ddl += "\n";
  }
}


CString
DDLCreateTable::ReplaceLengthPrecScale(CString p_template
                                       ,int p_length
                                       ,int p_precision
                                       ,int p_scale)
{
  // Set in lowercase for replacing
  p_template.MakeLower();

  // Confusion in some drivers
  if(p_length == 0 && p_precision > 0 && p_template.Find("length") >= 0)
  {
    p_length = p_precision;
    p_precision = 0;
  }

  // Format as strings
  CString length,precision,scale;
  length.Format("%d",p_length);
  precision.Format("%d",p_precision);
  scale.Format("%d",p_scale);

  // Replace as strings
  p_template.Replace("max length",length);    // ORACLE DOES THIS!!
  p_template.Replace("length",length);
  p_template.Replace("precision",precision);
  p_template.Replace("scale",scale);

  // Make sure we have parenthesis
  if(!p_template.IsEmpty() && p_template.Left(1) != "(")
  {
    p_template = "(" + p_template + ")";
  }
  return p_template;
}

CString
DDLCreateTable::FormatColumnName(CString p_column,int p_length)
{
  while(p_column.GetLength() < p_length)
  {
    p_column += " ";
  }
  return p_column;
}

int
DDLCreateTable::CalculateColumnLength(MColumnMap& p_columns)
{
  int length = 0;

  for(auto& column : p_columns)
  {
    if(column.m_column.GetLength() > length)
    {
      length = column.m_column.GetLength();
    }
  }
  return length;
}

void
DDLCreateTable::FindIndexFilter(MetaStatistics& p_index)
{
  // See if column name in index exists in the table
  for(auto& column : m_columns)
  {
    if(p_index.m_columnName.CompareNoCase(column.m_column) == 0)
    {
      return;
    }
  }
  // Not existing column. Find index filter
  p_index.m_filter = m_info->GetIndexFilter(p_index);
}

};