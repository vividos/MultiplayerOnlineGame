//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TgaImageReader.cpp Image reader for TGA format
//

// includes
#include "StdAfx.h"
#include "TgaImageReader.hpp"
#include <ulib/stream/IStream.hpp>

/* here is a snippet from the File Format List by Max Maischein
   (look for FFMTS???.ZIP on the inet)
--------I-TARGA-----------------------------
The Targa-File format is an image file format used by a wide variety of both
scanners and imaging software, and exists in many incarnations. The information
has been taken from Appendix C of the Truevision Technical Guide.Requests for
further information could be directed to:

           AT&T
           Electronic Photography and Imaging Center
           2002 Wellesley Ave.
           Indianapolis, IN 42619

The lack of completeness is due to the fact that the Targa recognizes
over half a dozen image file formats, some of which are more widely
used than others.


OFFSET              Count TYPE   Description
0000h                   1 byte   Length of image identification field (below)
0001h                   1 byte   Color map type :
                                 0 - no color map
                                 1 - 256 entry palette
0002h                   1 byte   Image type :
                                  0 - no image data included
                                  1 - Uncompressed, color-mapped image
                                  2 - Uncompressed, RGB image
                                  3 - Uncompressed, black and white image
                                  9 - RLE encoded color-mapped image
                                 10 - RLE encoded RGB image
                                 11 - Compressed, black and white image
                                 32 - Compressed color-mapped data, using
                                      Huffman, Delta, and runlength encoding.
                                 33 - Compressed color-mapped data, using
                                      Huffman, Delta, and RLE.  4-pass quadtree-
                                      type process.
0003h                   1 word   Index of first color map entry
0005h                   1 word   Count of color map entries
0007h                   1 byte   Number of bits per color map entry
0008h                   1 word   X coordinate of the lower left corner of
                                 the image.
000Ah                   1 word   Y coordinate of the lower left corner of
                                 the image.
000Ch                   1 word   Width of the image in pixels
000Eh                   1 word   Height of the image in pixels
0010h                   1 byte   Bytes per pixel
0011h                   1 byte   Flags (bitmapped):
                                 0-3 : Number of attribute bits
                                   4 : reserved
                                   5 : Screen origin in upper left corner
                                 6-7 : Data storage interleave
                                       00 - no interleave
                                       01 - even/odd interleave
                                       10 - four way interleave
                                       11 - reserved
                                 The byte should be set to 0. Don't know why.
0012h                   ? char   Image identification string, usually not there,
                                 when the length (see up) is 0.
????h                   ? byte   Color map data
                                 Depending on the number of bits per color map
                                 entry, the entries here have a different size.
                                 4 bytes : 1 byte for blue
                                           1 byte for green
                                           1 byte for red
                                           1 byte for attribute
                                 3 bytes : 1 byte for blue
                                           1 byte for green
                                           1 byte for red
                                 2 bytes : Bitmapped as a word in Intel byte
                                           order as follows :
                                           ARRRRRGG GGGBBBBB
????h                   ? byte   Image data
                                 For images of type 9 (using RLE), the image
                                 data is divided into packets, the first byte
                                 being the indicator for repetition or copy.
                                 If bit 7 of the first byte is set, then repeat
                                 (first byte and 07Fh+1) times the next byte,
                                 otherwise copy first byte+1 pixels from data
                                 stream. RLE packets may cross scan lines !
EXTENSION:TGA
OCCURENCES:PC
SEE ALSO:
--------------------------------------------
*/



/// targa file header
struct TgaHeader
{
   /// header length
   static const unsigned int c_uiHeaderLength = 0x0012;

   /// color map type
   enum T_enColorMapType
   {
      colorMapNone = 0, ///< no color map
      colorMap256 = 1   ///< 256 color map entries
   };

   /// image type
   enum T_enImageType
   {
      imageTypeNoData = 0,       ///< no image data is present
      imageTypeColorMapped = 1,  ///< uncompressed, color-mapped image
      imageTypeTrueColor = 2,    ///< uncompressed, true-color image
      imageTypeBlackWhite = 3,   ///< uncompressed, black-and-white image
      imageTypeColorMappedRLE = 9,  ///< runlength encoded color-mapped image
      imageTypeTrueColorRLE = 10,   ///< runlength encoded RGB image
      imageTypeBlackWhiteCompressed = 11, ///< compressed, black and white image
      imageTypeColorMappedHuffmanDeltaRLE = 32, ///< compressed color-mapped data, using Huffman, Delta, and runlength encoding
      imageTypeColorMappedHuffmanDeltaRLE4Pass = 33, ///< compressed color-mapped data, using Huffman, Delta, and runlength encoding; 4-pass quadtree-type process
   };

   // ctor
   TgaHeader()
   {
      memset(this, 0, sizeof(*this));
   }

   /// reads header from stream
   void Read(Stream::IStream& stream);

   // members

   T_enColorMapType m_enColorMapType;
   T_enImageType m_enImageType;
   bool m_bRunLengthEncoded;

   WORD m_wColorMapFirstIndex;
   WORD m_wColorMapLength;
   BYTE m_bColorMapBits;

   WORD m_wWidth;
   WORD m_wHeight;
   BYTE m_bBitsPerPixel;
   BYTE m_bAlphaChannelDepth;

   bool m_bUpsideDown;
};

/*
   runlength encoded tga format (tga type 9 or 10):
   the first byte's msb indicates what type of data follows.
   1 indicates that the bits 0..7 specify a repeat count for the
     next color value
   0 indicates that bits 0..7 specify the count of uncompressed
     color values that follows
   note that color value means 2, 3 or even 4 bytes of data, dependent
   on the nuber of bits per pixel
*/

void TgaHeader::Read(Stream::IStream& stream)
{
   ATLASSERT(true == stream.CanRead());
   ATLASSERT(true == stream.CanSeek());

   std::vector<BYTE> vecBuffer(c_uiHeaderLength);
   DWORD dwBytesRead = 0;
   stream.Read(&vecBuffer[0], c_uiHeaderLength, dwBytesRead);

   if (dwBytesRead != c_uiHeaderLength)
      throw Exception(_T("couldn't read tga file header"), __FILE__, __LINE__);

   // transfer fields to members

   // 0x0000: length of id field after header

   // 0x0001: color map type
   m_enColorMapType = static_cast<T_enColorMapType>(vecBuffer[0x0001]);
   if (m_enColorMapType != colorMapNone && m_enColorMapType != colorMap256)
      throw Exception(_T("invalid color map type field"), __FILE__, __LINE__);

   // 0x0002: image type and compression type
   m_enImageType = static_cast<T_enImageType>(vecBuffer[0x0002] & 7);
   m_bRunLengthEncoded = (vecBuffer[0x0002] & 8) != 0;

   if ((m_enImageType != imageTypeNoData &&
        m_enImageType != imageTypeColorMapped &&
        m_enImageType != imageTypeTrueColor &&
        m_enImageType != imageTypeBlackWhite) ||
       (m_enImageType == imageTypeNoData && m_bRunLengthEncoded == true))
      throw Exception(_T("invalid image type field"), __FILE__, __LINE__);

   // 0x0003/4, 0x0005/6 and 0x0007: color map origin (16bit), length (16bit), depth (8bit)
   if (m_enImageType == imageTypeColorMapped)
   {
      m_wColorMapFirstIndex = vecBuffer[0x0003] || (WORD(vecBuffer[0x0004]) << 8);
      m_wColorMapLength = vecBuffer[0x0005] || (WORD(vecBuffer[0x0006]) << 8);

      m_bColorMapBits = vecBuffer[0x0007];
   }

   // 0x0008/9, 0x000a/b: x origin, y origin

   // 0x000c/d, 0x000e/f: width, height
   m_wWidth = vecBuffer[0x000c] | (WORD(vecBuffer[0x000d]) << 8);
   m_wHeight = vecBuffer[0x000e] | (WORD(vecBuffer[0x000f]) << 8);

   // 0x0010: bits per pixel
   m_bBitsPerPixel = vecBuffer[0x0010];

   if (m_enImageType == imageTypeTrueColor &&
       (m_bBitsPerPixel != 16 && m_bBitsPerPixel != 24 && m_bBitsPerPixel != 32) )
      throw Exception(_T("invalid bits per pixel"), __FILE__, __LINE__);

   // 0x0011: image descriptor
   m_bAlphaChannelDepth = vecBuffer[0x0011] & 15; // bits 0-3
   m_bUpsideDown = (vecBuffer[0x0011] & (1<<5)) == 0; // bit 5: 1=screen origin in upper left corner

   // skip over identification field
   BYTE bIdentFieldLength = vecBuffer[0x0000];
   stream.Seek(static_cast<LONGLONG>(bIdentFieldLength), Stream::IStream::seekCurrent);
}

void TgaImageReader::Load(Stream::IStream& stream)
{
   // read header
   TgaHeader header;
   header.Read(stream);

   // load color map
   if (header.m_enColorMapType == TgaHeader::colorMap256)
      LoadColorMap(stream, header.m_wColorMapFirstIndex, header.m_wColorMapLength, header.m_bColorMapBits);
   else
   {
      // skip over color map
      DWORD dwSizeToSkip = header.m_wColorMapLength * (header.m_bColorMapBits >> 3);
      if (dwSizeToSkip > 0)
         stream.Seek(static_cast<LONGLONG>(dwSizeToSkip), Stream::IStream::seekCurrent);
   }

   if (header.m_bRunLengthEncoded)
      throw Exception(_T("tga files with RLE encoding not supported"), __FILE__, __LINE__);

   // load image data
   DWORD dwLength = header.m_wWidth * header.m_wHeight;
   if (header.m_enColorMapType == TgaHeader::colorMap256)
   {
      LoadColorMapPixels(stream, dwLength);
   }
   else
   {
      LoadTrueColorPixels(stream, dwLength, header.m_bBitsPerPixel);
   }

   m_uiWidth = header.m_wWidth;
   m_uiHeight = header.m_wHeight;
}

inline Color FromData32Bit(const std::vector<BYTE>& vecData, size_t iIndex)
{
   ATLASSERT(iIndex + 3 < vecData.size());
   return Color(vecData[iIndex+2], vecData[iIndex+1], vecData[iIndex+0], vecData[iIndex+3]);
}

inline Color FromData24Bit(const std::vector<BYTE>& vecData, size_t iIndex)
{
   ATLASSERT(iIndex + 2 < vecData.size());
   return Color(vecData[iIndex+2], vecData[iIndex+1], vecData[iIndex+0], 255);
}

inline Color FromData16Bit(const std::vector<BYTE>& vecData, size_t iIndex)
{
   ATLASSERT(iIndex + 1 < vecData.size());
   // 0: GGGBBBBB, 1: ARRRRRGG
   return Color(
      (vecData[iIndex+1] & 0x83) << 1, // red
      ((vecData[iIndex+1] & 3) << 6) | ((vecData[iIndex+0] & 0xe0) >> 2), // green
      (vecData[iIndex+0] & 0x1f) << 3, // blue
      (vecData[iIndex+1] & 0x80) != 0 ? 255 : 0 // alpha
   );
}

void TgaImageReader::LoadColorMap(Stream::IStream& stream,
   WORD wColorMapFirstIndex, WORD wColorMapLength, BYTE bColorMapBits)
{
   ATLASSERT(bColorMapBits == 16 || bColorMapBits == 24 || bColorMapBits == 32);

   // resize so that entries before first index are black
   if (wColorMapFirstIndex > 0)
      m_vecColorMap.resize(wColorMapFirstIndex, Color::Black());

   if (wColorMapLength == 0)
      return; // no color map to read

   // determine number of values to read
   DWORD dwSizeToRead = wColorMapLength * (bColorMapBits >> 3);

   std::vector<BYTE> vecData(dwSizeToRead);

   DWORD dwBytesRead = 0;
   stream.Read(&vecData[0], dwSizeToRead, dwBytesRead);

   if (dwSizeToRead != dwBytesRead)
      throw Exception(_T("couldn't read tga file color map"), __FILE__, __LINE__);

   m_vecColorMap.reserve(wColorMapFirstIndex + wColorMapLength);

   size_t iStep = (bColorMapBits >> 3);

   switch (iStep)
   {
   case 4:
      for (size_t i = 0, iMax = wColorMapLength * iStep; i<iMax; i += iStep)
         m_vecColorMap.push_back(FromData32Bit(vecData, i));
      break;

   case 3:
      for (size_t i = 0, iMax = wColorMapLength * iStep; i<iMax; i += iStep)
         m_vecColorMap.push_back(FromData24Bit(vecData, i));
      break;

   case 2:
      for (size_t i = 0, iMax = wColorMapLength * iStep; i<iMax; i += iStep)
         m_vecColorMap.push_back(FromData16Bit(vecData, i));
      break;
   };

   // resize color map so that it contains as many entries as bytes
   if (m_vecColorMap.size() < 256)
      m_vecColorMap.resize(256 - m_vecColorMap.size(), Color::Black());
}

void TgaImageReader::LoadColorMapPixels(Stream::IStream& stream, DWORD dwLength)
{
   m_vecPixels.resize(dwLength);

   std::vector<BYTE> vecData(dwLength);

   DWORD dwBytesRead = 0;
   stream.Read(&vecData[0], dwLength, dwBytesRead);

   if (dwLength != dwBytesRead)
      throw Exception(_T("couldn't read tga file pixels"), __FILE__, __LINE__);

   for (DWORD dw=0; dw<dwLength; dw++)
      m_vecPixels[dw] = m_vecColorMap[vecData[dw]];
}

void TgaImageReader::LoadTrueColorPixels(Stream::IStream& stream, DWORD dwLength, BYTE bBitsPerPixel)
{
   ATLASSERT(bBitsPerPixel == 16 || bBitsPerPixel == 24 || bBitsPerPixel == 32);

   m_vecPixels.resize(dwLength);

   DWORD dwStep = (bBitsPerPixel >> 3);
   DWORD dwBytesToRead = dwLength*dwStep;
   std::vector<BYTE> vecData(dwBytesToRead);

   DWORD dwBytesRead = 0;
   stream.Read(&vecData[0], dwBytesToRead, dwBytesRead);

   if (dwBytesToRead != dwBytesRead)
      throw Exception(_T("couldn't read tga file pixels"), __FILE__, __LINE__);

   switch (dwStep)
   {
   case 4:
      for (DWORD dw=0; dw<dwLength; dw++)
         m_vecPixels[dw] = FromData32Bit(vecData, dw<<2);
      break;

   case 3:
      for (DWORD dw=0; dw<dwLength; dw++)
         m_vecPixels[dw] = FromData24Bit(vecData, (dw<<1)+dw);
      break;

   case 2:
      for (DWORD dw=0; dw<dwLength; dw++)
         m_vecPixels[dw] = FromData16Bit(vecData, dw<<1);
      break;
   };
}

/*
case 10: // rle-compressed rgb image
{
   int size=0; // size in color value

   while (size<xres)
   {
      // raw bytes available?
      while (rawcount>0 && size<xres)
      {
         // read in color values
         int take = rawcount>xres-size ? xres-size : rawcount;
         istr.read(reinterpret_cast<char*>(scanline+size*bytesperpixel),take*bytesperpixel);
         size+=take;
         rawcount-=take;
      }

      // repeat color values?
      while(repeatcount>0 && size<xres)
      {
         memcpy(scanline+size*bytesperpixel,repeatbytes,bytesperpixel);
         size++;
         repeatcount--;
      }

      if (size<xres)
      {
         int next = istr.get();
         if (!istr.good() || next==EOF || next>255) // should never happen
            return false;

         if ((next & 0x80) != 0)
         {
            // repeat value
            repeatcount = (next & 0x7f)+1;
            istr.read(reinterpret_cast<char*>(repeatbytes),bytesperpixel);
         }
         else
            // raw count
            rawcount = (next & 0x7f)+1;
      }
   }

}
break;
*/
