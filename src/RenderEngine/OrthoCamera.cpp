//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file OrthoCamera.cpp Orthogonal projection camera
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
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glOrtho(m_dMinX, m_dMaxX, m_dMaxY, m_dMinY, -1.0, 1.0);
   //glTranslated(0.0, 0.0, -1.1);

   glMatrixMode(GL_MODELVIEW);
}
