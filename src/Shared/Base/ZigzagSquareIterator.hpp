//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ZigzagSquareIterator.hpp Zigzag square iterator
//
#pragma once

/// iterator that iterates a square in a zigzag way
struct ZigzagSquareIterator
{
public:
   /// ctor
   ZigzagSquareIterator(unsigned int uiSize)
      :m_uiSize(uiSize),
       m_x(uiSize-1),
       m_y(uiSize-1)
   {
   }

   /// returns current x coordinate
   unsigned int X() const { return m_x; }
   /// returns current y coordinate
   unsigned int Y() const { return m_y; }

   /// advances to next square tile
   bool Next()
   {
      if (m_x > 0)
      {
         if (m_y < m_uiSize-1)
         {
            // inside square
            m_y++;
            m_x--;
         }
         else
         {
            if (m_x == m_uiSize-1)
            {
               // upper right corner
               m_y--;
            }
            else
            {
               // upper border
               m_y -= m_uiSize-m_x;
               m_x = m_uiSize-1;
            }
         }
      }
      else
      {
         if (m_y == 0)
         {
            // lower left corner
            return false;
         }
         else
         {
            // left border
            m_x += m_y-1;
            m_y = 0;
         }
      }

      return true;
   }

private:
   unsigned int m_uiSize;  ///< block size
   unsigned int m_x;       ///< current x coordinate
   unsigned int m_y;       ///< current y coordinate
};
