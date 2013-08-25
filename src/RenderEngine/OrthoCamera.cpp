//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file OrthoCamera.cpp Orthogonal projection camera
//

// includes
#include "StdAfx.h"
#include "OrthoCamera.hpp"

OrthoCamera::OrthoCamera(double dMinX, double dMaxX, double dMinY, double dMaxY)
:m_dMinX(dMinX),
 m_dMaxX(dMaxX),
 m_dMinY(dMinY),
 m_dMaxY(dMaxY)
{
}

void OrthoCamera::Use()
{
   glOrtho(m_dMinX, m_dMaxX, m_dMaxY, m_dMinY, 1.0, 1000.0);
   glTranslated(0.0, 0.0, -1.1);
}
