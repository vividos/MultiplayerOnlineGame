//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DatabaseManager.cpp Database manager
//

// includes
#include "StdAfx.h"
#include "DatabaseManager.hpp"
#include "IDatabaseInterface.hpp"
#include <ulib/Exception.hpp>
#include "Transaction.hpp"
#include <ulib/log/Logger.hpp>
#include <ulib/stream/TextFileStream.hpp>
#include "LogCategories.hpp"
#include "Filesystem.hpp"
#include <ulib/FileFinder.hpp>
#include <ulib/Path.hpp>

using namespace Database;

Manager::Manager()
{
}

Manager::~Manager()
{
}

void Manager::CreateDatabase(const CString& cszFilename)
{
   IDatabaseProviderPtr spProvider = GetDatabaseProvider();

   m_spDatabase = spProvider->Create(cszFilename);

   CString cszMessage;
   cszMessage.Format(_T("creating database: %s"), cszFilename.GetString());
   LOG_INFO(cszMessage, Log::Server::Database);

   SetupDatabase();
   UpdateDatabase();
}

void Manager::OpenExisting(const CString& cszFilename)
{
   LOG_INFO(_T("opening database: ") + cszFilename, Log::Server::Database);

   if (!Path(cszFilename).FileExists())
      throw Exception(_T("couldn't find database file: ") + cszFilename, __FILE__, __LINE__);

   IDatabaseProviderPtr spProvider = GetDatabaseProvider();

   m_spDatabase = spProvider->Open(cszFilename);

   CString cszMessage;
   cszMessage.Format(_T("database open, current version %u"), GetCurrentDatabaseVersion());
   LOG_INFO(cszMessage, Log::Server::Database);

   UpdateDatabase();
}

IDatabaseProviderPtr Manager::GetDatabaseProvider()
{
   IDatabaseProviderPtr spProvider = Database::IDatabaseProvider::GetProvider();

   if (spProvider == nullptr)
      throw Exception(_T("couldn't get database provider"), __FILE__, __LINE__);

   CString cszProviderName, cszVersion;
   spProvider->Info(cszProviderName, cszVersion);

   CString cszLogMessage;
   cszLogMessage.Format(_T("Opening DB provider: [%s] version [%s]"),
      cszProviderName.GetString(), cszVersion.GetString());
   LOG_INFO(cszLogMessage, Log::Server::Database);

   return spProvider;
}

void Manager::SetupDatabase()
{
   ATLASSERT(m_spDatabase != NULL);

   LOG_INFO(_T("setting up new database"), Log::Server::Database);

   Database::Transaction transaction(m_spDatabase);

   SetupInternalTables();

   // also execute setup script
   ExecuteSqlScript(Filesystem().BaseFolder() + _T("script\\") + c_pszDatabaseSetupScriptFilename);

   transaction.Commit();
}

void Manager::UpdateDatabase()
{
   ATLASSERT(m_spDatabase != NULL);

   unsigned int uiDatabaseVersion = GetCurrentDatabaseVersion();
   unsigned int uiTargetVersion = uiDatabaseVersion;

   // find out all sql scripts to execute
   std::map<unsigned int, CString> mapSqlScriptFilenames;
   std::vector<CString> vecUpdateScriptFilenames = GetUpdateScriptFilenames();
   for (size_t i=0,iMax=vecUpdateScriptFilenames.size(); i<iMax; i++)
   {
      // check filename part only
      CString cszFilename = vecUpdateScriptFilenames[i];
      int iPos = cszFilename.ReverseFind(_T('\\'));
      if (iPos != -1)
         cszFilename = cszFilename.Mid(iPos+1);

      if (cszFilename.GetLength() < 4 ||
         !isdigit(cszFilename[0]) ||
         !isdigit(cszFilename[1]) ||
         !isdigit(cszFilename[2]) ||
         !isdigit(cszFilename[3]))
      {
         // error in filename
         LOG_WARN(_T("update script filename not in proper format: ") + cszFilename, Log::Server::Database);
         continue;
      }

      unsigned int uiFileVersion = static_cast<unsigned int>(_tcstoul(cszFilename.Left(4), NULL, 10));

      if (uiDatabaseVersion < uiFileVersion)
         mapSqlScriptFilenames.insert(std::make_pair(uiFileVersion, vecUpdateScriptFilenames[i]));

      uiTargetVersion = std::max(uiTargetVersion, uiFileVersion);
   }

   // execute all update scripts up to newest version, in version number order
   std::map<unsigned int, CString>::const_iterator iter = mapSqlScriptFilenames.begin(),
      stop = mapSqlScriptFilenames.end();

   if (iter == stop)
      return;

   Database::Transaction transaction(m_spDatabase);

   CString cszText;
   cszText.Format(_T("updating database from version %u to version %u"),
      uiDatabaseVersion, uiTargetVersion);
   LOG_INFO(cszText, Log::Server::Database);

   unsigned int uiCurrentVersion = 0;
   for (; iter != stop; ++iter)
   {
      uiCurrentVersion = iter->first;

      ExecuteSqlScript(iter->second);
   }

   // as last change, update version
   SetDatabaseVersion(uiCurrentVersion);

   transaction.Commit();
}

void Manager::SetupInternalTables()
{
   LPCTSTR pszCommands[] =
   {
      _T("create table db_info (")
         _T("'name' text primary key not null,")
         _T("'value' text)"),

      _T("insert into db_info (name, value) values ('version', 0)"),
   };

   for (size_t i=0, iMax=sizeof(pszCommands)/sizeof(*pszCommands); i<iMax; i++)
   {
      ICommandPtr spQuery = m_spDatabase->OpenQuery(pszCommands[i]);
      IResultSetPtr spResultSet;
      ATLVERIFY(true == spQuery->Execute(spResultSet));
   }
}

unsigned int Manager::GetCurrentDatabaseVersion()
{
   LPCTSTR c_pszSqlGetVersion = _T("select value from db_info where name='version'");
   ICommandPtr spQuery = m_spDatabase->OpenQuery(c_pszSqlGetVersion);
   IResultSetPtr spResultSet;
   ATLVERIFY(true == spQuery->Execute(spResultSet));

   int iVersion = 0;
   spResultSet->GetValue(0, iVersion);
   return static_cast<unsigned int>(iVersion);
}

void Manager::SetDatabaseVersion(unsigned int uiDatabaseVersion)
{
   ICommandPtr spQuery = m_spDatabase->OpenQuery(
      _T("update db_info set value=? where name='version'"));
   spQuery->SetParam(0, static_cast<int>(uiDatabaseVersion));

   IResultSetPtr spResultSet;
   ATLVERIFY(true == spQuery->Execute(spResultSet));
}

void Manager::ExecuteSqlScript(const CString& cszFilename)
{
   LOG_INFO(_T("executing sql script: ") + cszFilename, Log::Server::Database);

   Stream::TextFileStream script(cszFilename,
      Stream::FileStream::modeOpen,
      Stream::FileStream::accessRead,
      Stream::FileStream::shareRead,
      Stream::ITextStream::textEncodingAnsi);

   if (script.IsOpen())
   {
      CString cszLine, cszQuery;
      while (!script.AtEndOfStream())
      {
         script.ReadLine(cszLine);

         // remove sql comments
         int iPos = cszLine.Find(_T("--"));
         if (iPos != -1)
            cszLine = cszLine.Left(iPos);

         if (!cszQuery.IsEmpty())
            cszQuery += _T(" ");

         cszQuery += cszLine;

         // check if query is complete
         CString cszQuery2(cszQuery);
         cszQuery2.TrimRight();
         if (cszQuery2.Right(1) == _T(';') || script.AtEndOfStream())
         {
            bool bResult = m_spDatabase->ExecuteDirect(cszQuery);
            if (!bResult)
            {
               CString cszError;
               cszError.Format(_T("error in executing sql in file [%s], sql command [%s]"),
                  cszFilename.GetString(), cszLine.GetString());
               LOG_ERROR(cszError, Log::Server::Database);
            }

            cszQuery.Empty();
         }
      }
   }
}

std::vector<CString> Manager::GetUpdateScriptFilenames() const
{
   std::vector<CString> vecFilenames;
   Filesystem fs;
   FileFinder ff(fs.DatabaseScriptFolder(), c_pszDatabaseUpdateScriptFilenameFilespec);
   if (ff.IsValid())
   {
      do
      {
         if (ff.IsDot() || ff.IsFolder())
            continue;

         vecFilenames.push_back(ff.Filename());

      } while (ff.Next());
   }

   return vecFilenames;
}
