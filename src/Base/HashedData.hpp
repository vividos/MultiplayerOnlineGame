//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file HashedData.hpp Hashed data array class (SHA-256)
//
#pragma once

// includes
#include "Base.hpp"
#include <vector>

/// contains hashed data; SHA-256 is used internally
class BASE_DECLSPEC HashedData
{
public:
   /// ctor; assigns hashed data
   HashedData(const std::vector<unsigned char>& vecData);

   /// adds more data to the already hashed data
   void Add(const std::vector<unsigned char>& vecData);

   /// returns hashed data; 32 data bytes (256 bits)
   std::vector<unsigned char> Get() const throw();

private:
   /// private data; hash context
   std::shared_ptr<struct _SHA256_CTX> m_spData;
};
