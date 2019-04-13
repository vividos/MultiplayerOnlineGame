//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ModelViewerView.hpp Model viewer view
//
#pragma once

// includes
#include "IScenegraph.hpp"
#include "CharGenerator/View.hpp"
#include "LightingManager.hpp"
#include "ModelRenderManager.hpp"

// forward references
class PerspectiveCamera;
class GraphicsTaskManager;
class Player;

namespace ModelViewer
{
   class ViewModel;

   /// view for ModelViewer
   class View :
      public CharGenerator::View,
      public IScenegraph
   {
   public:
      /// ctor
      View(ViewModel& viewModel,
         IFileSystem& fileSystem,
         RenderEngine& renderEngine,
         GraphicsTaskManager& taskManager);

      /// returns camera
      std::shared_ptr<PerspectiveCamera> GetCamera() { return m_spCamera; }

   private:
      /// called when model description changed
      void OnModelDescriptionChanged(Player& player);

      /// called when animation has changed
      void OnAnimationChanged(unsigned int uiCurrentAnimation);

      // virtual methods from IScenegraph

      /// renders view
      virtual void Render(RenderOptions& options) override;

      // virtual methods from IView

      /// called before rendering view
      virtual void PreRender() override {}

   private:
      /// view model
      ViewModel& m_viewModel;

      /// render engine
      RenderEngine& m_engine;

      /// camera
      std::shared_ptr<PerspectiveCamera> m_spCamera;

      /// lighting manager
      OpenGL::LightingManager m_lightingManager;

      /// model render manager
      ModelRenderManager m_modelRenderManager;

      /// indicates if player model already has been init'ed
      bool m_bModelInited;
   };

} // namespace ModelViewer
