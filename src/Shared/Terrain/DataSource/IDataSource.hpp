//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IDataSource.hpp Interface for data sources for terrain height data blocks
//
#pragma once

namespace Terrain
{
namespace Model
{
// forward references
class DataBlock;
}

/// \brief terrain data source interface
/// \details provides DataBlock instances
class IDataSource
{
public:
   /// dtor
   virtual ~IDataSource() {}

   /// loads a square block of terrain data
   virtual std::shared_ptr<Model::DataBlock> LoadBlock(unsigned int x, unsigned int y, unsigned int size) = 0;
};

} // namespace Terrain
