//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ZipArchive.hpp Zip archive class
//
#pragma once

// includes
#include "Base.hpp"
#include <vector>

// forward references
struct ZipCentralDirectoryEndRecord;
struct ZipCentralFileHeader;
struct ZipLocalFileHeader;
namespace Stream
{
   class IStream;
};

/// \brief zip archive
/// \details this class doesn't support the following features:
/// * writing to zip archives
/// * decryption of encrypted archive files
/// * other compression methods than STORE and INFLATE
/// * Zip64 archives
/// * digital signature
/// * decryption of encrypted central directories
/// * zip archives that span more than one file
/// All incompatible archives will produce an exception on opening or
/// reading.
class BASE_DECLSPEC ZipArchive
{
public:
   /// ctor
   ZipArchive(Stream::IStream& stream);
   /// dtor
   ~ZipArchive();

   /// returns number of files in archive
   unsigned int FileCount() const throw() { return m_vecInfos.size(); }

   /// returns filename of entry in archive
   const CString& Filename(unsigned int uiIndex, bool bFullPath);

   /// returns a file in the archive as stream
   boost::shared_ptr<Stream::IStream> GetFile(unsigned int uiIndex);

private:
   /// parses global directory of zip archive
   void Parse(Stream::IStream& stream);

   /// finds central directory end record
   void FindCentralDirEndRecord(Stream::IStream& stream, ZipCentralDirectoryEndRecord& endRecord);

   /// reads central file header
   void ReadCentralFileHeader(Stream::IStream& stream, ZipCentralFileHeader& centralFileHeader, CString& cszFilename);

   /// reads local file header
   void ReadLocalFileHeader(Stream::IStream& stream, ZipLocalFileHeader& localFileHeader);

private:
   /// zip archive file info
   struct ZipFileInfo
   {
      /// ctor
      ZipFileInfo()
         :uiOffset(0),
          uiCompressedSize(0),
          uiUncompressedSize(0)
      {
      }

      /// filename
      CString cszFilename;

      /// offset in zip archive
      ULONGLONG uiOffset;

      /// size of compressed data
      unsigned int uiCompressedSize;

      /// size of uncompressed data
      unsigned int uiUncompressedSize;
   };

   /// zip archive stream
   Stream::IStream& m_archiveStream;

   /// file infos
   std::vector<ZipFileInfo> m_vecInfos;

   /// global offset in archive stream
   ULONGLONG m_ullGlobalOffset;
};
