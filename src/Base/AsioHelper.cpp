//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file AsioHelper.cpp Boost.Asio helper functions
//

// includes
#include "StdAfx.h"
#include "AsioHelper.hpp"

CString EndpointToString(boost::asio::ip::tcp::endpoint& endpoint)
{
   boost::asio::ip::address addr = endpoint.address();

   boost::system::error_code ec;
   std::string a = addr.to_string(ec);

   CString cszEndpoint;
   if (ec)
   {
      ATLASSERT(false);
      cszEndpoint = _T("{error}");
   }
   else
   {
      if (addr.is_v4())
         cszEndpoint.Format(_T("%hs:%u"), a.c_str(), endpoint.port());
      else
         cszEndpoint.Format(_T("[%hs]:%u"), a.c_str(), endpoint.port());
   }

   return cszEndpoint;
}

bool IsAvailIPv6() throw()
{
   // try to resolve IPv6 localhost; only works when at least
   // one network card with IPv6 is installed.
   boost::asio::ip::address addr;

   boost::system::error_code ec;
   addr.from_string("::1", ec);

   return !ec;
}
