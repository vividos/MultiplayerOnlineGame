//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ZlibDecompressor.cpp zlib inflate decompressor
//
#pragma once

// includes
#include "StdAfx.h"
#include "ZlibDecompressor.hpp"
#include <zlib.h>

// link to zlib1.dll
#pragma comment(lib, "zdll.lib")

/// exception for errors from zlib 
class ZlibException: public Exception
{
public:
   /// ctor
   ZlibException(int iZlibErrorValue, LPCSTR pszSourceFile, UINT uiSourceLine) throw()
      :Exception(FormatMessage(iZlibErrorValue), pszSourceFile, uiSourceLine)
   {
   }

private:
   /// formats error message
   static LPCTSTR FormatMessage(int iZlibErrorValue)
   {
      switch (iZlibErrorValue)
      {
      case Z_ERRNO:        return _T("zlib error (see errno)"); break;
      case Z_STREAM_ERROR: return _T("zlib stream error"); break;
      case Z_DATA_ERROR:   return _T("zlib data error"); break;
      case Z_MEM_ERROR:    return _T("zlib memory error"); break;
      case Z_BUF_ERROR:    return _T("zlib buffer error"); break;
      case Z_VERSION_ERROR:return _T("zlib version error"); break;
      default:
         ATLASSERT(false); // unknown error value
         break;
      }
      return _T("unknown zlib error");
   }
};

//
// ZlibDecompressor
//

/// \param bWindowSizeFromStream if true, window size is read from zlib stream
ZlibDecompressor::ZlibDecompressor(bool bWindowSizeFromStream)
:m_spStream(new z_stream)
{
   CheckVersion();

   memset(m_spStream.get(), 0, sizeof(z_stream));

   m_spStream->data_type = Z_BINARY;
   m_spStream->zalloc = Z_NULL;
   m_spStream->zfree = Z_NULL;
   m_spStream->opaque = Z_NULL;
   m_spStream->next_in = Z_NULL;
   m_spStream->avail_in = 0;

   int iRet;
   if (bWindowSizeFromStream)
      iRet = inflateInit(m_spStream.get());
   else
      iRet = inflateInit2(m_spStream.get(), -MAX_WBITS);

   if (iRet != Z_OK)
      throw ZlibException(iRet, __FILE__, __LINE__);
}

ZlibDecompressor::~ZlibDecompressor() throw()
{
   inflateEnd(m_spStream.get());
   m_spStream.reset();
}

bool ZlibDecompressor::Uncompress(const BYTE* pbData, size_t uiLength,
                                  size_t& uiUnusedInBytes,
                                  std::vector<BYTE>& vecUncompressedData,
                                  size_t uiMaxUncompress)
{
   ATLASSERT(m_spStream != NULL);
   ATLASSERT(pbData != NULL);
   ATLASSERT(uiLength > 0);
   ATLASSERT(uiMaxUncompress > 0);

   vecUncompressedData.resize(uiMaxUncompress);

   m_spStream->next_in = const_cast<BYTE*>(pbData);
   m_spStream->avail_in = uiLength;
   m_spStream->next_out = &vecUncompressedData[0];
   m_spStream->avail_out = uiMaxUncompress;

   int iRet = inflate(m_spStream.get(), Z_SYNC_FLUSH);

   if (iRet < 0)
      throw ZlibException(iRet, __FILE__, __LINE__);

   /// return number of unused bytes
   uiUnusedInBytes = m_spStream->avail_in;

   // resize to actual number of bytes produced
   // avail_out represents the remaining bytes not filled in buffer, so we can
   // calculate how many bytes are valid
   size_t uiProducedBytes = vecUncompressedData.size() - m_spStream->avail_out;

   vecUncompressedData.resize(uiProducedBytes);

   if (iRet == Z_STREAM_END)
      return false;

   return true; // not at end yet
}

unsigned int ZlibDecompressor::TotalIn() const throw()
{
   ATLASSERT(m_spStream != NULL);
   return m_spStream->total_in;
}

unsigned int ZlibDecompressor::TotalOut() const throw()
{
   ATLASSERT(m_spStream != NULL);
   return m_spStream->total_out;
}

void ZlibDecompressor::CheckVersion()
{
   HMODULE hMod = LoadLibrary(_T("zlib1.dll"));
   if (hMod == NULL)
      throw Exception(_T("Missing zlib1.dll"), __FILE__, __LINE__);

   FreeLibrary(hMod);

   const char* pchActualVersion = zlibVersion();
   if (pchActualVersion == NULL || 0 != strcmp(ZLIB_VERSION, pchActualVersion))
      throw Exception(_T("Wrong zlib1.dll used; expected version: ")_T(ZLIB_VERSION),
         __FILE__, __LINE__);
}

CString ZlibDecompressor::ZlibVersion() throw()
{
   return CString(_T(ZLIB_VERSION));
}
