//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file NamedTextureMap.hpp Named texture mapping class
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include <boost/noncopyable.hpp>
#include "Texture.hpp"
#include <map>

/// \brief named texture map
/// \details Manages shared_ptr instances of Texture objects by name. Additionally
/// counts size of all managed textures.
class RENDERENGINE_DECLSPEC NamedTextureMap: public boost::noncopyable
{
public:
   /// ctor
   NamedTextureMap() throw()
      :m_uiSize(0)
   {
   }

   /// returns if a given texture is available
   bool IsTextureAvail(const CString& cszName) const throw();

   /// register texture as named texture
   void Register(const CString& cszName, TexturePtr spTexture);

   /// unregisters named texture
   TexturePtr Unregister(const CString& cszName);

   /// returns a texture by name; throws when texture isn't registered
   TexturePtr Get(const CString& cszName);

   /// returns texture size in video ram
   unsigned int TextureSize() const throw() { return m_uiSize; }

   /// returns number of textures in map
   unsigned int NumTextures() const throw() { return m_mapAllTextures.size(); }

private:
   /// type for named textures mapping
   typedef std::map<CString, TexturePtr> T_mapAllTextures;

   /// all named textures
   T_mapAllTextures m_mapAllTextures;

   /// size of all textures, in bytes
   unsigned int m_uiSize;
};
