//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Scene.hpp Scene base class
//
#pragma once

// includes
#include "ClientLogic.hpp"
#include "IMouseEventReceiver.hpp"

// forward references
class ISceneManager;
class IView;
class IViewController;

/// scene base class
class CLIENTLOGIC_DECLSPEC Scene: public IMouseEventReceiver
{
public:
   /// ctor
   Scene(ISceneManager& sceneManager) throw()
      :m_sceneManager(sceneManager)
   {
   }
   /// dtor
   virtual ~Scene() throw()
   {
   }

   /// is called before rendering a scene
   virtual void PreRender();

   /// called to handle mouse button event
   virtual void OnMouseButtonEvent(bool bPressed, T_enMouseButtonType enButtonType, int x, int y) override;
   /// called to handle mouse motion event
   virtual void OnMouseMotionEvent(int x, int y, int xrel, int yrel) override;
   /// called to handle mouse wheel event
   virtual void OnMouseWheelEvent(int xrel, int yrel) override;

protected:
   /// scene manager
   ISceneManager& m_sceneManager;

   /// view
   std::shared_ptr<IView> m_spView;

   /// view controller
   std::shared_ptr<IViewController> m_spController;
};
