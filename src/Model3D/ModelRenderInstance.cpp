//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ModelRenderInstance.cpp Instance for model rendering
//

// includes
#include "StdAfx.h"
#include "ModelRenderInstance.hpp"
#include "IModelDisplayState.hpp"
#include "RenderOptions.hpp"
#include "MovementInfo.hpp"
#include "OpenGL.hpp"

/// time in seconds to correct a "too large" new position
const double c_dCorrectTimeRange = 0.5;

/// fade in/out time
const double c_dFadeTime = 4.5;

ModelRenderInstance::ModelRenderInstance(std::shared_ptr<IModelDisplayState> spDisplayState) throw()
:m_spDisplayState(spDisplayState),
 m_bSelected(false),
 m_dViewAngle(0.0),
 m_dTransparency(1.0),
 m_enFadeMode(fadeModeNone),
 m_dFadeState(0.0),
 m_bFirstPosInit(true),
 m_bCorrectPos(false)
{
   m_timerDisplayState.Start();
}

void ModelRenderInstance::SetPosition(const Vector3d& vPos, double dViewAngle)
{
   m_dViewAngle = dViewAngle;

   if (m_bFirstPosInit)
   {
      m_vPos = vPos;
      m_bFirstPosInit = false;
      return;
   }

   // check if new distance is "too far" and we should "correct"
   if (!m_bCorrectPos)
   {
      Vector3d vDist = m_vPos - vPos;
      double dMaxDist = 0.5;
      bool bTooFar = vDist.Length() > dMaxDist;

      if (bTooFar)
      {
         // start correcting
         m_bCorrectPos = true;
         m_timerCorrect.Start();
 
         m_vCorrectStart = m_vPos;
         m_vCorrectTarget = vPos;
      }
      else
      {
         // just use new coordinate
         m_vPos = vPos;
      }
   }
   else
   {
      // already correcting

      bool bFinishedCorrecting = m_timerCorrect.Elapsed() >= c_dCorrectTimeRange;

      if (!bFinishedCorrecting)
      {
         // still too far
         m_vCorrectTarget = vPos;
      }
      else
      {
         // correction finished
         m_timerCorrect.Stop();
         m_bCorrectPos = false;

         m_vPos = vPos;
      }
   }
}

void ModelRenderInstance::UpdateMovementInfo(const MovementInfo& movementInfo)
{
   m_spDisplayState->UpdateMovement(movementInfo);

   SetPosition(movementInfo.Position(), movementInfo.ViewAngle());
}

void ModelRenderInstance::SetTransparency(double dTransparency)
{
   m_dTransparency = dTransparency;
}

void ModelRenderInstance::StartFadein()
{
   m_enFadeMode = fadeModeFadein;
   m_dFadeState = 0.0;
   m_timerFade.Start();
}

void ModelRenderInstance::StartFadeout()
{
   m_enFadeMode = fadeModeFadeout;
   m_dFadeState = 0.0;
   m_timerFade.Start();
}

Vector3d ModelRenderInstance::CalculatedPos()
{
   if (!m_bCorrectPos)
      return m_vPos;

   // interpolate to correct position
   Vector3d vDist = m_vCorrectTarget - m_vCorrectStart;

   double dPercentDone = m_timerCorrect.Elapsed() / c_dCorrectTimeRange;
   if (dPercentDone > 1.0)
      dPercentDone = 1.0;

   return m_vPos + dPercentDone * vDist;
}

double ModelRenderInstance::CalcPlayerTransparency() throw()
{
   double dTransparency = m_dTransparency;

   if (m_enFadeMode != fadeModeNone)
   {
      if (m_enFadeMode == fadeModeInvisible)
         dTransparency = 0.0;
      else
      {
         double dFadeMin = m_enFadeMode == fadeModeFadein ? 0.0 : 1.0;

         double dElapsed = m_timerFade.Elapsed();
         if (dElapsed < c_dFadeTime)
            dTransparency *= LinearTransform<double>(0.0, c_dFadeTime, dFadeMin, 1.0 - dFadeMin)(dElapsed);
         else
         {
            m_timerFade.Stop();

            dTransparency *= 1.0 - dFadeMin;
            m_enFadeMode = m_enFadeMode == fadeModeFadeout ? fadeModeInvisible : fadeModeNone;
         }
      }

      ATLTRACE(_T("using player transparency %1.3f\n"), dTransparency);
   }

   // prevent rounding errors
   if (dTransparency > 0.99)
      dTransparency = 1.0;

   return dTransparency;
}

void ModelRenderInstance::Render(RenderOptions& renderOptions)
{
   m_spDisplayState->Tick(m_timerDisplayState.Elapsed());
   m_timerDisplayState.Restart();

   OpenGL::PushedAttributes attrib(GL_POLYGON_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);

   if (renderOptions.Get(RenderOptions::optionModelFilled))
      glPolygonMode(GL_FRONT, GL_FILL);
   else
      glPolygonMode(GL_FRONT, GL_LINE);

   glPushMatrix();

   // position model
   Vector3d vPos = CalculatedPos();
   glTranslated(vPos.X(), vPos.Y(), vPos.Z());

   glRotated(m_dViewAngle, 0.0, 1.0, 0.0);

   double dTransparency = CalcPlayerTransparency();

   // blend model
   if (dTransparency < 1.0)
   {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   }

   double dLuminance = m_bSelected ? 1.2 : 1.0;
   glColor4d(dLuminance, dLuminance, dLuminance, dTransparency);

   m_spDisplayState->Render(renderOptions);

   glPopMatrix();
}
