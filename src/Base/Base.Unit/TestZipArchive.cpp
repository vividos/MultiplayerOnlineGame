//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TestZipArchive.cpp Unit tests for class ZipArchive
//

// includes
#include "stdafx.h"
#include "ZipArchive.hpp"
#include <ulib/stream/FileStream.hpp>
#include <ulib/stream/TextStreamFilter.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

/// tests class ZipArchive
TEST_CLASS(TestZipArchive)
{
   /// tests loading zip archives
   TEST_METHOD(TestLoadingZipArchive)
   {
      CString cszFilename(_T("D:\\projekte\\MultiplayerOnlineGame\\downloads\\done\\terrain_source.zip"));

      Stream::FileStream fs(cszFilename,
         Stream::FileStream::modeOpen,
         Stream::FileStream::accessRead,
         Stream::FileStream::shareRead);

      ZipArchive za(fs);

      unsigned int iMax = za.FileCount();
      Assert::IsTrue(iMax != 0);

      for (unsigned int i=0; i<iMax; i++)
      {
         CString cszFilename = za.Filename(i, true);
         CString cszFilenameWithoutPath = za.Filename(i, false);

         ATLTRACE(_T("File %u: %s\n"), i, cszFilename);

         boost::shared_ptr<Stream::IStream> spStream = za.GetFile(i);

         if (cszFilename.Right(4) == _T(".cpp") ||
            cszFilename.Right(2) == _T(".h"))
         {
            // read source file
            Stream::TextStreamFilter text(*spStream, Stream::ITextStream::textEncodingAnsi);
            CString cszLine;
            while (!text.AtEndOfStream())
            {
               text.ReadLine(cszLine);
               ATLTRACE(_T("TEXT: %s\n"), cszLine);
            }

            spStream->Close();
         }
         else
         {
            // read binary file
            ULONGLONG ullSize = spStream->Length();
            if (ullSize > 0 && ullSize < std::numeric_limits<size_t>::max())
            {
               size_t uiSize = static_cast<size_t>(ullSize);
               std::vector<BYTE> vecData(uiSize);
               DWORD dwReadBytes = 0;
               spStream->Read(&vecData[0], uiSize, dwReadBytes);
            }
         }
      }
   }
};

} // namespace UnitTest
