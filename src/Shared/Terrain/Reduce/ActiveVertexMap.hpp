//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ActiveVertexMap.hpp active vertex map for reduce algorithm
//
#pragma once

// includes
#include "IActiveVertexMap.hpp"
#include <vector>
#include "Math.hpp"
#include <boost/dynamic_bitset.hpp>

namespace Terrain
{
namespace Reduce
{

/// \brief map for marking vertices active
class ActiveVertexMap: public ActiveVertexMapBase
{
public:
   /// ctor
   ActiveVertexMap(unsigned int uiMapSize)
      :ActiveVertexMapBase(uiMapSize),
       m_activeVertexMap(uiMapSize*uiMapSize)
   {
      ATLASSERT(IsPowerOfTwo(uiMapSize));
   }

   /// dtor
   virtual ~ActiveVertexMap() {}

   virtual bool IsInMap(unsigned int x, unsigned int y) const override
   {
      return x < Size() && y < Size();
   }

   virtual bool IsActive(unsigned int x, unsigned int y) const override
   {
      if (x >= Size() || y >= Size())
         return true;

      return m_activeVertexMap.test(MapCoordinates(x,y));
   }

   virtual void SetActive(unsigned int x, unsigned int y) override
   {
      ATLASSERT(x < Size());
      ATLASSERT(y < Size());

      m_activeVertexMap.set(MapCoordinates(x,y));
   }

private:
   /// maps coordinate to array index
   size_t MapCoordinates(unsigned int x, unsigned int y) const
   {
      ATLASSERT(x < Size());
      ATLASSERT(y < Size());

      return (y * Size()) | x;
   }

private:
   friend class ActiveVertexBlockMap;

   /// active vertex map
   boost::dynamic_bitset<> m_activeVertexMap;
};


/// \brief active vertex map with edge infos of adjacent blocks
/// \details holds adjacent ActiveVertexMap instances
class ActiveVertexMapWithEdgeInfo: public ActiveVertexMapBase
{
public:
   /// ctor
   ActiveVertexMapWithEdgeInfo(unsigned int uiMapSize,
         std::shared_ptr<ActiveVertexMap> spVertexMap,
         std::shared_ptr<ActiveVertexMap> spVertexMapAbove,
         std::shared_ptr<ActiveVertexMap> spVertexMapRight)
      :ActiveVertexMapBase(uiMapSize),
       m_spVertexMap(spVertexMap),
       m_spVertexMapAbove(spVertexMapAbove),
       m_spVertexMapRight(spVertexMapRight)
   {
      ATLASSERT(spVertexMap != NULL);
      // spVertexMapAbove and spVertexMapRight may be NULL
   }

   /// dtor
   virtual ~ActiveVertexMapWithEdgeInfo() {}

   virtual bool IsInMap(unsigned int x, unsigned int y) const override
   {
/*
      // TODO temporary
      if (x > Size() && y > Size())
         return false;
      return true;
*/
      // allowed quadrants:
      // .--.
      // |Q2|Q4   y      Q2 = m_spVertexMapAbove
      // +--+--.  ^      Q3 = m_spVertexMapRight
      // |Q1|Q3|  |
      // '--+--'  +--> x

      if (x >= 2*Size() || y >= 2*Size())
         return false; // at or beyond border of Q2 or Q3

      if (x >= Size() && y >= Size()) // in Q4
         return false;

      // check empty Q2
      if (m_spVertexMapAbove == NULL &&
          x < Size() &&
          y >= Size())
         return false;

      // check empty Q3
      if (m_spVertexMapRight == NULL &&
          x >= Size() &&
          y < Size())
         return false;

      return true; // in map
   }

   virtual bool IsActive(unsigned int x, unsigned int y) const override
   {
      ATLASSERT(x <= 2*Size() && y <= 2*Size());

      if (x == 2*Size() || y == 2*Size())
         return true; // at border of above or right block

      if (x > Size() && y > Size())
         return true; // Q4: above and right of our block

      std::shared_ptr<const ActiveVertexMap> spVertexMap = MapVertexMap(x,y);

      if (spVertexMap == NULL)
         return true; // uninitialized block

      // remap to single block
      x %= Size();
      y %= Size();

      return spVertexMap->IsActive(x,y);
   }

   virtual void SetActive(unsigned int x, unsigned int y) override
   {
      if (x > Size() && y > Size())
         return; // above and right of our block; do nothing

      std::shared_ptr<ActiveVertexMap> spVertexMap = MapVertexMap(x,y);
      ATLASSERT(spVertexMap != NULL);

      // remap to single block
      x %= Size();
      y %= Size();

      spVertexMap->SetActive(x, y);
   }

   /// sets new center map
   void SetNewMap(std::shared_ptr<ActiveVertexMap> spVertexMap)
   {
      m_spVertexMap = spVertexMap; // atomic op
   }

private:
   /// const version of MapVertexMap(); uses const_cast (ugly, but saves code)
   std::shared_ptr<const ActiveVertexMap> MapVertexMap(unsigned int x, unsigned int y) const
   {
      return std::shared_ptr<const ActiveVertexMap>(
         const_cast<ActiveVertexMapWithEdgeInfo&>(*this).MapVertexMap(x,y));
   }

   std::shared_ptr<ActiveVertexMap> MapVertexMap(unsigned int x, unsigned int y)
   {
      ATLASSERT(x < 2*Size());
      ATLASSERT(y < 2*Size());

      // x and y must not be in second block simultaneously
      ATLASSERT((x / Size()) + (y / Size()) < 3);

      std::shared_ptr<ActiveVertexMap> spVertexMap = m_spVertexMap;

      if (x >= Size() || y >= Size())
      {
         if (x >= Size())
         {
            // return "right" of our block
            spVertexMap = m_spVertexMapRight;
         }
         else
         {
            // return "above" our block
            spVertexMap = m_spVertexMapAbove;
         }
      }

      return spVertexMap;
   }

private:
   std::shared_ptr<ActiveVertexMap> m_spVertexMap;
   std::shared_ptr<ActiveVertexMap> m_spVertexMapAbove;
   std::shared_ptr<ActiveVertexMap> m_spVertexMapRight;
};

} // namespace Reduce
} // namespace Terrain
