//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TgaImageReader.hpp Image reader for TGA format
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "IImageReader.hpp"

/// image reader for Targa Image (.tga) files
class RENDERENGINE_DECLSPEC TgaImageReader: public IImageReader
{
public:
   /// dtor
   virtual ~TgaImageReader() throw()
   {
   }

   /// loads new image from stream
   virtual void Load(Stream::IStream& stream) override;

   // get methods

   /// returns color map (if any)
   const std::vector<Color>& GetColorMap() const throw() { return m_vecColorMap; }

private:
   /// loads color map from stream
   void LoadColorMap(Stream::IStream& stream,
      WORD wColorMapFirstIndex, WORD wColorMapLength, BYTE bNumberOfBits);

   /// loads image using color map
   void LoadColorMapPixels(Stream::IStream& stream, DWORD dwLength);

   /// loads truecolor image
   void LoadTrueColorPixels(Stream::IStream& stream, DWORD dwLength, BYTE bBitsPerPixel);

private:
   /// color map
   std::vector<Color> m_vecColorMap;
};
