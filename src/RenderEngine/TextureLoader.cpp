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
#include "Texture.hpp"
#include "Bitmap.hpp"
#include "IFileSystem.hpp"
#include "ImageReader\PcxImageReader.hpp"
#include "ImageReader\TgaImageReader.hpp"
#include "ImageReader\PngImageReader.hpp"
#include "ImageReader\JpegImageReader.hpp"
#include <ulib/stream/FileStream.hpp>

TextureLoader::TextureLoader(IFileSystem& fileSystem)
:m_fileSystem(fileSystem)
{
}

void TextureLoader::Load(const CString& cszFilename)
{
   CString cszFilenameLower(cszFilename);
   cszFilenameLower.MakeLower();

   if (cszFilenameLower.Right(4) == _T(".pcx"))
      m_spImageReader.reset(new PcxImageReader);

   if (cszFilenameLower.Right(4) == _T(".tga"))
      m_spImageReader.reset(new TgaImageReader);

   if (cszFilenameLower.Right(4) == _T(".png"))
      m_spImageReader.reset(new PngImageReader);

   if (cszFilenameLower.Right(4) == _T(".jpg"))
      m_spImageReader.reset(new JpegImageReader);

   if (m_spImageReader == NULL)
      throw Exception(_T("image file type not supported; filename: ") + cszFilename, __FILE__, __LINE__);

   m_spImageReader->Load(*m_fileSystem.OpenFile(cszFilename, true));
}

void TextureLoader::Upload(Texture& tex, bool bGenerateMipmap)
{
   if (m_spImageReader == NULL)
      throw Exception(_T("no image previously loaded"), __FILE__, __LINE__);

   Bitmap bmp(m_spImageReader->Width(), m_spImageReader->Height(), &m_spImageReader->Pixels()[0]);

   tex.Upload(bmp, bGenerateMipmap);

   m_spImageReader.reset();
}
