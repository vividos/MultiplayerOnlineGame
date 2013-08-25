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

/// buffer size when skipping over blocks in Seek()
const DWORD c_dwSkipBufferSize = 16384;

bool ZipArchiveFile::Read(void* bBuffer, DWORD dwMaxBufferLength, DWORD& dwBytesRead)
{
   if (AtEndOfStream())
      return false;

   FillOutBuffer(dwMaxBufferLength);

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

   return dwBytesRead > 0;
}

BYTE ZipArchiveFile::ReadByte()
{
   ATLASSERT(AtEndOfStream() == false);

   if (m_vecOutBuffer.empty())
      FillOutBuffer(1);

   ATLASSERT(m_vecOutBuffer.empty() == false); // must not be empty

   BYTE bRet = m_vecOutBuffer[0];

   m_vecOutBuffer.erase(m_vecOutBuffer.begin(), m_vecOutBuffer.begin() + 1);

   m_ullCurrentPos++;

   // check input buffer
   if (m_bEndOfInputStream && m_vecInBuffer.empty() && m_vecOutBuffer.empty())
   {
      // can't uncompress without data in input buffer
      m_bEndOfOutputStream = true;
   }

   return bRet;
}


ULONGLONG ZipArchiveFile::Seek(LONGLONG llOffset, ESeekOrigin origin)
{
   if (AtEndOfStream())
   {
      // seek beyond end of file
      return m_uiUncompressedSize;
   }

   // calc new position
   ULONGLONG ullNewPos = m_ullCurrentPos;
   switch (origin)
   {
   case seekBegin:
      ullNewPos = llOffset;
      break;

   case seekCurrent:
      ullNewPos += llOffset;

   case seekEnd:
      ullNewPos = ULONGLONG(m_uiUncompressedSize) - llOffset;
      break;

   default:
      ATLASSERT(false);
      break;
   }

   if (ullNewPos >= m_uiUncompressedSize)
   {
      // just seeked to end of file
      Close();
      return m_uiUncompressedSize;
   }

   // could be converted into rewind + forward skip
   if (ullNewPos < m_ullCurrentPos)
      throw Exception(_T("zip file archive stream can't be seeked backwards"), __FILE__, __LINE__);

   ULONGLONG ullBytesToSkip = ullNewPos - m_ullCurrentPos;

   if (ullBytesToSkip == 0)
      return m_ullCurrentPos;

   // skip over bytes
   do
   {
      // consume out buffer
      if (ullBytesToSkip < m_vecOutBuffer.size())
      {
         m_vecOutBuffer.erase(m_vecOutBuffer.begin(), m_vecOutBuffer.begin() + static_cast<size_t>(ullBytesToSkip));

         ullBytesToSkip = 0;
      }
      else
      if (ullBytesToSkip == m_vecOutBuffer.size())
      {
         m_vecOutBuffer.clear();
         ullBytesToSkip = 0;
      }
      else
      {
         // more than enough in buffer
         ullBytesToSkip -= m_vecOutBuffer.size();
         m_vecOutBuffer.clear();
      }

      // when not enough, decode more input
      if (ullBytesToSkip > 0)
      {
         FillInputBuffer();

         // read in at most 16k at a time
         DWORD dwBytesToRead = static_cast<DWORD>(std::min<ULONGLONG>(ullBytesToSkip, c_dwSkipBufferSize));

         FillOutBuffer(dwBytesToRead);
      }

   } while (ullBytesToSkip > 0);

   m_ullCurrentPos = ullNewPos;

   return m_ullCurrentPos;
}

/// at most dwMinInBufferSize bytes must be decoded, except for the last bytes in stream
void ZipArchiveFile::FillOutBuffer(DWORD dwMinInBufferSize)
{
   std::vector<BYTE> vecTempOutBuffer;
   while (m_vecOutBuffer.size() < dwMinInBufferSize)
   {
      FillInputBuffer();

      // check input buffer
      if (m_bEndOfInputStream && m_vecInBuffer.empty())
      {
         // can't uncompress without data in input buffer
         m_bEndOfOutputStream = true;
         return;
      }

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

         //ATLTRACE(_T("Uncompress(): input %u bytes, output %u bytes, consumed %u input bytes\n"),
         //   m_vecInBuffer.size(),
         //   vecTempOutBuffer.size(),
         //   m_vecInBuffer.size() - uiUnusedInBytes);

         if (!bRet && vecTempOutBuffer.empty())
         {
            m_bEndOfOutputStream = true;
            return;
         }
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
   if (m_bEndOfInputStream)
      return;

   size_t uiCurrentSize = m_vecInBuffer.size();
   size_t uiInRemainingSize = c_uiMaxInBufferSize - uiCurrentSize;

   if (uiInRemainingSize == 0)
      return; // no need to fill buffer

   DWORD dwBytesToRead = std::min(m_uiCompressedRemaining, uiInRemainingSize);

   m_vecInBuffer.resize(uiCurrentSize + dwBytesToRead);

   //ATLTRACE(_T("FillInputBuffer(): input buffer has %u bytes, filling up %u bytes... "),
   //   uiCurrentSize, dwBytesToRead);

   DWORD dwInBytesRead = 0;
   bool bRet = m_spArchiveFile->Read(&m_vecInBuffer[uiCurrentSize], dwBytesToRead, dwInBytesRead);
   if (bRet)
   {
      ATLASSERT(dwInBytesRead != 0);

      //ATLTRACE(_T("done, read %u bytes.\n"), dwInBytesRead);

      if (dwInBytesRead < dwBytesToRead)
         m_vecInBuffer.resize(uiCurrentSize + dwInBytesRead);
   }
   else
   {
      //ATLTRACE(_T("done, read %u bytes, and at end of stream.\n"), dwInBytesRead);

      // no more bytes read; at end of stream
      m_bEndOfInputStream = true;
   }

   m_uiCompressedRemaining -= dwInBytesRead;

   if (m_uiCompressedRemaining == 0)
      m_bEndOfInputStream = true;
}
