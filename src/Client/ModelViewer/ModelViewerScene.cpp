//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ModelViewerScene.cpp Model viewer scene
//

// includes
#include "StdAfx.h"
#include "ModelViewerScene.hpp"
#include "ModelViewerView.hpp"
#include "CharGenerator/Controller.hpp"
#include "ModelViewerPanel.hpp"
#include "KeyboardActionManager.hpp"
#include "RenderEngine.hpp"
#include "RenderOptions.hpp"

ModelViewer::Scene::Scene(ISceneManager& sceneManager,
   IWindowManager& windowManager,
   KeyboardActionManager& keyboardActionManager,
   IFileSystem& fileSystem,
   RenderEngine& renderEngine,
   GraphicsTaskManager& taskManager)
:BaseClass(sceneManager),
m_spView(new View(m_viewModel, fileSystem, renderEngine, taskManager)),
m_actionBindings(keyboardActionManager)
{
   // keyboard bindings
   keyboardActionManager.RegisterActionKey(_T("space"), KeyboardActionManager::actionAction1);

   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionAction1, false,
      std::bind(&ModelViewer::Scene::OnChangeAnimation, this, true));

   // set scenegraph
   renderEngine.SetScenegraph(m_spView);

   SetupPanel(windowManager, fileSystem);

   // set controller
   std::shared_ptr<CharGenerator::Controller> spController(
      new CharGenerator::Controller(keyboardActionManager, m_spView->GetCamera()));

   m_spController = spController;

   // set render options
   renderEngine.GetRenderOptions().Set(RenderOptions::optionModelBoundingBox, false);
   renderEngine.GetRenderOptions().Set(RenderOptions::optionModelJoints, false);
   renderEngine.GetRenderOptions().Set(RenderOptions::optionModelNormals, false);

   // init player
   MobileDisplayInfo displayInfo(0x0000);
   m_viewModel.DisplayInfo(displayInfo);
}

void ModelViewer::Scene::SetupPanel(IWindowManager& windowManager, IFileSystem& fileSystem)
{
   m_spPanel.reset(new ModelViewer::Panel(windowManager, fileSystem));
   m_spPanel->Show(windowManager);

   m_spPanel->AnimationName(m_viewModel.AnimationName(m_viewModel.CurrentAnimation()));

   m_spPanel->SetCallbacks(
      std::bind(&ModelViewer::Scene::OnChangeAnimation, this, std::placeholders::_1),
      std::bind(&ModelViewer::Scene::OnChangeSlider, this, std::placeholders::_1, std::placeholders::_2));
}

void ModelViewer::Scene::OnChangeAnimation(bool bNextAnim)
{
   unsigned int uiCurrentAnimation = m_viewModel.CurrentAnimation();

   if (bNextAnim)
   {
      uiCurrentAnimation++;
      if (uiCurrentAnimation >= m_viewModel.MaxAnimationNumber())
         uiCurrentAnimation = 0;
   }
   else
   {
      if (uiCurrentAnimation == 0)
         uiCurrentAnimation = m_viewModel.MaxAnimationNumber() - 1;
      else
         uiCurrentAnimation--;
   }

   m_viewModel.CurrentAnimation(uiCurrentAnimation);

   CString cszAnimationName = m_viewModel.AnimationName(uiCurrentAnimation);
   m_spPanel->AnimationName(cszAnimationName);
}

void ModelViewer::Scene::OnChangeSlider(ModelViewer::Panel::T_enSliderType enSliderType, unsigned int uiNewValue)
{
   MobileDisplayInfo newDisplayInfo = m_viewModel.DisplayInfo();

   switch (enSliderType)
   {
   case ModelViewer::Panel::sliderBaseFigure: newDisplayInfo.BaseFigure(uiNewValue); break;
   case ModelViewer::Panel::sliderSkinColor: newDisplayInfo.SkinColor(uiNewValue); break;
   case ModelViewer::Panel::sliderFaceStyle: newDisplayInfo.FaceStyle(uiNewValue); break;
   case ModelViewer::Panel::sliderHairColor: newDisplayInfo.HairColor(uiNewValue); break;
   case ModelViewer::Panel::sliderPilosityHairStyle: newDisplayInfo.PilosityHairStyle(uiNewValue); break;
   case ModelViewer::Panel::sliderPilosityBrowStyle: newDisplayInfo.PilosityBrowStyle(uiNewValue); break;
   default:
      ATLASSERT(false);
      break;
   }

   m_viewModel.DisplayInfo(newDisplayInfo);
}

