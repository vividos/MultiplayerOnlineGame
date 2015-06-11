//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestTgaImageReader.cpp Unit tests for class TgaImageReader
//

// includes
#include "stdafx.h"
#include "TgaImageReader.hpp"
#include <ulib/stream/FileStream.hpp>
#include "FileFinder.hpp"
#include "Path.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using Stream::FileStream;

namespace UnitTest
{
   /// tests class TgaImageReader
   TEST_CLASS(TestTgaImageReader)
   {
      /// tests tga image reader
      TEST_METHOD(TestReadTgaFiles)
      {
         FileFinder ff(_T("D:\\projekte\\MultiplayerOnlineGame\\"), _T("*.tga"));

         if (ff.IsValid())
         {
            do
            {
               if (ff.IsDot() || ff.IsFolder())
                  continue;

               CString cszFilename = Path(ff.Filename()).FilenameAndExt();

               ATLTRACE(_T("\nLoading [%s] ... "), cszFilename);

               TgaImageReader reader;

               FileStream fs(ff.Filename(),
                  FileStream::modeOpen, FileStream::accessRead, FileStream::shareRead);

               reader.Load(fs);

            } while (ff.Next());
         }
      }
   };

} // namespace UnitTest
