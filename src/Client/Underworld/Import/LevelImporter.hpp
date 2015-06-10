//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Import/LevelImporter.hpp Level importer
//
#pragma once

// includes
#include <vector>

// forward references
class IFileSystem;

namespace Stream
{
class IStream;
}

namespace Underworld
{
   class ObjectProperties;
   class LevelList;
   class ObjectList;
   class Tilemap;
   class Player;
}

/// import classes
namespace Import
{

/// stores tilemap object list start links; used while loading
class TileStartLinkList
{
public:
   /// ctor
   TileStartLinkList(){ m_vecLinks.resize(64*64); }

   /// returns link start for given tile
   Uint16 GetLinkStart(Uint16 xpos, Uint16 ypos) const { return m_vecLinks[ypos*64+xpos]; }

   /// sets link start for given tile
   void SetLinkStart(Uint16 xpos, Uint16 ypos, Uint16 uiLink){ m_vecLinks[ypos*64+xpos] = uiLink; }

private:
   /// array with links
   std::vector<Uint16> m_vecLinks;
};


/// imports levels
class LevelImporter
{
public:
   /// ctor
   LevelImporter(IFileSystem& fileSystem)
      :m_fileSystem(fileSystem)
   {}

   /// loads uw_demo level into level list
   void LoadUwDemoLevel(Underworld::LevelList& levelList);

   /// loads uw1 levels into level list
   void LoadUw1Levels(Underworld::LevelList& levelList);

   /// loads uw2 levels into level list
   void LoadUw2Levels(Underworld::LevelList& levelList);

private:
   /// common uw1 and uw2 level loading
   void LoadUwLevels(Underworld::LevelList& levelList, bool bUw2Mode,
      unsigned int uiNumLevels, unsigned int uiTexMapOffset);

   /// loads texture mapping from current file
   void LoadTextureMapping(std::vector<Uint16>& vecTextureMapping, bool bUw2Mode);

   /// loads tilemap from current file
   void LoadTilemap(Underworld::Tilemap& tilemap, std::vector<Uint16>& vecTextureMapping,
      TileStartLinkList& tileStartLinkList, bool bUw2Mode);

   /// loads object list from current file
   void LoadObjectList(Underworld::ObjectList& objectList, const TileStartLinkList& tileStartLinkList,
      std::vector<Uint16>& vecTextureMapping);

private:
   /// file system
   IFileSystem& m_fileSystem;

   /// current file
   std::shared_ptr<Stream::IStream> m_spFile;
};

} // namespace Import
