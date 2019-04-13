//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DatabaseSqlite3.cpp database access implementation for sqlite3
//

// includes
#include "stdafx.h"
#include "DatabaseSqlite3.hpp"

#if !defined(UNICODE) || !defined(_UNICODE)
#  error can only be used with unicode projects!
#endif

using namespace Database::Sqlite3;

// DatabaseProvider

Database::IDatabasePtr DatabaseProvider::Create(LPCTSTR pszFilename)
{
   DeleteFile(pszFilename);
   return Database::IDatabasePtr(new DatabaseSqlite3(pszFilename));
}

Database::IDatabasePtr DatabaseProvider::Open(LPCTSTR pszFilename)
{
   return Database::IDatabasePtr(new DatabaseSqlite3(pszFilename));
}

void DatabaseProvider::Info(CString& cszProviderName, CString& cszVersion)
{
   cszProviderName = _T("sqlite db provider");
   cszVersion = _T("sqlite ") _T(SQLITE_VERSION);
}

// Database

DatabaseSqlite3::DatabaseSqlite3(LPCTSTR pszFilename)
{
   ATLVERIFY(SQLITE_OK == sqlite3_open16(pszFilename, &m_pDb));
}

DatabaseSqlite3::~DatabaseSqlite3()
{
   ATLVERIFY(SQLITE_OK == sqlite3_close(GetDb()));
}

Database::ICommandPtr DatabaseSqlite3::OpenQuery(LPCTSTR pszCommandText)
{
   return Database::ICommandPtr(new Command(shared_from_this(), pszCommandText));
}

bool DatabaseSqlite3::ExecuteDirect(LPCTSTR pszCommandText)
{
   Database::ICommandPtr spCommand = OpenQuery(pszCommandText);
   if (spCommand.get() == NULL)
      return false;

   Database::IResultSetPtr spResultSet;
   return spCommand->Execute(spResultSet);
}

CString DatabaseSqlite3::GetLastError()
{
   CString cszText = reinterpret_cast<LPCWSTR>(sqlite3_errmsg16(GetDb()));
   return cszText;
}

// Command

Command::Command(std::shared_ptr<DatabaseSqlite3> spDatabase, LPCTSTR pszCommandText)
:m_spDatabase(spDatabase)
{
   int iRet = sqlite3_prepare16(spDatabase->GetDb(), pszCommandText, -1, &m_pStmt, NULL);
   ATLVERIFY(SQLITE_OK == iRet);

   if (iRet != SQLITE_OK)
   {
      ATLTRACE(_T("SQLITE: error while preparing [%s]:\n %s\n"),
         pszCommandText,
         sqlite3_errmsg16(spDatabase->GetDb()));
   }
   else
      ATLTRACE(_T("SQLITE: preparing [%s]\n"), pszCommandText);
}

Command::~Command()
{
   ATLVERIFY(SQLITE_OK == sqlite3_finalize(m_pStmt));
   m_pStmt = NULL;
}

unsigned int Command::GetParamCount()
{
   return static_cast<unsigned int>(sqlite3_bind_parameter_count(m_pStmt));
}

void Command::SetParamNull(unsigned int uiIndex)
{
   // 0-based index
   ATLASSERT(uiIndex < GetParamCount());

   // converting index to 1-based here
   ATLVERIFY(SQLITE_OK == sqlite3_bind_null(m_pStmt, uiIndex+1));
}

void Command::SetParam(unsigned int uiIndex, const CString& cszText)
{
   // 0-based index
   ATLASSERT(uiIndex < GetParamCount());

   // converting index to 1-based here
   ATLVERIFY(SQLITE_OK == sqlite3_bind_text16(m_pStmt, uiIndex+1, static_cast<LPCTSTR>(cszText), -1, SQLITE_TRANSIENT));
}

void Command::SetParam(unsigned int uiIndex, int iValue)
{
   // 0-based index
   ATLASSERT(uiIndex < GetParamCount());

   // converting index to 1-based here
   ATLVERIFY(SQLITE_OK == sqlite3_bind_int(m_pStmt, uiIndex+1, iValue));
}

bool Command::Execute(Database::IResultSetPtr& spResultSet)
{
   int iRet = sqlite3_step(m_pStmt);

   ATLASSERT(iRet != SQLITE_BUSY);

   if (iRet == SQLITE_MISUSE || iRet == SQLITE_ERROR)
      return false;

   if (iRet == SQLITE_DONE)
   {
      spResultSet.reset();
      return true;
   }

   ATLASSERT(SQLITE_ROW == iRet);

   if (iRet != SQLITE_ROW)
   {
      ATLTRACE(_T("SQLITE: error while executing:\n %s\n"),
         sqlite3_errmsg16(GetDb()));
   }

   spResultSet.reset(new ResultSet(shared_from_this()));
   return true;
}

void Command::Reset()
{
   int iRet = sqlite3_reset(m_pStmt);
   ATLASSERT(SQLITE_OK == iRet);
   if (iRet != SQLITE_OK)
   {
      ATLTRACE(_T("SQLITE: error while resetting:\n %s\n"),
         sqlite3_errmsg16(GetDb()));
   }
}

long long Command::GetLastInsertRowId()
{
   return sqlite3_last_insert_rowid(GetDb());
}

unsigned int ResultSet::GetColumnCount()
{
   ATLASSERT(GetStatement() != NULL);

   return static_cast<unsigned int>(sqlite3_data_count(GetStatement()));
}

void ResultSet::GetValue(unsigned int uiIndex, CString& cszText)
{
   ATLASSERT(GetStatement() != NULL);
   ATLASSERT(uiIndex < GetColumnCount()); // already 0-based index

   cszText = reinterpret_cast<LPCWSTR>(sqlite3_column_text16(GetStatement(), uiIndex));
}

void ResultSet::GetValue(unsigned int uiIndex, int& iValue)
{
   ATLASSERT(GetStatement() != NULL);
   ATLASSERT(uiIndex < GetColumnCount()); // already 0-based index

   iValue = sqlite3_column_int(GetStatement(), uiIndex);
}

bool ResultSet::MoveNext()
{
   ATLASSERT(GetStatement() != NULL);

   int iRet = sqlite3_step(GetStatement());

   ATLASSERT(iRet != SQLITE_BUSY);

   // error
   if (iRet == SQLITE_MISUSE || iRet == SQLITE_ERROR)
      return false;

   if (iRet == SQLITE_DONE)
      return false;

   ATLASSERT(SQLITE_ROW == iRet);

   // next row is available
   return true;
}
