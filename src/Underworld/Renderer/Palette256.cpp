//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/Renderer/Palette256.cpp Palette
//

// includes
#include "stdafx.h"
#include "Palette256.hpp"

void Palette256::Rotate(Uint8 start, Uint8 len, bool forward)
{
   Uint8 save[4];
   if (forward)
   {
      memcpy(save, &m_pal[start], sizeof(m_pal[0]));
      memmove(&m_pal[start],&m_pal[start+1],(len-1)*sizeof(m_pal[0]));
      memcpy(m_pal[start+len-1],save,sizeof(m_pal[0]));
   }
   else
   {
      memcpy(save,&m_pal[start+len-1],sizeof(m_pal[0]));
      memmove(&m_pal[start+1],&m_pal[start],(len-1)*sizeof(m_pal[0]));
      memcpy(m_pal[start],save,sizeof(m_pal[0]));
   }
}
