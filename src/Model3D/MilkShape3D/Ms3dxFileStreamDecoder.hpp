//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\Ms3dxFileStreamDecoder.hpp .ms3dx file stream decoder
//
#pragma once

// includes
#include <ulib/stream/IStream.hpp>

/// file stream decoder for .ms3dx files
class Ms3dxFileStreamDecoder : public Stream::IStream
{
public:
   /// ctor
   Ms3dxFileStreamDecoder(std::shared_ptr<Stream::IStream> spSourceStream, const std::vector<BYTE>& vecXorKey);
   /// dtor
   virtual ~Ms3dxFileStreamDecoder() throw() {}

   virtual bool CanRead() const throw() { return m_spStream->CanRead(); }
   virtual bool CanWrite() const throw() { return false; }
   virtual bool CanSeek() const throw() { return false; }

   /// reads amount of data into given buffer; returns if stream is at its end
   virtual bool Read(void* bBuffer, DWORD dwMaxBufferLength, DWORD& dwBytesRead) override;

   virtual bool AtEndOfStream() const throw() { return m_spStream->AtEndOfStream(); }

   virtual void Write(const void* /*bData*/, DWORD /*dwLength*/, DWORD& /*dwBytesWritten*/) { ATLASSERT(false); }
   virtual void WriteByte(BYTE /*b*/) { ATLASSERT(false); }

   // seek support

   virtual ULONGLONG Seek(LONGLONG llOffset, ESeekOrigin origin) override;
   virtual ULONGLONG Position() { return m_spStream->Position(); }
   virtual ULONGLONG Length() { return m_spStream->Length(); }

   virtual void Flush() {}
   virtual void Close() { m_spStream->Close(); }

private:
   /// decodes given buffer
   void DecodeBuffer(BYTE* pBuffer, DWORD dwBytesRead);

private:
   std::shared_ptr<Stream::IStream> m_spStream;

   std::vector<BYTE> m_vecXorKey;

   size_t m_uiCurrentPos;
};
