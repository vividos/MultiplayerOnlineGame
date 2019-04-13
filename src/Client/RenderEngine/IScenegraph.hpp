//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IScenegraph.hpp Scenegraph interface
//
#pragma once

// includes
#include "IRenderable.hpp"

// forward references
class RenderEngine;

/// scenegraph interface
class IScenegraph: public IRenderable
{
public:
   /// ctor
   IScenegraph()
   {
   }

   /// dtor
   virtual ~IScenegraph()
   {
   }

   /// returns if Prepare() call is needed (or it's a no-op)
   virtual bool IsPrepareNeeded() const override
   {
      return false;
   }

   /// returns if Upload() call is needed (or it's a no-op)
   virtual bool IsUploadNeeded() const override
   {
      return false;
   }

   /// prepares or calculates data; may be done on a worker thread
   virtual void Prepare() override
   {
   }

   /// uploads data to graphics card; called in thread with rendering context
   virtual void Upload() override
   {
   }

   /// renders object; called every frame; called in thread with rendering context
   virtual void Render(RenderOptions& options) = 0;

   /// cleans up data; called in thread with rendering context
   virtual void Done() override
   {
   }
};
