//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IDatabaseInterface.hpp database interface
//
#pragma once

// includes
#include <memory>

/// \brief database access
namespace Database
{

/// possible database provider
enum EDatabaseProvider
{
   databaseProviderSqlite3=0,  ///< sqlite3 provider
};

class IDatabaseProvider;

/// smart pointer type for IDatabaseProvider
typedef std::shared_ptr<IDatabaseProvider> IDatabaseProviderPtr;

class IDatabase;

/// smart pointer type for IDatabase
typedef std::shared_ptr<IDatabase> IDatabasePtr;

/// database provider interface
class IDatabaseProvider
{
public:
   /// dtor
   virtual ~IDatabaseProvider() throw() {}

   /// returns provider for given database type
   static IDatabaseProviderPtr GetProvider(EDatabaseProvider enProvider = databaseProviderSqlite3);

   /// creates new database with given filename
   virtual IDatabasePtr Create(LPCTSTR pszFilename) = 0;

   /// opens existing database with given filename
   virtual IDatabasePtr Open(LPCTSTR pszFilename) = 0;

   /// returns provider/database infos
   virtual void Info(CString& cszProviderName, CString& cszVersion) = 0;
};

class ICommand;

/// smart pointer type for ICommand
typedef std::shared_ptr<ICommand> ICommandPtr;

/// database interface
class IDatabase
{
public:
   /// dtor
   virtual ~IDatabase() throw() {}

   /// opens a new query with given sql command
   virtual ICommandPtr OpenQuery(LPCTSTR pszCommandText) = 0;

   /// directly executes sql command (and throw away result, if any)
   virtual bool ExecuteDirect(LPCTSTR pszCommandText) = 0;

   /// returns last db error as text
   virtual CString GetLastError() = 0;
};

class IResultSet;

/// smart pointer type for IResultSet
typedef std::shared_ptr<IResultSet> IResultSetPtr;

/// database command
class ICommand
{
public:
   /// dtor
   virtual ~ICommand() throw() {}

   /// returns parameter count of prepared statement
   virtual unsigned int GetParamCount() = 0;

   /// sets parameter to null
   /// \note all index values are 0-based
   virtual void SetParamNull(unsigned int uiIndex) = 0;

   /// sets string parameter
   virtual void SetParam(unsigned int uiIndex, const CString& cszText) = 0;

   /// sets integer parameter
   virtual void SetParam(unsigned int uiIndex, int iValue) = 0;

   /// executes prepared statement
   virtual bool Execute(IResultSetPtr& spResultSet) = 0;

   /// resets prepared statement for next execution; parameters keep being set
   virtual void Reset() = 0;

   /// returns row id of last insert operation (for autoincrement primary key field)
   virtual long long GetLastInsertRowId() = 0;
};

/// query result set
class IResultSet
{
public:
   /// dtor
   virtual ~IResultSet() throw() {}

   /// returns column count of result
   virtual unsigned int GetColumnCount() = 0;

   /// returns text result value
   /// \note all index values are 0-based
   virtual void GetValue(unsigned int uiIndex, CString& cszText) = 0;

   /// returns integer result value
   virtual void GetValue(unsigned int uiIndex, int& iValue) = 0;

   /// moves to next result; returns false when no more results are available
   virtual bool MoveNext() = 0;
};

} // namespace Database
