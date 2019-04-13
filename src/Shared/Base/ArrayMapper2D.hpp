//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ArrayMapper2D.hpp Array to linear vector mapper, for 2D
//
#pragma once

/// \brief class to map a 2D array to a linear vector
/// \details mapping is such that all values of one y comes first, then
/// the next line, and so forth.
class ArrayMapper2D
{
public:
   /// ctor
   ArrayMapper2D(unsigned int uiSizeX, unsigned int uiSizeY)
      :m_uiSizeX(uiSizeX),
       m_uiSizeY(uiSizeY)
   {
      ATLASSERT(uiSizeX > 0);
      ATLASSERT(uiSizeY > 0);
   }

   /// maps index to coordinates
   void IndexToCoord(size_t uiIndex, unsigned int& uiX, unsigned int& uiY) const
   {
      ATLASSERT(uiIndex < m_uiSizeX * m_uiSizeY);

      uiX = uiIndex % m_uiSizeX;
      uiY = uiIndex / m_uiSizeX;
   }

   /// maps coordinates to index
   size_t CoordToIndex(unsigned int uiX, unsigned int uiY) const
   {
      ATLASSERT(IsInRange(uiX, uiY));

      return uiY * m_uiSizeX + uiX;
   }

   /// check if coordinates are in array range
   bool IsInRange(unsigned int uiX, unsigned int uiY) const
   {
      return uiX < m_uiSizeX && uiY < m_uiSizeY;
   }

private:
   unsigned int m_uiSizeX; ///< width
   unsigned int m_uiSizeY; ///< height
};
