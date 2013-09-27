//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/Conv/Conversation.hpp Conversation class
//
#pragma once

// includes
#include "CodeVM.hpp"

// forward references
namespace Underworld
{
class World;
}

namespace Conv
{
class CodeCallback;


// classes

/// \brief conversation
/// \details implements conversation using the Underworld object
/// to implement imported functions and globals
class Conversation: public CodeVM
{
public:
   /// ctor
   Conversation(Underworld::World& uw)
      :m_uw(uw)
   {}

   /// inits basic conversation
   virtual void Init(unsigned int uiConvLevel,
      Uint16 uiConvObjPos,
      CodeCallback* pCodeCallback,
      std::vector<std::string>& vecLocalStrings);

   /// cleans up basic conversation
   virtual void Done();

   /// returns local string
   virtual std::string GetLocalString(Uint16 uiStringNr) override;

   /// allocates new local string
   Uint16 AllocString(const char* pszStr);

protected:
   // virtual methods from CodeVM
   virtual void ImportedFunc(const char* pszFuncName) override;
   virtual Uint16 GetGlobal(const char* pszGlobName) override;
   virtual void SetGlobal(const char* pszGlobName, Uint16 val) override;

protected:
   /// underworld object
   Underworld::World& m_uw;

   /// underworld level of conversation partner
   unsigned int m_uiConvLevel;

   /// object position of conversation partner
   Uint16 m_uiConvObjPos;

   /// all current local strings
   std::vector<std::string> m_vecLocalStrings;
};

} // namespace Conv
