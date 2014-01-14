//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file JpegSourceManager.hpp Source manager for reading in JPEG files
//
#pragma once

// includes
#include "JpegImageReader.hpp"
#include <ulib/stream/IStream.hpp>
#include <jpeglib.h>

/// source buffer size
static const unsigned int c_uiSourceBufferSize = 64 * 1024;

/// jpeg source manager
struct JpegSourceManager: public jpeg_source_mgr
{
   /// ctor
   JpegSourceManager(Stream::IStream& stream)
      :m_stream(stream)
   {
      this->init_source = &InitSource;
      this->fill_input_buffer = &FillInputBuffer;
      this->skip_input_data = &SkipInputData;
      this->resync_to_restart = &jpeg_resync_to_restart;
      this->term_source = &TermSource;

      m_vecBuffer.resize(c_uiSourceBufferSize);

      this->next_input_byte = &m_vecBuffer[0];
      this->bytes_in_buffer = 0;
   }

   /// dtor
   ~JpegSourceManager()
   {
      m_stream.Close();
   }

   /// inits source
   static void InitSource(j_decompress_ptr cinfo)
   {
      //JpegSourceManager* pT = reinterpret_cast<JpegSourceManager*>(cinfo->src);
      cinfo->src->bytes_in_buffer = 0;
   }

   /// fills input buffer
   static boolean FillInputBuffer(j_decompress_ptr cinfo)
   {
      JpegSourceManager* pT = reinterpret_cast<JpegSourceManager*>(cinfo->src);

      DWORD dwMaxBufferLength = sizeof(JOCTET) * pT->m_vecBuffer.size();
      DWORD dwBytesRead = 0;

      bool bRet = pT->m_stream.Read(&pT->m_vecBuffer[0], dwMaxBufferLength, dwBytesRead);

      if (bRet && dwBytesRead == 0)
      {
         // fake EOI
         pT->m_vecBuffer[0] = 0xFF;
         pT->m_vecBuffer[1] = JPEG_EOI;
         dwBytesRead = 2;
      }

      cinfo->src->next_input_byte = &pT->m_vecBuffer[0];
      cinfo->src->bytes_in_buffer = dwBytesRead;

      return TRUE;
   }

   /// skips input data
   static void SkipInputData(j_decompress_ptr cinfo, long num_bytes)
   {
      if (num_bytes <= 0)
         return;

      JpegSourceManager* pT = reinterpret_cast<JpegSourceManager*>(cinfo->src);

      if (static_cast<long>(cinfo->src->bytes_in_buffer) >= num_bytes)
      {
         // in-buffer skip
         cinfo->src->bytes_in_buffer -= num_bytes;
         cinfo->src->next_input_byte += num_bytes;
      }
      else
      {
         long num_skip = num_bytes - cinfo->src->bytes_in_buffer;
         pT->m_stream.Seek(num_skip, Stream::IStream::seekCurrent);

         cinfo->src->bytes_in_buffer = 0;
      }
   }

   /// terminates source
   static void TermSource(j_decompress_ptr /*cinfo*/)
   {
   }

   /// stream source
   Stream::IStream& m_stream;

   /// buffer
   std::vector<JOCTET> m_vecBuffer;
};
