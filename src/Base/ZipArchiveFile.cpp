//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ZipArchiveFile.cpp File stream from zip archive
//

// includes
#include "StdAfx.h"
#include "ZipArchiveFile.hpp"

/// max input buffer size, in bytes
const size_t c_uiMaxInBufferSize = 4096;

bool ZipArchiveFile::Read(void* bBuffer, DWORD dwMaxBufferLength, DWORD& dwBytesRead)
{
   FillOutBuffer(dwMaxBufferLength);

   ATLASSERT(m_vecOutBuffer.size() <= dwMaxBufferLength);

   // determine number of bytes to return
   dwBytesRead = std::min(dwMaxBufferLength, DWORD(m_vecOutBuffer.size()));
   if (dwBytesRead > 0)
   {       
      memcpy(bBuffer, &m_vecOutBuffer[0], dwBytesRead);

      if (dwBytesRead < m_vecOutBuffer.size())
         m_vecOutBuffer.erase(m_vecOutBuffer.begin(), m_vecOutBuffer.begin() + dwBytesRead);
      else
         m_vecOutBuffer.clear();
   }

   m_ullCurrentPos += dwBytesRead;
   return false;
}

BYTE ZipArchiveFile::ReadByte()
{
   if (m_vecOutBuffer.empty())
      FillOutBuffer(1);

   ATLASSERT(m_vecOutBuffer.empty() == false); // must not be empty

   BYTE bRet = m_vecOutBuffer[0];

   m_vecOutBuffer.erase(m_vecOutBuffer.begin(), m_vecOutBuffer.begin() + 1);

   m_ullCurrentPos++;

   return bRet;
}

/// at most dwMinInBufferSize bytes must be decoded, except for the last bytes in stream
void ZipArchiveFile::FillOutBuffer(DWORD dwMinInBufferSize)
{
   std::vector<BYTE> vecTempOutBuffer;
   while (m_vecOutBuffer.size() < dwMinInBufferSize)
   {
      FillInputBuffer();

      // uncompress
      size_t uiUnusedInBytes = 0;
      {
         DWORD dwBytesToUncompress = dwMinInBufferSize - m_vecOutBuffer.size();

         // ensure minimum number of bytes to produce
         dwBytesToUncompress = std::max(dwBytesToUncompress, DWORD(256));

         bool bRet = m_decompressor.Uncompress(
            &m_vecInBuffer[0],
            m_vecInBuffer.size(),
            uiUnusedInBytes,
            vecTempOutBuffer, dwBytesToUncompress);

         ATLTRACE(_T("Uncompress(): input %u bytes, output %u bytes, consumed %u input bytes\n"),
            m_vecInBuffer.size(),
            vecTempOutBuffer.size(),
            m_vecInBuffer.size() - uiUnusedInBytes);

         if (!bRet)
            m_bEndOfStream = true;
      }

      // append to out buffer
      if (!vecTempOutBuffer.empty())
      {
         m_vecOutBuffer.insert(m_vecOutBuffer.end(), vecTempOutBuffer.begin(), vecTempOutBuffer.end());
         vecTempOutBuffer.clear();
      }

      // move unused in bytes to front of buffer
      if (uiUnusedInBytes > 0)
      {
         size_t uiDecompressed = m_vecInBuffer.size() - uiUnusedInBytes;

         if (uiDecompressed > 0)
            m_vecInBuffer.erase(m_vecInBuffer.begin(), m_vecInBuffer.begin() + uiDecompressed);
      }
      else
         m_vecInBuffer.clear();
   }
}

void ZipArchiveFile::FillInputBuffer()
{
   size_t uiCurrentSize = m_vecInBuffer.size();
   size_t uiInRemainingSize = c_uiMaxInBufferSize - uiCurrentSize;

   if (uiInRemainingSize == 0)
      return; // no need to fill buffer

   m_vecInBuffer.resize(c_uiMaxInBufferSize);

   ATLTRACE(_T("FillInputBuffer(): input buffer has %u bytes, filling up %u bytes... "),
      uiCurrentSize, uiInRemainingSize);

   DWORD dwInBytesRead = 0;
   bool bRet = m_archiveFile.Read(&m_vecInBuffer[uiCurrentSize], uiInRemainingSize, dwInBytesRead);
   if (bRet)
   {
      ATLASSERT(dwInBytesRead != 0);

      ATLTRACE(_T("done, read %u bytes.\n"), dwInBytesRead);

      if (dwInBytesRead < uiInRemainingSize)
         m_vecInBuffer.resize(dwInBytesRead);
   }
   else
   {
      ATLTRACE(_T("done, read %u bytes, and at end of stream.\n"), dwInBytesRead);

      // no more bytes read; at end of stream
      m_bEndOfStream = true;
   }
}
