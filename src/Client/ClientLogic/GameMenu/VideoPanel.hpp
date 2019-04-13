//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GameMenu/VideoPanel.hpp Game menu: Video panel
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
   /// \brief video panel
   /// \details holds video settings that can be changed during gameplay
   class VideoPanel : public ISubPanel
   {
   public:
      /// ctor
      VideoPanel(IWindowManager& windowManager, IFileSystem& fileSystem, std::function<void()> fnOnReturnMainPanel);
      /// dtor
      virtual ~VideoPanel() {}

      /// shows sub panel
      virtual void Show(IWindowManager& windowManager) override;

      /// hides sub panel
      virtual void Hide() override;

   private:
      /// panel UI object
      std::shared_ptr<Panel> m_spPanel;
   };

} // namespace GameMenu
