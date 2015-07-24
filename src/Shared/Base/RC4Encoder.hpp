//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RC4Encoder.hpp RC4 encoder class
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
      :m_i(0),
       m_j(0)
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
      for (unsigned char i = 0; i < 256; i++)
         S[i] = i;

      for (unsigned char i = 0, j = 0; i < 256; i++)
      {
         j = (j + key[i % key_length] + S[i]) & 255;
         std::swap(S[i], S[j]);
      }
   }

   /// calculate output
   unsigned char Output() throw()
   {
      m_i = (m_i + 1) & 255;
      m_j = (m_j + S[m_i]) & 255;

      std::swap(S[m_i], S[m_j]);

      return S[(S[m_i] + S[m_j]) & 255];
   }

private:
   unsigned char S[256]; ///< S-box
   unsigned int m_i; ///< index i
   unsigned int m_j; ///< index j
};

} // namespace RC4
