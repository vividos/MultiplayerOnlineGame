//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RenderEngine/TextureLoader.cpp Texture loader class
//
#pragma once

// includes
#include "StdAfx.h"
#include "TextureLoader.hpp"
#include "GraphicsTaskManager.hpp"
#include "Texture.hpp"
#include "Bitmap.hpp"
#include "IFileSystem.hpp"
#include "ImageReader\PcxImageReader.hpp"
#include "ImageReader\TgaImageReader.hpp"
#include "ImageReader\PngImageReader.hpp"
#include "ImageReader\JpegImageReader.hpp"

TextureLoader::TextureLoader(GraphicsTaskManager& taskManager, IFileSystem& fileSystem)
:m_taskManager(taskManager),
 m_fileSystem(fileSystem)
{
}

void TextureLoader::Load(const CString& cszFilename, std::shared_ptr<Texture> spTexture, bool bGenerateMipmap)
{
   ATLASSERT(spTexture != nullptr);

   std::shared_ptr<IImageReader> spImageReader = SelectImageReader(cszFilename.Right(4));

   if (spImageReader == NULL)
      throw Exception(_T("image file type not supported; filename: ") + cszFilename, __FILE__, __LINE__);

   std::shared_ptr<Stream::IStream> spStream = m_fileSystem.OpenFile(cszFilename, true);

   LoadInternal(spStream, spImageReader, spTexture, bGenerateMipmap);
}

void TextureLoader::Load(std::shared_ptr<Stream::IStream> spStream, const CString& cszExtensionWithDot,
   std::shared_ptr<Texture> spTexture, bool bGenerateMipmap)
{
   std::shared_ptr<IImageReader> spImageReader = SelectImageReader(cszExtensionWithDot);

   if (spImageReader == NULL)
      throw Exception(_T("image file type not supported; extension: ") + cszExtensionWithDot, __FILE__, __LINE__);

   LoadInternal(spStream, spImageReader, spTexture, bGenerateMipmap);
}

void TextureLoader::LoadInternal(std::shared_ptr<Stream::IStream> spStream,
   std::shared_ptr<IImageReader> spImageReader,
   std::shared_ptr<Texture> spTexture,
   bool bGenerateMipmap)
{
   spImageReader->Load(*spStream);

   m_taskManager.UploadTaskGroup().Add(
      std::bind(&TextureLoader::Upload, this, spImageReader, spTexture, bGenerateMipmap));
}

std::shared_ptr<IImageReader> TextureLoader::SelectImageReader(const CString& cszExtensionWithDot)
{
   CString cszExtension(cszExtensionWithDot);
   cszExtension.MakeLower();

   std::shared_ptr<IImageReader> spImageReader;

   if (cszExtension == _T(".pcx"))
      spImageReader.reset(new PcxImageReader);

   if (cszExtension == _T(".tga"))
      spImageReader.reset(new TgaImageReader);

   if (cszExtension == _T(".png"))
      spImageReader.reset(new PngImageReader);

   if (cszExtension == _T(".jpg"))
      spImageReader.reset(new JpegImageReader);

   return spImageReader;
}

void TextureLoader::Upload(std::shared_ptr<IImageReader> spImageReader, std::shared_ptr<Texture> spTexture, bool bGenerateMipmap)
{
   ATLASSERT(spTexture != nullptr);

   Bitmap bmp(spImageReader->Width(), spImageReader->Height(), &spImageReader->Pixels()[0]);

   if (!spTexture->IsValid())
      spTexture->Generate();

   spTexture->Upload(bmp, bGenerateMipmap);
}
