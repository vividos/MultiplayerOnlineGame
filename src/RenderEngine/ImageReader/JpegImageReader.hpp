//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file JpegImageReader.hpp Image reader for JPEG format
//
#pragma once

// includes
#include "IImageReader.hpp"

/// image reader for JPEG image file format (.jpg) image files
class JpegImageReader: public IImageReader
{
public:
   /// dtor
   virtual ~JpegImageReader() throw()
   {
   }

   /// loads new image from stream
   virtual void Load(Stream::IStream& stream) override;
};
