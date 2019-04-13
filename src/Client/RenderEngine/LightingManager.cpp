//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file LightingManager.cpp OpenGL scene lighting manager
//

// includes
#include "StdAfx.h"
#include "LightingManager.hpp"
#include "OpenGL.hpp"

using OpenGL::LightSource;
using OpenGL::LightingManager;
using OpenGL::Material;

/// converts color to GLfloat values, with alpha set to 1.0
inline void ColorToFloat(Color c, GLfloat position[4], bool bCopyAlpha = false)
{
   position[0] = c.m_color[Color::red] / 255.f;
   position[1] = c.m_color[Color::green] / 255.f;
   position[2] = c.m_color[Color::blue] / 255.f;
   position[3] = bCopyAlpha ? (c.m_color[Color::alpha] / 255.f) : 1.f; ;
}


//
// LightSource
//

LightSource::LightSource(const Color& cLight, const Vector3d& vPos)
:m_vPos(vPos),
 m_cAmbient(cLight),//51,51,51),
 m_cDiffuse(cLight),//204,204,204),
 m_cSpecular(cLight)//128,128,128)
{
   // reduce to 10% diffuse color
   m_cAmbient.m_color[Color::red]   /= 10;
   m_cAmbient.m_color[Color::green] /= 10;
   m_cAmbient.m_color[Color::blue]  /= 10;
}

void LightSource::Enable(unsigned int uiLightName)
{
   GLfloat position[4] =
   {
      float(m_vPos.X()),
      float(m_vPos.Y()),
      float(m_vPos.Z()),
      1.0f
   };

   glLightfv(uiLightName, GL_POSITION, position);

   GLfloat ambientLight[4];
   ColorToFloat(m_cAmbient, ambientLight);
   glLightfv(uiLightName, GL_AMBIENT, ambientLight);

   GLfloat diffuseLight[4];
   ColorToFloat(m_cDiffuse, diffuseLight);
   glLightfv(uiLightName, GL_DIFFUSE, diffuseLight);

   GLfloat specularLight[4];
   ColorToFloat(m_cSpecular, specularLight);
   glLightfv(uiLightName, GL_SPECULAR, specularLight);
}


//
// Material
//

/// \details note that only front facing triangles use given material
void Material::Use() const
{
   GLfloat materialColor[4];
   ColorToFloat(m_cMaterial, materialColor, true); // copy alpha as well
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);

   GLfloat specReflection[4];
   ColorToFloat(m_cSpecular, specReflection);
   glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);

   glMateriali(GL_FRONT, GL_SHININESS, GLint(m_uiShininess));
}


//
// LightingManager
//

LightingManager::LightingManager()
:m_cAmbientLight(51,51,51,255), // OpenGL default
 m_bColorTracking(false)
{
}

size_t LightingManager::Add(const LightSource& ls)
{
   ATLASSERT(LightSourceCount() < 8);

   size_t uiNewIndex = m_vecLightSources.size();

   m_vecLightSources.push_back(ls);

   return uiNewIndex;
}

void LightingManager::Clear()
{
   m_vecLightSources.clear();
}

void LightingManager::Enable()
{
   glEnable(GL_LIGHTING);

   glShadeModel(GL_SMOOTH);

   // set ambient light
   GLfloat globalAmbientLight[4];
   ColorToFloat(m_cAmbientLight, globalAmbientLight);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);

   // enable all active light sources
   for (size_t i=0, iMax=m_vecLightSources.size(); i<iMax; i++)
   {
      m_vecLightSources[i].Enable(GL_LIGHT0 + i);
      glEnable(GL_LIGHT0 + i);
   }

   // disable all further light sources
   for (size_t i=m_vecLightSources.size(); i<8; i++)
      glDisable(GL_LIGHT0 + i);

   // color tracking
   if (m_bColorTracking)
   {
      glEnable(GL_COLOR_MATERIAL);
      glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
   }
   else
      glDisable(GL_COLOR_MATERIAL);
}

void LightingManager::Disable()
{
   glDisable(GL_LIGHTING);
}
