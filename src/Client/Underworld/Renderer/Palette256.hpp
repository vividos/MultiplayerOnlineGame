//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Renderer/Palette256.hpp Palette
//
#pragma once

// includes

/// palette with 256 entries, 4 color values per entry
class Palette256
{
public:
   /// ctor
   Palette256()
   {
      memset(m_pal, 0, sizeof(m_pal));
   }

   /// set value
   void Set(Uint8 idx, Uint8 component, Uint8 val){ m_pal[idx][component] = val; }

   /// returns pointer to palette data
   const Uint8* Get() const { return &m_pal[0][0]; }

   /// rotates palette indices
   void Rotate(Uint8 start, Uint8 len, bool forward);

protected:
   /// a GL_RGBA compatible palette
   Uint8 m_pal[256][4];
};
