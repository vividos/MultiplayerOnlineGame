//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Conv/Decompiler.hpp Underworld conversation code decompiler
//
#pragma once

// forward references
class IFileSystem;
class GameStrings;

namespace Conv
{
class CodeGraph;

/// uw conversation code decompiler
class Decompiler
{
public:
   /// ctor
   Decompiler(Uint16 uiConv, IFileSystem& fileSystem, GameStrings& strings);

   /// returns name of conversation partner
   std::string GetName() const;

   /// writes out decompiled code
   void Write(FILE* fd);

private:
   /// conversation number
   Uint16 m_uiConv;

   /// game strings
   GameStrings& m_strings;

   /// code graph
   std::shared_ptr<CodeGraph> m_spCodeGraph;
};

} // namespace Conv
