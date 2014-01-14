//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TexCoord2f.hpp Texture coordinates class
//
#pragma once

// includes

/// texture coordinates
class TexCoord2f
{
public:
   /// ctor
   TexCoord2f(float u = 0.0f, float v = 0.0f) throw()
   {
      m_afPos[0] = u;
      m_afPos[1] = v;
   }

   /// returns U coordinate
   float U() const throw() { return m_afPos[u]; }

   /// returns V coordinate
   float V() const throw() { return m_afPos[v]; }

   /// returns data, e.g. for a call to glTexCoord2fv()
   const float* Data() const throw() { return m_afPos; }

   /// returns data, e.g. for a call to glTexCoord2fv()
   float* Data() throw() { return m_afPos; }

private:
   /// enum to access texture pos components per name
   enum
   {
      u = 0,
      v = 1
   };
   /// texture coordinates
   float m_afPos[2];
};

// verify that TexCoord2f has exact size to be usable in arrays
static_assert(sizeof(TexCoord2f) == 2*sizeof(float), "TexCoord2f must have the size of two floats");
