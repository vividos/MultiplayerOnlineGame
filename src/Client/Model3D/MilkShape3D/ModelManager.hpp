//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\ModelManager.hpp MilkShape3D model manager
//
#pragma once

// includes
#include "NamedTextureMap.hpp"
#include "IModelDisplayState.hpp"
#include "Model3dCache.hpp"

// forward references
class IFileSystem;
class GraphicsTaskManager;
class TextureLoader;
class Mobile;
namespace Stream
{
   class IStream;
}

namespace MilkShape3D
{
class StaticModel3d;
class AnimatedModel3d;
class CompositeModel3d;
class ModelDisplayState;

/// blueprint with all needed infos to load/reload a composite model
struct ModelBlueprint
{
   /// model name
   CString m_cszAnimatedModelName;

   /// texture names for each material of the animated model
   std::vector<CString> m_vecAnimatedMaterialTextures;

   /// blueprint with all needed infos to load a static model
   struct StaticModelBlueprint
   {
      /// ctor
      StaticModelBlueprint(const CString& cszStaticModelName,
         const CString& cszMountJoint, const CString& cszStaticTexturePrefix = _T(""))
         :m_cszStaticModelName(cszStaticModelName),
          m_cszStaticTexturePrefix(cszStaticTexturePrefix),
          m_cszMountJoint(cszMountJoint)
      {
      }

      /// static model name
      CString m_cszStaticModelName;

      /// prefix for texture name of static model
      CString m_cszStaticTexturePrefix;

      /// name of joint to mount static model to, in animated model
      CString m_cszMountJoint;
   };

   /// all static models
   std::vector<StaticModelBlueprint> m_vecStaticBlueprints;
};

/// \brief MilkShape3D model manager
/// \details Manages loading and combining models for mobiles. The ModelManager
/// supports animated models (using AnimatedModel3d), combined with one or more
/// static objects (using StaticModel3d) mounted on mount points (all stored in
/// CompositeModel3d). The combination of animated model and static models are
/// determined by a ModelBlueprint class that stores combinations.
/// Since there canbe multiple models using the same model data, the manager uses
/// an instance of IModelDisplayState (the class ModelDisplayState) to manage the
/// data that is different for each model.
/// \todo implement a cache of already loaded model files
class ModelManager
{
public:
   /// ctor
   ModelManager(IFileSystem& fileSystem, GraphicsTaskManager& taskManager);
   /// dtor
   ~ModelManager() {}

   /// creates model instance from mobile
   std::shared_ptr<IModelDisplayState> Create(const Mobile& mobile);

   /// updates model instance from mobile, e.g. when inventory changes
   void Update(const Mobile& mobile, std::shared_ptr<IModelDisplayState> spIDisplayState);

private:
   friend class ModelDisplayState;

   /// loads model in background
   void AsyncLoadModel(const ModelBlueprint& blueprint,
      std::shared_ptr<CompositeModel3d> spModel,
      std::shared_ptr<ModelDisplayState> spDisplayState);

   /// updates model in background, with new blueprint
   void AsyncUpdateModel(const ModelBlueprint& blueprint,
      std::shared_ptr<ModelDisplayState> spDisplayState);

   /// creates a model blueprint from a mobile
   void CreateBlueprintFromMobile(const Mobile& mobile, ModelBlueprint& blueprint);

   /// loads a model blueprint
   void LoadModelBlueprint(const ModelBlueprint& blueprint, std::shared_ptr<CompositeModel3d> spModel);

   /// loads static model
   std::shared_ptr<StaticModel3d> LoadStatic(const CString& cszName);

   /// loads animated model
   std::shared_ptr<AnimatedModel3d> LoadAnimated(const CString& cszName);

   /// loads texture by name
   TexturePtr LoadTexture(const CString& cszTexture);

   /// returns model or texture data stream by filename
   std::shared_ptr<Stream::IStream> GetDataStream(CString cszFilename);

private:
   /// file system
   IFileSystem& m_fileSystem;

   /// task manager
   GraphicsTaskManager& m_taskManager;

   /// texture loader
   std::shared_ptr<TextureLoader> m_spTextureLoader;

   /// texture map for all model textures
   NamedTextureMap m_textureMap;

   /// cache for AnimatedModel3d objects
   Model3dCache<AnimatedModel3d> m_animatedModelCache;

   /// cache for StaticModel3d objects
   Model3dCache<StaticModel3d> m_staticModelCache;
};

} // namespace MilkShape3D
