//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RenderOptions.hpp Render options class
//
#pragma once

// includes
#include <array>

/// collection of rendering options
class RenderOptions
{
public:
   /// render option for on/off settings
   enum T_enRenderOption
   {
      // general
      optionRenderUI,            ///< render UI
      optionCullBackface,        ///< cull backfaces?
      optionBackfaceAsLines,     ///< when backfaces are rendered, render as lines?
      optionRenderLighting,      ///< use lighting manager
      optionRenderViewFrustum,   ///< render view frustum

      // model related
      optionModelFilled,      ///< indicates if model is rendered filled
      optionModelBoundingBox, ///< indicates if model bounding box is rendered
      optionModelJoints,      ///< indicates if model joints are drawn
      optionModelNormals,     ///< indicates if model normals are rendered

      // sky related
      optionSkyMeshLines,     ///< indicates if skydome mesh lines are drawn

      // terrain related
      optionTerrainFilled,          ///< render terrain filled, not wireframe
      optionTerrainZoneBoundingBox, ///< render terrain zone bounding boxes
      optionTerrainFrustumCulling,  ///< perform frustum culling invisible zones
      optionTerrainModelNormals,    ///< indicates if terrain normals are rendered

      optionMax ///< last option
   };

   /// ctor
   RenderOptions()
   {
      std::uninitialized_fill(m_aOptions.begin(), m_aOptions.end(), true);

      Set(optionBackfaceAsLines, false);
      Set(optionRenderViewFrustum, false);

      Set(optionModelBoundingBox, false);
      Set(optionModelJoints, false);
      Set(optionModelNormals, false);

      Set(optionSkyMeshLines, false);

      Set(optionTerrainZoneBoundingBox, false);
      Set(optionTerrainModelNormals, false);
   }

   // get methods

   /// returns boolean render option
   bool Get(T_enRenderOption enRenderOption) const throw()
   {
      ATLASSERT(enRenderOption < optionMax);
      return m_aOptions[enRenderOption];
   }

   // set methods

   /// sets boolean render option
   void Set(T_enRenderOption enRenderOption, bool bNewValue) throw()
   {
      ATLASSERT(enRenderOption < optionMax);
      m_aOptions[enRenderOption] = bNewValue;
   }

private:
   /// boolean render options
   std::array<bool, optionMax> m_aOptions;
};
