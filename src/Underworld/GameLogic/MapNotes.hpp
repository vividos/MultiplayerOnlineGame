//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/GameLogic/MapNotes.hpp Map notes
//
#pragma once

// includes
#include <vector>

namespace Underworld
{

// structs

/// map note entry
struct MapNote
{
   /// ctor
   MapNote()
      :m_xpos(0.0),
       m_ypos(0.0)
   {}

   /// relative x position on map
   double m_xpos;

   /// relative x position on map
   double m_ypos;

   /// note text
   CString m_cszText;
};


// classes

/// map notes class
class MapNotes
{
public:
   /// ctor
   MapNotes() {}

   /// returns number of notes
   unsigned int GetNoteCount() const { return m_vecNotes.size(); }

   /// returns a single note
   MapNote& GetNote(unsigned int uiIndex)
   {
      UaAssert(uiIndex < GetNoteCount());
      return m_vecNotes[uiIndex];
   }

   // loading / saving

   /// loads notes from savegame
   void Load(UwBase::Savegame& sg);

   /// saves notes to savegame
   void Save(UwBase::Savegame& sg) const;

   /// returns list of notes
   std::vector<MapNote>& GetVectorNotes(){ return m_vecNotes; }

protected:
   /// list of all notes
   std::vector<MapNote> m_vecNotes;
};

} // namespace Underworld
