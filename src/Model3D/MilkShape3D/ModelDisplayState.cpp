//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MilkShape3D\ModelDisplayState.cpp MilkShape3D model display state
//

// includes
#include "StdAfx.h"
#include "ModelDisplayState.hpp"
#include "CompositeModel3d.hpp"
#include "AnimatedModel3d.hpp"
#include "StaticModel3d.hpp"
#include "MilkShape3D/Data.hpp"
#include "RenderOptions.hpp"
#include "AABox.hpp"
#include "AnimationData.hpp"
#include "ModelManager.hpp"
#include "MovementInfo.hpp"
#include <boost/foreach.hpp>

using namespace MilkShape3D;

ModelDisplayState::ModelDisplayState(std::shared_ptr<CompositeModel3d> spModel)
:m_spModel(spModel),
 m_bNextAnimationAvail(false),
 m_vbo(0)
{
}

void ModelDisplayState::Prepare(ModelManager& modelManager, const ModelBlueprint& blueprint)
{
   PrepareAnimatedModelTextures(modelManager, blueprint);
   PrepareAnimatedModel();
   PrepareStaticModels(modelManager, blueprint);
}

void ModelDisplayState::PrepareAnimatedModelTextures(ModelManager& modelManager, const ModelBlueprint& blueprint)
{
   AnimatedModel3d& model = *m_spModel->GetAnimated();
   Data& data = model.GetData();

   // prepare textures for each material
   m_vecMaterialTextureData.resize(data.m_vecMaterials.size());
   ATLASSERT(blueprint.m_vecAnimatedMaterialTextures.size() == data.m_vecMaterials.size());

   for (size_t i=0, iMax=m_vecMaterialTextureData.size(); i<iMax; i++)
   {
      const Material& material = data.m_vecMaterials[i];

      CString cszTexture = material.cszTexture;
      cszTexture.Replace(_T(".\\"), _T(""));

      // use blueprint texture name, if set
      if (!blueprint.m_vecAnimatedMaterialTextures[i].IsEmpty())
         cszTexture = blueprint.m_vecAnimatedMaterialTextures[i];

      m_vecMaterialTextureData[i].m_spTexture = modelManager.LoadTexture(cszTexture);
   }
}

void ModelDisplayState::PrepareAnimatedModel()
{
   AnimatedModel3d& model = *m_spModel->GetAnimated();
   const Data& data = model.GetData();

   m_vecJointRenderData.resize(data.m_vecJoints.size());

   // reserve space in vertex buffer
   std::vector<VertexBufferEntry>& vecBuffer = m_vbo.Vertices();
   vecBuffer.resize(data.m_vecTriangles.size()*3);

   // store vertices of all triangles
   size_t uiTriangle = 0;
   BOOST_FOREACH(const Triangle& t, data.m_vecTriangles)
   {
      PrepareTriangle(model, data, t, &vecBuffer[uiTriangle*3], false);

      uiTriangle++;
   }
}

void ModelDisplayState::PrepareTriangle(const AnimatedModel3d& model, const Data& data, const Triangle& t,
                                        VertexBufferEntry* pEntries, bool bOnlyUpdate)
{
   for (unsigned int uiVertex = 0; uiVertex < 3; uiVertex++)
   {
      size_t uiVertexIndex = t.auiVertexIndices[uiVertex];
      ATLASSERT(uiVertexIndex < data.m_vecVertices.size());

      const Vertex& v = data.m_vecVertices[uiVertexIndex];

      Vector3d vPos = v.m_vPos;
      model.TransformVertex(v, m_vecJointRenderData, vPos);

      Vector3d vNormal = t.aNormals[uiVertex];
      model.TransformNormal(v, m_vecJointRenderData, vNormal);

      VertexBufferEntry& e = pEntries[uiVertex];

      e.vertex[0] = static_cast<float>(vPos.X());
      e.vertex[1] = static_cast<float>(vPos.Y());
      e.vertex[2] = static_cast<float>(vPos.Z());

      e.normal[0] = static_cast<float>(vNormal.X());
      e.normal[1] = static_cast<float>(vNormal.Y());
      e.normal[2] = static_cast<float>(vNormal.Z());

      if (!bOnlyUpdate)
      {
         e.texcoords[0] = t.aTex[uiVertex].U();
         e.texcoords[1] = t.aTex[uiVertex].V();

         e.color[0] = e.color[1] = e.color[2] = 255;
         e.color[3] = 0;
      }
   }
}

void ModelDisplayState::PrepareStaticModels(ModelManager& modelManager, const ModelBlueprint& blueprint)
{
   const std::vector<CompositeModel3d::StaticModelData>& vecStaticModels =
      m_spModel->StaticList();

   // blueprint must contain same number of statics as composite models
   ATLASSERT(blueprint.m_vecStaticBlueprints.size() == vecStaticModels.size());

   m_vecStaticModelTextures.resize(vecStaticModels.size());

   for (size_t i=0, iMax=vecStaticModels.size(); i<iMax; i++)
   {
      // static model must contain only one material; get texture name of it
      CString cszTexture = vecStaticModels[i].m_spStatic->SingleMaterialTextureName();
      cszTexture.Replace(_T(".\\"), _T(""));

      // add texture name prefix, if any
      const ModelBlueprint::StaticModelBlueprint& staticBlueprint =
         blueprint.m_vecStaticBlueprints[i];

      if (!staticBlueprint.m_cszStaticTexturePrefix.IsEmpty())
         cszTexture = staticBlueprint.m_cszStaticTexturePrefix + cszTexture;

      m_vecStaticModelTextures[i] = modelManager.LoadTexture(cszTexture);
   }
}

void ModelDisplayState::Upload()
{
   UploadAnimatedModel();
   UploadStaticModels();

   m_flagUploaded.Set();
}

#define USE_INPLACE_UPDATE

void ModelDisplayState::UpdateVertices()
{
   AnimatedModel3d& model = *m_spModel->GetAnimated();
   const Data& data = model.GetData();

#ifdef USE_INPLACE_UPDATE
   // update in-place
   m_vbo.Bind();
   VertexBufferEntry* vecBuffer = m_vbo.MapBuffer();
#else
   // reserve space in vertex buffer
   std::vector<VertexBufferEntry>& vecBuffer = m_vbo.Vertices();
   vecBuffer.resize(data.m_vecTriangles.size()*3);
#endif

   // update vertices of all triangles
   size_t uiTriangle = 0;
   BOOST_FOREACH(const Triangle& t, data.m_vecTriangles)
   {
      PrepareTriangle(model, data, t, &vecBuffer[uiTriangle*3], true);

      uiTriangle++;
   }

#ifdef USE_INPLACE_UPDATE
   m_vbo.UnmapBuffer();
#else
   m_vbo.Upload();
#endif
}

void ModelDisplayState::UploadAnimatedModel()
{
   m_vbo.Upload();

   AnimatedModel3d& model = *m_spModel->GetAnimated();
   model.IndexBuffer().Update();
}

void ModelDisplayState::UploadStaticModels()
{
   const std::vector<CompositeModel3d::StaticModelData>& vecStaticModels =
      m_spModel->StaticList();

   BOOST_FOREACH(const CompositeModel3d::StaticModelData& data, vecStaticModels)
      data.m_spStatic->Upload();
}

void ModelDisplayState::Tick(double dElapsed)
{
   if (!m_flagUploaded.IsSet())
      return;

   AnimatedModel3d& model = *m_spModel->GetAnimated();
   const Data& data = model.GetData();

   UpdateAnimationInfo(dElapsed, data.m_animationData.fAnimationFPS);

   // prepare joint matrices
   double dTime = m_animationInfo.m_dStartTime + m_animationInfo.m_dCurrent;

   // evaluate all joints, updating joint render data (the matrices)
   for (size_t i=0, iMax=data.m_vecJoints.size(); i<iMax; i++)
   {
      const Joint& joint = data.m_vecJoints[i];
      JointRenderData& renderData = m_vecJointRenderData[i];

      model.EvaluateJoint(joint, renderData, m_vecJointRenderData, dTime);
   }

   UpdateVertices();
}

void ModelDisplayState::UpdateAnimationInfo(double dElapsed, float fAnimationFPS)
{
   m_animationInfo.m_dCurrent += dElapsed * fAnimationFPS;

   // next available and this one is interruptable?
   if (m_bNextAnimationAvail && m_animationInfo.m_bInterruptable)
   {
      // use next animation
      m_bNextAnimationAvail = false;
      m_animationInfo = m_nextAnimationInfo;
   }

   double dDuration = m_animationInfo.m_dEndTime - m_animationInfo.m_dStartTime;

   if (m_animationInfo.m_dCurrent > dDuration)
   {
      // animation ended
      if (!m_animationInfo.m_bLoop)
         return; // no loop, do nothing

      if (m_bNextAnimationAvail)
      {
         // use next animation
         m_bNextAnimationAvail = false;
         m_animationInfo = m_nextAnimationInfo;
      }
      else
      {
         // just loop this one
         m_animationInfo.m_dCurrent = fmod(m_animationInfo.m_dCurrent, dDuration);
      }
   }
}

void ModelDisplayState::SetAnimation(T_enModelAnimation enAnimation,
                                     T_enModelAnimationWeaponType enAnimWeaponType, bool bLoop, bool bInterruptable)
{
   unsigned int uiStartFrame = 0, uiEndFrame = 1;

   WarriorsAndCommoners::AnimationData::GetAnimationFrames(enAnimation, enAnimWeaponType,
      uiStartFrame, uiEndFrame);

   SetAnimationFrames(uiStartFrame, uiEndFrame, bLoop, bInterruptable);
}

void ModelDisplayState::SetAnimationFrames(unsigned int uiStartFrame, unsigned int uiEndFrame, bool bLoop, bool bInterruptable)
{
   AnimationInfo animationInfo;
   animationInfo.m_dCurrent = 0.0;
   animationInfo.m_dStartTime = uiStartFrame;
   animationInfo.m_dEndTime = uiEndFrame;
   animationInfo.m_bLoop = bLoop;
   animationInfo.m_bInterruptable = bInterruptable;

   // set as next animation when current one isn't interruptable
   if (m_animationInfo.m_bInterruptable)
      m_animationInfo = animationInfo;
   else
      m_nextAnimationInfo = animationInfo;
}

void ModelDisplayState::UpdateMovement(const MovementInfo& info)
{
   T_enModelAnimation enAnimation = animIdle1;
   T_enModelAnimationWeaponType enAnimWeaponType = animWeaponHand; // TODO set appropriately

   // TODO check "combat" flag, e.g. for animIdleCombat, animRunCombat, etc.
   // TODO implement variations of animations, e.g. animIdle1, animIdle2, animIdle3; etc.

   bool bLoop = true;
   bool bInterruptable = true;

   switch (info.MovementType())
   {
   case MovementInfo::moveTypeStand: enAnimation = animIdle1; break;
   case MovementInfo::moveTypeWalkForward: enAnimation = animRunning; break;
   case MovementInfo::moveTypeWalkBackward: enAnimation = animMoveBackwards; break;
   case MovementInfo::moveTypeJump: enAnimation = animJumping; bInterruptable = false; break;
   case MovementInfo::moveTypeFall: enAnimation = animFalling; break;
   case MovementInfo::moveTypeSwim: enAnimation = animSwimming; break;
   case MovementInfo::moveTypeDie: enAnimation = animDeath1; bLoop = false; break;
   }

   SetAnimation(enAnimation, enAnimWeaponType, bLoop, bInterruptable);
}

void ModelDisplayState::Render(RenderOptions& options)
{
   if (options.Get(RenderOptions::optionModelJoints))
      OpenGL::RenderXyzAxes();

   if (!m_flagUploaded.IsSet())
      return;

   //RenderAnimatedModelImmediateMode(options);
   RenderAnimatedModelVertexBuffer(options);

   RenderStaticModels(options);

   // render joint lines and points
   if (options.Get(RenderOptions::optionModelJoints))
      RenderJoints();
}

void ModelDisplayState::RenderAnimatedModelImmediateMode(RenderOptions& options) const throw()
{
   AnimatedModel3d& model = *m_spModel->GetAnimated();
   const Data& data = model.GetData();

   AABox boundingBox;

   // now render all groups, using vertex and normals from joint render data
   BOOST_FOREACH(const Group& group, data.m_vecGroups)
   {
      BindMaterial(group);

      glBegin(GL_TRIANGLES);

      BOOST_FOREACH(size_t uiTriangleIndex, group.m_vecTriangleIndices)
      {
         ATLASSERT(uiTriangleIndex < data.m_vecTriangles.size());

         const Triangle& t = data.m_vecTriangles[uiTriangleIndex];
         for (unsigned int v=0; v<3; v++)
         {
            glTexCoord2fv(t.aTex[v].Data());

            size_t vertexIndex = t.auiVertexIndices[v];
            const Vertex& vert = data.m_vecVertices[vertexIndex];

            Vector3d vNormal = t.aNormals[v];
            model.TransformNormal(vert, m_vecJointRenderData, vNormal);

            glNormal3dv(vNormal.Data());

            Vector3d vVertex = vert.m_vPos;
            model.TransformVertex(vert, m_vecJointRenderData, vVertex);
            glVertex3dv(vVertex.Data());

            if (options.Get(RenderOptions::optionModelBoundingBox))
               boundingBox.UpdateBound(vVertex);
         }
      }

      glEnd();

      OpenGL::CountPolygons(group.m_vecTriangleIndices.size());

      if (options.Get(RenderOptions::optionModelNormals))
         RenderModelNormals(group);
   }

   if (options.Get(RenderOptions::optionModelBoundingBox))
      OpenGL::RenderBoundingBox(boundingBox.Min(), boundingBox.Max());
}

void ModelDisplayState::RenderAnimatedModelVertexBuffer(RenderOptions& options) const throw()
{
   AnimatedModel3d& model = *m_spModel->GetAnimated();
   const Data& data = model.GetData();

   m_vbo.Bind();
   model.IndexBuffer().Bind();

   // render all groups
   for (size_t i=0, iMax=data.m_vecGroups.size(); i<iMax; i++)
   {
      const Group& group = data.m_vecGroups[i];

      BindMaterial(group);

      const GroupRenderData& renderData = model.GroupRenderDataVector()[i];

      model.IndexBuffer().RenderRange(renderData.m_range);

      OpenGL::CountPolygons(renderData.m_range.m_uiSize / 3);

      if (options.Get(RenderOptions::optionModelNormals))
         RenderModelNormals(group);
   }

   model.IndexBuffer().Unbind();
   m_vbo.Unbind();
}

void ModelDisplayState::BindMaterial(const Group& group) const throw()
{
   AnimatedModel3d& model = *m_spModel->GetAnimated();
   const Data& data = model.GetData();

   size_t uiMaterial = group.m_uiMaterialIndex;
   ATLASSERT(uiMaterial < data.m_vecMaterials.size());

   data.m_vecMaterials[uiMaterial].Use();

   // use texture; stored in material texture data
   ATLASSERT(uiMaterial < m_vecMaterialTextureData.size());
   const MaterialTextureData& texData = m_vecMaterialTextureData[uiMaterial];

   if (texData.m_spTexture != NULL)
      texData.m_spTexture->Bind();
}

void ModelDisplayState::RenderStaticModels(RenderOptions& options) const throw()
{
   const std::vector<CompositeModel3d::StaticModelData>& vecStaticModels =
      m_spModel->StaticList();

   ATLASSERT(m_vecStaticModelTextures.size() == vecStaticModels.size());

   for (size_t i=0, iMax = vecStaticModels.size(); i<iMax; i++)
   {
      // enable model texture, if loaded
      if (m_vecStaticModelTextures[i] != NULL)
         m_vecStaticModelTextures[i]->Bind();

      // find mount point and matrix
      const CompositeModel3d::StaticModelData& data = vecStaticModels[i];

      ATLASSERT(data.m_iJointIndex >= 0);
      ATLASSERT(size_t(data.m_iJointIndex) < m_vecJointRenderData.size());

      size_t uiMountIndex = static_cast<size_t>(data.m_iJointIndex);

      Matrix4d matGlobal = m_vecJointRenderData[uiMountIndex].GlobalMatrix();
      matGlobal.Transpose();

      // transform current modelview matrix
      glPushMatrix();

      glMultMatrixd(matGlobal.Data());

      // render model
      data.m_spStatic->Render(options);

      // render mount point
      if (options.Get(RenderOptions::optionModelJoints))
         RenderMountPoint();

      glPopMatrix();
   }
}

void ModelDisplayState::RenderModelNormals(const Group& group) const throw()
{
   AnimatedModel3d& model = *m_spModel->GetAnimated();
   const Data& data = model.GetData();

   OpenGL::PushedAttributes attrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);

   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_TEXTURE_2D);

   glLineWidth(2.0f);

   glBegin(GL_LINES);

   BOOST_FOREACH(size_t uiTriangleIndex, group.m_vecTriangleIndices)
   {
      ATLASSERT(uiTriangleIndex < data.m_vecTriangles.size());

      const Triangle& t = data.m_vecTriangles[uiTriangleIndex];
      for (unsigned int v=0; v<3; v++)
      {
         size_t vertexIndex = t.auiVertexIndices[v];
         const Vertex& vert = data.m_vecVertices[vertexIndex];

         Vector3d vVertex = vert.m_vPos;
         model.TransformVertex(vert, m_vecJointRenderData, vVertex);

         Vector3d vNormal = t.aNormals[v];
         model.TransformNormal(vert, m_vecJointRenderData, vNormal);

         vNormal.Normalize();
         vNormal *= 0.1;
         vNormal += vVertex;

         glVertex3dv(vVertex.Data());
         glVertex3dv(vNormal.Data());
      }
   }

   glEnd();
}

void ModelDisplayState::RenderJoints() const throw()
{
   OpenGL::PushedAttributes attrib(GL_POINT_BIT | GL_LINE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);

   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_TEXTURE_2D);

   glColor3f(0.0f, 0.0f, 1.0f);
   glLineWidth(2.0f);
   RenderJointLines();

   glColor3f(1.0f, 1.0f, 0.0f);
   glPointSize(3.0f);
   RenderJointPoints();
}

void ModelDisplayState::RenderJointLines() const throw()
{
   AnimatedModel3d& model = *m_spModel->GetAnimated();
   const Data& data = model.GetData();

   ATLASSERT(data.m_vecJoints.size() == m_vecJointRenderData.size());

   glBegin(GL_LINES);
   for (size_t i=0, iMax = m_vecJointRenderData.size(); i<iMax; i++)
   {
      const Joint& joint = data.m_vecJoints[i];
      const JointRenderData& renderData = m_vecJointRenderData[i];

      const Matrix4d& matGlobal = renderData.GlobalMatrix();

      if (joint.ParentIndex() == -1)
      {
         glVertex3dv(matGlobal.Row(3).Data());
         glVertex3dv(matGlobal.Row(3).Data());
      }
      else
      {
         size_t uiParentIndex = static_cast<size_t>(joint.ParentIndex());
         ATLASSERT(uiParentIndex < m_vecJointRenderData.size());

         const Matrix4d& matParent = m_vecJointRenderData[uiParentIndex].GlobalMatrix();

         glVertex3dv(matGlobal.Row(3).Data());
         glVertex3dv(matParent.Row(3).Data());
      }
   }
   glEnd();
}

void ModelDisplayState::RenderJointPoints() const throw()
{
   glBegin(GL_POINTS);
   for (size_t i=0, iMax = m_vecJointRenderData.size(); i<iMax; i++)
   {
      const JointRenderData& renderData = m_vecJointRenderData[i];

      const Matrix4d& matGlobal = renderData.GlobalMatrix();

      glVertex3dv(matGlobal.Row(3).Data());
   }
   glEnd();
}

void ModelDisplayState::RenderMountPoint() const throw()
{
   OpenGL::PushedAttributes attrib(GL_POINT_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);

   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_TEXTURE_2D);

   glColor3f(1.0f, 0.0f, 1.0f);
   glPointSize(10.0f);

   glBegin(GL_POINTS);
   glVertex3d(0.0, 0.0, 0.0);
   glEnd();

   OpenGL::RenderXyzAxes();
}
