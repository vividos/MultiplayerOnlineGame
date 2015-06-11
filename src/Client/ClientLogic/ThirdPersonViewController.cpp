//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ThirdPersonViewController.cpp Third person view controller
//

// includes
#include "StdAfx.h"
#include "ThirdPersonViewController.hpp"
#include "KeyboardActionManager.hpp"
#include "Vector3.hpp"
#include "MovementInfo.hpp"
#include "SizePointRect.hpp"
#include "KeyboardActionBindings.hpp"
#include "IPlayerViewModel.hpp"
#include "PerspectiveCamera.hpp"
#include "ISceneManager.hpp"
#include <SDL.h>

/// view angle rotate speed using keyboard, in degrees/second
const double c_dAngleRotateSpeed = 180.0;

/// angle speed when rotating player angle to view angle, in degrees / second
const double c_dRotateToViewDegreePerSecond = 135.0;

/// max. scroll back distance
const double c_dMaxDistance = 20.0;

/// distance change per mouse wheel tick
const double c_dDistanceDeltaPerMousewheelTick = 1.0;//0.50;

namespace
{
   /// controls if plus or minus side of a key is pressed
   class PlusMinusControl
   {
   public:
      /// ctor
      PlusMinusControl()
         :m_bPlus(false),
          m_bMinus(false),
          m_bPlusPriority(false)
      {
      }

      /// called when plus key is pressed
      void OnPlusKey(bool bKeyDown) throw()
      {
         m_bPlus = bKeyDown;
         m_bPlusPriority = bKeyDown;
      }

      /// called when minus key is pressed
      void OnMinusKey(bool bKeyDown) throw()
      {
         m_bMinus = bKeyDown;
         m_bPlusPriority = !bKeyDown;
      }

      /// determines if a key is down
      bool IsKeyDown() const throw()
      {
         return m_bPlus || m_bMinus;
      }

      /// determines if plus side is currently active
      bool IsPlus() const throw()
      {
         return m_bPlusPriority ? m_bPlus : !m_bMinus;
      }

      /// resets sides
      void Reset() throw()
      {
         m_bPlus = m_bMinus = false;
      }

   private:
      bool m_bPlus;  ///< indicates if plus is active
      bool m_bMinus; ///< indicates if negative is active
      bool m_bPlusPriority;   ///< indicates if plus has priority
   };

   class CameraControl;

   /// position control for third person view controller
   class PositionControl
   {
      friend ThirdPersonViewController;

   public:
      /// ctor
      PositionControl(CameraControl& cameraControl, IPlayerViewModel& playerViewModel);

      // get methods

      /// returns if player angle is controllable
      bool IsPlayerAngleControllable() const throw();

      /// returns current player movement view angle
      double GetPlayerAngle() const throw() { return m_dMovementAngle; }

      // set methods

      /// sets or resets mouse look mode
      void SetMouseLookMode(bool bMouseLookMode) throw();

      /// sets new player movement view angle
      void SetPlayerAngle(double dAngle) throw() { m_dMovementAngle = dAngle; }

      // action methods

      /// tick processing
      void OnTick(double dElapsed);

   private:
      void OnKeyMoveForward(bool bKeyDown) throw();   ///< called on move forward key press
      void OnKeyMoveBackward(bool bKeyDown) throw();  ///< called on move backward key press
      void OnKeyRotateLeft(bool bKeyDown) throw();    ///< called on rotate left key press
      void OnKeyRotateRight(bool bKeyDown) throw();   ///< called on rotate right key press
      void OnKeyAutoMove(bool bKeyDown) throw();      ///< called on auto-move key press

      /// returns if currently running
      bool IsRunning() const throw();

   private:
      /// camera control
      CameraControl& m_cameraControl;

      /// player access in the view model
      IPlayerViewModel& m_playerViewModel;

      /// current movement view angle
      double m_dMovementAngle;

      PlusMinusControl m_moveForwardControl; ///< forward control keys
      PlusMinusControl m_moveLeftControl;    ///< strafe control keys
      PlusMinusControl m_rotateLeftControl;  ///< rotate control keys

      /// auto-move forward active
      bool m_bAutoMove;

      /// mouse look mode is active?
      bool m_bMouseLookMode;

      /// movement info
      MovementInfo m_movementInfo;
   };

   /// camera control for third person view controller
   class CameraControl
   {
      friend ThirdPersonViewController;

   public:
      /// ctor
      CameraControl(PositionControl& positionControl,
         ThirdPersonViewController::T_fnSetCameraPosition fnSetCameraPosition,
         IPlayerViewModel& playerViewModel,
         ISceneManager& sceneManager);

      // set methods

      /// sets running flag
      void SetRunningFlag(bool bIsRunning) throw();

      /// sets camera view angle
      void SetViewAngle(double dViewAngle) throw();

      // action methods

      /// tick processing
      void OnTick(double dElapsed);

      /// update camera pos
      void UpdateCameraPos();

      /// called when mouse button event occurs
      void OnMouseButtonEvent(bool bPressed, T_enMouseButtonType enButtonType);

      /// called when mouse motion event occurs
      void OnMouseMotionEvent(int xrel, int yrel);

      /// called when mouse wheel event occurs
      void OnMouseWheelEvent(int xrel, int yrel);

   private:
      /// moves camera distance
      void MoveCameraDistance(int iRelChange);

      /// moves camera angle by relative values
      void MoveCameraAngle(int iRelChangeX, int iRelChangeY);

      /// moves player angle when left-dragging
      void MovePlayerAngle();

      /// adjusts view angle while walking
      void AdjustViewAngleWalking(double dElapsed) throw();

      /// calculcates new viewpoint position
      void CalcPosition() throw();

      /// calculates player transparency by camera distance
      double PlayerTransparency() const throw();

   private:
      /// position control
      PositionControl& m_positionControl;

      /// function to set camera position
      ThirdPersonViewController::T_fnSetCameraPosition m_fnSetCameraPosition;

      /// player view model access
      IPlayerViewModel& m_playerViewModel;

      /// scene manager
      ISceneManager& m_sceneManager;

      bool m_bIsRunning; ///< indicates if player is currently running

      bool m_bLeftDragging;   ///< indicates if user is currently left-dragging
      bool m_bRightDragging;  ///< indicates if user is currently right-dragging
      Point m_ptStoredDragging; ///< cursor coordinates stored while dragging

      double m_dViewAngleDirection; ///< camera angle for direction
      double m_dViewAngleUp;        ///< camera angle for looking up
      double m_dDistance;           ///< distance of camera from player

      Vector3d m_vecCameraOffset;   ///< camera offset; used to locate center to "head" of player
      Vector3d m_vCameraPos;        ///< calculated camera position

      Timer m_timer; ///< timer for adjusting view angle while walking
   };
}


//
// ThirdPersonViewController::Impl
//

/// implementation for ThirdPersonViewController class
class ThirdPersonViewController::Impl
{
public:
   /// ctor
   Impl(ISceneManager& sceneManager, T_fnSetCameraPosition fnSetCameraPosition,
         IPlayerViewModel& playerViewModel, KeyboardActionManager& keyboardActionManager)
      :m_positionControl(m_cameraControl, playerViewModel),
       m_cameraControl(m_positionControl, fnSetCameraPosition, playerViewModel, sceneManager),
       m_actionBindings(keyboardActionManager)
   {
   }

   /// registers action keys
   void RegisterKeyActions();

private:
   friend ThirdPersonViewController;

   /// position control
   PositionControl m_positionControl;

   /// camera control
   CameraControl m_cameraControl;

   /// key action bindings
   KeyboardActionBindings m_actionBindings;
};

void ThirdPersonViewController::Impl::RegisterKeyActions()
{
   // register some actions
   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionMoveForward,
      std::bind(&PositionControl::OnKeyMoveForward, &m_positionControl, std::placeholders::_1));
   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionMoveBackward,
      std::bind(&PositionControl::OnKeyMoveBackward, &m_positionControl, std::placeholders::_1));

   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionMoveLeft,
      std::bind(&PlusMinusControl::OnPlusKey, &m_positionControl.m_moveLeftControl, std::placeholders::_1));
   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionMoveRight,
      std::bind(&PlusMinusControl::OnMinusKey, &m_positionControl.m_moveLeftControl, std::placeholders::_1));

   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionRotateLeft,
      std::bind(&PositionControl::OnKeyRotateLeft, &m_positionControl, std::placeholders::_1));
   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionRotateRight,
      std::bind(&PositionControl::OnKeyRotateRight, &m_positionControl, std::placeholders::_1));

   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionAutoMove,
      std::bind(&PositionControl::OnKeyAutoMove, &m_positionControl, std::placeholders::_1));
}


//
// ThirdPersonViewController
//

ThirdPersonViewController::ThirdPersonViewController(
   KeyboardActionManager& keyboardActionManager,
   ISceneManager& sceneManager,
   IPlayerViewModel& playerViewModel,
   T_fnSetCameraPosition fnSetCameraPosition)
:m_bCameraInited(false),
 m_spImpl(new Impl(sceneManager, fnSetCameraPosition, playerViewModel, keyboardActionManager))
{
   m_spImpl->RegisterKeyActions();

   TimedEffect<ThirdPersonViewController>::Init();
}

void ThirdPersonViewController::Tick()
{
   TimedEffect<ThirdPersonViewController>::Tick();
}

void ThirdPersonViewController::OnMouseButtonEvent(bool bPressed,
   T_enMouseButtonType enButtonType, int /*x*/, int /*y*/)
{
   ATLASSERT(m_spImpl != NULL);

   m_spImpl->m_cameraControl.OnMouseButtonEvent(bPressed, enButtonType);
}

void ThirdPersonViewController::OnMouseMotionEvent(int /*x*/, int /*y*/, int xrel, int yrel)
{
   ATLASSERT(m_spImpl != NULL);

   m_spImpl->m_cameraControl.OnMouseMotionEvent(xrel, yrel);
}

void ThirdPersonViewController::OnMouseWheelEvent(int xrel, int yrel)
{
   ATLASSERT(m_spImpl != NULL);

   m_spImpl->m_cameraControl.OnMouseWheelEvent(xrel, yrel);
}

void ThirdPersonViewController::OnTick(double dElapsed)
{
   ATLASSERT(m_spImpl != NULL);

   if (!m_bCameraInited)
   {
      m_bCameraInited = true;
      m_spImpl->m_cameraControl.CalcPosition();
   }

   m_spImpl->m_cameraControl.OnTick(dElapsed);
   m_spImpl->m_positionControl.OnTick(dElapsed);
}

//
// PositionControl
//

PositionControl::PositionControl(CameraControl& cameraControl,
   IPlayerViewModel& playerViewModel)
:m_cameraControl(cameraControl),
 m_playerViewModel(playerViewModel),
 m_dMovementAngle(0.0),
 m_bAutoMove(false),
 m_bMouseLookMode(false)
{
}

bool PositionControl::IsPlayerAngleControllable() const throw()
{
   // when rotating, don't control player angle
   return !m_rotateLeftControl.IsKeyDown();
}

void PositionControl::OnKeyMoveForward(bool bKeyDown) throw()
{
   m_moveForwardControl.OnPlusKey(bKeyDown);

   // only cancel auto-move when pressing down key
   if (bKeyDown)
      m_bAutoMove = false;
}

void PositionControl::OnKeyMoveBackward(bool bKeyDown) throw()
{
   m_moveForwardControl.OnMinusKey(bKeyDown);

   // always cancel auto-move
   m_bAutoMove = false;
}

void PositionControl::OnKeyRotateLeft(bool bKeyDown) throw()
{
   if (m_bMouseLookMode)
      m_moveLeftControl.OnPlusKey(bKeyDown);
   else
      m_rotateLeftControl.OnPlusKey(bKeyDown);
}

void PositionControl::OnKeyRotateRight(bool bKeyDown) throw()
{
   if (m_bMouseLookMode)
      m_moveLeftControl.OnMinusKey(bKeyDown);
   else
      m_rotateLeftControl.OnMinusKey(bKeyDown);
}

void PositionControl::OnKeyAutoMove(bool /*bKeyDown*/) throw()
{
   // note: when using "numlock", the behavior of that key is different:
   // when pressed and num lock is off, bKeyDown is false, else true, and
   // only one key event is sent each time.
   m_bAutoMove = !m_bAutoMove;
}

bool PositionControl::IsRunning() const throw()
{
   return m_bAutoMove || m_moveForwardControl.IsKeyDown() && m_moveForwardControl.IsPlus();
}

void PositionControl::SetMouseLookMode(bool bMouseLookMode) throw()
{
   // changing mouselook mode?
   if (bMouseLookMode != m_bMouseLookMode)
   {
      // when strafe is active, transfer to rotate
      // when rotate is active, transfer to strafe
      std::swap(m_moveLeftControl, m_rotateLeftControl);
   }

   m_bMouseLookMode = bMouseLookMode;
}

void PositionControl::OnTick(double dElapsed)
{
   // set running flag for camera controller
   m_cameraControl.SetRunningFlag(IsRunning());

   // check if auto-move should be canceled;
   // only when backward key is pressed
   if (m_bAutoMove && m_moveForwardControl.IsKeyDown() && !m_moveForwardControl.IsPlus())
      m_bAutoMove = false;

   if (dElapsed == 0.0)
      return;

   // set up new movement info
   MovementInfo movementInfo(MovementInfo::movementPlayer);

   movementInfo.Position(m_playerViewModel.GetPlayerPosition());
   movementInfo.Direction(m_playerViewModel.GetPlayerMovementAngle());
   movementInfo.SetForwardMovement(m_bAutoMove || m_moveForwardControl.IsKeyDown(), m_moveForwardControl.IsPlus());
   movementInfo.SetSidewaysMovement(m_moveLeftControl.IsKeyDown(), m_moveLeftControl.IsPlus());

   if (movementInfo != m_movementInfo)
   {
      m_playerViewModel.UpdatePlayerMovement(movementInfo);

      m_movementInfo = movementInfo;
   }

   m_playerViewModel.UpdatePlayerPos();

   m_cameraControl.UpdateCameraPos();
   

   double m_dRotateSpeed = 0.0;

   if (m_rotateLeftControl.IsKeyDown())
      m_dRotateSpeed = m_rotateLeftControl.IsPlus() ? c_dAngleRotateSpeed : -c_dAngleRotateSpeed;

   // calculate new view angle
   if (m_dRotateSpeed != 0.0)
   {
      m_dMovementAngle += m_dRotateSpeed * dElapsed;
      m_dMovementAngle = AngleInRange(m_dMovementAngle);

      {
         m_playerViewModel.UpdatePlayerViewAngle(m_dMovementAngle);

         m_cameraControl.SetViewAngle(m_dMovementAngle);
      }
   }
}


//
// CameraControl
//

CameraControl::CameraControl(PositionControl& positionControl,
   ThirdPersonViewController::T_fnSetCameraPosition fnSetCameraPosition,
   IPlayerViewModel& playerViewModel,
   ISceneManager& sceneManager)
:m_positionControl(positionControl),
 m_fnSetCameraPosition(fnSetCameraPosition),
 m_playerViewModel(playerViewModel),
 m_sceneManager(sceneManager),
 m_bLeftDragging(false),
 m_bRightDragging(false),
 m_dViewAngleDirection(0.0),
 m_dViewAngleUp(0.0),
 m_dDistance(10.0),
 m_vecCameraOffset(0.0, 2.0, 0.0),
 m_bIsRunning(false)
{
   m_timer.Start();
}

void CameraControl::SetRunningFlag(bool bIsRunning) throw()
{
   if (!bIsRunning && m_bIsRunning)
      m_timer.Stop();
   else
      m_timer.Restart();

   m_bIsRunning = bIsRunning;
}

void CameraControl::SetViewAngle(double dViewAngle) throw()
{
   if (m_bLeftDragging || m_bRightDragging)
      return; // we are currently in control of view angle, don't set another value

   m_dViewAngleDirection = AngleInRange(180.0-dViewAngle);
   CalcPosition();
}

/// \todo do smooth scrolling when using mouse wheel
void CameraControl::OnTick(double dElapsed)
{
   if (m_bIsRunning && !m_bLeftDragging && !m_bRightDragging)
      AdjustViewAngleWalking(dElapsed);

   // update player transparency
   m_playerViewModel.SetPlayerTransparency(PlayerTransparency());
}

void CameraControl::UpdateCameraPos()
{
   CalcPosition();
}

void CameraControl::OnMouseButtonEvent(bool bPressed, T_enMouseButtonType enButtonType)
{
   if (bPressed)
   {
      // mouse button press

      // check if we started dragging
      bool bLastLeftDragging = m_bLeftDragging;
      bool bLastRightDragging = m_bRightDragging;

      if (enButtonType == SDL_BUTTON_LEFT)
         m_bLeftDragging = bPressed;
      else
      if (enButtonType == SDL_BUTTON_RIGHT)
         m_bRightDragging = bPressed;

      if ((m_bLeftDragging && !bLastLeftDragging) ||
          (m_bRightDragging && !bLastRightDragging))
      {
         // if we entered left dragging, store mouse coords for later
         m_ptStoredDragging = m_sceneManager.GetMousePos();

         m_sceneManager.ShowCursor(false);

         // move cursor to middle of screen
         Size s = m_sceneManager.GetScreenSize();
         m_sceneManager.SetMousePos(Point(s.Width()/2, s.Height()/2));
      }
   }
   else
   {
      // mouse button release
      if (enButtonType == SDL_BUTTON_LEFT)
         m_bLeftDragging = bPressed;
      else
      if (enButtonType == SDL_BUTTON_RIGHT)
         m_bRightDragging = bPressed;

      // only restore when user isn't left- and right-dragging anymore
      if (!m_bLeftDragging && !m_bRightDragging)
      {
         // restore cursor coordinates
         m_sceneManager.SetMousePos(m_ptStoredDragging);
         m_sceneManager.ShowCursor(true);
      }
   }

   // notify position controller
   m_positionControl.SetMouseLookMode(m_bRightDragging);

   // when stopping dragging and player is running, restart timer
   if (m_bIsRunning && !m_bLeftDragging && !m_bRightDragging)
      m_timer.Restart();
}

void CameraControl::OnMouseMotionEvent(int xrel, int yrel)
{
   if (m_bLeftDragging || m_bRightDragging)
   {
      MoveCameraAngle(xrel, yrel);

      // if left-dragging, also move the player angle
      if (m_bLeftDragging)
         MovePlayerAngle();

      Size s = m_sceneManager.GetScreenSize();
      m_sceneManager.SetMousePos(Point(s.Width()/2, s.Height()/2));
   }
}

void CameraControl::OnMouseWheelEvent(int xrel, int yrel)
{
   int dist = xrel + yrel;
   if (dist != 0)
      MoveCameraDistance(dist);
}

void CameraControl::MoveCameraDistance(int iRelChange)
{
   m_dDistance += c_dDistanceDeltaPerMousewheelTick * iRelChange;
   if (m_dDistance < 0.0)
      m_dDistance = 0.0;

   if (m_dDistance > c_dMaxDistance)
      m_dDistance = c_dMaxDistance;

   CalcPosition();
}

void CameraControl::MoveCameraAngle(int iRelChangeX, int iRelChangeY)
{
   m_dViewAngleDirection += 0.2 * iRelChangeX;
   m_dViewAngleUp += -0.2 * iRelChangeY;
   if (m_dViewAngleUp > 90.0)
      m_dViewAngleUp = 90.0;

   if (m_dViewAngleUp < -90.0)
      m_dViewAngleUp = -90.0;

   // TODO move to own function; checks for min angle related to the current distance and ground
   if (m_dViewAngleUp > 0.0)
      m_dViewAngleUp = 0.0;

   m_dViewAngleDirection = AngleInRange(m_dViewAngleDirection);

   CalcPosition();
}

/// \note this function is only called when left-dragging
void CameraControl::MovePlayerAngle()
{
   // only modify player angle when player is controllable
   if (!m_positionControl.IsPlayerAngleControllable())
   {
      //ATLTRACE(_T("player angle not controllable, exit\n"));
      return;
   }

   // use view angle to set player angle
   double dPlayerAngle = AngleInRange(-m_dViewAngleDirection + 180);

   m_positionControl.SetPlayerAngle(dPlayerAngle);

   m_playerViewModel.UpdatePlayerViewAngle(dPlayerAngle);
}

/// \details try to adjust view angle to match player movement angle
void CameraControl::AdjustViewAngleWalking(double dElapsed) throw()
{
   if (dElapsed == 0.0)
      return;

   m_timer.Restart();

   double dViewAngle = 180.0-m_dViewAngleDirection;

   // calculate step to view angle
   double dPlayerAngle = m_positionControl.GetPlayerAngle();
   double dDeltaAngle = dPlayerAngle - dViewAngle;
   if (dDeltaAngle == 0.0)
      return;

   //ATLTRACE(_T("player=%3.1f view=%3.1f delta=%3.1f\n"), dPlayerAngle, dViewAngle, dDeltaAngle);

   if (dDeltaAngle > 180.0)
      dDeltaAngle -= 360.0;

   double dDist = c_dRotateToViewDegreePerSecond * dElapsed;
   if (fabs(dDeltaAngle) < dDist)
      dDist = fabs(dDeltaAngle);

   if (dDeltaAngle < 0.0)
      dDist = -dDist;

   m_dViewAngleDirection -= dDist;
   m_dViewAngleDirection = AngleInRange(m_dViewAngleDirection);

   // recalc camera
   CalcPosition();
}

void CameraControl::CalcPosition() throw()
{
   m_vCameraPos = Vector3d(0.0, 0.0, m_dDistance);
   m_vCameraPos.RotateX(m_dViewAngleUp);
   m_vCameraPos.RotateY(-m_dViewAngleDirection);

   m_vCameraPos += m_vecCameraOffset;

   // update camera
   if (m_fnSetCameraPosition != nullptr)
      m_fnSetCameraPosition(m_vCameraPos, m_dViewAngleDirection, m_dViewAngleUp);
}

double CameraControl::PlayerTransparency() const throw()
{
   const double dMax = 3.0; // distance where player starts to get transparent
   const double dMin = 1.0; // distance where player is fully transparent

   ATLASSERT(dMax > dMin);

   if (m_dDistance > dMax)
      return 1.0;

   if (m_dDistance < dMin)
      return 0.0;

   // linear
   return (m_dDistance - dMin) / (dMax - dMin);
}
