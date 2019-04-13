//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IEncryptModule.hpp Encrypt module interface
//
#pragma once

// includes
#include <vector>

/// interface for encrypt module of a session
class IEncryptModule
{
public:
   /// iterator type for byte ranges
   typedef std::vector<unsigned char>::iterator TIterator;

   /// dtor
   virtual ~IEncryptModule() {}

   /// decrypts received bytes
   virtual void DecryptRead(TIterator itFirst, TIterator itLast) = 0;

   /// encrypts bytes to send
   virtual void EncryptWrite(TIterator itFirst, TIterator itLast) = 0;
};
