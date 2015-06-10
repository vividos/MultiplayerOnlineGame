//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GameMenu/ISubPanel.hpp Game menu: Interface for sub-panel
//
#pragma once

// includes
#include <functional>

// forward references
class IWindowManager;

namespace GameMenu
{
   /// interface to sub panel
   class ISubPanel
   {
   public:
      /// ctor
      ISubPanel(std::function<void()> fnOnReturnMainPanel)
         :m_fnOnReturnMainPanel(fnOnReturnMainPanel)
      {
      }
      /// dtor
      virtual ~ISubPanel() throw() {}

      /// shows sub panel
      virtual void Show(IWindowManager& windowManager) = 0;

      /// hides sub panel
      virtual void Hide() = 0;

   protected:
      /// function to call when returning to main panel
      std::function<void()> m_fnOnReturnMainPanel;
   };

} // namespace GameMenu
