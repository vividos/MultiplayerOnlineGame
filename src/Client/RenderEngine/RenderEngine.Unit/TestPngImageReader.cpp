//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestPngImageReader.cpp Unit tests for class PngImageReader
//

// includes
#include "stdafx.h"
#include "PngImageReader.hpp"
#include <ulib/stream/FileStream.hpp>
#include "FileFinder.hpp"
#include "Path.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using Stream::FileStream;

namespace UnitTest
{
   /// tests class PngImageReader
   TEST_CLASS(TestPngImageReader)
   {
   public:
      // tests cannot be auto-run, since they depend on local files
      BEGIN_TEST_METHOD_ATTRIBUTE(TestReadPngFile1)
         TEST_IGNORE()
      END_TEST_METHOD_ATTRIBUTE()

      BEGIN_TEST_METHOD_ATTRIBUTE(TestReadPngFile2)
         TEST_IGNORE()
      END_TEST_METHOD_ATTRIBUTE()

      BEGIN_TEST_METHOD_ATTRIBUTE(TestPngSuite)
         TEST_IGNORE()
      END_TEST_METHOD_ATTRIBUTE()

      /// tests PNG image reader
      TEST_METHOD(TestReadPngFile1)
      {
         PngImageReader reader;

         FileStream fs(
            _T("D:\\projekte\\MultiplayerOnlineGame\\stuff\\PNG-Gradient.png"),
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

      /// tests PNG image reader
      TEST_METHOD(TestReadPngFile2)
      {
         PngImageReader reader;

         FileStream fs(
            _T("D:\\projekte\\MultiplayerOnlineGame\\stuff\\ps_texture_1k.png"),
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

      /// tests all images from PngSuite
      /// \see http://www.schaik.com/pngsuite2011/pngsuite.html
      TEST_METHOD(TestPngSuite)
      {
         FileFinder ff(_T("D:\\projekte\\MultiplayerOnlineGame\\stuff\\pngsuite\\"),
            _T("*.png"));

         unsigned int uiFiles = 0, uiErrors = 0;
         ATLTRACE(_T("Testing PngSuite...\n"));
         if (ff.IsValid())
         {
            do
            {
               if (ff.IsDot() || ff.IsFolder())
                  continue;

               CString cszFilename = Path(ff.Filename()).FilenameAndExt();

               ATLTRACE(_T("\nLoading [%s] ... "), cszFilename);

               PngImageReader reader;

               FileStream fs(
                  ff.Filename(),
                  FileStream::modeOpen, FileStream::accessRead, FileStream::shareRead);

               bool bExpectException = cszFilename[0] == _T('x');

               try
               {
                  reader.Load(fs);

                  if (bExpectException)
                  {
                     ATLTRACE(_T("expected exception, but got none!"));
                     uiErrors++;
                  }
                  else
                     ATLTRACE(_T("ok\n"));
               }
               catch (const Exception& ex)
               {
                  if (bExpectException)
                     ATLTRACE(_T("ok, expected exception: %s"), ex.Message());
                  else
                  {
                     ATLTRACE(_T("exception: %s"), ex.Message());
                     uiErrors++;
                  }
               }
               catch (...)
               {
                  ATLTRACE(_T("unknown exception!"));
                  uiErrors++;
               }

               uiFiles++;

            } while (ff.Next());
         }

         ATLTRACE(_T("Done; couldn't load %u of %u files.\n"), uiErrors, uiFiles);
      }
   };

} // namespace UnitTest
