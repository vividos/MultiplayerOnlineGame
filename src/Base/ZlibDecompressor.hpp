//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ZlibDecompressor.hpp zlib inflate decompressor
//
#pragma once

// includes
#include <vector>

// forward references
/// zlib stream struct
struct z_stream_s;

/// zlib inflate decompressor
class BASE_DECLSPEC ZlibDecompressor
{
public:
   /// ctor; opens stream
   ZlibDecompressor(bool bWindowSizeFromStream = false);

   /// dtor; closes stream
   ~ZlibDecompressor() throw();

   /// uncompresses more bytes
   /// \retval true end of stream; last bytes were decoded
   /// \retval false more bytes available to uncompress
   bool Uncompress(const BYTE* pbData, size_t uiLength, size_t& uiUnusedInBytes,
      std::vector<BYTE>& vecUncompressedData, size_t uiMaxUncompress);

   /// returns number of compressed bytes used so far
   unsigned int TotalIn() const throw();

   /// returns number of uncompressed bytes already produced
   unsigned int TotalOut() const throw();

   /// check version; throws exception on wrong zlib1.dll
   static void CheckVersion();

   /// returns zlib version string
   static CString ZlibVersion() throw();

private:
   /// zlib stream
   boost::shared_ptr<z_stream_s> m_spStream;
};
