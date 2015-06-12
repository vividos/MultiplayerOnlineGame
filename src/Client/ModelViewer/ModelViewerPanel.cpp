//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ModelViewerPanel.cpp Model viewer panel
//

// includes
#include "StdAfx.h"
#include "ModelViewerPanel.hpp"
#include "IWindowManager.hpp"
#include "WindowManager.hpp"
#include "TextField.hpp"
#include "Button.hpp"
#include "Slider.hpp"
#include "Panel.hpp"

static LPCTSTR c_pszSliderNameBaseFigure = _T("sliderBaseFigure");
static LPCTSTR c_pszSliderNameSkinColor = _T("sliderSkinColor");
static LPCTSTR c_pszSliderNameFaceStyle = _T("sliderFaceStyle");
static LPCTSTR c_pszSliderNameHairColor = _T("sliderHairColor");
static LPCTSTR c_pszSliderNamePilosityHairStyle = _T("sliderPilosityHairStyle");
static LPCTSTR c_pszSliderNamePilosityBrowStyle = _T("sliderPilosityBrowStyle");

ModelViewer::Panel::Panel(IWindowManager& windowManager, IFileSystem& fileSystem)
{
   WindowManager::CreateFromJson(windowManager.GetRootWindow(), fileSystem, _T("ui/ModelViewerPanel.json"));

   m_spPanel = windowManager.FindWindow<::Panel>(_T("mainPanel"));
}

void ModelViewer::Panel::Show(IWindowManager& windowManager)
{
   m_spPanel->Create(*windowManager.GetRootWindow());

   std::shared_ptr<Button> spBtnPrevAnim = windowManager.FindWindow<Button>(_T("btnPrevAnim"));
   ATLASSERT(spBtnPrevAnim != nullptr);

   spBtnPrevAnim->On(ButtonEvent::Clicked,
      std::bind(&Panel::OnButtonPrevNext, this, false));

   std::shared_ptr<Button> spBtnNextAnim = windowManager.FindWindow<Button>(_T("btnNextAnim"));
   ATLASSERT(spBtnNextAnim != nullptr);

   spBtnNextAnim->On(ButtonEvent::Clicked,
      std::bind(&Panel::OnButtonPrevNext, this, true));

   std::shared_ptr<Slider> spSliderBaseFigure = windowManager.FindWindow<Slider>(c_pszSliderNameBaseFigure);
   spSliderBaseFigure->On(SliderEvent::Moved, std::bind(&Panel::OnSliderChanged, this, sliderBaseFigure));

   std::shared_ptr<Slider> spSlidererSkinColor = windowManager.FindWindow<Slider>(c_pszSliderNameSkinColor);
   spSlidererSkinColor->On(SliderEvent::Moved, std::bind(&Panel::OnSliderChanged, this, sliderSkinColor));

   std::shared_ptr<Slider> spSlidererFaceStyle = windowManager.FindWindow<Slider>(c_pszSliderNameFaceStyle);
   spSlidererFaceStyle->On(SliderEvent::Moved, std::bind(&Panel::OnSliderChanged, this, sliderFaceStyle));

   std::shared_ptr<Slider> spSlidererHairColor = windowManager.FindWindow<Slider>(c_pszSliderNameHairColor);
   spSlidererHairColor->On(SliderEvent::Moved, std::bind(&Panel::OnSliderChanged, this, sliderHairColor));

   std::shared_ptr<Slider> spSlidererPilosityHairStyle = windowManager.FindWindow<Slider>(c_pszSliderNamePilosityHairStyle);
   spSlidererPilosityHairStyle->On(SliderEvent::Moved, std::bind(&Panel::OnSliderChanged, this, sliderPilosityHairStyle));

   std::shared_ptr<Slider> spSlidererPilosityBrowStyle = windowManager.FindWindow<Slider>(c_pszSliderNamePilosityBrowStyle);
   spSlidererPilosityBrowStyle->On(SliderEvent::Moved, std::bind(&Panel::OnSliderChanged, this, sliderPilosityBrowStyle));
}

void ModelViewer::Panel::AnimationName(const CString& cszAnimationName)
{
   std::shared_ptr<TextField> spTextField =
      m_spPanel->GetWindowManager().FindWindow<TextField>(_T("animationName"));
   ATLASSERT(spTextField != nullptr);

   spTextField->SetAttr(TextFieldAttr::Text, cszAnimationName);
}

void ModelViewer::Panel::OnButtonPrevNext(bool bDirectionNext)
{
   if (m_fnOnChangeAnimation)
      m_fnOnChangeAnimation(bDirectionNext);
}

void ModelViewer::Panel::OnSliderChanged(T_enSliderType enSliderType)
{
   if (m_spPanel == nullptr || !m_fnOnChangeSlider)
      return;

   LPCTSTR pszSliderName = NULL;
   switch (enSliderType)
   {
   case sliderBaseFigure: pszSliderName = c_pszSliderNameBaseFigure; break;
   case sliderSkinColor: pszSliderName = c_pszSliderNameSkinColor; break;
   case sliderFaceStyle: pszSliderName = c_pszSliderNameFaceStyle; break;
   case sliderHairColor: pszSliderName = c_pszSliderNameHairColor; break;
   case sliderPilosityHairStyle: pszSliderName = c_pszSliderNamePilosityHairStyle; break;
   case sliderPilosityBrowStyle: pszSliderName = c_pszSliderNamePilosityBrowStyle; break;
   default:
      ATLASSERT(false);
      break;
   }

   IWindowManager& windowManager = m_spPanel->GetWindowManager();

   std::shared_ptr<Slider> spSlider = windowManager.FindWindow<Slider>(pszSliderName);
   if (spSlider == nullptr)
      return;

   unsigned int uiValue = spSlider->Current();

   m_fnOnChangeSlider(enSliderType, uiValue);
}
