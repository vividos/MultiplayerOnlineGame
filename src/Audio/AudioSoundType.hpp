//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AudioSoundType.hpp Audio sound types
//
#pragma once

namespace Audio
{
/// sound ids
namespace Sound
{
/// user interface sound ids
namespace UI
{

/// sound for button press
static const TCHAR ButtonPress[] = _T("sound.ui.buttonpressed");

/// sound for item selected, e.g. checkbox
static const TCHAR ItemSelected[] = _T("sound.ui.itemselected");

/// open dialog sound
static const TCHAR Open[] = _T("sound.ui.opendialog");

/// close dialog sound
static const TCHAR Close[] = _T("sound.ui.closedialog");

} // namespace UI

/// world sound ids
namespace World
{
/// sound for getting loot
static const TCHAR GetLoot[] = _T("sound.world.get-loot");

/// sound for invalid target
static const TCHAR InvalidTarget[] = _T("sound.world.invalid-target");

/// sound for out of range
static const TCHAR OutOfRange[] = _T("sound.world.out-of-range");

/// sound for discovering new area
static const TCHAR NewArea[] = _T("sound.world.new-area");

/// sound for learning new skill
static const TCHAR NewSkill[] = _T("sound.world.new-skill");

/// sound for advancing skill
static const TCHAR AdvanceSkill[] = _T("sound.world.advance-skill");

} // namespace World

} // namespace Sound
} // namespace Audio
