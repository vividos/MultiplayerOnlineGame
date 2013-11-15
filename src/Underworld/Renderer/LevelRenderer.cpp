//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/Renderer/LevelRenderer.cpp Underworld level renderer
//

// includes
#include "stdafx.h"
#include "LevelRenderer.hpp"
#include "TextureAtlas.hpp"
#include "Underworld.hpp"
#include "TexCoord2f.hpp"
#include "OpenGL.hpp"
#include "LevelGeometry.hpp"

using namespace Underworld;

void LevelRenderer::Prepare(const TextureAtlas& atlas, const Level& level)
{
   PrepareTilemap(atlas, level);
}

void LevelRenderer::Upload()
{
   ATLASSERT(OpenGL::IsRenderThread());

   m_vertexBuffer.Upload(true);
   m_indexBuffer.Upload(true);
}

void LevelRenderer::Render()
{
   ATLASSERT(OpenGL::IsRenderThread());

   m_vertexBuffer.Bind();
   m_indexBuffer.Bind();

   m_indexBuffer.Render();

   m_indexBuffer.Unbind();
   m_vertexBuffer.Unbind();
}

void LevelRenderer::PrepareTilemap(const TextureAtlas& atlas, const Level& level)
{
   const Underworld::Tilemap& tilemap = level.GetTilemap();

   for (unsigned int xpos = 0; xpos < c_uiUnderworldTilemapSize; xpos++)
   for (unsigned int ypos = 0; ypos < c_uiUnderworldTilemapSize; ypos++)
   {
      PrepareTile(atlas, tilemap, xpos, ypos);
   }
}

void LevelRenderer::PrepareTile(const TextureAtlas& atlas, const Tilemap& tilemap, unsigned int xpos, unsigned int ypos)
{
   const TileInfo& tileInfo = tilemap.GetTileInfo(xpos, ypos);
   if (tileInfo.m_type == tileSolid)
      return;

   LevelGeometry geometry(tilemap);

   std::vector<LevelWall> vecLevelWalls;
   geometry.GetWalls(xpos, ypos, vecLevelWalls);

   // add all walls
   std::for_each(vecLevelWalls.begin(), vecLevelWalls.end(), [&](const LevelWall& wall)
   {
      AddLevelWall(atlas, xpos, ypos, tileInfo, wall);
   });

   AddLevelFloorCeiling(atlas, xpos, ypos, tileInfo);
}

void LevelRenderer::AddLevelFloorCeiling(const TextureAtlas& atlas, unsigned int xpos, unsigned int ypos, const TileInfo& tileInfo)
{
   // floor
   {
      //  [3]    [2]     zpos[x]
      //    +-----+      [0]: (0/0)         N
      //    |     |      [1]: (1/0)         ^
      //    |     |      [2]: (1/1)    W <--+--> E
      //    +-----+      [3]: (0/1)         v
      //  [0]     [1]                       S

      std::array<unsigned int, 4> zpos;
      zpos.fill(tileInfo.m_uiFloor);

      bool bDiagonal = false;
      unsigned int uiDiagPointToOmit = 0;

      // TODO move to LevelGeometry
      // add slope when needed
      switch (tileInfo.m_type)
      {
      case tileSlope_n: zpos[2] += tileInfo.m_uiSlope; zpos[3] += tileInfo.m_uiSlope; break;
      case tileSlope_e: zpos[1] += tileInfo.m_uiSlope; zpos[2] += tileInfo.m_uiSlope; break;
      case tileSlope_s: zpos[0] += tileInfo.m_uiSlope; zpos[1] += tileInfo.m_uiSlope; break;
      case tileSlope_w: zpos[0] += tileInfo.m_uiSlope; zpos[3] += tileInfo.m_uiSlope; break;
      case tileDiagonal_se: bDiagonal = true; uiDiagPointToOmit = 3; break; // diagonal wall, open corner to the lower right
      case tileDiagonal_sw: bDiagonal = true; uiDiagPointToOmit = 2; break; // diagonal wall, open corner to the lower left
      case tileDiagonal_nw: bDiagonal = true; uiDiagPointToOmit = 1; break; // diagonal wall, open corner to the upper left
      case tileDiagonal_ne: bDiagonal = true; uiDiagPointToOmit = 0; break; // diagonal wall, open corner to the upper right
      default:
         break;
      }

      // vector points
      std::array<Vector3d, 4> aPoints;
      aPoints[0] = Vector3d(-(xpos + 0.0), zpos[0] / 32.0, ypos + 0.0);
      aPoints[1] = Vector3d(-(xpos + 1.0), zpos[1] / 32.0, ypos + 0.0);
      aPoints[2] = Vector3d(-(xpos + 1.0), zpos[2] / 32.0, ypos + 1.0);
      aPoints[3] = Vector3d(-(xpos + 0.0), zpos[3] / 32.0, ypos + 1.0);

      // normal
      Vector3d v1 = Vector3d(0.0, zpos[3], 1.0) - Vector3d(0.0, zpos[0], 0.0);
      Vector3d v2 = Vector3d(1.0, zpos[1], 0.0) - Vector3d(0.0, zpos[0], 0.0);
      Vector3d vNormal = v1.Cross(v2);
      vNormal.Normalize();

      // texture
      std::array<TexCoord2f, 4> aTexCoords;
      atlas.GetUV(tileInfo.m_uiTextureFloor, aTexCoords[0], aTexCoords[2]);
      aTexCoords[1] = TexCoord2f(aTexCoords[2].U(), aTexCoords[0].V());
      aTexCoords[3] = TexCoord2f(aTexCoords[0].U(), aTexCoords[2].V());

      Color col = Color::White();

      // add vertices
      std::vector<VertexBufferEntry>& vecVertices = m_vertexBuffer.Vertices();

      size_t uiIndexBase = vecVertices.size();

      for (size_t i=0; i<4; i++)
      {
         if (bDiagonal && i == uiDiagPointToOmit)
            continue;

         vecVertices.push_back(VertexBufferEntry(aPoints[i], vNormal, aTexCoords[i], col));
      }

      std::vector<unsigned int>& vecIndices = m_indexBuffer.IndexBuffer();

      vecIndices.push_back(uiIndexBase + 0);
      vecIndices.push_back(uiIndexBase + 1);
      vecIndices.push_back(uiIndexBase + 2);

      if (!bDiagonal) // no second triangle on diagonal
      {
         vecIndices.push_back(uiIndexBase + 0);
         vecIndices.push_back(uiIndexBase + 2);
         vecIndices.push_back(uiIndexBase + 3);
      }
   }
}

static Vector3d CalcPosFromWallPoint(unsigned int xpos, unsigned int ypos, unsigned int zpos, T_enTileWallPoint enPoint)
{
   double x = xpos, y = ypos;
   switch (enPoint)
   {
   case wallPoint_1_0: x += 1.0; break;
   case wallPoint_0_1: y += 1.0; break;
   case wallPoint_1_1: x += 1.0; y += 1.0; break;
   default:
      break;
   }

   return Vector3d(-x, zpos / 32.0, y);
}

static TexCoord2f CalcTexCoordFromWallPoint(const TexCoord2f& tcMin, const TexCoord2f& tcMax, double deltaU, unsigned int zpos, unsigned int uiCeiling)
{
   // ceiling is the "base" for textures; for wall textures a texture is
   // mapped to 32.0 height units

   LinearTransform<double> transU(0.0, 1.0, tcMin.U(), tcMax.U());
   double u = transU(deltaU);

   LinearTransform<double> transV(0.0, 1.0, tcMin.V(), tcMax.V());
//   unsigned int dist = ((uiCeiling - zpos + 31) % 32 ) +1;

   double v = (uiCeiling - zpos) / 32.0;
   //double v = (uiCeiling - zpos)/32.0;
   v = fmod(v, 1.0);
   //if (v == 0.0 && zpos != uiCeiling)
   //   v = 1.0;

   v = transV(v);

   return TexCoord2f(float(u), float(v));
}

/// specialisation of Interpolate<T>() for type TexCoord2f
template <>
TexCoord2f Interpolate(TexCoord2f val1, TexCoord2f val2, double dDelta)
{
   ATLASSERT(dDelta >= 0.0 && dDelta <= 1.0);
   return TexCoord2f(
      Interpolate(val1.U(), val2.U(), dDelta),
      Interpolate(val1.V(), val2.V(), dDelta));
}

/// \note some special cases currently are not implemented, among others:
/// - z coordinates not divisable by 8
/// - slopes other than with value 8
/// - downward slope on one tile, and upward slope on the other; would result in two half size triangles
void LevelRenderer::AddLevelWall(const TextureAtlas& atlas, unsigned int xpos, unsigned int ypos, const TileInfo& tileInfo, const LevelWall& wall)
{
   /// subdivide all vertical coords (x or y coords) by this factor
   const unsigned int c_uiSubdivideVert = 4;

   /// subdivide all horizontal coords by this value
   const unsigned int c_uiSubdivideHoriz = 8;

   // assume all z coordinates to be divisable by 8
   ATLASSERT((wall.m_zposLo1 % 8) == 0);
   ATLASSERT((wall.m_zposHi1 % 8) == 0);

   ATLASSERT((wall.m_zposLo2 % 8) == 0);
   ATLASSERT((wall.m_zposHi2 % 8) == 0);

   // also assume all hi coordinates to be higher (or equal) as the lo coordinates
   ATLASSERT(wall.m_zposLo1 <= wall.m_zposHi1);
   ATLASSERT(wall.m_zposLo2 <= wall.m_zposHi2);

   // first, render non-slope parts; the missing triangles follow later
   unsigned int uiLowPos = std::max(wall.m_zposLo1, wall.m_zposLo2);
   unsigned int uiHighPos = std::min(wall.m_zposHi1, wall.m_zposHi2);

   ATLASSERT(uiHighPos >= uiLowPos);

   // calc normal
   Vector3d v1 = CalcPosFromWallPoint(xpos, ypos, uiLowPos, wall.m_enPoint1);
   Vector3d v2 = CalcPosFromWallPoint(xpos, ypos, uiHighPos, wall.m_enPoint1);
   Vector3d v3 = CalcPosFromWallPoint(xpos, ypos, uiLowPos, wall.m_enPoint2);

   Vector3d vNormal = (v2-v1).Cross(v3-v1);
   vNormal.Normalize();

   Color col = Color::White();

   TexCoord2f tcMin, tcMax;
   atlas.GetUV(tileInfo.m_uiTextureWall, tcMin, tcMax);

   std::vector<VertexBufferEntry>& vecVertices = m_vertexBuffer.Vertices();
   std::vector<unsigned int>& vecIndices = m_indexBuffer.IndexBuffer();

   // subdivide wall in vertical strips
   for (unsigned int horiz = uiLowPos; horiz < uiHighPos; horiz += c_uiSubdivideHoriz)
   {
      // aPoints, aTexCoords:
      // +-------------------------------+     horiz + c_uiSubdivideHoriz
      // |[1]          ^                 |[3]
      // |u1,v2        | c_uiSubdivide   |u2,v2
      // |             | Horiz           |
      // |             v                 |
      // +-------------------------------+     horiz
      //  [0]                             [2]
      //  u1,v1                           u2,v1
      // point 1                         point 2

      std::array<Vector3d, 4> aPoints;
      aPoints[0] = CalcPosFromWallPoint(xpos, ypos, horiz, wall.m_enPoint1);
      aPoints[1] = CalcPosFromWallPoint(xpos, ypos, horiz + c_uiSubdivideHoriz, wall.m_enPoint1);
      aPoints[2] = CalcPosFromWallPoint(xpos, ypos, horiz, wall.m_enPoint2);
      aPoints[3] = CalcPosFromWallPoint(xpos, ypos, horiz + c_uiSubdivideHoriz, wall.m_enPoint2);

      std::array<TexCoord2f, 4> aTexCoords;
      aTexCoords[0] = CalcTexCoordFromWallPoint(tcMin, tcMax, 1.0, horiz, tileInfo.m_uiCeiling);
      aTexCoords[1] = CalcTexCoordFromWallPoint(tcMin, tcMax, 1.0, horiz + c_uiSubdivideHoriz, tileInfo.m_uiCeiling);
      aTexCoords[2] = CalcTexCoordFromWallPoint(tcMin, tcMax, 0.0, horiz, tileInfo.m_uiCeiling);
      aTexCoords[3] = CalcTexCoordFromWallPoint(tcMin, tcMax, 0.0, horiz + c_uiSubdivideHoriz, tileInfo.m_uiCeiling);

      // further subdivide vertical strips horizontally, leading to squares
      for (unsigned int vert = 0; vert < c_uiSubdivideVert; vert++)
      {
         double d1 = double(vert) / c_uiSubdivideVert;
         double d2 = double(vert + 1) / c_uiSubdivideVert;

         // aPoints2, aTexCoords2:
         // +-------//---+---------+-----//------+
         // |[1]         |new[1]   |new[3]       |[3]
         // |            |         |             |
         // |            |         |             |
         // |            |         |             |
         // +-------//---+---------+-----//------+
         // [0]           new[0]    new[2]        [2]
         // 0.0          d1        d2             1.0

         std::array<Vector3d, 4> aPoints2;
         aPoints2[0] = Interpolate(aPoints[0], aPoints[2], d1);
         aPoints2[1] = Interpolate(aPoints[1], aPoints[3], d1);
         aPoints2[2] = Interpolate(aPoints[0], aPoints[2], d2);
         aPoints2[3] = Interpolate(aPoints[1], aPoints[3], d2);

         std::array<TexCoord2f, 4> aTexCoords2;
         aTexCoords2[0] = Interpolate(aTexCoords[0], aTexCoords[2], d1);
         aTexCoords2[1] = Interpolate(aTexCoords[1], aTexCoords[3], d1);
         aTexCoords2[2] = Interpolate(aTexCoords[0], aTexCoords[2], d2);
         aTexCoords2[3] = Interpolate(aTexCoords[1], aTexCoords[3], d2);

         size_t uiIndexBase = vecVertices.size();

         vecVertices.push_back(VertexBufferEntry(aPoints2[0], vNormal, aTexCoords2[0], col));
         vecVertices.push_back(VertexBufferEntry(aPoints2[1], vNormal, aTexCoords2[1], col));
         vecVertices.push_back(VertexBufferEntry(aPoints2[2], vNormal, aTexCoords2[2], col));
         vecVertices.push_back(VertexBufferEntry(aPoints2[3], vNormal, aTexCoords2[3], col));

         if (wall.m_bWindingCcw)
         {
            vecIndices.push_back(uiIndexBase + 0);
            vecIndices.push_back(uiIndexBase + 2);
            vecIndices.push_back(uiIndexBase + 3);

            vecIndices.push_back(uiIndexBase + 0);
            vecIndices.push_back(uiIndexBase + 3);
            vecIndices.push_back(uiIndexBase + 1);
         }
         else
         {
            vecIndices.push_back(uiIndexBase + 0);
            vecIndices.push_back(uiIndexBase + 3);
            vecIndices.push_back(uiIndexBase + 2);

            vecIndices.push_back(uiIndexBase + 0);
            vecIndices.push_back(uiIndexBase + 1);
            vecIndices.push_back(uiIndexBase + 3);
         }
      }
   }

   // TODO render slope parts
}
