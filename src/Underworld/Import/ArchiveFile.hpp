//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/Import/ArchiveFile.hpp Archive file (*.ark)
//
#pragma once

// includes

// forward references
namespace Stream
{
class IStream;
}

namespace Import
{

// structs

/// info for on archive file entry
struct ArchiveFileEntryInfo
{
   /// ctor
   ArchiveFileEntryInfo()
      :m_uiDataSize(0),
      m_uiAvailSize(0),
      m_bCompressed(false),
      m_bAllocatedExtraSpace(false)
   {}

   /// size of data in entry, either compressed or uncompressed, depending on m_bCompressed flag
   Uint32 m_uiDataSize;

   /// available size for entry in file (only valid when m_bAllocatedExtraSpace is true)
   Uint32 m_uiAvailSize;

   /// indicates if the entry is actually compressed
   bool m_bCompressed;

   /// indicates if the entry has extra space allocated (and m_uiAvailSize is valid)
   bool m_bAllocatedExtraSpace;
};

// classes

/// \brief Archive file class
/// Manages archive files (extension .ark) that contain blocks of data, e.g.
/// for level maps or conversations. Not all blocks may contain actual data.
/// The archive file class supports uw2 packed blocks.
class ArchiveFile
{
public:
   /// ctor
   ArchiveFile(std::shared_ptr<Stream::IStream> spFile, bool bUw2Mode = false);

   /// returns number of files in archive
   unsigned int GetNumFiles() const { return m_vecOffsets.size(); }

   /// checks if an archive file slot is available
   bool IsAvailable(unsigned int uiIndex) const;

   /// returns archive file 
   std::shared_ptr<Stream::IStream> GetFile(unsigned int uiIndex);

private:
   /// archive file
   std::shared_ptr<Stream::IStream> m_spArchiveFile;

   /// file offsets for all files in archive
   std::vector<Uint32> m_vecOffsets;

   /// infos for all entries in archive (only used in uw2 mode)
   std::vector<ArchiveFileEntryInfo> m_vecInfos;

   /// archive in uw2 mode?
   bool m_bUw2Mode;
};

} // namespace Import
