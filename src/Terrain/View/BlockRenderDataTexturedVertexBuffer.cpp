//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BlockRenderDataTexturedVertexBuffer.cpp Render data for rendering terrain using vertex buffer
//

// includes
#include "StdAfx.h"
#include "BlockRenderDataTexturedVertexBuffer.hpp"
#include "Model/DataBlock.hpp"
#include "OpenGL.hpp"
#include "Bitmap.hpp"
#include "TextureParameter.hpp"
#include "ColorGradient.hpp"
#include "Reduce/ZoneManager.hpp"
#include "ZigzagSquareIterator.hpp"
#include "Reduce/ZoneDataSource.hpp"
#include "GraphicsTaskManager.hpp"

using Terrain::Model::DataBlock;
using Terrain::BlockRenderDataTexturedVertexBuffer;

BlockRenderDataTexturedVertexBuffer::BlockRenderDataTexturedVertexBuffer(
   GraphicsTaskManager& taskManager) throw()
:m_taskManager(taskManager),
 m_vertData(0),
 m_bFirstUpdate(true)
{
}

void BlockRenderDataTexturedVertexBuffer::Prepare(std::shared_ptr<DataBlock> spDataBlock)
{
   m_spDataBlock = spDataBlock;

   unsigned int uiBlockSize = m_spDataBlock->GetElevationData().Size();

   m_spZoneManager.reset(new Reduce::ZoneManager(m_taskManager,
      m_spDataBlock->GetElevationData().ArrayMapper(), m_indexBuffer, uiBlockSize));

   PrepareVertexArray(*spDataBlock);
}

void BlockRenderDataTexturedVertexBuffer::PrepareVertexArray(DataBlock& dataBlock)
{
   ATLASSERT(OpenGL::BufferObjectBase::IsAvailable() == true);

   Model::ElevationData& elevationData = dataBlock.GetElevationData();

   std::vector<VertexBufferEntry>& vecVertices = m_vertData.Vertices();
   vecVertices.resize(elevationData.RawData().size());

   unsigned int uiBlockSize = elevationData.Size()+1;

   for (size_t i=0, iMax=vecVertices.size(); i<iMax; i++)
   {
      VertexBufferEntry& entry = vecVertices[i];

      unsigned int x, y;
      elevationData.ArrayMapper().IndexToCoord(i, x, y);

      entry.vertex[0] = static_cast<GLfloat>(x);
      entry.vertex[1] = static_cast<GLfloat>(elevationData.Height(x, y));
      entry.vertex[2] = static_cast<GLfloat>(y);

      // TODO
      Vector3d vNormal(0.0, 1.0, 0.0);

      entry.normal[0] = static_cast<GLfloat>(vNormal.X());
      entry.normal[1] = static_cast<GLfloat>(vNormal.Y());
      entry.normal[2] = static_cast<GLfloat>(vNormal.Z());

      entry.texcoords[0] = float(x) / uiBlockSize;
      entry.texcoords[1] = float(y) / uiBlockSize;

      entry.color[0] = entry.color[1] = entry.color[2] = 255;
      entry.color[3] = 0;
   }

   // upload with next frame
   m_taskManager.UploadTaskGroup().Add(
      std::bind(&BlockRenderDataTexturedVertexBuffer::UploadVertexArray, this));
}

void BlockRenderDataTexturedVertexBuffer::UploadVertexArray()
{
   m_vertData.Upload();
   m_vertexBufferUploaded.Set();
}

void BlockRenderDataTexturedVertexBuffer::Update(const Vector3d& vCamera)
{
   if (!m_bFirstUpdate && (vCamera-m_vLastCamera).Length() < 1.0)
      return;

   m_bFirstUpdate = false;
   m_vLastCamera = vCamera;

   if (m_blockUpdateInProgress.IsSet())
      return;

   // schedule new update
   m_taskManager.BackgroundTaskGroup().Add(
      std::bind(&BlockRenderDataTexturedVertexBuffer::UpdateBlocks, this, vCamera));
}

void BlockRenderDataTexturedVertexBuffer::UpdateBlocks(const Vector3d& vCamera)
{
   m_blockUpdateInProgress.Set();

   unsigned int uiNumZones = m_spZoneManager->NumZones();

   unsigned int uiZoneSize = m_spDataBlock->GetElevationData().Size() / uiNumZones;

   Terrain::Reduce::ZoneDataSource ds(*m_spDataBlock, uiZoneSize);

#define USE_ZIGZAG

#ifdef USE_ZIGZAG
   ZigzagSquareIterator iter(uiNumZones);
#else
   for (size_t i=0, iMax=uiNumZones*uiNumZones; i<iMax; i++)
#endif
   do
   {
      // check which new level should be calculated for each block
#ifdef USE_ZIGZAG
//      unsigned int xzone = iter.X(), yzone = iter.Y();
      unsigned int xzone = uiNumZones - 1 - iter.X();
      unsigned int yzone = uiNumZones - 1 - iter.Y();
#else
      unsigned int xzone, yzone;
      IndexToZone(i, xzone, yzone);
#endif

      double xmid = (xzone * uiZoneSize) + uiZoneSize/2;
      double ymid = (yzone * uiZoneSize) + uiZoneSize/2;

      double dist = (vCamera - Vector3d(xmid, vCamera.Y(), ymid)).Length();

      // map distance to level
      unsigned int uiLevel = MapDistToLevel(dist);

      unsigned int uiIndex = xzone * uiNumZones + yzone;
      ATLASSERT(xzone == uiIndex / uiNumZones);
      ATLASSERT(yzone == uiIndex % uiNumZones);

      // check if zone needs update
      m_spZoneManager->IsCurrentLevel(uiIndex, uiLevel);

      std::shared_ptr<DataBlock> spZoneDataBlock =
         ds.LoadBlock(xzone * uiZoneSize, yzone * uiZoneSize, uiZoneSize);

      m_spZoneManager->PrepareZone(xzone, yzone, uiIndex, uiLevel, spZoneDataBlock);

#ifdef USE_ZIGZAG
   } while (iter.Next());
#else
   }
#endif

   m_blockUpdateInProgress.Reset();

//   m_taskManager.BackgroundTaskGroup().Add(
//      std::bind(&BlockRenderDataTexturedVertexBuffer::UploadTexture, this));
}

/*
void BlockRenderDataTexturedVertexBuffer::UploadTexture()
{
   m_tex.Generate();
   m_tex.Bind();

   Bitmap bmp(512, 512);

   FillTextureBitmap(bmp);

   m_tex.Upload(bmp, true); // mip-map
}
*/

void BlockRenderDataTexturedVertexBuffer::Render(const RenderOptions& renderOptions,
                                                 const ViewFrustum3d& viewFrustum)
{
   // TODO use GraphicsTaskManager to upload
   if (m_tex.Size() == 0)
   {
      m_tex.Generate();
      m_tex.Bind();

      Bitmap bmp(512, 512);

      FillTextureBitmap(bmp);

      m_tex.Upload(bmp, true); // mip-map
   }

   if (!m_vertexBufferUploaded.IsSet())
      return; // not uploaded yet

   m_vertData.Bind();

   m_indexBuffer.Bind();

   m_tex.Bind();

   typedef TextureParameter<GL_CLAMP,GL_CLAMP,GL_NEAREST,GL_NEAREST_MIPMAP_NEAREST,GL_MODULATE> TexParam;

   TexParam::Use();

   // draw all zones
   m_spZoneManager->Render(renderOptions, viewFrustum);

   glDisable(GL_TEXTURE_2D);

   m_indexBuffer.Unbind();

   m_vertData.Unbind();
}

unsigned int BlockRenderDataTexturedVertexBuffer::MapDistToLevel(double dist) throw()
{
   unsigned int uiLevel = 0;
   if (dist > 128.0 && dist < 256.0)
      uiLevel = 1;
   else
   if (dist >= 256.0)
      uiLevel = 2;

   return uiLevel;
}

void BlockRenderDataTexturedVertexBuffer::FillTextureBitmap(Bitmap& bmp)
{
   ColorGradient cg;
   cg.Add(Color::Blue());
   cg.Add(Color::Green());
   cg.Add(Color(0,160,0));
   cg.Add(Color(64,130,64));
   cg.Add(Color::White());
   cg.Add(Color::Red());

   for (unsigned int x=0; x<bmp.XRes(); x++)
   for (unsigned int y=0; y<bmp.YRes(); y++)
   {
      // checkerboard pattern
      Color c = (x+y) & 1 ? Color::Red() : Color::Blue();

/*
      // based on max. slope angle
      double dHeight = m_spDataBlock->GetElevationData().GetHeight(x, y);
      double dAngle = 0.0;

      if (x > 0 && y > 0)
      {
         double dAdjacentX = m_spDataBlock->GetElevationData().GetHeight(x-1, y);
         double dAdjacentY = m_spDataBlock->GetElevationData().GetHeight(x, y-1);
         double dAdjacentXY = m_spDataBlock->GetElevationData().GetHeight(x-1, y-1);

         double dAngleX = Rad2Deg(std::atan(std::abs(dHeight-dAdjacentX)));
         double dAngleY = Rad2Deg(std::atan(std::abs(dHeight-dAdjacentY)));
         double dAngleXY = Rad2Deg(std::atan(std::abs(dHeight-dAdjacentXY)));

         dAngle = std::max(dAngle, dAngleX);
         dAngle = std::max(dAngle, dAngleY);
         dAngle = std::max(dAngle, dAngleXY);
      }

      Color c = cg.Get(dAngle / 90.0);
*/

/*
      // based on steepness (max. height diff on adjacent points)
      double dHeight = m_spDataBlock->GetElevationData().GetHeight(x, y);
      double dMaxDiff = 0.0;

      for (unsigned int i=0; i<4; i++)
      {
         double dAdjacent = dHeight;
         switch (i)
         {
         case 0:
            if (x > 0)
               dAdjacent = m_spDataBlock->GetElevationData().GetHeight(x-1, y);
            break;
         case 1:
            if (y > 0)
               dAdjacent = m_spDataBlock->GetElevationData().GetHeight(x, y-1);
            break;
         case 2:
            if (x < bmp.XRes()-1)
               dAdjacent = m_spDataBlock->GetElevationData().GetHeight(x+1, y);
            break;
         case 3:
            if (y < bmp.YRes()-1)
               dAdjacent = m_spDataBlock->GetElevationData().GetHeight(x, y+1);
            break;
         }

         double dDiff = std::abs(dHeight-dAdjacent);
         dMaxDiff = std::max(dDiff, dMaxDiff);
      }

      Color c = cg.Get(dMaxDiff);
*/

/*
      // based on height
      double dHeight = m_spDataBlock->GetElevationData().GetHeight(x, y);
      Color c = cg.Get(dHeight / 256.0);
//      Color c = Interpolate<Color>(Color::Red(), Color::Blue(), dHeight / 256.0);
*/

      bmp.Set(x, y, c);
   }
}
