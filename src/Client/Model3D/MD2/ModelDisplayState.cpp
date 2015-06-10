//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MD2\ModelDisplayState.cpp MD2 model display state
//

// includes
#include "stdafx.h"
#include "ModelDisplayState.hpp"
#include "ModelData.hpp"
#include "Normals.hpp"
#include "RenderOptions.hpp"
#include "Texture.hpp"
#include "MovementInfo.hpp"

using namespace MD2;

namespace
{
   /// animation list
   enum AnimType
   {
      STAND,
      RUN,
      ATTACK,
      PAIN_A,
      PAIN_B,
      PAIN_C,
      JUMP,
      FLIP,
      SALUTE,
      FALLBACK,
      WAVE,
      POINT,
      CROUCH_STAND,
      CROUCH_WALK,
      CROUCH_ATTACK,
      CROUCH_PAIN,
      CROUCH_DEATH,
      DEATH_FALLBACK,
      DEATH_FALLFORWARD,
      DEATH_FALLBACKSLOW,
      BOOM,

      special_STAND,

      MAX_ANIMATIONS
   };

   /// animation data
   struct Animation
   {
      int     first_frame;            ///< first frame of the animation
      int     last_frame;             ///< number of frames
      int     fps;                    ///< number of frames per second
   };

   // ----------------------------------------------
   // initialize the 21 MD2 model animations.
   // ----------------------------------------------

   /// list of all animations
   const Animation c_animlist[21 + 1] =
   {
      // first, last, fps

      { 0, 39, 9 },   //  0 STAND
      { 40, 45, 10 },   //  1 RUN
      { 46, 53, 10 },   //  2 ATTACK
      { 54, 57, 7 },   //  3 PAIN_A
      { 58, 61, 7 },   //  4 PAIN_B
      { 62, 65, 7 },   //  5 PAIN_C
      { 66, 71, 7 },   //  6 JUMP
      { 72, 83, 7 },   //  7 FLIP
      { 84, 94, 7 },   //  8 SALUTE
      { 95, 111, 10 },   //  9 FALLBACK
      { 112, 122, 7 },   // 10 WAVE
      { 123, 134, 6 },   // 11 POINT
      { 135, 153, 10 },   // 12 CROUCH_STAND
      { 154, 159, 7 },   // 13 CROUCH_WALK
      { 160, 168, 10 },   // 14 CROUCH_ATTACK
      { 169, 172, 7 },   // 15 CROUCH_PAIN
      { 173, 177, 5 },   // 16 CROUCH_DEATH
      { 178, 183, 7 },   // 17 DEATH_FALLBACK
      { 184, 189, 7 },   // 18 DEATH_FALLFORWARD
      { 190, 197, 7 },   // 19 DEATH_FALLBACKSLOW
      { 198, 198, 5 },   // 20 BOOM

      // special; used for a model just standing still
      { 0, 19, 20 },   // 21 STAND
   };

} // unnamed namespace

/// number of steps per frame, for renderDisplayListOnDemandPrepare
const unsigned int c_uiNumStepsPerFrame = 5;

ModelDisplayState::ModelDisplayState(std::shared_ptr<ModelData> spModelData, TexturePtr spTexture)
:m_spModelData(spModelData),
m_spTexture(spTexture),
m_enRenderMode(renderDisplayListOnDemandPrepare),
m_uiCurrentAnimation(STAND),
m_uiNextAnimation(STAND),
m_uiCurrentFrame(0),
m_uiNextFrame(0),
m_uiStartFrame(0),
m_uiEndFrame(1),
m_uiFramesPerSecond(10),
m_dPercentFrameDone(0.0)
{
}

void ModelDisplayState::Prepare() throw()
{
   if (m_enRenderMode == renderDisplayList)
   {
      // prepare display lists
      unsigned int uiListSize = m_spModelData->m_iNumFrames * c_uiNumStepsPerFrame;

      OpenGL::DisplayListArray& dla = m_spModelData->m_frameDisplayLists;
      dla.Init(uiListSize);

      for (unsigned int uiFrame = 0, uiNumFrames = m_spModelData->m_iNumFrames; uiFrame < uiNumFrames; uiFrame++)
      {
         for (unsigned int uiStep = 0; uiStep < c_uiNumStepsPerFrame; uiStep++)
         {
            PrepareFrameStep(dla, false, uiFrame, uiStep);
         }
      }
   }
   else
      if (m_enRenderMode == renderDisplayListOnDemandPrepare)
      {
         // allocate display lists
         unsigned int uiListSize = m_spModelData->m_iNumFrames * c_uiNumStepsPerFrame;

         m_spModelData->m_frameDisplayLists.Init(uiListSize);

         m_spModelData->m_deqDisplayListPrepared.resize(uiListSize, false);
      }

   m_timerAnimation.Start();
}

void ModelDisplayState::Tick()
{
   if (!m_timerAnimation.IsStarted())
      return;

   double dElapsed = m_timerAnimation.Elapsed();

   if (m_uiEndFrame == m_uiStartFrame)
   {
      // static model
      m_uiCurrentFrame = m_uiStartFrame;
      return;
   }

   // see at how many percent we are
   m_dPercentFrameDone += (dElapsed / m_uiFramesPerSecond) * 90.0;
   if (m_dPercentFrameDone >= 1.0)
   {
      unsigned int uiFrames = (unsigned int)m_dPercentFrameDone;
      m_uiCurrentFrame += uiFrames;

      ATLASSERT(m_uiEndFrame >= m_uiStartFrame);

      if (m_uiCurrentFrame > m_uiEndFrame)
         m_uiCurrentFrame = m_uiStartFrame + ((m_uiCurrentFrame - m_uiStartFrame) % (m_uiEndFrame - m_uiStartFrame));

      m_uiNextFrame = m_uiCurrentFrame + 1;
      if (m_uiNextFrame >= m_uiEndFrame)
         OnNextAnimation();

      m_dPercentFrameDone -= (unsigned int)m_dPercentFrameDone;
   }
}

void ModelDisplayState::SetAnimation(T_enModelAnimation enAnimation, T_enModelAnimationWeaponType /*enAnimWeaponType*/,
   bool /*bLoop*/, bool /*bInterruptable*/)
{
   // map model anim type to md2 model animations
   int type = MapModelAnimtypeToMd2Anim(enAnimation);

   SetAnimation(type);
}

void ModelDisplayState::SetAnimationFrames(unsigned int uiStartFrame, unsigned int uiEndFrame,
   bool bLoop, bool /*bInterruptable*/)
{
   m_uiCurrentFrame =
      m_uiStartFrame = uiStartFrame;
   m_uiEndFrame = uiEndFrame;
   m_uiNextFrame = m_uiStartFrame;
   m_uiFramesPerSecond = c_animlist[STAND].fps;

   // set current animation so that animation loops or stops in OnNextAnimation()
   m_uiCurrentAnimation = bLoop ? RUN : DEATH_FALLBACK;
}

void ModelDisplayState::UpdateMovement(const MovementInfo& info)
{
   switch (info.MovementType())
   {
   case MovementInfo::moveTypeStand: SetAnimation(AnimType::STAND); break;
   case MovementInfo::moveTypeWalkForward: SetAnimation(AnimType::RUN); break;
   case MovementInfo::moveTypeWalkBackward: SetAnimation(AnimType::CROUCH_WALK); break;
   case MovementInfo::moveTypeJump: SetAnimation(AnimType::JUMP); break;
   case MovementInfo::moveTypeFall: SetAnimation(AnimType::FALLBACK); break;
   case MovementInfo::moveTypeSwim: SetAnimation(AnimType::RUN); break;
   case MovementInfo::moveTypeDie: SetAnimation(AnimType::DEATH_FALLBACK); break;
   default:
      ATLASSERT(false);
   }
}

void ModelDisplayState::Render(RenderOptions& options)
{
   glPushMatrix();

   glTranslatef(0.0f, 1.4f, 0.0f);
   glScalef(0.1f, 0.1f, 0.1f);

   // rotate the model
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   glRotatef(-90.0, 0.0, 0.0, 1.0);

   // render it on the screen
   RenderFrame(options);

   glPopMatrix();
}

void ModelDisplayState::RenderFrame(RenderOptions& options) const throw()
{
   // set front face to clockwise
   glPushAttrib(GL_POLYGON_BIT);
   glFrontFace(GL_CW);

   // enable backface culling
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   ATLASSERT(m_uiCurrentFrame < static_cast<unsigned int>(m_spModelData->m_iNumFrames));

   // render bounding box
   if (options.Get(RenderOptions::optionModelBoundingBox))
      RenderBoundingBox(m_uiCurrentFrame);

   m_spTexture->Bind();

   if (m_enRenderMode == renderDirect)
   {
      unsigned int uiPolycount = 0;
      RenderFramePercent(m_uiCurrentFrame, m_uiNextFrame, m_dPercentFrameDone, uiPolycount);

      OpenGL::CountPolygons(uiPolycount);
   }
   else
      if (m_enRenderMode == renderDisplayList ||
         m_enRenderMode == renderDisplayListOnDemandPrepare)
      {
      unsigned int uiStep = static_cast<unsigned int>(m_dPercentFrameDone * c_uiNumStepsPerFrame);
      if (uiStep >= c_uiNumStepsPerFrame)
         uiStep = c_uiNumStepsPerFrame - 1;

      unsigned int uiListIndex = m_uiCurrentFrame*c_uiNumStepsPerFrame + uiStep;

      if (m_enRenderMode == renderDisplayListOnDemandPrepare &&
         !m_spModelData->m_deqDisplayListPrepared[uiListIndex])
      {
         // prepare item
         const_cast<ModelDisplayState&>(*this).
            PrepareFrameStep(m_spModelData->m_frameDisplayLists, true, m_uiCurrentFrame, uiStep);

         m_spModelData->m_deqDisplayListPrepared[uiListIndex] = true;
      }
      else
      {
         // just call appropriate display list
         m_spModelData->m_frameDisplayLists.Call(uiListIndex);
      }

      OpenGL::CountPolygons(m_spModelData->m_uiDisplayListPolycount);
      }
      else
         ATLASSERT(false);

   glDisable(GL_TEXTURE_2D);

   glDisable(GL_CULL_FACE);
   glPopAttrib();
}

void ModelDisplayState::PrepareFrameStep(OpenGL::DisplayListArray& dla, bool bCompileAndRender,
   unsigned int uiFrame, unsigned int uiStep) throw()
{
   dla.Open(uiFrame*c_uiNumStepsPerFrame + uiStep, bCompileAndRender);

   unsigned int uiNextFrame = uiFrame + 1;
   if (uiNextFrame >= static_cast<unsigned int>(m_spModelData->m_iNumFrames))
      uiNextFrame = 0;

   unsigned int uiPolycount = 0;
   RenderFramePercent(uiFrame, uiNextFrame, double(uiStep) / c_uiNumStepsPerFrame, uiPolycount);
   dla.Close();

   if (m_spModelData->m_uiDisplayListPolycount == 0)
      m_spModelData->m_uiDisplayListPolycount = uiPolycount;
}

void ModelDisplayState::RenderFramePercent(unsigned int uiNumFrame, unsigned int uiNextFrame,
   double dPercent, unsigned int& uiPolycount) const throw()
{
   const std::vector<int>& vecGlcmds = m_spModelData->m_vecGlcmds;

   for (size_t i = 0, iMax = vecGlcmds.size(); i<iMax && vecGlcmds[i] != 0; i++)
   {
      int iCmd = vecGlcmds[i];
      if (iCmd < 0)
      {
         glBegin(GL_TRIANGLE_FAN);
         iCmd = -iCmd;
      }
      else
      {
         glBegin(GL_TRIANGLE_STRIP);
      }

      // count polygons:
      // a fan or strip initially needs 3 points for a triangle, and one point for each next triangle
      if (iCmd > 6)
         uiPolycount += iCmd / 3 - 2;

      ATLASSERT(i + iCmd * 3 < iMax);

      // parse all OpenGL commands of this group
      for (int j = 0; j<iCmd; j++, i += 3)
      {
         // glcmds[0] : final S texture coord.
         // glcmds[1] : final T texture coord.
         // glcmds[2] : vertex index to draw
         GLCommand* pGLcmd = reinterpret_cast<GLCommand*>((void*)&vecGlcmds[i + 1]);
         int index = pGLcmd->index;

         ATLASSERT(index < m_spModelData->m_iNumXyz);
         Vector3d& v1 = m_spModelData->m_vecVertices[m_spModelData->m_iNumXyz * uiNumFrame + index];
         Vector3d& v2 = m_spModelData->m_vecVertices[m_spModelData->m_iNumXyz * uiNextFrame + index];
         int iNormalIndex = m_spModelData->m_vecLightNormals[index];

         // send texture coords. to OpenGL
         glTexCoord2f(pGLcmd->s, pGLcmd->t);

         // send normal vector to OpenGL
         ATLASSERT(iNormalIndex < sizeof(s_anorms) / sizeof(*s_anorms));
         glNormal3fv(s_anorms[iNormalIndex]);

         Vector3d vDist = v2 - v1;
         vDist *= dPercent;

         Vector3d v = v1 + vDist;

         glVertex3dv(v.Data());
      }

      glEnd();
   }
}

void MD2::ModelDisplayState::RenderBoundingBox(unsigned int uiCurrentFrame) const throw()
{
   const Vector3d& vMin = m_spModelData->m_vecMinBounds[uiCurrentFrame];
   const Vector3d& vMax = m_spModelData->m_vecMaxBounds[uiCurrentFrame];

   glColor3ub(255, 255, 255);

   OpenGL::RenderBoundingBox(vMin, vMax);
}

void ModelDisplayState::SetAnimation(int type)
{
   // note: we don't set current frame, as we just want the model to change into
   // next frame
   m_uiCurrentFrame =
      m_uiStartFrame = c_animlist[type].first_frame;
   m_uiEndFrame = c_animlist[type].last_frame;
   m_uiNextFrame = c_animlist[type].first_frame + 1;
   m_uiFramesPerSecond = c_animlist[type].fps;
   m_uiCurrentAnimation = type;

   m_timerAnimation.Restart();

   // start and end may be equal: then it's not animated
   ATLASSERT(m_uiEndFrame >= m_uiStartFrame);
}

void MD2::ModelDisplayState::OnNextAnimation()
{
   bool m_bRepeatAnimation = false;
   bool m_bStopAnimation = false;

   switch (m_uiCurrentAnimation)
   {
   case STAND:
   case RUN:
   case CROUCH_WALK:
      m_bRepeatAnimation = true;
      break;

   case DEATH_FALLBACK:
   case DEATH_FALLFORWARD:
   case DEATH_FALLBACKSLOW:
      m_bStopAnimation = true;
      break;

   default:
      m_uiNextAnimation = STAND;
      break;
   }

   // decide what to do after animation is complete
   if (m_bRepeatAnimation)
   {
      // repeat
      m_uiNextFrame = m_uiStartFrame;
   }
   else
      if (m_bStopAnimation)
      {
         // stop
         m_uiStartFrame = m_uiEndFrame = m_uiCurrentFrame;
      }
      else
      {
         // new model animation
         SetAnimation(m_uiNextAnimation);
      }

   m_timerAnimation.Restart();
}


int MD2::ModelDisplayState::MapModelAnimtypeToMd2Anim(T_enModelAnimation enType)
{
   int type = 0;
   switch (enType)
   {
   case animIdle1:         type = AnimType::STAND; break;
   case animIdle2:         type = AnimType::STAND; break;
   case animIdle3:         type = AnimType::STAND; break;

   case animIdleCombat:    type = AnimType::STAND; break;
   case animRunCombat:     type = AnimType::RUN; break;
   case animAttack1:       type = AnimType::ATTACK; break;
   case animAttack2:       type = AnimType::ATTACK; break;
   case animHit:           type = AnimType::PAIN_A; break;

   case animRunning:          type = AnimType::RUN; break;
   case animWalking:          type = AnimType::CROUCH_WALK; break;
   case animMoveBackwards:    type = AnimType::RUN; break;
   case animMoveSide:         type = AnimType::RUN; break;
   case animJumping:          type = AnimType::JUMP; break;
   case animFalling:          type = AnimType::FALLBACK; break;
   case animSwimming:         type = AnimType::RUN; break;

   case animUnlockDoor:       type = AnimType::POINT; break;
   case animUnlockChest:      type = AnimType::POINT; break;
   case animPickUp:           type = AnimType::POINT; break;
   case animDrinkPotion:      type = AnimType::POINT; break;

   case animSleep:            type = AnimType::STAND; break;

   case animDeath1:           type = AnimType::DEATH_FALLBACK; break;
   case animDeath2:           type = AnimType::DEATH_FALLFORWARD; break;
   case animDeath3:           type = AnimType::DEATH_FALLBACKSLOW; break;

   case animEmoteGreet:       type = AnimType::SALUTE; break;
   case animEmoteLaugh:       type = AnimType::STAND; break;
   case animEmoteYes:         type = AnimType::STAND; break;
   case animEmoteNo:          type = AnimType::STAND; break;
   case animEmoteApplaud:     type = AnimType::WAVE; break;
   default:
      ATLASSERT(false);
   };

   if ((type < 0) || (type >= AnimType::MAX_ANIMATIONS))
      type = 0;

   return type;
}
