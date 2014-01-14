//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestOggVorbisFileReader.cpp Unit tests for class OggVorbisFileReader
//

// includes
#include "stdafx.h"
#include "OggVorbisFileReader.hpp"
#include "BaseFileSystem.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

/// tests class OggVorbisFileReader
TEST_CLASS(TestOggVorbisFileReader)
{
   /// tests default ctor
   TEST_METHOD(TestDefaultCtor)
   {
      OggVorbisFileReader reader;
   }

   /// tests reading ogg vorbis file
   TEST_METHOD(TestReadOggVorbis)
   {
      OggVorbisFileReader reader;

      BaseFileSystem fs;
      std::shared_ptr<Stream::IStream> spStream = fs.OpenFile(
         fs.AppDataFolder() + _T("audio\\music\\Arcadia.ogg"), true);

      reader.Read(*spStream);

      Assert::IsTrue(reader.Channels() == 2);
      Assert::IsTrue(reader.Samplerate() == 44100);
      Assert::IsTrue(reader.Samples().size() > 0);
   }
};

} // namespace UnitTest
