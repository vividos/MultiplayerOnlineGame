//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Asio.hpp Boost.Asio include
//
#pragma once

// don't link Regex and DateTime
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB

// want asio but no winsock init?
#ifdef ULIB_ASIO_NO_WINSOCK
   #define BOOST_ASIO_DETAIL_IMPL_WINSOCK_INIT_IPP
#endif

#include <boost/asio.hpp>

#ifdef ULIB_ASIO_NO_WINSOCK
   inline void boost::asio::detail::winsock_init_base::startup(boost::asio::detail::winsock_init_base::data &,unsigned char,unsigned char)
   {
   }
   inline void boost::asio::detail::winsock_init_base::cleanup(boost::asio::detail::winsock_init_base::data &)
   {
   }
   inline void boost::asio::detail::winsock_init_base::throw_on_error(boost::asio::detail::winsock_init_base::data &)
   {
   }
#endif
