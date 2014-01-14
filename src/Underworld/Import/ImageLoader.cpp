//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Import/ImageLoader.hpp Image loader
//
#pragma once

// includes
#include "stdafx.h"
#include "ImageLoader.hpp"
#include "IFileSystem.hpp"
#include <ulib/stream/IStream.hpp>
#include <ulib/stream/EndianAwareFilter.hpp>

using Import::ImageLoader;

// ImageLoader methods

void ImageLoader::LoadPalettes(LPCTSTR pszAllPalettesName,
   std::array<std::shared_ptr<Palette256>, 8>& aAllPalettes)
{
   std::shared_ptr<Stream::IStream> spFile =
      m_fileSystem.OpenFile(pszAllPalettesName, true);

   // palettes are stored in ancient vga color format
   for (unsigned int pal=0; pal<8; pal++)
   {
      aAllPalettes[pal].reset(new Palette256);

      Palette256& palette = *aAllPalettes[pal];

      for (unsigned int color=0; color<256; color++)
      {
         // palette is in RGB format, and uses only 6 lower bits
         palette.Set(Uint8(color), 0, spFile->ReadByte() << 2);
         palette.Set(Uint8(color), 1, spFile->ReadByte() << 2);
         palette.Set(Uint8(color), 2, spFile->ReadByte() << 2);
         palette.Set(Uint8(color), 3, color == 0 ? 0 : 255); // index 0 is transparent
      }
   }
}

void ImageLoader::LoadAuxPalettes(LPCTSTR pszAuxPaletteName,
   Uint8 aAuxPalettes[32][16])
{
   std::shared_ptr<Stream::IStream> spFile =
      m_fileSystem.OpenFile(pszAuxPaletteName, true);

   // the standard guarantees that the [32][16] array is contiguous
   DWORD dwBytesRead = 0;
   spFile->Read(aAuxPalettes, 32*16, dwBytesRead);
}

void ImageLoader::LoadImageGrList(std::vector<IndexedImage>& vecImages,
   LPCTSTR pszImageName,
   Uint8 aAuxPalettes[32][16],
   unsigned int uiImageFrom, unsigned int uiImageTo,
   bool bPanelsGrFix)
{
   std::shared_ptr<Stream::IStream> spFile =
      m_fileSystem.OpenFile(pszImageName, true);

   // get file length
   ULONGLONG ullFileLength = spFile->Length();

   Stream::EndianAwareFilter filter(*spFile);

   // read in toc
   Uint8 id = spFile->ReadByte(); // always 1
   ATLVERIFY(id == 1);
   Uint16 uiEntries = filter.Read16LE();

   if (uiImageTo == 0)
      uiImageTo = uiEntries;

   if (uiImageFrom >= uiEntries || uiImageTo < uiImageFrom)
      return; // nothing to load

   // read in all offsets
   std::vector<Uint32> vecOffsets;
   vecOffsets.resize(uiEntries, 0);

   for (Uint16 i=0; i<uiEntries; i++)
      vecOffsets[i] = filter.Read32LE();

   for (unsigned int j=uiImageFrom; j<uiImageTo; j++)
   {
      if (vecOffsets[j] >= ullFileLength)
         continue;

      // in some files, multiple images map to one offset; check these here
      if (j > 0 && vecOffsets[j-1] == vecOffsets[j])
      {
         vecImages.push_back(vecImages.back());
         continue;
      }

      spFile->Seek(vecOffsets[j], Stream::IStream::seekBegin);

      vecImages.push_back(IndexedImage());

      IndexedImage& image = vecImages.back();

      LoadImageGrImpl(image, spFile, aAuxPalettes, bPanelsGrFix);
   }
}

void ImageLoader::LoadImageGrImpl(IndexedImage& image,
                                  std::shared_ptr<Stream::IStream> spFile,
                                  Uint8 aAuxPalettes[32][16],
                                  bool bPanelsGrFix)
{
   Uint8 type, width, height, auxpal = 0;
   Uint16 datalen;

   if (bPanelsGrFix)
   {
      // special case for "panels.gr" that lacks a normal header
      type = 4; // 8-bit uncompressed
      width = 83;
      height = 114;
      datalen = width*height;
   }
   else
   {
      // read in image header
      type = spFile->ReadByte();
      width = spFile->ReadByte();
      height = spFile->ReadByte();

      // read in auxpal, when needed
      if (type==0x08 || type==0x0a)
         auxpal = spFile->ReadByte();

      if (auxpal > 0x1f)
         auxpal=0;

      Stream::EndianAwareFilter filter(*spFile);

      // read in data length
      datalen = filter.Read16LE();
   }

   image.Create(width, height);

   switch (type)
   {
   case 0x04: // 8-bit uncompressed
      {
         unsigned int pixcount=0;

         while (datalen > 0)
         {
            unsigned int size = std::min<unsigned int>(datalen, 1024);

            DWORD dwBytesRead = 0;
            spFile->Read(&image.Pixels()[pixcount], size, dwBytesRead);

            datalen -= Uint16(size);
            pixcount += size;
         }
      }
      break;

   case 0x08: // 4-bit rle compressed
      DecodeRLEImage(spFile, &image.Pixels()[0], 4, datalen, width*height, aAuxPalettes[auxpal], 0, 0);
      break;

   case 0x0a: // 4-bit uncompressed
      {
         unsigned int pixcount=0, maxpix = width*height;
         Uint8* pal = aAuxPalettes[auxpal];
         Uint8 rawbyte;
         Uint8* pixels = &image.Pixels()[0];

         while (datalen > 0)
         {
            rawbyte = spFile->ReadByte();
            pixels[pixcount++] = pal[rawbyte >> 4];

            if (pixcount >= maxpix)
               break;

            pixels[pixcount++] = pal[rawbyte&0x0f];
            datalen--;
         }
      }
      break;
   }
}

/// decodes the underworld rle format, for word lengths up to 8 bit, and
/// stores the pixels in an array.
void ImageLoader::DecodeRLEImage(std::shared_ptr<Stream::IStream> spFile, Uint8* pixels, unsigned int bits,
   unsigned int datalen, unsigned int maxpix, unsigned char *aAuxPalettes,
   unsigned int padding, unsigned int linewidth)
{
   unsigned int linecount = 0;
   unsigned int bufpos = 0;

   // bit extraction variables
   unsigned int bits_avail = 0;
   unsigned int rawbits = 0;
   unsigned int bitmask = ((1<<bits)-1) << (8-bits);
   unsigned int nibble;

   // rle decoding vars
   unsigned int pixcount=0;
   unsigned int stage=0; // we start in stage 0
   unsigned int count=0;
   unsigned int record=0; // we start with record 0=repeat (3=run)
   unsigned int repeatcount=0;

   while (datalen>0 && pixcount<maxpix)
   {
      // get new bits
      if (bits_avail<bits)
      {
         // not enough bits available
         if (bits_avail > 0)
         {
            nibble = ((rawbits & bitmask) >> (8-bits_avail));
            nibble <<= (bits - bits_avail);
         }
         else
            nibble = 0;

         if (spFile->AtEndOfStream())
            return;

         rawbits = (unsigned int)spFile->ReadByte();

         //TRACE(_T("fgetc: %02x\n"), rawbits);

         unsigned int shiftval = 8 - (bits - bits_avail);

         nibble |= (rawbits >> shiftval);

         rawbits = (rawbits << (8-shiftval)) & 0xFF;

         bits_avail = shiftval;
      }
      else
      {
         // we still have enough bits
         nibble = (rawbits & bitmask) >> (8-bits);
         bits_avail -= bits;
         rawbits <<= bits;
      }

      //TRACE(_T("nibble: %02x\n"), nibble);

      // now that we have a nibble
      datalen--;

      switch(stage)
      {
      case 0: // we retrieve a new count
         if (nibble == 0)
            stage++;
         else
         {
            count = nibble;
            stage=6;
         }
         break;

      case 1:
         count = nibble;
         stage++;
         break;

      case 2:
         count = (count<<4) | nibble;
         if (count==0)
            stage++;
         else
            stage = 6;
         break;

      case 3:
      case 4:
      case 5:
         count = (count<<4) | nibble;
         stage++;
         break;
      }

      if (stage < 6)
         continue;

      switch (record)
      {
      case 0:
         // repeat record stage 1
         //TRACE(_T("repeat: new count: %x\n"), count);

         if (count == 1)
         {
            record=3; // skip this record; a run follows
            break;
         }

         if (count == 2)
         {
            record = 2; // multiple run records
            break;
         }

         record = 1; // read next nibble; it's the color to repeat
         continue;

      case 1:
         // repeat record stage 2
         {
            // repeat 'nibble' color 'count' times
            for(unsigned int n=0; n<count; n++)
            {
               pixels[bufpos++] = aAuxPalettes[nibble];

               // add padding space when needed
               if (padding != 0 && ++linecount >= linewidth)
               {
                  linecount = 0;
                  bufpos += padding;
               }

               if (++pixcount >= maxpix)
                  break;
            }
         }

         //TRACE(_T("repeat: wrote %x times a '%x'\n"), count, nibble);

         if (repeatcount==0)
         {
            record=3; // next one is a run record
         }
         else
         {
            repeatcount--;
            record = 0; // continue with repeat records
         }
         break;

      case 2:
         // multiple repeat stage

         // 'count' specifies the number of repeat record to appear
         //TRACE(_T("multiple repeat: %u\n"), count);
         repeatcount = count-1;
         record = 0;
         break;

      case 3:
         // run record stage 1
         // copy 'count' nibbles

         //TRACE(_T("run: count: %x\n"), count);

         record = 4; // retrieve next nibble
         continue;

      case 4:
         // run record stage 2

         // now we have a nibble to write
         pixels[bufpos++] = aAuxPalettes[nibble];
         pixcount++;

         // add padding space when needed
         if (padding != 0 && ++linecount >= linewidth)
         {
            linecount = 0;
            bufpos += padding;
         }

         if (--count==0)
         {
            //TRACE(_T("run: finished\n"));
            record = 0; // next one is a repeat again
         }
         else
            continue;
         break;
      }

      stage=0;

   } // end of while loop
}
