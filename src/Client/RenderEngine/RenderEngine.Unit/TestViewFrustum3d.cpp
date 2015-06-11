//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestViewFrustum3d.cpp Unit tests for class ViewFrustum3d
//

// includes
#include "stdafx.h"
#include "ViewFrustum3d.hpp"
#include "Bitmap.hpp"
#include "BitmapImageWriter.hpp"
#include <ulib/test/AutoCleanupFolder.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests class ViewFrustum3d
   TEST_CLASS(TestViewFrustum3d)
   {
      /// tests IsPointInside() method
      TEST_METHOD(TestIsPointInside)
      {
         ViewFrustum3d vf(Vector3d(),
            0.0, 0.0, // angles
            60.0, 1.3333, // fov, aspect ratio
            0.01, 1000.0); // near, far dist

         Assert::IsTrue(vf.IsPointInside(Vector3d(0.0, 0.0, 0.0)) == false, _T("point must be outside"));
         Assert::IsTrue(vf.IsPointInside(Vector3d(0.0, 0.0, 1.0)) == true, _T("point must be inside"));
         Assert::IsTrue(vf.IsPointInside(Vector3d(0.0, 0.0, 999.0)) == true, _T("point must be inside"));
         Assert::IsTrue(vf.IsPointInside(Vector3d(0.0, 0.0, 1001.0)) == false, _T("point must be outside"));
         Assert::IsTrue(vf.IsPointInside(Vector3d(0.0, 0.0, -1.0)) == false, _T("point must be outside"));
      }

      /// tests writing out view frustum as bitmap
      TEST_METHOD(TestWriteBitmap)
      {
         Bitmap bmp(1024, 1024, Color::White());

         Color* pColor = bmp.Data();

         ViewFrustum3d vf(Vector3d(420.0, 0.0, 20.0),
            0.0, 0.0, // angles
            170.0, 1.3333333, // fov, aspect ratio
            10.0, 500.0); // near, far dist

         for (unsigned int x = 0; x < 1024; x++)
            for (unsigned int y = 0; y < 1024; y++)
            {
               Vector3d vPos(x, 0.0, y);
               if (vf.IsPointInside(vPos))
                  pColor[y * 1024 + x] = Color::Blue();
            }

         AutoCleanupFolder folder;

         BitmapImageWriter biw(bmp);
         biw.Write(folder.FolderName() + _T("TestViewFrustum3d.bmp"));
      }
   };

} // namespace UnitTest
