//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\Ms3dxFileStreamDecoder.cpp .ms3dx file stream decoder
//

// includes
#include "stdafx.h"
#include "Ms3dxFileStreamDecoder.hpp"

Ms3dxFileStreamDecoder::Ms3dxFileStreamDecoder(std::shared_ptr<Stream::IStream> spSourceStream, const std::vector<BYTE>& vecXorKey)
:m_spStream(spSourceStream),
m_vecXorKey(vecXorKey)
{
}

bool Ms3dxFileStreamDecoder::Read(void* bBuffer, DWORD dwMaxBufferLength, DWORD& dwBytesRead)
{
   bool bRet = m_spStream->Read(bBuffer, dwMaxBufferLength, dwBytesRead);

   if (!bRet || dwBytesRead == 0)
      return bRet;

   // decode
   BYTE* pBuffer = reinterpret_cast<BYTE*>(bBuffer);
   DecodeBuffer(pBuffer, dwBytesRead);

   return bRet;
}

ULONGLONG Ms3dxFileStreamDecoder::Seek(LONGLONG llOffset, ESeekOrigin origin)
{
   ULONGLONG ullNewPos = m_spStream->Seek(llOffset, origin);

   // adjust current key pos
   m_uiCurrentPos = ullNewPos % m_vecXorKey.size();

   return ullNewPos;
}

void Ms3dxFileStreamDecoder::DecodeBuffer(BYTE* pBuffer, DWORD dwBytesRead)
{
   const size_t c_uiKeySize = m_vecXorKey.size();

   for (DWORD dw = 0; dw < dwBytesRead; dw++)
   {
      pBuffer[dw] ^= m_vecXorKey[m_uiCurrentPos++];

      if (m_uiCurrentPos >= c_uiKeySize)
         m_uiCurrentPos = 0;
   }
}
