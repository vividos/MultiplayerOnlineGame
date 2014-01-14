//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014 Michael Fink
//
/// \file MemoryStream.hpp memory read-write stream
//
#pragma once

// needed includes
#include <ulib/stream/IStream.hpp>
#include <vector>

namespace Stream
{

/// read-write memory stream
class MemoryStream: public IStream
{
public:
   /// ctor; opens an empty memory stream
   MemoryStream() throw()
      :m_uiCurrentPos(0)
   {
   }

   /// ctor; provides memory contents for memory stream
   MemoryStream(const BYTE* pbData, DWORD_PTR dwLength)
      :m_uiCurrentPos(0)
   {
      m_vecData.assign(pbData, pbData + dwLength);
   }

   /// returns data
   const std::vector<BYTE>& GetData() const throw() { return m_vecData; }

   // virtual methods from IStream

   virtual bool CanRead() const throw() { return true; }
   virtual bool CanWrite() const throw() { return true; }
   virtual bool CanSeek() const throw() { return true; }

   virtual bool Read(void* bBuffer, DWORD dwMaxBufferLength, DWORD& dwBytesRead) throw()
   {
      dwBytesRead = m_vecData.size() - m_uiCurrentPos > dwMaxBufferLength ? dwMaxBufferLength : static_cast<DWORD>(m_vecData.size() - m_uiCurrentPos);
      if (dwBytesRead > 0)
      {
         memcpy(bBuffer, &m_vecData[m_uiCurrentPos], dwBytesRead);
         m_uiCurrentPos += dwBytesRead;
      }
      return dwBytesRead != 0;
   }

   virtual bool AtEndOfStream() const throw(){ return m_uiCurrentPos >= m_vecData.size(); }

   /// \exception std::exception when resizing vector fails
   virtual void Write(const void* bData, DWORD dwLength, DWORD& dwBytesWritten)
   {
      // add at current pos
      if (m_vecData.size() < m_uiCurrentPos + dwLength)
         m_vecData.resize(m_uiCurrentPos + dwLength);

      memcpy(&m_vecData[m_uiCurrentPos], bData, dwLength);
      dwBytesWritten = dwLength;
      m_uiCurrentPos += dwBytesWritten;
   }

   virtual ULONGLONG Seek(LONGLONG llOffset, ESeekOrigin origin) throw()
   {
      switch(origin)
      {
      case seekBegin:
         m_uiCurrentPos = static_cast<size_t>(llOffset);
         break;

      case seekCurrent:
         {
            LONGLONG llResult = m_uiCurrentPos + llOffset;
            m_uiCurrentPos = llResult < 0 ? 0 : static_cast<size_t>(llResult);
         }
         break;

      case seekEnd:
         m_uiCurrentPos = static_cast<size_t>(llOffset) > m_vecData.size() ? 0 : m_vecData.size() - static_cast<size_t>(llOffset);
         break;
      }

      if (m_uiCurrentPos > m_vecData.size())
         m_uiCurrentPos = m_vecData.size();

      return Position();
   }
   virtual ULONGLONG Position() throw() { return static_cast<ULONGLONG>(m_uiCurrentPos); }
   virtual ULONGLONG Length() throw() { return static_cast<ULONGLONG>(m_vecData.size()); }

   virtual void Flush() throw() {}
   virtual void Close() throw() { try { m_vecData.clear(); } catch(...){} }

private:
   /// data bytes
   std::vector<BYTE> m_vecData;

   /// current position
   size_t m_uiCurrentPos;
};

} // namespace Stream
