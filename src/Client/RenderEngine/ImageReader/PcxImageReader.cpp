//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file PcxImageReader.cpp Image reader for PCX image format
//
#pragma once

// includes
#include "StdAfx.h"
#include "PcxImageReader.hpp"
#include <ulib/stream/IStream.hpp>

#pragma pack(push, 1)

/// PCX file header
struct PCXHeader
{
   BYTE manufacturer;
   BYTE version;
   BYTE encoding;
   BYTE bitsPerPixel;

   USHORT xmin, ymin;
   USHORT xmax, ymax;
   USHORT horzRes, vertRes;

   BYTE palette[48];
   BYTE reserved;
   BYTE numColorPlanes;

   USHORT bytesPerScanLine;
   USHORT paletteType;
   USHORT horzSize, vertSize;

   BYTE padding[54];
};

#pragma pack(pop)

BOOST_STATIC_ASSERT(sizeof(PCXHeader) == 16 + 48 + 10 + 54);

void PcxImageReader::Load(Stream::IStream& stream)
{
   // read header
   PCXHeader header = {0};
   DWORD dwRead = 0;
   stream.Read(&header, sizeof(header), dwRead);

   // check header
   if (header.manufacturer != 0x0a)
      throw Exception(_T("wrong manufacturer number!"), __FILE__, __LINE__);

   // allocate pixels
   m_uiWidth = header.xmax - header.xmin + 1;
   m_uiHeight = header.ymax - header.ymin + 1;

   m_vecPixels.resize(m_uiWidth * m_uiHeight);

   // read in palette
   ULONGLONG ullPos = stream.Position();

   BYTE palette[256][3];
   stream.Seek(-768LL, Stream::IStream::seekEnd);
   stream.Read(&palette, sizeof(palette), dwRead);
   ATLASSERT(dwRead == sizeof(palette));

   stream.Seek(static_cast<LONGLONG>(ullPos), Stream::IStream::seekBegin);

   // read in image
   // \note only supports 8-bit at the moment
   unsigned int uiBitcount = header.bitsPerPixel * header.numColorPlanes;
   if (uiBitcount != 8)
      throw Exception(_T("wrong bit count!"), __FILE__, __LINE__);

   // read pixel data
   for (unsigned int y = 0; y < m_uiHeight; ++y)
   {
      Color* pColor = &m_vecPixels[y * m_uiWidth];
      unsigned int uiBytesLeft = header.bytesPerScanLine;

      // decode line
      unsigned int uiCount = 0;
      BYTE ubValue = 0;
      while (uiBytesLeft--)
      {
         if (uiCount == 0)
         {
            BYTE b = stream.ReadByte();
            if (b < 0xc0)
            {
               uiCount = 1;
               ubValue = b;
            }
            else
            {
               uiCount = b - 0xc0;
               ubValue = stream.ReadByte();
            }
         }

         uiCount--;

         *pColor = Color(
            palette[ubValue][0],
            palette[ubValue][1],
            palette[ubValue][2],
            255); // opaque
         pColor++;
      }
   }
}
