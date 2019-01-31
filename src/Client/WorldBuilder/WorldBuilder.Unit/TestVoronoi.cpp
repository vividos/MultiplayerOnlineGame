//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestVoronoi.cpp Unit tests for class Voronoi
//

// includes
#include "stdafx.h"
#include "Voronoi2.hpp"
#include <ulib/unittest/AutoCleanupFolder.hpp>
#include <boost/random.hpp>
#include <ulib/Timer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
const unsigned int c_uiSizeX = 4096;
const unsigned int c_uiSizeY = 4096;

/// tests class Voronoi::Diagram
TEST_CLASS(TestVoronoi)
{
   /// tests default ctor
   TEST_METHOD(TestDefaultCtor)
   {
      std::vector<Vector2d> vecPoints;
      vecPoints.push_back(Vector2d(c_uiSizeX / 2.0, c_uiSizeY / 2.0));

      Voronoi2::Diagram voronoi(vecPoints, Vector2d(c_uiSizeX, c_uiSizeY));
   }

   /// tests OutputSvg() function
   TEST_METHOD(TestOutputSvg)
   {
      std::vector<Vector2d> vecPoints;
      vecPoints.push_back(Vector2d(c_uiSizeX / 2.0, c_uiSizeY / 2.0));

      Voronoi2::Diagram voronoi(vecPoints, Vector2d(c_uiSizeX, c_uiSizeY));

      AutoCleanupFolder folder;
      voronoi.OutputSvg(folder.FolderName() + _T("voronoi.svg"),
         true, true);
   }

   /// tests generating voronoi diagram with various numbers of points
   TEST_METHOD(TestMultiSizes)
   {
      unsigned int auiSizes[] =
      {
         //1, 3, 10, 30, 100, 300, 1000, 3000, 10000
         1, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 10000
      };

      ATLTRACE(_T("size;time"));

      for (unsigned int ui = 0; ui < sizeof(auiSizes) / sizeof(*auiSizes); ui++)
      {
         unsigned int uiSize = auiSizes[ui];

         Timer t;
         t.Start();
         {
            std::vector<Vector2d> vecPoints;

            boost::mt19937 rng;

            boost::uniform_int<> distX(10, c_uiSizeX - 10);
            boost::uniform_int<> distY(10, c_uiSizeX - 10);

            boost::variate_generator<boost::mt19937&, boost::uniform_int<> > dieX(rng, distX);
            boost::variate_generator<boost::mt19937&, boost::uniform_int<> > dieY(rng, distY);

            for (unsigned int i = 0; i < uiSize; i++)
               vecPoints.push_back(Vector2d(dieX(), dieY()));

            Voronoi2::Diagram voronoi(vecPoints, Vector2d(c_uiSizeX, c_uiSizeY));
         }
         t.Stop();

         ATLTRACE(_T("%u;%u"), uiSize, unsigned(t.TotalElapsed()*1000.0));
      }
   }
};

} // namespace UnitTest
