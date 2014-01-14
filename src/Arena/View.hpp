//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Arena\View.hpp Arena view
//
#pragma once

// includes
#include "IView.hpp"
#include "KeyboardActionBindings.hpp"

// forward references
class IFileSystem;
class ISceneManager;
class GraphicsTaskManager;
class RenderEngine;
class ModelRenderManager;
class PerspectiveCamera;

namespace Arena
{
class ViewModel;
class Scenegraph;

/// \brief arena view; View class of MVVM pattern
/// \details the view is a combination of scenegraph (ArenaScenegraph) and behavior
/// or controller
class View: public IView
{
public:
   /// ctor
   View(KeyboardActionManager& keyboardActionManager,
      IFileSystem& fileSystem,
      GraphicsTaskManager& taskManager,
      RenderEngine& engine,
      ViewModel& viewModel);

   /// returns camera
   std::shared_ptr<PerspectiveCamera> GetCamera() { return m_spCamera; }

   // virtual methods from IView

   /// called before rendering
   virtual void PreRender() override;

private:
   /// scenegraph to render view
   std::shared_ptr<Scenegraph> m_spScenegraph;

   /// view model
   ViewModel& m_viewModel;

   /// keyboard to action bindings
   KeyboardActionBindings m_actionBindings;

   /// camera
   std::shared_ptr<PerspectiveCamera> m_spCamera;
};

} // namespace Arena
