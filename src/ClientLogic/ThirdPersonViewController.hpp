//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ThirdPersonViewController.hpp Third person view controller
//
#pragma once

// includes
#include "ClientLogic.hpp"
#include "IViewController.hpp"
#include "TimedEffect.hpp"
#include "Scene.hpp"

// forward references
class Vector3d;
class IPlayerViewModel;
class ISceneManager;
class KeyboardActionManager;
class PerspectiveCamera;

/// \brief position controller for third person view (like in WoW)
/// \details The client reacts to key presses and suggests a view angle, movement
/// angle and a new position based on movement type. The user of this class
/// may or may not apply this movement. The following movements can be made:
/// General:
/// * move forward/backward keys: move player forward/backwards
/// * move left/right keys: rotate camera view angle and and player movement angle
/// * strafe left/right keys: move player sideways
/// * auto-run key: starts auto moving player forward
/// Player standing:
/// * left mouse drag: moves camera view angle and player movement angle
/// * right mouse drag: moves camera view angle
/// Player moving:
/// * not dragging mouse: camera view angle is slowly changed to player movement angle
/// * not dragging mouse: when camera view up angle is too high it's adjusted to a certain angle
/// * left mouse drag: as standing
/// * left mouse drag: as when standing
/// * when camera view up angle is too high it's adjusted to a certain angle
class CLIENTLOGIC_DECLSPEC ThirdPersonViewController:
   public IViewController,
   public TimedEffect<ThirdPersonViewController>
{
public:
   /// function type to set camera position
   typedef std::function<void(const Vector3d& vPos, double dAngleDirection, double dAngleUp)> T_fnSetCameraPosition;

   /// ctor
   ThirdPersonViewController(KeyboardActionManager& keyboardActionManager,
      ISceneManager& sceneManager,
      IPlayerViewModel& playerViewModel,
      T_fnSetCameraPosition fnSetCameraPosition = T_fnSetCameraPosition());
   /// dtor
   virtual ~ThirdPersonViewController() throw() {}

   // virtual methods from IViewController

   /// tick processing
   virtual void Tick() override;

   /// called on mouse button event
   virtual void OnMouseButtonEvent(bool bPressed, T_enMouseButtonType enButtonType, int x, int y) override;

   /// called on mouse motion event
   virtual void OnMouseMotionEvent(int x, int y, int xrel, int yrel) override;

   /// called to handle mouse wheel event
   virtual void OnMouseWheelEvent(int xrel, int yrel) override;

private:
   /// registers all movement keys
   void RegisterMovementKeys();

   friend TimedEffect<ThirdPersonViewController>;

   /// callback for TimedEffect mixin class
   void OnTick(double dElapsed);

private:
   /// indicates if camera is already init'ed
   bool m_bCameraInited;

   /// implementation class
   class Impl;

   /// implementation
   std::shared_ptr<Impl> m_spImpl;
};
