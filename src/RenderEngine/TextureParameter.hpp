//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TextureParameter.hpp OpenGL texture parameter class
//
#pragma once

// includes
#include "OpenGL.hpp"

/// \brief represents texture parameters common to one or many textures
/// \details best use as typedef and call the static Use() function
/// \tparam wrapTypeS wrap type for X (or U) axis
/// \tparam wrapTypeT wrap type for Y (or V) axis
/// \tparam filterTypeMag filter type for magnification
/// \tparam filterTypeMin filter type for minification
/// \tparam envMode environment mapping mode
template <GLuint wrapTypeS, GLuint wrapTypeT, GLuint filterTypeMag, GLuint filterTypeMin, GLuint envMode>
class TextureParameter
{
   static_assert(
      wrapTypeS == GL_CLAMP ||
      wrapTypeS == GL_CLAMP_TO_BORDER ||
      wrapTypeS == GL_CLAMP_TO_EDGE ||
      wrapTypeS == GL_MIRRORED_REPEAT ||
      wrapTypeS == GL_REPEAT, "not an allowed value for wrapTypeS");

   static_assert(
      wrapTypeT == GL_CLAMP ||
      wrapTypeT == GL_CLAMP_TO_BORDER ||
      wrapTypeT == GL_CLAMP_TO_EDGE ||
      wrapTypeT == GL_MIRRORED_REPEAT ||
      wrapTypeT == GL_REPEAT, "not an allowed value for wrapTypeT");

   static_assert(
      filterTypeMag == GL_NEAREST ||
      filterTypeMag == GL_LINEAR, "not an allowed value for filterTypeMag");

   static_assert(
      filterTypeMin == GL_NEAREST ||
      filterTypeMin == GL_LINEAR ||
      filterTypeMin == GL_NEAREST_MIPMAP_NEAREST ||
      filterTypeMin == GL_NEAREST_MIPMAP_LINEAR ||
      filterTypeMin == GL_LINEAR_MIPMAP_NEAREST ||
      filterTypeMin == GL_LINEAR_MIPMAP_LINEAR, "not an allowed value for filterTypeMin");

   static_assert(
      envMode == GL_ADD ||
      envMode == GL_MODULATE ||
      envMode == GL_DECAL ||
      envMode == GL_BLEND ||
      envMode == GL_REPLACE ||
      envMode == GL_COMBINE, "not an allowed value for envMode");

public:
   /// activate texture parameter
   static void Use() throw()
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapTypeS);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTypeT);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterTypeMag);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterTypeMin);

      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, envMode);
   }
};
