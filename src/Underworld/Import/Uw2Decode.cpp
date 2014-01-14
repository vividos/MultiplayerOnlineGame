//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Import/Uw2Decode.cpp Ultima Underworld 2 .ark compression format decoder
//

// includes
#include "stdafx.h"
#include "Uw2Decode.hpp"
#include <ulib/stream/MemoryStream.hpp>

/// \brief Data decoding for uw2 compression format
/// See uw-formats.txt for a detailed description how the data is compressed.
/// The code was adapted from the LoW project: http://low.sourceforge.net/
Uint32 Uw2DecodeData(const std::vector<Uint8>& vecSourceData, Uint8* pDestBuffer, Uint32 uiDestSize)
{
   Uint32 uiDestCount = 0;

   if (pDestBuffer == NULL)
      uiDestSize = 1; // when not passing a buffer, just set dest size to 1 to keep loop running

   // compressed data
   Uint8* ubuf = pDestBuffer;
   const Uint8* cp = &vecSourceData[4]; // for some reason the first 4 bytes are not used
   const Uint8* ce = &vecSourceData[vecSourceData.size()];
   Uint8* up = ubuf;
   Uint8* ue = ubuf+uiDestSize;

   while (up < ue && cp < ce)
   {
      unsigned char bits = *cp++;
      //UaTrace(" bits: %02x\n",bits);

      for (int i=0; i<8; i++, bits >>= 1)
      {
         if (bits&1)
         {
            //UaTrace("copy byte: %02x (to %04x)\n",*cp,up-ubuf);
            if (up != NULL)
               *up++ = *cp++;
            else
               cp++;

            uiDestCount++;
         }
         else
         {
            signed int m1 = *cp++; // m1: pos
            signed int m2 = *cp++; // m2: run

            m1 |= (m2&0xF0) << 4;

            // correct for sign bit
            if (m1&0x800)
               m1 |= 0xFFFFF000;

            // add offsets
            m2 =  (m2&0x0F) + 3;
            m1 += 18;

            uiDestCount += m2; // add count

            if (up != NULL)
            {
               if (m1 > up-ubuf)
                  throw Exception(_T("Uw2DecodeData: pos exceeds buffer!"), __FILE__, __LINE__);

               // adjust pos to current 4k segment
               while (m1 < (up-ubuf-0x1000))
                  m1 += 0x1000;

               //UaTrace("copy from: pos: %03x, run: %02x\n",m1,m2);

               while (m2-- && up<ue)
                  *up++ = ubuf[m1++];
            }
         }

         // stop if source or destination buffer pointer overrun
         if (up >= ue || cp >= ce)
            break;
      }
   }

//   UaTrace(" decoded %04x compressed bytes to %04x uncompressed data.\n",
//      cp - &vecSourceData[4], uiDestCount);
   return uiDestCount;
}

/// Reads in compressed blocks from uw2 .ark files and creates a file with
/// decoded data. Note that the given file must already be at the proper block
/// start position.
/// \param spStream file stream in .ark format, at the block start position
/// \param bCompressed indicates if block is actually compressed
/// \param uiDataSize number of source bytes in block (either compressed or uncompressed)
std::shared_ptr<Stream::IStream> Import::Uw2Decode(std::shared_ptr<Stream::IStream> spStream, bool bCompressed, Uint32 uiDataSize)
{
   UaAssert(uiDataSize > 0); // trying to load entry that has size 0?

   // read in source data
   std::vector<Uint8> vecSourceData;
   vecSourceData.resize(uiDataSize);

   std::vector<Uint8> vecDestBuffer;
   Uint32 uiDestSize = uiDataSize;

   if (bCompressed)
   {
      // read in data
      DWORD dwBytesRead = 0;
      spStream->Read(&vecSourceData[0], uiDataSize, dwBytesRead);

      // find out decoded length and allocate memory
      uiDestSize = Uw2DecodeData(vecSourceData, NULL, 0);

      vecDestBuffer.resize(uiDestSize);

      // decode it
      ATLVERIFY(uiDestSize == Uw2DecodeData(vecSourceData, &vecDestBuffer[0], uiDestSize));
   }
   else
   {
      // just copy the data
      vecDestBuffer.resize(uiDataSize);

      DWORD dwBytesRead = 0;
      spStream->Read(&vecDestBuffer[0], uiDataSize, dwBytesRead);
   }

   // create memory stream from decoded data
   std::shared_ptr<Stream::MemoryStream> spDecoded(
      new Stream::MemoryStream(vecDestBuffer.data(), vecDestBuffer.size()));

   return spDecoded;
}
