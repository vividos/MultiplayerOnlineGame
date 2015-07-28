//
// ulib - a collection of useful classes
// Copyright (C) 2015 Michael Fink
//
/// \file CString.hpp implementation of a platform independent CString class
//

// includes
#pragma once

// includes
#include <cstring>
#include <cstdio>
#include <stdexcept>
#include <cstdarg>
#include <atomic>
#include <iterator>
#include <algorithm>

/// character type traits, for single-byte char
template <typename T = CHAR>
struct CharTypeTraits
{
   typedef CHAR CharType;
   typedef WCHAR OtherCharType;

   typedef CharType* PXSTR;         ///< type of character string
   typedef const CharType* PCXSTR;  ///< type of character string; const version


   /// returns string length for narrow character
   static int StringLength(const CHAR* str)
   {
      if (str == nullptr)
         return 0;
      return strlen(str);
   }

   // narrow -> wide
   static int ConvertFromOther(LPWSTR strDestBuffer, int iDestLength, LPCSTR strSource)
   {
      //return ::MultiByteToWideChar(CP_ACP, 0, strSource, -1, strDestBuffer, iDestLength, nullptr, nullptr);
      return 0; // TODO
   }

   static int FormatBuffer(PXSTR strBuffer, int iBufferLength, PCXSTR strFormat, va_list args)
   {
#ifdef __ANDROID__
      return 0; // TODO
#endif
#ifdef _MSC_VER
      if (strBuffer == 0 || iBufferLength == 0)
         return _vscprintf(strFormat, args);

      return vsnprintf_s(strBuffer, iBufferLength, iBufferLength, strFormat, args);
#endif
   }
};

/// character type traits, for wchar_t, 16 bit, UCS
template <>
struct CharTypeTraits<WCHAR>
{
   typedef WCHAR CharType;
   typedef CHAR OtherCharType;

   typedef CharType* PXSTR;         ///< type of character string
   typedef const CharType* PCXSTR;  ///< type of character string; const version

   /// returns string length for wide character
   static int StringLength(const WCHAR* str)
   {
      if (str == nullptr)
         return 0;
      return wcslen(str);
   }

   // wide -> narrow
   static int ConvertFromOther(LPSTR strDestBuffer, int iDestLength, LPCWSTR strSource)
   {
      //return ::WideCharToMultiByte(CP_ACP, 0, strSource, -1, strDestBuffer, iDestLength, nullptr, nullptr);
      return 0; // TODO
   }

   static int FormatBuffer(PXSTR strBuffer, int iBufferLength, PCXSTR strFormat, va_list args)
   {
#ifdef __ANDROID__
      return 0; // TODO
#endif
#ifdef _MSC_VER
      if (strBuffer == 0 || iBufferLength == 0)
         return _vscwprintf(strFormat, args);

      return _vsnwprintf_s(strBuffer, iBufferLength, iBufferLength, strFormat, args);
#endif
   }
};

/// \brief A platform independent MFC CString template class
/// \details This class implements a platform independent string class
/// that is modeled after the CStringT class found in the MFC library.
template <typename T>
class CStringT
{
   // typedefs

   /// type of character managed in this string
   typedef typename CharTypeTraits<T>::CharType XCHAR;
   typedef XCHAR* PXSTR;         ///< type of character string
   typedef const XCHAR* PCXSTR;  ///< type of character string; const version

   /// type of the "other" character type (char vs. wchar_t)
   typedef typename CharTypeTraits<T>::OtherCharType YCHAR;
   typedef YCHAR* PYSTR;         ///< type of character string
   typedef const YCHAR* PCYSTR;  ///< type of character string; const version

   // nested structs

   /// nested class: actual string data
   // TODO make private?
   struct CStringData
   {
      int m_iDataLength;         ///< actual length of string
      int m_iAllocLength;        ///< number of chars allocated in buffer
      std::atomic<int> m_iRefs;  ///< number of references
      // these fields are followed by the actual string buffer, and a zero terminator char

      void* GetData() throw()
      {
         return (this + 1);
      }

      void AddRef()
      {
         ++m_iRefs;
      }

      void Release()
      {
         ATLASSERT(m_iRefs != 0);
         if (--m_iRefs <= 0)
            Free();
      }

      bool IsShared() const throw()
      {
         return m_iRefs > 1;
      }

      static CStringData* Allocate(int iLength) throw()
      {
         ATLASSERT(iLength >= 0);
         if (iLength < 0)
            return nullptr;

         int iDataBytes = (iLength + 1) * sizeof(XCHAR);
         int iTotalSize = sizeof(CStringData) + iDataBytes;
         CStringData* pData = (CStringData*)malloc(iTotalSize);

         if (pData == nullptr)
            return nullptr;

         pData->m_iDataLength = 0;
         pData->m_iAllocLength = iLength;
         pData->m_iRefs = 1;

         return pData;
      }

      void Free()
      {
         free(this);
      }
   };

public:
   // ctors and dtor

   /// default ctor
   CStringT()
   {
      CStringData* pData = GetNilString();
      Attach(pData);
   }

   /// copy ctor
   CStringT(const CStringT& str)
   {
      CStringData* pSrcData = str.GetData();
      CStringData* pNewData = CloneData(pSrcData);
      Attach(pNewData);
   }

   /// copy ctor; using other char type
   CStringT(const CStringT<YCHAR>& str)
   {
      int iDestLength = CharTypeTraits<YCHAR>::ConvertFromOther(nullptr, 0, str);
      if (iDestLength > 0)
      {
         PXSTR strBuffer = GetBuffer(iDestLength);

         CharTypeTraits<YCHAR>::ConvertFromOther(strBuffer, iDestLength, str);

         ReleaseBufferSetLength(iDestLength);
      }
      else
         Empty();
   }

   /// ctor, taking a C style string, zero-terminated
   CStringT(PCXSTR str)
   {
      int iLength = CharTypeTraits<T>::StringLength(str);
      CStringData* pData = Allocate(iLength);
      if (pData == nullptr)
         throw std::runtime_error("CString: out of memory");

      Attach(pData);
      SetLength(iLength);
      CopyChars(m_pszData, iLength, str, iLength);
   }

   /// ctor, taking an other character type C style string, zero-terminated
   CStringT(PCYSTR str)
   {
      int iDestLength = CharTypeTraits<YCHAR>::ConvertFromOther(nullptr, 0, str);
      if (iDestLength > 0)
      {
         PXSTR strBuffer = GetBuffer(iDestLength);

         CharTypeTraits<YCHAR>::ConvertFromOther(strBuffer, iDestLength, str);

         ReleaseBufferSetLength(iDestLength);
      }
      else
         Empty();
   }

   /// ctor, taking a C style string and a length
   CStringT(PCXSTR str, int iLength)
   {
      CStringData* pData = Allocate(iLength);
      if (pData == nullptr)
         throw std::runtime_error("CString: out of memory");

      Attach(pData);
      CopyChars(m_pszData, iLength, str, iLength);
   }

   /// ctor, taking an other character type C style string and a length
   CStringT(PCYSTR str, int iLength)
   {
      // TODO
   }

   /// ctor, taking a char and a repeat count
   CStringT(XCHAR ch, int iRepeat = 1)
   {
      // TODO
   }

   /// ctor, taking an other char type char and a repeat count
   CStringT(YCHAR ch, int iRepeat = 1)
   {
      // TODO
   }

   /// dtor
   ~CStringT() throw()
   {
      CStringData* pData = GetData();
      pData->Release();
   }

   // operators

   /// assign operator
   CStringT& operator=(const CStringT& str)
   {
      SetString(str);
      return *this;
   }

   /// assign operator, C-style strings
   CStringT& operator=(PCXSTR str)
   {
      SetString(str);
      return *this;
   }

   /// inplace add operator
   CStringT& operator+=(const CStringT& str)
   {
      Append(str);
      return *this;
   }

   /// inplace add operator, C-style strings
   CStringT& operator+=(PCXSTR str)
   {
      Append(str);
      return *this;
   }

   /// inplace add operator, single character
   CStringT& operator+=(XCHAR str)
   {
      AppendChar(str);
      return *this;
   }

   /// array operator
   XCHAR operator[](int iIndex) const
   {
      ATLASSERT(iIndex >= 0 && iIndex <= GetLength()); // indexing zero terminator is ok

      if (!(iIndex >= 0 && iIndex <= GetLength()))
         throw std::invalid_argument("CString: invalid index argument to array operator");

      return m_pszData[iIndex];
   }

   /// C-style string "cast" operator
   operator PCXSTR() const throw()
   {
      return m_pszData;
   }

   /// appends a string
   void Append(PCXSTR str)
   {
      Append(str, CharTypeTraits<XCHAR>::StringLength(str));
   }

   /// appends a C-style string
   void Append(const CStringT str)
   {
      Append(str.GetString(), str.GetLength());
   }

   /// appends a string, with given length
   void Append(PCXSTR str, int iLength)
   {
      // TODO
   }

   /// appends a single character
   void AppendChar(XCHAR ch)
   {
      // TODO
   }

   /// empties the string
   void Empty() throw()
   {
      if (GetLength() == 0)
         return;

      CStringData* pOldData = GetData();
      pOldData->Release();

      CStringData* pNewData = GetNilString();
      Attach(pNewData);
   }

   /// returns character at given index
   XCHAR GetAt(int iIndex)
   {
      ATLASSERT(iIndex >= 0 && iIndex <= GetLength()); // indexing zero terminator is ok

      if (!(iIndex >= 0 && iIndex <= GetLength()))
         throw std::invalid_argument("CString: invalid index argument to GetAt()");

      return m_pszData[iIndex];
   }

   /// returns a writable buffer of current string
   PXSTR GetBuffer()
   {
      CStringData* pData = GetData();

      if (pData->IsShared())
         Fork(pData->m_iDataLength);

      return m_pszData;
   }

   /// returns a writable buffer of current string, with minimum given buffer length
   PXSTR GetBuffer(int /*iBufferLength*/)
   {
      // TODO implement
      return nullptr;
   }

   PXSTR GetBufferSetLength(int /*iBufferLength*/)
   {
      // TODO implement
      return nullptr;
   }

   /// TODO
   void ReleaseBuffer(int iNewLength = -1)
   {
      // TODO
   }

   void ReleaseBufferSetLength(int iNewLength)
   {
      // TODO
   }

   /// returns string length, without string-terminating zero character
   int GetLength() const throw()
   {
      return GetData()->m_iDataLength;
   }

   /// returns raw C string
   PCXSTR GetString() const throw()
   {
      return m_pszData;
   }

   /// returns if string is empty
   bool IsEmpty() const throw()
   {
      return GetLength() == 0;
   }

   void SetAt(int iIndex, XCHAR ch)
   {
      ATLASSERT(iIndex >= 0 && iIndex < GetLength()); // indexing zero terminator is NOT ok

      if (!(iIndex >= 0 && iIndex < GetLength()))
         throw std::invalid_argument("CString: invalid index argument to SetAt()");

      // TODO do differently?
      m_pszData[iIndex] = ch;
   }

   /// sets new string
   void SetString(PCXSTR str)
   {
      SetString(str, CharTypeTraits<T>::StringLength(str));
   }

   /// sets new string, with given length
   void SetString(PCXSTR str, int iLength)
   {
      if (iLength == 0)
      {
         Empty();
         return;
      }

      // TODO
   }

   // friend operators

   /// add operator, with two CStringT's
   friend CStringT operator+(const CStringT& lhs, const CStringT& rhs)
   {
      CStringT result(lhs);
      result.Append(rhs);
      return result;
   }

   /// add operator, with a C-style string and a CStringT
   friend CStringT operator+(PCXSTR lhs, const CStringT& rhs)
   {
      CStringT result(lhs);
      result.Append(rhs);
      return result;
   }

   /// add operator, with a CStringT and a C-style string
   friend CStringT operator+(const CStringT& lhs, PCXSTR rhs)
   {
      CStringT result(lhs);
      result.Append(rhs);
      return result;
   }

   // higher-level functions

   int Compare(PCXSTR str) const
   {
      // TODO implement
      return 0;
   }

   int CompareNoCase(PCXSTR str) const
   {
      // TODO implement
      return 0;
   }

   int Delete(int iIndex, int nCount = 1)
   {
      // TODO implement
      return 0;
   }

   int Insert(int iIndex, XCHAR ch)
   {
      // TODO implement
      return 0;
   }

   int Insert(int iIndex, PCXSTR str)
   {
      // TODO implement
      return 0;
   }

   int Replace(XCHAR chOld, XCHAR chNew)
   {
      // TODO implement
      return 0;
   }

   int Replace(PCXSTR strOld, PCXSTR strNew)
   {
      // TODO implement
      return 0;
   }

   int Remove(XCHAR chRemove)
   {
      // TODO implement
      return 0;
   }

   CStringT Tokenize(PCXSTR pszTokens, int& iStart) const
   {
      // TODO implement
      return CStringT();
   }

   int Find(XCHAR /*ch*/, int /*iStart*/ = 0) const throw()
   {
      // TODO implement
      return 0;
   }

   int Find(PCXSTR /*str*/, int /*iStart*/ = 0) const throw()
   {
      return 0;
   }

   int FindOneOf(PCXSTR /*strCharColl*/) const throw()
   {
      // TODO implement
      return 0;
   }

   int ReverseFind(XCHAR ch) const throw()
   {
      // TODO implement
      return 0;
   }

   CStringT& MakeUpper()
   {
      return *this;
   }

   CStringT& MakeLower()
   {
      return *this;
   }

   CStringT& MakeReverse()
   {
      // TODO implement
      return *this;
   }

   CStringT& TrimLeft()
   {
      // TODO implement
      return *this;
   }

   CStringT& TrimLeft(XCHAR ch)
   {
      // TODO implement
      return *this;
   }

   CStringT& TrimLeft(PCXSTR str)
   {
      // TODO implement
      return *this;
   }

   CStringT& TrimRight()
   {
      // TODO implement
      return *this;
   }

   CStringT& TrimRight(XCHAR ch)
   {
      // TODO implement
      return *this;
   }

   CStringT& TrimRight(PCXSTR str)
   {
      // TODO implement
      return *this;
   }

   CStringT& Trim()
   {
      // TODO implement
      return *this;
   }

   CStringT& Trim(XCHAR ch)
   {
      // TODO implement
      return *this;
   }

   CStringT& Trim(PCXSTR str)
   {
      // TODO implement
      return *this;
   }



   CStringT Mid(int iFirst) const
   {
      // TODO implement
      return CStringT();
   }

   CStringT Mid(int /*iFirst*/, int /*iCount*/) const
   {
      // TODO implement
      return CStringT();
   }

   CStringT Right(int /*nCount*/) const
   {
      // TODO implement
      return CStringT();
   }

   CStringT Left(int /*nCount*/) const
   {
      // TODO implement
      return CStringT();
   }

   CStringT SpanIncluding(PCXSTR strCharColl) const
   {
      // TODO implement
      return CStringT();
   }

   CStringT SpanExcluding(PCXSTR strCharColl) const
   {
      // TODO implement
      return CStringT();
   }

   // formatting

   void Format(PCXSTR strFormat, ...)
   {
      va_list args;
      va_start(args, strFormat);

      FormatV(strFormat, args);

      va_end(args);
   }

   void AppendFormat(PCXSTR strFormat, ...)
   {
      va_list args;
      va_start(args, strFormat);

      AppendFormatV(strFormat, args);

      va_end(args);
   }

   void FormatV(PCXSTR strFormat, va_list args)
   {
      int iBufferLength = CharTypeTraits<T>::FormatBuffer(nullptr, 0, strFormat, args);

      PXSTR strBuffer = GetBufferSetLength(iBufferLength);
      if (strBuffer == nullptr)
         throw std::runtime_error("CString: out of memory");

      int iLength = CharTypeTraits<T>::FormatBuffer(strBuffer, iBufferLength, strFormat, args);
      ATLASSERT(iBufferLength == iLength);

      ReleaseBufferSetLength(iLength);
   }

   void AppendFormatV(PCXSTR strFormat, va_list args)
   {
      CStringT str;
      str.FormatV(strFormat, args);

      Append(str);
   }

private:
   struct CNilStringData : public CStringData
   {
      CNilStringData()
      {
         CStringData::m_iRefs = 2; // ensures that the nil string never gets freed
         CStringData::m_iDataLength = 0;
         CStringData::m_iAllocLength = 0;

         extraBuffer[0] = extraBuffer[1] = 0;
      }

   private:
      wchar_t extraBuffer[2];
   };

   /// returns the string data struct
   CStringData* GetData() const throw()
   {
      // this class uses the same layout as the MFC CString class;
      // it allocates a space of two pointers, the first pointer being
      // a CStringData struct, and the second pointer points to the
      // buffer actually holding the string data.
      return reinterpret_cast<CStringData*>(m_pszData) - 1;
   }

   /// Allocates a new string data and string data
   CStringData* Allocate(int iLength) throw()
   {
      return CStringData::Allocate(iLength);
   }

   /// frees string data
   void Free(CStringData* pData)
   {
      pData->Free();
   }

   /// attaches a new string data
   void Attach(CStringData* pData)
   {
      m_pszData = static_cast<PXSTR>(pData->GetData());
   }

   void SetLength(int iLength)
   {
      ATLASSERT(iLength >= 0);
      ATLASSERT(iLength <= GetData()->m_iAllocLength);

      if (iLength < 0 || iLength > GetData()->m_iAllocLength)
         throw std::invalid_argument("CString: invalid length argument to SetLength()");

      GetData()->m_iDataLength = iLength;
      m_pszData[iLength] = 0;
   }

   /// makes string non-shared by creating a unique copy
   void Fork(int iNewLength)
   {
      CStringData* pOldData = GetData();
      int iOldLength = pOldData->m_iDataLength;

      CStringData* pNewData = Allocate(iNewLength);
      if (pNewData == nullptr)
         throw std::runtime_error("CString: out of memory");

      int iCharsToCopy = std::min(iOldLength, iNewLength) + 1;

      CopyChars(
         static_cast<PXSTR>(pNewData->GetData()), iCharsToCopy,
         static_cast<PXSTR>(pOldData->GetData()), iCharsToCopy);

      pNewData->m_iDataLength = iNewLength;

      pOldData->Release();

      Attach(pNewData);
   }

   static CStringData* CloneData(CStringData* pData)
   {
      CStringData* pNewData = CStringData::Allocate(pData->m_iDataLength);
      if (pNewData == nullptr)
         throw std::runtime_error("CString: out of memory");

      pNewData->m_iDataLength = pData->m_iDataLength;

      CopyChars(
         static_cast<PXSTR>(pNewData->GetData()), pNewData->m_iDataLength + 1,
         static_cast<PXSTR>(pData->GetData()), pData->m_iDataLength + 1);

      return pNewData;
   }

   static void CopyChars(PXSTR strTargetBuffer, int iLenTargetBuffer,
      PCXSTR strSource, int iLenSource)
   {
#ifdef _MSC_VER
      auto targetIter = stdext::checked_array_iterator<CStringT::PXSTR>(strTargetBuffer, iLenTargetBuffer);

      std::copy(strSource, strSource + iLenSource, targetIter);
#else
      std::copy(strSource, strSource + iLenSource, strTargetBuffer);
#endif
   }

   /// returns a statically allocated CStringData struct that
   /// represents an empty string
   static CStringData* GetNilString()
   {
      // TODO not thread safe
      static CNilStringData s_nilData;

      s_nilData.AddRef();
      return &s_nilData;
   }

private:
   /// string data
   PXSTR m_pszData;
};

typedef CStringT<CHAR> CStringA;
typedef CStringT<WCHAR> CStringW;
typedef CStringT<TCHAR> CString;
