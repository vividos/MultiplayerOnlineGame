//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestReduceAlgorithm.cpp Tests for class ReduceAlgorithm
//

// includes
#include "stdafx.h"
#include "Reduce/ReduceAlgorithm.hpp"
#include "Reduce/ActiveVertexMap.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Terrain::Model;
using namespace Terrain::Reduce;

namespace UnitTest
{
   /// tests ReduceAlgorithm class
   TEST_CLASS(TestReduceAlgorithm)
   {

      /// tests class ReduceAlgorithmCameraDistance
      TEST_METHOD(TestReduceAlgorithmCameraDistance)
      {
         const unsigned int c_uiBlockSize = 4;

         DataBlock data(c_uiBlockSize);
         std::vector<float>& vecData = data.GetElevationData().RawData();
         Assert::IsTrue(vecData.size() == (c_uiBlockSize + 1)*(c_uiBlockSize + 1));

         ReduceAlgorithmCameraDistance ra(data);

         ActiveVertexMap avm(data.GetElevationData().Size());
         avm.Init();

         Vector3d vCamera(0.0, 0.0, 0.0);
         ra.Reduce(avm, vCamera);
      }

      /// \see http://bits.stephan-brumme.com/lowestBitSet.html
      unsigned int CalcLargestPowerOfTwo(unsigned int n, unsigned m_uiSize)
      {
         if (n == 0)
            return m_uiSize;

         // leave only lowest bit
         //   n &= -int(n);
         //   return n;
         return n & -int(n);
      }

      /// tests if boundary algorithm is the same as CalcLargestPowerOfTwo()
      TEST_METHOD(TestCalcLargestPowerOfTwo)
      {
         for (unsigned int m_uiSize = 512; m_uiSize > 2; m_uiSize >>= 1)
         {
            std::vector<unsigned int> m_vecBoundary(m_uiSize, 0);

            //This finds the largest power-of-two denominator for the given number.  This
            //is used to determine what level of the quadtree a grid position occupies.
            for (unsigned int n = 0; n < m_uiSize; n++)
            {
               m_vecBoundary[n] = unsigned(-1);
               if (n == 0)
                  m_vecBoundary[n] = m_uiSize;
               else
               {
                  for (unsigned int level = m_uiSize; level > 1; level /= 2)
                  {
                     if (!(n % level))
                     {
                        m_vecBoundary[n] = level;
                        break;
                     }
                  }
                  if (m_vecBoundary[n] == unsigned(-1))
                     m_vecBoundary[n] = 1;
               }
            }

            for (unsigned int n = 0; n < m_uiSize; n++)
            {
               ATLTRACE(_T("n=%u, vec=%u, calc=%u"), n, m_vecBoundary[n], CalcLargestPowerOfTwo(n, m_uiSize));
               Assert::IsTrue(m_vecBoundary[n] == CalcLargestPowerOfTwo(n, m_uiSize));
            }
         }
      }
   };

} // namespace UnitTest
