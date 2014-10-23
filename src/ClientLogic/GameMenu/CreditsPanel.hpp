//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GameMenu/CreditsPanel.hpp Game menu: Credits panel
//
#pragma once

// includes
#include "ISubPanel.hpp"
#include "Observable.hpp"

// forward references
class IFileSystem;
class Panel;

namespace GameMenu
{
   /// \brief credits panel
   /// \details contains the game credits screen that lists all credits and contributors to the game
   class CreditsPanel : public ISubPanel
   {
   public:
      /// ctor
      CreditsPanel(IWindowManager& windowManager, IFileSystem& fileSystem, std::function<void()> fnOnReturnMainPanel);
      /// dtor
      virtual ~CreditsPanel() throw() {}

      /// shows sub panel
      virtual void Show(IWindowManager& windowManager) override;

      /// hides sub panel
      virtual void Hide() override;

   private:
      /// panel UI object
      std::shared_ptr<Panel> m_spPanel;
   };

} // namespace GameMenu
