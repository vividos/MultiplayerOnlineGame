//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file OggVorbisFileReader.cpp Ogg Vorbis audio file reader
//

// includes
#include "StdAfx.h"
#include "OggVorbisFileReader.hpp"
#include <vorbis/vorbisfile.h>
#include <ulib/stream/IStream.hpp>
#include "OggVorbisStreamCallbacks.hpp"

// auto-link to static libs so that unused functions are removed by linker
#pragma comment(lib, "libvorbisfile_static.lib")
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libogg_static.lib")

OggVorbisFileReader::OggVorbisFileReader()
:m_uiChannels(0),
 m_uiSamplerate(0)
{
}

void OggVorbisFileReader::Read(Stream::IStream& stream)
{
   OggVorbis_File vf;

   // open file
   OggVorbisStreamCallbacks callbacks(false, stream.CanSeek());

   int iRet = ov_open_callbacks(&stream, &vf, NULL, 0, callbacks);
   if (iRet == -1)
   {
      ov_clear(&vf);
      throw Exception(_T("Ogg Vorbis: couldn't open file"), __FILE__, __LINE__);
   }

   // get metadata
   {
      vorbis_info* vi = ov_info(&vf, -1); // vi doesn't need to be freed
      if (vi == NULL)
      {
         ov_clear(&vf);
         throw Exception(_T("Ogg Vorbis: couldn't get info"), __FILE__, __LINE__);
      }

      m_uiChannels = vi->channels;
      m_uiSamplerate = vi->rate;
   }

   // prepare buffer
   ogg_int64_t ilLength = ov_pcm_total(&vf, -1);

   // double number of samples when stereo
   if (m_uiChannels == 2)
      ilLength <<= 1;

   ATLASSERT(ilLength >= 0 && ilLength <= std::numeric_limits<size_t>::max());
   size_t uiLength = static_cast<size_t>(ilLength);

   m_vecSamples.resize(uiLength);

   // read in samples
   int iCurrentBitstream = 0;
   bool bEof = false;
   size_t ulAlreadyRead = 0;
   while(!bEof)
   {
      // read 64k samples at once
      int iLengthToRead = std::min(int(uiLength - ulAlreadyRead), 65536);

      if (iLengthToRead == 0)
      {
         bEof = true;
         break;
      }

      long lRet = ov_read(&vf,
         reinterpret_cast<char*>(&m_vecSamples[ulAlreadyRead]),
         iLengthToRead * 2, // in bytes
         0, // big endian: no
         2, // size of word
         1, // signed: yes
         &iCurrentBitstream);

      if (lRet == 0)
         bEof = true;
      else if (lRet < 0)
      {
         // error in the stream
         break;
      }
      else
      {
         // ov_read returns number of bytes written, so divide by size of word
         ulAlreadyRead += lRet / 2;
      }
   }

   // finished
   ov_clear(&vf);

   // when not at EOF, we had an error
   if (!bEof)
      throw Exception(_T("Ogg Vorbis: couldn't read samples"), __FILE__, __LINE__);
}
