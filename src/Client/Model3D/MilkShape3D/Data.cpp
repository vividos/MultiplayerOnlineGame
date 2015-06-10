//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Data.cpp MilkShape3D data classes
//

// includes
#include "StdAfx.h"
#include "Data.hpp"
#include "OpenGL.hpp"

using namespace MilkShape3D;

void Material::Use() const
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, afAmbient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, afDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, afSpecular);
   glMaterialfv(GL_FRONT, GL_EMISSION, afEmissive);
   glMaterialf(GL_FRONT, GL_SHININESS, fShininess);
   //fTransparency: not supported
}
