//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IActiveVertexMap.hpp Active vertex map interface
//
#pragma once

// includes
#include "Math.hpp"

namespace Terrain
{
namespace Reduce
{

/// \brief interface to a map that manages active vertex state
class IActiveVertexMap
{
public:
   /// dtor
   virtual ~IActiveVertexMap() {}

   /// returns if a given point is inside the map
   virtual bool IsInMap(unsigned int x, unsigned int y) const = 0;

   /// returns if vertex at given point is active
   virtual bool IsActive(unsigned int x, unsigned int y) const = 0;

   /// activates vertex at given point
   virtual void SetActive(unsigned int x, unsigned int y) = 0;
};

/// base class for IActiveVertexMap implementations
class ActiveVertexMapBase: public IActiveVertexMap
{
public:
   /// ctor
   ActiveVertexMapBase(unsigned int uiSize)
      :m_uiSize(uiSize)
   {
      ATLASSERT(IsPowerOfTwo(uiSize));
   }

   /// dtor
   virtual ~ActiveVertexMapBase() {}

   /// returns size of map
   unsigned int Size() const { return m_uiSize; }

   /// inits vertex map
   void Init();

   /// activates an edge point
   void ActivatePoint(unsigned int x, unsigned int y);

   /// returns boundary value
   /// \see http://bits.stephan-brumme.com/lowestBitSet.html
   unsigned int GetBoundary(unsigned int n) const
   {
      return n == 0 ? m_uiSize : (n & -int(n));
   }

private:
   /// square map size
   unsigned int m_uiSize;
};

} // namespace Reduce
} // namespace Terrain
