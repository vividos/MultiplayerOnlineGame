//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Texture.hpp Texture class
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
   Texture() throw()
      :m_texId(0),
       m_uiSize(0)
   {
   }

   /// dtor
   ~Texture() throw();

   /// generates texture id
   void Generate();

   /// binds current texture for use
   void Bind() const throw();

   /// uploads bitmap to texture
   void Upload(const Bitmap& bmp, bool bGenerateMipmap = false);

   // get functions

   /// returns if texture object is valid
   bool IsValid() const throw() { return m_texId != 0; }

   /// returns texture id
   unsigned int Id() const throw() { return m_texId; }

   /// returns size of texture in video ram
   unsigned int Size() const throw() { return m_uiSize; }

private:
   GLuint m_texId;         ///< texture id
   unsigned int m_uiSize;  ///< size of texture in video ram
};

/// shared pointer for Texture
typedef std::shared_ptr<Texture> TexturePtr;
