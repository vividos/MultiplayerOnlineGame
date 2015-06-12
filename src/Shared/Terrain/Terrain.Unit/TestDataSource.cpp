//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestDataSource.cpp Unit tests for classes derived from IDataSource
//

// includes
#include "stdafx.h"
#include "DataSource/MandelbrotTerrainDataSource.hpp"
//#include "DataSource/ImageFileTerrainDataSource.hpp"
#include "DataSource/SineCosineTerrainDataSource.hpp"
#include "Filesystem.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests classes derived from IDataSource
   TEST_CLASS(TestDataSource)
   {
      /// tests MandelbrotTerrainDataSource class
      TEST_METHOD(TestMandelbrotDataSource)
      {
         Terrain::MandelbrotTerrainDataSource ds;

         for (unsigned int x = 0; x < 512; x += 64)
            for (unsigned int y = 0; y < 512; y += 64)
            {
               std::shared_ptr<Terrain::Model::DataBlock> spDataBlock;
               spDataBlock = ds.LoadBlock(x, y, 64);
            }
      }

      /// tests TargaFileTerrainDataSource class
      TEST_METHOD(TestTargaFileDataSource)
      {
#if 0
         Terrain::ImageFileTerrainDataSource ds(Filesystem().BaseFolder() + _T("terrain\\map0.tga"));

         for (unsigned int x=0; x<512; x+=64)
            for (unsigned int y=0; y<512; y+=64)
            {
               std::shared_ptr<Terrain::Model::DataBlock> spDataBlock;
               spDataBlock = ds.LoadBlock(x, y, 64);
            }
#endif
      }

      /// tests SineCosineTerrainDataSource class
      TEST_METHOD(TestSineCosineDataSource)
      {
         Terrain::SineCosineTerrainDataSource ds;
         std::shared_ptr<Terrain::Model::DataBlock> spDataBlock = ds.LoadBlock(0, 0, 512);
      }
   };

} // namespace UnitTest
