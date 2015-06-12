//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file FileDataBlock.cpp file data block
//

// includes
#include "StdAfx.h"
#include "FileDataBlock.hpp"
#include <ulib/Stream/IStream.hpp>
#include <ulib/Stream/EndianAwareFilter.hpp>

using Terrain::FileDataBlock;
//using Terrain::BlockLayerInfo;

FileDataBlock::FileDataBlock()
:DataBlock(c_uiFileDataBlockSize)
{
}

void FileDataBlock::Load(Stream::IStream& stream)
{
   // read layer info
   //ReadLayerInfo(stream, GetLayerInfo(0));
   //ReadLayerInfo(stream, GetLayerInfo(1));

   // read elevation data
   std::vector<float>& vecElevationData = GetElevationData().RawData();

   DWORD dwRead = 0;
   stream.Read(&vecElevationData[0], sizeof(float)*vecElevationData.size(), dwRead);

   if (dwRead != sizeof(float)*vecElevationData.size())
      throw Exception(_T("invalid terrain data block in file"), __FILE__, __LINE__);
}

#if 0
void FileDataBlock::ReadLayerInfo(Stream::IStream& stream, BlockLayerInfo& layerInfo)
{
   // colors
   DWORD dwRead = 0;
   stream.Read(&layerInfo.m_layerColor, sizeof(layerInfo.m_layerColor), dwRead);

   if (dwRead != sizeof(layerInfo.m_layerColor))
      throw Exception(_T("invalid terrain data block in file"), __FILE__, __LINE__);

   // id of layer
   stream.Read(&layerInfo.m_uiIdLayer, sizeof(layerInfo.m_uiIdLayer), dwRead);

   if (dwRead != sizeof(layerInfo.m_uiIdLayer))
      throw Exception(_T("invalid terrain data block in file"), __FILE__, __LINE__);
}
#endif

void FileDataBlock::Save(Stream::IStream& stream) const
{
   // write layer info
   //WriteLayerInfo(stream, GetLayerInfo(0));
   //WriteLayerInfo(stream, GetLayerInfo(1));

   // write elevation data
   const std::vector<float>& vecElevationData = GetElevationData().RawData();

   DWORD dwBytesWritten = 0;
   stream.Write(&vecElevationData[0], sizeof(float)*vecElevationData.size(), dwBytesWritten);

   if (dwBytesWritten != sizeof(float)*vecElevationData.size())
      throw Exception(_T("invalid terrain data block in file"), __FILE__, __LINE__);
}

#if 0
void FileDataBlock::WriteLayerInfo(Stream::IStream& stream, const BlockLayerInfo& layerInfo) const
{
   // colors
   DWORD dwWritten = 0;
   stream.Write(&layerInfo.m_layerColor, sizeof(layerInfo.m_layerColor), dwWritten);

   if (dwWritten != sizeof(layerInfo.m_layerColor))
      throw Exception(_T("invalid terrain data block in file"), __FILE__, __LINE__);

   stream.Write(&layerInfo.m_uiIdLayer, sizeof(layerInfo.m_uiIdLayer), dwWritten);

   if (dwWritten != sizeof(layerInfo.m_uiIdLayer))
      throw Exception(_T("invalid terrain data block in file"), __FILE__, __LINE__);
}
#endif
