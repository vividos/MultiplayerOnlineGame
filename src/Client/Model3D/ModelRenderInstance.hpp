//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ModelRenderInstance.hpp Instance for model rendering
//
#pragma once

// includes
#include "Model3DCommon.hpp"
#include "Vector3.hpp"
#include <ulib/Timer.hpp>

// forward references
class IModelDisplayState;
class RenderOptions;
class MovementInfo;

/// \brief model render instance
/// \details holds all information to render a distinct model instance.
/// Supports "position correction".
class MODEL3D_DECLSPEC ModelRenderInstance
{
public:
   /// ctor
   ModelRenderInstance(std::shared_ptr<IModelDisplayState> spDisplayState) throw();

   // get methods

   /// returns display state
   std::shared_ptr<IModelDisplayState> DisplayState() throw() { return m_spDisplayState; }

   // set methods

   /// sets new model position
   void SetPosition(const Vector3d& vPos, double dViewAngle);

   /// updates movement info
   void UpdateMovementInfo(const MovementInfo& movementInfo);

   /// sets transparency
   void SetTransparency(double dTransparency);

   /// sets selection
   void SetSelected(bool bSelected) throw() { m_bSelected = bSelected; }

   /// sets scale
   void SetScale(const Vector3d& vScale = Vector3d(1.0, 1.0, 1.0));

   /// starts fadein
   void StartFadein();

   /// starts fadeout
   void StartFadeout();

   /// renders model
   void Render(RenderOptions& renderOptions);

private:
   /// returns calculated pos, including position correction
   Vector3d CalculatedPos();

   /// returns calculated player transparency
   double CalcPlayerTransparency() throw();

private:
   enum T_enFadeMode
   {
      fadeModeNone = 0,
      fadeModeFadein = 1,
      fadeModeFadeout = 2,
      fadeModeInvisible = 3
   };

   /// model instance
   std::shared_ptr<IModelDisplayState> m_spDisplayState;

   /// indicates if model is selected
   bool m_bSelected;

   /// model position
   Vector3d m_vPos;

   /// view angle
   double m_dViewAngle;

   /// scale of model
   Vector3d m_vecScale;

   /// model transparency
   double m_dTransparency;

   /// current fade mode
   T_enFadeMode m_enFadeMode;

   /// current fade state, in range [0; 1]
   double m_dFadeState;

   /// timer for facein/out
   Timer m_timerFade;

   /// indicates if position is not inited yet
   bool m_bFirstPosInit;

   /// indicates if position needs to be corrected
   bool m_bCorrectPos;

   /// timer for position correction
   Timer m_timerCorrect;

   /// start pos for position correction
   Vector3d m_vCorrectStart;

   /// target pos for position correction
   Vector3d m_vCorrectTarget;
};
