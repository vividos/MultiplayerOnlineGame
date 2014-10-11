//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\ModelManager.cpp MilkShape3D model manager
//

// includes
#include "StdAfx.h"
#include "ModelManager.hpp"
#include "GraphicsTaskManager.hpp"
#include "Mobile.hpp"
#include "CompositeModel3d.hpp"
#include "StaticModel3d.hpp"
#include "AnimatedModel3d.hpp"
#include "ModelDisplayState.hpp"
#include "Data.hpp"
#include "Loader.hpp"
#include "IFileSystem.hpp"
#include "TextureLoader.hpp"
#include "Ms3dxFileStreamDecoder.hpp"
#include <boost/foreach.hpp>

#ifdef _DEBUG
#include "TraceOutputStopwatch.hpp"
#include "HighResolutionTimer.hpp"
#endif

using namespace MilkShape3D;

/// decoder key
const BYTE c_aMs3dxDecoderKey[] =
{
   240, 59, 117, 142, 148, 192, 244, 244, 25, 47, 54, 229, 223, 162, 66, 201, 28, 251, 16,
};

ModelManager::ModelManager(IFileSystem& fileSystem, GraphicsTaskManager& taskManager)
:m_fileSystem(fileSystem),
 m_taskManager(taskManager),
 m_spTextureLoader(new TextureLoader(taskManager, fileSystem))
{
}

std::shared_ptr<IModelDisplayState> ModelManager::Create(const Mobile& mobile)
{
   ModelBlueprint blueprint;
   CreateBlueprintFromMobile(mobile, blueprint);

   std::shared_ptr<CompositeModel3d> spModel(new CompositeModel3d);

   std::shared_ptr<ModelDisplayState> spDisplayState(
      new ModelDisplayState(spModel));

   // load in background
   m_taskManager.BackgroundTaskGroup().Add(
      std::bind(&ModelManager::AsyncLoadModel, this, blueprint, spModel, spDisplayState));

   return spDisplayState;
}

void ModelManager::Update(const Mobile& mobile, std::shared_ptr<IModelDisplayState> /*spDisplayState*/)
{
   ModelBlueprint blueprint;
   CreateBlueprintFromMobile(mobile, blueprint);

   // start loading/replacing in background, using ModelBlueprint
   // TODO implement
}

void ModelManager::AsyncLoadModel(const ModelBlueprint& blueprint,
                                  std::shared_ptr<CompositeModel3d> spModel,
                                  std::shared_ptr<ModelDisplayState> spDisplayState)
{
#ifdef _DEBUG
   TraceOutputStopwatch<HighResolutionTimer> stopwatch(_T("ModelManager::AsyncLoadModel"));
#endif

   LoadModelBlueprint(blueprint, spModel);

   spDisplayState->Prepare(*this, blueprint);

   m_taskManager.UploadTaskGroup().Add(
      std::bind(&ModelDisplayState::Upload, spDisplayState));
}

void ModelManager::CreateBlueprintFromMobile(const Mobile& mobile, ModelBlueprint& blueprint)
{
   const MobileDisplayInfo& info = mobile.DisplayInfo();

   // info.BaseFigure(): we only have this one yet
   blueprint.m_cszAnimatedModelName = _T("CS_muscle_animated");
   blueprint.m_vecAnimatedMaterialTextures.resize(5);

   // material 0, name [feet_texture] texture [.\base.feet.human.jpg]
   // material 1, name [pants_texture] texture [.\base.pants.human.jpg]
   // material 2, name [body_texture] texture [.\base.body.human.jpg]
   // material 3, name [hands_texture] texture [.\base.hands.human.jpg]
   // material 4, name [head_texture] texture [.\base.head.human.jpg]
   blueprint.m_vecAnimatedMaterialTextures[0] = _T("001.feet.human.jpg");
   blueprint.m_vecAnimatedMaterialTextures[1] = _T("001.pants.human.jpg");
   blueprint.m_vecAnimatedMaterialTextures[2] = _T("001.body.human.jpg");
   blueprint.m_vecAnimatedMaterialTextures[3] = _T("001_D.hands.human.jpg");

   blueprint.m_vecAnimatedMaterialTextures[4].Format(
      _T("%02u_%01u.head.human.jpg"), info.FaceStyle()+1, info.SkinColor()+1);

   CString cszPilosity;
   cszPilosity.Format(_T("pilosity%02u_%01u"),
      info.PilosityBrowStyle()+1,
      info.PilosityHairStyle()+1);

   CString cszHairColor;
   cszHairColor.Format(_T("%u."), info.HairColor()+1);

   blueprint.m_vecStaticBlueprints.push_back(ModelBlueprint::StaticModelBlueprint(cszPilosity, _T("mount1"), cszHairColor));

   // TODO process inventory/paperdoll
   blueprint.m_vecStaticBlueprints.push_back(ModelBlueprint::StaticModelBlueprint(_T("axe01"), _T("mount3")));
   //blueprint.m_vecStaticBlueprints.push_back(ModelBlueprint::StaticModelBlueprint(_T("hat01"), _T("mount0")));
}

void ModelManager::LoadModelBlueprint(const ModelBlueprint& blueprint, std::shared_ptr<CompositeModel3d> spModel)
{
   std::shared_ptr<AnimatedModel3d> spAnimated(new AnimatedModel3d);
   LoadAnimated(blueprint.m_cszAnimatedModelName, *spAnimated);

   spModel->SetAnimated(spAnimated);

   BOOST_FOREACH(const ModelBlueprint::StaticModelBlueprint& staticBlueprint, blueprint.m_vecStaticBlueprints)
   {
      std::shared_ptr<StaticModel3d> spStatic(new StaticModel3d);
      LoadStatic(staticBlueprint.m_cszStaticModelName, *spStatic);

      spModel->AddStatic(staticBlueprint.m_cszMountJoint, spStatic);
   }
}

void ModelManager::LoadStatic(const CString& cszName, StaticModel3d& model)
{
   Loader loader(model.GetData());

   CString cszFilename = _T("models\\objects\\") + cszName + _T(".ms3d");

   loader.Load(*GetDataStream(cszFilename));

   model.Prepare();
}

void ModelManager::LoadAnimated(const CString& cszName, AnimatedModel3d& model)
{
   Loader loader(model.GetData());

   CString cszFilename = _T("models\\chars\\") + cszName + _T(".ms3d");

   loader.Load(*GetDataStream(cszFilename));

   model.Prepare();
}

TexturePtr ModelManager::LoadTexture(const CString& cszTexture)
{
   if (m_textureMap.IsTextureAvail(cszTexture))
      return m_textureMap.Get(cszTexture);

   TexturePtr spTexture(new Texture);

   m_textureMap.Register(cszTexture, spTexture);

   // load texture
   m_spTextureLoader->Load(GetDataStream(_T("models\\textures\\") + cszTexture), _T(".jpg"), spTexture, false);

   return spTexture;
}

std::shared_ptr<Stream::IStream> ModelManager::GetDataStream(CString cszFilename)
{
   bool bEncodedAvail = m_fileSystem.IsFileAvail(cszFilename + _T("x"));

   if (bEncodedAvail)
      cszFilename += _T("x");

   std::shared_ptr<Stream::IStream> spStream = m_fileSystem.OpenFile(cszFilename);

   if (!bEncodedAvail)
      return spStream;

   spStream.reset(
      new Ms3dxFileStreamDecoder(
      spStream,
      std::initializer_list<BYTE>(std::begin(c_aMs3dxDecoderKey), std::end(c_aMs3dxDecoderKey))));

   return spStream;
}
