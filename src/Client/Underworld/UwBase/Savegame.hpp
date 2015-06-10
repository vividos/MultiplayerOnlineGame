//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/UwBase/Savegame.hpp Savegame
//
#pragma once

// includes
#include <vector>

namespace UwBase
{

/// \brief Savegame class
/// Represents a savegame on disk and can be opened for loading or saving.
/// The SavegameInfo class contains infos about the savegame.
///
/// A savegame contains several sections that are started with BeginSection()
/// and ended with EndSection(). Values can be read and written using the
/// methods provided by Base::File. The savegame is automatically clsed when
/// the object is destroyed.
///
/// A savegame carries a version number that lets the user decide which fields
/// or parts of a savegame have to be loaded. This way newer game versions can
/// load older savegames.
class Savegame
{
public:
   /// ctor
   Savegame(){}

   // savegame loading functions

   /// returns version of savegame to load/save
   Uint32 GetVersion() const { return 0; }

   /// reads string from savegame
   void ReadString(CString& cszText){ cszText; }

   // savegame saving functions

   /// writes string to savegame
   void WriteString(const CString& cszText){ cszText; }

   // common functions

   /// starts new section to load/save
   void BeginSection(const CString& strSectionName){ strSectionName; }

   /// ends current section
   void EndSection(){}

   Uint32 Read32(){ return 0; }
   Uint16 Read16(){ return 0; }
   Uint8 Read8(){ return 0; }

   void Write32(Uint32 u){ u; }
   void Write16(Uint16 u){ u; }
   void Write8(Uint8 u){ u; }
};

} // namespace UwBase
