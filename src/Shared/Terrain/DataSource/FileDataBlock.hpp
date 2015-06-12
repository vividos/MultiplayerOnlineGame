//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file FileDataBlock.hpp file data block
//
#pragma once

// includes
#include "TerrainCommon.hpp"
#include "Model/DataBlock.hpp"

// forward references
namespace Stream
{
class IStream;
}

namespace Terrain
{
/// size of a data block from file
const unsigned int c_uiFileDataBlockSize = 512;

/// \brief file based block data
/// \note always holds 512x512 height values (plus border to next block)
class TERRAIN_DECLSPEC FileDataBlock: public Model::DataBlock
{
public:
   /// ctor
   FileDataBlock();

   /// load data block
   void Load(Stream::IStream& stream);

   /// save data block
   void Save(Stream::IStream& stream) const;

private:
   /// reads block layer info
//   void ReadLayerInfo(Stream::IStream& stream, BlockLayerInfo& layerInfo);

   /// writes block layer info
//   void WriteLayerInfo(Stream::IStream& stream, const BlockLayerInfo& layerInfo) const;
};

} // namespace Terrain
