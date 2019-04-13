//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DatabaseManager.hpp Database manager
//
#pragma once

// includes
#include "DatabaseCommon.hpp"
#include "IDatabaseInterface.hpp"
#include <vector>

// forward references
namespace Database
{
   class IDatabaseProvider;
   class IDatabase;

   /// game database filename
   const TCHAR c_pszDatabaseFilename[] = _T("game.db");

   /// setup script filename
   const TCHAR c_pszDatabaseSetupScriptFilename[] = _T("init.sql");

   /// file specification for update scripts; first four chars must be digits
   const TCHAR c_pszDatabaseUpdateScriptFilenameFilespec[] = _T("????_*.sql");

   /// filename format for update scripts
   const TCHAR c_pszDatabaseUpdateScriptFilenameFormat[] = _T("%04u_%s.sql");

   /// manages database access
   class DATABASE_DECLSPEC Manager
   {
   public:
      /// ctor
      Manager();

      /// dtor; closes database
      ~Manager();

      /// creates new database
      void CreateDatabase(const CString& cszFilename);

      /// opens existing database
      void OpenExisting(const CString& cszFilename);

      /// returns database instance; non-const version
      IDatabasePtr GetDatabase() const
      {
         ATLASSERT(m_spDatabase != NULL);
         return m_spDatabase;
      }

   private:
      /// returns database provider
      IDatabaseProviderPtr GetDatabaseProvider();

      /// sets up database tables
      void SetupDatabase();

      /// updates database to latest version
      void UpdateDatabase();

      /// sets up internal tables
      void SetupInternalTables();

      /// returns current database version
      unsigned int GetCurrentDatabaseVersion();

      /// sets new database version
      void SetDatabaseVersion(unsigned int uiDatabaseVersion);

      /// executes single sql script
      void ExecuteSqlScript(const CString& cszFilename);

      /// returns sql scripts for updating database to latest versions
      std::vector<CString> GetUpdateScriptFilenames() const;

   private:
      /// database object
      IDatabasePtr m_spDatabase;
   };

} // namespace Database
