//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Import/LevelImporter.cpp Level importer
//

// includes
#include "stdafx.h"
#include "LevelImporter.hpp"
#include "LevelList.hpp"
#include "Level.hpp"
#include "Tilemap.hpp"
#include "Bits.hpp"
#include "ArchiveFile.hpp"
#include "ObjectListLoader.hpp"
#include "IFileSystem.hpp"
#include <ulib/stream/EndianAwareFilter.hpp>

using namespace Import;

// tables

/// maps type numbers read from lev.ark to level map tile type enum
Underworld::TilemapTileType g_aTileTypeMapping[16] =
{
   Underworld::tileSolid,
   Underworld::tileOpen,
   Underworld::tileDiagonal_se,
   Underworld::tileDiagonal_sw,
   Underworld::tileDiagonal_ne,
   Underworld::tileDiagonal_nw,
   Underworld::tileSlope_n,
   Underworld::tileSlope_s,
   Underworld::tileSlope_e,
   Underworld::tileSlope_w,
   Underworld::tileSolid,
   Underworld::tileSolid,
   Underworld::tileSolid,
   Underworld::tileSolid,
   Underworld::tileSolid,
   Underworld::tileSolid
};

// LevelImporter methods

void LevelImporter::LoadUwDemoLevel(Underworld::LevelList& levelList)
{
   UaTrace("importing uw_demo level map\n");

   std::vector<std::shared_ptr<Underworld::Level>>& vecLevels = levelList.GetVectorLevels();

   vecLevels.clear();
   vecLevels.resize(1);
   vecLevels[0].reset(new Underworld::Level);

   // load uw_demo texture map
   m_spFile = m_fileSystem.OpenFile(_T("data\\level13.txm"), true);

   std::vector<Uint16> vecTextureMapping;
   LoadTextureMapping(vecTextureMapping, false);

   // load tilemap
   m_spFile = m_fileSystem.OpenFile(_T("data\\level13.st"), true);

   TileStartLinkList tileStartLinkList;
   LoadTilemap(vecLevels[0]->GetTilemap(), vecTextureMapping, tileStartLinkList, false);

   // load object list
   LoadObjectList(vecLevels[0]->GetObjectList(), tileStartLinkList, vecTextureMapping);
}

void LevelImporter::LoadUw1Levels(Underworld::LevelList& levelList)
{
   LoadUwLevels(levelList, false, 9, 18);
}

void LevelImporter::LoadUw2Levels(Underworld::LevelList& levelList)
{
   LoadUwLevels(levelList, true, 80, 80);
}

void LevelImporter::LoadUwLevels(Underworld::LevelList& levelList, bool bUw2Mode, unsigned int uiNumLevels, unsigned int uiTexMapOffset)
{
   std::vector<std::shared_ptr<Underworld::Level>>& vecLevels = levelList.GetVectorLevels();

   vecLevels.clear();
   vecLevels.resize(uiNumLevels);

   std::shared_ptr<Stream::IStream> spArkFile = m_fileSystem.OpenFile(_T("data\\level13.st"), true);

   Import::ArchiveFile levArkFile(spArkFile, bUw2Mode);

   for (unsigned int ui = 0; ui < uiNumLevels; ui++)
   {
      if (!levArkFile.IsAvailable(ui))
         continue;

      vecLevels[ui].reset(new Underworld::Level);

      Underworld::Level& level = *vecLevels[ui];

      // load texture mapping
      UaAssert(true == levArkFile.IsAvailable(ui + uiTexMapOffset));
      m_spFile = levArkFile.GetFile(ui + uiTexMapOffset);

      std::vector<Uint16> vecTextureMapping;
      LoadTextureMapping(vecTextureMapping, false);

      // load tilemap
      m_spFile = levArkFile.GetFile(ui);

      TileStartLinkList tileStartLinkList;
      LoadTilemap(level.GetTilemap(), vecTextureMapping, tileStartLinkList, bUw2Mode);

      // load object list
      LoadObjectList(level.GetObjectList(), tileStartLinkList, vecTextureMapping);
   }
}

/// The texture mapping is stored in the vecTextureMapping as indices into
/// stock textures. In uw1 mode there are 48 wall textures, followed by 10
/// floor textures, followed by 6 empty entries, ending with 6 door textures.
/// In uw2 mode there are 64 textures used for wall and floor (floor textures
/// only use a 4 bit mapping and so only use the first 16 entries), followed
/// by 6 door textures. vecTextureMapping always contains 70 entries after
/// loading.
void LevelImporter::LoadTextureMapping(std::vector<Uint16>& vecTextureMapping, bool bUw2Mode)
{
   Stream::EndianAwareFilter filter(*m_spFile);

   if (!bUw2Mode)
   {
      // uw1 mapping
      vecTextureMapping.resize(48 + 10);

      // wall textures
      for (unsigned int uiTex = 0; uiTex < 48; uiTex++)
         vecTextureMapping[uiTex] = filter.Read16LE() + Underworld::c_uiStockTexturesWall;

      // floor textures
      for (unsigned int uiTex = 48; uiTex < 48 + 10; uiTex++)
         vecTextureMapping[uiTex] = filter.Read16LE() + Underworld::c_uiStockTexturesFloor;

      vecTextureMapping.resize(64);
   }
   else
   {
      // uw2 mapping
      vecTextureMapping.resize(64);

      // combined wall/floor textures
      for (unsigned int uiTex = 0; uiTex < 64; uiTex++)
         vecTextureMapping[uiTex] = filter.Read16LE();
   }

   // door textures
   for (unsigned int uiTex=0; uiTex < 6; uiTex++)
      vecTextureMapping.push_back(m_spFile->ReadByte() + Underworld::c_uiStockTexturesDoors);

   ATLASSERT(vecTextureMapping.size() == 70);
}

void LevelImporter::LoadTilemap(Underworld::Tilemap& tilemap, std::vector<Uint16>& vecTextureMapping,
   Import::TileStartLinkList& tileStartLinkList, bool bUw2Mode)
{
   tilemap.Destroy();
   tilemap.Create();

   Stream::EndianAwareFilter filter(*m_spFile);

   // read in map info
   for (Uint16 ypos = 0; ypos < 64; ypos++)
   for (Uint16 xpos = 0; xpos < 64; xpos++)
   {
      Underworld::TileInfo& tileInfo = tilemap.GetTileInfo(xpos, ypos);

      Uint32 uiTileInfo1 = filter.Read16LE();
      Uint32 uiTileInfo2 = filter.Read16LE();

      // extract infos from tile word
      tileInfo.m_type = g_aTileTypeMapping[GetBits(uiTileInfo1, 0, 4)];

      // all size values in height units
      tileInfo.m_uiFloor = static_cast<Uint16>(GetBits(uiTileInfo1, 4, 4) << 3);
      tileInfo.m_uiCeiling = 128;
      tileInfo.m_uiSlope = 8; // height units per tile

      // texture indices
      Uint8 uiFloorIndex = static_cast<Uint8>(GetBits(uiTileInfo1, 10, 4)); // 4 bit wide
      Uint8 uiWallIndex = static_cast<Uint8>(GetBits(uiTileInfo2, 0, 6)); // 6 bit wide

      if (!bUw2Mode)
      {
         // restrict to proper texture map indices
         if (uiFloorIndex >= 10)
            uiFloorIndex = 0;
         if (uiWallIndex >= 48)
            uiWallIndex = 0;
      }

      tileInfo.m_uiTextureWall = vecTextureMapping[uiWallIndex];
      tileInfo.m_uiTextureFloor = vecTextureMapping[uiFloorIndex + (bUw2Mode ? 0 : 48)];
      tileInfo.m_uiTextureCeiling = vecTextureMapping[bUw2Mode ? 32 : (9+48)];

      // tile object list start
      Uint16 uiLink = static_cast<Uint16>(GetBits(uiTileInfo2, 6, 10));
      tileStartLinkList.SetLinkStart(xpos, ypos, uiLink);

      // special flags
      tileInfo.m_bMagicDisabled = GetBits(uiTileInfo1, 14, 1) != 0;
      tileInfo.m_bDoorPresent = GetBits(uiTileInfo1, 15, 1) != 0;
      tileInfo.m_bSpecialLightFeature = GetBits(uiTileInfo1, 8, 1) != 0;

      // bit 9 is always 0
      UaAssert(0 == GetBits(uiTileInfo1, 9, 1));
   }
}

void Import::LevelImporter::LoadObjectList(Underworld::ObjectList& objectList,
   const TileStartLinkList& tileStartLinkList, std::vector<Uint16>& vecTextureMapping)
{
   objectList.Destroy();
   objectList.Create();

   Stream::EndianAwareFilter filter(*m_spFile);

   // read in master object list
   std::vector<Uint16> vecObjectWords;
   vecObjectWords.reserve(0x0400 * 4);

   std::vector<Uint8> vecNpcInfos;
   vecNpcInfos.resize(0x0100 * 19);

   for(Uint16 uiItem=0; uiItem < 0x400; uiItem++)
   {
      vecObjectWords.push_back(filter.Read16LE());
      vecObjectWords.push_back(filter.Read16LE());
      vecObjectWords.push_back(filter.Read16LE());
      vecObjectWords.push_back(filter.Read16LE());

      // read NPC info bytes
      if (uiItem < 0x100)
      {
         DWORD dwBytesRead = 0;
         m_spFile->Read(&vecNpcInfos[uiItem * 19], 19, dwBytesRead);
      }
   }

   ObjectListLoader loader(objectList, vecObjectWords, vecNpcInfos, vecTextureMapping);

   // follow each reference in all tiles
   for (Uint8 ypos = 0; ypos < 64; ypos++)
   for (Uint8 xpos = 0; xpos < 64; xpos++)
   {
      Uint16 uiLink = tileStartLinkList.GetLinkStart(xpos, ypos);

      if (uiLink != 0)
      {
         loader.FollowLink(uiLink, xpos, ypos);

         objectList.SetListStart(uiLink, xpos, ypos);
      }
   }
}
