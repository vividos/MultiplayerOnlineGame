//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file OggVorbisFileReader.hpp Ogg Vorbis audio file reader
//
#pragma once

// includes
#include <vector>

// forward references
namespace Stream
{
class IStream;
}

/// file reader for ogg vorbis audio files (*.ogg)
class OggVorbisFileReader
{
public:
   /// ctor
   OggVorbisFileReader() throw();

   /// reads
   void Read(Stream::IStream& stream);

   /// returns samples; when stereo, samples are interleaved, in left right order
   const std::vector<short>& Samples() const throw() { return m_vecSamples; }

   /// returns number of channels
   unsigned int Channels() const throw() { return m_uiChannels; }

   /// returns sample rate
   unsigned int Samplerate() const throw() { return m_uiSamplerate; }

private:
   std::vector<short> m_vecSamples; ///< audio samples
   unsigned int m_uiChannels;       ///< number of channels
   unsigned int m_uiSamplerate;     ///< sample rate
};
