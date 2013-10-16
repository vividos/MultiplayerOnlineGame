//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file JpegImageReader.cpp Image reader for JPEG format
//

// includes
#include "StdAfx.h"
#include "JpegImageReader.hpp"
#include "JpegDecoder.hpp"
#include <ulib/stream/IStream.hpp>

void JpegImageReader::Load(Stream::IStream& stream)
{
   JpegDecoder d(stream);

   d.ReadHeader();

   // set parameters for decompression
   if (d.cinfo.num_components != 3)
      d.cinfo.out_color_space = JCS_RGB; // convert grayscale to RGB

   d.StartDecompress();

   m_uiWidth = d.cinfo.output_width;
   m_uiHeight = d.cinfo.output_height;
   ATLASSERT(d.cinfo.output_components == 3);

   m_vecPixels.resize(m_uiWidth * m_uiHeight);

   std::vector<JSAMPLE> vecScanline(d.cinfo.output_width * d.cinfo.output_components);
   JSAMPROW pScanline = &vecScanline[0];

   // while (scan lines remain to be read)
   unsigned int uiScanline = 0;
   while (d.HasScanlines())
   {
      JDIMENSION dim = jpeg_read_scanlines(&d.cinfo, &pScanline, 1);
      if (dim != 1)
         break;

      // store RGB in RGBA array
      Color* pDest = &m_vecPixels[0] + uiScanline * d.cinfo.output_width;
      for (unsigned int i = 0; i<d.cinfo.output_width; i++)
      {
         JSAMPLE* pSrc = &vecScanline[0] + i*3;

         pDest[i].m_color[0] = pSrc[0];
         pDest[i].m_color[1] = pSrc[1];
         pDest[i].m_color[2] = pSrc[2];
      }

      uiScanline++;
   }
}
