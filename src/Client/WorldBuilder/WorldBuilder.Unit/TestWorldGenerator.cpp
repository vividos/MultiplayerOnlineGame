//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestWorldGenerator.cpp Unit tests for class WorldGenerator
//

// includes
#include "stdafx.h"
#include "WorldGenerator.hpp"
#include <ulib/test/AutoCleanupFolder.hpp>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
/// seed for world generator tests
const unsigned int c_uiSeed = 42;

/// tests class WorldGenerator
TEST_CLASS(TestWorldGenerator)
{
   void TraceStatus(const CString& cszStatus)
   {
      if (!cszStatus.IsEmpty())
         ATLTRACE(cszStatus);
   }

   /// tests default ctor
   TEST_METHOD(TestDefaultCtor)
   {
      WorldGenerator wg(2048);
   }

   /// tests generating 2048x2048 world
   TEST_METHOD(TestSize2048)
   {
      WorldGenerator wg(2048);
      wg.SetUpdateCallback(std::bind(&TestWorldGenerator::TraceStatus, this, std::placeholders::_1));
      wg.Generate(WorldGenerator::islandPerlinNoise, c_uiSeed);

      // write out SVG for test
      AutoCleanupFolder folder;

      wg.GetGraph().OutputSvg(folder.FolderName() + _T("polygraph-2048.svg"));
   }

   /// tests generating 4096x4096 world
   TEST_METHOD(TestSize4096)
   {
      WorldGenerator wg(4096);
      wg.SetUpdateCallback(std::bind(&TestWorldGenerator::TraceStatus, this, std::placeholders::_1));
      wg.Generate(WorldGenerator::islandPerlinNoise, c_uiSeed);

      AutoCleanupFolder folder;
      wg.GetGraph().OutputSvg(folder.FolderName() + _T("polygraph-4096.svg"));
   }

   /// tests generating 8192x8192 world
   TEST_METHOD(TestSize8192)
   {
      WorldGenerator wg(8192);
      wg.SetUpdateCallback(std::bind(&TestWorldGenerator::TraceStatus, this, std::placeholders::_1));
      wg.Generate(WorldGenerator::islandPerlinNoise, c_uiSeed);

      AutoCleanupFolder folder;
      wg.GetGraph().OutputSvg(folder.FolderName() + _T("polygraph-8192.svg"));
   }

   /// tests generating radial island
   TEST_METHOD(TestRadialIsland)
   {
      WorldGenerator wg(2048);
      wg.SetUpdateCallback(std::bind(&TestWorldGenerator::TraceStatus, this, std::placeholders::_1));
      wg.Generate(WorldGenerator::islandRadialRandom, c_uiSeed);
   }

   /// tests generating square island
   TEST_METHOD(TestSquareIsland)
   {
      WorldGenerator wg(2048);
      wg.SetUpdateCallback(std::bind(&TestWorldGenerator::TraceStatus, this, std::placeholders::_1));
      wg.Generate(WorldGenerator::islandSquare, c_uiSeed);
   }
};

} // namespace UnitTest
