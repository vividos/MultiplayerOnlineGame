//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TextureLoader.hpp Texture loader class
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"

// forward references
class GraphicsTaskManager;
class IFileSystem;
class IImageReader;
class Texture;
namespace Stream
{
   class IStream;
}

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

   /// loads texture from stream and uploads it asynchronously; extension must be given to find loader
   void Load(std::shared_ptr<Stream::IStream> spStream, const CString& cszExtensionWithDot,
      std::shared_ptr<Texture> spTexture, bool bGenerateMipmap);

private:
   /// selects image reader based on extension
   std::shared_ptr<IImageReader> SelectImageReader(const CString& cszExtensionWithDot);

   /// internal load function
   void LoadInternal(std::shared_ptr<Stream::IStream> spStream, std::shared_ptr<IImageReader> spImageReader,
      std::shared_ptr<Texture> spTexture, bool bGenerateMipmap);

   /// uploads last loaded image to texture; called in upload thread
   void Upload(std::shared_ptr<IImageReader> spImageReader, std::shared_ptr<Texture> spTexture, bool bGenerateMipmap);

private:
   /// task manager
   GraphicsTaskManager& m_taskManager;

   /// file system
   IFileSystem& m_fileSystem;
};
