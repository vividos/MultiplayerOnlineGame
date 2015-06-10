//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\CompositeModel3d.cpp MilkShape3D composite model
//

// includes
#include "StdAfx.h"
#include "CompositeModel3d.hpp"
#include "AnimatedModel3d.hpp"
#include "StaticModel3d.hpp"
#include <boost/foreach.hpp>

using namespace MilkShape3D;

void CompositeModel3d::Prepare() throw()
{
   ATLASSERT(m_spAnimated != NULL);
   m_spAnimated->Prepare();

   BOOST_FOREACH(const StaticModelData& modelData, m_vecStatics)
      modelData.m_spStatic->Prepare();
}

void CompositeModel3d::Upload() throw()
{
   ATLASSERT(m_spAnimated != NULL);
   m_spAnimated->Upload();

   BOOST_FOREACH(const StaticModelData& modelData, m_vecStatics)
      modelData.m_spStatic->Upload();
}

void CompositeModel3d::AddStatic(LPCTSTR pszMountJoint, std::shared_ptr<StaticModel3d> spStatic) throw()
{
   ATLASSERT(m_spAnimated != NULL);

   int iJointIndex = m_spAnimated->FindMountJointIndex(pszMountJoint);

   m_vecStatics.push_back(StaticModelData(iJointIndex, spStatic));
}
