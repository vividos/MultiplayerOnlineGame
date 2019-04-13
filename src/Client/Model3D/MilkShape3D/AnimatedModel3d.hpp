//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\AnimatedModel3d.hpp MilkShape3D animated model
//
#pragma once

// includes
#include "Model3dBase.hpp"
#include "RangedAppendOnlyIndexBuffer.hpp"
#include "Vector3.hpp"

// forward references
class GraphicsTaskManager;
class RenderOptions;
class Quaternion4d;

namespace MilkShape3D
{
struct JointRenderData;

/// render data for one group
struct GroupRenderData
{
public:
   /// index ranges into index buffer, for one group
   IndexRange m_range;
};

/// \brief animated MilkShape3D model
/// \details Model with animation, implemented by rotating and moving joints
/// and so animating the triangles attached to each joint. Since an animated
/// model data can be used for multiple instances, the base data is stored
/// here. Since the index buffer won't be changed during animation, it is also
/// stored here. The vertex buffer and all other data used for an instance is
/// stored in class MilkShape3D::ModelDisplayState.
class AnimatedModel3d: public Model3dBase
{
public:
   /// ctor
   AnimatedModel3d() {}
   /// dtor
   virtual ~AnimatedModel3d() {}

   /// find mount joint and return index
   int FindMountJointIndex(LPCTSTR pszName) const;

   /// prepares model for rendering
   virtual void Prepare() override;

   /// uploads data to graphics card
   virtual void Upload() override;

private:
   friend class ModelManager;
   friend class ModelDisplayState;

   /// calculates tangents to joint endpoints
   void SetupTangents();

   /// prepares index buffer
   void PrepareIndexBuffer();

   /// returns index buffer; const version
   const RangedAppendOnlyIndexBuffer& IndexBuffer() const { return m_ibo; }
   /// returns index buffer
         RangedAppendOnlyIndexBuffer& IndexBuffer() { return m_ibo; }

   /// returns group render data
   const std::vector<GroupRenderData>& GroupRenderDataVector() { return m_vecGroupRenderData; }

   /// evaluates one joint and calculates joint render data for given frame
   void EvaluateJoint(const Joint& joint, JointRenderData& renderData,
      const std::vector<JointRenderData>& vecRenderData, double dFrame) const;

   /// evaluates position from joint matrices
   void EvaluatePosition(const Joint& joint, double dFrame, Vector3d& vPos) const;

   /// evaluates rotation from joint matrices
   void EvaluateRotation(const Joint& joint, double dFrame, Quaternion4d& quat) const;

   /// transforms a single vertex for given joint render data
   void TransformVertex(const Vertex& vertex,
      const std::vector<JointRenderData>& vecRenderData,
      Vector3d& vVertex) const;

   /// transforms a normal vector for given joint render data
   void TransformNormal(const Vertex& vertex,
      const std::vector<JointRenderData>& vecRenderData, Vector3d& vNormal) const;

private:
   /// group render data
   std::vector<GroupRenderData> m_vecGroupRenderData;

   /// index buffer
   RangedAppendOnlyIndexBuffer m_ibo;
};

} // namespace MilkShape3D
