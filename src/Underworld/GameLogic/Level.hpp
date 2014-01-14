//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/GameLogic/Level.hpp Level
//
#pragma once

// includes
#include "Tilemap.hpp"
#include "ObjectList.hpp"
#include "MapNotes.hpp"

namespace Underworld
{

/// \brief Level of the underworld
/// \details A level of the underworld consists of a tilemap, an object list and automap
/// notes. A level can have a unique name.
class Level: public boost::noncopyable
{
public:
   /// ctor
   Level(){}

   /// returns level name
   CString GetLevelName() const throw() { return m_cszLevelName; }

   /// sets new level name
   void SetLevelName(const CString& cszLevelName){ m_cszLevelName = cszLevelName; }

   // level contents

   /// returns tilemap
   Tilemap& GetTilemap() throw(){ return m_tilemap; }
   /// returns tilemap; const version
   const Tilemap& GetTilemap() const throw() { return m_tilemap; }

   /// returns object list
   ObjectList& GetObjectList() throw() { return m_objectList; }
   /// returns object list; const version
   const ObjectList& GetObjectList() const throw() { return m_objectList; }

   /// returns map notes
   MapNotes& GetMapNotes(){ return m_mapNotes; }
   /// returns map notes
   const MapNotes& GetMapNotes() const { return m_mapNotes; }

   // loading / saving

   /// saves level
   void Load(UwBase::Savegame& sg)
   {
      GetTilemap().Load(sg);
      GetObjectList().Load(sg);
      GetMapNotes().Load(sg);
   }

   /// loads level
   void Save(UwBase::Savegame& sg) const
   {
      GetTilemap().Save(sg);
      GetObjectList().Save(sg);
      GetMapNotes().Save(sg);
   }

private:
   /// level name
   CString m_cszLevelName;

   /// tilemap of level
   Tilemap m_tilemap;

   /// level object list
   ObjectList m_objectList;

   /// automap notes
   MapNotes m_mapNotes;
};

} // namespace Underworld
