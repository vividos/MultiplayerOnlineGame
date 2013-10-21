//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file RC4EncryptModule.hpp RC4 encrypt module
//
#pragma once

// includes
#include "Base.hpp"
#include "IEncryptModule.hpp"
#include "RC4Encoder.hpp"
#include <vector>

namespace RC4
{

/// encrypt module that uses RC4 algorithm
class EncryptModule: public IEncryptModule
{
public:
   /// ctor
   EncryptModule(std::vector<unsigned char> vecSharedKey) throw()
      :m_rc4Read(&vecSharedKey[0], vecSharedKey.size()),
       m_rc4Write(&vecSharedKey[0], vecSharedKey.size())
   {
   }
   /// dtor
   virtual ~EncryptModule() throw() {}

   /// decrypts received data
   virtual void DecryptRead(TIterator itFirst, TIterator itLast) override
   {
      m_rc4Read.Encrypt<TIterator>(itFirst, itLast);
   }

   /// encrypts data to send
   virtual void EncryptWrite(TIterator itFirst, TIterator itLast) override
   {
      m_rc4Write.Encrypt<TIterator>(itFirst, itLast);
   }

private:
   RC4::Encoder m_rc4Read;    ///< encoder for receiving data
   RC4::Encoder m_rc4Write;   ///< encoder for sending out data
};

} // namespace RC4
