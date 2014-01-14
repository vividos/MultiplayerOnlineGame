//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestZigzagSquareIterator.cpp Tests for class ZigzagSquareIterator
//

// includes
#include "stdafx.h"
#include "ZigzagSquareIterator.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

/// tests class ZigzagSquareIterator
TEST_CLASS(TestZigzagSquareIterator)
{
public:
   /// tests square with side length 1
   TEST_METHOD(TestSquare1)
   {
      ZigzagSquareIterator i(1);
      Assert::IsTrue(i.X() == 0);
      Assert::IsTrue(i.Y() == 0);
      Assert::IsTrue(i.Next() == false);
   }

   /// tests square with side length 2
   TEST_METHOD(TestSquare2)
   {
      ZigzagSquareIterator i(2);
      Assert::IsTrue(i.X() == 1);
      Assert::IsTrue(i.Y() == 1);
      Assert::IsTrue(i.Next() == true);

      Assert::IsTrue(i.X() == 1);
      Assert::IsTrue(i.Y() == 0);
      Assert::IsTrue(i.Next() == true);

      Assert::IsTrue(i.X() == 0);
      Assert::IsTrue(i.Y() == 1);
      Assert::IsTrue(i.Next() == true);

      Assert::IsTrue(i.X() == 0);
      Assert::IsTrue(i.Y() == 0);
      Assert::IsTrue(i.Next() == false);
   }

   /// tests square with side length 3
   TEST_METHOD(TestSquare3)
   {
      unsigned int ax[9] = { 2,2,1,2,1,0,1,0,0 };
      unsigned int ay[9] = { 2,1,2,0,1,2,0,1,0 };

      ZigzagSquareIterator iter(3);

      for (int i=0; i<9; i++)
      {
         Assert::IsTrue(iter.X() == ax[i]);
         Assert::IsTrue(iter.Y() == ay[i]);

         bool bNext = i < 8;
         Assert::IsTrue(iter.Next() == bNext);
      }
   }

   /// tests square with side length 4
   TEST_METHOD(TestSquare4)
   {
      unsigned int ax[16] = { 3,3,2,3,2,1,3,2,1,0,2,1,0,1,0,0 };
      unsigned int ay[16] = { 3,2,3,1,2,3,0,1,2,3,0,1,2,0,1,0 };

      ZigzagSquareIterator iter(4);

      for (int i=0; i<16; i++)
      {
         Assert::IsTrue(iter.X() == ax[i]);
         Assert::IsTrue(iter.Y() == ay[i]);

         bool bNext = i < 15;
         Assert::IsTrue(iter.Next() == bNext);
      }
   }

   /// tests square with side length 8
   TEST_METHOD(TestSquare8)
   {
      ZigzagSquareIterator iter(8);

      unsigned int i=1;
      while(iter.Next())
         i++;

      Assert::IsTrue(i == 64);
   }
};

} // namespace UnitTest
