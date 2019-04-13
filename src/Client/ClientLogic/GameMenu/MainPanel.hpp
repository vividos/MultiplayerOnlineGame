//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GameMenu/MainPanel.hpp Game menu: Main menu panel
//
#pragma once

// includes
#include "ClientLogic.hpp"

// forward references
class IWindowManager;
class IFileSystem;
class Panel;

/// \brief Game menu implementation
/// \details Contains classes for the game menu. The game menu is shown when the user
/// presses Escape and can be used to adjust video, audio and controls settings. It also
/// can be used to show the credits, exit from the game and return to the game. In
/// multiplayer games it also has a button to log out and return to the login screen.
/// The current latency can be shown here, too.
namespace GameMenu
{
   class ISubPanel;

   /// main game menu panel
   class CLIENTLOGIC_DECLSPEC MainPanel
   {
   public:
      /// ctor
      MainPanel(IWindowManager& windowManager, IFileSystem& fileSystem);
      /// dtor
      ~MainPanel() {}

      /// shows panel
      void Show(IWindowManager& wm);

      /// hides panel
      void Hide();

      /// registers a function that is called when the user chooses to exit the game
      void RegisterExitClickedHandler(std::function<void()> fnOnExitClicked)
      {
         m_fnOnExitClicked = fnOnExitClicked;
      }

   private:
      /// main panel button types
      enum T_enButtonType
      {
         buttonVideo,      ///< "Video" button
         buttonAudio,      ///< "Audio" button
         buttonControls,   ///< "Controls" button
         buttonCredits,    ///< "Credits" button
         buttonExit,       ///< "Exit" button
         //buttonLogout, // not supported yet
         buttonReturn, ///< button for "Return to game"
      };

      /// called when a button is pressed
      void OnButtonPressed(T_enButtonType enButtonType);

      /// called when returning to the main panel
      void OnReturnMainPanel();

   private:
      /// window manager
      IWindowManager& m_windowManager;

      /// file system to load panel
      IFileSystem& m_fileSystem;

      /// indicates if main panel is currently shown
      bool m_bShown;

      /// main panel UI object
      std::shared_ptr<Panel> m_spPanel;

      /// sub panel object
      std::shared_ptr<ISubPanel> m_spSubPanel;

      /// function called when the user chooses to exit the game
      std::function<void()> m_fnOnExitClicked;
   };

} // namespace GameMenu
