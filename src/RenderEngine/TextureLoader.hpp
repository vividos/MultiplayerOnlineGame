//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TextureLoader.hpp Texture loader class
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"

// forward references
class IFileSystem;
class IImageReader;
class Texture;

/// loads a texture from a source, and uploads it to the video card
class RENDERENGINE_DECLSPEC TextureLoader
{
public:
   /// ctor
   TextureLoader(IFileSystem& fileSystem);
   /// dtor
   virtual ~TextureLoader() throw() {}

   /// loads texture from source; extension is used to find loader
   void Load(const CString& cszFilename);

   /// uploads last loaded image to texture; render context is needed
   void Upload(Texture& tex, bool bGenerateMipmap);

private:
   /// file system
   IFileSystem& m_fileSystem;

   /// current image reader
   std::shared_ptr<IImageReader> m_spImageReader;
};
