//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/LevelGeometry.cpp Level geometry provider class
//

// includes
#include "stdafx.h"
#include "LevelGeometry.hpp"

using namespace Underworld;

/// \param xpos x tile position
/// \param ypos y tile position
/// \param vecLevelWalls vector of level walls
void LevelGeometry::GetWalls(unsigned int xpos, unsigned int ypos, std::vector<LevelWall>& vecLevelWalls)
{
   const TileInfo& tileInfo = m_tilemap.GetTileInfo(xpos, ypos);

   if (tileInfo.m_type == tileSolid)
      return; // no walls to generate

   AddDiagonalWalls(tileInfo, vecLevelWalls);

   // draw every wall side
   for (T_enTileWallSide enSide = sideLeft; enSide <= sideMax; enSide = static_cast<T_enTileWallSide>(enSide + 1))
   {
      // ignore some walls for diagonal wall tiles
      if ((tileInfo.m_type == tileDiagonal_se && (enSide == sideLeft  || enSide == sideFront)) ||
            (tileInfo.m_type==tileDiagonal_sw && (enSide == sideRight || enSide == sideFront)) ||
            (tileInfo.m_type==tileDiagonal_nw && (enSide == sideRight || enSide == sideBack)) ||
            (tileInfo.m_type==tileDiagonal_ne && (enSide == sideLeft  || enSide == sideBack)) )
         continue;

      // get current tile coordinates
      T_enTileWallPoint enPoint1, enPoint2;
      unsigned int zpos1, zpos2;

      GetTileCoords(enSide, tileInfo, enPoint1, zpos1, enPoint2, zpos2);

      // get adjacent tile coordinates
      unsigned int next_zpos1, next_zpos2;
      GetAdjacentCoords(enSide, tileInfo, xpos, ypos,next_zpos1, next_zpos2);

      // special case: no wall to draw
      if (zpos1 == next_zpos1 && zpos2 == next_zpos2)
         continue;

      // If the wall to the adjacent tile goes up (e.g. a stair),
      // we draw that wall. if it goes down, the adjacent tile has to
      // draw that wall. So we only draw walls that go up to another
      // tile or the ceiling.

      // determine if we should draw the wall
      if (next_zpos1 < zpos1 || next_zpos2 < zpos2)
         continue;

      //// now that we have all info, add the tile wall
      bool bWindingCcw = enSide == sideLeft || enSide == sideFront;
      vecLevelWalls.push_back(LevelWall(enPoint1, zpos1, next_zpos1, enPoint2, zpos2, next_zpos2, bWindingCcw));

   } // end for
}

void LevelGeometry::AddDiagonalWalls(const TileInfo& tileInfo, std::vector<LevelWall>& vecLevelWalls)
{
   unsigned int uiFloor = tileInfo.m_uiFloor;
   unsigned int uiCeiling = tileInfo.m_uiCeiling;

   switch (tileInfo.m_type)
   {
   case tileDiagonal_se: vecLevelWalls.push_back(LevelWall(wallPoint_0_0, uiFloor, uiCeiling, wallPoint_1_1, uiFloor, uiCeiling, true)); break;
   case tileDiagonal_sw: vecLevelWalls.push_back(LevelWall(wallPoint_0_1, uiFloor, uiCeiling, wallPoint_1_0, uiFloor, uiCeiling, true)); break;
   case tileDiagonal_nw: vecLevelWalls.push_back(LevelWall(wallPoint_1_1, uiFloor, uiCeiling, wallPoint_0_0, uiFloor, uiCeiling, true)); break;
   case tileDiagonal_ne: vecLevelWalls.push_back(LevelWall(wallPoint_1_0, uiFloor, uiCeiling, wallPoint_0_1, uiFloor, uiCeiling, true)); break;
   default:
      break;
   }
}

void LevelGeometry::GetTileCoords(T_enTileWallSide enSide,
                                  const TileInfo& tileInfo,
                                  T_enTileWallPoint& enPoint1, unsigned int& zpos1,
                                  T_enTileWallPoint& enPoint2, unsigned int& zpos2)
{
   // determine wall points
   switch (enSide)
   {
   case sideLeft:  enPoint1 = wallPoint_0_0; enPoint2 = wallPoint_0_1; break;
   case sideRight: enPoint1 = wallPoint_1_0; enPoint2 = wallPoint_1_1; break;
   case sideFront: enPoint1 = wallPoint_0_1; enPoint2 = wallPoint_1_1; break;
   case sideBack:  enPoint1 = wallPoint_0_0; enPoint2 = wallPoint_1_0; break;
   }

   // determine z coordinates
   zpos1 = zpos2 = tileInfo.m_uiFloor;

   Underworld::TilemapTileType enType = tileInfo.m_type;

   switch (enSide)
   {
   case sideLeft:
      if (enType == tileSlope_w || enType == tileSlope_s) zpos1 += tileInfo.m_uiSlope;
      if (enType == tileSlope_w || enType == tileSlope_n) zpos2 += tileInfo.m_uiSlope;
      // note: wall height set to tileInfo.m_uiCeiling
      // as this function is called for adjacent tile walls only
      if (enType == tileDiagonal_se || enType == tileDiagonal_ne)
         zpos1 = zpos2 = tileInfo.m_uiCeiling;
      break;

   case sideRight:
      if (enType == tileSlope_e || enType == tileSlope_s) zpos1 += tileInfo.m_uiSlope;
      if (enType == tileSlope_e || enType == tileSlope_n) zpos2 += tileInfo.m_uiSlope;
      if (enType == tileDiagonal_sw || enType == tileDiagonal_nw)
         zpos1 = zpos2 = tileInfo.m_uiCeiling;
      break;

   case sideFront:
      if (enType == tileSlope_n || enType == tileSlope_w) zpos1 += tileInfo.m_uiSlope;
      if (enType == tileSlope_n || enType == tileSlope_e) zpos2 += tileInfo.m_uiSlope;
      if (enType == tileDiagonal_se || enType == tileDiagonal_sw)
         zpos1 = zpos2 = tileInfo.m_uiCeiling;
      break;

   case sideBack:
      if (enType == tileSlope_s || enType == tileSlope_w) zpos1 += tileInfo.m_uiSlope;
      if (enType == tileSlope_s || enType == tileSlope_e) zpos2 += tileInfo.m_uiSlope;
      if (enType == tileDiagonal_nw || enType == tileDiagonal_ne)
         zpos1 = zpos2 = tileInfo.m_uiCeiling;
      break;
   }
}

void LevelGeometry::GetAdjacentCoords(T_enTileWallSide enSide,
   const Underworld::TileInfo& tileInfo,
   unsigned int xpos, unsigned int ypos,
   unsigned int& next_zpos1, unsigned int& next_zpos2)
{
   unsigned int next_xpos = xpos;
   unsigned int next_ypos = ypos;

   // when tile would be outside the map, take the tile with wrapped coords
   switch (enSide)
   {
   case sideLeft:  next_xpos = xpos == 0 ? c_uiUnderworldTilemapSize-1 : xpos - 1; break;
   case sideRight: next_xpos = xpos == c_uiUnderworldTilemapSize-1 ? 0 : xpos + 1; break;
   case sideFront: next_ypos = ypos == c_uiUnderworldTilemapSize-1 ? 0 : ypos + 1; break;
   case sideBack:  next_ypos = ypos == 0 ? c_uiUnderworldTilemapSize-1 : ypos - 1; break;
   default:
      ATLASSERT(false);
      break;
   }

   const TileInfo& nextTileInfo = m_tilemap.GetTileInfo(next_xpos, next_ypos);

   if (nextTileInfo.m_type == tileSolid)
   {
      // wall goes up to the ceiling
      next_zpos1 = next_zpos2 = nextTileInfo.m_uiCeiling;
   }
   else
   {
      // get z coordinates for the adjacent tile
      T_enTileWallSide enAdjacentSide = sideFront;
      switch (enSide)
      {
      case sideLeft: enAdjacentSide = sideRight; break;
      case sideRight: enAdjacentSide = sideLeft; break;
      case sideFront: enAdjacentSide = sideBack; break;
      case sideBack: enAdjacentSide = sideFront; break;
      default:
         ATLASSERT(false);
         break;
      }

      T_enTileWallPoint enDummy;
      GetTileCoords(enAdjacentSide, nextTileInfo,
         enDummy, next_zpos1, enDummy, next_zpos2);
         
      if (next_zpos1 == next_zpos2 && next_zpos2 == nextTileInfo.m_uiCeiling)
      {
         // GetTileCoords() returns this when the adjacent wall is a
         // diagonal wall. Assume the diagonal tile has the same
         // height as our current tile to render.

         next_zpos1 = next_zpos2 = tileInfo.m_uiCeiling;
      }
   }
}
