//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file PNG.hpp PNG format loading
//
#pragma once

// includes
#include <vector>
#include "Color.hpp"
#include "ZlibDecompressor.hpp"

// forward references
namespace Stream
{
   class IStream;
}

/// \brief PNG decoding
/// \details based on my project "altpng", written 2003-2004.
namespace PNG
{

/// \brief Checks PNG signature; 8 bytes needed in the buffer
bool CheckSignature(const unsigned char* buffer);

/// \brief Class for calculating a 32 bit cyclic redundancy check value
class Crc32
{
public:
   /// calculates CRC for the bytes in the buffer
   static DWORD Calc(const unsigned char* buf, size_t len) throw();

   /// updates a running CRC
   static DWORD Update(DWORD crc, const unsigned char* buf, size_t len) throw();

private:
   /// ctor; not implemented
   Crc32();

   /// initializes CRC table
   static void Init() throw();

   /// table of CRCs of all 8-bit messages
   static boost::array<DWORD, 256> m_crc32Table;

   /// indicates if the crc32 table has been computed
   static bool m_bTableCalculated;
};

/// \brief PNG chunk data
class Chunk
{
public:
   /// ctor
   Chunk()
      :m_uiCrcRead(0),
       m_uiCrcCalc(0)
   {
      memset(m_acName, 0, sizeof(m_acName));
   }

   /// loads chunk from stream
   void Load(Stream::IStream& stream);

   /// checks if chunk name is equal given name
   bool IsName(LPCSTR pszName) const throw();

   /// returns name of chunk
   LPCSTR Name() const throw() { return m_acName; }

   /// returns length of chunk data
   size_t Length() const throw() { return m_vecData.size(); }

   /// returns chunk data block
   const std::vector<BYTE>& Data() const throw() { return m_vecData; }

   /// returns if CRC32 is valid
   bool IsValidCrc32() const throw() { return m_uiCrcRead == m_uiCrcCalc; }

   /// returns CRC32 value
   DWORD Crc32() const throw() { return m_uiCrcCalc; }

   /// returns if chunk is an ancilliary one (true) or a critical (false)
   bool IsAnciliary() const throw();

   /// returns if chunk is private
   bool Private() const throw();

   /// reserved bit; must be 'false' for a valid PNG chunk
   bool Reserved() const throw();

   /// returns if chunk is safe to copy
   bool SafeToCopy() const throw();

private:
   /// calculates CRC32 of chunk name and data
   DWORD CalcCrc32() const throw();

private:
   CHAR m_acName[5];    ///< chunk name
   DWORD m_uiCrcRead;   ///< CRC32 as read from stream
   DWORD m_uiCrcCalc;   ///< calculated CRC32
   std::vector<BYTE> m_vecData;  ///< chunk data
};

/// PNG image info
struct ImageInfo
{
   /// parses IHDR chunk
   void ParseHeader(const Chunk& ihdr);

   /// parses PLTE chunk
   void ParsePalette(const Chunk& plte);

   /// image width
   DWORD m_uiWidth;

   /// image height
   DWORD m_uiHeight;

   /// image bit depth
   BYTE m_bitDepth;

   /// image color types
   enum T_enColorType
   {
      colorTypeGrayscale = 0,       ///< grayscale image
      colorTypeTruecolor = 2,       ///< RGB truecolor image
      colorTypeIndexed = 3,         ///< palette indexed image
      colorTypeGrayscaleAlpha = 4,  ///< grayscale with alpha channel
      colorTypeTruecolorAlpha = 6,  ///< RGBA truecolor with alpha channel
   };
   T_enColorType m_enColorType;  ///< color type

   /// compression method types
   enum T_enCompressionMethod
   {
      compressionDeflate = 0, ///< deflate/inflate compression method
   };
   T_enCompressionMethod m_enCompressionMethod; ///< compression method

   /// filter method types
   enum T_enFilterMethod
   {
      filterAdaptive = 0, ///< adaptive filtering with 5 different filters
   };
   T_enFilterMethod m_enFilterMethod; ///< filter method

   /// \brief interlace method types
   enum T_enInterlaceMethod
   {
      interlaceNone = 0,   ///< no interlace
      interlaceAdam7 = 1,  ///< Adam7 interlace
   };
   T_enInterlaceMethod m_enInterlaceMethod;  ///< filter method

   /// number of bytes forming a color sample
   unsigned int m_uiBytesPerColor;

   /// palette
   std::vector<Color> m_vecPalette;

private:
   /// checks if image info values are valid
   bool CheckValid() const throw();
};


/// \brief scanline reading and decoding
struct Scanline
{
public:
   /// ctor
   Scanline()
      :m_uiScanlineFill(0),
       m_uiColorSamples(0),
       m_uiInterlacePass(0),
       m_decompressor(true) // true: read zlib window size from stream
   {
   }

   /// decodes scanline, given zlib encoded data; returns true on complete scanline
   bool Decode(const BYTE* pbData, size_t uiLen,
      const ImageInfo& imageInfo, size_t& uiBytesUsed);

   /// returns scanline data
   const std::vector<BYTE>& Data() const throw() { return m_vecScanline; }

   /// converts to RGBA data, possibly converting from palette or grayscale mode; not suitable for 16-bit images
   void ReadRGBA(const ImageInfo& imageInfo, std::vector<Color>& vecColorData, size_t offset) const;

private:
   /// filters current scanline
   void Filter(unsigned int uiBytesPerColor);

private:
   /// number of color samples in scanline; may vary when image is interlaced
   unsigned int m_uiColorSamples;

   /// current scanline data
   std::vector<BYTE> m_vecScanline;

   /// number of bytes already filled in scanline buffer
   size_t m_uiScanlineFill;

   /// filter types
   enum T_enFilterType
   {
      filterNone = 0,
      filterSub = 1,
      filterUp = 2,
      filterAverage = 3,
      filterPaeth = 4,

      filterMax = 4
   };
   T_enFilterType m_enFilterType;   ///< current filter type

   /// last scanline data, already filtered
   std::vector<BYTE> m_vecLastScanline;

   /// indicates number of current interlace pass
   unsigned int m_uiInterlacePass;

   /// zlib data stream
   ZlibDecompressor m_decompressor;
};

/// high-level PNG image decoder
class Decoder
{
public:
   /// ctor
   Decoder(Stream::IStream& stream) throw()
      :m_stream(stream),
       m_bHeaderDecoded(false),
       m_uiCurrentLine(0)
   {
   }
   /// dtor
   virtual ~Decoder() throw() {}

   /// decodes header so that image info is valid
   void DecodeHeader();

   /// decodes image; decodes header if not already done
   void DecodeImage(bool bAllocImageMemory);

   /// returns image info
   const ImageInfo& Info() const throw() { return m_imageInfo; }

   /// returns image as RGBA array; only when bAllocImageMemory is true in DecodeImage()
   const std::vector<Color>& Image() const throw() { return m_vecImage; }
   /// returns image as RGBA array; non-const version
   std::vector<Color>& Image() throw() { return m_vecImage; }

protected:
   /// called on next chunk loaded
   virtual bool OnNextChunk(const Chunk& ch);

   /// called on next scanline decoded
   virtual void OnScanline(unsigned int uiLine, const Scanline& scanline);

private:
   /// decodes data from a single IDAT chunk
   void DecodeData(const std::vector<BYTE>& vecData);

private:
   /// data stream
   Stream::IStream& m_stream;

   /// indicates if header was already decoded
   bool m_bHeaderDecoded;

   /// image info
   ImageInfo m_imageInfo;

   /// data not yet processed
   std::vector<BYTE> m_vecDataBuffer;

   /// current scanline
   unsigned int m_uiCurrentLine;

   /// current scanline
   Scanline m_scanline;

   /// RGBA decoded image
   std::vector<Color> m_vecImage;
};

} // namespace PNG
