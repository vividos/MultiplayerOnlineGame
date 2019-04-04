//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\StaticModel3d.cpp MilkShape3D static model
//

// includes
#include "StdAfx.h"
#include "StaticModel3d.hpp"
#include "RenderOptions.hpp"
#include <boost/foreach.hpp>

using namespace MilkShape3D;

void StaticModel3d::Prepare() throw()
{
   SetupJoints();
   PrepareVertices();
   PrepareIndices();
}

void StaticModel3d::PrepareVertices()
{
   // reserve space in vertex buffer
   std::vector<VertexBufferEntry>& vecBuffer = m_vbo.Vertices();
   vecBuffer.resize(m_data.m_vecTriangles.size()*3);

   // store triangles
   size_t uiTriangle = 0;
   for (const Triangle& t : m_data.m_vecTriangles)
   {
      for (unsigned int uiVertex = 0; uiVertex < 3; uiVertex++)
      {
         size_t uiVertexIndex = t.auiVertexIndices[uiVertex];
         ATLASSERT(uiVertexIndex < m_data.m_vecVertices.size());

         const Vertex& v = m_data.m_vecVertices[uiVertexIndex];

         Vector3d vPos = v.m_vPos;

         if (v.m_iJointIndex != -1)
         {
            const Joint& joint = m_data.m_vecJoints[v.m_iJointIndex];

            const Matrix4d& matGlobalSkeleton = joint.GlobalSkeletonMatrix();

            vPos = matGlobalSkeleton * vPos;
         }

         m_boundingBox.UpdateBound(vPos);

         VertexBufferEntry& e = vecBuffer[uiTriangle*3 + uiVertex];

         e.texcoords[0] = t.aTex[uiVertex].U();
         e.texcoords[1] = t.aTex[uiVertex].V();

         e.vertex[0] = static_cast<float>(vPos.X());
         e.vertex[1] = static_cast<float>(vPos.Y());
         e.vertex[2] = static_cast<float>(vPos.Z());

         e.normal[0] = static_cast<float>(t.aNormals[uiVertex].X());
         e.normal[1] = static_cast<float>(t.aNormals[uiVertex].Y());
         e.normal[2] = static_cast<float>(t.aNormals[uiVertex].Z());

         e.color[0] = e.color[1] = e.color[2] = 255;
         e.color[3] = 0;
      }

      uiTriangle++;
   }
}

void StaticModel3d::PrepareIndices()
{
   // count all groups
   size_t uiNumTriangles = 0;
   for (const Group& g : m_data.m_vecGroups)
      uiNumTriangles += g.m_vecTriangleIndices.size();

   // reserve space in index buffer
   std::vector<unsigned int> vecVertexIndices;
   vecVertexIndices.resize(uiNumTriangles*3);

   // store triangles
   size_t uiTriangle = 0;
   for (const Group& group : m_data.m_vecGroups)
   {
      for (size_t uiTriangleIndex : group.m_vecTriangleIndices)
      {
         unsigned int uiVertexIndex = uiTriangleIndex*3;
         ATLASSERT(uiVertexIndex < m_vbo.Vertices().size());

         for (unsigned int uiVertex = 0; uiVertex < 3; uiVertex++)
            vecVertexIndices[uiTriangle*3 + uiVertex] = uiVertexIndex + uiVertex;

         uiTriangle++;
      }
   }

   m_indexRange = m_ibo.Add(vecVertexIndices);
}

void StaticModel3d::Upload() throw()
{
   if (m_bAlreadyUploaded)
      return;

   m_bAlreadyUploaded = true;

   ATLASSERT(OpenGL::IsRenderThread() == true);

   m_vbo.Upload();
   m_ibo.Update();
}

CString StaticModel3d::SingleMaterialTextureName() const
{
   ATLASSERT(!m_data.m_vecMaterials.empty()); // must not be empty
   if (m_data.m_vecMaterials.empty())
      return CString();

   return m_data.m_vecMaterials[0].cszTexture;
}

void StaticModel3d::Render(RenderOptions& options) throw()
{
   m_vbo.Bind();
   m_ibo.Bind();

   m_ibo.RenderRange(m_indexRange);

   OpenGL::CountPolygons(m_indexRange.m_uiSize / 3);

   m_ibo.Unbind();
   m_vbo.Unbind();

   if (options.Get(RenderOptions::optionModelBoundingBox))
      OpenGL::RenderBoundingBox(m_boundingBox.Min(), m_boundingBox.Max());
}
