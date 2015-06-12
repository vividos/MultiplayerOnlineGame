//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestPath.cpp Unit tests for class Path
//

// includes
#include "stdafx.h"
#include "Path.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

/// tests class Path
TEST_CLASS(TestPath)
{
   /// tests default ctor
   TEST_METHOD(TestDefaultCtor)
   {
      Path p1, p2;
   }

   /// tests FilenameAndExt()
   TEST_METHOD(TestFilenameAndExt1)
   {
      Path p(_T("d:\\test\\bla.exe"));

      Assert::AreEqual(p.FilenameAndExt(), _T("bla.exe"));
   }

   /// tests FilenameAndExt()
   TEST_METHOD(TestFilenameAndExt2)
   {
      Path p(_T("d:\\test\\bla"));

      Assert::AreEqual(p.FilenameAndExt(), _T("bla"));
   }

   /// tests FilenameAndExt()
   TEST_METHOD(TestFilenameAndExt3)
   {
      Path p(_T("d:\\test\\bla\\"));

      Assert::AreEqual(p.FilenameAndExt(), _T(""));
   }

   /// tests FilenameOnly()
   TEST_METHOD(FilenameOnly1)
   {
      Path p(_T("d:\\test\\bla.exe"));

      Assert::AreEqual(p.FilenameOnly(), _T("bla"));
   }

   /// tests FilenameOnly()
   TEST_METHOD(FilenameOnly2)
   {
      Path p(_T("bla.exe"));

      Assert::AreEqual(p.FilenameOnly(), _T("bla"));
   }

   /// tests FilenameOnly()
   TEST_METHOD(FilenameOnly3)
   {
      Path p(_T("d:\\test\\bla"));

      Assert::AreEqual(p.FilenameOnly(), _T("bla"));
   }

   /// tests FilenameOnly()
   TEST_METHOD(FilenameOnly4)
   {
      Path p(_T("d:\\test\\bla."));

      Assert::AreEqual(p.FilenameOnly(), _T("bla"));
   }

   /// tests FilenameOnly()
   TEST_METHOD(FilenameOnly5)
   {
      Path p(_T("d:\\test\\bla.bla.exe"));

      Assert::AreEqual(p.FilenameOnly(), _T("bla.bla"));
   }
};

} // namespace UnitTest
