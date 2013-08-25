//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Checkerboard.hpp Checkerboard render class
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "IRenderable.hpp"

/// checkerboard floor
class RENDERENGINE_DECLSPEC Checkerboard: public IRenderable
{
public:
   /// ctor
   Checkerboard(int xMin = -16, int xMax = 16, int yMin = -16, int yMax = 16, float fHeight = 0.0)
      :m_fHeight(fHeight),
       m_xMin(xMin),
       m_xMax(xMax),
       m_yMin(yMin),
       m_yMax(yMax)
   {
   }

   // virtual methods from IRenderable

   virtual bool IsPrepareNeeded() const { return false; }
   virtual bool IsUploadNeeded() const { return false; }
   virtual void Prepare(){}
   virtual void Upload(){}
   virtual void Done(){}

   /// render checkerboard
   void Render(RenderOptions& options) throw();

private:
   int m_xMin; ///< min x value
   int m_xMax; ///< max x value
   int m_yMin; ///< min y value
   int m_yMax; ///< max y value
   float m_fHeight;  ///< height of checkerboard
};
