//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestScatteredPointInterpolator.cpp Unit tests for class ScatteredPointInterpolator
//

// includes
#include "stdafx.h"
#include "ScatteredPointInterpolator.hpp"
#include <boost/random.hpp>

#undef OUTPUT_BITMAP

#ifdef OUTPUT_BITMAP
#include "../../RenderEngine/Bitmap.hpp"
#include "../../RenderEngine/BitmapImageWriter.hpp"
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests class ScatteredPointInterpolator
   TEST_CLASS(TestScatteredPointInterpolator)
   {
      /// tests height interpolation
      TEST_METHOD(TestInterpolate)
      {
         std::vector<Vector3d> vecRefPoints;
         vecRefPoints.push_back(Vector3d(1.0, 1.0, 5.0));
         vecRefPoints.push_back(Vector3d(2.0, 2.0, -2.0));

         ScatteredPointInterpolator interpolator(0.5, vecRefPoints);
         interpolator.Init();

         double dHeight;
         dHeight = interpolator.Height(Vector2d(1.0, 1.0));
         Assert::AreEqual(dHeight, 5.0, 0.1, _T("height must be 5.0"));

         dHeight = interpolator.Height(Vector2d(2.0, 2.0));
         Assert::AreEqual(dHeight, -2.0, 0.2, _T("height must be -2.0"));

         dHeight = interpolator.Height(Vector2d(1.5, 1.5));
         dHeight = interpolator.Height(Vector2d(2.5, 2.5));
         dHeight = interpolator.Height(Vector2d(0.5, 0.5));
      }

      /// Tests interpolating with many reference points
      TEST_METHOD(TestManyPoints)
      {
         std::vector<Vector3d> vecRefPoints;
         vecRefPoints.push_back(Vector3d(10.0, 10.0, 0.1));
         vecRefPoints.push_back(Vector3d(40.0, 20.0, 0.2));
         vecRefPoints.push_back(Vector3d(20.0, 40.0, -0.1));
         vecRefPoints.push_back(Vector3d(60.0, 60.0, 0.15));
         vecRefPoints.push_back(Vector3d(10.0, 50.0, -0.3));
         vecRefPoints.push_back(Vector3d(50.0, 10.0, 0.3));

         ScatteredPointInterpolator interpolator(0.5, vecRefPoints);
         interpolator.Init();
      }

      /// less-than operator for Vector2d objects
      struct LessThanVector2d
      {
         bool operator()(const Vector2d& lhs, const Vector2d& rhs) const throw()
         {
            return fabs(lhs.X() - rhs.X()) > 1e-6 ? lhs.X() < rhs.X() : lhs.Y() < rhs.Y();
         }
      };

      /// Tests interpolating many random points
      TEST_METHOD(TestManyRandomPoints)
      {
         const unsigned int c_uiNumPoints = 64;

         boost::mt19937 rng;
         rng.seed(42);

         boost::uniform_int<> distXY(0, 100);
         boost::uniform_real<> distHeight(0.0, 1.0);

         boost::variate_generator<boost::mt19937&, boost::uniform_int<> > dieXY(rng, distXY);
         boost::variate_generator<boost::mt19937&, boost::uniform_real<> > dieHeight(rng, distHeight);

         std::vector<Vector3d> vecRefPoints;

         std::set<Vector2d, LessThanVector2d> setAllPoints;
         for (unsigned int i = 0; i < c_uiNumPoints; i++)
         {
            Vector2d vPoint(dieXY(), dieXY());
            if (setAllPoints.find(vPoint) != setAllPoints.end())
               continue;

            setAllPoints.insert(vPoint);

            vecRefPoints.push_back(Vector3d(vPoint.X(), vPoint.Y(), dieHeight()));
         }

         ScatteredPointInterpolator interpolator(5, vecRefPoints);
         interpolator.Init();

         // check all points if they match, with error
         for (size_t i = 0, iMax = vecRefPoints.size(); i < iMax; i++)
         {
            Vector2d vPos(vecRefPoints[i].X(), vecRefPoints[i].Y());
            double dHeight = interpolator.Height(vPos);
            double dError = fabs(dHeight - vecRefPoints[i].Z());

            ATLTRACE(_T("[%u] error=%3.2f"), i, dError);
         }

#ifdef OUTPUT_BITMAP
         // export to bmp
         Bitmap bmp(120, 120);

         LinearTransform<double> trans(-1.0, 16.0, 0.0, 1.0);

         for (unsigned int x = 0; x < 120; x++)
            for (unsigned int y = 0; y < 120; y++)
            {
               Vector2d vPos(x - 10.0, y - 10.0);
               double dHeight = interpolator.Height(vPos);

               dHeight = trans(dHeight);
               dHeight = Clamp(0.0, 1.0, dHeight);

               Color c = Interpolate<Color>(Color::Black(), Color::White(), dHeight);
               bmp.Set(x, y, c);
            }

         BitmapImageWriter writer(bmp);
         writer.Write(_T("d:\\temp\\interpolator.bmp"));
#endif
      }
   };

} // namespace UnitTest
