//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/Import/ArchiveFile.cpp Archive file (*.ark)
//
#pragma once

// includes
#include "stdafx.h"
#include "ArchiveFile.hpp"
#include "Uw2Decode.hpp"
#include <ulib/stream/IStream.hpp>
#include <ulib/stream/EndianAwareFilter.hpp>

using Import::ArchiveFile;

// ArchiveFile methods

ArchiveFile::ArchiveFile(std::shared_ptr<Stream::IStream> spFile, bool bUw2Mode)
:m_spArchiveFile(spFile),
 m_bUw2Mode(bUw2Mode)
{
   Stream::EndianAwareFilter filter(*m_spArchiveFile);

   Uint16 uiCount = filter.Read16LE();

   if (m_bUw2Mode)
      filter.Read32LE(); // extra Int32 in uw2 mode

   // read in all offsets
   m_vecOffsets.resize(uiCount);

   for (Uint16 ui=0; ui<uiCount; ui++)
      m_vecOffsets[ui] = filter.Read32LE();

   // read in extended tables, in uw2 mode
   if (m_bUw2Mode)
   {
      m_vecInfos.resize(uiCount);

      Uint16 ui;
      for (ui=0; ui<uiCount; ui++)
      {
         Uint32 uiFlags = filter.Read32LE();
         //UaAssert((uiFlags & 1) != 0); // flag is always set, except for scd.ark
         m_vecInfos[ui].m_bCompressed = (uiFlags & 2) != 0;
         m_vecInfos[ui].m_bAllocatedExtraSpace = (uiFlags & 4) != 0;
      }

      for (ui=0; ui<uiCount; ui++)
         m_vecInfos[ui].m_uiDataSize = filter.Read32LE();

      for (ui=0; ui<uiCount; ui++)
         m_vecInfos[ui].m_uiAvailSize = filter.Read32LE();
   }
}

/// Archive file entry is available when the file offset is not 0.
/// In uw2 the data size must be greater than 0, too.
bool ArchiveFile::IsAvailable(unsigned int uiIndex) const
{
   UaAssert(uiIndex < GetNumFiles());
   if (!m_bUw2Mode)
      return m_vecOffsets[uiIndex] != 0;

   return m_vecOffsets[uiIndex] != 0 && m_vecInfos[uiIndex].m_uiDataSize > 0;
}

/// Returns file from archive; note that this function returns a SDL_RWops
/// pointer that may depend on the archive file's internal SDL_RWops struct.
/// Only use one archive file pointer at one time!
/// \todo improve uw1 mode with finding out length using offsets
std::shared_ptr<Stream::IStream> ArchiveFile::GetFile(unsigned int uiIndex)
{
   UaAssert(uiIndex < GetNumFiles());
   UaAssert(true == IsAvailable(uiIndex));

   // seek to block
   m_spArchiveFile->Seek(m_vecOffsets[uiIndex], Stream::IStream::seekBegin);

   // in uw1 mode, just return file
   // todo: caller must know how long the file block is in this case,
   // or has to seek around
   if (!m_bUw2Mode)
      return m_spArchiveFile;

   // decode uw2 block
   return Uw2Decode(m_spArchiveFile, m_vecInfos[uiIndex].m_bCompressed, m_vecInfos[uiIndex].m_uiDataSize);
}
