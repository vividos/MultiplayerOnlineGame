//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file PngImageReader.hpp Image reader for PNG format
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "IImageReader.hpp"
#include <ulib/stream/IStream.hpp>
#include "Color.hpp"
#include <vector>

/// image reader for Portable Network Graphics (.png) image files
class RENDERENGINE_DECLSPEC PngImageReader : public IImageReader
{
public:
   /// dtor
   virtual ~PngImageReader()
   {
   }

   /// loads new image from stream
   virtual void Load(Stream::IStream& stream) override;
};
