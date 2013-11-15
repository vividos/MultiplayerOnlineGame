//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TextureLoader.cpp Texture loader class
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

   CString cszFilenameLower(cszFilename);
   cszFilenameLower.MakeLower();

   std::shared_ptr<IImageReader> spImageReader;

   if (cszFilenameLower.Right(4) == _T(".pcx"))
      spImageReader.reset(new PcxImageReader);

   if (cszFilenameLower.Right(4) == _T(".tga"))
      spImageReader.reset(new TgaImageReader);

   if (cszFilenameLower.Right(4) == _T(".png"))
      spImageReader.reset(new PngImageReader);

   if (cszFilenameLower.Right(4) == _T(".jpg"))
      spImageReader.reset(new JpegImageReader);

   if (spImageReader == NULL)
      throw Exception(_T("image file type not supported; filename: ") + cszFilename, __FILE__, __LINE__);

   spImageReader->Load(*m_fileSystem.OpenFile(cszFilename, true));

   m_taskManager.UploadTaskGroup().Add(
      std::bind(&TextureLoader::Upload, this, spImageReader, spTexture, bGenerateMipmap));
}

void TextureLoader::Upload(std::shared_ptr<IImageReader> spImageReader, std::shared_ptr<Texture> spTexture, bool bGenerateMipmap)
{
   ATLASSERT(spTexture != nullptr);

   Bitmap bmp(spImageReader->Width(), spImageReader->Height(), &spImageReader->Pixels()[0]);

   if (!spTexture->IsValid())
      spTexture->Generate();

   spTexture->Upload(bmp, bGenerateMipmap);
}
