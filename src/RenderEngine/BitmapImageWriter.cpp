//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BitmapImageWriter.cpp Bitmap image writer class
//

// includes
#include "StdAfx.h"
#include "BitmapImageWriter.hpp"
#include "Bitmap.hpp"
#include <ulib/stream/FileStream.hpp>

using Stream::FileStream;

void BitmapImageWriter::Write(const CString& cszFilename) const
{
   FileStream fs(cszFilename, FileStream::modeCreate, FileStream::accessWrite, FileStream::shareRead);

   DWORD dwBytesWritten = 0;

   unsigned int uiLineLength = m_bmp.XRes() * 3;
   unsigned int uiPadding = 0;
   if ((uiLineLength & 3) != 0)
   {
      uiPadding = 4 - (uiLineLength & 3);
      uiLineLength += uiPadding;
   }

   // write header
   BITMAPFILEHEADER bfh = {0};
   bfh.bfType = 0x4d42;
   bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
   bfh.bfSize = bfh.bfOffBits + uiLineLength * m_bmp.YRes();
   fs.Write(&bfh, sizeof(bfh), dwBytesWritten);

   BITMAPINFOHEADER bih = {0};
   bih.biSize = sizeof(bih);
   bih.biWidth = m_bmp.XRes();
   bih.biHeight = m_bmp.YRes();
   bih.biBitCount = 24;
   fs.Write(&bih, sizeof(bih), dwBytesWritten);

   // write each line
   const Color* pBase = m_bmp.Data();

   BYTE abPadding[3] = {0};

   for(int y=m_bmp.YRes()-1; y>=0; y--)
   {
      const Color* pLine = pBase + y * m_bmp.XRes();
      BOOST_STATIC_ASSERT(4 == sizeof(*pLine));

      for(int x=0, xMax = m_bmp.XRes(); x<xMax; x++)
         fs.Write(pLine[x].m_color, 3, dwBytesWritten);

      // write padding for this line
      if (uiPadding > 0)
         fs.Write(abPadding, uiPadding, dwBytesWritten);
   }
}
