////////////////////////////////////////////////////////////////////////
//
// File: SQLAssociation.h
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
#pragma once

#include "SQLDataSet.h"
#include "SQLRecord.h"
#include <vector>

typedef struct _primaryForeign
{
  CString     m_primary;
  CString     m_foreign;
  SQLVariant* m_value;
}
PrimaryForeign;

typedef std::vector<PrimaryForeign*> PFMap; 

class SQLAssociation
{
public:
  SQLAssociation();
 ~SQLAssociation();

  // ACTIONS
  bool UpdateMaster();
  bool UpdateDetails();

  // SETTERS
  void SetMaster          (SQLDataSet* p_master)  { m_master          = p_master; };
  void SetDetail          (SQLDataSet* p_detail)  { m_detail          = p_detail; };
  void SetMasterRecord(SQLRecord* p_record)   { m_masterRecord    = p_record; };
  void SetAssociation (CString p_primaryColumn,CString p_foreignColumn,SQLVariant* p_value = NULL);
  void SetAssociation (int p_num,SQLVariant* p_value);

  // GETTERS
  SQLDataSet* GetMaster()           { return m_master; };
  SQLDataSet* GetDetail()           { return m_detail; };
  SQLRecord*  GetMasterRecord()     { return m_masterRecord;   };
  CString     GetAssocationName (int p_column);
  SQLVariant* GetAssocationValue(int p_column);

private:
  // Free the association columns and values
  void        FreeAssocs();
  // Basic checks for a valid association
  bool        BasicChecks(bool p_toDetails);

  SQLDataSet* m_master;
  SQLDataSet* m_detail;
  PFMap       m_assocs;
  SQLRecord*  m_masterRecord;
};