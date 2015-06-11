//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Transaction.hpp auto-committing database transaction class
//
#pragma once

// includes
#include "IDatabaseInterface.hpp"

namespace Database
{

/// database transaction
class Transaction
{
public:
   /// ctor; begins transaction
   Transaction(IDatabasePtr spDatabase) throw()
      :m_spDatabase(spDatabase),
       m_bCommited(false)
   {
      ATLVERIFY(true == spDatabase->ExecuteDirect(_T("begin transaction")));
   }

   /// dtor; rolls back transaction when not already committed
   ~Transaction() throw()
   {
      if (!m_bCommited)
         Rollback();
   }

   /// commits transaction
   void Commit() throw()
   {
      ATLVERIFY(true == m_spDatabase->ExecuteDirect(_T("commit transaction")));
      m_bCommited = true;
   }

   /// rolls back transaction
   void Rollback() throw()
   {
      ATLVERIFY(true == m_spDatabase->ExecuteDirect(_T("rollback transaction")));
   }

private:
   /// pointer to database
   IDatabasePtr m_spDatabase;

   /// indicates if transaction is already committed
   bool m_bCommited;
};

} // namespace Database
