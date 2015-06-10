//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BitmapImageWriter.hpp Bitmap image writer class
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"

// forward references
class Bitmap;

/// bitmap image writer
class RENDERENGINE_DECLSPEC BitmapImageWriter: public boost::noncopyable
{
public:
   /// ctor
   BitmapImageWriter(const Bitmap& bmp) throw()
      :m_bmp(bmp)
   {
   }

   /// writes bitmap to file
   void Write(const CString& cszFilename) const;

private:
   /// bitmap to write
   const Bitmap& m_bmp;
};
