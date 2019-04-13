//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IRenderable.hpp Interface for renderable objects
//
#pragma once

// forward references
class RenderOptions;

/// \brief interface for renderable objects
class IRenderable
{
public:
   /// dtor
   virtual ~IRenderable() {}

   /// returns if Prepare() call is needed (or it's a no-op)
   virtual bool IsPrepareNeeded() const = 0;

   /// returns if Upload() call is needed (or it's a no-op)
   virtual bool IsUploadNeeded() const = 0;

   /// prepares or calculates data; may be done on a worker thread
   virtual void Prepare() = 0;

   /// uploads data to graphics card; called in thread with rendering context
   virtual void Upload() = 0;

   /// renders object; called every frame; called in thread with rendering context
   virtual void Render(RenderOptions& options) = 0;

   /// cleans up data; called in thread with rendering context
   virtual void Done() = 0;
};
