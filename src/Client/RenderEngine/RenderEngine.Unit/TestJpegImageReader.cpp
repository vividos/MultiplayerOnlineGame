//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestJpegImageReader.cpp Unit tests for class JpegImageReader
//

// includes
#include "stdafx.h"
#include "JpegImageReader.hpp"
#include <ulib/stream/FileStream.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using Stream::FileStream;

namespace UnitTest
{
   /// tests class JpegImageReader
   TEST_CLASS(TestJpegImageReader)
   {
   public:
      // tests cannot be auto-run, since they depend on local files
      BEGIN_TEST_METHOD_ATTRIBUTE(TestReadJpegFile)
         TEST_IGNORE()
      END_TEST_METHOD_ATTRIBUTE()

      /// tests jpeg image reader
      TEST_METHOD(TestReadJpegFile)
      {
         JpegImageReader reader;

         FileStream fs(
            _T("D:\\projekte\\MultiplayerOnlineGame\\data\\models\\textures\\axe01.jpg"),
            FileStream::modeOpen, FileStream::accessRead, FileStream::shareRead);

         try
         {
            reader.Load(fs);
         }
         catch (const Exception&)
         {
            Assert::Fail(_T("must not throw an exception"));
         }
      }
   };

} // namespace UnitTest
