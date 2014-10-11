//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MD2\ModelData.hpp MD2 model data
//
#pragma once

// includes
#include "NamedTextureMap.hpp"

// forward references
class IFileSystem;
class GraphicsTaskManager;
class IModelDisplayState;
class Mobile;

namespace MD2
{
struct ModelData;

/// model manager for md2 format models
class ModelManager
{
public:
   /// ctor
   ModelManager(IFileSystem& fileSystem, GraphicsTaskManager& taskManager) throw()
      :m_fileSystem(fileSystem),
      m_taskManager(taskManager)
   {
   }
   /// dtor
   ~ModelManager() throw() {}

   /// creates model instance from mobile
   std::shared_ptr<IModelDisplayState> Create(const Mobile& mobile);

   /// updates model instance from mobile, e.g. when inventory changes
   void Update(const Mobile& mobile, std::shared_ptr<IModelDisplayState> spDisplayState);

private:
   /// loads model data
   void LoadModelData(const CString& cszModelName);

   /// loads texture
   void LoadTexture(const CString& cszModelName, const CString& cszTextureName);

private:
   /// file system
   IFileSystem& m_fileSystem;

   /// graphics task manager
   GraphicsTaskManager& m_taskManager;

   /// mapping with all model data
   std::map<CString, std::shared_ptr<ModelData>> m_mapModelData;

   /// texture map for model textures
   NamedTextureMap m_textureMap;
};

} // namespace MD2
