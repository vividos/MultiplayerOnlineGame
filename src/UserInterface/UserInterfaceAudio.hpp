//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
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
   uiButtonLeave,       ///< button is depressed and more than 1 second has passed
   uiItemSelect,        ///< item, checkbox is being selected
   uiDialogOpenModal,      ///< modal dialog opened
   uiDialogOpenNonmodal,   ///< modal dialog closed
   uiDialogCloseModal,     ///< nonmodal dialog opened
   uiDialogCloseNonmodal,  ///< nonmodal dialog closed
   uiTooltipOpened,        ///< a tooltip has been opened using a button
   uiScrollOpen,        ///< a dialog with a scroll is opened (quest scroll, map, ...)
   uiScrollClosed,      ///< a dialog with a scroll is closed
};

/// typedef of a function that signals audio events for UI
typedef std::function<void(T_enUserInterfaceAudioEvents enAudioEvent)> T_fnOnUserInterfaceAudioEvent;
