//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file StringTools.hpp String tools
//
#pragma once

// includes
#include "Base.hpp"
#include <vector>

/// string tools
namespace StringTools
{

/// converts (unicode) text to UTF-8 representation
void BASE_DECLSPEC StringToUTF8(const CString& cszText, std::vector<unsigned char>& vecUtf8Text);

/// converts a string containing hex bytes to bytes
void BASE_DECLSPEC HexStringToBytes(const CString& cszText, std::vector<unsigned char>& vecData);


} // namespace StringTools
