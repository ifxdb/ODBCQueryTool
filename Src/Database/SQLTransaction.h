////////////////////////////////////////////////////////////////////////
//
// File: SQLTransaction.h
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
#include <sqltypes.h>

class SQLDatabase;

class SQLTransaction 
{
public:
  SQLTransaction(SQLDatabase* p_database, 
                 const char*  p_name,
                 bool         p_startImmediate   = true, 
                 bool         p_isSubTransaction = false);
  SQLTransaction(HDBC p_hdbc, bool p_startImmediate);

  // Destructor will rollback at an open transaction
 ~SQLTransaction();

  // Transaction on the database. If a transaction is already started and isSubTransaction
  // is requested, it will start a sub-transaction's SAVEPOINT, that can be rolled back
  // independently. Whenever a subtransaction is off, the transaction will be linked to 
  // earlier transactions, and thus influence the complete behaviour,
  // until the last commit/rollback of the main transaction
  void Start(CString p_name,bool p_startSubtransaction = false);
  // Commit the transaction
  void Commit();
  // Rollback the transaction
  void Rollback();
  // Is the transaction (still) active
  bool IsActive() const;
  // Database for which the transaction is active
  SQLDatabase* GetDatabase() const;
  // Name of the savepoint in the database
  CString GetSavePoint() const;
  // Setting a transaction in a deferred state
  // so that constraints get only committed at the end
  bool SetTransactionDeferred();
  // Setting a transaction in an immediate state
  // So that the constraints (uptil now) get checked immediately
  bool SetTransactionImmediate();

private:
  friend class SQLDatabase;
  // To do after a rollback
  void AfterRollback();

private:
  SQLDatabase*  m_database;
  HDBC          m_hdbc;
  CString       m_name;
  CString       m_savepoint;
  bool          m_active;
};

inline bool 
SQLTransaction::IsActive() const
{
  return m_active;
}

inline SQLDatabase*
SQLTransaction::GetDatabase() const
{
  return m_database;
}

inline CString 
SQLTransaction::GetSavePoint() const
{
  return m_savepoint;
}