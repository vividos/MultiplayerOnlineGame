//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MilkShape3D\Model3dBase.cpp MilkShape3D model base class
//

// includes
#include "StdAfx.h"
#include "Model3dBase.hpp"

extern void AngleMatrix(const MilkShape3D::RotAngle3d& rotation, Matrix4d& matrix);

using namespace MilkShape3D;

int Model3dBase::FindJointByName(const CString& cszJointName) const throw()
{
   for (size_t i=0, iMax=m_data.m_vecJoints.size(); i<iMax; i++)
      if (m_data.m_vecJoints[i].cszName == cszJointName)
         return i;
   return -1;
}

void Model3dBase::SetupJoints()
{
   size_t iMax = m_data.m_vecJoints.size();
   m_data.m_vecJoints.resize(iMax);
   for (size_t i=0; i<iMax; i++)
   {
      Joint& joint = m_data.m_vecJoints[i];

      joint.ParentIndex(FindJointByName(joint.cszParentName));
   }

   for (size_t i=0; i<iMax; i++)
   {
      Joint& joint = m_data.m_vecJoints[i];

      // calc local skeleton matrix
      Matrix4d& matLocalSkeleton = joint.LocalSkeletonMatrix();

      AngleMatrix(joint.rotation, matLocalSkeleton);

      matLocalSkeleton.Row(3, joint.vPosition);

      // calc global skeleton matrix
      Matrix4d& matGlobalSkeleton = joint.GlobalSkeletonMatrix();

      if (joint.ParentIndex() == -1)
      {
         matGlobalSkeleton = matLocalSkeleton;
      }
      else
      {
         ATLASSERT(joint.ParentIndex() >= 0 && size_t(joint.ParentIndex()) < m_data.m_vecJoints.size());

         // multiply with matrix from parent
         size_t uiParentIndex = static_cast<size_t>(joint.ParentIndex());
         const Joint& parentJoint = m_data.m_vecJoints[uiParentIndex];

         Matrix4d::Mult(matGlobalSkeleton,
            parentJoint.GlobalSkeletonMatrix(),
            matLocalSkeleton);
      }
   }
}
