//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ZipArchive.cpp Zip archive class
//

// includes
#include "StdAfx.h"
#include "ZipArchive.hpp"
#include "ZipArchiveFile.hpp"
#include <ulib/stream/IStream.hpp>

ZipArchive::ZipArchive(Stream::IStream& stream)
:m_ullGlobalOffset(stream.Position()),
 m_archiveStream(stream)
{
   Parse(stream);
}

ZipArchive::~ZipArchive()
{
}

const CString& ZipArchive::Filename(unsigned int uiIndex, bool /*bFullPath*/)
{
   ATLASSERT(uiIndex < FileCount());
   return m_vecInfos[uiIndex].cszFilename;
}

boost::shared_ptr<Stream::IStream> ZipArchive::GetFile(unsigned int uiIndex)
{
   ATLASSERT(uiIndex < FileCount());

   boost::shared_ptr<Stream::IStream> spFile(
      new ZipArchiveFile(m_archiveStream,
         m_vecInfos[uiIndex].uiOffset + m_ullGlobalOffset,
         m_vecInfos[uiIndex].uiCompressedSize,
         m_vecInfos[uiIndex].uiUncompressedSize));

   return spFile;
}

#pragma pack(push, 1)

/// DOS file time struct
struct DOSFILETIME
{
   WORD wFileTime;   ///< time bits
   WORD wFileDate;   ///< date bits
};

/// known zip compression methods
enum ZipCompressionMethod
{
   compressionStore = 0,   ///< store
   compressionInflate = 8, ///< inflate
};

/// \brief file header that prepends a single file
struct ZipLocalFileHeader
{
   /// checks if header is valid
   bool IsValid() const
   {
      return acMagic[0] == 'P' &&
         acMagic[1] == 'K' &&
         acMagic[2] == 3 &&
         acMagic[3] == 4;
   }

   char acMagic[4];  ///< contains 'P', 'K', '\3', '\4', or 0x04034b50
   WORD wVersion;    ///< version needed to extract
   WORD wFlags;      ///< general purpose flags
   WORD wCompressionMethod;   ///< only supported: 0: store, 8: inflate
   DOSFILETIME ftLastModified;///< last modified date
   DWORD dwCrc32;             ///< CRC-32 checksum of uncompressed data
   DWORD dwCompressedSize;    ///< compressed size
   DWORD dwUncompressedSize;  ///< uncompressed size
   WORD wFilenameLen;     ///< length of filename
   WORD wExtraFieldLen;   ///< length of extra field
   //CHAR szFilename[1];
   // after the filename the "extra" field comes
};

/// \brief zip data descriptor
/// \details the data descriptor comes after the compressed file data when
/// it was not possible for the compressor to write the fields into
/// ZipLocalFileHeader, e.g. when streaming; appears only when bit 3 of the
/// wFlags field is set.
struct ZipDataDescriptor
{
   DWORD dwCrc32;             ///< CRC-32 checksum of uncompressed data
   DWORD dwCompressedSize;    ///< compressed size
   DWORD dwUncompressedSize;  ///< uncompressed size
};

/// file header from the central directory
struct ZipCentralFileHeader
{
   /// checks if header is valid
   bool IsValid() const
   {
      return acMagic[0] == 'P' &&
         acMagic[1] == 'K' &&
         acMagic[2] == 1 &&
         acMagic[3] == 2;
   }

   char acMagic[4];  ///< contains 'P', 'K', '\1', '\2', or 0x02014b50
   WORD wVersionUsed;   ///< version used for creating file
   WORD wVersionNeeded; ///< version needed to extract file
   WORD wFlags;         ///< general purpose flags
   WORD wCompressionMethod; ///< only supported: 0: store, 8: inflate
   DOSFILETIME ftLastModified;///< last modified file time
   DWORD dwCrc32;             ///< CRC-32 checksum of uncompressed data
   DWORD dwCompressedSize;    ///< compressed size
   DWORD dwUncompressedSize;  ///< uncompressed size
   WORD wFilenameLen;      ///< length of filename
   WORD wExtraFieldLen;    ///< length of extra field
   WORD wFileCommentLen;   ///< length of file comment
   WORD wStartDiskNumber;  ///< disk number in which file starts
   WORD wInternalFileAttr; ///< internal file attribute
   DWORD dwExternalFileAttr;  ///< external file attribute
   DWORD dwOffsetLocalHeader; ///< file offset to local header
   //CHAR szFilename[1];
   // after the filename the "extra" field follows
   // after that the file comment follows
};

/// end record for central directory
struct ZipCentralDirectoryEndRecord
{
   /// checks if header is valid
   bool IsValid() const
   {
      return acMagic[0] == 'P' &&
         acMagic[1] == 'K' &&
         acMagic[2] == 5 &&
         acMagic[3] == 6;
   }

   char acMagic[4];  ///< contains 'P', 'K', '\5', '\6', or 0x06054b50
   WORD wDiskNumber;          ///< disk number
   WORD wDiskCentralDirectory;///< disk number with central directory
   WORD wNumEntriesThisDisk;  ///< number of entries in this disk's central dir
   WORD wNumEntriesTotal;     ///< number of entries total
   DWORD dwSizeCentralDir;    ///< size of central directory
   DWORD dwOffsetCentralDir;  ///< offset to central directory
   WORD wLenFileComment;      ///< length of zip file comment
   /// zip comment follows
};

#pragma pack(pop)

void ZipArchive::Parse(Stream::IStream& stream)
{
   ATLASSERT(true == stream.CanSeek());

   // read end record
   ZipCentralDirectoryEndRecord endRecord = {0};
   FindCentralDirEndRecord(stream, endRecord);

   // find central directory info
   stream.Seek(endRecord.dwOffsetCentralDir, Stream::IStream::seekBegin);

   for (WORD wEntry = 0; wEntry < endRecord.wNumEntriesTotal; wEntry++)
   {
      // read central file entry
      ZipCentralFileHeader centralFileHeader = {0};
      CString cszFilename;
      ReadCentralFileHeader(stream, centralFileHeader, cszFilename);

      ULONGLONG ullPos = stream.Position(); // remember pos

      stream.Seek(centralFileHeader.dwOffsetLocalHeader, Stream::IStream::seekBegin);

      // read local file entry
      ZipLocalFileHeader localFileHeader = {0};
      ReadLocalFileHeader(stream, localFileHeader);

      // add entry
      ZipFileInfo info;
      info.cszFilename = cszFilename;
      info.uiOffset = stream.Position();
      info.uiCompressedSize = localFileHeader.dwCompressedSize;
      info.uiUncompressedSize = localFileHeader.dwUncompressedSize;

      m_vecInfos.push_back(info);

      // back to central directory
      stream.Seek(ullPos, Stream::IStream::seekBegin);
   }
}

void ZipArchive::FindCentralDirEndRecord(Stream::IStream& stream, ZipCentralDirectoryEndRecord& endRecord)
{
   stream.Seek(-LONGLONG(sizeof(ZipCentralDirectoryEndRecord)), Stream::IStream::seekEnd);

   DWORD dwBytesRead = 0;
   DWORD dwSizeToRead = sizeof(endRecord);
   if (!stream.Read(&endRecord, dwSizeToRead, dwBytesRead) || dwBytesRead != dwSizeToRead)
      throw Exception(_T("couldn't read zip archive end record"), __FILE__, __LINE__);

   // special case of no comment at end?
   if (!endRecord.IsValid())
   {
      // no, try to find end record signature in the last 1k
      std::vector<BYTE> vecBuffer(1024);
      stream.Seek(-1024, Stream::IStream::seekEnd);

      if (!stream.Read(&vecBuffer[0], 1024, dwBytesRead) || dwBytesRead != 1024)
         throw Exception(_T("couldn't read zip archive end record"), __FILE__, __LINE__);

      for (size_t i=1024-sizeof(ZipCentralDirectoryEndRecord)+1; i>0; i--)
      {
         if (vecBuffer[i-1] == 'P' &&
             vecBuffer[i] == 'K' &&
             vecBuffer[i+1] == 5 &&
             vecBuffer[i+2] == 6)
         {
            // found!
            stream.Seek(1024-i, Stream::IStream::seekEnd);

            DWORD dwSizeToRead = sizeof(ZipCentralDirectoryEndRecord);
            if (!stream.Read(&endRecord, dwSizeToRead, dwBytesRead) || dwBytesRead != dwSizeToRead)
               throw Exception(_T("couldn't read zip archive end record"), __FILE__, __LINE__);

            break;
         }
      }

      if (!endRecord.IsValid())
         throw Exception(_T("couldn't find zip archive end record"), __FILE__, __LINE__);
   }

   if (endRecord.wDiskNumber != 0 ||
       endRecord.wDiskCentralDirectory != 0 ||
       endRecord.wNumEntriesThisDisk != endRecord.wNumEntriesTotal)
      throw Exception(_T("multi-file zip archive not supported"), __FILE__, __LINE__);
}

void ZipArchive::ReadCentralFileHeader(Stream::IStream& stream, ZipCentralFileHeader& centralFileHeader, CString& cszFilename)
{
   DWORD dwBytesRead = 0;
   DWORD dwSizeToRead = sizeof(centralFileHeader);
   if (!stream.Read(&centralFileHeader, dwSizeToRead, dwBytesRead) || dwBytesRead != dwSizeToRead)
      throw Exception(_T("couldn't read zip archive central file header"), __FILE__, __LINE__);

   if (!centralFileHeader.IsValid())
      throw Exception(_T("invalid zip archive central file header"), __FILE__, __LINE__);

   if (centralFileHeader.wCompressionMethod != compressionInflate &&
       centralFileHeader.wCompressionMethod != compressionStore)
      throw Exception(_T("invalid zip archive central file header"), __FILE__, __LINE__);

   if (centralFileHeader.wStartDiskNumber != 0)
      throw Exception(_T("multi-file zip archive not supported"), __FILE__, __LINE__);

   if ((centralFileHeader.wFlags & 1) != 0)
      throw Exception(_T("zip archive encrypted file not supported"), __FILE__, __LINE__);

   // skip over variable length fields
   ULONGLONG ullNextHeaderPos = stream.Position() +
      centralFileHeader.wFilenameLen +
      centralFileHeader.wExtraFieldLen +
      centralFileHeader.wFileCommentLen;

   // read filename
   std::vector<char> vecFilename(centralFileHeader.wFilenameLen);

   dwSizeToRead = centralFileHeader.wFilenameLen;
   if (!stream.Read(&vecFilename[0], dwSizeToRead, dwBytesRead) || dwBytesRead != dwSizeToRead)
      throw Exception(_T("couldn't read zip archive central file header"), __FILE__, __LINE__);

   vecFilename.push_back(0);

   USES_CONVERSION;
   cszFilename = A2CT(&vecFilename[0]);

   cszFilename.Replace(_T('/'), _T('\\'));

   stream.Seek(ullNextHeaderPos, Stream::IStream::seekBegin);
}

void ZipArchive::ReadLocalFileHeader(Stream::IStream& stream, ZipLocalFileHeader& localFileHeader)
{
   // try to read local file header
   DWORD dwBytesRead = 0;
   DWORD dwSizeToRead = sizeof(localFileHeader);
   if (!stream.Read(&localFileHeader, dwSizeToRead, dwBytesRead) || dwBytesRead != dwSizeToRead)
      throw Exception(_T("couldn't read zip archive local file header"), __FILE__, __LINE__);

   if (!localFileHeader.IsValid())
      throw Exception(_T("invalid zip archive local file header"), __FILE__, __LINE__);

   if (localFileHeader.wCompressionMethod != compressionInflate &&
       localFileHeader.wCompressionMethod != compressionStore)
      throw Exception(_T("invalid zip archive local file header"), __FILE__, __LINE__);

   // bit 3 of wFlags set?
   if ((localFileHeader.wFlags & (1<<3)) != 0 && (
       localFileHeader.dwUncompressedSize == 0 ||
       localFileHeader.dwCompressedSize == 0) )
      throw Exception(_T("zip archive local header contains unsupported local file data descriptor"), __FILE__, __LINE__);

   // skip over variable length fields
   LONGLONG llFieldLengths =
      localFileHeader.wFilenameLen +
      localFileHeader.wExtraFieldLen;

   stream.Seek(llFieldLengths, Stream::IStream::seekCurrent);
}
