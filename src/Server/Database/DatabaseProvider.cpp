//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DatabaseProvider.cpp database provider implementation
//

// includes
#include "stdafx.h"
#include "DatabaseSqlite3.hpp"

Database::IDatabaseProviderPtr Database::IDatabaseProvider::GetProvider(Database::EDatabaseProvider enProvider)
{
   switch (enProvider)
   {
   case databaseProviderSqlite3:
      return IDatabaseProviderPtr(new Sqlite3::DatabaseProvider);
      break;

   default:
      ATLASSERT(false);
      break;
   }

   return IDatabaseProviderPtr();
}
