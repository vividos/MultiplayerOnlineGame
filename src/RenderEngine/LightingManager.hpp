//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file LightingManager.hpp OpenGL scene lighting manager
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "Color.hpp"
#include "Vector3d.hpp"
#include <vector>

namespace OpenGL
{

/// \brief Light source
/// \note currenty not supported: directional and spotlight sources, and
/// attenuation
class RENDERENGINE_DECLSPEC LightSource
{
public:
   /// ctor
   LightSource(const Color& cLight, const Vector3d& vPos) throw();

   // actions

   /// enables light source
   void Enable(unsigned int uiLightName);

   // get methods

   /// returns position of light source
   const Vector3d& Pos() const throw() { return m_vPos; }

   // set methods

   /// sets new position
   void Pos(const Vector3d& vPos) throw() { m_vPos = vPos; }

private:
   Vector3d m_vPos;     ///< light source position
   Color m_cAmbient;    ///< ambient light color
   Color m_cDiffuse;    ///< diffuse light color
   Color m_cSpecular;   ///< specular light color
};

/// \brief Material properties
/// \note to use this class, color tracking must be switched off in the
/// LightingManager class.
/// \note currenty not supported: emission color
class RENDERENGINE_DECLSPEC Material
{
public:
   /// ctor
   Material(Color cMaterial) throw()
      :m_cMaterial(cMaterial),
       m_cSpecular(Color::White()),
       m_uiShininess(0)
   {
   }

   // set methods

   /// set specular reflection properties
   void Specular(Color cSpecular, unsigned int uiShininess) throw()
   {
      ATLASSERT(uiShininess >= 0 && uiShininess <= 128);

      m_cSpecular = cSpecular;
      m_uiShininess = uiShininess;
   }

   // actions

   /// uses material properties
   void Use() const throw();

private:
   Color m_cMaterial;   ///< material color
   Color m_cSpecular;   ///< specular reflection color
   unsigned int m_uiShininess; ///< specular shininess
};

/// \brief Lighting manager
/// \details Manages lighting OpenGL scenes. There can be up to 8 light
/// sources, represented by the LightSource class. There is a global ambient
/// light color for all sources that can e.g. be used for day/night cycles.
/// The manager supports materials by using Material class to specify ambient,
/// diffuse and specular color. Another way to specify material color is to
/// enable color tracking using ColorTracking() and then specifying material
/// color using glColor*() functions. Note that for using Material class,
/// color tracking must be switched off.
/// Call Enable() at start of your scene, and Disable() when lighting should
/// be switched off again (e.g. for UI rendering).
/// \note currenty not supported: local viewpoint (GL_LIGHT_MODEL_LOCAL_VIEWER)
/// \see http://www.falloutsoftware.com/tutorials/gl/gl8.htm
/// \see http://www.sjbaker.org/steve/omniv/opengl_lighting.html
class RENDERENGINE_DECLSPEC LightingManager
{
public:
   /// ctor
   LightingManager() throw();

   // get methods

   /// returns number of light sources (max. 7)
   size_t LightSourceCount() const throw() { return m_vecLightSources.size(); }

   /// returns a light source
   LightSource& Source(size_t uiIndex) throw()
   {
      ATLASSERT(uiIndex < LightSourceCount());
      return m_vecLightSources[uiIndex];
   }

   /// returns if color tracking is enabled
   bool IsEnabledColorTracking() const throw() { return m_bColorTracking; }

   // set methods

   /// sets global ambient light color
   void AmbientLight(Color c) throw() { m_cAmbientLight = c; }

   /// enables or disables color tracking
   void ColorTracking(bool bColorTracking) throw() { m_bColorTracking = bColorTracking; }

   // actions

   /// adds new light source; returns index of light source
   size_t Add(const LightSource& ls);

   /// clears list of light sources
   void Clear();

   /// enables lighting
   void Enable();

   /// disables lighting
   void Disable();

private:
   /// global ambient light color
   Color m_cAmbientLight;

   /// indicates if color tracking is enabled
   bool m_bColorTracking;

   /// light sources
   std::vector<LightSource> m_vecLightSources;
};

} // namespace OpenGL
