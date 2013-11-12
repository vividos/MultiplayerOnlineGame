//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file NamedTextureMap.cpp Named texture mapping class
//

// includes
#include "stdafx.h"
#include "NamedTextureMap.hpp"

bool NamedTextureMap::IsTextureAvail(const CString& cszName) const throw()
{
   T_mapAllTextures::const_iterator iter = m_mapAllTextures.find(cszName);
   return iter != m_mapAllTextures.end();
}

void NamedTextureMap::Register(const CString& cszName, TexturePtr spTexture)
{
   if (IsTextureAvail(cszName))
      m_uiSize -= m_mapAllTextures[cszName]->Size();

   m_mapAllTextures[cszName] = spTexture;

   m_uiSize += spTexture->Size();
}

/// \exception throws exception when texture name isn't found.
TexturePtr NamedTextureMap::Unregister(const CString& cszName)
{
   if (!IsTextureAvail(cszName))
      throw Exception(_T("Named texture not found: ") + cszName, __FILE__, __LINE__);

   T_mapAllTextures::const_iterator iter = m_mapAllTextures.find(cszName);
   TexturePtr spTex = iter->second;

   m_mapAllTextures.erase(iter);

   m_uiSize -= spTex->Size();

   return spTex;
}

/// \exception throws exception when texture name isn't found.
TexturePtr NamedTextureMap::Get(const CString& cszName)
{
   if (!IsTextureAvail(cszName))
      throw Exception(_T("Named texture not found: ") + cszName, __FILE__, __LINE__);

   T_mapAllTextures::const_iterator iter = m_mapAllTextures.find(cszName);
   ATLASSERT(iter != m_mapAllTextures.end());

   return iter->second;
}
