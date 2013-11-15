//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MilkShape3D\CompositeModel3d.hpp MilkShape3D composite model
//
#pragma once

// includes
#include "IModel3d.hpp"

namespace MilkShape3D
{
// forward references
class AnimatedModel3d;
class StaticModel3d;

/// model composed of single AnimatedModel3d and zero or more StaticModel3d
class CompositeModel3d: public IModel3d
{
public:
   /// static model data
   struct StaticModelData
   {
      /// ctor
      StaticModelData(int iJointIndex, std::shared_ptr<StaticModel3d> spStatic)
         :m_iJointIndex(iJointIndex),
          m_spStatic(spStatic)
      {
      }

      /// joint index
      int m_iJointIndex;

      /// static model
      std::shared_ptr<StaticModel3d> m_spStatic;
   };

   /// ctor
   CompositeModel3d() throw() {}
   /// dtor
   virtual ~CompositeModel3d() throw() {}

   // virtual methods from IModel3d

   /// prepares model for rendering
   virtual void Prepare() throw() override;

   /// uploads data to graphics card
   virtual void Upload() throw() override;

   /// sets animated model
   void SetAnimated(std::shared_ptr<AnimatedModel3d> spAnimated) throw() { m_spAnimated = spAnimated; }

   /// returns animated model
   std::shared_ptr<AnimatedModel3d> GetAnimated() throw() { return m_spAnimated; }

   /// adds static model
   void AddStatic(LPCTSTR pszMountJoint, std::shared_ptr<StaticModel3d> spStatic) throw();

   /// returns list of static models
   const std::vector<StaticModelData>& StaticList() const throw() { return m_vecStatics; }

private:
   /// animated model
   std::shared_ptr<AnimatedModel3d> m_spAnimated;

   /// static model data list
   std::vector<StaticModelData> m_vecStatics;
};

} // namespace MilkShape3D
