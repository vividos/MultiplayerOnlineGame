//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ByteStream.hpp Byte stream I/O class
//
#pragma once

// includes
#include <deque>
#include <vector>
#include "Uuid.hpp"
#include "Vector3d.hpp"
#include <limits>
#include <boost/array.hpp>

/// \brief byte stream class
/// used for serializing and deserializing objects
class ByteStream
{
public:
   /// default ctor
   ByteStream()
   {
   }

   /// dtor
   virtual ~ByteStream() throw()
   {
   }

   // virtual methods to override

   /// reads byte
   virtual unsigned char Read8() = 0;

   /// reads data block
   virtual void ReadBlock(unsigned char* pData, size_t uiSizeToRead) = 0;

   /// writes byte
   virtual void Write8(unsigned char uc) = 0;

   /// writes data block
   virtual void WriteBlock(const unsigned char* pData, size_t uiSize) = 0;

   /// reads block; convenience function for std::vector
   void ReadBlock(std::vector<unsigned char>& vecData, size_t uiSizeToRead)
   {
      vecData.resize(uiSizeToRead);
      ReadBlock(&vecData[0], uiSizeToRead);
   }

   /// writes block; convenience function for std::vector
   void WriteBlock(const std::vector<unsigned char>& vecData)
   {
      WriteBlock(&vecData[0], vecData.size());
   }

   /// read methods

   /// reads unsigned short
   unsigned short Read16()
   {
      unsigned short us = Read8(); // low byte
      us |= static_cast<unsigned short>(Read8()) << 8; // high byte
      return us;
   }

   /// reads unsigned int
   unsigned int Read32()
   {
      unsigned int ui = Read8();
      ui |= static_cast<unsigned int>(Read8()) << 8;
      ui |= static_cast<unsigned int>(Read8()) << 16;
      ui |= static_cast<unsigned int>(Read8()) << 24;
      return ui;
   }

   /// reads double value
   double ReadDouble()
   {
      // read in 23.8 bit format
      unsigned int uiValue = Read32();
      bool bNegative = (uiValue & 0x80000000) != 0;

      double dValue = double(uiValue & (~0x80000000))/256.0;

      return bNegative ? -dValue : dValue;
   }

   /// reads vector
   Vector3d ReadVector()
   {
      double x = ReadDouble();
      double y = ReadDouble();
      double z = ReadDouble();

      return Vector3d(x,y,z);
   }

   /// reads utf-8 encoded string
   CString ReadString(unsigned short usMaxLen)
   {
      // check length
      unsigned short usLength = Read16();
      if (usLength >= usMaxLen)
         throw Exception(_T("read string: length in stream too long"), __FILE__, __LINE__);

      if (usLength == 0)
         return CString();

      std::vector<unsigned char> vecData(usLength+1);

      ReadBlock(&vecData[0], usLength);

      return CString(reinterpret_cast<const char*>(&vecData[0]));
   }

   /// reads Uuid
   Uuid ReadUuid()
   {
      boost::array<unsigned char, 16> aData;
      ReadBlock(&aData[0], 16);
      return Uuid(&aData[0]);
   }

   // write functions

   /// reads unsigned short
   void Write16(unsigned short us)
   {
      Write8(static_cast<unsigned char>(us & 0x00ff)); // low byte
      Write8(static_cast<unsigned char>((us >> 8)& 0x00ff)); // high byte
   }

   /// reads unsigned int
   void Write32(unsigned int ui)
   {
      Write8(static_cast<unsigned char>(ui & 0x000000ff));
      Write8(static_cast<unsigned char>((ui >> 8)& 0x000000ff));
      Write8(static_cast<unsigned char>((ui >> 16)& 0x000000ff));
      Write8(static_cast<unsigned char>((ui >> 24)& 0x000000ff));
   }

   /// writes double value, with 1/(2^8) precision
   void WriteDouble(double dValue)
   {
      // write 23.8 bit format
      unsigned int uiValue = static_cast<unsigned int>(fabs(dValue) * 256.0);
      if (dValue < 0.0)
         uiValue |= 0x80000000;
      Write32(uiValue);
   }

   /// writes vector
   void WriteVector(const Vector3d& vec)
   {
      WriteDouble(vec.X());
      WriteDouble(vec.Y());
      WriteDouble(vec.Z());
   }

   /// writes UTF-8 encoded string
   void WriteString(const CString& csz)
   {
      if (csz.GetLength() > std::numeric_limits<unsigned short>::max())
         throw Exception(_T("string too long!"), __FILE__, __LINE__);
      Write16(static_cast<unsigned short>(csz.GetLength()));

      USES_CONVERSION;
      LPCSTR pszaText = T2CA(static_cast<LPCTSTR>(csz));

      const unsigned char* p = reinterpret_cast<const unsigned char*>(pszaText);
      WriteBlock(p, csz.GetLength());
   }

   /// writes Uuid
   void WriteUuid(const Uuid& uuid)
   {
      WriteBlock(uuid.Raw(), 16);
   }
};


/// const vector reference backed stream
class ConstVectorRefStream: public ByteStream
{
public:
   /// ctor; takes data stream
   ConstVectorRefStream(const std::vector<unsigned char>& vecData)
      :m_vecData(vecData),
       m_uiPos(0)
   {
   }

   // read functions

   /// reads unsigned char
   virtual unsigned char Read8() override
   {
      if (m_uiPos >= m_vecData.size())
         throw Exception(_T("Read8: not enough data in stream"), __FILE__, __LINE__);

      return m_vecData[m_uiPos++];
   }

   /// reads block of data
   virtual void ReadBlock(unsigned char* pData, size_t uiSizeToRead) override
   {
      if (m_uiPos + uiSizeToRead > m_vecData.size())
         throw Exception(_T("ReadBlock: not enough data in stream"), __FILE__, __LINE__);

      memcpy(pData, m_vecData.data() + m_uiPos, uiSizeToRead);
      m_uiPos += uiSizeToRead;
   }


   // write functions

   /// writes unsigned char
   void Write8(unsigned char /*uc*/) override
   {
      ATLASSERT(false); // call not supported
   }

   /// writes block of data
   void WriteBlock(const unsigned char* /*pData*/, size_t /*uiSize*/) override
   {
      ATLASSERT(false); // call not supported
   }

private:
   /// reference to data
   const std::vector<unsigned char>& m_vecData;

   /// current position
   size_t m_uiPos;
};

/// vector backed stream
class VectorStream: public ByteStream
{
public:
   /// default ctor
   VectorStream()
      :m_uiPos(0)
   {
   }

   /// ctor; copies data stream
   VectorStream(const std::vector<unsigned char>& vecData)
      :m_vecData(vecData.begin(), vecData.end()),
       m_uiPos(0)
   {
   }

   // read functions

   /// reads unsigned char
   virtual unsigned char Read8() override
   {
      if (m_uiPos >= m_vecData.size())
         throw Exception(_T("Read8: not enough data in stream"), __FILE__, __LINE__);

      return m_vecData[m_uiPos++];
   }

   /// reads block of data
   virtual void ReadBlock(unsigned char* pData, size_t uiSizeToRead) override
   {
      if (m_uiPos + uiSizeToRead >= m_vecData.size())
         throw Exception(_T("ReadBlock: not enough data in stream"), __FILE__, __LINE__);

      memcpy(pData, m_vecData.data() + m_uiPos, uiSizeToRead);
      m_uiPos += uiSizeToRead;
   }


   // write functions

   /// writes unsigned char
   void Write8(unsigned char uc) override
   {
      m_vecData.push_back(uc);
   }

   /// writes block of data
   void WriteBlock(const unsigned char* pData, size_t uiSize) override
   {
      m_vecData.insert(m_vecData.end(), pData, pData+uiSize);
   }


   // misc. methods

   /// returns remaining data
   std::vector<unsigned char>& Data() throw() { return m_vecData; }

   /// returns current pos
   size_t Pos() const throw() { return m_uiPos; }

   /// returns size
   size_t Size() const throw() { return m_vecData.size(); }

private:
   /// data
   std::vector<unsigned char> m_vecData;

   /// current position for reading
   size_t m_uiPos;
};
