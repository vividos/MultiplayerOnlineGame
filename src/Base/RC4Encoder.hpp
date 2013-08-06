//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file RC4Encoder.hpp RC4 encoder class
//
#pragma once

// includes
#include <algorithm>

/// \brief RC4 cipher classes
namespace RC4
{

/// \brief RC4 cipher encoder class
/// \details Since RC4 is a symmetric cipher, this class can also be used as decoder.
/// coded after pseudo code in http://de.wikipedia.org/wiki/RC4
class Encoder
{
public:
   /// ctor
   /// \param[in] key key bytes
   /// \param[in] key_length key length
   Encoder(const unsigned char* key, size_t key_length) throw()
      :i(0),
       j(0)
   {
      Init(key, key_length);
   }

   /// returns next byte to xor with
   unsigned char Get() throw() { return Output(); }

   /// encrypts given iterator range
   /// \tparam TForwardIterator forward iterator type
   /// \param itFirst start of range
   /// \param itLast end of range
   template <typename TForwardIterator>
   void Encrypt(TForwardIterator itFirst, TForwardIterator itLast) throw()
   {
      for (TForwardIterator it = itFirst; it != itLast; it++)
         (*it) ^= Output();
   }

   /// encrypts given buffer
   /// \param data data to encode
   /// \param length length of data
   void Encrypt(unsigned char* data, unsigned int length) throw()
   {
      Encrypt(data, data+length);
   }

private:
   /// inits S-box
   void Init(const unsigned char* key, size_t key_length) throw()
   {
      int i, j;
      for (i = 0; i < 256; i++)
         S[i] = static_cast<unsigned char>(i);

      for (int i = j = 0; i < 256; i++)
      {
         j = (j + key[i % key_length] + S[i]) & 255;
         std::swap(S[i], S[j]);
      }
   }

   /// calculate output
   unsigned char Output() throw()
   {
      i = (i + 1) & 255;
      j = (j + S[i]) & 255;

      std::swap(S[i], S[j]);

      return S[(S[i] + S[j]) & 255];
   }

private:
   unsigned char S[256]; ///< S-box
   unsigned int i; ///< index i
   unsigned int j; ///< index j
};

} // namespace RC4
