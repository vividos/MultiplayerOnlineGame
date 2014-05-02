//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IActiveVertexMap.cpp Active vertex map interface
//

// includes
#include "StdAfx.h"
#include "IActiveVertexMap.hpp"

using Terrain::Reduce::ActiveVertexMapBase;

void ActiveVertexMapBase::Init()
{
   ActivatePoint(m_uiSize / 2, m_uiSize / 2);
   ActivatePoint(0,            0);
}

/// \brief activates point in vertex map
// \details
/// This is tricky stuff.  When this is called, it means the given point is needed
/// for the terrain we are working on.  Each point, when activated, will recusivly
/// require two other points at the next lowest level of detail.  This is what
/// causes the "shattering" effect that breaks the terrain into triangles.
/// If you want to know more, Google for Peter Lindstrom, the inventor of this
/// very clever system.
void ActiveVertexMapBase::ActivatePoint(unsigned int x, unsigned int y)
{
   if (!IsInMap(x, y) || IsActive(x, y))
      return;

   SetActive(x, y);

   if (x >= m_uiSize || y >= m_uiSize)
       return; // TODO

   int xl = GetBoundary(x);
   int yl = GetBoundary(y);
   int level = std::min(xl, yl);

   if (xl > yl)
   {
      if (x - level >= 0)
         ActivatePoint(x - level, y);
      ActivatePoint(x + level, y);
   }
   else if (xl < yl)
   {
      ActivatePoint(x, y + level);
      if (y - level > 0)
         ActivatePoint(x, y - level);
   }
   else
   {
      int x2 = x & (level * 2);
      int y2 = y & (level * 2);

      if (x2 == y2)
      {
         if (x - level >= 0)
            ActivatePoint(x - level, y + level);
         if (y - level >= 0)
            ActivatePoint(x + level, y - level);
      }
      else
      {
         ActivatePoint(x + level, y + level);
         if (x - level >= 0 && y - level >= 0)
            ActivatePoint(x - level, y - level);
      }
   }
}
