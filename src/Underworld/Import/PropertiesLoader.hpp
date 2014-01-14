//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Import/PropertiesLoader.hpp Object propreties loader
//
#pragma once

// includes
#include "Properties.hpp"

// forward references
class IFileSystem;

namespace Stream
{
class IStream;
}

namespace Import
{

/// properties loader
class PropertiesLoader
{
public:
   /// ctor
   PropertiesLoader(IFileSystem& fileSystem)
      :m_fileSystem(fileSystem)
   {
   }

   /// loads object properties
   void Load(Underworld::ObjectProperties& properties);

private:
   /// loads common object properties
   void LoadCommonObjectProperties(Underworld::ObjectProperties& properties);

   /// loads specific object properties
   void LoadObjectProperties(Underworld::ObjectProperties& properties);

private:
   /// file system
   IFileSystem& m_fileSystem;

   /// current file
   std::shared_ptr<Stream::IStream> m_spFile;
};

} // namespace Import
