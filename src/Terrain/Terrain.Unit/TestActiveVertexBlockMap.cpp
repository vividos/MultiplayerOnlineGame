//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestActiveVertexBlockMap.cpp Unit tests for ActiveVertexBlockMap class
//

// includes
#include "stdafx.h"
#include "Reduce/ActiveVertexBlockMap.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Terrain::Reduce::ActiveVertexMap;
using Terrain::Reduce::ActiveVertexBlockMap;
using Terrain::Reduce::ActiveVertexMapWithEdgeInfo;

namespace UnitTest
{

/// tests ActiveVertexBlockMap class
TEST_CLASS(TestActiveVertexBlockMap)
{
   /// initializes all blocks in block map
   static void InitAllBlocks(ActiveVertexBlockMap& blockMap, unsigned int uiSize, unsigned int uiMapSize)
   {
      for (unsigned int x=0; x<uiSize; x++)
         for (unsigned int y=0; y<uiSize; y++)
            blockMap.UpdateBlock(x, y,
               std::shared_ptr<ActiveVertexMap>(new ActiveVertexMap(uiMapSize)));
   }

   /// tests default ctor
   TEST_METHOD(TestDefaultCtor)
   {
      ActiveVertexBlockMap m1(8, 64), m2(4, 16);
   }

   /// tests IsInMap() method
   TEST_METHOD(TestIsInMap)
   {
      ActiveVertexBlockMap m(2, 4);
      InitAllBlocks(m, 2, 4);

      Assert::IsTrue(true == m.IsInMap(0,0));
      Assert::IsTrue(true == m.IsInMap(2,3));
      Assert::IsTrue(true == m.IsInMap(5,7));

      Assert::IsTrue(false == m.IsInMap(0,9));
      Assert::IsTrue(false == m.IsInMap(9,0));
      Assert::IsTrue(false == m.IsInMap(64,42));
   }

   /// tests IsActive() method
   TEST_METHOD(TestIsActive)
   {
      ActiveVertexBlockMap m(2, 4);
      InitAllBlocks(m, 2, 4);

      Assert::IsTrue(false == m.IsActive(2,3));
      Assert::IsTrue(true == m.IsActive(0,9));
      Assert::IsTrue(true == m.IsActive(9,0));
   }

   /// tests IsActive() method, with non-initialized block
   TEST_METHOD(TestIsActive2)
   {
      ActiveVertexBlockMap m(2, 4);
      m.UpdateBlock(0, 0, std::shared_ptr<ActiveVertexMap>(new ActiveVertexMap(4)));
      m.UpdateBlock(1, 1, std::shared_ptr<ActiveVertexMap>(new ActiveVertexMap(4)));

      Assert::IsTrue(false == m.IsActive(2,3));
      Assert::IsTrue(true == m.IsActive(3,7));
      Assert::IsTrue(true == m.IsActive(7,3));
      Assert::IsTrue(false == m.IsActive(7,7));
   }

   /// tests SetActive() method
   TEST_METHOD(TestSetActive)
   {
      ActiveVertexBlockMap m(2, 4);
      InitAllBlocks(m, 2, 4);

      m.SetActive(2,3);
      Assert::IsTrue(true == m.IsActive(2,3));
      Assert::IsTrue(false == m.IsActive(5,7));
   }

   /// tests copy ctor
   TEST_METHOD(TestCopyCtor)
   {
      ActiveVertexBlockMap m(2, 4);
      InitAllBlocks(m, 2, 4);

      m.SetActive(2,3);
      Assert::IsTrue(true == m.IsActive(2,3));

      ActiveVertexBlockMap m2(m); // copy

      Assert::IsTrue(true == m2.IsActive(2,3));
      Assert::IsTrue(false == m2.IsActive(5,7));
   }

   /// tests GetBlock() method
   TEST_METHOD(TestGetBlock)
   {
      ActiveVertexBlockMap m(2, 4);
      InitAllBlocks(m, 2, 4);

      m.SetActive(5,7);
      Assert::IsTrue(true == m.IsActive(5,7));

      ActiveVertexMap& avm = *m.GetBlock(1,1);
      Assert::IsTrue(true == avm.IsActive(5-4, 7-4));
   }

   /// tests GetBlockWithEdgeInfo()
   TEST_METHOD(TestGetBlockWithEdgeInfo1)
   {
      ActiveVertexBlockMap m(2, 4);
      InitAllBlocks(m, 2, 4);

      m.SetActive(4,2);
      Assert::IsTrue(true == m.IsActive(4,2));

      ActiveVertexMapWithEdgeInfo avm =
         m.GetBlockWithEdgeInfo(1, 0);

      Assert::IsTrue(true == avm.IsActive(0,2));
   }

   /// tests GetBlockWithEdgeInfo()
   TEST_METHOD(TestGetBlockWithEdgeInfo2)
   {
      ActiveVertexBlockMap m(2, 4);
      InitAllBlocks(m, 2, 4);

      ActiveVertexMapWithEdgeInfo avm =
         m.GetBlockWithEdgeInfo(0, 1);

      avm.SetActive(2,0);
      Assert::IsTrue(true == avm.IsActive(2,0));

      Assert::IsTrue(true == m.IsActive(2,4));
   }

   /// tests GetBlockWithEdgeInfo()
   TEST_METHOD(TestGetBlockWithEdgeInfo3)
   {
      ActiveVertexBlockMap m(2, 4);
      InitAllBlocks(m, 2, 4);

      ActiveVertexMapWithEdgeInfo avm =
         m.GetBlockWithEdgeInfo(0, 0);

      // set on the edges
      avm.SetActive(4,0);
      avm.SetActive(0,4);

      Assert::IsTrue(true == avm.IsActive(4,0));

      Assert::IsTrue(true == m.IsActive(0,4));
   }

   /// tests GetBlockWithEdgeInfo()
   TEST_METHOD(TestGetBlockWithEdgeInfo4)
   {
      ActiveVertexBlockMap m(2, 4);
      InitAllBlocks(m, 2, 4);

      ActiveVertexMapWithEdgeInfo avm =
         m.GetBlockWithEdgeInfo(0, 0);

      avm.SetActive(5,5); // doesn't set anything, since it's not in the block or on the edge
      Assert::IsTrue(true == avm.IsActive(5,5));

      Assert::IsTrue(false == m.IsActive(5,5));
   }

   /// tests GetBlockWithEdgeInfo()
   TEST_METHOD(TestGetBlockWithEdgeInfo5)
   {
      ActiveVertexBlockMap m(2, 4);
      InitAllBlocks(m, 2, 4);

      ActiveVertexMapWithEdgeInfo avm =
         m.GetBlockWithEdgeInfo(0, 0);

      Assert::IsTrue(true == avm.IsInMap(0,3));
      Assert::IsTrue(true == avm.IsInMap(3,0));
      Assert::IsTrue(true == avm.IsInMap(2,1));

      Assert::IsTrue(false == m.IsActive(4,4));
      Assert::IsTrue(false == m.IsActive(6,4));
      Assert::IsTrue(false == m.IsActive(4,6));
      Assert::IsTrue(false == m.IsActive(6,5));
   }
};

} // namespace UnitTest
