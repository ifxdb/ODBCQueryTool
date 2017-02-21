////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoTree.h
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
#pragma once
#include "SQLDatabase.h"
#include "SQLInfoDB.h"

using namespace SQLComponents;

class InfoTree : public CTreeCtrl
{
public:
  InfoTree();
  InfoTree(SQLDatabase* p_database);
 ~InfoTree();

  // For database open/cloase
  void SetDatabase(SQLDatabase* p_database);
  // All ODBC Functions to the tree
  void MakeTreeInfo ();
  // Report all capabilities to a system file
  virtual void ReportAllCapabilities(CString& filename);

private:
  CString   InfoNumberToString(SQLINTEGER num);
  HTREEITEM DisplayBitfield(HTREEITEM   item
                           ,SQLUINTEGER var
                           ,SQLUINTEGER macro
                           ,char*       text);
  // Helper for displaying SQLGetInfo information
  void DisplayConversion(HTREEITEM  item
                        ,CString    type
                        ,SQLINTEGER totype);
  // Make ODBC Extensions in a tree control
  void MakeTreeODBCExtensions(HTREEITEM item);
  // All ODBC Info to a tree control
  void MakeTreeInfoFunctions(HTREEITEM item);
  void WordListToTree(WordList& p_list,HTREEITEM p_item);
  void MetaListToTree(MMetaMap& p_list,HTREEITEM p_item,CString& p_errors);


  // Printing the DBInfo tree to a report
  void ReportCapabilities(HTREEITEM  p_branch
                         ,int        p_level
                         ,FILE*      p_file);

  SQLDatabase* m_database;
  SQLInfoDB*   m_info;
};
