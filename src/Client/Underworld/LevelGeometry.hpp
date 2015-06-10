//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/LevelGeometry.hpp Level geometry provider class
//
#pragma once

// includes
#include "Level.hpp"

/// tile wall side
enum T_enTileWallSide
{
   sideLeft = 0, sideRight, sideFront, sideBack,
   sideMax = sideBack
};

/// tile wall point, relative to tile
enum T_enTileWallPoint
{
   wallPoint_0_0, ///< point at (0,0)
   wallPoint_1_0, ///< point at (1,0)
   wallPoint_0_1, ///< point at (0,1)
   wallPoint_1_1, ///< point at (1,1)
};

// classes

/// describes a single level wall
struct LevelWall
{
public:
   /// ctor
   LevelWall(T_enTileWallPoint enPoint1, unsigned int zposLo1, unsigned int zposHi1,
             T_enTileWallPoint enPoint2, unsigned int zposLo2, unsigned int zposHi2,
             bool bWindingCcw)
      :m_enPoint1(enPoint1),
       m_zposLo1(zposLo1),
       m_zposHi1(zposHi1),
       m_enPoint2(enPoint2),
       m_zposLo2(zposLo2),
       m_zposHi2(zposHi2),
       m_bWindingCcw(bWindingCcw)
   {
   }

   T_enTileWallPoint m_enPoint1; ///< wall point 1 in tile
   unsigned int m_zposLo1;       ///< low z position for point 1
   unsigned int m_zposHi1;       ///< high z position for point 1

   T_enTileWallPoint m_enPoint2; ///< wall point 2 in tile
   unsigned int m_zposLo2;       ///< low z position for point 1
   unsigned int m_zposHi2;       ///< high z position for point 1

   bool m_bWindingCcw; ///< winding is counter-clockwise
};

/// level geometry provider
class LevelGeometry
{
public:
   /// ctor
   LevelGeometry(const Underworld::Tilemap& tilemap)
      :m_tilemap(tilemap)
   {
   }

   /// returns walls for given position
   void GetWalls(unsigned int xpos, unsigned int ypos, std::vector<LevelWall>& vecLevelWalls);

private:
   /// adds diagonal walls
   void AddDiagonalWalls(const Underworld::TileInfo& tileInfo, std::vector<LevelWall>& vecLevelWalls);

   /// gets tile coordinates for given side
   void GetTileCoords(T_enTileWallSide enSide,
      const Underworld::TileInfo& tileInfo,
      T_enTileWallPoint& enPoint1, unsigned int& zpos1,
      T_enTileWallPoint& enPoint2, unsigned int& zpos2);

   /// gets tile coordinates for adjacent tile on given side
   void GetAdjacentCoords(T_enTileWallSide enSide,
      const Underworld::TileInfo& tileInfo,
      unsigned int xpos, unsigned int ypos,
      unsigned int& next_zpos1, unsigned int& next_zpos2);

private:
   /// tilemap
   const Underworld::Tilemap& m_tilemap;
};
