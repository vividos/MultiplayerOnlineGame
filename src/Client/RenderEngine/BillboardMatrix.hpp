//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BillboardMatrix.hpp Billboard sprite matrix classes
//
#pragma once

// includes
#include "Vector3d.hpp"
#include "Matrix4d.hpp"

/// base class for billboard matrix
class BillboardMatrixBase
{
public:
   /// ctor
   BillboardMatrixBase()
      :m_mBillboard(Matrix4d::Identity())
   {
   }

   /// applies matrix to model view matrix
   void ApplyMatrix()
   {
      glMultMatrixd(m_mBillboard.Data());
   }

   /// calculates camera pos and up vector
   static void GetCameraPosAndUp(Vector3d& vCameraPos, Vector3d& vCameraUp);

protected:
   /// billboard model view matrix
   Matrix4d m_mBillboard;
};

/// point billboard sprite
class PointBillboardMatrix: public BillboardMatrixBase
{
public:
   /// ctor
   PointBillboardMatrix(const Vector3d& vPos)
      :m_vPos(vPos)
   {
   }

   /// updates camera pos and up vector
   void Update(const Vector3d& vCameraPos, const Vector3d& vCameraUp)
   {
      // create look vector
      Vector3d vLook = vCameraPos - m_vPos;
      vLook.Normalize();

      // right hand rule cross product
      Vector3d vRight = vCameraUp.Cross(vLook);
      Vector3d vUp = vLook.Cross(vRight);

      m_mBillboard.Column(0, vRight);
      m_mBillboard.Column(1, vUp);
      m_mBillboard.Column(2, vLook);
      m_mBillboard.Column(3, m_vPos);
   }

private:
   /// billboard sprite pos
   Vector3d m_vPos;
};

/// axis-aligned billboard sprite
class AABillboardSprite: public BillboardMatrixBase
{
public:
   AABillboardSprite(const Vector3d& vPos, const Vector3d& vAxis);
};
