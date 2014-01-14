//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Import/ConvLoader.hpp Conversation code and conv. globals loader
//
#pragma once

// includes

// forward references
class IFileSystem;

namespace Stream
{
class IStream;
}

namespace Underworld
{
class ConvGlobals;
}

namespace Conv
{
class CodeVM;
}

namespace Import
{

/// conversation loader
class ConvLoader
{
public:
   /// loads conversation globals
   static void LoadConvGlobals(IFileSystem& fileSystem, Underworld::ConvGlobals& cg, bool bInitial);

   /// loads conversation code into VM
   static bool LoadConvCode(IFileSystem& fileSystem, Conv::CodeVM& vm, Uint16 conv);

private:
   /// loads imported functions
   static void LoadImportedFuncs(Conv::CodeVM& vm, std::shared_ptr<Stream::IStream> spFile);

private:
   /// ctor
   ConvLoader();
};

} // namespace Import
