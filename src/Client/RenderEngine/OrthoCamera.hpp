//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file OrthoCamera.hpp Orthogonal projection camera
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "ICamera.hpp"

/// orthogonal projection camera
class RENDERENGINE_DECLSPEC OrthoCamera : public ICamera
{
public:
   /// ctor
   OrthoCamera(double dMinX, double dMaxX, double dMinY, double dMaxY);
   /// dtor
   virtual ~OrthoCamera(){}

   /// activates camera
   virtual void Use() override;

private:
   double m_dMinX; ///< min x value
   double m_dMaxX; ///< max x value
   double m_dMinY; ///< min y value
   double m_dMaxY; ///< max y value
};
