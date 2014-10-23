//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GameMenu/ControlsPanel.hpp Game menu: Controls panel
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
   /// \brief controls panel
   /// \details contains game controls settings such as key bindings or controller settings
   class ControlsPanel : public ISubPanel
   {
   public:
      /// ctor
      ControlsPanel(IWindowManager& windowManager, IFileSystem& fileSystem, std::function<void()> fnOnReturnMainPanel);
      /// dtor
      virtual ~ControlsPanel() throw() {}

      /// shows sub panel
      virtual void Show(IWindowManager& windowManager) override;

      /// hides sub panel
      virtual void Hide() override;

   private:
      /// panel UI object
      std::shared_ptr<Panel> m_spPanel;
   };

} // namespace GameMenu
