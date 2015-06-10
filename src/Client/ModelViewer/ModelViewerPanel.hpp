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
   ~Panel() throw() {}

   /// function that is called when animation should be changed
   typedef std::function<void(bool)> T_fnOnChangeAnimation;

   /// sets callbacks
   void SetCallbacks(T_fnOnChangeAnimation fnOnChangeAnimation) throw()
   {
      m_fnOnChangeAnimation = fnOnChangeAnimation;
   }

   /// shows panel
   void Show(IWindowManager& wm);

   /// sets new animation name
   void AnimationName(const CString& cszAnimationName);

private:
   /// called when prev or next button is pressed
   void OnButtonPrevNext(bool bDirectionNext);

private:
   /// panel UI object
   std::shared_ptr<::Panel> m_spPanel;

   /// callback to change animation
   T_fnOnChangeAnimation m_fnOnChangeAnimation;
};

} // namespace ModelViewer
