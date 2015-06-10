//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file MD2\ModelManager.cpp MD2 model manager
//

// includes
#include "StdAfx.h"
#include "ModelManager.hpp"
#include "IModelDisplayState.hpp"
#include "IFileSystem.hpp"
#include "ModelData.hpp"
#include "ModelDisplayState.hpp"
#include "TextureLoader.hpp"

using namespace MD2;

std::shared_ptr<IModelDisplayState> ModelManager::Create(const Mobile& /*mobile*/)
{
   // TODO generate from mobile
   CString cszModelName = _T("homer");
   CString cszTextureName = _T("homer");

   if (m_mapModelData.find(cszModelName) == m_mapModelData.end())
      LoadModelData(cszModelName);

   if (!m_textureMap.IsTextureAvail(cszTextureName))
      LoadTexture(cszModelName, cszTextureName);

   std::shared_ptr<ModelDisplayState> spDisplayState(
      new MD2::ModelDisplayState(
         m_mapModelData[cszModelName],
         m_textureMap.Get(cszModelName + _T("-") + cszTextureName))
      );

   spDisplayState->Prepare();

   return spDisplayState;
}

void ModelManager::Update(const Mobile& /*mobile*/, std::shared_ptr<IModelDisplayState> /*spDisplayState*/)
{
   // the md2 models are so simple that we can't reflect changes in the mobile on the model
}

void ModelManager::LoadModelData(const CString& cszModelName)
{
   // construct filename for tris.md2
   CString cszFilename = _T("models\\md2\\") + cszModelName + _T("\\tris.md2");

   std::shared_ptr<Stream::IStream> spStream = m_fileSystem.OpenFile(cszFilename, true);

   std::shared_ptr<ModelData> spModelData(new ModelData());

   spModelData->Load(*spStream);

   m_mapModelData.insert(std::make_pair(cszModelName, spModelData));
}

void ModelManager::LoadTexture(const CString& cszModelName, const CString& cszTextureName)
{
   CString cszFilename = _T("models\\md2\\") + cszModelName + _T("\\") + cszTextureName + _T(".pcx");

   TextureLoader loader(m_taskManager, m_fileSystem);

   std::shared_ptr<Texture> spTexture(new Texture);
   //spTexture->Generate(); // TODO

   loader.Load(cszFilename, spTexture, false);

   m_textureMap.Register(cszModelName + _T("-") + cszTextureName, spTexture);
}
