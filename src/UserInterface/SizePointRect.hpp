//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SizePointRect.hpp user interface helper classes
//
#pragma once

// includes
#include <utility> // for std::pair

/// size
class Size
{
public:
   /// ctor
   Size(int width = 0, int height = 0) throw()
      :m_size(width, height)
   {
   }

   int Width() const throw() { return m_size.first; }    ///< returns width
   int Height() const throw() { return m_size.second; }  ///< returns height

   void Width(int iWidth) throw() { m_size.first = iWidth; }      ///< sets width
   void Height(int iHeight) throw() { m_size.second = iHeight; }  ///< sets height

   /// adds offset to size
   void Add(int x, int y) throw(){ m_size.first += x; m_size.second += y; }

private:
   /// size
   std::pair<int, int> m_size;
};

/// point
class Point
{
public:
   /// ctor
   Point(int x = 0, int y = 0) throw()
      :m_pos(x, y)
   {
   }

   int X() const throw() { return m_pos.first; }   ///< returns x coordinate
   int Y() const throw() { return m_pos.second; }  ///< returns y coordinate

   void X(int iX) throw() { m_pos.first = iX; }    ///< sets x coordinate
   void Y(int iY) throw() { m_pos.second = iY; }   ///< sets y coordinate

   /// adds offset to point
   void Add(int x, int y) throw(){ m_pos.first += x; m_pos.second += y; }

private:
   /// point position
   std::pair<int, int> m_pos;
};

/// rectangular area
class Rect
{
public:
   /// ctor
   Rect(int left = 0, int top = 0, int right = 0, int bottom = 0) throw()
   {
      m_aiCoords[left] = left;
      m_aiCoords[top] = top;
      m_aiCoords[right] = right;
      m_aiCoords[bottom] = bottom;
   }

   /// ctor; init with point and size
   Rect(Point pos, Size size) throw()
   {
      m_aiCoords[left] = pos.X();
      m_aiCoords[top] = pos.Y();
      m_aiCoords[right] = m_aiCoords[left] + size.Width();
      m_aiCoords[bottom] = m_aiCoords[top] + size.Height();
   }

   // get methods

   int Left() const throw() { return m_aiCoords[left]; }    ///< returns left coord
   int Top() const throw() { return m_aiCoords[top]; }      ///< returns top coord
   int Right() const throw() { return m_aiCoords[right]; }  ///< returns right coord
   int Bottom() const throw() { return m_aiCoords[bottom]; }///< returns bottom coord

   int Width() const throw() { return m_aiCoords[right] - m_aiCoords[left]; } ///< returns width
   int Height() const throw() { return m_aiCoords[bottom] - m_aiCoords[top]; }///< returns height

   Size GetSize() const throw() { return Size(Width(), Height()); }  ///< returns size

   // set methods

   void Left(int iLeft) throw() { m_aiCoords[left] = iLeft; }       ///< sets left coord
   void Top(int iTop) throw() { m_aiCoords[top] = iTop; }           ///< sets top coord
   void Right(int iRight) throw() { m_aiCoords[right] = iRight; }   ///< sets right coord
   void Bottom(int iBottom) throw() { m_aiCoords[bottom] = iBottom; }///< sets bottom coord

   // operations

   /// adds offset
   void Add(const Point& p) throw()
   {
      m_aiCoords[left] += p.X();
      m_aiCoords[right] += p.X();
      m_aiCoords[top] += p.Y();
      m_aiCoords[bottom] += p.Y();
   }

   /// checks if given point is inside rect
   bool IsInside(const Point& p) const throw()
   {
      int x = p.X(), y = p.Y();
      return x >= m_aiCoords[left] && x < m_aiCoords[right] && 
         y >= m_aiCoords[top] && y < m_aiCoords[bottom];
   }

private:
   /// enum to access rect coordinates
   enum
   {
      left = 0,
      top = 1,
      right = 2,
      bottom = 3
   };

   /// rect coordinates
   int m_aiCoords[4];
};
