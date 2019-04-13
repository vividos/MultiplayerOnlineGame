//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AsioHelper.hpp Boost.Asio helper functions
//
#pragma once

// includes
#include "Base.hpp"
#include <ulib/config/BoostAsio.hpp>

/// converts endpoint to readable string
CString BASE_DECLSPEC EndpointToString(boost::asio::ip::tcp::endpoint& endpoint);

/// returns if IPv6 is available
bool BASE_DECLSPEC IsAvailIPv6();
