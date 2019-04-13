//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file PcxImageReader.hpp Image reader for PCX image format
//
#pragma once

// includes
#include "IImageReader.hpp"

/// image reader for ZSoft PCX image files (.pcx)
class PcxImageReader: public IImageReader
{
public:
   /// dtor
   virtual ~PcxImageReader()
   {
   }

   /// loads new image from stream
   virtual void Load(Stream::IStream& stream) override;
};
