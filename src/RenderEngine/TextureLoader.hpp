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
class GraphicsTaskManager;
class IFileSystem;
class IImageReader;
class Texture;

/// loads a texture from a source, and uploads it to the video card
class RENDERENGINE_DECLSPEC TextureLoader
{
public:
   /// ctor
   TextureLoader(GraphicsTaskManager& taskManager, IFileSystem& fileSystem);
   /// dtor
   virtual ~TextureLoader() throw() {}

   /// loads texture from source and uploads it asynchronously; extension is used to find loader
   void Load(const CString& cszFilename, std::shared_ptr<Texture> spTexture, bool bGenerateMipmap);

private:
   /// uploads last loaded image to texture; called in upload thread
   void Upload(std::shared_ptr<IImageReader> spImageReader, std::shared_ptr<Texture> spTexture, bool bGenerateMipmap);

private:
   /// task manager
   GraphicsTaskManager& m_taskManager;

   /// file system
   IFileSystem& m_fileSystem;
};
