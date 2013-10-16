//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file PngImageReader.cpp Image reader for PNG format
//

// includes
#include "StdAfx.h"
#include "PngImageReader.hpp"
#include "PNG.hpp"

void PngImageReader::Load(Stream::IStream& stream)
{
   PNG::Decoder d(stream);
   d.DecodeImage(true);

   m_uiWidth = d.Info().m_uiWidth;
   m_uiHeight = d.Info().m_uiHeight;

   std::swap(m_vecPixels, d.Image());
}
