//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ActiveVertexBlockMap.hpp active vertex block map
//
#pragma once

// includes
#include "ActiveVertexMap.hpp"

namespace Terrain
{
namespace Reduce
{

/// map for marking blocks of ActiveVertexMap objects
class ActiveVertexBlockMap: public IActiveVertexMap
{
public:
   /// ctor
   ActiveVertexBlockMap(unsigned int uiNumBlocks, unsigned int uiMapSize)
      :m_vecAllBlocks(uiNumBlocks*uiNumBlocks),
       m_uiNumBlocks(uiNumBlocks),
       m_uiMapSize(uiMapSize)
   {
      ATLASSERT(IsPowerOfTwo(uiMapSize));
   }
   /// dtor
   virtual ~ActiveVertexBlockMap() {}

   virtual bool IsInMap(unsigned int x, unsigned int y) const override
   {
      return x < m_uiMapSize*m_uiNumBlocks && y < m_uiMapSize*m_uiNumBlocks;
   }

   virtual bool IsActive(unsigned int x, unsigned int y) const override
   {
      if (x >= m_uiMapSize*m_uiNumBlocks || y >= m_uiMapSize*m_uiNumBlocks)
         return true;

      // remap x and y, and get block index
      size_t iIndex = MapCoordinates(x, y);

      ATLASSERT(iIndex < m_vecAllBlocks.size());
      if (m_vecAllBlocks[iIndex] == NULL)
         return true; // block not set yet: pretend to be active

      // ask block; x and y are remapped to block coords now
      return m_vecAllBlocks[iIndex]->IsActive(x,y);
   }

   virtual void SetActive(unsigned int x, unsigned int y) override
   {
      size_t iIndex = MapCoordinates(x, y);

      ATLASSERT(m_vecAllBlocks[iIndex] != NULL);

      m_vecAllBlocks[iIndex]->SetActive(x, y);
   }

   /// updates a single block in the block map
   void UpdateBlock(unsigned int xblock, unsigned int yblock, std::shared_ptr<ActiveVertexMap> spNewBlock)
   {
      ATLASSERT(xblock < m_uiNumBlocks);
      ATLASSERT(yblock < m_uiNumBlocks);
      ATLASSERT(spNewBlock->Size() == m_uiMapSize);

      size_t uiBlockNum = MapBlockNum(xblock, yblock);
      ATLASSERT(uiBlockNum < m_vecAllBlocks.size());

      std::swap(m_vecAllBlocks[uiBlockNum], spNewBlock); // atomic op
   }

   /// returns block in block map
   std::shared_ptr<ActiveVertexMap> GetBlock(unsigned int xblock, unsigned int yblock)
   {
      ATLASSERT(xblock < m_uiNumBlocks);
      ATLASSERT(yblock < m_uiNumBlocks);

      size_t uiBlockNum = MapBlockNum(xblock, yblock);
      ATLASSERT(uiBlockNum < m_vecAllBlocks.size());

      return m_vecAllBlocks[uiBlockNum];
   }

   ActiveVertexMapWithEdgeInfo GetBlockWithEdgeInfo(unsigned int xblock, unsigned int yblock)
   {
      ATLASSERT(xblock < m_uiNumBlocks);
      ATLASSERT(yblock < m_uiNumBlocks);

      std::shared_ptr<ActiveVertexMap> spVertexMap = GetBlock(xblock, yblock);
      std::shared_ptr<ActiveVertexMap> spVertexMapAbove;
      std::shared_ptr<ActiveVertexMap> spVertexMapRight;

      // get "above" edge info
      if (yblock+1 < m_uiNumBlocks)
         spVertexMapAbove = GetBlock(xblock, yblock+1);

      // get "right" edge info
      if (xblock+1 < m_uiNumBlocks)
         spVertexMapRight = GetBlock(xblock+1, yblock);

      return ActiveVertexMapWithEdgeInfo(m_uiMapSize,
         spVertexMap, spVertexMapAbove, spVertexMapRight);
   }

private:
   size_t MapCoordinates(unsigned int& x, unsigned int& y) const
   {
      size_t uiBlockNum = (y / m_uiMapSize) * m_uiNumBlocks | (x / m_uiMapSize);
      x %= m_uiMapSize;
      y %= m_uiMapSize;

      return uiBlockNum;
   }

   size_t MapBlockNum(unsigned int xblock, unsigned int yblock)
   {
      ATLASSERT(xblock < m_uiNumBlocks);
      ATLASSERT(yblock < m_uiNumBlocks);

      return yblock * m_uiNumBlocks | xblock;
   }

private:
   std::vector<std::shared_ptr<ActiveVertexMap>> m_vecAllBlocks;
   unsigned int m_uiNumBlocks;
   unsigned int m_uiMapSize;
};

} // namespace Reduce
} // namespace Terrain
