//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file  Underworld/GameLogic/Tilemap.hpp Level tilemap
//
#pragma once

// includes
#include <vector>
#include <set>

namespace Underworld
{
// constants

/// default tilemap size for underworld games
static const unsigned int c_uiUnderworldTilemapSize = 64;

// enums

/// tile types
enum TilemapTileType
{
   tileSolid = 0x00,       ///< solid tile
   tileOpen  = 0x01,       ///< open tile; player can go through tile
   tileDiagonal_se = 0x02, ///< diagonal wall, open corner to the lower right
   tileDiagonal_sw = 0x03, ///< diagonal wall, open corner to the lower left
   tileDiagonal_nw = 0x04, ///< diagonal wall, open corner to the upper left
   tileDiagonal_ne = 0x05, ///< diagonal wall, open corner to the upper right
   tileSlope_n = 0x06,     ///< open tile with sloped floor going up north
   tileSlope_e = 0x07,     ///< open tile with sloped floor going up east
   tileSlope_s = 0x08,     ///< open tile with sloped floor going up south
   tileSlope_w = 0x09      ///< open tile with sloped floor going up west
};

// structs

/// tilemap tile info
struct TileInfo
{
   /// ctor
   TileInfo()
      :m_type(tileSolid),
      m_uiFloor(0),
      m_uiCeiling(128),
      m_uiSlope(8),
      m_uiTextureWall(c_uiStockTexturesWall),
      m_uiTextureFloor(c_uiStockTexturesFloor),
      m_uiTextureCeiling(c_uiStockTexturesFloor),
      m_bMagicDisabled(false),
      m_bDoorPresent(false),
      m_bSpecialLightFeature(false)
   {
   }

   /// tile type
   TilemapTileType m_type;

   /// floor height
   Uint16 m_uiFloor;

   /// ceiling height
   Uint16 m_uiCeiling;

   /// slope from this tile to next
   Uint8 m_uiSlope;

   /// texture id for wall
   Uint16 m_uiTextureWall;

   /// texture id for floor
   Uint16 m_uiTextureFloor;

   /// texture id for ceiling
   Uint16 m_uiTextureCeiling;

   /// indicates if magic is disabled in this tile
   bool m_bMagicDisabled;

   /// indicates if a door is present in this tile
   bool m_bDoorPresent;

   /// special light feature \todo rename to a better name
   bool m_bSpecialLightFeature;
};


// classes

/// tilemap
class Tilemap
{
public:
   /// ctor
   Tilemap()
      :m_bUsed(false)
   {
   }

   /// creates new tilemap
   void Create()
   {
      m_vecTiles.clear();
      m_vecTiles.resize(64*64);
      m_setUsedTextures.clear();
      m_bUsed = true;
   }

   /// destroy all tiles in tilemap
   void Destroy()
   {
      m_vecTiles.clear();
      m_setUsedTextures.clear();
      m_bUsed = false;
   }

   /// returns floor height on specific position
   double GetFloorHeight(double xpos, double ypos);

   /// returns a tile info struct
   TileInfo& GetTileInfo(unsigned int xpos, unsigned int ypos);

   /// returns a tile info struct
   const TileInfo& GetTileInfo(unsigned int xpos, unsigned int ypos) const;

   /// returns vector of used stock texture ids
   std::set<Uint16>& GetUsedTextures(){ return m_setUsedTextures; }

   // loading / saving

   /// loads tilemap from savegame
   void Load(UwBase::Savegame& sg);

   /// saves tilemap to savegame
   void Save(UwBase::Savegame& sg) const;

   /// returns tiles list
   std::vector<TileInfo>& GetVectorTileInfo(){ return m_vecTiles; }

protected:
   /// all levelmap tiles; 64x64 tiles always assumed
   std::vector<TileInfo> m_vecTiles;

   /// set with all used texture ids
   std::set<Uint16> m_setUsedTextures;

   /// indicates if tilemap is filled with actual tiles
   bool m_bUsed;
};

} // namespace Underworld
