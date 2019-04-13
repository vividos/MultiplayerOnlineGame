//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Texture.hpp Texture class
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "OpenGL.hpp"

// forward references
class Bitmap;

/// \brief texture class
class RENDERENGINE_DECLSPEC Texture: public boost::noncopyable
{
public:
   /// ctor
   Texture()
      :m_texId(0),
       m_uiSize(0)
   {
   }

   /// dtor
   ~Texture();

   /// generates texture id
   void Generate();

   /// binds current texture for use
   void Bind() const;

   /// uploads bitmap to texture
   void Upload(const Bitmap& bmp, bool bGenerateMipmap = false);

   /// uploads empty bitmap
   void UploadEmpty(unsigned int xres, unsigned int yres);

   // get functions

   /// returns if texture object is valid
   bool IsValid() const { return m_texId != 0; }

   /// returns texture id
   unsigned int Id() const { return m_texId; }

   /// returns size of texture in video ram
   unsigned int Size() const { return m_uiSize; }

private:
   GLuint m_texId;         ///< texture id
   unsigned int m_uiSize;  ///< size of texture in video ram
};

/// shared pointer for Texture
typedef std::shared_ptr<Texture> TexturePtr;
