//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Texture.cpp Texture class
//

// includes
#include "stdafx.h"
#include "Texture.hpp"
#include "Math.hpp"
#include "OpenGL.hpp"
#include "Bitmap.hpp"

Texture::~Texture() throw()
{
   if (m_texId != 0)
   {
      glDeleteTextures(1, &m_texId);
      glTraceError("glDeleteTextures");
   }
}

void Texture::Generate()
{
   ATLASSERT(m_texId == 0); // Generate() already called?

   glEnable(GL_TEXTURE_2D);

   glGenTextures(1, &m_texId);
   glTraceError("glGenTextures");
}

void Texture::Bind() const throw()
{
   ATLASSERT(m_texId != 0); // Generate() not called?

   glEnable(GL_TEXTURE_2D);

   glBindTexture(GL_TEXTURE_2D, m_texId);
   glTraceError("glBindTexture");
}

void Texture::Upload(const Bitmap& bmp, bool bGenerateMipmap)
{
   // check if non-power-of-two texture size is used
   if ((!IsPowerOfTwo(bmp.XRes()) || !IsPowerOfTwo(bmp.YRes())) &&
       !OpenGL::IsExtensionSupported(OpenGL::Extension::ARB_texture_non_power_of_two))
   {
      // fail here: modern graphics cards should support this extension
      throw Exception(_T("Non-power-of-two texture size not supported by graphics card"), __FILE__, __LINE__);
   }

   Bind();

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   // upload bits
   const GLubyte* data = &bmp.Data()->m_color[0];

   if (bGenerateMipmap)
   {
      // build mipmapped textures
      gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.XRes(), bmp.YRes(),
         GL_RGBA, GL_UNSIGNED_BYTE, data);

      glTraceError("gluBuild2DMipmaps");

      m_uiSize = unsigned(bmp.XRes() * bmp.YRes() * 4 * 1.3333);
   }
   else
   {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp.XRes(), bmp.YRes(), 0,
         GL_RGBA, GL_UNSIGNED_BYTE, data);

      glTraceError("glTexImage2D");

      m_uiSize = bmp.XRes() * bmp.YRes() * 4;
   }
}

void Texture::UploadEmpty(unsigned int xres, unsigned int yres)
{
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, xres, yres, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
