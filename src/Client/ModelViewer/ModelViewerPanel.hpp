//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ModelViewerPanel.hpp Model viewer panel
//
#pragma once

// includes
#include <functional>

// forward references
class IWindowManager;
class IFileSystem;
class Panel;

namespace ModelViewer
{

/// panel with prev/next buttons and text field
class Panel
{
public:
   /// ctor
   Panel(IWindowManager& windowManager, IFileSystem& fileSystem);
   /// dtor
   ~Panel() {}

   /// type of slider that is moved
   enum T_enSliderType
   {
      sliderBaseFigure = 0,
      sliderSkinColor,
      sliderFaceStyle,
      sliderHairColor,
      sliderPilosityHairStyle,
      sliderPilosityBrowStyle,
   };

   /// function that is called when animation should be changed
   typedef std::function<void(bool)> T_fnOnChangeAnimation;

   /// function that is called when a slider value should be changed
   typedef std::function<void(T_enSliderType, unsigned int)> T_fnOnChangeSlider;

   /// sets callbacks
   void SetCallbacks(T_fnOnChangeAnimation fnOnChangeAnimation, T_fnOnChangeSlider fnOnChangeSlider)
   {
      m_fnOnChangeAnimation = fnOnChangeAnimation;
      m_fnOnChangeSlider = fnOnChangeSlider;
   }

   /// shows panel
   void Show(IWindowManager& wm);

   /// sets new animation name
   void AnimationName(const CString& cszAnimationName);

private:
   /// called when prev or next button is pressed
   void OnButtonPrevNext(bool bDirectionNext);

   /// called when slider has moved
   void OnSliderChanged(T_enSliderType enSliderType);

private:
   /// panel UI object
   std::shared_ptr<::Panel> m_spPanel;

   /// callback to change animation
   T_fnOnChangeAnimation m_fnOnChangeAnimation;

   /// callback to change slider
   T_fnOnChangeSlider m_fnOnChangeSlider;
};

} // namespace ModelViewer
