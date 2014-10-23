//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GameMenu/AudioPanel.hpp Game menu: Audio panel
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
   /// \brief audio panel
   /// \details contains audio related settings such as volume sliders
   class AudioPanel : public ISubPanel
   {
   public:
      /// ctor
      AudioPanel(IWindowManager& windowManager, IFileSystem& fileSystem, std::function<void()> fnOnReturnMainPanel);
      /// dtor
      virtual ~AudioPanel() throw() {}

      /// shows sub panel
      virtual void Show(IWindowManager& windowManager) override;

      /// hides sub panel
      virtual void Hide() override;

   private:
      /// panel UI object
      std::shared_ptr<Panel> m_spPanel;
   };

} // namespace GameMenu
