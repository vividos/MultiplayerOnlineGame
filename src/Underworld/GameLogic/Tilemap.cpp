//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file  Underworld/GameLogic/Tilemap.cpp Tilemap
//

// includes
#include "stdafx.h"
#include "Tilemap.hpp"
#include "Savegame.hpp"
#include <cmath>

using Underworld::Tilemap;
using Underworld::TileInfo;

// Tilemap methods

double Tilemap::GetFloorHeight(double xpos, double ypos)
{
   if (xpos < 0.0 || xpos >= 64.0 || ypos < 0.0 || ypos >= 64.0)
      return 0.0;

   double dHeight = 0.0;

   TileInfo& tile = GetTileInfo(static_cast<Uint8>(xpos), static_cast<Uint8>(ypos));

   switch (tile.m_type)
   {
   case tileSolid:
      dHeight = tile.m_uiCeiling; // player shouldn't get there, though
      break;

   case tileOpen:
      dHeight = tile.m_uiFloor;
      break;

      // diagonal tiles
   case tileDiagonal_se:
      if (fmod(xpos, 1.0) - fmod(ypos, 1.0) < 0.0)
         dHeight = tile.m_uiCeiling;
      else
         dHeight = tile.m_uiFloor;
      break;
   case tileDiagonal_sw:
      if (fmod(xpos, 1.0) + fmod(ypos, 1.0) > 1.0)
         dHeight = tile.m_uiCeiling;
      else
         dHeight = tile.m_uiFloor;
      break;
   case tileDiagonal_nw:
      if (fmod(xpos, 1.0) - fmod(ypos, 1.0) > 0.0)
         dHeight = tile.m_uiCeiling;
      else
         dHeight = tile.m_uiFloor;
      break;
   case tileDiagonal_ne:
      if (fmod(xpos, 1.0) + fmod(ypos, 1.0) < 1.0)
         dHeight = tile.m_uiCeiling;
      else
         dHeight = tile.m_uiFloor;
      break;

      // sloped tiles
   case tileSlope_n:
      dHeight = tile.m_uiFloor +
         static_cast<double>(tile.m_uiSlope) * fmod(ypos, 1.0);
      break;
   case tileSlope_s:
      dHeight = (tile.m_uiFloor+tile.m_uiSlope) -
         static_cast<double>(tile.m_uiSlope)*fmod(ypos,1.0);
      break;
   case tileSlope_e:
      dHeight = tile.m_uiFloor +
         static_cast<double>(tile.m_uiSlope)*fmod(xpos,1.0);
      break;
   case tileSlope_w:
      dHeight = (tile.m_uiFloor+tile.m_uiSlope) -
         static_cast<double>(tile.m_uiSlope)*fmod(xpos,1.0);
      break;

   default:
      ATLASSERT(false);
      break;
   };

   return dHeight;
}

TileInfo& Tilemap::GetTileInfo(unsigned int xpos, unsigned int ypos)
{
   xpos %= 64; ypos %= 64;
   return m_vecTiles[ypos*64 + xpos];
}

const TileInfo& Tilemap::GetTileInfo(unsigned int xpos, unsigned int ypos) const
{
   xpos %= 64; ypos %= 64;
   return m_vecTiles[ypos*64 + xpos];
}

void Tilemap::Load(UwBase::Savegame& sg)
{
   sg.BeginSection(_T("tilemap"));

   m_bUsed = sg.Read8() != 0;

   if (!m_bUsed)
      return; // don't read empty tilemaps

   // read tilemap
   Create();

   for(unsigned int n=0; n<64*64; n++)
   {
      TileInfo& tile = m_vecTiles[n];

      tile.m_type = static_cast<TilemapTileType>(sg.Read8());
      tile.m_uiFloor = sg.Read16();
      tile.m_uiCeiling = sg.Read16();
      tile.m_uiSlope = sg.Read8();
      tile.m_uiTextureWall = sg.Read16();
      tile.m_uiTextureFloor = sg.Read16();
      tile.m_uiTextureCeiling = sg.Read16();

      m_setUsedTextures.insert(tile.m_uiTextureWall);
      m_setUsedTextures.insert(tile.m_uiTextureFloor);
      m_setUsedTextures.insert(tile.m_uiTextureCeiling);
   }

   sg.EndSection();
}

void Tilemap::Save(UwBase::Savegame& sg) const
{
   sg.BeginSection(_T("tilemap"));

   sg.Write8(m_bUsed ? 1 : 0);

   if (!m_bUsed)
      return; // don't write empty tilemaps

   // write tilemap
   for(unsigned int n=0; n<64*64; n++)
   {
      const TileInfo& tile = m_vecTiles[n];

      sg.Write8(static_cast<Uint8>(tile.m_type));
      sg.Write16(tile.m_uiFloor);
      sg.Write16(tile.m_uiCeiling);
      sg.Write8(tile.m_uiSlope);
      sg.Write16(tile.m_uiTextureWall);
      sg.Write16(tile.m_uiTextureFloor);
      sg.Write16(tile.m_uiTextureCeiling);
   }

   sg.EndSection();
}
