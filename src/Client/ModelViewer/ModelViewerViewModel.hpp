//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ModelViewerViewModel.hpp Model viewer view model
//
#pragma once

// includes
#include "CharGenerator/ViewModel.hpp"
#include <ulib/Observer.hpp>
#include "Player.hpp"

namespace ModelViewer
{
   /// \brief view model for ModelViewer
   /// \details builds on the character generator ViewModel
   class ViewModel: public CharGenerator::ViewModel
   {
   public:
      /// ctor
      ViewModel();
      /// dtor
      virtual ~ViewModel() {}

      // get methods

      /// returns current display info
      const MobileDisplayInfo& DisplayInfo() const { return m_player.DisplayInfo(); }

      /// returns current animation
      unsigned int CurrentAnimation() const { return m_uiCurrentAnimation; }

      /// returns max. animation number
      unsigned int MaxAnimationNumber() const { return m_vecAnimationNames.size(); }

      /// returns animation name
      CString AnimationName(unsigned int uiAnimation)
      {
         ATLASSERT(uiAnimation < m_vecAnimationNames.size());
         return m_vecAnimationNames[uiAnimation];
      }

      /// returns animation frames
      void GetAnimationFrames(const CString& cszAnimationName,
         unsigned int& uiStartFrame, unsigned int& uiEndFrame);

      // set methods

      /// sets new display info
      void DisplayInfo(const MobileDisplayInfo& displayInfo);

      /// sets current animation
      void CurrentAnimation(unsigned int uiCurrentAnimation);

      // events

      /// subject type for "model description changed" event
      typedef Subject<void(Player&)> T_ModelDescriptionChangedEvent;

      /// subject type for "animation changed" event
      typedef Subject<void(unsigned int)> T_AnimationChangedEvent;

      /// returns "model description changed" event object
      T_ModelDescriptionChangedEvent& ModelDescriptionChangedEvent() { return m_modelDescriptionChangedEvent; }

      /// returns "animation changed" event object
      T_AnimationChangedEvent& AnimationChangedEvent() { return m_animationChangedEvent; }

   private:
      /// current animation
      unsigned int m_uiCurrentAnimation;

      /// animation names
      std::vector<CString> m_vecAnimationNames;

      /// player object
      Player m_player;

      /// "model description changed" event subject
      T_ModelDescriptionChangedEvent m_modelDescriptionChangedEvent;

      /// "animation changed" event subject
      T_AnimationChangedEvent m_animationChangedEvent;
   };

} // namespace ModelViewer
