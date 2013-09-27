//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/Import/TextureLoader.hpp Texture loader
//
#pragma once

// includes
#include "IndexedImage.hpp"

// forward references
class IFileSystem;
class Palette256;

namespace Import
{

/// texture loader
class TextureLoader
{
public:
   /// ctor
   TextureLoader(IFileSystem& fileSystem)
      :m_fileSystem(fileSystem)
   {
   }

   /// loads all textures
   void Load(std::vector<IndexedImage>& vecImages,
      unsigned int startidx, LPCTSTR pszTextureFilename,
      std::shared_ptr<Palette256> spPalette);

private:
   /// file system
   IFileSystem& m_fileSystem;
};

} // namespace Import
