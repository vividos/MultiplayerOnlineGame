//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IBlockRenderData.hpp Interface to data for rendering a terrain block
//
#pragma once

// includes
#include "Vector3.hpp"

// forward references
class ViewFrustum3d;
class RenderOptions;

namespace Terrain
{
namespace Model
{
class DataBlock;
}
namespace View
{

/// interface for class that renders a single block
class IBlockRenderData
{
public:
   /// dtor
   virtual ~IBlockRenderData() {}

   /// prepares render data; called only once
   virtual void Prepare(std::shared_ptr<Model::DataBlock> spDataBlock) = 0;

   /// updates render data; called only once
   virtual void Upload() = 0;

   /// updates render data; called every frame
   virtual void Update(const Vector3d& vCamera) = 0;

   /// renders terrain block; called every frame
   virtual void Render(const RenderOptions& renderOptions, const ViewFrustum3d& viewFrustum) = 0;
};

} // namespace View
} // namespace Terrain
