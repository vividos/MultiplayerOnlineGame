//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file OggVorbisStreamCallbacks.hpp Ogg Vorbis IStream callbacks
//
#pragma once

// includes
#include <vorbis/vorbisfile.h>
#include <ulib/stream/IStream.hpp>

/// \brief implementation of ov_callbacks class for Stream::IStream interface
/// \details used by ogg vorbis library call ov_open_callbacks() to do custom
/// I/O
struct OggVorbisStreamCallbacks: public ov_callbacks
{
   /// ctor
   OggVorbisStreamCallbacks(bool bAutoClose, bool bSeekable) throw()
   {
      read_func = &OggVorbisStreamCallbacks::Read;

      if (bAutoClose)
         close_func = &Close;
      else
         close_func = NULL;

      if (bSeekable)
      {
         seek_func = &Seek;
         tell_func = &Tell;
      }
      else
      {
         seek_func = NULL;
         tell_func = NULL;
      }
   }

private:
   /// reads data from stream
   static size_t Read(void* ptr, size_t size, size_t nmemb, void* datasource)
   {
      if (ToStream(datasource).AtEndOfStream())
         return 0;

      try
      {
         DWORD dwBytesRead = 0;
         ToStream(datasource).Read(ptr, size*nmemb, dwBytesRead);

         errno = 0;

         return dwBytesRead;
      }
      catch(...)
      {
         errno = 1;
         return 0;
      }
      return 0;
   }

   /// seeks in stream
   static int Seek(void* datasource, ogg_int64_t offset, int whence)
   {
      if (!ToStream(datasource).CanSeek())
         return -1;

      try
      {
         // we can pass 'whence' to Seek() unmodified as long as these conditions are met:
         static_assert(SEEK_SET == Stream::IStream::seekBegin, "seekBegin must have same value as SEEK_SET");
         static_assert(SEEK_CUR == Stream::IStream::seekCurrent, "seekCurrent must have same value as SEEK_CUR");
         static_assert(SEEK_END == Stream::IStream::seekEnd, "seekEnd must have same value as SEEK_END");

         ToStream(datasource).Seek(offset,
            static_cast<Stream::IStream::ESeekOrigin>(whence));
      }
      catch(...)
      {
         return -1;
      }
      return 0;
   }

   /// closes stream
   static int Close(void* datasource)
   {
      try
      {
         ToStream(datasource).Close();
      }
      catch(...)
      {
         return 1;
      }
      return 0;
   }

   /// returns current position in stream
   static long Tell(void* datasource)
   {
      if (!ToStream(datasource).CanSeek())
         return -1;

      return static_cast<long>(ToStream(datasource).Position());
   }

   /// converts opaque data source pointer to IStream
   static Stream::IStream& ToStream(void* datasource)
   {
      ATLASSERT(datasource != NULL);
      return *reinterpret_cast<Stream::IStream*>(datasource);
   }
};
