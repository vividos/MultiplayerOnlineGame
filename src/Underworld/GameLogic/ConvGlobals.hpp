//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/GameLogic/ConvGlobals.hpp Conversation globals
//
#pragma once

// includes
#include <vector>

namespace Underworld
{

// classes

/// \brief Conversation globals
/// \details Stores all conversation globals for each conversation slot. Conversation
/// globals are loaded into the VM before executing the conversation script and are
/// retrieved after conversation ends.
class ConvGlobals
{
public:
   // types

   /// list of global variables for one conversation slot
   typedef std::vector<Uint16> ConvSlotGlobals;

   /// global variables for all slots
   typedef std::vector<ConvSlotGlobals> ConvGlobalsList;

   /// ctor
   ConvGlobals(){}

   /// returns number of slots
   unsigned int GetSlotCount() const { return m_vecAllGlobals.size(); }

   // loading / saving

   /// loads globals from savegame
   void Load(UwBase::Savegame& sg);

   /// saves globals to savegame
   void Save(UwBase::Savegame& sg) const;

   // direct access

   /// returns a list of globals for a given conv slot
   ConvSlotGlobals& GetVectorGlobals(Uint16 uiConvSlot)
   {
      UaAssert(uiConvSlot < GetSlotCount());
      return m_vecAllGlobals[uiConvSlot];
   }

   /// returns a list of globals for a given conv slot; const version
   const ConvSlotGlobals& GetVectorGlobals(Uint16 uiConvSlot) const
   {
      UaAssert(uiConvSlot < GetSlotCount());
      return m_vecAllGlobals[uiConvSlot];
   }

   /// returns globals for all slots
   ConvGlobalsList& GetVectorGlobalsList(){ return m_vecAllGlobals; }

protected:
   /// list with all globals from all conversations
   ConvGlobalsList m_vecAllGlobals;
};

} // namespace Underworld
