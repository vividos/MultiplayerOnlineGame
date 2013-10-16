//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file PNG.cpp PNG format loading
//

// includes
#include "StdAfx.h"
#include "PNG.hpp"
#include <ulib/stream/EndianAwareFilter.hpp>
#include <ulib/stream/MemoryStream.hpp>
#include <boost/array.hpp>

using namespace PNG;

/// PNG file signature bytes
static const unsigned char c_acSignature[8] =
{
   137, 80, 78, 71, 13, 10, 26, 10
};

/// \param buffer buffer with at least 8 bytes from the start of the file
/// \retval true signature is ok
/// \retval false signature not ok
bool PNG::CheckSignature(const unsigned char* buffer)
{
   return memcmp(buffer,c_acSignature,8) == 0;
}


//
// Crc32
//

/// \class Crc32
/// \details
/// The following CRC32 code is taken from the PNG specification
/// and slightly modified in terms of variable and function names.
///
/// The following sample code represents a practical implementation
/// of the CRC (Cyclic Redundancy Check) employed in PNG chunks. (See
/// also ISO 3309 [ISO-3309] or ITU-T V.42 [ITU-T-V42] for a formal
/// specification.)

bool Crc32::m_bTableCalculated = false;

boost::array<DWORD, 256> Crc32::m_crc32Table;

void Crc32::Init() throw()
{
   for (size_t n = 0; n < 256; n++)
   {
      DWORD c = (DWORD)n;
      for (int k = 0; k < 8; k++)
      {
         if (c & 1)
            c = 0xedb88320L ^ (c >> 1);
         else
            c = c >> 1;
      }
      m_crc32Table[n] = c;
   }

   m_bTableCalculated = true;
}

/// \param buf buffer with bytes to calculate CRC from
/// \param len number of bytes to use for CRC calculation, in bytes
/// \return calculated CRC value
DWORD Crc32::Calc(const unsigned char* buf, size_t len) throw()
{
   return Update(0, buf, len);
}

/// Update a running CRC with the bytes buf[0..len-1]--the CRC
/// should be initialized to all 1's, and the transmitted value
/// is the 1's complement of the final running CRC.
/// \param crc running crc value calculated in a previous run of Calc() or Update()
/// \param buf buffer with more bytes to calculate CRC from
/// \param len number of bytes to use for CRC calculation, in bytes
/// \return calculated CRC value
DWORD Crc32::Update(DWORD crc, const unsigned char* buf, size_t len) throw()
{
   if (!m_bTableCalculated)
      Init();

   crc ^= 0xffffffffL;

   for (size_t n = 0; n < len; n++)
      crc = m_crc32Table[(crc ^ buf[n]) & 0xff] ^ (crc >> 8);

   return crc ^ 0xffffffffL;
}


//
// Chunk
//

bool Chunk::IsName(LPCSTR pszName) const throw()
{
   ATLASSERT(strlen(pszName) == 4);
   return memcmp(m_acName, pszName, 4) == 0;
}

void Chunk::Load(Stream::IStream& stream)
{
   Stream::EndianAwareFilter filter(stream);

   DWORD dwLength = filter.Read32BE();

   DWORD dwBytesRead = 0;
   stream.Read(m_acName, 4, dwBytesRead);
   ATLASSERT(dwBytesRead == 4);

   if (dwLength > 0)
   {
      m_vecData.resize(dwLength);

      dwBytesRead = 0;
      stream.Read(&m_vecData[0], dwLength, dwBytesRead);
      ATLASSERT(dwBytesRead == dwLength);
   }

   m_uiCrcRead = filter.Read32BE();
   m_uiCrcCalc = CalcCrc32();
}

bool Chunk::IsAnciliary() const throw()
{
   return ((m_acName[0] >> 5) & 1) != 0;
}

bool Chunk::Private() const throw()
{
   return ((m_acName[1] >> 5) & 1) != 0;
}

bool Chunk::Reserved() const throw()
{
   return ((m_acName[2] >> 5) & 1) != 0;
}

bool Chunk::SafeToCopy() const throw()
{
   return ((m_acName[3] >> 5) & 1) != 0;
}

DWORD Chunk::CalcCrc32() const throw()
{
   DWORD crc32 = Crc32::Calc(reinterpret_cast<const unsigned char*>(&m_acName[0]), 4);
   if (!m_vecData.empty())
      crc32 = Crc32::Update(crc32, &m_vecData[0], m_vecData.size());

   return crc32;
}

//
// ImageInfo
//

void ImageInfo::ParseHeader(const Chunk& ihdr)
{
   ATLASSERT(ihdr.IsName("IHDR") && ihdr.Length() == 13);

   // retrieve image info values from the header
   const std::vector<BYTE>& vecData = ihdr.Data();

   Stream::MemoryStream ms(&vecData[0], 8);
   Stream::EndianAwareFilter filter(ms);

   m_uiWidth = filter.Read32BE();
   m_uiHeight = filter.Read32BE();

   m_bitDepth = vecData[8];
   m_enColorType = static_cast<T_enColorType>(vecData[9]);
   m_enCompressionMethod = static_cast<T_enCompressionMethod>(vecData[10]);
   m_enFilterMethod = static_cast<T_enFilterMethod>(vecData[11]);
   m_enInterlaceMethod = static_cast<T_enInterlaceMethod>(vecData[12]);

   if (!CheckValid())
      throw Exception(_T("invalid PNG header infos"), __FILE__, __LINE__);

   // calculate number of bytes forming a color sample
   unsigned int bitsPerValue = m_bitDepth;
   unsigned int valuesPerColor = 0;

   switch (m_enColorType)
   {
   case colorTypeGrayscale: // grayscale sample
      valuesPerColor = 1;
      break;
   case colorTypeTruecolor: // RGB triple
      valuesPerColor = 3;
      break;
   case colorTypeIndexed: // palette indexed
      valuesPerColor = 1;
      bitsPerValue = 8;
      break;
   case colorTypeGrayscaleAlpha: // grayscale with alpha sample
      valuesPerColor = 2;
      break;
   case colorTypeTruecolorAlpha: // RGB triple with alpha
      valuesPerColor = 4;
      break;
   default:
      throw Exception(_T("invalid PNG color type"), __FILE__, __LINE__);
   }

   m_uiBytesPerColor = (bitsPerValue * valuesPerColor) >> 3;

   // check images with multiple pixels per byte
   if (m_uiBytesPerColor == 0)
      throw Exception(_T("PNG images with bits per pixel < 8 not supported"), __FILE__, __LINE__);
}

bool ImageInfo::CheckValid() const throw()
{
   if (m_uiWidth == 0 || m_uiHeight == 0)
      return false;

   // check color types and bit depths
   switch (m_enColorType)
   {
   case colorTypeGrayscale: // allowed: 1,2,4,8,16
      if (m_bitDepth != 1 &&
          m_bitDepth != 2 &&
          m_bitDepth != 4 &&
          m_bitDepth != 8 &&
          m_bitDepth != 16)
         return false;
      break;

   case colorTypeIndexed: // allowed: 1,2,4,8
      if (m_bitDepth != 1 &&
          m_bitDepth != 2 &&
          m_bitDepth != 4 &&
          m_bitDepth != 8)
         return false;
      break;

   case colorTypeTruecolor: // allowed: 8,16
   case colorTypeGrayscaleAlpha:
   case colorTypeTruecolorAlpha:
      if (m_bitDepth != 8 &&
          m_bitDepth != 16)
         return false;
      break;

   default: // unknown color type
      return false;
   }

   // check for invalid enum values

   if (m_enCompressionMethod != ImageInfo::compressionDeflate)
      return false;

   if (m_enFilterMethod != ImageInfo::filterAdaptive)
      return false;

   if (m_enInterlaceMethod != ImageInfo::interlaceNone &&
       m_enInterlaceMethod != ImageInfo::interlaceAdam7)
      return false;

   return true;
}

void ImageInfo::ParsePalette(const Chunk& plte)
{
   if (!m_vecPalette.empty() ||
       m_enColorType == colorTypeGrayscale ||
       m_enColorType == colorTypeGrayscaleAlpha)
      throw Exception(_T("invalid PNG file; unexpected PLTE chunk"), __FILE__, __LINE__);

   const std::vector<BYTE>& vecData = plte.Data();
   if (vecData.size() % 3 != 0 ||
       vecData.size() > ((1U << m_bitDepth)*3))
      throw Exception(_T("invalid PNG file; PLTE chunk too large"), __FILE__, __LINE__);

   m_vecPalette.resize(vecData.size() / 3);

   for (size_t i=0, iMax=m_vecPalette.size(); i<iMax; i++)
   {
      m_vecPalette[i] = Color(
         vecData[i*3+0],
         vecData[i*3+1],
         vecData[i*3+2],
         255);
   }
}

//
// Scanline
//

bool Scanline::Decode(const BYTE* pbData, size_t uiLen,
   const ImageInfo& imageInfo, size_t& uiBytesUsed)
{
   // calculate length of a raw line
   // \todo take adam7 interlacing into account
   // \todo set interlace pass according to pass
   switch (imageInfo.m_enInterlaceMethod)
   {
   case ImageInfo::interlaceNone:
      m_uiColorSamples = imageInfo.m_uiWidth;
      break;

   case ImageInfo::interlaceAdam7:
      throw Exception(_T("Adam7 interlacing not supported"), __FILE__, __LINE__);

   default:
      ATLASSERT(false); // shouldn't get here, CheckValid() should abort before
   }

   if (!m_vecLastScanline.empty() && m_uiScanlineFill == 0)
   {
      // remember last scanline
      std::swap(m_vecScanline, m_vecLastScanline);
   }

   // alloc memory for last scanline, if needed
   if (m_vecLastScanline.empty())
   {
      m_vecLastScanline.resize(imageInfo.m_uiWidth * imageInfo.m_uiBytesPerColor, 0);
   }

   // alloc memory for scanline, if needed
   if (m_vecScanline.empty())
   {
      m_vecScanline.resize(imageInfo.m_uiWidth * imageInfo.m_uiBytesPerColor, 0);
   }

   // step 1: read more bytes by uncompressing data
   uiBytesUsed = 0;

   size_t uiUnusedInBytes = 0;
   size_t uiMaxUncompress = m_vecScanline.size() - m_uiScanlineFill + 1;

   std::vector<BYTE> vecUncompressedData;
   /*bool bRet = */m_decompressor.Uncompress(pbData,
      uiLen,
      uiUnusedInBytes,
      vecUncompressedData,
      uiMaxUncompress);
   // TODO bRet
   /// \retval true end of stream; last bytes were decoded
   /// \retval false more bytes available to uncompress

   // update in ptr
   uiBytesUsed += uiLen - uiUnusedInBytes;
   pbData += uiBytesUsed;
   uiLen = uiUnusedInBytes;

   if (!vecUncompressedData.empty())
   {
      // first byte is filter type
      if (m_uiScanlineFill == 0)
      {
         m_enFilterType = static_cast<T_enFilterType>(vecUncompressedData[0]);
         if (m_enFilterType > filterMax)
            throw Exception(_T("invalid PNG scanline filter type"), __FILE__, __LINE__);
      }

      // copy remaining bytes to scanline
      ATLASSERT(vecUncompressedData.size() - 1 <= m_vecScanline.size() - m_uiScanlineFill);

      std::copy(vecUncompressedData.begin() + 1, vecUncompressedData.end(),
         m_vecScanline.begin() + m_uiScanlineFill);

      m_uiScanlineFill += vecUncompressedData.size() - 1;
   }

   if (m_uiScanlineFill < imageInfo.m_uiWidth * imageInfo.m_uiBytesPerColor)
      return false; // still need more bytes

   // step 2: now that we have data, we can filter the scanline
   if (m_enFilterType != filterNone)
   {
      ATLASSERT(imageInfo.m_enFilterMethod == ImageInfo::filterAdaptive);

      Filter(imageInfo.m_uiBytesPerColor);
   }

   m_uiScanlineFill = 0;

   return true;
}

/// helper function for paeth predictor filter
int PaethPredictor(int a, int b, int c)
{
   /* a = left, b = above, c = upper left */
   int p, pa, pb, pc;

   p = a + b - c;    /* initial estimate */
   pa = abs(p - a);  /* distances to a, b, c */
   pb = abs(p - b);
   pc = abs(p - c);

   pa = abs(b-c);
   pb = abs(a-c);
   pc = abs(a+b-c-c);

   /* return nearest of a,b,c, breaking ties in order a,b,c. */
   return pa<=pb && pa<=pc ? a : ( pb<=pc ? b : c);
}

/// \note after filtering, filter type of scanline is set to filterNone again.
/// the last scanline must be a scanline which is already of filter type filterNone.
/// last scanline must not be empty.
void Scanline::Filter(unsigned int uiBytesPerColor)
{
   ATLASSERT(!m_vecScanline.empty());
   ATLASSERT(!m_vecLastScanline.empty());

   ATLASSERT(m_enFilterType <= filterMax);

   if (m_enFilterType == filterNone)
      return; // nothing to do

   unsigned int numPixels = m_uiColorSamples * uiBytesPerColor;

   for (unsigned int i=0; i<uiBytesPerColor; i++)
   {
      for (unsigned int j=i; j<numPixels; j += uiBytesPerColor)
      {
         unsigned char left = j<uiBytesPerColor ? 0 : m_vecScanline[j-uiBytesPerColor];
         unsigned char prior = m_vecLastScanline[j];
         int val = m_vecScanline[j];

         switch (m_enFilterType)
         {
         case filterSub:
            val += left;
            break;

         case filterUp:
            val += prior;
            break;

         case filterAverage:
            val += ((left+prior)>>1);
            break;

         case filterPaeth: // paeth predictor
            {
               unsigned char upperLeft = j < uiBytesPerColor ? 0 : m_vecLastScanline[j-uiBytesPerColor];

               int paeth = PaethPredictor(left, prior, upperLeft);
               val += paeth;
            }
            break;
         }

         m_vecScanline[j] = val & 0xff;
      }
   }

   m_enFilterType = filterNone;
}

void Scanline::ReadRGBA(const ImageInfo& imageInfo, std::vector<Color>& vecScanline, size_t offset) const
{
   switch (imageInfo.m_enColorType)
   {
   case ImageInfo::colorTypeGrayscale: // allowed: 1,2,4,8,16
   case ImageInfo::colorTypeIndexed: // allowed: 1,2,4,8
   case ImageInfo::colorTypeGrayscaleAlpha: // allowed: 8,16
      throw Exception(_T("unsupported PNG color type, in ReadRGBA()"), __FILE__, __LINE__);

   case ImageInfo::colorTypeTruecolor: // allowed: 8,16
      if (imageInfo.m_bitDepth == 16)
         throw Exception(_T("unsupported PNG bit depth for truecolor types, in ReadRGBA()"), __FILE__, __LINE__);

      ATLASSERT(imageInfo.m_uiBytesPerColor == 3);
      for (size_t iTarget=offset, iSrc=0, iTargetMax=imageInfo.m_uiWidth+offset; iTarget<iTargetMax; iTarget++, iSrc+=3)
      {
         vecScanline[iTarget] = Color(
            m_vecScanline[iSrc+0],
            m_vecScanline[iSrc+1],
            m_vecScanline[iSrc+2],
            255);
      }
      break;

   case ImageInfo::colorTypeTruecolorAlpha: // allowed: 8,16
      if (imageInfo.m_bitDepth == 16)
         throw Exception(_T("unsupported PNG bit depth for truecolor types, in ReadRGBA()"), __FILE__, __LINE__);

      {
         ATLASSERT(imageInfo.m_uiBytesPerColor == 4);
         const Color* pc = reinterpret_cast<const Color*>(&m_vecScanline[0]);
         std::copy(pc, pc + imageInfo.m_uiWidth,
            vecScanline.begin() + offset);
      }
      break;
   }
}


//
// Decoder
//

void Decoder::DecodeHeader()
{
   // read signature
   BYTE abSignature[8];

   DWORD dwBytesRead = 0;
   m_stream.Read(abSignature, 8, dwBytesRead);
   if (dwBytesRead != 8)
      throw Exception(_T("invalid PNG file length"), __FILE__, __LINE__);

   if (!CheckSignature(abSignature))
      throw Exception(_T("invalid PNG signature"), __FILE__, __LINE__);

   // read first chunk; must be a IHDR
   Chunk ihdr;
   ihdr.Load(m_stream);

   if (!ihdr.IsName("IHDR") || ihdr.Length() != 13 || !ihdr.IsValidCrc32())
      throw Exception(_T("invalid PNG file; invalid IHDR chunk"), __FILE__, __LINE__);

   m_imageInfo.ParseHeader(ihdr);

   m_bHeaderDecoded = true;
}

void Decoder::DecodeImage(bool bAllocImageMemory)
{
   if (!m_bHeaderDecoded)
      DecodeHeader();

   if (bAllocImageMemory)
      m_vecImage.resize(m_imageInfo.m_uiHeight * m_imageInfo.m_uiWidth);

   while (!m_stream.AtEndOfStream())
   {
      Chunk ch;
      ch.Load(m_stream);

      if (!ch.IsValidCrc32())
         throw Exception(_T("invalid PNG chunk; wrong CRC32 checksum"), __FILE__, __LINE__);

      if (!OnNextChunk(ch))
         break;
   }
}

/// \details override this virtual function when you want to get notified
/// about chunks being read; call the base class function when you also
/// want to decode scanlines; see OnScanline function.
bool Decoder::OnNextChunk(const Chunk& ch)
{
   if (ch.IsName("PLTE"))
   {
      m_imageInfo.ParsePalette(ch);
   }
   else
   if (ch.IsName("IDAT"))
   {
      DecodeData(ch.Data());
   }
   else
   if (ch.IsName("IEND"))
   {
      if (ch.Length() != 0)
         throw Exception(_T("invalid PNG file; invalid IEND chunk"), __FILE__, __LINE__);

      // decoded all scanlines?
      if (m_uiCurrentLine != m_imageInfo.m_uiHeight)
         throw Exception(_T("invalid PNG file; height mismatch"), __FILE__, __LINE__);

      // buffer still has data?
      if (!m_vecDataBuffer.empty())
         throw Exception(_T("invalid PNG file; remaining data after end"), __FILE__, __LINE__);

      return false;
   }
   else
      ATLTRACE(_T("ignored chunk: %hs\n"), ch.Name());

   return true;
}

void Decoder::DecodeData(const std::vector<BYTE>& vecData)
{
   m_vecDataBuffer.insert(m_vecDataBuffer.end(), vecData.begin(), vecData.end());

   size_t uiCurrentIndex = 0, uiBytesUsed;
   bool bRet = true;
   do
   {
      // decode scanline until more data is needed
      bRet = m_scanline.Decode(
         &m_vecDataBuffer[uiCurrentIndex],
         m_vecDataBuffer.size()-uiCurrentIndex,
         m_imageInfo,
         uiBytesUsed);

      uiCurrentIndex += uiBytesUsed;

      // call scanline callback
      if (bRet)
      {
         OnScanline(m_uiCurrentLine, m_scanline);

         m_uiCurrentLine++;
      }
      else
      {
         // move remaining data to front, if not empty
         if (uiCurrentIndex >= m_vecDataBuffer.size())
         {
            m_vecDataBuffer.clear();
         }
         else
         {
            m_vecDataBuffer.assign(
               m_vecDataBuffer.begin() + uiCurrentIndex,
               m_vecDataBuffer.end());
         }
      }
   } while (bRet &&
      m_uiCurrentLine < m_imageInfo.m_uiHeight &&
      uiCurrentIndex < m_vecDataBuffer.size());

   if (uiCurrentIndex >= m_vecDataBuffer.size())
      m_vecDataBuffer.clear();
}

/// \details override this virtual function when you want to get notified
/// about scanlines being decoded; call the base class function when you also
/// want to let the decoder store the data as RGBA values in the Decoder
/// class.
void Decoder::OnScanline(unsigned int uiLine, const Scanline& scanline)
{
   ATLASSERT(!m_vecImage.empty()); // did you pass true to DecodeImage(bool bAllocImageMemory) ?

   size_t offset = m_imageInfo.m_uiWidth * uiLine;

   scanline.ReadRGBA(m_imageInfo, m_vecImage, offset);
}
