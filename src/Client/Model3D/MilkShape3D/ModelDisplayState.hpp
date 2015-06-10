//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\ModelDisplayState.hpp MilkShape3D model display state
//
#pragma once

// includes
#include "IModelDisplayState.hpp"
#include "Matrix4d.hpp"
#include "Texture.hpp"
#include "TexturedVertexBuffer.hpp"
#include "SafeFlag.hpp"
#include "Timer.hpp"
#include <vector>

// forward references
class RenderOptions;

namespace MilkShape3D
{
class AnimatedModel3d;
class CompositeModel3d;
struct Data;
struct Triangle;
struct Group;
class Joint;
class ModelManager;
struct ModelBlueprint;

/// render data for one joint
struct JointRenderData
{
public:
   const Matrix4d& LocalMatrix() const throw() { return m_matLocal; } ///< returns current local matrix; const version
         Matrix4d& LocalMatrix()       throw() { return m_matLocal; } ///< returns current local matrix
   const Matrix4d& GlobalMatrix() const throw() { return m_matGlobal; } ///< returns current global matrix; const version
         Matrix4d& GlobalMatrix()       throw() { return m_matGlobal; } ///< returns current global matrix

private:
   /// local matrix; modified by ElevateJoints()
   Matrix4d m_matLocal;

   /// global matrix; modified by ElevateJoints()
   Matrix4d m_matGlobal;
};

/// texture data for one material entry
struct MaterialTextureData
{
   /// texture
   TexturePtr m_spTexture;
};

/// info for a single animation
struct AnimationInfo
{
   /// ctor
   AnimationInfo()
      :m_dCurrent(0.0),
       m_dStartTime(0.0),
       m_dEndTime(1.0),
       m_bLoop(false),
       m_bInterruptable(true)
   {
   }

   /// current frame time, started at 0.0
   double m_dCurrent;

   /// start frame time of current animation
   double m_dStartTime;

   /// end frame time of current animation
   double m_dEndTime;

   /// indicates if animation should be looped or be stopped at last frame
   bool m_bLoop;

   /// indicates if animation can be interrupted
   bool m_bInterruptable;
};

/// model display state for composite model
class ModelDisplayState: public IModelDisplayState
{
public:
   /// ctor
   ModelDisplayState(std::shared_ptr<CompositeModel3d> spModel);
   /// dtor
   virtual ~ModelDisplayState() throw() {}

   /// prepares display state
   void Prepare(ModelManager& modelManager, const ModelBlueprint& blueprint);

   /// uploads display state
   void Upload();

   /// tick calculation
   virtual void Tick() override;

   /// sets new animation
   virtual void SetAnimation(T_enModelAnimation enAnimation, T_enModelAnimationWeaponType enAnimWeaponType,
      bool bLoop, bool bInterruptable) override;

   /// sets new animation frames
   virtual void SetAnimationFrames(unsigned int uiStartFrame, unsigned int uiEndFrame,
      bool bLoop, bool bInterruptable) override;

   /// update model movement
   virtual void UpdateMovement(const MovementInfo& info) override;

   /// renders model instance
   virtual void Render(RenderOptions& options) override;

private:
   /// prepare animated model textures
   void PrepareAnimatedModelTextures(ModelManager& modelManager, const ModelBlueprint& blueprint);

   /// prepare animated model data
   void PrepareAnimatedModel();

   /// prepare static models data
   void PrepareStaticModels(ModelManager& modelManager, const ModelBlueprint& blueprint);

   /// prepares a triangle and stores 3 vertices in buffer entry
   void PrepareTriangle(const AnimatedModel3d& model, const Data& data, const Triangle& t, VertexBufferEntry* entries, bool bOnlyUpdate);

   /// updates vertices in VBO
   void UpdateVertices();

   /// upload animated models
   void UploadAnimatedModel();

   /// upload static models
   void UploadStaticModels();

   /// updates animation info
   void UpdateAnimationInfo(float fAnimationFPS);

   /// evaluates a joint for rendering
   void EvaluateJoint(Joint& joint, JointRenderData& renderData, double dFrame);

   /// renders animated model, using immediate mode
   void RenderAnimatedModelImmediateMode(RenderOptions& options) const throw();

   /// renders animated model, using vertex buffer
   void RenderAnimatedModelVertexBuffer(RenderOptions& options) const throw();

   /// binds material for group
   void BindMaterial(const Group& group) const throw();

   /// renders all static models
   void RenderStaticModels(RenderOptions& options) const throw();

   /// renders model normals
   void RenderModelNormals(const Group& group) const throw();

   /// renders joints
   void RenderJoints() const throw();

   /// renders joint lines
   void RenderJointLines() const throw();

   /// renders joint points
   void RenderJointPoints() const throw();

   /// renders mount point for static object
   void RenderMountPoint() const throw();

private:
   /// timer for display state
   Timer m_timerDisplayState;

   /// model data
   std::shared_ptr<CompositeModel3d> m_spModel;

   /// flag that indicates if everything is already uploaded
   SafeFlag m_flagUploaded;

   /// current animation info
   AnimationInfo m_animationInfo;

   /// next animation info (only used when current is not interruptible)
   AnimationInfo m_nextAnimationInfo;

   /// indicates if next animation info is available
   bool m_bNextAnimationAvail;

   // animated model stuff

   /// render data for all joints
   std::vector<JointRenderData> m_vecJointRenderData;

   /// material texture data for animated model; can be different for each model instance
   std::vector<MaterialTextureData> m_vecMaterialTextureData;

   /// textures for static models
   std::vector<TexturePtr> m_vecStaticModelTextures;

   /// vertices transformed by local matrix
   std::vector<Vector3d> m_vecVertices;

   /// normals transformed by local matrix
   std::vector<Vector3d> m_vecNormals;

   /// vertex buffer
   DynamicUpdateTexturedVertexBuffer m_vbo;
};

} // namespace MilkShape3D
