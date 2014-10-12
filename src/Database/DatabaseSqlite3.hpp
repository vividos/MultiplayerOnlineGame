//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DatabaseSqlite3.hpp database access implementation for sqlite3
//
#pragma once

// includes
#include "IDatabaseInterface.hpp"
#include "../Thirdparty/sqlite/sqlite3.h"

namespace Database
{

/// \brief sqlite3 database access classes
namespace Sqlite3
{

/// database provider class for sqlite3
class DatabaseProvider: public Database::IDatabaseProvider
{
public:
   virtual Database::IDatabasePtr Create(LPCTSTR pszFilename) override;
   virtual Database::IDatabasePtr Open(LPCTSTR pszFilename) override;
   virtual void Info(CString& cszProviderName, CString& cszVersion) override;
};

/// database class for sqlite3
class DatabaseSqlite3:
   public Database::IDatabase,
   public std::enable_shared_from_this<DatabaseSqlite3>
{
public:
   /// ctor
   DatabaseSqlite3(LPCTSTR pszFilename);

   /// dtor
   ~DatabaseSqlite3() throw();

   virtual Database::ICommandPtr OpenQuery(LPCTSTR pszCommandText) override;

   virtual bool ExecuteDirect(LPCTSTR pszCommandText) override;

   virtual CString GetLastError() override;

   /// returns db instance
   sqlite3* GetDb() const throw() { return m_pDb; }

private:
   /// db instance
   sqlite3* m_pDb;
};

/// command class for sqlite3
class Command:
   public Database::ICommand,
   public std::enable_shared_from_this<Command>
{
public:
   /// ctor
   Command(std::shared_ptr<DatabaseSqlite3> spDatabase, LPCTSTR pszCommandText);
   /// dtor
   ~Command();

   virtual unsigned int GetParamCount() override;

   virtual void SetParamNull(unsigned int uiIndex) override;
   virtual void SetParam(unsigned int uiIndex, const CString& cszText) override;
   virtual void SetParam(unsigned int uiIndex, int iValue) override;

   virtual bool Execute(Database::IResultSetPtr& spResultSet) override;

   virtual void Reset() override;

   virtual long long GetLastInsertRowId() override;

   /// returns db instance
   sqlite3* GetDb() const throw() { return m_spDatabase->GetDb(); }

   /// returns db statement instance
   sqlite3_stmt* GetStatement() const throw() { return m_pStmt; }

private:
   /// db statement instance
   sqlite3_stmt* m_pStmt;

   /// database
   std::shared_ptr<DatabaseSqlite3> m_spDatabase;
};

/// result set class for sqlite3
class ResultSet: public Database::IResultSet
{
public:
   /// ctor
   ResultSet(std::shared_ptr<Command> spCommand)
      :m_spCommand(spCommand)
   {
   }

   /// dtor
   virtual ~ResultSet() throw() {}

   virtual unsigned int GetColumnCount() override;

   virtual void GetValue(unsigned int uiIndex, CString& cszText) override;
   virtual void GetValue(unsigned int uiIndex, int& iValue) override;

   virtual bool MoveNext() override;

   /// returns db statement instance
   sqlite3_stmt* GetStatement() const throw() { return m_spCommand->GetStatement(); }

private:
   /// command instance
   std::shared_ptr<Command> m_spCommand;
};

} // namespace Sqlite3
} // namespace Database
