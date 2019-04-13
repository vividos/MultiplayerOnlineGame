//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MD2\ModelDisplayState.hpp MD2 model display state
//
#pragma once

// includes
#include "IModelDisplayState.hpp"
#include "Texture.hpp"
#include <ulib/Timer.hpp>

// forward references
namespace OpenGL
{
   class DisplayListArray;
}

namespace MD2
{
struct ModelData;

/// render mode
enum T_enRenderMode
{
   renderDirect,        ///< renders model in direct mode
   renderDisplayList,   ///< renders model using prepared display lists
   renderDisplayListOnDemandPrepare, ///< same as renderDisplayList, but with on-demand prepare
   renderVertexBuffer,  ///< renders model using vertex buffer objects
};

/// MD2 model display state
class ModelDisplayState : public IModelDisplayState
{
public:
   /// ctor
   ModelDisplayState(std::shared_ptr<ModelData> spModelData, TexturePtr spTexture);
   /// dtor
   virtual ~ModelDisplayState() {}

   /// prepares display state
   void Prepare();

   // virtual methods from IModelDisplayState

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
   /// renders a frame of the model
   void RenderFrame(RenderOptions& options) const;

   /// prepares an "on-demand" frame step
   void PrepareFrameStep(OpenGL::DisplayListArray& dla, bool bCompileAndRender,
      unsigned int uiFrame, unsigned int uiStep);

   /// directly renders frame by percent elapsed between current and next frame
   void RenderFramePercent(unsigned int uiNumFrame, unsigned int uiNextFrame,
      double dPercent, unsigned int& uiPolycount) const;

   /// renders bounding box
   void RenderBoundingBox(unsigned int uiCurrentFrame) const;

   /// sets new animation
   void SetAnimation(int type);

   /// called when animation is finished
   void OnNextAnimation();

   /// maps model animation to AnimType
   static int MapModelAnimtypeToMd2Anim(T_enModelAnimation enType);

private:
   /// model data
   std::shared_ptr<ModelData> m_spModelData;

   /// model texture
   TexturePtr m_spTexture;

   /// render mode
   T_enRenderMode m_enRenderMode;

   /// timer for animation
   Timer m_timerAnimation;

   unsigned int m_uiCurrentAnimation;  ///< current animation
   unsigned int m_uiNextAnimation;     ///< number of next animation
   unsigned int m_uiCurrentFrame;      ///< current animation frame
   unsigned int m_uiNextFrame;         ///< next animation frame
   unsigned int m_uiStartFrame;        ///< start animation frame
   unsigned int m_uiEndFrame;          ///< end animation frame
   unsigned int m_uiFramesPerSecond;   ///< fps of current animation
   double m_dPercentFrameDone;         ///< percent of current animation already done; [0.0; 1.0[
};

} // namespace MD2
