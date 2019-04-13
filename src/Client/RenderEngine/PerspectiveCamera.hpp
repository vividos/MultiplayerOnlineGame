//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file PerspectiveCamera.hpp Perspective camera class
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "ICamera.hpp"
#include "Vector3.hpp"
#include "ViewFrustum3d.hpp"

/// perspective camera
class RENDERENGINE_DECLSPEC PerspectiveCamera : public ICamera
{
public:
   /// ctor
   PerspectiveCamera();
   /// dtor
   virtual ~PerspectiveCamera() {}

   // virtual methods from ICamera

   virtual void Use() override;

   // set methods

   /// sets camera position by position and point to look at
   void SetPositionLookAt(const Vector3d& vPos, const Vector3d& vLookAt);

   /// sets camera position by position and direction and up angles
   void SetPosition(const Vector3d& vPos, double dAngleDirection, double dAngleUp);

   /// sets near and far plane distance
   void SetNearFarPlaneDistance(double dNearPlaneDist, double dFarPlaneDist);

   // get methods

   /// returns position
   const Vector3d& GetPosition() const { return m_vPos; }

   /// returns direction angle
   double GetAngleDirection() const { return m_dAngleDirection; }

   /// returns up angle
   double GetAngleUp() const { return m_dAngleUp; }

   /// returns view frustum
   ViewFrustum3d GetViewFrustum() const;

private:
   Vector3d m_vPos;           ///< camera position
   double m_dAngleDirection;  ///< direction angle
   double m_dAngleUp;         ///< up angle
   double m_dNearPlaneDist;   ///< near plane distance
   double m_dFarPlaneDist;    ///< far plane distance
};
