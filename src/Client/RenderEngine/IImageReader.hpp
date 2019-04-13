//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IImageReader.hpp Image reader interface
//
#pragma once

// includes
#include "Color.hpp"
#include <vector>

// forward references
namespace Stream
{
   class IStream;
}

/// image reader interface
class IImageReader
{
public:
   /// ctor
   IImageReader()
      :m_uiWidth(0),
       m_uiHeight(0)
   {
   }
   /// dtor
   virtual ~IImageReader()
   {
   }

   /// loads new image from stream
   virtual void Load(Stream::IStream& stream) = 0;

   // get methods

   /// returns x resolution
   unsigned int Width() const { return m_uiWidth; }

   /// returns y resolution
   unsigned int Height() const { return m_uiHeight; }

   /// returns image pixels
   const std::vector<Color>& Pixels() const { return m_vecPixels; }

protected:
   /// width
   unsigned int m_uiWidth;

   /// height
   unsigned int m_uiHeight;

   /// RGBA pixels
   std::vector<Color> m_vecPixels;
};
