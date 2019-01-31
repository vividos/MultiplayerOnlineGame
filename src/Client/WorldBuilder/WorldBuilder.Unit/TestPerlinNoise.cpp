//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestPerlinNoise.cpp Unit tests for class PerlinNoise2D
//

// includes
#include "stdafx.h"
#include "PerlinNoise.hpp"
#include "Bitmap.hpp"
#include "BitmapImageWriter.hpp"
#include <ulib/unittest/AutoCleanupFolder.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

/// tests class PerlinNoise2D
TEST_CLASS(TestPerlinNoise)
{
   /// tests PerlinNoise2D class
   TEST_METHOD(TestPerlinNoise2D)
   {
      PerlinNoise2D pn(42, 4, 0.25);

      Bitmap bmp(256, 256);

      for (unsigned int x=0; x<256; x++)
      for (unsigned int y=0; y<256; y++)
      {
         //double dx = (x - 128.0) / 128.0;
         //double dy = (y - 128.0) / 128.0;
         double dx = x / 4.0, dy = y / 4.0;

         double n = (pn.Get(dx, dy) / 2.0) + 0.5;
         if (n > 1.0)
            n = 1.0;
         if (n < 0.0)
            n = 0.0;
         //ATLTRACE(_T("n = %2.3f\n"), n);
         bmp.Set(x, y, Interpolate(Color::White(), Color::Black(), n));
      }

      BitmapImageWriter biw(bmp);

      AutoCleanupFolder folder;
      biw.Write(folder.FolderName() + _T("Perlin.bmp"));
   }

   /// tests ImprovedNoise class
   TEST_METHOD(TestImprovedNoise)
   {
      ImprovedNoise pn;

      Bitmap bmp(256, 256);

      LinearTransform<double> ltxy(0.0, 64.0, -1.0, 1.0);
      LinearTransform<double> ltn(-1.0, 1.0, 0.0, 1.0);

      for (unsigned int i=0; i<20; i++)
      {
         for (unsigned int x=0; x<256; x++)
         for (unsigned int y=0; y<256; y++)
         {
            // map x and y to -1.0, 1.0
            double dx = ltxy(x);
            double dy = ltxy(y);

            double z = i / 5.0;
            double n = pn.noise(dx, dy, z);

            // map n from [-1.0, 1.0] to [0.0, 1.0]
            n = ltn(n);

            if (n > 1.0)
               n = 1.0;
            if (n < 0.0)
               n = 0.0;
            bmp.Set(x, y, Interpolate(Color::White(), Color::Black(), n));
         }

         BitmapImageWriter biw(bmp);
         AutoCleanupFolder folder;

         CString cszFilename;
         cszFilename.Format(_T("%sImprovedPerlin-%u.bmp"), folder.FolderName().GetString(), i);

         biw.Write(cszFilename);
      }
   }
};

} // namespace UnitTest
