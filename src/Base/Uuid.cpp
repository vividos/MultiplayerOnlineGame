//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Uuid.cpp Universally Unique ID implementation
//

// includes
#include "stdafx.h"
#include "Uuid.hpp"

#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

static_assert(sizeof(Uuid) == 16, "Uuid must have a size of 16 bytes");

Uuid::Uuid(LPCTSTR pszUuid) throw()
{
   *this = FromString(pszUuid);
}

Uuid::Uuid(GUID& g) throw()
{
   memcpy(this, &g, sizeof(g));
}

Uuid::Uuid(const BYTE* abData) throw()
{
   memcpy(this, abData, sizeof(*this));
}

Uuid Uuid::Null() throw()
{
   GUID u = {0};
   return Uuid(u);
}

Uuid Uuid::New()
{
   GUID u = {0};
   RPC_STATUS ret = ::UuidCreate(&u);
   if (RPC_S_OK != ret)
      throw Exception(_T("Couldn't create new Uuid"), __FILE__, __LINE__);
   return Uuid(u);
}

Uuid Uuid::FromString(LPCTSTR pszUuid) throw()
{
   // note: the next cast only works when TCHAR == WCHAR
   static_cast(sizeof(TCHAR) == sizeof(WCHAR), "must compile using Unicode");

   RPC_WSTR w = reinterpret_cast<RPC_WSTR>(const_cast<LPTSTR>(pszUuid));
   GUID u;
   ::UuidFromString(w, &u);
   return Uuid(u);
}

CString Uuid::ToString() const throw()
{
   // we can pass "this" as UUID, since Uuid derives from GUID, and UUID is a
   // typedef of it.
   RPC_WSTR w = NULL;
   ATLVERIFY(RPC_S_OK == ::UuidToString(this, &w));

   CString cszUuid(reinterpret_cast<const WCHAR*>(w));

   ATLVERIFY(RPC_S_OK == ::RpcStringFree(&w));

   return cszUuid;
}

bool Uuid::operator==(const Uuid& lhs) const throw()
{
   return 0 == memcmp(this, &lhs, sizeof(*this));
}

bool Uuid::operator!=(const Uuid& lhs) const throw()
{
   return 0 != memcmp(this, &lhs, sizeof(*this));
}

bool Uuid::operator<(const Uuid& lhs) const throw()
{
   return memcmp(this, &lhs, sizeof(*this)) < 0;
}
