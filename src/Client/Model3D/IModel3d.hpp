//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IModel3d.hpp Interface for 3d model
//
#pragma once

/// \brief interface for 3d model
/// \details A model contains the data that is loaded and used for all
/// instances of this model. Model instances are managed using instances
/// of classes derived from IModelDisplayState.
class IModel3d
{
public:
   /// dtor
   virtual ~IModel3d() {}

   /// prepares model for rendering
   virtual void Prepare() = 0;

   /// uploads data to graphics card
   virtual void Upload() = 0;
};

/// 3d model smart ptr
typedef std::shared_ptr<IModel3d> Model3dPtr;
