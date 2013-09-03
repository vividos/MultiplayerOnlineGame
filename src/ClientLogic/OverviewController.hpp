//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file OverviewController.hpp Overview controller
//
#pragma once

// includes
#include "ClientLogic.hpp"
#include "Vector3d.hpp"
#include "KeyboardActionBindings.hpp"
#include "IViewController.hpp"
#include "TimedEffect.hpp"
#include "KeyboardActionBindings.hpp"

// forward references
class KeyboardActionManager;
class ISceneManager;
class PerspectiveCamera;

/// \brief overview controller
/// \details shows an overview of the scene where the user can freely move
/// with mouse and cursor keys. The player can still be moved with WASD keys,
/// only the camera movement from ThirdPersonViewController is ignored.
class CLIENTLOGIC_DECLSPEC OverviewController:
   public IViewController,
   public TimedEffect<OverviewController>
{
public:
   /// ctor
   OverviewController(KeyboardActionManager& keyboardActionManager,
      std::shared_ptr<PerspectiveCamera> spCamera);

   /// sets new view position
   void SetPosition(const Vector3d& vPosition, double dAngleDirection, double dAngleUp)
   {
      m_vPosition = vPosition;
      m_dAngleDirection = dAngleDirection;
      m_dAngleUp = dAngleUp;
   }

   /// sets new movement speed
   void SetMovementSpeed(double dSpeedInUnitsPerSecond)
   {
      m_dMovementSpeed = dSpeedInUnitsPerSecond;
   }

   // virtual methods from IViewController

   /// tick processing
   virtual void Tick() override;

   /// called on mouse button event
   virtual void OnMouseButtonEvent(bool bPressed, T_enMouseButtonType enButtonType, int x, int y) override;

   /// called on mouse motion event
   virtual void OnMouseMotionEvent(int x, int y, int xrel, int yrel) override;

private:
   friend TimedEffect<OverviewController>;

   /// registers key actions
   void RegisterKeys(KeyboardActionManager& keyboardActionManager);

   /// called to calculate new position on tick
   void OnTick(double dElapsed);

   /// sets forward and backward direction vector based on key press
   void SetForwardBackward(bool bPressed, Vector3d vDir);

   /// sets left and right direction vector based on key press
   void SetLeftRight(bool bPressed, Vector3d vDir);

private:
   /// camera
   std::shared_ptr<PerspectiveCamera> m_spCamera;

   /// direction angle
   double m_dAngleDirection;

   /// up angle
   double m_dAngleUp;

   /// movement speed
   double m_dMovementSpeed;

   /// indicates if currently dragging mouse
   bool m_bDragView;

   /// forward/backward movement vector
   Vector3d m_vForwardBackward;

   /// left/right movement vector
   Vector3d m_vLeftRight;

   /// current position
   Vector3d m_vPosition;

   /// key action bindings
   KeyboardActionBindings m_actionBindings;
};
