//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file HashedData.cpp Hashed data array class (SHA-256)
//

// includes
#include "stdafx.h"
#include "HashedData.hpp"
#include "sha2.h"

HashedData::HashedData(const std::vector<unsigned char>& vecData)
:m_spData(new SHA256_CTX)
{
   SHA256_Init(m_spData.get());

   if (!vecData.empty())
      Add(vecData);
}

void HashedData::Add(const std::vector<unsigned char>& vecData)
{
   ATLASSERT(vecData.empty() == false);

   SHA256_Update(m_spData.get(), const_cast<u_int8_t*>(&vecData[0]), vecData.size());
}

std::vector<unsigned char> HashedData::Get() const
{
   u_int8_t abData[SHA256_DIGEST_LENGTH];
   SHA256_Final(abData, m_spData.get());

   return std::vector<unsigned char>(abData, abData + SHA256_DIGEST_LENGTH);
}
