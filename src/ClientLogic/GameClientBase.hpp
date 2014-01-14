//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ClientLogic/GameClientBase.hpp Game client base class
//
#pragma once

// includes
#include "ClientLogic.hpp"
#include "MainGameLoop.hpp"
#include "ISceneManager.hpp"
#include "KeyboardActionBindings.hpp"
#include "GraphicsTaskManager.hpp"
#include <functional>

// forward references
class RenderEngine;
class IWindowManager;

/// game client base class
class CLIENTLOGIC_DECLSPEC GameClientBase :
   public MainGameLoop,
   public ISceneManager
{
public:
   /// ctor
   GameClientBase(const CString& cszClientName);
   /// dtor
   virtual ~GameClientBase();

   /// initializes client window
   void Init(unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen);

   /// runs game
   void Run();

   /// returns render engine
   RenderEngine& GetRenderEngine();

   /// returns window manager
   IWindowManager& GetWindowManager();

   /// returns keyboard action manager
   KeyboardActionManager& GetKeyboardActionManager() throw() { return m_keyboardActionManager; }

   /// returns graphics task manager
   GraphicsTaskManager& GetGraphicsTaskManager() throw() { return m_graphicsTaskManager; }

protected:
   // virtual methods from MainGameLoop

   virtual void OnRender() override;
   virtual void OnEvent(SDL_Event& evt) override;

   // virtual methods from ISceneManager

   virtual void ChangeScene(std::shared_ptr<Scene> spScene) override;
   virtual void DispatchInEventLoop(std::function<void()> fn) override;
   virtual Point GetMousePos() override;
   virtual void SetMousePos(const Point& pt) override;
   virtual void ShowCursor(bool bShow) override;
   virtual Size GetScreenSize() const throw() override;

private:
   /// updates caption of client
   void UpdateCaption(const CString& cszCaption);

   /// toggles fullscreen window
   void ToggleFullscreen();

private:
   /// render engine
   std::shared_ptr<RenderEngine> m_spRenderEngine;

   /// window manager
   std::shared_ptr<IWindowManager> m_spWindowManager;

   /// keyboard action manager
   /// \note this variable must be above the Scene ptr so that it gets destroyed after scene
   KeyboardActionManager m_keyboardActionManager;

   /// keyboard action bindings
   KeyboardActionBindings m_actionBindings;

   /// graphics task manager
   GraphicsTaskManager m_graphicsTaskManager;

   /// scene
   std::shared_ptr<Scene> m_spScene;
   /// previous scene; used to store scene ptr when changing scenes
   std::shared_ptr<Scene> m_spPreviousScene;

   /// client name
   CString m_cszClientName;

   /// indicates if client is in fullscreen
   bool m_bFullscreen;

   /// indicates that next mouse motion event should be ignored
   bool m_bIgnoreNextMouseMotion;
   /// mouse motion x pos to ignore
   Sint32 m_iMouseMotionPosToIgnoreX;
   /// mouse motion y pos to ignore
   Sint32 m_iMouseMotionPosToIgnoreY;
};
