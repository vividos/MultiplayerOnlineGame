//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file UserInterfaceAudio.hpp User interface audio events
//
#pragma once

// includes
#include <functional>

/// user interface audio events
enum T_enUserInterfaceAudioEvents
{
   uiButtonPress = 0,   ///< button is pressed
   uiItemSelect,        ///< item, checkbox is being selected
   uiDialogOpen,        ///< dialog opened
   uiDialogClose,       ///< dialog closed
};

/// typedef of a function that signals audio events for UI
typedef std::function<void(T_enUserInterfaceAudioEvents enAudioEvent)> T_fnOnUserInterfaceAudioEvent;
